/* ram_bos.js - açılıştan SONRA hiç okunmayan ve yazılmayan RAM baytlarını bulur (yeni özelliklere yer).
 *   jsc ram_bos.js -- ../../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX
 * Açılışta NC77 başlangıç kodu 0x0080..0x04EC'yi sıfırlar; bu yüzden açılış bitene kadar olan erişimler
 * sayılmaz. Sonra A ve B tipi kartta, Türkçe / İngilizce: menüler, liste, tarama (sinyalli / TKS'li),
 * kanal değiştirme, PTT, TKS, VFO (frekans + ton), APRS (gönderme, alma, liste, otomatik gönderim),
 * düşük pil, seri protokol (bağlan, EEPROM oku, ölçümler) çalıştırılır. Çıktı: 0x0080..0x087F'de
 * erişilmeyen aralıklar. Ölçülmeyen nadir bir yol olabilir: kullanmadan önce statik analizle
 * (yalnız kapatılan özelliklerin kodunun dokunduğu adresler) karşılaştırın. */
'use strict';
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let B, V, hexText, out;
if (isNode) {
  B = require('./board.js'); V = require('./radio_view.js'); globalThis.AFSK = require('./afsk.js'); globalThis.P4000 = require('./p4000.js');
  hexText = require('fs').readFileSync(process.argv[2], 'utf8'); out = s => console.log(s);
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js'); load('afsk.js'); load('p4000.js');
  B = globalThis.Board; V = globalThis.RadioView;
  const args = typeof scriptArgs !== 'undefined' ? scriptArgs : arguments;
  hexText = readFile(args[0]); out = s => print(s);
}
const { Board, parseHex, FCLK } = B;
const rom = parseHex(hexText);
const KP = '*0#q789p456s123m';
const erisim = new Uint32Array(0x880);
const BOOT = (rom[0xE0E1] === 0x5C ? 5.5 : 0) + 3.0;
function telsiz(eeMod, arac) {
  const b = new Board(rom), ee = B.defaultEeprom();
  if (eeMod) eeMod(ee);
  if (arac) ee[0x58B] |= 2;
  B.eeFixSums(ee); b.eeprom.mem.set(ee);
  if (arac) { b.hwArac = true; b.inputs.P4 = 0xCF; b.panel.setExt(0xBF, b); }
  b.font = V.buildFont(b.rom);
  b.run(FCLK * BOOT);
  const rd = b.rd.bind(b), wr = b.wr.bind(b);
  b.rd = a => { if (a >= 0x80 && a < 0x880) erisim[a]++; return rd(a); };
  b.wr = (a, v) => { if (a >= 0x80 && a < 0x880) erisim[a]++; return wr(a, v); };
  return b;
}
const tus = (b, seq) => { for (const k of seq) { const i = KP.indexOf(k); b.keypad.press(i, true, b); b.run(FCLK * 0.15); b.keypad.press(i, false, b); b.run(FCLK * 0.5); } };
const ptt = (b, sn) => { const p = b.inputs.P4; b.inputs.P4 = p & 0x7F; b.run(FCLK * (sn || 1.2)); b.inputs.P4 = p; b.run(FCLK * 0.6); };
const normal = b => { for (let k = 0; k < 12 && !/^KANAL|^CH|^[0-9 ]{3} /.test(V.decodeLcd(b, b.font).text); k++) tus(b, 'm'); };
const menuGit = (b, ad) => { normal(b); for (let k = 0; k < 14 && V.decodeLcd(b, b.font).text.trim() !== ad; k++) tus(b, 'm'); };
const sinyal = (b, sn, ton) => { if (ton) b.ctcssIn.setTone(ton); b.inputs.P5 |= 0x80; b.run(FCLK * sn); b.inputs.P5 &= ~0x80; b.ctcssIn.setTone(0); b.run(FCLK * 0.4); };
const aprsPaket = (b, text) => { const e = AFSK.edges(AFSK.encode(text), {}), t = b.now;
  b.selIn.setEdges(e.map(x => t + Math.round(x * FCLK))); b.inputs.P5 |= 0x80; b.run(FCLK * (e[e.length - 1] + 0.05)); b.inputs.P5 &= ~0x80; b.run(FCLK * 0.6); };
const cerceve = govde => { const o = [0x16, 0x10, 0x02]; for (const x of govde) { o.push(x); if (x === 0x10) o.push(0x10); } return o.concat([0x10, 0x03, 0, 0]); };
const aprsAyar = ee => P4000.aprsSet(ee, { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'test', tablo: '/', kod: '>', aralik: 10 });
const kanallar = ee => { B.eeSetChannel(ee, 2, 156.1e6, 156.1e6, { name: 'TKS', rxTone: 8, txTone: 9 }); };

for (const [arac, dil] of [[false, 0], [true, 0x80], [true, 0]]) {
  const b = telsiz(ee => { ee[0x018] |= dil | 0x20; ee[0x014] |= 0x08; kanallar(ee); aprsAyar(ee); }, arac);
  sinyal(b, 1.0); ptt(b);                                                     // normal ekran
  tus(b, 'msmmsmms05mmsms'); tus(b, 'p'); b.run(FCLK * 1.5); sinyal(b, 1.2); sinyal(b, 1.2, 88.5); tus(b, 'p');   // liste + tarama
  if (!arac) for (const c of [2, 3, 1]) { b.panel.setExt(((16 - c) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.6); sinyal(b, 0.6, 88.5); ptt(b, 0.8); }
  else { tus(b, '02'); b.run(FCLK * 0.5); sinyal(b, 0.6, 88.5); ptt(b, 0.8); tus(b, '01'); }
  menuGit(b, 'SUB MENU'); tus(b, 's'); tus(b, 'smssmmsmmm'); normal(b);   // alt menü
  menuGit(b, b.ram[0x340] & 0x80 ? 'FREKANS' : 'FREQ'); tus(b, 's156525#160650#885#1000#'); tus(b, 'm'); sinyal(b, 0.8, 88.5); ptt(b);  // VFO
  menuGit(b, 'APRS'); tus(b, 's'); b.run(FCLK * 0.5); ptt(b, 1.6);       // APRS
  aprsPaket(b, 'TA1XX>APRS,WIDE1-1*,WIDE2-1:!4050.63N/03109.39E>hello'); aprsPaket(b, 'YM7KA-12>APDR15:>status');
  tus(b, '**#'); b.run(FCLK * 12);                                         // liste, otomatik gönderim
  tus(b, 'm'); normal(b);
  b.serialIn(cerceve([0x01])); b.run(FCLK * 0.4);                         // seri protokol
  b.serialIn(cerceve([0x04, 0x01, 0x01, 0x00, 0x0C])); b.run(FCLK * 0.4);
  b.serialIn(cerceve([0x04, 0x02, 0x01, 0x00, 0x03, 0x01, 0x01, 0x00, 0x05, 0x8C, 0x20])); b.run(FCLK * 0.6);
  for (let k = 1; k <= 7; k++) { b.serialIn(cerceve([0x04, 2 + k, 0x01, 0x00, 0x07, k])); b.run(FCLK * 0.3); }
  b.analogIn && (b.analogIn[4] = 0x90); b.run(FCLK * 2);                    // düşük pil (varsa)
}
const bos = []; for (let a = 0x80; a < 0x880; a++) if (!erisim[a]) bos.push(a);
const r = []; for (const a of bos) { if (r.length && r[r.length - 1][1] === a - 1) r[r.length - 1][1] = a; else r.push([a, a]); }
const say = (lo, hi) => bos.filter(a => a >= lo && a <= hi).length;
out('açılıştan sonra erişilmeyen bayt: 0x080-0x4EC ' + say(0x80, 0x4EC) + ', 0x4ED-0x629 ' + say(0x4ED, 0x629) + ', 0x62A-0x87F ' + say(0x62A, 0x87F));
out(r.filter(([x, y]) => x < 0x4ED).map(([x, y]) => x.toString(16) + (x === y ? '' : '-' + y.toString(16)) + (y - x >= 3 ? '(' + (y - x + 1) + ')' : '')).join(' '));
