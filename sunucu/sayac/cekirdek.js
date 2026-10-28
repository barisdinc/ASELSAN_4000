/* cekirdek.js - programlama sayfasının sayaçları ve bağlantı kaydı (saf fonksiyonlar; worker.js kullanır,
 * test.mjs dener). Tam IP hiçbir zaman saklanmaz: yalnız maskeli hâli (85.105.12.x / 2a02:4e0:2d2a:x). */

export const SAYACLAR = ['ee_yedek', 'flash_yedek', 'ee_yaz', 'flash_yaz', 'eeprom_acilis'];
export const KAYITSIZ = ['eeprom_acilis'];   // sayfa açılışı (eeprom.html): yalnız sayılır, kayda yazılmaz (kaydı doldururdu)
export const TURLER = [...SAYACLAR, 'baglanti'];
export const KAYIT_EN_COK = 300;      // kayıttaki en çok satır (eskiler düşer)
export const TEKRAR_MS = 5000;        // aynı adresten aynı olay bu süre içinde yeniden gelirse sayılmaz

export function bosDurum() {
  return { sayac: Object.fromEntries(SAYACLAR.map(k => [k, 0])), kayit: [] };
}

export function maskele(ip) {
  ip = String(ip || '').trim();
  const v4 = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.\d{1,3}$/.exec(ip);
  if (v4) return v4[1] + '.' + v4[2] + '.' + v4[3] + '.x';
  if (/^[0-9a-f:]+$/i.test(ip) && ip.includes(':')) {
    const g = ip.toLowerCase().split('::')[0].split(':').filter(Boolean).slice(0, 3);
    return (g.length ? g.join(':') : '0') + ':x';
  }
  return '?';
}

/* ek bilgi (ör. telsizin yazılım sürümü): kısa, yalnız basılabilir karakterler */
export function temizle(s) {
  return String(s || '').replace(/[^\x20-\x7EÀ-ſ]/g, '').slice(0, 32);
}

/* olayı işle: durum değişirse yeni durum, değilse null (geçersiz tür ya da kısa sürede tekrar) */
export function olayEkle(durum, tur, ip, zaman, ek) {
  if (!TURLER.includes(tur)) return null;
  if (KAYITSIZ.includes(tur)) return { sayac: Object.assign({}, bosDurum().sayac, durum.sayac, { [tur]: (durum.sayac[tur] || 0) + 1 }), kayit: durum.kayit };
  const adres = maskele(ip);
  const son = durum.kayit.find(k => k.ip === adres && k.tur === tur);
  if (son && zaman - son.t < TEKRAR_MS) return null;
  const yeni = { sayac: Object.assign({}, bosDurum().sayac, durum.sayac), kayit: durum.kayit.slice() };
  if (SAYACLAR.includes(tur)) yeni.sayac[tur]++;
  const satir = { t: zaman, ip: adres, tur };
  const e = temizle(ek); if (e) satir.ek = e;
  yeni.kayit.unshift(satir);
  if (yeni.kayit.length > KAYIT_EN_COK) yeni.kayit.length = KAYIT_EN_COK;
  return yeni;
}

/* dışarı verilen görünüm: sayaçlar + son n satır */
export function gorunum(durum, n) {
  return { sayac: durum.sayac, kayit: durum.kayit.slice(0, Math.max(0, Math.min(n || 100, KAYIT_EN_COK))) };
}
