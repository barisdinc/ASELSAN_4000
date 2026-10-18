/* bank1_a.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 01058E  callers: 0097CD, 0097E7, 0098FF, 00990F, 009916, 009D02, 009D09, 009D60 ... */
void F_01058E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01058E LDA */
    push16(D);  /* 010590 PHD */
    D = A;  /* 010591 TAD */
    wr16(DP(0x00), Y);  /* 010592 STY */
    A = Y;  /* 010594 TYA */
    mflag = 1;  /* 010595 SEM */
    wr8(ABS(U0TB), (uint8_t)A);  /* 010596 STA */
L_010599: TICK();
    wr8(ABS(WDT), 0x00);  /* 010599 LDM */
L_01059D: TICK();
    if ((rd8(ABS(U0C1)) & 0x02) == 0x02) goto L_0105A4;  /* 01059D BBS */
    goto L_01059D;  /* 0105A2 BRA */
L_0105A4: TICK();
    mflag = 1;  /* 0105A4 SEM */
    if ((rd8(ABS(U0C0)) & 0x08) == 0) goto L_010599;  /* 0105A5 BBC */
    mflag = 0;  /* 0105AA CLM */
    D = pull16();  /* 0105AB PLD */
    S += 3; return;  /* 0105AC RTL */
}

/* 0105C9  callers: 01A25C */
void F_0105C9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0105C9 LDA */
    push16(D);  /* 0105CB PHD */
    D = A;  /* 0105CC TAD */
    C = 0;  /* 0105CD CLC */
    A = adc16(A, 0x000B);  /* 0105CE ADC */
    push16(A);  /* 0105D1 PHA */
    A = rd16(ABS(0x010F));  /* 0105D2 LDA */
    mflag = 1;  /* 0105D5 SEM */
    wr8(DP(0x09), (uint8_t)A);  /* 0105D6 STA */
    SETLO(A, rd8(ABS(0x0110)));  /* 0105D8 LDA */
    wr8(DP(0x0A), (uint8_t)A);  /* 0105DB STA */
    mflag = 0;  /* 0105DD CLM */
    S -= 3; F_00968D();  /* 0105DE JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0400);  /* 0105E2 SEB */
    Y = 0x0024;  /* 0105E7 LDY */
    S -= 3; F_003310();  /* 0105EA JSRL */
    S -= 3; F_00828C();  /* 0105EE JSRL */
    I = 1;  /* 0105F2 SEI */
    wr16(DP(0x00), 0x0100);  /* 0105F3 LDM */
    mflag = 1;  /* 0105F7 SEM */
    wr8(DP(0x02), 0x01);  /* 0105F8 LDM */
    mflag = 0;  /* 0105FB CLM */
    wr16(DP(0x03), 0x0080);  /* 0105FC LDM */
    mflag = 1;  /* 010600 SEM */
    wr8(DP(0x05), 0x00);  /* 010601 LDM */
    mflag = 0;  /* 010604 CLM */
    wr16(DP(0x06), 0x05C8);  /* 010605 LDM */
    mflag = 1;  /* 010609 SEM */
    wr8(DP(0x08), 0x01);  /* 01060A LDM */
L_01060D: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01060D LDA */
    wr8(LIND(0x03), (uint8_t)A);  /* 01060F STA */
    mflag = 0;  /* 010611 CLM */
    t = (rd16(DP(0x03)) + 1) & 0xFFFF; wr16(DP(0x03), t);  /* 010612 INC */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 010614 INC */
    X = rd16(DP(0x00));  /* 010616 LDX */
    t = rd16(DP(0x06));  /* 010618 CPX */
    C = (X >= t);
    mflag = 1;  /* 01061A SEM */
    if (!C) goto L_01060D;  /* 01061B BCC */
    SETLO(A, rd8(DP(0x0A)));  /* 01061D LDA */
    wr8(DP(0x0B), (uint8_t)A);  /* 01061F STA */
    Y = rd16(DP(0x09));  /* 010621 LDY */
    mflag = 0;  /* 010623 CLM */
    S -= 3; ramcode_flash_loader();  /* 010624 JSRL */
    Y = pull16();  /* 010628 PLY */
    D = pull16();  /* 010629 PLD */
    S += 3; return;  /* 01062A RTL */
}

/* 010630  callers: 00D8EA */
void set_options_010630(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 010630 SEM */
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0xF0);  /* 010631 CLB */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x80);  /* 010635 CLB */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) & ~0x10);  /* 010639 CLB */
    mflag = 0;  /* 01063D CLM */
    S += 3; return;  /* 01063E RTL */
}

/* 013287  callers: 013515 */
void F_013287(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 013287 LDA */
    push16(D);  /* 013289 PHD */
    D = A;  /* 01328A TAD */
    push16(A);  /* 01328B PHA */
    S -= 3; F_007F9B();  /* 01328C JSRL */
    /* 013290 CMP */
    if ((A & 0xFF) != 0x00) goto L_0132B2;  /* 013292 BNE */
    wr8(ABS(0x0118), (uint8_t)A);  /* 013294 STA */
    mflag = 0;  /* 013297 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0004);  /* 013298 SEB */
    Y = 0x0004;  /* 01329D LDY */
    S -= 3; F_003310();  /* 0132A0 JSRL */
    S -= 3; F_009782();  /* 0132A4 JSRL */
    Y = 0x0BB8;  /* 0132A8 LDY */
    S -= 3; F_007D7F();  /* 0132AB JSRL */
    goto L_013336;  /* 0132AF JMP */
L_0132B2: TICK();
    mflag = 0;  /* 0132B2 CLM */
    S -= 3; F_0133F6();  /* 0132B3 JSRL */
    X = A;  /* 0132B7 TAX */
    if (X != 0) goto L_0132DB;  /* 0132B8 BNE */
    mflag = 1;  /* 0132BA SEM */
    wr8(ABS(0x0118), (uint8_t)A);  /* 0132BB STA */
    Y = 0x0003;  /* 0132BE LDY */
    mflag = 0;  /* 0132C1 CLM */
    S -= 3; F_003310();  /* 0132C2 JSRL */
    Y = 0x0001;  /* 0132C6 LDY */
    S -= 3; F_0080AA();  /* 0132C9 JSRL */
    S -= 3; F_00828C();  /* 0132CD JSRL */
    Y = 0x0BB8;  /* 0132D1 LDY */
    S -= 3; F_007D7F();  /* 0132D4 JSRL */
    goto L_013336;  /* 0132D8 JMP */
L_0132DB: TICK();
    mflag = 0;  /* 0132DB CLM */
    S -= 3; F_01333A();  /* 0132DC JSRL */
    mflag = 1;  /* 0132E0 SEM */
    wr8(ABS(0x01A2), 0xC4);  /* 0132E1 LDM */
    wr8(ABS(0x01A3), 0xD7);  /* 0132E5 LDM */
    wr8(ABS(0x0118), 0x03);  /* 0132E9 LDM */
    mflag = 0;  /* 0132ED CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 0132EE CLB */
    mflag = 1;  /* 0132F3 SEM */
    wr8(ABS(0x04E6), 0x00);  /* 0132F4 LDM */
    wr8(ABS(0x019D), 0x00);  /* 0132F8 LDM */
    mflag = 0;  /* 0132FC CLM */
    wr16(DP(0x00), 0x00D7);  /* 0132FD LDM */
    wr16(DP(0x02), 0x00C4);  /* 013301 LDM */
    wr16(DP(0x04), 0x0044);  /* 013305 LDM */
    Y = 0x0003;  /* 013309 LDY */
    A = D;  /* 01330C TDA */
    C = 0;  /* 01330D CLC */
    A = adc16(A, 0x0006);  /* 01330E ADC */
    push16(A);  /* 013311 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 013312 JSRL */
    Y = pull16();  /* 013316 PLY */
    mflag = 0;  /* 013317 CLM */
    wr16(DP(0x00), 0x001A);  /* 013318 LDM */
    wr16(DP(0x02), 0x0040);  /* 01331C LDM */
    Y = 0x0002;  /* 013320 LDY */
    A = D;  /* 013323 TDA */
    C = 0;  /* 013324 CLC */
    A = adc16(A, 0x0004);  /* 013325 ADC */
    push16(A);  /* 013328 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 013329 JSRL */
    Y = pull16();  /* 01332D PLY */
    wr8(ABS(0x01AA), 0x00);  /* 01332E LDM */
    wr8(ABS(0x019C), 0x00);  /* 013332 LDM */
L_013336: TICK();
    mflag = 0;  /* 013336 CLM */
    Y = pull16();  /* 013337 PLY */
    D = pull16();  /* 013338 PLD */
    S += 3; return;  /* 013339 RTL */
}

/* 01333A  callers: 0132DC */
void F_01333A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01333A LDA */
    push16(D);  /* 01333C PHD */
    D = A;  /* 01333D TAD */
    push16(A);  /* 01333E PHA */
    wr16(DP(0x00), 0x0041);  /* 01333F LDM */
    Y = rd16(ABS(0x0195));  /* 013343 LDY */
    S -= 3; F_013363();  /* 013346 JSRL */
    wr16(DP(0x00), 0x0054);  /* 01334A LDM */
    Y = rd16(ABS(0x0197));  /* 01334E LDY */
    S -= 3; F_013363();  /* 013351 JSRL */
    Y = 0x002C;  /* 013355 LDY */
    S -= 3; F_003310();  /* 013358 JSRL */
    S -= 3; F_00828C();  /* 01335C JSRL */
    Y = pull16();  /* 013360 PLY */
    D = pull16();  /* 013361 PLD */
    S += 3; return;  /* 013362 RTL */
}

/* 013363  callers: 013346, 013351 */
void F_013363(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 013363 LDA */
    push16(D);  /* 013365 PHD */
    D = A;  /* 013366 TAD */
    A += 1;  /* 013367 INC */
    A += 1;  /* 013368 INC */
    A += 1;  /* 013369 INC */
    push16(A);  /* 01336A PHA */
    A = rd16(DP(0x00));  /* 01336B LDA */
    mflag = 1;  /* 01336D SEM */
    wr8(ABS(0x0252), (uint8_t)A);  /* 01336E STA */
    wr8(ABS(0x0253), 0x4E);  /* 013371 LDM */
    wr8(ABS(0x0254), 0x4F);  /* 013375 LDM */
    wr8(ABS(0x0255), 0x3A);  /* 013379 LDM */
    mflag = 0;  /* 01337D CLM */
    B = 0x0000;  /* 01337E LDA */
    A = Y;  /* 013382 TYA */
    div16(0x03E8);  /* 013383 DIV */
    C = 0;  /* 013387 CLC */
    mflag = 1;  /* 013388 SEM */
    SETLO(A, adc8(A, 0x30));  /* 013389 ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 01338B STA */
    mflag = 0;  /* 01338E CLM */
    B = 0x0000;  /* 01338F LDA */
    A = Y;  /* 013393 TYA */
    div16(0x0064);  /* 013394 DIV */
    B = 0x0000;  /* 013398 LDA */
    div16(0x000A);  /* 01339C DIV */
    push16(B);  /* 0133A0 PHB */
    A = pull16();  /* 0133A2 PLA */
    C = 0;  /* 0133A3 CLC */
    mflag = 1;  /* 0133A4 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0133A5 ADC */
    wr8(ABS(0x0257), (uint8_t)A);  /* 0133A7 STA */
    mflag = 0;  /* 0133AA CLM */
    B = 0x0000;  /* 0133AB LDA */
    A = Y;  /* 0133AF TYA */
    div16(0x000A);  /* 0133B0 DIV */
    B = 0x0000;  /* 0133B4 LDA */
    div16(0x000A);  /* 0133B8 DIV */
    push16(B);  /* 0133BC PHB */
    A = pull16();  /* 0133BE PLA */
    C = 0;  /* 0133BF CLC */
    mflag = 1;  /* 0133C0 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0133C1 ADC */
    wr8(ABS(0x0258), (uint8_t)A);  /* 0133C3 STA */
    mflag = 0;  /* 0133C6 CLM */
    B = 0x0000;  /* 0133C7 LDA */
    A = Y;  /* 0133CB TYA */
    wr16(DP(0x01), Y);  /* 0133CC STY */
    div16(0x000A);  /* 0133CE DIV */
    push16(B);  /* 0133D2 PHB */
    A = pull16();  /* 0133D4 PLA */
    C = 0;  /* 0133D5 CLC */
    mflag = 1;  /* 0133D6 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0133D7 ADC */
    wr8(ABS(0x0259), (uint8_t)A);  /* 0133D9 STA */
    mflag = 0;  /* 0133DC CLM */
    S -= 3; F_00828C();  /* 0133DD JSRL */
    mflag = 1;  /* 0133E1 SEM */
    wr8(ABS(0x0233), 0x10);  /* 0133E2 LDM */
L_0133E6: TICK();
    SETLO(A, rd8(ABS(0x0233)));  /* 0133E6 LDA */
    if ((A & 0xFF) == 0) goto L_0133F2;  /* 0133E9 BEQ */
    wr8(ABS(WDT), 0x00);  /* 0133EB LDM */
    goto L_0133E6;  /* 0133EF JMP */
L_0133F2: TICK();
    mflag = 0;  /* 0133F2 CLM */
    Y = pull16();  /* 0133F3 PLY */
    D = pull16();  /* 0133F4 PLD */
    S += 3; return;  /* 0133F5 RTL */
}

/* 0133F6  callers: 0132B3 */
void F_0133F6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0133F6 LDA */
    push16(D);  /* 0133F8 PHD */
    D = A;  /* 0133F9 TAD */
    C = 0;  /* 0133FA CLC */
    A = adc16(A, 0x0024);  /* 0133FB ADC */
    push16(A);  /* 0133FE PHA */
    wr16(DP(0x1E), 0x0000);  /* 0133FF LDM */
    wr16(DP(0x20), 0x0000);  /* 013403 LDM */
L_013407: TICK();
    X = rd16(DP(0x20));  /* 013407 LDX */
    C = (X >= 0x00D2);  /* 013409 CPX */
    if (C) goto L_01344F;  /* 01340C BCS */
    wr16(DP(0x24), 0x001E);  /* 01340E LDM */
    A = D;  /* 013412 TDA */
    B = 0x0000;  /* 013413 LDA */
    wr16(DP(0x25), A);  /* 013417 STA */
    wr16(DP(0x27), B);  /* 013419 STA */
    A = X;  /* 01341C TXA */
    A = adc16(A, 0x0600);  /* 01341D ADC */
    wr16(DP(0x28), A);  /* 013420 STA */
    Y = 0x00A0;  /* 013422 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 013425 JSRL */
    wr16(DP(0x22), 0x0000);  /* 013429 LDM */
L_01342D: TICK();
    X = rd16(DP(0x22));  /* 01342D LDX */
    C = (X >= 0x001E);  /* 01342F CPX */
    if (C) goto L_013443;  /* 013432 BCS */
    A = rd16(DP(0x00 + X));  /* 013434 LDA */
    A &= 0x00FF;  /* 013436 AND */
    A = adc16(A, rd16(DP(0x1E)));  /* 013439 ADC */
    wr16(DP(0x1E), A);  /* 01343B STA */
    X += 1;  /* 01343D INX */
    wr16(DP(0x22), X);  /* 01343E STX */
    goto L_01342D;  /* 013440 JMP */
L_013443: TICK();
    mflag = 0;  /* 013443 CLM */
    A = rd16(DP(0x20));  /* 013444 LDA */
    C = 0;  /* 013446 CLC */
    A = adc16(A, 0x001E);  /* 013447 ADC */
    wr16(DP(0x20), A);  /* 01344A STA */
    goto L_013407;  /* 01344C JMP */
L_01344F: TICK();
    Y = 0x06D2;  /* 01344F LDY */
    mflag = 0;  /* 013452 CLM */
    S -= 3; eeprom_read_word_0071F0();  /* 013453 JSRL */
    t = rd16(DP(0x1E));  /* 013457 CMP */
    if (A == t) goto L_013462;  /* 013459 BEQ */
    wr16(DP(0x22), 0x0000);  /* 01345B LDM */
    goto L_0134BA;  /* 01345F JMP */
L_013462: TICK();
    mflag = 0;  /* 013462 CLM */
    wr16(DP(0x22), 0x0001);  /* 013463 LDM */
    Y = 0x0600;  /* 013467 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 01346A JSRL */
    wr8(ABS(0x0199), (uint8_t)A);  /* 01346E STA */
    Y = 0x0601;  /* 013471 LDY */
    mflag = 0;  /* 013474 CLM */
    S -= 3; eeprom_read_word_0071F0();  /* 013475 JSRL */
    wr16(ABS(0x0195), A);  /* 013479 STA */
    Y = 0x0603;  /* 01347C LDY */
    S -= 3; eeprom_read_word_0071F0();  /* 01347F JSRL */
    wr16(ABS(0x0197), A);  /* 013483 STA */
    Y = 0x0605;  /* 013486 LDY */
    S -= 3; eeprom_read_word_0071F0();  /* 013489 JSRL */
    wr16(ABS(0x017F), A);  /* 01348D STA */
    wr16(DP(0x20), 0x0000);  /* 013490 LDM */
L_013494: TICK();
    X = rd16(DP(0x20));  /* 013494 LDX */
    /* 013496 CPX */
    if (X >= 0x0005) goto L_0134BA;  /* 013499 BCS */
    A = X;  /* 01349B TXA */
    A = asl16(A);  /* 01349C ASL */
    C = 0;  /* 01349D CLC */
    A = adc16(A, 0x0608);  /* 01349E ADC */
    Y = A;  /* 0134A1 TAY */
    S -= 3; eeprom_read_word_0071F0();  /* 0134A2 JSRL */
    wr16(DP(0x24), A);  /* 0134A6 STA */
    A = rd16(DP(0x20));  /* 0134A8 LDA */
    A = asl16(A);  /* 0134AA ASL */
    C = 0;  /* 0134AB CLC */
    A = adc16(A, 0x018B);  /* 0134AC ADC */
    X = A;  /* 0134AF TAX */
    A = rd16(DP(0x24));  /* 0134B0 LDA */
    wr16((ABS(0x0000) + X), A);  /* 0134B2 STA */
    t = (rd16(DP(0x20)) + 1) & 0xFFFF; wr16(DP(0x20), t);  /* 0134B5 INC */
    goto L_013494;  /* 0134B7 JMP */
L_0134BA: TICK();
    mflag = 0;  /* 0134BA CLM */
    A = rd16(DP(0x22));  /* 0134BB LDA */
    Y = pull16();  /* 0134BD PLY */
    D = pull16();  /* 0134BE PLD */
    S += 3; return;  /* 0134BF RTL */
}

/* 0134C0  callers: 01A7C9 */
void F_0134C0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0134C0 LDA */
    push16(D);  /* 0134C2 PHD */
    D = A;  /* 0134C3 TAD */
    C = 0;  /* 0134C4 CLC */
    A = adc16(A, 0x0006);  /* 0134C5 ADC */
    push16(A);  /* 0134C8 PHA */
    mflag = 1;  /* 0134C9 SEM */
    SETLO(A, rd8(ABS(0x0118)));  /* 0134CA LDA */
    if ((A & 0xFF) != 0) goto L_0134D2;  /* 0134CD BNE */
    goto L_013FD5;  /* 0134CF JMP */
L_0134D2: TICK();
    /* 0134D2 CMP */
    if ((A & 0xFF) != 0x02) goto L_013509;  /* 0134D4 BNE */
    SETLO(A, rd8(ABS(0x019D)));  /* 0134D6 LDA */
    if ((A & 0xFF) == 0) goto L_013509;  /* 0134D9 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 0134DB LDA */
    /* 0134DE CMP */
    if ((A & 0xFF) != 0x03) goto L_0134F1;  /* 0134E0 BNE */
    wr8(ABS(0x0293), 0x00);  /* 0134E2 LDM */
    wr8(ABS(0x0230), 0x00);  /* 0134E6 LDM */
    wr8(ABS(0x0118), 0x03);  /* 0134EA LDM */
    goto L_013509;  /* 0134EE JMP */
L_0134F1: TICK();
    mflag = 0;  /* 0134F1 CLM */
    wr16(DP(0x02), 0x001A);  /* 0134F2 LDM */
    wr16(DP(0x04), 0x0040);  /* 0134F6 LDM */
    Y = 0x0002;  /* 0134FA LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0134FD JSRL */
    wr8(ABS(0x01AA), 0x00);  /* 013501 LDM */
    wr8(ABS(0x019D), 0x00);  /* 013505 LDM */
L_013509: TICK();
    SETLO(A, rd8(ABS(0x0118)));  /* 013509 LDA */
    /* 01350C CMP */
    if ((A & 0xFF) != 0x01) goto L_01351D;  /* 01350E BNE */
    mflag = 0;  /* 013510 CLM */
    A = D;  /* 013511 TDA */
    A += 1;  /* 013512 INC */
    A += 1;  /* 013513 INC */
    push16(A);  /* 013514 PHA */
    S -= 3; F_013287();  /* 013515 JSRL */
    Y = pull16();  /* 013519 PLY */
    goto L_013FD5;  /* 01351A JMP */
L_01351D: TICK();
    mflag = 0;  /* 01351D CLM */
    A = rd16(ABS(0x0118));  /* 01351E LDA */
    mflag = 1;  /* 013521 SEM */
    /* 013522 CMP */
    if ((A & 0xFF) != 0x03) goto L_013578;  /* 013524 BNE */
    SETLO(A, rd8(ABS(0x019D)));  /* 013526 LDA */
    if ((A & 0xFF) == 0) goto L_013575;  /* 013529 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 01352B LDA */
    /* 01352E CMP */
    if ((A & 0xFF) != 0x03) goto L_01354C;  /* 013530 BNE */
    wr8(ABS(0x0118), 0x09);  /* 013532 LDM */
    wr8(ABS(0x0229), 0x01);  /* 013536 LDM */
    mflag = 0;  /* 01353A CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x1000);  /* 01353B SEB */
    A = D;  /* 013540 TDA */
    A += 1;  /* 013541 INC */
    A += 1;  /* 013542 INC */
    push16(A);  /* 013543 PHA */
    S -= 3; F_00CB52();  /* 013544 JSRL */
    Y = pull16();  /* 013548 PLY */
    goto L_01355C;  /* 013549 JMP */
L_01354C: TICK();
    mflag = 0;  /* 01354C CLM */
    wr16(DP(0x02), 0x001A);  /* 01354D LDM */
    wr16(DP(0x04), 0x0040);  /* 013551 LDM */
    Y = 0x0002;  /* 013555 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 013558 JSRL */
L_01355C: TICK();
    mflag = 1;  /* 01355C SEM */
    wr8(ABS(0x01AA), 0x00);  /* 01355D LDM */
    wr8(ABS(0x019C), 0x00);  /* 013561 LDM */
    mflag = 0;  /* 013565 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0400);  /* 013566 CLB */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0020);  /* 01356B CLB */
    mflag = 1;  /* 013570 SEM */
    wr8(ABS(0x019D), 0x00);  /* 013571 LDM */
L_013575: TICK();
    goto L_013FD5;  /* 013575 JMP */
L_013578: TICK();
    mflag = 0;  /* 013578 CLM */
    A = rd16(ABS(0x0118));  /* 013579 LDA */
    mflag = 1;  /* 01357C SEM */
    /* 01357D CMP */
    if ((A & 0xFF) != 0x04) goto L_0135B6;  /* 01357F BNE */
    wr8(ABS(0x0118), 0x05);  /* 013581 LDM */
    wr8(ABS(0x019A), 0x04);  /* 013585 LDM */
    SETLO(B, 0x00);  /* 013589 LDA */
    SETLO(A, rd8(ABS(0x0457)));  /* 01358C LDA */
    div8(0x04);  /* 01358F DIV */
    mflag = 0;  /* 013592 CLM */
    push16(B);  /* 013593 PHB */
    A = pull16();  /* 013595 PLA */
    A &= 0x00FF;  /* 013596 AND */
    A += 1;  /* 013599 INC */
    mpy16(0x0032);  /* 01359A MPY */
    C = 0;  /* 01359E CLC */
    A = adc16(A, 0x07D0);  /* 01359F ADC */
    B = 0x0020;  /* 0135A2 LDA */
    S -= 3; F_00FF8C();  /* 0135A6 JSRL */
    mflag = 1;  /* 0135AA SEM */
    SETLO(A, A + 1);  /* 0135AB INC */
    wr8(ABS(0x03A0), (uint8_t)A);  /* 0135AC STA */
    wr8(ABS(0x041E), 0x1E);  /* 0135AF LDM */
    goto L_013FD5;  /* 0135B3 JMP */
L_0135B6: TICK();
    mflag = 0;  /* 0135B6 CLM */
    A = rd16(ABS(0x0118));  /* 0135B7 LDA */
    mflag = 1;  /* 0135BA SEM */
    /* 0135BB CMP */
    if ((A & 0xFF) == 0x05) goto L_0135C2;  /* 0135BD BEQ */
    goto L_013659;  /* 0135BF JMP */
L_0135C2: TICK();
    SETLO(A, rd8(ABS(0x041E)));  /* 0135C2 LDA */
    if ((A & 0xFF) != 0) goto L_01360A;  /* 0135C5 BNE */
    wr8(DP(0x02), (uint8_t)A);  /* 0135C7 STA */
    SETLO(A, rd8(ABS(0x011D)));  /* 0135C9 LDA */
    SETLO(A, A & 0x0F);  /* 0135CC AND */
    mflag = 0;  /* 0135CE CLM */
    A &= 0x00FF;  /* 0135CF AND */
    A = rla16(A, 0x0008);  /* 0135D2 RLA */
    A &= 0xFF00;  /* 0135D6 AND */
    wr16(DP(0x03), A);  /* 0135D9 STA */
    A = rd16(ABS(0x011E));  /* 0135DB LDA */
    A &= 0x00FF;  /* 0135DE AND */
    C = 0;  /* 0135E1 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 0135E2 ADC */
    wr16(DP(0x03), A);  /* 0135E4 STA */
    A = rd16(ABS(0x011B));  /* 0135E6 LDA */
    mflag = 1;  /* 0135E9 SEM */
    SETLO(A, A & 0x7F);  /* 0135EA AND */
    wr8(DP(0x05), (uint8_t)A);  /* 0135EC STA */
    wr8(DP(0x06), 0x0B);  /* 0135EE LDM */
    Y = 0x00E0;  /* 0135F1 LDY */
    mflag = 0;  /* 0135F4 CLM */
    A = D;  /* 0135F5 TDA */
    A += 1;  /* 0135F6 INC */
    A += 1;  /* 0135F7 INC */
    push16(A);  /* 0135F8 PHA */
    S -= 3; F_01501B();  /* 0135F9 JSRL */
    Y = pull16();  /* 0135FD PLY */
    A = D;  /* 0135FE TDA */
    A += 1;  /* 0135FF INC */
    A += 1;  /* 013600 INC */
    push16(A);  /* 013601 PHA */
    S -= 3; F_014031();  /* 013602 JSRL */
    Y = pull16();  /* 013606 PLY */
    goto L_013656;  /* 013607 JMP */
L_01360A: TICK();
    mflag = 1;  /* 01360A SEM */
    SETLO(A, rd8(ABS(0x03A0)));  /* 01360B LDA */
    if ((A & 0xFF) != 0) goto L_013626;  /* 01360E BNE */
    wr8(ABS(0x041E), 0x0F);  /* 013610 LDM */
    wr8(ABS(0x0118), 0x06);  /* 013614 LDM */
    mflag = 0;  /* 013618 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 013619 SEB */
    mflag = 1;  /* 01361E SEM */
    wr8(ABS(0x04E6), 0x01);  /* 01361F LDM */
    goto L_013656;  /* 013623 JMP */
L_013626: TICK();
    mflag = 0;  /* 013626 CLM */
    A = rd16(ABS(0x0228));  /* 013627 LDA */
    mflag = 1;  /* 01362A SEM */
    /* 01362B CMP */
    if ((A & 0xFF) == 0x02) goto L_013656;  /* 01362D BEQ */
    SETLO(B, 0x00);  /* 01362F LDA */
    SETLO(A, rd8(ABS(0x0457)));  /* 013632 LDA */
    div8(rd8(ABS(0x019A)));  /* 013635 DIV */
    mflag = 0;  /* 013639 CLM */
    push16(B);  /* 01363A PHB */
    A = pull16();  /* 01363C PLA */
    A &= 0x00FF;  /* 01363D AND */
    A += 1;  /* 013640 INC */
    mpy16(0x0032);  /* 013641 MPY */
    C = 0;  /* 013645 CLC */
    A = adc16(A, 0x07D0);  /* 013646 ADC */
    B = 0x0020;  /* 013649 LDA */
    S -= 3; F_00FF8C();  /* 01364D JSRL */
    mflag = 1;  /* 013651 SEM */
    SETLO(A, A + 1);  /* 013652 INC */
    wr8(ABS(0x03A0), (uint8_t)A);  /* 013653 STA */
L_013656: TICK();
    goto L_013FD5;  /* 013656 JMP */
L_013659: TICK();
    mflag = 0;  /* 013659 CLM */
    A = rd16(ABS(0x0118));  /* 01365A LDA */
    mflag = 1;  /* 01365D SEM */
    /* 01365E CMP */
    if ((A & 0xFF) == 0x06) goto L_013665;  /* 013660 BEQ */
    goto L_01375D;  /* 013662 JMP */
L_013665: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 013665 LDA */
    /* 013668 CMP */
    if ((A & 0xFF) == 0x08) goto L_01366F;  /* 01366A BEQ */
    goto L_013714;  /* 01366C JMP */
L_01366F: TICK();
    SETLO(A, rd8(ABS(0x041E)));  /* 01366F LDA */
    C = ((A & 0xFF) >= 0x0A);  /* 013672 CMP */
    if (C) goto L_0136AD;  /* 013674 BCS */
    wr8(DP(0x02), 0x00);  /* 013676 LDM */
    SETLO(A, rd8(ABS(0x011D)));  /* 013679 LDA */
    SETLO(A, A & 0x0F);  /* 01367C AND */
    mflag = 0;  /* 01367E CLM */
    A &= 0x00FF;  /* 01367F AND */
    A = rla16(A, 0x0008);  /* 013682 RLA */
    A &= 0xFF00;  /* 013686 AND */
    wr16(DP(0x03), A);  /* 013689 STA */
    A = rd16(ABS(0x011E));  /* 01368B LDA */
    A &= 0x00FF;  /* 01368E AND */
    A = adc16(A, rd16(DP(0x03)));  /* 013691 ADC */
    wr16(DP(0x03), A);  /* 013693 STA */
    A = rd16(ABS(0x011B));  /* 013695 LDA */
    mflag = 1;  /* 013698 SEM */
    SETLO(A, A & 0x7F);  /* 013699 AND */
    wr8(DP(0x05), (uint8_t)A);  /* 01369B STA */
    wr8(DP(0x06), 0x02);  /* 01369D LDM */
    Y = 0x00D0;  /* 0136A0 LDY */
    mflag = 0;  /* 0136A3 CLM */
    A = D;  /* 0136A4 TDA */
    A += 1;  /* 0136A5 INC */
    A += 1;  /* 0136A6 INC */
    push16(A);  /* 0136A7 PHA */
    S -= 3; F_01501B();  /* 0136A8 JSRL */
    Y = pull16();  /* 0136AC PLY */
L_0136AD: TICK();
    mflag = 1;  /* 0136AD SEM */
    wr8(ABS(0x0118), 0x07);  /* 0136AE LDM */
    Y = 0x0607;  /* 0136B2 LDY */
    mflag = 0;  /* 0136B5 CLM */
    A = D;  /* 0136B6 TDA */
    A += 1;  /* 0136B7 INC */
    A += 1;  /* 0136B8 INC */
    push16(A);  /* 0136B9 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 0136BA JSRL */
    Y = pull16();  /* 0136BE PLY */
    wr8(ABS(0x019B), (uint8_t)A);  /* 0136BF STA */
    wr8(ABS(0x019D), 0x00);  /* 0136C2 LDM */
    wr8(ABS(0x01AA), 0x00);  /* 0136C6 LDM */
    mflag = 0;  /* 0136CA CLM */
    wr16(DP(0x02), 0x0009);  /* 0136CB LDM */
    wr16(DP(0x04), 0x0040);  /* 0136CF LDM */
    Y = 0x0002;  /* 0136D3 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0136D6 JSRL */
    mflag = 0;  /* 0136DA CLM */
    wr16(DP(0x02), 0x00AA);  /* 0136DB LDM */
    wr16(DP(0x04), 0x0043);  /* 0136DF LDM */
    Y = 0x0002;  /* 0136E3 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0136E6 JSRL */
    mflag = 0;  /* 0136EA CLM */
    wr16(ABS(0x019E), rd16(ABS(0x019E)) & ~0x0008);  /* 0136EB CLB */
    wr16(DP(0x02), 0x0006);  /* 0136F0 LDM */
    wr16(DP(0x03), 0x01A4);  /* 0136F4 LDM */
    wr16(DP(0x05), 0x0000);  /* 0136F8 LDM */
    B = 0x0000;  /* 0136FC LDA */
    Y = 0x0119;  /* 013700 LDY */
    A = D;  /* 013703 TDA */
    A += 1;  /* 013704 INC */
    A += 1;  /* 013705 INC */
    push16(A);  /* 013706 PHA */
    S -= 3; F_013FD9();  /* 013707 JSRL */
    Y = pull16();  /* 01370B PLY */
    mflag = 1;  /* 01370C SEM */
    wr8(ABS(0x019C), 0x06);  /* 01370D LDM */
    goto L_01375A;  /* 013711 JMP */
L_013714: TICK();
    mflag = 1;  /* 013714 SEM */
    SETLO(A, rd8(ABS(0x041E)));  /* 013715 LDA */
    if ((A & 0xFF) != 0) goto L_01375A;  /* 013718 BNE */
    wr8(DP(0x02), (uint8_t)A);  /* 01371A STA */
    SETLO(A, rd8(ABS(0x011D)));  /* 01371C LDA */
    SETLO(A, A & 0x0F);  /* 01371F AND */
    mflag = 0;  /* 013721 CLM */
    A &= 0x00FF;  /* 013722 AND */
    A = rla16(A, 0x0008);  /* 013725 RLA */
    A &= 0xFF00;  /* 013729 AND */
    wr16(DP(0x03), A);  /* 01372C STA */
    A = rd16(ABS(0x011E));  /* 01372E LDA */
    A &= 0x00FF;  /* 013731 AND */
    C = 0;  /* 013734 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 013735 ADC */
    wr16(DP(0x03), A);  /* 013737 STA */
    A = rd16(ABS(0x011B));  /* 013739 LDA */
    mflag = 1;  /* 01373C SEM */
    SETLO(A, A & 0x7F);  /* 01373D AND */
    wr8(DP(0x05), (uint8_t)A);  /* 01373F STA */
    wr8(DP(0x06), 0x08);  /* 013741 LDM */
    Y = 0x00E0;  /* 013744 LDY */
    mflag = 0;  /* 013747 CLM */
    A = D;  /* 013748 TDA */
    A += 1;  /* 013749 INC */
    A += 1;  /* 01374A INC */
    push16(A);  /* 01374B PHA */
    S -= 3; F_01501B();  /* 01374C JSRL */
    Y = pull16();  /* 013750 PLY */
    A = D;  /* 013751 TDA */
    A += 1;  /* 013752 INC */
    A += 1;  /* 013753 INC */
    push16(A);  /* 013754 PHA */
    S -= 3; F_014031();  /* 013755 JSRL */
    Y = pull16();  /* 013759 PLY */
L_01375A: TICK();
    goto L_013FD5;  /* 01375A JMP */
L_01375D: TICK();
    mflag = 0;  /* 01375D CLM */
    A = rd16(ABS(0x0118));  /* 01375E LDA */
    mflag = 1;  /* 013761 SEM */
    /* 013762 CMP */
    if ((A & 0xFF) == 0x07) goto L_013769;  /* 013764 BEQ */
    goto L_0138AE;  /* 013766 JMP */
L_013769: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 013769 LDA */
    /* 01376C CMP */
    if ((A & 0xFF) == 0x08) goto L_013773;  /* 01376E BEQ */
    goto L_013865;  /* 013770 JMP */
L_013773: TICK();
    SETLO(A, rd8(ABS(0x019D)));  /* 013773 LDA */
    if ((A & 0xFF) != 0) goto L_01377B;  /* 013776 BNE */
    goto L_013862;  /* 013778 JMP */
L_01377B: TICK();
    SETLO(A, rd8(ABS(0x019C)));  /* 01377B LDA */
    t = rd8(ABS(0x018A));  /* 01377E CMP */
    if ((A & 0xFF) >= t) goto L_013786;  /* 013781 BCS */
    goto L_013824;  /* 013783 JMP */
L_013786: TICK();
    mflag = 0;  /* 013786 CLM */
    if ((rd16(ABS(0x019E)) & 0x0008) == 0x0008) goto L_013790;  /* 013787 BBS */
    goto L_013821;  /* 01378D JMP */
L_013790: TICK();
    X = rd16(ABS(0x03D5));  /* 013790 LDX */
    /* 013793 CPX */
    if (X != 0x0FFF) goto L_0137EB;  /* 013796 BNE */
    wr16(DP(0x02), 0x0000);  /* 013798 LDM */
    mflag = 1;  /* 01379C SEM */
    SETLO(B, 0x00);  /* 01379D LDA */
    SETLO(A, rd8(ABS(0x011D)));  /* 0137A0 LDA */
    div8(0x10);  /* 0137A3 DIV */
    mflag = 0;  /* 0137A6 CLM */
    A &= 0x00FF;  /* 0137A7 AND */
    wr16(DP(0x03), A);  /* 0137AA STA */
    A = rd16(ABS(0x011C));  /* 0137AC LDA */
    A &= 0x00FF;  /* 0137AF AND */
    A = asl16(A);  /* 0137B2 ASL */
    A = asl16(A);  /* 0137B3 ASL */
    A = asl16(A);  /* 0137B4 ASL */
    A = asl16(A);  /* 0137B5 ASL */
    C = 0;  /* 0137B6 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 0137B7 ADC */
    wr16(DP(0x03), A);  /* 0137B9 STA */
    A = rd16(ABS(0x011B));  /* 0137BB LDA */
    mflag = 1;  /* 0137BE SEM */
    SETLO(A, A & 0x7F);  /* 0137BF AND */
    wr8(DP(0x05), (uint8_t)A);  /* 0137C1 STA */
    wr8(DP(0x06), 0x00);  /* 0137C3 LDM */
    Y = 0x00C0;  /* 0137C6 LDY */
    mflag = 0;  /* 0137C9 CLM */
    A = D;  /* 0137CA TDA */
    A += 1;  /* 0137CB INC */
    A += 1;  /* 0137CC INC */
    push16(A);  /* 0137CD PHA */
    S -= 3; F_01501B();  /* 0137CE JSRL */
    Y = pull16();  /* 0137D2 PLY */
    A = D;  /* 0137D3 TDA */
    A += 1;  /* 0137D4 INC */
    A += 1;  /* 0137D5 INC */
    push16(A);  /* 0137D6 PHA */
    S -= 3; F_014031();  /* 0137D7 JSRL */
    Y = pull16();  /* 0137DB PLY */
    Y = 0x0013;  /* 0137DC LDY */
    A = D;  /* 0137DF TDA */
    A += 1;  /* 0137E0 INC */
    A += 1;  /* 0137E1 INC */
    push16(A);  /* 0137E2 PHA */
    S -= 3; F_0080AA();  /* 0137E3 JSRL */
    Y = pull16();  /* 0137E7 PLY */
    goto L_013821;  /* 0137E8 JMP */
L_0137EB: TICK();
    mflag = 0;  /* 0137EB CLM */
    wr16(DP(0x02), 0x001A);  /* 0137EC LDM */
    wr16(DP(0x04), 0x0040);  /* 0137F0 LDM */
    Y = 0x0002;  /* 0137F4 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0137F7 JSRL */
    wr8(ABS(0x0118), 0x08);  /* 0137FB LDM */
    wr8(ABS(0x03A0), 0x20);  /* 0137FF LDM */
    wr8(ABS(0x041E), 0x08);  /* 013803 LDM */
    mflag = 0;  /* 013807 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 013808 CLB */
    mflag = 1;  /* 01380D SEM */
    wr8(ABS(0x04E6), 0x00);  /* 01380E LDM */
    mflag = 0;  /* 013812 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x1000);  /* 013813 CLB */
    mflag = 1;  /* 013818 SEM */
    wr8(ABS(0x01AA), 0x00);  /* 013819 LDM */
    wr8(ABS(0x019C), 0x00);  /* 01381D LDM */
L_013821: TICK();
    goto L_01385D;  /* 013821 JMP */
L_013824: TICK();
    mflag = 0;  /* 013824 CLM */
    wr16(DP(0x02), 0x0006);  /* 013825 LDM */
    wr16(DP(0x03), 0x01A4);  /* 013829 LDM */
    wr16(DP(0x05), 0x0000);  /* 01382D LDM */
    A = rd16(ABS(0x019C));  /* 013831 LDA */
    A &= 0x00FF;  /* 013834 AND */
    C = 0;  /* 013837 CLC */
    A = adc16(A, 0x0119);  /* 013838 ADC */
    B = 0x0000;  /* 01383B LDA */
    Y = A;  /* 01383F TAY */
    A = D;  /* 013840 TDA */
    A += 1;  /* 013841 INC */
    A += 1;  /* 013842 INC */
    push16(A);  /* 013843 PHA */
    S -= 3; F_013FD9();  /* 013844 JSRL */
    Y = pull16();  /* 013848 PLY */
    A = rd16(ABS(0x019C));  /* 013849 LDA */
    C = 0;  /* 01384C CLC */
    mflag = 1;  /* 01384D SEM */
    SETLO(A, adc8(A, 0x06));  /* 01384E ADC */
    wr8(ABS(0x019C), (uint8_t)A);  /* 013850 STA */
    wr8(ABS(0x01AA), 0x02);  /* 013853 LDM */
    mflag = 0;  /* 013857 CLM */
    wr16(ABS(0x019E), rd16(ABS(0x019E)) & ~0x0008);  /* 013858 CLB */
L_01385D: TICK();
    mflag = 1;  /* 01385D SEM */
    wr8(ABS(0x019D), 0x00);  /* 01385E LDM */
L_013862: TICK();
    goto L_0138AB;  /* 013862 JMP */
L_013865: TICK();
    mflag = 1;  /* 013865 SEM */
    SETLO(A, rd8(ABS(0x041E)));  /* 013866 LDA */
    if ((A & 0xFF) != 0) goto L_0138AB;  /* 013869 BNE */
    wr8(DP(0x02), (uint8_t)A);  /* 01386B STA */
    SETLO(A, rd8(ABS(0x011D)));  /* 01386D LDA */
    SETLO(A, A & 0x0F);  /* 013870 AND */
    mflag = 0;  /* 013872 CLM */
    A &= 0x00FF;  /* 013873 AND */
    A = rla16(A, 0x0008);  /* 013876 RLA */
    A &= 0xFF00;  /* 01387A AND */
    wr16(DP(0x03), A);  /* 01387D STA */
    A = rd16(ABS(0x011E));  /* 01387F LDA */
    A &= 0x00FF;  /* 013882 AND */
    C = 0;  /* 013885 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 013886 ADC */
    wr16(DP(0x03), A);  /* 013888 STA */
    A = rd16(ABS(0x011B));  /* 01388A LDA */
    mflag = 1;  /* 01388D SEM */
    SETLO(A, A & 0x7F);  /* 01388E AND */
    wr8(DP(0x05), (uint8_t)A);  /* 013890 STA */
    wr8(DP(0x06), 0x08);  /* 013892 LDM */
    Y = 0x00E0;  /* 013895 LDY */
    mflag = 0;  /* 013898 CLM */
    A = D;  /* 013899 TDA */
    A += 1;  /* 01389A INC */
    A += 1;  /* 01389B INC */
    push16(A);  /* 01389C PHA */
    S -= 3; F_01501B();  /* 01389D JSRL */
    Y = pull16();  /* 0138A1 PLY */
    A = D;  /* 0138A2 TDA */
    A += 1;  /* 0138A3 INC */
    A += 1;  /* 0138A4 INC */
    push16(A);  /* 0138A5 PHA */
    S -= 3; F_014031();  /* 0138A6 JSRL */
    Y = pull16();  /* 0138AA PLY */
L_0138AB: TICK();
    goto L_013FD5;  /* 0138AB JMP */
L_0138AE: TICK();
    mflag = 0;  /* 0138AE CLM */
    A = rd16(ABS(0x0118));  /* 0138AF LDA */
    mflag = 1;  /* 0138B2 SEM */
    /* 0138B3 CMP */
    if ((A & 0xFF) == 0x08) goto L_0138BA;  /* 0138B5 BEQ */
    goto L_013AA1;  /* 0138B7 JMP */
L_0138BA: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 0138BA LDA */
    /* 0138BD CMP */
    if ((A & 0xFF) == 0x02) goto L_0138C5;  /* 0138BF BEQ */
    wr8(ABS(0x03A0), 0x20);  /* 0138C1 LDM */
L_0138C5: TICK();
    SETLO(A, rd8(ABS(0x03A0)));  /* 0138C5 LDA */
    if ((A & 0xFF) == 0) goto L_0138CF;  /* 0138C8 BEQ */
    SETLO(A, rd8(ABS(0x041E)));  /* 0138CA LDA */
    if ((A & 0xFF) != 0) goto L_0138DC;  /* 0138CD BNE */
L_0138CF: TICK();
    mflag = 0;  /* 0138CF CLM */
    A = D;  /* 0138D0 TDA */
    A += 1;  /* 0138D1 INC */
    A += 1;  /* 0138D2 INC */
    push16(A);  /* 0138D3 PHA */
    S -= 3; F_01406D();  /* 0138D4 JSRL */
    Y = pull16();  /* 0138D8 PLY */
    goto L_013A9E;  /* 0138D9 JMP */
L_0138DC: TICK();
    mflag = 1;  /* 0138DC SEM */
    SETLO(A, rd8(ABS(0x019D)));  /* 0138DD LDA */
    if ((A & 0xFF) != 0) goto L_0138E5;  /* 0138E0 BNE */
    goto L_013A9E;  /* 0138E2 JMP */
L_0138E5: TICK();
    mflag = 0;  /* 0138E5 CLM */
    if ((rd16(ABS(0x019E)) & 0x0002) == 0x0002) goto L_0138EF;  /* 0138E6 BBS */
    goto L_013A99;  /* 0138EC JMP */
L_0138EF: TICK();
    A = rd16(ABS(0x0119));  /* 0138EF LDA */
    A &= 0x00FF;  /* 0138F2 AND */
    wr16(DP(0x00), A);  /* 0138F5 STA */
    wr16(DP(0x02), 0x0006);  /* 0138F7 LDM */
    wr16(DP(0x03), 0x0119);  /* 0138FB LDM */
    wr16(DP(0x05), 0x0000);  /* 0138FF LDM */
    B = 0x0000;  /* 013903 LDA */
    Y = 0x01A4;  /* 013907 LDY */
    A = D;  /* 01390A TDA */
    A += 1;  /* 01390B INC */
    A += 1;  /* 01390C INC */
    push16(A);  /* 01390D PHA */
    S -= 3; F_013FD9();  /* 01390E JSRL */
    Y = pull16();  /* 013912 PLY */
    A = rd16(ABS(0x0119));  /* 013913 LDA */
    mflag = 1;  /* 013916 SEM */
    SETLO(A, A & 0xFC);  /* 013917 AND */
    /* 013919 CMP */
    if ((A & 0xFF) == 0x48) goto L_013929;  /* 01391B BEQ */
    SETLO(A, rd8(ABS(0x0119)));  /* 01391D LDA */
    SETLO(A, A & 0xFC);  /* 013920 AND */
    /* 013922 CMP */
    if ((A & 0xFF) == 0x44) goto L_013929;  /* 013924 BEQ */
    goto L_013A99;  /* 013926 JMP */
L_013929: TICK();
    SETLO(A, rd8(ABS(0x0199)));  /* 013929 LDA */
    mflag = 0;  /* 01392C CLM */
    A &= 0x00FF;  /* 01392D AND */
    mflag = 1;  /* 013930 SEM */
    SETLO(B, 0x00);  /* 013931 LDA */
    mflag = 0;  /* 013934 CLM */
    wr16(DP(0x02), A);  /* 013935 STA */
    A = rd16(ABS(0x011A));  /* 013937 LDA */
    mflag = 1;  /* 01393A SEM */
    div8(0x10);  /* 01393B DIV */
    mflag = 0;  /* 01393E CLM */
    A &= 0x00FF;  /* 01393F AND */
    wr16(DP(0x04), A);  /* 013942 STA */
    A = rd16(ABS(0x0119));  /* 013944 LDA */
    mflag = 1;  /* 013947 SEM */
    SETLO(A, A & 0x03);  /* 013948 AND */
    mflag = 0;  /* 01394A CLM */
    A &= 0x00FF;  /* 01394B AND */
    A = asl16(A);  /* 01394E ASL */
    A = asl16(A);  /* 01394F ASL */
    A = asl16(A);  /* 013950 ASL */
    A = asl16(A);  /* 013951 ASL */
    C = 0;  /* 013952 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 013953 ADC */
    t = rd16(DP(0x02));  /* 013955 CMP */
    if (A == t) goto L_01395C;  /* 013957 BEQ */
    goto L_013A99;  /* 013959 JMP */
L_01395C: TICK();
    A = rd16(ABS(0x011A));  /* 01395C LDA */
    mflag = 1;  /* 01395F SEM */
    SETLO(A, A & 0x0F);  /* 013960 AND */
    mflag = 0;  /* 013962 CLM */
    A &= 0x00FF;  /* 013963 AND */
    A = rla16(A, 0x0008);  /* 013966 RLA */
    A &= 0xFF00;  /* 01396A AND */
    wr16(DP(0x02), A);  /* 01396D STA */
    A = rd16(ABS(0x011B));  /* 01396F LDA */
    A &= 0x00FF;  /* 013972 AND */
    C = 0;  /* 013975 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013976 ADC */
    t = rd16(ABS(0x0195));  /* 013978 CMP */
    if (A == t) goto L_013980;  /* 01397B BEQ */
    goto L_013A99;  /* 01397D JMP */
L_013980: TICK();
    mflag = 1;  /* 013980 SEM */
    SETLO(B, 0x00);  /* 013981 LDA */
    SETLO(A, rd8(ABS(0x011D)));  /* 013984 LDA */
    div8(0x10);  /* 013987 DIV */
    mflag = 0;  /* 01398A CLM */
    A &= 0x00FF;  /* 01398B AND */
    wr16(DP(0x02), A);  /* 01398E STA */
    A = rd16(ABS(0x011C));  /* 013990 LDA */
    A &= 0x00FF;  /* 013993 AND */
    A = asl16(A);  /* 013996 ASL */
    A = asl16(A);  /* 013997 ASL */
    A = asl16(A);  /* 013998 ASL */
    A = asl16(A);  /* 013999 ASL */
    C = 0;  /* 01399A CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 01399B ADC */
    t = rd16(ABS(0x03D5));  /* 01399D CMP */
    if (A == t) goto L_0139A5;  /* 0139A0 BEQ */
    goto L_013A99;  /* 0139A2 JMP */
L_0139A5: TICK();
    A = rd16(ABS(0x011D));  /* 0139A5 LDA */
    mflag = 1;  /* 0139A8 SEM */
    SETLO(A, A & 0x0F);  /* 0139A9 AND */
    mflag = 0;  /* 0139AB CLM */
    A &= 0x00FF;  /* 0139AC AND */
    A = rla16(A, 0x0008);  /* 0139AF RLA */
    A &= 0xFF00;  /* 0139B3 AND */
    wr16(DP(0x02), A);  /* 0139B6 STA */
    A = rd16(ABS(0x011E));  /* 0139B8 LDA */
    A &= 0x00FF;  /* 0139BB AND */
    C = 0;  /* 0139BE CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 0139BF ADC */
    t = rd16(ABS(0x0197));  /* 0139C1 CMP */
    if (A == t) goto L_0139C9;  /* 0139C4 BEQ */
    goto L_013A99;  /* 0139C6 JMP */
L_0139C9: TICK();
    A = rd16(ABS(0x0119));  /* 0139C9 LDA */
    mflag = 1;  /* 0139CC SEM */
    SETLO(A, A & 0xFC);  /* 0139CD AND */
    /* 0139CF CMP */
    if ((A & 0xFF) != 0x48) goto L_013A24;  /* 0139D1 BNE */
    wr8(DP(0x02), 0x00);  /* 0139D3 LDM */
    SETLO(B, 0x00);  /* 0139D6 LDA */
    SETLO(A, rd8(ABS(0x011D)));  /* 0139D9 LDA */
    div8(0x10);  /* 0139DC DIV */
    mflag = 0;  /* 0139DF CLM */
    A &= 0x00FF;  /* 0139E0 AND */
    wr16(DP(0x03), A);  /* 0139E3 STA */
    A = rd16(ABS(0x011C));  /* 0139E5 LDA */
    A &= 0x00FF;  /* 0139E8 AND */
    A = asl16(A);  /* 0139EB ASL */
    A = asl16(A);  /* 0139EC ASL */
    A = asl16(A);  /* 0139ED ASL */
    A = asl16(A);  /* 0139EE ASL */
    C = 0;  /* 0139EF CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 0139F0 ADC */
    wr16(DP(0x03), A);  /* 0139F2 STA */
    A = rd16(ABS(0x011B));  /* 0139F4 LDA */
    mflag = 1;  /* 0139F7 SEM */
    SETLO(A, A & 0x7F);  /* 0139F8 AND */
    wr8(DP(0x05), (uint8_t)A);  /* 0139FA STA */
    wr8(DP(0x06), 0x00);  /* 0139FC LDM */
    Y = 0x00C0;  /* 0139FF LDY */
    mflag = 0;  /* 013A02 CLM */
    A = D;  /* 013A03 TDA */
    A += 1;  /* 013A04 INC */
    A += 1;  /* 013A05 INC */
    push16(A);  /* 013A06 PHA */
    S -= 3; F_01501B();  /* 013A07 JSRL */
    Y = pull16();  /* 013A0B PLY */
    A = D;  /* 013A0C TDA */
    A += 1;  /* 013A0D INC */
    A += 1;  /* 013A0E INC */
    push16(A);  /* 013A0F PHA */
    S -= 3; F_014031();  /* 013A10 JSRL */
    Y = pull16();  /* 013A14 PLY */
    Y = 0x0013;  /* 013A15 LDY */
    A = D;  /* 013A18 TDA */
    A += 1;  /* 013A19 INC */
    A += 1;  /* 013A1A INC */
    push16(A);  /* 013A1B PHA */
    S -= 3; F_0080AA();  /* 013A1C JSRL */
    Y = pull16();  /* 013A20 PLY */
    goto L_013A99;  /* 013A21 JMP */
L_013A24: TICK();
    mflag = 0;  /* 013A24 CLM */
    A = rd16(DP(0x00));  /* 013A25 LDA */
    mflag = 1;  /* 013A27 SEM */
    wr8(ABS(0x0119), (uint8_t)A);  /* 013A28 STA */
    SETLO(A, rd8(ABS(0x011E)));  /* 013A2B LDA */
    mflag = 0;  /* 013A2E CLM */
    A &= 0x00FF;  /* 013A2F AND */
    A = lsr16(A);  /* 013A32 LSR */
    A = lsr16(A);  /* 013A33 LSR */
    A = lsr16(A);  /* 013A34 LSR */
    A = lsr16(A);  /* 013A35 LSR */
    wr16(DP(0x02), A);  /* 013A36 STA */
    A = rd16(ABS(0x011C));  /* 013A38 LDA */
    mflag = 1;  /* 013A3B SEM */
    SETLO(A, A & 0x0F);  /* 013A3C AND */
    mflag = 0;  /* 013A3E CLM */
    A &= 0x00FF;  /* 013A3F AND */
    A = rla16(A, 0x0008);  /* 013A42 RLA */
    A &= 0xFF00;  /* 013A46 AND */
    wr16(DP(0x04), A);  /* 013A49 STA */
    A = rd16(ABS(0x011D));  /* 013A4B LDA */
    A &= 0x00FF;  /* 013A4E AND */
    C = 0;  /* 013A51 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 013A52 ADC */
    A = asl16(A);  /* 013A54 ASL */
    A = asl16(A);  /* 013A55 ASL */
    A = asl16(A);  /* 013A56 ASL */
    A = asl16(A);  /* 013A57 ASL */
    C = 0;  /* 013A58 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013A59 ADC */
    wr16(DP(0x00), A);  /* 013A5B STA */
    A = rd16(ABS(0x011C));  /* 013A5D LDA */
    A &= 0x00FF;  /* 013A60 AND */
    A = lsr16(A);  /* 013A63 LSR */
    A = lsr16(A);  /* 013A64 LSR */
    A = lsr16(A);  /* 013A65 LSR */
    A = lsr16(A);  /* 013A66 LSR */
    mflag = 1;  /* 013A67 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 013A68 STA */
    SETLO(A, rd8(ABS(0x011E)));  /* 013A6A LDA */
    SETLO(A, A & 0x0F);  /* 013A6D AND */
    SETLO(A, asl8(A));  /* 013A6F ASL */
    SETLO(A, asl8(A));  /* 013A70 ASL */
    SETLO(A, asl8(A));  /* 013A71 ASL */
    SETLO(A, asl8(A));  /* 013A72 ASL */
    C = 0;  /* 013A73 CLC */
    SETLO(A, adc8(A, rd8(DP(0x02))));  /* 013A74 ADC */
    wr8(ABS(0x011D), (uint8_t)A);  /* 013A76 STA */
    SETLO(A, rd8(DP(0x00)));  /* 013A79 LDA */
    wr8(ABS(0x011C), (uint8_t)A);  /* 013A7B STA */
    SETLO(A, rd8(DP(0x01)));  /* 013A7E LDA */
    wr8(ABS(0x011E), (uint8_t)A);  /* 013A80 STA */
    mflag = 0;  /* 013A83 CLM */
    A = D;  /* 013A84 TDA */
    A += 1;  /* 013A85 INC */
    A += 1;  /* 013A86 INC */
    push16(A);  /* 013A87 PHA */
    S -= 3; F_01406D();  /* 013A88 JSRL */
    Y = pull16();  /* 013A8C PLY */
    Y = 0x001B;  /* 013A8D LDY */
    A = D;  /* 013A90 TDA */
    A += 1;  /* 013A91 INC */
    A += 1;  /* 013A92 INC */
    push16(A);  /* 013A93 PHA */
    S -= 3; F_0080AA();  /* 013A94 JSRL */
    Y = pull16();  /* 013A98 PLY */
L_013A99: TICK();
    mflag = 1;  /* 013A99 SEM */
    wr8(ABS(0x019D), 0x00);  /* 013A9A LDM */
L_013A9E: TICK();
    goto L_013FD5;  /* 013A9E JMP */
L_013AA1: TICK();
    mflag = 0;  /* 013AA1 CLM */
    A = rd16(ABS(0x0118));  /* 013AA2 LDA */
    mflag = 1;  /* 013AA5 SEM */
    /* 013AA6 CMP */
    if ((A & 0xFF) == 0x09) goto L_013AAD;  /* 013AA8 BEQ */
    goto L_013C2D;  /* 013AAA JMP */
L_013AAD: TICK();
    SETLO(A, rd8(ABS(0x019D)));  /* 013AAD LDA */
    if ((A & 0xFF) != 0) goto L_013AB5;  /* 013AB0 BNE */
    goto L_013C2A;  /* 013AB2 JMP */
L_013AB5: TICK();
    mflag = 0;  /* 013AB5 CLM */
    if ((rd16(ABS(0x019E)) & 0x0002) == 0x0002) goto L_013ABF;  /* 013AB6 BBS */
    goto L_013C20;  /* 013ABC JMP */
L_013ABF: TICK();
    wr16(DP(0x02), 0x0006);  /* 013ABF LDM */
    A = rd16(ABS(0x019C));  /* 013AC3 LDA */
    A &= 0x00FF;  /* 013AC6 AND */
    C = 0;  /* 013AC9 CLC */
    A = adc16(A, 0x0119);  /* 013ACA ADC */
    B = 0x0000;  /* 013ACD LDA */
    wr16(DP(0x03), A);  /* 013AD1 STA */
    wr16(DP(0x05), B);  /* 013AD3 STA */
    Y = 0x01A4;  /* 013AD6 LDY */
    A = D;  /* 013AD9 TDA */
    A += 1;  /* 013ADA INC */
    A += 1;  /* 013ADB INC */
    push16(A);  /* 013ADC PHA */
    S -= 3; F_013FFE();  /* 013ADD JSRL */
    Y = pull16();  /* 013AE1 PLY */
    wr16(DP(0x02), 0x0006);  /* 013AE2 LDM */
    A = rd16(ABS(0x019C));  /* 013AE6 LDA */
    A &= 0x00FF;  /* 013AE9 AND */
    C = 0;  /* 013AEC CLC */
    A = adc16(A, 0x0119);  /* 013AED ADC */
    B = 0x0000;  /* 013AF0 LDA */
    wr16(DP(0x03), A);  /* 013AF4 STA */
    wr16(DP(0x05), B);  /* 013AF6 STA */
    Y = 0x01A4;  /* 013AF9 LDY */
    A = D;  /* 013AFC TDA */
    A += 1;  /* 013AFD INC */
    A += 1;  /* 013AFE INC */
    push16(A);  /* 013AFF PHA */
    S -= 3; F_013FD9();  /* 013B00 JSRL */
    Y = pull16();  /* 013B04 PLY */
    mflag = 1;  /* 013B05 SEM */
    wr8(ABS(0x01AA), 0x00);  /* 013B06 LDM */
    wr8(ABS(0x019D), 0x00);  /* 013B0A LDM */
    SETLO(A, rd8(ABS(0x0119)));  /* 013B0E LDA */
    SETLO(A, A & 0xFC);  /* 013B11 AND */
    /* 013B13 CMP */
    if ((A & 0xFF) == 0x64) goto L_013B2B;  /* 013B15 BEQ */
    if ((rd8(ABS(0x0119)) & 0x80) == 0x80) goto L_013B1F;  /* 013B17 BBS */
    goto L_013C13;  /* 013B1C JMP */
L_013B1F: TICK();
    SETLO(A, rd8(ABS(0x0119)));  /* 013B1F LDA */
    SETLO(A, A & 0xE0);  /* 013B22 AND */
    /* 013B24 CMP */
    if ((A & 0xFF) != 0xC0) goto L_013B2B;  /* 013B26 BNE */
    goto L_013C13;  /* 013B28 JMP */
L_013B2B: TICK();
    SETLO(A, rd8(ABS(0x0199)));  /* 013B2B LDA */
    mflag = 0;  /* 013B2E CLM */
    A &= 0x00FF;  /* 013B2F AND */
    mflag = 1;  /* 013B32 SEM */
    SETLO(B, 0x00);  /* 013B33 LDA */
    mflag = 0;  /* 013B36 CLM */
    wr16(DP(0x02), A);  /* 013B37 STA */
    A = rd16(ABS(0x011A));  /* 013B39 LDA */
    mflag = 1;  /* 013B3C SEM */
    div8(0x10);  /* 013B3D DIV */
    mflag = 0;  /* 013B40 CLM */
    A &= 0x00FF;  /* 013B41 AND */
    wr16(DP(0x04), A);  /* 013B44 STA */
    A = rd16(ABS(0x0119));  /* 013B46 LDA */
    mflag = 1;  /* 013B49 SEM */
    SETLO(A, A & 0x03);  /* 013B4A AND */
    mflag = 0;  /* 013B4C CLM */
    A &= 0x00FF;  /* 013B4D AND */
    A = asl16(A);  /* 013B50 ASL */
    A = asl16(A);  /* 013B51 ASL */
    A = asl16(A);  /* 013B52 ASL */
    A = asl16(A);  /* 013B53 ASL */
    C = 0;  /* 013B54 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 013B55 ADC */
    t = rd16(DP(0x02));  /* 013B57 CMP */
    if (A == t) goto L_013B5E;  /* 013B59 BEQ */
    goto L_013C13;  /* 013B5B JMP */
L_013B5E: TICK();
    A = rd16(ABS(0x011A));  /* 013B5E LDA */
    mflag = 1;  /* 013B61 SEM */
    SETLO(A, A & 0x0F);  /* 013B62 AND */
    mflag = 0;  /* 013B64 CLM */
    A &= 0x00FF;  /* 013B65 AND */
    A = rla16(A, 0x0008);  /* 013B68 RLA */
    A &= 0xFF00;  /* 013B6C AND */
    wr16(DP(0x02), A);  /* 013B6F STA */
    A = rd16(ABS(0x011B));  /* 013B71 LDA */
    A &= 0x00FF;  /* 013B74 AND */
    C = 0;  /* 013B77 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013B78 ADC */
    t = rd16(ABS(0x0195));  /* 013B7A CMP */
    if (A == t) goto L_013B82;  /* 013B7D BEQ */
    goto L_013C13;  /* 013B7F JMP */
L_013B82: TICK();
    mflag = 1;  /* 013B82 SEM */
    SETLO(B, 0x00);  /* 013B83 LDA */
    SETLO(A, rd8(ABS(0x011D)));  /* 013B86 LDA */
    div8(0x10);  /* 013B89 DIV */
    mflag = 0;  /* 013B8C CLM */
    A &= 0x00FF;  /* 013B8D AND */
    wr16(DP(0x02), A);  /* 013B90 STA */
    A = rd16(ABS(0x011C));  /* 013B92 LDA */
    A &= 0x00FF;  /* 013B95 AND */
    A = asl16(A);  /* 013B98 ASL */
    A = asl16(A);  /* 013B99 ASL */
    A = asl16(A);  /* 013B9A ASL */
    A = asl16(A);  /* 013B9B ASL */
    C = 0;  /* 013B9C CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013B9D ADC */
    t = rd16(ABS(0x017F));  /* 013B9F CMP */
    if (A == t) goto L_013BA9;  /* 013BA2 BEQ */
    X = rd16(ABS(0x017F));  /* 013BA4 LDX */
    if (X != 0) goto L_013C13;  /* 013BA7 BNE */
L_013BA9: TICK();
    A = rd16(ABS(0x011D));  /* 013BA9 LDA */
    mflag = 1;  /* 013BAC SEM */
    SETLO(A, A & 0x0F);  /* 013BAD AND */
    mflag = 0;  /* 013BAF CLM */
    A &= 0x00FF;  /* 013BB0 AND */
    A = rla16(A, 0x0008);  /* 013BB3 RLA */
    A &= 0xFF00;  /* 013BB7 AND */
    wr16(DP(0x02), A);  /* 013BBA STA */
    A = rd16(ABS(0x011E));  /* 013BBC LDA */
    A &= 0x00FF;  /* 013BBF AND */
    C = 0;  /* 013BC2 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013BC3 ADC */
    t = rd16(ABS(0x0197));  /* 013BC5 CMP */
    if (A == t) goto L_013BF0;  /* 013BC8 BEQ */
    A = rd16(ABS(0x011D));  /* 013BCA LDA */
    mflag = 1;  /* 013BCD SEM */
    SETLO(A, A & 0x0F);  /* 013BCE AND */
    mflag = 0;  /* 013BD0 CLM */
    A &= 0x00FF;  /* 013BD1 AND */
    A = rla16(A, 0x0008);  /* 013BD4 RLA */
    A &= 0xFF00;  /* 013BD8 AND */
    wr16(DP(0x02), A);  /* 013BDB STA */
    A = rd16(ABS(0x011E));  /* 013BDD LDA */
    A &= 0x00FF;  /* 013BE0 AND */
    C = 0;  /* 013BE3 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013BE4 ADC */
    /* 013BE6 CMP */
    if (A != 0x0FFF) goto L_013C13;  /* 013BE9 BNE */
    X = rd16(ABS(0x017F));  /* 013BEB LDX */
    if (X == 0) goto L_013C13;  /* 013BEE BEQ */
L_013BF0: TICK();
    A = rd16(ABS(0x0119));  /* 013BF0 LDA */
    mflag = 1;  /* 013BF3 SEM */
    SETLO(A, A & 0xFC);  /* 013BF4 AND */
    /* 013BF6 CMP */
    if ((A & 0xFF) != 0x64) goto L_013C01;  /* 013BF8 BNE */
    wr8(ABS(0x0118), 0x0A);  /* 013BFA LDM */
    goto L_013C0B;  /* 013BFE JMP */
L_013C01: TICK();
    mflag = 0;  /* 013C01 CLM */
    A = D;  /* 013C02 TDA */
    A += 1;  /* 013C03 INC */
    A += 1;  /* 013C04 INC */
    push16(A);  /* 013C05 PHA */
    S -= 3; F_014144();  /* 013C06 JSRL */
    Y = pull16();  /* 013C0A PLY */
L_013C0B: TICK();
    mflag = 1;  /* 013C0B SEM */
    wr8(ABS(0x019C), 0x06);  /* 013C0C LDM */
    goto L_013C1D;  /* 013C10 JMP */
L_013C13: TICK();
    mflag = 0;  /* 013C13 CLM */
    A = D;  /* 013C14 TDA */
    A += 1;  /* 013C15 INC */
    A += 1;  /* 013C16 INC */
    push16(A);  /* 013C17 PHA */
    S -= 3; F_014031();  /* 013C18 JSRL */
    Y = pull16();  /* 013C1C PLY */
L_013C1D: TICK();
    goto L_013C2A;  /* 013C1D JMP */
L_013C20: TICK();
    mflag = 0;  /* 013C20 CLM */
    A = D;  /* 013C21 TDA */
    A += 1;  /* 013C22 INC */
    A += 1;  /* 013C23 INC */
    push16(A);  /* 013C24 PHA */
    S -= 3; F_014031();  /* 013C25 JSRL */
    Y = pull16();  /* 013C29 PLY */
L_013C2A: TICK();
    goto L_013FD5;  /* 013C2A JMP */
L_013C2D: TICK();
    mflag = 0;  /* 013C2D CLM */
    A = rd16(ABS(0x0118));  /* 013C2E LDA */
    mflag = 1;  /* 013C31 SEM */
    /* 013C32 CMP */
    if ((A & 0xFF) == 0x0A) goto L_013C39;  /* 013C34 BEQ */
    goto L_013CF3;  /* 013C36 JMP */
L_013C39: TICK();
    SETLO(A, rd8(ABS(0x019D)));  /* 013C39 LDA */
    if ((A & 0xFF) != 0) goto L_013C41;  /* 013C3C BNE */
    goto L_013CF0;  /* 013C3E JMP */
L_013C41: TICK();
    mflag = 0;  /* 013C41 CLM */
    if ((rd16(ABS(0x019E)) & 0x0002) == 0x0002) goto L_013C4B;  /* 013C42 BBS */
    goto L_013CD8;  /* 013C48 JMP */
L_013C4B: TICK();
    X = 0x01A4;  /* 013C4B LDX */
    A = rd16((ABS(0x0002) + X));  /* 013C4E LDA */
    mflag = 1;  /* 013C51 SEM */
    SETLO(A, A & 0xFC);  /* 013C52 AND */
    t = rd8(ABS(0x0121));  /* 013C54 CMP */
    if ((A & 0xFF) != t) goto L_013C5D;  /* 013C57 BNE */
    wr8(ABS(0x01A6), rd8(ABS(0x01A6)) & ~0x03);  /* 013C59 CLB */
L_013C5D: TICK();
    wr8(DP(0x02), 0x06);  /* 013C5D LDM */
    SETLO(A, rd8(ABS(0x019C)));  /* 013C60 LDA */
    mflag = 0;  /* 013C63 CLM */
    A &= 0x00FF;  /* 013C64 AND */
    C = 0;  /* 013C67 CLC */
    A = adc16(A, 0x0119);  /* 013C68 ADC */
    B = 0x0000;  /* 013C6B LDA */
    wr16(DP(0x03), A);  /* 013C6F STA */
    wr16(DP(0x05), B);  /* 013C71 STA */
    Y = X;  /* 013C74 TXY */
    A = D;  /* 013C75 TDA */
    A += 1;  /* 013C76 INC */
    A += 1;  /* 013C77 INC */
    push16(A);  /* 013C78 PHA */
    S -= 3; F_013FFE();  /* 013C79 JSRL */
    Y = pull16();  /* 013C7D PLY */
    wr16(DP(0x02), 0x0006);  /* 013C7E LDM */
    A = rd16(ABS(0x019C));  /* 013C82 LDA */
    A &= 0x00FF;  /* 013C85 AND */
    C = 0;  /* 013C88 CLC */
    A = adc16(A, 0x0119);  /* 013C89 ADC */
    B = 0x0000;  /* 013C8C LDA */
    wr16(DP(0x03), A);  /* 013C90 STA */
    wr16(DP(0x05), B);  /* 013C92 STA */
    Y = 0x01A4;  /* 013C95 LDY */
    A = D;  /* 013C98 TDA */
    A += 1;  /* 013C99 INC */
    A += 1;  /* 013C9A INC */
    push16(A);  /* 013C9B PHA */
    S -= 3; F_013FD9();  /* 013C9C JSRL */
    Y = pull16();  /* 013CA0 PLY */
    A = rd16(ABS(0x019C));  /* 013CA1 LDA */
    C = 0;  /* 013CA4 CLC */
    mflag = 1;  /* 013CA5 SEM */
    SETLO(A, adc8(A, 0x06));  /* 013CA6 ADC */
    wr8(ABS(0x019C), (uint8_t)A);  /* 013CA8 STA */
    SETLO(B, 0x00);  /* 013CAB LDA */
    SETLO(A, rd8(ABS(0x011F)));  /* 013CAE LDA */
    div8(0x10);  /* 013CB1 DIV */
    SETLO(A, A + 1);  /* 013CB4 INC */
    SETLO(A, A + 1);  /* 013CB5 INC */
    mpy8(0x06);  /* 013CB6 MPY */
    wr8(ABS(0x018A), (uint8_t)A);  /* 013CB9 STA */
    t = rd8(ABS(0x019C));  /* 013CBC CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_013CC3;  /* 013CBF BCC */
    if (!Z) goto L_013CD0;  /* 013CC1 BNE */
L_013CC3: TICK();
    mflag = 0;  /* 013CC3 CLM */
    A = D;  /* 013CC4 TDA */
    A += 1;  /* 013CC5 INC */
    A += 1;  /* 013CC6 INC */
    push16(A);  /* 013CC7 PHA */
    S -= 3; F_014144();  /* 013CC8 JSRL */
    Y = pull16();  /* 013CCC PLY */
    goto L_013CD5;  /* 013CCD JMP */
L_013CD0: TICK();
    mflag = 1;  /* 013CD0 SEM */
    wr8(ABS(0x0118), 0x0B);  /* 013CD1 LDM */
L_013CD5: TICK();
    goto L_013CE7;  /* 013CD5 JMP */
L_013CD8: TICK();
    mflag = 0;  /* 013CD8 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0400);  /* 013CD9 SEB */
    A = D;  /* 013CDE TDA */
    A += 1;  /* 013CDF INC */
    A += 1;  /* 013CE0 INC */
    push16(A);  /* 013CE1 PHA */
    S -= 3; F_014144();  /* 013CE2 JSRL */
    Y = pull16();  /* 013CE6 PLY */
L_013CE7: TICK();
    mflag = 1;  /* 013CE7 SEM */
    wr8(ABS(0x01AA), 0x00);  /* 013CE8 LDM */
    wr8(ABS(0x019D), 0x00);  /* 013CEC LDM */
L_013CF0: TICK();
    goto L_013FD5;  /* 013CF0 JMP */
L_013CF3: TICK();
    mflag = 0;  /* 013CF3 CLM */
    A = rd16(ABS(0x0118));  /* 013CF4 LDA */
    mflag = 1;  /* 013CF7 SEM */
    /* 013CF8 CMP */
    if ((A & 0xFF) == 0x0B) goto L_013CFF;  /* 013CFA BEQ */
    goto L_013D8B;  /* 013CFC JMP */
L_013CFF: TICK();
    SETLO(A, rd8(ABS(0x019D)));  /* 013CFF LDA */
    if ((A & 0xFF) != 0) goto L_013D07;  /* 013D02 BNE */
    goto L_013D88;  /* 013D04 JMP */
L_013D07: TICK();
    mflag = 0;  /* 013D07 CLM */
    if ((rd16(ABS(0x019E)) & 0x0002) == 0) goto L_013D70;  /* 013D08 BBC */
    wr16(DP(0x02), 0x0006);  /* 013D0E LDM */
    A = rd16(ABS(0x019C));  /* 013D12 LDA */
    A &= 0x00FF;  /* 013D15 AND */
    C = 0;  /* 013D18 CLC */
    A = adc16(A, 0x0119);  /* 013D19 ADC */
    B = 0x0000;  /* 013D1C LDA */
    wr16(DP(0x03), A);  /* 013D20 STA */
    wr16(DP(0x05), B);  /* 013D22 STA */
    Y = 0x01A4;  /* 013D25 LDY */
    A = D;  /* 013D28 TDA */
    A += 1;  /* 013D29 INC */
    A += 1;  /* 013D2A INC */
    push16(A);  /* 013D2B PHA */
    S -= 3; F_013FFE();  /* 013D2C JSRL */
    Y = pull16();  /* 013D30 PLY */
    wr16(DP(0x02), 0x0006);  /* 013D31 LDM */
    A = rd16(ABS(0x019C));  /* 013D35 LDA */
    A &= 0x00FF;  /* 013D38 AND */
    C = 0;  /* 013D3B CLC */
    A = adc16(A, 0x0119);  /* 013D3C ADC */
    B = 0x0000;  /* 013D3F LDA */
    wr16(DP(0x03), A);  /* 013D43 STA */
    wr16(DP(0x05), B);  /* 013D45 STA */
    Y = 0x01A4;  /* 013D48 LDY */
    A = D;  /* 013D4B TDA */
    A += 1;  /* 013D4C INC */
    A += 1;  /* 013D4D INC */
    push16(A);  /* 013D4E PHA */
    S -= 3; F_013FD9();  /* 013D4F JSRL */
    Y = pull16();  /* 013D53 PLY */
    A = rd16(ABS(0x019C));  /* 013D54 LDA */
    C = 0;  /* 013D57 CLC */
    mflag = 1;  /* 013D58 SEM */
    SETLO(A, adc8(A, 0x06));  /* 013D59 ADC */
    wr8(ABS(0x019C), (uint8_t)A);  /* 013D5B STA */
    t = rd8(ABS(0x018A));  /* 013D5E CMP */
    if ((A & 0xFF) < t) goto L_013D6D;  /* 013D61 BCC */
    mflag = 0;  /* 013D63 CLM */
    A = D;  /* 013D64 TDA */
    A += 1;  /* 013D65 INC */
    A += 1;  /* 013D66 INC */
    push16(A);  /* 013D67 PHA */
    S -= 3; F_014144();  /* 013D68 JSRL */
    Y = pull16();  /* 013D6C PLY */
L_013D6D: TICK();
    goto L_013D7F;  /* 013D6D JMP */
L_013D70: TICK();
    mflag = 0;  /* 013D70 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0400);  /* 013D71 SEB */
    A = D;  /* 013D76 TDA */
    A += 1;  /* 013D77 INC */
    A += 1;  /* 013D78 INC */
    push16(A);  /* 013D79 PHA */
    S -= 3; F_014144();  /* 013D7A JSRL */
    Y = pull16();  /* 013D7E PLY */
L_013D7F: TICK();
    mflag = 1;  /* 013D7F SEM */
    wr8(ABS(0x01AA), 0x00);  /* 013D80 LDM */
    wr8(ABS(0x019D), 0x00);  /* 013D84 LDM */
L_013D88: TICK();
    goto L_013FD5;  /* 013D88 JMP */
L_013D8B: TICK();
    mflag = 0;  /* 013D8B CLM */
    A = rd16(ABS(0x0118));  /* 013D8C LDA */
    mflag = 1;  /* 013D8F SEM */
    /* 013D90 CMP */
    if ((A & 0xFF) == 0x0C) goto L_013D97;  /* 013D92 BEQ */
    goto L_013E3B;  /* 013D94 JMP */
L_013D97: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 013D97 LDA */
    /* 013D9A CMP */
    if ((A & 0xFF) == 0x02) goto L_013DA3;  /* 013D9C BEQ */
    SETLO(A, rd8(ABS(0x03A0)));  /* 013D9E LDA */
    if ((A & 0xFF) != 0) goto L_013DBB;  /* 013DA1 BNE */
L_013DA3: TICK();
    mflag = 0;  /* 013DA3 CLM */
    if ((rd16(ABS(0x0244)) & 0x0008) == 0x0008) goto L_013DBB;  /* 013DA4 BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 013DAA SEB */
    mflag = 1;  /* 013DAF SEM */
    wr8(ABS(0x04E6), 0x01);  /* 013DB0 LDM */
    wr8(ABS(0x041E), 0x02);  /* 013DB4 LDM */
    goto L_013E2B;  /* 013DB8 JMP */
L_013DBB: TICK();
    mflag = 0;  /* 013DBB CLM */
    A = rd16(ABS(0x0228));  /* 013DBC LDA */
    mflag = 1;  /* 013DBF SEM */
    /* 013DC0 CMP */
    if ((A & 0xFF) != 0x08) goto L_013E1B;  /* 013DC2 BNE */
    SETLO(A, rd8(ABS(0x019C)));  /* 013DC4 LDA */
    if ((A & 0xFF) == 0) goto L_013DFE;  /* 013DC7 BEQ */
    wr8(ABS(0x019C), 0x00);  /* 013DC9 LDM */
    wr8(ABS(0x019D), 0x00);  /* 013DCD LDM */
    wr8(ABS(0x01AA), 0x00);  /* 013DD1 LDM */
    mflag = 0;  /* 013DD5 CLM */
    wr16(DP(0x02), 0x0009);  /* 013DD6 LDM */
    wr16(DP(0x04), 0x0040);  /* 013DDA LDM */
    Y = 0x0002;  /* 013DDE LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 013DE1 JSRL */
    mflag = 0;  /* 013DE5 CLM */
    wr16(DP(0x02), 0x00AA);  /* 013DE6 LDM */
    wr16(DP(0x04), 0x0043);  /* 013DEA LDM */
    Y = 0x0002;  /* 013DEE LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 013DF1 JSRL */
    mflag = 0;  /* 013DF5 CLM */
    wr16(ABS(0x019E), rd16(ABS(0x019E)) & ~0x0008);  /* 013DF6 CLB */
    goto L_013E18;  /* 013DFB JMP */
L_013DFE: TICK();
    mflag = 1;  /* 013DFE SEM */
    SETLO(A, rd8(ABS(0x019D)));  /* 013DFF LDA */
    if ((A & 0xFF) == 0) goto L_013E18;  /* 013E02 BEQ */
    wr8(ABS(0x019D), 0x00);  /* 013E04 LDM */
    mflag = 0;  /* 013E08 CLM */
    if ((rd16(ABS(0x019E)) & 0x0008) == 0) goto L_013E18;  /* 013E09 BBC */
    A = D;  /* 013E0F TDA */
    A += 1;  /* 013E10 INC */
    A += 1;  /* 013E11 INC */
    push16(A);  /* 013E12 PHA */
    S -= 3; F_014031();  /* 013E13 JSRL */
    Y = pull16();  /* 013E17 PLY */
L_013E18: TICK();
    goto L_013E2B;  /* 013E18 JMP */
L_013E1B: TICK();
    mflag = 1;  /* 013E1B SEM */
    SETLO(A, rd8(ABS(0x041E)));  /* 013E1C LDA */
    if ((A & 0xFF) != 0) goto L_013E2B;  /* 013E1F BNE */
    mflag = 0;  /* 013E21 CLM */
    A = D;  /* 013E22 TDA */
    A += 1;  /* 013E23 INC */
    A += 1;  /* 013E24 INC */
    push16(A);  /* 013E25 PHA */
    S -= 3; F_014031();  /* 013E26 JSRL */
    Y = pull16();  /* 013E2A PLY */
L_013E2B: TICK();
    mflag = 0;  /* 013E2B CLM */
    A = rd16(ABS(0x0118));  /* 013E2C LDA */
    mflag = 1;  /* 013E2F SEM */
    /* 013E30 CMP */
    if ((A & 0xFF) != 0x03) goto L_013E38;  /* 013E32 BNE */
    wr8(ABS(0x0118), 0x0D);  /* 013E34 LDM */
L_013E38: TICK();
    goto L_013FD5;  /* 013E38 JMP */
L_013E3B: TICK();
    mflag = 0;  /* 013E3B CLM */
    A = rd16(ABS(0x0118));  /* 013E3C LDA */
    mflag = 1;  /* 013E3F SEM */
    /* 013E40 CMP */
    if ((A & 0xFF) == 0x0D) goto L_013E47;  /* 013E42 BEQ */
    goto L_013FD5;  /* 013E44 JMP */
L_013E47: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 013E47 LDA */
    /* 013E4A CMP */
    if ((A & 0xFF) != 0x08) goto L_013E51;  /* 013E4C BNE */
    goto L_013FD5;  /* 013E4E JMP */
L_013E51: TICK();
    SETLO(A, rd8(ABS(0x01A4)));  /* 013E51 LDA */
    SETLO(A, A & 0xFC);  /* 013E54 AND */
    /* 013E56 CMP */
    if ((A & 0xFF) == 0x48) goto L_013E5D;  /* 013E58 BEQ */
    goto L_013FCB;  /* 013E5A JMP */
L_013E5D: TICK();
    if ((rd8(ABS(0x0119)) & 0x80) == 0x80) goto L_013E65;  /* 013E5D BBS */
    goto L_013F00;  /* 013E62 JMP */
L_013E65: TICK();
    SETLO(A, rd8(ABS(0x0119)));  /* 013E65 LDA */
    SETLO(A, A & 0xE0);  /* 013E68 AND */
    /* 013E6A CMP */
    if ((A & 0xFF) != 0xC0) goto L_013E71;  /* 013E6C BNE */
    goto L_013F00;  /* 013E6E JMP */
L_013E71: TICK();
    Y = rd16(ABS(0x0119));  /* 013E71 LDY */
    mflag = 0;  /* 013E74 CLM */
    A = D;  /* 013E75 TDA */
    A += 1;  /* 013E76 INC */
    A += 1;  /* 013E77 INC */
    push16(A);  /* 013E78 PHA */
    S -= 3; F_014D31();  /* 013E79 JSRL */
    Y = pull16();  /* 013E7D PLY */
    mflag = 0;  /* 013E7E CLM */
    A &= 0x00FF;  /* 013E7F AND */
    wr16(DP(0x00), A);  /* 013E82 STA */
    X = rd16(ABS(0x017F));  /* 013E84 LDX */
    if (X != 0) goto L_013EC5;  /* 013E87 BNE */
    wr16(DP(0x02), X);  /* 013E89 STX */
    mflag = 1;  /* 013E8B SEM */
    SETLO(B, X);  /* 013E8C TXB */
    SETLO(A, rd8(ABS(0x011D)));  /* 013E8E LDA */
    div8(0x10);  /* 013E91 DIV */
    mflag = 0;  /* 013E94 CLM */
    A &= 0x00FF;  /* 013E95 AND */
    wr16(DP(0x03), A);  /* 013E98 STA */
    A = rd16(ABS(0x011C));  /* 013E9A LDA */
    A &= 0x00FF;  /* 013E9D AND */
    A = asl16(A);  /* 013EA0 ASL */
    A = asl16(A);  /* 013EA1 ASL */
    A = asl16(A);  /* 013EA2 ASL */
    A = asl16(A);  /* 013EA3 ASL */
    C = 0;  /* 013EA4 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 013EA5 ADC */
    wr16(DP(0x03), A);  /* 013EA7 STA */
    A = rd16(ABS(0x011B));  /* 013EA9 LDA */
    mflag = 1;  /* 013EAC SEM */
    SETLO(A, A & 0x7F);  /* 013EAD AND */
    wr8(DP(0x05), (uint8_t)A);  /* 013EAF STA */
    SETLO(A, rd8(DP(0x00)));  /* 013EB1 LDA */
    wr8(DP(0x06), (uint8_t)A);  /* 013EB3 STA */
    Y = 0x0080;  /* 013EB5 LDY */
    mflag = 0;  /* 013EB8 CLM */
    A = D;  /* 013EB9 TDA */
    A += 1;  /* 013EBA INC */
    A += 1;  /* 013EBB INC */
    push16(A);  /* 013EBC PHA */
    S -= 3; F_01501B();  /* 013EBD JSRL */
    Y = pull16();  /* 013EC1 PLY */
    goto L_013ED1;  /* 013EC2 JMP */
L_013EC5: TICK();
    Y = rd16(DP(0x00));  /* 013EC5 LDY */
    mflag = 0;  /* 013EC7 CLM */
    A = D;  /* 013EC8 TDA */
    A += 1;  /* 013EC9 INC */
    A += 1;  /* 013ECA INC */
    push16(A);  /* 013ECB PHA */
    S -= 3; F_014D6F();  /* 013ECC JSRL */
    Y = pull16();  /* 013ED0 PLY */
L_013ED1: TICK();
    Y = 0x0013;  /* 013ED1 LDY */
    A = D;  /* 013ED4 TDA */
    A += 1;  /* 013ED5 INC */
    A += 1;  /* 013ED6 INC */
    push16(A);  /* 013ED7 PHA */
    S -= 3; F_0080AA();  /* 013ED8 JSRL */
    Y = pull16();  /* 013EDC PLY */
    Y = rd16(ABS(0x0119));  /* 013EDD LDY */
    A = D;  /* 013EE0 TDA */
    A += 1;  /* 013EE1 INC */
    A += 1;  /* 013EE2 INC */
    push16(A);  /* 013EE3 PHA */
    S -= 3; F_014D50();  /* 013EE4 JSRL */
    Y = pull16();  /* 013EE8 PLY */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 013EE9 SEB */
    mflag = 1;  /* 013EEE SEM */
    wr8(ABS(0x0232), 0x3F);  /* 013EEF LDM */
    mflag = 0;  /* 013EF3 CLM */
    A = D;  /* 013EF4 TDA */
    A += 1;  /* 013EF5 INC */
    A += 1;  /* 013EF6 INC */
    push16(A);  /* 013EF7 PHA */
    S -= 3; F_007A06();  /* 013EF8 JSRL */
    Y = pull16();  /* 013EFC PLY */
    goto L_013FCB;  /* 013EFD JMP */
L_013F00: TICK();
    mflag = 0;  /* 013F00 CLM */
    A = rd16(ABS(0x0119));  /* 013F01 LDA */
    mflag = 1;  /* 013F04 SEM */
    SETLO(A, A & 0xFC);  /* 013F05 AND */
    /* 013F07 CMP */
    if ((A & 0xFF) == 0x64) goto L_013F0E;  /* 013F09 BEQ */
    goto L_013FCB;  /* 013F0B JMP */
L_013F0E: TICK();
    mflag = 0;  /* 013F0E CLM */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0x0020) goto L_013F18;  /* 013F0F BBS */
    goto L_013FCB;  /* 013F15 JMP */
L_013F18: TICK();
    A = rd16(ABS(0x0121));  /* 013F18 LDA */
    mflag = 1;  /* 013F1B SEM */
    SETLO(A, A & 0x70);  /* 013F1C AND */
    SETLO(B, 0x00);  /* 013F1E LDA */
    div8(0x10);  /* 013F21 DIV */
    /* 013F24 CMP */
    if ((A & 0xFF) == 0x00) goto L_013F2B;  /* 013F26 BEQ */
    goto L_013FB3;  /* 013F28 JMP */
L_013F2B: TICK();
    SETLO(B, 0x00);  /* 013F2B LDA */
    SETLO(A, rd8(ABS(0x0120)));  /* 013F2E LDA */
    div8(0x40);  /* 013F31 DIV */
    mflag = 0;  /* 013F34 CLM */
    A &= 0x00FF;  /* 013F35 AND */
    wr16(DP(0x02), A);  /* 013F38 STA */
    A = rd16(ABS(0x011F));  /* 013F3A LDA */
    mflag = 1;  /* 013F3D SEM */
    SETLO(A, A & 0x0F);  /* 013F3E AND */
    mflag = 0;  /* 013F40 CLM */
    A &= 0x00FF;  /* 013F41 AND */
    A = asl16(A);  /* 013F44 ASL */
    A = asl16(A);  /* 013F45 ASL */
    C = 0;  /* 013F46 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 013F47 ADC */
    C = 0;  /* 013F49 CLC */
    A = adc16(A, 0x0007);  /* 013F4A ADC */
    B = 0x0008;  /* 013F4D LDA */
    S -= 3; F_00FF8C();  /* 013F51 JSRL */
    mflag = 1;  /* 013F55 SEM */
    SETLO(B, 0x00);  /* 013F56 LDA */
    mflag = 0;  /* 013F59 CLM */
    wr16(DP(0x02), A);  /* 013F5A STA */
    A = rd16(ABS(0x011F));  /* 013F5C LDA */
    mflag = 1;  /* 013F5F SEM */
    div8(0x10);  /* 013F60 DIV */
    mpy8(0x06);  /* 013F63 MPY */
    wr8(DP(0x04), (uint8_t)A);  /* 013F66 STA */
    wr8(DP(0x05), (uint8_t)B);  /* 013F68 STA */
    mflag = 0;  /* 013F6B CLM */
    A = rd16(DP(0x02));  /* 013F6C LDA */
    C = 0;  /* 013F6E CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 013F6F ADC */
    C = 0;  /* 013F71 CLC */
    A = adc16(A, 0xFFFD);  /* 013F72 ADC */
    wr16(DP(0x00), A);  /* 013F75 STA */
    wr16(DP(0x02), A);  /* 013F77 STA */
    mflag = 1;  /* 013F79 SEM */
    SETLO(B, 0x00);  /* 013F7A LDA */
    SETLO(A, rd8(ABS(0x011D)));  /* 013F7D LDA */
    div8(0x10);  /* 013F80 DIV */
    mflag = 0;  /* 013F83 CLM */
    A &= 0x00FF;  /* 013F84 AND */
    wr16(DP(0x03), A);  /* 013F87 STA */
    A = rd16(ABS(0x011C));  /* 013F89 LDA */
    A &= 0x00FF;  /* 013F8C AND */
    A = asl16(A);  /* 013F8F ASL */
    A = asl16(A);  /* 013F90 ASL */
    A = asl16(A);  /* 013F91 ASL */
    A = asl16(A);  /* 013F92 ASL */
    C = 0;  /* 013F93 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 013F94 ADC */
    wr16(DP(0x03), A);  /* 013F96 STA */
    A = rd16(ABS(0x011B));  /* 013F98 LDA */
    mflag = 1;  /* 013F9B SEM */
    SETLO(A, A & 0x7F);  /* 013F9C AND */
    wr8(DP(0x05), (uint8_t)A);  /* 013F9E STA */
    wr8(DP(0x06), 0xB7);  /* 013FA0 LDM */
    Y = 0x0082;  /* 013FA3 LDY */
    mflag = 0;  /* 013FA6 CLM */
    A = D;  /* 013FA7 TDA */
    A += 1;  /* 013FA8 INC */
    A += 1;  /* 013FA9 INC */
    push16(A);  /* 013FAA PHA */
    S -= 3; F_01501B();  /* 013FAB JSRL */
    Y = pull16();  /* 013FAF PLY */
    goto L_013FCB;  /* 013FB0 JMP */
L_013FB3: TICK();
    mflag = 1;  /* 013FB3 SEM */
    /* 013FB4 CMP */
    if ((A & 0xFF) != 0x01) goto L_013FCB;  /* 013FB6 BNE */
    SETLO(B, 0x00);  /* 013FB8 LDA */
    Y = 0x0122;  /* 013FBB LDY */
    mflag = 0;  /* 013FBE CLM */
    A = D;  /* 013FBF TDA */
    A += 1;  /* 013FC0 INC */
    A += 1;  /* 013FC1 INC */
    push16(A);  /* 013FC2 PHA */
    S -= 3; F_014329();  /* 013FC3 JSRL */
    Y = pull16();  /* 013FC7 PLY */
    goto L_013FCB;  /* 013FC8 JMP */
L_013FCB: TICK();
    mflag = 0;  /* 013FCB CLM */
    A = D;  /* 013FCC TDA */
    A += 1;  /* 013FCD INC */
    A += 1;  /* 013FCE INC */
    push16(A);  /* 013FCF PHA */
    S -= 3; F_014031();  /* 013FD0 JSRL */
    Y = pull16();  /* 013FD4 PLY */
L_013FD5: TICK();
    mflag = 0;  /* 013FD5 CLM */
    Y = pull16();  /* 013FD6 PLY */
    D = pull16();  /* 013FD7 PLD */
    S += 3; return;  /* 013FD8 RTL */
}

/* 013FD9  callers: 013707, 013844, 01390E, 013B00, 013C9C, 013D4F, 014173, 014F1A ... */
void F_013FD9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 013FD9 LDA */
    push16(D);  /* 013FDB PHD */
    D = A;  /* 013FDC TAD */
    wr16(DP(0x04), Y);  /* 013FDD STY */
    wr16(DP(0x06), B);  /* 013FDF STA */
    mflag = 1;  /* 013FE2 SEM */
    wr8(DP(0x07), 0x00);  /* 013FE3 LDM */
L_013FE6: TICK();
    SETLO(A, rd8(DP(0x07)));  /* 013FE6 LDA */
    t = rd8(DP(0x00));  /* 013FE8 CMP */
    if ((A & 0xFF) >= t) goto L_013FFB;  /* 013FEA BCS */
    SETLO(A, rd8(LIND(0x04)));  /* 013FEC LDA */
    wr8(LIND(0x01), (uint8_t)A);  /* 013FEE STA */
    mflag = 0;  /* 013FF0 CLM */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 013FF1 INC */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 013FF3 INC */
    mflag = 1;  /* 013FF5 SEM */
    t = (rd8(DP(0x07)) + 1) & 0xFF; wr8(DP(0x07), t);  /* 013FF6 INC */
    goto L_013FE6;  /* 013FF8 JMP */
L_013FFB: TICK();
    mflag = 0;  /* 013FFB CLM */
    D = pull16();  /* 013FFC PLD */
    S += 3; return;  /* 013FFD RTL */
}

/* 013FFE  callers: 013ADD, 013C79, 013D2C */
void F_013FFE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 013FFE LDA */
    push16(D);  /* 014000 PHD */
    D = A;  /* 014001 TAD */
    wr16(DP(0x04), Y);  /* 014002 STY */
    wr16(DP(0x06), B);  /* 014004 STA */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0x0020) goto L_01402E;  /* 014007 BBS */
    mflag = 1;  /* 01400D SEM */
    wr8(DP(0x07), 0x00);  /* 01400E LDM */
L_014011: TICK();
    SETLO(A, rd8(DP(0x07)));  /* 014011 LDA */
    t = rd8(DP(0x00));  /* 014013 CMP */
    if ((A & 0xFF) >= t) goto L_01402E;  /* 014015 BCS */
    SETLO(A, rd8(LIND(0x04)));  /* 014017 LDA */
    t = rd8(LIND(0x01));  /* 014019 CMP */
    if ((A & 0xFF) == t) goto L_014023;  /* 01401B BEQ */
    mflag = 0;  /* 01401D CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0020);  /* 01401E SEB */
L_014023: TICK();
    mflag = 0;  /* 014023 CLM */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 014024 INC */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 014026 INC */
    mflag = 1;  /* 014028 SEM */
    t = (rd8(DP(0x07)) + 1) & 0xFF; wr8(DP(0x07), t);  /* 014029 INC */
    goto L_014011;  /* 01402B JMP */
L_01402E: TICK();
    mflag = 0;  /* 01402E CLM */
    D = pull16();  /* 01402F PLD */
    S += 3; return;  /* 014030 RTL */
}

/* 014031  callers: 013602, 013755, 0137D7, 0138A6, 013A10, 013C18, 013C25, 013E13 ... */
void F_014031(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014031 LDA */
    push16(D);  /* 014033 PHD */
    D = A;  /* 014034 TAD */
    C = 0;  /* 014035 CLC */
    A = adc16(A, 0x0004);  /* 014036 ADC */
    push16(A);  /* 014039 PHA */
    mflag = 1;  /* 01403A SEM */
    wr8(ABS(0x0118), 0x03);  /* 01403B LDM */
    mflag = 0;  /* 01403F CLM */
    wr16(DP(0x00), 0x001A);  /* 014040 LDM */
    wr16(DP(0x02), 0x0040);  /* 014044 LDM */
    Y = 0x0002;  /* 014048 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 01404B JSRL */
    wr8(ABS(0x01AA), 0x00);  /* 01404F LDM */
    mflag = 0;  /* 014053 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 014054 CLB */
    mflag = 1;  /* 014059 SEM */
    wr8(ABS(0x04E6), 0x00);  /* 01405A LDM */
    mflag = 0;  /* 01405E CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x1000);  /* 01405F CLB */
    mflag = 1;  /* 014064 SEM */
    wr8(ABS(0x019D), 0x00);  /* 014065 LDM */
    mflag = 0;  /* 014069 CLM */
    Y = pull16();  /* 01406A PLY */
    D = pull16();  /* 01406B PLD */
    S += 3; return;  /* 01406C RTL */
}

/* 01406D  callers: 0138D4, 013A88 */
void F_01406D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01406D LDA */
    push16(D);  /* 01406F PHD */
    D = A;  /* 014070 TAD */
    A += 1;  /* 014071 INC */
    A += 1;  /* 014072 INC */
    push16(A);  /* 014073 PHA */
    A = rd16(ABS(0x0121));  /* 014074 LDA */
    mflag = 1;  /* 014077 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 014078 STA */
    SETLO(A, A & 0x03);  /* 01407A AND */
    mflag = 0;  /* 01407C CLM */
    A &= 0x00FF;  /* 01407D AND */
    A += 1;  /* 014080 INC */
    B = 0x0004;  /* 014081 LDA */
    S -= 3; F_01E350();  /* 014085 JSRL */
    mflag = 1;  /* 014089 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 01408A STA */
    SETLO(A, rd8(DP(0x01)));  /* 01408C LDA */
    SETLO(A, A & 0xFC);  /* 01408E AND */
    C = 0;  /* 014090 CLC */
    SETLO(A, adc8(A, rd8(DP(0x00))));  /* 014091 ADC */
    wr8(ABS(0x0121), (uint8_t)A);  /* 014093 STA */
    wr8(ABS(0x019A), asl8(rd8(ABS(0x019A))));  /* 014096 ASL */
    SETLO(A, rd8(ABS(0x019A)));  /* 014099 LDA */
    /* 01409C CMP */
    if ((A & 0xFF) != 0x40) goto L_0140DA;  /* 01409E BNE */
    wr8(DP(0x02), 0x00);  /* 0140A0 LDM */
    SETLO(A, rd8(ABS(0x011D)));  /* 0140A3 LDA */
    SETLO(A, A & 0x0F);  /* 0140A6 AND */
    mflag = 0;  /* 0140A8 CLM */
    A &= 0x00FF;  /* 0140A9 AND */
    A = rla16(A, 0x0008);  /* 0140AC RLA */
    A &= 0xFF00;  /* 0140B0 AND */
    wr16(DP(0x03), A);  /* 0140B3 STA */
    A = rd16(ABS(0x011E));  /* 0140B5 LDA */
    A &= 0x00FF;  /* 0140B8 AND */
    C = 0;  /* 0140BB CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 0140BC ADC */
    wr16(DP(0x03), A);  /* 0140BE STA */
    A = rd16(ABS(0x011B));  /* 0140C0 LDA */
    mflag = 1;  /* 0140C3 SEM */
    SETLO(A, A & 0x7F);  /* 0140C4 AND */
    wr8(DP(0x05), (uint8_t)A);  /* 0140C6 STA */
    wr8(DP(0x06), 0x04);  /* 0140C8 LDM */
    Y = 0x00E0;  /* 0140CB LDY */
    mflag = 0;  /* 0140CE CLM */
    S -= 3; F_01501B();  /* 0140CF JSRL */
    S -= 3; F_014031();  /* 0140D3 JSRL */
    goto L_014140;  /* 0140D7 JMP */
L_0140DA: TICK();
    mflag = 0;  /* 0140DA CLM */
    wr16(DP(0x02), 0x0000);  /* 0140DB LDM */
    A = rd16(ABS(0x011D));  /* 0140DF LDA */
    mflag = 1;  /* 0140E2 SEM */
    SETLO(A, A & 0x0F);  /* 0140E3 AND */
    mflag = 0;  /* 0140E5 CLM */
    A &= 0x00FF;  /* 0140E6 AND */
    A = rla16(A, 0x0008);  /* 0140E9 RLA */
    A &= 0xFF00;  /* 0140ED AND */
    wr16(DP(0x03), A);  /* 0140F0 STA */
    A = rd16(ABS(0x011E));  /* 0140F2 LDA */
    A &= 0x00FF;  /* 0140F5 AND */
    C = 0;  /* 0140F8 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 0140F9 ADC */
    wr16(DP(0x03), A);  /* 0140FB STA */
    A = rd16(ABS(0x011B));  /* 0140FD LDA */
    mflag = 1;  /* 014100 SEM */
    SETLO(A, A & 0x7F);  /* 014101 AND */
    wr8(DP(0x05), (uint8_t)A);  /* 014103 STA */
    wr8(DP(0x06), 0x02);  /* 014105 LDM */
    Y = 0x00D0;  /* 014108 LDY */
    mflag = 0;  /* 01410B CLM */
    S -= 3; F_01501B();  /* 01410C JSRL */
    mflag = 1;  /* 014110 SEM */
    wr8(ABS(0x0118), 0x05);  /* 014111 LDM */
    SETLO(B, 0x00);  /* 014115 LDA */
    SETLO(A, rd8(ABS(0x0457)));  /* 014118 LDA */
    div8(rd8(ABS(0x019A)));  /* 01411B DIV */
    mflag = 0;  /* 01411F CLM */
    push16(B);  /* 014120 PHB */
    A = pull16();  /* 014122 PLA */
    A &= 0x00FF;  /* 014123 AND */
    A += 1;  /* 014126 INC */
    mpy16(0x0032);  /* 014127 MPY */
    C = 0;  /* 01412B CLC */
    A = adc16(A, 0x07D0);  /* 01412C ADC */
    B = 0x0020;  /* 01412F LDA */
    S -= 3; F_00FF8C();  /* 014133 JSRL */
    mflag = 1;  /* 014137 SEM */
    SETLO(A, A + 1);  /* 014138 INC */
    wr8(ABS(0x03A0), (uint8_t)A);  /* 014139 STA */
    wr8(ABS(0x041E), 0x1E);  /* 01413C LDM */
L_014140: TICK();
    mflag = 0;  /* 014140 CLM */
    Y = pull16();  /* 014141 PLY */
    D = pull16();  /* 014142 PLD */
    S += 3; return;  /* 014143 RTL */
}

/* 014144  callers: 013C06, 013CC8, 013CE2, 013D68, 013D7A */
void F_014144(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014144 LDA */
    push16(D);  /* 014146 PHD */
    D = A;  /* 014147 TAD */
    C = 0;  /* 014148 CLC */
    A = adc16(A, 0x0006);  /* 014149 ADC */
    push16(A);  /* 01414C PHA */
    wr16(DP(0x02), 0x0004);  /* 01414D LDM */
    wr16(DP(0x04), 0x0040);  /* 014151 LDM */
    Y = 0x0002;  /* 014155 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 014158 JSRL */
    wr8(DP(0x02), 0x06);  /* 01415C LDM */
    mflag = 0;  /* 01415F CLM */
    wr16(DP(0x03), 0x01A4);  /* 014160 LDM */
    wr16(DP(0x05), 0x0000);  /* 014164 LDM */
    B = 0x0000;  /* 014168 LDA */
    Y = 0x0119;  /* 01416C LDY */
    A = D;  /* 01416F TDA */
    A += 1;  /* 014170 INC */
    A += 1;  /* 014171 INC */
    push16(A);  /* 014172 PHA */
    S -= 3; F_013FD9();  /* 014173 JSRL */
    Y = pull16();  /* 014177 PLY */
    if ((rd16(ABS(0x0246)) & 0x0400) == 0) goto L_01418C;  /* 014178 BBC */
    A = rd16(ABS(0x01A4));  /* 01417E LDA */
    mflag = 1;  /* 014181 SEM */
    SETLO(A, A & 0x03);  /* 014182 AND */
    SETLO(A, A | 0x44);  /* 014184 ORA */
    wr8(ABS(0x01A4), (uint8_t)A);  /* 014186 STA */
    goto L_014198;  /* 014189 JMP */
L_01418C: TICK();
    mflag = 0;  /* 01418C CLM */
    A = rd16(ABS(0x01A4));  /* 01418D LDA */
    mflag = 1;  /* 014190 SEM */
    SETLO(A, A & 0x03);  /* 014191 AND */
    SETLO(A, A | 0x48);  /* 014193 ORA */
    wr8(ABS(0x01A4), (uint8_t)A);  /* 014195 STA */
L_014198: TICK();
    SETLO(A, rd8(ABS(0x011D)));  /* 014198 LDA */
    SETLO(A, A & 0x0F);  /* 01419B AND */
    mflag = 0;  /* 01419D CLM */
    A &= 0x00FF;  /* 01419E AND */
    A = rla16(A, 0x0008);  /* 0141A1 RLA */
    A &= 0xFF00;  /* 0141A5 AND */
    wr16(DP(0x02), A);  /* 0141A8 STA */
    A = rd16(ABS(0x011E));  /* 0141AA LDA */
    A &= 0x00FF;  /* 0141AD AND */
    C = 0;  /* 0141B0 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 0141B1 ADC */
    /* 0141B3 CMP */
    if (A != 0x0FFF) goto L_0141C6;  /* 0141B6 BNE */
    if ((rd16(ABS(0x0246)) & 0x0400) == 0x0400) goto L_0141C6;  /* 0141B8 BBS */
    mflag = 1;  /* 0141BE SEM */
    wr8(ABS(0x0118), 0x0D);  /* 0141BF LDM */
    goto L_01423B;  /* 0141C3 JMP */
L_0141C6: TICK();
    X = 0x01A4;  /* 0141C6 LDX */
    mflag = 0;  /* 0141C9 CLM */
    A = rd16((ABS(0x0005) + X));  /* 0141CA LDA */
    mflag = 1;  /* 0141CD SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 0141CE STA */
    SETLO(A, rd8((ABS(0x0004) + X)));  /* 0141D0 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 0141D3 STA */
    SETLO(A, rd8((ABS(0x0003) + X)));  /* 0141D5 LDA */
    SETLO(A, A & 0x0F);  /* 0141D8 AND */
    SETLO(A, asl8(A));  /* 0141DA ASL */
    SETLO(A, asl8(A));  /* 0141DB ASL */
    SETLO(A, asl8(A));  /* 0141DC ASL */
    SETLO(A, asl8(A));  /* 0141DD ASL */
    SETLO(B, 0x00);  /* 0141DE LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 0141E1 STA */
    SETLO(A, rd8(DP(0x01)));  /* 0141E3 LDA */
    div8(0x10);  /* 0141E5 DIV */
    C = 0;  /* 0141E8 CLC */
    SETLO(A, adc8(A, rd8(DP(0x02))));  /* 0141E9 ADC */
    wr8(ABS(0x01A9), (uint8_t)A);  /* 0141EB STA */
    SETLO(B, 0x00);  /* 0141EE LDA */
    SETLO(A, rd8((ABS(0x0003) + X)));  /* 0141F1 LDA */
    div8(0x10);  /* 0141F4 DIV */
    wr8(DP(0x02), (uint8_t)A);  /* 0141F7 STA */
    SETLO(A, rd8(DP(0x00)));  /* 0141F9 LDA */
    SETLO(A, A & 0x0F);  /* 0141FB AND */
    SETLO(A, asl8(A));  /* 0141FD ASL */
    SETLO(A, asl8(A));  /* 0141FE ASL */
    SETLO(A, asl8(A));  /* 0141FF ASL */
    SETLO(A, asl8(A));  /* 014200 ASL */
    C = 0;  /* 014201 CLC */
    SETLO(A, adc8(A, rd8(DP(0x02))));  /* 014202 ADC */
    wr8(ABS(0x01A8), (uint8_t)A);  /* 014204 STA */
    SETLO(A, rd8(DP(0x01)));  /* 014207 LDA */
    SETLO(A, A & 0x0F);  /* 014209 AND */
    SETLO(A, asl8(A));  /* 01420B ASL */
    SETLO(A, asl8(A));  /* 01420C ASL */
    SETLO(A, asl8(A));  /* 01420D ASL */
    SETLO(A, asl8(A));  /* 01420E ASL */
    SETLO(B, 0x00);  /* 01420F LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 014212 STA */
    SETLO(A, rd8(DP(0x00)));  /* 014214 LDA */
    div8(0x10);  /* 014216 DIV */
    C = 0;  /* 014219 CLC */
    SETLO(A, adc8(A, rd8(DP(0x02))));  /* 01421A ADC */
    wr8(ABS(0x01A7), (uint8_t)A);  /* 01421C STA */
    Y = 0x0607;  /* 01421F LDY */
    mflag = 0;  /* 014222 CLM */
    A = D;  /* 014223 TDA */
    A += 1;  /* 014224 INC */
    A += 1;  /* 014225 INC */
    push16(A);  /* 014226 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 014227 JSRL */
    Y = pull16();  /* 01422B PLY */
    wr8(ABS(0x019B), (uint8_t)A);  /* 01422C STA */
    wr8(ABS(0x0118), 0x0C);  /* 01422F LDM */
    wr8(ABS(0x03A0), 0x20);  /* 014233 LDM */
    wr8(ABS(0x041E), 0x02);  /* 014237 LDM */
L_01423B: TICK();
    mflag = 0;  /* 01423B CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0400);  /* 01423C CLB */
    Y = pull16();  /* 014241 PLY */
    D = pull16();  /* 014242 PLD */
    S += 3; return;  /* 014243 RTL */
}

/* 014244  callers: 01C9A2 */
void F_014244(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014244 LDA */
    push16(D);  /* 014246 PHD */
    D = A;  /* 014247 TAD */
    C = 0;  /* 014248 CLC */
    A = adc16(A, 0x0006);  /* 014249 ADC */
    push16(A);  /* 01424C PHA */
    mflag = 1;  /* 01424D SEM */
    SETLO(A, rd8(ABS(0x0118)));  /* 01424E LDA */
    if ((A & 0xFF) != 0) goto L_014256;  /* 014251 BNE */
    goto L_014325;  /* 014253 JMP */
L_014256: TICK();
    mflag = 0;  /* 014256 CLM */
    A = rd16(DP(0x00));  /* 014257 LDA */
    wr16(ABS(0x019E), A);  /* 014259 STA */
    if ((rd16(ABS(0x019E)) & 0x0004) == 0) goto L_0142CC;  /* 01425C BBC */
    mflag = 1;  /* 014262 SEM */
    SETLO(A, rd8(ABS(0x019B)));  /* 014263 LDA */
    if ((A & 0xFF) == 0) goto L_01427E;  /* 014266 BEQ */
    t = (rd8(ABS(0x019B)) - 1) & 0xFF; wr8(ABS(0x019B), t);  /* 014268 DEC */
    mflag = 0;  /* 01426B CLM */
    wr16(DP(0x02), 0x00AA);  /* 01426C LDM */
    wr16(DP(0x04), 0x0043);  /* 014270 LDM */
    Y = 0x0002;  /* 014274 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 014277 JSRL */
    goto L_0142CC;  /* 01427B JMP */
L_01427E: TICK();
    mflag = 0;  /* 01427E CLM */
    A = rd16(ABS(0x01AA));  /* 01427F LDA */
    mflag = 1;  /* 014282 SEM */
    /* 014283 CMP */
    if ((A & 0xFF) >= 0x08) goto L_0142A5;  /* 014285 BCS */
    SETLO(A, rd8(ABS(0x01AA)));  /* 014287 LDA */
    t = (rd8(ABS(0x01AA)) + 1) & 0xFF; wr8(ABS(0x01AA), t);  /* 01428A INC */
    mflag = 0;  /* 01428D CLM */
    A &= 0x00FF;  /* 01428E AND */
    X = A;  /* 014291 TAX */
    A = rd16((ABS(0x01A2) + X));  /* 014292 LDA */
    A &= 0x00FF;  /* 014295 AND */
    wr16(DP(0x02), A);  /* 014298 STA */
    wr16(DP(0x04), 0x0043);  /* 01429A LDM */
    Y = 0x0002;  /* 01429E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0142A1 JSRL */
L_0142A5: TICK();
    SETLO(A, rd8(ABS(0x01AA)));  /* 0142A5 LDA */
    /* 0142A8 CMP */
    if ((A & 0xFF) != 0x08) goto L_0142B3;  /* 0142AA BNE */
    wr8(ABS(0x019D), 0x01);  /* 0142AC LDM */
    goto L_0142CC;  /* 0142B0 JMP */
L_0142B3: TICK();
    mflag = 0;  /* 0142B3 CLM */
    A = rd16(ABS(0x01AA));  /* 0142B4 LDA */
    mflag = 1;  /* 0142B7 SEM */
    /* 0142B8 CMP */
    if ((A & 0xFF) != 0x03) goto L_0142CC;  /* 0142BA BNE */
    mflag = 0;  /* 0142BC CLM */
    wr16(DP(0x02), 0x000B);  /* 0142BD LDM */
    wr16(DP(0x04), 0x0040);  /* 0142C1 LDM */
    Y = 0x0002;  /* 0142C5 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0142C8 JSRL */
L_0142CC: TICK();
    mflag = 0;  /* 0142CC CLM */
    if ((rd16(ABS(0x019E)) & 0x0008) == 0) goto L_0142D8;  /* 0142CD BBC */
    mflag = 1;  /* 0142D3 SEM */
    wr8(ABS(0x019D), 0x01);  /* 0142D4 LDM */
L_0142D8: TICK();
    mflag = 0;  /* 0142D8 CLM */
    if ((rd16(ABS(0x019E)) & 0x0010) == 0) goto L_0142E4;  /* 0142D9 BBC */
    mflag = 1;  /* 0142DF SEM */
    wr8(ABS(0x019D), 0x01);  /* 0142E0 LDM */
L_0142E4: TICK();
    mflag = 0;  /* 0142E4 CLM */
    if ((rd16(ABS(0x019E)) & 0x0001) == 0) goto L_014325;  /* 0142E5 BBC */
    A = rd16(ABS(0x01AA));  /* 0142EB LDA */
    mflag = 1;  /* 0142EE SEM */
    /* 0142EF CMP */
    if ((A & 0xFF) >= 0x06) goto L_014317;  /* 0142F1 BCS */
    mflag = 0;  /* 0142F3 CLM */
    wr16(DP(0x02), 0x0000);  /* 0142F4 LDM */
    wr16(DP(0x04), 0x0042);  /* 0142F8 LDM */
    Y = 0x0002;  /* 0142FC LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0142FF JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 014303 STA */
    SETLO(A, rd8(ABS(0x01AA)));  /* 014305 LDA */
    mflag = 0;  /* 014308 CLM */
    A &= 0x00FF;  /* 014309 AND */
    C = 0;  /* 01430C CLC */
    A = adc16(A, 0x01A4);  /* 01430D ADC */
    X = A;  /* 014310 TAX */
    A = rd16(DP(0x02));  /* 014311 LDA */
    mflag = 1;  /* 014313 SEM */
    wr8((ABS(0x0000) + X), (uint8_t)A);  /* 014314 STA */
L_014317: TICK();
    t = (rd8(ABS(0x01AA)) + 1) & 0xFF; wr8(ABS(0x01AA), t);  /* 014317 INC */
    SETLO(A, rd8(ABS(0x01AA)));  /* 01431A LDA */
    /* 01431D CMP */
    if ((A & 0xFF) != 0x08) goto L_014325;  /* 01431F BNE */
    wr8(ABS(0x019D), 0x01);  /* 014321 LDM */
L_014325: TICK();
    mflag = 0;  /* 014325 CLM */
    Y = pull16();  /* 014326 PLY */
    D = pull16();  /* 014327 PLD */
    S += 3; return;  /* 014328 RTL */
}

/* 014329  callers: 013FC3 */
void F_014329(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014329 LDA */
    push16(D);  /* 01432B PHD */
    D = A;  /* 01432C TAD */
    C = 0;  /* 01432D CLC */
    A = adc16(A, 0x0005);  /* 01432E ADC */
    push16(A);  /* 014331 PHA */
    wr16(DP(0x00), Y);  /* 014332 STY */
    mflag = 1;  /* 014334 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 014335 STA */
    mflag = 0;  /* 014338 CLM */
    S -= 3; F_0077F0();  /* 014339 JSRL */
    wr16(DP(0x05), 0x0007);  /* 01433D LDM */
    mflag = 1;  /* 014341 SEM */
    SETLO(B, 0x00);  /* 014342 LDA */
    Y = 0x2040;  /* 014345 LDY */
    mflag = 0;  /* 014348 CLM */
    S -= 3; F_014855();  /* 014349 JSRL */
    wr16(DP(0x05), 0x0003);  /* 01434D LDM */
    A = rd16(DP(0x00));  /* 014351 LDA */
    A += 1;  /* 014353 INC */
    B = rd16(DP(0x02));  /* 014354 LDA */
    Y = A;  /* 014357 TAY */
    S -= 3; F_0149CE();  /* 014358 JSRL */
    mflag = 0;  /* 01435C CLM */
    Y = A;  /* 01435D TAY */
    S -= 3; F_007627();  /* 01435E JSRL */
    wr16(DP(0x05), 0x0004);  /* 014362 LDM */
    A = rd16(DP(0x00));  /* 014366 LDA */
    A += 1;  /* 014368 INC */
    A += 1;  /* 014369 INC */
    B = rd16(DP(0x02));  /* 01436A LDA */
    Y = A;  /* 01436D TAY */
    S -= 3; F_0149CE();  /* 01436E JSRL */
    mflag = 0;  /* 014372 CLM */
    Y = A;  /* 014373 TAY */
    S -= 3; F_007627();  /* 014374 JSRL */
    wr16(DP(0x05), 0x0005);  /* 014378 LDM */
    A = rd16(DP(0x00));  /* 01437C LDA */
    A += 1;  /* 01437E INC */
    A += 1;  /* 01437F INC */
    A += 1;  /* 014380 INC */
    B = rd16(DP(0x02));  /* 014381 LDA */
    Y = A;  /* 014384 TAY */
    S -= 3; F_0149CE();  /* 014385 JSRL */
    mflag = 0;  /* 014389 CLM */
    Y = A;  /* 01438A TAY */
    S -= 3; F_007627();  /* 01438B JSRL */
    wr16(DP(0x05), 0x0006);  /* 01438F LDM */
    A = rd16(DP(0x00));  /* 014393 LDA */
    C = 0;  /* 014395 CLC */
    A = adc16(A, 0x0004);  /* 014396 ADC */
    B = rd16(DP(0x02));  /* 014399 LDA */
    Y = A;  /* 01439C TAY */
    S -= 3; F_0149CE();  /* 01439D JSRL */
    mflag = 0;  /* 0143A1 CLM */
    Y = A;  /* 0143A2 TAY */
    S -= 3; F_007627();  /* 0143A3 JSRL */
    A = rd16(DP(0x00));  /* 0143A7 LDA */
    C = 0;  /* 0143A9 CLC */
    A = adc16(A, 0x0005);  /* 0143AA ADC */
    B = rd16(DP(0x02));  /* 0143AD LDA */
    Y = A;  /* 0143B0 TAY */
    S -= 3; F_0149FD();  /* 0143B1 JSRL */
    mflag = 0;  /* 0143B5 CLM */
    Y = A;  /* 0143B6 TAY */
    S -= 3; F_007627();  /* 0143B7 JSRL */
    wr16(DP(0x05), 0x0006);  /* 0143BB LDM */
    A = rd16(DP(0x00));  /* 0143BF LDA */
    C = 0;  /* 0143C1 CLC */
    A = adc16(A, 0x0006);  /* 0143C2 ADC */
    B = rd16(DP(0x02));  /* 0143C5 LDA */
    Y = A;  /* 0143C8 TAY */
    S -= 3; F_0149CE();  /* 0143C9 JSRL */
    mflag = 0;  /* 0143CD CLM */
    Y = A;  /* 0143CE TAY */
    S -= 3; F_007627();  /* 0143CF JSRL */
    wr16(DP(0x05), 0x0007);  /* 0143D3 LDM */
    A = rd16(DP(0x00));  /* 0143D7 LDA */
    C = 0;  /* 0143D9 CLC */
    A = adc16(A, 0x0007);  /* 0143DA ADC */
    B = rd16(DP(0x02));  /* 0143DD LDA */
    Y = A;  /* 0143E0 TAY */
    S -= 3; F_0149CE();  /* 0143E1 JSRL */
    mflag = 0;  /* 0143E5 CLM */
    Y = A;  /* 0143E6 TAY */
    S -= 3; F_007627();  /* 0143E7 JSRL */
    wr16(DP(0x05), 0x0000);  /* 0143EB LDM */
    A = rd16(DP(0x00));  /* 0143EF LDA */
    C = 0;  /* 0143F1 CLC */
    A = adc16(A, 0x0007);  /* 0143F2 ADC */
    B = rd16(DP(0x02));  /* 0143F5 LDA */
    Y = A;  /* 0143F8 TAY */
    S -= 3; F_0149CE();  /* 0143F9 JSRL */
    mflag = 0;  /* 0143FD CLM */
    Y = A;  /* 0143FE TAY */
    S -= 3; F_007627();  /* 0143FF JSRL */
    wr16(DP(0x05), 0x0001);  /* 014403 LDM */
    A = rd16(DP(0x00));  /* 014407 LDA */
    C = 0;  /* 014409 CLC */
    A = adc16(A, 0x0008);  /* 01440A ADC */
    B = rd16(DP(0x02));  /* 01440D LDA */
    Y = A;  /* 014410 TAY */
    S -= 3; F_0149CE();  /* 014411 JSRL */
    mflag = 0;  /* 014415 CLM */
    Y = A;  /* 014416 TAY */
    S -= 3; F_007627();  /* 014417 JSRL */
    wr16(DP(0x05), 0x0002);  /* 01441B LDM */
    A = rd16(DP(0x00));  /* 01441F LDA */
    C = 0;  /* 014421 CLC */
    A = adc16(A, 0x0009);  /* 014422 ADC */
    B = rd16(DP(0x02));  /* 014425 LDA */
    Y = A;  /* 014428 TAY */
    S -= 3; F_0149CE();  /* 014429 JSRL */
    mflag = 0;  /* 01442D CLM */
    Y = A;  /* 01442E TAY */
    S -= 3; F_007627();  /* 01442F JSRL */
    wr16(DP(0x05), 0x0003);  /* 014433 LDM */
    A = rd16(DP(0x00));  /* 014437 LDA */
    C = 0;  /* 014439 CLC */
    A = adc16(A, 0x000A);  /* 01443A ADC */
    B = rd16(DP(0x02));  /* 01443D LDA */
    Y = A;  /* 014440 TAY */
    S -= 3; F_0149CE();  /* 014441 JSRL */
    mflag = 0;  /* 014445 CLM */
    Y = A;  /* 014446 TAY */
    S -= 3; F_007627();  /* 014447 JSRL */
    A = rd16(DP(0x00));  /* 01444B LDA */
    C = 0;  /* 01444D CLC */
    A = adc16(A, 0x000B);  /* 01444E ADC */
    B = rd16(DP(0x02));  /* 014451 LDA */
    Y = A;  /* 014454 TAY */
    S -= 3; F_014A20();  /* 014455 JSRL */
    mflag = 0;  /* 014459 CLM */
    Y = A;  /* 01445A TAY */
    S -= 3; F_007627();  /* 01445B JSRL */
    wr16(DP(0x05), 0x0007);  /* 01445F LDM */
    A = rd16(DP(0x00));  /* 014463 LDA */
    C = 0;  /* 014465 CLC */
    A = adc16(A, 0x000D);  /* 014466 ADC */
    B = rd16(DP(0x02));  /* 014469 LDA */
    Y = A;  /* 01446C TAY */
    S -= 3; F_0149CE();  /* 01446D JSRL */
    mflag = 0;  /* 014471 CLM */
    Y = A;  /* 014472 TAY */
    S -= 3; F_007627();  /* 014473 JSRL */
    S -= 3; F_0077F0();  /* 014477 JSRL */
    wr16(DP(0x05), 0x0007);  /* 01447B LDM */
    mflag = 1;  /* 01447F SEM */
    SETLO(B, 0x00);  /* 014480 LDA */
    Y = 0x2048;  /* 014483 LDY */
    mflag = 0;  /* 014486 CLM */
    S -= 3; F_014855();  /* 014487 JSRL */
    wr16(DP(0x05), 0x0000);  /* 01448B LDM */
    A = rd16(DP(0x00));  /* 01448F LDA */
    C = 0;  /* 014491 CLC */
    A = adc16(A, 0x000D);  /* 014492 ADC */
    B = rd16(DP(0x02));  /* 014495 LDA */
    Y = A;  /* 014498 TAY */
    S -= 3; F_0149CE();  /* 014499 JSRL */
    mflag = 0;  /* 01449D CLM */
    Y = A;  /* 01449E TAY */
    S -= 3; F_007627();  /* 01449F JSRL */
    wr16(DP(0x05), 0x0001);  /* 0144A3 LDM */
    A = rd16(DP(0x00));  /* 0144A7 LDA */
    C = 0;  /* 0144A9 CLC */
    A = adc16(A, 0x000E);  /* 0144AA ADC */
    B = rd16(DP(0x02));  /* 0144AD LDA */
    Y = A;  /* 0144B0 TAY */
    S -= 3; F_0149CE();  /* 0144B1 JSRL */
    mflag = 0;  /* 0144B5 CLM */
    Y = A;  /* 0144B6 TAY */
    S -= 3; F_007627();  /* 0144B7 JSRL */
    wr16(DP(0x05), 0x0002);  /* 0144BB LDM */
    A = rd16(DP(0x00));  /* 0144BF LDA */
    C = 0;  /* 0144C1 CLC */
    A = adc16(A, 0x000F);  /* 0144C2 ADC */
    B = rd16(DP(0x02));  /* 0144C5 LDA */
    Y = A;  /* 0144C8 TAY */
    S -= 3; F_0149CE();  /* 0144C9 JSRL */
    mflag = 0;  /* 0144CD CLM */
    Y = A;  /* 0144CE TAY */
    S -= 3; F_007627();  /* 0144CF JSRL */
    wr16(DP(0x05), 0x0003);  /* 0144D3 LDM */
    A = rd16(DP(0x00));  /* 0144D7 LDA */
    C = 0;  /* 0144D9 CLC */
    A = adc16(A, 0x0010);  /* 0144DA ADC */
    B = rd16(DP(0x02));  /* 0144DD LDA */
    Y = A;  /* 0144E0 TAY */
    S -= 3; F_0149CE();  /* 0144E1 JSRL */
    mflag = 0;  /* 0144E5 CLM */
    Y = A;  /* 0144E6 TAY */
    S -= 3; F_007627();  /* 0144E7 JSRL */
    A = rd16(DP(0x00));  /* 0144EB LDA */
    C = 0;  /* 0144ED CLC */
    A = adc16(A, 0x0011);  /* 0144EE ADC */
    B = rd16(DP(0x02));  /* 0144F1 LDA */
    Y = A;  /* 0144F4 TAY */
    S -= 3; F_014A50();  /* 0144F5 JSRL */
    mflag = 0;  /* 0144F9 CLM */
    Y = A;  /* 0144FA TAY */
    S -= 3; F_007627();  /* 0144FB JSRL */
    wr16(DP(0x05), 0x0003);  /* 0144FF LDM */
    A = rd16(DP(0x00));  /* 014503 LDA */
    C = 0;  /* 014505 CLC */
    A = adc16(A, 0x0012);  /* 014506 ADC */
    B = rd16(DP(0x02));  /* 014509 LDA */
    Y = A;  /* 01450C TAY */
    S -= 3; F_0149CE();  /* 01450D JSRL */
    mflag = 0;  /* 014511 CLM */
    Y = A;  /* 014512 TAY */
    S -= 3; F_007627();  /* 014513 JSRL */
    wr16(DP(0x05), 0x0004);  /* 014517 LDM */
    A = rd16(DP(0x00));  /* 01451B LDA */
    C = 0;  /* 01451D CLC */
    A = adc16(A, 0x0013);  /* 01451E ADC */
    B = rd16(DP(0x02));  /* 014521 LDA */
    Y = A;  /* 014524 TAY */
    S -= 3; F_0149CE();  /* 014525 JSRL */
    mflag = 0;  /* 014529 CLM */
    Y = A;  /* 01452A TAY */
    S -= 3; F_007627();  /* 01452B JSRL */
    wr16(DP(0x05), 0x0005);  /* 01452F LDM */
    A = rd16(DP(0x00));  /* 014533 LDA */
    C = 0;  /* 014535 CLC */
    A = adc16(A, 0x0014);  /* 014536 ADC */
    B = rd16(DP(0x02));  /* 014539 LDA */
    Y = A;  /* 01453C TAY */
    S -= 3; F_0149CE();  /* 01453D JSRL */
    mflag = 0;  /* 014541 CLM */
    Y = A;  /* 014542 TAY */
    S -= 3; F_007627();  /* 014543 JSRL */
    wr16(DP(0x05), 0x0006);  /* 014547 LDM */
    A = rd16(DP(0x00));  /* 01454B LDA */
    C = 0;  /* 01454D CLC */
    A = adc16(A, 0x0015);  /* 01454E ADC */
    B = rd16(DP(0x02));  /* 014551 LDA */
    Y = A;  /* 014554 TAY */
    S -= 3; F_0149CE();  /* 014555 JSRL */
    mflag = 0;  /* 014559 CLM */
    Y = A;  /* 01455A TAY */
    S -= 3; F_007627();  /* 01455B JSRL */
    Y = 0x0020;  /* 01455F LDY */
    S -= 3; F_007627();  /* 014562 JSRL */
    wr16(DP(0x05), 0x0007);  /* 014566 LDM */
    A = rd16(DP(0x00));  /* 01456A LDA */
    C = 0;  /* 01456C CLC */
    A = adc16(A, 0x0016);  /* 01456D ADC */
    B = rd16(DP(0x02));  /* 014570 LDA */
    Y = A;  /* 014573 TAY */
    S -= 3; F_0149CE();  /* 014574 JSRL */
    mflag = 0;  /* 014578 CLM */
    Y = A;  /* 014579 TAY */
    S -= 3; F_007627();  /* 01457A JSRL */
    wr16(DP(0x05), 0x0000);  /* 01457E LDM */
    A = rd16(DP(0x00));  /* 014582 LDA */
    C = 0;  /* 014584 CLC */
    A = adc16(A, 0x0016);  /* 014585 ADC */
    B = rd16(DP(0x02));  /* 014588 LDA */
    Y = A;  /* 01458B TAY */
    S -= 3; F_0149CE();  /* 01458C JSRL */
    mflag = 0;  /* 014590 CLM */
    Y = A;  /* 014591 TAY */
    S -= 3; F_007627();  /* 014592 JSRL */
    wr16(DP(0x05), 0x0002);  /* 014596 LDM */
    A = rd16(DP(0x00));  /* 01459A LDA */
    C = 0;  /* 01459C CLC */
    A = adc16(A, 0x0018);  /* 01459D ADC */
    B = rd16(DP(0x02));  /* 0145A0 LDA */
    Y = A;  /* 0145A3 TAY */
    S -= 3; F_0149CE();  /* 0145A4 JSRL */
    mflag = 0;  /* 0145A8 CLM */
    Y = A;  /* 0145A9 TAY */
    S -= 3; F_007627();  /* 0145AA JSRL */
    wr16(DP(0x05), 0x0003);  /* 0145AE LDM */
    A = rd16(DP(0x00));  /* 0145B2 LDA */
    C = 0;  /* 0145B4 CLC */
    A = adc16(A, 0x0019);  /* 0145B5 ADC */
    B = rd16(DP(0x02));  /* 0145B8 LDA */
    Y = A;  /* 0145BB TAY */
    S -= 3; F_0149CE();  /* 0145BC JSRL */
    mflag = 0;  /* 0145C0 CLM */
    Y = A;  /* 0145C1 TAY */
    S -= 3; F_007627();  /* 0145C2 JSRL */
    wr16(DP(0x05), 0x0004);  /* 0145C6 LDM */
    A = rd16(DP(0x00));  /* 0145CA LDA */
    C = 0;  /* 0145CC CLC */
    A = adc16(A, 0x001A);  /* 0145CD ADC */
    B = rd16(DP(0x02));  /* 0145D0 LDA */
    Y = A;  /* 0145D3 TAY */
    S -= 3; F_0149CE();  /* 0145D4 JSRL */
    mflag = 0;  /* 0145D8 CLM */
    Y = A;  /* 0145D9 TAY */
    S -= 3; F_007627();  /* 0145DA JSRL */
    wr16(DP(0x05), 0x0005);  /* 0145DE LDM */
    A = rd16(DP(0x00));  /* 0145E2 LDA */
    C = 0;  /* 0145E4 CLC */
    A = adc16(A, 0x001B);  /* 0145E5 ADC */
    B = rd16(DP(0x02));  /* 0145E8 LDA */
    Y = A;  /* 0145EB TAY */
    S -= 3; F_0149CE();  /* 0145EC JSRL */
    mflag = 0;  /* 0145F0 CLM */
    Y = A;  /* 0145F1 TAY */
    S -= 3; F_007627();  /* 0145F2 JSRL */
    wr16(DP(0x05), 0x0006);  /* 0145F6 LDM */
    A = rd16(DP(0x00));  /* 0145FA LDA */
    C = 0;  /* 0145FC CLC */
    A = adc16(A, 0x001C);  /* 0145FD ADC */
    B = rd16(DP(0x02));  /* 014600 LDA */
    Y = A;  /* 014603 TAY */
    S -= 3; F_0149CE();  /* 014604 JSRL */
    mflag = 0;  /* 014608 CLM */
    Y = A;  /* 014609 TAY */
    S -= 3; F_007627();  /* 01460A JSRL */
    A = rd16(DP(0x00));  /* 01460E LDA */
    C = 0;  /* 014610 CLC */
    A = adc16(A, 0x001D);  /* 014611 ADC */
    B = rd16(DP(0x02));  /* 014614 LDA */
    Y = A;  /* 014617 TAY */
    S -= 3; F_0149FD();  /* 014618 JSRL */
    mflag = 0;  /* 01461C CLM */
    Y = A;  /* 01461D TAY */
    S -= 3; F_007627();  /* 01461E JSRL */
    wr16(DP(0x05), 0x0006);  /* 014622 LDM */
    A = rd16(DP(0x00));  /* 014626 LDA */
    C = 0;  /* 014628 CLC */
    A = adc16(A, 0x001E);  /* 014629 ADC */
    B = rd16(DP(0x02));  /* 01462C LDA */
    Y = A;  /* 01462F TAY */
    S -= 3; F_0149CE();  /* 014630 JSRL */
    mflag = 0;  /* 014634 CLM */
    Y = A;  /* 014635 TAY */
    S -= 3; F_007627();  /* 014636 JSRL */
    wr16(DP(0x05), 0x0007);  /* 01463A LDM */
    A = rd16(DP(0x00));  /* 01463E LDA */
    C = 0;  /* 014640 CLC */
    A = adc16(A, 0x001F);  /* 014641 ADC */
    B = rd16(DP(0x02));  /* 014644 LDA */
    Y = A;  /* 014647 TAY */
    S -= 3; F_0149CE();  /* 014648 JSRL */
    mflag = 0;  /* 01464C CLM */
    Y = A;  /* 01464D TAY */
    S -= 3; F_007627();  /* 01464E JSRL */
    S -= 3; F_0077F0();  /* 014652 JSRL */
    wr16(DP(0x05), 0x0006);  /* 014656 LDM */
    mflag = 1;  /* 01465A SEM */
    SETLO(B, 0x00);  /* 01465B LDA */
    Y = 0x2050;  /* 01465E LDY */
    mflag = 0;  /* 014661 CLM */
    S -= 3; F_014855();  /* 014662 JSRL */
    wr16(DP(0x05), 0x0000);  /* 014666 LDM */
    A = rd16(DP(0x00));  /* 01466A LDA */
    C = 0;  /* 01466C CLC */
    A = adc16(A, 0x001F);  /* 01466D ADC */
    B = rd16(DP(0x02));  /* 014670 LDA */
    Y = A;  /* 014673 TAY */
    S -= 3; F_0149CE();  /* 014674 JSRL */
    mflag = 0;  /* 014678 CLM */
    Y = A;  /* 014679 TAY */
    S -= 3; F_007627();  /* 01467A JSRL */
    wr16(DP(0x05), 0x0001);  /* 01467E LDM */
    A = rd16(DP(0x00));  /* 014682 LDA */
    C = 0;  /* 014684 CLC */
    A = adc16(A, 0x0020);  /* 014685 ADC */
    B = rd16(DP(0x02));  /* 014688 LDA */
    Y = A;  /* 01468B TAY */
    S -= 3; F_0149CE();  /* 01468C JSRL */
    mflag = 0;  /* 014690 CLM */
    Y = A;  /* 014691 TAY */
    S -= 3; F_007627();  /* 014692 JSRL */
    wr16(DP(0x05), 0x0002);  /* 014696 LDM */
    A = rd16(DP(0x00));  /* 01469A LDA */
    C = 0;  /* 01469C CLC */
    A = adc16(A, 0x0021);  /* 01469D ADC */
    B = rd16(DP(0x02));  /* 0146A0 LDA */
    Y = A;  /* 0146A3 TAY */
    S -= 3; F_0149CE();  /* 0146A4 JSRL */
    mflag = 0;  /* 0146A8 CLM */
    Y = A;  /* 0146A9 TAY */
    S -= 3; F_007627();  /* 0146AA JSRL */
    wr16(DP(0x05), 0x0003);  /* 0146AE LDM */
    A = rd16(DP(0x00));  /* 0146B2 LDA */
    C = 0;  /* 0146B4 CLC */
    A = adc16(A, 0x0022);  /* 0146B5 ADC */
    B = rd16(DP(0x02));  /* 0146B8 LDA */
    Y = A;  /* 0146BB TAY */
    S -= 3; F_0149CE();  /* 0146BC JSRL */
    mflag = 0;  /* 0146C0 CLM */
    Y = A;  /* 0146C1 TAY */
    S -= 3; F_007627();  /* 0146C2 JSRL */
    A = rd16(DP(0x00));  /* 0146C6 LDA */
    C = 0;  /* 0146C8 CLC */
    A = adc16(A, 0x0023);  /* 0146C9 ADC */
    B = rd16(DP(0x02));  /* 0146CC LDA */
    Y = A;  /* 0146CF TAY */
    S -= 3; F_014A20();  /* 0146D0 JSRL */
    mflag = 0;  /* 0146D4 CLM */
    Y = A;  /* 0146D5 TAY */
    S -= 3; F_007627();  /* 0146D6 JSRL */
    wr16(DP(0x05), 0x0007);  /* 0146DA LDM */
    A = rd16(DP(0x00));  /* 0146DE LDA */
    C = 0;  /* 0146E0 CLC */
    A = adc16(A, 0x0025);  /* 0146E1 ADC */
    B = rd16(DP(0x02));  /* 0146E4 LDA */
    Y = A;  /* 0146E7 TAY */
    S -= 3; F_0149CE();  /* 0146E8 JSRL */
    mflag = 0;  /* 0146EC CLM */
    Y = A;  /* 0146ED TAY */
    S -= 3; F_007627();  /* 0146EE JSRL */
    wr16(DP(0x05), 0x0006);  /* 0146F2 LDM */
    mflag = 1;  /* 0146F6 SEM */
    SETLO(B, 0x00);  /* 0146F7 LDA */
    Y = 0x2057;  /* 0146FA LDY */
    mflag = 0;  /* 0146FD CLM */
    S -= 3; F_014855();  /* 0146FE JSRL */
    wr16(DP(0x05), 0x0000);  /* 014702 LDM */
    A = rd16(DP(0x00));  /* 014706 LDA */
    C = 0;  /* 014708 CLC */
    A = adc16(A, 0x0025);  /* 014709 ADC */
    B = rd16(DP(0x02));  /* 01470C LDA */
    Y = A;  /* 01470F TAY */
    S -= 3; F_0149CE();  /* 014710 JSRL */
    mflag = 0;  /* 014714 CLM */
    Y = A;  /* 014715 TAY */
    S -= 3; F_007627();  /* 014716 JSRL */
    wr16(DP(0x05), 0x0001);  /* 01471A LDM */
    A = rd16(DP(0x00));  /* 01471E LDA */
    C = 0;  /* 014720 CLC */
    A = adc16(A, 0x0026);  /* 014721 ADC */
    B = rd16(DP(0x02));  /* 014724 LDA */
    Y = A;  /* 014727 TAY */
    S -= 3; F_0149CE();  /* 014728 JSRL */
    mflag = 0;  /* 01472C CLM */
    Y = A;  /* 01472D TAY */
    S -= 3; F_007627();  /* 01472E JSRL */
    wr16(DP(0x05), 0x0002);  /* 014732 LDM */
    A = rd16(DP(0x00));  /* 014736 LDA */
    C = 0;  /* 014738 CLC */
    A = adc16(A, 0x0027);  /* 014739 ADC */
    B = rd16(DP(0x02));  /* 01473C LDA */
    Y = A;  /* 01473F TAY */
    S -= 3; F_0149CE();  /* 014740 JSRL */
    mflag = 0;  /* 014744 CLM */
    Y = A;  /* 014745 TAY */
    S -= 3; F_007627();  /* 014746 JSRL */
    wr16(DP(0x05), 0x0003);  /* 01474A LDM */
    A = rd16(DP(0x00));  /* 01474E LDA */
    C = 0;  /* 014750 CLC */
    A = adc16(A, 0x0028);  /* 014751 ADC */
    B = rd16(DP(0x02));  /* 014754 LDA */
    Y = A;  /* 014757 TAY */
    S -= 3; F_0149CE();  /* 014758 JSRL */
    mflag = 0;  /* 01475C CLM */
    Y = A;  /* 01475D TAY */
    S -= 3; F_007627();  /* 01475E JSRL */
    A = rd16(DP(0x00));  /* 014762 LDA */
    C = 0;  /* 014764 CLC */
    A = adc16(A, 0x0029);  /* 014765 ADC */
    B = rd16(DP(0x02));  /* 014768 LDA */
    Y = A;  /* 01476B TAY */
    S -= 3; F_014A50();  /* 01476C JSRL */
    mflag = 0;  /* 014770 CLM */
    Y = A;  /* 014771 TAY */
    S -= 3; F_007627();  /* 014772 JSRL */
    wr16(DP(0x05), 0x0003);  /* 014776 LDM */
    A = rd16(DP(0x00));  /* 01477A LDA */
    C = 0;  /* 01477C CLC */
    A = adc16(A, 0x002A);  /* 01477D ADC */
    B = rd16(DP(0x02));  /* 014780 LDA */
    Y = A;  /* 014783 TAY */
    S -= 3; F_0149CE();  /* 014784 JSRL */
    mflag = 0;  /* 014788 CLM */
    Y = A;  /* 014789 TAY */
    S -= 3; F_007627();  /* 01478A JSRL */
    wr16(DP(0x05), 0x0004);  /* 01478E LDM */
    A = rd16(DP(0x00));  /* 014792 LDA */
    C = 0;  /* 014794 CLC */
    A = adc16(A, 0x002B);  /* 014795 ADC */
    B = rd16(DP(0x02));  /* 014798 LDA */
    Y = A;  /* 01479B TAY */
    S -= 3; F_0149CE();  /* 01479C JSRL */
    mflag = 0;  /* 0147A0 CLM */
    Y = A;  /* 0147A1 TAY */
    S -= 3; F_007627();  /* 0147A2 JSRL */
    S -= 3; F_0077F0();  /* 0147A6 JSRL */
    wr16(DP(0x05), 0x0007);  /* 0147AA LDM */
    mflag = 1;  /* 0147AE SEM */
    SETLO(B, 0x00);  /* 0147AF LDA */
    Y = 0x205E;  /* 0147B2 LDY */
    mflag = 0;  /* 0147B5 CLM */
    S -= 3; F_014855();  /* 0147B6 JSRL */
    wr16(DP(0x05), 0x0005);  /* 0147BA LDM */
    A = rd16(DP(0x00));  /* 0147BE LDA */
    C = 0;  /* 0147C0 CLC */
    A = adc16(A, 0x002C);  /* 0147C1 ADC */
    B = rd16(DP(0x02));  /* 0147C4 LDA */
    Y = A;  /* 0147C7 TAY */
    S -= 3; F_0149CE();  /* 0147C8 JSRL */
    mflag = 0;  /* 0147CC CLM */
    Y = A;  /* 0147CD TAY */
    S -= 3; F_007627();  /* 0147CE JSRL */
    wr16(DP(0x05), 0x0007);  /* 0147D2 LDM */
    mflag = 1;  /* 0147D6 SEM */
    SETLO(B, 0x00);  /* 0147D7 LDA */
    Y = 0x2066;  /* 0147DA LDY */
    mflag = 0;  /* 0147DD CLM */
    S -= 3; F_014855();  /* 0147DE JSRL */
    wr16(DP(0x05), 0x0006);  /* 0147E2 LDM */
    A = rd16(DP(0x00));  /* 0147E6 LDA */
    C = 0;  /* 0147E8 CLC */
    A = adc16(A, 0x002D);  /* 0147E9 ADC */
    B = rd16(DP(0x02));  /* 0147EC LDA */
    Y = A;  /* 0147EF TAY */
    S -= 3; F_0149CE();  /* 0147F0 JSRL */
    mflag = 0;  /* 0147F4 CLM */
    Y = A;  /* 0147F5 TAY */
    S -= 3; F_01487E();  /* 0147F6 JSRL */
    Y = 0x002F;  /* 0147FA LDY */
    A = rd16((LIND(0x00) + Y));  /* 0147FD LDA */
    mflag = 1;  /* 0147FF SEM */
    SETLO(A, A & 0x07);  /* 014800 AND */
    mflag = 0;  /* 014802 CLM */
    Y = A;  /* 014803 TAY */
    S -= 3; F_0148E5();  /* 014804 JSRL */
    S -= 3; F_0077F0();  /* 014808 JSRL */
    wr16(DP(0x05), 0x0007);  /* 01480C LDM */
    mflag = 1;  /* 014810 SEM */
    SETLO(B, 0x00);  /* 014811 LDA */
    Y = 0x206E;  /* 014814 LDY */
    mflag = 0;  /* 014817 CLM */
    S -= 3; F_014855();  /* 014818 JSRL */
    wr16(DP(0x05), 0x0002);  /* 01481C LDM */
    B = rd16(DP(0x02));  /* 014820 LDA */
    Y = rd16(DP(0x00));  /* 014823 LDY */
    S -= 3; F_0149CE();  /* 014825 JSRL */
    mflag = 0;  /* 014829 CLM */
    Y = A;  /* 01482A TAY */
    S -= 3; F_01487E();  /* 01482B JSRL */
    A = rd16(DP(0x00));  /* 01482F LDA */
    C = 0;  /* 014831 CLC */
    A = adc16(A, 0x002E);  /* 014832 ADC */
    B = rd16(DP(0x02));  /* 014835 LDA */
    Y = A;  /* 014838 TAY */
    S -= 3; F_014963();  /* 014839 JSRL */
    wr16(DP(0x03), 0x0000);  /* 01483D LDM */
L_014841: TICK();
    X = rd16(DP(0x03));  /* 014841 LDX */
    /* 014843 CPX */
    if (X >= 0x0005) goto L_014851;  /* 014846 BCS */
    S -= 3; F_0077F0();  /* 014848 JSRL */
    t = (rd16(DP(0x03)) + 1) & 0xFFFF; wr16(DP(0x03), t);  /* 01484C INC */
    goto L_014841;  /* 01484E JMP */
L_014851: TICK();
    mflag = 0;  /* 014851 CLM */
    Y = pull16();  /* 014852 PLY */
    D = pull16();  /* 014853 PLD */
    S += 3; return;  /* 014854 RTL */
}

/* 014855  callers: 014349, 014487, 014662, 0146FE, 0147B6, 0147DE, 014818, 0148F9 ... */
void F_014855(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014855 LDA */
    push16(D);  /* 014857 PHD */
    D = A;  /* 014858 TAD */
    C = 0;  /* 014859 CLC */
    A = adc16(A, 0x0007);  /* 01485A ADC */
    push16(A);  /* 01485D PHA */
    wr16(DP(0x02), Y);  /* 01485E STY */
    wr16(DP(0x04), B);  /* 014860 STA */
    wr16(DP(0x05), 0x0000);  /* 014863 LDM */
L_014867: TICK();
    X = rd16(DP(0x05));  /* 014867 LDX */
    t = rd16(DP(0x00));  /* 014869 CPX */
    if (X >= t) goto L_01487A;  /* 01486B BCS */
    Y = X;  /* 01486D TXY */
    A = rd16((LIND(0x02) + Y));  /* 01486E LDA */
    Y = A;  /* 014870 TAY */
    S -= 3; F_007627();  /* 014871 JSRL */
    t = (rd16(DP(0x05)) + 1) & 0xFFFF; wr16(DP(0x05), t);  /* 014875 INC */
    goto L_014867;  /* 014877 JMP */
L_01487A: TICK();
    mflag = 0;  /* 01487A CLM */
    Y = pull16();  /* 01487B PLY */
    D = pull16();  /* 01487C PLD */
    S += 3; return;  /* 01487D RTL */
}

/* 01487E  callers: 0147F6, 01482B */
void F_01487E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01487E LDA */
    push16(D);  /* 014880 PHD */
    D = A;  /* 014881 TAD */
    A += 1;  /* 014882 INC */
    push16(A);  /* 014883 PHA */
    wr16(DP(0x00), Y);  /* 014884 STY */
    mflag = 1;  /* 014886 SEM */
    SETLO(B, 0x00);  /* 014887 LDA */
    SETLO(A, Y);  /* 01488A TYA */
    div8(0x64);  /* 01488B DIV */
    C = 0;  /* 01488E CLC */
    SETLO(A, adc8(A, 0x30));  /* 01488F ADC */
    mflag = 0;  /* 014891 CLM */
    Y = A;  /* 014892 TAY */
    S -= 3; F_007627();  /* 014893 JSRL */
    mflag = 1;  /* 014897 SEM */
    SETLO(B, 0x00);  /* 014898 LDA */
    SETLO(A, rd8(DP(0x00)));  /* 01489B LDA */
    div8(0x64);  /* 01489D DIV */
    mflag = 0;  /* 0148A0 CLM */
    push16(B);  /* 0148A1 PHB */
    A = pull16();  /* 0148A3 PLA */
    A &= 0x00FF;  /* 0148A4 AND */
    Y = A;  /* 0148A7 TAY */
    S -= 3; F_0148AF();  /* 0148A8 JSRL */
    Y = pull16();  /* 0148AC PLY */
    D = pull16();  /* 0148AD PLD */
    S += 3; return;  /* 0148AE RTL */
}

/* 0148AF  callers: 0148A8, 0149B0, 0149C7 */
void F_0148AF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0148AF LDA */
    push16(D);  /* 0148B1 PHD */
    D = A;  /* 0148B2 TAD */
    A += 1;  /* 0148B3 INC */
    A += 1;  /* 0148B4 INC */
    push16(A);  /* 0148B5 PHA */
    B = 0x0000;  /* 0148B6 LDA */
    A = Y;  /* 0148BA TYA */
    div16(0x000A);  /* 0148BB DIV */
    C = 0;  /* 0148BF CLC */
    mflag = 1;  /* 0148C0 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0148C1 ADC */
    wr16(DP(0x00), Y);  /* 0148C3 STY */
    mflag = 0;  /* 0148C5 CLM */
    Y = A;  /* 0148C6 TAY */
    S -= 3; F_007627();  /* 0148C7 JSRL */
    B = 0x0000;  /* 0148CB LDA */
    A = rd16(DP(0x00));  /* 0148CF LDA */
    div16(0x000A);  /* 0148D1 DIV */
    push16(B);  /* 0148D5 PHB */
    A = pull16();  /* 0148D7 PLA */
    C = 0;  /* 0148D8 CLC */
    mflag = 1;  /* 0148D9 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0148DA ADC */
    mflag = 0;  /* 0148DC CLM */
    Y = A;  /* 0148DD TAY */
    S -= 3; F_007627();  /* 0148DE JSRL */
    Y = pull16();  /* 0148E2 PLY */
    D = pull16();  /* 0148E3 PLD */
    S += 3; return;  /* 0148E4 RTL */
}

/* 0148E5  callers: 014804 */
void F_0148E5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0148E5 LDA */
    push16(D);  /* 0148E7 PHD */
    D = A;  /* 0148E8 TAD */
    A += 1;  /* 0148E9 INC */
    push16(A);  /* 0148EA PHA */
    wr16(DP(0x00), Y);  /* 0148EB STY */
    wr16(DP(0x01), 0x0008);  /* 0148ED LDM */
    mflag = 1;  /* 0148F1 SEM */
    SETLO(B, 0x00);  /* 0148F2 LDA */
    Y = 0x2076;  /* 0148F5 LDY */
    mflag = 0;  /* 0148F8 CLM */
    S -= 3; F_014855();  /* 0148F9 JSRL */
    A = rd16(DP(0x00));  /* 0148FD LDA */
    X = 0x0008;  /* 0148FF LDX */
    mflag = 1;  /* 014902 SEM */
    mflag = 1;  /* __switch_byte: inline table at 014907 */  /* 014903 JSRL */
    switch (A & 0xFF) {
        case 0x00: goto L_014921;
        case 0x01: goto L_014928;
        case 0x02: goto L_01492F;
        case 0x03: goto L_014936;
        case 0x04: goto L_01493D;
        case 0x05: goto L_014944;
        case 0x06: goto L_01494B;
        case 0x07: goto L_014952;
        default: goto L_014959;
    }
L_014921: TICK();
    mflag = 1;  /* 014921 SEM */
    wr8(DP(0x00), 0x50);  /* 014922 LDM */
    goto L_014959;  /* 014925 JMP */
L_014928: TICK();
    mflag = 1;  /* 014928 SEM */
    wr8(DP(0x00), 0x56);  /* 014929 LDM */
    goto L_014959;  /* 01492C JMP */
L_01492F: TICK();
    mflag = 1;  /* 01492F SEM */
    wr8(DP(0x00), 0x54);  /* 014930 LDM */
    goto L_014959;  /* 014933 JMP */
L_014936: TICK();
    mflag = 1;  /* 014936 SEM */
    wr8(DP(0x00), 0x41);  /* 014937 LDM */
    goto L_014959;  /* 01493A JMP */
L_01493D: TICK();
    mflag = 1;  /* 01493D SEM */
    wr8(DP(0x00), 0x44);  /* 01493E LDM */
    goto L_014959;  /* 014941 JMP */
L_014944: TICK();
    mflag = 1;  /* 014944 SEM */
    wr8(DP(0x00), 0x45);  /* 014945 LDM */
    goto L_014959;  /* 014948 JMP */
L_01494B: TICK();
    mflag = 1;  /* 01494B SEM */
    wr8(DP(0x00), 0x46);  /* 01494C LDM */
    goto L_014959;  /* 01494F JMP */
L_014952: TICK();
    mflag = 1;  /* 014952 SEM */
    wr8(DP(0x00), 0x4B);  /* 014953 LDM */
    goto L_014959;  /* 014956 JMP */
L_014959: TICK();
    Y = rd16(DP(0x00));  /* 014959 LDY */
    mflag = 0;  /* 01495B CLM */
    S -= 3; F_007627();  /* 01495C JSRL */
    Y = pull16();  /* 014960 PLY */
    D = pull16();  /* 014961 PLD */
    S += 3; return;  /* 014962 RTL */
}

/* 014963  callers: 014839 */
void F_014963(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014963 LDA */
    push16(D);  /* 014965 PHD */
    D = A;  /* 014966 TAD */
    C = 0;  /* 014967 CLC */
    A = adc16(A, 0x0005);  /* 014968 ADC */
    push16(A);  /* 01496B PHA */
    wr16(DP(0x05), 0x0007);  /* 01496C LDM */
    wr16(DP(0x00), Y);  /* 014970 STY */
    mflag = 1;  /* 014972 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 014973 STA */
    SETLO(B, 0x00);  /* 014976 LDA */
    Y = 0x207F;  /* 014979 LDY */
    mflag = 0;  /* 01497C CLM */
    S -= 3; F_014855();  /* 01497D JSRL */
    Y = 0x0001;  /* 014981 LDY */
    A = rd16((LIND(0x00) + Y));  /* 014984 LDA */
    A &= 0x00FF;  /* 014986 AND */
    wr16(DP(0x05), A);  /* 014989 STA */
    A = rd16(LIND(0x00));  /* 01498B LDA */
    A &= 0x00FF;  /* 01498D AND */
    A = rla16(A, 0x0008);  /* 014990 RLA */
    A &= 0xFF00;  /* 014994 AND */
    C = 0;  /* 014997 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 014998 ADC */
    B = 0x0008;  /* 01499A LDA */
    S -= 3; F_00FF8C();  /* 01499E JSRL */
    A &= 0x07FF;  /* 0149A2 AND */
    B = 0x0000;  /* 0149A5 LDA */
    wr16(DP(0x03), A);  /* 0149A9 STA */
    div16(0x003C);  /* 0149AB DIV */
    Y = A;  /* 0149AF TAY */
    S -= 3; F_0148AF();  /* 0149B0 JSRL */
    Y = 0x003A;  /* 0149B4 LDY */
    S -= 3; F_007627();  /* 0149B7 JSRL */
    B = 0x0000;  /* 0149BB LDA */
    A = rd16(DP(0x03));  /* 0149BF LDA */
    div16(0x003C);  /* 0149C1 DIV */
    Y = B;  /* 0149C5 TBY */
    S -= 3; F_0148AF();  /* 0149C7 JSRL */
    Y = pull16();  /* 0149CB PLY */
    D = pull16();  /* 0149CC PLD */
    S += 3; return;  /* 0149CD RTL */
}

/* 0149CE  callers: 014358, 01436E, 014385, 01439D, 0143C9, 0143E1, 0143F9, 014411 ... */
void F_0149CE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0149CE LDA */
    push16(D);  /* 0149D0 PHD */
    D = A;  /* 0149D1 TAD */
    wr16(DP(0x02), Y);  /* 0149D2 STY */
    mflag = 1;  /* 0149D4 SEM */
    wr8(DP(0x04), (uint8_t)B);  /* 0149D5 STA */
    Y = 0x0001;  /* 0149D8 LDY */
    SETLO(A, rd8((LIND(0x02) + Y)));  /* 0149DB LDA */
    mflag = 0;  /* 0149DD CLM */
    A &= 0x00FF;  /* 0149DE AND */
    wr16(DP(0x05), A);  /* 0149E1 STA */
    A = rd16(LIND(0x02));  /* 0149E3 LDA */
    A &= 0x00FF;  /* 0149E5 AND */
    A = rla16(A, 0x0008);  /* 0149E8 RLA */
    A &= 0xFF00;  /* 0149EC AND */
    C = 0;  /* 0149EF CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 0149F0 ADC */
    X = rd16(DP(0x00));  /* 0149F2 LDX */
    S -= 3; rt_lsar_01E48E();  /* 0149F4 JSRL */
    mflag = 1;  /* 0149F8 SEM */
    SETLO(A, A & 0x7F);  /* 0149F9 AND */
    D = pull16();  /* 0149FB PLD */
    S += 3; return;  /* 0149FC RTL */
}

/* 0149FD  callers: 0143B1, 014618 */
void F_0149FD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0149FD LDA */
    push16(D);  /* 0149FF PHD */
    D = A;  /* 014A00 TAD */
    wr16(DP(0x00), Y);  /* 014A01 STY */
    mflag = 1;  /* 014A03 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 014A04 STA */
    Y = 0x0001;  /* 014A07 LDY */
    SETLO(B, 0x00);  /* 014A0A LDA */
    SETLO(A, rd8((LIND(0x00) + Y)));  /* 014A0D LDA */
    div8(0x20);  /* 014A0F DIV */
    SETLO(A, A & 0x01);  /* 014A12 AND */
    wr8(DP(0x03), (uint8_t)A);  /* 014A14 STA */
    SETLO(A, rd8(LIND(0x00)));  /* 014A16 LDA */
    SETLO(A, A & 0x3F);  /* 014A18 AND */
    SETLO(A, asl8(A));  /* 014A1A ASL */
    C = 0;  /* 014A1B CLC */
    SETLO(A, adc8(A, rd8(DP(0x03))));  /* 014A1C ADC */
    D = pull16();  /* 014A1E PLD */
    S += 3; return;  /* 014A1F RTL */
}

/* 014A20  callers: 014455, 0146D0 */
void F_014A20(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014A20 LDA */
    push16(D);  /* 014A22 PHD */
    D = A;  /* 014A23 TAD */
    wr16(DP(0x00), Y);  /* 014A24 STY */
    mflag = 1;  /* 014A26 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 014A27 STA */
    Y = 0x0002;  /* 014A2A LDY */
    SETLO(B, 0x00);  /* 014A2D LDA */
    SETLO(A, rd8((LIND(0x00) + Y)));  /* 014A30 LDA */
    div8(0x40);  /* 014A32 DIV */
    Y -= 1;  /* 014A35 DEY */
    wr8(DP(0x03), (uint8_t)A);  /* 014A36 STA */
    SETLO(A, rd8((LIND(0x00) + Y)));  /* 014A38 LDA */
    SETLO(A, A & 0x03);  /* 014A3A AND */
    SETLO(A, asl8(A));  /* 014A3C ASL */
    SETLO(A, asl8(A));  /* 014A3D ASL */
    C = 0;  /* 014A3E CLC */
    SETLO(A, adc8(A, rd8(DP(0x03))));  /* 014A3F ADC */
    wr8(DP(0x03), (uint8_t)A);  /* 014A41 STA */
    SETLO(A, rd8(LIND(0x00)));  /* 014A43 LDA */
    SETLO(A, A & 0x07);  /* 014A45 AND */
    SETLO(A, asl8(A));  /* 014A47 ASL */
    SETLO(A, asl8(A));  /* 014A48 ASL */
    SETLO(A, asl8(A));  /* 014A49 ASL */
    SETLO(A, asl8(A));  /* 014A4A ASL */
    C = 0;  /* 014A4B CLC */
    SETLO(A, adc8(A, rd8(DP(0x03))));  /* 014A4C ADC */
    D = pull16();  /* 014A4E PLD */
    S += 3; return;  /* 014A4F RTL */
}

/* 014A50  callers: 0144F5, 01476C */
void F_014A50(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014A50 LDA */
    push16(D);  /* 014A52 PHD */
    D = A;  /* 014A53 TAD */
    wr16(DP(0x00), Y);  /* 014A54 STY */
    mflag = 1;  /* 014A56 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 014A57 STA */
    Y = 0x0001;  /* 014A5A LDY */
    SETLO(B, 0x00);  /* 014A5D LDA */
    SETLO(A, rd8((LIND(0x00) + Y)));  /* 014A60 LDA */
    div8(0x04);  /* 014A62 DIV */
    SETLO(A, A & 0x0F);  /* 014A65 AND */
    wr8(DP(0x03), (uint8_t)A);  /* 014A67 STA */
    SETLO(A, rd8(LIND(0x00)));  /* 014A69 LDA */
    SETLO(A, A & 0x07);  /* 014A6B AND */
    SETLO(A, asl8(A));  /* 014A6D ASL */
    SETLO(A, asl8(A));  /* 014A6E ASL */
    SETLO(A, asl8(A));  /* 014A6F ASL */
    SETLO(A, asl8(A));  /* 014A70 ASL */
    C = 0;  /* 014A71 CLC */
    SETLO(A, adc8(A, rd8(DP(0x03))));  /* 014A72 ADC */
    D = pull16();  /* 014A74 PLD */
    S += 3; return;  /* 014A75 RTL */
}

/* 014A76  callers: 00575E */
void F_014A76(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014A76 LDA */
    push16(A);  /* 014A78 PHA */
    A = rd16(ABS(0x0118));  /* 014A79 LDA */
    mflag = 1;  /* 014A7C SEM */
    /* 014A7D CMP */
    if ((A & 0xFF) != 0x03) goto L_014AB0;  /* 014A7F BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 014A81 LDA */
    if ((A & 0xFF) != 0) goto L_014AB0;  /* 014A84 BNE */
    mflag = 0;  /* 014A86 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_014AB0;  /* 014A87 BBS */
    if ((rd16(ABS(0x0240)) & 0x8000) == 0x8000) goto L_014AB0;  /* 014A8D BBS */
    mflag = 1;  /* 014A93 SEM */
    SETLO(A, rd8(0x0100FF));  /* 014A94 LDA */
    SETLO(A, A - 1);  /* 014A98 DEC */
    if ((A & 0xFF) == 0) goto L_014AB0;  /* 014A99 BEQ */
    SETLO(A, rd8(ABS(0x0293)));  /* 014A9B LDA */
    if ((A & 0xFF) == 0) goto L_014AAB;  /* 014A9E BEQ */
    Y = 0x000D;  /* 014AA0 LDY */
    mflag = 0;  /* 014AA3 CLM */
    S -= 3; F_0080AA();  /* 014AA4 JSRL */
    goto L_014AB0;  /* 014AA8 JMP */
L_014AAB: TICK();
    mflag = 1;  /* 014AAB SEM */
    wr8(ABS(0x0118), 0x02);  /* 014AAC LDM */
L_014AB0: TICK();
    mflag = 0;  /* 014AB0 CLM */
    Y = pull16();  /* 014AB1 PLY */
    S += 3; return;  /* 014AB2 RTL */
}

/* 014AB3  callers: 003771 */
void F_014AB3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014AB3 LDA */
    push16(D);  /* 014AB5 PHD */
    D = A;  /* 014AB6 TAD */
    C = 0;  /* 014AB7 CLC */
    A = adc16(A, 0x0006);  /* 014AB8 ADC */
    push16(A);  /* 014ABB PHA */
    A = rd16(ABS(0x0118));  /* 014ABC LDA */
    mflag = 1;  /* 014ABF SEM */
    /* 014AC0 CMP */
    if ((A & 0xFF) == 0x02) goto L_014AC7;  /* 014AC2 BEQ */
    goto L_014CCD;  /* 014AC4 JMP */
L_014AC7: TICK();
    wr8(ABS(0x0231), 0x15);  /* 014AC7 LDM */
    SETLO(A, rd8(ABS(0x0226)));  /* 014ACB LDA */
    /* 014ACE CMP */
    if ((A & 0xFF) == 0x64) goto L_014AD5;  /* 014AD0 BEQ */
    goto L_014B9B;  /* 014AD2 JMP */
L_014AD5: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 014AD5 LDA */
    /* 014AD8 CMP */
    if ((A & 0xFF) >= 0x04) goto L_014B42;  /* 014ADA BCS */
    mflag = 0;  /* 014ADC CLM */
    A &= 0x00FF;  /* 014ADD AND */
    A -= 1;  /* 014AE0 DEC */
    X = A;  /* 014AE1 TAX */
    A = rd16(ABS(0x0293));  /* 014AE2 LDA */
    A &= 0x00FF;  /* 014AE5 AND */
    A -= 1;  /* 014AE8 DEC */
    Y = A;  /* 014AE9 TAY */
    A = rd16((ABS(0x0283) + X));  /* 014AEA LDA */
    mflag = 1;  /* 014AED SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 014AEE STA */
    SETLO(A, rd8(ABS(0x0293)));  /* 014AF1 LDA */
    /* 014AF4 CMP */
    if ((A & 0xFF) != 0x03) goto L_014B3F;  /* 014AF6 BNE */
    SETLO(A, rd8(ABS(0x0284)));  /* 014AF8 LDA */
    mpy8(0x0A);  /* 014AFB MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 014AFE STA */
    wr8(DP(0x07), (uint8_t)B);  /* 014B00 STA */
    SETLO(A, rd8(ABS(0x0285)));  /* 014B03 LDA */
    mflag = 0;  /* 014B06 CLM */
    A &= 0x00FF;  /* 014B07 AND */
    C = 0;  /* 014B0A CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 014B0B ADC */
    C = 0;  /* 014B0D CLC */
    A = adc16(A, 0xFDF0);  /* 014B0E ADC */
    wr16(DP(0x00), A);  /* 014B11 STA */
    /* 014B13 CMP */
    if (A < 0x000A) goto L_014B2A;  /* 014B16 BCC */
    /* 014B18 CMP */
    if (A >= 0x001D) goto L_014B2A;  /* 014B1B BCS */
    /* 014B1D CMP */
    if (A == 0x0013) goto L_014B2A;  /* 014B20 BEQ */
    Y = A;  /* 014B22 TAY */
    S -= 3; F_014D00();  /* 014B23 JSRL */
    goto L_014B3F;  /* 014B27 JMP */
L_014B2A: TICK();
    Y = 0x0019;  /* 014B2A LDY */
    mflag = 0;  /* 014B2D CLM */
    S -= 3; F_0080AA();  /* 014B2E JSRL */
    mflag = 1;  /* 014B32 SEM */
    wr8(ABS(0x0254), 0x20);  /* 014B33 LDM */
    wr8(ABS(0x0253), 0x20);  /* 014B37 LDM */
    wr8(ABS(0x0293), 0x01);  /* 014B3B LDM */
L_014B3F: TICK();
    goto L_014B98;  /* 014B3F JMP */
L_014B42: TICK();
    mflag = 0;  /* 014B42 CLM */
    A = rd16(ABS(0x0293));  /* 014B43 LDA */
    mflag = 1;  /* 014B46 SEM */
    /* 014B47 CMP */
    if ((A & 0xFF) != 0x04) goto L_014B68;  /* 014B49 BNE */
    Y = 0x0023;  /* 014B4B LDY */
    mflag = 0;  /* 014B4E CLM */
    S -= 3; F_003310();  /* 014B4F JSRL */
    mflag = 1;  /* 014B53 SEM */
    wr8(ABS(0x0252), 0x2A);  /* 014B54 LDM */
    SETLO(A, rd8(ABS(0x0286)));  /* 014B58 LDA */
    wr8(ABS(0x0284), (uint8_t)A);  /* 014B5B STA */
    wr8(ABS(0x0253), (uint8_t)A);  /* 014B5E STA */
    wr8(ABS(0x0293), 0x02);  /* 014B61 LDM */
    goto L_014B98;  /* 014B65 JMP */
L_014B68: TICK();
    mflag = 0;  /* 014B68 CLM */
    A = rd16(ABS(0x0293));  /* 014B69 LDA */
    mflag = 1;  /* 014B6C SEM */
    /* 014B6D CMP */
    if ((A & 0xFF) >= 0x09) goto L_014B8C;  /* 014B6F BCS */
    SETLO(A, rd8(ABS(0x0293)));  /* 014B71 LDA */
    mflag = 0;  /* 014B74 CLM */
    A &= 0x00FF;  /* 014B75 AND */
    A -= 1;  /* 014B78 DEC */
    X = A;  /* 014B79 TAX */
    A = rd16(ABS(0x0293));  /* 014B7A LDA */
    A &= 0x00FF;  /* 014B7D AND */
    A -= 1;  /* 014B80 DEC */
    Y = A;  /* 014B81 TAY */
    A = rd16((ABS(0x0283) + X));  /* 014B82 LDA */
    mflag = 1;  /* 014B85 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 014B86 STA */
    goto L_014B98;  /* 014B89 JMP */
L_014B8C: TICK();
    Y = 0x0019;  /* 014B8C LDY */
    mflag = 0;  /* 014B8F CLM */
    S -= 3; F_0080AA();  /* 014B90 JSRL */
    mflag = 1;  /* 014B94 SEM */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 014B95 DEC */
L_014B98: TICK();
    goto L_014CCA;  /* 014B98 JMP */
L_014B9B: TICK();
    mflag = 0;  /* 014B9B CLM */
    A = rd16(ABS(0x0226));  /* 014B9C LDA */
    mflag = 1;  /* 014B9F SEM */
    /* 014BA0 CMP */
    if ((A & 0xFF) != 0x2A) goto L_014BEE;  /* 014BA2 BNE */
    Y = 0x0023;  /* 014BA4 LDY */
    mflag = 0;  /* 014BA7 CLM */
    S -= 3; F_003310();  /* 014BA8 JSRL */
    A = rd16(ABS(0x0293));  /* 014BAC LDA */
    mflag = 1;  /* 014BAF SEM */
    /* 014BB0 CMP */
    if ((A & 0xFF) != 0x03) goto L_014BDE;  /* 014BB2 BNE */
    mflag = 0;  /* 014BB4 CLM */
    X = rd16(ABS(0x017F));  /* 014BB5 LDX */
    if (X != 0) goto L_014BDE;  /* 014BB8 BNE */
    mflag = 1;  /* 014BBA SEM */
    wr8(ABS(0x0286), 0x2A);  /* 014BBB LDM */
    mflag = 0;  /* 014BBF CLM */
    wr16(DP(0x04), X);  /* 014BC0 STX */
L_014BC2: TICK();
    X = rd16(DP(0x04));  /* 014BC2 LDX */
    /* 014BC4 CPX */
    if (X >= 0x0004) goto L_014BD7;  /* 014BC7 BCS */
    A = rd16((ABS(0x0283) + X));  /* 014BC9 LDA */
    mflag = 1;  /* 014BCC SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 014BCD STA */
    X += 1;  /* 014BD0 INX */
    wr16(DP(0x04), X);  /* 014BD1 STX */
    mflag = 0;  /* 014BD3 CLM */
    goto L_014BC2;  /* 014BD4 JMP */
L_014BD7: TICK();
    mflag = 1;  /* 014BD7 SEM */
    t = (rd8(ABS(0x0293)) + 1) & 0xFF; wr8(ABS(0x0293), t);  /* 014BD8 INC */
    goto L_014BEB;  /* 014BDB JMP */
L_014BDE: TICK();
    mflag = 1;  /* 014BDE SEM */
    wr8(ABS(0x0283), 0x2A);  /* 014BDF LDM */
    wr8(ABS(0x0252), 0x2A);  /* 014BE3 LDM */
    wr8(ABS(0x0293), 0x01);  /* 014BE7 LDM */
L_014BEB: TICK();
    goto L_014CCA;  /* 014BEB JMP */
L_014BEE: TICK();
    mflag = 0;  /* 014BEE CLM */
    A = rd16(ABS(0x0226));  /* 014BEF LDA */
    mflag = 1;  /* 014BF2 SEM */
    /* 014BF3 CMP */
    if ((A & 0xFF) == 0x23) goto L_014BFA;  /* 014BF5 BEQ */
    goto L_014CC5;  /* 014BF7 JMP */
L_014BFA: TICK();
    SETLO(A, rd8(ABS(0x0284)));  /* 014BFA LDA */
    mpy8(0x0A);  /* 014BFD MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 014C00 STA */
    wr8(DP(0x07), (uint8_t)B);  /* 014C02 STA */
    SETLO(A, rd8(ABS(0x0285)));  /* 014C05 LDA */
    mflag = 0;  /* 014C08 CLM */
    A &= 0x00FF;  /* 014C09 AND */
    C = 0;  /* 014C0C CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 014C0D ADC */
    C = 0;  /* 014C0F CLC */
    A = adc16(A, 0xFDF0);  /* 014C10 ADC */
    wr16(DP(0x00), A);  /* 014C13 STA */
    A = rd16(ABS(0x0293));  /* 014C15 LDA */
    mflag = 1;  /* 014C18 SEM */
    /* 014C19 CMP */
    if ((A & 0xFF) != 0x03) goto L_014C42;  /* 014C1B BNE */
    wr8(DP(0x06), 0xFF);  /* 014C1D LDM */
    mflag = 0;  /* 014C20 CLM */
    X = rd16(ABS(0x017F));  /* 014C21 LDX */
    if (X != 0) goto L_014C2C;  /* 014C24 BNE */
    A = 0x0FFF;  /* 014C26 LDA */
    goto L_014C2D;  /* 014C29 JMP */
L_014C2C: TICK();
    A = X;  /* 014C2C TXA */
L_014C2D: TICK();
    wr16(DP(0x07), A);  /* 014C2D STA */
    X = rd16(DP(0x00));  /* 014C2F LDX */
    wr16(DP(0x09), X);  /* 014C31 STX */
    Y = 0x0080;  /* 014C33 LDY */
    S -= 3; F_014E15();  /* 014C36 JSRL */
    mflag = 1;  /* 014C3A SEM */
    wr8(ABS(0x0118), 0x04);  /* 014C3B LDM */
    goto L_014CBD;  /* 014C3F JMP */
L_014C42: TICK();
    mflag = 0;  /* 014C42 CLM */
    A = rd16(ABS(0x0293));  /* 014C43 LDA */
    mflag = 1;  /* 014C46 SEM */
    /* 014C47 CMP */
    if ((A & 0xFF) < 0x05) goto L_014CB8;  /* 014C49 BCC */
    mflag = 0;  /* 014C4B CLM */
    X = rd16(ABS(0x017F));  /* 014C4C LDX */
    if (X != 0) goto L_014CB8;  /* 014C4F BNE */
    wr16(DP(0x02), X);  /* 014C51 STX */
    wr16(DP(0x04), X);  /* 014C53 STX */
L_014C55: TICK();
    A = rd16(ABS(0x0293));  /* 014C55 LDA */
    A &= 0x00FF;  /* 014C58 AND */
    C = 0;  /* 014C5B CLC */
    A = adc16(A, 0xFFFC);  /* 014C5C ADC */
    t = rd16(DP(0x04));  /* 014C5F CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_014C88;  /* 014C61 BCC */
    if (Z) goto L_014C88;  /* 014C63 BEQ */
    X = rd16(DP(0x04));  /* 014C65 LDX */
    if (X == 0) goto L_014C71;  /* 014C67 BEQ */
    A = rd16(DP(0x02));  /* 014C69 LDA */
    mpy16(0x000A);  /* 014C6B MPY */
    wr16(DP(0x02), A);  /* 014C6F STA */
L_014C71: TICK();
    X = rd16(DP(0x04));  /* 014C71 LDX */
    A = rd16((ABS(0x0287) + X));  /* 014C73 LDA */
    A &= 0x00FF;  /* 014C76 AND */
    C = 0;  /* 014C79 CLC */
    A = adc16(A, 0xFFD0);  /* 014C7A ADC */
    C = 0;  /* 014C7D CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 014C7E ADC */
    wr16(DP(0x02), A);  /* 014C80 STA */
    X += 1;  /* 014C82 INX */
    wr16(DP(0x04), X);  /* 014C83 STX */
    goto L_014C55;  /* 014C85 JMP */
L_014C88: TICK();
    mflag = 0;  /* 014C88 CLM */
    X = rd16(DP(0x02));  /* 014C89 LDX */
    /* 014C8B CPX */
    if (X >= 0x1000) goto L_014CB0;  /* 014C8E BCS */
    X = rd16(ABS(0x017F));  /* 014C90 LDX */
    if (X != 0) goto L_014CB0;  /* 014C93 BNE */
    wr16(DP(0x06), 0x00FF);  /* 014C95 LDM */
    X = rd16(DP(0x02));  /* 014C99 LDX */
    wr16(DP(0x07), X);  /* 014C9B STX */
    X = rd16(DP(0x00));  /* 014C9D LDX */
    wr16(DP(0x09), X);  /* 014C9F STX */
    Y = 0x0080;  /* 014CA1 LDY */
    S -= 3; F_014E15();  /* 014CA4 JSRL */
    mflag = 1;  /* 014CA8 SEM */
    wr8(ABS(0x0118), 0x04);  /* 014CA9 LDM */
    goto L_014CB5;  /* 014CAD JMP */
L_014CB0: TICK();
    mflag = 0;  /* 014CB0 CLM */
    S -= 3; F_014CDD();  /* 014CB1 JSRL */
L_014CB5: TICK();
    goto L_014CBD;  /* 014CB5 JMP */
L_014CB8: TICK();
    mflag = 0;  /* 014CB8 CLM */
    S -= 3; F_014CDD();  /* 014CB9 JSRL */
L_014CBD: TICK();
    mflag = 1;  /* 014CBD SEM */
    wr8(ABS(0x0293), 0x00);  /* 014CBE LDM */
    goto L_014CCA;  /* 014CC2 JMP */
L_014CC5: TICK();
    mflag = 0;  /* 014CC5 CLM */
    S -= 3; F_014CDD();  /* 014CC6 JSRL */
L_014CCA: TICK();
    goto L_014CD5;  /* 014CCA JMP */
L_014CCD: TICK();
    mflag = 0;  /* 014CCD CLM */
    A = 0x0000;  /* 014CCE LDA */
    mflag = 1;  /* 014CD1 SEM */
    Y = pull16();  /* 014CD2 PLY */
    D = pull16();  /* 014CD3 PLD */
    S += 3; return;  /* 014CD4 RTL */
L_014CD5: TICK();
    mflag = 0;  /* 014CD5 CLM */
    A = 0x0001;  /* 014CD6 LDA */
    mflag = 1;  /* 014CD9 SEM */
    Y = pull16();  /* 014CDA PLY */
    D = pull16();  /* 014CDB PLD */
    S += 3; return;  /* 014CDC RTL */
}

/* 014CDD  callers: 014CB1, 014CB9, 014CC6 */
void F_014CDD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014CDD LDA */
    push16(A);  /* 014CDF PHA */
    mflag = 1;  /* 014CE0 SEM */
    wr8(ABS(0x0231), 0x00);  /* 014CE1 LDM */
    wr8(ABS(0x0293), 0x00);  /* 014CE5 LDM */
    Y = 0x0019;  /* 014CE9 LDY */
    mflag = 0;  /* 014CEC CLM */
    S -= 3; F_0080AA();  /* 014CED JSRL */
    Y = 0x002C;  /* 014CF1 LDY */
    S -= 3; F_003310();  /* 014CF4 JSRL */
    mflag = 1;  /* 014CF8 SEM */
    wr8(ABS(0x0118), 0x03);  /* 014CF9 LDM */
    mflag = 0;  /* 014CFD CLM */
    Y = pull16();  /* 014CFE PLY */
    S += 3; return;  /* 014CFF RTL */
}

/* 014D00  callers: 014B23, 014D61 */
void F_014D00(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014D00 LDA */
    push16(D);  /* 014D02 PHD */
    D = A;  /* 014D03 TAD */
    A += 1;  /* 014D04 INC */
    A += 1;  /* 014D05 INC */
    push16(A);  /* 014D06 PHA */
    wr16(DP(0x02), 0x0008);  /* 014D07 LDM */
    wr16(DP(0x03), 0x0252);  /* 014D0B LDM */
    wr16(DP(0x05), 0x0000);  /* 014D0F LDM */
    A = Y;  /* 014D13 TYA */
    wr16(DP(0x00), Y);  /* 014D14 STY */
    C = 0;  /* 014D16 CLC */
    A = adc16(A, 0xFFF6);  /* 014D17 ADC */
    A = asl16(A);  /* 014D1A ASL */
    A = asl16(A);  /* 014D1B ASL */
    A = asl16(A);  /* 014D1C ASL */
    C = 0;  /* 014D1D CLC */
    A = adc16(A, 0x0612);  /* 014D1E ADC */
    wr16(DP(0x06), A);  /* 014D21 STA */
    Y = 0x00A0;  /* 014D23 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 014D26 JSRL */
    S -= 3; F_00828C();  /* 014D2A JSRL */
    Y = pull16();  /* 014D2E PLY */
    D = pull16();  /* 014D2F PLD */
    S += 3; return;  /* 014D30 RTL */
}

/* 014D31  callers: 013E79, 014D58 */
void F_014D31(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014D31 LDA */
    push16(D);  /* 014D33 PHD */
    D = A;  /* 014D34 TAD */
    wr16(DP(0x00), Y);  /* 014D35 STY */
    A = Y;  /* 014D37 TYA */
    mflag = 1;  /* 014D38 SEM */
    SETLO(A, A & 0x7C);  /* 014D39 AND */
    SETLO(B, 0x00);  /* 014D3B LDA */
    div8(0x04);  /* 014D3E DIV */
    C = 0;  /* 014D41 CLC */
    SETLO(A, adc8(A, 0x0A));  /* 014D42 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 014D44 STA */
    C = ((A & 0xFF) >= 0x1B);  /* 014D46 CMP */
    if (!C) goto L_014D4E;  /* 014D48 BCC */
    SETLO(A, adc8(A, 0xF7));  /* 014D4A ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 014D4C STA */
L_014D4E: TICK();
    D = pull16();  /* 014D4E PLD */
    S += 3; return;  /* 014D4F RTL */
}

/* 014D50  callers: 013EE4 */
void F_014D50(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014D50 LDA */
    push16(D);  /* 014D52 PHD */
    D = A;  /* 014D53 TAD */
    A += 1;  /* 014D54 INC */
    push16(A);  /* 014D55 PHA */
    wr16(DP(0x00), Y);  /* 014D56 STY */
    S -= 3; F_014D31();  /* 014D58 JSRL */
    mflag = 0;  /* 014D5C CLM */
    A &= 0x00FF;  /* 014D5D AND */
    Y = A;  /* 014D60 TAY */
    S -= 3; F_014D00();  /* 014D61 JSRL */
    Y = 0x002C;  /* 014D65 LDY */
    S -= 3; F_003310();  /* 014D68 JSRL */
    Y = pull16();  /* 014D6C PLY */
    D = pull16();  /* 014D6D PLD */
    S += 3; return;  /* 014D6E RTL */
}

/* 014D6F  callers: 013ECC */
void F_014D6F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014D6F LDA */
    push16(D);  /* 014D71 PHD */
    D = A;  /* 014D72 TAD */
    C = 0;  /* 014D73 CLC */
    A = adc16(A, 0x000C);  /* 014D74 ADC */
    push16(A);  /* 014D77 PHA */
    wr16(DP(0x0C), 0x0008);  /* 014D78 LDM */
    A = D;  /* 014D7C TDA */
    C = 0;  /* 014D7D CLC */
    A = adc16(A, 0x0004);  /* 014D7E ADC */
    B = 0x0000;  /* 014D81 LDA */
    wr16(DP(0x0D), A);  /* 014D85 STA */
    wr16(DP(0x0F), B);  /* 014D87 STA */
    A = Y;  /* 014D8A TYA */
    wr16(DP(0x00), Y);  /* 014D8B STY */
    C = 0;  /* 014D8D CLC */
    A = adc16(A, 0xFFF6);  /* 014D8E ADC */
    A = asl16(A);  /* 014D91 ASL */
    A = asl16(A);  /* 014D92 ASL */
    A = asl16(A);  /* 014D93 ASL */
    C = 0;  /* 014D94 CLC */
    A = adc16(A, 0x0612);  /* 014D95 ADC */
    wr16(DP(0x10), A);  /* 014D98 STA */
    Y = 0x00A0;  /* 014D9A LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 014D9D JSRL */
    wr16(DP(0x02), 0x0000);  /* 014DA1 LDM */
L_014DA5: TICK();
    X = rd16(DP(0x02));  /* 014DA5 LDX */
    /* 014DA7 CPX */
    if (X >= 0x0005) goto L_014DB5;  /* 014DAA BCS */
    S -= 3; F_0077F0();  /* 014DAC JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 014DB0 INC */
    goto L_014DA5;  /* 014DB2 JMP */
L_014DB5: TICK();
    mflag = 0;  /* 014DB5 CLM */
    wr16(DP(0x02), 0x0000);  /* 014DB6 LDM */
L_014DBA: TICK();
    X = rd16(DP(0x02));  /* 014DBA LDX */
    /* 014DBC CPX */
    if (X >= 0x0005) goto L_014DCD;  /* 014DBF BCS */
    Y = 0x002A;  /* 014DC1 LDY */
    S -= 3; F_007627();  /* 014DC4 JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 014DC8 INC */
    goto L_014DBA;  /* 014DCA JMP */
L_014DCD: TICK();
    mflag = 0;  /* 014DCD CLM */
    wr16(DP(0x02), 0x0000);  /* 014DCE LDM */
L_014DD2: TICK();
    X = rd16(DP(0x02));  /* 014DD2 LDX */
    /* 014DD4 CPX */
    if (X >= 0x0008) goto L_014DE4;  /* 014DD7 BCS */
    Y = rd16(DP(0x04 + X));  /* 014DD9 LDY */
    S -= 3; F_007627();  /* 014DDB JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 014DDF INC */
    goto L_014DD2;  /* 014DE1 JMP */
L_014DE4: TICK();
    mflag = 0;  /* 014DE4 CLM */
    wr16(DP(0x02), 0x0000);  /* 014DE5 LDM */
L_014DE9: TICK();
    X = rd16(DP(0x02));  /* 014DE9 LDX */
    /* 014DEB CPX */
    if (X >= 0x0005) goto L_014DFC;  /* 014DEE BCS */
    Y = 0x002A;  /* 014DF0 LDY */
    S -= 3; F_007627();  /* 014DF3 JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 014DF7 INC */
    goto L_014DE9;  /* 014DF9 JMP */
L_014DFC: TICK();
    mflag = 0;  /* 014DFC CLM */
    wr16(DP(0x02), 0x0000);  /* 014DFD LDM */
L_014E01: TICK();
    X = rd16(DP(0x02));  /* 014E01 LDX */
    /* 014E03 CPX */
    if (X >= 0x0005) goto L_014E11;  /* 014E06 BCS */
    S -= 3; F_0077F0();  /* 014E08 JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 014E0C INC */
    goto L_014E01;  /* 014E0E JMP */
L_014E11: TICK();
    mflag = 0;  /* 014E11 CLM */
    Y = pull16();  /* 014E12 PLY */
    D = pull16();  /* 014E13 PLD */
    S += 3; return;  /* 014E14 RTL */
}

/* 014E15  callers: 014C36, 014CA4, 014F59 */
void F_014E15(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014E15 LDA */
    push16(D);  /* 014E17 PHD */
    D = A;  /* 014E18 TAD */
    wr16(DP(0x05), Y);  /* 014E19 STY */
    A = Y;  /* 014E1B TYA */
    mflag = 1;  /* 014E1C SEM */
    /* 014E1D CMP */
    if ((A & 0xFF) != 0x82) goto L_014E29;  /* 014E1F BNE */
    mflag = 0;  /* 014E21 CLM */
    wr16(DP(0x03), 0x0064);  /* 014E22 LDM */
    goto L_014E41;  /* 014E26 JMP */
L_014E29: TICK();
    mflag = 0;  /* 014E29 CLM */
    X = rd16(DP(0x03));  /* 014E2A LDX */
    /* 014E2C CPX */
    if (X >= 0x001A) goto L_014E37;  /* 014E2F BCS */
    A = 0x0016;  /* 014E31 LDA */
    goto L_014E3A;  /* 014E34 JMP */
L_014E37: TICK();
    A = 0x001E;  /* 014E37 LDA */
L_014E3A: TICK();
    C = 0;  /* 014E3A CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 014E3B ADC */
    A = asl16(A);  /* 014E3D ASL */
    A = asl16(A);  /* 014E3E ASL */
    wr16(DP(0x03), A);  /* 014E3F STA */
L_014E41: TICK();
    mflag = 1;  /* 014E41 SEM */
    SETLO(B, 0x00);  /* 014E42 LDA */
    SETLO(A, rd8(ABS(0x0199)));  /* 014E45 LDA */
    div8(0x10);  /* 014E48 DIV */
    C = 0;  /* 014E4B CLC */
    SETLO(A, adc8(A, rd8(DP(0x03))));  /* 014E4C ADC */
    wr8(ABS(0x0119), (uint8_t)A);  /* 014E4E STA */
    mflag = 0;  /* 014E51 CLM */
    A = rd16(ABS(0x0196));  /* 014E52 LDA */
    A &= 0x00FF;  /* 014E55 AND */
    mflag = 1;  /* 014E58 SEM */
    wr8(DP(0x06), (uint8_t)A);  /* 014E59 STA */
    SETLO(A, rd8(ABS(0x0199)));  /* 014E5B LDA */
    SETLO(A, A & 0x0F);  /* 014E5E AND */
    SETLO(A, asl8(A));  /* 014E60 ASL */
    SETLO(A, asl8(A));  /* 014E61 ASL */
    SETLO(A, asl8(A));  /* 014E62 ASL */
    SETLO(A, asl8(A));  /* 014E63 ASL */
    C = 0;  /* 014E64 CLC */
    SETLO(A, adc8(A, rd8(DP(0x06))));  /* 014E65 ADC */
    wr8(ABS(0x011A), (uint8_t)A);  /* 014E67 STA */
    SETLO(A, rd8(DP(0x00)));  /* 014E6A LDA */
    /* 014E6C CMP */
    if ((A & 0xFF) != 0xFF) goto L_014E76;  /* 014E6E BNE */
    SETLO(A, rd8(ABS(0x0195)));  /* 014E70 LDA */
    goto L_014E7E;  /* 014E73 JMP */
L_014E76: TICK();
    SETLO(A, rd8(ABS(0x0195)));  /* 014E76 LDA */
    SETLO(A, A & 0x80);  /* 014E79 AND */
    C = 0;  /* 014E7B CLC */
    SETLO(A, adc8(A, rd8(DP(0x00))));  /* 014E7C ADC */
L_014E7E: TICK();
    wr8(ABS(0x011B), (uint8_t)A);  /* 014E7E STA */
    mflag = 0;  /* 014E81 CLM */
    A = rd16(ABS(0x0197));  /* 014E82 LDA */
    A = lsr16(A);  /* 014E85 LSR */
    A = lsr16(A);  /* 014E86 LSR */
    A = lsr16(A);  /* 014E87 LSR */
    A = lsr16(A);  /* 014E88 LSR */
    mflag = 1;  /* 014E89 SEM */
    wr8(ABS(0x011C), (uint8_t)A);  /* 014E8A STA */
    mflag = 0;  /* 014E8D CLM */
    A = rd16(DP(0x02));  /* 014E8E LDA */
    A &= 0x00FF;  /* 014E90 AND */
    mflag = 1;  /* 014E93 SEM */
    wr8(DP(0x06), (uint8_t)A);  /* 014E94 STA */
    SETLO(A, rd8(ABS(0x0197)));  /* 014E96 LDA */
    SETLO(A, A & 0x0F);  /* 014E99 AND */
    SETLO(A, asl8(A));  /* 014E9B ASL */
    SETLO(A, asl8(A));  /* 014E9C ASL */
    SETLO(A, asl8(A));  /* 014E9D ASL */
    SETLO(A, asl8(A));  /* 014E9E ASL */
    C = 0;  /* 014E9F CLC */
    SETLO(A, adc8(A, rd8(DP(0x06))));  /* 014EA0 ADC */
    wr8(ABS(0x011D), (uint8_t)A);  /* 014EA2 STA */
    SETLO(A, rd8(DP(0x01)));  /* 014EA5 LDA */
    wr8(ABS(0x011E), (uint8_t)A);  /* 014EA7 STA */
    mflag = 0;  /* 014EAA CLM */
    X = rd16(DP(0x01));  /* 014EAB LDX */
    wr16(ABS(0x03D5), X);  /* 014EAD STX */
    D = pull16();  /* 014EB0 PLD */
    S += 3; return;  /* 014EB1 RTL */
}

/* 014EB2  callers: 01A278, 01A29A */
void F_014EB2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014EB2 LDA */
    push16(D);  /* 014EB4 PHD */
    D = A;  /* 014EB5 TAD */
    A += 1;  /* 014EB6 INC */
    A += 1;  /* 014EB7 INC */
    push16(A);  /* 014EB8 PHA */
    wr16(DP(0x01), Y);  /* 014EB9 STY */
    A = rd16(ABS(0x029D));  /* 014EBB LDA */
    A &= 0x00FF;  /* 014EBE AND */
    wr16(DP(0x02), A);  /* 014EC1 STA */
    A = rd16(ABS(0x0195));  /* 014EC3 LDA */
    A &= 0x007F;  /* 014EC6 AND */
    t = rd16(DP(0x02));  /* 014EC9 CMP */
    if (A == t) goto L_014EFE;  /* 014ECB BEQ */
    wr16(DP(0x02), 0x0000);  /* 014ECD LDM */
    A = rd16(ABS(0x029E));  /* 014ED1 LDA */
    A &= 0x00FF;  /* 014ED4 AND */
    A = rla16(A, 0x0008);  /* 014ED7 RLA */
    A &= 0xFF00;  /* 014EDB AND */
    wr16(DP(0x03), A);  /* 014EDE STA */
    A = rd16(ABS(0x029F));  /* 014EE0 LDA */
    A &= 0x00FF;  /* 014EE3 AND */
    C = 0;  /* 014EE6 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 014EE7 ADC */
    wr16(DP(0x03), A);  /* 014EE9 STA */
    A = rd16(ABS(0x029D));  /* 014EEB LDA */
    wr16(DP(0x05), A);  /* 014EEE STA */
    wr16(DP(0x06), 0x0008);  /* 014EF0 LDM */
    Y = 0x00E0;  /* 014EF4 LDY */
    S -= 3; F_01501B();  /* 014EF7 JSRL */
    goto L_014F71;  /* 014EFB JMP */
L_014EFE: TICK();
    mflag = 0;  /* 014EFE CLM */
    A = rd16(DP(0x01));  /* 014EFF LDA */
    mflag = 1;  /* 014F01 SEM */
    /* 014F02 CMP */
    if ((A & 0xFF) != 0x82) goto L_014F28;  /* 014F04 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 014F06 LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 014F08 STA */
    mflag = 0;  /* 014F0A CLM */
    wr16(DP(0x03), 0x0122);  /* 014F0B LDM */
    wr16(DP(0x05), 0x0000);  /* 014F0F LDM */
    B = 0x0000;  /* 014F13 LDA */
    Y = 0x008D;  /* 014F17 LDY */
    S -= 3; F_013FD9();  /* 014F1A JSRL */
    A = D;  /* 014F1E TDA */
    B = 0x0000;  /* 014F1F LDA */
    Y = A;  /* 014F23 TAY */
    S -= 3; F_014F75();  /* 014F24 JSRL */
L_014F28: TICK();
    mflag = 0;  /* 014F28 CLM */
    A = rd16(DP(0x00));  /* 014F29 LDA */
    mflag = 1;  /* 014F2B SEM */
    wr8(ABS(0x018A), (uint8_t)A);  /* 014F2C STA */
    SETLO(A, rd8(ABS(0x029D)));  /* 014F2F LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 014F32 STA */
    SETLO(A, rd8(ABS(0x029E)));  /* 014F34 LDA */
    mflag = 0;  /* 014F37 CLM */
    A &= 0x00FF;  /* 014F38 AND */
    A = rla16(A, 0x0008);  /* 014F3B RLA */
    A &= 0xFF00;  /* 014F3F AND */
    wr16(DP(0x03), A);  /* 014F42 STA */
    A = rd16(ABS(0x029F));  /* 014F44 LDA */
    A &= 0x00FF;  /* 014F47 AND */
    C = 0;  /* 014F4A CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 014F4B ADC */
    wr16(DP(0x03), A);  /* 014F4D STA */
    A = rd16(ABS(0x02A1));  /* 014F4F LDA */
    A &= 0x00FF;  /* 014F52 AND */
    wr16(DP(0x05), A);  /* 014F55 STA */
    Y = rd16(DP(0x01));  /* 014F57 LDY */
    S -= 3; F_014E15();  /* 014F59 JSRL */
    A = rd16(ABS(0x0118));  /* 014F5D LDA */
    mflag = 1;  /* 014F60 SEM */
    /* 014F61 CMP */
    if ((A & 0xFF) != 0x02) goto L_014F6D;  /* 014F63 BNE */
    wr8(ABS(0x0231), 0x15);  /* 014F65 LDM */
    wr8(ABS(0x0293), 0x00);  /* 014F69 LDM */
L_014F6D: TICK();
    wr8(ABS(0x0118), 0x04);  /* 014F6D LDM */
L_014F71: TICK();
    mflag = 0;  /* 014F71 CLM */
    Y = pull16();  /* 014F72 PLY */
    D = pull16();  /* 014F73 PLD */
    S += 3; return;  /* 014F74 RTL */
}

/* 014F75  callers: 014F24 */
void F_014F75(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 014F75 LDA */
    push16(D);  /* 014F77 PHD */
    D = A;  /* 014F78 TAD */
    wr16(DP(0x00), Y);  /* 014F79 STY */
    mflag = 1;  /* 014F7B SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 014F7C STA */
    SETLO(A, rd8(LIND(0x00)));  /* 014F7F LDA */
    mflag = 0;  /* 014F81 CLM */
    A &= 0x00FF;  /* 014F82 AND */
    A += 1;  /* 014F85 INC */
    A += 1;  /* 014F86 INC */
    A += 1;  /* 014F87 INC */
    B = 0x0006;  /* 014F88 LDA */
    S -= 3; F_01E350();  /* 014F8C JSRL */
    mflag = 1;  /* 014F90 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 014F91 STA */
L_014F93: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 014F93 LDA */
    /* 014F95 CMP */
    if ((A & 0xFF) >= 0x06) goto L_014FB6;  /* 014F97 BCS */
    mflag = 0;  /* 014F99 CLM */
    A &= 0x00FF;  /* 014F9A AND */
    wr16(DP(0x04), A);  /* 014F9D STA */
    A = rd16(LIND(0x00));  /* 014F9F LDA */
    A &= 0x00FF;  /* 014FA1 AND */
    C = 1;  /* 014FA4 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 014FA5 SBC */
    C = 0;  /* 014FA7 CLC */
    A = adc16(A, 0x000E);  /* 014FA8 ADC */
    X = A;  /* 014FAB TAX */
    mflag = 1;  /* 014FAC SEM */
    wr8((ABS(0x0119) + X), 0x00);  /* 014FAD LDM */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 014FB1 INC */
    goto L_014F93;  /* 014FB3 JMP */
L_014FB6: TICK();
    mflag = 0;  /* 014FB6 CLM */
    A = rd16(LIND(0x00));  /* 014FB7 LDA */
    A &= 0x00FF;  /* 014FB9 AND */
    A += 1;  /* 014FBC INC */
    A += 1;  /* 014FBD INC */
    B = 0x0006;  /* 014FBE LDA */
    S -= 3; F_00FF8C();  /* 014FC2 JSRL */
    mflag = 1;  /* 014FC6 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 014FC7 STA */
    SETLO(A, asl8(A));  /* 014FC9 ASL */
    SETLO(A, asl8(A));  /* 014FCA ASL */
    SETLO(A, asl8(A));  /* 014FCB ASL */
    SETLO(A, asl8(A));  /* 014FCC ASL */
    wr8(ABS(0x011F), (uint8_t)A);  /* 014FCD STA */
    SETLO(A, rd8(DP(0x03)));  /* 014FD0 LDA */
    SETLO(A, A - 1);  /* 014FD2 DEC */
    mpy8(0x06);  /* 014FD3 MPY */
    SETLO(A, A ^ 0xFF);  /* 014FD6 EOR */
    SETLO(A, A + 1);  /* 014FD8 INC */
    C = 0;  /* 014FD9 CLC */
    SETLO(A, adc8(A, rd8(LIND(0x00))));  /* 014FDA ADC */
    C = 0;  /* 014FDC CLC */
    SETLO(A, adc8(A, 0xFD));  /* 014FDD ADC */
    SETLO(A, asl8(A));  /* 014FDF ASL */
    SETLO(A, asl8(A));  /* 014FE0 ASL */
    SETLO(A, asl8(A));  /* 014FE1 ASL */
    SETLO(B, 0x00);  /* 014FE2 LDA */
    wr8(DP(0x03), (uint8_t)A);  /* 014FE5 STA */
    div8(0x04);  /* 014FE7 DIV */
    SETLO(A, A | rd8(ABS(0x011F)));  /* 014FEA ORA */
    wr8(ABS(0x011F), (uint8_t)A);  /* 014FED STA */
    SETLO(A, rd8(DP(0x03)));  /* 014FF0 LDA */
    SETLO(A, A & 0x03);  /* 014FF2 AND */
    SETLO(A, A & 0x03);  /* 014FF4 AND */
    SETLO(A, lsr8(A));  /* 014FF6 LSR */
    SETLO(A, ror8(A));  /* 014FF7 ROR */
    SETLO(A, ror8(A));  /* 014FF8 ROR */
    wr8(ABS(0x0120), (uint8_t)A);  /* 014FF9 STA */
    wr8(ABS(0x0121), 0x10);  /* 014FFC LDM */
    SETLO(A, rd8(LIND(0x00)));  /* 015000 LDA */
    mflag = 0;  /* 015002 CLM */
    A &= 0x00FF;  /* 015003 AND */
    A += 1;  /* 015006 INC */
    A += 1;  /* 015007 INC */
    B = 0x0006;  /* 015008 LDA */
    S -= 3; F_00FF8C();  /* 01500C JSRL */
    mflag = 1;  /* 015010 SEM */
    SETLO(A, A + 1);  /* 015011 INC */
    SETLO(A, A + 1);  /* 015012 INC */
    mpy8(0x06);  /* 015013 MPY */
    wr8(LIND(0x00), (uint8_t)A);  /* 015016 STA */
    mflag = 0;  /* 015018 CLM */
    D = pull16();  /* 015019 PLD */
    S += 3; return;  /* 01501A RTL */
}

/* 01501B  callers: 0135F9, 0136A8, 01374C, 0137CE, 01389D, 013A07, 013EBD, 013FAB ... */
void F_01501B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01501B LDA */
    push16(D);  /* 01501D PHD */
    D = A;  /* 01501E TAD */
    C = 0;  /* 01501F CLC */
    A = adc16(A, 0x0006);  /* 015020 ADC */
    push16(A);  /* 015023 PHA */
    wr16(DP(0x05), Y);  /* 015024 STY */
    A = Y;  /* 015026 TYA */
    mflag = 1;  /* 015027 SEM */
    wr8(ABS(0x029C), (uint8_t)A);  /* 015028 STA */
    SETLO(A, rd8(DP(0x03)));  /* 01502B LDA */
    wr8(ABS(0x029D), (uint8_t)A);  /* 01502D STA */
    mflag = 0;  /* 015030 CLM */
    A = rd16(DP(0x02));  /* 015031 LDA */
    A &= 0x00FF;  /* 015033 AND */
    mflag = 1;  /* 015036 SEM */
    wr8(ABS(0x029E), (uint8_t)A);  /* 015037 STA */
    SETLO(A, rd8(DP(0x01)));  /* 01503A LDA */
    wr8(ABS(0x029F), (uint8_t)A);  /* 01503C STA */
    wr8(ABS(0x02A0), 0x00);  /* 01503F LDM */
    SETLO(A, rd8(DP(0x04)));  /* 015043 LDA */
    wr8(ABS(0x02A1), (uint8_t)A);  /* 015045 STA */
    SETLO(A, Y);  /* 015048 TYA */
    /* 015049 CMP */
    if ((A & 0xFF) != 0x82) goto L_015068;  /* 01504B BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01504D LDA */
    C = 0;  /* 01504F CLC */
    SETLO(A, adc8(A, 0xF7));  /* 015050 ADC */
    wr8(DP(0x06), (uint8_t)A);  /* 015052 STA */
    mflag = 0;  /* 015054 CLM */
    wr16(DP(0x07), 0x008D);  /* 015055 LDM */
    wr16(DP(0x09), 0x0000);  /* 015059 LDM */
    B = 0x0000;  /* 01505D LDA */
    Y = 0x0122;  /* 015061 LDY */
    S -= 3; F_013FD9();  /* 015064 JSRL */
L_015068: TICK();
    Y = rd16(DP(0x00));  /* 015068 LDY */
    mflag = 0;  /* 01506A CLM */
    S -= 3; F_0196AA();  /* 01506B JSRL */
    Y = pull16();  /* 01506F PLY */
    D = pull16();  /* 015070 PLD */
    S += 3; return;  /* 015071 RTL */
}

/* 015072  callers:  */
void F_015072(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 015072 SEM */
    wr8(ABS(0x0293), 0x00);  /* 015073 LDM */
    mflag = 0;  /* 015077 CLM */
    S += 3; return;  /* 015078 RTL */
}

/* 015079  callers: 015F1E, 01689B */
void F_015079(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 015079 LDA */
    push16(D);  /* 01507B PHD */
    D = A;  /* 01507C TAD */
    C = 0;  /* 01507D CLC */
    A = adc16(A, 0x0004);  /* 01507E ADC */
    push16(A);  /* 015081 PHA */
    wr16(DP(0x00), 0x0004);  /* 015082 LDM */
    wr16(DP(0x02), 0x0040);  /* 015086 LDM */
    Y = 0x0002;  /* 01508A LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 01508D JSRL */
    mflag = 0;  /* 015091 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0002);  /* 015092 CLB */
    A = D;  /* 015097 TDA */
    push16(A);  /* 015098 PHA */
    S -= 3; F_008DED();  /* 015099 JSRL */
    Y = pull16();  /* 01509D PLY */
    mflag = 1;  /* 01509E SEM */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x04);  /* 01509F CLB */
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x10);  /* 0150A3 CLB */
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x08);  /* 0150A7 CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x80);  /* 0150AB CLB */
    Y = 0x0000;  /* 0150AF LDY */
    mflag = 0;  /* 0150B2 CLM */
    A = D;  /* 0150B3 TDA */
    push16(A);  /* 0150B4 PHA */
    S -= 3; F_01DA02();  /* 0150B5 JSRL */
    Y = pull16();  /* 0150B9 PLY */
    mflag = 1;  /* 0150BA SEM */
    wr8(ABS(0x0455), 0x00);  /* 0150BB LDM */
    wr8(ABS(0x0369), 0x00);  /* 0150BF LDM */
    wr8(ABS(0x0230), 0x00);  /* 0150C3 LDM */
    wr8(ABS(0x0233), 0x00);  /* 0150C7 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0150D9;  /* 0150CB BBS */
    mflag = 0;  /* 0150D0 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0040);  /* 0150D1 SEB */
    goto L_0150DE;  /* 0150D6 JMP */
L_0150D9: TICK();
    mflag = 1;  /* 0150D9 SEM */
    wr8(ABS(0x0231), 0x16);  /* 0150DA LDM */
L_0150DE: TICK();
    mflag = 1;  /* 0150DE SEM */
    wr8(ABS(0x0227), 0x00);  /* 0150DF LDM */
    mflag = 0;  /* 0150E3 CLM */
    A = 0x0000;  /* 0150E4 LDA */
    A = asl16(A);  /* 0150E7 ASL */
    A = asl16(A);  /* 0150E8 ASL */
    A += 1;  /* 0150E9 INC */
    X = A;  /* 0150EA TAX */
    A = rd16((0x00319E + X));  /* 0150EB LDA */
    mflag = 1;  /* 0150EF SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 0150F0 STA */
    mflag = 0;  /* 0150F3 CLM */
    A = rd16((0x00319C + X));  /* 0150F4 LDA */
    wr16(ABS(0x02DF), A);  /* 0150F8 STA */
    mflag = 1;  /* 0150FB SEM */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x20);  /* 0150FC CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x20);  /* 015100 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x04);  /* 015104 CLB */
    Y = 0x0000;  /* 015108 LDY */
    mflag = 0;  /* 01510B CLM */
    A = D;  /* 01510C TDA */
    push16(A);  /* 01510D PHA */
    S -= 3; F_0069EF();  /* 01510E JSRL */
    Y = pull16();  /* 015112 PLY */
    /* 015113 CMP */
    if ((A & 0xFF) == 0x0F) goto L_01511D;  /* 015115 BEQ */
    mflag = 0;  /* 015117 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x8000);  /* 015118 SEB */
L_01511D: TICK();
    Y = 0x0006;  /* 01511D LDY */
    mflag = 0;  /* 015120 CLM */
    A = D;  /* 015121 TDA */
    push16(A);  /* 015122 PHA */
    S -= 3; F_00927F();  /* 015123 JSRL */
    Y = pull16();  /* 015127 PLY */
    Y = pull16();  /* 015128 PLY */
    D = pull16();  /* 015129 PLD */
    S += 3; return;  /* 01512A RTL */
}

/* 01512B  callers: 01539C, 015418 */
void F_01512B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01512B LDA */
    push16(D);  /* 01512D PHD */
    D = A;  /* 01512E TAD */
    wr16(DP(0x02), Y);  /* 01512F STY */
    wr16(DP(0x04), B);  /* 015131 STA */
    mflag = 1;  /* 015134 SEM */
    wr8(DP(0x05), 0x00);  /* 015135 LDM */
    mflag = 0;  /* 015138 CLM */
    X = rd16(DP(0x00));  /* 015139 LDX */
    wr16(DP(0x06), X);  /* 01513B STX */
    /* 01513D CPX */
    if (X >= 0x1B5A) goto L_015192;  /* 015140 BCS */
L_015142: TICK();
    X = rd16(DP(0x06));  /* 015142 LDX */
    C = (X >= 0x02BE);  /* 015144 CPX */
    if (!C) goto L_015156;  /* 015147 BCC */
    A = X;  /* 015149 TXA */
    A = adc16(A, 0xFD43);  /* 01514A ADC */
    wr16(DP(0x06), A);  /* 01514D STA */
    mflag = 1;  /* 01514F SEM */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 015150 INC */
    mflag = 0;  /* 015152 CLM */
    goto L_015142;  /* 015153 JMP */
L_015156: TICK();
    mflag = 0;  /* 015156 CLM */
    A = rd16(DP(0x05));  /* 015157 LDA */
    mflag = 1;  /* 015159 SEM */
    t = rd8(ABS(0x01C2));  /* 01515A CMP */
    if ((A & 0xFF) < t) goto L_015186;  /* 01515D BCC */
    SETLO(A, rd8(DP(0x05)));  /* 01515F LDA */
    mpy8(0x0A);  /* 015161 MPY */
    wr8(DP(0x05), (uint8_t)A);  /* 015164 STA */
L_015166: TICK();
    mflag = 0;  /* 015166 CLM */
    X = rd16(DP(0x06));  /* 015167 LDX */
    C = (X >= 0x0048);  /* 015169 CPX */
    if (!C) goto L_01517A;  /* 01516C BCC */
    A = X;  /* 01516E TXA */
    A = adc16(A, 0xFFB9);  /* 01516F ADC */
    wr16(DP(0x06), A);  /* 015172 STA */
    mflag = 1;  /* 015174 SEM */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 015175 INC */
    goto L_015166;  /* 015177 JMP */
L_01517A: TICK();
    mflag = 0;  /* 01517A CLM */
    A = rd16(DP(0x06));  /* 01517B LDA */
    C = 0;  /* 01517D CLC */
    A = adc16(A, 0x0012);  /* 01517E ADC */
    wr16(DP(0x06), A);  /* 015181 STA */
    goto L_01518F;  /* 015183 JMP */
L_015186: TICK();
    mflag = 0;  /* 015186 CLM */
    A = rd16(DP(0x06));  /* 015187 LDA */
    C = 0;  /* 015189 CLC */
    A = adc16(A, 0x00C6);  /* 01518A ADC */
    wr16(DP(0x06), A);  /* 01518D STA */
L_01518F: TICK();
    goto L_0151E8;  /* 01518F JMP */
L_015192: TICK();
    mflag = 0;  /* 015192 CLM */
    A = rd16(DP(0x06));  /* 015193 LDA */
    C = 0;  /* 015195 CLC */
    A = adc16(A, 0xE4A8);  /* 015196 ADC */
    wr16(DP(0x06), A);  /* 015199 STA */
L_01519B: TICK();
    X = rd16(DP(0x06));  /* 01519B LDX */
    C = (X >= 0x0066);  /* 01519D CPX */
    if (!C) goto L_0151AF;  /* 0151A0 BCC */
    A = X;  /* 0151A2 TXA */
    A = adc16(A, 0xFF9B);  /* 0151A3 ADC */
    wr16(DP(0x06), A);  /* 0151A6 STA */
    mflag = 1;  /* 0151A8 SEM */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 0151A9 INC */
    mflag = 0;  /* 0151AB CLM */
    goto L_01519B;  /* 0151AC JMP */
L_0151AF: TICK();
    mflag = 0;  /* 0151AF CLM */
    A = rd16(DP(0x05));  /* 0151B0 LDA */
    mflag = 1;  /* 0151B2 SEM */
    t = rd8(ABS(0x01C2));  /* 0151B3 CMP */
    if ((A & 0xFF) < t) goto L_0151DF;  /* 0151B6 BCC */
    SETLO(A, rd8(DP(0x05)));  /* 0151B8 LDA */
    mpy8(0x0A);  /* 0151BA MPY */
    wr8(DP(0x05), (uint8_t)A);  /* 0151BD STA */
L_0151BF: TICK();
    mflag = 0;  /* 0151BF CLM */
    X = rd16(DP(0x06));  /* 0151C0 LDX */
    C = (X >= 0x000C);  /* 0151C2 CPX */
    if (!C) goto L_0151D3;  /* 0151C5 BCC */
    A = X;  /* 0151C7 TXA */
    A = adc16(A, 0xFFF5);  /* 0151C8 ADC */
    wr16(DP(0x06), A);  /* 0151CB STA */
    mflag = 1;  /* 0151CD SEM */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 0151CE INC */
    goto L_0151BF;  /* 0151D0 JMP */
L_0151D3: TICK();
    mflag = 0;  /* 0151D3 CLM */
    A = rd16(DP(0x06));  /* 0151D4 LDA */
    C = 0;  /* 0151D6 CLC */
    A = adc16(A, 0x0058);  /* 0151D7 ADC */
    wr16(DP(0x06), A);  /* 0151DA STA */
    goto L_0151E8;  /* 0151DC JMP */
L_0151DF: TICK();
    mflag = 0;  /* 0151DF CLM */
    A = rd16(DP(0x06));  /* 0151E0 LDA */
    C = 0;  /* 0151E2 CLC */
    A = adc16(A, 0x0382);  /* 0151E3 ADC */
    wr16(DP(0x06), A);  /* 0151E6 STA */
L_0151E8: TICK();
    A = D;  /* 0151E8 TDA */
    C = 0;  /* 0151E9 CLC */
    A = adc16(A, 0x0005);  /* 0151EA ADC */
    wr16(DP(0x08), A);  /* 0151ED STA */
    wr16(DP(0x0A), A);  /* 0151EF STA */
    mflag = 1;  /* 0151F1 SEM */
    wr8(DP(0x0C), 0x00);  /* 0151F2 LDM */
    mflag = 0;  /* 0151F5 CLM */
    A = rd16(DP(0x02));  /* 0151F6 LDA */
    wr16(DP(0x0D), A);  /* 0151F8 STA */
    A = rd16(DP(0x04));  /* 0151FA LDA */
    mflag = 1;  /* 0151FC SEM */
    wr8(DP(0x0F), (uint8_t)A);  /* 0151FD STA */
    mflag = 0;  /* 0151FF CLM */
    A = 0x000A;  /* 015200 LDA */
    Y = 0x0003;  /* 015203 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 015206 JSRL */
    D = pull16();  /* 01520A PLD */
    S += 3; return;  /* 01520B RTL */
}

/* 01520C  callers: 01559B, 0155DD, 015737, 015F74, 015F8F, 016046, 01608D, 0161A9 ... */
void F_01520C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01520C LDA */
    push16(D);  /* 01520E PHD */
    D = A;  /* 01520F TAD */
    C = 0;  /* 015210 CLC */
    A = adc16(A, 0x0014);  /* 015211 ADC */
    push16(A);  /* 015214 PHA */
    wr16(DP(0x00), Y);  /* 015215 STY */
    Y = 0x0023;  /* 015217 LDY */
    S -= 3; F_003310();  /* 01521A JSRL */
    A = rd16(DP(0x00));  /* 01521E LDA */
    mflag = 1;  /* 015220 SEM */
    /* 015221 CMP */
    if ((A & 0xFF) != 0x01) goto L_015231;  /* 015223 BNE */
    mflag = 0;  /* 015225 CLM */
    X = rd16(ABS(0x01CD));  /* 015226 LDX */
    if (X != 0) goto L_015231;  /* 015229 BNE */
    mflag = 1;  /* 01522B SEM */
    SETLO(A, rd8(ABS(0x01E4)));  /* 01522C LDA */
    if ((A & 0xFF) == 0) goto L_01523F;  /* 01522F BEQ */
L_015231: TICK();
    mflag = 0;  /* 015231 CLM */
    A = rd16(DP(0x00));  /* 015232 LDA */
    mflag = 1;  /* 015234 SEM */
    /* 015235 CMP */
    if ((A & 0xFF) != 0x02) goto L_01527D;  /* 015237 BNE */
    mflag = 0;  /* 015239 CLM */
    X = rd16(ABS(0x03D5));  /* 01523A LDX */
    if (X != 0) goto L_01527D;  /* 01523D BNE */
L_01523F: TICK();
    mflag = 0;  /* 01523F CLM */
    if ((rd16(ABS(0x039E)) & 0x4000) == 0) goto L_01526D;  /* 015240 BBC */
    A = rd16(ABS(0x01C1));  /* 015246 LDA */
    mflag = 1;  /* 015249 SEM */
    /* 01524A CMP */
    if ((A & 0xFF) != 0x0A) goto L_015255;  /* 01524C BNE */
    wr8(ABS(0x01C4), 0x19);  /* 01524E LDM */
    goto L_01526A;  /* 015252 JMP */
L_015255: TICK();
    mflag = 0;  /* 015255 CLM */
    A = rd16(ABS(0x01C1));  /* 015256 LDA */
    mflag = 1;  /* 015259 SEM */
    /* 01525A CMP */
    if ((A & 0xFF) != 0x0B) goto L_015265;  /* 01525C BNE */
    wr8(ABS(0x01C4), 0x1A);  /* 01525E LDM */
    goto L_01526A;  /* 015262 JMP */
L_015265: TICK();
    mflag = 1;  /* 015265 SEM */
    wr8(ABS(0x01C4), 0x17);  /* 015266 LDM */
L_01526A: TICK();
    goto L_015272;  /* 01526A JMP */
L_01526D: TICK();
    mflag = 1;  /* 01526D SEM */
    wr8(ABS(0x01C4), 0x17);  /* 01526E LDM */
L_015272: TICK();
    Y = rd16(ABS(0x01C4));  /* 015272 LDY */
    mflag = 0;  /* 015275 CLM */
    S -= 3; F_003310();  /* 015276 JSRL */
    Y = pull16();  /* 01527A PLY */
    D = pull16();  /* 01527B PLD */
    S += 3; return;  /* 01527C RTL */
L_01527D: TICK();
    mflag = 0;  /* 01527D CLM */
    A = rd16(DP(0x00));  /* 01527E LDA */
    mflag = 1;  /* 015280 SEM */
    /* 015281 CMP */
    if ((A & 0xFF) != 0x03) goto L_0152C4;  /* 015283 BNE */
    wr8(DP(0x04), 0x01);  /* 015285 LDM */
L_015288: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 015288 LDA */
    /* 01528A CMP */
    if ((A & 0xFF) >= 0x09) goto L_0152C0;  /* 01528C BCS */
    SETLO(A, rd8(ABS(0x0293)));  /* 01528E LDA */
    t = rd8(DP(0x04));  /* 015291 CMP */
    C = ((A & 0xFF) >= t);
    if (!C) goto L_0152C0;  /* 015293 BCC */
    SETLO(A, rd8(DP(0x04)));  /* 015295 LDA */
    mflag = 0;  /* 015297 CLM */
    A &= 0x00FF;  /* 015298 AND */
    wr16(DP(0x14), A);  /* 01529B STA */
    A = rd16(ABS(0x0293));  /* 01529D LDA */
    A &= 0x00FF;  /* 0152A0 AND */
    A = sbc16(A, rd16(DP(0x14)));  /* 0152A3 SBC */
    X = A;  /* 0152A5 TAX */
    A = rd16(DP(0x04));  /* 0152A6 LDA */
    A &= 0x00FF;  /* 0152A8 AND */
    A ^= 0xFFFF;  /* 0152AB EOR */
    A += 1;  /* 0152AE INC */
    C = 0;  /* 0152AF CLC */
    A = adc16(A, 0x025A);  /* 0152B0 ADC */
    Y = A;  /* 0152B3 TAY */
    A = rd16((ABS(0x0283) + X));  /* 0152B4 LDA */
    mflag = 1;  /* 0152B7 SEM */
    wr8((ABS(0x0000) + Y), (uint8_t)A);  /* 0152B8 STA */
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 0152BB INC */
    goto L_015288;  /* 0152BD JMP */
L_0152C0: TICK();
    mflag = 0;  /* 0152C0 CLM */
    Y = pull16();  /* 0152C1 PLY */
    D = pull16();  /* 0152C2 PLD */
    S += 3; return;  /* 0152C3 RTL */
L_0152C4: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0152C4 LDA */
    if ((A & 0xFF) != 0) goto L_01531F;  /* 0152C6 BNE */
    mflag = 0;  /* 0152C8 CLM */
    X = rd16(ABS(0x03FB));  /* 0152C9 LDX */
    if (X != 0) goto L_0152DE;  /* 0152CC BNE */
    mflag = 1;  /* 0152CE SEM */
    wr8(ABS(0x01C4), 0x18);  /* 0152CF LDM */
    Y = 0x0018;  /* 0152D3 LDY */
    mflag = 0;  /* 0152D6 CLM */
    S -= 3; F_003310();  /* 0152D7 JSRL */
    Y = pull16();  /* 0152DB PLY */
    D = pull16();  /* 0152DC PLD */
    S += 3; return;  /* 0152DD RTL */
L_0152DE: TICK();
    wr16(DP(0x14), 0x0008);  /* 0152DE LDM */
    wr16(DP(0x15), 0x0252);  /* 0152E2 LDM */
    wr16(DP(0x17), 0x0000);  /* 0152E6 LDM */
    wr16(DP(0x18), 0x0132);  /* 0152EA LDM */
    Y = 0x00A0;  /* 0152EE LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 0152F1 JSRL */
    mflag = 1;  /* 0152F5 SEM */
    wr8(DP(0x04), 0x00);  /* 0152F6 LDM */
L_0152F9: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 0152F9 LDA */
    /* 0152FB CMP */
    if ((A & 0xFF) >= 0x08) goto L_01531B;  /* 0152FD BCS */
    mflag = 0;  /* 0152FF CLM */
    A &= 0x00FF;  /* 015300 AND */
    X = A;  /* 015303 TAX */
    mflag = 1;  /* 015304 SEM */
    SETLO(A, rd8((ABS(0x0252) + X)));  /* 015305 LDA */
    if ((A & 0xFF) != 0) goto L_015316;  /* 015308 BNE */
    SETLO(A, rd8(DP(0x04)));  /* 01530A LDA */
    mflag = 0;  /* 01530C CLM */
    A &= 0x00FF;  /* 01530D AND */
    X = A;  /* 015310 TAX */
    mflag = 1;  /* 015311 SEM */
    wr8((ABS(0x0252) + X), 0x20);  /* 015312 LDM */
L_015316: TICK();
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 015316 INC */
    goto L_0152F9;  /* 015318 JMP */
L_01531B: TICK();
    mflag = 0;  /* 01531B CLM */
    Y = pull16();  /* 01531C PLY */
    D = pull16();  /* 01531D PLD */
    S += 3; return;  /* 01531E RTL */
L_01531F: TICK();
    /* 01531F CMP */
    if ((A & 0xFF) != 0x01) goto L_01533A;  /* 015321 BNE */
    mflag = 0;  /* 015323 CLM */
    X = rd16(ABS(0x01CD));  /* 015324 LDX */
    wr16(DP(0x07), X);  /* 015327 STX */
    mflag = 1;  /* 015329 SEM */
    SETLO(A, rd8(ABS(0x03A5)));  /* 01532A LDA */
    if ((A & 0xFF) == 0) goto L_015337;  /* 01532D BEQ */
    wr8(ABS(0x0252), 0x46);  /* 01532F LDM */
    wr8(ABS(0x0253), 0x42);  /* 015333 LDM */
L_015337: TICK();
    goto L_015366;  /* 015337 JMP */
L_01533A: TICK();
    mflag = 0;  /* 01533A CLM */
    A = rd16(DP(0x00));  /* 01533B LDA */
    mflag = 1;  /* 01533D SEM */
    /* 01533E CMP */
    if ((A & 0xFF) != 0x02) goto L_015350;  /* 015340 BNE */
    mflag = 0;  /* 015342 CLM */
    X = rd16(ABS(0x03D5));  /* 015343 LDX */
    wr16(DP(0x07), X);  /* 015346 STX */
    mflag = 1;  /* 015348 SEM */
    wr8(ABS(0x0252), 0x6A);  /* 015349 LDM */
    goto L_015366;  /* 01534D JMP */
L_015350: TICK();
    mflag = 0;  /* 015350 CLM */
    A = rd16(DP(0x00));  /* 015351 LDA */
    mflag = 1;  /* 015353 SEM */
    /* 015354 CMP */
    if ((A & 0xFF) != 0x04) goto L_015366;  /* 015356 BNE */
    mflag = 0;  /* 015358 CLM */
    X = rd16(ABS(0x03D7));  /* 015359 LDX */
    wr16(DP(0x07), X);  /* 01535C STX */
    mflag = 1;  /* 01535E SEM */
    wr8(DP(0x00), 0x02);  /* 01535F LDM */
    wr8(ABS(0x0252), 0x50);  /* 015362 LDM */
L_015366: TICK();
    mflag = 0;  /* 015366 CLM */
    X = rd16(DP(0x07));  /* 015367 LDX */
    /* 015369 CPX */
    if (X < 0x1FA4) goto L_015371;  /* 01536C BCC */
    goto L_015480;  /* 01536E JMP */
L_015371: TICK();
    mflag = 1;  /* 015371 SEM */
    SETLO(A, rd8(ABS(0x01E4)));  /* 015372 LDA */
    if ((A & 0xFF) == 0) goto L_01537A;  /* 015375 BEQ */
    goto L_015480;  /* 015377 JMP */
L_01537A: TICK();
    SETLO(A, rd8(ABS(0x01B8)));  /* 01537A LDA */
    t = rd8(ABS(0x0425));  /* 01537D CMP */
    if ((A & 0xFF) == t) goto L_015385;  /* 015380 BEQ */
    goto L_015480;  /* 015382 JMP */
L_015385: TICK();
    mflag = 0;  /* 015385 CLM */
    C = (X >= 0x1B5A);  /* 015386 CPX */
    if (C) goto L_015404;  /* 015389 BCS */
    mflag = 1;  /* 01538B SEM */
    wr8(ABS(0x0255), 0x32);  /* 01538C LDM */
    mflag = 0;  /* 015390 CLM */
    wr16(DP(0x14), X);  /* 015391 STX */
    A = D;  /* 015393 TDA */
    A = adc16(A, 0x0011);  /* 015394 ADC */
    B = 0x0000;  /* 015397 LDA */
    Y = A;  /* 01539B TAY */
    S -= 3; F_01512B();  /* 01539C JSRL */
    wr16(DP(0x14), A);  /* 0153A0 STA */
    A = D;  /* 0153A2 TDA */
    A += 1;  /* 0153A3 INC */
    wr16(DP(0x16), A);  /* 0153A4 STA */
    mflag = 1;  /* 0153A6 SEM */
    wr8(DP(0x18), (uint8_t)B);  /* 0153A7 STA */
    mflag = 0;  /* 0153AA CLM */
    A = rd16(DP(0x14));  /* 0153AB LDA */
    wr16(DP(0x19), A);  /* 0153AD STA */
    A = rd16(DP(0x18));  /* 0153AF LDA */
    mflag = 1;  /* 0153B1 SEM */
    wr8(DP(0x1B), (uint8_t)A);  /* 0153B2 STA */
    mflag = 0;  /* 0153B4 CLM */
    A = rd16(DP(0x16));  /* 0153B5 LDA */
    wr16(DP(0x1C), A);  /* 0153B7 STA */
    mflag = 1;  /* 0153B9 SEM */
    wr8(DP(0x1E), 0x00);  /* 0153BA LDM */
    mflag = 0;  /* 0153BD CLM */
    A = 0x0019;  /* 0153BE LDA */
    Y = 0x0003;  /* 0153C1 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 0153C4 JSRL */
    A = rd16(DP(0x01));  /* 0153C8 LDA */
    mflag = 1;  /* 0153CA SEM */
    C = ((A & 0xFF) >= 0x0A);  /* 0153CB CMP */
    if (C) goto L_0153DC;  /* 0153CD BCS */
    SETLO(A, rd8(DP(0x01)));  /* 0153CF LDA */
    SETLO(A, adc8(A, 0x30));  /* 0153D1 ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 0153D3 STA */
    wr8(DP(0x06), 0x05);  /* 0153D6 LDM */
    goto L_015401;  /* 0153D9 JMP */
L_0153DC: TICK();
    mflag = 1;  /* 0153DC SEM */
    SETLO(B, 0x00);  /* 0153DD LDA */
    SETLO(A, rd8(DP(0x01)));  /* 0153E0 LDA */
    div8(0x0A);  /* 0153E2 DIV */
    C = 0;  /* 0153E5 CLC */
    SETLO(A, adc8(A, 0x30));  /* 0153E6 ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 0153E8 STA */
    SETLO(B, 0x00);  /* 0153EB LDA */
    SETLO(A, rd8(DP(0x01)));  /* 0153EE LDA */
    div8(0x0A);  /* 0153F0 DIV */
    mflag = 0;  /* 0153F3 CLM */
    push16(B);  /* 0153F4 PHB */
    A = pull16();  /* 0153F6 PLA */
    C = 0;  /* 0153F7 CLC */
    mflag = 1;  /* 0153F8 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0153F9 ADC */
    wr8(ABS(0x0257), (uint8_t)A);  /* 0153FB STA */
    wr8(DP(0x06), 0x06);  /* 0153FE LDM */
L_015401: TICK();
    goto L_01547D;  /* 015401 JMP */
L_015404: TICK();
    mflag = 1;  /* 015404 SEM */
    wr8(ABS(0x0255), 0x39);  /* 015405 LDM */
    mflag = 0;  /* 015409 CLM */
    X = rd16(DP(0x07));  /* 01540A LDX */
    wr16(DP(0x14), X);  /* 01540C STX */
    A = D;  /* 01540E TDA */
    C = 0;  /* 01540F CLC */
    A = adc16(A, 0x0011);  /* 015410 ADC */
    B = 0x0000;  /* 015413 LDA */
    Y = A;  /* 015417 TAY */
    S -= 3; F_01512B();  /* 015418 JSRL */
    wr16(DP(0x14), A);  /* 01541C STA */
    A = D;  /* 01541E TDA */
    A += 1;  /* 01541F INC */
    wr16(DP(0x16), A);  /* 015420 STA */
    mflag = 1;  /* 015422 SEM */
    wr8(DP(0x18), (uint8_t)B);  /* 015423 STA */
    mflag = 0;  /* 015426 CLM */
    A = rd16(DP(0x14));  /* 015427 LDA */
    wr16(DP(0x19), A);  /* 015429 STA */
    A = rd16(DP(0x18));  /* 01542B LDA */
    mflag = 1;  /* 01542D SEM */
    wr8(DP(0x1B), (uint8_t)A);  /* 01542E STA */
    mflag = 0;  /* 015430 CLM */
    A = rd16(DP(0x16));  /* 015431 LDA */
    wr16(DP(0x1C), A);  /* 015433 STA */
    mflag = 1;  /* 015435 SEM */
    wr8(DP(0x1E), 0x00);  /* 015436 LDM */
    mflag = 0;  /* 015439 CLM */
    A = 0x0019;  /* 01543A LDA */
    Y = 0x0003;  /* 01543D LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 015440 JSRL */
    A = rd16(DP(0x01));  /* 015444 LDA */
    mflag = 1;  /* 015446 SEM */
    C = ((A & 0xFF) >= 0x0A);  /* 015447 CMP */
    if (C) goto L_015458;  /* 015449 BCS */
    SETLO(A, rd8(DP(0x01)));  /* 01544B LDA */
    SETLO(A, adc8(A, 0x30));  /* 01544D ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 01544F STA */
    wr8(DP(0x06), 0x05);  /* 015452 LDM */
    goto L_01547D;  /* 015455 JMP */
L_015458: TICK();
    mflag = 1;  /* 015458 SEM */
    SETLO(B, 0x00);  /* 015459 LDA */
    SETLO(A, rd8(DP(0x01)));  /* 01545C LDA */
    div8(0x0A);  /* 01545E DIV */
    C = 0;  /* 015461 CLC */
    SETLO(A, adc8(A, 0x30));  /* 015462 ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 015464 STA */
    SETLO(B, 0x00);  /* 015467 LDA */
    SETLO(A, rd8(DP(0x01)));  /* 01546A LDA */
    div8(0x0A);  /* 01546C DIV */
    mflag = 0;  /* 01546F CLM */
    push16(B);  /* 015470 PHB */
    A = pull16();  /* 015472 PLA */
    C = 0;  /* 015473 CLC */
    mflag = 1;  /* 015474 SEM */
    SETLO(A, adc8(A, 0x30));  /* 015475 ADC */
    wr8(ABS(0x0257), (uint8_t)A);  /* 015477 STA */
    wr8(DP(0x06), 0x06);  /* 01547A LDM */
L_01547D: TICK();
    goto L_015500;  /* 01547D JMP */
L_015480: TICK();
    mflag = 1;  /* 015480 SEM */
    wr8(ABS(0x01C4), 0x23);  /* 015481 LDM */
    mflag = 0;  /* 015485 CLM */
    X = rd16(DP(0x07));  /* 015486 LDX */
    /* 015488 CPX */
    if (X == 0x1FA6) goto L_015498;  /* 01548B BEQ */
    /* 01548D CPX */
    if (X == 0x1FA5) goto L_015498;  /* 015490 BEQ */
    mflag = 1;  /* 015492 SEM */
    SETLO(A, rd8(ABS(0x01E4)));  /* 015493 LDA */
    if ((A & 0xFF) == 0) goto L_0154A0;  /* 015496 BEQ */
L_015498: TICK();
    mflag = 1;  /* 015498 SEM */
    wr8(ABS(0x01C4), 0x0D);  /* 015499 LDM */
    goto L_0154E6;  /* 01549D JMP */
L_0154A0: TICK();
    mflag = 0;  /* 0154A0 CLM */
    X = rd16(DP(0x07));  /* 0154A1 LDX */
    /* 0154A3 CPX */
    if (X != 0x1FFF) goto L_0154B0;  /* 0154A6 BNE */
    mflag = 1;  /* 0154A8 SEM */
    wr8(ABS(0x01C4), 0x0E);  /* 0154A9 LDM */
    goto L_0154E6;  /* 0154AD JMP */
L_0154B0: TICK();
    mflag = 0;  /* 0154B0 CLM */
    X = rd16(DP(0x07));  /* 0154B1 LDX */
    /* 0154B3 CPX */
    if (X == 0x1FFD) goto L_0154C1;  /* 0154B6 BEQ */
    A = rd16(ABS(0x01B8));  /* 0154B8 LDA */
    mflag = 1;  /* 0154BB SEM */
    t = rd8(ABS(0x0425));  /* 0154BC CMP */
    if ((A & 0xFF) == t) goto L_0154C9;  /* 0154BF BEQ */
L_0154C1: TICK();
    mflag = 1;  /* 0154C1 SEM */
    wr8(ABS(0x01C4), 0x0F);  /* 0154C2 LDM */
    goto L_0154E6;  /* 0154C6 JMP */
L_0154C9: TICK();
    mflag = 0;  /* 0154C9 CLM */
    X = rd16(DP(0x07));  /* 0154CA LDX */
    /* 0154CC CPX */
    if (X != 0x1FFC) goto L_0154D9;  /* 0154CF BNE */
    mflag = 1;  /* 0154D1 SEM */
    wr8(ABS(0x01C4), 0x10);  /* 0154D2 LDM */
    goto L_0154E6;  /* 0154D6 JMP */
L_0154D9: TICK();
    mflag = 0;  /* 0154D9 CLM */
    X = rd16(DP(0x07));  /* 0154DA LDX */
    /* 0154DC CPX */
    if (X != 0x1FFA) goto L_0154E6;  /* 0154DF BNE */
    mflag = 1;  /* 0154E1 SEM */
    wr8(ABS(0x01C4), 0x11);  /* 0154E2 LDM */
L_0154E6: TICK();
    mflag = 0;  /* 0154E6 CLM */
    A = rd16(ABS(0x01C4));  /* 0154E7 LDA */
    mflag = 1;  /* 0154EA SEM */
    /* 0154EB CMP */
    if ((A & 0xFF) == 0x23) goto L_0154FA;  /* 0154ED BEQ */
    Y = rd16(ABS(0x01C4));  /* 0154EF LDY */
    mflag = 0;  /* 0154F2 CLM */
    S -= 3; F_003310();  /* 0154F3 JSRL */
    Y = pull16();  /* 0154F7 PLY */
    D = pull16();  /* 0154F8 PLD */
    S += 3; return;  /* 0154F9 RTL */
L_0154FA: TICK();
    wr8(DP(0x06), 0x00);  /* 0154FA LDM */
    mflag = 0;  /* 0154FD CLM */
    wr16(DP(0x02), X);  /* 0154FE STX */
L_015500: TICK();
    mflag = 0;  /* 015500 CLM */
    A = rd16(DP(0x06));  /* 015501 LDA */
    mflag = 1;  /* 015503 SEM */
    /* 015504 CMP */
    if ((A & 0xFF) != 0x06) goto L_015511;  /* 015506 BNE */
    wr8(DP(0x04), 0x01);  /* 015508 LDM */
    wr8(DP(0x05), 0x03);  /* 01550B LDM */
    goto L_01552D;  /* 01550E JMP */
L_015511: TICK();
    mflag = 0;  /* 015511 CLM */
    A = rd16(DP(0x06));  /* 015512 LDA */
    mflag = 1;  /* 015514 SEM */
    /* 015515 CMP */
    if ((A & 0xFF) != 0x05) goto L_015522;  /* 015517 BNE */
    wr8(DP(0x04), 0x02);  /* 015519 LDM */
    wr8(DP(0x05), 0x02);  /* 01551C LDM */
    goto L_01552D;  /* 01551F JMP */
L_015522: TICK();
    mflag = 1;  /* 015522 SEM */
    SETLO(A, rd8(DP(0x06)));  /* 015523 LDA */
    if ((A & 0xFF) != 0) goto L_01552D;  /* 015525 BNE */
    wr8(DP(0x04), 0x03);  /* 015527 LDM */
    wr8(DP(0x05), 0x01);  /* 01552A LDM */
L_01552D: TICK();
    mflag = 0;  /* 01552D CLM */
    A = D;  /* 01552E TDA */
    C = 0;  /* 01552F CLC */
    A = adc16(A, 0x0009);  /* 015530 ADC */
    B = 0x0000;  /* 015533 LDA */
    wr16(DP(0x14), A);  /* 015537 STA */
    wr16(DP(0x16), B);  /* 015539 STA */
    Y = rd16(DP(0x02));  /* 01553C LDY */
    S -= 3; F_006948();  /* 01553E JSRL */
L_015542: TICK();
    A = rd16(DP(0x04));  /* 015542 LDA */
    mflag = 1;  /* 015544 SEM */
    /* 015545 CMP */
    if ((A & 0xFF) == 0xFF) goto L_015568;  /* 015547 BEQ */
    SETLO(A, rd8(DP(0x05)));  /* 015549 LDA */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 01554B INC */
    mflag = 0;  /* 01554D CLM */
    A &= 0x00FF;  /* 01554E AND */
    X = A;  /* 015551 TAX */
    A = rd16(DP(0x06));  /* 015552 LDA */
    mflag = 1;  /* 015554 SEM */
    t = (rd8(DP(0x06)) + 1) & 0xFF; wr8(DP(0x06), t);  /* 015555 INC */
    mflag = 0;  /* 015557 CLM */
    A &= 0x00FF;  /* 015558 AND */
    Y = A;  /* 01555B TAY */
    A = rd16(DP(0x09 + X));  /* 01555C LDA */
    mflag = 1;  /* 01555E SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 01555F STA */
    t = (rd8(DP(0x04)) - 1) & 0xFF; wr8(DP(0x04), t);  /* 015562 DEC */
    mflag = 0;  /* 015564 CLM */
    goto L_015542;  /* 015565 JMP */
L_015568: TICK();
    mflag = 0;  /* 015568 CLM */
    Y = pull16();  /* 015569 PLY */
    D = pull16();  /* 01556A PLD */
    S += 3; return;  /* 01556B RTL */
}

/* 01556C  callers: 015714, 016270 */
void F_01556C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01556C LDA */
    push16(A);  /* 01556E PHA */
    X = rd16(ABS(0x03D5));  /* 01556F LDX */
    if (X != 0) goto L_015588;  /* 015572 BNE */
    mflag = 1;  /* 015574 SEM */
    wr8(ABS(0x01B6), 0x08);  /* 015575 LDM */
    wr8(ABS(0x01B7), 0x10);  /* 015579 LDM */
    wr8(ABS(0x01C4), 0x17);  /* 01557D LDM */
    wr8(ABS(0x01BA), 0x02);  /* 015581 LDM */
    goto L_0155A4;  /* 015585 JMP */
L_015588: TICK();
    mflag = 1;  /* 015588 SEM */
    wr8(ABS(0x0293), 0x00);  /* 015589 LDM */
    wr8(ABS(0x01B6), 0x00);  /* 01558D LDM */
    mflag = 0;  /* 015591 CLM */
    X = rd16(ABS(0x03D5));  /* 015592 LDX */
    wr16(ABS(0x01CD), X);  /* 015595 STX */
    Y = 0x0002;  /* 015598 LDY */
    S -= 3; F_01520C();  /* 01559B JSRL */
    mflag = 1;  /* 01559F SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0155A0 LDM */
L_0155A4: TICK();
    wr8(ABS(0x0227), 0x00);  /* 0155A4 LDM */
    mflag = 0;  /* 0155A8 CLM */
    Y = pull16();  /* 0155A9 PLY */
    S += 3; return;  /* 0155AA RTL */
}

/* 0155AB  callers: 015725, 016281 */
void F_0155AB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0155AB LDA */
    push16(A);  /* 0155AD PHA */
    mflag = 1;  /* 0155AE SEM */
    SETLO(A, rd8(ABS(0x0293)));  /* 0155AF LDA */
    if ((A & 0xFF) == 0) goto L_0155BA;  /* 0155B2 BEQ */
    wr8(ABS(0x01C3), (uint8_t)A);  /* 0155B4 STA */
    goto L_0155C2;  /* 0155B7 JMP */
L_0155BA: TICK();
    mflag = 0;  /* 0155BA CLM */
    A = rd16(ABS(0x01C3));  /* 0155BB LDA */
    mflag = 1;  /* 0155BE SEM */
    wr8(ABS(0x0293), (uint8_t)A);  /* 0155BF STA */
L_0155C2: TICK();
    /* 0155C2 CMP */
    if ((A & 0xFF) != 0x00) goto L_0155D9;  /* 0155C4 BNE */
    wr8(ABS(0x01B6), 0x08);  /* 0155C6 LDM */
    wr8(ABS(0x01B7), 0x10);  /* 0155CA LDM */
    wr8(ABS(0x01C4), 0x17);  /* 0155CE LDM */
    wr8(ABS(0x01BA), 0x02);  /* 0155D2 LDM */
    goto L_0155E6;  /* 0155D6 JMP */
L_0155D9: TICK();
    Y = 0x0003;  /* 0155D9 LDY */
    mflag = 0;  /* 0155DC CLM */
    S -= 3; F_01520C();  /* 0155DD JSRL */
    mflag = 1;  /* 0155E1 SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0155E2 LDM */
L_0155E6: TICK();
    wr8(ABS(0x0227), 0x00);  /* 0155E6 LDM */
    mflag = 0;  /* 0155EA CLM */
    Y = pull16();  /* 0155EB PLY */
    S += 3; return;  /* 0155EC RTL */
}

/* 0155ED  callers: 0156CB, 015730, 015F88, 0161A2, 016221, 01624F */
void F_0155ED(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0155ED LDA */
    push16(D);  /* 0155EF PHD */
    D = A;  /* 0155F0 TAD */
    A += 1;  /* 0155F1 INC */
    A += 1;  /* 0155F2 INC */
    push16(A);  /* 0155F3 PHA */
    wr16(DP(0x00), Y);  /* 0155F4 STY */
    mflag = 1;  /* 0155F6 SEM */
    wr8(ABS(0x0293), 0x00);  /* 0155F7 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015606;  /* 0155FB BBS */
    SETLO(A, Y);  /* 015600 TYA */
    if ((A & 0xFF) != 0) goto L_015606;  /* 015601 BNE */
    wr8(DP(0x00), 0x06);  /* 015603 LDM */
L_015606: TICK();
    wr8(DP(0x01), 0x00);  /* 015606 LDM */
L_015609: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 015609 LDA */
    /* 01560B CMP */
    if ((A & 0xFF) < 0x0B) goto L_015612;  /* 01560D BCC */
    goto L_0156AD;  /* 01560F JMP */
L_015612: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 015612 LDA */
    /* 015614 CMP */
    if ((A & 0xFF) != 0x05) goto L_01561E;  /* 015616 BNE */
    t = (rd8(ABS(0x01C1)) + 1) & 0xFF; wr8(ABS(0x01C1), t);  /* 015618 INC */
    goto L_01563D;  /* 01561B JMP */
L_01561E: TICK();
    mflag = 0;  /* 01561E CLM */
    A = rd16(DP(0x00));  /* 01561F LDA */
    mflag = 1;  /* 015621 SEM */
    /* 015622 CMP */
    if ((A & 0xFF) != 0x06) goto L_015639;  /* 015624 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015636;  /* 015626 BBS */
    SETLO(A, rd8(ABS(0x0206)));  /* 01562B LDA */
    /* 01562E CMP */
    if ((A & 0xFF) >= 0x0D) goto L_015636;  /* 015630 BCS */
    SETLO(A, A - 1);  /* 015632 DEC */
    wr8(ABS(0x01C1), (uint8_t)A);  /* 015633 STA */
L_015636: TICK();
    goto L_01563D;  /* 015636 JMP */
L_015639: TICK();
    mflag = 1;  /* 015639 SEM */
    wr8(DP(0x00), 0x05);  /* 01563A LDM */
L_01563D: TICK();
    mflag = 0;  /* 01563D CLM */
    if ((rd16(ABS(0x039E)) & 0x4000) == 0) goto L_01564A;  /* 01563E BBC */
    A = 0x000C;  /* 015644 LDA */
    goto L_01564D;  /* 015647 JMP */
L_01564A: TICK();
    A = 0x000A;  /* 01564A LDA */
L_01564D: TICK();
    mflag = 1;  /* 01564D SEM */
    SETLO(B, 0x00);  /* 01564E LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 015651 STA */
    SETLO(A, rd8(ABS(0x01C1)));  /* 015653 LDA */
    div8(rd8(DP(0x02)));  /* 015656 DIV */
    wr8(ABS(0x01C1), (uint8_t)B);  /* 015659 STA */
    /* 01565D CMP */
    if ((B & 0xFF) < 0x0A) goto L_01566B;  /* 015660 BCC */
    mflag = 0;  /* 015662 CLM */
    wr16(ABS(0x01CD), 0x0000);  /* 015663 LDM */
    goto L_0156AD;  /* 015668 JMP */
L_01566B: TICK();
    mflag = 0;  /* 01566B CLM */
    A = rd16(ABS(0x01C1));  /* 01566C LDA */
    mflag = 1;  /* 01566F SEM */
    mpy8(0x03);  /* 015670 MPY */
    wr8(DP(0x02), (uint8_t)A);  /* 015673 STA */
    wr8(DP(0x03), (uint8_t)B);  /* 015675 STA */
    mflag = 0;  /* 015678 CLM */
    A = rd16(DP(0x02));  /* 015679 LDA */
    C = 0;  /* 01567B CLC */
    A = adc16(A, 0x01F8);  /* 01567C ADC */
    Y = A;  /* 01567F TAY */
    S -= 3; eeprom_read_byte_007189();  /* 015680 JSRL */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 015684 STA */
    SETLO(A, rd8(ABS(0x01C1)));  /* 015687 LDA */
    mpy8(0x03);  /* 01568A MPY */
    wr8(DP(0x02), (uint8_t)A);  /* 01568D STA */
    wr8(DP(0x03), (uint8_t)B);  /* 01568F STA */
    mflag = 0;  /* 015692 CLM */
    A = rd16(DP(0x02));  /* 015693 LDA */
    C = 0;  /* 015695 CLC */
    A = adc16(A, 0x01F9);  /* 015696 ADC */
    Y = A;  /* 015699 TAY */
    S -= 3; F_007271();  /* 01569A JSRL */
    wr16(ABS(0x01CD), A);  /* 01569E STA */
    X = A;  /* 0156A1 TAX */
    if (X == 0) goto L_0156A7;  /* 0156A2 BEQ */
    goto L_0156AD;  /* 0156A4 JMP */
L_0156A7: TICK();
    mflag = 1;  /* 0156A7 SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0156A8 INC */
    goto L_015609;  /* 0156AA JMP */
L_0156AD: TICK();
    mflag = 0;  /* 0156AD CLM */
    Y = pull16();  /* 0156AE PLY */
    D = pull16();  /* 0156AF PLD */
    S += 3; return;  /* 0156B0 RTL */
}

/* 0156B1  callers: 01686C */
void F_0156B1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0156B1 LDA */
    push16(A);  /* 0156B3 PHA */
    Y = 0x013A;  /* 0156B4 LDY */
    S -= 3; F_007271();  /* 0156B7 JSRL */
    wr16(ABS(0x01D9), A);  /* 0156BB STA */
    Y = 0x013C;  /* 0156BE LDY */
    S -= 3; F_007271();  /* 0156C1 JSRL */
    wr16(ABS(0x01DB), A);  /* 0156C5 STA */
    Y = 0x0000;  /* 0156C8 LDY */
    S -= 3; F_0155ED();  /* 0156CB JSRL */
    X = rd16(ABS(0x01CD));  /* 0156CF LDX */
    if (X != 0) goto L_0156DA;  /* 0156D2 BNE */
    A = 0x0001;  /* 0156D4 LDA */
    goto L_0156DD;  /* 0156D7 JMP */
L_0156DA: TICK();
    A = 0x0000;  /* 0156DA LDA */
L_0156DD: TICK();
    mflag = 1;  /* 0156DD SEM */
    wr8(ABS(0x01E5), (uint8_t)A);  /* 0156DE STA */
    mflag = 0;  /* 0156E1 CLM */
    Y = pull16();  /* 0156E2 PLY */
    S += 3; return;  /* 0156E3 RTL */
}

/* 0156E4  callers: 015753, 01577F, 0157D2, 016314, 0165FD, 0166D0 */
void F_0156E4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0156E4 LDA */
    push16(A);  /* 0156E6 PHA */
    mflag = 1;  /* 0156E7 SEM */
    wr8(ABS(0x0227), 0x00);  /* 0156E8 LDM */
    SETLO(A, rd8(ABS(0x03BA)));  /* 0156EC LDA */
    /* 0156EF CMP */
    if ((A & 0xFF) < 0x40) goto L_0156FE;  /* 0156F1 BCC */
    wr8(ABS(0x01C4), 0x1B);  /* 0156F3 LDM */
    wr8(ABS(0x01BA), 0x02);  /* 0156F7 LDM */
    goto L_015740;  /* 0156FB JMP */
L_0156FE: TICK();
    mflag = 1;  /* 0156FE SEM */
    SETLO(A, rd8(ABS(0x01E5)));  /* 0156FF LDA */
    if ((A & 0xFF) == 0) goto L_01570A;  /* 015702 BEQ */
    mflag = 0;  /* 015704 CLM */
    wr16(ABS(0x01CD), 0x0000);  /* 015705 LDM */
L_01570A: TICK();
    mflag = 0;  /* 01570A CLM */
    A = rd16(ABS(0x0206));  /* 01570B LDA */
    mflag = 1;  /* 01570E SEM */
    /* 01570F CMP */
    if ((A & 0xFF) != 0x0D) goto L_01571B;  /* 015711 BNE */
    mflag = 0;  /* 015713 CLM */
    S -= 3; F_01556C();  /* 015714 JSRL */
    goto L_015740;  /* 015718 JMP */
L_01571B: TICK();
    mflag = 0;  /* 01571B CLM */
    A = rd16(ABS(0x0206));  /* 01571C LDA */
    mflag = 1;  /* 01571F SEM */
    /* 015720 CMP */
    if ((A & 0xFF) != 0x0E) goto L_01572C;  /* 015722 BNE */
    mflag = 0;  /* 015724 CLM */
    S -= 3; F_0155AB();  /* 015725 JSRL */
    goto L_015740;  /* 015729 JMP */
L_01572C: TICK();
    Y = 0x0000;  /* 01572C LDY */
    mflag = 0;  /* 01572F CLM */
    S -= 3; F_0155ED();  /* 015730 JSRL */
    Y = 0x0001;  /* 015734 LDY */
    S -= 3; F_01520C();  /* 015737 JSRL */
    mflag = 1;  /* 01573B SEM */
    wr8(ABS(0x01BA), 0x03);  /* 01573C LDM */
L_015740: TICK();
    mflag = 0;  /* 015740 CLM */
    Y = pull16();  /* 015741 PLY */
    S += 3; return;  /* 015742 RTL */
}

/* 015743  callers: 016731 */
void F_015743(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 015743 LDA */
    push16(A);  /* 015745 PHA */
    mflag = 1;  /* 015746 SEM */
    wr8(ABS(0x01BA), 0x00);  /* 015747 LDM */
    SETLO(A, rd8(ABS(0x01B6)));  /* 01574B LDA */
    /* 01574E CMP */
    if ((A & 0xFF) != 0x14) goto L_015775;  /* 015750 BNE */
    mflag = 0;  /* 015752 CLM */
    S -= 3; F_0156E4();  /* 015753 JSRL */
    A = rd16(ABS(0x0293));  /* 015757 LDA */
    mflag = 1;  /* 01575A SEM */
    wr8(ABS(0x01C3), (uint8_t)A);  /* 01575B STA */
    wr8(ABS(0x0293), 0x00);  /* 01575E LDM */
    wr8(ABS(0x01B6), 0x00);  /* 015762 LDM */
    wr8(ABS(0x0227), 0x00);  /* 015766 LDM */
    Y = 0x0013;  /* 01576A LDY */
    mflag = 0;  /* 01576D CLM */
    S -= 3; F_0080AA();  /* 01576E JSRL */
    goto L_0157DF;  /* 015772 JMP */
L_015775: TICK();
    mflag = 0;  /* 015775 CLM */
    A = rd16(ABS(0x01B6));  /* 015776 LDA */
    mflag = 1;  /* 015779 SEM */
    /* 01577A CMP */
    if ((A & 0xFF) != 0x08) goto L_015797;  /* 01577C BNE */
    mflag = 0;  /* 01577E CLM */
    S -= 3; F_0156E4();  /* 01577F JSRL */
    mflag = 1;  /* 015783 SEM */
    wr8(ABS(0x0293), 0x00);  /* 015784 LDM */
    wr8(ABS(0x01B6), 0x00);  /* 015788 LDM */
    Y = 0x0013;  /* 01578C LDY */
    mflag = 0;  /* 01578F CLM */
    S -= 3; F_0080AA();  /* 015790 JSRL */
    goto L_0157DF;  /* 015794 JMP */
L_015797: TICK();
    mflag = 0;  /* 015797 CLM */
    A = rd16(ABS(0x01B6));  /* 015798 LDA */
    mflag = 1;  /* 01579B SEM */
    /* 01579C CMP */
    if ((A & 0xFF) != 0x04) goto L_0157AB;  /* 01579E BNE */
    wr8(ABS(0x01B7), 0x20);  /* 0157A0 LDM */
    wr8(ABS(0x01BA), 0x03);  /* 0157A4 LDM */
    goto L_0157DF;  /* 0157A8 JMP */
L_0157AB: TICK();
    mflag = 0;  /* 0157AB CLM */
    A = rd16(ABS(0x01B6));  /* 0157AC LDA */
    mflag = 1;  /* 0157AF SEM */
    /* 0157B0 CMP */
    if ((A & 0xFF) != 0x05) goto L_0157C8;  /* 0157B2 BNE */
    Y = 0x0000;  /* 0157B4 LDY */
    mflag = 0;  /* 0157B7 CLM */
    S -= 3; F_0080AA();  /* 0157B8 JSRL */
    mflag = 1;  /* 0157BC SEM */
    wr8(ABS(0x01B7), 0x5E);  /* 0157BD LDM */
    wr8(ABS(0x01BA), 0x00);  /* 0157C1 LDM */
    goto L_0157DF;  /* 0157C5 JMP */
L_0157C8: TICK();
    mflag = 0;  /* 0157C8 CLM */
    A = rd16(ABS(0x01B6));  /* 0157C9 LDA */
    mflag = 1;  /* 0157CC SEM */
    /* 0157CD CMP */
    if ((A & 0xFF) != 0x06) goto L_0157DF;  /* 0157CF BNE */
    mflag = 0;  /* 0157D1 CLM */
    S -= 3; F_0156E4();  /* 0157D2 JSRL */
    mflag = 1;  /* 0157D6 SEM */
    wr8(ABS(0x01B6), 0x00);  /* 0157D7 LDM */
    wr8(ABS(0x01BA), 0x03);  /* 0157DB LDM */
L_0157DF: TICK();
    mflag = 0;  /* 0157DF CLM */
    A = rd16(ABS(0x01B6));  /* 0157E0 LDA */
    mflag = 1;  /* 0157E3 SEM */
    /* 0157E4 CMP */
    if ((A & 0xFF) != 0x09) goto L_01583A;  /* 0157E6 BNE */
    SETLO(A, rd8(ABS(0x041E)));  /* 0157E8 LDA */
    if ((A & 0xFF) == 0) goto L_01582A;  /* 0157EB BEQ */
    SETLO(A, rd8(ABS(0x01E7)));  /* 0157ED LDA */
    if ((A & 0xFF) == 0) goto L_01582A;  /* 0157F0 BEQ */
    wr8(ABS(0x01B7), 0x02);  /* 0157F2 LDM */
    t = (rd8(ABS(0x01BC)) + 1) & 0xFF; wr8(ABS(0x01BC), t);  /* 0157F6 INC */
    SETLO(A, rd8(ABS(0x01BC)));  /* 0157F9 LDA */
    /* 0157FC CMP */
    if ((A & 0xFF) < 0x19) goto L_015804;  /* 0157FE BCC */
    wr8(ABS(0x01BC), 0x00);  /* 015800 LDM */
L_015804: TICK();
    SETLO(A, rd8(ABS(0x01BC)));  /* 015804 LDA */
    /* 015807 CMP */
    if ((A & 0xFF) < 0x03) goto L_015813;  /* 015809 BCC */
    /* 01580B CMP */
    if ((A & 0xFF) < 0x06) goto L_015822;  /* 01580D BCC */
    /* 01580F CMP */
    if ((A & 0xFF) >= 0x09) goto L_015822;  /* 015811 BCS */
L_015813: TICK();
    wr8(ABS(0x01B6), 0x10);  /* 015813 LDM */
    Y = 0x0014;  /* 015817 LDY */
    mflag = 0;  /* 01581A CLM */
    S -= 3; F_0080AA();  /* 01581B JSRL */
    goto L_015827;  /* 01581F JMP */
L_015822: TICK();
    mflag = 1;  /* 015822 SEM */
    wr8(ABS(0x01B7), 0x04);  /* 015823 LDM */
L_015827: TICK();
    goto L_015837;  /* 015827 JMP */
L_01582A: TICK();
    mflag = 1;  /* 01582A SEM */
    wr8(ABS(0x01B6), 0x00);  /* 01582B LDM */
    wr8(ABS(0x01B7), 0x00);  /* 01582F LDM */
    wr8(ABS(0x01E7), 0x00);  /* 015833 LDM */
L_015837: TICK();
    goto L_0158D4;  /* 015837 JMP */
L_01583A: TICK();
    mflag = 0;  /* 01583A CLM */
    A = rd16(ABS(0x01B6));  /* 01583B LDA */
    mflag = 1;  /* 01583E SEM */
    /* 01583F CMP */
    if ((A & 0xFF) != 0x10) goto L_015856;  /* 015841 BNE */
    wr8(ABS(0x01B7), 0x02);  /* 015843 LDM */
    wr8(ABS(0x01B6), 0x09);  /* 015847 LDM */
    Y = 0x0015;  /* 01584B LDY */
    mflag = 0;  /* 01584E CLM */
    S -= 3; F_0080AA();  /* 01584F JSRL */
    goto L_0158D4;  /* 015853 JMP */
L_015856: TICK();
    mflag = 0;  /* 015856 CLM */
    A = rd16(ABS(0x01B6));  /* 015857 LDA */
    mflag = 1;  /* 01585A SEM */
    /* 01585B CMP */
    if ((A & 0xFF) != 0x0F) goto L_01587A;  /* 01585D BNE */
    SETLO(A, rd8(ABS(0x0420)));  /* 01585F LDA */
    if ((A & 0xFF) != 0) goto L_01586A;  /* 015862 BNE */
    wr8(ABS(0x01B6), (uint8_t)A);  /* 015864 STA */
    goto L_015877;  /* 015867 JMP */
L_01586A: TICK();
    Y = 0x0016;  /* 01586A LDY */
    mflag = 0;  /* 01586D CLM */
    S -= 3; F_0080AA();  /* 01586E JSRL */
    mflag = 1;  /* 015872 SEM */
    wr8(ABS(0x01B7), 0x20);  /* 015873 LDM */
L_015877: TICK();
    goto L_0158D4;  /* 015877 JMP */
L_01587A: TICK();
    mflag = 0;  /* 01587A CLM */
    A = rd16(ABS(0x01B6));  /* 01587B LDA */
    mflag = 1;  /* 01587E SEM */
    /* 01587F CMP */
    if ((A & 0xFF) != 0x11) goto L_0158BB;  /* 015881 BNE */
    wr8(ABS(0x01B7), 0x02);  /* 015883 LDM */
    t = (rd8(ABS(0x01BC)) + 1) & 0xFF; wr8(ABS(0x01BC), t);  /* 015887 INC */
    SETLO(A, rd8(ABS(0x01BC)));  /* 01588A LDA */
    /* 01588D CMP */
    if ((A & 0xFF) < 0x19) goto L_015895;  /* 01588F BCC */
    wr8(ABS(0x01BC), 0x00);  /* 015891 LDM */
L_015895: TICK();
    SETLO(A, rd8(ABS(0x01BC)));  /* 015895 LDA */
    /* 015898 CMP */
    if ((A & 0xFF) < 0x03) goto L_0158A4;  /* 01589A BCC */
    /* 01589C CMP */
    if ((A & 0xFF) < 0x06) goto L_0158B3;  /* 01589E BCC */
    /* 0158A0 CMP */
    if ((A & 0xFF) >= 0x09) goto L_0158B3;  /* 0158A2 BCS */
L_0158A4: TICK();
    wr8(ABS(0x01B6), 0x12);  /* 0158A4 LDM */
    Y = 0x0017;  /* 0158A8 LDY */
    mflag = 0;  /* 0158AB CLM */
    S -= 3; F_0080AA();  /* 0158AC JSRL */
    goto L_0158B8;  /* 0158B0 JMP */
L_0158B3: TICK();
    mflag = 1;  /* 0158B3 SEM */
    wr8(ABS(0x01B7), 0x04);  /* 0158B4 LDM */
L_0158B8: TICK();
    goto L_0158D4;  /* 0158B8 JMP */
L_0158BB: TICK();
    mflag = 0;  /* 0158BB CLM */
    A = rd16(ABS(0x01B6));  /* 0158BC LDA */
    mflag = 1;  /* 0158BF SEM */
    /* 0158C0 CMP */
    if ((A & 0xFF) != 0x12) goto L_0158D4;  /* 0158C2 BNE */
    wr8(ABS(0x01B7), 0x02);  /* 0158C4 LDM */
    wr8(ABS(0x01B6), 0x11);  /* 0158C8 LDM */
    Y = 0x0018;  /* 0158CC LDY */
    mflag = 0;  /* 0158CF CLM */
    S -= 3; F_0080AA();  /* 0158D0 JSRL */
L_0158D4: TICK();
    mflag = 0;  /* 0158D4 CLM */
    Y = pull16();  /* 0158D5 PLY */
    S += 3; return;  /* 0158D6 RTL */
}

/* 0158D7  callers: 015EF7 */
void F_0158D7(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0158D7 LDA */
    push16(D);  /* 0158D9 PHD */
    D = A;  /* 0158DA TAD */
    A += 1;  /* 0158DB INC */
    push16(A);  /* 0158DC PHA */
    mflag = 1;  /* 0158DD SEM */
    if ((rd8(ABS(0x023A)) & 0x02) == 0) goto L_0158E8;  /* 0158DE BBC */
    mflag = 0;  /* 0158E3 CLM */
    S -= 3; F_0050DA();  /* 0158E4 JSRL */
L_0158E8: TICK();
    mflag = 0;  /* 0158E8 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_015924;  /* 0158E9 BBS */
    A = rd16(ABS(0x0226));  /* 0158EF LDA */
    mflag = 1;  /* 0158F2 SEM */
    /* 0158F3 CMP */
    if ((A & 0xFF) == 0x0F) goto L_015921;  /* 0158F5 BEQ */
    mflag = 0;  /* 0158F7 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 0158F8 SEB */
    mflag = 1;  /* 0158FD SEM */
    wr8(ABS(0x0232), 0x3F);  /* 0158FE LDM */
    mflag = 0;  /* 015902 CLM */
    S -= 3; F_007A06();  /* 015903 JSRL */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0) goto L_015917;  /* 015907 BBC */
    Y = 0x000D;  /* 01590D LDY */
    S -= 3; F_0080AA();  /* 015910 JSRL */
    goto L_01591C;  /* 015914 JMP */
L_015917: TICK();
    mflag = 0;  /* 015917 CLM */
    S -= 3; F_0053A4();  /* 015918 JSRL */
L_01591C: TICK();
    mflag = 1;  /* 01591C SEM */
    wr8(ABS(0x0293), 0x00);  /* 01591D LDM */
L_015921: TICK();
    goto L_015B5D;  /* 015921 JMP */
L_015924: TICK();
    mflag = 1;  /* 015924 SEM */
    if ((rd8(ABS(0x0242)) & 0x02) == 0) goto L_015962;  /* 015925 BBC */
    SETLO(A, rd8(ABS(0x0233)));  /* 01592A LDA */
    if ((A & 0xFF) != 0) goto L_015962;  /* 01592D BNE */
    mflag = 0;  /* 01592F CLM */
    S -= 3; F_0050DA();  /* 015930 JSRL */
    mflag = 1;  /* 015934 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015959;  /* 015935 BBS */
    mflag = 0;  /* 01593A CLM */
    if ((rd16(ABS(0x0242)) & 0x0020) == 0) goto L_015959;  /* 01593B BBC */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0020);  /* 015941 CLB */
    A = rd16(ABS(0x0226));  /* 015946 LDA */
    mflag = 1;  /* 015949 SEM */
    /* 01594A CMP */
    if ((A & 0xFF) != 0x71) goto L_015954;  /* 01594C BNE */
    mflag = 0;  /* 01594E CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0010);  /* 01594F CLB */
L_015954: TICK();
    mflag = 1;  /* 015954 SEM */
    wr8(ABS(0x0226), 0x71);  /* 015955 LDM */
L_015959: TICK();
    mflag = 0;  /* 015959 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0002);  /* 01595A CLB */
    goto L_015ADE;  /* 01595F JMP */
L_015962: TICK();
    mflag = 0;  /* 015962 CLM */
    A = rd16(ABS(0x0226));  /* 015963 LDA */
    mflag = 1;  /* 015966 SEM */
    /* 015967 CMP */
    if ((A & 0xFF) != 0x0F) goto L_01596E;  /* 015969 BNE */
    goto L_015ADE;  /* 01596B JMP */
L_01596E: TICK();
    mflag = 0;  /* 01596E CLM */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0) goto L_015978;  /* 01596F BBC */
    goto L_015AB2;  /* 015975 JMP */
L_015978: TICK();
    A = rd16(ABS(0x0226));  /* 015978 LDA */
    mflag = 1;  /* 01597B SEM */
    /* 01597C CMP */
    if ((A & 0xFF) == 0x6D) goto L_015983;  /* 01597E BEQ */
    goto L_015A27;  /* 015980 JMP */
L_015983: TICK();
    SETLO(A, rd8(ABS(0x01B6)));  /* 015983 LDA */
    if ((A & 0xFF) == 0) goto L_01598C;  /* 015986 BEQ */
    /* 015988 CMP */
    if ((A & 0xFF) != 0x14) goto L_0159DE;  /* 01598A BNE */
L_01598C: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 01598C LDA */
    mflag = 0;  /* 01598F CLM */
    A &= 0x00FF;  /* 015990 AND */
    X = A;  /* 015993 TAX */
    A = rd16((0x0023E9 + X));  /* 015994 LDA */
    mflag = 1;  /* 015998 SEM */
    wr8(ABS(0x0227), (uint8_t)A);  /* 015999 STA */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_0159A9;  /* 01599C BBC */
    /* 0159A1 CMP */
    if ((A & 0xFF) != 0x0B) goto L_0159A9;  /* 0159A3 BNE */
    wr8(ABS(0x0227), 0x0E);  /* 0159A5 LDM */
L_0159A9: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 0159A9 LDA */
    /* 0159AC CMP */
    if ((A & 0xFF) != 0x11) goto L_0159B9;  /* 0159AE BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0159B9;  /* 0159B0 BBS */
    wr8(ABS(0x0227), 0x04);  /* 0159B5 LDM */
L_0159B9: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 0159B9 LDA */
    /* 0159BC CMP */
    if ((A & 0xFF) != 0x04) goto L_0159CE;  /* 0159BE BNE */
    if ((rd8(ABS(0x0334)) & 0x20) == 0) goto L_0159CA;  /* 0159C0 BBC */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0159CE;  /* 0159C5 BBS */
L_0159CA: TICK();
    wr8(ABS(0x0227), 0x08);  /* 0159CA LDM */
L_0159CE: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 0159CE LDA */
    /* 0159D1 CMP */
    if ((A & 0xFF) != 0x08) goto L_0159DE;  /* 0159D3 BNE */
    if ((rd8(ABS(0x0340)) & 0x20) == 0x20) goto L_0159DE;  /* 0159D5 BBS */
    wr8(ABS(0x0227), 0x00);  /* 0159DA LDM */
L_0159DE: TICK();
    Y = 0x002B;  /* 0159DE LDY */
    mflag = 0;  /* 0159E1 CLM */
    S -= 3; F_003310();  /* 0159E2 JSRL */
    mflag = 1;  /* 0159E6 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 0159E7 LDA */
    if ((A & 0xFF) == 0) goto L_015A09;  /* 0159EA BEQ */
    mflag = 0;  /* 0159EC CLM */
    A &= 0x00FF;  /* 0159ED AND */
    X = A;  /* 0159F0 TAX */
    A = rd16((0x0023E9 + X));  /* 0159F1 LDA */
    mflag = 1;  /* 0159F5 SEM */
    t = rd8(ABS(0x0227));  /* 0159F6 CMP */
    if ((A & 0xFF) == t) goto L_015A09;  /* 0159F9 BEQ */
    mflag = 0;  /* 0159FB CLM */
    wr16(ABS(0x02DF), 0x5072);  /* 0159FC LDM */
    mflag = 1;  /* 015A01 SEM */
    wr8(ABS(0x02E1), 0x01);  /* 015A02 LDM */
    goto L_015A24;  /* 015A06 JMP */
L_015A09: TICK();
    mflag = 0;  /* 015A09 CLM */
    A = rd16(ABS(0x0227));  /* 015A0A LDA */
    A &= 0x00FF;  /* 015A0D AND */
    A = asl16(A);  /* 015A10 ASL */
    A = asl16(A);  /* 015A11 ASL */
    A += 1;  /* 015A12 INC */
    X = A;  /* 015A13 TAX */
    A = rd16((0x0023FD + X));  /* 015A14 LDA */
    mflag = 1;  /* 015A18 SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 015A19 STA */
    mflag = 0;  /* 015A1C CLM */
    A = rd16((0x0023FB + X));  /* 015A1D LDA */
    wr16(ABS(0x02DF), A);  /* 015A21 STA */
L_015A24: TICK();
    goto L_015AAF;  /* 015A24 JMP */
L_015A27: TICK();
    mflag = 0;  /* 015A27 CLM */
    A = rd16(ABS(0x0226));  /* 015A28 LDA */
    mflag = 1;  /* 015A2B SEM */
    /* 015A2C CMP */
    if ((A & 0xFF) != 0x73) goto L_015A9B;  /* 015A2E BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 015A30 LDA */
    mflag = 0;  /* 015A33 CLM */
    A &= 0x00FF;  /* 015A34 AND */
    A = asl16(A);  /* 015A37 ASL */
    A = asl16(A);  /* 015A38 ASL */
    A += 1;  /* 015A39 INC */
    X = A;  /* 015A3A TAX */
    A = rd16((0x0023FD + X));  /* 015A3B LDA */
    mflag = 1;  /* 015A3F SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 015A40 STA */
    mflag = 0;  /* 015A43 CLM */
    A = rd16((0x0023FB + X));  /* 015A44 LDA */
    wr16(ABS(0x02DF), A);  /* 015A48 STA */
    A = rd16(ABS(0x0227));  /* 015A4B LDA */
    A &= 0x00FF;  /* 015A4E AND */
    A = asl16(A);  /* 015A51 ASL */
    A = asl16(A);  /* 015A52 ASL */
    X = A;  /* 015A53 TAX */
    A = rd16((0x0023FB + X));  /* 015A54 LDA */
    mflag = 1;  /* 015A58 SEM */
    wr8(ABS(0x0227), (uint8_t)A);  /* 015A59 STA */
    mflag = 0;  /* 015A5C CLM */
    A &= 0x00FF;  /* 015A5D AND */
    X = A;  /* 015A60 TAX */
    A = rd16((0x0023E9 + X));  /* 015A61 LDA */
    mflag = 1;  /* 015A65 SEM */
    t = rd8(ABS(0x0227));  /* 015A66 CMP */
    if ((A & 0xFF) != t) goto L_015A86;  /* 015A69 BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 015A6B LDA */
    mflag = 0;  /* 015A6E CLM */
    A &= 0x00FF;  /* 015A6F AND */
    A = asl16(A);  /* 015A72 ASL */
    A = asl16(A);  /* 015A73 ASL */
    A += 1;  /* 015A74 INC */
    X = A;  /* 015A75 TAX */
    A = rd16((0x0023FD + X));  /* 015A76 LDA */
    mflag = 1;  /* 015A7A SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 015A7B STA */
    mflag = 0;  /* 015A7E CLM */
    A = rd16((0x0023FB + X));  /* 015A7F LDA */
    wr16(ABS(0x02DF), A);  /* 015A83 STA */
L_015A86: TICK();
    Y = 0x002B;  /* 015A86 LDY */
    mflag = 0;  /* 015A89 CLM */
    S -= 3; F_003310();  /* 015A8A JSRL */
    mflag = 1;  /* 015A8E SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 015A8F LDA */
    if ((A & 0xFF) != 0) goto L_015A98;  /* 015A92 BNE */
    wr8(ABS(0x0226), 0x05);  /* 015A94 LDM */
L_015A98: TICK();
    goto L_015AAF;  /* 015A98 JMP */
L_015A9B: TICK();
    mflag = 0;  /* 015A9B CLM */
    A = rd16(ABS(0x0226));  /* 015A9C LDA */
    mflag = 1;  /* 015A9F SEM */
    /* 015AA0 CMP */
    if ((A & 0xFF) != 0x71) goto L_015AAF;  /* 015AA2 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015AAF;  /* 015AA4 BBS */
    mflag = 0;  /* 015AA9 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0010);  /* 015AAA SEB */
L_015AAF: TICK();
    goto L_015ADE;  /* 015AAF JMP */
L_015AB2: TICK();
    mflag = 0;  /* 015AB2 CLM */
    A = rd16(ABS(0x0226));  /* 015AB3 LDA */
    mflag = 1;  /* 015AB6 SEM */
    /* 015AB7 CMP */
    if ((A & 0xFF) != 0x71) goto L_015ACD;  /* 015AB9 BNE */
    wr8(ABS(0x0233), 0x10);  /* 015ABB LDM */
    mflag = 0;  /* 015ABF CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0020);  /* 015AC0 SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0002);  /* 015AC5 SEB */
    goto L_015ADE;  /* 015ACA JMP */
L_015ACD: TICK();
    mflag = 1;  /* 015ACD SEM */
    wr8(ABS(0x0293), 0x00);  /* 015ACE LDM */
    wr8(ABS(0x0226), 0x0F);  /* 015AD2 LDM */
    Y = 0x000D;  /* 015AD6 LDY */
    mflag = 0;  /* 015AD9 CLM */
    S -= 3; F_0080AA();  /* 015ADA JSRL */
L_015ADE: TICK();
    mflag = 0;  /* 015ADE CLM */
    A = rd16(ABS(0x0226));  /* 015ADF LDA */
    mflag = 1;  /* 015AE2 SEM */
    /* 015AE3 CMP */
    if ((A & 0xFF) == 0x0F) goto L_015B57;  /* 015AE5 BEQ */
    wr8(ABS(0x01B7), 0x9D);  /* 015AE7 LDM */
    wr8(ABS(0x0230), 0x9D);  /* 015AEB LDM */
    mflag = 0;  /* 015AEF CLM */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0) goto L_015B06;  /* 015AF0 BBC */
    if ((rd16(ABS(0x0242)) & 0x0020) == 0x0020) goto L_015B03;  /* 015AF6 BBS */
    Y = 0x000D;  /* 015AFC LDY */
    S -= 3; F_0080AA();  /* 015AFF JSRL */
L_015B03: TICK();
    goto L_015B22;  /* 015B03 JMP */
L_015B06: TICK();
    mflag = 1;  /* 015B06 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 015B07 LDA */
    if ((A & 0xFF) == 0) goto L_015B22;  /* 015B0A BEQ */
    mflag = 0;  /* 015B0C CLM */
    mflag = 1;  /* 015B0D SEM */
    SETLO(A, rd8(ABS(0x02E1)));  /* 015B0E LDA */
    mflag = 0;  /* 015B11 CLM */
    push16(A);  /* 015B12 PHA */
    A = rd16(ABS(0x02DF));  /* 015B13 LDA */
    push16(A);  /* 015B16 PHA */
    S -= 3; rt_call_ptr_01E4C2();  /* 015B17 JSRL */
    Y = 0x0000;  /* 015B1B LDY */
    S -= 3; F_0080AA();  /* 015B1E JSRL */
L_015B22: TICK();
    mflag = 1;  /* 015B22 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 015B23 LDA */
    if ((A & 0xFF) == 0) goto L_015B2F;  /* 015B26 BEQ */
    wr8(ABS(0x01BA), 0x03);  /* 015B28 LDM */
    goto L_015B34;  /* 015B2C JMP */
L_015B2F: TICK();
    mflag = 1;  /* 015B2F SEM */
    wr8(ABS(0x01B7), 0x00);  /* 015B30 LDM */
L_015B34: TICK();
    mflag = 0;  /* 015B34 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 015B35 SEB */
    mflag = 1;  /* 015B3A SEM */
    wr8(ABS(0x0232), 0x3F);  /* 015B3B LDM */
    mflag = 0;  /* 015B3F CLM */
    S -= 3; F_007A06();  /* 015B40 JSRL */
    mflag = 1;  /* 015B44 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 015B45 LDA */
    if ((A & 0xFF) == 0) goto L_015B57;  /* 015B48 BEQ */
    wr8(ABS(0x01B6), 0x14);  /* 015B4A LDM */
    SETLO(A, rd8(ABS(0x0231)));  /* 015B4E LDA */
    if ((A & 0xFF) != 0) goto L_015B57;  /* 015B51 BNE */
    wr8(ABS(0x0231), 0x14);  /* 015B53 LDM */
L_015B57: TICK();
    SETLO(A, rd8(ABS(0x0226)));  /* 015B57 LDA */
    wr8(ABS(0x01BB), (uint8_t)A);  /* 015B5A STA */
L_015B5D: TICK();
    SETLO(A, rd8(ABS(0x0230)));  /* 015B5D LDA */
    if ((A & 0xFF) != 0) goto L_015BBE;  /* 015B60 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 015B62 LDA */
    if ((A & 0xFF) == 0) goto L_015BBE;  /* 015B65 BEQ */
    wr8(DP(0x00), (uint8_t)A);  /* 015B67 STA */
    /* 015B69 CMP */
    if ((A & 0xFF) != 0x14) goto L_015B98;  /* 015B6B BNE */
    wr8(ABS(0x0227), 0x00);  /* 015B6D LDM */
    mflag = 0;  /* 015B71 CLM */
    A = 0x0000;  /* 015B72 LDA */
    A = asl16(A);  /* 015B75 ASL */
    A = asl16(A);  /* 015B76 ASL */
    A += 1;  /* 015B77 INC */
    X = A;  /* 015B78 TAX */
    A = rd16((0x00319E + X));  /* 015B79 LDA */
    mflag = 1;  /* 015B7D SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 015B7E STA */
    mflag = 0;  /* 015B81 CLM */
    A = rd16((0x00319C + X));  /* 015B82 LDA */
    wr16(ABS(0x02DF), A);  /* 015B86 STA */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 015B89 SEB */
    mflag = 1;  /* 015B8E SEM */
    wr8(ABS(0x0232), 0x3F);  /* 015B8F LDM */
    mflag = 0;  /* 015B93 CLM */
    S -= 3; F_007A06();  /* 015B94 JSRL */
L_015B98: TICK();
    mflag = 1;  /* 015B98 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 015B99 LDA */
    if ((A & 0xFF) == 0) goto L_015BB5;  /* 015B9C BEQ */
    mflag = 0;  /* 015B9E CLM */
    mflag = 1;  /* 015B9F SEM */
    SETLO(A, rd8(ABS(0x02E1)));  /* 015BA0 LDA */
    mflag = 0;  /* 015BA3 CLM */
    push16(A);  /* 015BA4 PHA */
    A = rd16(ABS(0x02DF));  /* 015BA5 LDA */
    push16(A);  /* 015BA8 PHA */
    S -= 3; rt_call_ptr_01E4C2();  /* 015BA9 JSRL */
    mflag = 1;  /* 015BAD SEM */
    wr8(ABS(0x01BA), 0x03);  /* 015BAE LDM */
    goto L_015BBA;  /* 015BB2 JMP */
L_015BB5: TICK();
    mflag = 1;  /* 015BB5 SEM */
    wr8(ABS(0x01B7), 0x00);  /* 015BB6 LDM */
L_015BBA: TICK();
    wr8(ABS(0x0231), 0x00);  /* 015BBA LDM */
L_015BBE: TICK();
    wr8(ABS(0x0226), 0x0F);  /* 015BBE LDM */
    mflag = 0;  /* 015BC2 CLM */
    Y = pull16();  /* 015BC3 PLY */
    D = pull16();  /* 015BC4 PLD */
    S += 3; return;  /* 015BC5 RTL */
}

/* 015BC6  callers: 015EFB */
void F_015BC6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 015BC6 LDA */
    push16(D);  /* 015BC8 PHD */
    D = A;  /* 015BC9 TAD */
    C = 0;  /* 015BCA CLC */
    A = adc16(A, 0x0004);  /* 015BCB ADC */
    push16(A);  /* 015BCE PHA */
    mflag = 1;  /* 015BCF SEM */
    if ((rd8(ABS(0x023A)) & 0x04) == 0x04) goto L_015BD8;  /* 015BD0 BBS */
    goto L_015D2B;  /* 015BD5 JMP */
L_015BD8: TICK();
    mflag = 0;  /* 015BD8 CLM */
    A = D;  /* 015BD9 TDA */
    A += 1;  /* 015BDA INC */
    B = 0x0000;  /* 015BDB LDA */
    Y = A;  /* 015BDF TAY */
    S -= 3; F_00589D();  /* 015BE0 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 015BE4 STA */
    mflag = 0;  /* 015BE6 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0004);  /* 015BE7 CLB */
    mflag = 1;  /* 015BEC SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015BF5;  /* 015BED BBS */
    goto L_015CAD;  /* 015BF2 JMP */
L_015BF5: TICK();
    mflag = 0;  /* 015BF5 CLM */
    A = rd16(ABS(0x0240));  /* 015BF6 LDA */
    A &= 0x0020;  /* 015BF9 AND */
    if (A == 0) goto L_015C01;  /* 015BFC BEQ */
    A = 0x0001;  /* 015BFE LDA */
L_015C01: TICK();
    mflag = 1;  /* 015C01 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 015C02 STA */
    mflag = 0;  /* 015C04 CLM */
    A = rd16(ABS(0x0244));  /* 015C05 LDA */
    A &= 0x0010;  /* 015C08 AND */
    if (A == 0) goto L_015C10;  /* 015C0B BEQ */
    A = 0x0001;  /* 015C0D LDA */
L_015C10: TICK();
    mflag = 1;  /* 015C10 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 015C11 STA */
    if ((rd8(ABS(0x033F)) & 0x02) == 0) goto L_015C2E;  /* 015C13 BBC */
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_015C25;  /* 015C18 BBC */
    mflag = 0;  /* 015C1C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0020);  /* 015C1D CLB */
    goto L_015C2B;  /* 015C22 JMP */
L_015C25: TICK();
    mflag = 0;  /* 015C25 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0020);  /* 015C26 SEB */
L_015C2B: TICK();
    goto L_015C42;  /* 015C2B JMP */
L_015C2E: TICK();
    mflag = 1;  /* 015C2E SEM */
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_015C3C;  /* 015C2F BBC */
    mflag = 0;  /* 015C33 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0020);  /* 015C34 SEB */
    goto L_015C42;  /* 015C39 JMP */
L_015C3C: TICK();
    mflag = 0;  /* 015C3C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0020);  /* 015C3D CLB */
L_015C42: TICK();
    mflag = 1;  /* 015C42 SEM */
    if ((rd8(DP(0x00)) & 0x40) == 0) goto L_015C5F;  /* 015C43 BBC */
    SETLO(A, rd8(DP(0x01)));  /* 015C47 LDA */
    if ((A & 0xFF) != 0) goto L_015C5F;  /* 015C49 BNE */
    mflag = 0;  /* 015C4B CLM */
    if ((rd16(ABS(0x0244)) & 0x0010) == 0x0010) goto L_015C5C;  /* 015C4C BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0010);  /* 015C52 SEB */
    mflag = 1;  /* 015C57 SEM */
    wr8(ABS(0x01BB), 0x0A);  /* 015C58 LDM */
L_015C5C: TICK();
    goto L_015C6F;  /* 015C5C JMP */
L_015C5F: TICK();
    mflag = 0;  /* 015C5F CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0010);  /* 015C60 CLB */
    mflag = 1;  /* 015C65 SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 015C66 LDA */
    if ((A & 0xFF) != 0) goto L_015C6F;  /* 015C69 BNE */
    wr8(ABS(0x01BB), 0x0B);  /* 015C6B LDM */
L_015C6F: TICK();
    mflag = 0;  /* 015C6F CLM */
    A = rd16(DP(0x02));  /* 015C70 LDA */
    A &= 0x00FF;  /* 015C72 AND */
    wr16(DP(0x04), A);  /* 015C75 STA */
    A = rd16(ABS(0x0240));  /* 015C77 LDA */
    A &= 0x0020;  /* 015C7A AND */
    if (A == 0) goto L_015C82;  /* 015C7D BEQ */
    A = 0x0001;  /* 015C7F LDA */
L_015C82: TICK();
    t = rd16(DP(0x04));  /* 015C82 CMP */
    if (A == t) goto L_015C99;  /* 015C84 BEQ */
    if ((rd16(ABS(0x0240)) & 0x0020) == 0) goto L_015C94;  /* 015C86 BBC */
    mflag = 1;  /* 015C8C SEM */
    wr8(ABS(0x01DE), 0x01);  /* 015C8D LDM */
    goto L_015C99;  /* 015C91 JMP */
L_015C94: TICK();
    mflag = 1;  /* 015C94 SEM */
    wr8(ABS(0x01DF), 0x01);  /* 015C95 LDM */
L_015C99: TICK();
    mflag = 0;  /* 015C99 CLM */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_015CAA;  /* 015C9A BBC */
    mflag = 1;  /* 015CA0 SEM */
    if ((rd8(DP(0x00)) & 0x20) == 0x20) goto L_015CAA;  /* 015CA1 BBS */
    mflag = 0;  /* 015CA5 CLM */
    S -= 3; F_00973E();  /* 015CA6 JSRL */
L_015CAA: TICK();
    goto L_015D2B;  /* 015CAA JMP */
L_015CAD: TICK();
    mflag = 1;  /* 015CAD SEM */
    SETLO(B, 0x00);  /* 015CAE LDA */
    SETLO(A, rd8(DP(0x00)));  /* 015CB1 LDA */
    div8(0x10);  /* 015CB3 DIV */
    SETLO(A, A ^ 0xFF);  /* 015CB6 EOR */
    SETLO(A, A + 1);  /* 015CB8 INC */
    C = 0;  /* 015CB9 CLC */
    SETLO(A, adc8(A, 0x10));  /* 015CBA ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 015CBC STA */
    t = rd8(ABS(0x0206));  /* 015CBE CMP */
    if ((A & 0xFF) == t) goto L_015CDC;  /* 015CC1 BEQ */
    mflag = 0;  /* 015CC3 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0004);  /* 015CC4 SEB */
    mflag = 1;  /* 015CC9 SEM */
    wr8(ABS(0x0233), 0x04);  /* 015CCA LDM */
    SETLO(A, rd8(DP(0x02)));  /* 015CCE LDA */
    wr8(ABS(0x0206), (uint8_t)A);  /* 015CD0 STA */
    mflag = 0;  /* 015CD3 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0040);  /* 015CD4 SEB */
    goto L_015D2B;  /* 015CD9 JMP */
L_015CDC: TICK();
    mflag = 1;  /* 015CDC SEM */
    if ((rd8(DP(0x00)) & 0x04) == 0) goto L_015CE5;  /* 015CDD BBC */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_015D2B;  /* 015CE1 BBS */
L_015CE5: TICK();
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_015D20;  /* 015CE5 BBC */
    if ((rd8(DP(0x00)) & 0x04) == 0x04) goto L_015D14;  /* 015CEA BBS */
    mflag = 0;  /* 015CEE CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0040);  /* 015CEF SEB */
    if ((rd16(ABS(0x023C)) & 0x0010) == 0) goto L_015D02;  /* 015CF4 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 015CFA CLB */
    goto L_015D08;  /* 015CFF JMP */
L_015D02: TICK();
    mflag = 0;  /* 015D02 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 015D03 SEB */
L_015D08: TICK();
    mflag = 1;  /* 015D08 SEM */
    wr8(ABS(0x01BA), 0x03);  /* 015D09 LDM */
    wr8(ABS(0x01BB), 0x01);  /* 015D0D LDM */
    goto L_015D1D;  /* 015D11 JMP */
L_015D14: TICK();
    mflag = 1;  /* 015D14 SEM */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_015D1D;  /* 015D15 BBS */
    wr8(ABS(0x01BB), 0x07);  /* 015D19 LDM */
L_015D1D: TICK();
    goto L_015D2B;  /* 015D1D JMP */
L_015D20: TICK();
    mflag = 1;  /* 015D20 SEM */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_015D2B;  /* 015D21 BBS */
    mflag = 0;  /* 015D25 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0002);  /* 015D26 CLB */
L_015D2B: TICK();
    mflag = 1;  /* 015D2B SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015D83;  /* 015D2C BBS */
    if ((rd8(ABS(0x023A)) & 0x01) == 0) goto L_015D83;  /* 015D31 BBC */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_015D83;  /* 015D36 BBC */
    SETLO(A, rd8(ABS(0x0233)));  /* 015D3B LDA */
    if ((A & 0xFF) != 0) goto L_015D83;  /* 015D3E BNE */
    if ((rd8(ABS(0x023A)) & 0x40) == 0) goto L_015D83;  /* 015D40 BBC */
    mflag = 0;  /* 015D45 CLM */
    A = D;  /* 015D46 TDA */
    A += 1;  /* 015D47 INC */
    B = 0x0000;  /* 015D48 LDA */
    Y = A;  /* 015D4C TAY */
    S -= 3; F_00589D();  /* 015D4D JSRL */
    SETLO(B, 0x00);  /* 015D51 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 015D54 STA */
    div8(0x10);  /* 015D56 DIV */
    SETLO(A, A ^ 0xFF);  /* 015D59 EOR */
    SETLO(A, A + 1);  /* 015D5B INC */
    C = 0;  /* 015D5C CLC */
    SETLO(A, adc8(A, 0x10));  /* 015D5D ADC */
    wr8(ABS(0x0206), (uint8_t)A);  /* 015D5F STA */
    wr8(ABS(0x0293), 0x00);  /* 015D62 LDM */
    wr8(ABS(0x0227), 0x00);  /* 015D66 LDM */
    wr8(ABS(0x01BB), 0x06);  /* 015D6A LDM */
    mflag = 0;  /* 015D6E CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0040);  /* 015D6F CLB */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 015D74 SEB */
    mflag = 1;  /* 015D79 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 015D7A LDM */
    mflag = 0;  /* 015D7E CLM */
    S -= 3; F_007A06();  /* 015D7F JSRL */
L_015D83: TICK();
    mflag = 0;  /* 015D83 CLM */
    Y = pull16();  /* 015D84 PLY */
    D = pull16();  /* 015D85 PLD */
    S += 3; return;  /* 015D86 RTL */
}

/* 015D87  callers: 015EFF */
void F_015D87(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 015D87 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015D90;  /* 015D88 BBS */
    goto L_015E16;  /* 015D8D JMP */
L_015D90: TICK();
    mflag = 0;  /* 015D90 CLM */
    if ((rd16(ABS(0x0240)) & 0x0200) == 0x0200) goto L_015DE5;  /* 015D91 BBS */
    mflag = 1;  /* 015D97 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0x10) goto L_015DA6;  /* 015D98 BBS [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 015D9D CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0001);  /* 015D9E CLB */
    goto L_015DB7;  /* 015DA3 JMP */
L_015DA6: TICK();
    mflag = 0;  /* 015DA6 CLM */
    if ((rd16(ABS(0x0242)) & 0x0001) == 0x0001) goto L_015DB7;  /* 015DA7 BBS */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 015DAD SEB */
    mflag = 1;  /* 015DB2 SEM */
    wr8(ABS(0x0233), 0x02);  /* 015DB3 LDM */
L_015DB7: TICK();
    mflag = 1;  /* 015DB7 SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0x80) goto L_015DD1;  /* 015DB8 BBS [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 015DBD CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0x0020) goto L_015DCE;  /* 015DBE BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0020);  /* 015DC4 SEB */
    mflag = 1;  /* 015DC9 SEM */
    wr8(ABS(0x01BB), 0x0A);  /* 015DCA LDM */
L_015DCE: TICK();
    goto L_015DE2;  /* 015DCE JMP */
L_015DD1: TICK();
    mflag = 0;  /* 015DD1 CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0) goto L_015DE2;  /* 015DD2 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 015DD8 SEB */
    mflag = 1;  /* 015DDD SEM */
    wr8(ABS(0x0233), 0x02);  /* 015DDE LDM */
L_015DE2: TICK();
    goto L_015E13;  /* 015DE2 JMP */
L_015DE5: TICK();
    mflag = 1;  /* 015DE5 SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 015DE6 LDA */
    if ((A & 0xFF) != 0) goto L_015E13;  /* 015DE9 BNE */
    mflag = 0;  /* 015DEB CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 015DEC CLB */
    mflag = 1;  /* 015DF1 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0) goto L_015DFD;  /* 015DF2 BBC [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 015DF7 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0001);  /* 015DF8 SEB */
L_015DFD: TICK();
    mflag = 1;  /* 015DFD SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0) goto L_015E13;  /* 015DFE BBC [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 015E03 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0020);  /* 015E04 CLB */
    mflag = 1;  /* 015E09 SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 015E0A LDA */
    if ((A & 0xFF) != 0) goto L_015E13;  /* 015E0D BNE */
    wr8(ABS(0x01BB), 0x0B);  /* 015E0F LDM */
L_015E13: TICK();
    goto L_015ECE;  /* 015E13 JMP */
L_015E16: TICK();
    mflag = 0;  /* 015E16 CLM */
    if ((rd16(ABS(0x0240)) & 0x0200) == 0x0200) goto L_015E87;  /* 015E17 BBS */
    mflag = 1;  /* 015E1D SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0x10) goto L_015E37;  /* 015E1E BBS [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 015E23 CLM */
    if ((rd16(ABS(0x0244)) & 0x0010) == 0x0010) goto L_015E34;  /* 015E24 BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0010);  /* 015E2A SEB */
    mflag = 1;  /* 015E2F SEM */
    wr8(ABS(0x01BB), 0x0A);  /* 015E30 LDM */
L_015E34: TICK();
    goto L_015E48;  /* 015E34 JMP */
L_015E37: TICK();
    mflag = 0;  /* 015E37 CLM */
    if ((rd16(ABS(0x0244)) & 0x0010) == 0) goto L_015E48;  /* 015E38 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 015E3E SEB */
    mflag = 1;  /* 015E43 SEM */
    wr8(ABS(0x0233), 0x02);  /* 015E44 LDM */
L_015E48: TICK();
    mflag = 1;  /* 015E48 SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0x80) goto L_015E62;  /* 015E49 BBS [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 015E4E CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0x0020) goto L_015E5F;  /* 015E4F BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0020);  /* 015E55 SEB */
    mflag = 1;  /* 015E5A SEM */
    wr8(ABS(0x01BB), 0x0A);  /* 015E5B LDM */
L_015E5F: TICK();
    goto L_015E73;  /* 015E5F JMP */
L_015E62: TICK();
    mflag = 0;  /* 015E62 CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0) goto L_015E73;  /* 015E63 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 015E69 SEB */
    mflag = 1;  /* 015E6E SEM */
    wr8(ABS(0x0233), 0x02);  /* 015E6F LDM */
L_015E73: TICK();
    mflag = 1;  /* 015E73 SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_015E84;  /* 015E74 BBS [P45 ACIL_CAGRI] */
    mflag = 0;  /* 015E79 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 015E7A SEB */
    mflag = 1;  /* 015E7F SEM */
    wr8(ABS(0x0233), 0x02);  /* 015E80 LDM */
L_015E84: TICK();
    goto L_015ECE;  /* 015E84 JMP */
L_015E87: TICK();
    mflag = 1;  /* 015E87 SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 015E88 LDA */
    if ((A & 0xFF) != 0) goto L_015ECE;  /* 015E8B BNE */
    mflag = 0;  /* 015E8D CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 015E8E CLB */
    mflag = 1;  /* 015E93 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0) goto L_015EA9;  /* 015E94 BBC [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 015E99 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0010);  /* 015E9A CLB */
    mflag = 1;  /* 015E9F SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 015EA0 LDA */
    if ((A & 0xFF) != 0) goto L_015EA9;  /* 015EA3 BNE */
    wr8(ABS(0x01BB), 0x0B);  /* 015EA5 LDM */
L_015EA9: TICK();
    if ((rd8(ABS(P4)) & 0x80) == 0) goto L_015EBE;  /* 015EA9 BBC [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 015EAE CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0020);  /* 015EAF CLB */
    mflag = 1;  /* 015EB4 SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 015EB5 LDA */
    if ((A & 0xFF) != 0) goto L_015EBE;  /* 015EB8 BNE */
    wr8(ABS(0x01BB), 0x0B);  /* 015EBA LDM */
L_015EBE: TICK();
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_015ECE;  /* 015EBE BBS [P45 ACIL_CAGRI] */
    SETLO(A, rd8(ABS(0x03B8)));  /* 015EC3 LDA */
    /* 015EC6 CMP */
    if ((A & 0xFF) == 0x50) goto L_015ECE;  /* 015EC8 BEQ */
    wr8(ABS(0x01BB), 0x03);  /* 015ECA LDM */
L_015ECE: TICK();
    mflag = 0;  /* 015ECE CLM */
    S += 3; return;  /* 015ECF RTL */
}

/* 015ED0  callers: 016773 */
void F_015ED0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 015ED0 LDA */
    push16(A);  /* 015ED2 PHA */
    mflag = 1;  /* 015ED3 SEM */
    wr8(ABS(0x01BA), 0x01);  /* 015ED4 LDM */
    SETLO(A, rd8(ABS(0x01BE)));  /* 015ED8 LDA */
    if ((A & 0xFF) == 0) goto L_015EE4;  /* 015EDB BEQ */
    wr8(ABS(0x01C0), 0x00);  /* 015EDD LDM */
    goto L_015F37;  /* 015EE1 JMP */
L_015EE4: TICK();
    mflag = 1;  /* 015EE4 SEM */
    SETLO(A, rd8(ABS(0x01B6)));  /* 015EE5 LDA */
    if ((A & 0xFF) == 0) goto L_015EF6;  /* 015EE8 BEQ */
    SETLO(A, rd8(ABS(0x01B7)));  /* 015EEA LDA */
    if ((A & 0xFF) != 0) goto L_015EF6;  /* 015EED BNE */
    wr8(ABS(0x01C0), 0x01);  /* 015EEF LDM */
    goto L_015F37;  /* 015EF3 JMP */
L_015EF6: TICK();
    mflag = 0;  /* 015EF6 CLM */
    S -= 3; F_0158D7();  /* 015EF7 JSRL */
    S -= 3; F_015BC6();  /* 015EFB JSRL */
    S -= 3; F_015D87();  /* 015EFF JSRL */
    mflag = 1;  /* 015F03 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_015F22;  /* 015F04 BBS */
    SETLO(A, rd8(ABS(0x0206)));  /* 015F09 LDA */
    /* 015F0C CMP */
    if ((A & 0xFF) < 0x0F) goto L_015F22;  /* 015F0E BCC */
    mflag = 0;  /* 015F10 CLM */
    if ((rd16(ABS(0x039E)) & 0x0400) == 0) goto L_015F22;  /* 015F11 BBC */
    mflag = 1;  /* 015F17 SEM */
    SETLO(A, rd8(ABS(0x03A8)));  /* 015F18 LDA */
    if ((A & 0xFF) != 0) goto L_015F22;  /* 015F1B BNE */
    mflag = 0;  /* 015F1D CLM */
    S -= 3; F_015079();  /* 015F1E JSRL */
L_015F22: TICK();
    mflag = 0;  /* 015F22 CLM */
    A = rd16(ABS(0x01BB));  /* 015F23 LDA */
    mflag = 1;  /* 015F26 SEM */
    /* 015F27 CMP */
    if ((A & 0xFF) == 0x0F) goto L_015F32;  /* 015F29 BEQ */
    wr8(ABS(0x01C0), 0x02);  /* 015F2B LDM */
    goto L_015F37;  /* 015F2F JMP */
L_015F32: TICK();
    mflag = 1;  /* 015F32 SEM */
    wr8(ABS(0x01BA), 0x00);  /* 015F33 LDM */
L_015F37: TICK();
    mflag = 0;  /* 015F37 CLM */
    Y = pull16();  /* 015F38 PLY */
    S += 3; return;  /* 015F39 RTL */
}

/* 015F3A  callers: 0161F1, 016202 */
void F_015F3A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 015F3A LDA */
    push16(A);  /* 015F3C PHA */
    mflag = 1;  /* 015F3D SEM */
    if ((rd8(ABS(0x033F)) & 0x08) == 0x08) goto L_015F46;  /* 015F3E BBS */
    mflag = 0;  /* 015F43 CLM */
    Y = pull16();  /* 015F44 PLY */
    S += 3; return;  /* 015F45 RTL */
L_015F46: TICK();
    SETLO(A, rd8(ABS(0x01BB)));  /* 015F46 LDA */
    /* 015F49 CMP */
    if ((A & 0xFF) != 0x2A) goto L_015F61;  /* 015F4B BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 015F4D LDA */
    t = (rd8(ABS(0x0293)) + 1) & 0xFF; wr8(ABS(0x0293), t);  /* 015F50 INC */
    SETLO(A, A & 0x0F);  /* 015F53 AND */
    mflag = 0;  /* 015F55 CLM */
    A &= 0x00FF;  /* 015F56 AND */
    X = A;  /* 015F59 TAX */
    A = rd16(ABS(0x01BB));  /* 015F5A LDA */
    mflag = 1;  /* 015F5D SEM */
    wr8((ABS(0x0283) + X), (uint8_t)A);  /* 015F5E STA */
L_015F61: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 015F61 LDA */
    /* 015F64 CMP */
    if ((A & 0xFF) >= 0x11) goto L_015F7B;  /* 015F66 BCS */
    wr8(ABS(0x01B6), 0x14);  /* 015F68 LDM */
    wr8(ABS(0x01B7), 0xFB);  /* 015F6C LDM */
    Y = 0x0003;  /* 015F70 LDY */
    mflag = 0;  /* 015F73 CLM */
    S -= 3; F_01520C();  /* 015F74 JSRL */
    goto L_015F93;  /* 015F78 JMP */
L_015F7B: TICK();
    mflag = 1;  /* 015F7B SEM */
    wr8(ABS(0x01BB), 0x09);  /* 015F7C LDM */
    wr8(ABS(0x01B6), 0x00);  /* 015F80 LDM */
    Y = 0x0000;  /* 015F84 LDY */
    mflag = 0;  /* 015F87 CLM */
    S -= 3; F_0155ED();  /* 015F88 JSRL */
    Y = 0x0001;  /* 015F8C LDY */
    S -= 3; F_01520C();  /* 015F8F JSRL */
L_015F93: TICK();
    mflag = 1;  /* 015F93 SEM */
    wr8(ABS(0x01BA), 0x03);  /* 015F94 LDM */
    mflag = 0;  /* 015F98 CLM */
    Y = pull16();  /* 015F99 PLY */
    S += 3; return;  /* 015F9A RTL */
}

/* 015F9B  callers: 01653A */
void F_015F9B(void)
{
    uint32_t t; (void)t;
    X = rd16(ABS(0x01D5));  /* 015F9B LDX */
    t = rd16(ABS(0x03D7));  /* 015F9E CPX */
    if (X != t) goto L_015FA8;  /* 015FA1 BNE */
    A = 0x0000;  /* 015FA3 LDA */
    mflag = 1;  /* 015FA6 SEM */
    S += 3; return;  /* 015FA7 RTL */
L_015FA8: TICK();
    X = rd16(ABS(0x01D3));  /* 015FA8 LDX */
    t = rd16(ABS(0x03D7));  /* 015FAB CPX */
    if (X != t) goto L_015FC1;  /* 015FAE BNE */
    X = rd16(ABS(0x01D5));  /* 015FB0 LDX */
    wr16(ABS(0x01D3), X);  /* 015FB3 STX */
    X = rd16(ABS(0x03D7));  /* 015FB6 LDX */
    wr16(ABS(0x01D5), X);  /* 015FB9 STX */
    A = 0x0000;  /* 015FBC LDA */
    mflag = 1;  /* 015FBF SEM */
    S += 3; return;  /* 015FC0 RTL */
L_015FC1: TICK();
    X = rd16(ABS(0x01D1));  /* 015FC1 LDX */
    t = rd16(ABS(0x03D7));  /* 015FC4 CPX */
    if (X != t) goto L_015FE0;  /* 015FC7 BNE */
    X = rd16(ABS(0x01D3));  /* 015FC9 LDX */
    wr16(ABS(0x01D1), X);  /* 015FCC STX */
    X = rd16(ABS(0x01D5));  /* 015FCF LDX */
    wr16(ABS(0x01D3), X);  /* 015FD2 STX */
    X = rd16(ABS(0x03D7));  /* 015FD5 LDX */
    wr16(ABS(0x01D5), X);  /* 015FD8 STX */
    A = 0x0000;  /* 015FDB LDA */
    mflag = 1;  /* 015FDE SEM */
    S += 3; return;  /* 015FDF RTL */
L_015FE0: TICK();
    X = rd16(ABS(0x01D3));  /* 015FE0 LDX */
    wr16(ABS(0x01D1), X);  /* 015FE3 STX */
    X = rd16(ABS(0x01D5));  /* 015FE6 LDX */
    wr16(ABS(0x01D3), X);  /* 015FE9 STX */
    X = rd16(ABS(0x03D7));  /* 015FEC LDX */
    wr16(ABS(0x01D5), X);  /* 015FEF STX */
    A = 0x0001;  /* 015FF2 LDA */
    mflag = 1;  /* 015FF5 SEM */
    S += 3; return;  /* 015FF6 RTL */
}

/* 015FF7  callers: 0164DA, 016546 */
void F_015FF7(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 015FF7 LDA */
    push16(D);  /* 015FF9 PHD */
    D = A;  /* 015FFA TAD */
    C = 0;  /* 015FFB CLC */
    A = adc16(A, 0x0005);  /* 015FFC ADC */
    push16(A);  /* 015FFF PHA */
    wr16(DP(0x00), Y);  /* 016000 STY */
    mflag = 1;  /* 016002 SEM */
    SETLO(A, rd8(ABS(0x01DD)));  /* 016003 LDA */
    if ((A & 0xFF) == 0) goto L_01600F;  /* 016006 BEQ */
    mflag = 0;  /* 016008 CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0x0100) goto L_016013;  /* 016009 BBS */
L_01600F: TICK();
    mflag = 0;  /* 01600F CLM */
    Y = pull16();  /* 016010 PLY */
    D = pull16();  /* 016011 PLD */
    S += 3; return;  /* 016012 RTL */
L_016013: TICK();
    A = Y;  /* 016013 TYA */
    mflag = 1;  /* 016014 SEM */
    /* 016015 CMP */
    if ((A & 0xFF) != 0x02) goto L_016024;  /* 016017 BNE */
    Y = 0x0000;  /* 016019 LDY */
    mflag = 0;  /* 01601C CLM */
    S -= 3; F_007809();  /* 01601D JSRL */
    goto L_0160B4;  /* 016021 JMP */
L_016024: TICK();
    mflag = 1;  /* 016024 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 016025 LDA */
    if ((A & 0xFF) != 0) goto L_016081;  /* 016027 BNE */
    wr8(DP(0x01), (uint8_t)A);  /* 016029 STA */
L_01602B: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 01602B LDA */
    /* 01602D CMP */
    if ((A & 0xFF) >= 0x02) goto L_016076;  /* 01602F BCS */
    mflag = 0;  /* 016031 CLM */
    A &= 0x00FF;  /* 016032 AND */
    A ^= 0xFFFF;  /* 016035 EOR */
    A += 1;  /* 016038 INC */
    A += 1;  /* 016039 INC */
    A += 1;  /* 01603A INC */
    A = asl16(A);  /* 01603B ASL */
    X = A;  /* 01603C TAX */
    A = rd16((ABS(0x01D1) + X));  /* 01603D LDA */
    wr16(ABS(0x03D7), A);  /* 016040 STA */
    Y = 0x0004;  /* 016043 LDY */
    S -= 3; F_01520C();  /* 016046 JSRL */
    wr16(DP(0x02), 0x0252);  /* 01604A LDM */
    mflag = 1;  /* 01604E SEM */
    wr8(DP(0x04), 0x00);  /* 01604F LDM */
L_016052: TICK();
    mflag = 0;  /* 016052 CLM */
    X = rd16(DP(0x02));  /* 016053 LDX */
    C = (X >= 0x0259);  /* 016055 CPX */
    Z = (X == 0x0259);
    if (!C) goto L_01605C;  /* 016058 BCC */
    if (!Z) goto L_016068;  /* 01605A BNE */
L_01605C: TICK();
    A = rd16(LIND(0x02));  /* 01605C LDA */
    Y = A;  /* 01605E TAY */
    S -= 3; F_007627();  /* 01605F JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 016063 INC */
    goto L_016052;  /* 016065 JMP */
L_016068: TICK();
    Y = 0x0020;  /* 016068 LDY */
    mflag = 0;  /* 01606B CLM */
    S -= 3; F_007627();  /* 01606C JSRL */
    mflag = 1;  /* 016070 SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 016071 INC */
    goto L_01602B;  /* 016073 JMP */
L_016076: TICK();
    Y = 0x0000;  /* 016076 LDY */
    mflag = 0;  /* 016079 CLM */
    S -= 3; F_007809();  /* 01607A JSRL */
    goto L_0160B4;  /* 01607E JMP */
L_016081: TICK();
    mflag = 0;  /* 016081 CLM */
    A = rd16(DP(0x00));  /* 016082 LDA */
    mflag = 1;  /* 016084 SEM */
    /* 016085 CMP */
    if ((A & 0xFF) != 0x01) goto L_0160B4;  /* 016087 BNE */
    Y = 0x0004;  /* 016089 LDY */
    mflag = 0;  /* 01608C CLM */
    S -= 3; F_01520C();  /* 01608D JSRL */
    wr16(DP(0x02), 0x0252);  /* 016091 LDM */
    mflag = 1;  /* 016095 SEM */
    wr8(DP(0x04), 0x00);  /* 016096 LDM */
L_016099: TICK();
    mflag = 0;  /* 016099 CLM */
    X = rd16(DP(0x02));  /* 01609A LDX */
    C = (X >= 0x0259);  /* 01609C CPX */
    Z = (X == 0x0259);
    if (!C) goto L_0160A3;  /* 01609F BCC */
    if (!Z) goto L_0160AF;  /* 0160A1 BNE */
L_0160A3: TICK();
    A = rd16(LIND(0x02));  /* 0160A3 LDA */
    Y = A;  /* 0160A5 TAY */
    S -= 3; F_007627();  /* 0160A6 JSRL */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 0160AA INC */
    goto L_016099;  /* 0160AC JMP */
L_0160AF: TICK();
    mflag = 0;  /* 0160AF CLM */
    S -= 3; F_0077F0();  /* 0160B0 JSRL */
L_0160B4: TICK();
    mflag = 0;  /* 0160B4 CLM */
    Y = pull16();  /* 0160B5 PLY */
    D = pull16();  /* 0160B6 PLD */
    S += 3; return;  /* 0160B7 RTL */
}

/* 0160B8  callers: 01673E */
void F_0160B8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0160B8 LDA */
    push16(D);  /* 0160BA PHD */
    D = A;  /* 0160BB TAD */
    A += 1;  /* 0160BC INC */
    A += 1;  /* 0160BD INC */
    push16(A);  /* 0160BE PHA */
    mflag = 1;  /* 0160BF SEM */
    wr8(ABS(0x01BA), 0x00);  /* 0160C0 LDM */
    wr8(ABS(0x01E3), 0x01);  /* 0160C4 LDM */
    wr8(ABS(0x01E0), 0x00);  /* 0160C8 LDM */
    wr8(ABS(0x01E1), 0x00);  /* 0160CC LDM */
    SETLO(A, rd8(ABS(0x0293)));  /* 0160D0 LDA */
    if ((A & 0xFF) != 0) goto L_0160DA;  /* 0160D3 BNE */
    SETLO(A, rd8(ABS(0x03A8)));  /* 0160D5 LDA */
    if ((A & 0xFF) != 0) goto L_0160DE;  /* 0160D8 BNE */
L_0160DA: TICK();
    wr8(ABS(0x01E4), 0x00);  /* 0160DA LDM */
L_0160DE: TICK();
    wr8(ABS(0x01E2), 0x00);  /* 0160DE LDM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 0160E2 LDA */
    if ((A & 0xFF) != 0) goto L_0160ED;  /* 0160E5 BNE */
    SETLO(A, rd8(ABS(0x0425)));  /* 0160E7 LDA */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 0160EA STA */
L_0160ED: TICK();
    SETLO(A, rd8(ABS(0x03B5)));  /* 0160ED LDA */
    /* 0160F0 CMP */
    if ((A & 0xFF) != 0x10) goto L_016102;  /* 0160F2 BNE */
    SETLO(A, rd8(ABS(0x01BB)));  /* 0160F4 LDA */
    /* 0160F7 CMP */
    if ((A & 0xFF) == 0x0C) goto L_016102;  /* 0160F9 BEQ */
    wr8(ABS(0x01BB), 0x09);  /* 0160FB LDM */
    goto L_016455;  /* 0160FF JMP */
L_016102: TICK();
    mflag = 0;  /* 016102 CLM */
    A = rd16(ABS(0x01BB));  /* 016103 LDA */
    mflag = 1;  /* 016106 SEM */
    /* 016107 CMP */
    if ((A & 0xFF) != 0x06) goto L_016126;  /* 016109 BNE */
    SETLO(A, rd8(ABS(0x0206)));  /* 01610B LDA */
    /* 01610E CMP */
    if ((A & 0xFF) != 0x0D) goto L_016119;  /* 016110 BNE */
    wr8(ABS(0x01BB), 0x04);  /* 016112 LDM */
    goto L_016126;  /* 016116 JMP */
L_016119: TICK();
    mflag = 0;  /* 016119 CLM */
    A = rd16(ABS(0x0206));  /* 01611A LDA */
    mflag = 1;  /* 01611D SEM */
    /* 01611E CMP */
    if ((A & 0xFF) != 0x0E) goto L_016126;  /* 016120 BNE */
    wr8(ABS(0x01BB), 0x02);  /* 016122 LDM */
L_016126: TICK();
    SETLO(A, rd8(ABS(0x01BB)));  /* 016126 LDA */
    /* 016129 CMP */
    if ((A & 0xFF) != 0x0A) goto L_016136;  /* 01612B BNE */
    SETLO(A, rd8(ABS(0x03A8)));  /* 01612D LDA */
    if ((A & 0xFF) != 0) goto L_016136;  /* 016130 BNE */
    wr8(ABS(0x01BB), 0x14);  /* 016132 LDM */
L_016136: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 016136 LDA */
    if ((A & 0xFF) == 0) goto L_016142;  /* 016139 BEQ */
    wr8(ABS(0x01BA), 0x03);  /* 01613B LDM */
    goto L_016455;  /* 01613F JMP */
L_016142: TICK();
    mflag = 1;  /* 016142 SEM */
    SETLO(A, rd8(ABS(0x01E8)));  /* 016143 LDA */
    if ((A & 0xFF) == 0) goto L_016153;  /* 016146 BEQ */
    SETLO(A, rd8(ABS(0x01BB)));  /* 016148 LDA */
    /* 01614B CMP */
    if ((A & 0xFF) == 0x23) goto L_016153;  /* 01614D BEQ */
    wr8(ABS(0x01E8), 0x00);  /* 01614F LDM */
L_016153: TICK();
    SETLO(A, rd8(ABS(0x01BB)));  /* 016153 LDA */
    /* 016156 CMP */
    if ((A & 0xFF) != 0x23) goto L_0161C4;  /* 016158 BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 01615A LDA */
    if ((A & 0xFF) == 0) goto L_01616D;  /* 01615D BEQ */
    mflag = 0;  /* 01615F CLM */
    A &= 0x00FF;  /* 016160 AND */
    A -= 1;  /* 016163 DEC */
    X = A;  /* 016164 TAX */
    A = rd16((ABS(0x0283) + X));  /* 016165 LDA */
    mflag = 1;  /* 016168 SEM */
    /* 016169 CMP */
    if ((A & 0xFF) == 0x2A) goto L_016172;  /* 01616B BEQ */
L_01616D: TICK();
    SETLO(A, rd8(ABS(0x01E8)));  /* 01616D LDA */
    if ((A & 0xFF) == 0) goto L_0161BF;  /* 016170 BEQ */
L_016172: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 016172 LDA */
    if ((A & 0xFF) != 0) goto L_01617C;  /* 016175 BNE */
    SETLO(A, rd8(ABS(0x01E8)));  /* 016177 LDA */
    if ((A & 0xFF) != 0) goto L_016186;  /* 01617A BNE */
L_01617C: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 01617C LDA */
    if ((A & 0xFF) != 0) goto L_016186;  /* 01617F BNE */
    SETLO(A, rd8(ABS(0x01E7)));  /* 016181 LDA */
    if ((A & 0xFF) == 0) goto L_016195;  /* 016184 BEQ */
L_016186: TICK();
    wr8(ABS(0x01BB), 0x07);  /* 016186 LDM */
    SETLO(A, rd8(ABS(0x03A8)));  /* 01618A LDA */
    if ((A & 0xFF) != 0) goto L_016192;  /* 01618D BNE */
    wr8(ABS(0x01C3), (uint8_t)A);  /* 01618F STA */
L_016192: TICK();
    goto L_01619A;  /* 016192 JMP */
L_016195: TICK();
    mflag = 1;  /* 016195 SEM */
    wr8(ABS(0x01C3), 0x00);  /* 016196 LDM */
L_01619A: TICK();
    wr8(ABS(0x01B6), 0x00);  /* 01619A LDM */
    Y = 0x0000;  /* 01619E LDY */
    mflag = 0;  /* 0161A1 CLM */
    S -= 3; F_0155ED();  /* 0161A2 JSRL */
    Y = 0x0001;  /* 0161A6 LDY */
    S -= 3; F_01520C();  /* 0161A9 JSRL */
    mflag = 1;  /* 0161AD SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0161AE LDM */
    SETLO(A, rd8(ABS(0x01BB)));  /* 0161B2 LDA */
    /* 0161B5 CMP */
    if ((A & 0xFF) != 0x23) goto L_0161BC;  /* 0161B7 BNE */
    goto L_016455;  /* 0161B9 JMP */
L_0161BC: TICK();
    goto L_0161C4;  /* 0161BC JMP */
L_0161BF: TICK();
    mflag = 1;  /* 0161BF SEM */
    wr8(ABS(0x01BB), 0x14);  /* 0161C0 LDM */
L_0161C4: TICK();
    mflag = 0;  /* 0161C4 CLM */
    A = rd16(ABS(0x03BA));  /* 0161C5 LDA */
    mflag = 1;  /* 0161C8 SEM */
    /* 0161C9 CMP */
    if ((A & 0xFF) >= 0x40) goto L_0161D2;  /* 0161CB BCS */
    SETLO(A, rd8(ABS(0x03A8)));  /* 0161CD LDA */
    if ((A & 0xFF) == 0) goto L_0161DD;  /* 0161D0 BEQ */
L_0161D2: TICK();
    SETLO(A, rd8(ABS(0x01BB)));  /* 0161D2 LDA */
    /* 0161D5 CMP */
    if ((A & 0xFF) != 0x03) goto L_0161DD;  /* 0161D7 BNE */
    wr8(ABS(0x01BB), 0x09);  /* 0161D9 LDM */
L_0161DD: TICK();
    SETLO(A, rd8(ABS(0x01BB)));  /* 0161DD LDA */
    /* 0161E0 CMP */
    if ((A & 0xFF) != 0x2A) goto L_0161F8;  /* 0161E2 BNE */
    SETLO(A, rd8(ABS(0x03A8)));  /* 0161E4 LDA */
    if ((A & 0xFF) == 0) goto L_0161F0;  /* 0161E7 BEQ */
    wr8(ABS(0x01E8), 0x01);  /* 0161E9 LDM */
    goto L_0161F5;  /* 0161ED JMP */
L_0161F0: TICK();
    mflag = 0;  /* 0161F0 CLM */
    S -= 3; F_015F3A();  /* 0161F1 JSRL */
L_0161F5: TICK();
    goto L_016455;  /* 0161F5 JMP */
L_0161F8: TICK();
    mflag = 0;  /* 0161F8 CLM */
    A = rd16(ABS(0x01BB));  /* 0161F9 LDA */
    mflag = 1;  /* 0161FC SEM */
    /* 0161FD CMP */
    if ((A & 0xFF) != 0x64) goto L_016209;  /* 0161FF BNE */
    mflag = 0;  /* 016201 CLM */
    S -= 3; F_015F3A();  /* 016202 JSRL */
    goto L_016455;  /* 016206 JMP */
L_016209: TICK();
    mflag = 0;  /* 016209 CLM */
    A = rd16(ABS(0x01BB));  /* 01620A LDA */
    mflag = 1;  /* 01620D SEM */
    /* 01620E CMP */
    if ((A & 0xFF) != 0x06) goto L_016238;  /* 016210 BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 016212 LDA */
    if ((A & 0xFF) == 0) goto L_01621C;  /* 016215 BEQ */
    SETLO(A, 0x00);  /* 016217 LDA */
    goto L_01621F;  /* 016219 JMP */
L_01621C: TICK();
    SETLO(A, rd8(ABS(0x01BB)));  /* 01621C LDA */
L_01621F: TICK();
    mflag = 0;  /* 01621F CLM */
    Y = A;  /* 016220 TAY */
    S -= 3; F_0155ED();  /* 016221 JSRL */
    Y = 0x0001;  /* 016225 LDY */
    S -= 3; F_01520C();  /* 016228 JSRL */
    mflag = 1;  /* 01622C SEM */
    wr8(ABS(0x01BA), 0x03);  /* 01622D LDM */
    wr8(ABS(0x01B6), 0x00);  /* 016231 LDM */
    goto L_016455;  /* 016235 JMP */
L_016238: TICK();
    mflag = 0;  /* 016238 CLM */
    A = rd16(ABS(0x01BB));  /* 016239 LDA */
    mflag = 1;  /* 01623C SEM */
    /* 01623D CMP */
    if ((A & 0xFF) != 0x05) goto L_016266;  /* 01623F BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 016241 LDA */
    if ((A & 0xFF) == 0) goto L_01624B;  /* 016244 BEQ */
    SETLO(A, 0x00);  /* 016246 LDA */
    goto L_01624D;  /* 016248 JMP */
L_01624B: TICK();
    SETLO(A, 0x05);  /* 01624B LDA */
L_01624D: TICK();
    mflag = 0;  /* 01624D CLM */
    Y = A;  /* 01624E TAY */
    S -= 3; F_0155ED();  /* 01624F JSRL */
    Y = 0x0001;  /* 016253 LDY */
    S -= 3; F_01520C();  /* 016256 JSRL */
    mflag = 1;  /* 01625A SEM */
    wr8(ABS(0x01BA), 0x03);  /* 01625B LDM */
    wr8(ABS(0x01B6), 0x00);  /* 01625F LDM */
    goto L_016455;  /* 016263 JMP */
L_016266: TICK();
    mflag = 0;  /* 016266 CLM */
    A = rd16(ABS(0x01BB));  /* 016267 LDA */
    mflag = 1;  /* 01626A SEM */
    /* 01626B CMP */
    if ((A & 0xFF) != 0x04) goto L_016277;  /* 01626D BNE */
    mflag = 0;  /* 01626F CLM */
    S -= 3; F_01556C();  /* 016270 JSRL */
    goto L_016455;  /* 016274 JMP */
L_016277: TICK();
    mflag = 0;  /* 016277 CLM */
    A = rd16(ABS(0x01BB));  /* 016278 LDA */
    mflag = 1;  /* 01627B SEM */
    /* 01627C CMP */
    if ((A & 0xFF) != 0x02) goto L_016288;  /* 01627E BNE */
    mflag = 0;  /* 016280 CLM */
    S -= 3; F_0155AB();  /* 016281 JSRL */
    goto L_016455;  /* 016285 JMP */
L_016288: TICK();
    mflag = 0;  /* 016288 CLM */
    A = rd16(ABS(0x01BB));  /* 016289 LDA */
    mflag = 1;  /* 01628C SEM */
    /* 01628D CMP */
    if ((A & 0xFF) != 0x03) goto L_0162EE;  /* 01628F BNE */
    mflag = 0;  /* 016291 CLM */
    if ((rd16(ABS(0x039E)) & 0x1000) == 0) goto L_0162E6;  /* 016292 BBC */
    A = rd16(ABS(0x03B8));  /* 016298 LDA */
    mflag = 1;  /* 01629B SEM */
    /* 01629C CMP */
    if ((A & 0xFF) == 0x50) goto L_0162E3;  /* 01629E BEQ */
    SETLO(A, rd8(ABS(0x041F)));  /* 0162A0 LDA */
    if ((A & 0xFF) != 0) goto L_0162E3;  /* 0162A3 BNE */
    wr8(ABS(0x0227), (uint8_t)A);  /* 0162A5 STA */
    wr8(ABS(0x01B6), (uint8_t)A);  /* 0162A8 STA */
    Y = 0x01D0;  /* 0162AB LDY */
    mflag = 0;  /* 0162AE CLM */
    S -= 3; eeprom_read_byte_007189();  /* 0162AF JSRL */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 0162B3 STA */
    Y = 0x01D1;  /* 0162B6 LDY */
    mflag = 0;  /* 0162B9 CLM */
    S -= 3; F_007271();  /* 0162BA JSRL */
    wr16(ABS(0x01CD), A);  /* 0162BE STA */
    X = A;  /* 0162C1 TAX */
    if (X != 0) goto L_0162CC;  /* 0162C2 BNE */
    mflag = 1;  /* 0162C4 SEM */
    wr8(ABS(0x01BB), 0x09);  /* 0162C5 LDM */
    goto L_016455;  /* 0162C9 JMP */
L_0162CC: TICK();
    Y = 0x0001;  /* 0162CC LDY */
    mflag = 0;  /* 0162CF CLM */
    S -= 3; F_01520C();  /* 0162D0 JSRL */
    X = rd16(ABS(0x01CD));  /* 0162D4 LDX */
    wr16(ABS(0x01CF), X);  /* 0162D7 STX */
    mflag = 1;  /* 0162DA SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0162DB LDM */
    wr8(ABS(0x01BD), 0x50);  /* 0162DF LDM */
L_0162E3: TICK();
    goto L_0162EB;  /* 0162E3 JMP */
L_0162E6: TICK();
    mflag = 1;  /* 0162E6 SEM */
    wr8(ABS(0x01BB), 0x09);  /* 0162E7 LDM */
L_0162EB: TICK();
    goto L_016455;  /* 0162EB JMP */
L_0162EE: TICK();
    mflag = 0;  /* 0162EE CLM */
    A = rd16(ABS(0x01BB));  /* 0162EF LDA */
    mflag = 1;  /* 0162F2 SEM */
    /* 0162F3 CMP */
    if ((A & 0xFF) != 0x0A) goto L_016306;  /* 0162F5 BNE */
    wr8(ABS(0x03B1), 0x01);  /* 0162F7 LDM */
    wr8(ABS(0x0227), 0x00);  /* 0162FB LDM */
    wr8(ABS(0x01B6), 0x04);  /* 0162FF LDM */
    goto L_016455;  /* 016303 JMP */
L_016306: TICK();
    mflag = 0;  /* 016306 CLM */
    A = rd16(ABS(0x01BB));  /* 016307 LDA */
    mflag = 1;  /* 01630A SEM */
    /* 01630B CMP */
    if ((A & 0xFF) != 0x0B) goto L_016320;  /* 01630D BNE */
    wr8(ABS(0x03B1), 0x00);  /* 01630F LDM */
    mflag = 0;  /* 016313 CLM */
    S -= 3; F_0156E4();  /* 016314 JSRL */
    mflag = 1;  /* 016318 SEM */
    wr8(ABS(0x01B6), 0x00);  /* 016319 LDM */
    goto L_016455;  /* 01631D JMP */
L_016320: TICK();
    mflag = 0;  /* 016320 CLM */
    A = rd16(ABS(0x01BB));  /* 016321 LDA */
    mflag = 1;  /* 016324 SEM */
    /* 016325 CMP */
    if ((A & 0xFF) != 0x14) goto L_016335;  /* 016327 BNE */
    SETLO(A, rd8(ABS(0x01E7)));  /* 016329 LDA */
    if ((A & 0xFF) == 0) goto L_016335;  /* 01632C BEQ */
    wr8(ABS(0x01DF), 0x01);  /* 01632E LDM */
    goto L_016455;  /* 016332 JMP */
L_016335: TICK();
    mflag = 0;  /* 016335 CLM */
    A = rd16(ABS(0x01BB));  /* 016336 LDA */
    mflag = 1;  /* 016339 SEM */
    /* 01633A CMP */
    if ((A & 0xFF) == 0x14) goto L_016341;  /* 01633C BEQ */
    goto L_0163F7;  /* 01633E JMP */
L_016341: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 016341 LDA */
    if ((A & 0xFF) == 0) goto L_016370;  /* 016344 BEQ */
    mflag = 0;  /* 016346 CLM */
    X = rd16(ABS(0x01CD));  /* 016347 LDX */
    wr16(DP(0x00), X);  /* 01634A STX */
    S -= 3; F_01B6FD();  /* 01634C JSRL */
    /* 016350 CMP */
    if ((A & 0xFF) != 0x00) goto L_01635F;  /* 016352 BNE */
    mflag = 0;  /* 016354 CLM */
    X = rd16(DP(0x00));  /* 016355 LDX */
    wr16(ABS(0x01CD), X);  /* 016357 STX */
    mflag = 1;  /* 01635A SEM */
    wr8(ABS(0x01BB), 0x09);  /* 01635B LDM */
L_01635F: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 01635F LDA */
    wr8(ABS(0x01C3), (uint8_t)A);  /* 016362 STA */
    wr8(ABS(0x01B6), 0x00);  /* 016365 LDM */
    wr8(ABS(0x0293), 0x00);  /* 016369 LDM */
    goto L_01638F;  /* 01636D JMP */
L_016370: TICK();
    mflag = 0;  /* 016370 CLM */
    X = rd16(ABS(0x01CD));  /* 016371 LDX */
    if (X == 0) goto L_01638A;  /* 016374 BEQ */
    /* 016376 CPX */
    if (X < 0x1FA5) goto L_01638F;  /* 016379 BCC */
    /* 01637B CPX */
    if (X < 0x1FA7) goto L_01638A;  /* 01637E BCC */
    /* 016380 CPX */
    if (X < 0x1FF9) goto L_01638F;  /* 016383 BCC */
    /* 016385 CPX */
    if (X >= 0x1FFE) goto L_01638F;  /* 016388 BCS */
L_01638A: TICK();
    mflag = 1;  /* 01638A SEM */
    wr8(ABS(0x01BB), 0x09);  /* 01638B LDM */
L_01638F: TICK();
    mflag = 1;  /* 01638F SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 016390 LDA */
    if ((A & 0xFF) != 0) goto L_0163E7;  /* 016393 BNE */
    SETLO(A, rd8(ABS(0x03A3)));  /* 016395 LDA */
    if ((A & 0xFF) == 0) goto L_0163DF;  /* 016398 BEQ */
    SETLO(A, rd8(ABS(0x03BA)));  /* 01639A LDA */
    /* 01639D CMP */
    if ((A & 0xFF) >= 0x40) goto L_0163DF;  /* 01639F BCS */
    SETLO(A, rd8(ABS(0x041F)));  /* 0163A1 LDA */
    if ((A & 0xFF) == 0) goto L_0163AF;  /* 0163A4 BEQ */
    mflag = 0;  /* 0163A6 CLM */
    X = rd16(ABS(0x01CD));  /* 0163A7 LDX */
    t = rd16(ABS(0x01CF));  /* 0163AA CPX */
    if (X == t) goto L_0163DF;  /* 0163AD BEQ */
L_0163AF: TICK();
    mflag = 0;  /* 0163AF CLM */
    A = rd16(ABS(0x01BB));  /* 0163B0 LDA */
    mflag = 1;  /* 0163B3 SEM */
    /* 0163B4 CMP */
    if ((A & 0xFF) != 0x14) goto L_0163DF;  /* 0163B6 BNE */
    SETLO(A, rd8(ABS(0x01BD)));  /* 0163B8 LDA */
    if ((A & 0xFF) != 0) goto L_0163C1;  /* 0163BB BNE */
    wr8(ABS(0x01BD), 0x40);  /* 0163BD LDM */
L_0163C1: TICK();
    mflag = 0;  /* 0163C1 CLM */
    X = rd16(ABS(0x01CD));  /* 0163C2 LDX */
    wr16(ABS(0x01CF), X);  /* 0163C5 STX */
    Y = 0x0001;  /* 0163C8 LDY */
    S -= 3; F_01520C();  /* 0163CB JSRL */
    mflag = 1;  /* 0163CF SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0163D0 LDM */
    wr8(ABS(0x0227), 0x00);  /* 0163D4 LDM */
    wr8(ABS(0x01B6), 0x00);  /* 0163D8 LDM */
    goto L_0163E4;  /* 0163DC JMP */
L_0163DF: TICK();
    mflag = 1;  /* 0163DF SEM */
    wr8(ABS(0x01BB), 0x09);  /* 0163E0 LDM */
L_0163E4: TICK();
    goto L_0163F4;  /* 0163E4 JMP */
L_0163E7: TICK();
    mflag = 0;  /* 0163E7 CLM */
    A = rd16(ABS(0x03B8));  /* 0163E8 LDA */
    mflag = 1;  /* 0163EB SEM */
    /* 0163EC CMP */
    if ((A & 0xFF) == 0x40) goto L_0163F4;  /* 0163EE BEQ */
    wr8(ABS(0x01BB), 0x09);  /* 0163F0 LDM */
L_0163F4: TICK();
    goto L_016455;  /* 0163F4 JMP */
L_0163F7: TICK();
    mflag = 0;  /* 0163F7 CLM */
    A = rd16(ABS(0x01BB));  /* 0163F8 LDA */
    mflag = 1;  /* 0163FB SEM */
    /* 0163FC CMP */
    if ((A & 0xFF) != 0x0C) goto L_01641E;  /* 0163FE BNE */
    SETLO(A, rd8(ABS(0x03A8)));  /* 016400 LDA */
    if ((A & 0xFF) == 0) goto L_016415;  /* 016403 BEQ */
    SETLO(A, rd8(ABS(0x03B1)));  /* 016405 LDA */
    if ((A & 0xFF) == 0) goto L_016415;  /* 016408 BEQ */
    wr8(ABS(0x01E6), 0x01);  /* 01640A LDM */
    wr8(ABS(0x03B1), 0x00);  /* 01640E LDM */
    goto L_01641B;  /* 016412 JMP */
L_016415: TICK();
    mflag = 0;  /* 016415 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0001);  /* 016416 CLB */
L_01641B: TICK();
    goto L_016455;  /* 01641B JMP */
L_01641E: TICK();
    mflag = 0;  /* 01641E CLM */
    A = rd16(ABS(0x01BB));  /* 01641F LDA */
    mflag = 1;  /* 016422 SEM */
    /* 016423 CMP */
    if ((A & 0xFF) != 0x07) goto L_016455;  /* 016425 BNE */
    SETLO(A, rd8(ABS(0x03A8)));  /* 016427 LDA */
    if ((A & 0xFF) == 0) goto L_016437;  /* 01642A BEQ */
    wr8(ABS(0x03B1), 0x00);  /* 01642C LDM */
    wr8(ABS(0x01DE), 0x01);  /* 016430 LDM */
    goto L_016452;  /* 016434 JMP */
L_016437: TICK();
    mflag = 1;  /* 016437 SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 016438 LDA */
    if ((A & 0xFF) == 0) goto L_016441;  /* 01643B BEQ */
    /* 01643D CMP */
    if ((A & 0xFF) != 0x54) goto L_016446;  /* 01643F BNE */
L_016441: TICK();
    SETLO(A, rd8(ABS(0x01E7)));  /* 016441 LDA */
    if ((A & 0xFF) == 0) goto L_01644D;  /* 016444 BEQ */
L_016446: TICK();
    wr8(ABS(0x01BD), 0x48);  /* 016446 LDM */
    goto L_016452;  /* 01644A JMP */
L_01644D: TICK();
    mflag = 1;  /* 01644D SEM */
    wr8(ABS(0x01BB), 0x09);  /* 01644E LDM */
L_016452: TICK();
    goto L_016455;  /* 016452 JMP */
L_016455: TICK();
    mflag = 0;  /* 016455 CLM */
    A = rd16(ABS(0x01BB));  /* 016456 LDA */
    mflag = 1;  /* 016459 SEM */
    /* 01645A CMP */
    if ((A & 0xFF) != 0x09) goto L_01647A;  /* 01645C BNE */
    Y = 0x0019;  /* 01645E LDY */
    mflag = 0;  /* 016461 CLM */
    S -= 3; F_0080AA();  /* 016462 JSRL */
    mflag = 1;  /* 016466 SEM */
    wr8(ABS(0x01BA), 0x00);  /* 016467 LDM */
    wr8(ABS(0x01B6), 0x08);  /* 01646B LDM */
    wr8(ABS(0x01B7), 0x20);  /* 01646F LDM */
    wr8(ABS(0x01BD), 0x00);  /* 016473 LDM */
    goto L_01649C;  /* 016477 JMP */
L_01647A: TICK();
    mflag = 0;  /* 01647A CLM */
    A = rd16(ABS(0x01BB));  /* 01647B LDA */
    mflag = 1;  /* 01647E SEM */
    /* 01647F CMP */
    if ((A & 0xFF) == 0x0A) goto L_01649C;  /* 016481 BEQ */
    SETLO(A, rd8(ABS(0x01BB)));  /* 016483 LDA */
    /* 016486 CMP */
    if ((A & 0xFF) == 0x0B) goto L_01649C;  /* 016488 BEQ */
    /* 01648A CMP */
    if ((A & 0xFF) == 0x06) goto L_01649C;  /* 01648C BEQ */
    mflag = 0;  /* 01648E CLM */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0x0010) goto L_01649C;  /* 01648F BBS */
    Y = 0x0000;  /* 016495 LDY */
    S -= 3; F_0080AA();  /* 016498 JSRL */
L_01649C: TICK();
    mflag = 1;  /* 01649C SEM */
    wr8(ABS(0x01BB), 0x0F);  /* 01649D LDM */
    mflag = 0;  /* 0164A1 CLM */
    Y = pull16();  /* 0164A2 PLY */
    D = pull16();  /* 0164A3 PLD */
    S += 3; return;  /* 0164A4 RTL */
}

/* 0164A5  callers: 016724 */
void F_0164A5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0164A5 LDA */
    push16(D);  /* 0164A7 PHD */
    D = A;  /* 0164A8 TAD */
    push16(A);  /* 0164A9 PHA */
    mflag = 1;  /* 0164AA SEM */
    wr8(ABS(0x01B6), 0x00);  /* 0164AB LDM */
    wr8(ABS(0x01BA), 0x00);  /* 0164AF LDM */
    SETLO(A, rd8(ABS(0x0227)));  /* 0164B3 LDA */
    if ((A & 0xFF) == 0) goto L_0164BC;  /* 0164B6 BEQ */
    wr8(ABS(0x0293), 0x00);  /* 0164B8 LDM */
L_0164BC: TICK();
    wr8(ABS(0x0227), 0x00);  /* 0164BC LDM */
    SETLO(A, rd8(ABS(0x01BE)));  /* 0164C0 LDA */
    /* 0164C3 CMP */
    if ((A & 0xFF) == 0x01) goto L_0164CF;  /* 0164C5 BEQ */
    /* 0164C7 CMP */
    if ((A & 0xFF) == 0x02) goto L_0164CF;  /* 0164C9 BEQ */
    /* 0164CB CMP */
    if ((A & 0xFF) != 0x09) goto L_016530;  /* 0164CD BNE */
L_0164CF: TICK();
    Y = 0x001A;  /* 0164CF LDY */
    mflag = 0;  /* 0164D2 CLM */
    S -= 3; F_0080AA();  /* 0164D3 JSRL */
    Y = 0x0000;  /* 0164D7 LDY */
    S -= 3; F_015FF7();  /* 0164DA JSRL */
    wr16(ABS(0x01D5), 0x0000);  /* 0164DE LDM */
    wr16(ABS(0x01D3), 0x0000);  /* 0164E3 LDM */
    wr16(ABS(0x01D1), 0x0000);  /* 0164E8 LDM */
    mflag = 1;  /* 0164ED SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0164EE LDM */
    SETLO(A, rd8(ABS(0x01BE)));  /* 0164F2 LDA */
    /* 0164F5 CMP */
    if ((A & 0xFF) != 0x01) goto L_01650A;  /* 0164F7 BNE */
    Y = 0x0002;  /* 0164F9 LDY */
    mflag = 0;  /* 0164FC CLM */
    S -= 3; F_01520C();  /* 0164FD JSRL */
    X = rd16(ABS(0x03D5));  /* 016501 LDX */
    wr16(ABS(0x01CD), X);  /* 016504 STX */
    goto L_016521;  /* 016507 JMP */
L_01650A: TICK();
    mflag = 0;  /* 01650A CLM */
    X = rd16(ABS(0x01CD));  /* 01650B LDX */
    t = rd16(ABS(0x03D5));  /* 01650E CPX */
    if (X != t) goto L_016519;  /* 016511 BNE */
    A = 0x0002;  /* 016513 LDA */
    goto L_01651C;  /* 016516 JMP */
L_016519: TICK();
    A = 0x0001;  /* 016519 LDA */
L_01651C: TICK();
    Y = A;  /* 01651C TAY */
    S -= 3; F_01520C();  /* 01651D JSRL */
L_016521: TICK();
    mflag = 1;  /* 016521 SEM */
    wr8(ABS(0x0244), rd8(ABS(0x0244)) & ~0x10);  /* 016522 CLB */
    wr8(ABS(0x03B1), 0x00);  /* 016526 LDM */
    mflag = 0;  /* 01652A CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0004);  /* 01652B SEB */
L_016530: TICK();
    mflag = 0;  /* 016530 CLM */
    A = rd16(ABS(0x01BE));  /* 016531 LDA */
    mflag = 1;  /* 016534 SEM */
    /* 016535 CMP */
    if ((A & 0xFF) != 0x0D) goto L_01655F;  /* 016537 BNE */
    mflag = 0;  /* 016539 CLM */
    S -= 3; F_015F9B();  /* 01653A JSRL */
    /* 01653E CMP */
    if ((A & 0xFF) == 0x00) goto L_01654A;  /* 016540 BEQ */
    Y = 0x0001;  /* 016542 LDY */
    mflag = 0;  /* 016545 CLM */
    S -= 3; F_015FF7();  /* 016546 JSRL */
L_01654A: TICK();
    mflag = 0;  /* 01654A CLM */
    X = rd16(ABS(0x01CD));  /* 01654B LDX */
    t = rd16(ABS(0x03D7));  /* 01654E CPX */
    if (X == t) goto L_01655F;  /* 016551 BEQ */
    Y = 0x0004;  /* 016553 LDY */
    S -= 3; F_01520C();  /* 016556 JSRL */
    mflag = 1;  /* 01655A SEM */
    wr8(ABS(0x01BA), 0x03);  /* 01655B LDM */
L_01655F: TICK();
    mflag = 0;  /* 01655F CLM */
    A = rd16(ABS(0x01BE));  /* 016560 LDA */
    mflag = 1;  /* 016563 SEM */
    /* 016564 CMP */
    if ((A & 0xFF) == 0x10) goto L_01656F;  /* 016566 BEQ */
    SETLO(A, rd8(ABS(0x01BE)));  /* 016568 LDA */
    /* 01656B CMP */
    if ((A & 0xFF) != 0x13) goto L_016586;  /* 01656D BNE */
L_01656F: TICK();
    wr8(ABS(0x01B6), 0x0F);  /* 01656F LDM */
    wr8(ABS(0x01B7), 0x20);  /* 016573 LDM */
    SETLO(A, rd8(ABS(0x01BE)));  /* 016577 LDA */
    /* 01657A CMP */
    if ((A & 0xFF) != 0x10) goto L_016586;  /* 01657C BNE */
    wr8(ABS(0x01C4), 0x1C);  /* 01657E LDM */
    wr8(ABS(0x01BA), 0x02);  /* 016582 LDM */
L_016586: TICK();
    /* 016586 CMP */
    if ((A & 0xFF) != 0x11) goto L_01659E;  /* 016588 BNE */
    wr8(ABS(0x01B6), 0x11);  /* 01658A LDM */
    wr8(ABS(0x01BC), 0x00);  /* 01658E LDM */
    wr8(ABS(0x01B7), 0x02);  /* 016592 LDM */
    wr8(ABS(0x01C4), 0x1D);  /* 016596 LDM */
    wr8(ABS(0x01BA), 0x02);  /* 01659A LDM */
L_01659E: TICK();
    /* 01659E CMP */
    if ((A & 0xFF) != 0x12) goto L_0165BF;  /* 0165A0 BNE */
    wr8(ABS(0x01E7), 0x01);  /* 0165A2 LDM */
    wr8(ABS(0x01B6), 0x09);  /* 0165A6 LDM */
    wr8(ABS(0x01BC), 0x11);  /* 0165AA LDM */
    wr8(ABS(0x01B7), 0x02);  /* 0165AE LDM */
    Y = 0x0002;  /* 0165B2 LDY */
    mflag = 0;  /* 0165B5 CLM */
    S -= 3; F_01520C();  /* 0165B6 JSRL */
    mflag = 1;  /* 0165BA SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0165BB LDM */
L_0165BF: TICK();
    SETLO(A, rd8(ABS(0x01BE)));  /* 0165BF LDA */
    /* 0165C2 CMP */
    if ((A & 0xFF) != 0x14) goto L_0165F3;  /* 0165C4 BNE */
    wr8(ABS(0x01BA), 0x03);  /* 0165C6 LDM */
    wr8(DP(0x00), 0x08);  /* 0165CA LDM */
    mflag = 0;  /* 0165CD CLM */
    wr16(DP(0x01), 0x0252);  /* 0165CE LDM */
    wr16(DP(0x03), 0x0000);  /* 0165D2 LDM */
    A = rd16(ABS(0x01BF));  /* 0165D6 LDA */
    A &= 0x00FF;  /* 0165D9 AND */
    A = asl16(A);  /* 0165DC ASL */
    A = asl16(A);  /* 0165DD ASL */
    A = asl16(A);  /* 0165DE ASL */
    C = 0;  /* 0165DF CLC */
    A = adc16(A, 0x0272);  /* 0165E0 ADC */
    wr16(DP(0x04), A);  /* 0165E3 STA */
    Y = 0x00A0;  /* 0165E5 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 0165E8 JSRL */
    Y = 0x001B;  /* 0165EC LDY */
    S -= 3; F_0080AA();  /* 0165EF JSRL */
L_0165F3: TICK();
    mflag = 0;  /* 0165F3 CLM */
    A = rd16(ABS(0x01BE));  /* 0165F4 LDA */
    mflag = 1;  /* 0165F7 SEM */
    /* 0165F8 CMP */
    if ((A & 0xFF) != 0x05) goto L_016612;  /* 0165FA BNE */
    mflag = 0;  /* 0165FC CLM */
    S -= 3; F_0156E4();  /* 0165FD JSRL */
    mflag = 1;  /* 016601 SEM */
    wr8(ABS(0x01B6), 0x00);  /* 016602 LDM */
    wr8(ABS(0x01E8), 0x00);  /* 016606 LDM */
    Y = 0x0013;  /* 01660A LDY */
    mflag = 0;  /* 01660D CLM */
    S -= 3; F_0080AA();  /* 01660E JSRL */
L_016612: TICK();
    mflag = 0;  /* 016612 CLM */
    A = rd16(ABS(0x01BE));  /* 016613 LDA */
    mflag = 1;  /* 016616 SEM */
    /* 016617 CMP */
    if ((A & 0xFF) != 0x04) goto L_016634;  /* 016619 BNE */
    wr8(ABS(0x01B6), 0x05);  /* 01661B LDM */
    wr8(ABS(0x01B7), 0x5E);  /* 01661F LDM */
    Y = 0x0000;  /* 016623 LDY */
    mflag = 0;  /* 016626 CLM */
    S -= 3; F_0080AA();  /* 016627 JSRL */
    mflag = 1;  /* 01662B SEM */
    wr8(ABS(0x01C4), 0x1E);  /* 01662C LDM */
    wr8(ABS(0x01BA), 0x02);  /* 016630 LDM */
L_016634: TICK();
    SETLO(A, rd8(ABS(0x01BE)));  /* 016634 LDA */
    /* 016637 CMP */
    if ((A & 0xFF) == 0x07) goto L_016647;  /* 016639 BEQ */
    /* 01663B CMP */
    if ((A & 0xFF) == 0x06) goto L_016647;  /* 01663D BEQ */
    /* 01663F CMP */
    if ((A & 0xFF) == 0x08) goto L_016647;  /* 016641 BEQ */
    /* 016643 CMP */
    if ((A & 0xFF) != 0x15) goto L_0166AB;  /* 016645 BNE */
L_016647: TICK();
    wr8(ABS(0x01B6), 0x06);  /* 016647 LDM */
    wr8(ABS(0x01B7), 0x5E);  /* 01664B LDM */
    wr8(ABS(0x01BA), 0x02);  /* 01664F LDM */
    /* 016653 CMP */
    if ((A & 0xFF) != 0x06) goto L_016666;  /* 016655 BNE */
    wr8(ABS(0x01C4), 0x13);  /* 016657 LDM */
    Y = 0x001C;  /* 01665B LDY */
    mflag = 0;  /* 01665E CLM */
    S -= 3; F_0080AA();  /* 01665F JSRL */
    goto L_0166AB;  /* 016663 JMP */
L_016666: TICK();
    mflag = 0;  /* 016666 CLM */
    A = rd16(ABS(0x01BE));  /* 016667 LDA */
    mflag = 1;  /* 01666A SEM */
    /* 01666B CMP */
    if ((A & 0xFF) != 0x07) goto L_01667E;  /* 01666D BNE */
    wr8(ABS(0x01C4), 0x14);  /* 01666F LDM */
    Y = 0x001D;  /* 016673 LDY */
    mflag = 0;  /* 016676 CLM */
    S -= 3; F_0080AA();  /* 016677 JSRL */
    goto L_0166AB;  /* 01667B JMP */
L_01667E: TICK();
    mflag = 0;  /* 01667E CLM */
    A = rd16(ABS(0x01BE));  /* 01667F LDA */
    mflag = 1;  /* 016682 SEM */
    /* 016683 CMP */
    if ((A & 0xFF) != 0x08) goto L_016696;  /* 016685 BNE */
    wr8(ABS(0x01C4), 0x15);  /* 016687 LDM */
    Y = 0x0013;  /* 01668B LDY */
    mflag = 0;  /* 01668E CLM */
    S -= 3; F_0080AA();  /* 01668F JSRL */
    goto L_0166AB;  /* 016693 JMP */
L_016696: TICK();
    mflag = 0;  /* 016696 CLM */
    A = rd16(ABS(0x01BE));  /* 016697 LDA */
    mflag = 1;  /* 01669A SEM */
    /* 01669B CMP */
    if ((A & 0xFF) != 0x15) goto L_0166AB;  /* 01669D BNE */
    wr8(ABS(0x01C4), 0x1F);  /* 01669F LDM */
    Y = 0x001C;  /* 0166A3 LDY */
    mflag = 0;  /* 0166A6 CLM */
    S -= 3; F_0080AA();  /* 0166A7 JSRL */
L_0166AB: TICK();
    mflag = 0;  /* 0166AB CLM */
    A = rd16(ABS(0x01BE));  /* 0166AC LDA */
    mflag = 1;  /* 0166AF SEM */
    /* 0166B0 CMP */
    if ((A & 0xFF) != 0x03) goto L_0166BC;  /* 0166B2 BNE */
    wr8(ABS(0x01C4), 0x12);  /* 0166B4 LDM */
    wr8(ABS(0x01BA), 0x02);  /* 0166B8 LDM */
L_0166BC: TICK();
    SETLO(A, rd8(ABS(0x01BE)));  /* 0166BC LDA */
    /* 0166BF CMP */
    if ((A & 0xFF) != 0x0A) goto L_0166CB;  /* 0166C1 BNE */
    wr8(ABS(0x01BA), 0x02);  /* 0166C3 LDM */
    wr8(ABS(0x01C4), 0x16);  /* 0166C7 LDM */
L_0166CB: TICK();
    /* 0166CB CMP */
    if ((A & 0xFF) != 0x0B) goto L_0166D9;  /* 0166CD BNE */
    mflag = 0;  /* 0166CF CLM */
    S -= 3; F_0156E4();  /* 0166D0 JSRL */
    mflag = 1;  /* 0166D4 SEM */
    wr8(ABS(0x01B6), 0x00);  /* 0166D5 LDM */
L_0166D9: TICK();
    SETLO(A, rd8(ABS(0x01BE)));  /* 0166D9 LDA */
    /* 0166DC CMP */
    if ((A & 0xFF) != 0x0C) goto L_0166E8;  /* 0166DE BNE */
    wr8(ABS(0x01C4), 0x0C);  /* 0166E0 LDM */
    wr8(ABS(0x01BA), 0x02);  /* 0166E4 LDM */
L_0166E8: TICK();
    /* 0166E8 CMP */
    if ((A & 0xFF) != 0x0E) goto L_0166F9;  /* 0166EA BNE */
    Y = 0x0002;  /* 0166EC LDY */
    mflag = 0;  /* 0166EF CLM */
    S -= 3; F_01520C();  /* 0166F0 JSRL */
    mflag = 1;  /* 0166F4 SEM */
    wr8(ABS(0x01BA), 0x03);  /* 0166F5 LDM */
L_0166F9: TICK();
    SETLO(A, rd8(ABS(0x01BE)));  /* 0166F9 LDA */
    /* 0166FC CMP */
    if ((A & 0xFF) != 0x0F) goto L_01670D;  /* 0166FE BNE */
    mflag = 0;  /* 016700 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0010);  /* 016701 SEB */
    Y = 0x0009;  /* 016706 LDY */
    S -= 3; F_0080AA();  /* 016709 JSRL */
L_01670D: TICK();
    mflag = 1;  /* 01670D SEM */
    wr8(ABS(0x01BE), 0x00);  /* 01670E LDM */
    mflag = 0;  /* 016712 CLM */
    Y = pull16();  /* 016713 PLY */
    D = pull16();  /* 016714 PLD */
    S += 3; return;  /* 016715 RTL */
}

/* 016716  callers: 016791 */
void F_016716(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016716 LDA */
    push16(A);  /* 016718 PHA */
    mflag = 1;  /* 016719 SEM */
    wr8(ABS(0x01BA), 0x00);  /* 01671A LDM */
    SETLO(A, rd8(ABS(0x01C0)));  /* 01671E LDA */
    if ((A & 0xFF) != 0) goto L_01672B;  /* 016721 BNE */
    mflag = 0;  /* 016723 CLM */
    S -= 3; F_0164A5();  /* 016724 JSRL */
    goto L_01674A;  /* 016728 JMP */
L_01672B: TICK();
    mflag = 1;  /* 01672B SEM */
    /* 01672C CMP */
    if ((A & 0xFF) != 0x01) goto L_016738;  /* 01672E BNE */
    mflag = 0;  /* 016730 CLM */
    S -= 3; F_015743();  /* 016731 JSRL */
    goto L_01674A;  /* 016735 JMP */
L_016738: TICK();
    mflag = 1;  /* 016738 SEM */
    /* 016739 CMP */
    if ((A & 0xFF) != 0x02) goto L_016745;  /* 01673B BNE */
    mflag = 0;  /* 01673D CLM */
    S -= 3; F_0160B8();  /* 01673E JSRL */
    goto L_01674A;  /* 016742 JMP */
L_016745: TICK();
    mflag = 1;  /* 016745 SEM */
    wr8(ABS(0x01C0), 0x00);  /* 016746 LDM */
L_01674A: TICK();
    mflag = 0;  /* 01674A CLM */
    A = rd16(ABS(0x03B5));  /* 01674B LDA */
    mflag = 1;  /* 01674E SEM */
    /* 01674F CMP */
    if ((A & 0xFF) != 0x10) goto L_016760;  /* 016751 BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 016753 LDA */
    if ((A & 0xFF) != 0) goto L_016760;  /* 016756 BNE */
    wr8(ABS(0x01C4), 0x12);  /* 016758 LDM */
    wr8(ABS(0x01BA), 0x02);  /* 01675C LDM */
L_016760: TICK();
    wr8(ABS(0x01C0), 0x00);  /* 016760 LDM */
    mflag = 0;  /* 016764 CLM */
    Y = pull16();  /* 016765 PLY */
    S += 3; return;  /* 016766 RTL */
}

/* 016767  callers: 01A79A */
void F_016767(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016767 LDA */
    push16(A);  /* 016769 PHA */
    A = rd16(ABS(0x01BA));  /* 01676A LDA */
    mflag = 1;  /* 01676D SEM */
    /* 01676E CMP */
    if ((A & 0xFF) != 0x00) goto L_01678B;  /* 016770 BNE */
    mflag = 0;  /* 016772 CLM */
    S -= 3; F_015ED0();  /* 016773 JSRL */
    mflag = 1;  /* 016777 SEM */
    SETLO(A, rd8(ABS(0x0232)));  /* 016778 LDA */
    if ((A & 0xFF) != 0) goto L_016783;  /* 01677B BNE */
    mflag = 0;  /* 01677D CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0100);  /* 01677E CLB */
L_016783: TICK();
    mflag = 0;  /* 016783 CLM */
    S -= 3; F_007A06();  /* 016784 JSRL */
    goto L_0167EF;  /* 016788 JMP */
L_01678B: TICK();
    mflag = 1;  /* 01678B SEM */
    /* 01678C CMP */
    if ((A & 0xFF) != 0x01) goto L_016798;  /* 01678E BNE */
    mflag = 0;  /* 016790 CLM */
    S -= 3; F_016716();  /* 016791 JSRL */
    goto L_0167EF;  /* 016795 JMP */
L_016798: TICK();
    mflag = 1;  /* 016798 SEM */
    /* 016799 CMP */
    if ((A & 0xFF) == 0x03) goto L_0167A9;  /* 01679B BEQ */
    /* 01679D CMP */
    if ((A & 0xFF) != 0x02) goto L_0167EA;  /* 01679F BNE */
    Y = rd16(ABS(0x01C4));  /* 0167A1 LDY */
    mflag = 0;  /* 0167A4 CLM */
    S -= 3; F_003310();  /* 0167A5 JSRL */
L_0167A9: TICK();
    mflag = 0;  /* 0167A9 CLM */
    A = rd16(ABS(0x03BA));  /* 0167AA LDA */
    mflag = 1;  /* 0167AD SEM */
    /* 0167AE CMP */
    if ((A & 0xFF) < 0x40) goto L_0167D0;  /* 0167B0 BCC */
    mflag = 0;  /* 0167B2 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 0167B3 CLB */
    mflag = 1;  /* 0167B8 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 0167B9 LDA */
    if ((A & 0xFF) == 0) goto L_0167C7;  /* 0167BC BEQ */
    mflag = 0;  /* 0167BE CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0400);  /* 0167BF CLB */
    goto L_0167CD;  /* 0167C4 JMP */
L_0167C7: TICK();
    mflag = 0;  /* 0167C7 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0400);  /* 0167C8 SEB */
L_0167CD: TICK();
    goto L_0167DB;  /* 0167CD JMP */
L_0167D0: TICK();
    mflag = 0;  /* 0167D0 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0020);  /* 0167D1 SEB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0400);  /* 0167D6 CLB */
L_0167DB: TICK();
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 0167DB SEB */
    mflag = 1;  /* 0167E0 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 0167E1 LDM */
    mflag = 0;  /* 0167E5 CLM */
    S -= 3; F_00828C();  /* 0167E6 JSRL */
L_0167EA: TICK();
    mflag = 1;  /* 0167EA SEM */
    wr8(ABS(0x01BA), 0x00);  /* 0167EB LDM */
L_0167EF: TICK();
    mflag = 0;  /* 0167EF CLM */
    Y = pull16();  /* 0167F0 PLY */
    S += 3; return;  /* 0167F1 RTL */
}

/* 0167F2  callers: 01A748 */
void F_0167F2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0167F2 LDA */
    push16(D);  /* 0167F4 PHD */
    D = A;  /* 0167F5 TAD */
    A += 1;  /* 0167F6 INC */
    A += 1;  /* 0167F7 INC */
    push16(A);  /* 0167F8 PHA */
    mflag = 1;  /* 0167F9 SEM */
    if ((rd8(ABS(0x0334)) & 0x10) == 0x10) goto L_016802;  /* 0167FA BBS */
    goto L_01690E;  /* 0167FF JMP */
L_016802: TICK();
    Y = 0x01AD;  /* 016802 LDY */
    mflag = 0;  /* 016805 CLM */
    S -= 3; F_007271();  /* 016806 JSRL */
    wr16(ABS(0x039E), A);  /* 01680A STA */
    mflag = 1;  /* 01680D SEM */
    wr8(ABS(0x01BA), 0x00);  /* 01680E LDM */
    Y = 0x055E;  /* 016812 LDY */
    mflag = 0;  /* 016815 CLM */
    S -= 3; eeprom_read_byte_007189();  /* 016816 JSRL */
    wr8(ABS(0x03B5), (uint8_t)A);  /* 01681A STA */
    Y = 0x01C7;  /* 01681D LDY */
    mflag = 0;  /* 016820 CLM */
    S -= 3; F_007271();  /* 016821 JSRL */
    wr16(ABS(0x01C5), A);  /* 016825 STA */
    Y = 0x01C9;  /* 016828 LDY */
    S -= 3; F_007271();  /* 01682B JSRL */
    wr16(ABS(0x01C7), A);  /* 01682F STA */
    Y = 0x01C4;  /* 016832 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 016835 JSRL */
    wr8(ABS(0x01C2), (uint8_t)A);  /* 016839 STA */
    Y = 0x01F4;  /* 01683C LDY */
    mflag = 0;  /* 01683F CLM */
    S -= 3; F_007271();  /* 016840 JSRL */
    wr16(ABS(0x01C9), A);  /* 016844 STA */
    Y = 0x01F6;  /* 016847 LDY */
    S -= 3; F_007271();  /* 01684A JSRL */
    wr16(ABS(0x01CB), A);  /* 01684E STA */
    Y = 0x0000;  /* 016851 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 016854 JSRL */
    SETLO(A, A & 0x20);  /* 016858 AND */
    if ((A & 0xFF) == 0) goto L_016863;  /* 01685A BEQ */
    mflag = 0;  /* 01685C CLM */
    A = 0x0001;  /* 01685D LDA */
    goto L_016867;  /* 016860 JMP */
L_016863: TICK();
    mflag = 0;  /* 016863 CLM */
    A = 0x0000;  /* 016864 LDA */
L_016867: TICK();
    mflag = 1;  /* 016867 SEM */
    wr8(ABS(0x01DD), (uint8_t)A);  /* 016868 STA */
    mflag = 0;  /* 01686B CLM */
    S -= 3; F_0156B1();  /* 01686C JSRL */
    mflag = 1;  /* 016870 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0168AB;  /* 016871 BBS */
    SETLO(B, 0x00);  /* 016876 LDA */
    Y = 0x0206;  /* 016879 LDY */
    mflag = 0;  /* 01687C CLM */
    S -= 3; F_00589D();  /* 01687D JSRL */
    SETLO(B, 0x00);  /* 016881 LDA */
    div8(0x10);  /* 016884 DIV */
    SETLO(A, A ^ 0xFF);  /* 016887 EOR */
    SETLO(A, A + 1);  /* 016889 INC */
    C = 0;  /* 01688A CLC */
    SETLO(A, adc8(A, 0x10));  /* 01688B ADC */
    wr8(ABS(0x0206), (uint8_t)A);  /* 01688D STA */
    /* 016890 CMP */
    if ((A & 0xFF) < 0x0F) goto L_0168A2;  /* 016892 BCC */
    mflag = 0;  /* 016894 CLM */
    if ((rd16(ABS(0x039E)) & 0x0400) == 0) goto L_0168A2;  /* 016895 BBC */
    S -= 3; F_015079();  /* 01689B JSRL */
    goto L_0168A8;  /* 01689F JMP */
L_0168A2: TICK();
    mflag = 0;  /* 0168A2 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0002);  /* 0168A3 SEB */
L_0168A8: TICK();
    goto L_0168FA;  /* 0168A8 JMP */
L_0168AB: TICK();
    mflag = 0;  /* 0168AB CLM */
    A = D;  /* 0168AC TDA */
    A += 1;  /* 0168AD INC */
    B = 0x0000;  /* 0168AE LDA */
    Y = A;  /* 0168B2 TAY */
    S -= 3; F_00589D();  /* 0168B3 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 0168B7 STA */
    if ((rd8(ABS(0x033F)) & 0x02) == 0) goto L_0168C5;  /* 0168B9 BBC */
    mflag = 0;  /* 0168BE CLM */
    A = 0x0001;  /* 0168BF LDA */
    goto L_0168C9;  /* 0168C2 JMP */
L_0168C5: TICK();
    mflag = 0;  /* 0168C5 CLM */
    A = 0x0000;  /* 0168C6 LDA */
L_0168C9: TICK();
    wr16(DP(0x02), A);  /* 0168C9 STA */
    mflag = 1;  /* 0168CB SEM */
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_0168D7;  /* 0168CC BBC */
    mflag = 0;  /* 0168D0 CLM */
    A = 0x0001;  /* 0168D1 LDA */
    goto L_0168DB;  /* 0168D4 JMP */
L_0168D7: TICK();
    mflag = 0;  /* 0168D7 CLM */
    A = 0x0000;  /* 0168D8 LDA */
L_0168DB: TICK();
    A ^= rd16(DP(0x02));  /* 0168DB EOR */
    mflag = 1;  /* 0168DD SEM */
    SETLO(A, A & 0x01);  /* 0168DE AND */
    if ((A & 0xFF) == 0) goto L_0168E4;  /* 0168E0 BEQ */
    SETLO(A, 0x20);  /* 0168E2 LDA */
L_0168E4: TICK();
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x20);  /* 0168E4 CLB */
    SETLO(A, A | rd8(ABS(0x0240)));  /* 0168E8 ORA */
    wr8(ABS(0x0240), (uint8_t)A);  /* 0168EB STA */
    mflag = 0;  /* 0168EE CLM */
    if ((rd16(ABS(0x039E)) & 0x0400) == 0x0400) goto L_0168FA;  /* 0168EF BBS */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0002);  /* 0168F5 SEB */
L_0168FA: TICK();
    mflag = 1;  /* 0168FA SEM */
    wr8(ABS(0x01B6), 0x06);  /* 0168FB LDM */
    mflag = 0;  /* 0168FF CLM */
    if ((rd16(ABS(0x0246)) & 0x0002) == 0) goto L_01690B;  /* 016900 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x8000);  /* 016906 CLB */
L_01690B: TICK();
    goto L_016919;  /* 01690B JMP */
L_01690E: TICK();
    mflag = 0;  /* 01690E CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0002);  /* 01690F CLB */
    wr16(ABS(0x039E), rd16(ABS(0x039E)) | 0x0400);  /* 016914 SEB */
L_016919: TICK();
    Y = pull16();  /* 016919 PLY */
    D = pull16();  /* 01691A PLD */
    S += 3; return;  /* 01691B RTL */
}

/* 01691C  callers: 0056F8, 01A767 */
void F_01691C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01691C LDA */
    push16(D);  /* 01691E PHD */
    D = A;  /* 01691F TAD */
    A += 1;  /* 016920 INC */
    push16(A);  /* 016921 PHA */
    mflag = 1;  /* 016922 SEM */
    wr8(DP(0x00), 0x00);  /* 016923 LDM */
    if ((rd8(ABS(0x0335)) & 0x10) == 0) goto L_016959;  /* 016926 BBC */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 01692B INC */
    mflag = 0;  /* 01692D CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_016959;  /* 01692E BBS */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_016959;  /* 016934 BBS */
    if ((rd16(ABS(0x0240)) & 0x4000) == 0x4000) goto L_016959;  /* 01693A BBS */
    mflag = 1;  /* 016940 SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 016941 INC */
    mflag = 0;  /* 016943 CLM */
    S -= 3; F_007F9B();  /* 016944 JSRL */
    /* 016948 CMP */
    if ((A & 0xFF) == 0x00) goto L_016959;  /* 01694A BEQ */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 01694C INC */
    mflag = 0;  /* 01694E CLM */
    S -= 3; F_0169F9();  /* 01694F JSRL */
    /* 016953 CMP */
    if ((A & 0xFF) == 0x00) goto L_016959;  /* 016955 BEQ */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 016957 INC */
L_016959: TICK();
    mflag = 0;  /* 016959 CLM */
    A = rd16(DP(0x00));  /* 01695A LDA */
    mflag = 1;  /* 01695C SEM */
    SETLO(A, A - 1);  /* 01695D DEC */
    if ((A & 0xFF) != 0) goto L_016974;  /* 01695E BNE */
    Y = 0x000D;  /* 016960 LDY */
    mflag = 0;  /* 016963 CLM */
    S -= 3; F_0080AA();  /* 016964 JSRL */
    mflag = 1;  /* 016968 SEM */
    wr8(ABS(0x0293), 0x00);  /* 016969 LDM */
    wr8(ABS(0x0226), 0x0F);  /* 01696D LDM */
    goto L_0169F2;  /* 016971 JMP */
L_016974: TICK();
    mflag = 1;  /* 016974 SEM */
    /* 016975 CMP */
    if ((A & 0xFF) != 0x01) goto L_0169A5;  /* 016977 BNE */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) & ~0x10);  /* 016979 CLB */
    mflag = 0;  /* 01697D CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0004);  /* 01697E SEB */
    Y = 0x0004;  /* 016983 LDY */
    S -= 3; F_003310();  /* 016986 JSRL */
    S -= 3; F_009782();  /* 01698A JSRL */
    Y = 0x0BB8;  /* 01698E LDY */
    S -= 3; F_007D7F();  /* 016991 JSRL */
    mflag = 1;  /* 016995 SEM */
    wr8(ABS(0x0231), 0x14);  /* 016996 LDM */
    wr8(ABS(0x0230), 0x00);  /* 01699A LDM */
    wr8(ABS(0x0226), 0x0F);  /* 01699E LDM */
    goto L_0169F2;  /* 0169A2 JMP */
L_0169A5: TICK();
    mflag = 1;  /* 0169A5 SEM */
    /* 0169A6 CMP */
    if ((A & 0xFF) != 0x02) goto L_0169D8;  /* 0169A8 BNE */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) & ~0x10);  /* 0169AA CLB */
    Y = 0x0003;  /* 0169AE LDY */
    mflag = 0;  /* 0169B1 CLM */
    S -= 3; F_003310();  /* 0169B2 JSRL */
    Y = 0x0001;  /* 0169B6 LDY */
    S -= 3; F_0080AA();  /* 0169B9 JSRL */
    S -= 3; F_00828C();  /* 0169BD JSRL */
    Y = 0x0BB8;  /* 0169C1 LDY */
    S -= 3; F_007D7F();  /* 0169C4 JSRL */
    mflag = 1;  /* 0169C8 SEM */
    wr8(ABS(0x0231), 0x14);  /* 0169C9 LDM */
    wr8(ABS(0x0230), 0x00);  /* 0169CD LDM */
    wr8(ABS(0x0226), 0x0F);  /* 0169D1 LDM */
    goto L_0169F2;  /* 0169D5 JMP */
L_0169D8: TICK();
    mflag = 1;  /* 0169D8 SEM */
    /* 0169D9 CMP */
    if ((A & 0xFF) != 0x03) goto L_0169F2;  /* 0169DB BNE */
    mflag = 0;  /* 0169DD CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0040);  /* 0169DE SEB */
    S -= 3; F_016AB1();  /* 0169E3 JSRL */
    S -= 3; F_016F7B();  /* 0169E7 JSRL */
    S -= 3; F_016BD2();  /* 0169EB JSRL */
    goto L_0169F2;  /* 0169EF JMP */
L_0169F2: TICK();
    mflag = 0;  /* 0169F2 CLM */
    A = rd16(DP(0x00));  /* 0169F3 LDA */
    mflag = 1;  /* 0169F5 SEM */
    Y = pull16();  /* 0169F6 PLY */
    D = pull16();  /* 0169F7 PLD */
    S += 3; return;  /* 0169F8 RTL */
}

/* 0169F9  callers: 01694F */
void F_0169F9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0169F9 LDA */
    push16(D);  /* 0169FB PHD */
    D = A;  /* 0169FC TAD */
    C = 0;  /* 0169FD CLC */
    A = adc16(A, 0x001B);  /* 0169FE ADC */
    push16(A);  /* 016A01 PHA */
    wr16(DP(0x1B), 0x0016);  /* 016A02 LDM */
    A = D;  /* 016A06 TDA */
    A += 1;  /* 016A07 INC */
    B = 0x0000;  /* 016A08 LDA */
    wr16(DP(0x1C), A);  /* 016A0C STA */
    wr16(DP(0x1E), B);  /* 016A0E STA */
    wr16(DP(0x1F), 0x07E0);  /* 016A11 LDM */
    Y = 0x00A0;  /* 016A15 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 016A18 JSRL */
    wr16(DP(0x17), 0x0000);  /* 016A1C LDM */
    mflag = 1;  /* 016A20 SEM */
    wr8(DP(0x00), 0x00);  /* 016A21 LDM */
    mflag = 0;  /* 016A24 CLM */
    wr16(DP(0x19), 0x0000);  /* 016A25 LDM */
L_016A29: TICK();
    X = rd16(DP(0x19));  /* 016A29 LDX */
    C = (X >= 0x0014);  /* 016A2B CPX */
    if (C) goto L_016A3F;  /* 016A2E BCS */
    A = rd16(DP(0x01 + X));  /* 016A30 LDA */
    A &= 0x00FF;  /* 016A32 AND */
    A = adc16(A, rd16(DP(0x17)));  /* 016A35 ADC */
    wr16(DP(0x17), A);  /* 016A37 STA */
    X += 1;  /* 016A39 INX */
    wr16(DP(0x19), X);  /* 016A3A STX */
    goto L_016A29;  /* 016A3C JMP */
L_016A3F: TICK();
    mflag = 0;  /* 016A3F CLM */
    A = rd16(DP(0x15));  /* 016A40 LDA */
    A &= 0x00FF;  /* 016A42 AND */
    A = rla16(A, 0x0008);  /* 016A45 RLA */
    A &= 0xFF00;  /* 016A49 AND */
    wr16(DP(0x1B), A);  /* 016A4C STA */
    A = rd16(DP(0x16));  /* 016A4E LDA */
    A &= 0x00FF;  /* 016A50 AND */
    C = 0;  /* 016A53 CLC */
    A = adc16(A, rd16(DP(0x1B)));  /* 016A54 ADC */
    t = rd16(DP(0x17));  /* 016A56 CMP */
    if (A != t) goto L_016AAA;  /* 016A58 BNE */
    mflag = 1;  /* 016A5A SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_016A7E;  /* 016A5B BBC */
    SETLO(A, rd8(DP(0x07)));  /* 016A60 LDA */
    SETLO(A, A & 0x3F);  /* 016A62 AND */
    mflag = 0;  /* 016A64 CLM */
    A &= 0x00FF;  /* 016A65 AND */
    A = rla16(A, 0x0008);  /* 016A68 RLA */
    A &= 0xFF00;  /* 016A6C AND */
    wr16(DP(0x1B), A);  /* 016A6F STA */
    A = rd16(DP(0x08));  /* 016A71 LDA */
    A &= 0x00FF;  /* 016A73 AND */
    C = 0;  /* 016A76 CLC */
    A = adc16(A, rd16(DP(0x1B)));  /* 016A77 ADC */
    /* 016A79 CMP */
    if (A != 0x3FFF) goto L_016AA2;  /* 016A7C BNE */
L_016A7E: TICK();
    mflag = 1;  /* 016A7E SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_016AAA;  /* 016A7F BBS */
    SETLO(A, rd8(DP(0x04)));  /* 016A84 LDA */
    SETLO(A, A & 0x3F);  /* 016A86 AND */
    mflag = 0;  /* 016A88 CLM */
    A &= 0x00FF;  /* 016A89 AND */
    A = rla16(A, 0x0008);  /* 016A8C RLA */
    A &= 0xFF00;  /* 016A90 AND */
    wr16(DP(0x1B), A);  /* 016A93 STA */
    A = rd16(DP(0x05));  /* 016A95 LDA */
    A &= 0x00FF;  /* 016A97 AND */
    C = 0;  /* 016A9A CLC */
    A = adc16(A, rd16(DP(0x1B)));  /* 016A9B ADC */
    /* 016A9D CMP */
    if (A == 0x3FFF) goto L_016AAA;  /* 016AA0 BEQ */
L_016AA2: TICK();
    mflag = 1;  /* 016AA2 SEM */
    SETLO(A, rd8(DP(0x03)));  /* 016AA3 LDA */
    if ((A & 0xFF) == 0) goto L_016AAA;  /* 016AA5 BEQ */
    wr8(DP(0x00), 0x01);  /* 016AA7 LDM */
L_016AAA: TICK();
    mflag = 0;  /* 016AAA CLM */
    A = rd16(DP(0x00));  /* 016AAB LDA */
    mflag = 1;  /* 016AAD SEM */
    Y = pull16();  /* 016AAE PLY */
    D = pull16();  /* 016AAF PLD */
    S += 3; return;  /* 016AB0 RTL */
}

/* 016AB1  callers: 0169E3 */
void F_016AB1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016AB1 LDA */
    push16(D);  /* 016AB3 PHD */
    D = A;  /* 016AB4 TAD */
    push16(A);  /* 016AB5 PHA */
    Y = 0x0000;  /* 016AB6 LDY */
    S -= 3; F_0080AA();  /* 016AB9 JSRL */
    mflag = 1;  /* 016ABD SEM */
    wr8(ABS(0x0230), 0x00);  /* 016ABE LDM */
    wr8(ABS(0x0226), 0x0F);  /* 016AC2 LDM */
    Y = 0x0000;  /* 016AC6 LDY */
    mflag = 0;  /* 016AC9 CLM */
    S -= 3; F_01DA02();  /* 016ACA JSRL */
    S -= 3; F_00EA66();  /* 016ACE JSRL */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 016AD2 CLB */
    mflag = 1;  /* 016AD7 SEM */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x20);  /* 016AD8 CLB */
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x80);  /* 016ADC CLB */
    wr8(ABS(0x0241), rd8(ABS(0x0241)) & ~0x80);  /* 016AE0 CLB */
    wr8(ABS(0x023B), rd8(ABS(0x023B)) & ~0x80);  /* 016AE4 CLB */
    wr8(ABS(0x023B), rd8(ABS(0x023B)) & ~0x02);  /* 016AE8 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x01);  /* 016AEC CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x01);  /* 016AF0 CLB */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x08);  /* 016AF4 CLB */
    wr8(ABS(0x023B), rd8(ABS(0x023B)) & ~0x20);  /* 016AF8 CLB */
    mflag = 0;  /* 016AFC CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 016AFD CLB */
    mflag = 1;  /* 016B02 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 016B03 LDM */
    mflag = 0;  /* 016B07 CLM */
    S -= 3; F_016C8A();  /* 016B08 JSRL */
    A = rd16(ABS(0x0375));  /* 016B0C LDA */
    mflag = 1;  /* 016B0F SEM */
    SETLO(A, A & 0x3F);  /* 016B10 AND */
    if ((A & 0xFF) == 0) goto L_016B1A;  /* 016B12 BEQ */
    mflag = 0;  /* 016B14 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 016B15 SEB */
L_016B1A: TICK();
    mflag = 0;  /* 016B1A CLM */
    A = rd16(ABS(0x0200));  /* 016B1B LDA */
    mflag = 1;  /* 016B1E SEM */
    wr8(ABS(0x019C), (uint8_t)A);  /* 016B1F STA */
    Y = 0x0006;  /* 016B22 LDY */
    mflag = 0;  /* 016B25 CLM */
    S -= 3; F_00927F();  /* 016B26 JSRL */
    mflag = 1;  /* 016B2A SEM */
    wr8(ABS(0x020A), 0x00);  /* 016B2B LDM */
    mflag = 0;  /* 016B2F CLM */
    wr16(ABS(0x021A), 0x0000);  /* 016B30 LDM */
    wr16(ABS(0x021C), 0x0000);  /* 016B35 LDM */
    mflag = 1;  /* 016B3A SEM */
    wr8(ABS(0x01A2), 0xC4);  /* 016B3B LDM */
    wr8(ABS(0x01A3), 0xD7);  /* 016B3F LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_016BC5;  /* 016B43 BBS */
    wr8(ABS(INT1IC), 0x00);  /* 016B48 LDM */
    wr8(ABS(0x01AB), 0x00);  /* 016B4C LDM */
    wr8(ABS(0x0229), 0x01);  /* 016B50 LDM */
    wr8(ABS(0x01A1), 0x00);  /* 016B54 LDM */
    wr8(ABS(0x019D), 0x00);  /* 016B58 LDM */
    mflag = 0;  /* 016B5C CLM */
    wr16(DP(0x00), 0x00D7);  /* 016B5D LDM */
    wr16(DP(0x02), 0x00C4);  /* 016B61 LDM */
    wr16(DP(0x04), 0x0044);  /* 016B65 LDM */
    Y = 0x0003;  /* 016B69 LDY */
    A = D;  /* 016B6C TDA */
    C = 0;  /* 016B6D CLC */
    A = adc16(A, 0x0006);  /* 016B6E ADC */
    push16(A);  /* 016B71 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 016B72 JSRL */
    Y = pull16();  /* 016B76 PLY */
    mflag = 0;  /* 016B77 CLM */
    wr16(DP(0x00), 0x001A);  /* 016B78 LDM */
    wr16(DP(0x02), 0x0040);  /* 016B7C LDM */
    Y = 0x0002;  /* 016B80 LDY */
    A = D;  /* 016B83 TDA */
    C = 0;  /* 016B84 CLC */
    A = adc16(A, 0x0004);  /* 016B85 ADC */
    push16(A);  /* 016B88 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 016B89 JSRL */
    Y = pull16();  /* 016B8D PLY */
    wr8(ABS(0x01AA), 0x00);  /* 016B8E LDM */
    mflag = 0;  /* 016B92 CLM */
    X = rd16(ABS(0x0197));  /* 016B93 LDX */
    if (X == 0) goto L_016BC2;  /* 016B96 BEQ */
    Y = 0x055D;  /* 016B98 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 016B9B JSRL */
    /* 016B9F CMP */
    if ((A & 0xFF) != 0xA5) goto L_016BC2;  /* 016BA1 BNE */
    wr8(DP(0x00), 0x00);  /* 016BA3 LDM */
    Y = rd16(ABS(0x019A));  /* 016BA6 LDY */
    mflag = 0;  /* 016BA9 CLM */
    S -= 3; F_016F45();  /* 016BAA JSRL */
    mflag = 1;  /* 016BAE SEM */
    wr8(ABS(0x01AB), 0x01);  /* 016BAF LDM */
    wr8(ABS(0x01A1), 0x01);  /* 016BB3 LDM */
    mflag = 0;  /* 016BB7 CLM */
    wr16(ABS(0x021C), 0x0020);  /* 016BB8 LDM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0020);  /* 016BBD SEB */
L_016BC2: TICK();
    goto L_016BCA;  /* 016BC2 JMP */
L_016BC5: TICK();
    mflag = 0;  /* 016BC5 CLM */
    S -= 3; F_017878();  /* 016BC6 JSRL */
L_016BCA: TICK();
    mflag = 0;  /* 016BCA CLM */
    S -= 3; F_0170D4();  /* 016BCB JSRL */
    Y = pull16();  /* 016BCF PLY */
    D = pull16();  /* 016BD0 PLD */
    S += 3; return;  /* 016BD1 RTL */
}

/* 016BD2  callers: 0169EB */
void F_016BD2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016BD2 LDA */
    push16(D);  /* 016BD4 PHD */
    D = A;  /* 016BD5 TAD */
    C = 0;  /* 016BD6 CLC */
    A = adc16(A, 0x0004);  /* 016BD7 ADC */
    push16(A);  /* 016BDA PHA */
    wr16(DP(0x00), 0x0004);  /* 016BDB LDM */
    wr16(DP(0x02), 0x0040);  /* 016BDF LDM */
    Y = 0x0002;  /* 016BE3 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 016BE6 JSRL */
    mflag = 0;  /* 016BEA CLM */
    A = D;  /* 016BEB TDA */
    push16(A);  /* 016BEC PHA */
    S -= 3; F_00EA66();  /* 016BED JSRL */
    Y = pull16();  /* 016BF1 PLY */
    mflag = 1;  /* 016BF2 SEM */
    wr8(ABS(0x020A), 0x00);  /* 016BF3 LDM */
    mflag = 0;  /* 016BF7 CLM */
    wr16(ABS(0x021A), 0x0000);  /* 016BF8 LDM */
    wr16(ABS(0x021C), 0x0000);  /* 016BFD LDM */
    mflag = 1;  /* 016C02 SEM */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x04);  /* 016C03 CLB */
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x10);  /* 016C07 CLB */
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x08);  /* 016C0B CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x80);  /* 016C0F CLB */
    mflag = 0;  /* 016C13 CLM */
    A = D;  /* 016C14 TDA */
    push16(A);  /* 016C15 PHA */
    S -= 3; F_009134();  /* 016C16 JSRL */
    Y = pull16();  /* 016C1A PLY */
    wr8(ABS(0x0369), (uint8_t)A);  /* 016C1B STA */
    wr8(ABS(0x0230), 0x00);  /* 016C1E LDM */
    wr8(ABS(0x0293), 0x00);  /* 016C22 LDM */
    wr8(ABS(0x0233), 0x00);  /* 016C26 LDM */
    wr8(ABS(0x0226), 0x0F);  /* 016C2A LDM */
    wr8(ABS(0x0231), 0x16);  /* 016C2E LDM */
    wr8(ABS(0x0227), 0x00);  /* 016C32 LDM */
    mflag = 0;  /* 016C36 CLM */
    A = 0x0000;  /* 016C37 LDA */
    A = asl16(A);  /* 016C3A ASL */
    A = asl16(A);  /* 016C3B ASL */
    A += 1;  /* 016C3C INC */
    X = A;  /* 016C3D TAX */
    A = rd16((0x00319E + X));  /* 016C3E LDA */
    mflag = 1;  /* 016C42 SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 016C43 STA */
    mflag = 0;  /* 016C46 CLM */
    A = rd16((0x00319C + X));  /* 016C47 LDA */
    wr16(ABS(0x02DF), A);  /* 016C4B STA */
    mflag = 1;  /* 016C4E SEM */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x20);  /* 016C4F CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x01);  /* 016C53 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x20);  /* 016C57 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x04);  /* 016C5B CLB */
    Y = 0x0000;  /* 016C5F LDY */
    mflag = 0;  /* 016C62 CLM */
    A = D;  /* 016C63 TDA */
    push16(A);  /* 016C64 PHA */
    S -= 3; F_0069EF();  /* 016C65 JSRL */
    Y = pull16();  /* 016C69 PLY */
    /* 016C6A CMP */
    if ((A & 0xFF) == 0x0F) goto L_016C74;  /* 016C6C BEQ */
    mflag = 0;  /* 016C6E CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x8000);  /* 016C6F SEB */
L_016C74: TICK();
    mflag = 0;  /* 016C74 CLM */
    A = rd16(ABS(0x019C));  /* 016C75 LDA */
    mflag = 1;  /* 016C78 SEM */
    wr8(ABS(0x0200), (uint8_t)A);  /* 016C79 STA */
    Y = 0x0006;  /* 016C7C LDY */
    mflag = 0;  /* 016C7F CLM */
    A = D;  /* 016C80 TDA */
    push16(A);  /* 016C81 PHA */
    S -= 3; F_00927F();  /* 016C82 JSRL */
    Y = pull16();  /* 016C86 PLY */
    Y = pull16();  /* 016C87 PLY */
    D = pull16();  /* 016C88 PLD */
    S += 3; return;  /* 016C89 RTL */
}

/* 016C8A  callers: 016B08 */
void F_016C8A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016C8A LDA */
    push16(D);  /* 016C8C PHD */
    D = A;  /* 016C8D TAD */
    C = 0;  /* 016C8E CLC */
    A = adc16(A, 0x0021);  /* 016C8F ADC */
    push16(A);  /* 016C92 PHA */
    wr16(DP(0x21), 0x0014);  /* 016C93 LDM */
    A = D;  /* 016C97 TDA */
    B = 0x0000;  /* 016C98 LDA */
    wr16(DP(0x22), A);  /* 016C9C STA */
    wr16(DP(0x24), B);  /* 016C9E STA */
    wr16(DP(0x25), 0x07E0);  /* 016CA1 LDM */
    Y = 0x00A0;  /* 016CA5 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 016CA8 JSRL */
    A = rd16(DP(0x02));  /* 016CAC LDA */
    mflag = 1;  /* 016CAE SEM */
    wr8(ABS(0x0199), (uint8_t)A);  /* 016CAF STA */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_016CBC;  /* 016CB2 BBC */
    SETLO(A, 0x36);  /* 016CB7 LDA */
    goto L_016CBE;  /* 016CB9 JMP */
L_016CBC: TICK();
    SETLO(A, 0x55);  /* 016CBC LDA */
L_016CBE: TICK();
    wr8(ABS(0x0376), (uint8_t)A);  /* 016CBE STA */
    SETLO(A, rd8(DP(0x03)));  /* 016CC1 LDA */
    SETLO(A, A & 0x3F);  /* 016CC3 AND */
    mflag = 0;  /* 016CC5 CLM */
    A &= 0x00FF;  /* 016CC6 AND */
    A = rla16(A, 0x0008);  /* 016CC9 RLA */
    A &= 0xFF00;  /* 016CCD AND */
    wr16(DP(0x21), A);  /* 016CD0 STA */
    A = rd16(DP(0x04));  /* 016CD2 LDA */
    A &= 0x00FF;  /* 016CD4 AND */
    C = 0;  /* 016CD7 CLC */
    A = adc16(A, rd16(DP(0x21)));  /* 016CD8 ADC */
    /* 016CDA CMP */
    if (A != 0x3FFF) goto L_016CE7;  /* 016CDD BNE */
    mflag = 1;  /* 016CDF SEM */
    wr8(ABS(0x0372), 0xFF);  /* 016CE0 LDM */
    goto L_016D58;  /* 016CE4 JMP */
L_016CE7: TICK();
    mflag = 0;  /* 016CE7 CLM */
    X = rd16(ABS(0x01F9));  /* 016CE8 LDX */
    wr16(DP(0x21), X);  /* 016CEB STX */
    A = rd16(DP(0x04));  /* 016CED LDA */
    wr16(DP(0x23), A);  /* 016CEF STA */
    A = rd16(DP(0x03));  /* 016CF1 LDA */
    wr16(DP(0x24), A);  /* 016CF3 STA */
    A = D;  /* 016CF5 TDA */
    C = 0;  /* 016CF6 CLC */
    A = adc16(A, 0x001C);  /* 016CF7 ADC */
    B = 0x0000;  /* 016CFA LDA */
    Y = A;  /* 016CFE TAY */
    S -= 3; F_0088B8();  /* 016CFF JSRL */
    wr16(DP(0x21), A);  /* 016D03 STA */
    A = D;  /* 016D05 TDA */
    C = 0;  /* 016D06 CLC */
    A = adc16(A, 0x0017);  /* 016D07 ADC */
    wr16(DP(0x23), A);  /* 016D0A STA */
    mflag = 1;  /* 016D0C SEM */
    wr8(DP(0x25), (uint8_t)B);  /* 016D0D STA */
    mflag = 0;  /* 016D10 CLM */
    A = rd16(DP(0x21));  /* 016D11 LDA */
    wr16(DP(0x26), A);  /* 016D13 STA */
    A = rd16(DP(0x25));  /* 016D15 LDA */
    mflag = 1;  /* 016D17 SEM */
    wr8(DP(0x28), (uint8_t)A);  /* 016D18 STA */
    mflag = 0;  /* 016D1A CLM */
    A = rd16(DP(0x23));  /* 016D1B LDA */
    wr16(DP(0x29), A);  /* 016D1D STA */
    mflag = 1;  /* 016D1F SEM */
    wr8(DP(0x2B), 0x00);  /* 016D20 LDM */
    mflag = 0;  /* 016D23 CLM */
    A = 0x0026;  /* 016D24 LDA */
    Y = 0x0005;  /* 016D27 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 016D2A JSRL */
    B = rd16(DP(0x19));  /* 016D2E LDA */
    Y = rd16(DP(0x17));  /* 016D31 LDY */
    S -= 3; F_0087DE();  /* 016D33 JSRL */
    wr16(ABS(0x0370), A);  /* 016D37 STA */
    B = rd16(DP(0x19));  /* 016D3A LDA */
    Y = rd16(DP(0x17));  /* 016D3D LDY */
    S -= 3; F_008825();  /* 016D3F JSRL */
    wr8(ABS(0x0372), (uint8_t)A);  /* 016D43 STA */
    Y = rd16(DP(0x1B));  /* 016D46 LDY */
    mflag = 0;  /* 016D48 CLM */
    S -= 3; F_008854();  /* 016D49 JSRL */
    wr16(ABS(0x0373), A);  /* 016D4D STA */
    A = rd16(DP(0x05));  /* 016D50 LDA */
    mflag = 1;  /* 016D52 SEM */
    SETLO(A, A & 0x7F);  /* 016D53 AND */
    wr8(ABS(0x0375), (uint8_t)A);  /* 016D55 STA */
L_016D58: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_016D64;  /* 016D58 BBS */
    wr8(ABS(0x0379), 0xFF);  /* 016D5D LDM */
    goto L_016E05;  /* 016D61 JMP */
L_016D64: TICK();
    mflag = 0;  /* 016D64 CLM */
    A = rd16(DP(0x06));  /* 016D65 LDA */
    mflag = 1;  /* 016D67 SEM */
    SETLO(A, A & 0x3F);  /* 016D68 AND */
    mflag = 0;  /* 016D6A CLM */
    A &= 0x00FF;  /* 016D6B AND */
    A = rla16(A, 0x0008);  /* 016D6E RLA */
    A &= 0xFF00;  /* 016D72 AND */
    wr16(DP(0x21), A);  /* 016D75 STA */
    A = rd16(DP(0x07));  /* 016D77 LDA */
    A &= 0x00FF;  /* 016D79 AND */
    C = 0;  /* 016D7C CLC */
    A = adc16(A, rd16(DP(0x21)));  /* 016D7D ADC */
    B = 0x0000;  /* 016D7F LDA */
    div16(0x0028);  /* 016D83 DIV */
    wr16(ABS(0x0220), A);  /* 016D87 STA */
    wr16(DP(0x21), 0x0000);  /* 016D8A LDM */
    A = rd16(DP(0x07));  /* 016D8E LDA */
    wr16(DP(0x23), A);  /* 016D90 STA */
    A = rd16(DP(0x06));  /* 016D92 LDA */
    wr16(DP(0x24), A);  /* 016D94 STA */
    A = D;  /* 016D96 TDA */
    C = 0;  /* 016D97 CLC */
    A = adc16(A, 0x001C);  /* 016D98 ADC */
    B = 0x0000;  /* 016D9B LDA */
    Y = A;  /* 016D9F TAY */
    S -= 3; F_0088B8();  /* 016DA0 JSRL */
    wr16(DP(0x21), A);  /* 016DA4 STA */
    A = D;  /* 016DA6 TDA */
    C = 0;  /* 016DA7 CLC */
    A = adc16(A, 0x0017);  /* 016DA8 ADC */
    wr16(DP(0x23), A);  /* 016DAB STA */
    mflag = 1;  /* 016DAD SEM */
    wr8(DP(0x25), (uint8_t)B);  /* 016DAE STA */
    mflag = 0;  /* 016DB1 CLM */
    A = rd16(DP(0x21));  /* 016DB2 LDA */
    wr16(DP(0x26), A);  /* 016DB4 STA */
    A = rd16(DP(0x25));  /* 016DB6 LDA */
    mflag = 1;  /* 016DB8 SEM */
    wr8(DP(0x28), (uint8_t)A);  /* 016DB9 STA */
    mflag = 0;  /* 016DBB CLM */
    A = rd16(DP(0x23));  /* 016DBC LDA */
    wr16(DP(0x29), A);  /* 016DBE STA */
    mflag = 1;  /* 016DC0 SEM */
    wr8(DP(0x2B), 0x00);  /* 016DC1 LDM */
    mflag = 0;  /* 016DC4 CLM */
    A = 0x0026;  /* 016DC5 LDA */
    Y = 0x0005;  /* 016DC8 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 016DCB JSRL */
    B = rd16(DP(0x19));  /* 016DCF LDA */
    Y = rd16(DP(0x17));  /* 016DD2 LDY */
    S -= 3; F_0087DE();  /* 016DD4 JSRL */
    wr16(ABS(0x0377), A);  /* 016DD8 STA */
    B = rd16(DP(0x19));  /* 016DDB LDA */
    Y = rd16(DP(0x17));  /* 016DDE LDY */
    S -= 3; F_008825();  /* 016DE0 JSRL */
    wr8(ABS(0x0379), (uint8_t)A);  /* 016DE4 STA */
    Y = rd16(DP(0x1B));  /* 016DE7 LDY */
    mflag = 0;  /* 016DE9 CLM */
    S -= 3; F_008854();  /* 016DEA JSRL */
    wr16(ABS(0x037A), A);  /* 016DEE STA */
    mflag = 1;  /* 016DF1 SEM */
    wr8(ABS(0x037C), 0x00);  /* 016DF2 LDM */
    SETLO(A, rd8(DP(0x08)));  /* 016DF6 LDA */
    SETLO(A, A & 0x3F);  /* 016DF8 AND */
    wr8(ABS(0x018A), (uint8_t)A);  /* 016DFA STA */
    if ((rd8(DP(0x09)) & 0x01) == 0) goto L_016E05;  /* 016DFD BBC */
    wr8(ABS(0x037C), rd8(ABS(0x037C)) | 0x80);  /* 016E01 SEB */
L_016E05: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 016E05 LDA */
    mflag = 0;  /* 016E07 CLM */
    A &= 0x00FF;  /* 016E08 AND */
    A = rla16(A, 0x0008);  /* 016E0B RLA */
    A &= 0xFF00;  /* 016E0F AND */
    wr16(DP(0x21), A);  /* 016E12 STA */
    A = rd16(DP(0x01));  /* 016E14 LDA */
    A &= 0x00FF;  /* 016E16 AND */
    C = 0;  /* 016E19 CLC */
    A = adc16(A, rd16(DP(0x21)));  /* 016E1A ADC */
    wr16(ABS(0x0195), A);  /* 016E1C STA */
    wr16(ABS(0x0197), 0x0000);  /* 016E1F LDM */
    mflag = 1;  /* 016E24 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_016E2D;  /* 016E25 BBC */
    goto L_016EB4;  /* 016E2A JMP */
L_016E2D: TICK();
    mflag = 0;  /* 016E2D CLM */
    wr16(DP(0x15), 0x0000);  /* 016E2E LDM */
L_016E32: TICK();
    X = rd16(DP(0x15));  /* 016E32 LDX */
    /* 016E34 CPX */
    if (X >= 0x0006) goto L_016E72;  /* 016E37 BCS */
    Y = X;  /* 016E39 TXY */
    S -= 3; F_0069BB();  /* 016E3A JSRL */
    wr8(DP(0x14), (uint8_t)A);  /* 016E3E STA */
    /* 016E40 CMP */
    if ((A & 0xFF) != 0x0F) goto L_016E47;  /* 016E42 BNE */
    goto L_016E72;  /* 016E44 JMP */
L_016E47: TICK();
    mflag = 0;  /* 016E47 CLM */
    A = rd16(DP(0x14));  /* 016E48 LDA */
    mflag = 1;  /* 016E4A SEM */
    /* 016E4B CMP */
    if ((A & 0xFF) < 0x0A) goto L_016E58;  /* 016E4D BCC */
    mflag = 0;  /* 016E4F CLM */
    wr16(ABS(0x0197), 0x0000);  /* 016E50 LDM */
    goto L_016E72;  /* 016E55 JMP */
L_016E58: TICK();
    mflag = 0;  /* 016E58 CLM */
    A = rd16(ABS(0x0197));  /* 016E59 LDA */
    mpy16(0x000A);  /* 016E5C MPY */
    wr16(DP(0x21), A);  /* 016E60 STA */
    A = rd16(DP(0x14));  /* 016E62 LDA */
    A &= 0x00FF;  /* 016E64 AND */
    C = 0;  /* 016E67 CLC */
    A = adc16(A, rd16(DP(0x21)));  /* 016E68 ADC */
    wr16(ABS(0x0197), A);  /* 016E6A STA */
    t = (rd16(DP(0x15)) + 1) & 0xFFFF; wr16(DP(0x15), t);  /* 016E6D INC */
    goto L_016E32;  /* 016E6F JMP */
L_016E72: TICK();
    mflag = 0;  /* 016E72 CLM */
    X = rd16(ABS(0x0197));  /* 016E73 LDX */
    /* 016E76 CPX */
    if (X >= 0x0800) goto L_016E80;  /* 016E79 BCS */
    /* 016E7B CPX */
    if (X >= 0x0064) goto L_016E85;  /* 016E7E BCS */
L_016E80: TICK();
    wr16(ABS(0x0197), 0x0000);  /* 016E80 LDM */
L_016E85: TICK();
    wr16(DP(0x15), 0x0000);  /* 016E85 LDM */
L_016E89: TICK();
    X = rd16(DP(0x15));  /* 016E89 LDX */
    /* 016E8B CPX */
    if (X >= 0x000A) goto L_016EAC;  /* 016E8E BCS */
    A = rd16(DP(0x0A + X));  /* 016E90 LDA */
    mflag = 1;  /* 016E92 SEM */
    wr8(DP(0x14), (uint8_t)A);  /* 016E93 STA */
    /* 016E95 CMP */
    if ((A & 0xFF) == 0x00) goto L_016EA0;  /* 016E97 BEQ */
    /* 016E99 CMP */
    if ((A & 0xFF) >= 0x64) goto L_016EA0;  /* 016E9B BCS */
    goto L_016EA2;  /* 016E9D JMP */
L_016EA0: TICK();
    SETLO(A, 0x63);  /* 016EA0 LDA */
L_016EA2: TICK();
    wr8((ABS(0x018B) + X), (uint8_t)A);  /* 016EA2 STA */
    X += 1;  /* 016EA5 INX */
    wr16(DP(0x15), X);  /* 016EA6 STX */
    mflag = 0;  /* 016EA8 CLM */
    goto L_016E89;  /* 016EA9 JMP */
L_016EAC: TICK();
    mflag = 0;  /* 016EAC CLM */
    S -= 3; F_016EB8();  /* 016EAD JSRL */
    wr8(ABS(0x019A), (uint8_t)A);  /* 016EB1 STA */
L_016EB4: TICK();
    mflag = 0;  /* 016EB4 CLM */
    Y = pull16();  /* 016EB5 PLY */
    D = pull16();  /* 016EB6 PLD */
    S += 3; return;  /* 016EB7 RTL */
}

/* 016EB8  callers: 016EAD */
void F_016EB8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016EB8 LDA */
    push16(D);  /* 016EBA PHD */
    D = A;  /* 016EBB TAD */
    A += 1;  /* 016EBC INC */
    A += 1;  /* 016EBD INC */
    A += 1;  /* 016EBE INC */
    push16(A);  /* 016EBF PHA */
    wr16(DP(0x03), 0x0003);  /* 016EC0 LDM */
    A = D;  /* 016EC4 TDA */
    B = 0x0000;  /* 016EC5 LDA */
    wr16(DP(0x04), A);  /* 016EC9 STA */
    wr16(DP(0x06), B);  /* 016ECB STA */
    wr16(DP(0x07), 0x055A);  /* 016ECE LDM */
    Y = 0x00A0;  /* 016ED2 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 016ED5 JSRL */
    A = rd16(DP(0x00));  /* 016ED9 LDA */
    mflag = 1;  /* 016EDB SEM */
    t = rd8(DP(0x01));  /* 016EDC CMP */
    if ((A & 0xFF) == t) goto L_016EF3;  /* 016EDE BEQ */
    SETLO(A, rd8(DP(0x00)));  /* 016EE0 LDA */
    t = rd8(DP(0x02));  /* 016EE2 CMP */
    if ((A & 0xFF) == t) goto L_016EF3;  /* 016EE4 BEQ */
    SETLO(A, rd8(DP(0x01)));  /* 016EE6 LDA */
    t = rd8(DP(0x02));  /* 016EE8 CMP */
    if ((A & 0xFF) != t) goto L_016EEF;  /* 016EEA BNE */
    goto L_016EF1;  /* 016EEC JMP */
L_016EEF: TICK();
    SETLO(A, 0x07);  /* 016EEF LDA */
L_016EF1: TICK();
    wr8(DP(0x00), (uint8_t)A);  /* 016EF1 STA */
L_016EF3: TICK();
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_016F05;  /* 016EF3 BBC */
    SETLO(A, rd8(DP(0x00)));  /* 016EF7 LDA */
    SETLO(A, A & 0x7F);  /* 016EF9 AND */
    /* 016EFB CMP */
    if ((A & 0xFF) < 0x08) goto L_016F02;  /* 016EFD BCC */
    wr8(DP(0x00), 0x87);  /* 016EFF LDM */
L_016F02: TICK();
    goto L_016F10;  /* 016F02 JMP */
L_016F05: TICK();
    mflag = 0;  /* 016F05 CLM */
    A = rd16(DP(0x00));  /* 016F06 LDA */
    mflag = 1;  /* 016F08 SEM */
    /* 016F09 CMP */
    if ((A & 0xFF) < 0x08) goto L_016F10;  /* 016F0B BCC */
    wr8(DP(0x00), 0x07);  /* 016F0D LDM */
L_016F10: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 016F10 LDA */
    Y = pull16();  /* 016F12 PLY */
    D = pull16();  /* 016F13 PLD */
    S += 3; return;  /* 016F14 RTL */
}

/* 016F15  callers: 017CC8, 017D26, 017D4D */
void F_016F15(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016F15 LDA */
    push16(D);  /* 016F17 PHD */
    D = A;  /* 016F18 TAD */
    C = 0;  /* 016F19 CLC */
    A = adc16(A, 0x0004);  /* 016F1A ADC */
    push16(A);  /* 016F1D PHA */
    wr16(DP(0x00), Y);  /* 016F1E STY */
    A = Y;  /* 016F20 TYA */
    mflag = 1;  /* 016F21 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 016F22 STA */
    wr8(DP(0x02), (uint8_t)A);  /* 016F24 STA */
    wr8(DP(0x01), (uint8_t)A);  /* 016F26 STA */
    wr8(DP(0x04), 0x03);  /* 016F28 LDM */
    mflag = 0;  /* 016F2B CLM */
    A = D;  /* 016F2C TDA */
    A += 1;  /* 016F2D INC */
    B = 0x0000;  /* 016F2E LDA */
    wr16(DP(0x05), A);  /* 016F32 STA */
    wr16(DP(0x07), B);  /* 016F34 STA */
    wr16(DP(0x08), 0x055A);  /* 016F37 LDM */
    Y = 0x00A0;  /* 016F3B LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 016F3E JSRL */
    Y = pull16();  /* 016F42 PLY */
    D = pull16();  /* 016F43 PLD */
    S += 3; return;  /* 016F44 RTL */
}

/* 016F45  callers: 016BAA, 017CED, 017D0E */
void F_016F45(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016F45 LDA */
    push16(D);  /* 016F47 PHD */
    D = A;  /* 016F48 TAD */
    wr16(DP(0x01), Y);  /* 016F49 STY */
    mflag = 1;  /* 016F4B SEM */
    wr8(DP(0x01), rd8(DP(0x01)) & ~0x80);  /* 016F4C CLB */
    SETLO(A, rd8(DP(0x01)));  /* 016F4F LDA */
    SETLO(A, A ^ 0x07);  /* 016F51 EOR */
    SETLO(A, asl8(A));  /* 016F53 ASL */
    wr8(DP(0x01), (uint8_t)A);  /* 016F54 STA */
    SETLO(A, rd8(DP(0x00)));  /* 016F56 LDA */
    /* 016F58 CMP */
    if ((A & 0xFF) != 0x01) goto L_016F63;  /* 016F5A BNE */
    SETLO(A, rd8(DP(0x01)));  /* 016F5C LDA */
    C = 0;  /* 016F5E CLC */
    SETLO(A, adc8(A, 0x04));  /* 016F5F ADC */
    wr8(DP(0x01), (uint8_t)A);  /* 016F61 STA */
L_016F63: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 016F63 LDA */
    t = rd8(ABS(0x019C));  /* 016F65 CMP */
    if ((A & 0xFF) < t) goto L_016F6F;  /* 016F68 BCC */
    SETLO(A, 0x01);  /* 016F6A LDA */
    goto L_016F75;  /* 016F6C JMP */
L_016F6F: TICK();
    SETLO(A, rd8(ABS(0x019C)));  /* 016F6F LDA */
    C = 1;  /* 016F72 SEC */
    SETLO(A, sbc8(A, rd8(DP(0x01))));  /* 016F73 SBC */
L_016F75: TICK();
    wr8(ABS(0x0200), (uint8_t)A);  /* 016F75 STA */
    mflag = 0;  /* 016F78 CLM */
    D = pull16();  /* 016F79 PLD */
    S += 3; return;  /* 016F7A RTL */
}

/* 016F7B  callers: 0169E7 */
void F_016F7B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016F7B LDA */
    push16(A);  /* 016F7D PHA */
L_016F7E: TICK();
    if ((rd16(ABS(0x0246)) & 0x0040) == 0) goto L_016FDD;  /* 016F7E BBC */
    mflag = 1;  /* 016F84 SEM */
    wr8(ABS(WDT), 0x00);  /* 016F85 LDM */
    mflag = 0;  /* 016F89 CLM */
    S -= 3; idle_check_01A664();  /* 016F8A JSRL */
    X = A;  /* 016F8E TAX */
    if (X == 0) goto L_016F95;  /* 016F8F BEQ */
    ram_trampoline_087A();  /* 016F91 JSRL */
L_016F95: TICK();
    mflag = 1;  /* 016F95 SEM */
    wr8(ABS(WDT), 0x00);  /* 016F96 LDM */
    SETLO(A, rd8(ABS(0x01E9)));  /* 016F9A LDA */
    if ((A & 0xFF) != 0) goto L_016FD9;  /* 016F9D BNE */
    wr8(ABS(0x01E9), 0x01);  /* 016F9F LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_016FB0;  /* 016FA3 BBS */
    mflag = 0;  /* 016FA8 CLM */
    S -= 3; F_017594();  /* 016FA9 JSRL */
    goto L_016FC5;  /* 016FAD JMP */
L_016FB0: TICK();
    mflag = 0;  /* 016FB0 CLM */
    S -= 3; F_017034();  /* 016FB1 JSRL */
    S -= 3; F_017388();  /* 016FB5 JSRL */
    S -= 3; F_017492();  /* 016FB9 JSRL */
    S -= 3; F_017501();  /* 016FBD JSRL */
    S -= 3; F_0176B7();  /* 016FC1 JSRL */
L_016FC5: TICK();
    mflag = 1;  /* 016FC5 SEM */
    if ((rd8(ABS(0x023C)) & 0x01) == 0) goto L_016FD0;  /* 016FC6 BBC */
    mflag = 0;  /* 016FCB CLM */
    S -= 3; F_007A06();  /* 016FCC JSRL */
L_016FD0: TICK();
    mflag = 0;  /* 016FD0 CLM */
    S -= 3; radio_state_machine_00F3E8();  /* 016FD1 JSRL */
    S -= 3; measurements_0198D5();  /* 016FD5 JSRL */
L_016FD9: TICK();
    mflag = 0;  /* 016FD9 CLM */
    goto L_016F7E;  /* 016FDA JMP */
L_016FDD: TICK();
    mflag = 0;  /* 016FDD CLM */
    Y = pull16();  /* 016FDE PLY */
    S += 3; return;  /* 016FDF RTL */
}

/* 016FE0  callers: 0170E4, 017126, 017131, 017163, 017202, 01724F */
void F_016FE0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 016FE0 LDA */
    push16(D);  /* 016FE2 PHD */
    D = A;  /* 016FE3 TAD */
    wr16(DP(0x00), Y);  /* 016FE4 STY */
    mflag = 1;  /* 016FE6 SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_016FFF;  /* 016FE7 BBC */
    SETLO(A, Y);  /* 016FEC TYA */
    mflag = 0;  /* 016FED CLM */
    A &= 0x00FF;  /* 016FEE AND */
    A = asl16(A);  /* 016FF1 ASL */
    A = asl16(A);  /* 016FF2 ASL */
    A = asl16(A);  /* 016FF3 ASL */
    C = 0;  /* 016FF4 CLC */
    A = adc16(A, 0x21B8);  /* 016FF5 ADC */
    mflag = 1;  /* 016FF8 SEM */
    SETLO(B, 0x00);  /* 016FF9 LDA */
    goto L_01700F;  /* 016FFC JMP */
L_016FFF: TICK();
    SETLO(A, Y);  /* 016FFF TYA */
    mflag = 0;  /* 017000 CLM */
    A &= 0x00FF;  /* 017001 AND */
    A = asl16(A);  /* 017004 ASL */
    A = asl16(A);  /* 017005 ASL */
    A = asl16(A);  /* 017006 ASL */
    C = 0;  /* 017007 CLC */
    A = adc16(A, 0x2190);  /* 017008 ADC */
    mflag = 1;  /* 01700B SEM */
    SETLO(B, 0x00);  /* 01700C LDA */
L_01700F: TICK();
    mflag = 0;  /* 01700F CLM */
    wr16(DP(0x01), A);  /* 017010 STA */
    mflag = 1;  /* 017012 SEM */
    wr8(DP(0x03), (uint8_t)B);  /* 017013 STA */
    mflag = 0;  /* 017016 CLM */
    wr16(DP(0x04), 0x0000);  /* 017017 LDM */
L_01701B: TICK();
    X = rd16(DP(0x04));  /* 01701B LDX */
    /* 01701D CPX */
    if (X >= 0x0008) goto L_017031;  /* 017020 BCS */
    A = rd16(LIND(0x01));  /* 017022 LDA */
    mflag = 1;  /* 017024 SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 017025 STA */
    mflag = 0;  /* 017028 CLM */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 017029 INC */
    X += 1;  /* 01702B INX */
    wr16(DP(0x04), X);  /* 01702C STX */
    goto L_01701B;  /* 01702E JMP */
L_017031: TICK();
    mflag = 0;  /* 017031 CLM */
    D = pull16();  /* 017032 PLD */
    S += 3; return;  /* 017033 RTL */
}

/* 017034  callers: 016FB1 */
void F_017034(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017034 LDA */
    push16(A);  /* 017036 PHA */
    mflag = 1;  /* 017037 SEM */
    if ((rd8(ABS(0x023A)) & 0x02) == 0x02) goto L_017040;  /* 017038 BBS */
    goto L_0170C2;  /* 01703D JMP */
L_017040: TICK();
    mflag = 0;  /* 017040 CLM */
    S -= 3; F_0050DA();  /* 017041 JSRL */
    A = rd16(ABS(0x0226));  /* 017045 LDA */
    mflag = 1;  /* 017048 SEM */
    /* 017049 CMP */
    if ((A & 0xFF) == 0x0F) goto L_0170C2;  /* 01704B BEQ */
    SETLO(A, rd8(ABS(0x01A1)));  /* 01704D LDA */
    /* 017050 CMP */
    if ((A & 0xFF) == 0x02) goto L_01705F;  /* 017052 BEQ */
    wr8(ABS(0x0226), 0x0F);  /* 017054 LDM */
    wr8(ABS(0x0293), 0x00);  /* 017058 LDM */
    goto L_0170C2;  /* 01705C JMP */
L_01705F: TICK();
    mflag = 0;  /* 01705F CLM */
    A = rd16(ABS(0x0226));  /* 017060 LDA */
    mflag = 1;  /* 017063 SEM */
    /* 017064 CMP */
    if ((A & 0xFF) != 0x70) goto L_017070;  /* 017066 BNE */
    mflag = 0;  /* 017068 CLM */
    S -= 3; F_0170EE();  /* 017069 JSRL */
    goto L_0170A9;  /* 01706D JMP */
L_017070: TICK();
    mflag = 0;  /* 017070 CLM */
    A = rd16(ABS(0x0226));  /* 017071 LDA */
    mflag = 1;  /* 017074 SEM */
    /* 017075 CMP */
    if ((A & 0xFF) != 0x71) goto L_017086;  /* 017077 BNE */
    mflag = 0;  /* 017079 CLM */
    S -= 3; F_0170F4();  /* 01707A JSRL */
    mflag = 1;  /* 01707E SEM */
    wr8(ABS(0x0231), 0x0D);  /* 01707F LDM */
    goto L_0170A9;  /* 017083 JMP */
L_017086: TICK();
    mflag = 0;  /* 017086 CLM */
    A = rd16(ABS(0x0226));  /* 017087 LDA */
    mflag = 1;  /* 01708A SEM */
    /* 01708B CMP */
    if ((A & 0xFF) != 0x73) goto L_01709C;  /* 01708D BNE */
    mflag = 0;  /* 01708F CLM */
    S -= 3; F_017137();  /* 017090 JSRL */
    mflag = 1;  /* 017094 SEM */
    wr8(ABS(0x0231), 0x0E);  /* 017095 LDM */
    goto L_0170A9;  /* 017099 JMP */
L_01709C: TICK();
    Y = rd16(ABS(0x0226));  /* 01709C LDY */
    mflag = 0;  /* 01709F CLM */
    S -= 3; F_017175();  /* 0170A0 JSRL */
    mflag = 1;  /* 0170A4 SEM */
    wr8(ABS(0x0231), 0x14);  /* 0170A5 LDM */
L_0170A9: TICK();
    mflag = 1;  /* 0170A9 SEM */
    wr8(ABS(0x0230), 0xFB);  /* 0170AA LDM */
    mflag = 0;  /* 0170AE CLM */
    X = rd16(ABS(0x0214));  /* 0170AF LDX */
    if (X != 0) goto L_0170B9;  /* 0170B2 BNE */
    Y = X;  /* 0170B4 TXY */
    S -= 3; F_0080AA();  /* 0170B5 JSRL */
L_0170B9: TICK();
    S -= 3; F_00828C();  /* 0170B9 JSRL */
    mflag = 1;  /* 0170BD SEM */
    wr8(ABS(0x0226), 0x0F);  /* 0170BE LDM */
L_0170C2: TICK();
    SETLO(A, rd8(ABS(0x0231)));  /* 0170C2 LDA */
    if ((A & 0xFF) == 0) goto L_0170D1;  /* 0170C5 BEQ */
    SETLO(A, rd8(ABS(0x0230)));  /* 0170C7 LDA */
    if ((A & 0xFF) != 0) goto L_0170D1;  /* 0170CA BNE */
    mflag = 0;  /* 0170CC CLM */
    S -= 3; F_0170D4();  /* 0170CD JSRL */
L_0170D1: TICK();
    mflag = 0;  /* 0170D1 CLM */
    Y = pull16();  /* 0170D2 PLY */
    S += 3; return;  /* 0170D3 RTL */
}

/* 0170D4  callers: 016BCB, 0170CD */
void F_0170D4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0170D4 LDA */
    push16(A);  /* 0170D6 PHA */
    mflag = 1;  /* 0170D7 SEM */
    wr8(ABS(0x0293), 0x00);  /* 0170D8 LDM */
    wr8(ABS(0x0231), 0x00);  /* 0170DC LDM */
    Y = 0x0000;  /* 0170E0 LDY */
    mflag = 0;  /* 0170E3 CLM */
    S -= 3; F_016FE0();  /* 0170E4 JSRL */
    S -= 3; F_00828C();  /* 0170E8 JSRL */
    Y = pull16();  /* 0170EC PLY */
    S += 3; return;  /* 0170ED RTL */
}

/* 0170EE  callers: 017069 */
void F_0170EE(void)
{
    uint32_t t; (void)t;
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0040);  /* 0170EE CLB */
    S += 3; return;  /* 0170F3 RTL */
}

/* 0170F4  callers: 01707A */
void F_0170F4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0170F4 LDA */
    push16(A);  /* 0170F6 PHA */
    mflag = 1;  /* 0170F7 SEM */
    wr8(ABS(0x0293), 0x00);  /* 0170F8 LDM */
    SETLO(A, rd8(ABS(0x0231)));  /* 0170FC LDA */
    /* 0170FF CMP */
    if ((A & 0xFF) != 0x0D) goto L_01711C;  /* 017101 BNE */
    mflag = 0;  /* 017103 CLM */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0) goto L_017112;  /* 017104 BBC */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0020);  /* 01710A CLB */
    goto L_017118;  /* 01710F JMP */
L_017112: TICK();
    mflag = 0;  /* 017112 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0020);  /* 017113 SEB */
L_017118: TICK();
    S -= 3; F_008DED();  /* 017118 JSRL */
L_01711C: TICK();
    mflag = 0;  /* 01711C CLM */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0) goto L_01712D;  /* 01711D BBC */
    Y = 0x0001;  /* 017123 LDY */
    S -= 3; F_016FE0();  /* 017126 JSRL */
    goto L_017135;  /* 01712A JMP */
L_01712D: TICK();
    Y = 0x0002;  /* 01712D LDY */
    mflag = 0;  /* 017130 CLM */
    S -= 3; F_016FE0();  /* 017131 JSRL */
L_017135: TICK();
    Y = pull16();  /* 017135 PLY */
    S += 3; return;  /* 017136 RTL */
}

/* 017137  callers: 017090 */
void F_017137(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017137 LDA */
    push16(A);  /* 017139 PHA */
    mflag = 1;  /* 01713A SEM */
    wr8(ABS(0x0293), 0x00);  /* 01713B LDM */
    SETLO(A, rd8(ABS(0x0231)));  /* 01713F LDA */
    /* 017142 CMP */
    if ((A & 0xFF) != 0x0E) goto L_01715F;  /* 017144 BNE */
    mflag = 0;  /* 017146 CLM */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_017155;  /* 017147 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0800);  /* 01714D CLB */
    goto L_01715B;  /* 017152 JMP */
L_017155: TICK();
    mflag = 0;  /* 017155 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0800);  /* 017156 SEB */
L_01715B: TICK();
    S -= 3; F_008DED();  /* 01715B JSRL */
L_01715F: TICK();
    Y = 0x0004;  /* 01715F LDY */
    mflag = 0;  /* 017162 CLM */
    S -= 3; F_016FE0();  /* 017163 JSRL */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0x0800) goto L_017172;  /* 017167 BBS */
    mflag = 1;  /* 01716D SEM */
    wr8(ABS(0x0258), 0x2A);  /* 01716E LDM */
L_017172: TICK();
    mflag = 0;  /* 017172 CLM */
    Y = pull16();  /* 017173 PLY */
    S += 3; return;  /* 017174 RTL */
}

/* 017175  callers: 0170A0 */
void F_017175(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017175 LDA */
    push16(D);  /* 017177 PHD */
    D = A;  /* 017178 TAD */
    C = 0;  /* 017179 CLC */
    A = adc16(A, 0x000C);  /* 01717A ADC */
    push16(A);  /* 01717D PHA */
    wr16(DP(0x00), Y);  /* 01717E STY */
    wr16(DP(0x05), 0x0000);  /* 017180 LDM */
    wr16(DP(0x07), 0x000A);  /* 017184 LDM */
    wr16(DP(0x09), 0x0000);  /* 017188 LDM */
    wr16(DP(0x01), 0x0000);  /* 01718C LDM */
L_017190: TICK();
    A = rd16(ABS(0x0293));  /* 017190 LDA */
    A &= 0x00FF;  /* 017193 AND */
    t = rd16(DP(0x01));  /* 017196 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_0171E9;  /* 017198 BCC */
    if (Z) goto L_0171E9;  /* 01719A BEQ */
    X = rd16(DP(0x01));  /* 01719C LDX */
    A = rd16((ABS(0x0283) + X));  /* 01719E LDA */
    mflag = 1;  /* 0171A1 SEM */
    /* 0171A2 CMP */
    if ((A & 0xFF) != 0x2A) goto L_0171C6;  /* 0171A4 BNE */
    mflag = 0;  /* 0171A6 CLM */
    wr16(DP(0x09), 0x0001);  /* 0171A7 LDM */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 0171AB INC */
    A = rd16(ABS(0x0293));  /* 0171AD LDA */
    A &= 0x00FF;  /* 0171B0 AND */
    t = rd16(DP(0x01));  /* 0171B3 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_0171C6;  /* 0171B5 BCC */
    if (Z) goto L_0171C6;  /* 0171B7 BEQ */
    X = rd16(DP(0x01));  /* 0171B9 LDX */
    A = rd16((ABS(0x0283) + X));  /* 0171BB LDA */
    A &= 0x00FF;  /* 0171BE AND */
    A = adc16(A, 0xFFCF);  /* 0171C1 ADC */
    wr16(DP(0x07), A);  /* 0171C4 STA */
L_0171C6: TICK();
    mflag = 0;  /* 0171C6 CLM */
    X = rd16(DP(0x09));  /* 0171C7 LDX */
    if (X != 0) goto L_0171E4;  /* 0171C9 BNE */
    X = rd16(DP(0x01));  /* 0171CB LDX */
    A = rd16((ABS(0x0283) + X));  /* 0171CD LDA */
    A &= 0x00FF;  /* 0171D0 AND */
    wr16(DP(0x0C), A);  /* 0171D3 STA */
    A = rd16(DP(0x05));  /* 0171D5 LDA */
    mpy16(0x000A);  /* 0171D7 MPY */
    C = 0;  /* 0171DB CLC */
    A = adc16(A, rd16(DP(0x0C)));  /* 0171DC ADC */
    C = 0;  /* 0171DE CLC */
    A = adc16(A, 0xFFD0);  /* 0171DF ADC */
    wr16(DP(0x05), A);  /* 0171E2 STA */
L_0171E4: TICK();
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 0171E4 INC */
    goto L_017190;  /* 0171E6 JMP */
L_0171E9: TICK();
    mflag = 0;  /* 0171E9 CLM */
    wr16(DP(0x03), 0x0000);  /* 0171EA LDM */
    A = rd16(ABS(0x0293));  /* 0171EE LDA */
    mflag = 1;  /* 0171F1 SEM */
    wr8(DP(0x0B), (uint8_t)A);  /* 0171F2 STA */
    SETLO(A, rd8(DP(0x00)));  /* 0171F4 LDA */
    /* 0171F6 CMP */
    if ((A & 0xFF) != 0x6D) goto L_017209;  /* 0171F8 BNE */
    wr8(ABS(0x0293), 0x00);  /* 0171FA LDM */
    Y = 0x0000;  /* 0171FE LDY */
    mflag = 0;  /* 017201 CLM */
    S -= 3; F_016FE0();  /* 017202 JSRL */
    goto L_017379;  /* 017206 JMP */
L_017209: TICK();
    mflag = 0;  /* 017209 CLM */
    A = rd16(DP(0x00));  /* 01720A LDA */
    mflag = 1;  /* 01720C SEM */
    /* 01720D CMP */
    if ((A & 0xFF) != 0x2A) goto L_01723F;  /* 01720F BNE */
    mflag = 0;  /* 017211 CLM */
    X = rd16(DP(0x09));  /* 017212 LDX */
    if (X != 0) goto L_017237;  /* 017214 BNE */
    X = rd16(DP(0x05));  /* 017216 LDX */
    if (X == 0) goto L_017237;  /* 017218 BEQ */
    A = rd16(DP(0x0B));  /* 01721A LDA */
    A &= 0x00FF;  /* 01721C AND */
    X = A;  /* 01721F TAX */
    mflag = 1;  /* 017220 SEM */
    wr8((ABS(0x0283) + X), 0x2A);  /* 017221 LDM */
    SETLO(A, rd8(DP(0x0B)));  /* 017225 LDA */
    mflag = 0;  /* 017227 CLM */
    A &= 0x00FF;  /* 017228 AND */
    X = A;  /* 01722B TAX */
    mflag = 1;  /* 01722C SEM */
    wr8((ABS(0x0252) + X), 0x2A);  /* 01722D LDM */
    t = (rd8(ABS(0x0293)) + 1) & 0xFF; wr8(ABS(0x0293), t);  /* 017231 INC */
    goto L_01723C;  /* 017234 JMP */
L_017237: TICK();
    mflag = 0;  /* 017237 CLM */
    wr16(DP(0x03), 0x0001);  /* 017238 LDM */
L_01723C: TICK();
    goto L_017379;  /* 01723C JMP */
L_01723F: TICK();
    mflag = 0;  /* 01723F CLM */
    A = rd16(DP(0x00));  /* 017240 LDA */
    mflag = 1;  /* 017242 SEM */
    /* 017243 CMP */
    if ((A & 0xFF) != 0x23) goto L_017286;  /* 017245 BNE */
    SETLO(A, rd8(DP(0x0B)));  /* 017247 LDA */
    if ((A & 0xFF) != 0) goto L_017256;  /* 017249 BNE */
    Y = 0x0003;  /* 01724B LDY */
    mflag = 0;  /* 01724E CLM */
    S -= 3; F_016FE0();  /* 01724F JSRL */
    goto L_017279;  /* 017253 JMP */
L_017256: TICK();
    mflag = 0;  /* 017256 CLM */
    X = rd16(DP(0x09));  /* 017257 LDX */
    if (X == 0) goto L_017262;  /* 017259 BEQ */
    X = rd16(DP(0x07));  /* 01725B LDX */
    /* 01725D CPX */
    if (X == 0x000A) goto L_017266;  /* 017260 BEQ */
L_017262: TICK();
    X = rd16(DP(0x05));  /* 017262 LDX */
    if (X != 0) goto L_01726D;  /* 017264 BNE */
L_017266: TICK();
    wr16(DP(0x03), 0x0001);  /* 017266 LDM */
    goto L_017279;  /* 01726A JMP */
L_01726D: TICK();
    mflag = 0;  /* 01726D CLM */
    A = rd16(DP(0x0B));  /* 01726E LDA */
    A &= 0x00FF;  /* 017270 AND */
    X = A;  /* 017273 TAX */
    mflag = 1;  /* 017274 SEM */
    wr8((ABS(0x0252) + X), 0x5E);  /* 017275 LDM */
L_017279: TICK();
    mflag = 0;  /* 017279 CLM */
    X = rd16(DP(0x03));  /* 01727A LDX */
    if (X != 0) goto L_017283;  /* 01727C BNE */
    mflag = 1;  /* 01727E SEM */
    wr8(ABS(0x01A0), 0x03);  /* 01727F LDM */
L_017283: TICK();
    goto L_017379;  /* 017283 JMP */
L_017286: TICK();
    mflag = 0;  /* 017286 CLM */
    A = rd16(ABS(0x0293));  /* 017287 LDA */
    mflag = 1;  /* 01728A SEM */
    SETLO(A, A - 1);  /* 01728B DEC */
    Z = ((A & 0xFF) == 0);
    wr8(DP(0x0B), (uint8_t)A);  /* 01728C STA */
    if (!Z) goto L_0172BF;  /* 01728E BNE */
    mflag = 0;  /* 017290 CLM */
    wr16(DP(0x01), 0x0001);  /* 017291 LDM */
L_017295: TICK();
    X = rd16(DP(0x01));  /* 017295 LDX */
    /* 017297 CPX */
    if (X >= 0x0008) goto L_0172A8;  /* 01729A BCS */
    mflag = 1;  /* 01729C SEM */
    wr8((ABS(0x0252) + X), 0x20);  /* 01729D LDM */
    X += 1;  /* 0172A1 INX */
    wr16(DP(0x01), X);  /* 0172A2 STX */
    mflag = 0;  /* 0172A4 CLM */
    goto L_017295;  /* 0172A5 JMP */
L_0172A8: TICK();
    mflag = 0;  /* 0172A8 CLM */
    A = rd16(DP(0x0B));  /* 0172A9 LDA */
    A &= 0x00FF;  /* 0172AB AND */
    X = A;  /* 0172AE TAX */
    A = rd16(DP(0x0B));  /* 0172AF LDA */
    A &= 0x00FF;  /* 0172B1 AND */
    Y = A;  /* 0172B4 TAY */
    A = rd16((ABS(0x0283) + X));  /* 0172B5 LDA */
    mflag = 1;  /* 0172B8 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 0172B9 STA */
    goto L_017379;  /* 0172BC JMP */
L_0172BF: TICK();
    mflag = 1;  /* 0172BF SEM */
    /* 0172C0 CMP */
    if ((A & 0xFF) < 0x01) goto L_0172DF;  /* 0172C2 BCC */
    /* 0172C4 CMP */
    if ((A & 0xFF) >= 0x03) goto L_0172DF;  /* 0172C6 BCS */
    SETLO(A, rd8(DP(0x0B)));  /* 0172C8 LDA */
    mflag = 0;  /* 0172CA CLM */
    A &= 0x00FF;  /* 0172CB AND */
    X = A;  /* 0172CE TAX */
    A = rd16(DP(0x0B));  /* 0172CF LDA */
    A &= 0x00FF;  /* 0172D1 AND */
    Y = A;  /* 0172D4 TAY */
    A = rd16((ABS(0x0283) + X));  /* 0172D5 LDA */
    mflag = 1;  /* 0172D8 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 0172D9 STA */
    goto L_017379;  /* 0172DC JMP */
L_0172DF: TICK();
    mflag = 1;  /* 0172DF SEM */
    /* 0172E0 CMP */
    if ((A & 0xFF) >= 0x03) goto L_0172E7;  /* 0172E2 BCS */
    goto L_017370;  /* 0172E4 JMP */
L_0172E7: TICK();
    /* 0172E7 CMP */
    if ((A & 0xFF) < 0x06) goto L_0172EE;  /* 0172E9 BCC */
    goto L_017370;  /* 0172EB JMP */
L_0172EE: TICK();
    mflag = 0;  /* 0172EE CLM */
    X = rd16(DP(0x09));  /* 0172EF LDX */
    if (X != 0) goto L_017327;  /* 0172F1 BNE */
    A = rd16(DP(0x0B));  /* 0172F3 LDA */
    mflag = 1;  /* 0172F5 SEM */
    /* 0172F6 CMP */
    if ((A & 0xFF) != 0x03) goto L_017304;  /* 0172F8 BNE */
    mflag = 0;  /* 0172FA CLM */
    X = rd16(DP(0x05));  /* 0172FB LDX */
    if (X == 0) goto L_017304;  /* 0172FD BEQ */
    /* 0172FF CPX */
    if (X < 0x0800) goto L_017310;  /* 017302 BCC */
L_017304: TICK();
    mflag = 1;  /* 017304 SEM */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 017305 DEC */
    mflag = 0;  /* 017308 CLM */
    wr16(DP(0x03), 0x0001);  /* 017309 LDM */
    goto L_017324;  /* 01730D JMP */
L_017310: TICK();
    mflag = 0;  /* 017310 CLM */
    A = rd16(DP(0x0B));  /* 017311 LDA */
    A &= 0x00FF;  /* 017313 AND */
    X = A;  /* 017316 TAX */
    A = rd16(DP(0x0B));  /* 017317 LDA */
    A &= 0x00FF;  /* 017319 AND */
    Y = A;  /* 01731C TAY */
    A = rd16((ABS(0x0283) + X));  /* 01731D LDA */
    mflag = 1;  /* 017320 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 017321 STA */
L_017324: TICK();
    goto L_01736D;  /* 017324 JMP */
L_017327: TICK();
    mflag = 0;  /* 017327 CLM */
    A = rd16(DP(0x0B));  /* 017328 LDA */
    A &= 0x00FF;  /* 01732A AND */
    A -= 1;  /* 01732D DEC */
    A -= 1;  /* 01732E DEC */
    X = A;  /* 01732F TAX */
    A = rd16((ABS(0x0283) + X));  /* 017330 LDA */
    mflag = 1;  /* 017333 SEM */
    /* 017334 CMP */
    if ((A & 0xFF) == 0x2A) goto L_01734E;  /* 017336 BEQ */
    mflag = 0;  /* 017338 CLM */
    X = rd16(DP(0x05));  /* 017339 LDX */
    /* 01733B CPX */
    if (X != 0x0063) goto L_017359;  /* 01733E BNE */
    A = rd16(DP(0x0B));  /* 017340 LDA */
    A &= 0x00FF;  /* 017342 AND */
    X = A;  /* 017345 TAX */
    A = rd16((ABS(0x0283) + X));  /* 017346 LDA */
    mflag = 1;  /* 017349 SEM */
    /* 01734A CMP */
    if ((A & 0xFF) != 0x30) goto L_017359;  /* 01734C BNE */
L_01734E: TICK();
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 01734E DEC */
    mflag = 0;  /* 017351 CLM */
    wr16(DP(0x03), 0x0001);  /* 017352 LDM */
    goto L_01736D;  /* 017356 JMP */
L_017359: TICK();
    mflag = 0;  /* 017359 CLM */
    A = rd16(DP(0x0B));  /* 01735A LDA */
    A &= 0x00FF;  /* 01735C AND */
    X = A;  /* 01735F TAX */
    A = rd16(DP(0x0B));  /* 017360 LDA */
    A &= 0x00FF;  /* 017362 AND */
    Y = A;  /* 017365 TAY */
    A = rd16((ABS(0x0283) + X));  /* 017366 LDA */
    mflag = 1;  /* 017369 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 01736A STA */
L_01736D: TICK();
    goto L_017379;  /* 01736D JMP */
L_017370: TICK();
    mflag = 1;  /* 017370 SEM */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 017371 DEC */
    mflag = 0;  /* 017374 CLM */
    wr16(DP(0x03), 0x0001);  /* 017375 LDM */
L_017379: TICK();
    mflag = 0;  /* 017379 CLM */
    X = rd16(DP(0x03));  /* 01737A LDX */
    if (X == 0) goto L_017385;  /* 01737C BEQ */
    Y = 0x000D;  /* 01737E LDY */
    S -= 3; F_0080AA();  /* 017381 JSRL */
L_017385: TICK();
    Y = pull16();  /* 017385 PLY */
    D = pull16();  /* 017386 PLD */
    S += 3; return;  /* 017387 RTL */
}

/* 017388  callers: 016FB5 */
void F_017388(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017388 LDA */
    push16(D);  /* 01738A PHD */
    D = A;  /* 01738B TAD */
    A += 1;  /* 01738C INC */
    A += 1;  /* 01738D INC */
    A += 1;  /* 01738E INC */
    push16(A);  /* 01738F PHA */
    mflag = 1;  /* 017390 SEM */
    if ((rd8(ABS(0x023A)) & 0x04) == 0) goto L_01740E;  /* 017391 BBC */
    mflag = 0;  /* 017396 CLM */
    A = D;  /* 017397 TDA */
    A += 1;  /* 017398 INC */
    B = 0x0000;  /* 017399 LDA */
    Y = A;  /* 01739D TAY */
    S -= 3; F_00589D();  /* 01739E JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 0173A2 STA */
    if ((rd8(ABS(0x0240)) & 0x20) == 0) goto L_0173AE;  /* 0173A4 BBC */
    SETLO(A, 0x01);  /* 0173A9 LDA */
    goto L_0173B0;  /* 0173AB JMP */
L_0173AE: TICK();
    SETLO(A, 0x00);  /* 0173AE LDA */
L_0173B0: TICK();
    wr8(DP(0x02), (uint8_t)A);  /* 0173B0 STA */
    mflag = 0;  /* 0173B2 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0004);  /* 0173B3 CLB */
    mflag = 1;  /* 0173B8 SEM */
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_0173C6;  /* 0173B9 BBC */
    mflag = 0;  /* 0173BD CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0020);  /* 0173BE CLB */
    goto L_0173CC;  /* 0173C3 JMP */
L_0173C6: TICK();
    mflag = 0;  /* 0173C6 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0020);  /* 0173C7 SEB */
L_0173CC: TICK();
    mflag = 1;  /* 0173CC SEM */
    if ((rd8(DP(0x00)) & 0x40) == 0) goto L_0173DE;  /* 0173CD BBC */
    SETLO(A, rd8(DP(0x01)));  /* 0173D1 LDA */
    if ((A & 0xFF) != 0) goto L_0173DE;  /* 0173D3 BNE */
    mflag = 0;  /* 0173D5 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0010);  /* 0173D6 SEB */
    goto L_0173E4;  /* 0173DB JMP */
L_0173DE: TICK();
    mflag = 0;  /* 0173DE CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0010);  /* 0173DF CLB */
L_0173E4: TICK();
    A = rd16(DP(0x02));  /* 0173E4 LDA */
    A &= 0x00FF;  /* 0173E6 AND */
    wr16(DP(0x03), A);  /* 0173E9 STA */
    A = rd16(ABS(0x0240));  /* 0173EB LDA */
    A &= 0x0020;  /* 0173EE AND */
    if (A == 0) goto L_0173F6;  /* 0173F1 BEQ */
    A = 0x0001;  /* 0173F3 LDA */
L_0173F6: TICK();
    t = rd16(DP(0x03));  /* 0173F6 CMP */
    if (A == t) goto L_0173FE;  /* 0173F8 BEQ */
    S -= 3; F_017412();  /* 0173FA JSRL */
L_0173FE: TICK();
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_01740E;  /* 0173FE BBC */
    mflag = 1;  /* 017404 SEM */
    if ((rd8(DP(0x00)) & 0x20) == 0x20) goto L_01740E;  /* 017405 BBS */
    mflag = 0;  /* 017409 CLM */
    S -= 3; F_00973E();  /* 01740A JSRL */
L_01740E: TICK();
    mflag = 0;  /* 01740E CLM */
    Y = pull16();  /* 01740F PLY */
    D = pull16();  /* 017410 PLD */
    S += 3; return;  /* 017411 RTL */
}

/* 017412  callers: 0173FA */
void F_017412(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017412 LDA */
    push16(D);  /* 017414 PHD */
    D = A;  /* 017415 TAD */
    C = 0;  /* 017416 CLC */
    A = adc16(A, 0x0004);  /* 017417 ADC */
    push16(A);  /* 01741A PHA */
    mflag = 1;  /* 01741B SEM */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_017428;  /* 01741C BBC */
    mflag = 0;  /* 017421 CLM */
    A = 0x0001;  /* 017422 LDA */
    goto L_01742C;  /* 017425 JMP */
L_017428: TICK();
    mflag = 0;  /* 017428 CLM */
    A = 0x0000;  /* 017429 LDA */
L_01742C: TICK();
    wr16(DP(0x02), A);  /* 01742C STA */
    A = rd16(ABS(0x0229));  /* 01742E LDA */
    mflag = 1;  /* 017431 SEM */
    /* 017432 CMP */
    if ((A & 0xFF) == 0x04) goto L_01745B;  /* 017434 BEQ */
    SETLO(A, rd8(ABS(0x0229)));  /* 017436 LDA */
    /* 017439 CMP */
    if ((A & 0xFF) == 0x03) goto L_01745B;  /* 01743B BEQ */
    mflag = 0;  /* 01743D CLM */
    if ((rd16(ABS(0x0240)) & 0x0020) == 0x0020) goto L_01744C;  /* 01743E BBS */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 017444 CLB */
    goto L_01745B;  /* 017449 JMP */
L_01744C: TICK();
    mflag = 0;  /* 01744C CLM */
    A = rd16(ABS(0x0375));  /* 01744D LDA */
    mflag = 1;  /* 017450 SEM */
    SETLO(A, A & 0x3F);  /* 017451 AND */
    if ((A & 0xFF) == 0) goto L_01745B;  /* 017453 BEQ */
    mflag = 0;  /* 017455 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 017456 SEB */
L_01745B: TICK();
    mflag = 1;  /* 01745B SEM */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_017468;  /* 01745C BBC */
    mflag = 0;  /* 017461 CLM */
    A = 0x0001;  /* 017462 LDA */
    goto L_01746C;  /* 017465 JMP */
L_017468: TICK();
    mflag = 0;  /* 017468 CLM */
    A = 0x0000;  /* 017469 LDA */
L_01746C: TICK();
    wr16(DP(0x00), A);  /* 01746C STA */
    t = rd16(DP(0x02));  /* 01746E CMP */
    if (A == t) goto L_01748F;  /* 017470 BEQ */
    X = A;  /* 017472 TAX */
    if (X == 0) goto L_01747D;  /* 017473 BEQ */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0001);  /* 017475 SEB */
    goto L_017483;  /* 01747A JMP */
L_01747D: TICK();
    mflag = 0;  /* 01747D CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0001);  /* 01747E CLB */
L_017483: TICK();
    S -= 3; F_007A06();  /* 017483 JSRL */
    S -= 3; F_00828C();  /* 017487 JSRL */
    S -= 3; F_00CB52();  /* 01748B JSRL */
L_01748F: TICK();
    Y = pull16();  /* 01748F PLY */
    D = pull16();  /* 017490 PLD */
    S += 3; return;  /* 017491 RTL */
}

/* 017492  callers: 016FB9 */
void F_017492(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0240)) & 0x0200) == 0x0200) goto L_0174DB;  /* 017492 BBS */
    mflag = 1;  /* 017498 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0x10) goto L_0174A7;  /* 017499 BBS [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 01749E CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0001);  /* 01749F CLB */
    goto L_0174B8;  /* 0174A4 JMP */
L_0174A7: TICK();
    mflag = 0;  /* 0174A7 CLM */
    if ((rd16(ABS(0x0242)) & 0x0001) == 0x0001) goto L_0174B8;  /* 0174A8 BBS */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 0174AE SEB */
    mflag = 1;  /* 0174B3 SEM */
    wr8(ABS(0x0233), 0x02);  /* 0174B4 LDM */
L_0174B8: TICK();
    mflag = 1;  /* 0174B8 SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0x80) goto L_0174C7;  /* 0174B9 BBS [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 0174BE CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0020);  /* 0174BF SEB */
    goto L_0174D8;  /* 0174C4 JMP */
L_0174C7: TICK();
    mflag = 0;  /* 0174C7 CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0) goto L_0174D8;  /* 0174C8 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 0174CE SEB */
    mflag = 1;  /* 0174D3 SEM */
    wr8(ABS(0x0233), 0x02);  /* 0174D4 LDM */
L_0174D8: TICK();
    goto L_0174FF;  /* 0174D8 JMP */
L_0174DB: TICK();
    mflag = 1;  /* 0174DB SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 0174DC LDA */
    if ((A & 0xFF) != 0) goto L_0174FF;  /* 0174DF BNE */
    mflag = 0;  /* 0174E1 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 0174E2 CLB */
    mflag = 1;  /* 0174E7 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0) goto L_0174F3;  /* 0174E8 BBC [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 0174ED CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0001);  /* 0174EE SEB */
L_0174F3: TICK();
    mflag = 1;  /* 0174F3 SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0) goto L_0174FF;  /* 0174F4 BBC [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 0174F9 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0020);  /* 0174FA CLB */
L_0174FF: TICK();
    mflag = 0;  /* 0174FF CLM */
    S += 3; return;  /* 017500 RTL */
}

/* 017501  callers: 016FBD */
void F_017501(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017501 LDA */
    push16(A);  /* 017503 PHA */
    A = rd16(ABS(0x01A0));  /* 017504 LDA */
    mflag = 1;  /* 017507 SEM */
    /* 017508 CMP */
    if ((A & 0xFF) != 0x03) goto L_01750F;  /* 01750A BNE */
    goto L_017591;  /* 01750C JMP */
L_01750F: TICK();
    SETLO(A, rd8(ABS(0x01A1)));  /* 01750F LDA */
    /* 017512 CMP */
    if ((A & 0xFF) == 0x04) goto L_017591;  /* 017514 BEQ */
    /* 017516 CMP */
    if ((A & 0xFF) == 0x07) goto L_017591;  /* 017518 BEQ */
    SETLO(A, rd8(ABS(0x0244)));  /* 01751A LDA */
    SETLO(A, A & 0x38);  /* 01751D AND */
    if ((A & 0xFF) == 0) goto L_017579;  /* 01751F BEQ */
    mflag = 0;  /* 017521 CLM */
    X = rd16(ABS(0x0214));  /* 017522 LDX */
    if (X != 0) goto L_017576;  /* 017525 BNE */
    if ((rd16(ABS(0x0244)) & 0x0008) == 0x0008) goto L_017571;  /* 017527 BBS */
    if ((rd16(ABS(0x0242)) & 0x0001) == 0) goto L_01753D;  /* 01752D BBC */
    Y = 0x0006;  /* 017533 LDY */
    S -= 3; F_0080AA();  /* 017536 JSRL */
    goto L_01756E;  /* 01753A JMP */
L_01753D: TICK();
    mflag = 0;  /* 01753D CLM */
    if ((rd16(ABS(0x0244)) & 0x0040) == 0) goto L_01754E;  /* 01753E BBC */
    Y = 0x0010;  /* 017544 LDY */
    S -= 3; F_0080AA();  /* 017547 JSRL */
    goto L_01756E;  /* 01754B JMP */
L_01754E: TICK();
    mflag = 1;  /* 01754E SEM */
    if ((rd8(ABS(0x0340)) & 0x10) == 0) goto L_017569;  /* 01754F BBC */
    if ((rd8(ABS(0x0240)) & 0x01) == 0) goto L_017569;  /* 017554 BBC */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_017569;  /* 017559 BBC */
    Y = 0x0003;  /* 01755E LDY */
    mflag = 0;  /* 017561 CLM */
    S -= 3; F_0080AA();  /* 017562 JSRL */
    goto L_01756E;  /* 017566 JMP */
L_017569: TICK();
    mflag = 1;  /* 017569 SEM */
    wr8(ABS(0x01A0), 0x02);  /* 01756A LDM */
L_01756E: TICK();
    goto L_017576;  /* 01756E JMP */
L_017571: TICK();
    mflag = 1;  /* 017571 SEM */
    wr8(ABS(0x01A0), 0x02);  /* 017572 LDM */
L_017576: TICK();
    goto L_017591;  /* 017576 JMP */
L_017579: TICK();
    mflag = 0;  /* 017579 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0080);  /* 01757A CLB */
    A = rd16(ABS(0x01A0));  /* 01757F LDA */
    mflag = 1;  /* 017582 SEM */
    /* 017583 CMP */
    if ((A & 0xFF) != 0x02) goto L_01758C;  /* 017585 BNE */
    SETLO(A, 0x01);  /* 017587 LDA */
    goto L_01758E;  /* 017589 JMP */
L_01758C: TICK();
    SETLO(A, 0x00);  /* 01758C LDA */
L_01758E: TICK();
    wr8(ABS(0x01A0), (uint8_t)A);  /* 01758E STA */
L_017591: TICK();
    mflag = 0;  /* 017591 CLM */
    Y = pull16();  /* 017592 PLY */
    S += 3; return;  /* 017593 RTL */
}

/* 017594  callers: 016FA9 */
void F_017594(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017594 LDA */
    push16(D);  /* 017596 PHD */
    D = A;  /* 017597 TAD */
    push16(A);  /* 017598 PHA */
    mflag = 1;  /* 017599 SEM */
    SETLO(A, rd8(ABS(0x01A1)));  /* 01759A LDA */
    if ((A & 0xFF) != 0) goto L_0175D2;  /* 01759D BNE */
    SETLO(A, rd8(ABS(0x019D)));  /* 01759F LDA */
    if ((A & 0xFF) == 0) goto L_0175CF;  /* 0175A2 BEQ */
    mflag = 0;  /* 0175A4 CLM */
    S -= 3; F_017BBD();  /* 0175A5 JSRL */
    A = rd16(ABS(0x01AB));  /* 0175A9 LDA */
    mflag = 1;  /* 0175AC SEM */
    /* 0175AD CMP */
    if ((A & 0xFF) != 0x01) goto L_0175CF;  /* 0175AF BNE */
    wr8(ABS(0x01A1), 0x01);  /* 0175B1 LDM */
    mflag = 0;  /* 0175B5 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 0175B6 CLB */
    mflag = 1;  /* 0175BB SEM */
    wr8(ABS(0x020A), 0x0A);  /* 0175BC LDM */
    mflag = 0;  /* 0175C0 CLM */
    wr16(ABS(0x021C), 0x0139);  /* 0175C1 LDM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0020);  /* 0175C6 SEB */
    S -= 3; F_00828C();  /* 0175CB JSRL */
L_0175CF: TICK();
    goto L_0176B3;  /* 0175CF JMP */
L_0175D2: TICK();
    mflag = 0;  /* 0175D2 CLM */
    A = rd16(ABS(0x01A1));  /* 0175D3 LDA */
    mflag = 1;  /* 0175D6 SEM */
    /* 0175D7 CMP */
    if ((A & 0xFF) == 0x01) goto L_0175DE;  /* 0175D9 BEQ */
    goto L_0176B3;  /* 0175DB JMP */
L_0175DE: TICK();
    SETLO(A, rd8(ABS(0x019D)));  /* 0175DE LDA */
    if ((A & 0xFF) == 0) goto L_017609;  /* 0175E1 BEQ */
    mflag = 0;  /* 0175E3 CLM */
    S -= 3; F_017BBD();  /* 0175E4 JSRL */
    mflag = 1;  /* 0175E8 SEM */
    SETLO(A, rd8(ABS(0x01AB)));  /* 0175E9 LDA */
    if ((A & 0xFF) != 0) goto L_017609;  /* 0175EC BNE */
    wr8(ABS(0x01A1), (uint8_t)A);  /* 0175EE STA */
    wr8(ABS(0x0229), 0x01);  /* 0175F1 LDM */
    mflag = 0;  /* 0175F5 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 0175F6 CLB */
    wr16(ABS(0x021C), 0x0000);  /* 0175FB LDM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 017600 CLB */
    S -= 3; F_00828C();  /* 017605 JSRL */
L_017609: TICK();
    mflag = 0;  /* 017609 CLM */
    A = rd16(ABS(0x0228));  /* 01760A LDA */
    mflag = 1;  /* 01760D SEM */
    /* 01760E CMP */
    if ((A & 0xFF) != 0x03) goto L_01763B;  /* 017610 BNE */
    mflag = 0;  /* 017612 CLM */
    wr16(ABS(0x021C), 0x0139);  /* 017613 LDM */
    mflag = 1;  /* 017618 SEM */
    SETLO(A, rd8(ABS(0x020A)));  /* 017619 LDA */
    if ((A & 0xFF) != 0) goto L_017638;  /* 01761C BNE */
    SETLO(A, rd8(ABS(0x0229)));  /* 01761E LDA */
    /* 017621 CMP */
    if ((A & 0xFF) == 0x02) goto L_017638;  /* 017623 BEQ */
    SETLO(A, rd8(ABS(0x01AB)));  /* 017625 LDA */
    if ((A & 0xFF) == 0) goto L_017638;  /* 017628 BEQ */
    wr8(ABS(0x0229), 0x02);  /* 01762A LDM */
    mflag = 0;  /* 01762E CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x1000);  /* 01762F CLB */
    S -= 3; F_00CB52();  /* 017634 JSRL */
L_017638: TICK();
    goto L_0176B3;  /* 017638 JMP */
L_01763B: TICK();
    mflag = 1;  /* 01763B SEM */
    wr8(ABS(0x020A), 0x0A);  /* 01763C LDM */
    SETLO(A, rd8(ABS(0x0229)));  /* 017640 LDA */
    /* 017643 CMP */
    if ((A & 0xFF) == 0x01) goto L_017655;  /* 017645 BEQ */
    wr8(ABS(0x0229), 0x01);  /* 017647 LDM */
    mflag = 0;  /* 01764B CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x1000);  /* 01764C SEB */
    S -= 3; F_00CB52();  /* 017651 JSRL */
L_017655: TICK();
    mflag = 0;  /* 017655 CLM */
    X = rd16(ABS(0x021C));  /* 017656 LDX */
    if (X != 0) goto L_0176B3;  /* 017659 BNE */
    X = rd16(ABS(0x0197));  /* 01765B LDX */
    if (X == 0) goto L_01767C;  /* 01765E BEQ */
    mflag = 1;  /* 017660 SEM */
    wr8(ABS(0x01AB), 0x5A);  /* 017661 LDM */
    wr8(DP(0x00), 0x01);  /* 017665 LDM */
    mflag = 0;  /* 017668 CLM */
    wr16(DP(0x01), 0x01AB);  /* 017669 LDM */
    wr16(DP(0x03), 0x0000);  /* 01766D LDM */
    wr16(DP(0x04), 0x055D);  /* 017671 LDM */
    Y = 0x00A0;  /* 017675 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 017678 JSRL */
L_01767C: TICK();
    mflag = 1;  /* 01767C SEM */
    wr8(ABS(0x01AB), 0x00);  /* 01767D LDM */
    wr8(ABS(0x01A1), 0x00);  /* 017681 LDM */
    wr8(ABS(0x0229), 0x01);  /* 017685 LDM */
    mflag = 0;  /* 017689 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 01768A CLB */
    wr16(DP(0x00), 0x001A);  /* 01768F LDM */
    wr16(DP(0x02), 0x0040);  /* 017693 LDM */
    Y = 0x0002;  /* 017697 LDY */
    A = D;  /* 01769A TDA */
    C = 0;  /* 01769B CLC */
    A = adc16(A, 0x0004);  /* 01769C ADC */
    push16(A);  /* 01769F PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0176A0 JSRL */
    Y = pull16();  /* 0176A4 PLY */
    wr8(ABS(0x01AA), 0x00);  /* 0176A5 LDM */
    mflag = 0;  /* 0176A9 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 0176AA CLB */
    S -= 3; F_00828C();  /* 0176AF JSRL */
L_0176B3: TICK();
    mflag = 0;  /* 0176B3 CLM */
    Y = pull16();  /* 0176B4 PLY */
    D = pull16();  /* 0176B5 PLD */
    S += 3; return;  /* 0176B6 RTL */
}

/* 0176B7  callers: 016FC1 */
void F_0176B7(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0176B7 LDA */
    push16(D);  /* 0176B9 PHD */
    D = A;  /* 0176BA TAD */
    C = 0;  /* 0176BB CLC */
    A = adc16(A, 0x0004);  /* 0176BC ADC */
    push16(A);  /* 0176BF PHA */
    A = rd16(ABS(0x0228));  /* 0176C0 LDA */
    mflag = 1;  /* 0176C3 SEM */
    /* 0176C4 CMP */
    if ((A & 0xFF) != 0x04) goto L_0176E8;  /* 0176C6 BNE */
    mflag = 0;  /* 0176C8 CLM */
    X = rd16(ABS(0x021A));  /* 0176C9 LDX */
    if (X != 0) goto L_0176E5;  /* 0176CC BNE */
    wr16(DP(0x00), 0x0004);  /* 0176CE LDM */
    wr16(DP(0x02), 0x0040);  /* 0176D2 LDM */
    Y = 0x0002;  /* 0176D6 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0176D9 JSRL */
    mflag = 0;  /* 0176DD CLM */
    A = D;  /* 0176DE TDA */
    push16(A);  /* 0176DF PHA */
    S -= 3; F_017878();  /* 0176E0 JSRL */
    Y = pull16();  /* 0176E4 PLY */
L_0176E5: TICK();
    goto L_017835;  /* 0176E5 JMP */
L_0176E8: TICK();
    mflag = 0;  /* 0176E8 CLM */
    A = rd16(ABS(0x01A1));  /* 0176E9 LDA */
    mflag = 1;  /* 0176EC SEM */
    /* 0176ED CMP */
    if ((A & 0xFF) != 0x02) goto L_017730;  /* 0176EF BNE */
    SETLO(A, rd8(ABS(0x01A0)));  /* 0176F1 LDA */
    /* 0176F4 CMP */
    if ((A & 0xFF) == 0x03) goto L_0176FC;  /* 0176F6 BEQ */
    /* 0176F8 CMP */
    if ((A & 0xFF) != 0x02) goto L_017725;  /* 0176FA BNE */
L_0176FC: TICK();
    Y = rd16(ABS(0x01A0));  /* 0176FC LDY */
    mflag = 0;  /* 0176FF CLM */
    A = D;  /* 017700 TDA */
    push16(A);  /* 017701 PHA */
    S -= 3; F_0178C2();  /* 017702 JSRL */
    Y = pull16();  /* 017706 PLY */
    A = rd16(ABS(0x01A8));  /* 017707 LDA */
    A &= 0xE000;  /* 01770A AND */
    if (A != 0) goto L_017719;  /* 01770D BNE */
    A = D;  /* 01770F TDA */
    push16(A);  /* 017710 PHA */
    S -= 3; F_017839();  /* 017711 JSRL */
    Y = pull16();  /* 017715 PLY */
    goto L_017722;  /* 017716 JMP */
L_017719: TICK();
    mflag = 1;  /* 017719 SEM */
    wr8(ABS(0x01A1), 0x04);  /* 01771A LDM */
    wr8(ABS(0x0229), 0x03);  /* 01771E LDM */
L_017722: TICK();
    goto L_01772D;  /* 017722 JMP */
L_017725: TICK();
    mflag = 0;  /* 017725 CLM */
    A = D;  /* 017726 TDA */
    push16(A);  /* 017727 PHA */
    S -= 3; F_017878();  /* 017728 JSRL */
    Y = pull16();  /* 01772C PLY */
L_01772D: TICK();
    goto L_017835;  /* 01772D JMP */
L_017730: TICK();
    mflag = 0;  /* 017730 CLM */
    A = rd16(ABS(0x01A1));  /* 017731 LDA */
    mflag = 1;  /* 017734 SEM */
    /* 017735 CMP */
    if ((A & 0xFF) != 0x03) goto L_017752;  /* 017737 BNE */
    SETLO(A, rd8(ABS(0x020A)));  /* 017739 LDA */
    if ((A & 0xFF) != 0) goto L_01774F;  /* 01773C BNE */
    wr8(ABS(0x01A1), 0x05);  /* 01773E LDM */
    mflag = 0;  /* 017742 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0020);  /* 017743 SEB */
    A = D;  /* 017748 TDA */
    push16(A);  /* 017749 PHA */
    S -= 3; F_00828C();  /* 01774A JSRL */
    Y = pull16();  /* 01774E PLY */
L_01774F: TICK();
    goto L_017835;  /* 01774F JMP */
L_017752: TICK();
    mflag = 0;  /* 017752 CLM */
    A = rd16(ABS(0x01A1));  /* 017753 LDA */
    mflag = 1;  /* 017756 SEM */
    /* 017757 CMP */
    if ((A & 0xFF) != 0x04) goto L_0177A1;  /* 017759 BNE */
    mflag = 0;  /* 01775B CLM */
    if ((rd16(ABS(0x019E)) & 0x0008) == 0) goto L_01779E;  /* 01775C BBC */
    A = rd16(ABS(0x01A8));  /* 017762 LDA */
    A &= 0xE000;  /* 017765 AND */
    if (A == 0) goto L_017774;  /* 017768 BEQ */
    A = D;  /* 01776A TDA */
    push16(A);  /* 01776B PHA */
    S -= 3; F_017887();  /* 01776C JSRL */
    Y = pull16();  /* 017770 PLY */
    goto L_01779E;  /* 017771 JMP */
L_017774: TICK();
    mflag = 0;  /* 017774 CLM */
    wr16(DP(0x00), 0x0004);  /* 017775 LDM */
    wr16(DP(0x02), 0x0040);  /* 017779 LDM */
    Y = 0x0002;  /* 01777D LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017780 JSRL */
    SETLO(A, rd8(ABS(0x01A0)));  /* 017784 LDA */
    /* 017787 CMP */
    if ((A & 0xFF) == 0x02) goto L_017796;  /* 017789 BEQ */
    mflag = 0;  /* 01778B CLM */
    A = D;  /* 01778C TDA */
    push16(A);  /* 01778D PHA */
    S -= 3; F_017878();  /* 01778E JSRL */
    Y = pull16();  /* 017792 PLY */
    goto L_01779E;  /* 017793 JMP */
L_017796: TICK();
    mflag = 0;  /* 017796 CLM */
    A = D;  /* 017797 TDA */
    push16(A);  /* 017798 PHA */
    S -= 3; F_017839();  /* 017799 JSRL */
    Y = pull16();  /* 01779D PLY */
L_01779E: TICK();
    goto L_017835;  /* 01779E JMP */
L_0177A1: TICK();
    mflag = 0;  /* 0177A1 CLM */
    A = rd16(ABS(0x01A1));  /* 0177A2 LDA */
    mflag = 1;  /* 0177A5 SEM */
    /* 0177A6 CMP */
    if ((A & 0xFF) != 0x05) goto L_0177E7;  /* 0177A8 BNE */
    mflag = 0;  /* 0177AA CLM */
    A = D;  /* 0177AB TDA */
    push16(A);  /* 0177AC PHA */
    S -= 3; F_017A9F();  /* 0177AD JSRL */
    Y = pull16();  /* 0177B1 PLY */
    A = rd16(ABS(0x01A0));  /* 0177B2 LDA */
    mflag = 1;  /* 0177B5 SEM */
    /* 0177B6 CMP */
    if ((A & 0xFF) != 0x02) goto L_0177C1;  /* 0177B8 BNE */
    wr8(ABS(0x01A1), 0x06);  /* 0177BA LDM */
    goto L_0177E4;  /* 0177BE JMP */
L_0177C1: TICK();
    mflag = 1;  /* 0177C1 SEM */
    SETLO(B, 0x00);  /* 0177C2 LDA */
    SETLO(A, rd8(ABS(0x0348)));  /* 0177C5 LDA */
    div8(0x20);  /* 0177C8 DIV */
    SETLO(A, A + 1);  /* 0177CB INC */
    SETLO(A, A + 1);  /* 0177CC INC */
    wr8(ABS(0x020A), (uint8_t)A);  /* 0177CD STA */
    wr8(ABS(0x01A1), 0x07);  /* 0177D0 LDM */
    Y = rd16(ABS(0x01A0));  /* 0177D4 LDY */
    mflag = 0;  /* 0177D7 CLM */
    A = D;  /* 0177D8 TDA */
    push16(A);  /* 0177D9 PHA */
    S -= 3; F_0178C2();  /* 0177DA JSRL */
    Y = pull16();  /* 0177DE PLY */
    mflag = 1;  /* 0177DF SEM */
    wr8(ABS(0x0229), 0x02);  /* 0177E0 LDM */
L_0177E4: TICK();
    goto L_017835;  /* 0177E4 JMP */
L_0177E7: TICK();
    mflag = 0;  /* 0177E7 CLM */
    A = rd16(ABS(0x01A1));  /* 0177E8 LDA */
    mflag = 1;  /* 0177EB SEM */
    /* 0177EC CMP */
    if ((A & 0xFF) != 0x07) goto L_01781A;  /* 0177EE BNE */
    mflag = 0;  /* 0177F0 CLM */
    if ((rd16(ABS(0x023C)) & 0x0020) == 0) goto L_017803;  /* 0177F1 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 0177F7 CLB */
    A = D;  /* 0177FC TDA */
    push16(A);  /* 0177FD PHA */
    S -= 3; F_00828C();  /* 0177FE JSRL */
    Y = pull16();  /* 017802 PLY */
L_017803: TICK();
    mflag = 1;  /* 017803 SEM */
    SETLO(A, rd8(ABS(0x020A)));  /* 017804 LDA */
    if ((A & 0xFF) != 0) goto L_017817;  /* 017807 BNE */
    wr8(ABS(0x01A1), 0x04);  /* 017809 LDM */
    wr8(ABS(0x0229), 0x03);  /* 01780D LDM */
    mflag = 0;  /* 017811 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 017812 CLB */
L_017817: TICK();
    goto L_017835;  /* 017817 JMP */
L_01781A: TICK();
    mflag = 0;  /* 01781A CLM */
    A = rd16(ABS(0x01A1));  /* 01781B LDA */
    mflag = 1;  /* 01781E SEM */
    /* 01781F CMP */
    if ((A & 0xFF) != 0x06) goto L_017835;  /* 017821 BNE */
    mflag = 0;  /* 017823 CLM */
    wr16(ABS(0x021A), 0x0139);  /* 017824 LDM */
    A = rd16(ABS(0x01A0));  /* 017829 LDA */
    mflag = 1;  /* 01782C SEM */
    /* 01782D CMP */
    if ((A & 0xFF) == 0x02) goto L_017835;  /* 01782F BEQ */
    wr8(ABS(0x01A1), 0x05);  /* 017831 LDM */
L_017835: TICK();
    mflag = 0;  /* 017835 CLM */
    Y = pull16();  /* 017836 PLY */
    D = pull16();  /* 017837 PLD */
    S += 3; return;  /* 017838 RTL */
}

/* 017839  callers: 017711, 017799 */
void F_017839(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x037C));  /* 017839 LDA */
    mflag = 1;  /* 01783C SEM */
    SETLO(A, A | rd8(ABS(0x018A)));  /* 01783D ORA */
    wr8(ABS(0x037C), (uint8_t)A);  /* 017840 STA */
    wr8(ABS(0x01A1), 0x03);  /* 017843 LDM */
    wr8(ABS(0x0229), 0x04);  /* 017847 LDM */
    SETLO(A, A & 0x3F);  /* 01784B AND */
    if ((A & 0xFF) == 0) goto L_017854;  /* 01784D BEQ */
    SETLO(A, 0x13);  /* 01784F LDA */
    goto L_017856;  /* 017851 JMP */
L_017854: TICK();
    SETLO(A, 0x0A);  /* 017854 LDA */
L_017856: TICK();
    wr8(ABS(0x020A), (uint8_t)A);  /* 017856 STA */
    mflag = 0;  /* 017859 CLM */
    wr16(ABS(0x021A), 0x005E);  /* 01785A LDM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 01785F CLB */
    S += 3; return;  /* 017864 RTL */
}

/* 017865  callers: 017882 */
void F_017865(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x0365));  /* 017865 LDA */
    mflag = 1;  /* 017868 SEM */
    SETLO(A, A + 1);  /* 017869 INC */
    if ((A & 0xFF) == 0) goto L_017871;  /* 01786A BEQ */
    SETLO(A, 0x02);  /* 01786C LDA */
    goto L_017873;  /* 01786E JMP */
L_017871: TICK();
    SETLO(A, 0x00);  /* 017871 LDA */
L_017873: TICK();
    wr8(ABS(0x0229), (uint8_t)A);  /* 017873 STA */
    mflag = 0;  /* 017876 CLM */
    S += 3; return;  /* 017877 RTL */
}

/* 017878  callers: 016BC6, 0176E0, 017728, 01778E */
void F_017878(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 017878 SEM */
    wr8(ABS(0x01A1), 0x02);  /* 017879 LDM */
    wr8(ABS(0x01A0), 0x00);  /* 01787D LDM */
    mflag = 0;  /* 017881 CLM */
    S -= 3; F_017865();  /* 017882 JSRL */
    S += 3; return;  /* 017886 RTL */
}

/* 017887  callers: 01776C */
void F_017887(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017887 LDA */
    push16(D);  /* 017889 PHD */
    D = A;  /* 01788A TAD */
    C = 0;  /* 01788B CLC */
    A = adc16(A, 0x0004);  /* 01788C ADC */
    push16(A);  /* 01788F PHA */
    mflag = 1;  /* 017890 SEM */
    wr8(ABS(0x019B), 0x01);  /* 017891 LDM */
    wr8(ABS(0x01AA), 0x00);  /* 017895 LDM */
    mflag = 0;  /* 017899 CLM */
    wr16(DP(0x00), 0x0009);  /* 01789A LDM */
    wr16(DP(0x02), 0x0040);  /* 01789E LDM */
    Y = 0x0002;  /* 0178A2 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0178A5 JSRL */
    mflag = 0;  /* 0178A9 CLM */
    wr16(DP(0x00), 0x00AA);  /* 0178AA LDM */
    wr16(DP(0x02), 0x0043);  /* 0178AE LDM */
    Y = 0x0002;  /* 0178B2 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0178B5 JSRL */
    mflag = 0;  /* 0178B9 CLM */
    wr16(ABS(0x019E), rd16(ABS(0x019E)) & ~0x0008);  /* 0178BA CLB */
    Y = pull16();  /* 0178BF PLY */
    D = pull16();  /* 0178C0 PLD */
    S += 3; return;  /* 0178C1 RTL */
}

/* 0178C2  callers: 017702, 0177DA */
void F_0178C2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0178C2 LDA */
    push16(D);  /* 0178C4 PHD */
    D = A;  /* 0178C5 TAD */
    wr16(DP(0x00), Y);  /* 0178C6 STY */
    A = rd16(ABS(0x0196));  /* 0178C8 LDA */
    A &= 0x00FF;  /* 0178CB AND */
    mflag = 1;  /* 0178CE SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 0178CF STA */
    SETLO(A, rd8(ABS(0x0195)));  /* 0178D1 LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 0178D4 STA */
    SETLO(A, Y);  /* 0178D6 TYA */
    /* 0178D7 CMP */
    if ((A & 0xFF) == 0x03) goto L_017918;  /* 0178D9 BEQ */
    mflag = 0;  /* 0178DB CLM */
    wr16(DP(0x08), 0x0063);  /* 0178DC LDM */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0x1F00);  /* 0178E0 CLB */
    A = Y;  /* 0178E4 TYA */
    mflag = 1;  /* 0178E5 SEM */
    /* 0178E6 CMP */
    if ((A & 0xFF) != 0x02) goto L_01790C;  /* 0178E8 BNE */
    mflag = 0;  /* 0178EA CLM */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0) goto L_0178F7;  /* 0178EB BBC */
    A = 0x0002;  /* 0178F1 LDA */
    goto L_0178FA;  /* 0178F4 JMP */
L_0178F7: TICK();
    A = 0x0001;  /* 0178F7 LDA */
L_0178FA: TICK();
    mflag = 1;  /* 0178FA SEM */
    SETLO(A, A & 0x07);  /* 0178FB AND */
    SETLO(A, asl8(A));  /* 0178FD ASL */
    SETLO(A, asl8(A));  /* 0178FE ASL */
    SETLO(A, asl8(A));  /* 0178FF ASL */
    SETLO(A, asl8(A));  /* 017900 ASL */
    SETLO(A, asl8(A));  /* 017901 ASL */
    wr8(DP(0x04), rd8(DP(0x04)) & ~0xE0);  /* 017902 CLB */
    SETLO(A, A | rd8(DP(0x04)));  /* 017905 ORA */
    wr8(DP(0x04), (uint8_t)A);  /* 017907 STA */
    goto L_017915;  /* 017909 JMP */
L_01790C: TICK();
    mflag = 1;  /* 01790C SEM */
    SETLO(A, rd8(DP(0x04)));  /* 01790D LDA */
    SETLO(A, A & 0x1F);  /* 01790F AND */
    SETLO(A, A | 0x80);  /* 017911 ORA */
    wr8(DP(0x04), (uint8_t)A);  /* 017913 STA */
L_017915: TICK();
    goto L_0179F8;  /* 017915 JMP */
L_017918: TICK();
    mflag = 0;  /* 017918 CLM */
    wr16(DP(0x0A), 0x0000);  /* 017919 LDM */
    mflag = 1;  /* 01791D SEM */
    SETLO(A, rd8(ABS(0x0293)));  /* 01791E LDA */
    if ((A & 0xFF) == 0) goto L_01796D;  /* 017921 BEQ */
    mflag = 0;  /* 017923 CLM */
    wr16(DP(0x08), 0x0000);  /* 017924 LDM */
    wr16(DP(0x0C), 0x0000);  /* 017928 LDM */
L_01792C: TICK();
    A = rd16(ABS(0x0293));  /* 01792C LDA */
    A &= 0x00FF;  /* 01792F AND */
    t = rd16(DP(0x0C));  /* 017932 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_01796A;  /* 017934 BCC */
    if (Z) goto L_01796A;  /* 017936 BEQ */
    X = rd16(DP(0x0C));  /* 017938 LDX */
    A = rd16((ABS(0x0283) + X));  /* 01793A LDA */
    mflag = 1;  /* 01793D SEM */
    /* 01793E CMP */
    if ((A & 0xFF) != 0x2A) goto L_01794A;  /* 017940 BNE */
    mflag = 0;  /* 017942 CLM */
    wr16(DP(0x0A), 0x0001);  /* 017943 LDM */
    goto L_01796A;  /* 017947 JMP */
L_01794A: TICK();
    X = rd16(DP(0x0C));  /* 01794A LDX */
    mflag = 0;  /* 01794C CLM */
    A = rd16((ABS(0x0283) + X));  /* 01794D LDA */
    A &= 0x00FF;  /* 017950 AND */
    wr16(DP(0x0E), A);  /* 017953 STA */
    A = rd16(DP(0x08));  /* 017955 LDA */
    mpy16(0x000A);  /* 017957 MPY */
    C = 0;  /* 01795B CLC */
    A = adc16(A, rd16(DP(0x0E)));  /* 01795C ADC */
    C = 0;  /* 01795E CLC */
    A = adc16(A, 0xFFD0);  /* 01795F ADC */
    wr16(DP(0x08), A);  /* 017962 STA */
    X += 1;  /* 017964 INX */
    wr16(DP(0x0C), X);  /* 017965 STX */
    goto L_01792C;  /* 017967 JMP */
L_01796A: TICK();
    goto L_017972;  /* 01796A JMP */
L_01796D: TICK();
    mflag = 0;  /* 01796D CLM */
    wr16(DP(0x08), 0x0063);  /* 01796E LDM */
L_017972: TICK();
    mflag = 0;  /* 017972 CLM */
    X = rd16(DP(0x0A));  /* 017973 LDX */
    if (X == 0) goto L_0179D4;  /* 017975 BEQ */
    A = rd16(ABS(0x0293));  /* 017977 LDA */
    A &= 0x00FF;  /* 01797A AND */
    A -= 1;  /* 01797D DEC */
    X = A;  /* 01797E TAX */
    A = rd16((ABS(0x0283) + X));  /* 01797F LDA */
    C = 0;  /* 017982 CLC */
    mflag = 1;  /* 017983 SEM */
    SETLO(A, adc8(A, 0xD0));  /* 017984 ADC */
    wr8(DP(0x07), (uint8_t)A);  /* 017986 STA */
    if (!Z) goto L_017991;  /* 017988 BNE */
    mflag = 0;  /* 01798A CLM */
    A = 0x0000;  /* 01798B LDA */
    goto L_0179A4;  /* 01798E JMP */
L_017991: TICK();
    mflag = 0;  /* 017991 CLM */
    A = rd16(DP(0x07));  /* 017992 LDA */
    mflag = 1;  /* 017994 SEM */
    /* 017995 CMP */
    if ((A & 0xFF) != 0x09) goto L_01799E;  /* 017997 BNE */
    SETLO(A, 0x05);  /* 017999 LDA */
    goto L_0179A0;  /* 01799B JMP */
L_01799E: TICK();
    SETLO(A, 0x03);  /* 01799E LDA */
L_0179A0: TICK();
    mflag = 0;  /* 0179A0 CLM */
    A &= 0x00FF;  /* 0179A1 AND */
L_0179A4: TICK();
    mflag = 1;  /* 0179A4 SEM */
    SETLO(A, A & 0x07);  /* 0179A5 AND */
    SETLO(A, asl8(A));  /* 0179A7 ASL */
    SETLO(A, asl8(A));  /* 0179A8 ASL */
    SETLO(A, asl8(A));  /* 0179A9 ASL */
    SETLO(A, asl8(A));  /* 0179AA ASL */
    SETLO(A, asl8(A));  /* 0179AB ASL */
    wr8(DP(0x04), rd8(DP(0x04)) & ~0xE0);  /* 0179AC CLB */
    SETLO(A, A | rd8(DP(0x04)));  /* 0179AF ORA */
    wr8(DP(0x04), (uint8_t)A);  /* 0179B1 STA */
    SETLO(A, rd8(DP(0x07)));  /* 0179B3 LDA */
    if ((A & 0xFF) == 0) goto L_0179C3;  /* 0179B5 BEQ */
    /* 0179B7 CMP */
    if ((A & 0xFF) == 0x09) goto L_0179C3;  /* 0179B9 BEQ */
    mflag = 0;  /* 0179BB CLM */
    A &= 0x00FF;  /* 0179BC AND */
    A -= 1;  /* 0179BF DEC */
    goto L_0179C7;  /* 0179C0 JMP */
L_0179C3: TICK();
    mflag = 0;  /* 0179C3 CLM */
    A = 0x0000;  /* 0179C4 LDA */
L_0179C7: TICK();
    mflag = 1;  /* 0179C7 SEM */
    SETLO(A, A & 0x1F);  /* 0179C8 AND */
    wr8(DP(0x04), rd8(DP(0x04)) & ~0x1F);  /* 0179CA CLB */
    SETLO(A, A | rd8(DP(0x04)));  /* 0179CD ORA */
    wr8(DP(0x04), (uint8_t)A);  /* 0179CF STA */
    goto L_0179F8;  /* 0179D1 JMP */
L_0179D4: TICK();
    mflag = 0;  /* 0179D4 CLM */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0) goto L_0179E1;  /* 0179D5 BBC */
    A = 0x0002;  /* 0179DB LDA */
    goto L_0179E4;  /* 0179DE JMP */
L_0179E1: TICK();
    A = 0x0001;  /* 0179E1 LDA */
L_0179E4: TICK();
    mflag = 1;  /* 0179E4 SEM */
    SETLO(A, A & 0x07);  /* 0179E5 AND */
    SETLO(A, asl8(A));  /* 0179E7 ASL */
    SETLO(A, asl8(A));  /* 0179E8 ASL */
    SETLO(A, asl8(A));  /* 0179E9 ASL */
    SETLO(A, asl8(A));  /* 0179EA ASL */
    SETLO(A, asl8(A));  /* 0179EB ASL */
    wr8(DP(0x04), rd8(DP(0x04)) & ~0xE0);  /* 0179EC CLB */
    SETLO(A, A | rd8(DP(0x04)));  /* 0179EF ORA */
    wr8(DP(0x04), (uint8_t)A);  /* 0179F1 STA */
    mflag = 0;  /* 0179F3 CLM */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0x1F00);  /* 0179F4 CLB */
L_0179F8: TICK();
    mflag = 0;  /* 0179F8 CLM */
    A = rd16(DP(0x08));  /* 0179F9 LDA */
    mflag = 1;  /* 0179FB SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 0179FC STA */
    mflag = 0;  /* 0179FE CLM */
    A = rd16(DP(0x09));  /* 0179FF LDA */
    A &= 0x00FF;  /* 017A01 AND */
    mflag = 1;  /* 017A04 SEM */
    SETLO(A, A & 0x07);  /* 017A05 AND */
    wr8(DP(0x06), rd8(DP(0x06)) & ~0x07);  /* 017A07 CLB */
    SETLO(A, A | rd8(DP(0x06)));  /* 017A0A ORA */
    wr8(DP(0x06), (uint8_t)A);  /* 017A0C STA */
    SETLO(A, rd8(ABS(0x0199)));  /* 017A0E LDA */
    wr8(DP(0x05), (uint8_t)A);  /* 017A11 STA */
    SETLO(A, rd8(DP(0x00)));  /* 017A13 LDA */
    /* 017A15 CMP */
    if ((A & 0xFF) != 0x02) goto L_017A25;  /* 017A17 BNE */
    mflag = 0;  /* 017A19 CLM */
    X = rd16(ABS(0x021C));  /* 017A1A LDX */
    if (X == 0) goto L_017A25;  /* 017A1D BEQ */
    A = 0x0000;  /* 017A1F LDA */
    goto L_017A29;  /* 017A22 JMP */
L_017A25: TICK();
    mflag = 0;  /* 017A25 CLM */
    A = 0x0001;  /* 017A26 LDA */
L_017A29: TICK();
    mflag = 1;  /* 017A29 SEM */
    SETLO(A, A & 0x07);  /* 017A2A AND */
    SETLO(A, asl8(A));  /* 017A2C ASL */
    SETLO(A, asl8(A));  /* 017A2D ASL */
    SETLO(A, asl8(A));  /* 017A2E ASL */
    SETLO(A, asl8(A));  /* 017A2F ASL */
    SETLO(A, asl8(A));  /* 017A30 ASL */
    wr8(DP(0x06), rd8(DP(0x06)) & ~0xE0);  /* 017A31 CLB */
    SETLO(A, A | rd8(DP(0x06)));  /* 017A34 ORA */
    wr8(DP(0x06), (uint8_t)A);  /* 017A36 STA */
    mflag = 0;  /* 017A38 CLM */
    wr16(DP(0x05), rd16(DP(0x05)) & ~0x1800);  /* 017A39 CLB */
    A = D;  /* 017A3D TDA */
    A += 1;  /* 017A3E INC */
    wr16(DP(0x0E), A);  /* 017A3F STA */
    wr16(DP(0x10), 0x01A4);  /* 017A41 LDM */
    A = 0x000E;  /* 017A45 LDA */
    Y = 0x0006;  /* 017A48 LDY */
    S -= 3; rt_memcpy_01E466();  /* 017A4B JSRL */
    mflag = 1;  /* 017A4F SEM */
    wr8(ABS(0x0293), 0x00);  /* 017A50 LDM */
    mflag = 0;  /* 017A54 CLM */
    wr16(ABS(0x019E), rd16(ABS(0x019E)) | 0x0008);  /* 017A55 SEB */
    A = rd16(DP(0x00));  /* 017A5A LDA */
    mflag = 1;  /* 017A5C SEM */
    /* 017A5D CMP */
    if ((A & 0xFF) != 0x03) goto L_017A86;  /* 017A5F BNE */
    SETLO(A, rd8(DP(0x04)));  /* 017A61 LDA */
    SETLO(A, lsr8(A));  /* 017A63 LSR */
    SETLO(A, lsr8(A));  /* 017A64 LSR */
    SETLO(A, lsr8(A));  /* 017A65 LSR */
    SETLO(A, lsr8(A));  /* 017A66 LSR */
    SETLO(A, lsr8(A));  /* 017A67 LSR */
    mflag = 0;  /* 017A68 CLM */
    A &= 0x0007;  /* 017A69 AND */
    /* 017A6C CMP */
    if (A == 0x0002) goto L_017A80;  /* 017A6F BEQ */
    A = rd16(DP(0x04));  /* 017A71 LDA */
    mflag = 1;  /* 017A73 SEM */
    SETLO(A, lsr8(A));  /* 017A74 LSR */
    SETLO(A, lsr8(A));  /* 017A75 LSR */
    SETLO(A, lsr8(A));  /* 017A76 LSR */
    SETLO(A, lsr8(A));  /* 017A77 LSR */
    SETLO(A, lsr8(A));  /* 017A78 LSR */
    mflag = 0;  /* 017A79 CLM */
    A &= 0x0007;  /* 017A7A AND */
    A -= 1;  /* 017A7D DEC */
    if (A != 0) goto L_017A86;  /* 017A7E BNE */
L_017A80: TICK();
    A = 0x0139;  /* 017A80 LDA */
    goto L_017A8A;  /* 017A83 JMP */
L_017A86: TICK();
    mflag = 0;  /* 017A86 CLM */
    A = 0x0000;  /* 017A87 LDA */
L_017A8A: TICK();
    wr16(ABS(0x021C), A);  /* 017A8A STA */
    mflag = 1;  /* 017A8D SEM */
    wr8(ABS(0x037C), rd8(ABS(0x037C)) & ~0x3F);  /* 017A8E CLB */
    mflag = 0;  /* 017A92 CLM */
    wr16(ABS(0x021A), 0x005E);  /* 017A93 LDM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 017A98 CLB */
    D = pull16();  /* 017A9D PLD */
    S += 3; return;  /* 017A9E RTL */
}

/* 017A9F  callers: 0177AD */
void F_017A9F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017A9F LDA */
    push16(D);  /* 017AA1 PHD */
    D = A;  /* 017AA2 TAD */
    C = 0;  /* 017AA3 CLC */
    A = adc16(A, 0x0004);  /* 017AA4 ADC */
    push16(A);  /* 017AA7 PHA */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_017AB1;  /* 017AA8 BBC */
    goto L_017BBA;  /* 017AAE JMP */
L_017AB1: TICK();
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0001);  /* 017AB1 SEB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0800);  /* 017AB6 CLB */
    X = rd16(ABS(0x029A));  /* 017ABB LDX */
    wr16(DP(0x02), X);  /* 017ABE STX */
    wr16(ABS(0x029A), rd16(ABS(0x029A)) | 0x8000);  /* 017AC0 SEB */
    wr16(ABS(0x029A), rd16(ABS(0x029A)) & ~0x4000);  /* 017AC5 CLB */
    mflag = 1;  /* 017ACA SEM */
    SETLO(A, rd8(ABS(0x029B)));  /* 017ACB LDA */
    SETLO(A, A & 0xF8);  /* 017ACE AND */
    SETLO(A, A | 0x06);  /* 017AD0 ORA */
    wr8(ABS(0x029B), (uint8_t)A);  /* 017AD2 STA */
    mflag = 0;  /* 017AD5 CLM */
    S -= 3; F_00CB52();  /* 017AD6 JSRL */
    X = rd16(DP(0x02));  /* 017ADA LDX */
    wr16(ABS(0x029A), X);  /* 017ADC STX */
    mflag = 1;  /* 017ADF SEM */
    wr8(ABS(TA1IC), 0x00);  /* 017AE0 LDM */
    wr8(ABS(TA1MOD), 0x06);  /* 017AE4 LDM */
    wr8(ABS(TA1IC), rd8(ABS(TA1IC)) & ~0x08);  /* 017AE8 CLB */
    wr8(ABS(TB1IC), 0x06);  /* 017AEC LDM */
    wr8(ABS(TB1MOD), 0x00);  /* 017AF0 LDM */
    wr8(ABS(TB1IC), rd8(ABS(TB1IC)) & ~0x08);  /* 017AF4 CLB */
    wr8(ABS(TA4IC), 0x00);  /* 017AF8 LDM */
    wr8(ABS(TA4MOD), 0xC0);  /* 017AFC LDM */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 017B00 CLB */
    SETLO(A, 0x01);  /* 017B04 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 017B06 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 017B07 STA */
    setP(pull16());  /* 017B09 PLP */
    mflag = 0;  /* 017B0A CLM */
    wr16(DP(0x00), 0x0000);  /* 017B0B LDM */
L_017B0F: TICK();
    X = rd16(DP(0x00));  /* 017B0F LDX */
    C = (X >= 0x0003);  /* 017B11 CPX */
    if (!C) goto L_017B19;  /* 017B14 BCC */
    goto L_017B9F;  /* 017B16 JMP */
L_017B19: TICK();
    mflag = 1;  /* 017B19 SEM */
    wr8(ABS(0x025F), 0x00);  /* 017B1A LDM */
    mflag = 0;  /* 017B1E CLM */
    A = X;  /* 017B1F TXA */
    mpy16(0x0005);  /* 017B20 MPY */
    X = A;  /* 017B24 TAX */
    A = rd16((0x0021E4 + X));  /* 017B25 LDA */
    mflag = 1;  /* 017B29 SEM */
    wr8(ABS(0x0260), (uint8_t)A);  /* 017B2A STA */
    mflag = 0;  /* 017B2D CLM */
    A = rd16(DP(0x00));  /* 017B2E LDA */
    mpy16(0x0005);  /* 017B30 MPY */
    X = A;  /* 017B34 TAX */
    A = rd16((0x0021E0 + X));  /* 017B35 LDA */
    wr16(ABS(TB1), A);  /* 017B39 STA */
    A = rd16(DP(0x00));  /* 017B3C LDA */
    mpy16(0x0005);  /* 017B3E MPY */
    X = A;  /* 017B42 TAX */
    A = rd16((0x0021E2 + X));  /* 017B43 LDA */
    A = lsr16(A);  /* 017B47 LSR */
    A = lsr16(A);  /* 017B48 LSR */
    mpy16(0x003F);  /* 017B49 MPY */
    wr16(ABS(TA4), A);  /* 017B4D STA */
    A = rd16(0x0028FA);  /* 017B50 LDA */
    wr16(ABS(TA1), A);  /* 017B54 STA */
    mflag = 1;  /* 017B57 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x10);  /* 017B58 SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x02);  /* 017B5C SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x40);  /* 017B60 SEB */
    wr8(ABS(TOSTART), rd8(ABS(TOSTART)) | 0x02);  /* 017B64 SEB */
    SETLO(A, rd8(ABS(0x025F)));  /* 017B68 LDA */
    C = 0;  /* 017B6B CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0260))));  /* 017B6C ADC */
    wr8(ABS(0x025F), (uint8_t)A);  /* 017B6F STA */
    mflag = 0;  /* 017B72 CLM */
    A &= 0x00FF;  /* 017B73 AND */
    A = asl16(A);  /* 017B76 ASL */
    X = A;  /* 017B77 TAX */
    A = rd16((0x0028FA + X));  /* 017B78 LDA */
    wr16(ABS(TA1), A);  /* 017B7C STA */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 017B7F INC */
L_017B81: TICK();
    mflag = 1;  /* 017B81 SEM */
    wr8(ABS(WDT), 0x00);  /* 017B82 LDM */
    if ((rd8(ABS(TA4IC)) & 0x08) == 0) goto L_017B81;  /* 017B86 BBC */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x10);  /* 017B8B CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x02);  /* 017B8F CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x40);  /* 017B93 CLB */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 017B97 CLB */
    mflag = 0;  /* 017B9B CLM */
    goto L_017B0F;  /* 017B9C JMP */
L_017B9F: TICK();
    mflag = 1;  /* 017B9F SEM */
    SETLO(A, 0x00);  /* 017BA0 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 017BA2 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 017BA3 STA */
    setP(pull16());  /* 017BA5 PLP */
    mflag = 1;  /* 017BA6 SEM */
    wr8(ABS(TB1IC), 0x00);  /* 017BA7 LDM */
    mflag = 0;  /* 017BAB CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0001);  /* 017BAC CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0800);  /* 017BB1 SEB */
    S -= 3; F_00CB52();  /* 017BB6 JSRL */
L_017BBA: TICK();
    Y = pull16();  /* 017BBA PLY */
    D = pull16();  /* 017BBB PLD */
    S += 3; return;  /* 017BBC RTL */
}

/* 017BBD  callers: 0175A5, 0175E4 */
void F_017BBD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017BBD LDA */
    push16(D);  /* 017BBF PHD */
    D = A;  /* 017BC0 TAD */
    C = 0;  /* 017BC1 CLC */
    A = adc16(A, 0x000B);  /* 017BC2 ADC */
    push16(A);  /* 017BC5 PHA */
    A = D;  /* 017BC6 TDA */
    C = 0;  /* 017BC7 CLC */
    A = adc16(A, 0x0005);  /* 017BC8 ADC */
    wr16(DP(0x0B), A);  /* 017BCB STA */
    wr16(DP(0x0D), 0x01A4);  /* 017BCD LDM */
    wr16(DP(0x0F), A);  /* 017BD1 STA */
    A = 0x000D;  /* 017BD3 LDA */
    Y = 0x0006;  /* 017BD6 LDY */
    S -= 3; rt_memcpy_01E466();  /* 017BD9 JSRL */
    mflag = 1;  /* 017BDD SEM */
    wr8(ABS(0x019D), 0x00);  /* 017BDE LDM */
    wr8(DP(0x04), 0x00);  /* 017BE2 LDM */
    mflag = 0;  /* 017BE5 CLM */
    if ((rd16(ABS(0x019E)) & 0x0002) == 0x0002) goto L_017BEF;  /* 017BE6 BBS */
    goto L_017D58;  /* 017BEC JMP */
L_017BEF: TICK();
    A = rd16(DP(0x05));  /* 017BEF LDA */
    A &= 0x00FF;  /* 017BF1 AND */
    A = rla16(A, 0x0008);  /* 017BF4 RLA */
    A &= 0xFF00;  /* 017BF8 AND */
    wr16(DP(0x0B), A);  /* 017BFB STA */
    A = rd16(DP(0x06));  /* 017BFD LDA */
    A &= 0x00FF;  /* 017BFF AND */
    C = 0;  /* 017C02 CLC */
    A = adc16(A, rd16(DP(0x0B)));  /* 017C03 ADC */
    t = rd16(ABS(0x0195));  /* 017C05 CMP */
    if (A == t) goto L_017C0D;  /* 017C08 BEQ */
    goto L_017D58;  /* 017C0A JMP */
L_017C0D: TICK();
    A = rd16(ABS(0x0199));  /* 017C0D LDA */
    A &= 0x00FF;  /* 017C10 AND */
    wr16(DP(0x0B), A);  /* 017C13 STA */
    A = rd16(DP(0x09));  /* 017C15 LDA */
    A &= 0x00FF;  /* 017C17 AND */
    t = rd16(DP(0x0B));  /* 017C1A CMP */
    if (A == t) goto L_017C21;  /* 017C1C BEQ */
    goto L_017D58;  /* 017C1E JMP */
L_017C21: TICK();
    A = rd16(DP(0x0A));  /* 017C21 LDA */
    mflag = 1;  /* 017C23 SEM */
    SETLO(A, lsr8(A));  /* 017C24 LSR */
    SETLO(A, lsr8(A));  /* 017C25 LSR */
    SETLO(A, lsr8(A));  /* 017C26 LSR */
    SETLO(A, lsr8(A));  /* 017C27 LSR */
    SETLO(A, lsr8(A));  /* 017C28 LSR */
    mflag = 0;  /* 017C29 CLM */
    A &= 0x0007;  /* 017C2A AND */
    /* 017C2D CMP */
    if (A >= 0x0001) goto L_017C35;  /* 017C30 BCS */
    goto L_017D58;  /* 017C32 JMP */
L_017C35: TICK();
    A = rd16(DP(0x0A));  /* 017C35 LDA */
    mflag = 1;  /* 017C37 SEM */
    SETLO(A, lsr8(A));  /* 017C38 LSR */
    SETLO(A, lsr8(A));  /* 017C39 LSR */
    SETLO(A, lsr8(A));  /* 017C3A LSR */
    SETLO(A, lsr8(A));  /* 017C3B LSR */
    SETLO(A, lsr8(A));  /* 017C3C LSR */
    mflag = 0;  /* 017C3D CLM */
    A &= 0x0007;  /* 017C3E AND */
    C = (A >= 0x0004);  /* 017C41 CMP */
    if (!C) goto L_017C49;  /* 017C44 BCC */
    goto L_017D58;  /* 017C46 JMP */
L_017C49: TICK();
    A = rd16(DP(0x0A));  /* 017C49 LDA */
    A &= 0x0007;  /* 017C4B AND */
    A = rla16(A, 0x0008);  /* 017C4E RLA */
    A &= 0xFF00;  /* 017C52 AND */
    wr16(DP(0x0B), A);  /* 017C55 STA */
    A = rd16(DP(0x07));  /* 017C57 LDA */
    A &= 0x00FF;  /* 017C59 AND */
    A = adc16(A, rd16(DP(0x0B)));  /* 017C5C ADC */
    wr16(DP(0x02), A);  /* 017C5E STA */
    /* 017C60 CMP */
    if (A == 0x0063) goto L_017C6A;  /* 017C63 BEQ */
    t = rd16(ABS(0x0197));  /* 017C65 CMP */
    if (A != t) goto L_017C71;  /* 017C68 BNE */
L_017C6A: TICK();
    mflag = 1;  /* 017C6A SEM */
    wr8(DP(0x04), 0x01);  /* 017C6B LDM */
    goto L_017C96;  /* 017C6E JMP */
L_017C71: TICK();
    mflag = 0;  /* 017C71 CLM */
    wr16(DP(0x00), 0x0000);  /* 017C72 LDM */
L_017C76: TICK();
    mflag = 1;  /* 017C76 SEM */
    SETLO(A, rd8(DP(0x04)));  /* 017C77 LDA */
    if ((A & 0xFF) != 0) goto L_017C96;  /* 017C79 BNE */
    mflag = 0;  /* 017C7B CLM */
    X = rd16(DP(0x00));  /* 017C7C LDX */
    /* 017C7E CPX */
    if (X >= 0x000A) goto L_017C96;  /* 017C81 BCS */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 017C83 INC */
    A = rd16((ABS(0x018B) + X));  /* 017C85 LDA */
    A &= 0x00FF;  /* 017C88 AND */
    t = rd16(DP(0x02));  /* 017C8B CMP */
    if (A != t) goto L_017C93;  /* 017C8D BNE */
    mflag = 1;  /* 017C8F SEM */
    wr8(DP(0x04), 0x01);  /* 017C90 LDM */
L_017C93: TICK();
    goto L_017C76;  /* 017C93 JMP */
L_017C96: TICK();
    mflag = 0;  /* 017C96 CLM */
    A = rd16(DP(0x04));  /* 017C97 LDA */
    mflag = 1;  /* 017C99 SEM */
    /* 017C9A CMP */
    if ((A & 0xFF) == 0x01) goto L_017CA1;  /* 017C9C BEQ */
    goto L_017D58;  /* 017C9E JMP */
L_017CA1: TICK();
    mflag = 0;  /* 017CA1 CLM */
    wr16(ABS(0x021C), 0x0000);  /* 017CA2 LDM */
    A = rd16(DP(0x08));  /* 017CA7 LDA */
    mflag = 1;  /* 017CA9 SEM */
    SETLO(A, lsr8(A));  /* 017CAA LSR */
    SETLO(A, lsr8(A));  /* 017CAB LSR */
    SETLO(A, lsr8(A));  /* 017CAC LSR */
    SETLO(A, lsr8(A));  /* 017CAD LSR */
    SETLO(A, lsr8(A));  /* 017CAE LSR */
    mflag = 0;  /* 017CAF CLM */
    A &= 0x0007;  /* 017CB0 AND */
    if (A != 0) goto L_017CD3;  /* 017CB3 BNE */
    A = rd16(DP(0x07));  /* 017CB5 LDA */
    A &= 0x00FF;  /* 017CB7 AND */
    /* 017CBA CMP */
    if (A == 0x0063) goto L_017CCC;  /* 017CBD BEQ */
    mflag = 1;  /* 017CBF SEM */
    wr8(ABS(0x019A), rd8(ABS(0x019A)) | 0x80);  /* 017CC0 SEB */
    Y = rd16(ABS(0x019A));  /* 017CC4 LDY */
    mflag = 0;  /* 017CC7 CLM */
    S -= 3; F_016F15();  /* 017CC8 JSRL */
L_017CCC: TICK();
    mflag = 1;  /* 017CCC SEM */
    wr8(DP(0x04), 0x00);  /* 017CCD LDM */
    goto L_017D58;  /* 017CD0 JMP */
L_017CD3: TICK();
    mflag = 0;  /* 017CD3 CLM */
    /* 017CD4 CMP */
    if (A != 0x0001) goto L_017CF4;  /* 017CD7 BNE */
    mflag = 1;  /* 017CD9 SEM */
    if ((rd8(ABS(0x019A)) & 0x80) == 0) goto L_017CE5;  /* 017CDA BBC */
    wr8(DP(0x04), 0x00);  /* 017CDF LDM */
    goto L_017CF1;  /* 017CE2 JMP */
L_017CE5: TICK();
    mflag = 0;  /* 017CE5 CLM */
    wr16(DP(0x0B), 0x0000);  /* 017CE6 LDM */
    Y = rd16(ABS(0x019A));  /* 017CEA LDY */
    S -= 3; F_016F45();  /* 017CED JSRL */
L_017CF1: TICK();
    goto L_017D58;  /* 017CF1 JMP */
L_017CF4: TICK();
    mflag = 0;  /* 017CF4 CLM */
    /* 017CF5 CMP */
    if (A != 0x0002) goto L_017D15;  /* 017CF8 BNE */
    mflag = 1;  /* 017CFA SEM */
    if ((rd8(ABS(0x019A)) & 0x80) == 0) goto L_017D06;  /* 017CFB BBC */
    wr8(DP(0x04), 0x00);  /* 017D00 LDM */
    goto L_017D12;  /* 017D03 JMP */
L_017D06: TICK();
    mflag = 0;  /* 017D06 CLM */
    wr16(DP(0x0B), 0x0001);  /* 017D07 LDM */
    Y = rd16(ABS(0x019A));  /* 017D0B LDY */
    S -= 3; F_016F45();  /* 017D0E JSRL */
L_017D12: TICK();
    goto L_017D58;  /* 017D12 JMP */
L_017D15: TICK();
    mflag = 0;  /* 017D15 CLM */
    /* 017D16 CMP */
    if (A != 0x0003) goto L_017D31;  /* 017D19 BNE */
    A = rd16(DP(0x08));  /* 017D1B LDA */
    A &= 0x001F;  /* 017D1D AND */
    mflag = 1;  /* 017D20 SEM */
    wr8(ABS(0x019A), (uint8_t)A);  /* 017D21 STA */
    mflag = 0;  /* 017D24 CLM */
    Y = A;  /* 017D25 TAY */
    S -= 3; F_016F15();  /* 017D26 JSRL */
    mflag = 1;  /* 017D2A SEM */
    wr8(DP(0x04), 0x00);  /* 017D2B LDM */
    goto L_017D58;  /* 017D2E JMP */
L_017D31: TICK();
    mflag = 0;  /* 017D31 CLM */
    /* 017D32 CMP */
    if (A != 0x0004) goto L_017D3E;  /* 017D35 BNE */
    mflag = 1;  /* 017D37 SEM */
    wr8(DP(0x04), 0x00);  /* 017D38 LDM */
    goto L_017D58;  /* 017D3B JMP */
L_017D3E: TICK();
    mflag = 0;  /* 017D3E CLM */
    /* 017D3F CMP */
    if (A != 0x0005) goto L_017D58;  /* 017D42 BNE */
    mflag = 1;  /* 017D44 SEM */
    wr8(ABS(0x019A), rd8(ABS(0x019A)) & ~0x80);  /* 017D45 CLB */
    Y = rd16(ABS(0x019A));  /* 017D49 LDY */
    mflag = 0;  /* 017D4C CLM */
    S -= 3; F_016F15();  /* 017D4D JSRL */
    mflag = 1;  /* 017D51 SEM */
    wr8(DP(0x04), 0x00);  /* 017D52 LDM */
    goto L_017D58;  /* 017D55 JMP */
L_017D58: TICK();
    mflag = 0;  /* 017D58 CLM */
    X = rd16(ABS(0x021C));  /* 017D59 LDX */
    if (X != 0) goto L_017D97;  /* 017D5C BNE */
    A = rd16(DP(0x04));  /* 017D5E LDA */
    mflag = 1;  /* 017D60 SEM */
    wr8(ABS(0x01AB), (uint8_t)A);  /* 017D61 STA */
    mflag = 0;  /* 017D64 CLM */
    X = rd16(ABS(0x0197));  /* 017D65 LDX */
    if (X == 0) goto L_017D97;  /* 017D68 BEQ */
    A = rd16(DP(0x04));  /* 017D6A LDA */
    mflag = 1;  /* 017D6C SEM */
    /* 017D6D CMP */
    if ((A & 0xFF) != 0x01) goto L_017D76;  /* 017D6F BNE */
    SETLO(A, 0xA5);  /* 017D71 LDA */
    goto L_017D78;  /* 017D73 JMP */
L_017D76: TICK();
    SETLO(A, 0x5A);  /* 017D76 LDA */
L_017D78: TICK();
    wr8(DP(0x04), (uint8_t)A);  /* 017D78 STA */
    wr8(DP(0x0B), 0x01);  /* 017D7A LDM */
    mflag = 0;  /* 017D7D CLM */
    A = D;  /* 017D7E TDA */
    C = 0;  /* 017D7F CLC */
    A = adc16(A, 0x0004);  /* 017D80 ADC */
    B = 0x0000;  /* 017D83 LDA */
    wr16(DP(0x0C), A);  /* 017D87 STA */
    wr16(DP(0x0E), B);  /* 017D89 STA */
    wr16(DP(0x0F), 0x055D);  /* 017D8C LDM */
    Y = 0x00A0;  /* 017D90 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 017D93 JSRL */
L_017D97: TICK();
    Y = pull16();  /* 017D97 PLY */
    D = pull16();  /* 017D98 PLD */
    S += 3; return;  /* 017D99 RTL */
}

/* 017D9A  callers: 01C995 */
void F_017D9A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017D9A LDA */
    push16(D);  /* 017D9C PHD */
    D = A;  /* 017D9D TAD */
    C = 0;  /* 017D9E CLC */
    A = adc16(A, 0x0006);  /* 017D9F ADC */
    push16(A);  /* 017DA2 PHA */
    if ((rd16(ABS(0x0246)) & 0x0040) == 0x0040) goto L_017DAC;  /* 017DA3 BBS */
    goto L_017EA0;  /* 017DA9 JMP */
L_017DAC: TICK();
    A = rd16(DP(0x00));  /* 017DAC LDA */
    wr16(ABS(0x019E), A);  /* 017DAE STA */
    if ((rd16(ABS(0x019E)) & 0x0004) == 0) goto L_017E11;  /* 017DB1 BBC */
    mflag = 1;  /* 017DB7 SEM */
    SETLO(A, rd8(ABS(0x019B)));  /* 017DB8 LDA */
    if ((A & 0xFF) == 0) goto L_017DD3;  /* 017DBB BEQ */
    t = (rd8(ABS(0x019B)) - 1) & 0xFF; wr8(ABS(0x019B), t);  /* 017DBD DEC */
    mflag = 0;  /* 017DC0 CLM */
    wr16(DP(0x02), 0x00AA);  /* 017DC1 LDM */
    wr16(DP(0x04), 0x0043);  /* 017DC5 LDM */
    Y = 0x0002;  /* 017DC9 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017DCC JSRL */
    goto L_017E11;  /* 017DD0 JMP */
L_017DD3: TICK();
    mflag = 0;  /* 017DD3 CLM */
    A = rd16(ABS(0x01AA));  /* 017DD4 LDA */
    mflag = 1;  /* 017DD7 SEM */
    /* 017DD8 CMP */
    if ((A & 0xFF) >= 0x08) goto L_017DFA;  /* 017DDA BCS */
    SETLO(A, rd8(ABS(0x01AA)));  /* 017DDC LDA */
    t = (rd8(ABS(0x01AA)) + 1) & 0xFF; wr8(ABS(0x01AA), t);  /* 017DDF INC */
    mflag = 0;  /* 017DE2 CLM */
    A &= 0x00FF;  /* 017DE3 AND */
    X = A;  /* 017DE6 TAX */
    A = rd16((ABS(0x01A2) + X));  /* 017DE7 LDA */
    A &= 0x00FF;  /* 017DEA AND */
    wr16(DP(0x02), A);  /* 017DED STA */
    wr16(DP(0x04), 0x0043);  /* 017DEF LDM */
    Y = 0x0002;  /* 017DF3 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017DF6 JSRL */
L_017DFA: TICK();
    SETLO(A, rd8(ABS(0x01AA)));  /* 017DFA LDA */
    /* 017DFD CMP */
    if ((A & 0xFF) != 0x03) goto L_017E11;  /* 017DFF BNE */
    mflag = 0;  /* 017E01 CLM */
    wr16(DP(0x02), 0x000B);  /* 017E02 LDM */
    wr16(DP(0x04), 0x0040);  /* 017E06 LDM */
    Y = 0x0002;  /* 017E0A LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017E0D JSRL */
L_017E11: TICK();
    mflag = 0;  /* 017E11 CLM */
    if ((rd16(ABS(0x019E)) & 0x0001) == 0) goto L_017E66;  /* 017E12 BBC */
    A = rd16(ABS(0x01AA));  /* 017E18 LDA */
    mflag = 1;  /* 017E1B SEM */
    /* 017E1C CMP */
    if ((A & 0xFF) >= 0x06) goto L_017E44;  /* 017E1E BCS */
    mflag = 0;  /* 017E20 CLM */
    wr16(DP(0x02), 0x0000);  /* 017E21 LDM */
    wr16(DP(0x04), 0x0042);  /* 017E25 LDM */
    Y = 0x0002;  /* 017E29 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017E2C JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 017E30 STA */
    SETLO(A, rd8(ABS(0x01AA)));  /* 017E32 LDA */
    mflag = 0;  /* 017E35 CLM */
    A &= 0x00FF;  /* 017E36 AND */
    C = 0;  /* 017E39 CLC */
    A = adc16(A, 0x01A4);  /* 017E3A ADC */
    X = A;  /* 017E3D TAX */
    A = rd16(DP(0x02));  /* 017E3E LDA */
    mflag = 1;  /* 017E40 SEM */
    wr8((ABS(0x0000) + X), (uint8_t)A);  /* 017E41 STA */
L_017E44: TICK();
    t = (rd8(ABS(0x01AA)) + 1) & 0xFF; wr8(ABS(0x01AA), t);  /* 017E44 INC */
    SETLO(A, rd8(ABS(0x01AA)));  /* 017E47 LDA */
    /* 017E4A CMP */
    if ((A & 0xFF) != 0x08) goto L_017E66;  /* 017E4C BNE */
    wr8(ABS(0x019D), 0x01);  /* 017E4E LDM */
    mflag = 0;  /* 017E52 CLM */
    wr16(DP(0x02), 0x001A);  /* 017E53 LDM */
    wr16(DP(0x04), 0x0040);  /* 017E57 LDM */
    Y = 0x0002;  /* 017E5B LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017E5E JSRL */
    wr8(ABS(0x01AA), 0x00);  /* 017E62 LDM */
L_017E66: TICK();
    mflag = 0;  /* 017E66 CLM */
    if ((rd16(ABS(0x019E)) & 0x0008) == 0) goto L_017E90;  /* 017E67 BBC */
    A = rd16(ABS(0x01A9));  /* 017E6D LDA */
    mflag = 1;  /* 017E70 SEM */
    SETLO(A, lsr8(A));  /* 017E71 LSR */
    SETLO(A, lsr8(A));  /* 017E72 LSR */
    SETLO(A, lsr8(A));  /* 017E73 LSR */
    SETLO(A, lsr8(A));  /* 017E74 LSR */
    SETLO(A, lsr8(A));  /* 017E75 LSR */
    mflag = 0;  /* 017E76 CLM */
    A &= 0x0007;  /* 017E77 AND */
    A += 1;  /* 017E7A INC */
    A &= 0x0003;  /* 017E7B AND */
    mflag = 1;  /* 017E7E SEM */
    SETLO(A, A & 0x07);  /* 017E7F AND */
    SETLO(A, asl8(A));  /* 017E81 ASL */
    SETLO(A, asl8(A));  /* 017E82 ASL */
    SETLO(A, asl8(A));  /* 017E83 ASL */
    SETLO(A, asl8(A));  /* 017E84 ASL */
    SETLO(A, asl8(A));  /* 017E85 ASL */
    wr8(ABS(0x01A9), rd8(ABS(0x01A9)) & ~0xE0);  /* 017E86 CLB */
    SETLO(A, A | rd8(ABS(0x01A9)));  /* 017E8A ORA */
    wr8(ABS(0x01A9), (uint8_t)A);  /* 017E8D STA */
L_017E90: TICK();
    mflag = 0;  /* 017E90 CLM */
    if ((rd16(ABS(0x019E)) & 0x0010) == 0) goto L_017EA0;  /* 017E91 BBC */
    mflag = 1;  /* 017E97 SEM */
    wr8(ABS(0x019D), 0x00);  /* 017E98 LDM */
    wr8(ABS(0x01AA), 0x00);  /* 017E9C LDM */
L_017EA0: TICK();
    mflag = 0;  /* 017EA0 CLM */
    Y = pull16();  /* 017EA1 PLY */
    D = pull16();  /* 017EA2 PLD */
    S += 3; return;  /* 017EA3 RTL */
}

/* 017EA4  callers: 01C9C8 */
void F_017EA4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 017EA4 LDA */
    push16(D);  /* 017EA6 PHD */
    D = A;  /* 017EA7 TAD */
    C = 0;  /* 017EA8 CLC */
    A = adc16(A, 0x0006);  /* 017EA9 ADC */
    push16(A);  /* 017EAC PHA */
    mflag = 1;  /* 017EAD SEM */
    if ((rd8(ABS(0x0334)) & 0x04) == 0x04) goto L_017EB7;  /* 017EAE BBS */
    mflag = 0;  /* 017EB3 CLM */
    Y = pull16();  /* 017EB4 PLY */
    D = pull16();  /* 017EB5 PLD */
    S += 3; return;  /* 017EB6 RTL */
L_017EB7: TICK();
    if ((rd8(DP(0x00)) & 0x20) == 0) goto L_017EF3;  /* 017EB7 BBC */
    SETLO(A, rd8(ABS(0x0455)));  /* 017EBB LDA */
    /* 017EBE CMP */
    if ((A & 0xFF) != 0x02) goto L_017EEA;  /* 017EC0 BNE */
    mflag = 0;  /* 017EC2 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0004);  /* 017EC3 SEB */
    wr16(DP(0x02), 0x0004);  /* 017EC8 LDM */
    wr16(DP(0x04), 0x0040);  /* 017ECC LDM */
    Y = 0x0002;  /* 017ED0 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017ED3 JSRL */
    mflag = 0;  /* 017ED7 CLM */
    wr16(DP(0x02), 0x0038);  /* 017ED8 LDM */
    wr16(DP(0x04), 0x0040);  /* 017EDC LDM */
    Y = 0x0002;  /* 017EE0 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017EE3 JSRL */
    goto L_017EF3;  /* 017EE7 JMP */
L_017EEA: TICK();
    mflag = 1;  /* 017EEA SEM */
    wr8(ABS(0x04EC), 0x01);  /* 017EEB LDM */
    wr8(ABS(0x04C6), 0x00);  /* 017EEF LDM */
L_017EF3: TICK();
    if ((rd8(DP(0x00)) & 0x10) == 0) goto L_017F08;  /* 017EF3 BBC */
    mflag = 0;  /* 017EF7 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0100);  /* 017EF8 CLB */
    mflag = 1;  /* 017EFD SEM */
    wr8(ABS(0x04C6), 0x00);  /* 017EFE LDM */
    mflag = 0;  /* 017F02 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0200);  /* 017F03 CLB */
L_017F08: TICK();
    mflag = 1;  /* 017F08 SEM */
    if ((rd8(DP(0x00)) & 0x08) == 0) goto L_017F55;  /* 017F09 BBC */
    SETLO(A, rd8(ABS(0x04C5)));  /* 017F0D LDA */
    /* 017F10 CMP */
    if ((A & 0xFF) < 0x02) goto L_017F24;  /* 017F12 BCC */
    mflag = 0;  /* 017F14 CLM */
    wr16(DP(0x02), 0x0004);  /* 017F15 LDM */
    wr16(DP(0x04), 0x0040);  /* 017F19 LDM */
    Y = 0x0002;  /* 017F1D LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017F20 JSRL */
L_017F24: TICK();
    mflag = 0;  /* 017F24 CLM */
    if ((rd16(ABS(0x0248)) & 0x0002) == 0) goto L_017F3D;  /* 017F25 BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0020);  /* 017F2B CLB */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0002);  /* 017F30 CLB */
    mflag = 1;  /* 017F35 SEM */
    wr8(ABS(0x04E6), 0x00);  /* 017F36 LDM */
    goto L_017F55;  /* 017F3A JMP */
L_017F3D: TICK();
    mflag = 0;  /* 017F3D CLM */
    X = rd16(ABS(TB2));  /* 017F3E LDX */
    wr16(ABS(0x04BB), X);  /* 017F41 STX */
    mflag = 1;  /* 017F44 SEM */
    wr8(ABS(0x04BA), 0x06);  /* 017F45 LDM */
    wr8(ABS(0x04E5), 0x01);  /* 017F49 LDM */
    wr8(ABS(0x04C5), 0x00);  /* 017F4D LDM */
    wr8(ABS(0x04E6), 0x00);  /* 017F51 LDM */
L_017F55: TICK();
    if ((rd8(DP(0x00)) & 0x04) == 0) goto L_017FB0;  /* 017F55 BBC */
    mflag = 0;  /* 017F59 CLM */
    if ((rd16(ABS(0x0248)) & 0x0002) == 0) goto L_017F89;  /* 017F5A BBC */
    A = rd16(ABS(0x04C5));  /* 017F60 LDA */
    mflag = 1;  /* 017F63 SEM */
    /* 017F64 CMP */
    if ((A & 0xFF) >= 0x05) goto L_017F86;  /* 017F66 BCS */
    SETLO(A, rd8(ABS(0x04C5)));  /* 017F68 LDA */
    mflag = 0;  /* 017F6B CLM */
    A &= 0x00FF;  /* 017F6C AND */
    X = A;  /* 017F6F TAX */
    A = rd16((ABS(0x04E0) + X));  /* 017F70 LDA */
    A &= 0x00FF;  /* 017F73 AND */
    wr16(DP(0x02), A);  /* 017F76 STA */
    wr16(DP(0x04), 0x0043);  /* 017F78 LDM */
    Y = 0x0002;  /* 017F7C LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017F7F JSRL */
    t = (rd8(ABS(0x04C5)) + 1) & 0xFF; wr8(ABS(0x04C5), t);  /* 017F83 INC */
L_017F86: TICK();
    goto L_017FB0;  /* 017F86 JMP */
L_017F89: TICK();
    mflag = 0;  /* 017F89 CLM */
    A = rd16(ABS(0x04C5));  /* 017F8A LDA */
    mflag = 1;  /* 017F8D SEM */
    /* 017F8E CMP */
    if ((A & 0xFF) >= 0x0D) goto L_017FB0;  /* 017F90 BCS */
    SETLO(A, rd8(ABS(0x04C5)));  /* 017F92 LDA */
    mflag = 0;  /* 017F95 CLM */
    A &= 0x00FF;  /* 017F96 AND */
    X = A;  /* 017F99 TAX */
    A = rd16((ABS(0x04D3) + X));  /* 017F9A LDA */
    A &= 0x00FF;  /* 017F9D AND */
    wr16(DP(0x02), A);  /* 017FA0 STA */
    wr16(DP(0x04), 0x0043);  /* 017FA2 LDM */
    Y = 0x0002;  /* 017FA6 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017FA9 JSRL */
    t = (rd8(ABS(0x04C5)) + 1) & 0xFF; wr8(ABS(0x04C5), t);  /* 017FAD INC */
L_017FB0: TICK();
    if ((rd8(DP(0x00)) & 0x01) == 0x01) goto L_017FB7;  /* 017FB0 BBS */
    goto L_0181AD;  /* 017FB4 JMP */
L_017FB7: TICK();
    SETLO(A, rd8(ABS(0x0455)));  /* 017FB7 LDA */
    /* 017FBA CMP */
    if ((A & 0xFF) != 0x02) goto L_017FC1;  /* 017FBC BNE */
    goto L_01805D;  /* 017FBE JMP */
L_017FC1: TICK();
    SETLO(A, rd8(ABS(0x04EC)));  /* 017FC1 LDA */
    if ((A & 0xFF) != 0) goto L_017FC9;  /* 017FC4 BNE */
    goto L_01805D;  /* 017FC6 JMP */
L_017FC9: TICK();
    mflag = 0;  /* 017FC9 CLM */
    wr16(DP(0x02), 0x0000);  /* 017FCA LDM */
    wr16(DP(0x04), 0x0042);  /* 017FCE LDM */
    Y = 0x0002;  /* 017FD2 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 017FD5 JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 017FD9 STA */
    SETLO(A, rd8(ABS(0x04C6)));  /* 017FDB LDA */
    mflag = 0;  /* 017FDE CLM */
    A &= 0x00FF;  /* 017FDF AND */
    X = A;  /* 017FE2 TAX */
    A = rd16(DP(0x02));  /* 017FE3 LDA */
    mflag = 1;  /* 017FE5 SEM */
    wr8((ABS(0x04C8) + X), (uint8_t)A);  /* 017FE6 STA */
    t = (rd8(ABS(0x04C6)) + 1) & 0xFF; wr8(ABS(0x04C6), t);  /* 017FE9 INC */
    SETLO(A, rd8(ABS(0x04C6)));  /* 017FEC LDA */
    /* 017FEF CMP */
    if ((A & 0xFF) != 0x02) goto L_018059;  /* 017FF1 BNE */
    wr8(ABS(0x04C6), 0x00);  /* 017FF3 LDM */
    wr8(ABS(0x04EC), 0x00);  /* 017FF7 LDM */
    SETLO(A, rd8(ABS(0x04C8)));  /* 017FFB LDA */
    mflag = 0;  /* 017FFE CLM */
    A &= 0x00FF;  /* 017FFF AND */
    A = rla16(A, 0x0008);  /* 018002 RLA */
    A &= 0xFF00;  /* 018006 AND */
    wr16(DP(0x02), A);  /* 018009 STA */
    A = rd16(ABS(0x04C9));  /* 01800B LDA */
    A &= 0x00FF;  /* 01800E AND */
    C = 0;  /* 018011 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 018012 ADC */
    wr16(ABS(0x04CF), A);  /* 018014 STA */
    Y = A;  /* 018017 TAY */
    A = D;  /* 018018 TDA */
    A += 1;  /* 018019 INC */
    A += 1;  /* 01801A INC */
    push16(A);  /* 01801B PHA */
    S -= 3; F_01848E();  /* 01801C JSRL */
    Y = pull16();  /* 018020 PLY */
    /* 018021 CMP */
    if ((A & 0xFF) != 0x00) goto L_018039;  /* 018023 BNE */
    mflag = 0;  /* 018025 CLM */
    X = rd16(ABS(0x04CF));  /* 018026 LDX */
    if (X == 0) goto L_018039;  /* 018029 BEQ */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x1000);  /* 01802B SEB */
    A = D;  /* 018030 TDA */
    A += 1;  /* 018031 INC */
    A += 1;  /* 018032 INC */
    push16(A);  /* 018033 PHA */
    S -= 3; F_0189CB();  /* 018034 JSRL */
    Y = pull16();  /* 018038 PLY */
L_018039: TICK();
    mflag = 0;  /* 018039 CLM */
    wr16(DP(0x02), 0x0004);  /* 01803A LDM */
    wr16(DP(0x04), 0x0040);  /* 01803E LDM */
    Y = 0x0002;  /* 018042 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018045 JSRL */
    mflag = 0;  /* 018049 CLM */
    wr16(DP(0x02), 0x0038);  /* 01804A LDM */
    wr16(DP(0x04), 0x0040);  /* 01804E LDM */
    Y = 0x0002;  /* 018052 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018055 JSRL */
L_018059: TICK();
    mflag = 0;  /* 018059 CLM */
    Y = pull16();  /* 01805A PLY */
    D = pull16();  /* 01805B PLD */
    S += 3; return;  /* 01805C RTL */
L_01805D: TICK();
    mflag = 0;  /* 01805D CLM */
    wr16(DP(0x02), 0x0000);  /* 01805E LDM */
    wr16(DP(0x04), 0x0042);  /* 018062 LDM */
    Y = 0x0002;  /* 018066 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018069 JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 01806D STA */
    SETLO(A, rd8(ABS(0x04C6)));  /* 01806F LDA */
    mflag = 0;  /* 018072 CLM */
    A &= 0x00FF;  /* 018073 AND */
    X = A;  /* 018076 TAX */
    A = rd16(DP(0x02));  /* 018077 LDA */
    mflag = 1;  /* 018079 SEM */
    wr8((ABS(0x04C8) + X), (uint8_t)A);  /* 01807A STA */
    SETLO(A, rd8(ABS(0x04C6)));  /* 01807D LDA */
    /* 018080 CMP */
    if ((A & 0xFF) < 0x08) goto L_0180A4;  /* 018082 BCC */
    mflag = 0;  /* 018084 CLM */
    wr16(DP(0x02), 0x0004);  /* 018085 LDM */
    wr16(DP(0x04), 0x0040);  /* 018089 LDM */
    Y = 0x0002;  /* 01808D LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018090 JSRL */
    mflag = 0;  /* 018094 CLM */
    wr16(DP(0x02), 0x0038);  /* 018095 LDM */
    wr16(DP(0x04), 0x0040);  /* 018099 LDM */
    Y = 0x0002;  /* 01809D LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0180A0 JSRL */
L_0180A4: TICK();
    mflag = 0;  /* 0180A4 CLM */
    if ((rd16(ABS(0x0248)) & 0x0200) == 0) goto L_0180C9;  /* 0180A5 BBC */
    A = rd16(ABS(0x04C6));  /* 0180AB LDA */
    mflag = 1;  /* 0180AE SEM */
    /* 0180AF CMP */
    if ((A & 0xFF) != 0x04) goto L_0180C9;  /* 0180B1 BNE */
    SETLO(A, rd8(ABS(0x04CB)));  /* 0180B3 LDA */
    /* 0180B6 CMP */
    if ((A & 0xFF) != 0xC4) goto L_0180C9;  /* 0180B8 BNE */
    SETLO(A, rd8(ABS(0x04CC)));  /* 0180BA LDA */
    /* 0180BD CMP */
    if ((A & 0xFF) != 0xD7) goto L_0180C9;  /* 0180BF BNE */
    wr8(ABS(0x04C6), 0x00);  /* 0180C1 LDM */
    mflag = 0;  /* 0180C5 CLM */
    Y = pull16();  /* 0180C6 PLY */
    D = pull16();  /* 0180C7 PLD */
    S += 3; return;  /* 0180C8 RTL */
L_0180C9: TICK();
    mflag = 1;  /* 0180C9 SEM */
    t = (rd8(ABS(0x04C6)) + 1) & 0xFF; wr8(ABS(0x04C6), t);  /* 0180CA INC */
    SETLO(A, rd8(ABS(0x04C6)));  /* 0180CD LDA */
    /* 0180D0 CMP */
    if ((A & 0xFF) == 0x02) goto L_0180D7;  /* 0180D2 BEQ */
    goto L_018165;  /* 0180D4 JMP */
L_0180D7: TICK();
    mflag = 0;  /* 0180D7 CLM */
    A = D;  /* 0180D8 TDA */
    A += 1;  /* 0180D9 INC */
    A += 1;  /* 0180DA INC */
    push16(A);  /* 0180DB PHA */
    S -= 3; F_0189AD();  /* 0180DC JSRL */
    Y = pull16();  /* 0180E0 PLY */
    mflag = 1;  /* 0180E1 SEM */
    if ((rd8(ABS(0x04C9)) & 0x01) == 0) goto L_0180F9;  /* 0180E2 BBC */
    mflag = 0;  /* 0180E7 CLM */
    A = D;  /* 0180E8 TDA */
    A += 1;  /* 0180E9 INC */
    A += 1;  /* 0180EA INC */
    push16(A);  /* 0180EB PHA */
    S -= 3; F_0189E9();  /* 0180EC JSRL */
    Y = pull16();  /* 0180F0 PLY */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0200);  /* 0180F1 SEB */
    Y = pull16();  /* 0180F6 PLY */
    D = pull16();  /* 0180F7 PLD */
    S += 3; return;  /* 0180F8 RTL */
L_0180F9: TICK();
    SETLO(A, rd8(ABS(0x04C8)));  /* 0180F9 LDA */
    mflag = 0;  /* 0180FC CLM */
    A &= 0x00FF;  /* 0180FD AND */
    A = rla16(A, 0x0008);  /* 018100 RLA */
    A &= 0xFF00;  /* 018104 AND */
    wr16(DP(0x02), A);  /* 018107 STA */
    A = rd16(ABS(0x04C9));  /* 018109 LDA */
    A &= 0x00FF;  /* 01810C AND */
    C = 0;  /* 01810F CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 018110 ADC */
    wr16(ABS(0x04CF), A);  /* 018112 STA */
    Y = A;  /* 018115 TAY */
    A = D;  /* 018116 TDA */
    A += 1;  /* 018117 INC */
    A += 1;  /* 018118 INC */
    push16(A);  /* 018119 PHA */
    S -= 3; F_01848E();  /* 01811A JSRL */
    Y = pull16();  /* 01811E PLY */
    /* 01811F CMP */
    if ((A & 0xFF) != 0x00) goto L_018153;  /* 018121 BNE */
    mflag = 0;  /* 018123 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0100);  /* 018124 SEB */
    mflag = 1;  /* 018129 SEM */
    if ((rd8(ABS(0x04C8)) & 0x80) == 0x80) goto L_018141;  /* 01812A BBS */
    mflag = 0;  /* 01812F CLM */
    A = D;  /* 018130 TDA */
    A += 1;  /* 018131 INC */
    A += 1;  /* 018132 INC */
    push16(A);  /* 018133 PHA */
    S -= 3; F_0189E9();  /* 018134 JSRL */
    Y = pull16();  /* 018138 PLY */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0080);  /* 018139 SEB */
    Y = pull16();  /* 01813E PLY */
    D = pull16();  /* 01813F PLD */
    S += 3; return;  /* 018140 RTL */
L_018141: TICK();
    SETLO(A, rd8(ABS(0x04C6)));  /* 018141 LDA */
    C = 0;  /* 018144 CLC */
    SETLO(A, adc8(A, 0x05));  /* 018145 ADC */
    wr8(ABS(0x04C6), (uint8_t)A);  /* 018147 STA */
    mflag = 0;  /* 01814A CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0080);  /* 01814B CLB */
    goto L_018165;  /* 018150 JMP */
L_018153: TICK();
    mflag = 0;  /* 018153 CLM */
    A = D;  /* 018154 TDA */
    A += 1;  /* 018155 INC */
    A += 1;  /* 018156 INC */
    push16(A);  /* 018157 PHA */
    S -= 3; F_0189E9();  /* 018158 JSRL */
    Y = pull16();  /* 01815C PLY */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0200);  /* 01815D SEB */
    Y = pull16();  /* 018162 PLY */
    D = pull16();  /* 018163 PLD */
    S += 3; return;  /* 018164 RTL */
L_018165: TICK();
    mflag = 0;  /* 018165 CLM */
    A = rd16(ABS(0x04C6));  /* 018166 LDA */
    mflag = 1;  /* 018169 SEM */
    /* 01816A CMP */
    if ((A & 0xFF) != 0x07) goto L_0181AD;  /* 01816C BNE */
    mflag = 0;  /* 01816E CLM */
    if ((rd16(ABS(0x0248)) & 0x0080) == 0) goto L_018183;  /* 01816F BBC */
    A = D;  /* 018175 TDA */
    A += 1;  /* 018176 INC */
    A += 1;  /* 018177 INC */
    push16(A);  /* 018178 PHA */
    S -= 3; F_0189AD();  /* 018179 JSRL */
    Y = pull16();  /* 01817D PLY */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0080);  /* 01817E CLB */
L_018183: TICK();
    if ((rd16(ABS(0x0248)) & 0x0100) == 0) goto L_01818E;  /* 018183 BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0001);  /* 018189 SEB */
L_01818E: TICK();
    wr16(DP(0x02), 0x0004);  /* 01818E LDM */
    wr16(DP(0x04), 0x0040);  /* 018192 LDM */
    Y = 0x0002;  /* 018196 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018199 JSRL */
    mflag = 0;  /* 01819D CLM */
    wr16(DP(0x02), 0x0038);  /* 01819E LDM */
    wr16(DP(0x04), 0x0040);  /* 0181A2 LDM */
    Y = 0x0002;  /* 0181A6 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0181A9 JSRL */
L_0181AD: TICK();
    mflag = 0;  /* 0181AD CLM */
    Y = pull16();  /* 0181AE PLY */
    D = pull16();  /* 0181AF PLD */
    S += 3; return;  /* 0181B0 RTL */
}

