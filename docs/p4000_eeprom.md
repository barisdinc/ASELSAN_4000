# P4000 programlama yazılımı ve EEPROM yerleşimi

ASELSAN'ın 4000 serisi için programlama yazılımı **P4000** (`P4000.EXE`, 25.11.1998). Program Borland C++ 5 ve OWL ile yazılmış; yanında İngilizce kaynak DLL'i `P4000009.DLL` geliyor. Bu belge, EXE'nin decompile edilmesiyle çıkan EEPROM (24C16, 2 KB) yerleşimini ve P4000'deki alan adlarını özetliyor. Simülatördeki EEPROM sekmesi ve `yukle.html` aynı tabloyu kullanıyor (`disasm/sim/p4000.js`).

## Nasıl çıkarıldı

- **Ekranlar.** Sayfalar ve alan adları EXE'nin diyalog kaynaklarından okundu: 61 Sabit, 62 Özel, 63 Aktarılabilir, 64 Kanal, 65 KSYS ve 72 Analog Veri. Hangi kontrolün hangi değişkene bağlı olduğu, diyalog sınıflarının kurucularındaki kontrol kimliklerinden çıkarıldı. Örneğin sayfa 62'nin kurucusu 0x46A500'de.
- **Paketleme.** P4000 her parametreyi bir global değişkende tutar ve bunu bir metin tanımla EEPROM görüntüsüne paketler.
  - Tanımın biçimi `"adres.bit.uzunluk;adres.bit.uzunluk"`.
  - Kayıt fonksiyonu 0x475900, paketleme 0x475D14, açma 0x475E90.
  - Değişkenin bitleri en anlamlı bitten başlayarak bölümlere sırayla dağıtılır. Her bölümde verilen bitten aşağı doğru yazılır.
- **Tanım listesi.** Kayıt fonksiyonları (0x47A81C, telsiz tipine göre) unicorn ile emüle edildi ve tam tanım listesi alındı. Telsiz tipi 0 (EL) için 2385 kayıt çıktı. `p4000.js`'teki 1847 tanımın hepsi bu listeyle birebir aynı.
- **Kütük anahtarları.** `.p40` anahtarları ve dönüşümleri P4000'in dosya okuma (0x487B3C) ve yazma (0x48926C) kodundan alındı.
- **Sağlama blokları.** 0x480CB0'daki okuma tablosundan alındı.
- **Doğrulama.**
  - Gerçek bir telsizin dökümünde yedi sağlamanın yedisi de tutuyor.
  - Çözülen değerler anlamlı: seri no, MHK tarihi 06.09.2025, sürüm "0402F-B-B-", üretim sıra no.
  - Beş `.p40` kütüğü kütük → EEPROM → kütük gidiş-dönüşünde aynı çıkıyor (`make test-p4000`).

## Alanlar (P4000 adresleri)

P4000 telsizi okurken önce **cihaz alanları**nı kontrol eder ve her alanın sağlamasına bakar. Sağlama, 16 bit bayt toplamıdır ve hemen arkasına büyük uçlu yazılır. Firmware de açılışta ilk dördünü denetler; tutmazsa "ARZ PRG" verir.

| Alan | Okunan | Sağlama aralığı → yer |
|---|---|---|
| SABİT | 0x000–0x006 | 0x000–0x004 → 0x005 |
| SERVİS | 0x008–0x017 | 0x008–0x015 → 0x016 |
| AKTARMA | 0x018–0x545 | 0x018–0x534 → 0x535 |
| TEST-AYAR | 0x578–0x5BE | 0x578–0x5BC → 0x5BD |
| KULLANICI | 0x548–0x553 | 0x54B–0x551 → 0x552 |
| ANALOG VERİ | 0x600–0x6D3 | 0x600–0x6D1 → 0x6D2 |
| KSYS | 0x7E0–0x7F5 | 0x7E0–0x7F3 → 0x7F4 |

### Sabit Parametreler

| Alan | EEPROM | Not |
|---|---|---|
| Sabit Merkez | 0x000 b5 | |
| Ses Sıkıştırma | 0x000 b3 | .p40 `[Master] Compander` |
| TDM | 0x000 b2 | `[Master] TDM` |
| Seri No | 0x001–0x002 | 16 bit, büyük uçlu |
| MHK Programlama Tarihi | 0x004:0x003 | gün b15..11, ay b10..7, yıl 1994 + b6..0 |
| Cihaz Versiyon No | 0x578–0x587 | C dizisi |
| Üretim Sıra No | 0x588–0x58A | 6 BCD hane |
| Telsiz Tipi | 0x58B b1 / b5, 0x000 b5 | b1 = 0 ise EL; değilse b5 = 1 ise TEKRARLAYICI, sabit merkez biti varsa SABİT MERKEZ, yoksa ARAÇ |
| Donanım Tipi | 0x58B b3 / b6 / b0 / b4 | sırasıyla SAYISAL KRİPTO, SK II, TDM, KSYS; hiçbiri yoksa STANDART |
| Frekans Bandı | 0x58C–0x58D | HalfBaseChan, 25 kHz birim |

### Özel Parametreler (SERVİS)

| Alan | EEPROM | .p40 |
|---|---|---|
| Programlayıcı Sürüm No | 0x009 . 0x008 | |
| Aktarma Anahtarı | 0x00A | `[Service] CloneKey` |
| ANI No | 0x00B–0x00D | 6 hane, 0–9 A–D, boş hane 0xF. İlk hane 0x00D'nin alt yarısında, ikinci hane üst yarısında; sonra 0x00C, 0x00B. |
| Seçmeli Çağrı No | 0x00E–0x010 | aynı biçim |
| ACK No | 0x011–0x013 | aynı biçim |
| Deniz El Telsizi | 0x014 b7 | `MarineRadio` |
| Kanal İzleme Özelliği | 0x014 b6 (ters) | `MonitorDisabled` |
| Açılışta Opsiyon | 0x014 b5 | `OptionAtStart` |
| Aktarma Özelliği | 0x014 b4 | `Cloner` |
| Tuş Takımı | 0x014 b3 | `KeyPad` |
| Seçmeli Çağrı Gönderme | 0x014 b2 | `SCEnc` |
| Sabit Merkez Telsizi | 0x014 b1 | `MicType` |
| VOX | 0x014 b0 | `VOXExists` |
| Yüksek güç / Düşük güç | 0x015 üst / alt yarım | `HPIndex` / `LPIndex` |

### Aktarılabilir Parametreler (AKTARMA)

| Alan | EEPROM | .p40 `[Clone]` |
|---|---|---|
| Dil | 0x018 b7 (1 = Türkçe) | `Language` |
| Güç Tasarrufu | 0x018 b6 | `PowerSave` |
| Alt Menü | 0x018 b5 | `DigerMenu` |
| Meşgul Kanal Kilidi | 0x018 b4 + 0x026 b3 | `BusyChan`: 0 yok, 1 kanal aktif ile, 2 TCS ile |
| Öncelik | 0x018 b3 (0 Çağrı, 1 ANI) | `Oncelik` Call / PTT |
| Grup Tonu | 0x018 b2 (0 = "0", 1 = "A") | `GrupTonu` |
| Ton Standardı | 0x018 b1..0 | DSC / CCIR / ZVEI / EEA |
| Acil Çağrı, kanal | 0x019 b7, b6..0 | `AcilCagri`, `AcilCagriKanali` |
| ANI Tipi | 0x01A üst yarım | 0 yok, 1 çağrı, 2 PTT, 3 PTT bırakılınca, 4 çağrı+PTT, 5 çağrı+PTT bırakılınca. Firmware 2 / 4'te PTT'de kimlik gönderir. |
| Tx Gecikmesi | 0x01A alt yarım × 100 ms | `TxDelay` |
| Uzatılmış İlk Ton Süresi | 0x01B üst yarım | `ExtendedTon` |
| Ton Sayısı | 0x01B alt yarım | `TonLength` (seçmeli çağrı no uzunluğu, ekranda yok) |
| Seçilebilir çağrı hane sayısı | 0x01C üst yarım (0, 2..6) | `UserSelectedDig` |
| Tarama Gecikmesi | 0x01C alt yarım × 0,5 sn | `ScanDelay` |
| Tot | 0x01D üst yarım × 15 sn | `Tot` |
| Rekey Gecikmesi | 0x01D alt yarım | `RekeyDelay` |
| Harici Giriş | 0x026 b4 (0 ses, 1 veri) | `ExtInputModem` |
| 0x01E–0x025, 0x026 b2..0, 0x027–0x02D | — | Seçmeli çağrı ton süreleri ve kod çözücü ayarları. P4000 bunları yeni kütükte ton standardına göre kendi tablosundan (0x4AA868) doldurur, ekranda göstermez. |
| Öncelikli kanal 1 / 2 | 0x544 / 0x545 | `PriorChannel1/2` (sağlama dışında) |
| Gösterge ters / Işıklar kapalı / Sesli uyarı yok | 0x54B b5 / b0 / b1 | `[User] invdisplay` / `lightsoff` / `warningoff` |

### Kanal Parametreleri

Kanal n'in kaydı 0x02E + 13·(n−1) adresinde başlar; kanal 1..99.

| Bayt | İçerik |
|---|---|
| +0 | b7 **HPRF**, b6 **TOT**, b5..0 TX kelimesinin üst 6 biti |
| +1 | TX kelimesinin alt 8 biti |
| +2..+3 | RX kelimesi (b7..6 kullanılmıyor) |
| +4 | b7 **ANI**, b6 kristal kaydırma, b5..0 RX TCS (1..50) |
| +5 | b7 TX TCS var |
| +6..+11 | b7'lerde TX TCS (6 bit, ilk bayt en anlamlı) |
| +5..+12 | b6..0'larda **kanal adı** (8 karakter, 7 bit) |

- **Frekans kelimesi.** Kelime = (f − taban) / 2,5 kHz, 14 bit. 0x3FFF boş demektir.
  - Firmware TX ve RX kelimelerini ayrı ayrı denetliyor (0x5B93 ve 0x87C5).
  - P4000 yalnız TX'i boş kanala izin veriyor, yani yalnız alma yapan kanal.
- **Tarama listesi.** 0x537 + (n−1)/8 baytı, bit (n−1)%8.
- **Kristal kaydırma.** P4000 bu biti RX frekansından kendisi hesaplar (0x4885B1). Ekranda göstermez.
  - **f < 300 MHz:** k = 289..346 için f, k × 504 kHz'in −25 / +20 kHz yakınındaysa bit 1 olur.
  - **UHF:** 32 bit taşma yüzünden kural hiç tutmuyor.
  - 504 kHz, işlemci saatinin harmoniği: 8,064 MHz / 16.
  - **Gerçek döküm farklı:** İncelenen dökümde bit P4000'in kuralından farklı kanallarda (145,675 ve 145,700 MHz) işaretli. Simülatör, telsizden okunan değeri korur ve biti yalnız RX değişince yeniden hesaplar.
- **TCS tablosu.** Telsizin ton numaralarıyla aynı sırada. P4000 kütüğe tam sayı Hz yazar, ör. 88,5 → 88.

### KSYS (0x7E0) ve Analog Veri (0x600)

- **KSYS Parametreleri:**
  - 0x7E0–0x7E1 Sistem Kimliği.
  - 0x7E2 SMT Kimliği.
  - 0x7E3–0x7E4 alma kelimesi, 0x7E5 alma TCS.
  - 0x7E6–0x7E7 gönderme kelimesi, 0x7E8 gönderme TCS.
  - 0x7E9 b0 Yüksek Güç.
  - 0x7EA–0x7F3 Grup No 1..10.
- **Analog Veri Parametreleri:**
  - 0x600 Sistem Kimliği.
  - 0x601–0x602 Bölge Kimliği (`FLEETID`), 0x603–0x604 Telsiz Kimliği (`RADIOID`), 0x605–0x606 Merkez Kimliği (`RXID`).
  - 0x607 Röle Tetiklenme Süresi (`PREAMBLE`).
  - 0x608–0x611 Grup 1..5 (16 bit, işaretli).
  - 0x612–0x6D1 Durum 1..24 (8'er karakter).
- Bu iki alanı bizim firmware kullanmıyor (doğrulanmadı), ama P4000 yazar ve sağlamasını tutar.

### KULLANICI ve diğerleri

- **0x548–0x54A:** 24 bit sayaç. Sağlama dışında; anlamı bilinmiyor.
- **0x54C:** Bit alanları.
- **0x54D–0x551:** Parola alanı. Firmware parolayı 0x54E–0x551'de tutuyor; hepsi 0xFF ise parola yok.
- **TEST-AYAR (0x578–0x5BC):** Fabrika ayar alanı. P4000 burayı yazmaz; T4000 test yazılımıyla doldurulur. Firmware'den bilinenler:
  - 0x58F referans seçimi.
  - 0x591–0x59A ilk güç PWM değerleri.
  - 0x59B–0x5A4 ve 0x5A5–0x5AE güç hedefleri.

## .p40 kütüğü

INI biçiminde bir metin dosyası; kodlaması Windows-1254, satır sonu CRLF.

- **Normal kipte yazılan bölümler:** `[TestAyar]` (yalnız `HalfBaseChan[0..1]`, yani bant), `[User]`, `[Clone]`, `[Kanal]` (99 kanal), `[KSYS]`, `[ADATA]`.
- **`[Master]` ve `[Service]`:** Yalnız test kipinde okunur ve yazılır.
- **Kanal alanları:**
  - `Kanal Adi[n]=<8 karakter>`.
  - `TxFreq` / `RxFreq` `%.4f` MHz biçiminde; boş kanal `0.0000`.
  - `TxTCS` / `RxTCS` Hz cinsinden; ton yoksa boş.
  - `TotVar`, `Hprf`, `ANIVar` ve `Scan` 0 / 1.
- **Öncelikli kanallar:** `PriorChannel1/2` kanallardan sonra `[Clone]` bölümüne eklenir.
- **Bant uyuşmazlığı:** P4000, kütüğün bandı telsizinkinden farklıysa telsize yazmaz.
