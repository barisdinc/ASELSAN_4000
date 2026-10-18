#!/usr/bin/env python3
"""Small MELPS 7700 interpreter used to cross-check the lifted C code.

It executes the original machine code from the HEX file, starting at the reset
vector, with deterministic pseudo-random SFR reads, and writes a trace of all
writes to SFRs (0x00-0x7F) and global RAM (0x80-0x5FF).  The same trace is
produced by c_lifted/test/trace_main.c; the two must be identical.

usage: emu7700.py <file.HEX> <events> <trace_out>
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import srec, d77

class Stop(Exception):
    def __init__(s, code='K'): s.code = code

class CPU:
    def __init__(s, path, limit, out):
        m = srec.load(path)
        s.rom = bytearray([0xFF]) * 0x20000
        for a, v in m.items():
            if 0x2000 <= a < 0x20000: s.rom[a] = v
        s.ram = bytearray(0x880)
        s.A = s.B = s.X = s.Y = s.S = s.D = 0
        s.DT = s.PG = 0
        s.C = s.Z = s.N = s.V = s.I = s.IPL = 0
        s.m = s.x = 0
        s.seed = int(os.environ.get('SEED', '12345'))
        s.cov = set()
        s.limit = limit; s.n = 0; s.out = out
        s.PC = s.rom[0xFFFE] | s.rom[0xFFFF] << 8
        s.steps = 0

    # ---- memory
    def sfr_read(s, a):
        s.nrd = getattr(s, 'nrd', 0) + 1
        if s.nrd > 20000: raise Stop('Q')
        s.seed = (s.seed * 1103515245 + 12345) & 0xFFFFFFFF
        return (s.seed >> 16) & 0xFF
    def rd8(s, a):
        a &= 0xFFFFFF
        if a < 0x80: return s.sfr_read(a)
        if a < 0x880: return s.ram[a]
        if 0x2000 <= a < 0x20000: return s.rom[a]
        return 0xFF
    def wr8(s, a, v, trace=True):
        a &= 0xFFFFFF; v &= 0xFF
        if a < 0x600 and trace:
            s.out.write(('%04X %02X r%d\n' % (a, v, getattr(s, 'nrd', 0))) if os.environ.get('DBG') else ('%04X %02X\n' % (a, v))); s.n += 1
            if s.n >= s.limit: raise Stop('L')
        if 0x80 <= a < 0x880: s.ram[a] = v
    def rd16(s, a): return s.rd8(a) | s.rd8(a + 1) << 8
    def wr16(s, a, v, trace=True): s.wr8(a, v & 0xFF, trace); s.wr8(a + 1, v >> 8, trace)
    def rd24(s, a): return s.rd16(a) | s.rd8(a + 2) << 16

    # ---- stack
    def push8(s, v, trace=True): s.wr8(s.S, v, trace); s.S = (s.S - 1) & 0xFFFF
    def pull8(s): s.S = (s.S + 1) & 0xFFFF; return s.rd8(s.S)
    def push16(s, v, trace=True): s.push8(v >> 8, trace); s.push8(v & 0xFF, trace)
    def pull16(s): lo = s.pull8(); return lo | s.pull8() << 8

    def getP(s): return s.C | s.Z << 1 | s.I << 2 | s.x << 4 | s.m << 5 | s.V << 6 | s.N << 7 | (s.IPL & 7) << 8
    def setP(s, p):
        s.C = p & 1; s.Z = p >> 1 & 1; s.I = p >> 2 & 1; s.x = p >> 4 & 1; s.m = p >> 5 & 1
        s.V = p >> 6 & 1; s.N = p >> 7 & 1; s.IPL = p >> 8 & 7

    # ---- fetch
    def fb(s):
        v = s.rd8(s.PG << 16 | s.PC); s.PC = (s.PC + 1) & 0xFFFF; return v
    def fw(s): lo = s.fb(); return lo | s.fb() << 8
    def fl(s): lo = s.fw(); return lo | s.fb() << 16

    def nz(s, v, w):
        msk = 0xFF if w == 1 else 0xFFFF
        s.Z = int(v & msk == 0); s.N = int(v & (0x80 if w == 1 else 0x8000) != 0)

    def ea(s, mode):
        D, X, Y, DT = s.D, s.X, s.Y, s.DT
        if mode in ('dp', 'bitdp', 'bbdp', 'ldmdp'): return (D + s.fb()) & 0xFFFF
        if mode in ('dpx', 'ldmdpx'): return (D + s.fb() + X) & 0xFFFF
        if mode == 'dpy': return (D + s.fb() + Y) & 0xFFFF
        if mode == 'ind': return DT << 16 | s.rd16((D + s.fb()) & 0xFFFF)
        if mode == 'indx': return DT << 16 | s.rd16((D + s.fb() + X) & 0xFFFF)
        if mode == 'indy': return ((DT << 16 | s.rd16((D + s.fb()) & 0xFFFF)) + Y) & 0xFFFFFF
        if mode == 'lind': return s.rd24((D + s.fb()) & 0xFFFF)
        if mode == 'lindy': return (s.rd24((D + s.fb()) & 0xFFFF) + Y) & 0xFFFFFF
        if mode == 'sr': return (s.S + s.fb()) & 0xFFFF
        if mode == 'sry': return ((DT << 16 | s.rd16((s.S + s.fb()) & 0xFFFF)) + Y) & 0xFFFFFF
        if mode in ('abs', 'bitabs', 'bbabs', 'ldmabs'): return DT << 16 | s.fw()
        if mode in ('absx', 'ldmabsx'): return ((DT << 16 | s.fw()) + X) & 0xFFFFFF
        if mode == 'absy': return ((DT << 16 | s.fw()) + Y) & 0xFFFFFF
        if mode == 'long': return s.fl()
        if mode == 'longx': return (s.fl() + X) & 0xFFFFFF
        raise KeyError(mode)

    def rdw(s, a, w): return s.rd8(a) if w == 1 else s.rd16(a)
    def wrw(s, a, v, w):
        if w == 1: s.wr8(a, v)
        else: s.wr16(a, v)

    def operand(s, mode, w):
        if mode in ('immM', 'immX'): return s.fb() if w == 1 else s.fw()
        if mode == 'imm8': return s.fb()
        return s.rdw(s.ea(mode), w)

    def getr(s, r): return getattr(s, r)
    def setr(s, r, v, w):
        if w == 1: setattr(s, r, (getattr(s, r) & 0xFF00) | (v & 0xFF))
        else: setattr(s, r, v & 0xFFFF)

    def adc(s, r, v, w):
        msk = 0xFF if w == 1 else 0xFFFF; sg = 0x80 if w == 1 else 0x8000
        a = r & msk; b = v & msk; t = a + b + s.C
        s.V = int((~(a ^ b) & (a ^ t) & sg) != 0); s.C = int(t > msk); s.nz(t, w); return t & msk
    def sbc(s, r, v, w):
        msk = 0xFF if w == 1 else 0xFFFF; sg = 0x80 if w == 1 else 0x8000
        a = r & msk; b = v & msk; t = a - b - (1 - s.C)
        s.V = int(((a ^ b) & (a ^ t) & sg) != 0); s.C = int(t >= 0); s.nz(t, w); return t & msk
    def shift(s, n, v, w):
        msk = 0xFF if w == 1 else 0xFFFF; top = 7 if w == 1 else 15; v &= msk
        if n == 'ASL': c = v >> top & 1; r = (v << 1) & msk
        elif n == 'LSR': c = v & 1; r = v >> 1
        elif n == 'ROL': c = v >> top & 1; r = ((v << 1) | s.C) & msk
        else: c = v & 1; r = (v >> 1) | (s.C << top)
        s.C = c; s.nz(r, w); return r

    def step(s):
        s.steps += 1
        a = s.PG << 16 | s.PC
        s.cov.add(a)
        op = s.fb(); pre = None
        if op == 0x42:
            op2 = s.fb()
            if op2 in d77.B_IMP: name, mode = d77.B_IMP[op2]
            else: name, mode = d77.OPS[op2]
            pre = 'B'
        elif op == 0x89:
            op2 = s.fb(); name, mode = d77.P89[op2]
        else:
            name, mode = d77.OPS[op]
        reg = 'B' if pre == 'B' else 'A'
        wa = 1 if s.m else 2; wx = 1 if s.x else 2
        n = name
        def rel8():
            r = s.fb(); return (s.PC + (r - 256 if r > 127 else r)) & 0xFFFF
        def branch(cond):
            t = rel8()
            if cond: s.PC = t
        if n == 'LDA': v = s.operand(mode, wa); s.setr(reg, v, wa); s.nz(v, wa)
        elif n in ('LDX', 'LDY'): v = s.operand(mode, wx); s.setr(n[2], v, wx); s.nz(v, wx)
        elif n == 'STA': s.wrw(s.ea(mode), s.getr(reg), wa)
        elif n in ('STX', 'STY'): s.wrw(s.ea(mode), s.getr(n[2]), wx)
        elif n == 'LDM':
            e = s.ea(mode); v = s.fb() if wa == 1 else s.fw(); s.wrw(e, v, wa)
        elif n in ('AND', 'ORA', 'EOR'):
            v = s.operand(mode, wa); r = s.getr(reg)
            r2 = r & v if n == 'AND' else (r | v if n == 'ORA' else r ^ v)
            s.setr(reg, r2, wa); s.nz(r2, wa)
        elif n == 'ADC': v = s.operand(mode, wa); s.setr(reg, s.adc(s.getr(reg), v, wa), wa)
        elif n == 'SBC': v = s.operand(mode, wa); s.setr(reg, s.sbc(s.getr(reg), v, wa), wa)
        elif n in ('CMP', 'CPX', 'CPY'):
            w = wa if n == 'CMP' else wx; r = s.getr(reg if n == 'CMP' else n[2])
            v = s.operand(mode, w); msk = 0xFF if w == 1 else 0xFFFF
            t = (r & msk) - v; s.C = int(t >= 0); s.nz(t, w)
        elif n in ('INC', 'DEC'):
            dlt = 1 if n == 'INC' else -1
            if mode in ('acc', 'accB'):
                r = 'B' if mode == 'accB' else 'A'; v = s.getr(r) + dlt; s.setr(r, v, wa); s.nz(v, wa)
            else:
                e = s.ea(mode); v = s.rdw(e, wa) + dlt; s.wrw(e, v & 0xFFFF, wa); s.nz(v, wa)
        elif n in ('INX', 'INY', 'DEX', 'DEY'):
            r = n[2]; v = s.getr(r) + (1 if n[0] == 'I' else -1)
            v &= 0xFF if wx == 1 else 0xFFFF; setattr(s, r, v); s.nz(v, wx)
        elif n in ('ASL', 'LSR', 'ROL', 'ROR'):
            if mode in ('acc', 'accB'):
                r = 'B' if mode == 'accB' else 'A'; s.setr(r, s.shift(n, s.getr(r), wa), wa)
            else:
                e = s.ea(mode); s.wrw(e, s.shift(n, s.rdw(e, wa), wa), wa)
        elif n == 'RLA':
            cnt = s.operand(mode, wa); w = wa; v = s.A & (0xFF if w == 1 else 0xFFFF)
            for _ in range(cnt):
                v = ((v << 1) | (v >> (7 if w == 1 else 15))) & (0xFF if w == 1 else 0xFFFF)
            s.setr('A', v, w); s.nz(v, w)
        elif n in ('SEB', 'CLB'):
            e = s.ea(mode); mk = s.fb() if wa == 1 else s.fw(); v = s.rdw(e, wa)
            s.wrw(e, v | mk if n == 'SEB' else v & ~mk & 0xFFFF, wa)
        elif n in ('BBS', 'BBC'):
            e = s.ea(mode); mk = s.fb() if wa == 1 else s.fw(); v = s.rdw(e, wa); t = rel8()
            if (n == 'BBS' and v & mk == mk) or (n == 'BBC' and v & mk == 0): s.PC = t
        elif n in ('BEQ', 'BNE', 'BCS', 'BCC', 'BMI', 'BPL', 'BVS', 'BVC'):
            f, neg = d77_br[n]; branch(bool(getattr(s, f)) != neg)
        elif n == 'BRA': branch(True)
        elif n == 'BRAL':
            r = s.fw(); s.PC = (s.PC + (r - 65536 if r > 32767 else r)) & 0xFFFF
        elif n == 'JMP' and mode == 'jabs': s.PC = s.fw()
        elif n == 'JMPL' and mode == 'jlong': v = s.fl(); s.PC = v & 0xFFFF; s.PG = v >> 16
        elif n == 'JMP' and mode == 'jindx': p = s.fw(); s.PC = s.rd16((s.PG << 16) | ((p + s.X) & 0xFFFF))
        elif n == 'JSR' and mode == 'jabs':
            t = s.fw(); s.push16(s.PC, False); s.PC = t
        elif n == 'JSRL':
            t = s.fl()
            if t == 0x87A:                   # RAM trampoline: NOP; WIT (or STP); RTL
                if s.ram[0x87B] == 0xDB: raise Stop('K')
                return
            s.push8(s.PG, False); s.push16(s.PC, False); s.PC = t & 0xFFFF; s.PG = t >> 16
        elif n == 'JSR' and mode == 'jindx':
            p = s.fw(); t = s.rd16((s.PG << 16) | ((p + s.X) & 0xFFFF)); s.push16(s.PC, False); s.PC = t
        elif n == 'RTS': s.PC = s.pull16()
        elif n == 'RTL': s.PC = s.pull16(); s.PG = s.pull8()
        elif n == 'RTI': raise Stop('R')
        elif n in ('TAX', 'TAY', 'TBX', 'TBY'):
            v = s.getr(n[1]) & (0xFF if wx == 1 else 0xFFFF); setattr(s, n[2], v); s.nz(v, wx)
        elif n in ('TXA', 'TYA', 'TXB', 'TYB'):
            v = s.getr(n[1]); s.setr(n[2], v, wa); s.nz(v, wa)
        elif n in ('TXY', 'TYX'): v = s.getr(n[1]); setattr(s, n[2], v); s.nz(v, wx)
        elif n == 'TXS': s.S = s.X
        elif n == 'TSX': s.X = s.S; s.nz(s.X, wx)
        elif n in ('TAS', 'TBS'): s.S = s.getr(n[1])
        elif n in ('TSA', 'TSB'): setattr(s, n[2], s.S); s.nz(s.S, 2)
        elif n in ('TAD', 'TBD'): s.D = s.getr(n[1])
        elif n in ('TDA', 'TDB'): setattr(s, n[2], s.D); s.nz(s.D, 2)
        elif n == 'XAB': s.A, s.B = s.B, s.A; s.nz(s.A, wa)
        elif n in ('PHA', 'PHB'):
            v = s.getr(n[2])
            if wa == 1: s.push8(v & 0xFF)
            else: s.push16(v)
        elif n in ('PLA', 'PLB'):
            v = s.pull8() if wa == 1 else s.pull16(); s.setr(n[2], v, wa); s.nz(v, wa)
        elif n in ('PHX', 'PHY'):
            v = s.getr(n[2])
            if wx == 1: s.push8(v & 0xFF)
            else: s.push16(v)
        elif n in ('PLX', 'PLY'):
            v = s.pull8() if wx == 1 else s.pull16(); setattr(s, n[2], v); s.nz(v, wx)
        elif n == 'PHD': s.push16(s.D)
        elif n == 'PLD': s.D = s.pull16()
        elif n == 'PHT': s.push8(s.DT)
        elif n == 'PLT': s.DT = s.pull8()
        elif n == 'PHG': s.push8(s.PG)
        elif n == 'PHP': s.push16(s.getP())
        elif n == 'PLP': s.setP(s.pull16())
        elif n == 'PSH':
            mk = s.fb()
            if mk & 1: s.push8(s.A & 0xFF) if s.m else s.push16(s.A)
            if mk & 2: s.push8(s.B & 0xFF) if s.m else s.push16(s.B)
            if mk & 4: s.push8(s.X & 0xFF) if s.x else s.push16(s.X)
            if mk & 8: s.push8(s.Y & 0xFF) if s.x else s.push16(s.Y)
            if mk & 0x10: s.push16(s.D)
            if mk & 0x20: s.push8(s.DT)
            if mk & 0x40: s.push8(s.PG)
            if mk & 0x80: s.push16(s.getP())
        elif n == 'PUL':
            mk = s.fb()
            if mk & 0x80: s.setP(s.pull16())
            if mk & 0x40: s.PG = s.pull8()
            if mk & 0x20: s.DT = s.pull8()
            if mk & 0x10: s.D = s.pull16()
            if mk & 8: s.Y = s.pull8() if s.x else s.pull16()
            if mk & 4: s.X = s.pull8() if s.x else s.pull16()
            if mk & 2: s.setr('B', s.pull8() if s.m else s.pull16(), 1 if s.m else 2)
            if mk & 1: s.setr('A', s.pull8() if s.m else s.pull16(), 1 if s.m else 2)
        elif n == 'PEA': s.push16(s.fw())
        elif n == 'PEI': s.push16(s.rd16((s.D + s.fb()) & 0xFFFF))
        elif n == 'PER': r = s.fw(); s.push16((s.PC + (r - 65536 if r > 32767 else r)) & 0xFFFF)
        elif n == 'CLC': s.C = 0
        elif n == 'SEC': s.C = 1
        elif n == 'CLV': s.V = 0
        elif n == 'CLI': s.I = 0
        elif n == 'SEI': s.I = 1
        elif n == 'CLM': s.m = 0
        elif n == 'SEM': s.m = 1
        elif n == 'CLP': s.setP(s.getP() & ~s.fb() & 0xFFFF)
        elif n == 'SEP': s.setP(s.getP() | s.fb())
        elif n == 'LDT': s.DT = s.fb()
        elif n == 'MPY':
            v = s.operand(mode, wa)
            if wa == 1: p = (s.A & 0xFF) * (v & 0xFF); s.setr('A', p, 1); s.setr('B', p >> 8, 1)
            else: p = s.A * v; s.A = p & 0xFFFF; s.B = p >> 16 & 0xFFFF
            s.nz(s.A, wa)
        elif n == 'DIV':
            v = s.operand(mode, wa)
            if wa == 1:
                num = (s.B & 0xFF) << 8 | (s.A & 0xFF); v &= 0xFF
                if v == 0: raise Stop()
                s.setr('A', num // v, 1); s.setr('B', num % v, 1)
            else:
                num = s.B << 16 | s.A
                if v == 0: raise Stop()
                s.A = (num // v) & 0xFFFF; s.B = num % v
            s.nz(s.A, wa)
        elif n == 'NOP': pass
        elif n in ('WIT',): pass
        elif n in ('STP', 'BRK'): raise Stop()
        elif n in ('MVN', 'MVP'):
            dst = s.fb(); src = s.fb()
            while True:
                s.wr8(dst << 16 | s.Y, s.rd8(src << 16 | s.X))
                dd = 1 if n == 'MVN' else -1
                s.X = (s.X + dd) & 0xFFFF; s.Y = (s.Y + dd) & 0xFFFF
                s.A = (s.A - 1) & 0xFFFF
                if s.A == 0xFFFF: break
            s.DT = dst
        else:
            raise NotImplementedError('%06X %s %s' % (a, n, mode))

d77_br = {'BEQ': ('Z', False), 'BNE': ('Z', True), 'BCS': ('C', False), 'BCC': ('C', True),
          'BMI': ('N', False), 'BPL': ('N', True), 'BVS': ('V', False), 'BVC': ('V', True)}

def lcg(c):
    c.seed = (c.seed * 1103515245 + 12345) & 0xFFFFFFFF
    return c.seed >> 16 & 0xFFFF

def funcs(path, table, trials, outp):
    import re
    ents = [tuple(int(v, 0) for v in mm.groups()) for mm in
            re.finditer(r'\{(0x[0-9A-F]+), \w+, (\d), (\d), (\d)\}', open(table).read())]
    out = open(outp, 'w')
    c = CPU(path, 3000, out)
    for addr, ms, rs, isr in ents:
        for k in range(trials):
            c.seed = (addr * 7 + k * 1000003) & 0xFFFFFFFF
            for a in range(0x80, 0x880): c.ram[a] = lcg(c) & 0xFF
            if addr < 0x2000:      # RAM-resident flash loader: install its code first
                for a in range(0x10100, 0x105C8): c.ram[a - 0x10100 + 0x80] = c.rom[a]
            c.A = lcg(c); c.B = lcg(c); c.X = lcg(c) & 0x0E; c.Y = lcg(c); c.DT = 0; c.x = 0; c.I = 1; c.IPL = 0
            c.C = c.Z = c.N = c.V = 0; c.PG = addr >> 16
            c.m = 1 if ms == 2 else ((lcg(c) & 1) if ms == 3 else 0)
            c.D = 0x0640; c.S = 0x0870
            if not isr:
                c.ram[0x870] = 0x06; c.ram[0x86F] = 0x40; c.S = 0x086E
                sent = 0xFFF0
                if rs == 3: c.push8(0, False)
                c.push16(sent, False)
            c.PC = addr & 0xFFFF; c.n = 0; c.nrd = 0
            out.write('T %06X %d\n' % (addr, k))
            st = 'T'
            try:
                for _ in range(300000):
                    if not isr and c.PC == 0xFFF0 and c.PG == 0: st = 'R'; break
                    c.step()
            except Stop as e:
                st = e.code
            except Exception as e:
                st = 'X'
            out.write('E %s %04X %04X %04X %04X %04X %04X %d\n' % (st, c.A, c.B, c.X, c.Y, c.D, c.S, c.m))
    out.close()
    print('unique instructions executed:', len(c.cov))

def main():
    if sys.argv[1] == '--funcs':
        funcs(sys.argv[2], sys.argv[3], int(sys.argv[4]), sys.argv[5]); return
    path, limit, outp = sys.argv[1], int(sys.argv[2]), sys.argv[3]
    with open(outp, 'w') as out:
        c = CPU(path, limit, out)
        try:
            while True: c.step()
        except Stop:
            pass
    print('steps', c.steps, 'events', c.n, 'PC %02X%04X' % (c.PG, c.PC), 'unique insns', len(c.cov))
    if os.environ.get('COV'):
        with open(os.environ['COV'], 'w') as f: f.write('\n'.join('%06X' % a for a in sorted(c.cov)))

if __name__ == '__main__':
    main()
