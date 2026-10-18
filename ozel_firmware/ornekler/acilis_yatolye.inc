; acilis_yatolye.inc - açılışta 5 saniye "yATOLYE" (ROM'a gömülü, EEPROM'dan bağımsız)
;
; 1) Açılış ekranı. F_00E0E1 açılışta bir kez çağrılır (F_00E305, 0x00E431): bip sesini
;    çalıp "AS V2-3" sürüm yazısını gösterir. Girişine bank 1'deki ACILIS'e bir JMPL koyuyoruz;
;    ACILIS aynı bip sesini çalar, "yATOLYE" yazar, 5 s bekler ve F_00E0E1'i çağırana döner.
;    Yazı görünürken arka ışıklar yanar: firmware'in kendi ışık yordamı F_008156 (Y = 1:
;    RAM $0242 bit 6 / 7 / 8 ve çıkışlar; tip B'de I2C 0x46 genişletici, tip A'da F_00755E).
;    Açılışın sonunda firmware ışığı kendisi söndürür (0x00E4A7, Y = 0) ve sonra ISIK
;    ayarına göre yönetir.
;    Ardından açılış olduğu gibi sürer (EEPROM'dan PN / SN / CN, kanal).
;    Yazı flash'ta durduğu için T4000 / EEPROM programlayıcısı onu değiştiremez.
;
; 2) Küçük "y". LCD fontunda (ROM 0x2C9C) yalnızca A-Z var. F_00828C her karakteri 16 bitlik
;    segment sözcüğüne çevirir; harf denetiminin başına (0x00854C) bir JMPL koyup 'y'yi
;    (0x79) kendi segment sözcüğümüzle çiziyoruz. Firmware 0x79'u başka yerde kullanmaz
;    (kanal adları yalnızca A-Z 0-9). Diğer karakterler orijinal yoldan devam eder.
;
;    Segmentler (0x2C9C tablosundan): bit 3 üst, 8 alt, 1/2 sağ dikeyler, 9/10 sol dikeyler,
;    6/13 orta çizgi.  '4' = $2446 (sağ dikeyler, sol üst, orta);  y = '4' + alt = $2546.
;    Ekran ters (GOSTERGE TERS, 0x023C bit 8) seçiliyken F_00828C başka bir yoldan (0x2D0A
;    tablosu) gider; o yolda 'y' boş görünür. Açılışta ekran normaldir.

Y_SEGMENT   .EQU $2546
ACILIS_MS   .EQU 5000
F_ISIK      .EQU $008156        ; Y = 1 arka ışıklar açık, 0 kapalı (tip A / B çıkışları)

; ---------------------------------------------------------------------------------------------
; Orijinal koda iki atlama
        .ORG $00E0E1                    ; F_00E0E1: LDA A,$04,S / PHD / TAD
        .EXPECT $A3,$04,$0B,$5B
        .ASSUME M=0,X=0
        JMPL  ACILIS

        .ORG $00854C                    ; F_00828C: CMP A,#$5B / BCS L_008575 (harf mi?)
        .EXPECT $C9,$5B,$B0,$25
        .ASSUME M=1,X=0
        JMPL  Y_DENETIM

; ---------------------------------------------------------------------------------------------
        .ORG $012000

; Açılış ekranı. F_00E0E1 yerine çalışır: M = 0, X = 0, 4,S = çağıranın çerçeve göstericisi.
ACILIS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA                             ; çağırdığımız NC77 fonksiyonları için çerçeve
        LDY   #$0001                    ; arka ışıklar açık
        JSRL  F_ISIK
        LDY   #$0023                    ; F_00E0E1'in açılış bip sesi
        JSRL  $003310
        LDX   #$0000
AC_YAZ: SEM
        .ASSUME M=1,X=0
        LDA   A,>ACILIS_METNI,X
        STA   A,LCD,X
        CLM
        .ASSUME M=0,X=0
        INX
        CPX   #$0008
        BNE   AC_YAZ
        JSRL  F_LCD
        LDY   #ACILIS_MS
        JSRL  F_DELAY                   ; bekler, bekçi köpeğini besler
        PLA
        RTL

ACILIS_METNI:
        .TEXT8 "yATOLYE"

; F_00828C'nin harf denetimi. Gelişte M = 1, X = 0, A = karakter, $00 = konum (3..10).
Y_DENETIM:
        .ASSUME M=1,X=0
        CMP   A,#$79                    ; 'y'
        BEQ   Y_CIZ
        CMP   A,#$5B                    ; orijinal: CMP A,#$5B / BCS L_008575
        BCC   Y_HARF_MI
        JMPL  $008575                   ; harf değil: rakam / işaret denetimleri
Y_HARF_MI:
        JMPL  $008550                   ; orijinal: CMP A,#$41 ...
Y_CIZ:  CLM                             ; harf yolundaki gibi: $02 + 2*konum = segment sözcüğü
        .ASSUME M=0,X=0
        LDA   A,$00
        AND   A,#$00FF
        ASL   A
        TAX
        LDA   A,#Y_SEGMENT
        STA   A,$02,X
        JMPL  $0086F5                   ; sonraki karaktere (orijinal yol da M = 0 ile gelir)
