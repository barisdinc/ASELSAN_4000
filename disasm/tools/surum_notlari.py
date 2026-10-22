#!/usr/bin/env python3
"""ozel_firmware/degisiklikler.json -> ozel_firmware/SURUMLER.md (sürüm notları ve özellikler).
usage: python3 surum_notlari.py degisiklikler.json surum.json > SURUMLER.md"""
import sys, json
notlar = json.load(open(sys.argv[1], encoding='utf-8'))
cfg = json.load(open(sys.argv[2], encoding='utf-8'))
on = cfg.get('on_ek', 'AT')
TUR = [('yeni', 'Yeni'), ('duzeltme', 'Düzeltme'), ('degisiklik', 'Değişiklik')]
DURUM = {'gercek': 'gerçek telsizde denendi', 'sim': 'yalnız simülatörde denendi'}
tarih = lambda t: '.'.join(reversed(t.split('-')))
o = ['# Sürüm notları', '',
     '*Bu dosya `ozel_firmware/degisiklikler.json`\'dan üretilir (`make -C disasm sim`); elle değiştirmeyin.*',
     'Aynı notlar sitede: [surumler.html](https://barisdinc.github.io/ASELSAN_4000/surumler.html); programlama sayfası firmware seçilince de gösterir.', '']
for ad, n in notlar.items():
    if ad.startswith('_') or 'surumler' not in n: continue
    k = cfg.get('kayitlar', {}).get(ad)
    o += ['## %s (`%s`)' % (n.get('ad', ad), ad), '']
    if k: o += ['Güncel sürüm: **%s V%d-%d** (%s)' % (on, k['major'], k['minor'], tarih(k['tarih'])), '']
    o += [n['ozet'], '', '### Özellikler', '']
    o += ['- **%s** (%s V%s ile geldi): %s' % (x['baslik'], on, x['surum'], x['aciklama']) for x in n.get('ozellikler', [])]
    o += ['', '### Sürüm geçmişi', '']
    for s in n['surumler']:
        d = 'geri alındı' if s.get('geri_alindi') else DURUM.get(s.get('durum'), '')
        o += ['#### %s V%s · %s%s' % (on, s['surum'], tarih(s['tarih']), ' · ' + d if d else ''), '']
        for key, baslik in TUR:
            o += [('- ~~%s: %s~~' if s.get('geri_alindi') else '- %s: %s') % (baslik, x) for x in s.get(key, [])]
        o.append('')
ori = [(ad, n) for ad, n in notlar.items() if not ad.startswith('_') and 'surumler' not in n]
if ori:
    o += ['## ASELSAN orijinal firmware\'ler', ''] + ['- **%s**: %s' % (ad, n['ozet']) for ad, n in ori]
print('\n'.join(o))
