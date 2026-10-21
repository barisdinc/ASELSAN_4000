# Özel firmware

Orijinal YA4000S yazılımına eklenen özellikler. Her örnek önce simülatörde (`disasm/sim/`), sonra cihazda denenir.

| Dosya | İçerik | Nasıl üretilir |
|---|---|---|
| `YA4000S_MERHABA.HEX` | Açılışta 4 mesaj | `disasm/tools/acilis_mesaji.py` |
| `YA4000S_SAYI.HEX` | Menüde **SAYI** öğesi: girilen sayıyı yazar, `s` ile tersini gösterir | `ornekler/sayi_ters.asm` |
| `YA4000S_HESAP.HEX` | SAYI + **HESAP**: dört işlemli hesap makinesi | `ornekler/hesap.asm` |
| `YA4000S_OYUN.HEX` | SAYI + HESAP + **OYUN**: sayı tahmin oyunu (rastgele sayı, sesler) | `ornekler/oyun.asm` |
| `YA4000S_FREKANS.HEX` | **FREKANS**: tuş takımından alma ve gönderme frekansı girip telsizi ayarlama (VFO modu; PTT'de ekranda gönderme frekansı) + **APRS**: PTT'de 144,800 MHz'de sabit konum paketi (1200 baud AFSK, AX.25); almada duyulan istasyonların çağrı işaretleri ekranda. **Seçmeli çağrı (Select-5) tamamen kaldırıldı**, ~4,1 KB flash boşaldı. SAYI / HESAP / OYUN bu firmware'de yok (ayrı örnek HEX'ler olarak duruyorlar), ~1,9 KB daha boşaldı | `ornekler/frekans.asm` |
| `YA4000S_FREKANS_YATOLYE.HEX` | FREKANS ve APRS'in tamamı (seçmeli çağrı kaldırılmış) + açılışta 5 saniye **yATOLYE** (flash'a gömülü, EEPROM / programlayıcı değiştiremez), ardından kendi sürüm yazımız ("AT V0-1" …) | `ornekler/frekans_yatolye.asm` |

## Sürüm, tarih ve metin

`make -C disasm ozel` her `ornekler/*.asm` çıktısının ROM başlığını (0x010000) `surum.json`'a göre yazar (`tools/yama.py --surum`):

- **Sürüm:** "AT V0-1"den başlar. Bir dosyanın içeriği son derlemeden farklıysa bir artar (V0-1 … V0-99, sonra V1-1). 10, 20, … 90 atlanır (V0-9'dan sonra V0-11): `F_00E0E1` sürüm baytını onlar hanesi + sıfır değilse birler hanesi olarak yazıyor, 10 ekranda "1" görünürdü. Kaynak değişmeden yeniden derlemek sürümü ve tarihi değiştirmez. Her dosyanın son sürümü, tarihi ve içerik imzası `surum.json`'daki `kayitlar`da durur. Bu dosyayı da commit edin.
- **Neler değişti:** `degisiklikler.json`'a her yeni sürüm için kısa notlar yazılır (`surum`, `tarih`, `durum`: `gercek` / `sim`, `degisiklikler`; gerçek telsizde çalışmayıp geri alınan sürüm `geri_alindi`). Programlama sayfası (`yukle.html`) firmware seçilince bunları gösterir; telsiz bağlıysa yalnız telsizdeki sürümden bu yana gelenleri. `make -C disasm sim`, `surum.json`'daki güncel sürümün notu yoksa durur.
- **Tarih:** O sürümün derlendiği gün.
- **Metin:** `metin` alanı: "yATOLYE 2026 - 4000/4100 SERISI TELSIZ YAZILIMI (BarisDInc)". En çok 114 karakter, ASCII.

Bu başlık seri yükleyicinin 2. adımında ("Sürüm", "Tarih", "Metinler") ve T4000'de görünür. Açılışta orijinal firmware'in sürüm ekranı (F_00E0E1) da aynı alandan okur. YATOLYE yapısında bu ekran yATOLYE'den sonra gelir.

Kaynak düzeni (`ornekler/`):

| Dosya | İçerik |
|---|---|
| `menu_altyapi.inc` | Ortak: firmware'in menü değişkenleri, sabitler, menü tablolarını bank 1'e taşıyan 18 yama |
| `sayi_ters.inc`, `hesap.inc`, `oyun.inc`, `frekans.inc`, `aprs.inc` | Her örneğin işleyicileri |
| `sayi_ters.asm`, `hesap.asm`, `oyun.asm`, `frekans.asm` | Bir firmware = altyapı + menü tabloları (hangi öğeler, hangi sırada) + örneklerin `.inc` dosyaları |
| `acilis_yatolye.inc` | Açılış ekranı: F_00E0E1'e (sürüm yazısı) JMPL, 5 s "yATOLYE"; LCD fontunda olmayan küçük `y` için F_00828C'ye segment yaması (0x2546) |
| `frekans_yatolye.asm` | `frekans.asm` + `acilis_yatolye.inc` |

**Menü sırası** (FREKANS ve FREKANS_YATOLYE; Türkçe EE[0x018] bit 7, ALT MENU bit 5, ör. EE[0x018] = 0xA1):

```
KANAL 01 -m-> LISTE -m-> FREKANS -m-> APRS -m-> ALT MENU -m-> KANAL 01
```

Bir öğeye `s` ile girilir; içindeyken `m` normal ekrana döner. ALT MENU kapalıysa (bit 5 = 0) APRS'ten sonra doğrudan normal ekran gelir. İngilizce menüde başlıklar FREQ / APRS / SUB MENU. SAYI / HESAP / OYUN yalnız kendi örnek HEX'lerinde (`YA4000S_SAYI/HESAP/OYUN.HEX`) var; aşağıdaki kullanım bölümleri onlar için.

```
cd disasm
make ozel        # örnekleri derle
make test-ozel   # simülatörde dene; seri yükleyiciyle yazmayı da dener
make sim         # simülatörün firmware listesine ekle (index.html → firmware seçimi)
```

## SAYI kullanımı

| Tuş | İş |
|---|---|
| `m` `m` | Menü: LIST → **NUMBER** (Türkçe menüde **SAYI**) |
| `s` | Moda gir, ekranda `_` |
| `0–9` | Hane ekle, en çok 8. Fazlasında bip (ALERT açıksa) |
| `*` | Son haneyi sil |
| `#` | Temizle |
| `s` | Tersini göster (`1234` → `4321`), tekrar `s` → düz |
| `m` | Çık, normal ekran |

## HESAP kullanımı

| Tuş | İş |
|---|---|
| `m` `m` `m` `s` | Menü: LIST → NUMBER → **CALC** (Türkçe **HESAP**), ekranda `0` |
| `0–9` | Sayı gir, en çok 8 hane |
| `*` | İşlem. Bir kez basınca `+`, art arda basıldıkça `+ − * /` arasında döner. Bekleyen işlem ekranın en solunda görünür (`+     12`) |
| `#` | Eşittir |
| `s` | Temizle |
| `m` | Çık |

Örnekler:
- `1 2 * * * 3 #` → `36` (12 × 3).
- `7 * * 9 #` → `-2`.
- `1 0 * 5 * * 3 #` → `12`: ikinci `*`'da önce 10 + 5 hesaplanır, sonra işlem `−` olur.
- Sonuçtan sonra `*` sonuçla devam eder, rakam yeni hesap başlatır.

Aritmetik 32 bit işaretli. Gösterilebilen aralık −9.999.999 … 99.999.999. Bu aralığın dışına çıkan sonuç, çarpma taşması ve sıfıra bölme `ERROR` (Türkçe menüde `HATA`) gösterir; `s` ile silinir. Bölme tamsayı bölmesidir, sıfıra doğru keser (−4 / 3 = −1).

İçinde örnek yazarken işe yarayacak parçalar:
- **Sayıyı ekrana yazma** (`HS_EKRAN`): 10'un kuvvetleriyle çıkarma; işaret ve baştaki sıfırlar dahil.
- **32 bit çarpma ve bölme:** kaydır-topla ve kalanlı bölme.
- **Ondalık giriş:** 10x = 8x + 2x.

## OYUN kullanımı

Telsiz 1–999 arasında bir sayı tutar.

| Tuş | İş |
|---|---|
| `m` ×4, `s` | Menü: LIST → NUMBER → CALC → **GAME** (Türkçe **OYUN**), ekranda `___` |
| `0–9` | Tahmin, en çok 3 hane |
| `*` | Son haneyi sil |
| `#` | Gönder |
| `s` | Yeni oyun |
| `m` | Çık |

`#` sonrası ekran:
- `500 ^  1`: tutulan sayı **daha büyük**. Tiz bip, 1500 Hz.
- `250 v  2`: tutulan sayı **daha küçük**. Pes bip, 400 Hz.
- `WIN    9` (Türkçe `BULDU  9`): bildin, do-mi-sol-do melodisi çalar. Sonra herhangi bir tuş yeni oyun başlatır.

İçinde sonraki örneklerde işe yarayacak parçalar:
- **Rastgele sayı.** TB2 firmware'in sürekli sayan 1 ms zamanlayıcısı. Her tuşta TB2 16 bitlik bir LFSR'ye karıştırılır. Kullanıcının basma anı milisaniyenin altında rastgele olduğu için sayılar öngörülemez. Aralık reddetmeyle seçilir.
- **İstenen frekans ve sürede ses (`OY_NOTA`).** Firmware'in bip yordamı `F_BEEP` tek notalık bir kodla çağrılır; ALERT ayarını ve zamanlayıcıyı o kurar. Ardından frekans `TA0 = 2016000 / f` ve süre `BEEP_SURE` (0x0214, 10 ms birim) üzerine yazılır. Firmware'in kendi tablosundaki (ROM 0x2C06) notalar en çok 30 ms sürer. ALERT kapalıysa oyun da sessizdir.
- **Bekleme:** `F_DELAY` (Y = ms). Melodi süresince (yaklaşık 0,7 s) işleyici bekler; kesmeler çalışmaya devam eder.

## FREKANS kullanımı (VFO modu)

Dört adım: **alma (RX)** frekansı, **gönderme (TX)** frekansı, **alma TKS (CTCSS) tonu**, **gönderme tonu** (tonlar AT V0-17 ile).

| Tuş | İş |
|---|---|
| `m` `m` `s` | Menü: LIST → **FREQ** (Türkçe **FREKANS**), ekranda `___ ____` (alma frekansı) |
| `0–9` | Frekans, noktasız. `156525` = 156,525 MHz. 12,5 kHz adımlar için 7 hane: `1565125`. Ekranda `156 5125`; 3. haneden sonraki boşluk ondalık noktadır |
| `*` | Son haneyi sil |
| `s` | Girişi temizle |
| `#` (1. adım) | Alma frekansını hafızaya al (onay sesi). Ekranda `GONDERME` (İngilizce `TX FREQ`): şimdi gönderme frekansı girilir |
| `0–9`, `*` | Gönderme frekansı, aynı şekilde: `160 65--` (çizgiler gönderme frekansının girildiğini gösterir) |
| `#` (2. adım) | Gönderme frekansını hafızaya al. Ekranda `RX TONE` (Türkçe `ALMA TON`) |
| `#` (2. adımda hiç hane yokken) | Simplex: gönderme frekansı = alma frekansı |
| `0–9` (3. adım) | Alma tonu, Hz, en çok 4 hane: ekranda `RT 885_`. Son hane ondalık: `885` = 88,5 Hz, `2503` = 250,3 Hz. 600'den küçük sayı tam Hz sayılır: `67` = 67,0, `100` = 100,0. En yakın standart ton seçilir; 1 Hz içinde ton yoksa `ERROR` |
| `#` (3. adım) | Alma tonunu al. Boşken `#`: alma tonu değişmez (son kanalın ya da önceki VFO ayarının tonu); `0 #`: ton yok. Ekranda `TX TONE` (`GON TON`) |
| `0–9`, `#` (4. adım) | Gönderme tonu, aynı şekilde (`TT 100_`). `#` **ayarlar** (onay sesi): telsiz almada 1. frekansı ve alma tonunu, gönderirken 2. frekansı ve gönderme tonunu kullanır. Boşken `#`: alma tonuyla aynı; `0 #`: ton yok |
| `*` (giriş boşken) | Bir önceki adıma dön (4 → 3 → 2 → 1) |
| `m` | Çık. Telsiz yeni frekanslarda kalır |

Örnekler:
- `156525#160650##` `#` → alma 156,525 MHz, gönderme 160,650 MHz, tonlar değişmez.
- `156525####` → ikisi de 156,525 MHz, tonlar değişmez.
- `145600#145000#0#885#` → röle: alma 145,600 MHz tonsuz, gönderme 145,000 MHz 88,5 Hz tonla.
- `156525#160650#885##` → alma ve gönderme 88,5 Hz.

**Tonlar:** Firmware'in 50 tonluk tablosu (67,0 … 254,1 Hz; kanal kaydındaki sıra numarası) kullanılır. Alma tonu `$0375`'in, gönderme tonu `$037C`'nin alt 6 bitine yazılır; üst bitler kanalın bayrakları olarak kalır. Gönderme tonu PTT'de hep gönderilir. Alma tonunda firmware tonu çözer (0x023A bit 12), ama hoparlörü yalnız tonla açması orijinal firmware'deki gibi panelden açılan TKS susturmasına bağlıdır (0x023E bit 0).

**VFO modunda ekran:** Almadayken alma frekansı (`156 5250`), PTT'ye basılıyken gönderme frekansı (`160 6500`) görünür; PTT bırakılınca alma frekansına döner. Bu, normal ekranda da ayardan sonra açık kalan FREKANS menüsünde de olur. Başka bir menüdeyken ekran değiştirilmez, yalnız kanal adı güncellenir. İki haneli kanal numarası yerine almada **VR**, gönderirken **VT** yazar. 7 parçalı hanelerde V `U`, R `r`, T `t` biçiminde görünür. Kanala dönünce kanal numarası geri gelir.

FREKANS açıkken firmware'in menü zaman aşımı (yaklaşık 4 s tuşa basılmazsa normal ekrana dönme) kapalıdır; çıkış yalnız `m` ile. İlk sürümde bu yoktu: gerçek telsizde 1. adımdan sonra 4 s beklenince ana ekrana dönülüyor, girilen frekans kayboluyordu.

Kanal anahtarı çevrilince (ya da LIST ile kanal seçilince) telsiz o kanala döner ve VFO modu biter. Frekanslar yalnızca RAM'de tutulur: EEPROM'a yazılmaz, kapatınca kaybolur.

Bu durumlarda `ERROR` (Türkçe `HATA`) gösterilir ve bip çalar; sonraki tuş girişe döner (aynı adımda kalınır):
- 6 haneden az giriş;
- 2,5 kHz ızgarasında olmayan frekans;
- firmware'in temsil edebildiği aralığın dışı: bant tabanı (EEPROM 0x58C) … taban + 40,95 MHz;
- telsiz göndermedeyken 2. adımda basılan `#`. Firmware gönderirken tuşları zaten yok sayıyor; bu denetim yedek olarak duruyor.

Nasıl çalışıyor:
- **Sentezör kelimeleri.** Firmware'in kanal hesabının aynısı burada da yapılır: taban, ±45 MHz ara frekans, ön bölücü ve referans, firmware'in açılışta EEPROM'dan kurduğu RAM değişkenlerinden okunur (formül `frekans.inc` başında). VHF ve UHF'de, üç adım türünde, simülatörde firmware'in aynı frekans için EEPROM kanalından hesapladığı baytlarla birebir aynı çıkıyor.
- **Yükleme.** RX baytları 1. adımda, TX baytları 2. adımda, her biri kendi adımıyla (12,5 / 10 / 5 kHz) hesaplanır. İkisi birlikte geçerli kanalın sentezör kaydına (RAM 0x0370–0x037C; RX 0x0370, TX 0x0377) yazılır ve radyo durumu (0x0228) 1 yapılır. LIST menüsü kanal seçerken de aynısını yapar. Ana döngü RX'e yeniden girer ve sentezörü yükler; PTT'de TX kelimeleri 0x0377'den alınır.
- **Ekran kancası.** Firmware PTT'de ekranı yeniden çizmiyor. Telsiz durum makinesinin sonundaki `JSRL F_00E626` (0x00F84D) `VFO_KANCA`'ya çevrildi. Kanca her turda (boşta saniyede ~30, gönderirken ~1800 kez) çalışır ve şunları yapar:
  - VFO modunda mı, bakar: imza `'VF'` olmalı, sentezör kaydı da hâlâ bizim baytlarımızı taşımalı.
  - Radyo durumunu okur: 4–12 gönderme, diğerleri alma.
  - Yalnız geçişte kanal adını (0x024A) diğer frekansın metniyle değiştirir.
  - Ekranda eski metin duruyorsa (normal ekran ya da ayardan sonraki FREKANS menüsü) LCD'yi hemen yeniler.

  Sonra `F_00E626`'ya aynı yığınla devam eder; yazmaçlar korunur.
- **Kanal numarası kancası.** `F_00828C` normal ekranda kanal numarasını 0x0376'dan okur (0x008332, `LDA A,$0376 / SEM`). Bu iki komut `JSRL VFO_KNO`'ya çevrildi. VFO modunda değilse `VFO_KNO` aynı iki komutu yapıp döner. VFO modundaysa "VR" ya da "VT"nin 7 parçalı kelimesini `F_00828C`'nin yerel değişkenine ($04) koyar ve kanal numarası hesabının sonrasına (0x00835A) atlar. İki cam tipinin parça düzeni farklı olduğundan (RAM 0x023C bit 8; ROM 0x2D62 / 0x2D00) iki tablo kullanılır.
- **Bölme.** 32 bit bölmeler kaydır-çıkar yordamıyla (`FR_BOL`) yapılır; işlemcinin DIV komutunun 32 bitlik bölünen davranışına güvenilmedi.

**Gerçek cihazda dikkat.** Kabul edilen aralık (VHF tabanı 136 MHz ise 136–176,95 MHz) kılavuzdaki donanım bandından (146–174 MHz) geniştir. Bant dışında VCO kilitlenmeyebilir ve TX filtreleri uygun değildir. Ayrıca yalnızca izinli frekanslarda gönderme yapın. İlk denemeleri yapay yükle (dummy load) ve bant içindeki bilinen bir frekansla yapın. İstenirse VFO'da TX kapatılabilir ya da aralık kılavuzdaki banda daraltılabilir.

Telsiz bu modlarda da çalışır: işleyici her tuşta bir kez çağrılır ve hemen döner, ana döngü (alma, kanal anahtarı, PTT) sürer. Firmware'in menü zaman aşımı burada da geçerli. Bir süre tuşa basılmazsa normal ekrana döner.

## APRS kullanımı

Menüde FREKANS'tan sonra gelir (`m` ile **APRS** başlığına gelip `s`).

| Ekran / tuş | Ne olur |
|---|---|
| `s` (APRS başlığında) | Telsiz alma ve gönderme **144,800 MHz**'e geçer, TKS kapanır, hoparlör tamamen susar (kanal sesi, tuş bipleri ve uyarı tonları yok). Ekranda `APRS PTT` |
| (bekle) | APRS bölümünde otomatik gönderim aralığı varsa PTT'ye basılmadan o aralıkla paket gider; telsiz kendisi gönderip almaya döner |
| PTT | ~130 ms sonra paket gider (0,75 s): ekranda `APRS TX`, bitince `APRS OK`. PTT basılı tutulursa telsiz 144,800'de göndermeye (konuşmaya) devam eder |
| `m` | Çık: telsiz önceki kanalına / frekansına döner. Menüdeyken kanal anahtarı çevrildiyse yeni kanal kalır |

FREKANS'taki gibi APRS açıkken firmware'in ~4 s'lik menü zaman aşımı kapalıdır. Bant dışıysa (bant tabanı 144 MHz'in üstünde) ekranda `HATA` / `ERROR` yazar ve frekans değişmez.

**Ayarlar EEPROM'daki APRS bölümünde.** APRS'e her girişte EEPROM 0x6E0–0x731 okunur ve paket buradan kurulur. Bu alanı ne P4000 ne de orijinal firmware kullanıyor (bkz. `docs/p4000_eeprom.md`, "Boş alanlar"). Bölüm, simülatörün **EEPROM / kanallar** sekmesindeki ya da yükleme sayfasının 3. adımındaki **APRS** sayfasından yazılır. Gerçek telsizde sıra şöyle: 3. adımda EEPROM'u oku, **Programlama** bölümünün APRS sayfasında düzenle, **Telsize yaz**'a bas; 6. adım yalnız değişen sayfaları yazar, geri okuyup doğrular ve telsizi yeniden başlatır.

| Yer | Ne | Varsayılan | Not |
|---|---|---|---|
| 0x6E0 | `AP`, sürüm 1 | | imza |
| 0x6E4 (10) | Çağrı işareti ve SSID | `TC19MK-10` | 1–6 harf/rakam, isteğe bağlı `-0`..`-15` (AX.25 sınırı); küçük harf büyüğe çevrilir |
| 0x6EE / 0x6EF | Simge tablosu / kodu | `/` `b` (bisiklet) | herhangi bir yazılabilir karakter; sayfada sık kullanılanların listesi var |
| 0x6F0 (8) | Enlem `DDMM.mmN` | `3955.52N` | Ankara, Anıtkabir. Sayfa `39.92533` ya da `39 55.52 N` biçimini de çevirir |
| 0x6F8 (9) | Boylam `DDDMM.mmE` | `03250.22E` | `32.837` ya da `32 50.22 E` de olur |
| 0x702 (2) | Otomatik gönderim aralığı (s, büyük uçlu) | 0 (kapalı) | en az 10 s |
| 0x704 (43) | Yorum | `ASELSAN 4000` | 0 ile biter; küçük harf, noktalama, Türkçe harf (UTF-8, 2 bayt) serbest |
| 0x730 (2) | Sağlama | | 0x6E0..0x72F toplamı, büyük uçlu |

İmza, sürüm ya da sağlama tutmazsa (bölüm hiç yazılmamışsa) firmware ROM'daki varsayılanlarla (`AP_VARSAYILAN`, aynı yerleşim) gönderir: yeni yüklenen firmware ilk anda `TC19MK-10`, Anıtkabir, bisiklet ile çalışır.

Paket:

```
<çağrı>>APIZCI,WIDE1-1,WIDE2-1:!<enlem><tablo><boylam><kod><yorum>
TC19MK-10>APIZCI,WIDE1-1,WIDE2-1:!3955.52N/03250.22EbASELSAN 4000
```

Bölümdeki bir alan geçersizse (sayfa geçersiz değeri zaten yazmaz) ekranda `CAGRI -`, `ENLEM -` ya da `BOYLAM -` (İngilizce `CALL -`, `LAT -`, `LON -`) yazar ve paket gönderilmez; PTT normal gönderme yapar.

Çerçeve (en çok 95 bayt) RAM 0x740–0x79F'de kurulur. Bu alan firmware'in iki yığınının arasında kalıyor: APRS dahil ölçülen en derin kullanım çerçeve yığınında 0x6A8, donanım yığınında 0x82B; iki yanda da ~140 bayt pay var.

- `APIZCI` hedef alanı (tocall): APRS haritalarında cihaz / yazılım adı olarak görünür. APRS tocall listesine kayıtlı değil; bilinmeyen kimliği çoğu hizmet "bilinmeyen cihaz" olarak gösterir. Yol `WIDE1-1,WIDE2-1`.

**Ton üretimi.** Seçmeli çağrının donanım yolu kullanılır (bkz. `docs/calisma_analizi.md` bölüm 9):

- TB1 zamanlayıcı modunda örnekleme saati: 4,032 MHz / 420 = **9600 Hz**, kesme önceliği 6.
- Her TB1 kesmesinde TA1 tek atım darbesi tetiklenir; darbe genişliği o anki örnek. Çıkış P52 SÇ_TON → TD14 süzgeci → FX806 modülatör girişi.
- Örnek: 16 bit faz sayacı + ROM'daki seçmeli çağrı sinüs tablosu (0x28FA). Artım 1200 Hz için 8192, 2200 Hz için 15019. Ton değişirken faz kesintisiz. Bit başına 8 örnek = 1200 baud.
- Çerçeve APRS'e girişte RAM'de kurulur (`AP_HAZIRLA`): AX.25 adresleri, kontrol 0x03, PID 0xF0, bilgi alanı, CRC-16-CCITT FCS. Kesme her 8 örnekte sıradaki biti üretir: başta 45 bayrak (300 ms), çerçeve baytları LSB önce, beş 1'den sonra doldurma biti, 3 bayrak, NRZI. APRS bölümü firmware'in kendi EEPROM okuma yordamıyla (`F_0072EE`) okunur.
- FX806 ayarı `F_00E65B`'nin (seçmeli çağrı gönderme) başındakiyle aynı: `$029A` / `$029B` ve `F_00CB52`. Ton yolunu açar, mikrofonu kapatır.

**Bağlantılar (yamalar).**

| Adres | Orijinal | Yeni |
|---|---|---|
| 0x002028 `VEC_TB1` | `JMPL $01C627` | `AFSK_TB1`: paket gidiyorsa örnek üretir, yoksa orijinal kesmeye geçer (DTMF, alma) |
| 0x00EDA2 (durum 4) | PTT'de kimlik gönderilecek mi? (5 ya da doğrudan 8) | APRS menüsündeyse 5'e geçer; gecikme 4 × 32 ms (EEPROM'daki ton gönderme gecikmesi değil: senin telsizinde 1,5 s) |
| 0x00F4E4 (durum 5) | `JSRL F_00E65B` (seçmeli çağrı kimliği) | `APRS_PTT`: APRS menüsündeyse paketi gönderir, değilse hiçbir şey |
| 0x00E9FE (`F_00E9D2`) | susturma açıldı → `SEB #$40,P4` (hoparlör açık) | `APRS_HOPARLOR`: APRS menüsündeyse P46 = 0 |
| 0x00CC44 (`F_00CB52`) | ses kapalı mı kararı ($00) | `APRS_SUS_KARAR`: APRS menüsünde, almada ses hep kapalı sayılır. Seçmeli çağrı / TKS beklerken firmware'in yaptığı susturma: taşıyıcı varken FX806'ya normal alma ayarı yazılır, yalnız C-BUS `$10` bit 7 (ses çıkışı) kapalı kalır; ses yolu, dolayısıyla P66'daki çözücünün girişi açık. Tip A kartta ses yükselteci de kapanır. Vericide dokunulmaz. (AT V0-3'e kadar FX806'ya hep L_00CD3F'nin sessiz ayarı yazılıyordu: vericide modülatörü, almada çözücüyü susturuyordu; 0x00CC92 yaması kaldırıldı) |
| 0x0080AA `F_0080AA` | bip (tuş, onay, hata) | `APRS_BIP`: APRS menüsündeyse çalmadan döner |
| 0x008084 `F_008084` | uyarı tonu (TA0 → P50) | `APRS_UYARI`: APRS menüsündeyse başlatmaz |
| `VFO_KANCA` (0x00F84D) | | `APRS_KANCA`: menü zaman aşımını yeniler, hoparlörü kapalı tutar, menüden çıkılınca sentezör kaydını geri yükler; otomatik gönderim zamanı gelince PTT isteği kurar |

**Otomatik gönderim.** Zaman tabanı TB2 kesmesinin her 32 ms'de bir artırdığı 0x0457 sayacı. Süre dolunca `APRS_KANCA` RAM 0x0244'ün bit 2'sini kurar. Firmware'in PTT işleyicisi (`F_005E56`) bu baytın alt 6 bitini PTT isteği sayar (0x20 P47, 0x10 panel...); bit 2 hiçbir yerde kullanılmıyordu. Verici firmware'in kendi yoluyla açılır (güç rampası, sentezör), durum 4 → 5 → `APRS_PTT` paketi gönderir ve biti siler; verici kapanır. Gönderme yasağı / kanal meşgulü yüzünden ~3 s içinde paket gitmezse istek bırakılır. Elle PTT ile gönderimden sonra da aralık baştan başlar.

**Alma: duyulan istasyonlar (`ornekler/aprs_rx.inc`).** (AT V0-8..V0-14'teki kayan yazı / taşıyıcı koşulu / teşhis denemeleri gerçek telsizde çalışmadı; AT V0-7'nin çağrı işareti gösteren hâline dönüldü.) APRS menüsündeyken gelen paketler çözülür; sağlaması tutan her paketin gönderen çağrı işareti ekranda ~4 s görünür (`TA7W-7`; 6 harf + iki haneli SSID'de tire yazılmaz: `TB1ABC15`), sonra APRS ekranına dönülür. Son duyulan 8 farklı istasyon listede durur (en yeni başta, tekrar duyulan başa geçer); `*` bir eskiye, `#` bir yeniye (en yeniden sonra APRS ekranı). Liste boşken `*`: `EMPTY` / `BOS`. Liste RAM'de, telsiz kapanınca silinir.

- Donanım: seçmeli çağrı çözücüsünün yolu. Alma sesi FX806 süzgecinden ve TD12 sıfır-geçiş algılayıcısından P66'ya (TB1IN) kare dalga olarak gelir. TB1 darbe genişliği ölçme modunda (TB1MOD `$4A`, 504 kHz): her kenarda yarım periyot ölçülür (1200 Hz: 210, 2200 Hz: 115 sayım; bit 420 sayım).
- Çözücü (TB1 kesmesi `AR_TB1`): 30 sayımdan kısa darbeler sonrakine eklenir; ton = son iki yarım periyodun toplamı 325'ten büyükse mark (görev oranı kaymasından etkilenmez); bit saati PLL'i ton değişiminde hatanın 1/4'ünü düzeltir, bit ortasında örneklenir (NRZI); HDLC bayrak / doldurma / iptal; CRC bayt bayt (kalan `$F0B8`), yalnız adreslerin ilk 14 baytı saklanır.
- Firmware alma girişinde ve susturma değişince seçmeli çağrı çözücüsünü durdurur (`F_007CF6`: TB1 durur); `AR_ISLE` her turda TB1'in çalıştığına bakıp gerekirse yeniden kurar. Göndermede TB1 AFSK örnekleme saati olur, sonra alma yeniden kurulur.
- RAM: liste `$0500..$0537` (bu derlemede SAYI / HESAP / OYUN yok), durum `$05FC..$0629`.
- Simülatörde (`test_ozel.js`, `afsk.js`'in kodlayıcısıyla P66'ya kenar dizisi): iki haneli SSID, 3 adreslik yol, 2200 Hz yarı / iki kat genlik + kayma, baud ±%0,3, bant sınırlı %10 gürültü; bozuk sağlama gösterilmez; liste sırası, taşma, tekrar; gönderme sonrası alma; 3 s gürültüde bekçi köpeği yok; menü dışında alma yok. JS denemesinde gürültü tepe genliğin %15'inde paketlerin ~%90'ı çözüldü. Simülatör sayfasında **RF ve ses → APRS paketi** ile denenir.
- Hoparlör APRS'te kapalı (yukarıdaki `APRS_SUS_KARAR`), çözücü çalışır: firmware seçmeli çağrıyı da hoparlör böyle kapalıyken çözüyor.
- Gerçek cihazda bilinmeyen: P66'ya giden sesin seviyesi / süzgeci. Paketler hiç görünmezse susturmayı açıp (monitör) deneyin.

**APRS TR / DG / IG (`ornekler/aprs_ayar.inc`, AT V0-24).** Menüde APRS'ten sonra üç öğe; başlığın 8. karakteri `*` açık, `-` kapalı, `s` değiştirir (menüde kalınır):

| Öğe | Açıkken (APRS ekranında) |
|---|---|
| `APRS TR` | tracker: aralıkta (EEPROM 0x702) otomatik konum paketi |
| `APRS DG` | digipeater (aşağıda, AT V0-25) |
| `APRS IG` | igate: duyulan paketler seri porta (aşağıda) |

Ayar EEPROM 0x733'te: `$A0` | bit 0 TR, bit 1 DG, bit 2 IG (APRS bölümünün sağlamasına girmez). Geçersizse TR ve IG açık, DG kapalı. Açılışta ve her `s`'de okunur; programlama sayfasının APRS sekmesindeki "Çalışma" çerçevesinden de değişir (`p4000.js` `aprsAyarGet` / `aprsAyarSet`). RAM: `AP_AYAR` $04ED, `AP_AYAR_B` $04EE. IG kapalıyken çerçeve yine tampona alınır (istasyon detayı için), yalnız seri porta yazılmaz.

**Digipeater (`ornekler/aprs_digi.inc`, AT V0-25).** APRS DG açıkken, APRS ekranında duyulan her geçerli paketin yolundaki ilk tekrarlanmamış (H biti 0) adrese bakılır:

| Gelen yol | Giden yol |
|---|---|
| `WIDE1-1` | `ÇAĞRIM*` |
| `WIDE2-2` | `ÇAĞRIM*,WIDE2-1` |
| `WIDE2-1` | `ÇAĞRIM,WIDE2*` |
| `ÇAĞRIM` (doğrudan) | `ÇAĞRIM*` |

- Tekrarlanmaz: kendi paketimiz; yolda zaten `ÇAĞRIM*`; başka yollar (`WIDE3-3`, `WIDE2-3`, `TRACE`…); yol dolu (8 tekrarlayıcı) ya da paket 151 bayttan uzun; son 30 s'de tekrarlanan aynı paket (anahtar: hedef + gönderen + bilgi alanının CRC'si, yol hariç; 3 kayıt).
- Çağrı işareti EEPROM'daki APRS bölümünden (gönderdiğimiz paketteki ile aynı).
- Akış: kesme geçerli paketi tampona aldığında IG ya da DG açıksa tamponu kilitler (`AR_Y_DURUM` = 8). Ana döngü karar verir (`DG_ISLE`): IG açıksa paket önce olduğu gibi seri porta yazılır, satır bitince digi; değilse hemen. Çerçeve tamponda düzenlenir, FCS eklenir (`AR_Y_DURUM` = 7) ve tracker'ın PTT isteğiyle gönderilir (`APRS_PTT`). Verici ~3 s'de açılmazsa bırakılır; kendi gönderimimiz (PTT / tracker) bekleyen digiyi iptal eder.
- RAM: `DIGI_ISTE` $012F, `DG_SURE` $0130, tekrar kayıtları $0399–$039C ve $03AB–$03B0.
- **Dikkat:** digipeater bir verici istasyonudur; yalnız amatör telsiz lisansıyla, izinli frekansta (144,800 MHz) çalıştırın. İlk denemeleri yapay yükle yapın.

**İstasyon konumu (`ornekler/aprs_detay.inc`, AT V0-22).** Listede ya da yeni duyulan bir istasyon ekrandayken `s` sayfa değiştirir: çağrı işareti → enlem `4050 63N` → boylam `3109 39E` → çağrı işareti (boşluk ondalık nokta; boylam ≥ 100 ise `10200 5W`, son hane onda bir). Konumu olmayan pakette `KONUM -`. Boylamdan sonra **mesaj** sayfası (AT V0-23): konumdan sonraki yorum, durum (`>`), mesaj (`:ALICI    :`) metni ya da Mic-E yorumu; ilk 8 karakter, büyük harf, ekranda olmayan karakter boşluk (baştakiler atlanır); metin yoksa `MESAJ -`. Sayfa 8 s görünür.
- Konum paketin bilgi alanından: düz biçim `!` / `=` ya da zaman damgalı `/` / `@`. Konum belirsizliği boşlukları 0 sayılır. Sıkıştırılmış konum ve Mic-E çözülmez.
- 8 istasyon için 6 baytlık konum (0,01 dakika; yarımküre ve geçerlilik bitleri) ve 8 baytlık metin, kapatılan özelliklerden boşalan RAM parçalarında (adres tabloları `DET_TABLO`, `MET_TABLO`): metinler $02AB, $02B3, $02BB, $02C3, $0285, $0264, $0538, $0598; konumlar $02CB, $028D, $0123, $0129, $03C8, $03CE, $026D, $03A2. Kayıtlar listeyle birlikte kayar; konumu / metni olmayan bir paketle tekrar duyulan istasyonun eskisi kalır.
- Paket seri port tamponundan (`AR_CER`) okunur; kesme çerçeveyi tampona aldıysa `AR_DETAY` = 1 ($04FE). Sayfa $04FF.
- Aynı düzeltmeyle: aynı istasyon farklı yollu bir paketle duyulunca listeye ikinci kez girmiyor (SSID baytının yol bitleri karşılaştırmadan önce temizlenir).

**Seri port çıkışı (`ornekler/aprs_seri.inc`, AT V0-18).** APRS menüsündeyken sağlaması tutan her paket UART0'a (programlama kablosu; firmware'in hızı, normalde 9600 8N1) TNC2 metni olarak yazılır:

```
TA7W-7>APRS,WIDE1-1*,WIDE2-1:!4050.63N/03109.39E>hello
```

- Biçim APRS-IS / direwolf ile aynı: `gönderen>hedef,yol:bilgi` + CR LF. Yıldız yalnız tekrarlanmış (H biti) son tekrarlayıcıda. Bilgi alanındaki CR / LF boşluk olarak yazılır.
- Kesme yalnız çözülen baytı tampona koyar (`AR_CER`, $0740, 160 bayt; gönderme tamponu `AP_CER` ile ortak). Uzun paketlerde bilgi alanının ilk ~144 baytı yazılır.
- Metin ana döngüde firmware'in kendi gönderme kuyruğuyla (`F_007627`, 80 bayt) yazılır; kuyrukta yer yoksa beklenmez, sonraki turda sürer. Satır yazılırken gelen paket ekrana ve listeye düşer ama seri porta yazılmaz.
- Telsiz gönderme yaparsa yarım kalan satır CR LF ile kapatılır.
- Aynı kablo telsizin programlama protokolünü de taşır: yükleme sayfası bağlıyken APRS menüsünde durmayın.
- Durum değişkenleri $04FA–$04FD.

**Kapat-aç sonrası APRS'te kalma.** APRS menüsüne girince EEPROM 0x732'ye `A5`, menüden çıkınca (m, kanal anahtarı, ...) `00` yazılır; değer zaten aynıysa yazılmaz. Telsiz APRS'teyken kapatılırsa açılışta, normal ekran gelip alma başlayınca (menü 0, radyo durumu 2, bekleyen tuş yok) `APRS_KANCA` menü durumunu APRS başlığına alır ve 's' tuşunu bırakır; firmware'in tuş yolu kullanıcı seçmiş gibi `APRS_GIR`'i çağırır. Otomatik gönderim aralığı baştan sayılır. EEPROM yazması firmware'in kendi yordamıyla (`F_0073C6`) yapılır. RAM açılışta temizlenmediği için denetimin yapıldığı `$05F8` = `A55A` ile işaretlenir.

**Simülatörde doğrulama** (`test_ozel.js`, `disasm/sim/afsk.js`): TA1'e yazılan örnekler firmware'den bağımsız bir çözücüden geçirilir (1200/2200 Hz enerjisi, bit saati, NRZI, bit doldurma, CRC). Bölüm yokken ve sağlaması bozukken ROM varsayılanlarıyla, ayrıca farklı ayarlarla (küçük harfli çağrı, UTF-8 yorum, iki haneli SSID, güney / batı, `\` tablosu, 43 baytlık yorum, boş yorum) paket bayt bayt çözülür ve sayfanın gösterdiği paketle karşılaştırılır; altı hatalı içerikte (7 karakterli çağrı, SSID 16, bozuk konum) hata ekranı çıkar ve paket gitmez. Örnekleme 9600 Hz.

**Gerçek cihazda dikkat.**

- FX806'nın ön vurgusu 2200 Hz'i 1200 Hz'den daha yüksek verebilir. Alıcıda (direwolf, APRSdroid) çözülmezse iki ton için ayrı genlik gerekebilir.
- 144,800 MHz kılavuzdaki donanım bandının (146–174 MHz) altında. VCO kilitlenmeyebilir; ilk deneme yapay yükle.
- Paket yalnızca lisanslı çağrı işaretiyle, izinli frekansta gönderilmelidir.

## Tarama

**Çökme düzeltmesi (`ornekler/frekans.inc`, 0x00832C).** FREKANS kancası 0x008332'deki `LDA A,$0376 / SEM`'in üstüne `JSRL VFO_KNO` yazıyordu. Taramada (0x0240 bit 10) aynı yordam kanal numarasını 0x0369'dan alıp `JMP L_008335` ile SEM'e atlıyor. 0x8335 artık JSRL'nin son baytı (`$00` = BRK), bu yüzden `p` ile tarama başlayınca telsiz yeniden başlıyordu (AT V0-15 ve öncesindeki bütün FREKANS sürümleri). Şimdi 0x832C'de `LDA A,$0369 / SEM / BRA $8336` var.

**TKS bekleme süresi (`ornekler/tarama_tks.inc`).** ASELSAN'ın sonraki "radio" sürümlerinden (YA4000E V2-51, `0ya4000s_son.hex` V2-54) alındı. Taramada sinyal olan bir kanalda alma TKS tonu tanımlıysa ve TKS susturması açıksa (0x023E bit 0), firmware tonun çözülmesini `0x020A` sayacı kadar bekler (32 ms adım). Ton gelmezse sonraki kanala geçer. Süre `ton_periyodu / 0x3F0 + 1`'di (67 Hz: 15 adım = 480 ms, 250 Hz: 4 adım = 128 ms); yeni sürümlerde + 11 (67 Hz: 800 ms, 250 Hz: 448 ms). Ton geç çözüldüğünde doğru kanalın atlanması azalır. Kanca: 0x00EFF1 → `TT_BEKLEME` ($012080).

## YAZICI menüsünün kaldırılması

`ornekler/yazici_kaldir.inc` (AT V0-19). B tipi (araç / sabit) kartta YAZICI / PRINTER menüsü (PRINTER, DATE, TIME) seri porttan yazıcıya çağrı günlüğü açıyordu (RAM 0x0240 bit 8). Açıkken:
- firmware her kimlik gönderiminde ve alınan çağrıda UART0'a `4       ·01/01 00:02` gibi satırlar yazıyordu (`F_007809`; tarih / saat açılıştan beri sayılıyor),
- UART0 9600 yerine 2400 baud'a geçiyordu (`F_0099AD`, U0BRG 0x68).

Seçmeli çağrı (ANI) kaldırıldığı için kullanılmıyor ve APRS seri çıkışına karışıyordu. İki yama:
- 0x0055F6: `m` YAZICI'ya gelince yapılan seçenek denetimi koşulsuz atlama oldu; YAZICI menüde görünmez (sonraki öğe ALT MENU ya da normal ekran).
- 0x00908D: açılışta `F_008ECC` kayıtlı ayar baytından (EEPROM 0x54B bit 3, sağlaması 0x552–0x553) yazıcıyı açmaz.

Ayrıca `aprs_seri.inc` APRS menüsündeyken `F_007809`'u susturur. Seri port artık her zaman 9600 8N1.

## Yalnız Türkçe

`ornekler/dil_kaldir.inc` (AT V0-22). Menü / mesaj dili EEPROM 0x018 bit 7'den RAM 0x0340'a (`OPTIONS`) iki yerde yükleniyor: açılışta (0x00E45C) ve ayar denetiminde (`F_00DA32`). İkisinin ardından bit 7 kurulur (kancalar 0x00E460, 0x00DAB0; kod $012090): EEPROM'da İngilizce seçili olsa da firmware'in ve özel menülerin bütün ekranları Türkçe. İngilizce metinler ROM'da kalır, seçilmez. P4000 sayfasındaki "Dil" alanı bu firmware'de etkisiz.

## Kullanılmayan özelliklerin kapatılması

`ornekler/ozellik_kaldir.inc` (AT V0-21). Çağrı grafiğinden (her alt sistemin yalnız kendine ait kodu) bulunan özelliklerin giriş noktaları kesildi; kod flash'ta kalır ama hiçbir yoldan çalışmaz:

| Özellik | Kod | Kapatılan giriş |
|---|---|---|
| Trunk: RADIO / TELSIZ modu (MPT1327 benzeri; ROAMING, PBX, FLEET, SDATA) | ~20,7 KB | ana döngüde 0x0246 bit 1 dalı (0x01A78C), RADIO menüsü (0x0055E1), bit 1'i kuran 4 yer |
| Veri mesajı / sipariş baskısı (FFSK, yazıcı) | ~6,1 KB | `*` tuşu (0x00575A), kanal ekranı (0x003771), seri komut 80 / 82 |
| Kablosuz ses yayın (Y opsiyonu) | ~5,2 KB | ana döngüde 0x0335 bit 4 dalı (0x01A761) |
| `#` ile seçmeli çağrı araması (`C GG___`) | — | 0x00378E |
| Seri servis terminali (0xF8, 2400 baud S-record) | ~1,7 KB | alma kesmesi 0x0194C0 |
| `*` servis / test modu (`HELP 1 FLASH 3 EEPROM`) | ~4,1 KB | 0x00575A (zaten ROM 0x0100FF = 1 ister) |

Not: S sürümünün opsiyon fonksiyonu (0x010630) trunk (0x334 bit 4) ve ses yayın (0x335 bit 4) bitlerini açılışta zaten siliyordu; bu iki mod tabanımızda hiç çalışmıyordu, RAM'leri boş sayılabilir. Seri programlama protokolü (yükleme sayfası, T4000) değişmedi. `test_ozel.js` seçenekleri açık bir B tipi telsizde bu fonksiyonlara hiç girilmediğini denetler.

### Boşalan RAM (AT V0-21)

`disasm/sim/ram_bos.js` açılıştan sonra hiç okunmayan / yazılmayan baytları ölçer (A ve B tipi kart, menüler, tarama, VFO, APRS, seri protokol). Ölçülen her boş adres için koddan kimin dokunduğuna bakıldı (çağrı grafiği, dominatör ağacı). Yalnız kapatılan özelliklerin kodunun dokunduğu ve ölçümde hiç erişilmeyen bloklar:

| Adres | Bayt | Eski sahibi |
|---|---|---|
| 0x02AB–0x02D1 | 39 | seçmeli çağrı alma: periyot tamponu (TB1 kesmesinin RX dalı, 0x023E bit 4; artık açılmıyor) |
| 0x0285–0x0292 | 14 | veri mesajı modülü (F_014AB3) |
| 0x0264–0x026B, 0x026D–0x0272 | 8 + 6 | seçmeli çağrı hane tamponu |
| 0x03C8–0x03D4 | 13 | trunk |
| 0x03A2–0x03A7, 0x03AB–0x03B0, 0x0399–0x039C | 6 + 6 + 4 | trunk |
| 0x0123–0x0131 | 15 | büyük olasılıkla veri mesajı (F_01501B); daha az kesin |

Toplam ~110 bayt (ek olarak `OZEL_RAM` çevresinde ~20 bayt). Diğer erişilmeyen bloklar (0x00AE–0x00FF, 0x0141–0x017E, 0x045A–0x04B8) canlı kodun (seri protokol, açılış) dizilerinin devamı olabilir; kullanılmamalı.

## Seçmeli çağrının kaldırılması


`ornekler/secmeli_kaldir.inc`. Çağrı grafiğinden yalnız seçmeli çağrının kullandığı dört kod bölgesi bulundu ve bunlara dışarıdan giren tüm çağrılar tarandı (5 çağrı + TA1 kesme vektörü; başka referans yok). Çağrılar `NOP` yapıldı, bölgeler `.SERBEST` ile boşaltıldı (0xFF).

| Bölge | Boyut | Ne yapıyordu |
|---|---|---|
| 0x00E65B–0x00E9D1 (`F_00E65B`) | 887 B | gönderme: kimlik, çağrı, transpond tonları |
| 0x007C46–0x007CF5 (`F_007C46`) | 176 B | alma çözücüsünü başlatma |
| 0x01BBE4–0x01C626 (`F_01BBE4`) | 2627 B | hane kararı, çağrı / grup / transpond, çağrı ekranı |
| 0x01C6C0–0x01C8DD (TA1 kesmesi) | 542 B | periyot ortalaması, hane bulma |

Toplam 4232 bayt; bank 0'da ilk kez boş yer açıldı (1063 bayt). Ayrıca "çağır" (`#` tuşu) ve "transpond" istekleri (RAM 0x0244 bit 0 / 1) kurulmuyor; kurulsalar telsiz ton göndermeden vericiye geçerdi. TA1 kesme vektörü boş bir `RTI`'ye bağlandı. TB1 kesmesi DTMF ve APRS için duruyor, sinüs tablosu 0x28FA APRS'te kullanılıyor.

Simülatörde: PTT'de / bırakınca kimlik tonu yok, CCIR 12345 alınınca çözülmüyor, `#123` ile verici açılmıyor; telsiz bekçi köpeğine düşmeden çalışıyor (`test_ozel.js`).

Flash kullanımı (0x2000–0x1FFFF, 120 KB): orijinal %83,9, şimdi %85,4 (FREKANS + APRS + açılış ekranı dahil).

`.SERBEST adres,uzunluk,ilk baytlar...` (a77 / yama.py): orijinaldeki kullanılmayan bir bölgeyi boşaltır; baştaki baytlar orijinalle karşılaştırılır (yanlış adresi silmemek için), bölge 0xFF yapılır ve yeni kod oraya yazılabilir.

## Yeni bir örnek yazmak

Kaynak `disasm/tools/a77.py` sözdiziminde yazılır. Bu, `disasm/YA4000S.asm` listing'inin sözdizimiyle aynıdır. Yama aracı:

```
python3 disasm/tools/yama.py YA4000S.HEX ozel_firmware/YENI.HEX ozel_firmware/ornekler/yeni.asm -l yeni.lst
```

Kurallar ve güvenlik:

- **Orijinal kodu değiştirmek için önce `.EXPECT`.** `.ORG` ile adrese gidin, `.EXPECT` ile oradaki orijinal baytları yazın, sonra yeni komutu yazın. Yama aracı baytları orijinal görüntüyle karşılaştırır. `.EXPECT` olmadan dolu bir bayta yazılırsa hata verir.
- **M/X bayraklarına dikkat.** Yamalanan yerde işlemcinin hangi modda olduğunu listing'deki `.ASSUME` satırından alıp kaynağa yazın. `LDM #0,$05` M=1'de 3 bayt, M=0'da 4 bayttır. Yanlış `.ASSUME` bir sonraki komutu bozar. `.EXPECT` kontrolü bunu yakalar.
- **Değişkenler için boş RAM:** `0x04ED–0x0629` (317 bayt). bss `0x0082–0x04EC`, NC77 çerçeve yığını `0x062A`'dan yukarı, donanım yığını `0x0879`'dan aşağı büyüyor. Kodda bu aralığa bellek erişimi yok. Simülatörde menü, tarama, PTT, TKS ve seçmeli çağrı senaryolarında hiç yazılmadı (`disasm/sim/ram_kullanim.js`); çerçeve yığını en fazla `0x733`'e, donanım yığını `0x829`'a ulaştı. `menu_altyapi.inc`'te `OZEL_RAM = $0500`. Açılışta sıfırlanmaz: her örnek kendi değişkenlerini girişte kurar.
- **Yeni kod için boş alan:** bank 1'de `0x010640–0x01327F` (yaklaşık 11 KB). Bu alan orijinal HEX'te yok, flash'ta silinmiş (0xFF) duruyor. Kullanılan kodun (`0x010100..`, `0x013280..`) arasında kaldığı için aynı flash yongasında olduğu kesin. `0x01E4F4` sonrası da büyük olasılıkla boş, ama adres çözümü şemadan doğrulanmadı. ROM toplamı yalnızca `0x2000–0xFFFF` aralığını kapsar. Yama aracı toplamı her durumda yeniden hesaplar.
- Bank 0'daki firmware fonksiyonlarını `JSRL` ile çağırın. Bank 1'deki veriye `LDA A,>ETIKET,X` gibi uzun adreslemeyle erişin. Mutlak adresleme DT = 0 bankını okur.
- **M/X denetimi.** Assembler SEM/CLM'yi satır sırasıyla izler. Bir etikete başka bir yerden farklı bayraklarla dallanılırsa o etikete `.ASSUME` gerekir. `yama.py` bunu denetler: her dallanma ve atlama için hedeften itibaren gerçek bayraklarla ilerler ve bayrağa bağlı bir komuta uyuşmazlıkla varılırsa hata verir. OYUN'u yazarken bu denetim, simülatörde `BRK` ile çöken iki eksik `.ASSUME`'u yakaladı.
- `.COPY adr,n` orijinal görüntüden bayt kopyalar. Bir tabloyu taşıyıp genişletmek için kullanılır.

### Menüye öğe ekleme (sayi_ters.asm'deki yöntem)

Firmware'in menüsü tablo ile çalışır (ayrıntı: `disasm/tus_takimi.md`):

| Adres | İçerik |
|---|---|
| RAM `0x0227` | Menü durumu |
| ROM `0x3188` | `m` tablosu: sonraki[durum] |
| ROM `0x319C` | `s` tablosu: `[sonraki durum][işleyici 24 bit]` |
| ROM `0x2DE6` / `0x2E86` | Başlıklar (İngilizce / Türkçe), 8 karakter |

Tablolar doludur, hemen arkalarından başka veri gelir. Bu yüzden `menu_altyapi.inc` tabloları okuyan 10 komutu ve `F_003310`'daki 4 başlık göstericisini bank 1'deki kopyalara çevirir. Tabloların kendisi üst dosyada: orijinal 20 durum `.COPY` ile kopyalanır, arkasına her öğe için iki durum eklenir (başlık ve etkin durum). SAYI `0x14`/`0x15`, HESAP `0x16`/`0x17` kullanır.

Yeni bir öğe eklemek için:
1. `ornek.inc`'e `GIR` ve `TUS` işleyicilerini yazın.
2. Üst dosyada `MENU_NEXT`'e iki bayt ekleyin: başlık → halkada sonraki öğe, etkin → `0`.
3. `MENU_S`'ye iki kayıt ekleyin.
4. İki başlığa `.TEXT8` ekleyin, Türkçe ve İngilizce.
5. Halkada öğeyi gösterecek durumu (önceki öğenin sonrakini) değiştirin.

İşleyici kuralları:

- Firmware işleyiciyi `F_01E4C2` ile çağırır: M=0, X=0. `4,S` çağıranın çerçeve göstericisidir. Firmware fonksiyonu çağıracaksanız onu `LDA A,$04,S` / `PHA` ile yığına koyun, çıkışta `PLA` / `RTL`.
- Firmware fonksiyonları (`F_BEEP`, `F_DELAY`, `F_LCD`) çağrı anında yığının tepesinde bu göstericiyi ister. İşleyicinin en üst seviyesinde bu kendiliğinden sağlanır.
  - Bir `JSR` alt yordamından çağırmak için alt yordam önce `LDA A,$03,S` / `PHA` yapar, sonunda `PLA` / `RTS`. Örnek: `oyun.inc`'teki `OY_NOTA`.
  - Arada `PHX` gibi bir şey yığına konmuşsa çağrı bozulur.
- Tuş `0x0226`'da ASCII olarak gelir: rakamlarda `'d'`, değilse tuşun kendisi (`* # s q`). `m` ve `p`'yi firmware kendisi işler.
- Rakamlar firmware'in tamponundadır: `0x0283[0x0293 & 15]`, sayaç `0x0293`.
- Ekran tamponu `0x0252..0x0259` (8 karakter). İşleyiciden sonra firmware `F_00828C` ile LCD'ye yazar.
- Firmware işleyiciyi yalnızca `s`'de tablodan alır. Rakamlarda en son kurulan işleyiciyi çağırır. Giriş işleyicisi (`SAYI_GIR`) bu yüzden `0x02DF`/`0x02E1`'i asıl işleyiciye çevirir.

### Doğrulama zinciri

1. `make test-a77`: assembler, listing'in tamamını orijinal HEX'e bayt bayt geri çeviriyor.
2. `yama.py`: `.EXPECT` kontrolü, üzerine yazma kontrolü, ROM toplamı.
3. `d77.py` ile yamalı HEX'i geri çevirip kaynağa bakın (isteğe bağlı).
4. `make test-ozel`: simülatörde tuş senaryoları. Diğer menülerin ekranları orijinalle aynı mı diye karşılaştırılır. Görüntü seri yükleyiciyle yazılıp yeniden başlatılır.
5. Cihaz.
