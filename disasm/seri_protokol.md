# UART0 (RS232, P82 RxD0 / P83 TxD0) — YA4000S

Firmware'de GPS/NMEA desteği yok (ROM'da "$GP" yok, alma kesmesinde '$' ya da satır sonu işlenmiyor).
UART0'ı üç şey kullanıyor:

| Kullanım | Hız | Ne zaman |
|---|---|---|
| İkili servis/programlama protokolü | 9600 8N1 (U0BRG = 0x19) | Her açılışta (F_019884 Y = 0x19, 0x01A740) |
| ASCII çağrı günlüğü ("dd/mm hh:mm" + çağrı bilgisi, CR LF) | 2400 8N1 (U0BRG = 0x68) | Tip B kart (0x335 bit 1 = 1) ve EE[0x014] bit 1 ya da 0x334 bit 5; ekranın 8. karakteri '*' (2400) / '-' (9600). F_0099AD, F_007809 |
| 0xF8 baytı → 2400 baud yükleyici benzeri rutin | 2400 | 0x334 bit 2 (EE[0x000] bit 2) açıksa, boşta 0xF8 gelince (F_0099BD). Çözülmedi |

## Bağlantı katmanı (alma kesmesi L_019469, gönderme F_0196AA / F_019673)

```
16 10 02  <gövde, içindeki 0x10 baytları 10 10 olarak ikilenir>  10 03  cc cc
SYN DLE STX                                                     DLE ETX 2 bayt
```
- Sondaki 2 bayt: telsiz hep `00 00` gönderiyor, alırken bakmıyor.
- Gövde en fazla 139 bayt; aşılırsa firmware kilitleniyor (SEI; JMP *), watchdog reseti.
- Telsiz yanıtı işlenene kadar (0x0080 bit 0) gelen baytları atıyor; bir istek gönderip yanıtı bekleyin.

Gövdenin ilk baytı çerçeve tipi:

| Tip | Gövde | Anlam |
|---|---|---|
| 1 | `01 01 01 80` (telsiz) / `01` (PC) | Bağlan. Telsiz açılışta gönderir. PC `01` gönderirse sıra numaraları 1'e döner, telsiz ACK verir |
| 2 | `02 ss 01 00` | ACK; ss = alınan veri çerçevesinin sıra numarası |
| 4 | `04 ss 01 00 <komut> <alt> <veri…>` | Veri; ss 1'den başlayıp her çerçevede artar. Telsiz her veri çerçevesine ACK verir, yanıtını da yine tip 4 olarak gönderir. PC bu yanıta ACK vermezse telsiz 10 kez yeniden gönderir, sonra bağlantıyı baştan kurar (tip 1) |

## Uygulama komutları (F_0198D5)

`komut alt …` (veri çerçevesinde 5. bayttan itibaren). Yanıtlar da aynı biçimde.

| Komut | İstek | Yanıt / etki |
|---|---|---|
| 01 | `01` | Yalnızca ACK |
| 03 | `03 01 01 00 aH aL n` EEPROM oku (24C16, en çok ~128 bayt) | `03 04 01 00 aH aL <n bayt>` |
| 03 | `03 01 02 bank aH aL n` CPU belleği oku (ROM/RAM, 24 bit adres) | `03 04 02 …` + veri |
| 03 | `03 03 01 00 aH aL <veri…>` EEPROM yaz | ACK. Sağlama toplamlarını PC düzeltmeli, yoksa açılışta "ARZ PRG" |
| 03 | `03 02 02`, `03 03 02` | Yalnızca ACK (bellek yazma yok) |
| 03 | `03 04 01`, `03 04 02` | Oturumu bitir (dönüş kodu 2) |
| 04 | `04 01` | Yazılım reseti (PMOD bit 3) |
| 04 | `04 02` | Telsizi yeniden başlat (F_00E305) |
| 05 | `05 01 …`, `05 02 x y z` | F_019383 (çözülmedi) → yanıt `05 03` |
| 06 | `06 01` | Uzaktan TX bitir (0x244 bit 3 = 0) |
| 06 | `06 02 t` | Uzaktan TX: 0x37C = t (TX TKS tonu), 0x244 bit 3 = 1 → F_005E56 TX'e geçer. Yanıt `05 03` |
| 07 | `07 k` ölçüm oku | `08 k v`: k=1 güç PWM (TA3), 2 AN7 giden güç, 3 AN6 yansıyan güç, 4 AN3 sıcaklık (yalnız tip B), 5 TA2 (0x1FB), 6 susturma açık (0x23E bit 5; P57 = 1, kanalda sinyal), 7 AN5 RSSI |
| 08 | `08 01 v` | Güç PWM'i yaz (0x20F, 0x279, TA3) — kalibrasyon |
| 08 | `08 05 v` | TA2 / 0x1FB yaz — kalibrasyon |
| 09 | `09 01` → `09 03 hi lo` ; `09 02` → `09 04 hi lo` | FX ses işlemcisi (C-BUS) yazmaç gölgelerini oku (0x296/0x297, 0x1FE/0x1FF) |
| 09 | `09 03…0F hi lo`, `09 10 v` | C-BUS yazmaçlarının (0x10, 0x11, 0x12, 0x13) alanlarını yaz (F_007E4C) — ses ayarları |
| 0A | `0A key` | Uzaktan panel tuşu (F_005822): `g` 0x67 = güç seviyesi (panel b2), `n` 0x6E / `o` 0x6F = F_004FF6 (monitör, panel b3) |
| 0B | `0B 01 brg` | `0B 02 01` yanıtı, sonra UART0 hızını brg ile yeniden kurar |
| 0B | `0B 02 x` | x ≠ 0 ise oturumu bitir |
| 0C | `0C` | `0C 01 00` (ping) |
| 0D | `0D m` | 0x114 = m; m = 3 → firmware güncelleme: yükleyiciye geçiş (aşağıda) |
| 80, 82 | `80 …`, `82 …` | F_014EB2: veri/durum mesajı (0x195 ile ilgili, çözülmedi) |
| C0, E0 | | Yalnızca ACK |

## Simülatörde doğrulama

```
açılış         → 16 10 02 01 01 01 80 10 03 00 00
01             → 16 10 02 02 01 01 00 10 03 00 00                       (ACK)
04 01 01 00 0C → ACK + 16 10 02 04 02 01 00 0C 01 00 10 03 00 00
04 03 01 00 07 07 → ACK + … 08 07 14 …                                  (RSSI = 0x14)
04 04 01 00 03 01 01 00 05 8C 03 → ACK + … 03 04 01 00 05 8C 15 40 00 …   (EE 0x58C = 15 40 = 136 MHz taban)
```

## Firmware güncelleme (yükleyici)

Firmware'i seri porttan firmware'in kendisi yazıyor; ayrı/korumalı bir bootloader yok.

1. Normal protokolde `0D 03` → F_0105C9: güç kapatma rutini (F_00968D), ekranda **"PROGRAM "**, ROM **0x010100–0x0105C7** RAM **0x0080**'e kopyalanır, `RAMCODE_ENTRY` (0x0080) çağrılır. Sıra numaraları devam eder.
2. Yükleyici (RAM'de, kesmeler kapalı, UART0 aynı hızda) **ilk iş tüm flash'ı siler** (29F010 chip erase `AA→5555 55→2AAA 80→5555 AA→5555 55→2AAA 10→5555`, F_00052D) ve bitmesini beklemez. Silme sürerken flash okumaları durum baytı döndürür (DQ7 = 0), yazmalar yok sayılır. PC, aşağıdaki okuma komutuyla 0x2000'in 0xFF okunmasını beklemelidir.
3. Aynı çerçeve ve bağlantı katmanı (tip 1/2/4, ACK = sıradaki beklenen numara). Yükleyicinin komutları:

| İstek (veri gövdesi) | Etki / yanıt |
|---|---|
| `03 01 02 bb hh ll n` | Flash/bellek oku → yanıt `03 04 02 bb hh ll <n bayt>` (PC bu veri çerçevesine ACK vermeli) |
| `03 03 02 bb hh ll <veri…>` | Flash'a yaz (adres bb:hh:ll, veri sayısı = gövde − 10). Bayt bayt `AA 55 A0` + bayt, DQ5/veri yoklaması, ~0,3 s zaman aşımı; zaman aşımında sayfa yazma kipine (AT29C010 tipi, `$92 = 1`) geçer. ACK programlamadan sonra gelir |
| `04 …` | ACK + yazılım reseti (PMOD bit 3) → yeni firmware açılır |
| `0D 03` | Flash'ı yeniden sil, bayt kipine dön |
| `0D x` (x ≠ 3) | Bayt kipine dön |
| diğer | Yalnızca ACK |

- Gövde uzunluğu yükleyicide denetlenmiyor (RAM'e taşar); en çok 128 bayt veri (gövde 138) gönderin.
- Silme en başta yapıldığı için yükleme yarıda kalırsa telsiz açılmaz; flash harici programlayıcıyla yazılmalıdır.
- Yeni firmware'in de seri porttan güncellenebilmesi için 0x010100–0x0105C7 bloğunu (RAM 0x0080'de çalışacak biçimde) ve servis protokolünü (`0D 03`) taşıması gerekir.

## ROM sağlama toplamı

Açılışta (0x00E3FA–0x00E48C) **0x2000–0xFFFF** baytlarının 16 bit toplamı, **0x010003–0x010004**'teki büyük endian değerle karşılaştırılır; tutmazsa **"ERR FLSH"** gösterilir ve telsiz kapanır. Bank 1 (0x010000 ve sonrası, yükleyici dahil) toplama girmez. Sekiz orijinal firmware'in hepsinde tutuyor (ör. YA4000S: 0xD6B1). Değiştirilmiş bir firmware'de bu değer düzeltilmelidir (`sim/updater.js` → `fixRomChecksum`, simülatördeki güncelleme bunu varsayılan olarak yapar).

## Simülatör

`sim/seri_istemci.js` + `sim/yukle.html`: aynı protokolle gerçek telsizle çalışan sayfa (Web Serial). Okuma `01`, `0C`, `03 01 01` (EEPROM) ve `03 01 02` (bellek, 128 baytlık bloklar) ile; flash'ın tamamı 9600 baud'da ≈ 184 s. **Okuma gerçek telsizde doğrulandı (2026-10-18).** Yükleme yukarıdaki yükleyici akışıyla (≈ 313 s, doğrulamalı); şimdilik yalnızca simülatörde doğrulandı. `make test-yukle`.

- Bağlantı katmanında sağlama yok: yazma çerçevesinde kaybolan bir bayt, kaymış veri olarak flash'a yazılır. İstemci sonda bütün blokları geri okur; tutmazsa yükleyicide `0D 03` ile yeniden silip baştan yazar (en çok 3 deneme). Silinmiş flash'a yeniden yazmak için silme şart olduğundan blok bazında düzeltme yapılamaz.
- Yazmadan önce `checkImage`: reset vektörü (0xFFFE) olmalı, 0x010100–0x0105C7 yükleyici bloğu tam ve bilinen yükleyiciyle aynı olmalı (yoksa telsiz bir daha seri porttan güncellenemez), ROM sağlama toplamı gerekirse düzeltilir.
- Yükleme yarıda kalırsa telsiz açık olduğu sürece yükleyici RAM'de çalışır: yeniden bağlanıp (`01`) yükleme baştan başlatılabilir.


`sim/updater.js` PC tarafını uygular (Seri port sekmesi → Firmware güncelleme). `make test-sim`, değiştirilmiş bir görüntüyü (0x2DE9 "CH" → "KN") seri porttan yükleyip yeni kodun açıldığını doğrular: silme 1,3 s, 817 blok × 128 bayt ≈ 140 s (9600 baud), doğrulama ≈ 165 s.
