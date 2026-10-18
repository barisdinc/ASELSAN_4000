/* Cross-check of the clean CTCSS decoder (ctcss.c) against the original
 * firmware's interrupt code, translated mechanically (../c_lifted).
 * Both are fed the same P65 square wave / noise and must reach the same
 * tone-present decision after every 80-sample block. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../c_lifted/m7700.h"
#include "../../c_lifted/functions.h"
#include "../ctcss.h"
#include "../tone_tables.h"

uint8_t test_regs[0x80];
extern uint8_t ram[0x880];
uint8_t sfr_read(uint8_t a) { return test_regs[a]; }
void sfr_write(uint8_t a, uint8_t v) { test_regs[a] = v; }
volatile uint8_t hal_dummy;
void hal_irq_disable(void) {}
void hal_irq_enable(void) {}
void hal_ipl_set(uint8_t l) { (void)l; }

static void call_isr(void (*f)(void)) { D = 0x0640; S = 0x0850; DT = 0; f(); }

static int run(int tone, double fin, int noise, unsigned seed, int *blocks_present)
{
    srand(seed);
    for (int a = 0x80; a < 0x880; a++) ram[a] = 0;
    ram[0x0375] = (uint8_t)tone; ram[0x023E] = 0x10;          /* RX mode for the TB0 handler */
    D = 0x0640; S = 0x0850; mflag = 0; xflag = 0;
    S -= 3; ctcss_rx_start_00F8BD();
    ctcss_rx_start((uint8_t)tone, 0, 0);
    double fs = 4.032e6 / (ctcss_table[tone - 1].rx_div + 1);
    double ph = (rand() % 1000) / 1000.0 * 6.2832, last_rise = 0;
    int prev = 0, mismatches = 0, present = 0;
    for (int k = 0; k < 80 * 60; k++) {
        double t = k / fs;
        int v = noise ? (rand() & 1) : (sin(6.2832 * fin * t + ph) > 0);
        if (v && !prev) {                               /* rising edge -> TB0 period interrupt */
            uint16_t per = (uint16_t)((t - last_rise) * 504000.0);
            last_rise = t;
            test_regs[0x50] = (uint8_t)per; test_regs[0x51] = (uint8_t)(per >> 8);
            call_isr(isr_TB0); ctcss_tb0_isr();
        }
        prev = v;
        test_regs[0x0E] = v ? 0x20 : 0x00;              /* P65 */
        call_isr(isr_TA4); ctcss_ta4_isr();
        if (k % 80 == 79) {
            int orig = (ram[0x023B] & 0x10) != 0;       /* 0x023A bit 12 */
            if (orig != ctcss_present()) mismatches++;
            present += orig;
        }
    }
    *blocks_present = present;
    return mismatches;
}

int main(void)
{
    int tones[] = {1, 6, 13, 20, 27, 38, 39, 45, 50}, bad = 0, n = 0;
    for (unsigned i = 0; i < sizeof tones / sizeof *tones; i++) {
        int t = tones[i], p1, p2, p3;
        double f = 4.032e6 / (ctcss_table[t - 1].rx_div + 1) / 4;
        int m1 = run(t, f, 0, 1 + i, &p1);              /* correct tone   */
        int m2 = run(t, f * 1.06, 0, 100 + i, &p2);     /* ~ next tone up */
        int m3 = run(t, 0, 1, 200 + i, &p3);            /* noise          */
        printf("tone %2d %6.1f Hz: mismatches %d/%d/%d   present blocks: on-tone %2d/60  off-tone %2d/60  noise %2d/60\n",
               t, f, m1, m2, m3, p1, p2, p3);
        bad += m1 + m2 + m3; n += 3 * 60;
    }
    printf("total decision mismatches: %d of %d\n", bad, n);
    return bad != 0;
}
