; dil_kaldir.inc - yalnız Türkçe (AT V0-22)
;
; Firmware menü / mesaj dilini EEPROM 0x018 bit 7'den alıyor (RAM 0x0340 = OPTIONS bit 7: 1 Türkçe,
; 0 İngilizce; başlık tabloları 0x2E86 / 0x2DE6). Bizim menüler de aynı biti kullanıyor. Bit iki
; yerde EEPROM'dan yükleniyor: açılışta (0x00E45C, 1 bayt) ve ayar denetiminde (F_00DA32, 0x00DA97,
; 0x018..0x02D). İkisinin hemen ardından bit 7 kurulur: EEPROM'daki ayar ne olursa olsun Türkçe.
; İngilizce metinler ROM'da kalır ama hiç seçilmez.

F_SONRAKI_E153 .EQU $00E153
F_EE_OKU_DIL   .EQU $0072EE

        .ORG $00E460                    ; açılış: EE[0x018] -> $0340 okundu; JSRL F_00E153
        .EXPECT $22,$53,$E1,$00
        JSRL  DIL_TR_ACILIS
        .ORG $00DAB0                    ; F_00DA32: 0x018..0x02D okundu; sıradaki okuma (0x58B)
        .EXPECT $22,$EE,$72,$00
        JSRL  DIL_TR_AYAR
        .ORG DIL_KOD

; Yazmaçlar ve bayraklar korunur; çağrılan fonksiyona aynı yığınla geçilir (JMPL).
DIL_TR_ACILIS:
        PHP
        SEM
        SEB   #$80,OPTIONS
        PLP
        JMPL  F_SONRAKI_E153
DIL_TR_AYAR:
        PHP
        SEM
        SEB   #$80,OPTIONS
        PLP
        JMPL  F_EE_OKU_DIL
