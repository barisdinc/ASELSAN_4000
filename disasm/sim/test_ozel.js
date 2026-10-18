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
  B = require('./board.js'); V = require('./radio_view.js'); globalThis.UPD = require('./updater.js'); globalThis.AFSK = require('./afsk.js');
  hexText = require('fs').readFileSync(process.argv[2], 'utf8'); out = s => console.log(s);
  origText = process.argv[3] && require('fs').readFileSync(process.argv[3], 'utf8');
  ringArg = process.argv[4];
} else {
  load('m7700.js'); load('board.js'); load('radio_view.js'); load('updater.js'); load('afsk.js');
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
{
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
{
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
  /* ayarlar kanal 50..54'ün adlarında (programlayıcı nokta ve küçük harf kabul etmez) */
  const adlar = a => ee => a.forEach((ad, i) => B.eeSetChannel(ee, 50 + i, 144.8e6, 144.8e6, { name: ad }));
  const AYAR = ['TA7W-7', '4050 63N', '0310939E', 'ASELSAN', '/B'];
  const PAKET = 'TA7W-7>APIZCI,WIDE1-1,WIDE2-1:!4050.63N/03109.39EbASELSAN';
  const vco = b => (b.radioState().vco / 1e6).toFixed(4);
  const enter = 'mm' + 'm'.repeat(RING.indexOf('APRS')) + 's';
  const paketGonder = (b, sure) => {
    const log = [], push = b.pushAudio;
    b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push.call(b, buf, v); };
    b.inputs.P4 = 0x7F; b.run(FCLK * (sure || 1.4)); b.inputs.P4 = 0xFF; b.run(FCLK * 0.5);
    b.pushAudio = push;
    return AFSK.decode(log, FCLK);
  };
  const b = radio(adlar(AYAR));
  boot(b, 2.5);
  const vco0 = vco(b), t0 = b.now;
  scenario(b, 'APRS: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('APRS') + 1), 'APRS PTT'].join('|'));
  b.run(FCLK * 0.3);
  check('APRS: alma 144,800 (VCO 189,800)', vco(b) === '189.8000', vco(b));
  const hop = () => b.radioState().squelchInfo;
  b.inputs.P5 |= 0x80; b.run(FCLK * 0.6);                    // P57 = 1: kanalda sinyal
  check('APRS: kanalda sinyal varken hoparlör kapalı (P46 = 0)', !hop(), 'P46=' + (hop() ? 1 : 0));
  b.inputs.P5 &= ~0x80; b.run(FCLK * 0.4);
  /* PTT: örnekleri toplayıp çöz */
  const log = [], push = b.pushAudio.bind(b);
  b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push(buf, v); };
  b.inputs.P4 = 0x7F; b.run(FCLK * 0.3);
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
  /* başka ayarlar: paket gerçekten kanal adlarından kuruluyor */
  for (const [ad, ayar, bek] of [
    ['başka çağrı, SSID 12, 6 haneli boylam, yorum yok, tek karakter simge',
      ['TA1AB-12', '3958-20N', '325120E', '', 'K'], 'TA1AB-12>APIZCI,WIDE1-1,WIDE2-1:!3958.20N/03251.20Ek'],
    ['SSID yok, güney / batı, simge tablosu ve kod', ['N0CALL', '3352 10S', '1512345W', 'TEST 123', '/-'],
      'N0CALL>APIZCI,WIDE1-1,WIDE2-1:!3352.10S/15123.45W-TEST 123']]) {
    const r = radio(adlar(ayar)); boot(r, 2.5); keys(r, enter); r.run(FCLK * 0.3);
    const f = paketGonder(r);
    check('APRS ayar: ' + ad, f.length === 1 && f[0].fcsOk && f[0].text === bek, f.map(x => x.text + (x.fcsOk ? '' : ' (FCS yanlış)')).join(' / ') || lcd(r));
  }
  for (const [ad, ayar, ekran] of [['çağrı işareti boş', ['', '4050 63N', '0310939E', '', ''], 'CALL -'],
                                   ['çağrı işaretinde geçersiz SSID', ['TA7W-16', '4050 63N', '0310939E', '', ''], 'CALL -'],
                                   ['enlemde harf yok', ['TA7W-7', '405063', '0310939E', '', ''], 'LAT -'],
                                   ['boylamda eksik rakam', ['TA7W-7', '4050 63N', '31093E', '', ''], 'LON -']]) {
    const r = radio(adlar(ayar)); boot(r, 2.5); const g = keys(r, enter); r.run(FCLK * 0.3);
    const f = paketGonder(r);
    check('APRS hatalı ayar: ' + ad + ' -> ' + ekran + ', paket yok', g[g.length - 1] === ekran.padEnd(8) && f.length === 0, g[g.length - 1] + ' / ' + f.length + ' paket');
  }
  const kotu = b.events.filter(e => e[0] > t0 && /WATCHDOG|RESET/.test(e[1]));   // (açılıştakiler hariç)
  check('APRS: bekçi köpeği / yeniden başlama yok', kotu.length === 0, kotu.map(e => (e[0] / FCLK).toFixed(2) + ' ' + e[1]).join(', '));
}

/* boot screen built into the ROM (acilis_yatolye.inc): 5 s, independent of the EEPROM */
if (new Board(rom).rom[0xE0E1] === 0x5C) {
  for (const [name, mod] of [['varsayılan EEPROM', null], ['Türkçe + UHF EEPROM', ee => { ee[0x018] |= 0x80; ee[0x58C] = 0x3E; ee[0x58D] = 0x80; }]]) {
    const b = radio(mod), seen = [];
    for (const t of [0.6, 3.0, 5.4, 7.5]) { b.run(FCLK * (t - b.now / FCLK)); seen.push(lcd(b)); }
    check('açılış ekranı 5 s yATOLYE (' + name + ')', seen[0] === 'yATOLYE ' && seen[1] === 'yATOLYE ' && seen[2] === 'yATOLYE ' && seen[3] !== 'yATOLYE ', seen.join('|'));
  }
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
  scenario(b, 'yüklemeden sonra SAYI çalışıyor', 'mms 2 0 2 6 s', 'LIST|NUMBER|_|2_|20_|202_|2026_|6202');
  if (RING.includes('FREQ')) { keys(b, 'm'); const g = keys(b, 'mmmmms156525##'); check('yüklemeden sonra FREKANS çalışıyor', g[g.length - 1] === '156 5250' && (b.radioState().vco / 1e6).toFixed(4) === '201.5250', g.join('|')); }
  if (RING.includes('GAME')) { keys(b, 'm'); const g = keys(b, 'mmmms123#'); check('yüklemeden sonra OYUN çalışıyor', g[4] === '___     ' && /^123 [v^]  1$|^WIN    1$/.test(g[8]), g.join('|')); }
  if (RING.includes('CALC')) { keys(b, 'm'); scenario(b, 'yüklemeden sonra HESAP çalışıyor', 'mmms 7 * * * 6 #', 'LIST|NUMBER|CALC|       0|       7|+      7|-      7|*      7|*      6|      42'); }
}

out(fails ? fails + ' test başarısız' : 'tüm özel firmware testleri geçti');
if (isNode && fails) process.exit(1);
