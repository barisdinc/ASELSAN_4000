; yazici_kaldir.inc - YAZICI / PRINTER menüsü ve yazıcı çıkışı kaldırıldı (AT V0-19)
;
; B tipi (araç / sabit) kartta YAZICI menüsü (durum 4: PRINTER -> PRINTER / DATE / TIME) seri
; porttan yazıcıya çağrı günlüğü açar (RAM 0x0240 bit 8). Açıkken firmware her kimlik gönderiminde
; ve alınan çağrıda UART0'a "4       ·01/01 00:02" gibi satırlar yazar (F_007809) ve UART0'ı
; 2400 baud'a alabilir (F_0099AD). Seçmeli çağrı (ANI) kaldırıldığından kullanılmıyor; APRS seri
; çıkışına karışıyordu.
;
; 1) Menü: 'm' YAZICI'ya (durum 4) gelince firmware 0x033F bit 1 ve kart tipine bakıp yoksa
;    ALT MENU'ye (8) geçiyor (0x0055F6). Denetim koşulsuz atlamaya çevrildi: YAZICI hiç görünmez.
; 2) Açılış: F_008ECC kalıcı ayar baytının bit 3'ünden yazıcıyı açıyor (0x00908D). Atlanır:
;    EEPROM'da açık kalmış olsa da yazıcı çıkışı kapalı başlar.
; Menü işleyicisi F_004AD6 ve tarih / saat (F_003F46, F_003D09) artık çağrılmıyor (~1,8 KB).

YK_ALT_MENU .EQU $005600        ; LDM #$08,$0227 (ALT MENU)
YK_ATLA     .EQU $009097

        .ORG $0055F6                    ; BBC #$02,$033F,L_005600 (+ BBS ...,$0335)
        .EXPECT $3C,$3F,$03,$02,$05
        .ASSUME M=1,X=0
        BRA   YK_ALT_MENU
        .BYTE $EA,$EA,$EA

        .ORG $00908D                    ; BBC #$08,$01,L_009097 / CLM / SEB #$0100,$0240
        .EXPECT $34,$01,$08,$06
        .ASSUME M=1,X=0
        BRA   YK_ATLA
        .BYTE $EA,$EA
