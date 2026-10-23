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
/* FREKANS firmware'i yalnız Türkçe (dil_kaldir.inc): beklenen İngilizce ekranlar Türkçeye çevrilir */
const TR = RING.includes('FREQ');
const TRMAP = { 'LIST': 'LISTE', 'FREQ': 'FREKANS', 'SUB MENU': 'ALT MENU', 'TX FREQ': 'GONDERME', 'RX TONE': 'ALMA TON',
  'TX TONE': 'GON TON', 'ERROR': 'HATA', '  SCAN  ': ' TARAMA ', 'CALL -': 'CAGRI -', 'LAT -': 'ENLEM -', 'LON -': 'BOYLAM -' };
const T = s => TR ? s.split('|').map(x => TRMAP[x] !== undefined ? TRMAP[x] : TRMAP[x.trimEnd()] !== undefined ? TRMAP[x.trimEnd()].padEnd(8) : x).join('|') : s;
const AFTER_NUMBER = RING[RING.indexOf('NUMBER') + 1] || 'KANAL 01';
function keys(b, seq) {
  const screens = [];
  for (const k of seq) {
    const i = KP.indexOf(k === 'P' ? 'p' : k);           // 'P' = p tuşunu ~1,3 s basılı tut (tus_guc.inc: tarama)
    b.keypad.press(i, true, b); b.run(FCLK * (k === 'P' ? 1.3 : 0.15)); b.keypad.press(i, false, b); b.run(FCLK * 0.5);
    screens.push(lcd(b));
  }
  return screens;
}
/* "m m s 1 2 3 ..." -> ekranlar; beklenenle karşılaştır */
function scenario(b, name, seq, expect) {
  const got = keys(b, seq.replace(/ /g, ''));
  const exp = T(expect).split('|');
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
  // [original keys, patched keys]: SUB MENU is one m further in the patched ring (after NUMBER);
  // stops at DISPLAY: PAROLA is removed in FREKANS (parola_rssi_kaldir.inc)
  const run = (r, seq) => { const b = new Board(r), ee = B.defaultEeprom(); ee[0x018] |= 0x20 | (TR ? 0x80 : 0); B.eeFixSums(ee); b.eeprom.mem.set(ee);
    b.font = V.buildFont(b.rom); boot(b, 2.5); return keys(b, seq).join('|'); };
  const extra = 'm'.repeat(RING.length);
  for (const [so, sp] of [['ms05mmsms', 'ms05mmsms'], ['p', TR ? 'P' : 'p'], ['mmsmsm', 'm' + extra + 'msmsm']]) {
    let c = run(rom, sp); for (const t of RING) c = c.replace('|' + T(t).padEnd(8), '');
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
  check('FREKANS açıkken menü zaman aşımı yok (8 s)', lcd(b) === T('TX FREQ ') && b.ram[0x227] !== 0, lcd(b) + ' menü ' + b.ram[0x227]);
  scenario(b, 'gönderme frekansı haneleri', '1 6 0 6 5', '1-- ----|16- ----|160 ----|160 6---|160 65--');
  scenario(b, 'boşken * alma frekansına döner', '* * * * * *', '160 6---|160 ----|16- ----|1-- ----|TX FREQ|156 5250');
  scenario(b, 'alma frekansı düzeltilir', '* * 7 5 #', '156 525_|156 52__|156 527_|156 5275|TX FREQ');
  keys(b, 'm'); keys(b, enter);
  scenario(b, 'yeniden: alma 156,525, gönderme 160,650', '1 5 6 5 2 5 # 1 6 0 6 5 0 # # #', '1__ ____|15_ ____|156 ____|156 5___|156 52__|156 525_|TX FREQ|1-- ----|16- ----|160 ----|160 6---|160 65--|160 650-|RX TONE|TX TONE|156 5250');
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
  scenario(b, 'bırakınca # ve ton adımları ayarlar', '# # #', 'RX TONE|TX TONE|156 5250');
  { const g = keys(b, 's156525#177000#*');
    check('gönderme adımında bant dışı: hata, sonra girişe dönüş', g[g.length - 2] === T('ERROR   ') && g[g.length - 1] === '177 000-', g.join('|')); }
  keys(b, 'm');
  for (const [ks, f, name] of [['1565125', 156.5125e6, '12,5 kHz adım'], ['156010', 156.010e6, '10 kHz adım'], ['1560050', 156.005e6, '5 kHz adım'], ['176900', 176.9e6, 'bant üst ucu']]) {
    keys(b, enter + 's' + ks + '####');
    const want = ((f + 45e6) / 1e6).toFixed(4);
    check(name + ': ' + (f / 1e6) + ' MHz', vco(b) === want && synthBytes(rec(b)) === reference(f), vco(b) + ' ' + synthBytes(rec(b)) + ' / ' + reference(f));
    keys(b, 'm');
  }
  keys(b, enter + 's');
  scenario(b, '2,5 kHz ızgarası dışı: hata', '1 5 6 0 0 1 0 #', '1__ ____|15_ ____|156 ____|156 0___|156 00__|156 001_|156 0010|ERROR   ');
  scenario(b, 'hatadan sonraki tuş girişe döner', '*', '156 0010');
  scenario(b, '8. hane yok sayılır', '5', '156 0010');
  last = (name, seq, exp) => { const g = keys(b, seq.replace(/ /g, '')); check(name, g[g.length - 1] === T(exp).padEnd(8), '"' + g[g.length - 1] + '"'); };
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
  keys(u, enter + '433125####');
  check('UHF: 433,125 MHz, RX VCO 388,125', vco(u) === '388.1250' && synthBytes(rec(u)) === reference(433.125e6, true), vco(u) + ' ' + synthBytes(rec(u)) + ' / ' + reference(433.125e6, true));
  keys(u, 'm'); u.inputs.P4 = 0x7F; u.run(FCLK * 0.6);
  check('UHF PTT: TX 433,125', vco(u) === '433.1250', vco(u));
  u.inputs.P4 = 0xFF; u.run(FCLK * 0.4);
  keys(u, enter + '433125#438125###m');
  check('UHF çift frekans: RX VCO 388,125, baytlar firmware ile aynı', vco(u) === '388.1250' && synthBytes(rec(u)) === reference(433.125e6, true, 438.125e6), vco(u) + ' ' + synthBytes(rec(u)) + ' / ' + reference(433.125e6, true, 438.125e6));
  u.inputs.P4 = 0x7F; u.run(FCLK * 0.6);
  check('UHF PTT: TX 438,125, ekranda 438 1250', vco(u) === '438.1250' && lcd(u) === '438 1250', vco(u) + ' ' + lcd(u));
  const t = radio(ee => { ee[0x018] |= 0x80; });
  boot(t, 2.5);
  const g = keys(t, enter + '1#');
  check('Türkçe: 2. adımda GONDERME', keys(t, '*s156525#')[8] === 'GONDERME', lcd(t));
  check('Türkçe: FREKANS / HATA', g[RING.indexOf('FREQ') + 1] === 'FREKANS ' && g[g.length - 1] === 'HATA    ', g.join('|'));
  check('Türkçe: ton adımları ALMA TON / GON TON', keys(t, '160650##').slice(-2).join('|') === 'ALMA TON|GON TON ', lcd(t));

  /* TKS (CTCSS) tonları: 3. adım alma, 4. adım gönderme ($0375 / $037C alt 6 bit) */
  const c = radio(ee => B.eeSetChannel(ee, 1, 156e6, 156e6, { name: 'TKS', rxTone: 5, txTone: 9 }));
  boot(c, 2.5);
  const ton = () => [c.ram[0x375] & 0x3F, c.ram[0x37C] & 0x3F].join('/');
  check('kanal 1 tonları: alma 5, gönderme 9', ton() === '5/9', ton());
  keys(c, enter);
  scenario(c, 'ton haneleri, en çok 4', '1 5 6 5 2 5 # 1 6 0 6 5 0 # 8 8 5 # 1 0 0 0 0 #',
    '1__ ____|15_ ____|156 ____|156 5___|156 52__|156 525_|TX FREQ|1-- ----|16- ----|160 ----|160 6---|160 65--|160 650-|RX TONE|RT 8___|RT 88__|RT 885_|TX TONE|TT 1___|TT 10__|TT 100_|TT 1000|TT 1000|156 5250');
  check('alma 88,5 (8), gönderme 100,0 (12); kopya $0363 aynı', ton() === '8/12' && (c.ram[0x368] & 0x3F) === 8 && (c.ram[0x36F] & 0x3F) === 12, ton());
  check('frekans yine ayarlandı', vco(c) === '201.5250', vco(c));
  keys(c, 'm');
  c.inputs.P4 = 0x7F; c.run(FCLK * 0.6);
  { const a = V.analyseTx(c, FCLK, FCLK * 0.05);
    check('PTT: TX 160,650, TKS 100,0 Hz', vco(c) === '160.6500' && a.ctcssHz && Math.abs(a.ctcssHz - 100) < 1, vco(c) + ' ' + (a.ctcssHz && a.ctcssHz.toFixed(2))); }
  c.inputs.P4 = 0xFF; c.run(FCLK * 0.4);
  /* alma tonu: firmware'in ton çözücüsü ($023A bit 12) yalnız 88,5 Hz'te. (Hoparlörü tonla açma
   * panelden açılan TKS susturmasına bağlı, 0x023E bit 0; orijinal firmware'de de öyle.) */
  c.inputs.P5 |= 0x80; c.run(FCLK * 0.4);
  const cozuldu = f => { c.ctcssIn.setTone(f); c.run(FCLK * 0.5); let n = 0; for (let i = 0; i < 20; i++) { c.run(FCLK * 0.025); if (c.ram[0x23B] & 0x10) n++; } return n; };
  const c885 = cozuldu(88.5), c100 = cozuldu(100), c0 = cozuldu(0);
  c.inputs.P5 &= ~0x80; c.run(FCLK * 0.4);
  check('alma TKS 88,5: firmware 88,5 Hz\'i çözüyor, 100 Hz\'i ve tonsuzu çözmüyor', c885 >= 18 && c100 <= 2 && c0 === 0, '88,5: ' + c885 + '/20, 100: ' + c100 + '/20, yok: ' + c0 + '/20');
  const tonla = (ks, name, want) => { keys(c, enter + 's156525#160650#' + ks); check(name, ton() === want, ton() + ' ' + lcd(c)); keys(c, 'm'); };
  tonla('##', 'boş #: alma tonu kalır, gönderme = alma', '8/8');
  tonla('0#0#', '0 #: ton yok', '0/0');
  tonla('67#2503#', 'tam Hz (67) ve ondalıklı (250,3)', '1/38');
  tonla('700#1234#', 'en yakın ton: 70,0 -> 69,3 (39), 123,4 -> 123,0 (18)', '39/18');
  { const g = keys(c, enter + 's156525#160650#50#');
    check('tablodan uzak ton (50 Hz): hata, tonlar değişmedi', g[g.length - 1] === T('ERROR   ') && ton() === '39/18', g.slice(-1) + ' ' + ton());
    keys(c, '*'); scenario(c, 'hatadan sonra girişe dönüş, boşken * bir adım geri', '* * * *', 'RT 5___|RX TONE|TX FREQ|156 5250'); keys(c, 'm'); }
}

/* APRS (ozel_firmware/ornekler/aprs.inc): PTT'de 144,800 MHz'de AFSK konum paketi */
if (RING.includes('APRS')) {
  /* ayarlar EEPROM'daki APRS bölümünde (0x6E0, p4000.js aprsSet); bölüm yoksa ROM varsayılanları */
  const P = globalThis.P4000;
  const aprs = o => ee => { if (o) P.aprsSet(ee, o); };
  const AYAR = { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'Düzce, 73 de TA7W!', tablo: '/', kod: '>' };
  const PAKET = 'TA7W-7>APIZCI,WIDE2-2:!4050.63N/03109.39E>' + String.fromCharCode(...P.utf8('Düzce, 73 de TA7W!'));
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
  scenario(b, 'APRS: gir', enter, ['LIST', ...RING.slice(0, RING.indexOf('APRS') + 1), 'APRS AKT'].join('|'));
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
  check('APRS açıkken menü zaman aşımı yok (8 s)', lcd(b) === 'APRS AKT' && vco(b) === '189.8000', lcd(b) + ' ' + vco(b) + ' halka ' + halka.join('|'));
  b.inputs.P4 = 0x7F; b.run(FCLK * 1.4); b.inputs.P4 = 0xFF; b.run(FCLK * 0.4);
  check('ikinci paket de gitti', lcd(b) === 'APRS OK ', lcd(b));
  /* kanal anahtarı APRS'teyken çevrilirse çıkışta yeni kanal korunur */
  b.panel.setExt(((16 - 3) & 15) << 4 | 0x0F, b); b.run(FCLK * 0.6);
  keys(b, 'm'); b.run(FCLK * 0.3);
  check('APRS\'te kanal değişti: çıkınca yeni kanal kalır', vco(b) === '201.0500', vco(b));
  /* başka ayarlar: paket gerçekten EEPROM'daki bölümden kuruluyor */
  const VARS = 'TC19MK-10>APIZCI,WIDE2-2:!3955.52N/03250.22EbASELSAN 4000';
  const ham = (o, f) => ee => { P.aprsSet(ee, o); f(ee); P.aprsFix(ee); };      // aprsSet'in reddedeceği içerik
  const yaz = (a, t) => ee => t.split('').forEach((c, i) => { ee[0x6E0 + a + i] = c.charCodeAt(0); });
  const uzun = 'Merhaba dunya: bu yorum tam kirk uc bayt...';
  for (const [ad, mod, bek] of [
    ['bölüm yok: ROM varsayılanları (Anıtkabir, bisiklet, TC19MK-10)', null, VARS],
    ['varsayılanlar EEPROM\'a yazılı', aprs(P.APRS_VARSAYILAN), VARS],
    ['bölümün sağlaması bozuk: ROM varsayılanları', ee => { P.aprsSet(ee, AYAR); ee[0x6E0 + 40] ^= 1; }, VARS],
    ['SSID 12, güney / batı, simge tablosu \\, yorum yok', aprs({ cagri: 'N0CALL-12', enlem: '-33.86833', boylam: '151 12.34 W', tablo: '\\', kod: 'k', yorum: '' }),
      'N0CALL-12>APIZCI,WIDE2-2:!3352.10S\\15112.34Wk'],
    ['SSID yok, 43 baytlık yorum', aprs({ cagri: 'TA1AB', yorum: uzun }), 'TA1AB>APIZCI,WIDE2-2:!3955.52N/03250.22Eb' + uzun],
    ['küçük harfli konum harfi büyüğe çevrilir', ham({ cagri: 'TA1AB' }, yaz(23, 'n')), 'TA1AB>APIZCI,WIDE2-2:!3955.52N/03250.22EbASELSAN 4000']]) {
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
    check('APRS hatalı ayar: ' + ad + ' -> ' + ekran + ', paket yok', g[g.length - 1] === T(ekran).padEnd(8) && f.length === 0, g[g.length - 1] + ' / ' + f.length + ' paket');
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
    check('APRS otomatik: aralık 0 ise gönderim yok', n === 0 && lcd(r) === 'APRS AKT', n + ' örnek');
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
    check('APRS modu: kapat-aç -> menü seçmeden APRS AKT, alma 144,800', lcd(r2) === 'APRS AKT' && vco(r2) === '189.8000' && r2.ram[0x227] === 0x17, lcd(r2) + ' ' + vco(r2));
    const log = [], push = r2.pushAudio;
    r2.pushAudio = (buf, v) => { if (buf === r2.audio.ta1) log.push(r2.now, v); push.call(r2, buf, v); };
    r2.run(FCLK * 14); r2.pushAudio = push;
    const f = AFSK.decode(log, FCLK);
    check('APRS modu: kapat-aç sonrası otomatik gönderim sürüyor (12 s)', f.length === 1 && f[0].fcsOk && f[0].text === PAKET && lcd(r2) === 'APRS OK ', f.length + ' paket, ' + lcd(r2));
    const r3 = yenidenAc(Uint8Array.from(r2.eeprom.mem));
    check('APRS modu: ikinci kapat-aç da APRS\'te', lcd(r3) === 'APRS AKT' && r3.eeprom.mem[0x732] === 0xA5, lcd(r3));
    keys(r3, 'm'); r3.run(FCLK * 0.3);
    check('APRS modu: m ile çıkınca 0x732 = 0, kanala dönüş', lcd(r3) === 'KANAL 01' && r3.eeprom.mem[0x732] === 0 && vco(r3) === vco0, lcd(r3) + ', 0x732 = ' + r3.eeprom.mem[0x732] + ', ' + vco(r3));
    const r4 = yenidenAc(Uint8Array.from(r3.eeprom.mem));
    check('APRS modu: çıktıktan sonra kapat-aç -> normal ekran', lcd(r4) === 'KANAL 01' && r4.ram[0x227] === 0, lcd(r4));
    keys(r4, 'm');
    check('APRS modu: menü halkası normal', lcd(r4) === T('LIST    '), lcd(r4));
    /* araç / sabit kart (EE 0x58B bit 1: I²C P60 üzerinden; kullanıcının telsizi) */
    const ea = Uint8Array.from(ee0); ea[0x58B] |= 2; B.eeFixSums(ea);
    const a1 = yenidenAc(ea, true); for (let k = 0; k < 16 && lcd(a1) !== 'APRS    '; k++) keys(a1, 'm'); keys(a1, 's'); a1.run(FCLK * 0.3);   // araçta menü halkası farklı (CHN SCAN ...)
    const a2 = yenidenAc(Uint8Array.from(a1.eeprom.mem), true);
    check('APRS modu (araç / sabit kart): kapat-aç -> APRS AKT', a1.eeprom.mem[0x732] === 0xA5 && lcd(a2) === 'APRS AKT', '0x732 = ' + a1.eeprom.mem[0x732].toString(16) + ', ' + lcd(a2));
    /* B tipi kartta YAZICI açıkken (0x0240 bit 8) firmware her kimlik gönderiminde UART0'a günlük
     * satırı yazıyordu ("4       ·01/01 00:02"); APRS menüsünde susturuldu (aprs_seri.inc) */
    { const g = yenidenAc(Uint8Array.from(a1.eeprom.mem), true); g.ram[0x241] |= 1;
      let su = []; const ub = g.uartByte.bind(g); g.uartByte = (n, v) => { if (n === 0) su.push(v); return ub(n, v); };
      g.inputs.P4 &= ~0x80; g.run(FCLK * 1.5); g.inputs.P4 |= 0x80; g.run(FCLK * 0.8);
      const t = String.fromCharCode(...su);
      check('APRS: YAZICI açıkken PTT seri porta günlük satırı yazmaz', lcd(g) === 'APRS OK ' && t === '', lcd(g) + ' ' + JSON.stringify(t)); }
    /* YAZICI kaldırıldı (yazici_kaldir.inc): B tipi kartta menüde yok; EEPROM'da (0x54B bit 3,
     * 0x552 sağlaması) açık kalmışsa da açılışta kapalı, UART0 9600'de (orijinalde 2400) */
    { const y = radio(ee => { ee[0x014] |= 0x0A; ee[0x58B] |= 2; ee[0x54B] |= 0x08;
        let t = 0; for (let a = 0x54B; a <= 0x551; a++) t += ee[a]; ee[0x552] = t >> 8 & 255; ee[0x553] = t & 255; });
      y.hwArac = true; y.inputs.P4 = 0xCF; y.panel.setExt(0xBF, y); boot(y, 2.6);
      const halka = keys(y, 'mmmmmmm');
      check('YAZICI kaldırıldı: menü halkasında yok, kayıtlıysa da kapalı başlar, seri port 9600',
        !halka.some(t => /PRINTER|YAZICI/.test(t)) && halka.includes('APRS    ') && !(y.ram[0x241] & 1) && y.sfr[0x31] === 25,
        halka.join('|') + ' yazıcı=' + (y.ram[0x241] & 1) + ' U0BRG=' + y.sfr[0x31]); keys(y, 'm'); }
  }
  /* alma (aprs_rx.inc): P66'ya (TB1IN, sıfır-geçiş algılayıcısı) AFSK paket; ekranda gönderen */
  {
    const r = radio(aprs(AYAR)); boot(r, 2.5); keys(r, enter); r.run(FCLK * 0.3);
    /* seri port (aprs_seri.inc): UART0'a giden baytlar */
    let seri = []; const ub = r.uartByte.bind(r); r.uartByte = (n, v) => { if (n === 0) seri.push(v); return ub(n, v); };
    const seriAl = () => { const t = String.fromCharCode(...seri); seri = []; return t; };
    const al = (text, o, bozuk) => {
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
    seriAl();
    const ilk = al('TA7W-7>APRS,WIDE1-1:!4050.63N/03109.39E>hello');
    { const t = seriAl(); check('APRS alma: paket seri porta TNC2 satırı', t === 'TA7W-7>APRS,WIDE1-1:!4050.63N/03109.39E>hello\r\n', JSON.stringify(t)); }
    check('APRS alma: paket -> ekranda gönderen "TA7W-7"', ilk === 'TA7W-7  ', ilk);
    r.run(FCLK * 4.5);
    check('APRS alma: ~4 s sonra APRS ekranına dönüş', lcd(r) === 'APRS AKT', lcd(r));
    const zor = [['iki haneli SSID', 'YM7KA-12>APDR15:=4100.00N/02900.00E-x', {}, 'YM7KA-12'],
      ['6 harf + SSID 15 (tire yok)', 'TB1ABC-15>APRS:>status', {}, 'TB1ABC15'],
      ['SSID yok, yol 3 adres', 'TA1XX>APRS,WIDE1-1,WIDE2-2,TA2DEF-3*:>uzun bir durum metni 0123456789 abcdefghij', {}, 'TA1XX'],
      ['2200 Hz yarı genlik + kayma', 'TA2AA-1>APRS:!4050.63N/03109.39E>', { as: 0.5, off: 0.2 }, 'TA2AA-1'],
      ['2200 Hz iki kat genlik', 'TA2AB-2>APRS:!4050.63N/03109.39E>', { as: 2 }, 'TA2AB-2'],
      ['baud +%0,3', 'TA2AC-3>APRS:!4050.63N/03109.39E>', { baud: 1203.6 }, 'TA2AC-3'],
      ['baud -%0,3', 'TA2AD-4>APRS:!4050.63N/03109.39E>', { baud: 1196.4 }, 'TA2AD-4'],
      ['gürültü %10', 'TA2AE-5>APRS:!4050.63N/03109.39E>', { noise: 0.1, seed: 7 }, 'TA2AE-5']];
    for (const [ad, text, o, bek] of zor) { const g = al(text, o); check('APRS alma: ' + ad, g === bek.padEnd(8), g); }
    { const t = seriAl(), bek = zor.map(z => z[1] + '\r\n').join('');
      check('APRS alma: hepsi seri portta, sırayla (SSID, yol, yıldız, uzun metin)', t === bek, JSON.stringify(t)); }
    const bz = al('TA9ZZ>APRS:>bozuk', {}, true);
    check('APRS alma: sağlaması bozuk paket seri porta yazılmaz', seriAl() === '', '');
    check('APRS alma: sağlaması bozuk paket gösterilmez', bz !== 'TA9ZZ   ' && r.ram[0x623] === 9 - 1, bz + ', liste ' + r.ram[0x623]);
    /* liste: en yeni başta, en çok 8; '*' eskiye, '#' yeniye */
    const liste = keys(r, '*********');
    check('APRS alma: liste * ile (8 istasyon, en eskide kalır)', liste.join('|') === ['TA2AE-5', 'TA2AD-4', 'TA2AC-3', 'TA2AB-2', 'TA2AA-1', 'TA1XX', 'TB1ABC15', 'YM7KA-12', 'YM7KA-12'].map(x => x.padEnd(8)).join('|'), liste.join('|'));
    const geri = keys(r, '#'.repeat(8));
    check('APRS alma: # ile yeniye, en yeniden sonra APRS ekranı', geri[6] === 'TA2AE-5 ' && geri[7] === 'APRS AKT', geri.join('|'));
    al('TA2AB-2>APRS:>tekrar');
    const l2 = keys(r, '**');
    check('APRS alma: tekrar duyulan istasyon başa geçer, çift kayıt yok', l2.join('|') === 'TA2AB-2 |TA2AE-5 ' && r.ram[0x623] === 8, l2.join('|') + ', ' + r.ram[0x623]);
    /* istasyon detayı (aprs_detay.inc): 's' çağrı işareti -> enlem -> boylam; konumsuzda KONUM - */
    { const d1 = al('TA5DT-1>APRS,WIDE1-1:!4050.63N/03109.39E>detay'), s1 = keys(r, 'ssss');
      check('APRS detay: s ile enlem / boylam / mesaj / çağrı işareti', d1 === 'TA5DT-1 ' && s1.join('|') === '4050 63N|3109 39E|DETAY   |TA5DT-1 ', d1 + ' -> ' + s1.join('|'));
      const d2 = al('TA6DT-2>APDR15:@092345z4100.12S/10200.50W-x'), s2 = keys(r, 'ss');
      check('APRS detay: zaman damgalı, güney / batı, boylam >= 100', s2.join('|') === '4100 12S|10200 5W', s2.join('|'));
      al('TA7DT-3>APRS:>yalniz durum'); const s3 = keys(r, 'ss');
      check('APRS detay: konumsuz paket KONUM -', s3.join('|') === 'KONUM - |KONUM - ', s3.join('|'));
      const g = keys(r, '**'), s4 = keys(r, 's');
      check('APRS detay: listede gezinirken o istasyonun konumu', g[1] === 'TA6DT-2 ' && s4[0] === '4100 12S', g.join('|') + ' -> ' + s4.join('|'));
      const n0 = r.ram[0x623]; al('TA5DT-1>APRS:>yine ben'); const s5 = keys(r, 'ss');
      check('APRS detay: farklı yollu paketle tekrar duyulan istasyon çift girmez, eski konumu kalır', r.ram[0x623] === n0 && s5.join('|') === '4050 63N|3109 39E', 'liste ' + n0 + ' -> ' + r.ram[0x623] + ', ' + s5.join('|'));
      /* mesaj sayfası: konumdan sonraki yorum, durum, mesaj metni, Mic-E yorumu; ilk 8 karakter */
      const m1 = al('TA5DT-1>APRS,WIDE1-1:!4050.63N/03109.39E>Merhaba Duzce 73'), sm1 = keys(r, 'ssss');
      check('APRS detay: 4. sayfa yorum, büyük harf; sonra çağrı işaretine döner', sm1.join('|') === '4050 63N|3109 39E|MERHABA |TA5DT-1 ', sm1.join('|'));
      const mt = [['durum', 'TA8DT-4>APRS:>durum: qrv', 'DURUM  Q'], ['mesaj', 'TA8DT-5>APRS::TA7W-7   :selam abi{01', 'SELAM AB'],
        ['Mic-E yorumu', "TA8DT-6>S32U6T,WIDE1-1:`(_fn\"Oj/]yolda", 'YOLDA   '], ['metinsiz konum', 'TA8DT-7>APRS:!4050.63N/03109.39E>', 'MESAJ - ']];
      for (const [ad, pk, bek] of mt) { al(pk); const g2 = keys(r, 'sss'); check('APRS detay mesajı: ' + ad, g2[2] === bek, g2.join('|')); }
      keys(r, '#'.repeat(9)); }
    /* gönderme sonrası alma sürer */
    const f = paketGonder(r);
    check('APRS alma: PTT ile paket hâlâ gidiyor', f.length === 1 && f[0].fcsOk && f[0].text === PAKET, f.length + ' paket');
    seriAl();
    const sonra = al('TA3BB-9>APRS:>after tx');
    check('APRS alma: gönderdikten sonra yine alıyor', sonra === 'TA3BB-9 ', sonra);
    { const t = seriAl(); check('APRS alma: gönderdikten sonra seri porta da yazıyor', t === 'TA3BB-9>APRS:>after tx\r\n', JSON.stringify(t)); }
    /* yalnız tekrarlayıcıların sonuncusunda yıldız; bilgi alanındaki CR boşluk; uzun paket kesilir */
    al('YM7KA-12>APDR15,TA2DEF-3*,WIDE2*:=4100.00N/02900.00E-x');
    { const t = seriAl(); check('APRS alma: yıldız yalnız son tekrarlayıcıda', t === 'YM7KA-12>APDR15,TA2DEF-3,WIDE2*:=4100.00N/02900.00E-x\r\n', JSON.stringify(t)); }
    const uzun = 'TA4CC>APRS:>' + 'abcdefghij'.repeat(18);
    al(uzun);
    { const t = seriAl(), bek = uzun.slice(0, 'TA4CC>APRS:'.length + 140 - 16) + '\r\n';
      check('APRS alma: uzun paket (196 bayt) tamponla sınırlı (140): bilgi alanının ilk 124 baytı', t === bek, t.length + ' ' + JSON.stringify(t.slice(-20))); }
    /* sürekli gürültü (susturma açık): bekçi köpeği yok, tuşlar çalışıyor */
    r.selIn.noise = true; r.inputs.P5 |= 0x80; const ev0 = r.events.length; r.run(FCLK * 3);
    const tus = keys(r, '#');
    r.selIn.noise = false; r.inputs.P5 &= ~0x80; r.run(FCLK * 0.5);
    check('APRS alma: 3 s gürültüde yeniden başlama yok, tuş çalışıyor', !r.events.slice(ev0).some(e => /WATCHDOG|RESET/.test(e[1])) && tus[0] === 'APRS OK ', tus.join('|'));
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

/* tarama (frekans.inc: 0x832C düzeltmesi; tarama_tks.inc): liste 1 / 2 / 5, kanallarda 67 Hz alma TKS'si.
 * Taramada telsiz yeniden başlamamalı; sinyal varken TKS bekleme süresi (0x020A) orijinal + 10 */
if (RING.includes('FREQ')) {
  const b = radio(ee => { for (const n of [1, 2, 5]) ee[0x2E + (n - 1) * 13 + 4] = (ee[0x2E + (n - 1) * 13 + 4] & 0xC0) | 1; });
  boot(b, 2.5); const ev0 = b.events.length;
  const g = keys(b, 'msmmsmms05mmsms' + 'P'); b.run(FCLK * 1);
  const reset = b.events.slice(ev0).some(e => e[1] === 'RESET');
  check('tarama başlıyor, telsiz yeniden başlamıyor', g[g.length - 1] === T('  SCAN  ') && lcd(b) === T('  SCAN  ') && !reset, g.slice(-2).join('|') + ' ' + lcd(b) + (reset ? ' RESET' : ''));
  let ilk = 0; b.ramHook = (a, v) => { if (a === 0x20A && v > 1 && !ilk) ilk = v; };
  b.inputs.P5 |= 0x80;
  for (let i = 0; i < 100 && !ilk; i++) { b.ram[0x23E] |= 1; b.run(FCLK * 0.01); }   // 0x023E bit 0: TKS susturması (panelden)
  b.ramHook = null; b.inputs.P5 &= ~0x80;
  check('taramada TKS bekleme 67 Hz: 15 + 10 = 25 adım (800 ms)', ilk === 25, '0x020A = ' + ilk);
}

/* p / q tuşları (tus_guc.inc): p kısa = güç yüksek / düşük ("YUKSEK G" / "ALCAK G", ~1,5 s), p uzun = tarama,
 * taramada kısa p durdurur; q = monitör ("MONITOR*" / "MONITOR-", 0x023A bit 13); menüde q yutulur, sonraki tuşlar çalışır;
 * ayarlanmış VFO'da ve APRS ekranında da p güç */
if (RING.includes('FREQ')) {
  const b = radio(); boot(b, 2.5);
  const guc = () => (b.ram[0x240] & 0x40 ? (b.ram[0x23C] & 0x10 ? 'Y' : 'A') : '-');
  const g1 = keys(b, 'p'), y1 = guc(); b.run(FCLK * 2); const g1b = lcd(b);
  const g2 = keys(b, 'p'), y2 = guc(); b.run(FCLK * 2);
  check('p kısa: güç yüksek, sonra düşük; ekran geri döner', g1[0] === 'YUKSEK G' && y1 === 'Y' && g1b === 'KANAL 01' && g2[0] === 'ALCAK G ' && y2 === 'A' && lcd(b) === 'KANAL 01',
    g1[0] + ' ' + y1 + ' ' + g1b + ' ' + g2[0] + ' ' + y2 + ' ' + lcd(b));
  const q1 = keys(b, 'q'), m1 = b.ram[0x23B] >> 5 & 1, p46 = b.latch[0x0A] >> 6 & 1; b.run(FCLK * 2);
  const q2 = keys(b, 'q'), m2 = b.ram[0x23B] >> 5 & 1; b.run(FCLK * 2);
  check('q: monitör aç / kapa', q1[0] === 'MONITOR*' && m1 === 1 && p46 === 1 && q2[0] === 'MONITOR-' && m2 === 0 && lcd(b) === 'KANAL 01', q1[0] + ' ' + m1 + ' P46=' + p46 + ' ' + q2[0] + ' ' + m2);
  const mq = keys(b, 'mqm'); keys(b, 'mmmmmmm'); b.run(FCLK * 5);
  check('menüde q yutulur, m çalışmaya devam eder', mq.join('|') === 'LISTE   |LISTE   |FREKANS ' && guc() === 'A', mq.join('|'));
  const c = radio(ee => { for (const n of [1, 2]) ee[0x2E + (n - 1) * 13] |= 0; }); boot(c, 2.5);
  keys(c, 'msmmsmmsms');
  const t1 = keys(c, 'P'), gc = c.ram[0x240] & 0x40; c.run(FCLK * 1);
  const t2 = keys(c, 'p');
  check('p uzun: tarama başlar (güç değişmez), kısa p taramayı durdurur', t1[0] === T('  SCAN  ') && !gc && t2[0] === 'KANAL 01' && !(c.ram[0x241] & 4), t1[0] + ' ' + gc + ' ' + t2[0]);
  const v = keys(c, 'mms145500####'); const vp = keys(c, 'p'); c.run(FCLK * 2);
  check('VFO ekranında p güç, sonra frekans geri gelir', v[v.length - 1] === '145 5000' && vp[0] === 'YUKSEK G' && lcd(c) === '145 5000', v[v.length - 1] + ' ' + vp[0] + ' ' + lcd(c));
  keys(c, 'm'); for (let k = 0; k < 12 && lcd(c) !== 'APRS    '; k++) keys(c, 'm'); keys(c, 's');
  const ap = keys(c, 'p'); c.run(FCLK * 2);
  check('APRS ekranında p güç, sonra APRS AKT', ap[0] === 'ALCAK G ' && lcd(c) === 'APRS AKT', ap[0] + ' ' + lcd(c));
  keys(c, 'm');
  /* araç / sabit kartında orijinal p taramaya bağlı değil (tarama KNL TARA menüsünde): uzun p onu kullanır */
  const d = radio(ee => { ee[0x58B] |= 2; for (const n of [1, 2]) ee[0x2E + (n - 1) * 13] |= 0; });
  d.hwArac = true; d.inputs.P4 = 0xCF; d.panel.setExt(0xBF, d); boot(d, 2.6);
  keys(d, 'mmsmmsmmsms'); d.run(FCLK * 5);
  const a1 = keys(d, 'P'), at = d.ram[0x241] & 4; d.run(FCLK * 1);
  const a2 = keys(d, 'p'), ad = d.ram[0x241] & 4; d.run(FCLK * 1);
  const a3 = keys(d, 'p');
  check('araç kartı: uzun p tarama (KNL TARA), kısa p durdurur, sonra güç', a1[0] === T('  SCAN  ') && at && !ad && d.ram[0x227] === 0 && a2[0] !== T('  SCAN  ') && a3[0] === 'YUKSEK G',
    a1[0] + ' ' + at + ' ' + a2[0] + ' ' + ad + ' ' + a3[0]);
}

/* APRS TR / DG / IG (aprs_ayar.inc): menüde '*' / '-', s ile değişir, EEPROM 0x733, kapat-aç sonrası kalır;
 * TR kapalıyken otomatik gönderim yok; IG kapalıyken seri porta yazılmaz ama detay çalışır */
if (RING.includes('APRS TR*')) {
  const P = globalThis.P4000;
  const AYAR = { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'x', tablo: '/', kod: '>', aralik: 10 };
  const b = radio(ee => P.aprsSet(ee, AYAR)); boot(b, 2.5);
  const yer = t => { for (let k = 0; k < 12 && lcd(b).slice(0, 7) !== t; k++) keys(b, 'm'); return lcd(b); };
  const tr0 = yer('APRS TR'), tr1 = keys(b, 's')[0], dg = (keys(b, 'm'), keys(b, 's')[0]), ig = (keys(b, 'm'), keys(b, 's')[0]);
  check('APRS TR / DG / IG: başlıkta durum, s ile değişir', tr0 === 'APRS TR*' && tr1 === 'APRS TR-' && dg === 'APRS DG*' && ig === 'APRS IG-',
    [tr0, tr1, dg, ig].join('|'));
  check('APRS ayarı EEPROM 0x733 = A2 (TR -, DG *, IG -), P4000.aprsAyarGet aynı', b.eeprom.mem[0x733] === 0xA2 && JSON.stringify(P.aprsAyarGet(b.eeprom.mem)) === '{"gecerli":true,"tr":false,"dg":true,"ig":false}',
    b.eeprom.mem[0x733].toString(16));
  const r = new Board(rom); r.eeprom.mem.set(b.eeprom.mem); r.font = b.font; boot(r, 2.5);
  for (let k = 0; k < 12 && lcd(r).slice(0, 7) !== 'APRS TR'; k++) keys(r, 'm');
  const g2 = [lcd(r), ...keys(r, 'mm')];
  check('APRS ayarı kapat-aç sonrası hatırlanıyor', g2.join('|') === 'APRS TR-|APRS DG*|APRS IG-', g2.join('|'));
  /* APRS ekranında: TR kapalı -> 12 s'de otomatik paket yok; IG kapalı -> seri port boş, detay var */
  keys(r, 'm'); for (let k = 0; k < 12 && lcd(r) !== 'APRS    '; k++) keys(r, 'm'); keys(r, 's');
  const log = [], push = r.pushAudio; r.pushAudio = (buf, v) => { if (buf === r.audio.ta1) log.push(r.now, v); push.call(r, buf, v); };
  r.run(FCLK * 13); r.pushAudio = push;
  check('APRS TR kapalı: aralık 10 s olsa da otomatik gönderim yok', lcd(r) === 'APRS AKT' && AFSK.decode(log, FCLK).length === 0, lcd(r));
  let seri = []; const ub = r.uartByte.bind(r); r.uartByte = (n, v) => { if (n === 0) seri.push(v); return ub(n, v); };
  const e = AFSK.edges(AFSK.encode('TA9IG-1>APRS:!4050.63N/03109.39E>igate yok'), {}), t = r.now;
  r.selIn.setEdges(e.map(x => t + Math.round(x * FCLK))); r.inputs.P5 |= 0x80; r.run(FCLK * (e[e.length - 1] + 0.05)); r.inputs.P5 &= ~0x80; r.run(FCLK * 0.4);
  const ek = lcd(r), d = keys(r, 'sss');
  check('APRS IG kapalı: seri porta yazılmaz, istasyon ve detay çalışır', seri.length === 0 && ek === 'TA9IG-1 ' && d.join('|') === '4050 63N|3109 39E|IGATE YO', seri.length + ' bayt, ' + ek + ' -> ' + d.join('|'));
  keys(r, 'm');
}

/* APRS DG (aprs_digi.inc): WIDE1-1 -> ÇAĞRIM*, WIDE2-2 -> ÇAĞRIM*,WIDE2-1, WIDE2-1 -> ÇAĞRIM,WIDE2*, doğrudan;
 * WIDE3-3 / kendi paketimiz / bizden geçmiş / 30 s'de aynısı tekrarlanmaz; IG açıkken önce seri porta, sonra digi */
if (RING.includes('APRS TR*')) {
  const P = globalThis.P4000;
  const b = radio(ee => { P.aprsSet(ee, { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'x', tablo: '/', kod: '>', aralik: 0 });
    P.aprsAyarSet(ee, { tr: false, dg: true, ig: true }); }); boot(b, 2.5);
  for (let k = 0; k < 12 && lcd(b) !== 'APRS    '; k++) keys(b, 'm'); keys(b, 's');
  let seri = []; const ub = b.uartByte.bind(b); b.uartByte = (n, v) => { if (n === 0) seri.push(v); return ub(n, v); };
  const dene = text => {
    const log = [], push = b.pushAudio; b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push.call(b, buf, v); };
    const e = AFSK.edges(AFSK.encode(text), {}), t = b.now; seri = [];
    b.selIn.setEdges(e.map(x => t + Math.round(x * FCLK))); b.inputs.P5 |= 0x80; b.run(FCLK * (e[e.length - 1] + 0.05)); b.inputs.P5 &= ~0x80;
    b.run(FCLK * 3.5); b.pushAudio = push;
    const f = AFSK.decode(log, FCLK);
    return { tx: f.map(x => (x.fcsOk ? '' : '[FCS] ') + x.text).join(' ; '), seri: String.fromCharCode(...seri) };
  };
  for (const [ad, giren, bek] of [
    ['WIDE1-1', 'TA1AA>APRS,WIDE1-1,WIDE2-1:!4050.63N/03109.39E>a', 'TA1AA>APRS,TA7W-7*,WIDE2-1:!4050.63N/03109.39E>a'],
    ['WIDE2-2', 'TA1AB>APRS,WIDE2-2:>b', 'TA1AB>APRS,TA7W-7*,WIDE2-1:>b'],
    ['30 s içinde aynı paket (başka digiden) tekrarlanmaz', 'TA1AB>APRS,DIGI1*,WIDE2-1:>b', ''],
    ['WIDE2-1', 'TA1AC>APRS,WIDE2-1:>c', 'TA1AC>APRS,TA7W-7,WIDE2*:>c'],
    ['doğrudan bize', 'TA1AD>APRS,TA7W-7:>d', 'TA1AD>APRS,TA7W-7*:>d'],
    ['başka digiden sonra WIDE2-1', 'TA1AE>APRS,TB2XX*,WIDE2-1:>e', 'TA1AE>APRS,TB2XX,TA7W-7,WIDE2*:>e'],
    ['WIDE3-3 tekrarlanmaz', 'TA1AF>APRS,WIDE3-3:>f', ''], ['yolsuz tekrarlanmaz', 'TA1AG>APRS:>g', ''],
    ['kendi paketimiz tekrarlanmaz', 'TA7W-7>APRS,WIDE1-1:>kendim', ''], ['bizden geçmiş tekrarlanmaz', 'TA1AH>APRS,TA7W-7*,WIDE2-1:>h', ''],
    ['30 s sonra aynı paket yeniden tekrarlanır', 'TA1AB>APRS,WIDE2-2:>b', 'TA1AB>APRS,TA7W-7*,WIDE2-1:>b']]) {
    const r = dene(giren), yol = giren.replace(/\*/g, '');
    check('APRS DG: ' + ad, r.tx === bek && r.seri === giren + '\r\n', JSON.stringify(r.tx) + ' | seri ' + JSON.stringify(r.seri));
  }
  /* DG kapalı: aynı paket tekrarlanmaz */
  const c = radio(ee => { P.aprsSet(ee, { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'x', tablo: '/', kod: '>', aralik: 0 }); }); boot(c, 2.5);
  for (let k = 0; k < 12 && lcd(c) !== 'APRS    '; k++) keys(c, 'm'); keys(c, 's');
  const log = [], push = c.pushAudio; c.pushAudio = (buf, v) => { if (buf === c.audio.ta1) log.push(c.now, v); push.call(c, buf, v); };
  const e = AFSK.edges(AFSK.encode('TA1ZZ>APRS,WIDE1-1:>z'), {}), t = c.now;
  c.selIn.setEdges(e.map(x => t + Math.round(x * FCLK))); c.inputs.P5 |= 0x80; c.run(FCLK * (e[e.length - 1] + 0.05)); c.inputs.P5 &= ~0x80; c.run(FCLK * 3.5);
  check('APRS DG kapalı (varsayılan): tekrarlanmaz', AFSK.decode(log, FCLK).length === 0 && lcd(c) === 'TA1ZZ   ', lcd(c));
  keys(b, 'm'); keys(c, 'm');
}

/* GPS (gps.inc): seri porttan $GPRMC; APRS ekranında hemen, sonra dakikada bir yeni konum; GPS geçersizleşince
 * en geç iki dakikada EEPROM konumuna dönüş; ekranda "GPS" */
if (RING.includes('APRS TR*')) {
  const P = globalThis.P4000;
  const b = radio(ee => P.aprsSet(ee, { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'gps', tablo: '/', kod: '>', aralik: 0 })); boot(b, 2.5);
  for (let k = 0; k < 12 && lcd(b) !== 'APRS    '; k++) keys(b, 'm'); keys(b, 's');
  const rmc = (lat, lon, d) => { const g = 'GPRMC,123519,' + (d || 'A') + ',' + lat + ',' + lon + ',022.4,084.4,230394,003.1,W'; let c = 0; for (const ch of g) c ^= ch.charCodeAt(0); return '$' + g + '*' + c.toString(16).toUpperCase().padStart(2, '0') + '\r\n'; };
  const gonder = (t, n) => { for (let i = 0; i < n; i++) { b.serialIn(Array.from(t, c => c.charCodeAt(0))); b.run(FCLK); } };
  const ptt = () => { const log = [], push = b.pushAudio; b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push.call(b, buf, v); };
    b.inputs.P4 = 0x7F; b.run(FCLK * 1.4); b.inputs.P4 = 0xFF; b.run(FCLK * 0.5); b.pushAudio = push; return AFSK.decode(log, FCLK).map(f => f.text).join(' ; '); };
  const konum = t => (/:!([^/\\]+)[/\\]([0-9.]+[EW])/.exec(t) || []).slice(1).join(' ');
  const k0 = konum(ptt());
  gonder(rmc('3955.5210,N', '03250.2234,E'), 3); const k1 = konum(ptt()), e1 = lcd(b);
  gonder(rmc('4100.1234,N', '02900.5678,E'), 20); const k2 = konum(ptt());
  gonder(rmc('4100.1234,N', '02900.5678,E'), 45); const k3 = konum(ptt());
  gonder(rmc('', '', 'V').replace('GPRMC,123519,V,,', 'GPRMC,123519,V,'), 125); const k4 = konum(ptt());
  check('GPS: yokken EEPROM konumu, gelince GPS konumu ve ekranda GPS', k0 === '4050.63N 03109.39E' && k1 === '3955.52N 03250.22E' && e1 === 'GPS  OK ', k0 + ' / ' + k1 + ' / ' + e1);
  check('GPS: yeni konum dakikada bir alınır', k2 === '3955.52N 03250.22E' && k3 === '4100.12N 02900.56E', k2 + ' / ' + k3);
  check('GPS: geçersiz cümlelerde EEPROM konumuna dönülür', k4 === '4050.63N 03109.39E', k4);
  keys(b, 'm');
}

/* arka plan tracker (aprs_arka.inc): yalnız TR açık; normal kanalda süre dolunca 30 s boşlukta 144,800'de gönder,
 * kanala dön; sinyalli kanalda bekle; VFO korunur. DG açıkken cihaz tipi APATAR. APRS ekranında ilk boşluk. */
if (RING.includes('APRS TR*')) {
  const P = globalThis.P4000;
  const ayar = { cagri: 'ta7w-7', enlem: '4050.63N', boylam: '03109.39E', yorum: 'arka', tablo: '/', kod: '>', aralik: 20 };
  const b = radio(ee => { P.aprsSet(ee, ayar); P.aprsAyarSet(ee, { tr: true, dg: false, ig: false }); }); boot(b, 2.5);
  const vco = () => (b.radioState().vco / 1e6).toFixed(4);
  const izle = (sn, sinyal) => { const log = [], push = b.pushAudio, ekran = new Set(), f = new Set();
    b.pushAudio = (buf, v) => { if (buf === b.audio.ta1) log.push(b.now, v); push.call(b, buf, v); };
    for (let i = 0; i < sn * 10; i++) { if (sinyal) { if (Math.floor(i / 50) % 2 === 0) b.inputs.P5 |= 0x80; else b.inputs.P5 &= ~0x80; } b.run(FCLK * 0.1); ekran.add(lcd(b)); f.add(vco()); }
    b.inputs.P5 &= ~0x80; b.pushAudio = push; return { p: AFSK.decode(log, FCLK).map(x => x.text), ekran: [...ekran], f: [...f] }; };
  let r = izle(60);
  check('arka plan tracker: 20 s + 30 s boşluk sonra 144,800\'de bir paket, kanala dönüş', r.p.length === 1 && r.p[0] === 'TA7W-7>APIZCI,WIDE2-2:!4050.63N/03109.39E>arka' &&
    r.ekran.includes('APRS TX ') && r.f.includes('144.8000') && lcd(b) === 'KANAL 01' && vco() === '201.0000', r.p.join(';') + ' | ' + r.ekran.join('|') + ' | ' + vco());
  r = izle(70, true);
  check('arka plan tracker: kanalda 5 s\'de bir sinyal varken (30 s boşluk yok) gönderilmez', r.p.length === 0 && !r.f.includes('144.8000'), r.p.length + ' paket');
  keys(b, 'mms156525#160650###m');
  r = izle(60);
  check('arka plan tracker: VFO modunda gönderip VFO frekansına döner, VFO modu kalır', r.p.length === 1 && lcd(b) === '156 5250' && vco() === '201.5250' && (b.ram[0x571] << 8 | b.ram[0x570]) === 0x4656, r.p.length + ' ' + lcd(b) + ' ' + vco());
  /* DG açık: kendi paketimiz APATAR; APRS ekranında beacon kanaldaki sinyal bitince */
  const c = radio(ee => { P.aprsSet(ee, Object.assign({}, ayar, { aralik: 10 })); P.aprsAyarSet(ee, { tr: true, dg: true, ig: false }); }); boot(c, 2.5);
  for (let k = 0; k < 12 && lcd(c) !== 'APRS    '; k++) keys(c, 'm'); keys(c, 's');
  check('APRS ekranı: APRS AKT', lcd(c) === 'APRS AKT', lcd(c));
  const log = [], push = c.pushAudio; let ilk = 0;
  c.pushAudio = (buf, v) => { if (buf === c.audio.ta1) { log.push(c.now, v); if (!ilk) ilk = c.now; } push.call(c, buf, v); };
  const t0 = c.now; c.inputs.P5 |= 0x80; c.run(FCLK * 16); const sinyalde = log.length; c.inputs.P5 &= ~0x80; c.run(FCLK * 3); c.pushAudio = push;
  const pk = AFSK.decode(log, FCLK).map(x => x.text);
  check('APRS ekranı: beacon kanalda sinyal varken beklenir, ilk boşlukta gider; DG açıkken APATAR', sinyalde === 0 && pk.length === 1 && /^TA7W-7>APATAR,WIDE2-2:/.test(pk[0]), sinyalde + ' / ' + pk.join(';'));
  keys(c, 'm');
  /* UHF telsiz (bant tabanı 400 MHz): APRS frekansı 432,500 (ekranda da, arka planda da) */
  const UHF = ee => { ee[0x58C] = 16000 >> 8; ee[0x58D] = 16000 & 255; B.eeSetChannel(ee, 1, 433e6, 433e6, { name: 'UHF' }); };
  const u = radio(ee => { UHF(ee); P.aprsSet(ee, ayar); P.aprsAyarSet(ee, { tr: true, dg: false, ig: false }); }); boot(u, 2.5);
  const uv = () => (u.radioState().vco / 1e6).toFixed(4), uf = new Set(), ulog = [], upush = u.pushAudio;
  u.pushAudio = (buf, v) => { if (buf === u.audio.ta1) ulog.push(u.now, v); upush.call(u, buf, v); };
  for (let i = 0; i < 600; i++) { u.run(FCLK * 0.1); uf.add(uv()); }
  u.pushAudio = upush;
  check('UHF: arka plan tracker 432,500\'de gönderir, kanala (433,000) döner', uf.has('432.5000') && !uf.has('144.8000') && AFSK.decode(ulog, FCLK).length === 1 && uv() === '388.0000',
    [...uf].join(',') + ' / ' + uv());
  for (let k = 0; k < 12 && lcd(u) !== 'APRS    '; k++) keys(u, 'm'); keys(u, 's'); u.run(FCLK * 0.5);
  const rx = uv(); u.inputs.P4 = 0x7F; u.run(FCLK * 0.6); const tx = uv(); u.inputs.P4 = 0xFF; u.run(FCLK * 1.5);
  check('UHF: APRS ekranında alma 432,500 (VCO 387,500), gönderme 432,500', rx === '387.5000' && tx === '432.5000' && lcd(u) === 'APRS OK ', rx + ' / ' + tx + ' / ' + lcd(u));
  keys(u, 'm');
}

/* yalnız Türkçe (dil_kaldir.inc): EEPROM'da İngilizce (0x018 bit 7 = 0) seçili olsa da menüler Türkçe */
if (TR) {
  const b = radio(ee => { ee[0x018] = (ee[0x018] & 0x7F) | 0x20; }); boot(b, 2.5);
  const g = keys(b, 'mmm');
  check('dil: EEPROM İngilizce iken de Türkçe (LISTE, FREKANS, APRS)', g.join('|') === 'LISTE   |FREKANS |APRS    ' && (b.ram[0x340] & 0x80), g.join('|'));
  keys(b, 'm');
}

/* kapatılan özellikler (ozellik_kaldir.inc): trunk, veri mesajı / sipariş, ses yayın, servis terminali,
 * '*' test modu, '#' ile seçmeli çağrı araması. Seçenekleri açık bir B tipi telsizde bu fonksiyonların
 * hiçbirine girilmemeli; '#' "C GG___" açmamalı, 0xF8 seri portu 2400'e almamalı */
if (RING.includes('FREQ')) {
  const GIR = { trunk: 0x00C91C, trunk_ui: 0x016767, radio_menu: 0x004C59, veri_tus: 0x014A76, veri_kanal: 0x014AB3,
    veri_seri: 0x014EB2, ses_yayin: 0x01691C, servis: 0x0099BD, test_modu: 0x01CA08 };
  const b = radio(ee => { ee[0x000] |= 0x14; ee[0x014] |= 0x0C; ee[0x58B] |= 0x12;
    for (let n = 1; n <= 16; n++) ee[0x2E + (n - 1) * 13 + 4] |= 0x80;
    ee[0x54B] |= 0x10; let t = 0; for (let a = 0x54B; a <= 0x551; a++) t += ee[a]; ee[0x552] = t >> 8 & 255; ee[0x553] = t & 255; });
  b.hwArac = true; b.inputs.P4 = 0xCF; b.panel.setExt(0xBF, b);
  const hit = new Set(); for (const a of Object.values(GIR)) b.breakpoints.add(a);
  const run = sec => { const end = b.now + FCLK * sec; while (b.now < end) if (b.run(end - b.now) === 'break') { const pc = b.cpu.PG << 16 | b.cpu.PC; hit.add(Object.keys(GIR).find(k => GIR[k] === pc)); b.breakpoints.delete(pc); } };
  run(2.6 + (b.rom[0xE0E1] === 0x5C ? 5.5 : 0));
  const tus = k => { const i = KP.indexOf(k); b.keypad.press(i, true, b); run(0.15); b.keypad.press(i, false, b); run(0.6); return lcd(b); };
  const halka = []; for (let k = 0; k < 8; k++) halka.push(tus('m'));
  for (let k = 0; k < 10 && !/^KANAL/.test(lcd(b)); k++) tus('m');
  const diyez = tus('#'), yildiz = tus('*');
  b.serialIn([0xF8]); run(1);
  check('kapatılan özellikler: trunk / veri / ses yayın / servis / test koduna girilmiyor, RADIO ve YAZICI menüde yok',
    hit.size === 0 && !(b.ram[0x246] & 2) && !halka.some(t => /RADIO|TELSIZ|PRINTER|YAZICI/.test(t)),
    'giren: ' + ([...hit].join(',') || 'yok') + ' | ' + halka.join('|'));
  check("'#' seçmeli çağrı araması açmıyor, 0xF8 seri portu 2400'e almıyor", !/^C /.test(diyez) && /^KANAL/.test(yildiz) && b.sfr[0x31] === 25,
    '# -> "' + diyez + '", * -> "' + yildiz + '", U0BRG ' + b.sfr[0x31]);
}

/* PAROLA, servis kodu ve RSSI göstergesi (parola_rssi_kaldir.inc): ALT MENU'de PAROLA / MSR RSSI yok (A ve
 * B tipi kart); EEPROM'da kayıtlı parola açılışta sorulmaz ve silinir; kalibre edilmiş RSSI (EE 0x54B bit 6 +
 * 0x571 tablosu) sinyalde "-115dBm" göstermez */
if (RING.includes('FREQ')) {
  const us = ee => { let t = 0; for (let a = 0x54B; a <= 0x551; a++) t += ee[a]; ee[0x552] = t >> 8 & 255; ee[0x553] = t & 255; };
  const tus = (b, k) => { const i = KP.indexOf(k); b.keypad.press(i, true, b); b.run(FCLK * 0.15); b.keypad.press(i, false, b); b.run(FCLK * 0.6); return lcd(b); };
  for (const arac of [false, true]) {
    const b = radio(ee => { ee[0x018] |= 0x20; ee[0x014] |= 0x08; if (arac) ee[0x58B] |= 0x12; ee.fill(0xFF, 0x54D, 0x552); us(ee); });
    if (arac) { b.hwArac = true; b.inputs.P4 = 0xCF; b.panel.setExt(0xBF, b); }
    boot(b, 3);
    const alt = [];
    for (let k = 0; k < 10 && !/ALT MENU/.test(lcd(b)); k++) tus(b, 'm');
    alt.push(tus(b, 's')); for (let k = 0; k < 4; k++) alt.push(tus(b, 'm'));
    check('ALT MENU: PAROLA ve MSR RSSI yok (' + (arac ? 'B' : 'A') + ' tipi)',
      alt.join('|') === (arac ? 'ISIK    |UYARI   |ALT MENU|KANAL 01|KNL TARA' : 'ISIK    |UYARI   |GOSTERGE|ALT MENU|KANAL 01'), alt.join('|'));
  }
  { const b = radio(ee => { ee.set([1, 2, 3, 4, 5], 0x54D); us(ee); }); boot(b, 3); b.run(FCLK);
    check('kayıtlı parola açılışta sorulmuyor ve siliniyor', /^KANAL/.test(lcd(b)) && b.eeprom.mem.slice(0x54D, 0x552).every(x => x === 0xFF),
      lcd(b) + ', EE 0x54D: ' + Array.from(b.eeprom.mem.slice(0x54D, 0x552)).map(x => x.toString(16)).join(' ')); }
  { const b = radio(ee => { ee[0x54B] |= 0x40; ee.fill(0xFF, 0x54D, 0x552); ee.set([0xC0, 0xA0, 0x80, 0x60, 0x40, 0x02, 0x80], 0x571); us(ee); });
    boot(b, 3); b.inputs.P5 |= 0x80; b.analog[5] = 0x50; b.run(FCLK * 2);
    check('kalibre edilmiş RSSI göstergesi açılmıyor', /^KANAL/.test(lcd(b)) && b.ram[0x1EB] >= 3, lcd(b) + ', 0x01EB = ' + b.ram[0x1EB]); }
}

/* kanal kayıt (kanal_kayit.inc): VFO kurulu iken q ~1 s -> "KAYIT __", haneler, s -> EEPROM'daki kanal
 * kaydı P4000'in yazacağıyla bayt bayt aynı, sağlamalar doğru; kanala geçince VFO frekansı ve adı geliyor;
 * başka tuş / zaman aşımı iptal, VFO yokken "VFO YOK", kısa q hâlâ monitör */
if (RING.includes('FREQ')) {
  const P = globalThis.P4000;
  const hex = (e, a, n) => Array.from(e.slice(a, a + n), x => x.toString(16).padStart(2, '0')).join(' ');
  const kur = arac => {
    const b = radio(ee => { ee[0x014] |= 0x08; if (arac) ee[0x58B] |= 0x12; });
    if (arac) { b.hwArac = true; b.inputs.P4 = 0xCF; b.panel.setExt(0xBF, b); }
    boot(b, 3);
    return b;
  };
  const tus = (b, k, s) => { const i = KP.indexOf(k); b.keypad.press(i, true, b); b.run(FCLK * (s || 0.15)); b.keypad.press(i, false, b); b.run(FCLK * 0.6); return lcd(b); };
  const vfo = (b, rx, tx, rt, tt) => {
    for (let k = 0; k < 10 && !/^FREKANS/.test(lcd(b)); k++) tus(b, 'm');
    for (const k of 's' + rx + '#' + tx + '#' + rt + '#' + tt + '#') tus(b, k);
    return lcd(b);
  };
  // ekran tamponu ($0252): güç yüksekken yanan simge 3. karakterle karışıyor, simülatör o 'Y'yi çözemiyor
  const tusT = (b, k, s) => { tus(b, k, s); return V.textBuffer(b); };
  const kayit = (b, no) => { const r = [tusT(b, 'q', 1.3)]; for (const k of no) r.push(tusT(b, k)); r.push(tusT(b, 's')); return r; };

  // el kartı, FREKANS ekranı açıkken: 145,6625 / 145,0625, 88,5 / 100,0 Hz, p ile yüksek güç -> kanal 12
  { const b = kur(false), once = new Uint8Array(b.eeprom.mem);
    const ekran = vfo(b, '1456625', '1450625', '885', '1000');
    tus(b, 'p');
    b.run(FCLK * 2);
    const r = kayit(b, '12');
    const bekle = new Uint8Array(once);
    P.setChannel(bekle, 12, { name: '145 6625', rx: 145662500, tx: 145062500, rxTcs: 8, txTcs: 12, hprf: 1, tot: 0, ani: 0 }, { xs: true });
    P.fixSums(bekle);
    const e = b.eeprom.mem, a = 0x2E + 13 * 11;
    check('kanal kayıt: "KAYIT __" -> "KAYIT 12" -> "TAMAM 12"', ekran === '145 6625' && r.join('|') === 'KAYIT __|KAYIT 1_|KAYIT 12|TAMAM 12', ekran + ' / ' + r.join('|'));
    check('kanal kayıt: kayıt P4000 kodlamasıyla aynı (kristal kaydırma 1, HPRF, tonlar, ad)', hex(e, a, 13) === hex(bekle, a, 13) && P.channel(e, 12).xshift === 1,
      hex(e, a, 13) + ' / ' + hex(bekle, a, 13));
    let fark = []; for (let i = 0; i < 2048; i++) if (e[i] !== bekle[i]) fark.push(i.toString(16));
    check('kanal kayıt: yalnız kanal 12 ve AKTARMA sağlaması değişti, bütün sağlamalar doğru', !fark.length && P.checkSums(e).every(s => s.ok),
      'fark: ' + (fark.join(' ') || 'yok'));
    b.run(FCLK * 2);
    const sonra = lcd(b), mon = tus(b, 'q');
    check('kanal kayıt: mesajdan sonra VFO ekranı, kısa q monitör', sonra === '145 6625' && mon === 'MONITOR*', sonra + ' / ' + mon);
    b.panel.setExt(((16 - 12) & 15) << 4 | 0x0F, b); b.run(FCLK * 1.5);
    const st = b.radioState();
    check('kanal kayıt: kanal 12 seçilince ad ve alma frekansı', lcd(b) === '145 6625' && (st.vco / 1e6).toFixed(4) === '190.6625', lcd(b) + ' VCO ' + (st.vco / 1e6).toFixed(4));
    const c = new Board(rom); c.eeprom.mem.set(e); c.font = b.font; boot(c, 3);
    check('kanal kayıt: kaydedilmiş EEPROM ile açılış', /^KANAL 01$/.test(lcd(c)), lcd(c));
  }
  // araç kartı, normal ekran (FREKANS'tan m ile çıkıldı): tek hane "7" -> "TAMAM 07"; kanal 07'ye tuşlarla
  { const b = kur(true);
    vfo(b, '1468000', '', '0', '');
    tus(b, 'm');
    const n = lcd(b), r = kayit(b, '7');
    b.run(FCLK * 2);
    const c7 = P.channel(b.eeprom.mem, 7);
    tus(b, '0'); tus(b, '7'); b.run(FCLK * 1.5);
    check('kanal kayıt (araç kartı, normal ekran): tek hane, simplex, tonsuz',
      n === '146 8000' && r.join('|') === 'KAYIT __|KAYIT 7_|TAMAM 07' && c7.rx === 146800000 && c7.tx === 146800000 && !c7.rxTcs && !c7.txTcs &&
      c7.name === '146 8000' && lcd(b) === '146 8000' && P.checkSums(b.eeprom.mem).every(s => s.ok), n + ' / ' + r.join('|') + ' / ' + JSON.stringify(c7) + ' / ' + lcd(b));
  }
  // iptal, zaman aşımı, 00, VFO yok
  { const b = kur(false);
    const yok = tus(b, 'q', 1.3);
    vfo(b, '1455000', '', '', '');
    const once = new Uint8Array(b.eeprom.mem);
    const iptal = [tus(b, 'q', 1.3), tus(b, '3'), tus(b, '*')];
    b.run(FCLK * 2);
    const za = [tus(b, 'q', 1.3)]; b.run(FCLK * 8.5); za.push(lcd(b));
    b.run(FCLK * 2);
    const sifir = [tus(b, 'q', 1.3), tus(b, '0'), tus(b, '0'), tus(b, 's'), tus(b, 'm')];
    let degisti = false; for (let i = 0; i < 2048; i++) if (b.eeprom.mem[i] !== once[i]) degisti = true;
    check('kanal kayıt: VFO yokken "VFO YOK"; başka tuş, zaman aşımı ve "00" yazmıyor',
      yok === 'VFO YOK ' && iptal.join('|') === 'KAYIT __|KAYIT 3_|IPTAL   ' && za.join('|') === 'KAYIT __|IPTAL   ' &&
      sifir.join('|') === 'KAYIT __|KAYIT 0_|KAYIT 00|KAYIT 00|IPTAL   ' && !degisti,
      yok + ' / ' + iptal.join('|') + ' / ' + za.join('|') + ' / ' + sifir.join('|') + (degisti ? ' / EEPROM değişti' : ''));
  }
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
  if (RING.includes('FREQ')) { const g = keys(b, 'mm' + 'm'.repeat(RING.indexOf('FREQ')) + 's156525####'); check('yüklemeden sonra FREKANS çalışıyor', g[g.length - 1] === '156 5250' && (b.radioState().vco / 1e6).toFixed(4) === '201.5250', g.join('|')); }
  if (RING.includes('GAME')) { if (RING.includes('FREQ')) keys(b, 'm'); const g = keys(b, 'mmmms123#'); check('yüklemeden sonra OYUN çalışıyor', g[4] === '___     ' && /^123 [v^]  1$|^WIN    1$/.test(g[8]), g.join('|')); }
  if (RING.includes('CALC')) { if (RING.includes('GAME') || RING.includes('FREQ')) keys(b, 'm'); scenario(b, 'yüklemeden sonra HESAP çalışıyor', 'mmms 7 * * * 6 #', 'LIST|NUMBER|CALC|       0|       7|+      7|-      7|*      7|*      6|      42'); }
}

out(fails ? fails + ' test başarısız' : 'tüm özel firmware testleri geçti');
if (isNode && fails) process.exit(1);
if (!isNode && fails) throw new Error(fails + ' test başarısız');   // jsc: yakalanmamış istisna -> çıkış kodu 3 (quit(n) kodu iletmiyor)
