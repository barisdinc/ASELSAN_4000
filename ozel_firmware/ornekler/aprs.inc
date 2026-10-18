; aprs.inc - APRS menü öğesi: PTT'ye basınca 144,800 MHz'de konum paketi (AX.25 / 1200 baud AFSK)
;
;   ... m (APRS) s     -> "APRS PTT"    kanal 50..54'ün adları EEPROM'dan okunup paket kurulur;
;                                       telsiz alma ve gönderme 144,800 MHz'e geçer (TKS kapalı),
;                                       hoparlör tamamen susar (kanal sesi, bip ve uyarı tonları yok)
;   PTT                -> "APRS TX"     paket gider (~0,75 s), ardından "APRS OK"; PTT basılı
;                                       tutulursa telsiz 144,800'de göndermeye devam eder
;   m                  -> çık: telsiz önceki kanalına / frekansına döner
;
; Ayarlar kanal adlarında (8 karakter; telsiz ve programlayıcı A-Z 0-9 boşluk _ - / kabul eder):
;   kanal 50  çağrı işareti ve SSID      "TA7W-7"    -> kaynak adresi TA7W, SSID 7
;   kanal 51  enlem  DDMM mm + N/S       "4050 63N"  -> 4050.63N  (boşluk = ondalık nokta)
;   kanal 52  boylam DDDMM mm + E/W      "3109 39E"  -> 03109.39E (noktadan önce 4 hane: başa 0)
;   kanal 53  yorum (mesaj)              "ASELSAN"   -> sondaki boşluklar atılır
;   kanal 54  simge: tablo + kod         "/B"        -> /b (koddaki harf küçük harfe çevrilir;
;                                                       tek karakterse tablo '/'; boşsa /b)
;   kanal 55  otomatik gönderim aralığı, saniye   "60"  -> APRS menüsündeyken PTT'ye basılmasa
;             da her 60 saniyede bir paket gider (boş ya da 0: kapalı; en kısa 10 s)
; Paket: <50>>APIZCI,WIDE1-1,WIDE2-1:!<51><tablo><52><kod><53>
;   örnek: TA7W-7>APIZCI,WIDE1-1,WIDE2-1:!4050.63N/03109.39EbASELSAN
; Ad geçersizse ekranda "CAGRI -" / "ENLEM -" / "BOYLAM -" (İngilizce "CALL -", "LAT -", "LON -")
; ve paket gönderilmez. Adlar her APRS'e girişte yeniden okunur.
;
; Ton üretimi seçmeli çağrının donanım yolunu kullanır (F_00E65B ile aynı ayarlar):
;   TB1 zamanlayıcı modunda örnekleme saati: 4,032 MHz / 420 = 9600 Hz, kesme önceliği 6
;   TA1 tek atım modunda (TA1MOD = 6): her TB1 kesmesinde tetiklenir, darbe genişliği = örnek
;   çıkış P52 SÇ_TON -> TD14 süzgeci -> FX806 modülatör girişi
;   FX806 ayarı F_00E65B'nin başındaki gibi ($029A / $029B, F_00CB52): mikrofon kapalı, ton açık
; Örnek: 16 bit faz sayacı, artım 1200 Hz = 8192, 2200 Hz = 15019 (9600 Hz örneklemede);
; sinüs tablosu ROM $28FA (seçmeli çağrınınki, 256 kelime, 114..314). Bit başına 8 örnek.
; Kesme her 8 örnekte bir sonraki biti üretir: 45 bayrak (300 ms), çerçeve baytları LSB önce
; (art arda beş 1'den sonra 0 eklenir), 3 bayrak; NRZI (0 = ton değişir).
;
; Bağlantılar:
;   0x002028 VEC_TB1   JMPL $01C627 -> AFSK_TB1 (paket gitmiyorsa orijinal kesmeye geçer)
;   0x00EDA2 durum 4'ün kararı (kimlik gönderilecekse 5, yoksa doğrudan 8 = konuşma): APRS
;            menüsünde paket hazırsa her zaman 5, gecikme ~130 ms ($022A = AP_GECIKME)
;   0x00F4E4 durum 5 (PTT'den sonra verici açıldı) JSRL F_00E65B (seçmeli çağrı kimliği)
;            -> APRS_PTT: APRS menüsündeyse paketi gönderir; değilse hiçbir şey (seçmeli
;            çağrı kimliği gönderimi kaldırıldı)
;   0x00E9FE F_00E9D2'nin hoparlörü açtığı yer (P46 SUSTURMA_BILGISI = 1): APRS menüsündeyken
;            hoparlör kapalı kalır (kanaldaki paketler kullanıcıyı rahatsız etmesin); uyarı ve tuş
;            sesleri (P50) etkilenmez. APRS'e girerken hoparlör hemen kapatılır, çıkışta
;            RX'e yeniden girilince firmware susturmaya göre yeniden açar
;   0x00CC44 / 0x00CC92 F_00CB52 (FX806 ses ayarı): APRS menüsündeyken ses her zaman kapalı
;            sayılır ($00 = 1 -> RAM 0x0240 bit 0, tip A'da ses yükselteci kapalı) ve FX806'ya
;            squelch kapalıyken yazılan ayar (L_00CD3F) yazılır. Tip B kartta hoparlörü asıl bu
;            keser (P46 değil). APRS'e girişte F_00CB52 bir kez çağrılır.
;   0x0080AA F_0080AA (bip) ve 0x008084 F_008084 (uyarı tonu): APRS menüsündeyken hiç ses
;            çalınmaz (TA0 / P50 başlatılmaz). Böylece APRS'te hoparlör tamamen sessiz.
;   VFO_KANCA (frekans.inc) her turda APRS_KANCA'yı çağırır: menü zaman aşımını yeniler,
;            menüden herhangi bir yolla çıkılınca sentezör kaydını geri yükler; otomatik
;            gönderim zamanı gelince PTT isteği kurar ($0244 bit 2: F_005E56 bunu PTT gibi
;            işler, verici açılır, durum 4 -> 5 -> APRS_PTT paketi gönderir ve isteği siler)

AP_FAZ      .EQU OZEL_RAM+$A0   ; 2 bayt: DDS faz sayacı
AP_ARTIM    .EQU OZEL_RAM+$A2   ; 2 bayt: faz artımı (ton)
AP_ORNEK    .EQU OZEL_RAM+$A6   ; bitin kalan örnek sayısı
AP_BAYT     .EQU OZEL_RAM+$A7   ; gönderilen bayt (sağa kaydırılarak okunur)
AP_GONDER   .EQU OZEL_RAM+$A8   ; 2 bayt: bit 0 = kesme ton üretiyor
AP_DURUM    .EQU OZEL_RAM+$AA   ; bit 0 = APRS frekansı kurulu (çıkışta geri yükle),
                                ; bit 1 = paket gitti, bit 2 = hata (AP_HATA_KOD)
AP_YEDEK    .EQU OZEL_RAM+$AB   ; 13 bayt: önceki sentezör kaydı ($0370..$037C)
AP_KAYIT    .EQU OZEL_RAM+$B8   ; 13 bayt: APRS'in kurduğu kayıt (çıkışta karşılaştırmak için)
AP_029A     .EQU OZEL_RAM+$C5   ; 2 bayt: FX806 ayar kopyasının yedeği
AP_UZ       .EQU OZEL_RAM+$C7   ; 2 bayt: çerçeve uzunluğu (FCS dahil)
AP_PTR      .EQU OZEL_RAM+$C9   ; 2 bayt: kesmede sıradaki çerçeve baytı
AP_ASAMA    .EQU OZEL_RAM+$CB   ; 0 baş bayrakları, 1 çerçeve, 2 son bayraklar
AP_SAYAC    .EQU OZEL_RAM+$CC   ; kalan bayrak sayısı
AP_BITSAY   .EQU OZEL_RAM+$CD   ; AP_BAYT'ta kalan bit
AP_BIRLER   .EQU OZEL_RAM+$CE   ; art arda 1 sayısı (bit doldurma)
AP_TON      .EQU OZEL_RAM+$CF   ; 1 mark (1200 Hz), 0 space (2200 Hz)
AP_HATA_KOD .EQU OZEL_RAM+$D0   ; 1 bant, 2 çağrı işareti, 3 enlem, 4 boylam
AP_F        .EQU OZEL_RAM+$D1   ; 2 bayt: menü işleyicisinin çerçevesi (F_EE_OKU için)
AP_CRC      .EQU OZEL_RAM+$D3   ; 2 bayt
AP_SEMBOL   .EQU OZEL_RAM+$D5   ; 2 bayt: simge tablosu, simge kodu
AP_HSAY     .EQU OZEL_RAM+$D7   ; 2 bayt: konumda okunan rakam sayısı
AP_YARI     .EQU OZEL_RAM+$D9   ; konumdaki harf (N S E W)
AP_SAY      .EQU OZEL_RAM+$DA   ; çağrı işaretindeki karakter sayısı
AP_SSID     .EQU OZEL_RAM+$DB
AP_T1       .EQU OZEL_RAM+$DC
AP_T2       .EQU OZEL_RAM+$DD
AP_AD       .EQU OZEL_RAM+$DE   ; 8 bayt: EEPROM'dan okunan kanal adı
AP_HANE     .EQU OZEL_RAM+$E6   ; 7 bayt: konumun rakamları
AP_CER      .EQU OZEL_RAM+$ED   ; 60 bayt: AX.25 çerçevesi (en uzun 60 bayt) .. $0628
; otomatik gönderim (OZEL_RAM'den önceki boş alan, $04ED..$04FF)
AP_ARALIK   .EQU $04F0          ; 2 bayt: kanal 55'teki saniye (0 = kapalı)
AP_SANIYE   .EQU $04F2          ; 2 bayt: son gönderimden beri geçen saniye
AP_MS       .EQU $04F4          ; 2 bayt: saniyenin milisaniyesi (32 ms adım)
AP_SONTIK   .EQU $04F6          ; son görülen 32 ms sayacı ($0457)
AP_FARK     .EQU $04F7          ; geçen 32 ms adımı
AP_OTO      .EQU $04F8          ; 1 = otomatik gönderim için PTT isteği kuruldu
AP_OTO_SURE .EQU $04F9          ; isteğin kalan süresi (32 ms adım); verici açılmazsa bırakılır
TIK_32MS    .EQU $0457          ; TB2 kesmesi her 32 ms'de bir artırır
PTT_ISTEK   .EQU $0244          ; F_005E56: bit 0..5 PTT istekleri (0x20 P47, 0x10 panel ...)
F_PTT_APRS  .EQU $0004          ; firmware'de kullanılmayan istek biti: otomatik APRS gönderimi
EE_ARALIK   .EQU $2E+13*54+EE_AD    ; kanal 55
AP_EN_KISA  .EQU 10             ; en kısa otomatik gönderim aralığı (s)

F_AP_KURULU .EQU 1
F_AP_GITTI  .EQU 2
F_AP_HATA   .EQU 4

ARTIM_MARK  .EQU 8192           ; 1200 Hz
ARTIM_SPACE .EQU 15019          ; 2200 Hz
AP_GECIKME  .EQU 4              ; PTT'den paketin başına: 4 x 32 ms (verici ve sentezör otursun)
ON_BAYRAK   .EQU 45             ; 45 x 8 bit / 1200 baud = 300 ms
SON_BAYRAK  .EQU 3
TB1_9600    .EQU 419            ; 4032000 / (419 + 1) = 9600 Hz
SINUS       .EQU $0028FA

EE_AD       .EQU 5              ; kanal kaydında adın yeri; kayıt 0x02E + 13 * (n - 1)
EE_CAGRI    .EQU $2E+13*49+EE_AD    ; kanal 50
EE_ENLEM    .EQU $2E+13*50+EE_AD    ; kanal 51
EE_BOYLAM   .EQU $2E+13*51+EE_AD    ; kanal 52
EE_MESAJ    .EQU $2E+13*52+EE_AD    ; kanal 53
EE_SEMBOL   .EQU $2E+13*53+EE_AD    ; kanal 54

R_P4        .EQU $000A          ; P46 SUSTURMA_BILGISI: 1 = hoparlör açık
R_TCSTART   .EQU $0040          ; sayıcı başlatma (bit 1 TA1, bit 6 TB1)
R_TOSTART   .EQU $0042          ; tek atım tetikleme (bit 1 TA1)
R_TA1       .EQU $0048
R_TB1       .EQU $0052
R_TA1MOD    .EQU $0057
R_TB1MOD    .EQU $005C
R_WDT       .EQU $0060
R_TA1IC     .EQU $0076
R_TB1IC     .EQU $007B
TB1_ORIJINAL .EQU $01C627
F_FX806     .EQU $00CB52        ; $029A / $029B'deki ayarı FX806'ya yazar
F_EE_OKU    .EQU $0072EE        ; EEPROM'dan oku. Y = $A0; çerçeve: +0 sayı, +1..+3 hedef (24 bit),
                                ; +4..+5 EEPROM adresi; çerçevenin geri kalanını yerel olarak kullanır

; ---------------------------------------------------------------------------------------------
AP_KOD_ADRES .EQU *
        .ORG $002028                    ; VEC_TB1
        .EXPECT $5C,$27,$C6,$01
        JMPL  AFSK_TB1
        .ORG $00EDA2                    ; durum 4: PTT'de kimlik gönderilecek mi? (5 ya da 8)
        .EXPECT $2C,$44,$02,$02,$00,$1C ; BBS #$0002,$0244,L_00EDC4
        .ASSUME M=0,X=0
        JMPL  APRS_KARAR
        .BYTE $EA,$EA                   ; (NOP, çalışmaz)
        .ORG $00E9FE                    ; F_00E9D2: susturma açıldı -> hoparlörü aç (P46 = 1)
        .EXPECT $F8,$0C,$0A,$00,$40,$4C,$0B,$EA   ; SEM / SEB #$40,P4 / JMP L_00EA0B
        JMPL  APRS_HOPARLOR
        .BYTE $EA,$EA,$EA,$EA           ; (NOP, çalışmaz)
        .ORG $0080AA                    ; F_0080AA (tuş / onay / hata bipleri)
        .EXPECT $A3,$04,$0B,$5B         ; LDA A,$04,S / PHD / TAD
        JMPL  APRS_BIP
        .ORG $008084                    ; F_008084 (uyarı tonu aç: TA0 -> P50)
        .EXPECT $A3,$04,$48,$F8         ; LDA A,$04,S / PHA / SEM
        JMPL  APRS_UYARI
        .ORG $00CC44                    ; F_00CB52: ses kapalı mı ($00) kararı -> APRS'te hep kapalı
        .EXPECT $D8,$A5,$00,$F8         ; CLM / LDA A,$00 / SEM
        JMPL  APRS_SUS_KARAR
        .ORG $00CC92                    ; F_00CB52: squelch kapalı FX806 ayarına (L_00CD3F) gidilsin mi
        .EXPECT $A5,$00,$F0,$0F         ; LDA A,$00 / BEQ L_00CCA5
        JMPL  APRS_SUS_FX806
        .ORG $00F4E4                    ; durum 5 -> 6: seçmeli çağrı kimliği yerine APRS
        .EXPECT $22,$5B,$E6,$00
        JSRL  APRS_PTT
        .ORG AP_KOD_ADRES

; ---------------------------------------------------------------------------------------------
; TB1 kesmesi. Giriş durumu bilinmiyor: orijinal kesme gibi CLP #$39 / PSH #$35 / LDT #0.
AFSK_TB1:
        CLP   #$39
        .ASSUME M=0,X=0
        PSH   #$35
        LDT   #$00
        BBS   #$0001,AP_GONDER,AT_URET
        PUL   #$35
        JMPL  TB1_ORIJINAL              ; seçmeli çağrı / DTMF / alma: orijinal kesme
AT_URET:
        SEM
        SEB   #$02,R_TOSTART            ; TA1 darbesi (genişliği bir önceki örnek)
        CLM
        LDA   A,AP_FAZ
        CLC
        ADC   A,AP_ARTIM
        STA   A,AP_FAZ
        LDA   A,AP_FAZ+1                ; üst bayt -> tablo indeksi (kelime)
        AND   A,#$00FF
        ASL   A
        TAX
        LDA   A,>SINUS,X
        STA   A,R_TA1
        SEM
        DEC   AP_ORNEK
        BEQ   AT_SONRAKI
        JMP   AT_SON
AT_SONRAKI:
        LDM   #8,AP_ORNEK               ; sıradaki bit
        LDA   A,AP_ASAMA
        CMP   A,#1
        BEQ   AT_VERI
; bayrak (aşama 0 ya da 2)
AT_BAYRAK:
        LDA   A,AP_BITSAY
        BNE   AB_1
        LDA   A,AP_SAYAC
        BEQ   AB_ASAMA_SONU
        DEC   AP_SAYAC
        LDM   #$7E,AP_BAYT
        LDM   #8,AP_BITSAY
AB_1:   DEC   AP_BITSAY
        LSR   AP_BAYT                   ; C = bit (doldurma yok)
        BRA   AT_NRZI
AB_ASAMA_SONU:
        LDA   A,AP_ASAMA
        BNE   AT_BITTI                  ; son bayraklar bitti
        LDM   #1,AP_ASAMA               ; çerçeveye geç
        LDM   #0,AP_BIRLER
        LDM   #0,AP_BITSAY
        LDX   #0
        STX   AP_PTR
; çerçeve (aşama 1)
AT_VERI:
        LDA   A,AP_BIRLER
        CMP   A,#5
        BNE   AV_1
        LDM   #0,AP_BIRLER              ; beş 1'den sonra doldurma biti 0
        CLC
        BRA   AT_NRZI
AV_1:   LDA   A,AP_BITSAY
        BNE   AV_2
        LDX   AP_PTR                    ; sıradaki bayt
        CPX   AP_UZ
        BCS   AV_SON
        LDA   A,AP_CER,X
        STA   A,AP_BAYT
        INX
        STX   AP_PTR
        LDM   #8,AP_BITSAY
AV_2:   DEC   AP_BITSAY
        LSR   AP_BAYT
        BCC   AV_SIFIR
        INC   AP_BIRLER
        SEC
        BRA   AT_NRZI
AV_SIFIR:
        LDM   #0,AP_BIRLER
        CLC
        BRA   AT_NRZI
AV_SON: LDM   #2,AP_ASAMA               ; son bayraklar
        LDM   #SON_BAYRAK,AP_SAYAC
        LDM   #0,AP_BITSAY
        BRA   AT_BAYRAK
; NRZI: C = bit; 0 ise ton değişir
AT_NRZI:
        BCS   AN_1
        LDA   A,AP_TON
        EOR   A,#1
        STA   A,AP_TON
AN_1:   LDA   A,AP_TON
        CLM
        BEQ   AN_SPACE
        LDA   A,#ARTIM_MARK
        BRA   AN_YAZ
AN_SPACE:
        LDA   A,#ARTIM_SPACE
AN_YAZ: STA   A,AP_ARTIM
        SEM
        BRA   AT_SON
AT_BITTI:
        LDM   #0,AP_GONDER              ; paket bitti (ana döngü bekliyor)
AT_SON: CLM
        PUL   #$35
        RTI

; ---------------------------------------------------------------------------------------------
; Durum 4'ün kararı (F_00ED53, M = 0, D = onun çerçevesi): APRS menüsündeyse kimlik yolu (5)
APRS_KARAR:
        .ASSUME M=0,X=0
        SEM
        JSR   AP_HAZIR
        BNE   AKR_ORJ
        LDM   #5,RADYO_DURUM            ; durum 5; gecikme EEPROM'daki ton gönderme gecikmesi
        LDM   #AP_GECIKME,$022A         ; değil (0x342, 1,5 s'ye kadar): paketin kendi 300 ms
        JMPL  $00EDF0                   ; bayrağı var. Sonra L_00EDC4'ün devamı
AKR_ORJ:
        CLM                             ; orijinal: BBS #$0002,$0244,L_00EDC4
        BBS   #$0002,$0244,AKR_5
        JMPL  $00EDA8
AKR_5:  JMPL  $00EDC4

; ---------------------------------------------------------------------------------------------
; F_00E9D2'nin hoparlörü açtığı yer (M = 1): APRS menüsündeyse kapalı tut
APRS_HOPARLOR:
        .ASSUME M=1,X=0
        SEM
        JSR   AP_ETKIN
        BNE   AH_AC
        CLB   #$40,R_P4                 ; APRS: hoparlör kapalı
        JMPL  $00EA0E
AH_AC:  SEB   #$40,R_P4                 ; orijinal
        JMPL  $00EA0E

; F_00CB52, L_00CC44 (M bilinmiyor, D = F_00CB52'nin çerçevesi; $00 = 1 ses kapalı)
APRS_SUS_KARAR:
        SEM
        .ASSUME M=1,X=0
        JSR   AP_ETKIN
        BNE   SSK_1
        LDM   #1,$00                    ; APRS: ses kapalı
SSK_1:  CLM                             ; orijinal: CLM / LDA A,$00 / SEM
        LDA   A,$00
        SEM
        JMPL  $00CC48

; F_00CB52, L_00CC92 (M = 1): APRS'teyse squelch açık olsa da sessiz FX806 ayarı
APRS_SUS_FX806:
        .ASSUME M=1,X=0
        JSR   AP_ETKIN
        BNE   SSF_1
        JMPL  $00CD3F
SSF_1:  LDA   A,$00                     ; orijinal: LDA A,$00 / BEQ L_00CCA5
        BNE   SSF_2
        JMPL  $00CCA5
SSF_2:  JMPL  $00CC96

; Bip (F_0080AA girişi, JMPL ile: yığın girişteki gibi): APRS'te sessiz döner
APRS_BIP:
        .ASSUME M=0,X=0
        PHP
        SEM
        JSR   AP_ETKIN
        BEQ   ABP_SUS
        PLP
        LDA   A,$04,S                   ; orijinal: LDA A,$04,S / PHD / TAD
        PHD
        TAD
        JMPL  $0080AE
ABP_SUS:
        PLP
        RTL

; Uyarı tonu (F_008084 girişi; TB2 kesmesinden de çağrılır): APRS'te başlatma
APRS_UYARI:
        .ASSUME M=0,X=0
        PHP
        SEM
        JSR   AP_ETKIN
        BEQ   AUY_SUS
        PLP
        LDA   A,$04,S                   ; orijinal: LDA A,$04,S / PHA / SEM (JMPL bu üçünü
        PHA                             ; kapladı), sonra 0x008088'den devam
        SEM
        JMPL  $008088
AUY_SUS:
        PLP
        RTL

; APRS menüsü açık ve frekans kurulu mu? Z = 1: evet (M = 1 giriş ve çıkış, A bozulur)
AP_ETKIN:
        .ASSUME M=1,X=0
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AE_SON
        LDA   A,AP_DURUM
        AND   A,#F_AP_KURULU
        EOR   A,#F_AP_KURULU            ; kuruluysa 0
AE_SON: RTS

; Üstelik paket hazır mı (hata yok)? Z = 1: gönderilebilir (M = 1, A bozulur)
AP_HAZIR:
        .ASSUME M=1,X=0
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AHZ_SON
        LDA   A,AP_DURUM
        AND   A,#F_AP_KURULU|F_AP_HATA
        EOR   A,#F_AP_KURULU
AHZ_SON:
        RTS

; ---------------------------------------------------------------------------------------------
; Durum 5'te (verici açıldı, PTT basılı) telsiz durum makinesinden çağrılır (M = 0, 4,S çerçeve).
APRS_PTT:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
        SEM
        JSR   AP_HAZIR
        BNE   AP_YOK
        CLM
        LDX   #0                        ; ekranda "APRS TX"
        JSR   AP_METIN_YAZ
        JSRL  F_LCD
        CLM
        JSR   AFSK_GONDER
        CLB   #F_PTT_APRS,PTT_ISTEK     ; otomatik gönderimse PTT isteğini bırak: verici kapanır
        LDM   #0,AP_SANIYE              ; aralık yeniden başlar
        LDM   #0,AP_MS
        SEM
        LDM   #0,AP_OTO
        SEB   #F_AP_GITTI,AP_DURUM
        CLM
        LDX   #8                        ; "APRS OK"
        JSR   AP_METIN_YAZ
        JSRL  F_LCD
AP_YOK: CLM
        PLA
        RTL

; Paketi gönder ve bitmesini bekle (M = 0). Çağıranın çerçevesi 3,S'de: F_FX806 için yığının
; tepesine yeniden konur; F_FX806'ya kadar yığına başka bir şey konmamalı.
AFSK_GONDER:
        .ASSUME M=0,X=0
        LDA   A,$03,S                   ; APRS_PTT'nin koyduğu çerçeve (JSR 2 bayt)
        PHA
        CLB   #$0800,$023E              ; FX806: F_00E65B'deki gibi ton yolu açık, mikrofon kapalı
        LDX   $029A                     ; (yığına değil: F_FX806 yığının tepesini çerçeve sanar)
        STX   AP_029A
        SEB   #$8000,$029A
        CLB   #$4000,$029A
        SEM
        LDA   A,$029B
        AND   A,#$F8
        ORA   A,#$06
        STA   A,$029B
        CLM
        JSRL  F_FX806
        CLM
        LDX   AP_029A
        STX   $029A
        SEM
        LDM   #$00,R_TA1IC              ; TA1 tek atım, kesmesiz
        LDM   #$06,R_TA1MOD
        LDM   #$00,R_TB1MOD             ; TB1 zamanlayıcı, f2
        CLM
        LDA   A,#TB1_9600
        STA   A,R_TB1
        LDA   A,>SINUS
        STA   A,R_TA1
        LDM   #0,AP_FAZ
        LDM   #ARTIM_MARK,AP_ARTIM
        SEM
        LDM   #0,AP_ASAMA               ; baş bayrakları
        LDM   #ON_BAYRAK,AP_SAYAC
        LDM   #0,AP_BITSAY
        LDM   #1,AP_TON                 ; mark ile başla
        LDM   #1,AP_ORNEK               ; ilk kesmede ilk bite geçilir
        CLM
        LDM   #1,AP_GONDER
        SEM
        LDA   A,#$01                    ; IPL = 1 (F_00E65B gibi)
        PHP
        STA   A,$02,S
        PLP
        SEM
        LDM   #$06,R_TB1IC              ; TB1 kesmesi, öncelik 6
        SEB   #$02,R_TCSTART            ; TA1 ve TB1 başlasın
        SEB   #$40,R_TCSTART
        SEB   #$02,R_TOSTART
AG_BEKLE:
        LDM   #$00,R_WDT                ; bekçi köpeği
        CLM
        BBS   #$0001,AP_GONDER,AG_BEKLE2
        BRA   AG_BITTI
AG_BEKLE2:
        SEM
        BRA   AG_BEKLE
AG_BITTI:
        SEM
        CLB   #$02,R_TCSTART
        CLB   #$40,R_TCSTART
        LDM   #$00,R_TB1IC
        LDA   A,#$00                    ; IPL = 0
        PHP
        STA   A,$02,S
        PLP
        CLM
        PLA
        RTS

; LCD = AP_METIN[X..X+7] (M = 0 giriş ve çıkış)
AP_METIN_YAZ:
        .ASSUME M=0,X=0
        LDY   #0
        SEM
AMY_1:  LDA   A,>AP_METIN,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   AMY_1
        CLM
        RTS

AP_METIN:
        .TEXT8 "APRS TX"                ; 0
        .TEXT8 "APRS OK"                ; 8
        .TEXT8 "APRS PTT"               ; 16
        .TEXT8 "ERROR"                  ; 24 + 16 * (hata - 1) İngilizce, +8 Türkçe
        .TEXT8 "HATA"
        .TEXT8 "CALL -"                 ; (LCD fontunda '?' yok)
        .TEXT8 "CAGRI -"
        .TEXT8 "LAT -"
        .TEXT8 "ENLEM -"
        .TEXT8 "LON -"
        .TEXT8 "BOYLAM -"

; ---------------------------------------------------------------------------------------------
; Menü: girişte 144,800'ü kur ve paketi kanal adlarından hazırla
APRS_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        STA   A,AP_F
        PHA
        LDM   #APRS_TUS & $FFFF,HANDLER
        SEM
        LDM   #bank(APRS_TUS),HANDLER_BK
        CLM
        JSR   AP_KUR
        SEM
        BBC   #F_AP_KURULU,AP_DURUM,AG_1
        CLM                             ; FX806 ses ayarını hemen yenile: APRS'te sessiz
        JSRL  F_FX806
        SEM
        CLM
        JSR   AP_HAZIRLA
        SEM
        BCC   AG_1
        SEB   #F_AP_HATA,AP_DURUM
AG_1:   CLM
        JMP   AP_EKRAN_SON

APRS_TUS:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        PHA
AP_EKRAN_SON:
        CLM
        LDX   #16                       ; "APRS PTT"
        SEM
        BBC   #F_AP_GITTI,AP_DURUM,AE_1
        LDX   #8                        ; "APRS OK"
AE_1:   BBC   #F_AP_HATA,AP_DURUM,AE_3
        CLM                             ; X = 24 + 16 * (kod - 1) (+8 Türkçe)
        LDA   A,AP_HATA_KOD
        AND   A,#$00FF
        DEC   A
        ASL   A
        ASL   A
        ASL   A
        ASL   A
        CLC
        ADC   A,#24
        TAX
        SEM
        BBC   #$80,OPTIONS,AE_3
        INX
        INX
        INX
        INX
        INX
        INX
        INX
        INX
AE_3:   CLM
        JSR   AP_METIN_YAZ
        PLA
        RTL

; Sentezör kaydını sakla, 144,800 MHz'i alma ve gönderme olarak kur, TKS'yi kapat (M = 0)
AP_KUR:
        .ASSUME M=0,X=0
        SEM
        LDM   #0,AP_DURUM
        LDX   #0
AK_1:   LDA   A,SYN_KAYIT,X
        STA   A,AP_YEDEK,X
        INX
        CPX   #13
        BNE   AK_1
        LDX   #0                        ; haneler "1448000"
AK_2:   LDA   A,>AP_HANELER,X
        STA   A,FR_HANE,X
        INX
        CPX   #7
        BNE   AK_2
        LDM   #7,FR_NDIG
        CLM
        JSR   FR_HESAPLA
        BCC   AK_GECERLI
        SEM
        LDM   #F_AP_HATA,AP_DURUM
        LDM   #1,AP_HATA_KOD            ; bant dışı
        CLM
        RTS
AK_GECERLI:
        LDA   A,RX_KAYMA
        JSR   FR_SENTEZ
        LDX   #0
        SEM
AK_3:   LDA   A,FR_KAYIT,X
        STA   A,SYN_KAYIT,X
        INX
        CPX   #5
        BNE   AK_3
        CLM
        LDA   A,#0
        JSR   FR_SENTEZ
        LDX   #0
        SEM
AK_4:   LDA   A,FR_KAYIT,X
        STA   A,SYN_KAYIT+7,X
        INX
        CPX   #5
        BNE   AK_4
        LDA   A,SYN_KAYIT+5             ; TKS kapalı (alt 6 bit); seçmeli çağrı biti (7) de
        AND   A,#$40
        STA   A,SYN_KAYIT+5
        LDA   A,SYN_KAYIT+12
        AND   A,#$C0
        STA   A,SYN_KAYIT+12
        LDX   #0
AK_5:   LDA   A,SYN_KAYIT,X             ; kopya ($0363) ve karşılaştırma için
        STA   A,SYN_YEDEK,X
        STA   A,AP_KAYIT,X
        INX
        CPX   #13
        BNE   AK_5
        LDM   #F_AP_KURULU,AP_DURUM
        CLB   #$40,R_P4                 ; hoparlör kapalı (APRS_HOPARLOR açık tutmaz)
        LDM   #1,RADYO_DURUM            ; RX'e yeniden gir, sentezörü yükle
        CLM
        RTS

AP_HANELER:
        .BYTE "1448000"

; ---------------------------------------------------------------------------------------------
; Paketi kanal adlarından kur: AP_CER / AP_UZ (M = 0 giriş ve çıkış). C = 1: hata (AP_HATA_KOD)
AP_HAZIRLA:
        .ASSUME M=0,X=0
        LDA   A,#EE_SEMBOL
        JSR   AP_AD_OKU
        SEM
        JSR   AP_SEMBOL_AL
        CLM
        LDA   A,#EE_CAGRI
        JSR   AP_AD_OKU
        LDY   #0                        ; Y = çerçevede yazma yeri (kopyalarken; sonra X)
        LDX   #0
        SEM
AHA_1:  LDA   A,>AP_HEDEF,X             ; hedef APIZCI (uzun adres yalnız X ile indekslenir)
        STA   A,AP_CER,Y
        INX
        INY
        CPX   #7
        BNE   AHA_1
        TYX
        JSR   AP_CAGRI_YAZ              ; kaynak
        BCC   AHA_2
        LDA   A,#2
        BRA   AHA_HATA
AHA_2:  TXY
        LDX   #0
AHA_3:  LDA   A,>AP_YOL,X               ; WIDE1-1, WIDE2-1, kontrol, PID, '!'
        STA   A,AP_CER,Y
        INX
        INY
        CPX   #17
        BNE   AHA_3
        TYX
        CLM
        LDA   A,#EE_ENLEM
        JSR   AP_AD_OKU
        SEM
        LDA   A,#4                      ; DDMM.mm
        LDY   #'N'+'S'*256
        JSR   AP_KONUM_YAZ
        BCC   AHA_4
        LDA   A,#3
        BRA   AHA_HATA
AHA_4:  LDA   A,AP_SEMBOL               ; simge tablosu
        STA   A,AP_CER,X
        INX
        CLM
        LDA   A,#EE_BOYLAM
        JSR   AP_AD_OKU
        SEM
        LDA   A,#5                      ; DDDMM.mm
        LDY   #'E'+'W'*256
        JSR   AP_KONUM_YAZ
        BCC   AHA_5
        LDA   A,#4
        BRA   AHA_HATA
AHA_5:  LDA   A,AP_SEMBOL+1             ; simge kodu
        STA   A,AP_CER,X
        INX
        CLM
        LDA   A,#EE_MESAJ
        JSR   AP_AD_OKU
        SEM
        JSR   AP_MESAJ_YAZ
        CLM
        STX   AP_UZ
        JSR   AP_FCS
        LDA   A,#EE_ARALIK              ; kanal 55: otomatik gönderim aralığı
        JSR   AP_AD_OKU
        JSR   AP_ARALIK_AL
        CLC
        RTS
AHA_HATA:
        .ASSUME M=1,X=0
        STA   A,AP_HATA_KOD
        CLM
        SEC
        RTS

AP_HEDEF:                               ; "APIZCI", SSID 0, C biti (komut) = 1
        .BYTE 'A'*2,'P'*2,'I'*2,'Z'*2,'C'*2,'I'*2,$E0
AP_YOL:                                 ; WIDE1-1, WIDE2-1 (son adres), UI, PID yok, '!'
        .BYTE 'W'*2,'I'*2,'D'*2,'E'*2,'1'*2,' '*2,$62
        .BYTE 'W'*2,'I'*2,'D'*2,'E'*2,'2'*2,' '*2,$63
        .BYTE $03,$F0,'!'

; EEPROM'dan 8 baytlık kanal adını AP_AD'ye oku (M = 0, A = EEPROM adresi; X, Y korunur).
; Bit 7 (TX TKS bitleri) atılır; kontrol karakterleri ve $7F boşluk olur.
AP_AD_OKU:
        .ASSUME M=0,X=0
        PHX
        PHY
        PHD
        TAY
        LDA   A,AP_F                    ; menü işleyicisinin boş çerçevesi: parametreler + yereller
        TAD
        STY   $04                       ; EEPROM adresi
        LDA   A,#AP_AD                  ; hedef (bank 0)
        STA   A,$01
        SEM
        LDA   A,#8
        STA   A,$00
        LDA   A,#0
        STA   A,$03
        CLM
        TDA
        PHA
        LDY   #$00A0
        JSRL  F_EE_OKU
        CLM
        PLA
        PLD
        LDX   #0
        SEM
AAO_1:  LDA   A,AP_AD,X
        AND   A,#$7F
        CMP   A,#$20
        BCC   AAO_B
        CMP   A,#$7F
        BNE   AAO_2
AAO_B:  LDA   A,#' '
AAO_2:  STA   A,AP_AD,X
        INX
        CPX   #8
        BNE   AAO_1
        CLM
        PLY
        PLX
        RTS

; Kanal 54: simge. İlk boşluk olmayan karakter tablo, ikincisi kod (harfse küçük harf).
; Tek karakterse tablo '/' ve o karakter kod; boşsa /b (bisiklet). (M = 1)
AP_SEMBOL_AL:
        .ASSUME M=1,X=0
        LDM   #'/',AP_SEMBOL
        LDM   #'b',AP_SEMBOL+1
        LDM   #0,AP_SAY
        LDY   #0
ASA_1:  CPY   #8
        BEQ   ASA_SON
        LDA   A,AP_AD,Y
        INY
        CMP   A,#' '
        BEQ   ASA_1
        STA   A,AP_T1
        LDA   A,AP_SAY
        BNE   ASA_2
        LDA   A,AP_T1                   ; ilk karakter: şimdilik kod
        STA   A,AP_SEMBOL+1
        LDM   #1,AP_SAY
        BRA   ASA_1
ASA_2:  CMP   A,#1                      ; ikinci: ilki tablo, bu kod (sonrakiler yok sayılır)
        BNE   ASA_1
        LDA   A,AP_SEMBOL+1
        STA   A,AP_SEMBOL
        LDA   A,AP_T1
        STA   A,AP_SEMBOL+1
        LDM   #2,AP_SAY
        BRA   ASA_1
ASA_SON:
        LDA   A,AP_SEMBOL+1             ; kod harfse küçük harf
        CMP   A,#'A'
        BCC   ASA_X
        CMP   A,#'Z'+1
        BCS   ASA_X
        ORA   A,#$20
        STA   A,AP_SEMBOL+1
ASA_X:  RTS

; Kanal 50: çağrı işareti (1..6 harf / rakam), isteğe bağlı -SSID (0..15) -> AX.25 adresi
; (M = 1, X = çerçevede yazma yeri, ilerler). C = 1: geçersiz
AP_CAGRI_YAZ:
        .ASSUME M=1,X=0
        LDM   #0,AP_SAY
        LDY   #0
ACY_1:  CPY   #8
        BEQ   ACY_DOLDUR
        LDA   A,AP_AD,Y
        CMP   A,#'-'
        BEQ   ACY_DOLDUR
        CMP   A,#' '
        BEQ   ACY_DOLDUR
        JSR   AP_HARF_RAKAM
        BCS   ACY_HATA
        ASL   A
        STA   A,AP_CER,X
        INX
        INY
        INC   AP_SAY
        LDA   A,AP_SAY
        CMP   A,#7
        BCS   ACY_HATA
        BRA   ACY_1
ACY_DOLDUR:
        LDA   A,AP_SAY
        BEQ   ACY_HATA
ACY_D1: LDA   A,AP_SAY                  ; 6 karaktere boşlukla tamamla
        CMP   A,#6
        BEQ   ACY_SSID
        LDA   A,#' '*2
        STA   A,AP_CER,X
        INX
        INC   AP_SAY
        BRA   ACY_D1
ACY_SSID:
        LDM   #0,AP_SSID
        CPY   #8
        BEQ   ACY_YAZ
        LDA   A,AP_AD,Y
        CMP   A,#'-'
        BNE   ACY_YAZ
        INY
        CPY   #8
        BEQ   ACY_HATA
        LDA   A,AP_AD,Y
        JSR   AP_RAKAM
        BCS   ACY_HATA
        STA   A,AP_SSID
        INY
        CPY   #8
        BEQ   ACY_YAZ
        LDA   A,AP_AD,Y
        JSR   AP_RAKAM
        BCS   ACY_YAZ                   ; tek haneli SSID
        STA   A,AP_T1                   ; SSID = 10 * ilk + ikinci
        LDA   A,AP_SSID
        ASL   A
        STA   A,AP_T2
        ASL   A
        ASL   A
        CLC
        ADC   A,AP_T2
        CLC
        ADC   A,AP_T1
        STA   A,AP_SSID
        CMP   A,#16
        BCS   ACY_HATA
ACY_YAZ:
        LDA   A,AP_SSID
        ASL   A
        ORA   A,#$60
        STA   A,AP_CER,X
        INX
        CLC
        RTS
ACY_HATA:
        SEC
        RTS

; A = karakter: A-Z ya da 0-9 ise C = 0 (A korunur) (M = 1)
AP_HARF_RAKAM:
        .ASSUME M=1,X=0
        CMP   A,#'0'
        BCC   AHR_H
        CMP   A,#'9'+1
        BCC   AHR_E
        CMP   A,#'A'
        BCC   AHR_H
        CMP   A,#'Z'+1
        BCC   AHR_E
AHR_H:  SEC
        RTS
AHR_E:  CLC
        RTS

; A = karakter: rakamsa A = değeri, C = 0 (M = 1)
AP_RAKAM:
        .ASSUME M=1,X=0
        SEC
        SBC   A,#'0'
        BCC   AR_H
        CMP   A,#10
        BCS   AR_H
        CLC
        RTS
AR_H:   SEC
        RTS

; Kanal 51 / 52: konum. A = noktadan önceki hane sayısı (4 enlem, 5 boylam),
; Y = izin verilen iki harf (alt bayt / üst bayt). Çerçeveye "DDMM.mmH" / "DDDMM.mmH" yazar.
; Rakamlar arasındaki boşluk ondalık noktadır (kanal adında nokta yazılamaz): noktadan sonra
; 1 ya da 2 hane (1 ise sona 0), boylamda noktadan önce 4 hane yazılmışsa başa 0.
; Boşluk yoksa noktanın yeri sabittir (son iki hane ondalık; boylamda 6 rakam ise başa 0).
; Baştaki / harften sonraki boşluklar ve - _ / atlanır.
; (M = 1, X = yazma yeri, ilerler). C = 1: geçersiz. AP_CRC harfleri, AP_SAY noktanın yerini
; ($FF = yok) geçici olarak tutar.
AP_KONUM_YAZ:
        .ASSUME M=1,X=0
        STA   A,AP_T2                   ; noktadan önceki hane
        STY   AP_CRC                    ; izin verilen harfler
        LDM   #0,AP_YARI
        LDM   #$FF,AP_SAY               ; nokta yok
        PHX
        LDX   #0
        STX   AP_HSAY
        LDY   #0
AKY_1:  CPY   #8
        BEQ   AKY_2
        LDA   A,AP_AD,Y
        INY
        CMP   A,#' '
        BEQ   AKY_BOSLUK
        CMP   A,#'0'                    ; - / atlanır
        BCC   AKY_1
        CMP   A,#'9'+1
        BCS   AKY_HARF
        LDX   AP_HSAY
        CPX   #7
        BCC   AKY_R
        JMP   AKY_HATA
AKY_R:  STA   A,AP_HANE,X
        INX
        STX   AP_HSAY
        BRA   AKY_1
AKY_HARF:
        CMP   A,#'_'
        BEQ   AKY_1
        CMP   A,AP_CRC
        BEQ   AKY_H1
        CMP   A,AP_CRC+1
        BNE   AKY_HJ
AKY_H1: STA   A,AP_YARI
        BRA   AKY_1
AKY_BOSLUK:                             ; rakamlardan sonra, harften önceki ilk boşluk = nokta
        LDA   A,AP_YARI
        BNE   AKY_1
        LDA   A,AP_HSAY
        BEQ   AKY_1
        LDA   A,AP_SAY
        CMP   A,#$FF
        BNE   AKY_1
        LDA   A,AP_HSAY
        STA   A,AP_SAY
        BRA   AKY_1
AKY_HJ: JMP   AKY_HATA                  ; (yakın dallanma için)
AKY_2:  LDA   A,AP_YARI
        BEQ   AKY_HJ
        LDA   A,AP_SAY
        CMP   A,#$FF
        BEQ   AKY_SABIT
        LDA   A,AP_HSAY                 ; noktadan sonra 1 ya da 2 hane
        SEC
        SBC   A,AP_SAY
        BEQ   AKY_HJ
        CMP   A,#3
        BCS   AKY_HJ
        CMP   A,#2
        BEQ   AKY_N1
        LDX   AP_HSAY                   ; tek hane: sona 0
        LDA   A,#'0'
        STA   A,AP_HANE,X
        INX
        STX   AP_HSAY
AKY_N1: LDA   A,AP_SAY                  ; noktadan önce tam hane ya da (boylam) bir eksik
        CMP   A,AP_T2
        BEQ   AKY_YAZ
        INC   A
        CMP   A,AP_T2
        BNE   AKY_HJ
        LDA   A,AP_T2
        CMP   A,#5
        BNE   AKY_HJ
        BRA   AKY_KAYDIR
AKY_SABIT:
        LDA   A,AP_T2                   ; beklenen rakam sayısı = hane + 2
        CLC
        ADC   A,#2
        STA   A,AP_T1
        LDA   A,AP_HSAY
        CMP   A,AP_T1
        BEQ   AKY_YAZ
        INC   A                         ; bir eksikse ve boylamsa başa 0
        CMP   A,AP_T1
        BNE   AKY_HJ
        LDA   A,AP_T2
        CMP   A,#5
        BNE   AKY_HJ
AKY_KAYDIR:
        LDX   #5
AKY_K:  LDA   A,AP_HANE,X
        STA   A,AP_HANE+1,X
        DEX
        BPL   AKY_K
        LDA   A,#'0'
        STA   A,AP_HANE
AKY_YAZ:
        PLX
        LDY   #0
AKY_3:  LDA   A,AP_HANE,Y
        STA   A,AP_CER,X
        INX
        INY
        TYA
        CMP   A,AP_T2
        BNE   AKY_3
        LDA   A,#'.'
        STA   A,AP_CER,X
        INX
        LDA   A,AP_HANE,Y
        STA   A,AP_CER,X
        INX
        INY
        LDA   A,AP_HANE,Y
        STA   A,AP_CER,X
        INX
        LDA   A,AP_YARI
        STA   A,AP_CER,X
        INX
        CLC
        RTS
AKY_HATA:
        PLX
        SEC
        RTS

; Kanal 53: yorum, sondaki boşluklar atılır (M = 1, X = yazma yeri, ilerler)
AP_MESAJ_YAZ:
        .ASSUME M=1,X=0
        LDY   #8
AMS_1:  DEY
        BMI   AMS_SON                   ; boş
        LDA   A,AP_AD,Y
        CMP   A,#' '
        BEQ   AMS_1
        STY   AP_HSAY                   ; son karakterin yeri
        LDY   #0
AMS_2:  LDA   A,AP_AD,Y
        STA   A,AP_CER,X
        INX
        CPY   AP_HSAY
        BEQ   AMS_SON
        INY
        BRA   AMS_2
AMS_SON:
        RTS

; Kanal 55: baştaki boşluklardan sonraki rakamlar (en çok 4) = saniye; rakam yoksa 0 (kapalı),
; 1..9 ise 10. Sayaçları sıfırlar (M = 0 giriş ve çıkış)
AP_ARALIK_AL:
        .ASSUME M=0,X=0
        LDM   #0,AP_ARALIK
        LDM   #0,AP_SANIYE
        LDM   #0,AP_MS
        LDY   #0
AAL_1:  CPY   #8                        ; baştaki boşluklar
        BEQ   AAL_SON
        LDA   A,AP_AD,Y
        AND   A,#$00FF
        CMP   A,#' '
        BNE   AAL_2
        INY
        BRA   AAL_1
AAL_2:  LDX   #4                        ; en çok 4 rakam
AAL_3:  CPY   #8
        BEQ   AAL_SON
        LDA   A,AP_AD,Y
        AND   A,#$00FF
        SEC
        SBC   A,#'0'
        BCC   AAL_SON
        CMP   A,#10
        BCS   AAL_SON
        STA   A,AP_T1                   ; ARALIK = 10 * ARALIK + rakam (AP_T1/AP_T2 2 bayt)
        LDA   A,AP_ARALIK
        ASL   A
        STA   A,AP_CRC                  ; (geçici, FCS çoktan yazıldı)
        ASL   A
        ASL   A
        CLC
        ADC   A,AP_CRC
        CLC
        ADC   A,AP_T1
        STA   A,AP_ARALIK
        INY
        DEX
        BNE   AAL_3
AAL_SON:
        LDA   A,AP_ARALIK
        BEQ   AAL_X
        CMP   A,#AP_EN_KISA
        BCS   AAL_X
        LDA   A,#AP_EN_KISA
        STA   A,AP_ARALIK
AAL_X:  SEM
        LDA   A,TIK_32MS
        STA   A,AP_SONTIK
        LDM   #0,AP_OTO
        CLM
        RTS

; FCS: CRC-16-CCITT (yansıtılmış 0x8408, başlangıç 0xFFFF, sonda tersi) çerçevenin sonuna,
; düşük bayt önce; AP_UZ 2 artar (M = 0)
AP_FCS:
        .ASSUME M=0,X=0
        LDA   A,#$FFFF
        STA   A,AP_CRC
        LDX   #0
AF_1:   CPX   AP_UZ
        BEQ   AF_SON
        LDA   A,AP_CER,X
        AND   A,#$00FF
        EOR   A,AP_CRC
        LDY   #8
AF_2:   LSR   A
        BCC   AF_3
        EOR   A,#$8408
AF_3:   DEY
        BNE   AF_2
        STA   A,AP_CRC
        INX
        BRA   AF_1
AF_SON: LDA   A,AP_CRC
        EOR   A,#$FFFF
        STA   A,AP_CER,X
        INX
        INX
        STX   AP_UZ
        RTS

; ---------------------------------------------------------------------------------------------
; VFO_KANCA'dan her turda (M = 0, X = 0; A, X, Y korunmaz). Menü açıkken zaman aşımını yeniler;
; APRS menüsünden çıkılmışsa (m, zaman aşımı) ve kayıt hâlâ APRS'inkiyse eski kaydı geri yükler.
APRS_KANCA:
        .ASSUME M=0,X=0
        SEM
        BBC   #F_AP_KURULU,AP_DURUM,AKN_SON
        LDA   A,MENU
        CMP   A,#ST_APRS_ON
        BNE   AKN_CIKIS
        CLB   #$40,R_P4                 ; hoparlör her turda yeniden kapatılır
        LDA   A,MENU_OLAY               ; açıkken menü zaman aşımı yok
        CMP   A,#$14
        BNE   AKN_ZAMAN
        LDM   #$3F,MENU_SAYAC
AKN_ZAMAN:
        JMP   AP_OTO_ZAMAN
AKN_CIKIS:
        LDM   #0,AP_DURUM
        LDA   A,AP_OTO                  ; otomatik gönderim isteği kalmasın
        BEQ   AKN_C1
        LDM   #0,AP_OTO
        CLM
        CLB   #F_PTT_APRS,PTT_ISTEK
        SEM
AKN_C1:
        LDX   #0                        ; kayıt hâlâ bizimki mi? (kanal anahtarı çevrildiyse dokunma)
AKN_1:  LDA   A,SYN_KAYIT,X
        CMP   A,AP_KAYIT,X
        BNE   AKN_SON
        INX
        CPX   #13
        BNE   AKN_1
        LDX   #0
AKN_2:  LDA   A,AP_YEDEK,X
        STA   A,SYN_KAYIT,X
        STA   A,SYN_YEDEK,X
        INX
        CPX   #13
        BNE   AKN_2
        LDM   #1,RADYO_DURUM
AKN_SON:
        CLM
        RTS

; Otomatik gönderim zamanlaması (APRS_KANCA'dan, M = 1; RTS ile M = 0 döner). Geçen 32 ms
; adımlarını saniyeye çevirir; aralık dolunca PTT isteği kurar. İstek ~3 s içinde paketle
; sonuçlanmazsa (gönderme yasağı, kanal meşgul) bırakılır.
AP_OTO_ZAMAN:
        .ASSUME M=1,X=0
        LDA   A,TIK_32MS                ; geçen adım
        SEC
        SBC   A,AP_SONTIK
        STA   A,AP_FARK
        LDA   A,TIK_32MS
        STA   A,AP_SONTIK
        JSR   AP_HAZIR                  ; paket hazır değilse sayma
        BNE   AOZ_SON
        CLM
        LDA   A,AP_ARALIK
        BEQ   AOZ_SON
        SEM
        LDA   A,AP_OTO
        BEQ   AOZ_SAY
        LDA   A,AP_OTO_SURE             ; istek bekliyor: süresi dolarsa bırak
        SEC
        SBC   A,AP_FARK
        BCS   AOZ_S1
        LDA   A,#0
AOZ_S1: STA   A,AP_OTO_SURE
        BNE   AOZ_SON
        LDM   #0,AP_OTO
        CLM
        CLB   #F_PTT_APRS,PTT_ISTEK
        LDM   #0,AP_SANIYE
        BRA   AOZ_SON
AOZ_SAY:
        .ASSUME M=1,X=0
        LDA   A,AP_FARK
        BEQ   AOZ_SON
        CLM
AOZ_1:  LDA   A,AP_MS                   ; her adım 32 ms
        CLC
        ADC   A,#32
        CMP   A,#1000
        BCC   AOZ_2
        SBC   A,#1000
        INC   AP_SANIYE
AOZ_2:  STA   A,AP_MS
        SEM
        DEC   AP_FARK
        CLM
        BNE   AOZ_1
        LDA   A,AP_SANIYE
        CMP   A,AP_ARALIK
        BCC   AOZ_SON
        SEB   #F_PTT_APRS,PTT_ISTEK     ; zamanı geldi: PTT isteği
        SEM
        LDM   #1,AP_OTO
        LDM   #94,AP_OTO_SURE           ; ~3 s
AOZ_SON:
        CLM
        RTS
