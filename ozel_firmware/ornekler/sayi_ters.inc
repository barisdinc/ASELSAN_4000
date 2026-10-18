; sayi_ters.inc - SAYI menü öğesinin işleyicileri (menu_altyapi.inc ile)
;
;   m (LIST) m (SAYI) s   -> "_"
;   0-9                    -> haneler sona eklenir (en çok 8, fazlası bip)
;   *                      -> son haneyi sil
;   #                      -> temizle
;   s                      -> tersini göster / düz göster
;   m                      -> çık (normal ekran)
;
; Haneler firmware'in tamponunda: sayacı en çok 8'de tuttuğumuz için $0283..$028A'da sırayla
; durur. Tampondaki son bayt ($0292) bu modda hiç yazılmaz; "ters göster" bayrağı olarak
; kullanıyoruz.

REVFLAG     .EQU $0292

; ---------------------------------------------------------------------------------------------
; İşleyiciler: F_01E4C2 ile M=0, X=0 olarak çağrılır; 4,S çağıranın çerçeve göstericisidir.
; Firmware fonksiyonu çağırabilmek için onu yığına geri koyuyoruz (NC77 kuralı: çağrılan
; fonksiyon kendi çerçevesini 4,S'deki değerden başlatır).
; ---------------------------------------------------------------------------------------------

; SAYI başlığında s: tamponu temizle, sonraki tuşlar için işleyiciyi SAYI_TUS yap.
; (Firmware yalnızca s'de işleyiciyi tablodan alır; rakamlarda son işleyici çağrılır.)
SAYI_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        LDM   #SAYI_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(SAYI_TUS),HANDLER_BK
        LDM   #0,NDIGIT
        LDM   #0,REVFLAG
        JMP   CIZ

; Etkin durumda her tuş
SAYI_TUS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        SEM
        LDA   A,KEY
        CMP   A,#'d'
        BEQ   RAKAM
        CMP   A,#'*'
        BEQ   SIL
        CMP   A,#'#'
        BEQ   TEMIZLE
        CMP   A,#'s'
        BNE   CIZ
        LDA   A,REVFLAG                 ; s: ters / düz
        EOR   A,#1
        STA   A,REVFLAG
        BRA   CIZ

RAKAM:  LDM   #0,REVFLAG
        LDA   A,NDIGIT
        CMP   A,#9                      ; 9. hane geldiyse geri al ve bip
        BCC   CIZ
        LDM   #8,NDIGIT
        CLM
        LDY   #13
        JSRL  F_BEEP
        SEM
        BRA   CIZ

SIL:    LDM   #0,REVFLAG
        LDA   A,NDIGIT
        BEQ   CIZ
        DEC   NDIGIT
        BRA   CIZ

TEMIZLE:
        LDM   #0,NDIGIT
        LDM   #0,REVFLAG
        ; ekrana çiz

; ---- LCD tamponunu doldur (M=1, X=0 ile girilir), işleyiciden çık
CIZ:    .ASSUME M=1,X=0
        LDX   #0
        LDA   A,#' '
BOSLUK: STA   A,LCD,X
        INX
        CPX   #8
        BNE   BOSLUK

        CLM                             ; Y = rakam sayısı (16 bit)
        LDA   A,NDIGIT
        AND   A,#$00FF
        TAY
        SEM
        LDX   #0
        LDA   A,REVFLAG
        BNE   TERS

        CPY   #0                        ; düz: LCD[i] = DIGITS[i]
        BEQ   IMLEC
DUZ:    LDA   A,DIGITS,X
        STA   A,LCD,X
        INX
        DEY
        BNE   DUZ
IMLEC:  CPX   #8                        ; yer varsa imleç
        BEQ   BITTI
        LDA   A,#'_'
        STA   A,LCD,X
        BRA   BITTI

TERS:   CPY   #0                        ; ters: LCD[i] = DIGITS[n-1-i]
        BEQ   BITTI
TLOOP:  DEY
        LDA   A,DIGITS,Y
        STA   A,LCD,X
        INX
        CPY   #0
        BNE   TLOOP

BITTI:  CLM
        PLA
        RTL
