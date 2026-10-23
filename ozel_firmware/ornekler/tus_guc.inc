; tus_guc.inc - daire (p) tuşu güç / tarama, üçgen (q) tuşu monitör / kanal kayıt
;
; Cihazda panelde güç ve monitör düğmesi yok; bu işleri tuş takımı üstlenir (normal ekran,
; ayarlanmış FREKANS ekranı ve APRS ekranında; uzun basış ve monitör APRS'te yok):
;   p kısa bas  : çıkış gücü yüksek <-> düşük, ekranda ~1,5 s "YUKSEK G" / "ALCAK G"
;                 (tarama sürüyorsa kısa basış taramayı durdurur, güce dokunmaz)
;   p ~1 s tut  : taramayı başlat / durdur (el kartında orijinal 'p'; araç kartında KNL TARA + s)
;   q kısa bas  : monitör (susturma devre dışı / devrede), ekranda "MONITOR*" / "MONITOR-";
;                 tuş bırakılınca (AT V0-32'ye kadar basınca)
;   q ~1 s tut  : VFO'yu hafıza kanalına kaydet (kanal_kayit.inc)
; Diğer menülerde p firmware'e aynen gider; q her yerde yutulur (orijinalde A tipi kartta q,
; 0x0242 bit 4'ü kurup sonraki tuşları yutuyordu).
;
; Tuş kodu: F_0050xx tuş tarama yordamı kodu 0x0226'ya yazar ve L_005220'den geçer; tuş
; bırakılınca da aynı yerden 0x0F ile geçer (0x005202). Basılı tutulan tuş ~0,5 s'de bir
; tekrar eder. 0x005220'deki LDA A,$0226 / CMP A,#$3A yerine JMPL TUS_KANCA; kanca aynı iki
; komutla 0x005225'e döner (M = 1).
;
; Güç: panel düğmesinin yaptığının aynısı (L_005B11, uzak komut 'g' F_005822): 0x0240 bit 6
; (kullanıcı seçti) ve 0x023C bit 4 (1 = yüksek). F_00CEF3 güç indeksini bundan seçer; 0x0240
; bit 6 yokken kanalın HPRF biti ($037C bit 7) geçerlidir. İlk basışta bit 4 kanalınkine
; eşitlenir ki ilk basış gerçekten değiştirsin.
; Monitör: firmware'in q yolu (L_00549D): 0x0240 bit 1 + F_004FF6(Y = 'q'); 0x023A bit 13.
; İşler tuş yordamının içinde değil, VFO_KANCA'dan her turda çağrılan TUS_ISLE'de yapılır
; (firmware fonksiyonları ve LCD çizimi orada güvenli; çerçeve $0E,S).
;
; RAM: 0x0447..0x0449 (hiçbir kodun erişmediği, çalışırken dokunulmayan baytlar; 0x0445 ve
; 0x044A 16 bitlik değişkenler, bunlara değmez).

TUS_P       .EQU $0447          ; 0 boşta; $80|tık p / q basılı; 1 uzun basış işlendi (bırakılmayı bekle)
TUS_IS      .EQU $0448          ; bit 0 güç değiştir, bit 1 monitör, bit 2 basılı tuş q (p değil),
                                ; bit 3 mesaja kanal numarasını ekle, bit 7 mesaj ekranda
TUS_MS      .EQU $0449          ; mesajın biteceği tık (alt 7 bit)
TARAMA_D    .EQU $0241          ; bit 2 (0x0240 bit 10): tarama sürüyor (F_0048C1 kurar, 0x004A17 siler)
F_MONITOR   .EQU $004FF6
TG_UZUN     .EQU 31             ; 31 x 32 ms ~ 1 s
TG_MESAJ    .EQU 47             ; ~1,5 s

TUS_KOD     .EQU *

        .ORG $005220
        .EXPECT $AD,$26,$02,$C9,$3A
        .ASSUME M=1,X=0
        JMPL  TUS_KANCA
        NOP

        .ORG TUS_KOD
TUS_KANCA:
        .ASSUME M=1,X=0
        LDA   A,$0226
        CMP   A,#$0F
        BEQ   TK_BIRAK
        BBC   #KY_ACIK,KY_DURUM,TK_0    ; kanal kayıt modunda bütün tuşlar orada
        JMP   KY_TUS
TK_0:   CMP   A,#'p'
        BEQ   TK_P
        CMP   A,#'q'
        BNE   TK_SON
        LDA   A,MENU                    ; q: normal ekranda ve FREKANS'ta monitör / kayıt,
        BEQ   TK_Q                      ; her yerde yutulur
        CMP   A,#ST_FREKANS_ON
        BNE   TK_YUT
TK_Q:   LDA   A,TUS_P                   ; basılı tutarken gelen tekrarlar yutulur
        BNE   TK_YUT
        SEB   #$04,TUS_IS
        BRA   TK_BAS
TK_P:   JSR   TUS_EKRAN                 ; p: normal ekran, FREKANS, APRS; diğer menülerde firmware'e
        BNE   TK_SON
        LDA   A,TUS_P
        BNE   TK_YUT
        CLB   #$04,TUS_IS
TK_BAS: LDA   A,TIK_32MS
        ORA   A,#$80
        STA   A,TUS_P
        BRA   TK_YUT
TK_BIRAK:
        LDA   A,TUS_P
        BEQ   TK_SON
        LDM   #0,TUS_P
        CMP   A,#$80
        BCC   TK_SON                    ; uzun basış zaten işlendi
        BBC   #$04,TUS_IS,TK_PKISA
        SEB   #$02,TUS_IS               ; q kısa: monitör
        BRA   TK_SON
TK_PKISA:
        LDA   A,MENU                    ; p kısa: normal ekranda tarama sürüyorsa durdur
        BNE   TK_GUC
        BBC   #$04,TARAMA_D,TK_GUC
        JSR   TARAMA_TUS
        BRA   TK_SON
TK_GUC: SEB   #$01,TUS_IS
TK_YUT: LDM   #$0F,$0226
TK_SON: LDA   A,$0226
        CMP   A,#$3A
        JMPL  $005225

; Taramayı aç / kapat: el kartında orijinal 'p'; araç / sabit kartında (0x0335 bit 1) p tuşu
; taramaya bağlı değil, tarama KNL TARA menüsünden (durum 1) 's' ile açılıp kapanır: o tuşa basılmış
; gibi yapılır (firmware s'de işleyiciyi tablodan yükler). M = 1.
TARAMA_TUS:
        .ASSUME M=1,X=0
        BBC   #$02,$0335,TT_EL
        LDM   #1,MENU
        LDM   #'s',$0226
        RTS
TT_EL:  LDM   #'p',$0226
        RTS

; Z = 1: güç / monitör tuşlarının çalıştığı ekran (normal, FREKANS, APRS). M = 1, A bozulur.
TUS_EKRAN:
        .ASSUME M=1,X=0
        LDA   A,MENU
        BEQ   TE_SON
        CMP   A,#ST_FREKANS_ON
        BEQ   TE_SON
        CMP   A,#ST_APRS_ON
TE_SON: RTS

; VFO_KANCA'dan her turda (JSR; çerçeve $0E,S). Yazmaçları VFO_KANCA korur.
TUS_ISLE:
        SEM
        .ASSUME M=1,X=0
        BBC   #KY_ACIK,KY_DURUM,TI_0    ; kanal kayıt modu: yalnız o
        JMP   KY_ISLE
TI_0:   LDA   A,TUS_P                   ; p / q ~1 s basılı
        CMP   A,#$80
        BCC   TI_GUC
        LDA   A,TIK_32MS
        SEC
        SBC   A,TUS_P
        AND   A,#$7F
        CMP   A,#TG_UZUN
        BCC   TI_GUC
        BBC   #$04,TUS_IS,TI_PUZUN
        LDM   #1,TUS_P                  ; q: kanal kayıt
        JMP   KY_BASLA
TI_PUZUN:                               ; p: normal ekranda taramayı başlat / durdur
        LDA   A,MENU
        BNE   TI_UZUN
        LDA   A,$0226                   ; bekleyen tuş yoksa taramayı aç / kapat
        CMP   A,#$0F
        BNE   TI_GUC
        JSR   TARAMA_TUS
TI_UZUN:
        LDM   #1,TUS_P
TI_GUC: BBS   #$01,TUS_IS,TI_GUC1
        JMP   TI_MON
TI_GUC1:
        CLB   #$01,TUS_IS
        CLM
        .ASSUME M=0,X=0
        BBS   #$0040,$0240,TI_GUC2      ; ilk basış: bit 4 = kanalın HPRF'i
        SEM
        .ASSUME M=1,X=0
        CLB   #$10,$023C
        BBC   #$80,$037C,TI_GUC3
        SEB   #$10,$023C
TI_GUC3:
        CLM
        .ASSUME M=0,X=0
        SEB   #$0040,$0240
TI_GUC2:
        SEM
        .ASSUME M=1,X=0
        LDA   A,$023C
        EOR   A,#$10
        STA   A,$023C
        LDX   #TM_YUKSEK-TUS_METIN
        BBS   #$10,$023C,TI_GOSTER
        LDX   #TM_ALCAK-TUS_METIN
        BRA   TI_GOSTER
TI_MON: BBS   #$02,TUS_IS,TI_MON1
        JMP   TI_SURE
TI_MON1:
        CLB   #$02,TUS_IS
        CLM
        .ASSUME M=0,X=0
        SEB   #$0002,$0240
        LDY   #'q'
        LDA   A,$0E,S
        PHA
        JSRL  F_MONITOR
        CLM
        PLA
        SEM
        .ASSUME M=1,X=0
        LDX   #TM_MON_A-TUS_METIN
        BBS   #$20,$023B,TI_GOSTER      ; 0x023A bit 13: monitör açık
        LDX   #TM_MON_K-TUS_METIN
TI_GOSTER:                              ; X = metin
        CLB   #$40,TUS_IS
        LDA   A,MENU                    ; APRS ekranında hep; diğerlerinde ekranda kanal adı
        CMP   A,#ST_APRS_ON             ; (ya da VFO frekansı) duruyorsa, taramada değil
        BNE   TI_G1
        SEB   #$40,TUS_IS
        BRA   TI_G3
TI_G1:  BBS   #$04,TARAMA_D,TI_SON1
        LDY   #0
TI_G2:  LDA   A,KANAL_ADI,Y
        CMP   A,LCD,Y
        BNE   TI_SON1
        INY
        CPY   #8
        BNE   TI_G2
TI_G3:  LDY   #0
TI_KOPYA:
        LDA   A,>TUS_METIN,X
        STA   A,LCD,Y
        INX
        INY
        CPY   #8
        BNE   TI_KOPYA
        BBC   #$08,TUS_IS,TI_K2         ; "TAMAM 05": kaydedilen kanal
        CLB   #$08,TUS_IS
        LDA   A,KY_H
        STA   A,LCD+6
        LDA   A,KY_H+1
        STA   A,LCD+7
TI_K2:  LDA   A,TIK_32MS
        CLC
        ADC   A,#TG_MESAJ
        STA   A,TUS_MS
        SEB   #$80,TUS_IS
        BRA   TI_CIZ
TI_SON1:
        JMP   TI_SON
TI_SURE:                                ; mesaj süresi doldu mu?
        BBC   #$80,TUS_IS,TI_SON1
        LDA   A,TIK_32MS
        SEC
        SBC   A,TUS_MS
        AND   A,#$7F
        CMP   A,#$40
        BCS   TI_SON1                   ; henüz değil
        CLB   #$80,TUS_IS
        BBC   #$40,TUS_IS,TI_KANAL
        LDA   A,MENU                    ; APRS ekranı: "APRS AKT"
        CMP   A,#ST_APRS_ON
        BNE   TI_SON1
        CLM
        .ASSUME M=0,X=0
        LDX   #16
        JSR   AP_METIN_YAZ
        BRA   TI_CIZ
TI_KANAL:
        .ASSUME M=1,X=0
        LDA   A,MENU                    ; başka menüye geçildiyse ekranı firmware çizdi
        BEQ   TI_K1
        CMP   A,#ST_FREKANS_ON
        BNE   TI_SON1
TI_K1:  BBS   #$04,TARAMA_D,TI_SON1
        LDX   #0
TI_GERI:
        LDA   A,KANAL_ADI,X
        STA   A,LCD,X
        INX
        CPX   #8
        BNE   TI_GERI
TI_CIZ: CLM
        .ASSUME M=0,X=0
        LDA   A,$0E,S
        PHA
        JSRL  F_LCD
        CLM
        PLA
TI_SON: CLM
        .ASSUME M=0,X=0
        RTS

TUS_METIN:
TM_YUKSEK:
        .TEXT8 "YUKSEK G"
TM_ALCAK:
        .TEXT8 "ALCAK G"
TM_MON_A:
        .TEXT8 "MONITOR*"
TM_MON_K:
        .TEXT8 "MONITOR-"
TM_KAYIT:
        .TEXT8 "KAYIT"                  ; kayıt ekranı "KAYIT 0_" (ilk 6 karakter)
TM_TAMAM:
        .TEXT8 "TAMAM"                  ; + kanal numarası
TM_IPTAL:
        .TEXT8 "IPTAL"
TM_VFOYOK:
        .TEXT8 "VFO YOK"
TM_KHATA:
        .TEXT8 "KYT HATA"
