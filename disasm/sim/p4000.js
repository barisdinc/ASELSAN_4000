/* p4000.js - ASELSAN P4000 programlama yazılımının (P4000.EXE 25.11.1998, Borland C++/OWL)
 * EEPROM yerleşimi, sayfaları ve .p40 dosya biçimi.
 *
 * Kaynak: P4000.EXE decompile edildi. Yazılım her parametreyi bir global değişkene bağlar ve
 * "adres.bit.uzunluk;adres.bit.uzunluk" biçiminde bir tanımla 2 KB'lık EEPROM görüntüsüne
 * paketler (kayıt 0x475900, paketleme 0x475D14, açma 0x475E90; tanımlar 0x490AC0). Değişkenin
 * bitleri en anlamlı bitten başlayarak bölümlere sırayla yazılır, her bölümde verilen bitten
 * aşağı doğru. Aşağıdaki "d" alanları P4000'deki tanımların aynısıdır (telsiz tipi 0 = EL telsizi,
 * kayıt fonksiyonları 0x476004..0x4783E8). Sayfa ve alan adları P4000'in Türkçe diyaloglarından
 * (kaynak 61..65, 72), .p40 anahtarları P4000'in dosya okuma / yazma kodundan (0x487B3C,
 * 0x48926C) alındı. Sağlama blokları 0x480CB0'daki okuma tablosundan.
 *
 *   P4000.FIELDS, P4000.PAGES      alan ve sayfa tanımları
 *   P4000.get(ee, f) / set(ee, f, v)
 *   P4000.channel(ee, n) / setChannel(ee, n, c)
 *   P4000.checkSums(ee) / fixSums(ee)
 *   P4000.toP40(ee) -> Uint8Array,  P4000.fromP40(bytes, ee) -> { ee, warnings, base }
 *   P4000.render(el, ee, { editable, onChange, page, chPage })  (tarayıcıda) */
(function (root) {
'use strict';

/* ---------------------------------------------------------------- bit alanları */
function segs(d) { return d.split(';').map(s => s.split('.').map(Number)); }
function getBits(ee, d) {
  let v = 0;
  for (const [a, b, n] of segs(d)) for (let i = 0; i < n; i++) v = v * 2 + (ee[a] >> (b - i) & 1);
  return v;
}
function setBits(ee, d, v) {
  const s = segs(d);
  let left = s.reduce((t, x) => t + x[2], 0);
  for (const [a, b, n] of s) for (let i = 0; i < n; i++) {
    left--;
    const m = 1 << (b - i);
    if (Math.floor(v / Math.pow(2, left)) & 1) ee[a] |= m; else ee[a] &= ~m & 0xFF;
  }
}
const D = (a, b, n) => a + '.' + b + '.' + n;
const nbits = d => segs(d).reduce((t, x) => t + x[2], 0);

/* ---------------------------------------------------------------- sabitler */
/* P4000 TCS tablosu (0x4908BC): ton numarası 1..50, telsizin tablosuyla aynı sıra; P4000 tam
 * sayı Hz yazar (88.5 -> 88). */
const TCS_HZ = [67, 71, 74, 77, 79, 82, 85, 88, 91, 94, 97, 100, 103, 107, 110, 114, 118, 123, 127, 131, 136, 141,
  146, 151, 156, 162, 167, 173, 179, 186, 192, 203, 210, 218, 225, 233, 241, 250, 69, 159, 165, 171, 177, 183, 189,
  196, 199, 206, 229, 254];
const TCS_TAM = [67.0, 71.9, 74.4, 77.0, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100.0, 103.5, 107.2, 110.9, 114.8,
  118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5,
  210.7, 218.1, 225.7, 233.6, 241.8, 250.3, 69.3, 159.8, 165.5, 171.3, 177.3, 183.5, 189.9, 196.6, 199.5, 206.5, 229.1, 254.1];

const CP = { 'ı': 0xFD, 'İ': 0xDD, 'ş': 0xFE, 'Ş': 0xDE, 'ğ': 0xF0, 'Ğ': 0xD0, 'ü': 0xFC, 'Ü': 0xDC, 'ö': 0xF6, 'Ö': 0xD6, 'ç': 0xE7, 'Ç': 0xC7 };
const CPR = {}; for (const k in CP) CPR[CP[k]] = k;

/* Telsizin açılışta denetlediği ve P4000'in yazarken güncellediği bloklar: [ad, başlangıç, uzunluk]
 * 16 bit bayt toplamı, hemen arkasına büyük uçlu yazılır. İlk dördünü firmware de denetler. */
const SUMS = [['SABİT', 0x000, 0x05], ['SERVİS', 0x008, 0x0E], ['AKTARMA', 0x018, 0x51D], ['TEST-AYAR', 0x578, 0x45],
  ['KULLANICI', 0x54B, 0x07], ['ANALOG VERİ', 0x600, 0xD2], ['KSYS', 0x7E0, 0x14]];
function sum(ee, a, n) { let s = 0; for (let i = 0; i < n; i++) s += ee[a + i]; return s & 0xFFFF; }
function checkSums(ee) { return SUMS.map(([name, a, n]) => ({ name, a, n, ok: (ee[a + n] << 8 | ee[a + n + 1]) === sum(ee, a, n) })); }
function fixSums(ee) { for (const [, a, n] of SUMS) { const s = sum(ee, a, n); ee[a + n] = s >> 8; ee[a + n + 1] = s & 0xFF; } return ee; }

/* ---------------------------------------------------------------- alanlar
 * t: bool | int | enum | digits (6 haneli numara, 0xF = boş) | bcd (üretim sıra no) | str | date
 * inv: kutu işaretliyken bit 0; scale: gösterilen = ham × scale; ini: .p40 [bölüm] anahtar */
const F = [], BY = {};
function f(o) { F.push(o); BY[o.k] = o; return o; }
// Sabit Parametreler (diyalog 61) - SABİT alanı 0x000..0x006 ve TEST-AYAR alanının kimlik baytları
f({ k: 'seriNo', p: 'sabit', l: 'Seri No', d: '1.7.8;2.7.8', t: 'int', max: 65535 });
f({ k: 'sabitMerkez', p: 'sabit', l: 'Sabit Merkez', d: '0.5.1', t: 'bool' });
f({ k: 'compander', p: 'sabit', l: 'Ses Sıkıştırma', d: '0.3.1', t: 'bool', ini: ['Master', 'Compander'] });
f({ k: 'tdm', p: 'sabit', l: 'TDM', d: '0.2.1', t: 'bool', ini: ['Master', 'TDM'] });
f({ k: 'mhkTarih', p: 'sabit', l: 'MHK Programlama Tarihi', d: '4.7.8;3.7.8', t: 'date' });
f({ k: 'uretimNo', p: 'sabit', l: 'Üretim Sıra No', a: 0x588, t: 'bcd' });
f({ k: 'versiyon', p: 'sabit', l: 'Cihaz Versiyon No', a: 0x578, n: 16, t: 'str' });
// Özel Parametreler (diyalog 62) - SERVİS alanı 0x008..0x015
f({ k: 'progSurum', p: 'ozel', l: 'Programlayıcı Sürüm No', t: 'ro', get: ee => ee[9] + '.' + ee[8] });
f({ k: 'aniNo', p: 'ozel', l: 'ANI No', a: 0x00B, t: 'digits' });
f({ k: 'secNo', p: 'ozel', l: 'Seçmeli Çağrı No', a: 0x00E, t: 'digits' });
f({ k: 'ackNo', p: 'ozel', l: 'ACK No', a: 0x011, t: 'digits' });
f({ k: 'cloneKey', p: 'ozel', l: 'Aktarma Anahtarı', d: '10.7.8', t: 'int', max: 255, ini: ['Service', 'CloneKey'] });
f({ k: 'lp', p: 'ozel', l: 'Düşük güç', d: '21.3.4', t: 'int', max: 15, ini: ['Service', 'LPIndex'] });
f({ k: 'hp', p: 'ozel', l: 'Yüksek güç', d: '21.7.4', t: 'int', max: 15, ini: ['Service', 'HPIndex'] });
f({ k: 'optStart', p: 'ozel', l: 'Açılışta Opsiyon', d: '20.5.1', t: 'bool', ini: ['Service', 'OptionAtStart'] });
f({ k: 'scEnc', p: 'ozel', l: 'Seçmeli Çağrı Gönderme', d: '20.2.1', t: 'bool', ini: ['Service', 'SCEnc'] });
f({ k: 'monitor', p: 'ozel', l: 'Kanal İzleme Özelliği', d: '20.6.1', t: 'bool', inv: true, ini: ['Service', 'MonitorDisabled'] });
f({ k: 'keypad', p: 'ozel', l: 'Tuş Takımı', d: '20.3.1', t: 'bool', ini: ['Service', 'KeyPad'] });
f({ k: 'cloner', p: 'ozel', l: 'Aktarma Özelliği', d: '20.4.1', t: 'bool', ini: ['Service', 'Cloner'] });
f({ k: 'micType', p: 'ozel', l: 'Sabit Merkez Telsizi', d: '20.1.1', t: 'bool', ini: ['Service', 'MicType'] });
f({ k: 'vox', p: 'ozel', l: 'VOX', d: '20.0.1', t: 'bool', ini: ['Service', 'VOXExists'] });
f({ k: 'marine', p: 'ozel', l: 'Deniz El Telsizi', d: '20.7.1', t: 'bool', ini: ['Service', 'MarineRadio'] });
// Aktarılabilir Parametreler (diyalog 63) - AKTARMA alanı 0x018..0x02D, KULLANICI 0x54B, öncelik 0x544
f({ k: 'dil', p: 'aktar', l: 'Dil', d: '24.7.1', t: 'enum', o: ['İngilizce', 'Türkçe'] });
f({ k: 'mesgul', p: 'aktar', l: 'Meşgul Kanal Kilidi', t: 'enum', o: ['Yok', 'Kanal Aktif ile', 'TCS ile'], ini: ['Clone', 'BusyChan'],
  get: ee => getBits(ee, '24.4.1') ? (getBits(ee, '38.3.1') ? 2 : 1) : 0,
  set: (ee, v) => { setBits(ee, '24.4.1', v ? 1 : 0); setBits(ee, '38.3.1', v === 2 ? 1 : 0); } });
f({ k: 'harici', p: 'aktar', l: 'Harici Giriş', d: '38.4.1', t: 'enum', o: ['Ses', 'Veri'], ini: ['Clone', 'ExtInputModem'] });
f({ k: 'gucTas', p: 'aktar', l: 'Güç Tasarrufu', d: '24.6.1', t: 'bool', ini: ['Clone', 'PowerSave'] });
f({ k: 'altMenu', p: 'aktar', l: 'Alt Menü', d: '24.5.1', t: 'bool', ini: ['Clone', 'DigerMenu'] });
f({ k: 'acil', p: 'aktar', l: 'Acil Çağrı', d: '25.7.1', t: 'bool', ini: ['Clone', 'AcilCagri'] });
f({ k: 'acilKanal', p: 'aktar', l: 'Acil Çağrı Kanalı', d: '25.6.7', t: 'int', max: 99, ini: ['Clone', 'AcilCagriKanali'] });
f({ k: 'oncelik1', p: 'aktar', l: '1. Kanal', d: '1348.7.8', t: 'int', max: 99, ini: ['Clone', 'PriorChannel1'] });
f({ k: 'oncelik2', p: 'aktar', l: '2. Kanal', d: '1349.7.8', t: 'int', max: 99, ini: ['Clone', 'PriorChannel2'] });
f({ k: 'tot', p: 'aktar', l: 'Tot (0-225 sn)', d: '29.7.4', t: 'int', scale: 15, max: 225, ini: ['Clone', 'Tot'] });
f({ k: 'ilkTon', p: 'aktar', l: 'Uzatılmış İlk Ton Süresi (1-15)', d: '27.7.4', t: 'int', min: 1, max: 15, ini: ['Clone', 'ExtendedTon'] });
f({ k: 'rekey', p: 'aktar', l: 'Rekey Gecikmesi (0-15 sn)', d: '29.3.4', t: 'int', max: 15, ini: ['Clone', 'RekeyDelay'] });
f({ k: 'tarama', p: 'aktar', l: 'Tarama Gecikmesi (0-7.5 sn)', d: '28.3.4', t: 'int', scale: 0.5, max: 7.5, ini: ['Clone', 'ScanDelay'] });
f({ k: 'txGec', p: 'aktar', l: 'Tx Gecikmesi (0-1500 ms)', d: '26.3.4', t: 'int', scale: 100, max: 1500, ini: ['Clone', 'TxDelay'] });
f({ k: 'tonStd', p: 'aktar', l: 'Ton Standardı', d: '24.1.2', t: 'enum', o: ['DSC', 'CCIR', 'ZVEI', 'EEA'], ini: ['Clone', 'TonStandardi'], iniV: ['DSC', 'CCIR', 'ZVEI', 'EEA'] });
f({ k: 'hane', p: 'aktar', l: 'Seçilebilir çağrı hane sayısı', d: '28.7.4', t: 'enum', o: { 0: 'Yok', 2: '2', 3: '3', 4: '4', 5: '5', 6: '6' }, ini: ['Clone', 'UserSelectedDig'] });
f({ k: 'aniTip', p: 'aktar', l: 'ANI Tipi', d: '26.7.4', t: 'enum', o: ['Yok', 'Çağrı İle', 'PTT  İle', 'PTT Bırakılınca', 'Çağrı ve PTT İle', 'Çağrı ve PTT Bırakılınca'],
  ini: ['Clone', 'ANITipi'], iniV: ['Yok', 'Cagri', 'PTT', 'PTTBirak', 'CagriPTT', 'CagriPTTBirak'] });
f({ k: 'invDisp', p: 'aktar', l: 'Gösterge ters', d: '1355.5.1', t: 'bool', ini: ['User', 'invdisplay'] });
f({ k: 'isikKapali', p: 'aktar', l: 'Işıklar kapalı', d: '1355.0.1', t: 'bool', ini: ['User', 'lightsoff'] });
f({ k: 'uyariYok', p: 'aktar', l: 'Sesli uyarı yok', d: '1355.1.1', t: 'bool', ini: ['User', 'warningoff'] });
f({ k: 'grupTonu', p: 'aktar', l: 'Grup Tonu', d: '24.2.1', t: 'enum', o: ['0', 'A'], ini: ['Clone', 'GrupTonu'], iniV: ['0', 'A'] });
f({ k: 'oncelik', p: 'aktar', l: 'Öncelik', d: '24.3.1', t: 'enum', o: ['Çağrı', 'ANI'], ini: ['Clone', 'Oncelik'], iniV: ['Call', 'PTT'] });
// diyalogda görünmeyen ama .p40'ta saklanan
f({ k: 'tonSayisi', p: 'gizli', l: 'Ton Sayısı (seçmeli çağrı no uzunluğu)', d: '27.3.4', t: 'int', max: 15, ini: ['Clone', 'TonLength'] });
// KSYS Parametreleri (diyalog 65) - KSYS alanı 0x7E0..0x7F3
f({ k: 'kSys', p: 'ksys', l: 'Sistem Kimliği', d: '2016.7.8;2017.7.8', t: 'int', max: 65535, ini: ['KSYS', 'SYSID'] });
f({ k: 'kSmt', p: 'ksys', l: 'SMT Kimliği', d: '2018.7.8', t: 'int', max: 255, ini: ['KSYS', 'SMTIDENT'] });
f({ k: 'kRx', p: 'ksys', l: 'Alma Frekansı', d: '2019.7.8;2020.7.8', t: 'freq', ini: ['KSYS', 'RxFreq'] });
f({ k: 'kRxT', p: 'ksys', l: 'Alma TCS Ton', d: '2021.7.8', t: 'tcs', ini: ['KSYS', 'RxTCS'] });
f({ k: 'kTx', p: 'ksys', l: 'SMT Gönderme Frekansı', d: '2022.7.8;2023.7.8', t: 'freq', ini: ['KSYS', 'TxFreq'] });
f({ k: 'kTxT', p: 'ksys', l: 'Gönderme TCS Ton', d: '2024.7.8', t: 'tcs', ini: ['KSYS', 'TxTCS'] });
f({ k: 'kHp', p: 'ksys', l: 'Yüksek Güç', d: '2025.0.1', t: 'bool', ini: ['KSYS', 'Yüksek Güç'] });
for (let i = 1; i <= 10; i++) f({ k: 'kGrup' + i, p: 'ksys', g: 'grup', l: i + '', d: D(2025 + i, 7, 8), t: 'int', max: 255, ini: ['KSYS', 'Grup No[' + i + ']'] });
// Analog Veri Parametreleri (diyalog 72) - ANALOG VERİ alanı 0x600..0x6D1
f({ k: 'aSys', p: 'adata', l: 'Sistem Kimliği', d: '1536.7.8', t: 'int', max: 255, ini: ['ADATA', 'SYSID'] });
f({ k: 'aFleet', p: 'adata', l: 'Bölge Kimliği', d: '1537.7.8;1538.7.8', t: 'int', signed: true, min: -32768, max: 32767, ini: ['ADATA', 'FLEETID'] });
f({ k: 'aRadio', p: 'adata', l: 'Telsiz Kimliği', d: '1539.7.8;1540.7.8', t: 'int', signed: true, min: -32768, max: 32767, ini: ['ADATA', 'RADIOID'] });
f({ k: 'aRx', p: 'adata', l: 'Merkez Kimliği', d: '1541.7.8;1542.7.8', t: 'int', signed: true, min: -32768, max: 32767, ini: ['ADATA', 'RXID'] });
f({ k: 'aPre', p: 'adata', l: 'Röle Tetiklenme Süresi', d: '1543.7.8', t: 'int', max: 255, ini: ['ADATA', 'PREAMBLE'] });
for (let i = 1; i <= 5; i++) f({ k: 'aGrup' + i, p: 'adata', g: 'grup', l: 'Grup ' + i, d: D(1542 + 2 * i, 7, 8) + ';' + D(1543 + 2 * i, 7, 8), t: 'int', signed: true, min: -32768, max: 32767, ini: ['ADATA', 'GRUP[' + i + ']'] });
for (let i = 1; i <= 24; i++) f({ k: 'aDurum' + i, p: 'adata', g: 'durum', l: 'Durum ' + i, a: 0x612 + 8 * (i - 1), n: 8, t: 'str', ini: ['ADATA', 'STATUS[' + i + ']'] });
const FIELDS = F;

function base(ee) { return ee[0x58C] << 8 | ee[0x58D]; }        // HalfBaseChan, 25 kHz birim
function bandText(ee) {
  const mhz = Math.floor(base(ee) / 40);
  return { 146: '146-174', 406: '406-430', 430: '430-450', 450: '450-470' }[mhz] || mhz + ' - ' + (mhz + 30);
}
function telsizTipi(ee) {                                      // 0x469A9E: 0x58B bit 1 / 5, sabit merkez
  const b = ee[0x58B];
  return !(b & 2) ? 'EL' : b & 0x20 ? 'TEKRARLAYICI' : getBits(ee, '0.5.1') ? 'SABİT MERKEZ' : 'ARAÇ';
}
function donanimTipi(ee) {
  const b = ee[0x58B];
  return b & 8 ? 'SAYISAL KRİPTO' : b & 0x40 ? 'SAYISAL KRİPTO II' : b & 1 ? 'TDM' : b & 0x10 ? 'KSYS' : 'STANDART';
}
f({ k: 'telsizTipi', p: 'sabit', l: 'Telsiz Tipi', t: 'ro', get: telsizTipi });
f({ k: 'donanimTipi', p: 'sabit', l: 'Donanım Tipi', t: 'ro', get: donanimTipi });
f({ k: 'bant', p: 'sabit', l: 'Frekans Bandı', t: 'ro', get: bandText });

function wordToHz(ee, w) { return w === 0x3FFF ? 0 : base(ee) * 25000 + w * 2500; }
function hzToWord(ee, hz) {
  if (!hz) return 0x3FFF;
  const w = (hz - base(ee) * 25000) / 2500;
  if (w !== Math.round(w)) throw new Error('frekans 2,5 kHz adımında olmalı');
  if (w < 0 || w > 0x3FFE) throw new Error('frekans ' + (base(ee) * 0.025).toFixed(3) + '–' + ((base(ee) * 25000 + 0x3FFE * 2500) / 1e6).toFixed(4) + ' MHz dışında');
  return w;
}

function get(ee, x) {
  if (typeof x === 'string') x = BY[x];
  if (x.get) return x.get(ee);
  switch (x.t) {
    case 'digits': {                                           // 0x4638EC / 0x463970: ilk hane son baytın düşük yarısında
      let s = '';
      for (let i = 0; i < 6; i++) { const b = ee[x.a + 2 - (i >> 1)], h = i & 1 ? b >> 4 : b & 15; if (h < 14) s += h.toString(16).toUpperCase(); }
      return s;
    }
    case 'bcd': { let s = ''; for (let i = 0; i < 6; i++) s += (ee[x.a + (i >> 1)] >> (i & 1 ? 0 : 4) & 15).toString(16).toUpperCase(); return s; }
    case 'str': {                                              // 8 bit ham baytlar (Windows-1254); sürüm C dizisi
      let s = '';
      for (let i = 0; i < x.n; i++) { const c = ee[x.a + i]; if (!c && x.p === 'sabit') break; s += CPR[c] || String.fromCharCode(c); }
      return s;
    }
    case 'date': { const v = getBits(ee, x.d); return { gun: v >> 11, ay: v >> 7 & 15, yil: (v & 127) + 1994 }; }
    case 'freq': return wordToHz(ee, getBits(ee, x.d));
    default: {
      let v = getBits(ee, x.d);
      if (x.signed) { const w = nbits(x.d); if (v >= Math.pow(2, w - 1)) v -= Math.pow(2, w); }   // P4000 movsx ile yazar
      return x.inv ? 1 - v : x.scale ? v * x.scale : v;
    }
  }
}
function set(ee, x, v) {
  if (typeof x === 'string') x = BY[x];
  if (x.set) return x.set(ee, v);
  switch (x.t) {
    case 'ro': return;
    case 'digits': {
      const s = String(v).toUpperCase().replace(/[^0-9A-D]/g, '').slice(0, 6);
      for (let i = 0; i < 6; i++) {
        const h = i < s.length ? parseInt(s[i], 16) : 15, a = x.a + 2 - (i >> 1);
        ee[a] = i & 1 ? (ee[a] & 0x0F) | h << 4 : (ee[a] & 0xF0) | h;
      }
      return;
    }
    case 'bcd': {
      const s = String(v).replace(/\D/g, '').padStart(6, '0').slice(-6);
      for (let i = 0; i < 3; i++) ee[x.a + i] = +s[2 * i] << 4 | +s[2 * i + 1];
      return;
    }
    case 'str': for (let i = 0; i < x.n; i++) ee[x.a + i] = i < v.length ? CP[v[i]] || v.charCodeAt(i) & 0xFF : (x.p === 'sabit' ? 0 : 0x20); return;
    case 'date': setBits(ee, x.d, (v.gun & 31) << 11 | (v.ay & 15) << 7 | (v.yil - 1994) & 127); return;
    case 'freq': setBits(ee, x.d, hzToWord(ee, v)); return;
    default: {
      let r = (x.inv ? 1 - v : x.scale ? Math.round(v / x.scale) : v) | 0;
      if (r < 0) r = x.signed ? r + Math.pow(2, nbits(x.d)) : 0;
      setBits(ee, x.d, r);
    }
  }
}

/* ---------------------------------------------------------------- kanallar (Kanal Parametreleri, diyalog 64)
 * kayıt 0x02E + 13·(n−1):  +0 b7 HPRF, b6 TOT, b5..0 + +1 TX kelimesi (14 bit, 2,5 kHz, 0x3FFF boş)
 *   +2 b5..0 + +3 RX kelimesi, +4 b7 ANI, b6 kristal kaydırma (P4000 RX frekansından hesaplar),
 *   b5..0 RX TCS, +5 b7 TX TCS var, +6..+11 b7 TX TCS (6 bit), +5..+12 b6..0 kanal adı (8 karakter)
 * tarama listesi: 0x537 + (n−1)/8, bit (n−1)%8 */
const CH0 = 0x2E;
function chAddr(n) { return CH0 + 13 * (n - 1); }
function xshift(hz) {                                          // 0x4885B1 (32 bit taşmasıyla birlikte)
  if (!hz) return 0;
  const near = (k, lo, hi) => { let e = Math.imul(k, 4032000); if (e < 0) e += 7; e >>= 3; return hz >= e - lo && hz <= e + hi; };
  if (hz < 300e6) { for (let k = 0x121; k <= 0x15A; k++) if (near(k, 25000, 20000)) return 1; }
  else for (let k = 0x325; k <= 0x3A5; k++) if (near(k, 50000, 50000)) return 1;
  return 0;
}
function channel(ee, n) {
  const a = chAddr(n);
  let name = '';
  for (let i = 0; i < 8; i++) { const c = ee[a + 5 + i] & 0x7F; name += c >= 32 && c < 127 ? String.fromCharCode(c) : ' '; }
  const txT = getBits(ee, D(a + 5, 7, 1)) ? getBits(ee, [6, 7, 8, 9, 10, 11].map(i => D(a + i, 7, 1)).join(';')) : 0;
  return {
    n, name, tx: wordToHz(ee, getBits(ee, D(a, 5, 6) + ';' + D(a + 1, 7, 8))), rx: wordToHz(ee, getBits(ee, D(a + 2, 5, 6) + ';' + D(a + 3, 7, 8))),
    txTcs: txT, rxTcs: getBits(ee, D(a + 4, 5, 6)), tot: getBits(ee, D(a, 6, 1)), hprf: getBits(ee, D(a, 7, 1)),
    ani: getBits(ee, D(a + 4, 7, 1)), xshift: getBits(ee, D(a + 4, 6, 1)), scan: ee[0x537 + (n - 1 >> 3)] >> (n - 1 & 7) & 1,
  };
}
/* kristal kaydırma biti P4000'deki gibi yalnız RX frekansı değişince (ya da kütükten okurken,
 * opt.xs) yeniden hesaplanır; telsizden okunan değer aksi hâlde korunur */
function setChannel(ee, n, c, opt) {
  const a = chAddr(n), old = channel(ee, n), o = Object.assign({}, old, c);
  const xs = opt && opt.xs || o.rx !== old.rx ? xshift(o.rx) : old.xshift;
  const tw = hzToWord(ee, o.tx), rw = hzToWord(ee, o.rx);
  setBits(ee, D(a, 5, 6) + ';' + D(a + 1, 7, 8), tw); setBits(ee, D(a + 2, 5, 6) + ';' + D(a + 3, 7, 8), rw);
  setBits(ee, D(a, 7, 1), o.hprf ? 1 : 0); setBits(ee, D(a, 6, 1), o.tot ? 1 : 0);
  setBits(ee, D(a + 4, 7, 1), o.ani ? 1 : 0); setBits(ee, D(a + 4, 6, 1), xs); setBits(ee, D(a + 4, 5, 6), o.rxTcs & 63);
  const name = String(o.name).toUpperCase().padEnd(8).slice(0, 8);
  for (let i = 0; i < 8; i++) setBits(ee, D(a + 5 + i, 6, 7), name.charCodeAt(i) & 0x7F);
  setBits(ee, D(a + 5, 7, 1), o.txTcs ? 1 : 0);
  setBits(ee, [6, 7, 8, 9, 10, 11].map(i => D(a + i, 7, 1)).join(';'), o.txTcs & 63);
  setBits(ee, D(a + 12, 7, 1), 0);
  const sb = 0x537 + (n - 1 >> 3), m = 1 << (n - 1 & 7);
  ee[sb] = o.scan ? ee[sb] | m : ee[sb] & ~m;
  return channel(ee, n);
}

/* ---------------------------------------------------------------- .p40 (P4000 kütüğü, INI biçimi, Windows-1254)
 * P4000 normal kipte [TestAyar] (yalnız bant), [User], [Clone], [Kanal], [KSYS], [ADATA] yazar;
 * [Master] / [Service] yalnız test kipinde okunur / yazılır (burada da okunmaz). */
function enc1254(s) { return Uint8Array.from(Array.from(s, ch => CP[ch] || ch.charCodeAt(0) & 0xFF)); }
function dec1254(b) { let s = ''; for (const c of b) s += CPR[c] || String.fromCharCode(c); return s; }
const mhz = hz => (hz / 1e6).toFixed(4);
function iniVal(x, ee) {
  const v = get(ee, x);
  if (x.iniV) return x.iniV[v] !== undefined ? x.iniV[v] : String(v);
  if (x.t === 'freq') return mhz(v);
  if (x.t === 'tcs') return TCS_HZ[v - 1] ? String(TCS_HZ[v - 1]) : '0';
  if (x.t === 'str') return '<' + v.padEnd(x.n) + '>';
  if (x.k === 'dil') return v ? 'Turkce' : 'Ingilizce';
  if (x.k === 'tarama') return v.toFixed(1);
  return String(v);
}
function toP40(ee) {
  const L = [], sec = s => { if (L.length) L.push(''); L.push('[' + s + ']'); };
  const put = (k, v) => L.push(k + '=' + v);
  const b = base(ee);
  sec('TestAyar'); put('HalfBaseChan[0]', b >> 8); put('HalfBaseChan[1]', b & 0xFF);
  sec('User'); for (const k of ['invDisp', 'isikKapali', 'uyariYok']) put(BY[k].ini[1], iniVal(BY[k], ee));
  sec('Clone');
  put('Language', iniVal(BY.dil, ee));
  for (const k of ['grupTonu', 'harici', 'gucTas', 'mesgul', 'acil', 'acilKanal', 'altMenu', 'txGec', 'ilkTon', 'tot', 'tarama', 'rekey', 'tonSayisi', 'tonStd', 'hane', 'aniTip', 'oncelik'])
    put(BY[k].ini[1], iniVal(BY[k], ee));
  sec('Kanal');
  for (let n = 1; n <= 99; n++) {
    const c = channel(ee, n);
    put('Kanal Adi[' + n + ']', '<' + c.name + '>');
    put('TxFreq[' + n + ']', mhz(c.tx)); put('RxFreq[' + n + ']', mhz(c.rx));
    put('TxTCS[' + n + ']', TCS_HZ[c.txTcs - 1] || ''); put('RxTCS[' + n + ']', TCS_HZ[c.rxTcs - 1] || '');
    put('TotVar[' + n + ']', c.tot); put('Hprf[' + n + ']', c.hprf); put('ANIVar[' + n + ']', c.ani); put('Scan[' + n + ']', c.scan);
  }
  // P4000 öncelikli kanalları kanallardan sonra [Clone]'a ekler (WritePrivateProfileString)
  const ci = L.indexOf('[Kanal]') - 1;
  L.splice(ci, 0, 'PriorChannel1=' + iniVal(BY.oncelik1, ee), 'PriorChannel2=' + iniVal(BY.oncelik2, ee));
  sec('KSYS'); for (const x of F.filter(x => x.p === 'ksys')) put(x.ini[1], iniVal(x, ee));
  sec('ADATA'); for (const x of F.filter(x => x.p === 'adata')) put(x.ini[1], iniVal(x, ee));
  return enc1254(L.join('\r\n') + '\r\n');
}
function parseIni(text) {
  const o = {}; let s = null;
  for (const raw of text.split(/\r?\n/)) {
    const l = raw.replace(/\s+$/, '');
    const m = /^\[(.*)\]$/.exec(l);
    if (m) { s = o[m[1]] = o[m[1]] || {}; continue; }
    const e = l.indexOf('=');
    if (s && e > 0) s[l.slice(0, e)] = l.slice(e + 1);
  }
  return o;
}
function tcsIndex(hz) { const v = Math.round(parseFloat(hz)); if (!v) return 0; const i = TCS_HZ.indexOf(v); return i < 0 ? 0 : i + 1; }
/* opt.setBase: kütüğün bant tabanı EEPROM'dakinden farklıysa EEPROM'unkini değiştir (yalnız
 * simülatörde anlamlı; gerçek telsizde P4000 "frekans bandı aynı değil" diyerek yazmaz). */
function p40Base(bytes) {
  const T = parseIni(dec1254(bytes)).TestAyar || {};
  return (+T['HalfBaseChan[0]'] << 8) + +T['HalfBaseChan[1]'];
}
function fromP40(bytes, cur, opt) {
  const ini = parseIni(dec1254(bytes)), ee = Uint8Array.from(cur), warn = [];
  if (!ini.Clone || !ini.Kanal) throw new Error('P4000 kütüğü değil: [Clone] / [Kanal] bölümü yok');
  const fb = p40Base(bytes);
  if (!(fb > 0)) throw new Error('Seçilen kütükte frekans bandı bilgisi hatalı veya yok !');
  if (fb !== base(ee)) {
    if (opt && opt.setBase) { ee[0x58C] = fb >> 8; ee[0x58D] = fb & 0xFF; }
    else warn.push('kütüğün bandı ' + (fb * 0.025).toFixed(3) + ' MHz, EEPROM\'unki ' + (base(ee) * 0.025).toFixed(3) + ' MHz: frekanslar EEPROM\'un bandına göre yazıldı');
  }
  const int = (s, d) => { const v = parseInt(s, 10); return isNaN(v) ? d : v; };
  for (const x of F) {
    if (!x.ini || x.ini[0] === 'Master' || x.ini[0] === 'Service') continue;
    const s = (ini[x.ini[0]] || {})[x.ini[1]];
    if (s === undefined) continue;
    try {
      if (x.iniV) { const i = x.iniV.indexOf(s); set(ee, x, i < 0 ? (x.k === 'aniTip' ? 0 : x.k === 'tonStd' ? 1 : x.k === 'oncelik' ? 0 : 1) : i); }
      else if (x.t === 'freq') set(ee, x, Math.round(parseFloat(s) * 1e6) || 0);
      else if (x.t === 'tcs') setBits(ee, x.d, tcsIndex(s));
      else if (x.t === 'str') set(ee, x, s.replace(/^</, '').replace(/>$/, ''));
      else if (x.k === 'tarama') set(ee, x, Math.round(parseFloat(s) * 2) / 2);
      else set(ee, x, x.scale ? int(s, 0) : int(s, 0));
    } catch (e) { warn.push(x.ini.join('/') + ': ' + e.message); }
  }
  set(ee, BY.dil, ini.Clone.Language === 'Ingilizce' ? 0 : 1);
  const K = ini.Kanal;
  for (let n = 1; n <= 99; n++) {
    const g = k => K[k + '[' + n + ']'];
    if (g('Kanal Adi') === undefined) continue;
    const fz = s => Math.round(parseFloat(s || '0') * 1e6) || 0;
    const c = { name: (g('Kanal Adi') || '').replace(/^</, '').replace(/>$/, ''), tx: fz(g('TxFreq')), rx: fz(g('RxFreq')),
      txTcs: tcsIndex(g('TxTCS')), rxTcs: tcsIndex(g('RxTCS')), tot: int(g('TotVar'), 1), hprf: int(g('Hprf'), 0), ani: int(g('ANIVar'), 0), scan: int(g('Scan'), 1) };
    try { setChannel(ee, n, c, { xs: true }); }
    catch (e) { warn.push('kanal ' + n + ': ' + e.message + ' (boş bırakıldı)'); setChannel(ee, n, Object.assign(c, { tx: 0, rx: 0 }), { xs: true }); }
  }
  return { ee: fixSums(ee), warnings: warn, base: fb };
}

/* ---------------------------------------------------------------- APRS bölümü (özel firmware)
 * P4000'in de firmware'in de kullanmadığı 0x6E0..0x731; ozel_firmware/ornekler/aprs.inc okur.
 *   +0 "AP", +2 sürüm 1, +4 çağrı işareti[-SSID] (10, 0 ile biter), +14 simge tablosu + kodu,
 *   +16 enlem "DDMM.mmN", +24 boylam "DDDMM.mmE", +34 otomatik gönderim aralığı (s, büyük uçlu),
 *   +36 yorum (43 bayt, 0 ile biter), +80 sağlama (+0..+79 toplamı, büyük uçlu).
 * Bölüm geçersizse (yazılmamışsa) firmware ROM'daki varsayılanları kullanır: APRS_VARSAYILAN. */
const APRS0 = 0x6E0, APRS_UZ = 80, APRS_YORUM = 43;
const APRS_VARSAYILAN = { cagri: 'TC19MK-10', tablo: '/', kod: 'b', enlem: '3955.52N', boylam: '03250.22E', aralik: 0, yorum: 'ASELSAN 4000' };
const APRS_SIMGE = [['/b', 'Bisiklet'], ['/>', 'Araba'], ['/[', 'Yaya'], ['/<', 'Motosiklet'], ['/-', 'Ev'], ['/k', 'Kamyonet'],
  ['/u', 'Kamyon'], ['/j', 'Jip'], ['/R', 'Karavan'], ['/Y', 'Yelkenli'], ['/s', 'Tekne'], ['/v', 'Minibüs'], ['/f', 'İtfaiye'],
  ['/a', 'Ambulans'], ['/O', 'Balon'], ['/\'', 'Uçak'], ['/r', 'Röle'], ['/#', 'Sayısal tekrarlayıcı (digi)'], ['/_', 'Hava istasyonu']];
function utf8(s) {
  const o = [];
  for (const ch of String(s)) {
    let c = ch.codePointAt(0);
    if (c < 0x80) o.push(c);
    else if (c < 0x800) o.push(0xC0 | c >> 6, 0x80 | c & 63);
    else if (c < 0x10000) o.push(0xE0 | c >> 12, 0x80 | c >> 6 & 63, 0x80 | c & 63);
    else o.push(0xF0 | c >> 18, 0x80 | c >> 12 & 63, 0x80 | c >> 6 & 63, 0x80 | c & 63);
  }
  return o;
}
function unutf8(b) { try { return decodeURIComponent(b.map(c => '%' + hex2(c)).join('')); } catch (e) { return b.map(c => String.fromCharCode(c)).join(''); } }
function aprsTopla(ee) { return sum(ee, APRS0, APRS_UZ); }
function aprsGecerli(ee) {
  return ee[APRS0] === 0x41 && ee[APRS0 + 1] === 0x50 && ee[APRS0 + 2] === 1 && (ee[APRS0 + APRS_UZ] << 8 | ee[APRS0 + APRS_UZ + 1]) === aprsTopla(ee);
}
function aprsFix(ee) { const s = aprsTopla(ee); ee[APRS0 + APRS_UZ] = s >> 8; ee[APRS0 + APRS_UZ + 1] = s & 0xFF; return ee; }
/* bölümün içeriği (geçersizse firmware'in kullanacağı varsayılanlar, gecerli: false) */
function aprsGet(ee) {
  if (!aprsGecerli(ee)) return Object.assign({ gecerli: false }, APRS_VARSAYILAN);
  const txt = (a, n) => { const b = []; for (let i = 0; i < n && ee[APRS0 + a + i]; i++) b.push(ee[APRS0 + a + i]); return b; };
  const asc = b => b.map(c => String.fromCharCode(c)).join('');
  return { gecerli: true, cagri: asc(txt(4, 10)), tablo: String.fromCharCode(ee[APRS0 + 14]), kod: String.fromCharCode(ee[APRS0 + 15]),
    enlem: asc(txt(16, 8)), boylam: asc(txt(24, 9)), aralik: ee[APRS0 + 34] << 8 | ee[APRS0 + 35], yorum: unutf8(txt(36, APRS_YORUM)) };
}
/* konum: "3955.52N" (APRS), "39 55.52 N" (derece dakika) ya da "39.92533" / "-32.5" (ondalık derece) */
function aprsKonum(s, enlem) {
  const t = String(s).trim().toUpperCase().replace(',', '.'), H = enlem ? 'NS' : 'EW', dd = enlem ? 2 : 3, mx = enlem ? 90 : 180;
  const ad = enlem ? 'Enlem' : 'Boylam';
  let m = new RegExp('^(\\d{' + (dd - 1) + ',' + dd + '})(\\d\\d\\.\\d\\d)\\s*([' + H + '])$').exec(t), deg, min, h;
  if (m) { deg = +m[1]; min = +m[2]; h = m[3]; }
  else if ((m = new RegExp('^(\\d{1,3})[°\\s]+(\\d{1,2}(?:\\.\\d+)?)\'?\\s*([' + H + '])$').exec(t))) { deg = +m[1]; min = +m[2]; h = m[3]; }
  else if ((m = new RegExp('^([-+]?\\d{1,3}(?:\\.\\d+)?)°?\\s*([' + H + '])?$').exec(t))) {
    let v = +m[1]; h = m[2] || (v < 0 ? H[1] : H[0]); v = Math.abs(v); deg = Math.floor(v); min = (v - deg) * 60;
  } else throw new Error(ad + ' anlaşılamadı: "' + s + '" (örnek ' + (enlem ? '3955.52N ya da 39.92533' : '03250.22E ya da 32.837') + ')');
  min = Math.round(min * 100) / 100;
  if (min >= 60) { deg++; min -= 60; }
  if (deg + min / 60 > mx) throw new Error(ad + ' en çok ' + mx + '°');
  return String(deg).padStart(dd, '0') + min.toFixed(2).padStart(5, '0') + h;
}
function aprsDerece(s) { const m = /^(\d{2,3})(\d\d\.\d\d)([NSEW])$/.exec(s); if (!m) return NaN; const v = +m[1] + m[2] / 60; return /[SW]/.test(m[3]) ? -v : v; }
/* bölümü yazar (eksik alanlar mevcut içerikten / varsayılandan); geçersiz değerde hata */
function aprsSet(ee, o) {
  const v = Object.assign(aprsGet(ee), o);
  const cagri = String(v.cagri).trim().toUpperCase();
  if (!/^[A-Z0-9]{1,6}(-([0-9]|1[0-5]))?$/.test(cagri)) throw new Error('Çağrı işareti en çok 6 harf / rakam, isteğe bağlı -SSID 0..15 olmalı (AX.25 sınırı), ör. TC19MK-10');
  const enlem = aprsKonum(v.enlem, true), boylam = aprsKonum(v.boylam, false);
  const ok = c => typeof c === 'string' && c.length === 1 && c >= '!' && c <= '~';
  if (!ok(v.tablo) || !ok(v.kod)) throw new Error('Simge tablosu ve kodu birer yazılabilir karakter olmalı (ör. / ve b)');
  const aralik = Math.round(+v.aralik || 0);
  if (aralik < 0 || aralik > 65535) throw new Error('Aralık 0..65535 saniye');
  if (aralik && aralik < 10) throw new Error('Aralık en az 10 saniye (0 = kapalı)');
  const yorum = utf8(String(v.yorum).replace(/\s+$/, ''));
  if (yorum.length > APRS_YORUM) throw new Error('Yorum en çok ' + APRS_YORUM + ' bayt (şu an ' + yorum.length + '; Türkçe harfler 2 bayt)');
  if (yorum.some(c => c < 0x20 || c === 0x7F)) throw new Error('Yorumda kontrol karakteri olamaz');
  ee.fill(0, APRS0, APRS0 + APRS_UZ);
  const put = (a, b) => b.forEach((c, i) => { ee[APRS0 + a + i] = c; });
  const asc = s => Array.from(s, c => c.charCodeAt(0));
  put(0, asc('AP')); ee[APRS0 + 2] = 1;
  put(4, asc(cagri)); put(14, asc(v.tablo + v.kod)); put(16, asc(enlem)); put(24, asc(boylam));
  ee[APRS0 + 34] = aralik >> 8; ee[APRS0 + 35] = aralik & 0xFF;
  put(36, yorum);
  return aprsFix(ee);
}
/* firmware'in göndereceği paket (metin; yorumun baytları Latin-1 olarak, afsk.js gibi) */
function aprsPaket(ee) {
  const a = aprsGet(ee), yb = a.gecerli ? (() => { const b = []; for (let i = 0; i < APRS_YORUM && ee[APRS0 + 36 + i]; i++) b.push(ee[APRS0 + 36 + i]); return b; })() : utf8(a.yorum);
  return a.cagri.toUpperCase() + '>APIZCI,WIDE1-1,WIDE2-1:!' + a.enlem.toUpperCase() + a.tablo + a.boylam.toUpperCase() + a.kod + yb.map(c => String.fromCharCode(c)).join('');
}

/* ---------------------------------------------------------------- sayfalar */
const PAGES = [
  { id: 'sabit', title: 'Sabit Parametreler' },
  { id: 'ozel', title: 'Özel Parametreler' },
  { id: 'aktar', title: 'Aktarılabilir Parametreler' },
  { id: 'kanal', title: 'Kanal Parametreleri' },
  { id: 'ksys', title: 'Kablosuz Ses Yayın Sistemi Parametreleri', menu: 'KSYS Parametreleri' },
  { id: 'adata', title: 'Analog Veri Parametreleri' },
  { id: 'test', title: 'Test-Ayar Alanı (T4000)', menu: 'Test-Ayar (T4000)' },
  { id: 'aprs', title: 'APRS (özel firmware)', menu: 'APRS' },
];

/* ---------------------------------------------------------------- tarayıcıda çizim */
function esc(s) { return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/"/g, '&quot;'); }
const hex2 = v => v.toString(16).toUpperCase().padStart(2, '0'), hex3 = v => v.toString(16).toUpperCase().padStart(3, '0');
function where(x) {
  if (x.d) return segs(x.d).map(([a, b, n]) => '0x' + hex3(a) + (n === 8 ? '' : n === 1 ? ' b' + b : ' b' + b + '..' + (b - n + 1))).join(' + ');
  if (x.a !== undefined) return '0x' + hex3(x.a) + (x.n ? '..0x' + hex3(x.a + x.n - 1) : x.t === 'digits' || x.t === 'bcd' ? '..0x' + hex3(x.a + 2) : '');
  return '';
}
function input(x, ee, ro) {
  const v = get(ee, x), dis = ro || x.t === 'ro' ? ' disabled' : '', id = ' data-k="' + x.k + '"', t = ' title="' + esc(where(x)) + '"';
  switch (x.t) {
    case 'bool': return '<label class="p4-cb"' + t + '><input type="checkbox"' + id + (v ? ' checked' : '') + dis + '> ' + esc(x.l) + '</label>';
    case 'enum': {
      const o = Array.isArray(x.o) ? x.o.map((s, i) => [i, s]) : Object.entries(x.o).map(([k, s]) => [+k, s]);
      if (!o.some(([k]) => k === v)) o.push([v, '? (' + v + ')']);
      return '<div class="p4-radio"' + t + '>' + o.map(([k, s]) => '<label><input type="radio" name="p4-' + x.k + '" value="' + k + '"' + id + (k === v ? ' checked' : '') + dis + '> ' + esc(s) + '</label>').join('') + '</div>';
    }
    case 'date': return '<input type="text" class="mono" size="10"' + id + t + ' value="' + String(v.gun).padStart(2, '0') + '.' + String(v.ay).padStart(2, '0') + '.' + v.yil + '"' + dis + '>';
    case 'ro': return '<span class="mono">' + esc(v) + '</span>';
    case 'freq': return '<input type="text" class="mono" size="9"' + id + t + ' value="' + (v ? mhz(v) : '') + '"' + dis + '>';
    case 'tcs': return tcsSelect(v, id + t, dis);
    case 'str': return '<input type="text" class="mono" size="' + (x.n + 1) + '" maxlength="' + x.n + '"' + id + t + ' value="' + esc(v.replace(/\s+$/, '')) + '"' + dis + '>';
    default: return '<input type="text" class="mono" size="' + (x.t === 'digits' || x.t === 'bcd' ? 7 : 5) + '"' + id + t + ' value="' + esc(v) + '"' + dis + '>';
  }
}
function tcsSelect(v, attr, dis) {
  return '<select' + attr + dis + '><option value="0"></option>' + (v > TCS_HZ.length ? '<option value="' + v + '" selected>? (' + v + ')</option>' : '') + TCS_HZ.map((h, i) => '<option value="' + (i + 1) + '"' + (i + 1 === v ? ' selected' : '') + '>' + h + (TCS_TAM[i] !== h ? ' (' + TCS_TAM[i] + ')' : '') + '</option>').join('') + '</select>';
}
function row(x, ee, ro) { return x.t === 'bool' || x.t === 'enum' ? '<div class="p4-row">' + input(x, ee, ro) + '</div>' : '<div class="p4-row"><span class="p4-l">' + esc(x.l) + '</span>' + input(x, ee, ro) + '</div>'; }
function frame(title, html) { return '<fieldset class="p4-fs">' + (title ? '<legend>' + esc(title) + '</legend>' : '') + html + '</fieldset>'; }
const R = (ee, ro, ...ks) => ks.map(k => row(BY[k], ee, ro)).join('');

function pageHtml(id, ee, ro, chPage) {
  switch (id) {
    case 'sabit': return '<div class="p4-cols">' +
      frame('', R(ee, ro, 'seriNo', 'mhkTarih', 'uretimNo', 'versiyon') + R(ee, true, 'telsizTipi', 'donanimTipi', 'bant')) +
      frame('', R(ee, ro, 'sabitMerkez', 'compander', 'tdm')) + '</div>' +
      '<p class="note">Telsiz tipi, donanım tipi ve frekans bandı Test-Ayar alanından (0x58B, 0x58C..0x58D) okunur; P4000 bunları göstermekle yetinir.</p>';
    case 'ozel': return '<div class="p4-cols">' +
      frame('', R(ee, ro, 'progSurum', 'aniNo', 'secNo', 'ackNo', 'cloneKey', 'lp', 'hp')) +
      frame('', R(ee, ro, 'optStart', 'scEnc', 'monitor', 'keypad', 'cloner', 'micType', 'vox', 'marine')) + '</div>' +
      '<p class="note">Numaralar en çok 6 hane (0–9, A–D). Programlayıcı sürüm no 0x009.0x008: telsizi en son programlayan yazılımın sürümü.</p>';
    case 'aktar': return '<div class="p4-cols">' +
      frame('Dil', R(ee, ro, 'dil')) + frame('Meşgul Kanal Kilidi', R(ee, ro, 'mesgul')) + frame('Harici Giriş', R(ee, ro, 'harici')) +
      frame('', R(ee, ro, 'gucTas', 'altMenu', 'acil', 'acilKanal')) + frame('Öncelikli Kanallar', R(ee, ro, 'oncelik1', 'oncelik2')) +
      frame('', R(ee, ro, 'tot', 'ilkTon', 'rekey', 'tarama', 'txGec')) +
      frame('Ton Standardı', R(ee, ro, 'tonStd')) + frame('Seçilebilir çağrı hane sayısı', R(ee, ro, 'hane')) + frame('ANI Tipi', R(ee, ro, 'aniTip')) +
      frame('Kullanıcı Alanı', R(ee, ro, 'invDisp', 'isikKapali', 'uyariYok')) + frame('Grup Tonu', R(ee, ro, 'grupTonu')) + frame('Öncelik', R(ee, ro, 'oncelik')) +
      frame('P4000 ekranında yok', R(ee, ro, 'tonSayisi') + '<div class="p4-row"><span class="p4-l">0x01E..0x02D</span><span class="mono">' +
        Array.from(ee.slice(0x1E, 0x2E), hex2).join(' ') + '</span></div><p class="note">Seçmeli çağrı ton süreleri / kod çözücü ayarları: P4000 yeni kütükte ton standardına göre kendi tablosundan doldurur (0x4AA868), ekranda göstermez.</p>') +
      '</div>';
    case 'kanal': {
      const p = chPage || 0, n0 = p * 10 + 1, n1 = Math.min(99, n0 + 9);
      let h = '<div class="p4-chnav">' + Array.from({ length: 10 }, (_, i) => '<button data-chp="' + i + '"' + (i === p ? ' class="primary"' : '') + '>' + (i * 10 + 1) + '-' + (i === 9 ? 99 : i * 10 + 10) + '</button>').join('') + '</div>';
      h += '<div class="p4-scroll"><table class="p4-ch"><thead><tr><th>Kanal No</th><th>Kanal Adı</th><th>TxFrek.</th><th>RxFrek.</th><th>TxTCS</th><th>RxTCS</th><th>TOT</th><th>HPRF</th><th>ANI</th><th>Tarama</th></tr></thead><tbody>';
      const dis = ro ? ' disabled' : '';
      for (let n = n0; n <= n1; n++) {
        const c = channel(ee, n), at = ' data-ch="' + n + '"';
        h += '<tr' + (c.tx || c.rx ? '' : ' class="p4-empty"') + '><td class="num">' + n + '</td>' +
          '<td><input type="text" class="mono" size="9" maxlength="8"' + at + ' data-f="name" value="' + esc(c.name.replace(/\s+$/, '')) + '"' + dis + '></td>' +
          '<td><input type="text" class="mono" size="9"' + at + ' data-f="tx" value="' + (c.tx ? mhz(c.tx) : '') + '"' + dis + '></td>' +
          '<td><input type="text" class="mono" size="9"' + at + ' data-f="rx" value="' + (c.rx ? mhz(c.rx) : '') + '"' + dis + (c.xshift ? ' title="kristal kaydırma: 1 (P4000 RX frekansından hesaplar)"' : '') + '></td>' +
          '<td>' + tcsSelect(c.txTcs, at + ' data-f="txTcs"', dis) + '</td><td>' + tcsSelect(c.rxTcs, at + ' data-f="rxTcs"', dis) + '</td>' +
          ['tot', 'hprf', 'ani', 'scan'].map(k => '<td class="c"><input type="checkbox"' + at + ' data-f="' + k + '"' + (c[k] ? ' checked' : '') + dis + '></td>').join('') + '</tr>';
      }
      h += '</tbody></table></div><p class="note">Kayıt 0x02E + 13·(n−1). Boş frekans kelimesi 0x3FFF: yalnız TX\'i boş kanal yalnız alır, ikisi de boşsa kanal yoktur. Ad 8 karakter; telsiz yalnız A–Z 0–9 boşluk _ - / gösterir. Kristal kaydırma biti P4000\'deki gibi RX frekansından hesaplanır. Tarama listesi 0x537..0x543.</p>';
      return h;
    }
    case 'ksys': return '<div class="p4-cols">' + frame('', R(ee, ro, 'kSys', 'kSmt', 'kRx', 'kRxT', 'kTx', 'kTxT', 'kHp')) +
      frame('Grup Numaraları', '<div class="p4-grid5">' + F.filter(x => x.p === 'ksys' && x.g === 'grup').map(x => row(x, ee, ro)).join('') + '</div>') + '</div>';
    case 'adata': return '<div class="p4-cols">' + frame('Kimlik Bilgileri', R(ee, ro, 'aSys', 'aFleet', 'aRadio', 'aRx')) + frame('', R(ee, ro, 'aPre')) +
      frame('Gruplar', F.filter(x => x.p === 'adata' && x.g === 'grup').map(x => row(x, ee, ro)).join('')) +
      frame('Durum Mesajları', '<div class="p4-grid3">' + F.filter(x => x.p === 'adata' && x.g === 'durum').map(x => row(x, ee, ro)).join('') + '</div>') + '</div>';
    case 'test': {
      const known = { 0x58B: 'opsiyon (telsiz / donanım tipi)', 0x58C: 'bant tabanı (HalfBaseChan, 25 kHz)', 0x58D: '', 0x58F: 'referans: 0 = 12,8 MHz, aksi 13,0 MHz' };
      let h = '<p class="note">0x578..0x5BC fabrika ayar alanı. P4000 burayı yazmaz; T4000 test yazılımıyla doldurulur. Firmware\'den bilinenler: 0x591..0x59A ilk güç PWM değerleri, 0x59B..0x5A4 ve 0x5A5..0x5AE güç hedefleri.</p>';
      h += '<div class="p4-cols">' + frame('', R(ee, ro, 'versiyon', 'uretimNo') + Object.entries(known).filter(([, s]) => s).map(([a, s]) => '<div class="p4-row"><span class="p4-l">0x' + hex3(+a) + ' ' + esc(s) + '</span><span class="mono">' + hex2(ee[+a]) + (+a === 0x58C ? hex2(ee[0x58D]) + ' = ' + (base(ee) * 0.025).toFixed(3) + ' MHz' : '') + '</span></div>').join('')) + '</div>';
      let hx = '';
      for (let r = 0x570; r < 0x5C0; r += 16) { hx += hex3(r) + '  '; for (let i = 0; i < 16; i++) hx += (r + i >= 0x578 && r + i <= 0x5BE ? hex2(ee[r + i]) : '  ') + ' '; hx += '\n'; }
      return h + '<pre class="mono p4-hex">' + hx + '</pre>';
    }
    case 'aprs': {
      const a = aprsGet(ee), dis = ro ? ' disabled' : '';
      const inp = (k, v, size, extra) => '<input type="text" class="mono" size="' + size + '" data-aprs="' + k + '" value="' + esc(v) + '"' + (extra || '') + dis + '>';
      const rw = (l, h, w) => '<div class="p4-row"' + (w ? ' title="' + esc(w) + '"' : '') + '><span class="p4-l">' + esc(l) + '</span>' + h + '</div>';
      const sim = a.tablo + a.kod, de = x => { const v = aprsDerece(x); return isNaN(v) ? '' : ' <span style="color:var(--muted)">' + v.toFixed(5) + '°</span>'; };
      const simSel = '<select data-aprs="simge"' + dis + '>' + (APRS_SIMGE.some(s => s[0] === sim) ? '' : '<option value="" selected>' + esc(sim) + '</option>') +
        APRS_SIMGE.map(([k, l]) => '<option value="' + esc(k) + '"' + (k === sim ? ' selected' : '') + '>' + esc(l + '  ' + k) + '</option>').join('') + '</select>';
      let h = '<p class="note">' + (a.gecerli ? 'EEPROM\'da APRS bölümü var (0x6E0..0x731, sağlama tutuyor).' :
        'EEPROM\'da APRS bölümü yok: firmware aşağıdaki varsayılanlarla gönderir.' + (ro ? '' : ' Bir alanı değiştirince bölüm yazılır.')) +
        ' Bu bölüm yalnız özel firmware\'in (YA4000S_FREKANS) APRS menüsü içindir; P4000 ve orijinal firmware bu alanı kullanmaz.</p>';
      h += '<div class="p4-cols">' + frame('İstasyon',
        rw('Çağrı işareti', inp('cagri', a.cagri, 11, ' maxlength="9"'), 'AX.25: en çok 6 harf / rakam, -SSID 0..15. 0x6E4..0x6ED') +
        rw('Simge', simSel + ' ' + inp('tablo', a.tablo, 2, ' maxlength="1" title="tablo"') + inp('kod', a.kod, 2, ' maxlength="1" title="kod"'), '0x6EE tablo, 0x6EF kod') +
        rw('Yorum', inp('yorum', a.yorum, 44, ' maxlength="43"'), 'en çok 43 bayt; her karakter gider (Türkçe harfler UTF-8, 2 bayt). 0x704..0x72E')) +
        frame('Konum', rw('Enlem', inp('enlem', a.enlem, 10) + de(a.enlem), '3955.52N, 39 55.52 N ya da 39.92533. 0x6F0') +
          rw('Boylam', inp('boylam', a.boylam, 10) + de(a.boylam), '03250.22E, 32 50.22 E ya da 32.837. 0x6F8')) +
        frame('Otomatik gönderim', rw('Aralık (s)', inp('aralik', a.aralik || '', 6), '0 ya da boş: kapalı; en az 10 s. APRS menüsündeyken PTT\'siz gönderir. 0x702') ) + '</div>';
      h += '<div class="p4-row"><span class="p4-l">Gidecek paket</span><span class="mono">' + esc(aprsPaket(ee)) + '</span></div>';
      if (!ro) h += '<div class="p4-row"><button data-aprs-cmd="varsayilan">Varsayılanları yaz</button>' + (a.gecerli ? '<button data-aprs-cmd="sil">Bölümü sil (firmware varsayılanı kullanır)</button>' : '') + '</div>';
      return h;
    }
  }
  return '';
}

function render(el, ee, opt) {
  opt = opt || {};
  const st = el._p4 = el._p4 || { page: opt.page || 'sabit', chPage: 0 };
  const ro = !opt.editable;
  const sums = checkSums(ee);
  let h = '<div class="p4-nav">' + PAGES.map(p => '<button data-pg="' + p.id + '"' + (p.id === st.page ? ' class="active"' : '') + '>' + esc(p.menu || p.title) + '</button>').join('') + '</div>';
  const pg = PAGES.find(p => p.id === st.page);
  h += '<div class="p4-page"><div class="p4-title">' + esc(pg.title) + '</div>' + pageHtml(st.page, ee, ro, st.chPage) + '</div>';
  h += '<div class="p4-sums">Sağlama: ' + sums.map(s => '<span class="pill ' + (s.ok ? 'ok' : 'bad') + '" title="0x' + hex3(s.a) + '..0x' + hex3(s.a + s.n - 1) + ' → 0x' + hex3(s.a + s.n) + '">' + esc(s.name) + '</span>').join(' ') +
    ' <span class="pill' + (aprsGecerli(ee) ? ' ok' : '') + '" title="0x6E0..0x72F → 0x730 (özel firmware)">APRS' + (aprsGecerli(ee) ? '' : ' (yok)') + '</span></div>';
  el.innerHTML = h;
  el.querySelectorAll('[data-pg]').forEach(b => b.onclick = () => { st.page = b.dataset.pg; render(el, ee, opt); });
  el.querySelectorAll('[data-chp]').forEach(b => b.onclick = () => { st.chPage = +b.dataset.chp; render(el, ee, opt); });
  if (ro) return;
  const done = msg => { if (opt.onChange) opt.onChange(msg); render(el, ee, opt); };
  el.querySelectorAll('[data-k]').forEach(inp => inp.onchange = () => {
    const x = BY[inp.dataset.k];
    try {
      let v;
      if (x.t === 'bool') v = inp.checked ? 1 : 0;
      else if (x.t === 'enum') v = +inp.value;
      else if (x.t === 'tcs') { setBits(ee, x.d, +inp.value); return done(); }
      else if (x.t === 'freq') v = inp.value.trim() ? Math.round(parseFloat(inp.value.replace(',', '.')) * 1e6) : 0;
      else if (x.t === 'date') { const m = /^(\d{1,2})\.(\d{1,2})\.(\d{4})$/.exec(inp.value.trim()); if (!m) throw new Error('tarih gg.aa.yyyy'); v = { gun: +m[1], ay: +m[2], yil: +m[3] }; if (v.yil < 1994 || v.yil > 2121) throw new Error('yıl 1994–2121'); }
      else if (x.t === 'int') { v = parseFloat(inp.value.replace(',', '.')); if (isNaN(v) || v < (x.min || 0) || v > x.max) throw new Error('Değer ' + (x.min || 0) + ' ile ' + x.max + ' arasında olmalıdır.'); }
      else v = inp.value;
      if (x.t === 'int' && x.scale && Math.abs(v / x.scale - Math.round(v / x.scale)) > 1e-9) throw new Error('Basamak = ' + x.scale);
      set(ee, x, v);
      done();
    } catch (e) { alert(x.l + ': ' + e.message); render(el, ee, opt); }
  });
  el.querySelectorAll('[data-aprs]').forEach(inp => inp.onchange = () => {
    const k = inp.dataset.aprs, o = {};
    try {
      if (k === 'simge') { if (!inp.value) return; o.tablo = inp.value[0]; o.kod = inp.value[1]; }
      else o[k] = inp.value;
      aprsSet(ee, o);
      done('APRS');
    } catch (e) { alert('APRS: ' + e.message); render(el, ee, opt); }
  });
  el.querySelectorAll('[data-aprs-cmd]').forEach(b => b.onclick = () => {
    if (b.dataset.aprsCmd === 'varsayilan') aprsSet(ee, APRS_VARSAYILAN);
    else ee.fill(0xFF, APRS0, APRS0 + APRS_UZ + 2);
    done('APRS');
  });
  el.querySelectorAll('[data-ch]').forEach(inp => inp.onchange = () => {
    const n = +inp.dataset.ch, k = inp.dataset.f, c = channel(ee, n);
    try {
      if (k === 'name') c.name = inp.value;
      else if (k === 'tx' || k === 'rx') {
        const v = inp.value.trim() ? Math.round(parseFloat(inp.value.replace(',', '.')) * 1e6) : 0;
        if (v && !c.tx && !c.rx) c.tx = c.rx = v;                               // yeni kanal: iki frekans da aynı
        else c[k] = v;                                                           // boş = yalnız alma / yalnız gönderme
      } else if (k === 'txTcs' || k === 'rxTcs') c[k] = +inp.value;
      else c[k] = inp.checked ? 1 : 0;
      setChannel(ee, n, c);
      done();
    } catch (e) { alert('Kanal ' + n + ': ' + e.message); render(el, ee, opt); }
  });
}

const CSS = '.p4-nav{display:flex;flex-wrap:wrap;gap:4px;margin-bottom:10px}.p4-nav button.active{background:var(--accent);color:var(--accent-ink);border-color:var(--accent)}' +
  '.p4-title{font-weight:650;margin:0 0 8px;padding-bottom:4px;border-bottom:1px solid var(--line)}' +
  '.p4-cols{display:flex;flex-wrap:wrap;gap:10px;align-items:flex-start}.p4-fs{border:1px solid var(--line);border-radius:8px;padding:6px 10px 8px;margin:0;min-width:0}' +
  '.p4-fs legend{font-size:12px;color:var(--muted);padding:0 4px}.p4-row{display:flex;align-items:center;gap:8px;margin:3px 0;font-size:13px}.p4-l{min-width:150px;color:var(--muted)}' +
  '.p4-radio{display:flex;flex-direction:column;gap:2px}.p4-radio label,.p4-cb{white-space:nowrap}.p4-grid5{display:grid;grid-template-columns:repeat(5,auto);gap:0 14px}.p4-grid5 .p4-l,.p4-grid3 .p4-l{min-width:0}' +
  '.p4-grid3{display:grid;grid-template-columns:repeat(3,auto);gap:0 14px}.p4-chnav{display:flex;flex-wrap:wrap;gap:3px;margin-bottom:8px}.p4-chnav button{padding:3px 7px;font-size:12px}' +
  '.p4-scroll{overflow-x:auto}.p4-ch{border-collapse:collapse;font-size:13px}.p4-ch th{font-weight:600;color:var(--muted);text-align:left;padding:2px 6px;border-bottom:1px solid var(--line);white-space:nowrap}' +
  '.p4-ch td{padding:2px 6px;border-bottom:1px solid var(--line)}.p4-ch td.c{text-align:center}.p4-ch tr.p4-empty td input[type=text]{opacity:.6}.p4-ch select{max-width:96px}' +
  '.p4-sums{margin-top:10px;font-size:12px;color:var(--muted)}.p4-sums .pill{border:1px solid currentColor;border-radius:9px;padding:0 6px;font-size:11px}.p4-hex{white-space:pre;overflow-x:auto;margin:8px 0 0}';
function injectCss(doc) {
  if (doc.getElementById('p4css')) return;
  const s = doc.createElement('style'); s.id = 'p4css'; s.textContent = CSS; doc.head.appendChild(s);
}

const api = { FIELDS, PAGES, SUMS, TCS_HZ, get, set, getBits, setBits, channel, setChannel, xshift, base, checkSums, fixSums,
  toP40, fromP40, p40Base, parseIni, dec1254, render, injectCss, telsizTipi, donanimTipi, bandText, where,
  APRS0, APRS_UZ, APRS_VARSAYILAN, aprsGet, aprsSet, aprsGecerli, aprsFix, aprsKonum, aprsPaket, utf8 };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
root.P4000 = api;
})(typeof globalThis !== 'undefined' ? globalThis : this);
