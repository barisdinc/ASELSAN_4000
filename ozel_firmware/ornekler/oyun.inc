; oyun.inc - OYUN menü öğesi: sayı tahmin oyunu (menu_altyapi.inc ile)
;
;   ... m (OYUN) s   -> "___"          telsiz 1..999 arası bir sayı tuttu
;   0-9      tahmin (en çok 3 hane)
;   *        son haneyi sil
;   #        tahmini gönder:  "500 ^  1"  ^ = tutulan sayı daha büyük (tiz bip)
;                             "250 v  2"  v = daha küçük (pes bip)
;                             "WIN    9"  bildin (melodi); Türkçe "BULDU  9"
;   s        yeni oyun
;   m        çık
; Bildikten sonra herhangi bir tuş yeni oyunu başlatır.
;
; Rastgelelik: TB2 firmware'in sürekli sayan 1 ms zamanlayıcısı. Her tuşta TB2 16 bitlik bir
; LFSR'ye karıştırılır; kullanıcının basma anı milisaniyenin altında rastgele olduğu için
; her oyunun sayısı öngörülemez. Sayı reddetmeyle seçilir: LFSR'nin alt 10 biti < 999 olana
; kadar adım at, sonra +1.
;
; Sesler (OY_NOTA): firmware'in bip yordamı F_BEEP tek notalık bir kodla (20) çağrılır; o
; ALERT ayarını ve zamanlayıcı kurulumunu yapar. Hemen ardından frekans (TA0 = 2016000 / f)
; ve süre (BEEP_SURE, 10 ms birim) kendi tablomuzdan yazılır. Firmware'in tablosundaki
; notalar en çok 30 ms olduğundan melodi için yetmiyor.
;   daha büyük: 1500 Hz 80 ms     daha küçük: 400 Hz 150 ms
;   bildin:     do mi sol do' (523 659 784 1047 Hz), notalar arası F_DELAY ile bekleme;
;               melodi süresince (yaklaşık 0,7 s) işleyici bekler, kesmeler çalışır.
; Hata sesi (13 = 2 x 600 Hz) doğrudan firmware'in tablosundan.

OY_SEED     .EQU OZEL_RAM+$20   ; 2 bayt LFSR durumu
OY_HEDEF    .EQU OZEL_RAM+$22   ; 2 bayt tutulan sayı
OY_V        .EQU OZEL_RAM+$24   ; 2 bayt hesaplanan tahmin
OY_T        .EQU OZEL_RAM+$26   ; 2 bayt ara değer
OY_HANE     .EQU OZEL_RAM+$28   ; 3 bayt girilen haneler (ASCII)
OY_NDIG     .EQU OZEL_RAM+$2B   ; girilen hane sayısı
OY_DENEME   .EQU OZEL_RAM+$2C   ; deneme sayısı (en çok 99)
OY_OK       .EQU OZEL_RAM+$2D   ; son yön: ' ', '^', 'v'
OY_DURUM    .EQU OZEL_RAM+$2E   ; bit 0: bildi, bit 1: haneler son tahmini gösteriyor
OY_I        .EQU OZEL_RAM+$2F   ; döngü sayacı

O_BILDI     .EQU 1
O_SON       .EQU 2

; ---------------------------------------------------------------------------------------------
OYUN_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        LDM   #OYUN_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(OYUN_TUS),HANDLER_BK
        CLM
        JSR   OY_KARISTIR
        JSR   OY_YENI
        JMP   OK_SON

; ---------------------------------------------------------------------------------------------
OYUN_TUS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        JSR   OY_KARISTIR               ; her tuş rastgeleliğe katkı
        SEM
        BBC   #O_BILDI,OY_DURUM,OK_1
        CLM                             ; bildikten sonra her tuş: yeni oyun
        JSR   OY_YENI
        BRA   OK_SON
OK_1:   .ASSUME M=1,X=0
        LDA   A,KEY
        CMP   A,#'s'
        BNE   OK_2
        CLM
        JSR   OY_YENI
        BRA   OK_SON
OK_2:   .ASSUME M=1,X=0
        CMP   A,#'d'
        BEQ   OK_RAKAM
        CMP   A,#'*'
        BEQ   OK_SIL
        CMP   A,#'#'
        BNE   OK_SON
        JMP   OK_GONDER

OK_SON: CLM
        JSR   OY_EKRAN
        PLA
        RTL

OK_BIP: CLM                             ; geçersiz tuş sesi
        LDY   #13
        JSRL  F_BEEP
        BRA   OK_SON

; ---- rakam
OK_RAKAM:
        .ASSUME M=1,X=0
        LDA   A,NDIGIT                  ; firmware'in tamponundan al, tamponu boşalt
        DEC   A
        CLM
        AND   A,#$000F
        TAX
        SEM
        LDA   A,DIGITS,X
        LDM   #0,NDIGIT
        BBC   #O_SON,OY_DURUM,OR_1      ; ekranda son tahmin vardı: yeni giriş
        CLB   #O_SON,OY_DURUM
        LDM   #0,OY_NDIG
OR_1:   STA   A,OY_T                    ; hane
        CLM
        LDA   A,OY_NDIG
        AND   A,#$00FF
        TAX
        SEM
        CPX   #3
        BCS   OK_BIP
        LDA   A,OY_T
        STA   A,OY_HANE,X
        INC   OY_NDIG
        BRA   OK_SON

; ---- sil
OK_SIL: .ASSUME M=1,X=0
        BBS   #O_SON,OY_DURUM,OK_BIP    ; gönderilmiş tahmin silinmez
        LDA   A,OY_NDIG
        BEQ   OK_BIP
        DEC   OY_NDIG
        BRA   OK_SON

; ---- gönder
OK_GONDER:
        .ASSUME M=1,X=0
        LDA   A,OY_NDIG
        BEQ   OK_BIP
        BBS   #O_SON,OY_DURUM,OK_BIP    ; aynı tahmin iki kez sayılmaz
        CLM
        JSR   OY_DEGER                  ; OY_V = girilen sayı
        SEM
        LDA   A,OY_DENEME
        CMP   A,#99
        BCS   OG_1
        INC   OY_DENEME
OG_1:   SEB   #O_SON,OY_DURUM
        CLM
        LDA   A,OY_V
        CMP   A,OY_HEDEF
        BEQ   OG_BILDI
        BCS   OG_KUCUK
        SEM                             ; tahmin < hedef: daha büyük
        LDM   #'^',OY_OK
        CLM
        LDY   #NOTA*N_YUKSEK
        JSR   OY_NOTA
        JMP   OK_SON
OG_KUCUK:
        SEM                             ; tahmin > hedef: daha küçük
        LDM   #'v',OY_OK
        CLM
        LDY   #NOTA*N_ALCAK
        JSR   OY_NOTA
        JMP   OK_SON
OG_BILDI:
        SEM
        SEB   #O_BILDI,OY_DURUM
        CLM
        JSR   OY_EKRAN                  ; melodiden önce ekrana yaz
        JSRL  F_LCD
        LDA   A,#NOTA*N_MELODI          ; OY_V = melodi notasının tablo konumu
        STA   A,OY_V
OG_NOTA:
        LDY   OY_V
        JSR   OY_NOTA
        LDA   A,OY_V
        CLC
        ADC   A,#NOTA
        STA   A,OY_V
        CMP   A,#NOTA*N_SON
        BNE   OG_NOTA
        JMP   OK_SON

; ---------------------------------------------------------------------------------------------
; Nota çal: Y = NOTALAR içindeki konum (M=0). Yalnız işleyicinin en üst seviyesinden JSR ile
; çağrılır: çerçeve göstericisi 3,S'de (2 baytlık dönüş adresinin altında); firmware
; fonksiyonları için yeniden yığına konur.
OY_NOTA:
        .ASSUME M=0,X=0
        LDA   A,$03,S
        PHA
        STY   OY_T
        LDY   #20                       ; tek, kısa nota: kurulumu firmware yapsın
        JSRL  F_BEEP
        LDX   OY_T
        LDA   A,>NOTALAR,X              ; frekans
        STA   A,TA0
        LDA   A,>NOTALAR+2,X            ; süre (10 ms)
        AND   A,#$00FF
        STA   A,BEEP_SURE
        LDA   A,>NOTALAR+3,X            ; sonraki notadan önce bekleme (ms)
        BEQ   ON_SON
        TAY
        JSRL  F_DELAY
ON_SON: PLA
        RTS

; ---------------------------------------------------------------------------------------------
; Alt yordamlar (JSR / RTS). Bunların içinden firmware fonksiyonu çağrılmaz.

; SEED ^= TB2, bir LFSR adımı (M=0)
OY_KARISTIR:
        .ASSUME M=0,X=0
        LDA   A,OY_SEED
        EOR   A,TB2
        STA   A,OY_SEED
        JMP   OY_ADIM

; 16 bit Galois LFSR (x^16 + x^14 + x^13 + x^11 + 1), sıfır durumdan kaçınılır (M=0)
OY_ADIM:
        .ASSUME M=0,X=0
        LDA   A,OY_SEED
        BNE   OA_1
        LDA   A,#$ACE1
OA_1:   LSR   A
        BCC   OA_2
        EOR   A,#$B400
OA_2:   STA   A,OY_SEED
        RTS

; yeni oyun: hedef = 1..999 (M=0)
OY_YENI:
        .ASSUME M=0,X=0
OY_SEC: JSR   OY_ADIM
        LDA   A,OY_SEED
        AND   A,#$03FF
        CMP   A,#999
        BCS   OY_SEC
        INC   A
        STA   A,OY_HEDEF
        SEM
        LDM   #0,OY_NDIG
        LDM   #0,OY_DENEME
        LDM   #' ',OY_OK
        LDM   #0,OY_DURUM
        CLM
        RTS

; OY_V = OY_HANE[0..NDIG-1] ondalık değeri (M=0)
OY_DEGER:
        .ASSUME M=0,X=0
        LDM   #0,OY_V
        LDX   #0
OD_DON: LDA   A,OY_V                    ; V = V*10 + hane
        ASL   A
        STA   A,OY_T                    ; T = 2V
        ASL   A
        ASL   A                         ; A = 8V
        CLC
        ADC   A,OY_T
        STA   A,OY_V
        LDA   A,OY_HANE,X
        AND   A,#$000F                  ; '0'..'9' -> 0..9
        CLC
        ADC   A,OY_V
        STA   A,OY_V
        INX
        SEM
        TXA
        CMP   A,OY_NDIG
        CLM
        BNE   OD_DON
        RTS

; ekran (M=0 giriş/çıkış)
OY_EKRAN:
        .ASSUME M=0,X=0
        SEM
        LDX   #0                        ; boşluk
        LDA   A,#' '
OE_BOS: STA   A,LCD,X
        INX
        CPX   #8
        BNE   OE_BOS
        BBC   #O_BILDI,OY_DURUM,OE_OYUN
        LDX   #0                        ; WIN / BULDU
        BBS   #$80,OPTIONS,OE_TR
OE_EN:  LDA   A,>TXT_WIN,X
        STA   A,LCD,X
        INX
        CPX   #5
        BNE   OE_EN
        BRA   OE_SAYAC
OE_TR:  LDA   A,>TXT_BULDU,X
        STA   A,LCD,X
        INX
        CPX   #5
        BNE   OE_TR
        BRA   OE_SAYAC

OE_OYUN:
        LDX   #0                        ; haneler 0..2: girilenler, kalanı '_' (tahmin gönderilmişse boş)
OE_H:   TXA
        CMP   A,OY_NDIG
        BCS   OE_ALT
        LDA   A,OY_HANE,X
        BRA   OE_HK
OE_ALT: LDA   A,#'_'
        BBC   #O_SON,OY_DURUM,OE_HK
        LDA   A,#' '
OE_HK:  STA   A,LCD,X
        INX
        CPX   #3
        BNE   OE_H
        LDA   A,OY_OK                   ; yön
        STA   A,LCD+4

OE_SAYAC:
        LDA   A,OY_DENEME               ; deneme sayısı, 6-7. haneler
        BEQ   OE_SON
        LDM   #'0',OY_I                 ; onlar = ' ' ya da '1'..'9'
OE_ON:  CMP   A,#10
        BCC   OE_BIR
        SEC
        SBC   A,#10
        INC   OY_I
        BRA   OE_ON
OE_BIR: CLC
        ADC   A,#'0'
        STA   A,LCD+7
        LDA   A,OY_I
        CMP   A,#'0'
        BEQ   OE_SON
        STA   A,LCD+6
OE_SON: CLM
        RTS

; nota: [TA0 yeniden yükleme][süre, 10 ms][bekleme ms, 16 bit] = 5 bayt
NOTA        .EQU 5
N_YUKSEK    .EQU 0
N_ALCAK     .EQU 1
N_MELODI    .EQU 2
N_SON       .EQU 6
NOTALAR:
        .WORD 2016000/1500
        .BYTE 8
        .WORD 0
        .WORD 2016000/400
        .BYTE 15
        .WORD 0
        .WORD 2016000/523               ; do
        .BYTE 12
        .WORD 150
        .WORD 2016000/659               ; mi
        .BYTE 12
        .WORD 150
        .WORD 2016000/784               ; sol
        .BYTE 12
        .WORD 150
        .WORD 2016000/1047              ; do'
        .BYTE 35
        .WORD 0

TXT_WIN:
        .BYTE "WIN  "
TXT_BULDU:
        .BYTE "BULDU"
