; parola_rssi_kaldir.inc - PAROLA, servis kodu ve RSSI göstergesi kaldırıldı (AT V0-32)
;
; ALT MENU'deki PAROLA (durum $0E, NP / OP ekranı F_0047C4) yalnız parola koymak için değil, gizli
; servis kodu için de kullanılıyordu: NP 25970, OP 25970 -> NP 10992, OP 10992 -> NP 00000 (ROM 0x317E
; tablosu, F_004596; RAM 0x01EB 4 -> 5 -> 6 -> 1/2). Kod girilince PAROLA'dan sonra MSR RSSI (durum
; $12, F_004E1A) görünür: e80 ... e120 dBm kalibrasyonu, EE 0x571-0x577. Kalibre edilmiş telsiz
; susturma açıkken kanal adı yerine "-115dBm" gösterir (F_00604C); EE 0x54B bit 6 ile açılışta da açılır.
;
; 1) Menü: DISPLAY (durum $0B) -> ALT MENU ($08). frekans.asm MENU_NEXT tablosunda; B tipi kart
;    DISPLAY'i atlayıp PAROLA'ya geçiyordu (0x0055D6), o da ALT MENU'ye çevrildi. PAROLA ve MSR RSSI
;    hiç görünmez; servis kodu girilemez, kalibrasyon yapılamaz.
; 2) Açılış: EEPROM'da kayıtlı parola (0x54D-0x551) yüklenmez; firmware'in "00000" (parolayı sil) yolu
;    izlenir, kullanıcı bloğu FF'li parolayla yeniden yazılır. Parolası olan telsiz kilitli kalmaz.
; 3) Açılış: EE 0x54B bit 6 (RSSI göstergesi) yüklenmez; 0x01EB açılıştaki 4'te kalır, gösterge
;    çalışmaz ve kapanışta bit 6 sıfır yazılır.
; F_004596, F_004E1A, F_00604C'nin gösterge dalı ve F_008D47 artık çağrılmıyor.

        .ORG $0055D6                    ; B tipi kart: DISPLAY yerine LDM #$0E,$0227 (PAROLA)
        .EXPECT $9C,$27,$02,$0E
        .BYTE $9C,$27,$02,$08           ; -> ALT MENU

        .ORG $008FC2                    ; BEQ L_008FE2 (geçerli parola -> RAM 0x025A'ya kopyala)
        .EXPECT $F0,$1E
        .BYTE $EA,$EA                   ; düşer: 0x025A.. = FF, kullanıcı bloğu yeniden yazılır

        .ORG $0090AE                    ; BBC #$40,$01,L_0090C5 (bit 6 -> F_008D47 -> 0x01EB = 1/2)
        .EXPECT $34,$01,$40,$13
        .ASSUME M=1,X=0
        BRA   PR_RSSI_YOK
        .BYTE $EA,$EA
PR_RSSI_YOK .EQU $0090C5
