; sayi_ters.asm - menüye "SAYI" öğesi: tuş takımından girilen sayıyı yaz, `s` ile tersini göster
;
;   m (LIST) m (SAYI) s   -> "_"
;   0-9                    -> haneler sona eklenir (en çok 8, fazlası bip)
;   *                      -> son haneyi sil
;   #                      -> temizle
;   s                      -> tersini göster / düz göster
;   m                      -> çık (normal ekran)
;
; Bu dosya yalnızca menü tablolarını kurar; altyapı menu_altyapi.inc'te, işleyiciler sayi_ters.inc'te.
;
; Derleme:  make -C disasm ozel   (ya da)
;   python3 disasm/tools/yama.py YA4000S.HEX ozel_firmware/YA4000S_SAYI.HEX ozel_firmware/ornekler/sayi_ters.asm
;
        .INCLUDE "menu_altyapi.inc"

ST_SAYI     .EQU $14            ; başlık durumu: "SAYI"
ST_SAYI_ON  .EQU $15            ; etkin durum

; =============================================================================================
; Yeni tablolar ve kod (bank 1, boş flash alanı; ROM toplamına girmez)
; =============================================================================================
        .ORG $011000

; ---- m tablosu: LIST(2) -> SAYI -> (eskiden LIST'ten sonra gelen) RADIO($11) ...
MENU_NEXT:
        .COPY $003188, 2                ; durum 0, 1
        .BYTE ST_SAYI                   ; durum 2 (LIST): sonraki artık SAYI
        .COPY $00318B, N_OLD-3          ; durum 3..$13
        .BYTE $11                       ; $14 SAYI   -> RADIO (A tipi kartta PRINTER/SUB MENU/normal'e atlar)
        .BYTE $00                       ; $15 etkin  -> m = normal ekran

; ---- s tablosu
MENU_S:
        .COPY $00319C, 4*N_OLD
        .BYTE ST_SAYI_ON                ; $14: s -> etkin duruma geç, SAYI_GIR çağrılır
        .LONG SAYI_GIR
        .BYTE ST_SAYI_ON                ; $15: s -> aynı durumda kal, SAYI_TUS çağrılır
        .LONG SAYI_TUS

; ---- başlıklar
TITLES_EN:
        .COPY $002DE6, 8*N_OLD
        .TEXT8 "NUMBER"
        .TEXT8 ""
TITLES_TR:
        .COPY $002E86, 8*N_OLD
        .TEXT8 "SAYI"
        .TEXT8 ""

        .INCLUDE "sayi_ters.inc"
