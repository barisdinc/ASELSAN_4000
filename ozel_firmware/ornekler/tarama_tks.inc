; tarama_tks.inc - taramada TKS (CTCSS) bekleme süresi 10 adım (320 ms) uzun
;
; ASELSAN'ın sonraki "radio" sürümlerinden (YA4000E V2-51, 0ya4000s_son.hex V2-54) alındı.
; F_00EEE9 (tarama / öncelikli kanal), durum 3: taranan kanalda sinyal var (0x023E bit 0)
; ve kanalın alma TKS tonu tanımlıysa (0x0375 & 0x3F) firmware tonun çözülmesini bekler:
;
;     0x020A = ton_periyodu[ton] / 0x3F0 + 1        (ROM 0x2385; ~periyot ms + 1)
;
; 0x020A 32 ms'lik kesmede (0x01BA64) azalır. Süre bitince ton hâlâ çözülmediyse
; (0x023A bit 12) sonraki kanala geçer. 67 Hz için 15 adım = 480 ms, 250 Hz için 4 adım =
; 128 ms; ton geç çözülürse doğru kanal atlanıyordu. Yeni sürümlerde + 1 yerine + 11:
; 67 Hz 800 ms, 250 Hz 448 ms.
;
; 0x00EFF1: SEM / INC A / STA A,$020A / JMP L_00F030. İlk 4 baytın yerine JMPL; DIV'den
; sonra elde biti belirsiz olduğu için toplama burada CLC ile yapılır.

TT_EK       .EQU 11                     ; orijinal + 1, yeni sürümlerde + 11

        .ORG $00EFF1
        .EXPECT $F8,$3A,$8D,$0A
        .ASSUME M=0,X=0
        JMPL  TT_BEKLEME

        .ORG $012080                    ; bank 1 boş alanı (açılış ekranı $012000-$012068)
TT_BEKLEME:
        SEM
        .ASSUME M=1,X=0
        CLC
        ADC   A,#TT_EK
        STA   A,$020A
        JMPL  $00F030                   ; orijinal akış: JMP L_00F03E (M = 1)
