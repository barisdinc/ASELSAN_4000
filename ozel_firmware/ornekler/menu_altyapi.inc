; menu_altyapi.inc - firmware menüsüne yeni öğe ekleme altyapısı (YA4000S ve aynı derlemeler)
;
; Kullanım (bkz. sayi_ters.asm, hesap.asm):
;       .INCLUDE "menu_altyapi.inc"
;       .ORG $011000
;   MENU_NEXT:  (N_OLD + yeni) bayt     m tablosu
;   MENU_S:     4*(N_OLD + yeni) bayt   s tablosu [sonraki][işleyici 24 bit]
;   TITLES_EN:  8*(N_OLD + yeni) bayt   başlıklar
;   TITLES_TR:  8*(N_OLD + yeni) bayt
;       .INCLUDE "ornek.inc"           işleyiciler
;
; ---------------------------------------------------------------------------------------------
; Firmware'in menü düzeni (YA4000S, disasm/tus_takimi.md):
;   RAM $0227      menü durumu (0 = normal ekran)
;   ROM $3188      m tablosu: sonraki[durum]                               (20 durum, 0..$13)
;   ROM $319C      s tablosu: 4 bayt [sonraki durum][işleyici lo hi bank]
;   ROM $2DE6      durum başlıkları, 8 karakter (Türkçe $2E86, EE[0x018] bit 7)
;   RAM $02DF/$02E1  geçerli işleyici (24 bit). F_005471.. her tuşta m/s/p'yi işler, sonra
;                  işleyiciyi çağırır (F_01E4C2 ile), ardından F_00828C ekrana yazar.
;   Bir durumda sonraki[d] == d ise o durum "etkin" durumdur: m ve s'yi işleyici kendisi yorumlar.
;
; Tablolar tam dolu ve ardından başka veri geliyor; yeni durum eklemek için tabloları bank 1'e
; kopyalayıp (yeni durumlar $14, $15, ... eklenmiş olarak) okuyan komutların adreslerini değiştiriyoruz.
;
; Tuş: F_0050DA tuşu RAM $0226'ya ASCII olarak yazar (yoksa $0F). Rakamları ayrıca 16 baytlık
; halka tampona koyar: $0283[$0293 & 15] = rakam, $0293++ ; $0226 = 'd'.
; Biz sayacı en çok 8'de tuttuğumuz için haneler her zaman $0283..$028A'da sırayla durur.
; Tampondaki son bayt ($0292) bu modda hiç yazılmaz; "ters göster" bayrağı olarak kullanıyoruz.
; ---------------------------------------------------------------------------------------------

; Örnekler için boş RAM: $04ED..$0629 (bss $0082..$04EC'den sonra, NC77 çerçeve yığını $062A'dan
; yukarı). Kodda bu aralığa bellek erişimi yok; simülatörde hiçbir senaryoda yazılmadı
; (sim/ram_kullanim.js). Açılışta sıfırlanmaz: her örnek kendi değişkenlerini girişte kurar.
; ---------------------------------------------------------------------------------------------

KEY         .EQU $0226          ; son tuş (ASCII), $0F = yok; rakamlarda 'd'
MENU        .EQU $0227          ; menü durumu
DIGITS      .EQU $0283          ; firmware'in rakam tamponu (16 bayt, halka)
NDIGIT      .EQU $0293          ; tampondaki rakam sayısı
HANDLER     .EQU $02DF          ; geçerli işleyici, 16 bit adres
HANDLER_BK  .EQU $02E1          ;                   bank
LCD         .EQU $0252          ; 8 karakterlik ekran tamponu
OPTIONS     .EQU $0340          ; EE[0x018] kopyası; bit 7 = Türkçe menü
OZEL_RAM    .EQU $0500          ; örneklerin değişkenleri ($04ED..$0629 boş)

F_BEEP      .EQU $0080AA        ; Y = ses numarası (13 = hata), ALERT kapalıysa sessiz
F_LCD       .EQU $00828C        ; LCD tamponunu ekrana yaz (işleyiciden sonra firmware de çağırır)
F_DELAY     .EQU $007D7F        ; Y = milisaniye bekle (TB2 ile), bekçi köpeğini besler
; Bunlar NC77 fonksiyonları: yığının tepesinde (çağrı anında 4,S) çerçeve göstericisi ister.
; İşleyicinin en üst seviyesinde girişte PHA ile konan değer budur. JSR ile çağrılan bir alt
; yordamdan çağırmak için önce onu yeniden yığına koyun:  LDA A,$03,S / PHA ... PLA / RTS

TA0         .EQU $0046          ; bip zamanlayıcısı (yeniden yükleme = 2016000 / f)
TB2         .EQU $0054          ; 1 ms sistem zamanlayıcısı, sürekli sayar
BEEP_SURE   .EQU $0214          ; bip süresi, 10 ms'lik kesmede geri sayılır; 0'da ses kesilir

N_OLD       .EQU 20             ; orijinal durum sayısı (0..$13)

; =============================================================================================
; 1) Orijinal koddaki tablo adreslerini bank 1'deki kopyalara çevir
; =============================================================================================
; m tablosunu okuyan üç komut:  LDA A,$003188,X
        .ORG $005591
        .EXPECT $BF,$88,$31,$00
        .ASSUME M=0,X=0
        LDA   A,>MENU_NEXT,X
        .ORG $005641
        .EXPECT $BF,$88,$31,$00
        LDA   A,>MENU_NEXT,X
        .ORG $0056B9
        .EXPECT $BF,$88,$31,$00
        LDA   A,>MENU_NEXT,X

; s tablosu, bank baytı (X = 4*durum + 1):  LDA A,$00319E,X
        .ORG $005664
        .EXPECT $BF,$9E,$31,$00
        LDA   A,>MENU_S+2,X
        .ORG $005693
        .EXPECT $BF,$9E,$31,$00
        LDA   A,>MENU_S+2,X
        .ORG $0056CE
        .EXPECT $BF,$9E,$31,$00
        LDA   A,>MENU_S+2,X

; s tablosu, adres sözcüğü (X = 4*durum + 1) ya da sonraki durum (X = 4*durum):  LDA A,$00319C,X
        .ORG $00566D
        .EXPECT $BF,$9C,$31,$00
        LDA   A,>MENU_S,X
        .ORG $00569C
        .EXPECT $BF,$9C,$31,$00
        LDA   A,>MENU_S,X
        .ORG $0056AC
        .EXPECT $BF,$9C,$31,$00
        LDA   A,>MENU_S,X
        .ORG $0056D7
        .EXPECT $BF,$9C,$31,$00
        LDA   A,>MENU_S,X

; Başlık metinleri (F_003310): ADC A,#tablo (M=0) ...  LDM #bank,$05 (M=1)  (uzun gösterici $03..$05)
; Y = $2B yolu (menü başlığı)
        .ORG $0033C4
        .ASSUME M=0,X=0
        .EXPECT $69,$86,$2E
        ADC   A,#TITLES_TR & $FFFF
        .ORG $0033CA
        .ASSUME M=1,X=0
        .EXPECT $64,$05,$00
        LDM   #bank(TITLES_TR),$05
        .ORG $0033DA
        .ASSUME M=0,X=0
        .EXPECT $69,$E6,$2D
        ADC   A,#TITLES_EN & $FFFF
        .ORG $0033E0
        .ASSUME M=1,X=0
        .EXPECT $64,$05,$00
        LDM   #bank(TITLES_EN),$05
; Y = $2C yolu (normal ekran, kanal adı yoksa durum başlığı)
        .ORG $00348A
        .ASSUME M=0,X=0
        .EXPECT $69,$86,$2E
        ADC   A,#TITLES_TR & $FFFF
        .ORG $003490
        .ASSUME M=1,X=0
        .EXPECT $64,$05,$00
        LDM   #bank(TITLES_TR),$05
        .ORG $0034A0
        .ASSUME M=0,X=0
        .EXPECT $69,$E6,$2D
        ADC   A,#TITLES_EN & $FFFF
        .ORG $0034A6
        .ASSUME M=1,X=0
        .EXPECT $64,$05,$00
        LDM   #bank(TITLES_EN),$05

        .ASSUME M=0,X=0
