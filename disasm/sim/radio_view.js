/* radio_view.js - turns raw board state into what the radio shows.
 *
 * LCD: the original firmware (F_00828C) sends 0x70 E4 C8 F0 00 and then
 * 10 words, high byte first:
 *   word 0  channel number, two 7-segment digits (ROM 0x2D62 / 0x2D00)
 *   word 1  symbol bits
 *   word 2..9  alphanumeric characters (font ROM 0x2D0A A-Z, 0x2D3E 0-9),
 *              text buffer RAM 0x0259 (word 2) .. 0x0252 (word 9)
 * Characters are decoded by reverse lookup in the font of the loaded ROM,
 * so the result does not depend on the glass layout.
 */
(function (root) {
'use strict';

/* Two glass variants exist in the original (RAM 0x023C bit 8):
 *   A (bit set):   letters 0x2D0A, digits 0x2D3E, channel 0x2D62, words in reverse order
 *   B (bit clear): letters 0x2C9C, digits 0x2CD0, channel 0x2D00, words in natural order */
const PUNCT_A = { '*': 0x2CE4, '<': 0x2D52, '_': 0x2D54, '-': 0x2CEA, ':': 0x2CEC, '/': 0x2CEE, '+': 0x2CF0,
                  '^': 0x2D56, 'v': 0x2D58, 'j': 0x2D5A, 'd': 0x2D5C, 'm': 0x2D5E, 'e': 0x2D60 };
const PUNCT_B = { '*': 0x2CE4, '<': 0x2CE6, '_': 0x2CE8, '-': 0x2CEA, ':': 0x2CEC, '/': 0x2CEE, '+': 0x2CF0,
                  '^': 0x2CF4, 'v': 0x2CF6, 'j': 0x2CF8, 'd': 0x2CFA, 'm': 0x2CFC, 'e': 0x2CFE };
function buildFont(rom) {
  const w = a => rom[a] | rom[a + 1] << 8;
  const mk = (let0, dig0, punct, ch7, reverse) => {
    const map = new Map([[0, ' ']]);
    for (let i = 0; i < 26; i++) { const v = w(let0 + 2 * i); if (!map.has(v)) map.set(v, String.fromCharCode(65 + i)); }
    for (let i = 0; i < 10; i++) { const v = w(dig0 + 2 * i); if (!map.has(v)) map.set(v, String(i)); }
    for (const c in punct) { const v = w(punct[c]); if (!map.has(v)) map.set(v, c); }
    const d7 = new Map(); for (let i = 0; i < 10; i++) d7.set(rom[ch7 + i], String(i));
    return { map, d7, reverse };
  };
  return { variants: [mk(0x2D0A, 0x2D3E, PUNCT_A, 0x2D62, true), mk(0x2C9C, 0x2CD0, PUNCT_B, 0x2D00, false)],
           ok: w(0x2D0A) !== 0xFFFF };
}

/* 14-segment drawing: bit -> segment name, inferred from the font
 * (bit 8 a, 10/9 b c, 3 d, 1/2 e f, 13 g1, 6 g2, 15/4 i l, rest diagonals) */
const SEG14 = { 8: 'a', 10: 'b', 9: 'c', 3: 'd', 2: 'e', 1: 'f', 13: 'g1', 6: 'g2', 15: 'i', 4: 'l',
                12: 'h', 14: 'k', 5: 'j', 7: 'm', 0: 'x0', 11: 'x11' };

function decodeWith(words, v) {
  let text = '', bad = 0;
  for (let i = 0; i < 8; i++) {
    const x = words[v.reverse ? 9 - i : 2 + i];
    let c = v.map.get(x);
    if (c === undefined) c = v.map.get(x & ~0x0811);   // ignore decimal point / extra bits
    if (c === undefined) { c = '?'; bad++; }
    text += c;
  }
  const cw = words[0], dig = b => v.d7.get(b) !== undefined ? v.d7.get(b) : (b ? (bad++, '?') : ' ');
  return { text, chan: v.reverse ? dig(cw >> 8) + dig(cw & 0xFF) : dig(cw & 0xFF) + dig(cw >> 8), bad };
}
function decodeLcd(board, font) {
  const words = board.lcdWords();
  let best = null;
  for (const v of font.variants) { const r = decodeWith(words, v); if (!best || r.bad < best.bad) best = r; }
  return { text: best.text, chan: best.chan, symbols: words[1], words, on: board.lcd.enabled };
}

/* read the firmware's own text buffer (original firmware only) */
function textBuffer(board) {
  let s = '';
  for (let a = 0x252; a <= 0x259; a++) { const c = board.ram[a]; s += c >= 32 && c < 127 ? String.fromCharCode(c) : '.'; }
  return s;
}

/* simple tone analysis of a (time, value) sample log */
function dominantFreq(log, fclk, fromT) {
  const ts = [], vs = [];
  for (let i = 0; i < log.length; i += 2) if (log[i] >= fromT) { ts.push(log[i]); vs.push(log[i + 1]); }
  if (vs.length < 16) return null;
  const mean = vs.reduce((a, b) => a + b, 0) / vs.length;
  let cross = 0, first = -1, last = -1;
  for (let i = 1; i < vs.length; i++) if (vs[i - 1] < mean && vs[i] >= mean) { if (first < 0) first = ts[i]; last = ts[i]; cross++; }
  if (cross < 3) return null;
  return (cross - 1) * fclk / (last - first);
}

function goertzel(log, fclk, fromT, freqs) {
  const ts = [], vs = [];
  for (let i = 0; i < log.length; i += 2) if (log[i] >= fromT) { ts.push(log[i]); vs.push(log[i + 1]); }
  if (vs.length < 64) return null;
  const mean = vs.reduce((a, b) => a + b, 0) / vs.length;
  return freqs.map(f => {
    let re = 0, im = 0;
    for (let i = 0; i < vs.length; i++) { const ph = 2 * Math.PI * f * ts[i] / fclk; re += (vs[i] - mean) * Math.cos(ph); im += (vs[i] - mean) * Math.sin(ph); }
    return Math.hypot(re, im) / vs.length;
  });
}

const DTMF_LO = [697, 770, 852, 941], DTMF_HI = [1209, 1336, 1477, 1633];
const DTMF_KEYS = ['123A', '456B', '789C', '*0#D'];
/* Select-5 tone sets, digits 0-9 A-E (E = repeat) */
const SELCALL = {
  CCIR:  { ms: 100, f: [1981, 1124, 1197, 1275, 1358, 1446, 1540, 1640, 1747, 1860, 2400, 930, 2247, 991, 2110] },
  ZVEI1: { ms: 70,  f: [2400, 1060, 1160, 1270, 1400, 1530, 1670, 1830, 2000, 2200, 2800, 810, 970, 885, 2600] },
  EEA:   { ms: 40,  f: [1981, 1124, 1197, 1275, 1358, 1446, 1540, 1640, 1747, 1860, 1055, 930, 2247, 991, 2110] },
};
const SEL_DIGITS = '0123456789ABCDE';

function analyseTx(board, fclk, windowCycles) {
  const from = board.now - windowCycles;
  const out = {};
  const ta1 = board.audio.ta1;
  if (ta1.length && ta1[ta1.length - 2] >= from) {
    const lo = goertzel(ta1, fclk, from, DTMF_LO), hi = goertzel(ta1, fclk, from, DTMF_HI);
    const f = dominantFreq(ta1, fclk, from);
    if (lo && hi) {
      const il = lo.indexOf(Math.max(...lo)), ih = hi.indexOf(Math.max(...hi));
      const tot = lo.reduce((a, b) => a + b) + hi.reduce((a, b) => a + b);
      if (lo[il] > 0.25 * tot && hi[ih] > 0.25 * tot && lo[il] > 0.3 * hi[ih] && hi[ih] > 0.3 * lo[il])
        out.dtmf = DTMF_KEYS[il][ih];
    }
    if (f) {
      out.toneHz = f;
      for (const std in SELCALL) {
        const i = SELCALL[std].f.findIndex(x => Math.abs(x - f) < x * 0.012);
        if (i >= 0) (out.selcall = out.selcall || []).push(std + ':' + SEL_DIGITS[i]);
      }
    }
  }
  const ta2 = board.audio.ta2;
  if (ta2.length && ta2[ta2.length - 2] >= board.now - fclk * 0.3) {
    const f = dominantFreq(ta2, fclk, board.now - fclk * 0.3);
    if (f) out.ctcssHz = f;
  }
  return out;
}

/* CTCSS tone numbers as used by the firmware (ROM tables 0x22EF / 0x2353):
 * 1..38 the classic 38 tones, 39..50 the later additions */
const CTCSS = [67.0, 71.9, 74.4, 77.0, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100.0, 103.5, 107.2, 110.9, 114.8,
  118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5,
  210.7, 218.1, 225.7, 233.6, 241.8, 250.3, 69.3, 159.8, 165.5, 171.3, 177.3, 183.5, 189.9, 196.6, 199.5, 206.5, 229.1, 254.1];

const api = { buildFont, decodeLcd, textBuffer, analyseTx, SELCALL, SEL_DIGITS, CTCSS, DTMF_LO, DTMF_HI, DTMF_KEYS, SEG14 };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
else root.RadioView = api;
})(typeof window !== 'undefined' ? window : globalThis);
