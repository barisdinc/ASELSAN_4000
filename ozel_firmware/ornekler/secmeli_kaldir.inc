; secmeli_kaldir.inc - seçmeli çağrıyı (Select-5: CCIR / ZVEI / EEA) firmware'den kaldırır
;
; Yalnız seçmeli çağrının kullandığı kod (çağrı grafiğinden, docs/calisma_analizi.md bölüm 9):
;   F_00E65B   0x00E65B..0x00E9D1   887 bayt  gönderme: kimlik / çağrı / transpond tonları
;   F_007C46   0x007C46..0x007CF5   176 bayt  alma çözücüsünü başlatma (TB1 periyot, TA1 pencere)
;   F_01BBE4   0x01BBE4..0x01C626  2627 bayt  hane kararı, çağrı / grup / transpond, çağrı ekranı
;   TA1 kesmesi 0x01C6C0..0x01C8DD  542 bayt  pencere sonu: periyot ortalaması -> F_01BBE4
; Bu bölgelere dışarıdan yalnız aşağıdaki çağrılar giriyor (taranarak doğrulandı); çağrılar
; kaldırılıp bölgeler .SERBEST ile boşaltılır (0xFF), yeni kod için toplam 4232 bayt açılır.
;
; Kalanlar: TB1 kesmesi (0x01C627) DTMF ve APRS için kalır; RX dalı (0x023E bit 4) artık hiç
; açılmaz. Çözücüyü durduran F_007CF6 kalır (zararsız). Sinüs tablosu 0x28FA APRS'te kullanılıyor.
; "Çağır" (# tuşu) ve "transpond" istekleri (RAM 0x0244 bit 0 / 1) kurulmaz: kurulsalar telsiz
; ton göndermeden vericiye geçerdi. PTT'deki kimlik gönderimi (0x00F4E4) aprs.inc'te APRS_PTT'ye
; bağlı.

; ---- çağrılar ---------------------------------------------------------------------------------
SC_KOD_ADRES .EQU *
        .ORG $00F628                    ; durum 12 (PTT bırakıldı): kimlik gönder
        .EXPECT $22,$5B,$E6,$00         ; JSRL F_00E65B
        .BYTE $EA,$EA,$EA,$EA           ; NOP
        .ORG $00F67A                    ; durum 7: çağrı / transpond gönder
        .EXPECT $22,$5B,$E6,$00
        .BYTE $EA,$EA,$EA,$EA
        .ORG $00EBF2                    ; alma kurulumu: çözücüyü başlat
        .EXPECT $22,$46,$7C,$00         ; JSRL F_007C46
        .BYTE $EA,$EA,$EA,$EA
        .ORG $00202C                    ; VEC_TA1: karar kesmesi yerine boş kesme
        .EXPECT $5C,$C0,$C6,$01         ; JMPL L_01C6C0
        JMPL  SC_TA1
; ---- çağır / transpond istekleri -------------------------------------------------------------
        .ORG $003964                    ; # menüsü: numara tamam -> çağır
        .EXPECT $0C,$44,$02,$01,$00     ; SEB #$0001,$0244
        .BYTE $EA,$EA,$EA,$EA,$EA
        .ORG $006461
        .EXPECT $0C,$44,$02,$02,$00     ; SEB #$0002,$0244
        .BYTE $EA,$EA,$EA,$EA,$EA
        .ORG $0068FB                    ; bekleyen çağrı (0x0246 bit 7) -> gönder
        .EXPECT $0C,$44,$02,$02,$00
        .BYTE $EA,$EA,$EA,$EA,$EA
; ---- boşaltılan bölgeler ---------------------------------------------------------------------
        .SERBEST $00E65B,$00E9D2-$00E65B,$A3,$04,$0B,$5B,$18,$69,$15,$00   ; F_00E65B
        .SERBEST $007C46,$007CF6-$007C46,$A3,$04,$0B,$5B,$64,$00,$00,$00   ; F_007C46
        .SERBEST $01BBE4,$01C627-$01BBE4,$A3,$04,$0B,$5B,$84,$00           ; F_01BBE4
        .SERBEST $01C6C0,$01C8DE-$01C6C0,$C2,$39,$EB,$3F,$89,$C2,$00       ; TA1 kesmesi
        .ORG SC_KOD_ADRES

; TA1 kesmesi artık hiç açılmıyor (TA1IC önceliği 0); yine de gelirse dön
SC_TA1: RTI
