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

/* 4. APRS bölümü (özel firmware, 0x6E0..0x731) */
{
  const t = B.defaultEeprom(), once = t.slice();
  const v = P.aprsGet(t);
  check('APRS: bölüm yokken varsayılanlar (TC19MK-10, /b, Anıtkabir)', !v.gecerli && v.cagri === 'TC19MK-10' && v.tablo + v.kod === '/b' && v.enlem === '3955.52N' && v.boylam === '03250.22E');
  P.aprsSet(t, P.APRS_VARSAYILAN);
  const d = diff(t, once).map(s => parseInt(s.split(':')[0], 16));
  check('APRS: yalnız 0x6E0..0x731 değişir, sağlama tutar', P.aprsGecerli(t) && d.every(a => a >= 0x6E0 && a <= 0x731) && P.checkSums(t).every((s, i) => s.ok === P.checkSums(once)[i].ok), d.length + ' bayt');
  check('APRS: imza, çağrı işareti ve konum yerleri', t[0x6E0] === 0x41 && t[0x6E1] === 0x50 && t[0x6E2] === 1 &&
    String.fromCharCode(...t.slice(0x6E4, 0x6ED)) === 'TC19MK-10' && t[0x6ED] === 0 && String.fromCharCode(...t.slice(0x6F0, 0x6F8)) === '3955.52N' &&
    String.fromCharCode(...t.slice(0x6F8, 0x701)) === '03250.22E' && t[0x6EE] === 0x2F && t[0x6EF] === 0x62);
  P.aprsSet(t, { cagri: 'ta7w-7', enlem: '40.84383', boylam: '31 9.39 E', yorum: 'Düzce küçük Harf, nokta.', aralik: 600 });
  const g = P.aprsGet(t);
  check('APRS: küçük harf çağrı büyüğe, ondalık derece / derece-dakika konum, UTF-8 yorum, aralık',
    g.cagri === 'TA7W-7' && g.enlem === '4050.63N' && g.boylam === '03109.39E' && g.yorum === 'Düzce küçük Harf, nokta.' && g.aralik === 600 && t[0x702] === 2 && t[0x703] === 0x58,
    JSON.stringify(g));
  check('APRS: konum dönüşümleri', P.aprsKonum('-33.86833', true) === '3352.10S' && P.aprsKonum('151 12.34 w', false) === '15112.34W' && P.aprsKonum('3955.52n', true) === '3955.52N' && P.aprsKonum('59.99999', true) === '6000.00N');
  const red = o => { try { P.aprsSet(Uint8Array.from(t), o); return ''; } catch (e) { return e.message; } };
  check('APRS: geçersiz değerler reddedilir',
    /6 harf/.test(red({ cagri: 'TC1881ATA-10' })) && /6 harf/.test(red({ cagri: 'TA7W-16' })) && /43 bayt/.test(red({ yorum: 'ş'.repeat(22) })) &&
    /en az 10/.test(red({ aralik: 5 })) && /Enlem/.test(red({ enlem: '95.0' })) && /Boylam/.test(red({ boylam: 'abc' })) && /Simge/.test(red({ kod: '' })));
  check('APRS: paket metni', P.aprsPaket(t) === 'TA7W-7>APIZCI,WIDE2-2:!4050.63N/03109.39Eb' + String.fromCharCode(...P.utf8('Düzce küçük Harf, nokta.')), P.aprsPaket(t));
  const p40 = P.fromP40(P.toP40(t), t).ee;
  check('APRS: .p40 okuma bölümü korur', P.aprsGecerli(p40) && P.aprsGet(p40).cagri === 'TA7W-7');
}

/* bant tabanı değiştirme (eeprom.html): frekanslar, bayraklar ve sağlamalar korunur; sığmayan taban reddedilir */
{
  const ee = B.defaultEeprom(); P.set(ee, 'kRx', 150e6); P.set(ee, 'kTx', 151e6); ee[0x2E] |= 0x80; P.fixSums(ee);
  const once = []; for (let n = 1; n <= 16; n++) once.push(P.channel(ee, n));
  const s0 = P.tabanSinir(ee);
  check('taban sınırı: VHF, 130 MHz – en düşük frekans (150)', s0.aile === 'VHF' && s0.min === 5200 && s0.max === 6000, JSON.stringify(s0));
  const r = P.rebase(ee, 5760);
  const ayni = once.every((c, i) => { const d = P.channel(ee, i + 1); return d.tx === c.tx && d.rx === c.rx && d.name === c.name && d.rxTcs === c.rxTcs; });
  check('136 → 144 MHz: kanallar ve KSYS aynı frekansta, bayrak korunur, sağlamalar tutar',
    r.ok && P.base(ee) === 5760 && ayni && P.get(ee, 'kRx') === 150e6 && P.get(ee, 'kTx') === 151e6 && (ee[0x2E] & 0x80) && P.checkSums(ee).every(x => x.ok), JSON.stringify(r));
  const z = B.defaultEeprom(); P.fixSums(z);
  check('sıfır KSYS bloğu frekans sayılmaz (taban 144\'e iner)', P.rebase(z, 5760).ok && P.base(z) === 5760 && P.checkSums(z).every(x => x.ok));
  const kopya = ee.slice();
  const r2 = P.rebase(ee, 6040), r3 = P.rebase(ee, 16000);
  check('sığmayan / başka banda taban reddedilir, EEPROM değişmez', !r2.ok && /KSYS alma/.test(r2.hatalar.join()) && !r3.ok && same(ee, kopya), r2.hatalar.concat(r3.hatalar).join('; '));
}

/* 5. dışarıdan verilen dosyalar */
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
if (!isNode && fails) throw new Error(fails + ' test başarısız');   // jsc: yakalanmamış istisna -> çıkış kodu 3 (quit(n) kodu iletmiyor)
