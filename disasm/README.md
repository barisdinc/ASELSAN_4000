# ASELSAN 4000/4100 telsiz yazılımı — M37732 disassembly

Kaynak: `../YA4000?.HEX` (Motorola S-record, S1/S2). CPU: Mitsubishi M37732 (MELPS 7700).
Derleyici imzaları NC77 ile uyumlu (ncrt0 başlangıç kodu, runtime kütüphanesi, çağırma kuralı).

## Dosyalar

| Dosya | İçerik |
|---|---|
| `YA4000?.asm` | Tam listing: her komutta adres + orijinal baytlar, `.ASSUME M=,X=` durumları, etiketler, çağıran listesi |
| `YA4000D_functions.txt`, `YA4000E_functions.txt` | Fonksiyon indeksi, en çok kullanılan RAM adresleri, SFR kullanımı |
| `donanim_haritasi.md` | Kesme vektörü → handler, port pini/A-D kanalı → onu okuyan/yazan fonksiyonlar, port yön ayarları |
| `ornek_decompile.c` | Elle C'ye çevrilmiş örnek fonksiyonlar (main, init, opsiyon ayarı, gecikme) |
| `c_lifted/` | **Firmware'in tamamının C çevirisi** (494 fonksiyon), emülatörle eşdeğerliği test edildi; bkz. `c_lifted/README.md` |
| `c_clean/` | Yeni yazılım için okunur sürücüler (I²C/EEPROM, sentezör, TKS, DTMF, seçmeli çağrı); bkz. `c_clean/README.md` |
| `sim/` | **Web simülatörü**: orijinal ya da yeni HEX'i tarayıcıda K3 kartı modeli üzerinde çalıştırır (LCD, tuşlar, PTT, seri port, EEPROM, RF/ses analizi, hata ayıklayıcı); bkz. `sim/README.md` |
| `Makefile` | `make`: çeviriyi yeniden üretir, derler, tüm testleri koşturur (`make sim test-sim` simülatör için) |
| `tools/lift.py`, `tools/emu7700.py` | C çeviricisi ve doğrulama için bağımsız 7700 emülatörü |
| `tools/d77.py` | Yazılan 7700 disassembler (`python3 tools/d77.py ../YA4000D.HEX out.asm`) |
| `tools/verify.py` | Listing'leri HEX ile bayt bayt karşılaştırır |
| `tools/a77.py` | 7700 assembler, `d77.py` sözdizimiyle. `make test-a77` listing'in tamamını yeniden birleştirip HEX ile karşılaştırır |
| `tools/yama.py` | `.asm` yamalarını HEX'e uygular (`.EXPECT` denetimi, ROM toplamı). Örnekler: `../ozel_firmware/README.md` |

Doğrulama:
- 8 listing'in hepsi HEX içeriğiyle **bayt bayt aynı** (eksik/fazla/yanlış bayt yok).
- Opcode tabloları `../MCUDocs/1989_MELPS_7700_Software_Manual.pdf` Appendix C
  (Instruction Code Table 1/2/3: tek bayt, `42` ve `89` önekleri) ile hücre hücre karşılaştırıldı, birebir aynı.
- SFR adresleri ve vektör tablosu M37732S4 veri sayfasından (1991 data book, Fig. 2 ve Table 1) alındı.
- Port pinlerine kılavuzdaki sinyal adları eklendi (`[P67 SENTEZOR_KILIT]` gibi). Kaynak: 4021/4025 Depo Bakım
  Onarım kılavuzu 3.6.2.3 ve Şekil 6-5 (K3 Kontrol ve SF kartı). `main`'deki port yön ayarları şemayla tutarlı.
ROM'un yaklaşık %95'i kod olarak çözüldü; geri kalanı string/tablo verisi.

## Varyantlar

| Dosya | S0 başlığı | Opsiyon metni |
|---|---|---|
| D | battery | DENIZ EL TELSIZI OPSIYONU |
| K | battery | KONVANSIYONEL VERI OPSIYONU |
| Y | battery | KABLOSUZ SES YAYIN OPSIYONU |
| P | battery | DISPATCH-TELSIZ OPSIYONU |
| R | battery | KIRSAL ROLE TELSIZ OPSIYONU |
| S | battery | STANDART VERSIYON |
| V | battery | VOX DEVREDE TELSIZ OPSIYONU |
| E | radio | ayrı build, opsiyon metni yok (kod daha büyük, adresler farklı) |

D/K/P/R/S/V/Y **aynı kaynaktan** derlenmiş. Aralarındaki tek kod farkı `0x010630`'daki opsiyon
ayar fonksiyonu (RAM `0x0334/0x0335/0x033F/0x0246/0x023E` bayrakları). Bunun dışında yalnızca
versiyon metni (`0x01000D..`) ve başlangıç kodundaki bir bölüm-sonu sabiti (`0x00FEEF`) değişiyor.

## Bellek haritası (D)

| Adres | İçerik |
|---|---|
| `0x0000-0x007F` | SFR'ler |
| `0x0080-0x0879` | İç RAM. Yığın `0x0879`'dan aşağı doğru büyüyor |
| `0x087A-0x087C` | Çalışma anında yazılan trampolin: `NOP; WIT; RTL` (güç kapatmada `STP` yazılıyor) |
| `0x2000-0x203F` | Reset kodu ve kesme stub'ları (`JMPL` ile asıl handler'a) |
| `0x2040-0x3209` | Stringler, menü metinleri (TR/EN), tablolar |
| `0x3280-0xFFC7` | Kod (bank 0); `0xFE60` = NC77 ncrt0 |
| `0xFFD6-0xFFFF` | Kesme vektörleri |
| `0x10000-0x10076` | Versiyon/telif metni ("ASELSAN HC ETM (C)1995 ... Yazanlar: AB, HG, YK") |
| `0x10100-0x105C7` | **RAM overlay**: `0x0105F3`'teki döngü bunu RAM `0x0080`'e kopyalayıp çalıştırıyor. Flash yükleyici: UART0'dan veri alıp JEDEC komut dizisiyle (`5555=AA, 2AAA=55, 5555=A0`) paralel flash'a yazıyor. Listing'de `.ORG $000080` bölümü olarak ayrıca çözüldü |
| `0x13280-0x1E4F3` | Kod (bank 1); `0x1E000-0x1E4F3` NC77 runtime (memcpy, 32-bit çarpma/bölme, shift, switch) |

Vektörler: RESET→`0x2000`, UART0 RX/TX (RS232), TA1, TA3, TA4, TB0 (TKS çözme), TB1 (SÇ çözme), TB2 (tick),
INT1 (OKUMA_İSTEK0), INT2 (OKUMA_İSTEK1: FX809/FX802), sıfıra bölme, WDT, DBC, BRK. Kullanılmayanlar: AD, UART1 RX/TX, TA0, TA2, INT0.
UART1 kesmesiz kullanılıyor (C-BUS ve sentezör için senkron seri).

## Donanım (K3 kartı, kılavuzdan)

- TD1 M37732S4LGP, 8.064 MHz kristal (K11); P43 ile kristal ~200-250 ppm kaydırılıyor (XSHIFT).
- TD2 AMD 29F010 (128 KB). 8-bit veri yolu, 17-bit adres; 0x02000-0x1FFFF görünür. Flash yükleyici bunu yerinde programlıyor.
- EEPROM 2 KB I²C (SDA=P62, SCL=P61, bit-bang). Ayarlar, kanal frekansları ve kalibrasyon burada.
- C-BUS (UART1, P85/P86/P87) üzerinden: TD3 FX806 ses işlem, TD4 FX809 FFSK modem (1200 baud), TD15 FX802 DVSR. Seçim: TDSEC1 (P51) / TDSEC2 (P53).
- Sentezör: P70 SENT_YÜKLE, P81 SENT_SEÇ, saat/veri UART1 hattını paylaşıyor; kilit P67.
- PWM çıkışlar: P54 TKS tonu / susturma ayarı, P56 güç ayarı; analog: AN3 sıcaklık, AN4 13.2V, AN5 RSSI, AN6/AN7 yansıyan/giden güç.

## NC77 kalıpları (listing'i okurken)

- **Prolog:** `LDA $04,S / PHD / TAD / (INC A | CLC;ADC #n) / PHA`. D = yerel değişken çerçevesi;
  `$00,$02..` gibi dp erişimleri **yerel değişken**, SFR değil. SFR'lere mutlak adresle erişiliyor.
- **Parametre:** 1. parametre `Y`'de; diğerleri çağrılanın çerçevesine yazılıyor. Dönüş değeri `A`'da (long için `A:B`).
- **M bayrağı:** `char` döndüren fonksiyonlar M=1 ile dönüyor. Disassembler bunu fonksiyonlar arası takip ediyor.
- **switch:** `LDX #n / SEM / JSRL __switch_byte` çağrısının hemen arkasında satır içi tablo var:
  n adet case baytı + (n+1) adet hedef adresi (sonuncusu default). Listing'de `.BYTE/.WORD` olarak çözüldü.
- **Jump table:** `CMP #n / BCC / ASL A / TAX / JMP (tbl,X)`.

## Sözdizimi notu

Komutlar Mitsubishi tarzında (`LDA A,..`, `LDA B,..`, `SEB #m,adr`, `BBS #m,adr,hedef`, `LDM #imm,adr`) yazıldı.
`tools/a77.py` bu sözdizimini doğrudan birleştirir. Her satırda orijinal baytlar olduğu için başka bir assembler'a (RASM77 / AS `CPU MELPS7700`) uyarlamak
mekanik bir iş. `.ASSUME M=x,X=y` satırlarını RASM77'de `.DATA 8/16` ve `.INDEX 8/16` direktiflerine çevirmeniz gerekir.

## Bilinen sınırlamalar

- Kılavuz 4021/4025 araç ve 4031/4035 sabit telsizleri için. Firmware'de "4000/4100 serisi" yazıyor; el telsizi ("battery") varyantlarında pin kullanımı farklı olabilir.
- "no direct caller" notlu fonksiyonlar sezgisel olarak bulundu (pointer ile çağrılıyor olabilirler).
- `.ASSUME` durumları statik analizden geliyor; `PLP` sonrası bayraklar önceki durumla aynı kabul edildi.
