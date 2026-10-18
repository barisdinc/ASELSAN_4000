/* afsk.js - 1200 baud AFSK (Bell 202) / AX.25 çözücü: simülatörün TA1 örnek kaydından
 * (board.audio.ta1 biçiminde [zaman, değer, zaman, değer, ...]; zaman işlemci çevrimi)
 * APRS paketlerini çıkarır. Firmware'den bağımsız yazıldı: ozel_firmware/ornekler/aprs.inc'in
 * ürettiği sesi denetlemek için.
 *
 *   AFSK.decode(log, fclk) -> [{ text, bytes, fcsOk }]
 *
 * Yöntem: bit süresi (fclk/1200 çevrim) pencerelerinde 1200 ve 2200 Hz enerjisi (sinüs ve
 * kosinüsle çarpım, faz bağımsız) karşılaştırılır; bit saati 16 farklı başlangıç denenip
 * en belirgin ayrımı veren seçilerek bulunur. Sonra NRZI çözme (aynı ton = 1), 0x7E bayrakları,
 * bit doldurmanın atılması, CRC-16-CCITT denetimi ve AX.25 adreslerinin metne çevrilmesi. */
(function (root) {
'use strict';

function tones(log, fclk, offset) {
  const bitLen = fclk / 1200, t0 = log[0] + offset, tEnd = log[log.length - 2];
  const out = [], score = [];
  let i = 0;
  for (let ws = t0; ws + bitLen <= tEnd; ws += bitLen) {
    let m1s = 0, m1c = 0, m2s = 0, m2c = 0, n = 0, sum = 0;
    while (i < log.length && log[i] < ws) i += 2;
    let j = i;
    const vals = [];
    for (; j < log.length && log[j] < ws + bitLen; j += 2) { vals.push(log[j], log[j + 1]); sum += log[j + 1]; n++; }
    if (!n) { out.push(null); score.push(0); continue; }
    const mean = sum / n;
    for (let k = 0; k < vals.length; k += 2) {
      const t = vals[k] / fclk, v = vals[k + 1] - mean;
      m1s += v * Math.sin(2 * Math.PI * 1200 * t); m1c += v * Math.cos(2 * Math.PI * 1200 * t);
      m2s += v * Math.sin(2 * Math.PI * 2200 * t); m2c += v * Math.cos(2 * Math.PI * 2200 * t);
    }
    const e1 = m1s * m1s + m1c * m1c, e2 = m2s * m2s + m2c * m2c;
    out.push(e1 > e2 ? 1 : 0);
    score.push(Math.abs(e1 - e2) / (e1 + e2 + 1e-9));
  }
  return { bits: out, quality: score.reduce((a, b) => a + b, 0) / (score.length || 1) };
}

function crc16(bytes) {
  let crc = 0xFFFF;
  for (const b of bytes) { crc ^= b; for (let k = 0; k < 8; k++) crc = crc & 1 ? (crc >>> 1) ^ 0x8408 : crc >>> 1; }
  return crc ^ 0xFFFF;
}

function addr(bytes, o) {
  let s = '';
  for (let k = 0; k < 6; k++) s += String.fromCharCode(bytes[o + k] >> 1);
  s = s.trim();
  const ssid = bytes[o + 6] >> 1 & 15;
  return ssid ? s + '-' + ssid : s;
}

function frameText(b) {
  let o = 0; const a = [];
  while (o + 7 <= b.length) { a.push(addr(b, o)); o += 7; if (b[o - 1] & 1) break; }
  if (a.length < 2 || o + 2 > b.length) return null;
  const info = b.slice(o + 2, b.length - 2).map(c => String.fromCharCode(c)).join('');
  return a[1] + '>' + a[0] + (a.length > 2 ? ',' + a.slice(2).join(',') : '') + ':' + info;
}

function frames(tbits) {
  // NRZI: aynı ton = 1, değişim = 0
  const raw = [];
  for (let k = 1; k < tbits.length; k++) raw.push(tbits[k] === null || tbits[k - 1] === null ? null : (tbits[k] === tbits[k - 1] ? 1 : 0));
  const out = [];
  let ones = 0, inFrame = false, cur = [], byte = 0, nb = 0, last8 = 0;
  for (const bit of raw) {
    if (bit === null) { inFrame = false; continue; }
    last8 = ((last8 >> 1) | (bit << 7)) & 0xFF;
    if (last8 === 0x7E) {                     // bayrak: çerçeve sonu / başı
      if (inFrame && cur.length >= 18) {
        const body = cur.slice(0, cur.length);
        const fcs = body[body.length - 2] | body[body.length - 1] << 8;
        out.push({ bytes: body, fcsOk: crc16(body.slice(0, -2)) === fcs, text: frameText(body) });
      }
      inFrame = true; cur = []; byte = 0; nb = 0; ones = 0;
      continue;
    }
    if (!inFrame) continue;
    if (bit) { ones++; if (ones > 6) { inFrame = false; continue; } }
    else { if (ones === 5) { ones = 0; continue; } ones = 0; }   // doldurma biti
    byte |= bit << nb; nb++;
    if (nb === 8) { cur.push(byte); byte = 0; nb = 0; }
  }
  // bayrağın ilk 7 biti çerçeveye girmiş olabilir: bayrak tespitinde son bayt yarımdır, atılır
  return out;
}

function decode(log, fclk) {
  if (log.length < 64) return [];
  let best = null;
  for (let k = 0; k < 16; k++) {
    const r = tones(log, fclk, k * fclk / 1200 / 16);
    if (!best || r.quality > best.quality) best = r;
  }
  return frames(best.bits);
}

const api = { decode, crc16 };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
root.AFSK = api;
})(typeof globalThis !== 'undefined' ? globalThis : this);
