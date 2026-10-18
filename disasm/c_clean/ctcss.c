/* ctcss.c - TKS (CTCSS) encoder and decoder, 50 EIA tones.
 *
 * Encoder: DDS.  TB0 runs as a sample clock (rate from ctcss_table.tx_div);
 *   each TB0 interrupt advances an 8-bit phase and writes the sine table to
 *   the TA2 8-bit PWM (P54 TKS_URET), which TD8 low-pass filters.
 * Decoder: P65 carries the 250 Hz low-passed, zero-crossing-detected receive
 *   audio as a 1-bit square wave.  TA4 samples it at 4 x the tone frequency
 *   and correlates it with +/-1 sine and cosine references (I and Q).  Every
 *   80 samples (20 tone periods) |I|,|Q| are combined to a magnitude and
 *   compared with a threshold with hysteresis.  TB0, in period-measurement
 *   mode, counts fast edges on P65 so that noise/voice can veto the decision.
 *
 * Clean rewrite of: 0x00F854 tx start, 0x00F8AE tx stop, 0x00F8BD rx start,
 * 0x00FB0B TB0 interrupt, 0x00F94C TA4 interrupt.  Constants are the originals.
 */
#include "hal.h"
#include "ctcss.h"
#include "tone_tables.h"

#define P65_HIGH()  ((P6 & 0x20) != 0)

static struct {
    volatile uint8_t rx_mode;        /* original: flag word 0x023E bit 4 */
    volatile uint8_t reverse;        /* 0x023A bit 14: run the phase backwards */
    uint8_t phase, inc;              /* 0x01AC, 0x01AD */
    /* decoder */
    int8_t  i_acc, q_acc;            /* 0x01AF, 0x01B0 */
    uint8_t samples_left;            /* 0x01B1 */
    uint8_t threshold;               /* 0x01AE: 40, 30 while a tone is present */
    uint8_t toggle_a, toggle_b;      /* 0x0240 bit 12 / bit 11 */
    uint16_t edges, fast_edges;      /* 0x01B2, 0x01B4 */
    volatile uint8_t present;        /* 0x023A bit 12: tone detected (squelch open) */
    uint8_t noise;                   /* 0x0240 bit 13 */
    uint8_t hang;                    /* 0x020A (non-zero = hang timer running) */
    uint8_t hang_enable;             /* option 0x0244 bit 11 */
} c;

/* ---------------------------------------------------------------- encoder */

/* 0x00F854 */
void ctcss_tx_start(uint8_t tone)
{
    if (tone == 0 || tone > CTCSS_TONES) return;
    const ctcss_entry_t *e = &ctcss_table[tone - 1];
    c.rx_mode = 0;
    c.inc = e->tx_inc;
    TA2IC = 0;
    TA2MOD = 0x27;                  /* 8-bit PWM, f2 */
    TA2_H = 0;
    TA2 = 0;
    TB0IC = 5;                      /* priority 5 */
    TB0MOD = 0x00;                  /* timer mode, f2 = 4.032 MHz */
    TB0 = e->tx_div;
    TA2IC &= (uint8_t)~IC_IR;
    TB0IC &= (uint8_t)~IC_IR;
    TCSTART |= TS_TA2;
    TCSTART |= TS_TB0;
}

/* 0x00F8AE */
void ctcss_tx_stop(void)
{
    TCSTART &= (uint8_t)~TS_TB0;
    TCSTART &= (uint8_t)~TS_TA2;
    TB0IC = 0;
}

void ctcss_tx_reverse(int on) { c.reverse = (uint8_t)(on != 0); }

/* ---------------------------------------------------------------- decoder */

/* 0x00F8BD.  initially_present: original sets 0x023A bit 12 when
 * option bits 0x0244.11 and 0x0244.14 are both set. */
void ctcss_rx_start(uint8_t tone, int initially_present, int hang_enable)
{
    if (tone == 0 || tone > CTCSS_TONES) return;
    c.rx_mode = 1;
    c.present = (uint8_t)(initially_present != 0);
    c.hang_enable = (uint8_t)(hang_enable != 0);
    c.noise = 0;
    c.edges = c.fast_edges = 0;
    c.toggle_a = 1;
    c.threshold = 40;
    c.samples_left = 80;
    c.i_acc = c.q_acc = 0;
    TB0IC = 5;
    TB0MOD = 0x4A;                  /* pulse period measurement, f16 = 504 kHz */
    TA4IC = 6;
    TA4MOD = 0x00;                  /* timer mode, f2 */
    TA4 = ctcss_table[tone - 1].rx_div;   /* 4 x tone frequency */
    TCSTART |= TS_TA4;
    TCSTART |= TS_TB0;
}

int  ctcss_present(void) { return c.present; }
void ctcss_set_hang(uint8_t ticks) { c.hang = ticks; }

/* TB0 interrupt (0x00FB0B) */
void ctcss_tb0_isr(void)
{
    if (c.rx_mode) {
        /* period measured in f16 counts: < 840 (above ~600 Hz) = fast edge,
           >= 4201 (below ~120 Hz) slowly forgives earlier fast edges */
        uint16_t period = TB0;
        c.edges++;
        if (period < 0x0348) c.fast_edges++;
        else if (period >= 0x1069 && c.fast_edges) c.fast_edges--;
    } else {
        c.phase = c.reverse ? (uint8_t)(c.phase - c.inc) : (uint8_t)(c.phase + c.inc);
        TA2_H = ctcss_sine[c.phase];
    }
}

static uint8_t iabs8(int8_t v) { return (uint8_t)(v < 0 ? -v : v); }

/* TA4 interrupt (0x00F94C) */
void ctcss_ta4_isr(void)
{
    uint8_t in = P65_HIGH();
    /* references alternate so that I and Q see +1 +1 -1 -1 shifted by 90 deg */
    if (c.toggle_a) {
        c.toggle_a = 0;
        if (c.toggle_b) { c.i_acc += in ? -1 : 1; c.toggle_b = 0; }
        else            { c.i_acc += in ? 1 : -1; c.toggle_b = 1; }
    } else {
        c.toggle_a = 1;
        if (c.toggle_b) c.q_acc += in ? -1 : 1;
        else            c.q_acc += in ? 1 : -1;
    }

    if (--c.samples_left) return;
    c.samples_left = 80;

    TCSTART &= (uint8_t)~TS_TB0;               /* snapshot the edge counters */
    uint16_t fast = c.fast_edges;
    uint16_t slow = (uint16_t)(c.edges - c.fast_edges);
    c.edges = c.fast_edges = 0;
    TCSTART |= TS_TB0;

    uint8_t prev = c.present;
    uint8_t mx = iabs8(c.i_acc), mn = iabs8(c.q_acc);
    if (mn > mx) { uint8_t t = mx; mx = mn; mn = t; }
    int8_t mag = (int8_t)(mx + (mn >> 2));    /* |z| ~ max + min/4 */
    c.i_acc = c.q_acc = 0;

    if (fast >= 0x65) {
        c.noise = 1;                            /* too many fast edges: noise/voice */
    } else if (slow >= 0x0B) {                  /* enough slow (tone-like) edges */
        if ((uint16_t)(slow * 10 - fast) >= 0x3E9) c.noise = 1;
        if (fast < 0x5A) mag = (int8_t)(mag + (fast >= 0x29 ? 5 : (fast >> 2)));
    }

    uint8_t det;
    if ((int8_t)(mag - (int8_t)c.threshold) >= 0 && !c.noise) {
        det = 1; c.threshold = 30;
    } else {
        det = 0; c.threshold = 40; c.noise = 0;
    }

    if (det != prev) {
        if (det || (prev && c.hang && c.hang_enable)) c.present = 1;
        else c.present = 0;
    }
    /* original also sets 0x023E bit 13 and clears 0x01E9 here when 0x023E bit 0
       is set (wakes the battery-save loop) - left to the caller */
}
