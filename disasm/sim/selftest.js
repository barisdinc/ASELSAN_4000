/* selftest.js - regression test of the simulator with the original firmware.
 * Runs under node (node selftest.js ../../YA4000S.HEX) or macOS JavaScriptCore
 * (jsc selftest.js -- ../../YA4000S.HEX).  Exit status / last line tells the result. */
'use strict';
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let B, V, hexText, out;
if (isNode) {
  B = require('./board.js'); V = require('./radio_view.js'); globalThis.UPD = require('./updater.js');
  hexText = require('fs').readFileSync(process.argv[2] || '../../YA4000S.HEX', 'utf8'); out = s => console.log(s);
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js'); load('updater.js');
  B = globalThis.Board; V = globalThis.RadioView; globalThis.UPD = globalThis.Updater;
  const args = typeof scriptArgs !== 'undefined' ? scriptArgs : arguments;
  hexText = readFile(args[0] || '../../YA4000S.HEX'); out = s => print(s);
}
const { Board, parseHex, FCLK } = B;
const rom = parseHex(hexText);
let fails = 0;
function check(name, ok, info) { out((ok ? 'ok   ' : 'FAIL ') + name + (info ? '  (' + info + ')' : '')); if (!ok) fails++; }
function radio(eeMod) {
  const b = new Board(rom), ee = B.defaultEeprom();
  if (eeMod) eeMod(ee);
  B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom);
  return b;
}
const lcd = b => V.decodeLcd(b, b.font).text;
const mhz = v => (v / 1e6).toFixed(4);

/* 1. power on, RX on channel 1 */
let b = radio();
b.run(FCLK * 0.5); check('açılış mesajı', lcd(b) === 'AS V2-3 ', lcd(b));
b.run(FCLK * 2.0); check('kanal adı ekranda', lcd(b) === 'KANAL 01', lcd(b));
let st = b.radioState();
check('RX VCO = 156,000 + 45 MHz', mhz(st.vco) === '201.0000', mhz(st.vco));
check('RX kilitli, P71 açık', st.locked && st.powerHold === 1);
check('sentezör bir kez yüklendi', b.synth.loads === 1, 'loads=' + b.synth.loads);

/* 2. channel switch */
b.panel.setExt(((16 - 3) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.5);
st = b.radioState();
check('kanal 3', lcd(b) === 'KANAL 03' && mhz(st.vco) === '201.0500', lcd(b) + ' ' + mhz(st.vco));
/* successive changes: each needs a new INT1 falling edge (line released after the read) */
for (const c of [4, 5, 2, 3]) { b.panel.setExt(((16 - c) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.3); }
check('art arda kanal değişimi 4→5→2→3', lcd(b) === 'KANAL 03', lcd(b));
/* a name with a char outside A-Z 0-9 space _ - / is rejected: "CH nn" template instead */
{ const c = radio(ee => { ee[0x2E + 12] = 0x00; }); c.run(FCLK * 2.6);
  check('geçersiz ad → "CH 01"', lcd(c) === '   CH 01', lcd(c)); }

/* 3. PTT -> TX, power loop */
b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
st = b.radioState();
check('PTT: TX VCO = 156,050', mhz(st.vco) === '156.0500' && st.rxSelect === 0, mhz(st.vco));
const loads = b.synth.loads;
b.run(FCLK * 0.5);
check('TX kararlı (yeniden yükleme yok)', b.synth.loads === loads && b.ram[0x228] === 8, 'state=' + b.ram[0x228]);
check('güç döngüsü hedefe oturdu', b.analogRead(7) >= 0xB0 - 8 && b.analogRead(7) <= 0xB0 + 8, 'AN7=' + b.analogRead(7));
b.inputs.P4 = 0xFF; b.run(FCLK * 0.3);
check('PTT bırakıldı: RX', b.radioState().rxSelect === 1 && mhz(b.radioState().vco) === '201.0500');

/* 4. CTCSS TX tone 9 = 91.5 Hz (firmware numbering) */
b = radio(ee => B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'TKS', txTone: 9 }));
b.run(FCLK * 2.6); b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
let a = V.analyseTx(b, FCLK, FCLK * 0.05);
check('TX TKS 91,5 Hz', a.ctcssHz && Math.abs(a.ctcssHz - 91.5) < 1, a.ctcssHz && a.ctcssHz.toFixed(2));

/* 5. Select-5 on PTT: EEPROM 0x01A = 0x20, channel selcall flag, CCIR */
b = radio(ee => { ee[0x01A] = 0x20; B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'SEL', selcall: 1 }); });
b.run(FCLK * 2.6); b.inputs.P4 = 0x7F;
const seen = [];
for (let i = 0; i < 40; i++) {
  b.run(FCLK * 0.02);
  const r = V.analyseTx(b, FCLK, FCLK * 0.02);
  const d = r.selcall && r.selcall.find(x => x.startsWith('CCIR:'));
  if (d && seen[seen.length - 1] !== d.slice(5)) seen.push(d.slice(5));
}
check('Select-5 CCIR gönderildi', seen.join('').startsWith('0E0E0'), seen.join(''));

/* 6. DTMF from the keypad during TX (keypad option EEPROM 0x014 bit 3) */
b = radio(ee => { ee[0x014] = 0x08; });
b.run(FCLK * 2.6); b.inputs.P4 = 0x7F; b.run(FCLK * 0.4);
b.keypad.press('*0#q789p456s123m'.indexOf('5'), true, b);
let dt = null;
for (let i = 0; i < 20 && !dt; i++) { b.run(FCLK * 0.01); dt = V.analyseTx(b, FCLK, FCLK * 0.02).dtmf; }
check('DTMF 5', dt === '5', dt);

/* 7. Select-5 receive: CCIR "12345" on P66 is decoded into RAM 0x0264.. */
b = radio(ee => { ee[0x01A] = 0x20; B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'SEL', selcall: 1 }); });
b.run(FCLK * 2.6); b.inputs.P5 |= 0x80; b.run(FCLK * 0.3);   // P57 = 1: squelch open
const ccir = V.SELCALL.CCIR, seq = [{ f: 0, ms: 20 }];
for (const ch of '12345') seq.push({ f: ccir.f[+ch], ms: ccir.ms });
b.selIn.setSequence(b.now, seq);
let best = '';
for (let i = 0; i < 14; i++) {
  b.run(FCLK * 0.05);
  let d = ''; for (let k = 0; k < 8; k++) { const v = b.ram[0x264 + k]; if (v < 15) d += V.SEL_DIGITS[v]; }
  if (d.length > best.length) best = d;
}
check('Select-5 alındı ve çözüldü', best === '12345', best);

/* 7b. keypad scan list + scan: m s, m m (= "+") s for ch 1 and 2, 0 5 m m s for ch 5,
 * m s = EXIT; p starts the scan, it walks 1/2/5 while the squelch is closed (P57 = 0)
 * and stops on the channel with a signal (P57 = 1) */
{
  const c = radio(), KP = '*0#q789p456s123m';
  c.run(FCLK * 2.5);
  const key = k => { const i = KP.indexOf(k); c.keypad.press(i, true, c); c.run(FCLK * 0.15); c.keypad.press(i, false, c); c.run(FCLK * 0.6); };
  for (const k of 'msmmsmms05mmsms') key(k);
  key('p');
  const vcos = new Set(); for (let i = 0; i < 12; i++) { c.run(FCLK * 0.2); vcos.add(mhz(c.radioState().vco)); }
  check('tarama: SCAN, liste kanalları geziliyor', lcd(c) === '  SCAN  ' && vcos.has('201.0250') && vcos.has('201.1000') && !vcos.has('201.0500'), lcd(c) + ' ' + [...vcos].join(','));
  c.inputs.P5 |= 0x80; c.run(FCLK * 1.0);
  check('tarama sinyalde duruyor', /^KANAL 0[125]$/.test(lcd(c)) && c.ram[0x228] === 3, lcd(c));
}

/* 7c. selective call from the keypad: EE 0x01B = 5 digits, 0x01C = 0x30 (3 to dial), 0x014 bit 2;
 * # 1 2 3 keys up and sends CCIR "00123" */
{
  const c = radio(ee => { ee[0x014] = 0x0C; ee[0x01B] = 5; ee[0x01C] = 0x30; for (let n = 1; n <= 16; n++) ee[0x2E + (n - 1) * 13 + 4] |= 0x80; }), KP = '*0#q789p456s123m';
  c.run(FCLK * 2.5);
  const sel = []; let scr = [];
  const tick = sec => { for (let i = 0; i < sec / 0.03; i++) { c.run(FCLK * 0.03); const r = V.analyseTx(c, FCLK, FCLK * 0.03); const d = r.selcall && r.selcall.find(x => x.startsWith('CCIR:')); if (d && sel[sel.length - 1] !== d.slice(5)) sel.push(d.slice(5)); } };
  for (const k of '#123') { const i = KP.indexOf(k); c.keypad.press(i, true, c); tick(0.12); c.keypad.press(i, false, c); tick(0.7); scr.push(lcd(c)); }
  tick(1.5);
  check('tuş takımından seçmeli çağrı', scr.join('|') === 'C GG___ |C GG1__ |C GG12_ |C GG123 ', scr.join('|'));
  check('çağrı gönderildi (CCIR 00123)', sel.join('').startsWith('0E123'), sel.join(''));
}

/* 8. empty EEPROM -> "ARZ PRG" */
b = new Board(rom); b.font = V.buildFont(b.rom);
b.run(FCLK * 1.0);
check('boş EEPROM: ARZ PRG', lcd(b) === 'ARZ PRG ', lcd(b));

/* 8b. board type: EE 0x58B bit 1 must match the probed hardware (F_00DF55). On the
 * araç / sabit merkez board P60 is wired to SDA; P44 / P45 are active high there */
{ const c = radio(ee => { ee[0x58B] |= 2; }); c.run(FCLK * 2.6);
  check('el kartı + EE58B bit1 = 1: ERR HW', lcd(c) === ' ERR HW ', lcd(c));
  const d = radio(ee => { ee[0x58B] |= 2; }); d.hwArac = true; d.inputs.P4 = 0xCF; d.run(FCLK * 2.6);
  check('araç kartı algılandı (RAM 0x335 bit 1)', (d.ram[0x335] & 2) === 2, d.ram[0x335].toString(16));
  check('araç kartı açılış: KANAL 01', lcd(d) === 'KANAL 01', lcd(d));
  /* 0x46 on this board: b6 = PTT, active high (F_0058FB -> RAM 0x244 bit 4) */
  d.panel.setExt(0xBF, d); d.run(FCLK * 0.5); const rx = d.outBit(0x0B, 5);
  d.panel.setExt(0xFF, d); d.run(FCLK * 0.5); const tx = d.outBit(0x0B, 5);
  check('araç kartı: 0x46 b6 = 1 → TX', rx === 1 && tx === 0, 'P55 ' + rx + '→' + tx); }

/* 9. serial firmware update through the loader: new image = original with the
 * channel screen text "CH" (ROM 0x2DE9) changed to "KN"; after the update and
 * the reset the radio must run the new code */
b = radio(ee => { ee[0x2E + 12] = 0x00; }); b.run(FCLK * 2.6);   // no name, so the CH template is on screen
const neu = new Map(rom); neu.set(0x2DE9, 0x4B); neu.set(0x2DEA, 0x4E);
const up = new UPD.FlashUpdater(b, neu, { verify: false });
while (!up.done && b.now < FCLK * 400) { b.run(FCLK * 0.01); up.tick(); }
check('seri güncelleme tamamlandı', !up.error, up.error || up.log.slice(-1)[0]);
b.font = V.buildFont(b.rom); b.run(FCLK * 3);
check('yeni firmware çalışıyor ("KN 01")', lcd(b) === '   KN 01', lcd(b));

out(fails ? fails + ' test başarısız' : 'tüm simülatör testleri geçti');
if (isNode && fails) process.exit(1);
if (!isNode && fails) throw new Error(fails + ' test başarısız');   // jsc: yakalanmamış istisna -> çıkış kodu 3 (quit(n) kodu iletmiyor)
