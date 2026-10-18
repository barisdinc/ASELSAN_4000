/* tones.c - DTMF and selective-call (Select-5) tone generation.
 *
 * Same DDS principle as CTCSS, on other timers:
 *   TB1 (timer mode, f2)   sample clock, one interrupt per sample
 *   TA1 (one-shot, P52)    pulse width = sine sample -> TD14 filter -> FX806
 *   TA4 (timer, f64)       tone duration, polled (TA4 interrupt is disabled)
 * DTMF adds two phase accumulators (low and high group tables); a single tone
 * uses selcall_sine.  Every TB1 interrupt re-triggers the TA1 one-shot.
 *
 * Clean rewrite of: 0x005282 (DTMF setup), 0x00E8C2.. (selcall tone setup,
 * inside 0x00E65B), 0x01C627 TX half of the TB1 interrupt.
 * Receive-side selcall decoding (TB1 period measurement + TA1 window,
 * 0x007CAE / 0x01C6C0) is still only available in ../c_lifted.
 */
#include "hal.h"
#include "tones.h"
#include "tone_tables.h"

static struct {
    volatile uint8_t dual;              /* 0x023C bit 9 */
    uint8_t ph1, inc1, ph2, inc2;       /* 0x025F, 0x0260, 0x0261, 0x0262 */
} g;

static void timers_setup(uint16_t tb1_div, uint16_t duration_counts)
{
    TA1IC = 0;
    TA1MOD = 0x06;                      /* one-shot pulse, triggered via TOSTART */
    TA1IC &= (uint8_t)~IC_IR;
    TB1IC = 6;                          /* priority 6 */
    TB1MOD = 0x00;                      /* timer, f2 = 4.032 MHz */
    TB1IC &= (uint8_t)~IC_IR;
    TA4IC = 0;                          /* duration: polled */
    TA4MOD = 0x80;                      /* timer, f64 = 126 kHz -> 126 counts per ms */
    TA4 = duration_counts;
    TA4IC &= (uint8_t)~IC_IR;
    TB1 = tb1_div;
    g.ph1 = g.ph2 = 0;
}

/* key: 0..11 = '1'..'9', '*', '0', '#' (table order of ROM 0x002443) */
void tone_start_dtmf(uint8_t key, uint16_t ms)
{
    const dtmf_entry_t *e = &dtmf_table[key % 12];
    timers_setup(e->tb1_div, (uint16_t)(ms * 126u));
    hal_ipl_set(2);
    g.inc1 = e->inc_low;
    g.inc2 = e->inc_high;
    g.dual = 1;
    TA1 = (uint16_t)(dtmf_sine_low[0] + dtmf_sine_high[0]);
    TCSTART |= TS_TA4 | TS_TB1 | TS_TA1;
}

/* std: 0 = CCIR, 1 = ZVEI1, 2 = EEA ; digit 0..14 (0-9, A-D, E = repeat) */
void tone_start_selcall(uint8_t std, uint8_t digit)
{
    const selcall_std_t *s = &selcall_std[std % 3];
    if (digit > 14) return;             /* 0x0F = end of number in the original */
    timers_setup(s->tb1_div[digit], (uint16_t)(s->tone_ms * 126u));
    hal_ipl_set(1);
    g.inc1 = s->inc[digit];
    g.dual = 0;
    TA1 = selcall_sine[0];
    TCSTART |= TS_TA4 | TS_TB1 | TS_TA1;
}

int tone_done(void) { return (TA4IC & IC_IR) != 0; }

void tone_stop(void)
{
    TCSTART &= (uint8_t)~(TS_TB1 | TS_TA1 | TS_TA4);
    TB1IC = 0;
    hal_ipl_set(0);
}

/* TX half of the TB1 interrupt (0x01C663..0x01C6B9) */
void tones_tb1_isr(void)
{
    TOSTART |= 0x02;                    /* re-trigger TA1 one-shot */
    if (g.dual) {
        g.ph2 = (uint8_t)(g.ph2 + g.inc2);
        g.ph1 = (uint8_t)(g.ph1 + g.inc1);
        TA1 = (uint16_t)(dtmf_sine_high[g.ph2] + dtmf_sine_low[g.ph1]);
    } else {
        g.ph1 = (uint8_t)(g.ph1 + g.inc1);
        TA1 = selcall_sine[g.ph1];
    }
}

/* Standard Select-5 rule: a digit equal to the previous one is sent as the
 * repeat tone E.  (Where the original applies this was not traced yet.) */
uint8_t selcall_apply_repeat(const uint8_t *in, uint8_t n, uint8_t *out)
{
    for (uint8_t k = 0; k < n; k++)
        out[k] = (k && in[k] == in[k - 1] && out[k - 1] != 14) ? 14 : in[k];
    return n;
}

/* Blocking send of a digit string, one tone after another. */
void selcall_send(uint8_t std, const uint8_t *digits, uint8_t n)
{
    uint8_t seq[8];
    if (n > sizeof seq) n = sizeof seq;
    selcall_apply_repeat(digits, n, seq);
    for (uint8_t k = 0; k < n; k++) {
        tone_start_selcall(std, seq[k]);
        while (!tone_done()) WDT_KICK();
    }
    tone_stop();
}
