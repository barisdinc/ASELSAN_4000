/* m7700.js - MELPS 7700 (M37732) CPU core for the ASELSAN radio simulator.
 *
 * Port of tools/emu7700.py (which was cross-checked against the lifted C
 * code), extended with real interrupts, RTI, WIT/STP and cycle counting.
 * Opcode tables are the same as tools/d77.py (MELPS 7700 SW manual App. C).
 *
 * The bus is supplied by the board: bus.rd(a), bus.wr(a, v), bus.tick(cycles).
 * Works in the browser (window.M7700) and in node (module.exports).
 */
(function (root) {
'use strict';

/* ------------------------------------------------------------ opcode tables */
const OPS = {}, B_IMP = {}, P89 = {};
const ALU = {0x01:'indx',0x03:'sr',0x05:'dp',0x07:'lind',0x09:'immM',0x0D:'abs',0x0F:'long',
             0x11:'indy',0x12:'ind',0x13:'sry',0x15:'dpx',0x17:'lindy',0x19:'absy',0x1D:'absx',0x1F:'longx'};
function alu(base, name, store) {
  for (const k in ALU) { if (store && +k === 0x09) continue; OPS[base + +k] = [name, ALU[k]]; }
}
[[0x00,'ORA'],[0x20,'AND'],[0x40,'EOR'],[0x60,'ADC'],[0xA0,'LDA'],[0xC0,'CMP'],[0xE0,'SBC']].forEach(([b,n]) => alu(b, n));
alu(0x80, 'STA', true);
[[0x00,'ASL'],[0x20,'ROL'],[0x40,'LSR'],[0x60,'ROR']].forEach(([b,n]) => {
  OPS[b+0x06]=[n,'dp']; OPS[b+0x0E]=[n,'abs']; OPS[b+0x16]=[n,'dpx']; OPS[b+0x1E]=[n,'absx']; OPS[b+0x0A]=[n,'acc'];
});
[[0xC0,'DEC'],[0xE0,'INC']].forEach(([b,n]) => {
  OPS[b+0x06]=[n,'dp']; OPS[b+0x0E]=[n,'abs']; OPS[b+0x16]=[n,'dpx']; OPS[b+0x1E]=[n,'absx'];
});
Object.assign(OPS, {
 0x00:['BRK','brk'],0x1A:['DEC','acc'],0x3A:['INC','acc'],
 0x04:['SEB','bitdp'],0x0C:['SEB','bitabs'],0x14:['CLB','bitdp'],0x1C:['CLB','bitabs'],
 0x24:['BBS','bbdp'],0x2C:['BBS','bbabs'],0x34:['BBC','bbdp'],0x3C:['BBC','bbabs'],
 0xE8:['INX','imp'],0xC8:['INY','imp'],0xCA:['DEX','imp'],0x88:['DEY','imp'],
 0xE0:['CPX','immX'],0xE4:['CPX','dp'],0xEC:['CPX','abs'],
 0xC0:['CPY','immX'],0xC4:['CPY','dp'],0xCC:['CPY','abs'],
 0xA2:['LDX','immX'],0xA6:['LDX','dp'],0xAE:['LDX','abs'],0xB6:['LDX','dpy'],0xBE:['LDX','absy'],
 0xA0:['LDY','immX'],0xA4:['LDY','dp'],0xAC:['LDY','abs'],0xB4:['LDY','dpx'],0xBC:['LDY','absx'],
 0x86:['STX','dp'],0x8E:['STX','abs'],0x96:['STX','dpy'],
 0x84:['STY','dp'],0x8C:['STY','abs'],0x94:['STY','dpx'],
 0x64:['LDM','ldmdp'],0x74:['LDM','ldmdpx'],0x9C:['LDM','ldmabs'],0x9E:['LDM','ldmabsx'],
 0x10:['BPL','rel8'],0x30:['BMI','rel8'],0x50:['BVC','rel8'],0x70:['BVS','rel8'],
 0x90:['BCC','rel8'],0xB0:['BCS','rel8'],0xD0:['BNE','rel8'],0xF0:['BEQ','rel8'],
 0x80:['BRA','rel8'],0x82:['BRAL','rel16'],
 0x20:['JSR','jabs'],0x22:['JSRL','jlong'],0xFC:['JSR','jindx'],
 0x4C:['JMP','jabs'],0x5C:['JMPL','jlong'],0x6C:['JMP','jind'],0x7C:['JMP','jindx'],0xDC:['JMPL','jlind'],
 0x60:['RTS','imp'],0x6B:['RTL','imp'],0x40:['RTI','imp'],
 0x08:['PHP','imp'],0x28:['PLP','imp'],0x48:['PHA','imp'],0x68:['PLA','imp'],
 0x0B:['PHD','imp'],0x2B:['PLD','imp'],0x4B:['PHG','imp'],0x8B:['PHT','imp'],0xAB:['PLT','imp'],
 0x5A:['PHY','imp'],0x7A:['PLY','imp'],0xDA:['PHX','imp'],0xFA:['PLX','imp'],
 0xD4:['PEI','dp'],0xF4:['PEA','imm16'],0x62:['PER','rel16'],
 0x18:['CLC','imp'],0x38:['SEC','imp'],0x58:['CLI','imp'],0x78:['SEI','imp'],0xB8:['CLV','imp'],
 0xD8:['CLM','imp'],0xF8:['SEM','imp'],0xC2:['CLP','imm8'],0xE2:['SEP','imm8'],
 0xAA:['TAX','imp'],0xA8:['TAY','imp'],0x8A:['TXA','imp'],0x98:['TYA','imp'],0x9A:['TXS','imp'],
 0xBA:['TSX','imp'],0x9B:['TXY','imp'],0xBB:['TYX','imp'],
 0x1B:['TAS','imp'],0x3B:['TSA','imp'],0x5B:['TAD','imp'],0x7B:['TDA','imp'],
 0x44:['MVP','mv'],0x54:['MVN','mv'],
 0xCB:['WIT','imp'],0xDB:['STP','imp'],0xEA:['NOP','imp'],0xEB:['PSH','imm8'],0xFB:['PUL','imm8'],
});
Object.assign(B_IMP, {0x0A:['ASL','accB'],0x1A:['DEC','accB'],0x2A:['ROL','accB'],0x3A:['INC','accB'],
  0x4A:['LSR','accB'],0x6A:['ROR','accB'],0x48:['PHB','imp'],0x68:['PLB','imp'],
  0x8A:['TXB','imp'],0x98:['TYB','imp'],0xAA:['TBX','imp'],0xA8:['TBY','imp'],
  0x1B:['TBS','imp'],0x3B:['TSB','imp'],0x5B:['TBD','imp'],0x7B:['TDB','imp']});
for (const k in ALU) { P89[+k] = ['MPY', ALU[k]]; P89[0x20 + +k] = ['DIV', ALU[k]]; }
Object.assign(P89, {0x28:['XAB','imp'],0x49:['RLA','immM'],0xC2:['LDT','imm8']});
const ACC_OPS = new Set(['ORA','AND','EOR','ADC','STA','LDA','CMP','SBC']);

/* numeric decode: index = op (0..255), 256+op (0x42 prefix), 512+op (0x89 prefix) */
const NAMES = [], MODES = [];
const nameId = {}, modeId = {};
function nid(n) { if (!(n in nameId)) { nameId[n] = NAMES.length; NAMES.push(n); } return nameId[n]; }
function mid(m) { if (!(m in modeId)) { modeId[m] = MODES.length; MODES.push(m); } return modeId[m]; }
const DEC_N = new Int16Array(768).fill(-1), DEC_M = new Int16Array(768).fill(-1);
for (let op = 0; op < 256; op++) {
  if (OPS[op]) { DEC_N[op] = nid(OPS[op][0]); DEC_M[op] = mid(OPS[op][1]); }
  const b = B_IMP[op] || (OPS[op] && ACC_OPS.has(OPS[op][0]) ? OPS[op] : null);
  if (b) { DEC_N[256 + op] = nid(b[0]); DEC_M[256 + op] = mid(b[1]); }
  if (P89[op]) { DEC_N[512 + op] = nid(P89[op][0]); DEC_M[512 + op] = mid(P89[op][1]); }
}
const N = nameId, M = modeId;
['immM','immX','imm8','imm16','acc','accB','imp'].forEach(mid);

/* operand byte count for the disassembler (m, x = 1 means 8-bit) */
function operandLen(mode, m, x) {
  switch (mode) {
    case 'imp': case 'acc': case 'accB': return 0;
    case 'immM': return m ? 1 : 2;
    case 'immX': return x ? 1 : 2;
    case 'imm8': case 'dp': case 'dpx': case 'dpy': case 'ind': case 'indx': case 'indy':
    case 'lind': case 'lindy': case 'sr': case 'sry': case 'rel8': case 'brk': return 1;
    case 'abs': case 'absx': case 'absy': case 'jabs': case 'jind': case 'jindx': case 'jlind':
    case 'rel16': case 'imm16': case 'mv': return 2;
    case 'long': case 'longx': case 'jlong': return 3;
    case 'bitdp': return 1 + (m ? 1 : 2);
    case 'bitabs': return 2 + (m ? 1 : 2);
    case 'bbdp': return 1 + (m ? 1 : 2) + 1;
    case 'bbabs': return 2 + (m ? 1 : 2) + 1;
    case 'ldmdp': case 'ldmdpx': return 1 + (m ? 1 : 2);
    case 'ldmabs': case 'ldmabsx': return 2 + (m ? 1 : 2);
  }
  return 0;
}

/* hardware interrupt priority (high -> low) and vectors, M37732S4 datasheet */
const IRQS = [  // [IC register, vector, name]
  [0x7D, 0xFFF4, 'INT0'], [0x7E, 0xFFF2, 'INT1'], [0x7F, 0xFFF0, 'INT2'],
  [0x75, 0xFFEE, 'TA0'], [0x76, 0xFFEC, 'TA1'], [0x77, 0xFFEA, 'TA2'], [0x78, 0xFFE8, 'TA3'], [0x79, 0xFFE6, 'TA4'],
  [0x7A, 0xFFE4, 'TB0'], [0x7B, 0xFFE2, 'TB1'], [0x7C, 0xFFE0, 'TB2'],
  [0x72, 0xFFDE, 'UART0_RX'], [0x71, 0xFFDC, 'UART0_TX'], [0x74, 0xFFDA, 'UART1_RX'], [0x73, 0xFFD8, 'UART1_TX'],
  [0x70, 0xFFD6, 'AD'],
];

class CPU {
  constructor(bus) {
    this.bus = bus;
    this.reset();
  }
  reset() {
    this.A = this.B = this.X = this.Y = 0; this.S = 0x0FFF; this.D = 0;
    this.DT = 0; this.PG = 0;
    this.C = this.Z = this.N = this.V = 0; this.I = 1; this.Dm = 0; this.IPL = 0;
    this.m = 0; this.x = 0;
    this.cyc = 0;                 // cycle counter of the current instruction
    this.waiting = false;         // WIT
    this.stopped = false;         // STP
    this.lastIrq = null;
    this.PC = this.rd16(0xFFFE);
    this.prevPC = 0;
  }

  /* memory, 1 cycle per byte access */
  rd8(a) { this.cyc++; return this.bus.rd(a & 0xFFFFFF); }
  wr8(a, v) { this.cyc++; this.bus.wr(a & 0xFFFFFF, v & 0xFF); }
  rd16(a) { return this.rd8(a) | this.rd8(a + 1) << 8; }
  wr16(a, v) { this.wr8(a, v); this.wr8(a + 1, v >> 8); }
  rd24(a) { return this.rd16(a) | this.rd8(a + 2) << 16; }
  push8(v) { this.wr8(this.S, v); this.S = (this.S - 1) & 0xFFFF; }
  pull8() { this.S = (this.S + 1) & 0xFFFF; return this.rd8(this.S); }
  push16(v) { this.push8(v >> 8); this.push8(v); }
  pull16() { const lo = this.pull8(); return lo | this.pull8() << 8; }

  getP() { return this.C | this.Z << 1 | this.I << 2 | this.Dm << 3 | this.x << 4 | this.m << 5 | this.V << 6 | this.N << 7 | (this.IPL & 7) << 8; }
  setP(p) {
    this.C = p & 1; this.Z = p >> 1 & 1; this.I = p >> 2 & 1; this.Dm = p >> 3 & 1; this.x = p >> 4 & 1;
    this.m = p >> 5 & 1; this.V = p >> 6 & 1; this.N = p >> 7 & 1; this.IPL = p >> 8 & 7;
  }

  fb() { const v = this.rd8(this.PG << 16 | this.PC); this.PC = (this.PC + 1) & 0xFFFF; return v; }
  fw() { const lo = this.fb(); return lo | this.fb() << 8; }
  fl() { const lo = this.fw(); return lo | this.fb() << 16; }

  nz(v, w) {
    if (w === 1) { this.Z = (v & 0xFF) === 0 ? 1 : 0; this.N = v >> 7 & 1; }
    else { this.Z = (v & 0xFFFF) === 0 ? 1 : 0; this.N = v >> 15 & 1; }
  }

  ea(mode) {
    const D = this.D, X = this.X, Y = this.Y, DT = this.DT;
    switch (mode) {
      case M.dp: case M.bitdp: case M.bbdp: case M.ldmdp: return (D + this.fb()) & 0xFFFF;
      case M.dpx: case M.ldmdpx: return (D + this.fb() + X) & 0xFFFF;
      case M.dpy: return (D + this.fb() + Y) & 0xFFFF;
      case M.ind: return DT << 16 | this.rd16((D + this.fb()) & 0xFFFF);
      case M.indx: return DT << 16 | this.rd16((D + this.fb() + X) & 0xFFFF);
      case M.indy: return ((DT << 16 | this.rd16((D + this.fb()) & 0xFFFF)) + Y) & 0xFFFFFF;
      case M.lind: return this.rd24((D + this.fb()) & 0xFFFF);
      case M.lindy: return (this.rd24((D + this.fb()) & 0xFFFF) + Y) & 0xFFFFFF;
      case M.sr: return (this.S + this.fb()) & 0xFFFF;
      case M.sry: return ((DT << 16 | this.rd16((this.S + this.fb()) & 0xFFFF)) + Y) & 0xFFFFFF;
      case M.abs: case M.bitabs: case M.bbabs: case M.ldmabs: return DT << 16 | this.fw();
      case M.absx: case M.ldmabsx: return ((DT << 16 | this.fw()) + X) & 0xFFFFFF;
      case M.absy: return ((DT << 16 | this.fw()) + Y) & 0xFFFFFF;
      case M.long: return this.fl();
      case M.longx: return (this.fl() + X) & 0xFFFFFF;
    }
    throw new Error('ea mode ' + MODES[mode]);
  }
  rdw(a, w) { return w === 1 ? this.rd8(a) : this.rd16(a); }
  wrw(a, v, w) { if (w === 1) this.wr8(a, v); else this.wr16(a, v); }
  operand(mode, w) {
    if (mode === M.immM || mode === M.immX) return w === 1 ? this.fb() : this.fw();
    if (mode === M.imm8) return this.fb();
    return this.rdw(this.ea(mode), w);
  }
  setAcc(isB, v, w) {
    if (isB) this.B = w === 1 ? (this.B & 0xFF00) | (v & 0xFF) : v & 0xFFFF;
    else this.A = w === 1 ? (this.A & 0xFF00) | (v & 0xFF) : v & 0xFFFF;
  }
  adc(r, v, w) {
    const msk = w === 1 ? 0xFF : 0xFFFF, sg = w === 1 ? 0x80 : 0x8000;
    const a = r & msk, b = v & msk, t = a + b + this.C;
    this.V = (~(a ^ b) & (a ^ t) & sg) ? 1 : 0; this.C = t > msk ? 1 : 0; this.nz(t, w); return t & msk;
  }
  sbc(r, v, w) {
    const msk = w === 1 ? 0xFF : 0xFFFF, sg = w === 1 ? 0x80 : 0x8000;
    const a = r & msk, b = v & msk, t = a - b - (1 - this.C);
    this.V = ((a ^ b) & (a ^ t) & sg) ? 1 : 0; this.C = t >= 0 ? 1 : 0; this.nz(t, w); return t & msk;
  }
  shift(n, v, w) {
    const msk = w === 1 ? 0xFF : 0xFFFF, top = w === 1 ? 7 : 15; v &= msk;
    let c, r;
    if (n === N.ASL) { c = v >> top & 1; r = (v << 1) & msk; }
    else if (n === N.LSR) { c = v & 1; r = v >> 1; }
    else if (n === N.ROL) { c = v >> top & 1; r = ((v << 1) | this.C) & msk; }
    else { c = v & 1; r = (v >> 1) | (this.C << top); }
    this.C = c; this.nz(r, w); return r;
  }

  /* software / hardware interrupt entry */
  interrupt(vector, level) {
    this.push8(this.PG); this.push16(this.PC); this.push16(this.getP());
    this.I = 1;
    if (level !== undefined) this.IPL = level;
    this.PG = 0; this.PC = this.rd16(vector);
    this.cyc += 6;
  }

  /* called by the board each step: returns true if an interrupt was taken */
  checkIrq(sfr) {
    if (this.I && !this.waiting) return false;
    let best = -1, bestLvl = 0;
    for (let i = 0; i < IRQS.length; i++) {
      const ic = sfr[IRQS[i][0]];
      if (!(ic & 0x08)) continue;
      const lvl = ic & 7;
      if (lvl > bestLvl) { bestLvl = lvl; best = i; }
    }
    if (best < 0 || bestLvl <= this.IPL) return false;
    this.waiting = false;              // WIT ends on an accepted request
    if (this.I) return false;
    sfr[IRQS[best][0]] &= ~0x08;
    this.lastIrq = IRQS[best][2];
    this.interrupt(IRQS[best][1], bestLvl);
    return true;
  }

  step() {
    this.cyc = 1;
    this.prevPC = this.PG << 16 | this.PC;
    let op = this.fb(), idx = op, isB = false;
    if (op === 0x42) { idx = 256 + this.fb(); isB = true; }
    else if (op === 0x89) idx = 512 + this.fb();
    const n = DEC_N[idx], mode = DEC_M[idx];
    if (n < 0) throw new Error('illegal opcode ' + idx.toString(16) + ' at ' + this.prevPC.toString(16));
    const wa = this.m ? 1 : 2, wx = this.x ? 1 : 2;
    let v, e, t, r;
    switch (n) {
      case N.LDA: v = this.operand(mode, wa); this.setAcc(isB, v, wa); this.nz(v, wa); break;
      case N.LDX: v = this.operand(mode, wx); this.X = wx === 1 ? (this.X & 0xFF00) | v : v; this.nz(v, wx); break;
      case N.LDY: v = this.operand(mode, wx); this.Y = wx === 1 ? (this.Y & 0xFF00) | v : v; this.nz(v, wx); break;
      case N.STA: this.wrw(this.ea(mode), isB ? this.B : this.A, wa); break;
      case N.STX: this.wrw(this.ea(mode), this.X, wx); break;
      case N.STY: this.wrw(this.ea(mode), this.Y, wx); break;
      case N.LDM: e = this.ea(mode); v = wa === 1 ? this.fb() : this.fw(); this.wrw(e, v, wa); break;
      case N.AND: case N.ORA: case N.EOR:
        v = this.operand(mode, wa); r = isB ? this.B : this.A;
        r = n === N.AND ? r & v : (n === N.ORA ? r | v : r ^ v);
        this.setAcc(isB, r, wa); this.nz(r, wa); break;
      case N.ADC: v = this.operand(mode, wa); this.setAcc(isB, this.adc(isB ? this.B : this.A, v, wa), wa); break;
      case N.SBC: v = this.operand(mode, wa); this.setAcc(isB, this.sbc(isB ? this.B : this.A, v, wa), wa); break;
      case N.CMP: case N.CPX: case N.CPY: {
        const w = n === N.CMP ? wa : wx;
        r = n === N.CMP ? (isB ? this.B : this.A) : (n === N.CPX ? this.X : this.Y);
        v = this.operand(mode, w); const msk = w === 1 ? 0xFF : 0xFFFF;
        t = (r & msk) - v; this.C = t >= 0 ? 1 : 0; this.nz(t, w); break;
      }
      case N.INC: case N.DEC: {
        const d = n === N.INC ? 1 : -1;
        if (mode === M.acc || mode === M.accB) {
          const b = mode === M.accB; v = (b ? this.B : this.A) + d; this.setAcc(b, v, wa); this.nz(v, wa);
        } else { e = this.ea(mode); v = this.rdw(e, wa) + d; this.wrw(e, v & 0xFFFF, wa); this.nz(v, wa); }
        break;
      }
      case N.INX: this.X = (this.X + 1) & (wx === 1 ? 0xFF : 0xFFFF); this.nz(this.X, wx); break;
      case N.INY: this.Y = (this.Y + 1) & (wx === 1 ? 0xFF : 0xFFFF); this.nz(this.Y, wx); break;
      case N.DEX: this.X = (this.X - 1) & (wx === 1 ? 0xFF : 0xFFFF); this.nz(this.X, wx); break;
      case N.DEY: this.Y = (this.Y - 1) & (wx === 1 ? 0xFF : 0xFFFF); this.nz(this.Y, wx); break;
      case N.ASL: case N.LSR: case N.ROL: case N.ROR:
        if (mode === M.acc || mode === M.accB) {
          const b = mode === M.accB; this.setAcc(b, this.shift(n, b ? this.B : this.A, wa), wa);
        } else { e = this.ea(mode); this.wrw(e, this.shift(n, this.rdw(e, wa), wa), wa); }
        break;
      case N.RLA: {
        const cnt = this.operand(mode, wa), msk = wa === 1 ? 0xFF : 0xFFFF, top = wa === 1 ? 7 : 15;
        v = this.A & msk;
        for (let k = 0; k < cnt; k++) v = ((v << 1) | (v >> top)) & msk;
        this.setAcc(false, v, wa); this.nz(v, wa); this.cyc += cnt; break;
      }
      case N.SEB: case N.CLB: {
        e = this.ea(mode); const mk = wa === 1 ? this.fb() : this.fw(); v = this.rdw(e, wa);
        this.wrw(e, n === N.SEB ? v | mk : v & ~mk & 0xFFFF, wa); break;
      }
      case N.BBS: case N.BBC: {
        e = this.ea(mode); const mk = wa === 1 ? this.fb() : this.fw(); v = this.rdw(e, wa);
        r = this.fb(); t = (this.PC + (r > 127 ? r - 256 : r)) & 0xFFFF;
        if ((n === N.BBS && (v & mk) === mk) || (n === N.BBC && (v & mk) === 0)) { this.PC = t; this.cyc += 2; }
        break;
      }
      case N.BEQ: case N.BNE: case N.BCS: case N.BCC: case N.BMI: case N.BPL: case N.BVS: case N.BVC: case N.BRA: {
        r = this.fb(); t = (this.PC + (r > 127 ? r - 256 : r)) & 0xFFFF;
        let c;
        switch (n) {
          case N.BEQ: c = this.Z; break; case N.BNE: c = !this.Z; break;
          case N.BCS: c = this.C; break; case N.BCC: c = !this.C; break;
          case N.BMI: c = this.N; break; case N.BPL: c = !this.N; break;
          case N.BVS: c = this.V; break; case N.BVC: c = !this.V; break;
          default: c = true;
        }
        if (c) { this.PC = t; this.cyc += 2; }
        break;
      }
      case N.BRAL: r = this.fw(); this.PC = (this.PC + (r > 32767 ? r - 65536 : r)) & 0xFFFF; this.cyc += 2; break;
      case N.JMP:
        if (mode === M.jabs) this.PC = this.fw();
        else if (mode === M.jind) this.PC = this.rd16(this.fw());
        else { const p = this.fw(); this.PC = this.rd16(this.PG << 16 | ((p + this.X) & 0xFFFF)); }
        this.cyc += 1; break;
      case N.JMPL:
        if (mode === M.jlong) { v = this.fl(); this.PC = v & 0xFFFF; this.PG = v >> 16; }
        else { v = this.rd24(this.fw()); this.PC = v & 0xFFFF; this.PG = v >> 16 & 0xFF; }
        this.cyc += 1; break;
      case N.JSR:
        if (mode === M.jabs) { t = this.fw(); this.push16(this.PC); this.PC = t; }
        else { const p = this.fw(); t = this.rd16(this.PG << 16 | ((p + this.X) & 0xFFFF)); this.push16(this.PC); this.PC = t; }
        this.cyc += 2; break;
      case N.JSRL: t = this.fl(); this.push8(this.PG); this.push16(this.PC); this.PC = t & 0xFFFF; this.PG = t >> 16; this.cyc += 2; break;
      case N.RTS: this.PC = this.pull16(); this.cyc += 2; break;
      case N.RTL: this.PC = this.pull16(); this.PG = this.pull8(); this.cyc += 2; break;
      case N.RTI: this.setP(this.pull16()); this.PC = this.pull16(); this.PG = this.pull8(); this.cyc += 2; break;
      case N.TAX: case N.TBX: v = (n === N.TAX ? this.A : this.B) & (wx === 1 ? 0xFF : 0xFFFF); this.X = v; this.nz(v, wx); break;
      case N.TAY: case N.TBY: v = (n === N.TAY ? this.A : this.B) & (wx === 1 ? 0xFF : 0xFFFF); this.Y = v; this.nz(v, wx); break;
      case N.TXA: v = this.X; this.setAcc(false, v, wa); this.nz(v, wa); break;
      case N.TYA: v = this.Y; this.setAcc(false, v, wa); this.nz(v, wa); break;
      case N.TXB: v = this.X; this.setAcc(true, v, wa); this.nz(v, wa); break;
      case N.TYB: v = this.Y; this.setAcc(true, v, wa); this.nz(v, wa); break;
      case N.TXY: this.Y = this.X; this.nz(this.Y, wx); break;
      case N.TYX: this.X = this.Y; this.nz(this.X, wx); break;
      case N.TXS: this.S = this.X; break;
      case N.TSX: this.X = this.S; this.nz(this.X, wx); break;
      case N.TAS: this.S = this.A; break;
      case N.TBS: this.S = this.B; break;
      case N.TSA: this.A = this.S; this.nz(this.S, 2); break;
      case N.TSB: this.B = this.S; this.nz(this.S, 2); break;
      case N.TAD: this.D = this.A; break;
      case N.TBD: this.D = this.B; break;
      case N.TDA: this.A = this.D; this.nz(this.D, 2); break;
      case N.TDB: this.B = this.D; this.nz(this.D, 2); break;
      case N.XAB: t = this.A; this.A = this.B; this.B = t; this.nz(this.A, wa); break;
      case N.PHA: if (wa === 1) this.push8(this.A); else this.push16(this.A); break;
      case N.PHB: if (wa === 1) this.push8(this.B); else this.push16(this.B); break;
      case N.PLA: v = wa === 1 ? this.pull8() : this.pull16(); this.setAcc(false, v, wa); this.nz(v, wa); break;
      case N.PLB: v = wa === 1 ? this.pull8() : this.pull16(); this.setAcc(true, v, wa); this.nz(v, wa); break;
      case N.PHX: if (wx === 1) this.push8(this.X); else this.push16(this.X); break;
      case N.PHY: if (wx === 1) this.push8(this.Y); else this.push16(this.Y); break;
      case N.PLX: v = wx === 1 ? this.pull8() : this.pull16(); this.X = v; this.nz(v, wx); break;
      case N.PLY: v = wx === 1 ? this.pull8() : this.pull16(); this.Y = v; this.nz(v, wx); break;
      case N.PHD: this.push16(this.D); break;
      case N.PLD: this.D = this.pull16(); break;
      case N.PHT: this.push8(this.DT); break;
      case N.PLT: this.DT = this.pull8(); break;
      case N.PHG: this.push8(this.PG); break;
      case N.PHP: this.push16(this.getP()); break;
      case N.PLP: this.setP(this.pull16()); break;
      case N.PSH: {
        const mk = this.fb();
        if (mk & 1) { if (this.m) this.push8(this.A); else this.push16(this.A); }
        if (mk & 2) { if (this.m) this.push8(this.B); else this.push16(this.B); }
        if (mk & 4) { if (this.x) this.push8(this.X); else this.push16(this.X); }
        if (mk & 8) { if (this.x) this.push8(this.Y); else this.push16(this.Y); }
        if (mk & 0x10) this.push16(this.D);
        if (mk & 0x20) this.push8(this.DT);
        if (mk & 0x40) this.push8(this.PG);
        if (mk & 0x80) this.push16(this.getP());
        break;
      }
      case N.PUL: {
        const mk = this.fb();
        if (mk & 0x80) this.setP(this.pull16());
        if (mk & 0x40) this.PG = this.pull8();
        if (mk & 0x20) this.DT = this.pull8();
        if (mk & 0x10) this.D = this.pull16();
        if (mk & 8) this.Y = this.x ? this.pull8() : this.pull16();
        if (mk & 4) this.X = this.x ? this.pull8() : this.pull16();
        if (mk & 2) this.setAcc(true, this.m ? this.pull8() : this.pull16(), this.m ? 1 : 2);
        if (mk & 1) this.setAcc(false, this.m ? this.pull8() : this.pull16(), this.m ? 1 : 2);
        break;
      }
      case N.PEA: this.push16(this.fw()); break;
      case N.PEI: this.push16(this.rd16((this.D + this.fb()) & 0xFFFF)); break;
      case N.PER: r = this.fw(); this.push16((this.PC + (r > 32767 ? r - 65536 : r)) & 0xFFFF); break;
      case N.CLC: this.C = 0; break;
      case N.SEC: this.C = 1; break;
      case N.CLV: this.V = 0; break;
      case N.CLI: this.I = 0; break;
      case N.SEI: this.I = 1; break;
      case N.CLM: this.m = 0; break;
      case N.SEM: this.m = 1; break;
      case N.CLP: this.setP(this.getP() & ~this.fb() & 0xFFFF); break;
      case N.SEP: this.setP(this.getP() | this.fb()); break;
      case N.LDT: this.DT = this.fb(); break;
      case N.MPY:
        v = this.operand(mode, wa);
        if (wa === 1) { const p = (this.A & 0xFF) * (v & 0xFF); this.setAcc(false, p, 1); this.setAcc(true, p >> 8, 1); }
        else { const p = this.A * v; this.A = p & 0xFFFF; this.B = Math.floor(p / 65536) & 0xFFFF; }
        this.nz(this.A, wa); this.cyc += 8; break;
      case N.DIV:
        v = this.operand(mode, wa);
        if (wa === 1) {
          const num = (this.B & 0xFF) << 8 | (this.A & 0xFF); v &= 0xFF;
          if (v === 0) { if (this.bus.trap) this.bus.trap('ZERODIV', this.prevPC); this.interrupt(0xFFFC); break; }
          this.setAcc(false, Math.floor(num / v), 1); this.setAcc(true, num % v, 1);
        } else {
          const num = this.B * 65536 + this.A;
          if (v === 0) { if (this.bus.trap) this.bus.trap('ZERODIV', this.prevPC); this.interrupt(0xFFFC); break; }
          this.A = Math.floor(num / v) & 0xFFFF; this.B = num % v;
        }
        this.nz(this.A, wa); this.cyc += 16; break;
      case N.NOP: break;
      case N.WIT: this.waiting = true; break;
      case N.STP: this.stopped = true; this.waiting = true; break;
      case N.BRK: this.fb(); if (this.bus.trap) this.bus.trap('BRK', this.prevPC); this.interrupt(0xFFFA); break;
      case N.MVN: case N.MVP: {
        const dst = this.fb(), src = this.fb(), dd = n === N.MVN ? 1 : -1;
        for (;;) {
          this.wr8(dst << 16 | this.Y, this.rd8(src << 16 | this.X));
          this.X = (this.X + dd) & 0xFFFF; this.Y = (this.Y + dd) & 0xFFFF;
          this.A = (this.A - 1) & 0xFFFF;
          if (this.A === 0xFFFF) break;
        }
        this.DT = dst; break;
      }
      default:
        throw new Error('not implemented ' + NAMES[n] + ' ' + MODES[mode]);
    }
    return this.cyc;
  }
}

/* ------------------------------------------------------------ disassembler */
function hex(v, n) { return v.toString(16).toUpperCase().padStart(n, '0'); }
function disasm(rd, addr, m, x) {
  let a = addr;
  const b0 = rd(a++); let idx = b0, pre = '';
  if (b0 === 0x42) { idx = 256 + rd(a++); }
  else if (b0 === 0x89) idx = 512 + rd(a++);
  const n = DEC_N[idx];
  if (n < 0) return { len: a - addr, text: '.BYTE $' + hex(b0, 2) };
  const name = NAMES[n], mode = MODES[DEC_M[idx]];
  const isB = idx >= 256 && idx < 512 && ACC_OPS.has(name);
  let mm = m, xx = x;
  if (['LDX','LDY','CPX','CPY'].includes(name)) mm = x;
  const len = operandLen(mode === 'immX' ? 'immX' : mode, mm, xx);
  const ops = []; for (let i = 0; i < len; i++) ops.push(rd(a + i));
  const w = ops.length >= 2 ? ops[0] | ops[1] << 8 : ops[0];
  const l = ops.length >= 3 ? ops[0] | ops[1] << 8 | ops[2] << 16 : 0;
  const acc = ACC_OPS.has(name) ? (isB ? 'B,' : 'A,') : '';
  let s;
  const next = a + len;
  switch (mode) {
    case 'imp': s = ''; break;
    case 'acc': s = 'A'; break;
    case 'accB': s = 'B'; break;
    case 'immM': case 'immX': case 'imm8': case 'imm16': s = '#$' + hex(w, len * 2); break;
    case 'dp': s = '$' + hex(w, 2); break;
    case 'dpx': s = '$' + hex(w, 2) + ',X'; break;
    case 'dpy': s = '$' + hex(w, 2) + ',Y'; break;
    case 'ind': s = '($' + hex(w, 2) + ')'; break;
    case 'indx': s = '($' + hex(w, 2) + ',X)'; break;
    case 'indy': s = '($' + hex(w, 2) + '),Y'; break;
    case 'lind': s = 'L($' + hex(w, 2) + ')'; break;
    case 'lindy': s = 'L($' + hex(w, 2) + '),Y'; break;
    case 'sr': s = '$' + hex(w, 2) + ',S'; break;
    case 'sry': s = '($' + hex(w, 2) + ',S),Y'; break;
    case 'abs': s = '$' + hex(w, 4); break;
    case 'absx': s = '$' + hex(w, 4) + ',X'; break;
    case 'absy': s = '$' + hex(w, 4) + ',Y'; break;
    case 'long': s = '$' + hex(l, 6); break;
    case 'longx': s = '$' + hex(l, 6) + ',X'; break;
    case 'rel8': s = '$' + hex((addr & 0xFF0000) | ((next + (w > 127 ? w - 256 : w)) & 0xFFFF), 6); break;
    case 'rel16': s = '$' + hex((addr & 0xFF0000) | ((next + (w > 32767 ? w - 65536 : w)) & 0xFFFF), 6); break;
    case 'jabs': s = '$' + hex((addr & 0xFF0000) | w, 6); break;
    case 'jlong': s = '$' + hex(l, 6); break;
    case 'jind': s = '($' + hex(w, 4) + ')'; break;
    case 'jindx': s = '($' + hex(w, 4) + ',X)'; break;
    case 'jlind': s = 'L($' + hex(w, 4) + ')'; break;
    case 'mv': s = '$' + hex(ops[1], 2) + ',$' + hex(ops[0], 2); break;
    case 'brk': s = ''; break;
    default: {
      const isAbs = mode.indexOf('abs') >= 0;
      const ad = isAbs ? ops[0] | ops[1] << 8 : ops[0];
      const o = isAbs ? 2 : 1;
      const imm = mm ? ops[o] : ops[o] | ops[o + 1] << 8;
      const adr = '$' + hex(ad, isAbs ? 4 : 2) + (mode.endsWith('x') ? ',X' : '');
      if (mode.startsWith('bb')) {
        const r = ops[ops.length - 1];
        s = '#$' + hex(imm, mm ? 2 : 4) + ',' + adr + ',$' + hex((addr & 0xFF0000) | ((next + (r > 127 ? r - 256 : r)) & 0xFFFF), 6);
      } else s = '#$' + hex(imm, mm ? 2 : 4) + ',' + adr;
    }
  }
  return { len: next - addr, text: (name.padEnd(5) + ' ' + acc + s).trim(), name };
}

const api = { CPU, disasm, IRQS, hex };
if (typeof module !== 'undefined' && module.exports) module.exports = api;
else root.M7700 = api;
})(typeof window !== 'undefined' ? window : globalThis);
