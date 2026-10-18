#!/usr/bin/env python3
"""yama.py - apply assembler patches (a77.py sources) to a firmware HEX.

    python3 tools/yama.py ../YA4000S.HEX out.HEX patch1.asm [patch2.asm ...] [-l out.lst]

Safety rules, so that a patch cannot silently break the original code:
  * a byte may be written where the original image is erased (0xFF) or absent, or
    where a preceding `.EXPECT` in the source has confirmed the original bytes;
  * every `.EXPECT` is checked against the original image;
  * `.SERBEST a,n,...` releases an unused original region (first bytes checked, rest 0xFF);
  * every branch / jump must arrive with the M/X its target was assembled for (a77 check_flow);
  * the ROM checksum (16-bit sum of 0x2000..0xFFFF, big-endian word at 0x010003,
    checked at power on: "ERR FLSH") is recomputed;
  * existing S-records are rewritten in place, new bytes go to new S2 records.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import a77

SUM_AT = 0x010003
SUM_RANGE = range(0x2000, 0x10000)


def read_srec(path):
    recs, mem = [], {}
    raw = open(path, 'rb').read()
    eol = '\r\n' if b'\r\n' in raw[:4096] else '\n'
    for line in raw.decode('ascii').splitlines():
        line = line.strip()
        if not line:
            continue
        recs.append(line)
        if line[:2] in ('S1', 'S2', 'S3'):
            al = {'S1': 2, 'S2': 3, 'S3': 4}[line[:2]]
            n = int(line[2:4], 16)
            a = int(line[4:4 + al * 2], 16)
            for i, v in enumerate(bytes.fromhex(line[4 + al * 2:4 + n * 2 - 2])):
                mem[a + i] = v
    return recs, mem, eol


def srec_line(kind, addr, data):
    al = {'S1': 2, 'S2': 3, 'S3': 4}[kind]
    body = bytes([len(data) + al + 1]) + addr.to_bytes(al, 'big') + bytes(data)
    return kind + body.hex().upper() + '%02X' % (~sum(body) & 0xFF)


def write_srec(recs, mem, path, eol):
    out, covered = [], set()
    for line in recs:
        if line[:2] in ('S1', 'S2', 'S3'):
            al = {'S1': 2, 'S2': 3, 'S3': 4}[line[:2]]
            n = int(line[2:4], 16)
            a = int(line[4:4 + al * 2], 16)
            rng = range(a, a + n - al - 1)
            covered.update(rng)
            line = srec_line(line[:2], a, [mem[x] for x in rng])
        out.append(line)
    new = sorted(a for a in mem if a not in covered)
    extra, i = [], 0
    while i < len(new):                      # 16-byte S2 records, never across a 16-byte line
        a = new[i]
        run = [a]
        while i + len(run) < len(new) and new[i + len(run)] == a + len(run) and (a + len(run)) % 16:
            run.append(new[i + len(run)])
        extra.append(srec_line('S2', a, [mem[x] for x in run]))
        i += len(run)
    k = next((j for j, l in enumerate(out) if l[:2] in ('S7', 'S8', 'S9')), len(out))
    out[k:k] = extra
    open(path, 'w', newline='').write(eol.join(out) + eol)
    return len(extra)


def rom_sum(mem):
    return sum(mem.get(a, 0xFF) for a in SUM_RANGE) & 0xFFFF


def patch(base, sources, listing=None):
    mem = dict(base)
    report = []
    for src in sources:
        asm = a77.assemble_file(src, base=base)
        if asm.flow_warnings:
            raise SystemExit('%s: M/X uyuşmazlığı:\n  %s' % (src, '\n  '.join(asm.flow_warnings)))
        allowed = set()
        for at, data in asm.expects:
            have = bytes(base.get(at + i, 0xFF) for i in range(len(data)))
            if have != data:
                raise SystemExit('%s: .EXPECT 0x%06X tutmuyor: beklenen %s, görüntüde %s' %
                                 (src, at, data.hex(' ').upper(), have.hex(' ').upper()))
            allowed.update(range(at, at + len(data)))
        for at, n in asm.released:                  # .SERBEST: bölge boşaltılır
            allowed.update(range(at, at + n))
            for a in range(at, at + n):
                if a not in asm.mem and a in mem:
                    mem[a] = 0xFF
        for a, v in sorted(asm.mem.items()):
            if a in allowed or base.get(a, 0xFF) == 0xFF:
                mem[a] = v
            else:
                raise SystemExit('%s: 0x%06X orijinal kodun üzerine yazıyor (%02X) ama .EXPECT yok' %
                                 (src, a, base[a]))
        ks = sorted(asm.mem)
        report.append((src, len(ks), ks, len(asm.expects)))
        if listing:
            a77.write_listing(asm, listing if len(sources) == 1 else '%s.%d' % (listing, len(report)))
    return mem, report


def regions(ks):
    out = []
    for a in ks:
        if out and a == out[-1][1] + 1:
            out[-1][1] = a
        else:
            out.append([a, a])
    return out


if __name__ == '__main__':
    import argparse
    ap = argparse.ArgumentParser(description='a77 yamalarını HEX dosyasına uygula')
    ap.add_argument('base')
    ap.add_argument('out')
    ap.add_argument('sources', nargs='+')
    ap.add_argument('-l', '--list')
    o = ap.parse_args()
    recs, base, eol = read_srec(o.base)
    old = rom_sum(base)
    if old != (base[SUM_AT] << 8 | base[SUM_AT + 1]):
        print('uyarı: giriş dosyasının ROM toplamı zaten yanlış')
    try:
        mem, report = patch(base, o.sources, o.list)
    except a77.AsmError as e:
        sys.exit('hata: %s' % e)
    s = rom_sum(mem)
    mem[SUM_AT], mem[SUM_AT + 1] = s >> 8, s & 0xFF
    n_new = write_srec(recs, mem, o.out, eol)
    for src, n, ks, ne in report:
        print('%s: %d bayt, %d .EXPECT, bölgeler %s' %
              (src, n, ne, ' '.join('%06X-%06X' % (a, b) for a, b in regions(ks))))
    print('%s: ROM toplamı %04X -> %04X, %d yeni S2 kaydı' % (o.out, old, s, n_new))
