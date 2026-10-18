/* Runs the lifted firmware from the reset vector with deterministic SFR reads
 * and prints every SFR / global-RAM write; compare with tools/emu7700.py. */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "../m7700.h"
#include "../functions.h"

static jmp_buf done;
static long limit, n;
static uint32_t seed;

uint8_t sfr_read(uint8_t a) { (void)a; seed = seed * 1103515245u + 12345u; return (uint8_t)(seed >> 16); }
void mem_trace(uint32_t a, uint8_t v)
{
    printf("%04X %02X\n", (unsigned)a, v);
    if (++n >= limit) longjmp(done, 1);
}
void cpu_brk(void) { longjmp(done, 2); }

int main(int argc, char **argv)
{
    limit = argc > 1 ? atol(argv[1]) : 100000;
    seed = getenv("SEED") ? (uint32_t)atol(getenv("SEED")) : 12345u;
    if (!setjmp(done)) reset_entry();
    return 0;
}
