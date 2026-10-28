/* jsc -m test.mjs  ya da  node test.mjs  (sunucu/sayac içinden) - çekirdek ve worker'ın yanıtları, sahte KV ile */
const out = typeof print === 'function' ? print : console.log;
import { maskele, olayEkle, bosDurum, gorunum, KAYIT_EN_COK } from './cekirdek.js';
let fails = 0;
const check = (ad, ok, info) => { out((ok ? 'ok   ' : 'FAIL ') + ad + (info ? '  (' + info + ')' : '')); if (!ok) fails++; };

check('IPv4 maskelenir', maskele('85.105.12.201') === '85.105.12.x', maskele('85.105.12.201'));
check('IPv6 maskelenir', maskele('2a02:4e0:2d2a:1:abcd::5') === '2a02:4e0:2d2a:x', maskele('2a02:4e0:2d2a:1:abcd::5'));
check('bozuk IP ?', maskele('<script>') === '?' && maskele('') === '?');
let d = bosDurum();
d = olayEkle(d, 'ee_yedek', '1.2.3.4', 1000, 'AS V2-3');
check('sayaç artar, kayıtta tam IP yok', d.sayac.ee_yedek === 1 && d.kayit[0].ip === '1.2.3.x' && !JSON.stringify(d).includes('1.2.3.4') && d.kayit[0].ek === 'AS V2-3');
check('5 s içinde aynı olay sayılmaz', olayEkle(d, 'ee_yedek', '1.2.3.9', 3000) === null);
d = olayEkle(d, 'ee_yedek', '1.2.3.9', 7000);
check('5 s sonra sayılır', d && d.sayac.ee_yedek === 2);
check('başka adres hemen sayılır', olayEkle(d, 'ee_yedek', '5.6.7.8', 7001).sayac.ee_yedek === 3);
check('bağlantı sayaç değil, kayıtta', (x => x.sayac.ee_yedek === 2 && x.kayit[0].tur === 'baglanti')(olayEkle(d, 'baglanti', '9.9.9.9', 8000)));
check('geçersiz tür reddedilir', olayEkle(d, 'sil', '9.9.9.9', 9000) === null && olayEkle(d, '__proto__', '9.9.9.9', 9000) === null);
check('ek temizlenir', olayEkle(d, 'flash_yaz', '9.9.9.9', 9000, '<b>x</b>\u0000' + 'y'.repeat(50)).kayit[0].ek.length <= 32);
{ const x = olayEkle(olayEkle(d, 'eeprom_acilis', '9.9.9.9', 8000), 'eeprom_acilis', '9.9.9.9', 8001);
  check('sayfa açılışı her seferinde sayılır, kayda yazılmaz', x.sayac.eeprom_acilis === 2 && x.kayit.length === d.kayit.length); }
let e = bosDurum(); for (let i = 0; i < KAYIT_EN_COK + 20; i++) e = olayEkle(e, 'flash_yedek', '10.' + (i >> 8) + '.' + (i & 255) + '.1', i * 10);
check('kayıt sınırlı, en yeni başta', e.kayit.length === KAYIT_EN_COK && e.sayac.flash_yedek === KAYIT_EN_COK + 20 && e.kayit[0].t > e.kayit[1].t);
check('görünüm n satır', gorunum(e, 5).kayit.length === 5);

/* worker: sahte Response / KV / istek */
globalThis.Response = class { constructor(b, i) { this.body = b; this.status = i.status; this.headers = i.headers; } };
const { default: worker } = await import('./worker.js');
const kv = { v: null, async get() { return this.v ? JSON.parse(this.v) : null; }, async put(k, v) { this.v = v; } };
const istek = (method, url, origin, ip, govde) => ({ method, url, headers: { get: h => ({ origin, 'cf-connecting-ip': ip })[h] }, json: async () => govde });
const gh = 'https://barisdinc.github.io';
let r = await worker.fetch(istek('POST', 'https://x.workers.dev/olay', gh, '85.105.12.201', { tur: 'flash_yaz', ek: 'AT V0-15' }), { SAYAC: kv });
check('POST /olay sayar', r.status === 200 && JSON.parse(r.body).sayac.flash_yaz === 1 && r.headers['access-control-allow-origin'] === gh, r.body);
r = await worker.fetch(istek('POST', 'https://x.workers.dev/olay', 'https://kotu.example', '1.1.1.1', { tur: 'flash_yaz' }), { SAYAC: kv });
check('başka siteden POST reddedilir', r.status === 403);
r = await worker.fetch(istek('GET', 'https://x.workers.dev/durum?n=10', gh, '1.1.1.1'), { SAYAC: kv });
const g = JSON.parse(r.body);
check('GET /durum', r.status === 200 && g.sayac.flash_yaz === 1 && g.kayit[0].ip === '85.105.12.x' && g.kayit[0].ek === 'AT V0-15', r.body);
check('KV\'de tam IP yok', !kv.v.includes('85.105.12.201'));
r = await worker.fetch(istek('OPTIONS', 'https://x.workers.dev/olay', gh), { SAYAC: kv });
check('OPTIONS 204 + CORS', r.status === 204 && r.headers['access-control-allow-methods'].includes('POST'));
r = await worker.fetch(istek('GET', 'https://x.workers.dev/yok', gh), { SAYAC: kv });
check('bilinmeyen yol 404', r.status === 404);
out(fails ? fails + ' test başarısız' : 'tüm sayaç testleri geçti');
if (fails) throw new Error(fails + ' test başarısız');
