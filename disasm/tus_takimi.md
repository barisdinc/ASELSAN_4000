# Tuş takımı ve panel düğmeleri — YA4000S (A tipi kart)

> Özel firmware (AT V0-31'den) `p` ve `q`'yu değiştirir: `p` kısa = güç yüksek / düşük, `p` uzun = tarama, `q` kısa = monitör, `q` uzun = VFO'yu hafıza kanalına kaydet (AT V0-33, `kanal_kayit.inc`). Ayrıntı: `../ozel_firmware/README.md`, "Güç ve monitör tuşları". AT V0-32'den ALT MENU'de PAROLA ve MSR RSSI yok; kayıtlı parola açılışta silinir, RSSI göstergesi açılmaz (`parola_rssi_kaldir.inc`). Aşağısı orijinal firmware.

Koddan çıkarıldı ve simülatörde denendi (`sim/`, `make test-sim`). "Doğrulandı" yazanlar simülatörde çalıştırıldı; diğerleri yalnızca koddan okundu.

Tuşlar: `0–9 * #` ve işlev tuşları `m s p q` (ROM 0x31EC tablosu `*0#q 789p 456s 123m`). Tuş takımı EE[0x014] bit 3 ile açılır.

## Normal ekranda

| Tuş | İş | Koşul | Durum |
|---|---|---|---|
| PTT basılıyken `0–9 * #` | DTMF tonu | | Doğrulandı |
| `m` | Menü (aşağıda) | | Doğrulandı |
| `p` | Taramayı başlat / durdur. Ekranda **SCAN**; liste boşsa **NO LIST** | Liste dolu olmalı | Doğrulandı |
| `#` | Seçmeli çağrı ile arama ("C …") | EE[0x014] bit 2, kanalda seçmeli çağrı açık | Doğrulandı |
| `q` | Monitör (B tipi kartta); A tipinde etkisiz | | |
| rakamlar | Kanala git: iki hane (`0 5` → `CH 05`) | Kanal anahtarı **16** konumunda (0x46 b7..4 = 0 → RAM 0x0206 = 0x10 → 0x023A bit 10) ve EE[0x014] bit 3 | Doğrulandı |
| `s`, `*`, rakamlar | Etkisiz | Kanal anahtarı 1–15 | Doğrulandı |

## Menü (`m` sonraki öğe, `s` gir / seç)

Sıra tablosu ROM 0x3188, `s` tablosu 0x319C (4 bayt: sonraki durum + 24 bit işleyici), ekran şablonları 0x2DE6 (Türkçe 0x2E86, EE[0x018] bit 7). Menü durumu RAM 0x0227, geçerli işleyici RAM 0x02DF/0x02E1. Menüye yeni öğe ekleme örneği: `../ozel_firmware/ornekler/sayi_ters.asm` (ayrıntı `../ozel_firmware/README.md`).

```
normal ──m──▶ [CHN SCAN] ──m──▶ LIST ──m──▶ [RADIO] ──m──▶ [PRINTER] ──m──▶ [SUB MENU] ──m──▶ normal
```

- **CHN SCAN** ve **RADIO**: yalnız B tipi kartta (RADIO ayrıca EE[0x000] bit 4).
- **PRINTER** → `s` → PRINTER / DATE / TIME: EE[0x014] bit 1 ve B tipi kart.
- **SUB MENU**: EE[0x018] bit 5.

### LIST — tarama listesi (doğrulandı)

`m` (LIST) → `s` → ekranda `CH 01 -`.

| Tuş | İş |
|---|---|
| rakamlar | Kanala git (`0 5` → `CH 05`) |
| `m` | Seçenek: `-` (listede değil) → `EXIT` → `+` (listede) → `P1` → `P2` (öncelikli kanal 1 / 2) → `-` |
| `s` | Seçeneği kaydet ve sonraki kanala geç; `EXIT` görünürken `s` = menüden çık |

Örnek — kanal 1, 2 ve 5'i listeye ekle, çık, taramayı başlat:

```
m s  m m s  m m s  0 5 m m s  m s   p
```

**Tarama:** Liste kanallarını sırayla dinler. Susturma açılınca (kanalda sinyal, P57 = 1) o kanalda durur ve ekranda kanal adı görünür; sinyal gidince devam eder. `p` ile durur. P1/P2 öncelikli kanallarına aralıklarla bakılır (F_00EEE9). Zamanlamalar EE[0x01C] alt 4 bit ve EE[0x025].

### SUB MENU / ALT MENU (doğrulandı)

Menüde yalnızca **EE[0x018] bit 5** (0x20) açıksa görünür (PC'deki programlama yazılımıyla ayarlanan bir seçenek; P4000'de mi T4000'de mi olduğu doğrulanmadı). EE[0x018] bit 7 dili seçer (0 = İngilizce, 1 = Türkçe: ALT MENU / ISIK / UYARI / GOSTERGE / PAROLA). EEPROM elle değiştirilirse 0x018–0x534 bloğunun sağlama toplamı (0x535–0x536) da düzeltilmeli, yoksa açılışta `ARZ PRG` (simülatörde "EEPROM / kanallar" sekmesindeki **Düzelt** düğmesi, `eeFixSums`). Cihazdan okunan EEPROM'da EE[0x018] = 0x01 olduğu için alt menü görünmüyor.

`s` ile girilir, `m` ile öğeler arasında:

| Öğe | `s` |
|---|---|
| LIGHT / ISIK | Arka ışık aç `*` / kapa `-` (0x023C bit 12) |
| ALERT / UYARI | Uyarı sesi aç / kapa (0x023A bit 11) |
| DISPLAY / GOSTERGE | `s` → INVERT ↔ NORMAL (TR TERS ↔ NORMAL) (`m` ile), ekran ters / normal |
| PASSWORD / PAROLA | `s` → `NP` (TR `YP`) yeni parola: rakamlar, `s` kaydeder. EEPROM 0x54E–0x551 (boş = FF); kaydederken 0x552–0x553 de değişiyor (kontrol değeri, formülü çözülmedi). Parola varsa telsiz açılışta `PASSWORD` / `PAROLA` gösterir, `PR ----` ile 4 hane ve `s` ister; yanlışsa yeniden sorar. Kılavuz (1.9.8.3.3): parolayı silmek için eski parola girilir, yeni parola istenince 5 kez `0` yazılır |
| MSR RSSI | RSSI kalibrasyonu (aşağıda). Yalnızca servis kodu girildikten sonra ya da RSSI göstergesi açıkken görünür (0x01EB < 3) |

### RSSI göstergesi ve kalibrasyonu (doğrulandı)

Kalibre edildiğinde telsiz, susturma açıkken (kanalda sinyal, P57 = 1) kanal adı yerine sinyal gücünü gösterir: `-115dBm` gibi, 5 kalibrasyon noktası arasında 5 dB adımlarla, dışında da uzatarak (−60 … −135). Kodu F_00604C, ölçümü F_004DCC (AN5).

1. **Servis kodu** (F_004596, ROM 0x317E tablosu `25970 / 10992`): SUB MENU → PASSWORD.
   - Parola yokken: `NP` ekranında `25970` (0x01EB 4 → 5; 25970 parola olarak da kaydedilir).
   - PASSWORD → `OP 25970` → `NP 10992` (5 → 6).
   - PASSWORD → `OP 10992` → `NP 00000` (parola silinir; 0x01EB = 1 ya da 2).
   - Herhangi bir adımda yanlış kod girilirse 0x01EB = 3 olur ve gösterge kapanır.
   - Tuş dizisi (parola boşken, `m m s` = SUB MENU ve sonra `m m m s` = PASSWORD):
     ```
     m m s m m m s 2 5 9 7 0
     m m s m m m s 2 5 9 7 0 1 0 9 9 2
     m m s m m m s 1 0 9 9 2 0 0 0 0 0
     ```
2. **MSR RSSI** artık PASSWORD'dan sonra görünür. `s` → ` e80dBm`: girişe −80 dBm sinyal verilir ve `s` iki kez basılır. Aynı işlem ` e90dBm`, `e100dBm`, `e110dBm` ve `e120dBm` için tekrarlanır.
   - Her ölçüm bir öncekinden küçük olmalı; değilse uyarı sesi çalar ve aynı seviye yeniden istenir.
   - Bitince AN5 değerleri **EE 0x571–0x575**'e, toplamları büyük-endian olarak **0x576–0x577**'ye yazılır. Ardından gösterge açılır (0x01EB = 0).
3. **Kalıcılık:** Gösterge açıkken kapanışta **EE 0x54B bit 6** yazılır. KULLANICI sağlaması 0x552–0x553, 0x54B–0x551 baytlarının toplamıdır. Açılışta bu bit 1 ve 0x571 tablosunun sağlaması doğruysa gösterge kendiliğinden açılır.
   - Bit 1 ama tablo bozuksa gösterge kapalı kalır (0x01EB = 2).
   - Bit 0 ise tablo dolu olsa da gösterge kapalıdır.

## `#` — seçmeli çağrı ile arama (doğrulandı)

EEPROM: 0x01B alt 4 bit = toplam hane, 0x01C üst 4 bit = kullanıcının girdiği hane sayısı (N); baştaki haneler sabit önek (kendi filo numarası). EE[0x014] bit 2 ve kanalda seçmeli çağrı açık olmalı.

- `#` → `C GG___` (G: grup / tekrar hanesi, `_`: girilecek hane)
- Rakamlar haneleri doldurur; `*` bir G hanesi koyar.
- N hane girilince telsiz **kendiliğinden TX'e geçip** çağrıyı gönderir (0x0244 bit 0 → F_005E56).
- `m` ya da birkaç saniye beklemek iptal eder.

Örnek: EE[0x01B] = 5, EE[0x01C] = 0x30 → `# 1 2 3` → CCIR "00123" gönderilir.

## Panel düğmeleri (genişletici 0x46, alt 4 bit)

| Bit | İş | Durum |
|---|---|---|
| 3 | **Monitör**: ≈1 s bas → susturma ve TKS atlanır, hoparlör (P46) açılır; tekrar bas → kapanır (0x023A bit 13, F_004FF6) | Doğrulandı |
| 2 | **Güç seviyesi** yüksek / düşük: ≈1 s bas (EE[0x014] bit 7 açıksa kısa basış) → 0x023C bit 4; F_00CEF3 güç tablosu indeksini seçer (0x0277.., 3 seviye). Kısa basış: EE[0x000] bit 2 açıksa 0x023C bit 2 (çözülmedi) | Bayrak doğrulandı; güç farkı EEPROM kalibrasyonuna bağlı, varsayılan EEPROM'da seviyeler eşit |
| 0, 1 | Firmware'in sürdüğü çıkışlar (düğme değil) | |
| 4–7 | Kanal anahtarı (16 − kanal) | Doğrulandı |

Seri porttaki uzak tuş komutu (`0A key`, `seri_protokol.md`) aynı işlevleri çağırır: `g` = güç (b2), `n` / `o` = b3 / F_004FF6.

## Simülatörde

- Ekrandaki tuşlar ya da klavye: `0–9 * # m s p q`, boşluk = PTT.
- "Güç (b2)" / "Monitör (b3)" düğmelerini ≈1 s basılı tutun.
- Taramayı denemek için "RF ve ses" sekmesinde **Taşıyıcı var** (P57 = 1) kutusunu açıp kapatın.
- Seçenekler "EEPROM / kanallar" sekmesinde (0x014, hex düzenleyiciyle 0x018, 0x01B, 0x01C).
