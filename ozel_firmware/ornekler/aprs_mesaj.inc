; aprs_mesaj.inc - APRS almada paketin tamamı ekranda kayan yazı (aprs_rx.inc ile)
;
; Sağlaması tutan paket TNC2 biçiminde gösterilir:  TA7W-7>APRS,WIDE1-1*:!4050.63N/03109.39E>hello
; (gönderen > hedef, yol (* = aktarılmış), ':' bilgi alanı). İlk 8 karakter 1 s durur, sonra
; metin her 320 ms'de bir karakter sola kayar; sonu 1 s durur, APRS ekranına dönülür. Kayarken
; gelen paketler listeye eklenir ama gösterilmez. Bir tuş ya da PTT kaymayı keser.
; Paketin ilk 96 baytı (FCS'siz) saklanır; daha uzun paketin sonu gösterilmez.
; Küçük harfler büyük yazılır; LCD fontunda olmayan işaretler glif.inc'te çizilir.
; Kod seçmeli çağrıdan boşalan alanda (secmeli_kaldir.inc'ten sonra eklenmeli).

KY_ILK      .EQU 31             ; 31 x 32 ms ~ 1 s
KY_ADIM_SURE .EQU 10            ; 320 ms / karakter

        .ORG $01BBE4

; Ana döngüden (AR_ISLE, M = 1). C = 1: LCD yazıldı (çağıran F_LCD'yi çağırır). M = 1 döner.
AR_KAYAN:
        .ASSUME M=1,X=0
        LDA   A,RADYO_DURUM
        CMP   A,#4
        BCC   KY_RX
        LDA   A,AR_KDURUM               ; gönderiyor: kayma kesilir
        BEQ   KY_HAYIR
        LDM   #0,AR_KDURUM
        LDM   #0,AR_KILIT
KY_HAYIR:
        CLC
        RTS
KY_RX:  LDA   A,AR_KDURUM
        BNE   KY_SURE
        LDA   A,AR_KILIT
        BEQ   KY_HAYIR
        LDM   #1,AR_KDURUM              ; yeni paket: baştan göster
        LDM   #0,AR_KAY
        LDM   #KY_ILK,AR_KSURE
        LDM   #0,AR_GOSTER
        LDM   #0,AR_SEC
        JSR   KY_CIZ
        SEC
        RTS
KY_SURE:
        LDA   A,AP_FARK                 ; geçen 32 ms adımı (AP_OTO_ZAMAN)
        BEQ   KY_HAYIR
        LDA   A,AR_KSURE
        SEC
        SBC   A,AP_FARK
        BCC   KY_ZAMAN
        BEQ   KY_ZAMAN
        STA   A,AR_KSURE
        CLC
        RTS
KY_ZAMAN:
        LDA   A,AR_KAY                  ; sonu ekranda mı?
        CLC
        ADC   A,#8
        CMP   A,AR_KTOP
        BCC   KY_KAY
        LDA   A,AR_KDURUM
        CMP   A,#2
        BEQ   KY_BITTI
        LDM   #2,AR_KDURUM              ; sonda 1 s bekle
        LDM   #KY_ILK,AR_KSURE
        CLC
        RTS
KY_BITTI:
        CLM
        JSR   AP_EKRAN                  ; APRS ekranı; kayma ve kilit sıfırlanır
        SEM
        SEC
        RTS
KY_KAY: INC   AR_KAY
        LDM   #KY_ADIM_SURE,AR_KSURE
        JSR   KY_CIZ
        SEC
        RTS

; LCD = metnin AR_KAY'dan başlayan 8 karakteri; AR_KTOP = metnin uzunluğu (M = 1)
KY_CIZ:
        .ASSUME M=1,X=0
        LDM   #0,AR_KI
        LDX   #7                        ; gönderen
        JSR   KY_ADRES
        LDA   A,#'>'
        JSR   KY_YAZ
        LDX   #0                        ; hedef
        JSR   KY_ADRES
        LDX   #14                       ; yol
KC_YOL: LDA   A,AR_MSJ-1,X              ; önceki adresin SSID baytı: bit 0 = son adres
        AND   A,#$01
        BNE   KC_BILGI
        TXA
        CLC
        ADC   A,#7
        CMP   A,AR_MN
        BEQ   KC_Y1
        BCS   KC_SON                    ; kesik paket
KC_Y1:  LDA   A,#','
        JSR   KY_YAZ
        JSR   KY_ADRES
        LDA   A,AR_MSJ-1,X              ; H biti: aktarılmış
        BPL   KC_YOL
        LDA   A,#'*'
        JSR   KY_YAZ
        BRA   KC_YOL
KC_BILGI:
        INX                             ; kontrol, PID
        INX
        LDA   A,#':'
        JSR   KY_YAZ
KC_B1:  TXA
        CMP   A,AR_MN
        BCS   KC_SON
        LDA   A,AR_MSJ,X
        CMP   A,#'a'
        BCC   KC_B2
        CMP   A,#'z'+1
        BCS   KC_B2
        SEC
        SBC   A,#$20                    ; büyük harf
KC_B2:  CMP   A,#$20
        BCC   KC_B3
        CMP   A,#$7F
        BCC   KC_B4
KC_B3:  LDA   A,#' '                    ; denetim / UTF-8 baytları
KC_B4:  JSR   KY_YAZ
        INX
        BRA   KC_B1
KC_SON: LDA   A,AR_KI
        STA   A,AR_KTOP
        LDA   A,#' '                    ; metin bittiyse pencerenin kalanı boş (her konum bir kez yazılır)
KC_BOS: JSR   KY_YAZ
        LDA   A,AR_KI
        SEC
        SBC   A,AR_KAY
        BCC   KC_BOS1
        CMP   A,#8
        BCS   KC_BOS2
KC_BOS1:
        LDA   A,#' '
        BRA   KC_BOS
KC_BOS2:
        RTS

; AR_MSJ[X..X+6] adresini yaz ("TA7W-7"); X = X + 7 döner (M = 1)
KY_ADRES:
        .ASSUME M=1,X=0
        LDY   #6
KA_1:   LDA   A,AR_MSJ,X
        LSR   A
        CMP   A,#' '
        BEQ   KA_2
        JSR   KY_YAZ
KA_2:   INX
        DEY
        BNE   KA_1
        LDA   A,AR_MSJ,X                ; SSID
        INX
        LSR   A
        AND   A,#$0F
        BEQ   KA_SON
        PHA
        LDA   A,#'-'
        JSR   KY_YAZ
        PLA
        CMP   A,#10
        BCC   KA_3
        PHA
        LDA   A,#'1'
        JSR   KY_YAZ
        PLA
        SEC
        SBC   A,#10
KA_3:   CLC
        ADC   A,#'0'
        JSR   KY_YAZ
KA_SON: RTS

; Metnin AR_KI'nci karakteri A: ekran penceresindeyse LCD'ye. X, Y korunur (M = 1)
KY_YAZ:
        .ASSUME M=1,X=0
        PHY
        STA   A,AR_KX
        LDA   A,AR_KI
        SEC
        SBC   A,AR_KAY
        BCC   KYZ_1
        CMP   A,#8
        BCS   KYZ_1
        CLM
        AND   A,#$00FF
        TAY
        SEM
        LDA   A,AR_KX
        STA   A,LCD,Y
KYZ_1:  INC   AR_KI
        BNE   KYZ_2
        DEC   AR_KI                     ; 255'te kal
KYZ_2:  PLY
        RTS

; ---------------------------------------------------------------------------------------------
; (aprs_rx.inc) ; Teşhis sayfası (M = 1 giriş ve çıkış): 1 KNR en çok kenar / 32 ms, 2 TSY taşıyıcı ($023E bit 5)
; ve çözücü (TB1) açık mı, 3 BYR bayrak, 4 CRC bozuk çerçeve, 5 OK geçerli paket
AR_TANI:
        .ASSUME M=1,X=0
        LDM   #16,AR_DG_SURE
        LDA   A,AR_DG_SAYFA
        DEC   A
        ASL   A
        ASL   A
        ASL   A
        CLM
        AND   A,#$00FF
        TAX
        LDY   #0
        SEM
RTN_1:  LDA   A,>AR_TANI_M,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   RTN_1
        LDA   A,AR_DG_SAYFA
        CMP   A,#2
        BNE   RTN_SAYI
        LDA   A,#'0'                    ; TSY: "TSY 1 1"
        BBC   #$20,$023E,RTN_2
        INC   A
RTN_2:  STA   A,LCD+4
        LDA   A,#'0'
        BBC   #$40,R_TCSTART,RTN_3
        INC   A
RTN_3:  STA   A,LCD+6
        RTS
RTN_SAYI:
        CLM
        LDX   #0                        ; sayfa 1 KNR, 3 BYR, 4 CRC, 5 OK -> sayaç
        CMP   A,#1
        BNE   RTN_4
        LDA   A,AR_DG_KMAX
        LDM   #0,AR_DG_KMAX             ; bir sonraki gösterime kadarki en çok
        BRA   RTN_YAZ
RTN_4:  CMP   A,#3
        BNE   RTN_5
        LDA   A,AR_DG_BYR
        BRA   RTN_YAZ
RTN_5:  CMP   A,#4
        BNE   RTN_6
        LDA   A,AR_DG_CRC
        BRA   RTN_YAZ
RTN_6:  LDA   A,AR_DG_OK
RTN_YAZ:                                ; LCD+4..+7 = A (en çok 9999)
        CMP   A,#10000
        BCC   RTN_7
        LDA   A,#9999
RTN_7:  STA   A,AR_KI                   ; (AR_KI / AR_KX: kayan yazının geçicileri, burada boşta)
        LDY   #4
        LDX   #0
RTN_HANE:
        SEM
        LDM   #'0',AR_T
        CLM
RTN_8:  LDA   A,AR_KI
        CMP   A,>AR_BOLEN,X
        BCC   RTN_9
        SBC   A,>AR_BOLEN,X
        STA   A,AR_KI
        SEM
        INC   AR_T
        CLM
        BRA   RTN_8
RTN_9:  SEM
        LDA   A,AR_T
        STA   A,LCD,Y
        INY
        INX
        INX
        CPX   #8
        BNE   RTN_HANE
        RTS

AR_BOLEN:
        .WORD 1000,100,10,1
AR_TANI_M:
        .TEXT8 "KNR"
        .TEXT8 "TSY"
        .TEXT8 "BYR"
        .TEXT8 "CRC"
        .TEXT8 "OK"
