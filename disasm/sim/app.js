/* app.js - user interface of the ASELSAN 4000 simulator */
(function () {
'use strict';
const { Board, parseHex, FCLK } = window.Board;
const V = window.RadioView;
const { disasm, hex } = window.M7700;
const $ = id => document.getElementById(id);
const store = {
  get(k) { try { return localStorage.getItem(k); } catch (e) { return null; } },
  set(k, v) { try { localStorage.setItem(k, v); } catch (e) { /* storage blocked */ } },
};

/* ------------------------------------------------------------ state */
let board = null, font = null, running = true, speed = 1, lastT = 0, fwName = '';
let customHex = null;
const inp = { P4: 0xFF, P5: 0x7F, P6: 0xFF, P7: 0xFF, P8: 0xFF };   // P57 = 0: no signal
let chan = 1, panelBits = 0x0F, pttDown = false, pttLatched = false;
const watch = new Set();
let txLog = [], lastTone = null, toneSince = 0, lastSelRx = '';

/* ------------------------------------------------------------ firmware */
const fwList = Object.keys(window.FIRMWARE_HEX || {}).sort((a, b) => (a === 'YA4000S.HEX' ? -1 : b === 'YA4000S.HEX' ? 1 : a.localeCompare(b)));
for (const n of fwList) { const o = document.createElement('option'); o.value = n; o.textContent = n; $('fwSel').appendChild(o); }
if (!fwList.length) { const o = document.createElement('option'); o.textContent = '(firmware.js yok)'; $('fwSel').appendChild(o); }
$('fwSel').value = store.get('sim.fw') || fwList[0] || '';
$('fwBtn').onclick = () => $('fwFile').click();
$('fwFile').onchange = e => {
  const f = e.target.files[0]; if (!f) return;
  f.text().then(t => {
    customHex = t; fwName = f.name;
    let o = [...$('fwSel').options].find(x => x.value === '__custom');
    if (!o) { o = document.createElement('option'); o.value = '__custom'; $('fwSel').appendChild(o); }
    o.textContent = f.name + ' (dosya)'; $('fwSel').value = '__custom';
    boot();
  });
};
$('fwSel').onchange = () => { if ($('fwSel').value !== '__custom') store.set('sim.fw', $('fwSel').value); boot(); };

/* ------------------------------------------------------------ EEPROM persistence */
function loadEeprom() {
  const s = store.get('sim.eeprom');
  if (s && $('eeKeep').checked) { try { const a = Uint8Array.from(atob(s), c => c.charCodeAt(0)); if (a.length === 2048) return migrateEeprom(a); } catch (e) { /* ignore */ } }
  return window.Board.defaultEeprom();
}
/* images saved by older versions of the page: keypad became a default (0x014 bit 3), once */
function migrateEeprom(a) {
  const v = +(store.get('sim.eeprom.v') || 1);
  if (v < 2) a[0x014] |= 0x08;
  if (v < 3) {   // names were written with 7 chars and a 0x00 as 8th: the firmware rejects such a name
    for (let n = 1; n <= 98; n++) { const r = 0x2E + (n - 1) * 13; if ((a[r] & 0x3F) !== 0x3F && a[r + 12] === 0) a[r + 12] = 0x20; }
  }
  if (v < 3) { window.Board.eeFixSums(a); store.set('sim.eeprom', btoa(String.fromCharCode.apply(null, a))); store.set('sim.eeprom.v', '3'); }
  return a;
}
function saveEeprom() {
  if (!board || !$('eeKeep').checked) return;
  store.set('sim.eeprom', btoa(String.fromCharCode.apply(null, board.eeprom.mem)));
  board.eeprom.dirty = false;
}

/* ------------------------------------------------------------ boot */
function boot(keepEeprom) {
  const sel = $('fwSel').value;
  const text = sel === '__custom' ? customHex : (window.FIRMWARE_HEX || {})[sel];
  if (!text) { $('stat').textContent = 'firmware yok: sim/firmware.js üretin (make sim) ya da HEX yükleyin'; return; }
  const ee = keepEeprom && board ? board.eeprom.mem.slice() : loadEeprom();
  board = new Board(parseHex(text));
  board.eeprom.mem.set(ee);
  board.inputs = inp;
  font = V.buildFont(board.rom);
  fwName = sel === '__custom' ? fwName : sel;
  $('fwName').textContent = fwName.replace(/\.HEX$/i, '');
  board.onSerial = v => u0push(v);
  board.cbusReply = parseInt($('cbReply').value, 16) || 0;
  board.lockFault = $('lockFault').checked;
  board.autoPower = $('autoPwr').checked;
  applyPanel(); applyRxInputs();
  for (const w of watch) addWatch(w);
  u0buf = []; renderU0(); txLog = []; renderTxLog();
  refreshEeUi();
  lastT = performance.now();
}
$('resetBtn').onclick = () => boot(true);
$('runBtn').onclick = () => { running = !running; $('runBtn').textContent = running ? 'Duraklat' : 'Çalıştır'; if (running) lastT = performance.now(); };
$('speed').onchange = e => { speed = parseFloat(e.target.value); };
$('themeBtn').onclick = () => {
  const r = document.documentElement, cur = r.getAttribute('data-theme');
  const dark = cur ? cur === 'dark' : matchMedia('(prefers-color-scheme: dark)').matches;
  r.setAttribute('data-theme', dark ? 'light' : 'dark');
};

/* ------------------------------------------------------------ front panel */
function applyPanel() {
  if (!board) return;
  // channel switch: high nibble = 16 - channel (original F_0058FB: 16 - (v >> 4))
  const v = ((16 - chan) & 0x0F) << 4 | (panelBits & 0x0F);
  board.panel.setExt(v, board);
  let p4 = 0xFF;
  if (pttDown || pttLatched) p4 &= ~0x80;
  if ($('btnEmg').classList.contains('latched')) p4 &= ~0x20;
  if ($('btnIgn').classList.contains('latched')) p4 &= ~0x10;
  inp.P4 = (inp.P4 & 0x0F) | (p4 & 0xF0);
  $('chanVal').textContent = chan;
  $('dial').style.setProperty('--rot', ((chan - 1) * 22.5 - 168) + 'deg');
}
function setChan(c) { chan = ((c - 1 + 16) % 16) + 1; applyPanel(); }
$('chUp').onclick = () => setChan(chan + 1);
$('chDn').onclick = () => setChan(chan - 1);
$('dial').addEventListener('wheel', e => { e.preventDefault(); setChan(chan + (e.deltaY < 0 ? 1 : -1)); });
(() => {
  let y0 = null, c0 = 1;
  $('dial').addEventListener('pointerdown', e => { y0 = e.clientY; c0 = chan; e.target.setPointerCapture(e.pointerId); });
  $('dial').addEventListener('pointermove', e => { if (y0 !== null) setChan(c0 + Math.round((y0 - e.clientY) / 12)); });
  $('dial').addEventListener('pointerup', () => { y0 = null; });
})();
function holdBtn(el, on, off) {
  el.addEventListener('pointerdown', e => { e.preventDefault(); el.classList.add('down'); on(); });
  const up = () => { if (el.classList.contains('down')) { el.classList.remove('down'); off(); } };
  el.addEventListener('pointerup', up); el.addEventListener('pointerleave', up); el.addEventListener('pointercancel', up);
}
holdBtn($('btnPtt'), () => { pttDown = true; applyPanel(); }, () => { pttDown = false; applyPanel(); });
$('btnExt').textContent = 'PTT kilitle';
$('btnExt').onclick = () => { pttLatched = !pttLatched; $('btnExt').classList.toggle('latched', pttLatched); applyPanel(); };
$('btnEmg').onclick = () => { $('btnEmg').classList.toggle('latched'); applyPanel(); };
$('btnIgn').onclick = () => { $('btnIgn').classList.toggle('latched'); applyPanel(); };
document.querySelectorAll('[data-pb]').forEach(b => {
  const bit = +b.dataset.pb;
  holdBtn(b, () => { panelBits &= ~(1 << bit); applyPanel(); }, () => { panelBits |= 1 << bit; applyPanel(); });
});
/* keypad: ROM 0x31EC order "*0#q789p456s123m", code = index */
const KP = '*0#q789p456s123m';
const KP_LAYOUT = ['1', '2', '3', 'm', '4', '5', '6', 's', '7', '8', '9', 'p', '*', '0', '#', 'q'];
for (const k of KP_LAYOUT) {
  const b = document.createElement('button'); b.className = 'rbtn'; b.textContent = k;
  b.title = 'kod ' + KP.indexOf(k) + ' → 0x' + k.charCodeAt(0).toString(16);
  holdBtn(b, () => board && board.keypad.press(KP.indexOf(k), true, board), () => board && board.keypad.press(KP.indexOf(k), false, board));
  b.dataset.key = k;
  $('keypad').appendChild(b);
}
document.addEventListener('keydown', e => {
  if (e.target.tagName === 'INPUT' || e.target.tagName === 'SELECT' || e.target.tagName === 'TEXTAREA' || e.repeat) return;
  if (e.code === 'Space') { e.preventDefault(); pttDown = true; $('btnPtt').classList.add('down'); applyPanel(); }
  else if (e.key === 'ArrowRight' || e.key === 'ArrowUp') { e.preventDefault(); setChan(chan + 1); }
  else if (e.key === 'ArrowLeft' || e.key === 'ArrowDown') { e.preventDefault(); setChan(chan - 1); }
  else if (KP.includes(e.key) && board) { board.keypad.press(KP.indexOf(e.key), true, board); const b = document.querySelector(`[data-key="${CSS.escape(e.key)}"]`); if (b) b.classList.add('down'); }
});
document.addEventListener('keyup', e => {
  if (e.code === 'Space') { pttDown = false; $('btnPtt').classList.remove('down'); applyPanel(); }
  else if (KP.includes(e.key) && board) { board.keypad.press(KP.indexOf(e.key), false, board); const b = document.querySelector(`[data-key="${CSS.escape(e.key)}"]`); if (b) b.classList.remove('down'); }
});

/* ------------------------------------------------------------ LCD rendering (14-segment) */
const SEGL = { a: [6, 4, 34, 4], b: [36, 7, 36, 27], c: [36, 33, 36, 53], d: [6, 56, 34, 56], e: [4, 33, 4, 53], f: [4, 7, 4, 27],
  g1: [7, 30, 18, 30], g2: [22, 30, 33, 30], h: [8, 8, 17, 26], i: [20, 7, 20, 27], j: [32, 8, 23, 26], k: [17, 34, 8, 52], l: [20, 33, 20, 53], m: [23, 34, 32, 52] };
const GLYPH = {
  '0': 'abcdefjk', '1': 'bcj', '2': 'abdeg1g2', '3': 'abcdg2', '4': 'bcfg1g2', '5': 'acdfg1g2', '6': 'acdefg1g2', '7': 'abc', '8': 'abcdefg1g2', '9': 'abcdfg1g2',
  A: 'abcefg1g2', B: 'abcdg2il', C: 'adef', D: 'abcdil', E: 'adefg1', F: 'aefg1', G: 'acdefg2', H: 'bcefg1g2', I: 'adil', J: 'bcde', K: 'efg1jm', L: 'def',
  M: 'bcefhj', N: 'bcefhm', O: 'abcdef', P: 'abefg1g2', Q: 'abcdefm', R: 'abefg1g2m', S: 'acdfg1g2', T: 'ail', U: 'bcdef', V: 'efjk', W: 'bcefkm', X: 'hjkm', Y: 'hjl', Z: 'adjk',
  '-': 'g1g2', '+': 'g1g2il', '*': 'g1g2hijklm', '/': 'jk', ':': 'il', '<': 'jm', '_': 'd', '^': 'hj', 'v': 'km', 'd': 'bcdeg1g2', 'm': 'ceg1g2l', 'e': 'abdefg1g2', 'j': 'cd', 'y': 'bcdfg1g2', 'r': 'eg1g2', 't': 'defg1g2', '?': 'abg2l',
};
function segList(g) { const r = []; let s = g; while (s.length) { const m = s.match(/^(g1|g2|[a-m])/); r.push(m[1]); s = s.slice(m[1].length); } return r; }
function glyphSvg(id, w, h) {
  let s = `<svg viewBox="-2 -2 44 64" width="${w}" height="${h}" id="${id}"><g transform="skewX(-6)">`;
  for (const k in SEGL) { const [x1, y1, x2, y2] = SEGL[k]; s += `<line class="seg" data-s="${k}" x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}"/>`; }
  return s + '</g></svg>';
}
$('lcdChan').innerHTML = glyphSvg('c0', 30, 44) + glyphSvg('c1', 30, 44);
$('lcdText').innerHTML = [...Array(8)].map((_, i) => glyphSvg('t' + i, 26, 38)).join('');
$('lcdText').style.display = 'flex'; $('lcdText').style.gap = '3px'; $('lcdChan').style.display = 'flex'; $('lcdChan').style.gap = '3px'; $('lcdChan').style.marginRight = '6px';
$('lcdSym').innerHTML = [...Array(16)].map((_, i) => `<span id="sym${i}">S${15 - i}</span>`).join('');
const glyphCache = {};
function setGlyph(id, ch) {
  if (glyphCache[id] === ch) return; glyphCache[id] = ch;
  const on = new Set(segList(GLYPH[ch] || GLYPH[ch.toUpperCase()] || (ch === ' ' ? '' : GLYPH['?'])));
  for (const l of $(id).querySelectorAll('line')) l.classList.toggle('on', on.has(l.dataset.s));
}
function renderLcd() {
  const d = V.decodeLcd(board, font);
  $('lcd').classList.toggle('off', !d.on);
  setGlyph('c0', d.chan[0]); setGlyph('c1', d.chan[1]);
  for (let i = 0; i < 8; i++) setGlyph('t' + i, d.text[i]);
  for (let i = 0; i < 16; i++) $('sym' + i).classList.toggle('on', !!(d.symbols >> (15 - i) & 1));
  $('lcd').title = 'LCD kelimeleri: ' + d.words.map(w => hex(w, 4)).join(' ') + ' | tampon 0x252: "' + V.textBuffer(board) + '"';
}

/* ------------------------------------------------------------ RX inputs */
for (let i = 0; i < V.CTCSS.length; i++) { const o = document.createElement('option'); o.value = V.CTCSS[i]; o.textContent = (i + 1) + ': ' + V.CTCSS[i].toFixed(1) + ' Hz'; $('ctcssIn').appendChild(o); }
for (const id of ['chRxTone', 'chTxTone']) {
  $(id).innerHTML = '<option value="0">yok</option>' + V.CTCSS.map((f, i) => `<option value="${i + 1}">${i + 1}: ${f.toFixed(1)} Hz</option>`).join('');
}
function applyRxInputs() {
  if (!board) return;
  inp.P5 = $('carrier').checked ? (inp.P5 | 0x80) : (inp.P5 & ~0x80);   // P57 = 1: squelch open
  board.analog[5] = +$('rssi').value; $('rssiV').textContent = $('rssi').value;
  board.ctcssIn.setTone(parseFloat($('ctcssIn').value) || 0);
  board.ctcssIn.noise = $('noiseIn').checked; board.selIn.noise = $('noiseIn').checked;
}
['carrier', 'rssi', 'ctcssIn', 'noiseIn'].forEach(id => $(id).addEventListener('input', applyRxInputs));
$('selSend').onclick = () => {
  const std = V.SELCALL[$('selStd').value];
  const seq = []; let prev = null;
  for (const ch of $('selDigits').value.toUpperCase()) {
    let d = V.SEL_DIGITS.indexOf(ch); if (d < 0) continue;
    if (d === prev) d = 14;
    seq.push({ f: std.f[d], ms: std.ms }); prev = d;
  }
  board.selIn.setSequence(board.now, [{ f: 0, ms: 20 }].concat(seq));
  addTxLog('RX seçmeli çağrı girişi: ' + $('selStd').value + ' ' + $('selDigits').value);
};
$('lockFault').onchange = e => { if (board) board.lockFault = e.target.checked; };
$('autoPwr').onchange = e => { if (board) board.autoPower = e.target.checked; };
$('int2').onchange = e => { if (board) board.setInt2(e.target.checked ? 0 : 1); };
$('cbReply').onchange = e => { if (board) board.cbusReply = parseInt(e.target.value, 16) || 0; };

/* ------------------------------------------------------------ TX analysis + audio */
function addTxLog(s) {
  txLog.push(((board ? board.now : 0) / FCLK).toFixed(3).padStart(8) + ' s  ' + s);
  if (txLog.length > 200) txLog.shift();
  renderTxLog();
}
function renderTxLog() { const el = $('txLog'); el.textContent = txLog.join('\n'); el.scrollTop = el.scrollHeight; }
$('clrTxLog').onclick = () => { txLog = []; renderTxLog(); };
function nearestCtcss(f) { let b = 0; for (let i = 1; i < V.CTCSS.length; i++) if (Math.abs(V.CTCSS[i] - f) < Math.abs(V.CTCSS[b] - f)) b = i; return b; }
function analyse() {
  const r = V.analyseTx(board, FCLK, FCLK * 0.03);
  if (r.ctcssHz) { const i = nearestCtcss(r.ctcssHz); $('txCtcss').textContent = r.ctcssHz.toFixed(1) + ' Hz (ton ' + (i + 1) + ': ' + V.CTCSS[i] + ')'; }
  else $('txCtcss').textContent = '—';
  $('txTone').textContent = r.toneHz ? r.toneHz.toFixed(0) + ' Hz' + (r.selcall ? '  ' + r.selcall.join(' ') : '') : '—';
  $('txDtmf').textContent = r.dtmf || '—';
  let sd = ''; for (let k = 0; k < 8; k++) { const v = board.ram[0x264 + k]; if (v < 15) sd += V.SEL_DIGITS[v]; }
  if (sd && sd !== lastSelRx) addTxLog('RX seçmeli çağrı tamponu: ' + sd);
  lastSelRx = sd; $('selRx').textContent = sd || '—';
  const key = r.dtmf ? 'DTMF ' + r.dtmf : (r.toneHz ? 'ton ' + Math.round(r.toneHz / 10) * 10 + ' Hz' + (r.selcall ? ' (' + r.selcall.join(' ') + ')' : '') : null);
  if (key !== lastTone) {
    if (key && board.now - toneSince > 0) addTxLog('TX ' + key);
    lastTone = key; toneSince = board.now;
  }
}
let actx = null, audioT = 0, lastAudioIdx = 0;
$('audioOn').onchange = e => {
  if (e.target.checked && !actx) { try { actx = new AudioContext(); } catch (err) { e.target.checked = false; } }
};
function pumpAudio(fromT, toT) {
  if (!actx || !$('audioOn').checked || toT <= fromT) return;
  const sr = actx.sampleRate, n = Math.min(Math.floor((toT - fromT) / FCLK * sr), sr);
  if (n < 32) return;
  const buf = actx.createBuffer(1, n, sr), d = buf.getChannelData(0);
  const ta1 = board.audio.ta1, ta2 = board.audio.ta2;
  const sample = (log, t) => { let lo = 0, hi = log.length / 2 - 1, v = null; while (lo <= hi) { const m = (lo + hi) >> 1; if (log[2 * m] <= t) { v = log[2 * m + 1]; lo = m + 1; } else hi = m - 1; } return v; };
  let mean1 = 0, c1 = 0; for (let i = 1; i < ta1.length; i += 2) if (ta1[i - 1] >= fromT - FCLK * 0.05) { mean1 += ta1[i]; c1++; }
  mean1 = c1 ? mean1 / c1 : 0;
  const tx = board.outBit(0x0B, 5) === 0;
  for (let i = 0; i < n; i++) {
    const t = fromT + i / sr * FCLK;
    let v = 0;
    const s1 = sample(ta1, t); if (s1 !== null && c1 && t - ta1[ta1.length - 2] < FCLK * 0.01) v += (s1 - mean1) / (mean1 || 1) * 0.5;
    const fb = sample(board.audio.beep, t); if (fb) v += (Math.floor(2 * fb * t / FCLK) & 1 ? 0.18 : -0.18);
    if (tx) { const s2 = sample(ta2, t); if (s2 !== null && t - ta2[ta2.length - 2] < FCLK * 0.05) v += (s2 - 128) / 128 * 0.15; }
    d[i] = Math.max(-1, Math.min(1, v));
  }
  const src = actx.createBufferSource(); src.buffer = buf; src.connect(actx.destination);
  audioT = Math.max(audioT, actx.currentTime + 0.05); src.start(audioT); audioT += n / sr;
}

/* ------------------------------------------------------------ status views */
function fmtMHz(h) { return (h / 1e6).toFixed(4) + ' MHz'; }
function renderRf() {
  const st = board.radioState(), s = st.synth, tx = st.rxSelect === 0 && st.powerHold === 1;
  const P = s.prescaler(), vco = st.vco;
  const rf = !vco ? '—' : tx ? fmtMHz(vco) + ' (TX)' : fmtMHz(P === 64 ? vco - 45e6 : vco + 45e6) + ' (RX)';
  $('synthKv').innerHTML = `<div>VCO</div><div class="mono">${vco ? fmtMHz(vco) : '—'}</div><div>RF</div><div class="mono">${rf}</div>
    <div>R / adım</div><div class="mono">${s.R || '—'}${s.R ? ' / ' + (12.8e6 / s.R / 1e3).toFixed(2) + ' kHz' : ''}</div>
    <div>N, A, ön bölücü</div><div class="mono">${s.Nc}, ${s.Ac}, ${P}/${P + 1}</div>
    <div>Yükleme sayısı</div><div class="mono">${s.loads}</div><div>Kilit (P67)</div><div>${st.locked ? '<span class="pill ok">kilitli</span>' : '<span class="pill bad">kilitsiz</span>'}</div>`;
  const b = v => v === null ? '<span class="note">giriş</span>' : v;
  $('outKv').innerHTML = `<div>P71 GÜÇ_AÇMA</div><div>${b(st.powerHold)}</div><div>P55 ALMA_SEÇ</div><div>${b(st.rxSelect)} ${tx ? '<span class="pill bad">TX</span>' : ''}</div>
    <div>P72 GÖNDERME_SES</div><div>${b(st.txAudio)}</div><div>P50 UYARI</div><div>${b(st.alert)}</div><div>P46 SUSTURMA_BİLGİSİ</div><div>${b(st.squelchInfo)}</div>
    <div>P43 KRİSTAL_KAYDIRMA</div><div>${b(st.xshift)}</div>
    <div>Güç PWM (TA3)</div><div>${st.pwrPwm === null ? 'kapalı' : (st.pwrPwm * 100).toFixed(0) + ' %'}  AN7=${board.analogRead(7)}</div>
    <div>TKS PWM (TA2)</div><div>${st.ctcssPwm ? 'çalışıyor' : 'kapalı'}</div>`;
  $('ledTx').classList.toggle('on', tx && st.powerHold === 1);
  $('ledRx').classList.toggle('on', $('carrier').checked && !tx);
  $('ledLock').classList.toggle('on', st.locked);
  $('ledPwr').classList.toggle('on', st.powerHold === 1);
}

/* ------------------------------------------------------------ serial */
let u0buf = [];
function u0push(v) { u0buf.push(v); if (u0buf.length > 8000) u0buf.splice(0, 2000); u0dirty = true; }
let u0dirty = false;
function renderU0() {
  const asc = document.querySelector('input[name=u0v]:checked').value === 'ascii';
  $('u0term').textContent = asc ? u0buf.map(v => v === 10 ? '\n' : v === 13 ? '' : v >= 32 && v < 127 ? String.fromCharCode(v) : '·').join('')
                                : u0buf.map(v => hex(v, 2)).join(' ');
  $('u0term').scrollTop = $('u0term').scrollHeight; u0dirty = false;
}
document.querySelectorAll('input[name=u0v]').forEach(r => r.onchange = renderU0);
$('u0clr').onclick = () => { u0buf = []; renderU0(); };
$('u0send').onclick = () => {
  const s = $('u0in').value, m = $('u0mode').value;
  let bytes = m === 'hex' ? s.split(/[\s,]+/).filter(Boolean).map(x => parseInt(x, 16) & 0xFF) : [...s].map(c => c.charCodeAt(0) & 0xFF);
  if (m === 'textcr') bytes.push(13);
  board.serialIn(bytes);
};
$('u0in').addEventListener('keydown', e => { if (e.key === 'Enter') $('u0send').click(); });
let cbShown = 0;
$('cbclr').onclick = () => { board.cbusLog.length = 0; cbShown = 0; $('cbLog').textContent = ''; };
function renderSerialInfo() {
  const s = board.sfr, mod = s[0x30];
  const baud = (mod & 7) >= 4 ? Math.round(FCLK / [1, 8, 32, 256][s[0x34] & 3] / 16 / (s[0x31] + 1)) : 0;
  $('u0info').textContent = 'U0MOD=' + hex(mod, 2) + ' BRG=' + s[0x31] + (baud ? ' ≈ ' + baud + ' baud' : '') + ' · ' + u0buf.length + ' bayt';
  if (board.cbusLog.length < cbShown) cbShown = 0;
  if (board.cbusLog.length !== cbShown) {
    const lines = board.cbusLog.slice(-150).map(e => (e[0] / FCLK).toFixed(4).padStart(9) + '  ' + e[1].padEnd(5) + hex(e[2], 2));
    $('cbLog').textContent = lines.join('\n'); $('cbLog').scrollTop = $('cbLog').scrollHeight; cbShown = board.cbusLog.length;
  }
}

/* ------------------------------------------------------------ EEPROM UI */
function refreshEeUi() {
  if (!board) return;
  const ee = board.eeprom.mem, B = window.Board;
  const sums = B.eeCheckSums(ee);
  $('eeSums').innerHTML = sums.map((ok, i) => `<span class="pill ${ok ? 'ok' : 'bad'}">${hex(B.EE_SUMS[i][0], 3)}</span>`).join(' ');
  $('eeBase').value = (B.eeBase(ee) * 0.025).toFixed(3);
  $('eeOpt58b').value = hex(ee[0x58B], 2); $('eeOpt014').value = hex(ee[0x014], 2); $('eeOpt01a').value = hex(ee[0x01A], 2);
  $('eeSelStd').value = ee[0x018] & 3;
  $('eeLang').checked = !!(ee[0x018] & 0x80);
  const tn = t => t ? t + ' (' + V.CTCSS[t - 1] + ')' : '—';
  let rows = '<tr><th>Kanal</th><th>Ad</th><th>RX MHz</th><th>TX MHz</th><th>RX TKS</th><th>TX TKS</th><th>Sç</th><th>Ham kayıt</th></tr>';
  for (let n = 1; n <= 98; n++) {
    const c = B.eeChannel(ee, n); if (!c) continue;
    rows += `<tr data-ch="${n}" style="cursor:pointer"><td>${n}</td><td class="mono">${c.name.replace(/</g, '&lt;')}</td><td class="mono">${(c.rx / 1e6).toFixed(4)}</td><td class="mono">${(c.tx / 1e6).toFixed(4)}</td><td>${tn(c.rxTone)}</td><td>${tn(c.txTone)}</td><td>${c.selcall ? '✓' : ''}</td><td class="mono">${c.bytes.map(x => hex(x, 2)).join(' ')}</td></tr>`;
  }
  $('chTable').innerHTML = rows;
  $('chTable').querySelectorAll('tr[data-ch]').forEach(tr => tr.onclick = () => {
    const n = +tr.dataset.ch, c = B.eeChannel(ee, n);
    $('chN').value = n; $('chRx').value = (c.rx / 1e6).toFixed(4); $('chTx').value = (c.tx / 1e6).toFixed(4);
    $('chName').value = c.name.trim(); $('chRxTone').value = c.rxTone; $('chTxTone').value = c.txTone; $('chSel').checked = !!c.selcall; $('chXs').checked = !!c.xshift;
  });
  renderEeHex();
}
function renderEeHex() {
  const ee = board.eeprom.mem, a0 = (parseInt($('eeAddr').value, 16) || 0) & 0x7F0;
  let s = '';
  for (let r = a0; r < Math.min(a0 + 0x200, 0x800); r += 16) {
    s += hex(r, 3) + '  ';
    for (let i = 0; i < 16; i++) s += `<span data-ea="${r + i}">${hex(ee[r + i], 2)}</span> `;
    s += ' ' + [...ee.slice(r, r + 16)].map(v => v >= 32 && v < 127 ? String.fromCharCode(v).replace('<', '&lt;').replace('&', '&amp;') : '.').join('') + '\n';
  }
  $('eeHex').innerHTML = s;
}
$('eeAddr').onchange = renderEeHex;
$('eeHex').onclick = e => {
  const a = e.target.dataset && e.target.dataset.ea; if (a === undefined) return;
  const v = prompt('EEPROM 0x' + hex(+a, 3) + ' yeni değer (hex):', hex(board.eeprom.mem[+a], 2));
  if (v === null) return; board.eeprom.mem[+a] = parseInt(v, 16) & 0xFF; saveEeprom(); refreshEeUi();
};
$('eeFix').onclick = () => { window.Board.eeFixSums(board.eeprom.mem); saveEeprom(); refreshEeUi(); };
$('eeDefault').onclick = () => { board.eeprom.mem.set(window.Board.defaultEeprom()); saveEeprom(); boot(true); };
$('eeDown').onclick = () => {
  const a = document.createElement('a'); a.href = URL.createObjectURL(new Blob([board.eeprom.mem], { type: 'application/octet-stream' }));
  a.download = 'eeprom_24c16.bin'; a.click(); setTimeout(() => URL.revokeObjectURL(a.href), 1000);
};
$('eeUp').onclick = () => $('eeFile').click();
$('eeFile').onchange = e => {
  const f = e.target.files[0]; if (!f) return;
  f.arrayBuffer().then(b => { const a = new Uint8Array(b); board.eeprom.mem.fill(0xFF); board.eeprom.mem.set(a.slice(0, 2048)); saveEeprom(); boot(true); });
};
$('eeKeep').onchange = () => { if ($('eeKeep').checked) saveEeprom(); };
$('eeApply').onclick = () => {
  const ee = board.eeprom.mem, B = window.Board;
  const oldBase = B.eeBase(ee), chans = [];
  for (let n = 1; n <= 98; n++) { const c = B.eeChannel(ee, n); if (c) chans.push([n, c]); }
  const base = Math.round(parseFloat($('eeBase').value) / 0.025);
  ee[B.EE_CFG + 1] = base >> 8; ee[B.EE_CFG + 2] = base & 0xFF;
  if (base !== oldBase) for (const [n, c] of chans) { try { B.eeSetChannel(ee, n, c.rx, c.tx, c); } catch (err) { B.eeClearChannel(ee, n); } }
  ee[0x58B] = parseInt($('eeOpt58b').value, 16) & 0xFF; ee[0x014] = parseInt($('eeOpt014').value, 16) & 0xFF;
  ee[0x018] = (ee[0x018] & 0x7C) | ($('eeLang').checked ? 0x80 : 0) | (+$('eeSelStd').value & 3);
  ee[0x01A] = parseInt($('eeOpt01a').value, 16) & 0xFF;
  B.eeFixSums(ee); saveEeprom(); boot(true);
};
$('chSet').onclick = () => {
  try {
    window.Board.eeSetChannel(board.eeprom.mem, +$('chN').value, parseFloat($('chRx').value) * 1e6, parseFloat($('chTx').value) * 1e6,
      { name: $('chName').value, rxTone: +$('chRxTone').value, txTone: +$('chTxTone').value, selcall: $('chSel').checked, xshift: $('chXs').checked });
    window.Board.eeFixSums(board.eeprom.mem); saveEeprom(); refreshEeUi();
  } catch (err) { alert(err.message); }
};
$('chDel').onclick = () => { window.Board.eeClearChannel(board.eeprom.mem, +$('chN').value); window.Board.eeFixSums(board.eeprom.mem); saveEeprom(); refreshEeUi(); };

/* ------------------------------------------------------------ analog + pins */
const ANA = [[3, 'AN3 sıcaklık'], [4, 'AN4 13,2 V'], [5, 'AN5 RSSI'], [6, 'AN6 yansıyan güç'], [7, 'AN7 giden güç']];
$('anaRows').innerHTML = ANA.map(([c, n]) => `<div class="row"><label style="min-width:150px">${n}</label><input type="range" min="0" max="255" id="an${c}"><span class="mono" id="anv${c}"></span></div>`).join('');
for (const [c] of ANA) $('an' + c).oninput = e => { board.analog[c] = +e.target.value; if (c === 5) { $('rssi').value = e.target.value; $('rssiV').textContent = e.target.value; } };
const PORTS = [['P4', 0x0A], ['P5', 0x0B], ['P6', 0x0E], ['P7', 0x0F], ['P8', 0x12]];
const PIN_NAMES = {
  P4: ['', '', '', 'KRİSTAL_KAYDIRMA', 'KONTAK/PTT', 'ACİL', 'SUSTURMA_BİLGİSİ', 'DIŞ_BAS_KONUŞ'],
  P5: ['UYARI', 'TDSEC1', 'SÇ_TON (TA1OUT)', 'TDSEC2', 'TKS (TA2OUT)', 'ALMA_SEÇ', 'GÜÇ_AYAR (TA3OUT)', 'SUSTURMA'],
  P6: ['UZAK_VERİ', 'SCL', 'SDA', 'INT1 genişletici', 'INT2 C-BUS', 'TKS_ÇÖZME', 'SÇ_ÇÖZME', 'SENTEZÖR_KİLİT'],
  P7: ['SENT_YÜKLE', 'GÜÇ_AÇMA', 'GÖNDERME_SES', 'AN3', 'AN4', 'AN5', 'AN6', 'AN7'],
  P8: ['', 'SENT_SEÇ', 'RxD0', 'TxD0', '', 'CLK1', 'RxD1', 'TxD1'],
};
function renderPorts() {
  let h = '';
  for (const [n, a] of PORTS) {
    const d = board.dirOf(a), val = board.portRead(a);
    h += `<div class="row" style="margin:3px 0"><span style="width:28px">${n}</span><span class="bits">`;
    for (let bit = 7; bit >= 0; bit--) {
      const out = d >> bit & 1, v = val >> bit & 1, lo = !out && !(inp[n] >> bit & 1);
      h += `<button data-p="${n}" data-b="${bit}" class="${out ? 'out' : ''} ${lo ? 'lo' : ''}" title="${n}${bit} ${PIN_NAMES[n][bit]} ${out ? '(çıkış)' : '(giriş)'}">${v}</button>`;
    }
    h += `</span><span class="note">yön ${hex(d, 2)} · latch ${hex(board.latch[a], 2)}</span></div>`;
  }
  $('portRows').innerHTML = h;
}
$('portRows').onclick = e => {
  const t = e.target; if (!t.dataset.p || t.classList.contains('out')) return;
  inp[t.dataset.p] ^= 1 << +t.dataset.b; renderPorts();
};
function renderTimers() {
  const T = board.timers, s = board.sfr;
  const modeN = (t, m) => t.isB ? ['zamanlayıcı', 'olay sayacı', 'darbe ölçme', '-'][m & 3] : ['zamanlayıcı', 'olay sayacı', 'tek darbe', 'PWM'][m & 3];
  let o = 'Ad   çalışıyor  mod             sayaç  yeniden  IC (öncelik, istek)\n';
  for (const t of T) {
    const m = s[t.mod];
    o += `${t.name.padEnd(5)}${(t.running ? 'evet' : '-').padEnd(11)}${modeN(t, m).padEnd(16)}${hex(t.counter & 0xFFFF, 4)}   ${hex(t.reload & 0xFFFF, 4)}     ${s[t.ic] & 7}${s[t.ic] & 8 ? ' IR' : ''}\n`;
  }
  const ics = [[0x7D, 'INT0'], [0x7E, 'INT1'], [0x7F, 'INT2'], [0x71, 'U0TX'], [0x72, 'U0RX'], [0x70, 'AD']];
  o += '\n' + ics.map(([a, n]) => n + '=' + (s[a] & 7) + (s[a] & 8 ? '*' : '')).join('  ');
  o += `\nCPU: I=${board.cpu.I} IPL=${board.cpu.IPL} ${board.cpu.waiting ? (board.cpu.stopped ? 'STP' : 'WIT (kesme bekliyor)') : 'çalışıyor'} · son kesme: ${board.cpu.lastIrq || '-'}`;
  $('timerInfo').textContent = o;
  for (const [c] of ANA) { const v = board.analogRead(c); if (document.activeElement !== $('an' + c)) $('an' + c).value = v; $('anv' + c).textContent = v; }
}

/* ------------------------------------------------------------ debugger */
const rd = a => board.rd(a & 0xFFFFFF);
let disBase = null;
function renderDbg() {
  const c = board.cpu;
  const f = (n, v) => n + '=' + v;
  $('regs').textContent = [
    `PC=${hex(c.PG, 2)}:${hex(c.PC, 4)}  A=${hex(c.A, 4)}  B=${hex(c.B, 4)}  X=${hex(c.X, 4)}  Y=${hex(c.Y, 4)}`,
    `S=${hex(c.S, 4)}  D=${hex(c.D, 4)}  DT=${hex(c.DT, 2)}  PS=${hex(c.getP(), 4)}`,
    `N${c.N} V${c.V} m${c.m} x${c.x} D${c.Dm} I${c.I} Z${c.Z} C${c.C}  IPL=${c.IPL}  ${c.waiting ? (c.stopped ? 'STP' : 'WIT') : ''}`,
    `zaman ${(board.now / FCLK).toFixed(4)} s`,
  ].join('\n');
  const pc = c.PG << 16 | c.PC;
  let a = disBase !== null ? disBase : pc;
  // start a few instructions before PC using the previous PC as anchor
  if (disBase === null && c.prevPC < pc && pc - c.prevPC < 12) a = c.prevPC;
  let s = '';
  for (let i = 0; i < 22; i++) {
    const d = disasm(rd, a, c.m, c.x);
    const bytes = []; for (let k = 0; k < d.len; k++) bytes.push(hex(rd(a + k), 2));
    const cls = a === pc ? 'cur' : board.breakpoints.has(a) ? 'bp' : '';
    s += `<span class="${cls}" data-da="${a}">${board.breakpoints.has(a) ? '●' : ' '} ${hex(a, 6)}  ${bytes.join(' ').padEnd(15)} ${d.text}</span>\n`;
    a += d.len;
  }
  $('dis').innerHTML = s;
  const m0 = (parseInt($('memAddr').value, 16) || 0) & 0xFFFFF0;
  let ms = '';
  for (let r = m0; r < m0 + 0x100; r += 16) {
    const bs = []; for (let i = 0; i < 16; i++) bs.push(rd(r + i));
    ms += hex(r, 6) + '  ' + bs.map(v => hex(v, 2)).join(' ') + '  ' + bs.map(v => v >= 32 && v < 127 ? String.fromCharCode(v) : '.').join('').replace(/&/g, '&amp;').replace(/</g, '&lt;') + '\n';
  }
  $('mem').innerHTML = ms;
  $('evLog').textContent = board.events.slice(-60).map(e => (e[0] / FCLK).toFixed(4).padStart(9) + '  ' + e[1]).join('\n');
  $('i2cLog').textContent = board.i2c.log.join(' ');
  $('bpList').textContent = [...board.breakpoints].map(x => hex(x, 6)).join(' ');
  $('wpList').textContent = [...watch].map(x => hex(x, 4)).join(' ');
}
$('dis').onclick = e => {
  const a = e.target.dataset && e.target.dataset.da; if (a === undefined) return;
  const v = +a; if (board.breakpoints.has(v)) board.breakpoints.delete(v); else board.breakpoints.add(v); renderDbg();
};
function pause(msg) { running = false; $('runBtn').textContent = 'Çalıştır'; if (msg) $('stat').textContent = msg; renderAll(true); }
$('stepBtn').onclick = () => { pause(); board.step(); disBase = null; renderAll(true); };
$('step100').onclick = () => { pause(); for (let i = 0; i < 100; i++) board.step(); disBase = null; renderAll(true); };
$('stepOver').onclick = () => { pause(); board.run(FCLK / 1000); disBase = null; renderAll(true); };
$('bpAdd').onclick = () => { const v = parseInt($('bpIn').value, 16); if (!isNaN(v)) board.breakpoints.add(v); renderDbg(); };
function addWatch(a) { watch.add(a); board.watch = watch; }
$('wpAdd').onclick = () => { const v = parseInt($('wpIn').value, 16); if (!isNaN(v)) addWatch(v); renderDbg(); };
$('disGo').onclick = () => { const v = parseInt($('disAddr').value, 16); disBase = isNaN(v) ? null : v; renderDbg(); };
$('disPc').onclick = () => { disBase = null; renderDbg(); };
$('memGo').onclick = renderDbg;

/* ------------------------------------------------------------ tabs */
let activeTab = store.get('sim.tab') || 'rf';
function showTab(t) {
  activeTab = t; store.set('sim.tab', t);
  document.querySelectorAll('#tabs button').forEach(b => b.classList.toggle('active', b.dataset.tab === t));
  document.querySelectorAll('.tab').forEach(d => d.classList.toggle('active', d.id === 'tab-' + t));
  renderAll(true);
}
document.querySelectorAll('#tabs button').forEach(b => b.onclick = () => showTab(b.dataset.tab));

/* ------------------------------------------------------------ main loop */
let frame = 0, perfC = 0, perfT = performance.now(), simRate = 0;
function renderAll(force) {
  if (!board) return;
  renderLcd(); renderRf();
  if (activeTab === 'rf' || force) analyse();
  if (activeTab === 'serial') { if (u0dirty || force) renderU0(); renderSerialInfo(); }
  if (activeTab === 'io' && (frame % 6 === 0 || force)) { renderPorts(); renderTimers(); }
  if (activeTab === 'dbg' && (frame % 6 === 0 || force)) renderDbg();
  if (activeTab === 'ee' && board.eeprom.dirty) { saveEeprom(); refreshEeUi(); }
  else if (board.eeprom.dirty && frame % 60 === 0) saveEeprom();
}
function loop(t) {
  requestAnimationFrame(loop);
  if (!board) return;
  frame++;
  if (running) {
    const dt = Math.min(0.1, (t - lastT) / 1000); lastT = t;
    const t0 = performance.now(), from = board.now;
    const sp = updater && $('updFast').checked ? 1000 : speed;
    let cycles = sp >= 100 ? FCLK : FCLK * dt * sp;
    const end = board.now + cycles;
    let res = 'ok';
    while (board.now < end) {
      res = board.run(Math.min(end - board.now, FCLK / 200));
      if (updater) updTick();
      if (res !== 'ok' || performance.now() - t0 > 30) break;
    }
    perfC += board.now - from;
    if (res !== 'ok') pause(res === 'break' ? 'kesme noktası ' + hex(board.cpu.PG << 16 | board.cpu.PC, 6) : 'RAM izleme: ' + res);
    pumpAudio(from, board.now);
  }
  if (t - perfT > 500) { simRate = perfC / FCLK / ((t - perfT) / 1000); perfC = 0; perfT = t; }
  $('stat').textContent = (board.now / FCLK).toFixed(2) + ' s · hız ' + (simRate * 100).toFixed(0) + ' % · ' +
    (board.cpu.stopped ? 'STP' : board.cpu.waiting ? 'WIT' : 'PC ' + hex(board.cpu.PG, 2) + ':' + hex(board.cpu.PC, 4));
  renderAll(false);
}
/* ------------------------------------------------------------ serial firmware update */
let updater = null, updHex = null, updHexName = '';
function updFillSources() {
  const sel = $('updSrc'); sel.innerHTML = '';
  for (const n of fwList) { const o = document.createElement('option'); o.value = n; o.textContent = n; sel.appendChild(o); }
  if (updHex) { const o = document.createElement('option'); o.value = '__file'; o.textContent = updHexName + ' (dosya)'; sel.appendChild(o); sel.value = '__file'; }
}
updFillSources();
$('updFile').onchange = e => {
  const f = e.target.files[0]; if (!f) return;
  f.text().then(t => { updHex = t; updHexName = f.name; updFillSources(); });
};
function updLog(s) { const d = $('updLog'); d.textContent += s + '\n'; d.scrollTop = d.scrollHeight; }
$('updStart').onclick = () => {
  if (!board || updater) return;
  const src = $('updSrc').value, text = src === '__file' ? updHex : (window.FIRMWARE_HEX || {})[src];
  if (!text) { updLog('HEX yok'); return; }
  const image = parseHex(text);
  if (!image.size) { updLog('HEX okunamadı'); return; }
  $('updLog').textContent = ''; $('updProg').value = 0;
  updater = new window.Updater.FlashUpdater(board, image, { verify: $('updVerify').checked, fixSum: $('updFixSum').checked, fclk: FCLK, onLog: updLog });
  if (!running) $('runBtn').click();
};
$('updStop').onclick = () => { if (updater) { updater.finish('kullanıcı durdurdu (flash yarım kalmış olabilir)'); updater = null; } };
function updTick() {
  updater.tick();
  $('updProg').value = updater.progress;
  if (updater.done) {
    if (!updater.error) { font = V.buildFont(board.rom); fwName = ($('updSrc').value === '__file' ? updHexName : $('updSrc').value) + ' (seri)'; $('fwName').textContent = fwName.replace(/\.HEX$/i, ''); }
    updater = null;
  }
}

window.addEventListener('error', e => { pause('hata: ' + e.message); });
boot();
/* test hook: index.html#t=3&ptt=1&tab=dbg runs 3 s before the first frame */
const hp = new URLSearchParams(location.hash.slice(1));
if (hp.get('tab')) activeTab = hp.get('tab');
if (hp.get('t') && board) {
  board.run(FCLK * parseFloat(hp.get('t')));
  if (hp.get('ptt')) { pttDown = true; applyPanel(); board.run(FCLK * parseFloat(hp.get('ptt'))); }
  if (hp.get('ch')) { setChan(+hp.get('ch')); board.run(FCLK * 0.5); }
  if (hp.get('upd')) {        // #t=2.6&upd=1&verify=0: serial update run to the end before the first frame
    $('updVerify').checked = hp.get('verify') !== '0'; $('updStart').onclick();
    while (updater && board.now < FCLK * 900) { board.run(FCLK / 200); updTick(); }
    board.run(FCLK * 3);
  }
  if (hp.get('pause')) running = false;
}
showTab(activeTab);
requestAnimationFrame(loop);
window.sim = { get board() { return board; }, boot };
})();
