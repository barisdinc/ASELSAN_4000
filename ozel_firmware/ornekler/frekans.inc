; frekans.inc - FREKANS menü öğesi: tuş takımından frekans girip telsizi ona ayarlama (VFO)
;
;   ... m (FREKANS) s   -> "___ ____"
;   0-9      frekans, noktasız: 156525 = 156,525 MHz; 12,5 kHz adım için 7 hane: 1565125
;            ekranda "156 5125" (3. haneden sonra boşluk = ondalık nokta)
;   *        son haneyi sil
;   #        ayarla: RX ve TX (simplex) bu frekansa; onay sesi; normal ekranda frekans görünür
;   s        girişi temizle
;   m        çık (telsiz yeni frekansta kalır; kanal anahtarı çevrilince kanala döner)
; Hatalar ("ERROR" / "HATA" + bip): 6 haneden az, 2,5 kHz ızgarasında değil, bandın dışında,
; telsiz göndermedeyken. Hatadan sonraki tuş girişe döner.
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

FR_HANE     .EQU OZEL_RAM+$40   ; 7 bayt girilen haneler (ASCII)
FR_NDIG     .EQU OZEL_RAM+$47
FR_DURUM    .EQU OZEL_RAM+$48   ; bit 0: hata gösteriliyor
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

F_HATA_G    .EQU 1

SYN_KAYIT   .EQU $0370          ; geçerli kanalın sentezör kaydı (13 bayt)
SYN_YEDEK   .EQU $0363          ; kopyası
RADYO_DURUM .EQU $0228          ; 1 = RX'e gir (sentezörü yükle), 2..3 RX, 4.. TX tarafı
KANAL_ADI   .EQU $024A          ; normal ekranda gösterilen 8 karakter
BAND_TABAN  .EQU $0224
RX_KAYMA    .EQU $01F9
ON_BOLUCU   .EQU $020B
FREF        .EQU $01F5
TERS_TABLO  .EQU $002B06        ; bit sırası ters çevirme tablosu (256 bayt)

; ---------------------------------------------------------------------------------------------
FREKANS_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        LDM   #FREKANS_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(FREKANS_TUS),HANDLER_BK
        LDM   #0,FR_NDIG
        LDM   #0,FR_DURUM
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
        BEQ   FK_SON
        DEC   FR_NDIG
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
        LDA   A,FR_T
        STA   A,FR_HANE,X
        INC   FR_NDIG
        BRA   FK_SON

; ---- ayarla
FK_AYARLA:
        .ASSUME M=1,X=0
        LDA   A,FR_NDIG
        CMP   A,#6
        BCC   FK_HATA
        LDA   A,RADYO_DURUM             ; göndermedeyken değiştirme
        CMP   A,#4
        BCS   FK_HATA
        CLM
        JSR   FR_HESAPLA                ; C = 1: geçersiz frekans
        BCS   FK_HATA
        SEM                             ; 6 hane girildiyse 7. hane 0
        LDA   A,FR_NDIG
        CMP   A,#7
        BEQ   FA_1
        LDM   #'0',FR_HANE+6
        LDM   #7,FR_NDIG
FA_1:   CLM
        JSR   FR_YAZ                    ; sentezör kaydı, kanal adı, RX'i yeniden yükle
        LDY   #3                        ; onay: 2 x 1000 Hz
        JSRL  F_BEEP
        JMP   FK_SON

; ---------------------------------------------------------------------------------------------
; Alt yordamlar (JSR / RTS, M=0 giriş ve çıkış). Firmware fonksiyonu çağırmazlar.

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

; RX ve TX sentezör baytlarını kayda yaz, kopyala, kanal adını ayarla, RX'i yeniden yükle
FR_YAZ:
        .ASSUME M=0,X=0
        LDA   A,RX_KAYMA
        JSR   FR_SENTEZ
        LDX   #0
        SEM
FY_RX:  LDA   A,FR_KAYIT,X
        STA   A,SYN_KAYIT,X             ; $0370..$0374
        INX
        CPX   #5
        BNE   FY_RX
        CLM
        LDA   A,#0                      ; TX: kayma yok (simplex)
        JSR   FR_SENTEZ
        LDX   #0
        SEM
FY_TX:  LDA   A,FR_KAYIT,X
        STA   A,SYN_KAYIT+7,X           ; $0377..$037B
        INX
        CPX   #5
        BNE   FY_TX
        LDX   #0                        ; kaydın kopyası ($0363) ve kanal adı
FY_KOP: LDA   A,SYN_KAYIT,X
        STA   A,SYN_YEDEK,X
        INX
        CPX   #13
        BNE   FY_KOP
        CLM
        JSR   FR_METIN
        LDX   #0
        SEM
FY_AD:  LDA   A,LCD,X
        STA   A,KANAL_ADI,X
        INX
        CPX   #8
        BNE   FY_AD
        LDM   #1,RADYO_DURUM            ; ana döngü RX'e yeniden girer, sentezörü yükler
        CLM
        RTS

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
FE_GIRIS:
        CLM
        ; düşer

; girilen haneleri "156 5250" biçiminde LCD'ye yaz; girilmeyen haneler '_' (M=0)
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
FM_KOY: STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   FM_DON
        CLM
        RTS

TXT_FR_ERROR:
        .TEXT8 "ERROR"
TXT_FR_HATA:
        .TEXT8 "HATA"
