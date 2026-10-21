# Sürüm notları

*Bu dosya `ozel_firmware/degisiklikler.json`'dan üretilir (`make -C disasm sim`); elle değiştirmeyin.*
Aynı notlar sitede: [surumler.html](https://barisdinc.github.io/ASELSAN_4000/surumler.html); programlama sayfası firmware seçilince de gösterir.

## yATOLYE özel firmware'i (`YA4000S_FREKANS_YATOLYE.HEX`)

Güncel sürüm: **AT V0-16** (21.10.2026)

Orijinal YA4000S (AS V2-30) üzerine FREKANS (VFO) menüsü, APRS gönderme / alma, seçmeli çağrı kaldırıldı, açılışta 5 s yATOLYE.

### Özellikler

- **FREKANS (VFO) menüsü** (AT V0-1 ile geldi): Kanal tablosundan bağımsız alma ve gönderme frekansı tuşlarla girilir. Ekranda kanal numarası yerine VR (alma) / VT (gönderme); PTT'de gönderme frekansı görünür.
- **APRS gönderme** (AT V0-1 ile geldi): APRS menüsünde PTT ile ya da ayarlanan aralıkta (PTT'siz) 144,800 MHz'de 1200 baud konum paketi. Çağrı işareti, konum, simge, yorum ve aralık EEPROM'da (0x6E0); programlama sayfasının APRS sekmesinden düzenlenir.
- **APRS alma: duyulan istasyonlar** (AT V0-6 ile geldi): APRS menüsündeyken çözülen paketin gönderen çağrı işareti ~4 s ekranda. Son 8 istasyon listesi: * bir eskiye, # bir yeniye.
- **APRS'te sessiz hoparlör** (AT V0-7 ile geldi): APRS menüsündeyken bip, uyarı ve hoparlör kapalı; alma çözücüsü çalışmaya devam eder.
- **APRS modunu hatırlama** (AT V0-4 ile geldi): Telsiz APRS menüsündeyken kapatılırsa açılışta yine APRS'te başlar (EEPROM 0x732).
- **Tarama** (AT V0-16 ile geldi): Orijinal tarama (LIST menüsü + p tuşu) çalışır; TKS'li kanallarda ton için daha uzun bekleme (ASELSAN V2-51 / V2-54'ten).
- **Seçmeli çağrı yok** (AT V0-1 ile geldi): Select-5 (CCIR / ZVEI / EEA) kodu kaldırıldı; yer yeni özelliklere açıldı. # ile çağrı ve PTT kimliği gönderilmez.
- **Açılış ekranı ve sürüm** (AT V0-1 ile geldi): Açılışta 5 s yATOLYE, ardından AT V0-x sürüm numarası. Sürüm, tarih ve metin ROM başlığında; T4000 ve yükleme sayfası da gösterir.

### Sürüm geçmişi

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
