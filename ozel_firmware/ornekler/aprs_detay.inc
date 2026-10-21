; aprs_detay.inc - APRS alma: duyulan istasyonların konumu (AT V0-23)
;
; Listede (aprs_rx.inc) bir istasyon görünürken 's' sayfa değiştirir:
;   çağrı işareti -> enlem "4050 63N" -> boylam "3109 39E" -> mesaj "MERHABA " -> çağrı işareti ...
; Mesaj: konumdan sonraki yorum, durum (">") metni, mesaj (":ALICI    :") metni ya da Mic-E
; yorumu; ilk 8 karakter, büyük harf; LCD'de olmayan karakter boşluk. Yoksa "MESAJ -".
; (boşluk ondalık nokta, frekans ekranındaki gibi; boylam 100 ve üstündeyse "10109 3E").
; Konumu olmayan / çözülemeyen pakette "KONUM -". Sayfa 8 s görünür, sonra APRS ekranı.
;
; Konum paketin bilgi alanından alınır: düz biçim "!" / "=" (konum hemen), "/" / "@" (7 baytlık
; zaman damgasından sonra): "DDMM.mmN" + simge tablosu + "DDDMM.mmE" + simge. Konum belirsizliği
; için boşluklar 0 sayılır. Sıkıştırılmış konum ve Mic-E çözülmez ("KONUM -").
; Paket seri port tamponundan (AR_CER) okunur: ISR çerçeveyi tampona aldıysa AR_DETAY = 1.
;
; Kayıt (6 bayt): +0..+2 enlem, 0,01 dakika (20 bit), +2 bit 6 güney, bit 7 geçerli;
;                 +3..+5 boylam, 0,01 dakika (21 bit), +5 bit 7 batı.
; RAM: kapatılan özelliklerden boşalan yerde (ozel_firmware/README.md "Boşalan RAM"), parçalı:
;   konum kayıtları (6 bayt) DET_TABLO, metinler (8 bayt) MET_TABLO adreslerinde.
; Geçiciler: FREKANS girişinin değişkenleri (APRS'teyken kullanılmıyor): FR_HANE (eski kayıt +
; bulundu bayrağı), FR_BOLUNEN.. (yeni kayıt / bölme).

AR_DETAY    .EQU $04FE          ; 1 = AR_CER'deki çerçeve AR_YENI_ADR'deki istasyonun (ISR kurar)
AR_SAYFA    .EQU $04FF          ; 0 çağrı işareti, 1 enlem, 2 boylam
DET_YENI    .EQU FR_BOLUNEN     ; 6 bayt: bu paketten çözülen kayıt (FR_BOLUNEN + FR_BOLEN)
DET_ESKI    .EQU FR_HANE        ; 6 bayt: bulunan istasyonun eski kaydı
DET_BULUNDU .EQU FR_HANE+6
DET_SURE    .EQU 250            ; 250 x 32 ms = 8 s
DET_RAM     .EQU $0100          ; kayıt / metin adresleri X = adres - $0100 ile (mutlak, D'den bağımsız)
DET_MYENI   .EQU FR_FU          ; 8 bayt: bu paketin metni (FR_FU, FR_KELIME, FR_D)
DET_MESKI   .EQU FR_KALAN       ; 8 bayt: bulunan istasyonun eski metni (FR_KALAN, FR_T)
DET_MVAR    .EQU FR_KAYIT       ; 1 = DET_MYENI'de metin var

; ---------------------------------------------------------------------------------------------
; AR_ISLE'den, yeni adres listeye girmeden önce (M = 1): DET_YENI'yi doldur.
DET_HAZIRLA:
        .ASSUME M=1,X=0
        LDM   #0,DET_YENI+2             ; geçersiz
        LDM   #0,DET_MVAR
        LDA   A,AR_DETAY
        BEQ   DH_SON
        LDM   #0,AR_DETAY
        JSR   DET_COZ
DH_SON: RTS
DC_Y1:  JMP   DC_YOK                    ; uzak dallanmalar için

; AR_CER (AR_Y_N bayt) -> DET_YENI (M = 1). Bozuksa DET_YENI+2 bit 7 = 0 kalır.
DET_COZ:
        .ASSUME M=1,X=0
        LDX   #0                        ; adres alanı: son adres biti (SSID baytı bit 0)
DC_ADR: CLM
        TXA
        CLC
        ADC   A,#7
        TAX                             ; X = sonraki adresin başı
        SEM
        TXA                             ; X <= AR_Y_N olmalı (AR_Y_N <= 160)
        CMP   A,AR_Y_N
        BEQ   DC_A1
        BCS   DC_Y1
DC_A1:  LDA   A,AR_CER-1,X              ; bu adresin SSID baytı
        AND   A,#$01
        BEQ   DC_ADR
        INX                             ; denetim, PID
        INX
        TXA
        CMP   A,AR_Y_N
        BCS   DC_Y1
        LDA   A,AR_CER,X                ; veri tipi
        INX
        CMP   A,#'!'
        BEQ   DC_KONUM
        CMP   A,#'='
        BEQ   DC_KONUM
        CMP   A,#'/'
        BEQ   DC_ZAMAN
        CMP   A,#'@'
        BEQ   DC_ZAMAN
        CMP   A,#'>'                    ; durum: metin hemen
        BEQ   DC_METIN
        LDY   #10                       ; mesaj ":ALICI    :metin"
        CMP   A,#':'
        BEQ   DC_ATLA
        LDY   #8                        ; Mic-E: 8 bayt boylam / hız / simge, sonra yorum
        CMP   A,#$60
        BEQ   DC_ATLA
        CMP   A,#$27
        BNE   DC_Y1
DC_ATLA:
        INX
        DEY
        BNE   DC_ATLA
DC_METIN:
        JMP   DET_METIN_AL              ; konum yok (DET_YENI+2 = 0), yalnız metin
DC_ZAMAN:
        CLM
        TXA
        CLC
        ADC   A,#7                      ; "DDHHMMz"
        TAX
        SEM
DC_KONUM:                               ; X = enlemin başı; 19 bayt gerekli
        CLM
        TXA
        CLC
        ADC   A,#19
        SEM
        CMP   A,AR_Y_N                  ; (AR_Y_N <= 160 < 256: alt bayt yeter)
        BEQ   DC_1
        BCS   DC_Y1
DC_1:   LDA   A,AR_CER+4,X              ; "DDMM.mmN"
        CMP   A,#'.'
        BNE   DC_Y1
        JSR   DC_IKI                    ; DD
        BCS   DC_Y1
        STA   A,FR_T                    ; derece
        INX
        INX
        JSR   DC_IKI                    ; MM
        BCC   UZ_119
        JMP   DC_Y1
UZ_119:
        STA   A,FR_T+1
        INX
        INX
        INX                             ; '.'
        JSR   DC_IKI                    ; mm
        BCC   UZ_127
        JMP   DC_Y1
UZ_127:
        STA   A,FR_T+2
        INX
        INX
        JSR   DC_DEGER                  ; -> DET_YENI+0..+2
        LDA   A,AR_CER,X                ; yarımküre
        INX
        CMP   A,#'N'
        BEQ   DC_2
        CMP   A,#'S'
        BEQ   DC_GUNEY
        JMP   DC_YOK
DC_GUNEY:
        SEB   #$40,DET_YENI+2
DC_2:   LDA   A,DET_YENI+2              ; geçici olarak boylama taşı
        PHA
        LDA   A,DET_YENI+1
        PHA
        LDA   A,DET_YENI
        PHA
        INX                             ; simge tablosu
        LDA   A,AR_CER+5,X              ; "DDDMM.mmE"
        CMP   A,#'.'
        BNE   DC_YOK3
        LDA   A,AR_CER,X                ; yüzler hanesi
        JSR   DC_HANE
        BCS   DC_YOK3
        STA   A,FR_KALAN                ; (DC_IKI FR_T+3'ü kullanıyor)
        INX
        JSR   DC_IKI                    ; onlar + birler
        BCS   DC_YOK3
        STA   A,FR_T
        LDA   A,FR_KALAN                ; derece = 100 * yüzler + iki hane
        BEQ   DC_3
        LDA   A,FR_T
        CLC
        ADC   A,#100
        STA   A,FR_T
DC_3:   INX
        INX
        JSR   DC_IKI                    ; MM
        BCS   DC_YOK3
        STA   A,FR_T+1
        INX
        INX
        INX
        JSR   DC_IKI                    ; mm
        BCS   DC_YOK3
        STA   A,FR_T+2
        INX
        INX
        JSR   DC_DEGER
        LDA   A,DET_YENI                ; boylam +3..+5
        STA   A,DET_YENI+3
        LDA   A,DET_YENI+1
        STA   A,DET_YENI+4
        LDA   A,DET_YENI+2
        STA   A,DET_YENI+5
        PLA                             ; enlem geri
        STA   A,DET_YENI
        PLA
        STA   A,DET_YENI+1
        PLA
        STA   A,DET_YENI+2
        LDA   A,AR_CER,X
        CMP   A,#'E'
        BEQ   DC_4
        CMP   A,#'W'
        BNE   DC_YOK
        SEB   #$80,DET_YENI+5
DC_4:   SEB   #$80,DET_YENI+2           ; geçerli
        INX                             ; yarımküre, simge kodu: sonra yorum
        INX
        JMP   DET_METIN_AL
DC_YOK3:
        PLA
        PLA
        PLA
DC_YOK: LDM   #0,DET_YENI+2
        RTS

; AR_CER[X], AR_CER[X+1] iki hane -> A (0..99), C = 1 bozuk (M = 1, X korunur)
DC_IKI:
        .ASSUME M=1,X=0
        LDA   A,AR_CER,X
        JSR   DC_HANE
        BCS   DCI_SON
        STA   A,FR_T+3
        ASL   A                         ; 10 * a = 8a + 2a
        ASL   A
        ASL   A
        CLC
        ADC   A,FR_T+3
        ADC   A,FR_T+3
        STA   A,FR_T+3
        LDA   A,AR_CER+1,X
        JSR   DC_HANE
        BCS   DCI_SON
        CLC
        ADC   A,FR_T+3
        CLC
DCI_SON:
        RTS

; A = '0'..'9' -> 0..9, ' ' -> 0; değilse C = 1 (M = 1)
DC_HANE:
        .ASSUME M=1,X=0
        CMP   A,#' '
        BEQ   DH_0
        SEC
        SBC   A,#'0'
        BCC   DH_BOZUK
        CMP   A,#10
        BCS   DH_BOZUK
        CLC
        RTS
DH_0:   LDA   A,#0
        CLC
        RTS
DH_BOZUK:
        SEC
        RTS

; FR_T = derece, FR_T+1 = dakika, FR_T+2 = yüzde dakika -> DET_YENI+0..+2 = 0,01 dakika
; (derece * 6000 + dakika * 100 + yüzde; M = 1 giriş ve çıkış, X korunur)
DC_DEGER:
        .ASSUME M=1,X=0
        CLM
        PHX
        LDA   A,FR_T
        AND   A,#$00FF
        MPY   #6000                     ; B:A = derece * 6000
        STA   A,DET_YENI
        XAB                             ; üst kelime
        SEM
        STA   A,DET_YENI+2
        CLM
        LDA   A,FR_T+1
        AND   A,#$00FF
        MPY   #100
        CLC
        ADC   A,DET_YENI
        STA   A,DET_YENI
        SEM
        LDA   A,DET_YENI+2
        ADC   A,#0
        STA   A,DET_YENI+2
        CLM
        LDA   A,FR_T+2
        AND   A,#$00FF
        CLC
        ADC   A,DET_YENI
        STA   A,DET_YENI
        SEM
        LDA   A,DET_YENI+2
        ADC   A,#0
        STA   A,DET_YENI+2
        PLX
        RTS

; ---------------------------------------------------------------------------------------------
; AR_LISTEYE'den, kaydırmadan hemen önce (M = 1, X korunur): AR_T = kayacak öğe sayısı
; (bulunan istasyonun sırası ya da yeni / düşen son öğe). Kayıtlar listeyle aynı kayar;
; başa DET_YENI (geçerliyse) ya da bulunan istasyonun eski kaydı konur.
DET_KAYDIR:
        .ASSUME M=1,X=0
        PHX
        LDA   A,AR_T
        PHA                             ; metinler için yeniden
        LDA   A,AR_T                    ; eski kayıt
        JSR   DET_X
        LDY   #0
DK_1:   LDA   A,DET_RAM,X
        STA   A,DET_ESKI,Y
        INX
        INY
        CPY   #6
        BNE   DK_1
DK_KAYDIR:
        LDA   A,AR_T                    ; j = AR_T .. 1: kayıt[j] = kayıt[j - 1]
        BEQ   DK_BAS
        JSR   DET_X
        TXY                             ; Y = hedef
        DEC   AR_T
        LDA   A,AR_T
        JSR   DET_X                     ; X = kaynak
        LDA   A,#6
        STA   A,FR_T+3
DK_2:   LDA   A,DET_RAM,X
        STA   A,DET_RAM,Y
        INX
        INY
        DEC   FR_T+3
        BNE   DK_2
        BRA   DK_KAYDIR
DK_BAS: LDA   A,#0                      ; kayıt[0]
        JSR   DET_X
        LDY   #0
        BBS   #$80,DET_YENI+2,DK_YENI
        LDA   A,DET_BULUNDU
        BNE   DK_ESKI
        LDM   #0,DET_RAM+2,X            ; yeni istasyon, konum yok
        BRA   DK_SON
DK_ESKI:
        LDA   A,DET_ESKI,Y
        STA   A,DET_RAM,X
        INX
        INY
        CPY   #6
        BNE   DK_ESKI
        BRA   DK_SON
DK_YENI:
        LDA   A,DET_YENI,Y
        STA   A,DET_RAM,X
        INX
        INY
        CPY   #6
        BNE   DK_YENI
DK_SON: PLA
        STA   A,AR_T
        JSR   MET_KAYDIR
        PLX
        RTS
        ; (AR_T burada 0'a iner; AR_LISTEYE kaydırmayı X'le yapar, AR_T'yi yeniden kullanmaz)

; Metinler de aynı kayar (M = 1). Başa: bu paketin metni; yoksa bulunan istasyonun eskisi;
; yoksa boş (ilk bayt 0).
MET_KAYDIR:
        .ASSUME M=1,X=0
        LDA   A,AR_T                    ; eski metin
        JSR   MET_X
        LDY   #0
MK_1:   LDA   A,DET_RAM,X
        STA   A,DET_MESKI,Y
        INX
        INY
        CPY   #8
        BNE   MK_1
MK_KAYDIR:
        LDA   A,AR_T
        BEQ   MK_BAS
        JSR   MET_X
        TXY
        DEC   AR_T
        LDA   A,AR_T
        JSR   MET_X
        LDA   A,#8
        STA   A,FR_DP                   ; sayaç (DET_MYENI ve DET_MESKI dışında)
MK_2:   LDA   A,DET_RAM,X
        STA   A,DET_RAM,Y
        INX
        INY
        DEC   FR_DP
        BNE   MK_2
        BRA   MK_KAYDIR
MK_BAS: LDA   A,#0
        JSR   MET_X
        LDY   #0
        LDA   A,DET_MVAR
        BNE   MK_YENI
        LDA   A,DET_BULUNDU
        BNE   MK_ESKI
        LDM   #0,DET_RAM,X              ; boş
        RTS
MK_ESKI:
        LDA   A,DET_MESKI,Y
        STA   A,DET_RAM,X
        INX
        INY
        CPY   #8
        BNE   MK_ESKI
        RTS
MK_YENI:
        LDA   A,DET_MYENI,Y
        STA   A,DET_RAM,X
        INX
        INY
        CPY   #8
        BNE   MK_YENI
        RTS

; AR_CER[X..] metni -> DET_MYENI (8 karakter, boşlukla doldurulur; M = 1). Baştaki boşluklar
; (ve gösterilemeyen karakterler) atlanır; küçük harf büyür; LCD'de olmayan karakter boşluk. En az bir karakter: DET_MVAR = 1.
DET_METIN_AL:
        .ASSUME M=1,X=0
        LDY   #0
DMA_BOS:
        TXA
        CMP   A,AR_Y_N
        BCS   DMA_DOLDUR
        LDA   A,AR_CER,X                ; baştaki gösterilemeyenler de (Mic-E üretici işareti ...)
        JSR   DMA_HARF
        CMP   A,#' '
        BNE   DMA_K
        INX
        BRA   DMA_BOS
DMA_K:  TXA
        CMP   A,AR_Y_N
        BCS   DMA_DOLDUR
        LDA   A,AR_CER,X
        JSR   DMA_HARF
        STA   A,DET_MYENI,Y
        LDM   #1,DET_MVAR
        INX
        INY
        CPY   #8
        BNE   DMA_K
        RTS
DMA_DOLDUR:
        CPY   #8
        BEQ   DMA_SON
        LDA   A,#' '
        STA   A,DET_MYENI,Y
        INY
        BRA   DMA_DOLDUR
DMA_SON:
        RTS

; A: a-z -> A-Z; A-Z 0-9 boşluk - / _ kalır; diğerleri boşluk (M = 1)
DMA_HARF:
        .ASSUME M=1,X=0
        CMP   A,#'a'
        BCC   DH_1
        CMP   A,#'z'+1
        BCS   DH_BOSLUK
        SEC
        SBC   A,#$20
        RTS
DH_1:   CMP   A,#'A'
        BCC   DH_2
        CMP   A,#'Z'+1
        BCC   DH_TAMAM
        CMP   A,#'_'
        BEQ   DH_TAMAM
        BRA   DH_BOSLUK
DH_2:   CMP   A,#'0'
        BCC   DH_3
        CMP   A,#'9'+1
        BCC   DH_TAMAM
        BRA   DH_BOSLUK
DH_3:   CMP   A,#'-'
        BEQ   DH_TAMAM
        CMP   A,#'/'
        BEQ   DH_TAMAM
DH_BOSLUK:
        LDA   A,#' '
DH_TAMAM:
        RTS

; A = sıra 0..7 -> X = metin adresi - $0100 (M = 1)
MET_X:
        .ASSUME M=1,X=0
        CLM
        AND   A,#$0007
        ASL   A
        TAX
        LDA   A,>MET_TABLO,X
        TAX
        SEM
        RTS
MET_TABLO:
        .WORD $02AB-DET_RAM, $02B3-DET_RAM, $02BB-DET_RAM, $02C3-DET_RAM
        .WORD $0285-DET_RAM, $0264-DET_RAM, $0538-DET_RAM, $0598-DET_RAM

; A = kayıt sırası 0..7 -> X = kayıt adresi - $0200 (M = 1)
DET_X:
        .ASSUME M=1,X=0
        CLM
        AND   A,#$0007
        ASL   A
        TAX
        LDA   A,>DET_TABLO,X
        TAX
        SEM
        RTS
DET_TABLO:
        .WORD $02CB-DET_RAM, $028D-DET_RAM, $0123-DET_RAM, $0129-DET_RAM
        .WORD $03C8-DET_RAM, $03CE-DET_RAM, $026D-DET_RAM, $03A2-DET_RAM

; ---------------------------------------------------------------------------------------------
; AR_TUS'tan 's' (M = 1): görünen istasyonun sonraki sayfası. C = 1 (ekran yazıldı), M = 0.
DET_TUS:
        .ASSUME M=1,X=0
        LDA   A,AR_GOSTER               ; istasyon gösterilmiyorsa APRS ekranı
        BEQ   DT_APRSJ
        LDA   A,AR_LN
        BNE   DT_0
DT_APRSJ:
        JMP   DT_APRS
DT_0:
        LDA   A,AR_SAYFA
        INC   A
        CMP   A,#4
        BCC   DT_1
        LDA   A,#0
DT_1:   STA   A,AR_SAYFA
        LDA   A,AR_SEC                  ; gösterilen: 0 = yeni gelen (listenin başı)
        BEQ   DT_2
        DEC   A
DT_2:   STA   A,AR_T
        LDA   A,AR_SAYFA
        BNE   DT_KONUM
        LDA   A,AR_T                    ; sayfa 0: çağrı işareti
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
        JMP   DT_SURE
DT_KONUM:
        LDA   A,AR_SAYFA
        CMP   A,#3
        BNE   DT_K1
        LDA   A,AR_T                    ; sayfa 3: mesaj metni
        JSR   MET_X
        LDA   A,DET_RAM,X
        BEQ   DT_MYOK
        LDY   #0
DT_M1:  LDA   A,DET_RAM,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   DT_M1
        JMP   DT_SURE
DT_MYOK:
        LDX   #0
DT_M2:  LDA   A,>DET_MYOK_METIN,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   DT_M2
        JMP   DT_SURE
DT_K1:  LDA   A,AR_T
        JSR   DET_X
        LDA   A,DET_RAM+2,X
        BMI   DT_3
        LDX   #0                        ; "KONUM -"
DT_YOKY:
        LDA   A,>DET_YOK_METIN,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   DT_YOKY
        JMP   DT_SURE
DT_3:   LDA   A,AR_SAYFA
        CMP   A,#2
        BEQ   DT_BOYLAM
        LDA   A,DET_RAM+2,X             ; enlem: 20 bit
        AND   A,#$0F
        STA   A,FR_BOLUNEN+2
        LDA   A,DET_RAM+2,X
        AND   A,#$40
        BEQ   DT_4N
        LDA   A,#'S'
        BRA   DT_4
DT_4N:  LDA   A,#'N'
DT_4:   STA   A,LCD+7
        BRA   DT_YAZ
DT_BOYLAM:
        LDA   A,DET_RAM+5,X             ; boylam: 21 bit
        AND   A,#$1F
        STA   A,FR_BOLUNEN+2
        LDA   A,DET_RAM+5,X
        BMI   DT_5W
        LDA   A,#'E'
        BRA   DT_5
DT_5W:  LDA   A,#'W'
DT_5:   STA   A,LCD+7
        INX                             ; +3: alt kelime aşağıda X+0 / X+1
        INX
        INX
DT_YAZ: LDA   A,DET_RAM,X
        STA   A,FR_BOLUNEN
        LDA   A,DET_RAM+1,X
        STA   A,FR_BOLUNEN+1
        LDM   #0,FR_BOLUNEN+3
        CLM
        LDA   A,#6000                   ; derece, kalan
        JSR   FR_BOL
        LDA   A,FR_KALAN
        STA   A,FR_D                    ; dakika * 100 + yüzde (< 6000)
        LDA   A,FR_BOLUNEN
        SEM
        STA   A,FR_T                    ; derece (<= 180)
        LDM   #' ',LCD+4
        CMP   A,#100
        BCS   DT_UC
        LDX   #0                        ; "DDMM mmH"
        JSR   DT_IKI_YAZ
        JSR   DT_DAKIKA                 ; A = dakika, FR_KALAN = yüzde
        LDX   #2
        JSR   DT_IKI_YAZ
        LDA   A,FR_KALAN
        LDX   #5
        JSR   DT_IKI_YAZ
        JMP   DT_SURE
DT_UC:  LDM   #'1',LCD                  ; "DDDMM mH" (derece 100..180)
        SEC
        SBC   A,#100
        LDX   #1
        JSR   DT_IKI_YAZ
        JSR   DT_DAKIKA
        LDX   #3
        JSR   DT_IKI_YAZ
        LDM   #' ',LCD+5
        LDA   A,FR_KALAN                ; yüzde -> onda bir hane
        LDA   B,#0
        DIV   #10
        CLC
        ADC   A,#'0'
        STA   A,LCD+6
DT_SURE:
        LDM   #DET_SURE,AR_GOSTER
        CLM
        SEC
        RTS
DT_APRS:
        .ASSUME M=1,X=0
        LDM   #0,AR_SAYFA
        CLM
        JSR   AP_EKRAN
        SEC
        RTS

; FR_D = dakika * 100 + yüzde -> A = dakika, FR_KALAN = yüzde (M = 1)
DT_DAKIKA:
        .ASSUME M=1,X=0
        CLM
        LDA   A,FR_D
        STA   A,FR_BOLUNEN
        LDM   #0,FR_BOLUNEN+2
        LDA   A,#100
        JSR   FR_BOL
        LDA   A,FR_BOLUNEN
        SEM
        RTS

; A = 0..99 -> LCD[X], LCD[X+1] iki hane (M = 1)
DT_IKI_YAZ:
        .ASSUME M=1,X=0
        LDA   B,#0
        DIV   #10
        CLC
        ADC   A,#'0'
        STA   A,LCD,X
        CLC
        ADC   B,#'0'
        STA   B,LCD+1,X
        RTS

DET_YOK_METIN:
        .TEXT8 "KONUM -"
DET_MYOK_METIN:
        .TEXT8 "MESAJ -"
