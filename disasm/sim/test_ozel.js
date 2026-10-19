/* test_ozel.js - özel firmware örneklerinin simülatör testi.
 *   node test_ozel.js ../../ozel_firmware/YA4000S_HESAP.HEX ../../YA4000S.HEX NUMBER,CALC
 *   jsc test_ozel.js -- ../../ozel_firmware/YA4000S_HESAP.HEX ../../YA4000S.HEX NUMBER,CALC
 * (ikinci dosya orijinal firmware: diğer menülerin değişmediğini karşılaştırmak için)
 * Üçüncü bağımsız değişken menüde LIST'ten sonra eklenen öğeler: "NUMBER" (varsayılan) ya da
 * "NUMBER,CALC,GAME" ya da "FREQ,APRS" (FREKANS firmware'i); testler buna göre seçilir.
 * Tuşlara basıp her tuştan sonra LCD'yi okur ve beklenenle karşılaştırır. */
'use strict';
const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;
let B, V, hexText, origText, ringArg, out;
if (isNode) {
  B = require('./board.js'); V = require('./radio_view.js'); globalThis.UPD = require('./updater.js'); globalThis.AFSK = require('./afsk.js'); globalThis.P4000 = require('./p4000.js');
  hexText = require('fs').readFileSync(process.argv[2], 'utf8'); out = s => console.log(s);
  origText = process.argv[3] && require('fs').readFileSync(process.argv[3], 'utf8');
  ringArg = process.argv[4];
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js'); load('updater.js'); load('afsk.js'); load('p4000.js');
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
/* power-on wait; firmware with the 5 s boot screen (acilis_yatolye.inc: JMPL at F_00E0E1) needs longer */
function boot(b, s) { b.run(FCLK * (s + (b.rom[0xE0E1] === 0x5C ? 5.5 : 0))); }
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
if (RING.includes('NUMBER')) {
  const b = radio();
  boot(b, 2.5);
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
if (RING.includes('NUMBER')) {
  const b = radio(ee => { ee[0x018] |= 0x80; });
  boot(b, 2.5);
  scenario(b, 'Türkçe menü: SAYI', 'm m s 4 2 s', 'LISTE|SAYI|_|4_|42_|24');
}

/* the other menus behave exactly as in the original firmware (same screens key by key) */
if (origText) {
  const orig = parseHex(origText);
  // [original keys, patched keys]: SUB MENU is one m further in the patched ring (after NUMBER)
  const run = (r, seq) => { const b = new Board(r), ee = B.defaultEeprom(); ee[0x018] |= 0x20; B.eeFixSums(ee); b.eeprom.mem.set(ee);
    b.font = V.buildFont(b.rom); boot(b, 2.5); return keys(b, seq).join('|'); };
  const extra = 'm'.repeat(RING.length);
  for (const [so, sp] of [['ms05mmsms', 'ms05mmsms'], ['p', 'p'], ['mmsmsmmsm', 'm' + extra + 'msmsmmsm']]) {
    let c = run(rom, sp); for (const t of RING) c = c.replace('|' + t.padEnd(8), '');
    const a = run(orig, so);
    check('orijinalle aynı: ' + so, a === c, a + '  /  ' + c);
  }
  const b = new Board(rom), ee = B.defaultEeprom(); ee[0x018] |= 0x20; B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom); boot(b, 2.5);
  scenario(b, 'halka (SUB MENU açık): normal → LIST → ' + RING.join(' → ') + ' → SUB MENU → normal',
    'mmm' + extra, ['LIST', ...RING, 'SUB MENU', 'KANAL 01'].join('|'));
}

/* HESAP / CALC (ozel_firmware/ornekler/hesap.inc) */
if (RING.includes('CALC')) {
  const enter = 'mm' + 'm'.repeat(RING.indexOf('CALC')) + 's';
  const b = radio();
  boot(b, 2.5);
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
  boot(t, 2.5);
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
  boot(b, 2.5);
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
  boot(tr, 2.5); keys(tr, enter);
  const tt = target(tr); keys(tr, String(tt)); const w = keys(tr, '#')[0];
  check('Türkçe: BULDU', w === 'BULDU  1', '"' + w + '"');
}

/* FREKANS / FREQ (ozel_firmware/ornekler/frekans.inc) */
if (RING.includes('FREQ')) {
  const enter = 'mm' + 'm'.repeat(RING.indexOf('FREQ')) + 's';
  const UHF = ee => { ee[0x58C] = 16000 >> 8; ee[0x58D] = 16000 & 255; B.eeSetChannel(ee, 1, 433e6, 433e6, { name: 'UHF' }); };
  const vco = b => (b.radioState().vco / 1e6).toFixed(4);
  const chan = b => V.decodeLcd(b, b.font).chan;
  const rec = b => Array.from(b.ram.slice(0x370, 0x37D));
  const synthBytes = r => r.slice(0, 5).concat(r.slice(7, 12)).map(x => x.toString(16).padStart(2, '0')).join(' ');
  /* the firmware's own words for a frequency: put it into EEPROM channel 1 of the original image */
  const reference = (hz, uhf, txHz) => { const o = new Board(origText ? parseHex(origText) : rom), ee = B.defaultEeprom();
    if (uhf) UHF(ee); B.eeSetChannel(ee, 1, hz, txHz || hz, { name: 'REF' }); B.eeFixSums(ee); o.eeprom.mem.set(ee);
    o.font = V.buildFont(o.rom); boot(o, 2.6); return synthBytes(rec(o)); };
  const b = radio();
  boot(b, 2.5);
  scenario(b, 'FREKANS: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('FREQ') + 1), '___ ____'].join('|'));
  scenario(b, 'haneler, 3. haneden sonra boşluk', '1 5 6 5 2 5', '1__ ____|15_ ____|156 ____|156 5___|156 52__|156 525_');
  const vco0 = vco(b);
  scenario(b, '# alma frekansını saklar, gönderme adımı', '#', 'TX FREQ');
  check('1. adımda telsiz değişmedi', vco(b) === vco0, vco(b));
  b.run(FCLK * 8);
  check('FREKANS açıkken menü zaman aşımı yok (8 s)', lcd(b) === 'TX FREQ ' && b.ram[0x227] !== 0, lcd(b) + ' menü ' + b.ram[0x227]);
  scenario(b, 'gönderme frekansı haneleri', '1 6 0 6 5', '1-- ----|16- ----|160 ----|160 6---|160 65--');
  scenario(b, 'boşken * alma frekansına döner', '* * * * * *', '160 6---|160 ----|16- ----|1-- ----|TX FREQ|156 5250');
  scenario(b, 'alma frekansı düzeltilir', '* * 7 5 #', '156 525_|156 52__|156 527_|156 5275|TX FREQ');
  keys(b, 'm'); keys(b, enter);
  scenario(b, 'yeniden: alma 156,525, gönderme 160,650', '1 5 6 5 2 5 # 1 6 0 6 5 0 #', '1__ ____|15_ ____|156 ____|156 5___|156 52__|156 525_|TX FREQ|1-- ----|16- ----|160 ----|160 6---|160 65--|160 650-|156 5250');
  check('RX VCO = 156,525 + 45', vco(b) === '201.5250', vco(b));
  check('sentezör baytları firmware ile aynı (RX 156,525 / TX 160,650)', synthBytes(rec(b)) === reference(156.525e6, false, 160.65e6), synthBytes(rec(b)) + ' / ' + reference(156.525e6, false, 160.65e6));
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
  check('FREKANS menüsündeyken PTT: ekranda gönderme frekansı', vco(b) === '160.6500' && lcd(b) === '160 6500', vco(b) + ' ' + lcd(b));
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  check('menüde PTT bırakıldı: ekranda alma frekansı', vco(b) === '201.5250' && lcd(b) === '156 5250', vco(b) + ' ' + lcd(b));
  scenario(b, 'normal ekranda alma frekansı', 'm', '156 5250');
  check('kanal numarası yerine VR (U r)', chan(b) === 'Ur', '"' + chan(b) + '"');
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
  check('PTT: TX VCO 160,650, ekranda gönderme frekansı', vco(b) === '160.6500' && lcd(b) === '160 6500', vco(b) + ' ' + lcd(b));
  check('gönderirken kanal numarası yerine VT (U t)', chan(b) === 'Ut', '"' + chan(b) + '"');
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  check('PTT bırakıldı: RX 201,525, ekranda alma frekansı', vco(b) === '201.5250' && lcd(b) === '156 5250', vco(b) + ' ' + lcd(b));
  check('bırakınca yine VR', chan(b) === 'Ur', '"' + chan(b) + '"');
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.3); b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  check('ikinci PTT de aynı', lcd(b) === '156 5250' && vco(b) === '201.5250', vco(b) + ' ' + lcd(b));
  keys(b, enter + '156525#160650');
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
  check('gönderirken tuşlar yok sayılır (firmware)', keys(b, '#')[0] === '160 650-' && vco(b) === '160.6500', lcd(b) + ' ' + vco(b));
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  scenario(b, 'bırakınca # ayarlar', '#', '156 5250');
  { const g = keys(b, 's156525#177000#*');
    check('gönderme adımında bant dışı: hata, sonra girişe dönüş', g[g.length - 2] === 'ERROR   ' && g[g.length - 1] === '177 000-', g.join('|')); }
  keys(b, 'm');
  for (const [ks, f, name] of [['1565125', 156.5125e6, '12,5 kHz adım'], ['156010', 156.010e6, '10 kHz adım'], ['1560050', 156.005e6, '5 kHz adım'], ['176900', 176.9e6, 'bant üst ucu']]) {
    keys(b, enter + 's' + ks + '##');
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
  check('kanalda kanal numarası geri geldi', chan(b) === ' 3' || chan(b) === '03', '"' + chan(b) + '"');
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.6);
  check('kanaldayken PTT: ekranda kanal adı kalır (VFO modu bitti)', lcd(b) === 'KANAL 03' && vco(b) !== '201.0500', vco(b) + ' ' + lcd(b));
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  /* UHF configuration (base 400 MHz, -45 MHz injection, prescaler 128) */
  const u = radio(UHF);
  boot(u, 2.5);
  keys(u, enter + '433125##');
  check('UHF: 433,125 MHz, RX VCO 388,125', vco(u) === '388.1250' && synthBytes(rec(u)) === reference(433.125e6, true), vco(u) + ' ' + synthBytes(rec(u)) + ' / ' + reference(433.125e6, true));
  keys(u, 'm'); u.inputs.P4 = 0x7F; u.run(FCLK * 0.6);
  check('UHF PTT: TX 433,125', vco(u) === '433.1250', vco(u));
  u.inputs.P4 = 0xFF; u.run(FCLK * 0.4);
  keys(u, enter + '433125#438125#m');
  check('UHF çift frekans: RX VCO 388,125, baytlar firmware ile aynı', vco(u) === '388.1250' && synthBytes(rec(u)) === reference(433.125e6, true, 438.125e6), vco(u) + ' ' + synthBytes(rec(u)) + ' / ' + reference(433.125e6, true, 438.125e6));
  u.inputs.P4 = 0x7F; u.run(FCLK * 0.6);
  check('UHF PTT: TX 438,125, ekranda 438 1250', vco(u) === '438.1250' && lcd(u) === '438 1250', vco(u) + ' ' + lcd(u));
  const t = radio(ee => { ee[0x018] |= 0x80; });
  boot(t, 2.5);
  const g = keys(t, enter + '1#');
  check('Türkçe: 2. adımda GONDERME', keys(t, '*s156525#')[8] === 'GONDERME', lcd(t));
  check('Türkçe: FREKANS / HATA', g[RING.indexOf('FREQ') + 1] === 'FREKANS ' && g[g.length - 1] === 'HATA    ', g.join('|'));
}

/* APRS (ozel_firmware/ornekler/aprs.inc): PTT'de 144,800 MHz'de AFSK konum paketi */
if (RING.includes('APRS')) {
  /* ayarlar EEPROM'daki APRS bölümünde (0x6E0, p4000.js aprsSet); bölüm yoksa ROM varsayılanları */
  const P = globalThis.P4000;
  const aprs = o => ee => { if (o) P.aprsSet(ee, o); };
  const AYAR = { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'Düzce, 73 de TA7W!', tablo: '/', kod: '>' };
  const PAKET = 'TA7W-7>APIZCI,WIDE1-1,WIDE2-1:!4050.63N/03109.39E>' + String.fromCharCode(...P.utf8('Düzce, 73 de TA7W!'));
  const vco = b => (b.radioState().vco / 1e6).toFixed(4);
  const enter = 'mm' + 'm'.repeat(RING.indexOf('APRS')) + 's';
  const paketGonder = (b, sure) => {
    const log = [], push = b.pushAudio;
    b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push.call(b, buf, v); };
    b.inputs.P4 = 0x7F; b.run(FCLK * (sure || 1.4)); b.inputs.P4 = 0xFF; b.run(FCLK * 0.5);
    b.pushAudio = push;
    return AFSK.decode(log, FCLK);
  };
  const b = radio(aprs(AYAR));
  boot(b, 2.5);
  const vco0 = vco(b), t0 = b.now;
  scenario(b, 'APRS: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('APRS') + 1), 'APRS PTT'].join('|'));
  b.run(FCLK * 0.3);
  check('APRS: alma 144,800 (VCO 189,800)', vco(b) === '189.8000', vco(b));
  const hop = () => b.radioState().squelchInfo;
  b.inputs.P5 |= 0x80; b.run(FCLK * 0.6);                    // P57 = 1: kanalda sinyal
  check('APRS: kanalda sinyal varken hoparlör kapalı (P46 = 0)', !hop(), 'P46=' + (hop() ? 1 : 0));
  b.inputs.P5 &= ~0x80; b.run(FCLK * 0.4);
  const bip = b => b.events.filter(e => /^BİP \d/.test(e[1])).length;
  let n0 = bip(b); keys(b, '5*#0');
  check('APRS: tuşlara basınca bip yok (TA0 / P50 sessiz)', bip(b) === n0, (bip(b) - n0) + ' bip');
  /* PTT: örnekleri toplayıp çöz */
  const log = [], push = b.pushAudio.bind(b);
  b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push(buf, v); };
  /* FX806 modülatör seviyesi (C-BUS $13, F_00CB52 vericide $0201'den yazar): sıfırsa taşıyıcı boş gider */
  const rev = x => { let r = 0; for (let k = 0; k < 8; k++) r |= (x >> k & 1) << (7 - k); return r; };   // F_007E4C: baytlar ters bit sırasıyla (tablo 0x2B06)
  const mod13 = (b, t) => { const l = b.cbusLog.filter(e => e[1] === 'SEC1' && e[0] >= t); let v; for (let k = 0; k + 1 < l.length; k++) if (rev(l[k][2]) === 0x13) v = rev(l[++k][2]); return v; };
  const tPtt = b.now;
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.3);
  check('APRS: PTT: FX806 modülatör seviyesi açık (C-BUS $13 = $0201, sıfır değil)', mod13(b, tPtt) !== undefined && mod13(b, tPtt) === b.ram[0x201] && b.ram[0x201] !== 0, 'C-BUS $13 = ' + mod13(b, tPtt) + ', $0201 = ' + b.ram[0x201]);
  check('APRS: PTT: TX 144,800, ekranda APRS TX', vco(b) === '144.8000' && lcd(b) === 'APRS TX ', vco(b) + ' ' + lcd(b));
  b.run(FCLK * 1.2);
  const fr = AFSK.decode(log, FCLK);
  check('APRS: paket çözüldü, FCS doğru', fr.length === 1 && fr[0].fcsOk && fr[0].text === PAKET, fr.map(f => f.text + (f.fcsOk ? '' : ' (FCS yanlış)')).join(' / ') || 'çerçeve yok');
  const dur = log.length ? (log[log.length - 2] - log[0]) / FCLK : 0;
  check('APRS: süre ~0,7 s, örnekleme 9600 Hz', dur > 0.6 && dur < 0.8 && Math.abs((log.length / 2 - 1) / dur - 9600) < 30, dur.toFixed(3) + ' s, ' + ((log.length / 2 - 1) / dur).toFixed(0) + ' Hz');
  check('APRS: paketten sonra ekranda APRS OK, hâlâ gönderiyor', lcd(b) === 'APRS OK ' && vco(b) === '144.8000', lcd(b) + ' ' + vco(b));
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.5);
  check('APRS: PTT bırakıldı: alma 144,800', vco(b) === '189.8000', vco(b));
  scenario(b, 'APRS: m ile çık', 'm', 'KANAL 01');
  b.run(FCLK * 0.3);
  check('APRS: çıkınca kanala dönüş', vco(b) === vco0, vco(b) + ' / ' + vco0);
  b.inputs.P5 |= 0x80; b.run(FCLK * 0.6);
  check('çıkınca kanalda sinyal: hoparlör açılır (P46 = 1)', hop(), 'P46=' + (hop() ? 1 : 0));
  n0 = bip(b); keys(b, 'mm');
  check('çıkınca bipler geri geldi', bip(b) > n0, (bip(b) - n0) + ' bip');
  keys(b, 'm'.repeat(RING.length + 1)); for (let k = 0; k < 14 && lcd(b) !== 'KANAL 01'; k++) keys(b, 'm');
  b.inputs.P5 &= ~0x80; b.run(FCLK * 0.6);
  check('sinyal gidince hoparlör kapanır', !hop(), 'P46=' + (hop() ? 1 : 0));
  log.length = 0;
  b.inputs.P4 = 0x7F; b.run(FCLK * 1.0);
  check('APRS menüsü dışında PTT: paket yok (seçmeli çağrı kimliği de yok)', AFSK.decode(log, FCLK).length === 0 && log.length === 0, log.length / 2 + ' örnek');
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.5);
  /* menü zaman aşımı: APRS'te kalır */
  const halka = []; for (let k = 0; k < 14 && lcd(b) !== 'APRS    '; k++) halka.push(keys(b, 'm')[0]);   // halka son konumdan devam eder
  keys(b, 's'); b.run(FCLK * 8);
  check('APRS açıkken menü zaman aşımı yok (8 s)', lcd(b) === 'APRS PTT' && vco(b) === '189.8000', lcd(b) + ' ' + vco(b) + ' halka ' + halka.join('|'));
  b.inputs.P4 = 0x7F; b.run(FCLK * 1.4); b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  check('ikinci paket de gitti', lcd(b) === 'APRS OK ', lcd(b));
  /* kanal anahtarı APRS'teyken çevrilirse çıkışta yeni kanal korunur */
  b.panel.setExt(((16 - 3) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.6);
  keys(b, 'm'); b.run(FCLK * 0.3);
  check('APRS\'te kanal değişti: çıkınca yeni kanal kalır', vco(b) === '201.0500', vco(b));
  /* başka ayarlar: paket gerçekten EEPROM'daki bölümden kuruluyor */
  const VARS = 'TC19MK-10>APIZCI,WIDE1-1,WIDE2-1:!3955.52N/03250.22EbASELSAN 4000';
  const ham = (o, f) => ee => { P.aprsSet(ee, o); f(ee); P.aprsFix(ee); };      // aprsSet'in reddedeceği içerik
  const yaz = (a, t) => ee => t.split('').forEach((c, i) => { ee[0x6E0 + a + i] = c.charCodeAt(0); });
  const uzun = 'Merhaba dunya: bu yorum tam kirk uc bayt...';
  for (const [ad, mod, bek] of [
    ['bölüm yok: ROM varsayılanları (Anıtkabir, bisiklet, TC19MK-10)', null, VARS],
    ['varsayılanlar EEPROM\'a yazılı', aprs(P.APRS_VARSAYILAN), VARS],
    ['bölümün sağlaması bozuk: ROM varsayılanları', ee => { P.aprsSet(ee, AYAR); ee[0x6E0 + 40] ^= 1; }, VARS],
    ['SSID 12, güney / batı, simge tablosu \\, yorum yok', aprs({ cagri: 'N0CALL-12', enlem: '-33.86833', boylam: '151 12.34 W', tablo: '\\', kod: 'k', yorum: '' }),
      'N0CALL-12>APIZCI,WIDE1-1,WIDE2-1:!3352.10S\\15112.34Wk'],
    ['SSID yok, 43 baytlık yorum', aprs({ cagri: 'TA1AB', yorum: uzun }), 'TA1AB>APIZCI,WIDE1-1,WIDE2-1:!3955.52N/03250.22Eb' + uzun],
    ['küçük harfli konum harfi büyüğe çevrilir', ham({ cagri: 'TA1AB' }, yaz(23, 'n')), 'TA1AB>APIZCI,WIDE1-1,WIDE2-1:!3955.52N/03250.22EbASELSAN 4000']]) {
    const r = radio(mod); boot(r, 2.5); keys(r, enter); r.run(FCLK * 0.3);
    const f = paketGonder(r), sim = r.eeprom.mem.slice();
    check('APRS ayar: ' + ad, uzun.length === 43 && f.length === 1 && f[0].fcsOk && f[0].text === bek && P.aprsPaket(sim) === bek,
      (f.map(x => x.text + (x.fcsOk ? '' : ' (FCS yanlış)')).join(' / ') || lcd(r)) + ' | sayfa: ' + P.aprsPaket(sim));
  }
  for (const [ad, mod, ekran] of [['çağrı işareti 7 karakter', ham(AYAR, yaz(4, 'TC19ATA-1')), 'CALL -'],
                                   ['çağrı işareti boş', ham(AYAR, ee => { ee[0x6E4] = 0; }), 'CALL -'],
                                   ['geçersiz SSID 16', ham(AYAR, yaz(4, 'TA7W-16')), 'CALL -'],
                                   ['enlemde harf yanlış', ham(AYAR, yaz(23, 'E')), 'LAT -'],
                                   ['enlemde nokta yok', ham(AYAR, yaz(20, '0')), 'LAT -'],
                                   ['boylamda rakam yerine harf', ham(AYAR, yaz(25, 'A')), 'LON -']]) {
    const r = radio(mod); boot(r, 2.5); const g = keys(r, enter); r.run(FCLK * 0.3);
    const f = paketGonder(r);
    check('APRS hatalı ayar: ' + ad + ' -> ' + ekran + ', paket yok', g[g.length - 1] === ekran.padEnd(8) && f.length === 0, g[g.length - 1] + ' / ' + f.length + ' paket');
  }
  /* otomatik gönderim (PTT'ye basılmadan): bölümdeki aralık */
  {
    const r = radio(aprs(Object.assign({}, AYAR, { aralik: 12 }))); boot(r, 2.5); keys(r, enter); r.run(FCLK * 0.3);
    const log = [], push = r.pushAudio, tx = [];
    r.pushAudio = (buf, v) => { if (buf === r.audio.ta1) log.push(r.now, v); push.call(r, buf, v); };
    for (let t = 0; t < 27; t += 0.25) { r.run(FCLK * 0.25); tx.push(r.radioState().vco / 1e6 < 150 ? 1 : 0); }
    r.pushAudio = push;
    const f = AFSK.decode(log, FCLK), txs = tx.join('').split(/0+/).filter(Boolean).length;
    check('APRS otomatik: aralık 12 s, PTT yok, 27 s\'de 2 paket', f.length === 2 && f.every(x => x.fcsOk && x.text === PAKET), f.map(x => x.text).join(' / ') || 'paket yok');
    const t1 = log.length ? log[0] / FCLK : 0;
    check('APRS otomatik: ilk paket ~12 s sonra, her gönderimden sonra verici kapanıyor', txs === 2 && tx[tx.length - 1] === 0, 'gönderim ' + txs + ', son durum ' + (tx[tx.length - 1] ? 'TX' : 'RX') + ', ekran ' + lcd(r));
    keys(r, 'm'); log.length = 0;
    r.pushAudio = (buf, v) => { if (buf === r.audio.ta1) log.push(r.now, v); push.call(r, buf, v); };
    r.run(FCLK * 14); r.pushAudio = push;
    check('APRS otomatik: menüden çıkınca durur', log.length === 0 && lcd(r) === 'KANAL 01', log.length / 2 + ' örnek, ' + lcd(r));
  }
  {
    const r = radio(aprs(AYAR)); boot(r, 2.5); keys(r, enter); r.run(FCLK * 0.3);
    let n = 0; const push = r.pushAudio;
    r.pushAudio = (buf, v) => { if (buf === r.audio.ta1) n++; push.call(r, buf, v); };
    r.run(FCLK * 14); r.pushAudio = push;
    check('APRS otomatik: aralık 0 ise gönderim yok', n === 0 && lcd(r) === 'APRS PTT', n + ' örnek');
  }
  /* kapat-aç: APRS'teyken kapanırsa açılışta APRS'e döner (EEPROM 0x732 = $A5; çıkınca 0) */
  {
    const yenidenAc = (ee, arac) => { const r = new Board(rom); r.eeprom.mem.set(ee); if (arac) { r.hwArac = true; r.inputs.P4 = 0xCF; r.panel.setExt(0xBF, r); } r.font = b.font; boot(r, 2.5); r.run(FCLK * 0.5); return r; };
    const r = radio(aprs(Object.assign({}, AYAR, { aralik: 12 }))); boot(r, 2.5);
    const ee0 = Uint8Array.from(r.eeprom.mem);
    check('APRS modu: ilk açılışta normal ekran (işaret yok)', lcd(r) === 'KANAL 01' && ee0[0x732] !== 0xA5, lcd(r) + ', 0x732 = ' + ee0[0x732].toString(16));
    keys(r, enter); r.run(FCLK * 0.3);
    const ee1 = Uint8Array.from(r.eeprom.mem), fark = [];
    for (let a = 0; a < ee1.length; a++) if (ee1[a] !== ee0[a]) fark.push(a.toString(16));
    check('APRS modu: menüye girince EEPROM 0x732 = A5, başka bayt değişmez', ee1[0x732] === 0xA5 && fark.join() === '732', 'değişen ' + fark.join(','));
    const r2 = yenidenAc(ee1);
    check('APRS modu: kapat-aç -> menü seçmeden APRS PTT, alma 144,800', lcd(r2) === 'APRS PTT' && vco(r2) === '189.8000' && r2.ram[0x227] === 0x17, lcd(r2) + ' ' + vco(r2));
    const log = [], push = r2.pushAudio;
    r2.pushAudio = (buf, v) => { if (buf === r2.audio.ta1) log.push(r2.now, v); push.call(r2, buf, v); };
    r2.run(FCLK * 14); r2.pushAudio = push;
    const f = AFSK.decode(log, FCLK);
    check('APRS modu: kapat-aç sonrası otomatik gönderim sürüyor (12 s)', f.length === 1 && f[0].fcsOk && f[0].text === PAKET && lcd(r2) === 'APRS OK ', f.length + ' paket, ' + lcd(r2));
    const r3 = yenidenAc(Uint8Array.from(r2.eeprom.mem));
    check('APRS modu: ikinci kapat-aç da APRS\'te', lcd(r3) === 'APRS PTT' && r3.eeprom.mem[0x732] === 0xA5, lcd(r3));
    keys(r3, 'm'); r3.run(FCLK * 0.3);
    check('APRS modu: m ile çıkınca 0x732 = 0, kanala dönüş', lcd(r3) === 'KANAL 01' && r3.eeprom.mem[0x732] === 0 && vco(r3) === vco0, lcd(r3) + ', 0x732 = ' + r3.eeprom.mem[0x732] + ', ' + vco(r3));
    const r4 = yenidenAc(Uint8Array.from(r3.eeprom.mem));
    check('APRS modu: çıktıktan sonra kapat-aç -> normal ekran', lcd(r4) === 'KANAL 01' && r4.ram[0x227] === 0, lcd(r4));
    keys(r4, 'm');
    check('APRS modu: menü halkası normal', lcd(r4) === 'LIST    ', lcd(r4));
    /* araç / sabit kart (EE 0x58B bit 1: I²C P60 üzerinden; kullanıcının telsizi) */
    const ea = Uint8Array.from(ee0); ea[0x58B] |= 2; B.eeFixSums(ea);
    const a1 = yenidenAc(ea, true); for (let k = 0; k < 16 && lcd(a1) !== 'APRS    '; k++) keys(a1, 'm'); keys(a1, 's'); a1.run(FCLK * 0.3);   // araçta menü halkası farklı (CHN SCAN ...)
    const a2 = yenidenAc(Uint8Array.from(a1.eeprom.mem), true);
    check('APRS modu (araç / sabit kart): kapat-aç -> APRS PTT', a1.eeprom.mem[0x732] === 0xA5 && lcd(a2) === 'APRS PTT', '0x732 = ' + a1.eeprom.mem[0x732].toString(16) + ', ' + lcd(a2));
  }
  /* alma (aprs_rx.inc): P66'ya (TB1IN, sıfır-geçiş algılayıcısı) AFSK paket; ekranda gönderen */
  {
    const r = radio(aprs(AYAR)); boot(r, 2.5); keys(r, enter); r.run(FCLK * 0.3);
    const al = (text, o, bozuk, kesme) => {
      if (r.ram[0x227] === 0x17 && kesme !== false) keys(r, '0');                     // kayan mesaj sürüyorsa kes (her tuş keser)
      const fr = AFSK.encode(text); if (bozuk) fr[fr.length - 3] ^= 1;
      const e = AFSK.edges(fr, o), t = r.now;
      r.selIn.setEdges(e.map(x => t + Math.round(x * FCLK)));
      r.inputs.P5 |= 0x80; r.run(FCLK * (e[e.length - 1] + 0.05)); r.inputs.P5 &= ~0x80; r.run(FCLK * 0.3);
      return lcd(r);
    };
    /* hoparlör: APRS'te taşıyıcı varken FX806 $10 bit 7 (ses çıkışı) kapalı, diğer ayarlar almadaki gibi
     * (seçmeli çağrı / TKS beklerken firmware'in susturması; P66'ya giden ses yolu açık) */
    const fxReg = (b, reg) => { const l = b.cbusLog.filter(e => e[1] === 'SEC1'); let v; for (let k = 0; k + 1 < l.length; k += 2) if (rev(l[k][2]) === reg) v = rev(l[k + 1][2]); return v; };
    r.inputs.P5 |= 0x80; r.run(FCLK * 0.5);
    const a10 = fxReg(r, 0x10), a11 = fxReg(r, 0x11), a13 = fxReg(r, 0x13);
    r.inputs.P5 &= ~0x80; r.run(FCLK * 0.5);
    const k = radio(aprs(AYAR)); boot(k, 2.5); k.inputs.P5 |= 0x80; k.run(FCLK * 0.5);
    check('APRS: taşıyıcı varken hoparlör çıkışı kapalı (FX806 $10 bit 7 = 0), kanalda açık; ses yolu aynı',
      a10 !== undefined && !(a10 & 0x80) && (fxReg(k, 0x10) & 0x80) && a11 === fxReg(k, 0x11) && a13 === fxReg(k, 0x13) && (a10 | 0x80) === fxReg(k, 0x10),
      'APRS $10=' + a10 + ' $11=' + a11 + ' $13=' + a13 + ' / kanal $10=' + fxReg(k, 0x10) + ' $11=' + fxReg(k, 0x11) + ' $13=' + fxReg(k, 0x13));
    /* kayan mesaj (aprs_mesaj.inc): ekran karelerini topla, birleştir */
    const tampon = () => V.textBuffer(r).padEnd(8).slice(0, 8);   // LCD tamponu (0x252): LCD I²C ile karakter karakter yazılırken yarım kare okunmasın
    const kay = (sn) => { const k = [tampon()]; let once = k[0];   // kare 50 ms değişmeden kaldıysa sayılır
      for (let i = 0; i < sn / 0.05; i++) { r.run(FCLK * 0.05); const l = tampon(); if (l === once && l !== k[k.length - 1]) k.push(l); once = l; if (/^APRS (PTT|OK)/.test(k[k.length - 1])) break; } return k; };
    const birlestir = k => { let t = k[0]; for (let i = 1; i < k.length && !/^APRS (PTT|OK)/.test(k[i]); i++) t += k[i].slice(7); return t.trimEnd(); };
    const M1 = 'TA7W-7>APRS,WIDE1-1*,WIDE2-1:!4050.63N/03109.39E>Merhaba, 73! (#1 @ $5; a=b?)';
    const ilk0 = al(M1), ilk = tampon(), t1 = r.now, k1 = kay(40);
    const sure = (r.now - t1) / FCLK, beklenen = M1.toUpperCase();
    check('APRS alma: paketin tamamı kayarak (TNC2, büyük harf, > . , ! ( # @ $ ; = ? çizilir)', birlestir([ilk].concat(k1.slice(1))) === beklenen, birlestir([ilk].concat(k1.slice(1))));
    check('APRS alma: 1 s bekler, 320 ms / karakter, sonda 1 s, sonra APRS ekranı', k1[k1.length - 1] === 'APRS PTT' && Math.abs(sure - (1 + (beklenen.length - 8) * 0.32 + 1)) < 1.2, sure.toFixed(1) + ' s, ' + beklenen.length + ' karakter');
    const uzun = 'TA5UZ-1>APRS:>' + '0123456789'.repeat(10);
    al(uzun); const ku = birlestir([tampon()].concat(kay(60).slice(1)));
    check('APRS alma: 96 bayttan uzun paketin başı gösterilir', ku === uzun.slice(0, 'TA5UZ-1>APRS:'.length + 96 - 16), ku.length + ' karakter: ' + ku.slice(-12));
    al(M1); r.run(FCLK * 2); const tk = keys(r, '5');
    check('APRS alma: tuş kaymayı keser', tk[0] === 'APRS PTT', tk[0]);
    al('TA6AA>APRS:>birinci mesaj uzun uzun uzun'); r.run(FCLK * 1.5);
    const ikinciEkran = al('TA6BB>APRS:>ikinci', {}, false, false);
    check('APRS alma: kayarken gelen paket gösterilmez (ilki sürer), listeye girer', ikinciEkran !== 'APRS PTT' && !/TA6BB/.test(ikinciEkran) && keys(r, '*')[0] === 'TA6BB   ', ikinciEkran);
    const zor = [['iki haneli SSID', 'YM7KA-12>APDR15:=4100.00N/02900.00E-x', {}, 'YM7KA-12'],
      ['6 harf + SSID 15 (tire yok)', 'TB1ABC-15>APRS:>status', {}, 'TB1ABC15'],
      ['SSID yok, yol 3 adres', 'TA1XX>APRS,WIDE1-1,WIDE2-2,TA2DEF-3*:>uzun bir durum metni 0123456789 abcdefghij', {}, 'TA1XX'],
      ['2200 Hz yarı genlik + kayma', 'TA2AA-1>APRS:!4050.63N/03109.39E>', { as: 0.5, off: 0.2 }, 'TA2AA-1'],
      ['2200 Hz iki kat genlik', 'TA2AB-2>APRS:!4050.63N/03109.39E>', { as: 2 }, 'TA2AB-2'],
      ['baud +%0,3', 'TA2AC-3>APRS:!4050.63N/03109.39E>', { baud: 1203.6 }, 'TA2AC-3'],
      ['baud -%0,3', 'TA2AD-4>APRS:!4050.63N/03109.39E>', { baud: 1196.4 }, 'TA2AD-4'],
      ['gürültü %10', 'TA2AE-5>APRS:!4050.63N/03109.39E>', { noise: 0.1, seed: 7 }, 'TA2AE-5']];
    for (const [ad, text, o, bek] of zor) { const g = al(text, o); check('APRS alma: ' + ad, g === text.toUpperCase().slice(0, 8) && r.ram[0x500] << 0 !== undefined, g + ' (' + bek + ')'); }
    const bz = al('TA9ZZ>APRS:>bozuk', {}, true);
    check('APRS alma: sağlaması bozuk paket gösterilmez', bz === 'APRS PTT' && r.ram[0x623] === 8, bz + ', liste ' + r.ram[0x623]);
    keys(r, '0');
    /* liste: en yeni başta, en çok 8; '*' eskiye, '#' yeniye */
    const liste = keys(r, '*********');
    check('APRS alma: liste * ile (8 istasyon, en eskide kalır)', liste.join('|') === ['TA2AE-5', 'TA2AD-4', 'TA2AC-3', 'TA2AB-2', 'TA2AA-1', 'TA1XX', 'TB1ABC15', 'YM7KA-12', 'YM7KA-12'].map(x => x.padEnd(8)).join('|'), liste.join('|'));
    const geri = keys(r, '#'.repeat(8));
    check('APRS alma: # ile yeniye, en yeniden sonra APRS ekranı', geri[6] === 'TA2AE-5 ' && geri[7] === 'APRS PTT', geri.join('|'));
    al('TA2AB-2>APRS:>tekrar');
    const l2 = keys(r, '**');
    check('APRS alma: tekrar duyulan istasyon başa geçer, çift kayıt yok', l2.join('|') === 'TA2AB-2 |TA2AE-5 ' && r.ram[0x623] === 8, l2.join('|') + ', ' + r.ram[0x623]);
    /* gönderme sonrası alma sürer */
    const f = paketGonder(r);
    check('APRS alma: PTT ile paket hâlâ gidiyor', f.length === 1 && f[0].fcsOk && f[0].text === PAKET, f.length + ' paket');
    const sonra = al('TA3BB-9>APRS:>after tx');
    check('APRS alma: gönderdikten sonra yine alıyor', sonra === 'TA3BB-9>', sonra);
    r.run(FCLK * 1.5); r.inputs.P4 = 0x7F; r.run(FCLK * 0.3); const pttEkran = lcd(r); r.run(FCLK * 1.2); r.inputs.P4 = 0xFF; r.run(FCLK * 0.5);
    check('APRS alma: PTT kaymayı keser (APRS TX), sonra kayma sürmez', pttEkran === 'APRS TX ' && lcd(r) === 'APRS OK ', pttEkran + ' / ' + lcd(r));
    /* sürekli gürültü (susturma açık): bekçi köpeği yok, tuşlar çalışıyor */
    r.selIn.noise = true; r.inputs.P5 |= 0x80; const ev0 = r.events.length; r.run(FCLK * 3);
    const tus = keys(r, '#');
    r.selIn.noise = false; r.inputs.P5 &= ~0x80; r.run(FCLK * 0.5);
    check('APRS alma: 3 s gürültüde yeniden başlama yok, tuş çalışıyor', !r.events.slice(ev0).some(e => /WATCHDOG|RESET/.test(e[1])) && tus[0] === 'APRS OK ', tus.join('|'));
    /* P66 çok hızlı titrerse (girişsiz karşılaştırıcı) kesme seli ana döngüyü durdurmasın: AR_KENAR_MAX */
    r.selIn.noiseUs = [5, 10]; r.selIn.noise = true; r.inputs.P5 |= 0x80; const ev1 = r.events.length;
    const titrek = al('TA8TT>APRS:>titresim oncesi', {}, false), k8 = kay(30); r.selIn.noise = true; r.inputs.P5 |= 0x80; r.run(FCLK * 2);
    const tus2 = keys(r, '#');
    r.selIn.noise = false; r.selIn.noiseUs = null; r.inputs.P5 &= ~0x80; r.run(FCLK * 0.5);
    const sonraki = al('TA8UU>APRS:>sonra');
    check('APRS alma: P66 5-10 µs titrerken yeniden başlama yok, tuş çalışıyor, sonra yine alıyor', !r.events.slice(ev1).some(e => /WATCHDOG|RESET/.test(e[1])) && tus2[0] === 'APRS OK ' && sonraki === 'TA8UU>AP', tus2[0] + ' / ' + sonraki);
    check('APRS alma: taşıyıcı yokken çözücü (TB1) kapalı', !(r.sfr[0x40] & 0x40) && !(r.ram[0x5FC] & 1), 'TCSTART ' + r.sfr[0x40].toString(16));
    keys(r, 'm');
    const tb1 = r.sfr[0x40] & 0x40;
    const disari = al('TA4CC>APRS:>menü dışı');
    check('APRS alma: menüden çıkınca TB1 durur, paket gösterilmez', disari === 'KANAL 01' && !tb1, disari + ', TB1 ' + (tb1 ? 'çalışıyor' : 'durdu'));
  }
  const kotu = b.events.filter(e => e[0] > t0 && /WATCHDOG|RESET/.test(e[1]));   // (açılıştakiler hariç)
  check('APRS: bekçi köpeği / yeniden başlama yok', kotu.length === 0, kotu.map(e => (e[0] / FCLK).toFixed(2) + ' ' + e[1]).join(', '));
}

/* seçmeli çağrı kaldırıldı (ozel_firmware/ornekler/secmeli_kaldir.inc): orijinalde selftest.js'nin
 * çalıştırdığı üç senaryo artık ton üretmez, çözmez, verici açmaz; telsiz çalışmaya devam eder */
if (RING.includes('APRS')) {
  const selTon = (b, sec) => { const seen = []; for (let i = 0; i < sec / 0.02; i++) { b.run(FCLK * 0.02); const r = V.analyseTx(b, FCLK, FCLK * 0.02); const d = r.selcall && r.selcall.find(x => x.startsWith('CCIR:')); if (d && seen[seen.length - 1] !== d.slice(5)) seen.push(d.slice(5)); } return seen.join(''); };
  let b = radio(ee => { ee[0x01A] = 0x20; B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'SEL', selcall: 1 }); });
  boot(b, 2.6); b.inputs.P4 = 0x7F;
  const s1 = selTon(b, 0.8), vtx = (b.radioState().vco / 1e6).toFixed(4);
  check('seçmeli çağrı yok: PTT\'de kimlik tonu gitmiyor, verici açık', s1 === '' && vtx === '156.0000', '"' + s1 + '" ' + vtx);
  b.inputs.P4 = 0xFF; b.run(FCLK * 0.6);
  check('seçmeli çağrı yok: PTT bırakınca da ton yok', selTon(b, 0.6) === '', '');
  b = radio(ee => { ee[0x01A] = 0x20; B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'SEL', selcall: 1 }); });
  boot(b, 2.6); const tb = b.now; b.inputs.P5 |= 0x80; b.run(FCLK * 0.3);
  const ccir = V.SELCALL.CCIR, seq = [{ f: 0, ms: 20 }];
  for (const ch of '12345') seq.push({ f: ccir.f[+ch], ms: ccir.ms });
  b.selIn.setSequence(b.now, seq); b.run(FCLK * 1.0);
  let d = ''; for (let k = 0; k < 8; k++) { const v = b.ram[0x264 + k]; if (v < 15) d += V.SEL_DIGITS[v]; }
  const kotu = b.events.filter(e => e[0] > tb && /WATCHDOG|RESET/.test(e[1]));
  check('seçmeli çağrı yok: CCIR 12345 alınınca çözülmüyor, telsiz çalışıyor', d.indexOf('12345') < 0 && kotu.length === 0 && lcd(b) === 'SEL     ', '"' + d + '" ' + lcd(b) + ' ' + kotu.length);
  b = radio(ee => { ee[0x014] = 0x0C; ee[0x01B] = 5; ee[0x01C] = 0x30; for (let n = 1; n <= 16; n++) ee[0x2E + (n - 1) * 13 + 4] |= 0x80; });
  boot(b, 2.5);
  let tx = 0, sel = '';
  for (const k of '#123') { const i = KP.indexOf(k); b.keypad.press(i, true, b); b.run(FCLK * 0.12); b.keypad.press(i, false, b);
    for (let t = 0; t < 0.7; t += 0.05) { b.run(FCLK * 0.05); if (b.ram[0x228] >= 4 && b.ram[0x228] <= 12) tx++; } }
  for (let t = 0; t < 1.5; t += 0.05) { sel += selTon(b, 0.05); if (b.ram[0x228] >= 4 && b.ram[0x228] <= 12) tx++; }
  check('seçmeli çağrı yok: # ile çağrı vericiyi açmıyor, ton yok', tx === 0 && sel === '', tx + ' TX örneği, "' + sel + '"');
}

/* boot screen built into the ROM (acilis_yatolye.inc): 5 s, independent of the EEPROM */
if (new Board(rom).rom[0xE0E1] === 0x5C) {
  for (const [name, mod] of [['varsayılan EEPROM', null], ['Türkçe + UHF EEPROM', ee => { ee[0x018] |= 0x80; ee[0x58C] = 0x3E; ee[0x58D] = 0x80; }]]) {
    const b = radio(mod), seen = [];
    for (const t of [0.6, 3.0, 5.4, 7.5]) { b.run(FCLK * (t - b.now / FCLK)); seen.push(lcd(b)); }
    check('açılış ekranı 5 s yATOLYE (' + name + ')', seen[0] === 'yATOLYE ' && seen[1] === 'yATOLYE ' && seen[2] === 'yATOLYE ' && seen[3] !== 'yATOLYE ', seen.join('|'));
  }
  /* then the version from the ROM header (tools/yama.py --surum), shown by F_00E0E1's own code */
  const b = radio(), seen = [];
  for (let t = 0.5; t < 8; t += 0.1) { b.run(FCLK * 0.1); const s = lcd(b); if (s !== seen[seen.length - 1]) seen.push(s); }
  const h = b.rom, minor = h[0x10002] % 10 ? h[0x10002] : h[0x10002] / 10;
  const want = String.fromCharCode(h[0x10009], h[0x1000A]) + ' V' + h[0x10001] + '-' + minor;
  /* ozel_firmware/surum.json'daki sürümle aynı mı (10, 20, ... 1, 2, ... gibi görünür; yama.py atlar) */
  const ad = (isNode ? process.argv[2] : (typeof scriptArgs !== 'undefined' ? scriptArgs : arguments)[0]).split('/').pop();
  const kayit = JSON.parse(isNode ? require('fs').readFileSync('../../ozel_firmware/surum.json', 'utf8') : readFile('../../ozel_firmware/surum.json')).kayitlar[ad];
  check('sürüm ekranı surum.json ile aynı', kayit && want === 'AT V' + kayit.major + '-' + kayit.minor, want + ' / ' + (kayit ? kayit.major + '-' + kayit.minor : ad + ' kaydı yok'));
  check('yATOLYE\'den sonra sürüm "' + want + '"', seen[seen.indexOf('yATOLYE ') + 1] === want.padEnd(8), seen.join('|'));
}

/* the real device is programmed through the serial loader: flash the new image into a
 * radio that runs the original firmware, reset, and try the new mode */
if (origText) {
  const b = new Board(parseHex(origText)), ee = B.defaultEeprom(); B.eeFixSums(ee); b.eeprom.mem.set(ee);
  b.font = V.buildFont(b.rom); boot(b, 2.6);
  const up = new UPD.FlashUpdater(b, rom, { verify: true });
  while (!up.done && b.now < FCLK * 600) { b.run(FCLK * 0.01); up.tick(); }
  check('seri yükleyiciyle yazıldı ve doğrulandı', !up.error, up.error || up.log.slice(-1)[0]);
  b.font = V.buildFont(b.rom); boot(b, 3);
  if (RING.includes('NUMBER')) { scenario(b, 'yüklemeden sonra SAYI çalışıyor', 'mms 2 0 2 6 s', 'LIST|NUMBER|_|2_|20_|202_|2026_|6202'); keys(b, 'm'); }
  if (RING.includes('FREQ')) { const g = keys(b, 'mm' + 'm'.repeat(RING.indexOf('FREQ')) + 's156525##'); check('yüklemeden sonra FREKANS çalışıyor', g[g.length - 1] === '156 5250' && (b.radioState().vco / 1e6).toFixed(4) === '201.5250', g.join('|')); }
  if (RING.includes('GAME')) { if (RING.includes('FREQ')) keys(b, 'm'); const g = keys(b, 'mmmms123#'); check('yüklemeden sonra OYUN çalışıyor', g[4] === '___     ' && /^123 [v^]  1$|^WIN    1$/.test(g[8]), g.join('|')); }
  if (RING.includes('CALC')) { if (RING.includes('GAME') || RING.includes('FREQ')) keys(b, 'm'); scenario(b, 'yüklemeden sonra HESAP çalışıyor', 'mmms 7 * * * 6 #', 'LIST|NUMBER|CALC|       0|       7|+      7|-      7|*      7|*      6|      42'); }
}

out(fails ? fails + ' test başarısız' : 'tüm özel firmware testleri geçti');
if (isNode && fails) process.exit(1);
if (!isNode && fails) throw new Error(fails + ' test başarısız');   // jsc: yakalanmamış istisna -> çıkış kodu 3 (quit(n) kodu iletmiyor)
