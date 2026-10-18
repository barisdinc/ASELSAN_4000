/*
 * YA4000D.HEX - elle yapilmis ornek C geri donusumu (decompile)
 * ----------------------------------------------------------------
 * Bu dosya, disassembly'den C'ye nasil gecilecegini gostermek icin
 * birkac fonksiyonun elle cevrilmis halidir. Isimler (delay_or_key,
 * set_options vb.) TAHMINDIR; adresler YA4000D.asm ile birebir eslesir.
 *
 * NC77 cagirma kurali (koddan gozlenen):
 *   - D (direct page register) = fonksiyonun yerel degisken cercevesi.
 *     Prolog:  LDA 4,S / PHD / TAD / (CLC; ADC #n | INC A..) / PHA
 *     -> cagiran, cagrilanin cerceve tabanini yigina iter; cagrilan onu D'ye yukler,
 *        kendi yerel alan boyu kadar ilerletip kendi cagrilari icin yigina iter.
 *   - 1. parametre Y register'inda gelir (genelde STY $00 ile yerele yazilir),
 *     diger parametreler cagrilanin cercevesine ($02,$04.. dp) yazilir.
 *   - Donus degeri A'da (32-bit: A=alt, B=ust). char donduren fonksiyonlar
 *     M=1 (8-bit) modda doner; int donduren fonksiyonlar M=0.
 */

#include <stdint.h>

/* ---- M37732 SFR'leri (adresler 7700 ailesi standart haritasi) ---- */
#define SFR8(a)   (*(volatile uint8_t  *)(a))
#define SFR16(a)  (*(volatile uint16_t *)(a))
/* M37732S4'te P0-P3 yok (harici adres/veri yolu); 0x02-0x09 bos adresler */
/* Pin gorevleri: 4021/4025 bakim kilavuzu 3.6.2.3 */
#define P4      SFR8(0x0A)   /* P43 XSHIFT, P44 kontak, P45 acil cagri, P46 susturma bilgisi, P47 dis PTT */
#define P5      SFR8(0x0B)   /* P50 uyari, P51 TDSEC1, P52 SC ton, P53 TDSEC2, P54 TKS/sus.ayar PWM, P55 alma sec, P56 guc ayar PWM, P57 susturma */
#define P6      SFR8(0x0E)   /* P60 uzak veri, P61 SCL, P62 SDA, P63 IRQ0(INT1), P64 IRQ1(INT2), P65 TKS cozme(TB0), P66 SC cozme(TB1), P67 sentezor kilit */
#define P6_SYNTH_LOCK 0x80
#define P7      SFR8(0x0F)   /* P70 sent. yukle, P71 guc acma, P72 gond. ses, P73..P77 = AN3..AN7 (sicaklik, 13V2, RSSI, yansiyan, giden guc) */
#define P8      SFR8(0x12)   /* P81 sent. sec, P82/P83 RS232 (UART0), P85/P86/P87 C-BUS + sentezor (UART1) */
#define P4D     SFR8(0x0C)
#define P5D     SFR8(0x0D)
#define P6D     SFR8(0x10)
#define P7D     SFR8(0x11)
#define P8D     SFR8(0x14)
#define TCSTART SFR8(0x40)   /* timer count start flag */
#define TA3     SFR16(0x4C)
#define TB2     SFR16(0x54)
#define TA3MOD  SFR8(0x59)
#define TB2MOD  SFR8(0x5D)
#define PMOD    SFR8(0x5E)   /* processor mode register */
#define WDT     SFR8(0x60)
#define WDTFS   SFR8(0x61)
#define TA3IC   SFR8(0x78)
#define TB2IC   SFR8(0x7C)

/* ---- RAM degiskenleri (isimler bilinmiyor, adresle anildi) ---- */
#define RAM8(a)   (*(volatile uint8_t  *)(a))
#define RAM16(a)  (*(volatile uint16_t *)(a))
#define g_flags_023A RAM16(0x023A)
#define g_flags_023C RAM16(0x023C)
#define g_flags_023E RAM16(0x023E)
#define g_flags_0240 RAM16(0x0240)
#define g_flags_0242 RAM16(0x0242)
#define g_flags_0244 RAM16(0x0244)
#define g_flags_0246 RAM16(0x0246)
#define g_flags_0248 RAM16(0x0248)
#define g_opt_0334   RAM8(0x0334)
#define g_opt_0335   RAM8(0x0335)
#define g_opt_033F   RAM8(0x033F)
#define g_delay_0346 RAM8(0x0346)
#define g_delay_034B RAM8(0x034B)

/* RAM 0x087A..0x087C : calisma zamaninda yazilan 3 baytlik kod
 *   NOP; WIT; RTL   (normal bekleme)  -- 0x00E30F'de yaziliyor
 *   0x087B'ye 0xDB (STP) yazilarak guc-kapatma moduna geciliyor (0x009776) */
extern void ram_wait_trampoline(void);   /* JSRL $00087A */

/* henuz cevrilmemis fonksiyonlar (adresleri YA4000D.asm'deki etiketler) */
void F_00CDE8(void); void F_00713E(void); void F_007E4C(uint16_t, uint16_t);
void F_01DE16(void); void F_019884(uint16_t); void F_00B98D(void); void F_0167F2(void);
void F_01A3EF(void); void F_018656(void); void F_01DAC4(void); void F_006696(void);
void F_01E013(void); void F_01691C(void); void F_0099AD(void); uint16_t F_01A664(void);
void F_00C91C(void); void F_00F3E8(void); void F_016767(void); void F_0198D5(void);

/* ------------------------------------------------------------------
 * 0x002000  RESET vektoru
 *   LDX #$0879 / TXS / JMPL $00FE60
 * 0x00FE60  NC77 baslangic kodu (ncrt0): DT=0, D=0x062A, .bss sifirlama
 *   (0x0082'den 0x046B bayt), bos .data kopyalamalari, sonra main().
 * ------------------------------------------------------------------ */

/* ------------------------------------------------------------------
 * 0x010630  set_options()  -- VARYANTLAR ARASINDAKI TEK KOD FARKI
 *   0x00D8EA'dan cagrilir. Her opsiyon (D,K,P,R,S,V,Y) bu fonksiyonu
 *   farkli derler; geri kalan tum kod aynidir.
 * ------------------------------------------------------------------ */
void set_options_D(void)          /* "DENIZ EL TELSIZI OPSIYONU" */
{
    g_opt_0334 &= ~0xF0;
    g_opt_0335 &= ~0x10;
}
void set_options_Y(void)          /* "KABLOSUZ SES YAYIN OPSIYONU" */
{
    g_opt_0334 &= ~0xF0;
    g_opt_033F &= ~0x80;
}
void set_options_P(void)
{
    g_opt_0334 &= ~0xF0;
    g_opt_033F &= ~0x80;
    g_opt_0335 &= ~0x10;
    g_flags_0246 |= 0x0100;
}
/* K: ayrica F_013280'i cagiriyor; R: 0x0246 |= 0x0200; V: 0x023E |= 0x0200; S: P'nin bayraksiz hali */

/* ------------------------------------------------------------------
 * 0x00D3B4  Sentezor kilidinin kararliligini kontrol et (Timer A3 ile sure).
 *   P67 = SENTEZOR_KILIT (kilavuz 3.6.2.3), AKTIF DUSUK: 0 = kilitli.
 *   char doner (M=1 ile RTL).
 *   Donus: 0 = kilit koptu (P67 = 1 goruldu), 1 = sure boyunca kilitli kaldi
 *          (mode != 0 ise P67'ye bakilmaz, sadece bekler).
 *   0x00D51D bunu P67 = 0 iken cagirir ve 1'i "kilit dogrulandi" sayar.
 *   Not: polarite simulatorde dogrulandi (sim/README.md); aktif yuksek
 *   varsayiminda orijinal firmware TX'i her 32 ms'de yeniden baslatiyor.
 * ------------------------------------------------------------------ */
uint8_t synth_lock_stable(uint16_t mode /* Y */)
{
    TCSTART &= ~0x08;                 /* TA3 durdur          */
    TA3IC    = 0x00;
    TA3MOD   = 0x80;
    TA3      = (uint16_t)(mode ? g_delay_034B : g_delay_0346) << 5;
    TA3IC   &= ~0x08;                 /* istek bayragini sil */
    TCSTART |=  0x08;                 /* TA3 baslat          */

    for (;;) {
        WDT = 0;                      /* watchdog besle      */
        if ((uint8_t)mode == 0 && (P6 & P6_SYNTH_LOCK))
            return 0;                 /* P67 = 1: kilit koptu */
        if (TA3IC & 0x08)             /* sure doldu          */
            break;
    }
    TCSTART &= ~0x08;
    TA3IC   &= ~0x08;
    return 1;
}

/* ------------------------------------------------------------------
 * 0x00E305  sistem/zamanlayici baslatma (parca)
 * ------------------------------------------------------------------ */
void sys_init(void)
{
    RAM8(0x087A) = 0xEA;   /* NOP */
    RAM8(0x087B) = 0xCB;   /* WIT */
    RAM8(0x087C) = 0x6B;   /* RTL */
    /* PHP / I-bayragi ayari ... */
    __asm("CLI");

    TB2MOD = 0x80;
    TB2    = 0x0FBF;
    TCSTART |= 0x80;       /* TB2 baslat (periyodik tick) */
    TB2IC  = 0x02;         /* TB2 kesmesi, oncelik 2      */
    /* 8.064 MHz kristal; TB2 = 0x0FBF -> 4032'ye bolme. TB2MOD=0x80 sayim kaynagini secer;
       veri sayfasindaki bit 7-6 tablosuna gore f64 ise tick = 8.064MHz/64/4032 = ~31 Hz (32 ms). Dogrulanmadi. */

    g_flags_023A = g_flags_023C = g_flags_023E = g_flags_0240 = 0;
    g_flags_0242 = g_flags_0244 = g_flags_0246 = g_flags_0248 = 0;
    RAM8(0x01EB) = 0x04;
    RAM8(0x01E9) = 0x00;
    F_00CDE8();
    RAM8(0x0228) = 0x0D;
    /* ... devami YA4000D.asm 0x00E372 ... */
}

/* ------------------------------------------------------------------
 * 0x01A6D5  main()  (geri donmez)
 * ------------------------------------------------------------------ */
void main(void)
{
    PMOD = 0x26;
    P4 = 0x00;                   /* ayrica 0x02,0x03,0x06,0x07'ye 0 yaziliyor (bos adresler) */
    P5 = 0x02;  P6 = 0x02;  P7 = 0x00;  P8 = 0x0A;
    P4D = 0x4C;   /* P43 XSHIFT, P46 cikis                         */
    P5D = 0x7F;   /* P50-P56 cikis, P57 SUSTURMA giris             */
    P6D = 0x02;   /* P61 SCL cikis (SDA open-drain, yonle suruluyor) */
    P7D = 0x07;   /* P70 SENT_YUKLE, P71 GUC_ACMA, P72 cikis       */
    P8D = 0xAA;   /* P81 SENT_SEC, P83 TxD0, P85 CLK1, P87 TxD1    */
    WDTFS = 0; WDT = 0;

    F_00713E();
    F_007E4C(1, 1);
    sys_init();                  /* F_00E305 */
    F_01DE16();
    F_019884(0x19);
    F_00B98D(); F_0167F2(); F_01A3EF(); F_018656(); F_01DAC4();
    F_006696(); F_01E013();
    if (g_opt_0335 & 0x10)   F_01691C();
    if (g_flags_0240 & 0x0100) F_0099AD();

    for (;;) {                   /* ana dongu, 0x01A776 */
        WDT = 0;
        if (F_01A664())
            ram_wait_trampoline();      /* WIT: kesmeye kadar uyu */
        WDT = 0;
        if (g_flags_0246 & 0x02) {
            F_00C91C(); F_00F3E8(); F_016767(); F_0198D5();
            /* ... */
        }
        /* ... devami YA4000D.asm ... */
    }
}

/* ------------------------------------------------------------------
 * 0x000080 (ROM 0x010100-0x0105C7)  RAM'e kopyalanip calisan flash yukleyici.
 *   0x0105F3'teki dongu bu blogu RAM 0x0080'e kopyalar ve JSRL $000080 yapar.
 *   Icerik: UART0 uzerinden veri alma (0x0035/0x0036), JEDEC flash yazma
 *   komut dizisi  [$5555]=AA, [$2AAA]=55, [$5555]=A0 / F0  -> AMD/Atmel tipi
 *   5V paralel flash (29Cxxx/29Fxxx ailesi). TA3 ile zaman asimi.
 * ------------------------------------------------------------------ */
