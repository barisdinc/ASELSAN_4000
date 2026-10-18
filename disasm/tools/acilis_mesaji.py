#!/usr/bin/env python3
"""acilis_mesaji.py - firmware with a custom power-on message sequence.

Replaces F_00E0E1 (shows the version "AS V2-3" at power on, called once from
F_00E305 at 0x00E431) with a loop that shows up to 4 texts of 8 characters,
each for DELAY_MS, then returns; the rest of the start-up (PN / SN / CN, channel)
is unchanged.  The ROM checksum (sum of 0x2000..0xFFFF = BE word at 0x010003,
checked at power on, "ERR FLSH" if wrong) is corrected.

    python3 tools/acilis_mesaji.py ../YA4000S.HEX out.HEX "MERHABA" "DUNYA" "YENIDEN" "ATOLYE"

The LCD font only has 7-bit ASCII (upper case, digits, some signs); Turkish
letters are written without dots (U, O, I, S, G, C).
"""
import sys

FUNC = 0x00E0E1          # F_00E0E1 .. 0x00E152 (114 bytes)
FUNC_END = 0x00E153
LCD_UPDATE = 0x00828C    # F_00828C: text buffer 0x0252..0x0259 -> PCF8576
DELAY = 0x007D7F         # F_007D7F: Y = milliseconds, kicks the watchdog
DELAY_MS = 800
SUM_AT = 0x010003

TR = str.maketrans('ÇĞİÖŞÜçğıöşü', 'CGIOSUCGIOSU')


def code(n_msgs, table):
    """M37700 machine code, entered with M = 0, X = 0 like every NC77 function.
    The first word on the caller's stack is the next free direct-page frame;
    the prologue/epilogue follow the compiler (frame of 2 bytes, X kept in $00
    across the calls because the callees read their frame pointer at 4,S)."""
    t = table
    c = bytes([
        0xA3, 0x04,                     # LDA A,$04,S     prologue
        0x0B,                           # PHD
        0x5B,                           # TAD
        0x3A, 0x3A,                     # INC A; INC A    2-byte frame
        0x48,                           # PHA
        0xA2, 0x00, 0x00,               # LDX #0          table offset
        # outer:
        0xA0, 0x00, 0x00,               # LDY #0
        # inner:
        0xF8,                           # SEM
        0xBF, t & 0xFF, t >> 8 & 0xFF, t >> 16,   # LDA A,table,X (long)
        0x99, 0x52, 0x02,               # STA A,$0252,Y
        0xD8,                           # CLM
        0xE8,                           # INX
        0xC8,                           # INY
        0xC0, 0x08, 0x00,               # CPY #8
        0xD0, 0xF0,                     # BNE inner
        0x86, 0x00,                     # STX $00
        0x22, LCD_UPDATE & 0xFF, LCD_UPDATE >> 8 & 0xFF, LCD_UPDATE >> 16,   # JSRL F_00828C
        0xA0, DELAY_MS & 0xFF, DELAY_MS >> 8,                                # LDY #ms
        0x22, DELAY & 0xFF, DELAY >> 8 & 0xFF, DELAY >> 16,                  # JSRL F_007D7F
        0xA6, 0x00,                     # LDX $00
        0xE0, n_msgs * 8, 0x00,         # CPX #n*8
        0xD0, 0xD9,                     # BNE outer
        0x7A,                           # PLY             epilogue
        0x2B,                           # PLD
        0x6B,                           # RTL
    ])
    return c


def read_srec(path):
    recs, mem = [], {}
    for line in open(path):
        line = line.strip()
        recs.append(line)
        if line[:2] in ('S1', 'S2', 'S3'):
            al = {'S1': 2, 'S2': 3, 'S3': 4}[line[:2]]
            n = int(line[2:4], 16)
            a = int(line[4:4 + al * 2], 16)
            data = bytes.fromhex(line[4 + al * 2:4 + n * 2 - 2])
            for i, v in enumerate(data):
                mem[a + i] = v
    return recs, mem


def write_srec(recs, mem, path):
    out = []
    for line in recs:
        if line[:2] in ('S1', 'S2', 'S3'):
            al = {'S1': 2, 'S2': 3, 'S3': 4}[line[:2]]
            n = int(line[2:4], 16)
            a = int(line[4:4 + al * 2], 16)
            data = bytes(mem[a + i] for i in range(n - al - 1))
            body = bytes([n]) + a.to_bytes(al, 'big') + data
            line = line[:2] + body.hex().upper() + '%02X' % (~sum(body) & 0xFF)
        out.append(line)
    eol = '\r\n' if b'\r\n' in open(path_in, 'rb').read(4096) else '\n'
    open(path, 'w', newline='').write(eol.join(out) + eol)


def rom_sum(mem):
    return sum(mem.get(a, 0xFF) for a in range(0x2000, 0x10000)) & 0xFFFF


if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    path_in, path_out, texts = sys.argv[1], sys.argv[2], sys.argv[3:]
    if len(texts) > 4:
        sys.exit('en fazla 4 mesaj (yer: 0x%06X..0x%06X)' % (FUNC, FUNC_END))
    msgs = []
    for t in texts:
        t = t.translate(TR).upper()
        if len(t) > 8 or any(not (32 <= ord(ch) < 127) for ch in t):
            sys.exit('mesaj en çok 8 ASCII karakter olmalı: %r' % t)
        msgs.append(t.center(8).encode())

    recs, mem = read_srec(path_in)
    if mem.get(FUNC) != 0xA3 or mem.get(0x00E0E7) != 0xA0 or mem.get(0x00E0EA) != 0x22:
        sys.exit('F_00E0E1 beklenen yerde değil (bu araç YA4000x firmware ailesi içindir)')
    old_sum = rom_sum(mem)
    if old_sum != (mem[SUM_AT] << 8 | mem[SUM_AT + 1]):
        print('uyarı: giriş dosyasının sağlama toplamı zaten yanlış')

    c = code(len(msgs), 0)                     # length first, table right after the code
    table = FUNC + len(c)
    c = code(len(msgs), table) + b''.join(msgs)
    if FUNC + len(c) > FUNC_END:
        sys.exit('kod sığmıyor')
    for i, v in enumerate(c):
        mem[FUNC + i] = v
    for a in range(FUNC + len(c), FUNC_END):  # rest of the old function: never reached
        mem[a] = 0xFF

    s = rom_sum(mem)
    mem[SUM_AT], mem[SUM_AT + 1] = s >> 8, s & 0xFF
    write_srec(recs, mem, path_out)
    print('%s: %d mesaj %s, kod 0x%06X..0x%06X, ROM toplamı %04X -> %04X' %
          (path_out, len(msgs), [m.decode() for m in msgs], FUNC, FUNC + len(c) - 1, old_sum, s))
