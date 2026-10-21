; aprs_ayar.inc - APRS TR / DG / IG ayarları (AT V0-24)
;
; Menüde APRS'ten sonra üç öğe; 's' ile açılır / kapanır, başlıkta '*' açık, '-' kapalı:
;   APRS TR  tracker: ayarlanan aralıkta otomatik konum paketi (aralık EEPROM 0x702)
;   APRS DG  digipeater (aprs_digi.inc)
;   APRS IG  igate: duyulan paketleri seri porta yaz (aprs_seri.inc)
; Özellikler APRS ekranında (APRS menüsüne girince, 144,800) çalışır; bu öğeler yalnız ayardır.
; Kalıcı: EEPROM 0x733 = $A0 | bitler (APRS bölümünün sağlamasına girmez; programlama sayfasının
; APRS sekmesinden de değişir). Geçersizse varsayılan: TR açık, IG açık, DG kapalı (V0-23 davranışı).

AP_AYAR     .EQU $04ED          ; bit 0 TR, bit 1 DG, bit 2 IG
AP_AYAR_B   .EQU $04EE          ; EEPROM'dan okunan / yazılan bayt
AYAR_TR     .EQU $01
AYAR_DG     .EQU $02
AYAR_IG     .EQU $04
AYAR_VARS   .EQU AYAR_TR|AYAR_IG
EE_AYAR     .EQU $0733
ST_APRS_TR  .EQU $18
ST_APRS_DG  .EQU $19
ST_APRS_IG  .EQU $1A

; EE_AYAR'ı AP_AYAR_B'ye oku (C = 0) ya da AP_AYAR_B'yi yaz (C = 1). D = boş çerçeve, M = 0.
AY_ERIS:
        .ASSUME M=0,X=0
        PHP
        LDA   A,#EE_AYAR
        STA   A,$04
        LDA   A,#AP_AYAR_B
        STA   A,$01
        SEM
        LDA   A,#1
        STA   A,$00
        LDA   A,#0
        STA   A,$03
        CLM
        LDY   #$00A0
        PLP
        TDA
        PHA
        BCS   AZE_YAZ
        JSRL  F_EE_OKU
        BRA   AZE_SON
AZE_YAZ:
        JSRL  F_EE_YAZ
AZE_SON:
        CLM
        PLA
        RTS

; EEPROM'dan AP_AYAR'ı yükle (D = boş çerçeve; M = 0 giriş ve çıkış)
AY_YUKLE:
        .ASSUME M=0,X=0
        CLC
        JSR   AY_ERIS
        SEM
        LDA   A,AP_AYAR_B
        AND   A,#$F8
        CMP   A,#$A0
        BEQ   AZY_1
        LDA   A,#AYAR_VARS
        BRA   AZY_2
AZY_1:  LDA   A,AP_AYAR_B
        AND   A,#$07
AZY_2:  STA   A,AP_AYAR
        CLM
        RTS

; Menü 's' işleyicileri (JSRL, 4,S = boş çerçeve, M = 0): biti değiştir, yaz, başlığı göster
AYAR_TUS_TR:
        LDA   A,#AYAR_TR
        BRA   AYAR_TUS
AYAR_TUS_DG:
        LDA   A,#AYAR_DG
        BRA   AYAR_TUS
AYAR_TUS_IG:
        LDA   A,#AYAR_IG
AYAR_TUS:
        .ASSUME M=0,X=0
        TAX                             ; X = bit
        LDA   A,$04,S
        PHA
        PHD
        TAD
        PHX
        JSR   AY_YUKLE
        PLX
        SEM
        TXA
        EOR   A,AP_AYAR
        STA   A,AP_AYAR
        ORA   A,#$A0
        STA   A,AP_AYAR_B
        CLM
        SEC
        JSR   AY_ERIS
        PLD
        SEM
        LDM   #' ',LCD+7                ; başlık yeniden: AY_GOSTER durum işaretini koyar
        CLM
        PLA
        PHA
        JSR   AY_GOSTER
        PLA
        RTL

; Menüde APRS TR / DG / IG başlığındayken (LCD "APRS TR ") 8. karaktere '*' / '-' koy ve yaz.
; APRS_KANCA'dan her turda ve işleyiciden (A = boş çerçeve, M = 0; M = 0 döner).
AY_GOSTER:
        .ASSUME M=0,X=0
        PHA
        SEM
        LDA   A,MENU
        LDX   #AYAR_TR
        CMP   A,#ST_APRS_TR
        BEQ   AZG_1
        LDX   #AYAR_DG
        CMP   A,#ST_APRS_DG
        BEQ   AZG_1
        LDX   #AYAR_IG
        CMP   A,#ST_APRS_IG
        BNE   AZG_SON
AZG_1:  LDA   A,LCD                     ; ekranda bizim başlık mı ("APRS ", 8. karakter boş)
        CMP   A,#'A'
        BNE   AZG_SON
        LDA   A,LCD+4
        CMP   A,#' '
        BNE   AZG_SON
        LDA   A,LCD+7
        CMP   A,#' '
        BNE   AZG_SON
        TXA
        AND   A,AP_AYAR
        BEQ   AZG_KAPALI
        LDM   #'*',LCD+7
        BRA   AZG_YAZ
AZG_KAPALI:
        LDM   #'-',LCD+7
AZG_YAZ:
        CLM
        PLA
        PHA
        PHA
        JSRL  F_LCD
        CLM
        PLA
AZG_SON:
        CLM
        PLA
        RTS
