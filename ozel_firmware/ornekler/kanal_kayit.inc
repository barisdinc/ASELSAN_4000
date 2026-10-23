; kanal_kayit.inc - VFO'yu hafıza kanalına kaydet (AT V0-33)
;
; VFO kuruluyken (FREKANS ile alma / gönderme frekansı ve tonlar ayarlanmış; normal ekranda ya da
; açık kalan FREKANS ekranında) üçgen (q) tuşunu ~1 s basılı tutun:
;   "KAYIT __"   kanal numarasını yazın (1 ya da 2 hane, 1..99): "KAYIT 0_", "KAYIT 05"
;   s            kaydet: EEPROM'daki kanal kaydına yazılır, ekranda ~1,5 s "TAMAM 05"
;   başka tuş    iptal ("IPTAL"); ~8 s tuşa basılmazsa da iptal
; Hane girmeden ya da 00 ile s: hata sesi, kayıt ekranı kalır. VFO kurulu değilse "VFO YOK".
; Yazılan değerler kaydın tamamı (disasm/sim/p4000.js, docs/p4000_eeprom.md):
;   +0 b7 HPRF, b6 TOT, b5..0 + +1 TX kelimesi   gönderme frekansı; HPRF p ile seçilen güç
;                                                 (seçilmediyse kanalınki), TOT kanalınki ($037C)
;   +2..+3 RX kelimesi                            alma frekansı
;   +4 b6 kristal kaydırma, b5..0 RX TKS          P4000 kuralı (0x4885B1), alma tonu; ANI 0
;   +5..+12 b6..0 kanal adı                       alma frekansının metni ("145 5000")
;   +5 b7, +6..+11 b7 TX TKS                      gönderme tonu
; Tarama listesi biti değişmez. AKTARMA bölümünün sağlaması (0x535-0x536, 0x018-0x534 bayt
; toplamı, büyük endian) eski kayıt çıkarılıp yenisi eklenerek düzeltilir; yazılan okunup
; karşılaştırılır, farklıysa "KYT HATA".
;
; Kelimeler VFO'nun metninden FR_HESAPLA ile yeniden hesaplanır (FR_* ara değişkenleri; FREKANS
; ekranında FR_HANE sonra alma hanelerine geri konur). Kayıt yığında 40 baytlık bir tamponda
; hazırlanır (D = tampon); EEPROM yordamları TUS_ISLE'nin boş çerçevesiyle çağrılır.
;
; RAM: 0x044D..0x0452 (hiçbir kodun adreslemediği, ölçümde açılıştan sonra erişilmeyen baytlar,
; disasm/sim/ram_bos.js).

KY_H        .EQU $044D          ; 2 bayt: girilen haneler (ASCII), boş '_'
KY_DURUM    .EQU $044F          ; bit 7 kayıt modu, bit 6 iptal istendi, bit 5 kaydet istendi
KY_SURE     .EQU $0450          ; son tuşun tığı (zaman aşımı)
KY_F        .EQU $0451          ; 2 bayt: EEPROM yordamları için boş çerçeve
KY_ACIK     .EQU $80
KY_IPTAL    .EQU $40
KY_ONAY     .EQU $20
KY_ZA       .EQU 250            ; 250 x 32 ms = 8 s

EE_AKTARMA_T .EQU $0535         ; AKTARMA sağlaması (büyük endian)
KANAL_0     .EQU $002E          ; kanal n: KANAL_0 + 13*(n-1)

; yığın tamponu (D'ye göre)
KB_YENI     .EQU $00            ; 13 bayt: yazılacak kayıt
KB_ESKI     .EQU $10            ; 13 bayt: eski kayıt / geri okunan
KB_TOPLAM   .EQU $1D            ; 2 bayt: sağlama (büyük endian)
KB_ADR      .EQU $20            ; 2 bayt: kaydın EEPROM adresi
KB_T        .EQU $22            ; 2 bayt: ara değer
KB_S        .EQU $24            ; 2 bayt: toplam (küçük endian)
KB_D        .EQU $26            ; 2 bayt: tamponun adresi
KB_UZ       .EQU 40

; ---------------------------------------------------------------------------------------------
; q ~1 s basılı tutuldu (TUS_ISLE'den JMP; M = 1, çerçeve $0E,S). TUS_ISLE gibi M = 0 ile RTS.
KY_BASLA:
        .ASSUME M=1,X=0
        LDA   A,MENU
        BEQ   KS_1
        CMP   A,#ST_FREKANS_ON
        BNE   KS_SON
        LDA   A,FR_DURUM                ; FREKANS girişinin ortasında değil
        BEQ   KS_1
        LDY   #13
        JSR   KY_BIP
KS_SON: JMP   TI_SON
KS_1:   CLM
        JSR   VFO_ETKIN                 ; Z = 1: VFO kurulu
        SEM
        BEQ   KS_2
        LDY   #13
        JSR   KY_BIP
        LDX   #TM_VFOYOK-TUS_METIN
        JMP   TI_GOSTER
KS_2:   LDM   #'_',KY_H
        LDM   #'_',KY_H+1
        LDM   #KY_ACIK,KY_DURUM
        LDA   A,TIK_32MS
        STA   A,KY_SURE
        CLB   #$80,TUS_IS               ; gösterilen mesaj varsa biter
        LDY   #3                        ; onay sesi
        JSR   KY_BIP
        JMP   KY_CIZ

; Kayıt modunda her turda (TUS_ISLE'den JMP; M = 1, çerçeve $0E,S)
KY_ISLE:
        .ASSUME M=1,X=0
        BBC   #KY_IPTAL,KY_DURUM,KI_1
KI_IPTAL:
        LDY   #13
        JSR   KY_BIP
        LDX   #TM_IPTAL-TUS_METIN
        BRA   KI_BITIR
KI_1:   BBS   #KY_ONAY,KY_DURUM,KI_ONAY
        LDA   A,TIK_32MS                ; zaman aşımı
        SEC
        SBC   A,KY_SURE
        CMP   A,#KY_ZA
        BCS   KI_IPTAL
        JMP   KY_CIZ
KI_ONAY:
        CLB   #KY_ONAY,KY_DURUM
        JSR   KY_NUMARA
        BNE   KI_YAZ
        LDY   #13                       ; numara yok ya da 00
        JSR   KY_BIP
        JMP   KY_CIZ
KI_YAZ: CLM
        JSR   KY_YAZ                    ; C = 1: X = hata metni
        SEM
        BCS   KI_HATA
        LDA   A,KY_H+1                  ; tek hane: "7_" -> "07"
        CMP   A,#'_'
        BNE   KI_Y1
        LDA   A,KY_H
        STA   A,KY_H+1
        LDM   #'0',KY_H
KI_Y1:  SEB   #$08,TUS_IS               ; mesaja numara
        LDY   #3
        JSR   KY_BIP
        LDX   #TM_TAMAM-TUS_METIN
        BRA   KI_BITIR
KI_HATA:
        PHX
        LDY   #13
        JSR   KY_BIP
        PLX
KI_BITIR:                               ; X = mesaj. Ekrana kanal adını koy ki TI_GOSTER mesajı
        LDM   #0,KY_DURUM               ; göstersin ve süresi dolunca geri yazsın
        PHX
        LDX   #0
KI_AD:  LDA   A,KANAL_ADI,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   KI_AD
        LDA   A,MENU                    ; FREKANS ekranında girişe alma frekansı geri konur
        CMP   A,#ST_FREKANS_ON
        BNE   KI_B1
        CLM
        JSR   FR_RX_HANELER
        SEM
KI_B1:  PLX
        JMP   TI_GOSTER

; "KAYIT " + haneler; ekrandakinden farklıysa yaz ve çiz (firmware ekranı yenilerse geri gelir)
KY_CIZ:
        .ASSUME M=1,X=0
        LDX   #0
        LDY   #0                        ; Y = 1: değişti
KC_1:   CPX   #6
        BCS   KC_H
        LDA   A,>TM_KAYIT,X
        BRA   KC_2
KC_H:   LDA   A,!KY_H-6,X
KC_2:   CMP   A,LCD,X
        BEQ   KC_3
        STA   A,LCD,X
        LDY   #1
KC_3:   INX
        CPX   #8
        BNE   KC_1
        CPY   #0
        BEQ   KC_SON
        JMP   TI_CIZ
KC_SON: JMP   TI_SON

; TUS_KANCA'dan (M = 1, A = tuş). Bütün tuşlar yutulur.
KY_TUS:
        .ASSUME M=1,X=0
        CMP   A,#'q'
        BNE   KT_1
        LDA   A,TUS_P                   ; kaydı açan q hâlâ basılı: tekrarları yut
        BNE   KT_YUT
        BRA   KT_IPTAL
KT_1:   CMP   A,#'s'
        BNE   KT_2
        SEB   #KY_ONAY,KY_DURUM
        BRA   KT_YUT
KT_2:   CMP   A,#'0'
        BCC   KT_IPTAL
        CMP   A,#'9'+1
        BCS   KT_IPTAL
        LDX   #0                        ; ilk boş haneye
        PHA
        LDA   A,KY_H
        CMP   A,#'_'
        BEQ   KT_3
        INX
        LDA   A,KY_H+1
        CMP   A,#'_'
        BEQ   KT_3
        PLA                             ; iki hane dolu: yok say
        BRA   KT_YUT
KT_3:   PLA
        STA   A,!KY_H,X
        LDA   A,TIK_32MS
        STA   A,KY_SURE
        BRA   KT_YUT
KT_IPTAL:
        SEB   #KY_IPTAL,KY_DURUM
KT_YUT: JMP   TK_YUT

; A = girilen kanal numarası (0 = yok), Z = 1 ise 0. M = 1.
KY_NUMARA:
        .ASSUME M=1,X=0
        LDA   A,KY_H
        CMP   A,#'_'
        BEQ   KN_0
        AND   A,#$0F
        PHA
        LDA   A,KY_H+1
        CMP   A,#'_'
        BEQ   KN_TEK
        PLA                             ; onlar * 10 = *2 + *8
        ASL   A
        PHA
        ASL   A
        ASL   A
        CLC
        ADC   A,$01,S
        STA   A,$01,S
        LDA   A,KY_H+1
        AND   A,#$0F
        CLC
        ADC   A,$01,S
        STA   A,$01,S
KN_TEK: PLA
        CMP   A,#0
        RTS
KN_0:   LDA   A,#0
        RTS

; Y = ses (3 onay, 13 hata). TUS_ISLE bağlamından JSR ile: çerçeve $10,S. M = 1 giriş ve çıkış.
KY_BIP:
        .ASSUME M=1,X=0
        CLM
        LDA   A,$10,S
        PHA
        JSRL  F_BEEP
        CLM
        PLA
        SEM
        RTS

; ---------------------------------------------------------------------------------------------
; VFO'yu KY_H kanalına yaz. KY_ISLE'den JSR (çerçeve $10,S). M = 0 giriş ve çıkış.
; C = 0 başarı; C = 1: X = hata metni (VFO YOK / KYT HATA).
KY_YAZ:
        .ASSUME M=0,X=0
        LDA   A,$10,S
        STA   A,KY_F
        JSR   VFO_ETKIN
        BEQ   KY_V
        LDX   #TM_VFOYOK-TUS_METIN
        SEC
        RTS
KY_V:   PHD
        TSX                             ; yığında KB_UZ bayt ayır, D = tampon
        TXA
        SEC
        SBC   A,#KB_UZ
        TAX
        TXS
        INC   A
        TAD
        STA   A,KB_D
        SEM                             ; adres = KANAL_0 + 13 * (n - 1)
        JSR   KY_NUMARA
        CLM
        AND   A,#$00FF
        DEC   A
        STA   A,KB_ADR
        ASL   A
        ASL   A
        STA   A,KB_T                    ; 4(n-1)
        ASL   A
        CLC
        ADC   A,KB_T
        CLC
        ADC   A,KB_ADR
        CLC
        ADC   A,#KANAL_0
        STA   A,KB_ADR
        ; alma kelimesi (+2, +3) ve kristal kaydırma
        LDX   #0
        SEM
KY_RXH: LDA   A,VFO_RX_HANE,X
        STA   A,FR_HANE,X
        INX
        CPX   #7
        BNE   KY_RXH
        LDM   #7,FR_NDIG
        CLM
        JSR   FR_HESAPLA
        BCC   KY_RX1
        JMP   KY_HATA
KY_RX1: SEM
        LDA   A,FR_KELIME+1
        STA   A,KB_YENI+2
        LDA   A,FR_KELIME
        STA   A,KB_YENI+3
        CLM
        JSR   KY_KRISTAL                ; A = $40 / 0
        SEM
        STA   A,KB_T
        LDA   A,VFO_RX_TON              ; +4: ANI 0, kristal, RX tonu
        AND   A,#$3F
        ORA   A,KB_T
        STA   A,KB_YENI+4
        ; gönderme kelimesi (+0, +1): metin "160 6500", 4. karakter boşluk
        LDX   #0
        LDY   #0
KY_TXH: CPY   #3
        BNE   KY_TX1
        INY
KY_TX1: LDA   A,VFO_METIN+8,Y
        STA   A,FR_HANE,X
        INX
        INY
        CPX   #7
        BNE   KY_TXH
        LDM   #7,FR_NDIG
        CLM
        JSR   FR_HESAPLA
        BCC   KY_TX2
        JMP   KY_HATA
KY_TX2: SEM
        LDA   A,FR_KELIME
        STA   A,KB_YENI+1
        LDA   A,SYN_KAYIT+12            ; $037C: b7 HPRF, b6 TOT
        AND   A,#$C0
        BBC   #$40,$0240,KY_G           ; p ile güç seçildiyse HPRF = seçilen
        AND   A,#$40
        BBC   #$10,$023C,KY_G
        ORA   A,#$80
KY_G:   STA   A,KB_T
        LDA   A,FR_KELIME+1
        AND   A,#$3F
        ORA   A,KB_T
        STA   A,KB_YENI
        ; ad (+5..+12, b6..0) ve gönderme tonu (+5 b7 var, +6..+11 b7 değer, ilk bayt en anlamlı)
        LDX   #0
KY_AD:  LDA   A,VFO_METIN,X
        AND   A,#$7F
        STA   A,KB_YENI+5,X
        INX
        CPX   #8
        BNE   KY_AD
        LDA   A,VFO_TX_TON
        AND   A,#$3F
        BEQ   KY_OKU
        SEB   #$80,KB_YENI+5
        STA   A,KB_T
        LDX   #11
KY_TB:  LSR   KB_T
        BCC   KY_TB1
        LDA   A,KB_YENI,X
        ORA   A,#$80
        STA   A,KB_YENI,X
KY_TB1: DEX
        CPX   #6
        BCS   KY_TB
KY_OKU: ; eski kayıt ve sağlama
        CLM
        LDA   A,KB_ADR
        LDX   #KB_ESKI
        LDY   #13
        JSR   KY_EE_OKU
        LDA   A,#EE_AKTARMA_T
        LDX   #KB_TOPLAM
        LDY   #2
        JSR   KY_EE_OKU
        SEM                             ; toplam = eski - eski kayıt + yeni kayıt
        LDA   A,KB_TOPLAM
        STA   A,KB_S+1
        LDA   A,KB_TOPLAM+1
        STA   A,KB_S
        CLM
        LDX   #0
KY_TOP: LDA   A,KB_ESKI,X
        AND   A,#$00FF
        STA   A,KB_T
        LDA   A,KB_S
        SEC
        SBC   A,KB_T
        STA   A,KB_S
        LDA   A,KB_YENI,X
        AND   A,#$00FF
        CLC
        ADC   A,KB_S
        STA   A,KB_S
        INX
        CPX   #13
        BNE   KY_TOP
        SEM
        LDA   A,KB_S+1
        STA   A,KB_TOPLAM
        LDA   A,KB_S
        STA   A,KB_TOPLAM+1
        CLM
        ; yaz
        LDA   A,KB_ADR
        LDX   #KB_YENI
        LDY   #13
        JSR   KY_EE_YAZ
        LDA   A,#EE_AKTARMA_T
        LDX   #KB_TOPLAM
        LDY   #2
        JSR   KY_EE_YAZ
        ; geri oku, karşılaştır
        LDA   A,KB_ADR
        LDX   #KB_ESKI
        LDY   #13
        JSR   KY_EE_OKU
        LDX   #0
        SEM
KY_K1:  LDA   A,KB_ESKI,X
        CMP   A,KB_YENI,X
        BNE   KY_HATA
        INX
        CPX   #13
        BNE   KY_K1
        CLM
        LDA   A,#EE_AKTARMA_T
        LDX   #KB_ESKI
        LDY   #2
        JSR   KY_EE_OKU
        LDA   A,KB_ESKI
        CMP   A,KB_TOPLAM
        BNE   KY_HATA
        LDY   #0
        BRA   KY_CIK
KY_HATA:
        LDY   #TM_KHATA-TUS_METIN
KY_CIK: CLM                             ; Y = 0 ya da hata metni
        TSX
        TXA
        CLC
        ADC   A,#KB_UZ
        TAX
        TXS
        PLD
        TYX
        CPY   #1                        ; C = 1: hata
        RTS

; EEPROM oku / yaz: A = EEPROM adresi, X = tampondaki yer (D'ye göre), Y = bayt sayısı.
; D = tampon, M = 0 giriş ve çıkış. Firmware yordamları KY_F çerçevesiyle çağrılır.
KY_EE_OKU:
        .ASSUME M=0,X=0
        CLC
        BRA   KE_1
KY_EE_YAZ:
        SEC
KE_1:   PHP
        PHA
        TXA
        CLC
        ADC   A,KB_D
        TAX                             ; X = tamponun mutlak adresi
        PLA
        PHD
        PHA
        LDA   A,KY_F
        TAD
        PLA
        STA   A,$04                     ; EEPROM adresi
        STX   $01                       ; hedef / kaynak (bank 0)
        TYA
        SEM
        STA   A,$00                     ; sayı
        LDM   #0,$03
        CLM
        TDA
        PHA
        LDY   #$00A0
        LDA   A,$05,S                   ; PHP'nin baytı: C = yaz
        AND   A,#$0001
        BNE   KE_YAZ
        JSRL  F_EE_OKU
        BRA   KE_SON
KE_YAZ: JSRL  F_EE_YAZ
KE_SON: CLM
        PLA
        PLD
        PLP
        RTS

; Kristal kaydırma biti, P4000'in kuralı (0x4885B1): alma frekansı f, k = 289..346 için
; k * 504 kHz - 25 kHz <= f <= k * 504 kHz + 20 kHz ise 1. 0,5 kHz birimiyle: v = 5 * FR_FU,
; v = 1008 q + r; r <= 40 ise k = q, r >= 958 ise k = q + 1. A = $40 / 0 (M = 0).
KY_KRISTAL:
        .ASSUME M=0,X=0
        LDA   A,FR_FU
        STA   A,FR_BOLUNEN
        LDA   A,FR_FU+2
        STA   A,FR_BOLUNEN+2
        ASL   FR_BOLUNEN
        ROL   FR_BOLUNEN+2
        ASL   FR_BOLUNEN
        ROL   FR_BOLUNEN+2
        CLC
        LDA   A,FR_BOLUNEN
        ADC   A,FR_FU
        STA   A,FR_BOLUNEN
        LDA   A,FR_BOLUNEN+2
        ADC   A,FR_FU+2
        STA   A,FR_BOLUNEN+2
        LDA   A,#1008
        JSR   FR_BOL
        LDA   A,FR_BOLUNEN+2
        BNE   KK_0
        LDA   A,FR_KALAN
        CMP   A,#41
        BCC   KK_ALT
        CMP   A,#958
        BCC   KK_0
        LDA   A,FR_BOLUNEN
        INC   A
        BRA   KK_K
KK_ALT: LDA   A,FR_BOLUNEN
KK_K:   CMP   A,#289
        BCC   KK_0
        CMP   A,#347
        BCS   KK_0
        LDA   A,#$0040
        RTS
KK_0:   LDA   A,#0
        RTS
