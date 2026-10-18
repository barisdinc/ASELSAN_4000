; frekans.asm - menüye SAYI, HESAP, OYUN ve FREKANS öğeleri (YA4000S_FREKANS.HEX)
;
;   normal -m-> LIST -m-> SAYI -m-> HESAP -m-> OYUN -m-> FREKANS -m-> APRS -m-> (RADIO / PRINTER /
;   SUB MENU) -> normal
;
; Kullanım: sayi_ters.inc, hesap.inc, oyun.inc, frekans.inc ve aprs.inc başındaki açıklamalar.
; Derleme:  make -C disasm ozel
;
        .INCLUDE "menu_altyapi.inc"

ST_SAYI     .EQU $14
ST_SAYI_ON  .EQU $15
ST_HESAP    .EQU $16
ST_HESAP_ON .EQU $17
ST_OYUN     .EQU $18
ST_OYUN_ON  .EQU $19
ST_FREKANS  .EQU $1A
ST_FREKANS_ON .EQU $1B
ST_APRS     .EQU $1C
ST_APRS_ON  .EQU $1D

        .ORG $011000

MENU_NEXT:
        .COPY $003188, 2                ; durum 0, 1
        .BYTE ST_SAYI                   ; durum 2 (LIST) -> SAYI
        .COPY $00318B, N_OLD-3          ; durum 3..$13
        .BYTE ST_HESAP                  ; $14 SAYI  -> HESAP
        .BYTE $00                       ; $15 etkin -> m = normal ekran
        .BYTE ST_OYUN                   ; $16 HESAP -> OYUN
        .BYTE $00                       ; $17
        .BYTE ST_FREKANS                ; $18 OYUN  -> FREKANS
        .BYTE $00                       ; $19
        .BYTE ST_APRS                   ; $1A FREKANS -> APRS
        .BYTE $00                       ; $1B
        .BYTE $11                       ; $1C APRS -> RADIO (eskiden LIST'ten sonra gelen)
        .BYTE $00                       ; $1D

MENU_S:
        .COPY $00319C, 4*N_OLD
        .BYTE ST_SAYI_ON                ; $14
        .LONG SAYI_GIR
        .BYTE ST_SAYI_ON                ; $15
        .LONG SAYI_TUS
        .BYTE ST_HESAP_ON               ; $16
        .LONG HESAP_GIR
        .BYTE ST_HESAP_ON               ; $17
        .LONG HESAP_TUS
        .BYTE ST_OYUN_ON                ; $18
        .LONG OYUN_GIR
        .BYTE ST_OYUN_ON                ; $19
        .LONG OYUN_TUS
        .BYTE ST_FREKANS_ON             ; $1A
        .LONG FREKANS_GIR
        .BYTE ST_FREKANS_ON             ; $1B
        .LONG FREKANS_TUS
        .BYTE ST_APRS_ON                ; $1C
        .LONG APRS_GIR
        .BYTE ST_APRS_ON                ; $1D
        .LONG APRS_TUS

TITLES_EN:
        .COPY $002DE6, 8*N_OLD
        .TEXT8 "NUMBER"
        .TEXT8 ""
        .TEXT8 "CALC"
        .TEXT8 ""
        .TEXT8 "GAME"
        .TEXT8 ""
        .TEXT8 "FREQ"
        .TEXT8 ""
        .TEXT8 "APRS"
        .TEXT8 ""
TITLES_TR:
        .COPY $002E86, 8*N_OLD
        .TEXT8 "SAYI"
        .TEXT8 ""
        .TEXT8 "HESAP"
        .TEXT8 ""
        .TEXT8 "OYUN"
        .TEXT8 ""
        .TEXT8 "FREKANS"
        .TEXT8 ""
        .TEXT8 "APRS"
        .TEXT8 ""

        .INCLUDE "sayi_ters.inc"
        .INCLUDE "hesap.inc"
        .INCLUDE "oyun.inc"
        .INCLUDE "frekans.inc"

        .ORG $012400                    ; bank 1 boş alanı (açılış ekranı $012000'de)
        .INCLUDE "aprs.inc"
