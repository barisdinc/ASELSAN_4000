#!/usr/bin/env python3
"""aprs_paket.py - sabit bir APRS konum paketini (AX.25 UI çerçevesi) derleme sırasında
1200 baud AFSK ton dizisine çevirir ve assembler için .inc dosyası yazar.

    python3 -I tools/aprs_paket.py ../ozel_firmware/ornekler/aprs_paket.inc

Ton dizisi: bit başına bir ton, 1 = mark (1200 Hz), 0 = space (2200 Hz); baytlar LSB önce.
Firmware (aprs.inc) bu bitleri sırayla okuyup her biri için 8 örnek (9600 Hz) üretir.

Kodlama (AX.25 2.2 / APRS 1.0.1):
  - adres alanı: çağrı işaretinin her karakteri 1 bit sola kaydırılmış, 6 karaktere boşlukla
    tamamlanmış; SSID baytı 0b011SSSS0 (hedefte C biti = 1: 0b111SSSS0), son adreste bit 0 = 1
  - kontrol 0x03 (UI), PID 0xF0 (katman 3 yok), bilgi alanı APRS metni
  - FCS: CRC-16-CCITT (yansıtılmış 0x8408, başlangıç 0xFFFF, sonda tersi), düşük bayt önce
  - baytlar LSB önce; çerçeve içinde art arda beş 1'den sonra bir 0 eklenir (bit doldurma)
  - çerçeve 0x7E bayraklarıyla sarılır (doldurma yok); başta ~300 ms bayrak (alıcı kilitlensin)
  - NRZI: 0 = ton değişir, 1 = ton aynı kalır
"""
import sys

CAGRI = ('TA7W', 7)                      # gönderen
HEDEF = ('APIZCI', 0)                    # cihaz kimliği (tocall)
YOL = [('WIDE1', 1), ('WIDE2', 1)]
# Düzce: 40,8438 K 31,1565 D -> 40° 50,63' K, 031° 09,39' D; simge tablosu '/', simge 'b' (bisiklet)
BILGI = '!4050.63N/03109.39Eb ASELSAN 4000'
ON_BAYRAK = 45                           # 45 x 8 bit / 1200 baud = 300 ms
SON_BAYRAK = 3


def adres(cagri, ssid, son, komut=False):
    c = cagri.ljust(6)[:6]
    b = [ord(x) << 1 for x in c]
    b.append((0x80 if komut else 0) | 0x60 | (ssid & 15) << 1 | (1 if son else 0))
    return b


def fcs(veri):
    crc = 0xFFFF
    for b in veri:
        crc ^= b
        for _ in range(8):
            crc = (crc >> 1) ^ 0x8408 if crc & 1 else crc >> 1
    crc ^= 0xFFFF
    return [crc & 0xFF, crc >> 8]


def cerceve():
    adr = adres(*HEDEF, son=False, komut=True) + adres(*CAGRI, son=not YOL)
    for i, (c, s) in enumerate(YOL):
        adr += adres(c, s, son=i == len(YOL) - 1)
    govde = adr + [0x03, 0xF0] + [ord(x) for x in BILGI]
    return govde + fcs(govde)


def bitler(veri, doldur):
    out, birler = [], 0
    for b in veri:
        for i in range(8):
            bit = b >> i & 1
            out.append(bit)
            if not doldur:
                continue
            birler = birler + 1 if bit else 0
            if birler == 5:
                out.append(0)
                birler = 0
    return out


def tonlar():
    ham = bitler([0x7E] * ON_BAYRAK, False) + bitler(cerceve(), True) + bitler([0x7E] * SON_BAYRAK, False)
    ton, out = 1, []                     # mark ile başla
    for bit in ham:
        if bit == 0:
            ton ^= 1
        out.append(ton)
    return out


def main(yol):
    t = tonlar()
    paket = bytes(t[i:i + 8] and sum(b << k for k, b in enumerate(t[i:i + 8])) for i in range(0, len(t), 8))
    c = cerceve()
    metin = '%s-%d>%s,%s:%s' % (CAGRI[0], CAGRI[1], HEDEF[0], ','.join('%s-%d' % y for y in YOL), BILGI)
    with open(yol, 'w') as f:
        f.write('; aprs_paket.inc - disasm/tools/aprs_paket.py ile üretildi, elle değiştirmeyin\n')
        f.write(';   %s\n' % metin)
        f.write(';   çerçeve %d bayt (FCS dahil), %d ton biti = %.3f s\n' % (len(c), len(t), len(t) / 1200))
        f.write(';   çerçeve: %s\n' % ' '.join('%02X' % b for b in c))
        f.write('APRS_NBIT   .EQU %d\n' % len(t))
        f.write('APRS_TONLAR:\n')
        for i in range(0, len(paket), 16):
            f.write('        .BYTE ' + ','.join('$%02X' % b for b in paket[i:i + 16]) + '\n')


if __name__ == '__main__':
    main(sys.argv[1])
