; hesap.inc - HESAP menü öğesi: dört işlemli hesap makinesi (menu_altyapi.inc ile)
;
;   m (LIST) m (SAYI) m (HESAP) s   -> "0"
;   0-9      sayı gir (en çok 8 hane, fazlası bip)
;   *        işlem: 1. basış +, art arda basıldıkça  + - * /  arasında döner
;   #        eşittir
;   s        temizle (C)
;   m        çık
;
; Örnek:  1 2 * * * 3 #  ->  "*     3" ... "36"       (12 * 3)
;         7 * * 9 #      ->  "-2"                     (7 - 9)
;         1 0 * 5 * * 3 #  -> 15, sonra "-" ile 15-3 = 12   (zincirleme: yeni işlemde önce öncekini hesaplar)
;
; Sayılar 32 bit işaretli; gösterilebilen aralık -9 999 999 .. 99 999 999. Dışına çıkan sonuç,
; çarpmada taşma ve sıfıra bölme "ERROR" ("HATA") gösterir; s ile silinir. Bölme tamsayı
; bölmedir, sıfıra doğru keser (-7 / 2 = -3).
;
; Ekran durumdan çizilir (HS_EKRAN): hata varsa hata metni, sayı girilmekteyse girilen sayı,
; yoksa birikimli sonuç; bekleyen işlem en soldaki boş haneye yazılır ("+     12").

HS_ACC      .EQU OZEL_RAM+$00   ; 4 bayt: birinci işlenen / sonuç (işaretli)
HS_CUR      .EQU OZEL_RAM+$04   ; 4 bayt: girilmekte olan sayı
HS_T        .EQU OZEL_RAM+$08   ; 4 bayt: ara değer
HS_R        .EQU OZEL_RAM+$0C   ; 4 bayt: ara değer
HS_OP       .EQU OZEL_RAM+$10   ; bekleyen işlem: 0 ya da '+' '-' '*' '/'
HS_NDIG     .EQU OZEL_RAM+$11   ; girilen hane sayısı (0 = sayı girilmiyor)
HS_FLG      .EQU OZEL_RAM+$12   ; bit 0: son tuş işlemdi, bit 1: sonuç gösteriliyor, bit 2: hata
HS_NEG      .EQU OZEL_RAM+$13   ; işaret (1 = negatif)
HS_DIG      .EQU OZEL_RAM+$14   ; yazdırılan hane
HS_BAS      .EQU OZEL_RAM+$15   ; yazdırmada ilk anlamlı hane görüldü

F_SONISLEM  .EQU 1
F_SONUC     .EQU 2
F_HATA      .EQU 4

; ---------------------------------------------------------------------------------------------
; HESAP başlığında s: değişkenleri sıfırla, işleyiciyi HESAP_TUS yap, "0" göster
HESAP_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        LDM   #HESAP_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(HESAP_TUS),HANDLER_BK
        CLM
        JSR   HS_TEMIZLE
        JMP   HK_SON

; ---------------------------------------------------------------------------------------------
; Etkin durumda her tuş. F_BEEP yalnız bu seviyede çağrılır (yığının tepesi çerçeve göstericisi).
HESAP_TUS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        SEM
        LDA   A,KEY
        CMP   A,#'s'
        BEQ   HK_S
        BBS   #F_HATA,HS_FLG,HK_SON     ; hata gösterilirken yalnız s
        CMP   A,#'d'
        BEQ   HK_RAKAM
        CMP   A,#'*'
        BNE   HK_1
        JMP   HK_OP
HK_1:   CMP   A,#'#'
        BNE   HK_SON
        JMP   HK_ESIT

HK_S:   CLM
        JSR   HS_TEMIZLE
HK_SON: CLM
        JSR   HS_EKRAN
        PLA
        RTL

; ---- rakam: firmware onu DIGITS[(NDIGIT-1) & 15]'e koydu; alıp tamponu boşalt
HK_RAKAM:
        .ASSUME M=1,X=0
        LDA   A,NDIGIT
        DEC   A
        CLM
        AND   A,#$000F
        TAX
        SEM
        LDA   A,DIGITS,X
        SEC
        SBC   A,#'0'
        STA   A,HS_DIG
        LDM   #0,NDIGIT
        BBC   #F_SONUC,HS_FLG,HR_1      ; sonuç gösterilirken rakam: yeni hesap
        LDA   A,HS_OP
        BNE   HR_1
        CLM
        LDM   #0,HS_ACC
        LDM   #0,HS_ACC+2
        SEM
HR_1:   LDA   A,HS_NDIG
        CMP   A,#8
        BCC   HR_2
        CLM                             ; 9. hane: bip, yok say
        LDY   #13
        JSRL  F_BEEP
        BRA   HK_SON
HR_2:   CLM                             ; CUR = CUR*10 + rakam  (10x = 8x + 2x)
        LDA   A,HS_CUR
        ASL   A
        STA   A,HS_T
        LDA   A,HS_CUR+2
        ROL   A
        STA   A,HS_T+2                  ; T = 2*CUR
        LDA   A,HS_T
        ASL   A
        STA   A,HS_R
        LDA   A,HS_T+2
        ROL   A
        STA   A,HS_R+2
        ASL   HS_R
        ROL   HS_R+2                    ; R = 8*CUR
        CLC
        LDA   A,HS_R
        ADC   A,HS_T
        STA   A,HS_CUR
        LDA   A,HS_R+2
        ADC   A,HS_T+2
        STA   A,HS_CUR+2
        LDA   A,HS_DIG
        AND   A,#$00FF
        CLC
        ADC   A,HS_CUR
        STA   A,HS_CUR
        LDA   A,HS_CUR+2
        ADC   A,#0
        STA   A,HS_CUR+2
        SEM
        INC   HS_NDIG
        CLB   #F_SONISLEM|F_SONUC,HS_FLG
        JMP   HK_SON

; ---- işlem tuşu
HK_OP:  .ASSUME M=1,X=0
        BBC   #F_SONISLEM,HS_FLG,HO_YENI
        LDA   A,HS_OP                   ; art arda: + -> - -> * -> / -> +
        CMP   A,#'+'
        BNE   HO_1
        LDA   A,#'-'
        BRA   HO_YAZ
HO_1:   CMP   A,#'-'
        BNE   HO_2
        LDA   A,#'*'
        BRA   HO_YAZ
HO_2:   CMP   A,#'*'
        BNE   HO_3
        LDA   A,#'/'
        BRA   HO_YAZ
HO_3:   LDA   A,#'+'
HO_YAZ: STA   A,HS_OP
        JMP   HK_SON
HO_YENI:
        LDA   A,HS_NDIG
        BEQ   HO_KUR                    ; yeni sayı yok: ACC üzerinde işlem seç
        LDA   A,HS_OP
        BEQ   HO_KOPYA
        CLM
        JSR   HS_ISLEM                  ; zincirleme: önce bekleyen işlemi yap
        SEM
        BRA   HO_KUR
HO_KOPYA:
        CLM
        LDA   A,HS_CUR
        STA   A,HS_ACC
        LDA   A,HS_CUR+2
        STA   A,HS_ACC+2
        SEM
HO_KUR: LDM   #'+',HS_OP
        JSR   HS_GIRDI_BITIR
        LDA   A,HS_FLG
        AND   A,#~(F_SONISLEM|F_SONUC)
        ORA   A,#F_SONISLEM
        STA   A,HS_FLG
        JMP   HK_SON

; ---- eşittir
HK_ESIT:
        .ASSUME M=1,X=0
        LDA   A,HS_OP
        BEQ   HE_YOK
        LDA   A,HS_NDIG
        BEQ   HE_BITIR                  ; ikinci sayı girilmedi: işlemi bırak
        CLM
        JSR   HS_ISLEM
        SEM
        BRA   HE_BITIR
HE_YOK: LDA   A,HS_NDIG                 ; işlem yok: girilen sayı sonuç olur
        BEQ   HE_BITIR
        CLM
        LDA   A,HS_CUR
        STA   A,HS_ACC
        LDA   A,HS_CUR+2
        STA   A,HS_ACC+2
        SEM
HE_BITIR:
        LDM   #0,HS_OP
        JSR   HS_GIRDI_BITIR
        LDA   A,HS_FLG
        AND   A,#~(F_SONISLEM|F_SONUC)
        ORA   A,#F_SONUC
        STA   A,HS_FLG
        JMP   HK_SON

; ---------------------------------------------------------------------------------------------
; Alt yordamlar (JSR / RTS, bank 1 içinde). Girişte ve çıkışta belirtilen M ile.

; girilen sayıyı sıfırla (M=1 giriş/çıkış)
HS_GIRDI_BITIR:
        .ASSUME M=1,X=0
        LDM   #0,HS_NDIG
        CLM
        LDM   #0,HS_CUR
        LDM   #0,HS_CUR+2
        SEM
        RTS

; her şeyi sıfırla (M=0)
HS_TEMIZLE:
        .ASSUME M=0,X=0
        LDM   #0,HS_ACC
        LDM   #0,HS_ACC+2
        LDM   #0,HS_CUR
        LDM   #0,HS_CUR+2
        SEM
        LDM   #0,HS_OP
        LDM   #0,HS_NDIG
        LDM   #0,HS_FLG
        CLM
        RTS

; ACC = ACC (HS_OP) CUR, sonra aralık denetimi (M=0)
HS_ISLEM:
        .ASSUME M=0,X=0
        SEM
        LDA   A,HS_OP
        CMP   A,#'-'
        BEQ   HI_CIKAR
        CMP   A,#'*'
        BEQ   HI_CARP
        CMP   A,#'/'
        BEQ   HI_BOL
        CLM                             ; '+'
        CLC
        LDA   A,HS_ACC
        ADC   A,HS_CUR
        STA   A,HS_ACC
        LDA   A,HS_ACC+2
        ADC   A,HS_CUR+2
        STA   A,HS_ACC+2
        JMP   HS_ARALIK
HI_CIKAR:
        CLM
        SEC
        LDA   A,HS_ACC
        SBC   A,HS_CUR
        STA   A,HS_ACC
        LDA   A,HS_ACC+2
        SBC   A,HS_CUR+2
        STA   A,HS_ACC+2
        JMP   HS_ARALIK
HI_CARP:
        CLM
        JMP   HS_CARP
HI_BOL: CLM
        JMP   HS_BOL

; ACC = |ACC| * CUR, işaret geri konur. Kaydır-topla: CUR (çarpan) sağa, T (çarpılan) sola.
HS_CARP:
        .ASSUME M=0,X=0
        JSR   HS_MUTLAK
        LDA   A,HS_ACC
        STA   A,HS_T
        LDA   A,HS_ACC+2
        STA   A,HS_T+2
        LDM   #0,HS_R
        LDM   #0,HS_R+2
HC_DON: LDA   A,HS_CUR
        ORA   A,HS_CUR+2
        BEQ   HC_BITTI
        LSR   HS_CUR+2
        ROR   HS_CUR                    ; en düşük bit -> C
        BCC   HC_KAYDIR
        CLC
        LDA   A,HS_R
        ADC   A,HS_T
        STA   A,HS_R
        LDA   A,HS_R+2
        ADC   A,HS_T+2
        STA   A,HS_R+2
        BCS   HC_TASMA
HC_KAYDIR:
        LDA   A,HS_CUR
        ORA   A,HS_CUR+2
        BEQ   HC_BITTI
        ASL   HS_T                      ; çarpan bitmediyse çarpılan 32 biti aşamaz
        ROL   HS_T+2
        BCS   HC_TASMA
        BRA   HC_DON
HC_BITTI:
        LDA   A,HS_R+2
        BMI   HC_TASMA                  ; >= 2^31
        STA   A,HS_ACC+2
        LDA   A,HS_R
        STA   A,HS_ACC
        JSR   HS_ISARET
        JMP   HS_ARALIK
HC_TASMA:
        JMP   HS_HATA

; ACC = |ACC| / CUR (tamsayı), işaret geri konur. 32 adımlı kalanlı bölme, kalan R'de.
HS_BOL:
        .ASSUME M=0,X=0
        LDA   A,HS_CUR
        ORA   A,HS_CUR+2
        BNE   HB_1
        JMP   HS_HATA                   ; sıfıra bölme
HB_1:   JSR   HS_MUTLAK
        LDM   #0,HS_R
        LDM   #0,HS_R+2
        LDX   #32
HB_DON: ASL   HS_ACC                    ; R:ACC sola kaydır
        ROL   HS_ACC+2
        ROL   HS_R
        ROL   HS_R+2
        SEC                             ; R >= CUR ise R -= CUR, bölüm bitini 1 yap
        LDA   A,HS_R
        SBC   A,HS_CUR
        TAY
        LDA   A,HS_R+2
        SBC   A,HS_CUR+2
        BCC   HB_2
        STA   A,HS_R+2
        STY   HS_R
        INC   HS_ACC                    ; kaydırmadan sonra bit 0 boş
HB_2:   DEX
        BNE   HB_DON
        JSR   HS_ISARET
        JMP   HS_ARALIK

; ACC negatifse HS_NEG = 1 ve ACC = -ACC (M=0)
HS_MUTLAK:
        .ASSUME M=0,X=0
        SEM
        LDM   #0,HS_NEG
        CLM
        LDA   A,HS_ACC+2
        BPL   HM_1
        SEM
        LDM   #1,HS_NEG
        CLM
        JSR   HS_EKSI_ACC
HM_1:   RTS

; HS_NEG = 1 ise ACC = -ACC (M=0)
HS_ISARET:
        .ASSUME M=0,X=0
        SEM
        LDA   A,HS_NEG
        CLM
        BEQ   HM_1
HS_EKSI_ACC:
        LDA   A,HS_ACC
        EOR   A,#$FFFF
        CLC
        ADC   A,#1
        STA   A,HS_ACC
        LDA   A,HS_ACC+2
        EOR   A,#$FFFF
        ADC   A,#0
        STA   A,HS_ACC+2
        RTS

; -9 999 999 <= ACC <= 99 999 999 değilse hata (M=0)
HS_ARALIK:
        .ASSUME M=0,X=0
        LDA   A,HS_ACC+2
        BMI   HA_EKSI
        CMP   A,#99999999 >> 16
        BCC   HA_TAMAM
        BNE   HS_HATA
        LDA   A,HS_ACC
        CMP   A,#(99999999 + 1) & $FFFF
        BCS   HS_HATA
HA_TAMAM:
        RTS
HA_EKSI:
        CMP   A,#(-9999999 >> 16) & $FFFF
        BCC   HS_HATA
        BNE   HA_TAMAM
        LDA   A,HS_ACC
        CMP   A,#-9999999 & $FFFF
        BCC   HS_HATA
        RTS
HS_HATA:
        SEM
        LDA   A,HS_FLG
        ORA   A,#F_HATA
        STA   A,HS_FLG
        CLM
        RTS

; ---------------------------------------------------------------------------------------------
; Ekranı çiz (M=0 giriş/çıkış): hata metni, girilen sayı ya da sonuç; sağa dayalı
HS_EKRAN:
        .ASSUME M=0,X=0
        SEM
        BBC   #F_HATA,HS_FLG,HY_SAYI
        LDX   #0
        BBS   #$80,OPTIONS,HY_TR
HY_EN:  LDA   A,>TXT_ERROR,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   HY_EN
        JMP   HY_SON
HY_TR:  LDA   A,>TXT_HATA,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   HY_TR
        JMP   HY_SON

HY_SAYI:
        LDA   A,HS_NDIG                 ; R = gösterilecek sayı
        CLM
        BEQ   HY_ACC
        LDA   A,HS_CUR
        STA   A,HS_R
        LDA   A,HS_CUR+2
        STA   A,HS_R+2
        BRA   HY_ISARET
HY_ACC: LDA   A,HS_ACC
        STA   A,HS_R
        LDA   A,HS_ACC+2
        STA   A,HS_R+2
HY_ISARET:
        SEM
        LDM   #0,HS_NEG
        LDM   #0,HS_BAS
        CLM
        LDA   A,HS_R+2
        BPL   HY_BASLA
        SEM
        LDM   #1,HS_NEG
        CLM
        LDA   A,HS_R                    ; R = -R
        EOR   A,#$FFFF
        CLC
        ADC   A,#1
        STA   A,HS_R
        LDA   A,HS_R+2
        EOR   A,#$FFFF
        ADC   A,#0
        STA   A,HS_R+2

; Her konum i (Y = 0..7) için basamak 10^(7-i): çıkarma ile bul (X = 4*i, tablo POW10)
HY_BASLA:
        LDX   #0
        LDY   #0
HY_KONUM:
        SEM
        LDM   #'0',HS_DIG
        CLM
HY_CIKAR:
        SEC
        LDA   A,HS_R
        SBC   A,>POW10,X
        STA   A,HS_T
        LDA   A,HS_R+2
        SBC   A,>POW10+2,X
        BCC   HY_HANE
        STA   A,HS_R+2
        LDA   A,HS_T
        STA   A,HS_R
        SEM
        INC   HS_DIG
        CLM
        BRA   HY_CIKAR
HY_HANE:
        SEM
        LDA   A,HS_DIG
        CMP   A,#'0'
        BNE   HY_GOSTER
        LDA   A,HS_BAS
        BNE   HY_GOSTER
        CPY   #7                        ; son hane her zaman gösterilir
        BEQ   HY_GOSTER
        LDA   A,#' '                    ; baştaki sıfır
        BRA   HY_KOY
HY_GOSTER:
        LDA   A,HS_BAS
        BNE   HY_G2
        LDM   #1,HS_BAS                 ; ilk anlamlı hane: önüne eksi
        LDA   A,HS_NEG
        BEQ   HY_G2
        LDA   A,#'-'
        STA   A,LCD-1,Y                 ; negatif en çok 7 hane: Y >= 1
HY_G2:  LDA   A,HS_DIG
HY_KOY: STA   A,LCD,Y
        CLM
        INX
        INX
        INX
        INX
        INY
        CPY   #8
        BNE   HY_KONUM

        SEM                             ; bekleyen işlem en soldaki boş haneye
        LDA   A,HS_OP
        BEQ   HY_SON
        LDA   A,LCD
        CMP   A,#' '
        BNE   HY_SON
        LDA   A,HS_OP
        STA   A,LCD
HY_SON: CLM
        RTS

POW10:  .DWORD 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1
TXT_ERROR:
        .TEXT8 "ERROR"
TXT_HATA:
        .TEXT8 "HATA"
