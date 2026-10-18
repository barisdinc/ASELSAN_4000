#!/usr/bin/env python3
"""test_a77.py - assemble a d77.py listing back and compare it with the HEX file.

    python3 tools/test_a77.py ../YA4000S.HEX YA4000S.asm
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import a77
import srec

hexpath, asmpath = sys.argv[1], sys.argv[2]
ref = srec.load(hexpath)
text = open(asmpath).read()
# names the disassembler prints for addresses outside the listing (RAM code etc.)
defs = {'RAM_WAIT_TRAMPOLINE': 0x87A}
a = a77.Asm(defs)
for it in range(3):
    try:
        a.assemble(text, os.path.basename(asmpath))
        break
    except a77.AsmError as e:
        m = re.search(r'tanımsız sembol: (\S+)', str(e))
        if not m or not re.fullmatch(r'[LF]_[0-9A-F]{6}', m.group(1).split(',')[0]):
            sys.exit('hata: %s' % e)
        for n in m.group(0).split(': ')[1].split(', '):
            defs[n] = int(n[2:], 16)
        a = a77.Asm(defs)
# the flash loader is listed at its RAM address: ROM 0x010100.. is copied to RAM 0x80..
ram = {x: v for x, v in a.mem.items() if x < 0x2000}
for x, v in ram.items():
    del a.mem[x]
ram_bad = [x for x, v in ram.items() if ref.get(x - 0x80 + 0x010100) != v]
bad = [x for x in a.mem if ref.get(x) != a.mem[x]] + ram_bad
missing = [x for x in ref if x not in a.mem]
extra = [x for x in a.mem if x not in ref]
print('%s: %d bayt birleştirildi, %d farklı, %d eksik, %d fazla (ek semboller: %s)' %
      (asmpath, len(a.mem), len(bad), len(missing), len(extra), ', '.join(sorted(defs))))
for x in bad[:20]:
    print('  0x%06X: a77 %02X, HEX %s' % (x, a.mem.get(x, ram.get(x)), ref.get(x)))
sys.exit(1 if bad or missing or extra else 0)
