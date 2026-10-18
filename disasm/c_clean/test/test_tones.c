/* Cross-check: TA1 sample sequence of the clean tone generator (tones.c)
 * against the original TB1 interrupt translated in ../c_lifted. */
#include <stdio.h>
#include "../../c_lifted/m7700.h"
#include "../../c_lifted/functions.h"
#include "../tones.h"
#include "../tone_tables.h"

uint8_t test_regs[0x80];
extern uint8_t ram[0x880];
uint8_t sfr_read(uint8_t a) { return test_regs[a]; }
void sfr_write(uint8_t a, uint8_t v) { test_regs[a] = v; }
volatile uint8_t hal_dummy;
void hal_irq_disable(void) {}
void hal_irq_enable(void) {}
void hal_ipl_set(uint8_t l) { (void)l; }
static uint16_t ta1(void) { return (uint16_t)(test_regs[0x48] | test_regs[0x49] << 8); }

static int compare(int dual, uint8_t inc1, uint8_t inc2, void (*start)(void))
{
    int bad = 0;
    for (int a = 0x80; a < 0x880; a++) ram[a] = 0;
    ram[0x023E] = 0x00;                         /* TX: bit 4 clear */
    ram[0x023D] = dual ? 0x02 : 0x00;           /* 0x023C bit 9 */
    ram[0x0260] = inc1; ram[0x0262] = inc2;
    start();
    for (int k = 0; k < 2000; k++) {
        D = 0x0640; S = 0x0850; isr_TB1();
        uint16_t orig = ta1();
        tones_tb1_isr();
        if (ta1() != orig) bad++;
    }
    return bad;
}
static uint8_t cur_key, cur_digit, cur_std;
static void s_dtmf(void) { tone_start_dtmf(cur_key, 50); }
static void s_sel(void)  { tone_start_selcall(cur_std, cur_digit); }

int main(void)
{
    int bad = 0;
    for (cur_key = 0; cur_key < 12; cur_key++)
        bad += compare(1, dtmf_table[cur_key].inc_low, dtmf_table[cur_key].inc_high, s_dtmf);
    for (cur_std = 0; cur_std < 3; cur_std++)
        for (cur_digit = 0; cur_digit < 15; cur_digit++)
            bad += compare(0, selcall_std[cur_std].inc[cur_digit], 0, s_sel);
    printf("TA1 sample mismatches: %d of %d\n", bad, (12 + 45) * 2000);
    return bad != 0;
}
