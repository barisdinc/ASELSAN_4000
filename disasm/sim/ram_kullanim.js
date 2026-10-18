/* ram_kullanim.js - firmware'in hangi RAM adreslerine yazdığını ölçer (boş RAM aramak için).
 *   jsc ram_kullanim.js -- ../../YA4000S.HEX
 * Açılıştan itibaren her RAM yazmasını sayar; tuş takımı, menüler, tarama, PTT, TKS,
 * seçmeli çağrı (gönderme ve alma) ve seri port senaryolarını çalıştırır. */
'use strict';
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let B, V, hexText, out;
if (isNode) {
  B = require('./board.js'); V = require('./radio_view.js');
  hexText = require('fs').readFileSync(process.argv[2], 'utf8'); out = s => console.log(s);
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js');
  B = globalThis.Board; V = globalThis.RadioView;
  const args = typeof scriptArgs !== 'undefined' ? scriptArgs : arguments;
  hexText = readFile(args[0]); out = s => print(s);
}
const { Board, parseHex, FCLK } = B;
const rom = parseHex(hexText);
const KP = '*0#q789p456s123m';
const written = new Uint32Array(0x880);
function radio(eeMod) {
  const b = new Board(rom), ee = B.defaultEeprom();
  if (eeMod) eeMod(ee);
  B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom);
  b.ramHook = a => { written[a]++; };
  return b;
}
const keys = (b, seq) => { for (const k of seq) { const i = KP.indexOf(k); b.keypad.press(i, true, b); b.run(FCLK * 0.15); b.keypad.press(i, false, b); b.run(FCLK * 0.5); } };
let minS = 0x880;
const runS = (b, sec) => { for (let t = 0; t < sec / 0.005; t++) { b.run(FCLK * 0.005); if (b.cpu.S < minS) minS = b.cpu.S; } };

const all = ee => { ee[0x014] = 0x0C; ee[0x018] |= 0x20; ee[0x01A] = 0x20; ee[0x01B] = 5; ee[0x01C] = 0x30;
  for (let n = 1; n <= 16; n++) ee[0x2E + (n - 1) * 13 + 4] |= 0x80; };
{ // menus, list, scan, sub menu items
  const b = radio(all); runS(b, 2.6);
  keys(b, 'msmmsmms05mmsms'); keys(b, 'p'); runS(b, 2); b.inputs.P5 |= 0x80; runS(b, 1); b.inputs.P5 &= 0x7F; keys(b, 'p');
  keys(b, 'mmsss'); keys(b, 'msmsmssmmmsmm'); keys(b, 'mmsmmmmsm'); keys(b, 'mmsmmmmmsm1234'); runS(b, 5);
  keys(b, '#123'); runS(b, 4);
  for (const c of [4, 5, 2, 3]) { b.panel.setExt(((16 - c) & 15) << 4 | 0x0F, b); runS(b, 0.3); }
  b.inputs.P4 = 0x7F; runS(b, 1.5); keys(b, '159*#'); b.inputs.P4 = 0xFF; runS(b, 1);
}
{ // CTCSS TX, Select-5 RX
  const b = radio(ee => { all(ee); B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'TKS', txTone: 9, selcall: 1 }); });
  runS(b, 2.6); b.inputs.P4 = 0x7F; runS(b, 1.5); b.inputs.P4 = 0xFF; runS(b, 0.5);
  if (b.gen && b.gen.selcall) { b.gen.selcall('CCIR', '12345'); runS(b, 2); }
}
out('S en düşük: 0x' + minS.toString(16));
const ranges = (lo, hi) => { const r = []; for (let a = lo; a < hi; a++) if (written[a]) { if (r.length && r[r.length - 1][1] === a - 1) r[r.length - 1][1] = a; else r.push([a, a]); } return r.map(([x, y]) => x.toString(16) + (x === y ? '' : '-' + y.toString(16))).join(' '); };
out('yazılan 0x080-0x4EC (bss): ' + ranges(0x80, 0x4ED));
out('yazılan 0x4ED-0x629: ' + (ranges(0x4ED, 0x62A) || 'hiç'));
out('yazılan 0x62A-0x879 (çerçeve + yığın): ' + ranges(0x62A, 0x880));
