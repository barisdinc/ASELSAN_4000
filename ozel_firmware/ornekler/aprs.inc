; aprs.inc - APRS menü öğesi: PTT'ye basınca 144,800 MHz'de sabit konum paketi (AX.25 / 1200 baud AFSK)
;
;   ... m (APRS) s     -> "APRS PTT"    telsiz alma ve gönderme 144,800 MHz'e geçer (TKS kapalı),
;                                       hoparlör kapanır (kanaldaki paketler duyulmaz)
;   PTT                -> "APRS TX"     paket gider (~0,75 s), ardından "APRS OK"; PTT basılı
;                                       tutulursa telsiz 144,800'de göndermeye devam eder
;   m                  -> çık: telsiz önceki kanalına / frekansına döner
; Paket (aprs_paket.inc, disasm/tools/aprs_paket.py ile derlemede üretilir):
;   TA7W-7>APIZCI,WIDE1-1,WIDE2-1:!4050.63N/03109.39Eb ASELSAN 4000   (Düzce, simge bisiklet)
;
; Ton üretimi seçmeli çağrının donanım yolunu kullanır (F_00E65B ile aynı ayarlar):
;   TB1 zamanlayıcı modunda örnekleme saati: 4,032 MHz / 420 = 9600 Hz, kesme önceliği 6
;   TA1 tek atım modunda (TA1MOD = 6): her TB1 kesmesinde tetiklenir, darbe genişliği = örnek
;   çıkış P52 SÇ_TON -> TD14 süzgeci -> FX806 modülatör girişi
;   FX806 ayarı F_00E65B'nin başındaki gibi ($029A / $029B, F_00CB52): mikrofon kapalı, ton açık
; Örnek: 16 bit faz sayacı, artım 1200 Hz = 8192, 2200 Hz = 15019 (9600 Hz örneklemede);
; sinüs tablosu ROM $28FA (seçmeli çağrınınki, 256 kelime, 114..314). Bit başına 8 örnek.
;
; Bağlantılar:
;   0x002028 VEC_TB1   JMPL $01C627 -> AFSK_TB1 (paket gitmiyorsa orijinal kesmeye geçer)
;   0x00EDA2 durum 4'ün kararı (kimlik gönderilecekse 5, yoksa doğrudan 8 = konuşma): APRS
;            menüsünde frekans kuruluysa her zaman 5, gecikme ~130 ms ($022A = AP_GECIKME)
;   0x00F4E4 durum 5 (PTT'den sonra verici açıldı) JSRL F_00E65B (seçmeli çağrı kimliği)
;            -> APRS_PTT: APRS menüsündeyse paketi gönderir; değilse hiçbir şey (seçmeli
;            çağrı kimliği gönderimi kaldırıldı)
;   0x00E9FE F_00E9D2'nin hoparlörü açtığı yer (P46 SUSTURMA_BILGISI = 1): APRS menüsündeyken
;            hoparlör kapalı kalır (kanaldaki paketler kullanıcıyı rahatsız etmesin); uyarı ve tuş
;            sesleri (P50) etkilenmez. APRS'e girerken hoparlör hemen kapatılır, çıkışta
;            RX'e yeniden girilince firmware susturmaya göre yeniden açar
;   VFO_KANCA (frekans.inc) her turda APRS_KANCA'yı çağırır: menü zaman aşımını yeniler,
;            menüden herhangi bir yolla çıkılınca sentezör kaydını geri yükler

AP_FAZ      .EQU OZEL_RAM+$A0   ; 2 bayt: DDS faz sayacı
AP_ARTIM    .EQU OZEL_RAM+$A2   ; 2 bayt: faz artımı (ton)
AP_BIT      .EQU OZEL_RAM+$A4   ; 2 bayt: sıradaki ton biti
AP_ORNEK    .EQU OZEL_RAM+$A6   ; bitin kalan örnek sayısı
AP_BAYT     .EQU OZEL_RAM+$A7   ; ton baytı (sağa kaydırılarak okunur)
AP_GONDER   .EQU OZEL_RAM+$A8   ; 2 bayt: bit 0 = kesme ton üretiyor
AP_DURUM    .EQU OZEL_RAM+$AA   ; bit 0 = APRS frekansı kurulu (çıkışta geri yükle),
                                ; bit 1 = paket gitti, bit 2 = hata (bant dışı)
AP_YEDEK    .EQU OZEL_RAM+$AB   ; 13 bayt: önceki sentezör kaydı ($0370..$037C)
AP_KAYIT    .EQU OZEL_RAM+$B8   ; 13 bayt: APRS'in kurduğu kayıt (çıkışta karşılaştırmak için)
AP_029A     .EQU OZEL_RAM+$C5   ; 2 bayt: FX806 ayar kopyasının yedeği

F_AP_KURULU .EQU 1
F_AP_GITTI  .EQU 2
F_AP_HATA   .EQU 4

ARTIM_MARK  .EQU 8192           ; 1200 Hz
ARTIM_SPACE .EQU 15019          ; 2200 Hz
AP_GECIKME  .EQU 4              ; PTT'den paketin başına: 4 x 32 ms (verici ve sentezör otursun)
TB1_9600    .EQU 419            ; 4032000 / (419 + 1) = 9600 Hz
SINUS       .EQU $0028FA

R_P4        .EQU $000A          ; P46 SUSTURMA_BILGISI: 1 = hoparlör açık
R_TCSTART   .EQU $0040          ; sayıcı başlatma (bit 1 TA1, bit 6 TB1)
R_TOSTART   .EQU $0042          ; tek atım tetikleme (bit 1 TA1)
R_TA1       .EQU $0048
R_TB1       .EQU $0052
R_TA1MOD    .EQU $0057
R_TB1MOD    .EQU $005C
R_WDT       .EQU $0060
R_TA1IC     .EQU $0076
R_TB1IC     .EQU $007B
TB1_ORIJINAL .EQU $01C627
F_FX806     .EQU $00CB52        ; $029A / $029B'deki ayarı FX806'ya yazar

; ---------------------------------------------------------------------------------------------
AP_KOD_ADRES .EQU *
        .ORG $002028                    ; VEC_TB1
        .EXPECT $5C,$27,$C6,$01
        JMPL  AFSK_TB1
        .ORG $00EDA2                    ; durum 4: PTT'de kimlik gönderilecek mi? (5 ya da 8)
        .EXPECT $2C,$44,$02,$02,$00,$1C ; BBS #$0002,$0244,L_00EDC4
        .ASSUME M=0,X=0
        JMPL  APRS_KARAR
        .BYTE $EA,$EA                   ; (NOP, çalışmaz)
        .ORG $00E9FE                    ; F_00E9D2: susturma açıldı -> hoparlörü aç (P46 = 1)
        .EXPECT $F8,$0C,$0A,$00,$40,$4C,$0B,$EA   ; SEM / SEB #$40,P4 / JMP L_00EA0B
        JMPL  APRS_HOPARLOR
        .BYTE $EA,$EA,$EA,$EA           ; (NOP, çalışmaz)
        .ORG $00F4E4                    ; durum 5 -> 6: seçmeli çağrı kimliği yerine APRS
        .EXPECT $22,$5B,$E6,$00
        JSRL  APRS_PTT
        .ORG AP_KOD_ADRES

; ---------------------------------------------------------------------------------------------
; TB1 kesmesi. Giriş durumu bilinmiyor: orijinal kesme gibi CLP #$39 / PSH #$35 / LDT #0.
AFSK_TB1:
        CLP   #$39
        .ASSUME M=0,X=0
        PSH   #$35
        LDT   #$00
        BBS   #$0001,AP_GONDER,AT_URET
        PUL   #$35
        JMPL  TB1_ORIJINAL              ; seçmeli çağrı / DTMF / alma: orijinal kesme
AT_URET:
        SEM
        SEB   #$02,R_TOSTART            ; TA1 darbesi (genişliği bir önceki örnek)
        CLM
        LDA   A,AP_FAZ
        CLC
        ADC   A,AP_ARTIM
        STA   A,AP_FAZ
        LDA   A,AP_FAZ+1                ; üst bayt -> tablo indeksi (kelime)
        AND   A,#$00FF
        ASL   A
        TAX
        LDA   A,>SINUS,X
        STA   A,R_TA1
        SEM
        DEC   AP_ORNEK
        BNE   AT_SON
        LDM   #8,AP_ORNEK               ; sıradaki ton biti
        CLM
        LDA   A,AP_BIT
        CMP   A,#APRS_NBIT
        BCS   AT_BITTI
        INC   AP_BIT
        AND   A,#7
        BNE   AT_KAYDIR
        LDA   A,AP_BIT                  ; 8 bitte bir yeni bayt: APRS_TONLAR[bit / 8]
        DEC   A
        LSR   A
        LSR   A
        LSR   A
        TAX
        SEM
        LDA   A,>APRS_TONLAR,X
        STA   A,AP_BAYT
AT_KAYDIR:
        SEM
        LSR   AP_BAYT                   ; C = ton (1 mark, 0 space)
        CLM
        LDA   A,#ARTIM_MARK
        BCS   AT_TON
        LDA   A,#ARTIM_SPACE
AT_TON: STA   A,AP_ARTIM
        BRA   AT_SON
AT_BITTI:
        LDM   #0,AP_GONDER              ; paket bitti (ana döngü bekliyor)
AT_SON: CLM
        PUL   #$35
        RTI

; ---------------------------------------------------------------------------------------------
; Durum 4'ün kararı (F_00ED53, M = 0, D = onun çerçevesi): APRS menüsündeyse kimlik yolu (5)
APRS_KARAR:
        .ASSUME M=0,X=0
        SEM
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AKR_ORJ
        BBC   #F_AP_KURULU,AP_DURUM,AKR_ORJ
        LDM   #5,RADYO_DURUM            ; durum 5; gecikme EEPROM'daki ton gönderme gecikmesi
        LDM   #AP_GECIKME,$022A         ; değil (0x342, 1,5 s'ye kadar): paketin kendi 300 ms
        JMPL  $00EDF0                   ; bayrağı var. Sonra L_00EDC4'ün devamı
AKR_ORJ:
        CLM                             ; orijinal: BBS #$0002,$0244,L_00EDC4
        BBS   #$0002,$0244,AKR_5
        JMPL  $00EDA8
AKR_5:  JMPL  $00EDC4

; ---------------------------------------------------------------------------------------------
; F_00E9D2'nin hoparlörü açtığı yer (M = 1): APRS menüsündeyse kapalı tut
APRS_HOPARLOR:
        .ASSUME M=1,X=0
        SEM
        JSR   AP_ETKIN
        BNE   AH_AC
        CLB   #$40,R_P4                 ; APRS: hoparlör kapalı
        JMPL  $00EA0E
AH_AC:  SEB   #$40,R_P4                 ; orijinal
        JMPL  $00EA0E

; APRS menüsü açık ve frekans kurulu mu? Z = 1: evet (M = 1 giriş ve çıkış, A bozulur)
AP_ETKIN:
        .ASSUME M=1,X=0
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AE_SON
        LDA   A,AP_DURUM
        AND   A,#F_AP_KURULU
        EOR   A,#F_AP_KURULU            ; kuruluysa 0
AE_SON: RTS

; ---------------------------------------------------------------------------------------------
; Durum 5'te (verici açıldı, PTT basılı) telsiz durum makinesinden çağrılır (M = 0, 4,S çerçeve).
APRS_PTT:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        SEM
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AP_YOK
        BBC   #F_AP_KURULU,AP_DURUM,AP_YOK
        CLM
        LDX   #0                        ; ekranda "APRS TX"
        JSR   AP_METIN_YAZ
        JSRL  F_LCD
        CLM
        JSR   AFSK_GONDER
        SEM
        SEB   #F_AP_GITTI,AP_DURUM
        CLM
        LDX   #8                        ; "APRS OK"
        JSR   AP_METIN_YAZ
        JSRL  F_LCD
AP_YOK: CLM
        PLA
        RTL

; Paketi gönder ve bitmesini bekle (M = 0). Çağıranın çerçevesi 3,S'de: F_FX806 için yığının
; tepesine yeniden konur; F_FX806'ya kadar yığına başka bir şey konmamalı.
AFSK_GONDER:
        .ASSUME M=0,X=0
        LDA   A,$03,S                   ; APRS_PTT'nin koyduğu çerçeve (JSR 2 bayt)
        PHA
        CLB   #$0800,$023E              ; FX806: F_00E65B'deki gibi ton yolu açık, mikrofon kapalı
        LDX   $029A                     ; (yığına değil: F_FX806 yığının tepesini çerçeve sanar)
        STX   AP_029A
        SEB   #$8000,$029A
        CLB   #$4000,$029A
        SEM
        LDA   A,$029B
        AND   A,#$F8
        ORA   A,#$06
        STA   A,$029B
        CLM
        JSRL  F_FX806
        CLM
        LDX   AP_029A
        STX   $029A
        SEM
        LDM   #$00,R_TA1IC              ; TA1 tek atım, kesmesiz
        LDM   #$06,R_TA1MOD
        LDM   #$00,R_TB1MOD             ; TB1 zamanlayıcı, f2
        CLM
        LDA   A,#TB1_9600
        STA   A,R_TB1
        LDA   A,>SINUS
        STA   A,R_TA1
        LDM   #0,AP_FAZ
        LDM   #ARTIM_MARK,AP_ARTIM
        LDM   #0,AP_BIT
        SEM
        LDM   #1,AP_ORNEK               ; ilk kesmede 0. bite geçilir
        CLM
        LDM   #1,AP_GONDER
        SEM
        LDA   A,#$01                    ; IPL = 1 (F_00E65B gibi)
        PHP
        STA   A,$02,S
        PLP
        SEM
        LDM   #$06,R_TB1IC              ; TB1 kesmesi, öncelik 6
        SEB   #$02,R_TCSTART            ; TA1 ve TB1 başlasın
        SEB   #$40,R_TCSTART
        SEB   #$02,R_TOSTART
AG_BEKLE:
        LDM   #$00,R_WDT                ; bekçi köpeği
        CLM
        BBS   #$0001,AP_GONDER,AG_BEKLE2
        BRA   AG_BITTI
AG_BEKLE2:
        SEM
        BRA   AG_BEKLE
AG_BITTI:
        SEM
        CLB   #$02,R_TCSTART
        CLB   #$40,R_TCSTART
        LDM   #$00,R_TB1IC
        LDA   A,#$00                    ; IPL = 0
        PHP
        STA   A,$02,S
        PLP
        CLM
        PLA
        RTS

; LCD = AP_METIN[X..X+7] (M = 0 giriş ve çıkış)
AP_METIN_YAZ:
        .ASSUME M=0,X=0
        LDY   #0
        SEM
AMY_1:  LDA   A,>AP_METIN,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   AMY_1
        CLM
        RTS

AP_METIN:
        .TEXT8 "APRS TX"
        .TEXT8 "APRS OK"
        .TEXT8 "APRS PTT"
        .TEXT8 "ERROR"
        .TEXT8 "HATA"

; ---------------------------------------------------------------------------------------------
; Menü: girişte 144,800'ü kur
APRS_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        LDM   #APRS_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(APRS_TUS),HANDLER_BK
        CLM
        JSR   AP_KUR
        JMP   AP_EKRAN_SON

APRS_TUS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
AP_EKRAN_SON:
        CLM
        LDX   #16                       ; "APRS PTT"
        SEM
        BBC   #F_AP_GITTI,AP_DURUM,AE_1
        LDX   #8                        ; "APRS OK"
AE_1:   BBC   #F_AP_HATA,AP_DURUM,AE_2
        LDX   #24                       ; "ERROR"
        BBC   #$80,OPTIONS,AE_2
        LDX   #32                       ; "HATA"
AE_2:   CLM
        JSR   AP_METIN_YAZ
        PLA
        RTL

; Sentezör kaydını sakla, 144,800 MHz'i alma ve gönderme olarak kur, TKS'yi kapat (M = 0)
AP_KUR:
        .ASSUME M=0,X=0
        SEM
        LDM   #0,AP_DURUM
        LDX   #0
AK_1:   LDA   A,SYN_KAYIT,X
        STA   A,AP_YEDEK,X
        INX
        CPX   #13
        BNE   AK_1
        LDX   #0                        ; haneler "1448000"
AK_2:   LDA   A,>AP_HANELER,X
        STA   A,FR_HANE,X
        INX
        CPX   #7
        BNE   AK_2
        LDM   #7,FR_NDIG
        CLM
        JSR   FR_HESAPLA
        BCC   AK_GECERLI
        SEM
        LDM   #F_AP_HATA,AP_DURUM
        CLM
        RTS
AK_GECERLI:
        LDA   A,RX_KAYMA
        JSR   FR_SENTEZ
        LDX   #0
        SEM
AK_3:   LDA   A,FR_KAYIT,X
        STA   A,SYN_KAYIT,X
        INX
        CPX   #5
        BNE   AK_3
        CLM
        LDA   A,#0
        JSR   FR_SENTEZ
        LDX   #0
        SEM
AK_4:   LDA   A,FR_KAYIT,X
        STA   A,SYN_KAYIT+7,X
        INX
        CPX   #5
        BNE   AK_4
        LDA   A,SYN_KAYIT+5             ; TKS kapalı (alt 6 bit); seçmeli çağrı biti (7) de
        AND   A,#$40
        STA   A,SYN_KAYIT+5
        LDA   A,SYN_KAYIT+12
        AND   A,#$C0
        STA   A,SYN_KAYIT+12
        LDX   #0
AK_5:   LDA   A,SYN_KAYIT,X             ; kopya ($0363) ve karşılaştırma için
        STA   A,SYN_YEDEK,X
        STA   A,AP_KAYIT,X
        INX
        CPX   #13
        BNE   AK_5
        LDM   #F_AP_KURULU,AP_DURUM
        CLB   #$40,R_P4                 ; hoparlör kapalı (APRS_HOPARLOR açık tutmaz)
        LDM   #1,RADYO_DURUM            ; RX'e yeniden gir, sentezörü yükle
        CLM
        RTS

AP_HANELER:
        .BYTE "1448000"

; ---------------------------------------------------------------------------------------------
; VFO_KANCA'dan her turda (M = 0, X = 0; A, X, Y korunmaz). Menü açıkken zaman aşımını yeniler;
; APRS menüsünden çıkılmışsa (m, zaman aşımı) ve kayıt hâlâ APRS'inkiyse eski kaydı geri yükler.
APRS_KANCA:
        .ASSUME M=0,X=0
        SEM
        BBC   #F_AP_KURULU,AP_DURUM,AKN_SON
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AKN_CIKIS
        LDA   A,MENU_OLAY               ; açıkken menü zaman aşımı yok
        CMP   A,#$14
        BNE   AKN_SON
        LDM   #$3F,MENU_SAYAC
        BRA   AKN_SON
AKN_CIKIS:
        LDM   #0,AP_DURUM
        LDX   #0                        ; kayıt hâlâ bizimki mi? (kanal anahtarı çevrildiyse dokunma)
AKN_1:  LDA   A,SYN_KAYIT,X
        CMP   A,AP_KAYIT,X
        BNE   AKN_SON
        INX
        CPX   #13
        BNE   AKN_1
        LDX   #0
AKN_2:  LDA   A,AP_YEDEK,X
        STA   A,SYN_KAYIT,X
        STA   A,SYN_YEDEK,X
        INX
        CPX   #13
        BNE   AKN_2
        LDM   #1,RADYO_DURUM
AKN_SON:
        CLM
        RTS

        .INCLUDE "aprs_paket.inc"
