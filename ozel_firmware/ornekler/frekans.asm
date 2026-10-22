; frekans.asm - menüye FREKANS ve APRS öğeleri (YA4000S_FREKANS.HEX)
;
;   normal -m-> LIST -m-> FREKANS -m-> APRS -m-> APRS TR -m-> APRS DG -m-> APRS IG -m-> (ALT MENU) -> normal
;
; SAYI, HESAP ve OYUN örnekleri (sayi_ters.asm, hesap.asm, oyun.asm) bu firmware'de yok; yer açmak
; için çıkarıldı. Kullanım: frekans.inc ve aprs.inc başındaki açıklamalar.
; Derleme:  make -C disasm ozel
;
        .INCLUDE "menu_altyapi.inc"

ST_FREKANS  .EQU $14
ST_FREKANS_ON .EQU $15
ST_APRS     .EQU $16
ST_APRS_ON  .EQU $17

        .ORG $011000

MENU_NEXT:
        .COPY $003188, 2                ; durum 0, 1
        .BYTE ST_FREKANS                ; durum 2 (LIST) -> FREKANS
        .COPY $00318B, N_OLD-3          ; durum 3..$13
        .BYTE ST_APRS                   ; $14 FREKANS -> APRS
        .BYTE $00                       ; $15 etkin -> m = normal ekran
        .BYTE ST_APRS_TR                ; $16 APRS -> APRS TR
        .BYTE $00                       ; $17
        .BYTE ST_APRS_DG                ; $18 APRS TR -> APRS DG
        .BYTE ST_APRS_IG                ; $19 APRS DG -> APRS IG
        .BYTE $11                       ; $1A APRS IG -> RADIO (atlanır) -> ALT MENU / normal

MENU_S:
        .COPY $00319C, 4*N_OLD
        .BYTE ST_FREKANS_ON             ; $14
        .LONG FREKANS_GIR
        .BYTE ST_FREKANS_ON             ; $15
        .LONG FREKANS_TUS
        .BYTE ST_APRS_ON                ; $16
        .LONG APRS_GIR
        .BYTE ST_APRS_ON                ; $17
        .LONG APRS_TUS
        .BYTE ST_APRS_TR                ; $18 s: aç / kapat, aynı öğede kal
        .LONG AYAR_TUS_TR
        .BYTE ST_APRS_DG                ; $19
        .LONG AYAR_TUS_DG
        .BYTE ST_APRS_IG                ; $1A
        .LONG AYAR_TUS_IG

TITLES_EN:
        .COPY $002DE6, 8*N_OLD
        .TEXT8 "FREQ"
        .TEXT8 ""
        .TEXT8 "APRS"
        .TEXT8 ""
        .TEXT8 "APRS TR"
        .TEXT8 "APRS DG"
        .TEXT8 "APRS IG"
TITLES_TR:
        .COPY $002E86, 8*N_OLD
        .TEXT8 "FREKANS"
        .TEXT8 ""
        .TEXT8 "APRS"
        .TEXT8 ""
        .TEXT8 "APRS TR"
        .TEXT8 "APRS DG"
        .TEXT8 "APRS IG"

        .INCLUDE "frekans.inc"
        .INCLUDE "aprs_seri.inc"        ; APRS alma: paketleri seri porta yaz
        .ORG $01E600                    ; bank 1 boş alanı ($01E4F4..$01FFFF; $011000 bölgesi doldu)
        .INCLUDE "aprs_detay.inc"       ; APRS alma: istasyonların konumu
        .INCLUDE "aprs_digi.inc"        ; APRS digipeater
        .INCLUDE "aprs_arka.inc"        ; arka planda APRS tracker

        .ORG $012400                    ; bank 1 boş alanı (açılış ekranı $012000'de)
        .INCLUDE "aprs.inc"
        .INCLUDE "aprs_rx.inc"          ; APRS alma: duyulan istasyonlar
        .INCLUDE "secmeli_kaldir.inc"   ; seçmeli çağrı kaldırıldı, ~4,1 KB boşaltıldı
        .INCLUDE "tarama_tks.inc"       ; taramada TKS bekleme süresi uzun (ASELSAN V2-51/V2-54)
        .INCLUDE "yazici_kaldir.inc"    ; YAZICI menüsü ve yazıcı çıkışı yok
        .INCLUDE "ozellik_kaldir.inc"   ; trunk, veri mesajı, ses yayın, # arama, servis kapalı
DIL_KOD     .EQU $012090        ; bank 1 boş alanı (tarama_tks $012080..$01208A)
        .INCLUDE "dil_kaldir.inc"       ; yalnız Türkçe
        .ORG $012100                    ; bank 1 boş alanı ($012090.. dil_kaldir)
        .INCLUDE "aprs_ayar.inc"        ; APRS TR / DG / IG ayarları
GPS_KOD     .EQU *
        .INCLUDE "gps.inc"              ; seri porttan GPS konumu
