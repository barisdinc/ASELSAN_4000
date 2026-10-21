; aprs_seri.inc - APRS alma: çözülen paketleri seri porta (UART0) TNC2 metni olarak yazar
; (aprs_rx.inc'teki AR_ISLE her turda AR_SERI'yi çağırır; ayrıntı aprs_rx.inc başında).
; frekans.asm'de $011000 bölgesine alınır: APRS bölgesi ($012400..) dolu. JSR'lar aynı bankta (1).

; ---------------------------------------------------------------------------------------------
; Seri port: tampondaki paketi TNC2 metni olarak yaz (AR_ISLE'den; AR_D = NC77 çerçevesi).
; Kuyrukta en az 12 bayt yer varken bir parça yazar (adres en çok "-" + 6 + "-15" + "*" + ayraç).
AR_SERI:
        .ASSUME M=0,X=0
        SEM
        LDA   A,AR_Y_DURUM
        BNE   AS_1
        RTS
AS_1:   LDA   A,RADYO_DURUM             ; gönderiyor: tampon gönderme paketine geçecek
        CMP   A,#4
        BCC   AS_DON
        LDM   #5,AR_Y_DURUM             ; satırı kapat
AS_DON: .ASSUME M=1,X=0
        JSR   AY_YER
        BCC   AS_2
        RTS
AS_2:   LDA   A,AR_Y_DURUM
        BNE   AS_P1
        RTS
AS_P1:  CMP   A,#1
        BNE   AS_P2
        LDX   #7                        ; gönderen
        JSR   AY_ADRES
        LDM   #2,AR_Y_DURUM
        BRA   AS_DON
AS_P2:  CMP   A,#2
        BNE   AS_P3
        LDA   A,#'>'
        JSR   AY_KOY
        LDX   #0                        ; hedef
        JSR   AY_ADRES
        LDM   #14,AR_Y_I
        LDA   A,#3                      ; gönderenin son adres biti: yol yok
        BBC   #$01,AR_CER+13,AS_P2A
        LDA   A,#4
AS_P2A: STA   A,AR_Y_DURUM
        BRA   AS_DON
AS_P3:  CMP   A,#3
        BNE   AS_P4
        LDA   A,AR_Y_I                  ; yol adresi: I + 7 <= N olmalı
        CLC
        ADC   A,#7
        BCS   AS_BOZUK
        CMP   A,AR_Y_N
        BEQ   AS_P3A
        BCS   AS_BOZUK
AS_P3A: CLM
        LDA   A,AR_Y_I
        AND   A,#$00FF
        TAX
        SEM
        LDA   A,#','
        JSR   AY_KOY
        JSR   AY_ADRES
        LDA   A,AR_CER+6,X              ; H biti: tekrarlandı; yıldız yalnız sonuncusunda
        BPL   AS_P3B
        AND   A,#$01
        BNE   AS_YILDIZ
        LDA   A,AR_CER+13,X
        BMI   AS_P3B
AS_YILDIZ:
        LDA   A,#'*'
        JSR   AY_KOY
AS_P3B: TXA
        CLC
        ADC   A,#7
        STA   A,AR_Y_I
        LDA   A,AR_CER+6,X
        AND   A,#$01
        BEQ   AS_DON2
        LDM   #4,AR_Y_DURUM
AS_DON2:
        JMP   AS_DON
AS_BOZUK:
        LDM   #5,AR_Y_DURUM
        JMP   AS_DON
AS_P4:  CMP   A,#4
        BNE   AS_P6
        LDA   A,AR_Y_I                  ; denetim + PID baytlarını atla
        CLC
        ADC   A,#2
        CMP   A,AR_Y_N
        BEQ   AS_P4A
        BCS   AS_BOZUK
AS_P4A: STA   A,AR_Y_I
        LDA   A,#':'
        JSR   AY_KOY
        LDM   #6,AR_Y_DURUM
        JMP   AS_DON
AS_P6:  CMP   A,#6
        BNE   AS_P5
        LDA   A,AR_Y_I
        CMP   A,AR_Y_N
        BCC   AS_P6A
        LDM   #5,AR_Y_DURUM
        JMP   AS_DON
AS_P6A: CLM
        AND   A,#$00FF
        TAX
        SEM
        LDA   A,AR_CER,X
        CMP   A,#$0D
        BEQ   AS_BOSLUK
        CMP   A,#$0A
        BNE   AS_P6B
AS_BOSLUK:
        LDA   A,#' '
AS_P6B: JSR   AY_KOY
        INC   AR_Y_I
        JMP   AS_DON
AS_P5:  LDA   A,#$0D                    ; satır sonu
        JSR   AY_KOY
        LDA   A,#$0A
        JSR   AY_KOY
        LDM   #0,AR_Y_DURUM
        RTS

; Kuyrukta 12 bayt yer var mı (M = 1)? C = 0: var. Kuyruk boşsa (okunan = yazılan) firmware ilk
; baytı doğrudan gönderip göstergeleri sıfırlar.
AY_YER:
        .ASSUME M=1,X=0
        LDA   A,SERI_OKU
        CMP   A,SERI_YAZ
        BEQ   AYY_VAR
        LDA   A,SERI_YAZ
        CMP   A,#$50-12
        RTS
AYY_VAR:
        CLC
        RTS

; AX.25 adresi (X = başı, M = 1): çağrı işareti (boşluklar atlanır) ve SSID ("-n", 0 ise yok).
; X korunur.
AY_ADRES:
        .ASSUME M=1,X=0
        PHX
        LDY   #6
AYA_1:  LDA   A,AR_CER,X
        LSR   A
        CMP   A,#' '
        BEQ   AYA_2
        JSR   AY_KOY
        INX
        DEY
        BNE   AYA_1
AYA_2:  PLX
        LDA   A,AR_CER+6,X
        LSR   A
        AND   A,#$0F
        BEQ   AYA_SON
        PHA
        LDA   A,#'-'
        JSR   AY_KOY
        PLA
        CMP   A,#10
        BCC   AYA_3
        PHA
        LDA   A,#'1'
        JSR   AY_KOY
        PLA
        SEC
        SBC   A,#10
AYA_3:  CLC
        ADC   A,#'0'
        JSR   AY_KOY
AYA_SON:
        RTS

; A (M = 1) -> UART0 kuyruğu (F_007627). X ve Y korunur.
AY_KOY:
        .ASSUME M=1,X=0
        PHX
        PHY
        CLM
        AND   A,#$00FF
        TAY
        LDA   A,AR_D
        PHA
        JSRL  F_SERI
        CLM
        PLA
        SEM
        PLY
        PLX
        RTS
