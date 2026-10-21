; aprs_digi.inc - APRS digipeater (APRS DG açıkken, APRS ekranında) (AT V0-25)
;
; Duyulan paketin yolundaki ilk tekrarlanmamış (H biti 0) adrese bakılır:
;   ÇAĞRIM          -> ÇAĞRIM*                 (doğrudan bize)
;   WIDE1-1         -> ÇAĞRIM*                 (fill-in)
;   WIDE2-2         -> ÇAĞRIM*,WIDE2-1         (araya kendimizi koyup N azaltılır)
;   WIDE2-1         -> ÇAĞRIM*,WIDE2*
; Tekrarlanmaz: kendi paketimiz, yolda zaten ÇAĞRIM* varsa, başka yol (WIDE3-3, WIDE2-3, TRACE ...),
; yol dolu (8 tekrarlayıcı) ya da paket çok uzunsa, aynı paket son 30 s'de tekrarlandıysa
; (anahtar: hedef + gönderen + bilgi alanının CRC'si; yol hesaba girmez).
; Gönderim tracker'ın PTT isteğiyle (F_PTT_APRS) yapılır; ~3 s'de verici açılmazsa bırakılır.
;
; Tampon (AR_CER = AP_CER) durumu AR_Y_DURUM: kesme geçerli paketi aldığında IG ya da DG açıksa 8
; (kilitli, karar ana döngüde: DG_ISLE). IG açıksa önce paket olduğu gibi seri porta yazılır
; (1..5), sonra digi; değilse doğrudan 7 (digi gönderimi bekliyor). Kendi gönderimimiz (PTT /
; tracker) bekleyen digiyi iptal eder.
; Çağrı işareti EEPROM'daki APRS bölümünden (AP_ALAN_AD + AP_CAGRI_YAZ) tamponun sonuna (DG_MY)
; yazılır; uzun paket (> DG_N_EN_COK) tekrarlanmaz.
; RAM (boşalan parçalar): DIGI_ISTE $012F, DG_SURE $0130, DG_MS $039C, yaşlar $0399..$039B,
; CRC'ler $03AB..$03B0. Geçiciler: FR_HANE (çağrı), FR_D (CRC), FR_DP / FR_KOD.

DIGI_ISTE   .EQU $012F          ; 1 = bu paket tekrarlanacak (IG yazdıktan sonra)
DG_SURE     .EQU $0130          ; PTT isteğinin kalan süresi (32 ms)
DG_MS       .EQU $039C          ; saniye için 32 ms sayacı
DG_YAS      .EQU $0399          ; 3 bayt: tekrar kaydının kalan süresi (s), 0 = boş
DG_CRCK     .EQU $03AB          ; 3 x 2 bayt
DG_MY       .EQU AR_CER_N-8     ; tamponda çağrımızın geçici yeri (152..158)
DG_N_EN_COK .EQU AR_CER_N-9     ; tekrarlanacak paket en çok 151 bayt (+7 + FCS 2 = 160)
DG_ESIK     .EQU 30             ; s
DG_CAGRI    .EQU FR_HANE        ; 7 bayt: çağrımız (kaydırma sırasında)
DG_CRC      .EQU FR_D           ; 2 bayt
DG_EYLEM    .EQU FR_DP          ; 0 bize, 1 WIDE1-1, 2 WIDE2-N
DG_GIRDI    .EQU FR_DP+1        ; ilk tekrarlanmamış adresin yeri
DG_BILGI    .EQU FR_KOD         ; bilgi alanının başı
DG_SAY      .EQU FR_KOD+1       ; tekrarlayıcı sayısı
SSID_MASKE  .EQU $1E

; ---------------------------------------------------------------------------------------------
; AR_ISLE'den, yeni adres listeye girdikten sonra (M = 1; AR_D = çerçeve)
DG_ISLE:
        .ASSUME M=1,X=0
        LDA   A,AR_Y_DURUM
        CMP   A,#8
        BNE   DGI_SON
        LDM   #0,DIGI_ISTE
        BBC   #AYAR_DG,AP_AYAR,DGI_1
        LDA   A,RADYO_DURUM             ; alıyorken
        CMP   A,#4
        BCS   DGI_1
        JSR   DG_KARAR
        BCC   DGI_1
        LDM   #1,DIGI_ISTE
DGI_1:  BBC   #AYAR_IG,AP_AYAR,DGI_2
        LDM   #1,AR_Y_DURUM             ; önce seri port (aprs_seri.inc), bitince DG_BASLAT
        RTS
DGI_2:  LDA   A,DIGI_ISTE
        BEQ   DGI_3
        JMP   DG_BASLAT
DGI_3:  LDM   #0,AR_Y_DURUM
DGI_SON:
        RTS

; Tekrarlanacaksa çerçeveyi düzenle, FCS ekle, PTT iste; değilse tamponu bırak (M = 1)
DG_BASLAT:
        .ASSUME M=1,X=0
        LDM   #0,DIGI_ISTE
        JSR   DG_KARAR                  ; (tampon değişmedi; çağrıyı da yeniden yazar)
        BCS   DGB_1
        LDM   #0,AR_Y_DURUM
        RTS
DGB_1:  JSR   DG_DUZENLE
        JSR   DG_KAYDET
        LDM   #7,AR_Y_DURUM             ; APRS_PTT gönderir
        LDM   #94,DG_SURE               ; ~3 s
        CLM
        SEB   #F_PTT_APRS,PTT_ISTEK
        SEM
        RTS

; Her turda (AR_ISLE'den, M = 1): tekrar kayıtlarının yaşı; PTT isteğinin süresi
DG_ZAMAN:
        .ASSUME M=1,X=0
        LDA   A,DG_MS
        CLC
        ADC   A,AP_FARK
        STA   A,DG_MS
DGZ_1:  LDA   A,DG_MS
        CMP   A,#31                     ; ~1 s
        BCC   DGZ_3
        SBC   A,#31
        STA   A,DG_MS
        LDX   #0
DGZ_2:  LDA   A,DG_YAS,X
        BEQ   DGZ_2B
        DEC   A
        STA   A,DG_YAS,X
DGZ_2B: INX
        CPX   #3
        BNE   DGZ_2
        BRA   DGZ_1
DGZ_3:  LDA   A,AR_Y_DURUM
        CMP   A,#7
        BNE   DGZ_SON
        LDA   A,DG_SURE
        SEC
        SBC   A,AP_FARK
        BCS   DGZ_4
        LDA   A,#0
DGZ_4:  STA   A,DG_SURE
        BNE   DGZ_SON
        LDM   #0,AR_Y_DURUM             ; verici açılmadı: bırak
        LDA   A,AP_OTO                  ; tracker de beklemiyorsa PTT isteğini kaldır
        BNE   DGZ_SON
        CLM
        CLB   #F_PTT_APRS,PTT_ISTEK
        SEM
DGZ_SON:
        RTS

; ---------------------------------------------------------------------------------------------
; Karar (M = 1). C = 1: tekrarla (DG_EYLEM, DG_GIRDI, DG_CRC hazır); çağrımız tamponda DG_MY'de.
DG_KARAR:
        .ASSUME M=1,X=0
        LDA   A,AR_Y_N
        CMP   A,#DG_N_EN_COK+1
        BCC   DK_N
        JMP   DK_HAYIR
DK_N:   CLM                             ; çağrımız -> AP_CER+DG_MY (EEPROM, AP_F = bu çerçeve)
        LDA   A,AP_F
        PHA
        LDA   A,AR_D
        STA   A,AP_F
        LDA   A,#AB_CAGRI
        LDY   #10
        JSR   AP_ALAN_AD
        PLA
        STA   A,AP_F
        SEM
        LDX   #DG_MY
        JSR   AP_CAGRI_YAZ
        BCC   DK_C
        JMP   DK_HAYIR
DK_C:   LDX   #7                        ; kendi paketimiz mi
        JSR   DG_BENIM
        BCC   DK_Y
        JMP   DK_HAYIR
DK_Y:   LDM   #$FF,DG_GIRDI             ; yol
        LDM   #0,DG_SAY
        LDX   #7
DK_YOL: LDA   A,AR_CER+6,X              ; önceki adres son mu
        AND   A,#$01
        BNE   DK_YSON
        CLM
        TXA
        CLC
        ADC   A,#7
        TAX
        SEM
        TXA                             ; X + 7 <= N
        CLC
        ADC   A,#7
        CMP   A,AR_Y_N
        BEQ   DK_Y1
        BCC   DK_Y1
        JMP   DK_HAYIR
DK_Y1:  INC   DG_SAY
        JSR   DG_BENIM
        BCC   DK_Y2
        LDA   A,AR_CER+6,X              ; bizimki: zaten tekrarlandıysa hayır, değilse bu
        BPL   DK_Y2
        JMP   DK_HAYIR
DK_Y2:  LDA   A,DG_GIRDI
        CMP   A,#$FF
        BNE   DK_YOL
        LDA   A,AR_CER+6,X
        BMI   DK_YOL
        TXA
        STA   A,DG_GIRDI
        BRA   DK_YOL
DK_YSON:
        TXA                             ; bilgi alanı = son adres + 7 + denetim + PID
        CLC
        ADC   A,#9
        STA   A,DG_BILGI
        LDA   A,DG_GIRDI
        CMP   A,#$FF
        BNE   DK_G
        JMP   DK_HAYIR
DK_G:   CLM
        AND   A,#$00FF
        TAX
        SEM
        JSR   DG_BENIM                  ; doğrudan bize
        BCC   DK_W
        LDM   #0,DG_EYLEM
        BRA   DK_TEKRAR
DK_W:   LDA   A,AR_CER,X                ; "WIDE" + rakam + boşluk
        CMP   A,#'W'*2
        BNE   DK_HAYIRJ
        LDA   A,AR_CER+1,X
        CMP   A,#'I'*2
        BNE   DK_HAYIRJ
        LDA   A,AR_CER+2,X
        CMP   A,#'D'*2
        BNE   DK_HAYIRJ
        LDA   A,AR_CER+3,X
        CMP   A,#'E'*2
        BNE   DK_HAYIRJ
        LDA   A,AR_CER+5,X              ; 6. karakter boşluk
        CMP   A,#' '*2
        BNE   DK_HAYIRJ
        LDA   A,AR_CER+6,X              ; SSID (N)
        LSR   A
        AND   A,#$0F
        STA   A,DG_EYLEM                ; geçici: N
        LDA   A,AR_CER+4,X              ; n
        CMP   A,#'1'*2
        BEQ   DK_W_1
        CMP   A,#'2'*2
        BNE   DK_HAYIRJ
        LDA   A,DG_EYLEM                ; WIDE2-N: N = 1..2, yol dolu değil
        BEQ   DK_HAYIRJ
        CMP   A,#3
        BCS   DK_HAYIRJ
        LDA   A,DG_SAY
        CMP   A,#8
        BCS   DK_HAYIRJ
        LDM   #2,DG_EYLEM
        BRA   DK_TEKRAR
DK_W_1: LDA   A,DG_EYLEM                ; WIDE1-1
        CMP   A,#1
        BNE   DK_HAYIRJ
        LDM   #1,DG_EYLEM
DK_TEKRAR:                              ; son 30 s'de tekrarlandı mı
        JSR   DG_CRC_HESAP
        LDX   #0
        LDY   #0
DK_T1:  LDA   A,DG_YAS,X
        BEQ   DK_T2
        CLM
        LDA   A,DG_CRCK,Y
        CMP   A,DG_CRC
        SEM
        BEQ   DK_HAYIR
DK_T2:  INX
        INY
        INY
        CPX   #3
        BNE   DK_T1
        SEC
        RTS
DK_HAYIRJ:
DK_HAYIR:
        CLC
        RTS

; AR_CER[X..X+6] çağrımız mı (6 karakter + SSID; H / son / ayrılmış bitler hariç). C = 1 evet (M = 1, X korunur)
DG_BENIM:
        .ASSUME M=1,X=0
        PHX
        LDY   #0
DB_1:   LDA   A,AR_CER,X
        CMP   A,AR_CER+DG_MY,Y
        BNE   DB_HAYIR
        INX
        INY
        CPY   #6
        BNE   DB_1
        LDA   A,AR_CER,X
        EOR   A,AR_CER+DG_MY,Y
        AND   A,#SSID_MASKE
        BNE   DB_HAYIR
        PLX
        SEC
        RTS
DB_HAYIR:
        PLX
        CLC
        RTS

; DG_CRC = CRC-16 (hedef + gönderen, SSID baytları maskeli; bilgi alanı) (M = 1)
DG_CRC_HESAP:
        .ASSUME M=1,X=0
        CLM
        LDA   A,#$FFFF
        STA   A,DG_CRC
        SEM
        LDX   #0
DC_A:   LDA   A,AR_CER,X
        CPX   #6
        BEQ   DC_M
        CPX   #13
        BNE   DC_E
DC_M:   AND   A,#SSID_MASKE
DC_E:   JSR   DG_CRC_BAYT
        INX
        CPX   #14
        BNE   DC_A
        LDA   A,DG_BILGI
        CLM
        AND   A,#$00FF
        TAX
        SEM
DC_B:   TXA
        CMP   A,AR_Y_N
        BCS   DC_SON
        LDA   A,AR_CER,X
        JSR   DG_CRC_BAYT
        INX
        BRA   DC_B
DC_SON: RTS

DG_CRC_BAYT:
        .ASSUME M=1,X=0
        PHX
        CLM
        AND   A,#$00FF
        EOR   A,DG_CRC
        LDX   #8
DCB_1:  LSR   A
        BCC   DCB_2
        EOR   A,#$8408
DCB_2:  DEX
        BNE   DCB_1
        STA   A,DG_CRC
        SEM
        PLX
        RTS

; Çerçeveyi düzenle (DG_KARAR'dan hemen sonra; M = 1), FCS ekle (AP_UZ)
DG_DUZENLE:
        .ASSUME M=1,X=0
        LDX   #0                        ; çağrımızı kaydırma öncesi kopyala
DD_0:   LDA   A,AR_CER+DG_MY,X
        STA   A,DG_CAGRI,X
        INX
        CPX   #7
        BNE   DD_0
        LDA   A,DG_GIRDI
        CLM
        AND   A,#$00FF
        TAX
        SEM
        LDA   A,DG_EYLEM
        BEQ   DD_H                      ; bize: yalnız H biti
        CMP   A,#1
        BEQ   DD_YAZ                    ; WIDE1-1 yerine çağrımız
        ; WIDE2-N: [X..N-1] 7 bayt yukarı, X'e çağrımız*, X+7'deki WIDE2'nin N'si bir azalır
        PHX
        LDA   A,AR_Y_N
        CLM
        AND   A,#$00FF
        TAX                             ; X = N (kaynak sonu)
        SEM
DD_K:   DEX
        LDA   A,AR_CER,X
        STA   A,AR_CER+7,X
        TXA
        CMP   A,DG_GIRDI
        BNE   DD_K
        PLX
        LDA   A,AR_CER+7+6,X            ; WIDE2 SSID: N - 1 (0 ise H biti)
        AND   A,#$01                    ; son adres biti
        STA   A,DG_SAY
        LDA   A,AR_CER+7+6,X
        LSR   A
        AND   A,#$0F
        DEC   A
        PHA
        ASL   A
        ORA   A,#$60
        ORA   A,DG_SAY
        STA   A,AR_CER+7+6,X
        PLA
        BNE   DD_K2
        LDA   A,AR_CER+7+6,X
        ORA   A,#$80
        STA   A,AR_CER+7+6,X
DD_K2:  LDA   A,AR_CER+6,X              ; araya giren çağrımız: son adres değil
        AND   A,#$FE
        STA   A,AR_CER+6,X
        LDA   A,AR_Y_N
        CLC
        ADC   A,#7
        STA   A,AR_Y_N
DD_YAZ: LDA   A,AR_CER+6,X              ; çağrımız*, eski girdinin son adres biti korunur
        AND   A,#$01
        STA   A,DG_SAY
        LDY   #0
DD_Y1:  LDA   A,DG_CAGRI,Y
        STA   A,AR_CER,X
        INX
        INY
        CPY   #6
        BNE   DD_Y1
        LDA   A,DG_CAGRI+6
        AND   A,#$7E
        ORA   A,DG_SAY
        STA   A,AR_CER,X                ; X = girdi + 6
        BRA   DD_H1
DD_H:   CLM
        TXA
        CLC
        ADC   A,#6
        TAX
        SEM
DD_H1:  LDA   A,AR_CER,X
        ORA   A,#$80                    ; H: tekrarlandı
        STA   A,AR_CER,X
        LDA   A,AR_Y_N
        CLM
        AND   A,#$00FF
        STA   A,AP_UZ
        JSR   AP_FCS
        SEM
        RTS

; Tekrar kaydı: en eski (en küçük yaşlı) yuvaya DG_CRC, yaş DG_ESIK (M = 1)
DG_KAYDET:
        .ASSUME M=1,X=0
        LDX   #0
        LDY   #0                        ; Y = seçilen
        LDA   A,#$FF
        STA   A,DG_EYLEM
DKY_1:  LDA   A,DG_YAS,X
        CMP   A,DG_EYLEM
        BCS   DKY_2
        STA   A,DG_EYLEM
        TXY
DKY_2:  INX
        CPX   #3
        BNE   DKY_1
        LDA   A,#DG_ESIK
        STA   A,DG_YAS,Y
        TYA
        ASL   A
        CLM
        AND   A,#$00FF
        TAY
        LDA   A,DG_CRC
        STA   A,DG_CRCK,Y
        SEM
        RTS
