# Özel firmware

Orijinal YA4000S yazılımına eklenen özellikler. Her örnek önce simülatörde (`disasm/sim/`), sonra cihazda denenir.

| Dosya | İçerik | Nasıl üretilir |
|---|---|---|
| `YA4000S_MERHABA.HEX` | Açılışta 4 mesaj | `disasm/tools/acilis_mesaji.py` |
| `YA4000S_SAYI.HEX` | Menüde **SAYI** öğesi: girilen sayıyı yazar, `s` ile tersini gösterir | `ornekler/sayi_ters.asm` |
| `YA4000S_HESAP.HEX` | SAYI + **HESAP**: dört işlemli hesap makinesi | `ornekler/hesap.asm` |
| `YA4000S_OYUN.HEX` | SAYI + HESAP + **OYUN**: sayı tahmin oyunu (rastgele sayı, sesler) | `ornekler/oyun.asm` |
| `YA4000S_FREKANS.HEX` | Önceki üçü + **FREKANS**: tuş takımından frekans girip telsizi ayarlama (VFO) | `ornekler/frekans.asm` |

Kaynak düzeni (`ornekler/`):

| Dosya | İçerik |
|---|---|
| `menu_altyapi.inc` | Ortak: firmware'in menü değişkenleri, sabitler, menü tablolarını bank 1'e taşıyan 18 yama |
| `sayi_ters.inc`, `hesap.inc`, `oyun.inc`, `frekans.inc` | Her örneğin işleyicileri |
| `sayi_ters.asm`, `hesap.asm`, `oyun.asm`, `frekans.asm` | Bir firmware = altyapı + menü tabloları (hangi öğeler, hangi sırada) + örneklerin `.inc` dosyaları |

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

## FREKANS kullanımı (VFO)

| Tuş | İş |
|---|---|
| `m` ×5, `s` | Menü: … GAME → **FREQ** (Türkçe **FREKANS**), ekranda `___ ____` |
| `0–9` | Frekans, noktasız. `156525` = 156,525 MHz. 12,5 kHz adımlar için 7 hane: `1565125`. Ekranda `156 5125`; 3. haneden sonraki boşluk ondalık noktadır |
| `*` | Son haneyi sil |
| `s` | Girişi temizle |
| `#` | Ayarla: RX ve TX bu frekansa (simplex). Onay sesi: 2 × 1000 Hz. Normal ekranda kanal adı yerine frekans görünür |
| `m` | Çık. Telsiz yeni frekansta kalır |

Kanal anahtarı çevrilince telsiz o kanala döner. Frekans yalnızca RAM'de tutulur: EEPROM'a yazılmaz, kapatınca kaybolur.

Bu durumlarda `ERROR` (Türkçe `HATA`) gösterilir ve bip çalar; sonraki tuş girişe döner:
- 6 haneden az giriş;
- 2,5 kHz ızgarasında olmayan frekans;
- firmware'in temsil edebildiği aralığın dışı: bant tabanı (EEPROM 0x58C) … taban + 40,95 MHz;
- telsiz göndermedeyken basılan `#`.

Nasıl çalışıyor:
- **Sentezör kelimeleri.** Firmware'in kanal hesabının aynısı burada da yapılır: taban, ±45 MHz ara frekans, ön bölücü ve referans, firmware'in açılışta EEPROM'dan kurduğu RAM değişkenlerinden okunur (formül `frekans.inc` başında). VHF ve UHF'de, üç adım türünde, simülatörde firmware'in aynı frekans için EEPROM kanalından hesapladığı baytlarla birebir aynı çıkıyor.
- **Yükleme.** Baytlar geçerli kanalın sentezör kaydına (RAM 0x0370–0x037C) yazılır ve radyo durumu (0x0228) 1 yapılır. LIST menüsü kanal seçerken de aynısını yapar. Ana döngü RX'e yeniden girer ve sentezörü yükler; PTT'de TX kelimeleri buradan alınır.
- **Bölme.** 32 bit bölmeler kaydır-çıkar yordamıyla (`FR_BOL`) yapılır; işlemcinin DIV komutunun 32 bitlik bölünen davranışına güvenilmedi.

**Gerçek cihazda dikkat.** Kabul edilen aralık (VHF tabanı 136 MHz ise 136–176,95 MHz) kılavuzdaki donanım bandından (146–174 MHz) geniştir. Bant dışında VCO kilitlenmeyebilir ve TX filtreleri uygun değildir. Ayrıca yalnızca izinli frekanslarda gönderme yapın. İlk denemeleri yapay yükle (dummy load) ve bant içindeki bilinen bir frekansla yapın. İstenirse VFO'da TX kapatılabilir ya da aralık kılavuzdaki banda daraltılabilir.

Telsiz bu modlarda da çalışır: işleyici her tuşta bir kez çağrılır ve hemen döner, ana döngü (alma, kanal anahtarı, PTT) sürer. Firmware'in menü zaman aşımı burada da geçerli. Bir süre tuşa basılmazsa normal ekrana döner.

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
