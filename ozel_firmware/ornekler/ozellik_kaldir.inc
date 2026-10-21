; ozellik_kaldir.inc - kullanılmayan özellikler kapatıldı (AT V0-20)
;
; Kod flash'ta kalır ama hiçbir yoldan çalışmaz: giriş noktaları kesildi, menüden kaldırıldı.
; (Çağrı grafiğiyle bulundu; boyutlar orijinal YA4000S'de o özelliğin yalnız kendine ait kodu.)
;
;  Özellik                                  Kod     Kapatılan giriş
;  Trunk (RADIO / TELSIZ modu, MPT1327      ~20,7 KB ana döngü 0x0246 bit 1 dalı; RADIO menüsü;
;    benzeri: ROAMING, PBX, FLEET, SDATA)            bit 1'i kuran 4 yer (EE 0x000 bit 4 opsiyonu,
;                                                    açılışta EE 0x54B bit 4, menü)
;  Veri mesajı / sipariş (FFSK, yazıcıya    ~6,1 KB  '*' tuşu (F_014A76), kanal ekranı (F_014AB3),
;    Isim / Adres / Sip.No ...)                      seri komut 80 / 82 (F_014EB2). F_0134C0 durum
;                                                    makinesi $0118 = 0'da hemen döner
;  Kablosuz ses yayın (Y opsiyonu,          ~5,2 KB  ana döngü 0x0335 bit 4 dalı
;    SESYAYIN / GECE / GUNDUZ)
;  Seçmeli çağrı ile arama ('#' "C GG___")  -       normal ekranda '#' (EE 0x014 bit 2)
;    (gönderme / alma secmeli_kaldir.inc'te)
;  Servis terminali: UART0'da 0xF8 ile      ~1,7 KB  alma kesmesi 0x0194C0 (EE 0x000 bit 2)
;    2400 baud S-record monitörü (F_0099BD)
;  Servis / test modu ('*', "HELP 1 FLASH   ~4,1 KB  '*' tuşu (F_01CA08; zaten ROM 0x0100FF = 1
;    3 EEPROM", F_01CA08)                            ister, hiçbir sürümde yok)
;
; Seri programlama protokolü (yükleme sayfası, T4000) dokunulmadı.

; ---- Trunk ------------------------------------------------------------------------------------
        .ORG $01A78C                    ; ana döngü: BBC #$02,$0246 -> hep konvansiyonel dal
        .EXPECT $3C,$46,$02,$02,$14
        .ASSUME M=1,X=0
        BRA   OK_KONV
        .BYTE $EA,$EA,$EA
OK_KONV     .EQU $01A7A5

        .ORG $0055E1                    ; menü: RADIO (durum $11) her zaman atlanır -> 4 -> 8
        .EXPECT $3C,$34,$03,$10,$05
        .ASSUME M=1,X=0
        BRA   OK_RADIO_ATLA
        .BYTE $EA,$EA,$EA
OK_RADIO_ATLA .EQU $0055EB

        .ORG $004CEE                    ; RADIO menüsü işleyicisi: SEB #$0002,$0246
        .EXPECT $0C,$46,$02,$02,$00
        .BYTE $EA,$EA,$EA,$EA,$EA
        .ORG $0067C2                    ; A tipi kart, kanal anahtarı >= 15 ve trunk opsiyonu
        .EXPECT $0C,$46,$02,$02,$00
        .BYTE $EA,$EA,$EA,$EA,$EA
        .ORG $008F64                    ; açılış: EE 0x000 bit 4 (trunk opsiyonu)
        .EXPECT $0C,$46,$02,$02,$00
        .BYTE $EA,$EA,$EA,$EA,$EA
        .ORG $00909D                    ; açılış: kayıtlı mod (EE 0x54B bit 4)
        .EXPECT $0C,$46,$02,$02,$00
        .BYTE $EA,$EA,$EA,$EA,$EA

; ---- Veri mesajı / sipariş ve servis / test modu ('*') ----------------------------------------
        .ORG $00575A                    ; normal ekranda '*': JSRL F_01CA08 / JSRL F_014A76
        .EXPECT $22,$08,$CA,$01,$22,$76,$4A,$01
        .BYTE $EA,$EA,$EA,$EA,$EA,$EA,$EA,$EA
        .ORG $003771                    ; kanal ekranı: JSRL F_014AB3 (A = 0: işlenmedi)
        .EXPECT $22,$B3,$4A,$01,$C9,$00
        .ASSUME M=0,X=0
        SEM                             ; char fonksiyonu gibi M = 1, A = 0
        LDA   A,#$00
        NOP
        .ORG $01A278                    ; seri komut 80: JSRL F_014EB2
        .EXPECT $22,$B2,$4E,$01
        .BYTE $EA,$EA,$EA,$EA
        .ORG $01A29A                    ; seri komut 82
        .EXPECT $22,$B2,$4E,$01
        .BYTE $EA,$EA,$EA,$EA

; ---- Kablosuz ses yayın -----------------------------------------------------------------------
        .ORG $01A761                    ; ana döngü: BBC #$10,$0335 -> hep atla
        .EXPECT $3C,$35,$03,$10,$05
        .ASSUME M=1,X=0
        BRA   OK_YAYIN_YOK
        .BYTE $EA,$EA,$EA
OK_YAYIN_YOK .EQU $01A76B

; ---- Seçmeli çağrı ile arama ('#') ------------------------------------------------------------
        .ORG $00378E                    ; BBS #$04,$033F,L_003796 -> düşer: JMP L_003A1E (normal)
        .EXPECT $2C,$3F,$03,$04,$03
        .BYTE $EA,$EA,$EA,$EA,$EA

; ---- Servis terminali (UART0 0xF8) ------------------------------------------------------------
        .ORG $0194C0                    ; BBC #$04,$0334,L_0194CA -> hep atla
        .EXPECT $3C,$34,$03,$04,$05
        .ASSUME M=1,X=0
        BRA   OK_SERVIS_YOK
        .BYTE $EA,$EA,$EA
OK_SERVIS_YOK .EQU $0194CA
