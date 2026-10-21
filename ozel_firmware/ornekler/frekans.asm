; frekans.asm - menüye FREKANS ve APRS öğeleri (YA4000S_FREKANS.HEX)
;
;   normal -m-> LIST -m-> FREKANS -m-> APRS -m-> (RADIO / PRINTER / SUB MENU) -> normal
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
        .BYTE $11                       ; $16 APRS -> RADIO (eskiden LIST'ten sonra gelen)
        .BYTE $00                       ; $17

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

TITLES_EN:
        .COPY $002DE6, 8*N_OLD
        .TEXT8 "FREQ"
        .TEXT8 ""
        .TEXT8 "APRS"
        .TEXT8 ""
TITLES_TR:
        .COPY $002E86, 8*N_OLD
        .TEXT8 "FREKANS"
        .TEXT8 ""
        .TEXT8 "APRS"
        .TEXT8 ""

        .INCLUDE "frekans.inc"
        .INCLUDE "aprs_seri.inc"        ; APRS alma: paketleri seri porta yaz

        .ORG $012400                    ; bank 1 boş alanı (açılış ekranı $012000'de)
        .INCLUDE "aprs.inc"
        .INCLUDE "aprs_rx.inc"          ; APRS alma: duyulan istasyonlar
        .INCLUDE "secmeli_kaldir.inc"   ; seçmeli çağrı kaldırıldı, ~4,1 KB boşaltıldı
        .INCLUDE "tarama_tks.inc"       ; taramada TKS bekleme süresi uzun (ASELSAN V2-51/V2-54)
