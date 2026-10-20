/* worker.js - Cloudflare Worker: ASELSAN 4000 programlama sayfasının (disasm/sim/yukle.html) sayaçları
 * ve bağlantı kaydı. Depo: KV ("SAYAC" bağı, tek anahtar "durum").
 *   GET  /durum?n=100   -> { sayac: {ee_yedek, flash_yedek, ee_yaz, flash_yaz}, kayit: [{t, ip, tur, ek?}] }
 *   POST /olay          <- { tur: 'ee_yedek' | 'flash_yedek' | 'ee_yaz' | 'flash_yaz' | 'baglanti', ek?: '...' }
 *                       -> { sayac }
 * IP isteğin kendisinden (CF-Connecting-IP) alınır ve yalnız maskeli hâli saklanır.
 * KV son yazanın kazandığı bir depo: aynı anda gelen iki olaydan biri kaybolabilir (bu trafikte önemsiz). */
import { olayEkle, gorunum, bosDurum } from './cekirdek.js';

const IZINLI = ['https://barisdinc.github.io', 'http://localhost', 'http://127.0.0.1', 'null'];

function izinli(origin) {
  return IZINLI.some(o => origin === o || (o.startsWith('http://') && origin && origin.startsWith(o + ':')));
}

function yanit(govde, durum, origin) {
  return new Response(govde === null ? null : JSON.stringify(govde), {
    status: durum,
    headers: {
      'content-type': 'application/json; charset=utf-8',
      'access-control-allow-origin': izinli(origin) ? origin : 'https://barisdinc.github.io',
      'access-control-allow-methods': 'GET, POST, OPTIONS',
      'access-control-allow-headers': 'content-type',
      'cache-control': 'no-store',
      'vary': 'origin',
    },
  });
}

async function oku(env) {
  const d = await env.SAYAC.get('durum', 'json');
  return d && d.sayac && Array.isArray(d.kayit) ? d : bosDurum();
}

export default {
  async fetch(req, env) {
    const origin = req.headers.get('origin') || '';
    const m = /^https?:\/\/[^/]+(\/[^?#]*)(?:\?([^#]*))?/.exec(req.url) || [];
    const yol = m[1] || '/', sorgu = m[2] || '';
    if (req.method === 'OPTIONS') return yanit(null, 204, origin);
    if (req.method === 'GET' && yol === '/durum') {
      const n = +((/(?:^|&)n=(\d+)/.exec(sorgu) || [])[1] || 100);
      return yanit(gorunum(await oku(env), n), 200, origin);
    }
    if (req.method === 'POST' && yol === '/olay') {
      if (!izinli(origin)) return yanit({ hata: 'izin yok' }, 403, origin);
      let g; try { g = await req.json(); } catch (e) { return yanit({ hata: 'json' }, 400, origin); }
      const d = await oku(env);
      const yeni = olayEkle(d, g && g.tur, req.headers.get('cf-connecting-ip'), Date.now(), g && g.ek);
      if (yeni) await env.SAYAC.put('durum', JSON.stringify(yeni));
      return yanit({ sayac: (yeni || d).sayac, sayildi: !!yeni }, 200, origin);
    }
    return yanit({ hata: 'yok' }, 404, origin);
  },
};
