; oyun.asm - menüye SAYI, HESAP ve OYUN öğeleri (YA4000S_OYUN.HEX)
;
;   normal -m-> LIST -m-> SAYI -m-> HESAP -m-> OYUN -m-> (RADIO / PRINTER / SUB MENU) -> normal
;
; Kullanım: sayi_ters.inc, hesap.inc ve oyun.inc başındaki açıklamalar.
; Derleme:  make -C disasm ozel
;
        .INCLUDE "menu_altyapi.inc"

ST_SAYI     .EQU $14
ST_SAYI_ON  .EQU $15
ST_HESAP    .EQU $16
ST_HESAP_ON .EQU $17
ST_OYUN     .EQU $18
ST_OYUN_ON  .EQU $19

        .ORG $011000

MENU_NEXT:
        .COPY $003188, 2                ; durum 0, 1
        .BYTE ST_SAYI                   ; durum 2 (LIST) -> SAYI
        .COPY $00318B, N_OLD-3          ; durum 3..$13
        .BYTE ST_HESAP                  ; $14 SAYI  -> HESAP
        .BYTE $00                       ; $15 etkin -> m = normal ekran
        .BYTE ST_OYUN                   ; $16 HESAP -> OYUN
        .BYTE $00                       ; $17
        .BYTE $11                       ; $18 OYUN  -> RADIO (eskiden LIST'ten sonra gelen)
        .BYTE $00                       ; $19

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

TITLES_EN:
        .COPY $002DE6, 8*N_OLD
        .TEXT8 "NUMBER"
        .TEXT8 ""
        .TEXT8 "CALC"
        .TEXT8 ""
        .TEXT8 "GAME"
        .TEXT8 ""
TITLES_TR:
        .COPY $002E86, 8*N_OLD
        .TEXT8 "SAYI"
        .TEXT8 ""
        .TEXT8 "HESAP"
        .TEXT8 ""
        .TEXT8 "OYUN"
        .TEXT8 ""

        .INCLUDE "sayi_ters.inc"
        .INCLUDE "hesap.inc"
        .INCLUDE "oyun.inc"
