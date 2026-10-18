# sim — ASELSAN 4000 web simülatörü

Orijinal makine kodunu (ya da yeni derlenen bir HEX'i) tarayıcıda, K3 kontrol kartının modeli üzerinde çalıştırır. Gerçek cihaza yüklemeden yazılımı denemek için kullanılır.

**Açmak için:** `index.html`'i tarayıcıda açın. Sunucu gerekmez; `file://` ile de çalışır. Firmware'ler `firmware.js` içinde gömülü (`make sim` yeniden üretir). Başka bir HEX dosyası için **HEX yükle…** düğmesini kullanın; S-record ve Intel HEX desteklenir.

## Neler var

| Bölüm | İçerik |
|---|---|
| Ön panel | LCD (PCF8576 belleğinden çözülen 8 karakter + 2 haneli kanal + 16 simge biti), 16 konumlu kanal anahtarı, PTT (boşluk tuşu), PTT kilitleme, Acil (P45), P44, panel düğmeleri (0x46 bit 0–3), 4×4 tuş takımı (0x42) |
| RF ve ses | MB1502'ye giden R/N/A kelimelerinden VCO ve RF frekansı, kilit durumu ve kilit hatası, TX'te TKS / ton / DTMF / Select-5 analizi (hoparlörden dinlenebilir). RX girişleri: taşıyıcı (P57), RSSI (AN5), TKS tonu (P65), Select-5 ton dizisi (P66) |
| Seri port | UART0 terminali (hex/ASCII gönder-al); UART1 C-BUS günlüğü (FX806/FX809/FX802), yanıt baytı, INT2 isteği |
| EEPROM / kanallar | 24C16 içeriği (tarayıcıda saklanır, .bin indir/yükle), sağlama toplamı kontrolü ve düzeltme, bant tabanı, seçenekler, kanal tablosu (ad, RX/TX frekansı, TKS, seçmeli çağrı), hex düzenleyici |
| Analog ve pinler | AN3–AN7 kaydırıcıları (AN7/AN6 TX'te güç PWM'inden otomatik), port pinlerinin tek tek değiştirilmesi, zamanlayıcı ve kesme durumu |
| Hata ayıklama | Register'lar, disassembly (tıkla = kesme noktası), adım adım çalıştırma, RAM yazma izleme, bellek görünümü, olay ve I²C günlüğü |

## Dosyalar

| Dosya | İçerik |
|---|---|
| `m7700.js` | MELPS 7700 CPU çekirdeği (opcode tabloları `tools/d77.py` ile aynı) ve disassembler |
| `board.js` | Kart modeli: portlar, Timer A0–A4 / B0–B2 (zamanlayıcı, tek darbe, PWM, darbe ölçme, olay sayacı), kesme önceliği, UART0/1, A-D, WDT, yazılım reseti, 29F010 flash, I²C (24C16, PCF8576, 2×PCF8574), MB1502, giriş sinyal üreteçleri. Ayrıca EEPROM düzeni ve varsayılan görüntü |
| `radio_view.js` | LCD çözümü (ROM'daki yazı tiplerinden ters arama), TX ses analizi (Goertzel / sıfır geçiş), ton tabloları |
| `app.js`, `index.html` | Arayüz |
| `yukle.html`, `seri_istemci.js` | Gerçek telsizle yedekleme ve firmware yükleme sayfası (Web Serial, Chrome / Edge): bağlantı, sürüm, EEPROM ve flash yedeği, firmware tanıma; yükleme yalnızca yedekler alındıktan sonra açılır; EEPROM yedeğini geri yükleme (yalnız farklı sayfalar, doğrulamalı). Simülatör moduyla donanımsız denenir (`yukle.html#otomatik-test`, `#otomatik-test-eeprom` akışı kendisi yürütür) |
| `yukle_test.js` | Seri istemcinin simülatöre karşı testi (`make test-yukle`): okunan her bayt, okumada telsize yazılmadığı, yükleme + yeni firmware'in açılması, geri yükleme, yarıda kesilip tamamlama, kayıplı hatta otomatik yeniden deneme, tehlikeli dosyaların reddi |
| `selftest.js` | Orijinal firmware ile regresyon testi (`make test-sim`) |
| `test_ozel.js` | Özel firmware örneklerinin testi (`make test-ozel`): tuş senaryoları, orijinalle karşılaştırma, seri yükleyiciyle yazma |
| `ram_kullanim.js` | Firmware'in yazdığı RAM adreslerini ölçer (örnekler için boş RAM aramak) |
| `run_node.js`, `run.sh` | Komut satırından başsız çalıştırma ve iz alma (`./run.sh ../../YA4000S.HEX 4 "2.5 p4 7f" TRACE=1`) |
| `mkfirmware.py`, `mkee.js` | `firmware.js` ve varsayılan EEPROM görüntüsü üretimi |

Node yoksa testler macOS'teki JavaScriptCore (`jsc`) ile çalışır; Makefile bunu kendisi seçer.

## Doğrulanan davranışlar (`make test-sim`)

| Test | Sonuç |
|---|---|
| Açılış: "AS V2-3" → "PN" → "SN" → "CN" → "CH 01" | ✓ |
| RX: kanal 1 (156,000 MHz) için VCO = 201,000 MHz (+45 MHz ara frekans) | ✓ |
| Kanal anahtarı 3 → "CH 03", VCO 201,050 MHz | ✓ |
| PTT (P47 = 0) → TX VCO 156,050 MHz, P55 = 0, durum 8'de kararlı | ✓ |
| Güç döngüsü TA3 PWM'ini AN7 hedefine (0xB0) oturtuyor | ✓ |
| TX TKS ton 9 → 91,5 Hz | ✓ |
| PTT'de Select-5 kimliği (CCIR "00000" → 0E0E0) | ✓ |
| TX sırasında tuş takımından DTMF "5" | ✓ |
| Select-5 alma: P66'ya CCIR "12345" → firmware tamponunda 12345 | ✓ |
| Boş EEPROM → "ARZ PRG" ve güç kapanması | ✓ |

## Simülatörle ortaya çıkan donanım/yazılım bilgileri

Bunlar orijinal kodun simülatörde davranışından çıkarıldı (önceki dokümandaki bazı varsayımları düzeltir):

- **P67 sentezör kilidi aktif düşük: 0 = kilitli.** F_00D51D, F_00D3B4, F_00EE14 ve durum makinesi yalnızca bu şekilde tutarlı. Aktif yüksek varsayımıyla firmware TX'i her 32 ms'de yeniden başlatıyor ve sentezörü sürekli yeniden yüklüyor.
- **EEPROM'da hazır MB1502 kelimesi yok; frekansı firmware hesaplıyor.** Formül: f = (taban·10 + kelime) × 2,5 kHz. Taban EE[0x58C..0x58D], 25 kHz birim. RX VCO = f ± 45 MHz: taban < 250 MHz ise +45 MHz ve ön bölücü 64/65, değilse −45 MHz ve 128/129. Adım 12,5 / 10 / 5 kHz (kelime 5'e, 4'e ya da hiçbirine bölünürse). Referans 12,8 MHz, EE[0x58F] ≠ 0 ise 13,0 MHz.
- **Kanal kaydı 0x02E + 13·(n−1)'de:**
  - +0..1: TX kelimesi.
  - +2..3: RX kelimesi.
  - +4: RX TKS, kristal kaydırma ve seçmeli çağrı bayrağı.
  - +5..12: 8 karakter kanal adı (RAM 0x024A). Yalnız A–Z, 0–9, boşluk, `_ - /` kabul ediliyor; başka bir karakter (0x00 dahil) varsa ad gösterilmiyor. TX TKS tonu +5..+11 baytlarının 7. bitlerine gömülü.
- **TKS numaraları ROM sırasında:** 1–38 klasik tonlar, 39–50 sonradan eklenenler (69,3 … 254,1 Hz).
- **EEPROM sağlama toplamları:** 0x000–004, 0x008–015, 0x018–534 ve 0x578–5BC. Her biri 16 bit bayt toplamı, arkasında büyük endian olarak saklanıyor.
- **Telsiz parametreleri EE[0x018..0x02D]'de** (RAM 0x0340..0x0355):
  - 0x018 bit 1..0: Select-5 standardı (1 CCIR, 2 ZVEI1, 3 EEA). Bit 7: dil.
  - 0x01A üst yarım bayt: PTT'de kimlik gönderme.
  - 0x029..0x02C: Select-5 çözücüsünün ortalama, kabul ve tolerans parametreleri.
  - Bu baytlardan biri 0 olursa firmware sıfıra bölüyor ya da kesme fırtınasına giriyor (STP, watchdog).
- **Güç kalibrasyonu:** başlangıç PWM'i EE[0x591..], iki hedef tablosu EE[0x59B..] ve EE[0x5A5..].
- **Susturma (P57):** 1 = susturma açık (kanalda sinyal var). Firmware bunu Timer A3'ün olay sayıcı modunda (TA3IN = P57) kenar bekleyerek ve ardından seviyeyi sayarak izliyor (F_007D35, TA3 kesmesi 0x01BB88) → RAM 0x023E bit 5. Tarama, seçmeli çağrı alma ve hoparlör (P46) buna bağlı.
- **Tuş takımı:** EE[0x014] bit 3 ile açılıyor (varsayılan EEPROM'da açık). Menüler, tarama, tuşla seçmeli çağrı ve panel düğmeleri için bkz. `../tus_takimi.md`. Tuş kodu ROM 0x31EC'deki "*0#q789p456s123m" tablosundan geliyor.
- **P44'ün görevi açılışta algılanan donanım tipine bağlı** (F_00DF55, RAM 0x335 bit 1; EE[0x58B] bit 1 bununla aynı olmalı, yoksa "ERR HW" verip kapanıyor):
  - Tip bit 1 = 0 (simülatördeki kart): P44 = 0 ikinci bir bas-konuş girişi, P47 ile aynı (RAM 0x244 bit 4, P47 bit 5).
  - Tip bit 1 = 1: P44 = 1 olunca TX yasaklanıyor; uyarı bipi (F_0080AA, 6) çalıyor ve PTT RX'te kalıyor (RAM 0x242 bit 0). P45 (acil) bu tipte ters polariteli.
  - Açılışta EE[0x014] bit 4 = 1, tip bit 1 = 0 ve P44 = 0 ise UART0 üzerinden bir seri haberleşme oturumuna giriliyor (F_01A3EF). Muhtemelen bilgisayardan programlama; henüz çözülmedi.

## Varsayımlar ve sınırlamalar

- **Varsayılan EEPROM:** Varsayılan EEPROM'daki telsiz parametreleri, güç kalibrasyonu ve seçenekler firmware'i çalıştıracak tahmini değerler. Gerçek bir telsizin EEPROM dökümü yüklenirse (EEPROM sekmesi → Yükle) onun değerleri kullanılır.
- **Zamanlama:** Komut başına çevrim sayısı yaklaşık (bayt erişimi başına 1 çevrim). Zamanlayıcılar ve kesmeler doğru frekansta çalışıyor ama CPU gerçek işlemciden biraz farklı hızda. Açılıştaki ROM sağlama toplamı sırasında watchdog kesmesi geliyor; firmware bunu bu aşamada yok sayıyor.
- **Analog ve RF:** Analog girişler ve RF tarafı modellenmedi. Taşıyıcı, RSSI, TKS ve Select-5 girişleri elle veriliyor; ses yolu FX806 üzerinden modellenmiyor.
- **C-BUS:** Yalnızca günlük tutuluyor; FX806/FX809/FX802 komutlarının anlamı henüz çözülmedi. Okumalar sabit yanıt baytını döndürüyor.
- **Bilinmeyen anlamlar:** LCD simge bitlerinin ve panel genişleticisinin (0x46) alt 4 bitinin anlamı bilinmiyor; ham olarak gösteriliyor.
