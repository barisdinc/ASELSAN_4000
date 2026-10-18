# Özel firmware

Orijinal YA4000S yazılımına eklenen özellikler. Her örnek önce simülatörde (`disasm/sim/`), sonra cihazda denenir.

| Dosya | İçerik | Nasıl üretilir |
|---|---|---|
| `YA4000S_MERHABA.HEX` | Açılışta 4 mesaj | `disasm/tools/acilis_mesaji.py` |
| `YA4000S_SAYI.HEX` | Menüde **SAYI** öğesi: girilen sayıyı yazar, `s` ile tersini gösterir | `ornekler/sayi_ters.asm` |
| `YA4000S_HESAP.HEX` | SAYI + **HESAP**: dört işlemli hesap makinesi | `ornekler/hesap.asm` |
| `YA4000S_OYUN.HEX` | SAYI + HESAP + **OYUN**: sayı tahmin oyunu (rastgele sayı, sesler) | `ornekler/oyun.asm` |
| `YA4000S_FREKANS.HEX` | Önceki üçü + **FREKANS**: tuş takımından alma ve gönderme frekansı girip telsizi ayarlama (VFO modu; PTT'de ekranda gönderme frekansı) + **APRS**: PTT'de 144,800 MHz'de sabit konum paketi (1200 baud AFSK, AX.25). Seçmeli çağrı kimliğinin PTT'de gönderilmesi kaldırıldı | `ornekler/frekans.asm` |
| `YA4000S_FREKANS_YATOLYE.HEX` | FREKANS ve APRS'in tamamı + açılışta 5 saniye **yATOLYE** (flash'a gömülü, EEPROM / programlayıcı değiştiremez; sürüm yazısı "AS V2-3" yerine) | `ornekler/frekans_yatolye.asm` |

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
KANAL 01 -m-> LISTE -m-> SAYI -m-> HESAP -m-> OYUN -m-> FREKANS -m-> ALT MENU -m-> KANAL 01
```

Bir öğeye `s` ile girilir; içindeyken `m` normal ekrana döner. ALT MENU kapalıysa (bit 5 = 0) FREKANS'tan sonra doğrudan normal ekran gelir. İngilizce menüde başlıklar NUMBER / CALC / GAME / FREQ / SUB MENU.

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

İki adım: önce **alma (RX)**, sonra **gönderme (TX)** frekansı.

| Tuş | İş |
|---|---|
| `m` ×5, `s` | Menü: … GAME → **FREQ** (Türkçe **FREKANS**), ekranda `___ ____` (alma frekansı) |
| `0–9` | Frekans, noktasız. `156525` = 156,525 MHz. 12,5 kHz adımlar için 7 hane: `1565125`. Ekranda `156 5125`; 3. haneden sonraki boşluk ondalık noktadır |
| `*` | Son haneyi sil |
| `s` | Girişi temizle |
| `#` (1. adım) | Alma frekansını hafızaya al (onay sesi). Ekranda `GONDERME` (İngilizce `TX FREQ`): şimdi gönderme frekansı girilir |
| `0–9`, `*` | Gönderme frekansı, aynı şekilde: `160 65--` (çizgiler gönderme frekansının girildiğini gösterir) |
| `#` (2. adım) | Ayarla (onay sesi): telsiz almada 1. frekansı, gönderirken 2. frekansı kullanır. Ekranda alma frekansı kalır |
| `#` (2. adımda hiç hane yokken) | Simplex: gönderme frekansı = alma frekansı |
| `*` (2. adımda giriş boşken) | 1. adıma dön; alma frekansı düzeltilebilir |
| `m` | Çık. Telsiz yeni frekanslarda kalır |

Örnek: `156525#160650#` → alma 156,525 MHz, gönderme 160,650 MHz. `156525##` → ikisi de 156,525 MHz.

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
| (bekle) | Kanal 55'te saniye yazılıysa PTT'ye basılmadan o aralıkla paket gider; telsiz kendisi gönderip almaya döner |
| PTT | ~130 ms sonra paket gider (0,75 s): ekranda `APRS TX`, bitince `APRS OK`. PTT basılı tutulursa telsiz 144,800'de göndermeye (konuşmaya) devam eder |
| `m` | Çık: telsiz önceki kanalına / frekansına döner. Menüdeyken kanal anahtarı çevrildiyse yeni kanal kalır |

FREKANS'taki gibi APRS açıkken firmware'in ~4 s'lik menü zaman aşımı kapalıdır. Bant dışıysa (bant tabanı 144 MHz'in üstünde) ekranda `HATA` / `ERROR` yazar ve frekans değişmez.

**Ayarlar kanal adlarında.** APRS'e her girişte kanal 50–54'ün adları EEPROM'dan okunur ve paket bunlardan kurulur. Kanalın frekansı olması gerekmez: boş kanalların adları da EEPROM'da durur. Adlar PC programlayıcısıyla yazılır; telsiz 8 karakteri ve yalnızca `A-Z 0-9 boşluk _ - /` karakterlerini kabul ettiği için nokta ve küçük harf kullanılmaz.

| Kanal | Ne | Ad örneği | Pakette |
|---|---|---|---|
| 50 | Çağrı işareti ve SSID (1–6 harf/rakam, isteğe bağlı `-0`..`-15`) | `TA7W-7` | `TA7W-7` |
| 51 | Enlem: DDMM, **boşluk = ondalık nokta**, mm, `N`/`S` | `4050 63N` | `4050.63N` |
| 52 | Boylam: DDDMM (4 hane yazılırsa başa 0), boşluk, mm, `E`/`W` | `3109 39E` | `03109.39E` |
| 53 | Yorum (sondaki boşluklar atılır; boş olabilir) | `ASELSAN` | `ASELSAN` |
| 54 | Simge: tablo + kod. Koddaki harf küçük harfe çevrilir; tek karakterse tablo `/`; boşsa `/b` | `/B` | `/b` (bisiklet) |
| 55 | Otomatik gönderim aralığı, saniye (en çok 4 rakam). Boş ya da 0: kapalı; 1–9 yazılırsa 10 | `60` | her 60 s'de bir paket |

Koordinatlarda rakamlar arasındaki ilk boşluk ondalık noktadır (kanal adında nokta yazılamaz). Noktadan sonra 1 ya da 2 hane yazılabilir (tek haneyse sona 0 eklenir: `4050 6N` → `4050.60N`); noktadan önce enlemde 4, boylamda 5 hane (4 yazılırsa başa 0) olmalı. Boşluk hiç yoksa son iki rakam ondalık sayılır (`405063N`, `0310939E`). Baştaki ve harften sonraki boşluklar ile `-` `_` `/` atlanır.

Paket:

```
<50>>APIZCI,WIDE1-1,WIDE2-1:!<51><tablo><52><kod><53>
TA7W-7>APIZCI,WIDE1-1,WIDE2-1:!4050.63N/03109.39EbASELSAN
```

Ad geçersizse ekranda `CAGRI -`, `ENLEM -` ya da `BOYLAM -` (İngilizce `CALL -`, `LAT -`, `LON -`) yazar ve paket gönderilmez; PTT normal gönderme yapar.

- `APIZCI` hedef alanı (tocall): APRS haritalarında cihaz / yazılım adı olarak görünür. APRS tocall listesine kayıtlı değil; bilinmeyen kimliği çoğu hizmet "bilinmeyen cihaz" olarak gösterir. Yol `WIDE1-1,WIDE2-1`.

**Ton üretimi.** Seçmeli çağrının donanım yolu kullanılır (bkz. `docs/calisma_analizi.md` bölüm 9):

- TB1 zamanlayıcı modunda örnekleme saati: 4,032 MHz / 420 = **9600 Hz**, kesme önceliği 6.
- Her TB1 kesmesinde TA1 tek atım darbesi tetiklenir; darbe genişliği o anki örnek. Çıkış P52 SÇ_TON → TD14 süzgeci → FX806 modülatör girişi.
- Örnek: 16 bit faz sayacı + ROM'daki seçmeli çağrı sinüs tablosu (0x28FA). Artım 1200 Hz için 8192, 2200 Hz için 15019. Ton değişirken faz kesintisiz. Bit başına 8 örnek = 1200 baud.
- Çerçeve APRS'e girişte RAM'de kurulur (`AP_HAZIRLA`): AX.25 adresleri, kontrol 0x03, PID 0xF0, bilgi alanı, CRC-16-CCITT FCS. Kesme her 8 örnekte sıradaki biti üretir: başta 45 bayrak (300 ms), çerçeve baytları LSB önce, beş 1'den sonra doldurma biti, 3 bayrak, NRZI. Kanal adları firmware'in kendi EEPROM okuma yordamıyla (`F_0072EE`) okunur.
- FX806 ayarı `F_00E65B`'nin (seçmeli çağrı gönderme) başındakiyle aynı: `$029A` / `$029B` ve `F_00CB52`. Ton yolunu açar, mikrofonu kapatır.

**Bağlantılar (yamalar).**

| Adres | Orijinal | Yeni |
|---|---|---|
| 0x002028 `VEC_TB1` | `JMPL $01C627` | `AFSK_TB1`: paket gidiyorsa örnek üretir, yoksa orijinal kesmeye geçer (DTMF, alma) |
| 0x00EDA2 (durum 4) | PTT'de kimlik gönderilecek mi? (5 ya da doğrudan 8) | APRS menüsündeyse 5'e geçer; gecikme 4 × 32 ms (EEPROM'daki ton gönderme gecikmesi değil: senin telsizinde 1,5 s) |
| 0x00F4E4 (durum 5) | `JSRL F_00E65B` (seçmeli çağrı kimliği) | `APRS_PTT`: APRS menüsündeyse paketi gönderir, değilse hiçbir şey |
| 0x00E9FE (`F_00E9D2`) | susturma açıldı → `SEB #$40,P4` (hoparlör açık) | `APRS_HOPARLOR`: APRS menüsündeyse P46 = 0 |
| 0x00CC44, 0x00CC92 (`F_00CB52`) | ses kapalı mı kararı; squelch kapalıysa FX806'ya sessiz ayar (L_00CD3F) | `APRS_SUS_KARAR` / `APRS_SUS_FX806`: APRS menüsünde ses hep kapalı, FX806'ya hep sessiz ayar (`C8 00`, `08 A0`). Tip B kartta hoparlörü asıl bu keser; P46 kesmiyor. APRS'e girişte `F_00CB52` bir kez çağrılır |
| 0x0080AA `F_0080AA` | bip (tuş, onay, hata) | `APRS_BIP`: APRS menüsündeyse çalmadan döner |
| 0x008084 `F_008084` | uyarı tonu (TA0 → P50) | `APRS_UYARI`: APRS menüsündeyse başlatmaz |
| `VFO_KANCA` (0x00F84D) | | `APRS_KANCA`: menü zaman aşımını yeniler, hoparlörü kapalı tutar, menüden çıkılınca sentezör kaydını geri yükler; otomatik gönderim zamanı gelince PTT isteği kurar |

**Otomatik gönderim.** Zaman tabanı TB2 kesmesinin her 32 ms'de bir artırdığı 0x0457 sayacı. Süre dolunca `APRS_KANCA` RAM 0x0244'ün bit 2'sini kurar. Firmware'in PTT işleyicisi (`F_005E56`) bu baytın alt 6 bitini PTT isteği sayar (0x20 P47, 0x10 panel...); bit 2 hiçbir yerde kullanılmıyordu. Verici firmware'in kendi yoluyla açılır (güç rampası, sentezör), durum 4 → 5 → `APRS_PTT` paketi gönderir ve biti siler; verici kapanır. Gönderme yasağı / kanal meşgulü yüzünden ~3 s içinde paket gitmezse istek bırakılır. Elle PTT ile gönderimden sonra da aralık baştan başlar.

**Simülatörde doğrulama** (`test_ozel.js`, `disasm/sim/afsk.js`): TA1'e yazılan örnekler firmware'den bağımsız bir çözücüden geçirilir (1200/2200 Hz enerjisi, bit saati, NRZI, bit doldurma, CRC). Üç farklı ayarla paket bayt bayt çözülür (SSID'siz, iki haneli SSID, güney / batı, 6 haneli boylam, boş yorum, tek karakterli simge); dört hatalı ayarda hata ekranı çıkar ve paket gitmez. Örnekleme 9600 Hz.

**Gerçek cihazda dikkat.**

- FX806'nın ön vurgusu 2200 Hz'i 1200 Hz'den daha yüksek verebilir. Alıcıda (direwolf, APRSdroid) çözülmezse iki ton için ayrı genlik gerekebilir.
- 144,800 MHz kılavuzdaki donanım bandının (146–174 MHz) altında. VCO kilitlenmeyebilir; ilk deneme yapay yükle.
- Paket yalnızca lisanslı çağrı işaretiyle, izinli frekansta gönderilmelidir.

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
