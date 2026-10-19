; aprs_rx.inc - APRS alma: duyulan istasyonların çağrı işaretleri (aprs.inc ile, frekans.asm)
;
; APRS menüsündeyken (144,800 MHz alma) gelen AX.25 paketleri çözülür; sağlaması (FCS) tutan
; her paketin gönderen çağrı işareti ekranda ~4 s görünür (ör. "TA7W-7"), sonra APRS ekranına
; dönülür. Son duyulan 8 farklı istasyon bir listede tutulur (en yeni başta; aynı istasyon
; yeniden duyulursa başa alınır). APRS ekranında:
;   *   listede bir eskiye     #   bir yeniye (en yeniden sonra APRS ekranı)
; Liste telsiz kapanana kadar durur. Liste boşsa "EMPTY" / "BOS".
;
; Donanım: alma sesi FX806 süzgecinden (TD3-17) ve TD12 sıfır-geçiş algılayıcısından geçip P66'ya
; (TB1IN) kare dalga olarak gelir; seçmeli çağrı çözücüsünün yolu (calisma_analizi.md bölüm 9).
; TB1 darbe genişliği ölçme modunda (TB1MOD $4A, f16 = 504 kHz): her kenarda bir önceki kenardan
; beri geçen süre (yarım periyot) ölçülür ve kesme gelir. 1200 Hz'in yarım periyodu 210,
; 2200 Hz'inki 115 sayım; bir bit 420 sayım.
;
; Çözücü (kesmede, AR_TB1):
;   - 30 sayımdan kısa darbeler bir sonrakine eklenir (gürültü kıpırtısı); 600'den uzun = sinyal yok
;   - ton: son iki yarım periyodun toplamı (tam periyot) 325'ten büyükse mark (1200 Hz), değilse
;     space. Tam periyot karşılaştırıcının görev oranı kaymasından etkilenmez
;   - bit saati: AR_P bit içindeki konum (0..419). Ton değişince değişimin bit sınırında (210)
;     olması beklenir; hatanın 1/4'ü kadar düzeltilir (PLL). AR_P 420'yi geçtikçe o anki ton
;     örneklenir: önceki örnekle aynıysa 1, değilse 0 (NRZI)
;   - HDLC: altı 1 + 0 = bayrak, beş 1'den sonraki 0 atılır (bit doldurma), yedi 1 = iptal;
;     baytlar LSB önce. Bayrakta çerçeve bayta hizalıysa, en az 18 baytsa ve CRC kalanı $F0B8
;     ise (FCS dahil CRC-16-CCITT) paket geçerlidir
;   - adres alanının ilk 14 baytı saklanır (hedef + gönderen); CRC bayt bayt hesaplanır, bu yüzden
;     uzun paketler için tampon gerekmez. Geçerli paketin gönderen adresi AR_YENI_ADR'ye konur;
;     ana döngü (APRS_KANCA -> AR_ISLE) listeye ekler ve ekrana yazar
; Algoritma JS'de denendi: temiz sinyalde, tonlar arasında 2 kat genlik farkında, görev oranı
; kaymasında ve ±%0,3 baud farkında kayıpsız; bant sınırlı gürültü tepe genliğin %15'i iken
; paketlerin ~%90'ı çözülür.
;
; Hoparlör: almada ses kapalı sayılır (aprs.inc APRS_SUS_KARAR). Taşıyıcı varken FX806'ya normal
; alma ayarı yazılır, yalnız C-BUS $10 bit 7 (ses çıkışı) kapalı: firmware'in seçmeli çağrı / TKS
; beklerken yaptığı susturma. Çözücünün girişi (FX806 süzgeci -> P66) açık kalır.

; RAM: liste OZEL_RAM+$00..$37 (bu derlemede SAYI / HESAP / OYUN yok, boş), durum $05FC..$0627
AR_LISTE    .EQU OZEL_RAM+$00   ; 8 x 7 bayt: AX.25 adresi (karakterler <<1, SSID baytı)
AR_N_MAX    .EQU 8
AR_DURUM    .EQU $05FC          ; bit 0 alma kurulu (TB1 ölçmede), bit 1 çerçeve içinde
AR_BIRLER   .EQU $05FD          ; art arda 1 sayısı
AR_BITSAY   .EQU $05FE          ; AR_BAYT'taki bit sayısı
AR_BAYT     .EQU $05FF
AR_DPREV    .EQU $0600          ; 2 bayt: önceki yarım periyot (0 = yok)
AR_P        .EQU $0602          ; 2 bayt: bit saati
AR_BEKLE    .EQU $0604          ; 2 bayt: kısa darbelerin toplamı
AR_TON      .EQU $0606          ; 1 mark, 0 space
AR_SONTON   .EQU $0607          ; son örneklenen ton
AR_SAY      .EQU $0608          ; 2 bayt: çerçevedeki bayt sayısı
AR_CRC      .EQU $060A          ; 2 bayt
AR_ADR      .EQU $060C          ; 14 bayt: hedef + gönderen adresi
AR_YENI     .EQU $061A          ; 1 = AR_YENI_ADR'de işlenmemiş bir adres var
AR_YENI_ADR .EQU $061B          ; 7 bayt
AR_GOSTER   .EQU $0622          ; ekrandaki çağrı işaretinin kalan süresi (32 ms adım), 0 = yok
AR_LN       .EQU $0623          ; listedeki istasyon sayısı
AR_SEC      .EQU $0624          ; gösterilen liste öğesi (1..AR_LN), 0 = yeni gelen
AR_D        .EQU $0625          ; 2 bayt: geçici
AR_T        .EQU $0627          ; geçici (bayt)
AR_IT       .EQU $0628          ; 2 bayt: kesmenin geçicisi

R_TB1MOD_OLC .EQU $4A           ; darbe genişliği ölçme, f16 (504 kHz)
AR_BIT      .EQU 420            ; 504000 / 1200
AR_ESIK     .EQU 326            ; tam periyot >= 326: mark
AR_KISA     .EQU 30
AR_UZUN     .EQU 600
AR_SURE     .EQU 125            ; 125 x 32 ms = 4 s

; ---------------------------------------------------------------------------------------------
; TB1 kesmesi, almada (AFSK_TB1'den; M = 0, X = 0, DT = 0; A, X, D, DT yığında). Y burada saklanır.
AR_TB1:
        .ASSUME M=0,X=0
        PHY
        LDA   A,R_TB1                   ; ölçülen yarım periyot
        CLC
        ADC   A,AR_BEKLE
        CMP   A,#AR_KISA
        BCS   RTB_1
        STA   A,AR_BEKLE                ; kısa darbe: sonrakine ekle
        JMP   RTB_SON
RTB_1:  LDM   #0,AR_BEKLE
        CMP   A,#AR_UZUN
        BCC   RTB_2
        LDM   #0,AR_DPREV               ; sinyal yok: çerçeve biter
        SEM
        CLB   #$02,AR_DURUM
        JMP   RTB_SON
        .ASSUME M=0,X=0
RTB_2:  STA   A,AR_IT
        CLC
        ADC   A,AR_DPREV                ; tam periyot
        LDX   AR_IT
        STX   AR_DPREV
        LDY   #0
        CMP   A,#AR_ESIK
        BCC   RTB_3
        INY                             ; mark
RTB_3:  TYA
        SEM
        CMP   A,AR_TON
        BEQ   RTB_SAAT
        STA   A,AR_TON                  ; ton değişti: bit sınırı (AR_P = 210) beklenir
        CLM
        LDA   A,AR_P
        SEC
        SBC   A,#AR_BIT/2               ; hata -210..+209
        CMP   A,#$8000                  ; işaretli 2 bit sağa
        ROR   A
        CMP   A,#$8000
        ROR   A
        STA   A,AR_IT                   ; düzeltme
        LDA   A,AR_P
        SEC
        SBC   A,AR_IT
        BPL   RTB_4
        CLC
        ADC   A,#AR_BIT
RTB_4:  CMP   A,#AR_BIT
        BCC   RTB_5
        SBC   A,#AR_BIT
RTB_5:  STA   A,AR_P
RTB_SAAT:
        CLM
        LDA   A,AR_P
        CLC
        ADC   A,AR_DPREV                ; (= bu yarım periyot)
RTB_DONGU:
        CMP   A,#AR_BIT
        BCC   RTB_6
        SBC   A,#AR_BIT                 ; bit ortası geçildi: örnekle
        STA   A,AR_P
        SEM
        LDY   #1
        LDA   A,AR_TON
        CMP   A,AR_SONTON
        BEQ   RTB_7
        LDY   #0
RTB_7:  STA   A,AR_SONTON
        TYA
        JSR   AR_BIT_AL
        CLM
        LDA   A,AR_P
        BRA   RTB_DONGU
RTB_6:  STA   A,AR_P
RTB_SON:
        CLM
        PLY
        PUL   #$35
        RTI

; Bir bit (A = 0 / 1, M = 1). HDLC.
AR_BIT_AL:
        .ASSUME M=1,X=0
        CMP   A,#0
        BEQ   RBA_0
        INC   AR_BIRLER                 ; 1
        LDA   A,AR_BIRLER
        CMP   A,#7
        BCC   RBA_1
        CLB   #$02,AR_DURUM             ; yedi 1: iptal
        LDM   #6,AR_BIRLER              ; (taşmasın)
RBA_1:  SEC
        BRA   RBA_KAYDIR
RBA_0:  LDA   A,AR_BIRLER
        LDM   #0,AR_BIRLER
        CMP   A,#5
        BNE   RBA_2
        RTS                             ; doldurma biti
RBA_2:  CMP   A,#6
        BNE   RBA_3
        JMP   AR_BAYRAK
RBA_3:  CLC
RBA_KAYDIR:
        ROR   AR_BAYT                   ; LSB önce: yeni bit 7'ye
        BBS   #$02,AR_DURUM,RBA_4
        RTS
RBA_4:  INC   AR_BITSAY
        LDA   A,AR_BITSAY
        CMP   A,#8
        BEQ   RBA_5
        RTS
RBA_5:  LDM   #0,AR_BITSAY
; bayt tamam
        CLM
        LDX   AR_SAY
        CPX   #14
        BCS   RBA_6
        SEM
        LDA   A,AR_BAYT
        STA   A,AR_ADR,X
RBA_6:  CLM
        INX
        STX   AR_SAY
        CPX   #331
        BCC   RBA_7
        SEM
        CLB   #$02,AR_DURUM             ; çok uzun
        RTS
        .ASSUME M=0,X=0
RBA_7:  LDA   A,AR_BAYT                 ; CRC-16-CCITT (yansıtılmış)
        AND   A,#$00FF
        EOR   A,AR_CRC
        LDX   #8
RBA_8:  LSR   A
        BCC   RBA_9
        EOR   A,#$8408
RBA_9:  DEX
        BNE   RBA_8
        STA   A,AR_CRC
        SEM
        RTS

; Bayrak (M = 1): biten çerçeveyi denetle, yenisine başla
AR_BAYRAK:
        .ASSUME M=1,X=0
        BBC   #$02,AR_DURUM,RBY_YENI
        LDA   A,AR_BITSAY
        CMP   A,#7                      ; bayrağın 7 biti: çerçeve bayta hizalı
        BNE   RBY_YENI
        CLM
        LDA   A,AR_SAY
        CMP   A,#18
        BCC   RBY_YENI
        LDA   A,AR_CRC
        CMP   A,#$F0B8
        BNE   RBY_YENI
        SEM
        LDA   A,AR_YENI
        BNE   RBY_YENI                  ; öncekini ana döngü henüz almadı
        LDX   #0
RBY_1:  LDA   A,AR_ADR+7,X              ; gönderen
        STA   A,AR_YENI_ADR,X
        INX
        CPX   #7
        BNE   RBY_1
        LDM   #1,AR_YENI
RBY_YENI:
        SEM
        SEB   #$02,AR_DURUM
        LDM   #0,AR_BITSAY
        CLM
        LDM   #0,AR_SAY
        LDM   #$FFFF,AR_CRC
        SEM
        RTS

; ---------------------------------------------------------------------------------------------
; Almayı kur / kaldır (M = 1 giriş ve çıkış). AR_AC: TB1 ölçme, kesme önceliği 6.
; Firmware alma girişinde ve susturma değişince seçmeli çağrı çözücüsünü durdurur (F_007CF6:
; TB1 durur, TB1IC = 0); bu yüzden her turda TB1'in gerçekten çalıştığına bakılır.
AR_AC:
        .ASSUME M=1,X=0
        BBC   #$01,AR_DURUM,RAC_KUR
        BBC   #$40,R_TCSTART,RAC_KUR
        LDA   A,R_TB1IC
        AND   A,#$07
        CMP   A,#$06
        BEQ   RAC_SON
RAC_KUR:
        CLB   #$40,R_TCSTART
        LDM   #R_TB1MOD_OLC,R_TB1MOD
        LDM   #0,AR_DURUM
        LDM   #0,AR_BIRLER
        LDM   #0,AR_BITSAY
        LDM   #1,AR_TON
        LDM   #1,AR_SONTON
        CLM
        LDM   #0,AR_DPREV
        LDM   #0,AR_P
        LDM   #0,AR_BEKLE
        SEM
        SEB   #$01,AR_DURUM
        LDM   #$06,R_TB1IC
        SEB   #$40,R_TCSTART
RAC_SON:
        RTS

AR_KAPAT:
        .ASSUME M=1,X=0
        BBC   #$01,AR_DURUM,RKP_SON
        CLB   #$40,R_TCSTART
        LDM   #$00,R_TB1IC
        LDM   #$00,R_TB1MOD
        LDM   #0,AR_DURUM
RKP_SON:
        RTS

; Açılışta (AP_ACILIS_DENETLE, M = 1): RAM rastgele
AR_SIFIRLA:
        .ASSUME M=1,X=0
        LDM   #0,AR_DURUM
        LDM   #0,AR_YENI
        LDM   #0,AR_GOSTER
        LDM   #0,AR_LN
        LDM   #0,AR_SEC
        RTS

; ---------------------------------------------------------------------------------------------
; Ana döngüden (APRS_KANCA, APRS açıkken her tur; M = 0, A = NC77 çerçeve göstericisi).
; Almayı yönetir, yeni adresi listeye ekleyip gösterir, gösterme süresini sayar.
AR_ISLE:
        .ASSUME M=0,X=0
        STA   A,AR_D                    ; çerçeve
        SEM
        LDA   A,RADYO_DURUM             ; alma: 2..3
        CMP   A,#2
        BCC   RIS_KAPAT
        CMP   A,#4
        BCS   RIS_KAPAT
        JSR   AR_AC
        BRA   RIS_1
RIS_KAPAT:
        JSR   AR_KAPAT
RIS_1:   LDA   A,AR_YENI
        BEQ   RIS_SURE
        JSR   AR_LISTEYE
        LDM   #0,AR_YENI
        LDM   #0,AR_SEC
        LDA   A,RADYO_DURUM
        CMP   A,#4
        BCS   RIS_SON                    ; gönderiyor: ekrana dokunma
        LDX   #0                        ; listenin başı = yeni gelen
        JSR   AR_CAGRI_YAZ
        LDM   #AR_SURE,AR_GOSTER
        BRA   RIS_LCD
RIS_SURE:
        LDA   A,AR_GOSTER
        BEQ   RIS_SON
        LDA   A,AP_FARK                 ; AP_OTO_ZAMAN bu turda hesapladı
        BEQ   RIS_SON
        LDA   A,AR_GOSTER
        SEC
        SBC   A,AP_FARK
        BCS   RIS_2
        LDA   A,#0
RIS_2:   STA   A,AR_GOSTER
        BNE   RIS_SON
        LDA   A,RADYO_DURUM
        CMP   A,#4
        BCC   RIS_3
        LDM   #1,AR_GOSTER              ; gönderiyor: sonra dön
        BRA   RIS_SON
RIS_3:   LDM   #0,AR_SEC
        CLM
        JSR   AP_EKRAN                  ; süre doldu: APRS ekranı
RIS_LCD: CLM
        LDA   A,AR_D
        PHA
        JSRL  F_LCD
        CLM
        PLA
RIS_SON: CLM
        RTS

; Tuş (APRS_TUS'tan, M = 0): '*' / '#' ile listede gezin. C = 1: tuş işlendi (ekran yazıldı)
AR_TUS:
        .ASSUME M=0,X=0
        SEM
        LDA   A,KEY
        CMP   A,#'*'
        BEQ   RTU_ESKI
        CMP   A,#'#'
        BEQ   RTU_YENI
        CLM
        CLC
        RTS
        .ASSUME M=1,X=0
RTU_ESKI:
        LDA   A,AR_SEC
        CMP   A,AR_LN
        BCS   RTU_GOSTER                 ; en eskide kal
        INC   AR_SEC
        BRA   RTU_GOSTER
RTU_YENI:
        LDA   A,AR_SEC
        BEQ   RTU_GOSTER
        DEC   AR_SEC
RTU_GOSTER:
        LDA   A,AR_SEC
        BNE   RTU_OGE
        LDA   A,AR_LN                   ; liste boş ve '*': "EMPTY"
        BNE   RTU_APRS
        LDA   A,KEY
        CMP   A,#'*'
        BNE   RTU_APRS
        CLM
        LDX   #0
        SEM
        BBC   #$80,OPTIONS,RTU_BOS
        LDX   #8
RTU_BOS: CLM
        LDY   #0
        SEM
RTU_B1:  LDA   A,>AR_METIN,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   RTU_B1
        LDM   #AR_SURE,AR_GOSTER
        BRA   RTU_EVET
RTU_APRS:
        LDM   #0,AR_GOSTER              ; APRS ekranı
        CLM
        JSR   AP_EKRAN
        BRA   RTU_EVET
        .ASSUME M=1,X=0
RTU_OGE: DEC   A                         ; X = 7 * (sec - 1)
        STA   A,AR_T
        ASL   A
        ASL   A
        ASL   A
        SEC
        SBC   A,AR_T
        CLM
        AND   A,#$00FF
        TAX
        SEM
        JSR   AR_CAGRI_YAZ
        LDM   #AR_SURE,AR_GOSTER
RTU_EVET:
        CLM
        SEC
        RTS

AR_METIN:
        .TEXT8 "EMPTY"
        .TEXT8 "BOS"

; AR_YENI_ADR'yi listenin başına koy; aynı istasyon varsa oradan çıkar (M = 1)
AR_LISTEYE:
        .ASSUME M=1,X=0
        LDM   #0,AR_T                   ; öğe sırası
        LDX   #0                        ; X = öğenin başı
RLY_ARA:
        LDA   A,AR_T
        CMP   A,AR_LN
        BCS   RLY_YOK
        PHX
        LDY   #0
RLY_K:  LDA   A,AR_LISTE,X
        CMP   A,AR_YENI_ADR,Y
        BNE   RLY_FARKLI
        INX
        INY
        CPY   #7
        BNE   RLY_K
        PLX                             ; bulundu: önündekiler bir kayar, bu öğenin üstüne
        BRA   RLY_KAYDIR
RLY_FARKLI:
        PLX
        CLM
        TXA
        CLC
        ADC   A,#7
        TAX
        SEM
        INC   AR_T
        BRA   RLY_ARA
RLY_YOK:                                ; yok: liste doluysa sonuncusu düşer
        LDA   A,AR_LN
        CMP   A,#AR_N_MAX
        BCS   RLY_DOLU
        INC   AR_LN
        LDA   A,AR_LN
RLY_DOLU:
        DEC   A                         ; kayacak öğe sayısı = yeni sayı - 1
        STA   A,AR_T
        ASL   A
        ASL   A
        ASL   A
        SEC
        SBC   A,AR_T
        CLM
        AND   A,#$00FF
        TAX                             ; X = kayacak bayt sayısı
        SEM
RLY_KAYDIR:                             ; AR_LISTE[0..X-1] -> AR_LISTE[7..X+6] (sondan başa)
        CPX   #0
        BEQ   RLY_KOY
        DEX
        LDA   A,AR_LISTE,X
        STA   A,AR_LISTE+7,X
        BRA   RLY_KAYDIR
RLY_KOY:
        LDX   #0
RLY_K3: LDA   A,AR_YENI_ADR,X
        STA   A,AR_LISTE,X
        INX
        CPX   #7
        BNE   RLY_K3
        RTS

; AR_LISTE[X..X+6]'daki adresi LCD'ye yaz: "TA7W-7". 6 harfli çağrı + iki haneli SSID'de tire
; yazılmaz (8 karakter). M = 1 giriş ve çıkış.
AR_CAGRI_YAZ:
        .ASSUME M=1,X=0
        LDY   #0                        ; LCD konumu
        LDM   #6,AR_T
RCY_1:  LDA   A,AR_LISTE,X
        LSR   A
        CMP   A,#' '
        BEQ   RCY_2
        STA   A,LCD,Y
        INY
RCY_2:  INX
        DEC   AR_T
        BNE   RCY_1
        LDA   A,AR_LISTE,X              ; SSID baytı
        LSR   A
        AND   A,#$0F
        BEQ   RCY_DOLDUR
        STA   A,AR_T
        CMP   A,#10
        BCS   RCY_IKI
        LDA   A,#'-'
        STA   A,LCD,Y
        INY
        LDA   A,AR_T
        BRA   RCY_HANE
RCY_IKI:
        CPY   #6
        BCS   RCY_I1
        LDA   A,#'-'
        STA   A,LCD,Y
        INY
RCY_I1: LDA   A,#'1'
        STA   A,LCD,Y
        INY
        LDA   A,AR_T
        SEC
        SBC   A,#10
RCY_HANE:
        CLC
        ADC   A,#'0'
        STA   A,LCD,Y
        INY
RCY_DOLDUR:
        CPY   #8
        BCS   RCY_SON
        LDA   A,#' '
        STA   A,LCD,Y
        INY
        BRA   RCY_DOLDUR
RCY_SON:
        RTS
