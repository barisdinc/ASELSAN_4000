# Sürüm notları

*Bu dosya `ozel_firmware/degisiklikler.json`'dan üretilir (`make -C disasm sim`); elle değiştirmeyin.*
Aynı notlar sitede: [surumler.html](https://barisdinc.github.io/ASELSAN_4000/surumler.html); programlama sayfası firmware seçilince de gösterir.

## yATOLYE özel firmware'i (`YA4000S_FREKANS_YATOLYE.HEX`)

Güncel sürüm: **AT V0-29** (22.10.2026)

Orijinal YA4000S (AS V2-30) üzerine FREKANS (VFO) menüsü, APRS gönderme / alma, seçmeli çağrı kaldırıldı, açılışta 5 s yATOLYE.

### Özellikler

- **FREKANS (VFO) menüsü** (AT V0-1 ile geldi): Kanal tablosundan bağımsız alma ve gönderme frekansı, alma ve gönderme TKS (CTCSS) tonu tuşlarla girilir (tonlar V0-17 ile). Ekranda kanal numarası yerine VR (alma) / VT (gönderme); PTT'de gönderme frekansı görünür.
- **APRS gönderme** (AT V0-1 ile geldi): APRS menüsünde PTT ile ya da ayarlanan aralıkta (PTT'siz) 144,800 MHz'de 1200 baud konum paketi. Çağrı işareti, konum, simge, yorum ve aralık EEPROM'da (0x6E0); programlama sayfasının APRS sekmesinden düzenlenir.
- **APRS TR / DG / IG ayarları** (AT V0-24 ile geldi): Menüde APRS TR (tracker), APRS DG (digipeater), APRS IG (seri port / igate); 's' ile * / -. Yalnız TR açıkken normal kanalda arka plan tracker (30 s boşlukta 144,800). DG açıkken cihaz tipi APATAR. EEPROM 0x733.
- **GPS konumu** (AT V0-26 ile geldi): Seri porttan $GPRMC (GPS ya da sayfadaki benzetim); APRS ekranında dakikada bir konum güncellenir, paketlerde kullanılır; ekranda GPS.
- **APRS alma: duyulan istasyonlar** (AT V0-6 ile geldi): APRS menüsündeyken çözülen paketin gönderen çağrı işareti ~4 s ekranda. Son 8 istasyon listesi: * bir eskiye, # bir yeniye.
- **APRS alma: istasyon konumu ve mesajı** (AT V0-22 ile geldi): Listede bir istasyon görünürken 's': enlem, boylam, mesaj (ilk 8 karakter), çağrı işareti sayfaları (8 s). Düz konum biçimleri; sıkıştırılmış ve Mic-E konumu çözülmez (Mic-E yorumu gösterilir).
- **APRS alma: seri port çıkışı** (AT V0-18 ile geldi): Çözülen her paket UART0'a (programlama kablosu, 9600 8N1) TNC2 satırı olarak yazılır. 160 baytı aşan paketlerin bilgi alanı kesilir.
- **APRS'te sessiz hoparlör** (AT V0-7 ile geldi): APRS menüsündeyken bip, uyarı ve hoparlör kapalı; alma çözücüsü çalışmaya devam eder.
- **APRS modunu hatırlama** (AT V0-4 ile geldi): Telsiz APRS menüsündeyken kapatılırsa açılışta yine APRS'te başlar (EEPROM 0x732).
- **Tarama** (AT V0-16 ile geldi): Orijinal tarama (LIST menüsü + p tuşu) çalışır; TKS'li kanallarda ton için daha uzun bekleme (ASELSAN V2-51 / V2-54'ten).
- **Sadeleştirilmiş firmware** (AT V0-1 ile geldi): Select-5 (CCIR / ZVEI / EEA) kodu ve '#' ile arama kaldırıldı; yer yeni özelliklere açıldı. Trunk, veri mesajı, ses yayın, yazıcı ve servis terminali de kapalı (V0-19 / V0-21). Yalnız Türkçe (V0-22).
- **Açılış ekranı ve sürüm** (AT V0-1 ile geldi): Açılışta 5 s yATOLYE, ardından AT V0-x sürüm numarası. Sürüm, tarih ve metin ROM başlığında; T4000 ve yükleme sayfası da gösterir.

### Sürüm geçmişi

#### AT V0-29 · 22.10.2026 · yalnız simülatörde denendi

- Değişiklik: Kendi APRS paketlerimizin yolu WIDE2-2 (önceden WIDE1-1,WIDE2-1).

#### AT V0-28 · 22.10.2026 · yalnız simülatörde denendi

- Değişiklik: APRS frekansı telsizin bandına göre: VHF telsizde 144,800 MHz, UHF telsizde 432,500 MHz (APRS ekranı ve arka plan tracker). Bant EEPROM'daki bant tabanından (0x58C) anlaşılır.

#### AT V0-27 · 22.10.2026 · yalnız simülatörde denendi

- Yeni: Arka planda tracker: yalnız APRS TR açıkken (DG, IG kapalı) telsiz normal kanalında (ya da VFO frekansında) kalır, orayı dinler ve oradan konuşur. Beacon süresi dolunca kanal gözlenir; 30 s kesintisiz sinyal yoksa 144,800'e geçip tracker paketini gönderir (ekranda APRS TX) ve kanala döner.
- Değişiklik: APRS ekranında beacon kanalda sinyal varken beklenir, ilk boşlukta gider.
- Değişiklik: Digipeater açıkken kendi paketlerimizin cihaz tipi (hedef) APATAR, değilken APIZCI.
- Değişiklik: APRS ekranındaki 'APRS PTT' yazısı 'APRS AKT' (aktif; ekran 8 karakter).

#### AT V0-26 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: GPS: seri porta (programlama kablosu, 9600) bağlı bir GPS ya da programlama sayfasındaki GPS benzetimi $GPRMC gönderirse, APRS ekranındayken dakikada bir yeni konum alınır ve paketlerde EEPROM konumu yerine kullanılır; ekranda "GPS". GPS konumu yokken her saniye denenir; geçerli cümle gelmezse en geç iki dakikada EEPROM konumuna dönülür.
- Yeni: Programlama sayfası: seri izlemede harita ve GPS benzetimi (haritadan nokta, GÖNDER: saniyede bir $GPRMC + $GPGGA).
- Düzeltme: Tracker açık ve aralık ayarlıyken duyulan istasyonun ekranda kalma süresi ve digipeater'ın 30 s tekrar engeli / PTT zaman aşımı işlemiyordu (geçen süre sayacı sıfırlanıyordu).
- Değişiklik: APRS paket tamponu 160'tan 140 bayta indi (GPS'e yer): seri porta yazılan / detayı çıkarılan paketlerde bilgi alanının ilk ~124 baytı; tekrarlanan paket en çok 131 bayt.

#### AT V0-25 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: APRS digipeater (APRS DG açıkken, APRS ekranında): WIDE1-1 → ÇAĞRIM*, WIDE2-2 → ÇAĞRIM*,WIDE2-1, WIDE2-1 → ÇAĞRIM,WIDE2*, doğrudan çağrımız → ÇAĞRIM*. Kendi paketimiz, bizden geçmiş paket, başka yollar (WIDE3-3 …) ve son 30 s'de tekrarlanan aynı paket tekrarlanmaz. IG açıksa paket önce duyulduğu haliyle seri porta yazılır.
- Değişiklik: Kendi gönderimimiz (PTT / tracker) bekleyen digi paketini iptal eder; verici ~3 s'de açılmazsa digi paketi bırakılır.

#### AT V0-24 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: Menüde APRS'ten sonra APRS TR (tracker: otomatik konum paketi), APRS DG (digipeater) ve APRS IG (igate: duyulan paketleri seri porta yaz) öğeleri. Başlıkta '*' açık, '-' kapalı; 's' ile değişir. Ayarlar EEPROM 0x733'te kalıcı; programlama sayfasının APRS sekmesinden de değişir. Özellikler APRS ekranında çalışır.
- Yeni: Programlama sayfası: giden / gelen baytları gösteren seri izleme (HEX / ASCII) ve telsize bir şey göndermeden yalnız dinleme.
- Değişiklik: APRS DG ayarı bu sürümde yalnız kaydedilir; digipeater bir sonraki sürümde.
- Değişiklik: İstasyon detay kodu flash'ta 0x01E600'e taşındı (0x011000 bölgesi doldu).

#### AT V0-23 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: APRS istasyon detayına mesaj sayfası: konumdan sonra 's' ile paketin metni (konum yorumu, durum, mesaj ya da Mic-E yorumu), ilk 8 karakter, büyük harf. Metin yoksa "MESAJ -". 8 istasyon için saklanır; tekrar duyulan istasyonun metni yenisiyle değişir, metinsiz pakette eskisi kalır.
- Değişiklik: Konum kayıtları RAM'de yeniden yerleştirildi (metinlere yer açmak için).

#### AT V0-22 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: APRS istasyon detayı: listede (ya da yeni duyulan) bir istasyon görünürken 's' ile enlem "4050 63N" ve boylam "3109 39E" sayfaları. Düz konum biçimleri (!, =, /, @); sıkıştırılmış konum ve Mic-E için "KONUM -". Konum 8 istasyon için kapatılan özelliklerden boşalan RAM'de tutulur.
- Düzeltme: APRS listesi: aynı istasyon farklı yollu bir paketle duyulunca listeye ikinci kez giriyordu (adresin son baytındaki yol bitleri).
- Değişiklik: Dil desteği kaldırıldı: menüler ve mesajlar her zaman Türkçe. EEPROM'daki dil ayarı (0x018 bit 7) yok sayılır.

#### AT V0-21 · 21.10.2026 · yalnız simülatörde denendi

- Değişiklik: Kullanılmayan özellikler kapatıldı (kod çalışmaz, giriş noktaları kesildi): trunk / RADIO (TELSIZ) modu, veri mesajı ve sipariş baskısı ('*' tuşu, seri komut 80 / 82), kablosuz ses yayın, '#' ile seçmeli çağrı araması, seri servis terminali (0xF8, 2400 baud), '*' servis / test modu. Seri programlama protokolü aynı.

#### AT V0-19 · 21.10.2026 · yalnız simülatörde denendi

- Düzeltme: APRS menüsünde gönderirken seri porta "4       ·01/01 00:02" gibi bir satır çıkıyordu: B tipi kartta YAZICI açıkken firmware'in yazdığı çağrı günlüğü. Yazıcı açıkken seri port 9600 yerine 2400 baud'a da geçiyordu.
- Değişiklik: YAZICI / PRINTER menüsü (yazıcı, tarih, saat) kaldırıldı; EEPROM'da açık kalmışsa da açılışta kapalı başlar. Seri port her zaman 9600 8N1. Seçmeli çağrı (ANI) olmadan kullanım alanı yoktu.
- Değişiklik: APRS menüsündeyken firmware'in çağrı günlüğü ayrıca susturuldu: seri portta yalnız APRS satırları.

#### AT V0-18 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: APRS menüsündeyken havadan çözülen her geçerli paket seri porta (RS232, 9600 8N1) TNC2 metni olarak yazılır: "TA7W-7>APRS,WIDE1-1*:!4050.63N/03109.39E>..." + satır sonu. Bilgisayarda bir terminal programı ya da APRS yazılımı ile izlenebilir.
- Değişiklik: APRS paketi menüye girerken değil, her gönderimden hemen önce EEPROM'dan kurulur (tampon almada seri port için kullanılıyor); gönderimden önce kısa bir EEPROM okuması eklenir.

#### AT V0-17 · 21.10.2026 · yalnız simülatörde denendi

- Yeni: FREKANS (VFO) modunda TKS (CTCSS) tonu: gönderme frekansından sonra 3. adımda alma tonu, 4. adımda gönderme tonu girilir (885 = 88,5 Hz, 100 = 100,0 Hz; en yakın standart ton). Boş # tonu değiştirmez (gönderme tonunda: alma ile aynı), 0 # ton yok.
- Değişiklik: FREKANS ayarı artık 4 adım: alma frekansı, gönderme frekansı, alma tonu, gönderme tonu. Tonsuz hızlı ayar: frekanslardan sonra iki kez # (156525 # 160650 # # #).

#### AT V0-16 · 21.10.2026 · yalnız simülatörde denendi

- Düzeltme: Tarama: önceki bütün FREKANS sürümlerinde p ile tarama başlatılınca telsiz kendini yeniden başlatıyordu (FREKANS kancası taramanın atladığı adresi bozuyordu).
- Değişiklik: Taramada TKS (CTCSS) tonlu kanalda tonun çözülmesi için bekleme uzadı (67 Hz: 480 → 800 ms, 250 Hz: 128 → 448 ms); ton geç çözüldüğünde kanal atlanmaz. ASELSAN'ın sonraki sürümlerinden (V2-51 / V2-54) alındı.

#### AT V0-15 · 20.10.2026 · gerçek telsizde denendi

- Değişiklik: APRS alma, gerçek telsizde en iyi çalışan hâline (AT V0-7) döndürüldü: duyulan istasyonun çağrı işareti ekranda görünür. V0-11 … V0-14'teki denemeler geri alındı.

#### AT V0-14 · 20.10.2026 · geri alındı

- ~~Değişiklik: APRS alma: taşıyıcı koşulu kaldırıldı, teşhis ekranı eklendi. Gerçek telsizde çalışmadı.~~

#### AT V0-12 · 19.10.2026 · geri alındı

- ~~Değişiklik: APRS alma: kesme seli koruması, çözücü yalnız taşıyıcı varken. Gerçek telsizde hiçbir paket duyulmadı.~~

#### AT V0-11 · 19.10.2026 · geri alındı

- ~~Yeni: APRS alma: paketin tamamı ekranda kayan yazı. Gerçek telsizde ilk paketten sonra takıldı.~~

#### AT V0-7 · 19.10.2026 · gerçek telsizde denendi

- Yeni: APRS menüsündeyken hoparlör kapalı; paketler yine çözülür.

#### AT V0-6 · 19.10.2026 · gerçek telsizde denendi

- Yeni: APRS alma: duyulan istasyonların çağrı işaretleri ekranda (~4 s).
- Yeni: Son duyulan 8 istasyonun listesi: * bir eskiye, # bir yeniye.

#### AT V0-4 · 19.10.2026 · gerçek telsizde denendi

- Yeni: Telsiz APRS modundayken kapanırsa açılışta yine APRS modunda başlar (EEPROM 0x732).

#### AT V0-3 · 19.10.2026 · gerçek telsizde denendi

- Düzeltme: APRS gönderme: paket yerine boş taşıyıcı gidiyordu (FX806 modülatörü vericide susturuluyordu).

#### AT V0-2 · 19.10.2026 · gerçek telsizde denendi

- Değişiklik: SAYI, HESAP ve OYUN örnek menüleri bu firmware'den çıkarıldı (yer açmak için; ayrı HEX'lerde duruyor).

#### AT V0-1 · 19.10.2026 · gerçek telsizde denendi

- Yeni: İlk numaralı sürüm: açılışta 5 s yATOLYE, ardından AT V0-1.
- Yeni: FREKANS menüsü: kanal tablosundan bağımsız alma / gönderme frekansı (VFO); ekranda VR / VT.
- Yeni: APRS menüsü: PTT'de ya da ayarlanan aralıkta 144,800 MHz'de konum paketi; ayarlar EEPROM'da (0x6E0).
- Yeni: Seçmeli çağrı (Select-5) kaldırıldı.

## ASELSAN orijinal firmware'ler

- **YA4000S.HEX**: ASELSAN orijinal, STANDART VERSIYON (AS V2-30, 23.11.1999). Özel firmware'lerimizin tabanı; gerçek telsizden okunan flash ile aynı.
- **YA4000D.HEX**: ASELSAN orijinal, DENIZ EL TELSIZI OPSIYONU. Standart sürümle aynı kod; yalnız açılıştaki opsiyon ayarı farklı.
- **YA4000K.HEX**: ASELSAN orijinal, KONVANSIYONEL VERI OPSIYONU. Standart sürümle aynı kod; veri (sipariş / yazıcı) bölümü açık.
- **YA4000P.HEX**: ASELSAN orijinal, DISPATCH-TELSIZ OPSIYONU. Standart sürümle aynı kod; yalnız açılıştaki opsiyon bayrağı farklı.
- **YA4000R.HEX**: ASELSAN orijinal, KIRSAL ROLE TELSIZ OPSIYONU. Standart sürümle aynı kod; yalnız açılıştaki opsiyon bayrağı farklı.
- **YA4000V.HEX**: ASELSAN orijinal, VOX DEVREDE TELSIZ OPSIYONU. Standart sürümle aynı kod; yalnız açılıştaki opsiyon bayrağı farklı.
- **YA4000Y.HEX**: ASELSAN orijinal, KABLOSUZ SES YAYIN OPSIYONU. Standart sürümle aynı kod; yalnız açılıştaki opsiyon ayarı farklı.
- **YA4000E.HEX**: ASELSAN orijinal "radio" derlemesi (AS V2-51, 12.04.2002). Ayrı kaynak: RAM yerleşimi farklı, sipariş / yazıcı modülü genişletilmiş, pil yönetimi ve taramada TKS beklemesi değişmiş. Özel firmware yamalarımız bu sürüme uymaz.
