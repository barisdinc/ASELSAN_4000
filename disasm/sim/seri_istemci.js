/* seri_istemci.js - PC side of the radio's serial service protocol (../seri_protokol.md),
 * read-only operations: link, ROM header, EEPROM backup, flash backup, firmware identification.
 * Nothing here writes to the radio.
 *
 * One client, two ports:
 *   WebSerialPort  a real radio through a USB-RS232 adapter (Chrome / Edge, Web Serial API)
 *   SimPort        the simulated radio (board.js); used by yukle_test.js and the page's demo mode
 * A port provides: write(bytes) -> Promise, onByte = fn (set by the client), sleep(ms) -> Promise,
 * now() -> ms.  Framing comes from updater.js (frame, FrameParser).
 */
(function (root) {
'use strict';
const U = root.Updater || (typeof require !== 'undefined' ? require('./updater.js') : null);

/* ------------------------------------------------------------------ ports */
class WebSerialPort {
  static supported() { return typeof navigator !== 'undefined' && !!navigator.serial; }
  async open(baudRate = 9600) {
    this.port = await navigator.serial.requestPort();
    await this.port.open({ baudRate, dataBits: 8, stopBits: 1, parity: 'none', flowControl: 'none' });
    this.writer = this.port.writable.getWriter();
    this.reading = this.readLoop();
  }
  async readLoop() {
    while (this.port && this.port.readable) {
      this.reader = this.port.readable.getReader();
      try {
        for (;;) {
          const { value, done } = await this.reader.read();
          if (done) break;
          if (this.onByte) for (const b of value) this.onByte(b);
        }
      } catch (e) { /* framing/parity error or unplugged: keep reading if still open */ }
      finally { this.reader.releaseLock(); }
      if (this.closing) break;
    }
  }
  async write(bytes) { await this.writer.write(Uint8Array.from(bytes)); }
  sleep(ms) { return new Promise(r => setTimeout(r, ms)); }
  now() { return performance.now(); }
  async close() {
    this.closing = true;
    try { if (this.reader) await this.reader.cancel(); } catch (e) {}
    try { await this.reading; } catch (e) {}
    try { this.writer.releaseLock(); } catch (e) {}
    try { await this.port.close(); } catch (e) {}
    this.port = null;
  }
}

class SimPort {
  /* board: a running Board (board.js); fclk: its clock */
  constructor(board, fclk) {
    this.b = board; this.fclk = fclk || 4032000; this.yieldEvery = 0;
    board.onSerial = v => { if (this.onByte) this.onByte(v); };
  }
  async write(bytes) { this.b.serialIn(Array.from(bytes)); }
  async sleep(ms) {
    this.b.run(this.fclk * ms / 1000);
    // in a browser give the page a chance to repaint now and then
    if (typeof setTimeout !== 'undefined' && ++this.yieldEvery % 20 === 0) await new Promise(r => setTimeout(r, 0));
  }
  now() { return this.b.now / this.fclk * 1000; }
  async close() { this.b.onSerial = null; }
}

/* ------------------------------------------------------------------ client */
class RadioClient {
  constructor(port, opts = {}) {
    this.port = port; this.timeout = opts.timeout || 1500; this.tries = opts.tries || 4;
    this.log = opts.log || (() => {});
    this.seq = 1; this.rxq = [];
    this.parser = new U.FrameParser(f => this.onFrame(f));
    port.onByte = b => this.parser.push(b);
  }
  onFrame(f) {
    if (f[0] === 4) this.port.write(U.frame([2, (f[1] + 1) & 0xFF, 1, 0]));   // ACK every data frame of the radio
    this.rxq.push(f);
  }
  async waitFrame(match, ms) {
    const until = this.port.now() + (ms || this.timeout);
    for (;;) {
      while (this.rxq.length) { const f = this.rxq.shift(); if (match(f)) return f; }
      if (this.port.now() > until) return null;
      await this.port.sleep(5);
    }
  }
  send(body) { return this.port.write(U.frame(body)); }

  /* type 1: (re)start the link; the radio answers with an ACK */
  async connect() {
    for (let i = 0; i < 5; i++) {
      this.rxq.length = 0; await this.send([1]);
      if (await this.waitFrame(f => f[0] === 2)) { this.seq = 1; this.log('bağlantı kuruldu'); return true; }
    }
    throw new Error('telsiz cevap vermiyor: kablo, seri port ve 9600 baud ayarını kontrol edin, telsiz açık olmalı');
  }

  /* type 4 data frame, stop-and-wait; the ACK carries the next expected sequence number */
  async request(payload, wantReply) {
    for (let t = 0; t < this.tries; t++) {
      this.rxq.length = 0;
      await this.send([4, this.seq, 1, 0].concat(payload));
      const ack = await this.waitFrame(f => f[0] === 2);
      if (!ack) { this.log('cevap yok, tekrar (' + (t + 1) + ')'); continue; }
      if (ack[1] !== ((this.seq + 1) & 0xFF)) { this.log('sıra no. ' + this.seq + ' → ' + ack[1]); this.seq = ack[1]; continue; }
      this.seq = ack[1];
      if (!wantReply) return true;
      const rep = await this.waitFrame(f => f[0] === 4);
      if (rep) return rep.slice(4);
      this.log('veri cevabı yok, tekrar');
    }
    throw new Error('telsiz cevap vermiyor (komut ' + payload.slice(0, 2).map(x => x.toString(16).padStart(2, '0')).join(' ') + ')');
  }

  async ping() { const r = await this.request([0x0C], true); return r[0] === 0x0C; }

  /* 03 01 01 00 aH aL n -> 03 04 01 00 aH aL <data> */
  async readEepromBlock(addr, n) {
    const r = await this.request([3, 1, 1, 0, addr >> 8 & 0xFF, addr & 0xFF, n], true);
    if (r[0] !== 3 || r[1] !== 4 || r[2] !== 1 || (r[4] << 8 | r[5]) !== addr || r.length - 6 < n) throw new Error('beklenmeyen EEPROM cevabı @' + addr.toString(16));
    return r.slice(6, 6 + n);
  }
  /* 03 01 02 bb hh ll n -> 03 04 02 bb hh ll <data> */
  async readMemoryBlock(addr, n) {
    const r = await this.request([3, 1, 2, addr >> 16 & 0xFF, addr >> 8 & 0xFF, addr & 0xFF, n], true);
    if (r[0] !== 3 || r[1] !== 4 || r[2] !== 2 || (r[3] << 16 | r[4] << 8 | r[5]) !== addr || r.length - 6 < n) throw new Error('beklenmeyen bellek cevabı @' + addr.toString(16));
    return r.slice(6, 6 + n);
  }

  async readEeprom(onProgress, size = 2048, chunk = 64) {
    const ee = new Uint8Array(size);
    for (let a = 0; a < size; a += chunk) {
      ee.set(await this.readEepromBlock(a, chunk), a);
      if (onProgress) onProgress((a + chunk) / size);
    }
    return ee;
  }
  /* flash of the 29F010 as the CPU sees it: 0x2000..0x1FFFF (0x0000..0x1FFF is under the CPU's SFRs / RAM) */
  async readFlash(onProgress, lo = 0x2000, hi = 0x20000, chunk = 128) {
    const img = new Uint8Array(0x20000).fill(0xFF);
    for (let a = lo; a < hi; a += chunk) {
      img.set(await this.readMemoryBlock(a, chunk), a);
      if (onProgress) onProgress((a + chunk - lo) / (hi - lo));
    }
    return img;
  }
  async readHeader() { return parseHeader(await this.readMemoryBlock(0x10000, 128)); }
}

/* ------------------------------------------------------------------ helpers */
/* ROM 0x010000..: version (F_00E0E1 shows "AS V2-3"), ROM checksum, date, texts */
function parseHeader(h) {
  const txt = [];
  let s = '';
  for (let i = 9; i < h.length; i++) {
    const c = h[i];
    if (c >= 32 && c < 127) s += String.fromCharCode(c);
    else { if (s.trim().length >= 4) txt.push(s.trim()); s = ''; }
  }
  if (s.trim().length >= 4) txt.push(s.trim());
  const ver = h[2] < 100 ? String(h[2] / 10 | 0) + (h[2] % 10 ? String(h[2] % 10) : '') : '?';
  return {
    version: String.fromCharCode(h[9], h[10]) + ' ' + String.fromCharCode(h[0]) + h[1] + '-' + ver,
    romSum: h[3] << 8 | h[4],
    date: String(h[5]).padStart(2, '0') + '.' + String(h[6]).padStart(2, '0') + '.' + (h[7] | h[8] << 8),
    texts: txt.filter(t => t !== 'AS'),
  };
}

/* 16-bit sum of 0x2000..0xFFFF, checked at power on against the word at 0x010003 */
function romSum(img) { let s = 0; for (let a = 0x2000; a < 0x10000; a++) s = (s + img[a]) & 0xFFFF; return s; }

/* compare a flash image with known HEX files {name: text}; parseHex from board.js */
function identify(img, known, parseHex) {
  const out = [];
  for (const name of Object.keys(known)) {
    const m = parseHex(known[name]);
    let diff = 0, n = 0, extra = 0;
    for (const [a, v] of m) if (a >= 0x2000 && a < 0x20000) { n++; if (img[a] !== v) diff++; }
    for (let a = 0x2000; a < 0x20000; a++) if (img[a] !== 0xFF && !m.has(a)) extra++;
    out.push({ name, diff, extra, bytes: n });
  }
  out.sort((x, y) => (x.diff + x.extra) - (y.diff + y.extra));
  return out;
}

/* Motorola S2 records, 16 bytes per line, all-0xFF lines skipped (like the original files) */
function toSrec(img, lo = 0x2000, hi = 0x20000, header = 'radio') {
  const rec = (t, addrLen, addr, data) => {
    const body = [data.length + addrLen + 1];
    for (let i = addrLen - 1; i >= 0; i--) body.push(addr >> (8 * i) & 0xFF);
    body.push(...data);
    const sum = ~body.reduce((s, x) => s + x, 0) & 0xFF;
    return t + body.map(x => x.toString(16).toUpperCase().padStart(2, '0')).join('') + sum.toString(16).toUpperCase().padStart(2, '0');
  };
  const lines = [rec('S0', 2, 0, Array.from(header, c => c.charCodeAt(0)))];
  for (let a = lo; a < hi; a += 16) {
    const d = Array.from(img.slice(a, a + 16));
    if (d.some(x => x !== 0xFF)) lines.push(rec('S2', 3, a, d));
  }
  lines.push(rec('S9', 2, 0, []));
  return lines.join('\r\n') + '\r\n';
}

const api = { WebSerialPort, SimPort, RadioClient, parseHeader, romSum, identify, toSrec };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
else root.SeriIstemci = api;
})(typeof window !== 'undefined' ? window : globalThis);
