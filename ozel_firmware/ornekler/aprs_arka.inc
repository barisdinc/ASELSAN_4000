; aprs_arka.inc - arka planda APRS tracker (AT V0-27)
;
; Yalnız APRS TR açıkken (DG ve IG kapalı) ve aralık ayarlıyken telsiz normal kanalında (ya da VFO
; frekansında) çalışır, orayı dinler ve oradan konuşur. Beacon süresi dolunca kanal gözlenir:
; 30 s kesintisiz sinyal yoksa (susturma kapalı, normal ekran, alıyor) 144,800'e geçilir, ekranda
; "APRS TX", tracker paketi gönderilir, kanala dönülür, ekran geri gelir. Sinyal gelirse, menüye
; girilirse ya da konuşulursa 30 s baştan sayılır. APRS ekranındayken bu yol çalışmaz (orada
; tracker AP_OTO_ZAMAN ile kanalın ilk boşluğunda gönderir).
; Durumlar (ARKA_DURUM): 0 süre sayılıyor, 1 sessizlik bekleniyor, 2 144,800'e geçiliyor,
; 3 gönderiliyor (APRS_KARAR / APRS_PTT bunu APRS ekranı gibi kabul eder), 4 kanala dönüş.
; 144,800'e geçiş ve dönüş APRS menüsününkiyle aynı (AP_KUR: kayıt AP_YEDEK'e; dönüşte kayıt
; hâlâ bizimkiyse geri). GPS (gps.inc) burada da dakikada bir güncellenir.
; RAM: ARKA_DURUM $0131; sayaçlar APRS ekranınınkiler (AP_SANIYE, AP_MS, AP_FARK, AP_OTO_SURE).

ARKA_DURUM  .EQU $0131
ARKA_SESSIZ .EQU 30                 ; s

; Açılışta (AP_ACILIS_DENETLE, M = 0, D = boş çerçeve): aralığı EEPROM'dan yükle
ARKA_YUKLE:
        .ASSUME M=0,X=0
        TDA
        STA   A,AP_F
        JSR   AP_BLOK_DENETLE
        LDA   A,#AB_ARALIK
        LDY   #2
        JSR   AP_ALAN_AD
        JSR   AP_ARALIK_AL
        SEM
        LDM   #0,ARKA_DURUM
        CLM
        RTS

; APRS_KANCA'dan her turda, APRS ekranında değilken ya da arka plan sürerken (M = 0, A = çerçeve)
ARKA_ISLE:
        .ASSUME M=0,X=0
        PHA
        SEM
        LDA   A,TIK_32MS                ; geçen 32 ms adımı
        SEC
        SBC   A,AP_SONTIK
        STA   A,AP_FARK
        LDA   A,TIK_32MS
        STA   A,AP_SONTIK
        LDA   A,ARKA_DURUM
        BNE   AI_DURUM
        JSR   AI_KOSUL                  ; 0: süre
        BCS   AI_SIFIR
        JSR   AI_SAY
        CLM
        LDA   A,AP_SANIYE
        CMP   A,AP_ARALIK
        SEM
        BCC   AI_SONJ
        LDM   #1,ARKA_DURUM
AI_SIFIR:
        CLM
        LDM   #0,AP_SANIYE
        LDM   #0,AP_MS
        SEM
AI_SONJ:
        JMP   AI_SON
AI_DURUM:
        CMP   A,#1
        BEQ   AI_SESSIZ
        CMP   A,#2
        BEQ   AI_GECIS
        CMP   A,#3
        BNE   AI_DONUSJ
        JMP   AI_GONDER
AI_DONUSJ:
        JMP   AI_DONUS
AI_SESSIZ:                              ; 1: 30 s kesintisiz boşluk
        JSR   AI_KOSUL
        BCC   AI_S1
        LDM   #0,ARKA_DURUM
        BRA   AI_SIFIR
AI_S1:  BBS   #$20,$023E,AI_SIFIR       ; kanalda sinyal (susturma açık)
        LDA   A,MENU
        BNE   AI_SIFIR
        LDA   A,RADYO_DURUM
        CMP   A,#2
        BNE   AI_SIFIR
        JSR   AI_SAY
        CLM
        LDA   A,AP_SANIYE
        CMP   A,#ARKA_SESSIZ
        SEM
        BCC   AI_SONJ
        CLM                             ; 144,800'e geç (APRS menüsü gibi)
        JSR   AP_KUR
        SEM
        BBS   #F_AP_KURULU,AP_DURUM,AI_S2
        LDM   #0,ARKA_DURUM             ; frekans bantta değil
        LDM   #0,AP_DURUM
        BRA   AI_SIFIR
AI_S2:  LDM   #2,ARKA_DURUM
        CLM
        LDX   #0                        ; "APRS TX"
        JSR   AP_METIN_YAZ
        JSR   AI_LCD
        SEM
        JMP   AI_SON
AI_GECIS:                               ; 2: 144,800'de alıma girince PTT iste
        LDA   A,MENU
        BNE   AI_DONUS
        LDA   A,RADYO_DURUM
        CMP   A,#2
        BEQ   UZAPR_108
        JMP   AI_SON
UZAPR_108:
        LDM   #0,AP_OTO
        LDM   #94,AP_OTO_SURE           ; ~3 s
        LDM   #3,ARKA_DURUM
        CLM
        SEB   #F_PTT_APRS,PTT_ISTEK
        SEM
        JMP   AI_SON
AI_GONDER:                              ; 3: APRS_PTT gönderir ve isteği kaldırır
        LDA   A,RADYO_DURUM
        CMP   A,#4
        BCS   AI_SON                    ; gönderiyor
        CLM
        BBS   #F_PTT_APRS,PTT_ISTEK,AI_G1
        SEM
        CMP   A,#2                      ; istek kalktı, alıma döndü: kanala
        BEQ   AI_DONUS
        BRA   AI_SON
AI_G1:  SEM
        LDA   A,AP_OTO_SURE             ; verici açılmadı mı
        SEC
        SBC   A,AP_FARK
        BCS   AI_G2
        LDA   A,#0
AI_G2:  STA   A,AP_OTO_SURE
        BNE   AI_SON
AI_DONUS:                               ; 4: kanala dön
        CLM
        CLB   #F_PTT_APRS,PTT_ISTEK
        SEM
        LDM   #0,AP_DURUM
        LDM   #0,ARKA_DURUM
        LDX   #0                        ; kayıt hâlâ bizimki mi (kanal anahtarı çevrildiyse dokunma)
AI_D1:  LDA   A,SYN_KAYIT,X
        CMP   A,AP_KAYIT,X
        BNE   AI_D3
        INX
        CPX   #13
        BNE   AI_D1
        LDX   #0
AI_D2:  LDA   A,AP_YEDEK,X
        STA   A,SYN_KAYIT,X
        STA   A,SYN_YEDEK,X
        INX
        CPX   #13
        BNE   AI_D2
        LDM   #1,RADYO_DURUM
AI_D3:  CLM
        LDM   #0,AP_SANIYE
        LDM   #0,AP_MS
        SEM
        LDA   A,MENU                    ; normal ekran: kanal adı (VFO'da frekans) geri
        BNE   AI_SON
        LDX   #0
AI_D4:  LDA   A,KANAL_ADI,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   AI_D4
        CLM
        JSR   AI_LCD
        SEM
AI_SON: CLM
        PLA
        RTS

; Yalnız TR açık (DG, IG kapalı) ve aralık var mı? C = 0 evet (M = 1)
AI_KOSUL:
        .ASSUME M=1,X=0
        LDA   A,AP_AYAR
        AND   A,#AYAR_TR|AYAR_DG|AYAR_IG
        CMP   A,#AYAR_TR
        BNE   AK_HAYIR
        CLM
        LDA   A,AP_ARALIK
        SEM
        BEQ   AK_HAYIR
        CLC
        RTS
AK_HAYIR:
        SEC
        RTS

; AP_FARK x 32 ms -> AP_MS / AP_SANIYE; her saniye GPS (M = 1)
AI_SAY:
        .ASSUME M=1,X=0
        LDA   A,AP_FARK
        BEQ   AIS_SON
        STA   A,AP_T2
        CLM
AIS_1:   LDA   A,AP_MS
        CLC
        ADC   A,#32
        CMP   A,#1000
        BCC   AIS_2
        SBC   A,#1000
        INC   AP_SANIYE
        SEM
        JSR   GPS_SANIYE
        CLM
AIS_2:   STA   A,AP_MS
        SEM
        DEC   AP_T2
        CLM
        BNE   AIS_1
        SEM
AIS_SON: RTS

; LCD tamponunu ekrana yaz (M = 0; çerçeve ARKA_ISLE'nin yığınında: 2,S dönüş, 3,S çerçeve)
AI_LCD:
        .ASSUME M=0,X=0
        LDA   A,$03,S
        PHA
        JSRL  F_LCD
        CLM
        PLA
        RTS
