#!/usr/bin/env python3
"""M37732 / NC77 binary -> C lifter.

Builds on d77.py's analysis (M/X flag tracking, jump/switch tables, function list)
and emits one C function per firmware function.  The output models the CPU
registers as globals (A, B, X, Y, S, D, DT, flags) so it is behaviour-preserving,
but uses the static M/X state of every instruction to pick 8- or 16-bit
operations, removes dead flag computations (per-function flag liveness) and
fuses compare+branch pairs into plain C conditions.

usage: lift.py <file.HEX> <outdir>
"""
import sys, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import d77

# ------------------------------------------------------------------ names
NAMES = {
    0x01A6D5: 'c_main', 0x00E305: 'sys_init', 0x00713E: 'i2c_init',
    0x00D3B4: 'wait_synth_lock', 0x00D45C: 'synth_load', 0x00D51D: 'synth_load_and_wait',
    0x00D2A3: 'synth_select_rx_tx_words', 0x00D2F6: 'synth_check_lock',
    0x00F854: 'ctcss_tx_start', 0x00F8AE: 'ctcss_tx_stop', 0x00F8BD: 'ctcss_rx_start',
    0x00E65B: 'selcall_send', 0x00F3E8: 'radio_state_machine', 0x0198D5: 'measurements',
    0x00CFC4: 'tx_power_loop', 0x00D1C8: 'tx_power_protect',
    0x007189: 'eeprom_read_byte', 0x0071F0: 'eeprom_read_word',
    0x0072EE: 'i2c_block_xfer_a', 0x0073C6: 'i2c_block_xfer_b',
    0x00715B: 'i2c_stop', 0x007172: 'i2c_start', 0x006A68: 'i2c_send_byte',
    0x006AC2: 'i2c_send_byte_fast', 0x006EB3: 'i2c_recv_byte', 0x006FA0: 'i2c_recv_byte_fast',
    0x0087A7: 'channel_is_programmed', 0x00350F: 'channel_record_copy',
    0x007E4C: 'cbus_write_sel1', 0x00755E: 'cbus_write_sel2',
    0x010630: 'set_options', 0x01A664: 'idle_check',
    0x01E388: 'rt_far_memcpy_init', 0x01E3BA: 'rt_far_memcpy', 0x01E466: 'rt_memcpy',
    0x01E3EA: 'rt_lmul', 0x01E414: 'rt_ludiv', 0x01E43E: 'rt_lumod', 0x01E1A6: 'rt_ldiv',
    0x01E214: 'rt_lmod', 0x01E2C6: 'rt_ldiv_core', 0x01E49E: 'rt_lshr', 0x01E48E: 'rt_lsar',
    0x01E4E0: 'rt_shl', 0x01E4C2: 'rt_call_ptr',
}
VECNAME = {}

def fname(d, a):
    if a in NAMES: return NAMES[a] if a in (0x01A6D5,) else '%s_%06X' % (NAMES[a], a)
    if a in VECNAME: return VECNAME[a]
    if a == 0x80: return 'ramcode_flash_loader'
    return 'F_%06X' % a

# ------------------------------------------------------------------ flag effects
NZ = {'N', 'Z'}; ALLF = {'N', 'Z', 'C', 'V'}
DEF_NZ = {'LDA', 'LDX', 'LDY', 'AND', 'ORA', 'EOR', 'INC', 'DEC', 'INX', 'INY', 'DEX', 'DEY',
          'TAX', 'TAY', 'TXA', 'TYA', 'TXY', 'TYX', 'TSX', 'TSA', 'TDA', 'TBX', 'TBY', 'TXB', 'TYB',
          'TSB', 'TDB', 'PLA', 'PLB', 'PLX', 'PLY', 'XAB', 'MPY', 'DIV', 'RLA'}
BR_USE = {'BEQ': ('Z', False), 'BNE': ('Z', True), 'BCS': ('C', False), 'BCC': ('C', True),
          'BMI': ('N', False), 'BPL': ('N', True), 'BVS': ('V', False), 'BVC': ('V', True)}

def flag_use_def(i, mem, a, runtime_ret):
    n = i['name']
    if n in BR_USE: return {BR_USE[n][0]}, set()
    if n in ('ADC', 'SBC', 'ROL', 'ROR'): return {'C'}, set(ALLF if n in ('ADC', 'SBC') else {'N', 'Z', 'C'})
    if n in ('CMP', 'CPX', 'CPY', 'ASL', 'LSR'): return set(), {'N', 'Z', 'C'}
    if n in DEF_NZ: return set(), set(NZ)
    if n in ('CLC', 'SEC'): return set(), {'C'}
    if n == 'CLV': return set(), {'V'}
    if n in ('PHP',): return set(ALLF), set()
    if n in ('PLP',): return set(), set(ALLF)
    if n == 'PUL' and mem[a + 1] & 0x80: return set(), set(ALLF)
    if n == 'PSH' and mem[a + 1] & 0x80: return set(ALLF), set()
    if n in ('SEP', 'CLP'):
        v = mem[a + 1]; s = set()
        if v & 1: s.add('C')
        if v & 2: s.add('Z')
        if v & 0x40: s.add('V')
        if v & 0x80: s.add('N')
        return set(), s
    if n in ('JSR', 'JSRL'):
        return set(), set(ALLF)          # callee defines flags
    if n in ('RTS', 'RTL'):
        return (set(ALLF) if runtime_ret else set()), set()
    return set(), set()

# ------------------------------------------------------------------ helpers
def hx(v, w): return '0x%0*X' % (w * 2, v)

def strip(c):
    # remove one pair of redundant outer parentheses
    if c.startswith('(') and c.endswith(')'):
        depth = 0
        for k, ch in enumerate(c):
            depth += ch == '('; depth -= ch == ')'
            if depth == 0 and k < len(c) - 1: return c
        return c[1:-1]
    return c

def negate(c):
    c = strip(c)
    for a_, b_ in ((' == ', ' != '), (' != ', ' == '), (' >= ', ' < '), (' < ', ' >= ')):
        if c.count(a_) == 1 and c.count(' ') <= 6 and not any(o in c for o in (' && ', ' || ')):
            l, r = c.split(a_)
            if l.count('(') == l.count(')') and r.count('(') == r.count(')'): return l + b_ + r
    return '!(%s)' % c

class Lifter:
    def __init__(s, d):
        s.d = d; s.mem = d.mem
        s.unknown = collections.Counter()
        sig = bytes.fromhex('A305AA688303F868830468DA48D86B')   # NC77 indirect-call helper
        s.callptr = {a for a in d.mem if d.mem.get(a) == 0xA3 and bytes(d.mem.get(a + k, 0) for k in range(len(sig))) == sig}

    def body(s, entry):
        """instructions reachable from entry inside one C function"""
        d = s.d; seen = set(); work = [entry]
        while work:
            a = work.pop()
            if a in seen or a not in d.insn: continue
            seen.add(a); i = d.insn[a]; nxt = a + i['len']
            for t in s.succ(a, i): work.append(t)
        return sorted(seen)

    def succ(s, a, i):
        d = s.d; nxt = a + i['len']; f = i['flow']; out = []
        if f == 'call':
            t = i['tg'][0] if i['tg'] else None
            if t in d.switch_helpers and nxt in d.swtab: return list(d.swtab[nxt][1])
            if t in getattr(d, 'noreturn', ()): return []
            return [nxt]
        if f in ('seq',): return [nxt]
        if f == 'cond': return [nxt] + i['tg']
        if f == 'uncond': return list(i['tg'])
        if f == 'jtab':
            t = i['tg'][0]
            return list(d.jtables.get(t, (0, []))[1])
        if f == 'calltab': return [nxt]
        return []

    # ---------------------------------------------------------- operands
    def opstart(s, a):
        return a + 2 if s.mem[a] in (0x42, 0x89) else a + 1

    def b(s, a): return s.mem[a]
    def w(s, a): return s.mem[a] | s.mem[a + 1] << 8
    def l(s, a): return s.mem[a] | s.mem[a + 1] << 8 | s.mem[a + 2] << 16

    def absname(s, v):
        if v in d77.SFR: return d77.SFR[v]
        return '0x%04X' % v

    def ea(s, mode, p):
        """C expression of effective address for memory modes"""
        if mode in ('dp', 'bitdp', 'bbdp', 'ldmdp'): return 'DP(0x%02X)' % s.b(p)
        if mode in ('dpx', 'ldmdpx'): return 'DP(0x%02X + X)' % s.b(p)
        if mode == 'dpy': return 'DP(0x%02X + Y)' % s.b(p)
        if mode == 'ind': return 'IND(0x%02X)' % s.b(p)
        if mode == 'indx': return 'IND(0x%02X + X)' % s.b(p)
        if mode == 'indy': return '(IND(0x%02X) + Y)' % s.b(p)
        if mode == 'lind': return 'LIND(0x%02X)' % s.b(p)
        if mode == 'lindy': return '(LIND(0x%02X) + Y)' % s.b(p)
        if mode == 'sr': return 'SR(0x%02X)' % s.b(p)
        if mode == 'sry': return '(SRIND(0x%02X) + Y)' % s.b(p)
        if mode in ('abs', 'bitabs', 'bbabs', 'ldmabs'): return 'ABS(%s)' % s.absname(s.w(p))
        if mode in ('absx', 'ldmabsx'): return '(ABS(%s) + X)' % s.absname(s.w(p))
        if mode == 'absy': return '(ABS(%s) + Y)' % s.absname(s.w(p))
        if mode == 'long': return '0x%06X' % s.l(p)
        if mode == 'longx': return '(0x%06X + X)' % s.l(p)
        raise KeyError(mode)

    def opsize(s, mode):
        return {'dp': 1, 'dpx': 1, 'dpy': 1, 'ind': 1, 'indx': 1, 'indy': 1, 'lind': 1, 'lindy': 1,
                'sr': 1, 'sry': 1, 'abs': 2, 'absx': 2, 'absy': 2, 'long': 3, 'longx': 3,
                'bitdp': 1, 'bbdp': 1, 'ldmdp': 1, 'ldmdpx': 1, 'bitabs': 2, 'bbabs': 2,
                'ldmabs': 2, 'ldmabsx': 2}[mode]

    # ---------------------------------------------------------- emit one insn
    def emit(s, a, i, m, x, fn_labels, live_out, fuse):
        """returns (lines, flagexpr) ; flagexpr maps flag -> C expr valid right after"""
        mem = s.mem; n = i['name']; mode = i['mode']; p = s.opstart(a)
        reg = i.get('reg') or 'A'
        wa = 1 if m else 2          # accumulator width
        wx = 1 if x else 2          # index width
        L = []; fx = {}
        R8 = lambda e: 'rd8(%s)' % e
        R = lambda e, w: ('rd8(%s)' if w == 1 else 'rd16(%s)') % e
        W = lambda e, v, w: ('wr8(%s, %s);' if w == 1 else 'wr16(%s, %s);') % (e, v)
        mask = lambda w: '0xFF' if w == 1 else '0xFFFF'
        sign = lambda w: '0x80' if w == 1 else '0x8000'

        def src(w):
            if mode in ('immM', 'immX'):
                return hx(s.b(p) if w == 1 else s.w(p), w)
            if mode == 'imm8': return hx(s.b(p), 1)
            return R(s.ea(mode, p), w)

        def setreg(r, v, w):
            if w == 1: L.append('SETLO(%s, %s);' % (r, v))
            else: L.append('%s = %s;' % (r, v))

        def nzof(r, w):
            v = r if w == 2 else '(%s & 0xFF)' % r
            fx['Z'] = '(%s == 0)' % v
            fx['N'] = '((%s & %s) != 0)' % (r, sign(w))

        def target(t): return 'L_%06X' % t

        # ---- loads / stores
        if n in ('LDA',):
            setreg(reg, src(wa), wa); nzof(reg, wa)
        elif n in ('LDX', 'LDY'):
            r = n[2]; setreg(r, src(wx), wx); nzof(r, wx)
        elif n == 'STA': L.append(W(s.ea(mode, p), reg if wa == 2 else '(uint8_t)' + reg, wa))
        elif n in ('STX', 'STY'): L.append(W(s.ea(mode, p), n[2] if wx == 2 else '(uint8_t)' + n[2], wx))
        elif n == 'LDM':
            sz = s.opsize(mode); imm = s.b(p + sz) if wa == 1 else s.w(p + sz)
            L.append(W(s.ea(mode, p), hx(imm, wa), wa))
        # ---- logic
        elif n in ('AND', 'ORA', 'EOR'):
            op = {'AND': '&', 'ORA': '|', 'EOR': '^'}[n]
            if wa == 2: L.append('%s %s= %s;' % (reg, op, src(2)))
            else: L.append('SETLO(%s, %s %s %s);' % (reg, reg, op, src(1)))
            nzof(reg, wa)
        elif n in ('ADC', 'SBC'):
            fn = n.lower() + ('8' if wa == 1 else '16')
            setreg(reg, '%s(%s, %s)' % (fn, reg, src(wa)), wa)
        elif n in ('CMP', 'CPX', 'CPY'):
            r = reg if n == 'CMP' else n[2]
            w_ = wa if n == 'CMP' else wx
            rv = r if w_ == 2 else '(%s & 0xFF)' % r
            if mode in ('immM', 'immX'):
                v = src(w_)
            else:
                L.append('t = %s;' % src(w_)); v = 't'
            fx['Z'] = '(%s == %s)' % (rv, v)
            fx['C'] = '(%s >= %s)' % (rv, v)
            fx['N'] = '(((%s - %s) & %s) != 0)' % (rv, v, sign(w_))
            fx['_cmp'] = (rv, v)
        # ---- inc / dec
        elif n in ('INC', 'DEC'):
            op = '+' if n == 'INC' else '-'
            if mode in ('acc', 'accB'):
                r = 'B' if mode == 'accB' else 'A'
                if wa == 2: L.append('%s %s= 1;' % (r, op))
                else: L.append('SETLO(%s, %s %s 1);' % (r, r, op))
                nzof(r, wa)
            else:
                e = s.ea(mode, p)
                L.append('t = (%s %s 1) & %s; %s' % (R(e, wa), op, mask(wa), W(e, 't', wa)))
                fx['Z'] = '(t == 0)'; fx['N'] = '((t & %s) != 0)' % sign(wa)
        elif n in ('INX', 'INY', 'DEX', 'DEY'):
            r = n[2]; op = '+' if n[0] == 'I' else '-'
            if wx == 2: L.append('%s %s= 1;' % (r, op))
            else: L.append('%s = (%s %s 1) & 0xFF;' % (r, r, op))
            nzof(r, wx)
        # ---- shifts
        elif n in ('ASL', 'LSR', 'ROL', 'ROR'):
            fn = n.lower() + ('8' if wa == 1 else '16')
            if mode in ('acc', 'accB'):
                r = 'B' if mode == 'accB' else 'A'
                setreg(r, '%s(%s)' % (fn, r), wa)
            else:
                e = s.ea(mode, p); L.append(W(e, '%s(%s)' % (fn, R(e, wa)), wa))
        elif n == 'RLA':
            L.append('A = rla%s(A, %s);' % ('8' if wa == 1 else '16', src(wa))); nzof('A', wa)
        # ---- bit ops
        elif n in ('SEB', 'CLB'):
            sz = s.opsize(mode); mk = s.b(p + sz) if wa == 1 else s.w(p + sz)
            e = s.ea(mode, p)
            if n == 'SEB': L.append(W(e, '%s | %s' % (R(e, wa), hx(mk, wa)), wa))
            else: L.append(W(e, '%s & ~%s' % (R(e, wa), hx(mk, wa)), wa))
        elif n in ('BBS', 'BBC'):
            sz = s.opsize(mode); mk = s.b(p + sz) if wa == 1 else s.w(p + sz)
            e = s.ea(mode, p); v = R(e, wa)
            cond = ('(%s & %s) == %s' % (v, hx(mk, wa), hx(mk, wa))) if n == 'BBS' else ('(%s & %s) == 0' % (v, hx(mk, wa)))
            L.append('if (%s) goto %s;' % (cond, target(i['tg'][0])))
        # ---- branches
        elif n in BR_USE:
            f, neg = BR_USE[n]
            if fuse and f in fuse:
                c = fuse[f]
                if '_cmp' in fuse and f == 'C':
                    rv, v = fuse['_cmp']; c = '(%s %s %s)' % (rv, '<' if neg else '>=', v); neg = False
                elif '_cmp' in fuse and f == 'Z':
                    rv, v = fuse['_cmp']; c = '(%s %s %s)' % (rv, '!=' if neg else '==', v); neg = False
                L.append('if (%s) goto %s;' % (negate(c) if neg else strip(c), target(i['tg'][0])))
            else:
                L.append('if (%s%s) goto %s;' % ('!' if neg else '', f, target(i['tg'][0])))
        elif n in ('BRA', 'BRAL', 'JMP', 'JMPL') and mode in ('rel8', 'rel16', 'jabs', 'jlong'):
            L.append('goto %s;' % target(i['tg'][0]))
        # ---- calls / returns
        elif n in ('JSR', 'JSRL') and mode in ('jabs', 'jlong'):
            t = i['tg'][0]; nb = 3 if n == 'JSRL' else 2
            if t in s.d.extern:
                L.append('ram_trampoline_087A();')
            elif t in s.d.switch_helpers:
                nxt = a + i['len']
                if nxt in s.d.swtab:
                    vals, tg = s.d.swtab[nxt]
                    L.append('mflag = 1;  /* __switch_byte: inline table at %06X */' % nxt)
                    L.append('switch (A & 0xFF) {')
                    for v, tt in zip(vals, tg): L.append('    case 0x%02X: goto %s;' % (v, target(tt)))
                    L.append('    default: goto %s;' % target(tg[-1]))
                    L.append('}')
                else:
                    L.append('unknown_switch(0x%06X);' % a); s.unknown['switch'] += 1
            else:
                L.append('S -= %d; %s();' % (nb, fname(s.d, t)))
        elif n == 'JSR' and mode == 'jindx':
            t = i['tg'][0]; tl = s.d.jtables.get(t, (0, []))[1]
            L.append('S -= 2;')
            L.append('switch (X) {')
            for k, tt in enumerate(tl): L.append('    case 0x%02X: %s(); break;' % (2 * k, fname(s.d, tt)))
            L.append('    default: bad_dispatch(0x%06X);' % a)
            L.append('}')
        elif n == 'JMP' and mode == 'jindx':
            t = i['tg'][0]; tl = s.d.jtables.get(t, (0, []))[1]
            L.append('switch (X) {   /* jump table %06X */' % t)
            for k, tt in enumerate(tl): L.append('    case 0x%02X: goto %s;' % (2 * k, target(tt)))
            L.append('    default: bad_dispatch(0x%06X); return;' % a)
            L.append('}')
        elif n in ('JMP', 'JMPL') and mode in ('jind', 'jlind'):
            L.append('indirect_jump(0x%04X); return;' % s.w(p)); s.unknown['ijmp'] += 1
        elif n == 'RTS': L.append('S += 2; return;')
        elif n == 'RTL' and getattr(s, 'cur_entry', None) in s.callptr:
            L.append('t = rd16(SR(1)) | (uint32_t)rd8(SR(3)) << 16; S += 3;  /* RTL into the pushed function pointer */')
            L.append('call_address(t); return;')
        elif n == 'RTL': L.append('S += 3; return;')
        elif n == 'RTI': L.append('return;  /* RTI: dispatcher restores PS/PC */')
        # ---- transfers
        elif n in ('TAX', 'TAY', 'TBX', 'TBY'):
            r = n[2]; srcr = n[1]
            L.append('%s = %s;' % (r, srcr if wx == 2 else '(%s & 0xFF)' % srcr)); nzof(r, wx)
        elif n in ('TXA', 'TYA', 'TXB', 'TYB'):
            r = n[2]; srcr = n[1]; setreg(r, srcr, wa); nzof(r, wa)
        elif n in ('TXY', 'TYX'):
            L.append('%s = %s;' % (n[2], n[1])); nzof(n[2], wx)
        elif n == 'TXS': L.append('S = X;')
        elif n == 'TSX': L.append('X = S;'); nzof('X', wx)
        elif n in ('TAS', 'TBS'): L.append('S = %s;' % n[1])
        elif n in ('TSA', 'TSB'): L.append('%s = S;' % n[2]); nzof(n[2], 2)
        elif n in ('TAD', 'TBD'): L.append('D = %s;' % n[1])
        elif n in ('TDA', 'TDB'): L.append('%s = D;' % n[2]); nzof(n[2], 2)
        elif n == 'XAB': L.append('t = A; A = B; B = t;'); nzof('A', wa)
        # ---- stack
        elif n in ('PHA', 'PHB'):
            r = n[2]; L.append('push%d(%s);' % (wa * 8, r))
        elif n in ('PLA', 'PLB'):
            r = n[2]; setreg(r, 'pull%d()' % (wa * 8), wa); nzof(r, wa)
        elif n in ('PHX', 'PHY'): L.append('push%d(%s);' % (wx * 8, n[2]))
        elif n in ('PLX', 'PLY'):
            r = n[2]; L.append('%s = pull%d();' % (r, wx * 8)); nzof(r, wx)
        elif n == 'PHD': L.append('push16(D);')
        elif n == 'PLD': L.append('D = pull16();')
        elif n == 'PHT': L.append('push8(DT);')
        elif n == 'PLT': L.append('DT = pull8();')
        elif n == 'PHG': L.append('push8(PG);')
        elif n == 'PHP': L.append('push16(getP());')
        elif n == 'PLP': L.append('setP(pull16());')
        elif n == 'PSH': L.append('psh(0x%02X);' % s.b(p))
        elif n == 'PUL': L.append('pul(0x%02X);' % s.b(p))
        elif n == 'PEA': L.append('push16(0x%04X);' % s.w(p))
        elif n == 'PEI': L.append('push16(rd16(DP(0x%02X)));' % s.b(p))
        elif n == 'PER':
            r = s.w(p); tt = (a + i['len'] + (r - 65536 if r > 32767 else r)) & 0xFFFF
            L.append('push16(0x%04X);' % tt)
        # ---- flags
        elif n == 'CLC': L.append('C = 0;')
        elif n == 'SEC': L.append('C = 1;')
        elif n == 'CLV': L.append('V = 0;')
        elif n == 'CLI': L.append('I = 0;')
        elif n == 'SEI': L.append('I = 1;')
        elif n == 'CLM': L.append('mflag = 0;')
        elif n == 'SEM': L.append('mflag = 1;')
        elif n == 'CLP': L.append('clp(0x%02X);' % s.b(p))
        elif n == 'SEP': L.append('sep(0x%02X);' % s.b(p))
        elif n == 'LDT': L.append('DT = 0x%02X;' % s.b(p))
        # ---- arithmetic unit
        elif n in ('MPY', 'DIV'):
            L.append('%s%d(%s);' % (n.lower(), wa * 8, src(wa))); nzof('A', wa)
        # ---- misc
        elif n == 'NOP': L.append(';')
        elif n == 'WIT': L.append('cpu_wait();')
        elif n == 'STP': L.append('cpu_stop();')
        elif n == 'BRK': L.append('cpu_brk(); return;')
        elif n in ('MVN', 'MVP'):
            L.append('%s(0x%02X, 0x%02X);' % (n.lower(), s.b(p), s.b(p + 1)))
        else:
            L.append('/* TODO %s */' % n); s.unknown[n] += 1
        return L, fx

    # ---------------------------------------------------------- one function
    def function(s, entry, isr=False):
        d = s.d; mem = s.mem; s.cur_entry = entry
        addrs = s.body(entry)
        if not addrs: return None, 0
        aset = set(addrs)
        runtime_ret = entry >= 0x1E000 and entry < 0x1E500
        # CFG + labels
        succ = {a: [t for t in s.succ(a, d.insn[a]) if t in aset] for a in addrs}
        targets = set()
        for a in addrs:
            i = d.insn[a]; nxt = a + i['len']
            for t in succ[a]:
                if t != nxt or i['flow'] in ('uncond', 'jtab') or (i['flow'] == 'call' and t != nxt): targets.add(t)
        # flag liveness
        use = {}; de = {}
        for a in addrs:
            u, df = flag_use_def(d.insn[a], mem, a, runtime_ret)
            if d.insn[a]['name'] in ('ADC', 'SBC', 'ASL', 'LSR', 'ROL', 'ROR'):
                df = set(df)   # helper sets flags itself
            use[a] = u; de[a] = df
        live_in = {a: set() for a in addrs}
        changed = True
        while changed:
            changed = False
            for a in reversed(addrs):
                lo = set()
                for t in succ[a]: lo |= live_in[t]
                li = use[a] | (lo - de[a])
                if li != live_in[a]: live_in[a] = li; changed = True
        def live_out(a):
            lo = set()
            for t in succ[a]: lo |= live_in[t]
            return lo
        # emit
        out = []
        name = fname(d, entry)
        out.append('void %s(void)\n{' % name)
        out.append('    uint32_t t; (void)t;')
        if addrs[0] != entry: out.append('    goto L_%06X;' % entry)
        prev_fall = None
        k = 0
        while k < len(addrs):
            a = addrs[k]; i = d.insn[a]
            states = {i['mx']} | i.get('alt', set())
            if prev_fall is not None and prev_fall != a:
                out.append('    goto L_%06X;' % prev_fall)
            if a in targets or (prev_fall is None and k > 0) or a == entry and k > 0:
                out.append('L_%06X: TICK();' % a)
            lo = live_out(a)
            # fusion: flag-setter followed by a branch it alone feeds
            fuse = None
            nxt = a + i['len']
            hn = d77.hwnote(i, mem, a) if hasattr(d77, 'hwnote') else ''
            cmt = '/* %06X %s%s */' % (a, i['name'], (' [' + hn + ']') if hn else '')
            if len(states) == 1:
                m, x = next(iter(states))
                bi = d.insn.get(nxt)
                L, fx = s.emit(a, i, m, x, targets, lo, None)
                if (bi and nxt in aset and nxt not in targets and bi['name'] in BR_USE
                        and len({bi['mx']} | bi.get('alt', set())) == 1):
                    f = BR_USE[bi['name']][0]
                    after = live_out(nxt)
                    if f in fx and not (de[a] & after):
                        fuse = fx
                defs = de[a] & lo
                if fuse is None:
                    for f in sorted(defs):
                        if f in fx: L.append('%s = %s;' % (f, fx[f]))
                        elif i['name'] in ('ADC', 'SBC', 'ASL', 'LSR', 'ROL', 'ROR'): pass
                        elif f in ('N', 'Z', 'C', 'V') and i['name'] in ('JSR', 'JSRL', 'PLP', 'PUL', 'SEP', 'CLP', 'CLC', 'SEC', 'CLV'): pass
                emitted = ['    ' + x_ for x_ in L]
                if emitted: emitted[0] = emitted[0] + '  ' + cmt
                else: emitted = ['    ' + cmt]
                out.extend(emitted)
                if fuse is not None:
                    bL, _ = s.emit(nxt, bi, *bi['mx'], targets, live_out(nxt), fuse)
                    bc = '/* %06X %s */' % (nxt, bi['name'])
                    out.append('    ' + bL[0] + '  ' + bc)
                    k += 1; a = nxt; i = bi
            else:
                # instruction reached with several M/X states: decide at run time
                parts = []
                for (m, x) in sorted(states):
                    L, fx = s.emit(a, i, m, x, targets, lo, None)
                    for f in sorted(de[a] & lo):
                        if f in fx: L.append('%s = %s;' % (f, fx[f]))
                    parts.append(((m, x), L))
                if all(L == parts[0][1] for _, L in parts):
                    em = ['    ' + x_ for x_ in parts[0][1]] or ['    ;']
                    em[0] += '  ' + cmt
                    out.extend(em)
                    parts = []
                else:
                    out.append('    ' + cmt + ' /* reached with several M/X states */')
                for idx, ((m, x), L) in enumerate(parts):
                    c = 'mflag == %d' % m
                    out.append('    %sif (%s) { %s }' % ('else ' if idx else '', c, ' '.join(L)))
            fl = i['flow']
            if fl in ('seq', 'cond', 'call', 'calltab') and not (fl == 'call' and i['tg'] and i['tg'][0] in d.switch_helpers):
                prev_fall = a + i['len']
            else:
                prev_fall = None
            k += 1
        if prev_fall is not None:
            out.append('    goto L_%06X;  /* falls into code outside this function */' % prev_fall
                       if prev_fall in aset else '    return;  /* falls off: %06X */' % prev_fall)
        out.append('}\n')
        return '\n'.join(out), len(addrs)

# ------------------------------------------------------------------ driver
HEADER = r'''/* m7700.h - minimal M37732 CPU model used by the lifted C code.
 * Registers are globals so that lifted functions behave exactly like the
 * original machine code.  Memory, SFR access and interrupts are provided by
 * m7700_rt.c.  Generated by tools/lift.py. */
#ifndef M7700_H
#define M7700_H
#include <stdint.h>

extern uint16_t A, B, X, Y, S, D;
extern uint8_t  DT, PG;
extern uint8_t  C, Z, N, V, I, mflag, xflag, IPL;   /* PS = IPL(3) | N V m x D I Z C */

uint8_t  rd8(uint32_t a);
void     wr8(uint32_t a, uint8_t v);
static inline uint16_t rd16(uint32_t a) { return (uint16_t)(rd8(a) | rd8(a + 1) << 8); }
static inline void     wr16(uint32_t a, uint16_t v) { wr8(a, (uint8_t)v); wr8(a + 1, (uint8_t)(v >> 8)); }
static inline uint32_t rd24(uint32_t a) { return rd16(a) | (uint32_t)rd8(a + 2) << 16; }

#ifndef TICK
#define TICK()       ((void)0)
#endif
#define SETLO(r, v)  ((r) = (uint16_t)(((r) & 0xFF00) | ((v) & 0xFF)))
#define DP(o)        ((uint32_t)((D + (o)) & 0xFFFF))              /* direct page = NC77 frame */
#define SR(o)        ((uint32_t)((S + (o)) & 0xFFFF))              /* stack relative */
#define ABS(a)       (((uint32_t)DT << 16) | (uint16_t)(a))
#define IND(o)       ABS(rd16(DP(o)))
#define LIND(o)      (rd24(DP(o)))
#define SRIND(o)     ABS(rd16(SR(o)))

static inline void push8(uint8_t v)   { wr8(S, v); S--; }
static inline uint8_t pull8(void)     { S++; return rd8(S); }
static inline void push16(uint16_t v) { push8((uint8_t)(v >> 8)); push8((uint8_t)v); }
static inline uint16_t pull16(void)   { uint16_t lo = pull8(); return (uint16_t)(lo | pull8() << 8); }

uint16_t getP(void);
void     setP(uint16_t p);
void     sep(uint8_t m);
void     clp(uint8_t m);
void     psh(uint8_t mask);
void     pul(uint8_t mask);

uint8_t  adc8(uint16_t r, uint16_t v);   uint16_t adc16(uint16_t r, uint16_t v);
uint8_t  sbc8(uint16_t r, uint16_t v);   uint16_t sbc16(uint16_t r, uint16_t v);
uint8_t  asl8(uint16_t r);  uint16_t asl16(uint16_t r);
uint8_t  lsr8(uint16_t r);  uint16_t lsr16(uint16_t r);
uint8_t  rol8(uint16_t r);  uint16_t rol16(uint16_t r);
uint8_t  ror8(uint16_t r);  uint16_t ror16(uint16_t r);
uint16_t rla8(uint16_t r, uint16_t n);  uint16_t rla16(uint16_t r, uint16_t n);
void     mpy8(uint16_t v);  void mpy16(uint16_t v);
void     div8(uint16_t v);  void div16(uint16_t v);
void     mvn(uint8_t dst, uint8_t src);
void     mvp(uint8_t dst, uint8_t src);

void     cpu_wait(void);
void     cpu_stop(void);
void     cpu_brk(void);
void     ram_trampoline_087A(void);   /* RAM 0x087A: NOP; WIT (or STP); RTL */
void     indirect_jump(uint16_t ptr);
void     call_address(uint32_t a);       /* dispatch.c */
void     bad_dispatch(uint32_t at);
void     unknown_switch(uint32_t at);
#endif
'''

RUNTIME = r'''/* m7700_rt.c - CPU helpers for the lifted code (flag semantics of MELPS 7700). */
#include "m7700.h"
#include "rom_image.h"

uint16_t A, B, X, Y, S, D;
uint8_t  DT, PG;
uint8_t  C, Z, N, V, I, mflag, xflag, IPL;

uint8_t ram[0x880];   /* internal RAM (non-static so tests can seed it) */

/* Hook these up to real/emulated peripherals. */
__attribute__((weak)) uint8_t sfr_read(uint8_t a)            { (void)a; return 0; }
__attribute__((weak)) void    sfr_write(uint8_t a, uint8_t v) { (void)a; (void)v; }

uint8_t rd8(uint32_t a)
{
    a &= 0xFFFFFF;
    if (a < 0x80) return sfr_read((uint8_t)a);
    if (a < 0x880) return ram[a];
    if (a >= 0x2000 && a < 0x20000) return rom_image[a];
    return 0xFF;
}
__attribute__((weak)) void mem_trace(uint32_t a, uint8_t v) { (void)a; (void)v; }

void wr8(uint32_t a, uint8_t v)
{
    a &= 0xFFFFFF;
    if (a < 0x600) mem_trace(a, v);      /* SFR + global RAM write trace (tests) */
    if (a < 0x80) sfr_write((uint8_t)a, v);
    else if (a < 0x880) ram[a] = v;
    /* writes to 0x5555/0x2AAA etc. reach the 29F010 in hardware (flash programming) */
}

uint16_t getP(void) { return (uint16_t)(C | Z << 1 | I << 2 | xflag << 4 | mflag << 5 | V << 6 | N << 7 | (IPL & 7) << 8); }
void setP(uint16_t p) { C = p & 1; Z = p >> 1 & 1; I = p >> 2 & 1; xflag = p >> 4 & 1; mflag = p >> 5 & 1; V = p >> 6 & 1; N = p >> 7 & 1; IPL = p >> 8 & 7; }
void sep(uint8_t m) { setP(getP() | m); }
void clp(uint8_t m) { setP(getP() & (uint16_t)~m); }

/* PSH/PUL (MELPS 7700 manual): b0 A, b1 B, b2 X, b3 Y, b4 DPR, b5 DT, b6 PG, b7 PS.
 * PSH saves A first and PS last; PUL restores in the reverse order. PS is 16 bits (IPL). */
void psh(uint8_t m)
{
    if (m & 0x01) { if (mflag) push8((uint8_t)A); else push16(A); }
    if (m & 0x02) { if (mflag) push8((uint8_t)B); else push16(B); }
    if (m & 0x04) { if (xflag) push8((uint8_t)X); else push16(X); }
    if (m & 0x08) { if (xflag) push8((uint8_t)Y); else push16(Y); }
    if (m & 0x10) push16(D);
    if (m & 0x20) push8(DT);
    if (m & 0x40) push8(PG);
    if (m & 0x80) push16(getP());
}
void pul(uint8_t m)
{
    if (m & 0x80) setP(pull16());
    if (m & 0x40) PG = pull8();
    if (m & 0x20) DT = pull8();
    if (m & 0x10) D = pull16();
    if (m & 0x08) { if (xflag) Y = pull8(); else Y = pull16(); }
    if (m & 0x04) { if (xflag) X = pull8(); else X = pull16(); }
    if (m & 0x02) { if (mflag) SETLO(B, pull8()); else B = pull16(); }
    if (m & 0x01) { if (mflag) SETLO(A, pull8()); else A = pull16(); }
}

static void nz8(uint8_t r)   { Z = r == 0; N = r >> 7; }
static void nz16(uint16_t r) { Z = r == 0; N = r >> 15; }

uint8_t adc8(uint16_t r, uint16_t v)
{ unsigned a = r & 0xFF, b = v & 0xFF, s = a + b + C; V = (~(a ^ b) & (a ^ s) & 0x80) != 0; C = s > 0xFF; nz8((uint8_t)s); return (uint8_t)s; }
uint16_t adc16(uint16_t r, uint16_t v)
{ unsigned long s = (unsigned long)r + v + C; V = (~(r ^ v) & (r ^ s) & 0x8000) != 0; C = s > 0xFFFF; nz16((uint16_t)s); return (uint16_t)s; }
uint8_t sbc8(uint16_t r, uint16_t v)
{ int a = r & 0xFF, b = v & 0xFF, s = a - b - (1 - C); V = ((a ^ b) & (a ^ s) & 0x80) != 0; C = s >= 0; nz8((uint8_t)s); return (uint8_t)s; }
uint16_t sbc16(uint16_t r, uint16_t v)
{ long s = (long)r - v - (1 - C); V = ((r ^ v) & (r ^ s) & 0x8000) != 0; C = s >= 0; nz16((uint16_t)s); return (uint16_t)s; }
uint8_t  asl8(uint16_t r)  { C = (r >> 7) & 1; nz8((uint8_t)(r << 1)); return (uint8_t)(r << 1); }
uint16_t asl16(uint16_t r) { C = r >> 15; nz16((uint16_t)(r << 1)); return (uint16_t)(r << 1); }
uint8_t  lsr8(uint16_t r)  { C = r & 1; nz8((uint8_t)((r & 0xFF) >> 1)); return (uint8_t)((r & 0xFF) >> 1); }
uint16_t lsr16(uint16_t r) { C = r & 1; nz16((uint16_t)(r >> 1)); return (uint16_t)(r >> 1); }
uint8_t  rol8(uint16_t r)  { uint8_t o = (uint8_t)(r << 1 | C); C = (r >> 7) & 1; nz8(o); return o; }
uint16_t rol16(uint16_t r) { uint16_t o = (uint16_t)(r << 1 | C); C = r >> 15; nz16(o); return o; }
uint8_t  ror8(uint16_t r)  { uint8_t o = (uint8_t)((r & 0xFF) >> 1 | C << 7); C = r & 1; nz8(o); return o; }
uint16_t ror16(uint16_t r) { uint16_t o = (uint16_t)(r >> 1 | C << 15); C = r & 1; nz16(o); return o; }
uint16_t rla8(uint16_t r, uint16_t n)  { uint8_t v = (uint8_t)r; while (n--) v = (uint8_t)(v << 1 | v >> 7); return (uint16_t)((r & 0xFF00) | v); }
uint16_t rla16(uint16_t r, uint16_t n) { while (n--) r = (uint16_t)(r << 1 | r >> 15); return r; }
/* MPY: A x M -> B:A ; DIV: B:A / M -> quotient A, remainder B (MELPS 7700 manual) */
void mpy8(uint16_t v)  { unsigned p = (A & 0xFF) * (v & 0xFF); SETLO(A, p); SETLO(B, p >> 8); }
void mpy16(uint16_t v) { unsigned long p = (unsigned long)A * v; A = (uint16_t)p; B = (uint16_t)(p >> 16); }
void div8(uint16_t v)  { unsigned n = (B & 0xFF) << 8 | (A & 0xFF); v &= 0xFF; if (!v) { cpu_brk(); return; } SETLO(A, n / v); SETLO(B, n % v); }
void div16(uint16_t v) { unsigned long n = (unsigned long)B << 16 | A; if (!v) { cpu_brk(); return; } A = (uint16_t)(n / v); B = (uint16_t)(n % v); }
void mvn(uint8_t dst, uint8_t src) { do { wr8((uint32_t)dst << 16 | Y, rd8((uint32_t)src << 16 | X)); X++; Y++; } while (A-- != 0); DT = dst; }
void mvp(uint8_t dst, uint8_t src) { do { wr8((uint32_t)dst << 16 | Y, rd8((uint32_t)src << 16 | X)); X--; Y--; } while (A-- != 0); DT = dst; }

__attribute__((weak)) void cpu_wait(void) {}
__attribute__((weak)) void cpu_stop(void) {}
__attribute__((weak)) void cpu_brk(void) {}
void ram_trampoline_087A(void) { if (ram[0x87B] == 0xDB) cpu_stop(); else cpu_wait(); }
__attribute__((weak)) void indirect_jump(uint16_t p) { (void)p; }
__attribute__((weak)) void bad_dispatch(uint32_t at) { (void)at; }
__attribute__((weak)) void unknown_switch(uint32_t at) { (void)at; }
'''

def main():
    src, outdir = sys.argv[1], sys.argv[2]
    os.makedirs(outdir, exist_ok=True)
    d = d77.disassemble(src, '/dev/null')
    # NAMES were assigned on YA4000S; keep a name only where the code bytes are identical
    ref = os.path.join(os.path.dirname(os.path.abspath(src)), 'YA4000S.HEX')
    if os.path.exists(ref) and os.path.basename(src) != 'YA4000S.HEX':
        import srec
        rm = srec.load(ref)
        for a in list(NAMES):
            if any(d.mem.get(a + k) != rm.get(a + k) for k in range(24)):
                del NAMES[a]
        print('names kept (code identical to YA4000S):', len(NAMES))
    for va, nm in d77.VECTORS:
        v = d.w(va)
        if v: VECNAME[v] = 'isr_' + nm if nm != 'RESET' else 'reset_entry'
    d.noreturn = set()
    lf = Lifter(d)
    entries = sorted(set(d.funcs) | set(VECNAME) | {0x80})
    entries = [e for e in entries if e in d.insn and e not in d.switch_helpers]
    groups = collections.OrderedDict([('ramcode', []), ('bank0_a', []), ('bank0_b', []), ('bank0_c', []),
                                      ('bank1_a', []), ('bank1_b', []), ('runtime', [])])
    for e in entries:
        if e < 0x2000: g = 'ramcode'
        elif e < 0x7000: g = 'bank0_a'
        elif e < 0xB000: g = 'bank0_b'
        elif e < 0x10000: g = 'bank0_c'
        elif e < 0x18000: g = 'bank1_a'
        elif e < 0x1E000: g = 'bank1_b'
        else: g = 'runtime'
        groups[g].append(e)
    total = 0; nf = 0
    with open(os.path.join(outdir, 'functions.h'), 'w') as h:
        h.write('/* prototypes of all lifted functions */\n#ifndef FUNCTIONS_H\n#define FUNCTIONS_H\n')
        for e in entries: h.write('void %s(void);\n' % fname(d, e))
        h.write('#endif\n')
    for g, es in groups.items():
        with open(os.path.join(outdir, g + '.c'), 'w') as f:
            f.write('/* %s.c - lifted from %s by tools/lift.py. Do not edit by hand;\n'
                    ' * cleaned-up versions live in ../c_clean/. */\n' % (g, os.path.basename(src)))
            f.write('#include "m7700.h"\n#include "functions.h"\n\n')
            for e in es:
                xr = sorted(d.xref.get(e, ()))
                f.write('/* %06X  callers: %s */\n' % (e, ', '.join('%06X' % x for x in xr[:8]) + (' ...' if len(xr) > 8 else '')))
                code, n = lf.function(e)
                if code: f.write(code + '\n'); total += n; nf += 1
    sfr = '\n/* M37732S4 special function registers (datasheet Fig. 2) */\n' + ''.join(
        '#define %-8s 0x%02X\n' % (nm, a) for a, nm in sorted(d77.SFR.items()))
    with open(os.path.join(outdir, 'dispatch.c'), 'w') as f:
        f.write('/* call_address(): calls a lifted function by its firmware address (function pointers) */\n')
        f.write('#include "m7700.h"\n#include "functions.h"\nvoid call_address(uint32_t a)\n{\n    switch (a) {\n')
        for e in entries: f.write('    case 0x%06X: %s(); return;\n' % (e, fname(d, e)))
        f.write('    default: bad_dispatch(a);\n    }\n}\n')
    print('indirect-call helpers:', ['%06X' % a for a in sorted(lf.callptr)])
    os.makedirs(os.path.join(outdir, 'test'), exist_ok=True)
    with open(os.path.join(outdir, 'test', 'func_table.c'), 'w') as f:
        f.write('/* generated: every lifted function with its entry M state and return size */\n')
        f.write('#include "../m7700.h"\n#include "../functions.h"\n')
        f.write('typedef struct { uint32_t addr; void (*fn)(void); uint8_t mstates; uint8_t retsize; uint8_t isr; } func_t;\n')
        f.write('const func_t func_table[] = {\n')
        for e in entries:
            i = d.insn[e]; st = {i['mx']} | i.get('alt', set())
            ms = (1 if any(m == 0 for m, x in st) else 0) | (2 if any(m == 1 for m, x in st) else 0)
            rets = [d.insn[a]['name'] for a in lf.body(e) if d.insn[a]['name'] in ('RTS', 'RTL', 'RTI')]
            rs = 2 if rets and all(r == 'RTS' for r in rets) else 3
            isr = 1 if (e in VECNAME and VECNAME[e] != 'reset_entry') else 0
            f.write('  {0x%06X, %s, %d, %d, %d},\n' % (e, fname(d, e), ms, rs, isr))
        f.write('};\nconst int func_count = sizeof func_table / sizeof func_table[0];\n')
    with open(os.path.join(outdir, 'm7700.h'), 'w') as f: f.write(HEADER.replace('#endif\n', sfr + '#endif\n'))
    with open(os.path.join(outdir, 'm7700_rt.c'), 'w') as f: f.write(RUNTIME)
    with open(os.path.join(outdir, 'rom_image.h'), 'w') as f:
        f.write('#include <stdint.h>\nextern const uint8_t rom_image[0x20000];\n')
    with open(os.path.join(outdir, 'rom_image.c'), 'w') as f:
        f.write('/* ROM image 0x00000-0x1FFFF from %s (unprogrammed = 0xFF) */\n#include "rom_image.h"\n' % os.path.basename(src))
        f.write('const uint8_t rom_image[0x20000] = {\n')
        rom = [d.mem.get(a, 0xFF) if a >= 0x2000 else 0xFF for a in range(0x20000)]
        for k in range(0, 0x20000, 16):
            f.write('  ' + ','.join('0x%02X' % v for v in rom[k:k + 16]) + ',\n')
        f.write('};\n')
    print('functions', nf, 'instructions lifted (incl. shared tails)', total, 'unknown', dict(lf.unknown))

if __name__ == '__main__':
    main()
