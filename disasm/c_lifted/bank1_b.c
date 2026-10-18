/* bank1_b.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 0181B1  callers: 01C9FE */
void F_0181B1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0181B1 LDA */
    push16(D);  /* 0181B3 PHD */
    D = A;  /* 0181B4 TAD */
    C = 0;  /* 0181B5 CLC */
    A = adc16(A, 0x000E);  /* 0181B6 ADC */
    push16(A);  /* 0181B9 PHA */
    mflag = 1;  /* 0181BA SEM */
    wr8(DP(0x02), 0x00);  /* 0181BB LDM */
    wr8(DP(0x03), 0x00);  /* 0181BE LDM */
    if ((rd8(DP(0x00)) & 0x40) == 0) goto L_0181E3;  /* 0181C1 BBC */
    wr8(DP(0x03), 0x01);  /* 0181C5 LDM */
    SETLO(A, rd8(ABS(0x0450)));  /* 0181C8 LDA */
    mflag = 0;  /* 0181CB CLM */
    A &= 0x00FF;  /* 0181CC AND */
    wr16(DP(0x08), A);  /* 0181CF STA */
    wr16(DP(0x0A), 0x0000);  /* 0181D1 LDM */
    wr16(DP(0x0C), 0x0063);  /* 0181D5 LDM */
    Y = 0x0003;  /* 0181D9 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 0181DC JSRL */
    t = (rd8(ABS(0x0450)) - 1) & 0xFF; wr8(ABS(0x0450), t);  /* 0181E0 DEC */
L_0181E3: TICK();
    if ((rd8(DP(0x00)) & 0x20) == 0x20) goto L_0181EA;  /* 0181E3 BBS */
    goto L_018320;  /* 0181E7 JMP */
L_0181EA: TICK();
    wr8(DP(0x02), 0x01);  /* 0181EA LDM */
    SETLO(A, rd8(ABS(0x0455)));  /* 0181ED LDA */
    /* 0181F0 CMP */
    if ((A & 0xFF) != 0x02) goto L_018217;  /* 0181F2 BNE */
    mflag = 0;  /* 0181F4 CLM */
    if ((rd16(ABS(0x0248)) & 0x0400) == 0) goto L_018217;  /* 0181F5 BBC */
    A = rd16(ABS(0x0453));  /* 0181FB LDA */
    mflag = 1;  /* 0181FE SEM */
    /* 0181FF CMP */
    if ((A & 0xFF) >= 0x0B) goto L_01820A;  /* 018201 BCS */
    SETLO(A, rd8(ABS(0x0453)));  /* 018203 LDA */
    /* 018206 CMP */
    if ((A & 0xFF) >= 0x01) goto L_018217;  /* 018208 BCS */
L_01820A: TICK();
    SETLO(A, rd8(ABS(0x04BE)));  /* 01820A LDA */
    mpy8(rd8(ABS(0x04BD)));  /* 01820D MPY */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018211 STA */
    goto L_01821D;  /* 018214 JMP */
L_018217: TICK();
    mflag = 0;  /* 018217 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0400);  /* 018218 CLB */
L_01821D: TICK();
    mflag = 0;  /* 01821D CLM */
    A = rd16(ABS(0x0451));  /* 01821E LDA */
    A &= 0x00FF;  /* 018221 AND */
    wr16(DP(0x08), A);  /* 018224 STA */
    wr16(DP(0x0A), 0x0000);  /* 018226 LDM */
    wr16(DP(0x0C), 0x0065);  /* 01822A LDM */
    Y = 0x0003;  /* 01822E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018231 JSRL */
    t = (rd8(ABS(0x0451)) + 1) & 0xFF; wr8(ABS(0x0451), t);  /* 018235 INC */
    t = (rd8(ABS(0x04C4)) - 1) & 0xFF; wr8(ABS(0x04C4), t);  /* 018238 DEC */
    SETLO(A, rd8(ABS(0x04C4)));  /* 01823B LDA */
    if ((A & 0xFF) == 0) goto L_018243;  /* 01823E BEQ */
    goto L_018320;  /* 018240 JMP */
L_018243: TICK();
    mflag = 0;  /* 018243 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0010);  /* 018244 SEB */
    A = rd16(ABS(0x0452));  /* 018249 LDA */
    mflag = 1;  /* 01824C SEM */
    wr8(ABS(0x0450), (uint8_t)A);  /* 01824D STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 018250 LDA */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018253 STA */
    SETLO(A, rd8(ABS(0x04BD)));  /* 018256 LDA */
    wr8(ABS(0x04C4), (uint8_t)A);  /* 018259 STA */
    SETLO(A, rd8(ABS(0x0228)));  /* 01825C LDA */
    /* 01825F CMP */
    if ((A & 0xFF) == 0x08) goto L_01826A;  /* 018261 BEQ */
    /* 018263 CMP */
    if ((A & 0xFF) == 0x09) goto L_01826A;  /* 018265 BEQ */
    goto L_018320;  /* 018267 JMP */
L_01826A: TICK();
    mflag = 0;  /* 01826A CLM */
    A = D;  /* 01826B TDA */
    C = 0;  /* 01826C CLC */
    A = adc16(A, 0x0008);  /* 01826D ADC */
    push16(A);  /* 018270 PHA */
    S -= 3; F_018700();  /* 018271 JSRL */
    Y = pull16();  /* 018275 PLY */
    A = rd16(ABS(0x04C1));  /* 018276 LDA */
    mflag = 1;  /* 018279 SEM */
    t = rd8(ABS(0x04E8));  /* 01827A CMP */
    if ((A & 0xFF) != t) goto L_018283;  /* 01827D BNE */
    wr8(ABS(0x04C1), 0x00);  /* 01827F LDM */
L_018283: TICK();
    mflag = 0;  /* 018283 CLM */
    if ((rd16(ABS(0x0248)) & 0x0040) == 0x0040) goto L_01828D;  /* 018284 BBS */
    goto L_018320;  /* 01828A JMP */
L_01828D: TICK();
    A = rd16(ABS(0x04C1));  /* 01828D LDA */
    mflag = 1;  /* 018290 SEM */
    t = rd8(ABS(0x04EA));  /* 018291 CMP */
    if ((A & 0xFF) == t) goto L_018299;  /* 018294 BEQ */
    goto L_018320;  /* 018296 JMP */
L_018299: TICK();
    mflag = 0;  /* 018299 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0020);  /* 01829A SEB */
    mflag = 1;  /* 01829F SEM */
    wr8(ABS(0x04E6), 0x01);  /* 0182A0 LDM */
    mflag = 0;  /* 0182A4 CLM */
    wr16(DP(0x08), 0x0009);  /* 0182A5 LDM */
    wr16(DP(0x0A), 0x0040);  /* 0182A9 LDM */
    Y = 0x0002;  /* 0182AD LDY */
    A = D;  /* 0182B0 TDA */
    C = 0;  /* 0182B1 CLC */
    A = adc16(A, 0x000C);  /* 0182B2 ADC */
    push16(A);  /* 0182B5 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0182B6 JSRL */
    Y = pull16();  /* 0182BA PLY */
    SETLO(A, rd8(ABS(0x04E0)));  /* 0182BB LDA */
    mflag = 0;  /* 0182BE CLM */
    A &= 0x00FF;  /* 0182BF AND */
    wr16(DP(0x08), A);  /* 0182C2 STA */
    wr16(DP(0x0A), 0x0043);  /* 0182C4 LDM */
    Y = 0x0002;  /* 0182C8 LDY */
    A = D;  /* 0182CB TDA */
    C = 0;  /* 0182CC CLC */
    A = adc16(A, 0x000C);  /* 0182CD ADC */
    push16(A);  /* 0182D0 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0182D1 JSRL */
    Y = pull16();  /* 0182D5 PLY */
    wr8(ABS(0x04C5), 0x01);  /* 0182D6 LDM */
    mflag = 0;  /* 0182DA CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0002);  /* 0182DB SEB */
    A = rd16(ABS(0x0456));  /* 0182E0 LDA */
    A &= 0x00FF;  /* 0182E3 AND */
    A = asl16(A);  /* 0182E6 ASL */
    A = asl16(A);  /* 0182E7 ASL */
    A = asl16(A);  /* 0182E8 ASL */
    A = asl16(A);  /* 0182E9 ASL */
    A = asl16(A);  /* 0182EA ASL */
    A = asl16(A);  /* 0182EB ASL */
    wr16(DP(0x08), A);  /* 0182EC STA */
    A = rd16(ABS(0x0454));  /* 0182EE LDA */
    A &= 0x00FF;  /* 0182F1 AND */
    A &= 0x001F;  /* 0182F4 AND */
    A = lsr16(A);  /* 0182F7 LSR */
    A = ror16(A);  /* 0182F8 ROR */
    A = ror16(A);  /* 0182F9 ROR */
    A = ror16(A);  /* 0182FA ROR */
    A = ror16(A);  /* 0182FB ROR */
    A = ror16(A);  /* 0182FC ROR */
    C = 0;  /* 0182FD CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 0182FE ADC */
    Y = A;  /* 018300 TAY */
    wr16(DP(0x06), Y);  /* 018301 STY */
    A = D;  /* 018303 TDA */
    C = 0;  /* 018304 CLC */
    A = adc16(A, 0x0008);  /* 018305 ADC */
    push16(A);  /* 018308 PHA */
    S -= 3; F_01844E();  /* 018309 JSRL */
    Y = pull16();  /* 01830D PLY */
    wr16(DP(0x04), A);  /* 01830E STA */
    A = rla16(A, 0x0008);  /* 018310 RLA */
    A &= 0x00FF;  /* 018314 AND */
    mflag = 1;  /* 018317 SEM */
    wr8(ABS(0x04E3), (uint8_t)A);  /* 018318 STA */
    SETLO(A, rd8(DP(0x04)));  /* 01831B LDA */
    wr8(ABS(0x04E4), (uint8_t)A);  /* 01831D STA */
L_018320: TICK();
    mflag = 1;  /* 018320 SEM */
    SETLO(A, rd8(DP(0x02)));  /* 018321 LDA */
    if ((A & 0xFF) == 0) goto L_018329;  /* 018323 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 018325 LDA */
    if ((A & 0xFF) != 0) goto L_01833C;  /* 018327 BNE */
L_018329: TICK();
    SETLO(A, rd8(ABS(0x0455)));  /* 018329 LDA */
    /* 01832C CMP */
    if ((A & 0xFF) != 0x02) goto L_01833C;  /* 01832E BNE */
    mflag = 0;  /* 018330 CLM */
    A = D;  /* 018331 TDA */
    C = 0;  /* 018332 CLC */
    A = adc16(A, 0x0008);  /* 018333 ADC */
    push16(A);  /* 018336 PHA */
    S -= 3; F_01906F();  /* 018337 JSRL */
    Y = pull16();  /* 01833B PLY */
L_01833C: TICK();
    mflag = 0;  /* 01833C CLM */
    Y = pull16();  /* 01833D PLY */
    D = pull16();  /* 01833E PLD */
    S += 3; return;  /* 01833F RTL */
}

/* 018340  callers: 01866E */
void F_018340(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018340 LDA */
    push16(D);  /* 018342 PHD */
    D = A;  /* 018343 TAD */
    C = 0;  /* 018344 CLC */
    A = adc16(A, 0x0006);  /* 018345 ADC */
    push16(A);  /* 018348 PHA */
    wr16(DP(0x04), 0x0000);  /* 018349 LDM */
    wr16(DP(0x02), 0x0001);  /* 01834D LDM */
L_018351: TICK();
    X = rd16(DP(0x02));  /* 018351 LDX */
    /* 018353 CPX */
    if (X >= 0x0006) goto L_018397;  /* 018356 BCS */
    wr16(DP(0x06), 0x0050);  /* 018358 LDM */
    wr16(DP(0x07), 0x02E2);  /* 01835C LDM */
    wr16(DP(0x09), 0x0000);  /* 018360 LDM */
    A = X;  /* 018364 TXA */
    mpy16(0x0050);  /* 018365 MPY */
    C = 0;  /* 018369 CLC */
    A = adc16(A, 0x0600);  /* 01836A ADC */
    wr16(DP(0x0A), A);  /* 01836D STA */
    Y = 0x00A0;  /* 01836F LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 018372 JSRL */
    wr16(DP(0x00), 0x0000);  /* 018376 LDM */
L_01837A: TICK();
    X = rd16(DP(0x00));  /* 01837A LDX */
    C = (X >= 0x0050);  /* 01837C CPX */
    if (C) goto L_018391;  /* 01837F BCS */
    A = rd16((ABS(0x02E2) + X));  /* 018381 LDA */
    A &= 0x00FF;  /* 018384 AND */
    A = adc16(A, rd16(DP(0x04)));  /* 018387 ADC */
    wr16(DP(0x04), A);  /* 018389 STA */
    X += 1;  /* 01838B INX */
    wr16(DP(0x00), X);  /* 01838C STX */
    goto L_01837A;  /* 01838E JMP */
L_018391: TICK();
    mflag = 0;  /* 018391 CLM */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 018392 INC */
    goto L_018351;  /* 018394 JMP */
L_018397: TICK();
    mflag = 0;  /* 018397 CLM */
    wr16(DP(0x06), 0x0020);  /* 018398 LDM */
    wr16(DP(0x07), 0x02E2);  /* 01839C LDM */
    wr16(DP(0x09), 0x0000);  /* 0183A0 LDM */
    wr16(DP(0x0A), 0x07E0);  /* 0183A4 LDM */
    Y = 0x00A0;  /* 0183A8 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 0183AB JSRL */
    wr16(DP(0x00), 0x0000);  /* 0183AF LDM */
L_0183B3: TICK();
    X = rd16(DP(0x00));  /* 0183B3 LDX */
    C = (X >= 0x0020);  /* 0183B5 CPX */
    if (C) goto L_0183CA;  /* 0183B8 BCS */
    A = rd16((ABS(0x02E2) + X));  /* 0183BA LDA */
    A &= 0x00FF;  /* 0183BD AND */
    A = adc16(A, rd16(DP(0x04)));  /* 0183C0 ADC */
    wr16(DP(0x04), A);  /* 0183C2 STA */
    X += 1;  /* 0183C4 INX */
    wr16(DP(0x00), X);  /* 0183C5 STX */
    goto L_0183B3;  /* 0183C7 JMP */
L_0183CA: TICK();
    mflag = 0;  /* 0183CA CLM */
    wr16(DP(0x06), 0x0050);  /* 0183CB LDM */
    wr16(DP(0x07), 0x02E2);  /* 0183CF LDM */
    wr16(DP(0x09), 0x0000);  /* 0183D3 LDM */
    wr16(DP(0x0A), 0x0600);  /* 0183D7 LDM */
    Y = 0x00A0;  /* 0183DB LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 0183DE JSRL */
    wr16(DP(0x00), 0x0002);  /* 0183E2 LDM */
L_0183E6: TICK();
    X = rd16(DP(0x00));  /* 0183E6 LDX */
    C = (X >= 0x0050);  /* 0183E8 CPX */
    if (C) goto L_0183FD;  /* 0183EB BCS */
    A = rd16((ABS(0x02E2) + X));  /* 0183ED LDA */
    A &= 0x00FF;  /* 0183F0 AND */
    A = adc16(A, rd16(DP(0x04)));  /* 0183F3 ADC */
    wr16(DP(0x04), A);  /* 0183F5 STA */
    X += 1;  /* 0183F7 INX */
    wr16(DP(0x00), X);  /* 0183F8 STX */
    goto L_0183E6;  /* 0183FA JMP */
L_0183FD: TICK();
    mflag = 0;  /* 0183FD CLM */
    A = rd16(ABS(0x02E2));  /* 0183FE LDA */
    A &= 0x00FF;  /* 018401 AND */
    A = rla16(A, 0x0008);  /* 018404 RLA */
    A &= 0xFF00;  /* 018408 AND */
    wr16(DP(0x06), A);  /* 01840B STA */
    A = rd16(ABS(0x02E3));  /* 01840D LDA */
    A &= 0x00FF;  /* 018410 AND */
    C = 0;  /* 018413 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 018414 ADC */
    wr16(DP(0x06), A);  /* 018416 STA */
    A = rd16(DP(0x04));  /* 018418 LDA */
    A ^= 0xFFFF;  /* 01841A EOR */
    t = rd16(DP(0x06));  /* 01841D CMP */
    if (A != t) goto L_018428;  /* 01841F BNE */
    A = 0x0000;  /* 018421 LDA */
    mflag = 1;  /* 018424 SEM */
    Y = pull16();  /* 018425 PLY */
    D = pull16();  /* 018426 PLD */
    S += 3; return;  /* 018427 RTL */
L_018428: TICK();
    A = 0x0001;  /* 018428 LDA */
    mflag = 1;  /* 01842B SEM */
    Y = pull16();  /* 01842C PLY */
    D = pull16();  /* 01842D PLD */
    S += 3; return;  /* 01842E RTL */
}

/* 01842F  callers: 01928F */
void F_01842F(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x0228));  /* 01842F LDA */
    mflag = 1;  /* 018432 SEM */
    /* 018433 CMP */
    if ((A & 0xFF) == 0x03) goto L_018442;  /* 018435 BEQ */
    SETLO(A, rd8(ABS(0x0458)));  /* 018437 LDA */
    if ((A & 0xFF) == 0) goto L_01843F;  /* 01843A BEQ */
    t = (rd8(ABS(0x0458)) - 1) & 0xFF; wr8(ABS(0x0458), t);  /* 01843C DEC */
L_01843F: TICK();
    goto L_01844A;  /* 01843F JMP */
L_018442: TICK();
    mflag = 0;  /* 018442 CLM */
    A = rd16(ABS(0x04C3));  /* 018443 LDA */
    mflag = 1;  /* 018446 SEM */
    wr8(ABS(0x0458), (uint8_t)A);  /* 018447 STA */
L_01844A: TICK();
    SETLO(A, rd8(ABS(0x0458)));  /* 01844A LDA */
    S += 3; return;  /* 01844D RTL */
}

/* 01844E  callers: 018309, 018FF3, 019010 */
void F_01844E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01844E LDA */
    push16(D);  /* 018450 PHD */
    D = A;  /* 018451 TAD */
    wr16(DP(0x00), Y);  /* 018452 STY */
    wr16(DP(0x04), 0x8000);  /* 018454 LDM */
    mflag = 1;  /* 018458 SEM */
    wr8(DP(0x02), 0x00);  /* 018459 LDM */
    wr8(DP(0x03), 0x00);  /* 01845C LDM */
L_01845F: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01845F LDA */
    /* 018461 CMP */
    if ((A & 0xFF) >= 0x0A) goto L_018484;  /* 018463 BCS */
    mflag = 0;  /* 018465 CLM */
    A = rd16(DP(0x04));  /* 018466 LDA */
    A &= rd16(DP(0x00));  /* 018468 AND */
    if (A == 0) goto L_01847B;  /* 01846A BEQ */
    A = rd16(DP(0x03));  /* 01846C LDA */
    A &= 0x00FF;  /* 01846E AND */
    X = A;  /* 018471 TAX */
    A = rd16(DP(0x02));  /* 018472 LDA */
    mflag = 1;  /* 018474 SEM */
    SETLO(A, A ^ rd8((0x003200 + X)));  /* 018475 EOR */
    wr8(DP(0x02), (uint8_t)A);  /* 018479 STA */
L_01847B: TICK();
    mflag = 0;  /* 01847B CLM */
    wr16(DP(0x04), lsr16(rd16(DP(0x04))));  /* 01847C LSR */
    mflag = 1;  /* 01847E SEM */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01847F INC */
    goto L_01845F;  /* 018481 JMP */
L_018484: TICK();
    mflag = 0;  /* 018484 CLM */
    A = rd16(DP(0x02));  /* 018485 LDA */
    A &= 0x00FF;  /* 018487 AND */
    A |= rd16(DP(0x00));  /* 01848A ORA */
    D = pull16();  /* 01848C PLD */
    S += 3; return;  /* 01848D RTL */
}

/* 01848E  callers: 01801C, 01811A */
void F_01848E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01848E LDA */
    push16(D);  /* 018490 PHD */
    D = A;  /* 018491 TAD */
    wr16(DP(0x00), Y);  /* 018492 STY */
    A = Y;  /* 018494 TYA */
    mflag = 1;  /* 018495 SEM */
    SETLO(A, A & 0x3E);  /* 018496 AND */
    wr8(DP(0x02), (uint8_t)A);  /* 018498 STA */
    mflag = 0;  /* 01849A CLM */
    wr16(DP(0x00), rd16(DP(0x00)) & ~0x003F);  /* 01849B CLB */
    wr16(DP(0x05), 0x8000);  /* 01849F LDM */
    mflag = 1;  /* 0184A3 SEM */
    wr8(DP(0x03), 0x00);  /* 0184A4 LDM */
    wr8(DP(0x04), 0x00);  /* 0184A7 LDM */
L_0184AA: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 0184AA LDA */
    /* 0184AC CMP */
    if ((A & 0xFF) >= 0x0A) goto L_0184CF;  /* 0184AE BCS */
    mflag = 0;  /* 0184B0 CLM */
    A = rd16(DP(0x05));  /* 0184B1 LDA */
    A &= rd16(DP(0x00));  /* 0184B3 AND */
    if (A == 0) goto L_0184C6;  /* 0184B5 BEQ */
    A = rd16(DP(0x04));  /* 0184B7 LDA */
    A &= 0x00FF;  /* 0184B9 AND */
    X = A;  /* 0184BC TAX */
    A = rd16(DP(0x03));  /* 0184BD LDA */
    mflag = 1;  /* 0184BF SEM */
    SETLO(A, A ^ rd8((0x003200 + X)));  /* 0184C0 EOR */
    wr8(DP(0x03), (uint8_t)A);  /* 0184C4 STA */
L_0184C6: TICK();
    mflag = 0;  /* 0184C6 CLM */
    wr16(DP(0x05), lsr16(rd16(DP(0x05))));  /* 0184C7 LSR */
    mflag = 1;  /* 0184C9 SEM */
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 0184CA INC */
    goto L_0184AA;  /* 0184CC JMP */
L_0184CF: TICK();
    mflag = 0;  /* 0184CF CLM */
    A = rd16(DP(0x03));  /* 0184D0 LDA */
    mflag = 1;  /* 0184D2 SEM */
    t = rd8(DP(0x02));  /* 0184D3 CMP */
    if ((A & 0xFF) == t) goto L_0184DB;  /* 0184D5 BEQ */
    SETLO(A, 0x01);  /* 0184D7 LDA */
    D = pull16();  /* 0184D9 PLD */
    S += 3; return;  /* 0184DA RTL */
L_0184DB: TICK();
    SETLO(A, 0x00);  /* 0184DB LDA */
    D = pull16();  /* 0184DD PLD */
    S += 3; return;  /* 0184DE RTL */
}

/* 0184DF  callers: 01867C, 018684 */
void F_0184DF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0184DF LDA */
    push16(D);  /* 0184E1 PHD */
    D = A;  /* 0184E2 TAD */
    C = 0;  /* 0184E3 CLC */
    A = adc16(A, 0x0006);  /* 0184E4 ADC */
    push16(A);  /* 0184E7 PHA */
    if ((rd16(ABS(0x0248)) & 0x0800) == 0x0800) goto L_018529;  /* 0184E8 BBS */
    wr16(DP(0x06), 0x0006);  /* 0184EE LDM */
    A = D;  /* 0184F2 TDA */
    B = 0x0000;  /* 0184F3 LDA */
    wr16(DP(0x07), A);  /* 0184F7 STA */
    wr16(DP(0x09), B);  /* 0184F9 STA */
    wr16(DP(0x0A), 0x07F8);  /* 0184FC LDM */
    Y = 0x00A0;  /* 018500 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 018503 JSRL */
    A = rd16(DP(0x00));  /* 018507 LDA */
    mflag = 1;  /* 018509 SEM */
    wr8(ABS(0x04C3), (uint8_t)A);  /* 01850A STA */
    SETLO(A, rd8(DP(0x01)));  /* 01850D LDA */
    wr8(ABS(0x04E8), (uint8_t)A);  /* 01850F STA */
    SETLO(A, rd8(DP(0x02)));  /* 018512 LDA */
    wr8(ABS(0x04BE), (uint8_t)A);  /* 018514 STA */
    SETLO(A, rd8(DP(0x03)));  /* 018517 LDA */
    wr8(ABS(0x04BD), (uint8_t)A);  /* 018519 STA */
    SETLO(A, rd8(DP(0x04)));  /* 01851C LDA */
    wr8(ABS(0x04BF), (uint8_t)A);  /* 01851E STA */
    SETLO(A, rd8(DP(0x05)));  /* 018521 LDA */
    wr8(ABS(0x04C2), (uint8_t)A);  /* 018523 STA */
    goto L_018542;  /* 018526 JMP */
L_018529: TICK();
    mflag = 1;  /* 018529 SEM */
    wr8(ABS(0x04C3), 0x64);  /* 01852A LDM */
    wr8(ABS(0x04E8), 0x26);  /* 01852E LDM */
    wr8(ABS(0x04BE), 0x20);  /* 018532 LDM */
    wr8(ABS(0x04BD), 0x80);  /* 018536 LDM */
    wr8(ABS(0x04BF), 0x32);  /* 01853A LDM */
    wr8(ABS(0x04C2), 0x2D);  /* 01853E LDM */
L_018542: TICK();
    SETLO(A, rd8(ABS(0x04BD)));  /* 018542 LDA */
    /* 018545 CMP */
    if ((A & 0xFF) != 0x40) goto L_018554;  /* 018547 BNE */
    wr8(ABS(0x04BD), 0x01);  /* 018549 LDM */
    wr8(ABS(0x04E7), 0x3F);  /* 01854D LDM */
    goto L_018596;  /* 018551 JMP */
L_018554: TICK();
    mflag = 0;  /* 018554 CLM */
    A = rd16(ABS(0x04BD));  /* 018555 LDA */
    mflag = 1;  /* 018558 SEM */
    /* 018559 CMP */
    if ((A & 0xFF) != 0x80) goto L_018568;  /* 01855B BNE */
    wr8(ABS(0x04BD), 0x02);  /* 01855D LDM */
    wr8(ABS(0x04E7), 0x3F);  /* 018561 LDM */
    goto L_018596;  /* 018565 JMP */
L_018568: TICK();
    mflag = 0;  /* 018568 CLM */
    A = rd16(ABS(0x04BD));  /* 018569 LDA */
    mflag = 1;  /* 01856C SEM */
    /* 01856D CMP */
    if ((A & 0xFF) != 0x50) goto L_01857C;  /* 01856F BNE */
    wr8(ABS(0x04BD), 0x01);  /* 018571 LDM */
    wr8(ABS(0x04E7), 0x36);  /* 018575 LDM */
    goto L_018596;  /* 018579 JMP */
L_01857C: TICK();
    mflag = 0;  /* 01857C CLM */
    A = rd16(ABS(0x04BD));  /* 01857D LDA */
    mflag = 1;  /* 018580 SEM */
    /* 018581 CMP */
    if ((A & 0xFF) != 0xA0) goto L_018590;  /* 018583 BNE */
    wr8(ABS(0x04BD), 0x02);  /* 018585 LDM */
    wr8(ABS(0x04E7), 0x36);  /* 018589 LDM */
    goto L_018596;  /* 01858D JMP */
L_018590: TICK();
    mflag = 0;  /* 018590 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0800);  /* 018591 SEB */
L_018596: TICK();
    mflag = 1;  /* 018596 SEM */
    wr8(ABS(0x04EC), 0x00);  /* 018597 LDM */
    mflag = 0;  /* 01859B CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0080);  /* 01859C CLB */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0001);  /* 0185A1 CLB */
    mflag = 1;  /* 0185A6 SEM */
    wr8(ABS(0x04C5), 0x00);  /* 0185A7 LDM */
    mflag = 0;  /* 0185AB CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0010);  /* 0185AC CLB */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0020);  /* 0185B1 CLB */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0002);  /* 0185B6 CLB */
    A = rd16(ABS(0x04C2));  /* 0185BB LDA */
    mflag = 1;  /* 0185BE SEM */
    SETLO(A, lsr8(A));  /* 0185BF LSR */
    SETLO(A, lsr8(A));  /* 0185C0 LSR */
    SETLO(A, lsr8(A));  /* 0185C1 LSR */
    SETLO(A, lsr8(A));  /* 0185C2 LSR */
    SETLO(A, A & 0x03);  /* 0185C3 AND */
    SETLO(A, A + 1);  /* 0185C5 INC */
    wr8(ABS(0x04D1), (uint8_t)A);  /* 0185C6 STA */
    SETLO(A, rd8(ABS(0x04E8)));  /* 0185C9 LDA */
    SETLO(A, lsr8(A));  /* 0185CC LSR */
    SETLO(A, A + 1);  /* 0185CD INC */
    wr8(ABS(0x04EA), (uint8_t)A);  /* 0185CE STA */
    if ((rd8(ABS(0x04C2)) & 0x04) == 0) goto L_0185DF;  /* 0185D1 BBC */
    mflag = 0;  /* 0185D6 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0040);  /* 0185D7 SEB */
    goto L_0185E5;  /* 0185DC JMP */
L_0185DF: TICK();
    mflag = 0;  /* 0185DF CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0040);  /* 0185E0 CLB */
L_0185E5: TICK();
    A = rd16(ABS(0x04BE));  /* 0185E5 LDA */
    A &= 0x00FF;  /* 0185E8 AND */
    wr16(DP(0x06), A);  /* 0185EB STA */
    A = 0x03E8;  /* 0185ED LDA */
    B = rd16(DP(0x06));  /* 0185F0 LDA */
    S -= 3; F_00FF8C();  /* 0185F3 JSRL */
    mflag = 1;  /* 0185F7 SEM */
    SETLO(A, A & 0x1F);  /* 0185F8 AND */
    wr8(ABS(0x04B9), (uint8_t)A);  /* 0185FA STA */
    wr8(ABS(0x04D3), 0xAA);  /* 0185FD LDM */
    wr8(ABS(0x04D4), 0xAA);  /* 018601 LDM */
    wr8(ABS(0x04D5), 0xAA);  /* 018605 LDM */
    wr8(ABS(0x04D6), 0xAA);  /* 018609 LDM */
    wr8(ABS(0x04D7), 0xC4);  /* 01860D LDM */
    wr8(ABS(0x04D8), 0xD7);  /* 018611 LDM */
    wr8(ABS(0x04DB), 0xAA);  /* 018615 LDM */
    wr8(ABS(0x04DC), 0xC4);  /* 018619 LDM */
    wr8(ABS(0x04DD), 0xD7);  /* 01861D LDM */
    wr8(ABS(0x04E0), 0xAA);  /* 018621 LDM */
    wr8(ABS(0x04E1), 0x3B);  /* 018625 LDM */
    wr8(ABS(0x04E2), 0x28);  /* 018629 LDM */
    wr8(ABS(0x04E6), 0x00);  /* 01862D LDM */
    wr8(ABS(0x0455), 0x00);  /* 018631 LDM */
    mflag = 0;  /* 018635 CLM */
    Y = pull16();  /* 018636 PLY */
    D = pull16();  /* 018637 PLD */
    S += 3; return;  /* 018638 RTL */
}

/* 018639  callers: 018660 */
void F_018639(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018639 LDA */
    push16(A);  /* 01863B PHA */
    S -= 3; F_007EE2();  /* 01863C JSRL */
    /* 018640 CMP */
    if ((A & 0xFF) == 0x00) goto L_018652;  /* 018642 BEQ */
    mflag = 0;  /* 018644 CLM */
    S -= 3; F_007F9B();  /* 018645 JSRL */
    /* 018649 CMP */
    if ((A & 0xFF) == 0x00) goto L_018652;  /* 01864B BEQ */
    SETLO(A, 0x01);  /* 01864D LDA */
    goto L_018654;  /* 01864F JMP */
L_018652: TICK();
    SETLO(A, 0x00);  /* 018652 LDA */
L_018654: TICK();
    Y = pull16();  /* 018654 PLY */
    S += 3; return;  /* 018655 RTL */
}

/* 018656  callers: 01A750 */
void F_018656(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018656 LDA */
    push16(A);  /* 018658 PHA */
    mflag = 1;  /* 018659 SEM */
    if ((rd8(ABS(0x0335)) & 0x01) == 0) goto L_01869D;  /* 01865A BBC */
    mflag = 0;  /* 01865F CLM */
    S -= 3; F_018639();  /* 018660 JSRL */
    /* 018664 CMP */
    if ((A & 0xFF) == 0x00) goto L_01868B;  /* 018666 BEQ */
    if ((rd8(ABS(0x0334)) & 0x04) == 0) goto L_018688;  /* 018668 BBC */
    mflag = 0;  /* 01866D CLM */
    S -= 3; F_018340();  /* 01866E JSRL */
    /* 018672 CMP */
    if ((A & 0xFF) == 0x00) goto L_018683;  /* 018674 BEQ */
    mflag = 0;  /* 018676 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0800);  /* 018677 SEB */
    S -= 3; F_0184DF();  /* 01867C JSRL */
    goto L_018688;  /* 018680 JMP */
L_018683: TICK();
    mflag = 0;  /* 018683 CLM */
    S -= 3; F_0184DF();  /* 018684 JSRL */
L_018688: TICK();
    goto L_01869A;  /* 018688 JMP */
L_01868B: TICK();
    mflag = 0;  /* 01868B CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0004);  /* 01868C SEB */
    S -= 3; F_009782();  /* 018691 JSRL */
    mflag = 1;  /* 018695 SEM */
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0x05);  /* 018696 CLB */
L_01869A: TICK();
    goto L_0186A2;  /* 01869A JMP */
L_01869D: TICK();
    mflag = 1;  /* 01869D SEM */
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0x04);  /* 01869E CLB */
L_0186A2: TICK();
    mflag = 0;  /* 0186A2 CLM */
    Y = pull16();  /* 0186A3 PLY */
    S += 3; return;  /* 0186A4 RTL */
}

/* 0186A5  callers: 01874A, 0187AA */
void F_0186A5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0186A5 LDA */
    push16(D);  /* 0186A7 PHD */
    D = A;  /* 0186A8 TAD */
    A = rd16(ABS(0x0453));  /* 0186A9 LDA */
    C = 0;  /* 0186AC CLC */
    mflag = 1;  /* 0186AD SEM */
    SETLO(A, adc8(A, rd8(ABS(0x04BD))));  /* 0186AE ADC */
    wr8(ABS(0x0453), (uint8_t)A);  /* 0186B1 STA */
    SETLO(A, rd8(ABS(0x04BE)));  /* 0186B4 LDA */
    mpy8(rd8(ABS(0x04BD)));  /* 0186B7 MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 0186BB STA */
    wr8(DP(0x01), (uint8_t)B);  /* 0186BD STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 0186C0 LDA */
    mflag = 0;  /* 0186C3 CLM */
    A &= 0x00FF;  /* 0186C4 AND */
    C = 1;  /* 0186C7 SEC */
    A = sbc16(A, rd16(DP(0x00)));  /* 0186C8 SBC */
    if (!V) goto L_0186CF;  /* 0186CA BVC */
    A ^= 0x8000;  /* 0186CC EOR */
    N = ((A & 0x8000) != 0);
L_0186CF: TICK();
    if (N) goto L_0186D6;  /* 0186CF BMI */
    mflag = 1;  /* 0186D1 SEM */
    wr8(ABS(0x0453), 0x00);  /* 0186D2 LDM */
L_0186D6: TICK();
    mflag = 0;  /* 0186D6 CLM */
    D = pull16();  /* 0186D7 PLD */
    S += 3; return;  /* 0186D8 RTL */
}

/* 0186D9  callers: 01882D, 018857 */
void F_0186D9(void)
{
    uint32_t t; (void)t;
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0008);  /* 0186D9 SEB */
    mflag = 1;  /* 0186DE SEM */
    SETLO(A, rd8(ABS(0x0453)));  /* 0186DF LDA */
    if ((A & 0xFF) != 0) goto L_0186F2;  /* 0186E2 BNE */
    SETLO(A, rd8(ABS(0x04BE)));  /* 0186E4 LDA */
    SETLO(A, A - 1);  /* 0186E7 DEC */
    mpy8(rd8(ABS(0x04BD)));  /* 0186E8 MPY */
    wr8(ABS(0x0453), (uint8_t)A);  /* 0186EC STA */
    goto L_0186FE;  /* 0186EF JMP */
L_0186F2: TICK();
    mflag = 0;  /* 0186F2 CLM */
    A = rd16(ABS(0x0453));  /* 0186F3 LDA */
    C = 1;  /* 0186F6 SEC */
    mflag = 1;  /* 0186F7 SEM */
    SETLO(A, sbc8(A, rd8(ABS(0x04BD))));  /* 0186F8 SBC */
    wr8(ABS(0x0453), (uint8_t)A);  /* 0186FB STA */
L_0186FE: TICK();
    mflag = 0;  /* 0186FE CLM */
    S += 3; return;  /* 0186FF RTL */
}

/* 018700  callers: 018271, 018C2D, 018CEC, 018D53, 018E45, 018F0A, 018F63, 0192A0 */
void F_018700(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018700 LDA */
    push16(D);  /* 018702 PHD */
    D = A;  /* 018703 TAD */
    A += 1;  /* 018704 INC */
    A += 1;  /* 018705 INC */
    push16(A);  /* 018706 PHA */
    A = rd16(ABS(0x0456));  /* 018707 LDA */
    A &= 0x00FF;  /* 01870A AND */
    X = A;  /* 01870D TAX */
    A = rd16((ABS(0x0489) + X));  /* 01870E LDA */
    mflag = 1;  /* 018711 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 018712 STA */
    SETLO(A, rd8(ABS(0x0228)));  /* 018714 LDA */
    /* 018717 CMP */
    if ((A & 0xFF) == 0x08) goto L_01871F;  /* 018719 BEQ */
    /* 01871B CMP */
    if ((A & 0xFF) != 0x09) goto L_01872D;  /* 01871D BNE */
L_01871F: TICK();
    SETLO(A, rd8(ABS(0x0456)));  /* 01871F LDA */
    mflag = 0;  /* 018722 CLM */
    A &= 0x00FF;  /* 018723 AND */
    X = A;  /* 018726 TAX */
    A = rd16((ABS(0x0459) + X));  /* 018727 LDA */
    mflag = 1;  /* 01872A SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 01872B STA */
L_01872D: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 01872D LDA */
    mpy8(rd8(ABS(0x04BD)));  /* 01872F MPY */
    SETLO(A, A - 1);  /* 018733 DEC */
    wr8(ABS(0x0452), (uint8_t)A);  /* 018734 STA */
    t = (rd8(ABS(0x0456)) + 1) & 0xFF; wr8(ABS(0x0456), t);  /* 018737 INC */
    t = (rd8(ABS(0x04C1)) + 1) & 0xFF; wr8(ABS(0x04C1), t);  /* 01873A INC */
    SETLO(A, rd8(ABS(0x0456)));  /* 01873D LDA */
    t = rd8(ABS(0x04BE));  /* 018740 CMP */
    if ((A & 0xFF) != t) goto L_018749;  /* 018743 BNE */
    wr8(ABS(0x0456), 0x00);  /* 018745 LDM */
L_018749: TICK();
    mflag = 0;  /* 018749 CLM */
    S -= 3; F_0186A5();  /* 01874A JSRL */
    A = rd16(ABS(0x0228));  /* 01874E LDA */
    mflag = 1;  /* 018751 SEM */
    /* 018752 CMP */
    if ((A & 0xFF) != 0x08) goto L_018759;  /* 018754 BNE */
    goto L_01885B;  /* 018756 JMP */
L_018759: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 018759 LDA */
    /* 01875C CMP */
    if ((A & 0xFF) != 0x09) goto L_018763;  /* 01875E BNE */
    goto L_01885B;  /* 018760 JMP */
L_018763: TICK();
    mflag = 0;  /* 018763 CLM */
    if ((rd16(ABS(0x0248)) & 0x0040) == 0x0040) goto L_01876D;  /* 018764 BBS */
    goto L_01885B;  /* 01876A JMP */
L_01876D: TICK();
    A = rd16(ABS(0x04C1));  /* 01876D LDA */
    mflag = 1;  /* 018770 SEM */
    t = rd8(ABS(0x04E8));  /* 018771 CMP */
    if ((A & 0xFF) != t) goto L_01879E;  /* 018774 BNE */
    wr8(ABS(0x04C1), 0x00);  /* 018776 LDM */
    mflag = 0;  /* 01877A CLM */
    if ((rd16(ABS(0x0248)) & 0x0004) == 0) goto L_018790;  /* 01877B BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0004);  /* 018781 CLB */
    A = rd16(ABS(0x04D1));  /* 018786 LDA */
    mflag = 1;  /* 018789 SEM */
    wr8(ABS(0x04E9), (uint8_t)A);  /* 01878A STA */
    goto L_01879E;  /* 01878D JMP */
L_018790: TICK();
    mflag = 1;  /* 018790 SEM */
    t = (rd8(ABS(0x04E9)) - 1) & 0xFF; wr8(ABS(0x04E9), t);  /* 018791 DEC */
    SETLO(A, rd8(ABS(0x04E9)));  /* 018794 LDA */
    if ((A & 0xFF) != 0) goto L_01879E;  /* 018797 BNE */
    mflag = 0;  /* 018799 CLM */
    S -= 3; F_01906F();  /* 01879A JSRL */
L_01879E: TICK();
    mflag = 0;  /* 01879E CLM */
    if ((rd16(ABS(0x0248)) & 0x0008) == 0) goto L_0187AE;  /* 01879F BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0008);  /* 0187A5 CLB */
    S -= 3; F_0186A5();  /* 0187AA JSRL */
L_0187AE: TICK();
    mflag = 1;  /* 0187AE SEM */
    SETLO(A, rd8(ABS(0x04EB)));  /* 0187AF LDA */
    if ((A & 0xFF) != 0) goto L_0187CF;  /* 0187B2 BNE */
    SETLO(A, rd8(ABS(0x04C1)));  /* 0187B4 LDA */
    mflag = 0;  /* 0187B7 CLM */
    A &= 0x00FF;  /* 0187B8 AND */
    wr16(DP(0x02), A);  /* 0187BB STA */
    A = rd16(ABS(0x04E8));  /* 0187BD LDA */
    A &= 0x00FF;  /* 0187C0 AND */
    A -= 1;  /* 0187C3 DEC */
    t = rd16(DP(0x02));  /* 0187C4 CMP */
    if (A != t) goto L_0187CF;  /* 0187C6 BNE */
    A = rd16(ABS(0x0456));  /* 0187C8 LDA */
    mflag = 1;  /* 0187CB SEM */
    wr8(ABS(0x04D2), (uint8_t)A);  /* 0187CC STA */
L_0187CF: TICK();
    mflag = 0;  /* 0187CF CLM */
    A = rd16(ABS(0x04EB));  /* 0187D0 LDA */
    mflag = 1;  /* 0187D3 SEM */
    /* 0187D4 CMP */
    if ((A & 0xFF) != 0x01) goto L_0187E6;  /* 0187D6 BNE */
    SETLO(A, rd8(ABS(0x04C1)));  /* 0187D8 LDA */
    t = rd8(ABS(0x04EA));  /* 0187DB CMP */
    if ((A & 0xFF) != t) goto L_0187E6;  /* 0187DE BNE */
    SETLO(A, rd8(ABS(0x0456)));  /* 0187E0 LDA */
    wr8(ABS(0x04D2), (uint8_t)A);  /* 0187E3 STA */
L_0187E6: TICK();
    SETLO(A, rd8(ABS(0x04BD)));  /* 0187E6 LDA */
    /* 0187E9 CMP */
    if ((A & 0xFF) >= 0x02) goto L_018831;  /* 0187EB BCS */
    SETLO(A, rd8(ABS(0x04D2)));  /* 0187ED LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 0187F0 STA */
    mflag = 0;  /* 0187F2 CLM */
    A &= 0x00FF;  /* 0187F3 AND */
    wr16(DP(0x02), A);  /* 0187F6 STA */
    A = rd16(ABS(0x04BE));  /* 0187F8 LDA */
    A &= 0x00FF;  /* 0187FB AND */
    A -= 1;  /* 0187FE DEC */
    t = rd16(DP(0x02));  /* 0187FF CMP */
    if (A != t) goto L_018807;  /* 018801 BNE */
    mflag = 1;  /* 018803 SEM */
    wr8(DP(0x01), 0x00);  /* 018804 LDM */
L_018807: TICK();
    mflag = 0;  /* 018807 CLM */
    A = rd16(ABS(0x0453));  /* 018808 LDA */
    A &= 0x00FF;  /* 01880B AND */
    wr16(DP(0x02), A);  /* 01880E STA */
    A = rd16(DP(0x01));  /* 018810 LDA */
    A &= 0x00FF;  /* 018812 AND */
    A += 1;  /* 018815 INC */
    X = A;  /* 018816 TAX */
    A = rd16((ABS(0x0489) + X));  /* 018817 LDA */
    A &= 0x00FF;  /* 01881A AND */
    A -= 1;  /* 01881D DEC */
    wr16(DP(0x04), A);  /* 01881E STA */
    A = rd16(ABS(0x04BD));  /* 018820 LDA */
    A &= 0x00FF;  /* 018823 AND */
    mpy16(rd16(DP(0x04)));  /* 018826 MPY */
    t = rd16(DP(0x02));  /* 018829 CMP */
    if (A != t) goto L_018831;  /* 01882B BNE */
    S -= 3; F_0186D9();  /* 01882D JSRL */
L_018831: TICK();
    mflag = 0;  /* 018831 CLM */
    A = rd16(ABS(0x0453));  /* 018832 LDA */
    A &= 0x00FF;  /* 018835 AND */
    wr16(DP(0x02), A);  /* 018838 STA */
    A = rd16(ABS(0x04D2));  /* 01883A LDA */
    A &= 0x00FF;  /* 01883D AND */
    X = A;  /* 018840 TAX */
    A = rd16((ABS(0x0489) + X));  /* 018841 LDA */
    A &= 0x00FF;  /* 018844 AND */
    A -= 1;  /* 018847 DEC */
    wr16(DP(0x04), A);  /* 018848 STA */
    A = rd16(ABS(0x04BD));  /* 01884A LDA */
    A &= 0x00FF;  /* 01884D AND */
    mpy16(rd16(DP(0x04)));  /* 018850 MPY */
    t = rd16(DP(0x02));  /* 018853 CMP */
    if (A != t) goto L_01885B;  /* 018855 BNE */
    S -= 3; F_0186D9();  /* 018857 JSRL */
L_01885B: TICK();
    mflag = 0;  /* 01885B CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0010);  /* 01885C CLB */
    Y = pull16();  /* 018861 PLY */
    D = pull16();  /* 018862 PLD */
    S += 3; return;  /* 018863 RTL */
}

/* 018864  callers: 01899C */
void F_018864(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018864 LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    push16(D);  /* 018866 PHD */
    D = A;  /* 018867 TAD */
    wr16(DP(0x00), Y);  /* 018868 STY */
    push16(getP());  /* 01886A PHP */
    mflag = 1;  /* 01886B SEM */
    SETLO(A, pull8());  /* 01886C PLA */
    SETLO(A, pull8());  /* 01886D PLA */
    wr8(DP(0x02), (uint8_t)A);  /* 01886E STA */
    C = ((A & 0xFF) >= 0x03);  /* 018870 CMP */
    if (C) goto L_01887A;  /* 018872 BCS */
    SETLO(A, 0x03);  /* 018874 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 018876 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 018877 STA */
    setP(pull16());  /* 018879 PLP */
L_01887A: TICK();
    mflag = 0;  /* 01887A CLM */
    A = D;  /* 01887B TDA */
    wr16(DP(0x03), A);  /* 01887C STA */
    mflag = 1;  /* 01887E SEM */
    wr8(DP(0x05), 0x00);  /* 01887F LDM */
    wr8(ABS(U1MOD), 0x01);  /* 018882 LDM */
    wr8(ABS(U1C0), 0x00);  /* 018886 LDM */
    wr8(ABS(U1BRG), 0x07);  /* 01888A LDM */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x02);  /* 01888E CLB [P51 TDSEC1] */
    wr8(ABS(U1C1), 0x05);  /* 018892 LDM */
L_018896: TICK();
    mflag = 0;  /* 018896 CLM */
    A = rd16(DP(0x03));  /* 018897 LDA */
    A -= 1;  /* 018899 DEC */
    A -= 1;  /* 01889A DEC */
    wr16(DP(0x03), A);  /* 01889B STA */
    wr16(DP(0x08), A);  /* 01889D STA */
    A = rd16(DP(0x05));  /* 01889F LDA */
    mflag = 1;  /* 0188A1 SEM */
    wr8(DP(0x0A), (uint8_t)A);  /* 0188A2 STA */
    mflag = 0;  /* 0188A4 CLM */
    A = rd16(LIND(0x08));  /* 0188A5 LDA */
    X = A;  /* 0188A7 TAX */
    A = rd16((0x002B06 + X));  /* 0188A8 LDA */
    wr16(DP(0x06), X);  /* 0188AC STX */
    mflag = 1;  /* 0188AE SEM */
    wr8(ABS(U1TB), (uint8_t)A);  /* 0188AF STA */
L_0188B2: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_0188BA;  /* 0188B2 BBS */
    goto L_0188B2;  /* 0188B7 JMP */
L_0188BA: TICK();
    mflag = 1;  /* 0188BA SEM */
    if ((rd8(ABS(U1C1)) & 0x08) == 0x08) goto L_0188C3;  /* 0188BB BBS */
    goto L_0188BA;  /* 0188C0 JMP */
L_0188C3: TICK();
    mflag = 1;  /* 0188C3 SEM */
    SETLO(A, rd8(ABS(U1RB)));  /* 0188C4 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 0188C7 STA */
    t = (rd8(DP(0x00)) - 1) & 0xFF; wr8(DP(0x00), t);  /* 0188C9 DEC */
    SETLO(A, rd8(DP(0x00)));  /* 0188CB LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    if (N) goto L_0188D1;  /* 0188CD BMI */
    if (!Z) goto L_018896;  /* 0188CF BNE */
L_0188D1: TICK();
    if ((rd8(ABS(U1C0)) & 0x08) == 0x08) goto L_0188D9;  /* 0188D1 BBS */
    goto L_0188D1;  /* 0188D6 JMP */
L_0188D9: TICK();
    mflag = 1;  /* 0188D9 SEM */
    wr8(ABS(U1C1), rd8(ABS(U1C1)) & ~0x05);  /* 0188DA CLB */
    wr8(ABS(P5), rd8(ABS(P5)) | 0x02);  /* 0188DE SEB [P51 TDSEC1] */
    wr8(ABS(U1MOD), 0x00);  /* 0188E2 LDM */
    SETLO(A, rd8(DP(0x02)));  /* 0188E6 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0188E8 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0188E9 STA */
    setP(pull16());  /* 0188EB PLP */
    mflag = 0;  /* 0188EC CLM */
    A = rd16(DP(0x01));  /* 0188ED LDA */
    A &= 0x00FF;  /* 0188EF AND */
    X = A;  /* 0188F2 TAX */
    A = rd16((0x002B06 + X));  /* 0188F3 LDA */
    mflag = 1;  /* 0188F7 SEM */
    D = pull16();  /* 0188F8 PLD */
    S += 3; return;  /* 0188F9 RTL */
}

/* 0188FA  callers: 0189A5 */
void F_0188FA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0188FA LDA */
    push16(D);  /* 0188FC PHD */
    D = A;  /* 0188FD TAD */
    mflag = 1;  /* 0188FE SEM */
    SETLO(A, 0x03);  /* 0188FF LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 018901 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 018902 STA */
    setP(pull16());  /* 018904 PLP */
    mflag = 1;  /* 018905 SEM */
    wr8(ABS(U1MOD), 0x01);  /* 018906 LDM */
    wr8(ABS(U1C0), 0x00);  /* 01890A LDM */
    wr8(ABS(U1BRG), 0x07);  /* 01890E LDM */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x02);  /* 018912 CLB [P51 TDSEC1] */
    wr8(ABS(U1C1), 0x05);  /* 018916 LDM */
    SETLO(A, rd8(0x002B6E));  /* 01891A LDA */
    wr8(ABS(U1TB), (uint8_t)A);  /* 01891E STA */
L_018921: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_018929;  /* 018921 BBS */
    goto L_018921;  /* 018926 JMP */
L_018929: TICK();
    mflag = 0;  /* 018929 CLM */
    wr16(DP(0x00), 0x017F);  /* 01892A LDM */
L_01892E: TICK();
    mflag = 1;  /* 01892E SEM */
    wr8(ABS(WDT), 0x00);  /* 01892F LDM */
    wr8(ABS(U1TB), 0x55);  /* 018933 LDM */
L_018937: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_01893F;  /* 018937 BBS */
    goto L_018937;  /* 01893C JMP */
L_01893F: TICK();
    mflag = 0;  /* 01893F CLM */
    t = (rd16(DP(0x00)) - 1) & 0xFFFF; wr16(DP(0x00), t);  /* 018940 DEC */
    X = rd16(DP(0x00));  /* 018942 LDX */
    if (X != 0) goto L_01892E;  /* 018944 BNE */
L_018946: TICK();
    mflag = 1;  /* 018946 SEM */
    if ((rd8(ABS(U1C0)) & 0x08) == 0x08) goto L_01894F;  /* 018947 BBS */
    goto L_018946;  /* 01894C JMP */
L_01894F: TICK();
    mflag = 1;  /* 01894F SEM */
    wr8(ABS(U1C1), rd8(ABS(U1C1)) & ~0x05);  /* 018950 CLB */
    wr8(ABS(P5), rd8(ABS(P5)) | 0x02);  /* 018954 SEB [P51 TDSEC1] */
    wr8(ABS(U1MOD), 0x00);  /* 018958 LDM */
    SETLO(A, 0x00);  /* 01895C LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 01895E PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 01895F STA */
    setP(pull16());  /* 018961 PLP */
    mflag = 0;  /* 018962 CLM */
    D = pull16();  /* 018963 PLD */
    S += 3; return;  /* 018964 RTL */
}

/* 018965  callers: 018E9D */
void F_018965(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018965 LDA */
    push16(D);  /* 018967 PHD */
    D = A;  /* 018968 TAD */
    C = 0;  /* 018969 CLC */
    A = adc16(A, 0x000A);  /* 01896A ADC */
    push16(A);  /* 01896D PHA */
    A = rd16(ABS(0x04BE));  /* 01896E LDA */
    mflag = 1;  /* 018971 SEM */
    mpy8(rd8(ABS(0x04BD)));  /* 018972 MPY */
    wr8(DP(0x02), (uint8_t)A);  /* 018976 STA */
    wr8(DP(0x03), (uint8_t)B);  /* 018978 STA */
    mflag = 0;  /* 01897B CLM */
    A = rd16(DP(0x02));  /* 01897C LDA */
    A -= 1;  /* 01897E DEC */
    A = asl16(A);  /* 01897F ASL */
    A = asl16(A);  /* 018980 ASL */
    A = asl16(A);  /* 018981 ASL */
    A = asl16(A);  /* 018982 ASL */
    wr16(DP(0x02), 0x0055);  /* 018983 LDM */
    wr16(DP(0x00), A);  /* 018987 STA */
    A &= 0x00FF;  /* 018989 AND */
    wr16(DP(0x04), A);  /* 01898C STA */
    A = rd16(DP(0x01));  /* 01898E LDA */
    A &= 0x00FF;  /* 018990 AND */
    wr16(DP(0x06), A);  /* 018993 STA */
    wr16(DP(0x08), 0x0066);  /* 018995 LDM */
    Y = 0x0004;  /* 018999 LDY */
    S -= 3; F_018864();  /* 01899C JSRL */
    mflag = 0;  /* 0189A0 CLM */
    A = D;  /* 0189A1 TDA */
    A += 1;  /* 0189A2 INC */
    A += 1;  /* 0189A3 INC */
    push16(A);  /* 0189A4 PHA */
    S -= 3; F_0188FA();  /* 0189A5 JSRL */
    Y = pull16();  /* 0189A9 PLY */
    Y = pull16();  /* 0189AA PLY */
    D = pull16();  /* 0189AB PLD */
    S += 3; return;  /* 0189AC RTL */
}

/* 0189AD  callers: 0180DC, 018179 */
void F_0189AD(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 0189AD SEM */
    wr8(ABS(TA3IC), 0x00);  /* 0189AE LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 0189B2 CLB */
    wr8(ABS(TA3MOD), 0x80);  /* 0189B6 LDM */
    mflag = 0;  /* 0189BA CLM */
    wr16(ABS(TA3), 0x5FFF);  /* 0189BB LDM */
    mflag = 1;  /* 0189C0 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 0189C1 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 0189C5 SEB */
    mflag = 0;  /* 0189C9 CLM */
    S += 3; return;  /* 0189CA RTL */
}

/* 0189CB  callers: 018034 */
void F_0189CB(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 0189CB SEM */
    wr8(ABS(TA3IC), 0x00);  /* 0189CC LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 0189D0 CLB */
    wr8(ABS(TA3MOD), 0x80);  /* 0189D4 LDM */
    mflag = 0;  /* 0189D8 CLM */
    wr16(ABS(TA3), 0x0EC4);  /* 0189D9 LDM */
    mflag = 1;  /* 0189DE SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 0189DF CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 0189E3 SEB */
    mflag = 0;  /* 0189E7 CLM */
    S += 3; return;  /* 0189E8 RTL */
}

/* 0189E9  callers: 0180EC, 018134, 018158, 018EE0, 01907A, 019100 */
void F_0189E9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0189E9 LDA */
    push16(A);  /* 0189EB PHA */
    A = rd16(ABS(0x0228));  /* 0189EC LDA */
    mflag = 1;  /* 0189EF SEM */
    /* 0189F0 CMP */
    if ((A & 0xFF) == 0x02) goto L_0189FB;  /* 0189F2 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 0189F4 LDA */
    /* 0189F7 CMP */
    if ((A & 0xFF) != 0x03) goto L_018A00;  /* 0189F9 BNE */
L_0189FB: TICK();
    mflag = 0;  /* 0189FB CLM */
    S -= 3; F_007D35();  /* 0189FC JSRL */
L_018A00: TICK();
    mflag = 0;  /* 018A00 CLM */
    Y = pull16();  /* 018A01 PLY */
    S += 3; return;  /* 018A02 RTL */
}

/* 018A03  callers: 018C0A, 018E3D */
void F_018A03(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018A03 LDA */
    push16(D);  /* 018A05 PHD */
    D = A;  /* 018A06 TAD */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0004);  /* 018A07 CLB */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0008);  /* 018A0C CLB */
    A = rd16(ABS(0x0228));  /* 018A11 LDA */
    mflag = 1;  /* 018A14 SEM */
    /* 018A15 CMP */
    if ((A & 0xFF) == 0x08) goto L_018A20;  /* 018A17 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 018A19 LDA */
    /* 018A1C CMP */
    if ((A & 0xFF) != 0x09) goto L_018A3B;  /* 018A1E BNE */
L_018A20: TICK();
    SETLO(A, rd8(ABS(0x04BE)));  /* 018A20 LDA */
    SETLO(A, A - 1);  /* 018A23 DEC */
    mpy8(rd8(ABS(0x04BD)));  /* 018A24 MPY */
    wr8(ABS(0x0453), (uint8_t)A);  /* 018A28 STA */
    SETLO(A, rd8(ABS(0x0459)));  /* 018A2B LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 018A2E STA */
    wr8(ABS(0x0456), 0x01);  /* 018A30 LDM */
    wr8(ABS(0x04C1), 0x00);  /* 018A34 LDM */
    goto L_018AA1;  /* 018A38 JMP */
L_018A3B: TICK();
    mflag = 1;  /* 018A3B SEM */
    wr8(ABS(0x04C1), 0x02);  /* 018A3C LDM */
    SETLO(A, rd8(ABS(0x04EB)));  /* 018A40 LDA */
    /* 018A43 CMP */
    if ((A & 0xFF) != 0x01) goto L_018A60;  /* 018A45 BNE */
    SETLO(A, rd8(ABS(0x04BE)));  /* 018A47 LDA */
    C = 0;  /* 018A4A CLC */
    SETLO(A, adc8(A, 0xF0));  /* 018A4B ADC */
    mpy8(rd8(ABS(0x04BD)));  /* 018A4D MPY */
    wr8(ABS(0x0453), (uint8_t)A);  /* 018A51 STA */
    SETLO(A, rd8(ABS(0x0489)));  /* 018A54 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 018A57 STA */
    wr8(ABS(0x0456), 0x00);  /* 018A59 LDM */
    goto L_018AA1;  /* 018A5D JMP */
L_018A60: TICK();
    mflag = 0;  /* 018A60 CLM */
    A = rd16(ABS(0x04C7));  /* 018A61 LDA */
    mflag = 1;  /* 018A64 SEM */
    C = ((A & 0xFF) >= 0x10);  /* 018A65 CMP */
    if (!C) goto L_018A78;  /* 018A67 BCC */
    SETLO(A, rd8(ABS(0x04C7)));  /* 018A69 LDA */
    SETLO(A, adc8(A, 0xEF));  /* 018A6C ADC */
    mpy8(rd8(ABS(0x04BD)));  /* 018A6E MPY */
    wr8(ABS(0x0453), (uint8_t)A);  /* 018A72 STA */
    goto L_018A8B;  /* 018A75 JMP */
L_018A78: TICK();
    mflag = 0;  /* 018A78 CLM */
    A = rd16(ABS(0x04BE));  /* 018A79 LDA */
    C = 0;  /* 018A7C CLC */
    mflag = 1;  /* 018A7D SEM */
    SETLO(A, adc8(A, rd8(ABS(0x04C7))));  /* 018A7E ADC */
    C = 0;  /* 018A81 CLC */
    SETLO(A, adc8(A, 0xF0));  /* 018A82 ADC */
    mpy8(rd8(ABS(0x04BD)));  /* 018A84 MPY */
    wr8(ABS(0x0453), (uint8_t)A);  /* 018A88 STA */
L_018A8B: TICK();
    SETLO(A, rd8(ABS(0x04C7)));  /* 018A8B LDA */
    mflag = 0;  /* 018A8E CLM */
    A &= 0x00FF;  /* 018A8F AND */
    A -= 1;  /* 018A92 DEC */
    X = A;  /* 018A93 TAX */
    A = rd16((ABS(0x0489) + X));  /* 018A94 LDA */
    mflag = 1;  /* 018A97 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 018A98 STA */
    SETLO(A, rd8(ABS(0x04C7)));  /* 018A9A LDA */
    SETLO(A, A - 1);  /* 018A9D DEC */
    wr8(ABS(0x0456), (uint8_t)A);  /* 018A9E STA */
L_018AA1: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 018AA1 LDA */
    mpy8(rd8(ABS(0x04BD)));  /* 018AA3 MPY */
    SETLO(A, A - 1);  /* 018AA7 DEC */
    wr8(ABS(0x0452), (uint8_t)A);  /* 018AA8 STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 018AAB LDA */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018AAE STA */
    SETLO(A, rd8(ABS(0x0452)));  /* 018AB1 LDA */
    wr8(ABS(0x0450), (uint8_t)A);  /* 018AB4 STA */
    SETLO(A, rd8(ABS(0x04BD)));  /* 018AB7 LDA */
    wr8(ABS(0x04C4), (uint8_t)A);  /* 018ABA STA */
    mflag = 0;  /* 018ABD CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0010);  /* 018ABE SEB */
    D = pull16();  /* 018AC3 PLD */
    S += 3; return;  /* 018AC4 RTL */
}

/* 018AC5  callers: 018C06, 018E35 */
void F_018AC5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018AC5 LDA */
    push16(D);  /* 018AC7 PHD */
    D = A;  /* 018AC8 TAD */
    C = 0;  /* 018AC9 CLC */
    A = adc16(A, 0x0020);  /* 018ACA ADC */
    push16(A);  /* 018ACD PHA */
    wr16(DP(0x00), Y);  /* 018ACE STY */
    A = rd16(ABS(0x04BE));  /* 018AD0 LDA */
    mflag = 1;  /* 018AD3 SEM */
    mpy8(rd8(DP(0x00)));  /* 018AD4 MPY */
    wr8(DP(0x20), (uint8_t)A);  /* 018AD7 STA */
    wr8(DP(0x21), (uint8_t)B);  /* 018AD9 STA */
    mflag = 0;  /* 018ADC CLM */
    A = rd16(DP(0x20));  /* 018ADD LDA */
    B = 0x0002;  /* 018ADF LDA */
    S -= 3; F_00FF8C();  /* 018AE3 JSRL */
    wr16(DP(0x06), A);  /* 018AE7 STA */
    if ((rd16(ABS(0x0248)) & 0x0800) == 0x0800) goto L_018B13;  /* 018AE9 BBS */
    wr16(DP(0x20), 0x0018);  /* 018AEF LDM */
    A = D;  /* 018AF3 TDA */
    C = 0;  /* 018AF4 CLC */
    A = adc16(A, 0x0008);  /* 018AF5 ADC */
    B = 0x0000;  /* 018AF8 LDA */
    wr16(DP(0x21), A);  /* 018AFC STA */
    wr16(DP(0x23), B);  /* 018AFE STA */
    A = rd16(DP(0x06));  /* 018B01 LDA */
    C = 0;  /* 018B03 CLC */
    A = adc16(A, 0x0604);  /* 018B04 ADC */
    wr16(DP(0x24), A);  /* 018B07 STA */
    Y = 0x00A0;  /* 018B09 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 018B0C JSRL */
    goto L_018B2B;  /* 018B10 JMP */
L_018B13: TICK();
    mflag = 1;  /* 018B13 SEM */
    wr8(DP(0x02), 0x00);  /* 018B14 LDM */
L_018B17: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 018B17 LDA */
    /* 018B19 CMP */
    if ((A & 0xFF) >= 0x18) goto L_018B2B;  /* 018B1B BCS */
    mflag = 0;  /* 018B1D CLM */
    A &= 0x00FF;  /* 018B1E AND */
    X = A;  /* 018B21 TAX */
    mflag = 1;  /* 018B22 SEM */
    wr8(DP(0x08 + X), 0x88);  /* 018B23 LDM */
    t = (rd8(DP(0x02)) + 1) & 0xFF; wr8(DP(0x02), t);  /* 018B26 INC */
    goto L_018B17;  /* 018B28 JMP */
L_018B2B: TICK();
    mflag = 1;  /* 018B2B SEM */
    wr8(DP(0x01), 0x00);  /* 018B2C LDM */
    wr8(DP(0x02), 0x00);  /* 018B2F LDM */
L_018B32: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 018B32 LDA */
    mflag = 0;  /* 018B34 CLM */
    A &= 0x00FF;  /* 018B35 AND */
    wr16(DP(0x20), A);  /* 018B38 STA */
    A = rd16(ABS(0x04BE));  /* 018B3A LDA */
    A &= 0x00FF;  /* 018B3D AND */
    A -= 1;  /* 018B40 DEC */
    C = 1;  /* 018B41 SEC */
    A = sbc16(A, rd16(DP(0x20)));  /* 018B42 SBC */
    if (!V) goto L_018B49;  /* 018B44 BVC */
    A ^= 0x8000;  /* 018B46 EOR */
    N = ((A & 0x8000) != 0);
L_018B49: TICK();
    if (N) goto L_018B91;  /* 018B49 BMI */
    A = rd16(DP(0x02));  /* 018B4B LDA */
    A &= 0x00FF;  /* 018B4D AND */
    X = A;  /* 018B50 TAX */
    A = rd16(DP(0x08 + X));  /* 018B51 LDA */
    mflag = 1;  /* 018B53 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 018B54 STA */
    SETLO(A, A & 0x0F);  /* 018B56 AND */
    wr8(DP(0x04), (uint8_t)A);  /* 018B58 STA */
    SETLO(A, rd8(DP(0x03)));  /* 018B5A LDA */
    SETLO(A, lsr8(A));  /* 018B5C LSR */
    SETLO(A, lsr8(A));  /* 018B5D LSR */
    SETLO(A, lsr8(A));  /* 018B5E LSR */
    SETLO(A, lsr8(A));  /* 018B5F LSR */
    wr8(DP(0x05), (uint8_t)A);  /* 018B60 STA */
    C = 0;  /* 018B62 CLC */
    SETLO(A, adc8(A, rd8(DP(0x01))));  /* 018B63 ADC */
    wr8(DP(0x20), (uint8_t)A);  /* 018B65 STA */
    SETLO(A, rd8(DP(0x01)));  /* 018B67 LDA */
    mflag = 0;  /* 018B69 CLM */
    A &= 0x00FF;  /* 018B6A AND */
    X = A;  /* 018B6D TAX */
    A = rd16(DP(0x20));  /* 018B6E LDA */
    mflag = 1;  /* 018B70 SEM */
    wr8((ABS(0x0459) + X), (uint8_t)A);  /* 018B71 STA */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 018B74 INC */
    SETLO(A, rd8(DP(0x01)));  /* 018B76 LDA */
    C = 0;  /* 018B78 CLC */
    SETLO(A, adc8(A, rd8(DP(0x04))));  /* 018B79 ADC */
    wr8(DP(0x20), (uint8_t)A);  /* 018B7B STA */
    SETLO(A, rd8(DP(0x01)));  /* 018B7D LDA */
    mflag = 0;  /* 018B7F CLM */
    A &= 0x00FF;  /* 018B80 AND */
    X = A;  /* 018B83 TAX */
    A = rd16(DP(0x20));  /* 018B84 LDA */
    mflag = 1;  /* 018B86 SEM */
    wr8((ABS(0x0459) + X), (uint8_t)A);  /* 018B87 STA */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 018B8A INC */
    t = (rd8(DP(0x02)) + 1) & 0xFF; wr8(DP(0x02), t);  /* 018B8C INC */
    goto L_018B32;  /* 018B8E JMP */
L_018B91: TICK();
    mflag = 1;  /* 018B91 SEM */
    wr8(DP(0x01), 0x00);  /* 018B92 LDM */
L_018B95: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 018B95 LDA */
    mflag = 0;  /* 018B97 CLM */
    A &= 0x00FF;  /* 018B98 AND */
    X = A;  /* 018B9B TAX */
    A = rd16(ABS(0x04BE));  /* 018B9C LDA */
    mflag = 1;  /* 018B9F SEM */
    t = rd8((ABS(0x0459) + X));  /* 018BA0 CMP */
    if ((A & 0xFF) >= t) goto L_018BB7;  /* 018BA3 BCS */
    SETLO(A, rd8(DP(0x01)));  /* 018BA5 LDA */
    mflag = 0;  /* 018BA7 CLM */
    A &= 0x00FF;  /* 018BA8 AND */
    X = A;  /* 018BAB TAX */
    A = rd16((ABS(0x0459) + X));  /* 018BAC LDA */
    C = 1;  /* 018BAF SEC */
    mflag = 1;  /* 018BB0 SEM */
    SETLO(A, sbc8(A, rd8(ABS(0x04BE))));  /* 018BB1 SBC */
    wr8((ABS(0x0459) + X), (uint8_t)A);  /* 018BB4 STA */
L_018BB7: TICK();
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 018BB7 INC */
    SETLO(A, rd8(DP(0x01)));  /* 018BB9 LDA */
    t = rd8(ABS(0x04BE));  /* 018BBB CMP */
    if ((A & 0xFF) >= t) goto L_018BC3;  /* 018BBE BCS */
    goto L_018B95;  /* 018BC0 JMP */
L_018BC3: TICK();
    mflag = 1;  /* 018BC3 SEM */
    wr8(DP(0x01), 0x00);  /* 018BC4 LDM */
L_018BC7: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 018BC7 LDA */
    SETLO(A, A + 1);  /* 018BC9 INC */
    wr8(DP(0x20), (uint8_t)A);  /* 018BCA STA */
    SETLO(A, rd8(DP(0x01)));  /* 018BCC LDA */
    mflag = 0;  /* 018BCE CLM */
    A &= 0x00FF;  /* 018BCF AND */
    X = A;  /* 018BD2 TAX */
    A = rd16((ABS(0x0459) + X));  /* 018BD3 LDA */
    A &= 0x00FF;  /* 018BD6 AND */
    A -= 1;  /* 018BD9 DEC */
    X = A;  /* 018BDA TAX */
    A = rd16(DP(0x20));  /* 018BDB LDA */
    mflag = 1;  /* 018BDD SEM */
    wr8((ABS(0x0489) + X), (uint8_t)A);  /* 018BDE STA */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 018BE1 INC */
    SETLO(A, rd8(DP(0x01)));  /* 018BE3 LDA */
    t = rd8(ABS(0x04BE));  /* 018BE5 CMP */
    if ((A & 0xFF) >= t) goto L_018BED;  /* 018BE8 BCS */
    goto L_018BC7;  /* 018BEA JMP */
L_018BED: TICK();
    mflag = 0;  /* 018BED CLM */
    Y = pull16();  /* 018BEE PLY */
    D = pull16();  /* 018BEF PLD */
    S += 3; return;  /* 018BF0 RTL */
}

/* 018BF1  callers: 019369 */
void F_018BF1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018BF1 LDA */
    push16(D);  /* 018BF3 PHD */
    D = A;  /* 018BF4 TAD */
    A += 1;  /* 018BF5 INC */
    A += 1;  /* 018BF6 INC */
    push16(A);  /* 018BF7 PHA */
    mflag = 1;  /* 018BF8 SEM */
    SETLO(B, 0x00);  /* 018BF9 LDA */
    SETLO(A, rd8(ABS(0x0454)));  /* 018BFC LDA */
    div8(rd8(ABS(0x04B9)));  /* 018BFF DIV */
    mflag = 0;  /* 018C03 CLM */
    Y = B;  /* 018C04 TBY */
    S -= 3; F_018AC5();  /* 018C06 JSRL */
    S -= 3; F_018A03();  /* 018C0A JSRL */
    A = rd16(ABS(0x04E7));  /* 018C0E LDA */
    A &= 0x00FF;  /* 018C11 AND */
    wr16(DP(0x02), A);  /* 018C14 STA */
    wr16(DP(0x04), 0x0026);  /* 018C16 LDM */
    wr16(DP(0x06), 0x0060);  /* 018C1A LDM */
    Y = 0x0003;  /* 018C1E LDY */
    A = D;  /* 018C21 TDA */
    C = 0;  /* 018C22 CLC */
    A = adc16(A, 0x0008);  /* 018C23 ADC */
    push16(A);  /* 018C26 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018C27 JSRL */
    Y = pull16();  /* 018C2B PLY */
    mflag = 0;  /* 018C2C CLM */
    S -= 3; F_018700();  /* 018C2D JSRL */
L_018C31: TICK();
    mflag = 1;  /* 018C31 SEM */
    wr8(ABS(WDT), 0x00);  /* 018C32 LDM */
    SETLO(A, rd8(ABS(0x04BA)));  /* 018C36 LDA */
    if ((A & 0xFF) != 0) goto L_018C31;  /* 018C39 BNE */
    mflag = 0;  /* 018C3B CLM */
    X = rd16(ABS(TB2));  /* 018C3C LDX */
    wr16(DP(0x00), X);  /* 018C3F STX */
    X = rd16(ABS(0x04BB));  /* 018C41 LDX */
    /* 018C44 CPX */
    if (X >= 0x0181) goto L_018C69;  /* 018C47 BCS */
L_018C49: TICK();
    mflag = 1;  /* 018C49 SEM */
    if ((rd8(ABS(TB2IC)) & 0x08) == 0x08) goto L_018C52;  /* 018C4A BBS */
    goto L_018C49;  /* 018C4F JMP */
L_018C52: TICK();
    mflag = 0;  /* 018C52 CLM */
    A = rd16(ABS(0x04BB));  /* 018C53 LDA */
    C = 1;  /* 018C56 SEC */
    A = sbc16(A, rd16(ABS(TB2)));  /* 018C57 SBC */
    C = 0;  /* 018C5A CLC */
    A = adc16(A, 0x0FC0);  /* 018C5B ADC */
    /* 018C5E CMP */
    if (A >= 0x0181) goto L_018C66;  /* 018C61 BCS */
    goto L_018C52;  /* 018C63 JMP */
L_018C66: TICK();
    goto L_018C8D;  /* 018C66 JMP */
L_018C69: TICK();
    mflag = 0;  /* 018C69 CLM */
    X = rd16(DP(0x00));  /* 018C6A LDX */
    t = rd16(ABS(TB2));  /* 018C6C CPX */
    if (X < t) goto L_018C79;  /* 018C6F BCC */
    X = rd16(ABS(TB2));  /* 018C71 LDX */
    wr16(DP(0x00), X);  /* 018C74 STX */
    goto L_018C7C;  /* 018C76 JMP */
L_018C79: TICK();
    goto L_018C8D;  /* 018C79 JMP */
L_018C7C: TICK();
    mflag = 0;  /* 018C7C CLM */
    A = rd16(ABS(0x04BB));  /* 018C7D LDA */
    C = 1;  /* 018C80 SEC */
    A = sbc16(A, rd16(ABS(TB2)));  /* 018C81 SBC */
    C = 0;  /* 018C84 CLC */
    A = adc16(A, 0x0FC0);  /* 018C85 ADC */
    /* 018C88 CMP */
    if (A < 0x1141) goto L_018C69;  /* 018C8B BCC */
L_018C8D: TICK();
    mflag = 0;  /* 018C8D CLM */
    A = rd16(ABS(0x0450));  /* 018C8E LDA */
    A &= 0x00FF;  /* 018C91 AND */
    wr16(DP(0x02), A);  /* 018C94 STA */
    wr16(DP(0x04), 0x0000);  /* 018C96 LDM */
    wr16(DP(0x06), 0x0062);  /* 018C9A LDM */
    Y = 0x0003;  /* 018C9E LDY */
    A = D;  /* 018CA1 TDA */
    C = 0;  /* 018CA2 CLC */
    A = adc16(A, 0x0008);  /* 018CA3 ADC */
    push16(A);  /* 018CA6 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018CA7 JSRL */
    Y = pull16();  /* 018CAB PLY */
    SETLO(A, rd8(ABS(0x0451)));  /* 018CAC LDA */
    mflag = 0;  /* 018CAF CLM */
    A &= 0x00FF;  /* 018CB0 AND */
    wr16(DP(0x02), A);  /* 018CB3 STA */
    wr16(DP(0x04), 0x0000);  /* 018CB5 LDM */
    wr16(DP(0x06), 0x0064);  /* 018CB9 LDM */
    Y = 0x0003;  /* 018CBD LDY */
    A = D;  /* 018CC0 TDA */
    C = 0;  /* 018CC1 CLC */
    A = adc16(A, 0x0008);  /* 018CC2 ADC */
    push16(A);  /* 018CC5 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018CC6 JSRL */
    Y = pull16();  /* 018CCA PLY */
    t = (rd8(ABS(0x0450)) - 1) & 0xFF; wr8(ABS(0x0450), t);  /* 018CCB DEC */
    t = (rd8(ABS(0x0451)) + 1) & 0xFF; wr8(ABS(0x0451), t);  /* 018CCE INC */
    t = (rd8(ABS(0x04C4)) - 1) & 0xFF; wr8(ABS(0x04C4), t);  /* 018CD1 DEC */
    SETLO(A, rd8(ABS(0x04C4)));  /* 018CD4 LDA */
    if ((A & 0xFF) != 0) goto L_018CF4;  /* 018CD7 BNE */
    SETLO(A, rd8(ABS(0x0452)));  /* 018CD9 LDA */
    wr8(ABS(0x0450), (uint8_t)A);  /* 018CDC STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 018CDF LDA */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018CE2 STA */
    SETLO(A, rd8(ABS(0x04BD)));  /* 018CE5 LDA */
    wr8(ABS(0x04C4), (uint8_t)A);  /* 018CE8 STA */
    mflag = 0;  /* 018CEB CLM */
    S -= 3; F_018700();  /* 018CEC JSRL */
    mflag = 1;  /* 018CF0 SEM */
    t = (rd8(ABS(0x04C1)) - 1) & 0xFF; wr8(ABS(0x04C1), t);  /* 018CF1 DEC */
L_018CF4: TICK();
    SETLO(A, rd8(ABS(0x0450)));  /* 018CF4 LDA */
    mflag = 0;  /* 018CF7 CLM */
    A &= 0x00FF;  /* 018CF8 AND */
    wr16(DP(0x02), A);  /* 018CFB STA */
    wr16(DP(0x04), 0x0000);  /* 018CFD LDM */
    wr16(DP(0x06), 0x0063);  /* 018D01 LDM */
    Y = 0x0003;  /* 018D05 LDY */
    A = D;  /* 018D08 TDA */
    C = 0;  /* 018D09 CLC */
    A = adc16(A, 0x0008);  /* 018D0A ADC */
    push16(A);  /* 018D0D PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018D0E JSRL */
    Y = pull16();  /* 018D12 PLY */
    SETLO(A, rd8(ABS(0x0451)));  /* 018D13 LDA */
    mflag = 0;  /* 018D16 CLM */
    A &= 0x00FF;  /* 018D17 AND */
    wr16(DP(0x02), A);  /* 018D1A STA */
    wr16(DP(0x04), 0x0000);  /* 018D1C LDM */
    wr16(DP(0x06), 0x0065);  /* 018D20 LDM */
    Y = 0x0003;  /* 018D24 LDY */
    A = D;  /* 018D27 TDA */
    C = 0;  /* 018D28 CLC */
    A = adc16(A, 0x0008);  /* 018D29 ADC */
    push16(A);  /* 018D2C PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018D2D JSRL */
    Y = pull16();  /* 018D31 PLY */
    t = (rd8(ABS(0x0450)) - 1) & 0xFF; wr8(ABS(0x0450), t);  /* 018D32 DEC */
    t = (rd8(ABS(0x0451)) + 1) & 0xFF; wr8(ABS(0x0451), t);  /* 018D35 INC */
    t = (rd8(ABS(0x04C4)) - 1) & 0xFF; wr8(ABS(0x04C4), t);  /* 018D38 DEC */
    SETLO(A, rd8(ABS(0x04C4)));  /* 018D3B LDA */
    if ((A & 0xFF) != 0) goto L_018D5B;  /* 018D3E BNE */
    SETLO(A, rd8(ABS(0x0452)));  /* 018D40 LDA */
    wr8(ABS(0x0450), (uint8_t)A);  /* 018D43 STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 018D46 LDA */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018D49 STA */
    SETLO(A, rd8(ABS(0x04BD)));  /* 018D4C LDA */
    wr8(ABS(0x04C4), (uint8_t)A);  /* 018D4F STA */
    mflag = 0;  /* 018D52 CLM */
    S -= 3; F_018700();  /* 018D53 JSRL */
    mflag = 1;  /* 018D57 SEM */
    t = (rd8(ABS(0x04C1)) - 1) & 0xFF; wr8(ABS(0x04C1), t);  /* 018D58 DEC */
L_018D5B: TICK();
    wr8(ABS(0x04E5), 0x00);  /* 018D5B LDM */
    wr8(ABS(0x0455), 0x04);  /* 018D5F LDM */
    mflag = 0;  /* 018D63 CLM */
    Y = pull16();  /* 018D64 PLY */
    D = pull16();  /* 018D65 PLD */
    S += 3; return;  /* 018D66 RTL */
}

/* 018D67  callers: 01919E, 019247, 019271 */
void F_018D67(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018D67 LDA */
    push16(D);  /* 018D69 PHD */
    D = A;  /* 018D6A TAD */
    C = 0;  /* 018D6B CLC */
    A = adc16(A, 0x0004);  /* 018D6C ADC */
    push16(A);  /* 018D6F PHA */
    mflag = 1;  /* 018D70 SEM */
    wr8(ABS(0x04E6), 0x00);  /* 018D71 LDM */
    mflag = 0;  /* 018D75 CLM */
    wr16(DP(0x00), 0x0004);  /* 018D76 LDM */
    wr16(DP(0x02), 0x0040);  /* 018D7A LDM */
    Y = 0x0002;  /* 018D7E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018D81 JSRL */
    mflag = 0;  /* 018D85 CLM */
    if ((rd16(ABS(0x023E)) & 0x0200) == 0x0200) goto L_018DA6;  /* 018D86 BBS */
    wr16(DP(0x00), 0x0010);  /* 018D8C LDM */
    wr16(DP(0x02), 0x0058);  /* 018D90 LDM */
    wr16(DP(0x04), 0x0060);  /* 018D94 LDM */
    Y = 0x0003;  /* 018D98 LDY */
    A = D;  /* 018D9B TDA */
    C = 0;  /* 018D9C CLC */
    A = adc16(A, 0x0006);  /* 018D9D ADC */
    push16(A);  /* 018DA0 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018DA1 JSRL */
    Y = pull16();  /* 018DA5 PLY */
L_018DA6: TICK();
    mflag = 1;  /* 018DA6 SEM */
    wr8(ABS(0x0455), 0x03);  /* 018DA7 LDM */
    mflag = 0;  /* 018DAB CLM */
    Y = pull16();  /* 018DAC PLY */
    D = pull16();  /* 018DAD PLD */
    S += 3; return;  /* 018DAE RTL */
}

/* 018DAF  callers: 0191DD, 01920A, 0192C1, 0192EE */
void F_018DAF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018DAF LDA */
    push16(D);  /* 018DB1 PHD */
    D = A;  /* 018DB2 TAD */
    C = 0;  /* 018DB3 CLC */
    A = adc16(A, 0x0007);  /* 018DB4 ADC */
    push16(A);  /* 018DB7 PHA */
    wr16(DP(0x01), 0x0010);  /* 018DB8 LDM */
    wr16(DP(0x03), 0x0058);  /* 018DBC LDM */
    wr16(DP(0x05), 0x0060);  /* 018DC0 LDM */
    Y = 0x0003;  /* 018DC4 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018DC7 JSRL */
    mflag = 0;  /* 018DCB CLM */
    wr16(DP(0x01), 0x0004);  /* 018DCC LDM */
    wr16(DP(0x03), 0x0040);  /* 018DD0 LDM */
    Y = 0x0002;  /* 018DD4 LDY */
    A = D;  /* 018DD7 TDA */
    C = 0;  /* 018DD8 CLC */
    A = adc16(A, 0x0005);  /* 018DD9 ADC */
    push16(A);  /* 018DDC PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018DDD JSRL */
    Y = pull16();  /* 018DE1 PLY */
    SETLO(A, rd8(ABS(0x04EB)));  /* 018DE2 LDA */
    /* 018DE5 CMP */
    if ((A & 0xFF) != 0x01) goto L_018DFC;  /* 018DE7 BNE */
    mflag = 0;  /* 018DE9 CLM */
    A = rd16(ABS(0x04CF));  /* 018DEA LDA */
    A &= 0x07C0;  /* 018DED AND */
    A = lsr16(A);  /* 018DF0 LSR */
    A = lsr16(A);  /* 018DF1 LSR */
    A = lsr16(A);  /* 018DF2 LSR */
    A = lsr16(A);  /* 018DF3 LSR */
    A = lsr16(A);  /* 018DF4 LSR */
    A = lsr16(A);  /* 018DF5 LSR */
    mflag = 1;  /* 018DF6 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 018DF7 STA */
    goto L_018E26;  /* 018DF9 JMP */
L_018DFC: TICK();
    mflag = 0;  /* 018DFC CLM */
    A = rd16(ABS(0x04D0));  /* 018DFD LDA */
    A &= 0x00FF;  /* 018E00 AND */
    A = lsr16(A);  /* 018E03 LSR */
    A = lsr16(A);  /* 018E04 LSR */
    A = lsr16(A);  /* 018E05 LSR */
    mflag = 1;  /* 018E06 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 018E07 STA */
    SETLO(B, 0x00);  /* 018E09 LDA */
    SETLO(A, rd8(ABS(0x04CF)));  /* 018E0C LDA */
    div8(0x40);  /* 018E0F DIV */
    wr8(DP(0x01), (uint8_t)A);  /* 018E12 STA */
    mflag = 0;  /* 018E14 CLM */
    A = rd16(ABS(0x04D0));  /* 018E15 LDA */
    A &= 0x00FF;  /* 018E18 AND */
    mflag = 1;  /* 018E1B SEM */
    SETLO(A, A & 0x07);  /* 018E1C AND */
    SETLO(A, asl8(A));  /* 018E1E ASL */
    SETLO(A, asl8(A));  /* 018E1F ASL */
    C = 0;  /* 018E20 CLC */
    SETLO(A, adc8(A, rd8(DP(0x01))));  /* 018E21 ADC */
    wr8(ABS(0x04C7), (uint8_t)A);  /* 018E23 STA */
L_018E26: TICK();
    SETLO(B, 0x00);  /* 018E26 LDA */
    SETLO(A, rd8(DP(0x00)));  /* 018E29 LDA */
    div8(rd8(ABS(0x04B9)));  /* 018E2B DIV */
    mflag = 0;  /* 018E2F CLM */
    Y = B;  /* 018E30 TBY */
    A = D;  /* 018E32 TDA */
    A += 1;  /* 018E33 INC */
    push16(A);  /* 018E34 PHA */
    S -= 3; F_018AC5();  /* 018E35 JSRL */
    Y = pull16();  /* 018E39 PLY */
    A = D;  /* 018E3A TDA */
    A += 1;  /* 018E3B INC */
    push16(A);  /* 018E3C PHA */
    S -= 3; F_018A03();  /* 018E3D JSRL */
    Y = pull16();  /* 018E41 PLY */
    A = D;  /* 018E42 TDA */
    A += 1;  /* 018E43 INC */
    push16(A);  /* 018E44 PHA */
    S -= 3; F_018700();  /* 018E45 JSRL */
    Y = pull16();  /* 018E49 PLY */
    A = rd16(ABS(0x04D1));  /* 018E4A LDA */
    mflag = 1;  /* 018E4D SEM */
    wr8(ABS(0x04E9), (uint8_t)A);  /* 018E4E STA */
    mflag = 0;  /* 018E51 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0020);  /* 018E52 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0400);  /* 018E57 SEB */
    A = D;  /* 018E5C TDA */
    A += 1;  /* 018E5D INC */
    push16(A);  /* 018E5E PHA */
    S -= 3; F_00CB52();  /* 018E5F JSRL */
    Y = pull16();  /* 018E63 PLY */
    wr16(DP(0x01), 0x0038);  /* 018E64 LDM */
    wr16(DP(0x03), 0x0040);  /* 018E68 LDM */
    Y = 0x0002;  /* 018E6C LDY */
    A = D;  /* 018E6F TDA */
    C = 0;  /* 018E70 CLC */
    A = adc16(A, 0x0005);  /* 018E71 ADC */
    push16(A);  /* 018E74 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 018E75 JSRL */
    Y = pull16();  /* 018E79 PLY */
    SETLO(A, rd8(ABS(0x04E7)));  /* 018E7A LDA */
    mflag = 0;  /* 018E7D CLM */
    A &= 0x00FF;  /* 018E7E AND */
    wr16(DP(0x01), A);  /* 018E81 STA */
    wr16(DP(0x03), 0x0026);  /* 018E83 LDM */
    wr16(DP(0x05), 0x0060);  /* 018E87 LDM */
    Y = 0x0003;  /* 018E8B LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018E8E JSRL */
    SETLO(A, rd8(ABS(0x04EB)));  /* 018E92 LDA */
    /* 018E95 CMP */
    if ((A & 0xFF) != 0x01) goto L_018EA2;  /* 018E97 BNE */
    mflag = 0;  /* 018E99 CLM */
    A = D;  /* 018E9A TDA */
    A += 1;  /* 018E9B INC */
    push16(A);  /* 018E9C PHA */
    S -= 3; F_018965();  /* 018E9D JSRL */
    Y = pull16();  /* 018EA1 PLY */
L_018EA2: TICK();
    mflag = 1;  /* 018EA2 SEM */
    wr8(ABS(WDT), 0x00);  /* 018EA3 LDM */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_018EA2;  /* 018EA7 BBC */
    SETLO(A, rd8(ABS(0x0450)));  /* 018EAC LDA */
    mflag = 0;  /* 018EAF CLM */
    A &= 0x00FF;  /* 018EB0 AND */
    wr16(DP(0x01), A);  /* 018EB3 STA */
    wr16(DP(0x03), 0x0000);  /* 018EB5 LDM */
    wr16(DP(0x05), 0x0062);  /* 018EB9 LDM */
    Y = 0x0003;  /* 018EBD LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018EC0 JSRL */
    SETLO(A, rd8(ABS(0x0451)));  /* 018EC4 LDA */
    mflag = 0;  /* 018EC7 CLM */
    A &= 0x00FF;  /* 018EC8 AND */
    wr16(DP(0x01), A);  /* 018ECB STA */
    wr16(DP(0x03), 0x0000);  /* 018ECD LDM */
    wr16(DP(0x05), 0x0064);  /* 018ED1 LDM */
    Y = 0x0003;  /* 018ED5 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018ED8 JSRL */
    mflag = 0;  /* 018EDC CLM */
    A = D;  /* 018EDD TDA */
    A += 1;  /* 018EDE INC */
    push16(A);  /* 018EDF PHA */
    S -= 3; F_0189E9();  /* 018EE0 JSRL */
    Y = pull16();  /* 018EE4 PLY */
    mflag = 1;  /* 018EE5 SEM */
    t = (rd8(ABS(0x0450)) - 1) & 0xFF; wr8(ABS(0x0450), t);  /* 018EE6 DEC */
    t = (rd8(ABS(0x0451)) + 1) & 0xFF; wr8(ABS(0x0451), t);  /* 018EE9 INC */
    t = (rd8(ABS(0x04C4)) - 1) & 0xFF; wr8(ABS(0x04C4), t);  /* 018EEC DEC */
    SETLO(A, rd8(ABS(0x04C4)));  /* 018EEF LDA */
    if ((A & 0xFF) != 0) goto L_018F0F;  /* 018EF2 BNE */
    SETLO(A, rd8(ABS(0x0452)));  /* 018EF4 LDA */
    wr8(ABS(0x0450), (uint8_t)A);  /* 018EF7 STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 018EFA LDA */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018EFD STA */
    SETLO(A, rd8(ABS(0x04BD)));  /* 018F00 LDA */
    wr8(ABS(0x04C4), (uint8_t)A);  /* 018F03 STA */
    mflag = 0;  /* 018F06 CLM */
    A = D;  /* 018F07 TDA */
    A += 1;  /* 018F08 INC */
    push16(A);  /* 018F09 PHA */
    S -= 3; F_018700();  /* 018F0A JSRL */
    Y = pull16();  /* 018F0E PLY */
L_018F0F: TICK();
    mflag = 0;  /* 018F0F CLM */
    A = rd16(ABS(0x0450));  /* 018F10 LDA */
    A &= 0x00FF;  /* 018F13 AND */
    wr16(DP(0x01), A);  /* 018F16 STA */
    wr16(DP(0x03), 0x0000);  /* 018F18 LDM */
    wr16(DP(0x05), 0x0063);  /* 018F1C LDM */
    Y = 0x0003;  /* 018F20 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018F23 JSRL */
    SETLO(A, rd8(ABS(0x0451)));  /* 018F27 LDA */
    mflag = 0;  /* 018F2A CLM */
    A &= 0x00FF;  /* 018F2B AND */
    wr16(DP(0x01), A);  /* 018F2E STA */
    wr16(DP(0x03), 0x0000);  /* 018F30 LDM */
    wr16(DP(0x05), 0x0065);  /* 018F34 LDM */
    Y = 0x0003;  /* 018F38 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018F3B JSRL */
    t = (rd8(ABS(0x0450)) - 1) & 0xFF; wr8(ABS(0x0450), t);  /* 018F3F DEC */
    t = (rd8(ABS(0x0451)) + 1) & 0xFF; wr8(ABS(0x0451), t);  /* 018F42 INC */
    t = (rd8(ABS(0x04C4)) - 1) & 0xFF; wr8(ABS(0x04C4), t);  /* 018F45 DEC */
    SETLO(A, rd8(ABS(0x04C4)));  /* 018F48 LDA */
    if ((A & 0xFF) != 0) goto L_018F68;  /* 018F4B BNE */
    SETLO(A, rd8(ABS(0x0452)));  /* 018F4D LDA */
    wr8(ABS(0x0450), (uint8_t)A);  /* 018F50 STA */
    SETLO(A, rd8(ABS(0x0453)));  /* 018F53 LDA */
    wr8(ABS(0x0451), (uint8_t)A);  /* 018F56 STA */
    SETLO(A, rd8(ABS(0x04BD)));  /* 018F59 LDA */
    wr8(ABS(0x04C4), (uint8_t)A);  /* 018F5C STA */
    mflag = 0;  /* 018F5F CLM */
    A = D;  /* 018F60 TDA */
    A += 1;  /* 018F61 INC */
    push16(A);  /* 018F62 PHA */
    S -= 3; F_018700();  /* 018F63 JSRL */
    Y = pull16();  /* 018F67 PLY */
L_018F68: TICK();
    mflag = 0;  /* 018F68 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) | 0x0400);  /* 018F69 SEB */
    mflag = 1;  /* 018F6E SEM */
    wr8(ABS(0x0455), 0x02);  /* 018F6F LDM */
    mflag = 0;  /* 018F73 CLM */
    Y = pull16();  /* 018F74 PLY */
    D = pull16();  /* 018F75 PLD */
    S += 3; return;  /* 018F76 RTL */
}

/* 018F77  callers: 019183, 01922C, 019269 */
void F_018F77(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018F77 LDA */
    push16(D);  /* 018F79 PHD */
    D = A;  /* 018F7A TAD */
    C = 0;  /* 018F7B CLC */
    A = adc16(A, 0x0006);  /* 018F7C ADC */
    push16(A);  /* 018F7F PHA */
    mflag = 1;  /* 018F80 SEM */
    wr8(ABS(0x04E6), 0x01);  /* 018F81 LDM */
    SETLO(A, rd8(ABS(0x04BF)));  /* 018F85 LDA */
    mpy8(0x05);  /* 018F88 MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 018F8B STA */
    wr8(DP(0x01), (uint8_t)B);  /* 018F8D STA */
    mflag = 0;  /* 018F90 CLM */
    A = rd16(DP(0x00));  /* 018F91 LDA */
    B = 0x0020;  /* 018F93 LDA */
    S -= 3; F_00FF8C();  /* 018F97 JSRL */
    mflag = 1;  /* 018F9B SEM */
    SETLO(A, A + 1);  /* 018F9C INC */
    wr8(ABS(0x04C0), (uint8_t)A);  /* 018F9D STA */
    mflag = 0;  /* 018FA0 CLM */
    wr16(DP(0x00), 0x0010);  /* 018FA1 LDM */
    wr16(DP(0x02), 0x0058);  /* 018FA5 LDM */
    wr16(DP(0x04), 0x0060);  /* 018FA9 LDM */
    Y = 0x0003;  /* 018FAD LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 018FB0 JSRL */
    wr8(ABS(0x0455), 0x06);  /* 018FB4 LDM */
    mflag = 0;  /* 018FB8 CLM */
    Y = pull16();  /* 018FB9 PLY */
    D = pull16();  /* 018FBA PLD */
    S += 3; return;  /* 018FBB RTL */
}

/* 018FBC  callers: 01937C */
void F_018FBC(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 018FBC LDA */
    push16(D);  /* 018FBE PHD */
    D = A;  /* 018FBF TAD */
    C = 0;  /* 018FC0 CLC */
    A = adc16(A, 0x0005);  /* 018FC1 ADC */
    push16(A);  /* 018FC4 PHA */
    mflag = 1;  /* 018FC5 SEM */
    wr8(DP(0x02), 0x01);  /* 018FC6 LDM */
    mflag = 0;  /* 018FC9 CLM */
    A = 0x0001;  /* 018FCA LDA */
    A &= 0x001F;  /* 018FCD AND */
    A = lsr16(A);  /* 018FD0 LSR */
    A = ror16(A);  /* 018FD1 ROR */
    A = ror16(A);  /* 018FD2 ROR */
    A = ror16(A);  /* 018FD3 ROR */
    A = ror16(A);  /* 018FD4 ROR */
    A = ror16(A);  /* 018FD5 ROR */
    wr16(DP(0x00), A);  /* 018FD6 STA */
    A = rd16(ABS(0x0457));  /* 018FD8 LDA */
    mflag = 1;  /* 018FDB SEM */
    SETLO(A, A & 0x1F);  /* 018FDC AND */
    wr8(ABS(0x0454), (uint8_t)A);  /* 018FDE STA */
    mflag = 0;  /* 018FE1 CLM */
    A &= 0x00FF;  /* 018FE2 AND */
    A = asl16(A);  /* 018FE5 ASL */
    A = asl16(A);  /* 018FE6 ASL */
    A = asl16(A);  /* 018FE7 ASL */
    A = asl16(A);  /* 018FE8 ASL */
    A = asl16(A);  /* 018FE9 ASL */
    A = asl16(A);  /* 018FEA ASL */
    A |= rd16(DP(0x00));  /* 018FEB ORA */
    A &= 0x7FC0;  /* 018FED AND */
    Y = A;  /* 018FF0 TAY */
    wr16(DP(0x00), Y);  /* 018FF1 STY */
    S -= 3; F_01844E();  /* 018FF3 JSRL */
    wr16(DP(0x03), A);  /* 018FF7 STA */
    A = rla16(A, 0x0008);  /* 018FF9 RLA */
    A &= 0x00FF;  /* 018FFD AND */
    mflag = 1;  /* 019000 SEM */
    wr8(ABS(0x04D9), (uint8_t)A);  /* 019001 STA */
    SETLO(A, rd8(DP(0x03)));  /* 019004 LDA */
    wr8(ABS(0x04DA), (uint8_t)A);  /* 019006 STA */
    mflag = 0;  /* 019009 CLM */
    wr16(DP(0x00), rd16(DP(0x00)) | 0x8000);  /* 01900A SEB */
    Y = rd16(DP(0x00));  /* 01900E LDY */
    S -= 3; F_01844E();  /* 019010 JSRL */
    wr16(DP(0x03), A);  /* 019014 STA */
    A = rla16(A, 0x0008);  /* 019016 RLA */
    A &= 0x00FF;  /* 01901A AND */
    mflag = 1;  /* 01901D SEM */
    wr8(ABS(0x04DE), (uint8_t)A);  /* 01901E STA */
    SETLO(A, rd8(DP(0x03)));  /* 019021 LDA */
    wr8(ABS(0x04DF), (uint8_t)A);  /* 019023 STA */
    mflag = 0;  /* 019026 CLM */
    wr16(DP(0x05), 0x0009);  /* 019027 LDM */
    wr16(DP(0x07), 0x0040);  /* 01902B LDM */
    Y = 0x0002;  /* 01902F LDY */
    A = D;  /* 019032 TDA */
    C = 0;  /* 019033 CLC */
    A = adc16(A, 0x0009);  /* 019034 ADC */
    push16(A);  /* 019037 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 019038 JSRL */
    Y = pull16();  /* 01903C PLY */
    SETLO(A, rd8(ABS(0x04D3)));  /* 01903D LDA */
    mflag = 0;  /* 019040 CLM */
    A &= 0x00FF;  /* 019041 AND */
    wr16(DP(0x05), A);  /* 019044 STA */
    wr16(DP(0x07), 0x0043);  /* 019046 LDM */
    Y = 0x0002;  /* 01904A LDY */
    A = D;  /* 01904D TDA */
    C = 0;  /* 01904E CLC */
    A = adc16(A, 0x0009);  /* 01904F ADC */
    push16(A);  /* 019052 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 019053 JSRL */
    Y = pull16();  /* 019057 PLY */
    mflag = 0;  /* 019058 CLM */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0002);  /* 019059 CLB */
    mflag = 1;  /* 01905E SEM */
    wr8(ABS(0x04C5), 0x01);  /* 01905F LDM */
    wr8(ABS(0x04E5), 0x00);  /* 019063 LDM */
    wr8(ABS(0x0455), 0x05);  /* 019067 LDM */
    mflag = 0;  /* 01906B CLM */
    Y = pull16();  /* 01906C PLY */
    D = pull16();  /* 01906D PLD */
    S += 3; return;  /* 01906E RTL */
}

/* 01906F  callers: 018337, 01879A, 019168, 01934F */
void F_01906F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01906F LDA */
    push16(D);  /* 019071 PHD */
    D = A;  /* 019072 TAD */
    push16(A);  /* 019073 PHA */
    mflag = 1;  /* 019074 SEM */
    wr8(ABS(0x04C5), 0x00);  /* 019075 LDM */
    mflag = 0;  /* 019079 CLM */
    S -= 3; F_0189E9();  /* 01907A JSRL */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0400);  /* 01907E CLB */
    S -= 3; F_00CB52();  /* 019083 JSRL */
    wr16(DP(0x00), 0x0010);  /* 019087 LDM */
    wr16(DP(0x02), 0x0058);  /* 01908B LDM */
    wr16(DP(0x04), 0x0060);  /* 01908F LDM */
    Y = 0x0003;  /* 019093 LDY */
    A = D;  /* 019096 TDA */
    C = 0;  /* 019097 CLC */
    A = adc16(A, 0x0006);  /* 019098 ADC */
    push16(A);  /* 01909B PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01909C JSRL */
    Y = pull16();  /* 0190A0 PLY */
    mflag = 0;  /* 0190A1 CLM */
    wr16(DP(0x00), 0x0004);  /* 0190A2 LDM */
    wr16(DP(0x02), 0x0040);  /* 0190A6 LDM */
    Y = 0x0002;  /* 0190AA LDY */
    A = D;  /* 0190AD TDA */
    C = 0;  /* 0190AE CLC */
    A = adc16(A, 0x0004);  /* 0190AF ADC */
    push16(A);  /* 0190B2 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0190B3 JSRL */
    Y = pull16();  /* 0190B7 PLY */
    mflag = 0;  /* 0190B8 CLM */
    wr16(DP(0x00), 0x0038);  /* 0190B9 LDM */
    wr16(DP(0x02), 0x0040);  /* 0190BD LDM */
    Y = 0x0002;  /* 0190C1 LDY */
    A = D;  /* 0190C4 TDA */
    C = 0;  /* 0190C5 CLC */
    A = adc16(A, 0x0004);  /* 0190C6 ADC */
    push16(A);  /* 0190C9 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0190CA JSRL */
    Y = pull16();  /* 0190CE PLY */
    mflag = 0;  /* 0190CF CLM */
    wr16(DP(0x00), 0x00D7);  /* 0190D0 LDM */
    wr16(DP(0x02), 0x00C4);  /* 0190D4 LDM */
    wr16(DP(0x04), 0x0044);  /* 0190D8 LDM */
    Y = 0x0003;  /* 0190DC LDY */
    A = D;  /* 0190DF TDA */
    C = 0;  /* 0190E0 CLC */
    A = adc16(A, 0x0006);  /* 0190E1 ADC */
    push16(A);  /* 0190E4 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0190E5 JSRL */
    Y = pull16();  /* 0190E9 PLY */
    wr8(ABS(0x0455), 0x01);  /* 0190EA LDM */
    mflag = 0;  /* 0190EE CLM */
    Y = pull16();  /* 0190EF PLY */
    D = pull16();  /* 0190F0 PLD */
    S += 3; return;  /* 0190F1 RTL */
}

/* 0190F2  callers: 0191B8, 0191E5, 019212, 019285, 019298, 0192C9, 0192F6, 01930F ... */
void F_0190F2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0190F2 LDA */
    push16(D);  /* 0190F4 PHD */
    D = A;  /* 0190F5 TAD */
    push16(A);  /* 0190F6 PHA */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0400);  /* 0190F7 CLB */
    S -= 3; F_00CB52();  /* 0190FC JSRL */
    S -= 3; F_0189E9();  /* 019100 JSRL */
    if ((rd16(ABS(0x023E)) & 0x0200) == 0x0200) goto L_019124;  /* 019104 BBS */
    wr16(DP(0x00), 0x0010);  /* 01910A LDM */
    wr16(DP(0x02), 0x0058);  /* 01910E LDM */
    wr16(DP(0x04), 0x0060);  /* 019112 LDM */
    Y = 0x0003;  /* 019116 LDY */
    A = D;  /* 019119 TDA */
    C = 0;  /* 01911A CLC */
    A = adc16(A, 0x0006);  /* 01911B ADC */
    push16(A);  /* 01911E PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01911F JSRL */
    Y = pull16();  /* 019123 PLY */
L_019124: TICK();
    mflag = 0;  /* 019124 CLM */
    wr16(DP(0x00), 0x0004);  /* 019125 LDM */
    wr16(DP(0x02), 0x0040);  /* 019129 LDM */
    Y = 0x0002;  /* 01912D LDY */
    A = D;  /* 019130 TDA */
    C = 0;  /* 019131 CLC */
    A = adc16(A, 0x0004);  /* 019132 ADC */
    push16(A);  /* 019135 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 019136 JSRL */
    Y = pull16();  /* 01913A PLY */
    wr8(ABS(0x04C5), 0x00);  /* 01913B LDM */
    wr8(ABS(0x04E6), 0x00);  /* 01913F LDM */
    wr8(ABS(0x0455), 0x00);  /* 019143 LDM */
    mflag = 0;  /* 019147 CLM */
    Y = pull16();  /* 019148 PLY */
    D = pull16();  /* 019149 PLD */
    S += 3; return;  /* 01914A RTL */
}

/* 01914B  callers: 01A7BC */
void F_01914B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01914B LDA */
    push16(A);  /* 01914D PHA */
    mflag = 1;  /* 01914E SEM */
    if ((rd8(ABS(0x0334)) & 0x04) == 0x04) goto L_019157;  /* 01914F BBS */
    goto L_019380;  /* 019154 JMP */
L_019157: TICK();
    wr8(ABS(WDT), 0x00);  /* 019157 LDM */
    SETLO(A, rd8(ABS(0x0455)));  /* 01915B LDA */
    if ((A & 0xFF) != 0) goto L_0191A2;  /* 01915E BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 019160 LDA */
    /* 019163 CMP */
    if ((A & 0xFF) != 0x03) goto L_01916C;  /* 019165 BNE */
    mflag = 0;  /* 019167 CLM */
    S -= 3; F_01906F();  /* 019168 JSRL */
L_01916C: TICK();
    mflag = 0;  /* 01916C CLM */
    A = rd16(ABS(0x0228));  /* 01916D LDA */
    mflag = 1;  /* 019170 SEM */
    /* 019171 CMP */
    if ((A & 0xFF) == 0x08) goto L_01917C;  /* 019173 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 019175 LDA */
    /* 019178 CMP */
    if ((A & 0xFF) != 0x09) goto L_019187;  /* 01917A BNE */
L_01917C: TICK();
    mflag = 0;  /* 01917C CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_019187;  /* 01917D BBC */
    S -= 3; F_018F77();  /* 019183 JSRL */
L_019187: TICK();
    mflag = 0;  /* 019187 CLM */
    A = rd16(ABS(0x0228));  /* 019188 LDA */
    mflag = 1;  /* 01918B SEM */
    /* 01918C CMP */
    if ((A & 0xFF) == 0x08) goto L_019197;  /* 01918E BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 019190 LDA */
    /* 019193 CMP */
    if ((A & 0xFF) != 0x09) goto L_0191A2;  /* 019195 BNE */
L_019197: TICK();
    mflag = 0;  /* 019197 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_0191A2;  /* 019198 BBS */
    S -= 3; F_018D67();  /* 01919E JSRL */
L_0191A2: TICK();
    mflag = 0;  /* 0191A2 CLM */
    A = rd16(ABS(0x0455));  /* 0191A3 LDA */
    mflag = 1;  /* 0191A6 SEM */
    /* 0191A7 CMP */
    if ((A & 0xFF) == 0x01) goto L_0191AE;  /* 0191A9 BEQ */
    goto L_01924B;  /* 0191AB JMP */
L_0191AE: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 0191AE LDA */
    if ((A & 0xFF) == 0) goto L_0191B7;  /* 0191B1 BEQ */
    /* 0191B3 CMP */
    if ((A & 0xFF) != 0x02) goto L_0191BF;  /* 0191B5 BNE */
L_0191B7: TICK();
    mflag = 0;  /* 0191B7 CLM */
    S -= 3; F_0190F2();  /* 0191B8 JSRL */
    goto L_019216;  /* 0191BC JMP */
L_0191BF: TICK();
    mflag = 0;  /* 0191BF CLM */
    if ((rd16(ABS(0x0248)) & 0x0001) == 0) goto L_0191EC;  /* 0191C0 BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0001);  /* 0191C6 CLB */
    mflag = 1;  /* 0191CB SEM */
    wr8(ABS(0x04EB), 0x01);  /* 0191CC LDM */
    mflag = 0;  /* 0191D0 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_0191DD;  /* 0191D1 BBS */
    if ((rd16(ABS(0x0248)) & 0x2000) == 0) goto L_0191E4;  /* 0191D7 BBC */
L_0191DD: TICK();
    S -= 3; F_018DAF();  /* 0191DD JSRL */
    goto L_0191E9;  /* 0191E1 JMP */
L_0191E4: TICK();
    mflag = 0;  /* 0191E4 CLM */
    S -= 3; F_0190F2();  /* 0191E5 JSRL */
L_0191E9: TICK();
    goto L_019216;  /* 0191E9 JMP */
L_0191EC: TICK();
    mflag = 0;  /* 0191EC CLM */
    if ((rd16(ABS(0x0248)) & 0x1000) == 0) goto L_019216;  /* 0191ED BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x1000);  /* 0191F3 CLB */
    mflag = 1;  /* 0191F8 SEM */
    wr8(ABS(0x04EB), 0x00);  /* 0191F9 LDM */
    mflag = 0;  /* 0191FD CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_01920A;  /* 0191FE BBS */
    if ((rd16(ABS(0x0248)) & 0x2000) == 0) goto L_019211;  /* 019204 BBC */
L_01920A: TICK();
    S -= 3; F_018DAF();  /* 01920A JSRL */
    goto L_019216;  /* 01920E JMP */
L_019211: TICK();
    mflag = 0;  /* 019211 CLM */
    S -= 3; F_0190F2();  /* 019212 JSRL */
L_019216: TICK();
    A = rd16(ABS(0x0228));  /* 019216 LDA */
    mflag = 1;  /* 019219 SEM */
    /* 01921A CMP */
    if ((A & 0xFF) == 0x08) goto L_019225;  /* 01921C BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 01921E LDA */
    /* 019221 CMP */
    if ((A & 0xFF) != 0x09) goto L_019230;  /* 019223 BNE */
L_019225: TICK();
    mflag = 0;  /* 019225 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_019230;  /* 019226 BBC */
    S -= 3; F_018F77();  /* 01922C JSRL */
L_019230: TICK();
    mflag = 0;  /* 019230 CLM */
    A = rd16(ABS(0x0228));  /* 019231 LDA */
    mflag = 1;  /* 019234 SEM */
    /* 019235 CMP */
    if ((A & 0xFF) == 0x08) goto L_019240;  /* 019237 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 019239 LDA */
    /* 01923C CMP */
    if ((A & 0xFF) != 0x09) goto L_01924B;  /* 01923E BNE */
L_019240: TICK();
    mflag = 0;  /* 019240 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_01924B;  /* 019241 BBS */
    S -= 3; F_018D67();  /* 019247 JSRL */
L_01924B: TICK();
    mflag = 0;  /* 01924B CLM */
    A = rd16(ABS(0x0455));  /* 01924C LDA */
    mflag = 1;  /* 01924F SEM */
    /* 019250 CMP */
    if ((A & 0xFF) == 0x02) goto L_019257;  /* 019252 BEQ */
    goto L_0192FA;  /* 019254 JMP */
L_019257: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 019257 LDA */
    /* 01925A CMP */
    if ((A & 0xFF) == 0x08) goto L_019262;  /* 01925C BEQ */
    /* 01925E CMP */
    if ((A & 0xFF) != 0x09) goto L_019278;  /* 019260 BNE */
L_019262: TICK();
    mflag = 0;  /* 019262 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_019270;  /* 019263 BBC */
    S -= 3; F_018F77();  /* 019269 JSRL */
    goto L_019275;  /* 01926D JMP */
L_019270: TICK();
    mflag = 0;  /* 019270 CLM */
    S -= 3; F_018D67();  /* 019271 JSRL */
L_019275: TICK();
    goto L_0192FA;  /* 019275 JMP */
L_019278: TICK();
    mflag = 0;  /* 019278 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_019289;  /* 019279 BBS */
    if ((rd16(ABS(0x0248)) & 0x2000) == 0x2000) goto L_019289;  /* 01927F BBS */
    S -= 3; F_0190F2();  /* 019285 JSRL */
L_019289: TICK();
    if ((rd16(ABS(0x0248)) & 0x0010) == 0) goto L_0192A4;  /* 019289 BBC */
    S -= 3; F_01842F();  /* 01928F JSRL */
    /* 019293 CMP */
    if ((A & 0xFF) != 0x00) goto L_01929F;  /* 019295 BNE */
    mflag = 0;  /* 019297 CLM */
    S -= 3; F_0190F2();  /* 019298 JSRL */
    goto L_0192A4;  /* 01929C JMP */
L_01929F: TICK();
    mflag = 0;  /* 01929F CLM */
    S -= 3; F_018700();  /* 0192A0 JSRL */
L_0192A4: TICK();
    if ((rd16(ABS(0x0248)) & 0x0001) == 0) goto L_0192D0;  /* 0192A4 BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x0001);  /* 0192AA CLB */
    mflag = 1;  /* 0192AF SEM */
    wr8(ABS(0x04EB), 0x01);  /* 0192B0 LDM */
    mflag = 0;  /* 0192B4 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_0192C1;  /* 0192B5 BBS */
    if ((rd16(ABS(0x0248)) & 0x2000) == 0) goto L_0192C8;  /* 0192BB BBC */
L_0192C1: TICK();
    S -= 3; F_018DAF();  /* 0192C1 JSRL */
    goto L_0192CD;  /* 0192C5 JMP */
L_0192C8: TICK();
    mflag = 0;  /* 0192C8 CLM */
    S -= 3; F_0190F2();  /* 0192C9 JSRL */
L_0192CD: TICK();
    goto L_0192FA;  /* 0192CD JMP */
L_0192D0: TICK();
    mflag = 0;  /* 0192D0 CLM */
    if ((rd16(ABS(0x0248)) & 0x1000) == 0) goto L_0192FA;  /* 0192D1 BBC */
    wr16(ABS(0x0248), rd16(ABS(0x0248)) & ~0x1000);  /* 0192D7 CLB */
    mflag = 1;  /* 0192DC SEM */
    wr8(ABS(0x04EB), 0x00);  /* 0192DD LDM */
    mflag = 0;  /* 0192E1 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_0192EE;  /* 0192E2 BBS */
    if ((rd16(ABS(0x0248)) & 0x2000) == 0) goto L_0192F5;  /* 0192E8 BBC */
L_0192EE: TICK();
    S -= 3; F_018DAF();  /* 0192EE JSRL */
    goto L_0192FA;  /* 0192F2 JMP */
L_0192F5: TICK();
    mflag = 0;  /* 0192F5 CLM */
    S -= 3; F_0190F2();  /* 0192F6 JSRL */
L_0192FA: TICK();
    mflag = 0;  /* 0192FA CLM */
    A = rd16(ABS(0x0455));  /* 0192FB LDA */
    mflag = 1;  /* 0192FE SEM */
    /* 0192FF CMP */
    if ((A & 0xFF) != 0x03) goto L_019313;  /* 019301 BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 019303 LDA */
    /* 019306 CMP */
    if ((A & 0xFF) == 0x08) goto L_019313;  /* 019308 BEQ */
    /* 01930A CMP */
    if ((A & 0xFF) == 0x09) goto L_019313;  /* 01930C BEQ */
    mflag = 0;  /* 01930E CLM */
    S -= 3; F_0190F2();  /* 01930F JSRL */
L_019313: TICK();
    mflag = 0;  /* 019313 CLM */
    A = rd16(ABS(0x0455));  /* 019314 LDA */
    mflag = 1;  /* 019317 SEM */
    /* 019318 CMP */
    if ((A & 0xFF) != 0x04) goto L_01932C;  /* 01931A BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 01931C LDA */
    /* 01931F CMP */
    if ((A & 0xFF) == 0x08) goto L_01932C;  /* 019321 BEQ */
    /* 019323 CMP */
    if ((A & 0xFF) == 0x09) goto L_01932C;  /* 019325 BEQ */
    mflag = 0;  /* 019327 CLM */
    S -= 3; F_0190F2();  /* 019328 JSRL */
L_01932C: TICK();
    mflag = 0;  /* 01932C CLM */
    A = rd16(ABS(0x0455));  /* 01932D LDA */
    mflag = 1;  /* 019330 SEM */
    /* 019331 CMP */
    if ((A & 0xFF) != 0x05) goto L_01936D;  /* 019333 BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 019335 LDA */
    /* 019338 CMP */
    if ((A & 0xFF) == 0x08) goto L_019345;  /* 01933A BEQ */
    /* 01933C CMP */
    if ((A & 0xFF) == 0x09) goto L_019345;  /* 01933E BEQ */
    mflag = 0;  /* 019340 CLM */
    S -= 3; F_0190F2();  /* 019341 JSRL */
L_019345: TICK();
    mflag = 0;  /* 019345 CLM */
    A = rd16(ABS(0x0228));  /* 019346 LDA */
    mflag = 1;  /* 019349 SEM */
    /* 01934A CMP */
    if ((A & 0xFF) != 0x03) goto L_019353;  /* 01934C BNE */
    mflag = 0;  /* 01934E CLM */
    S -= 3; F_01906F();  /* 01934F JSRL */
L_019353: TICK();
    mflag = 0;  /* 019353 CLM */
    A = rd16(ABS(0x0228));  /* 019354 LDA */
    mflag = 1;  /* 019357 SEM */
    /* 019358 CMP */
    if ((A & 0xFF) == 0x08) goto L_019363;  /* 01935A BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 01935C LDA */
    /* 01935F CMP */
    if ((A & 0xFF) != 0x09) goto L_01936D;  /* 019361 BNE */
L_019363: TICK();
    SETLO(A, rd8(ABS(0x04E5)));  /* 019363 LDA */
    if ((A & 0xFF) == 0) goto L_01936D;  /* 019366 BEQ */
    mflag = 0;  /* 019368 CLM */
    S -= 3; F_018BF1();  /* 019369 JSRL */
L_01936D: TICK();
    mflag = 0;  /* 01936D CLM */
    A = rd16(ABS(0x0455));  /* 01936E LDA */
    mflag = 1;  /* 019371 SEM */
    /* 019372 CMP */
    if ((A & 0xFF) != 0x06) goto L_019380;  /* 019374 BNE */
    SETLO(A, rd8(ABS(0x04C0)));  /* 019376 LDA */
    if ((A & 0xFF) != 0) goto L_019380;  /* 019379 BNE */
    mflag = 0;  /* 01937B CLM */
    S -= 3; F_018FBC();  /* 01937C JSRL */
L_019380: TICK();
    mflag = 0;  /* 019380 CLM */
    Y = pull16();  /* 019381 PLY */
    S += 3; return;  /* 019382 RTL */
}

/* 019383  callers: 019BD3, 019C0C */
void F_019383(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 019383 LDA */
    push16(D);  /* 019385 PHD */
    D = A;  /* 019386 TAD */
    C = 0;  /* 019387 CLC */
    A = adc16(A, 0x000C);  /* 019388 ADC */
    push16(A);  /* 01938B PHA */
    wr16(DP(0x03), Y);  /* 01938C STY */
    A = Y;  /* 01938E TYA */
    mflag = 1;  /* 01938F SEM */
    /* 019390 CMP */
    if ((A & 0xFF) != 0x01) goto L_0193A4;  /* 019392 BNE */
    mflag = 0;  /* 019394 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x2000);  /* 019395 CLB */
    A = rd16(LIND(0x00));  /* 01939A LDA */
    Y = A;  /* 01939C TAY */
    S -= 3; F_008D14();  /* 01939D JSRL */
    goto L_019443;  /* 0193A1 JMP */
L_0193A4: TICK();
    mflag = 0;  /* 0193A4 CLM */
    A = rd16(DP(0x03));  /* 0193A5 LDA */
    mflag = 1;  /* 0193A7 SEM */
    /* 0193A8 CMP */
    if ((A & 0xFF) == 0x02) goto L_0193AF;  /* 0193AA BEQ */
    goto L_019443;  /* 0193AC JMP */
L_0193AF: TICK();
    Y = 0x0002;  /* 0193AF LDY */
    mflag = 0;  /* 0193B2 CLM */
    A = rd16(LIND(0x00));  /* 0193B3 LDA */
    B = rd16((LIND(0x00) + Y));  /* 0193B5 LDA */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x2000);  /* 0193B8 SEB */
    Y = 0x007D;  /* 0193BD LDY */
    wr16(DP(0x08), A);  /* 0193C0 STA */
    wr16(DP(0x0A), B);  /* 0193C2 STA */
    S -= 3; F_008854();  /* 0193C5 JSRL */
    wr16(ABS(0x0373), A);  /* 0193C9 STA */
    wr16(ABS(0x037A), A);  /* 0193CC STA */
    A = rd16(ABS(0x01F9));  /* 0193CF LDA */
    B = 0x0005;  /* 0193D2 LDA */
    S -= 3; F_00FF8C();  /* 0193D6 JSRL */
    B = 0x0000;  /* 0193DA LDA */
    /* 0193DE CMP */
    if (A < 0x8000) goto L_0193E5;  /* 0193E1 BCC */
    B -= 1;  /* 0193E3 DEC */
L_0193E5: TICK();
    C = 0;  /* 0193E5 CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 0193E6 ADC */
    B = adc16(B, rd16(DP(0x0A)));  /* 0193E8 ADC */
    wr16(DP(0x04), A);  /* 0193EB STA */
    wr16(DP(0x06), B);  /* 0193ED STA */
    Y = A;  /* 0193F0 TAY */
    S -= 3; F_0087DE();  /* 0193F1 JSRL */
    wr16(ABS(0x0370), A);  /* 0193F5 STA */
    B = rd16(DP(0x06));  /* 0193F8 LDA */
    Y = rd16(DP(0x04));  /* 0193FB LDY */
    S -= 3; F_008825();  /* 0193FD JSRL */
    wr8(ABS(0x0372), (uint8_t)A);  /* 019401 STA */
    wr8(ABS(0x0375), rd8(ABS(0x0375)) & ~0x40);  /* 019404 CLB */
    mflag = 0;  /* 019408 CLM */
    B = rd16(DP(0x0A));  /* 019409 LDA */
    Y = rd16(DP(0x08));  /* 01940C LDY */
    S -= 3; F_0087DE();  /* 01940E JSRL */
    wr16(ABS(0x0377), A);  /* 019412 STA */
    B = rd16(DP(0x0A));  /* 019415 LDA */
    Y = rd16(DP(0x08));  /* 019418 LDY */
    S -= 3; F_008825();  /* 01941A JSRL */
    wr8(ABS(0x0379), (uint8_t)A);  /* 01941E STA */
    mflag = 0;  /* 019421 CLM */
    A = rd16(ABS(0x0224));  /* 019422 LDA */
    A = lsr16(A);  /* 019425 LSR */
    A = lsr16(A);  /* 019426 LSR */
    wr16(DP(0x0E), 0x0000);  /* 019427 LDM */
    wr16(DP(0x0C), 0x0008);  /* 01942B LDM */
    wr16(DP(0x10), A);  /* 01942F STA */
    A = rd16(DP(0x08));  /* 019431 LDA */
    B = rd16(DP(0x0A));  /* 019433 LDA */
    X = 0x000C;  /* 019436 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 019439 JSRL */
    C = 1;  /* 01943D SEC */
    A = sbc16(A, rd16(DP(0x10)));  /* 01943E SBC */
    wr16(ABS(0x0220), A);  /* 019440 STA */
L_019443: TICK();
    mflag = 0;  /* 019443 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 019444 CLB */
    wr16(DP(0x0C), 0x0370);  /* 019449 LDM */
    wr16(DP(0x0E), 0x0363);  /* 01944D LDM */
    A = 0x000C;  /* 019451 LDA */
    Y = 0x000D;  /* 019454 LDY */
    S -= 3; rt_memcpy_01E466();  /* 019457 JSRL */
    Y = 0x002C;  /* 01945B LDY */
    S -= 3; F_003310();  /* 01945E JSRL */
    S -= 3; F_00828C();  /* 019462 JSRL */
    Y = pull16();  /* 019466 PLY */
    D = pull16();  /* 019467 PLD */
    S += 3; return;  /* 019468 RTL */
}

/* 0195DA  callers: 01A418, 01A447, 01D2B2, 01D320, 01D342, 01D588, 01D81A, 01D9E2 */
void F_0195DA(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 0195DA SEM */
    wr8(ABS(0x0111), 0x14);  /* 0195DB LDM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) | 0x02);  /* 0195DF SEB */
    mflag = 0;  /* 0195E3 CLM */
    S += 3; return;  /* 0195E4 RTL */
}

/* 0195E5  callers: 01984B, 0199C7 */
void F_0195E5(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 0195E5 SEM */
    wr8(ABS(0x0112), 0x1E);  /* 0195E6 LDM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) | 0x04);  /* 0195EA SEB */
    mflag = 0;  /* 0195EE CLM */
    S += 3; return;  /* 0195EF RTL */
}

/* 0195F0  callers: 01965B, 019662, 019669, 019692, 019699, 0196DA, 0196EF, 0196F6 ... */
void F_0195F0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0195F0 LDA */
    push16(D);  /* 0195F2 PHD */
    D = A;  /* 0195F3 TAD */
    A += 1;  /* 0195F4 INC */
    push16(A);  /* 0195F5 PHA */
    wr16(DP(0x00), Y);  /* 0195F6 STY */
    S -= 3; F_007627();  /* 0195F8 JSRL */
    A = rd16(DP(0x00));  /* 0195FC LDA */
    mflag = 1;  /* 0195FE SEM */
    /* 0195FF CMP */
    if ((A & 0xFF) != 0x10) goto L_01960B;  /* 019601 BNE */
    Y = 0x0010;  /* 019603 LDY */
    mflag = 0;  /* 019606 CLM */
    S -= 3; F_007627();  /* 019607 JSRL */
L_01960B: TICK();
    mflag = 0;  /* 01960B CLM */
    Y = pull16();  /* 01960C PLY */
    D = pull16();  /* 01960D PLD */
    S += 3; return;  /* 01960E RTL */
}

/* 01960F  callers: 01964D, 019684, 0196CC */
void F_01960F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01960F LDA */
    push16(A);  /* 019611 PHA */
    Y = 0x0016;  /* 019612 LDY */
    S -= 3; F_007627();  /* 019615 JSRL */
    Y = 0x0010;  /* 019619 LDY */
    S -= 3; F_007627();  /* 01961C JSRL */
    Y = 0x0002;  /* 019620 LDY */
    S -= 3; F_007627();  /* 019623 JSRL */
    Y = pull16();  /* 019627 PLY */
    S += 3; return;  /* 019628 RTL */
}

/* 019629  callers: 01966D, 0196A4, 019836 */
void F_019629(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 019629 LDA */
    push16(A);  /* 01962B PHA */
    Y = 0x0010;  /* 01962C LDY */
    S -= 3; F_007627();  /* 01962F JSRL */
    Y = 0x0003;  /* 019633 LDY */
    S -= 3; F_007627();  /* 019636 JSRL */
    Y = 0x0000;  /* 01963A LDY */
    S -= 3; F_007627();  /* 01963D JSRL */
    Y = 0x0000;  /* 019641 LDY */
    S -= 3; F_007627();  /* 019644 JSRL */
    Y = pull16();  /* 019648 PLY */
    S += 3; return;  /* 019649 RTL */
}

/* 01964A  callers: 0198C3 */
void F_01964A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01964A LDA */
    push16(A);  /* 01964C PHA */
    S -= 3; F_01960F();  /* 01964D JSRL */
    Y = 0x0001;  /* 019651 LDY */
    S -= 3; F_007627();  /* 019654 JSRL */
    Y = 0x0001;  /* 019658 LDY */
    S -= 3; F_0195F0();  /* 01965B JSRL */
    Y = 0x0001;  /* 01965F LDY */
    S -= 3; F_0195F0();  /* 019662 JSRL */
    Y = 0x0080;  /* 019666 LDY */
    S -= 3; F_0195F0();  /* 019669 JSRL */
    S -= 3; F_019629();  /* 01966D JSRL */
    Y = pull16();  /* 019671 PLY */
    S += 3; return;  /* 019672 RTL */
}

/* 019673  callers: 019913, 019988, 019A3E, 019A59, 019AA3, 019AFC, 019B3E, 019B4D ... */
void F_019673(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 019673 LDA */
    push16(A);  /* 019675 PHA */
L_019676: TICK();
    mflag = 1;  /* 019676 SEM */
    wr8(ABS(WDT), 0x00);  /* 019677 LDM */
    SETLO(A, rd8(ABS(0x0332)));  /* 01967B LDA */
    t = rd8(ABS(0x0333));  /* 01967E CMP */
    if ((A & 0xFF) != t) goto L_019676;  /* 019681 BNE */
    mflag = 0;  /* 019683 CLM */
    S -= 3; F_01960F();  /* 019684 JSRL */
    Y = 0x0002;  /* 019688 LDY */
    S -= 3; F_007627();  /* 01968B JSRL */
    Y = rd16(ABS(0x0110));  /* 01968F LDY */
    S -= 3; F_0195F0();  /* 019692 JSRL */
    Y = 0x0001;  /* 019696 LDY */
    S -= 3; F_0195F0();  /* 019699 JSRL */
    Y = 0x0000;  /* 01969D LDY */
    S -= 3; F_007627();  /* 0196A0 JSRL */
    S -= 3; F_019629();  /* 0196A4 JSRL */
    Y = pull16();  /* 0196A8 PLY */
    S += 3; return;  /* 0196A9 RTL */
}

/* 0196AA  callers: 01506B, 01987E, 019962, 019A94, 019AED, 019D24, 019DAF, 019DD3 ... */
void F_0196AA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0196AA LDA */
    push16(D);  /* 0196AC PHD */
    D = A;  /* 0196AD TAD */
    A += 1;  /* 0196AE INC */
    A += 1;  /* 0196AF INC */
    push16(A);  /* 0196B0 PHA */
    wr16(DP(0x00), Y);  /* 0196B1 STY */
    A = rd16(ABS(0x0116));  /* 0196B3 LDA */
    mflag = 1;  /* 0196B6 SEM */
    /* 0196B7 CMP */
    if ((A & 0xFF) == 0x0A) goto L_0196BF;  /* 0196B9 BEQ */
    mflag = 0;  /* 0196BB CLM */
    Y = pull16();  /* 0196BC PLY */
    D = pull16();  /* 0196BD PLD */
    S += 3; return;  /* 0196BE RTL */
L_0196BF: TICK();
    wr8(ABS(WDT), 0x00);  /* 0196BF LDM */
    SETLO(A, rd8(ABS(0x0332)));  /* 0196C3 LDA */
    t = rd8(ABS(0x0333));  /* 0196C6 CMP */
    if ((A & 0xFF) != t) goto L_0196BF;  /* 0196C9 BNE */
    mflag = 0;  /* 0196CB CLM */
    S -= 3; F_01960F();  /* 0196CC JSRL */
    Y = 0x0004;  /* 0196D0 LDY */
    S -= 3; F_007627();  /* 0196D3 JSRL */
    Y = rd16(ABS(0x010F));  /* 0196D7 LDY */
    S -= 3; F_0195F0();  /* 0196DA JSRL */
    Y = 0x0001;  /* 0196DE LDY */
    S -= 3; F_007627();  /* 0196E1 JSRL */
    Y = 0x0000;  /* 0196E5 LDY */
    S -= 3; F_007627();  /* 0196E8 JSRL */
    Y = rd16(ABS(0x029C));  /* 0196EC LDY */
    S -= 3; F_0195F0();  /* 0196EF JSRL */
    Y = rd16(ABS(0x029D));  /* 0196F3 LDY */
    S -= 3; F_0195F0();  /* 0196F6 JSRL */
    A = rd16(ABS(0x029C));  /* 0196FA LDA */
    mflag = 1;  /* 0196FD SEM */
    /* 0196FE CMP */
    if ((A & 0xFF) != 0x03) goto L_019759;  /* 019700 BNE */
    Y = rd16(ABS(0x029E));  /* 019702 LDY */
    mflag = 0;  /* 019705 CLM */
    S -= 3; F_0195F0();  /* 019706 JSRL */
    Y = rd16(ABS(0x029F));  /* 01970A LDY */
    S -= 3; F_0195F0();  /* 01970D JSRL */
    Y = rd16(ABS(0x02A0));  /* 019711 LDY */
    S -= 3; F_0195F0();  /* 019714 JSRL */
    Y = rd16(ABS(0x02A1));  /* 019718 LDY */
    S -= 3; F_0195F0();  /* 01971B JSRL */
    A = rd16(ABS(0x029D));  /* 01971F LDA */
    mflag = 1;  /* 019722 SEM */
    /* 019723 CMP */
    if ((A & 0xFF) == 0x04) goto L_01972E;  /* 019725 BEQ */
    SETLO(A, rd8(ABS(0x029D)));  /* 019727 LDA */
    /* 01972A CMP */
    if ((A & 0xFF) != 0x03) goto L_01974F;  /* 01972C BNE */
L_01972E: TICK();
    wr8(DP(0x01), 0x00);  /* 01972E LDM */
L_019731: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 019731 LDA */
    t = rd8(DP(0x00));  /* 019733 CMP */
    C = ((A & 0xFF) >= t);
    if (C) goto L_01974C;  /* 019735 BCS */
    mflag = 0;  /* 019737 CLM */
    A &= 0x00FF;  /* 019738 AND */
    A = adc16(A, 0x000A);  /* 01973B ADC */
    X = A;  /* 01973E TAX */
    Y = rd16((ABS(0x0083) + X));  /* 01973F LDY */
    S -= 3; F_0195F0();  /* 019742 JSRL */
    mflag = 1;  /* 019746 SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 019747 INC */
    goto L_019731;  /* 019749 JMP */
L_01974C: TICK();
    goto L_019756;  /* 01974C JMP */
L_01974F: TICK();
    Y = rd16(DP(0x00));  /* 01974F LDY */
    mflag = 0;  /* 019751 CLM */
    S -= 3; F_0195F0();  /* 019752 JSRL */
L_019756: TICK();
    goto L_019835;  /* 019756 JMP */
L_019759: TICK();
    mflag = 0;  /* 019759 CLM */
    A = rd16(ABS(0x029C));  /* 01975A LDA */
    mflag = 1;  /* 01975D SEM */
    /* 01975E CMP */
    if ((A & 0xFF) != 0x05) goto L_01976D;  /* 019760 BNE */
    Y = rd16(ABS(0x029E));  /* 019762 LDY */
    mflag = 0;  /* 019765 CLM */
    S -= 3; F_0195F0();  /* 019766 JSRL */
    goto L_019835;  /* 01976A JMP */
L_01976D: TICK();
    mflag = 0;  /* 01976D CLM */
    A = rd16(ABS(0x029C));  /* 01976E LDA */
    mflag = 1;  /* 019771 SEM */
    /* 019772 CMP */
    if ((A & 0xFF) == 0x08) goto L_01977D;  /* 019774 BEQ */
    SETLO(A, rd8(ABS(0x029C)));  /* 019776 LDA */
    /* 019779 CMP */
    if ((A & 0xFF) != 0x06) goto L_019788;  /* 01977B BNE */
L_01977D: TICK();
    Y = rd16(ABS(0x029E));  /* 01977D LDY */
    mflag = 0;  /* 019780 CLM */
    S -= 3; F_0195F0();  /* 019781 JSRL */
    goto L_019835;  /* 019785 JMP */
L_019788: TICK();
    mflag = 0;  /* 019788 CLM */
    A = rd16(ABS(0x029C));  /* 019789 LDA */
    mflag = 1;  /* 01978C SEM */
    /* 01978D CMP */
    if ((A & 0xFF) != 0x09) goto L_0197A3;  /* 01978F BNE */
    Y = rd16(ABS(0x029E));  /* 019791 LDY */
    mflag = 0;  /* 019794 CLM */
    S -= 3; F_0195F0();  /* 019795 JSRL */
    Y = rd16(ABS(0x029F));  /* 019799 LDY */
    S -= 3; F_0195F0();  /* 01979C JSRL */
    goto L_019835;  /* 0197A0 JMP */
L_0197A3: TICK();
    mflag = 0;  /* 0197A3 CLM */
    A = rd16(ABS(0x029C));  /* 0197A4 LDA */
    mflag = 1;  /* 0197A7 SEM */
    /* 0197A8 CMP */
    if ((A & 0xFF) != 0x0B) goto L_0197B7;  /* 0197AA BNE */
    Y = rd16(ABS(0x029E));  /* 0197AC LDY */
    mflag = 0;  /* 0197AF CLM */
    S -= 3; F_0195F0();  /* 0197B0 JSRL */
    goto L_019835;  /* 0197B4 JMP */
L_0197B7: TICK();
    mflag = 0;  /* 0197B7 CLM */
    A = rd16(ABS(0x029C));  /* 0197B8 LDA */
    mflag = 1;  /* 0197BB SEM */
    /* 0197BC CMP */
    if ((A & 0xFF) != 0x0C) goto L_0197CB;  /* 0197BE BNE */
    Y = rd16(ABS(0x029E));  /* 0197C0 LDY */
    mflag = 0;  /* 0197C3 CLM */
    S -= 3; F_0195F0();  /* 0197C4 JSRL */
    goto L_019835;  /* 0197C8 JMP */
L_0197CB: TICK();
    mflag = 0;  /* 0197CB CLM */
    A = rd16(ABS(0x029C));  /* 0197CC LDA */
    mflag = 1;  /* 0197CF SEM */
    SETLO(A, A - 1);  /* 0197D0 DEC */
    if ((A & 0xFF) != 0) goto L_0197D6;  /* 0197D1 BNE */
    goto L_019835;  /* 0197D3 JMP */
L_0197D6: TICK();
    mflag = 0;  /* 0197D6 CLM */
    A = rd16(ABS(0x029C));  /* 0197D7 LDA */
    mflag = 1;  /* 0197DA SEM */
    /* 0197DB CMP */
    if ((A & 0xFF) == 0x80) goto L_0197F2;  /* 0197DD BEQ */
    SETLO(A, rd8(ABS(0x029C)));  /* 0197DF LDA */
    /* 0197E2 CMP */
    if ((A & 0xFF) == 0x82) goto L_0197F2;  /* 0197E4 BEQ */
    /* 0197E6 CMP */
    if ((A & 0xFF) == 0xC0) goto L_0197F2;  /* 0197E8 BEQ */
    /* 0197EA CMP */
    if ((A & 0xFF) == 0xD0) goto L_0197F2;  /* 0197EC BEQ */
    /* 0197EE CMP */
    if ((A & 0xFF) != 0xE0) goto L_019835;  /* 0197F0 BNE */
L_0197F2: TICK();
    Y = rd16(ABS(0x029E));  /* 0197F2 LDY */
    mflag = 0;  /* 0197F5 CLM */
    S -= 3; F_0195F0();  /* 0197F6 JSRL */
    Y = rd16(ABS(0x029F));  /* 0197FA LDY */
    S -= 3; F_0195F0();  /* 0197FD JSRL */
    Y = rd16(ABS(0x02A0));  /* 019801 LDY */
    S -= 3; F_0195F0();  /* 019804 JSRL */
    Y = rd16(ABS(0x02A1));  /* 019808 LDY */
    S -= 3; F_0195F0();  /* 01980B JSRL */
    A = rd16(ABS(0x029C));  /* 01980F LDA */
    mflag = 1;  /* 019812 SEM */
    /* 019813 CMP */
    if ((A & 0xFF) != 0x82) goto L_019835;  /* 019815 BNE */
    wr8(DP(0x01), 0x00);  /* 019817 LDM */
L_01981A: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 01981A LDA */
    t = rd8(DP(0x00));  /* 01981C CMP */
    C = ((A & 0xFF) >= t);
    if (C) goto L_019835;  /* 01981E BCS */
    mflag = 0;  /* 019820 CLM */
    A &= 0x00FF;  /* 019821 AND */
    A = adc16(A, 0x000A);  /* 019824 ADC */
    X = A;  /* 019827 TAX */
    Y = rd16((ABS(0x0083) + X));  /* 019828 LDY */
    S -= 3; F_0195F0();  /* 01982B JSRL */
    mflag = 1;  /* 01982F SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 019830 INC */
    goto L_01981A;  /* 019832 JMP */
L_019835: TICK();
    mflag = 0;  /* 019835 CLM */
    S -= 3; F_019629();  /* 019836 JSRL */
    mflag = 1;  /* 01983A SEM */
    SETLO(A, rd8(ABS(0x0080)));  /* 01983B LDA */
    SETLO(A, A & 0x04);  /* 01983E AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_019844;  /* 019840 BEQ */
    SETLO(A, 0x01);  /* 019842 LDA */
    Z = ((A & 0xFF) == 0);
L_019844: TICK();
    if (!Z) goto L_01984A;  /* 019844 BNE */
    wr8(ABS(0x0113), 0x0A);  /* 019846 LDM */
L_01984A: TICK();
    mflag = 0;  /* 01984A CLM */
    S -= 3; F_0195E5();  /* 01984B JSRL */
    Y = pull16();  /* 01984F PLY */
    D = pull16();  /* 019850 PLD */
    S += 3; return;  /* 019851 RTL */
}

/* 019852  callers: 019860, 019B8A, 01A1F4, 01CB96, 01CEC0, 01D315, 01D56E, 01D7C0 */
void F_019852(void)
{
    uint32_t t; (void)t;
L_019852: TICK();
    A = rd16(ABS(0x0332));  /* 019852 LDA */
    mflag = 1;  /* 019855 SEM */
    t = rd8(ABS(0x0333));  /* 019856 CMP */
    if ((A & 0xFF) == t) goto L_019863;  /* 019859 BEQ */
    wr8(ABS(WDT), 0x00);  /* 01985B LDM */
    mflag = 0;  /* 01985F CLM */
    goto L_019852;  /* 019860 JMP */
L_019863: TICK();
    mflag = 1;  /* 019863 SEM */
    if ((rd8(ABS(U0C0)) & 0x08) == 0x08) goto L_01986C;  /* 019864 BBS */
    goto L_019863;  /* 019869 JMP */
L_01986C: TICK();
    mflag = 0;  /* 01986C CLM */
    S += 3; return;  /* 01986D RTL */
}

/* 01986E  callers: 01A605, 01D4C0, 01D7BC */
void F_01986E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01986E LDA */
    push16(A);  /* 019870 PHA */
    mflag = 1;  /* 019871 SEM */
    wr8(ABS(0x029C), 0x04);  /* 019872 LDM */
    wr8(ABS(0x029D), 0x01);  /* 019876 LDM */
    Y = 0x0000;  /* 01987A LDY */
    mflag = 0;  /* 01987D CLM */
    S -= 3; F_0196AA();  /* 01987E JSRL */
    Y = pull16();  /* 019882 PLY */
    S += 3; return;  /* 019883 RTL */
}

/* 019884  callers: 004B16, 0198ED, 01A1FA, 01A2DB, 01A649, 01A740, 01CB9D, 01CEC7 ... */
void F_019884(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 019884 LDA */
    push16(D);  /* 019886 PHD */
    D = A;  /* 019887 TAD */
    A += 1;  /* 019888 INC */
    push16(A);  /* 019889 PHA */
    wr16(DP(0x00), Y);  /* 01988A STY */
    A = Y;  /* 01988C TYA */
    mflag = 1;  /* 01988D SEM */
    wr8(ABS(0x01EA), (uint8_t)A);  /* 01988E STA */
    mflag = 0;  /* 019891 CLM */
    Y = A;  /* 019892 TAY */
    S -= 3; F_00997F();  /* 019893 JSRL */
    I = 1;  /* 019897 SEI */
    mflag = 1;  /* 019898 SEM */
    wr8(ABS(0x0332), 0x00);  /* 019899 LDM */
    wr8(ABS(0x0333), 0x00);  /* 01989D LDM */
    wr8(ABS(0x010E), 0x00);  /* 0198A1 LDM */
    wr8(ABS(0x0115), 0x01);  /* 0198A5 LDM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x01);  /* 0198A9 CLB */
    I = 0;  /* 0198AD CLI */
    wr8(ABS(0x0114), 0x01);  /* 0198AE LDM */
    wr8(ABS(0x0110), 0x01);  /* 0198B2 LDM */
    wr8(ABS(0x010F), 0x01);  /* 0198B6 LDM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x04);  /* 0198BA CLB */
    wr8(ABS(0x0116), 0x09);  /* 0198BE LDM */
    mflag = 0;  /* 0198C2 CLM */
    S -= 3; F_01964A();  /* 0198C3 JSRL */
    Y = pull16();  /* 0198C7 PLY */
    D = pull16();  /* 0198C8 PLD */
    S += 3; return;  /* 0198C9 RTL */
}

/* 0198CA  callers: 01990F, 019984 */
void F_0198CA(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 0198CA SEM */
    wr8(ABS(0x0110), 0x01);  /* 0198CB LDM */
    wr8(ABS(0x010F), 0x01);  /* 0198CF LDM */
    mflag = 0;  /* 0198D3 CLM */
    S += 3; return;  /* 0198D4 RTL */
}

/* 0198D5  callers: 006670, 016FD5, 01A425, 01A65D, 01A79E, 01A7C0, 01D281 */
void measurements_0198D5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0198D5 LDA */
    push16(D);  /* 0198D7 PHD */
    D = A;  /* 0198D8 TAD */
    C = 0;  /* 0198D9 CLC */
    A = adc16(A, 0x000D);  /* 0198DA ADC */
    push16(A);  /* 0198DD PHA */
    mflag = 1;  /* 0198DE SEM */
    wr8(DP(0x03), 0x00);  /* 0198DF LDM */
L_0198E2: TICK();
    SETLO(A, rd8(ABS(0x0116)));  /* 0198E2 LDA */
    /* 0198E5 CMP */
    if ((A & 0xFF) != 0x08) goto L_0198F4;  /* 0198E7 BNE */
    Y = rd16(ABS(0x01EA));  /* 0198E9 LDY */
    mflag = 0;  /* 0198EC CLM */
    S -= 3; F_019884();  /* 0198ED JSRL */
    goto L_01A2EE;  /* 0198F1 JMP */
L_0198F4: TICK();
    mflag = 0;  /* 0198F4 CLM */
    A = rd16(ABS(0x0116));  /* 0198F5 LDA */
    mflag = 1;  /* 0198F8 SEM */
    /* 0198F9 CMP */
    if ((A & 0xFF) != 0x09) goto L_01994B;  /* 0198FB BNE */
    SETLO(A, rd8(ABS(0x0080)));  /* 0198FD LDA */
    SETLO(A, A & 0x01);  /* 019900 AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_019906;  /* 019902 BEQ */
    SETLO(A, 0x01);  /* 019904 LDA */
    Z = ((A & 0xFF) == 0);
L_019906: TICK();
    if (Z) goto L_019944;  /* 019906 BEQ */
    SETLO(A, rd8(ABS(0x0083)));  /* 019908 LDA */
    SETLO(A, A - 1);  /* 01990B DEC */
    if ((A & 0xFF) != 0) goto L_019923;  /* 01990C BNE */
    mflag = 0;  /* 01990E CLM */
    S -= 3; F_0198CA();  /* 01990F JSRL */
    S -= 3; F_019673();  /* 019913 JSRL */
    mflag = 1;  /* 019917 SEM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x02);  /* 019918 CLB */
    wr8(ABS(0x0116), 0x0A);  /* 01991C LDM */
    goto L_019944;  /* 019920 JMP */
L_019923: TICK();
    mflag = 0;  /* 019923 CLM */
    A = rd16(ABS(0x0083));  /* 019924 LDA */
    mflag = 1;  /* 019927 SEM */
    /* 019928 CMP */
    if ((A & 0xFF) != 0x02) goto L_019937;  /* 01992A BNE */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x02);  /* 01992C CLB */
    wr8(ABS(0x0116), 0x0A);  /* 019930 LDM */
    goto L_019944;  /* 019934 JMP */
L_019937: TICK();
    mflag = 0;  /* 019937 CLM */
    A = rd16(ABS(0x0083));  /* 019938 LDA */
    mflag = 1;  /* 01993B SEM */
    /* 01993C CMP */
    if ((A & 0xFF) != 0x04) goto L_019944;  /* 01993E BNE */
    wr8(ABS(0x0116), 0x08);  /* 019940 LDM */
L_019944: TICK();
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x01);  /* 019944 CLB */
    goto L_01A2EE;  /* 019948 JMP */
L_01994B: TICK();
    mflag = 0;  /* 01994B CLM */
    A = rd16(ABS(0x0116));  /* 01994C LDA */
    mflag = 1;  /* 01994F SEM */
    /* 019950 CMP */
    if ((A & 0xFF) == 0x0A) goto L_019957;  /* 019952 BEQ */
    goto L_01A2EE;  /* 019954 JMP */
L_019957: TICK();
    SETLO(A, rd8(ABS(0x02A3)));  /* 019957 LDA */
    /* 01995A CMP */
    if ((A & 0xFF) != 0x02) goto L_01996B;  /* 01995C BNE */
    Y = 0x0000;  /* 01995E LDY */
    mflag = 0;  /* 019961 CLM */
    S -= 3; F_0196AA();  /* 019962 JSRL */
    mflag = 1;  /* 019966 SEM */
    wr8(ABS(0x02A3), 0x00);  /* 019967 LDM */
L_01996B: TICK();
    SETLO(A, rd8(ABS(0x0080)));  /* 01996B LDA */
    SETLO(A, A & 0x01);  /* 01996E AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_019974;  /* 019970 BEQ */
    SETLO(A, 0x01);  /* 019972 LDA */
    Z = ((A & 0xFF) == 0);
L_019974: TICK();
    if (!Z) goto L_019979;  /* 019974 BNE */
    goto L_01A2C1;  /* 019976 JMP */
L_019979: TICK();
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x01);  /* 019979 CLB */
    SETLO(A, rd8(ABS(0x0083)));  /* 01997D LDA */
    SETLO(A, A - 1);  /* 019980 DEC */
    if ((A & 0xFF) != 0) goto L_01999C;  /* 019981 BNE */
    mflag = 0;  /* 019983 CLM */
    S -= 3; F_0198CA();  /* 019984 JSRL */
    S -= 3; F_019673();  /* 019988 JSRL */
    mflag = 1;  /* 01998C SEM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x02);  /* 01998D CLB */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x04);  /* 019991 CLB */
    wr8(ABS(0x0116), 0x0A);  /* 019995 LDM */
    goto L_01A2BE;  /* 019999 JMP */
L_01999C: TICK();
    mflag = 0;  /* 01999C CLM */
    A = rd16(ABS(0x0083));  /* 01999D LDA */
    mflag = 1;  /* 0199A0 SEM */
    /* 0199A1 CMP */
    if ((A & 0xFF) != 0x02) goto L_0199D7;  /* 0199A3 BNE */
    SETLO(A, rd8(ABS(0x010F)));  /* 0199A5 LDA */
    SETLO(A, A + 1);  /* 0199A8 INC */
    t = rd8(ABS(0x0084));  /* 0199A9 CMP */
    if ((A & 0xFF) == t) goto L_0199B4;  /* 0199AC BEQ */
    SETLO(A, rd8(ABS(0x0084)));  /* 0199AE LDA */
    SETLO(A, A - 1);  /* 0199B1 DEC */
    if ((A & 0xFF) != 0) goto L_0199C6;  /* 0199B2 BNE */
L_0199B4: TICK();
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x04);  /* 0199B4 CLB */
    t = (rd8(ABS(0x010F)) + 1) & 0xFF; wr8(ABS(0x010F), t);  /* 0199B8 INC */
    wr8(DP(0x03), 0x01);  /* 0199BB LDM */
    mflag = 0;  /* 0199BE CLM */
    S -= 3; F_01DF9B();  /* 0199BF JSRL */
    goto L_0199CB;  /* 0199C3 JMP */
L_0199C6: TICK();
    mflag = 0;  /* 0199C6 CLM */
    S -= 3; F_0195E5();  /* 0199C7 JSRL */
L_0199CB: TICK();
    mflag = 1;  /* 0199CB SEM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) & ~0x02);  /* 0199CC CLB */
    wr8(ABS(0x0116), 0x0A);  /* 0199D0 LDM */
    goto L_01A2BE;  /* 0199D4 JMP */
L_0199D7: TICK();
    mflag = 0;  /* 0199D7 CLM */
    A = rd16(ABS(0x0083));  /* 0199D8 LDA */
    mflag = 1;  /* 0199DB SEM */
    /* 0199DC CMP */
    if ((A & 0xFF) == 0x04) goto L_0199E3;  /* 0199DE BEQ */
    goto L_01A2BE;  /* 0199E0 JMP */
L_0199E3: TICK();
    SETLO(A, rd8(ABS(0x0084)));  /* 0199E3 LDA */
    t = rd8(ABS(0x0110));  /* 0199E6 CMP */
    if ((A & 0xFF) == t) goto L_0199EE;  /* 0199E9 BEQ */
    goto L_01A2B9;  /* 0199EB JMP */
L_0199EE: TICK();
    t = (rd8(ABS(0x0110)) + 1) & 0xFF; wr8(ABS(0x0110), t);  /* 0199EE INC */
    SETLO(A, rd8(ABS(0x0089)));  /* 0199F1 LDA */
    mflag = 0;  /* 0199F4 CLM */
    A &= 0x00FF;  /* 0199F5 AND */
    A = rla16(A, 0x0008);  /* 0199F8 RLA */
    A &= 0xFF00;  /* 0199FC AND */
    wr16(DP(0x0D), A);  /* 0199FF STA */
    A = rd16(ABS(0x0088));  /* 019A01 LDA */
    A &= 0x00FF;  /* 019A04 AND */
    C = 0;  /* 019A07 CLC */
    A = adc16(A, rd16(DP(0x0D)));  /* 019A08 ADC */
    wr16(DP(0x00), A);  /* 019A0A STA */
    A = rd16(ABS(0x0087));  /* 019A0C LDA */
    mflag = 1;  /* 019A0F SEM */
    wr8(ABS(0x029C), (uint8_t)A);  /* 019A10 STA */
    SETLO(A, rd8(ABS(0x0088)));  /* 019A13 LDA */
    wr8(ABS(0x029D), (uint8_t)A);  /* 019A16 STA */
    SETLO(A, rd8(ABS(0x0089)));  /* 019A19 LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019A1C STA */
    SETLO(A, rd8(ABS(0x008A)));  /* 019A1F LDA */
    wr8(ABS(0x029F), (uint8_t)A);  /* 019A22 STA */
    SETLO(A, rd8(ABS(0x008B)));  /* 019A25 LDA */
    wr8(ABS(0x02A0), (uint8_t)A);  /* 019A28 STA */
    SETLO(A, rd8(ABS(0x008C)));  /* 019A2B LDA */
    wr8(ABS(0x02A1), (uint8_t)A);  /* 019A2E STA */
    SETLO(A, rd8(ABS(0x008D)));  /* 019A31 LDA */
    wr8(ABS(0x02A2), (uint8_t)A);  /* 019A34 STA */
    SETLO(A, rd8(ABS(0x029C)));  /* 019A37 LDA */
    SETLO(A, A - 1);  /* 019A3A DEC */
    if ((A & 0xFF) != 0) goto L_019A45;  /* 019A3B BNE */
    mflag = 0;  /* 019A3D CLM */
    S -= 3; F_019673();  /* 019A3E JSRL */
    goto L_01A2B6;  /* 019A42 JMP */
L_019A45: TICK();
    mflag = 0;  /* 019A45 CLM */
    A = rd16(ABS(0x029C));  /* 019A46 LDA */
    mflag = 1;  /* 019A49 SEM */
    /* 019A4A CMP */
    if ((A & 0xFF) == 0x03) goto L_019A51;  /* 019A4C BEQ */
    goto L_019B7C;  /* 019A4E JMP */
L_019A51: TICK();
    mflag = 0;  /* 019A51 CLM */
    X = rd16(DP(0x00));  /* 019A52 LDX */
    /* 019A54 CPX */
    if (X != 0x0101) goto L_019A9B;  /* 019A57 BNE */
    S -= 3; F_019673();  /* 019A59 JSRL */
    A = rd16(ABS(0x02A2));  /* 019A5D LDA */
    wr16(DP(0x0D), A);  /* 019A60 STA */
    wr16(DP(0x0E), 0x008D);  /* 019A62 LDM */
    wr16(DP(0x10), 0x0000);  /* 019A66 LDM */
    A = rd16(ABS(0x02A0));  /* 019A6A LDA */
    A &= 0x00FF;  /* 019A6D AND */
    A = rla16(A, 0x0008);  /* 019A70 RLA */
    A &= 0xFF00;  /* 019A74 AND */
    wr16(DP(0x11), A);  /* 019A77 STA */
    A = rd16(ABS(0x02A1));  /* 019A79 LDA */
    A &= 0x00FF;  /* 019A7C AND */
    C = 0;  /* 019A7F CLC */
    A = adc16(A, rd16(DP(0x11)));  /* 019A80 ADC */
    wr16(DP(0x11), A);  /* 019A82 STA */
    Y = 0x00A0;  /* 019A84 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 019A87 JSRL */
    mflag = 1;  /* 019A8B SEM */
    wr8(ABS(0x029D), 0x04);  /* 019A8C LDM */
    Y = rd16(ABS(0x02A2));  /* 019A90 LDY */
    mflag = 0;  /* 019A93 CLM */
    S -= 3; F_0196AA();  /* 019A94 JSRL */
    goto L_019B79;  /* 019A98 JMP */
L_019A9B: TICK();
    mflag = 0;  /* 019A9B CLM */
    X = rd16(DP(0x00));  /* 019A9C LDX */
    /* 019A9E CPX */
    if (X != 0x0201) goto L_019AF4;  /* 019AA1 BNE */
    S -= 3; F_019673();  /* 019AA3 JSRL */
    A = rd16(ABS(0x02A1));  /* 019AA7 LDA */
    mflag = 1;  /* 019AAA SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 019AAB STA */
    SETLO(A, rd8(ABS(0x02A0)));  /* 019AAD LDA */
    wr8(DP(0x05), (uint8_t)A);  /* 019AB0 STA */
    SETLO(A, rd8(ABS(0x029F)));  /* 019AB2 LDA */
    wr8(DP(0x06), (uint8_t)A);  /* 019AB5 STA */
    wr8(DP(0x02), 0x00);  /* 019AB7 LDM */
L_019ABA: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 019ABA LDA */
    t = rd8(ABS(0x02A2));  /* 019ABC CMP */
    C = ((A & 0xFF) >= t);
    if (C) goto L_019AE4;  /* 019ABF BCS */
    SETLO(B, rd8(DP(0x06)));  /* 019AC1 LDA */
    mflag = 0;  /* 019AC4 CLM */
    A = rd16(DP(0x04));  /* 019AC5 LDA */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 019AC7 INC */
    wr16(DP(0x0D), A);  /* 019AC9 STA */
    mflag = 1;  /* 019ACB SEM */
    wr8(DP(0x0F), (uint8_t)B);  /* 019ACC STA */
    SETLO(A, rd8(DP(0x02)));  /* 019ACF LDA */
    mflag = 0;  /* 019AD1 CLM */
    A &= 0x00FF;  /* 019AD2 AND */
    A = adc16(A, 0x000A);  /* 019AD5 ADC */
    X = A;  /* 019AD8 TAX */
    A = rd16(LIND(0x0D));  /* 019AD9 LDA */
    mflag = 1;  /* 019ADB SEM */
    wr8((ABS(0x0083) + X), (uint8_t)A);  /* 019ADC STA */
    t = (rd8(DP(0x02)) + 1) & 0xFF; wr8(DP(0x02), t);  /* 019ADF INC */
    goto L_019ABA;  /* 019AE1 JMP */
L_019AE4: TICK();
    mflag = 1;  /* 019AE4 SEM */
    wr8(ABS(0x029D), 0x04);  /* 019AE5 LDM */
    Y = rd16(ABS(0x02A2));  /* 019AE9 LDY */
    mflag = 0;  /* 019AEC CLM */
    S -= 3; F_0196AA();  /* 019AED JSRL */
    goto L_019B79;  /* 019AF1 JMP */
L_019AF4: TICK();
    mflag = 0;  /* 019AF4 CLM */
    X = rd16(DP(0x00));  /* 019AF5 LDX */
    /* 019AF7 CPX */
    if (X != 0x0202) goto L_019B03;  /* 019AFA BNE */
    S -= 3; F_019673();  /* 019AFC JSRL */
    goto L_019B79;  /* 019B00 JMP */
L_019B03: TICK();
    mflag = 0;  /* 019B03 CLM */
    X = rd16(DP(0x00));  /* 019B04 LDX */
    /* 019B06 CPX */
    if (X != 0x0103) goto L_019B45;  /* 019B09 BNE */
    A = rd16(ABS(0x010E));  /* 019B0B LDA */
    C = 0;  /* 019B0E CLC */
    mflag = 1;  /* 019B0F SEM */
    SETLO(A, adc8(A, 0xF6));  /* 019B10 ADC */
    wr8(DP(0x0D), (uint8_t)A);  /* 019B12 STA */
    mflag = 0;  /* 019B14 CLM */
    wr16(DP(0x0E), 0x008D);  /* 019B15 LDM */
    wr16(DP(0x10), 0x0000);  /* 019B19 LDM */
    A = rd16(ABS(0x02A0));  /* 019B1D LDA */
    A &= 0x00FF;  /* 019B20 AND */
    A = rla16(A, 0x0008);  /* 019B23 RLA */
    A &= 0xFF00;  /* 019B27 AND */
    wr16(DP(0x11), A);  /* 019B2A STA */
    A = rd16(ABS(0x02A1));  /* 019B2C LDA */
    A &= 0x00FF;  /* 019B2F AND */
    C = 0;  /* 019B32 CLC */
    A = adc16(A, rd16(DP(0x11)));  /* 019B33 ADC */
    wr16(DP(0x11), A);  /* 019B35 STA */
    Y = 0x00A0;  /* 019B37 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 019B3A JSRL */
    S -= 3; F_019673();  /* 019B3E JSRL */
    goto L_019B79;  /* 019B42 JMP */
L_019B45: TICK();
    mflag = 0;  /* 019B45 CLM */
    X = rd16(DP(0x00));  /* 019B46 LDX */
    /* 019B48 CPX */
    if (X != 0x0203) goto L_019B54;  /* 019B4B BNE */
    S -= 3; F_019673();  /* 019B4D JSRL */
    goto L_019B79;  /* 019B51 JMP */
L_019B54: TICK();
    mflag = 0;  /* 019B54 CLM */
    X = rd16(DP(0x00));  /* 019B55 LDX */
    /* 019B57 CPX */
    if (X != 0x0104) goto L_019B68;  /* 019B5A BNE */
    mflag = 1;  /* 019B5C SEM */
    wr8(DP(0x03), 0x02);  /* 019B5D LDM */
    mflag = 0;  /* 019B60 CLM */
    S -= 3; F_019673();  /* 019B61 JSRL */
    goto L_019B79;  /* 019B65 JMP */
L_019B68: TICK();
    mflag = 0;  /* 019B68 CLM */
    X = rd16(DP(0x00));  /* 019B69 LDX */
    /* 019B6B CPX */
    if (X != 0x0204) goto L_019B79;  /* 019B6E BNE */
    mflag = 1;  /* 019B70 SEM */
    wr8(DP(0x03), 0x02);  /* 019B71 LDM */
    mflag = 0;  /* 019B74 CLM */
    S -= 3; F_019673();  /* 019B75 JSRL */
L_019B79: TICK();
    goto L_01A2B6;  /* 019B79 JMP */
L_019B7C: TICK();
    mflag = 0;  /* 019B7C CLM */
    A = rd16(ABS(0x029C));  /* 019B7D LDA */
    mflag = 1;  /* 019B80 SEM */
    /* 019B81 CMP */
    if ((A & 0xFF) != 0x04) goto L_019BB2;  /* 019B83 BNE */
    mflag = 0;  /* 019B85 CLM */
    S -= 3; F_019673();  /* 019B86 JSRL */
    S -= 3; F_019852();  /* 019B8A JSRL */
    A = rd16(ABS(0x029D));  /* 019B8E LDA */
    mflag = 1;  /* 019B91 SEM */
    SETLO(A, A - 1);  /* 019B92 DEC */
    if ((A & 0xFF) != 0) goto L_019BA1;  /* 019B93 BNE */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 019B95 SEB */
    mflag = 0;  /* 019B99 CLM */
    S -= 3; reset_entry();  /* 019B9A JSRL */
    goto L_019BAF;  /* 019B9E JMP */
L_019BA1: TICK();
    mflag = 0;  /* 019BA1 CLM */
    A = rd16(ABS(0x029D));  /* 019BA2 LDA */
    mflag = 1;  /* 019BA5 SEM */
    /* 019BA6 CMP */
    if ((A & 0xFF) != 0x02) goto L_019BAF;  /* 019BA8 BNE */
    mflag = 0;  /* 019BAA CLM */
    S -= 3; sys_init_00E305();  /* 019BAB JSRL */
L_019BAF: TICK();
    goto L_01A2B6;  /* 019BAF JMP */
L_019BB2: TICK();
    mflag = 0;  /* 019BB2 CLM */
    A = rd16(ABS(0x029C));  /* 019BB3 LDA */
    mflag = 1;  /* 019BB6 SEM */
    /* 019BB7 CMP */
    if ((A & 0xFF) != 0x05) goto L_019C1C;  /* 019BB9 BNE */
    mflag = 0;  /* 019BBB CLM */
    S -= 3; F_019673();  /* 019BBC JSRL */
    A = rd16(ABS(0x029D));  /* 019BC0 LDA */
    mflag = 1;  /* 019BC3 SEM */
    SETLO(A, A - 1);  /* 019BC4 DEC */
    if ((A & 0xFF) != 0) goto L_019BDF;  /* 019BC5 BNE */
    mflag = 0;  /* 019BC7 CLM */
    wr16(DP(0x0D), 0x0089);  /* 019BC8 LDM */
    wr16(DP(0x0F), 0x0000);  /* 019BCC LDM */
    Y = rd16(ABS(0x029D));  /* 019BD0 LDY */
    S -= 3; F_019383();  /* 019BD3 JSRL */
    mflag = 1;  /* 019BD7 SEM */
    wr8(ABS(0x02A3), 0x01);  /* 019BD8 LDM */
    goto L_019C15;  /* 019BDC JMP */
L_019BDF: TICK();
    mflag = 0;  /* 019BDF CLM */
    A = rd16(ABS(0x029D));  /* 019BE0 LDA */
    mflag = 1;  /* 019BE3 SEM */
    /* 019BE4 CMP */
    if ((A & 0xFF) != 0x02) goto L_019C15;  /* 019BE6 BNE */
    wr8(DP(0x0A), 0x00);  /* 019BE8 LDM */
    SETLO(A, rd8(ABS(0x029E)));  /* 019BEB LDA */
    wr8(DP(0x09), (uint8_t)A);  /* 019BEE STA */
    SETLO(A, rd8(ABS(0x029F)));  /* 019BF0 LDA */
    wr8(DP(0x08), (uint8_t)A);  /* 019BF3 STA */
    SETLO(A, rd8(ABS(0x02A0)));  /* 019BF5 LDA */
    wr8(DP(0x07), (uint8_t)A);  /* 019BF8 STA */
    mflag = 0;  /* 019BFA CLM */
    A = D;  /* 019BFB TDA */
    C = 0;  /* 019BFC CLC */
    A = adc16(A, 0x0007);  /* 019BFD ADC */
    B = 0x0000;  /* 019C00 LDA */
    wr16(DP(0x0D), A);  /* 019C04 STA */
    wr16(DP(0x0F), B);  /* 019C06 STA */
    Y = rd16(ABS(0x029D));  /* 019C09 LDY */
    S -= 3; F_019383();  /* 019C0C JSRL */
    mflag = 1;  /* 019C10 SEM */
    wr8(ABS(0x02A3), 0x01);  /* 019C11 LDM */
L_019C15: TICK();
    wr8(ABS(0x029D), 0x03);  /* 019C15 LDM */
    goto L_01A2B6;  /* 019C19 JMP */
L_019C1C: TICK();
    mflag = 0;  /* 019C1C CLM */
    A = rd16(ABS(0x029C));  /* 019C1D LDA */
    mflag = 1;  /* 019C20 SEM */
    /* 019C21 CMP */
    if ((A & 0xFF) != 0x06) goto L_019C5F;  /* 019C23 BNE */
    mflag = 0;  /* 019C25 CLM */
    S -= 3; F_019673();  /* 019C26 JSRL */
    A = rd16(ABS(0x029D));  /* 019C2A LDA */
    mflag = 1;  /* 019C2D SEM */
    SETLO(A, A - 1);  /* 019C2E DEC */
    if ((A & 0xFF) != 0) goto L_019C3A;  /* 019C2F BNE */
    mflag = 0;  /* 019C31 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 019C32 CLB */
    goto L_019C4F;  /* 019C37 JMP */
L_019C3A: TICK();
    mflag = 0;  /* 019C3A CLM */
    A = rd16(ABS(0x029D));  /* 019C3B LDA */
    mflag = 1;  /* 019C3E SEM */
    /* 019C3F CMP */
    if ((A & 0xFF) != 0x02) goto L_019C4F;  /* 019C41 BNE */
    SETLO(A, rd8(ABS(0x029E)));  /* 019C43 LDA */
    wr8(ABS(0x037C), (uint8_t)A);  /* 019C46 STA */
    mflag = 0;  /* 019C49 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 019C4A SEB */
L_019C4F: TICK();
    mflag = 1;  /* 019C4F SEM */
    wr8(ABS(0x02A3), 0x01);  /* 019C50 LDM */
    wr8(ABS(0x029D), 0x03);  /* 019C54 LDM */
    wr8(ABS(0x029C), 0x05);  /* 019C58 LDM */
    goto L_01A2B6;  /* 019C5C JMP */
L_019C5F: TICK();
    mflag = 0;  /* 019C5F CLM */
    A = rd16(ABS(0x029C));  /* 019C60 LDA */
    mflag = 1;  /* 019C63 SEM */
    /* 019C64 CMP */
    if ((A & 0xFF) == 0x07) goto L_019C6B;  /* 019C66 BEQ */
    goto L_019D2B;  /* 019C68 JMP */
L_019C6B: TICK();
    mflag = 0;  /* 019C6B CLM */
    S -= 3; F_019673();  /* 019C6C JSRL */
    mflag = 1;  /* 019C70 SEM */
    wr8(ABS(0x029C), 0x08);  /* 019C71 LDM */
    SETLO(A, rd8(ABS(0x029D)));  /* 019C75 LDA */
    SETLO(A, A - 1);  /* 019C78 DEC */
    if ((A & 0xFF) != 0) goto L_019C84;  /* 019C79 BNE */
    SETLO(A, rd8(ABS(0x020F)));  /* 019C7B LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019C7E STA */
    goto L_019D20;  /* 019C81 JMP */
L_019C84: TICK();
    mflag = 0;  /* 019C84 CLM */
    A = rd16(ABS(0x029D));  /* 019C85 LDA */
    mflag = 1;  /* 019C88 SEM */
    /* 019C89 CMP */
    if ((A & 0xFF) != 0x02) goto L_019C9F;  /* 019C8B BNE */
    Y = 0x0007;  /* 019C8D LDY */
    mflag = 0;  /* 019C90 CLM */
    S -= 3; F_008DC6();  /* 019C91 JSRL */
    mflag = 1;  /* 019C95 SEM */
    SETLO(A, rd8(ABS(AD7)));  /* 019C96 LDA [AN7 GIDEN_GUC] */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019C99 STA */
    goto L_019D20;  /* 019C9C JMP */
L_019C9F: TICK();
    mflag = 0;  /* 019C9F CLM */
    A = rd16(ABS(0x029D));  /* 019CA0 LDA */
    mflag = 1;  /* 019CA3 SEM */
    /* 019CA4 CMP */
    if ((A & 0xFF) != 0x03) goto L_019CBA;  /* 019CA6 BNE */
    Y = 0x0006;  /* 019CA8 LDY */
    mflag = 0;  /* 019CAB CLM */
    S -= 3; F_008DC6();  /* 019CAC JSRL */
    mflag = 1;  /* 019CB0 SEM */
    SETLO(A, rd8(ABS(AD6)));  /* 019CB1 LDA [AN6 YANSIYAN_GUC] */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019CB4 STA */
    goto L_019D20;  /* 019CB7 JMP */
L_019CBA: TICK();
    mflag = 0;  /* 019CBA CLM */
    A = rd16(ABS(0x029D));  /* 019CBB LDA */
    mflag = 1;  /* 019CBE SEM */
    /* 019CBF CMP */
    if ((A & 0xFF) != 0x04) goto L_019CDA;  /* 019CC1 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_019CD7;  /* 019CC3 BBC */
    Y = 0x0003;  /* 019CC8 LDY */
    mflag = 0;  /* 019CCB CLM */
    S -= 3; F_008DC6();  /* 019CCC JSRL */
    mflag = 1;  /* 019CD0 SEM */
    SETLO(A, rd8(ABS(AD3)));  /* 019CD1 LDA [AN3 SICAKLIK] */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019CD4 STA */
L_019CD7: TICK();
    goto L_019D20;  /* 019CD7 JMP */
L_019CDA: TICK();
    mflag = 0;  /* 019CDA CLM */
    A = rd16(ABS(0x029D));  /* 019CDB LDA */
    mflag = 1;  /* 019CDE SEM */
    /* 019CDF CMP */
    if ((A & 0xFF) != 0x05) goto L_019CEC;  /* 019CE1 BNE */
    SETLO(A, rd8(ABS(0x01FB)));  /* 019CE3 LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019CE6 STA */
    goto L_019D20;  /* 019CE9 JMP */
L_019CEC: TICK();
    mflag = 0;  /* 019CEC CLM */
    A = rd16(ABS(0x029D));  /* 019CED LDA */
    mflag = 1;  /* 019CF0 SEM */
    /* 019CF1 CMP */
    if ((A & 0xFF) != 0x06) goto L_019D08;  /* 019CF3 BNE */
    mflag = 0;  /* 019CF5 CLM */
    A = rd16(ABS(0x023E));  /* 019CF6 LDA */
    A &= 0x0020;  /* 019CF9 AND */
    if (A == 0) goto L_019D01;  /* 019CFC BEQ */
    A = 0x0001;  /* 019CFE LDA */
L_019D01: TICK();
    mflag = 1;  /* 019D01 SEM */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019D02 STA */
    goto L_019D20;  /* 019D05 JMP */
L_019D08: TICK();
    mflag = 0;  /* 019D08 CLM */
    A = rd16(ABS(0x029D));  /* 019D09 LDA */
    mflag = 1;  /* 019D0C SEM */
    /* 019D0D CMP */
    if ((A & 0xFF) != 0x07) goto L_019D20;  /* 019D0F BNE */
    Y = 0x0005;  /* 019D11 LDY */
    mflag = 0;  /* 019D14 CLM */
    S -= 3; F_008DC6();  /* 019D15 JSRL */
    mflag = 1;  /* 019D19 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 019D1A LDA [AN5 RSSI] */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019D1D STA */
L_019D20: TICK();
    Y = 0x0000;  /* 019D20 LDY */
    mflag = 0;  /* 019D23 CLM */
    S -= 3; F_0196AA();  /* 019D24 JSRL */
    goto L_01A2B6;  /* 019D28 JMP */
L_019D2B: TICK();
    mflag = 0;  /* 019D2B CLM */
    A = rd16(ABS(0x029C));  /* 019D2C LDA */
    mflag = 1;  /* 019D2F SEM */
    /* 019D30 CMP */
    if ((A & 0xFF) != 0x08) goto L_019D64;  /* 019D32 BNE */
    mflag = 0;  /* 019D34 CLM */
    S -= 3; F_019673();  /* 019D35 JSRL */
    A = rd16(ABS(0x029D));  /* 019D39 LDA */
    mflag = 1;  /* 019D3C SEM */
    SETLO(A, A - 1);  /* 019D3D DEC */
    if ((A & 0xFF) != 0) goto L_019D4F;  /* 019D3E BNE */
    SETLO(A, rd8(ABS(0x029E)));  /* 019D40 LDA */
    wr8(ABS(0x020F), (uint8_t)A);  /* 019D43 STA */
    wr8(ABS(0x0279), (uint8_t)A);  /* 019D46 STA */
    wr8(ABS(TA3_H), (uint8_t)A);  /* 019D49 STA */
    goto L_019D61;  /* 019D4C JMP */
L_019D4F: TICK();
    mflag = 0;  /* 019D4F CLM */
    A = rd16(ABS(0x029D));  /* 019D50 LDA */
    mflag = 1;  /* 019D53 SEM */
    /* 019D54 CMP */
    if ((A & 0xFF) != 0x05) goto L_019D61;  /* 019D56 BNE */
    SETLO(A, rd8(ABS(0x029E)));  /* 019D58 LDA */
    wr8(ABS(0x01FB), (uint8_t)A);  /* 019D5B STA */
    wr8(ABS(TA2_H), (uint8_t)A);  /* 019D5E STA */
L_019D61: TICK();
    goto L_01A2B6;  /* 019D61 JMP */
L_019D64: TICK();
    mflag = 0;  /* 019D64 CLM */
    A = rd16(ABS(0x029C));  /* 019D65 LDA */
    mflag = 1;  /* 019D68 SEM */
    /* 019D69 CMP */
    if ((A & 0xFF) == 0x09) goto L_019D70;  /* 019D6B BEQ */
    goto L_01A1B2;  /* 019D6D JMP */
L_019D70: TICK();
    mflag = 0;  /* 019D70 CLM */
    S -= 3; F_019673();  /* 019D71 JSRL */
    A = rd16(ABS(0x029E));  /* 019D75 LDA */
    A &= 0x00FF;  /* 019D78 AND */
    A = rla16(A, 0x0008);  /* 019D7B RLA */
    A &= 0xFF00;  /* 019D7F AND */
    wr16(DP(0x0D), A);  /* 019D82 STA */
    A = rd16(ABS(0x029F));  /* 019D84 LDA */
    A &= 0x00FF;  /* 019D87 AND */
    C = 0;  /* 019D8A CLC */
    A = adc16(A, rd16(DP(0x0D)));  /* 019D8B ADC */
    wr16(DP(0x0B), A);  /* 019D8D STA */
    A = rd16(ABS(0x029D));  /* 019D8F LDA */
    mflag = 1;  /* 019D92 SEM */
    SETLO(A, A - 1);  /* 019D93 DEC */
    if ((A & 0xFF) != 0) goto L_019DB6;  /* 019D94 BNE */
    wr8(ABS(0x029D), 0x03);  /* 019D96 LDM */
    mflag = 0;  /* 019D9A CLM */
    A = rd16(ABS(0x0297));  /* 019D9B LDA */
    A &= 0x00FF;  /* 019D9E AND */
    mflag = 1;  /* 019DA1 SEM */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019DA2 STA */
    SETLO(A, rd8(ABS(0x0296)));  /* 019DA5 LDA */
    wr8(ABS(0x029F), (uint8_t)A);  /* 019DA8 STA */
    Y = 0x0000;  /* 019DAB LDY */
    mflag = 0;  /* 019DAE CLM */
    S -= 3; F_0196AA();  /* 019DAF JSRL */
    goto L_01A1AF;  /* 019DB3 JMP */
L_019DB6: TICK();
    mflag = 0;  /* 019DB6 CLM */
    A = rd16(ABS(0x029D));  /* 019DB7 LDA */
    mflag = 1;  /* 019DBA SEM */
    /* 019DBB CMP */
    if ((A & 0xFF) != 0x02) goto L_019DDA;  /* 019DBD BNE */
    wr8(ABS(0x029D), 0x04);  /* 019DBF LDM */
    SETLO(A, rd8(ABS(0x01FE)));  /* 019DC3 LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 019DC6 STA */
    SETLO(A, rd8(ABS(0x01FF)));  /* 019DC9 LDA */
    wr8(ABS(0x029F), (uint8_t)A);  /* 019DCC STA */
    Y = 0x0000;  /* 019DCF LDY */
    mflag = 0;  /* 019DD2 CLM */
    S -= 3; F_0196AA();  /* 019DD3 JSRL */
    goto L_01A1AF;  /* 019DD7 JMP */
L_019DDA: TICK();
    mflag = 0;  /* 019DDA CLM */
    A = rd16(ABS(0x029D));  /* 019DDB LDA */
    mflag = 1;  /* 019DDE SEM */
    /* 019DDF CMP */
    if ((A & 0xFF) == 0x03) goto L_019DE6;  /* 019DE1 BEQ */
    goto L_019EC8;  /* 019DE3 JMP */
L_019DE6: TICK();
    mflag = 0;  /* 019DE6 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0) goto L_019E3F;  /* 019DE7 BBC */
    A = rd16(DP(0x0B));  /* 019DED LDA */
    A &= 0x000F;  /* 019DEF AND */
    mflag = 1;  /* 019DF2 SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 019DF3 STA */
    SETLO(A, A & 0x0F);  /* 019DF5 AND */
    wr8(ABS(0x0298), rd8(ABS(0x0298)) & ~0x0F);  /* 019DF7 CLB */
    SETLO(A, A | rd8(ABS(0x0298)));  /* 019DFB ORA */
    wr8(ABS(0x0298), (uint8_t)A);  /* 019DFE STA */
    mflag = 0;  /* 019E01 CLM */
    A = rd16(DP(0x0D));  /* 019E02 LDA */
    A &= 0x000F;  /* 019E04 AND */
    mflag = 1;  /* 019E07 SEM */
    SETLO(A, A & 0x0F);  /* 019E08 AND */
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x0F);  /* 019E0A CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019E0E ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019E11 STA */
    mflag = 0;  /* 019E14 CLM */
    A = rd16(DP(0x0C));  /* 019E15 LDA */
    A &= 0x0007;  /* 019E17 AND */
    mflag = 1;  /* 019E1A SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 019E1B STA */
    SETLO(A, A & 0x07);  /* 019E1D AND */
    wr8(ABS(0x0299), rd8(ABS(0x0299)) & ~0x07);  /* 019E1F CLB */
    SETLO(A, A | rd8(ABS(0x0299)));  /* 019E23 ORA */
    wr8(ABS(0x0299), (uint8_t)A);  /* 019E26 STA */
    mflag = 0;  /* 019E29 CLM */
    A = rd16(DP(0x0D));  /* 019E2A LDA */
    A &= 0x0007;  /* 019E2C AND */
    mflag = 1;  /* 019E2F SEM */
    SETLO(A, A & 0x07);  /* 019E30 AND */
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x07);  /* 019E32 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 019E36 ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 019E39 STA */
    goto L_019E8F;  /* 019E3C JMP */
L_019E3F: TICK();
    mflag = 0;  /* 019E3F CLM */
    A = rd16(DP(0x0B));  /* 019E40 LDA */
    A &= 0x000F;  /* 019E42 AND */
    mflag = 1;  /* 019E45 SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 019E46 STA */
    SETLO(A, A & 0x0F);  /* 019E48 AND */
    wr8(ABS(0x029A), rd8(ABS(0x029A)) & ~0x0F);  /* 019E4A CLB */
    SETLO(A, A | rd8(ABS(0x029A)));  /* 019E4E ORA */
    wr8(ABS(0x029A), (uint8_t)A);  /* 019E51 STA */
    mflag = 0;  /* 019E54 CLM */
    A = rd16(DP(0x0D));  /* 019E55 LDA */
    A &= 0x000F;  /* 019E57 AND */
    mflag = 1;  /* 019E5A SEM */
    SETLO(A, A & 0x0F);  /* 019E5B AND */
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x0F);  /* 019E5D CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019E61 ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019E64 STA */
    mflag = 0;  /* 019E67 CLM */
    A = rd16(DP(0x0C));  /* 019E68 LDA */
    A &= 0x0007;  /* 019E6A AND */
    mflag = 1;  /* 019E6D SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 019E6E STA */
    SETLO(A, A & 0x07);  /* 019E70 AND */
    wr8(ABS(0x029B), rd8(ABS(0x029B)) & ~0x07);  /* 019E72 CLB */
    SETLO(A, A | rd8(ABS(0x029B)));  /* 019E76 ORA */
    wr8(ABS(0x029B), (uint8_t)A);  /* 019E79 STA */
    mflag = 0;  /* 019E7C CLM */
    A = rd16(DP(0x0D));  /* 019E7D LDA */
    A &= 0x0007;  /* 019E7F AND */
    mflag = 1;  /* 019E82 SEM */
    SETLO(A, A & 0x07);  /* 019E83 AND */
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x07);  /* 019E85 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 019E89 ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 019E8C STA */
L_019E8F: TICK();
    mflag = 0;  /* 019E8F CLM */
    A = rd16(ABS(0x0297));  /* 019E90 LDA */
    A &= 0x00FF;  /* 019E93 AND */
    wr16(DP(0x0D), A);  /* 019E96 STA */
    wr16(DP(0x0F), 0x0011);  /* 019E98 LDM */
    Y = 0x0002;  /* 019E9C LDY */
    A = D;  /* 019E9F TDA */
    C = 0;  /* 019EA0 CLC */
    A = adc16(A, 0x0011);  /* 019EA1 ADC */
    push16(A);  /* 019EA4 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 019EA5 JSRL */
    Y = pull16();  /* 019EA9 PLY */
    SETLO(A, rd8(ABS(0x0296)));  /* 019EAA LDA */
    mflag = 0;  /* 019EAD CLM */
    A &= 0x00FF;  /* 019EAE AND */
    wr16(DP(0x0D), A);  /* 019EB1 STA */
    wr16(DP(0x0F), 0x0010);  /* 019EB3 LDM */
    Y = 0x0002;  /* 019EB7 LDY */
    A = D;  /* 019EBA TDA */
    C = 0;  /* 019EBB CLC */
    A = adc16(A, 0x0011);  /* 019EBC ADC */
    push16(A);  /* 019EBF PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 019EC0 JSRL */
    Y = pull16();  /* 019EC4 PLY */
    goto L_01A1AF;  /* 019EC5 JMP */
L_019EC8: TICK();
    mflag = 0;  /* 019EC8 CLM */
    A = rd16(ABS(0x029D));  /* 019EC9 LDA */
    mflag = 1;  /* 019ECC SEM */
    /* 019ECD CMP */
    if ((A & 0xFF) != 0x04) goto L_019EFF;  /* 019ECF BNE */
    SETLO(A, rd8(ABS(0x029F)));  /* 019ED1 LDA */
    wr8(ABS(0x01FF), (uint8_t)A);  /* 019ED4 STA */
    mflag = 0;  /* 019ED7 CLM */
    A &= 0x00FF;  /* 019ED8 AND */
    wr16(DP(0x0D), A);  /* 019EDB STA */
    A = rd16(ABS(0x029E));  /* 019EDD LDA */
    mflag = 1;  /* 019EE0 SEM */
    wr8(ABS(0x01FE), (uint8_t)A);  /* 019EE1 STA */
    mflag = 0;  /* 019EE4 CLM */
    A &= 0x00FF;  /* 019EE5 AND */
    wr16(DP(0x0F), A);  /* 019EE8 STA */
    wr16(DP(0x11), 0x0012);  /* 019EEA LDM */
    Y = 0x0003;  /* 019EEE LDY */
    A = D;  /* 019EF1 TDA */
    C = 0;  /* 019EF2 CLC */
    A = adc16(A, 0x0013);  /* 019EF3 ADC */
    push16(A);  /* 019EF6 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 019EF7 JSRL */
    Y = pull16();  /* 019EFB PLY */
    goto L_01A1AF;  /* 019EFC JMP */
L_019EFF: TICK();
    mflag = 0;  /* 019EFF CLM */
    A = rd16(ABS(0x029D));  /* 019F00 LDA */
    mflag = 1;  /* 019F03 SEM */
    /* 019F04 CMP */
    if ((A & 0xFF) >= 0x05) goto L_019F0B;  /* 019F06 BCS */
    goto L_01A030;  /* 019F08 JMP */
L_019F0B: TICK();
    SETLO(A, rd8(ABS(0x029D)));  /* 019F0B LDA */
    /* 019F0E CMP */
    if ((A & 0xFF) < 0x0A) goto L_019F15;  /* 019F10 BCC */
    goto L_01A030;  /* 019F12 JMP */
L_019F15: TICK();
    /* 019F15 CMP */
    if ((A & 0xFF) != 0x05) goto L_019F75;  /* 019F17 BNE */
    mflag = 0;  /* 019F19 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0) goto L_019F4A;  /* 019F1A BBC */
    A = rd16(DP(0x0B));  /* 019F20 LDA */
    A &= 0x000F;  /* 019F22 AND */
    mflag = 1;  /* 019F25 SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 019F26 STA */
    SETLO(A, A & 0x0F);  /* 019F28 AND */
    wr8(ABS(0x0298), rd8(ABS(0x0298)) & ~0x0F);  /* 019F2A CLB */
    SETLO(A, A | rd8(ABS(0x0298)));  /* 019F2E ORA */
    wr8(ABS(0x0298), (uint8_t)A);  /* 019F31 STA */
    mflag = 0;  /* 019F34 CLM */
    A = rd16(DP(0x0D));  /* 019F35 LDA */
    A &= 0x000F;  /* 019F37 AND */
    mflag = 1;  /* 019F3A SEM */
    SETLO(A, A & 0x0F);  /* 019F3B AND */
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x0F);  /* 019F3D CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019F41 ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019F44 STA */
    goto L_019F72;  /* 019F47 JMP */
L_019F4A: TICK();
    mflag = 0;  /* 019F4A CLM */
    A = rd16(DP(0x0B));  /* 019F4B LDA */
    A &= 0x000F;  /* 019F4D AND */
    mflag = 1;  /* 019F50 SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 019F51 STA */
    SETLO(A, A & 0x0F);  /* 019F53 AND */
    wr8(ABS(0x029A), rd8(ABS(0x029A)) & ~0x0F);  /* 019F55 CLB */
    SETLO(A, A | rd8(ABS(0x029A)));  /* 019F59 ORA */
    wr8(ABS(0x029A), (uint8_t)A);  /* 019F5C STA */
    mflag = 0;  /* 019F5F CLM */
    A = rd16(DP(0x0D));  /* 019F60 LDA */
    A &= 0x000F;  /* 019F62 AND */
    mflag = 1;  /* 019F65 SEM */
    SETLO(A, A & 0x0F);  /* 019F66 AND */
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x0F);  /* 019F68 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019F6C ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019F6F STA */
L_019F72: TICK();
    goto L_01A012;  /* 019F72 JMP */
L_019F75: TICK();
    mflag = 0;  /* 019F75 CLM */
    A = rd16(ABS(0x029D));  /* 019F76 LDA */
    mflag = 1;  /* 019F79 SEM */
    /* 019F7A CMP */
    if ((A & 0xFF) != 0x06) goto L_019F9D;  /* 019F7C BNE */
    mflag = 0;  /* 019F7E CLM */
    A = rd16(DP(0x0B));  /* 019F7F LDA */
    A &= 0x0010;  /* 019F81 AND */
    if (A == 0) goto L_019F89;  /* 019F84 BEQ */
    A = 0x0001;  /* 019F86 LDA */
L_019F89: TICK();
    mflag = 1;  /* 019F89 SEM */
    SETLO(A, A & 0x01);  /* 019F8A AND */
    if ((A & 0xFF) == 0) goto L_019F90;  /* 019F8C BEQ */
    SETLO(A, 0x10);  /* 019F8E LDA */
L_019F90: TICK();
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x10);  /* 019F90 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019F94 ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019F97 STA */
    goto L_01A012;  /* 019F9A JMP */
L_019F9D: TICK();
    mflag = 0;  /* 019F9D CLM */
    A = rd16(ABS(0x029D));  /* 019F9E LDA */
    mflag = 1;  /* 019FA1 SEM */
    /* 019FA2 CMP */
    if ((A & 0xFF) != 0x07) goto L_019FC5;  /* 019FA4 BNE */
    mflag = 0;  /* 019FA6 CLM */
    A = rd16(DP(0x0B));  /* 019FA7 LDA */
    A &= 0x0020;  /* 019FA9 AND */
    if (A == 0) goto L_019FB1;  /* 019FAC BEQ */
    A = 0x0001;  /* 019FAE LDA */
L_019FB1: TICK();
    mflag = 1;  /* 019FB1 SEM */
    SETLO(A, A & 0x01);  /* 019FB2 AND */
    if ((A & 0xFF) == 0) goto L_019FB8;  /* 019FB4 BEQ */
    SETLO(A, 0x20);  /* 019FB6 LDA */
L_019FB8: TICK();
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x20);  /* 019FB8 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019FBC ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019FBF STA */
    goto L_01A012;  /* 019FC2 JMP */
L_019FC5: TICK();
    mflag = 0;  /* 019FC5 CLM */
    A = rd16(ABS(0x029D));  /* 019FC6 LDA */
    mflag = 1;  /* 019FC9 SEM */
    /* 019FCA CMP */
    if ((A & 0xFF) != 0x08) goto L_019FED;  /* 019FCC BNE */
    mflag = 0;  /* 019FCE CLM */
    A = rd16(DP(0x0B));  /* 019FCF LDA */
    A &= 0x0040;  /* 019FD1 AND */
    if (A == 0) goto L_019FD9;  /* 019FD4 BEQ */
    A = 0x0001;  /* 019FD6 LDA */
L_019FD9: TICK();
    mflag = 1;  /* 019FD9 SEM */
    SETLO(A, A & 0x01);  /* 019FDA AND */
    if ((A & 0xFF) == 0) goto L_019FE0;  /* 019FDC BEQ */
    SETLO(A, 0x40);  /* 019FDE LDA */
L_019FE0: TICK();
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x40);  /* 019FE0 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 019FE4 ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 019FE7 STA */
    goto L_01A012;  /* 019FEA JMP */
L_019FED: TICK();
    mflag = 0;  /* 019FED CLM */
    A = rd16(ABS(0x029D));  /* 019FEE LDA */
    mflag = 1;  /* 019FF1 SEM */
    /* 019FF2 CMP */
    if ((A & 0xFF) != 0x09) goto L_01A012;  /* 019FF4 BNE */
    mflag = 0;  /* 019FF6 CLM */
    A = rd16(DP(0x0B));  /* 019FF7 LDA */
    A &= 0x0080;  /* 019FF9 AND */
    if (A == 0) goto L_01A001;  /* 019FFC BEQ */
    A = 0x0001;  /* 019FFE LDA */
L_01A001: TICK();
    mflag = 1;  /* 01A001 SEM */
    SETLO(A, A & 0x01);  /* 01A002 AND */
    if ((A & 0xFF) == 0) goto L_01A008;  /* 01A004 BEQ */
    SETLO(A, 0x80);  /* 01A006 LDA */
L_01A008: TICK();
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x80);  /* 01A008 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 01A00C ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 01A00F STA */
L_01A012: TICK();
    SETLO(A, rd8(ABS(0x0296)));  /* 01A012 LDA */
    mflag = 0;  /* 01A015 CLM */
    A &= 0x00FF;  /* 01A016 AND */
    wr16(DP(0x0D), A);  /* 01A019 STA */
    wr16(DP(0x0F), 0x0010);  /* 01A01B LDM */
    Y = 0x0002;  /* 01A01F LDY */
    A = D;  /* 01A022 TDA */
    C = 0;  /* 01A023 CLC */
    A = adc16(A, 0x0011);  /* 01A024 ADC */
    push16(A);  /* 01A027 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01A028 JSRL */
    Y = pull16();  /* 01A02C PLY */
    goto L_01A1AF;  /* 01A02D JMP */
L_01A030: TICK();
    mflag = 0;  /* 01A030 CLM */
    A = rd16(ABS(0x029D));  /* 01A031 LDA */
    mflag = 1;  /* 01A034 SEM */
    /* 01A035 CMP */
    if ((A & 0xFF) >= 0x0A) goto L_01A03C;  /* 01A037 BCS */
    goto L_01A188;  /* 01A039 JMP */
L_01A03C: TICK();
    SETLO(A, rd8(ABS(0x029D)));  /* 01A03C LDA */
    /* 01A03F CMP */
    if ((A & 0xFF) < 0x10) goto L_01A046;  /* 01A041 BCC */
    goto L_01A188;  /* 01A043 JMP */
L_01A046: TICK();
    /* 01A046 CMP */
    if ((A & 0xFF) != 0x0A) goto L_01A0A6;  /* 01A048 BNE */
    mflag = 0;  /* 01A04A CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0) goto L_01A07B;  /* 01A04B BBC */
    A = rd16(DP(0x0C));  /* 01A051 LDA */
    A &= 0x0007;  /* 01A053 AND */
    mflag = 1;  /* 01A056 SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 01A057 STA */
    SETLO(A, A & 0x07);  /* 01A059 AND */
    wr8(ABS(0x0299), rd8(ABS(0x0299)) & ~0x07);  /* 01A05B CLB */
    SETLO(A, A | rd8(ABS(0x0299)));  /* 01A05F ORA */
    wr8(ABS(0x0299), (uint8_t)A);  /* 01A062 STA */
    mflag = 0;  /* 01A065 CLM */
    A = rd16(DP(0x0D));  /* 01A066 LDA */
    A &= 0x0007;  /* 01A068 AND */
    mflag = 1;  /* 01A06B SEM */
    SETLO(A, A & 0x07);  /* 01A06C AND */
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x07);  /* 01A06E CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A072 ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A075 STA */
    goto L_01A0A3;  /* 01A078 JMP */
L_01A07B: TICK();
    mflag = 0;  /* 01A07B CLM */
    A = rd16(DP(0x0C));  /* 01A07C LDA */
    A &= 0x0007;  /* 01A07E AND */
    mflag = 1;  /* 01A081 SEM */
    wr8(DP(0x0D), (uint8_t)A);  /* 01A082 STA */
    SETLO(A, A & 0x07);  /* 01A084 AND */
    wr8(ABS(0x029B), rd8(ABS(0x029B)) & ~0x07);  /* 01A086 CLB */
    SETLO(A, A | rd8(ABS(0x029B)));  /* 01A08A ORA */
    wr8(ABS(0x029B), (uint8_t)A);  /* 01A08D STA */
    mflag = 0;  /* 01A090 CLM */
    A = rd16(DP(0x0D));  /* 01A091 LDA */
    A &= 0x0007;  /* 01A093 AND */
    mflag = 1;  /* 01A096 SEM */
    SETLO(A, A & 0x07);  /* 01A097 AND */
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x07);  /* 01A099 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A09D ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A0A0 STA */
L_01A0A3: TICK();
    goto L_01A16A;  /* 01A0A3 JMP */
L_01A0A6: TICK();
    mflag = 0;  /* 01A0A6 CLM */
    A = rd16(ABS(0x029D));  /* 01A0A7 LDA */
    mflag = 1;  /* 01A0AA SEM */
    /* 01A0AB CMP */
    if ((A & 0xFF) != 0x0B) goto L_01A0CD;  /* 01A0AD BNE */
    mflag = 0;  /* 01A0AF CLM */
    A = rd16(DP(0x0B));  /* 01A0B0 LDA */
    A &= 0x0800;  /* 01A0B2 AND */
    if (A == 0) goto L_01A0BA;  /* 01A0B5 BEQ */
    A = 0x0001;  /* 01A0B7 LDA */
L_01A0BA: TICK();
    mflag = 1;  /* 01A0BA SEM */
    SETLO(A, A & 0x01);  /* 01A0BB AND */
    SETLO(A, asl8(A));  /* 01A0BD ASL */
    SETLO(A, asl8(A));  /* 01A0BE ASL */
    SETLO(A, asl8(A));  /* 01A0BF ASL */
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x08);  /* 01A0C0 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A0C4 ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A0C7 STA */
    goto L_01A16A;  /* 01A0CA JMP */
L_01A0CD: TICK();
    mflag = 0;  /* 01A0CD CLM */
    A = rd16(ABS(0x029D));  /* 01A0CE LDA */
    mflag = 1;  /* 01A0D1 SEM */
    /* 01A0D2 CMP */
    if ((A & 0xFF) != 0x0C) goto L_01A0F5;  /* 01A0D4 BNE */
    mflag = 0;  /* 01A0D6 CLM */
    A = rd16(DP(0x0B));  /* 01A0D7 LDA */
    A &= 0x1000;  /* 01A0D9 AND */
    if (A == 0) goto L_01A0E1;  /* 01A0DC BEQ */
    A = 0x0001;  /* 01A0DE LDA */
L_01A0E1: TICK();
    mflag = 1;  /* 01A0E1 SEM */
    SETLO(A, A & 0x01);  /* 01A0E2 AND */
    if ((A & 0xFF) == 0) goto L_01A0E8;  /* 01A0E4 BEQ */
    SETLO(A, 0x10);  /* 01A0E6 LDA */
L_01A0E8: TICK();
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x10);  /* 01A0E8 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A0EC ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A0EF STA */
    goto L_01A16A;  /* 01A0F2 JMP */
L_01A0F5: TICK();
    mflag = 0;  /* 01A0F5 CLM */
    A = rd16(ABS(0x029D));  /* 01A0F6 LDA */
    mflag = 1;  /* 01A0F9 SEM */
    /* 01A0FA CMP */
    if ((A & 0xFF) != 0x0D) goto L_01A11D;  /* 01A0FC BNE */
    mflag = 0;  /* 01A0FE CLM */
    A = rd16(DP(0x0B));  /* 01A0FF LDA */
    A &= 0x2000;  /* 01A101 AND */
    if (A == 0) goto L_01A109;  /* 01A104 BEQ */
    A = 0x0001;  /* 01A106 LDA */
L_01A109: TICK();
    mflag = 1;  /* 01A109 SEM */
    SETLO(A, A & 0x01);  /* 01A10A AND */
    if ((A & 0xFF) == 0) goto L_01A110;  /* 01A10C BEQ */
    SETLO(A, 0x20);  /* 01A10E LDA */
L_01A110: TICK();
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x20);  /* 01A110 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A114 ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A117 STA */
    goto L_01A16A;  /* 01A11A JMP */
L_01A11D: TICK();
    mflag = 0;  /* 01A11D CLM */
    A = rd16(ABS(0x029D));  /* 01A11E LDA */
    mflag = 1;  /* 01A121 SEM */
    /* 01A122 CMP */
    if ((A & 0xFF) != 0x0E) goto L_01A145;  /* 01A124 BNE */
    mflag = 0;  /* 01A126 CLM */
    A = rd16(DP(0x0B));  /* 01A127 LDA */
    A &= 0x4000;  /* 01A129 AND */
    if (A == 0) goto L_01A131;  /* 01A12C BEQ */
    A = 0x0001;  /* 01A12E LDA */
L_01A131: TICK();
    mflag = 1;  /* 01A131 SEM */
    SETLO(A, A & 0x01);  /* 01A132 AND */
    if ((A & 0xFF) == 0) goto L_01A138;  /* 01A134 BEQ */
    SETLO(A, 0x40);  /* 01A136 LDA */
L_01A138: TICK();
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x40);  /* 01A138 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A13C ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A13F STA */
    goto L_01A16A;  /* 01A142 JMP */
L_01A145: TICK();
    mflag = 0;  /* 01A145 CLM */
    A = rd16(ABS(0x029D));  /* 01A146 LDA */
    mflag = 1;  /* 01A149 SEM */
    /* 01A14A CMP */
    if ((A & 0xFF) != 0x0F) goto L_01A16A;  /* 01A14C BNE */
    mflag = 0;  /* 01A14E CLM */
    A = rd16(DP(0x0B));  /* 01A14F LDA */
    A &= 0x8000;  /* 01A151 AND */
    if (A == 0) goto L_01A159;  /* 01A154 BEQ */
    A = 0x0001;  /* 01A156 LDA */
L_01A159: TICK();
    mflag = 1;  /* 01A159 SEM */
    SETLO(A, A & 0x01);  /* 01A15A AND */
    if ((A & 0xFF) == 0) goto L_01A160;  /* 01A15C BEQ */
    SETLO(A, 0x80);  /* 01A15E LDA */
L_01A160: TICK();
    wr8(ABS(0x0297), rd8(ABS(0x0297)) & ~0x80);  /* 01A160 CLB */
    SETLO(A, A | rd8(ABS(0x0297)));  /* 01A164 ORA */
    wr8(ABS(0x0297), (uint8_t)A);  /* 01A167 STA */
L_01A16A: TICK();
    mflag = 0;  /* 01A16A CLM */
    A = rd16(ABS(0x0297));  /* 01A16B LDA */
    A &= 0x00FF;  /* 01A16E AND */
    wr16(DP(0x0D), A);  /* 01A171 STA */
    wr16(DP(0x0F), 0x0011);  /* 01A173 LDM */
    Y = 0x0002;  /* 01A177 LDY */
    A = D;  /* 01A17A TDA */
    C = 0;  /* 01A17B CLC */
    A = adc16(A, 0x0011);  /* 01A17C ADC */
    push16(A);  /* 01A17F PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01A180 JSRL */
    Y = pull16();  /* 01A184 PLY */
    goto L_01A1AF;  /* 01A185 JMP */
L_01A188: TICK();
    mflag = 0;  /* 01A188 CLM */
    A = rd16(ABS(0x029D));  /* 01A189 LDA */
    mflag = 1;  /* 01A18C SEM */
    /* 01A18D CMP */
    if ((A & 0xFF) != 0x10) goto L_01A1AF;  /* 01A18F BNE */
    SETLO(A, rd8(ABS(0x029E)));  /* 01A191 LDA */
    wr8(ABS(0x0200), (uint8_t)A);  /* 01A194 STA */
    mflag = 0;  /* 01A197 CLM */
    A &= 0x00FF;  /* 01A198 AND */
    wr16(DP(0x0D), A);  /* 01A19B STA */
    wr16(DP(0x0F), 0x0013);  /* 01A19D LDM */
    Y = 0x0002;  /* 01A1A1 LDY */
    A = D;  /* 01A1A4 TDA */
    C = 0;  /* 01A1A5 CLC */
    A = adc16(A, 0x0011);  /* 01A1A6 ADC */
    push16(A);  /* 01A1A9 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01A1AA JSRL */
    Y = pull16();  /* 01A1AE PLY */
L_01A1AF: TICK();
    goto L_01A2B6;  /* 01A1AF JMP */
L_01A1B2: TICK();
    mflag = 0;  /* 01A1B2 CLM */
    A = rd16(ABS(0x029C));  /* 01A1B3 LDA */
    mflag = 1;  /* 01A1B6 SEM */
    /* 01A1B7 CMP */
    if ((A & 0xFF) != 0x0A) goto L_01A1CA;  /* 01A1B9 BNE */
    mflag = 0;  /* 01A1BB CLM */
    S -= 3; F_019673();  /* 01A1BC JSRL */
    Y = rd16(ABS(0x029D));  /* 01A1C0 LDY */
    S -= 3; F_005822();  /* 01A1C3 JSRL */
    goto L_01A2B6;  /* 01A1C7 JMP */
L_01A1CA: TICK();
    mflag = 0;  /* 01A1CA CLM */
    A = rd16(ABS(0x029C));  /* 01A1CB LDA */
    mflag = 1;  /* 01A1CE SEM */
    /* 01A1CF CMP */
    if ((A & 0xFF) != 0x0B) goto L_01A215;  /* 01A1D1 BNE */
    mflag = 0;  /* 01A1D3 CLM */
    S -= 3; F_019673();  /* 01A1D4 JSRL */
    A = rd16(ABS(0x029D));  /* 01A1D8 LDA */
    mflag = 1;  /* 01A1DB SEM */
    SETLO(A, A - 1);  /* 01A1DC DEC */
    if ((A & 0xFF) != 0) goto L_01A201;  /* 01A1DD BNE */
    SETLO(A, rd8(ABS(0x029E)));  /* 01A1DF LDA */
    wr8(ABS(0x029D), 0x02);  /* 01A1E2 LDM */
    wr8(ABS(0x029E), 0x01);  /* 01A1E6 LDM */
    Y = 0x0000;  /* 01A1EA LDY */
    wr8(DP(0x02), (uint8_t)A);  /* 01A1ED STA */
    mflag = 0;  /* 01A1EF CLM */
    S -= 3; F_0196AA();  /* 01A1F0 JSRL */
    S -= 3; F_019852();  /* 01A1F4 JSRL */
    Y = rd16(DP(0x02));  /* 01A1F8 LDY */
    S -= 3; F_019884();  /* 01A1FA JSRL */
    goto L_01A212;  /* 01A1FE JMP */
L_01A201: TICK();
    mflag = 0;  /* 01A201 CLM */
    A = rd16(ABS(0x029D));  /* 01A202 LDA */
    mflag = 1;  /* 01A205 SEM */
    /* 01A206 CMP */
    if ((A & 0xFF) != 0x02) goto L_01A212;  /* 01A208 BNE */
    SETLO(A, rd8(ABS(0x029E)));  /* 01A20A LDA */
    if ((A & 0xFF) == 0) goto L_01A212;  /* 01A20D BEQ */
    wr8(DP(0x03), 0x02);  /* 01A20F LDM */
L_01A212: TICK();
    goto L_01A2B6;  /* 01A212 JMP */
L_01A215: TICK();
    mflag = 0;  /* 01A215 CLM */
    A = rd16(ABS(0x029C));  /* 01A216 LDA */
    mflag = 1;  /* 01A219 SEM */
    /* 01A21A CMP */
    if ((A & 0xFF) != 0x0C) goto L_01A237;  /* 01A21C BNE */
    mflag = 0;  /* 01A21E CLM */
    S -= 3; F_019673();  /* 01A21F JSRL */
    mflag = 1;  /* 01A223 SEM */
    wr8(ABS(0x029D), 0x01);  /* 01A224 LDM */
    wr8(ABS(0x029E), 0x00);  /* 01A228 LDM */
    Y = 0x0000;  /* 01A22C LDY */
    mflag = 0;  /* 01A22F CLM */
    S -= 3; F_0196AA();  /* 01A230 JSRL */
    goto L_01A2B6;  /* 01A234 JMP */
L_01A237: TICK();
    mflag = 0;  /* 01A237 CLM */
    A = rd16(ABS(0x029C));  /* 01A238 LDA */
    mflag = 1;  /* 01A23B SEM */
    /* 01A23C CMP */
    if ((A & 0xFF) != 0x0D) goto L_01A263;  /* 01A23E BNE */
    mflag = 0;  /* 01A240 CLM */
    S -= 3; F_019673();  /* 01A241 JSRL */
    A = rd16(ABS(0x029D));  /* 01A245 LDA */
    mflag = 1;  /* 01A248 SEM */
    wr8(ABS(0x0114), (uint8_t)A);  /* 01A249 STA */
    /* 01A24C CMP */
    if ((A & 0xFF) != 0x03) goto L_01A260;  /* 01A24E BNE */
L_01A250: TICK();
    SETLO(A, rd8(ABS(0x0332)));  /* 01A250 LDA */
    t = rd8(ABS(0x0333));  /* 01A253 CMP */
    if ((A & 0xFF) == t) goto L_01A25B;  /* 01A256 BEQ */
    goto L_01A250;  /* 01A258 JMP */
L_01A25B: TICK();
    mflag = 0;  /* 01A25B CLM */
    S -= 3; F_0105C9();  /* 01A25C JSRL */
L_01A260: TICK();
    goto L_01A2B6;  /* 01A260 JMP */
L_01A263: TICK();
    mflag = 0;  /* 01A263 CLM */
    A = rd16(ABS(0x029C));  /* 01A264 LDA */
    mflag = 1;  /* 01A267 SEM */
    /* 01A268 CMP */
    if ((A & 0xFF) != 0x80) goto L_01A27F;  /* 01A26A BNE */
    mflag = 0;  /* 01A26C CLM */
    S -= 3; F_019673();  /* 01A26D JSRL */
    wr16(DP(0x0D), 0x0006);  /* 01A271 LDM */
    Y = 0x0080;  /* 01A275 LDY */
    S -= 3; F_014EB2();  /* 01A278 JSRL */
    goto L_01A2B6;  /* 01A27C JMP */
L_01A27F: TICK();
    mflag = 0;  /* 01A27F CLM */
    A = rd16(ABS(0x029C));  /* 01A280 LDA */
    mflag = 1;  /* 01A283 SEM */
    /* 01A284 CMP */
    if ((A & 0xFF) != 0x82) goto L_01A2A1;  /* 01A286 BNE */
    mflag = 0;  /* 01A288 CLM */
    S -= 3; F_019673();  /* 01A289 JSRL */
    A = rd16(ABS(0x010E));  /* 01A28D LDA */
    C = 0;  /* 01A290 CLC */
    mflag = 1;  /* 01A291 SEM */
    SETLO(A, adc8(A, 0xF6));  /* 01A292 ADC */
    wr8(DP(0x0D), (uint8_t)A);  /* 01A294 STA */
    Y = 0x0082;  /* 01A296 LDY */
    mflag = 0;  /* 01A299 CLM */
    S -= 3; F_014EB2();  /* 01A29A JSRL */
    goto L_01A2B6;  /* 01A29E JMP */
L_01A2A1: TICK();
    mflag = 0;  /* 01A2A1 CLM */
    A = rd16(ABS(0x029C));  /* 01A2A2 LDA */
    mflag = 1;  /* 01A2A5 SEM */
    /* 01A2A6 CMP */
    if ((A & 0xFF) == 0xC0) goto L_01A2B1;  /* 01A2A8 BEQ */
    SETLO(A, rd8(ABS(0x029C)));  /* 01A2AA LDA */
    /* 01A2AD CMP */
    if ((A & 0xFF) != 0xE0) goto L_01A2B6;  /* 01A2AF BNE */
L_01A2B1: TICK();
    mflag = 0;  /* 01A2B1 CLM */
    S -= 3; F_019673();  /* 01A2B2 JSRL */
L_01A2B6: TICK();
    goto L_01A2BE;  /* 01A2B6 JMP */
L_01A2B9: TICK();
    mflag = 0;  /* 01A2B9 CLM */
    S -= 3; F_019673();  /* 01A2BA JSRL */
L_01A2BE: TICK();
    goto L_01A2EE;  /* 01A2BE JMP */
L_01A2C1: TICK();
    mflag = 1;  /* 01A2C1 SEM */
    SETLO(A, rd8(ABS(0x0080)));  /* 01A2C2 LDA */
    SETLO(A, A & 0x04);  /* 01A2C5 AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_01A2CB;  /* 01A2C7 BEQ */
    SETLO(A, 0x01);  /* 01A2C9 LDA */
    Z = ((A & 0xFF) == 0);
L_01A2CB: TICK();
    if (Z) goto L_01A2EE;  /* 01A2CB BEQ */
    SETLO(A, rd8(ABS(0x0112)));  /* 01A2CD LDA */
    if ((A & 0xFF) != 0) goto L_01A2EE;  /* 01A2D0 BNE */
    SETLO(A, rd8(ABS(0x0113)));  /* 01A2D2 LDA */
    if ((A & 0xFF) != 0) goto L_01A2E2;  /* 01A2D5 BNE */
    Y = rd16(ABS(0x01EA));  /* 01A2D7 LDY */
    mflag = 0;  /* 01A2DA CLM */
    S -= 3; F_019884();  /* 01A2DB JSRL */
    goto L_01A2EE;  /* 01A2DF JMP */
L_01A2E2: TICK();
    mflag = 1;  /* 01A2E2 SEM */
    t = (rd8(ABS(0x0113)) - 1) & 0xFF; wr8(ABS(0x0113), t);  /* 01A2E3 DEC */
    Y = rd16(ABS(0x02A2));  /* 01A2E6 LDY */
    mflag = 0;  /* 01A2E9 CLM */
    S -= 3; F_0196AA();  /* 01A2EA JSRL */
L_01A2EE: TICK();
    mflag = 0;  /* 01A2EE CLM */
    A = rd16(ABS(0x0114));  /* 01A2EF LDA */
    mflag = 1;  /* 01A2F2 SEM */
    /* 01A2F3 CMP */
    if ((A & 0xFF) != 0x02) goto L_01A2FA;  /* 01A2F5 BNE */
    goto L_0198E2;  /* 01A2F7 JMP */
L_01A2FA: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01A2FA LDA */
    Y = pull16();  /* 01A2FC PLY */
    D = pull16();  /* 01A2FD PLD */
    S += 3; return;  /* 01A2FE RTL */
}

/* 01A2FF  callers: 01A414, 01A4A7, 01A517, 01A52F, 01A537 */
void F_01A2FF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A2FF LDA */
    push16(D);  /* 01A301 PHD */
    D = A;  /* 01A302 TAD */
    C = 0;  /* 01A303 CLC */
    A = adc16(A, 0x0006);  /* 01A304 ADC */
    push16(A);  /* 01A307 PHA */
    wr16(DP(0x00), Y);  /* 01A308 STY */
    wr16(DP(0x02), 0x0000);  /* 01A30A LDM */
    mflag = 1;  /* 01A30E SEM */
    wr8(DP(0x01), 0x07);  /* 01A30F LDM */
L_01A312: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 01A312 LDA */
    /* 01A314 CMP */
    if ((A & 0xFF) >= 0x02) goto L_01A322;  /* 01A316 BCS */
    wr8(DP(0x01), (uint8_t)A);  /* 01A318 STA */
    mflag = 0;  /* 01A31A CLM */
    wr16(DP(0x02), 0x0001);  /* 01A31B LDM */
    goto L_01A393;  /* 01A31F JMP */
L_01A322: TICK();
    mflag = 1;  /* 01A322 SEM */
    /* 01A323 CMP */
    if ((A & 0xFF) != 0x02) goto L_01A348;  /* 01A325 BNE */
    SETLO(A, rd8(DP(0x01)));  /* 01A327 LDA */
    SETLO(A, A - 1);  /* 01A329 DEC */
    if ((A & 0xFF) != 0) goto L_01A332;  /* 01A32A BNE */
    wr8(DP(0x01), 0x02);  /* 01A32C LDM */
    goto L_01A345;  /* 01A32F JMP */
L_01A332: TICK();
    mflag = 1;  /* 01A332 SEM */
    /* 01A333 CMP */
    if ((A & 0xFF) != 0x01) goto L_01A33D;  /* 01A335 BNE */
    wr8(DP(0x01), 0x07);  /* 01A337 LDM */
    goto L_01A345;  /* 01A33A JMP */
L_01A33D: TICK();
    mflag = 1;  /* 01A33D SEM */
    /* 01A33E CMP */
    if ((A & 0xFF) != 0x06) goto L_01A345;  /* 01A340 BNE */
    wr8(DP(0x01), 0x01);  /* 01A342 LDM */
L_01A345: TICK();
    goto L_01A393;  /* 01A345 JMP */
L_01A348: TICK();
    mflag = 1;  /* 01A348 SEM */
    /* 01A349 CMP */
    if ((A & 0xFF) != 0x03) goto L_01A36F;  /* 01A34B BNE */
    SETLO(A, rd8(DP(0x01)));  /* 01A34D LDA */
    /* 01A34F CMP */
    if ((A & 0xFF) != 0x03) goto L_01A359;  /* 01A351 BNE */
    wr8(DP(0x01), 0x05);  /* 01A353 LDM */
    goto L_01A36C;  /* 01A356 JMP */
L_01A359: TICK();
    mflag = 1;  /* 01A359 SEM */
    /* 01A35A CMP */
    if ((A & 0xFF) != 0x05) goto L_01A364;  /* 01A35C BNE */
    wr8(DP(0x01), 0x07);  /* 01A35E LDM */
    goto L_01A36C;  /* 01A361 JMP */
L_01A364: TICK();
    mflag = 1;  /* 01A364 SEM */
    /* 01A365 CMP */
    if ((A & 0xFF) != 0x07) goto L_01A36C;  /* 01A367 BNE */
    wr8(DP(0x01), 0x03);  /* 01A369 LDM */
L_01A36C: TICK();
    goto L_01A393;  /* 01A36C JMP */
L_01A36F: TICK();
    mflag = 1;  /* 01A36F SEM */
    /* 01A370 CMP */
    if ((A & 0xFF) != 0x04) goto L_01A393;  /* 01A372 BNE */
    SETLO(A, rd8(DP(0x01)));  /* 01A374 LDA */
    /* 01A376 CMP */
    if ((A & 0xFF) != 0x04) goto L_01A380;  /* 01A378 BNE */
    wr8(DP(0x01), 0x06);  /* 01A37A LDM */
    goto L_01A393;  /* 01A37D JMP */
L_01A380: TICK();
    mflag = 1;  /* 01A380 SEM */
    /* 01A381 CMP */
    if ((A & 0xFF) != 0x06) goto L_01A38B;  /* 01A383 BNE */
    wr8(DP(0x01), 0x07);  /* 01A385 LDM */
    goto L_01A393;  /* 01A388 JMP */
L_01A38B: TICK();
    mflag = 1;  /* 01A38B SEM */
    /* 01A38C CMP */
    if ((A & 0xFF) != 0x07) goto L_01A393;  /* 01A38E BNE */
    wr8(DP(0x01), 0x04);  /* 01A390 LDM */
L_01A393: TICK();
    mflag = 0;  /* 01A393 CLM */
    wr16(DP(0x04), 0x0000);  /* 01A394 LDM */
L_01A398: TICK();
    X = rd16(DP(0x04));  /* 01A398 LDX */
    /* 01A39A CPX */
    if (X >= 0x0008) goto L_01A3D7;  /* 01A39D BCS */
    mflag = 1;  /* 01A39F SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_01A3B9;  /* 01A3A0 BBC */
    SETLO(A, rd8(DP(0x01)));  /* 01A3A5 LDA */
    mflag = 0;  /* 01A3A7 CLM */
    A &= 0x00FF;  /* 01A3A8 AND */
    A = asl16(A);  /* 01A3AB ASL */
    A = asl16(A);  /* 01A3AC ASL */
    A = asl16(A);  /* 01A3AD ASL */
    C = 0;  /* 01A3AE CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01A3AF ADC */
    X = A;  /* 01A3B1 TAX */
    A = rd16((0x0020C8 + X));  /* 01A3B2 LDA */
    goto L_01A3CA;  /* 01A3B6 JMP */
L_01A3B9: TICK();
    mflag = 0;  /* 01A3B9 CLM */
    A = rd16(DP(0x01));  /* 01A3BA LDA */
    A &= 0x00FF;  /* 01A3BC AND */
    A = asl16(A);  /* 01A3BF ASL */
    A = asl16(A);  /* 01A3C0 ASL */
    A = asl16(A);  /* 01A3C1 ASL */
    C = 0;  /* 01A3C2 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01A3C3 ADC */
    X = A;  /* 01A3C5 TAX */
    A = rd16((0x002088 + X));  /* 01A3C6 LDA */
L_01A3CA: TICK();
    X = rd16(DP(0x04));  /* 01A3CA LDX */
    mflag = 1;  /* 01A3CC SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 01A3CD STA */
    X += 1;  /* 01A3D0 INX */
    wr16(DP(0x04), X);  /* 01A3D1 STX */
    mflag = 0;  /* 01A3D3 CLM */
    goto L_01A398;  /* 01A3D4 JMP */
L_01A3D7: TICK();
    mflag = 0;  /* 01A3D7 CLM */
    S -= 3; F_00828C();  /* 01A3D8 JSRL */
    Y = 0x03E8;  /* 01A3DC LDY */
    S -= 3; F_007D7F();  /* 01A3DF JSRL */
    X = rd16(DP(0x02));  /* 01A3E3 LDX */
    Z = (X == 0);
    mflag = 1;  /* 01A3E5 SEM */
    if (!Z) goto L_01A3EB;  /* 01A3E6 BNE */
    goto L_01A312;  /* 01A3E8 JMP */
L_01A3EB: TICK();
    mflag = 0;  /* 01A3EB CLM */
    Y = pull16();  /* 01A3EC PLY */
    D = pull16();  /* 01A3ED PLD */
    S += 3; return;  /* 01A3EE RTL */
}

/* 01A3EF  callers: 01A74C */
void F_01A3EF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A3EF LDA */
    push16(D);  /* 01A3F1 PHD */
    D = A;  /* 01A3F2 TAD */
    C = 0;  /* 01A3F3 CLC */
    A = adc16(A, 0x000A);  /* 01A3F4 ADC */
    push16(A);  /* 01A3F7 PHA */
    wr16(DP(0x06), 0x0000);  /* 01A3F8 LDM */
    mflag = 1;  /* 01A3FC SEM */
    if ((rd8(ABS(0x033F)) & 0x10) == 0) goto L_01A40C;  /* 01A3FD BBC */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_01A40C;  /* 01A402 BBS */
    if ((rd8(ABS(P4)) & 0x10) == 0) goto L_01A410;  /* 01A407 BBC [P44 KONTAK_ANAHTARI] */
L_01A40C: TICK();
    mflag = 0;  /* 01A40C CLM */
    Y = pull16();  /* 01A40D PLY */
    D = pull16();  /* 01A40E PLD */
    S += 3; return;  /* 01A40F RTL */
L_01A410: TICK();
    Y = 0x0000;  /* 01A410 LDY */
    mflag = 0;  /* 01A413 CLM */
    S -= 3; F_01A2FF();  /* 01A414 JSRL */
    S -= 3; F_0195DA();  /* 01A418 JSRL */
    mflag = 1;  /* 01A41C SEM */
    wr8(DP(0x00), 0x00);  /* 01A41D LDM */
L_01A420: TICK();
    wr8(ABS(WDT), 0x00);  /* 01A420 LDM */
    mflag = 0;  /* 01A424 CLM */
    S -= 3; measurements_0198D5();  /* 01A425 JSRL */
    wr8(DP(0x01), (uint8_t)A);  /* 01A429 STA */
    SETLO(A, rd8(ABS(0x0111)));  /* 01A42B LDA */
    if ((A & 0xFF) != 0) goto L_01A44B;  /* 01A42E BNE */
    SETLO(A, rd8(ABS(0x0080)));  /* 01A430 LDA */
    SETLO(A, A & 0x02);  /* 01A433 AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_01A439;  /* 01A435 BEQ */
    SETLO(A, 0x01);  /* 01A437 LDA */
    Z = ((A & 0xFF) == 0);
L_01A439: TICK();
    if (Z) goto L_01A44B;  /* 01A439 BEQ */
    SETLO(A, rd8(ABS(0x0116)));  /* 01A43B LDA */
    /* 01A43E CMP */
    if ((A & 0xFF) != 0x09) goto L_01A44B;  /* 01A440 BNE */
    wr8(ABS(0x0116), 0x08);  /* 01A442 LDM */
    mflag = 0;  /* 01A446 CLM */
    S -= 3; F_0195DA();  /* 01A447 JSRL */
L_01A44B: TICK();
    mflag = 0;  /* 01A44B CLM */
    A = rd16(DP(0x00));  /* 01A44C LDA */
    mflag = 1;  /* 01A44E SEM */
    /* 01A44F CMP */
    if ((A & 0xFF) != 0x00) goto L_01A48B;  /* 01A451 BNE */
    SETLO(A, rd8(ABS(0x0116)));  /* 01A453 LDA */
    /* 01A456 CMP */
    if ((A & 0xFF) != 0x0A) goto L_01A488;  /* 01A458 BNE */
    wr8(DP(0x00), 0x02);  /* 01A45A LDM */
    wr8(ABS(0x029C), 0x03);  /* 01A45D LDM */
    wr8(ABS(0x029D), 0x01);  /* 01A461 LDM */
    wr8(ABS(0x029E), 0x01);  /* 01A465 LDM */
    wr8(ABS(0x029F), 0x00);  /* 01A469 LDM */
    wr8(ABS(0x02A0), 0x00);  /* 01A46D LDM */
    wr8(ABS(0x02A1), 0x0A);  /* 01A471 LDM */
    wr8(ABS(0x02A2), 0x01);  /* 01A475 LDM */
    Y = 0x0064;  /* 01A479 LDY */
    mflag = 0;  /* 01A47C CLM */
    S -= 3; F_007D7F();  /* 01A47D JSRL */
    Y = 0x0001;  /* 01A481 LDY */
    S -= 3; F_0196AA();  /* 01A484 JSRL */
L_01A488: TICK();
    goto L_01A5FB;  /* 01A488 JMP */
L_01A48B: TICK();
    mflag = 1;  /* 01A48B SEM */
    /* 01A48C CMP */
    if ((A & 0xFF) != 0x02) goto L_01A4D6;  /* 01A48E BNE */
    SETLO(A, rd8(DP(0x01)));  /* 01A490 LDA */
    /* 01A492 CMP */
    if ((A & 0xFF) != 0x02) goto L_01A4D3;  /* 01A494 BNE */
    Y = 0x000A;  /* 01A496 LDY */
    mflag = 0;  /* 01A499 CLM */
    S -= 3; eeprom_read_byte_007189();  /* 01A49A JSRL */
    t = rd8(ABS(0x008D));  /* 01A49E CMP */
    if ((A & 0xFF) == t) goto L_01A4AB;  /* 01A4A1 BEQ */
    Y = 0x0002;  /* 01A4A3 LDY */
    mflag = 0;  /* 01A4A6 CLM */
    S -= 3; F_01A2FF();  /* 01A4A7 JSRL */
L_01A4AB: TICK();
    mflag = 1;  /* 01A4AB SEM */
    wr8(DP(0x00), 0x03);  /* 01A4AC LDM */
    wr8(ABS(0x029C), 0x03);  /* 01A4AF LDM */
    wr8(ABS(0x029D), 0x01);  /* 01A4B3 LDM */
    wr8(ABS(0x029E), 0x01);  /* 01A4B7 LDM */
    wr8(ABS(0x029F), 0x00);  /* 01A4BB LDM */
    wr8(ABS(0x02A0), 0x05);  /* 01A4BF LDM */
    wr8(ABS(0x02A1), 0x8C);  /* 01A4C3 LDM */
    wr8(ABS(0x02A2), 0x03);  /* 01A4C7 LDM */
    Y = 0x0003;  /* 01A4CB LDY */
    mflag = 0;  /* 01A4CE CLM */
    S -= 3; F_0196AA();  /* 01A4CF JSRL */
L_01A4D3: TICK();
    goto L_01A5FB;  /* 01A4D3 JMP */
L_01A4D6: TICK();
    mflag = 1;  /* 01A4D6 SEM */
    /* 01A4D7 CMP */
    if ((A & 0xFF) == 0x03) goto L_01A4DE;  /* 01A4D9 BEQ */
    goto L_01A582;  /* 01A4DB JMP */
L_01A4DE: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 01A4DE LDA */
    /* 01A4E0 CMP */
    if ((A & 0xFF) == 0x02) goto L_01A4E7;  /* 01A4E2 BEQ */
    goto L_01A57F;  /* 01A4E4 JMP */
L_01A4E7: TICK();
    SETLO(A, rd8(ABS(0x008D)));  /* 01A4E7 LDA */
    mflag = 0;  /* 01A4EA CLM */
    A &= 0x00FF;  /* 01A4EB AND */
    A = rla16(A, 0x0008);  /* 01A4EE RLA */
    A &= 0xFF00;  /* 01A4F2 AND */
    wr16(DP(0x0A), A);  /* 01A4F5 STA */
    A = rd16(ABS(0x008E));  /* 01A4F7 LDA */
    A &= 0x00FF;  /* 01A4FA AND */
    C = 0;  /* 01A4FD CLC */
    A = adc16(A, rd16(DP(0x0A)));  /* 01A4FE ADC */
    wr16(DP(0x0A), A);  /* 01A500 STA */
    Y = 0x058C;  /* 01A502 LDY */
    A = D;  /* 01A505 TDA */
    C = 0;  /* 01A506 CLC */
    A = adc16(A, 0x000C);  /* 01A507 ADC */
    push16(A);  /* 01A50A PHA */
    S -= 3; eeprom_read_word_0071F0();  /* 01A50B JSRL */
    Y = pull16();  /* 01A50F PLY */
    t = rd16(DP(0x0A));  /* 01A510 CMP */
    if (A == t) goto L_01A51E;  /* 01A512 BEQ */
    Y = 0x0003;  /* 01A514 LDY */
    S -= 3; F_01A2FF();  /* 01A517 JSRL */
    goto L_01A533;  /* 01A51B JMP */
L_01A51E: TICK();
    Y = 0x058E;  /* 01A51E LDY */
    mflag = 0;  /* 01A521 CLM */
    S -= 3; eeprom_read_byte_007189();  /* 01A522 JSRL */
    t = rd8(ABS(0x008F));  /* 01A526 CMP */
    if ((A & 0xFF) == t) goto L_01A533;  /* 01A529 BEQ */
    Y = 0x0004;  /* 01A52B LDY */
    mflag = 0;  /* 01A52E CLM */
    S -= 3; F_01A2FF();  /* 01A52F JSRL */
L_01A533: TICK();
    Y = 0x0001;  /* 01A533 LDY */
    mflag = 0;  /* 01A536 CLM */
    S -= 3; F_01A2FF();  /* 01A537 JSRL */
    mflag = 1;  /* 01A53B SEM */
    wr8(DP(0x00), 0x01);  /* 01A53C LDM */
    wr8(DP(0x0A), 0x80);  /* 01A53F LDM */
    mflag = 0;  /* 01A542 CLM */
    wr16(DP(0x0B), 0x008D);  /* 01A543 LDM */
    wr16(DP(0x0D), 0x0000);  /* 01A547 LDM */
    wr16(DP(0x0E), 0x0018);  /* 01A54B LDM */
    Y = 0x00A0;  /* 01A54F LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 01A552 JSRL */
    mflag = 1;  /* 01A556 SEM */
    wr8(ABS(0x029C), 0x03);  /* 01A557 LDM */
    wr8(ABS(0x029D), 0x03);  /* 01A55B LDM */
    wr8(ABS(0x029E), 0x01);  /* 01A55F LDM */
    wr8(ABS(0x029F), 0x00);  /* 01A563 LDM */
    wr8(ABS(0x02A0), 0x00);  /* 01A567 LDM */
    wr8(ABS(0x02A1), 0x18);  /* 01A56B LDM */
    mflag = 0;  /* 01A56F CLM */
    wr16(DP(0x02), 0x0080);  /* 01A570 LDM */
    wr16(DP(0x08), 0x0080);  /* 01A574 LDM */
    Y = 0x0080;  /* 01A578 LDY */
    S -= 3; F_0196AA();  /* 01A57B JSRL */
L_01A57F: TICK();
    goto L_01A5FB;  /* 01A57F JMP */
L_01A582: TICK();
    mflag = 1;  /* 01A582 SEM */
    /* 01A583 CMP */
    if ((A & 0xFF) != 0x01) goto L_01A5FB;  /* 01A585 BNE */
    SETLO(A, rd8(DP(0x01)));  /* 01A587 LDA */
    /* 01A589 CMP */
    if ((A & 0xFF) != 0x01) goto L_01A5FB;  /* 01A58B BNE */
    mflag = 0;  /* 01A58D CLM */
    X = rd16(DP(0x08));  /* 01A58E LDX */
    /* 01A590 CPX */
    if (X == 0x0080) goto L_01A59C;  /* 01A593 BEQ */
    wr16(DP(0x06), 0x0001);  /* 01A595 LDM */
    goto L_01A5FB;  /* 01A599 JMP */
L_01A59C: TICK();
    mflag = 0;  /* 01A59C CLM */
    A = rd16(DP(0x02));  /* 01A59D LDA */
    C = 0;  /* 01A59F CLC */
    A = adc16(A, 0x0018);  /* 01A5A0 ADC */
    wr16(DP(0x04), A);  /* 01A5A3 STA */
    X = rd16(DP(0x02));  /* 01A5A5 LDX */
    /* 01A5A7 CPX */
    if (X != 0x0500) goto L_01A5B2;  /* 01A5AA BNE */
    A = 0x002D;  /* 01A5AC LDA */
    goto L_01A5B5;  /* 01A5AF JMP */
L_01A5B2: TICK();
    A = 0x0080;  /* 01A5B2 LDA */
L_01A5B5: TICK();
    wr16(DP(0x08), A);  /* 01A5B5 STA */
    wr16(DP(0x0A), A);  /* 01A5B7 STA */
    wr16(DP(0x0B), 0x008D);  /* 01A5B9 LDM */
    wr16(DP(0x0D), 0x0000);  /* 01A5BD LDM */
    X = rd16(DP(0x04));  /* 01A5C1 LDX */
    wr16(DP(0x0E), X);  /* 01A5C3 STX */
    Y = 0x00A0;  /* 01A5C5 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 01A5C8 JSRL */
    mflag = 1;  /* 01A5CC SEM */
    wr8(ABS(0x029C), 0x03);  /* 01A5CD LDM */
    wr8(ABS(0x029D), 0x03);  /* 01A5D1 LDM */
    wr8(ABS(0x029E), 0x01);  /* 01A5D5 LDM */
    wr8(ABS(0x029F), 0x00);  /* 01A5D9 LDM */
    mflag = 0;  /* 01A5DD CLM */
    A = rd16(DP(0x05));  /* 01A5DE LDA */
    A &= 0x00FF;  /* 01A5E0 AND */
    mflag = 1;  /* 01A5E3 SEM */
    wr8(ABS(0x02A0), (uint8_t)A);  /* 01A5E4 STA */
    SETLO(A, rd8(DP(0x04)));  /* 01A5E7 LDA */
    wr8(ABS(0x02A1), (uint8_t)A);  /* 01A5E9 STA */
    mflag = 0;  /* 01A5EC CLM */
    A = rd16(DP(0x02));  /* 01A5ED LDA */
    C = 0;  /* 01A5EF CLC */
    A = adc16(A, 0x0080);  /* 01A5F0 ADC */
    wr16(DP(0x02), A);  /* 01A5F3 STA */
    Y = rd16(DP(0x08));  /* 01A5F5 LDY */
    S -= 3; F_0196AA();  /* 01A5F7 JSRL */
L_01A5FB: TICK();
    mflag = 0;  /* 01A5FB CLM */
    X = rd16(DP(0x06));  /* 01A5FC LDX */
    Z = (X == 0);
    mflag = 1;  /* 01A5FE SEM */
    if (!Z) goto L_01A604;  /* 01A5FF BNE */
    goto L_01A420;  /* 01A601 JMP */
L_01A604: TICK();
    mflag = 0;  /* 01A604 CLM */
    S -= 3; F_01986E();  /* 01A605 JSRL */
    Y = pull16();  /* 01A609 PLY */
    D = pull16();  /* 01A60A PLD */
    S += 3; return;  /* 01A60B RTL */
}

/* 01A60C  callers: 0035DD, 00DACF, 00DECB, 00E0B2, 00E494 */
void F_01A60C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A60C LDA */
    push16(A);  /* 01A60E PHA */
    S -= 3; F_00968D();  /* 01A60F JSRL */
    I = 0;  /* 01A613 CLI */
    Y = 0x0001;  /* 01A614 LDY */
    S -= 3; F_0080AA();  /* 01A617 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0400);  /* 01A61B SEB */
    S -= 3; F_00828C();  /* 01A620 JSRL */
L_01A624: TICK();
    mflag = 1;  /* 01A624 SEM */
    wr8(ABS(WDT), 0x00);  /* 01A625 LDM */
    mflag = 0;  /* 01A629 CLM */
    X = rd16(ABS(0x0214));  /* 01A62A LDX */
    if (X != 0) goto L_01A624;  /* 01A62D BNE */
    mflag = 1;  /* 01A62F SEM */
    SETLO(A, 0x01);  /* 01A630 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 01A632 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 01A633 STA */
    setP(pull16());  /* 01A635 PLP */
    mflag = 0;  /* 01A636 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x2000);  /* 01A637 CLB */
    S -= 3; F_007529();  /* 01A63C JSRL */
    mflag = 1;  /* 01A640 SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 01A641 CLB [P71 GUC_ACMA] */
    Y = 0x0019;  /* 01A645 LDY */
    mflag = 0;  /* 01A648 CLM */
    S -= 3; F_019884();  /* 01A649 JSRL */
L_01A64D: TICK();
    mflag = 1;  /* 01A64D SEM */
    wr8(ABS(WDT), 0x00);  /* 01A64E LDM */
    mflag = 0;  /* 01A652 CLM */
    ram_trampoline_087A();  /* 01A653 JSRL */
    mflag = 1;  /* 01A657 SEM */
    wr8(ABS(WDT), 0x00);  /* 01A658 LDM */
    mflag = 0;  /* 01A65C CLM */
    S -= 3; measurements_0198D5();  /* 01A65D JSRL */
    goto L_01A64D;  /* 01A661 JMP */
}

/* 01A664  callers: 016F8A, 01A77C */
void idle_check_01A664(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0246)) & 0x0002) == 0x0002) goto L_01A690;  /* 01A664 BBS */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_01A676;  /* 01A66A BBS */
    mflag = 1;  /* 01A670 SEM */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_01A67D;  /* 01A671 BBC */
L_01A676: TICK();
    mflag = 0;  /* 01A676 CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_01A682;  /* 01A677 BBS */
L_01A67D: TICK();
    mflag = 1;  /* 01A67D SEM */
    wr8(ABS(0x01E9), 0x00);  /* 01A67E LDM */
L_01A682: TICK();
    mflag = 1;  /* 01A682 SEM */
    SETLO(A, rd8(ABS(0x01E9)));  /* 01A683 LDA */
    if ((A & 0xFF) != 0) goto L_01A68D;  /* 01A686 BNE */
    mflag = 0;  /* 01A688 CLM */
    A = 0x0000;  /* 01A689 LDA */
    S += 3; return;  /* 01A68C RTL */
L_01A68D: TICK();
    goto L_01A6D0;  /* 01A68D JMP */
L_01A690: TICK();
    mflag = 0;  /* 01A690 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_01A69D;  /* 01A691 BBS */
    mflag = 1;  /* 01A697 SEM */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_01A6A4;  /* 01A698 BBC */
L_01A69D: TICK();
    mflag = 0;  /* 01A69D CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_01A6A9;  /* 01A69E BBS */
L_01A6A4: TICK();
    mflag = 0;  /* 01A6A4 CLM */
    A = 0x0000;  /* 01A6A5 LDA */
    S += 3; return;  /* 01A6A8 RTL */
L_01A6A9: TICK();
    A = rd16(ABS(0x0229));  /* 01A6A9 LDA */
    mflag = 1;  /* 01A6AC SEM */
    /* 01A6AD CMP */
    if ((A & 0xFF) != 0x01) goto L_01A6BD;  /* 01A6AF BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 01A6B1 LDA */
    /* 01A6B4 CMP */
    if ((A & 0xFF) == 0x03) goto L_01A6BD;  /* 01A6B6 BEQ */
    mflag = 0;  /* 01A6B8 CLM */
    A = 0x0000;  /* 01A6B9 LDA */
    S += 3; return;  /* 01A6BC RTL */
L_01A6BD: TICK();
    SETLO(A, rd8(ABS(0x0229)));  /* 01A6BD LDA */
    /* 01A6C0 CMP */
    if ((A & 0xFF) != 0x03) goto L_01A6D0;  /* 01A6C2 BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 01A6C4 LDA */
    /* 01A6C7 CMP */
    if ((A & 0xFF) == 0x08) goto L_01A6D0;  /* 01A6C9 BEQ */
    mflag = 0;  /* 01A6CB CLM */
    A = 0x0000;  /* 01A6CC LDA */
    S += 3; return;  /* 01A6CF RTL */
L_01A6D0: TICK();
    mflag = 0;  /* 01A6D0 CLM */
    A = 0x0001;  /* 01A6D1 LDA */
    S += 3; return;  /* 01A6D4 RTL */
}

/* 01A6D5  callers: 00FE6B */
void c_main(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A6D5 LDA */
    push16(D);  /* 01A6D7 PHD */
    D = A;  /* 01A6D8 TAD */
    push16(A);  /* 01A6D9 PHA */
    mflag = 1;  /* 01A6DA SEM */
    wr8(ABS(PMOD), 0x26);  /* 01A6DB LDM */
    wr8(ABS(P4), 0x00);  /* 01A6DF LDM */
    wr8(ABS(0x0007), 0x00);  /* 01A6E3 LDM */
    wr8(ABS(0x0006), 0x00);  /* 01A6E7 LDM */
    wr8(ABS(0x0003), 0x00);  /* 01A6EB LDM */
    wr8(ABS(0x0002), 0x00);  /* 01A6EF LDM */
    wr8(ABS(P5), 0x02);  /* 01A6F3 LDM */
    wr8(ABS(P6), 0x02);  /* 01A6F7 LDM */
    wr8(ABS(P7), 0x00);  /* 01A6FB LDM */
    wr8(ABS(P8), 0x0A);  /* 01A6FF LDM */
    wr8(ABS(P4D), 0x4C);  /* 01A703 LDM */
    wr8(ABS(P5D), 0x7F);  /* 01A707 LDM */
    wr8(ABS(P6D), 0x02);  /* 01A70B LDM */
    wr8(ABS(P7D), 0x07);  /* 01A70F LDM */
    wr8(ABS(P8D), 0xAA);  /* 01A713 LDM */
    wr8(ABS(WDTFS), 0x00);  /* 01A717 LDM */
    wr8(ABS(WDT), 0x00);  /* 01A71B LDM */
    mflag = 0;  /* 01A71F CLM */
    S -= 3; i2c_init_00713E();  /* 01A720 JSRL */
    wr16(DP(0x00), 0x0001);  /* 01A724 LDM */
    Y = 0x0001;  /* 01A728 LDY */
    A = D;  /* 01A72B TDA */
    A += 1;  /* 01A72C INC */
    A += 1;  /* 01A72D INC */
    push16(A);  /* 01A72E PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01A72F JSRL */
    Y = pull16();  /* 01A733 PLY */
    mflag = 0;  /* 01A734 CLM */
    S -= 3; sys_init_00E305();  /* 01A735 JSRL */
    S -= 3; F_01DE16();  /* 01A739 JSRL */
    Y = 0x0019;  /* 01A73D LDY */
    S -= 3; F_019884();  /* 01A740 JSRL */
    S -= 3; F_00B98D();  /* 01A744 JSRL */
    S -= 3; F_0167F2();  /* 01A748 JSRL */
    S -= 3; F_01A3EF();  /* 01A74C JSRL */
    S -= 3; F_018656();  /* 01A750 JSRL */
    S -= 3; F_01DAC4();  /* 01A754 JSRL */
    S -= 3; F_006696();  /* 01A758 JSRL */
    S -= 3; F_01E013();  /* 01A75C JSRL */
    mflag = 1;  /* 01A760 SEM */
    if ((rd8(ABS(0x0335)) & 0x10) == 0) goto L_01A76B;  /* 01A761 BBC */
    mflag = 0;  /* 01A766 CLM */
    S -= 3; F_01691C();  /* 01A767 JSRL */
L_01A76B: TICK();
    mflag = 0;  /* 01A76B CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0) goto L_01A776;  /* 01A76C BBC */
    S -= 3; F_0099AD();  /* 01A772 JSRL */
L_01A776: TICK();
    mflag = 1;  /* 01A776 SEM */
    wr8(ABS(WDT), 0x00);  /* 01A777 LDM */
    mflag = 0;  /* 01A77B CLM */
    S -= 3; idle_check_01A664();  /* 01A77C JSRL */
    X = A;  /* 01A780 TAX */
    if (X == 0) goto L_01A787;  /* 01A781 BEQ */
    ram_trampoline_087A();  /* 01A783 JSRL */
L_01A787: TICK();
    mflag = 1;  /* 01A787 SEM */
    wr8(ABS(WDT), 0x00);  /* 01A788 LDM */
    if ((rd8(ABS(0x0246)) & 0x02) == 0) goto L_01A7A5;  /* 01A78C BBC */
    mflag = 0;  /* 01A791 CLM */
    S -= 3; F_00C91C();  /* 01A792 JSRL */
    S -= 3; radio_state_machine_00F3E8();  /* 01A796 JSRL */
    S -= 3; F_016767();  /* 01A79A JSRL */
    S -= 3; measurements_0198D5();  /* 01A79E JSRL */
    goto L_01A7CD;  /* 01A7A2 JMP */
L_01A7A5: TICK();
    mflag = 1;  /* 01A7A5 SEM */
    SETLO(A, rd8(ABS(0x01E9)));  /* 01A7A6 LDA */
    if ((A & 0xFF) != 0) goto L_01A7CD;  /* 01A7A9 BNE */
    wr8(ABS(0x01E9), 0x01);  /* 01A7AB LDM */
    mflag = 0;  /* 01A7AF CLM */
    S -= 3; F_006817();  /* 01A7B0 JSRL */
    S -= 3; radio_state_machine_00F3E8();  /* 01A7B4 JSRL */
    S -= 3; F_01DB8B();  /* 01A7B8 JSRL */
    S -= 3; F_01914B();  /* 01A7BC JSRL */
    S -= 3; measurements_0198D5();  /* 01A7C0 JSRL */
    mflag = 0;  /* 01A7C4 CLM */
    S -= 3; F_01DE4E();  /* 01A7C5 JSRL */
    S -= 3; F_0134C0();  /* 01A7C9 JSRL */
L_01A7CD: TICK();
    goto L_01A776;  /* 01A7CD JMP */
}

/* 01A7D0  callers: 01AB89, 01ADC4 */
void F_01A7D0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A7D0 LDA */
    push16(D);  /* 01A7D2 PHD */
    D = A;  /* 01A7D3 TAD */
    wr16(DP(0x00), Y);  /* 01A7D4 STY */
    A = Y;  /* 01A7D6 TYA */
    A &= 0x00FF;  /* 01A7D7 AND */
    X = A;  /* 01A7DA TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A7DB LDA */
    mflag = 1;  /* 01A7DE SEM */
    /* 01A7DF CMP */
    if ((A & 0xFF) == 0x32) goto L_01A7F1;  /* 01A7E1 BEQ */
    SETLO(A, Y);  /* 01A7E3 TYA */
    mflag = 0;  /* 01A7E4 CLM */
    A &= 0x00FF;  /* 01A7E5 AND */
    X = A;  /* 01A7E8 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A7E9 LDA */
    mflag = 1;  /* 01A7EC SEM */
    /* 01A7ED CMP */
    if ((A & 0xFF) != 0x33) goto L_01A835;  /* 01A7EF BNE */
L_01A7F1: TICK();
    SETLO(A, Y);  /* 01A7F1 TYA */
    mflag = 0;  /* 01A7F2 CLM */
    A &= 0x00FF;  /* 01A7F3 AND */
    A += 1;  /* 01A7F6 INC */
    A += 1;  /* 01A7F7 INC */
    X = A;  /* 01A7F8 TAX */
    A = Y;  /* 01A7F9 TYA */
    A &= 0x00FF;  /* 01A7FA AND */
    A += 1;  /* 01A7FD INC */
    Y = A;  /* 01A7FE TAY */
    A = rd16((ABS(0x0283) + Y));  /* 01A7FF LDA */
    mflag = 1;  /* 01A802 SEM */
    mpy8(0x0A);  /* 01A803 MPY */
    C = 0;  /* 01A806 CLC */
    SETLO(A, adc8(A, rd8((ABS(0x0283) + X))));  /* 01A807 ADC */
    wr8(DP(0x01), (uint8_t)A);  /* 01A80A STA */
    SETLO(A, rd8(DP(0x00)));  /* 01A80C LDA */
    mflag = 0;  /* 01A80E CLM */
    A &= 0x00FF;  /* 01A80F AND */
    X = A;  /* 01A812 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A813 LDA */
    mflag = 1;  /* 01A816 SEM */
    mpy8(0x64);  /* 01A817 MPY */
    C = 0;  /* 01A81A CLC */
    SETLO(A, adc8(A, rd8(DP(0x01))));  /* 01A81B ADC */
    C = 0;  /* 01A81D CLC */
    SETLO(A, adc8(A, 0x68));  /* 01A81E ADC */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 01A820 STA */
    /* 01A823 CMP */
    if ((A & 0xFF) >= 0x80) goto L_01A82E;  /* 01A825 BCS */
    mflag = 0;  /* 01A827 CLM */
    A = 0x0001;  /* 01A828 LDA */
    goto L_01A832;  /* 01A82B JMP */
L_01A82E: TICK();
    mflag = 0;  /* 01A82E CLM */
    A = 0x0000;  /* 01A82F LDA */
L_01A832: TICK();
    mflag = 1;  /* 01A832 SEM */
    D = pull16();  /* 01A833 PLD */
    S += 3; return;  /* 01A834 RTL */
L_01A835: TICK();
    SETLO(A, 0x00);  /* 01A835 LDA */
    D = pull16();  /* 01A837 PLD */
    S += 3; return;  /* 01A838 RTL */
}

/* 01A839  callers: 01AB98, 01ABD7, 01ADD3, 01AE12 */
void F_01A839(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A839 LDA */
    push16(D);  /* 01A83B PHD */
    D = A;  /* 01A83C TAD */
    wr16(DP(0x00), Y);  /* 01A83D STY */
    A = Y;  /* 01A83F TYA */
    A &= 0x00FF;  /* 01A840 AND */
    A += 1;  /* 01A843 INC */
    A += 1;  /* 01A844 INC */
    A += 1;  /* 01A845 INC */
    X = A;  /* 01A846 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A847 LDA */
    A &= 0x00FF;  /* 01A84A AND */
    wr16(DP(0x01), A);  /* 01A84D STA */
    A = Y;  /* 01A84F TYA */
    A &= 0x00FF;  /* 01A850 AND */
    A += 1;  /* 01A853 INC */
    A += 1;  /* 01A854 INC */
    X = A;  /* 01A855 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A856 LDA */
    mflag = 1;  /* 01A859 SEM */
    mpy8(0x0A);  /* 01A85A MPY */
    wr8(DP(0x03), (uint8_t)A);  /* 01A85D STA */
    wr8(DP(0x04), (uint8_t)B);  /* 01A85F STA */
    mflag = 0;  /* 01A862 CLM */
    A = rd16(DP(0x01));  /* 01A863 LDA */
    C = 0;  /* 01A865 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 01A866 ADC */
    wr16(DP(0x01), A);  /* 01A868 STA */
    A = Y;  /* 01A86A TYA */
    A &= 0x00FF;  /* 01A86B AND */
    A += 1;  /* 01A86E INC */
    X = A;  /* 01A86F TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A870 LDA */
    mflag = 1;  /* 01A873 SEM */
    mpy8(0x64);  /* 01A874 MPY */
    wr8(DP(0x03), (uint8_t)A);  /* 01A877 STA */
    wr8(DP(0x04), (uint8_t)B);  /* 01A879 STA */
    mflag = 0;  /* 01A87C CLM */
    A = rd16(DP(0x01));  /* 01A87D LDA */
    C = 0;  /* 01A87F CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 01A880 ADC */
    wr16(DP(0x01), A);  /* 01A882 STA */
    A = Y;  /* 01A884 TYA */
    A &= 0x00FF;  /* 01A885 AND */
    X = A;  /* 01A888 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A889 LDA */
    A &= 0x00FF;  /* 01A88C AND */
    mpy16(0x03E8);  /* 01A88F MPY */
    C = 0;  /* 01A893 CLC */
    A = adc16(A, rd16(DP(0x01)));  /* 01A894 ADC */
    C = 0;  /* 01A896 CLC */
    A = adc16(A, 0x27E0);  /* 01A897 ADC */
    A = asl16(A);  /* 01A89A ASL */
    wr16(ABS(0x01D7), A);  /* 01A89B STA */
    /* 01A89E CMP */
    if (A < 0x0002) goto L_01A8AE;  /* 01A8A1 BCC */
    /* 01A8A3 CMP */
    if (A >= 0x1FA5) goto L_01A8AE;  /* 01A8A6 BCS */
    A = 0x0001;  /* 01A8A8 LDA */
    goto L_01A8B1;  /* 01A8AB JMP */
L_01A8AE: TICK();
    A = 0x0000;  /* 01A8AE LDA */
L_01A8B1: TICK();
    mflag = 1;  /* 01A8B1 SEM */
    D = pull16();  /* 01A8B2 PLD */
    S += 3; return;  /* 01A8B3 RTL */
}

/* 01A8B4  callers: 01ABBB, 01ABFA, 01ACA3, 01AD67 */
void F_01A8B4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A8B4 LDA */
    push16(D);  /* 01A8B6 PHD */
    D = A;  /* 01A8B7 TAD */
    wr16(DP(0x00), Y);  /* 01A8B8 STY */
    A = Y;  /* 01A8BA TYA */
    A &= 0x00FF;  /* 01A8BB AND */
    X = A;  /* 01A8BE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A8BF LDA */
    mflag = 1;  /* 01A8C2 SEM */
    /* 01A8C3 CMP */
    if ((A & 0xFF) >= 0x32) goto L_01A8CA;  /* 01A8C5 BCS */
    goto L_01A94A;  /* 01A8C7 JMP */
L_01A8CA: TICK();
    SETLO(A, Y);  /* 01A8CA TYA */
    mflag = 0;  /* 01A8CB CLM */
    A &= 0x00FF;  /* 01A8CC AND */
    X = A;  /* 01A8CF TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A8D0 LDA */
    mflag = 1;  /* 01A8D3 SEM */
    /* 01A8D4 CMP */
    if ((A & 0xFF) >= 0x39) goto L_01A94A;  /* 01A8D6 BCS */
    SETLO(A, Y);  /* 01A8D8 TYA */
    mflag = 0;  /* 01A8D9 CLM */
    A &= 0x00FF;  /* 01A8DA AND */
    A += 1;  /* 01A8DD INC */
    X = A;  /* 01A8DE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A8DF LDA */
    A &= 0x00FF;  /* 01A8E2 AND */
    wr16(DP(0x01), A);  /* 01A8E5 STA */
    A = Y;  /* 01A8E7 TYA */
    A &= 0x00FF;  /* 01A8E8 AND */
    X = A;  /* 01A8EB TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A8EC LDA */
    mflag = 1;  /* 01A8EF SEM */
    mpy8(0x0A);  /* 01A8F0 MPY */
    wr8(DP(0x03), (uint8_t)A);  /* 01A8F3 STA */
    wr8(DP(0x04), (uint8_t)B);  /* 01A8F5 STA */
    mflag = 0;  /* 01A8F8 CLM */
    A = rd16(DP(0x01));  /* 01A8F9 LDA */
    C = 0;  /* 01A8FB CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 01A8FC ADC */
    C = 0;  /* 01A8FE CLC */
    A = adc16(A, rd16(ABS(0x01D7)));  /* 01A8FF ADC */
    C = 0;  /* 01A902 CLC */
    A = adc16(A, 0xFDDC);  /* 01A903 ADC */
    wr16(ABS(0x01CD), A);  /* 01A906 STA */
    A = rd16(ABS(0x01B8));  /* 01A909 LDA */
    mflag = 1;  /* 01A90C SEM */
    t = rd8(ABS(0x0425));  /* 01A90D CMP */
    if ((A & 0xFF) != t) goto L_01A92F;  /* 01A910 BNE */
    mflag = 0;  /* 01A912 CLM */
    X = rd16(ABS(0x01D7));  /* 01A913 LDX */
    t = rd16(ABS(0x01C5));  /* 01A916 CPX */
    if (X != t) goto L_01A92F;  /* 01A919 BNE */
    X = rd16(ABS(0x01C9));  /* 01A91B LDX */
    t = rd16(ABS(0x01CD));  /* 01A91E CPX */
    if (X < t) goto L_01A929;  /* 01A921 BCC */
    A = 0x0001;  /* 01A923 LDA */
    goto L_01A92C;  /* 01A926 JMP */
L_01A929: TICK();
    A = 0x0000;  /* 01A929 LDA */
L_01A92C: TICK();
    mflag = 1;  /* 01A92C SEM */
    D = pull16();  /* 01A92D PLD */
    S += 3; return;  /* 01A92E RTL */
L_01A92F: TICK();
    mflag = 0;  /* 01A92F CLM */
    if ((rd16(ABS(0x039E)) & 0x0200) == 0x0200) goto L_01A944;  /* 01A930 BBS */
    X = rd16(ABS(0x01CD));  /* 01A936 LDX */
    /* 01A939 CPX */
    if (X >= 0x1B5A) goto L_01A944;  /* 01A93C BCS */
    A = 0x0001;  /* 01A93E LDA */
    goto L_01A947;  /* 01A941 JMP */
L_01A944: TICK();
    A = 0x0000;  /* 01A944 LDA */
L_01A947: TICK();
    mflag = 1;  /* 01A947 SEM */
    D = pull16();  /* 01A948 PLD */
    S += 3; return;  /* 01A949 RTL */
L_01A94A: TICK();
    SETLO(A, 0x00);  /* 01A94A LDA */
    D = pull16();  /* 01A94C PLD */
    S += 3; return;  /* 01A94D RTL */
}

/* 01A94E  callers: 01ABAD, 01ABEC, 01AC64, 01AD4B */
void F_01A94E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01A94E LDA */
    push16(D);  /* 01A950 PHD */
    D = A;  /* 01A951 TAD */
    wr16(DP(0x00), Y);  /* 01A952 STY */
    A = Y;  /* 01A954 TYA */
    A &= 0x00FF;  /* 01A955 AND */
    X = A;  /* 01A958 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A959 LDA */
    mflag = 1;  /* 01A95C SEM */
    /* 01A95D CMP */
    if ((A & 0xFF) >= 0x32) goto L_01A964;  /* 01A95F BCS */
    goto L_01AA02;  /* 01A961 JMP */
L_01A964: TICK();
    SETLO(A, Y);  /* 01A964 TYA */
    mflag = 0;  /* 01A965 CLM */
    A &= 0x00FF;  /* 01A966 AND */
    X = A;  /* 01A969 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A96A LDA */
    mflag = 1;  /* 01A96D SEM */
    /* 01A96E CMP */
    if ((A & 0xFF) < 0x39) goto L_01A975;  /* 01A970 BCC */
    goto L_01AA02;  /* 01A972 JMP */
L_01A975: TICK();
    SETLO(A, Y);  /* 01A975 TYA */
    mflag = 0;  /* 01A976 CLM */
    A &= 0x00FF;  /* 01A977 AND */
    A += 1;  /* 01A97A INC */
    A += 1;  /* 01A97B INC */
    X = A;  /* 01A97C TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A97D LDA */
    A &= 0x00FF;  /* 01A980 AND */
    wr16(DP(0x01), A);  /* 01A983 STA */
    A = Y;  /* 01A985 TYA */
    A &= 0x00FF;  /* 01A986 AND */
    A += 1;  /* 01A989 INC */
    X = A;  /* 01A98A TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A98B LDA */
    mflag = 1;  /* 01A98E SEM */
    mpy8(0x0A);  /* 01A98F MPY */
    wr8(DP(0x03), (uint8_t)A);  /* 01A992 STA */
    wr8(DP(0x04), (uint8_t)B);  /* 01A994 STA */
    mflag = 0;  /* 01A997 CLM */
    A = rd16(DP(0x01));  /* 01A998 LDA */
    C = 0;  /* 01A99A CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 01A99B ADC */
    wr16(DP(0x01), A);  /* 01A99D STA */
    A = Y;  /* 01A99F TYA */
    A &= 0x00FF;  /* 01A9A0 AND */
    X = A;  /* 01A9A3 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01A9A4 LDA */
    mflag = 1;  /* 01A9A7 SEM */
    mpy8(0x64);  /* 01A9A8 MPY */
    wr8(DP(0x03), (uint8_t)A);  /* 01A9AB STA */
    wr8(DP(0x04), (uint8_t)B);  /* 01A9AD STA */
    mflag = 0;  /* 01A9B0 CLM */
    A = rd16(DP(0x01));  /* 01A9B1 LDA */
    C = 0;  /* 01A9B3 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 01A9B4 ADC */
    C = 0;  /* 01A9B6 CLC */
    A = adc16(A, rd16(ABS(0x01D7)));  /* 01A9B7 ADC */
    C = 0;  /* 01A9BA CLC */
    A = adc16(A, 0xEA68);  /* 01A9BB ADC */
    wr16(ABS(0x01CD), A);  /* 01A9BE STA */
    A = rd16(ABS(0x01B8));  /* 01A9C1 LDA */
    mflag = 1;  /* 01A9C4 SEM */
    t = rd8(ABS(0x0425));  /* 01A9C5 CMP */
    if ((A & 0xFF) != t) goto L_01A9E7;  /* 01A9C8 BNE */
    mflag = 0;  /* 01A9CA CLM */
    X = rd16(ABS(0x01D7));  /* 01A9CB LDX */
    t = rd16(ABS(0x01C5));  /* 01A9CE CPX */
    if (X != t) goto L_01A9E7;  /* 01A9D1 BNE */
    X = rd16(ABS(0x01C9));  /* 01A9D3 LDX */
    t = rd16(ABS(0x01CD));  /* 01A9D6 CPX */
    if (X < t) goto L_01A9E1;  /* 01A9D9 BCC */
    A = 0x0001;  /* 01A9DB LDA */
    goto L_01A9E4;  /* 01A9DE JMP */
L_01A9E1: TICK();
    A = 0x0000;  /* 01A9E1 LDA */
L_01A9E4: TICK();
    mflag = 1;  /* 01A9E4 SEM */
    D = pull16();  /* 01A9E5 PLD */
    S += 3; return;  /* 01A9E6 RTL */
L_01A9E7: TICK();
    mflag = 0;  /* 01A9E7 CLM */
    if ((rd16(ABS(0x039E)) & 0x0200) == 0x0200) goto L_01A9FC;  /* 01A9E8 BBS */
    X = rd16(ABS(0x01CD));  /* 01A9EE LDX */
    /* 01A9F1 CPX */
    if (X >= 0x1B5A) goto L_01A9FC;  /* 01A9F4 BCS */
    A = 0x0001;  /* 01A9F6 LDA */
    goto L_01A9FF;  /* 01A9F9 JMP */
L_01A9FC: TICK();
    A = 0x0000;  /* 01A9FC LDA */
L_01A9FF: TICK();
    mflag = 1;  /* 01A9FF SEM */
    D = pull16();  /* 01AA00 PLD */
    S += 3; return;  /* 01AA01 RTL */
L_01AA02: TICK();
    SETLO(A, 0x00);  /* 01AA02 LDA */
    D = pull16();  /* 01AA04 PLD */
    S += 3; return;  /* 01AA05 RTL */
}

/* 01AA06  callers: 01ADF6, 01AE35, 01AEE8, 01AF20 */
void F_01AA06(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01AA06 LDA */
    push16(D);  /* 01AA08 PHD */
    D = A;  /* 01AA09 TAD */
    wr16(DP(0x00), Y);  /* 01AA0A STY */
    A = Y;  /* 01AA0C TYA */
    A &= 0x00FF;  /* 01AA0D AND */
    X = A;  /* 01AA10 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AA11 LDA */
    mflag = 1;  /* 01AA14 SEM */
    /* 01AA15 CMP */
    if ((A & 0xFF) != 0x39) goto L_01AA77;  /* 01AA17 BNE */
    SETLO(A, Y);  /* 01AA19 TYA */
    mflag = 0;  /* 01AA1A CLM */
    A &= 0x00FF;  /* 01AA1B AND */
    A += 1;  /* 01AA1E INC */
    X = A;  /* 01AA1F TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AA20 LDA */
    A &= 0x00FF;  /* 01AA23 AND */
    C = 0;  /* 01AA26 CLC */
    A = adc16(A, rd16(ABS(0x01D7)));  /* 01AA27 ADC */
    C = 0;  /* 01AA2A CLC */
    A = adc16(A, 0xFFD0);  /* 01AA2B ADC */
    wr16(ABS(0x01CD), A);  /* 01AA2E STA */
    A = rd16(ABS(0x01B8));  /* 01AA31 LDA */
    mflag = 1;  /* 01AA34 SEM */
    t = rd8(ABS(0x0425));  /* 01AA35 CMP */
    if ((A & 0xFF) != t) goto L_01AA57;  /* 01AA38 BNE */
    mflag = 0;  /* 01AA3A CLM */
    X = rd16(ABS(0x01D7));  /* 01AA3B LDX */
    t = rd16(ABS(0x01C7));  /* 01AA3E CPX */
    if (X != t) goto L_01AA57;  /* 01AA41 BNE */
    X = rd16(ABS(0x01CB));  /* 01AA43 LDX */
    t = rd16(ABS(0x01CD));  /* 01AA46 CPX */
    if (X < t) goto L_01AA51;  /* 01AA49 BCC */
    A = 0x0001;  /* 01AA4B LDA */
    goto L_01AA54;  /* 01AA4E JMP */
L_01AA51: TICK();
    A = 0x0000;  /* 01AA51 LDA */
L_01AA54: TICK();
    mflag = 1;  /* 01AA54 SEM */
    D = pull16();  /* 01AA55 PLD */
    S += 3; return;  /* 01AA56 RTL */
L_01AA57: TICK();
    mflag = 0;  /* 01AA57 CLM */
    if ((rd16(ABS(0x039E)) & 0x0100) == 0x0100) goto L_01AA77;  /* 01AA58 BBS */
    X = rd16(ABS(0x01CD));  /* 01AA5E LDX */
    /* 01AA61 CPX */
    if (X >= 0x1FA7) goto L_01AA71;  /* 01AA64 BCS */
    /* 01AA66 CPX */
    if (X < 0x1B5A) goto L_01AA71;  /* 01AA69 BCC */
    A = 0x0001;  /* 01AA6B LDA */
    goto L_01AA74;  /* 01AA6E JMP */
L_01AA71: TICK();
    A = 0x0000;  /* 01AA71 LDA */
L_01AA74: TICK();
    mflag = 1;  /* 01AA74 SEM */
    D = pull16();  /* 01AA75 PLD */
    S += 3; return;  /* 01AA76 RTL */
L_01AA77: TICK();
    mflag = 0;  /* 01AA77 CLM */
    A = 0x0000;  /* 01AA78 LDA */
    mflag = 1;  /* 01AA7B SEM */
    D = pull16();  /* 01AA7C PLD */
    S += 3; return;  /* 01AA7D RTL */
}

/* 01AA7E  callers: 01ADE8, 01AE27, 01AEA4, 01AF04 */
void F_01AA7E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01AA7E LDA */
    push16(D);  /* 01AA80 PHD */
    D = A;  /* 01AA81 TAD */
    wr16(DP(0x00), Y);  /* 01AA82 STY */
    A = Y;  /* 01AA84 TYA */
    A &= 0x00FF;  /* 01AA85 AND */
    X = A;  /* 01AA88 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AA89 LDA */
    mflag = 1;  /* 01AA8C SEM */
    /* 01AA8D CMP */
    if ((A & 0xFF) != 0x39) goto L_01AB0A;  /* 01AA8F BNE */
    SETLO(A, Y);  /* 01AA91 TYA */
    mflag = 0;  /* 01AA92 CLM */
    A &= 0x00FF;  /* 01AA93 AND */
    A += 1;  /* 01AA96 INC */
    A += 1;  /* 01AA97 INC */
    X = A;  /* 01AA98 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AA99 LDA */
    A &= 0x00FF;  /* 01AA9C AND */
    wr16(DP(0x01), A);  /* 01AA9F STA */
    A = Y;  /* 01AAA1 TYA */
    A &= 0x00FF;  /* 01AAA2 AND */
    A += 1;  /* 01AAA5 INC */
    X = A;  /* 01AAA6 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AAA7 LDA */
    mflag = 1;  /* 01AAAA SEM */
    mpy8(0x0A);  /* 01AAAB MPY */
    wr8(DP(0x03), (uint8_t)A);  /* 01AAAE STA */
    wr8(DP(0x04), (uint8_t)B);  /* 01AAB0 STA */
    mflag = 0;  /* 01AAB3 CLM */
    A = rd16(DP(0x01));  /* 01AAB4 LDA */
    C = 0;  /* 01AAB6 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 01AAB7 ADC */
    C = 0;  /* 01AAB9 CLC */
    A = adc16(A, rd16(ABS(0x01D7)));  /* 01AABA ADC */
    C = 0;  /* 01AABD CLC */
    A = adc16(A, 0xFDF0);  /* 01AABE ADC */
    wr16(ABS(0x01CD), A);  /* 01AAC1 STA */
    A = rd16(ABS(0x01B8));  /* 01AAC4 LDA */
    mflag = 1;  /* 01AAC7 SEM */
    t = rd8(ABS(0x0425));  /* 01AAC8 CMP */
    if ((A & 0xFF) != t) goto L_01AAEA;  /* 01AACB BNE */
    mflag = 0;  /* 01AACD CLM */
    X = rd16(ABS(0x01D7));  /* 01AACE LDX */
    t = rd16(ABS(0x01C7));  /* 01AAD1 CPX */
    if (X != t) goto L_01AAEA;  /* 01AAD4 BNE */
    X = rd16(ABS(0x01CB));  /* 01AAD6 LDX */
    t = rd16(ABS(0x01CD));  /* 01AAD9 CPX */
    if (X < t) goto L_01AAE4;  /* 01AADC BCC */
    A = 0x0001;  /* 01AADE LDA */
    goto L_01AAE7;  /* 01AAE1 JMP */
L_01AAE4: TICK();
    A = 0x0000;  /* 01AAE4 LDA */
L_01AAE7: TICK();
    mflag = 1;  /* 01AAE7 SEM */
    D = pull16();  /* 01AAE8 PLD */
    S += 3; return;  /* 01AAE9 RTL */
L_01AAEA: TICK();
    mflag = 0;  /* 01AAEA CLM */
    if ((rd16(ABS(0x039E)) & 0x0100) == 0x0100) goto L_01AB0A;  /* 01AAEB BBS */
    X = rd16(ABS(0x01CD));  /* 01AAF1 LDX */
    /* 01AAF4 CPX */
    if (X >= 0x1FA7) goto L_01AB04;  /* 01AAF7 BCS */
    /* 01AAF9 CPX */
    if (X < 0x1B5A) goto L_01AB04;  /* 01AAFC BCC */
    A = 0x0001;  /* 01AAFE LDA */
    goto L_01AB07;  /* 01AB01 JMP */
L_01AB04: TICK();
    A = 0x0000;  /* 01AB04 LDA */
L_01AB07: TICK();
    mflag = 1;  /* 01AB07 SEM */
    D = pull16();  /* 01AB08 PLD */
    S += 3; return;  /* 01AB09 RTL */
L_01AB0A: TICK();
    mflag = 0;  /* 01AB0A CLM */
    A = 0x0000;  /* 01AB0B LDA */
    mflag = 1;  /* 01AB0E SEM */
    D = pull16();  /* 01AB0F PLD */
    S += 3; return;  /* 01AB10 RTL */
}

/* 01AB11  callers: 01AB4E, 01AD89, 01AF42 */
void F_01AB11(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01AB11 LDA */
    push16(D);  /* 01AB13 PHD */
    D = A;  /* 01AB14 TAD */
    wr16(DP(0x00), Y);  /* 01AB15 STY */
L_01AB17: TICK();
    A = rd16(DP(0x00));  /* 01AB17 LDA */
    mflag = 1;  /* 01AB19 SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 01AB1A INC */
    mflag = 0;  /* 01AB1C CLM */
    A &= 0x00FF;  /* 01AB1D AND */
    X = A;  /* 01AB20 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AB21 LDA */
    mflag = 1;  /* 01AB24 SEM */
    /* 01AB25 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01AB2D;  /* 01AB27 BNE */
    SETLO(A, 0x01);  /* 01AB29 LDA */
    D = pull16();  /* 01AB2B PLD */
    S += 3; return;  /* 01AB2C RTL */
L_01AB2D: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 01AB2D LDA */
    t = rd8(ABS(0x0293));  /* 01AB2F CMP */
    C = ((A & 0xFF) >= t);
    mflag = 0;  /* 01AB32 CLM */
    if (!C) goto L_01AB17;  /* 01AB33 BCC */
    A = 0x0000;  /* 01AB35 LDA */
    mflag = 1;  /* 01AB38 SEM */
    D = pull16();  /* 01AB39 PLD */
    S += 3; return;  /* 01AB3A RTL */
}

/* 01AB3B  callers: 01B5B3, 01B6A5, 01B70E, 01B7AF, 01B807, 01B86B */
void F_01AB3B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01AB3B LDA */
    push16(D);  /* 01AB3D PHD */
    D = A;  /* 01AB3E TAD */
    C = 0;  /* 01AB3F CLC */
    A = adc16(A, 0x0004);  /* 01AB40 ADC */
    push16(A);  /* 01AB43 PHA */
    wr16(DP(0x00), Y);  /* 01AB44 STY */
    mflag = 1;  /* 01AB46 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01AB47 STA */
    mflag = 0;  /* 01AB4A CLM */
    A = rd16(LIND(0x00));  /* 01AB4B LDA */
    Y = A;  /* 01AB4D TAY */
    S -= 3; F_01AB11();  /* 01AB4E JSRL */
    /* 01AB52 CMP */
    if ((A & 0xFF) == 0x00) goto L_01AB5B;  /* 01AB54 BEQ */
    SETLO(A, 0x00);  /* 01AB56 LDA */
    Y = pull16();  /* 01AB58 PLY */
    D = pull16();  /* 01AB59 PLD */
    S += 3; return;  /* 01AB5A RTL */
L_01AB5B: TICK();
    C = 1;  /* 01AB5B SEC */
    SETLO(A, sbc8(A, rd8(LIND(0x00))));  /* 01AB5C SBC */
    C = 0;  /* 01AB5E CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0293))));  /* 01AB5F ADC */
    wr8(DP(0x03), (uint8_t)A);  /* 01AB62 STA */
    /* 01AB64 CMP */
    if ((A & 0xFF) < 0x01) goto L_01AB6C;  /* 01AB66 BCC */
    /* 01AB68 CMP */
    if ((A & 0xFF) < 0x0B) goto L_01AB71;  /* 01AB6A BCC */
L_01AB6C: TICK();
    SETLO(A, 0x00);  /* 01AB6C LDA */
    Y = pull16();  /* 01AB6E PLY */
    D = pull16();  /* 01AB6F PLD */
    S += 3; return;  /* 01AB70 RTL */
L_01AB71: TICK();
    mflag = 0;  /* 01AB71 CLM */
    X = rd16(ABS(0x01C5));  /* 01AB72 LDX */
    wr16(ABS(0x01D7), X);  /* 01AB75 STX */
    A = rd16(DP(0x03));  /* 01AB78 LDA */
    mflag = 1;  /* 01AB7A SEM */
    /* 01AB7B CMP */
    if ((A & 0xFF) == 0x0A) goto L_01AB85;  /* 01AB7D BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01AB7F LDA */
    /* 01AB81 CMP */
    if ((A & 0xFF) != 0x09) goto L_01ABC5;  /* 01AB83 BNE */
L_01AB85: TICK();
    mflag = 0;  /* 01AB85 CLM */
    A = rd16(LIND(0x00));  /* 01AB86 LDA */
    Y = A;  /* 01AB88 TAY */
    S -= 3; F_01A7D0();  /* 01AB89 JSRL */
    /* 01AB8D CMP */
    if ((A & 0xFF) == 0x00) goto L_01ABC2;  /* 01AB8F BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01AB91 LDA */
    SETLO(A, A + 1);  /* 01AB93 INC */
    SETLO(A, A + 1);  /* 01AB94 INC */
    SETLO(A, A + 1);  /* 01AB95 INC */
    mflag = 0;  /* 01AB96 CLM */
    Y = A;  /* 01AB97 TAY */
    S -= 3; F_01A839();  /* 01AB98 JSRL */
    /* 01AB9C CMP */
    if ((A & 0xFF) == 0x00) goto L_01ABC2;  /* 01AB9E BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01ABA0 LDA */
    /* 01ABA2 CMP */
    if ((A & 0xFF) != 0x0A) goto L_01ABB4;  /* 01ABA4 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01ABA6 LDA */
    C = 0;  /* 01ABA8 CLC */
    SETLO(A, adc8(A, 0x07));  /* 01ABA9 ADC */
    mflag = 0;  /* 01ABAB CLM */
    Y = A;  /* 01ABAC TAY */
    S -= 3; F_01A94E();  /* 01ABAD JSRL */
    goto L_01ABBF;  /* 01ABB1 JMP */
L_01ABB4: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01ABB4 LDA */
    C = 0;  /* 01ABB6 CLC */
    SETLO(A, adc8(A, 0x07));  /* 01ABB7 ADC */
    mflag = 0;  /* 01ABB9 CLM */
    Y = A;  /* 01ABBA TAY */
    S -= 3; F_01A8B4();  /* 01ABBB JSRL */
L_01ABBF: TICK();
    Y = pull16();  /* 01ABBF PLY */
    D = pull16();  /* 01ABC0 PLD */
    S += 3; return;  /* 01ABC1 RTL */
L_01ABC2: TICK();
    goto L_01AD6E;  /* 01ABC2 JMP */
L_01ABC5: TICK();
    mflag = 0;  /* 01ABC5 CLM */
    A = rd16(DP(0x03));  /* 01ABC6 LDA */
    mflag = 1;  /* 01ABC8 SEM */
    /* 01ABC9 CMP */
    if ((A & 0xFF) == 0x07) goto L_01ABD3;  /* 01ABCB BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01ABCD LDA */
    /* 01ABCF CMP */
    if ((A & 0xFF) != 0x06) goto L_01AC04;  /* 01ABD1 BNE */
L_01ABD3: TICK();
    mflag = 0;  /* 01ABD3 CLM */
    A = rd16(LIND(0x00));  /* 01ABD4 LDA */
    Y = A;  /* 01ABD6 TAY */
    S -= 3; F_01A839();  /* 01ABD7 JSRL */
    /* 01ABDB CMP */
    if ((A & 0xFF) == 0x00) goto L_01AC01;  /* 01ABDD BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01ABDF LDA */
    /* 01ABE1 CMP */
    if ((A & 0xFF) != 0x07) goto L_01ABF3;  /* 01ABE3 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01ABE5 LDA */
    C = 0;  /* 01ABE7 CLC */
    SETLO(A, adc8(A, 0x04));  /* 01ABE8 ADC */
    mflag = 0;  /* 01ABEA CLM */
    Y = A;  /* 01ABEB TAY */
    S -= 3; F_01A94E();  /* 01ABEC JSRL */
    goto L_01ABFE;  /* 01ABF0 JMP */
L_01ABF3: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01ABF3 LDA */
    C = 0;  /* 01ABF5 CLC */
    SETLO(A, adc8(A, 0x04));  /* 01ABF6 ADC */
    mflag = 0;  /* 01ABF8 CLM */
    Y = A;  /* 01ABF9 TAY */
    S -= 3; F_01A8B4();  /* 01ABFA JSRL */
L_01ABFE: TICK();
    Y = pull16();  /* 01ABFE PLY */
    D = pull16();  /* 01ABFF PLD */
    S += 3; return;  /* 01AC00 RTL */
L_01AC01: TICK();
    goto L_01AD6E;  /* 01AC01 JMP */
L_01AC04: TICK();
    mflag = 0;  /* 01AC04 CLM */
    A = rd16(DP(0x03));  /* 01AC05 LDA */
    mflag = 1;  /* 01AC07 SEM */
    /* 01AC08 CMP */
    if ((A & 0xFF) == 0x05) goto L_01AC0F;  /* 01AC0A BEQ */
    goto L_01ACAD;  /* 01AC0C JMP */
L_01AC0F: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AC0F LDA */
    mflag = 0;  /* 01AC11 CLM */
    A &= 0x00FF;  /* 01AC12 AND */
    X = A;  /* 01AC15 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AC16 LDA */
    mflag = 1;  /* 01AC19 SEM */
    /* 01AC1A CMP */
    if ((A & 0xFF) == 0x32) goto L_01AC21;  /* 01AC1C BEQ */
    goto L_01ACAA;  /* 01AC1E JMP */
L_01AC21: TICK();
    SETLO(A, rd8(ABS(0x01C2)));  /* 01AC21 LDA */
    mflag = 0;  /* 01AC24 CLM */
    A &= 0x00FF;  /* 01AC25 AND */
    wr16(DP(0x04), A);  /* 01AC28 STA */
    A = rd16(LIND(0x00));  /* 01AC2A LDA */
    A &= 0x00FF;  /* 01AC2C AND */
    A += 1;  /* 01AC2F INC */
    X = A;  /* 01AC30 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AC31 LDA */
    A &= 0x00FF;  /* 01AC34 AND */
    C = 0;  /* 01AC37 CLC */
    A = adc16(A, 0xFFD0);  /* 01AC38 ADC */
    C = 1;  /* 01AC3B SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 01AC3C SBC */
    if (!V) goto L_01AC43;  /* 01AC3E BVC */
    A ^= 0x8000;  /* 01AC40 EOR */
    N = ((A & 0x8000) != 0);
L_01AC43: TICK();
    if (!N) goto L_01AC6B;  /* 01AC43 BPL */
    A = rd16(LIND(0x00));  /* 01AC45 LDA */
    A &= 0x00FF;  /* 01AC47 AND */
    A += 1;  /* 01AC4A INC */
    X = A;  /* 01AC4B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AC4C LDA */
    A &= 0x00FF;  /* 01AC4F AND */
    mpy16(0x02BC);  /* 01AC52 MPY */
    C = 0;  /* 01AC56 CLC */
    A = adc16(A, 0x7CC2);  /* 01AC57 ADC */
    wr16(ABS(0x01D7), A);  /* 01AC5A STA */
    A = rd16(LIND(0x00));  /* 01AC5D LDA */
    mflag = 1;  /* 01AC5F SEM */
    SETLO(A, A + 1);  /* 01AC60 INC */
    SETLO(A, A + 1);  /* 01AC61 INC */
    mflag = 0;  /* 01AC62 CLM */
    Y = A;  /* 01AC63 TAY */
    S -= 3; F_01A94E();  /* 01AC64 JSRL */
    Y = pull16();  /* 01AC68 PLY */
    D = pull16();  /* 01AC69 PLD */
    S += 3; return;  /* 01AC6A RTL */
L_01AC6B: TICK();
    A = rd16(LIND(0x00));  /* 01AC6B LDA */
    A &= 0x00FF;  /* 01AC6D AND */
    A += 1;  /* 01AC70 INC */
    A += 1;  /* 01AC71 INC */
    X = A;  /* 01AC72 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AC73 LDA */
    mflag = 1;  /* 01AC76 SEM */
    mpy8(0x46);  /* 01AC77 MPY */
    wr8(DP(0x04), (uint8_t)A);  /* 01AC7A STA */
    wr8(DP(0x05), (uint8_t)B);  /* 01AC7C STA */
    SETLO(A, rd8(LIND(0x00)));  /* 01AC7F LDA */
    mflag = 0;  /* 01AC81 CLM */
    A &= 0x00FF;  /* 01AC82 AND */
    A += 1;  /* 01AC85 INC */
    X = A;  /* 01AC86 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AC87 LDA */
    A &= 0x00FF;  /* 01AC8A AND */
    mpy16(0x02BC);  /* 01AC8D MPY */
    C = 0;  /* 01AC91 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01AC92 ADC */
    C = 0;  /* 01AC94 CLC */
    A = adc16(A, 0x6FA2);  /* 01AC95 ADC */
    wr16(ABS(0x01D7), A);  /* 01AC98 STA */
    A = rd16(LIND(0x00));  /* 01AC9B LDA */
    mflag = 1;  /* 01AC9D SEM */
    SETLO(A, A + 1);  /* 01AC9E INC */
    SETLO(A, A + 1);  /* 01AC9F INC */
    SETLO(A, A + 1);  /* 01ACA0 INC */
    mflag = 0;  /* 01ACA1 CLM */
    Y = A;  /* 01ACA2 TAY */
    S -= 3; F_01A8B4();  /* 01ACA3 JSRL */
    Y = pull16();  /* 01ACA7 PLY */
    D = pull16();  /* 01ACA8 PLD */
    S += 3; return;  /* 01ACA9 RTL */
L_01ACAA: TICK();
    goto L_01AD6E;  /* 01ACAA JMP */
L_01ACAD: TICK();
    mflag = 0;  /* 01ACAD CLM */
    A = rd16(DP(0x03));  /* 01ACAE LDA */
    mflag = 1;  /* 01ACB0 SEM */
    /* 01ACB1 CMP */
    if ((A & 0xFF) == 0x03) goto L_01ACB8;  /* 01ACB3 BEQ */
    goto L_01AD55;  /* 01ACB5 JMP */
L_01ACB8: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01ACB8 LDA */
    mflag = 0;  /* 01ACBA CLM */
    A &= 0x00FF;  /* 01ACBB AND */
    X = A;  /* 01ACBE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01ACBF LDA */
    mflag = 1;  /* 01ACC2 SEM */
    /* 01ACC3 CMP */
    if ((A & 0xFF) != 0x39) goto L_01ACF5;  /* 01ACC5 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01ACC7 LDA */
    mflag = 0;  /* 01ACC9 CLM */
    A &= 0x00FF;  /* 01ACCA AND */
    A += 1;  /* 01ACCD INC */
    X = A;  /* 01ACCE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01ACCF LDA */
    mflag = 1;  /* 01ACD2 SEM */
    /* 01ACD3 CMP */
    if ((A & 0xFF) != 0x39) goto L_01ACF5;  /* 01ACD5 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01ACD7 LDA */
    mflag = 0;  /* 01ACD9 CLM */
    A &= 0x00FF;  /* 01ACDA AND */
    A += 1;  /* 01ACDD INC */
    A += 1;  /* 01ACDE INC */
    X = A;  /* 01ACDF TAX */
    A = rd16((ABS(0x0283) + X));  /* 01ACE0 LDA */
    mflag = 1;  /* 01ACE3 SEM */
    /* 01ACE4 CMP */
    if ((A & 0xFF) != 0x39) goto L_01ACF5;  /* 01ACE6 BNE */
    mflag = 0;  /* 01ACE8 CLM */
    wr16(ABS(0x01CD), 0x1FF4);  /* 01ACE9 LDM */
    A = 0x0001;  /* 01ACEE LDA */
    mflag = 1;  /* 01ACF1 SEM */
    Y = pull16();  /* 01ACF2 PLY */
    D = pull16();  /* 01ACF3 PLD */
    S += 3; return;  /* 01ACF4 RTL */
L_01ACF5: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01ACF5 LDA */
    mflag = 0;  /* 01ACF7 CLM */
    A &= 0x00FF;  /* 01ACF8 AND */
    X = A;  /* 01ACFB TAX */
    A = rd16((ABS(0x0283) + X));  /* 01ACFC LDA */
    mflag = 1;  /* 01ACFF SEM */
    /* 01AD00 CMP */
    if ((A & 0xFF) != 0x31) goto L_01AD41;  /* 01AD02 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01AD04 LDA */
    mflag = 0;  /* 01AD06 CLM */
    A &= 0x00FF;  /* 01AD07 AND */
    A += 1;  /* 01AD0A INC */
    A += 1;  /* 01AD0B INC */
    X = A;  /* 01AD0C TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AD0D LDA */
    mflag = 1;  /* 01AD10 SEM */
    /* 01AD11 CMP */
    if ((A & 0xFF) != 0x31) goto L_01AD41;  /* 01AD13 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01AD15 LDA */
    mflag = 0;  /* 01AD17 CLM */
    A &= 0x00FF;  /* 01AD18 AND */
    A += 1;  /* 01AD1B INC */
    X = A;  /* 01AD1C TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AD1D LDA */
    mflag = 1;  /* 01AD20 SEM */
    /* 01AD21 CMP */
    if ((A & 0xFF) == 0x2A) goto L_01AD41;  /* 01AD23 BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01AD25 LDA */
    mflag = 0;  /* 01AD27 CLM */
    A &= 0x00FF;  /* 01AD28 AND */
    A += 1;  /* 01AD2B INC */
    X = A;  /* 01AD2C TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AD2D LDA */
    A &= 0x00FF;  /* 01AD30 AND */
    C = 0;  /* 01AD33 CLC */
    A = adc16(A, 0x1FBA);  /* 01AD34 ADC */
    wr16(ABS(0x01CD), A);  /* 01AD37 STA */
    A = 0x0001;  /* 01AD3A LDA */
    mflag = 1;  /* 01AD3D SEM */
    Y = pull16();  /* 01AD3E PLY */
    D = pull16();  /* 01AD3F PLD */
    S += 3; return;  /* 01AD40 RTL */
L_01AD41: TICK();
    mflag = 0;  /* 01AD41 CLM */
    if ((rd16(ABS(0x039E)) & 0x0040) == 0) goto L_01AD52;  /* 01AD42 BBC */
    A = rd16(LIND(0x00));  /* 01AD48 LDA */
    Y = A;  /* 01AD4A TAY */
    S -= 3; F_01A94E();  /* 01AD4B JSRL */
    Y = pull16();  /* 01AD4F PLY */
    D = pull16();  /* 01AD50 PLD */
    S += 3; return;  /* 01AD51 RTL */
L_01AD52: TICK();
    goto L_01AD6E;  /* 01AD52 JMP */
L_01AD55: TICK();
    mflag = 0;  /* 01AD55 CLM */
    A = rd16(DP(0x03));  /* 01AD56 LDA */
    mflag = 1;  /* 01AD58 SEM */
    /* 01AD59 CMP */
    if ((A & 0xFF) != 0x02) goto L_01AD6E;  /* 01AD5B BNE */
    mflag = 0;  /* 01AD5D CLM */
    if ((rd16(ABS(0x039E)) & 0x0040) == 0x0040) goto L_01AD6E;  /* 01AD5E BBS */
    A = rd16(LIND(0x00));  /* 01AD64 LDA */
    Y = A;  /* 01AD66 TAY */
    S -= 3; F_01A8B4();  /* 01AD67 JSRL */
    Y = pull16();  /* 01AD6B PLY */
    D = pull16();  /* 01AD6C PLD */
    S += 3; return;  /* 01AD6D RTL */
L_01AD6E: TICK();
    mflag = 0;  /* 01AD6E CLM */
    A = 0x0000;  /* 01AD6F LDA */
    mflag = 1;  /* 01AD72 SEM */
    Y = pull16();  /* 01AD73 PLY */
    D = pull16();  /* 01AD74 PLD */
    S += 3; return;  /* 01AD75 RTL */
}

/* 01AD76  callers: 01B513, 01B5C1, 01B6B3, 01B71D, 01B7BE, 01B816, 01B87A */
void F_01AD76(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01AD76 LDA */
    push16(D);  /* 01AD78 PHD */
    D = A;  /* 01AD79 TAD */
    C = 0;  /* 01AD7A CLC */
    A = adc16(A, 0x0004);  /* 01AD7B ADC */
    push16(A);  /* 01AD7E PHA */
    wr16(DP(0x00), Y);  /* 01AD7F STY */
    mflag = 1;  /* 01AD81 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01AD82 STA */
    mflag = 0;  /* 01AD85 CLM */
    A = rd16(LIND(0x00));  /* 01AD86 LDA */
    Y = A;  /* 01AD88 TAY */
    S -= 3; F_01AB11();  /* 01AD89 JSRL */
    /* 01AD8D CMP */
    if ((A & 0xFF) == 0x00) goto L_01AD96;  /* 01AD8F BEQ */
    SETLO(A, 0x00);  /* 01AD91 LDA */
    Y = pull16();  /* 01AD93 PLY */
    D = pull16();  /* 01AD94 PLD */
    S += 3; return;  /* 01AD95 RTL */
L_01AD96: TICK();
    C = 1;  /* 01AD96 SEC */
    SETLO(A, sbc8(A, rd8(LIND(0x00))));  /* 01AD97 SBC */
    C = 0;  /* 01AD99 CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0293))));  /* 01AD9A ADC */
    wr8(DP(0x03), (uint8_t)A);  /* 01AD9D STA */
    /* 01AD9F CMP */
    if ((A & 0xFF) < 0x02) goto L_01ADA7;  /* 01ADA1 BCC */
    /* 01ADA3 CMP */
    if ((A & 0xFF) < 0x0B) goto L_01ADAC;  /* 01ADA5 BCC */
L_01ADA7: TICK();
    SETLO(A, 0x00);  /* 01ADA7 LDA */
    Y = pull16();  /* 01ADA9 PLY */
    D = pull16();  /* 01ADAA PLD */
    S += 3; return;  /* 01ADAB RTL */
L_01ADAC: TICK();
    mflag = 0;  /* 01ADAC CLM */
    X = rd16(ABS(0x01C7));  /* 01ADAD LDX */
    wr16(ABS(0x01D7), X);  /* 01ADB0 STX */
    A = rd16(DP(0x03));  /* 01ADB3 LDA */
    mflag = 1;  /* 01ADB5 SEM */
    /* 01ADB6 CMP */
    if ((A & 0xFF) == 0x0A) goto L_01ADC0;  /* 01ADB8 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01ADBA LDA */
    /* 01ADBC CMP */
    if ((A & 0xFF) != 0x09) goto L_01AE00;  /* 01ADBE BNE */
L_01ADC0: TICK();
    mflag = 0;  /* 01ADC0 CLM */
    A = rd16(LIND(0x00));  /* 01ADC1 LDA */
    Y = A;  /* 01ADC3 TAY */
    S -= 3; F_01A7D0();  /* 01ADC4 JSRL */
    /* 01ADC8 CMP */
    if ((A & 0xFF) == 0x00) goto L_01ADFD;  /* 01ADCA BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01ADCC LDA */
    SETLO(A, A + 1);  /* 01ADCE INC */
    SETLO(A, A + 1);  /* 01ADCF INC */
    SETLO(A, A + 1);  /* 01ADD0 INC */
    mflag = 0;  /* 01ADD1 CLM */
    Y = A;  /* 01ADD2 TAY */
    S -= 3; F_01A839();  /* 01ADD3 JSRL */
    /* 01ADD7 CMP */
    if ((A & 0xFF) == 0x00) goto L_01ADFD;  /* 01ADD9 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01ADDB LDA */
    /* 01ADDD CMP */
    if ((A & 0xFF) != 0x0A) goto L_01ADEF;  /* 01ADDF BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01ADE1 LDA */
    C = 0;  /* 01ADE3 CLC */
    SETLO(A, adc8(A, 0x07));  /* 01ADE4 ADC */
    mflag = 0;  /* 01ADE6 CLM */
    Y = A;  /* 01ADE7 TAY */
    S -= 3; F_01AA7E();  /* 01ADE8 JSRL */
    goto L_01ADFA;  /* 01ADEC JMP */
L_01ADEF: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01ADEF LDA */
    C = 0;  /* 01ADF1 CLC */
    SETLO(A, adc8(A, 0x07));  /* 01ADF2 ADC */
    mflag = 0;  /* 01ADF4 CLM */
    Y = A;  /* 01ADF5 TAY */
    S -= 3; F_01AA06();  /* 01ADF6 JSRL */
L_01ADFA: TICK();
    Y = pull16();  /* 01ADFA PLY */
    D = pull16();  /* 01ADFB PLD */
    S += 3; return;  /* 01ADFC RTL */
L_01ADFD: TICK();
    goto L_01AF27;  /* 01ADFD JMP */
L_01AE00: TICK();
    mflag = 0;  /* 01AE00 CLM */
    A = rd16(DP(0x03));  /* 01AE01 LDA */
    mflag = 1;  /* 01AE03 SEM */
    /* 01AE04 CMP */
    if ((A & 0xFF) == 0x07) goto L_01AE0E;  /* 01AE06 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01AE08 LDA */
    /* 01AE0A CMP */
    if ((A & 0xFF) != 0x06) goto L_01AE3F;  /* 01AE0C BNE */
L_01AE0E: TICK();
    mflag = 0;  /* 01AE0E CLM */
    A = rd16(LIND(0x00));  /* 01AE0F LDA */
    Y = A;  /* 01AE11 TAY */
    S -= 3; F_01A839();  /* 01AE12 JSRL */
    /* 01AE16 CMP */
    if ((A & 0xFF) == 0x00) goto L_01AE3C;  /* 01AE18 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01AE1A LDA */
    /* 01AE1C CMP */
    if ((A & 0xFF) != 0x07) goto L_01AE2E;  /* 01AE1E BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01AE20 LDA */
    C = 0;  /* 01AE22 CLC */
    SETLO(A, adc8(A, 0x04));  /* 01AE23 ADC */
    mflag = 0;  /* 01AE25 CLM */
    Y = A;  /* 01AE26 TAY */
    S -= 3; F_01AA7E();  /* 01AE27 JSRL */
    goto L_01AE39;  /* 01AE2B JMP */
L_01AE2E: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AE2E LDA */
    C = 0;  /* 01AE30 CLC */
    SETLO(A, adc8(A, 0x04));  /* 01AE31 ADC */
    mflag = 0;  /* 01AE33 CLM */
    Y = A;  /* 01AE34 TAY */
    S -= 3; F_01AA06();  /* 01AE35 JSRL */
L_01AE39: TICK();
    Y = pull16();  /* 01AE39 PLY */
    D = pull16();  /* 01AE3A PLD */
    S += 3; return;  /* 01AE3B RTL */
L_01AE3C: TICK();
    goto L_01AF27;  /* 01AE3C JMP */
L_01AE3F: TICK();
    mflag = 0;  /* 01AE3F CLM */
    A = rd16(DP(0x03));  /* 01AE40 LDA */
    mflag = 1;  /* 01AE42 SEM */
    /* 01AE43 CMP */
    if ((A & 0xFF) == 0x05) goto L_01AE4A;  /* 01AE45 BEQ */
    goto L_01AEF2;  /* 01AE47 JMP */
L_01AE4A: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AE4A LDA */
    mflag = 0;  /* 01AE4C CLM */
    A &= 0x00FF;  /* 01AE4D AND */
    X = A;  /* 01AE50 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AE51 LDA */
    mflag = 1;  /* 01AE54 SEM */
    /* 01AE55 CMP */
    if ((A & 0xFF) == 0x39) goto L_01AE5C;  /* 01AE57 BEQ */
    goto L_01AEEF;  /* 01AE59 JMP */
L_01AE5C: TICK();
    SETLO(A, rd8(ABS(0x01C2)));  /* 01AE5C LDA */
    mflag = 0;  /* 01AE5F CLM */
    A &= 0x00FF;  /* 01AE60 AND */
    wr16(DP(0x04), A);  /* 01AE63 STA */
    A = rd16(LIND(0x00));  /* 01AE65 LDA */
    A &= 0x00FF;  /* 01AE67 AND */
    A += 1;  /* 01AE6A INC */
    X = A;  /* 01AE6B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AE6C LDA */
    A &= 0x00FF;  /* 01AE6F AND */
    C = 0;  /* 01AE72 CLC */
    A = adc16(A, 0xFFD0);  /* 01AE73 ADC */
    C = 1;  /* 01AE76 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 01AE77 SBC */
    if (!V) goto L_01AE7E;  /* 01AE79 BVC */
    A ^= 0x8000;  /* 01AE7B EOR */
    N = ((A & 0x8000) != 0);
L_01AE7E: TICK();
    if (!N) goto L_01AEAB;  /* 01AE7E BPL */
    A = rd16(LIND(0x00));  /* 01AE80 LDA */
    A &= 0x00FF;  /* 01AE82 AND */
    A += 1;  /* 01AE85 INC */
    X = A;  /* 01AE86 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AE87 LDA */
    mflag = 1;  /* 01AE8A SEM */
    mpy8(0x64);  /* 01AE8B MPY */
    wr8(DP(0x04), (uint8_t)A);  /* 01AE8E STA */
    wr8(DP(0x05), (uint8_t)B);  /* 01AE90 STA */
    mflag = 0;  /* 01AE93 CLM */
    A = rd16(DP(0x04));  /* 01AE94 LDA */
    C = 0;  /* 01AE96 CLC */
    A = adc16(A, 0x089A);  /* 01AE97 ADC */
    wr16(ABS(0x01D7), A);  /* 01AE9A STA */
    A = rd16(LIND(0x00));  /* 01AE9D LDA */
    mflag = 1;  /* 01AE9F SEM */
    SETLO(A, A + 1);  /* 01AEA0 INC */
    SETLO(A, A + 1);  /* 01AEA1 INC */
    mflag = 0;  /* 01AEA2 CLM */
    Y = A;  /* 01AEA3 TAY */
    S -= 3; F_01AA7E();  /* 01AEA4 JSRL */
    Y = pull16();  /* 01AEA8 PLY */
    D = pull16();  /* 01AEA9 PLD */
    S += 3; return;  /* 01AEAA RTL */
L_01AEAB: TICK();
    A = rd16(LIND(0x00));  /* 01AEAB LDA */
    A &= 0x00FF;  /* 01AEAD AND */
    A += 1;  /* 01AEB0 INC */
    A += 1;  /* 01AEB1 INC */
    X = A;  /* 01AEB2 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AEB3 LDA */
    mflag = 1;  /* 01AEB6 SEM */
    mpy8(0x0A);  /* 01AEB7 MPY */
    wr8(DP(0x04), (uint8_t)A);  /* 01AEBA STA */
    wr8(DP(0x05), (uint8_t)B);  /* 01AEBC STA */
    SETLO(A, rd8(LIND(0x00)));  /* 01AEBF LDA */
    mflag = 0;  /* 01AEC1 CLM */
    A &= 0x00FF;  /* 01AEC2 AND */
    A += 1;  /* 01AEC5 INC */
    X = A;  /* 01AEC6 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AEC7 LDA */
    mflag = 1;  /* 01AECA SEM */
    mpy8(0x64);  /* 01AECB MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 01AECE STA */
    wr8(DP(0x07), (uint8_t)B);  /* 01AED0 STA */
    mflag = 0;  /* 01AED3 CLM */
    A = rd16(DP(0x04));  /* 01AED4 LDA */
    C = 0;  /* 01AED6 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 01AED7 ADC */
    C = 0;  /* 01AED9 CLC */
    A = adc16(A, 0x06BA);  /* 01AEDA ADC */
    wr16(ABS(0x01D7), A);  /* 01AEDD STA */
    A = rd16(LIND(0x00));  /* 01AEE0 LDA */
    mflag = 1;  /* 01AEE2 SEM */
    SETLO(A, A + 1);  /* 01AEE3 INC */
    SETLO(A, A + 1);  /* 01AEE4 INC */
    SETLO(A, A + 1);  /* 01AEE5 INC */
    mflag = 0;  /* 01AEE6 CLM */
    Y = A;  /* 01AEE7 TAY */
    S -= 3; F_01AA06();  /* 01AEE8 JSRL */
    Y = pull16();  /* 01AEEC PLY */
    D = pull16();  /* 01AEED PLD */
    S += 3; return;  /* 01AEEE RTL */
L_01AEEF: TICK();
    goto L_01AF27;  /* 01AEEF JMP */
L_01AEF2: TICK();
    mflag = 0;  /* 01AEF2 CLM */
    A = rd16(DP(0x03));  /* 01AEF3 LDA */
    mflag = 1;  /* 01AEF5 SEM */
    /* 01AEF6 CMP */
    if ((A & 0xFF) != 0x03) goto L_01AF0E;  /* 01AEF8 BNE */
    mflag = 0;  /* 01AEFA CLM */
    if ((rd16(ABS(0x039E)) & 0x0080) == 0) goto L_01AF0B;  /* 01AEFB BBC */
    A = rd16(LIND(0x00));  /* 01AF01 LDA */
    Y = A;  /* 01AF03 TAY */
    S -= 3; F_01AA7E();  /* 01AF04 JSRL */
    Y = pull16();  /* 01AF08 PLY */
    D = pull16();  /* 01AF09 PLD */
    S += 3; return;  /* 01AF0A RTL */
L_01AF0B: TICK();
    goto L_01AF27;  /* 01AF0B JMP */
L_01AF0E: TICK();
    mflag = 0;  /* 01AF0E CLM */
    A = rd16(DP(0x03));  /* 01AF0F LDA */
    mflag = 1;  /* 01AF11 SEM */
    /* 01AF12 CMP */
    if ((A & 0xFF) != 0x02) goto L_01AF27;  /* 01AF14 BNE */
    mflag = 0;  /* 01AF16 CLM */
    if ((rd16(ABS(0x039E)) & 0x0080) == 0x0080) goto L_01AF27;  /* 01AF17 BBS */
    A = rd16(LIND(0x00));  /* 01AF1D LDA */
    Y = A;  /* 01AF1F TAY */
    S -= 3; F_01AA06();  /* 01AF20 JSRL */
    Y = pull16();  /* 01AF24 PLY */
    D = pull16();  /* 01AF25 PLD */
    S += 3; return;  /* 01AF26 RTL */
L_01AF27: TICK();
    mflag = 0;  /* 01AF27 CLM */
    A = 0x0000;  /* 01AF28 LDA */
    mflag = 1;  /* 01AF2B SEM */
    Y = pull16();  /* 01AF2C PLY */
    D = pull16();  /* 01AF2D PLD */
    S += 3; return;  /* 01AF2E RTL */
}

/* 01AF2F  callers: 01B5CF, 01B6C1, 01B72C */
void F_01AF2F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01AF2F LDA */
    push16(D);  /* 01AF31 PHD */
    D = A;  /* 01AF32 TAD */
    C = 0;  /* 01AF33 CLC */
    A = adc16(A, 0x0004);  /* 01AF34 ADC */
    push16(A);  /* 01AF37 PHA */
    wr16(DP(0x00), Y);  /* 01AF38 STY */
    mflag = 1;  /* 01AF3A SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01AF3B STA */
    mflag = 0;  /* 01AF3E CLM */
    A = rd16(LIND(0x00));  /* 01AF3F LDA */
    Y = A;  /* 01AF41 TAY */
    S -= 3; F_01AB11();  /* 01AF42 JSRL */
    /* 01AF46 CMP */
    if ((A & 0xFF) == 0x00) goto L_01AF4F;  /* 01AF48 BEQ */
    SETLO(A, 0x00);  /* 01AF4A LDA */
    Y = pull16();  /* 01AF4C PLY */
    D = pull16();  /* 01AF4D PLD */
    S += 3; return;  /* 01AF4E RTL */
L_01AF4F: TICK();
    C = 1;  /* 01AF4F SEC */
    SETLO(A, sbc8(A, rd8(LIND(0x00))));  /* 01AF50 SBC */
    C = 0;  /* 01AF52 CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0293))));  /* 01AF53 ADC */
    wr8(DP(0x03), (uint8_t)A);  /* 01AF56 STA */
    /* 01AF58 CMP */
    if ((A & 0xFF) < 0x01) goto L_01AF60;  /* 01AF5A BCC */
    /* 01AF5C CMP */
    if ((A & 0xFF) < 0x0B) goto L_01AF65;  /* 01AF5E BCC */
L_01AF60: TICK();
    SETLO(A, 0x00);  /* 01AF60 LDA */
    Y = pull16();  /* 01AF62 PLY */
    D = pull16();  /* 01AF63 PLD */
    S += 3; return;  /* 01AF64 RTL */
L_01AF65: TICK();
    /* 01AF65 CMP */
    if ((A & 0xFF) < 0x08) goto L_01AF85;  /* 01AF67 BCC */
    SETLO(A, rd8(LIND(0x00)));  /* 01AF69 LDA */
    mflag = 0;  /* 01AF6B CLM */
    A &= 0x00FF;  /* 01AF6C AND */
    X = A;  /* 01AF6F TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AF70 LDA */
    mflag = 1;  /* 01AF73 SEM */
    /* 01AF74 CMP */
    if ((A & 0xFF) != 0x30) goto L_01AF85;  /* 01AF76 BNE */
    mflag = 0;  /* 01AF78 CLM */
    wr16(ABS(0x01CD), 0x1FA5);  /* 01AF79 LDM */
    A = 0x0001;  /* 01AF7E LDA */
    mflag = 1;  /* 01AF81 SEM */
    Y = pull16();  /* 01AF82 PLY */
    D = pull16();  /* 01AF83 PLD */
    S += 3; return;  /* 01AF84 RTL */
L_01AF85: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01AF85 LDA */
    /* 01AF87 CMP */
    if ((A & 0xFF) < 0x05) goto L_01AFAB;  /* 01AF89 BCC */
    /* 01AF8B CMP */
    if ((A & 0xFF) >= 0x08) goto L_01AFAB;  /* 01AF8D BCS */
    SETLO(A, rd8(LIND(0x00)));  /* 01AF8F LDA */
    mflag = 0;  /* 01AF91 CLM */
    A &= 0x00FF;  /* 01AF92 AND */
    X = A;  /* 01AF95 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AF96 LDA */
    mflag = 1;  /* 01AF99 SEM */
    /* 01AF9A CMP */
    if ((A & 0xFF) != 0x30) goto L_01AFAB;  /* 01AF9C BNE */
    mflag = 0;  /* 01AF9E CLM */
    wr16(ABS(0x01CD), 0x1FA6);  /* 01AF9F LDM */
    A = 0x0001;  /* 01AFA4 LDA */
    mflag = 1;  /* 01AFA7 SEM */
    Y = pull16();  /* 01AFA8 PLY */
    D = pull16();  /* 01AFA9 PLD */
    S += 3; return;  /* 01AFAA RTL */
L_01AFAB: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01AFAB LDA */
    /* 01AFAD CMP */
    if ((A & 0xFF) == 0x05) goto L_01AFB4;  /* 01AFAF BEQ */
    goto L_01B0B6;  /* 01AFB1 JMP */
L_01AFB4: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AFB4 LDA */
    mflag = 0;  /* 01AFB6 CLM */
    A &= 0x00FF;  /* 01AFB7 AND */
    X = A;  /* 01AFBA TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AFBB LDA */
    mflag = 1;  /* 01AFBE SEM */
    /* 01AFBF CMP */
    if ((A & 0xFF) >= 0x33) goto L_01AFC6;  /* 01AFC1 BCS */
    goto L_01B0B3;  /* 01AFC3 JMP */
L_01AFC6: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AFC6 LDA */
    mflag = 0;  /* 01AFC8 CLM */
    A &= 0x00FF;  /* 01AFC9 AND */
    X = A;  /* 01AFCC TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AFCD LDA */
    mflag = 1;  /* 01AFD0 SEM */
    /* 01AFD1 CMP */
    if ((A & 0xFF) < 0x37) goto L_01AFD8;  /* 01AFD3 BCC */
    goto L_01B0B3;  /* 01AFD5 JMP */
L_01AFD8: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AFD8 LDA */
    mflag = 0;  /* 01AFDA CLM */
    A &= 0x00FF;  /* 01AFDB AND */
    A += 1;  /* 01AFDE INC */
    X = A;  /* 01AFDF TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AFE0 LDA */
    mflag = 1;  /* 01AFE3 SEM */
    /* 01AFE4 CMP */
    if ((A & 0xFF) >= 0x31) goto L_01AFEB;  /* 01AFE6 BCS */
    goto L_01B0B3;  /* 01AFE8 JMP */
L_01AFEB: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AFEB LDA */
    mflag = 0;  /* 01AFED CLM */
    A &= 0x00FF;  /* 01AFEE AND */
    A += 1;  /* 01AFF1 INC */
    X = A;  /* 01AFF2 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01AFF3 LDA */
    mflag = 1;  /* 01AFF6 SEM */
    /* 01AFF7 CMP */
    if ((A & 0xFF) < 0x39) goto L_01AFFE;  /* 01AFF9 BCC */
    goto L_01B0B3;  /* 01AFFB JMP */
L_01AFFE: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01AFFE LDA */
    mflag = 0;  /* 01B000 CLM */
    A &= 0x00FF;  /* 01B001 AND */
    X = A;  /* 01B004 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B005 LDA */
    mflag = 1;  /* 01B008 SEM */
    /* 01B009 CMP */
    if ((A & 0xFF) != 0x34) goto L_01B014;  /* 01B00B BNE */
    wr8(ABS(0x01E1), 0x01);  /* 01B00D LDM */
    goto L_01B041;  /* 01B011 JMP */
L_01B014: TICK();
    mflag = 0;  /* 01B014 CLM */
    A = rd16(LIND(0x00));  /* 01B015 LDA */
    A &= 0x00FF;  /* 01B017 AND */
    X = A;  /* 01B01A TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B01B LDA */
    mflag = 1;  /* 01B01E SEM */
    /* 01B01F CMP */
    if ((A & 0xFF) != 0x35) goto L_01B02A;  /* 01B021 BNE */
    wr8(ABS(0x01E0), 0x01);  /* 01B023 LDM */
    goto L_01B041;  /* 01B027 JMP */
L_01B02A: TICK();
    mflag = 0;  /* 01B02A CLM */
    A = rd16(LIND(0x00));  /* 01B02B LDA */
    A &= 0x00FF;  /* 01B02D AND */
    X = A;  /* 01B030 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B031 LDA */
    mflag = 1;  /* 01B034 SEM */
    /* 01B035 CMP */
    if ((A & 0xFF) != 0x36) goto L_01B041;  /* 01B037 BNE */
    wr8(ABS(0x01E0), 0x01);  /* 01B039 LDM */
    wr8(ABS(0x01E1), 0x01);  /* 01B03D LDM */
L_01B041: TICK();
    wr8(ABS(0x01E4), 0x01);  /* 01B041 LDM */
    SETLO(A, rd8(LIND(0x00)));  /* 01B045 LDA */
    mflag = 0;  /* 01B047 CLM */
    A &= 0x00FF;  /* 01B048 AND */
    C = 0;  /* 01B04B CLC */
    A = adc16(A, 0x0004);  /* 01B04C ADC */
    X = A;  /* 01B04F TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B050 LDA */
    A &= 0x00FF;  /* 01B053 AND */
    wr16(DP(0x04), A);  /* 01B056 STA */
    A = rd16(LIND(0x00));  /* 01B058 LDA */
    A &= 0x00FF;  /* 01B05A AND */
    A += 1;  /* 01B05D INC */
    A += 1;  /* 01B05E INC */
    A += 1;  /* 01B05F INC */
    X = A;  /* 01B060 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B061 LDA */
    mflag = 1;  /* 01B064 SEM */
    mpy8(0x0A);  /* 01B065 MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 01B068 STA */
    wr8(DP(0x07), (uint8_t)B);  /* 01B06A STA */
    mflag = 0;  /* 01B06D CLM */
    A = rd16(DP(0x04));  /* 01B06E LDA */
    C = 0;  /* 01B070 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 01B071 ADC */
    wr16(DP(0x04), A);  /* 01B073 STA */
    A = rd16(LIND(0x00));  /* 01B075 LDA */
    A &= 0x00FF;  /* 01B077 AND */
    A += 1;  /* 01B07A INC */
    A += 1;  /* 01B07B INC */
    X = A;  /* 01B07C TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B07D LDA */
    mflag = 1;  /* 01B080 SEM */
    mpy8(0x64);  /* 01B081 MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 01B084 STA */
    wr8(DP(0x07), (uint8_t)B);  /* 01B086 STA */
    mflag = 0;  /* 01B089 CLM */
    A = rd16(DP(0x04));  /* 01B08A LDA */
    C = 0;  /* 01B08C CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 01B08D ADC */
    wr16(DP(0x04), A);  /* 01B08F STA */
    A = rd16(LIND(0x00));  /* 01B091 LDA */
    A &= 0x00FF;  /* 01B093 AND */
    A += 1;  /* 01B096 INC */
    X = A;  /* 01B097 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B098 LDA */
    A &= 0x00FF;  /* 01B09B AND */
    mpy16(0x03E8);  /* 01B09E MPY */
    C = 0;  /* 01B0A2 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01B0A3 ADC */
    C = 0;  /* 01B0A5 CLC */
    A = adc16(A, 0x2BC8);  /* 01B0A6 ADC */
    wr16(ABS(0x01CD), A);  /* 01B0A9 STA */
    A = 0x0001;  /* 01B0AC LDA */
    mflag = 1;  /* 01B0AF SEM */
    Y = pull16();  /* 01B0B0 PLY */
    D = pull16();  /* 01B0B1 PLD */
    S += 3; return;  /* 01B0B2 RTL */
L_01B0B3: TICK();
    goto L_01B20C;  /* 01B0B3 JMP */
L_01B0B6: TICK();
    mflag = 0;  /* 01B0B6 CLM */
    A = rd16(DP(0x03));  /* 01B0B7 LDA */
    mflag = 1;  /* 01B0B9 SEM */
    /* 01B0BA CMP */
    if ((A & 0xFF) == 0x04) goto L_01B0C1;  /* 01B0BC BEQ */
    goto L_01B150;  /* 01B0BE JMP */
L_01B0C1: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B0C1 LDA */
    mflag = 0;  /* 01B0C3 CLM */
    A &= 0x00FF;  /* 01B0C4 AND */
    X = A;  /* 01B0C7 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B0C8 LDA */
    mflag = 1;  /* 01B0CB SEM */
    /* 01B0CC CMP */
    if ((A & 0xFF) < 0x31) goto L_01B14D;  /* 01B0CE BCC */
    SETLO(A, rd8(LIND(0x00)));  /* 01B0D0 LDA */
    mflag = 0;  /* 01B0D2 CLM */
    A &= 0x00FF;  /* 01B0D3 AND */
    X = A;  /* 01B0D6 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B0D7 LDA */
    mflag = 1;  /* 01B0DA SEM */
    /* 01B0DB CMP */
    if ((A & 0xFF) >= 0x39) goto L_01B14D;  /* 01B0DD BCS */
    wr8(ABS(0x01E4), 0x01);  /* 01B0DF LDM */
    SETLO(A, rd8(LIND(0x00)));  /* 01B0E3 LDA */
    mflag = 0;  /* 01B0E5 CLM */
    A &= 0x00FF;  /* 01B0E6 AND */
    A += 1;  /* 01B0E9 INC */
    A += 1;  /* 01B0EA INC */
    A += 1;  /* 01B0EB INC */
    X = A;  /* 01B0EC TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B0ED LDA */
    A &= 0x00FF;  /* 01B0F0 AND */
    wr16(DP(0x04), A);  /* 01B0F3 STA */
    A = rd16(LIND(0x00));  /* 01B0F5 LDA */
    A &= 0x00FF;  /* 01B0F7 AND */
    A += 1;  /* 01B0FA INC */
    A += 1;  /* 01B0FB INC */
    X = A;  /* 01B0FC TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B0FD LDA */
    mflag = 1;  /* 01B100 SEM */
    mpy8(0x0A);  /* 01B101 MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 01B104 STA */
    wr8(DP(0x07), (uint8_t)B);  /* 01B106 STA */
    mflag = 0;  /* 01B109 CLM */
    A = rd16(DP(0x04));  /* 01B10A LDA */
    C = 0;  /* 01B10C CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 01B10D ADC */
    wr16(DP(0x04), A);  /* 01B10F STA */
    A = rd16(LIND(0x00));  /* 01B111 LDA */
    A &= 0x00FF;  /* 01B113 AND */
    A += 1;  /* 01B116 INC */
    X = A;  /* 01B117 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B118 LDA */
    mflag = 1;  /* 01B11B SEM */
    mpy8(0x64);  /* 01B11C MPY */
    wr8(DP(0x06), (uint8_t)A);  /* 01B11F STA */
    wr8(DP(0x07), (uint8_t)B);  /* 01B121 STA */
    mflag = 0;  /* 01B124 CLM */
    A = rd16(DP(0x04));  /* 01B125 LDA */
    C = 0;  /* 01B127 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 01B128 ADC */
    wr16(DP(0x04), A);  /* 01B12A STA */
    A = rd16(LIND(0x00));  /* 01B12C LDA */
    A &= 0x00FF;  /* 01B12E AND */
    X = A;  /* 01B131 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B132 LDA */
    A &= 0x00FF;  /* 01B135 AND */
    mpy16(0x03E8);  /* 01B138 MPY */
    C = 0;  /* 01B13C CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01B13D ADC */
    C = 0;  /* 01B13F CLC */
    A = adc16(A, 0x2BC8);  /* 01B140 ADC */
    wr16(ABS(0x01CD), A);  /* 01B143 STA */
    A = 0x0001;  /* 01B146 LDA */
    mflag = 1;  /* 01B149 SEM */
    Y = pull16();  /* 01B14A PLY */
    D = pull16();  /* 01B14B PLD */
    S += 3; return;  /* 01B14C RTL */
L_01B14D: TICK();
    goto L_01B20C;  /* 01B14D JMP */
L_01B150: TICK();
    mflag = 0;  /* 01B150 CLM */
    A = rd16(DP(0x03));  /* 01B151 LDA */
    mflag = 1;  /* 01B153 SEM */
    /* 01B154 CMP */
    if ((A & 0xFF) == 0x02) goto L_01B15B;  /* 01B156 BEQ */
    goto L_01B1E4;  /* 01B158 JMP */
L_01B15B: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B15B LDA */
    mflag = 0;  /* 01B15D CLM */
    A &= 0x00FF;  /* 01B15E AND */
    X = A;  /* 01B161 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B162 LDA */
    mflag = 1;  /* 01B165 SEM */
    /* 01B166 CMP */
    if ((A & 0xFF) != 0x30) goto L_01B1E1;  /* 01B168 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B16A LDA */
    mflag = 0;  /* 01B16C CLM */
    A &= 0x00FF;  /* 01B16D AND */
    A += 1;  /* 01B170 INC */
    X = A;  /* 01B171 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B172 LDA */
    mflag = 1;  /* 01B175 SEM */
    /* 01B176 CMP */
    if ((A & 0xFF) < 0x33) goto L_01B1E1;  /* 01B178 BCC */
    SETLO(A, rd8(LIND(0x00)));  /* 01B17A LDA */
    mflag = 0;  /* 01B17C CLM */
    A &= 0x00FF;  /* 01B17D AND */
    A += 1;  /* 01B180 INC */
    X = A;  /* 01B181 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B182 LDA */
    mflag = 1;  /* 01B185 SEM */
    /* 01B186 CMP */
    if ((A & 0xFF) >= 0x37) goto L_01B1E1;  /* 01B188 BCS */
    SETLO(A, rd8(LIND(0x00)));  /* 01B18A LDA */
    mflag = 0;  /* 01B18C CLM */
    A &= 0x00FF;  /* 01B18D AND */
    A += 1;  /* 01B190 INC */
    X = A;  /* 01B191 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B192 LDA */
    mflag = 1;  /* 01B195 SEM */
    /* 01B196 CMP */
    if ((A & 0xFF) != 0x34) goto L_01B1A1;  /* 01B198 BNE */
    wr8(ABS(0x01E1), 0x01);  /* 01B19A LDM */
    goto L_01B1D0;  /* 01B19E JMP */
L_01B1A1: TICK();
    mflag = 0;  /* 01B1A1 CLM */
    A = rd16(LIND(0x00));  /* 01B1A2 LDA */
    A &= 0x00FF;  /* 01B1A4 AND */
    A += 1;  /* 01B1A7 INC */
    X = A;  /* 01B1A8 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B1A9 LDA */
    mflag = 1;  /* 01B1AC SEM */
    /* 01B1AD CMP */
    if ((A & 0xFF) != 0x35) goto L_01B1B8;  /* 01B1AF BNE */
    wr8(ABS(0x01E0), 0x01);  /* 01B1B1 LDM */
    goto L_01B1D0;  /* 01B1B5 JMP */
L_01B1B8: TICK();
    mflag = 0;  /* 01B1B8 CLM */
    A = rd16(LIND(0x00));  /* 01B1B9 LDA */
    A &= 0x00FF;  /* 01B1BB AND */
    A += 1;  /* 01B1BE INC */
    X = A;  /* 01B1BF TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B1C0 LDA */
    mflag = 1;  /* 01B1C3 SEM */
    /* 01B1C4 CMP */
    if ((A & 0xFF) != 0x36) goto L_01B1D0;  /* 01B1C6 BNE */
    wr8(ABS(0x01E0), 0x01);  /* 01B1C8 LDM */
    wr8(ABS(0x01E1), 0x01);  /* 01B1CC LDM */
L_01B1D0: TICK();
    wr8(ABS(0x01E4), 0x01);  /* 01B1D0 LDM */
    mflag = 0;  /* 01B1D4 CLM */
    wr16(ABS(0x01CD), 0x1F40);  /* 01B1D5 LDM */
    A = 0x0001;  /* 01B1DA LDA */
    mflag = 1;  /* 01B1DD SEM */
    Y = pull16();  /* 01B1DE PLY */
    D = pull16();  /* 01B1DF PLD */
    S += 3; return;  /* 01B1E0 RTL */
L_01B1E1: TICK();
    goto L_01B20C;  /* 01B1E1 JMP */
L_01B1E4: TICK();
    mflag = 0;  /* 01B1E4 CLM */
    A = rd16(DP(0x03));  /* 01B1E5 LDA */
    mflag = 1;  /* 01B1E7 SEM */
    /* 01B1E8 CMP */
    if ((A & 0xFF) != 0x01) goto L_01B20C;  /* 01B1EA BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B1EC LDA */
    mflag = 0;  /* 01B1EE CLM */
    A &= 0x00FF;  /* 01B1EF AND */
    X = A;  /* 01B1F2 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B1F3 LDA */
    mflag = 1;  /* 01B1F6 SEM */
    /* 01B1F7 CMP */
    if ((A & 0xFF) != 0x30) goto L_01B20C;  /* 01B1F9 BNE */
    wr8(ABS(0x01E4), 0x01);  /* 01B1FB LDM */
    mflag = 0;  /* 01B1FF CLM */
    wr16(ABS(0x01CD), 0x1F40);  /* 01B200 LDM */
    A = 0x0001;  /* 01B205 LDA */
    mflag = 1;  /* 01B208 SEM */
    Y = pull16();  /* 01B209 PLY */
    D = pull16();  /* 01B20A PLD */
    S += 3; return;  /* 01B20B RTL */
L_01B20C: TICK();
    SETLO(A, 0x00);  /* 01B20C LDA */
    Y = pull16();  /* 01B20E PLY */
    D = pull16();  /* 01B20F PLD */
    S += 3; return;  /* 01B210 RTL */
}

/* 01B211  callers: 01B760 */
void F_01B211(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B211 LDA */
    push16(D);  /* 01B213 PHD */
    D = A;  /* 01B214 TAD */
    A += 1;  /* 01B215 INC */
    A += 1;  /* 01B216 INC */
    A += 1;  /* 01B217 INC */
    push16(A);  /* 01B218 PHA */
    A = rd16(ABS(0x0293));  /* 01B219 LDA */
    A &= 0x00FF;  /* 01B21C AND */
    wr16(DP(0x00), Y);  /* 01B21F STY */
    mflag = 1;  /* 01B221 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B222 STA */
    mflag = 0;  /* 01B225 CLM */
    wr16(DP(0x03), A);  /* 01B226 STA */
    A = rd16(LIND(0x00));  /* 01B228 LDA */
    A &= 0x00FF;  /* 01B22A AND */
    A += 1;  /* 01B22D INC */
    t = rd16(DP(0x03));  /* 01B22E CMP */
    if (A != t) goto L_01B25F;  /* 01B230 BNE */
    A = rd16(LIND(0x00));  /* 01B232 LDA */
    A &= 0x00FF;  /* 01B234 AND */
    X = A;  /* 01B237 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B238 LDA */
    mflag = 1;  /* 01B23B SEM */
    /* 01B23C CMP */
    if ((A & 0xFF) != 0x30) goto L_01B25F;  /* 01B23E BNE */
    wr8(ABS(0x0434), 0x00);  /* 01B240 LDM */
    Y = 0x01CB;  /* 01B244 LDY */
    mflag = 0;  /* 01B247 CLM */
    S -= 3; F_007271();  /* 01B248 JSRL */
    wr16(ABS(0x01CD), A);  /* 01B24C STA */
    X = A;  /* 01B24F TAX */
    if (X == 0) goto L_01B258;  /* 01B250 BEQ */
    A = 0x0001;  /* 01B252 LDA */
    goto L_01B25B;  /* 01B255 JMP */
L_01B258: TICK();
    A = 0x0000;  /* 01B258 LDA */
L_01B25B: TICK();
    mflag = 1;  /* 01B25B SEM */
    Y = pull16();  /* 01B25C PLY */
    D = pull16();  /* 01B25D PLD */
    S += 3; return;  /* 01B25E RTL */
L_01B25F: TICK();
    mflag = 0;  /* 01B25F CLM */
    A = 0x0000;  /* 01B260 LDA */
    mflag = 1;  /* 01B263 SEM */
    Y = pull16();  /* 01B264 PLY */
    D = pull16();  /* 01B265 PLD */
    S += 3; return;  /* 01B266 RTL */
}

/* 01B267  callers: 01B76F */
void F_01B267(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B267 LDA */
    push16(D);  /* 01B269 PHD */
    D = A;  /* 01B26A TAD */
    A += 1;  /* 01B26B INC */
    A += 1;  /* 01B26C INC */
    A += 1;  /* 01B26D INC */
    push16(A);  /* 01B26E PHA */
    A = rd16(ABS(0x0293));  /* 01B26F LDA */
    A &= 0x00FF;  /* 01B272 AND */
    wr16(DP(0x00), Y);  /* 01B275 STY */
    mflag = 1;  /* 01B277 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B278 STA */
    mflag = 0;  /* 01B27B CLM */
    wr16(DP(0x03), A);  /* 01B27C STA */
    A = rd16(LIND(0x00));  /* 01B27E LDA */
    A &= 0x00FF;  /* 01B280 AND */
    A += 1;  /* 01B283 INC */
    A += 1;  /* 01B284 INC */
    t = rd16(DP(0x03));  /* 01B285 CMP */
    if (A != t) goto L_01B2D4;  /* 01B287 BNE */
    A = rd16(LIND(0x00));  /* 01B289 LDA */
    A &= 0x00FF;  /* 01B28B AND */
    X = A;  /* 01B28E TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B28F LDA */
    mflag = 1;  /* 01B292 SEM */
    /* 01B293 CMP */
    if ((A & 0xFF) != 0x30) goto L_01B2D4;  /* 01B295 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B297 LDA */
    mflag = 0;  /* 01B299 CLM */
    A &= 0x00FF;  /* 01B29A AND */
    A += 1;  /* 01B29D INC */
    X = A;  /* 01B29E TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B29F LDA */
    mflag = 1;  /* 01B2A2 SEM */
    /* 01B2A3 CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B2D4;  /* 01B2A5 BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B2A7 LDA */
    mflag = 0;  /* 01B2A9 CLM */
    A &= 0x00FF;  /* 01B2AA AND */
    A += 1;  /* 01B2AD INC */
    X = A;  /* 01B2AE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B2AF LDA */
    C = 0;  /* 01B2B2 CLC */
    mflag = 1;  /* 01B2B3 SEM */
    SETLO(A, adc8(A, 0xD0));  /* 01B2B4 ADC */
    wr8(ABS(0x0434), (uint8_t)A);  /* 01B2B6 STA */
    Y = 0x01CD;  /* 01B2B9 LDY */
    mflag = 0;  /* 01B2BC CLM */
    S -= 3; F_007271();  /* 01B2BD JSRL */
    wr16(ABS(0x01CD), A);  /* 01B2C1 STA */
    X = A;  /* 01B2C4 TAX */
    if (X == 0) goto L_01B2CD;  /* 01B2C5 BEQ */
    A = 0x0001;  /* 01B2C7 LDA */
    goto L_01B2D0;  /* 01B2CA JMP */
L_01B2CD: TICK();
    A = 0x0000;  /* 01B2CD LDA */
L_01B2D0: TICK();
    mflag = 1;  /* 01B2D0 SEM */
    Y = pull16();  /* 01B2D1 PLY */
    D = pull16();  /* 01B2D2 PLD */
    S += 3; return;  /* 01B2D3 RTL */
L_01B2D4: TICK();
    mflag = 0;  /* 01B2D4 CLM */
    A = rd16(ABS(0x0293));  /* 01B2D5 LDA */
    A &= 0x00FF;  /* 01B2D8 AND */
    wr16(DP(0x03), A);  /* 01B2DB STA */
    A = rd16(LIND(0x00));  /* 01B2DD LDA */
    A &= 0x00FF;  /* 01B2DF AND */
    A += 1;  /* 01B2E2 INC */
    A += 1;  /* 01B2E3 INC */
    A += 1;  /* 01B2E4 INC */
    t = rd16(DP(0x03));  /* 01B2E5 CMP */
    if (A != t) goto L_01B35C;  /* 01B2E7 BNE */
    A = rd16(LIND(0x00));  /* 01B2E9 LDA */
    A &= 0x00FF;  /* 01B2EB AND */
    X = A;  /* 01B2EE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B2EF LDA */
    mflag = 1;  /* 01B2F2 SEM */
    /* 01B2F3 CMP */
    if ((A & 0xFF) != 0x30) goto L_01B35C;  /* 01B2F5 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B2F7 LDA */
    mflag = 0;  /* 01B2F9 CLM */
    A &= 0x00FF;  /* 01B2FA AND */
    A += 1;  /* 01B2FD INC */
    X = A;  /* 01B2FE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B2FF LDA */
    mflag = 1;  /* 01B302 SEM */
    /* 01B303 CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B35C;  /* 01B305 BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B307 LDA */
    mflag = 0;  /* 01B309 CLM */
    A &= 0x00FF;  /* 01B30A AND */
    A += 1;  /* 01B30D INC */
    A += 1;  /* 01B30E INC */
    X = A;  /* 01B30F TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B310 LDA */
    mflag = 1;  /* 01B313 SEM */
    /* 01B314 CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B35C;  /* 01B316 BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B318 LDA */
    mflag = 0;  /* 01B31A CLM */
    A &= 0x00FF;  /* 01B31B AND */
    A += 1;  /* 01B31E INC */
    A += 1;  /* 01B31F INC */
    X = A;  /* 01B320 TAX */
    A = rd16(LIND(0x00));  /* 01B321 LDA */
    A &= 0x00FF;  /* 01B323 AND */
    A += 1;  /* 01B326 INC */
    Y = A;  /* 01B327 TAY */
    A = rd16((ABS(0x0283) + Y));  /* 01B328 LDA */
    mflag = 1;  /* 01B32B SEM */
    mpy8(0x0A);  /* 01B32C MPY */
    C = 0;  /* 01B32F CLC */
    SETLO(A, adc8(A, rd8((ABS(0x0283) + X))));  /* 01B330 ADC */
    C = 0;  /* 01B333 CLC */
    SETLO(A, adc8(A, 0xF0));  /* 01B334 ADC */
    wr8(ABS(0x0434), (uint8_t)A);  /* 01B336 STA */
    Y = 0x01CD;  /* 01B339 LDY */
    mflag = 0;  /* 01B33C CLM */
    S -= 3; F_007271();  /* 01B33D JSRL */
    wr16(ABS(0x01CD), A);  /* 01B341 STA */
    X = A;  /* 01B344 TAX */
    if (X == 0) goto L_01B354;  /* 01B345 BEQ */
    A = rd16(ABS(0x0434));  /* 01B347 LDA */
    mflag = 1;  /* 01B34A SEM */
    /* 01B34B CMP */
    if ((A & 0xFF) >= 0x20) goto L_01B354;  /* 01B34D BCS */
    SETLO(A, 0x01);  /* 01B34F LDA */
    goto L_01B358;  /* 01B351 JMP */
L_01B354: TICK();
    mflag = 0;  /* 01B354 CLM */
    A = 0x0000;  /* 01B355 LDA */
L_01B358: TICK();
    mflag = 1;  /* 01B358 SEM */
    Y = pull16();  /* 01B359 PLY */
    D = pull16();  /* 01B35A PLD */
    S += 3; return;  /* 01B35B RTL */
L_01B35C: TICK();
    mflag = 0;  /* 01B35C CLM */
    A = 0x0000;  /* 01B35D LDA */
    mflag = 1;  /* 01B360 SEM */
    Y = pull16();  /* 01B361 PLY */
    D = pull16();  /* 01B362 PLD */
    S += 3; return;  /* 01B363 RTL */
}

/* 01B364  callers: 01B567, 01B609 */
void F_01B364(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B364 LDA */
    push16(D);  /* 01B366 PHD */
    D = A;  /* 01B367 TAD */
    C = 0;  /* 01B368 CLC */
    A = adc16(A, 0x0005);  /* 01B369 ADC */
    push16(A);  /* 01B36C PHA */
    A = rd16(ABS(0x0293));  /* 01B36D LDA */
    A &= 0x00FF;  /* 01B370 AND */
    wr16(DP(0x00), Y);  /* 01B373 STY */
    mflag = 1;  /* 01B375 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B376 STA */
    mflag = 0;  /* 01B379 CLM */
    wr16(DP(0x03), A);  /* 01B37A STA */
    A = rd16(LIND(0x00));  /* 01B37C LDA */
    A &= 0x00FF;  /* 01B37E AND */
    A += 1;  /* 01B381 INC */
    t = rd16(DP(0x03));  /* 01B382 CMP */
    if (A != t) goto L_01B3EE;  /* 01B384 BNE */
    A = rd16(LIND(0x00));  /* 01B386 LDA */
    A &= 0x00FF;  /* 01B388 AND */
    X = A;  /* 01B38B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B38C LDA */
    mflag = 1;  /* 01B38F SEM */
    /* 01B390 CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B3EB;  /* 01B392 BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B394 LDA */
    mflag = 0;  /* 01B396 CLM */
    A &= 0x00FF;  /* 01B397 AND */
    X = A;  /* 01B39A TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B39B LDA */
    mflag = 1;  /* 01B39E SEM */
    /* 01B39F CMP */
    if ((A & 0xFF) == 0x30) goto L_01B3EB;  /* 01B3A1 BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B3A3 LDA */
    mflag = 0;  /* 01B3A5 CLM */
    A &= 0x00FF;  /* 01B3A6 AND */
    X = A;  /* 01B3A9 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B3AA LDA */
    A &= 0x00FF;  /* 01B3AD AND */
    C = 0;  /* 01B3B0 CLC */
    A = adc16(A, 0x1F88);  /* 01B3B1 ADC */
    wr16(ABS(0x01CD), A);  /* 01B3B4 STA */
    A = rd16(LIND(0x00));  /* 01B3B7 LDA */
    A &= 0x00FF;  /* 01B3B9 AND */
    X = A;  /* 01B3BC TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B3BD LDA */
    A &= 0x00FF;  /* 01B3C0 AND */
    wr16(DP(0x03), A);  /* 01B3C3 STA */
    Y = 0x01CF;  /* 01B3C5 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 01B3C8 JSRL */
    mflag = 0;  /* 01B3CC CLM */
    A &= 0x00FF;  /* 01B3CD AND */
    C = 0;  /* 01B3D0 CLC */
    A = adc16(A, 0x0030);  /* 01B3D1 ADC */
    C = 1;  /* 01B3D4 SEC */
    A = sbc16(A, rd16(DP(0x03)));  /* 01B3D5 SBC */
    if (!V) goto L_01B3DC;  /* 01B3D7 BVC */
    A ^= 0x8000;  /* 01B3D9 EOR */
    N = ((A & 0x8000) != 0);
L_01B3DC: TICK();
    if (N) goto L_01B3E4;  /* 01B3DC BMI */
    A = 0x0001;  /* 01B3DE LDA */
    goto L_01B3E7;  /* 01B3E1 JMP */
L_01B3E4: TICK();
    A = 0x0000;  /* 01B3E4 LDA */
L_01B3E7: TICK();
    mflag = 1;  /* 01B3E7 SEM */
    Y = pull16();  /* 01B3E8 PLY */
    D = pull16();  /* 01B3E9 PLD */
    S += 3; return;  /* 01B3EA RTL */
L_01B3EB: TICK();
    goto L_01B475;  /* 01B3EB JMP */
L_01B3EE: TICK();
    mflag = 0;  /* 01B3EE CLM */
    A = rd16(ABS(0x0293));  /* 01B3EF LDA */
    A &= 0x00FF;  /* 01B3F2 AND */
    wr16(DP(0x03), A);  /* 01B3F5 STA */
    A = rd16(LIND(0x00));  /* 01B3F7 LDA */
    A &= 0x00FF;  /* 01B3F9 AND */
    A += 1;  /* 01B3FC INC */
    A += 1;  /* 01B3FD INC */
    t = rd16(DP(0x03));  /* 01B3FE CMP */
    if (A != t) goto L_01B475;  /* 01B400 BNE */
    A = rd16(LIND(0x00));  /* 01B402 LDA */
    A &= 0x00FF;  /* 01B404 AND */
    X = A;  /* 01B407 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B408 LDA */
    mflag = 1;  /* 01B40B SEM */
    /* 01B40C CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B475;  /* 01B40E BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B410 LDA */
    mflag = 0;  /* 01B412 CLM */
    A &= 0x00FF;  /* 01B413 AND */
    A += 1;  /* 01B416 INC */
    X = A;  /* 01B417 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B418 LDA */
    mflag = 1;  /* 01B41B SEM */
    /* 01B41C CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B475;  /* 01B41E BEQ */
    SETLO(A, rd8(LIND(0x00)));  /* 01B420 LDA */
    mflag = 0;  /* 01B422 CLM */
    A &= 0x00FF;  /* 01B423 AND */
    A += 1;  /* 01B426 INC */
    X = A;  /* 01B427 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B428 LDA */
    A &= 0x00FF;  /* 01B42B AND */
    wr16(DP(0x03), A);  /* 01B42E STA */
    A = rd16(LIND(0x00));  /* 01B430 LDA */
    A &= 0x00FF;  /* 01B432 AND */
    X = A;  /* 01B435 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B436 LDA */
    mflag = 1;  /* 01B439 SEM */
    mpy8(0x0A);  /* 01B43A MPY */
    wr8(DP(0x05), (uint8_t)A);  /* 01B43D STA */
    wr8(DP(0x06), (uint8_t)B);  /* 01B43F STA */
    mflag = 0;  /* 01B442 CLM */
    A = rd16(DP(0x03));  /* 01B443 LDA */
    C = 0;  /* 01B445 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 01B446 ADC */
    C = 0;  /* 01B448 CLC */
    A = adc16(A, 0x1DA8);  /* 01B449 ADC */
    wr16(ABS(0x01CD), A);  /* 01B44C STA */
    C = (A >= 0x1FB9);  /* 01B44F CMP */
    if (!C) goto L_01B475;  /* 01B452 BCC */
    A = adc16(A, 0xE047);  /* 01B454 ADC */
    wr16(DP(0x03), A);  /* 01B457 STA */
    Y = 0x01CF;  /* 01B459 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 01B45C JSRL */
    mflag = 0;  /* 01B460 CLM */
    A &= 0x00FF;  /* 01B461 AND */
    t = rd16(DP(0x03));  /* 01B464 CMP */
    if (A < t) goto L_01B46E;  /* 01B466 BCC */
    A = 0x0001;  /* 01B468 LDA */
    goto L_01B471;  /* 01B46B JMP */
L_01B46E: TICK();
    A = 0x0000;  /* 01B46E LDA */
L_01B471: TICK();
    mflag = 1;  /* 01B471 SEM */
    Y = pull16();  /* 01B472 PLY */
    D = pull16();  /* 01B473 PLD */
    S += 3; return;  /* 01B474 RTL */
L_01B475: TICK();
    mflag = 0;  /* 01B475 CLM */
    A = 0x0000;  /* 01B476 LDA */
    mflag = 1;  /* 01B479 SEM */
    Y = pull16();  /* 01B47A PLY */
    D = pull16();  /* 01B47B PLD */
    S += 3; return;  /* 01B47C RTL */
}

/* 01B47D  callers: 01B535, 01B5F1, 01B6E3, 01B73B */
void F_01B47D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B47D LDA */
    push16(D);  /* 01B47F PHD */
    D = A;  /* 01B480 TAD */
    wr16(DP(0x00), Y);  /* 01B481 STY */
    mflag = 1;  /* 01B483 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B484 STA */
    SETLO(A, rd8(LIND(0x00)));  /* 01B487 LDA */
    mflag = 0;  /* 01B489 CLM */
    A &= 0x00FF;  /* 01B48A AND */
    X = A;  /* 01B48D TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B48E LDA */
    mflag = 1;  /* 01B491 SEM */
    /* 01B492 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B4FD;  /* 01B494 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B496 LDA */
    mflag = 0;  /* 01B498 CLM */
    A &= 0x00FF;  /* 01B499 AND */
    A += 1;  /* 01B49C INC */
    X = A;  /* 01B49D TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B49E LDA */
    mflag = 1;  /* 01B4A1 SEM */
    /* 01B4A2 CMP */
    if ((A & 0xFF) != 0x31) goto L_01B4FD;  /* 01B4A4 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B4A6 LDA */
    mflag = 0;  /* 01B4A8 CLM */
    A &= 0x00FF;  /* 01B4A9 AND */
    A += 1;  /* 01B4AC INC */
    A += 1;  /* 01B4AD INC */
    X = A;  /* 01B4AE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B4AF LDA */
    mflag = 1;  /* 01B4B2 SEM */
    /* 01B4B3 CMP */
    if ((A & 0xFF) != 0x39) goto L_01B4FD;  /* 01B4B5 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B4B7 LDA */
    mflag = 0;  /* 01B4B9 CLM */
    A &= 0x00FF;  /* 01B4BA AND */
    A += 1;  /* 01B4BD INC */
    A += 1;  /* 01B4BE INC */
    A += 1;  /* 01B4BF INC */
    X = A;  /* 01B4C0 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B4C1 LDA */
    mflag = 1;  /* 01B4C4 SEM */
    /* 01B4C5 CMP */
    if ((A & 0xFF) != 0x38) goto L_01B4FD;  /* 01B4C7 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B4C9 LDA */
    mflag = 0;  /* 01B4CB CLM */
    A &= 0x00FF;  /* 01B4CC AND */
    C = 0;  /* 01B4CF CLC */
    A = adc16(A, 0x0004);  /* 01B4D0 ADC */
    X = A;  /* 01B4D3 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B4D4 LDA */
    mflag = 1;  /* 01B4D7 SEM */
    /* 01B4D8 CMP */
    if ((A & 0xFF) != 0x37) goto L_01B4FD;  /* 01B4DA BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 01B4DC LDA */
    mflag = 0;  /* 01B4DF CLM */
    A &= 0x00FF;  /* 01B4E0 AND */
    wr16(DP(0x03), A);  /* 01B4E3 STA */
    A = rd16(LIND(0x00));  /* 01B4E5 LDA */
    A &= 0x00FF;  /* 01B4E7 AND */
    C = 0;  /* 01B4EA CLC */
    A = adc16(A, 0x0005);  /* 01B4EB ADC */
    t = rd16(DP(0x03));  /* 01B4EE CMP */
    if (A != t) goto L_01B4FD;  /* 01B4F0 BNE */
    wr16(ABS(0x01CD), 0x1FFF);  /* 01B4F2 LDM */
    A = 0x0001;  /* 01B4F7 LDA */
    mflag = 1;  /* 01B4FA SEM */
    D = pull16();  /* 01B4FB PLD */
    S += 3; return;  /* 01B4FC RTL */
L_01B4FD: TICK();
    mflag = 0;  /* 01B4FD CLM */
    A = 0x0000;  /* 01B4FE LDA */
    mflag = 1;  /* 01B501 SEM */
    D = pull16();  /* 01B502 PLD */
    S += 3; return;  /* 01B503 RTL */
}

/* 01B504  callers: 01B63E, 01B680 */
void F_01B504(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B504 LDA */
    push16(D);  /* 01B506 PHD */
    D = A;  /* 01B507 TAD */
    A += 1;  /* 01B508 INC */
    A += 1;  /* 01B509 INC */
    A += 1;  /* 01B50A INC */
    push16(A);  /* 01B50B PHA */
    wr16(DP(0x00), Y);  /* 01B50C STY */
    mflag = 1;  /* 01B50E SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B50F STA */
    mflag = 0;  /* 01B512 CLM */
    S -= 3; F_01AD76();  /* 01B513 JSRL */
    /* 01B517 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B520;  /* 01B519 BEQ */
    SETLO(A, 0x01);  /* 01B51B LDA */
    Y = pull16();  /* 01B51D PLY */
    D = pull16();  /* 01B51E PLD */
    S += 3; return;  /* 01B51F RTL */
L_01B520: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B520 LDA */
    mflag = 0;  /* 01B522 CLM */
    A &= 0x00FF;  /* 01B523 AND */
    X = A;  /* 01B526 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B527 LDA */
    mflag = 1;  /* 01B52A SEM */
    /* 01B52B CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B53C;  /* 01B52D BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B52F LDA */
    Y = rd16(DP(0x00));  /* 01B532 LDY */
    mflag = 0;  /* 01B534 CLM */
    S -= 3; F_01B47D();  /* 01B535 JSRL */
    goto L_01B53E;  /* 01B539 JMP */
L_01B53C: TICK();
    SETLO(A, 0x00);  /* 01B53C LDA */
L_01B53E: TICK();
    Y = pull16();  /* 01B53E PLY */
    D = pull16();  /* 01B53F PLD */
    S += 3; return;  /* 01B540 RTL */
}

/* 01B541  callers: 01B6F6, 01B987 */
void F_01B541(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B541 LDA */
    push16(D);  /* 01B543 PHD */
    D = A;  /* 01B544 TAD */
    A += 1;  /* 01B545 INC */
    A += 1;  /* 01B546 INC */
    A += 1;  /* 01B547 INC */
    push16(A);  /* 01B548 PHA */
    wr16(DP(0x00), Y);  /* 01B549 STY */
    mflag = 1;  /* 01B54B SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B54C STA */
    SETLO(A, rd8(LIND(0x00)));  /* 01B54F LDA */
    mflag = 0;  /* 01B551 CLM */
    A &= 0x00FF;  /* 01B552 AND */
    X = A;  /* 01B555 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B556 LDA */
    mflag = 1;  /* 01B559 SEM */
    /* 01B55A CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B56E;  /* 01B55C BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B55E LDA */
    SETLO(A, A + 1);  /* 01B560 INC */
    wr8(LIND(0x00), (uint8_t)A);  /* 01B561 STA */
    SETLO(B, rd8(DP(0x02)));  /* 01B563 LDA */
    mflag = 0;  /* 01B566 CLM */
    S -= 3; F_01B364();  /* 01B567 JSRL */
    Y = pull16();  /* 01B56B PLY */
    D = pull16();  /* 01B56C PLD */
    S += 3; return;  /* 01B56D RTL */
L_01B56E: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B56E LDA */
    mflag = 0;  /* 01B570 CLM */
    A &= 0x00FF;  /* 01B571 AND */
    X = A;  /* 01B574 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B575 LDA */
    mflag = 1;  /* 01B578 SEM */
    /* 01B579 CMP */
    if ((A & 0xFF) == 0x33) goto L_01B580;  /* 01B57B BEQ */
    goto L_01B615;  /* 01B57D JMP */
L_01B580: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B580 LDA */
    mflag = 0;  /* 01B582 CLM */
    A &= 0x00FF;  /* 01B583 AND */
    A += 1;  /* 01B586 INC */
    X = A;  /* 01B587 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B588 LDA */
    mflag = 1;  /* 01B58B SEM */
    /* 01B58C CMP */
    if ((A & 0xFF) == 0x31) goto L_01B593;  /* 01B58E BEQ */
    goto L_01B615;  /* 01B590 JMP */
L_01B593: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B593 LDA */
    mflag = 0;  /* 01B595 CLM */
    A &= 0x00FF;  /* 01B596 AND */
    A += 1;  /* 01B599 INC */
    A += 1;  /* 01B59A INC */
    X = A;  /* 01B59B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B59C LDA */
    mflag = 1;  /* 01B59F SEM */
    /* 01B5A0 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B615;  /* 01B5A2 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B5A4 LDA */
    SETLO(A, A + 1);  /* 01B5A6 INC */
    SETLO(A, A + 1);  /* 01B5A7 INC */
    SETLO(A, A + 1);  /* 01B5A8 INC */
    wr8(LIND(0x00), (uint8_t)A);  /* 01B5A9 STA */
    wr8(ABS(0x01E2), 0x01);  /* 01B5AB LDM */
    SETLO(B, rd8(DP(0x02)));  /* 01B5AF LDA */
    mflag = 0;  /* 01B5B2 CLM */
    S -= 3; F_01AB3B();  /* 01B5B3 JSRL */
    /* 01B5B7 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B5D7;  /* 01B5B9 BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B5BB LDA */
    Y = rd16(DP(0x00));  /* 01B5BE LDY */
    mflag = 0;  /* 01B5C0 CLM */
    S -= 3; F_01AD76();  /* 01B5C1 JSRL */
    /* 01B5C5 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B5D7;  /* 01B5C7 BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B5C9 LDA */
    Y = rd16(DP(0x00));  /* 01B5CC LDY */
    mflag = 0;  /* 01B5CE CLM */
    S -= 3; F_01AF2F();  /* 01B5CF JSRL */
    /* 01B5D3 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B5DC;  /* 01B5D5 BEQ */
L_01B5D7: TICK();
    SETLO(A, 0x01);  /* 01B5D7 LDA */
    Y = pull16();  /* 01B5D9 PLY */
    D = pull16();  /* 01B5DA PLD */
    S += 3; return;  /* 01B5DB RTL */
L_01B5DC: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B5DC LDA */
    mflag = 0;  /* 01B5DE CLM */
    A &= 0x00FF;  /* 01B5DF AND */
    X = A;  /* 01B5E2 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B5E3 LDA */
    mflag = 1;  /* 01B5E6 SEM */
    /* 01B5E7 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B610;  /* 01B5E9 BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B5EB LDA */
    Y = rd16(DP(0x00));  /* 01B5EE LDY */
    mflag = 0;  /* 01B5F0 CLM */
    S -= 3; F_01B47D();  /* 01B5F1 JSRL */
    /* 01B5F5 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B5FE;  /* 01B5F7 BEQ */
    SETLO(A, 0x01);  /* 01B5F9 LDA */
    Y = pull16();  /* 01B5FB PLY */
    D = pull16();  /* 01B5FC PLD */
    S += 3; return;  /* 01B5FD RTL */
L_01B5FE: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B5FE LDA */
    SETLO(A, A + 1);  /* 01B600 INC */
    wr8(LIND(0x00), (uint8_t)A);  /* 01B601 STA */
    SETLO(B, rd8(DP(0x02)));  /* 01B603 LDA */
    Y = rd16(DP(0x00));  /* 01B606 LDY */
    mflag = 0;  /* 01B608 CLM */
    S -= 3; F_01B364();  /* 01B609 JSRL */
    Y = pull16();  /* 01B60D PLY */
    D = pull16();  /* 01B60E PLD */
    S += 3; return;  /* 01B60F RTL */
L_01B610: TICK();
    SETLO(A, 0x00);  /* 01B610 LDA */
    Y = pull16();  /* 01B612 PLY */
    D = pull16();  /* 01B613 PLD */
    S += 3; return;  /* 01B614 RTL */
L_01B615: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B615 LDA */
    mflag = 0;  /* 01B617 CLM */
    A &= 0x00FF;  /* 01B618 AND */
    X = A;  /* 01B61B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B61C LDA */
    mflag = 1;  /* 01B61F SEM */
    /* 01B620 CMP */
    if ((A & 0xFF) != 0x31) goto L_01B645;  /* 01B622 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B624 LDA */
    mflag = 0;  /* 01B626 CLM */
    A &= 0x00FF;  /* 01B627 AND */
    A += 1;  /* 01B62A INC */
    X = A;  /* 01B62B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B62C LDA */
    mflag = 1;  /* 01B62F SEM */
    /* 01B630 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B645;  /* 01B632 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B634 LDA */
    SETLO(A, A + 1);  /* 01B636 INC */
    SETLO(A, A + 1);  /* 01B637 INC */
    wr8(LIND(0x00), (uint8_t)A);  /* 01B638 STA */
    SETLO(B, rd8(DP(0x02)));  /* 01B63A LDA */
    mflag = 0;  /* 01B63D CLM */
    S -= 3; F_01B504();  /* 01B63E JSRL */
    Y = pull16();  /* 01B642 PLY */
    D = pull16();  /* 01B643 PLD */
    S += 3; return;  /* 01B644 RTL */
L_01B645: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B645 LDA */
    mflag = 0;  /* 01B647 CLM */
    A &= 0x00FF;  /* 01B648 AND */
    X = A;  /* 01B64B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B64C LDA */
    mflag = 1;  /* 01B64F SEM */
    /* 01B650 CMP */
    if ((A & 0xFF) != 0x31) goto L_01B691;  /* 01B652 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B654 LDA */
    mflag = 0;  /* 01B656 CLM */
    A &= 0x00FF;  /* 01B657 AND */
    A += 1;  /* 01B65A INC */
    X = A;  /* 01B65B TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B65C LDA */
    mflag = 1;  /* 01B65F SEM */
    /* 01B660 CMP */
    if ((A & 0xFF) != 0x31) goto L_01B691;  /* 01B662 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B664 LDA */
    mflag = 0;  /* 01B666 CLM */
    A &= 0x00FF;  /* 01B667 AND */
    A += 1;  /* 01B66A INC */
    A += 1;  /* 01B66B INC */
    X = A;  /* 01B66C TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B66D LDA */
    mflag = 1;  /* 01B670 SEM */
    /* 01B671 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B691;  /* 01B673 BNE */
    SETLO(A, rd8(LIND(0x00)));  /* 01B675 LDA */
    SETLO(A, A + 1);  /* 01B677 INC */
    SETLO(A, A + 1);  /* 01B678 INC */
    SETLO(A, A + 1);  /* 01B679 INC */
    wr8(LIND(0x00), (uint8_t)A);  /* 01B67A STA */
    SETLO(B, rd8(DP(0x02)));  /* 01B67C LDA */
    mflag = 0;  /* 01B67F CLM */
    S -= 3; F_01B504();  /* 01B680 JSRL */
    /* 01B684 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B691;  /* 01B686 BEQ */
    wr8(ABS(0x01E0), 0x01);  /* 01B688 LDM */
    SETLO(A, 0x01);  /* 01B68C LDA */
    Y = pull16();  /* 01B68E PLY */
    D = pull16();  /* 01B68F PLD */
    S += 3; return;  /* 01B690 RTL */
L_01B691: TICK();
    SETLO(A, 0x00);  /* 01B691 LDA */
    Y = pull16();  /* 01B693 PLY */
    D = pull16();  /* 01B694 PLD */
    S += 3; return;  /* 01B695 RTL */
}

/* 01B696  callers: 01B913, 01B960 */
void F_01B696(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B696 LDA */
    push16(D);  /* 01B698 PHD */
    D = A;  /* 01B699 TAD */
    A += 1;  /* 01B69A INC */
    A += 1;  /* 01B69B INC */
    A += 1;  /* 01B69C INC */
    push16(A);  /* 01B69D PHA */
    wr16(DP(0x00), Y);  /* 01B69E STY */
    mflag = 1;  /* 01B6A0 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 01B6A1 STA */
    mflag = 0;  /* 01B6A4 CLM */
    S -= 3; F_01AB3B();  /* 01B6A5 JSRL */
    /* 01B6A9 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B6C9;  /* 01B6AB BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B6AD LDA */
    Y = rd16(DP(0x00));  /* 01B6B0 LDY */
    mflag = 0;  /* 01B6B2 CLM */
    S -= 3; F_01AD76();  /* 01B6B3 JSRL */
    /* 01B6B7 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B6C9;  /* 01B6B9 BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B6BB LDA */
    Y = rd16(DP(0x00));  /* 01B6BE LDY */
    mflag = 0;  /* 01B6C0 CLM */
    S -= 3; F_01AF2F();  /* 01B6C1 JSRL */
    /* 01B6C5 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B6CE;  /* 01B6C7 BEQ */
L_01B6C9: TICK();
    SETLO(A, 0x01);  /* 01B6C9 LDA */
    Y = pull16();  /* 01B6CB PLY */
    D = pull16();  /* 01B6CC PLD */
    S += 3; return;  /* 01B6CD RTL */
L_01B6CE: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 01B6CE LDA */
    mflag = 0;  /* 01B6D0 CLM */
    A &= 0x00FF;  /* 01B6D1 AND */
    X = A;  /* 01B6D4 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B6D5 LDA */
    mflag = 1;  /* 01B6D8 SEM */
    /* 01B6D9 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B6F0;  /* 01B6DB BNE */
    SETLO(B, rd8(DP(0x02)));  /* 01B6DD LDA */
    Y = rd16(DP(0x00));  /* 01B6E0 LDY */
    mflag = 0;  /* 01B6E2 CLM */
    S -= 3; F_01B47D();  /* 01B6E3 JSRL */
    /* 01B6E7 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B6F0;  /* 01B6E9 BEQ */
    SETLO(A, 0x01);  /* 01B6EB LDA */
    Y = pull16();  /* 01B6ED PLY */
    D = pull16();  /* 01B6EE PLD */
    S += 3; return;  /* 01B6EF RTL */
L_01B6F0: TICK();
    SETLO(B, rd8(DP(0x02)));  /* 01B6F0 LDA */
    Y = rd16(DP(0x00));  /* 01B6F3 LDY */
    mflag = 0;  /* 01B6F5 CLM */
    S -= 3; F_01B541();  /* 01B6F6 JSRL */
    Y = pull16();  /* 01B6FA PLY */
    D = pull16();  /* 01B6FB PLD */
    S += 3; return;  /* 01B6FC RTL */
}

/* 01B6FD  callers: 01634C */
void F_01B6FD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01B6FD LDA */
    push16(D);  /* 01B6FF PHD */
    D = A;  /* 01B700 TAD */
    A += 1;  /* 01B701 INC */
    push16(A);  /* 01B702 PHA */
    mflag = 1;  /* 01B703 SEM */
    wr8(DP(0x00), 0x00);  /* 01B704 LDM */
    mflag = 0;  /* 01B707 CLM */
    A = D;  /* 01B708 TDA */
    B = 0x0000;  /* 01B709 LDA */
    Y = A;  /* 01B70D TAY */
    S -= 3; F_01AB3B();  /* 01B70E JSRL */
    /* 01B712 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B743;  /* 01B714 BNE */
    mflag = 0;  /* 01B716 CLM */
    A = D;  /* 01B717 TDA */
    B = 0x0000;  /* 01B718 LDA */
    Y = A;  /* 01B71C TAY */
    S -= 3; F_01AD76();  /* 01B71D JSRL */
    /* 01B721 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B743;  /* 01B723 BNE */
    mflag = 0;  /* 01B725 CLM */
    A = D;  /* 01B726 TDA */
    B = 0x0000;  /* 01B727 LDA */
    Y = A;  /* 01B72B TAY */
    S -= 3; F_01AF2F();  /* 01B72C JSRL */
    /* 01B730 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B743;  /* 01B732 BNE */
    mflag = 0;  /* 01B734 CLM */
    A = D;  /* 01B735 TDA */
    B = 0x0000;  /* 01B736 LDA */
    Y = A;  /* 01B73A TAY */
    S -= 3; F_01B47D();  /* 01B73B JSRL */
    /* 01B73F CMP */
    if ((A & 0xFF) == 0x00) goto L_01B74C;  /* 01B741 BEQ */
L_01B743: TICK();
    wr8(ABS(0x01BD), 0x40);  /* 01B743 LDM */
    SETLO(A, 0x01);  /* 01B747 LDA */
    Y = pull16();  /* 01B749 PLY */
    D = pull16();  /* 01B74A PLD */
    S += 3; return;  /* 01B74B RTL */
L_01B74C: TICK();
    SETLO(A, rd8(ABS(0x0283)));  /* 01B74C LDA */
    /* 01B74F CMP */
    if ((A & 0xFF) == 0x2A) goto L_01B756;  /* 01B751 BEQ */
    goto L_01B998;  /* 01B753 JMP */
L_01B756: TICK();
    wr8(DP(0x00), 0x01);  /* 01B756 LDM */
    mflag = 0;  /* 01B759 CLM */
    A = D;  /* 01B75A TDA */
    B = 0x0000;  /* 01B75B LDA */
    Y = A;  /* 01B75F TAY */
    S -= 3; F_01B211();  /* 01B760 JSRL */
    /* 01B764 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B777;  /* 01B766 BNE */
    mflag = 0;  /* 01B768 CLM */
    A = D;  /* 01B769 TDA */
    B = 0x0000;  /* 01B76A LDA */
    Y = A;  /* 01B76E TAY */
    S -= 3; F_01B267();  /* 01B76F JSRL */
    /* 01B773 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B780;  /* 01B775 BEQ */
L_01B777: TICK();
    wr8(ABS(0x01BD), 0x58);  /* 01B777 LDM */
    SETLO(A, 0x01);  /* 01B77B LDA */
    Y = pull16();  /* 01B77D PLY */
    D = pull16();  /* 01B77E PLD */
    S += 3; return;  /* 01B77F RTL */
L_01B780: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 01B780 LDA */
    mflag = 0;  /* 01B782 CLM */
    A &= 0x00FF;  /* 01B783 AND */
    X = A;  /* 01B786 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B787 LDA */
    mflag = 1;  /* 01B78A SEM */
    /* 01B78B CMP */
    if ((A & 0xFF) == 0x30) goto L_01B792;  /* 01B78D BEQ */
    goto L_01B89A;  /* 01B78F JMP */
L_01B792: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 01B792 LDA */
    mflag = 0;  /* 01B794 CLM */
    A &= 0x00FF;  /* 01B795 AND */
    A += 1;  /* 01B798 INC */
    X = A;  /* 01B799 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B79A LDA */
    mflag = 1;  /* 01B79D SEM */
    /* 01B79E CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B7D6;  /* 01B7A0 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B7A2 LDA */
    SETLO(A, A + 1);  /* 01B7A4 INC */
    SETLO(A, A + 1);  /* 01B7A5 INC */
    wr8(DP(0x00), (uint8_t)A);  /* 01B7A6 STA */
    mflag = 0;  /* 01B7A8 CLM */
    A = D;  /* 01B7A9 TDA */
    B = 0x0000;  /* 01B7AA LDA */
    Y = A;  /* 01B7AE TAY */
    S -= 3; F_01AB3B();  /* 01B7AF JSRL */
    /* 01B7B3 CMP */
    if ((A & 0xFF) != 0x00) goto L_01B7C6;  /* 01B7B5 BNE */
    mflag = 0;  /* 01B7B7 CLM */
    A = D;  /* 01B7B8 TDA */
    B = 0x0000;  /* 01B7B9 LDA */
    Y = A;  /* 01B7BD TAY */
    S -= 3; F_01AD76();  /* 01B7BE JSRL */
    /* 01B7C2 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B7D3;  /* 01B7C4 BEQ */
L_01B7C6: TICK();
    wr8(ABS(0x0434), 0x00);  /* 01B7C6 LDM */
    wr8(ABS(0x01BD), 0x58);  /* 01B7CA LDM */
    SETLO(A, 0x01);  /* 01B7CE LDA */
    Y = pull16();  /* 01B7D0 PLY */
    D = pull16();  /* 01B7D1 PLD */
    S += 3; return;  /* 01B7D2 RTL */
L_01B7D3: TICK();
    goto L_01B897;  /* 01B7D3 JMP */
L_01B7D6: TICK();
    mflag = 0;  /* 01B7D6 CLM */
    A = rd16(DP(0x00));  /* 01B7D7 LDA */
    A &= 0x00FF;  /* 01B7D9 AND */
    A += 1;  /* 01B7DC INC */
    A += 1;  /* 01B7DD INC */
    X = A;  /* 01B7DE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B7DF LDA */
    mflag = 1;  /* 01B7E2 SEM */
    /* 01B7E3 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B82A;  /* 01B7E5 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B7E7 LDA */
    mflag = 0;  /* 01B7E9 CLM */
    A &= 0x00FF;  /* 01B7EA AND */
    A += 1;  /* 01B7ED INC */
    X = A;  /* 01B7EE TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B7EF LDA */
    C = 0;  /* 01B7F2 CLC */
    mflag = 1;  /* 01B7F3 SEM */
    SETLO(A, adc8(A, 0xD0));  /* 01B7F4 ADC */
    wr8(ABS(0x0434), (uint8_t)A);  /* 01B7F6 STA */
    SETLO(A, rd8(DP(0x00)));  /* 01B7F9 LDA */
    SETLO(A, A + 1);  /* 01B7FB INC */
    SETLO(A, A + 1);  /* 01B7FC INC */
    SETLO(A, A + 1);  /* 01B7FD INC */
    wr8(DP(0x00), (uint8_t)A);  /* 01B7FE STA */
    mflag = 0;  /* 01B800 CLM */
    A = D;  /* 01B801 TDA */
    B = 0x0000;  /* 01B802 LDA */
    Y = A;  /* 01B806 TAY */
    S -= 3; F_01AB3B();  /* 01B807 JSRL */
    /* 01B80B CMP */
    if ((A & 0xFF) != 0x00) goto L_01B81E;  /* 01B80D BNE */
    mflag = 0;  /* 01B80F CLM */
    A = D;  /* 01B810 TDA */
    B = 0x0000;  /* 01B811 LDA */
    Y = A;  /* 01B815 TAY */
    S -= 3; F_01AD76();  /* 01B816 JSRL */
    /* 01B81A CMP */
    if ((A & 0xFF) == 0x00) goto L_01B827;  /* 01B81C BEQ */
L_01B81E: TICK();
    wr8(ABS(0x01BD), 0x58);  /* 01B81E LDM */
    SETLO(A, 0x01);  /* 01B822 LDA */
    Y = pull16();  /* 01B824 PLY */
    D = pull16();  /* 01B825 PLD */
    S += 3; return;  /* 01B826 RTL */
L_01B827: TICK();
    goto L_01B892;  /* 01B827 JMP */
L_01B82A: TICK();
    mflag = 0;  /* 01B82A CLM */
    A = rd16(DP(0x00));  /* 01B82B LDA */
    A &= 0x00FF;  /* 01B82D AND */
    A += 1;  /* 01B830 INC */
    A += 1;  /* 01B831 INC */
    A += 1;  /* 01B832 INC */
    X = A;  /* 01B833 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B834 LDA */
    mflag = 1;  /* 01B837 SEM */
    /* 01B838 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B892;  /* 01B83A BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B83C LDA */
    mflag = 0;  /* 01B83E CLM */
    A &= 0x00FF;  /* 01B83F AND */
    A += 1;  /* 01B842 INC */
    A += 1;  /* 01B843 INC */
    X = A;  /* 01B844 TAX */
    A = rd16(DP(0x00));  /* 01B845 LDA */
    A &= 0x00FF;  /* 01B847 AND */
    A += 1;  /* 01B84A INC */
    Y = A;  /* 01B84B TAY */
    A = rd16((ABS(0x0283) + Y));  /* 01B84C LDA */
    mflag = 1;  /* 01B84F SEM */
    mpy8(0x0A);  /* 01B850 MPY */
    C = 0;  /* 01B853 CLC */
    SETLO(A, adc8(A, rd8((ABS(0x0283) + X))));  /* 01B854 ADC */
    C = 0;  /* 01B857 CLC */
    SETLO(A, adc8(A, 0xF0));  /* 01B858 ADC */
    wr8(ABS(0x0434), (uint8_t)A);  /* 01B85A STA */
    SETLO(A, rd8(DP(0x00)));  /* 01B85D LDA */
    C = 0;  /* 01B85F CLC */
    SETLO(A, adc8(A, 0x04));  /* 01B860 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 01B862 STA */
    mflag = 0;  /* 01B864 CLM */
    A = D;  /* 01B865 TDA */
    B = 0x0000;  /* 01B866 LDA */
    Y = A;  /* 01B86A TAY */
    S -= 3; F_01AB3B();  /* 01B86B JSRL */
    /* 01B86F CMP */
    if ((A & 0xFF) != 0x00) goto L_01B882;  /* 01B871 BNE */
    mflag = 0;  /* 01B873 CLM */
    A = D;  /* 01B874 TDA */
    B = 0x0000;  /* 01B875 LDA */
    Y = A;  /* 01B879 TAY */
    S -= 3; F_01AD76();  /* 01B87A JSRL */
    /* 01B87E CMP */
    if ((A & 0xFF) == 0x00) goto L_01B892;  /* 01B880 BEQ */
L_01B882: TICK();
    SETLO(A, rd8(ABS(0x0434)));  /* 01B882 LDA */
    /* 01B885 CMP */
    if ((A & 0xFF) >= 0x20) goto L_01B892;  /* 01B887 BCS */
    wr8(ABS(0x01BD), 0x58);  /* 01B889 LDM */
    SETLO(A, 0x01);  /* 01B88D LDA */
    Y = pull16();  /* 01B88F PLY */
    D = pull16();  /* 01B890 PLD */
    S += 3; return;  /* 01B891 RTL */
L_01B892: TICK();
    SETLO(A, 0x00);  /* 01B892 LDA */
    Y = pull16();  /* 01B894 PLY */
    D = pull16();  /* 01B895 PLD */
    S += 3; return;  /* 01B896 RTL */
L_01B897: TICK();
    goto L_01B998;  /* 01B897 JMP */
L_01B89A: TICK();
    mflag = 0;  /* 01B89A CLM */
    A = rd16(DP(0x00));  /* 01B89B LDA */
    A &= 0x00FF;  /* 01B89D AND */
    X = A;  /* 01B8A0 TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B8A1 LDA */
    mflag = 1;  /* 01B8A4 SEM */
    /* 01B8A5 CMP */
    if ((A & 0xFF) != 0x39) goto L_01B8E7;  /* 01B8A7 BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 01B8A9 LDA */
    mflag = 0;  /* 01B8AC CLM */
    A &= 0x00FF;  /* 01B8AD AND */
    wr16(DP(0x01), A);  /* 01B8B0 STA */
    A = rd16(DP(0x00));  /* 01B8B2 LDA */
    A &= 0x00FF;  /* 01B8B4 AND */
    A += 1;  /* 01B8B7 INC */
    t = rd16(DP(0x01));  /* 01B8B8 CMP */
    if (A != t) goto L_01B8E7;  /* 01B8BA BNE */
    Y = 0x01D0;  /* 01B8BC LDY */
    S -= 3; eeprom_read_byte_007189();  /* 01B8BF JSRL */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 01B8C3 STA */
    Y = 0x01D1;  /* 01B8C6 LDY */
    mflag = 0;  /* 01B8C9 CLM */
    S -= 3; F_007271();  /* 01B8CA JSRL */
    wr16(ABS(0x01CD), A);  /* 01B8CE STA */
    if ((rd16(ABS(0x039E)) & 0x1000) == 0) goto L_01B8E4;  /* 01B8D1 BBC */
    X = A;  /* 01B8D7 TAX */
    if (X == 0) goto L_01B8E4;  /* 01B8D8 BEQ */
    mflag = 1;  /* 01B8DA SEM */
    wr8(ABS(0x01BD), 0x50);  /* 01B8DB LDM */
    SETLO(A, 0x01);  /* 01B8DF LDA */
    Y = pull16();  /* 01B8E1 PLY */
    D = pull16();  /* 01B8E2 PLD */
    S += 3; return;  /* 01B8E3 RTL */
L_01B8E4: TICK();
    goto L_01B998;  /* 01B8E4 JMP */
L_01B8E7: TICK();
    mflag = 0;  /* 01B8E7 CLM */
    A = rd16(DP(0x00));  /* 01B8E8 LDA */
    A &= 0x00FF;  /* 01B8EA AND */
    X = A;  /* 01B8ED TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B8EE LDA */
    mflag = 1;  /* 01B8F1 SEM */
    /* 01B8F2 CMP */
    if ((A & 0xFF) != 0x39) goto L_01B934;  /* 01B8F4 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B8F6 LDA */
    mflag = 0;  /* 01B8F8 CLM */
    A &= 0x00FF;  /* 01B8F9 AND */
    A += 1;  /* 01B8FC INC */
    X = A;  /* 01B8FD TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B8FE LDA */
    mflag = 1;  /* 01B901 SEM */
    /* 01B902 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B934;  /* 01B904 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B906 LDA */
    SETLO(A, A + 1);  /* 01B908 INC */
    SETLO(A, A + 1);  /* 01B909 INC */
    wr8(DP(0x00), (uint8_t)A);  /* 01B90A STA */
    mflag = 0;  /* 01B90C CLM */
    A = D;  /* 01B90D TDA */
    B = 0x0000;  /* 01B90E LDA */
    Y = A;  /* 01B912 TAY */
    S -= 3; F_01B696();  /* 01B913 JSRL */
    /* 01B917 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B931;  /* 01B919 BEQ */
    mflag = 0;  /* 01B91B CLM */
    if ((rd16(ABS(0x039E)) & 0x1000) == 0) goto L_01B931;  /* 01B91C BBC */
    X = rd16(ABS(0x01CD));  /* 01B922 LDX */
    if (X == 0) goto L_01B931;  /* 01B925 BEQ */
    mflag = 1;  /* 01B927 SEM */
    wr8(ABS(0x01BD), 0x50);  /* 01B928 LDM */
    SETLO(A, 0x01);  /* 01B92C LDA */
    Y = pull16();  /* 01B92E PLY */
    D = pull16();  /* 01B92F PLD */
    S += 3; return;  /* 01B930 RTL */
L_01B931: TICK();
    goto L_01B998;  /* 01B931 JMP */
L_01B934: TICK();
    mflag = 0;  /* 01B934 CLM */
    A = rd16(DP(0x00));  /* 01B935 LDA */
    A &= 0x00FF;  /* 01B937 AND */
    X = A;  /* 01B93A TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B93B LDA */
    mflag = 1;  /* 01B93E SEM */
    /* 01B93F CMP */
    if ((A & 0xFF) != 0x38) goto L_01B980;  /* 01B941 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B943 LDA */
    mflag = 0;  /* 01B945 CLM */
    A &= 0x00FF;  /* 01B946 AND */
    A += 1;  /* 01B949 INC */
    X = A;  /* 01B94A TAX */
    A = rd16((ABS(0x0283) + X));  /* 01B94B LDA */
    mflag = 1;  /* 01B94E SEM */
    /* 01B94F CMP */
    if ((A & 0xFF) != 0x2A) goto L_01B980;  /* 01B951 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01B953 LDA */
    SETLO(A, A + 1);  /* 01B955 INC */
    SETLO(A, A + 1);  /* 01B956 INC */
    wr8(DP(0x00), (uint8_t)A);  /* 01B957 STA */
    mflag = 0;  /* 01B959 CLM */
    A = D;  /* 01B95A TDA */
    B = 0x0000;  /* 01B95B LDA */
    Y = A;  /* 01B95F TAY */
    S -= 3; F_01B696();  /* 01B960 JSRL */
    /* 01B964 CMP */
    if ((A & 0xFF) == 0x00) goto L_01B97D;  /* 01B966 BEQ */
    mflag = 0;  /* 01B968 CLM */
    if ((rd16(ABS(0x039E)) & 0x0800) == 0) goto L_01B97D;  /* 01B969 BBC */
    mflag = 1;  /* 01B96F SEM */
    wr8(ABS(0x01E3), 0x00);  /* 01B970 LDM */
    wr8(ABS(0x01BD), 0x40);  /* 01B974 LDM */
    SETLO(A, 0x01);  /* 01B978 LDA */
    Y = pull16();  /* 01B97A PLY */
    D = pull16();  /* 01B97B PLD */
    S += 3; return;  /* 01B97C RTL */
L_01B97D: TICK();
    goto L_01B998;  /* 01B97D JMP */
L_01B980: TICK();
    mflag = 0;  /* 01B980 CLM */
    A = D;  /* 01B981 TDA */
    B = 0x0000;  /* 01B982 LDA */
    Y = A;  /* 01B986 TAY */
    S -= 3; F_01B541();  /* 01B987 JSRL */
    /* 01B98B CMP */
    if ((A & 0xFF) == 0x00) goto L_01B998;  /* 01B98D BEQ */
    wr8(ABS(0x01BD), 0x40);  /* 01B98F LDM */
    SETLO(A, 0x01);  /* 01B993 LDA */
    Y = pull16();  /* 01B995 PLY */
    D = pull16();  /* 01B996 PLD */
    S += 3; return;  /* 01B997 RTL */
L_01B998: TICK();
    mflag = 0;  /* 01B998 CLM */
    A = 0x0000;  /* 01B999 LDA */
    mflag = 1;  /* 01B99C SEM */
    Y = pull16();  /* 01B99D PLY */
    D = pull16();  /* 01B99E PLD */
    S += 3; return;  /* 01B99F RTL */
}

/* 01BBE4  callers: 01C8D6 */
void F_01BBE4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01BBE4 LDA */
    push16(D);  /* 01BBE6 PHD */
    D = A;  /* 01BBE7 TAD */
    wr16(DP(0x00), Y);  /* 01BBE8 STY */
    if ((rd16(ABS(0x023E)) & 0x0100) == 0) goto L_01BBF3;  /* 01BBEA BBC */
    goto L_01C624;  /* 01BBF0 JMP */
L_01BBF3: TICK();
    A = rd16(ABS(0x0270));  /* 01BBF3 LDA */
    mflag = 1;  /* 01BBF6 SEM */
    wr8(ABS(0x0271), (uint8_t)A);  /* 01BBF7 STA */
    SETLO(A, rd8(ABS(0x026F)));  /* 01BBFA LDA */
    wr8(ABS(0x0270), (uint8_t)A);  /* 01BBFD STA */
    SETLO(A, rd8(ABS(0x026E)));  /* 01BC00 LDA */
    wr8(ABS(0x026F), (uint8_t)A);  /* 01BC03 STA */
    SETLO(A, rd8(ABS(0x026D)));  /* 01BC06 LDA */
    wr8(ABS(0x026E), (uint8_t)A);  /* 01BC09 STA */
    wr8(DP(0x02), 0x0F);  /* 01BC0C LDM */
    mflag = 0;  /* 01BC0F CLM */
    X = Y;  /* 01BC10 TYX */
    if (X == 0) goto L_01BC16;  /* 01BC11 BEQ */
    goto L_01BFC7;  /* 01BC13 JMP */
L_01BC16: TICK();
    mflag = 1;  /* 01BC16 SEM */
    wr8(ABS(0x039C), 0x00);  /* 01BC17 LDM */
    mflag = 0;  /* 01BC1B CLM */
    X = rd16(ABS(0x0272));  /* 01BC1C LDX */
    wr16(DP(0x04), X);  /* 01BC1F STX */
    A = rd16(ABS(0x0340));  /* 01BC21 LDA */
    mflag = 1;  /* 01BC24 SEM */
    SETLO(A, A & 0x03);  /* 01BC25 AND */
    /* 01BC27 CMP */
    if ((A & 0xFF) == 0x02) goto L_01BC2E;  /* 01BC29 BEQ */
    goto L_01BD5B;  /* 01BC2B JMP */
L_01BC2E: TICK();
    mflag = 0;  /* 01BC2E CLM */
    /* 01BC2F CPX */
    if (X < 0x0151) goto L_01BC37;  /* 01BC32 BCC */
    goto L_01BCCF;  /* 01BC34 JMP */
L_01BC37: TICK();
    /* 01BC37 CPX */
    if (X >= 0x00EA) goto L_01BC84;  /* 01BC3A BCS */
    /* 01BC3C CPX */
    if (X >= 0x00C6) goto L_01BC61;  /* 01BC3F BCS */
    /* 01BC41 CPX */
    if (X >= 0x00B8) goto L_01BC52;  /* 01BC44 BCS */
    /* 01BC46 CPX */
    if (X < 0x00B1) goto L_01BC4F;  /* 01BC49 BCC */
    mflag = 1;  /* 01BC4B SEM */
    wr8(DP(0x02), 0x0A);  /* 01BC4C LDM */
L_01BC4F: TICK();
    goto L_01BC5E;  /* 01BC4F JMP */
L_01BC52: TICK();
    mflag = 0;  /* 01BC52 CLM */
    X = rd16(DP(0x04));  /* 01BC53 LDX */
    /* 01BC55 CPX */
    if (X < 0x00BF) goto L_01BC5E;  /* 01BC58 BCC */
    mflag = 1;  /* 01BC5A SEM */
    wr8(DP(0x02), 0x0E);  /* 01BC5B LDM */
L_01BC5E: TICK();
    goto L_01BC81;  /* 01BC5E JMP */
L_01BC61: TICK();
    mflag = 0;  /* 01BC61 CLM */
    X = rd16(DP(0x04));  /* 01BC62 LDX */
    /* 01BC64 CPX */
    if (X >= 0x00D7) goto L_01BC75;  /* 01BC67 BCS */
    /* 01BC69 CPX */
    if (X < 0x00CE) goto L_01BC72;  /* 01BC6C BCC */
    mflag = 1;  /* 01BC6E SEM */
    wr8(DP(0x02), 0x00);  /* 01BC6F LDM */
L_01BC72: TICK();
    goto L_01BC81;  /* 01BC72 JMP */
L_01BC75: TICK();
    mflag = 0;  /* 01BC75 CLM */
    X = rd16(DP(0x04));  /* 01BC76 LDX */
    /* 01BC78 CPX */
    if (X < 0x00E1) goto L_01BC81;  /* 01BC7B BCC */
    mflag = 1;  /* 01BC7D SEM */
    wr8(DP(0x02), 0x09);  /* 01BC7E LDM */
L_01BC81: TICK();
    goto L_01BCCC;  /* 01BC81 JMP */
L_01BC84: TICK();
    mflag = 0;  /* 01BC84 CLM */
    X = rd16(DP(0x04));  /* 01BC85 LDX */
    /* 01BC87 CPX */
    if (X >= 0x011A) goto L_01BCAC;  /* 01BC8A BCS */
    /* 01BC8C CPX */
    if (X >= 0x0102) goto L_01BC9D;  /* 01BC8F BCS */
    /* 01BC91 CPX */
    if (X < 0x00F7) goto L_01BC9A;  /* 01BC94 BCC */
    mflag = 1;  /* 01BC96 SEM */
    wr8(DP(0x02), 0x08);  /* 01BC97 LDM */
L_01BC9A: TICK();
    goto L_01BCA9;  /* 01BC9A JMP */
L_01BC9D: TICK();
    mflag = 0;  /* 01BC9D CLM */
    X = rd16(DP(0x04));  /* 01BC9E LDX */
    /* 01BCA0 CPX */
    if (X < 0x010E) goto L_01BCA9;  /* 01BCA3 BCC */
    mflag = 1;  /* 01BCA5 SEM */
    wr8(DP(0x02), 0x07);  /* 01BCA6 LDM */
L_01BCA9: TICK();
    goto L_01BCCC;  /* 01BCA9 JMP */
L_01BCAC: TICK();
    mflag = 0;  /* 01BCAC CLM */
    X = rd16(DP(0x04));  /* 01BCAD LDX */
    /* 01BCAF CPX */
    if (X >= 0x0135) goto L_01BCC0;  /* 01BCB2 BCS */
    /* 01BCB4 CPX */
    if (X < 0x0128) goto L_01BCBD;  /* 01BCB7 BCC */
    mflag = 1;  /* 01BCB9 SEM */
    wr8(DP(0x02), 0x06);  /* 01BCBA LDM */
L_01BCBD: TICK();
    goto L_01BCCC;  /* 01BCBD JMP */
L_01BCC0: TICK();
    mflag = 0;  /* 01BCC0 CLM */
    X = rd16(DP(0x04));  /* 01BCC1 LDX */
    /* 01BCC3 CPX */
    if (X < 0x0143) goto L_01BCCC;  /* 01BCC6 BCC */
    mflag = 1;  /* 01BCC8 SEM */
    wr8(DP(0x02), 0x05);  /* 01BCC9 LDM */
L_01BCCC: TICK();
    goto L_01BD58;  /* 01BCCC JMP */
L_01BCCF: TICK();
    mflag = 0;  /* 01BCCF CLM */
    X = rd16(DP(0x04));  /* 01BCD0 LDX */
    /* 01BCD2 CPX */
    if (X >= 0x01E7) goto L_01BD1F;  /* 01BCD5 BCS */
    /* 01BCD7 CPX */
    if (X >= 0x0196) goto L_01BCFC;  /* 01BCDA BCS */
    /* 01BCDC CPX */
    if (X >= 0x0171) goto L_01BCED;  /* 01BCDF BCS */
    /* 01BCE1 CPX */
    if (X < 0x0161) goto L_01BCEA;  /* 01BCE4 BCC */
    mflag = 1;  /* 01BCE6 SEM */
    wr8(DP(0x02), 0x04);  /* 01BCE7 LDM */
L_01BCEA: TICK();
    goto L_01BCF9;  /* 01BCEA JMP */
L_01BCED: TICK();
    mflag = 0;  /* 01BCED CLM */
    X = rd16(DP(0x04));  /* 01BCEE LDX */
    /* 01BCF0 CPX */
    if (X < 0x0185) goto L_01BCF9;  /* 01BCF3 BCC */
    mflag = 1;  /* 01BCF5 SEM */
    wr8(DP(0x02), 0x03);  /* 01BCF6 LDM */
L_01BCF9: TICK();
    goto L_01BD1C;  /* 01BCF9 JMP */
L_01BCFC: TICK();
    mflag = 0;  /* 01BCFC CLM */
    X = rd16(DP(0x04));  /* 01BCFD LDX */
    /* 01BCFF CPX */
    if (X >= 0x01BD) goto L_01BD10;  /* 01BD02 BCS */
    /* 01BD04 CPX */
    if (X < 0x01A9) goto L_01BD0D;  /* 01BD07 BCC */
    mflag = 1;  /* 01BD09 SEM */
    wr8(DP(0x02), 0x02);  /* 01BD0A LDM */
L_01BD0D: TICK();
    goto L_01BD1C;  /* 01BD0D JMP */
L_01BD10: TICK();
    mflag = 0;  /* 01BD10 CLM */
    X = rd16(DP(0x04));  /* 01BD11 LDX */
    /* 01BD13 CPX */
    if (X < 0x01D1) goto L_01BD1C;  /* 01BD16 BCC */
    mflag = 1;  /* 01BD18 SEM */
    wr8(DP(0x02), 0x01);  /* 01BD19 LDM */
L_01BD1C: TICK();
    goto L_01BD58;  /* 01BD1C JMP */
L_01BD1F: TICK();
    mflag = 0;  /* 01BD1F CLM */
    X = rd16(DP(0x04));  /* 01BD20 LDX */
    /* 01BD22 CPX */
    if (X >= 0x0247) goto L_01BD47;  /* 01BD25 BCS */
    /* 01BD27 CPX */
    if (X >= 0x0214) goto L_01BD38;  /* 01BD2A BCS */
    /* 01BD2C CPX */
    if (X < 0x01FC) goto L_01BD35;  /* 01BD2F BCC */
    mflag = 1;  /* 01BD31 SEM */
    wr8(DP(0x02), 0x0C);  /* 01BD32 LDM */
L_01BD35: TICK();
    goto L_01BD44;  /* 01BD35 JMP */
L_01BD38: TICK();
    mflag = 0;  /* 01BD38 CLM */
    X = rd16(DP(0x04));  /* 01BD39 LDX */
    /* 01BD3B CPX */
    if (X < 0x022D) goto L_01BD44;  /* 01BD3E BCC */
    mflag = 1;  /* 01BD40 SEM */
    wr8(DP(0x02), 0x0D);  /* 01BD41 LDM */
L_01BD44: TICK();
    goto L_01BD58;  /* 01BD44 JMP */
L_01BD47: TICK();
    mflag = 0;  /* 01BD47 CLM */
    X = rd16(DP(0x04));  /* 01BD48 LDX */
    /* 01BD4A CPX */
    if (X >= 0x027D) goto L_01BD58;  /* 01BD4D BCS */
    /* 01BD4F CPX */
    if (X < 0x0261) goto L_01BD58;  /* 01BD52 BCC */
    mflag = 1;  /* 01BD54 SEM */
    wr8(DP(0x02), 0x0B);  /* 01BD55 LDM */
L_01BD58: TICK();
    goto L_01BFC4;  /* 01BD58 JMP */
L_01BD5B: TICK();
    mflag = 0;  /* 01BD5B CLM */
    A = rd16(ABS(0x0340));  /* 01BD5C LDA */
    mflag = 1;  /* 01BD5F SEM */
    SETLO(A, A & 0x03);  /* 01BD60 AND */
    /* 01BD62 CMP */
    if ((A & 0xFF) == 0x03) goto L_01BD69;  /* 01BD64 BEQ */
    goto L_01BE98;  /* 01BD66 JMP */
L_01BD69: TICK();
    mflag = 0;  /* 01BD69 CLM */
    X = rd16(DP(0x04));  /* 01BD6A LDX */
    /* 01BD6C CPX */
    if (X < 0x0165) goto L_01BD74;  /* 01BD6F BCC */
    goto L_01BE0C;  /* 01BD71 JMP */
L_01BD74: TICK();
    /* 01BD74 CPX */
    if (X >= 0x0115) goto L_01BDC1;  /* 01BD77 BCS */
    /* 01BD79 CPX */
    if (X >= 0x00F4) goto L_01BD9E;  /* 01BD7C BCS */
    /* 01BD7E CPX */
    if (X >= 0x00E5) goto L_01BD8F;  /* 01BD81 BCS */
    /* 01BD83 CPX */
    if (X < 0x00DC) goto L_01BD8C;  /* 01BD86 BCC */
    mflag = 1;  /* 01BD88 SEM */
    wr8(DP(0x02), 0x0C);  /* 01BD89 LDM */
L_01BD8C: TICK();
    goto L_01BD9B;  /* 01BD8C JMP */
L_01BD8F: TICK();
    mflag = 0;  /* 01BD8F CLM */
    X = rd16(DP(0x04));  /* 01BD90 LDX */
    /* 01BD92 CPX */
    if (X < 0x00EB) goto L_01BD9B;  /* 01BD95 BCC */
    mflag = 1;  /* 01BD97 SEM */
    wr8(DP(0x02), 0x0E);  /* 01BD98 LDM */
L_01BD9B: TICK();
    goto L_01BDBE;  /* 01BD9B JMP */
L_01BD9E: TICK();
    mflag = 0;  /* 01BD9E CLM */
    X = rd16(DP(0x04));  /* 01BD9F LDX */
    /* 01BDA1 CPX */
    if (X >= 0x0104) goto L_01BDB2;  /* 01BDA4 BCS */
    /* 01BDA6 CPX */
    if (X < 0x00FA) goto L_01BDAF;  /* 01BDA9 BCC */
    mflag = 1;  /* 01BDAB SEM */
    wr8(DP(0x02), 0x00);  /* 01BDAC LDM */
L_01BDAF: TICK();
    goto L_01BDBE;  /* 01BDAF JMP */
L_01BDB2: TICK();
    mflag = 0;  /* 01BDB2 CLM */
    X = rd16(DP(0x04));  /* 01BDB3 LDX */
    /* 01BDB5 CPX */
    if (X < 0x010A) goto L_01BDBE;  /* 01BDB8 BCC */
    mflag = 1;  /* 01BDBA SEM */
    wr8(DP(0x02), 0x09);  /* 01BDBB LDM */
L_01BDBE: TICK();
    goto L_01BE09;  /* 01BDBE JMP */
L_01BDC1: TICK();
    mflag = 0;  /* 01BDC1 CLM */
    X = rd16(DP(0x04));  /* 01BDC2 LDX */
    /* 01BDC4 CPX */
    if (X >= 0x013B) goto L_01BDE9;  /* 01BDC7 BCS */
    /* 01BDC9 CPX */
    if (X >= 0x0127) goto L_01BDDA;  /* 01BDCC BCS */
    /* 01BDCE CPX */
    if (X < 0x011B) goto L_01BDD7;  /* 01BDD1 BCC */
    mflag = 1;  /* 01BDD3 SEM */
    wr8(DP(0x02), 0x08);  /* 01BDD4 LDM */
L_01BDD7: TICK();
    goto L_01BDE6;  /* 01BDD7 JMP */
L_01BDDA: TICK();
    mflag = 0;  /* 01BDDA CLM */
    X = rd16(DP(0x04));  /* 01BDDB LDX */
    /* 01BDDD CPX */
    if (X < 0x012D) goto L_01BDE6;  /* 01BDE0 BCC */
    mflag = 1;  /* 01BDE2 SEM */
    wr8(DP(0x02), 0x07);  /* 01BDE3 LDM */
L_01BDE6: TICK();
    goto L_01BE09;  /* 01BDE6 JMP */
L_01BDE9: TICK();
    mflag = 0;  /* 01BDE9 CLM */
    X = rd16(DP(0x04));  /* 01BDEA LDX */
    /* 01BDEC CPX */
    if (X >= 0x014F) goto L_01BDFD;  /* 01BDEF BCS */
    /* 01BDF1 CPX */
    if (X < 0x0141) goto L_01BDFA;  /* 01BDF4 BCC */
    mflag = 1;  /* 01BDF6 SEM */
    wr8(DP(0x02), 0x06);  /* 01BDF7 LDM */
L_01BDFA: TICK();
    goto L_01BE09;  /* 01BDFA JMP */
L_01BDFD: TICK();
    mflag = 0;  /* 01BDFD CLM */
    X = rd16(DP(0x04));  /* 01BDFE LDX */
    /* 01BE00 CPX */
    if (X < 0x0156) goto L_01BE09;  /* 01BE03 BCC */
    mflag = 1;  /* 01BE05 SEM */
    wr8(DP(0x02), 0x05);  /* 01BE06 LDM */
L_01BE09: TICK();
    goto L_01BE95;  /* 01BE09 JMP */
L_01BE0C: TICK();
    mflag = 0;  /* 01BE0C CLM */
    X = rd16(DP(0x04));  /* 01BE0D LDX */
    /* 01BE0F CPX */
    if (X >= 0x01CB) goto L_01BE5C;  /* 01BE12 BCS */
    /* 01BE14 CPX */
    if (X >= 0x0195) goto L_01BE39;  /* 01BE17 BCS */
    /* 01BE19 CPX */
    if (X >= 0x017C) goto L_01BE2A;  /* 01BE1C BCS */
    /* 01BE1E CPX */
    if (X < 0x016C) goto L_01BE27;  /* 01BE21 BCC */
    mflag = 1;  /* 01BE23 SEM */
    wr8(DP(0x02), 0x04);  /* 01BE24 LDM */
L_01BE27: TICK();
    goto L_01BE36;  /* 01BE27 JMP */
L_01BE2A: TICK();
    mflag = 0;  /* 01BE2A CLM */
    X = rd16(DP(0x04));  /* 01BE2B LDX */
    /* 01BE2D CPX */
    if (X < 0x0183) goto L_01BE36;  /* 01BE30 BCC */
    mflag = 1;  /* 01BE32 SEM */
    wr8(DP(0x02), 0x03);  /* 01BE33 LDM */
L_01BE36: TICK();
    goto L_01BE59;  /* 01BE36 JMP */
L_01BE39: TICK();
    mflag = 0;  /* 01BE39 CLM */
    X = rd16(DP(0x04));  /* 01BE3A LDX */
    /* 01BE3C CPX */
    if (X >= 0x01AF) goto L_01BE4D;  /* 01BE3F BCS */
    /* 01BE41 CPX */
    if (X < 0x019C) goto L_01BE4A;  /* 01BE44 BCC */
    mflag = 1;  /* 01BE46 SEM */
    wr8(DP(0x02), 0x02);  /* 01BE47 LDM */
L_01BE4A: TICK();
    goto L_01BE59;  /* 01BE4A JMP */
L_01BE4D: TICK();
    mflag = 0;  /* 01BE4D CLM */
    X = rd16(DP(0x04));  /* 01BE4E LDX */
    /* 01BE50 CPX */
    if (X < 0x01B7) goto L_01BE59;  /* 01BE53 BCC */
    mflag = 1;  /* 01BE55 SEM */
    wr8(DP(0x02), 0x01);  /* 01BE56 LDM */
L_01BE59: TICK();
    goto L_01BE95;  /* 01BE59 JMP */
L_01BE5C: TICK();
    mflag = 0;  /* 01BE5C CLM */
    X = rd16(DP(0x04));  /* 01BE5D LDX */
    /* 01BE5F CPX */
    if (X >= 0x0209) goto L_01BE84;  /* 01BE62 BCS */
    /* 01BE64 CPX */
    if (X >= 0x01E9) goto L_01BE75;  /* 01BE67 BCS */
    /* 01BE69 CPX */
    if (X < 0x01D4) goto L_01BE72;  /* 01BE6C BCC */
    mflag = 1;  /* 01BE6E SEM */
    wr8(DP(0x02), 0x0A);  /* 01BE6F LDM */
L_01BE72: TICK();
    goto L_01BE81;  /* 01BE72 JMP */
L_01BE75: TICK();
    mflag = 0;  /* 01BE75 CLM */
    X = rd16(DP(0x04));  /* 01BE76 LDX */
    /* 01BE78 CPX */
    if (X < 0x01F2) goto L_01BE81;  /* 01BE7B BCC */
    mflag = 1;  /* 01BE7D SEM */
    wr8(DP(0x02), 0x0D);  /* 01BE7E LDM */
L_01BE81: TICK();
    goto L_01BE95;  /* 01BE81 JMP */
L_01BE84: TICK();
    mflag = 0;  /* 01BE84 CLM */
    X = rd16(DP(0x04));  /* 01BE85 LDX */
    /* 01BE87 CPX */
    if (X >= 0x022B) goto L_01BE95;  /* 01BE8A BCS */
    /* 01BE8C CPX */
    if (X < 0x0212) goto L_01BE95;  /* 01BE8F BCC */
    mflag = 1;  /* 01BE91 SEM */
    wr8(DP(0x02), 0x0B);  /* 01BE92 LDM */
L_01BE95: TICK();
    goto L_01BFC4;  /* 01BE95 JMP */
L_01BE98: TICK();
    mflag = 0;  /* 01BE98 CLM */
    X = rd16(DP(0x04));  /* 01BE99 LDX */
    /* 01BE9B CPX */
    if (X < 0x014F) goto L_01BEA3;  /* 01BE9E BCC */
    goto L_01BF3B;  /* 01BEA0 JMP */
L_01BEA3: TICK();
    /* 01BEA3 CPX */
    if (X >= 0x0104) goto L_01BEF0;  /* 01BEA6 BCS */
    /* 01BEA8 CPX */
    if (X >= 0x00E5) goto L_01BECD;  /* 01BEAB BCS */
    /* 01BEAD CPX */
    if (X >= 0x00D7) goto L_01BEBE;  /* 01BEB0 BCS */
    /* 01BEB2 CPX */
    if (X < 0x00CE) goto L_01BEBB;  /* 01BEB5 BCC */
    mflag = 1;  /* 01BEB7 SEM */
    wr8(DP(0x02), 0x0A);  /* 01BEB8 LDM */
L_01BEBB: TICK();
    goto L_01BECA;  /* 01BEBB JMP */
L_01BEBE: TICK();
    mflag = 0;  /* 01BEBE CLM */
    X = rd16(DP(0x04));  /* 01BEBF LDX */
    /* 01BEC1 CPX */
    if (X < 0x00DC) goto L_01BECA;  /* 01BEC4 BCC */
    mflag = 1;  /* 01BEC6 SEM */
    wr8(DP(0x02), 0x0C);  /* 01BEC7 LDM */
L_01BECA: TICK();
    goto L_01BEED;  /* 01BECA JMP */
L_01BECD: TICK();
    mflag = 0;  /* 01BECD CLM */
    X = rd16(DP(0x04));  /* 01BECE LDX */
    /* 01BED0 CPX */
    if (X >= 0x00F4) goto L_01BEE1;  /* 01BED3 BCS */
    /* 01BED5 CPX */
    if (X < 0x00EB) goto L_01BEDE;  /* 01BED8 BCC */
    mflag = 1;  /* 01BEDA SEM */
    wr8(DP(0x02), 0x0E);  /* 01BEDB LDM */
L_01BEDE: TICK();
    goto L_01BEED;  /* 01BEDE JMP */
L_01BEE1: TICK();
    mflag = 0;  /* 01BEE1 CLM */
    X = rd16(DP(0x04));  /* 01BEE2 LDX */
    /* 01BEE4 CPX */
    if (X < 0x00FA) goto L_01BEED;  /* 01BEE7 BCC */
    mflag = 1;  /* 01BEE9 SEM */
    wr8(DP(0x02), 0x00);  /* 01BEEA LDM */
L_01BEED: TICK();
    goto L_01BF38;  /* 01BEED JMP */
L_01BEF0: TICK();
    mflag = 0;  /* 01BEF0 CLM */
    X = rd16(DP(0x04));  /* 01BEF1 LDX */
    /* 01BEF3 CPX */
    if (X >= 0x0127) goto L_01BF18;  /* 01BEF6 BCS */
    /* 01BEF8 CPX */
    if (X >= 0x0115) goto L_01BF09;  /* 01BEFB BCS */
    /* 01BEFD CPX */
    if (X < 0x010A) goto L_01BF06;  /* 01BF00 BCC */
    mflag = 1;  /* 01BF02 SEM */
    wr8(DP(0x02), 0x09);  /* 01BF03 LDM */
L_01BF06: TICK();
    goto L_01BF15;  /* 01BF06 JMP */
L_01BF09: TICK();
    mflag = 0;  /* 01BF09 CLM */
    X = rd16(DP(0x04));  /* 01BF0A LDX */
    /* 01BF0C CPX */
    if (X < 0x011B) goto L_01BF15;  /* 01BF0F BCC */
    mflag = 1;  /* 01BF11 SEM */
    wr8(DP(0x02), 0x08);  /* 01BF12 LDM */
L_01BF15: TICK();
    goto L_01BF38;  /* 01BF15 JMP */
L_01BF18: TICK();
    mflag = 0;  /* 01BF18 CLM */
    X = rd16(DP(0x04));  /* 01BF19 LDX */
    /* 01BF1B CPX */
    if (X >= 0x013B) goto L_01BF2C;  /* 01BF1E BCS */
    /* 01BF20 CPX */
    if (X < 0x012D) goto L_01BF29;  /* 01BF23 BCC */
    mflag = 1;  /* 01BF25 SEM */
    wr8(DP(0x02), 0x07);  /* 01BF26 LDM */
L_01BF29: TICK();
    goto L_01BF38;  /* 01BF29 JMP */
L_01BF2C: TICK();
    mflag = 0;  /* 01BF2C CLM */
    X = rd16(DP(0x04));  /* 01BF2D LDX */
    /* 01BF2F CPX */
    if (X < 0x0141) goto L_01BF38;  /* 01BF32 BCC */
    mflag = 1;  /* 01BF34 SEM */
    wr8(DP(0x02), 0x06);  /* 01BF35 LDM */
L_01BF38: TICK();
    goto L_01BFC4;  /* 01BF38 JMP */
L_01BF3B: TICK();
    mflag = 0;  /* 01BF3B CLM */
    X = rd16(DP(0x04));  /* 01BF3C LDX */
    /* 01BF3E CPX */
    if (X >= 0x01AF) goto L_01BF8B;  /* 01BF41 BCS */
    /* 01BF43 CPX */
    if (X >= 0x017C) goto L_01BF68;  /* 01BF46 BCS */
    /* 01BF48 CPX */
    if (X >= 0x0165) goto L_01BF59;  /* 01BF4B BCS */
    /* 01BF4D CPX */
    if (X < 0x0156) goto L_01BF56;  /* 01BF50 BCC */
    mflag = 1;  /* 01BF52 SEM */
    wr8(DP(0x02), 0x05);  /* 01BF53 LDM */
L_01BF56: TICK();
    goto L_01BF65;  /* 01BF56 JMP */
L_01BF59: TICK();
    mflag = 0;  /* 01BF59 CLM */
    X = rd16(DP(0x04));  /* 01BF5A LDX */
    /* 01BF5C CPX */
    if (X < 0x016C) goto L_01BF65;  /* 01BF5F BCC */
    mflag = 1;  /* 01BF61 SEM */
    wr8(DP(0x02), 0x04);  /* 01BF62 LDM */
L_01BF65: TICK();
    goto L_01BF88;  /* 01BF65 JMP */
L_01BF68: TICK();
    mflag = 0;  /* 01BF68 CLM */
    X = rd16(DP(0x04));  /* 01BF69 LDX */
    /* 01BF6B CPX */
    if (X >= 0x0195) goto L_01BF7C;  /* 01BF6E BCS */
    /* 01BF70 CPX */
    if (X < 0x0183) goto L_01BF79;  /* 01BF73 BCC */
    mflag = 1;  /* 01BF75 SEM */
    wr8(DP(0x02), 0x03);  /* 01BF76 LDM */
L_01BF79: TICK();
    goto L_01BF88;  /* 01BF79 JMP */
L_01BF7C: TICK();
    mflag = 0;  /* 01BF7C CLM */
    X = rd16(DP(0x04));  /* 01BF7D LDX */
    /* 01BF7F CPX */
    if (X < 0x019C) goto L_01BF88;  /* 01BF82 BCC */
    mflag = 1;  /* 01BF84 SEM */
    wr8(DP(0x02), 0x02);  /* 01BF85 LDM */
L_01BF88: TICK();
    goto L_01BFC4;  /* 01BF88 JMP */
L_01BF8B: TICK();
    mflag = 0;  /* 01BF8B CLM */
    X = rd16(DP(0x04));  /* 01BF8C LDX */
    /* 01BF8E CPX */
    if (X >= 0x0209) goto L_01BFB3;  /* 01BF91 BCS */
    /* 01BF93 CPX */
    if (X >= 0x01CB) goto L_01BFA4;  /* 01BF96 BCS */
    /* 01BF98 CPX */
    if (X < 0x01B7) goto L_01BFA1;  /* 01BF9B BCC */
    mflag = 1;  /* 01BF9D SEM */
    wr8(DP(0x02), 0x01);  /* 01BF9E LDM */
L_01BFA1: TICK();
    goto L_01BFB0;  /* 01BFA1 JMP */
L_01BFA4: TICK();
    mflag = 0;  /* 01BFA4 CLM */
    X = rd16(DP(0x04));  /* 01BFA5 LDX */
    /* 01BFA7 CPX */
    if (X < 0x01F2) goto L_01BFB0;  /* 01BFAA BCC */
    mflag = 1;  /* 01BFAC SEM */
    wr8(DP(0x02), 0x0D);  /* 01BFAD LDM */
L_01BFB0: TICK();
    goto L_01BFC4;  /* 01BFB0 JMP */
L_01BFB3: TICK();
    mflag = 0;  /* 01BFB3 CLM */
    X = rd16(DP(0x04));  /* 01BFB4 LDX */
    /* 01BFB6 CPX */
    if (X >= 0x022B) goto L_01BFC4;  /* 01BFB9 BCS */
    /* 01BFBB CPX */
    if (X < 0x0212) goto L_01BFC4;  /* 01BFBE BCC */
    mflag = 1;  /* 01BFC0 SEM */
    wr8(DP(0x02), 0x0B);  /* 01BFC1 LDM */
L_01BFC4: TICK();
    goto L_01C37D;  /* 01BFC4 JMP */
L_01BFC7: TICK();
    mflag = 1;  /* 01BFC7 SEM */
    t = (rd8(ABS(0x039C)) + 1) & 0xFF; wr8(ABS(0x039C), t);  /* 01BFC8 INC */
    SETLO(A, rd8(ABS(0x039C)));  /* 01BFCB LDA */
    /* 01BFCE CMP */
    if ((A & 0xFF) < 0x03) goto L_01BFD5;  /* 01BFD0 BCC */
    goto L_01C37D;  /* 01BFD2 JMP */
L_01BFD5: TICK();
    mflag = 0;  /* 01BFD5 CLM */
    X = rd16(DP(0x00));  /* 01BFD6 LDX */
    wr16(DP(0x04), X);  /* 01BFD8 STX */
    A = rd16(ABS(0x0340));  /* 01BFDA LDA */
    mflag = 1;  /* 01BFDD SEM */
    SETLO(A, A & 0x03);  /* 01BFDE AND */
    /* 01BFE0 CMP */
    if ((A & 0xFF) == 0x02) goto L_01BFE7;  /* 01BFE2 BEQ */
    goto L_01C114;  /* 01BFE4 JMP */
L_01BFE7: TICK();
    mflag = 0;  /* 01BFE7 CLM */
    /* 01BFE8 CPX */
    if (X < 0x014C) goto L_01BFF0;  /* 01BFEB BCC */
    goto L_01C088;  /* 01BFED JMP */
L_01BFF0: TICK();
    /* 01BFF0 CPX */
    if (X >= 0x00E7) goto L_01C03D;  /* 01BFF3 BCS */
    /* 01BFF5 CPX */
    if (X >= 0x00C3) goto L_01C01A;  /* 01BFF8 BCS */
    /* 01BFFA CPX */
    if (X >= 0x00B5) goto L_01C00B;  /* 01BFFD BCS */
    /* 01BFFF CPX */
    if (X < 0x00B4) goto L_01C008;  /* 01C002 BCC */
    mflag = 1;  /* 01C004 SEM */
    wr8(DP(0x02), 0x0A);  /* 01C005 LDM */
L_01C008: TICK();
    goto L_01C017;  /* 01C008 JMP */
L_01C00B: TICK();
    mflag = 0;  /* 01C00B CLM */
    X = rd16(DP(0x04));  /* 01C00C LDX */
    /* 01C00E CPX */
    if (X < 0x00C1) goto L_01C017;  /* 01C011 BCC */
    mflag = 1;  /* 01C013 SEM */
    wr8(DP(0x02), 0x0E);  /* 01C014 LDM */
L_01C017: TICK();
    goto L_01C03A;  /* 01C017 JMP */
L_01C01A: TICK();
    mflag = 0;  /* 01C01A CLM */
    X = rd16(DP(0x04));  /* 01C01B LDX */
    /* 01C01D CPX */
    if (X >= 0x00D4) goto L_01C02E;  /* 01C020 BCS */
    /* 01C022 CPX */
    if (X < 0x00D1) goto L_01C02B;  /* 01C025 BCC */
    mflag = 1;  /* 01C027 SEM */
    wr8(DP(0x02), 0x00);  /* 01C028 LDM */
L_01C02B: TICK();
    goto L_01C03A;  /* 01C02B JMP */
L_01C02E: TICK();
    mflag = 0;  /* 01C02E CLM */
    X = rd16(DP(0x04));  /* 01C02F LDX */
    /* 01C031 CPX */
    if (X < 0x00E4) goto L_01C03A;  /* 01C034 BCC */
    mflag = 1;  /* 01C036 SEM */
    wr8(DP(0x02), 0x09);  /* 01C037 LDM */
L_01C03A: TICK();
    goto L_01C085;  /* 01C03A JMP */
L_01C03D: TICK();
    mflag = 0;  /* 01C03D CLM */
    X = rd16(DP(0x04));  /* 01C03E LDX */
    /* 01C040 CPX */
    if (X >= 0x0116) goto L_01C065;  /* 01C043 BCS */
    /* 01C045 CPX */
    if (X >= 0x00FE) goto L_01C056;  /* 01C048 BCS */
    /* 01C04A CPX */
    if (X < 0x00FB) goto L_01C053;  /* 01C04D BCC */
    mflag = 1;  /* 01C04F SEM */
    wr8(DP(0x02), 0x08);  /* 01C050 LDM */
L_01C053: TICK();
    goto L_01C062;  /* 01C053 JMP */
L_01C056: TICK();
    mflag = 0;  /* 01C056 CLM */
    X = rd16(DP(0x04));  /* 01C057 LDX */
    /* 01C059 CPX */
    if (X < 0x0112) goto L_01C062;  /* 01C05C BCC */
    mflag = 1;  /* 01C05E SEM */
    wr8(DP(0x02), 0x07);  /* 01C05F LDM */
L_01C062: TICK();
    goto L_01C085;  /* 01C062 JMP */
L_01C065: TICK();
    mflag = 0;  /* 01C065 CLM */
    X = rd16(DP(0x04));  /* 01C066 LDX */
    /* 01C068 CPX */
    if (X >= 0x0130) goto L_01C079;  /* 01C06B BCS */
    /* 01C06D CPX */
    if (X < 0x012C) goto L_01C076;  /* 01C070 BCC */
    mflag = 1;  /* 01C072 SEM */
    wr8(DP(0x02), 0x06);  /* 01C073 LDM */
L_01C076: TICK();
    goto L_01C085;  /* 01C076 JMP */
L_01C079: TICK();
    mflag = 0;  /* 01C079 CLM */
    X = rd16(DP(0x04));  /* 01C07A LDX */
    /* 01C07C CPX */
    if (X < 0x0148) goto L_01C085;  /* 01C07F BCC */
    mflag = 1;  /* 01C081 SEM */
    wr8(DP(0x02), 0x05);  /* 01C082 LDM */
L_01C085: TICK();
    goto L_01C111;  /* 01C085 JMP */
L_01C088: TICK();
    mflag = 0;  /* 01C088 CLM */
    X = rd16(DP(0x04));  /* 01C089 LDX */
    /* 01C08B CPX */
    if (X >= 0x01E0) goto L_01C0D8;  /* 01C08E BCS */
    /* 01C090 CPX */
    if (X >= 0x0190) goto L_01C0B5;  /* 01C093 BCS */
    /* 01C095 CPX */
    if (X >= 0x016B) goto L_01C0A6;  /* 01C098 BCS */
    /* 01C09A CPX */
    if (X < 0x0166) goto L_01C0A3;  /* 01C09D BCC */
    mflag = 1;  /* 01C09F SEM */
    wr8(DP(0x02), 0x04);  /* 01C0A0 LDM */
L_01C0A3: TICK();
    goto L_01C0B2;  /* 01C0A3 JMP */
L_01C0A6: TICK();
    mflag = 0;  /* 01C0A6 CLM */
    X = rd16(DP(0x04));  /* 01C0A7 LDX */
    /* 01C0A9 CPX */
    if (X < 0x018A) goto L_01C0B2;  /* 01C0AC BCC */
    mflag = 1;  /* 01C0AE SEM */
    wr8(DP(0x02), 0x03);  /* 01C0AF LDM */
L_01C0B2: TICK();
    goto L_01C0D5;  /* 01C0B2 JMP */
L_01C0B5: TICK();
    mflag = 0;  /* 01C0B5 CLM */
    X = rd16(DP(0x04));  /* 01C0B6 LDX */
    /* 01C0B8 CPX */
    if (X >= 0x01B6) goto L_01C0C9;  /* 01C0BB BCS */
    /* 01C0BD CPX */
    if (X < 0x01B0) goto L_01C0C6;  /* 01C0C0 BCC */
    mflag = 1;  /* 01C0C2 SEM */
    wr8(DP(0x02), 0x02);  /* 01C0C3 LDM */
L_01C0C6: TICK();
    goto L_01C0D5;  /* 01C0C6 JMP */
L_01C0C9: TICK();
    mflag = 0;  /* 01C0C9 CLM */
    X = rd16(DP(0x04));  /* 01C0CA LDX */
    /* 01C0CC CPX */
    if (X < 0x01D8) goto L_01C0D5;  /* 01C0CF BCC */
    mflag = 1;  /* 01C0D1 SEM */
    wr8(DP(0x02), 0x01);  /* 01C0D2 LDM */
L_01C0D5: TICK();
    goto L_01C111;  /* 01C0D5 JMP */
L_01C0D8: TICK();
    mflag = 0;  /* 01C0D8 CLM */
    X = rd16(DP(0x04));  /* 01C0D9 LDX */
    /* 01C0DB CPX */
    if (X >= 0x023F) goto L_01C100;  /* 01C0DE BCS */
    /* 01C0E0 CPX */
    if (X >= 0x020C) goto L_01C0F1;  /* 01C0E3 BCS */
    /* 01C0E5 CPX */
    if (X < 0x0204) goto L_01C0EE;  /* 01C0E8 BCC */
    mflag = 1;  /* 01C0EA SEM */
    wr8(DP(0x02), 0x0C);  /* 01C0EB LDM */
L_01C0EE: TICK();
    goto L_01C0FD;  /* 01C0EE JMP */
L_01C0F1: TICK();
    mflag = 0;  /* 01C0F1 CLM */
    X = rd16(DP(0x04));  /* 01C0F2 LDX */
    /* 01C0F4 CPX */
    if (X < 0x0235) goto L_01C0FD;  /* 01C0F7 BCC */
    mflag = 1;  /* 01C0F9 SEM */
    wr8(DP(0x02), 0x0D);  /* 01C0FA LDM */
L_01C0FD: TICK();
    goto L_01C111;  /* 01C0FD JMP */
L_01C100: TICK();
    mflag = 0;  /* 01C100 CLM */
    X = rd16(DP(0x04));  /* 01C101 LDX */
    /* 01C103 CPX */
    if (X >= 0x0274) goto L_01C111;  /* 01C106 BCS */
    /* 01C108 CPX */
    if (X < 0x026A) goto L_01C111;  /* 01C10B BCC */
    mflag = 1;  /* 01C10D SEM */
    wr8(DP(0x02), 0x0B);  /* 01C10E LDM */
L_01C111: TICK();
    goto L_01C37D;  /* 01C111 JMP */
L_01C114: TICK();
    mflag = 0;  /* 01C114 CLM */
    A = rd16(ABS(0x0340));  /* 01C115 LDA */
    mflag = 1;  /* 01C118 SEM */
    SETLO(A, A & 0x03);  /* 01C119 AND */
    /* 01C11B CMP */
    if ((A & 0xFF) == 0x03) goto L_01C122;  /* 01C11D BEQ */
    goto L_01C251;  /* 01C11F JMP */
L_01C122: TICK();
    mflag = 0;  /* 01C122 CLM */
    X = rd16(DP(0x04));  /* 01C123 LDX */
    /* 01C125 CPX */
    if (X < 0x0160) goto L_01C12D;  /* 01C128 BCC */
    goto L_01C1C5;  /* 01C12A JMP */
L_01C12D: TICK();
    /* 01C12D CPX */
    if (X >= 0x0111) goto L_01C17A;  /* 01C130 BCS */
    /* 01C132 CPX */
    if (X >= 0x00F1) goto L_01C157;  /* 01C135 BCS */
    /* 01C137 CPX */
    if (X >= 0x00E2) goto L_01C148;  /* 01C13A BCS */
    /* 01C13C CPX */
    if (X < 0x00E0) goto L_01C145;  /* 01C13F BCC */
    mflag = 1;  /* 01C141 SEM */
    wr8(DP(0x02), 0x0C);  /* 01C142 LDM */
L_01C145: TICK();
    goto L_01C154;  /* 01C145 JMP */
L_01C148: TICK();
    mflag = 0;  /* 01C148 CLM */
    X = rd16(DP(0x04));  /* 01C149 LDX */
    /* 01C14B CPX */
    if (X < 0x00EE) goto L_01C154;  /* 01C14E BCC */
    mflag = 1;  /* 01C150 SEM */
    wr8(DP(0x02), 0x0E);  /* 01C151 LDM */
L_01C154: TICK();
    goto L_01C177;  /* 01C154 JMP */
L_01C157: TICK();
    mflag = 0;  /* 01C157 CLM */
    X = rd16(DP(0x04));  /* 01C158 LDX */
    /* 01C15A CPX */
    if (X >= 0x0100) goto L_01C16B;  /* 01C15D BCS */
    /* 01C15F CPX */
    if (X < 0x00FD) goto L_01C168;  /* 01C162 BCC */
    mflag = 1;  /* 01C164 SEM */
    wr8(DP(0x02), 0x00);  /* 01C165 LDM */
L_01C168: TICK();
    goto L_01C177;  /* 01C168 JMP */
L_01C16B: TICK();
    mflag = 0;  /* 01C16B CLM */
    X = rd16(DP(0x04));  /* 01C16C LDX */
    /* 01C16E CPX */
    if (X < 0x010E) goto L_01C177;  /* 01C171 BCC */
    mflag = 1;  /* 01C173 SEM */
    wr8(DP(0x02), 0x09);  /* 01C174 LDM */
L_01C177: TICK();
    goto L_01C1C2;  /* 01C177 JMP */
L_01C17A: TICK();
    mflag = 0;  /* 01C17A CLM */
    X = rd16(DP(0x04));  /* 01C17B LDX */
    /* 01C17D CPX */
    if (X >= 0x0136) goto L_01C1A2;  /* 01C180 BCS */
    /* 01C182 CPX */
    if (X >= 0x0123) goto L_01C193;  /* 01C185 BCS */
    /* 01C187 CPX */
    if (X < 0x011F) goto L_01C190;  /* 01C18A BCC */
    mflag = 1;  /* 01C18C SEM */
    wr8(DP(0x02), 0x08);  /* 01C18D LDM */
L_01C190: TICK();
    goto L_01C19F;  /* 01C190 JMP */
L_01C193: TICK();
    mflag = 0;  /* 01C193 CLM */
    X = rd16(DP(0x04));  /* 01C194 LDX */
    /* 01C196 CPX */
    if (X < 0x0132) goto L_01C19F;  /* 01C199 BCC */
    mflag = 1;  /* 01C19B SEM */
    wr8(DP(0x02), 0x07);  /* 01C19C LDM */
L_01C19F: TICK();
    goto L_01C1C2;  /* 01C19F JMP */
L_01C1A2: TICK();
    mflag = 0;  /* 01C1A2 CLM */
    X = rd16(DP(0x04));  /* 01C1A3 LDX */
    /* 01C1A5 CPX */
    if (X >= 0x014A) goto L_01C1B6;  /* 01C1A8 BCS */
    /* 01C1AA CPX */
    if (X < 0x0146) goto L_01C1B3;  /* 01C1AD BCC */
    mflag = 1;  /* 01C1AF SEM */
    wr8(DP(0x02), 0x06);  /* 01C1B0 LDM */
L_01C1B3: TICK();
    goto L_01C1C2;  /* 01C1B3 JMP */
L_01C1B6: TICK();
    mflag = 0;  /* 01C1B6 CLM */
    X = rd16(DP(0x04));  /* 01C1B7 LDX */
    /* 01C1B9 CPX */
    if (X < 0x015B) goto L_01C1C2;  /* 01C1BC BCC */
    mflag = 1;  /* 01C1BE SEM */
    wr8(DP(0x02), 0x05);  /* 01C1BF LDM */
L_01C1C2: TICK();
    goto L_01C24E;  /* 01C1C2 JMP */
L_01C1C5: TICK();
    mflag = 0;  /* 01C1C5 CLM */
    X = rd16(DP(0x04));  /* 01C1C6 LDX */
    /* 01C1C8 CPX */
    if (X >= 0x01C4) goto L_01C215;  /* 01C1CB BCS */
    /* 01C1CD CPX */
    if (X >= 0x018F) goto L_01C1F2;  /* 01C1D0 BCS */
    /* 01C1D2 CPX */
    if (X >= 0x0176) goto L_01C1E3;  /* 01C1D5 BCS */
    /* 01C1D7 CPX */
    if (X < 0x0171) goto L_01C1E0;  /* 01C1DA BCC */
    mflag = 1;  /* 01C1DC SEM */
    wr8(DP(0x02), 0x04);  /* 01C1DD LDM */
L_01C1E0: TICK();
    goto L_01C1EF;  /* 01C1E0 JMP */
L_01C1E3: TICK();
    mflag = 0;  /* 01C1E3 CLM */
    X = rd16(DP(0x04));  /* 01C1E4 LDX */
    /* 01C1E6 CPX */
    if (X < 0x0189) goto L_01C1EF;  /* 01C1E9 BCC */
    mflag = 1;  /* 01C1EB SEM */
    wr8(DP(0x02), 0x03);  /* 01C1EC LDM */
L_01C1EF: TICK();
    goto L_01C212;  /* 01C1EF JMP */
L_01C1F2: TICK();
    mflag = 0;  /* 01C1F2 CLM */
    X = rd16(DP(0x04));  /* 01C1F3 LDX */
    /* 01C1F5 CPX */
    if (X >= 0x01A9) goto L_01C206;  /* 01C1F8 BCS */
    /* 01C1FA CPX */
    if (X < 0x01A2) goto L_01C203;  /* 01C1FD BCC */
    mflag = 1;  /* 01C1FF SEM */
    wr8(DP(0x02), 0x02);  /* 01C200 LDM */
L_01C203: TICK();
    goto L_01C212;  /* 01C203 JMP */
L_01C206: TICK();
    mflag = 0;  /* 01C206 CLM */
    X = rd16(DP(0x04));  /* 01C207 LDX */
    /* 01C209 CPX */
    if (X < 0x01BD) goto L_01C212;  /* 01C20C BCC */
    mflag = 1;  /* 01C20E SEM */
    wr8(DP(0x02), 0x01);  /* 01C20F LDM */
L_01C212: TICK();
    goto L_01C24E;  /* 01C212 JMP */
L_01C215: TICK();
    mflag = 0;  /* 01C215 CLM */
    X = rd16(DP(0x04));  /* 01C216 LDX */
    /* 01C218 CPX */
    if (X >= 0x0201) goto L_01C23D;  /* 01C21B BCS */
    /* 01C21D CPX */
    if (X >= 0x01E2) goto L_01C22E;  /* 01C220 BCS */
    /* 01C222 CPX */
    if (X < 0x01DA) goto L_01C22B;  /* 01C225 BCC */
    mflag = 1;  /* 01C227 SEM */
    wr8(DP(0x02), 0x0A);  /* 01C228 LDM */
L_01C22B: TICK();
    goto L_01C23A;  /* 01C22B JMP */
L_01C22E: TICK();
    mflag = 0;  /* 01C22E CLM */
    X = rd16(DP(0x04));  /* 01C22F LDX */
    /* 01C231 CPX */
    if (X < 0x01F9) goto L_01C23A;  /* 01C234 BCC */
    mflag = 1;  /* 01C236 SEM */
    wr8(DP(0x02), 0x0D);  /* 01C237 LDM */
L_01C23A: TICK();
    goto L_01C24E;  /* 01C23A JMP */
L_01C23D: TICK();
    mflag = 0;  /* 01C23D CLM */
    X = rd16(DP(0x04));  /* 01C23E LDX */
    /* 01C240 CPX */
    if (X >= 0x0223) goto L_01C24E;  /* 01C243 BCS */
    /* 01C245 CPX */
    if (X < 0x021A) goto L_01C24E;  /* 01C248 BCC */
    mflag = 1;  /* 01C24A SEM */
    wr8(DP(0x02), 0x0B);  /* 01C24B LDM */
L_01C24E: TICK();
    goto L_01C37D;  /* 01C24E JMP */
L_01C251: TICK();
    mflag = 0;  /* 01C251 CLM */
    X = rd16(DP(0x04));  /* 01C252 LDX */
    /* 01C254 CPX */
    if (X < 0x014A) goto L_01C25C;  /* 01C257 BCC */
    goto L_01C2F4;  /* 01C259 JMP */
L_01C25C: TICK();
    /* 01C25C CPX */
    if (X >= 0x0100) goto L_01C2A9;  /* 01C25F BCS */
    /* 01C261 CPX */
    if (X >= 0x00E2) goto L_01C286;  /* 01C264 BCS */
    /* 01C266 CPX */
    if (X >= 0x00D4) goto L_01C277;  /* 01C269 BCS */
    /* 01C26B CPX */
    if (X < 0x00D1) goto L_01C274;  /* 01C26E BCC */
    mflag = 1;  /* 01C270 SEM */
    wr8(DP(0x02), 0x0A);  /* 01C271 LDM */
L_01C274: TICK();
    goto L_01C283;  /* 01C274 JMP */
L_01C277: TICK();
    mflag = 0;  /* 01C277 CLM */
    X = rd16(DP(0x04));  /* 01C278 LDX */
    /* 01C27A CPX */
    if (X < 0x00E0) goto L_01C283;  /* 01C27D BCC */
    mflag = 1;  /* 01C27F SEM */
    wr8(DP(0x02), 0x0C);  /* 01C280 LDM */
L_01C283: TICK();
    goto L_01C2A6;  /* 01C283 JMP */
L_01C286: TICK();
    mflag = 0;  /* 01C286 CLM */
    X = rd16(DP(0x04));  /* 01C287 LDX */
    /* 01C289 CPX */
    if (X >= 0x00F1) goto L_01C29A;  /* 01C28C BCS */
    /* 01C28E CPX */
    if (X < 0x00EE) goto L_01C297;  /* 01C291 BCC */
    mflag = 1;  /* 01C293 SEM */
    wr8(DP(0x02), 0x0E);  /* 01C294 LDM */
L_01C297: TICK();
    goto L_01C2A6;  /* 01C297 JMP */
L_01C29A: TICK();
    mflag = 0;  /* 01C29A CLM */
    X = rd16(DP(0x04));  /* 01C29B LDX */
    /* 01C29D CPX */
    if (X < 0x00FD) goto L_01C2A6;  /* 01C2A0 BCC */
    mflag = 1;  /* 01C2A2 SEM */
    wr8(DP(0x02), 0x00);  /* 01C2A3 LDM */
L_01C2A6: TICK();
    goto L_01C2F1;  /* 01C2A6 JMP */
L_01C2A9: TICK();
    mflag = 0;  /* 01C2A9 CLM */
    X = rd16(DP(0x04));  /* 01C2AA LDX */
    /* 01C2AC CPX */
    if (X >= 0x0123) goto L_01C2D1;  /* 01C2AF BCS */
    /* 01C2B1 CPX */
    if (X >= 0x0111) goto L_01C2C2;  /* 01C2B4 BCS */
    /* 01C2B6 CPX */
    if (X < 0x010E) goto L_01C2BF;  /* 01C2B9 BCC */
    mflag = 1;  /* 01C2BB SEM */
    wr8(DP(0x02), 0x09);  /* 01C2BC LDM */
L_01C2BF: TICK();
    goto L_01C2CE;  /* 01C2BF JMP */
L_01C2C2: TICK();
    mflag = 0;  /* 01C2C2 CLM */
    X = rd16(DP(0x04));  /* 01C2C3 LDX */
    /* 01C2C5 CPX */
    if (X < 0x011F) goto L_01C2CE;  /* 01C2C8 BCC */
    mflag = 1;  /* 01C2CA SEM */
    wr8(DP(0x02), 0x08);  /* 01C2CB LDM */
L_01C2CE: TICK();
    goto L_01C2F1;  /* 01C2CE JMP */
L_01C2D1: TICK();
    mflag = 0;  /* 01C2D1 CLM */
    X = rd16(DP(0x04));  /* 01C2D2 LDX */
    /* 01C2D4 CPX */
    if (X >= 0x0136) goto L_01C2E5;  /* 01C2D7 BCS */
    /* 01C2D9 CPX */
    if (X < 0x0132) goto L_01C2E2;  /* 01C2DC BCC */
    mflag = 1;  /* 01C2DE SEM */
    wr8(DP(0x02), 0x07);  /* 01C2DF LDM */
L_01C2E2: TICK();
    goto L_01C2F1;  /* 01C2E2 JMP */
L_01C2E5: TICK();
    mflag = 0;  /* 01C2E5 CLM */
    X = rd16(DP(0x04));  /* 01C2E6 LDX */
    /* 01C2E8 CPX */
    if (X < 0x0146) goto L_01C2F1;  /* 01C2EB BCC */
    mflag = 1;  /* 01C2ED SEM */
    wr8(DP(0x02), 0x06);  /* 01C2EE LDM */
L_01C2F1: TICK();
    goto L_01C37D;  /* 01C2F1 JMP */
L_01C2F4: TICK();
    mflag = 0;  /* 01C2F4 CLM */
    X = rd16(DP(0x04));  /* 01C2F5 LDX */
    /* 01C2F7 CPX */
    if (X >= 0x01A9) goto L_01C344;  /* 01C2FA BCS */
    /* 01C2FC CPX */
    if (X >= 0x0176) goto L_01C321;  /* 01C2FF BCS */
    /* 01C301 CPX */
    if (X >= 0x0160) goto L_01C312;  /* 01C304 BCS */
    /* 01C306 CPX */
    if (X < 0x015B) goto L_01C30F;  /* 01C309 BCC */
    mflag = 1;  /* 01C30B SEM */
    wr8(DP(0x02), 0x05);  /* 01C30C LDM */
L_01C30F: TICK();
    goto L_01C31E;  /* 01C30F JMP */
L_01C312: TICK();
    mflag = 0;  /* 01C312 CLM */
    X = rd16(DP(0x04));  /* 01C313 LDX */
    /* 01C315 CPX */
    if (X < 0x0171) goto L_01C31E;  /* 01C318 BCC */
    mflag = 1;  /* 01C31A SEM */
    wr8(DP(0x02), 0x04);  /* 01C31B LDM */
L_01C31E: TICK();
    goto L_01C341;  /* 01C31E JMP */
L_01C321: TICK();
    mflag = 0;  /* 01C321 CLM */
    X = rd16(DP(0x04));  /* 01C322 LDX */
    /* 01C324 CPX */
    if (X >= 0x018F) goto L_01C335;  /* 01C327 BCS */
    /* 01C329 CPX */
    if (X < 0x0189) goto L_01C332;  /* 01C32C BCC */
    mflag = 1;  /* 01C32E SEM */
    wr8(DP(0x02), 0x03);  /* 01C32F LDM */
L_01C332: TICK();
    goto L_01C341;  /* 01C332 JMP */
L_01C335: TICK();
    mflag = 0;  /* 01C335 CLM */
    X = rd16(DP(0x04));  /* 01C336 LDX */
    /* 01C338 CPX */
    if (X < 0x01A2) goto L_01C341;  /* 01C33B BCC */
    mflag = 1;  /* 01C33D SEM */
    wr8(DP(0x02), 0x02);  /* 01C33E LDM */
L_01C341: TICK();
    goto L_01C37D;  /* 01C341 JMP */
L_01C344: TICK();
    mflag = 0;  /* 01C344 CLM */
    X = rd16(DP(0x04));  /* 01C345 LDX */
    /* 01C347 CPX */
    if (X >= 0x0201) goto L_01C36C;  /* 01C34A BCS */
    /* 01C34C CPX */
    if (X >= 0x01C4) goto L_01C35D;  /* 01C34F BCS */
    /* 01C351 CPX */
    if (X < 0x01BD) goto L_01C35A;  /* 01C354 BCC */
    mflag = 1;  /* 01C356 SEM */
    wr8(DP(0x02), 0x01);  /* 01C357 LDM */
L_01C35A: TICK();
    goto L_01C369;  /* 01C35A JMP */
L_01C35D: TICK();
    mflag = 0;  /* 01C35D CLM */
    X = rd16(DP(0x04));  /* 01C35E LDX */
    /* 01C360 CPX */
    if (X < 0x01F9) goto L_01C369;  /* 01C363 BCC */
    mflag = 1;  /* 01C365 SEM */
    wr8(DP(0x02), 0x0D);  /* 01C366 LDM */
L_01C369: TICK();
    goto L_01C37D;  /* 01C369 JMP */
L_01C36C: TICK();
    mflag = 0;  /* 01C36C CLM */
    X = rd16(DP(0x04));  /* 01C36D LDX */
    /* 01C36F CPX */
    if (X >= 0x0223) goto L_01C37D;  /* 01C372 BCS */
    /* 01C374 CPX */
    if (X < 0x021A) goto L_01C37D;  /* 01C377 BCC */
    mflag = 1;  /* 01C379 SEM */
    wr8(DP(0x02), 0x0B);  /* 01C37A LDM */
L_01C37D: TICK();
    mflag = 0;  /* 01C37D CLM */
    A = rd16(ABS(0x026C));  /* 01C37E LDA */
    mflag = 1;  /* 01C381 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 01C382 STA */
    t = (rd8(ABS(0x039A)) + 1) & 0xFF; wr8(ABS(0x039A), t);  /* 01C384 INC */
    SETLO(A, rd8(DP(0x02)));  /* 01C387 LDA */
    /* 01C389 CMP */
    if ((A & 0xFF) != 0x0F) goto L_01C3C9;  /* 01C38B BNE */
    SETLO(A, rd8(ABS(0x026E)));  /* 01C38D LDA */
    /* 01C390 CMP */
    if ((A & 0xFF) != 0x0F) goto L_01C3A9;  /* 01C392 BNE */
    SETLO(A, rd8(ABS(0x026F)));  /* 01C394 LDA */
    /* 01C397 CMP */
    if ((A & 0xFF) != 0x0F) goto L_01C3A9;  /* 01C399 BNE */
    SETLO(A, rd8(ABS(0x0270)));  /* 01C39B LDA */
    /* 01C39E CMP */
    if ((A & 0xFF) != 0x0F) goto L_01C3A9;  /* 01C3A0 BNE */
    SETLO(A, rd8(ABS(0x0271)));  /* 01C3A2 LDA */
    /* 01C3A5 CMP */
    if ((A & 0xFF) == 0x0F) goto L_01C3B0;  /* 01C3A7 BEQ */
L_01C3A9: TICK();
    SETLO(A, rd8(ABS(0x039C)));  /* 01C3A9 LDA */
    /* 01C3AC CMP */
    if ((A & 0xFF) != 0x04) goto L_01C3C6;  /* 01C3AE BNE */
L_01C3B0: TICK();
    wr8(ABS(0x0399), 0x00);  /* 01C3B0 LDM */
    wr8(ABS(0x039A), 0x00);  /* 01C3B4 LDM */
    wr8(ABS(0x039C), 0x00);  /* 01C3B8 LDM */
    SETLO(A, rd8(DP(0x03)));  /* 01C3BC LDA */
    if ((A & 0xFF) == 0) goto L_01C3C6;  /* 01C3BE BEQ */
    mflag = 0;  /* 01C3C0 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C3C1 SEB */
L_01C3C6: TICK();
    goto L_01C60C;  /* 01C3C6 JMP */
L_01C3C9: TICK();
    mflag = 0;  /* 01C3C9 CLM */
    A = rd16(DP(0x02));  /* 01C3CA LDA */
    mflag = 1;  /* 01C3CC SEM */
    t = rd8(ABS(0x039B));  /* 01C3CD CMP */
    if ((A & 0xFF) == t) goto L_01C3DA;  /* 01C3D0 BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 01C3D2 LDA */
    wr8(ABS(0x039B), (uint8_t)A);  /* 01C3D4 STA */
    t = (rd8(ABS(0x0399)) + 1) & 0xFF; wr8(ABS(0x0399), t);  /* 01C3D7 INC */
L_01C3DA: TICK();
    SETLO(A, rd8(ABS(0x0399)));  /* 01C3DA LDA */
    /* 01C3DD CMP */
    if ((A & 0xFF) < 0x03) goto L_01C3E4;  /* 01C3DF BCC */
    goto L_01C5F9;  /* 01C3E1 JMP */
L_01C3E4: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 01C3E4 LDA */
    t = rd8(ABS(0x026E));  /* 01C3E6 CMP */
    if ((A & 0xFF) != t) goto L_01C45F;  /* 01C3E9 BNE */
    t = rd8(ABS(0x026F));  /* 01C3EB CMP */
    if ((A & 0xFF) == t) goto L_01C45C;  /* 01C3EE BEQ */
    t = rd8(ABS(0x0270));  /* 01C3F0 CMP */
    if ((A & 0xFF) == t) goto L_01C45C;  /* 01C3F3 BEQ */
    t = rd8(ABS(0x0271));  /* 01C3F5 CMP */
    if ((A & 0xFF) == t) goto L_01C45C;  /* 01C3F8 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01C3FA LDA */
    if ((A & 0xFF) != 0) goto L_01C416;  /* 01C3FC BNE */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C3FE INC */
    mflag = 0;  /* 01C400 CLM */
    A &= 0x00FF;  /* 01C401 AND */
    X = A;  /* 01C404 TAX */
    A = rd16(DP(0x02));  /* 01C405 LDA */
    mflag = 1;  /* 01C407 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C408 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C40B LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C40F LDM */
    goto L_01C45C;  /* 01C413 JMP */
L_01C416: TICK();
    mflag = 0;  /* 01C416 CLM */
    A = rd16(DP(0x03));  /* 01C417 LDA */
    mflag = 1;  /* 01C419 SEM */
    /* 01C41A CMP */
    if ((A & 0xFF) != 0x01) goto L_01C422;  /* 01C41C BNE */
    wr8(ABS(0x039A), 0x00);  /* 01C41E LDM */
L_01C422: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01C422 LDA */
    mflag = 0;  /* 01C424 CLM */
    A &= 0x00FF;  /* 01C425 AND */
    A -= 1;  /* 01C428 DEC */
    X = A;  /* 01C429 TAX */
    A = rd16((ABS(0x0264) + X));  /* 01C42A LDA */
    mflag = 1;  /* 01C42D SEM */
    t = rd8(DP(0x02));  /* 01C42E CMP */
    if ((A & 0xFF) == t) goto L_01C45C;  /* 01C430 BEQ */
    SETLO(A, rd8(ABS(0x039A)));  /* 01C432 LDA */
    /* 01C435 CMP */
    if ((A & 0xFF) >= 0x08) goto L_01C45C;  /* 01C437 BCS */
    SETLO(A, rd8(DP(0x03)));  /* 01C439 LDA */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C43B INC */
    mflag = 0;  /* 01C43D CLM */
    A &= 0x00FF;  /* 01C43E AND */
    X = A;  /* 01C441 TAX */
    A = rd16(DP(0x02));  /* 01C442 LDA */
    mflag = 1;  /* 01C444 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C445 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C448 LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C44C LDM */
    SETLO(A, rd8(DP(0x03)));  /* 01C450 LDA */
    /* 01C452 CMP */
    if ((A & 0xFF) != 0x07) goto L_01C45C;  /* 01C454 BNE */
    mflag = 0;  /* 01C456 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C457 SEB */
L_01C45C: TICK();
    goto L_01C5D9;  /* 01C45C JMP */
L_01C45F: TICK();
    mflag = 0;  /* 01C45F CLM */
    A = rd16(DP(0x02));  /* 01C460 LDA */
    mflag = 1;  /* 01C462 SEM */
    t = rd8(ABS(0x026F));  /* 01C463 CMP */
    if ((A & 0xFF) != t) goto L_01C4DE;  /* 01C466 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01C468 LDA */
    t = rd8(ABS(0x026E));  /* 01C46A CMP */
    if ((A & 0xFF) == t) goto L_01C4DB;  /* 01C46D BEQ */
    t = rd8(ABS(0x0270));  /* 01C46F CMP */
    if ((A & 0xFF) == t) goto L_01C4DB;  /* 01C472 BEQ */
    t = rd8(ABS(0x0271));  /* 01C474 CMP */
    if ((A & 0xFF) == t) goto L_01C4DB;  /* 01C477 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01C479 LDA */
    if ((A & 0xFF) != 0) goto L_01C495;  /* 01C47B BNE */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C47D INC */
    mflag = 0;  /* 01C47F CLM */
    A &= 0x00FF;  /* 01C480 AND */
    X = A;  /* 01C483 TAX */
    A = rd16(DP(0x02));  /* 01C484 LDA */
    mflag = 1;  /* 01C486 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C487 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C48A LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C48E LDM */
    goto L_01C4DB;  /* 01C492 JMP */
L_01C495: TICK();
    mflag = 0;  /* 01C495 CLM */
    A = rd16(DP(0x03));  /* 01C496 LDA */
    mflag = 1;  /* 01C498 SEM */
    /* 01C499 CMP */
    if ((A & 0xFF) != 0x01) goto L_01C4A1;  /* 01C49B BNE */
    wr8(ABS(0x039A), 0x00);  /* 01C49D LDM */
L_01C4A1: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01C4A1 LDA */
    mflag = 0;  /* 01C4A3 CLM */
    A &= 0x00FF;  /* 01C4A4 AND */
    A -= 1;  /* 01C4A7 DEC */
    X = A;  /* 01C4A8 TAX */
    A = rd16((ABS(0x0264) + X));  /* 01C4A9 LDA */
    mflag = 1;  /* 01C4AC SEM */
    t = rd8(DP(0x02));  /* 01C4AD CMP */
    if ((A & 0xFF) == t) goto L_01C4DB;  /* 01C4AF BEQ */
    SETLO(A, rd8(ABS(0x039A)));  /* 01C4B1 LDA */
    /* 01C4B4 CMP */
    if ((A & 0xFF) >= 0x08) goto L_01C4DB;  /* 01C4B6 BCS */
    SETLO(A, rd8(DP(0x03)));  /* 01C4B8 LDA */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C4BA INC */
    mflag = 0;  /* 01C4BC CLM */
    A &= 0x00FF;  /* 01C4BD AND */
    X = A;  /* 01C4C0 TAX */
    A = rd16(DP(0x02));  /* 01C4C1 LDA */
    mflag = 1;  /* 01C4C3 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C4C4 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C4C7 LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C4CB LDM */
    SETLO(A, rd8(DP(0x03)));  /* 01C4CF LDA */
    /* 01C4D1 CMP */
    if ((A & 0xFF) != 0x07) goto L_01C4DB;  /* 01C4D3 BNE */
    mflag = 0;  /* 01C4D5 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C4D6 SEB */
L_01C4DB: TICK();
    goto L_01C5D9;  /* 01C4DB JMP */
L_01C4DE: TICK();
    mflag = 0;  /* 01C4DE CLM */
    A = rd16(DP(0x02));  /* 01C4DF LDA */
    mflag = 1;  /* 01C4E1 SEM */
    t = rd8(ABS(0x0270));  /* 01C4E2 CMP */
    if ((A & 0xFF) != t) goto L_01C55D;  /* 01C4E5 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01C4E7 LDA */
    t = rd8(ABS(0x026E));  /* 01C4E9 CMP */
    if ((A & 0xFF) == t) goto L_01C55A;  /* 01C4EC BEQ */
    t = rd8(ABS(0x026F));  /* 01C4EE CMP */
    if ((A & 0xFF) == t) goto L_01C55A;  /* 01C4F1 BEQ */
    t = rd8(ABS(0x0271));  /* 01C4F3 CMP */
    if ((A & 0xFF) == t) goto L_01C55A;  /* 01C4F6 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01C4F8 LDA */
    if ((A & 0xFF) != 0) goto L_01C514;  /* 01C4FA BNE */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C4FC INC */
    mflag = 0;  /* 01C4FE CLM */
    A &= 0x00FF;  /* 01C4FF AND */
    X = A;  /* 01C502 TAX */
    A = rd16(DP(0x02));  /* 01C503 LDA */
    mflag = 1;  /* 01C505 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C506 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C509 LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C50D LDM */
    goto L_01C55A;  /* 01C511 JMP */
L_01C514: TICK();
    mflag = 0;  /* 01C514 CLM */
    A = rd16(DP(0x03));  /* 01C515 LDA */
    mflag = 1;  /* 01C517 SEM */
    /* 01C518 CMP */
    if ((A & 0xFF) != 0x01) goto L_01C520;  /* 01C51A BNE */
    wr8(ABS(0x039A), 0x00);  /* 01C51C LDM */
L_01C520: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01C520 LDA */
    mflag = 0;  /* 01C522 CLM */
    A &= 0x00FF;  /* 01C523 AND */
    A -= 1;  /* 01C526 DEC */
    X = A;  /* 01C527 TAX */
    A = rd16((ABS(0x0264) + X));  /* 01C528 LDA */
    mflag = 1;  /* 01C52B SEM */
    t = rd8(DP(0x02));  /* 01C52C CMP */
    if ((A & 0xFF) == t) goto L_01C55A;  /* 01C52E BEQ */
    SETLO(A, rd8(ABS(0x039A)));  /* 01C530 LDA */
    /* 01C533 CMP */
    if ((A & 0xFF) >= 0x08) goto L_01C55A;  /* 01C535 BCS */
    SETLO(A, rd8(DP(0x03)));  /* 01C537 LDA */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C539 INC */
    mflag = 0;  /* 01C53B CLM */
    A &= 0x00FF;  /* 01C53C AND */
    X = A;  /* 01C53F TAX */
    A = rd16(DP(0x02));  /* 01C540 LDA */
    mflag = 1;  /* 01C542 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C543 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C546 LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C54A LDM */
    SETLO(A, rd8(DP(0x03)));  /* 01C54E LDA */
    /* 01C550 CMP */
    if ((A & 0xFF) != 0x07) goto L_01C55A;  /* 01C552 BNE */
    mflag = 0;  /* 01C554 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C555 SEB */
L_01C55A: TICK();
    goto L_01C5D9;  /* 01C55A JMP */
L_01C55D: TICK();
    mflag = 0;  /* 01C55D CLM */
    A = rd16(DP(0x02));  /* 01C55E LDA */
    mflag = 1;  /* 01C560 SEM */
    t = rd8(ABS(0x0271));  /* 01C561 CMP */
    if ((A & 0xFF) != t) goto L_01C5D9;  /* 01C564 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01C566 LDA */
    t = rd8(ABS(0x026E));  /* 01C568 CMP */
    if ((A & 0xFF) == t) goto L_01C5D9;  /* 01C56B BEQ */
    t = rd8(ABS(0x026F));  /* 01C56D CMP */
    if ((A & 0xFF) == t) goto L_01C5D9;  /* 01C570 BEQ */
    t = rd8(ABS(0x0270));  /* 01C572 CMP */
    if ((A & 0xFF) == t) goto L_01C5D9;  /* 01C575 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 01C577 LDA */
    if ((A & 0xFF) != 0) goto L_01C593;  /* 01C579 BNE */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C57B INC */
    mflag = 0;  /* 01C57D CLM */
    A &= 0x00FF;  /* 01C57E AND */
    X = A;  /* 01C581 TAX */
    A = rd16(DP(0x02));  /* 01C582 LDA */
    mflag = 1;  /* 01C584 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C585 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C588 LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C58C LDM */
    goto L_01C5D9;  /* 01C590 JMP */
L_01C593: TICK();
    mflag = 0;  /* 01C593 CLM */
    A = rd16(DP(0x03));  /* 01C594 LDA */
    mflag = 1;  /* 01C596 SEM */
    /* 01C597 CMP */
    if ((A & 0xFF) != 0x01) goto L_01C59F;  /* 01C599 BNE */
    wr8(ABS(0x039A), 0x00);  /* 01C59B LDM */
L_01C59F: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 01C59F LDA */
    mflag = 0;  /* 01C5A1 CLM */
    A &= 0x00FF;  /* 01C5A2 AND */
    A -= 1;  /* 01C5A5 DEC */
    X = A;  /* 01C5A6 TAX */
    A = rd16((ABS(0x0264) + X));  /* 01C5A7 LDA */
    mflag = 1;  /* 01C5AA SEM */
    t = rd8(DP(0x02));  /* 01C5AB CMP */
    if ((A & 0xFF) == t) goto L_01C5D9;  /* 01C5AD BEQ */
    SETLO(A, rd8(ABS(0x039A)));  /* 01C5AF LDA */
    /* 01C5B2 CMP */
    if ((A & 0xFF) >= 0x08) goto L_01C5D9;  /* 01C5B4 BCS */
    SETLO(A, rd8(DP(0x03)));  /* 01C5B6 LDA */
    t = (rd8(DP(0x03)) + 1) & 0xFF; wr8(DP(0x03), t);  /* 01C5B8 INC */
    mflag = 0;  /* 01C5BA CLM */
    A &= 0x00FF;  /* 01C5BB AND */
    X = A;  /* 01C5BE TAX */
    A = rd16(DP(0x02));  /* 01C5BF LDA */
    mflag = 1;  /* 01C5C1 SEM */
    wr8((ABS(0x0264) + X), (uint8_t)A);  /* 01C5C2 STA */
    wr8(ABS(0x039A), 0x00);  /* 01C5C5 LDM */
    wr8(ABS(0x0399), 0x00);  /* 01C5C9 LDM */
    SETLO(A, rd8(DP(0x03)));  /* 01C5CD LDA */
    /* 01C5CF CMP */
    if ((A & 0xFF) != 0x07) goto L_01C5D9;  /* 01C5D1 BNE */
    mflag = 0;  /* 01C5D3 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C5D4 SEB */
L_01C5D9: TICK();
    mflag = 0;  /* 01C5D9 CLM */
    A = rd16(ABS(0x039A));  /* 01C5DA LDA */
    mflag = 1;  /* 01C5DD SEM */
    /* 01C5DE CMP */
    if ((A & 0xFF) < 0x08) goto L_01C5F6;  /* 01C5E0 BCC */
    SETLO(A, rd8(DP(0x03)));  /* 01C5E2 LDA */
    /* 01C5E4 CMP */
    if ((A & 0xFF) < 0x02) goto L_01C5F6;  /* 01C5E6 BCC */
    wr8(ABS(0x0399), 0x00);  /* 01C5E8 LDM */
    wr8(ABS(0x039A), 0x00);  /* 01C5EC LDM */
    mflag = 0;  /* 01C5F0 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C5F1 SEB */
L_01C5F6: TICK();
    goto L_01C60C;  /* 01C5F6 JMP */
L_01C5F9: TICK();
    mflag = 1;  /* 01C5F9 SEM */
    wr8(ABS(0x0399), 0x00);  /* 01C5FA LDM */
    wr8(ABS(0x039A), 0x00);  /* 01C5FE LDM */
    SETLO(A, rd8(DP(0x03)));  /* 01C602 LDA */
    if ((A & 0xFF) == 0) goto L_01C60C;  /* 01C604 BEQ */
    mflag = 0;  /* 01C606 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0100);  /* 01C607 SEB */
L_01C60C: TICK();
    mflag = 0;  /* 01C60C CLM */
    A = rd16(DP(0x03));  /* 01C60D LDA */
    mflag = 1;  /* 01C60F SEM */
    wr8(ABS(0x026C), (uint8_t)A);  /* 01C610 STA */
    SETLO(A, rd8(DP(0x02)));  /* 01C613 LDA */
    wr8(ABS(0x026D), (uint8_t)A);  /* 01C615 STA */
    mflag = 0;  /* 01C618 CLM */
    if ((rd16(ABS(0x023E)) & 0x0100) == 0) goto L_01C624;  /* 01C619 BBC */
    mflag = 1;  /* 01C61F SEM */
    wr8(ABS(0x01E9), 0x00);  /* 01C620 LDM */
L_01C624: TICK();
    mflag = 0;  /* 01C624 CLM */
    D = pull16();  /* 01C625 PLD */
    S += 3; return;  /* 01C626 RTL */
}

/* 01CA08  callers: 00575A */
void F_01CA08(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01CA08 LDA */
    push16(D);  /* 01CA0A PHD */
    D = A;  /* 01CA0B TAD */
    C = 0;  /* 01CA0C CLC */
    A = adc16(A, 0x0009);  /* 01CA0D ADC */
    push16(A);  /* 01CA10 PHA */
    mflag = 1;  /* 01CA11 SEM */
    wr8(ABS(WDT), 0x00);  /* 01CA12 LDM */
    SETLO(A, rd8(0x0100FF));  /* 01CA16 LDA */
    SETLO(A, A - 1);  /* 01CA1A DEC */
    if ((A & 0xFF) == 0) goto L_01CA20;  /* 01CA1B BEQ */
    goto L_01CBBF;  /* 01CA1D JMP */
L_01CA20: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 01CA20 LDA */
    if ((A & 0xFF) == 0) goto L_01CA28;  /* 01CA23 BEQ */
    goto L_01CBBF;  /* 01CA25 JMP */
L_01CA28: TICK();
    mflag = 0;  /* 01CA28 CLM */
    if ((rd16(ABS(0x0240)) & 0x8000) == 0) goto L_01CA32;  /* 01CA29 BBC */
    goto L_01CBBF;  /* 01CA2F JMP */
L_01CA32: TICK();
    mflag = 1;  /* 01CA32 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 01CA33 LDA */
    if ((A & 0xFF) == 0) goto L_01CA3B;  /* 01CA36 BEQ */
    goto L_01CBBF;  /* 01CA38 JMP */
L_01CA3B: TICK();
    mflag = 0;  /* 01CA3B CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_01CA45;  /* 01CA3C BBC */
    goto L_01CBBF;  /* 01CA42 JMP */
L_01CA45: TICK();
    S -= 3; F_00EA66();  /* 01CA45 JSRL */
    wr16(DP(0x09), 0x0001);  /* 01CA49 LDM */
    Y = 0x0001;  /* 01CA4D LDY */
    A = D;  /* 01CA50 TDA */
    C = 0;  /* 01CA51 CLC */
    A = adc16(A, 0x000B);  /* 01CA52 ADC */
    push16(A);  /* 01CA55 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01CA56 JSRL */
    Y = pull16();  /* 01CA5A PLY */
    mflag = 0;  /* 01CA5B CLM */
    wr16(DP(0x09), 0x0010);  /* 01CA5C LDM */
    wr16(DP(0x0B), 0x0058);  /* 01CA60 LDM */
    wr16(DP(0x0D), 0x0060);  /* 01CA64 LDM */
    Y = 0x0003;  /* 01CA68 LDY */
    A = D;  /* 01CA6B TDA */
    C = 0;  /* 01CA6C CLC */
    A = adc16(A, 0x000F);  /* 01CA6D ADC */
    push16(A);  /* 01CA70 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01CA71 JSRL */
    Y = pull16();  /* 01CA75 PLY */
    mflag = 0;  /* 01CA76 CLM */
    wr16(DP(0x09), 0x0004);  /* 01CA77 LDM */
    wr16(DP(0x0B), 0x0040);  /* 01CA7B LDM */
    Y = 0x0002;  /* 01CA7F LDY */
    A = D;  /* 01CA82 TDA */
    C = 0;  /* 01CA83 CLC */
    A = adc16(A, 0x000D);  /* 01CA84 ADC */
    push16(A);  /* 01CA87 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01CA88 JSRL */
    Y = pull16();  /* 01CA8C PLY */
    mflag = 0;  /* 01CA8D CLM */
    wr16(DP(0x09), 0x0020);  /* 01CA8E LDM */
    wr16(DP(0x0B), 0x0013);  /* 01CA92 LDM */
    Y = 0x0002;  /* 01CA96 LDY */
    A = D;  /* 01CA99 TDA */
    C = 0;  /* 01CA9A CLC */
    A = adc16(A, 0x000D);  /* 01CA9B ADC */
    push16(A);  /* 01CA9E PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01CA9F JSRL */
    Y = pull16();  /* 01CAA3 PLY */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x20);  /* 01CAA4 CLB [P55 ALMA_SEC] */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 01CAA8 CLB [P71 GUC_ACMA] */
    Y = 0x0000;  /* 01CAAC LDY */
    mflag = 0;  /* 01CAAF CLM */
    S -= 3; F_0080AA();  /* 01CAB0 JSRL */
    mflag = 1;  /* 01CAB4 SEM */
    wr8(DP(0x00), 0x01);  /* 01CAB5 LDM */
    mflag = 0;  /* 01CAB8 CLM */
    S -= 3; F_01CBC3();  /* 01CAB9 JSRL */
    wr16(DP(0x03), A);  /* 01CABD STA */
    A = rd16(ABS(0x01EA));  /* 01CABF LDA */
    mflag = 1;  /* 01CAC2 SEM */
    /* 01CAC3 CMP */
    if ((A & 0xFF) != 0x05) goto L_01CAD0;  /* 01CAC5 BNE */
    mflag = 0;  /* 01CAC7 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 01CAC8 SEB */
    goto L_01CAD6;  /* 01CACD JMP */
L_01CAD0: TICK();
    mflag = 0;  /* 01CAD0 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 01CAD1 CLB */
L_01CAD6: TICK();
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 01CAD6 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0008);  /* 01CADB CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0400);  /* 01CAE0 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0004);  /* 01CAE5 SEB */
    mflag = 1;  /* 01CAEA SEM */
    wr8(ABS(0x0226), 0x0F);  /* 01CAEB LDM */
    wr8(DP(0x01), 0x00);  /* 01CAEF LDM */
    wr8(DP(0x02), 0x00);  /* 01CAF2 LDM */
    mflag = 0;  /* 01CAF5 CLM */
    wr16(DP(0x05), 0x0000);  /* 01CAF6 LDM */
    wr16(DP(0x07), 0x0000);  /* 01CAFA LDM */
    mflag = 1;  /* 01CAFE SEM */
    wr8(ABS(0x0231), 0x09);  /* 01CAFF LDM */
    wr8(ABS(0x0230), 0x00);  /* 01CB03 LDM */
    Y = 0x0009;  /* 01CB07 LDY */
    mflag = 0;  /* 01CB0A CLM */
    S -= 3; F_01CC04();  /* 01CB0B JSRL */
    Y = rd16(DP(0x03));  /* 01CB0F LDY */
    S -= 3; F_01CF78();  /* 01CB11 JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 01CB15 SEB */
    S -= 3; F_007A06();  /* 01CB1A JSRL */
L_01CB1E: TICK();
    mflag = 1;  /* 01CB1E SEM */
    SETLO(A, rd8(DP(0x00)));  /* 01CB1F LDA */
    if ((A & 0xFF) == 0) goto L_01CB74;  /* 01CB21 BEQ */
    mflag = 0;  /* 01CB23 CLM */
    A = D;  /* 01CB24 TDA */
    B = 0x0000;  /* 01CB25 LDA */
    wr16(DP(0x09), A);  /* 01CB29 STA */
    wr16(DP(0x0B), B);  /* 01CB2B STA */
    A += 1;  /* 01CB2E INC */
    A += 1;  /* 01CB2F INC */
    A += 1;  /* 01CB30 INC */
    wr16(DP(0x0C), A);  /* 01CB31 STA */
    wr16(DP(0x0E), B);  /* 01CB33 STA */
    A = D;  /* 01CB36 TDA */
    A += 1;  /* 01CB37 INC */
    Y = A;  /* 01CB38 TAY */
    S -= 3; F_01CC39();  /* 01CB39 JSRL */
    X = rd16(DP(0x03));  /* 01CB3D LDX */
    wr16(DP(0x09), X);  /* 01CB3F STX */
    A = D;  /* 01CB41 TDA */
    C = 0;  /* 01CB42 CLC */
    A = adc16(A, 0x0005);  /* 01CB43 ADC */
    B = 0x0000;  /* 01CB46 LDA */
    wr16(DP(0x0B), A);  /* 01CB4A STA */
    wr16(DP(0x0D), B);  /* 01CB4C STA */
    A = D;  /* 01CB4F TDA */
    A += 1;  /* 01CB50 INC */
    A += 1;  /* 01CB51 INC */
    wr16(DP(0x0E), A);  /* 01CB52 STA */
    wr16(DP(0x10), B);  /* 01CB54 STA */
    A = D;  /* 01CB57 TDA */
    A += 1;  /* 01CB58 INC */
    Y = A;  /* 01CB59 TAY */
    S -= 3; F_01D273();  /* 01CB5A JSRL */
    S -= 3; F_01DC35();  /* 01CB5E JSRL */
    mflag = 1;  /* 01CB62 SEM */
    wr8(ABS(WDT), 0x00);  /* 01CB63 LDM */
    mflag = 0;  /* 01CB67 CLM */
    ram_trampoline_087A();  /* 01CB68 JSRL */
    mflag = 1;  /* 01CB6C SEM */
    wr8(ABS(WDT), 0x00);  /* 01CB6D LDM */
    goto L_01CB1E;  /* 01CB71 JMP */
L_01CB74: TICK();
    mflag = 1;  /* 01CB74 SEM */
    wr8(ABS(0x0231), 0x14);  /* 01CB75 LDM */
    wr8(ABS(0x0230), 0x00);  /* 01CB79 LDM */
    wr8(ABS(0x0226), 0x2A);  /* 01CB7D LDM */
    mflag = 0;  /* 01CB81 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 01CB82 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 01CB87 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0008);  /* 01CB8C CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0004);  /* 01CB91 CLB */
    S -= 3; F_019852();  /* 01CB96 JSRL */
    Y = 0x0019;  /* 01CB9A LDY */
    S -= 3; F_019884();  /* 01CB9D JSRL */
    mflag = 1;  /* 01CBA1 SEM */
    wr8(ABS(0x0228), 0x01);  /* 01CBA2 LDM */
    wr8(ABS(0x0229), 0x02);  /* 01CBA6 LDM */
    mflag = 0;  /* 01CBAA CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0080);  /* 01CBAB CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 01CBB0 CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x8000);  /* 01CBB5 CLB */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0008);  /* 01CBBA CLB */
L_01CBBF: TICK();
    mflag = 0;  /* 01CBBF CLM */
    Y = pull16();  /* 01CBC0 PLY */
    D = pull16();  /* 01CBC1 PLD */
    S += 3; return;  /* 01CBC2 RTL */
}

/* 01CBC3  callers: 01CAB9 */
void F_01CBC3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01CBC3 LDA */
    push16(D);  /* 01CBC5 PHD */
    D = A;  /* 01CBC6 TAD */
    C = 0;  /* 01CBC7 CLC */
    A = adc16(A, 0x0004);  /* 01CBC8 ADC */
    push16(A);  /* 01CBCB PHA */
    Y = 0x0589;  /* 01CBCC LDY */
    S -= 3; eeprom_read_word_0071F0();  /* 01CBCF JSRL */
    A &= 0x0FFF;  /* 01CBD3 AND */
    wr16(DP(0x00), A);  /* 01CBD6 STA */
    A = rla16(A, 0x0008);  /* 01CBD8 RLA */
    A &= 0x00FF;  /* 01CBDC AND */
    mpy16(0x0064);  /* 01CBDF MPY */
    wr16(DP(0x04), A);  /* 01CBE3 STA */
    A = rd16(DP(0x00));  /* 01CBE5 LDA */
    A = lsr16(A);  /* 01CBE7 LSR */
    A = lsr16(A);  /* 01CBE8 LSR */
    A = lsr16(A);  /* 01CBE9 LSR */
    A = lsr16(A);  /* 01CBEA LSR */
    A &= 0x000F;  /* 01CBEB AND */
    mpy16(0x000A);  /* 01CBEE MPY */
    C = 0;  /* 01CBF2 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01CBF3 ADC */
    wr16(DP(0x04), A);  /* 01CBF5 STA */
    A = rd16(DP(0x00));  /* 01CBF7 LDA */
    A &= 0x000F;  /* 01CBF9 AND */
    C = 0;  /* 01CBFC CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01CBFD ADC */
    wr16(DP(0x02), A);  /* 01CBFF STA */
    Y = pull16();  /* 01CC01 PLY */
    D = pull16();  /* 01CC02 PLD */
    S += 3; return;  /* 01CC03 RTL */
}

/* 01CC04  callers: 01CB0B, 01CC9D, 01CD08, 01CDAE, 01CF0F, 01CF69, 01D50E, 01D82A */
void F_01CC04(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01CC04 LDA */
    push16(D);  /* 01CC06 PHD */
    D = A;  /* 01CC07 TAD */
    wr16(DP(0x00), Y);  /* 01CC08 STY */
    A = Y;  /* 01CC0A TYA */
    A &= 0x00FF;  /* 01CC0B AND */
    A = asl16(A);  /* 01CC0E ASL */
    A = asl16(A);  /* 01CC0F ASL */
    A = asl16(A);  /* 01CC10 ASL */
    C = 0;  /* 01CC11 CLC */
    A = adc16(A, 0x2108);  /* 01CC12 ADC */
    wr16(DP(0x01), A);  /* 01CC15 STA */
    mflag = 1;  /* 01CC17 SEM */
    wr8(DP(0x03), 0x00);  /* 01CC18 LDM */
    mflag = 0;  /* 01CC1B CLM */
    wr16(DP(0x04), 0x0000);  /* 01CC1C LDM */
L_01CC20: TICK();
    X = rd16(DP(0x04));  /* 01CC20 LDX */
    /* 01CC22 CPX */
    if (X >= 0x0008) goto L_01CC36;  /* 01CC25 BCS */
    A = rd16(LIND(0x01));  /* 01CC27 LDA */
    mflag = 1;  /* 01CC29 SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 01CC2A STA */
    mflag = 0;  /* 01CC2D CLM */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 01CC2E INC */
    X += 1;  /* 01CC30 INX */
    wr16(DP(0x04), X);  /* 01CC31 STX */
    goto L_01CC20;  /* 01CC33 JMP */
L_01CC36: TICK();
    mflag = 0;  /* 01CC36 CLM */
    D = pull16();  /* 01CC37 PLD */
    S += 3; return;  /* 01CC38 RTL */
}

/* 01CC39  callers: 01CB39 */
void F_01CC39(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01CC39 LDA */
    push16(D);  /* 01CC3B PHD */
    D = A;  /* 01CC3C TAD */
    C = 0;  /* 01CC3D CLC */
    A = adc16(A, 0x000B);  /* 01CC3E ADC */
    push16(A);  /* 01CC41 PHA */
    wr16(DP(0x06), Y);  /* 01CC42 STY */
    wr16(DP(0x08), B);  /* 01CC44 STA */
    mflag = 1;  /* 01CC47 SEM */
    if ((rd8(ABS(0x023A)) & 0x02) == 0x02) goto L_01CC50;  /* 01CC48 BBS */
    goto L_01CF27;  /* 01CC4D JMP */
L_01CC50: TICK();
    mflag = 0;  /* 01CC50 CLM */
    S -= 3; F_0050DA();  /* 01CC51 JSRL */
    A = rd16(ABS(0x0226));  /* 01CC55 LDA */
    mflag = 1;  /* 01CC58 SEM */
    /* 01CC59 CMP */
    if ((A & 0xFF) != 0x0F) goto L_01CC60;  /* 01CC5B BNE */
    goto L_01CF24;  /* 01CC5D JMP */
L_01CC60: TICK();
    SETLO(A, rd8(LIND(0x06)));  /* 01CC60 LDA */
    if ((A & 0xFF) == 0) goto L_01CC75;  /* 01CC62 BEQ */
    SETLO(A, rd8(LIND(0x06)));  /* 01CC64 LDA */
    /* 01CC66 CMP */
    if ((A & 0xFF) == 0x04) goto L_01CC75;  /* 01CC68 BEQ */
    wr8(ABS(0x0226), 0x0F);  /* 01CC6A LDM */
    wr8(ABS(0x0293), 0x00);  /* 01CC6E LDM */
    goto L_01CF24;  /* 01CC72 JMP */
L_01CC75: TICK();
    mflag = 0;  /* 01CC75 CLM */
    A = rd16(ABS(0x0231));  /* 01CC76 LDA */
    mflag = 1;  /* 01CC79 SEM */
    /* 01CC7A CMP */
    if ((A & 0xFF) == 0x0A) goto L_01CC81;  /* 01CC7C BEQ */
    goto L_01CD26;  /* 01CC7E JMP */
L_01CC81: TICK();
    wr8(ABS(0x0230), 0x5E);  /* 01CC81 LDM */
    SETLO(A, rd8(ABS(0x0226)));  /* 01CC85 LDA */
    /* 01CC88 CMP */
    if ((A & 0xFF) == 0x64) goto L_01CCA4;  /* 01CC8A BEQ */
    Y = 0x000D;  /* 01CC8C LDY */
    mflag = 0;  /* 01CC8F CLM */
    S -= 3; F_0080AA();  /* 01CC90 JSRL */
    mflag = 1;  /* 01CC94 SEM */
    wr8(ABS(0x0293), 0x00);  /* 01CC95 LDM */
    Y = rd16(ABS(0x0231));  /* 01CC99 LDY */
    mflag = 0;  /* 01CC9C CLM */
    S -= 3; F_01CC04();  /* 01CC9D JSRL */
    goto L_01CD17;  /* 01CCA1 JMP */
L_01CCA4: TICK();
    mflag = 0;  /* 01CCA4 CLM */
    wr16(DP(0x09), 0x0000);  /* 01CCA5 LDM */
L_01CCA9: TICK();
    A = rd16(ABS(0x0293));  /* 01CCA9 LDA */
    A &= 0x00FF;  /* 01CCAC AND */
    t = rd16(DP(0x09));  /* 01CCAF CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_01CCC5;  /* 01CCB1 BCC */
    if (Z) goto L_01CCC5;  /* 01CCB3 BEQ */
    X = rd16(DP(0x09));  /* 01CCB5 LDX */
    A = rd16((ABS(0x0283) + X));  /* 01CCB7 LDA */
    mflag = 1;  /* 01CCBA SEM */
    wr8((ABS(0x0257) + X), (uint8_t)A);  /* 01CCBB STA */
    X += 1;  /* 01CCBE INX */
    wr16(DP(0x09), X);  /* 01CCBF STX */
    mflag = 0;  /* 01CCC1 CLM */
    goto L_01CCA9;  /* 01CCC2 JMP */
L_01CCC5: TICK();
    mflag = 0;  /* 01CCC5 CLM */
    A = rd16(ABS(0x0293));  /* 01CCC6 LDA */
    mflag = 1;  /* 01CCC9 SEM */
    /* 01CCCA CMP */
    if ((A & 0xFF) != 0x03) goto L_01CD0C;  /* 01CCCC BNE */
    SETLO(A, rd8(ABS(0x0284)));  /* 01CCCE LDA */
    mpy8(0x0A);  /* 01CCD1 MPY */
    wr8(DP(0x0B), (uint8_t)A);  /* 01CCD4 STA */
    wr8(DP(0x0C), (uint8_t)B);  /* 01CCD6 STA */
    SETLO(A, rd8(ABS(0x0283)));  /* 01CCD9 LDA */
    mpy8(0x64);  /* 01CCDC MPY */
    wr8(DP(0x0D), (uint8_t)A);  /* 01CCDF STA */
    wr8(DP(0x0E), (uint8_t)B);  /* 01CCE1 STA */
    mflag = 0;  /* 01CCE4 CLM */
    A = rd16(DP(0x0B));  /* 01CCE5 LDA */
    C = 0;  /* 01CCE7 CLC */
    A = adc16(A, rd16(DP(0x0D)));  /* 01CCE8 ADC */
    wr16(DP(0x0B), A);  /* 01CCEA STA */
    A = rd16(ABS(0x0285));  /* 01CCEC LDA */
    A &= 0x00FF;  /* 01CCEF AND */
    C = 0;  /* 01CCF2 CLC */
    A = adc16(A, rd16(DP(0x0B)));  /* 01CCF3 ADC */
    C = 0;  /* 01CCF5 CLC */
    A = adc16(A, 0xEB30);  /* 01CCF6 ADC */
    wr16(LIND(0x03), A);  /* 01CCF9 STA */
    mflag = 1;  /* 01CCFB SEM */
    wr8(ABS(0x0293), 0x00);  /* 01CCFC LDM */
    wr8(ABS(0x0231), 0x09);  /* 01CD00 LDM */
    Y = 0x0009;  /* 01CD04 LDY */
    mflag = 0;  /* 01CD07 CLM */
    S -= 3; F_01CC04();  /* 01CD08 JSRL */
L_01CD0C: TICK();
    mflag = 0;  /* 01CD0C CLM */
    X = rd16(ABS(0x0214));  /* 01CD0D LDX */
    if (X != 0) goto L_01CD17;  /* 01CD10 BNE */
    Y = X;  /* 01CD12 TXY */
    S -= 3; F_0080AA();  /* 01CD13 JSRL */
L_01CD17: TICK();
    A = rd16(LIND(0x03));  /* 01CD17 LDA */
    Y = A;  /* 01CD19 TAY */
    S -= 3; F_01CF78();  /* 01CD1A JSRL */
    mflag = 1;  /* 01CD1E SEM */
    wr8(ABS(0x0226), 0x0F);  /* 01CD1F LDM */
    goto L_01CF24;  /* 01CD23 JMP */
L_01CD26: TICK();
    mflag = 1;  /* 01CD26 SEM */
    wr8(ABS(0x0230), 0x3F);  /* 01CD27 LDM */
    SETLO(A, rd8(ABS(0x0226)));  /* 01CD2B LDA */
    /* 01CD2E CMP */
    if ((A & 0xFF) != 0x6D) goto L_01CD39;  /* 01CD30 BNE */
    wr8(ABS(0x0231), 0x00);  /* 01CD32 LDM */
    goto L_01CEF8;  /* 01CD36 JMP */
L_01CD39: TICK();
    mflag = 0;  /* 01CD39 CLM */
    A = rd16(ABS(0x0226));  /* 01CD3A LDA */
    mflag = 1;  /* 01CD3D SEM */
    /* 01CD3E CMP */
    if ((A & 0xFF) != 0x73) goto L_01CD65;  /* 01CD40 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 01CD42 LDA */
    /* 01CD45 CMP */
    if ((A & 0xFF) >= 0x09) goto L_01CD5D;  /* 01CD47 BCS */
    mflag = 0;  /* 01CD49 CLM */
    A &= 0x00FF;  /* 01CD4A AND */
    A += 1;  /* 01CD4D INC */
    B = 0x0009;  /* 01CD4E LDA */
    S -= 3; F_01E350();  /* 01CD52 JSRL */
    mflag = 1;  /* 01CD56 SEM */
    wr8(ABS(0x0231), (uint8_t)A);  /* 01CD57 STA */
    goto L_01CD62;  /* 01CD5A JMP */
L_01CD5D: TICK();
    mflag = 1;  /* 01CD5D SEM */
    wr8(ABS(0x0231), 0x09);  /* 01CD5E LDM */
L_01CD62: TICK();
    goto L_01CEF8;  /* 01CD62 JMP */
L_01CD65: TICK();
    mflag = 0;  /* 01CD65 CLM */
    A = rd16(ABS(0x0226));  /* 01CD66 LDA */
    mflag = 1;  /* 01CD69 SEM */
    /* 01CD6A CMP */
    if ((A & 0xFF) != 0x70) goto L_01CD8A;  /* 01CD6C BNE */
    wr8(ABS(0x0231), 0x09);  /* 01CD6E LDM */
    mflag = 0;  /* 01CD72 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_01CD81;  /* 01CD73 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0004);  /* 01CD79 CLB */
    goto L_01CD87;  /* 01CD7E JMP */
L_01CD81: TICK();
    mflag = 0;  /* 01CD81 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0004);  /* 01CD82 SEB */
L_01CD87: TICK();
    goto L_01CEF8;  /* 01CD87 JMP */
L_01CD8A: TICK();
    mflag = 0;  /* 01CD8A CLM */
    A = rd16(ABS(0x0226));  /* 01CD8B LDA */
    mflag = 1;  /* 01CD8E SEM */
    /* 01CD8F CMP */
    if ((A & 0xFF) != 0x23) goto L_01CD9A;  /* 01CD91 BNE */
    wr8(ABS(0x0231), 0x0A);  /* 01CD93 LDM */
    goto L_01CEF8;  /* 01CD97 JMP */
L_01CD9A: TICK();
    mflag = 0;  /* 01CD9A CLM */
    A = rd16(ABS(0x0226));  /* 01CD9B LDA */
    mflag = 1;  /* 01CD9E SEM */
    /* 01CD9F CMP */
    if ((A & 0xFF) == 0x71) goto L_01CDA6;  /* 01CDA1 BEQ */
    goto L_01CE30;  /* 01CDA3 JMP */
L_01CDA6: TICK();
    wr8(ABS(0x0231), 0x10);  /* 01CDA6 LDM */
    Y = 0x0010;  /* 01CDAA LDY */
    mflag = 0;  /* 01CDAD CLM */
    S -= 3; F_01CC04();  /* 01CDAE JSRL */
    mflag = 1;  /* 01CDB2 SEM */
    if ((rd8(ABS(0x0335)) & 0x08) == 0) goto L_01CDBF;  /* 01CDB3 BBC */
    wr8(ABS(0x0252), 0x44);  /* 01CDB8 LDM */
    goto L_01CDC9;  /* 01CDBC JMP */
L_01CDBF: TICK();
    mflag = 1;  /* 01CDBF SEM */
    if ((rd8(ABS(0x0335)) & 0x01) == 0) goto L_01CDC9;  /* 01CDC0 BBC */
    wr8(ABS(0x0252), 0x54);  /* 01CDC5 LDM */
L_01CDC9: TICK();
    mflag = 0;  /* 01CDC9 CLM */
    B = 0x0000;  /* 01CDCA LDA */
    A = rd16(ABS(0x0224));  /* 01CDCE LDA */
    div16(0x0028);  /* 01CDD1 DIV */
    B = 0x0000;  /* 01CDD5 LDA */
    wr16(DP(0x09), A);  /* 01CDD9 STA */
    div16(0x0064);  /* 01CDDB DIV */
    C = 0;  /* 01CDDF CLC */
    mflag = 1;  /* 01CDE0 SEM */
    SETLO(A, adc8(A, 0x30));  /* 01CDE1 ADC */
    wr8(ABS(0x0254), (uint8_t)A);  /* 01CDE3 STA */
    mflag = 0;  /* 01CDE6 CLM */
    B = 0x0000;  /* 01CDE7 LDA */
    A = rd16(DP(0x09));  /* 01CDEB LDA */
    div16(0x000A);  /* 01CDED DIV */
    B = 0x0000;  /* 01CDF1 LDA */
    div16(0x000A);  /* 01CDF5 DIV */
    push16(B);  /* 01CDF9 PHB */
    A = pull16();  /* 01CDFB PLA */
    C = 0;  /* 01CDFC CLC */
    mflag = 1;  /* 01CDFD SEM */
    SETLO(A, adc8(A, 0x30));  /* 01CDFE ADC */
    wr8(ABS(0x0255), (uint8_t)A);  /* 01CE00 STA */
    mflag = 0;  /* 01CE03 CLM */
    B = 0x0000;  /* 01CE04 LDA */
    A = rd16(DP(0x09));  /* 01CE08 LDA */
    div16(0x000A);  /* 01CE0A DIV */
    push16(B);  /* 01CE0E PHB */
    A = pull16();  /* 01CE10 PLA */
    C = 0;  /* 01CE11 CLC */
    mflag = 1;  /* 01CE12 SEM */
    SETLO(A, adc8(A, 0x30));  /* 01CE13 ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 01CE15 STA */
    Y = 0x058E;  /* 01CE18 LDY */
    mflag = 0;  /* 01CE1B CLM */
    S -= 3; eeprom_read_byte_007189();  /* 01CE1C JSRL */
    SETLO(A, A - 1);  /* 01CE20 DEC */
    if ((A & 0xFF) != 0) goto L_01CE28;  /* 01CE21 BNE */
    SETLO(A, 0x32);  /* 01CE23 LDA */
    goto L_01CE2A;  /* 01CE25 JMP */
L_01CE28: TICK();
    SETLO(A, 0x31);  /* 01CE28 LDA */
L_01CE2A: TICK();
    wr8(ABS(0x0258), (uint8_t)A);  /* 01CE2A STA */
    goto L_01CEF8;  /* 01CE2D JMP */
L_01CE30: TICK();
    mflag = 0;  /* 01CE30 CLM */
    A = rd16(ABS(0x0226));  /* 01CE31 LDA */
    mflag = 1;  /* 01CE34 SEM */
    /* 01CE35 CMP */
    if ((A & 0xFF) == 0x64) goto L_01CE3C;  /* 01CE37 BEQ */
    goto L_01CEEB;  /* 01CE39 JMP */
L_01CE3C: TICK();
    SETLO(A, rd8(ABS(0x0283)));  /* 01CE3C LDA */
    /* 01CE3F CMP */
    if ((A & 0xFF) != 0x31) goto L_01CE67;  /* 01CE41 BNE */
    mflag = 0;  /* 01CE43 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_01CE57;  /* 01CE44 BBS */
    S -= 3; F_01D9C5();  /* 01CE4A JSRL */
    A = 0x0001;  /* 01CE4E LDA */
    mflag = 1;  /* 01CE51 SEM */
    wr8(LIND(0x06), (uint8_t)A);  /* 01CE52 STA */
    goto L_01CE64;  /* 01CE54 JMP */
L_01CE57: TICK();
    Y = 0x000D;  /* 01CE57 LDY */
    mflag = 0;  /* 01CE5A CLM */
    S -= 3; F_0080AA();  /* 01CE5B JSRL */
    mflag = 1;  /* 01CE5F SEM */
    wr8(ABS(0x0231), 0x09);  /* 01CE60 LDM */
L_01CE64: TICK();
    goto L_01CEE8;  /* 01CE64 JMP */
L_01CE67: TICK();
    mflag = 1;  /* 01CE67 SEM */
    /* 01CE68 CMP */
    if ((A & 0xFF) != 0x33) goto L_01CE7A;  /* 01CE6A BNE */
    mflag = 0;  /* 01CE6C CLM */
    S -= 3; F_01D9C5();  /* 01CE6D JSRL */
    A = 0x0002;  /* 01CE71 LDA */
    mflag = 1;  /* 01CE74 SEM */
    wr8(LIND(0x06), (uint8_t)A);  /* 01CE75 STA */
    goto L_01CEE8;  /* 01CE77 JMP */
L_01CE7A: TICK();
    mflag = 1;  /* 01CE7A SEM */
    /* 01CE7B CMP */
    if ((A & 0xFF) != 0x37) goto L_01CE8D;  /* 01CE7D BNE */
    mflag = 0;  /* 01CE7F CLM */
    S -= 3; F_01D9C5();  /* 01CE80 JSRL */
    A = 0x0003;  /* 01CE84 LDA */
    mflag = 1;  /* 01CE87 SEM */
    wr8(LIND(0x06), (uint8_t)A);  /* 01CE88 STA */
    goto L_01CEE8;  /* 01CE8A JMP */
L_01CE8D: TICK();
    mflag = 1;  /* 01CE8D SEM */
    /* 01CE8E CMP */
    if ((A & 0xFF) != 0x34) goto L_01CEE3;  /* 01CE90 BNE */
    mflag = 0;  /* 01CE92 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_01CED3;  /* 01CE93 BBS */
    A = rd16(ABS(0x0231));  /* 01CE99 LDA */
    mflag = 1;  /* 01CE9C SEM */
    /* 01CE9D CMP */
    if ((A & 0xFF) != 0x0B) goto L_01CECB;  /* 01CE9F BNE */
    SETLO(A, rd8(ABS(0x01EA)));  /* 01CEA1 LDA */
    /* 01CEA4 CMP */
    if ((A & 0xFF) != 0x05) goto L_01CEB5;  /* 01CEA6 BNE */
    wr8(ABS(0x01EA), 0x19);  /* 01CEA8 LDM */
    mflag = 0;  /* 01CEAC CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 01CEAD CLB */
    goto L_01CEC0;  /* 01CEB2 JMP */
L_01CEB5: TICK();
    mflag = 1;  /* 01CEB5 SEM */
    wr8(ABS(0x01EA), 0x05);  /* 01CEB6 LDM */
    mflag = 0;  /* 01CEBA CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 01CEBB SEB */
L_01CEC0: TICK();
    S -= 3; F_019852();  /* 01CEC0 JSRL */
    Y = rd16(ABS(0x01EA));  /* 01CEC4 LDY */
    S -= 3; F_019884();  /* 01CEC7 JSRL */
L_01CECB: TICK();
    mflag = 1;  /* 01CECB SEM */
    wr8(ABS(0x0231), 0x0B);  /* 01CECC LDM */
    goto L_01CEE0;  /* 01CED0 JMP */
L_01CED3: TICK();
    Y = 0x000D;  /* 01CED3 LDY */
    mflag = 0;  /* 01CED6 CLM */
    S -= 3; F_0080AA();  /* 01CED7 JSRL */
    mflag = 1;  /* 01CEDB SEM */
    wr8(ABS(0x0231), 0x09);  /* 01CEDC LDM */
L_01CEE0: TICK();
    goto L_01CEE8;  /* 01CEE0 JMP */
L_01CEE3: TICK();
    mflag = 1;  /* 01CEE3 SEM */
    wr8(ABS(0x0231), 0x09);  /* 01CEE4 LDM */
L_01CEE8: TICK();
    goto L_01CEF8;  /* 01CEE8 JMP */
L_01CEEB: TICK();
    mflag = 0;  /* 01CEEB CLM */
    A = rd16(ABS(0x0226));  /* 01CEEC LDA */
    mflag = 1;  /* 01CEEF SEM */
    /* 01CEF0 CMP */
    if ((A & 0xFF) != 0x2A) goto L_01CEF8;  /* 01CEF2 BNE */
    SETLO(A, 0x00);  /* 01CEF4 LDA */
    wr8(LIND(0x00), (uint8_t)A);  /* 01CEF6 STA */
L_01CEF8: TICK();
    mflag = 0;  /* 01CEF8 CLM */
    X = rd16(ABS(0x0214));  /* 01CEF9 LDX */
    if (X != 0) goto L_01CF03;  /* 01CEFC BNE */
    Y = X;  /* 01CEFE TXY */
    S -= 3; F_0080AA();  /* 01CEFF JSRL */
L_01CF03: TICK();
    A = rd16(ABS(0x0231));  /* 01CF03 LDA */
    mflag = 1;  /* 01CF06 SEM */
    /* 01CF07 CMP */
    if ((A & 0xFF) == 0x10) goto L_01CF13;  /* 01CF09 BEQ */
    Y = rd16(ABS(0x0231));  /* 01CF0B LDY */
    mflag = 0;  /* 01CF0E CLM */
    S -= 3; F_01CC04();  /* 01CF0F JSRL */
L_01CF13: TICK();
    mflag = 0;  /* 01CF13 CLM */
    A = rd16(LIND(0x03));  /* 01CF14 LDA */
    Y = A;  /* 01CF16 TAY */
    S -= 3; F_01CF78();  /* 01CF17 JSRL */
    mflag = 1;  /* 01CF1B SEM */
    wr8(ABS(0x0226), 0x0F);  /* 01CF1C LDM */
    wr8(ABS(0x0293), 0x00);  /* 01CF20 LDM */
L_01CF24: TICK();
    goto L_01CF74;  /* 01CF24 JMP */
L_01CF27: TICK();
    mflag = 0;  /* 01CF27 CLM */
    A = rd16(ABS(0x0231));  /* 01CF28 LDA */
    mflag = 1;  /* 01CF2B SEM */
    /* 01CF2C CMP */
    if ((A & 0xFF) == 0x09) goto L_01CF74;  /* 01CF2E BEQ */
    SETLO(A, rd8(ABS(0x0230)));  /* 01CF30 LDA */
    if ((A & 0xFF) != 0) goto L_01CF74;  /* 01CF33 BNE */
    SETLO(A, rd8(LIND(0x06)));  /* 01CF35 LDA */
    if ((A & 0xFF) != 0) goto L_01CF74;  /* 01CF37 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 01CF39 LDA */
    /* 01CF3C CMP */
    if ((A & 0xFF) >= 0x09) goto L_01CF60;  /* 01CF3E BCS */
    wr8(ABS(0x0230), 0x3F);  /* 01CF40 LDM */
    mflag = 0;  /* 01CF44 CLM */
    A &= 0x00FF;  /* 01CF45 AND */
    A += 1;  /* 01CF48 INC */
    B = 0x0009;  /* 01CF49 LDA */
    S -= 3; F_01E350();  /* 01CF4D JSRL */
    mflag = 1;  /* 01CF51 SEM */
    wr8(ABS(0x0231), (uint8_t)A);  /* 01CF52 STA */
    /* 01CF55 CMP */
    if ((A & 0xFF) != 0x00) goto L_01CF5D;  /* 01CF57 BNE */
    wr8(ABS(0x0231), 0x09);  /* 01CF59 LDM */
L_01CF5D: TICK();
    goto L_01CF65;  /* 01CF5D JMP */
L_01CF60: TICK();
    mflag = 1;  /* 01CF60 SEM */
    wr8(ABS(0x0231), 0x09);  /* 01CF61 LDM */
L_01CF65: TICK();
    Y = rd16(ABS(0x0231));  /* 01CF65 LDY */
    mflag = 0;  /* 01CF68 CLM */
    S -= 3; F_01CC04();  /* 01CF69 JSRL */
    A = rd16(LIND(0x03));  /* 01CF6D LDA */
    Y = A;  /* 01CF6F TAY */
    S -= 3; F_01CF78();  /* 01CF70 JSRL */
L_01CF74: TICK();
    mflag = 0;  /* 01CF74 CLM */
    Y = pull16();  /* 01CF75 PLY */
    D = pull16();  /* 01CF76 PLD */
    S += 3; return;  /* 01CF77 RTL */
}

/* 01CF78  callers: 01CB11, 01CD1A, 01CF17, 01CF70, 01D311, 01D518, 01D57D, 01D5AA ... */
void F_01CF78(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01CF78 LDA */
    push16(D);  /* 01CF7A PHD */
    D = A;  /* 01CF7B TAD */
    C = 0;  /* 01CF7C CLC */
    A = adc16(A, 0x001A);  /* 01CF7D ADC */
    push16(A);  /* 01CF80 PHA */
    wr16(DP(0x00), Y);  /* 01CF81 STY */
    wr16(DP(0x04), 0xF000);  /* 01CF83 LDM */
    if ((rd16(ABS(0x023C)) & 0x0400) == 0) goto L_01CF91;  /* 01CF87 BBC */
    wr16(DP(0x04), rd16(DP(0x04)) | 0x0100);  /* 01CF8D SEB */
L_01CF91: TICK();
    B = 0x0000;  /* 01CF91 LDA */
    A = Y;  /* 01CF95 TYA */
    div16(0x000A);  /* 01CF96 DIV */
    B = 0x0000;  /* 01CF9A LDA */
    div16(0x000A);  /* 01CF9E DIV */
    X = B;  /* 01CFA2 TBX */
    A = rd16((0x002D00 + X));  /* 01CFA4 LDA */
    A &= 0x00FF;  /* 01CFA8 AND */
    B = 0x0000;  /* 01CFAB LDA */
    wr16(DP(0x1A), A);  /* 01CFAF STA */
    A = Y;  /* 01CFB1 TYA */
    div16(0x000A);  /* 01CFB2 DIV */
    X = B;  /* 01CFB6 TBX */
    A = rd16((0x002D00 + X));  /* 01CFB8 LDA */
    A &= 0x00FF;  /* 01CFBC AND */
    A = rla16(A, 0x0008);  /* 01CFBF RLA */
    A &= 0xFF00;  /* 01CFC3 AND */
    C = 0;  /* 01CFC6 CLC */
    A = adc16(A, rd16(DP(0x1A)));  /* 01CFC7 ADC */
    wr16(DP(0x06), A);  /* 01CFC9 STA */
    B = 0x0000;  /* 01CFCB LDA */
    A = Y;  /* 01CFCF TYA */
    div16(0x0064);  /* 01CFD0 DIV */
    X = A;  /* 01CFD4 TAX */
    A = rd16((0x002D00 + X));  /* 01CFD5 LDA */
    A &= 0x00FF;  /* 01CFD9 AND */
    A = rla16(A, 0x0008);  /* 01CFDC RLA */
    A &= 0xFF00;  /* 01CFE0 AND */
    wr16(DP(0x08), A);  /* 01CFE3 STA */
    mflag = 1;  /* 01CFE5 SEM */
    wr8(DP(0x02), 0x03);  /* 01CFE6 LDM */
L_01CFE9: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 01CFE9 LDA */
    C = ((A & 0xFF) >= 0x0B);  /* 01CFEB CMP */
    if (!C) goto L_01CFF2;  /* 01CFED BCC */
    goto L_01D1DD;  /* 01CFEF JMP */
L_01CFF2: TICK();
    mflag = 0;  /* 01CFF2 CLM */
    A &= 0x00FF;  /* 01CFF3 AND */
    A = adc16(A, 0xFFFD);  /* 01CFF6 ADC */
    X = A;  /* 01CFF9 TAX */
    A = rd16((ABS(0x0252) + X));  /* 01CFFA LDA */
    mflag = 1;  /* 01CFFD SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 01CFFE STA */
    /* 01D000 CMP */
    if ((A & 0xFF) >= 0x5B) goto L_01D029;  /* 01D002 BCS */
    /* 01D004 CMP */
    if ((A & 0xFF) < 0x41) goto L_01D029;  /* 01D006 BCC */
    mflag = 0;  /* 01D008 CLM */
    A &= 0x00FF;  /* 01D009 AND */
    A = asl16(A);  /* 01D00C ASL */
    C = 0;  /* 01D00D CLC */
    A = adc16(A, 0xFF7E);  /* 01D00E ADC */
    X = A;  /* 01D011 TAX */
    A = rd16(DP(0x02));  /* 01D012 LDA */
    A &= 0x00FF;  /* 01D014 AND */
    A = asl16(A);  /* 01D017 ASL */
    wr16(DP(0x1A), A);  /* 01D018 STA */
    A = rd16((0x002C9C + X));  /* 01D01A LDA */
    wr16(DP(0x1C), A);  /* 01D01E STA */
    X = rd16(DP(0x1A));  /* 01D020 LDX */
    A = rd16(DP(0x1C));  /* 01D022 LDA */
    wr16(DP(0x04 + X), A);  /* 01D024 STA */
    goto L_01D1D7;  /* 01D026 JMP */
L_01D029: TICK();
    mflag = 0;  /* 01D029 CLM */
    A = rd16(DP(0x03));  /* 01D02A LDA */
    mflag = 1;  /* 01D02C SEM */
    /* 01D02D CMP */
    if ((A & 0xFF) >= 0x3A) goto L_01D058;  /* 01D02F BCS */
    SETLO(A, rd8(DP(0x03)));  /* 01D031 LDA */
    /* 01D033 CMP */
    if ((A & 0xFF) < 0x30) goto L_01D058;  /* 01D035 BCC */
    mflag = 0;  /* 01D037 CLM */
    A &= 0x00FF;  /* 01D038 AND */
    A = asl16(A);  /* 01D03B ASL */
    C = 0;  /* 01D03C CLC */
    A = adc16(A, 0xFFA0);  /* 01D03D ADC */
    X = A;  /* 01D040 TAX */
    A = rd16(DP(0x02));  /* 01D041 LDA */
    A &= 0x00FF;  /* 01D043 AND */
    A = asl16(A);  /* 01D046 ASL */
    wr16(DP(0x1A), A);  /* 01D047 STA */
    A = rd16((0x002CD0 + X));  /* 01D049 LDA */
    wr16(DP(0x1C), A);  /* 01D04D STA */
    X = rd16(DP(0x1A));  /* 01D04F LDX */
    A = rd16(DP(0x1C));  /* 01D051 LDA */
    wr16(DP(0x04 + X), A);  /* 01D053 STA */
    goto L_01D1D7;  /* 01D055 JMP */
L_01D058: TICK();
    mflag = 0;  /* 01D058 CLM */
    A = rd16(DP(0x03));  /* 01D059 LDA */
    mflag = 1;  /* 01D05B SEM */
    /* 01D05C CMP */
    if ((A & 0xFF) != 0x2A) goto L_01D071;  /* 01D05E BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D060 LDA */
    mflag = 0;  /* 01D062 CLM */
    A &= 0x00FF;  /* 01D063 AND */
    A = asl16(A);  /* 01D066 ASL */
    X = A;  /* 01D067 TAX */
    A = rd16(0x002CE4);  /* 01D068 LDA */
    wr16(DP(0x04 + X), A);  /* 01D06C STA */
    goto L_01D1D7;  /* 01D06E JMP */
L_01D071: TICK();
    mflag = 0;  /* 01D071 CLM */
    A = rd16(DP(0x03));  /* 01D072 LDA */
    mflag = 1;  /* 01D074 SEM */
    /* 01D075 CMP */
    if ((A & 0xFF) != 0x3C) goto L_01D08A;  /* 01D077 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D079 LDA */
    mflag = 0;  /* 01D07B CLM */
    A &= 0x00FF;  /* 01D07C AND */
    A = asl16(A);  /* 01D07F ASL */
    X = A;  /* 01D080 TAX */
    A = rd16(0x002CE6);  /* 01D081 LDA */
    wr16(DP(0x04 + X), A);  /* 01D085 STA */
    goto L_01D1D7;  /* 01D087 JMP */
L_01D08A: TICK();
    mflag = 0;  /* 01D08A CLM */
    A = rd16(DP(0x03));  /* 01D08B LDA */
    mflag = 1;  /* 01D08D SEM */
    /* 01D08E CMP */
    if ((A & 0xFF) != 0x5F) goto L_01D0A3;  /* 01D090 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D092 LDA */
    mflag = 0;  /* 01D094 CLM */
    A &= 0x00FF;  /* 01D095 AND */
    A = asl16(A);  /* 01D098 ASL */
    X = A;  /* 01D099 TAX */
    A = rd16(0x002CE8);  /* 01D09A LDA */
    wr16(DP(0x04 + X), A);  /* 01D09E STA */
    goto L_01D1D7;  /* 01D0A0 JMP */
L_01D0A3: TICK();
    mflag = 0;  /* 01D0A3 CLM */
    A = rd16(DP(0x03));  /* 01D0A4 LDA */
    mflag = 1;  /* 01D0A6 SEM */
    /* 01D0A7 CMP */
    if ((A & 0xFF) != 0x2D) goto L_01D0BC;  /* 01D0A9 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D0AB LDA */
    mflag = 0;  /* 01D0AD CLM */
    A &= 0x00FF;  /* 01D0AE AND */
    A = asl16(A);  /* 01D0B1 ASL */
    X = A;  /* 01D0B2 TAX */
    A = rd16(0x002CEA);  /* 01D0B3 LDA */
    wr16(DP(0x04 + X), A);  /* 01D0B7 STA */
    goto L_01D1D7;  /* 01D0B9 JMP */
L_01D0BC: TICK();
    mflag = 0;  /* 01D0BC CLM */
    A = rd16(DP(0x03));  /* 01D0BD LDA */
    mflag = 1;  /* 01D0BF SEM */
    /* 01D0C0 CMP */
    if ((A & 0xFF) != 0x3A) goto L_01D0D5;  /* 01D0C2 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D0C4 LDA */
    mflag = 0;  /* 01D0C6 CLM */
    A &= 0x00FF;  /* 01D0C7 AND */
    A = asl16(A);  /* 01D0CA ASL */
    X = A;  /* 01D0CB TAX */
    A = rd16(0x002CEC);  /* 01D0CC LDA */
    wr16(DP(0x04 + X), A);  /* 01D0D0 STA */
    goto L_01D1D7;  /* 01D0D2 JMP */
L_01D0D5: TICK();
    mflag = 0;  /* 01D0D5 CLM */
    A = rd16(DP(0x03));  /* 01D0D6 LDA */
    mflag = 1;  /* 01D0D8 SEM */
    /* 01D0D9 CMP */
    if ((A & 0xFF) != 0x2F) goto L_01D0EE;  /* 01D0DB BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D0DD LDA */
    mflag = 0;  /* 01D0DF CLM */
    A &= 0x00FF;  /* 01D0E0 AND */
    A = asl16(A);  /* 01D0E3 ASL */
    X = A;  /* 01D0E4 TAX */
    A = rd16(0x002CEE);  /* 01D0E5 LDA */
    wr16(DP(0x04 + X), A);  /* 01D0E9 STA */
    goto L_01D1D7;  /* 01D0EB JMP */
L_01D0EE: TICK();
    mflag = 0;  /* 01D0EE CLM */
    A = rd16(DP(0x03));  /* 01D0EF LDA */
    mflag = 1;  /* 01D0F1 SEM */
    /* 01D0F2 CMP */
    if ((A & 0xFF) != 0x2B) goto L_01D107;  /* 01D0F4 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D0F6 LDA */
    mflag = 0;  /* 01D0F8 CLM */
    A &= 0x00FF;  /* 01D0F9 AND */
    A = asl16(A);  /* 01D0FC ASL */
    X = A;  /* 01D0FD TAX */
    A = rd16(0x002CF0);  /* 01D0FE LDA */
    wr16(DP(0x04 + X), A);  /* 01D102 STA */
    goto L_01D1D7;  /* 01D104 JMP */
L_01D107: TICK();
    mflag = 0;  /* 01D107 CLM */
    A = rd16(DP(0x03));  /* 01D108 LDA */
    mflag = 1;  /* 01D10A SEM */
    /* 01D10B CMP */
    if ((A & 0xFF) != 0x5E) goto L_01D120;  /* 01D10D BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D10F LDA */
    mflag = 0;  /* 01D111 CLM */
    A &= 0x00FF;  /* 01D112 AND */
    A = asl16(A);  /* 01D115 ASL */
    X = A;  /* 01D116 TAX */
    A = rd16(0x002CF4);  /* 01D117 LDA */
    wr16(DP(0x04 + X), A);  /* 01D11B STA */
    goto L_01D1D7;  /* 01D11D JMP */
L_01D120: TICK();
    mflag = 0;  /* 01D120 CLM */
    A = rd16(DP(0x03));  /* 01D121 LDA */
    mflag = 1;  /* 01D123 SEM */
    /* 01D124 CMP */
    if ((A & 0xFF) != 0x76) goto L_01D139;  /* 01D126 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D128 LDA */
    mflag = 0;  /* 01D12A CLM */
    A &= 0x00FF;  /* 01D12B AND */
    A = asl16(A);  /* 01D12E ASL */
    X = A;  /* 01D12F TAX */
    A = rd16(0x002CF6);  /* 01D130 LDA */
    wr16(DP(0x04 + X), A);  /* 01D134 STA */
    goto L_01D1D7;  /* 01D136 JMP */
L_01D139: TICK();
    mflag = 0;  /* 01D139 CLM */
    A = rd16(DP(0x03));  /* 01D13A LDA */
    mflag = 1;  /* 01D13C SEM */
    /* 01D13D CMP */
    if ((A & 0xFF) != 0x6A) goto L_01D152;  /* 01D13F BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D141 LDA */
    mflag = 0;  /* 01D143 CLM */
    A &= 0x00FF;  /* 01D144 AND */
    A = asl16(A);  /* 01D147 ASL */
    X = A;  /* 01D148 TAX */
    A = rd16(0x002CF8);  /* 01D149 LDA */
    wr16(DP(0x04 + X), A);  /* 01D14D STA */
    goto L_01D1D7;  /* 01D14F JMP */
L_01D152: TICK();
    mflag = 0;  /* 01D152 CLM */
    A = rd16(DP(0x03));  /* 01D153 LDA */
    mflag = 1;  /* 01D155 SEM */
    /* 01D156 CMP */
    if ((A & 0xFF) != 0x64) goto L_01D16B;  /* 01D158 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D15A LDA */
    mflag = 0;  /* 01D15C CLM */
    A &= 0x00FF;  /* 01D15D AND */
    A = asl16(A);  /* 01D160 ASL */
    X = A;  /* 01D161 TAX */
    A = rd16(0x002CFA);  /* 01D162 LDA */
    wr16(DP(0x04 + X), A);  /* 01D166 STA */
    goto L_01D1D7;  /* 01D168 JMP */
L_01D16B: TICK();
    mflag = 0;  /* 01D16B CLM */
    A = rd16(DP(0x03));  /* 01D16C LDA */
    mflag = 1;  /* 01D16E SEM */
    /* 01D16F CMP */
    if ((A & 0xFF) != 0x6D) goto L_01D184;  /* 01D171 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D173 LDA */
    mflag = 0;  /* 01D175 CLM */
    A &= 0x00FF;  /* 01D176 AND */
    A = asl16(A);  /* 01D179 ASL */
    X = A;  /* 01D17A TAX */
    A = rd16(0x002CFC);  /* 01D17B LDA */
    wr16(DP(0x04 + X), A);  /* 01D17F STA */
    goto L_01D1D7;  /* 01D181 JMP */
L_01D184: TICK();
    mflag = 0;  /* 01D184 CLM */
    A = rd16(DP(0x03));  /* 01D185 LDA */
    mflag = 1;  /* 01D187 SEM */
    /* 01D188 CMP */
    if ((A & 0xFF) != 0x65) goto L_01D19D;  /* 01D18A BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D18C LDA */
    mflag = 0;  /* 01D18E CLM */
    A &= 0x00FF;  /* 01D18F AND */
    A = asl16(A);  /* 01D192 ASL */
    X = A;  /* 01D193 TAX */
    A = rd16(0x002CFE);  /* 01D194 LDA */
    wr16(DP(0x04 + X), A);  /* 01D198 STA */
    goto L_01D1D7;  /* 01D19A JMP */
L_01D19D: TICK();
    mflag = 0;  /* 01D19D CLM */
    A = rd16(DP(0x03));  /* 01D19E LDA */
    mflag = 1;  /* 01D1A0 SEM */
    /* 01D1A1 CMP */
    if ((A & 0xFF) != 0x75) goto L_01D1B4;  /* 01D1A3 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D1A5 LDA */
    mflag = 0;  /* 01D1A7 CLM */
    A &= 0x00FF;  /* 01D1A8 AND */
    A = asl16(A);  /* 01D1AB ASL */
    X = A;  /* 01D1AC TAX */
    wr16(DP(0x04 + X), 0x4126);  /* 01D1AD LDM */
    goto L_01D1D7;  /* 01D1B1 JMP */
L_01D1B4: TICK();
    mflag = 0;  /* 01D1B4 CLM */
    A = rd16(DP(0x03));  /* 01D1B5 LDA */
    mflag = 1;  /* 01D1B7 SEM */
    /* 01D1B8 CMP */
    if ((A & 0xFF) != 0x23) goto L_01D1CB;  /* 01D1BA BNE */
    SETLO(A, rd8(DP(0x02)));  /* 01D1BC LDA */
    mflag = 0;  /* 01D1BE CLM */
    A &= 0x00FF;  /* 01D1BF AND */
    A = asl16(A);  /* 01D1C2 ASL */
    X = A;  /* 01D1C3 TAX */
    wr16(DP(0x04 + X), 0xA152);  /* 01D1C4 LDM */
    goto L_01D1D7;  /* 01D1C8 JMP */
L_01D1CB: TICK();
    mflag = 0;  /* 01D1CB CLM */
    A = rd16(DP(0x02));  /* 01D1CC LDA */
    A &= 0x00FF;  /* 01D1CE AND */
    A = asl16(A);  /* 01D1D1 ASL */
    X = A;  /* 01D1D2 TAX */
    wr16(DP(0x04 + X), 0x0000);  /* 01D1D3 LDM */
L_01D1D7: TICK();
    mflag = 1;  /* 01D1D7 SEM */
    t = (rd8(DP(0x02)) + 1) & 0xFF; wr8(DP(0x02), t);  /* 01D1D8 INC */
    goto L_01CFE9;  /* 01D1DA JMP */
L_01D1DD: TICK();
    mflag = 1;  /* 01D1DD SEM */
    if ((rd8(ABS(0x023C)) & 0x02) == 0) goto L_01D1E8;  /* 01D1DE BBC */
    mflag = 0;  /* 01D1E3 CLM */
    wr16(DP(0x08), rd16(DP(0x08)) | 0x0010);  /* 01D1E4 SEB */
L_01D1E8: TICK();
    mflag = 0;  /* 01D1E8 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_01D1F3;  /* 01D1E9 BBC */
    wr16(DP(0x0A), rd16(DP(0x0A)) | 0x0001);  /* 01D1EF SEB */
L_01D1F3: TICK();
    mflag = 1;  /* 01D1F3 SEM */
    if ((rd8(ABS(0x023C)) & 0x08) == 0) goto L_01D1FE;  /* 01D1F4 BBC */
    mflag = 0;  /* 01D1F9 CLM */
    wr16(DP(0x0C), rd16(DP(0x0C)) | 0x0001);  /* 01D1FA SEB */
L_01D1FE: TICK();
    mflag = 1;  /* 01D1FE SEM */
    if ((rd8(ABS(0x023C)) & 0x10) == 0) goto L_01D209;  /* 01D1FF BBC */
    mflag = 0;  /* 01D204 CLM */
    wr16(DP(0x0E), rd16(DP(0x0E)) | 0x0001);  /* 01D205 SEB */
L_01D209: TICK();
    mflag = 1;  /* 01D209 SEM */
    if ((rd8(ABS(0x023C)) & 0x20) == 0) goto L_01D214;  /* 01D20A BBC */
    mflag = 0;  /* 01D20F CLM */
    wr16(DP(0x10), rd16(DP(0x10)) | 0x0001);  /* 01D210 SEB */
L_01D214: TICK();
    mflag = 1;  /* 01D214 SEM */
    SETLO(A, 0x01);  /* 01D215 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 01D217 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 01D218 STA */
    setP(pull16());  /* 01D21A PLP */
    mflag = 0;  /* 01D21B CLM */
    S -= 3; i2c_start_007172();  /* 01D21C JSRL */
    Y = 0x0070;  /* 01D220 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01D223 JSRL */
    Y = 0x00E4;  /* 01D227 LDY */
    mflag = 0;  /* 01D22A CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01D22B JSRL */
    Y = 0x00C8;  /* 01D22F LDY */
    mflag = 0;  /* 01D232 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01D233 JSRL */
    wr8(DP(0x02), 0x00);  /* 01D237 LDM */
L_01D23A: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 01D23A LDA */
    /* 01D23C CMP */
    if ((A & 0xFF) >= 0x0B) goto L_01D263;  /* 01D23E BCS */
    mflag = 0;  /* 01D240 CLM */
    A &= 0x00FF;  /* 01D241 AND */
    A = asl16(A);  /* 01D244 ASL */
    X = A;  /* 01D245 TAX */
    A = rd16(DP(0x05 + X));  /* 01D246 LDA */
    A &= 0x00FF;  /* 01D248 AND */
    Y = A;  /* 01D24B TAY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01D24C JSRL */
    SETLO(A, rd8(DP(0x02)));  /* 01D250 LDA */
    mflag = 0;  /* 01D252 CLM */
    A &= 0x00FF;  /* 01D253 AND */
    A = asl16(A);  /* 01D256 ASL */
    X = A;  /* 01D257 TAX */
    Y = rd16(DP(0x04 + X));  /* 01D258 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01D25A JSRL */
    t = (rd8(DP(0x02)) + 1) & 0xFF; wr8(DP(0x02), t);  /* 01D25E INC */
    goto L_01D23A;  /* 01D260 JMP */
L_01D263: TICK();
    mflag = 0;  /* 01D263 CLM */
    S -= 3; i2c_stop_00715B();  /* 01D264 JSRL */
    mflag = 1;  /* 01D268 SEM */
    SETLO(A, 0x00);  /* 01D269 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 01D26B PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 01D26C STA */
    setP(pull16());  /* 01D26E PLP */
    mflag = 0;  /* 01D26F CLM */
    Y = pull16();  /* 01D270 PLY */
    D = pull16();  /* 01D271 PLD */
    S += 3; return;  /* 01D272 RTL */
}

/* 01D273  callers: 01CB5A */
void F_01D273(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D273 LDA */
    push16(D);  /* 01D275 PHD */
    D = A;  /* 01D276 TAD */
    C = 0;  /* 01D277 CLC */
    A = adc16(A, 0x0010);  /* 01D278 ADC */
    push16(A);  /* 01D27B PHA */
    wr16(DP(0x08), Y);  /* 01D27C STY */
    wr16(DP(0x0A), B);  /* 01D27E STA */
    S -= 3; measurements_0198D5();  /* 01D281 JSRL */
    wr8(DP(0x0F), (uint8_t)A);  /* 01D285 STA */
    mflag = 0;  /* 01D287 CLM */
    wr16(DP(0x0B), 0x0000);  /* 01D288 LDM */
    A = rd16(ABS(0x0116));  /* 01D28C LDA */
    mflag = 1;  /* 01D28F SEM */
    /* 01D290 CMP */
    if ((A & 0xFF) == 0x09) goto L_01D297;  /* 01D292 BEQ */
    goto L_01D349;  /* 01D294 JMP */
L_01D297: TICK();
    SETLO(A, rd8(ABS(0x0080)));  /* 01D297 LDA */
    SETLO(A, A & 0x02);  /* 01D29A AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_01D2A0;  /* 01D29C BEQ */
    SETLO(A, 0x01);  /* 01D29E LDA */
    Z = ((A & 0xFF) == 0);
L_01D2A0: TICK();
    if (!Z) goto L_01D2A5;  /* 01D2A0 BNE */
    goto L_01D333;  /* 01D2A2 JMP */
L_01D2A5: TICK();
    SETLO(A, rd8(ABS(0x0111)));  /* 01D2A5 LDA */
    if ((A & 0xFF) == 0) goto L_01D2AD;  /* 01D2A8 BEQ */
    goto L_01D330;  /* 01D2AA JMP */
L_01D2AD: TICK();
    wr8(ABS(0x0116), 0x08);  /* 01D2AD LDM */
    mflag = 0;  /* 01D2B1 CLM */
    S -= 3; F_0195DA();  /* 01D2B2 JSRL */
    mflag = 1;  /* 01D2B6 SEM */
    SETLO(A, rd8(ABS(0x0230)));  /* 01D2B7 LDA */
    if ((A & 0xFF) != 0) goto L_01D330;  /* 01D2BA BNE */
    SETLO(A, rd8(LIND(0x08)));  /* 01D2BC LDA */
    if ((A & 0xFF) == 0) goto L_01D330;  /* 01D2BE BEQ */
    mflag = 0;  /* 01D2C0 CLM */
    if ((rd16(ABS(0x023C)) & 0x0020) == 0x0020) goto L_01D2CD;  /* 01D2C1 BBS */
    if ((rd16(ABS(0x023C)) & 0x0008) == 0x0008) goto L_01D2DB;  /* 01D2C7 BBS */
L_01D2CD: TICK();
    wr16(DP(0x0B), 0x0001);  /* 01D2CD LDM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0008);  /* 01D2D1 SEB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 01D2D6 CLB */
L_01D2DB: TICK();
    A = rd16(LIND(0x08));  /* 01D2DB LDA */
    mflag = 1;  /* 01D2DD SEM */
    /* 01D2DE CMP */
    if ((A & 0xFF) == 0x04) goto L_01D2F2;  /* 01D2E0 BEQ */
    wr8(ABS(0x0231), 0x0C);  /* 01D2E2 LDM */
    wr8(ABS(0x0230), 0x5E);  /* 01D2E6 LDM */
    Y = 0x0012;  /* 01D2EA LDY */
    mflag = 0;  /* 01D2ED CLM */
    S -= 3; F_0080AA();  /* 01D2EE JSRL */
L_01D2F2: TICK();
    mflag = 0;  /* 01D2F2 CLM */
    A = rd16(LIND(0x08));  /* 01D2F3 LDA */
    mflag = 1;  /* 01D2F5 SEM */
    SETLO(A, A - 1);  /* 01D2F6 DEC */
    if ((A & 0xFF) != 0) goto L_01D327;  /* 01D2F7 BNE */
    mflag = 0;  /* 01D2F9 CLM */
    if ((rd16(ABS(0x023C)) & 0x0010) == 0x0010) goto L_01D327;  /* 01D2FA BBS */
    mflag = 1;  /* 01D300 SEM */
    wr8(ABS(0x0231), 0x0D);  /* 01D301 LDM */
    mflag = 0;  /* 01D305 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 01D306 SEB */
    Y = 0x0002;  /* 01D30B LDY */
    A = rd16(LIND(0x02));  /* 01D30E LDA */
    Y = A;  /* 01D310 TAY */
    S -= 3; F_01CF78();  /* 01D311 JSRL */
    S -= 3; F_019852();  /* 01D315 JSRL */
    Y = 0x0005;  /* 01D319 LDY */
    S -= 3; F_019884();  /* 01D31C JSRL */
    S -= 3; F_0195DA();  /* 01D320 JSRL */
    goto L_01D330;  /* 01D324 JMP */
L_01D327: TICK();
    mflag = 0;  /* 01D327 CLM */
    A = 0x0000;  /* 01D328 LDA */
    mflag = 1;  /* 01D32B SEM */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D32C STA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D32E STA */
L_01D330: TICK();
    goto L_01D346;  /* 01D330 JMP */
L_01D333: TICK();
    mflag = 1;  /* 01D333 SEM */
    SETLO(A, rd8(ABS(0x0230)));  /* 01D334 LDA */
    if ((A & 0xFF) != 0) goto L_01D346;  /* 01D337 BNE */
    SETLO(A, rd8(LIND(0x08)));  /* 01D339 LDA */
    if ((A & 0xFF) == 0) goto L_01D346;  /* 01D33B BEQ */
    wr8(ABS(0x0116), 0x08);  /* 01D33D LDM */
    mflag = 0;  /* 01D341 CLM */
    S -= 3; F_0195DA();  /* 01D342 JSRL */
L_01D346: TICK();
    goto L_01D7ED;  /* 01D346 JMP */
L_01D349: TICK();
    mflag = 0;  /* 01D349 CLM */
    A = rd16(ABS(0x0116));  /* 01D34A LDA */
    mflag = 1;  /* 01D34D SEM */
    /* 01D34E CMP */
    if ((A & 0xFF) == 0x0A) goto L_01D355;  /* 01D350 BEQ */
    goto L_01D7ED;  /* 01D352 JMP */
L_01D355: TICK();
    SETLO(A, rd8(LIND(0x08)));  /* 01D355 LDA */
    if ((A & 0xFF) != 0) goto L_01D35C;  /* 01D357 BNE */
    goto L_01D3F5;  /* 01D359 JMP */
L_01D35C: TICK();
    mflag = 0;  /* 01D35C CLM */
    if ((rd16(ABS(0x023C)) & 0x0020) == 0) goto L_01D386;  /* 01D35D BBC */
    mflag = 1;  /* 01D363 SEM */
    SETLO(A, rd8(ABS(0x0230)));  /* 01D364 LDA */
    if ((A & 0xFF) != 0) goto L_01D383;  /* 01D367 BNE */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D369 STA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D36B STA */
    SETLO(A, rd8(ABS(0x0231)));  /* 01D36D LDA */
    /* 01D370 CMP */
    if ((A & 0xFF) == 0x09) goto L_01D37D;  /* 01D372 BEQ */
    wr8(ABS(0x0231), 0x09);  /* 01D374 LDM */
    mflag = 0;  /* 01D378 CLM */
    wr16(DP(0x0B), 0x0001);  /* 01D379 LDM */
L_01D37D: TICK();
    mflag = 0;  /* 01D37D CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0400);  /* 01D37E CLB */
L_01D383: TICK();
    goto L_01D3F5;  /* 01D383 JMP */
L_01D386: TICK();
    mflag = 0;  /* 01D386 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0008);  /* 01D387 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0020);  /* 01D38C SEB */
    wr16(DP(0x0B), 0x0001);  /* 01D391 LDM */
    mflag = 1;  /* 01D395 SEM */
    wr8(ABS(0x0231), 0x09);  /* 01D396 LDM */
    wr8(ABS(0x0230), 0x00);  /* 01D39A LDM */
    SETLO(A, rd8(LIND(0x08)));  /* 01D39E LDA */
    /* 01D3A0 CMP */
    if ((A & 0xFF) != 0x03) goto L_01D3AD;  /* 01D3A2 BNE */
    SETLO(A, 0x00);  /* 01D3A4 LDA */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D3A6 STA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D3A8 STA */
    goto L_01D3F5;  /* 01D3AA JMP */
L_01D3AD: TICK();
    mflag = 0;  /* 01D3AD CLM */
    A = rd16(LIND(0x08));  /* 01D3AE LDA */
    mflag = 1;  /* 01D3B0 SEM */
    /* 01D3B1 CMP */
    if ((A & 0xFF) != 0x02) goto L_01D3D5;  /* 01D3B3 BNE */
    SETLO(A, 0x01);  /* 01D3B5 LDA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D3B7 STA */
    Y = 0x0002;  /* 01D3B9 LDY */
    mflag = 0;  /* 01D3BC CLM */
    A = 0x0000;  /* 01D3BD LDA */
    wr16((LIND(0x02) + Y), A);  /* 01D3C0 STA */
    wr16(LIND(0x02), A);  /* 01D3C2 STA */
    mflag = 1;  /* 01D3C4 SEM */
    wr8(DP(0x0F), 0x01);  /* 01D3C5 LDM */
    wr8(ABS(0x0231), 0x0E);  /* 01D3C8 LDM */
    mflag = 0;  /* 01D3CC CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0400);  /* 01D3CD SEB */
    goto L_01D3F5;  /* 01D3D2 JMP */
L_01D3D5: TICK();
    mflag = 0;  /* 01D3D5 CLM */
    A = rd16(LIND(0x08));  /* 01D3D6 LDA */
    mflag = 1;  /* 01D3D8 SEM */
    SETLO(A, A - 1);  /* 01D3D9 DEC */
    if ((A & 0xFF) != 0) goto L_01D3F5;  /* 01D3DA BNE */
    SETLO(A, A + 1);  /* 01D3DC INC */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D3DD STA */
    Y = 0x0002;  /* 01D3DF LDY */
    mflag = 0;  /* 01D3E2 CLM */
    A = 0x0000;  /* 01D3E3 LDA */
    wr16((LIND(0x02) + Y), A);  /* 01D3E6 STA */
    A = 0x001E;  /* 01D3E8 LDA */
    wr16(LIND(0x02), A);  /* 01D3EB STA */
    mflag = 1;  /* 01D3ED SEM */
    wr8(DP(0x0F), 0x01);  /* 01D3EE LDM */
    wr8(ABS(0x0231), 0x0F);  /* 01D3F1 LDM */
L_01D3F5: TICK();
    mflag = 0;  /* 01D3F5 CLM */
    A = rd16(LIND(0x08));  /* 01D3F6 LDA */
    mflag = 1;  /* 01D3F8 SEM */
    /* 01D3F9 CMP */
    if ((A & 0xFF) == 0x02) goto L_01D400;  /* 01D3FB BEQ */
    goto L_01D4EC;  /* 01D3FD JMP */
L_01D400: TICK();
    SETLO(A, rd8(DP(0x0F)));  /* 01D400 LDA */
    /* 01D402 CMP */
    if ((A & 0xFF) == 0x01) goto L_01D409;  /* 01D404 BEQ */
    goto L_01D4E9;  /* 01D406 JMP */
L_01D409: TICK();
    wr8(ABS(0x0230), 0x5E);  /* 01D409 LDM */
    Y = 0x0002;  /* 01D40D LDY */
    mflag = 0;  /* 01D410 CLM */
    A = rd16((LIND(0x02) + Y));  /* 01D411 LDA */
    C = (A >= 0x0000);  /* 01D413 CMP */
    Z = (A == 0x0000);
    if (!Z) goto L_01D41D;  /* 01D416 BNE */
    A = rd16(LIND(0x02));  /* 01D418 LDA */
    C = (A >= 0x0800);  /* 01D41A CMP */
L_01D41D: TICK();
    if (C) goto L_01D467;  /* 01D41D BCS */
    A = Y;  /* 01D41F TYA */
    mflag = 1;  /* 01D420 SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D421 STA */
    wr8(DP(0x10), 0x80);  /* 01D423 LDM */
    mflag = 0;  /* 01D426 CLM */
    wr16(DP(0x11), 0x008D);  /* 01D427 LDM */
    wr16(DP(0x13), 0x0000);  /* 01D42B LDM */
    A = rd16(LIND(0x02));  /* 01D42F LDA */
    wr16(DP(0x14), A);  /* 01D431 STA */
    Y = 0x00A0;  /* 01D433 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 01D436 JSRL */
    wr16(DP(0x10), 0x0001);  /* 01D43A LDM */
    wr16(DP(0x11), 0x0080);  /* 01D43E LDM */
    Y = 0x0002;  /* 01D442 LDY */
    B = rd16((LIND(0x02) + Y));  /* 01D445 LDA */
    A = rd16(LIND(0x02));  /* 01D448 LDA */
    Y = A;  /* 01D44A TAY */
    S -= 3; F_01D837();  /* 01D44B JSRL */
    Y = 0x0002;  /* 01D44F LDY */
    A = rd16(LIND(0x02));  /* 01D452 LDA */
    B = rd16((LIND(0x02) + Y));  /* 01D454 LDA */
    C = 0;  /* 01D457 CLC */
    A = adc16(A, 0x0080);  /* 01D458 ADC */
    B = adc16(B, 0x0000);  /* 01D45B ADC */
    wr16(LIND(0x02), A);  /* 01D45F STA */
    wr16((LIND(0x02) + Y), B);  /* 01D461 STA */
    goto L_01D4E9;  /* 01D464 JMP */
L_01D467: TICK();
    mflag = 0;  /* 01D467 CLM */
    A = rd16(LIND(0x05));  /* 01D468 LDA */
    mflag = 1;  /* 01D46A SEM */
    /* 01D46B CMP */
    if ((A & 0xFF) != 0x02) goto L_01D492;  /* 01D46D BNE */
    Y = rd16(DP(0x00));  /* 01D46F LDY */
    mflag = 0;  /* 01D471 CLM */
    S -= 3; F_01D8D9();  /* 01D472 JSRL */
    wr16(DP(0x10), 0x0001);  /* 01D476 LDM */
    wr16(DP(0x11), 0x0002);  /* 01D47A LDM */
    B = 0x0000;  /* 01D47E LDA */
    Y = 0x0589;  /* 01D482 LDY */
    S -= 3; F_01D837();  /* 01D485 JSRL */
    A = 0x0003;  /* 01D489 LDA */
    mflag = 1;  /* 01D48C SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D48D STA */
    goto L_01D4E9;  /* 01D48F JMP */
L_01D492: TICK();
    mflag = 1;  /* 01D492 SEM */
    /* 01D493 CMP */
    if ((A & 0xFF) != 0x03) goto L_01D4BA;  /* 01D495 BNE */
    Y = rd16(DP(0x00));  /* 01D497 LDY */
    mflag = 0;  /* 01D499 CLM */
    S -= 3; F_01D954();  /* 01D49A JSRL */
    wr16(DP(0x10), 0x0001);  /* 01D49E LDM */
    wr16(DP(0x11), 0x0002);  /* 01D4A2 LDM */
    B = 0x0000;  /* 01D4A6 LDA */
    Y = 0x05BD;  /* 01D4AA LDY */
    S -= 3; F_01D837();  /* 01D4AD JSRL */
    A = 0x0004;  /* 01D4B1 LDA */
    mflag = 1;  /* 01D4B4 SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D4B5 STA */
    goto L_01D4E9;  /* 01D4B7 JMP */
L_01D4BA: TICK();
    mflag = 1;  /* 01D4BA SEM */
    /* 01D4BB CMP */
    if ((A & 0xFF) != 0x04) goto L_01D4E9;  /* 01D4BD BNE */
    mflag = 0;  /* 01D4BF CLM */
    S -= 3; F_01986E();  /* 01D4C0 JSRL */
    A = 0x0000;  /* 01D4C4 LDA */
    mflag = 1;  /* 01D4C7 SEM */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D4C8 STA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D4CA STA */
    Y = 0x0002;  /* 01D4CC LDY */
    mflag = 0;  /* 01D4CF CLM */
    A = 0x0000;  /* 01D4D0 LDA */
    wr16((LIND(0x02) + Y), A);  /* 01D4D3 STA */
    wr16(LIND(0x02), A);  /* 01D4D5 STA */
    mflag = 1;  /* 01D4D7 SEM */
    wr8(ABS(0x0231), 0x09);  /* 01D4D8 LDM */
    mflag = 0;  /* 01D4DC CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0400);  /* 01D4DD CLB */
    wr16(DP(0x0B), 0x0001);  /* 01D4E2 LDM */
    goto L_01D4E9;  /* 01D4E6 JMP */
L_01D4E9: TICK();
    goto L_01D7ED;  /* 01D4E9 JMP */
L_01D4EC: TICK();
    mflag = 0;  /* 01D4EC CLM */
    A = rd16(LIND(0x08));  /* 01D4ED LDA */
    mflag = 1;  /* 01D4EF SEM */
    SETLO(A, A - 1);  /* 01D4F0 DEC */
    if ((A & 0xFF) == 0) goto L_01D4F6;  /* 01D4F1 BEQ */
    goto L_01D7ED;  /* 01D4F3 JMP */
L_01D4F6: TICK();
    SETLO(A, rd8(LIND(0x05)));  /* 01D4F6 LDA */
    SETLO(A, A - 1);  /* 01D4F8 DEC */
    if ((A & 0xFF) != 0) goto L_01D542;  /* 01D4F9 BNE */
    SETLO(A, rd8(ABS(0x01EA)));  /* 01D4FB LDA */
    /* 01D4FE CMP */
    if ((A & 0xFF) != 0x05) goto L_01D51F;  /* 01D500 BNE */
    SETLO(A, 0x07);  /* 01D502 LDA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D504 STA */
    mflag = 0;  /* 01D506 CLM */
    wr16(DP(0x0B), 0x0000);  /* 01D507 LDM */
    Y = rd16(ABS(0x0231));  /* 01D50B LDY */
    S -= 3; F_01CC04();  /* 01D50E JSRL */
    Y = 0x0002;  /* 01D512 LDY */
    A = rd16(LIND(0x02));  /* 01D515 LDA */
    Y = A;  /* 01D517 TAY */
    S -= 3; F_01CF78();  /* 01D518 JSRL */
    goto L_01D53A;  /* 01D51C JMP */
L_01D51F: TICK();
    mflag = 1;  /* 01D51F SEM */
    wr8(ABS(0x029C), 0x0B);  /* 01D520 LDM */
    wr8(ABS(0x029D), 0x01);  /* 01D524 LDM */
    wr8(ABS(0x029E), 0x05);  /* 01D528 LDM */
    Y = 0x0000;  /* 01D52C LDY */
    mflag = 0;  /* 01D52F CLM */
    S -= 3; F_0196AA();  /* 01D530 JSRL */
    A = 0x0005;  /* 01D534 LDA */
    mflag = 1;  /* 01D537 SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D538 STA */
L_01D53A: TICK();
    mflag = 1;  /* 01D53A SEM */
    wr8(ABS(0x0230), 0x5E);  /* 01D53B LDM */
    goto L_01D694;  /* 01D53F JMP */
L_01D542: TICK();
    mflag = 0;  /* 01D542 CLM */
    A = rd16(LIND(0x05));  /* 01D543 LDA */
    mflag = 1;  /* 01D545 SEM */
    /* 01D546 CMP */
    if ((A & 0xFF) != 0x05) goto L_01D55F;  /* 01D548 BNE */
    SETLO(A, rd8(DP(0x0F)));  /* 01D54A LDA */
    /* 01D54C CMP */
    if ((A & 0xFF) != 0x01) goto L_01D55C;  /* 01D54E BNE */
    SETLO(A, 0x06);  /* 01D550 LDA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D552 STA */
    wr8(ABS(0x0112), 0x1F);  /* 01D554 LDM */
    wr8(ABS(0x0230), 0x5E);  /* 01D558 LDM */
L_01D55C: TICK();
    goto L_01D694;  /* 01D55C JMP */
L_01D55F: TICK();
    mflag = 0;  /* 01D55F CLM */
    A = rd16(LIND(0x05));  /* 01D560 LDA */
    mflag = 1;  /* 01D562 SEM */
    /* 01D563 CMP */
    if ((A & 0xFF) != 0x06) goto L_01D5B6;  /* 01D565 BNE */
    SETLO(A, rd8(DP(0x0F)));  /* 01D567 LDA */
    /* 01D569 CMP */
    if ((A & 0xFF) != 0x02) goto L_01D599;  /* 01D56B BNE */
    mflag = 0;  /* 01D56D CLM */
    S -= 3; F_019852();  /* 01D56E JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 01D572 SEB */
    Y = 0x0002;  /* 01D577 LDY */
    A = rd16(LIND(0x02));  /* 01D57A LDA */
    Y = A;  /* 01D57C TAY */
    S -= 3; F_01CF78();  /* 01D57D JSRL */
    Y = 0x0005;  /* 01D581 LDY */
    S -= 3; F_019884();  /* 01D584 JSRL */
    S -= 3; F_0195DA();  /* 01D588 JSRL */
    A = 0x0007;  /* 01D58C LDA */
    mflag = 1;  /* 01D58F SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D590 STA */
    wr8(ABS(0x0230), 0x5E);  /* 01D592 LDM */
    goto L_01D5B3;  /* 01D596 JMP */
L_01D599: TICK();
    mflag = 1;  /* 01D599 SEM */
    SETLO(A, rd8(ABS(0x0112)));  /* 01D59A LDA */
    if ((A & 0xFF) != 0) goto L_01D5B3;  /* 01D59D BNE */
    SETLO(A, 0x07);  /* 01D59F LDA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D5A1 STA */
    Y = 0x0002;  /* 01D5A3 LDY */
    mflag = 0;  /* 01D5A6 CLM */
    A = rd16(LIND(0x02));  /* 01D5A7 LDA */
    Y = A;  /* 01D5A9 TAY */
    S -= 3; F_01CF78();  /* 01D5AA JSRL */
    mflag = 1;  /* 01D5AE SEM */
    wr8(ABS(0x0230), 0x5E);  /* 01D5AF LDM */
L_01D5B3: TICK();
    goto L_01D694;  /* 01D5B3 JMP */
L_01D5B6: TICK();
    mflag = 0;  /* 01D5B6 CLM */
    A = rd16(LIND(0x05));  /* 01D5B7 LDA */
    mflag = 1;  /* 01D5B9 SEM */
    /* 01D5BA CMP */
    if ((A & 0xFF) != 0x07) goto L_01D5DB;  /* 01D5BC BNE */
    wr8(ABS(0x029C), 0x0D);  /* 01D5BE LDM */
    wr8(ABS(0x029D), 0x03);  /* 01D5C2 LDM */
    Y = 0x0000;  /* 01D5C6 LDY */
    mflag = 0;  /* 01D5C9 CLM */
    S -= 3; F_0196AA();  /* 01D5CA JSRL */
    A = 0x0008;  /* 01D5CE LDA */
    mflag = 1;  /* 01D5D1 SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D5D2 STA */
    wr8(ABS(0x0230), 0xFB);  /* 01D5D4 LDM */
    goto L_01D694;  /* 01D5D8 JMP */
L_01D5DB: TICK();
    mflag = 0;  /* 01D5DB CLM */
    A = rd16(LIND(0x05));  /* 01D5DC LDA */
    mflag = 1;  /* 01D5DE SEM */
    /* 01D5DF CMP */
    if ((A & 0xFF) != 0x08) goto L_01D60D;  /* 01D5E1 BNE */
    SETLO(A, rd8(DP(0x0F)));  /* 01D5E3 LDA */
    /* 01D5E5 CMP */
    if ((A & 0xFF) == 0x01) goto L_01D5EE;  /* 01D5E7 BEQ */
    SETLO(A, rd8(ABS(0x0112)));  /* 01D5E9 LDA */
    if ((A & 0xFF) != 0) goto L_01D60A;  /* 01D5EC BNE */
L_01D5EE: TICK();
    wr8(DP(0x10), 0x02);  /* 01D5EE LDM */
    wr8(DP(0x11), 0x03);  /* 01D5F1 LDM */
    mflag = 0;  /* 01D5F4 CLM */
    B = 0x0001;  /* 01D5F5 LDA */
    Y = 0x0000;  /* 01D5F9 LDY */
    S -= 3; F_01D889();  /* 01D5FC JSRL */
    A = 0x0009;  /* 01D600 LDA */
    mflag = 1;  /* 01D603 SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D604 STA */
    wr8(ABS(0x0230), 0xFB);  /* 01D606 LDM */
L_01D60A: TICK();
    goto L_01D694;  /* 01D60A JMP */
L_01D60D: TICK();
    mflag = 0;  /* 01D60D CLM */
    A = rd16(LIND(0x05));  /* 01D60E LDA */
    mflag = 1;  /* 01D610 SEM */
    /* 01D611 CMP */
    if ((A & 0xFF) != 0x09) goto L_01D694;  /* 01D613 BNE */
    SETLO(A, rd8(DP(0x0F)));  /* 01D615 LDA */
    /* 01D617 CMP */
    if ((A & 0xFF) != 0x02) goto L_01D694;  /* 01D619 BNE */
    wr8(ABS(0x0230), 0xFB);  /* 01D61B LDM */
    SETLO(A, rd8(ABS(0x008D)));  /* 01D61F LDA */
    SETLO(A, A + 1);  /* 01D622 INC */
    if ((A & 0xFF) != 0) goto L_01D650;  /* 01D623 BNE */
    SETLO(A, rd8(ABS(0x008E)));  /* 01D625 LDA */
    SETLO(A, A + 1);  /* 01D628 INC */
    if ((A & 0xFF) != 0) goto L_01D650;  /* 01D629 BNE */
    SETLO(A, rd8(ABS(0x008F)));  /* 01D62B LDA */
    SETLO(A, A + 1);  /* 01D62E INC */
    if ((A & 0xFF) != 0) goto L_01D650;  /* 01D62F BNE */
    Y = 0x0000;  /* 01D631 LDY */
    mflag = 0;  /* 01D634 CLM */
    S -= 3; F_01CF78();  /* 01D635 JSRL */
    Y = 0x0002;  /* 01D639 LDY */
    A = 0x0000;  /* 01D63C LDA */
    wr16((LIND(0x02) + Y), A);  /* 01D63F STA */
    A = 0x2000;  /* 01D641 LDA */
    wr16(LIND(0x02), A);  /* 01D644 STA */
    A = Y;  /* 01D646 TYA */
    mflag = 1;  /* 01D647 SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D648 STA */
    wr8(DP(0x0F), 0x01);  /* 01D64A LDM */
    goto L_01D694;  /* 01D64D JMP */
L_01D650: TICK();
    Y = 0x0002;  /* 01D650 LDY */
    mflag = 0;  /* 01D653 CLM */
    A = rd16(LIND(0x02));  /* 01D654 LDA */
    A |= rd16((LIND(0x02) + Y));  /* 01D656 ORA */
    if (A == 0) goto L_01D67F;  /* 01D658 BEQ */
    A = rd16(LIND(0x02));  /* 01D65A LDA */
    B = rd16((LIND(0x02) + Y));  /* 01D65C LDA */
    C = 0;  /* 01D65F CLC */
    A = adc16(A, 0xFFFF);  /* 01D660 ADC */
    B = adc16(B, 0xFFFF);  /* 01D663 ADC */
    wr16(LIND(0x02), A);  /* 01D667 STA */
    wr16((LIND(0x02) + Y), B);  /* 01D669 STA */
    A = rd16(LIND(0x02));  /* 01D66C LDA */
    Y = A;  /* 01D66E TAY */
    S -= 3; F_01CF78();  /* 01D66F JSRL */
    mflag = 1;  /* 01D673 SEM */
    wr8(ABS(0x0112), 0x10);  /* 01D674 LDM */
    SETLO(A, 0x08);  /* 01D678 LDA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D67A STA */
    goto L_01D694;  /* 01D67C JMP */
L_01D67F: TICK();
    mflag = 0;  /* 01D67F CLM */
    A = 0x0000;  /* 01D680 LDA */
    mflag = 1;  /* 01D683 SEM */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D684 STA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D686 STA */
    wr8(ABS(0x0231), 0x09);  /* 01D688 LDM */
    wr8(ABS(0x0230), (uint8_t)A);  /* 01D68C STA */
    mflag = 0;  /* 01D68F CLM */
    wr16(DP(0x0B), 0x0001);  /* 01D690 LDM */
L_01D694: TICK();
    mflag = 0;  /* 01D694 CLM */
    A = rd16(LIND(0x05));  /* 01D695 LDA */
    mflag = 1;  /* 01D697 SEM */
    /* 01D698 CMP */
    if ((A & 0xFF) == 0x02) goto L_01D69F;  /* 01D69A BEQ */
    goto L_01D7AD;  /* 01D69C JMP */
L_01D69F: TICK();
    SETLO(A, rd8(DP(0x0F)));  /* 01D69F LDA */
    /* 01D6A1 CMP */
    if ((A & 0xFF) == 0x01) goto L_01D6A8;  /* 01D6A3 BEQ */
    goto L_01D7AA;  /* 01D6A5 JMP */
L_01D6A8: TICK();
    mflag = 0;  /* 01D6A8 CLM */
    wr16(DP(0x0B), 0x0001);  /* 01D6A9 LDM */
L_01D6AD: TICK();
    mflag = 1;  /* 01D6AD SEM */
    wr8(ABS(WDT), 0x00);  /* 01D6AE LDM */
    wr8(ABS(0x0230), 0xFB);  /* 01D6B2 LDM */
    Y = 0x0002;  /* 01D6B6 LDY */
    mflag = 0;  /* 01D6B9 CLM */
    A = rd16((LIND(0x02) + Y));  /* 01D6BA LDA */
    C = (A >= 0x0002);  /* 01D6BC CMP */
    Z = (A == 0x0002);
    if (!Z) goto L_01D6C6;  /* 01D6BF BNE */
    A = rd16(LIND(0x02));  /* 01D6C1 LDA */
    C = (A >= 0x0000);  /* 01D6C3 CMP */
L_01D6C6: TICK();
    if (!C) goto L_01D6CB;  /* 01D6C6 BCC */
    goto L_01D781;  /* 01D6C8 JMP */
L_01D6CB: TICK();
    wr16(DP(0x0D), 0x0000);  /* 01D6CB LDM */
L_01D6CF: TICK();
    X = rd16(DP(0x0D));  /* 01D6CF LDX */
    C = (X >= 0x0080);  /* 01D6D1 CPX */
    if (C) goto L_01D70D;  /* 01D6D4 BCS */
    Y = 0x0002;  /* 01D6D6 LDY */
    A = rd16((LIND(0x02) + Y));  /* 01D6D9 LDA */
    mflag = 1;  /* 01D6DB SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 01D6DC STA */
    mflag = 0;  /* 01D6DE CLM */
    A = rd16(LIND(0x02));  /* 01D6DF LDA */
    wr16(DP(0x10), A);  /* 01D6E1 STA */
    A = rd16(LIND(0x10));  /* 01D6E3 LDA */
    mflag = 1;  /* 01D6E5 SEM */
    wr8((ABS(0x008D) + X), (uint8_t)A);  /* 01D6E6 STA */
    mflag = 0;  /* 01D6E9 CLM */
    A = rd16(LIND(0x02));  /* 01D6EA LDA */
    B = rd16((LIND(0x02) + Y));  /* 01D6EC LDA */
    A = adc16(A, 0x0001);  /* 01D6EF ADC */
    B = adc16(B, 0x0000);  /* 01D6F2 ADC */
    wr16(LIND(0x02), A);  /* 01D6F6 STA */
    wr16((LIND(0x02) + Y), B);  /* 01D6F8 STA */
    A = rd16((ABS(0x008D) + X));  /* 01D6FB LDA */
    mflag = 1;  /* 01D6FE SEM */
    SETLO(A, A + 1);  /* 01D6FF INC */
    if ((A & 0xFF) == 0) goto L_01D707;  /* 01D700 BEQ */
    mflag = 0;  /* 01D702 CLM */
    wr16(DP(0x0B), 0x0000);  /* 01D703 LDM */
L_01D707: TICK();
    mflag = 0;  /* 01D707 CLM */
    t = (rd16(DP(0x0D)) + 1) & 0xFFFF; wr16(DP(0x0D), t);  /* 01D708 INC */
    goto L_01D6CF;  /* 01D70A JMP */
L_01D70D: TICK();
    mflag = 0;  /* 01D70D CLM */
    X = rd16(DP(0x0B));  /* 01D70E LDX */
    if (X != 0) goto L_01D72F;  /* 01D710 BNE */
    wr16(DP(0x10), 0x0002);  /* 01D712 LDM */
    wr16(DP(0x11), 0x0080);  /* 01D716 LDM */
    Y = 0x0002;  /* 01D71A LDY */
    A = rd16(LIND(0x02));  /* 01D71D LDA */
    B = rd16((LIND(0x02) + Y));  /* 01D71F LDA */
    C = 0;  /* 01D722 CLC */
    A = adc16(A, 0xFF80);  /* 01D723 ADC */
    B = adc16(B, 0xFFFF);  /* 01D726 ADC */
    Y = A;  /* 01D72A TAY */
    S -= 3; F_01D837();  /* 01D72B JSRL */
L_01D72F: TICK();
    Y = 0x0002;  /* 01D72F LDY */
    A = rd16(LIND(0x02));  /* 01D732 LDA */
    B = rd16((LIND(0x02) + Y));  /* 01D734 LDA */
    C = 0;  /* 01D737 CLC */
    A = adc16(A, 0xE000);  /* 01D738 ADC */
    B = adc16(B, 0xFFFF);  /* 01D73B ADC */
    wr16(DP(0x12), 0x0000);  /* 01D73F LDM */
    wr16(DP(0x10), 0x1800);  /* 01D743 LDM */
    X = 0x0010;  /* 01D747 LDX */
    S -= 3; rt_lumod_01E43E();  /* 01D74A JSRL */
    Z = (B == 0x0000);  /* 01D74E CMP */
    if (!Z) goto L_01D757;  /* 01D752 BNE */
    Z = (A == 0x0000);  /* 01D754 CMP */
L_01D757: TICK();
    if (!Z) goto L_01D77E;  /* 01D757 BNE */
    A = rd16(LIND(0x02));  /* 01D759 LDA */
    B = rd16((LIND(0x02) + Y));  /* 01D75B LDA */
    C = 0;  /* 01D75E CLC */
    A = adc16(A, 0xE000);  /* 01D75F ADC */
    B = adc16(B, 0xFFFF);  /* 01D762 ADC */
    wr16(DP(0x12), 0x0000);  /* 01D766 LDM */
    wr16(DP(0x10), 0x1800);  /* 01D76A LDM */
    X = 0x0010;  /* 01D76E LDX */
    S -= 3; rt_ludiv_01E414();  /* 01D771 JSRL */
    mpy16(0x0005);  /* 01D775 MPY */
    Y = A;  /* 01D779 TAY */
    S -= 3; F_01CF78();  /* 01D77A JSRL */
L_01D77E: TICK();
    goto L_01D7A3;  /* 01D77E JMP */
L_01D781: TICK();
    mflag = 1;  /* 01D781 SEM */
    wr8(ABS(0x008D), 0x00);  /* 01D782 LDM */
    wr8(DP(0x10), 0x02);  /* 01D786 LDM */
    wr8(DP(0x11), 0x01);  /* 01D789 LDM */
    mflag = 0;  /* 01D78C CLM */
    B = 0x0001;  /* 01D78D LDA */
    Y = 0x00FF;  /* 01D791 LDY */
    S -= 3; F_01D837();  /* 01D794 JSRL */
    A = 0x000A;  /* 01D798 LDA */
    mflag = 1;  /* 01D79B SEM */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D79C STA */
    mflag = 0;  /* 01D79E CLM */
    wr16(DP(0x0B), 0x0000);  /* 01D79F LDM */
L_01D7A3: TICK();
    X = rd16(DP(0x0B));  /* 01D7A3 LDX */
    if (X == 0) goto L_01D7AA;  /* 01D7A5 BEQ */
    goto L_01D6AD;  /* 01D7A7 JMP */
L_01D7AA: TICK();
    goto L_01D7ED;  /* 01D7AA JMP */
L_01D7AD: TICK();
    mflag = 0;  /* 01D7AD CLM */
    A = rd16(LIND(0x05));  /* 01D7AE LDA */
    mflag = 1;  /* 01D7B0 SEM */
    /* 01D7B1 CMP */
    if ((A & 0xFF) != 0x0A) goto L_01D7ED;  /* 01D7B3 BNE */
    SETLO(A, rd8(DP(0x0F)));  /* 01D7B5 LDA */
    /* 01D7B7 CMP */
    if ((A & 0xFF) != 0x01) goto L_01D7ED;  /* 01D7B9 BNE */
    mflag = 0;  /* 01D7BB CLM */
    S -= 3; F_01986E();  /* 01D7BC JSRL */
    S -= 3; F_019852();  /* 01D7C0 JSRL */
    Y = 0x0019;  /* 01D7C4 LDY */
    S -= 3; F_019884();  /* 01D7C7 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 01D7CB CLB */
    A = 0x0000;  /* 01D7D0 LDA */
    mflag = 1;  /* 01D7D3 SEM */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D7D4 STA */
    wr8(LIND(0x05), (uint8_t)A);  /* 01D7D6 STA */
    Y = 0x0002;  /* 01D7D8 LDY */
    mflag = 0;  /* 01D7DB CLM */
    A = 0x0000;  /* 01D7DC LDA */
    wr16((LIND(0x02) + Y), A);  /* 01D7DF STA */
    wr16(LIND(0x02), A);  /* 01D7E1 STA */
    mflag = 1;  /* 01D7E3 SEM */
    wr8(ABS(0x0231), 0x09);  /* 01D7E4 LDM */
    mflag = 0;  /* 01D7E8 CLM */
    wr16(DP(0x0B), 0x0001);  /* 01D7E9 LDM */
L_01D7ED: TICK();
    mflag = 1;  /* 01D7ED SEM */
    SETLO(A, rd8(LIND(0x08)));  /* 01D7EE LDA */
    if ((A & 0xFF) != 0) goto L_01D81E;  /* 01D7F0 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 01D7F2 LDA */
    /* 01D7F5 CMP */
    if ((A & 0xFF) != 0x09) goto L_01D81E;  /* 01D7F7 BNE */
    SETLO(A, rd8(ABS(0x0230)));  /* 01D7F9 LDA */
    if ((A & 0xFF) != 0) goto L_01D81E;  /* 01D7FC BNE */
    mflag = 0;  /* 01D7FE CLM */
    if ((rd16(ABS(0x023C)) & 0x0020) == 0x0020) goto L_01D80B;  /* 01D7FF BBS */
    if ((rd16(ABS(0x023C)) & 0x0008) == 0) goto L_01D81E;  /* 01D805 BBC */
L_01D80B: TICK();
    A = 0x0004;  /* 01D80B LDA */
    mflag = 1;  /* 01D80E SEM */
    wr8(LIND(0x08), (uint8_t)A);  /* 01D80F STA */
    wr8(ABS(0x0230), 0x5E);  /* 01D811 LDM */
    wr8(ABS(0x0116), 0x08);  /* 01D815 LDM */
    mflag = 0;  /* 01D819 CLM */
    S -= 3; F_0195DA();  /* 01D81A JSRL */
L_01D81E: TICK();
    mflag = 0;  /* 01D81E CLM */
    X = rd16(DP(0x0B));  /* 01D81F LDX */
    if (X == 0) goto L_01D834;  /* 01D821 BEQ */
    wr16(DP(0x0B), 0x0000);  /* 01D823 LDM */
    Y = rd16(ABS(0x0231));  /* 01D827 LDY */
    S -= 3; F_01CC04();  /* 01D82A JSRL */
    Y = rd16(DP(0x00));  /* 01D82E LDY */
    S -= 3; F_01CF78();  /* 01D830 JSRL */
L_01D834: TICK();
    Y = pull16();  /* 01D834 PLY */
    D = pull16();  /* 01D835 PLD */
    S += 3; return;  /* 01D836 RTL */
}

/* 01D837  callers: 01D44B, 01D485, 01D4AD, 01D72B, 01D794 */
void F_01D837(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D837 LDA */
    push16(D);  /* 01D839 PHD */
    D = A;  /* 01D83A TAD */
    C = 0;  /* 01D83B CLC */
    A = adc16(A, 0x0006);  /* 01D83C ADC */
    push16(A);  /* 01D83F PHA */
    mflag = 1;  /* 01D840 SEM */
    wr8(ABS(0x029C), 0x03);  /* 01D841 LDM */
    wr8(ABS(0x029D), 0x03);  /* 01D845 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 01D849 LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 01D84B STA */
    wr16(DP(0x02), Y);  /* 01D84E STY */
    mflag = 0;  /* 01D850 CLM */
    wr16(DP(0x04), B);  /* 01D851 STA */
    A = Y;  /* 01D854 TYA */
    X = 0x0008;  /* 01D855 LDX */
L_01D858: TICK();
    B = lsr16(B);  /* 01D858 LSR */
    A = ror16(A);  /* 01D85A ROR */
    X -= 1;  /* 01D85B DEX */
    if (X != 0) goto L_01D858;  /* 01D85C BNE */
    X = 0x0008;  /* 01D85E LDX */
L_01D861: TICK();
    B = lsr16(B);  /* 01D861 LSR */
    A = ror16(A);  /* 01D863 ROR */
    X -= 1;  /* 01D864 DEX */
    if (X != 0) goto L_01D861;  /* 01D865 BNE */
    mflag = 1;  /* 01D867 SEM */
    wr8(ABS(0x029F), (uint8_t)A);  /* 01D868 STA */
    mflag = 0;  /* 01D86B CLM */
    A = rd16(DP(0x03));  /* 01D86C LDA */
    A &= 0x00FF;  /* 01D86E AND */
    mflag = 1;  /* 01D871 SEM */
    wr8(ABS(0x02A0), (uint8_t)A);  /* 01D872 STA */
    SETLO(A, Y);  /* 01D875 TYA */
    wr8(ABS(0x02A1), (uint8_t)A);  /* 01D876 STA */
    Y = rd16(DP(0x01));  /* 01D879 LDY */
    mflag = 0;  /* 01D87B CLM */
    S -= 3; F_0196AA();  /* 01D87C JSRL */
    mflag = 1;  /* 01D880 SEM */
    wr8(ABS(0x0113), 0x00);  /* 01D881 LDM */
    mflag = 0;  /* 01D885 CLM */
    Y = pull16();  /* 01D886 PLY */
    D = pull16();  /* 01D887 PLD */
    S += 3; return;  /* 01D888 RTL */
}

/* 01D889  callers: 01D5FC */
void F_01D889(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D889 LDA */
    push16(D);  /* 01D88B PHD */
    D = A;  /* 01D88C TAD */
    C = 0;  /* 01D88D CLC */
    A = adc16(A, 0x0006);  /* 01D88E ADC */
    push16(A);  /* 01D891 PHA */
    mflag = 1;  /* 01D892 SEM */
    wr8(ABS(0x029C), 0x03);  /* 01D893 LDM */
    wr8(ABS(0x029D), 0x01);  /* 01D897 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 01D89B LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 01D89D STA */
    wr16(DP(0x02), Y);  /* 01D8A0 STY */
    mflag = 0;  /* 01D8A2 CLM */
    wr16(DP(0x04), B);  /* 01D8A3 STA */
    A = Y;  /* 01D8A6 TYA */
    X = 0x0008;  /* 01D8A7 LDX */
L_01D8AA: TICK();
    B = lsr16(B);  /* 01D8AA LSR */
    A = ror16(A);  /* 01D8AC ROR */
    X -= 1;  /* 01D8AD DEX */
    if (X != 0) goto L_01D8AA;  /* 01D8AE BNE */
    X = 0x0008;  /* 01D8B0 LDX */
L_01D8B3: TICK();
    B = lsr16(B);  /* 01D8B3 LSR */
    A = ror16(A);  /* 01D8B5 ROR */
    X -= 1;  /* 01D8B6 DEX */
    if (X != 0) goto L_01D8B3;  /* 01D8B7 BNE */
    mflag = 1;  /* 01D8B9 SEM */
    wr8(ABS(0x029F), (uint8_t)A);  /* 01D8BA STA */
    mflag = 0;  /* 01D8BD CLM */
    A = rd16(DP(0x03));  /* 01D8BE LDA */
    A &= 0x00FF;  /* 01D8C0 AND */
    mflag = 1;  /* 01D8C3 SEM */
    wr8(ABS(0x02A0), (uint8_t)A);  /* 01D8C4 STA */
    SETLO(A, Y);  /* 01D8C7 TYA */
    wr8(ABS(0x02A1), (uint8_t)A);  /* 01D8C8 STA */
    SETLO(A, rd8(DP(0x01)));  /* 01D8CB LDA */
    wr8(ABS(0x02A2), (uint8_t)A);  /* 01D8CD STA */
    mflag = 0;  /* 01D8D0 CLM */
    Y = A;  /* 01D8D1 TAY */
    S -= 3; F_0196AA();  /* 01D8D2 JSRL */
    Y = pull16();  /* 01D8D6 PLY */
    D = pull16();  /* 01D8D7 PLD */
    S += 3; return;  /* 01D8D8 RTL */
}

/* 01D8D9  callers: 01D472 */
void F_01D8D9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D8D9 LDA */
    push16(D);  /* 01D8DB PHD */
    D = A;  /* 01D8DC TAD */
    C = 0;  /* 01D8DD CLC */
    A = adc16(A, 0x0004);  /* 01D8DE ADC */
    push16(A);  /* 01D8E1 PHA */
    wr16(DP(0x00), Y);  /* 01D8E2 STY */
    Y = 0x0589;  /* 01D8E4 LDY */
    S -= 3; eeprom_read_word_0071F0();  /* 01D8E7 JSRL */
    A &= 0xF000;  /* 01D8EB AND */
    Y = rd16(DP(0x00));  /* 01D8EE LDY */
    wr16(DP(0x02), A);  /* 01D8F0 STA */
    S -= 3; F_01D90E();  /* 01D8F2 JSRL */
    A |= rd16(DP(0x02));  /* 01D8F6 ORA */
    wr16(DP(0x02), A);  /* 01D8F8 STA */
    A = rla16(A, 0x0008);  /* 01D8FA RLA */
    A &= 0x00FF;  /* 01D8FE AND */
    mflag = 1;  /* 01D901 SEM */
    wr8(ABS(0x008D), (uint8_t)A);  /* 01D902 STA */
    SETLO(A, rd8(DP(0x02)));  /* 01D905 LDA */
    wr8(ABS(0x008E), (uint8_t)A);  /* 01D907 STA */
    mflag = 0;  /* 01D90A CLM */
    Y = pull16();  /* 01D90B PLY */
    D = pull16();  /* 01D90C PLD */
    S += 3; return;  /* 01D90D RTL */
}

/* 01D90E  callers: 01D8F2, 01D993 */
void F_01D90E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D90E LDA */
    push16(D);  /* 01D910 PHD */
    D = A;  /* 01D911 TAD */
    B = 0x0000;  /* 01D912 LDA */
    A = Y;  /* 01D916 TYA */
    div16(0x0064);  /* 01D917 DIV */
    A = rla16(A, 0x0008);  /* 01D91B RLA */
    A &= 0xFF00;  /* 01D91F AND */
    B = 0x0000;  /* 01D922 LDA */
    wr16(DP(0x02), A);  /* 01D926 STA */
    A = Y;  /* 01D928 TYA */
    div16(0x000A);  /* 01D929 DIV */
    B = 0x0000;  /* 01D92D LDA */
    div16(0x000A);  /* 01D931 DIV */
    push16(B);  /* 01D935 PHB */
    A = pull16();  /* 01D937 PLA */
    A = asl16(A);  /* 01D938 ASL */
    A = asl16(A);  /* 01D939 ASL */
    A = asl16(A);  /* 01D93A ASL */
    A = asl16(A);  /* 01D93B ASL */
    A |= rd16(DP(0x02));  /* 01D93C ORA */
    B = 0x0000;  /* 01D93E LDA */
    wr16(DP(0x02), A);  /* 01D942 STA */
    A = Y;  /* 01D944 TYA */
    wr16(DP(0x00), Y);  /* 01D945 STY */
    div16(0x000A);  /* 01D947 DIV */
    push16(B);  /* 01D94B PHB */
    A = pull16();  /* 01D94D PLA */
    A |= rd16(DP(0x02));  /* 01D94E ORA */
    wr16(DP(0x02), A);  /* 01D950 STA */
    D = pull16();  /* 01D952 PLD */
    S += 3; return;  /* 01D953 RTL */
}

/* 01D954  callers: 01D49A */
void F_01D954(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D954 LDA */
    push16(D);  /* 01D956 PHD */
    D = A;  /* 01D957 TAD */
    C = 0;  /* 01D958 CLC */
    A = adc16(A, 0x0006);  /* 01D959 ADC */
    push16(A);  /* 01D95C PHA */
    wr16(DP(0x00), Y);  /* 01D95D STY */
    Y = 0x0589;  /* 01D95F LDY */
    S -= 3; eeprom_read_word_0071F0();  /* 01D962 JSRL */
    Y = 0x05BD;  /* 01D966 LDY */
    wr16(DP(0x04), A);  /* 01D969 STA */
    S -= 3; eeprom_read_word_0071F0();  /* 01D96B JSRL */
    wr16(DP(0x02), A);  /* 01D96F STA */
    A = rd16(DP(0x05));  /* 01D971 LDA */
    A &= 0x00FF;  /* 01D973 AND */
    A ^= 0xFFFF;  /* 01D976 EOR */
    A += 1;  /* 01D979 INC */
    C = 0;  /* 01D97A CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 01D97B ADC */
    wr16(DP(0x02), A);  /* 01D97D STA */
    A = rd16(DP(0x04));  /* 01D97F LDA */
    A &= 0x00FF;  /* 01D981 AND */
    A ^= 0xFFFF;  /* 01D984 EOR */
    A += 1;  /* 01D987 INC */
    C = 0;  /* 01D988 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 01D989 ADC */
    wr16(DP(0x04), rd16(DP(0x04)) & ~0x0FFF);  /* 01D98B CLB */
    Y = rd16(DP(0x00));  /* 01D98F LDY */
    wr16(DP(0x02), A);  /* 01D991 STA */
    S -= 3; F_01D90E();  /* 01D993 JSRL */
    A |= rd16(DP(0x04));  /* 01D997 ORA */
    wr16(DP(0x04), A);  /* 01D999 STA */
    A = rla16(A, 0x0008);  /* 01D99B RLA */
    A &= 0x00FF;  /* 01D99F AND */
    C = 0;  /* 01D9A2 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 01D9A3 ADC */
    wr16(DP(0x02), A);  /* 01D9A5 STA */
    A = rd16(DP(0x04));  /* 01D9A7 LDA */
    A &= 0x00FF;  /* 01D9A9 AND */
    C = 0;  /* 01D9AC CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 01D9AD ADC */
    wr16(DP(0x02), A);  /* 01D9AF STA */
    A = rla16(A, 0x0008);  /* 01D9B1 RLA */
    A &= 0x00FF;  /* 01D9B5 AND */
    mflag = 1;  /* 01D9B8 SEM */
    wr8(ABS(0x008D), (uint8_t)A);  /* 01D9B9 STA */
    SETLO(A, rd8(DP(0x02)));  /* 01D9BC LDA */
    wr8(ABS(0x008E), (uint8_t)A);  /* 01D9BE STA */
    mflag = 0;  /* 01D9C1 CLM */
    Y = pull16();  /* 01D9C2 PLY */
    D = pull16();  /* 01D9C3 PLD */
    S += 3; return;  /* 01D9C4 RTL */
}

/* 01D9C5  callers: 01CE4A, 01CE6D, 01CE80 */
void F_01D9C5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D9C5 LDA */
    push16(A);  /* 01D9C7 PHA */
    mflag = 1;  /* 01D9C8 SEM */
    wr8(ABS(0x0231), 0x0D);  /* 01D9C9 LDM */
    mflag = 0;  /* 01D9CD CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 01D9CE CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0008);  /* 01D9D3 CLB */
    mflag = 1;  /* 01D9D8 SEM */
    wr8(ABS(0x0230), 0x5E);  /* 01D9D9 LDM */
    wr8(ABS(0x0116), 0x08);  /* 01D9DD LDM */
    mflag = 0;  /* 01D9E1 CLM */
    S -= 3; F_0195DA();  /* 01D9E2 JSRL */
    Y = pull16();  /* 01D9E6 PLY */
    S += 3; return;  /* 01D9E7 RTL */
}

/* 01D9E8  callers: 01C9F1 */
void F_01D9E8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01D9E8 LDA */
    push16(D);  /* 01D9EA PHD */
    D = A;  /* 01D9EB TAD */
    if ((rd16(DP(0x00)) & 0x0080) == 0) goto L_01D9FF;  /* 01D9EC BBC */
    X = rd16(ABS(0x044E));  /* 01D9F1 LDX */
    if (X == 0) goto L_01D9F9;  /* 01D9F4 BEQ */
    t = (rd16(ABS(0x044E)) - 1) & 0xFFFF; wr16(ABS(0x044E), t);  /* 01D9F6 DEC */
L_01D9F9: TICK();
    A = rd16(DP(0x00));  /* 01D9F9 LDA */
    mflag = 1;  /* 01D9FB SEM */
    wr8(ABS(0x044D), (uint8_t)A);  /* 01D9FC STA */
L_01D9FF: TICK();
    mflag = 0;  /* 01D9FF CLM */
    D = pull16();  /* 01DA00 PLD */
    S += 3; return;  /* 01DA01 RTL */
}

/* 01DA02  callers: 0150B5, 016ACA, 01DADF, 01DBC7, 01DBED, 01DC09, 01DC2E */
void F_01DA02(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DA02 LDA */
    push16(D);  /* 01DA04 PHD */
    D = A;  /* 01DA05 TAD */
    A += 1;  /* 01DA06 INC */
    push16(A);  /* 01DA07 PHA */
    wr16(DP(0x00), Y);  /* 01DA08 STY */
    A = Y;  /* 01DA0A TYA */
    mflag = 1;  /* 01DA0B SEM */
    wr8(ABS(0x044C), (uint8_t)A);  /* 01DA0C STA */
    wr8(ABS(0x044D), 0x00);  /* 01DA0F LDM */
    mflag = 0;  /* 01DA13 CLM */
    S -= 3; F_00CB52();  /* 01DA14 JSRL */
    mflag = 1;  /* 01DA18 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 01DA19 LDA */
    if ((A & 0xFF) != 0) goto L_01DA45;  /* 01DA1B BNE */
    mflag = 0;  /* 01DA1D CLM */
    wr16(ABS(0x044E), 0x0000);  /* 01DA1E LDM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x8000);  /* 01DA23 CLB */
    wr16(DP(0x01), 0x0010);  /* 01DA28 LDM */
    wr16(DP(0x03), 0x0058);  /* 01DA2C LDM */
    wr16(DP(0x05), 0x0060);  /* 01DA30 LDM */
    Y = 0x0003;  /* 01DA34 LDY */
    A = D;  /* 01DA37 TDA */
    C = 0;  /* 01DA38 CLC */
    A = adc16(A, 0x0007);  /* 01DA39 ADC */
    push16(A);  /* 01DA3C PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01DA3D JSRL */
    Y = pull16();  /* 01DA41 PLY */
    goto L_01DAC0;  /* 01DA42 JMP */
L_01DA45: TICK();
    mflag = 0;  /* 01DA45 CLM */
    A = rd16(DP(0x00));  /* 01DA46 LDA */
    mflag = 1;  /* 01DA48 SEM */
    /* 01DA49 CMP */
    if ((A & 0xFF) != 0x01) goto L_01DA64;  /* 01DA4B BNE */
    SETLO(A, rd8(ABS(0x0280)));  /* 01DA4D LDA */
    mflag = 0;  /* 01DA50 CLM */
    A &= 0x00FF;  /* 01DA51 AND */
    wr16(ABS(0x044E), A);  /* 01DA54 STA */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x8000);  /* 01DA57 CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0004);  /* 01DA5C CLB */
    goto L_01DA70;  /* 01DA61 JMP */
L_01DA64: TICK();
    mflag = 0;  /* 01DA64 CLM */
    X = rd16(ABS(0x0281));  /* 01DA65 LDX */
    wr16(ABS(0x044E), X);  /* 01DA68 STX */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x8000);  /* 01DA6B SEB */
L_01DA70: TICK();
    wr16(DP(0x01), 0x0036);  /* 01DA70 LDM */
    wr16(DP(0x03), 0x0004);  /* 01DA74 LDM */
    wr16(DP(0x05), 0x0060);  /* 01DA78 LDM */
    Y = 0x0003;  /* 01DA7C LDY */
    A = D;  /* 01DA7F TDA */
    C = 0;  /* 01DA80 CLC */
    A = adc16(A, 0x0007);  /* 01DA81 ADC */
    push16(A);  /* 01DA84 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01DA85 JSRL */
    Y = pull16();  /* 01DA89 PLY */
    mflag = 0;  /* 01DA8A CLM */
    wr16(DP(0x01), 0x0000);  /* 01DA8B LDM */
    wr16(DP(0x03), 0x0000);  /* 01DA8F LDM */
    wr16(DP(0x05), 0x0062);  /* 01DA93 LDM */
    Y = 0x0003;  /* 01DA97 LDY */
    A = D;  /* 01DA9A TDA */
    C = 0;  /* 01DA9B CLC */
    A = adc16(A, 0x0007);  /* 01DA9C ADC */
    push16(A);  /* 01DA9F PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01DAA0 JSRL */
    Y = pull16();  /* 01DAA4 PLY */
    mflag = 0;  /* 01DAA5 CLM */
    wr16(DP(0x01), 0x0001);  /* 01DAA6 LDM */
    wr16(DP(0x03), 0x0000);  /* 01DAAA LDM */
    wr16(DP(0x05), 0x0063);  /* 01DAAE LDM */
    Y = 0x0003;  /* 01DAB2 LDY */
    A = D;  /* 01DAB5 TDA */
    C = 0;  /* 01DAB6 CLC */
    A = adc16(A, 0x0007);  /* 01DAB7 ADC */
    push16(A);  /* 01DABA PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 01DABB JSRL */
    Y = pull16();  /* 01DABF PLY */
L_01DAC0: TICK();
    mflag = 0;  /* 01DAC0 CLM */
    Y = pull16();  /* 01DAC1 PLY */
    D = pull16();  /* 01DAC2 PLD */
    S += 3; return;  /* 01DAC3 RTL */
}

/* 01DAC4  callers: 01A754 */
void F_01DAC4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DAC4 LDA */
    push16(A);  /* 01DAC6 PHA */
    mflag = 1;  /* 01DAC7 SEM */
    if ((rd8(ABS(0x033F)) & 0x01) == 0) goto L_01DAFD;  /* 01DAC8 BBC */
    if ((rd8(ABS(0x0335)) & 0x01) == 0) goto L_01DAF8;  /* 01DACD BBC */
    mflag = 0;  /* 01DAD2 CLM */
    S -= 3; F_007EE2();  /* 01DAD3 JSRL */
    /* 01DAD7 CMP */
    if ((A & 0xFF) == 0x00) goto L_01DAE6;  /* 01DAD9 BEQ */
    Y = 0x0000;  /* 01DADB LDY */
    mflag = 0;  /* 01DADE CLM */
    S -= 3; F_01DA02();  /* 01DADF JSRL */
    goto L_01DAF5;  /* 01DAE3 JMP */
L_01DAE6: TICK();
    mflag = 0;  /* 01DAE6 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0004);  /* 01DAE7 SEB */
    S -= 3; F_009782();  /* 01DAEC JSRL */
    mflag = 1;  /* 01DAF0 SEM */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x01);  /* 01DAF1 CLB */
L_01DAF5: TICK();
    goto L_01DAFD;  /* 01DAF5 JMP */
L_01DAF8: TICK();
    mflag = 1;  /* 01DAF8 SEM */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x01);  /* 01DAF9 CLB */
L_01DAFD: TICK();
    mflag = 1;  /* 01DAFD SEM */
    if ((rd8(ABS(0x033F)) & 0x01) == 0x01) goto L_01DB09;  /* 01DAFE BBS */
    mflag = 0;  /* 01DB03 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0200);  /* 01DB04 CLB */
L_01DB09: TICK();
    mflag = 0;  /* 01DB09 CLM */
    Y = pull16();  /* 01DB0A PLY */
    S += 3; return;  /* 01DB0B RTL */
}

/* 01DB0C  callers: 01DBD2, 01DBF8 */
void F_01DB0C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DB0C LDA */
    push16(D);  /* 01DB0E PHD */
    D = A;  /* 01DB0F TAD */
    wr16(DP(0x00), Y);  /* 01DB10 STY */
    mflag = 1;  /* 01DB12 SEM */
    if ((rd8(ABS(0x044D)) & 0x80) == 0) goto L_01DB88;  /* 01DB13 BBC */
    wr8(ABS(0x044D), rd8(ABS(0x044D)) & ~0xE0);  /* 01DB18 CLB */
    SETLO(A, Y);  /* 01DB1C TYA */
    /* 01DB1D CMP */
    if ((A & 0xFF) != 0x01) goto L_01DB59;  /* 01DB1F BNE */
    SETLO(A, rd8(ABS(0x044D)));  /* 01DB21 LDA */
    t = rd8(ABS(0x0202));  /* 01DB24 CMP */
    if ((A & 0xFF) < t) goto L_01DB32;  /* 01DB27 BCC */
    mflag = 0;  /* 01DB29 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x8000);  /* 01DB2A SEB */
    goto L_01DB41;  /* 01DB2F JMP */
L_01DB32: TICK();
    mflag = 0;  /* 01DB32 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x8000);  /* 01DB33 CLB */
    A = rd16(ABS(0x0280));  /* 01DB38 LDA */
    A &= 0x00FF;  /* 01DB3B AND */
    wr16(ABS(0x044E), A);  /* 01DB3E STA */
L_01DB41: TICK();
    X = rd16(ABS(0x044E));  /* 01DB41 LDX */
    if (X != 0) goto L_01DB56;  /* 01DB44 BNE */
    if ((rd16(ABS(0x023C)) & 0x8000) == 0) goto L_01DB56;  /* 01DB46 BBC */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0004);  /* 01DB4C SEB */
    mflag = 1;  /* 01DB51 SEM */
    wr8(ABS(0x0238), 0x5E);  /* 01DB52 LDM */
L_01DB56: TICK();
    goto L_01DB88;  /* 01DB56 JMP */
L_01DB59: TICK();
    mflag = 0;  /* 01DB59 CLM */
    A = rd16(ABS(0x0203));  /* 01DB5A LDA */
    mflag = 1;  /* 01DB5D SEM */
    t = rd8(ABS(0x044D));  /* 01DB5E CMP */
    if ((A & 0xFF) < t) goto L_01DB6C;  /* 01DB61 BCC */
    mflag = 0;  /* 01DB63 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x8000);  /* 01DB64 CLB */
    goto L_01DB78;  /* 01DB69 JMP */
L_01DB6C: TICK();
    mflag = 0;  /* 01DB6C CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x8000);  /* 01DB6D SEB */
    X = rd16(ABS(0x0281));  /* 01DB72 LDX */
    wr16(ABS(0x044E), X);  /* 01DB75 STX */
L_01DB78: TICK();
    X = rd16(ABS(0x044E));  /* 01DB78 LDX */
    if (X != 0) goto L_01DB88;  /* 01DB7B BNE */
    if ((rd16(ABS(0x023C)) & 0x8000) == 0x8000) goto L_01DB88;  /* 01DB7D BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0004);  /* 01DB83 CLB */
L_01DB88: TICK();
    mflag = 0;  /* 01DB88 CLM */
    D = pull16();  /* 01DB89 PLD */
    S += 3; return;  /* 01DB8A RTL */
}

/* 01DB8B  callers: 01A7B8 */
void F_01DB8B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DB8B LDA */
    push16(A);  /* 01DB8D PHA */
    if ((rd16(ABS(0x023E)) & 0x0200) == 0x0200) goto L_01DB97;  /* 01DB8E BBS */
    goto L_01DC24;  /* 01DB94 JMP */
L_01DB97: TICK();
    if ((rd16(ABS(0x0240)) & 0x4000) == 0) goto L_01DBA0;  /* 01DB97 BBC */
    goto L_01DC24;  /* 01DB9D JMP */
L_01DBA0: TICK();
    A = rd16(ABS(0x0365));  /* 01DBA0 LDA */
    mflag = 1;  /* 01DBA3 SEM */
    SETLO(A, A + 1);  /* 01DBA4 INC */
    if ((A & 0xFF) == 0) goto L_01DC24;  /* 01DBA5 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 01DBA7 LDA */
    if ((A & 0xFF) == 0) goto L_01DBB4;  /* 01DBAA BEQ */
    /* 01DBAC CMP */
    if ((A & 0xFF) == 0x01) goto L_01DBB4;  /* 01DBAE BEQ */
    /* 01DBB0 CMP */
    if ((A & 0xFF) != 0x02) goto L_01DBD9;  /* 01DBB2 BNE */
L_01DBB4: TICK();
    mflag = 0;  /* 01DBB4 CLM */
    if ((rd16(ABS(0x023E)) & 0x0080) == 0x0080) goto L_01DBD6;  /* 01DBB5 BBS */
    A = rd16(ABS(0x044C));  /* 01DBBB LDA */
    mflag = 1;  /* 01DBBE SEM */
    /* 01DBBF CMP */
    if ((A & 0xFF) == 0x01) goto L_01DBCE;  /* 01DBC1 BEQ */
    Y = 0x0001;  /* 01DBC3 LDY */
    mflag = 0;  /* 01DBC6 CLM */
    S -= 3; F_01DA02();  /* 01DBC7 JSRL */
    goto L_01DBD6;  /* 01DBCB JMP */
L_01DBCE: TICK();
    Y = 0x0001;  /* 01DBCE LDY */
    mflag = 0;  /* 01DBD1 CLM */
    S -= 3; F_01DB0C();  /* 01DBD2 JSRL */
L_01DBD6: TICK();
    goto L_01DC21;  /* 01DBD6 JMP */
L_01DBD9: TICK();
    mflag = 0;  /* 01DBD9 CLM */
    A = rd16(ABS(0x0228));  /* 01DBDA LDA */
    mflag = 1;  /* 01DBDD SEM */
    /* 01DBDE CMP */
    if ((A & 0xFF) != 0x08) goto L_01DBFF;  /* 01DBE0 BNE */
    SETLO(A, rd8(ABS(0x044C)));  /* 01DBE2 LDA */
    /* 01DBE5 CMP */
    if ((A & 0xFF) == 0x02) goto L_01DBF4;  /* 01DBE7 BEQ */
    Y = 0x0002;  /* 01DBE9 LDY */
    mflag = 0;  /* 01DBEC CLM */
    S -= 3; F_01DA02();  /* 01DBED JSRL */
    goto L_01DBFC;  /* 01DBF1 JMP */
L_01DBF4: TICK();
    Y = 0x0002;  /* 01DBF4 LDY */
    mflag = 0;  /* 01DBF7 CLM */
    S -= 3; F_01DB0C();  /* 01DBF8 JSRL */
L_01DBFC: TICK();
    goto L_01DC21;  /* 01DBFC JMP */
L_01DBFF: TICK();
    mflag = 1;  /* 01DBFF SEM */
    SETLO(A, rd8(ABS(0x044C)));  /* 01DC00 LDA */
    if ((A & 0xFF) == 0) goto L_01DC0D;  /* 01DC03 BEQ */
    Y = 0x0000;  /* 01DC05 LDY */
    mflag = 0;  /* 01DC08 CLM */
    S -= 3; F_01DA02();  /* 01DC09 JSRL */
L_01DC0D: TICK();
    mflag = 0;  /* 01DC0D CLM */
    A = rd16(ABS(0x0228));  /* 01DC0E LDA */
    mflag = 1;  /* 01DC11 SEM */
    /* 01DC12 CMP */
    if ((A & 0xFF) != 0x04) goto L_01DC21;  /* 01DC14 BNE */
    SETLO(A, rd8(ABS(0x0238)));  /* 01DC16 LDA */
    if ((A & 0xFF) != 0) goto L_01DC21;  /* 01DC19 BNE */
    mflag = 0;  /* 01DC1B CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0004);  /* 01DC1C CLB */
L_01DC21: TICK();
    goto L_01DC32;  /* 01DC21 JMP */
L_01DC24: TICK();
    mflag = 1;  /* 01DC24 SEM */
    SETLO(A, rd8(ABS(0x044C)));  /* 01DC25 LDA */
    if ((A & 0xFF) == 0) goto L_01DC32;  /* 01DC28 BEQ */
    Y = 0x0000;  /* 01DC2A LDY */
    mflag = 0;  /* 01DC2D CLM */
    S -= 3; F_01DA02();  /* 01DC2E JSRL */
L_01DC32: TICK();
    mflag = 0;  /* 01DC32 CLM */
    Y = pull16();  /* 01DC33 PLY */
    S += 3; return;  /* 01DC34 RTL */
}

/* 01DC35  callers: 00E629, 01CB5E */
void F_01DC35(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DC35 LDA */
    push16(D);  /* 01DC37 PHD */
    D = A;  /* 01DC38 TAD */
    A += 1;  /* 01DC39 INC */
    push16(A);  /* 01DC3A PHA */
    mflag = 1;  /* 01DC3B SEM */
    wr8(DP(0x00), 0x00);  /* 01DC3C LDM */
    SETLO(A, rd8(ABS(0x0236)));  /* 01DC3F LDA */
    if ((A & 0xFF) == 0) goto L_01DC47;  /* 01DC42 BEQ */
    goto L_01DDDB;  /* 01DC44 JMP */
L_01DC47: TICK();
    Y = 0x0004;  /* 01DC47 LDY */
    mflag = 0;  /* 01DC4A CLM */
    S -= 3; F_008DC6();  /* 01DC4B JSRL */
    mflag = 1;  /* 01DC4F SEM */
    SETLO(A, rd8(ABS(AD4)));  /* 01DC50 LDA [AN4 13V2] */
    /* 01DC53 CMP */
    if ((A & 0xFF) < 0xAF) goto L_01DC8E;  /* 01DC55 BCC */
    mflag = 0;  /* 01DC57 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0040);  /* 01DC58 CLB */
    A = rd16(ABS(0x0082));  /* 01DC5D LDA */
    mflag = 1;  /* 01DC60 SEM */
    /* 01DC61 CMP */
    if ((A & 0xFF) >= 0x5F) goto L_01DC6C;  /* 01DC63 BCS */
    wr8(ABS(0x0082), 0x5F);  /* 01DC65 LDM */
    goto L_01DC8B;  /* 01DC69 JMP */
L_01DC6C: TICK();
    mflag = 0;  /* 01DC6C CLM */
    A = rd16(ABS(0x0082));  /* 01DC6D LDA */
    mflag = 1;  /* 01DC70 SEM */
    /* 01DC71 CMP */
    if ((A & 0xFF) >= 0x64) goto L_01DC7B;  /* 01DC73 BCS */
    t = (rd8(ABS(0x0082)) + 1) & 0xFF; wr8(ABS(0x0082), t);  /* 01DC75 INC */
    goto L_01DC8B;  /* 01DC78 JMP */
L_01DC7B: TICK();
    mflag = 0;  /* 01DC7B CLM */
    if ((rd16(ABS(0x023C)) & 0x0002) == 0) goto L_01DC8B;  /* 01DC7C BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0002);  /* 01DC82 CLB */
    S -= 3; F_00828C();  /* 01DC87 JSRL */
L_01DC8B: TICK();
    goto L_01DDCC;  /* 01DC8B JMP */
L_01DC8E: TICK();
    mflag = 1;  /* 01DC8E SEM */
    SETLO(A, rd8(ABS(AD4)));  /* 01DC8F LDA [AN4 13V2] */
    /* 01DC92 CMP */
    if ((A & 0xFF) < 0x9F) goto L_01DC99;  /* 01DC94 BCC */
    goto L_01DD51;  /* 01DC96 JMP */
L_01DC99: TICK();
    SETLO(A, rd8(ABS(0x0082)));  /* 01DC99 LDA */
    /* 01DC9C CMP */
    if ((A & 0xFF) < 0x1F) goto L_01DCCA;  /* 01DC9E BCC */
    /* 01DCA0 CMP */
    if ((A & 0xFF) < 0x3D) goto L_01DCAB;  /* 01DCA2 BCC */
    wr8(ABS(0x0082), 0x3C);  /* 01DCA4 LDM */
    goto L_01DCB0;  /* 01DCA8 JMP */
L_01DCAB: TICK();
    mflag = 1;  /* 01DCAB SEM */
    wr8(ABS(0x0082), 0x1E);  /* 01DCAC LDM */
L_01DCB0: TICK();
    mflag = 0;  /* 01DCB0 CLM */
    if ((rd16(ABS(0x023C)) & 0x0002) == 0x0002) goto L_01DCC7;  /* 01DCB1 BBS */
    Y = 0x0010;  /* 01DCB7 LDY */
    S -= 3; F_0080AA();  /* 01DCBA JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0002);  /* 01DCBE SEB */
    S -= 3; F_00828C();  /* 01DCC3 JSRL */
L_01DCC7: TICK();
    goto L_01DD4E;  /* 01DCC7 JMP */
L_01DCCA: TICK();
    mflag = 1;  /* 01DCCA SEM */
    SETLO(A, rd8(ABS(0x0082)));  /* 01DCCB LDA */
    if ((A & 0xFF) == 0) goto L_01DCF2;  /* 01DCCE BEQ */
    mflag = 0;  /* 01DCD0 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0) goto L_01DCDD;  /* 01DCD1 BBC */
    mflag = 1;  /* 01DCD7 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_01DCEB;  /* 01DCD8 BBS */
L_01DCDD: TICK();
    Y = rd16(ABS(0x0082));  /* 01DCDD LDY */
    mflag = 0;  /* 01DCE0 CLM */
    S -= 3; F_01DE21();  /* 01DCE1 JSRL */
    wr8(ABS(0x0082), (uint8_t)A);  /* 01DCE5 STA */
    goto L_01DCEF;  /* 01DCE8 JMP */
L_01DCEB: TICK();
    mflag = 1;  /* 01DCEB SEM */
    t = (rd8(ABS(0x0082)) - 1) & 0xFF; wr8(ABS(0x0082), t);  /* 01DCEC DEC */
L_01DCEF: TICK();
    goto L_01DD4E;  /* 01DCEF JMP */
L_01DCF2: TICK();
    mflag = 0;  /* 01DCF2 CLM */
    S -= 3; F_01DDDF();  /* 01DCF3 JSRL */
    /* 01DCF7 CMP */
    if ((A & 0xFF) == 0x00) goto L_01DD4E;  /* 01DCF9 BEQ */
    mflag = 0;  /* 01DCFB CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_01DD33;  /* 01DCFC BBS */
    if ((rd16(ABS(0x0244)) & 0x0040) == 0x0040) goto L_01DD30;  /* 01DD02 BBS */
    Y = 0x0010;  /* 01DD08 LDY */
    S -= 3; F_0080AA();  /* 01DD0B JSRL */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0040);  /* 01DD0F SEB */
    mflag = 1;  /* 01DD14 SEM */
    wr8(DP(0x00), 0x01);  /* 01DD15 LDM */
    wr8(ABS(0x0082), 0x1E);  /* 01DD18 LDM */
    wr8(ABS(0x0229), 0x02);  /* 01DD1C LDM */
    mflag = 0;  /* 01DD20 CLM */
    if ((rd16(ABS(0x023C)) & 0x0002) == 0x0002) goto L_01DD30;  /* 01DD21 BBS */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0002);  /* 01DD27 SEB */
    S -= 3; F_00828C();  /* 01DD2C JSRL */
L_01DD30: TICK();
    goto L_01DD4E;  /* 01DD30 JMP */
L_01DD33: TICK();
    Y = 0x0011;  /* 01DD33 LDY */
    mflag = 0;  /* 01DD36 CLM */
    S -= 3; F_0080AA();  /* 01DD37 JSRL */
L_01DD3B: TICK();
    X = rd16(ABS(0x0214));  /* 01DD3B LDX */
    if (X == 0) goto L_01DD49;  /* 01DD3E BEQ */
    mflag = 1;  /* 01DD40 SEM */
    wr8(ABS(WDT), 0x00);  /* 01DD41 LDM */
    mflag = 0;  /* 01DD45 CLM */
    goto L_01DD3B;  /* 01DD46 JMP */
L_01DD49: TICK();
    mflag = 0;  /* 01DD49 CLM */
    S -= 3; F_00973E();  /* 01DD4A JSRL */
L_01DD4E: TICK();
    goto L_01DDCC;  /* 01DD4E JMP */
L_01DD51: TICK();
    mflag = 1;  /* 01DD51 SEM */
    SETLO(A, rd8(ABS(AD4)));  /* 01DD52 LDA [AN4 13V2] */
    /* 01DD55 CMP */
    if ((A & 0xFF) >= 0xA9) goto L_01DDA9;  /* 01DD57 BCS */
    SETLO(A, rd8(ABS(0x0082)));  /* 01DD59 LDA */
    /* 01DD5C CMP */
    if ((A & 0xFF) >= 0x3D) goto L_01DD64;  /* 01DD5E BCS */
    /* 01DD60 CMP */
    if ((A & 0xFF) >= 0x1E) goto L_01DD6B;  /* 01DD62 BCS */
L_01DD64: TICK();
    wr8(ABS(0x0082), 0x3C);  /* 01DD64 LDM */
    goto L_01DDA6;  /* 01DD68 JMP */
L_01DD6B: TICK();
    mflag = 0;  /* 01DD6B CLM */
    A = rd16(ABS(0x0082));  /* 01DD6C LDA */
    mflag = 1;  /* 01DD6F SEM */
    /* 01DD70 CMP */
    if ((A & 0xFF) < 0x1F) goto L_01DD8F;  /* 01DD72 BCC */
    mflag = 0;  /* 01DD74 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_01DD88;  /* 01DD75 BBS */
    Y = rd16(ABS(0x0082));  /* 01DD7B LDY */
    S -= 3; F_01DE21();  /* 01DD7E JSRL */
    wr8(ABS(0x0082), (uint8_t)A);  /* 01DD82 STA */
    goto L_01DD8C;  /* 01DD85 JMP */
L_01DD88: TICK();
    mflag = 1;  /* 01DD88 SEM */
    t = (rd8(ABS(0x0082)) - 1) & 0xFF; wr8(ABS(0x0082), t);  /* 01DD89 DEC */
L_01DD8C: TICK();
    goto L_01DDA6;  /* 01DD8C JMP */
L_01DD8F: TICK();
    mflag = 0;  /* 01DD8F CLM */
    if ((rd16(ABS(0x023C)) & 0x0002) == 0x0002) goto L_01DDA6;  /* 01DD90 BBS */
    Y = 0x0010;  /* 01DD96 LDY */
    S -= 3; F_0080AA();  /* 01DD99 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0002);  /* 01DD9D SEB */
    S -= 3; F_00828C();  /* 01DDA2 JSRL */
L_01DDA6: TICK();
    goto L_01DDCC;  /* 01DDA6 JMP */
L_01DDA9: TICK();
    mflag = 0;  /* 01DDA9 CLM */
    A = rd16(ABS(0x0082));  /* 01DDAA LDA */
    mflag = 1;  /* 01DDAD SEM */
    /* 01DDAE CMP */
    if ((A & 0xFF) >= 0x60) goto L_01DDB9;  /* 01DDB0 BCS */
    SETLO(A, rd8(ABS(0x0082)));  /* 01DDB2 LDA */
    /* 01DDB5 CMP */
    if ((A & 0xFF) >= 0x3C) goto L_01DDC0;  /* 01DDB7 BCS */
L_01DDB9: TICK();
    wr8(ABS(0x0082), 0x5F);  /* 01DDB9 LDM */
    goto L_01DDCC;  /* 01DDBD JMP */
L_01DDC0: TICK();
    mflag = 0;  /* 01DDC0 CLM */
    A = rd16(ABS(0x0082));  /* 01DDC1 LDA */
    mflag = 1;  /* 01DDC4 SEM */
    /* 01DDC5 CMP */
    if ((A & 0xFF) < 0x3D) goto L_01DDCC;  /* 01DDC7 BCC */
    t = (rd8(ABS(0x0082)) - 1) & 0xFF; wr8(ABS(0x0082), t);  /* 01DDC9 DEC */
L_01DDCC: TICK();
    mflag = 1;  /* 01DDCC SEM */
    SETLO(A, rd8(DP(0x00)));  /* 01DDCD LDA */
    if ((A & 0xFF) == 0) goto L_01DDD6;  /* 01DDCF BEQ */
    SETLO(A, 0x9D);  /* 01DDD1 LDA */
    goto L_01DDD8;  /* 01DDD3 JMP */
L_01DDD6: TICK();
    SETLO(A, 0x20);  /* 01DDD6 LDA */
L_01DDD8: TICK();
    wr8(ABS(0x0236), (uint8_t)A);  /* 01DDD8 STA */
L_01DDDB: TICK();
    mflag = 0;  /* 01DDDB CLM */
    Y = pull16();  /* 01DDDC PLY */
    D = pull16();  /* 01DDDD PLD */
    S += 3; return;  /* 01DDDE RTL */
}

/* 01DDDF  callers: 01DCF3 */
void F_01DDDF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DDDF LDA */
    push16(D);  /* 01DDE1 PHD */
    D = A;  /* 01DDE2 TAD */
    A += 1;  /* 01DDE3 INC */
    A += 1;  /* 01DDE4 INC */
    push16(A);  /* 01DDE5 PHA */
    mflag = 1;  /* 01DDE6 SEM */
    wr8(DP(0x00), 0x05);  /* 01DDE7 LDM */
    wr8(DP(0x01), 0x01);  /* 01DDEA LDM */
L_01DDED: TICK();
    Y = 0x0032;  /* 01DDED LDY */
    mflag = 0;  /* 01DDF0 CLM */
    S -= 3; F_007D7F();  /* 01DDF1 JSRL */
    Y = 0x0004;  /* 01DDF5 LDY */
    S -= 3; F_008DC6();  /* 01DDF8 JSRL */
    mflag = 1;  /* 01DDFC SEM */
    SETLO(A, rd8(ABS(AD4)));  /* 01DDFD LDA [AN4 13V2] */
    /* 01DE00 CMP */
    if ((A & 0xFF) < 0x9F) goto L_01DE07;  /* 01DE02 BCC */
    wr8(DP(0x01), 0x00);  /* 01DE04 LDM */
L_01DE07: TICK();
    t = (rd8(DP(0x00)) - 1) & 0xFF; wr8(DP(0x00), t);  /* 01DE07 DEC */
    SETLO(A, rd8(DP(0x00)));  /* 01DE09 LDA */
    if ((A & 0xFF) == 0) goto L_01DE11;  /* 01DE0B BEQ */
    SETLO(A, rd8(DP(0x01)));  /* 01DE0D LDA */
    if ((A & 0xFF) != 0) goto L_01DDED;  /* 01DE0F BNE */
L_01DE11: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 01DE11 LDA */
    Y = pull16();  /* 01DE13 PLY */
    D = pull16();  /* 01DE14 PLD */
    S += 3; return;  /* 01DE15 RTL */
}

/* 01DE16  callers: 01A739 */
void F_01DE16(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 01DE16 SEM */
    wr8(ABS(0x0082), 0x64);  /* 01DE17 LDM */
    wr8(ABS(0x0236), 0x20);  /* 01DE1B LDM */
    mflag = 0;  /* 01DE1F CLM */
    S += 3; return;  /* 01DE20 RTL */
}

/* 01DE21  callers: 01DCE1, 01DD7E */
void F_01DE21(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DE21 LDA */
    push16(D);  /* 01DE23 PHD */
    D = A;  /* 01DE24 TAD */
    wr16(DP(0x00), Y);  /* 01DE25 STY */
    mflag = 1;  /* 01DE27 SEM */
    SETLO(B, 0x00);  /* 01DE28 LDA */
    SETLO(A, Y);  /* 01DE2B TYA */
    div8(0x06);  /* 01DE2C DIV */
    /* 01DE2F CMP */
    if ((B & 0xFF) == 0x00) goto L_01DE43;  /* 01DE32 BEQ */
    SETLO(B, 0x00);  /* 01DE34 LDA */
    SETLO(A, Y);  /* 01DE37 TYA */
    div8(0x06);  /* 01DE38 DIV */
    mpy8(0x06);  /* 01DE3B MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 01DE3E STA */
    goto L_01DE4C;  /* 01DE40 JMP */
L_01DE43: TICK();
    mflag = 0;  /* 01DE43 CLM */
    A = rd16(DP(0x00));  /* 01DE44 LDA */
    C = 0;  /* 01DE46 CLC */
    mflag = 1;  /* 01DE47 SEM */
    SETLO(A, adc8(A, 0xFA));  /* 01DE48 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 01DE4A STA */
L_01DE4C: TICK();
    D = pull16();  /* 01DE4C PLD */
    S += 3; return;  /* 01DE4D RTL */
}

/* 01DE4E  callers: 01A7C5 */
void F_01DE4E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DE4E LDA */
    push16(A);  /* 01DE50 PHA */
    if ((rd16(ABS(0x0246)) & 0x0200) == 0x0200) goto L_01DE5A;  /* 01DE51 BBS */
    goto L_01DF98;  /* 01DE57 JMP */
L_01DE5A: TICK();
    A = rd16(ABS(0x0332));  /* 01DE5A LDA */
    mflag = 1;  /* 01DE5D SEM */
    t = rd8(ABS(0x0333));  /* 01DE5E CMP */
    if ((A & 0xFF) == t) goto L_01DE66;  /* 01DE61 BEQ */
    goto L_01DF98;  /* 01DE63 JMP */
L_01DE66: TICK();
    SETLO(A, rd8(ABS(0x0115)));  /* 01DE66 LDA */
    /* 01DE69 CMP */
    if ((A & 0xFF) == 0x01) goto L_01DE70;  /* 01DE6B BEQ */
    goto L_01DF98;  /* 01DE6D JMP */
L_01DE70: TICK();
    SETLO(A, rd8(ABS(0x0117)));  /* 01DE70 LDA */
    if ((A & 0xFF) != 0) goto L_01DE81;  /* 01DE73 BNE */
    mflag = 0;  /* 01DE75 CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_01DE81;  /* 01DE76 BBC */
    mflag = 1;  /* 01DE7C SEM */
    wr8(ABS(0x0117), 0x01);  /* 01DE7D LDM */
L_01DE81: TICK();
    mflag = 0;  /* 01DE81 CLM */
    A = rd16(ABS(0x0117));  /* 01DE82 LDA */
    mflag = 1;  /* 01DE85 SEM */
    /* 01DE86 CMP */
    if ((A & 0xFF) != 0x01) goto L_01DEAC;  /* 01DE88 BNE */
    wr8(ABS(0x0117), 0x02);  /* 01DE8A LDM */
    wr8(ABS(0x029C), 0x05);  /* 01DE8E LDM */
    wr8(ABS(0x029D), 0x01);  /* 01DE92 LDM */
    SETLO(A, rd8(ABS(0x0376)));  /* 01DE96 LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 01DE99 STA */
    Y = 0x0000;  /* 01DE9C LDY */
    mflag = 0;  /* 01DE9F CLM */
    S -= 3; F_0196AA();  /* 01DEA0 JSRL */
    mflag = 1;  /* 01DEA4 SEM */
    wr8(ABS(0x03A0), 0x9D);  /* 01DEA5 LDM */
    goto L_01DF98;  /* 01DEA9 JMP */
L_01DEAC: TICK();
    mflag = 0;  /* 01DEAC CLM */
    A = rd16(ABS(0x0117));  /* 01DEAD LDA */
    mflag = 1;  /* 01DEB0 SEM */
    /* 01DEB1 CMP */
    if ((A & 0xFF) != 0x03) goto L_01DEEB;  /* 01DEB3 BNE */
    mflag = 0;  /* 01DEB5 CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0x0100) goto L_01DEC1;  /* 01DEB6 BBS */
    mflag = 1;  /* 01DEBC SEM */
    wr8(ABS(0x0117), 0x00);  /* 01DEBD LDM */
L_01DEC1: TICK();
    mflag = 0;  /* 01DEC1 CLM */
    if ((rd16(ABS(0x0240)) & 0x0001) == 0x0001) goto L_01DEE8;  /* 01DEC2 BBS */
    mflag = 1;  /* 01DEC8 SEM */
    wr8(ABS(0x0117), 0x04);  /* 01DEC9 LDM */
    wr8(ABS(0x029C), 0x06);  /* 01DECD LDM */
    wr8(ABS(0x029D), 0x02);  /* 01DED1 LDM */
    SETLO(A, rd8(ABS(0x037C)));  /* 01DED5 LDA */
    wr8(ABS(0x029E), (uint8_t)A);  /* 01DED8 STA */
    Y = 0x0000;  /* 01DEDB LDY */
    mflag = 0;  /* 01DEDE CLM */
    S -= 3; F_0196AA();  /* 01DEDF JSRL */
    mflag = 1;  /* 01DEE3 SEM */
    wr8(ABS(0x03A0), 0x9D);  /* 01DEE4 LDM */
L_01DEE8: TICK();
    goto L_01DF98;  /* 01DEE8 JMP */
L_01DEEB: TICK();
    mflag = 0;  /* 01DEEB CLM */
    A = rd16(ABS(0x0117));  /* 01DEEC LDA */
    mflag = 1;  /* 01DEEF SEM */
    /* 01DEF0 CMP */
    if ((A & 0xFF) != 0x05) goto L_01DF1C;  /* 01DEF2 BNE */
    mflag = 0;  /* 01DEF4 CLM */
    if ((rd16(ABS(0x0240)) & 0x0001) == 0) goto L_01DF19;  /* 01DEF5 BBC */
    mflag = 1;  /* 01DEFB SEM */
    wr8(ABS(0x0117), 0x06);  /* 01DEFC LDM */
    wr8(ABS(0x029C), 0x06);  /* 01DF00 LDM */
    wr8(ABS(0x029D), 0x01);  /* 01DF04 LDM */
    wr8(ABS(0x029E), 0x00);  /* 01DF08 LDM */
    Y = 0x0000;  /* 01DF0C LDY */
    mflag = 0;  /* 01DF0F CLM */
    S -= 3; F_0196AA();  /* 01DF10 JSRL */
    mflag = 1;  /* 01DF14 SEM */
    wr8(ABS(0x03A0), 0x9D);  /* 01DF15 LDM */
L_01DF19: TICK();
    goto L_01DF98;  /* 01DF19 JMP */
L_01DF1C: TICK();
    mflag = 0;  /* 01DF1C CLM */
    A = rd16(ABS(0x0117));  /* 01DF1D LDA */
    mflag = 1;  /* 01DF20 SEM */
    /* 01DF21 CMP */
    if ((A & 0xFF) != 0x07) goto L_01DF45;  /* 01DF23 BNE */
    wr8(ABS(0x0117), 0x08);  /* 01DF25 LDM */
    wr8(ABS(0x029C), 0x06);  /* 01DF29 LDM */
    wr8(ABS(0x029D), 0x01);  /* 01DF2D LDM */
    wr8(ABS(0x029E), 0x00);  /* 01DF31 LDM */
    Y = 0x0000;  /* 01DF35 LDY */
    mflag = 0;  /* 01DF38 CLM */
    S -= 3; F_0196AA();  /* 01DF39 JSRL */
    mflag = 1;  /* 01DF3D SEM */
    wr8(ABS(0x03A0), 0x9D);  /* 01DF3E LDM */
    goto L_01DF98;  /* 01DF42 JMP */
L_01DF45: TICK();
    mflag = 1;  /* 01DF45 SEM */
    SETLO(A, rd8(ABS(0x03A0)));  /* 01DF46 LDA */
    if ((A & 0xFF) != 0) goto L_01DF98;  /* 01DF49 BNE */
    SETLO(A, rd8(ABS(0x0116)));  /* 01DF4B LDA */
    /* 01DF4E CMP */
    if ((A & 0xFF) != 0x09) goto L_01DF5D;  /* 01DF50 BNE */
    wr8(ABS(0x0116), 0x08);  /* 01DF52 LDM */
    wr8(ABS(0x03A0), 0x20);  /* 01DF56 LDM */
    goto L_01DF98;  /* 01DF5A JMP */
L_01DF5D: TICK();
    mflag = 0;  /* 01DF5D CLM */
    A = rd16(ABS(0x0116));  /* 01DF5E LDA */
    mflag = 1;  /* 01DF61 SEM */
    /* 01DF62 CMP */
    if ((A & 0xFF) != 0x0A) goto L_01DF98;  /* 01DF64 BNE */
    SETLO(A, rd8(ABS(0x0117)));  /* 01DF66 LDA */
    /* 01DF69 CMP */
    if ((A & 0xFF) != 0x02) goto L_01DF74;  /* 01DF6B BNE */
    wr8(ABS(0x0117), 0x00);  /* 01DF6D LDM */
    goto L_01DF98;  /* 01DF71 JMP */
L_01DF74: TICK();
    mflag = 1;  /* 01DF74 SEM */
    /* 01DF75 CMP */
    if ((A & 0xFF) != 0x04) goto L_01DF80;  /* 01DF77 BNE */
    wr8(ABS(0x0117), 0x03);  /* 01DF79 LDM */
    goto L_01DF98;  /* 01DF7D JMP */
L_01DF80: TICK();
    mflag = 1;  /* 01DF80 SEM */
    /* 01DF81 CMP */
    if ((A & 0xFF) != 0x06) goto L_01DF8C;  /* 01DF83 BNE */
    wr8(ABS(0x0117), 0x07);  /* 01DF85 LDM */
    goto L_01DF98;  /* 01DF89 JMP */
L_01DF8C: TICK();
    mflag = 1;  /* 01DF8C SEM */
    /* 01DF8D CMP */
    if ((A & 0xFF) != 0x08) goto L_01DF98;  /* 01DF8F BNE */
    wr8(ABS(0x0117), 0x07);  /* 01DF91 LDM */
    goto L_01DF98;  /* 01DF95 JMP */
L_01DF98: TICK();
    mflag = 0;  /* 01DF98 CLM */
    Y = pull16();  /* 01DF99 PLY */
    S += 3; return;  /* 01DF9A RTL */
}

/* 01DF9B  callers: 0199BF */
void F_01DF9B(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0246)) & 0x0200) == 0) goto L_01DFDD;  /* 01DF9B BBC */
    A = rd16(ABS(0x0117));  /* 01DFA1 LDA */
    mflag = 1;  /* 01DFA4 SEM */
    /* 01DFA5 CMP */
    if ((A & 0xFF) != 0x02) goto L_01DFB0;  /* 01DFA7 BNE */
    wr8(ABS(0x0117), 0x03);  /* 01DFA9 LDM */
    goto L_01DFDD;  /* 01DFAD JMP */
L_01DFB0: TICK();
    mflag = 0;  /* 01DFB0 CLM */
    A = rd16(ABS(0x0117));  /* 01DFB1 LDA */
    mflag = 1;  /* 01DFB4 SEM */
    /* 01DFB5 CMP */
    if ((A & 0xFF) != 0x04) goto L_01DFC0;  /* 01DFB7 BNE */
    wr8(ABS(0x0117), 0x05);  /* 01DFB9 LDM */
    goto L_01DFDD;  /* 01DFBD JMP */
L_01DFC0: TICK();
    mflag = 0;  /* 01DFC0 CLM */
    A = rd16(ABS(0x0117));  /* 01DFC1 LDA */
    mflag = 1;  /* 01DFC4 SEM */
    /* 01DFC5 CMP */
    if ((A & 0xFF) != 0x06) goto L_01DFD0;  /* 01DFC7 BNE */
    wr8(ABS(0x0117), 0x03);  /* 01DFC9 LDM */
    goto L_01DFDD;  /* 01DFCD JMP */
L_01DFD0: TICK();
    mflag = 0;  /* 01DFD0 CLM */
    A = rd16(ABS(0x0117));  /* 01DFD1 LDA */
    mflag = 1;  /* 01DFD4 SEM */
    /* 01DFD5 CMP */
    if ((A & 0xFF) != 0x08) goto L_01DFDD;  /* 01DFD7 BNE */
    wr8(ABS(0x0117), 0x00);  /* 01DFD9 LDM */
L_01DFDD: TICK();
    mflag = 0;  /* 01DFDD CLM */
    S += 3; return;  /* 01DFDE RTL */
}

/* 01DFDF  callers: 0067FD */
void F_01DFDF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01DFDF LDA */
    push16(A);  /* 01DFE1 PHA */
    mflag = 1;  /* 01DFE2 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_01DFED;  /* 01DFE3 BBC */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_01DFF3;  /* 01DFE8 BBS */
L_01DFED: TICK();
    mflag = 0;  /* 01DFED CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0200);  /* 01DFEE CLB */
L_01DFF3: TICK();
    mflag = 0;  /* 01DFF3 CLM */
    if ((rd16(ABS(0x0246)) & 0x0200) == 0) goto L_01E004;  /* 01DFF4 BBC */
    mflag = 1;  /* 01DFFA SEM */
    wr8(ABS(0x0117), 0x00);  /* 01DFFB LDM */
    mflag = 0;  /* 01DFFF CLM */
    S -= 3; F_004A5C();  /* 01E000 JSRL */
L_01E004: TICK();
    Y = pull16();  /* 01E004 PLY */
    S += 3; return;  /* 01E005 RTL */
}

