; gps.inc - seri porttan gelen GPS (NMEA) konumu APRS'te (AT V0-26)
;
; UART0'a (programlama kablosu, 9600 8N1) bağlı bir GPS ya da yükleme sayfasındaki GPS benzetimi
; "$GPRMC" (ya da $GNRMC ...) cümleleri gönderirse: dakikada bir sonraki geçerli (durum 'A') RMC
; cümlesinin enlem / boylamı alınır; APRS paketi (tracker, PTT) EEPROM'daki konum yerine bunu
; kullanır. GPS konumu yokken her saniye denenir; bir dakika içinde geçerli cümle gelmezse GPS
; konumu bırakılır (EEPROM konumuna dönülür). APRS ekranında GPS konumu varken "APRS" yerine "GPS".
; Seri hat RS232 seviyesindedir: TTL GPS modülü MAX3232 gibi bir TTL->RS232 dönüştürücüyle bağlanır.
;
; Alma kesmesi (L_019469) baytı U0RB'den okuduğu yerde (0x01948B) GPS_RX'e uğrar: bayt firmware'e
; aynen gider (servis protokolü etkilenmez), ayrıca GPS_BAYT ayrıştırır. Ayrıştırıcı yalnız
; istenince (GPS_YAKALA) tampona yazar; alanlar APRS biçiminde: "DDMM.mmN" + "DDDMM.mmE"
; (NMEA'daki fazla ondalıklar atılır).
; RAM: APRS tamponunun (AR_CER, 140 bayta indirildi) arkası $07CC..$07DF; saniye sayacı $04EF.

GPS_TAMP    .EQU AR_CER+AR_CER_N    ; 17 bayt: enlem 8, boylam 9 (APRS biçiminde)
GPS_ALAN    .EQU GPS_TAMP+17        ; RMC alan sırası
GPS_I       .EQU GPS_TAMP+18        ; alandaki karakter
GPS_DURUM   .EQU GPS_TAMP+19        ; bitler aşağıda
GPS_SN      .EQU $04EF              ; son istekten beri saniye
G_YAKALA    .EQU $01                ; yeni konum istendi
G_GECERLI   .EQU $02                ; GPS_TAMP geçerli konum
G_CUMLE     .EQU $04                ; RMC cümlesi içindeyiz (istek varken)
G_DURUM_A   .EQU $08                ; bu cümlede durum 'A' (konum geçerli)
U0RB_ADR    .EQU $0036
GPS_RX_DON  .EQU $019490

        .ORG $01948B                    ; L_01948A: LDA A,U0RB / STA A,$00
        .EXPECT $AD,$36,$00,$85,$00
        .ASSUME M=1,X=0
        JMPL  GPS_RX
        NOP
        .ORG GPS_KOD

GPS_RX:
        .ASSUME M=1,X=0
        LDA   A,U0RB_ADR
        STA   A,$00                     ; firmware'in yaptığı (D = kesmenin çerçevesi)
        PHX
        PHY
        JSR   GPS_BAYT
        PLY
        PLX
        JMPL  GPS_RX_DON

; A = gelen bayt (M = 1). $G?RMC,saat,durum,enlem,K/G,boylam,D/B,...
GPS_BAYT:
        .ASSUME M=1,X=0
        CMP   A,#'$'
        BNE   GB_1
        LDM   #0,GPS_ALAN
        LDM   #0,GPS_I
        LDA   A,GPS_DURUM
        AND   A,#G_YAKALA|G_GECERLI     ; yeni cümle: durum 'A' henüz yok
        STA   A,GPS_DURUM
        BBC   #G_YAKALA,GPS_DURUM,GB_D1
        SEB   #G_CUMLE,GPS_DURUM        ; istek varken cümleyi izle
GB_D1:  RTS
GB_1:   BBS   #G_CUMLE,GPS_DURUM,GB_2
        RTS
GB_2:   CMP   A,#','
        BNE   UZGPS_61
        JMP   GB_VIRGUL
UZGPS_61:
        CMP   A,#'*'
        BNE   UZGPS_63
        JMP   GB_BITIR
UZGPS_63:
        CMP   A,#' '
        BCS   UZGPS_67
        JMP   GB_BITIR
UZGPS_67:
        TAY                             ; Y = karakter
        LDA   A,GPS_ALAN
        BEQ   GB_BASLIK
        CMP   A,#2
        BEQ   GB_DURUMA
        CMP   A,#3
        BEQ   GB_ENLEM
        CMP   A,#4
        BEQ   GB_KG
        CMP   A,#5
        BEQ   GB_BOYLAM
        CMP   A,#6
        BEQ   GB_DB
        RTS
GB_BASLIK:                              ; "GPRMC": 3..5. karakter "RMC"
        LDA   A,GPS_I
        INC   GPS_I
        CMP   A,#2
        BCC   GB_SON
        CMP   A,#5
        BCS   GB_SON
        CLM
        AND   A,#$00FF
        TAX
        SEM
        TYA
        CMP   A,>GB_RMC-2,X
        BEQ   GB_SON
        BRA   GB_BITIR
GB_DURUMA:
        TYA
        CMP   A,#'A'
        BNE   GB_SON
        SEB   #G_DURUM_A,GPS_DURUM
        RTS
GB_ENLEM:                               ; "DDMM.mm" (fazlası atılır)
        LDA   A,GPS_I
        BNE   GB_E1
        CLB   #G_GECERLI,GPS_DURUM      ; tampon yazılıyor
GB_E1:  CMP   A,#7
        BCS   GB_SON
        CLM
        AND   A,#$00FF
        TAX
        SEM
        TYA
        STA   A,GPS_TAMP,X
        INC   GPS_I
        RTS
GB_KG:  TYA
        STA   A,GPS_TAMP+7
        RTS
GB_BOYLAM:                              ; "DDDMM.mm"
        LDA   A,GPS_I
        CMP   A,#8
        BCS   GB_SON
        CLM
        AND   A,#$00FF
        TAX
        SEM
        TYA
        STA   A,GPS_TAMP+8,X
        INC   GPS_I
        RTS
GB_DB:  TYA
        STA   A,GPS_TAMP+16
GB_SON: RTS
GB_VIRGUL:
        INC   GPS_ALAN
        LDM   #0,GPS_I
        LDA   A,GPS_ALAN
        CMP   A,#7                      ; D/B alanı bitti: konum tamam
        BNE   GB_SON
        BBC   #G_DURUM_A,GPS_DURUM,GB_BITIR
        LDA   A,GPS_DURUM
        ORA   A,#G_GECERLI
        AND   A,#$FA                    ; G_YAKALA ve G_CUMLE silinir
        STA   A,GPS_DURUM
        RTS
GB_BITIR:
        CLB   #G_CUMLE,GPS_DURUM
        RTS
GB_RMC: .BYTE 'R','M','C'

; Her saniye (aprs_digi.inc DG_ZAMAN'dan, M = 1): dakikada bir (konum yokken her saniye) istek.
; Önceki istek bir dakikada karşılanmadıysa GPS konumu bırakılır.
GPS_SANIYE:
        .ASSUME M=1,X=0
        INC   GPS_SN
        BBC   #G_GECERLI,GPS_DURUM,GS_ISTE
        LDA   A,GPS_SN
        CMP   A,#60
        BCC   GS_SON
GS_ISTE:
        LDA   A,GPS_SN
        CMP   A,#60
        BCC   GS_1
        CLB   #G_GECERLI,GPS_DURUM      ; bir dakikadır istek karşılanmadı / konum eski
GS_1:   BBC   #G_YAKALA,GPS_DURUM,GS_YENI
        BBS   #G_GECERLI,GPS_DURUM,GS_SON
        RTS                             ; konum yokken bekleyen istek sürer
GS_YENI:
        SEB   #G_YAKALA,GPS_DURUM
        LDM   #0,GPS_SN
GS_SON: RTS

; AP_ALAN_AD yerine (enlem / boylam; M = 0, A = alan, Y = uzunluk): GPS konumu geçerliyse AP_AD onunla
GPS_ALAN_AD:
        .ASSUME M=0,X=0
        PHA
        JSR   AP_ALAN_AD
        PLA
        PHX
        SEM
        BBC   #G_GECERLI,GPS_DURUM,GAA_SON
        LDX   #0
        LDY   #8
        CMP   A,#AB_ENLEM
        BEQ   GAA_1
        LDX   #8
        LDY   #9
GAA_1:  STY   AP_ALAN_N                 ; (geçici sayaç)
        LDY   #0
GAA_2:  LDA   A,GPS_TAMP,X
        STA   A,AP_AD,Y
        INX
        INY
        DEC   AP_ALAN_N
        BNE   GAA_2
GAA_SON:
        CLM
        PLX
        RTS

; AP_METIN_YAZ'ın sonu: GPS konumu varken "APRS" yerine "GPS " (M = 0 giriş ve çıkış)
GPS_EKRAN:
        .ASSUME M=0,X=0
        SEM
        BBC   #G_GECERLI,GPS_DURUM,GE_SON
        LDA   A,LCD
        CMP   A,#'A'
        BNE   GE_SON
        LDM   #'G',LCD
        LDM   #'P',LCD+1
        LDM   #'S',LCD+2
        LDM   #' ',LCD+3
GE_SON: CLM
        RTS
