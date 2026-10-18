# c_lifted — firmware'in tamamının C çevirisi (YA4000S)

`tools/lift.py` ile makine kodundan otomatik üretildi. **Elle düzenlemeyin**; değişiklik gerekiyorsa çeviriciyi düzeltip yeniden üretin (`make lift`). Okunur, elle yazılmış sürücüler `../c_clean/` klasöründe.

## İçerik

| Dosya | İçerik |
|---|---|
| `ramcode.c` | RAM'de (0x0080) çalışan flash yükleyici |
| `bank0_a.c`, `bank0_b.c`, `bank0_c.c` | 0x2000–0xFFFF arası fonksiyonlar, kesme handler'ları (`isr_*`), `reset_entry` |
| `bank1_a.c`, `bank1_b.c` | 0x10000–0x1DFFF arası fonksiyonlar; `c_main` = 0x01A6D5 |
| `runtime.c` | NC77 kütüphanesi (`rt_lmul`, `rt_ldiv`, `rt_memcpy`, `rt_call_ptr` …) |
| `dispatch.c` | `call_address()`: fonksiyon işaretçisiyle yapılan çağrılar için adres → fonksiyon tablosu |
| `m7700.h`, `m7700_rt.c` | CPU modeli: register'lar, bayraklar, yığın, MPY/DIV, PSH/PUL, bellek erişimi, SFR kancaları |
| `rom_image.c` | 128 KB ROM görüntüsü (tablolar ve metinler buradan okunuyor) |
| `functions.h` | Tüm fonksiyon prototipleri |
| `test/` | Eşdeğerlik testleri (aşağıda) |

494 fonksiyon, 42.893 komut. Tanınmayan komut yok; derleme `-Wall` ile uyarısız.

## Kod nasıl okunur

- Register'lar global: `A`, `B`, `X`, `Y`, `S`, `D`, `DT`. Bayraklar ayrı değişken: `C`, `Z`, `N`, `V`, `mflag`.
- **8/16 bit:** Her komutun M/X durumu statik olarak biliniyor. 8 bitlik işlemler `SETLO()` ve `rd8`/`wr8` ile yazıldı.
- **Değişkenler:**
  - `DP(0x05)` = NC77 yerel değişkeni (çerçeve = D).
  - `ABS(0x0244)` = global değişken.
  - `ABS(P6)` = SFR.
  - Her satırdaki yorumda orijinal adres, komut ve donanım sinyali var.
- **Bayraklar:** Kullanılmayan bayrak hesapları atıldı (fonksiyon içi canlılık analizi). Karşılaştırma + dallanma çiftleri tek C koşuluna çevrildi: `if (A < 0x0007) goto …`.
- **Çağrılar:**
  - `S -= 3; F_xxx();` gerçek CPU'nun yığın kullanımını taklit ediyor. Fonksiyon girişindeki `LDA $04,S` bu sayede doğru çerçeveyi okuyor.
  - NC77'nin satır içi `switch` tabloları gerçek `switch`'e çevrildi.
  - İşaretçi çağrıları (`rt_call_ptr`) `call_address()` üzerinden gidiyor.

## Doğrulama

`tools/emu7700.py` makine kodunu doğrudan çalıştıran bağımsız bir 7700 emülatörü. Emülatör ve C çevirisi aynı deterministik SFR girdileriyle koşturulup SFR ve global RAM yazma izleri karşılaştırıldı (`make test`):

| Test | Sonuç |
|---|---|
| NC77 32-bit çarpma/bölme, 80.000 rastgele işlem | hepsi doğru |
| Reset'ten açılış, 3 tohum × 500.000 yazma | izler bire bir aynı |
| 494 fonksiyon × 10 rastgele deneme (komutların %52'si çalıştı) | 4.887 aynı, 39 zaman aşımı (ortak kısım aynı), 13 geçersiz işaretçi (atlandı), 1 fark |

Kalan tek fark bir test kusuru: rastgele bir `memcpy` hedefi emülatörde yığındaki dönüş adresini eziyor. Bu da modelin bilinen tek farkını gösteriyor: **C'de dönüş adresleri belleğe yazılmıyor.** Kendi dönüş adresini ezmeyen kod için iki taraf aynı davranır.

## Çalıştırmak için

`m7700_rt.c` içindeki zayıf (weak) fonksiyonlar donanıma bağlanmalı:
- `sfr_read` / `sfr_write`: çevre birimleri.
- `cpu_wait`: WIT komutu.
- Kesme dağıtıcısı: zamanlayıcı kesmelerinde ilgili `isr_*` fonksiyonunu çağırmalı.

Böylece kod bir PC simülatöründe ya da başka bir MCU'da orijinal firmware gibi koşturulabilir. Yeni yazılım için asıl yol `../c_clean/`.
