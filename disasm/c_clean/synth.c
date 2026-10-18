/* synth.c - Fujitsu MB1502 PLL (TD201 on the RF board).
 *
 * The radio sends two serial words through UART1 in clock-synchronous mode:
 *   1. the 16-bit reference word (SW, R14..R1, C=1), latched with a pulse on P70 (LE)
 *   2. the 19-bit divider word (N11..N1, A7..A1, C=0) in 3 bytes, latched by LE
 * UART1 shifts LSB first while the MB1502 expects MSB first, so every byte
 * goes through the bit-reverse table at ROM 0x2B06.
 * P81 selects the synthesizer on the serial line that is shared with C-BUS.
 *
 * Lock detect P67 is ACTIVE LOW (0 = locked).  The four routines that use it
 * (0x00D51D, 0x00D3B4, 0x00EE14 and the state machine 0x00F2A1) are only
 * consistent this way; verified in the simulator (../sim): with P67 = 1 for
 * "locked" the original firmware re-keys TX every 32 ms.
 *
 * Frequency plan (the EEPROM stores 14-bit channel words, not synthesizer words):
 *   f = (base*10 + word) * 2.5 kHz          base = EEPROM 0x58C..0x58D (25 kHz units)
 *   RX VCO = f + 45 MHz (base < 250 MHz, prescaler 64/65)
 *          = f - 45 MHz (base >= 250 MHz, prescaler 128/129)
 *   TX VCO = f
 *   step 12.5 kHz if word % 5 == 0, 10 kHz if word % 4 == 0, else 5 kHz
 *   (an odd word on the 5 kHz path loses 2.5 kHz - the original does the same)
 *
 * Clean rewrite of 0x00DA32 (config), 0x0088B8, 0x0087DE, 0x008825, 0x008854
 * (word computation), 0x00D2A3 (RX/TX select), 0x00D45C (load), 0x00D3B4.
 * test/test_synth.c checks synth_compute against bytes the original firmware
 * sent in the simulator.
 */
#include "hal.h"
#include "synth.h"

static uint8_t rev8(uint8_t b)          /* ROM table 0x2B06 */
{
    b = (uint8_t)((b & 0xF0) >> 4 | (b & 0x0F) << 4);
    b = (uint8_t)((b & 0xCC) >> 2 | (b & 0x33) << 2);
    return (uint8_t)((b & 0xAA) >> 1 | (b & 0x55) << 1);
}

/* 0x00DA32 (part) */
void synth_cfg_from_eeprom(uint16_t base, uint8_t refsel, synth_cfg_t *cfg)
{
    cfg->base = base;
    cfg->fref_param = refsel ? 130000u : 128000u;
    if (base >= 10001u) { cfg->prescaler = 128; cfg->rx_offset = -18000; }   /* >= 250 MHz */
    else                { cfg->prescaler = 64;  cfg->rx_offset =  18000; }
}

uint32_t synth_word_to_hz(const synth_cfg_t *cfg, uint16_t word)
{
    return ((uint32_t)cfg->base * 10u + word) * 2500u;
}

/* 0x0088B8 + 0x0087DE + 0x008825 + 0x008854 */
void synth_compute(const synth_cfg_t *cfg, uint16_t word, int16_t offset, synth_words_t *w)
{
    int32_t total = (int32_t)offset + (int32_t)cfg->base * 10 + word;
    uint8_t code;
    if (word % 5 == 0)       { total /= 5; code = 125; }    /* 12.5 kHz */
    else if ((word & 3) == 0) { total /= 4; code = 100; }   /* 10 kHz */
    else                     { total /= 2; code = 50; }     /* 5 kHz */

    uint16_t n = (uint16_t)(total / cfg->prescaler);
    uint8_t  a = (uint8_t)(total % cfg->prescaler);
    uint16_t r = (uint16_t)(cfg->fref_param / code);
    uint16_t rw = (uint16_t)(r << 1 | 1u | (cfg->prescaler == 128 ? 0 : 0x8000u));   /* SW R14..R1 C */

    w->r[0] = rev8((uint8_t)(rw >> 8));
    w->r[1] = rev8((uint8_t)rw);
    w->n[0] = rev8((uint8_t)(n >> 8));
    w->n[1] = rev8((uint8_t)n);
    w->n[2] = rev8((uint8_t)(a << 1));                      /* A7..A1, C = 0 */
}

/* 0x00D2A3: RX uses the RX word plus the injection offset and sets the
 * crystal shift (P43) from the channel; TX uses the TX word without offset. */
void synth_words_for_channel(const synth_cfg_t *cfg, const channel_t *ch, int tx, synth_words_t *w)
{
    if (tx) synth_compute(cfg, CH_TX_WORD(ch), 0, w);
    else {
        synth_compute(cfg, CH_RX_WORD(ch), cfg->rx_offset, w);
        XSHIFT(CH_XSHIFT(ch->rx_flags));
    }
}

static void put(uint8_t b)
{
    SER1_WAIT_EMPTY();
    SER1_PUT(b);
}

/* 0x00D45C */
void synth_load(const synth_words_t *w)
{
    hal_irq_disable();
    SER1_OPEN_SYNC();
    SYNTH_SELECT(1);
    SYNTH_LE(0);

    SER1_PUT(w->r[0]);            /* first byte: buffer is empty after open */
    put(w->r[1]);
    SER1_WAIT_EMPTY();
    SER1_WAIT_DONE();
    SYNTH_LE(1);                  /* latch reference word */
    SYNTH_LE(0);

    put(w->n[0]);
    put(w->n[1]);
    put(w->n[2]);
    SER1_WAIT_EMPTY();
    SER1_WAIT_DONE();
    SYNTH_LE(1);                  /* latch divider word */
    SYNTH_SELECT(0);
    SER1_CLOSE();
    hal_irq_enable();
}

/* 0x00D3B4 - lock stability check with a Timer A3 timeout.
 * timeout_ticks is the original (byte from RAM 0x0346 or 0x034B) << 5,
 * counted by TA3 in timer mode (TA3MOD = 0x80, f64 = 126 kHz).
 * mode == 0: returns 0 as soon as the loop is seen unlocked (P67 = 1),
 *            1 when it stayed locked for the whole time.
 * mode != 0: plain delay, returns 1.
 * 0x00D51D calls it while P67 = 0 and treats 1 as "lock confirmed". */
uint8_t synth_lock_stable(uint8_t mode, uint16_t timeout_ticks)
{
    TCSTART &= (uint8_t)~TS_TA3;
    TA3IC = 0;
    TA3MOD = 0x80;
    TA3 = timeout_ticks;
    TA3IC &= (uint8_t)~IC_IR;
    TCSTART |= TS_TA3;
    for (;;) {
        WDT_KICK();
        if (mode == 0 && !SYNTH_LOCKED()) return 0;
        if (TA3IC & IC_IR) break;
    }
    TCSTART &= (uint8_t)~TS_TA3;
    TA3IC &= (uint8_t)~IC_IR;
    return 1;
}
