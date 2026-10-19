/* board.js - K3 control board model around the M37732 (ASELSAN 4000).
 *
 * Internal peripherals: ports, Timer A0-A4 / B0-B2, interrupt control,
 * UART0 (RS232) / UART1 (clock-synchronous: MB1502 + C-BUS), A-D, WDT,
 * software reset.  External devices: 29F010 flash, I2C bus (P61 SCL,
 * P62 SDA) with 24C16 EEPROM, PCF8576 LCD driver, two PCF8574 expanders
 * (0x42 keypad matrix, 0x46 channel switch / buttons), MB1502 PLL.
 *
 * Pin assignment: maintenance manual 3.6.2.3, see ../donanim_haritasi.md.
 * Things not known from the documents are marked "assumption".
 */
(function (root) {
'use strict';
const M7700 = (typeof module !== 'undefined' && module.exports) ? require('./m7700.js') : root.M7700;
const FCLK = 4032000;            // phi = f(XIN)/2, 8.064 MHz crystal; also timer source f2

/* ------------------------------------------------------------ firmware file */
function parseHex(text) {
  const mem = new Map();
  for (let line of text.split(/\r?\n/)) {
    line = line.trim();
    if (!line) continue;
    if (line[0] === 'S') {                       // Motorola S-record
      const t = line[1], n = parseInt(line.substr(2, 2), 16);
      const al = { '1': 2, '2': 3, '3': 4 }[t];
      if (!al) continue;
      const addr = parseInt(line.substr(4, al * 2), 16);
      const cnt = n - al - 1;
      for (let i = 0; i < cnt; i++) mem.set(addr + i, parseInt(line.substr(4 + al * 2 + i * 2, 2), 16));
    } else if (line[0] === ':') {                // Intel HEX
      if (!root._ihexBase) root._ihexBase = 0;
      const n = parseInt(line.substr(1, 2), 16), addr = parseInt(line.substr(3, 4), 16);
      const type = parseInt(line.substr(7, 2), 16);
      if (type === 0) for (let i = 0; i < n; i++) mem.set(root._ihexBase + addr + i, parseInt(line.substr(9 + i * 2, 2), 16));
      else if (type === 2) root._ihexBase = parseInt(line.substr(9, 4), 16) << 4;
      else if (type === 4) root._ihexBase = parseInt(line.substr(9, 4), 16) << 16;
      else if (type === 1) root._ihexBase = 0;
    }
  }
  return mem;
}

/* ------------------------------------------------------------ I2C devices */
class EEPROM24C16 {
  constructor() { this.mem = new Uint8Array(2048).fill(0xFF); this.ptr = 0; this.block = 0; this.wcount = 0; this.dirty = false; }
  match(a) { return (a & 0xF0) === 0xA0; }
  start(a) { this.block = (a >> 1) & 7; this.wcount = 0; this.read = a & 1; this.ptr = (this.block << 8) | (this.ptr & 0xFF); }
  write(b) {
    if (this.wcount++ === 0) { this.ptr = (this.block << 8) | b; return true; }
    const page = this.ptr & ~0x0F;
    this.mem[this.ptr] = b; this.dirty = true;
    this.ptr = page | ((this.ptr + 1) & 0x0F);
    return true;
  }
  readByte() { if (this.onRead) this.onRead(this.ptr); const v = this.mem[this.ptr]; this.ptr = (this.ptr + 1) & 0x7FF; return v; }
}

class PCF8574 {
  constructor(addr, name) { this.addr = addr; this.name = name; this.latch = 0xFF; this.ext = 0xFF; this.int = false; this.lastRead = 0xFF; }
  match(a) { return (a & 0xFE) === this.addr; }
  start() {}
  write(b) { this.latch = b; this.clearInt(); this.lastRead = this.pins(); return true; }
  pins() { return this.latch & this.ext; }      // quasi-bidirectional, buttons pull low
  readByte() { this.clearInt(); this.lastRead = this.pins(); return this.lastRead; }
  // /INT is released by any read or write; the shared INT1 line must follow,
  // otherwise it stays low and the next change gives no falling edge
  clearInt() { this.int = false; if (this.board) this.board.updateIntLines(); }
  setExt(v, board) {
    const before = this.pins(); this.ext = v;
    if (this.pins() !== before && this.pins() !== this.lastRead) { this.int = true; board.updateIntLines(); }
  }
}

/* 4x4 keypad on the 0x42 expander: rows P4..P7, columns P0..P3.
 * Scan in the original: write F0, read row; write 0F, read column;
 * key = ROM 0x31EC[col*4+row] ("*0#q 789p 456s 123m"). */
class KeypadExpander extends PCF8574 {
  constructor(addr, name) { super(addr, name); this.keys = new Set(); }
  pins() {
    let p = this.latch & this.ext;
    for (const k of this.keys) {
      const row = 4 + (k & 3), col = 3 - (k >> 2);
      if (!(p >> row & 1) || !(p >> col & 1)) p &= ~(1 << row | 1 << col);
    }
    return p;
  }
  press(code, down, board) {
    const before = this.pins();
    if (down) this.keys.add(code); else this.keys.delete(code);
    if (this.pins() !== before) { this.int = true; board.updateIntLines(); }
  }
}

/* PCF8576 1:4 multiplex LCD driver; 40 segment outputs x 4 backplanes = 20 bytes */
class PCF8576 {
  constructor() { this.ram = new Uint8Array(20); this.ptr = 0; this.cmd = true; this.enabled = false; this.blink = 0; this.writes = 0; }
  match(a) { return (a & 0xFE) === 0x70; }
  start() { this.cmd = true; }
  write(b) {
    if (this.cmd) {
      const c = b & 0x80;
      if ((b & 0x40) === 0) this.ptr = b & 0x3F;                 // load data pointer
      else if ((b & 0x60) === 0x40) this.enabled = !!(b & 0x08); // mode set
      else if ((b & 0x78) === 0x70) this.blink = b & 3;          // blink
      // device select (0x60) and bank select (0x78) ignored
      if (!c) this.cmd = false;
      return true;
    }
    this.ram[(this.ptr >> 1) % 20] = b;
    this.ptr = (this.ptr + 2) % 40;
    this.writes++;
    return true;
  }
  readByte() { return 0xFF; }
}

class I2CBus {
  constructor(devs) { this.devs = devs; this.scl = 1; this.sdaM = 1; this.state = 'idle'; this.bits = 0; this.nb = 0; this.dev = null; this.slaveSda = 1; this.log = []; }
  /* called whenever the master lines change */
  update(scl, sdaM) {
    if (this.scl && scl && this.sdaM !== sdaM) {
      if (!sdaM) { this.state = 'addr'; this.nb = 0; this.bits = 0; this.slaveSda = 1; }   // START
      else { this.state = 'idle'; this.slaveSda = 1; this.dev = null; }                    // STOP
    } else if (!this.scl && scl) this.rise(sdaM & this.slaveSda);
    else if (this.scl && !scl) this.fall();
    this.scl = scl; this.sdaM = sdaM;
  }
  rise(sda) {
    if (this.state === 'addr' || this.state === 'wr') { this.bits = (this.bits << 1) | sda; this.nb++; }
    else if (this.state === 'rd') { this.nb++; }
    else if (this.state === 'rdack') { this.masterAck = !sda; }
  }
  fall() {
    if (this.state === 'addr' && this.nb === 8) {
      const a = this.bits & 0xFF;
      this.dev = this.devs.find(d => d.match(a)) || null;
      this.log.push('S' + a.toString(16));
      if (this.log.length > 64) this.log.shift();
      if (this.dev) { this.dev.start(a); this.slaveSda = 0; this.state = (a & 1) ? 'ackr' : 'ack'; }
      else { this.slaveSda = 1; this.state = 'nack'; }
    } else if (this.state === 'wr' && this.nb === 8) {
      const ok = this.dev.write(this.bits & 0xFF);
      this.slaveSda = ok ? 0 : 1; this.state = 'ack';
    } else if (this.state === 'ack') { this.slaveSda = 1; this.state = 'wr'; this.nb = 0; this.bits = 0; }
    else if (this.state === 'ackr') { this.loadRead(); }
    else if (this.state === 'rd' && this.nb < 8) { this.slaveSda = this.rdByte >> (7 - this.nb) & 1; }
    else if (this.state === 'rd' && this.nb === 8) { this.slaveSda = 1; this.state = 'rdack'; }
    else if (this.state === 'rdack') { if (this.masterAck) this.loadRead(); else { this.state = 'nack'; this.slaveSda = 1; } }
  }
  loadRead() { this.rdByte = this.dev.readByte(); this.nb = 0; this.state = 'rd'; this.slaveSda = this.rdByte >> 7 & 1; }
}

/* ------------------------------------------------------------ MB1502 */
class MB1502 {
  constructor() { this.bits = []; this.R = 0; this.SW = 0; this.Nc = 0; this.Ac = 0; this.lockAt = 0; this.loads = 0; }
  shiftByte(b) { for (let i = 0; i < 8; i++) this.bits.push(b >> i & 1); if (this.bits.length > 64) this.bits.splice(0, this.bits.length - 64); }  // UART: LSB first
  latch(now) {
    const b = this.bits; if (b.length < 16) return;
    const c = b[b.length - 1];
    if (c === 1) {                         // reference word: SW R14..R1 C
      const w = b.slice(-16); this.SW = w[0]; let r = 0; for (let i = 1; i <= 14; i++) r = r << 1 | w[i]; this.R = r;
    } else if (b.length >= 19) {           // divider word: N11..N1 A7..A1 C
      const w = b.slice(-19); let n = 0, a = 0;
      for (let i = 0; i < 11; i++) n = n << 1 | w[i];
      for (let i = 11; i < 18; i++) a = a << 1 | w[i];
      this.Nc = n; this.Ac = a; this.loads++;
      this.lockAt = now + FCLK * 0.004;    // assumption: 4 ms lock time
    }
    this.bits = [];
  }
  prescaler() { return this.SW ? 64 : 128; }        // MB1502: SW=1 64/65, SW=0 128/129
  vcoHz(fref) { return this.R ? (this.prescaler() * this.Nc + this.Ac) * fref / this.R : 0; }
}

/* Frequency plan of the original firmware (F_00DA32, F_0088B8, F_008854, F_0087DE):
 *   unit 2.5 kHz; base (EEPROM 0x58C..0x58D, big-endian) in 25 kHz units
 *   f = (base*10 + word14) * 2.5 kHz          word14 = 14-bit channel word
 *   RX VCO = f + 45 MHz (base < 250 MHz, prescaler 64) or f - 45 MHz (prescaler 128)
 *   TX VCO = f
 *   R from the reference (12.8 MHz, or 13.0 MHz when EEPROM 0x58F != 0):
 *   step 12.5 kHz if word14 % 5 == 0, 10 kHz if % 4 == 0, else 5 kHz. */
const UNIT = 2500;
function freqToWord(f, base) { return Math.round(f / UNIT) - base * 10; }
function wordToFreq(w, base) { return (base * 10 + w) * UNIT; }
/* ------------------------------------------------------------ signal sources
 * Inputs P65 (CTCSS, TB0IN) and P66 (selcall, TB1IN) are square waves
 * computed from simulated time; a tone list gives a selcall sequence. */
class Signal {
  constructor() { this.segs = []; this.noise = false; this.nextNoise = 0; this.noiseLvl = 0; }
  setTone(f) { this.segs = f ? [{ t0: 0, t1: Infinity, f }] : []; }
  setSequence(now, list) {           // list: [{f, ms}]
    let t = now; this.segs = [];
    for (const s of list) { const d = s.ms * FCLK / 1000; if (s.f) this.segs.push({ t0: t, t1: t + d, f: s.f }); t += d; }
    this.seqEnd = t;
  }
  level(t) {
    for (const s of this.segs) if (t >= s.t0 && t < s.t1) return Math.floor((t - s.t0) * 2 * s.f / FCLK) & 1;
    if (this.noise) {
      while (t >= this.nextNoise) { this.noiseLvl ^= 1; this.nextNoise += FCLK * (0.00005 + Math.random() * 0.0004); }
      return this.noiseLvl;
    }
    return 1;
  }
}

/* ------------------------------------------------------------ timers */
const DIV = [1, 8, 32, 256];         // f2, f16, f64, f512 in phi cycles
class Timer {
  constructor(board, name, reg, mod, ic, bit, isB) {
    Object.assign(this, { board, name, reg, mod, ic, bit, isB });
    this.counter = 0; this.reload = 0; this.acc = 0; this.running = false; this.out = 0; this.armed = false;
    this.lastIn = 1; this.meas = 0; this.underflows = 0;
  }
  mode() { return this.board.sfr[this.mod] & 3; }
  write(v) {                                 // 16-bit write to the count register
    this.reload = v;
    const md = this.mode();
    if (!this.running || (!this.isB && md === 1)) this.counter = v;
    if (!this.isB && md === 3) this.pwmLoad = true;
    if (this.board.onTimerWrite) this.board.onTimerWrite(this, v);
  }
  read() {
    if (this.isB && this.mode() === 2) return this.reload;  // measurement: last measured value
    return this.counter & 0xFFFF;
  }
  start(on) {
    if (on && !this.running) { this.acc = 0; if (!this.isB && this.mode() === 3) this.pwmStart(); }
    if (!on && this.running && !this.isB && this.mode() !== 1) this.out = 0;
    this.running = on;
    if (!on) this.armed = false;
  }
  trigger() {                                // one-shot start (TOSTART or software)
    if (!this.running) return;
    this.counter = this.reload; this.armed = true; this.out = 1; this.acc = 0;
  }
  irq() { this.board.sfr[this.ic] |= 0x08; this.board.irqDirty = true; }
  pwmStart() { this.pwmPhase = 0; this.counter = 0; this.out = 1; }
  tick(dc, now) {
    const sfr = this.board.sfr, md = sfr[this.mod];
    const mode = md & 3;
    if (this.isB && (mode === 1 || mode === 2)) return this.tickEdge(dc, now, md);
    if (!this.isB && mode === 1) {            // event counter on TAiIN (only TA3IN = P57 squelch is wired)
      const lv = this.board.taInput(this.name);
      if (lv === null) return;
      const edge = md & 0x08 ? (this.lastIn === 0 && lv === 1) : (this.lastIn === 1 && lv === 0);   // bit 3: 1 rising, 0 falling
      this.lastIn = lv;
      if (edge) { if (this.counter === 0) { this.counter = this.reload; this.irq(); } else this.counter--; }
      return;
    }
    this.acc += dc;
    const d = DIV[md >> 6 & 3];
    if (this.acc < d) return;
    let n = Math.floor(this.acc / d); this.acc -= n * d;
    if (!this.isB && mode === 2) {           // one-shot
      if (!this.armed) return;
      if (n > this.counter) { this.counter = 0; this.armed = false; this.out = 0; this.irq(); }
      else this.counter -= n;
      return;
    }
    if (!this.isB && mode === 3) {           // PWM
      const eight = md & 0x20;
      const m = eight ? (this.reload & 0xFF) + 1 : 1;
      const hi = eight ? (this.reload >> 8) * m : this.reload;
      const per = eight ? 255 * m : 65535;
      this.pwmPhase = (this.pwmPhase || 0) + n;
      while (this.pwmPhase >= per) { this.pwmPhase -= per; this.irq(); }
      this.out = this.pwmPhase < hi ? 1 : 0;
      this.duty = per ? hi / per : 0;
      return;
    }
    // timer mode: underflow every reload+1 counts
    if (n <= this.counter) { this.counter -= n; return; }
    n -= this.counter + 1;
    const per = this.reload + 1;
    this.counter = this.reload - (n % per);
    this.underflows += 1 + Math.floor(n / per);
    this.irq();
  }
  /* TB event counter / pulse measurement on TBiIN */
  tickEdge(dc, now, md) {
    const lv = this.board.tbInput(this.name, now);
    const mode = md & 3, sel = md >> 2 & 3;
    const fall = this.lastIn === 1 && lv === 0, rise = this.lastIn === 0 && lv === 1;
    this.lastIn = lv;
    if (mode === 1) {
      const edge = sel === 0 ? fall : sel === 1 ? rise : (fall || rise);
      if (edge) { if (this.counter === 0) { this.counter = this.reload; this.irq(); } else this.counter--; }
      return;
    }
    this.acc += dc;
    const d = DIV[md >> 6 & 3];
    if (this.acc >= d) { const n = Math.floor(this.acc / d); this.acc -= n * d; this.meas += n; if (this.meas > 0xFFFF) { this.meas = 0xFFFF; this.board.sfr[this.mod] |= 0x20; } }
    const edge = sel === 0 ? fall : sel === 1 ? rise : (fall || rise);
    if (edge) { this.reload = this.meas | 0; this.meas = 0; this.irq(); }
  }
}

/* ------------------------------------------------------------ UARTs */
class UART {
  constructor(board, n) {
    this.board = board; this.n = n; this.base = n ? 0x38 : 0x30; this.ticA = n ? 0x73 : 0x71; this.ricA = n ? 0x74 : 0x72;
    this.tb = -1; this.shift = -1; this.shiftEnd = 0; this.rb = 0; this.rxq = []; this.nextRx = 0;
    this.out = [];
  }
  charCycles() {
    const s = this.board.sfr, mod = s[this.base], brg = s[this.base + 1], c0 = s[this.base + 4];
    const d = DIV[c0 & 3] * (brg + 1);
    return (mod & 7) === 1 ? 8 * 2 * d : 10 * 16 * d;
  }
  c1() { return this.board.sfr[this.base + 5]; }
  writeTB(v) {
    if (!(this.c1() & 1)) return;            // TE off
    if (this.shift < 0) this.startShift(v); else this.tb = v;
  }
  startShift(v) {
    this.shift = v; this.shiftEnd = this.board.now + this.charCycles();
    this.board.sfr[this.ticA] |= 0x08; this.board.irqDirty = true;   // TB empty -> TX interrupt request
  }
  tick(now) {
    if (this.shift >= 0 && now >= this.shiftEnd) {
      const v = this.shift; this.shift = -1;
      const rx = this.board.uartByte(this.n, v);
      if ((this.board.sfr[this.base] & 7) === 1 && (this.c1() & 4)) {   // synchronous: receive while sending
        this.rb = rx === undefined ? 0xFF : rx;
        this.board.sfr[this.base + 5] |= 0x08;
        this.board.sfr[this.ricA] |= 0x08; this.board.irqDirty = true;
      }
      if (this.tb >= 0) { const t = this.tb; this.tb = -1; this.startShift(t); }
    }
    if (this.rxq.length && now >= this.nextRx && (this.c1() & 4)) {
      this.rb = this.rxq.shift();
      this.board.sfr[this.base + 5] |= 0x08;  // RI
      this.board.sfr[this.ricA] |= 0x08; this.board.irqDirty = true;
      this.nextRx = now + this.charCycles();
    }
  }
  readC0() { return (this.board.sfr[this.base + 4] & ~0x08) | (this.shift < 0 && this.tb < 0 ? 0x08 : 0); }
  readC1() { return (this.board.sfr[this.base + 5] & ~0x02) | (this.tb < 0 ? 0x02 : 0); }
  readRB() { this.board.sfr[this.base + 5] &= ~0x08; return this.rb; }
}

/* ------------------------------------------------------------ the board */
class Board {
  constructor(romMap) {
    this.rom = new Uint8Array(0x20000).fill(0xFF);
    this.loadRom(romMap);
    this.ram = new Uint8Array(0x880);
    this.sfr = new Uint8Array(0x80);
    this.cpu = new M7700.CPU(this);
    this.eeprom = new EEPROM24C16();
    this.lcd = new PCF8576();
    this.keypad = new KeypadExpander(0x42, 'keypad');
    this.panel = new PCF8574(0x46, 'panel');
    this.keypad.board = this.panel.board = this;
    this.i2c = new I2CBus([this.eeprom, this.lcd, this.keypad, this.panel]);
    this.synth = new MB1502();
    this.ctcssIn = new Signal(); this.selIn = new Signal();
    this.uart = [new UART(this, 0), new UART(this, 1)];
    this.analog = [0, 0, 0, 128, 180, 20, 0, 0];   // AN0..AN7 raw 8-bit values (assumption on scaling)
    this.inputs = { P4: 0xFF, P5: 0x7F, P6: 0xFF, P7: 0xFF, P8: 0xFF };   // P57 = 0: squelch closed (no signal)
    this.lockFault = false; this.lockModel = 'low';
    this.hwArac = false;   // true: araç / sabit merkez kartı (EE 0x58B bit 1), P60 SDA hattına bağlı
    this.autoPower = true; this.powerGain = 1.0; this.reflRatio = 0.05;
    this.events = [];      // human readable log
    this.cbusLog = [];
    this.serialOut = [];   // bytes from UART0
    this.audio = { ta1: [], ta2: [], beep: [] };
    this.beepHz = 0;
    this.flashState = 0;
    this.romWrites = 0;
    // 29F010 embedded algorithms: erase takes time, during it reads give status
    // (DQ7 = 0, DQ6 toggles, DQ3 = 1) and writes are ignored
    this.flashBusyUntil = 0; this.flashToggle = 0;
    this.flashEraseSec = 1.0;      // chip erase time (Am29F010 typ. ~1 s per sector group; set 0 for instant)
    this.breakpoints = new Set();
    this.watch = null;
    this.mkTimers();
    this.hardReset();
  }
  loadRom(map) {
    this.rom.fill(0xFF);
    for (const [a, v] of map) if (a < 0x20000) this.rom[a] = v;
  }
  mkTimers() {
    const T = [];
    for (let i = 0; i < 5; i++) T.push(new Timer(this, 'TA' + i, 0x46 + 2 * i, 0x56 + i, 0x75 + i, i, false));
    for (let i = 0; i < 3; i++) T.push(new Timer(this, 'TB' + i, 0x50 + 2 * i, 0x5B + i, 0x7A + i, 5 + i, true));
    this.timers = T; this.timerAt = {};
    for (const t of T) { this.timerAt[t.reg] = t; this.timerAt[t.reg + 1] = t; }
  }
  hardReset() {
    this.sfr.fill(0);
    this.latch = { 0x0A: 0, 0x0B: 0, 0x0E: 0, 0x0F: 0, 0x12: 0 };
    for (const t of this.timers) { t.running = false; t.counter = t.reload = 0; t.armed = false; t.meas = 0; t.out = 0; }
    this.now = this.now || 0;
    this.wdt = 0; this.int1Lvl = 1;
    this.irqDirty = true;
    this.cpu.reset();
    this.events.push([this.now, 'RESET']);
  }

  trap(name, pc) { this.events.push([this.now, name + ' @' + pc.toString(16).toUpperCase().padStart(6, '0')]); }

  /* ---------------- bus */
  rd(a) {
    if (a < 0x80) return this.sfrRead(a);
    if (a < 0x880) return this.ram[a];
    if (a < 0x2000) return 0xFF;
    if (a < 0x20000) {
      if (this.now < this.flashBusyUntil) { this.flashToggle ^= 0x40; return 0x08 | this.flashToggle; }
      return this.rom[a];
    }
    return 0xFF;
  }
  wr(a, v) {
    if (a < 0x80) return this.sfrWrite(a, v);
    if (a < 0x880) { this.ram[a] = v; if (this.ramHook) this.ramHook(a, v); if (this.watch && this.watch.has(a)) this.watchHit = a; return; }
    if (a >= 0x2000 && a < 0x20000) return this.flashWrite(a, v);
  }
  flashWrite(a, v) {
    const ca = a & 0x7FFF;   // assumption: unlock addresses decoded on A0-A14
    const st = this.flashState;
    if (this.now < this.flashBusyUntil) return;
    if (st === 3) { this.rom[a] &= v; this.romWrites++; this.flashState = 0; return; }   // program: any value, 0xF0 too
    if (v === 0xF0) { this.flashState = 0; return; }
    if (st === 0 && ca === 0x5555 && v === 0xAA) this.flashState = 1;
    else if (st === 1 && ca === 0x2AAA && v === 0x55) this.flashState = 2;
    else if (st === 2 && ca === 0x5555 && v === 0xA0) this.flashState = 3;
    else if (st === 2 && ca === 0x5555 && v === 0x80) this.flashState = 4;
    else if (st === 4 && ca === 0x5555 && v === 0xAA) this.flashState = 5;
    else if (st === 5 && ca === 0x2AAA && v === 0x55) this.flashState = 6;
    else if (st === 6 && v === 0x10) { this.rom.fill(0xFF, 0x2000); this.flashState = 0; this.flashBusyUntil = this.now + this.flashEraseSec * FCLK; this.events.push([this.now, 'FLASH chip erase']); }
    else if (st === 6 && v === 0x30) { const s = a & 0x1C000; this.rom.fill(0xFF, Math.max(s, 0x2000), s + 0x4000); this.flashState = 0; this.flashBusyUntil = this.now + this.flashEraseSec / 8 * FCLK; this.events.push([this.now, 'FLASH sector erase ' + s.toString(16)]); }
    else this.flashState = 0;
  }

  /* ---------------- ports */
  pinsIn(port) {          // external levels
    switch (port) {
      case 0x0A: return this.inputs.P4;
      case 0x0B: return this.inputs.P5;
      case 0x0E: {
        let v = this.inputs.P6 & ~0xFC;
        if (this.hwArac) v = v & ~1 | this.i2cSda();          // araç/sabit kartı: P60 SDA hattına bağlı
        v |= this.i2cSda() << 2;
        v |= (this.i2cSclLine()) << 1;
        v |= (this.keypad.int || this.panel.int ? 0 : 1) << 3;   // P63 INT1 /INT of the expanders (wired-AND)
        v |= (this.inputs.P6 & 0x10);                          // P64 INT2 (C-BUS IRQ)
        v |= this.ctcssIn.level(this.now) << 5;
        v |= this.selIn.level(this.now) << 6;
        v |= this.lockPin() << 7;
        return v;
      }
      case 0x0F: return this.inputs.P7;
      case 0x12: return this.inputs.P8;
    }
    return 0xFF;
  }
  analogRead(ch) {
    if (this.autoPower && (ch === 7 || ch === 6)) {
      const t = this.timers[3], tx = this.outBit(0x0B, 5) === 0;
      const duty = tx && t.running && (this.sfr[0x59] & 3) === 3 ? (t.duty || 0) : 0;
      const fwd = Math.min(255, Math.round(duty * 255 * this.powerGain));
      return ch === 7 ? fwd : Math.round(fwd * this.reflRatio);
    }
    return this.analog[ch] & 0xFF;
  }
  /* P67 level. The original firmware treats P67 = 0 as "locked" (F_00D51D,
   * F_00D3B4, F_00EE14 and the state machine are only consistent that way),
   * so the default model is active low. 'high' is kept for experiments. */
  lockPin() {
    const l = this.synthLocked();
    switch (this.lockModel) {
      case 'low': return l ? 0 : 1;
      case 'pulse': return l ? ((this.now % 4032) < 40 ? 0 : 1) : 0;
      default: return l ? 1 : 0;
    }
  }
  synthLocked() { return !this.lockFault && this.synth.loads > 0 && this.now >= this.synth.lockAt; }
  dirOf(port) { return this.sfr[{ 0x0A: 0x0C, 0x0B: 0x0D, 0x0E: 0x10, 0x0F: 0x11, 0x12: 0x14 }[port]]; }
  portRead(port) { const d = this.dirOf(port); return (this.latch[port] & d) | (this.pinsIn(port) & ~d); }
  i2cSdaMaster() {
    const d = this.sfr[0x10], l = this.latch[0x0E];
    return (d & 4) && !(l & 4) || this.hwArac && (d & 1) && !(l & 1) ? 0 : 1;
  }
  i2cSclLine() { const d = this.sfr[0x10]; return (d & 2) ? (this.latch[0x0E] >> 1 & 1) : 1; }
  i2cSda() { return this.i2cSdaMaster() & this.i2c.slaveSda; }
  i2cUpdate() { this.i2c.update(this.i2cSclLine(), this.i2cSdaMaster()); }
  updateIntLines() {
    const lv = (this.keypad.int || this.panel.int) ? 0 : 1;
    if (this.int1Lvl === 1 && lv === 0) { this.sfr[0x7E] |= 0x08; this.irqDirty = true; }   // falling edge (polarity bit assumed 0)
    this.int1Lvl = lv;
  }
  setInt2(level) {
    const prev = this.inputs.P6 >> 4 & 1;
    this.inputs.P6 = (this.inputs.P6 & ~0x10) | (level << 4);
    if (prev && !level) { this.sfr[0x7F] |= 0x08; this.irqDirty = true; }
  }
  portWrite(port, v) {
    const old = this.latch[port]; this.latch[port] = v;
    if (port === 0x0E) this.i2cUpdate();
    if (port === 0x0F && !(old & 1) && (v & 1) && this.synthSelected()) this.synth.latch(this.now);   // P70 LE rising
    if (port === 0x0F && (old ^ v) & 2) this.events.push([this.now, 'P71 GUC_ACMA=' + (v >> 1 & 1)]);
  }
  synthSelected() { return (this.sfr[0x14] & 2) && !(this.latch[0x12] & 2); }   // P81 active low (assumption from synth.c)
  outBit(port, bit) { const d = this.dirOf(port); return d >> bit & 1 ? this.latch[port] >> bit & 1 : null; }

  /* TA3 event mode counts edges of the squelch line P57 (F_007D35 / TA3 ISR 0x01BB88):
   * P57 = 1 = squelch open (signal on the channel) -> RAM 0x023E bit 5 */
  taInput(name) { return name === 'TA3' ? this.inputs.P5 >> 7 & 1 : null; }
  tbInput(name, now) {
    if (name === 'TB0') return this.ctcssIn.level(now);
    if (name === 'TB1') return this.selIn.level(now);
    return 1;
  }

  /* ---------------- SFR */
  sfrRead(a) {
    switch (a) {
      case 0x0A: case 0x0B: case 0x0E: case 0x0F: case 0x12: return this.portRead(a);
      case 0x34: return this.uart[0].readC0();
      case 0x35: return this.uart[0].readC1();
      case 0x36: return this.uart[0].readRB();
      case 0x37: return 0;
      case 0x3C: return this.uart[1].readC0();
      case 0x3D: return this.uart[1].readC1();
      case 0x3E: return this.uart[1].readRB();
      case 0x3F: return 0;
      case 0x40: { let v = 0; for (const t of this.timers) if (t.running) v |= 1 << t.bit; return v; }
    }
    if (a >= 0x20 && a < 0x30) return a & 1 ? 0 : this.analogRead((a - 0x20) >> 1);
    if (a >= 0x46 && a < 0x56) { const t = this.timerAt[a]; const v = t.read(); return a & 1 ? v >> 8 : v & 0xFF; }
    if (a === 0x1E) return this.sfr[0x1E];
    return this.sfr[a];
  }
  sfrWrite(a, v) {
    if (a >= 0x46 && a < 0x56) {                // timer registers: 16-bit, low byte first
      const t = this.timerAt[a];
      if (a & 1) t.write((t.reload & 0xFF) | v << 8);
      else { t.reload = (t.reload & 0xFF00) | v; if (!t.running) t.counter = t.reload; }
      return;
    }
    const old = this.sfr[a];
    this.sfr[a] = v;
    switch (a) {
      case 0x0A: case 0x0B: case 0x0E: case 0x0F: case 0x12: this.portWrite(a, v); break;
      case 0x10: this.i2cUpdate(); break;
      case 0x1E: if (v & 0x40) { this.adDone = this.now + 60; } break;
      case 0x32: this.uart[0].writeTB(v); break;
      case 0x3A: this.uart[1].writeTB(v); break;
      case 0x40: for (const t of this.timers) t.start(!!(v >> t.bit & 1)); this.beepUpdate(); break;
      case 0x42: for (let i = 0; i < 5; i++) if (v >> i & 1) this.timers[i].trigger(); this.sfr[0x42] = 0; break;
      case 0x5E: if (v & 0x08) { this.events.push([this.now, 'software reset (PMOD)']); this.pendingReset = true; } break;
      case 0x60: this.wdt = 0; break;
    }
    if (a >= 0x70) this.irqDirty = true;
    if (a >= 0x56 && a <= 0x5D && old !== v) {
      const t = this.timers[a - 0x56];
      if (t.isB && (v & 3) === 2) t.meas = 0;
    }
  }

  uartByte(n, v) {
    if (n === 0) { this.serialOut.push(v); if (this.onSerial) this.onSerial(v); return; }
    // UART1: synthesizer or C-BUS
    if (this.synthSelected()) { this.synth.shiftByte(v); return; }
    const sel1 = this.outBit(0x0B, 1), sel2 = this.outBit(0x0B, 3);
    this.cbusLog.push([this.now, sel1 === 0 ? 'SEC1' : sel2 === 0 ? 'SEC2' : '-', v]);
    if (this.cbusLog.length > 400) this.cbusLog.splice(0, 100);
    return this.cbusReply !== undefined ? this.cbusReply : 0x00;
  }
  serialIn(bytes) { for (const b of bytes) this.uart[0].rxq.push(b & 0xFF); }

  /* alert tone: TA0 in timer mode with pulse output (TA0MOD bit 2), started by
   * F_008084 / stopped by F_008053; f = count source / (2 (TA0 + 1)) */
  beepUpdate() {
    const t = this.timers[0], md = this.sfr[0x56];
    const f = t.running && (md & 3) === 0 && (md & 4) ? FCLK / (DIV[md >> 6 & 3] * 2 * (t.reload + 1)) : 0;
    if (Math.abs(f - this.beepHz) > 0.5) {
      this.beepHz = f; this.pushAudio(this.audio.beep, f);
      this.events.push([this.now, f ? 'BİP ' + f.toFixed(0) + ' Hz' : 'BİP kapalı']);
    }
  }
  onTimerWrite(t, v) {
    if (t === this.timers[0]) this.beepUpdate();
    if (t.name === 'TA1' && this.timers[1].running) this.pushAudio(this.audio.ta1, v);
    if (t.name === 'TA2') this.pushAudio(this.audio.ta2, v >> 8);
  }
  pushAudio(buf, v) { buf.push(this.now, v); if (buf.length > 40000) buf.splice(0, 20000); }

  /* ---------------- run */
  step() {
    const cpu = this.cpu;
    if (this.pendingReset) { this.pendingReset = false; this.hardReset(); }
    if (this.irqDirty || cpu.waiting) {
      this.irqDirty = false;
      for (let i = 0x70; i < 0x80; i++) if (this.sfr[i] & 0x08) { this.irqDirty = true; break; }
      if (this.irqDirty && cpu.checkIrq(this.sfr)) { this.advance(cpu.cyc); cpu.cyc = 0; }
    }
    let dc;
    if (cpu.waiting) {
      dc = cpu.stopped ? 256 : 16;  // STP: oscillator stopped (power off); time still advances for the UI
    } else dc = cpu.step();
    this.advance(dc);
    return dc;
  }
  advance(dc) {
    this.now += dc;
    const now = this.now;
    for (let i = 0; i < 8; i++) { const t = this.timers[i]; if (t.running) t.tick(dc, now); }
    this.uart[0].tick(now); this.uart[1].tick(now);
    if (this.adDone && now >= this.adDone) { this.adDone = 0; this.sfr[0x1E] &= ~0x40; this.sfr[0x70] |= 0x08; this.irqDirty = true; }
    // watchdog: 12-bit counter, f512 (or f32 when WDTFS bit0 = 1): assumption for the period
    this.wdt += dc;
    const wper = 4096 * ((this.sfr[0x61] & 1) ? 16 : 256);
    if (this.wdt > wper) { this.wdt = 0; this.events.push([now, 'WATCHDOG']); this.cpu.interrupt(0xFFF6); this.cpu.waiting = false; }
  }
  /* run up to n cycles; stops early on a breakpoint */
  run(cycles) {
    const end = this.now + cycles;
    const bp = this.breakpoints.size ? this.breakpoints : null;
    while (this.now < end) {
      this.step();
      if (bp && bp.has(this.cpu.PG << 16 | this.cpu.PC)) return 'break';
      if (this.watchHit !== undefined && this.watchHit !== null) { const w = this.watchHit; this.watchHit = null; return 'watch ' + w.toString(16); }
    }
    return 'ok';
  }

  /* ---------------- high level views */
  lcdWords() { const r = this.lcd.ram, w = []; for (let k = 0; k < 10; k++) w.push(r[2 * k] << 8 | r[2 * k + 1]); return w; }
  radioState() {
    const p5 = this.latch[0x0B], d5 = this.sfr[0x0D];
    const pwr = this.timers[3];
    return {
      beepHz: this.beepHz,
      powerHold: this.outBit(0x0F, 1),
      rxSelect: this.outBit(0x0B, 5),
      txAudio: this.outBit(0x0F, 2),
      alert: this.outBit(0x0B, 0),
      squelchInfo: this.outBit(0x0A, 6),
      xshift: this.outBit(0x0A, 3),
      pwrPwm: pwr.running && (this.sfr[0x59] & 3) === 3 ? pwr.duty || 0 : null,
      ctcssPwm: this.timers[2].running && (this.sfr[0x58] & 3) === 3,
      locked: this.synthLocked(),
      vco: this.synth.vcoHz(this.fref || 12.8e6),
      synth: this.synth,
    };
  }
}

/* ------------------------------------------------------------ EEPROM image
 * Checksums verified by the original at boot (F_00DCFD), 16-bit byte sum
 * stored big-endian right after each block; a failure shows "ARZ PRG". */
const EE_SUMS = [[0x578, 0x45], [0x000, 0x05], [0x008, 0x0E], [0x018, 0x51D]];
function eeFixSums(ee) {
  for (const [a, n] of EE_SUMS) {
    let s = 0; for (let i = 0; i < n; i++) s += ee[a + i];
    ee[a + n] = s >> 8 & 0xFF; ee[a + n + 1] = s & 0xFF;
  }
  return ee;
}
function eeCheckSums(ee) {
  return EE_SUMS.map(([a, n]) => { let s = 0; for (let i = 0; i < n; i++) s += ee[a + i]; return ((ee[a + n] << 8 | ee[a + n + 1]) === (s & 0xFFFF)); });
}
/* EEPROM layout found so far (channel n = 1..):
 *   0x000..0x00A  options (RAM 0x0334..), 0x018.. RAM 0x0340.. (bit7 of 0x018 = language)
 *   0x02E + 13*(n-1)  channel record:
 *        +0 TX word bits 13..8 (bits 7,6 flags), +1 TX word bits 7..0,
 *        +2 RX word bits 13..8 (bits 7,6 flags), +3 RX word bits 7..0,
 *        +4 RX CTCSS tone 1..50 (bits 0-5), crystal shift (bit 6), selcall (bit 7) -> RAM 0x0375
 *        +5..+12 channel name, 8 chars, 7-bit ASCII -> RAM 0x024A (F_008A47); only A-Z 0-9
 *                space _ - / are accepted, any other char (also 0x00 / 0x7F) and no name is shown;
 *                bit 7 of +5 enables the TX tone, bit 7 of +6..+11 = TX CTCSS tone (MSB first) -> RAM 0x037C
 *        empty channel: first word & 0x3FFF == 0x3FFF
 *   0x58B  option byte (RAM 0x0335), 0x58C..0x58D band base (25 kHz units),
 *   0x58F  reference select (0 = 12.8 MHz)
 *   0x591..0x59A initial power PWM, 0x59B..0x5A4 / 0x5A5..0x5AE power targets (RAM 0x0277..0x027F) */
const EE_CH0 = 0x2E, EE_CFG = 0x58B;
const NAME_OK = c => (c >= 0x41 && c <= 0x5A) || (c >= 0x30 && c <= 0x39) || c === 0x20 || c === 0x5F || c === 0x2D || c === 0x2F;
function eeBase(ee) { return ee[EE_CFG + 1] << 8 | ee[EE_CFG + 2]; }
function eeSetChannel(ee, n, rxHz, txHz, opt) {
  opt = opt || {};
  const base = eeBase(ee), a = EE_CH0 + (n - 1) * 13;
  const tw = freqToWord(txHz, base), rw = freqToWord(rxHz, base);
  if (tw < 0 || tw > 0x3FFE || rw < 0 || rw > 0x3FFE) throw new Error('frekans taban..taban+40,9 MHz dışında');
  ee.fill(0, a, a + 13);
  ee[a] = tw >> 8; ee[a + 1] = tw & 0xFF; ee[a + 2] = rw >> 8; ee[a + 3] = rw & 0xFF;
  ee[a + 4] = (opt.rxTone || 0) & 0x3F | (opt.xshift ? 0x40 : 0) | (opt.selcall ? 0x80 : 0);
  const name = (opt.name || '').toUpperCase().replace(/[^A-Z0-9 _\-\/]/g, ' ').padEnd(8).slice(0, 8);
  for (let i = 0; i < 8; i++) ee[a + 5 + i] = name.charCodeAt(i);
  const tt = (opt.txTone || 0) & 0x3F;
  if (tt) { ee[a + 5] |= 0x80; for (let i = 0; i < 6; i++) if (tt >> (5 - i) & 1) ee[a + 6 + i] |= 0x80; }
}
function eeClearChannel(ee, n) { const a = EE_CH0 + (n - 1) * 13; ee.fill(0xFF, a, a + 13); }
function eeChannel(ee, n) {
  const a = EE_CH0 + (n - 1) * 13, base = eeBase(ee);
  const tw = (ee[a] & 0x3F) << 8 | ee[a + 1], rw = (ee[a + 2] & 0x3F) << 8 | ee[a + 3];
  if (tw === 0x3FFF) return null;
  let txTone = 0;
  if (ee[a + 5] & 0x80) for (let i = 0; i < 6; i++) txTone = txTone << 1 | (ee[a + 6 + i] >> 7);
  let name = '', nameOk = true;
  for (let i = 0; i < 8; i++) { const c = ee[a + 5 + i] & 0x7F; name += c >= 32 && c < 127 ? String.fromCharCode(c) : '·'; if (!NAME_OK(c)) nameOk = false; }
  return { nameOk, tx: wordToFreq(tw, base), rx: wordToFreq(rw, base), rxTone: ee[a + 4] & 0x3F, xshift: ee[a + 4] >> 6 & 1,
           selcall: ee[a + 4] >> 7, txTone, name, bytes: Array.from(ee.slice(a, a + 13)) };
}
function defaultEeprom() {
  const ee = new Uint8Array(2048).fill(0x00);
  ee.fill(0xFF, EE_CH0, 0x535);
  const base = 136000000 / 25000;                // band base 136 MHz (VHF)
  ee[EE_CFG + 1] = base >> 8; ee[EE_CFG + 2] = base & 0xFF;
  // radio parameters 0x018..0x02D (RAM 0x0340..0x0355). Values are guesses that keep the
  // original firmware running; a dump of a real radio should replace them.
  ee[0x014] = 0x08;   // 0x033F bit 3: keypad scanned (DTMF, direct entry)
  ee[0x018] = 0x01;   // 0x0340 bits 0-1 selcall standard (1 CCIR, 2 ZVEI1, 3 EEA), bit 7 language
  ee[0x01E] = 0x20;   // 0x0346 lock verify time (x32 counts of f64)
  ee[0x01F] = 0x10;   // 0x0347
  ee[0x023] = 0x10;   // 0x034B power-up delay before TX
  ee[0x024] = 0x02;   // 0x034C lock monitor window in TX
  ee[0x029] = 0x08;   // 0x0351 selcall decoder: periods averaged per window (TB1 ISR / TA1 ISR)
  ee[0x02A] = 0x04;   // 0x0352 selcall decoder: minimum accepted periods
  ee[0x02B] = 0x03;   // 0x0353 selcall decoder: allowed outliers + 1
  ee[0x02C] = 0x10;   // 0x0354 selcall decoder: deviation tolerance (x/10)
  // TX power calibration in the 0x58B block (F_00DA32): initial PWM 0x591.., targets 0x59B.., 0x5A5..
  ee.fill(0x80, 0x591, 0x59B); ee.fill(0xB0, 0x59B, 0x5A5); ee.fill(0x10, 0x5A5, 0x5AF);
  for (let n = 1; n <= 16; n++) { const f = 156.000e6 + (n - 1) * 25e3; eeSetChannel(ee, n, f, f, { name: 'KANAL ' + String(n).padStart(2, '0') }); }
  return eeFixSums(ee);
}

const api = { Board, parseHex, FCLK, EEPROM24C16, eeFixSums, eeCheckSums, eeSetChannel, eeClearChannel, eeChannel, eeBase, defaultEeprom, EE_SUMS, EE_CH0, EE_CFG, freqToWord, wordToFreq };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
else root.Board = api;
})(typeof window !== 'undefined' ? window : globalThis);
