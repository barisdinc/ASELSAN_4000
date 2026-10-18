/* test_ozel.js - özel firmware örneklerinin simülatör testi.
 *   node test_ozel.js ../../ozel_firmware/YA4000S_HESAP.HEX ../../YA4000S.HEX NUMBER,CALC
 *   jsc test_ozel.js -- ../../ozel_firmware/YA4000S_HESAP.HEX ../../YA4000S.HEX NUMBER,CALC
 * (ikinci dosya orijinal firmware: diğer menülerin değişmediğini karşılaştırmak için)
 * Üçüncü bağımsız değişken menüde LIST'ten sonra eklenen öğeler: "NUMBER" (varsayılan) ya da
 * "NUMBER,CALC,GAME,FREQ"; testler buna göre seçilir.
 * Tuşlara basıp her tuştan sonra LCD'yi okur ve beklenenle karşılaştırır. */
'use strict';
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let B, V, hexText, origText, ringArg, out;
if (isNode) {
  B = require('./board.js'); V = require('./radio_view.js'); globalThis.UPD = require('./updater.js');
  hexText = require('fs').readFileSync(process.argv[2], 'utf8'); out = s => console.log(s);
  origText = process.argv[3] && require('fs').readFileSync(process.argv[3], 'utf8');
  ringArg = process.argv[4];
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js'); load('updater.js');
  B = globalThis.Board; V = globalThis.RadioView; globalThis.UPD = globalThis.Updater;
  const args = typeof scriptArgs !== 'undefined' ? scriptArgs : arguments;
  hexText = readFile(args[0]); out = s => print(s);
  origText = args[1] && readFile(args[1]);
  ringArg = args[2];
}
const { Board, parseHex, FCLK } = B;
const rom = parseHex(hexText);
let fails = 0;
function check(name, ok, info) { out((ok ? 'ok   ' : 'FAIL ') + name + (info ? '  (' + info + ')' : '')); if (!ok) fails++; }
function radio(eeMod) {
  const b = new Board(rom), ee = B.defaultEeprom();
  if (eeMod) eeMod(ee);
  B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom);
  return b;
}
const lcd = b => V.decodeLcd(b, b.font).text;
const KP = '*0#q789p456s123m';
const RING = (ringArg || 'NUMBER').split(',');
let last;          // titles inserted after LIST
const AFTER_NUMBER = RING[RING.indexOf('NUMBER') + 1] || 'KANAL 01';
function keys(b, seq) {
  const screens = [];
  for (const k of seq) {
    const i = KP.indexOf(k);
    b.keypad.press(i, true, b); b.run(FCLK * 0.15); b.keypad.press(i, false, b); b.run(FCLK * 0.5);
    screens.push(lcd(b));
  }
  return screens;
}
/* "m m s 1 2 3 ..." -> ekranlar; beklenenle karşılaştır */
function scenario(b, name, seq, expect) {
  const got = keys(b, seq.replace(/ /g, ''));
  const exp = expect.split('|');
  const ok = got.length === exp.length && got.every((g, i) => g === exp[i].padEnd(8));
  check(name, ok, got.map(s => '"' + s + '"').join(' '));
  return ok;
}

/* SAYI / TERS (ozel_firmware/ornekler/sayi_ters.asm) */
{
  const b = radio();
  b.run(FCLK * 2.5);
  check('açılış normal', lcd(b) === 'KANAL 01', lcd(b));
  scenario(b, 'menü: LIST → NUMBER', 'm m', 'LIST|NUMBER');
  scenario(b, 's: SAYI moduna gir', 's', '_');
  scenario(b, 'rakamlar', '1 2 3 4', '1_|12_|123_|1234_');
  scenario(b, 's: ters, s: düz', 's s', '4321|1234_');
  scenario(b, '* siler, sonra yeni rakam', '* 9', '123_|1239_');
  scenario(b, '8 haneye kadar, fazlası yok sayılır', '5 6 7 8 0', '12395_|123956_|1239567_|12395678|12395678');
  scenario(b, 'ters (8 hane)', 's', '87659321');
  scenario(b, '# temizler', '# 7 s', '_|7_|7');
  scenario(b, 'm: çıkış', 'm', 'KANAL 01');
  scenario(b, 'tekrar gir: tampon boş', 'm m s', 'LIST|NUMBER|_');
  scenario(b, 'menü halkası NUMBER\'dan devam ediyor', 'm m m', 'KANAL 01|LIST|NUMBER');
  scenario(b, 'SAYI başlığında m → sonraki öğe', 'm', AFTER_NUMBER);
  if (AFTER_NUMBER !== 'KANAL 01') keys(b, 'm'.repeat(RING.length - RING.indexOf('NUMBER') - 1));
  /* the radio still works: channel switch while in SAYI mode */
  keys(b, 'mms12');
  b.panel.setExt(((16 - 3) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.5);
  const st = b.radioState();
  check('SAYI modunda kanal değişimi sentezörü yüklüyor', (st.vco / 1e6).toFixed(4) === '201.0500', lcd(b) + ' VCO ' + (st.vco / 1e6).toFixed(4));
}

/* Turkish menu texts: EE 0x018 bit 7 */
{
  const b = radio(ee => { ee[0x018] |= 0x80; });
  b.run(FCLK * 2.5);
  scenario(b, 'Türkçe menü: SAYI', 'm m s 4 2 s', 'LISTE|SAYI|_|4_|42_|24');
}

/* the other menus behave exactly as in the original firmware (same screens key by key) */
if (origText) {
  const orig = parseHex(origText);
  // [original keys, patched keys]: SUB MENU is one m further in the patched ring (after NUMBER)
  const run = (r, seq) => { const b = new Board(r), ee = B.defaultEeprom(); ee[0x018] |= 0x20; B.eeFixSums(ee); b.eeprom.mem.set(ee);
    b.font = V.buildFont(b.rom); b.run(FCLK * 2.5); return keys(b, seq).join('|'); };
  const extra = 'm'.repeat(RING.length);
  for (const [so, sp] of [['ms05mmsms', 'ms05mmsms'], ['p', 'p'], ['mmsmsmmsm', 'm' + extra + 'msmsmmsm']]) {
    let c = run(rom, sp); for (const t of RING) c = c.replace('|' + t.padEnd(8), '');
    const a = run(orig, so);
    check('orijinalle aynı: ' + so, a === c, a + '  /  ' + c);
  }
  const b = new Board(rom), ee = B.defaultEeprom(); ee[0x018] |= 0x20; B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom); b.run(FCLK * 2.5);
  scenario(b, 'halka (SUB MENU açık): normal → LIST → ' + RING.join(' → ') + ' → SUB MENU → normal',
    'mmm' + extra, ['LIST', ...RING, 'SUB MENU', 'KANAL 01'].join('|'));
}

/* HESAP / CALC (ozel_firmware/ornekler/hesap.inc) */
if (RING.includes('CALC')) {
  const enter = 'mm' + 'm'.repeat(RING.indexOf('CALC')) + 's';
  const b = radio();
  b.run(FCLK * 2.5);
  last = (name, seq, exp) => { const g = keys(b, seq.replace(/ /g, '')); const l = g[g.length - 1];
    check(name, l === exp.padStart(8), '"' + l + '"'); };
  scenario(b, 'HESAP: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('CALC') + 1), '       0'].join('|'));
  scenario(b, 'sayı sağa dayalı', '1 2', '       1|      12');
  scenario(b, '* işlem seçer ve döndürür', '* * *', '+     12|-     12|*     12');
  scenario(b, 'ikinci sayı, eşittir: 12*3', '3 #', '*      3|      36');
  scenario(b, 'sonuçla devam: 36-40 = -4', '* * 4 0 #', '+     36|-     36|-      4|-     40|      -4');
  scenario(b, 'bölme sıfıra doğru keser: -4/3 = -1', '* * * * 3 #', '+     -4|-     -4|*     -4|/     -4|/      3|      -1');
  last('zincirleme: 10+5 sonra -3 = 12', 's 1 0 * 5 * * 3 #', '12');
  last('büyük çarpım: 12345*6789', 's 1 2 3 4 5 * * * 6 7 8 9 #', '83810205');
  last('9. hane yok sayılır', 's 1 2 3 4 5 6 7 8 9', '12345678');
  last('en büyük', 's 9 9 9 9 9 9 9 8 * 1 #', '99999999');
  last('taşma: 99999999+1', 's 9 9 9 9 9 9 9 9 * 1 #', 'ERROR   ');
  last('hatada yalnız s çalışır', '1 2 *', 'ERROR   ');
  last('s hatayı siler', 's', '0');
  last('çarpma taşması', '9 9 9 9 9 9 9 9 * * * 9 9 9 9 9 9 9 9 #', 'ERROR   ');
  last('sıfıra bölme', 's 5 * * * * 0 #', 'ERROR   ');
  last('en küçük: 1-9999999', 's 1 * * 9 9 9 9 9 9 9 #', '-9999998');
  last('aralık altı: -9999998-2', '* * 2 #', 'ERROR   ');
  last('sonuçtan sonra rakam yeni hesap başlatır', 's 2 * 3 # 4 * 1 #', '5');
  last('ikinci sayı yoksa # işlemi bırakır', 's 7 * #', '7');
  scenario(b, 'm: çıkış', 'm', 'KANAL 01');
  const t = radio(ee => { ee[0x018] |= 0x80; });
  t.run(FCLK * 2.5);
  const g = keys(t, enter + '1****0#');
  check('Türkçe: HESAP / HATA', g[RING.indexOf('CALC') + 1] === 'HESAP   ' && g[g.length - 1] === 'HATA    ', g.join('|'));
}

/* OYUN / GAME (ozel_firmware/ornekler/oyun.inc) */
if (RING.includes('GAME')) {
  const enter = 'mm' + 'm'.repeat(RING.indexOf('GAME')) + 's';
  const OY = 0x500 + 0x20, target = b => b.ram[OY + 2] | b.ram[OY + 3] << 8;
  const beeps = (b, from) => b.events.filter(e => e[0] >= from && /^BİP \d/.test(e[1])).map(e => +e[1].split(' ')[1]);
  const near = (f, want) => Math.abs(f - want) / want < 0.02;
  const b = radio();
  b.run(FCLK * 2.5);
  scenario(b, 'OYUN: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('GAME') + 1), '___'].join('|'));
  const t0 = target(b);
  check('tutulan sayı 1..999', t0 >= 1 && t0 <= 999, t0);
  scenario(b, 'haneler, * siler, 4. hane yok sayılır', '1 2 * 5 0 9', '1__|12_|1__|15_|150|150');
  scenario(b, 's: yeni oyun', 's', '___');
  /* one guess with the expected direction and sound */
  let t = target(b), g = t > 500 ? 250 : 750, now = b.now;
  keys(b, String(g)); const scr = keys(b, '#')[0];
  const dir = g < t ? '^' : 'v', bp = beeps(b, now);
  check('tahmin: yön ve deneme sayısı', scr === String(g) + ' ' + dir + '  1', '"' + scr + '" hedef ' + t);
  check('yön sesi (' + (dir === '^' ? '1500' : '400') + ' Hz)', bp.some(f => near(f, dir === '^' ? 1500 : 400)), bp.join(','));
  scenario(b, 'gönderilen tahmin silinmez, tekrar gönderilmez', '* #', scr + '|' + scr);
  scenario(b, 'yeni rakam yeni tahmin başlatır', '7', '7__ ' + dir + '  1');
  /* solve by binary search reading only the screen */
  keys(b, 's');
  let lo = 1, hi = 999, n = 0, last = '';
  now = b.now;
  while (n < 12) {
    const mid = (lo + hi) >> 1; n++;
    keys(b, String(mid)); last = keys(b, '#')[0];
    if (last.startsWith('WIN')) break;
    if (last[4] === '^') lo = mid + 1; else hi = mid - 1;
  }
  check('ikili arama en çok 10 tahminde bilir', last === ('WIN' + String(n).padStart(5)) && n <= 10, '"' + last + '" ' + n + ' tahmin');
  const mel = beeps(b, b.now - FCLK * 1.2);
  const want = [523, 659, 784, 1047];
  let k = 0; for (const f of mel) if (k < 4 && near(f, want[k])) k++;
  check('melodi: do mi sol do\'', k === 4, mel.map(f => f.toFixed(0)).join(','));
  scenario(b, 'bildikten sonra her tuş yeni oyun', '5', '___');
  /* randomness: 40 new games */
  const seen = new Set(); let inRange = true;
  for (let i = 0; i < 40; i++) { keys(b, 's'); b.run(FCLK * 0.013 * (i % 7)); const v = target(b); seen.add(v); if (v < 1 || v > 999) inRange = false; }
  check('40 oyunda sayılar aralıkta ve çeşitli', inRange && seen.size >= 35, seen.size + ' farklı: ' + [...seen].slice(0, 10).join(' ') + ' …');
  scenario(b, 'm: çıkış', 'm', 'KANAL 01');
  const tr = radio(ee => { ee[0x018] |= 0x80; });
  tr.run(FCLK * 2.5); keys(tr, enter);
  const tt = target(tr); keys(tr, String(tt)); const w = keys(tr, '#')[0];
  check('Türkçe: BULDU', w === 'BULDU  1', '"' + w + '"');
}

/* FREKANS / FREQ (ozel_firmware/ornekler/frekans.inc) */
if (RING.includes('FREQ')) {
  const enter = 'mm' + 'm'.repeat(RING.indexOf('FREQ')) + 's';
  const UHF = ee => { ee[0x58C] = 16000 >> 8; ee[0x58D] = 16000 & 255; B.eeSetChannel(ee, 1, 433e6, 433e6, { name: 'UHF' }); };
  const vco = b => (b.radioState().vco / 1e6).toFixed(4);
  const rec = b => Array.from(b.ram.slice(0x370, 0x37D));
  const synthBytes = r => r.slice(0, 5).concat(r.slice(7, 12)).map(x => x.toString(16).padStart(2, '0')).join(' ');
  /* the firmware's own words for a frequency: put it into EEPROM channel 1 of the original image */
  const reference = (hz, uhf) => { const o = new Board(origText ? parseHex(origText) : rom), ee = B.defaultEeprom();
    if (uhf) UHF(ee); B.eeSetChannel(ee, 1, hz, hz, { name: 'REF' }); B.eeFixSums(ee); o.eeprom.mem.set(ee);
    o.font = V.buildFont(o.rom); o.run(FCLK * 2.6); return synthBytes(rec(o)); };
  const b = radio();
  b.run(FCLK * 2.5);
  scenario(b, 'FREKANS: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('FREQ') + 1), '___ ____'].join('|'));
  scenario(b, 'haneler, 3. haneden sonra boşluk', '1 5 6 5 2 5', '1__ ____|15_ ____|156 ____|156 5___|156 52__|156 525_');
  scenario(b, '# ayarlar (6 hane = kHz)', '#', '156 5250');
  check('RX VCO = 156,525 + 45', vco(b) === '201.5250', vco(b));
  check('sentezör baytları firmware ile aynı (156,525)', synthBytes(rec(b)) === reference(156.525e6), synthBytes(rec(b)) + ' / ' + reference(156.525e6));
  scenario(b, 'normal ekranda frekans', 'm', '156 5250');
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
  check('PTT: TX VCO = 156,525 (simplex)', vco(b) === '156.5250', vco(b));
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  check('PTT bırakıldı: RX 201,525', vco(b) === '201.5250', vco(b));
  for (const [ks, f, name] of [['1565125', 156.5125e6, '12,5 kHz adım'], ['156010', 156.010e6, '10 kHz adım'], ['1560050', 156.005e6, '5 kHz adım'], ['176900', 176.9e6, 'bant üst ucu']]) {
    keys(b, enter + 's' + ks + '#');
    const want = ((f + 45e6) / 1e6).toFixed(4);
    check(name + ': ' + (f / 1e6) + ' MHz', vco(b) === want && synthBytes(rec(b)) === reference(f), vco(b) + ' ' + synthBytes(rec(b)) + ' / ' + reference(f));
    keys(b, 'm');
  }
  keys(b, enter + 's');
  scenario(b, '2,5 kHz ızgarası dışı: hata', '1 5 6 0 0 1 0 #', '1__ ____|15_ ____|156 ____|156 0___|156 00__|156 001_|156 0010|ERROR   ');
  scenario(b, 'hatadan sonraki tuş girişe döner', '*', '156 0010');
  scenario(b, '8. hane yok sayılır', '5', '156 0010');
  last = (name, seq, exp) => { const g = keys(b, seq.replace(/ /g, '')); check(name, g[g.length - 1] === exp.padEnd(8), '"' + g[g.length - 1] + '"'); };
  last('6 haneden az: hata', 's 1 5 6 5 2 #', 'ERROR');
  last('bandın altı: hata', '* s 1 2 0 0 0 0 #', 'ERROR');
  last('bandın üstü: hata', '* s 1 7 7 0 0 0 #', 'ERROR');
  check('hatalı girişte frekans değişmedi', vco(b) === '221.9000', vco(b));
  keys(b, '*m');
  b.panel.setExt(((16 - 3) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.6);
  check('kanal anahtarı: kanala dönüş', vco(b) === '201.0500' && lcd(b) === 'KANAL 03', vco(b) + ' ' + lcd(b));
  /* UHF configuration (base 400 MHz, -45 MHz injection, prescaler 128) */
  const u = radio(UHF);
  u.run(FCLK * 2.5);
  keys(u, enter + '433125#');
  check('UHF: 433,125 MHz, RX VCO 388,125', vco(u) === '388.1250' && synthBytes(rec(u)) === reference(433.125e6, true), vco(u) + ' ' + synthBytes(rec(u)) + ' / ' + reference(433.125e6, true));
  keys(u, 'm'); u.inputs.P4 = 0x7F; u.run(FCLK * 0.6);
  check('UHF PTT: TX 433,125', vco(u) === '433.1250', vco(u));
  const t = radio(ee => { ee[0x018] |= 0x80; });
  t.run(FCLK * 2.5);
  const g = keys(t, enter + '1#');
  check('Türkçe: FREKANS / HATA', g[RING.indexOf('FREQ') + 1] === 'FREKANS ' && g[g.length - 1] === 'HATA    ', g.join('|'));
}

/* the real device is programmed through the serial loader: flash the new image into a
 * radio that runs the original firmware, reset, and try the new mode */
if (origText) {
  const b = new Board(parseHex(origText)), ee = B.defaultEeprom(); B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom); b.run(FCLK * 2.6);
  const up = new UPD.FlashUpdater(b, rom, { verify: true });
  while (!up.done && b.now < FCLK * 600) { b.run(FCLK * 0.01); up.tick(); }
  check('seri yükleyiciyle yazıldı ve doğrulandı', !up.error, up.error || up.log.slice(-1)[0]);
  b.font = V.buildFont(b.rom); b.run(FCLK * 3);
  scenario(b, 'yüklemeden sonra SAYI çalışıyor', 'mms 2 0 2 6 s', 'LIST|NUMBER|_|2_|20_|202_|2026_|6202');
  if (RING.includes('FREQ')) { keys(b, 'm'); const g = keys(b, 'mmmmms156525#'); check('yüklemeden sonra FREKANS çalışıyor', g[g.length - 1] === '156 5250' && (b.radioState().vco / 1e6).toFixed(4) === '201.5250', g.join('|')); }
  if (RING.includes('GAME')) { keys(b, 'm'); const g = keys(b, 'mmmms123#'); check('yüklemeden sonra OYUN çalışıyor', g[4] === '___     ' && /^123 [v^]  1$|^WIN    1$/.test(g[8]), g.join('|')); }
  if (RING.includes('CALC')) { keys(b, 'm'); scenario(b, 'yüklemeden sonra HESAP çalışıyor', 'mmms 7 * * * 6 #', 'LIST|NUMBER|CALC|       0|       7|+      7|-      7|*      7|*      6|      42'); }
}

out(fails ? fails + ' test başarısız' : 'tüm özel firmware testleri geçti');
if (isNode && fails) process.exit(1);
