/* test_synth.c - synth_compute() against the bytes the ORIGINAL firmware sent
 * to the MB1502 in the simulator (../sim, probe with EEPROM base / channel
 * words below; R bytes, then N bytes in transmit order). */
#include <stdio.h>
#include <string.h>
#include "../synth.h"

unsigned char test_regs[0x80];
volatile uint8_t hal_dummy;
void hal_irq_disable(void) {}
void hal_irq_enable(void) {}
void hal_ipl_set(uint8_t l) { (void)l; }

static const struct { uint16_t base; uint16_t word; int tx; uint8_t r[2], n[3]; uint32_t vco; } V[] = {
    { 5440, 8000, 0, {0x11, 0x80}, {0x00, 0xdf, 0x04}, 201000000 },
    { 5440, 8000, 1, {0x11, 0x80}, {0x00, 0xc3, 0x00}, 156000000 },
    { 5440, 8003, 0, {0x29, 0x80}, {0x40, 0x2e, 0x48}, 201005000 },
    { 5440, 8004, 1, {0x51, 0x80}, {0x00, 0xcf, 0x46}, 156010000 },
    { 5440, 5605, 0, {0x11, 0x80}, {0x00, 0xcf, 0x46}, 195012500 },
    { 5440, 3602, 1, {0x29, 0x80}, {0x80, 0xa3, 0x48}, 145005000 },
    {16000,12005, 0, {0x10, 0x80}, {0x00, 0x0f, 0x45}, 385012500 },
    {16000,10010, 1, {0x10, 0x80}, {0x80, 0x90, 0x25}, 425025000 },
    { 2720,  800, 0, {0x11, 0x80}, {0x00, 0xf1, 0x06}, 115000000 },
    { 2720,  805, 1, {0x11, 0x80}, {0x00, 0xea, 0x42}, 70012500 },
};

int main(void)
{
    int bad = 0;
    for (unsigned k = 0; k < sizeof V / sizeof V[0]; k++) {
        synth_cfg_t cfg; synth_words_t w;
        synth_cfg_from_eeprom(V[k].base, 0, &cfg);
        synth_compute(&cfg, V[k].word, V[k].tx ? 0 : cfg.rx_offset, &w);
        if (memcmp(w.r, V[k].r, 2) || memcmp(w.n, V[k].n, 3)) {
            printf("mismatch base %u word %u %s: got %02x %02x / %02x %02x %02x\n", V[k].base, V[k].word,
                   V[k].tx ? "TX" : "RX", w.r[0], w.r[1], w.n[0], w.n[1], w.n[2]);
            bad++;
        }
    }
    printf("synth: %d/%u mismatches against the original firmware\n", bad, (unsigned)(sizeof V / sizeof V[0]));
    return bad != 0;
}
