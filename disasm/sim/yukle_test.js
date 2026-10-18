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

/* ------------------------------------------------------------------ writing */
function bootText(b, sec) {
  const font = V.buildFont(b.rom), seen = [];
  for (let t = 0; t < sec; t += 0.5) { b.run(FCLK * 0.5); const l = V.decodeLcd(b, font).text; if (seen[seen.length - 1] !== l) seen.push(l); }
  return seen;
}
function sameFlash(b, image) {
  let bad = 0; for (let a = 0x2000; a < 0x20000; a++) { const v = image.has(a) ? image.get(a) : 0xFF; if (b.rom[a] !== v) bad++; }
  return bad;
}

async function write(basePath, newPath) {
  const ref = parseHex(readFile(basePath));
  const b = radio(basePath), eeBefore = b.eeprom.mem.slice();
  const port = new S.SimPort(b, FCLK);
  const logs = [];
  let c = new S.RadioClient(port, { log: s => logs.push(s) });
  await c.connect();
  const chk = S.checkImage(parseHex(readFile(newPath)), ref);
  check('denetim: ' + newPath.split('/').pop() + ' kabul', chk.ok, chk.errors.join('; ') + ' ' + chk.notes.join('; '));
  const t0 = port.now();
  await c.writeFirmware(chk.image);
  check('yükleme tamamlandı', true, ((port.now() - t0) / 1000).toFixed(0) + ' s');
  check('flash dosyayla aynı', sameFlash(b, chk.image) === 0, sameFlash(b, chk.image) + ' fark');
  check('EEPROM\'a dokunulmadı', b.eeprom.mem.every((v, i) => v === eeBefore[i]));
  const seen = bootText(b, 9);
  check('yeni firmware açıldı: yATOLYE, sonra kanal', seen.includes('yATOLYE ') && /KANAL 01|CH 01/.test(seen[seen.length - 1]), seen.join('|'));

  /* the new firmware must keep the loader: write the original back */
  c = new S.RadioClient(port, { log: s => logs.push(s) });
  await c.connect();
  const back = S.checkImage(ref, ref);
  await c.writeFirmware(back.image);
  check('yeni firmware de güncellenebiliyor: orijinal geri yüklendi', sameFlash(b, back.image) === 0);
  const seen2 = bootText(b, 4);
  check('orijinal açıldı', !seen2.includes('yATOLYE ') && /CH 01|KANAL 01/.test(seen2[seen2.length - 1]), seen2.join('|'));
}

/* the link breaks in the middle: the loader keeps running from RAM; reconnect and start again */
async function interrupted(basePath, newPath) {
  const ref = parseHex(readFile(basePath));
  const b = radio(basePath);
  const port = new S.SimPort(b, FCLK);
  const chk = S.checkImage(parseHex(readFile(newPath)), ref);
  let c = new S.RadioClient(port, {});
  await c.connect();
  let msg = '';
  try { await c.writeFirmware(chk.image, { onProgress: f => { if (f > 0.3) throw new Error('kablo çekildi'); } }); } catch (e) { msg = e.message; }
  b.run(FCLK * 3);                                   // a pause; the radio stays powered
  check('yarıda kesildi', msg === 'kablo çekildi', msg);
  c = new S.RadioClient(port, {});                   // a fresh page: new client, sequence numbers from 1
  await c.connect();
  await c.writeFirmware(chk.image);
  check('yeniden bağlanıp tamamlandı', sameFlash(b, chk.image) === 0);
  const seen = bootText(b, 9);
  check('açıldı', seen.includes('yATOLYE '), seen.join('|'));
}

/* no checksum on the link: a byte lost inside a write frame is programmed shifted.  Two bytes are lost
 * in the first attempt; the final read-back must catch it and the client must start over by itself. */
async function lossyWrite(basePath, newPath) {
  const ref = parseHex(readFile(basePath));
  const b = radio(basePath);
  const port = new S.SimPort(b, FCLK);
  let n = 0, lost = 0;
  const w = port.write.bind(port);
  port.write = async bytes => { n++; if ((n === 120 || n === 900) && bytes.length > 40) { lost++; bytes = Array.from(bytes); bytes.splice(30, 1); } return w(bytes); };
  const logs = [];
  const c = new S.RadioClient(port, { log: s => logs.push(s) });
  await c.connect();
  const chk = S.checkImage(parseHex(readFile(newPath)), ref);
  await c.writeFirmware(chk.image);
  check('kayıplı hat: doğrulama yakaladı, kendiliğinden baştan yazıldı', sameFlash(b, chk.image) === 0 && logs.some(s => /deneme 2/.test(s)),
        lost + ' bayt kayboldu; ' + logs.filter(s => /doğrulama|deneme/.test(s)).join(' / '));
  const seen = bootText(b, 9);
  check('açıldı', seen.includes('yATOLYE '), seen.join('|'));

  /* a line that keeps losing bytes: a clear error after 3 attempts, the radio stays in the loader */
  const b2 = radio(basePath), p2 = new S.SimPort(b2, FCLK);
  let m = 0; const w2 = p2.write.bind(p2);
  p2.write = async bytes => { m++; if (m % 50 === 7 && bytes.length > 40) { bytes = Array.from(bytes); bytes.splice(30, 1); } return w2(bytes); };
  const c2 = new S.RadioClient(p2, {});
  await c2.connect();
  let msg = '';
  try { await c2.writeFirmware(chk.image); } catch (e) { msg = e.message; }
  check('sürekli bozuk hat: 3 denemeden sonra anlaşılır hata', /3 denemede de/.test(msg) && /KAPATMAYIN/.test(msg), msg.slice(0, 90));
  p2.write = w2;                                         // the cable is fixed: reconnect, write again
  const c3 = new S.RadioClient(p2, {});
  await c3.connect();
  await c3.writeFirmware(chk.image);
  check('kablo düzelince aynı telsize yükleme tamamlandı', sameFlash(b2, chk.image) === 0);
}

function refusals(basePath) {
  const ref = parseHex(readFile(basePath));
  const noLoader = new Map(ref); for (let a = S.LOADER_LO; a < S.LOADER_HI; a++) noLoader.delete(a);
  const badLoader = new Map(ref); badLoader.set(S.LOADER_LO + 10, badLoader.get(S.LOADER_LO + 10) ^ 1);
  const noVec = new Map(ref); noVec.delete(0xFFFE); noVec.delete(0xFFFF);
  const r1 = S.checkImage(noLoader, ref), r2 = S.checkImage(badLoader, ref), r3 = S.checkImage(noVec, ref);
  check('reddedilir: yükleyici eksik', !r1.ok && /eksik/.test(r1.errors[0]), r1.errors[0]);
  check('reddedilir: yükleyici değiştirilmiş', !r2.ok && /farklı/.test(r2.errors[0]), r2.errors[0]);
  check('reddedilir: reset vektörü yok', !r3.ok && /reset/.test(r3.errors[0]), r3.errors[0]);
  const patched = new Map(ref); patched.set(0x2DE9, 0x4B);   // a byte in the checksummed area, sum not fixed
  const r4 = S.checkImage(patched, ref);
  check('sağlama toplamı düzeltilir', r4.ok && r4.notes.some(s => /düzeltildi/.test(s)) && U2.checkRomChecksum(r4.image).ok, r4.notes.join('; '));
}

const V = globalThis.RadioView, U2 = globalThis.Updater;
(async () => {
  try {
    await full(args[0] || '../../YA4000S.HEX', (args[0] || 'YA4000S.HEX').split('/').pop());
    if (args[1]) await full(args[1], args[1].split('/').pop());
    await lossy(args[0] || '../../YA4000S.HEX');
    await noRadio();
    refusals(args[0] || '../../YA4000S.HEX');
    if (args[1]) {
      await write(args[0], args[1]);
      await interrupted(args[0], args[1]);
      await lossyWrite(args[0], args[1]);
    }
  } catch (e) { check('istisna', false, e.message + ' ' + (e.stack || '')); }
  print(fails ? fails + ' test başarısız' : 'tüm seri istemci testleri geçti');
})();
