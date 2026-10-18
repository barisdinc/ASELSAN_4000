#!/usr/bin/env python3
"""a77.py - MELPS 7700 (M37732) assembler.

The syntax is the one d77.py prints, so a listing can be assembled back
(`make test-a77` rebuilds YA4000S.asm and compares it byte by byte with the HEX):

    LABEL:  LDA   A,$04,S          ; accumulator A / B is written as the first operand
            LDA   B,#$0000
            SEB   #$0200,$023C     ; bit ops: #mask,address
            BBC   #$40,$0340,SKIP  ; branch on bit: #mask,address,target
            LDM   #$00,$0227       ; load memory: #value,address
            LDA   A,L($0E)         ; long indirect;  ($0E) ($0E,X) ($0E),Y ($02,S),Y
            LDA   A,$0031EC,X      ; dp / abs / long chosen from the number of hex digits
            JSRL  F_00828C
            .ASSUME M=1,X=0        ; data / index register length (SEM CLM SEP CLP are tracked)

Address size: a literal `$12` is direct page, `$1234` absolute, `$123456` long; a
decimal number or a label takes the smallest size that holds its value. The width
of an .EQU symbol is that of its definition. Force a size with a prefix:
`<expr` direct page, `!expr` absolute, `>expr` long.

Expressions: + - * / % & | ^ ~ << >> ( ), numbers $1F 0x1F %101 31 'A',
lo(x) hi(x) bank(x), `*` = address of the current line.

Directives:
    .ORG addr             .EQU / =        .ASSUME M=m,X=x
    .BYTE 1,"text",lo(L)  .WORD L1,L2     .LONG L1 (3 bytes, bank last)   .DWORD n (4 bytes)
    .FILL n[,value]       .TEXT8 "abc"    (8 characters, padded with blanks: one LCD screen)
    .COPY src,n           n bytes copied from the original image (yama.py supplies it)
    .INCLUDE "file"       insert another source (path relative to this file)
    .SERBEST a,n,b1,...   release n original bytes at a (first bytes checked; yama.py fills
                          the rest of the region with 0xFF and lets new code use it)
    .EXPECT b1,b2,...     the original image must hold these bytes here; it also allows
                          the following code to overwrite them (see yama.py)

    python3 tools/a77.py source.asm [-l listing.lst] [-D NAME=value ...]
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from d77 import OPS, B_IMP, P89, ACC_OPS   # opcode tables shared with the disassembler

# ---------------------------------------------------------------- opcode index
# (NAME, mode) -> opcode prefix bytes;  accumulator-B forms use mode + '/B'
ENC = {}
for op, (n, m) in OPS.items():
    ENC[(n, m)] = bytes([op])
    if n in ACC_OPS and m != 'acc':
        ENC[(n, m + '/B')] = bytes([0x42, op])
for op, (n, m) in B_IMP.items():
    ENC[(n, m)] = bytes([0x42, op])
for op, (n, m) in P89.items():
    ENC[(n, m)] = bytes([0x89, op])
MNEMONICS = {n for n, _ in ENC}
OP_SIZE = {'dp': 1, 'dpx': 1, 'dpy': 1, 'ind': 1, 'indx': 1, 'indy': 1, 'lind': 1, 'lindy': 1,
           'sr': 1, 'sry': 1, 'abs': 2, 'absx': 2, 'absy': 2, 'long': 3, 'longx': 3,
           'jabs': 2, 'jlong': 3, 'jind': 2, 'jindx': 2, 'jlind': 2}


class AsmError(Exception):
    pass


# ---------------------------------------------------------------- expressions
TOK = re.compile(r"\s*(?:(\$[0-9A-Fa-f]+|0[xX][0-9A-Fa-f]+|%[01]+|\d+)|('(?:\\.|[^'])')|"
                 r"([A-Za-z_.@][\w.@]*)|(<<|>>|[-+*/%&|^~()]))")


def tdiv(x, y):
    """integer division truncating toward zero (like C)"""
    q = abs(x) // abs(y)
    return q if (x < 0) == (y < 0) else -q


class Val:
    """value + width in bytes (1 dp, 2 abs, 3 long; None = unknown yet)"""
    __slots__ = ('v', 'w')

    def __init__(s, v, w):
        s.v, s.w = v, w


def width_of(v):
    return 1 if 0 <= v <= 0xFF else 2 if 0 <= v <= 0xFFFF else 3


class Expr:
    def __init__(s, asm, text):
        s.asm, s.text = asm, text
        s.toks = []
        p = 0
        text = text.strip()
        while p < len(text):
            m = TOK.match(text, p)
            if not m or m.end() == p:
                raise AsmError('ifade anlaşılmadı: %r' % text)
            s.toks.append(m.groups())
            p = m.end()
        s.i = 0

    def peek(s):
        return s.toks[s.i] if s.i < len(s.toks) else (None, None, None, None)

    def take(s):
        t = s.peek()
        s.i += 1
        return t

    def parse(s):
        v = s.binary(0)
        if s.i != len(s.toks):
            raise AsmError('ifadenin sonunda fazlalık: %r' % s.text)
        return v

    PREC = {'|': 1, '^': 2, '&': 3, '<<': 4, '>>': 4, '+': 5, '-': 5, '*': 6, '/': 6, '%': 6}

    def binary(s, minp):
        a = s.unary()
        while True:
            op = s.peek()[3]
            if op not in s.PREC or s.PREC[op] < minp:
                return a
            s.take()
            b = s.binary(s.PREC[op] + 1)
            if a.v is None or b.v is None:
                a = Val(None, None)
                continue
            x, y = a.v, b.v
            if op in '/%' and y == 0:
                raise AsmError('sıfıra bölme: %r' % s.text)
            r = {'|': lambda: x | y, '^': lambda: x ^ y, '&': lambda: x & y, '<<': lambda: x << y,
                 '>>': lambda: x >> y, '+': lambda: x + y, '-': lambda: x - y, '*': lambda: x * y,
                 '/': lambda: tdiv(x, y), '%': lambda: x - tdiv(x, y) * y}[op]()
            ws = [w for w in (a.w, b.w) if w]
            a = Val(r, max(ws) if ws else None)

    def unary(s):
        num, ch, ident, op = s.take()
        if op == '-':
            a = s.unary()
            return Val(None if a.v is None else -a.v, a.w)
        if op == '~':
            a = s.unary()
            return Val(None if a.v is None else ~a.v, a.w)
        if op == '+':
            return s.unary()
        if op == '(':
            a = s.binary(0)
            if s.take()[3] != ')':
                raise AsmError('")" eksik: %r' % s.text)
            return a
        if op == '*':
            return Val(s.asm.pc, width_of(s.asm.pc))
        if num:
            if num[0] == '$':
                return Val(int(num[1:], 16), min(3, (len(num) - 1 + 1) // 2))
            if num[:2] in ('0x', '0X'):
                v = int(num, 16)
            elif num[0] == '%':
                v = int(num[1:], 2)
            else:
                v = int(num)
            return Val(v, width_of(v))
        if ch:
            c = ch[1:-1]
            c = {'\\n': '\n', "\\'": "'", '\\\\': '\\'}.get(c, c)
            return Val(ord(c), 1)
        if ident:
            low = ident.lower()
            if low in ('lo', 'hi', 'bank') and s.peek()[3] == '(':
                s.take()
                a = s.binary(0)
                if s.take()[3] != ')':
                    raise AsmError('")" eksik: %r' % s.text)
                if a.v is None:
                    return Val(None, 1)
                return Val({'lo': a.v & 0xFF, 'hi': a.v >> 8 & 0xFF, 'bank': a.v >> 16 & 0xFF}[low], 1)
            return s.asm.symbol(ident)
        raise AsmError('ifade anlaşılmadı: %r' % s.text)


# ---------------------------------------------------------------- operand syntax
def split_commas(s):
    out, depth, cur, q = [], 0, '', None
    for c in s:
        if q:
            cur += c
            if c == q:
                q = None
            continue
        if c in '"\'':
            q = c
        elif c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
        if c == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += c
    if cur.strip() or out:
        out.append(cur.strip())
    return out


def classify(opnd):
    """-> (candidate modes, [expression texts])"""
    o = opnd.strip()
    u = o.upper().replace(' ', '')
    if o == '':
        return ['imp', 'brk'], []
    if u == 'A':
        return ['acc'], []
    if u == 'B':
        return ['accB'], []
    parts = split_commas(o)
    if o.startswith('#'):
        if len(parts) == 1:
            return ['immM', 'immX', 'imm8', 'imm16', 'brk'], [o[1:]]
        if len(parts) == 2:
            return ['bitdp', 'bitabs', 'ldmdp', 'ldmabs'], [parts[0][1:], parts[1]]
        if len(parts) == 3 and parts[2].upper() == 'X':
            return ['ldmdpx', 'ldmabsx'], [parts[0][1:], parts[1]]
        if len(parts) == 3:
            return ['bbdp', 'bbabs'], [parts[0][1:], parts[1], parts[2]]
    m = re.fullmatch(r'L\((.*)\)', o, re.I)
    if m:
        return ['lind', 'jlind'], [m.group(1)]
    m = re.fullmatch(r'L\((.*)\)\s*,\s*Y', o, re.I)
    if m:
        return ['lindy'], [m.group(1)]
    m = re.fullmatch(r'\((.*)\s*,\s*S\s*\)\s*,\s*Y', o, re.I)
    if m:
        return ['sry'], [m.group(1)]
    m = re.fullmatch(r'\((.*)\)\s*,\s*Y', o, re.I)
    if m:
        return ['indy'], [m.group(1)]
    m = re.fullmatch(r'\((.*)\s*,\s*X\s*\)', o, re.I)
    if m:
        return ['indx', 'jindx'], [m.group(1)]
    m = re.fullmatch(r'\((.*)\)', o)
    if m and len(split_commas(m.group(1))) == 1:
        return ['ind', 'jind'], [m.group(1)]
    if len(parts) == 2 and parts[1].upper() == 'S':
        return ['sr'], [parts[0]]
    if len(parts) == 2 and parts[1].upper() == 'X':
        return ['dpx', 'absx', 'longx'], [parts[0]]
    if len(parts) == 2 and parts[1].upper() == 'Y':
        return ['dpy', 'absy'], [parts[0]]
    if len(parts) == 2:
        return ['mv'], parts
    return ['dp', 'abs', 'long', 'rel8', 'rel16', 'jabs', 'jlong'], [o]


# for check_flow: instructions whose behaviour does not depend on M / that depend on X
FLAG_OPS = {'CLM', 'SEM', 'SEP', 'CLP'}
FLOW = {'BPL', 'BMI', 'BVC', 'BVS', 'BCC', 'BCS', 'BNE', 'BEQ', 'BRA', 'BRAL', 'JMP', 'JMPL',
        'JSR', 'JSRL', 'RTS', 'RTL', 'RTI'}
X_REGS = {'LDX', 'LDY', 'STX', 'STY', 'CPX', 'CPY', 'INX', 'INY', 'DEX', 'DEY', 'TAX', 'TAY', 'TXA',
          'TYA', 'TXY', 'TYX', 'TXS', 'TSX', 'PHX', 'PHY', 'PLX', 'PLY', 'TXB', 'TYB', 'TBX', 'TBY',
          'MVN', 'MVP'}
M_FREE = FLAG_OPS | FLOW | (X_REGS - {'TAX', 'TAY', 'TXA', 'TYA', 'TXB', 'TYB', 'TBX', 'TBY'}) | {
    'SEC', 'CLC', 'SEI', 'CLI', 'CLV', 'NOP', 'PHP', 'PLP', 'PHD', 'PLD', 'PHG', 'PHT', 'PLT', 'WIT', 'STP'}
INDEXED = {'dpx', 'dpy', 'absx', 'absy', 'longx', 'indx', 'indy', 'lindy', 'sry', 'ldmdpx', 'ldmabsx', 'jindx'}

SIZE_GROUPS = [('dp', 'abs', 'long'), ('dpx', 'absx', 'longx'), ('dpy', 'absy'),
               ('bitdp', 'bitabs'), ('bbdp', 'bbabs'), ('ldmdp', 'ldmabs'), ('ldmdpx', 'ldmabsx')]


# ---------------------------------------------------------------- assembler
class Asm:
    def __init__(s, defines=None, base=None):
        s.defines = dict(defines or {})
        s.base = base            # original image (addr -> byte) for .COPY

    def symbol(s, name):
        if name in s.syms:
            v, w = s.syms[name]
            return Val(v, w)
        if name in s.prev_syms:
            v, w = s.prev_syms[name]
            return Val(v, w)
        s.unknown.add(name)
        return Val(None, None)

    def value(s, text, need=True):
        force = None
        t = text.strip()
        if t[:1] in '<!>' and t[:2] not in ('<<', '>>'):
            force = {'<': 1, '!': 2, '>': 3}[t[0]]
            t = t[1:]
        v = Expr(s, t).parse()
        if v.v is None and need and s.final:
            raise AsmError('tanımsız sembol: %s' % (', '.join(sorted(s.unknown_labels())) or text))
        if force:
            v.w = force
        return v

    def emit(s, data):
        for b in data:
            if s.pc in s.mem and s.final:
                raise AsmError('0x%06X adresine iki kez yazılıyor' % s.pc)
            s.mem[s.pc] = b & 0xFF
            s.pc += 1

    def assemble(s, text, name='<asm>', folder='.'):
        """Assemble until label values stop moving; returns self (mem, syms, listing)."""
        s.prev_syms = {}
        lines = expand_includes(text, name, folder)
        for it in range(12):
            s.final = False
            s.run(lines, name)
            if s.syms == s.prev_syms and not s.unknown_labels():
                break
            s.prev_syms = dict(s.syms)
        s.final = True
        s.run(lines, name)
        s.flow_warnings = s.check_flow()
        return s

    def check_flow(s):
        """A branch or jump must arrive with the M/X the target was assembled for, at least
        until an instruction whose encoding depends on it (immediate, bit op, LDM).  The
        assembler follows SEM/CLM line by line; a label reached from elsewhere needs .ASSUME."""
        out = []
        for tgt, m, x, src in s.flow_refs:
            bad = s.flow_conflict(tgt, m, x)
            if bad:
                name = next((k for k, (v, w) in s.syms.items() if v == tgt), '0x%06X' % tgt)
                out.append('%s: buraya M=%d,X=%d ile atlanıyor ama 0x%06X %s M=%d,X=%d için birleştirildi '
                           '(etikete .ASSUME ekleyin)\n    %s' % (name, m, x, bad[0], bad[1], bad[2], bad[3], src.strip()))
        return out

    def flow_conflict(s, a, m, x):
        """walk from a with the real flags (m, x) until they agree with the assembled ones"""
        for _ in range(64):
            if a not in s.state_at or s.state_at[a] == (m, x):
                return None
            name, mode, n, exprs = s.insn_at[a]
            am, ax = s.state_at[a]
            if (am != m and name not in M_FREE) or (ax != x and (name in X_REGS or mode in INDEXED)):
                return (a, name, am, ax)
            if name == 'CLM':
                m = 0
            elif name == 'SEM':
                m = 1
            elif name in ('SEP', 'CLP'):
                v = s.value(exprs[0]).v
                if v & 0x20:
                    m = 1 if name == 'SEP' else 0
                if v & 0x10:
                    x = 1 if name == 'SEP' else 0
            elif name in ('BRA', 'BRAL', 'JMP', 'JMPL', 'RTS', 'RTL', 'RTI', 'STP'):
                return None    # the jump target is checked on its own
            a += n
        return None

    def unknown_labels(s):
        return {u for u in s.unknown if u not in s.syms}

    def run(s, lines, name):
        s.mem, s.syms, s.unknown = {}, {}, set()
        for k, v in s.defines.items():
            s.syms[k] = (v, width_of(v))
        s.pc, s.m, s.x = 0, 0, 0
        s.listing, s.expects, s.released = [], [], []
        s.state_at, s.flow_refs = {}, []     # M/X at each instruction; (target, m, x, line) of jumps
        s.insn_at = {}                       # addr -> (name, mode, length, operand text)
        s.cur_line = ''
        for fname, no, raw in lines:
            try:
                s.line(raw)
            except AsmError as e:
                if s.final:
                    raise AsmError('%s:%d: %s\n    %s' % (fname, no, e, raw.strip()))

    def define(s, name, v, w):
        if s.final and name in s.syms and s.syms[name][0] != v:
            raise AsmError('sembol iki kez tanımlandı: %s' % name)
        s.syms[name] = (v, w)

    def line(s, raw):
        code = strip_comment(raw)
        start = s.pc
        m = re.match(r'^\s*([A-Za-z_.@][\w.@]*):', code)
        if m:
            s.define(m.group(1), s.pc, width_of(s.pc))
            code = code[m.end():]
        if not code.strip():
            return
        m = re.match(r'^\s*([A-Za-z_][\w]*)\s*(?:\.EQU\b|=)\s*(.*)$', code, re.I)
        if m:
            v = s.value(m.group(2), need=False)
            if v.v is not None:
                s.define(m.group(1), v.v, v.w)
            return
        m = re.match(r'^\s*(\.?[A-Za-z][\w]*)\s*(.*)$', code)
        if not m:
            raise AsmError('satır anlaşılmadı')
        op, args = m.group(1).upper(), m.group(2).strip()
        if op.startswith('.'):
            s.directive(op, args)
        else:
            s.state_at[s.pc] = (s.m, s.x)
            s.cur_line = raw
            s.instruction(op, args)
        if s.final and s.pc != start:
            s.listing.append((start, bytes(s.mem.get(a, 0) for a in range(start, s.pc)), raw.rstrip()))
        elif s.final:
            s.listing.append((start, b'', raw.rstrip()))

    def directive(s, op, args):
        if op == '.ORG':
            s.pc = s.value(args).v
        elif op == '.ASSUME':
            for k, v in re.findall(r'([MX])\s*=\s*([01])', args.upper()):
                setattr(s, k.lower(), int(v))
        elif op in ('.BYTE', '.DB'):
            for a in split_commas(args):
                if a[:1] == '"':
                    s.emit(parse_string(a))
                else:
                    v = s.value(a).v
                    s.emit([0 if v is None else v])
                    if s.final and not -128 <= v <= 255:
                        raise AsmError('bayt sınırı dışında: %s' % a)
        elif op == '.TEXT8':
            t = parse_string(args)
            if len(t) > 8:
                raise AsmError('8 karakterden uzun: %s' % args)
            s.emit(t.ljust(8, b' '))
        elif op in ('.WORD', '.DW'):
            for a in split_commas(args):
                v = s.value(a).v or 0
                s.emit([v, v >> 8])
        elif op in ('.DWORD', '.DD'):
            for a in split_commas(args):
                v = s.value(a).v or 0
                s.emit([v, v >> 8, v >> 16, v >> 24])
        elif op in ('.LONG', '.ADDR'):
            for a in split_commas(args):
                v = s.value(a).v or 0
                s.emit([v, v >> 8, v >> 16])
        elif op == '.FILL':
            p = split_commas(args)
            n = s.value(p[0]).v
            v = s.value(p[1]).v if len(p) > 1 else 0xFF
            s.emit([v] * n)
        elif op == '.COPY':
            p = split_commas(args)
            src, n = s.value(p[0]).v, s.value(p[1]).v
            if s.base is None:
                raise AsmError('.COPY için orijinal görüntü gerekli (yama.py ile çalıştırın)')
            if any(a not in s.base for a in range(src, src + n)):
                raise AsmError('.COPY: 0x%06X..0x%06X orijinal görüntüde yok' % (src, src + n - 1))
            s.emit([s.base[a] for a in range(src, src + n)])
        elif op == '.EXPECT':
            data = bytes(s.value(a).v & 0xFF for a in split_commas(args))
            s.expects.append((s.pc, data))
        elif op == '.SERBEST':
            # .SERBEST adres,uzunluk,b1,b2,...: orijinaldeki bu bölge artık kullanılmıyor (yama.py
            # baştaki baytları denetler, bölgeyi 0xFF yapar; yeni kod buraya yazılabilir)
            p = split_commas(args)
            at, n = s.value(p[0]).v, s.value(p[1]).v
            data = bytes(s.value(a).v & 0xFF for a in p[2:])
            if not data:
                raise AsmError('.SERBEST: bölgenin ilk baytları verilmeli (yanlış adresi silmemek için)')
            s.expects.append((at, data))
            s.released.append((at, n))
        elif op == '.END':
            pass
        else:
            raise AsmError('bilinmeyen yönerge %s' % op)

    def instruction(s, name, args):
        if name not in MNEMONICS:
            raise AsmError('bilinmeyen komut %s' % name)
        reg = None
        if name in ACC_OPS:
            p = split_commas(args)
            if p and p[0].upper() in ('A', 'B') and len(p) > 1:
                reg = p[0].upper()
                args = args[args.index(',') + 1:].strip()
        modes, exprs = classify(args)
        avail = [md for md in modes if (name, md + ('/B' if reg == 'B' else '')) in ENC]
        if not avail:
            raise AsmError('%s için bu adresleme yok: %r' % (name, args))
        mode = s.pick(name, avail, exprs, reg)
        key = (name, mode + ('/B' if reg == 'B' else ''))
        at = s.pc
        s.encode(ENC[key], name, mode, exprs)
        s.insn_at[at] = (name, mode, s.pc - at, exprs)
        if name == 'CLM':
            s.m = 0
        elif name == 'SEM':
            s.m = 1
        elif name in ('SEP', 'CLP'):
            v = s.value(exprs[0]).v or 0
            if v & 0x20:
                s.m = 1 if name == 'SEP' else 0
            if v & 0x10:
                s.x = 1 if name == 'SEP' else 0

    def pick(s, name, avail, exprs, reg):
        if s.final and avail and set(avail) <= {'dpy', 'absy'}:
            # ör. LDA A,>tablo,Y: uzun adres + Y modu yok; sessizce kısaltmak başka banka okur
            v = s.value(exprs[0], need=False)
            if exprs[0].strip()[:1] == '>' or (v.v is not None and v.v > 0xFFFF):
                raise AsmError('%s: bu indeksle uzun adres yok (%r); X kullanın' % (name, exprs[0]))
        if len(avail) == 1:
            return avail[0]
        for md in ('rel8', 'rel16', 'jabs', 'jlong', 'jind', 'jindx', 'jlind', 'imp', 'brk'):
            if md in avail:
                return md
        for g in SIZE_GROUPS:
            cands = [md for md in g if md in avail]
            if not cands:
                continue
            ad = exprs[1] if g[0].startswith(('bit', 'bb', 'ldm')) else exprs[0]
            v = s.value(ad, need=False)
            w = v.w if v.w else (2 if v.v is None else width_of(v.v))
            for md in g[w - 1 if w <= len(g) else len(g) - 1:]:
                if md in cands:
                    return md
            if s.final:
                raise AsmError('%s: adres boyu uygun değil (%r)' % (name, ad))
            return cands[-1]
        return avail[0]

    def encode(s, pre, name, mode, exprs):
        end = s.pc + len(pre)        # filled in as operands are counted
        out = list(pre)

        def val(t):
            v = s.value(t).v
            return 0 if v is None else v

        def le(v, n):
            if s.final and not -(1 << (8 * n - 1)) <= v < (1 << (8 * n)):
                raise AsmError('değer %d bayta sığmıyor: %d' % (n, v))
            return [(v >> (8 * i)) & 0xFF for i in range(n)]

        def rel(t, n, length):
            tgt = val(t)
            here = s.pc + length
            if s.final and (tgt >> 16) != (s.pc >> 16):
                raise AsmError('dallanma başka banka: 0x%06X' % tgt)
            d = (tgt & 0xFFFF) - (here & 0xFFFF)
            if n == 1 and s.final and not -128 <= d <= 127:
                raise AsmError('dallanma çok uzak (%d)' % d)
            return le(d & ((1 << (8 * n)) - 1), n)

        szM, szX = (1 if s.m else 2), (1 if s.x else 2)
        if mode in ('imp', 'acc', 'accB'):
            pass
        elif mode == 'brk':
            out += le(val(exprs[0]), 1) if exprs else [0]
        elif mode == 'immM':
            out += le(val(exprs[0]) & ((1 << 8 * szM) - 1), szM)
        elif mode == 'immX':
            out += le(val(exprs[0]) & ((1 << 8 * szX) - 1), szX)
        elif mode == 'imm8':
            out += le(val(exprs[0]) & 0xFF, 1)
        elif mode == 'imm16':
            out += le(val(exprs[0]) & 0xFFFF, 2)
        elif mode in ('rel8', 'rel16'):
            n = 1 if mode == 'rel8' else 2
            out += rel(exprs[0], n, len(out) + n)
            if name != 'PER':
                s.flow_refs.append((val(exprs[0]), s.m, s.x, s.cur_line))
        elif mode == 'jabs':
            t = val(exprs[0])
            s.flow_refs.append((t, s.m, s.x, s.cur_line))
            if s.final and (t >> 16) != (s.pc >> 16):
                raise AsmError('%s başka bankaya gidemez (JSRL/JMPL kullanın): 0x%06X' % (name, t))
            out += le(t & 0xFFFF, 2)
        elif mode == 'jlong':
            out += le(val(exprs[0]), 3)
            s.flow_refs.append((val(exprs[0]), s.m, s.x, s.cur_line))
        elif mode in OP_SIZE:
            n = OP_SIZE[mode]
            out += le(val(exprs[0]) & ((1 << 8 * n) - 1), n)
        elif mode in ('bitdp', 'bitabs', 'bbdp', 'bbabs', 'ldmdp', 'ldmabs', 'ldmdpx', 'ldmabsx'):
            n = 1 if 'dp' in mode else 2
            out += le(val(exprs[1]) & ((1 << 8 * n) - 1), n)
            out += le(val(exprs[0]) & ((1 << 8 * szM) - 1), szM)
            if mode.startswith('bb'):
                out += rel(exprs[2], 1, len(out) + 1)
                s.flow_refs.append((val(exprs[2]), s.m, s.x, s.cur_line))
        elif mode == 'mv':
            out += le(val(exprs[0]), 1) + le(val(exprs[1]), 1)
        else:
            raise AsmError('kodlanamayan mod %s' % mode)
        s.emit(out)


def strip_comment(line):
    q = None
    for i, c in enumerate(line):
        if q:
            if c == q:
                q = None
        elif c in '"\'':
            # a char literal like ';' or a string
            q = c
        elif c == ';':
            return line[:i]
    return line


def parse_string(a):
    a = a.strip()
    if len(a) < 2 or a[0] != '"' or a[-1] != '"':
        raise AsmError('metin "..." biçiminde olmalı: %s' % a)
    return a[1:-1].encode('latin-1').decode('unicode_escape').encode('latin-1')


def expand_includes(text, name, folder, depth=0):
    """-> [(file name, line number, text)] with every .INCLUDE "file" replaced by its lines
    (path relative to the including file)"""
    out = []
    for no, raw in enumerate(text.splitlines(), 1):
        m = re.match(r'^\s*\.INCLUDE\s+"([^"]+)"', strip_comment(raw), re.I)
        if not m:
            out.append((name, no, raw))
            continue
        if depth > 8:
            raise AsmError('%s:%d: .INCLUDE çok derin' % (name, no))
        path = os.path.join(folder, m.group(1))
        try:
            sub = open(path, encoding='utf-8').read()
        except OSError as e:
            raise AsmError('%s:%d: %s' % (name, no, e))
        out += expand_includes(sub, m.group(1), os.path.dirname(path), depth + 1)
    return out


def assemble_file(path, defines=None, base=None):
    return Asm(defines, base).assemble(open(path, encoding='utf-8').read(), os.path.basename(path),
                                       os.path.dirname(os.path.abspath(path)))


def write_listing(a, path):
    with open(path, 'w') as f:
        for addr, data, src in a.listing:
            hx = ' '.join('%02X' % b for b in data[:6]) + (' …' if len(data) > 6 else '')
            f.write('%06X  %-20s %s\n' % (addr, hx if data else '', src))
        f.write('\n; semboller\n')
        for k, (v, w) in sorted(a.syms.items(), key=lambda kv: kv[1][0]):
            f.write('%-24s $%06X\n' % (k, v))


if __name__ == '__main__':
    import argparse
    ap = argparse.ArgumentParser(description='MELPS 7700 assembler')
    ap.add_argument('src')
    ap.add_argument('-l', '--list')
    ap.add_argument('-D', action='append', default=[], metavar='NAME=VALUE')
    o = ap.parse_args()
    defs = {}
    for d in o.D:
        k, v = d.split('=')
        defs[k] = int(v, 0) if not v.startswith('$') else int(v[1:], 16)
    try:
        a = assemble_file(o.src, defs)
    except AsmError as e:
        sys.exit('hata: %s' % e)
    if o.list:
        write_listing(a, o.list)
    ks = sorted(a.mem)
    print('%s: %d bayt, 0x%06X..0x%06X, %d sembol' % (o.src, len(ks), ks[0], ks[-1], len(a.syms)) if ks
          else '%s: kod yok' % o.src)
