# uret.py - kaynak.html -> kullanim_kilavuzu.html (tuş kısaltmaları [m] [s] [p] [q] [P] [0] [PTT], telsiz çizimi)
#   python3 uret.py kaynak.html kullanim_kilavuzu.html

import re, urllib.parse, sys
s = open(sys.argv[1] if len(sys.argv) > 1 else 'kaynak.html').read()
back = 'M2.6 10.2 V2.6 H10.2 V4.6 M2.6 10.2 H4.6'
SYM = {
 'm': '<path d="%s" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="square"/><rect x="5.8" y="5.8" width="7.6" height="7.6" rx=".6" fill="currentColor"/>' % back,
 's': '<rect x="3.6" y="3.6" width="8.8" height="8.8" rx=".7" fill="currentColor"/>',
 'p': '<circle cx="8" cy="8" r="4.6" fill="currentColor"/>',
 'q': '<path d="M8 3.2 L13 12.4 H3 Z" fill="currentColor" stroke="currentColor" stroke-width=".8" stroke-linejoin="round"/>',
}
syms = ''.join('<symbol id="k-%s" viewBox="0 0 16 16">%s</symbol>' % kv for kv in SYM.items())
s = s.replace('<svg width="0" height="0" style="position:absolute" aria-hidden="true">', '<svg width="0" height="0" style="position:absolute" aria-hidden="true">\n  ' + syms, 1)
msvg = '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 16 16"><path d="%s" fill="none" stroke="#000" stroke-width="1.5" stroke-linecap="square"/><rect x="5.8" y="5.8" width="7.6" height="7.6" rx=".6" fill="#000"/></svg>' % back
s = s.replace('* { box-sizing: border-box; }', ':root { --m-mask: url("data:image/svg+xml,%s"); }\n* { box-sizing: border-box; }' % urllib.parse.quote(msvg), 1)
icon = lambda k: '<svg class="kg" aria-hidden="true"><use href="#k-%s"/></svg>' % k
NAMES = {'m': 'Menü', 's': 'Seç', 'p': 'Güç', 'q': 'Monitör'}
pad = ''.join(('<b class="f" aria-label="%s">%s</b>' % (NAMES[c], icon(c))) if c in 'mspq' else '<b>%s</b>' % c for c in '123m456s789p*0#q')
def radyo(num):
    cn = (lambda n, st: '<span class="cn" style="%s">%d</span>' % (st, n)) if num else (lambda n, st: '')
    return ('<div class="brand"><span>ASELSAN</span><span>4025</span></div><div class="face">'
      '<div class="left" style="position:relative"><div class="knob"></div>%s<div class="leds"><span class="led r"><i></i>TX</span><span class="led y"><i></i>RX</span></div>%s</div>'
      '<div class="glass" style="position:relative"><span class="lcd" data-t="KANAL 01" data-c="01"></span>%s%s</div>'
      '<div class="pad" style="position:relative">%s%s</div></div><span class="mic"></span>%s') % (
      cn(3, 'left:-4px;top:-6px'), cn(4, 'left:-4px;bottom:-8px'), cn(1, 'left:-8px;top:-8px'), cn(2, 'right:-8px;bottom:-8px'),
      pad, cn(5, 'right:-8px;top:-8px'), cn(6, 'left:-16px;top:30%'))
s = s.replace('[[RADYO_ON_NUMARALI]]', radyo(True)).replace('[[RADYO_ON]]', radyo(False))
pins = ''.join('<circle cx="%d" cy="78" r="4"/>' % (172 + 20 * i) for i in range(8)) + ''.join('<circle cx="%d" cy="94" r="4"/>' % (182 + 20 * i) for i in range(7))
s = s.replace('[[OPSIYON_PINLER]]', pins)
s = re.sub(r'\[P\]', '<kbd class="f" title="p (1 s)" aria-label="p tuşunu 1 saniye basılı tut">%s<small>1 s</small></kbd>' % icon('p'), s)
s = re.sub(r'\[([mspq])\]', lambda m: '<kbd class="f" title="%s" aria-label="%s tuşu">%s</kbd>' % (m.group(1), NAMES[m.group(1)], icon(m.group(1))), s)
s = re.sub(r'\[PTT\]', '<kbd class="ptt">PTT</kbd>', s)
s = re.sub(r'\[([0-9*#])\]', r'<kbd>\1</kbd>', s)
left = re.findall(r'\[[^\]\s]{1,6}\]', s.split('<script>')[0])
left = [x for x in left if x not in ('[data-theme="light"]', '[data-theme="dark"]')]
if left: print('açıkta kalan:', set(left))
open(sys.argv[2] if len(sys.argv) > 2 else 'kullanim_kilavuzu.html', 'w').write('<!doctype html>\n<html lang="tr">\n<meta charset="utf-8">\n<meta name="viewport" content="width=device-width, initial-scale=1">\n' + s)
print(len(s))
