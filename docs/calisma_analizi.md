# ASELSAN 4000 Telsiz Yazılımı — Çalışma Analizi

16 Ekim 2026 · Barış Dinç  
*Kaynak: Claude Docs belgesi "ASELSAN 4000 Telsiz Yazılımı — Çalışma Analizi" (18 Ekim 2026'da Markdown olarak dışa aktarıldı).*

## 1. Genel bakış ve donanım

Yazılım, K3 Kontrol ve SF kartındaki TD1 M37732S4LGP işlemcisinde çalışan, NC77 ile derlenmiş tek bir C programıdır (1995, "Yazanlar: AB, HG, YK"). Telsizin bütün işlevlerini işlemci yönetir: sentezör, RX/TX anahtarlama, güç ve susturma ayarı, ton üretme/çözme, ön panel ve EEPROM'daki ayarlar.

Bu doküman şu kaynakları birleştirir: firmware disassembly'si (`disasm/YA4000S.asm` ve diğerleri, adresler bu dosyalara göredir), 4021/4025 Depo Bakım Onarım kılavuzu (bölüm 3.6–3.7) ve Şekil 6-5 K3 kartı devre şeması.

### Firmware dosyaları

| Dosya | Opsiyon metni | Fark |
| --- | --- | --- |
| YA4000S | STANDART VERSIYON | temel |
| YA4000D | DENIZ EL TELSIZI OPSIYONU | 0x010630 opsiyon fonksiyonu |
| YA4000K | KONVANSIYONEL VERI OPSIYONU | aynı + F\_013280 çağrısı |
| YA4000P | DISPATCH-TELSIZ OPSIYONU | aynı + bayrak 0x0246 bit 8 |
| YA4000R | KIRSAL ROLE TELSIZ OPSIYONU | aynı + bayrak 0x0246 bit 9 |
| YA4000V | VOX DEVREDE TELSIZ OPSIYONU | aynı + bayrak 0x023E bit 9 |
| YA4000Y | KABLOSUZ SES YAYIN OPSIYONU | 0x010630 opsiyon fonksiyonu |
| YA4000E | (yok, "radio" başlıklı) | ayrı derleme, adresler farklı |

Yedi "battery" dosyası aynı kaynak koddan derlenmiş. Aralarındaki tek kod farkı, açılışta opsiyon bayraklarını ayarlayan 0x010630 fonksiyonu.

### K3 kartındaki ana parçalar

| Parça | Görev | İşlemciye bağlantı |
| --- | --- | --- |
| TD1 M37732S4LGP | 16-bit MCU, 2 KB RAM, ROM yok, 8.064 MHz kristal | — |
| TD2 AMD 29F010 | 128 KB program flash (0x02000–0x1FFFF görünür) | 8-bit veri yolu, 17-bit adres, TD19/TD20 kod çözücü |
| EEPROM 2 KB | Kanallar, opsiyonlar, kalibrasyon | I²C: P61 SCL, P62 SDA (yazılımla) |
| TD3 FX806 | Ses işlem: filtre, vurgu, sapma, seviye, ton filtreleri | C-BUS, seçim TDSEC1 (P51) |
| TD4 FX809 | 1200 baud FFSK modem | C-BUS + OKUMA\_İSTEK1 (P64, INT2) |
| TD15 FX802 + TD16 DRAM | Ses kayıt/çalma, analog şifreleme | C-BUS + OKUMA\_İSTEK1 |
| Sentezör (RF kartı) | VCO/PLL | P70 YÜKLE, P81 SEÇ, saat/veri UART1 hattı, kilit P67 |
| TD8 + filtreler | PWM'den DC: güç ayarı (P56), susturma eşiği / TKS tonu (P54) | Timer A çıkışları |
| TD9 / TD12 | Sıfır-geçiş algılayıcılar: TKS (P65) ve seçmeli çağrı (P66) | Timer B girişleri |
| K4 kartı (ön panel) | LCD sürücü + iki I²C port genişletici (tuş, PTT, LED) | I²C + OKUMA\_İSTEK0 (P63, INT1) |

Genel kural: "akıllı" işlerin çoğu işlemcide, yazılımla yapılıyor. CTCSS ve seçmeli çağrı tonları için çözme yongası yok; işlemci kare dalganın periyodunu Timer B ile ölçüyor. Tonlar da PWM ile üretiliyor.

*(Şema: K3 kartı mimarisi · TD1 ve 8 çevre birimi grubu — çizim yalnızca [Claude Docs belgesinde](https://claude.ai/code/artifact/b54074fc-29ad-4bf0-8501-ab50199cd1cd) var.)*

Sol tarafta bellek ve kullanıcı arayüzü, sağ tarafta RF ve ses işlemi var. UART1 hattı sentezör ile C-BUS arasında paylaşılıyor; hangi yonganın dinleyeceğini seçim pinleri belirliyor.

## 2. Bellek, açılış ve ana döngü

Reset'ten sonra yazılım önce donanımı ve EEPROM'daki ayarları hazırlar, sonra hiç çıkmayan bir ana döngüye girer. Zamana bağlı işlerin hepsini kesmeler yapar.

### Bellek haritası

| Adres | İçerik |
| --- | --- |
| 0x0000–0x007F | SFR (P4–P8, zamanlayıcılar, UART, A-D). P0–P3 yok, harici yol olarak kullanılıyor |
| 0x0080–0x0879 | İç RAM (2 KB). Global değişkenler 0x0082–0x04ED; yığın 0x0879'dan aşağı |
| 0x0230–0x0248 | Durum bayrak kelimeleri (0x023A, 0x023C, 0x023E, 0x0240, 0x0242, 0x0244, 0x0246, 0x0248) |
| 0x0334–0x0344 | Opsiyon bayrakları (EEPROM'dan ve 0x010630 opsiyon fonksiyonundan) |
| 0x0356 / 0x0363 / 0x0370 | Kanal kaydının 3 kopyası (13'er bayt); 0x0370 = etkin kanal |
| 0x087A–0x087C | RAM'e yazılan bekleme kodu: `NOP; WIT; RTL` |
| 0x2000–0x203F | Reset kodu ve kesme atlama sapıkları |
| 0x2040–0x3209 | Metinler (TR/EN), ton tabloları, tuş haritası |
| 0x3280–0xFFC7 | Kod, bank 0 |
| 0x10100–0x105C7 | Flash yükleyici; çalışmadan önce RAM 0x0080'e kopyalanır |
| 0x13280–0x1E4F3 | Kod, bank 1; sonunda NC77 kütüphanesi (memcpy, 32-bit çarpma/bölme, switch) |

### Açılış sırası

1. **0x2000 RESET:** Yığın 0x0879'a kurulur, 0xFE60'a atlanır.
2. **0xFE60 ncrt0:** DT=0, .bss (0x0082'den 0x046B bayt) sıfırlanır, `main` çağrılır.
3. **0x01A6D5 main, donanım:** PMOD=0x26, port değerleri ve yönleri yazılır, watchdog ayarlanır.
4. **I²C hazırlık (0x00713E):** Hat boşta konuma alınır.
5. **C-BUS reset (0x007E4C):** TDSEC1 ile seçilen C-BUS yongasına (FX806) ilk komut gönderilir; büyük olasılıkla genel reset.
6. **Sistem başlatma (0x00E305):** RAM bekleme kodu yazılır, kesmeler açılır, TB2 32 ms tick'i başlatılır, bayraklar sıfırlanır.
7. **Ayarlar:** EEPROM'dan telsiz parametreleri, opsiyonlar ve son kanal yüklenir (0x00B98D, 0x0167F2, 0x01A3EF...). 0x01A3EF kontak anahtarını (P44) da okur.
8. **Opsiyona göre ek modüller:** 0x0335 bit 4 ise 0x01691C, 0x0240 bit 8 ise 0x0099AD çağrılır.

### Ana döngü (0x01A776)

Döngü her turda watchdog'u besler. 0x01A664 "yapılacak iş yok" derse RAM'deki `WIT` komutuyla bir sonraki kesmeye kadar uyur; pil tasarrufu böyle yapılıyor. Sonra iki koldan birini çalıştırır:

| Kol | Koşul | Çağrılan fonksiyonlar | Tahmini görev |
| --- | --- | --- | --- |
| Normal | 0x0246 bit 1 = 0 | 0x006817, 0x00F3E8, 0x01DB8B, 0x01914B, 0x0198D5, 0x01DE4E, 0x0134C0 | Tuş/menü, RX-TX durum makinesi, seçmeli çağrı, ölçümler, gösterge |
| Özel | 0x0246 bit 1 = 1 | 0x00C91C, 0x00F3E8, 0x016767, 0x0198D5 | Tarama veya PC/programlama modu (doğrulanmadı) |

- **0x00F3E8:** Telsizin kalbi. Susturmayı (P57) okur; RX/TX geçişlerini, TKS ve seçmeli çağrı gönderimini, sentezör kilit kontrolünü yönetir.
- **0x0198D5:** Pil gerilimi, sıcaklık, RSSI, giden ve yansıyan güç ölçümlerini (AN3–AN7) yapar.
- **0x0134C0:** En büyük fonksiyon (1294 komut). Menü ve gösterge mantığı olduğu tahmin ediliyor.

## 3. Kesmeler ve zamanlama

Sekiz zamanlayıcının yedisi kullanılıyor ve çoğu RX ile TX'te farklı iş yapıyor. Ana saat kaynağı 8.064 MHz kristal. Zamanlayıcı saatleri: f2 = 4.032 MHz, f16 = 504 kHz, f64 = 126 kHz.

| Kesme | Handler | RX'teki görevi | TX'teki görevi | Donanım |
| --- | --- | --- | --- | --- |
| TB2 | 0x01B9E6 | 32 ms sistem tick'i: \~20 yazılım sayacını azaltır, saat sayacı 0x01F1 | aynı | — |
| TB0 | 0x00FB0B | TKS gürültü sayacı (P65 periyot ölçümü) | TKS DDS örnekleme saati → TA2 PWM | P65 / P54 |
| TA4 | 0x00F94C | TKS I/Q korelatörü (4×f örnekleme) | seçmeli çağrı / DTMF ton süresi (ms) | P65 |
| TB1 | 0x01C627 | Seçmeli çağrı tonu periyot ölçümü | Seçmeli çağrı / DTMF DDS örnekleme saati → TA1 PWM | P66 / P52 |
| TA1 | 0x01C6C0 | — | Ton çıkışı PWM (P52 SÇ\_TON) | P52 |
| TA3 | 0x01BB88 | Sentezör kilit zaman aşımı (tek atım) | Güç ayarı PWM (P56) | P56 |
| TA2 | (kesme kapalı) | Susturma eşiği PWM | TKS ton PWM | P54 |
| INT1 | 0x01C8DE | Ön panel istek hattı (tuş/PTT değişti) | aynı | P63 |
| INT2 | 0x01C960 | FX809 modem / FX802 kayıt yongası isteği | aynı | P64 |
| UART0 RX/TX | 0x019469 / 0x0195B2 | PC ve klonlama bağlantısı (RS232) | aynı | P82/P83 |

Kesmeyle çalışmayan iki seri hat var. UART1 C-BUS ve sentezör için senkron modda, yoklamayla kullanılıyor. I²C ise tamamen yazılımla sürülüyor.

Kesme öncelikleri: TB0 = 5, TA4 = 6, TB1 = 6, TB2 = 2. Ton işleme, tick'i kesebilecek kadar yüksek öncelikli. Handler'lar `CLP #$39` + `PSH` ile başlayıp `CLI` ile iç içe kesmeye izin veriyor.

**32 ms tick hesabı:** TB2MOD=0x80 f64'ü seçer, TB2 de 0x0FBF'ye ayarlı. 8.064 MHz / 64 / 4032 = 31.25 Hz. Ton süreleri de aynı f64 saatiyle sayılıyor (TA4 = ms × 126), bu da hesabı doğruluyor.

## 4. Frekans ayarı ve RX/TX geçişi

**Düzeltme (simülatörle doğrulandı):** EEPROM'da hazır MB1502 kelimesi yok; frekansı yazılım kendisi hesaplıyor. Her kanal için 14 bitlik bir frekans kelimesi saklanıyor. Bant tabanı EEPROM 0x58C–0x58D'de, 25 kHz birimle duruyor. Formül: f = (taban × 10 + kelime) × 2,5 kHz. Alışta VCO = f + 45 MHz (taban 250 MHz'in altındaysa, ön bölücü 64/65) ya da f − 45 MHz (ön bölücü 128/129); gönderişte VCO = f. Adım, kelime 5'e bölünüyorsa 12,5 kHz, 4'e bölünüyorsa 10 kHz, ikisi de değilse 5 kHz. R ve N/A kelimelerini 0x0088B8, 0x0087DE, 0x008825 ve 0x008854 hesaplıyor; baytlar ROM 0x2B06 tablosuyla bit sırası ters çevrilerek gönderiliyor. Referans 12,8 MHz; EEPROM 0x58F sıfır değilse 13,0 MHz.

### Sentezör: TD201 Fujitsu MB1502

Referans 12.8 MHz SDKO (sıcaklık düzeltmeli kristal osilatör). Prescaler MB1502'nin içinde. VCO frekansı MB1502 formülüyle bulunur:

```latex
f_{VCO} = \frac{(P \cdot N + A) \cdot 12.8\,\text{MHz}}{R}
```

Burada P prescaler oranı (64 veya 128), N 11-bit bir bölücü, A 7-bit swallow sayacı, R 14-bit referans bölücüdür. RX'te VCO, alma frekansı ile ara frekans farkında çalışır. Firmware ara frekansı 45 MHz olarak kullanıyor (RAM 0x01F9 = ±18000 × 2,5 kHz).

### Kanal kaydı (EEPROM 0x02E + 13·(n−1), 13 bayt)

| Bayt | İçerik |
| --- | --- |
| +0..+1 | TX kelimesi (14 bit; +0'ın 7. ve 6. bitleri RAM 0x037C'ye bayrak olarak gider) |
| +2..+3 | RX kelimesi (14 bit). Alt 14 biti 0x3FFF ise kanal boştur (0x0087A7) |
| +4 | bit 0–5 RX TKS ton no, bit 6 kristal kaydırma, bit 7 seçmeli çağrı (RAM 0x0375) |
| +5..+11 | 7 karakter kanal adı (7 bit ASCII, açılışta "CN" ekranında). +5'in 7. biti TX tonunu açar, +6..+11'in 7. bitleri TX TKS ton numarasıdır (RAM 0x037C) |
| +12 | çözülmedi |

RAM 0x0370–0x037C kanal kaydının kopyası değil, hesaplanmış sentezör kelimelerini tutuyor (0x0370–0x0374 RX, 0x0377–0x037B TX).

### Yükleme (0x00D45C)

1. Kesmeler kapatılır. UART1 senkron moda alınır (BRG=3); P81 SENT\_SEÇ ve P70 SENT\_YÜKLE düşürülür.
2. R kelimesi gönderilir: 2 bayt (0x0216, 0x0217). P70'e bir darbe verilerek MB1502'ye mandallanır.
3. N/A kelimesi gönderilir: 3 bayt (0x0213, 0x0212, 0x0208), yine P70 darbesiyle mandallanır.
4. P81 bırakılır, UART1 kapatılır, kesmeler açılır.

Saat ve veri hattı (P85/P87) C-BUS ile ortak. Hangi yonganın dinleyeceğini seçim hatları belirliyor: P81 sentezör, P51/P53 C-BUS yongaları.

### Kanal seçimi ve kilit

- **0x00D2A3 hazırlık:** Hesaplanmış RX veya TX sentezör kelimelerini (RAM 0x0370 / 0x0377) 0x0208/0x0212/0x0216'ya kopyalar. RX'te 0x0375 bit 6'ya göre P43'ü sürer; kristal 200–250 ppm kayar, amaç işlemci saat harmoniğinin alıcıyı bozmasını önlemek. TX'te ek olarak 0x00CE7E'yi çağırır; bu fonksiyon TX tonu ve C-BUS ayarlarını yapıyor.
- **0x00D51D yükle ve bekle:** Sentezörü yükler, sonra 0x00D3B4 ile P67 kilit sinyalini bekler. P67 aktif düşük: 0 = kilitli (simülatörde doğrulandı; aktif yüksek varsayılınca firmware TX'i her 32 ms'de yeniden başlatıyor). 0x00D3B4 P67 = 1 görürse 0, süre boyunca kilit kalırsa 1 döner; 1 "kilit doğrulandı" demek. Zaman aşımını TA3 tek atım modunda sayıyor.
- **Kilit gelmezse:** "Sentezör Kilitlenmedi" uyarısı verilir (600 Hz, 64 ms ses / 64 ms sus) ve gönderme engellenir.
- **Döngü süzgeci:** Kılavuza göre önce geniş bantta kilitlenir, sonra dar banda geçer (TD203).

### RX ↔ TX geçişi

| Adım | TX'e geçiş (0x00EA11) | RX'e dönüş (0x00ED0F / 0x00EE8B) |
| --- | --- | --- |
| 1 | Ses yolunu kes (P72), alma beslemesini kapat (P55 ALMA\_SEÇ) | TKS üretecini durdur (0x00F8AE) |
| 2 | TX sentezör kelimeleriyle yükle + kilit bekle | RX kelimeleriyle yükle + kilit bekle |
| 3 | Kristal kaydırma ve C-BUS ayarı (FX806 TX modu) | FX806 RX modu, susturma eşiği |
| 4 | Güç PWM'ini başlat (TA3 → P56), TKS üretecini başlat (0x00F854) | Alma beslemesini aç, hoparlör susturması |

Gönderme süresi sınırı (TOT) ve meşgul kanal kilidi bu geçişlerden önce 0x00F3E8 durum makinesinde kontrol ediliyor. Tablodaki adımlar fonksiyonların kullandığı pinlerden ve çağrılardan çıkarıldı; tam sırası kod satır satır okunarak doğrulanmalı.

### Güç ayarı (kapalı çevrim)

TX'te TA3 8-bit PWM moduna (TA3MOD=0x27) geçer ve P56 üzerinden 400 Hz süzgece giden bir ayar gerilimi üretir. 0x00CFC4 giden güç ölçümünü (AN7) okuyup PWM'i ayarlar. 0x00D1C8 sıcaklık (AN3) ve yansıyan güç (AN6) korumasını ekler.

Hedef değer EEPROM'daki kalibrasyondan hesaplanır. Bant altı için kayıtlı bir değer ve frekansla eğim kullanılır; 10 güç seviyesi vardır:

```latex
P_{hedef} = P_{alt} + (f - f_{alt}) \cdot eğim
```

## 5. Hafıza: EEPROM ve kanallar

Kalıcı bilgilerin hepsi 2 KB'lık I²C EEPROM'da duruyor: 99 kanal, opsiyonlar, kimlik numaraları, tarama listesi ve kalibrasyon. Bu bilgiler PC programlama yazılımı ya da telsizden telsize aktarma ile yazılıyor.

### Erişim katmanı

| Katman | Fonksiyon | İş |
| --- | --- | --- |
| Bit düzeyi | 0x00715B / 0x007172 | I²C stop / start. SCL = P61, SDA = P62; SDA yön register'ıyla açık-akaç (open drain) olarak sürülür |
| Bayt düzeyi | 0x006A68 (yaz + ACK), 0x006EB3 (oku) | Tek bayt gönderme/alma |
| EEPROM | 0x007189 | `eeprom_oku_bayt(adres)` |
| EEPROM | 0x0071F0 | `eeprom_oku_kelime(adres)` |
| Genel I²C | 0x0072EE, 0x0073C6 | Cihaz + adres + tampon + uzunluk ile blok okuma/yazma (EEPROM 0xA0, ön panel 0x42 ve 0x46) |

EEPROM 24C16 tipi adresleniyor. Cihaz baytı `0xA0 | (adres >> 8) << 1`, ardından adresin düşük baytı gidiyor; toplam 11 bit. Kod 137 yerden EEPROM erişim fonksiyonlarını çağırıyor.

### EEPROM haritası (bilinen kısım)

| Adres | İçerik | Kesinlik |
| --- | --- | --- |
| 0x000–0x02F | Genel telsiz parametreleri, opsiyon bitleri | adresler kodda var, alanlar tek tek çözülmedi |
| 0x02E–0x534 | Kanallar × 13 bayt. Kanal n'nin adresi = 0x02E + (n−1) × 13 (0x0087A7 boşluk kontrolü için +2'deki RX kelimesini okur) | kesin (0x0087A7) |
| 0x538–0x7FF | Tarama listesi, seçmeli çağrı/ANI numaraları, güç kalibrasyonu, iletişim ayarları | kod 0x055D–0x06D2 arasını okuyor, alanlar açık |

Kanal kaydının formatı Bölüm 4'teki tabloda. Kayıt RAM'e olduğu gibi kopyalanmıyor: 0x008A47 ve 0x00899E kaydı okuyup sentezör kelimelerini hesaplıyor. Açılışta 0x00DCFD dört bloğun sağlama toplamını kontrol eder (0x000–004, 0x008–015, 0x018–534, 0x578–5BC; 16 bit bayt toplamı, arkasında büyük endian). Tutmazsa "ARZ PRG" gösterilip güç kapatılır. 0x018–0x02D telsiz parametreleri (bit 1–0 seçmeli çağrı standardı, bit 7 dil), 0x58B'den başlayan 50 bayt bant ve güç kalibrasyonu.

### Kanal değiştirme

1. Kullanıcı kanalı döner anahtar ya da tuş takımıyla seçer.
2. Kanal boş mu diye bakılır (0x0087A7).
3. 13 baytlık kayıt EEPROM'dan RAM 0x0370'e okunur; önceki kanal 0x0363 ve 0x0356 kopyalarında tutulur (0x00350F, NC77 `memcpy` ile).
4. Kanal kaydındaki TKS ve kristal kaydırma bitleri uygulanır.
5. RX sentezör kelimeleri yüklenip kilit beklenir (Bölüm 4).

### Tarama

Kılavuza göre tarama listesi 99 kanala kadar çıkabiliyor ve iki aşamalı öncelikli kanal desteği var. "Tarama gecikmesi" parametresi, aktivite bitince taramaya ne kadar sonra dönüleceğini belirliyor. Tarama döngüsünün kodu henüz ayrıntılı incelenmedi; ana döngünün özel kolu (0x00C91C) en güçlü aday.

## 6. Kullanıcı arayüzü: gösterge ve tuşlar

Ön panel (K4 kartı) işlemciye yalnızca I²C ve bir istek hattıyla bağlı. Tuş, PTT veya aç-kapa değişince K4 kartı OKUMA\_İSTEK0 hattını (P63, INT1) düşürür, işlemci de ilgili yongayı I²C'den okur.

| I²C adresi | K4 parçası | Yazılımdaki kullanımı |
| --- | --- | --- |
| 0x70 | TD1 LCD sürücü (PCF8576 tipi komutlar: 0xC8 mod, 0xE4 cihaz seçimi, 0xF0 yanıp sönme) | Segment verisi yazılır |
| 0x42 / 0x43 | TD2 port genişletici (yazma / okuma) | Tuş takımı tarama ve okuma |
| 0x46 / 0x47 | TD3 port genişletici | Aç-kapa, bas-konuş, izleme girdileri; RX/TX/tuş LED'leri ve LCD ışığı |

TD2 ve TD3'ün hangisinin 0x42, hangisinin 0x46 olduğu kesin değil; tabloda kodun kullanımına göre eşleştirildi.

### I²C bayt yazma

0x006AC2 sekiz biti döngüsüz (açılmış) olarak gönderiyor. Her bit için SDA yön register'ı ayarlanıyor, sonra SCL darbesi veriliyor; ACK de kontrol ediliyor. I²C fonksiyonları toplam 42 yerden çağrılıyor.

### Tuş haritası ve menüler

ROM'da 0x31EC'de 16 tuşluk bir harita var: `*0#q 789p 456s 123m` (4 × 4 matris). Harfler özel tuşları temsil ediyor; `m` menü, `s` seç olarak tahmin ediliyor, `p` ve `q` kripto ile monitör/reset olabilir.

Menü ve uyarı metinleri 0x2088–0x3209 arasında. Her metin 8 karakterlik bir LCD alanına göre, İngilizce ve Türkçe iki paralel tablo halinde:

| İngilizce | Türkçe |
| --- | --- |
| CHANNEL / SCANLIST / SCAN | KANAL / TARALISTE / TARAMA |
| CLONING / RADIO / FLASH / EEPROM | AKTARMA / TELSIZ / FLASH / EEPROM |
| BRD\_CAST / ALL CALL / ALERT | SESYAYIN / GN CAGRI / UYARI |
| CALLING / RINGING / QUEUED / BUSY | ARIYOR / CALIYOR / SIRADA / MESGUL |
| RSSI e80dBm … e120dBm | RSSI OLC |
| PASSWORD / LIGHT / DISPLAY INVERT | PAROLA / ISIK / GOSTERGE TERS |

Dil seçimi (TR/EN) bir EEPROM parametresi. CALLING, QUEUED, PBX, FLEET gibi metinler, telsizin bir trunk veya çağrı sistemiyle (MPT tipi) çalışma desteği olduğunu düşündürüyor; bu kısım henüz incelenmedi.

### Uyarı sesleri

Uyarı tonları hoparlör hattına P50 UYARI üzerinden veriliyor. Kılavuzdaki tabloya göre her uyarının kendi frekansı ve süresi var: acil çağrı 2000 Hz, bireysel çağrı 1625 Hz, sentezör kilitlenmedi 600 Hz vb. Çoğu 64 ms ses / 64 ms sus kalıbında; bu da 32 ms tick'in iki katı.

## 7. TKS (CTCSS) ton üretme ve çözme

Telsiz 50 standart EIA CTCSS tonunu (67.0–254.1 Hz) destekliyor ve ikisini de tamamen yazılımla yapıyor. Göndermede DDS ile sinüs üretiliyor, almada bir I/Q korelatörü tonu arayan kanaldaki enerjiyi ölçüyor. Ton numarası kanal kaydında: RX için 0x0375, TX için 0x037C'nin alt 6 biti (1–50; 0 = TKS kapalı).

### ROM tabloları

| Adres | Boyut | İçerik |
| --- | --- | --- |
| 0x21EF | 256 bayt | Sinüs tablosu, 98–152 aralığında (8-bit PWM için küçük genlik) |
| 0x22EF | 50 kelime | TX örnekleme bölücüsü: TB0 yeniden yükleme değeri |
| 0x2353 | 50 bayt | TX faz artımı (12–19) |
| 0x2385 | 50 kelime | RX örnekleme bölücüsü: TA4 = 4 × ton frekansı |

Ton sırası EIA tablosuyla aynı. 1–38 standart tonlar (67.0, 71.9, 74.4 … 250.3 Hz), 39–50 genişletilmiş tonlar (69.3, 159.8, 165.5, 171.3, 177.3, 183.5, 189.9, 196.6, 199.5, 206.5, 229.1, 254.1 Hz). Tablodan hesaplanan frekansların hepsi standart değerden en fazla 0.2 Hz (%0.1) sapıyor.

### Gönderme (0x00F854 + TB0 kesmesi)

1. Ton numarasına göre faz artımı (0x2353) ve TB0 bölücüsü (0x22EF) okunur.
2. TA2 8-bit PWM moduna alınır (TA2MOD=0x27). Çıkışı P54 TKS\_ÜRET; TD8 alçak geçiren süzgeci bunu modulasyon hattına verir.
3. TB0 zamanlayıcı modunda, her kesmede: `faz += artım`, `TA2_H = sinüs[faz]`.

Ton frekansı şöyle çıkıyor:

```latex
f_{ton} = \frac{4.032\,\text{MHz}}{TB0 + 1} \cdot \frac{artım}{256}
```

Örneğin ton 1: 4.032 MHz / 2822 × 12/256 = 66.97 Hz. 0x023A bit 14 açıkken faz azaltılarak ilerletiliyor. Bu muhtemelen gönderme sonunda 180° faz ters çevirme (reverse burst / squelch tail elimination); bunu gerçek telsizde doğrulamak gerekiyor.

### Alma (0x00F8BD + TA4 ve TB0 kesmeleri)

RF kartındaki ses, TD9 çevresindeki 250 Hz süzgeçten ve sıfır-geçiş algılayıcıdan geçip P65'e kare dalga olarak geliyor. Yazılım bu tek bitlik sinyali kullanıyor.

1. **TA4 korelatör (0x00F94C):** TA4, seçili tonun 4 katı hızda kesme üretir. Her kesmede P65 okunur ve örnekler sırayla I (0x01AF) ve Q (0x01B0) toplayıcılarına +1/−1 referans ile eklenir. Bu, sinüs ve kosinüs ile çarpmanın 1-bitlik karşılığı.
2. **Karar (her 80 örnekte bir, yani 20 ton periyodu):** Genlik |I| ve |Q|'dan "büyük + küçük/4" yaklaşımıyla hesaplanır. Eşik 0x01AE; ilk değer 40, ton bulununca 30'a iner (histerezis).
3. **Gürültü koruması (TB0, 0x00FB0B):** TB0 aynı anda P65'in periyotlarını ölçer (f16). 600 Hz'den hızlı geçişler 0x01B4'te sayılır. 120 Hz'den yavaş periyotlar bu sayacı azaltır. Gürültü fazlaysa (≥ 101) karar reddedilir ve 0x0240 bit 13 gürültü bayrağı kalkar.
4. Sonuç bayrak kelimelerine yazılır. 0x00F3E8 bu bayrağı taşıyıcı susturma bilgisiyle (P57 SUSTURMA girişi) birleştirip hoparlör ses yolunu açar ya da kapatır.

Bu yöntem ucuz donanımla, yani bir karşılaştırıcı ve bir zamanlayıcıyla, seçici bir dar bant filtre kuruyor. Yeni yazılımda aynı algoritma doğrudan korunabilir; ADC'si olan bir MCU'da Goertzel ile daha hassas yapılabilir.

## 8. DTMF

Yazılım DTMF'i yalnızca gönderiyor; tuş takımından numara çevirmek için. Kodda DTMF çözme bulunamadı. Donanımda iki tonu ayırabilecek bir yol yok: seçmeli çağrı girişi (P66) tek bitlik bir kare dalga ve yazılım onun yalnızca periyodunu ölçüyor.

### Üretim (0x005282 civarı + TB1 kesmesi)

TB1 örnekleme saati olarak çalışıyor. Her TB1 kesmesinde iki faz akümülatörü ilerletiliyor: düşük grup 0x025F, yüksek grup 0x0261. Bunların sinüs değerleri toplanıp TA1'e yazılıyor. TA1 tek atım modunda, TB1 her seferinde onu tetikliyor ve çıkış P52 SÇ\_TON'dan TD14 ve FX806 üzerinden modulasyona gidiyor.

| Tablo | Adres | İçerik |
| --- | --- | --- |
| Tuş tablosu | 0x2443 | 12 tuş × 4 bayt: TB1 bölücüsü (kelime), düşük ve yüksek grup faz artımı |
| Düşük grup sinüs | 0x2473 | 256 kelime, genlik 65–185 |
| Yüksek grup sinüs | 0x2673 | 256 kelime, genlik 49–201 |

Yüksek grubun genliği daha büyük. Bu bilinçli bir "twist": yüksek frekanslar ses yolunda daha çok zayıfladığı için önden telafi ediliyor.

### Tuşların frekansları (tablodan hesaplandı)

| Tuş | Düşük grup (Hz) | Yüksek grup (Hz) |
| --- | --- | --- |
| 1 / 2 / 3 | 699 | 1214 / 1332 / 1472 |
| 4 / 5 / 6 | 773 / 772 / 773 | 1214 / 1333 / 1472 |
| 7 / 8 / 9 | 849 / 854 / 854 | 1203 / 1337 / 1486 |
| \* / 0 / # | 947 / 939 / 939 | 1212 / 1337 / 1481 |

Standart frekanslar 697/770/852/941 ve 1209/1336/1477 Hz; tüm sapmalar DTMF toleransı olan %1.5'in içinde. A–D tuşları (1633 Hz sütunu) yok.

Her tuş için iki 8-bit faz artımından sonra en iyi sonucu verecek örnekleme hızı (8.5–9.7 kHz) ayrı seçilmiş; bu yüzden TB1 değeri tuştan tuşa değişiyor. Ton süresini TA4 ölçüyor; ilk yüklemesi 0x189C (= 50 ms).

## 9. Seçmeli çağrı (Select-5)

Seçmeli çağrı 1–6 ardışık tondan oluşur. Desteklenen standartlar CCIR, ZVEI ve EEA; ayrıca ASELSAN'ın sayısal (FFSK) çağrısı var. Standart, EEPROM'dan gelen 0x0342 baytının üst 4 bitinde seçiliyor. Ton üretme TKS ile aynı DDS yöntemini kullanıyor, ama ayrı zamanlayıcılar ve P52 çıkışıyla. Alma tarafı kare dalganın periyodunu ölçerek tonu buluyor.

### Ton tabloları (ROM'dan hesaplandı, Hz)

| Hane | CCIR (100 ms) | ZVEI1 (70 ms) | EEA (40 ms) |
| --- | --- | --- | --- |
| 0 | 1982 | 2401 | 1982 |
| 1 | 1125 | 1060 | 1125 |
| 2 | 1198 | 1160 | 1198 |
| 3 | 1274 | 1270 | 1274 |
| 4 | 1358 | 1400 | 1358 |
| 5 | 1445 | 1531 | 1445 |
| 6 | 1539 | 1669 | 1539 |
| 7 | 1639 | 1830 | 1639 |
| 8 | 1746 | 2000 | 1746 |
| 9 | 1859 | 2199 | 1859 |
| A (grup) | 2401 | 2801 | 1055 |
| B | 931 | 810 | 931 |
| C | 2245 | 970 | 2245 |
| D | 991 | 885 | 991 |
| E (tekrar) | 2109 | 2601 | 2109 |

Her standardın iki tablosu var: bir TB1 bölücü tablosu (15 kelime) ve bir faz artımı tablosu (15 bayt). Adresleri: CCIR 0x2873/0x2891, ZVEI 0x28A0/0x28BE, EEA 0x28CD/0x28EB. Ton süresi (100/70/40 ms) aynı yerde parametre olarak geliyor ve TA4'e `süre × 126` olarak yükleniyor. Kılavuzdaki ZVEI2 için ayrı bir tablo bulunamadı; ZVEI2 muhtemelen ZVEI1 tablosunu farklı bir hane eşlemesiyle kullanıyor, bu doğrulanmadı.

### Gönderme (0x00E65B)

1. Gönderilecek numara (kimlik, çağrı ya da transpond, en fazla 6 hane) bir tampona dizilir. Boş haneler 0x0F ile doldurulur ve 0x0F "son" anlamına gelir.
2. Aynı hane arka arkaya gelirse ikincisi yerine E (tekrar) tonu konur. Bu Select-5 kuralı; kodda tam yeri ayrıca doğrulanmalı.
3. Rölenin açması için "ton gönderme gecikmesi" beklenir. İlk ton, "ilk ton süresi katsayısı"na göre uzatılabilir.
4. Her hane için: TB1 bölücüsü ve faz artımı tablodan okunur. TB1 kesmesi DDS'yi (sinüs 0x28FA) ilerletip TA1'i tetikler, çıkış P52'den gider. TA4 ton süresini sayar ve süre dolunca bir sonraki haneye geçilir.

ANI (PTT ile kimlik gönderme) da büyük olasılıkla aynı fonksiyonla yapılıyor. Kılavuza göre ANI çağrıyla, PTT basılınca, PTT bırakılınca veya bunların kombinasyonlarında gönderilebiliyor.

### Alma (0x007CAE kurulum, TB1 + TA1 kesmeleri)

Alma sesi FX806'nın filtresinden (TD3-17) ve TD12 sıfır-geçiş algılayıcısından geçip P66'ya kare dalga olarak geliyor.

1. **Periyot ölçümü:** TB1 periyot ölçme modunda (TB1MOD=0x42, f16 = 504 kHz). Her kenarda ölçülen periyot 0x02AA tamponuna yazılır ve 0x0274'te toplanır. Tampon boyu 0x0351'de.
2. **Pencere:** TA1 zamanlayıcı modunda bir analiz penceresi açar. Pencere bitince TA1 kesmesi (0x01C6C0) çalışır.
3. **Gürbüz ortalama:** Toplanan örnek sayısı tampon boyunun yarısından azsa pencere reddedilir. Yoksa ortalama periyot bulunur, ortalamanın 0.7–1.4 katı dışındaki örnekler atılıp kalanlar yeniden ortalanır. Frekans yaklaşık 5.4 kHz'in üstündeyse de pencere reddedilir.
4. **Hane kararı:** Bulunan periyot standardın ton tablosuyla karşılaştırılıp hane belirlenir. Hane dizisi beklenen "Seçmeli Çağrı No" ile eşleşirse bireysel çağrı (1625 Hz uyarı) verilir. Grup kodu yerinde A veya 0 varsa grup çağrısı olur. Programlanmışsa transpond numarası otomatik gönderilir.

Son adımın ayrıntıları (tablo karşılaştırma toleransı, hane zamanlama kuralları) henüz satır satır okunmadı; davranış, kılavuzun 1.6.3 ve 1.7 bölümleriyle birleştirilerek yazıldı.

### Kod haritası (kaldırmak için)

Çağrı grafiğinden (giriş noktaları: gönderme `F_00E65B`, alma kurulumu `F_007C46` / `L_007CAE`, karar kesmesi TA1 `L_01C6C0`) yalnız seçmeli çağrının ulaştığı kod:

| Kod | Adres | Komut | Görev |
|---|---|---|---|
| `F_00E65B` | 0x00E65B–0x00E9D1 | 395 | Gönderme: kimlik / çağrı / transpond tonları (TB1 DDS + TA1 PWM, TA4 süre) |
| `F_007C46` + `L_007CAE` | 0x007C46–0x007CF5 | 76 | Alma çözücüsünü başlatır: TB1 periyot ölçme (TB1MOD=0x42), TA1 pencere kesmesi |
| TA1 kesmesi `L_01C6C0` | 0x01C6C0– | 252 | Pencere sonu: periyotları ortalar, haneyi bulur |
| `F_01BBE4` | 0x01BBE4– | 1177 | Hane dizisi, çağrı / grup / transpond kararı, çağrı ekranı ve uyarı |

Toplam ~1850 komut. TB1 kesmesinin (`L_01C627`) TX dalı DTMF için de kullanıldığından kalır; RX dalı (0x023E bit 4) yalnız çözücü içindir. Çözücü, telsizin kendi seçmeli çağrı numarası programlıysa (0x0066B9, 0x015118: 0x023A bit 15) alma kurulumunda (0x00EBF2) başlatılır.

APRS (ozel_firmware/ornekler/aprs.inc) PTT'deki kimlik gönderimini (0x00F4E4) kendisi kullanıyor. Özel firmware'de bu dört bölge `ozel_firmware/ornekler/secmeli_kaldir.inc` ile tamamen kaldırıldı (4232 bayt boşaltıldı).

## 10. Veri, C-BUS, ses kaydı, PC bağlantısı ve flash yükleme

Ses işleme ve modem işleri CML (Consumer Microcircuits) FX serisi yongalara bırakılmış; işlemci onları C-BUS'tan yönetiyor. PC ile bağlantı RS232 üzerinden. Aynı hat hem EEPROM programlama ve klonlama için, hem de flash'ı yeniden yazmak için kullanılıyor.

### C-BUS

| Hat | Pin | Görev |
| --- | --- | --- |
| Seri saat | P85 (CLK1) | UART1 senkron saat, sentezörle ortak |
| Seri veri | P87 (TxD1) | Komut ve veri |
| Yanıt veri | P86 (RxD1) | Yongadan okunan veri |
| Seçim | P51 TDSEC1, P53 TDSEC2 | TDSEC1: FX806 / FX809 / FX802; TDSEC2 ikinci grup |
| İstek | P64 OKUMA\_İSTEK1 (INT2) | FX809 ve FX802 kesme isteği |

0x007E4C TDSEC1 yongalarına, 0x00755E TDSEC2 yongalarına yazıyor. FX806'ya her RX/TX geçişinde mod, modülatör seviyesi ve ses seviyesi komutları gönderiliyor. Komut bayt değerleri henüz tabloya dökülmedi.

### FFSK veri ve sayısal çağrı (FX809)

FX809 1200 baud FFSK modem. Almada çözdüğü baytı INT2 ile bildiriyor, işlemci C-BUS'tan okuyor. Göndermede işlemci baytları C-BUS'tan yazıyor. Kılavuzdaki "sayısal seçmeli çağrı (ASELSAN)" ile K varyantındaki "konvansiyonel veri" opsiyonu bu yolu kullanıyor. Ekrandaki CALLING / QUEUED / PBX / FLEET / SDATA metinleri de buna bağlı bir çağrı protokolüne işaret ediyor. Paket formatı henüz çözülmedi.

### Ses kayıt ve analog şifreleme (FX802 + TD16 DRAM)

FX802 sesi delta modülasyonla sayısala çevirip 1024 bitlik sayfalar halinde DRAM'e yazıyor ve geri çalıyor. Kılavuza göre bu özellik ses saklama ve analog şifreleme (kripto tuşu) için kullanılıyor.

### PC bağlantısı ve klonlama (UART0)

UART0 kesmelerle çalışıyor (RX 0x019469, TX 0x0195B2). Bağlantı ekranı ve servis menüsünün metinleri ROM'da duruyor: `CONNECT`, `CLONING`, `BAUDRATE`, `NO LINK`, `LINKING` ve yardım satırı `HELP 1 FLASH 3 EEPROM 4 CHNGBR 7 RLINK * RADIO # PLNO`. Bu menüde flash yükleme, EEPROM işlemi, baud değiştirme ve uzak bağlantı seçenekleri var.

Telsizden telsize klonlama, kılavuza göre aktaran telsizin PTT'si basılı açılmasıyla başlıyor. Ekranda AKTARMA ve ALIYOR yazıyor. Yalnızca genel özellikler aktarılıyor ve iki telsizin aktarma anahtarı, bandı ve kanal aralığı aynı olmalı.

### Flash yükleyici (RAM 0x0080)

Program kendi flash'ını (AMD 29F010) yerinde yeniden yazabiliyor. Flash yazılırken ondan kod çalıştırılamadığı için yükleyici önce RAM'e kopyalanıyor.

1. 0x0105F3: kesmeler kapatılır. ROM 0x010100–0x0105C7 (1224 bayt) RAM 0x0080'e kopyalanır ve `JSRL $000080` ile oraya atlanır. Bu işlem RAM'deki tüm değişkenleri ezer; geri dönüş yok.
2. Yükleyici UART0'ı yoklamayla (kesmesiz) dinler ve bir çerçeve durum makinesi işletir. Karşılaştırılan karakterler 0x16 (SYN), 0x10 (DLE), 0x02 (STX), 0x03 (ETX); yani DLE kaçışlı bir çerçeve protokolü.
3. Her bayt JEDEC komut dizisiyle yazılır: `[5555]=AA`, `[2AAA]=55`, `[5555]=A0`, ardından veri. Yazılan adres geri okunup beklenen bayt gelene kadar bekleniyor (data polling); zaman aşımını TA3 sayıyor. Gerektiğinde `F0` komutuyla yonga okuma moduna döndürülüyor. Sektör silme dizisinin yeri henüz bulunmadı.
4. Yükleme bitince ne olduğu (reset, bekleme) henüz izlenmedi.

Bu yükleyici yeni yazılımı karttan sökmeden yüklemek için kullanılabilir. Bunun için çerçeve formatının tam çözülmesi gerekiyor (Bölüm 11). Not: flash'ın ilk 8 KB'ı (0x00000–0x01FFF) adres çözücü yüzünden işlemciye görünmüyor.

## 11. Yeni yazılım için yol haritası

Yeni yazılımın işe yarayan kısmı büyük oranda belli: donanım sürücüleri az ve basit. Asıl emek EEPROM formatını ve menü/durum makinesini yeniden kurmakta. İlk karar, yazılımın mevcut M37732 üzerinde mi yoksa taşıyıcı kartta (MSP430 vb.) mi çalışacağı.

|  | Mevcut M37732 | Taşıyıcı kart (MSP430) |
| --- | --- | --- |
| Donanım değişikliği | yok | işlemci sökülür, adaptör lehimlenir |
| Derleyici | NC77 (demo + dongle elinizde) veya assembler (AS `CPU MELPS7700`) | ücretsiz GCC / TI araçları |
| Yükleme | Mevcut flash yükleyici ile RS232'den (çerçeve formatı çözülmeli) | JTAG / BSL |
| Ton işleme | Aynı algoritmalar (DDS + 1-bit korelatör) | ADC + Goertzel ile daha iyi; DTMF çözme de eklenebilir |
| Pin uyumu | birebir | 37 G/Ç sinyali ve A/D kanalları eşlenmeli (donanim\_haritasi.md) |
| Risk | eski araç zinciri | yeni kart tasarımı |

### Modül sırası (iki yol için de)

1. **Temel:** Saat, port yönleri, watchdog, 32 ms tick, P71 güç tutma (ilk iş bu; yoksa telsiz kendini kapatır).
2. **I²C + EEPROM:** Mevcut EEPROM'u okuyup kanal ve parametreleri dökebilmek. Ayarları korumak için kritik.
3. **Sentezör:** MB1502'ye R ve N/A kelimelerini yüklemek, kilit (P67, aktif düşük) beklemek. Frekans Bölüm 4'teki formülle hesaplanıyor; c\_clean/synth.c bunu orijinalle aynı baytları üretecek şekilde uyguluyor. Yeni kod gerçek cihaza yüklenmeden disasm/sim/ altındaki web simülatöründe denenebilir.
4. **RX/TX:** ALMA\_SEÇ, PTT, güç PWM'i ve AN7 ile kapalı çevrim, susturma (P57) ve hoparlör.
5. **C-BUS:** FX806 ilk ayarlar ve RX/TX modları. Bunsuz ses yolu açılmaz.
6. **Ön panel:** LCD (0x70), tuş ve LED genişleticileri (0x42/0x46), INT1.
7. **TKS:** DDS tabloları ve korelatör ROM'dan aynen alınabilir.
8. **Seçmeli çağrı, DTMF, FFSK, tarama, menüler.**

### Açık sorular

- [ ] EEPROM 0x000–0x02F ve 0x538–0x7FF alanlarının tek tek anlamı (çalışan bir telsizin EEPROM dökümü çok hızlandırır)
- [ ] RX ara frekansı ve MB1502 prescaler seçimi (RF kartı şeması, Şekil 6-3/6-4)
- [ ] FX806 / FX809 C-BUS komut değerleri (CML veri sayfaları + 0x007E4C çağrıları)
- [ ] Flash yükleyicinin çerçeve formatı ve PC programlama protokolü (UART0)
- [ ] Ana döngünün özel kolunun (0x0246 bit 1) görevi: tarama mı, PC modu mu
- [ ] Bu firmware'in el telsizi mi araç telsizi mi olduğu; kılavuz 4021/4025 için, pin kullanımı farklı olabilir

### Kaynaklar

- Firmware disassembly ve araçlar: `disasm/` (YA4000?.asm, donanim\_haritasi.md, tools/d77.py); firmware'in tamamının C çevirisi `disasm/c_lifted/` (emülatörle eşdeğerliği test edildi) ve yeni yazılım için temiz sürücüler `disasm/c_clean/`
- 4021/4025 VHF/UHF-FM AT, 4031/4035 SMT İşletme, Depo Bakım ve Yedek Parça İkmal Kataloğu, Rev. C (bölüm 1.6–1.8, 3.3, 3.6, 3.7, parça listesi)
- Şekil 6-5 K3 Kontrol ve SF Kartı devre şeması
- MELPS 7700 Software Manual (1989) ve M37732S4 veri sayfası (1991 Mitsubishi 16-bit data book)
