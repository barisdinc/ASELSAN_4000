/* test_p4000.js - p4000.js (P4000 EEPROM yerleşimi ve .p40) testi.
 *   node test_p4000.js [eeprom.bin ...] [kütük.p40 ...]
 *   jsc test_p4000.js -- [eeprom.bin ...] [kütük.p40 ...]
 * Dosya verilmezse board.js'in varsayılan EEPROM'uyla çalışır. Verilen 2048 baytlık dosyalar gerçek
 * EEPROM dökümü, diğerleri P4000 kütüğü olarak denenir. */
'use strict';
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let B, P, args, readBin, out;
if (isNode) {
  B = require('./board.js'); P = require('./p4000.js'); args = process.argv.slice(2);
  readBin = f => new Uint8Array(require('fs').readFileSync(f)); out = s => console.log(s);
} else {
  load('m7700.js'); load('board.js'); load('p4000.js');
  B = globalThis.Board; P = globalThis.P4000; args = typeof scriptArgs !== 'undefined' ? scriptArgs : typeof arguments !== 'undefined' ? arguments : [];
  readBin = f => new Uint8Array(read(f, 'binary')); out = s => print(s);
}
let fails = 0;
function check(name, ok, info) { out((ok ? 'ok   ' : 'FAIL ') + name + (info ? '  (' + info + ')' : '')); if (!ok) fails++; }
const same = (a, b) => a.length === b.length && a.every((v, i) => v === b[i]);
const diff = (a, b) => { const d = []; for (let i = 0; i < a.length; i++) if (a[i] !== b[i]) d.push('0x' + i.toString(16) + ':' + a[i].toString(16) + '/' + b[i].toString(16)); return d; };

function suite(name, ee0) {
  out('--- ' + name);
  const ee = Uint8Array.from(ee0);
  // board.js ile aynı kanal yorumu
  let chOk = true, chInfo = '';
  for (let n = 1; n <= 99; n++) {
    const b = B.eeChannel(ee, n), p = P.channel(ee, n);
    if (!b) { if (p.tx) { chOk = false; chInfo = 'kanal ' + n + ' board boş, P4000 dolu'; } continue; }
    if (b.tx !== p.tx || b.rx !== p.rx || b.rxTone !== p.rxTcs || b.txTone !== p.txTcs || b.selcall !== p.ani || b.xshift !== p.xshift) { chOk = false; chInfo = 'kanal ' + n; }
  }
  check('kanallar board.js eeChannel ile aynı', chOk, chInfo);
  // her alan için oku -> yaz baytları değiştirmemeli
  let fOk = true, fInfo = '';
  for (const x of P.FIELDS) {
    if (x.t === 'ro') continue;
    const t = Uint8Array.from(ee);
    try { P.set(t, x, P.get(t, x)); } catch (e) { if (x.t !== 'freq') { fOk = false; fInfo = x.k + ': ' + e.message; } continue; }
    if (!same(t, ee) && x.t !== 'str') { fOk = false; fInfo = x.k + ' ' + diff(t, ee).slice(0, 4).join(' '); }
  }
  check('alanlar: oku -> yaz baytları korur', fOk, fInfo);
  let cOk = true, cInfo = '';
  for (let n = 1; n <= 99; n++) {
    const t = Uint8Array.from(ee), c = P.channel(t, n);
    if (!c.tx && !c.rx) continue;
    try { P.setChannel(t, n, c); } catch (e) { cOk = false; cInfo = n + ': ' + e.message; continue; }
    const d = diff(t, ee);
    if (d.length) { cOk = false; cInfo = 'kanal ' + n + ' ' + d.slice(0, 3).join(' '); }
  }
  check('kanallar: oku -> yaz baytları korur (kristal kaydırma P4000 kuralıyla)', cOk, cInfo);
  // .p40 gidiş-dönüş: kütüğün kapsadığı değerler aynı kalmalı; kütükte olmayanlar (kanal dışı
  // alanlarda) bayt bayt aynı. Kanal kayıtlarında P4000 boş kanalları ve kristal kaydırma bitini
  // kendi kuralıyla yeniden yazar.
  const p40 = P.toP40(ee), back = P.fromP40(p40, ee);
  const t1 = P.dec1254(p40), t2 = P.dec1254(P.toP40(back.ee));
  check('.p40 yaz -> oku aynı değerler', t1 === t2 && back.warnings.length === 0, back.warnings.slice(0, 2).join('; '));
  const again = P.fromP40(P.toP40(back.ee), back.ee), d = diff(again.ee, back.ee);
  check('.p40 ikinci gidiş-dönüş bayt bayt aynı', d.length === 0, d.slice(0, 6).join(' '));
  check('.p40 bölümleri', /\[TestAyar\][\s\S]*\[User\][\s\S]*\[Clone\][\s\S]*PriorChannel2=[\s\S]*\[Kanal\][\s\S]*\[KSYS\][\s\S]*\[ADATA\]/.test(P.dec1254(p40)));
  return ee;
}

/* 1. varsayılan EEPROM */
const def = B.defaultEeprom();
suite('board.js varsayılan EEPROM', def);
check('firmware sağlamaları = P4000 ilk dört blok', P.checkSums(def).slice(0, 4).every(s => s.ok));

/* 2. bit tanımları: P4000'deki sıra (en anlamlı bit önce, bölüm bölüm) */
{
  const t = new Uint8Array(2048);
  P.setBits(t, '4.7.8;3.7.8', 0x349F);
  check('tanım "4.7.8;3.7.8": ilk bölüm üst bayt', t[4] === 0x34 && t[3] === 0x9F);
  P.setBits(t, '46.5.6;47.7.8', 0x3ABC);
  check('tanım "46.5.6;47.7.8": 14 bit kelime', (t[46] & 0x3F) === 0x3A && t[47] === 0xBC && P.getBits(t, '46.5.6;47.7.8') === 0x3ABC);
  P.setBits(t, '52.7.1;53.7.1;54.7.1;55.7.1;56.7.1;57.7.1', 0b101101);
  check('TX TCS 6 bit, +6..+11 bit 7', [52, 53, 54, 55, 56, 57].map(a => t[a] >> 7).join('') === '101101');
}

/* 3. değer dönüşümleri */
{
  const t = B.defaultEeprom();
  P.set(t, 'mhkTarih', { gun: 6, ay: 9, yil: 2025 });
  check('MHK tarihi 06.09.2025 -> 0x003..0x004 = 9F 34', t[3] === 0x9F && t[4] === 0x34);
  P.set(t, 'aniNo', '1234');
  check('ANI No "1234": 0x00D = 21, 0x00C = 43, 0x00B = FF', t[0xD] === 0x21 && t[0xC] === 0x43 && t[0xB] === 0xFF && P.get(t, 'aniNo') === '1234');
  P.set(t, 'tot', 180); check('Tot 180 sn -> 0x01D üst yarım = 12', (t[0x1D] >> 4) === 12 && P.get(t, 'tot') === 180);
  P.set(t, 'txGec', 600); check('Tx gecikmesi 600 ms -> 0x01A alt yarım = 6', (t[0x1A] & 15) === 6);
  P.set(t, 'tarama', 1.5); check('Tarama gecikmesi 1.5 sn -> 0x01C alt yarım = 3', (t[0x1C] & 15) === 3);
  P.set(t, 'aniTip', 2); check('ANI tipi PTT İle -> 0x01A üst yarım = 2 (firmware: PTT\'de kimlik)', (t[0x1A] >> 4) === 2);
  P.set(t, 'dil', 1); check('Dil Türkçe -> 0x018 bit 7', !!(t[0x18] & 0x80));
  P.set(t, 'keypad', 1); check('Tuş takımı -> 0x014 bit 3', !!(t[0x14] & 8));
  P.set(t, 'mesgul', 2); check('Meşgul kanal kilidi TCS ile -> 0x018 b4 + 0x026 b3', (t[0x18] & 0x10) && (t[0x26] & 8));
  P.set(t, 'oncelik1', 5); check('1. öncelikli kanal -> 0x544', t[0x544] === 5);
  P.set(t, 'kRx', 145.6e6); check('KSYS alma frekansı -> kelime', P.get(t, 'kRx') === 145.6e6);
  P.set(t, 'aDurum3', 'MESAJ'); check('Durum 3 -> 0x622', String.fromCharCode(...t.slice(0x622, 0x62A)) === 'MESAJ   ');
  check('kristal kaydırma: 145,656 MHz (289 × 504 kHz) var, 145,700 MHz yok, UHF yok',
    P.xshift(145.656e6) === 1 && P.xshift(145.7e6) === 0 && P.xshift(433.0e6) === 0);
  const c = P.setChannel(t, 20, { name: 'aprs', tx: 144.8e6, rx: 144.8e6, txTcs: 8, rxTcs: 0, tot: 1, hprf: 1, ani: 0, scan: 1 });
  const b = B.eeChannel(t, 20);
  check('setChannel -> board.js okur', b && b.tx === 144.8e6 && b.txTone === 8 && b.name === 'APRS    ' && c.scan === 1 && (t[0x537 + 2] & 8));
  P.setChannel(t, 20, { tx: 0, rx: 0 });
  check('frekansı silinen kanal boş (0x3FFF), ad kalır', B.eeChannel(t, 20) === null && P.channel(t, 20).name === 'APRS    ');
  let err = '';
  try { P.setChannel(t, 21, { tx: 144.8013e6, rx: 144.8e6 }); } catch (e) { err = e.message; }
  check('2,5 kHz adımı dışı reddedilir', /2,5 kHz/.test(err));
  P.fixSums(t);
  check('fixSums -> yedi blok tutar', P.checkSums(t).every(s => s.ok));
}

/* 4. dışarıdan verilen dosyalar */
for (const f of args) {
  const raw = readBin(f);
  if (raw.length === 2048) {
    const ee = suite(f, raw);
    const s = P.checkSums(ee);
    check('sağlamalar: ' + s.map(x => x.name + (x.ok ? '✓' : '✗')).join(' '), s.every(x => x.ok));
    out('     seri no ' + P.get(ee, 'seriNo') + ', MHK ' + JSON.stringify(P.get(ee, 'mhkTarih')) + ', sürüm "' + P.get(ee, 'versiyon') + '", üretim ' + P.get(ee, 'uretimNo') +
      ', telsiz ' + P.telsizTipi(ee) + ' / ' + P.donanimTipi(ee) + ', bant ' + P.bandText(ee) + ', ANI ' + P.get(ee, 'aniNo'));
  } else {
    out('--- kütük ' + f);
    const r = P.fromP40(raw, B.defaultEeprom(), { setBase: true });
    check('kütük okundu, uyarı yok', r.warnings.length === 0, r.warnings.slice(0, 3).join('; '));
    const ini = P.parseIni(P.dec1254(raw)), back = P.parseIni(P.dec1254(P.toP40(r.ee)));
    const skip = /^TestAyar/;
    let bad = [];
    for (const s of Object.keys(ini)) for (const k of Object.keys(ini[s])) {
      if (skip.test(s + '/' + k)) continue;
      const a = ini[s][k], b = (back[s] || {})[k];
      const num = x => x !== undefined && x !== '' && !isNaN(+x) ? +x : x;
      if (num(a) !== num(b) && !(s === 'Kanal' && /^Kanal Adi/.test(k) && a.toUpperCase() === b)) bad.push(s + '/' + k + ': ' + a + ' → ' + b);
    }
    check('kütük -> EEPROM -> kütük aynı değerler', bad.length === 0, bad.length + ' fark: ' + bad.slice(0, 4).join('; '));
    for (let n = 1; n <= 3; n++) { const c = P.channel(r.ee, n); out('     ' + n + ' <' + c.name + '> ' + (c.tx / 1e6).toFixed(4) + ' / ' + (c.rx / 1e6).toFixed(4) + ' TCS ' + c.txTcs + '/' + c.rxTcs); }
  }
}
out(fails ? fails + ' HATA' : 'hepsi geçti');
if (isNode && fails) process.exit(1);
