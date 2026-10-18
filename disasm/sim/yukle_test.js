/* yukle_test.js - the read-only serial client (seri_istemci.js) against the simulated radio.
 *   jsc yukle_test.js -- ../../YA4000S.HEX [../../ozel_firmware/YA4000S_FREKANS_YATOLYE.HEX]
 * Checks: link, ROM header, EEPROM and flash backups byte for byte, firmware identification,
 * S-record export, nothing written to the radio, retries on a lossy line. */
'use strict';
var window = globalThis;
load('m7700.js'); load('board.js'); load('radio_view.js'); load('updater.js'); load('seri_istemci.js'); load('firmware.js');
const B = globalThis.Board, S = globalThis.SeriIstemci, { Board, parseHex, FCLK } = B;
const args = typeof scriptArgs !== 'undefined' ? scriptArgs : arguments;
let fails = 0;
const check = (name, ok, info) => { print((ok ? 'ok   ' : 'FAIL ') + name + (info !== undefined ? '  (' + info + ')' : '')); if (!ok) fails++; };

function radio(path) {
  const b = new Board(parseHex(readFile(path)));
  b.eeprom.mem.set(B.defaultEeprom());
  b.run(FCLK * (b.rom[0xE0E1] === 0x5C ? 8 : 2.5));      // boot (5 s longer with the yATOLYE screen)
  return b;
}

async function full(path, expectName) {
  const b = radio(path);
  const romBefore = b.rom.slice(), eeBefore = b.eeprom.mem.slice();
  const port = new S.SimPort(b, FCLK);
  const c = new S.RadioClient(port, { log: s => {} });
  const t0 = port.now();
  await c.connect();
  check(expectName + ': ping', await c.ping());
  const h = await c.readHeader();
  check(expectName + ': başlık', h.version === 'AS V2-3' && h.date === '23.11.1999' && h.texts.some(t => /SERISI TELSIZ/.test(t)), JSON.stringify(h));
  const ee = await c.readEeprom();
  check(expectName + ': EEPROM yedeği aynı', ee.every((v, i) => v === b.eeprom.mem[i]), ee.length + ' bayt');
  const ta = port.now();
  const img = await c.readFlash();
  let bad = 0; for (let a = 0x2000; a < 0x20000; a++) if (img[a] !== b.rom[a]) bad++;
  check(expectName + ': flash yedeği aynı', bad === 0, bad + ' fark, ' + ((port.now() - ta) / 1000).toFixed(0) + ' s');
  check(expectName + ': ROM toplamı başlıktakiyle aynı', S.romSum(img) === h.romSum, S.romSum(img).toString(16));
  const id = S.identify(img, window.FIRMWARE_HEX, parseHex);
  check(expectName + ': tanındı', id[0].name === expectName && id[0].diff === 0 && id[0].extra === 0, id.slice(0, 3).map(x => x.name + ':' + x.diff + '/' + x.extra).join(' '));
  const orig = parseHex(readFile(path)), back = parseHex(S.toSrec(img));
  let d2 = 0; for (const [a, v] of orig) if (a >= 0x2000 && back.get(a) !== v) d2++;
  for (const [a, v] of back) if (!orig.has(a) && v !== 0xFF) d2++;
  check(expectName + ': .HEX çıktısı orijinalle aynı bayt', d2 === 0, d2 + ' fark');
  let w = 0; for (let a = 0; a < 0x20000; a++) if (b.rom[a] !== romBefore[a]) w++;
  let we = 0; for (let a = 0; a < 2048; a++) if (b.eeprom.mem[a] !== eeBefore[a]) we++;
  check(expectName + ': telsize hiçbir şey yazılmadı', w === 0 && we === 0, 'flash ' + w + ', EEPROM ' + we);
  b.run(FCLK * 1);
  const lcd = V.decodeLcd(b, V.buildFont(b.rom)).text;
  check(expectName + ': telsiz çalışmaya devam ediyor', /CH 01|KANAL 01/.test(lcd), lcd);
  print('     toplam ' + ((port.now() - t0) / 1000).toFixed(0) + ' s (simülasyon zamanı)');
}

/* a line that loses one byte now and then: the client must retry and still read correctly */
async function lossy(path) {
  const b = radio(path);
  const port = new S.SimPort(b, FCLK);
  let n = 0, lost = 0;
  const w = port.write.bind(port);
  port.write = async bytes => { n++; if (n % 7 === 3 && bytes.length > 12) { lost++; bytes = Array.from(bytes); bytes.splice(8, 1); } return w(bytes); };
  const logs = [];
  const c = new S.RadioClient(port, { log: s => logs.push(s) });
  await c.connect();
  const ee = await c.readEeprom();
  const part = [];
  for (let a = 0x10000; a < 0x10800; a += 128) part.push(...await c.readMemoryBlock(a, 128));
  let bad = 0; for (let i = 0; i < part.length; i++) if (part[i] !== b.rom[0x10000 + i]) bad++;
  check('kayıplı hat: EEPROM ve bellek yine doğru', ee.every((v, i) => v === b.eeprom.mem[i]) && bad === 0, lost + ' bayt kayboldu, ' + logs.filter(s => /tekrar|sıra/.test(s)).length + ' tekrar');
}

async function noRadio() {
  /* nothing on the other end: a clear error instead of hanging */
  const fake = { onByte: null, t: 0, async write() {}, async sleep(ms) { this.t += ms; }, now() { return this.t; } };
  const c = new S.RadioClient(fake, {});
  let msg = '';
  try { await c.connect(); } catch (e) { msg = e.message; }
  check('telsiz yokken anlaşılır hata', /cevap vermiyor/.test(msg), msg);
}

const V = globalThis.RadioView;
(async () => {
  try {
    await full(args[0] || '../../YA4000S.HEX', (args[0] || 'YA4000S.HEX').split('/').pop());
    if (args[1]) await full(args[1], args[1].split('/').pop());
    await lossy(args[0] || '../../YA4000S.HEX');
    await noRadio();
  } catch (e) { check('istisna', false, e.message + ' ' + (e.stack || '')); }
  print(fails ? fails + ' test başarısız' : 'tüm seri istemci testleri geçti');
})();
