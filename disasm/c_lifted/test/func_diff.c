/* Per-function differential test driver (C side).  For every lifted function and
 * several random trials: seed RAM/registers/SFR reads from one LCG, call the
 * function, print every SFR/global write and the final register state.
 * tools/emu7700.py --funcs produces the same output from the machine code. */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "../m7700.h"
#include "../rom_image.h"

typedef struct { uint32_t addr; void (*fn)(void); uint8_t mstates; uint8_t retsize; uint8_t isr; } func_t;
extern const func_t func_table[]; extern const int func_count;
extern uint8_t ram[0x880];

static jmp_buf jb; static uint32_t seed; static long nev, nrd, ntick, trials;
#define EV_LIMIT 3000
#define RD_LIMIT 20000
#define TICK_LIMIT 2000000
static uint32_t lcg(void) { seed = seed * 1103515245u + 12345u; return seed >> 16 & 0xFFFF; }
uint8_t sfr_read(uint8_t a) { (void)a; if (++nrd > RD_LIMIT) longjmp(jb, 'Q'); return (uint8_t)lcg(); }
static int dbg; void mem_trace(uint32_t a, uint8_t v) { if (dbg) printf("%04X %02X r%ld\n", (unsigned)a, v, nrd); else printf("%04X %02X\n", (unsigned)a, v); if (++nev >= EV_LIMIT) longjmp(jb, 'L'); }
void cpu_brk(void) { longjmp(jb, 'K'); }
void cpu_stop(void) { longjmp(jb, 'K'); }
void bad_dispatch(uint32_t at) { (void)at; longjmp(jb, 'X'); }
void indirect_jump(uint16_t p) { (void)p; longjmp(jb, 'X'); }
void unknown_switch(uint32_t at) { (void)at; longjmp(jb, 'X'); }
void tick_hook(void) { if (++ntick > TICK_LIMIT) longjmp(jb, 'T'); }

int main(int argc, char **argv)
{
    trials = argc > 1 ? atol(argv[1]) : 3;
    dbg = getenv("DBG") != 0;
    for (int f = 0; f < func_count; f++)
        for (int k = 0; k < trials; k++) {
            const func_t *F = &func_table[f];
            seed = F->addr * 7u + (uint32_t)k * 1000003u;
            for (int a = 0x80; a < 0x880; a++) ram[a] = (uint8_t)lcg();
            if (F->addr < 0x2000)  /* RAM-resident flash loader: install its code first */
                for (int a = 0x10100; a < 0x105C8; a++) ram[a - 0x10100 + 0x80] = rom_image[a];
            A = lcg(); B = lcg(); X = lcg() & 0x0E; Y = lcg(); DT = 0; xflag = 0; I = 1; IPL = 0;
            C = Z = N = V = 0; PG = (uint8_t)(F->addr >> 16);
            mflag = F->mstates == 2 ? 1 : (F->mstates == 3 ? (lcg() & 1) : 0);
            D = 0x0640; S = 0x0870;
            if (!F->isr) { ram[0x870] = 0x06; ram[0x86F] = 0x40; S = 0x086E; S -= F->retsize; }
            nev = nrd = ntick = 0;
            printf("T %06X %d\n", (unsigned)F->addr, k);
            int st = setjmp(jb);
            if (!st) { F->fn(); st = 'R'; }
            printf("E %c %04X %04X %04X %04X %04X %04X %d\n", st, A, B, X, Y, D, S, mflag);
        }
    return 0;
}
