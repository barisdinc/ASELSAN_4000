/* m7700_rt.c - CPU helpers for the lifted code (flag semantics of MELPS 7700). */
#include "m7700.h"
#include "rom_image.h"

uint16_t A, B, X, Y, S, D;
uint8_t  DT, PG;
uint8_t  C, Z, N, V, I, mflag, xflag, IPL;

uint8_t ram[0x880];   /* internal RAM (non-static so tests can seed it) */

/* Hook these up to real/emulated peripherals. */
__attribute__((weak)) uint8_t sfr_read(uint8_t a)            { (void)a; return 0; }
__attribute__((weak)) void    sfr_write(uint8_t a, uint8_t v) { (void)a; (void)v; }

uint8_t rd8(uint32_t a)
{
    a &= 0xFFFFFF;
    if (a < 0x80) return sfr_read((uint8_t)a);
    if (a < 0x880) return ram[a];
    if (a >= 0x2000 && a < 0x20000) return rom_image[a];
    return 0xFF;
}
__attribute__((weak)) void mem_trace(uint32_t a, uint8_t v) { (void)a; (void)v; }

void wr8(uint32_t a, uint8_t v)
{
    a &= 0xFFFFFF;
    if (a < 0x600) mem_trace(a, v);      /* SFR + global RAM write trace (tests) */
    if (a < 0x80) sfr_write((uint8_t)a, v);
    else if (a < 0x880) ram[a] = v;
    /* writes to 0x5555/0x2AAA etc. reach the 29F010 in hardware (flash programming) */
}

uint16_t getP(void) { return (uint16_t)(C | Z << 1 | I << 2 | xflag << 4 | mflag << 5 | V << 6 | N << 7 | (IPL & 7) << 8); }
void setP(uint16_t p) { C = p & 1; Z = p >> 1 & 1; I = p >> 2 & 1; xflag = p >> 4 & 1; mflag = p >> 5 & 1; V = p >> 6 & 1; N = p >> 7 & 1; IPL = p >> 8 & 7; }
void sep(uint8_t m) { setP(getP() | m); }
void clp(uint8_t m) { setP(getP() & (uint16_t)~m); }

/* PSH/PUL (MELPS 7700 manual): b0 A, b1 B, b2 X, b3 Y, b4 DPR, b5 DT, b6 PG, b7 PS.
 * PSH saves A first and PS last; PUL restores in the reverse order. PS is 16 bits (IPL). */
void psh(uint8_t m)
{
    if (m & 0x01) { if (mflag) push8((uint8_t)A); else push16(A); }
    if (m & 0x02) { if (mflag) push8((uint8_t)B); else push16(B); }
    if (m & 0x04) { if (xflag) push8((uint8_t)X); else push16(X); }
    if (m & 0x08) { if (xflag) push8((uint8_t)Y); else push16(Y); }
    if (m & 0x10) push16(D);
    if (m & 0x20) push8(DT);
    if (m & 0x40) push8(PG);
    if (m & 0x80) push16(getP());
}
void pul(uint8_t m)
{
    if (m & 0x80) setP(pull16());
    if (m & 0x40) PG = pull8();
    if (m & 0x20) DT = pull8();
    if (m & 0x10) D = pull16();
    if (m & 0x08) { if (xflag) Y = pull8(); else Y = pull16(); }
    if (m & 0x04) { if (xflag) X = pull8(); else X = pull16(); }
    if (m & 0x02) { if (mflag) SETLO(B, pull8()); else B = pull16(); }
    if (m & 0x01) { if (mflag) SETLO(A, pull8()); else A = pull16(); }
}

static void nz8(uint8_t r)   { Z = r == 0; N = r >> 7; }
static void nz16(uint16_t r) { Z = r == 0; N = r >> 15; }

uint8_t adc8(uint16_t r, uint16_t v)
{ unsigned a = r & 0xFF, b = v & 0xFF, s = a + b + C; V = (~(a ^ b) & (a ^ s) & 0x80) != 0; C = s > 0xFF; nz8((uint8_t)s); return (uint8_t)s; }
uint16_t adc16(uint16_t r, uint16_t v)
{ unsigned long s = (unsigned long)r + v + C; V = (~(r ^ v) & (r ^ s) & 0x8000) != 0; C = s > 0xFFFF; nz16((uint16_t)s); return (uint16_t)s; }
uint8_t sbc8(uint16_t r, uint16_t v)
{ int a = r & 0xFF, b = v & 0xFF, s = a - b - (1 - C); V = ((a ^ b) & (a ^ s) & 0x80) != 0; C = s >= 0; nz8((uint8_t)s); return (uint8_t)s; }
uint16_t sbc16(uint16_t r, uint16_t v)
{ long s = (long)r - v - (1 - C); V = ((r ^ v) & (r ^ s) & 0x8000) != 0; C = s >= 0; nz16((uint16_t)s); return (uint16_t)s; }
uint8_t  asl8(uint16_t r)  { C = (r >> 7) & 1; nz8((uint8_t)(r << 1)); return (uint8_t)(r << 1); }
uint16_t asl16(uint16_t r) { C = r >> 15; nz16((uint16_t)(r << 1)); return (uint16_t)(r << 1); }
uint8_t  lsr8(uint16_t r)  { C = r & 1; nz8((uint8_t)((r & 0xFF) >> 1)); return (uint8_t)((r & 0xFF) >> 1); }
uint16_t lsr16(uint16_t r) { C = r & 1; nz16((uint16_t)(r >> 1)); return (uint16_t)(r >> 1); }
uint8_t  rol8(uint16_t r)  { uint8_t o = (uint8_t)(r << 1 | C); C = (r >> 7) & 1; nz8(o); return o; }
uint16_t rol16(uint16_t r) { uint16_t o = (uint16_t)(r << 1 | C); C = r >> 15; nz16(o); return o; }
uint8_t  ror8(uint16_t r)  { uint8_t o = (uint8_t)((r & 0xFF) >> 1 | C << 7); C = r & 1; nz8(o); return o; }
uint16_t ror16(uint16_t r) { uint16_t o = (uint16_t)(r >> 1 | C << 15); C = r & 1; nz16(o); return o; }
uint16_t rla8(uint16_t r, uint16_t n)  { uint8_t v = (uint8_t)r; while (n--) v = (uint8_t)(v << 1 | v >> 7); return (uint16_t)((r & 0xFF00) | v); }
uint16_t rla16(uint16_t r, uint16_t n) { while (n--) r = (uint16_t)(r << 1 | r >> 15); return r; }
/* MPY: A x M -> B:A ; DIV: B:A / M -> quotient A, remainder B (MELPS 7700 manual) */
void mpy8(uint16_t v)  { unsigned p = (A & 0xFF) * (v & 0xFF); SETLO(A, p); SETLO(B, p >> 8); }
void mpy16(uint16_t v) { unsigned long p = (unsigned long)A * v; A = (uint16_t)p; B = (uint16_t)(p >> 16); }
void div8(uint16_t v)  { unsigned n = (B & 0xFF) << 8 | (A & 0xFF); v &= 0xFF; if (!v) { cpu_brk(); return; } SETLO(A, n / v); SETLO(B, n % v); }
void div16(uint16_t v) { unsigned long n = (unsigned long)B << 16 | A; if (!v) { cpu_brk(); return; } A = (uint16_t)(n / v); B = (uint16_t)(n % v); }
void mvn(uint8_t dst, uint8_t src) { do { wr8((uint32_t)dst << 16 | Y, rd8((uint32_t)src << 16 | X)); X++; Y++; } while (A-- != 0); DT = dst; }
void mvp(uint8_t dst, uint8_t src) { do { wr8((uint32_t)dst << 16 | Y, rd8((uint32_t)src << 16 | X)); X--; Y--; } while (A-- != 0); DT = dst; }

__attribute__((weak)) void cpu_wait(void) {}
__attribute__((weak)) void cpu_stop(void) {}
__attribute__((weak)) void cpu_brk(void) {}
void ram_trampoline_087A(void) { if (ram[0x87B] == 0xDB) cpu_stop(); else cpu_wait(); }
__attribute__((weak)) void indirect_jump(uint16_t p) { (void)p; }
__attribute__((weak)) void bad_dispatch(uint32_t at) { (void)at; }
__attribute__((weak)) void unknown_switch(uint32_t at) { (void)at; }
