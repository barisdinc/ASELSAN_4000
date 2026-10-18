/* Behavioural test of lifted NC77 runtime routines against host C arithmetic.
 * Convention (from disassembly): 32-bit left operand in B:A, right operand at
 * DP(X) (4 bytes), result in B:A. */
#include <stdio.h>
#include <stdlib.h>
#include "../m7700.h"
#include "../functions.h"

static uint32_t rnd(void) { return (uint32_t)rand() << 17 ^ (uint32_t)rand() << 2 ^ (uint32_t)rand(); }

static uint32_t run(void (*f)(void), uint32_t l, uint32_t r)
{
    S = 0x0800; D = 0x0100; X = 0x0010; Y = 0x1234; DT = 0; mflag = 0; xflag = 0;
    wr16(0x110, (uint16_t)r); wr16(0x112, (uint16_t)(r >> 16));
    A = (uint16_t)l; B = (uint16_t)(l >> 16);
    S -= 3; f();
    if (S != 0x0800) printf("  stack imbalance S=%04X\n", S);
    if (D != 0x0100) printf("  D not preserved D=%04X\n", D);  /* X is scratch in NC77 */
    return (uint32_t)B << 16 | A;
}

int main(void)
{
    int fail[4] = {0}, n = 20000;
    srand(7700);
    for (int k = 0; k < n; k++) {
        uint32_t a = rnd(), b = rnd();
        if (k % 3 == 0) b &= 0xFFFF;            /* exercise the fast 16-bit divisor path */
        if (k % 5 == 0) a &= 0xFFFFF;
        if (b == 0) b = 1;
        if (run(rt_lmul_01E3EA, a, b) != a * b) fail[0]++;
        if (run(rt_ludiv_01E414, a, b) != a / b) fail[1]++;
        if (run(rt_lumod_01E43E, a, b) != a % b) fail[2]++;
        int32_t sa = (int32_t)a, sb = (int32_t)b;
        if (!(sa == INT32_MIN && sb == -1) && run(rt_ldiv_01E1A6, a, b) != (uint32_t)(sa / sb)) fail[3]++;
    }
    printf("lmul  fail %d/%d\nludiv fail %d/%d\nlumod fail %d/%d\nldiv  fail %d/%d\n",
           fail[0], n, fail[1], n, fail[2], n, fail[3], n);
    return fail[0] || fail[1] || fail[2] || fail[3];
}
