; glif.inc - LCD fontunda olmayan karakterler (frekans.asm; açılış ekranının 'y'si de burada)
;
; F_00828C her karakteri 16 bitlik segment sözcüğüne çevirir; fontta (ROM 0x2C9C) yalnız A-Z,
; 0-9 ve * < _ - : / + ^ var. Harf denetiminin başına (0x00854C) bir JMPL konur; karakter
; GL_TABLO'daysa segment sözcüğü oradan yazılır, değilse orijinal yoldan devam edilir.
; Segmentler (fonttan): bit 3 üst, 8 alt, 2 / 1 sağ üst / alt dikey, 10 / 9 sol üst / alt dikey,
; 6 / 13 orta sol / sağ, 15 / 4 orta üst / alt dikey, 7 / 14 sol / sağ üst çapraz,
; 5 / 12 sol / sağ alt çapraz.
; Ekran ters (GOSTERGE TERS, 0x023C bit 8) seçiliyken F_00828C başka bir tablodan (0x2D0A)
; gider; o yolda bu karakterler boş görünür.

GL_ADRES    .EQU *
        .ORG $00854C                    ; F_00828C: CMP A,#$5B / BCS L_008575 (harf mi?)
        .EXPECT $C9,$5B,$B0,$25
        .ASSUME M=1,X=0
        JMPL  GL_DENETIM
        .ORG GL_ADRES

; Gelişte M = 1, X = 0, A = $01 = karakter, $00 = konum; D = F_00828C'nin çerçevesi
GL_DENETIM:
        .ASSUME M=1,X=0
        PHX
        LDX   #0
GL_1:   LDA   A,>GL_TABLO,X
        BEQ   GL_YOK
        CMP   A,$01
        BEQ   GL_VAR
        INX
        INX
        INX
        BRA   GL_1
GL_YOK: PLX
        LDA   A,$01
        CMP   A,#$5B                    ; orijinal: CMP A,#$5B / BCS L_008575
        BCC   GL_HARF
        JMPL  $008575                   ; harf değil: rakam / işaret denetimleri
GL_HARF:
        JMPL  $008550                   ; orijinal: CMP A,#$41 ...
GL_VAR: CLM                             ; harf yolundaki gibi: $02 + 2*konum = segment sözcüğü
        .ASSUME M=0,X=0
        LDA   A,>GL_TABLO+1,X
        STA   A,$1A
        PLX
        LDA   A,$00
        AND   A,#$00FF
        ASL   A
        TAX
        LDA   A,$1A
        STA   A,$02,X
        JMPL  $0086F5                   ; sonraki karaktere (orijinal yol da M = 0 ile gelir)

GL_TABLO:
        .BYTE 'y'
        .WORD $2546                     ; '4' + alt
        .BYTE '>'
        .WORD $00A0                     ; sol çaprazlar
        .BYTE ')'
        .WORD $00A0
        .BYTE '('
        .WORD $5000                     ; sağ çaprazlar ('<')
        .BYTE '.'
        .WORD $0010                     ; orta alt dikey
        .BYTE ','
        .WORD $0020                     ; sol alt çapraz
        .BYTE '!'
        .WORD $8000                     ; orta üst dikey
        .BYTE '|'
        .WORD $8010
        .BYTE '='
        .WORD $2140                     ; orta + alt
        .BYTE '?'
        .WORD $201C                     ; üst, sağ üst, orta sağ, orta alt
        .BYTE '"'
        .WORD $8004
        .BYTE $27                       ; '
        .WORD $4000
        .BYTE '`'
        .WORD $0080
        .BYTE '#'
        .WORD $A056
        .BYTE '@'
        .WORD $270E
        .BYTE '$'
        .WORD $A19A                     ; S + orta dikeyler
        .BYTE ';'
        .WORD $8020
        .BYTE '%'
        .WORD $4020
        .BYTE '&'
        .WORD $A050
        .BYTE '['
        .WORD $0708
        .BYTE ']'
        .WORD $010E
        .BYTE '{'
        .WORD $0708
        .BYTE '}'
        .WORD $010E
        .BYTE $5C                       ; \
        .WORD $1080
        .BYTE '~'
        .WORD $2040
        .BYTE 0
