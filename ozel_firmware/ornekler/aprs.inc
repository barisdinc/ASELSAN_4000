; aprs.inc - APRS menü öğesi: PTT'ye basınca 144,800 MHz'de konum paketi (AX.25 / 1200 baud AFSK)
;
;   ... m (APRS) s     -> "APRS PTT"    EEPROM'daki APRS bölümü okunup paket kurulur;
;                                       telsiz alma ve gönderme 144,800 MHz'e geçer (TKS kapalı),
;                                       hoparlör tamamen susar (kanal sesi, bip ve uyarı tonları yok)
;   PTT                -> "APRS TX"     paket gider (~0,75 s), ardından "APRS OK"; PTT basılı
;                                       tutulursa telsiz 144,800'de göndermeye devam eder
;   m                  -> çık: telsiz önceki kanalına / frekansına döner
;
; Ayarlar EEPROM 0x6E0..0x731'deki APRS bölümünde (P4000'in ve firmware'in kullanmadığı alan;
; docs/p4000_eeprom.md). Simülatörün / yükleme sayfasının EEPROM görünümündeki "APRS" sayfası yazar.
;   +0   "AP" imza, +2 sürüm (1), +3 boş
;   +4   çağrı işareti[-SSID], 10 bayt, 0 ile biter  "TC19MK-10" (küçük harf büyüğe çevrilir;
;        AX.25: en çok 6 harf / rakam, SSID 0..15)
;   +14  simge tablosu, simge kodu                    "/b" bisiklet
;   +16  enlem "DDMM.mmN" (8 bayt)                    "3955.52N"   Ankara, Anıtkabir
;   +24  boylam "DDDMM.mmE" (9 bayt)                  "03250.22E"
;   +33  boş
;   +34  otomatik gönderim aralığı, saniye (2 bayt büyük uçlu): APRS menüsündeyken PTT'ye
;        basılmasa da bu aralıkla paket gider (0: kapalı; en kısa 10 s)
;   +36  yorum, 43 bayt, 0 ile biter (baytlar olduğu gibi gider: küçük harf, noktalama, UTF-8)
;   +79  boş
;   +80  sağlama: +0..+79 baytlarının 16 bit toplamı, büyük uçlu (0x730..0x731)
; İmza, sürüm ya da sağlama tutmazsa (bölüm hiç yazılmamışsa) ROM'daki varsayılanlar kullanılır
; (AP_VARSAYILAN, aynı yerleşim). Bölüm her APRS'e girişte yeniden okunur.
; Paket: <çağrı>>APIZCI,WIDE1-1,WIDE2-1:!<enlem><tablo><boylam><kod><yorum>
;   örnek: TC19MK-10>APIZCI,WIDE1-1,WIDE2-1:!3955.52N/03250.22EbASELSAN 4000
; Alan geçersizse ekranda "CAGRI -" / "ENLEM -" / "BOYLAM -" (İngilizce "CALL -", "LAT -", "LON -")
; ve paket gönderilmez.

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
;            sayılır ($00 = 1 -> RAM 0x0240 bit 0, tip A'da ses yükselteci kapalı) ve almada
;            FX806'ya squelch kapalıyken yazılan ayar (L_00CD3F) yazılır (vericide değil: o ayar
;            modülatör seviyesini sıfırlar). Tip B kartta hoparlörü asıl bu
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
AP_HSAY     .EQU OZEL_RAM+$D7   ; 2 bayt: konumun uzunluğu
AP_SAY      .EQU OZEL_RAM+$DA   ; çağrı işaretindeki karakter sayısı
AP_SSID     .EQU OZEL_RAM+$DB
AP_T1       .EQU OZEL_RAM+$DC
AP_T2       .EQU OZEL_RAM+$DD
AP_AD       .EQU OZEL_RAM+$DE   ; 16 bayt: EEPROM'dan okunan alan
AP_EE_OK    .EQU OZEL_RAM+$EE   ; 1 = EEPROM'daki APRS bölümü geçerli, 0 = ROM varsayılanları
AP_HEDEF_P  .EQU OZEL_RAM+$EF   ; 2 bayt: AP_ALAN_OKU hedefi (bank 0)
AP_ALAN_N   .EQU OZEL_RAM+$F1   ; 2 bayt: AP_ALAN_OKU bayt sayısı (alt bayt)
AP_TOPLAM   .EQU OZEL_RAM+$F3   ; 2 bayt: bölüm sağlaması
AP_YER      .EQU OZEL_RAM+$F5   ; 2 bayt: bölümdeki yer / EEPROM adresi
AP_MOD_B    .EQU OZEL_RAM+$F7   ; EEPROM'daki APRS modu işaretinin okunan / yazılan kopyası
AP_ACILIS   .EQU OZEL_RAM+$F8   ; 2 bayt: ACILIS_TAMAM ise açılış denetimi yapıldı
AP_MOD_IST  .EQU OZEL_RAM+$FA   ; 2 bayt: AP_MOD_KUR'un istenen değeri
; AX.25 çerçevesi (en uzun 95 bayt) iki yığının arasındaki boş alanda: firmware'in çerçeve
; yığını 0x62A'dan yukarı ~0x6A0'a, donanım yığını 0x879'dan aşağı ~0x821'e kadar iniyor
; (sim/ram_kullanim.js); 0x740..0x79F iki yanda da 100 bayttan fazla pay bırakıyor
AP_CER      .EQU $0740          ; 96 bayt
; otomatik gönderim (OZEL_RAM'den önceki boş alan, $04ED..$04FF)
AP_ARALIK   .EQU $04F0          ; 2 bayt: otomatik gönderim aralığı, saniye (0 = kapalı)
AP_SANIYE   .EQU $04F2          ; 2 bayt: son gönderimden beri geçen saniye
AP_MS       .EQU $04F4          ; 2 bayt: saniyenin milisaniyesi (32 ms adım)
AP_SONTIK   .EQU $04F6          ; son görülen 32 ms sayacı ($0457)
AP_FARK     .EQU $04F7          ; geçen 32 ms adımı
AP_OTO      .EQU $04F8          ; 1 = otomatik gönderim için PTT isteği kuruldu
AP_OTO_SURE .EQU $04F9          ; isteğin kalan süresi (32 ms adım); verici açılmazsa bırakılır
TIK_32MS    .EQU $0457          ; TB2 kesmesi her 32 ms'de bir artırır
PTT_ISTEK   .EQU $0244          ; F_005E56: bit 0..5 PTT istekleri (0x20 P47, 0x10 panel ...)
F_PTT_APRS  .EQU $0004          ; firmware'de kullanılmayan istek biti: otomatik APRS gönderimi
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


; EEPROM'daki APRS bölümü (yerler bölümün başına göre)
EE_APRS     .EQU $06E0
AB_SURUM_NO .EQU 1
AB_CAGRI    .EQU 4              ; 10 bayt
AB_SEMBOL   .EQU 14             ; 2 bayt
AB_ENLEM    .EQU 16             ; 8 bayt
AB_BOYLAM   .EQU 24             ; 9 bayt
AB_ARALIK   .EQU 34             ; 2 bayt büyük uçlu
AB_YORUM    .EQU 36             ; 43 bayt
AB_YORUM_N  .EQU 43
AB_UZ       .EQU 80             ; sağlama EE_APRS + 80 (büyük uçlu)
; APRS modu işareti (bölümün dışında, sağlamaya girmez): APRS menüsüne girince MOD_ISARET,
; menüden çıkınca 0 yazılır. Telsiz APRS'teyken kapanırsa açılışta APRS'e kendiliğinden döner.
EE_MOD      .EQU $0732
MOD_ISARET  .EQU $A5
ACILIS_TAMAM .EQU $A55A         ; AP_ACILIS: RAM açılışta temizlenmez, rastgele eşleşme 1/65536

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
F_EE_YAZ    .EQU $0073C6        ; EEPROM'a yaz: F_EE_OKU ile aynı çerçeve (+0 sayı, +1..+3 kaynak,
                                ; +4..+5 adres); sayfa sınırını ve yazmanın bitişini kendisi bekler
F_EE_OKU    .EQU $0072EE        ; EEPROM'dan oku. Y = $A0; çerçeve: +0 sayı, +1..+3 hedef (24 bit),
                                ; +4..+5 EEPROM adresi; çerçevenin geri kalanını yerel olarak kullanır

; ---------------------------------------------------------------------------------------------
AP_KOD_ADRES .EQU *
        .ORG $002028                    ; VEC_TB1
        .EXPECT $5C,$27,$C6,$01
        JMPL  AFSK_TB1
        .ORG $00CC44                    ; F_00CB52: ses kapalı mı ($00) kararı -> APRS almada kapalı
        .EXPECT $D8,$A5,$00,$F8         ; CLM / LDA A,$00 / SEM
        JMPL  APRS_SUS_KARAR
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
        BBC   #$0001,AR_DURUM,AT_ORJ
        JMP   AR_TB1                    ; APRS alma (aprs_rx.inc)
AT_ORJ: PUL   #$35
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

; F_00CB52, L_00CC44 (D = F_00CB52'nin çerçevesi; $00 = 1 ses kapalı). APRS menüsünde, almada
; ses hep kapalı sayılır: seçmeli çağrı / TKS beklerken firmware'in yaptığı susturma. Taşıyıcı
; varken ($023E bit 5) FX806'ya normal ayar yazılır, yalnız C-BUS $10 bit 7 (ses çıkışı) kapalı
; kalır (L_00CD32); ses yolu, dolayısıyla P66'daki çözücünün girişi açık kalır. Tip A kartta
; $0240 bit 0 ile ses yükselteci de kapanır. Vericide dokunulmaz.
APRS_SUS_KARAR:
        SEM
        .ASSUME M=1,X=0
        BBC   #$10,$023E,SSK_1          ; verici: orijinal karar
        JSR   AP_ETKIN
        BNE   SSK_1
        LDM   #1,$00                    ; APRS: ses kapalı
SSK_1:  CLM                             ; orijinal: CLM / LDA A,$00 / SEM
        LDA   A,$00
        SEM
        JMPL  $00CC48

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
        LDM   #0,AR_DURUM               ; alma çözücüsü kapalı (TB1 gönderimde)
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
; Menü: girişte 144,800'ü kur ve paketi EEPROM'daki APRS bölümünden hazırla
APRS_GIR:
        .ASSUME M=0,X=0
        LDA   A,$04,S
        STA   A,AP_F
        PHA
        LDX   #MOD_ISARET               ; kapanıp açılırsa APRS'e dönülsün
        JSR   AP_MOD_KUR
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
        JSR   AR_TUS                    ; '*' / '#': duyulanlar listesi (aprs_rx.inc)
        BCS   ATS_SON
AP_EKRAN_SON:
        JSR   AP_EKRAN
ATS_SON:
        CLM
        PLA
        RTL

; APRS ekranı: "APRS PTT" / "APRS OK" / hata (M = 0 giriş ve çıkış)
AP_EKRAN:
        .ASSUME M=0,X=0
        SEM
        LDM   #0,AR_GOSTER              ; çağrı işareti / mesaj gösterilmiyor
        LDM   #0,AR_SEC
        LDM   #0,AR_KDURUM
        LDM   #0,AR_KILIT
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
        RTS

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
; Paketi EEPROM'daki APRS bölümünden (geçersizse ROM varsayılanlarından) kur: AP_CER / AP_UZ
; (M = 0 giriş ve çıkış). C = 1: hata (AP_HATA_KOD)
AP_HAZIRLA:
        .ASSUME M=0,X=0
        JSR   AP_BLOK_DENETLE
        LDA   A,#AB_SEMBOL
        LDY   #2
        JSR   AP_ALAN_AD
        SEM
        JSR   AP_SEMBOL_AL
        CLM
        LDA   A,#AB_CAGRI
        LDY   #10
        JSR   AP_ALAN_AD
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
        LDA   A,#AB_ENLEM
        LDY   #8
        JSR   AP_ALAN_AD
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
        LDA   A,#AB_BOYLAM
        LDY   #9
        JSR   AP_ALAN_AD
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
        JSR   AP_YORUM_YAZ
        STX   AP_UZ
        JSR   AP_FCS
        LDA   A,#AB_ARALIK              ; otomatik gönderim aralığı
        LDY   #2
        JSR   AP_ALAN_AD
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

; Bölüm yazılmamışsa kullanılan değerler: EEPROM'daki yerleşimin aynısı (sağlama hariç).
; Ankara, Anıtkabir 39°55,52' K 32°50,22' D; bisiklet; otomatik gönderim kapalı.
AP_VARSAYILAN:
        .BYTE "AP",AB_SURUM_NO,0        ; +0
        .BYTE "TC19MK-10",0             ; +4  çağrı işareti
        .BYTE "/b"                      ; +14 simge
        .BYTE "3955.52N"                ; +16 enlem
        .BYTE "03250.22E",0             ; +24 boylam
        .BYTE 0,0                       ; +34 aralık
        .BYTE "ASELSAN 4000"            ; +36 yorum
        .FILL AP_VARSAYILAN+AB_UZ-*,0

; AP_AD'ye bölümün A'daki yerinden Y bayt oku (M = 0; X, Y korunur)
AP_ALAN_AD:
        .ASSUME M=0,X=0
        PHA
        LDA   A,#AP_AD
        STA   A,AP_HEDEF_P
        PLA
        STY   AP_ALAN_N
; Bölümün A'daki yerinden AP_ALAN_N bayt AP_HEDEF_P'ye: bölüm geçerliyse EEPROM'dan, değilse
; ROM'daki varsayılandan (M = 0; X, Y korunur)
AP_ALAN_OKU:
        .ASSUME M=0,X=0
        PHX
        PHY
        STA   A,AP_YER
        SEM
        LDA   A,AP_EE_OK
        CLM
        BNE   AAK_EE
        LDX   AP_YER
        LDY   AP_HEDEF_P
        SEM
AAK_1:  LDA   A,AP_ALAN_N
        BEQ   AAK_SON
        LDA   A,>AP_VARSAYILAN,X
        STA   A,!$0000,Y
        INX
        INY
        DEC   AP_ALAN_N
        BRA   AAK_1
AAK_SON:
        CLM
        PLY
        PLX
        RTS
AAK_EE: LDA   A,AP_YER
        CLC
        ADC   A,#EE_APRS
        JSR   AP_EE_OKU
        PLY
        PLX
        RTS

; EEPROM'un A adresinden AP_ALAN_N bayt AP_HEDEF_P'ye (M = 0; X, Y korunur). Firmware'in
; okuma yordamı menü işleyicisinin boş çerçevesini (AP_F) parametre ve yerel olarak kullanır.
AP_EE_OKU:
        .ASSUME M=0,X=0
        PHX
        PHY
        PHD
        TAY
        LDA   A,AP_F
        TAD
        STY   $04                       ; EEPROM adresi
        LDA   A,AP_HEDEF_P              ; hedef (bank 0)
        STA   A,$01
        SEM
        LDA   A,AP_ALAN_N
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
        PLY
        PLX
        RTS

; Bölüm geçerli mi: imza "AP", sürüm, sağlama (16 baytlık parçalarla toplanır).
; AP_EE_OK = 1 / 0 (M = 0 giriş ve çıkış)
AP_BLOK_DENETLE:
        .ASSUME M=0,X=0
        SEM
        LDM   #0,AP_EE_OK
        CLM
        LDM   #0,AP_TOPLAM
        LDM   #16,AP_ALAN_N
        LDA   A,#AP_AD
        STA   A,AP_HEDEF_P
        LDA   A,#EE_APRS
        STA   A,AP_YER
ABD_1:  LDA   A,AP_YER
        JSR   AP_EE_OKU
        LDA   A,AP_YER
        CMP   A,#EE_APRS
        BNE   ABD_TOPLA
        LDA   A,AP_AD                   ; ilk parça: imza ve sürüm
        CMP   A,#'A'+'P'*256
        BNE   ABD_SON
        LDA   A,AP_AD+2
        AND   A,#$00FF
        CMP   A,#AB_SURUM_NO
        BNE   ABD_SON
ABD_TOPLA:
        LDX   #0
ABD_2:  LDA   A,AP_AD,X
        AND   A,#$00FF
        CLC
        ADC   A,AP_TOPLAM
        STA   A,AP_TOPLAM
        INX
        CPX   #16
        BNE   ABD_2
        LDA   A,AP_YER
        CLC
        ADC   A,#16
        STA   A,AP_YER
        CMP   A,#EE_APRS+AB_UZ
        BNE   ABD_1
        LDM   #2,AP_ALAN_N              ; sağlama, büyük uçlu
        JSR   AP_EE_OKU                 ; A = EE_APRS + AB_UZ
        SEM
        LDA   A,AP_AD
        CMP   A,AP_TOPLAM+1
        BNE   ABD_S
        LDA   A,AP_AD+1
        CMP   A,AP_TOPLAM
        BNE   ABD_S
        LDM   #1,AP_EE_OK
ABD_S:  CLM
ABD_SON:
        RTS

; Simge: AP_AD[0] tablo, AP_AD[1] kod; ikisi de yazılabilir karakter değilse /b (M = 1)
AP_SEMBOL_AL:
        .ASSUME M=1,X=0
        LDA   A,AP_AD
        JSR   AP_YAZILIR
        BCS   ASA_V
        LDA   A,AP_AD+1
        JSR   AP_YAZILIR
        BCS   ASA_V
        LDA   A,AP_AD
        STA   A,AP_SEMBOL
        LDA   A,AP_AD+1
        STA   A,AP_SEMBOL+1
        RTS
ASA_V:  LDM   #'/',AP_SEMBOL
        LDM   #'b',AP_SEMBOL+1
        RTS

; A = karakter: '!'..'~' ise C = 0 (M = 1, A korunur)
AP_YAZILIR:
        .ASSUME M=1,X=0
        CMP   A,#'!'
        BCC   AYZ_H
        CMP   A,#$7F
        BCS   AYZ_H
        CLC
        RTS
AYZ_H:  SEC
        RTS

; A = karakter: küçük harfse büyük harf (M = 1)
AP_BUYUK:
        .ASSUME M=1,X=0
        CMP   A,#'a'
        BCC   ABY_X
        CMP   A,#'z'+1
        BCS   ABY_X
        AND   A,#$DF
ABY_X:  RTS

; Çağrı işareti (AP_AD, 10 bayt; 0, boşluk ya da '-' ile biter): 1..6 harf / rakam, küçük harf
; büyüğe çevrilir; isteğe bağlı -SSID (0..15) -> AX.25 adresi
; (M = 1, X = çerçevede yazma yeri, ilerler). C = 1: geçersiz
AP_CAGRI_YAZ:
        .ASSUME M=1,X=0
        LDM   #0,AP_SAY
        LDY   #0
ACY_1:  CPY   #10
        BEQ   ACY_DOLDUR
        LDA   A,AP_AD,Y
        BEQ   ACY_DOLDUR
        CMP   A,#'-'
        BEQ   ACY_DOLDUR
        CMP   A,#' '
        BEQ   ACY_DOLDUR
        JSR   AP_BUYUK
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
        CPY   #10
        BEQ   ACY_YAZ
        LDA   A,AP_AD,Y
        CMP   A,#'-'
        BNE   ACY_YAZ
        INY
        CPY   #10
        BEQ   ACY_HATA
        LDA   A,AP_AD,Y
        JSR   AP_RAKAM
        BCS   ACY_HATA
        STA   A,AP_SSID
        INY
        CPY   #10
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

; A = karakter: rakamsa C = 0 (M = 1, A korunur)
AP_RAKAM_MI:
        .ASSUME M=1,X=0
        CMP   A,#'0'
        BCC   ARM_H
        CMP   A,#'9'+1
        BCS   ARM_H
        CLC
        RTS
ARM_H:  SEC
        RTS

; Konum (AP_AD): A = noktadan önceki rakam sayısı (4 enlem, 5 boylam), Y = izin verilen iki
; harf (alt / üst bayt). Biçim tam olarak "DDMM.mmH" / "DDDMM.mmH" (harf küçükse büyüğe
; çevrilir); çerçeveye olduğu gibi yazılır. (M = 1, X = yazma yeri, ilerler). C = 1: geçersiz
AP_KONUM_YAZ:
        .ASSUME M=1,X=0
        STA   A,AP_T2
        STY   AP_CRC                    ; izin verilen harfler (geçici)
        LDY   #0
AKY_1:  LDA   A,AP_AD,Y
        JSR   AP_RAKAM_MI
        BCS   AKY_HATA
        INY
        TYA
        CMP   A,AP_T2
        BNE   AKY_1
        LDA   A,AP_AD,Y
        CMP   A,#'.'
        BNE   AKY_HATA
        INY
        LDA   A,AP_AD,Y
        JSR   AP_RAKAM_MI
        BCS   AKY_HATA
        INY
        LDA   A,AP_AD,Y
        JSR   AP_RAKAM_MI
        BCS   AKY_HATA
        INY
        LDA   A,AP_AD,Y
        JSR   AP_BUYUK
        CMP   A,AP_CRC
        BEQ   AKY_2
        CMP   A,AP_CRC+1
        BNE   AKY_HATA
AKY_2:  STA   A,AP_AD,Y
        INY
        STY   AP_HSAY                   ; uzunluk
        LDY   #0
AKY_3:  LDA   A,AP_AD,Y
        STA   A,AP_CER,X
        INX
        INY
        CPY   AP_HSAY
        BNE   AKY_3
        CLC
        RTS
AKY_HATA:
        SEC
        RTS

; Yorum: bölümden doğrudan çerçeveye okunur, ilk 0 baytında biter (en çok 43 bayt)
; (M = 0 giriş ve çıkış, X = yazma yeri, ilerler)
AP_YORUM_YAZ:
        .ASSUME M=0,X=0
        TXA
        CLC
        ADC   A,#AP_CER
        STA   A,AP_HEDEF_P
        LDA   A,#AB_YORUM_N
        STA   A,AP_ALAN_N
        LDA   A,#AB_YORUM
        JSR   AP_ALAN_OKU
        LDY   #0
        SEM
AYY_1:  LDA   A,AP_CER,X
        BEQ   AYY_SON
        INX
        INY
        CPY   #AB_YORUM_N
        BNE   AYY_1
AYY_SON:
        CLM
        RTS

; Otomatik gönderim aralığı (AP_AD, 2 bayt büyük uçlu, saniye): 0 kapalı, 1..9 ise 10.
; Sayaçları sıfırlar (M = 0 giriş ve çıkış)
AP_ARALIK_AL:
        .ASSUME M=0,X=0
        SEM
        LDA   A,AP_AD
        STA   A,AP_ARALIK+1
        LDA   A,AP_AD+1
        STA   A,AP_ARALIK
        CLM
        LDM   #0,AP_SANIYE
        LDM   #0,AP_MS
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
        LDA   A,AP_ACILIS
        CMP   A,#ACILIS_TAMAM
        BEQ   AKN_0
        JMP   AP_ACILIS_DENETLE
AKN_0:  SEM
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
        JSR   AP_OTO_ZAMAN
        CLM
        LDA   A,$0E,S                   ; çerçeve göstericisi (VFO_KANCA $0C,S + JSR)
        JSR   AR_ISLE                   ; alma: duyulan istasyonlar (aprs_rx.inc)
        RTS
        .ASSUME M=1,X=0
AKN_CIKIS:
        LDM   #0,AP_DURUM
        JSR   AR_KAPAT                  ; alma çözücüsü kapansın (TB1)
        LDM   #0,AR_GOSTER
        LDM   #0,AR_SEC
        LDM   #0,AR_KDURUM
        LDM   #0,AR_KILIT
        CLM                             ; menüden çıkıldı: açılışta APRS'e dönülmesin
        LDA   A,$0E,S                   ; çerçeve göstericisi (VFO_KANCA $0C,S + JSR)
        LDX   #0
        JSR   AP_MOD_KUR
        SEM
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

; Açılıştan sonraki ilk uygun anda (APRS_KANCA'dan, M = 0; yığın APRS_KANCA'nın girişindeki gibi):
; normal ekran (menü 0), alma bekleniyor (radyo durumu 2), bekleyen tuş yok. EEPROM'daki işaret
; MOD_ISARET ise menü durumu APRS başlığına alınır ve 's' tuşu bırakılır: firmware'in tuş yolu
; bir sonraki turda kullanıcı seçmiş gibi APRS_GIR'i çağırır.
AP_ACILIS_DENETLE:
        .ASSUME M=0,X=0
        SEM
        LDA   A,MENU
        BNE   AAD_SON
        LDA   A,RADYO_DURUM
        CMP   A,#2
        BNE   AAD_SON
        LDA   A,KEY
        CMP   A,#$0F
        BNE   AAD_SON
        LDM   #0,AP_DURUM               ; RAM açılışta rastgele: APRS değişkenlerini temizle
        LDM   #0,AP_OTO
        JSR   AR_SIFIRLA
        CLM
        LDA   A,#ACILIS_TAMAM
        STA   A,AP_ACILIS
        LDA   A,$0E,S                   ; çerçeve göstericisi (VFO_KANCA $0C,S + JSR)
        PHD
        TAD
        CLC                             ; oku
        JSR   AP_MOD_ERIS
        PLD
        SEM
        LDA   A,AP_MOD_B
        CMP   A,#MOD_ISARET
        BNE   AAD_SON
        LDM   #ST_APRS,MENU
        LDM   #'s',KEY
AAD_SON:
        CLM
        RTS

; EEPROM'daki APRS modu işaretini X'in alt baytı yap (farklıysa yazar). A = NC77 çerçeve
; göstericisi (boş çerçeve alanı). M = 0 giriş ve çıkış; X, Y, D korunur.
AP_MOD_KUR:
        .ASSUME M=0,X=0
        PHX
        PHY
        PHD
        TAD
        STX   AP_MOD_IST
        CLC                             ; oku
        JSR   AP_MOD_ERIS
        SEM
        LDA   A,AP_MOD_B
        CMP   A,AP_MOD_IST
        BEQ   AMK_SON
        LDA   A,AP_MOD_IST
        STA   A,AP_MOD_B
        CLM
        SEC                             ; yaz
        JSR   AP_MOD_ERIS
AMK_SON:
        CLM
        PLD
        PLY
        PLX
        RTS

; EE_MOD'daki bir baytı AP_MOD_B'ye oku (C = 0) ya da AP_MOD_B'yi oraya yaz (C = 1).
; D = çerçeve. M = 0 giriş ve çıkış.
AP_MOD_ERIS:
        .ASSUME M=0,X=0
        PHP
        LDA   A,#EE_MOD
        STA   A,$04
        LDA   A,#AP_MOD_B
        STA   A,$01
        SEM
        LDA   A,#1
        STA   A,$00
        LDA   A,#0
        STA   A,$03
        CLM
        LDY   #$00A0
        PLP
        TDA
        PHA
        BCS   AME_YAZ
        JSRL  F_EE_OKU
        BRA   AME_SON
AME_YAZ:
        JSRL  F_EE_YAZ
AME_SON:
        CLM
        PLA
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
