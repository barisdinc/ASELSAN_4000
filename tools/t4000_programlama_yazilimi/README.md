# T4000 — 4000 serisi test ve ayar yazılımı (sürüm 3.52)

ASELSAN'ın 4000 serisi telsizler için PC yazılımı. Kaynak: `t4000-3,52.rar`; dosyalar arşivdeki halleriyle, değiştirilmeden eklendi.

| Dosya | İçerik |
|---|---|
| `T4000.EXE` | Program (16 bit Windows, Borland C++ / OWL, 1996) |
| `T4000.INI` | Ayarlar: seri port, hız, varsayılan telsiz değerleri, `pstr=` program şifresi. Türkçe karakterler Windows-1254 kodlamasında |
| `OWL250.DLL`, `BC450RTL.DLL`, `BIDS45.DLL`, `Bwcc.dll` | Borland çalışma kitaplıkları |
| `CHKLIST.MS` | Kurulum programının dosya listesi |

## Ne işe yarar

Kılavuz: `ASELSAN_Dokuman/4000-0003_4021-4025_Depo_Bakim_Onarim_RevC.pdf`, bölüm 2.

- **Firmware yükleme:** Radyo → Yazılım Yükle. Bizim `ozel_firmware/*.HEX` dosyalarımız bu yolla yüklenir. Program telsizdeki yazılımın bilgilerini okur, yeni dosyayla birlikte gösterip onay ister.
- **Ayar parametreleri:** Radyo → Oku / Yaz. Güç, susturma, ses seviyeleri, bant, FX806 ayarları.
- Kanal ve kullanıcı ayarları T4000'de değil, **P4000** programlayıcı yazılımında (`.P40` dosyaları).

## Gerekenler

- **Yazılım kilidi (hardkey):** ASELSAN'ın verdiği, PC'nin paralel (yazıcı) portuna takılan donanım kilidi; kilitsiz program çalışmaz. SERVİS ve MASTER olmak üzere iki tür var; SERVİS kilidi sürüm ve seri numarasını değiştiremez.
- **Program şifresi:** `T4000.INI`'deki `pstr=` satırı. Şifre unutulursa bu satır tamamen silinir; program şifreyi ASELSAN'ın ilk değerine döndürür (kılavuz 2.2.3.4.3).
- **PC:** 386 ya da üstü, Windows 3.1 ya da sonrası, 2 seri port, 1 paralel port (kılavuz 2.5). Bağlantı telsizin seri portundan; `Seriport=` ve `Hız=9600` INI'de.

## Dikkat

- Yazılım yükleme sırasında bir sorun çıkarsa PC tarafı kapatılıp açılabilir, ama **telsizin gücü asla kesilmemeli**; yoksa telsiz açılmaz ve servis gerekir (kılavuz 2.2.3.2.3).
- Yüklemeden önce EEPROM'u yedekleyin ve ayarları P4000 ile kaydedin.
- Yeni firmware'i önce web simülatöründe deneyin: https://barisdinc.github.io/ASELSAN_4000/ (simülatör aynı seri yükleyiciyle flash'a yazmayı da test eder, `disasm/sim/updater.js`).
