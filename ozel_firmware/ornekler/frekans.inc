; frekans.inc - FREKANS menü öğesi: tuş takımından alma ve gönderme frekansı girip telsizi
;               onlara ayarlama (VFO modu)
;
;   ... m (FREKANS) s   -> "___ ____"            1. adım: ALMA (RX) frekansı
;   0-9      frekans, noktasız: 156525 = 156,525 MHz; 12,5 kHz adım için 7 hane: 1565125
;            ekranda "156 5125" (3. haneden sonra boşluk = ondalık nokta)
;   *        son haneyi sil
;   #        alma frekansını hafızaya al (onay sesi), ekranda "GONDERME" ("TX FREQ")
;                                                 2. adım: GÖNDERME (TX) frekansı
;   0-9 *    aynı şekilde gönderme frekansı: "160 65--" ('-' = gönderme frekansı giriliyor)
;   #        gönderme frekansını hafızaya al, ekranda "RX TONE" ("ALMA TON")
;            hiç hane girmeden # = gönderme de alma frekansında (simplex)
;   * (boşken) 1. adıma, alma frekansına geri dön
;                                                 3. adım: ALMA TKS (CTCSS) tonu, "RT  88 5"
;   0-9      ton, Hz: son hane ondalık (885 = 88,5 Hz, 2503 = 250,3 Hz); 600'den küçük sayı tam
;            Hz sayılır (67 = 67,0 Hz, 100 = 100,0 Hz). En yakın standart ton seçilir (1 Hz içinde)
;   0 #      ton yok;  hiç hane girmeden # = tonu değiştirme (son kanalın / VFO'nun tonu kalır)
;   #        ekranda "TX TONE" ("GON TON")
;                                                 4. adım: GÖNDERME tonu, "TT  88 5"
;   0-9 * #  aynı şekilde; hiç hane girmeden # = alma tonuyla aynı
;   #        ayarla: telsiz RX'te 1. frekansı ve alma tonunu, TX'te 2. frekansı ve gönderme
;            tonunu kullanır (onay sesi)
;   * (boşken) bir önceki adıma dön
;   s        girişi temizle
;   m        çık (telsiz yeni frekanslarda kalır; kanal anahtarı çevrilince kanala döner)
; FREKANS açıkken firmware'in ~4 s'lik menü zaman aşımı kapalıdır (VFO_KANCA sayacı yeniler).
;
; VFO modunda ekran almada alma frekansını, gönderirken (PTT) gönderme frekansını gösterir
; (normal ekranda ve ayardan sonra açık kalan FREKANS menüsünde): VFO_KANCA telsiz durum
; makinesinin her turunda (0x00F84D) radyo durumuna bakar. Kanal numarası yerine almada "VR",
; gönderirken "VT" yazar (VFO_KNO, 0x008332).
;
; Hatalar ("ERROR" / "HATA" + bip): 6 haneden az, 2,5 kHz ızgarasında değil, bandın dışında,
; telsiz göndermedeyken. Hatadan sonraki tuş girişe döner (aynı adımda kalınır).
;
; Firmware'in frekans düzeni (disasm/c_clean/synth.c, simülatörle doğrulandı):
;   f = (taban*10 + kelime) * 2,5 kHz;  taban RAM $0224 (25 kHz birim, EE[0x58C] kopyası)
;   kelime 14 bit (0..$3FFE; $3FFF = "TX yok")
;   toplam = f/2,5k (+ RX'te ara frekans kayması RAM $01F9: +18000 VHF, -18000 UHF)
;   adım: kelime%5 == 0 -> d=5 (12,5 kHz, kod 125); kelime%4 == 0 -> d=4 (10 kHz, 100);
;         yoksa d=2 (5 kHz, 50)
;   N = toplam / (d*P), A = (toplam % (d*P)) / d    P = ön bölücü RAM $020B (64 / 128)
;   R = fref / kod  (fref RAM $01F5, 32 bit: 128000 ya da 130000)
;   RW = R<<1 | 1 | (P == 64 ? $8000 : 0)
; MB1502'ye giden baytlar bit sırası ters (ROM $2B06 tablosu) ve RAM'de hazır tutulur:
;   $0370 rev(N lo)  $0371 rev(N hi)  $0372 rev(A<<1)  $0373 rev(RW hi)  $0374 rev(RW lo)   RX
;   $0375 RX bayrakları (kristal kaydırma)  $0376 kanal
;   $0377..$037B aynı düzende TX, $037C TX bayrakları
; $0363 bu 13 baytlık kaydın kopyası. Kanal seçince firmware (LIST menüsü, F_00350F) yeni
; kelimeleri $0370'e koyup radyo durumunu ($0228) 1 yapar: ana döngü RX'e yeniden girer ve
; sentezörü yükler (F_00EA11). Burada da aynısı yapılır; TX'e geçişte $0377 kullanılır.
; RX ve TX'in adımı (d, kod, RW) ayrı ayrı hesaplanır: iki frekans farklı ızgarada olabilir.
;
; Radyo durumu ($0228, simülatörde izlendi): 1 RX'e gir, 2 RX bekleme, PTT ile 4 -> 8 (gönderme),
; bırakınca 12 -> 1 -> 2. 4..12 gönderme tarafı sayılır.

FR_HANE     .EQU OZEL_RAM+$40   ; 7 bayt girilen haneler (ASCII)
FR_NDIG     .EQU OZEL_RAM+$47
FR_DURUM    .EQU OZEL_RAM+$48   ; bit 0: hata gösteriliyor, bit 1: gönderme frekansı adımı
FR_FU       .EQU OZEL_RAM+$4A   ; 4 bayt: frekans, 2,5 kHz birim
FR_KELIME   .EQU OZEL_RAM+$4E   ; 2 bayt
FR_D        .EQU OZEL_RAM+$50   ; 2 bayt: adım böleni d
FR_DP       .EQU OZEL_RAM+$52   ; 2 bayt: d*P
FR_KOD      .EQU OZEL_RAM+$54   ; 2 bayt: 125 / 100 / 50
FR_RW       .EQU OZEL_RAM+$56   ; 2 bayt
FR_BOLUNEN  .EQU OZEL_RAM+$58   ; 4 bayt: bölme girişi / bölüm
FR_BOLEN    .EQU OZEL_RAM+$5C   ; 2 bayt
FR_KALAN    .EQU OZEL_RAM+$5E   ; 4 bayt
FR_T        .EQU OZEL_RAM+$62   ; 4 bayt ara değer
FR_KAYIT    .EQU OZEL_RAM+$66   ; 5 bayt: hesaplanan sentezör baytları (FR_SENTEZ çıktısı)
; VFO modu (açılışta sıfırlanmaz: VFO_IMZA ve sentezör kaydının eşleşmesi aranır)
VFO_IMZA    .EQU OZEL_RAM+$70   ; 2 bayt: 'VF' = VFO modu kuruldu
VFO_DURUM   .EQU OZEL_RAM+$72   ; ekranda/kanal adında olan: 0 alma, 1 gönderme frekansı
VFO_RX_KAYIT .EQU OZEL_RAM+$73  ; 5 bayt: alma frekansının sentezör baytları
VFO_TX_KAYIT .EQU OZEL_RAM+$78  ; 5 bayt: gönderme frekansınınkiler
VFO_METIN   .EQU OZEL_RAM+$7D   ; 8 bayt alma frekansı metni, ardından 8 bayt gönderme
VFO_RX_HANE .EQU OZEL_RAM+$8D   ; 7 bayt: alma frekansının haneleri (geri dönmek / simplex için)

VFO_RX_TON  .EQU OZEL_RAM+$94   ; alma tonu sıra numarası (0 = yok, 1..50), FR_UYGULA $0375'e yazar
VFO_TX_TON  .EQU OZEL_RAM+$95   ; gönderme tonu ($037C)
FR_TON      .EQU OZEL_RAM+$96   ; 2 bayt: girilen ton, 0,1 Hz

F_HATA_G    .EQU 1
F_TX_ADIM   .EQU 2
F_RXTON     .EQU 4              ; 3. adım: alma tonu
F_TXTON     .EQU 8              ; 4. adım: gönderme tonu
F_TON       .EQU F_RXTON|F_TXTON
TON_YOK     .EQU $FF            ; FR_TON_COZ: hane girilmedi
VFO_IMZA_D  .EQU $4656          ; "VF"

SYN_KAYIT   .EQU $0370          ; geçerli kanalın sentezör kaydı (13 bayt)
SYN_YEDEK   .EQU $0363          ; kopyası
RADYO_DURUM .EQU $0228          ; 1 = RX'e gir (sentezörü yükle), 2..3 RX, 4..12 TX tarafı
KANAL_ADI   .EQU $024A          ; normal ekranda gösterilen 8 karakter
BAND_TABAN  .EQU $0224
RX_KAYMA    .EQU $01F9
ON_BOLUCU   .EQU $020B
FREF        .EQU $01F5
TERS_TABLO  .EQU $002B06        ; bit sırası ters çevirme tablosu (256 bayt)
F_RADYO_SON .EQU $00E626        ; telsiz durum makinesinin sonunda çağrılan fonksiyon
MENU_SAYAC  .EQU $0230          ; menü zaman aşımı: geri sayar (menüye girişte $3F, ~4 s)
MENU_OLAY   .EQU $0231          ; sayaç bitince yapılacak iş; $14 = normal ekrana dön

; ---------------------------------------------------------------------------------------------
; Kanca: durum makinesinin sonundaki "JSRL F_00E626" bizim yordamımıza gider
FR_KOD_ADRES .EQU *
        .ORG $00F84D
        .EXPECT $22,$26,$E6,$00
        JSRL  VFO_KANCA
        .ORG $008332                    ; normal ekranın kanal numarası: VFO'da "VR" / "VT"
        .EXPECT $AD,$76,$03,$F8
        JSRL  VFO_KNO
        ; Taramada (0x0240 bit 10) aynı yer kanal numarasını 0x0369'dan alıp "JMP L_008335"
        ; ile SEM'e atlıyordu; 0x8335 artık JSRL'nin son baytı ($00 = BRK) -> telsiz yeniden
        ; başlıyordu. SEM'i buraya alıp JSRL'den sonraki STA'ya (0x8336) geçiyoruz.
        .ORG $00832C
        .EXPECT $AD,$69,$03,$4C,$35,$83 ; LDA A,$0369 / JMP L_008335
        .ASSUME M=0,X=0
        LDA   A,$0369
        SEM
        .ASSUME M=1,X=0
        BRA   VK_STA
VK_STA      .EQU $008336
        .ORG FR_KOD_ADRES

; ---------------------------------------------------------------------------------------------
FREKANS_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        LDM   #FREKANS_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(FREKANS_TUS),HANDLER_BK
        LDM   #0,FR_NDIG
        LDM   #0,FR_DURUM               ; 1. adım: alma frekansı
        JMP   FK_SON

; ---------------------------------------------------------------------------------------------
FREKANS_TUS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        SEM
        BBC   #F_HATA_G,FR_DURUM,FK_1
        CLB   #F_HATA_G,FR_DURUM        ; hata gösteriliyordu: bu tuş yalnız onu kapatır
        LDA   A,KEY
        CMP   A,#'d'
        BNE   FK_SON
        LDM   #0,NDIGIT                 ; rakamsa firmware'in tamponunu da boşalt
        BRA   FK_SON
FK_1:   LDA   A,KEY
        CMP   A,#'d'
        BEQ   FK_RAKAM
        CMP   A,#'*'
        BEQ   FK_SIL
        CMP   A,#'s'
        BEQ   FK_TEMIZLE
        CMP   A,#'#'
        BNE   FK_SON
        JMP   FK_AYARLA

FK_SON: CLM
        JSR   FR_EKRAN
        PLA
        RTL

FK_HATA:
        SEM                             ; hata metni + bip
        SEB   #F_HATA_G,FR_DURUM
FK_BIP: CLM                             ; yalnız bip
        LDY   #13
        JSRL  F_BEEP
        BRA   FK_SON

FK_TEMIZLE:
        .ASSUME M=1,X=0
        LDM   #0,FR_NDIG
        BRA   FK_SON

FK_SIL: .ASSUME M=1,X=0
        LDA   A,FR_NDIG
        BEQ   FK_GERI
        DEC   FR_NDIG
        BRA   FK_SON
FK_GERI:                                ; boşken *: bir önceki adıma dön
        BBC   #F_TXTON,FR_DURUM,FK_G1
        CLB   #F_TXTON,FR_DURUM         ; gönderme tonu -> alma tonu
        SEB   #F_RXTON,FR_DURUM
        BRA   FK_SON
FK_G1:  BBC   #F_RXTON,FR_DURUM,FK_G2
        CLB   #F_RXTON,FR_DURUM         ; alma tonu -> gönderme frekansı (boş: # = simplex)
        SEB   #F_TX_ADIM,FR_DURUM
        BRA   FK_SON
FK_G2:  BBC   #F_TX_ADIM,FR_DURUM,FK_SON
        CLB   #F_TX_ADIM,FR_DURUM
        CLM
        JSR   FR_RX_HANELER
        BRA   FK_SON

FK_RAKAM:
        .ASSUME M=1,X=0
        LDA   A,NDIGIT                  ; firmware'in tamponundan al
        DEC   A
        CLM
        AND   A,#$000F
        TAX
        SEM
        LDA   A,DIGITS,X
        LDM   #0,NDIGIT
        STA   A,FR_T
        CLM
        LDA   A,FR_NDIG
        AND   A,#$00FF
        TAX
        SEM
        CPX   #7
        BCS   FK_BIP
        BBC   #F_TON,FR_DURUM,FK_HANE   ; ton adımında en çok 4 hane
        CPX   #4
        BCS   FK_BIP
FK_HANE:
        LDA   A,FR_T
        STA   A,FR_HANE,X
        INC   FR_NDIG
        JMP   FK_SON

; ---- # : alma frekansını sakla ya da (2. adımda) ayarla
FK_AYARLA:
        .ASSUME M=1,X=0
        BBC   #F_TON,FR_DURUM,FA_FREK
        JMP   FA_TON
FA_FREK:
        BBC   #F_TX_ADIM,FR_DURUM,FA_0
        LDA   A,FR_NDIG                 ; 2. adımda hane yoksa: simplex
        BNE   FA_0
        CLM
        JSR   FR_RX_HANELER
        SEM
FA_0:   LDA   A,FR_NDIG
        CMP   A,#6
        BCC   FA_HATA
        BBC   #F_TX_ADIM,FR_DURUM,FA_HESAP
        LDA   A,RADYO_DURUM             ; göndermedeyken değiştirme
        CMP   A,#4
        BCC   FA_HESAP
FA_HATA:
        JMP   FK_HATA
FA_HESAP:
        CLM
        JSR   FR_HESAPLA                ; C = 1: geçersiz frekans
        BCC   FA_GECERLI
        JMP   FK_HATA
FA_GECERLI:
        SEM                             ; 6 hane girildiyse 7. hane 0
        LDA   A,FR_NDIG
        CMP   A,#7
        BEQ   FA_1
        LDM   #'0',FR_HANE+6
        LDM   #7,FR_NDIG
FA_1:   BBS   #F_TX_ADIM,FR_DURUM,FA_TX
        ; 1. adım: alma frekansı -> sentezör baytları, metin, haneler; ekranı temizle
        CLM
        LDA   A,RX_KAYMA
        JSR   FR_SENTEZ
        LDX   #0
        JSR   FR_SAKLA
        LDX   #0
        SEM
FA_RXH: LDA   A,FR_HANE,X
        STA   A,VFO_RX_HANE,X
        INX
        CPX   #7
        BNE   FA_RXH
        SEB   #F_TX_ADIM,FR_DURUM
        LDM   #0,FR_NDIG
        BRA   FA_ONAY
FA_TX:  ; 2. adım: gönderme frekansı (kayma yok) -> sakla, alma tonuna geç
        CLM
        LDA   A,#0
        JSR   FR_SENTEZ
        LDX   #5
        JSR   FR_SAKLA
        SEM
        CLB   #F_TX_ADIM,FR_DURUM
        SEB   #F_RXTON,FR_DURUM
        LDM   #0,FR_NDIG
        BRA   FA_ONAY

; ---- # : 3. adım alma tonu, 4. adım gönderme tonu (ve telsizi ayarla)
FA_TON: .ASSUME M=1,X=0
        CLM
        JSR   FR_TON_COZ                ; A = sıra numarası / TON_YOK, C = 1: geçersiz
        SEM
        BCC   FT_1
        JMP   FK_HATA
FT_1:   BBC   #F_RXTON,FR_DURUM,FT_TX
        CMP   A,#TON_YOK                ; boş: şimdiki alma tonu kalır
        BNE   FT_RX
        LDA   A,SYN_KAYIT+5
        AND   A,#$3F
FT_RX:  STA   A,VFO_RX_TON
        CLB   #F_RXTON,FR_DURUM
        SEB   #F_TXTON,FR_DURUM
        LDM   #0,FR_NDIG
        BRA   FA_ONAY
FT_TX:  CMP   A,#TON_YOK                ; boş: alma tonuyla aynı
        BNE   FT_TX2
        LDA   A,VFO_RX_TON
FT_TX2: STA   A,VFO_TX_TON
        LDA   A,RADYO_DURUM             ; göndermedeyken değiştirme
        CMP   A,#4
        BCC   FT_UYGULA
        JMP   FK_HATA
FT_UYGULA:
        CLM
        JSR   FR_UYGULA                 ; sentezör kaydı, tonlar, kanal adı, RX'i yeniden yükle
        SEM
        CLB   #F_TXTON,FR_DURUM
        CLM
        JSR   FR_RX_HANELER             ; ekranda alma frekansı kalır
FA_ONAY:
        CLM
        LDY   #3                        ; onay: 2 x 1000 Hz
        JSRL  F_BEEP
        JMP   FK_SON

; ---------------------------------------------------------------------------------------------
; Alt yordamlar (JSR / RTS, M=0 giriş ve çıkış). Firmware fonksiyonu çağırmazlar.

; FR_HANE (en çok 4 hane) -> A = ton sıra numarası (0 = yok, 1..50), TON_YOK = hane yok.
; Son hane ondalık; 600'den küçük sayı tam Hz. En yakın tablo tonu, 1 Hz'den uzaksa C = 1.
FR_TON_COZ:
        .ASSUME M=0,X=0
        SEM
        LDA   A,FR_NDIG
        CLM
        AND   A,#$00FF
        BNE   FTC_1
        LDA   A,#TON_YOK
        CLC
        RTS
FTC_1:  LDM   #0,FR_TON
        LDX   #0
FTC_HANE:
        LDA   A,FR_TON                  ; FR_TON = FR_TON * 10 + hane
        ASL   A
        STA   A,FR_T
        ASL   A
        ASL   A
        CLC
        ADC   A,FR_T
        STA   A,FR_TON
        LDA   A,FR_HANE,X
        AND   A,#$000F
        CLC
        ADC   A,FR_TON
        STA   A,FR_TON
        INX
        SEM
        TXA
        CMP   A,FR_NDIG
        CLM
        BNE   FTC_HANE
        LDA   A,FR_TON
        BNE   FTC_2
        CLC                             ; 0 = ton yok
        RTS
FTC_2:  CMP   A,#600                    ; tam Hz yazılmış: 0,1 Hz'e çevir
        BCS   FTC_3
        ASL   A
        STA   A,FR_T
        ASL   A
        ASL   A
        CLC
        ADC   A,FR_T
        STA   A,FR_TON
FTC_3:  LDM   #$FFFF,FR_T               ; en iyi fark
        LDX   #0                        ; tablo indeksi * 2
FTC_ARA:
        SEC
        LDA   A,FR_TON
        SBC   A,>TON_TABLO,X
        BCS   FTC_POZ
        EOR   A,#$FFFF
        INC   A
FTC_POZ:
        CMP   A,FR_T
        BCS   FTC_SONRAKI
        STA   A,FR_T
        STX   FR_T+2
FTC_SONRAKI:
        INX
        INX
        CPX   #2*50
        BNE   FTC_ARA
        LDA   A,FR_T
        CMP   A,#11                     ; 1 Hz içinde mi
        BCS   FTC_YOK
        LDA   A,FR_T+2
        LSR   A
        INC   A                         ; sıra numarası 1..50
        CLC
        RTS
FTC_YOK:
        SEC
        RTS

; TKS tonları, 0,1 Hz; firmware'in sıra numarası 1..50 (ROM $2385 periyot tablosuyla aynı sıra)
TON_TABLO:
        .WORD 670,719,744,770,797,825,854,885,915,948,974,1000,1035,1072,1109,1148
        .WORD 1188,1230,1273,1318,1365,1413,1462,1514,1567,1622,1679,1738,1799,1862,1928,2035
        .WORD 2107,2181,2257,2336,2418,2503,693,1598,1655,1713,1773,1835,1899,1966,1995,2065
        .WORD 2291,2541

; FR_HANE -> FR_FU (2,5 kHz birim), FR_KELIME, FR_D, FR_DP, FR_KOD, FR_RW.  C = 1: geçersiz
FR_HESAPLA:
        .ASSUME M=0,X=0
        LDM   #0,FR_BOLUNEN             ; haneleri 100 Hz birimine çevir (7 haneye tamamla)
        LDM   #0,FR_BOLUNEN+2
        LDX   #0
FH_HANE:
        JSR   FR_ON_KAT                 ; BOLUNEN *= 10
        LDA   A,FR_HANE,X
        AND   A,#$000F
        CLC
        ADC   A,FR_BOLUNEN
        STA   A,FR_BOLUNEN
        LDA   A,FR_BOLUNEN+2
        ADC   A,#0
        STA   A,FR_BOLUNEN+2
        INX
        SEM
        TXA
        CMP   A,FR_NDIG
        CLM
        BNE   FH_HANE
        CPX   #7
        BEQ   FH_7
        JSR   FR_ON_KAT                 ; 6 hane: kHz -> 100 Hz
FH_7:   LDA   A,#25                     ; 100 Hz -> 2,5 kHz; kalan olmamalı
        JSR   FR_BOL
        LDA   A,FR_KALAN
        BEQ   FH_IZGARA
        JMP   FH_GECERSIZ               ; 2,5 kHz ızgarasında değil
FH_IZGARA:
        LDA   A,FR_BOLUNEN
        STA   A,FR_FU
        LDA   A,FR_BOLUNEN+2
        STA   A,FR_FU+2
        LDA   A,BAND_TABAN              ; kelime = FU - taban*10 (32 bit: UHF'de 160000)
        STA   A,FR_T
        LDM   #0,FR_T+2
        JSR   FR_T_ON_KAT
        SEC
        LDA   A,FR_FU
        SBC   A,FR_T
        STA   A,FR_KELIME
        LDA   A,FR_FU+2
        SBC   A,FR_T+2
        BNE   FH_BANT_DISI              ; negatif ya da >= 65536
        LDA   A,FR_KELIME
        CMP   A,#$3FFF
        BCC   FH_BANT_ICI
FH_BANT_DISI:
        JMP   FH_GECERSIZ
FH_BANT_ICI:
        STA   A,FR_BOLUNEN              ; adım: kelime % 5, kelime % 4
        LDM   #0,FR_BOLUNEN+2
        LDA   A,#5
        JSR   FR_BOL
        LDA   A,FR_KALAN
        BNE   FH_10
        LDA   A,#5
        LDX   #125
        BRA   FH_ADIM
FH_10:  LDA   A,FR_KELIME
        AND   A,#3
        BNE   FH_5
        LDA   A,#4
        LDX   #100
        BRA   FH_ADIM
FH_5:   LDA   A,#2
        LDX   #50
FH_ADIM:
        STA   A,FR_D
        STX   FR_KOD
        STA   A,FR_DP                   ; d*P: P = 64 ya da 128 (kaydırma)
        LDY   #6
        SEM
        LDA   A,ON_BOLUCU
        CLM
        BPL   FH_DP                     ; 128 ise bit 7 dolu
        INY
FH_DP:  ASL   FR_DP
        DEY
        BNE   FH_DP
        LDA   A,FREF                    ; R = fref / kod
        STA   A,FR_BOLUNEN
        LDA   A,FREF+2
        STA   A,FR_BOLUNEN+2
        LDA   A,FR_KOD
        JSR   FR_BOL
        LDA   A,FR_BOLUNEN              ; RW = R<<1 | 1 | (P == 64 ? $8000 : 0)
        ASL   A
        ORA   A,#1
        STA   A,FR_RW
        SEM
        LDA   A,ON_BOLUCU
        CLM
        BMI   FH_TAMAM
        LDA   A,FR_RW
        ORA   A,#$8000
        STA   A,FR_RW
FH_TAMAM:
        CLC
        RTS
FH_GECERSIZ:
        SEC
        RTS

; BOLUNEN *= 10 (32 bit, X korunur)
FR_ON_KAT:
        .ASSUME M=0,X=0
        LDA   A,FR_BOLUNEN
        STA   A,FR_T
        LDA   A,FR_BOLUNEN+2
        STA   A,FR_T+2
        JSR   FR_T_ON_KAT
        LDA   A,FR_T
        STA   A,FR_BOLUNEN
        LDA   A,FR_T+2
        STA   A,FR_BOLUNEN+2
        RTS

; T *= 10 (32 bit): 10T = 8T + 2T, KALAN ara değer
FR_T_ON_KAT:
        .ASSUME M=0,X=0
        ASL   FR_T
        ROL   FR_T+2                    ; T = 2T
        LDA   A,FR_T
        STA   A,FR_KALAN
        LDA   A,FR_T+2
        STA   A,FR_KALAN+2
        ASL   FR_KALAN
        ROL   FR_KALAN+2
        ASL   FR_KALAN
        ROL   FR_KALAN+2                ; KALAN = 8T
        CLC
        LDA   A,FR_T
        ADC   A,FR_KALAN
        STA   A,FR_T
        LDA   A,FR_T+2
        ADC   A,FR_KALAN+2
        STA   A,FR_T+2
        RTS

; BOLUNEN (32 bit) / A (16 bit) -> BOLUNEN = bölüm, KALAN = kalan. Kaydır-çıkar, X korunur.
FR_BOL:
        .ASSUME M=0,X=0
        STA   A,FR_BOLEN
        PHX
        LDM   #0,FR_KALAN
        LDM   #0,FR_KALAN+2
        LDX   #32
FB_DON: ASL   FR_BOLUNEN
        ROL   FR_BOLUNEN+2
        ROL   FR_KALAN
        ROL   FR_KALAN+2
        SEC
        LDA   A,FR_KALAN
        SBC   A,FR_BOLEN
        TAY
        LDA   A,FR_KALAN+2
        SBC   A,#0
        BCC   FB_2
        STA   A,FR_KALAN+2
        STY   FR_KALAN
        INC   FR_BOLUNEN
FB_2:   DEX
        BNE   FB_DON
        PLX
        RTS

; toplam = FU + A (işaretli 16 bit kayma) için N/A/R baytları -> FR_KAYIT[0..4]
FR_SENTEZ:
        .ASSUME M=0,X=0
        STA   A,FR_T                    ; kaymayı 32 bite genişlet
        LDM   #0,FR_T+2
        CMP   A,#$8000
        BCC   FS_1
        LDM   #$FFFF,FR_T+2
FS_1:   CLC
        LDA   A,FR_FU
        ADC   A,FR_T
        STA   A,FR_BOLUNEN
        LDA   A,FR_FU+2
        ADC   A,FR_T+2
        STA   A,FR_BOLUNEN+2
        LDA   A,FR_DP                   ; N = toplam / (d*P)
        JSR   FR_BOL
        LDA   A,FR_BOLUNEN
        STA   A,FR_T                    ; N
        LDA   A,FR_KALAN                ; A = kalan / d
        STA   A,FR_BOLUNEN
        LDM   #0,FR_BOLUNEN+2
        LDA   A,FR_D
        JSR   FR_BOL
        LDA   A,FR_T                    ; [0] rev(N lo)
        JSR   FR_TERS
        STA   A,FR_KAYIT
        LDA   A,FR_T+1                  ; [1] rev(N hi)
        JSR   FR_TERS
        STA   A,FR_KAYIT+1
        LDA   A,FR_BOLUNEN              ; [2] rev(A<<1)
        ASL   A
        JSR   FR_TERS
        STA   A,FR_KAYIT+2
        LDA   A,FR_RW+1                 ; [3] rev(RW hi)
        JSR   FR_TERS
        STA   A,FR_KAYIT+3
        LDA   A,FR_RW                   ; [4] rev(RW lo)
        JSR   FR_TERS
        STA   A,FR_KAYIT+4
        RTS

; A alt baytının bit sırasını ters çevir (ROM tablosu); sonuç A'nın alt baytında (M=0)
FR_TERS:
        .ASSUME M=0,X=0
        AND   A,#$00FF
        TAX
        LDA   A,>TERS_TABLO,X
        AND   A,#$00FF
        RTS

; FR_KAYIT'i ve girilen frekansın metnini sakla: X = 0 alma, 5 gönderme (M=0)
FR_SAKLA:
        .ASSUME M=0,X=0
        LDY   #0
        SEM
FSK_1:  LDA   A,FR_KAYIT,Y
        STA   A,VFO_RX_KAYIT,X          ; X = 5 ise VFO_TX_KAYIT
        INX
        INY
        CPY   #5
        BNE   FSK_1
        CLM
        JSR   FR_METIN                  ; "156 5250" -> LCD
        LDX   #0                        ; metin: alma VFO_METIN, gönderme VFO_METIN+8
        SEM
        BBC   #F_TX_ADIM,FR_DURUM,FSK_2
        LDX   #8
FSK_2:  LDY   #0
FSK_3:  LDA   A,LCD,Y
        STA   A,VFO_METIN,X
        INX
        INY
        CPY   #8
        BNE   FSK_3
        CLM
        RTS

; alma frekansının hanelerini girişe geri koy (M=0)
FR_RX_HANELER:
        .ASSUME M=0,X=0
        LDX   #0
        SEM
FRH_1:  LDA   A,VFO_RX_HANE,X
        STA   A,FR_HANE,X
        INX
        CPX   #7
        BNE   FRH_1
        LDM   #7,FR_NDIG
        CLM
        RTS

; VFO'yu kur: RX ve TX sentezör baytlarını kayda yaz, kopyala, kanal adı = alma frekansı,
; RX'i yeniden yükle (M=0)
FR_UYGULA:
        .ASSUME M=0,X=0
        LDX   #0
        SEM
FU_1:   LDA   A,VFO_RX_KAYIT,X
        STA   A,SYN_KAYIT,X             ; $0370..$0374
        LDA   A,VFO_TX_KAYIT,X
        STA   A,SYN_KAYIT+7,X           ; $0377..$037B
        INX
        CPX   #5
        BNE   FU_1
        LDA   A,SYN_KAYIT+5             ; tonlar: alt 6 bit; üst bitler kanalın bayrakları
        AND   A,#$C0                    ; ($0375: kristal kaydırma, seçmeli çağrı)
        ORA   A,VFO_RX_TON
        STA   A,SYN_KAYIT+5
        LDA   A,SYN_KAYIT+12
        AND   A,#$C0
        ORA   A,VFO_TX_TON
        STA   A,SYN_KAYIT+12
        LDX   #0                        ; kaydın kopyası ($0363)
FU_2:   LDA   A,SYN_KAYIT,X
        STA   A,SYN_YEDEK,X
        INX
        CPX   #13
        BNE   FU_2
        LDX   #0
FU_3:   LDA   A,VFO_METIN,X
        STA   A,KANAL_ADI,X
        INX
        CPX   #8
        BNE   FU_3
        LDM   #0,VFO_DURUM              ; kanal adında alma frekansı
        LDM   #1,RADYO_DURUM            ; ana döngü RX'e yeniden girer, sentezörü yükler
        CLM
        LDM   #VFO_IMZA_D,VFO_IMZA
        RTS

; ---------------------------------------------------------------------------------------------
; VFO_KANCA: telsiz durum makinesinin her turunda (0x00F84D'den JSRL ile), sonra F_00E626'ya
; devam eder. VFO modundaysa ve almadan göndermeye (ya da tersine) geçildiyse kanal adını
; diğer frekansın metniyle değiştirir; ekranda eski frekans duruyorsa (normal ekran ya da
; ayardan sonra açık kalan FREKANS menüsü) ekranı da hemen yeniler.
; Kanal anahtarı / LIST / tarama sentezör kaydını değiştirince VFO modu kendiliğinden biter.
; Tüm yazmaçlar korunur; F_00E626 aynı yığınla çağrılmış olur.
VFO_KANCA:
        .ASSUME M=0,X=0
        PHP
        CLM
        PHA
        PHX
        PHY                             ; yığında 8 bayt: çerçeve göstericisi $0C,S
        JSR   APRS_KANCA                ; aprs.inc: APRS menüsünün zaman aşımı / çıkışı
        SEM                             ; FREKANS açıkken menü zaman aşımı yok (çıkış m ile):
        LDA   A,MENU                    ; firmware ~4 s tuşa basılmazsa normal ekrana atıyor,
        CMP   A,#ST_FREKANS_ON          ; ikinci frekansı girmeye vakit kalmıyordu
        BNE   VK_ZA
        LDA   A,MENU_OLAY
        CMP   A,#$14
        BNE   VK_ZA
        LDM   #$3F,MENU_SAYAC
VK_ZA:  CLM
        JSR   VFO_ETKIN
        BEQ   VK_VFO
        JMP   VK_BITTI
VK_VFO: SEM
        LDY   #0                        ; istenen: 0 alma, 1 gönderme (durum 4..12)
        LDA   A,RADYO_DURUM
        CMP   A,#4
        BCC   VK_ISTENEN
        CMP   A,#$0D
        BCS   VK_ISTENEN
        INY
VK_ISTENEN:
        TYA
        CMP   A,VFO_DURUM
        BEQ   VK_SON
        STA   A,VFO_DURUM
        CLM
        AND   A,#$00FF
        ASL   A
        ASL   A
        ASL   A
        TAX                             ; X = yeni metin (0 / 8)
        EOR   A,#8
        TAY                             ; Y = eski metin
        SEM                             ; ekranda eski metin mi? (normal ekran ya da
        PHX                             ; ayardan sonra FREKANS menüsü)
        LDX   #0
VK_KARS:
        LDA   A,VFO_METIN,Y
        CMP   A,LCD,X
        BNE   VK_BASKA
        INY
        INX
        CPX   #8
        BNE   VK_KARS
        PLX
        LDY   #0
VK_YAZ: LDA   A,VFO_METIN,X             ; kanal adı ve ekran tamponu
        STA   A,KANAL_ADI,Y
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   VK_YAZ
        CLM
        LDA   A,$0C,S                   ; NC77 çerçeve göstericisi
        PHA
        JSRL  F_LCD
        CLM
        PLA
        BRA   VK_SON
VK_BASKA:
        .ASSUME M=1,X=0
        PLX                             ; ekranda başka bir şey var: yalnız kanal adı
        LDY   #0
VK_AD1: LDA   A,VFO_METIN,X
        STA   A,KANAL_ADI,Y
        INX
        INY
        CPY   #8
        BNE   VK_AD1
        BRA   VK_SON
VK_BITTI:
        .ASSUME M=0,X=0
        LDM   #0,VFO_IMZA               ; kanala dönülmüş: VFO modu bitti
VK_SON: CLM
        PLY
        PLX
        PLA
        PLP
        JMPL  F_RADYO_SON

;  VFO modu kurulu ve sentezör kaydı hâlâ bizim mi? Z = 1: evet (M=0 giriş ve çıkış, X bozulur)
VFO_ETKIN:
        .ASSUME M=0,X=0
        LDA   A,VFO_IMZA
        CMP   A,#VFO_IMZA_D
        BNE   VE_SON
        LDX   #0
        SEM
VE_1:   LDA   A,SYN_KAYIT,X
        CMP   A,VFO_RX_KAYIT,X
        BNE   VE_2
        LDA   A,SYN_KAYIT+7,X
        CMP   A,VFO_TX_KAYIT,X
        BNE   VE_2
        INX
        CPX   #5
        BNE   VE_1
VE_2:   CLM                             ; Z korunur
VE_SON: RTS

; ---------------------------------------------------------------------------------------------
; VFO_KNO: normal ekranda kanal numarası yerine VFO modunda "VR" (alma) / "VT" (gönderme).
; F_00828C kanal numarasını $0376'dan okuduğu yerden (0x008332) JSRL ile gelinir, M=0.
; VFO modunda değilse oradaki iki komutu yapıp döner; VFO modundaysa 7 parçalı iki haneyi
; F_00828C'nin yerel değişkenine ($04, doğrudan sayfa) koyar ve hesaplamanın sonrasına
; (0x00835A) atlar. 7 parçalı ekranda V = 'U', R = 'r', T = 't'.
VFO_KNO:
        .ASSUME M=0,X=0
        PHX
        JSR   VFO_ETKIN
        BEQ   VN_VFO
        PLX
        LDA   A,$0376                   ; orijinal: LDA A,$0376 / SEM
        SEM
        RTL
VN_VFO: LDX   #0                        ; tablo: [cam A/B][alma/gönderme]
        SEM
        LDA   A,VFO_DURUM
        CLM
        BEQ   VN_1
        LDX   #2
VN_1:   BBS   #$0100,$023C,VN_2         ; cam B (bit 8 = 0): 7 parça bit düzeni farklı
        INX
        INX
        INX
        INX
VN_2:   LDA   A,>VN_TABLO,X
        STA   A,$04
        PLX
        SEM                             ; JSRL'nin dönüş adresini (3 bayt) at
        PLA
        CLM
        PLA
        JMPL  $00835A

; F_008209'un düzeni: cam A (ROM $2D62) onlar << 8 | birler, parçalar a8 b4 c2 d10 e20 f80 g40;
; cam B ($2D00) birler << 8 | onlar, a10 b20 c80 d8 e4 f2 g40
VN_TABLO:
        .WORD $B660                     ; A: U r
        .WORD $B6F0                     ; A: U t
        .WORD $44AE                     ; B: U r
        .WORD $4EAE                     ; B: U t

; ekran (M=0)
FR_EKRAN:
        .ASSUME M=0,X=0
        SEM
        BBC   #F_HATA_G,FR_DURUM,FE_GIRIS
        LDX   #0
        BBS   #$80,OPTIONS,FE_TR
FE_EN:  LDA   A,>TXT_FR_ERROR,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   FE_EN
        CLM
        RTS
FE_TR:  .ASSUME M=1,X=0
        LDA   A,>TXT_FR_HATA,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   FE_TR
        CLM
        RTS
FE_GIRIS:                               ; 2. adımda giriş boşken: "GONDERME" / "TX FREQ"
        .ASSUME M=1,X=0
        BBC   #F_TON,FR_DURUM,FE_FREK
        JMP   FE_TON
FE_FREK:
        BBC   #F_TX_ADIM,FR_DURUM,FE_HANE
        LDA   A,FR_NDIG
        BNE   FE_HANE
        LDX   #0
        BBS   #$80,OPTIONS,FE_TR2
FE_EN2: LDA   A,>TXT_FR_TX,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   FE_EN2
        CLM
        RTS
FE_TR2: .ASSUME M=1,X=0
        LDA   A,>TXT_FR_GONDER,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   FE_TR2
        CLM
        RTS
FE_HANE:
        .ASSUME M=1,X=0
        CLM
        ; düşer

; girilen haneleri "156 5250" biçiminde LCD'ye yaz; girilmeyen haneler '_' (alma frekansı)
; ya da '-' (gönderme frekansı) (M=0)
FR_METIN:
        .ASSUME M=0,X=0
        SEM
        LDX   #0                        ; X = hane, Y = LCD konumu
        LDY   #0
FM_DON: CPY   #3
        BNE   FM_1
        LDA   A,#' '
        STA   A,LCD,Y
        INY
FM_1:   TXA
        CMP   A,FR_NDIG
        BCS   FM_ALT
        LDA   A,FR_HANE,X
        BRA   FM_KOY
FM_ALT: LDA   A,#'_'
        BBC   #F_TX_ADIM,FR_DURUM,FM_KOY
        LDA   A,#'-'
FM_KOY: STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   FM_DON
        CLM
        RTS

; ton adımları: boşken "RX TONE" / "ALMA TON" ("TX TONE" / "GON TON"), hane varken "RT 885_"
FE_TON: .ASSUME M=1,X=0
        LDA   A,FR_NDIG
        BNE   FET_HANE
        LDX   #0                        ; metin: [RX/TX][EN/TR], 8'er bayt
        BBC   #F_TXTON,FR_DURUM,FET_1
        LDX   #16
FET_1:  BBC   #$80,OPTIONS,FET_2
        CLM
        TXA
        CLC
        ADC   A,#8
        TAX
        SEM
FET_2:  LDY   #0
FET_3:  LDA   A,>TXT_FR_TON,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   FET_3
        CLM
        RTS
FET_HANE:
        .ASSUME M=1,X=0
        LDA   A,#'R'
        BBC   #F_TXTON,FR_DURUM,FET_4
        LDA   A,#'T'
FET_4:  STA   A,LCD
        LDM   #'T',LCD+1
        LDM   #' ',LCD+2
        LDM   #' ',LCD+7
        LDX   #0
FET_5:  TXA
        CMP   A,FR_NDIG
        LDA   A,#'_'                    ; LDA C'yi değiştirmez
        BCS   FET_6
        LDA   A,FR_HANE,X
FET_6:  STA   A,LCD+3,X
        INX
        CPX   #4
        BNE   FET_5
        CLM
        RTS

TXT_FR_TON:
        .TEXT8 "RX TONE"
        .TEXT8 "ALMA TON"
        .TEXT8 "TX TONE"
        .TEXT8 "GON TON"

TXT_FR_ERROR:
        .TEXT8 "ERROR"
TXT_FR_HATA:
        .TEXT8 "HATA"
TXT_FR_TX:
        .TEXT8 "TX FREQ"
TXT_FR_GONDER:
        .TEXT8 "GONDERME"
