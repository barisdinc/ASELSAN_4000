/* updater.js - PC side of the serial firmware update, run against the simulated
 * radio (or as a reference for a real PC tool).  Protocol: ../seri_protokol.md
 *
 *   frame  16 10 02 <body, 0x10 doubled> 10 03 00 00
 *   body   01                         connect (sequence numbers back to 1)
 *          02 nn 01 00                ACK, nn = next expected sequence number
 *          04 ss 01 00 <cmd> <sub>..  data, ss = sequence number
 *
 * Steps:
 *   1. connect, then data "0D 03": the firmware (F_0105C9) copies the loader
 *      from ROM 0x010100..0x0105C7 to RAM 0x0080 and runs it; the loader
 *      erases the whole flash first (no wait for the erase to finish)
 *   2. poll "03 01 02 bb hh ll n" (read) until the flash reads 0xFF: during the
 *      erase the 29F010 returns status bytes instead of data
*   3. "03 03 02 bb hh ll <data>" per block (all-0xFF blocks are skipped);
 *      the loader ACKs after programming
 *   4. optional read-back verify, then "04 01" = software reset into the new code
 * Before sending, the ROM checksum at 0x010003 is checked and (opts.fixSum, default on)
 * corrected, because a modified image otherwise stops at boot with "ERR FLSH".
 *
 * Usage: const u = new FlashUpdater(board, image, opts); then after every
 * board.run() slice call u.tick(); u.done / u.error / u.progress / u.log.
 * image: Map addr->byte (parseHex) or {addr: Uint8Array}.  Works in browser, node, jsc.
 */
(function (root) {
'use strict';
const SYN = 0x16, DLE = 0x10, STX = 0x02, ETX = 0x03;

function frame(body) {
  const r = [SYN, DLE, STX];
  for (const x of body) { r.push(x); if (x === DLE) r.push(DLE); }
  return r.concat([DLE, ETX, 0, 0]);
}

/* receiver: same state machine as L_019469 / the RAM loader */
class FrameParser {
  constructor(onFrame) { this.st = 1; this.buf = []; this.onFrame = onFrame; }
  push(b) {
    switch (this.st) {
      case 1: if (b === SYN) this.st = 2; this.buf = []; break;
      case 2: this.st = b === DLE ? 3 : 1; break;
      case 3: this.st = b === STX ? 4 : 1; break;
      case 4: if (b === DLE) this.st = 5; else this.buf.push(b); break;
      case 5:
        if (b === DLE) { this.buf.push(b); this.st = 4; }
        else if (b === ETX) this.st = 6;
        else this.st = 1;
        break;
      case 6: this.st = 7; break;
      case 7: this.st = 1; this.onFrame(this.buf); break;
    }
  }
}

/* image -> sorted list of {addr, data} blocks of `size` bytes, aligned, non-0xFF only */
function blocks(image, size, lo = 0x2000, hi = 0x20000) {
  const mem = new Uint8Array(hi).fill(0xFF), used = new Uint8Array(hi);
  if (image instanceof Map) { for (const [a, v] of image) if (a >= lo && a < hi) { mem[a] = v; used[a] = 1; } }
  else for (const k of Object.keys(image)) { const a0 = +k, d = image[k]; for (let i = 0; i < d.length; i++) if (a0 + i >= lo && a0 + i < hi) { mem[a0 + i] = d[i]; used[a0 + i] = 1; } }
  const out = [];
  for (let a = lo; a < hi; a += size) {
    let any = false; for (let i = 0; i < size; i++) if (used[a + i] && mem[a + i] !== 0xFF) { any = true; break; }
    if (any) out.push({ addr: a, data: mem.slice(a, a + size) });
  }
  return out;
}

/* boot check (0x00E3FA..0x00E48C): 16-bit sum of the bytes 0x2000..0xFFFF must
 * equal the big-endian word at 0x010003, otherwise "ERR FLSH" and power off.
 * Bank 1 (0x10000..) is not covered.  Works on a Map addr->byte (missing = 0xFF). */
function romChecksum(image) {
  let s = 0;
  for (let a = 0x2000; a < 0x10000; a++) { const v = image.get(a); s = (s + (v === undefined ? 0xFF : v)) & 0xFFFF; }
  return s;
}
function checkRomChecksum(image) {
  const sum = romChecksum(image), hi = image.get(0x10003), lo = image.get(0x10004);
  const stored = hi === undefined || lo === undefined ? -1 : hi << 8 | lo;
  return { sum, stored, ok: sum === stored };
}
function fixRomChecksum(image) {
  const c = checkRomChecksum(image);
  if (!c.ok) { image.set(0x10003, c.sum >> 8); image.set(0x10004, c.sum & 0xFF); }
  return c;
}

class FlashUpdater {
  constructor(board, image, opts = {}) {
    this.b = board; this.fclk = opts.fclk || 4032000;
    this.chunk = opts.chunk || 128;                 // data bytes per frame (frame body <= 139)
    this.verify = opts.verify !== false;
    this.timeout = opts.timeout || 2.0;             // seconds to wait for an answer
    this.eraseTimeout = opts.eraseTimeout || 30;
    this.log = []; this.onLog = opts.onLog || null;
    let sumNote = null;
    if (image instanceof Map) {
      const c = checkRomChecksum(image);
      if (!c.ok && opts.fixSum !== false) { image = new Map(image); fixRomChecksum(image); }
      sumNote = c.ok ? 'ROM sağlama toplamı doğru (0x' + c.sum.toString(16) + ')'
        : (opts.fixSum !== false ? 'ROM sağlama toplamı düzeltildi: 0x010003 = ' + c.sum.toString(16) + ' (eski ' + (c.stored < 0 ? 'yok' : c.stored.toString(16)) + ')'
          : 'UYARI: ROM sağlama toplamı yanlış (hesap ' + c.sum.toString(16) + ', kayıtlı ' + (c.stored < 0 ? 'yok' : c.stored.toString(16)) + '); telsiz "ERR FLSH" verip kapanır');
    }
    this.blocks = blocks(image, this.chunk);
    this.total = this.blocks.length * (this.verify ? 2 : 1);
    this.count = 0; this.progress = 0; this.done = false; this.error = null;
    this.seq = 1; this.rxq = [];
    this.parser = new FrameParser(f => this.rxq.push(f));
    const prev = board.onSerial;
    this.restore = () => { board.onSerial = prev; };
    board.onSerial = v => { this.parser.push(v); if (prev) prev(v); };
    this.job = this.run();
    this.wait = null;
    this.say('güncelleme: ' + this.blocks.length + ' blok × ' + this.chunk + ' bayt');
    if (sumNote) this.say(sumNote);
  }
  say(s) { const t = (this.b.now / this.fclk).toFixed(2) + ' s  ' + s; this.log.push(t); if (this.onLog) this.onLog(t); }
  send(body) { this.b.serialIn(frame(body)); }
  now() { return this.b.now / this.fclk; }

  /* drive the coroutine; call after each simulation slice */
  tick() {
    if (this.done) return;
    for (let guard = 0; guard < 100; guard++) {
      if (this.wait) {
        const w = this.wait;
        if (w.kind === 'frame') {
          let f;
          while ((f = this.rxq.shift())) {
            if (f[0] === 4) this.send([2, (f[1] + 1) & 0xFF, 1, 0]);      // ACK every data frame of the radio
            if (w.match(f)) { this.wait = null; this.resume(f); break; }
          }
          if (this.wait && this.now() > w.until) { this.wait = null; this.resume(null); }
        } else if (this.now() >= w.until) { this.wait = null; this.resume(undefined); }
        if (this.wait || this.done) return;
      } else this.resume(undefined);
      if (this.done) return;
    }
  }
  resume(v) {
    try {
      const r = this.job.next(v);
      if (r.done) { this.finish(null); return; }
      this.wait = r.value;
    } catch (e) { this.finish(e.message || String(e)); }
  }
  finish(err) {
    this.done = true; this.error = err; this.restore();
    this.say(err ? 'HATA: ' + err : 'güncelleme tamamlandı');
  }

  waitFrame(match, sec) { return { kind: 'frame', match, until: this.now() + (sec || this.timeout) }; }
  sleep(sec) { return { kind: 'time', until: this.now() + sec }; }

  /* data frame with stop-and-wait; the ACK carries the next expected sequence number */
  *request(payload, wantReply) {
    for (let tries = 0; tries < 4; tries++) {
      this.rxq.length = 0;
      this.send([4, this.seq, 1, 0].concat(payload));
      const ack = yield this.waitFrame(f => f[0] === 2);
      if (!ack) { this.say('cevap yok, tekrar (' + (tries + 1) + ')'); continue; }
      if (ack[1] !== ((this.seq + 1) & 0xFF)) {             // out of step: take the radio's number
        this.say('sıra no. düzeltildi ' + this.seq + ' → ' + ack[1]); this.seq = ack[1]; continue;
      }
      this.seq = ack[1];
      if (!wantReply) return true;
      const rep = yield this.waitFrame(f => f[0] === 4);
      if (rep) return rep.slice(4);
      this.say('veri cevabı yok, tekrar');
    }
    throw new Error('telsiz cevap vermiyor (' + payload.slice(0, 2).map(x => x.toString(16)).join(' ') + ')');
  }

  readCmd(addr, n) { return [3, 1, 2, addr >> 16 & 0xFF, addr >> 8 & 0xFF, addr & 0xFF, n]; }

  *run() {
    /* 1. link + enter the loader */
    let ok = false;
    for (let i = 0; i < 5 && !ok; i++) {
      this.rxq.length = 0; this.send([1]);
      const a = yield this.waitFrame(f => f[0] === 2);
      if (a) { ok = true; this.seq = 1; }
    }
    if (!ok) throw new Error('bağlantı kurulamadı (telsiz açık mı, 9600 baud servis protokolü çalışıyor mu?)');
    this.say('bağlandı; 0D 03 → yükleyici');
    yield* this.request([0x0D, 0x03], false);

    /* 2. wait for the chip erase: the loader runs from RAM and erases at once */
    yield this.sleep(0.3);
    const t0 = this.now();
    for (;;) {
      const d = yield* this.request(this.readCmd(0x2000, 16), true);
      const data = d.slice(6);
      if (data.length === 16 && data.every(x => x === 0xFF)) break;
      if (this.now() - t0 > this.eraseTimeout) throw new Error('flash silinmedi');
      yield this.sleep(0.25);
    }
    this.say('flash silindi (' + (this.now() - t0 + 0.3).toFixed(1) + ' s)');

    /* 3. program */
    for (const blk of this.blocks) {
      yield* this.request([3, 3, 2, blk.addr >> 16 & 0xFF, blk.addr >> 8 & 0xFF, blk.addr & 0xFF].concat(Array.from(blk.data)), false);
      this.count++; this.progress = this.count / this.total;
    }
    this.say('yazıldı: ' + this.blocks.length + ' blok');

    /* 4. verify */
    if (this.verify) {
      let bad = 0;
      for (const blk of this.blocks) {
        const d = yield* this.request(this.readCmd(blk.addr, this.chunk), true);
        const data = d.slice(6);
        for (let i = 0; i < this.chunk; i++) if (data[i] !== blk.data[i]) { bad++; if (bad < 5) this.say('fark 0x' + (blk.addr + i).toString(16) + ': ' + data[i] + ' ≠ ' + blk.data[i]); }
        this.count++; this.progress = this.count / this.total;
      }
      if (bad) throw new Error('doğrulama: ' + bad + ' bayt farklı');
      this.say('doğrulandı');
    }

    /* 5. start the new firmware */
    yield* this.request([4, 1], false);
    this.say('reset gönderildi');
  }
}

const api = { FlashUpdater, FrameParser, frame, blocks, romChecksum, checkRomChecksum, fixRomChecksum };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
else root.Updater = api;
})(typeof window !== 'undefined' ? window : globalThis);
