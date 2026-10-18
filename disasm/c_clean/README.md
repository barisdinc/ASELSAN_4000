# c_clean — yeni yazılım için okunur sürücüler

Orijinal firmware'in çekirdek sürücüleri, çevrilmiş koddan (`../c_lifted`) satır satır okunarak sade C'ye yeniden yazıldı. Donanım erişimi `hal.h` arkasında; başka bir MCU'ya (örneğin MSP430 taşıyıcı kart) geçmek için yalnızca HAL dosyasının yeniden yazılması gerekiyor.

| Modül | Yerini aldığı orijinal kod | İçerik | Doğrulama |
|---|---|---|---|
| `hal.h`, `hal_m37732.h` | — | Pin/register eşlemesi (K3 kartı, kılavuz 3.6.2.3) | — |
| `i2c.c` | 0x007172, 0x00715B, 0x006A68/0x006AC2, 0x006EB3/0x006FA0 | Yazılımla I²C (P61/P62) | Çevrilmiş kodla adım adım karşılaştırıldı |
| `eeprom.c` | 0x007189, 0x007271, 0x0087A7, 0x008A47, 0x00DCFD | 24C16 okuma, kanal kaydı (0x02E + 13·(n−1)), TX tonu, sağlama toplamları | Okuma: aynı sıra. Kayıt düzeni simülatörde doğrulandı. **Yazma standart 24C16 yöntemi, orijinalle henüz karşılaştırılmadı** |
| `synth.c` | 0x00DA32, 0x0088B8, 0x0087DE, 0x008825, 0x008854, 0x00D2A3, 0x00D45C, 0x00D3B4 | Frekans planı ve MB1502 kelime hesabı, yükleme, kilit kontrolü (P67 aktif düşük) | `test/test_synth`: 10 durumda orijinal firmware'in simülatörde gönderdiği baytlarla aynı |
| `ctcss.c` | 0x00F854, 0x00F8AE, 0x00F8BD, 0x00FB0B, 0x00F94C | TKS DDS üretici + I/Q korelatör çözücü | `test/test_ctcss`: 9 ton × 3 sinyal, 1.620 kararın hepsi orijinalle aynı |
| `tones.c` | 0x005282, 0x00E8C2, 0x01C627 (TX) | DTMF ve seçmeli çağrı üretimi | `test/test_tones`: 57 ton × 2000 örnek, hepsi aynı |
| `tone_tables.c` | ROM 0x21EF–0x2AF9 | TKS, DTMF, CCIR/ZVEI1/EEA tabloları (`tools/gen_tables.py`) | Testlerde orijinal kod ROM'dan okuyor, aynı sonuç |

## Henüz temiz C'ye geçmeyenler

Bunlar `../c_lifted` içinde çalışır durumda; okunur hale getirilmeleri gerekiyor:
- Seçmeli çağrı alma (0x007CAE, 0x01C6C0)
- RX/TX durum makinesi (0x00F3E8)
- Güç döngüsü (0x00CFC4)
- C-BUS ve FX806/FX809 komutları (0x007E4C, 0x00755E)
- Ön panel / LCD, menüler (0x0134C0)
- Tarama
- UART0 protokolü ve flash yükleyici

## Açık noktalar

- **`selcall_apply_repeat`:** Standart Select-5 tekrar kuralı. Orijinalde nerede uygulandığı izlenmedi.
- **Düzeltme (simülatörle bulundu):** İlk sürümde EEPROM'da hazır MB1502 kelimeleri olduğu ve P67'nin aktif yüksek olduğu yazılmıştı; ikisi de yanlıştı. EEPROM 14 bitlik frekans kelimeleri tutuyor, frekansı firmware hesaplıyor. P67 = 0 kilitli demek.
- 0x37C'nin 7/6. bitlerinin anlamı çözülmedi. (Kanal kaydının +12 baytı çözüldü: kanal adının 8. karakteri.)
- **`hal_ipl_set`, `hal_irq_*`:** M37732'de satır içi assembly ile yazılmalı. NC77'de bu iş `PHP / STA 2,S / PLP` kalıbıyla yapılıyor.
