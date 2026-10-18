; hesap.asm - menüye SAYI ve HESAP öğeleri (YA4000S_HESAP.HEX)
;
;   normal -m-> LIST -m-> SAYI -m-> HESAP -m-> (RADIO / PRINTER / SUB MENU, karta göre) -> normal
;
; Kullanım: sayi_ters.inc ve hesap.inc başındaki açıklamalar.
; Derleme:  make -C disasm ozel
;
        .INCLUDE "menu_altyapi.inc"

ST_SAYI     .EQU $14
ST_SAYI_ON  .EQU $15
ST_HESAP    .EQU $16
ST_HESAP_ON .EQU $17

        .ORG $011000

MENU_NEXT:
        .COPY $003188, 2                ; durum 0, 1
        .BYTE ST_SAYI                   ; durum 2 (LIST) -> SAYI
        .COPY $00318B, N_OLD-3          ; durum 3..$13
        .BYTE ST_HESAP                  ; $14 SAYI  -> HESAP
        .BYTE $00                       ; $15 etkin -> m = normal ekran
        .BYTE $11                       ; $16 HESAP -> RADIO (eskiden LIST'ten sonra gelen)
        .BYTE $00                       ; $17 etkin -> m = normal ekran

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

TITLES_EN:
        .COPY $002DE6, 8*N_OLD
        .TEXT8 "NUMBER"
        .TEXT8 ""
        .TEXT8 "CALC"
        .TEXT8 ""
TITLES_TR:
        .COPY $002E86, 8*N_OLD
        .TEXT8 "SAYI"
        .TEXT8 ""
        .TEXT8 "HESAP"
        .TEXT8 ""

        .INCLUDE "sayi_ters.inc"
        .INCLUDE "hesap.inc"
