/* seri_istemci.js - PC side of the radio's serial service protocol (../seri_protokol.md):
 * link, ROM header, EEPROM backup, flash backup, firmware identification (read only),
 * writeFirmware() (flash update through the radio's loader; checkImage() first) and
 * writeEeprom() (EEPROM restore with the firmware's own write command; checkEeprom() first).
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
const BD = root.Board || (typeof require !== 'undefined' ? require('./board.js') : null);

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
    for (let t = 0; ; t++) {
      const r = await this.request([3, 1, 1, 0, addr >> 8 & 0xFF, addr & 0xFF, n], true);
      if (r[0] === 3 && r[1] === 4 && r[2] === 1 && (r[4] << 8 | r[5]) === addr && r.length - 6 >= n) return r.slice(6, 6 + n);
      if (t >= 3) throw new Error('beklenmeyen EEPROM cevabı @' + addr.toString(16));
      this.log('beklenmeyen cevap, tekrar');
    }
  }
  /* 03 01 02 bb hh ll n -> 03 04 02 bb hh ll <data> */
  async readMemoryBlock(addr, n) {
    for (let t = 0; ; t++) {
      const r = await this.request([3, 1, 2, addr >> 16 & 0xFF, addr >> 8 & 0xFF, addr & 0xFF, n], true);
      if (r[0] === 3 && r[1] === 4 && r[2] === 2 && (r[3] << 16 | r[4] << 8 | r[5]) === addr && r.length - 6 >= n) return r.slice(6, 6 + n);
      if (t >= 3) throw new Error('beklenmeyen bellek cevabı @' + addr.toString(16));
      this.log('beklenmeyen cevap, tekrar');
    }
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

  /* EEPROM restore: 03 03 01 00 aH aL <data> (the running firmware writes the 24C16 and ACKs).
   * One 16-byte page per command, page aligned; only pages that differ from `current` (the backup
   * just read) are written.  Every written page is read back; a page that does not match (the link
   * has no checksum) is written again, up to `attempts` times; then the whole EEPROM is compared.  Ends with a software reset (04 01)
   * so the firmware starts with the new settings (it keeps a copy of many of them in RAM). */
  async writeEeprom(ee, opts = {}) {
    const cur = opts.current, prog = opts.onProgress || (() => {}), attempts = opts.attempts || 3;
    const pages = [];
    for (let a = 0; a < ee.length; a += 16) {
      if (cur && cur.length === ee.length && ee.slice(a, a + 16).every((v, i) => v === cur[a + i])) continue;
      pages.push(a);
    }
    this.log('EEPROM: ' + pages.length + ' sayfa × 16 bayt yazılacak' + (cur ? ' (yalnız farklı olanlar)' : ''));
    let done = 0;
    for (const a of pages) {
      const d = Array.from(ee.slice(a, a + 16));
      for (let t = 1; ; t++) {
        await this.request([3, 3, 1, 0, a >> 8 & 0xFF, a & 0xFF].concat(d), false);
        const back = await this.readEepromBlock(a, 16);
        if (back.every((v, i) => v === d[i])) break;
        if (t >= attempts) throw new Error('EEPROM doğrulama: 0x' + a.toString(16).padStart(3, '0') + ' sayfası ' + attempts + ' denemede yazılamadı');
        this.log('0x' + a.toString(16) + ' sayfası tutmadı, yeniden yazılıyor');
      }
      prog(++done / (pages.length + 1));
    }
    /* a byte lost in the address field writes the page somewhere else, which the page's own
     * read-back cannot see: compare the whole EEPROM and rewrite every page that differs */
    for (let t = 1; ; t++) {
      const now = await this.readEeprom();
      const bad = [];
      for (let a = 0; a < ee.length; a += 16) if (!ee.slice(a, a + 16).every((v, i) => v === now[a + i])) bad.push(a);
      if (!bad.length) break;
      if (t >= attempts) throw new Error('EEPROM doğrulama: ' + bad.length + ' sayfa tutmuyor (' + attempts + ' tam okumada)');
      this.log('tam karşılaştırma: ' + bad.length + ' sayfa farklı (0x' + bad.map(a => a.toString(16)).join(', 0x') + '), yeniden yazılıyor');
      for (const a of bad) await this.request([3, 3, 1, 0, a >> 8 & 0xFF, a & 0xFF].concat(Array.from(ee.slice(a, a + 16))), false);
    }
    this.log('EEPROM tamamı doğrulandı');
    prog(1);
    if (opts.reset !== false) {
      await this.request([4, 1], false);
      this.log('reset gönderildi: telsiz yeni EEPROM ile açılıyor');
    }
    return { pages: pages.length };
  }

  /* Firmware update through the radio's own loader (../seri_protokol.md, "Firmware güncelleme"):
   *   0D 03     the firmware copies the loader to RAM 0x0080 and runs it; it erases the whole flash
   *   poll      03 01 02 00 20 00 16 until the flash reads 0xFF (status bytes while erasing)
   *   program   03 03 02 bb hh ll <128 bytes> per non-empty block; ACK after programming
   *   verify    read every block back
   *   04 01     software reset into the new firmware
   * image: a Map addr->byte that passed checkImage().  If anything breaks after the erase, the radio
   * keeps running the loader from RAM as long as it stays powered: reconnect and call this again
   * (0D 03 in the loader erases again and starts over). */
  async writeFirmware(image, opts = {}) {
    const chunk = 128, verify = opts.verify !== false;
    const phase = opts.onPhase || (() => {});
    const blks = U.blocks(image, chunk);
    const attempts = opts.attempts || 3;
    this.log('yükleme: ' + blks.length + ' blok × ' + chunk + ' bayt' + (verify ? ', doğrulamalı' : ''));
    /* the link has no checksum: a byte lost inside a write frame is programmed shifted. The final
     * read-back finds it; flash bits cannot be set back without an erase, so start over (0D 03
     * in the loader erases again). */
    for (let attempt = 1; ; attempt++) {
      try { await this.writeOnce(blks, verify, phase, opts, attempt); break; }
      catch (e) {
        if (!/^doğrulama/.test(e.message)) throw e;
        if (attempt >= attempts) throw new Error(e.message + ' (' + attempts + ' denemede de). Telsizi KAPATMAYIN: yükleyici RAM\'de çalışıyor; kabloyu kontrol edip yeniden bağlanın ve yüklemeyi tekrar başlatın');
        this.log(e.message + ' → baştan (deneme ' + (attempt + 1) + '/' + attempts + ')');
      }
    }
    phase('reset');
    await this.request([4, 1], false);
    this.log('reset gönderildi: yeni firmware açılıyor');
  }

  async writeOnce(blks, verify, phase, opts, attempt) {
    const chunk = 128, prog = opts.onProgress || (() => {});
    const total = blks.length * (verify ? 2 : 1);
    let done = 0;
    phase(attempt > 1 ? 'silme (deneme ' + attempt + ')' : 'silme');
    await this.request([0x0D, 0x03], false);
    this.log('0D 03 gönderildi: yükleyici RAM\'de, flash siliniyor');
    await this.port.sleep(300);
    const t0 = this.port.now();
    for (;;) {
      const d = await this.readMemoryBlock(0x2000, 16);
      if (d.every(x => x === 0xFF)) break;
      if (this.port.now() - t0 > (opts.eraseTimeout || 30000)) throw new Error('flash silinmedi (30 s)');
      await this.port.sleep(250);
    }
    this.log('flash silindi (' + ((this.port.now() - t0) / 1000 + 0.3).toFixed(1) + ' s)');

    phase('yazma');
    for (const b of blks) {
      await this.request([3, 3, 2, b.addr >> 16 & 0xFF, b.addr >> 8 & 0xFF, b.addr & 0xFF].concat(Array.from(b.data)), false);
      prog(++done / total);
    }
    this.log('yazıldı: ' + blks.length + ' blok');

    if (verify) {
      phase('doğrulama');
      let bad = 0;
      for (const b of blks) {
        const d = await this.readMemoryBlock(b.addr, chunk);
        for (let i = 0; i < chunk; i++) if (d[i] !== b.data[i]) { if (bad < 5) this.log('fark 0x' + (b.addr + i).toString(16) + ': ' + d[i].toString(16) + ' ≠ ' + b.data[i].toString(16)); bad++; }
        prog(++done / total);
      }
      if (bad) throw new Error('doğrulama: ' + bad + ' bayt farklı');
      this.log('doğrulandı');
    }
  }
}

/* ------------------------------------------------------------------ image checks before writing */
const LOADER_LO = 0x010100, LOADER_HI = 0x0105C8;     // copied to RAM 0x0080 by 0D 03 (F_0105C9)
/* image: Map addr->byte (parseHex).  ref: Map of a known original (YA4000S) for the loader block.
 * Returns { ok, errors, notes, image (copy, ROM checksum fixed), header, blocks } */
function checkImage(image, ref) {
  const errors = [], notes = [];
  const img = new Map();
  let outside = 0;
  for (const [a, v] of image) { if (a >= 0x2000 && a < 0x20000) img.set(a, v); else outside++; }
  if (outside) notes.push(outside + ' bayt 0x2000–0x1FFFF dışında, yazılmayacak');
  const lo = img.get(0xFFFE), hi = img.get(0xFFFF);
  if (lo === undefined || hi === undefined) errors.push('reset vektörü (0xFFFE) yok');
  else if ((hi << 8 | lo) < 0x2000) errors.push('reset vektörü geçersiz: 0x' + (hi << 8 | lo).toString(16));
  /* the loader (1224 bytes, copied to RAM 0x0080 by 0D 03) must be in the image, byte for byte as in
   * the known firmware: YA4000x 1999 at 0x010100, the 2001 builds at 0x010200; the 0D 03 code copies
   * it from there.  Without it the radio could never be updated over the serial port again. */
  let loaderAt = -1;
  if (ref) {
    const L = []; for (let a = LOADER_LO; a < LOADER_HI; a++) L.push(ref.get(a));
    const flat = new Uint8Array(0x20000).fill(0xFF); for (const [a, v] of img) flat[a] = v;
    loaderAt = findBytes(flat, L);
    if (loaderAt < 0) errors.push('seri yükleyici bulunamadı (bilinen 1224 baytlık yükleyici dosyada yok). Bu dosya yüklenirse telsiz bir daha seri porttan güncellenemez');
    else if (loaderAt !== LOADER_LO) notes.push('seri yükleyici 0x' + loaderAt.toString(16).toUpperCase() + ' adresinde (1999 sürümlerinde 0x010100)');
  } else {
    let missing = 0; for (let a = LOADER_LO; a < LOADER_HI; a++) if (!img.has(a)) missing++;
    if (missing) errors.push('seri yükleyici (0x010100–0x0105C7) eksik: ' + missing + ' bayt');
  }
  for (let a = 0x10000; a < 0x10010; a++) if (!img.has(a)) { errors.push('sürüm başlığı (0x010000) eksik'); break; }
  let header = null;
  if (!errors.length) {
    const h = []; for (let a = 0x10000; a < 0x10080; a++) h.push(img.has(a) ? img.get(a) : 0xFF);
    header = parseHeader(h);
    const c = U.checkRomChecksum(img);
    if (!c.ok) { U.fixRomChecksum(img); notes.push('ROM sağlama toplamı düzeltildi: 0x' + c.sum.toString(16).toUpperCase() + ' (dosyada 0x' + (c.stored < 0 ? 'yok' : c.stored.toString(16).toUpperCase()) + '); yoksa telsiz "ERR FLSH" verirdi'); }
    if (header) header.romSum = U.checkRomChecksum(img).sum;
  }
  return { ok: !errors.length, errors, notes, image: img, header, loaderAt, blocks: errors.length ? 0 : U.blocks(img, 128).length };
}

function findBytes(hay, needle) {
  const first = needle[0];
  outer: for (let i = 0x2000; i <= hay.length - needle.length; i++) {
    if (hay[i] !== first) continue;
    for (let j = 1; j < needle.length; j++) if (hay[i + j] !== needle[j]) continue outer;
    return i;
  }
  return -1;
}

/* EEPROM file checks before a restore.  ee: Uint8Array from the file; cur: the radio's EEPROM read
 * in this session (or null).  Returns { ok, errors, warnings, notes, ee (copy, checksums fixed), diff }
 * warnings need the user's explicit confirmation. */
function checkEeprom(ee, cur) {
  const errors = [], warnings = [], notes = [];
  if (!ee || ee.length !== 2048) return { ok: false, errors: ['dosya 2048 bayt olmalı (24C16 EEPROM yedeği); bu dosya ' + (ee ? ee.length : 0) + ' bayt'], warnings, notes };
  const img = new Uint8Array(ee);
  if (img.every(x => x === 0xFF) || img.every(x => x === 0)) errors.push('dosya boş (tamamı ' + (img[0] ? 'FF' : '00') + ')');
  /* EE 0x58B bit 1 = board type; F_00DA32 compares it with the probed hardware and shows
   * "ERR HW" / "ARZ DN" and switches the radio off when they differ */
  if (cur && ((img[0x58B] ^ cur[0x58B]) & 2)) errors.push('donanım tipi farklı (EE 0x58B bit 1: dosyada ' + (img[0x58B] >> 1 & 1) + ', telsizde ' + (cur[0x58B] >> 1 & 1) + '). Bu yedek başka tip bir telsizden; yüklenirse telsiz açılışta "ERR HW" / "ARZ DN" verip kapanır');
  if (!errors.length) {
    const sums = BD.eeCheckSums(img);
    if (!sums.every(x => x)) {
      BD.eeFixSums(img);
      notes.push('sağlama toplamı tutmayan blok düzeltildi (' + BD.EE_SUMS.filter((b, i) => !sums[i]).map(b => '0x' + b[0].toString(16).padStart(3, '0')).join(', ') + '); yoksa telsiz "ARZ PRG" verirdi');
    }
    if (cur) {
      let calDiff = 0; for (let a = 0x578; a < 0x5BD; a++) if (img[a] !== cur[a]) calDiff++;
      if (calDiff) warnings.push('kalibrasyon bloğu (0x578–0x5BC) telsizdekinden ' + calDiff + ' baytta farklı: yedek başka bir telsize ait olabilir; güç, frekans ve bant ayarları değişir');
      if (BD.eeBase(img) !== BD.eeBase(cur)) warnings.push('bant tabanı farklı: dosyada ' + (BD.eeBase(img) * 0.025).toFixed(3) + ' MHz, telsizde ' + (BD.eeBase(cur) * 0.025).toFixed(3) + ' MHz');
    }
  }
  let diff = null;
  if (cur && cur.length === 2048) { diff = 0; for (let a = 0; a < 2048; a++) if (img[a] !== cur[a]) diff++; }
  return { ok: !errors.length, errors, warnings, notes, ee: img, diff };
}

/* bytes that would change on the radio (img = flash backup Uint8Array, image = Map after checkImage) */
function diffCount(img, image) {
  let n = 0;
  for (let a = 0x2000; a < 0x20000; a++) { const v = image.has(a) ? image.get(a) : 0xFF; if (img[a] !== v) n++; }
  return n;
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

/* flash görüntüsünün parmak izi (FNV-1a 32, 0x2000..0x1FFFF): sayfada gömülü olmayan orijinal firmware'leri
 * tanımak için (mkfirmware.py --kimlik -> window.FIRMWARE_KIMLIK) */
function kimlik(img) {
  let h = 0x811C9DC5;
  for (let a = 0x2000; a < 0x20000; a++) { h ^= img[a] === undefined ? 0xFF : img[a]; h = Math.imul(h, 0x01000193) >>> 0; }
  return h.toString(16).padStart(8, '0');
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

const api = { WebSerialPort, SimPort, RadioClient, parseHeader, romSum, identify, kimlik, toSrec, checkImage, checkEeprom, diffCount, LOADER_LO, LOADER_HI };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
else root.SeriIstemci = api;
})(typeof window !== 'undefined' ? window : globalThis);
