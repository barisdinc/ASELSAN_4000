#!/usr/bin/env node
/* Headless run of the simulator: node run_node.js ../../YA4000S.HEX [seconds] [script]
 * script: semicolon separated actions with times, e.g. "1.0 ptt 1; 2.0 ptt 0; 2.5 key 5"
 */
'use strict';
/* runs under node, or under macOS JavaScriptCore:
 *   jsc run_node.js -- ../../YA4000S.HEX 3 "script"   (env via ENV="TRACE=1 ...") */
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let fs, B, V, args, env, out;
if (isNode) {
  fs = require('fs'); B = require('./board.js'); V = require('./radio_view.js');
  args = process.argv.slice(2); env = process.env; out = s => process.stdout.write(s);
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js');
  B = globalThis.Board; V = globalThis.RadioView; args = typeof scriptArgs !== "undefined" ? scriptArgs : arguments;
  env = {}; (args[3] || '').split(/\s+/).filter(Boolean).forEach(kv => { const i = kv.indexOf('='); env[kv.slice(0, i)] = kv.slice(i + 1); });
  fs = { readFileSync: (p, enc) => enc ? readFile(p) : new Uint8Array(readFile(p, 'binary')), writeFileSync: (p, d) => writeFile(p, d) };
  out = s => (globalThis._buf = (globalThis._buf || '') + s);
  globalThis.console = { log: (...a) => { print((globalThis._buf || '') + a.join(' ')); globalThis._buf = ''; } };
  globalThis.process = { env };
}
const { Board, parseHex, FCLK } = B;
const path = args[0] || '../../YA4000S.HEX';
const secs = parseFloat(args[1] || '3');
const script = (args[2] || '').split(';').map(s => s.trim()).filter(Boolean).map(s => s.split(/\s+/));
const b = new Board(parseHex(fs.readFileSync(path, 'utf8')));
if (env.EEPROM) b.eeprom.mem.set(fs.readFileSync(env.EEPROM));
if (env.INPUTS) Object.assign(b.inputs, JSON.parse(env.INPUTS));
const font = V.buildFont(b.rom);
b.onSerial = v => out('[U0 ' + v.toString(16).padStart(2, '0') + ']');
const step = 0.1;
let lastLcd = '', lastEv = 0, lastCb = 0;
const t0 = Date.now();
for (let t = 0; t < secs; t += step) {
  for (const a of script) if (Math.abs(parseFloat(a[0]) - t) < step / 2) {
    const [, cmd, arg] = a;
    console.log(`  >> ${t.toFixed(1)}s ${cmd} ${arg || ''}`);
    if (cmd === 'p4') b.inputs.P4 = parseInt(arg, 16);
    if (cmd === 'p5') b.inputs.P5 = parseInt(arg, 16);
    if (cmd === 'panel') b.panel.setExt(parseInt(arg, 16), b);
    if (cmd === 'key') b.keypad.press(parseInt(arg, 16), true, b);
    if (cmd === 'keyup') b.keypad.press(parseInt(arg, 16), false, b);
  }
  let r;
  try { r = b.run(FCLK * step); }
  catch (e) { console.log('EXC', e.message, 'PC', b.cpu.prevPC.toString(16)); break; }
  const lcd = V.decodeLcd(b, font);
  const s = `${lcd.on ? 'ON ' : 'off'} [${lcd.chan}] "${lcd.text}" sym=${lcd.symbols.toString(16)} buf="${V.textBuffer(b)}"`;
  if (s !== lastLcd) { console.log(t.toFixed(1).padStart(5) + 's LCD ' + s); lastLcd = s; }
  while (lastEv < b.events.length) { const e = b.events[lastEv++]; console.log('       ev ' + (e[0] / FCLK).toFixed(3) + ' ' + e[1]); }
  if (env.CBUS) while (lastCb < b.cbusLog.length) { const e = b.cbusLog[lastCb++]; console.log('       cb ' + (e[0] / FCLK).toFixed(3) + ' ' + e[1] + ' ' + e[2].toString(16)); }
  if (env.TRACE) {
    const rs = V.analyseTx(b, FCLK, FCLK * 0.05), st = b.radioState();
    console.log(`       vco=${(st.vco / 1e6).toFixed(4)} lock=${st.locked} hold=${st.powerHold} P55rx=${st.rxSelect} P72=${st.txAudio} P50=${st.alert} P46=${st.squelchInfo} pwm=${st.pwrPwm === null ? '-' : st.pwrPwm.toFixed(2)} tks=${st.ctcssPwm} ${JSON.stringify(rs)} p5=${b.latch[0x0B].toString(16)} p7=${b.latch[0x0F].toString(16)} p4=${b.latch[0x0A].toString(16)}`);
  }
}
const st = b.radioState();
console.log(`done ${(Date.now() - t0) / 1000}s wall, PC ${b.cpu.PG.toString(16)}:${b.cpu.PC.toString(16)} waiting=${b.cpu.waiting} stopped=${b.cpu.stopped}`);
console.log('synth', JSON.stringify({ R: st.synth.R, N: st.synth.Nc, A: st.synth.Ac, SW: st.synth.SW, vco: st.vco, loads: st.synth.loads }));
console.log('ram 0x230..0x24f', Array.from(b.ram.slice(0x230, 0x250)).map(x => x.toString(16).padStart(2, '0')).join(' '));
console.log('eeprom dirty', b.eeprom.dirty, 'romWrites', b.romWrites);
if (env.EEOUT) fs.writeFileSync(env.EEOUT, b.eeprom.mem);
