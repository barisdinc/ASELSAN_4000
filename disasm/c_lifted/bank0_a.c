/* bank0_a.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 002000  callers: 019B9A */
void reset_entry(void)
{
    uint32_t t; (void)t;
    X = 0x0879;  /* 002000 LDX */
    S = X;  /* 002003 TXS */
    goto L_00FE60;  /* 002004 JMPL */
L_00FE60: TICK();
    DT = 0x00;  /* 00FE60 LDT */
    A = 0x062A;  /* 00FE63 LDA */
    D = A;  /* 00FE66 TAD */
    push16(A);  /* 00FE67 PHA */
    S -= 2; F_00FE73();  /* 00FE68 JSR */
    S -= 3; c_main();  /* 00FE6B JSRL */
    goto L_00FFC6;  /* 00FE6F JMPL */
L_00FFC6: TICK();
    goto L_00FFC6;  /* 00FFC6 BRA */
}

/* 002008  callers:  */
void isr_UART0_RX(void)
{
    uint32_t t; (void)t;
    goto L_019469;  /* 002008 JMPL */
L_019469: TICK();
    clp(0x39);  /* 019469 CLP */
    psh(0x3F);  /* 01946B PSH */
    DT = 0x00;  /* 01946D LDT */
    A = D;  /* 019470 TDA */
    A = adc16(A, 0x0064);  /* 019471 ADC */
    D = A;  /* 019474 TAD */
    A += 1;  /* 019475 INC */
    push16(A);  /* 019476 PHA */
    I = 0;  /* 019477 CLI */
    mflag = 1;  /* 019478 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 019479 LDM */
    if ((rd8(ABS(U0C1)) & 0x80) == 0) goto L_01948A;  /* 01947D BBC */
    SETLO(A, rd8(ABS(U0RB)));  /* 019482 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 019485 STA */
    goto L_0195AD;  /* 019487 JMP */
L_01948A: TICK();
    mflag = 1;  /* 01948A SEM */
    SETLO(A, rd8(ABS(U0RB)));  /* 01948B LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 01948E STA */
    SETLO(A, rd8(ABS(0x0080)));  /* 019490 LDA */
    SETLO(A, A & 0x01);  /* 019493 AND */
    Z = ((A & 0xFF) == 0);
    if (Z) goto L_019499;  /* 019495 BEQ */
    SETLO(A, 0x01);  /* 019497 LDA */
    Z = ((A & 0xFF) == 0);
L_019499: TICK();
    if (Z) goto L_0194A0;  /* 019499 BEQ */
    mflag = 0;  /* 01949B CLM */
    Y = pull16();  /* 01949C PLY */
    pul(0x3F);  /* 01949D PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01949F RTI */
L_0194A0: TICK();
    SETLO(A, rd8(ABS(0x0115)));  /* 0194A0 LDA */
    /* 0194A3 CMP */
    if ((A & 0xFF) != 0x01) goto L_0194CD;  /* 0194A5 BNE */
    wr8(ABS(0x010E), 0x00);  /* 0194A7 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 0194AB LDA */
    /* 0194AD CMP */
    if ((A & 0xFF) != 0x16) goto L_0194B8;  /* 0194AF BNE */
    wr8(ABS(0x0115), 0x02);  /* 0194B1 LDM */
    goto L_0194CA;  /* 0194B5 JMP */
L_0194B8: TICK();
    mflag = 0;  /* 0194B8 CLM */
    A = rd16(DP(0x00));  /* 0194B9 LDA */
    mflag = 1;  /* 0194BB SEM */
    /* 0194BC CMP */
    if ((A & 0xFF) != 0xF8) goto L_0194CA;  /* 0194BE BNE */
    if ((rd8(ABS(0x0334)) & 0x04) == 0) goto L_0194CA;  /* 0194C0 BBC */
    mflag = 0;  /* 0194C5 CLM */
    S -= 3; F_0099BD();  /* 0194C6 JSRL */
L_0194CA: TICK();
    goto L_0195AD;  /* 0194CA JMP */
L_0194CD: TICK();
    mflag = 0;  /* 0194CD CLM */
    A = rd16(ABS(0x0115));  /* 0194CE LDA */
    mflag = 1;  /* 0194D1 SEM */
    /* 0194D2 CMP */
    if ((A & 0xFF) != 0x02) goto L_0194E9;  /* 0194D4 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0194D6 LDA */
    /* 0194D8 CMP */
    if ((A & 0xFF) != 0x10) goto L_0194E1;  /* 0194DA BNE */
    SETLO(A, 0x03);  /* 0194DC LDA */
    goto L_0194E3;  /* 0194DE JMP */
L_0194E1: TICK();
    SETLO(A, 0x01);  /* 0194E1 LDA */
L_0194E3: TICK();
    wr8(ABS(0x0115), (uint8_t)A);  /* 0194E3 STA */
    goto L_0195AD;  /* 0194E6 JMP */
L_0194E9: TICK();
    mflag = 0;  /* 0194E9 CLM */
    A = rd16(ABS(0x0115));  /* 0194EA LDA */
    mflag = 1;  /* 0194ED SEM */
    /* 0194EE CMP */
    if ((A & 0xFF) != 0x03) goto L_019505;  /* 0194F0 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0194F2 LDA */
    /* 0194F4 CMP */
    if ((A & 0xFF) != 0x02) goto L_0194FD;  /* 0194F6 BNE */
    SETLO(A, 0x04);  /* 0194F8 LDA */
    goto L_0194FF;  /* 0194FA JMP */
L_0194FD: TICK();
    SETLO(A, 0x01);  /* 0194FD LDA */
L_0194FF: TICK();
    wr8(ABS(0x0115), (uint8_t)A);  /* 0194FF STA */
    goto L_0195AD;  /* 019502 JMP */
L_019505: TICK();
    mflag = 0;  /* 019505 CLM */
    A = rd16(ABS(0x0115));  /* 019506 LDA */
    mflag = 1;  /* 019509 SEM */
    /* 01950A CMP */
    if ((A & 0xFF) != 0x04) goto L_01953F;  /* 01950C BNE */
    SETLO(A, rd8(DP(0x00)));  /* 01950E LDA */
    /* 019510 CMP */
    if ((A & 0xFF) != 0x10) goto L_01951B;  /* 019512 BNE */
    wr8(ABS(0x0115), 0x05);  /* 019514 LDM */
    goto L_01953C;  /* 019518 JMP */
L_01951B: TICK();
    mflag = 0;  /* 01951B CLM */
    A = rd16(ABS(0x010E));  /* 01951C LDA */
    mflag = 1;  /* 01951F SEM */
    /* 019520 CMP */
    if ((A & 0xFF) >= 0x8B) goto L_019538;  /* 019522 BCS */
    SETLO(A, rd8(ABS(0x010E)));  /* 019524 LDA */
    t = (rd8(ABS(0x010E)) + 1) & 0xFF; wr8(ABS(0x010E), t);  /* 019527 INC */
    mflag = 0;  /* 01952A CLM */
    A &= 0x00FF;  /* 01952B AND */
    X = A;  /* 01952E TAX */
    A = rd16(DP(0x00));  /* 01952F LDA */
    mflag = 1;  /* 019531 SEM */
    wr8((ABS(0x0083) + X), (uint8_t)A);  /* 019532 STA */
    goto L_01953C;  /* 019535 JMP */
L_019538: TICK();
    I = 1;  /* 019538 SEI */
    goto L_019538;  /* 019539 JMP */
L_01953C: TICK();
    goto L_0195AD;  /* 01953C JMP */
L_01953F: TICK();
    mflag = 0;  /* 01953F CLM */
    A = rd16(ABS(0x0115));  /* 019540 LDA */
    mflag = 1;  /* 019543 SEM */
    /* 019544 CMP */
    if ((A & 0xFF) != 0x05) goto L_01958C;  /* 019546 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 019548 LDA */
    /* 01954A CMP */
    if ((A & 0xFF) != 0x10) goto L_019571;  /* 01954C BNE */
    wr8(ABS(0x0115), 0x04);  /* 01954E LDM */
    SETLO(A, rd8(ABS(0x010E)));  /* 019552 LDA */
    /* 019555 CMP */
    if ((A & 0xFF) >= 0x8B) goto L_01956A;  /* 019557 BCS */
    t = (rd8(ABS(0x010E)) + 1) & 0xFF; wr8(ABS(0x010E), t);  /* 019559 INC */
    mflag = 0;  /* 01955C CLM */
    A &= 0x00FF;  /* 01955D AND */
    X = A;  /* 019560 TAX */
    A = rd16(DP(0x00));  /* 019561 LDA */
    mflag = 1;  /* 019563 SEM */
    wr8((ABS(0x0083) + X), (uint8_t)A);  /* 019564 STA */
    goto L_01956E;  /* 019567 JMP */
L_01956A: TICK();
    I = 1;  /* 01956A SEI */
    goto L_01956A;  /* 01956B JMP */
L_01956E: TICK();
    goto L_019589;  /* 01956E JMP */
L_019571: TICK();
    mflag = 0;  /* 019571 CLM */
    A = rd16(DP(0x00));  /* 019572 LDA */
    mflag = 1;  /* 019574 SEM */
    /* 019575 CMP */
    if ((A & 0xFF) != 0x03) goto L_019580;  /* 019577 BNE */
    wr8(ABS(0x0115), 0x06);  /* 019579 LDM */
    goto L_019589;  /* 01957D JMP */
L_019580: TICK();
    mflag = 1;  /* 019580 SEM */
    wr8(ABS(0x0115), 0x01);  /* 019581 LDM */
    wr8(ABS(0x010E), 0x00);  /* 019585 LDM */
L_019589: TICK();
    goto L_0195AD;  /* 019589 JMP */
L_01958C: TICK();
    mflag = 0;  /* 01958C CLM */
    A = rd16(ABS(0x0115));  /* 01958D LDA */
    mflag = 1;  /* 019590 SEM */
    /* 019591 CMP */
    if ((A & 0xFF) != 0x06) goto L_01959C;  /* 019593 BNE */
    wr8(ABS(0x0115), 0x07);  /* 019595 LDM */
    goto L_0195AD;  /* 019599 JMP */
L_01959C: TICK();
    mflag = 0;  /* 01959C CLM */
    A = rd16(ABS(0x0115));  /* 01959D LDA */
    mflag = 1;  /* 0195A0 SEM */
    /* 0195A1 CMP */
    if ((A & 0xFF) != 0x07) goto L_0195AD;  /* 0195A3 BNE */
    wr8(ABS(0x0115), 0x01);  /* 0195A5 LDM */
    wr8(ABS(0x0080), rd8(ABS(0x0080)) | 0x01);  /* 0195A9 SEB */
L_0195AD: TICK();
    mflag = 0;  /* 0195AD CLM */
    Y = pull16();  /* 0195AE PLY */
    pul(0x3F);  /* 0195AF PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 0195B1 RTI */
}

/* 00200C  callers:  */
void isr_UART0_TX(void)
{
    uint32_t t; (void)t;
    goto L_0195B2;  /* 00200C JMPL */
L_0195B2: TICK();
    clp(0x39);  /* 0195B2 CLP */
    psh(0x25);  /* 0195B4 PSH */
    DT = 0x00;  /* 0195B6 LDT */
    I = 0;  /* 0195B9 CLI */
    mflag = 1;  /* 0195BA SEM */
    wr8(ABS(0x01E9), 0x00);  /* 0195BB LDM */
    t = (rd8(ABS(0x0332)) + 1) & 0xFF; wr8(ABS(0x0332), t);  /* 0195BF INC */
    SETLO(A, rd8(ABS(0x0332)));  /* 0195C2 LDA */
    t = rd8(ABS(0x0333));  /* 0195C5 CMP */
    if ((A & 0xFF) == t) goto L_0195D6;  /* 0195C8 BEQ */
    mflag = 0;  /* 0195CA CLM */
    A &= 0x00FF;  /* 0195CB AND */
    X = A;  /* 0195CE TAX */
    A = rd16((ABS(0x02E2) + X));  /* 0195CF LDA */
    mflag = 1;  /* 0195D2 SEM */
    wr8(ABS(U0TB), (uint8_t)A);  /* 0195D3 STA */
L_0195D6: TICK();
    mflag = 0;  /* 0195D6 CLM */
    pul(0x25);  /* 0195D7 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 0195D9 RTI */
}

/* 002010  callers:  */
void isr_ZERODIV(void)
{
    uint32_t t; (void)t;
    goto L_01B9A0;  /* 002010 JMPL */
L_01B9A0: TICK();
    clp(0x39);  /* 01B9A0 CLP */
    psh(0x20);  /* 01B9A2 PSH */
    DT = 0x00;  /* 01B9A4 LDT */
    cpu_stop();  /* 01B9A7 STP */
}

/* 002014  callers:  */
void isr_BRK(void)
{
    uint32_t t; (void)t;
    goto L_01B9AB;  /* 002014 JMPL */
L_01B9AB: TICK();
    clp(0x39);  /* 01B9AB CLP */
    psh(0x20);  /* 01B9AD PSH */
    DT = 0x00;  /* 01B9AF LDT */
    cpu_stop();  /* 01B9B2 STP */
}

/* 002018  callers:  */
void isr_DBC(void)
{
    uint32_t t; (void)t;
    goto L_01B9B6;  /* 002018 JMPL */
L_01B9B6: TICK();
    clp(0x39);  /* 01B9B6 CLP */
    psh(0x20);  /* 01B9B8 PSH */
    DT = 0x00;  /* 01B9BA LDT */
    cpu_stop();  /* 01B9BD STP */
}

/* 00201C  callers:  */
void isr_WDT(void)
{
    uint32_t t; (void)t;
    goto L_01B9C1;  /* 00201C JMPL */
L_01B9C1: TICK();
    clp(0x39);  /* 01B9C1 CLP */
    psh(0x3F);  /* 01B9C3 PSH */
    DT = 0x00;  /* 01B9C5 LDT */
    A = D;  /* 01B9C8 TDA */
    A = adc16(A, 0x0064);  /* 01B9C9 ADC */
    D = A;  /* 01B9CC TAD */
    push16(A);  /* 01B9CD PHA */
    if ((rd16(ABS(0x023E)) & 0x4000) == 0) goto L_01B9E1;  /* 01B9CE BBC */
    S -= 3; i2c_init_00713E();  /* 01B9D4 JSRL */
    S -= 3; F_00968D();  /* 01B9D8 JSRL */
    mflag = 1;  /* 01B9DC SEM */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 01B9DD SEB */
L_01B9E1: TICK();
    mflag = 0;  /* 01B9E1 CLM */
    Y = pull16();  /* 01B9E2 PLY */
    pul(0x3F);  /* 01B9E3 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01B9E5 RTI */
}

/* 002020  callers:  */
void isr_TB2(void)
{
    uint32_t t; (void)t;
    goto L_01B9E6;  /* 002020 JMPL */
L_01B9E6: TICK();
    clp(0x39);  /* 01B9E6 CLP */
    psh(0x3F);  /* 01B9E8 PSH */
    DT = 0x00;  /* 01B9EA LDT */
    A = D;  /* 01B9ED TDA */
    A = adc16(A, 0x0064);  /* 01B9EE ADC */
    D = A;  /* 01B9F1 TAD */
    push16(A);  /* 01B9F2 PHA */
    I = 0;  /* 01B9F3 CLI */
    mflag = 1;  /* 01B9F4 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 01B9F5 LDM */
    t = (rd8(ABS(0x0457)) + 1) & 0xFF; wr8(ABS(0x0457), t);  /* 01B9F9 INC */
    SETLO(A, rd8(ABS(0x04C0)));  /* 01B9FC LDA */
    if ((A & 0xFF) == 0) goto L_01BA04;  /* 01B9FF BEQ */
    t = (rd8(ABS(0x04C0)) - 1) & 0xFF; wr8(ABS(0x04C0), t);  /* 01BA01 DEC */
L_01BA04: TICK();
    SETLO(A, rd8(ABS(0x04BA)));  /* 01BA04 LDA */
    if ((A & 0xFF) == 0) goto L_01BA0C;  /* 01BA07 BEQ */
    t = (rd8(ABS(0x04BA)) - 1) & 0xFF; wr8(ABS(0x04BA), t);  /* 01BA09 DEC */
L_01BA0C: TICK();
    SETLO(A, rd8(ABS(0x022A)));  /* 01BA0C LDA */
    if ((A & 0xFF) == 0) goto L_01BA14;  /* 01BA0F BEQ */
    t = (rd8(ABS(0x022A)) - 1) & 0xFF; wr8(ABS(0x022A), t);  /* 01BA11 DEC */
L_01BA14: TICK();
    SETLO(A, rd8(ABS(0x0236)));  /* 01BA14 LDA */
    if ((A & 0xFF) == 0) goto L_01BA1C;  /* 01BA17 BEQ */
    t = (rd8(ABS(0x0236)) - 1) & 0xFF; wr8(ABS(0x0236), t);  /* 01BA19 DEC */
L_01BA1C: TICK();
    SETLO(A, rd8(ABS(0x0230)));  /* 01BA1C LDA */
    if ((A & 0xFF) == 0) goto L_01BA24;  /* 01BA1F BEQ */
    t = (rd8(ABS(0x0230)) - 1) & 0xFF; wr8(ABS(0x0230), t);  /* 01BA21 DEC */
L_01BA24: TICK();
    SETLO(A, rd8(ABS(0x0232)));  /* 01BA24 LDA */
    if ((A & 0xFF) == 0) goto L_01BA2C;  /* 01BA27 BEQ */
    t = (rd8(ABS(0x0232)) - 1) & 0xFF; wr8(ABS(0x0232), t);  /* 01BA29 DEC */
L_01BA2C: TICK();
    SETLO(A, rd8(ABS(0x0237)));  /* 01BA2C LDA */
    if ((A & 0xFF) == 0) goto L_01BA34;  /* 01BA2F BEQ */
    t = (rd8(ABS(0x0237)) - 1) & 0xFF; wr8(ABS(0x0237), t);  /* 01BA31 DEC */
L_01BA34: TICK();
    SETLO(A, rd8(ABS(0x0238)));  /* 01BA34 LDA */
    if ((A & 0xFF) == 0) goto L_01BA3C;  /* 01BA37 BEQ */
    t = (rd8(ABS(0x0238)) - 1) & 0xFF; wr8(ABS(0x0238), t);  /* 01BA39 DEC */
L_01BA3C: TICK();
    SETLO(A, rd8(ABS(0x022F)));  /* 01BA3C LDA */
    if ((A & 0xFF) == 0) goto L_01BA44;  /* 01BA3F BEQ */
    t = (rd8(ABS(0x022F)) - 1) & 0xFF; wr8(ABS(0x022F), t);  /* 01BA41 DEC */
L_01BA44: TICK();
    SETLO(A, rd8(ABS(0x0233)));  /* 01BA44 LDA */
    if ((A & 0xFF) == 0) goto L_01BA4C;  /* 01BA47 BEQ */
    t = (rd8(ABS(0x0233)) - 1) & 0xFF; wr8(ABS(0x0233), t);  /* 01BA49 DEC */
L_01BA4C: TICK();
    SETLO(A, rd8(ABS(0x0207)));  /* 01BA4C LDA */
    if ((A & 0xFF) == 0) goto L_01BA54;  /* 01BA4F BEQ */
    t = (rd8(ABS(0x0207)) - 1) & 0xFF; wr8(ABS(0x0207), t);  /* 01BA51 DEC */
L_01BA54: TICK();
    SETLO(A, rd8(ABS(0x0111)));  /* 01BA54 LDA */
    if ((A & 0xFF) == 0) goto L_01BA5C;  /* 01BA57 BEQ */
    t = (rd8(ABS(0x0111)) - 1) & 0xFF; wr8(ABS(0x0111), t);  /* 01BA59 DEC */
L_01BA5C: TICK();
    SETLO(A, rd8(ABS(0x0112)));  /* 01BA5C LDA */
    if ((A & 0xFF) == 0) goto L_01BA64;  /* 01BA5F BEQ */
    t = (rd8(ABS(0x0112)) - 1) & 0xFF; wr8(ABS(0x0112), t);  /* 01BA61 DEC */
L_01BA64: TICK();
    SETLO(A, rd8(ABS(0x020A)));  /* 01BA64 LDA */
    if ((A & 0xFF) == 0) goto L_01BA6C;  /* 01BA67 BEQ */
    t = (rd8(ABS(0x020A)) - 1) & 0xFF; wr8(ABS(0x020A), t);  /* 01BA69 DEC */
L_01BA6C: TICK();
    SETLO(A, rd8(ABS(0x0209)));  /* 01BA6C LDA */
    if ((A & 0xFF) == 0) goto L_01BA74;  /* 01BA6F BEQ */
    t = (rd8(ABS(0x0209)) - 1) & 0xFF; wr8(ABS(0x0209), t);  /* 01BA71 DEC */
L_01BA74: TICK();
    SETLO(A, rd8(ABS(0x03A0)));  /* 01BA74 LDA */
    if ((A & 0xFF) == 0) goto L_01BA7C;  /* 01BA77 BEQ */
    t = (rd8(ABS(0x03A0)) - 1) & 0xFF; wr8(ABS(0x03A0), t);  /* 01BA79 DEC */
L_01BA7C: TICK();
    SETLO(A, rd8(ABS(0x01B7)));  /* 01BA7C LDA */
    if ((A & 0xFF) == 0) goto L_01BA84;  /* 01BA7F BEQ */
    t = (rd8(ABS(0x01B7)) - 1) & 0xFF; wr8(ABS(0x01B7), t);  /* 01BA81 DEC */
L_01BA84: TICK();
    SETLO(A, rd8(ABS(0x041D)));  /* 01BA84 LDA */
    if ((A & 0xFF) == 0) goto L_01BA8C;  /* 01BA87 BEQ */
    t = (rd8(ABS(0x041D)) - 1) & 0xFF; wr8(ABS(0x041D), t);  /* 01BA89 DEC */
L_01BA8C: TICK();
    SETLO(A, rd8(ABS(0x041F)));  /* 01BA8C LDA */
    if ((A & 0xFF) == 0) goto L_01BA94;  /* 01BA8F BEQ */
    t = (rd8(ABS(0x041F)) - 1) & 0xFF; wr8(ABS(0x041F), t);  /* 01BA91 DEC */
L_01BA94: TICK();
    SETLO(A, rd8(ABS(0x0424)));  /* 01BA94 LDA */
    if ((A & 0xFF) == 0) goto L_01BA9C;  /* 01BA97 BEQ */
    t = (rd8(ABS(0x0424)) - 1) & 0xFF; wr8(ABS(0x0424), t);  /* 01BA99 DEC */
L_01BA9C: TICK();
    mflag = 0;  /* 01BA9C CLM */
    X = rd16(ABS(0x03F7));  /* 01BA9D LDX */
    if (X == 0) goto L_01BAA5;  /* 01BAA0 BEQ */
    t = (rd16(ABS(0x03F7)) - 1) & 0xFFFF; wr16(ABS(0x03F7), t);  /* 01BAA2 DEC */
L_01BAA5: TICK();
    mflag = 1;  /* 01BAA5 SEM */
    t = (rd8(ABS(0x022B)) - 1) & 0xFF; wr8(ABS(0x022B), t);  /* 01BAA6 DEC */
    SETLO(A, rd8(ABS(0x022B)));  /* 01BAA9 LDA */
    C = ((A & 0xFF) >= 0x7E);  /* 01BAAC CMP */
    if (!C) goto L_01BACD;  /* 01BAAE BCC */
    wr8(ABS(0x022B), 0x7C);  /* 01BAB0 LDM */
    mflag = 0;  /* 01BAB4 CLM */
    A = rd16(ABS(0x01F1));  /* 01BAB5 LDA */
    B = rd16(ABS(0x01F3));  /* 01BAB8 LDA */
    A = adc16(A, 0x0003);  /* 01BABC ADC */
    B = adc16(B, 0x0000);  /* 01BABF ADC */
    wr16(ABS(0x01F1), A);  /* 01BAC3 STA */
    wr16(ABS(0x01F3), B);  /* 01BAC6 STA */
    goto L_01BB15;  /* 01BACA JMP */
L_01BACD: TICK();
    mflag = 1;  /* 01BACD SEM */
    SETLO(B, 0x00);  /* 01BACE LDA */
    SETLO(A, rd8(ABS(0x022B)));  /* 01BAD1 LDA */
    div8(0x1F);  /* 01BAD4 DIV */
    /* 01BAD7 CMP */
    if ((B & 0xFF) != 0x00) goto L_01BB15;  /* 01BADA BNE */
    SETLO(A, rd8(ABS(0x041E)));  /* 01BADC LDA */
    if ((A & 0xFF) == 0) goto L_01BAE4;  /* 01BADF BEQ */
    t = (rd8(ABS(0x041E)) - 1) & 0xFF; wr8(ABS(0x041E), t);  /* 01BAE1 DEC */
L_01BAE4: TICK();
    SETLO(A, rd8(ABS(0x0420)));  /* 01BAE4 LDA */
    if ((A & 0xFF) == 0) goto L_01BAEC;  /* 01BAE7 BEQ */
    t = (rd8(ABS(0x0420)) - 1) & 0xFF; wr8(ABS(0x0420), t);  /* 01BAE9 DEC */
L_01BAEC: TICK();
    SETLO(A, rd8(ABS(0x0421)));  /* 01BAEC LDA */
    if ((A & 0xFF) == 0) goto L_01BAF4;  /* 01BAEF BEQ */
    t = (rd8(ABS(0x0421)) - 1) & 0xFF; wr8(ABS(0x0421), t);  /* 01BAF1 DEC */
L_01BAF4: TICK();
    SETLO(A, rd8(ABS(0x0422)));  /* 01BAF4 LDA */
    if ((A & 0xFF) == 0) goto L_01BAFC;  /* 01BAF7 BEQ */
    t = (rd8(ABS(0x0422)) - 1) & 0xFF; wr8(ABS(0x0422), t);  /* 01BAF9 DEC */
L_01BAFC: TICK();
    SETLO(A, rd8(ABS(0x0423)));  /* 01BAFC LDA */
    if ((A & 0xFF) == 0) goto L_01BB04;  /* 01BAFF BEQ */
    t = (rd8(ABS(0x0423)) - 1) & 0xFF; wr8(ABS(0x0423), t);  /* 01BB01 DEC */
L_01BB04: TICK();
    mflag = 0;  /* 01BB04 CLM */
    X = rd16(ABS(0x0405));  /* 01BB05 LDX */
    if (X == 0) goto L_01BB0D;  /* 01BB08 BEQ */
    t = (rd16(ABS(0x0405)) - 1) & 0xFFFF; wr16(ABS(0x0405), t);  /* 01BB0A DEC */
L_01BB0D: TICK();
    X = rd16(ABS(0x0407));  /* 01BB0D LDX */
    if (X == 0) goto L_01BB15;  /* 01BB10 BEQ */
    t = (rd16(ABS(0x0407)) - 1) & 0xFFFF; wr16(ABS(0x0407), t);  /* 01BB12 DEC */
L_01BB15: TICK();
    mflag = 0;  /* 01BB15 CLM */
    X = rd16(ABS(0x0218));  /* 01BB16 LDX */
    if (X == 0) goto L_01BB1E;  /* 01BB19 BEQ */
    t = (rd16(ABS(0x0218)) - 1) & 0xFFFF; wr16(ABS(0x0218), t);  /* 01BB1B DEC */
L_01BB1E: TICK();
    X = rd16(ABS(0x021A));  /* 01BB1E LDX */
    if (X == 0) goto L_01BB26;  /* 01BB21 BEQ */
    t = (rd16(ABS(0x021A)) - 1) & 0xFFFF; wr16(ABS(0x021A), t);  /* 01BB23 DEC */
L_01BB26: TICK();
    X = rd16(ABS(0x021C));  /* 01BB26 LDX */
    if (X == 0) goto L_01BB2E;  /* 01BB29 BEQ */
    t = (rd16(ABS(0x021C)) - 1) & 0xFFFF; wr16(ABS(0x021C), t);  /* 01BB2B DEC */
L_01BB2E: TICK();
    X = rd16(ABS(0x0222));  /* 01BB2E LDX */
    if (X == 0) goto L_01BB36;  /* 01BB31 BEQ */
    t = (rd16(ABS(0x0222)) - 1) & 0xFFFF; wr16(ABS(0x0222), t);  /* 01BB33 DEC */
L_01BB36: TICK();
    X = rd16(ABS(0x0214));  /* 01BB36 LDX */
    if (X == 0) goto L_01BB83;  /* 01BB39 BEQ */
    t = (rd16(ABS(0x0214)) - 1) & 0xFFFF; wr16(ABS(0x0214), t);  /* 01BB3B DEC */
    X = rd16(ABS(0x0214));  /* 01BB3E LDX */
    if (X != 0) goto L_01BB4F;  /* 01BB41 BNE */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0001);  /* 01BB43 CLB */
    S -= 3; F_008053();  /* 01BB48 JSRL */
    goto L_01BB83;  /* 01BB4C JMP */
L_01BB4F: TICK();
    mflag = 1;  /* 01BB4F SEM */
    SETLO(A, rd8(ABS(0x0234)));  /* 01BB50 LDA */
    if ((A & 0xFF) == 0) goto L_01BB78;  /* 01BB53 BEQ */
    t = (rd8(ABS(0x0235)) - 1) & 0xFF; wr8(ABS(0x0235), t);  /* 01BB55 DEC */
    SETLO(A, rd8(ABS(0x0235)));  /* 01BB58 LDA */
    if ((A & 0xFF) != 0) goto L_01BB75;  /* 01BB5B BNE */
    SETLO(A, rd8(ABS(0x0234)));  /* 01BB5D LDA */
    wr8(ABS(0x0235), (uint8_t)A);  /* 01BB60 STA */
    if ((rd8(ABS(0x023E)) & 0x02) == 0) goto L_01BB70;  /* 01BB63 BBC */
    mflag = 0;  /* 01BB68 CLM */
    S -= 3; F_008053();  /* 01BB69 JSRL */
    goto L_01BB75;  /* 01BB6D JMP */
L_01BB70: TICK();
    mflag = 0;  /* 01BB70 CLM */
    S -= 3; F_008084();  /* 01BB71 JSRL */
L_01BB75: TICK();
    goto L_01BB83;  /* 01BB75 JMP */
L_01BB78: TICK();
    mflag = 0;  /* 01BB78 CLM */
    if ((rd16(ABS(0x023E)) & 0x0002) == 0x0002) goto L_01BB83;  /* 01BB79 BBS */
    S -= 3; F_008084();  /* 01BB7F JSRL */
L_01BB83: TICK();
    mflag = 0;  /* 01BB83 CLM */
    Y = pull16();  /* 01BB84 PLY */
    pul(0x3F);  /* 01BB85 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01BB87 RTI */
}

/* 002024  callers:  */
void isr_TA3(void)
{
    uint32_t t; (void)t;
    goto L_01BB88;  /* 002024 JMPL */
L_01BB88: TICK();
    clp(0x39);  /* 01BB88 CLP */
    psh(0x3F);  /* 01BB8A PSH */
    DT = 0x00;  /* 01BB8C LDT */
    A = D;  /* 01BB8F TDA */
    A = adc16(A, 0x0064);  /* 01BB90 ADC */
    D = A;  /* 01BB93 TAD */
    push16(A);  /* 01BB94 PHA */
    I = 0;  /* 01BB95 CLI */
    mflag = 1;  /* 01BB96 SEM */
    if ((rd8(ABS(TA3MOD)) & 0x01) == 0) goto L_01BBA4;  /* 01BB97 BBC */
    mflag = 0;  /* 01BB9C CLM */
    S -= 3; F_007D0E();  /* 01BB9D JSRL */
    goto L_01BBDF;  /* 01BBA1 JMP */
L_01BBA4: TICK();
    mflag = 1;  /* 01BBA4 SEM */
    if ((rd8(ABS(P5)) & 0x80) == 0) goto L_01BBC5;  /* 01BBA5 BBC [P57 SUSTURMA] */
    SETLO(A, rd8(ABS(0x022C)));  /* 01BBAA LDA */
    wr8(ABS(0x022E), (uint8_t)A);  /* 01BBAD STA */
    t = (rd8(ABS(0x022D)) - 1) & 0xFF; wr8(ABS(0x022D), t);  /* 01BBB0 DEC */
    SETLO(A, rd8(ABS(0x022D)));  /* 01BBB3 LDA */
    if ((A & 0xFF) != 0) goto L_01BBC2;  /* 01BBB6 BNE */
    mflag = 0;  /* 01BBB8 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0020);  /* 01BBB9 SEB */
    S -= 3; F_007D35();  /* 01BBBE JSRL */
L_01BBC2: TICK();
    goto L_01BBDF;  /* 01BBC2 JMP */
L_01BBC5: TICK();
    mflag = 0;  /* 01BBC5 CLM */
    A = rd16(ABS(0x022C));  /* 01BBC6 LDA */
    mflag = 1;  /* 01BBC9 SEM */
    wr8(ABS(0x022D), (uint8_t)A);  /* 01BBCA STA */
    t = (rd8(ABS(0x022E)) - 1) & 0xFF; wr8(ABS(0x022E), t);  /* 01BBCD DEC */
    SETLO(A, rd8(ABS(0x022E)));  /* 01BBD0 LDA */
    if ((A & 0xFF) != 0) goto L_01BBDF;  /* 01BBD3 BNE */
    mflag = 0;  /* 01BBD5 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 01BBD6 CLB */
    S -= 3; F_007D35();  /* 01BBDB JSRL */
L_01BBDF: TICK();
    mflag = 0;  /* 01BBDF CLM */
    Y = pull16();  /* 01BBE0 PLY */
    pul(0x3F);  /* 01BBE1 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01BBE3 RTI */
}

/* 002028  callers:  */
void isr_TB1(void)
{
    uint32_t t; (void)t;
    goto L_01C627;  /* 002028 JMPL */
L_01C627: TICK();
    clp(0x39);  /* 01C627 CLP */
    psh(0x35);  /* 01C629 PSH */
    DT = 0x00;  /* 01C62B LDT */
    A = D;  /* 01C62E TDA */
    A = adc16(A, 0x0064);  /* 01C62F ADC */
    D = A;  /* 01C632 TAD */
    mflag = 1;  /* 01C633 SEM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_01C663;  /* 01C634 BBC */
    I = 0;  /* 01C639 CLI */
    SETLO(A, rd8(ABS(0x0263)));  /* 01C63A LDA */
    t = rd8(ABS(0x0351));  /* 01C63D CMP */
    if ((A & 0xFF) >= t) goto L_01C65B;  /* 01C640 BCS */
    t = (rd8(ABS(0x0263)) + 1) & 0xFF; wr8(ABS(0x0263), t);  /* 01C642 INC */
    mflag = 0;  /* 01C645 CLM */
    A &= 0x00FF;  /* 01C646 AND */
    A = asl16(A);  /* 01C649 ASL */
    X = A;  /* 01C64A TAX */
    A = rd16(ABS(TB1));  /* 01C64B LDA */
    wr16((ABS(0x02AA) + X), A);  /* 01C64E STA */
    C = 0;  /* 01C651 CLC */
    A = adc16(A, rd16(ABS(0x0274)));  /* 01C652 ADC */
    wr16(ABS(0x0274), A);  /* 01C655 STA */
    goto L_01C660;  /* 01C658 JMP */
L_01C65B: TICK();
    mflag = 1;  /* 01C65B SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x40);  /* 01C65C CLB */
L_01C660: TICK();
    goto L_01C6BC;  /* 01C660 JMP */
L_01C663: TICK();
    mflag = 1;  /* 01C663 SEM */
    wr8(ABS(TOSTART), rd8(ABS(TOSTART)) | 0x02);  /* 01C664 SEB */
    mflag = 0;  /* 01C668 CLM */
    if ((rd16(ABS(0x023C)) & 0x0200) == 0x0200) goto L_01C687;  /* 01C669 BBS */
    A = rd16(ABS(0x025F));  /* 01C66F LDA */
    C = 0;  /* 01C672 CLC */
    mflag = 1;  /* 01C673 SEM */
    SETLO(A, adc8(A, rd8(ABS(0x0260))));  /* 01C674 ADC */
    wr8(ABS(0x025F), (uint8_t)A);  /* 01C677 STA */
    mflag = 0;  /* 01C67A CLM */
    A &= 0x00FF;  /* 01C67B AND */
    A = asl16(A);  /* 01C67E ASL */
    X = A;  /* 01C67F TAX */
    A = rd16((0x0028FA + X));  /* 01C680 LDA */
    goto L_01C6B9;  /* 01C684 JMP */
L_01C687: TICK();
    A = rd16(ABS(0x0261));  /* 01C687 LDA */
    C = 0;  /* 01C68A CLC */
    mflag = 1;  /* 01C68B SEM */
    SETLO(A, adc8(A, rd8(ABS(0x0262))));  /* 01C68C ADC */
    wr8(ABS(0x0261), (uint8_t)A);  /* 01C68F STA */
    mflag = 0;  /* 01C692 CLM */
    A &= 0x00FF;  /* 01C693 AND */
    A = asl16(A);  /* 01C696 ASL */
    X = A;  /* 01C697 TAX */
    A = rd16(ABS(0x025F));  /* 01C698 LDA */
    C = 0;  /* 01C69B CLC */
    mflag = 1;  /* 01C69C SEM */
    SETLO(A, adc8(A, rd8(ABS(0x0260))));  /* 01C69D ADC */
    wr8(ABS(0x025F), (uint8_t)A);  /* 01C6A0 STA */
    mflag = 0;  /* 01C6A3 CLM */
    A &= 0x00FF;  /* 01C6A4 AND */
    A = asl16(A);  /* 01C6A7 ASL */
    wr16(DP(0x00), A);  /* 01C6A8 STA */
    A = rd16((0x002673 + X));  /* 01C6AA LDA */
    wr16(DP(0x02), A);  /* 01C6AE STA */
    X = rd16(DP(0x00));  /* 01C6B0 LDX */
    A = rd16(DP(0x02));  /* 01C6B2 LDA */
    C = 0;  /* 01C6B4 CLC */
    A = adc16(A, rd16((0x002473 + X)));  /* 01C6B5 ADC */
L_01C6B9: TICK();
    wr16(ABS(TA1), A);  /* 01C6B9 STA */
L_01C6BC: TICK();
    mflag = 0;  /* 01C6BC CLM */
    pul(0x35);  /* 01C6BD PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01C6BF RTI */
}

/* 00202C  callers:  */
void isr_TA1(void)
{
    uint32_t t; (void)t;
    goto L_01C6C0;  /* 00202C JMPL */
L_01C6C0: TICK();
    clp(0x39);  /* 01C6C0 CLP */
    psh(0x3F);  /* 01C6C2 PSH */
    DT = 0x00;  /* 01C6C4 LDT */
    A = D;  /* 01C6C7 TDA */
    A = adc16(A, 0x0064);  /* 01C6C8 ADC */
    D = A;  /* 01C6CB TAD */
    C = 0;  /* 01C6CC CLC */
    A = adc16(A, 0x003B);  /* 01C6CD ADC */
    push16(A);  /* 01C6D0 PHA */
    mflag = 1;  /* 01C6D1 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x40);  /* 01C6D2 CLB */
    SETLO(A, rd8(ABS(0x0263)));  /* 01C6D6 LDA */
    mflag = 0;  /* 01C6D9 CLM */
    X = rd16(ABS(0x0274));  /* 01C6DA LDX */
    wr16(DP(0x02), X);  /* 01C6DD STX */
    I = 0;  /* 01C6DF CLI */
    wr16(DP(0x04), 0x0000);  /* 01C6E0 LDM */
    wr16(DP(0x06), 0x0000);  /* 01C6E4 LDM */
    mflag = 1;  /* 01C6E8 SEM */
    SETLO(B, 0x00);  /* 01C6E9 LDA */
    wr8(DP(0x38), (uint8_t)A);  /* 01C6EC STA */
    SETLO(A, rd8(ABS(0x0351)));  /* 01C6EE LDA */
    div8(0x02);  /* 01C6F1 DIV */
    t = rd8(DP(0x38));  /* 01C6F4 CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_01C6FD;  /* 01C6F6 BCC */
    if (Z) goto L_01C6FD;  /* 01C6F8 BEQ */
    goto L_01C8A2;  /* 01C6FA JMP */
L_01C6FD: TICK();
    SETLO(A, rd8(DP(0x38)));  /* 01C6FD LDA */
    mflag = 0;  /* 01C6FF CLM */
    A &= 0x00FF;  /* 01C700 AND */
    B = 0x0000;  /* 01C703 LDA */
    wr16(DP(0x3B), A);  /* 01C707 STA */
    A = X;  /* 01C709 TXA */
    div16(rd16(DP(0x3B)));  /* 01C70A DIV */
    mpy16(0x0007);  /* 01C70D MPY */
    B = 0x0000;  /* 01C711 LDA */
    div16(0x000A);  /* 01C715 DIV */
    wr16(DP(0x04), A);  /* 01C719 STA */
    A = asl16(A);  /* 01C71B ASL */
    wr16(DP(0x00), A);  /* 01C71C STA */
    X = rd16(DP(0x04));  /* 01C71E LDX */
    C = (X >= 0x005E);  /* 01C720 CPX */
    if (C) goto L_01C728;  /* 01C723 BCS */
    goto L_01C8A2;  /* 01C725 JMP */
L_01C728: TICK();
    mflag = 1;  /* 01C728 SEM */
    wr8(DP(0x36), 0x00);  /* 01C729 LDM */
    wr8(DP(0x37), 0x00);  /* 01C72C LDM */
L_01C72F: TICK();
    SETLO(A, rd8(DP(0x36)));  /* 01C72F LDA */
    t = rd8(DP(0x38));  /* 01C731 CMP */
    if ((A & 0xFF) >= t) goto L_01C76A;  /* 01C733 BCS */
    mflag = 0;  /* 01C735 CLM */
    A &= 0x00FF;  /* 01C736 AND */
    A = asl16(A);  /* 01C739 ASL */
    X = A;  /* 01C73A TAX */
    A = rd16((ABS(0x02AA) + X));  /* 01C73B LDA */
    wr16(DP(0x39), A);  /* 01C73E STA */
    t = rd16(DP(0x04));  /* 01C740 CMP */
    if (A < t) goto L_01C74A;  /* 01C742 BCC */
    X = rd16(DP(0x00));  /* 01C744 LDX */
    t = rd16(DP(0x39));  /* 01C746 CPX */
    if (X >= t) goto L_01C754;  /* 01C748 BCS */
L_01C74A: TICK();
    A = rd16(DP(0x02));  /* 01C74A LDA */
    C = 1;  /* 01C74C SEC */
    A = sbc16(A, rd16(DP(0x39)));  /* 01C74D SBC */
    wr16(DP(0x02), A);  /* 01C74F STA */
    goto L_01C764;  /* 01C751 JMP */
L_01C754: TICK();
    mflag = 0;  /* 01C754 CLM */
    A = rd16(DP(0x37));  /* 01C755 LDA */
    mflag = 1;  /* 01C757 SEM */
    t = (rd8(DP(0x37)) + 1) & 0xFF; wr8(DP(0x37), t);  /* 01C758 INC */
    mflag = 0;  /* 01C75A CLM */
    A &= 0x00FF;  /* 01C75B AND */
    A = asl16(A);  /* 01C75E ASL */
    X = A;  /* 01C75F TAX */
    A = rd16(DP(0x39));  /* 01C760 LDA */
    wr16(DP(0x08 + X), A);  /* 01C762 STA */
L_01C764: TICK();
    mflag = 1;  /* 01C764 SEM */
    t = (rd8(DP(0x36)) + 1) & 0xFF; wr8(DP(0x36), t);  /* 01C765 INC */
    goto L_01C72F;  /* 01C767 JMP */
L_01C76A: TICK();
    mflag = 0;  /* 01C76A CLM */
    A = rd16(ABS(0x0353));  /* 01C76B LDA */
    A &= 0x00FF;  /* 01C76E AND */
    wr16(DP(0x3B), A);  /* 01C771 STA */
    A = rd16(DP(0x37));  /* 01C773 LDA */
    A &= 0x00FF;  /* 01C775 AND */
    wr16(DP(0x3D), A);  /* 01C778 STA */
    A = rd16(DP(0x38));  /* 01C77A LDA */
    A &= 0x00FF;  /* 01C77C AND */
    C = 1;  /* 01C77F SEC */
    A = sbc16(A, rd16(DP(0x3D)));  /* 01C780 SBC */
    C = 1;  /* 01C782 SEC */
    A = sbc16(A, rd16(DP(0x3B)));  /* 01C783 SBC */
    if (!V) goto L_01C78A;  /* 01C785 BVC */
    A ^= 0x8000;  /* 01C787 EOR */
    N = ((A & 0x8000) != 0);
L_01C78A: TICK();
    if (!N) goto L_01C794;  /* 01C78A BPL */
    A = rd16(ABS(0x0352));  /* 01C78C LDA */
    mflag = 1;  /* 01C78F SEM */
    t = rd8(DP(0x37));  /* 01C790 CMP */
    C = ((A & 0xFF) >= t);
    if (!C) goto L_01C79C;  /* 01C792 BCC */
L_01C794: TICK();
    mflag = 0;  /* 01C794 CLM */
    wr16(DP(0x04), 0x0000);  /* 01C795 LDM */
    goto L_01C8A2;  /* 01C799 JMP */
L_01C79C: TICK();
    mflag = 0;  /* 01C79C CLM */
    A = rd16(DP(0x37));  /* 01C79D LDA */
    A &= 0x00FF;  /* 01C79F AND */
    B = 0x0000;  /* 01C7A2 LDA */
    wr16(DP(0x3B), A);  /* 01C7A6 STA */
    A = rd16(DP(0x02));  /* 01C7A8 LDA */
    div16(rd16(DP(0x3B)));  /* 01C7AA DIV */
    wr16(DP(0x04), A);  /* 01C7AD STA */
    wr16(DP(0x34), 0x0000);  /* 01C7AF LDM */
    wr16(DP(0x32), 0x0000);  /* 01C7B3 LDM */
    wr16(DP(0x30), 0x0000);  /* 01C7B7 LDM */
    mflag = 1;  /* 01C7BB SEM */
    SETLO(B, 0x00);  /* 01C7BC LDA */
    SETLO(A, rd8(DP(0x37)));  /* 01C7BF LDA */
    div8(0x02);  /* 01C7C1 DIV */
    wr8(DP(0x36), (uint8_t)A);  /* 01C7C4 STA */
    wr8(DP(0x38), (uint8_t)A);  /* 01C7C6 STA */
L_01C7C8: TICK();
    SETLO(A, rd8(DP(0x36)));  /* 01C7C8 LDA */
    if ((A & 0xFF) == 0) goto L_01C7E1;  /* 01C7CA BEQ */
    t = (rd8(DP(0x36)) - 1) & 0xFF; wr8(DP(0x36), t);  /* 01C7CC DEC */
    SETLO(A, rd8(DP(0x36)));  /* 01C7CE LDA */
    mflag = 0;  /* 01C7D0 CLM */
    A &= 0x00FF;  /* 01C7D1 AND */
    A = asl16(A);  /* 01C7D4 ASL */
    X = A;  /* 01C7D5 TAX */
    A = rd16(DP(0x34));  /* 01C7D6 LDA */
    C = 0;  /* 01C7D8 CLC */
    A = adc16(A, rd16(DP(0x08 + X)));  /* 01C7D9 ADC */
    wr16(DP(0x34), A);  /* 01C7DB STA */
    mflag = 1;  /* 01C7DD SEM */
    goto L_01C7C8;  /* 01C7DE JMP */
L_01C7E1: TICK();
    mflag = 0;  /* 01C7E1 CLM */
    A = rd16(DP(0x38));  /* 01C7E2 LDA */
    mflag = 1;  /* 01C7E4 SEM */
    wr8(DP(0x36), (uint8_t)A);  /* 01C7E5 STA */
L_01C7E7: TICK();
    SETLO(A, rd8(DP(0x36)));  /* 01C7E7 LDA */
    t = rd8(DP(0x37));  /* 01C7E9 CMP */
    if ((A & 0xFF) >= t) goto L_01C800;  /* 01C7EB BCS */
    t = (rd8(DP(0x36)) + 1) & 0xFF; wr8(DP(0x36), t);  /* 01C7ED INC */
    mflag = 0;  /* 01C7EF CLM */
    A &= 0x00FF;  /* 01C7F0 AND */
    A = asl16(A);  /* 01C7F3 ASL */
    X = A;  /* 01C7F4 TAX */
    A = rd16(DP(0x32));  /* 01C7F5 LDA */
    C = 0;  /* 01C7F7 CLC */
    A = adc16(A, rd16(DP(0x08 + X)));  /* 01C7F8 ADC */
    wr16(DP(0x32), A);  /* 01C7FA STA */
    mflag = 1;  /* 01C7FC SEM */
    goto L_01C7E7;  /* 01C7FD JMP */
L_01C800: TICK();
    mflag = 0;  /* 01C800 CLM */
    A = rd16(DP(0x38));  /* 01C801 LDA */
    A &= 0x00FF;  /* 01C803 AND */
    B = 0x0000;  /* 01C806 LDA */
    wr16(DP(0x3B), A);  /* 01C80A STA */
    A = rd16(DP(0x34));  /* 01C80C LDA */
    div16(rd16(DP(0x3B)));  /* 01C80E DIV */
    wr16(DP(0x3B), A);  /* 01C811 STA */
    A = rd16(DP(0x38));  /* 01C813 LDA */
    A &= 0x00FF;  /* 01C815 AND */
    wr16(DP(0x3D), A);  /* 01C818 STA */
    A = rd16(DP(0x37));  /* 01C81A LDA */
    A &= 0x00FF;  /* 01C81C AND */
    C = 1;  /* 01C81F SEC */
    A = sbc16(A, rd16(DP(0x3D)));  /* 01C820 SBC */
    B = 0x0000;  /* 01C822 LDA */
    wr16(DP(0x3D), A);  /* 01C826 STA */
    A = rd16(DP(0x32));  /* 01C828 LDA */
    div16(rd16(DP(0x3D)));  /* 01C82A DIV */
    C = 1;  /* 01C82D SEC */
    A = sbc16(A, rd16(DP(0x3B)));  /* 01C82E SBC */
    wr16(DP(0x39), A);  /* 01C830 STA */
    if (!N) goto L_01C83C;  /* 01C832 BPL */
    A = 0x0000;  /* 01C834 LDA */
    C = 1;  /* 01C837 SEC */
    A = sbc16(A, rd16(DP(0x39)));  /* 01C838 SBC */
    wr16(DP(0x39), A);  /* 01C83A STA */
L_01C83C: TICK();
    mpy16(0x0011);  /* 01C83C MPY */
    t = rd16(DP(0x04));  /* 01C840 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_01C84D;  /* 01C842 BCC */
    if (Z) goto L_01C84D;  /* 01C844 BEQ */
    wr16(DP(0x04), 0x0000);  /* 01C846 LDM */
    goto L_01C8A2;  /* 01C84A JMP */
L_01C84D: TICK();
    mflag = 1;  /* 01C84D SEM */
    SETLO(A, rd8(DP(0x36)));  /* 01C84E LDA */
    if ((A & 0xFF) == 0) goto L_01C882;  /* 01C850 BEQ */
    SETLO(A, rd8(DP(0x37)));  /* 01C852 LDA */
    mflag = 0;  /* 01C854 CLM */
    A &= 0x00FF;  /* 01C855 AND */
    mflag = 1;  /* 01C858 SEM */
    t = (rd8(DP(0x36)) - 1) & 0xFF; wr8(DP(0x36), t);  /* 01C859 DEC */
    mflag = 0;  /* 01C85B CLM */
    wr16(DP(0x3B), A);  /* 01C85C STA */
    A = rd16(DP(0x36));  /* 01C85E LDA */
    A &= 0x00FF;  /* 01C860 AND */
    A = asl16(A);  /* 01C863 ASL */
    X = A;  /* 01C864 TAX */
    A = 0x0000;  /* 01C865 LDA */
    C = 1;  /* 01C868 SEC */
    A = sbc16(A, rd16(DP(0x08 + X)));  /* 01C869 SBC */
    C = 0;  /* 01C86B CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 01C86C ADC */
    wr16(DP(0x39), A);  /* 01C86E STA */
    B = rd16(DP(0x3B));  /* 01C870 LDA */
    S -= 3; F_00FF8C();  /* 01C873 JSRL */
    mpy16(rd16(DP(0x39)));  /* 01C877 MPY */
    C = 0;  /* 01C87A CLC */
    A = adc16(A, rd16(DP(0x30)));  /* 01C87B ADC */
    wr16(DP(0x30), A);  /* 01C87D STA */
    goto L_01C84D;  /* 01C87F JMP */
L_01C882: TICK();
    mflag = 0;  /* 01C882 CLM */
    A = rd16(ABS(0x0354));  /* 01C883 LDA */
    A &= 0x00FF;  /* 01C886 AND */
    mpy16(rd16(DP(0x04)));  /* 01C889 MPY */
    B = 0x0000;  /* 01C88C LDA */
    div16(0x000A);  /* 01C890 DIV */
    t = rd16(DP(0x30));  /* 01C894 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_01C89A;  /* 01C896 BCC */
    if (!Z) goto L_01C8A2;  /* 01C898 BNE */
L_01C89A: TICK();
    X = rd16(DP(0x04));  /* 01C89A LDX */
    wr16(DP(0x06), X);  /* 01C89C STX */
    wr16(DP(0x04), 0x0000);  /* 01C89E LDM */
L_01C8A2: TICK();
    mflag = 0;  /* 01C8A2 CLM */
    X = rd16(DP(0x04));  /* 01C8A3 LDX */
    wr16(ABS(0x0272), X);  /* 01C8A5 STX */
    mflag = 1;  /* 01C8A8 SEM */
    wr8(ABS(0x0263), 0x00);  /* 01C8A9 LDM */
    mflag = 0;  /* 01C8AD CLM */
    wr16(ABS(0x0274), 0x0000);  /* 01C8AE LDM */
    mflag = 1;  /* 01C8B3 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x40);  /* 01C8B4 SEB */
    wr8(ABS(TB1IC), rd8(ABS(TB1IC)) & ~0x08);  /* 01C8B8 CLB */
    SETLO(A, rd8(ABS(U1C1)));  /* 01C8BC LDA */
    SETLO(A, A & 0xFE);  /* 01C8BF AND */
    if ((A & 0xFF) == 0) goto L_01C8CC;  /* 01C8C1 BEQ */
    SETLO(A, 0x03);  /* 01C8C3 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 01C8C5 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 01C8C6 STA */
    setP(pull16());  /* 01C8C8 PLP */
    goto L_01C8D3;  /* 01C8C9 JMP */
L_01C8CC: TICK();
    mflag = 1;  /* 01C8CC SEM */
    SETLO(A, 0x01);  /* 01C8CD LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 01C8CF PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 01C8D0 STA */
    setP(pull16());  /* 01C8D2 PLP */
L_01C8D3: TICK();
    Y = rd16(DP(0x06));  /* 01C8D3 LDY */
    mflag = 0;  /* 01C8D5 CLM */
    S -= 3; F_01BBE4();  /* 01C8D6 JSRL */
    Y = pull16();  /* 01C8DA PLY */
    pul(0x3F);  /* 01C8DB PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01C8DD RTI */
}

/* 002030  callers:  */
void isr_INT1(void)
{
    uint32_t t; (void)t;
    goto L_01C8DE;  /* 002030 JMPL */
L_01C8DE: TICK();
    clp(0x39);  /* 01C8DE CLP */
    psh(0x3F);  /* 01C8E0 PSH */
    DT = 0x00;  /* 01C8E2 LDT */
    A = D;  /* 01C8E5 TDA */
    A = adc16(A, 0x0064);  /* 01C8E6 ADC */
    D = A;  /* 01C8E9 TAD */
    A += 1;  /* 01C8EA INC */
    push16(A);  /* 01C8EB PHA */
    I = 0;  /* 01C8EC CLI */
    mflag = 1;  /* 01C8ED SEM */
    wr8(ABS(0x01E9), 0x00);  /* 01C8EE LDM */
    SETLO(A, rd8(ABS(0x01EB)));  /* 01C8F2 LDA */
    if ((A & 0xFF) != 0) goto L_01C8FB;  /* 01C8F5 BNE */
    wr8(ABS(0x01EB), 0x01);  /* 01C8F7 LDM */
L_01C8FB: TICK();
    Y = 0x0003;  /* 01C8FB LDY */
    mflag = 0;  /* 01C8FE CLM */
    S -= 3; F_007D7F();  /* 01C8FF JSRL */
    mflag = 1;  /* 01C903 SEM */
    if ((rd8(ABS(P6)) & 0x08) == 0x08) goto L_01C92E;  /* 01C904 BBS [P63 OKUMA_ISTEK0(INT1)] */
    mflag = 0;  /* 01C909 CLM */
    S -= 3; i2c_start_007172();  /* 01C90A JSRL */
    Y = 0x0043;  /* 01C90E LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01C911 JSRL */
    Y = 0x0001;  /* 01C915 LDY */
    mflag = 0;  /* 01C918 CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 01C919 JSRL */
    mflag = 0;  /* 01C91D CLM */
    S -= 3; i2c_stop_00715B();  /* 01C91E JSRL */
    mflag = 1;  /* 01C922 SEM */
    if ((rd8(ABS(P6)) & 0x08) == 0) goto L_01C92E;  /* 01C923 BBC [P63 OKUMA_ISTEK0(INT1)] */
    mflag = 0;  /* 01C928 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0002);  /* 01C929 SEB */
L_01C92E: TICK();
    mflag = 1;  /* 01C92E SEM */
    if ((rd8(ABS(P6)) & 0x08) == 0x08) goto L_01C95B;  /* 01C92F BBS [P63 OKUMA_ISTEK0(INT1)] */
    mflag = 0;  /* 01C934 CLM */
    S -= 3; i2c_start_007172();  /* 01C935 JSRL */
    Y = 0x0047;  /* 01C939 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 01C93C JSRL */
    Y = 0x0001;  /* 01C940 LDY */
    mflag = 0;  /* 01C943 CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 01C944 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 01C948 STA */
    mflag = 0;  /* 01C94A CLM */
    S -= 3; i2c_stop_00715B();  /* 01C94B JSRL */
    mflag = 1;  /* 01C94F SEM */
    if ((rd8(ABS(P6)) & 0x08) == 0) goto L_01C95B;  /* 01C950 BBC [P63 OKUMA_ISTEK0(INT1)] */
    mflag = 0;  /* 01C955 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0004);  /* 01C956 SEB */
L_01C95B: TICK();
    mflag = 0;  /* 01C95B CLM */
    Y = pull16();  /* 01C95C PLY */
    pul(0x3F);  /* 01C95D PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01C95F RTI */
}

/* 002034  callers:  */
void isr_INT2(void)
{
    uint32_t t; (void)t;
    goto L_01C960;  /* 002034 JMPL */
L_01C960: TICK();
    clp(0x39);  /* 01C960 CLP */
    psh(0x3F);  /* 01C962 PSH */
    DT = 0x00;  /* 01C964 LDT */
    A = D;  /* 01C967 TDA */
    A = adc16(A, 0x0064);  /* 01C968 ADC */
    D = A;  /* 01C96B TAD */
    C = 0;  /* 01C96C CLC */
    A = adc16(A, 0x0006);  /* 01C96D ADC */
    push16(A);  /* 01C970 PHA */
    I = 0;  /* 01C971 CLI */
    mflag = 1;  /* 01C972 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 01C973 LDM */
    mflag = 0;  /* 01C977 CLM */
    wr16(DP(0x02), 0x0000);  /* 01C978 LDM */
    wr16(DP(0x04), 0x0041);  /* 01C97C LDM */
    Y = 0x0002;  /* 01C980 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 01C983 JSRL */
    mflag = 0;  /* 01C987 CLM */
    A &= 0x00FF;  /* 01C988 AND */
    wr16(DP(0x00), A);  /* 01C98B STA */
    A = rd16(DP(0x00));  /* 01C98D LDA */
    wr16(DP(0x02), A);  /* 01C98F STA */
    A = D;  /* 01C991 TDA */
    A += 1;  /* 01C992 INC */
    A += 1;  /* 01C993 INC */
    push16(A);  /* 01C994 PHA */
    S -= 3; F_017D9A();  /* 01C995 JSRL */
    Y = pull16();  /* 01C999 PLY */
    A = rd16(DP(0x00));  /* 01C99A LDA */
    wr16(DP(0x02), A);  /* 01C99C STA */
    A = D;  /* 01C99E TDA */
    A += 1;  /* 01C99F INC */
    A += 1;  /* 01C9A0 INC */
    push16(A);  /* 01C9A1 PHA */
    S -= 3; F_014244();  /* 01C9A2 JSRL */
    Y = pull16();  /* 01C9A6 PLY */
    mflag = 1;  /* 01C9A7 SEM */
    if ((rd8(ABS(0x0246)) & 0x02) == 0) goto L_01C9BF;  /* 01C9A8 BBC */
    mflag = 0;  /* 01C9AD CLM */
    A = rd16(DP(0x00));  /* 01C9AE LDA */
    wr16(DP(0x02), A);  /* 01C9B0 STA */
    A = D;  /* 01C9B2 TDA */
    A += 1;  /* 01C9B3 INC */
    A += 1;  /* 01C9B4 INC */
    push16(A);  /* 01C9B5 PHA */
    S -= 3; F_009E50();  /* 01C9B6 JSRL */
    Y = pull16();  /* 01C9BA PLY */
    Y = pull16();  /* 01C9BB PLY */
    pul(0x3F);  /* 01C9BC PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01C9BE RTI */
L_01C9BF: TICK();
    mflag = 0;  /* 01C9BF CLM */
    A = rd16(DP(0x00));  /* 01C9C0 LDA */
    wr16(DP(0x02), A);  /* 01C9C2 STA */
    A = D;  /* 01C9C4 TDA */
    A += 1;  /* 01C9C5 INC */
    A += 1;  /* 01C9C6 INC */
    push16(A);  /* 01C9C7 PHA */
    S -= 3; F_017EA4();  /* 01C9C8 JSRL */
    Y = pull16();  /* 01C9CC PLY */
    mflag = 1;  /* 01C9CD SEM */
    if ((rd8(ABS(P6)) & 0x10) == 0x10) goto L_01CA03;  /* 01C9CE BBS [P64 OKUMA_ISTEK1(INT2)] */
    mflag = 0;  /* 01C9D3 CLM */
    wr16(DP(0x02), 0x0000);  /* 01C9D4 LDM */
    wr16(DP(0x04), 0x0061);  /* 01C9D8 LDM */
    Y = 0x0002;  /* 01C9DC LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 01C9DF JSRL */
    mflag = 0;  /* 01C9E3 CLM */
    A &= 0x00FF;  /* 01C9E4 AND */
    wr16(DP(0x00), A);  /* 01C9E7 STA */
    A = rd16(DP(0x00));  /* 01C9E9 LDA */
    wr16(DP(0x02), A);  /* 01C9EB STA */
    A = D;  /* 01C9ED TDA */
    A += 1;  /* 01C9EE INC */
    A += 1;  /* 01C9EF INC */
    push16(A);  /* 01C9F0 PHA */
    S -= 3; F_01D9E8();  /* 01C9F1 JSRL */
    Y = pull16();  /* 01C9F5 PLY */
    A = rd16(DP(0x00));  /* 01C9F6 LDA */
    wr16(DP(0x02), A);  /* 01C9F8 STA */
    A = D;  /* 01C9FA TDA */
    A += 1;  /* 01C9FB INC */
    A += 1;  /* 01C9FC INC */
    push16(A);  /* 01C9FD PHA */
    S -= 3; F_0181B1();  /* 01C9FE JSRL */
    Y = pull16();  /* 01CA02 PLY */
L_01CA03: TICK();
    mflag = 0;  /* 01CA03 CLM */
    Y = pull16();  /* 01CA04 PLY */
    pul(0x3F);  /* 01CA05 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 01CA07 RTI */
}

/* 002038  callers:  */
void isr_TA4(void)
{
    uint32_t t; (void)t;
    goto L_00F94C;  /* 002038 JMPL */
L_00F94C: TICK();
    clp(0x39);  /* 00F94C CLP */
    psh(0x37);  /* 00F94E PSH */
    DT = 0x00;  /* 00F950 LDT */
    A = D;  /* 00F953 TDA */
    A = adc16(A, 0x0064);  /* 00F954 ADC */
    D = A;  /* 00F957 TAD */
    I = 0;  /* 00F958 CLI */
    if ((rd16(ABS(0x0240)) & 0x1000) == 0) goto L_00F99C;  /* 00F959 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x1000);  /* 00F95F CLB */
    if ((rd16(ABS(0x0240)) & 0x0800) == 0) goto L_00F983;  /* 00F964 BBC */
    mflag = 1;  /* 00F96A SEM */
    if ((rd8(ABS(P6)) & 0x20) == 0) goto L_00F976;  /* 00F96B BBC [P65 TKS_COZME(TB0IN)] */
    t = (rd8(ABS(0x01AF)) - 1) & 0xFF; wr8(ABS(0x01AF), t);  /* 00F970 DEC */
    goto L_00F97A;  /* 00F973 JMP */
L_00F976: TICK();
    mflag = 1;  /* 00F976 SEM */
    t = (rd8(ABS(0x01AF)) + 1) & 0xFF; wr8(ABS(0x01AF), t);  /* 00F977 INC */
L_00F97A: TICK();
    mflag = 0;  /* 00F97A CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0800);  /* 00F97B CLB */
    goto L_00F999;  /* 00F980 JMP */
L_00F983: TICK();
    mflag = 1;  /* 00F983 SEM */
    if ((rd8(ABS(P6)) & 0x20) == 0) goto L_00F98F;  /* 00F984 BBC [P65 TKS_COZME(TB0IN)] */
    t = (rd8(ABS(0x01AF)) + 1) & 0xFF; wr8(ABS(0x01AF), t);  /* 00F989 INC */
    goto L_00F993;  /* 00F98C JMP */
L_00F98F: TICK();
    mflag = 1;  /* 00F98F SEM */
    t = (rd8(ABS(0x01AF)) - 1) & 0xFF; wr8(ABS(0x01AF), t);  /* 00F990 DEC */
L_00F993: TICK();
    mflag = 0;  /* 00F993 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0800);  /* 00F994 SEB */
L_00F999: TICK();
    goto L_00F9CB;  /* 00F999 JMP */
L_00F99C: TICK();
    mflag = 0;  /* 00F99C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x1000);  /* 00F99D SEB */
    if ((rd16(ABS(0x0240)) & 0x0800) == 0) goto L_00F9BB;  /* 00F9A2 BBC */
    mflag = 1;  /* 00F9A8 SEM */
    if ((rd8(ABS(P6)) & 0x20) == 0) goto L_00F9B4;  /* 00F9A9 BBC [P65 TKS_COZME(TB0IN)] */
    t = (rd8(ABS(0x01B0)) - 1) & 0xFF; wr8(ABS(0x01B0), t);  /* 00F9AE DEC */
    goto L_00F9B8;  /* 00F9B1 JMP */
L_00F9B4: TICK();
    mflag = 1;  /* 00F9B4 SEM */
    t = (rd8(ABS(0x01B0)) + 1) & 0xFF; wr8(ABS(0x01B0), t);  /* 00F9B5 INC */
L_00F9B8: TICK();
    goto L_00F9CB;  /* 00F9B8 JMP */
L_00F9BB: TICK();
    mflag = 1;  /* 00F9BB SEM */
    if ((rd8(ABS(P6)) & 0x20) == 0) goto L_00F9C7;  /* 00F9BC BBC [P65 TKS_COZME(TB0IN)] */
    t = (rd8(ABS(0x01B0)) + 1) & 0xFF; wr8(ABS(0x01B0), t);  /* 00F9C1 INC */
    goto L_00F9CB;  /* 00F9C4 JMP */
L_00F9C7: TICK();
    mflag = 1;  /* 00F9C7 SEM */
    t = (rd8(ABS(0x01B0)) - 1) & 0xFF; wr8(ABS(0x01B0), t);  /* 00F9C8 DEC */
L_00F9CB: TICK();
    mflag = 1;  /* 00F9CB SEM */
    t = (rd8(ABS(0x01B1)) - 1) & 0xFF; wr8(ABS(0x01B1), t);  /* 00F9CC DEC */
    SETLO(A, rd8(ABS(0x01B1)));  /* 00F9CF LDA */
    if ((A & 0xFF) == 0) goto L_00F9D7;  /* 00F9D2 BEQ */
    goto L_00FB07;  /* 00F9D4 JMP */
L_00F9D7: TICK();
    wr8(ABS(0x01B1), 0x50);  /* 00F9D7 LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x20);  /* 00F9DB CLB */
    mflag = 0;  /* 00F9DF CLM */
    X = rd16(ABS(0x01B4));  /* 00F9E0 LDX */
    wr16(DP(0x05), X);  /* 00F9E3 STX */
    A = rd16(ABS(0x01B2));  /* 00F9E5 LDA */
    C = 1;  /* 00F9E8 SEC */
    A = sbc16(A, rd16(DP(0x05)));  /* 00F9E9 SBC */
    wr16(DP(0x03), A);  /* 00F9EB STA */
    wr16(ABS(0x01B4), 0x0000);  /* 00F9ED LDM */
    wr16(ABS(0x01B2), 0x0000);  /* 00F9F2 LDM */
    mflag = 1;  /* 00F9F7 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x20);  /* 00F9F8 SEB */
    mflag = 0;  /* 00F9FC CLM */
    if ((rd16(ABS(0x023A)) & 0x1000) == 0) goto L_00FA09;  /* 00F9FD BBC */
    A = 0x0001;  /* 00FA03 LDA */
    goto L_00FA0C;  /* 00FA06 JMP */
L_00FA09: TICK();
    A = 0x0000;  /* 00FA09 LDA */
L_00FA0C: TICK();
    mflag = 1;  /* 00FA0C SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 00FA0D STA */
    wr8(DP(0x01), (uint8_t)A);  /* 00FA0F STA */
    SETLO(A, rd8(ABS(0x01AF)));  /* 00FA11 LDA */
    if ((A & 0x80) == 0) goto L_00FA1F;  /* 00FA14 BPL */
    SETLO(A, 0x00);  /* 00FA16 LDA */
    C = 1;  /* 00FA18 SEC */
    SETLO(A, sbc8(A, rd8(ABS(0x01AF))));  /* 00FA19 SBC */
    wr8(ABS(0x01AF), (uint8_t)A);  /* 00FA1C STA */
L_00FA1F: TICK();
    SETLO(A, rd8(ABS(0x01B0)));  /* 00FA1F LDA */
    if ((A & 0x80) == 0) goto L_00FA2D;  /* 00FA22 BPL */
    SETLO(A, 0x00);  /* 00FA24 LDA */
    C = 1;  /* 00FA26 SEC */
    SETLO(A, sbc8(A, rd8(ABS(0x01B0))));  /* 00FA27 SBC */
    wr8(ABS(0x01B0), (uint8_t)A);  /* 00FA2A STA */
L_00FA2D: TICK();
    C = 1;  /* 00FA2D SEC */
    SETLO(A, sbc8(A, rd8(ABS(0x01AF))));  /* 00FA2E SBC */
    if (!V) goto L_00FA35;  /* 00FA31 BVC */
    SETLO(A, A ^ 0x80);  /* 00FA33 EOR */
    N = ((A & 0x80) != 0);
L_00FA35: TICK();
    if (!N) goto L_00FA47;  /* 00FA35 BPL */
    SETLO(A, rd8(ABS(0x01B0)));  /* 00FA37 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 00FA3A STA */
    SETLO(A, rd8(ABS(0x01AF)));  /* 00FA3C LDA */
    wr8(ABS(0x01B0), (uint8_t)A);  /* 00FA3F STA */
    SETLO(A, rd8(DP(0x00)));  /* 00FA42 LDA */
    wr8(ABS(0x01AF), (uint8_t)A);  /* 00FA44 STA */
L_00FA47: TICK();
    SETLO(A, rd8(ABS(0x01AF)));  /* 00FA47 LDA */
    SETLO(A, lsr8(A));  /* 00FA4A LSR */
    SETLO(A, lsr8(A));  /* 00FA4B LSR */
    C = 0;  /* 00FA4C CLC */
    SETLO(A, adc8(A, rd8(ABS(0x01B0))));  /* 00FA4D ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00FA50 STA */
    wr8(ABS(0x01B0), 0x00);  /* 00FA52 LDM */
    wr8(ABS(0x01AF), 0x00);  /* 00FA56 LDM */
    mflag = 0;  /* 00FA5A CLM */
    /* 00FA5B CPX */
    if (X < 0x0065) goto L_00FA68;  /* 00FA5E BCC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x2000);  /* 00FA60 SEB */
    goto L_00FA9D;  /* 00FA65 JMP */
L_00FA68: TICK();
    mflag = 0;  /* 00FA68 CLM */
    X = rd16(DP(0x03));  /* 00FA69 LDX */
    /* 00FA6B CPX */
    if (X < 0x000B) goto L_00FA9D;  /* 00FA6E BCC */
    A = X;  /* 00FA70 TXA */
    mpy16(0x000A);  /* 00FA71 MPY */
    C = 1;  /* 00FA75 SEC */
    A = sbc16(A, rd16(DP(0x05)));  /* 00FA76 SBC */
    /* 00FA78 CMP */
    if (A < 0x03E9) goto L_00FA82;  /* 00FA7B BCC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x2000);  /* 00FA7D SEB */
L_00FA82: TICK();
    X = rd16(DP(0x05));  /* 00FA82 LDX */
    /* 00FA84 CPX */
    if (X >= 0x005A) goto L_00FA9D;  /* 00FA87 BCS */
    /* 00FA89 CPX */
    if (X < 0x0029) goto L_00FA94;  /* 00FA8C BCC */
    A = 0x0005;  /* 00FA8E LDA */
    goto L_00FA97;  /* 00FA91 JMP */
L_00FA94: TICK();
    A = X;  /* 00FA94 TXA */
    A = lsr16(A);  /* 00FA95 LSR */
    A = lsr16(A);  /* 00FA96 LSR */
L_00FA97: TICK();
    C = 0;  /* 00FA97 CLC */
    mflag = 1;  /* 00FA98 SEM */
    SETLO(A, adc8(A, rd8(DP(0x00))));  /* 00FA99 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00FA9B STA */
L_00FA9D: TICK();
    mflag = 0;  /* 00FA9D CLM */
    A = rd16(DP(0x00));  /* 00FA9E LDA */
    C = 1;  /* 00FAA0 SEC */
    mflag = 1;  /* 00FAA1 SEM */
    SETLO(A, sbc8(A, rd8(ABS(0x01AE))));  /* 00FAA2 SBC */
    if (!V) goto L_00FAA9;  /* 00FAA5 BVC */
    SETLO(A, A ^ 0x80);  /* 00FAA7 EOR */
    N = ((A & 0x80) != 0);
L_00FAA9: TICK();
    if (N) goto L_00FABD;  /* 00FAA9 BMI */
    mflag = 0;  /* 00FAAB CLM */
    if ((rd16(ABS(0x0240)) & 0x2000) == 0x2000) goto L_00FABD;  /* 00FAAC BBS */
    mflag = 1;  /* 00FAB2 SEM */
    wr8(DP(0x01), 0x01);  /* 00FAB3 LDM */
    wr8(ABS(0x01AE), 0x1E);  /* 00FAB6 LDM */
    goto L_00FACB;  /* 00FABA JMP */
L_00FABD: TICK();
    mflag = 1;  /* 00FABD SEM */
    wr8(DP(0x01), 0x00);  /* 00FABE LDM */
    wr8(ABS(0x01AE), 0x28);  /* 00FAC1 LDM */
    mflag = 0;  /* 00FAC5 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x2000);  /* 00FAC6 CLB */
L_00FACB: TICK();
    mflag = 0;  /* 00FACB CLM */
    A = rd16(DP(0x01));  /* 00FACC LDA */
    mflag = 1;  /* 00FACE SEM */
    t = rd8(DP(0x02));  /* 00FACF CMP */
    if ((A & 0xFF) == t) goto L_00FB07;  /* 00FAD1 BEQ */
    SETLO(A, rd8(DP(0x01)));  /* 00FAD3 LDA */
    if ((A & 0xFF) != 0) goto L_00FAE7;  /* 00FAD5 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 00FAD7 LDA */
    if ((A & 0xFF) == 0) goto L_00FAF0;  /* 00FAD9 BEQ */
    SETLO(A, rd8(ABS(0x020A)));  /* 00FADB LDA */
    if ((A & 0xFF) == 0) goto L_00FAF0;  /* 00FADE BEQ */
    mflag = 0;  /* 00FAE0 CLM */
    if ((rd16(ABS(0x0244)) & 0x0800) == 0) goto L_00FAF0;  /* 00FAE1 BBC */
L_00FAE7: TICK();
    mflag = 0;  /* 00FAE7 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x1000);  /* 00FAE8 SEB */
    goto L_00FAF6;  /* 00FAED JMP */
L_00FAF0: TICK();
    mflag = 0;  /* 00FAF0 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x1000);  /* 00FAF1 CLB */
L_00FAF6: TICK();
    mflag = 1;  /* 00FAF6 SEM */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_00FB07;  /* 00FAF7 BBC */
    mflag = 0;  /* 00FAFC CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x2000);  /* 00FAFD SEB */
    mflag = 1;  /* 00FB02 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 00FB03 LDM */
L_00FB07: TICK();
    mflag = 0;  /* 00FB07 CLM */
    pul(0x37);  /* 00FB08 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 00FB0A RTI */
}

/* 00203C  callers:  */
void isr_TB0(void)
{
    uint32_t t; (void)t;
    goto L_00FB0B;  /* 00203C JMPL */
L_00FB0B: TICK();
    clp(0x39);  /* 00FB0B CLP */
    psh(0x25);  /* 00FB0D PSH */
    DT = 0x00;  /* 00FB0F LDT */
    I = 0;  /* 00FB12 CLI */
    mflag = 1;  /* 00FB13 SEM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_00FB3F;  /* 00FB14 BBC */
    mflag = 0;  /* 00FB19 CLM */
    t = (rd16(ABS(0x01B2)) + 1) & 0xFFFF; wr16(ABS(0x01B2), t);  /* 00FB1A INC */
    X = rd16(ABS(TB0));  /* 00FB1D LDX */
    /* 00FB20 CPX */
    if (X >= 0x0348) goto L_00FB2B;  /* 00FB23 BCS */
    t = (rd16(ABS(0x01B4)) + 1) & 0xFFFF; wr16(ABS(0x01B4), t);  /* 00FB25 INC */
    goto L_00FB3C;  /* 00FB28 JMP */
L_00FB2B: TICK();
    mflag = 0;  /* 00FB2B CLM */
    X = rd16(ABS(TB0));  /* 00FB2C LDX */
    /* 00FB2F CPX */
    if (X < 0x1069) goto L_00FB3C;  /* 00FB32 BCC */
    X = rd16(ABS(0x01B4));  /* 00FB34 LDX */
    if (X == 0) goto L_00FB3C;  /* 00FB37 BEQ */
    t = (rd16(ABS(0x01B4)) - 1) & 0xFFFF; wr16(ABS(0x01B4), t);  /* 00FB39 DEC */
L_00FB3C: TICK();
    goto L_00FB75;  /* 00FB3C JMP */
L_00FB3F: TICK();
    mflag = 0;  /* 00FB3F CLM */
    if ((rd16(ABS(0x023A)) & 0x4000) == 0) goto L_00FB5D;  /* 00FB40 BBC */
    A = rd16(ABS(0x01AC));  /* 00FB46 LDA */
    C = 1;  /* 00FB49 SEC */
    mflag = 1;  /* 00FB4A SEM */
    SETLO(A, sbc8(A, rd8(ABS(0x01AD))));  /* 00FB4B SBC */
    wr8(ABS(0x01AC), (uint8_t)A);  /* 00FB4E STA */
    mflag = 0;  /* 00FB51 CLM */
    A &= 0x00FF;  /* 00FB52 AND */
    X = A;  /* 00FB55 TAX */
    A = rd16((0x0021EF + X));  /* 00FB56 LDA */
    goto L_00FB71;  /* 00FB5A JMP */
L_00FB5D: TICK();
    A = rd16(ABS(0x01AC));  /* 00FB5D LDA */
    C = 0;  /* 00FB60 CLC */
    mflag = 1;  /* 00FB61 SEM */
    SETLO(A, adc8(A, rd8(ABS(0x01AD))));  /* 00FB62 ADC */
    wr8(ABS(0x01AC), (uint8_t)A);  /* 00FB65 STA */
    mflag = 0;  /* 00FB68 CLM */
    A &= 0x00FF;  /* 00FB69 AND */
    X = A;  /* 00FB6C TAX */
    A = rd16((0x0021EF + X));  /* 00FB6D LDA */
L_00FB71: TICK();
    mflag = 1;  /* 00FB71 SEM */
    wr8(ABS(TA2_H), (uint8_t)A);  /* 00FB72 STA */
L_00FB75: TICK();
    mflag = 0;  /* 00FB75 CLM */
    pul(0x25);  /* 00FB76 PUL */
    return;  /* RTI: dispatcher restores PS/PC */  /* 00FB78 RTI */
}

/* 00328B  callers: 005520, 00590E, 005E6E */
void F_00328B(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x023C)) & 0x4000) == 0) goto L_0032A0;  /* 00328B BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x4000);  /* 003291 CLB */
    wr16(ABS(0x0214), 0x0000);  /* 003296 LDM */
    mflag = 1;  /* 00329B SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x01);  /* 00329C CLB */
L_0032A0: TICK();
    mflag = 0;  /* 0032A0 CLM */
    S += 3; return;  /* 0032A1 RTL */
}

/* 0032A2  callers: 003517, 003A56, 003AA8 */
void F_0032A2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0032A2 LDA */
    push16(D);  /* 0032A4 PHD */
    D = A;  /* 0032A5 TAD */
    wr16(DP(0x00), Y);  /* 0032A6 STY */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_0032C6;  /* 0032A8 BBS */
    A = Y;  /* 0032AE TYA */
    mflag = 1;  /* 0032AF SEM */
    wr8(ABS(0x0376), (uint8_t)A);  /* 0032B0 STA */
    wr8(ABS(0x0372), 0xFF);  /* 0032B3 LDM */
    wr8(ABS(0x0379), 0xFF);  /* 0032B7 LDM */
    wr8(ABS(0x0375), 0x00);  /* 0032BB LDM */
    wr8(ABS(0x024A), 0xFF);  /* 0032BF LDM */
    goto L_0032CB;  /* 0032C3 JMP */
L_0032C6: TICK();
    mflag = 1;  /* 0032C6 SEM */
    wr8(ABS(0x0376), 0x01);  /* 0032C7 LDM */
L_0032CB: TICK();
    mflag = 0;  /* 0032CB CLM */
    D = pull16();  /* 0032CC PLD */
    S += 3; return;  /* 0032CD RTL */
}

/* 0032CE  callers: 003502, 003F1D, 003F28, 004171, 00417F, 0041EC, 0044D3, 004C2D ... */
void F_0032CE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0032CE LDA */
    push16(D);  /* 0032D0 PHD */
    D = A;  /* 0032D1 TAD */
    wr16(DP(0x01), Y);  /* 0032D2 STY */
    mflag = 1;  /* 0032D4 SEM */
    SETLO(B, 0x00);  /* 0032D5 LDA */
    SETLO(A, rd8(DP(0x00)));  /* 0032D8 LDA */
    div8(0x0A);  /* 0032DA DIV */
    C = 0;  /* 0032DD CLC */
    SETLO(A, adc8(A, 0x30));  /* 0032DE ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 0032E0 STA */
    SETLO(A, Y);  /* 0032E2 TYA */
    mflag = 0;  /* 0032E3 CLM */
    A &= 0x00FF;  /* 0032E4 AND */
    X = A;  /* 0032E7 TAX */
    A = rd16(DP(0x02));  /* 0032E8 LDA */
    mflag = 1;  /* 0032EA SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 0032EB STA */
    SETLO(B, 0x00);  /* 0032EE LDA */
    SETLO(A, rd8(DP(0x00)));  /* 0032F1 LDA */
    div8(0x0A);  /* 0032F3 DIV */
    mflag = 0;  /* 0032F6 CLM */
    push16(B);  /* 0032F7 PHB */
    A = pull16();  /* 0032F9 PLA */
    C = 0;  /* 0032FA CLC */
    mflag = 1;  /* 0032FB SEM */
    SETLO(A, adc8(A, 0x30));  /* 0032FC ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 0032FE STA */
    SETLO(A, Y);  /* 003300 TYA */
    mflag = 0;  /* 003301 CLM */
    A &= 0x00FF;  /* 003302 AND */
    A += 1;  /* 003305 INC */
    X = A;  /* 003306 TAX */
    A = rd16(DP(0x02));  /* 003307 LDA */
    mflag = 1;  /* 003309 SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 00330A STA */
    mflag = 0;  /* 00330D CLM */
    D = pull16();  /* 00330E PLD */
    S += 3; return;  /* 00330F RTL */
}

/* 003310  callers: 003582, 0035D2, 00375C, 0038FB, 003AE6, 003B1F, 003C3E, 003D21 ... */
void F_003310(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003310 LDA */
    push16(D);  /* 003312 PHD */
    D = A;  /* 003313 TAD */
    C = 0;  /* 003314 CLC */
    A = adc16(A, 0x000E);  /* 003315 ADC */
    push16(A);  /* 003318 PHA */
    wr16(DP(0x00), Y);  /* 003319 STY */
    A = rd16(ABS(0x0227));  /* 00331B LDA */
    mflag = 1;  /* 00331E SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 00331F STA */
    SETLO(A, Y);  /* 003321 TYA */
    /* 003322 CMP */
    if ((A & 0xFF) >= 0x20) goto L_003355;  /* 003324 BCS */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_00333F;  /* 003326 BBC */
    mflag = 0;  /* 00332B CLM */
    A &= 0x00FF;  /* 00332C AND */
    A = asl16(A);  /* 00332F ASL */
    A = asl16(A);  /* 003330 ASL */
    A = asl16(A);  /* 003331 ASL */
    C = 0;  /* 003332 CLC */
    A = adc16(A, 0x3026);  /* 003333 ADC */
    wr16(DP(0x03), A);  /* 003336 STA */
    mflag = 1;  /* 003338 SEM */
    wr8(DP(0x05), 0x00);  /* 003339 LDM */
    goto L_003352;  /* 00333C JMP */
L_00333F: TICK();
    mflag = 0;  /* 00333F CLM */
    A = rd16(DP(0x00));  /* 003340 LDA */
    A &= 0x00FF;  /* 003342 AND */
    A = asl16(A);  /* 003345 ASL */
    A = asl16(A);  /* 003346 ASL */
    A = asl16(A);  /* 003347 ASL */
    C = 0;  /* 003348 CLC */
    A = adc16(A, 0x2F26);  /* 003349 ADC */
    wr16(DP(0x03), A);  /* 00334C STA */
    mflag = 1;  /* 00334E SEM */
    wr8(DP(0x05), 0x00);  /* 00334F LDM */
L_003352: TICK();
    goto L_0034A9;  /* 003352 JMP */
L_003355: TICK();
    mflag = 0;  /* 003355 CLM */
    A = rd16(DP(0x00));  /* 003356 LDA */
    mflag = 1;  /* 003358 SEM */
    /* 003359 CMP */
    if ((A & 0xFF) >= 0x2B) goto L_003373;  /* 00335B BCS */
    SETLO(A, rd8(DP(0x00)));  /* 00335D LDA */
    mflag = 0;  /* 00335F CLM */
    A &= 0x00FF;  /* 003360 AND */
    A = asl16(A);  /* 003363 ASL */
    A = asl16(A);  /* 003364 ASL */
    A = asl16(A);  /* 003365 ASL */
    C = 0;  /* 003366 CLC */
    A = adc16(A, 0x3026);  /* 003367 ADC */
    wr16(DP(0x03), A);  /* 00336A STA */
    mflag = 1;  /* 00336C SEM */
    wr8(DP(0x05), 0x00);  /* 00336D LDM */
    goto L_0034A9;  /* 003370 JMP */
L_003373: TICK();
    mflag = 0;  /* 003373 CLM */
    A = rd16(DP(0x00));  /* 003374 LDA */
    mflag = 1;  /* 003376 SEM */
    /* 003377 CMP */
    if ((A & 0xFF) != 0x2B) goto L_0033E6;  /* 003379 BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 00337B LDA */
    /* 00337E CMP */
    if ((A & 0xFF) != 0x11) goto L_0033B4;  /* 003380 BNE */
    mflag = 0;  /* 003382 CLM */
    if ((rd16(ABS(0x0246)) & 0x0002) == 0x0002) goto L_0033B4;  /* 003383 BBS */
    wr16(DP(0x0E), 0x0008);  /* 003389 LDM */
    A = D;  /* 00338D TDA */
    C = 0;  /* 00338E CLC */
    A = adc16(A, 0x0006);  /* 00338F ADC */
    B = 0x0000;  /* 003392 LDA */
    wr16(DP(0x0F), A);  /* 003396 STA */
    wr16(DP(0x11), B);  /* 003398 STA */
    wr16(DP(0x12), 0x0132);  /* 00339B LDM */
    Y = 0x00A0;  /* 00339F LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 0033A2 JSRL */
    A = D;  /* 0033A6 TDA */
    C = 0;  /* 0033A7 CLC */
    A = adc16(A, 0x0006);  /* 0033A8 ADC */
    wr16(DP(0x03), A);  /* 0033AB STA */
    mflag = 1;  /* 0033AD SEM */
    wr8(DP(0x05), 0x00);  /* 0033AE LDM */
    goto L_0033E3;  /* 0033B1 JMP */
L_0033B4: TICK();
    mflag = 1;  /* 0033B4 SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_0033D0;  /* 0033B5 BBC */
    SETLO(A, rd8(DP(0x02)));  /* 0033BA LDA */
    mflag = 0;  /* 0033BC CLM */
    A &= 0x00FF;  /* 0033BD AND */
    A = asl16(A);  /* 0033C0 ASL */
    A = asl16(A);  /* 0033C1 ASL */
    A = asl16(A);  /* 0033C2 ASL */
    C = 0;  /* 0033C3 CLC */
    A = adc16(A, 0x2E86);  /* 0033C4 ADC */
    wr16(DP(0x03), A);  /* 0033C7 STA */
    mflag = 1;  /* 0033C9 SEM */
    wr8(DP(0x05), 0x00);  /* 0033CA LDM */
    goto L_0033E3;  /* 0033CD JMP */
L_0033D0: TICK();
    mflag = 0;  /* 0033D0 CLM */
    A = rd16(DP(0x02));  /* 0033D1 LDA */
    A &= 0x00FF;  /* 0033D3 AND */
    A = asl16(A);  /* 0033D6 ASL */
    A = asl16(A);  /* 0033D7 ASL */
    A = asl16(A);  /* 0033D8 ASL */
    C = 0;  /* 0033D9 CLC */
    A = adc16(A, 0x2DE6);  /* 0033DA ADC */
    wr16(DP(0x03), A);  /* 0033DD STA */
    mflag = 1;  /* 0033DF SEM */
    wr8(DP(0x05), 0x00);  /* 0033E0 LDM */
L_0033E3: TICK();
    goto L_0034A9;  /* 0033E3 JMP */
L_0033E6: TICK();
    mflag = 0;  /* 0033E6 CLM */
    A = rd16(DP(0x00));  /* 0033E7 LDA */
    mflag = 1;  /* 0033E9 SEM */
    /* 0033EA CMP */
    if ((A & 0xFF) == 0x2C) goto L_0033F1;  /* 0033EC BEQ */
    goto L_0034A9;  /* 0033EE JMP */
L_0033F1: TICK();
    mflag = 0;  /* 0033F1 CLM */
    if ((rd16(ABS(0x023C)) & 0x2000) == 0) goto L_003416;  /* 0033F2 BBC */
    mflag = 1;  /* 0033F8 SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_00340A;  /* 0033F9 BBC */
    mflag = 0;  /* 0033FE CLM */
    wr16(DP(0x03), 0x305E);  /* 0033FF LDM */
    mflag = 1;  /* 003403 SEM */
    wr8(DP(0x05), 0x00);  /* 003404 LDM */
    goto L_003413;  /* 003407 JMP */
L_00340A: TICK();
    mflag = 0;  /* 00340A CLM */
    wr16(DP(0x03), 0x2F5E);  /* 00340B LDM */
    mflag = 1;  /* 00340F SEM */
    wr8(DP(0x05), 0x00);  /* 003410 LDM */
L_003413: TICK();
    goto L_0034A9;  /* 003413 JMP */
L_003416: TICK();
    mflag = 0;  /* 003416 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_003441;  /* 003417 BBC */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0x0100) goto L_003441;  /* 00341D BBS */
    mflag = 1;  /* 003423 SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_003435;  /* 003424 BBC */
    mflag = 0;  /* 003429 CLM */
    wr16(DP(0x03), 0x302E);  /* 00342A LDM */
    mflag = 1;  /* 00342E SEM */
    wr8(DP(0x05), 0x00);  /* 00342F LDM */
    goto L_00343E;  /* 003432 JMP */
L_003435: TICK();
    mflag = 0;  /* 003435 CLM */
    wr16(DP(0x03), 0x2F2E);  /* 003436 LDM */
    mflag = 1;  /* 00343A SEM */
    wr8(DP(0x05), 0x00);  /* 00343B LDM */
L_00343E: TICK();
    goto L_0034A9;  /* 00343E JMP */
L_003441: TICK();
    mflag = 0;  /* 003441 CLM */
    A = rd16(ABS(0x0365));  /* 003442 LDA */
    mflag = 1;  /* 003445 SEM */
    SETLO(A, A + 1);  /* 003446 INC */
    if ((A & 0xFF) != 0) goto L_003466;  /* 003447 BNE */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_00345A;  /* 003449 BBC */
    mflag = 0;  /* 00344E CLM */
    wr16(DP(0x03), 0x3026);  /* 00344F LDM */
    mflag = 1;  /* 003453 SEM */
    wr8(DP(0x05), 0x00);  /* 003454 LDM */
    goto L_003463;  /* 003457 JMP */
L_00345A: TICK();
    mflag = 0;  /* 00345A CLM */
    wr16(DP(0x03), 0x2F26);  /* 00345B LDM */
    mflag = 1;  /* 00345F SEM */
    wr8(DP(0x05), 0x00);  /* 003460 LDM */
L_003463: TICK();
    goto L_0034A9;  /* 003463 JMP */
L_003466: TICK();
    mflag = 0;  /* 003466 CLM */
    A = rd16(ABS(0x024A));  /* 003467 LDA */
    mflag = 1;  /* 00346A SEM */
    SETLO(A, A + 1);  /* 00346B INC */
    if ((A & 0xFF) == 0) goto L_00347A;  /* 00346C BEQ */
    mflag = 0;  /* 00346E CLM */
    wr16(DP(0x03), 0x024A);  /* 00346F LDM */
    mflag = 1;  /* 003473 SEM */
    wr8(DP(0x05), 0x00);  /* 003474 LDM */
    goto L_0034A9;  /* 003477 JMP */
L_00347A: TICK();
    mflag = 1;  /* 00347A SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_003496;  /* 00347B BBC */
    SETLO(A, rd8(DP(0x02)));  /* 003480 LDA */
    mflag = 0;  /* 003482 CLM */
    A &= 0x00FF;  /* 003483 AND */
    A = asl16(A);  /* 003486 ASL */
    A = asl16(A);  /* 003487 ASL */
    A = asl16(A);  /* 003488 ASL */
    C = 0;  /* 003489 CLC */
    A = adc16(A, 0x2E86);  /* 00348A ADC */
    wr16(DP(0x03), A);  /* 00348D STA */
    mflag = 1;  /* 00348F SEM */
    wr8(DP(0x05), 0x00);  /* 003490 LDM */
    goto L_0034A9;  /* 003493 JMP */
L_003496: TICK();
    mflag = 0;  /* 003496 CLM */
    A = rd16(DP(0x02));  /* 003497 LDA */
    A &= 0x00FF;  /* 003499 AND */
    A = asl16(A);  /* 00349C ASL */
    A = asl16(A);  /* 00349D ASL */
    A = asl16(A);  /* 00349E ASL */
    C = 0;  /* 00349F CLC */
    A = adc16(A, 0x2DE6);  /* 0034A0 ADC */
    wr16(DP(0x03), A);  /* 0034A3 STA */
    mflag = 1;  /* 0034A5 SEM */
    wr8(DP(0x05), 0x00);  /* 0034A6 LDM */
L_0034A9: TICK();
    wr8(DP(0x01), 0x00);  /* 0034A9 LDM */
L_0034AC: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0034AC LDA */
    C = ((A & 0xFF) >= 0x08);  /* 0034AE CMP */
    if (C) goto L_0034D1;  /* 0034B0 BCS */
    mflag = 0;  /* 0034B2 CLM */
    A &= 0x00FF;  /* 0034B3 AND */
    A = adc16(A, rd16(DP(0x03)));  /* 0034B6 ADC */
    wr16(DP(0x0E), A);  /* 0034B8 STA */
    A = rd16(DP(0x05));  /* 0034BA LDA */
    mflag = 1;  /* 0034BC SEM */
    wr8(DP(0x10), (uint8_t)A);  /* 0034BD STA */
    SETLO(A, rd8(DP(0x01)));  /* 0034BF LDA */
    mflag = 0;  /* 0034C1 CLM */
    A &= 0x00FF;  /* 0034C2 AND */
    X = A;  /* 0034C5 TAX */
    A = rd16(LIND(0x0E));  /* 0034C6 LDA */
    mflag = 1;  /* 0034C8 SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 0034C9 STA */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0034CC INC */
    goto L_0034AC;  /* 0034CE JMP */
L_0034D1: TICK();
    mflag = 0;  /* 0034D1 CLM */
    A = rd16(ABS(0x0365));  /* 0034D2 LDA */
    mflag = 1;  /* 0034D5 SEM */
    SETLO(A, A + 1);  /* 0034D6 INC */
    if ((A & 0xFF) == 0) goto L_003506;  /* 0034D7 BEQ */
    mflag = 0;  /* 0034D9 CLM */
    if ((rd16(ABS(0x023C)) & 0x2000) == 0x2000) goto L_003506;  /* 0034DA BBS */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_0034EC;  /* 0034E0 BBC */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_003506;  /* 0034E6 BBC */
L_0034EC: TICK();
    A = rd16(DP(0x00));  /* 0034EC LDA */
    mflag = 1;  /* 0034EE SEM */
    /* 0034EF CMP */
    if ((A & 0xFF) != 0x2C) goto L_003506;  /* 0034F1 BNE */
    SETLO(A, rd8(ABS(0x024A)));  /* 0034F3 LDA */
    SETLO(A, A + 1);  /* 0034F6 INC */
    if ((A & 0xFF) != 0) goto L_003506;  /* 0034F7 BNE */
    SETLO(A, rd8(ABS(0x0376)));  /* 0034F9 LDA */
    wr8(DP(0x0E), (uint8_t)A);  /* 0034FC STA */
    Y = 0x0006;  /* 0034FE LDY */
    mflag = 0;  /* 003501 CLM */
    S -= 3; F_0032CE();  /* 003502 JSRL */
L_003506: TICK();
    mflag = 0;  /* 003506 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0080);  /* 003507 CLB */
    Y = pull16();  /* 00350C PLY */
    D = pull16();  /* 00350D PLD */
    S += 3; return;  /* 00350E RTL */
}

/* 00350F  callers: 0035EC, 00365A, 003A97, 004D7C */
void channel_record_copy_00350F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00350F LDA */
    push16(D);  /* 003511 PHD */
    D = A;  /* 003512 TAD */
    A += 1;  /* 003513 INC */
    push16(A);  /* 003514 PHA */
    wr16(DP(0x00), Y);  /* 003515 STY */
    S -= 3; F_0032A2();  /* 003517 JSRL */
    wr16(DP(0x01), 0x0370);  /* 00351B LDM */
    wr16(DP(0x03), 0x0363);  /* 00351F LDM */
    A = 0x0001;  /* 003523 LDA */
    Y = 0x000D;  /* 003526 LDY */
    S -= 3; rt_memcpy_01E466();  /* 003529 JSRL */
    wr16(DP(0x01), 0x0370);  /* 00352D LDM */
    wr16(DP(0x03), 0x0356);  /* 003531 LDM */
    A = 0x0001;  /* 003535 LDA */
    Y = 0x000D;  /* 003538 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00353B JSRL */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00357E;  /* 00353F BBC */
    mflag = 1;  /* 003545 SEM */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x08);  /* 003546 CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x04);  /* 00354A CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x02);  /* 00354E CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x01);  /* 003552 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x04);  /* 003556 CLB */
    wr8(ABS(0x020A), 0x00);  /* 00355A LDM */
    wr8(ABS(0x0209), 0x00);  /* 00355E LDM */
    wr8(ABS(0x0207), 0x00);  /* 003562 LDM */
    mflag = 0;  /* 003566 CLM */
    wr16(DP(0x01), 0x037D);  /* 003567 LDM */
    wr16(DP(0x03), 0x0370);  /* 00356B LDM */
    A = 0x0001;  /* 00356F LDA */
    Y = 0x0007;  /* 003572 LDY */
    S -= 3; rt_memcpy_01E466();  /* 003575 JSRL */
    mflag = 1;  /* 003579 SEM */
    wr8(ABS(0x0228), 0x01);  /* 00357A LDM */
L_00357E: TICK();
    Y = 0x002C;  /* 00357E LDY */
    mflag = 0;  /* 003581 CLM */
    S -= 3; F_003310();  /* 003582 JSRL */
    Y = pull16();  /* 003586 PLY */
    D = pull16();  /* 003587 PLD */
    S += 3; return;  /* 003588 RTL */
}

/* 003589  callers: 003690, 004384, 0044A6 */
void F_003589(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003589 LDA */
    push16(D);  /* 00358B PHD */
    D = A;  /* 00358C TAD */
    A += 1;  /* 00358D INC */
    A += 1;  /* 00358E INC */
    A += 1;  /* 00358F INC */
    push16(A);  /* 003590 PHA */
    wr16(DP(0x00), Y);  /* 003591 STY */
    mflag = 1;  /* 003593 SEM */
    SETLO(A, Y);  /* 003594 TYA */
    if ((A & 0xFF) != 0) goto L_00359A;  /* 003595 BNE */
    wr8(DP(0x00), 0x01);  /* 003597 LDM */
L_00359A: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00359A LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 00359C STA */
    mflag = 0;  /* 00359E CLM */
    Y = A;  /* 00359F TAY */
    S -= 3; channel_is_programmed_0087A7();  /* 0035A0 JSRL */
    X = A;  /* 0035A4 TAX */
    if (X != 0) goto L_003609;  /* 0035A5 BNE */
    mflag = 1;  /* 0035A7 SEM */
    wr8(DP(0x02), 0x01);  /* 0035A8 LDM */
L_0035AB: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 0035AB LDA */
    if ((A & 0xFF) == 0) goto L_003609;  /* 0035AD BEQ */
    wr8(ABS(WDT), 0x00);  /* 0035AF LDM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0035B3 INC */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_0035BF;  /* 0035B5 BBC */
    SETLO(A, 0x0A);  /* 0035BA LDA */
    goto L_0035C1;  /* 0035BC JMP */
L_0035BF: TICK();
    SETLO(A, 0x63);  /* 0035BF LDA */
L_0035C1: TICK();
    t = rd8(DP(0x01));  /* 0035C1 CMP */
    if ((A & 0xFF) >= t) goto L_0035C8;  /* 0035C3 BCS */
    wr8(DP(0x01), 0x01);  /* 0035C5 LDM */
L_0035C8: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0035C8 LDA */
    t = rd8(DP(0x00));  /* 0035CA CMP */
    if ((A & 0xFF) != t) goto L_0035F7;  /* 0035CC BNE */
    Y = 0x0000;  /* 0035CE LDY */
    mflag = 0;  /* 0035D1 CLM */
    S -= 3; F_003310();  /* 0035D2 JSRL */
    mflag = 1;  /* 0035D6 SEM */
    if ((rd8(ABS(0x0334)) & 0x10) == 0x10) goto L_0035E4;  /* 0035D7 BBS */
    mflag = 0;  /* 0035DC CLM */
    S -= 3; F_01A60C();  /* 0035DD JSRL */
    goto L_0035F4;  /* 0035E1 JMP */
L_0035E4: TICK();
    mflag = 1;  /* 0035E4 SEM */
    wr8(DP(0x01), 0x01);  /* 0035E5 LDM */
    Y = 0x0001;  /* 0035E8 LDY */
    mflag = 0;  /* 0035EB CLM */
    S -= 3; channel_record_copy_00350F();  /* 0035EC JSRL */
    mflag = 1;  /* 0035F0 SEM */
    wr8(DP(0x02), 0x00);  /* 0035F1 LDM */
L_0035F4: TICK();
    goto L_003605;  /* 0035F4 JMP */
L_0035F7: TICK();
    Y = rd16(DP(0x01));  /* 0035F7 LDY */
    mflag = 0;  /* 0035F9 CLM */
    S -= 3; channel_is_programmed_0087A7();  /* 0035FA JSRL */
    X = A;  /* 0035FE TAX */
    if (X == 0) goto L_003605;  /* 0035FF BEQ */
    mflag = 1;  /* 003601 SEM */
    wr8(DP(0x02), 0x00);  /* 003602 LDM */
L_003605: TICK();
    mflag = 1;  /* 003605 SEM */
    goto L_0035AB;  /* 003606 JMP */
L_003609: TICK();
    mflag = 0;  /* 003609 CLM */
    A = rd16(DP(0x01));  /* 00360A LDA */
    mflag = 1;  /* 00360C SEM */
    Y = pull16();  /* 00360D PLY */
    D = pull16();  /* 00360E PLD */
    S += 3; return;  /* 00360F RTL */
}

/* 003610  callers: 003C36, 004D5A */
void F_003610(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003610 LDA */
    push16(D);  /* 003612 PHD */
    D = A;  /* 003613 TAD */
    A += 1;  /* 003614 INC */
    A += 1;  /* 003615 INC */
    push16(A);  /* 003616 PHA */
    wr16(DP(0x01), Y);  /* 003617 STY */
    S -= 3; channel_is_programmed_0087A7();  /* 003619 JSRL */
    X = A;  /* 00361D TAX */
    if (X == 0) goto L_003635;  /* 00361E BEQ */
    mflag = 1;  /* 003620 SEM */
    SETLO(A, rd8(DP(0x01)));  /* 003621 LDA */
    if ((A & 0xFF) == 0) goto L_003635;  /* 003623 BEQ */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00362F;  /* 003625 BBC */
    SETLO(A, 0x0A);  /* 00362A LDA */
    goto L_003631;  /* 00362C JMP */
L_00362F: TICK();
    SETLO(A, 0x63);  /* 00362F LDA */
L_003631: TICK();
    t = rd8(DP(0x01));  /* 003631 CMP */
    if ((A & 0xFF) >= t) goto L_00369F;  /* 003633 BCS */
L_003635: TICK();
    Y = 0x0002;  /* 003635 LDY */
    mflag = 0;  /* 003638 CLM */
    S -= 3; F_0080AA();  /* 003639 JSRL */
    if ((rd16(ABS(0x023A)) & 0x0040) == 0x0040) goto L_003657;  /* 00363D BBS */
    A = rd16(ABS(0x0231));  /* 003643 LDA */
    mflag = 1;  /* 003646 SEM */
    /* 003647 CMP */
    if ((A & 0xFF) != 0x16) goto L_003666;  /* 003649 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_003666;  /* 00364B BBS */
    SETLO(A, rd8(ABS(0x0206)));  /* 003650 LDA */
    /* 003653 CMP */
    if ((A & 0xFF) == 0x10) goto L_003666;  /* 003655 BEQ */
L_003657: TICK();
    Y = rd16(DP(0x01));  /* 003657 LDY */
    mflag = 0;  /* 003659 CLM */
    S -= 3; channel_record_copy_00350F();  /* 00365A JSRL */
    A = rd16(DP(0x01));  /* 00365E LDA */
    mflag = 1;  /* 003660 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 003661 STA */
    goto L_00369F;  /* 003663 JMP */
L_003666: TICK();
    mflag = 0;  /* 003666 CLM */
    A = rd16(ABS(0x0231));  /* 003667 LDA */
    mflag = 1;  /* 00366A SEM */
    /* 00366B CMP */
    if ((A & 0xFF) != 0x16) goto L_003675;  /* 00366D BNE */
    wr8(DP(0x01), 0x01);  /* 00366F LDM */
    goto L_003689;  /* 003672 JMP */
L_003675: TICK();
    mflag = 0;  /* 003675 CLM */
    A = rd16(ABS(0x0226));  /* 003676 LDA */
    mflag = 1;  /* 003679 SEM */
    /* 00367A CMP */
    if ((A & 0xFF) != 0x73) goto L_003685;  /* 00367C BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 00367E LDA */
    /* 003681 CMP */
    if ((A & 0xFF) != 0x15) goto L_003689;  /* 003683 BNE */
L_003685: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 003685 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 003687 STA */
L_003689: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 003689 LDA */
    if ((A & 0xFF) == 0) goto L_003699;  /* 00368B BEQ */
    Y = rd16(DP(0x01));  /* 00368D LDY */
    mflag = 0;  /* 00368F CLM */
    S -= 3; F_003589();  /* 003690 JSRL */
    wr8(DP(0x01), (uint8_t)A);  /* 003694 STA */
    goto L_00369F;  /* 003696 JMP */
L_003699: TICK();
    mflag = 0;  /* 003699 CLM */
    A = rd16(DP(0x01));  /* 00369A LDA */
    mflag = 1;  /* 00369C SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00369D STA */
L_00369F: TICK();
    Y = rd16(DP(0x01));  /* 00369F LDY */
    mflag = 0;  /* 0036A1 CLM */
    S -= 3; channel_is_programmed_0087A7();  /* 0036A2 JSRL */
    X = A;  /* 0036A6 TAX */
    if (X != 0) goto L_0036AC;  /* 0036A7 BNE */
    goto L_003746;  /* 0036A9 JMP */
L_0036AC: TICK();
    A = rd16(DP(0x01));  /* 0036AC LDA */
    mflag = 1;  /* 0036AE SEM */
    t = rd8(DP(0x00));  /* 0036AF CMP */
    if ((A & 0xFF) != t) goto L_0036B6;  /* 0036B1 BNE */
    goto L_003746;  /* 0036B3 JMP */
L_0036B6: TICK();
    Y = rd16(DP(0x01));  /* 0036B6 LDY */
    mflag = 0;  /* 0036B8 CLM */
    S -= 3; F_008D14();  /* 0036B9 JSRL */
    wr16(DP(0x02), 0x0370);  /* 0036BD LDM */
    wr16(DP(0x04), 0x0363);  /* 0036C1 LDM */
    A = 0x0002;  /* 0036C5 LDA */
    Y = 0x000D;  /* 0036C8 LDY */
    S -= 3; rt_memcpy_01E466();  /* 0036CB JSRL */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 0036CF CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 0036D4 CLB */
    A = rd16(DP(0x01));  /* 0036D9 LDA */
    mflag = 1;  /* 0036DB SEM */
    t = rd8(DP(0x00));  /* 0036DC CMP */
    if ((A & 0xFF) == t) goto L_0036E7;  /* 0036DE BEQ */
    Y = rd16(DP(0x01));  /* 0036E0 LDY */
    mflag = 0;  /* 0036E2 CLM */
    S -= 3; F_0090FE();  /* 0036E3 JSRL */
L_0036E7: TICK();
    mflag = 0;  /* 0036E7 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00373E;  /* 0036E8 BBC */
    A = rd16(ABS(0x0231));  /* 0036EE LDA */
    mflag = 1;  /* 0036F1 SEM */
    /* 0036F2 CMP */
    if ((A & 0xFF) == 0x15) goto L_003704;  /* 0036F4 BEQ */
    SETLO(A, rd8(ABS(0x0226)));  /* 0036F6 LDA */
    /* 0036F9 CMP */
    if ((A & 0xFF) == 0x73) goto L_003704;  /* 0036FB BEQ */
    mflag = 0;  /* 0036FD CLM */
    if ((rd16(ABS(0x023A)) & 0x0040) == 0) goto L_00373E;  /* 0036FE BBC */
L_003704: TICK();
    mflag = 0;  /* 003704 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0100);  /* 003705 SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0200);  /* 00370A SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0400);  /* 00370F CLB */
    if ((rd16(ABS(0x023A)) & 0x0040) == 0) goto L_00372C;  /* 003714 BBC */
    wr16(DP(0x02), 0x0370);  /* 00371A LDM */
    wr16(DP(0x04), 0x0356);  /* 00371E LDM */
    A = 0x0002;  /* 003722 LDA */
    Y = 0x000D;  /* 003725 LDY */
    S -= 3; rt_memcpy_01E466();  /* 003728 JSRL */
L_00372C: TICK();
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0800);  /* 00372C CLB */
    mflag = 1;  /* 003731 SEM */
    wr8(ABS(0x0207), 0xFB);  /* 003732 LDM */
    wr8(ABS(0x0209), 0x5E);  /* 003736 LDM */
    wr8(ABS(0x020A), 0x00);  /* 00373A LDM */
L_00373E: TICK();
    Y = 0x0003;  /* 00373E LDY */
    mflag = 0;  /* 003741 CLM */
    S -= 3; F_00927F();  /* 003742 JSRL */
L_003746: TICK();
    mflag = 0;  /* 003746 CLM */
    Y = pull16();  /* 003747 PLY */
    D = pull16();  /* 003748 PLD */
    S += 3; return;  /* 003749 RTL */
}

/* 00374A  callers:  */
void F_00374A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00374A LDA */
    push16(D);  /* 00374C PHD */
    D = A;  /* 00374D TAD */
    C = 0;  /* 00374E CLC */
    A = adc16(A, 0x0004);  /* 00374F ADC */
    push16(A);  /* 003752 PHA */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_003770;  /* 003753 BBS */
    Y = 0x002C;  /* 003759 LDY */
    S -= 3; F_003310();  /* 00375C JSRL */
    mflag = 1;  /* 003760 SEM */
    wr8(ABS(0x0293), 0x00);  /* 003761 LDM */
    wr8(ABS(0x0230), 0x00);  /* 003765 LDM */
    wr8(ABS(0x0231), 0x00);  /* 003769 LDM */
    goto L_003C4F;  /* 00376D JMP */
L_003770: TICK();
    mflag = 0;  /* 003770 CLM */
    S -= 3; F_014AB3();  /* 003771 JSRL */
    /* 003775 CMP */
    if ((A & 0xFF) == 0x00) goto L_00377C;  /* 003777 BEQ */
    goto L_003C4F;  /* 003779 JMP */
L_00377C: TICK();
    SETLO(A, rd8(ABS(0x0226)));  /* 00377C LDA */
    /* 00377F CMP */
    if ((A & 0xFF) == 0x23) goto L_00378D;  /* 003781 BEQ */
    mflag = 0;  /* 003783 CLM */
    if ((rd16(ABS(0x0240)) & 0x8000) == 0x8000) goto L_00378D;  /* 003784 BBS */
    goto L_003A1E;  /* 00378A JMP */
L_00378D: TICK();
    mflag = 1;  /* 00378D SEM */
    if ((rd8(ABS(0x033F)) & 0x04) == 0x04) goto L_003796;  /* 00378E BBS */
    goto L_003A1E;  /* 003793 JMP */
L_003796: TICK();
    mflag = 0;  /* 003796 CLM */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0x8000) goto L_0037A0;  /* 003797 BBS */
    goto L_003A1E;  /* 00379D JMP */
L_0037A0: TICK();
    A = rd16(ABS(0x0365));  /* 0037A0 LDA */
    mflag = 1;  /* 0037A3 SEM */
    SETLO(A, A + 1);  /* 0037A4 INC */
    if ((A & 0xFF) != 0) goto L_0037AA;  /* 0037A5 BNE */
    goto L_003A13;  /* 0037A7 JMP */
L_0037AA: TICK();
    wr8(ABS(0x0252), 0x43);  /* 0037AA LDM */
    wr8(ABS(0x0253), 0x20);  /* 0037AE LDM */
    SETLO(A, rd8(ABS(0x0343)));  /* 0037B2 LDA */
    SETLO(A, A & 0x0F);  /* 0037B5 AND */
    SETLO(A, A + 1);  /* 0037B7 INC */
    SETLO(A, A + 1);  /* 0037B8 INC */
    wr8(DP(0x00), (uint8_t)A);  /* 0037B9 STA */
L_0037BB: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0037BB LDA */
    /* 0037BD CMP */
    if ((A & 0xFF) >= 0x08) goto L_0037D0;  /* 0037BF BCS */
    mflag = 0;  /* 0037C1 CLM */
    A &= 0x00FF;  /* 0037C2 AND */
    X = A;  /* 0037C5 TAX */
    mflag = 1;  /* 0037C6 SEM */
    wr8((ABS(0x0252) + X), 0x20);  /* 0037C7 LDM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 0037CB INC */
    goto L_0037BB;  /* 0037CD JMP */
L_0037D0: TICK();
    mflag = 1;  /* 0037D0 SEM */
    wr8(DP(0x00), 0x00);  /* 0037D1 LDM */
L_0037D4: TICK();
    SETLO(A, rd8(ABS(0x0343)));  /* 0037D4 LDA */
    SETLO(A, A & 0x0F);  /* 0037D7 AND */
    t = rd8(DP(0x00));  /* 0037D9 CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_0037DF;  /* 0037DB BCC */
    if (!Z) goto L_0037E2;  /* 0037DD BNE */
L_0037DF: TICK();
    goto L_003863;  /* 0037DF JMP */
L_0037E2: TICK();
    SETLO(B, 0x00);  /* 0037E2 LDA */
    SETLO(A, rd8(ABS(0x0344)));  /* 0037E5 LDA */
    div8(0x10);  /* 0037E8 DIV */
    mflag = 0;  /* 0037EB CLM */
    A &= 0x00FF;  /* 0037EC AND */
    wr16(DP(0x04), A);  /* 0037EF STA */
    A = rd16(ABS(0x0343));  /* 0037F1 LDA */
    mflag = 1;  /* 0037F4 SEM */
    SETLO(A, A & 0x0F);  /* 0037F5 AND */
    mflag = 0;  /* 0037F7 CLM */
    A &= 0x00FF;  /* 0037F8 AND */
    C = 1;  /* 0037FB SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 0037FC SBC */
    wr16(DP(0x04), A);  /* 0037FE STA */
    A = rd16(DP(0x00));  /* 003800 LDA */
    A &= 0x00FF;  /* 003802 AND */
    C = 1;  /* 003805 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 003806 SBC */
    if (!V) goto L_00380D;  /* 003808 BVC */
    A ^= 0x8000;  /* 00380A EOR */
    N = ((A & 0x8000) != 0);
L_00380D: TICK();
    if (!N) goto L_003850;  /* 00380D BPL */
    Y = rd16(DP(0x00));  /* 00380F LDY */
    S -= 3; F_0069EF();  /* 003811 JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 003815 STA */
    t = rd8(ABS(0x020E));  /* 003817 CMP */
    if ((A & 0xFF) != t) goto L_003821;  /* 00381A BNE */
    SETLO(A, 0x47);  /* 00381C LDA */
    goto L_00383B;  /* 00381E JMP */
L_003821: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 003821 LDA */
    C = ((A & 0xFF) >= 0x0A);  /* 003823 CMP */
    if (C) goto L_003831;  /* 003825 BCS */
    mflag = 0;  /* 003827 CLM */
    A &= 0x00FF;  /* 003828 AND */
    A = adc16(A, 0x0030);  /* 00382B ADC */
    goto L_00383B;  /* 00382E JMP */
L_003831: TICK();
    mflag = 0;  /* 003831 CLM */
    A = rd16(DP(0x02));  /* 003832 LDA */
    A &= 0x00FF;  /* 003834 AND */
    C = 0;  /* 003837 CLC */
    A = adc16(A, 0x0037);  /* 003838 ADC */
L_00383B: TICK();
    mflag = 1;  /* 00383B SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 00383C STA */
    SETLO(A, rd8(DP(0x00)));  /* 00383E LDA */
    mflag = 0;  /* 003840 CLM */
    A &= 0x00FF;  /* 003841 AND */
    A += 1;  /* 003844 INC */
    A += 1;  /* 003845 INC */
    X = A;  /* 003846 TAX */
    A = rd16(DP(0x04));  /* 003847 LDA */
    mflag = 1;  /* 003849 SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 00384A STA */
    goto L_00385E;  /* 00384D JMP */
L_003850: TICK();
    mflag = 0;  /* 003850 CLM */
    A = rd16(DP(0x00));  /* 003851 LDA */
    A &= 0x00FF;  /* 003853 AND */
    A += 1;  /* 003856 INC */
    A += 1;  /* 003857 INC */
    X = A;  /* 003858 TAX */
    mflag = 1;  /* 003859 SEM */
    wr8((ABS(0x0252) + X), 0x5F);  /* 00385A LDM */
L_00385E: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00385E INC */
    goto L_0037D4;  /* 003860 JMP */
L_003863: TICK();
    mflag = 0;  /* 003863 CLM */
    A = rd16(ABS(0x0226));  /* 003864 LDA */
    mflag = 1;  /* 003867 SEM */
    /* 003868 CMP */
    if ((A & 0xFF) != 0x64) goto L_0038AB;  /* 00386A BNE */
    SETLO(B, 0x00);  /* 00386C LDA */
    SETLO(A, rd8(ABS(0x0344)));  /* 00386F LDA */
    div8(0x10);  /* 003872 DIV */
    t = rd8(ABS(0x0293));  /* 003875 CMP */
    if ((A & 0xFF) >= t) goto L_003880;  /* 003878 BCS */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 00387A DEC */
    goto L_0038A0;  /* 00387D JMP */
L_003880: TICK();
    mflag = 0;  /* 003880 CLM */
    A = rd16(ABS(0x0293));  /* 003881 LDA */
    A &= 0x00FF;  /* 003884 AND */
    A -= 1;  /* 003887 DEC */
    X = A;  /* 003888 TAX */
    A = rd16((ABS(0x0283) + X));  /* 003889 LDA */
    mflag = 1;  /* 00388C SEM */
    t = rd8(ABS(0x020E));  /* 00388D CMP */
    if ((A & 0xFF) != t) goto L_0038A0;  /* 003890 BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 003892 LDA */
    mflag = 0;  /* 003895 CLM */
    A &= 0x00FF;  /* 003896 AND */
    A -= 1;  /* 003899 DEC */
    X = A;  /* 00389A TAX */
    mflag = 1;  /* 00389B SEM */
    wr8((ABS(0x0283) + X), 0x47);  /* 00389C LDM */
L_0038A0: TICK();
    wr8(ABS(0x0231), 0x15);  /* 0038A0 LDM */
    wr8(ABS(0x0230), 0x3F);  /* 0038A4 LDM */
    goto L_003904;  /* 0038A8 JMP */
L_0038AB: TICK();
    mflag = 0;  /* 0038AB CLM */
    A = rd16(ABS(0x0226));  /* 0038AC LDA */
    mflag = 1;  /* 0038AF SEM */
    /* 0038B0 CMP */
    if ((A & 0xFF) != 0x2A) goto L_0038CF;  /* 0038B2 BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 0038B4 LDA */
    mflag = 0;  /* 0038B7 CLM */
    A &= 0x00FF;  /* 0038B8 AND */
    X = A;  /* 0038BB TAX */
    mflag = 1;  /* 0038BC SEM */
    wr8((ABS(0x0283) + X), 0x47);  /* 0038BD LDM */
    t = (rd8(ABS(0x0293)) + 1) & 0xFF; wr8(ABS(0x0293), t);  /* 0038C1 INC */
    wr8(ABS(0x0231), 0x15);  /* 0038C4 LDM */
    wr8(ABS(0x0230), 0x3F);  /* 0038C8 LDM */
    goto L_003904;  /* 0038CC JMP */
L_0038CF: TICK();
    mflag = 0;  /* 0038CF CLM */
    A = rd16(ABS(0x0226));  /* 0038D0 LDA */
    mflag = 1;  /* 0038D3 SEM */
    /* 0038D4 CMP */
    if ((A & 0xFF) != 0x23) goto L_0038EA;  /* 0038D6 BNE */
    mflag = 0;  /* 0038D8 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x8000);  /* 0038D9 SEB */
    mflag = 1;  /* 0038DE SEM */
    wr8(ABS(0x0293), 0x00);  /* 0038DF LDM */
    wr8(ABS(0x0231), 0x15);  /* 0038E3 LDM */
    goto L_003904;  /* 0038E7 JMP */
L_0038EA: TICK();
    mflag = 1;  /* 0038EA SEM */
    wr8(ABS(0x0293), 0x00);  /* 0038EB LDM */
    wr8(ABS(0x0230), 0x00);  /* 0038EF LDM */
    wr8(ABS(0x0231), 0x00);  /* 0038F3 LDM */
    Y = 0x002C;  /* 0038F7 LDY */
    mflag = 0;  /* 0038FA CLM */
    S -= 3; F_003310();  /* 0038FB JSRL */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x8000);  /* 0038FF CLB */
L_003904: TICK();
    mflag = 0;  /* 003904 CLM */
    if ((rd16(ABS(0x0240)) & 0x8000) == 0) goto L_003951;  /* 003905 BBC */
    mflag = 1;  /* 00390B SEM */
    wr8(DP(0x00), 0x00);  /* 00390C LDM */
L_00390F: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00390F LDA */
    t = rd8(ABS(0x0293));  /* 003911 CMP */
    if ((A & 0xFF) >= t) goto L_003951;  /* 003914 BCS */
    mflag = 0;  /* 003916 CLM */
    A &= 0x00FF;  /* 003917 AND */
    X = A;  /* 00391A TAX */
    mflag = 1;  /* 00391B SEM */
    SETLO(B, 0x00);  /* 00391C LDA */
    SETLO(A, rd8(ABS(0x0344)));  /* 00391F LDA */
    div8(0x10);  /* 003922 DIV */
    mflag = 0;  /* 003925 CLM */
    A &= 0x00FF;  /* 003926 AND */
    wr16(DP(0x04), A);  /* 003929 STA */
    A = rd16(ABS(0x0343));  /* 00392B LDA */
    mflag = 1;  /* 00392E SEM */
    SETLO(A, A & 0x0F);  /* 00392F AND */
    mflag = 0;  /* 003931 CLM */
    A &= 0x00FF;  /* 003932 AND */
    C = 1;  /* 003935 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 003936 SBC */
    wr16(DP(0x04), A);  /* 003938 STA */
    A = rd16(DP(0x00));  /* 00393A LDA */
    A &= 0x00FF;  /* 00393C AND */
    C = 0;  /* 00393F CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 003940 ADC */
    A += 1;  /* 003942 INC */
    A += 1;  /* 003943 INC */
    Y = A;  /* 003944 TAY */
    A = rd16((ABS(0x0283) + X));  /* 003945 LDA */
    mflag = 1;  /* 003948 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 003949 STA */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00394C INC */
    goto L_00390F;  /* 00394E JMP */
L_003951: TICK();
    mflag = 1;  /* 003951 SEM */
    SETLO(B, 0x00);  /* 003952 LDA */
    SETLO(A, rd8(ABS(0x0344)));  /* 003955 LDA */
    div8(0x10);  /* 003958 DIV */
    t = rd8(ABS(0x0293));  /* 00395B CMP */
    if ((A & 0xFF) == t) goto L_003963;  /* 00395E BEQ */
    goto L_003A10;  /* 003960 JMP */
L_003963: TICK();
    mflag = 0;  /* 003963 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0001);  /* 003964 SEB */
L_003969: TICK();
    mflag = 1;  /* 003969 SEM */
    wr8(ABS(WDT), 0x00);  /* 00396A LDM */
    mflag = 0;  /* 00396E CLM */
    X = rd16(ABS(0x0214));  /* 00396F LDX */
    if (X != 0) goto L_003969;  /* 003972 BNE */
    mflag = 1;  /* 003974 SEM */
    wr8(ABS(0x0231), 0x14);  /* 003975 LDM */
    wr8(ABS(0x0230), 0x3F);  /* 003979 LDM */
    if ((rd8(ABS(0x034E)) & 0x08) == 0) goto L_00399A;  /* 00397D BBC */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_00399A;  /* 003982 BBC */
    mflag = 0;  /* 003987 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0001);  /* 003988 CLB */
    mflag = 1;  /* 00398D SEM */
    wr8(ABS(0x0230), 0x00);  /* 00398E LDM */
    Y = 0x0003;  /* 003992 LDY */
    mflag = 0;  /* 003995 CLM */
    S -= 3; F_0080AA();  /* 003996 JSRL */
L_00399A: TICK();
    mflag = 1;  /* 00399A SEM */
    wr8(ABS(0x0293), 0x00);  /* 00399B LDM */
    mflag = 0;  /* 00399F CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x8000);  /* 0039A0 CLB */
    mflag = 1;  /* 0039A5 SEM */
    wr8(DP(0x00), 0x00);  /* 0039A6 LDM */
L_0039A9: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0039A9 LDA */
    /* 0039AB CMP */
    if ((A & 0xFF) >= 0x06) goto L_003A10;  /* 0039AD BCS */
    SETLO(A, rd8(ABS(0x0343)));  /* 0039AF LDA */
    SETLO(A, A & 0x0F);  /* 0039B2 AND */
    t = rd8(DP(0x00));  /* 0039B4 CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_0039FF;  /* 0039B6 BCC */
    if (Z) goto L_0039FF;  /* 0039B8 BEQ */
    SETLO(A, rd8(DP(0x00)));  /* 0039BA LDA */
    mflag = 0;  /* 0039BC CLM */
    A &= 0x00FF;  /* 0039BD AND */
    A += 1;  /* 0039C0 INC */
    A += 1;  /* 0039C1 INC */
    X = A;  /* 0039C2 TAX */
    A = rd16((ABS(0x0252) + X));  /* 0039C3 LDA */
    mflag = 1;  /* 0039C6 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 0039C7 STA */
    /* 0039C9 CMP */
    if ((A & 0xFF) != 0x47) goto L_0039D5;  /* 0039CB BNE */
    SETLO(A, rd8(ABS(0x020E)));  /* 0039CD LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 0039D0 STA */
    goto L_0039EF;  /* 0039D2 JMP */
L_0039D5: TICK();
    mflag = 0;  /* 0039D5 CLM */
    A = rd16(DP(0x02));  /* 0039D6 LDA */
    mflag = 1;  /* 0039D8 SEM */
    C = ((A & 0xFF) >= 0x41);  /* 0039D9 CMP */
    if (!C) goto L_0039E6;  /* 0039DB BCC */
    SETLO(A, rd8(DP(0x02)));  /* 0039DD LDA */
    SETLO(A, adc8(A, 0xC8));  /* 0039DF ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 0039E1 STA */
    goto L_0039EF;  /* 0039E3 JMP */
L_0039E6: TICK();
    mflag = 0;  /* 0039E6 CLM */
    A = rd16(DP(0x02));  /* 0039E7 LDA */
    C = 0;  /* 0039E9 CLC */
    mflag = 1;  /* 0039EA SEM */
    SETLO(A, adc8(A, 0xD0));  /* 0039EB ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 0039ED STA */
L_0039EF: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0039EF LDA */
    mflag = 0;  /* 0039F1 CLM */
    A &= 0x00FF;  /* 0039F2 AND */
    X = A;  /* 0039F5 TAX */
    A = rd16(DP(0x02));  /* 0039F6 LDA */
    mflag = 1;  /* 0039F8 SEM */
    wr8((ABS(0x02A4) + X), (uint8_t)A);  /* 0039F9 STA */
    goto L_003A0B;  /* 0039FC JMP */
L_0039FF: TICK();
    mflag = 0;  /* 0039FF CLM */
    A = rd16(DP(0x00));  /* 003A00 LDA */
    A &= 0x00FF;  /* 003A02 AND */
    X = A;  /* 003A05 TAX */
    mflag = 1;  /* 003A06 SEM */
    wr8((ABS(0x02A4) + X), 0x0F);  /* 003A07 LDM */
L_003A0B: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 003A0B INC */
    goto L_0039A9;  /* 003A0D JMP */
L_003A10: TICK();
    goto L_003A1B;  /* 003A10 JMP */
L_003A13: TICK();
    Y = 0x0002;  /* 003A13 LDY */
    mflag = 0;  /* 003A16 CLM */
    S -= 3; F_0080AA();  /* 003A17 JSRL */
L_003A1B: TICK();
    goto L_003C4F;  /* 003A1B JMP */
L_003A1E: TICK();
    mflag = 1;  /* 003A1E SEM */
    wr8(DP(0x01), 0x00);  /* 003A1F LDM */
    wr8(DP(0x02), 0x00);  /* 003A22 LDM */
    SETLO(A, rd8(ABS(0x0369)));  /* 003A25 LDA */
    wr8(DP(0x03), (uint8_t)A);  /* 003A28 STA */
    SETLO(A, rd8(ABS(0x0231)));  /* 003A2A LDA */
    /* 003A2D CMP */
    if ((A & 0xFF) != 0x16) goto L_003A6E;  /* 003A2F BNE */
    wr8(DP(0x01), 0x01);  /* 003A31 LDM */
    SETLO(A, rd8(DP(0x03)));  /* 003A34 LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 003A36 STA */
    wr8(DP(0x03), 0x00);  /* 003A38 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_003A6B;  /* 003A3B BBS */
    SETLO(A, rd8(ABS(0x0206)));  /* 003A40 LDA */
    /* 003A43 CMP */
    if ((A & 0xFF) == 0x10) goto L_003A4C;  /* 003A45 BEQ */
    wr8(DP(0x02), (uint8_t)A);  /* 003A47 STA */
    goto L_003A6B;  /* 003A49 JMP */
L_003A4C: TICK();
    mflag = 1;  /* 003A4C SEM */
    if ((rd8(ABS(0x033F)) & 0x08) == 0x08) goto L_003A65;  /* 003A4D BBS */
    Y = 0x0000;  /* 003A52 LDY */
    mflag = 0;  /* 003A55 CLM */
    S -= 3; F_0032A2();  /* 003A56 JSRL */
    S -= 3; F_004A9A();  /* 003A5A JSRL */
    mflag = 1;  /* 003A5E SEM */
    wr8(DP(0x01), 0x00);  /* 003A5F LDM */
    goto L_003A6B;  /* 003A62 JMP */
L_003A65: TICK();
    mflag = 0;  /* 003A65 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0400);  /* 003A66 SEB */
L_003A6B: TICK();
    goto L_003C0E;  /* 003A6B JMP */
L_003A6E: TICK();
    mflag = 0;  /* 003A6E CLM */
    if ((rd16(ABS(0x023A)) & 0x0040) == 0) goto L_003AD6;  /* 003A6F BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x2000);  /* 003A75 CLB */
    A = rd16(ABS(0x0206));  /* 003A7A LDA */
    mflag = 1;  /* 003A7D SEM */
    /* 003A7E CMP */
    if ((A & 0xFF) != 0x10) goto L_003AB3;  /* 003A80 BNE */
    if ((rd8(ABS(0x033F)) & 0x08) == 0) goto L_003A9E;  /* 003A82 BBC */
    Y = 0x0002;  /* 003A87 LDY */
    mflag = 0;  /* 003A8A CLM */
    S -= 3; F_0080AA();  /* 003A8B JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0400);  /* 003A8F SEB */
    Y = 0x0000;  /* 003A94 LDY */
    S -= 3; channel_record_copy_00350F();  /* 003A97 JSRL */
    goto L_003AB0;  /* 003A9B JMP */
L_003A9E: TICK();
    mflag = 0;  /* 003A9E CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_003AB0;  /* 003A9F BBS */
    Y = 0x0000;  /* 003AA5 LDY */
    S -= 3; F_0032A2();  /* 003AA8 JSRL */
    S -= 3; F_004A9A();  /* 003AAC JSRL */
L_003AB0: TICK();
    goto L_003AD3;  /* 003AB0 JMP */
L_003AB3: TICK();
    mflag = 1;  /* 003AB3 SEM */
    if ((rd8(ABS(0x033F)) & 0x08) == 0x08) goto L_003AC4;  /* 003AB4 BBS */
    mflag = 0;  /* 003AB9 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_003AC4;  /* 003ABA BBC */
    S -= 3; F_004A9A();  /* 003AC0 JSRL */
L_003AC4: TICK();
    mflag = 0;  /* 003AC4 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0400);  /* 003AC5 CLB */
    A = rd16(ABS(0x0206));  /* 003ACA LDA */
    mflag = 1;  /* 003ACD SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 003ACE STA */
    wr8(DP(0x01), 0x01);  /* 003AD0 LDM */
L_003AD3: TICK();
    goto L_003C0E;  /* 003AD3 JMP */
L_003AD6: TICK();
    mflag = 0;  /* 003AD6 CLM */
    if ((rd16(ABS(0x023A)) & 0x0400) == 0x0400) goto L_003B08;  /* 003AD7 BBS */
    mflag = 1;  /* 003ADD SEM */
    wr8(ABS(0x0293), 0x00);  /* 003ADE LDM */
    Y = 0x002C;  /* 003AE2 LDY */
    mflag = 0;  /* 003AE5 CLM */
    S -= 3; F_003310();  /* 003AE6 JSRL */
    A = rd16(ABS(0x0226));  /* 003AEA LDA */
    mflag = 1;  /* 003AED SEM */
    /* 003AEE CMP */
    if ((A & 0xFF) == 0x64) goto L_003AFD;  /* 003AF0 BEQ */
    SETLO(A, rd8(ABS(0x0226)));  /* 003AF2 LDA */
    /* 003AF5 CMP */
    if ((A & 0xFF) == 0x2A) goto L_003AFD;  /* 003AF7 BEQ */
    /* 003AF9 CMP */
    if ((A & 0xFF) != 0x23) goto L_003B05;  /* 003AFB BNE */
L_003AFD: TICK();
    Y = 0x000D;  /* 003AFD LDY */
    mflag = 0;  /* 003B00 CLM */
    S -= 3; F_0080AA();  /* 003B01 JSRL */
L_003B05: TICK();
    goto L_003C0E;  /* 003B05 JMP */
L_003B08: TICK();
    mflag = 0;  /* 003B08 CLM */
    A = rd16(ABS(0x0226));  /* 003B09 LDA */
    mflag = 1;  /* 003B0C SEM */
    /* 003B0D CMP */
    if ((A & 0xFF) == 0x64) goto L_003B1B;  /* 003B0F BEQ */
    SETLO(A, rd8(ABS(0x0231)));  /* 003B11 LDA */
    /* 003B14 CMP */
    if ((A & 0xFF) == 0x15) goto L_003B1B;  /* 003B16 BEQ */
    goto L_003BB5;  /* 003B18 JMP */
L_003B1B: TICK();
    Y = 0x002B;  /* 003B1B LDY */
    mflag = 0;  /* 003B1E CLM */
    S -= 3; F_003310();  /* 003B1F JSRL */
    A = rd16(ABS(0x0231));  /* 003B23 LDA */
    mflag = 1;  /* 003B26 SEM */
    /* 003B27 CMP */
    if ((A & 0xFF) == 0x15) goto L_003B40;  /* 003B29 BEQ */
    wr8(ABS(0x0231), 0x15);  /* 003B2B LDM */
    wr8(ABS(0x0230), 0x3F);  /* 003B2F LDM */
    wr8(ABS(0x0258), 0x20);  /* 003B33 LDM */
    SETLO(A, rd8(ABS(0x0283)));  /* 003B37 LDA */
    wr8(ABS(0x0259), (uint8_t)A);  /* 003B3A STA */
    goto L_003BB2;  /* 003B3D JMP */
L_003B40: TICK();
    mflag = 0;  /* 003B40 CLM */
    A = rd16(ABS(0x0226));  /* 003B41 LDA */
    mflag = 1;  /* 003B44 SEM */
    /* 003B45 CMP */
    if ((A & 0xFF) != 0x64) goto L_003B6B;  /* 003B47 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 003B49 LDA */
    /* 003B4C CMP */
    if ((A & 0xFF) != 0x30) goto L_003B6B;  /* 003B4E BNE */
    SETLO(A, rd8(ABS(0x0284)));  /* 003B50 LDA */
    /* 003B53 CMP */
    if ((A & 0xFF) != 0x30) goto L_003B6B;  /* 003B55 BNE */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 003B57 DEC */
    wr8(ABS(0x0230), 0x3F);  /* 003B5A LDM */
    wr8(ABS(0x0258), 0x20);  /* 003B5E LDM */
    SETLO(A, rd8(ABS(0x0283)));  /* 003B62 LDA */
    wr8(ABS(0x0259), (uint8_t)A);  /* 003B65 STA */
    goto L_003BB2;  /* 003B68 JMP */
L_003B6B: TICK();
    mflag = 1;  /* 003B6B SEM */
    wr8(DP(0x01), 0x01);  /* 003B6C LDM */
    SETLO(A, rd8(ABS(0x0293)));  /* 003B6F LDA */
    /* 003B72 CMP */
    if ((A & 0xFF) != 0x02) goto L_003B88;  /* 003B74 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 003B76 LDA */
    mpy8(0x0A);  /* 003B79 MPY */
    C = 0;  /* 003B7C CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0284))));  /* 003B7D ADC */
    C = 0;  /* 003B80 CLC */
    SETLO(A, adc8(A, 0xF0));  /* 003B81 ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 003B83 STA */
    goto L_003BA2;  /* 003B85 JMP */
L_003B88: TICK();
    mflag = 0;  /* 003B88 CLM */
    A = rd16(ABS(0x0293));  /* 003B89 LDA */
    mflag = 1;  /* 003B8C SEM */
    /* 003B8D CMP */
    if ((A & 0xFF) != 0x01) goto L_003B9C;  /* 003B8F BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 003B91 LDA */
    C = 0;  /* 003B94 CLC */
    SETLO(A, adc8(A, 0xD0));  /* 003B95 ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 003B97 STA */
    goto L_003BA2;  /* 003B99 JMP */
L_003B9C: TICK();
    mflag = 0;  /* 003B9C CLM */
    A = rd16(DP(0x03));  /* 003B9D LDA */
    mflag = 1;  /* 003B9F SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 003BA0 STA */
L_003BA2: TICK();
    /* 003BA2 CMP */
    if ((A & 0xFF) != 0x00) goto L_003BB2;  /* 003BA4 BNE */
    SETLO(A, rd8(DP(0x03)));  /* 003BA6 LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 003BA8 STA */
    Y = 0x0002;  /* 003BAA LDY */
    mflag = 0;  /* 003BAD CLM */
    S -= 3; F_0080AA();  /* 003BAE JSRL */
L_003BB2: TICK();
    goto L_003C0E;  /* 003BB2 JMP */
L_003BB5: TICK();
    mflag = 0;  /* 003BB5 CLM */
    A = rd16(ABS(0x0226));  /* 003BB6 LDA */
    mflag = 1;  /* 003BB9 SEM */
    /* 003BBA CMP */
    if ((A & 0xFF) == 0x6D) goto L_003BC5;  /* 003BBC BEQ */
    SETLO(A, rd8(ABS(0x0231)));  /* 003BBE LDA */
    /* 003BC1 CMP */
    if ((A & 0xFF) != 0x14) goto L_003BCF;  /* 003BC3 BNE */
L_003BC5: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 003BC5 LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 003BC7 STA */
    wr8(DP(0x01), 0x01);  /* 003BC9 LDM */
    goto L_003C0E;  /* 003BCC JMP */
L_003BCF: TICK();
    mflag = 0;  /* 003BCF CLM */
    A = rd16(ABS(0x0226));  /* 003BD0 LDA */
    mflag = 1;  /* 003BD3 SEM */
    /* 003BD4 CMP */
    if ((A & 0xFF) != 0x73) goto L_003BF6;  /* 003BD6 BNE */
    SETLO(A, rd8(DP(0x03)));  /* 003BD8 LDA */
    SETLO(A, A + 1);  /* 003BDA INC */
    wr8(DP(0x02), (uint8_t)A);  /* 003BDB STA */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_003BE7;  /* 003BDD BBC */
    SETLO(A, 0x0A);  /* 003BE2 LDA */
    goto L_003BE9;  /* 003BE4 JMP */
L_003BE7: TICK();
    SETLO(A, 0x63);  /* 003BE7 LDA */
L_003BE9: TICK();
    t = rd8(DP(0x02));  /* 003BE9 CMP */
    if ((A & 0xFF) >= t) goto L_003BF0;  /* 003BEB BCS */
    wr8(DP(0x02), 0x01);  /* 003BED LDM */
L_003BF0: TICK();
    wr8(DP(0x01), 0x01);  /* 003BF0 LDM */
    goto L_003C0E;  /* 003BF3 JMP */
L_003BF6: TICK();
    mflag = 0;  /* 003BF6 CLM */
    A = rd16(ABS(0x0226));  /* 003BF7 LDA */
    mflag = 1;  /* 003BFA SEM */
    /* 003BFB CMP */
    if ((A & 0xFF) == 0x2A) goto L_003C06;  /* 003BFD BEQ */
    SETLO(A, rd8(ABS(0x0226)));  /* 003BFF LDA */
    /* 003C02 CMP */
    if ((A & 0xFF) != 0x23) goto L_003C0E;  /* 003C04 BNE */
L_003C06: TICK();
    Y = 0x000D;  /* 003C06 LDY */
    mflag = 0;  /* 003C09 CLM */
    S -= 3; F_0080AA();  /* 003C0A JSRL */
L_003C0E: TICK();
    mflag = 1;  /* 003C0E SEM */
    SETLO(A, rd8(DP(0x01)));  /* 003C0F LDA */
    if ((A & 0xFF) == 0) goto L_003C4F;  /* 003C11 BEQ */
    mflag = 0;  /* 003C13 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x2000);  /* 003C14 CLB */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_003C2F;  /* 003C19 BBC */
    A = rd16(DP(0x02));  /* 003C1F LDA */
    mflag = 1;  /* 003C21 SEM */
    wr8(ABS(0x0369), (uint8_t)A);  /* 003C22 STA */
    wr8(ABS(0x0376), (uint8_t)A);  /* 003C25 STA */
    wr8(ABS(0x024A), 0xFF);  /* 003C28 LDM */
    goto L_003C3A;  /* 003C2C JMP */
L_003C2F: TICK();
    mflag = 0;  /* 003C2F CLM */
    A = rd16(DP(0x03));  /* 003C30 LDA */
    wr16(DP(0x04), A);  /* 003C32 STA */
    Y = rd16(DP(0x02));  /* 003C34 LDY */
    S -= 3; F_003610();  /* 003C36 JSRL */
L_003C3A: TICK();
    Y = 0x002C;  /* 003C3A LDY */
    mflag = 0;  /* 003C3D CLM */
    S -= 3; F_003310();  /* 003C3E JSRL */
    mflag = 1;  /* 003C42 SEM */
    wr8(ABS(0x0293), 0x00);  /* 003C43 LDM */
    wr8(ABS(0x0230), 0x00);  /* 003C47 LDM */
    wr8(ABS(0x0231), 0x00);  /* 003C4B LDM */
L_003C4F: TICK();
    mflag = 0;  /* 003C4F CLM */
    Y = pull16();  /* 003C50 PLY */
    D = pull16();  /* 003C51 PLD */
    S += 3; return;  /* 003C52 RTL */
}

/* 003C53  callers:  */
void F_003C53(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003C53 LDA */
    push16(A);  /* 003C55 PHA */
    A = rd16(ABS(0x0226));  /* 003C56 LDA */
    mflag = 1;  /* 003C59 SEM */
    /* 003C5A CMP */
    if ((A & 0xFF) != 0x73) goto L_003CA6;  /* 003C5C BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 003C5E LDA */
    if ((A & 0xFF) == 0) goto L_003C8A;  /* 003C61 BEQ */
    mflag = 0;  /* 003C63 CLM */
    if ((rd16(ABS(0x023C)) & 0x1000) == 0) goto L_003C77;  /* 003C64 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x1000);  /* 003C6A CLB */
    mflag = 1;  /* 003C6F SEM */
    wr8(ABS(0x0258), 0x2A);  /* 003C70 LDM */
    goto L_003C82;  /* 003C74 JMP */
L_003C77: TICK();
    mflag = 0;  /* 003C77 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x1000);  /* 003C78 SEB */
    mflag = 1;  /* 003C7D SEM */
    wr8(ABS(0x0258), 0x2D);  /* 003C7E LDM */
L_003C82: TICK();
    mflag = 0;  /* 003C82 CLM */
    S -= 3; F_008DED();  /* 003C83 JSRL */
    goto L_003CA3;  /* 003C87 JMP */
L_003C8A: TICK();
    mflag = 1;  /* 003C8A SEM */
    wr8(ABS(0x0293), 0x01);  /* 003C8B LDM */
    mflag = 0;  /* 003C8F CLM */
    if ((rd16(ABS(0x023C)) & 0x1000) == 0) goto L_003C9C;  /* 003C90 BBC */
    A = 0x002D;  /* 003C96 LDA */
    goto L_003C9F;  /* 003C99 JMP */
L_003C9C: TICK();
    A = 0x002A;  /* 003C9C LDA */
L_003C9F: TICK();
    mflag = 1;  /* 003C9F SEM */
    wr8(ABS(0x0258), (uint8_t)A);  /* 003CA0 STA */
L_003CA3: TICK();
    goto L_003CAB;  /* 003CA3 JMP */
L_003CA6: TICK();
    mflag = 1;  /* 003CA6 SEM */
    wr8(ABS(0x0293), 0x00);  /* 003CA7 LDM */
L_003CAB: TICK();
    mflag = 0;  /* 003CAB CLM */
    Y = pull16();  /* 003CAC PLY */
    S += 3; return;  /* 003CAD RTL */
}

/* 003CAE  callers:  */
void F_003CAE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003CAE LDA */
    push16(A);  /* 003CB0 PHA */
    A = rd16(ABS(0x0226));  /* 003CB1 LDA */
    mflag = 1;  /* 003CB4 SEM */
    /* 003CB5 CMP */
    if ((A & 0xFF) != 0x73) goto L_003D01;  /* 003CB7 BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 003CB9 LDA */
    if ((A & 0xFF) == 0) goto L_003CE5;  /* 003CBC BEQ */
    mflag = 0;  /* 003CBE CLM */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_003CD2;  /* 003CBF BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0800);  /* 003CC5 CLB */
    mflag = 1;  /* 003CCA SEM */
    wr8(ABS(0x0258), 0x2A);  /* 003CCB LDM */
    goto L_003CDD;  /* 003CCF JMP */
L_003CD2: TICK();
    mflag = 0;  /* 003CD2 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0800);  /* 003CD3 SEB */
    mflag = 1;  /* 003CD8 SEM */
    wr8(ABS(0x0258), 0x2D);  /* 003CD9 LDM */
L_003CDD: TICK();
    mflag = 0;  /* 003CDD CLM */
    S -= 3; F_008DED();  /* 003CDE JSRL */
    goto L_003CFE;  /* 003CE2 JMP */
L_003CE5: TICK();
    mflag = 1;  /* 003CE5 SEM */
    wr8(ABS(0x0293), 0x01);  /* 003CE6 LDM */
    mflag = 0;  /* 003CEA CLM */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_003CF7;  /* 003CEB BBC */
    A = 0x002D;  /* 003CF1 LDA */
    goto L_003CFA;  /* 003CF4 JMP */
L_003CF7: TICK();
    A = 0x002A;  /* 003CF7 LDA */
L_003CFA: TICK();
    mflag = 1;  /* 003CFA SEM */
    wr8(ABS(0x0258), (uint8_t)A);  /* 003CFB STA */
L_003CFE: TICK();
    goto L_003D06;  /* 003CFE JMP */
L_003D01: TICK();
    mflag = 1;  /* 003D01 SEM */
    wr8(ABS(0x0293), 0x00);  /* 003D02 LDM */
L_003D06: TICK();
    mflag = 0;  /* 003D06 CLM */
    Y = pull16();  /* 003D07 PLY */
    S += 3; return;  /* 003D08 RTL */
}

/* 003D09  callers:  */
void F_003D09(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003D09 LDA */
    push16(D);  /* 003D0B PHD */
    D = A;  /* 003D0C TAD */
    C = 0;  /* 003D0D CLC */
    A = adc16(A, 0x000B);  /* 003D0E ADC */
    push16(A);  /* 003D11 PHA */
    A = rd16(ABS(0x0226));  /* 003D12 LDA */
    mflag = 1;  /* 003D15 SEM */
    /* 003D16 CMP */
    if ((A & 0xFF) == 0x64) goto L_003D1D;  /* 003D18 BEQ */
    goto L_003DAA;  /* 003D1A JMP */
L_003D1D: TICK();
    Y = 0x0021;  /* 003D1D LDY */
    mflag = 0;  /* 003D20 CLM */
    S -= 3; F_003310();  /* 003D21 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0080);  /* 003D25 SEB */
    A = rd16(ABS(0x0293));  /* 003D2A LDA */
    mflag = 1;  /* 003D2D SEM */
    /* 003D2E CMP */
    if ((A & 0xFF) != 0x01) goto L_003D39;  /* 003D30 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 003D32 LDA */
    /* 003D35 CMP */
    if ((A & 0xFF) >= 0x33) goto L_003D63;  /* 003D37 BCS */
L_003D39: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 003D39 LDA */
    /* 003D3C CMP */
    if ((A & 0xFF) != 0x02) goto L_003D4E;  /* 003D3E BNE */
    SETLO(A, rd8(ABS(0x0284)));  /* 003D40 LDA */
    /* 003D43 CMP */
    if ((A & 0xFF) < 0x34) goto L_003D4E;  /* 003D45 BCC */
    SETLO(A, rd8(ABS(0x0283)));  /* 003D47 LDA */
    /* 003D4A CMP */
    if ((A & 0xFF) == 0x32) goto L_003D63;  /* 003D4C BEQ */
L_003D4E: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 003D4E LDA */
    /* 003D51 CMP */
    if ((A & 0xFF) != 0x03) goto L_003D5C;  /* 003D53 BNE */
    SETLO(A, rd8(ABS(0x0285)));  /* 003D55 LDA */
    /* 003D58 CMP */
    if ((A & 0xFF) >= 0x36) goto L_003D63;  /* 003D5A BCS */
L_003D5C: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 003D5C LDA */
    /* 003D5F CMP */
    if ((A & 0xFF) < 0x05) goto L_003D66;  /* 003D61 BCC */
L_003D63: TICK();
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 003D63 DEC */
L_003D66: TICK();
    wr8(DP(0x04), 0x00);  /* 003D66 LDM */
L_003D69: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 003D69 LDA */
    t = rd8(ABS(0x0293));  /* 003D6B CMP */
    if ((A & 0xFF) >= t) goto L_003DA7;  /* 003D6E BCS */
    /* 003D70 CMP */
    if ((A & 0xFF) >= 0x02) goto L_003D8B;  /* 003D72 BCS */
    mflag = 0;  /* 003D74 CLM */
    A &= 0x00FF;  /* 003D75 AND */
    X = A;  /* 003D78 TAX */
    A = rd16(DP(0x04));  /* 003D79 LDA */
    A &= 0x00FF;  /* 003D7B AND */
    A += 1;  /* 003D7E INC */
    A += 1;  /* 003D7F INC */
    Y = A;  /* 003D80 TAY */
    A = rd16((ABS(0x0283) + X));  /* 003D81 LDA */
    mflag = 1;  /* 003D84 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 003D85 STA */
    goto L_003DA2;  /* 003D88 JMP */
L_003D8B: TICK();
    mflag = 0;  /* 003D8B CLM */
    A = rd16(DP(0x04));  /* 003D8C LDA */
    A &= 0x00FF;  /* 003D8E AND */
    X = A;  /* 003D91 TAX */
    A = rd16(DP(0x04));  /* 003D92 LDA */
    A &= 0x00FF;  /* 003D94 AND */
    A += 1;  /* 003D97 INC */
    A += 1;  /* 003D98 INC */
    A += 1;  /* 003D99 INC */
    Y = A;  /* 003D9A TAY */
    A = rd16((ABS(0x0283) + X));  /* 003D9B LDA */
    mflag = 1;  /* 003D9E SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 003D9F STA */
L_003DA2: TICK();
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 003DA2 INC */
    goto L_003D69;  /* 003DA4 JMP */
L_003DA7: TICK();
    goto L_003F42;  /* 003DA7 JMP */
L_003DAA: TICK();
    mflag = 0;  /* 003DAA CLM */
    A = rd16(ABS(0x0226));  /* 003DAB LDA */
    mflag = 1;  /* 003DAE SEM */
    /* 003DAF CMP */
    if ((A & 0xFF) == 0x73) goto L_003DB6;  /* 003DB1 BEQ */
    goto L_003F42;  /* 003DB3 JMP */
L_003DB6: TICK();
    mflag = 0;  /* 003DB6 CLM */
    A = D;  /* 003DB7 TDA */
    C = 0;  /* 003DB8 CLC */
    A = adc16(A, 0x0009);  /* 003DB9 ADC */
    B = 0x0000;  /* 003DBC LDA */
    Y = A;  /* 003DC0 TAY */
    S -= 3; F_0076DB();  /* 003DC1 JSRL */
    wr16(DP(0x0B), A);  /* 003DC5 STA */
    A = D;  /* 003DC7 TDA */
    wr16(DP(0x0D), A);  /* 003DC8 STA */
    mflag = 1;  /* 003DCA SEM */
    wr8(DP(0x0F), (uint8_t)B);  /* 003DCB STA */
    mflag = 0;  /* 003DCE CLM */
    A = rd16(DP(0x0B));  /* 003DCF LDA */
    wr16(DP(0x10), A);  /* 003DD1 STA */
    A = rd16(DP(0x0F));  /* 003DD3 LDA */
    mflag = 1;  /* 003DD5 SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 003DD6 STA */
    mflag = 0;  /* 003DD8 CLM */
    A = rd16(DP(0x0D));  /* 003DD9 LDA */
    wr16(DP(0x13), A);  /* 003DDB STA */
    mflag = 1;  /* 003DDD SEM */
    wr8(DP(0x15), 0x00);  /* 003DDE LDM */
    mflag = 0;  /* 003DE1 CLM */
    A = 0x0010;  /* 003DE2 LDA */
    Y = 0x0002;  /* 003DE5 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 003DE8 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0080);  /* 003DEC CLB */
    A = rd16(ABS(0x0293));  /* 003DF1 LDA */
    mflag = 1;  /* 003DF4 SEM */
    /* 003DF5 CMP */
    if ((A & 0xFF) >= 0x04) goto L_003DFC;  /* 003DF7 BCS */
    goto L_003EDE;  /* 003DF9 JMP */
L_003DFC: TICK();
    mflag = 0;  /* 003DFC CLM */
    wr16(DP(0x05), 0x0000);  /* 003DFD LDM */
    wr16(DP(0x07), 0x0000);  /* 003E01 LDM */
    mflag = 1;  /* 003E05 SEM */
    wr8(DP(0x04), 0x00);  /* 003E06 LDM */
L_003E09: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 003E09 LDA */
    t = rd8(DP(0x00));  /* 003E0B CMP */
    C = ((A & 0xFF) >= t);
    if (C) goto L_003E2F;  /* 003E0D BCS */
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 003E0F INC */
    mflag = 0;  /* 003E11 CLM */
    A &= 0x00FF;  /* 003E12 AND */
    X = A;  /* 003E15 TAX */
    A = rd16((0x002AFA + X));  /* 003E16 LDA */
    A &= 0x00FF;  /* 003E1A AND */
    B = 0x0000;  /* 003E1D LDA */
    A = adc16(A, rd16(DP(0x05)));  /* 003E21 ADC */
    B = adc16(B, rd16(DP(0x07)));  /* 003E23 ADC */
    wr16(DP(0x05), A);  /* 003E26 STA */
    wr16(DP(0x07), B);  /* 003E28 STA */
    mflag = 1;  /* 003E2B SEM */
    goto L_003E09;  /* 003E2C JMP */
L_003E2F: TICK();
    mflag = 0;  /* 003E2F CLM */
    A = rd16(ABS(0x0285));  /* 003E30 LDA */
    mflag = 1;  /* 003E33 SEM */
    mpy8(0x0A);  /* 003E34 MPY */
    wr8(DP(0x0B), (uint8_t)A);  /* 003E37 STA */
    wr8(DP(0x0C), (uint8_t)B);  /* 003E39 STA */
    SETLO(A, rd8(ABS(0x0286)));  /* 003E3C LDA */
    mflag = 0;  /* 003E3F CLM */
    A &= 0x00FF;  /* 003E40 AND */
    C = 0;  /* 003E43 CLC */
    A = adc16(A, rd16(DP(0x0B)));  /* 003E44 ADC */
    C = 0;  /* 003E46 CLC */
    A = adc16(A, 0xFDF0);  /* 003E47 ADC */
    B = 0x0000;  /* 003E4A LDA */
    /* 003E4E CMP */
    if (A < 0x8000) goto L_003E55;  /* 003E51 BCC */
    B -= 1;  /* 003E53 DEC */
L_003E55: TICK();
    wr16(DP(0x0B), A);  /* 003E55 STA */
    A = rd16(ABS(0x0283));  /* 003E57 LDA */
    wr16(DP(0x0D), B);  /* 003E5A STA */
    mflag = 1;  /* 003E5D SEM */
    mpy8(0x0A);  /* 003E5E MPY */
    wr8(DP(0x0F), (uint8_t)A);  /* 003E61 STA */
    wr8(DP(0x10), (uint8_t)B);  /* 003E63 STA */
    SETLO(A, rd8(ABS(0x0284)));  /* 003E66 LDA */
    mflag = 0;  /* 003E69 CLM */
    A &= 0x00FF;  /* 003E6A AND */
    C = 0;  /* 003E6D CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 003E6E ADC */
    C = 0;  /* 003E70 CLC */
    A = adc16(A, 0xFDF0);  /* 003E71 ADC */
    B = 0x0000;  /* 003E74 LDA */
    /* 003E78 CMP */
    if (A < 0x8000) goto L_003E7F;  /* 003E7B BCC */
    B -= 1;  /* 003E7D DEC */
L_003E7F: TICK();
    wr16(DP(0x0F), A);  /* 003E7F STA */
    A = rd16(DP(0x01));  /* 003E81 LDA */
    A &= 0x00FF;  /* 003E83 AND */
    wr16(DP(0x11), B);  /* 003E86 STA */
    B = 0x0000;  /* 003E89 LDA */
    C = 0;  /* 003E8D CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 003E8E ADC */
    B = adc16(B, rd16(DP(0x07)));  /* 003E90 ADC */
    C = 0;  /* 003E93 CLC */
    A = adc16(A, 0xFFFF);  /* 003E94 ADC */
    B = adc16(B, 0xFFFF);  /* 003E97 ADC */
    wr16(DP(0x15), 0x0000);  /* 003E9B LDM */
    wr16(DP(0x13), 0x0018);  /* 003E9F LDM */
    X = 0x0013;  /* 003EA3 LDX */
    S -= 3; rt_lmul_01E3EA();  /* 003EA6 JSRL */
    C = 0;  /* 003EAA CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 003EAB ADC */
    B = adc16(B, rd16(DP(0x11)));  /* 003EAD ADC */
    wr16(DP(0x11), 0x0000);  /* 003EB0 LDM */
    wr16(DP(0x0F), 0x003C);  /* 003EB4 LDM */
    X = 0x000F;  /* 003EB8 LDX */
    S -= 3; rt_lmul_01E3EA();  /* 003EBB JSRL */
    C = 0;  /* 003EBF CLC */
    A = adc16(A, rd16(DP(0x0B)));  /* 003EC0 ADC */
    B = adc16(B, rd16(DP(0x0D)));  /* 003EC2 ADC */
    wr16(DP(0x0D), 0x0000);  /* 003EC5 LDM */
    wr16(DP(0x0B), 0x003C);  /* 003EC9 LDM */
    X = 0x000B;  /* 003ECD LDX */
    S -= 3; rt_lmul_01E3EA();  /* 003ED0 JSRL */
    wr16(ABS(0x01F1), A);  /* 003ED4 STA */
    wr16(ABS(0x01F3), B);  /* 003ED7 STA */
    goto L_003F3D;  /* 003EDB JMP */
L_003EDE: TICK();
    mflag = 0;  /* 003EDE CLM */
    A = D;  /* 003EDF TDA */
    C = 0;  /* 003EE0 CLC */
    A = adc16(A, 0x0009);  /* 003EE1 ADC */
    B = 0x0000;  /* 003EE4 LDA */
    Y = A;  /* 003EE8 TAY */
    S -= 3; F_007771();  /* 003EE9 JSRL */
    wr16(DP(0x0B), A);  /* 003EED STA */
    A = D;  /* 003EEF TDA */
    A += 1;  /* 003EF0 INC */
    A += 1;  /* 003EF1 INC */
    wr16(DP(0x0D), A);  /* 003EF2 STA */
    mflag = 1;  /* 003EF4 SEM */
    wr8(DP(0x0F), (uint8_t)B);  /* 003EF5 STA */
    mflag = 0;  /* 003EF8 CLM */
    A = rd16(DP(0x0B));  /* 003EF9 LDA */
    wr16(DP(0x10), A);  /* 003EFB STA */
    A = rd16(DP(0x0F));  /* 003EFD LDA */
    mflag = 1;  /* 003EFF SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 003F00 STA */
    mflag = 0;  /* 003F02 CLM */
    A = rd16(DP(0x0D));  /* 003F03 LDA */
    wr16(DP(0x13), A);  /* 003F05 STA */
    mflag = 1;  /* 003F07 SEM */
    wr8(DP(0x15), 0x00);  /* 003F08 LDM */
    mflag = 0;  /* 003F0B CLM */
    A = 0x0010;  /* 003F0C LDA */
    Y = 0x0002;  /* 003F0F LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 003F12 JSRL */
    A = rd16(DP(0x02));  /* 003F16 LDA */
    wr16(DP(0x0B), A);  /* 003F18 STA */
    Y = 0x0002;  /* 003F1A LDY */
    S -= 3; F_0032CE();  /* 003F1D JSRL */
    A = rd16(DP(0x03));  /* 003F21 LDA */
    wr16(DP(0x0B), A);  /* 003F23 STA */
    Y = 0x0005;  /* 003F25 LDY */
    S -= 3; F_0032CE();  /* 003F28 JSRL */
    mflag = 1;  /* 003F2C SEM */
    wr8(ABS(0x0256), 0x3A);  /* 003F2D LDM */
    wr8(ABS(0x0259), 0x20);  /* 003F31 LDM */
    wr8(ABS(0x0253), 0x20);  /* 003F35 LDM */
    wr8(ABS(0x0252), 0x20);  /* 003F39 LDM */
L_003F3D: TICK();
    mflag = 1;  /* 003F3D SEM */
    wr8(ABS(0x0293), 0x00);  /* 003F3E LDM */
L_003F42: TICK();
    mflag = 0;  /* 003F42 CLM */
    Y = pull16();  /* 003F43 PLY */
    D = pull16();  /* 003F44 PLD */
    S += 3; return;  /* 003F45 RTL */
}

/* 003F46  callers:  */
void F_003F46(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 003F46 LDA */
    push16(D);  /* 003F48 PHD */
    D = A;  /* 003F49 TAD */
    C = 0;  /* 003F4A CLC */
    A = adc16(A, 0x000B);  /* 003F4B ADC */
    push16(A);  /* 003F4E PHA */
    A = rd16(ABS(0x0226));  /* 003F4F LDA */
    mflag = 1;  /* 003F52 SEM */
    /* 003F53 CMP */
    if ((A & 0xFF) == 0x64) goto L_003F5A;  /* 003F55 BEQ */
    goto L_003FFD;  /* 003F57 JMP */
L_003F5A: TICK();
    SETLO(A, rd8(ABS(0x0293)));  /* 003F5A LDA */
    /* 003F5D CMP */
    if ((A & 0xFF) != 0x01) goto L_003F6E;  /* 003F5F BNE */
    Y = 0x0022;  /* 003F61 LDY */
    mflag = 0;  /* 003F64 CLM */
    S -= 3; F_003310();  /* 003F65 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0080);  /* 003F69 SEB */
L_003F6E: TICK();
    mflag = 0;  /* 003F6E CLM */
    A = rd16(ABS(0x0293));  /* 003F6F LDA */
    mflag = 1;  /* 003F72 SEM */
    /* 003F73 CMP */
    if ((A & 0xFF) != 0x01) goto L_003F84;  /* 003F75 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 003F77 LDA */
    /* 003F7A CMP */
    if ((A & 0xFF) >= 0x34) goto L_003F84;  /* 003F7C BCS */
    wr8(ABS(0x0254), (uint8_t)A);  /* 003F7E STA */
    goto L_003FFA;  /* 003F81 JMP */
L_003F84: TICK();
    mflag = 0;  /* 003F84 CLM */
    A = rd16(ABS(0x0293));  /* 003F85 LDA */
    mflag = 1;  /* 003F88 SEM */
    /* 003F89 CMP */
    if ((A & 0xFF) != 0x02) goto L_003FB2;  /* 003F8B BNE */
    SETLO(A, rd8(ABS(0x0284)));  /* 003F8D LDA */
    /* 003F90 CMP */
    if ((A & 0xFF) != 0x30) goto L_003F9B;  /* 003F92 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 003F94 LDA */
    /* 003F97 CMP */
    if ((A & 0xFF) == 0x30) goto L_003FB2;  /* 003F99 BEQ */
L_003F9B: TICK();
    SETLO(A, rd8(ABS(0x0284)));  /* 003F9B LDA */
    /* 003F9E CMP */
    if ((A & 0xFF) < 0x32) goto L_003FA9;  /* 003FA0 BCC */
    SETLO(A, rd8(ABS(0x0283)));  /* 003FA2 LDA */
    /* 003FA5 CMP */
    if ((A & 0xFF) == 0x33) goto L_003FB2;  /* 003FA7 BEQ */
L_003FA9: TICK();
    SETLO(A, rd8(ABS(0x0284)));  /* 003FA9 LDA */
    wr8(ABS(0x0255), (uint8_t)A);  /* 003FAC STA */
    goto L_003FFA;  /* 003FAF JMP */
L_003FB2: TICK();
    mflag = 0;  /* 003FB2 CLM */
    A = rd16(ABS(0x0293));  /* 003FB3 LDA */
    mflag = 1;  /* 003FB6 SEM */
    /* 003FB7 CMP */
    if ((A & 0xFF) != 0x03) goto L_003FC8;  /* 003FB9 BNE */
    SETLO(A, rd8(ABS(0x0285)));  /* 003FBB LDA */
    /* 003FBE CMP */
    if ((A & 0xFF) >= 0x32) goto L_003FC8;  /* 003FC0 BCS */
    wr8(ABS(0x0257), (uint8_t)A);  /* 003FC2 STA */
    goto L_003FFA;  /* 003FC5 JMP */
L_003FC8: TICK();
    mflag = 0;  /* 003FC8 CLM */
    A = rd16(ABS(0x0293));  /* 003FC9 LDA */
    mflag = 1;  /* 003FCC SEM */
    /* 003FCD CMP */
    if ((A & 0xFF) != 0x04) goto L_003FF6;  /* 003FCF BNE */
    SETLO(A, rd8(ABS(0x0286)));  /* 003FD1 LDA */
    /* 003FD4 CMP */
    if ((A & 0xFF) < 0x33) goto L_003FDF;  /* 003FD6 BCC */
    SETLO(A, rd8(ABS(0x0285)));  /* 003FD8 LDA */
    /* 003FDB CMP */
    if ((A & 0xFF) == 0x31) goto L_003FF6;  /* 003FDD BEQ */
L_003FDF: TICK();
    SETLO(A, rd8(ABS(0x0286)));  /* 003FDF LDA */
    /* 003FE2 CMP */
    if ((A & 0xFF) != 0x30) goto L_003FED;  /* 003FE4 BNE */
    SETLO(A, rd8(ABS(0x0285)));  /* 003FE6 LDA */
    /* 003FE9 CMP */
    if ((A & 0xFF) == 0x30) goto L_003FF6;  /* 003FEB BEQ */
L_003FED: TICK();
    SETLO(A, rd8(ABS(0x0286)));  /* 003FED LDA */
    wr8(ABS(0x0258), (uint8_t)A);  /* 003FF0 STA */
    goto L_003FFA;  /* 003FF3 JMP */
L_003FF6: TICK();
    mflag = 1;  /* 003FF6 SEM */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 003FF7 DEC */
L_003FFA: TICK();
    goto L_004199;  /* 003FFA JMP */
L_003FFD: TICK();
    mflag = 0;  /* 003FFD CLM */
    A = rd16(ABS(0x0226));  /* 003FFE LDA */
    mflag = 1;  /* 004001 SEM */
    /* 004002 CMP */
    if ((A & 0xFF) == 0x73) goto L_004009;  /* 004004 BEQ */
    goto L_004199;  /* 004006 JMP */
L_004009: TICK();
    mflag = 0;  /* 004009 CLM */
    A = D;  /* 00400A TDA */
    C = 0;  /* 00400B CLC */
    A = adc16(A, 0x0009);  /* 00400C ADC */
    B = 0x0000;  /* 00400F LDA */
    Y = A;  /* 004013 TAY */
    S -= 3; F_007771();  /* 004014 JSRL */
    wr16(DP(0x0B), A);  /* 004018 STA */
    A = D;  /* 00401A TDA */
    wr16(DP(0x0D), A);  /* 00401B STA */
    mflag = 1;  /* 00401D SEM */
    wr8(DP(0x0F), (uint8_t)B);  /* 00401E STA */
    mflag = 0;  /* 004021 CLM */
    A = rd16(DP(0x0B));  /* 004022 LDA */
    wr16(DP(0x10), A);  /* 004024 STA */
    A = rd16(DP(0x0F));  /* 004026 LDA */
    mflag = 1;  /* 004028 SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 004029 STA */
    mflag = 0;  /* 00402B CLM */
    A = rd16(DP(0x0D));  /* 00402C LDA */
    wr16(DP(0x13), A);  /* 00402E STA */
    mflag = 1;  /* 004030 SEM */
    wr8(DP(0x15), 0x00);  /* 004031 LDM */
    mflag = 0;  /* 004034 CLM */
    A = 0x0010;  /* 004035 LDA */
    Y = 0x0002;  /* 004038 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00403B JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0080);  /* 00403F CLB */
    A = rd16(ABS(0x0293));  /* 004044 LDA */
    mflag = 1;  /* 004047 SEM */
    /* 004048 CMP */
    if ((A & 0xFF) >= 0x04) goto L_00404F;  /* 00404A BCS */
    goto L_004132;  /* 00404C JMP */
L_00404F: TICK();
    SETLO(A, rd8(ABS(0x0285)));  /* 00404F LDA */
    mpy8(0x0A);  /* 004052 MPY */
    C = 0;  /* 004055 CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0286))));  /* 004056 ADC */
    C = 0;  /* 004059 CLC */
    SETLO(A, adc8(A, 0xEF));  /* 00405A ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 00405C STA */
    SETLO(A, rd8(ABS(0x0283)));  /* 00405E LDA */
    mpy8(0x0A);  /* 004061 MPY */
    C = 0;  /* 004064 CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0284))));  /* 004065 ADC */
    C = 0;  /* 004068 CLC */
    SETLO(A, adc8(A, 0xF0));  /* 004069 ADC */
    wr8(DP(0x03), (uint8_t)A);  /* 00406B STA */
    mflag = 0;  /* 00406D CLM */
    wr16(DP(0x05), 0x0000);  /* 00406E LDM */
    wr16(DP(0x07), 0x0000);  /* 004072 LDM */
    mflag = 1;  /* 004076 SEM */
    wr8(DP(0x04), 0x00);  /* 004077 LDM */
    wr8(ABS(0x0293), 0x00);  /* 00407A LDM */
L_00407E: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 00407E LDA */
    t = rd8(DP(0x02));  /* 004080 CMP */
    C = ((A & 0xFF) >= t);
    if (C) goto L_0040A4;  /* 004082 BCS */
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 004084 INC */
    mflag = 0;  /* 004086 CLM */
    A &= 0x00FF;  /* 004087 AND */
    X = A;  /* 00408A TAX */
    A = rd16((0x002AFA + X));  /* 00408B LDA */
    A &= 0x00FF;  /* 00408F AND */
    B = 0x0000;  /* 004092 LDA */
    A = adc16(A, rd16(DP(0x05)));  /* 004096 ADC */
    B = adc16(B, rd16(DP(0x07)));  /* 004098 ADC */
    wr16(DP(0x05), A);  /* 00409B STA */
    wr16(DP(0x07), B);  /* 00409D STA */
    mflag = 1;  /* 0040A0 SEM */
    goto L_00407E;  /* 0040A1 JMP */
L_0040A4: TICK();
    mflag = 0;  /* 0040A4 CLM */
    wr16(DP(0x0D), 0x0000);  /* 0040A5 LDM */
    wr16(DP(0x0B), 0x003C);  /* 0040A9 LDM */
    A = rd16(ABS(0x01F1));  /* 0040AD LDA */
    B = rd16(ABS(0x01F3));  /* 0040B0 LDA */
    X = 0x000B;  /* 0040B4 LDX */
    S -= 3; rt_lumod_01E43E();  /* 0040B7 JSRL */
    wr16(DP(0x0B), A);  /* 0040BB STA */
    A = rd16(DP(0x03));  /* 0040BD LDA */
    A &= 0x00FF;  /* 0040BF AND */
    wr16(DP(0x0D), B);  /* 0040C2 STA */
    B = 0x0000;  /* 0040C5 LDA */
    C = 0;  /* 0040C9 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 0040CA ADC */
    B = adc16(B, rd16(DP(0x07)));  /* 0040CC ADC */
    C = 0;  /* 0040CF CLC */
    A = adc16(A, 0xFFFF);  /* 0040D0 ADC */
    B = adc16(B, 0xFFFF);  /* 0040D3 ADC */
    wr16(DP(0x11), 0x0000);  /* 0040D7 LDM */
    wr16(DP(0x0F), 0x0018);  /* 0040DB LDM */
    X = 0x000F;  /* 0040DF LDX */
    S -= 3; rt_lmul_01E3EA();  /* 0040E2 JSRL */
    wr16(DP(0x0F), A);  /* 0040E6 STA */
    A = rd16(DP(0x00));  /* 0040E8 LDA */
    A &= 0x00FF;  /* 0040EA AND */
    wr16(DP(0x11), A);  /* 0040ED STA */
    A = rd16(DP(0x0F));  /* 0040EF LDA */
    C = 0;  /* 0040F1 CLC */
    A = adc16(A, rd16(DP(0x11)));  /* 0040F2 ADC */
    B = adc16(B, 0x0000);  /* 0040F4 ADC */
    wr16(DP(0x11), 0x0000);  /* 0040F8 LDM */
    wr16(DP(0x0F), 0x003C);  /* 0040FC LDM */
    S -= 3; rt_lmul_01E3EA();  /* 004100 JSRL */
    wr16(DP(0x0F), A);  /* 004104 STA */
    A = rd16(DP(0x01));  /* 004106 LDA */
    A &= 0x00FF;  /* 004108 AND */
    wr16(DP(0x11), A);  /* 00410B STA */
    A = rd16(DP(0x0F));  /* 00410D LDA */
    C = 0;  /* 00410F CLC */
    A = adc16(A, rd16(DP(0x11)));  /* 004110 ADC */
    B = adc16(B, 0x0000);  /* 004112 ADC */
    wr16(DP(0x11), 0x0000);  /* 004116 LDM */
    wr16(DP(0x0F), 0x003C);  /* 00411A LDM */
    S -= 3; rt_lmul_01E3EA();  /* 00411E JSRL */
    C = 0;  /* 004122 CLC */
    A = adc16(A, rd16(DP(0x0B)));  /* 004123 ADC */
    B = adc16(B, rd16(DP(0x0D)));  /* 004125 ADC */
    wr16(ABS(0x01F1), A);  /* 004128 STA */
    wr16(ABS(0x01F3), B);  /* 00412B STA */
    goto L_004194;  /* 00412F JMP */
L_004132: TICK();
    mflag = 0;  /* 004132 CLM */
    A = D;  /* 004133 TDA */
    C = 0;  /* 004134 CLC */
    A = adc16(A, 0x0009);  /* 004135 ADC */
    B = 0x0000;  /* 004138 LDA */
    Y = A;  /* 00413C TAY */
    S -= 3; F_0076DB();  /* 00413D JSRL */
    wr16(DP(0x0B), A);  /* 004141 STA */
    A = D;  /* 004143 TDA */
    A += 1;  /* 004144 INC */
    A += 1;  /* 004145 INC */
    wr16(DP(0x0D), A);  /* 004146 STA */
    mflag = 1;  /* 004148 SEM */
    wr8(DP(0x0F), (uint8_t)B);  /* 004149 STA */
    mflag = 0;  /* 00414C CLM */
    A = rd16(DP(0x0B));  /* 00414D LDA */
    wr16(DP(0x10), A);  /* 00414F STA */
    A = rd16(DP(0x0F));  /* 004151 LDA */
    mflag = 1;  /* 004153 SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 004154 STA */
    mflag = 0;  /* 004156 CLM */
    A = rd16(DP(0x0D));  /* 004157 LDA */
    wr16(DP(0x13), A);  /* 004159 STA */
    mflag = 1;  /* 00415B SEM */
    wr8(DP(0x15), 0x00);  /* 00415C LDM */
    mflag = 0;  /* 00415F CLM */
    A = 0x0010;  /* 004160 LDA */
    Y = 0x0002;  /* 004163 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 004166 JSRL */
    A = rd16(DP(0x03));  /* 00416A LDA */
    wr16(DP(0x0B), A);  /* 00416C STA */
    Y = 0x0002;  /* 00416E LDY */
    S -= 3; F_0032CE();  /* 004171 JSRL */
    A = rd16(DP(0x02));  /* 004175 LDA */
    mflag = 1;  /* 004177 SEM */
    SETLO(A, A + 1);  /* 004178 INC */
    wr8(DP(0x0B), (uint8_t)A);  /* 004179 STA */
    Y = 0x0005;  /* 00417B LDY */
    mflag = 0;  /* 00417E CLM */
    S -= 3; F_0032CE();  /* 00417F JSRL */
    mflag = 1;  /* 004183 SEM */
    wr8(ABS(0x0256), 0x2F);  /* 004184 LDM */
    wr8(ABS(0x0259), 0x20);  /* 004188 LDM */
    wr8(ABS(0x0253), 0x20);  /* 00418C LDM */
    wr8(ABS(0x0252), 0x20);  /* 004190 LDM */
L_004194: TICK();
    mflag = 1;  /* 004194 SEM */
    wr8(ABS(0x0293), 0x00);  /* 004195 LDM */
L_004199: TICK();
    mflag = 0;  /* 004199 CLM */
    Y = pull16();  /* 00419A PLY */
    D = pull16();  /* 00419B PLD */
    S += 3; return;  /* 00419C RTL */
}

/* 00419D  callers:  */
void F_00419D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00419D LDA */
    push16(A);  /* 00419F PHA */
    A = rd16(ABS(0x0226));  /* 0041A0 LDA */
    mflag = 1;  /* 0041A3 SEM */
    /* 0041A4 CMP */
    if ((A & 0xFF) != 0x73) goto L_0041B2;  /* 0041A6 BNE */
    mflag = 0;  /* 0041A8 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0100);  /* 0041A9 SEB */
    S -= 3; F_008DED();  /* 0041AE JSRL */
L_0041B2: TICK();
    mflag = 0;  /* 0041B2 CLM */
    Y = pull16();  /* 0041B3 PLY */
    S += 3; return;  /* 0041B4 RTL */
}

/* 0041B5  callers:  */
void F_0041B5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0041B5 LDA */
    push16(A);  /* 0041B7 PHA */
    A = rd16(ABS(0x0226));  /* 0041B8 LDA */
    mflag = 1;  /* 0041BB SEM */
    /* 0041BC CMP */
    if ((A & 0xFF) != 0x73) goto L_0041CA;  /* 0041BE BNE */
    mflag = 0;  /* 0041C0 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0100);  /* 0041C1 CLB */
    S -= 3; F_008DED();  /* 0041C6 JSRL */
L_0041CA: TICK();
    mflag = 0;  /* 0041CA CLM */
    Y = pull16();  /* 0041CB PLY */
    S += 3; return;  /* 0041CC RTL */
}

/* 0041CD  callers: 00438C, 00445C, 0044AE */
void F_0041CD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0041CD LDA */
    push16(D);  /* 0041CF PHD */
    D = A;  /* 0041D0 TAD */
    C = 0;  /* 0041D1 CLC */
    A = adc16(A, 0x000A);  /* 0041D2 ADC */
    push16(A);  /* 0041D5 PHA */
    wr16(DP(0x00), Y);  /* 0041D6 STY */
    A = Y;  /* 0041D8 TYA */
    mflag = 1;  /* 0041D9 SEM */
    wr8(ABS(0x0294), (uint8_t)A);  /* 0041DA STA */
    Y = 0x002B;  /* 0041DD LDY */
    mflag = 0;  /* 0041E0 CLM */
    S -= 3; F_003310();  /* 0041E1 JSRL */
    A = rd16(DP(0x00));  /* 0041E5 LDA */
    wr16(DP(0x0A), A);  /* 0041E7 STA */
    Y = 0x0003;  /* 0041E9 LDY */
    S -= 3; F_0032CE();  /* 0041EC JSRL */
    mflag = 1;  /* 0041F0 SEM */
    wr8(ABS(0x0293), 0x00);  /* 0041F1 LDM */
    mflag = 0;  /* 0041F5 CLM */
    wr16(DP(0x0A), 0x0008);  /* 0041F6 LDM */
    A = rd16(DP(0x00));  /* 0041FA LDA */
    A &= 0x00FF;  /* 0041FC AND */
    A -= 1;  /* 0041FF DEC */
    wr16(DP(0x0C), A);  /* 004200 STA */
    A = D;  /* 004202 TDA */
    C = 0;  /* 004203 CLC */
    A = adc16(A, 0x0006);  /* 004204 ADC */
    B = 0x0000;  /* 004207 LDA */
    Y = A;  /* 00420B TAY */
    S -= 3; F_01E30B();  /* 00420C JSRL */
    wr16(DP(0x0A), A);  /* 004210 STA */
    A = D;  /* 004212 TDA */
    A += 1;  /* 004213 INC */
    A += 1;  /* 004214 INC */
    wr16(DP(0x0C), A);  /* 004215 STA */
    mflag = 1;  /* 004217 SEM */
    wr8(DP(0x0E), (uint8_t)B);  /* 004218 STA */
    mflag = 0;  /* 00421B CLM */
    A = rd16(DP(0x0A));  /* 00421C LDA */
    wr16(DP(0x0F), A);  /* 00421E STA */
    A = rd16(DP(0x0E));  /* 004220 LDA */
    mflag = 1;  /* 004222 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 004223 STA */
    mflag = 0;  /* 004225 CLM */
    A = rd16(DP(0x0C));  /* 004226 LDA */
    wr16(DP(0x12), A);  /* 004228 STA */
    mflag = 1;  /* 00422A SEM */
    wr8(DP(0x14), 0x00);  /* 00422B LDM */
    mflag = 0;  /* 00422E CLM */
    A = 0x000F;  /* 00422F LDA */
    Y = 0x0004;  /* 004232 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 004235 JSRL */
    X = rd16(DP(0x02));  /* 004239 LDX */
    Y = X;  /* 00423B TXY */
    X = rd16(DP(0x04));  /* 00423C LDX */
    A = 0x0001;  /* 00423E LDA */
    mflag = 1;  /* 004241 SEM */
    S -= 3; rt_shl_01E4E0();  /* 004242 JSRL */
    SETLO(A, A & rd8((ABS(0x02D2) + Y)));  /* 004246 AND */
    if ((A & 0xFF) == 0) goto L_004275;  /* 004249 BEQ */
    wr8(ABS(0x0258), 0x2B);  /* 00424B LDM */
    SETLO(A, rd8(ABS(0x038A)));  /* 00424F LDA */
    t = rd8(DP(0x00));  /* 004252 CMP */
    if ((A & 0xFF) != t) goto L_004261;  /* 004254 BNE */
    wr8(ABS(0x0258), 0x50);  /* 004256 LDM */
    wr8(ABS(0x0259), 0x31);  /* 00425A LDM */
    goto L_004272;  /* 00425E JMP */
L_004261: TICK();
    mflag = 0;  /* 004261 CLM */
    A = rd16(ABS(0x0391));  /* 004262 LDA */
    mflag = 1;  /* 004265 SEM */
    t = rd8(DP(0x00));  /* 004266 CMP */
    if ((A & 0xFF) != t) goto L_004272;  /* 004268 BNE */
    wr8(ABS(0x0258), 0x50);  /* 00426A LDM */
    wr8(ABS(0x0259), 0x32);  /* 00426E LDM */
L_004272: TICK();
    goto L_00427A;  /* 004272 JMP */
L_004275: TICK();
    mflag = 1;  /* 004275 SEM */
    wr8(ABS(0x0258), 0x2D);  /* 004276 LDM */
L_00427A: TICK();
    SETLO(A, rd8(ABS(0x0258)));  /* 00427A LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 00427D STA */
    /* 00427F CMP */
    if ((A & 0xFF) != 0x50) goto L_004288;  /* 004281 BNE */
    SETLO(A, rd8(ABS(0x0259)));  /* 004283 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 004286 STA */
L_004288: TICK();
    Y = pull16();  /* 004288 PLY */
    D = pull16();  /* 004289 PLD */
    S += 3; return;  /* 00428A RTL */
}

/* 00428B  callers: 004489 */
void F_00428B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00428B LDA */
    push16(D);  /* 00428D PHD */
    D = A;  /* 00428E TAD */
    A += 1;  /* 00428F INC */
    push16(A);  /* 004290 PHA */
    wr16(DP(0x00), Y);  /* 004291 STY */
    A = rd16(ABS(0x0295));  /* 004293 LDA */
    mflag = 1;  /* 004296 SEM */
    /* 004297 CMP */
    if ((A & 0xFF) != 0x2B) goto L_0042BA;  /* 004299 BNE */
    mflag = 0;  /* 00429B CLM */
    S -= 3; F_007B78();  /* 00429C JSRL */
    A = rd16(ABS(0x038A));  /* 0042A0 LDA */
    mflag = 1;  /* 0042A3 SEM */
    t = rd8(DP(0x00));  /* 0042A4 CMP */
    if ((A & 0xFF) != t) goto L_0042AC;  /* 0042A6 BNE */
    wr8(ABS(0x038A), 0x00);  /* 0042A8 LDM */
L_0042AC: TICK();
    SETLO(A, rd8(ABS(0x0391)));  /* 0042AC LDA */
    t = rd8(DP(0x00));  /* 0042AF CMP */
    if ((A & 0xFF) != t) goto L_0042B7;  /* 0042B1 BNE */
    wr8(ABS(0x0391), 0x00);  /* 0042B3 LDM */
L_0042B7: TICK();
    goto L_00430F;  /* 0042B7 JMP */
L_0042BA: TICK();
    mflag = 0;  /* 0042BA CLM */
    A = rd16(ABS(0x0295));  /* 0042BB LDA */
    mflag = 1;  /* 0042BE SEM */
    /* 0042BF CMP */
    if ((A & 0xFF) != 0x2D) goto L_0042E4;  /* 0042C1 BNE */
    Y = rd16(DP(0x00));  /* 0042C3 LDY */
    mflag = 0;  /* 0042C5 CLM */
    S -= 3; F_007BDE();  /* 0042C6 JSRL */
    A = rd16(ABS(0x038A));  /* 0042CA LDA */
    mflag = 1;  /* 0042CD SEM */
    t = rd8(DP(0x00));  /* 0042CE CMP */
    if ((A & 0xFF) != t) goto L_0042D6;  /* 0042D0 BNE */
    wr8(ABS(0x038A), 0x00);  /* 0042D2 LDM */
L_0042D6: TICK();
    SETLO(A, rd8(ABS(0x0391)));  /* 0042D6 LDA */
    t = rd8(DP(0x00));  /* 0042D9 CMP */
    if ((A & 0xFF) != t) goto L_0042E1;  /* 0042DB BNE */
    wr8(ABS(0x0391), 0x00);  /* 0042DD LDM */
L_0042E1: TICK();
    goto L_00430F;  /* 0042E1 JMP */
L_0042E4: TICK();
    mflag = 0;  /* 0042E4 CLM */
    A = rd16(ABS(0x0295));  /* 0042E5 LDA */
    mflag = 1;  /* 0042E8 SEM */
    /* 0042E9 CMP */
    if ((A & 0xFF) != 0x31) goto L_0042FB;  /* 0042EB BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0042ED LDA */
    wr8(ABS(0x038A), (uint8_t)A);  /* 0042EF STA */
    mflag = 0;  /* 0042F2 CLM */
    Y = A;  /* 0042F3 TAY */
    S -= 3; F_007B78();  /* 0042F4 JSRL */
    goto L_00430F;  /* 0042F8 JMP */
L_0042FB: TICK();
    mflag = 0;  /* 0042FB CLM */
    A = rd16(ABS(0x0295));  /* 0042FC LDA */
    mflag = 1;  /* 0042FF SEM */
    /* 004300 CMP */
    if ((A & 0xFF) != 0x32) goto L_00430F;  /* 004302 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 004304 LDA */
    wr8(ABS(0x0391), (uint8_t)A);  /* 004306 STA */
    mflag = 0;  /* 004309 CLM */
    Y = A;  /* 00430A TAY */
    S -= 3; F_007B78();  /* 00430B JSRL */
L_00430F: TICK();
    mflag = 0;  /* 00430F CLM */
    Y = pull16();  /* 004310 PLY */
    D = pull16();  /* 004311 PLD */
    S += 3; return;  /* 004312 RTL */
}

/* 004313  callers: 00447F, 006848 */
void F_004313(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004313 LDA */
    push16(D);  /* 004315 PHD */
    D = A;  /* 004316 TAD */
    C = 0;  /* 004317 CLC */
    A = adc16(A, 0x0011);  /* 004318 ADC */
    push16(A);  /* 00431B PHA */
    wr16(DP(0x00), 0x0000);  /* 00431C LDM */
L_004320: TICK();
    X = rd16(DP(0x00));  /* 004320 LDX */
    /* 004322 CPX */
    if (X >= 0x000D) goto L_004334;  /* 004325 BCS */
    A = rd16((ABS(0x02D2) + X));  /* 004327 LDA */
    mflag = 1;  /* 00432A SEM */
    wr8(DP(0x02 + X), (uint8_t)A);  /* 00432B STA */
    X += 1;  /* 00432D INX */
    wr16(DP(0x00), X);  /* 00432E STX */
    mflag = 0;  /* 004330 CLM */
    goto L_004320;  /* 004331 JMP */
L_004334: TICK();
    mflag = 0;  /* 004334 CLM */
    A = rd16(ABS(0x038A));  /* 004335 LDA */
    mflag = 1;  /* 004338 SEM */
    wr8(DP(0x0F), (uint8_t)A);  /* 004339 STA */
    SETLO(A, rd8(ABS(0x0391)));  /* 00433B LDA */
    wr8(DP(0x10), (uint8_t)A);  /* 00433E STA */
    wr8(DP(0x11), 0x0F);  /* 004340 LDM */
    mflag = 0;  /* 004343 CLM */
    A = D;  /* 004344 TDA */
    A += 1;  /* 004345 INC */
    A += 1;  /* 004346 INC */
    B = 0x0000;  /* 004347 LDA */
    wr16(DP(0x12), A);  /* 00434B STA */
    wr16(DP(0x14), B);  /* 00434D STA */
    wr16(DP(0x15), 0x0537);  /* 004350 LDM */
    Y = 0x00A0;  /* 004354 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 004357 JSRL */
    mflag = 1;  /* 00435B SEM */
    wr8(ABS(0x0294), 0x00);  /* 00435C LDM */
    wr8(ABS(0x0231), 0x14);  /* 004360 LDM */
    wr8(ABS(0x0293), 0x00);  /* 004364 LDM */
    wr8(ABS(0x0230), 0x00);  /* 004368 LDM */
    mflag = 0;  /* 00436C CLM */
    Y = pull16();  /* 00436D PLY */
    D = pull16();  /* 00436E PLD */
    S += 3; return;  /* 00436F RTL */
}

/* 004370  callers:  */
void F_004370(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004370 LDA */
    push16(D);  /* 004372 PHD */
    D = A;  /* 004373 TAD */
    A += 1;  /* 004374 INC */
    push16(A);  /* 004375 PHA */
    A = rd16(ABS(0x0294));  /* 004376 LDA */
    mflag = 1;  /* 004379 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00437A STA */
    /* 00437C CMP */
    if ((A & 0xFF) != 0x00) goto L_004396;  /* 00437E BNE */
    Y = 0x0001;  /* 004380 LDY */
    mflag = 0;  /* 004383 CLM */
    S -= 3; F_003589();  /* 004384 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 004388 STA */
    mflag = 0;  /* 00438A CLM */
    Y = A;  /* 00438B TAY */
    S -= 3; F_0041CD();  /* 00438C JSRL */
    wr8(ABS(0x0295), (uint8_t)A);  /* 004390 STA */
    goto L_004554;  /* 004393 JMP */
L_004396: TICK();
    mflag = 0;  /* 004396 CLM */
    A = rd16(ABS(0x0226));  /* 004397 LDA */
    mflag = 1;  /* 00439A SEM */
    /* 00439B CMP */
    if ((A & 0xFF) == 0x64) goto L_0043A9;  /* 00439D BEQ */
    SETLO(A, rd8(ABS(0x0231)));  /* 00439F LDA */
    /* 0043A2 CMP */
    if ((A & 0xFF) == 0x15) goto L_0043A9;  /* 0043A4 BEQ */
    goto L_00446E;  /* 0043A6 JMP */
L_0043A9: TICK();
    SETLO(A, rd8(ABS(0x0295)));  /* 0043A9 LDA */
    /* 0043AC CMP */
    if ((A & 0xFF) != 0x2A) goto L_0043B7;  /* 0043AE BNE */
    wr8(ABS(0x0293), 0x00);  /* 0043B0 LDM */
    goto L_00446B;  /* 0043B4 JMP */
L_0043B7: TICK();
    Y = 0x002B;  /* 0043B7 LDY */
    mflag = 0;  /* 0043BA CLM */
    S -= 3; F_003310();  /* 0043BB JSRL */
    A = rd16(ABS(0x0231));  /* 0043BF LDA */
    mflag = 1;  /* 0043C2 SEM */
    /* 0043C3 CMP */
    if ((A & 0xFF) == 0x15) goto L_0043DC;  /* 0043C5 BEQ */
    wr8(ABS(0x0231), 0x15);  /* 0043C7 LDM */
    wr8(ABS(0x0230), 0x3F);  /* 0043CB LDM */
    wr8(ABS(0x0255), 0x20);  /* 0043CF LDM */
    SETLO(A, rd8(ABS(0x0283)));  /* 0043D3 LDA */
    wr8(ABS(0x0256), (uint8_t)A);  /* 0043D6 STA */
    goto L_00446B;  /* 0043D9 JMP */
L_0043DC: TICK();
    mflag = 0;  /* 0043DC CLM */
    A = rd16(ABS(0x0226));  /* 0043DD LDA */
    mflag = 1;  /* 0043E0 SEM */
    /* 0043E1 CMP */
    if ((A & 0xFF) != 0x64) goto L_00440F;  /* 0043E3 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 0043E5 LDA */
    /* 0043E8 CMP */
    if ((A & 0xFF) != 0x30) goto L_00440F;  /* 0043EA BNE */
    SETLO(A, rd8(ABS(0x0284)));  /* 0043EC LDA */
    /* 0043EF CMP */
    if ((A & 0xFF) != 0x30) goto L_00440F;  /* 0043F1 BNE */
    t = (rd8(ABS(0x0293)) - 1) & 0xFF; wr8(ABS(0x0293), t);  /* 0043F3 DEC */
    wr8(ABS(0x0230), 0x3F);  /* 0043F6 LDM */
    wr8(ABS(0x0255), 0x20);  /* 0043FA LDM */
    SETLO(A, rd8(ABS(0x0283)));  /* 0043FE LDA */
    wr8(ABS(0x0256), (uint8_t)A);  /* 004401 STA */
    Y = 0x0002;  /* 004404 LDY */
    mflag = 0;  /* 004407 CLM */
    S -= 3; F_0080AA();  /* 004408 JSRL */
    goto L_00446B;  /* 00440C JMP */
L_00440F: TICK();
    mflag = 0;  /* 00440F CLM */
    A = rd16(ABS(0x0293));  /* 004410 LDA */
    mflag = 1;  /* 004413 SEM */
    /* 004414 CMP */
    if ((A & 0xFF) != 0x02) goto L_00442A;  /* 004416 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 004418 LDA */
    mpy8(0x0A);  /* 00441B MPY */
    C = 0;  /* 00441E CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0284))));  /* 00441F ADC */
    C = 0;  /* 004422 CLC */
    SETLO(A, adc8(A, 0xF0));  /* 004423 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 004425 STA */
    goto L_00443B;  /* 004427 JMP */
L_00442A: TICK();
    mflag = 0;  /* 00442A CLM */
    A = rd16(ABS(0x0293));  /* 00442B LDA */
    mflag = 1;  /* 00442E SEM */
    /* 00442F CMP */
    if ((A & 0xFF) != 0x01) goto L_00443B;  /* 004431 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 004433 LDA */
    C = 0;  /* 004436 CLC */
    SETLO(A, adc8(A, 0xD0));  /* 004437 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 004439 STA */
L_00443B: TICK();
    Y = rd16(DP(0x00));  /* 00443B LDY */
    mflag = 0;  /* 00443D CLM */
    S -= 3; channel_is_programmed_0087A7();  /* 00443E JSRL */
    X = A;  /* 004442 TAX */
    if (X == 0) goto L_00444A;  /* 004443 BEQ */
    mflag = 1;  /* 004445 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 004446 LDA */
    if ((A & 0xFF) != 0) goto L_004459;  /* 004448 BNE */
L_00444A: TICK();
    mflag = 0;  /* 00444A CLM */
    A = rd16(ABS(0x0294));  /* 00444B LDA */
    mflag = 1;  /* 00444E SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00444F STA */
    Y = 0x0002;  /* 004451 LDY */
    mflag = 0;  /* 004454 CLM */
    S -= 3; F_0080AA();  /* 004455 JSRL */
L_004459: TICK();
    Y = rd16(DP(0x00));  /* 004459 LDY */
    mflag = 0;  /* 00445B CLM */
    S -= 3; F_0041CD();  /* 00445C JSRL */
    wr8(ABS(0x0295), (uint8_t)A);  /* 004460 STA */
    wr8(ABS(0x0231), 0x14);  /* 004463 LDM */
    wr8(ABS(0x0230), 0x9D);  /* 004467 LDM */
L_00446B: TICK();
    goto L_004554;  /* 00446B JMP */
L_00446E: TICK();
    mflag = 0;  /* 00446E CLM */
    A = rd16(ABS(0x0226));  /* 00446F LDA */
    mflag = 1;  /* 004472 SEM */
    /* 004473 CMP */
    if ((A & 0xFF) != 0x73) goto L_0044B8;  /* 004475 BNE */
    SETLO(A, rd8(ABS(0x0295)));  /* 004477 LDA */
    /* 00447A CMP */
    if ((A & 0xFF) != 0x2A) goto L_004486;  /* 00447C BNE */
    mflag = 0;  /* 00447E CLM */
    S -= 3; F_004313();  /* 00447F JSRL */
    goto L_0044B5;  /* 004483 JMP */
L_004486: TICK();
    Y = rd16(DP(0x00));  /* 004486 LDY */
    mflag = 0;  /* 004488 CLM */
    S -= 3; F_00428B();  /* 004489 JSRL */
    mflag = 1;  /* 00448D SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00448E INC */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00449A;  /* 004490 BBC */
    SETLO(A, 0x0A);  /* 004495 LDA */
    goto L_00449C;  /* 004497 JMP */
L_00449A: TICK();
    SETLO(A, 0x63);  /* 00449A LDA */
L_00449C: TICK();
    t = rd8(DP(0x00));  /* 00449C CMP */
    if ((A & 0xFF) >= t) goto L_0044A3;  /* 00449E BCS */
    wr8(DP(0x00), 0x01);  /* 0044A0 LDM */
L_0044A3: TICK();
    Y = rd16(DP(0x00));  /* 0044A3 LDY */
    mflag = 0;  /* 0044A5 CLM */
    S -= 3; F_003589();  /* 0044A6 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 0044AA STA */
    mflag = 0;  /* 0044AC CLM */
    Y = A;  /* 0044AD TAY */
    S -= 3; F_0041CD();  /* 0044AE JSRL */
    wr8(ABS(0x0295), (uint8_t)A);  /* 0044B2 STA */
L_0044B5: TICK();
    goto L_004554;  /* 0044B5 JMP */
L_0044B8: TICK();
    mflag = 0;  /* 0044B8 CLM */
    A = rd16(ABS(0x0226));  /* 0044B9 LDA */
    mflag = 1;  /* 0044BC SEM */
    /* 0044BD CMP */
    if ((A & 0xFF) == 0x6D) goto L_0044C4;  /* 0044BF BEQ */
    goto L_004554;  /* 0044C1 JMP */
L_0044C4: TICK();
    Y = 0x002B;  /* 0044C4 LDY */
    mflag = 0;  /* 0044C7 CLM */
    S -= 3; F_003310();  /* 0044C8 JSRL */
    A = rd16(DP(0x00));  /* 0044CC LDA */
    wr16(DP(0x01), A);  /* 0044CE STA */
    Y = 0x0003;  /* 0044D0 LDY */
    S -= 3; F_0032CE();  /* 0044D3 JSRL */
    A = rd16(ABS(0x0295));  /* 0044D7 LDA */
    mflag = 1;  /* 0044DA SEM */
    /* 0044DB CMP */
    if ((A & 0xFF) != 0x2B) goto L_00450A;  /* 0044DD BNE */
    if ((rd8(ABS(0x033F)) & 0x40) == 0) goto L_0044FA;  /* 0044DF BBC */
    Y = rd16(DP(0x00));  /* 0044E4 LDY */
    mflag = 0;  /* 0044E6 CLM */
    S -= 3; F_0091BB();  /* 0044E7 JSRL */
    X = A;  /* 0044EB TAX */
    if (X == 0) goto L_0044FA;  /* 0044EC BEQ */
    mflag = 1;  /* 0044EE SEM */
    wr8(ABS(0x0295), 0x2D);  /* 0044EF LDM */
    wr8(ABS(0x0258), 0x2D);  /* 0044F3 LDM */
    goto L_004507;  /* 0044F7 JMP */
L_0044FA: TICK();
    mflag = 1;  /* 0044FA SEM */
    wr8(ABS(0x0258), 0x50);  /* 0044FB LDM */
    wr8(ABS(0x0295), 0x31);  /* 0044FF LDM */
    wr8(ABS(0x0259), 0x31);  /* 004503 LDM */
L_004507: TICK();
    goto L_004554;  /* 004507 JMP */
L_00450A: TICK();
    mflag = 1;  /* 00450A SEM */
    /* 00450B CMP */
    if ((A & 0xFF) != 0x31) goto L_00451E;  /* 00450D BNE */
    wr8(ABS(0x0258), 0x50);  /* 00450F LDM */
    wr8(ABS(0x0295), 0x32);  /* 004513 LDM */
    wr8(ABS(0x0259), 0x32);  /* 004517 LDM */
    goto L_004554;  /* 00451B JMP */
L_00451E: TICK();
    mflag = 1;  /* 00451E SEM */
    /* 00451F CMP */
    if ((A & 0xFF) != 0x32) goto L_004532;  /* 004521 BNE */
    wr8(ABS(0x0295), 0x2D);  /* 004523 LDM */
    wr8(ABS(0x0258), 0x2D);  /* 004527 LDM */
    wr8(ABS(0x0259), 0x20);  /* 00452B LDM */
    goto L_004554;  /* 00452F JMP */
L_004532: TICK();
    mflag = 1;  /* 004532 SEM */
    /* 004533 CMP */
    if ((A & 0xFF) != 0x2D) goto L_004547;  /* 004535 BNE */
    Y = 0x0005;  /* 004537 LDY */
    mflag = 0;  /* 00453A CLM */
    S -= 3; F_003310();  /* 00453B JSRL */
    mflag = 1;  /* 00453F SEM */
    wr8(ABS(0x0295), 0x2A);  /* 004540 LDM */
    goto L_004554;  /* 004544 JMP */
L_004547: TICK();
    mflag = 1;  /* 004547 SEM */
    /* 004548 CMP */
    if ((A & 0xFF) != 0x2A) goto L_004554;  /* 00454A BNE */
    wr8(ABS(0x0258), 0x2B);  /* 00454C LDM */
    wr8(ABS(0x0295), 0x2B);  /* 004550 LDM */
L_004554: TICK();
    mflag = 0;  /* 004554 CLM */
    Y = pull16();  /* 004555 PLY */
    D = pull16();  /* 004556 PLD */
    S += 3; return;  /* 004557 RTL */
}

/* 004558  callers: 0047A3, 004851, 0048B9, 00665A */
void F_004558(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004558 LDA */
    push16(D);  /* 00455A PHD */
    D = A;  /* 00455B TAD */
    A += 1;  /* 00455C INC */
    A += 1;  /* 00455D INC */
    push16(A);  /* 00455E PHA */
    wr16(DP(0x00), 0x0000);  /* 00455F LDM */
L_004563: TICK();
    X = rd16(DP(0x00));  /* 004563 LDX */
    /* 004565 CPX */
    if (X >= 0x0005) goto L_00457D;  /* 004568 BCS */
    A = rd16((ABS(0x025A) + X));  /* 00456A LDA */
    mflag = 1;  /* 00456D SEM */
    SETLO(A, A + 1);  /* 00456E INC */
    if ((A & 0xFF) == 0) goto L_004577;  /* 00456F BEQ */
    mflag = 0;  /* 004571 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0004);  /* 004572 SEB */
L_004577: TICK();
    mflag = 0;  /* 004577 CLM */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 004578 INC */
    goto L_004563;  /* 00457A JMP */
L_00457D: TICK();
    Y = 0x000B;  /* 00457D LDY */
    mflag = 0;  /* 004580 CLM */
    S -= 3; F_0080AA();  /* 004581 JSRL */
    mflag = 1;  /* 004585 SEM */
    wr8(ABS(0x0231), 0x14);  /* 004586 LDM */
    wr8(ABS(0x0293), 0x00);  /* 00458A LDM */
    wr8(ABS(0x0230), 0x00);  /* 00458E LDM */
    mflag = 0;  /* 004592 CLM */
    Y = pull16();  /* 004593 PLY */
    D = pull16();  /* 004594 PLD */
    S += 3; return;  /* 004595 RTL */
}

/* 004596  callers: 00488E, 0048B1 */
void F_004596(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004596 LDA */
    push16(D);  /* 004598 PHD */
    D = A;  /* 004599 TAD */
    C = 0;  /* 00459A CLC */
    A = adc16(A, 0x0015);  /* 00459B ADC */
    push16(A);  /* 00459E PHA */
    wr16(DP(0x00), Y);  /* 00459F STY */
    A = D;  /* 0045A1 TDA */
    C = 0;  /* 0045A2 CLC */
    A = adc16(A, 0x000B);  /* 0045A3 ADC */
    wr16(DP(0x15), A);  /* 0045A6 STA */
    wr16(DP(0x17), 0x317E);  /* 0045A8 LDM */
    mflag = 1;  /* 0045AC SEM */
    wr8(DP(0x19), 0x00);  /* 0045AD LDM */
    mflag = 0;  /* 0045B0 CLM */
    A = rd16(DP(0x15));  /* 0045B1 LDA */
    wr16(DP(0x1A), A);  /* 0045B3 STA */
    mflag = 1;  /* 0045B5 SEM */
    wr8(DP(0x1C), 0x00);  /* 0045B6 LDM */
    mflag = 0;  /* 0045B9 CLM */
    A = 0x0017;  /* 0045BA LDA */
    Y = 0x000A;  /* 0045BD LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 0045C0 JSRL */
    mflag = 1;  /* 0045C4 SEM */
    wr8(DP(0x02), 0x01);  /* 0045C5 LDM */
    mflag = 0;  /* 0045C8 CLM */
    wr16(DP(0x09), 0x0000);  /* 0045C9 LDM */
L_0045CD: TICK();
    A = rd16(DP(0x09));  /* 0045CD LDA */
    A ^= 0x8000;  /* 0045CF EOR */
    /* 0045D2 CMP */
    if (A >= 0x8005) goto L_004611;  /* 0045D5 BCS */
    A = rd16(DP(0x09));  /* 0045D7 LDA */
    C = 1;  /* 0045D9 SEC */
    A = sbc16(A, rd16(DP(0x00)));  /* 0045DA SBC */
    if (!V) goto L_0045E1;  /* 0045DC BVC */
    A ^= 0x8000;  /* 0045DE EOR */
    N = ((A & 0x8000) != 0);
L_0045E1: TICK();
    if (!N) goto L_004600;  /* 0045E1 BPL */
    X = rd16(DP(0x09));  /* 0045E3 LDX */
    A = rd16((ABS(0x0283) + X));  /* 0045E5 LDA */
    C = 0;  /* 0045E8 CLC */
    mflag = 1;  /* 0045E9 SEM */
    SETLO(A, adc8(A, 0xD0));  /* 0045EA ADC */
    wr8(DP(0x15), (uint8_t)A);  /* 0045EC STA */
    mflag = 0;  /* 0045EE CLM */
    A = X;  /* 0045EF TXA */
    C = 1;  /* 0045F0 SEC */
    A = sbc16(A, rd16(DP(0x00)));  /* 0045F1 SBC */
    C = 0;  /* 0045F3 CLC */
    A = adc16(A, 0x0005);  /* 0045F4 ADC */
    X = A;  /* 0045F7 TAX */
    A = rd16(DP(0x15));  /* 0045F8 LDA */
    mflag = 1;  /* 0045FA SEM */
    wr8(DP(0x03 + X), (uint8_t)A);  /* 0045FB STA */
    goto L_00460B;  /* 0045FD JMP */
L_004600: TICK();
    mflag = 0;  /* 004600 CLM */
    A = rd16(DP(0x09));  /* 004601 LDA */
    C = 1;  /* 004603 SEC */
    A = sbc16(A, rd16(DP(0x00)));  /* 004604 SBC */
    X = A;  /* 004606 TAX */
    mflag = 1;  /* 004607 SEM */
    wr8(DP(0x03 + X), 0xFF);  /* 004608 LDM */
L_00460B: TICK();
    mflag = 0;  /* 00460B CLM */
    t = (rd16(DP(0x09)) + 1) & 0xFFFF; wr16(DP(0x09), t);  /* 00460C INC */
    goto L_0045CD;  /* 00460E JMP */
L_004611: TICK();
    mflag = 0;  /* 004611 CLM */
    X = rd16(DP(0x00));  /* 004612 LDX */
    /* 004614 CPX */
    if (X != 0x0005) goto L_00463C;  /* 004617 BNE */
    mflag = 1;  /* 004619 SEM */
    wr8(DP(0x02), 0x00);  /* 00461A LDM */
    mflag = 0;  /* 00461D CLM */
    wr16(DP(0x09), 0x0000);  /* 00461E LDM */
L_004622: TICK();
    A = rd16(DP(0x09));  /* 004622 LDA */
    A ^= 0x8000;  /* 004624 EOR */
    /* 004627 CMP */
    if (A >= 0x8005) goto L_00463C;  /* 00462A BCS */
    X = rd16(DP(0x09));  /* 00462C LDX */
    mflag = 1;  /* 00462E SEM */
    SETLO(A, rd8(DP(0x03 + X)));  /* 00462F LDA */
    if ((A & 0xFF) == 0) goto L_004636;  /* 004631 BEQ */
    wr8(DP(0x02), 0x01);  /* 004633 LDM */
L_004636: TICK();
    mflag = 0;  /* 004636 CLM */
    t = (rd16(DP(0x09)) + 1) & 0xFFFF; wr16(DP(0x09), t);  /* 004637 INC */
    goto L_004622;  /* 004639 JMP */
L_00463C: TICK();
    mflag = 0;  /* 00463C CLM */
    wr16(DP(0x09), 0x0000);  /* 00463D LDM */
L_004641: TICK();
    A = rd16(DP(0x09));  /* 004641 LDA */
    A ^= 0x8000;  /* 004643 EOR */
    /* 004646 CMP */
    if (A >= 0x8005) goto L_004665;  /* 004649 BCS */
    mflag = 1;  /* 00464B SEM */
    SETLO(A, rd8(DP(0x02)));  /* 00464C LDA */
    if ((A & 0xFF) == 0) goto L_004657;  /* 00464E BEQ */
    X = rd16(DP(0x09));  /* 004650 LDX */
    SETLO(A, rd8(DP(0x03 + X)));  /* 004652 LDA */
    goto L_004659;  /* 004654 JMP */
L_004657: TICK();
    SETLO(A, 0xFF);  /* 004657 LDA */
L_004659: TICK();
    X = rd16(DP(0x09));  /* 004659 LDX */
    wr8((ABS(0x025A) + X), (uint8_t)A);  /* 00465B STA */
    X += 1;  /* 00465E INX */
    wr16(DP(0x09), X);  /* 00465F STX */
    mflag = 0;  /* 004661 CLM */
    goto L_004641;  /* 004662 JMP */
L_004665: TICK();
    mflag = 0;  /* 004665 CLM */
    X = rd16(DP(0x00));  /* 004666 LDX */
    /* 004668 CPX */
    if (X == 0x0005) goto L_004675;  /* 00466B BEQ */
    mflag = 1;  /* 00466D SEM */
    wr8(ABS(0x01EB), 0x03);  /* 00466E LDM */
    goto L_0046CB;  /* 004672 JMP */
L_004675: TICK();
    mflag = 0;  /* 004675 CLM */
    wr16(DP(0x09), 0x0000);  /* 004676 LDM */
L_00467A: TICK();
    A = rd16(DP(0x09));  /* 00467A LDA */
    A ^= 0x8000;  /* 00467C EOR */
    /* 00467F CMP */
    if (A >= 0x8005) goto L_0046CB;  /* 004682 BCS */
    A = rd16(ABS(0x01EB));  /* 004684 LDA */
    mflag = 1;  /* 004687 SEM */
    /* 004688 CMP */
    if ((A & 0xFF) != 0x04) goto L_004695;  /* 00468A BNE */
    X = rd16(DP(0x09));  /* 00468C LDX */
    SETLO(A, rd8(DP(0x0B + X)));  /* 00468E LDA */
    wr8(DP(0x08), (uint8_t)A);  /* 004690 STA */
    goto L_0046B9;  /* 004692 JMP */
L_004695: TICK();
    mflag = 1;  /* 004695 SEM */
    /* 004696 CMP */
    if ((A & 0xFF) != 0x05) goto L_0046AA;  /* 004698 BNE */
    mflag = 0;  /* 00469A CLM */
    A = rd16(DP(0x09));  /* 00469B LDA */
    C = 0;  /* 00469D CLC */
    A = adc16(A, 0x0005);  /* 00469E ADC */
    X = A;  /* 0046A1 TAX */
    A = rd16(DP(0x0B + X));  /* 0046A2 LDA */
    mflag = 1;  /* 0046A4 SEM */
    wr8(DP(0x08), (uint8_t)A);  /* 0046A5 STA */
    goto L_0046B9;  /* 0046A7 JMP */
L_0046AA: TICK();
    mflag = 1;  /* 0046AA SEM */
    /* 0046AB CMP */
    if ((A & 0xFF) != 0x06) goto L_0046B5;  /* 0046AD BNE */
    wr8(DP(0x08), 0x00);  /* 0046AF LDM */
    goto L_0046B9;  /* 0046B2 JMP */
L_0046B5: TICK();
    mflag = 1;  /* 0046B5 SEM */
    wr8(DP(0x08), 0x0F);  /* 0046B6 LDM */
L_0046B9: TICK();
    X = rd16(DP(0x09));  /* 0046B9 LDX */
    SETLO(A, rd8(DP(0x03 + X)));  /* 0046BB LDA */
    t = rd8(DP(0x08));  /* 0046BD CMP */
    if ((A & 0xFF) == t) goto L_0046C5;  /* 0046BF BEQ */
    wr8(ABS(0x01EB), 0x03);  /* 0046C1 LDM */
L_0046C5: TICK();
    mflag = 0;  /* 0046C5 CLM */
    t = (rd16(DP(0x09)) + 1) & 0xFFFF; wr16(DP(0x09), t);  /* 0046C6 INC */
    goto L_00467A;  /* 0046C8 JMP */
L_0046CB: TICK();
    mflag = 0;  /* 0046CB CLM */
    A = rd16(ABS(0x01EB));  /* 0046CC LDA */
    mflag = 1;  /* 0046CF SEM */
    /* 0046D0 CMP */
    if ((A & 0xFF) != 0x04) goto L_0046DB;  /* 0046D2 BNE */
    wr8(ABS(0x01EB), 0x05);  /* 0046D4 LDM */
    goto L_004702;  /* 0046D8 JMP */
L_0046DB: TICK();
    mflag = 1;  /* 0046DB SEM */
    /* 0046DC CMP */
    if ((A & 0xFF) != 0x05) goto L_0046E7;  /* 0046DE BNE */
    wr8(ABS(0x01EB), 0x06);  /* 0046E0 LDM */
    goto L_004702;  /* 0046E4 JMP */
L_0046E7: TICK();
    mflag = 1;  /* 0046E7 SEM */
    /* 0046E8 CMP */
    if ((A & 0xFF) != 0x06) goto L_004702;  /* 0046EA BNE */
    mflag = 0;  /* 0046EC CLM */
    S -= 3; F_008D47();  /* 0046ED JSRL */
    /* 0046F1 CMP */
    if ((A & 0xFF) == 0x00) goto L_0046FA;  /* 0046F3 BEQ */
    SETLO(A, 0x01);  /* 0046F5 LDA */
    goto L_0046FC;  /* 0046F7 JMP */
L_0046FA: TICK();
    SETLO(A, 0x02);  /* 0046FA LDA */
L_0046FC: TICK();
    wr8(ABS(0x01EB), (uint8_t)A);  /* 0046FC STA */
    goto L_004702;  /* 0046FF JMP */
L_004702: TICK();
    mflag = 1;  /* 004702 SEM */
    wr8(ABS(0x0231), 0x14);  /* 004703 LDM */
    wr8(ABS(0x0293), 0x00);  /* 004707 LDM */
    wr8(ABS(0x0230), 0x00);  /* 00470B LDM */
    SETLO(A, rd8(DP(0x02)));  /* 00470F LDA */
    if ((A & 0xFF) == 0) goto L_004719;  /* 004711 BEQ */
    mflag = 0;  /* 004713 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0004);  /* 004714 SEB */
L_004719: TICK();
    mflag = 0;  /* 004719 CLM */
    S -= 3; F_008DED();  /* 00471A JSRL */
    Y = pull16();  /* 00471E PLY */
    D = pull16();  /* 00471F PLD */
    S += 3; return;  /* 004720 RTL */
}

/* 004721  callers: 004825, 004849, 006625, 006645 */
void F_004721(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004721 LDA */
    push16(D);  /* 004723 PHD */
    D = A;  /* 004724 TAD */
    C = 0;  /* 004725 CLC */
    A = adc16(A, 0x000D);  /* 004726 ADC */
    push16(A);  /* 004729 PHA */
    wr16(DP(0x00), Y);  /* 00472A STY */
    mflag = 1;  /* 00472C SEM */
    wr8(DP(0x02), 0x00);  /* 00472D LDM */
    mflag = 0;  /* 004730 CLM */
    wr16(DP(0x0B), 0x0000);  /* 004731 LDM */
    wr16(DP(0x09), 0x0000);  /* 004735 LDM */
L_004739: TICK();
    A = rd16(DP(0x0B));  /* 004739 LDA */
    A ^= 0x8000;  /* 00473B EOR */
    /* 00473E CMP */
    if (A >= 0x8005) goto L_004759;  /* 004741 BCS */
    X = rd16(DP(0x0B));  /* 004743 LDX */
    A = rd16((ABS(0x025A) + X));  /* 004745 LDA */
    mflag = 1;  /* 004748 SEM */
    wr8(DP(0x04 + X), (uint8_t)A);  /* 004749 STA */
    SETLO(A, rd8(DP(0x04 + X)));  /* 00474B LDA */
    SETLO(A, A + 1);  /* 00474D INC */
    if ((A & 0xFF) == 0) goto L_004753;  /* 00474E BEQ */
    mflag = 0;  /* 004750 CLM */
    t = (rd16(DP(0x09)) + 1) & 0xFFFF; wr16(DP(0x09), t);  /* 004751 INC */
L_004753: TICK();
    mflag = 0;  /* 004753 CLM */
    t = (rd16(DP(0x0B)) + 1) & 0xFFFF; wr16(DP(0x0B), t);  /* 004754 INC */
    goto L_004739;  /* 004756 JMP */
L_004759: TICK();
    mflag = 0;  /* 004759 CLM */
    X = rd16(DP(0x00));  /* 00475A LDX */
    t = rd16(DP(0x09));  /* 00475C CPX */
    if (X != t) goto L_00479D;  /* 00475E BNE */
    mflag = 1;  /* 004760 SEM */
    wr8(DP(0x02), 0x01);  /* 004761 LDM */
    mflag = 0;  /* 004764 CLM */
    wr16(DP(0x0B), 0x0000);  /* 004765 LDM */
L_004769: TICK();
    X = rd16(DP(0x0B));  /* 004769 LDX */
    A = rd16((ABS(0x0283) + X));  /* 00476B LDA */
    C = 0;  /* 00476E CLC */
    mflag = 1;  /* 00476F SEM */
    SETLO(A, adc8(A, 0xD0));  /* 004770 ADC */
    wr8(DP(0x03), (uint8_t)A);  /* 004772 STA */
    mflag = 0;  /* 004774 CLM */
    A = X;  /* 004775 TXA */
    C = 1;  /* 004776 SEC */
    A = sbc16(A, rd16(DP(0x09)));  /* 004777 SBC */
    C = 0;  /* 004779 CLC */
    A = adc16(A, 0x0005);  /* 00477A ADC */
    X = A;  /* 00477D TAX */
    A = rd16(DP(0x04 + X));  /* 00477E LDA */
    mflag = 1;  /* 004780 SEM */
    t = rd8(DP(0x03));  /* 004781 CMP */
    if ((A & 0xFF) == t) goto L_004788;  /* 004783 BEQ */
    wr8(DP(0x02), 0x00);  /* 004785 LDM */
L_004788: TICK();
    mflag = 0;  /* 004788 CLM */
    t = (rd16(DP(0x0B)) + 1) & 0xFFFF; wr16(DP(0x0B), t);  /* 004789 INC */
    A = rd16(DP(0x0B));  /* 00478B LDA */
    C = 1;  /* 00478D SEC */
    A = sbc16(A, rd16(DP(0x09)));  /* 00478E SBC */
    if (!V) goto L_004795;  /* 004790 BVC */
    A ^= 0x8000;  /* 004792 EOR */
    N = ((A & 0x8000) != 0);
L_004795: TICK();
    if (!N) goto L_00479D;  /* 004795 BPL */
    mflag = 1;  /* 004797 SEM */
    SETLO(A, rd8(DP(0x02)));  /* 004798 LDA */
    Z = ((A & 0xFF) == 0);
    mflag = 0;  /* 00479A CLM */
    if (!Z) goto L_004769;  /* 00479B BNE */
L_00479D: TICK();
    mflag = 1;  /* 00479D SEM */
    SETLO(A, rd8(DP(0x02)));  /* 00479E LDA */
    if ((A & 0xFF) != 0) goto L_0047AA;  /* 0047A0 BNE */
    mflag = 0;  /* 0047A2 CLM */
    S -= 3; F_004558();  /* 0047A3 JSRL */
    goto L_0047C0;  /* 0047A7 JMP */
L_0047AA: TICK();
    mflag = 0;  /* 0047AA CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0004);  /* 0047AB CLB */
    Y = 0x002B;  /* 0047B0 LDY */
    S -= 3; F_003310();  /* 0047B3 JSRL */
    mflag = 1;  /* 0047B7 SEM */
    wr8(ABS(0x0231), 0x14);  /* 0047B8 LDM */
    wr8(ABS(0x0293), 0x00);  /* 0047BC LDM */
L_0047C0: TICK();
    mflag = 0;  /* 0047C0 CLM */
    Y = pull16();  /* 0047C1 PLY */
    D = pull16();  /* 0047C2 PLD */
    S += 3; return;  /* 0047C3 RTL */
}

/* 0047C4  callers:  */
void F_0047C4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0047C4 LDA */
    push16(D);  /* 0047C6 PHD */
    D = A;  /* 0047C7 TAD */
    C = 0;  /* 0047C8 CLC */
    A = adc16(A, 0x0004);  /* 0047C9 ADC */
    push16(A);  /* 0047CC PHA */
    A = rd16(ABS(0x0293));  /* 0047CD LDA */
    A &= 0x00FF;  /* 0047D0 AND */
    wr16(DP(0x02), A);  /* 0047D3 STA */
    Y = 0x002B;  /* 0047D5 LDY */
    S -= 3; F_003310();  /* 0047D8 JSRL */
    wr16(DP(0x00), 0x0000);  /* 0047DC LDM */
L_0047E0: TICK();
    A = rd16(DP(0x00));  /* 0047E0 LDA */
    C = 1;  /* 0047E2 SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 0047E3 SBC */
    if (!V) goto L_0047EA;  /* 0047E5 BVC */
    A ^= 0x8000;  /* 0047E7 EOR */
    N = ((A & 0x8000) != 0);
L_0047EA: TICK();
    if (!N) goto L_004805;  /* 0047EA BPL */
    X = rd16(DP(0x00));  /* 0047EC LDX */
    A = X;  /* 0047EE TXA */
    C = 1;  /* 0047EF SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 0047F0 SBC */
    C = 0;  /* 0047F2 CLC */
    A = adc16(A, 0x0008);  /* 0047F3 ADC */
    Y = A;  /* 0047F6 TAY */
    A = rd16((ABS(0x0283) + X));  /* 0047F7 LDA */
    mflag = 1;  /* 0047FA SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 0047FB STA */
    X += 1;  /* 0047FE INX */
    wr16(DP(0x00), X);  /* 0047FF STX */
    mflag = 0;  /* 004801 CLM */
    goto L_0047E0;  /* 004802 JMP */
L_004805: TICK();
    mflag = 0;  /* 004805 CLM */
    if ((rd16(ABS(0x0240)) & 0x0004) == 0) goto L_004855;  /* 004806 BBC */
    A = rd16(ABS(0x0226));  /* 00480C LDA */
    mflag = 1;  /* 00480F SEM */
    /* 004810 CMP */
    if ((A & 0xFF) != 0x64) goto L_00482C;  /* 004812 BNE */
    wr8(ABS(0x0231), 0x15);  /* 004814 LDM */
    wr8(ABS(0x0230), 0x5E);  /* 004818 LDM */
    mflag = 0;  /* 00481C CLM */
    X = rd16(DP(0x02));  /* 00481D LDX */
    /* 00481F CPX */
    if (X != 0x0005) goto L_004829;  /* 004822 BNE */
    Y = X;  /* 004824 TXY */
    S -= 3; F_004721();  /* 004825 JSRL */
L_004829: TICK();
    goto L_004855;  /* 004829 JMP */
L_00482C: TICK();
    mflag = 0;  /* 00482C CLM */
    A = rd16(ABS(0x0226));  /* 00482D LDA */
    mflag = 1;  /* 004830 SEM */
    /* 004831 CMP */
    if ((A & 0xFF) != 0x73) goto L_004850;  /* 004833 BNE */
    mflag = 0;  /* 004835 CLM */
    X = rd16(DP(0x02));  /* 004836 LDX */
    if (X != 0) goto L_004846;  /* 004838 BNE */
    mflag = 1;  /* 00483A SEM */
    wr8(ABS(0x0231), 0x15);  /* 00483B LDM */
    wr8(ABS(0x0230), 0x5E);  /* 00483F LDM */
    goto L_00484D;  /* 004843 JMP */
L_004846: TICK();
    Y = rd16(DP(0x02));  /* 004846 LDY */
    mflag = 0;  /* 004848 CLM */
    S -= 3; F_004721();  /* 004849 JSRL */
L_00484D: TICK();
    goto L_004855;  /* 00484D JMP */
L_004850: TICK();
    mflag = 0;  /* 004850 CLM */
    S -= 3; F_004558();  /* 004851 JSRL */
L_004855: TICK();
    mflag = 0;  /* 004855 CLM */
    if ((rd16(ABS(0x0240)) & 0x0004) == 0x0004) goto L_0048BD;  /* 004856 BBS */
    mflag = 1;  /* 00485C SEM */
    if ((rd8(ABS(0x0340)) & 0x80) == 0) goto L_004867;  /* 00485D BBC */
    SETLO(A, 0x59);  /* 004862 LDA */
    goto L_004869;  /* 004864 JMP */
L_004867: TICK();
    SETLO(A, 0x4E);  /* 004867 LDA */
L_004869: TICK();
    wr8(ABS(0x0252), (uint8_t)A);  /* 004869 STA */
    SETLO(A, rd8(ABS(0x0226)));  /* 00486C LDA */
    /* 00486F CMP */
    if ((A & 0xFF) == 0x64) goto L_00487A;  /* 004871 BEQ */
    SETLO(A, rd8(ABS(0x0231)));  /* 004873 LDA */
    /* 004876 CMP */
    if ((A & 0xFF) != 0x14) goto L_004895;  /* 004878 BNE */
L_00487A: TICK();
    wr8(ABS(0x0231), 0x15);  /* 00487A LDM */
    wr8(ABS(0x0230), 0x5E);  /* 00487E LDM */
    SETLO(A, rd8(ABS(0x0293)));  /* 004882 LDA */
    /* 004885 CMP */
    if ((A & 0xFF) != 0x05) goto L_004892;  /* 004887 BNE */
    mflag = 0;  /* 004889 CLM */
    A &= 0x00FF;  /* 00488A AND */
    Y = A;  /* 00488D TAY */
    S -= 3; F_004596();  /* 00488E JSRL */
L_004892: TICK();
    goto L_0048BD;  /* 004892 JMP */
L_004895: TICK();
    mflag = 0;  /* 004895 CLM */
    A = rd16(ABS(0x0226));  /* 004896 LDA */
    mflag = 1;  /* 004899 SEM */
    /* 00489A CMP */
    if ((A & 0xFF) != 0x73) goto L_0048B8;  /* 00489C BNE */
    SETLO(A, rd8(ABS(0x0293)));  /* 00489E LDA */
    if ((A & 0xFF) != 0) goto L_0048AE;  /* 0048A1 BNE */
    wr8(ABS(0x0231), 0x15);  /* 0048A3 LDM */
    wr8(ABS(0x0230), 0x5E);  /* 0048A7 LDM */
    goto L_0048B5;  /* 0048AB JMP */
L_0048AE: TICK();
    Y = rd16(DP(0x02));  /* 0048AE LDY */
    mflag = 0;  /* 0048B0 CLM */
    S -= 3; F_004596();  /* 0048B1 JSRL */
L_0048B5: TICK();
    goto L_0048BD;  /* 0048B5 JMP */
L_0048B8: TICK();
    mflag = 0;  /* 0048B8 CLM */
    S -= 3; F_004558();  /* 0048B9 JSRL */
L_0048BD: TICK();
    mflag = 0;  /* 0048BD CLM */
    Y = pull16();  /* 0048BE PLY */
    D = pull16();  /* 0048BF PLD */
    S += 3; return;  /* 0048C0 RTL */
}

/* 0048C1  callers: 004A87 */
void F_0048C1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0048C1 LDA */
    push16(D);  /* 0048C3 PHD */
    D = A;  /* 0048C4 TAD */
    C = 0;  /* 0048C5 CLC */
    A = adc16(A, 0x0011);  /* 0048C6 ADC */
    push16(A);  /* 0048C9 PHA */
    wr16(DP(0x11), 0x0370);  /* 0048CA LDM */
    wr16(DP(0x13), 0x0363);  /* 0048CE LDM */
    A = 0x0011;  /* 0048D2 LDA */
    Y = 0x000D;  /* 0048D5 LDY */
    S -= 3; rt_memcpy_01E466();  /* 0048D8 JSRL */
    wr16(DP(0x11), 0x0370);  /* 0048DC LDM */
    wr16(DP(0x13), 0x0356);  /* 0048E0 LDM */
    A = 0x0011;  /* 0048E4 LDA */
    Y = 0x000D;  /* 0048E7 LDY */
    S -= 3; rt_memcpy_01E466();  /* 0048EA JSRL */
    wr16(DP(0x11), 0x000F);  /* 0048EE LDM */
    A = D;  /* 0048F2 TDA */
    A += 1;  /* 0048F3 INC */
    A += 1;  /* 0048F4 INC */
    B = 0x0000;  /* 0048F5 LDA */
    wr16(DP(0x12), A);  /* 0048F9 STA */
    wr16(DP(0x14), B);  /* 0048FB STA */
    wr16(DP(0x15), 0x0537);  /* 0048FE LDM */
    Y = 0x00A0;  /* 004902 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 004905 JSRL */
    A = rd16(DP(0x0F));  /* 004909 LDA */
    mflag = 1;  /* 00490B SEM */
    wr8(ABS(0x038A), (uint8_t)A);  /* 00490C STA */
    SETLO(A, rd8(DP(0x10)));  /* 00490F LDA */
    wr8(ABS(0x0391), (uint8_t)A);  /* 004911 STA */
    wr8(DP(0x00), 0x00);  /* 004914 LDM */
L_004917: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 004917 LDA */
    /* 004919 CMP */
    if ((A & 0xFF) >= 0x0D) goto L_004933;  /* 00491B BCS */
    mflag = 0;  /* 00491D CLM */
    A &= 0x00FF;  /* 00491E AND */
    X = A;  /* 004921 TAX */
    A = rd16(DP(0x00));  /* 004922 LDA */
    A &= 0x00FF;  /* 004924 AND */
    Y = A;  /* 004927 TAY */
    A = rd16(DP(0x02 + X));  /* 004928 LDA */
    mflag = 1;  /* 00492A SEM */
    wr8((ABS(0x02D2) + Y), (uint8_t)A);  /* 00492B STA */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00492E INC */
    goto L_004917;  /* 004930 JMP */
L_004933: TICK();
    mflag = 1;  /* 004933 SEM */
    wr8(DP(0x01), 0x00);  /* 004934 LDM */
    wr8(DP(0x00), 0x00);  /* 004937 LDM */
L_00493A: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00493A LDA */
    mflag = 0;  /* 00493C CLM */
    A &= 0x00FF;  /* 00493D AND */
    X = A;  /* 004940 TAX */
    mflag = 1;  /* 004941 SEM */
    SETLO(A, rd8(DP(0x02 + X)));  /* 004942 LDA */
    if ((A & 0xFF) == 0) goto L_004949;  /* 004944 BEQ */
    wr8(DP(0x01), 0x01);  /* 004946 LDM */
L_004949: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 004949 INC */
    SETLO(A, rd8(DP(0x00)));  /* 00494B LDA */
    /* 00494D CMP */
    if ((A & 0xFF) >= 0x0D) goto L_004955;  /* 00494F BCS */
    SETLO(A, rd8(DP(0x01)));  /* 004951 LDA */
    if ((A & 0xFF) == 0) goto L_00493A;  /* 004953 BEQ */
L_004955: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 004955 LDA */
    if ((A & 0xFF) != 0) goto L_00496F;  /* 004957 BNE */
    wr8(ABS(0x0230), 0x9D);  /* 004959 LDM */
    Y = 0x000A;  /* 00495D LDY */
    mflag = 0;  /* 004960 CLM */
    S -= 3; F_0080AA();  /* 004961 JSRL */
    Y = 0x0002;  /* 004965 LDY */
    S -= 3; F_003310();  /* 004968 JSRL */
    goto L_004A0F;  /* 00496C JMP */
L_00496F: TICK();
    mflag = 1;  /* 00496F SEM */
    wr8(ABS(0x0228), 0x01);  /* 004970 LDM */
    mflag = 0;  /* 004974 CLM */
    S -= 3; F_0091EA();  /* 004975 JSRL */
    X = A;  /* 004979 TAX */
    if (X == 0) goto L_004984;  /* 00497A BEQ */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0010);  /* 00497C SEB */
    goto L_00498A;  /* 004981 JMP */
L_004984: TICK();
    mflag = 0;  /* 004984 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0010);  /* 004985 CLB */
L_00498A: TICK();
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0400);  /* 00498A SEB */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0080);  /* 00498F CLB */
    Y = rd16(ABS(0x038A));  /* 004994 LDY */
    S -= 3; F_00899E();  /* 004997 JSRL */
    wr16(DP(0x11), 0x0392);  /* 00499B LDM */
    wr16(DP(0x13), 0x0384);  /* 00499F LDM */
    A = 0x0011;  /* 0049A3 LDA */
    Y = 0x0007;  /* 0049A6 LDY */
    S -= 3; rt_memcpy_01E466();  /* 0049A9 JSRL */
    mflag = 1;  /* 0049AD SEM */
    wr8(ABS(0x0389), rd8(ABS(0x0389)) & ~0x3F);  /* 0049AE CLB */
    Y = rd16(ABS(0x0391));  /* 0049B2 LDY */
    mflag = 0;  /* 0049B5 CLM */
    S -= 3; F_00899E();  /* 0049B6 JSRL */
    wr16(DP(0x11), 0x0392);  /* 0049BA LDM */
    wr16(DP(0x13), 0x038B);  /* 0049BE LDM */
    A = 0x0011;  /* 0049C2 LDA */
    Y = 0x0007;  /* 0049C5 LDY */
    S -= 3; rt_memcpy_01E466();  /* 0049C8 JSRL */
    mflag = 1;  /* 0049CC SEM */
    wr8(ABS(0x0390), rd8(ABS(0x0390)) & ~0x3F);  /* 0049CD CLB */
    Y = 0x0000;  /* 0049D1 LDY */
    mflag = 0;  /* 0049D4 CLM */
    S -= 3; F_008C05();  /* 0049D5 JSRL */
    wr16(DP(0x11), 0x037D);  /* 0049D9 LDM */
    wr16(DP(0x13), 0x0370);  /* 0049DD LDM */
    A = 0x0011;  /* 0049E1 LDA */
    Y = 0x0007;  /* 0049E4 LDY */
    S -= 3; rt_memcpy_01E466();  /* 0049E7 JSRL */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_004A00;  /* 0049EB BBC */
    A = rd16(ABS(0x0365));  /* 0049F1 LDA */
    mflag = 1;  /* 0049F4 SEM */
    SETLO(A, A + 1);  /* 0049F5 INC */
    if ((A & 0xFF) == 0) goto L_004A00;  /* 0049F6 BEQ */
    mflag = 0;  /* 0049F8 CLM */
    S -= 3; F_0095FA();  /* 0049F9 JSRL */
    goto L_004A0F;  /* 0049FD JMP */
L_004A00: TICK();
    Y = 0x0001;  /* 004A00 LDY */
    mflag = 0;  /* 004A03 CLM */
    S -= 3; F_003310();  /* 004A04 JSRL */
    Y = 0x0004;  /* 004A08 LDY */
    S -= 3; F_00927F();  /* 004A0B JSRL */
L_004A0F: TICK();
    Y = pull16();  /* 004A0F PLY */
    D = pull16();  /* 004A10 PLD */
    S += 3; return;  /* 004A11 RTL */
}

/* 004A12  callers: 004A8F */
void F_004A12(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004A12 LDA */
    push16(D);  /* 004A14 PHD */
    D = A;  /* 004A15 TAD */
    push16(A);  /* 004A16 PHA */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0400);  /* 004A17 CLB */
    A = rd16(ABS(0x035C));  /* 004A1C LDA */
    mflag = 1;  /* 004A1F SEM */
    t = rd8(ABS(0x0369));  /* 004A20 CMP */
    if ((A & 0xFF) == t) goto L_004A2D;  /* 004A23 BEQ */
    Y = rd16(ABS(0x035C));  /* 004A25 LDY */
    mflag = 0;  /* 004A28 CLM */
    S -= 3; F_008A47();  /* 004A29 JSRL */
L_004A2D: TICK();
    mflag = 0;  /* 004A2D CLM */
    wr16(DP(0x00), 0x0356);  /* 004A2E LDM */
    wr16(DP(0x02), 0x0370);  /* 004A32 LDM */
    A = 0x0000;  /* 004A36 LDA */
    Y = 0x000D;  /* 004A39 LDY */
    S -= 3; rt_memcpy_01E466();  /* 004A3C JSRL */
    wr16(DP(0x00), 0x0356);  /* 004A40 LDM */
    wr16(DP(0x02), 0x0363);  /* 004A44 LDM */
    A = 0x0000;  /* 004A48 LDA */
    Y = 0x000D;  /* 004A4B LDY */
    S -= 3; rt_memcpy_01E466();  /* 004A4E JSRL */
    Y = 0x0004;  /* 004A52 LDY */
    S -= 3; F_00927F();  /* 004A55 JSRL */
    Y = pull16();  /* 004A59 PLY */
    D = pull16();  /* 004A5A PLD */
    S += 3; return;  /* 004A5B RTL */
}

/* 004A5C  callers: 004ACA, 00FD79, 01E000 */
void F_004A5C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004A5C LDA */
    push16(A);  /* 004A5E PHA */
    mflag = 1;  /* 004A5F SEM */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x80);  /* 004A60 CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x04);  /* 004A64 CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x08);  /* 004A68 CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x02);  /* 004A6C CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x01);  /* 004A70 CLB */
    wr8(ABS(0x020A), 0x00);  /* 004A74 LDM */
    wr8(ABS(0x0209), 0x00);  /* 004A78 LDM */
    wr8(ABS(0x0207), 0x00);  /* 004A7C LDM */
    mflag = 0;  /* 004A80 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_004A8E;  /* 004A81 BBS */
    S -= 3; F_0048C1();  /* 004A87 JSRL */
    goto L_004A93;  /* 004A8B JMP */
L_004A8E: TICK();
    mflag = 0;  /* 004A8E CLM */
    S -= 3; F_004A12();  /* 004A8F JSRL */
L_004A93: TICK();
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 004A93 CLB */
    Y = pull16();  /* 004A98 PLY */
    S += 3; return;  /* 004A99 RTL */
}

/* 004A9A  callers: 003A5A, 003AAC, 003AC0 */
void F_004A9A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004A9A LDA */
    push16(A);  /* 004A9C PHA */
    mflag = 1;  /* 004A9D SEM */
    wr8(ABS(0x0230), 0x00);  /* 004A9E LDM */
    mflag = 0;  /* 004AA2 CLM */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_004AD3;  /* 004AA3 BBS */
    A = rd16(ABS(0x0226));  /* 004AA9 LDA */
    mflag = 1;  /* 004AAC SEM */
    /* 004AAD CMP */
    if ((A & 0xFF) == 0x73) goto L_004AC2;  /* 004AAF BEQ */
    SETLO(A, rd8(ABS(0x0226)));  /* 004AB1 LDA */
    /* 004AB4 CMP */
    if ((A & 0xFF) == 0x70) goto L_004ABD;  /* 004AB6 BEQ */
    if ((rd8(ABS(0x033F)) & 0x08) == 0x08) goto L_004AD3;  /* 004AB8 BBS */
L_004ABD: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_004AD3;  /* 004ABD BBS */
L_004AC2: TICK();
    mflag = 0;  /* 004AC2 CLM */
    S -= 3; F_01E142();  /* 004AC3 JSRL */
    X = A;  /* 004AC7 TAX */
    if (X != 0) goto L_004ACE;  /* 004AC8 BNE */
    S -= 3; F_004A5C();  /* 004ACA JSRL */
L_004ACE: TICK();
    mflag = 1;  /* 004ACE SEM */
    wr8(ABS(0x0231), 0x14);  /* 004ACF LDM */
L_004AD3: TICK();
    mflag = 0;  /* 004AD3 CLM */
    Y = pull16();  /* 004AD4 PLY */
    S += 3; return;  /* 004AD5 RTL */
}

/* 004AD6  callers:  */
void F_004AD6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004AD6 LDA */
    push16(A);  /* 004AD8 PHA */
    A = rd16(ABS(0x0226));  /* 004AD9 LDA */
    mflag = 1;  /* 004ADC SEM */
    /* 004ADD CMP */
    if ((A & 0xFF) != 0x73) goto L_004B43;  /* 004ADF BNE */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_004AEB;  /* 004AE1 BBS */
    if ((rd8(ABS(0x0334)) & 0x20) == 0) goto L_004B43;  /* 004AE6 BBC */
L_004AEB: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_004B43;  /* 004AEB BBC */
    SETLO(A, rd8(ABS(0x0293)));  /* 004AF0 LDA */
    if ((A & 0xFF) == 0) goto L_004B27;  /* 004AF3 BEQ */
    mflag = 0;  /* 004AF5 CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0x0100) goto L_004B0D;  /* 004AF6 BBS */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0100);  /* 004AFC SEB */
    S -= 3; F_0099AD();  /* 004B01 JSRL */
    mflag = 1;  /* 004B05 SEM */
    wr8(ABS(0x0259), 0x2A);  /* 004B06 LDM */
    goto L_004B1F;  /* 004B0A JMP */
L_004B0D: TICK();
    mflag = 0;  /* 004B0D CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0100);  /* 004B0E CLB */
    Y = 0x0019;  /* 004B13 LDY */
    S -= 3; F_019884();  /* 004B16 JSRL */
    mflag = 1;  /* 004B1A SEM */
    wr8(ABS(0x0259), 0x2D);  /* 004B1B LDM */
L_004B1F: TICK();
    mflag = 0;  /* 004B1F CLM */
    S -= 3; F_008DED();  /* 004B20 JSRL */
    goto L_004B40;  /* 004B24 JMP */
L_004B27: TICK();
    mflag = 1;  /* 004B27 SEM */
    wr8(ABS(0x0293), 0x01);  /* 004B28 LDM */
    mflag = 0;  /* 004B2C CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0) goto L_004B39;  /* 004B2D BBC */
    A = 0x002A;  /* 004B33 LDA */
    goto L_004B3C;  /* 004B36 JMP */
L_004B39: TICK();
    A = 0x002D;  /* 004B39 LDA */
L_004B3C: TICK();
    mflag = 1;  /* 004B3C SEM */
    wr8(ABS(0x0259), (uint8_t)A);  /* 004B3D STA */
L_004B40: TICK();
    goto L_004B48;  /* 004B40 JMP */
L_004B43: TICK();
    mflag = 1;  /* 004B43 SEM */
    wr8(ABS(0x0293), 0x00);  /* 004B44 LDM */
L_004B48: TICK();
    mflag = 0;  /* 004B48 CLM */
    Y = pull16();  /* 004B49 PLY */
    S += 3; return;  /* 004B4A RTL */
}

/* 004B4B  callers:  */
void F_004B4B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004B4B LDA */
    push16(D);  /* 004B4D PHD */
    D = A;  /* 004B4E TAD */
    A += 1;  /* 004B4F INC */
    push16(A);  /* 004B50 PHA */
    A = rd16(ABS(0x0226));  /* 004B51 LDA */
    mflag = 1;  /* 004B54 SEM */
    /* 004B55 CMP */
    if ((A & 0xFF) != 0x64) goto L_004BAC;  /* 004B57 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 004B59 LDA */
    /* 004B5C CMP */
    if ((A & 0xFF) != 0x31) goto L_004B6E;  /* 004B5E BNE */
    mflag = 0;  /* 004B60 CLM */
    A = rd16(ABS(0x044A));  /* 004B61 LDA */
    C = 0;  /* 004B64 CLC */
    A = adc16(A, 0x000A);  /* 004B65 ADC */
    wr16(ABS(0x044A), A);  /* 004B68 STA */
    goto L_004BA9;  /* 004B6B JMP */
L_004B6E: TICK();
    mflag = 0;  /* 004B6E CLM */
    A = rd16(ABS(0x0283));  /* 004B6F LDA */
    mflag = 1;  /* 004B72 SEM */
    /* 004B73 CMP */
    if ((A & 0xFF) != 0x32) goto L_004B85;  /* 004B75 BNE */
    mflag = 0;  /* 004B77 CLM */
    A = rd16(ABS(0x044A));  /* 004B78 LDA */
    C = 0;  /* 004B7B CLC */
    A = adc16(A, 0x0064);  /* 004B7C ADC */
    wr16(ABS(0x044A), A);  /* 004B7F STA */
    goto L_004BA9;  /* 004B82 JMP */
L_004B85: TICK();
    mflag = 0;  /* 004B85 CLM */
    A = rd16(ABS(0x0283));  /* 004B86 LDA */
    mflag = 1;  /* 004B89 SEM */
    /* 004B8A CMP */
    if ((A & 0xFF) != 0x33) goto L_004B9C;  /* 004B8C BNE */
    mflag = 0;  /* 004B8E CLM */
    A = rd16(ABS(0x044A));  /* 004B8F LDA */
    C = 0;  /* 004B92 CLC */
    A = adc16(A, 0x03E8);  /* 004B93 ADC */
    wr16(ABS(0x044A), A);  /* 004B96 STA */
    goto L_004BA9;  /* 004B99 JMP */
L_004B9C: TICK();
    mflag = 0;  /* 004B9C CLM */
    A = rd16(ABS(0x0283));  /* 004B9D LDA */
    mflag = 1;  /* 004BA0 SEM */
    /* 004BA1 CMP */
    if ((A & 0xFF) != 0x34) goto L_004BA9;  /* 004BA3 BNE */
    mflag = 0;  /* 004BA5 CLM */
    t = (rd16(ABS(0x044A)) + 1) & 0xFFFF; wr16(ABS(0x044A), t);  /* 004BA6 INC */
L_004BA9: TICK();
    goto L_004BBB;  /* 004BA9 JMP */
L_004BAC: TICK();
    mflag = 0;  /* 004BAC CLM */
    A = rd16(ABS(0x0226));  /* 004BAD LDA */
    mflag = 1;  /* 004BB0 SEM */
    /* 004BB1 CMP */
    if ((A & 0xFF) != 0x73) goto L_004BBB;  /* 004BB3 BNE */
    mflag = 0;  /* 004BB5 CLM */
    wr16(ABS(0x044A), 0x0000);  /* 004BB6 LDM */
L_004BBB: TICK();
    mflag = 0;  /* 004BBB CLM */
    X = rd16(ABS(0x044A));  /* 004BBC LDX */
    /* 004BBF CPX */
    if (X < 0x0800) goto L_004BC9;  /* 004BC2 BCC */
    wr16(ABS(0x044A), 0x0000);  /* 004BC4 LDM */
L_004BC9: TICK();
    Y = rd16(ABS(0x044A));  /* 004BC9 LDY */
    S -= 3; eeprom_read_byte_007189();  /* 004BCC JSRL */
    SETLO(B, 0x00);  /* 004BD0 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 004BD3 STA */
    div8(0x10);  /* 004BD5 DIV */
    /* 004BD8 CMP */
    if ((A & 0xFF) >= 0x0A) goto L_004BED;  /* 004BDA BCS */
    SETLO(B, 0x00);  /* 004BDC LDA */
    SETLO(A, rd8(DP(0x00)));  /* 004BDF LDA */
    div8(0x10);  /* 004BE1 DIV */
    C = 0;  /* 004BE4 CLC */
    SETLO(A, adc8(A, 0x30));  /* 004BE5 ADC */
    wr8(ABS(0x0258), (uint8_t)A);  /* 004BE7 STA */
    goto L_004BFC;  /* 004BEA JMP */
L_004BED: TICK();
    mflag = 1;  /* 004BED SEM */
    SETLO(B, 0x00);  /* 004BEE LDA */
    SETLO(A, rd8(DP(0x00)));  /* 004BF1 LDA */
    div8(0x10);  /* 004BF3 DIV */
    C = 0;  /* 004BF6 CLC */
    SETLO(A, adc8(A, 0x37));  /* 004BF7 ADC */
    wr8(ABS(0x0258), (uint8_t)A);  /* 004BF9 STA */
L_004BFC: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 004BFC LDA */
    SETLO(A, A & 0x0F);  /* 004BFE AND */
    C = ((A & 0xFF) >= 0x0A);  /* 004C00 CMP */
    if (C) goto L_004C10;  /* 004C02 BCS */
    SETLO(A, rd8(DP(0x00)));  /* 004C04 LDA */
    SETLO(A, A & 0x0F);  /* 004C06 AND */
    SETLO(A, adc8(A, 0x30));  /* 004C08 ADC */
    wr8(ABS(0x0259), (uint8_t)A);  /* 004C0A STA */
    goto L_004C1C;  /* 004C0D JMP */
L_004C10: TICK();
    mflag = 0;  /* 004C10 CLM */
    A = rd16(DP(0x00));  /* 004C11 LDA */
    mflag = 1;  /* 004C13 SEM */
    SETLO(A, A & 0x0F);  /* 004C14 AND */
    C = 0;  /* 004C16 CLC */
    SETLO(A, adc8(A, 0x37));  /* 004C17 ADC */
    wr8(ABS(0x0259), (uint8_t)A);  /* 004C19 STA */
L_004C1C: TICK();
    mflag = 0;  /* 004C1C CLM */
    B = 0x0000;  /* 004C1D LDA */
    A = rd16(ABS(0x044A));  /* 004C21 LDA */
    div16(0x0064);  /* 004C24 DIV */
    wr16(DP(0x01), A);  /* 004C28 STA */
    Y = 0x0001;  /* 004C2A LDY */
    S -= 3; F_0032CE();  /* 004C2D JSRL */
    B = 0x0000;  /* 004C31 LDA */
    A = rd16(ABS(0x044A));  /* 004C35 LDA */
    div16(0x0064);  /* 004C38 DIV */
    push16(B);  /* 004C3C PHB */
    A = pull16();  /* 004C3E PLA */
    wr16(DP(0x01), A);  /* 004C3F STA */
    Y = 0x0003;  /* 004C41 LDY */
    S -= 3; F_0032CE();  /* 004C44 JSRL */
    mflag = 1;  /* 004C48 SEM */
    wr8(ABS(0x0252), 0x45);  /* 004C49 LDM */
    wr8(ABS(0x0257), 0x2D);  /* 004C4D LDM */
    wr8(ABS(0x0293), 0x00);  /* 004C51 LDM */
    mflag = 0;  /* 004C55 CLM */
    Y = pull16();  /* 004C56 PLY */
    D = pull16();  /* 004C57 PLD */
    S += 3; return;  /* 004C58 RTL */
}

/* 004C59  callers: 004D39 */
void F_004C59(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004C59 LDA */
    push16(D);  /* 004C5B PHD */
    D = A;  /* 004C5C TAD */
    C = 0;  /* 004C5D CLC */
    A = adc16(A, 0x0006);  /* 004C5E ADC */
    push16(A);  /* 004C61 PHA */
    wr16(DP(0x00), 0x0010);  /* 004C62 LDM */
    wr16(DP(0x02), 0x0058);  /* 004C66 LDM */
    wr16(DP(0x04), 0x0060);  /* 004C6A LDM */
    Y = 0x0003;  /* 004C6E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 004C71 JSRL */
    mflag = 0;  /* 004C75 CLM */
    wr16(DP(0x00), 0x0000);  /* 004C76 LDM */
    wr16(DP(0x02), 0x0040);  /* 004C7A LDM */
    Y = 0x0002;  /* 004C7E LDY */
    A = D;  /* 004C81 TDA */
    C = 0;  /* 004C82 CLC */
    A = adc16(A, 0x0004);  /* 004C83 ADC */
    push16(A);  /* 004C86 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 004C87 JSRL */
    Y = pull16();  /* 004C8B PLY */
    mflag = 0;  /* 004C8C CLM */
    A = D;  /* 004C8D TDA */
    push16(A);  /* 004C8E PHA */
    S -= 3; F_00EA66();  /* 004C8F JSRL */
    Y = pull16();  /* 004C93 PLY */
    mflag = 1;  /* 004C94 SEM */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x08);  /* 004C95 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x80);  /* 004C99 CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x04);  /* 004C9D CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x02);  /* 004CA1 CLB */
    wr8(ABS(0x0245), rd8(ABS(0x0245)) & ~0x01);  /* 004CA5 CLB */
    wr8(ABS(0x0241), rd8(ABS(0x0241)) & ~0x04);  /* 004CA9 CLB */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x20);  /* 004CAD CLB */
    wr8(ABS(0x0240), rd8(ABS(0x0240)) & ~0x80);  /* 004CB1 CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x20);  /* 004CB5 CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x04);  /* 004CB9 CLB */
    wr8(ABS(0x023B), rd8(ABS(0x023B)) & ~0x80);  /* 004CBD CLB */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x08);  /* 004CC1 CLB */
    wr8(ABS(0x023B), rd8(ABS(0x023B)) & ~0x02);  /* 004CC5 CLB */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x01);  /* 004CC9 CLB */
    wr8(ABS(0x023C), rd8(ABS(0x023C)) & ~0x01);  /* 004CCD CLB */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_004CDC;  /* 004CD1 BBS */
    mflag = 0;  /* 004CD6 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0400);  /* 004CD7 CLB */
L_004CDC: TICK();
    mflag = 1;  /* 004CDC SEM */
    wr8(ABS(0x020A), 0x00);  /* 004CDD LDM */
    wr8(ABS(0x0209), 0x00);  /* 004CE1 LDM */
    wr8(ABS(0x0207), 0x00);  /* 004CE5 LDM */
    wr8(ABS(0x01E9), 0x00);  /* 004CE9 LDM */
    mflag = 0;  /* 004CED CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0002);  /* 004CEE SEB */
    A = D;  /* 004CF3 TDA */
    push16(A);  /* 004CF4 PHA */
    S -= 3; F_008DED();  /* 004CF5 JSRL */
    Y = pull16();  /* 004CF9 PLY */
    mflag = 1;  /* 004CFA SEM */
    wr8(ABS(0x03BA), 0x00);  /* 004CFB LDM */
    mflag = 0;  /* 004CFF CLM */
    Y = pull16();  /* 004D00 PLY */
    D = pull16();  /* 004D01 PLD */
    S += 3; return;  /* 004D02 RTL */
}

/* 004D03  callers: 005AF6, 0067D0 */
void F_004D03(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004D03 LDA */
    push16(D);  /* 004D05 PHD */
    D = A;  /* 004D06 TAD */
    A += 1;  /* 004D07 INC */
    push16(A);  /* 004D08 PHA */
    A = rd16(ABS(0x0206));  /* 004D09 LDA */
    mflag = 1;  /* 004D0C SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 004D0D STA */
    mflag = 0;  /* 004D0F CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x2000);  /* 004D10 CLB */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0002);  /* 004D15 CLB */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_004D30;  /* 004D1A BBC */
    A = rd16(DP(0x00));  /* 004D20 LDA */
    mflag = 1;  /* 004D22 SEM */
    wr8(ABS(0x0369), (uint8_t)A);  /* 004D23 STA */
    wr8(ABS(0x0376), (uint8_t)A);  /* 004D26 STA */
    wr8(ABS(0x024A), 0xFF);  /* 004D29 LDM */
    goto L_004DAD;  /* 004D2D JMP */
L_004D30: TICK();
    mflag = 0;  /* 004D30 CLM */
    A = rd16(DP(0x00));  /* 004D31 LDA */
    mflag = 1;  /* 004D33 SEM */
    /* 004D34 CMP */
    if ((A & 0xFF) >= 0x0F) goto L_004D40;  /* 004D36 BCS */
    mflag = 0;  /* 004D38 CLM */
    S -= 3; F_004C59();  /* 004D39 JSRL */
    goto L_004DAD;  /* 004D3D JMP */
L_004D40: TICK();
    mflag = 1;  /* 004D40 SEM */
    if ((rd8(ABS(0x033F)) & 0x08) == 0) goto L_004D61;  /* 004D41 BBC */
    mflag = 0;  /* 004D46 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0400);  /* 004D47 SEB */
    S -= 3; F_009134();  /* 004D4C JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 004D50 STA */
    SETLO(A, rd8(ABS(0x0369)));  /* 004D52 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 004D55 STA */
    Y = rd16(DP(0x00));  /* 004D57 LDY */
    mflag = 0;  /* 004D59 CLM */
    S -= 3; F_003610();  /* 004D5A JSRL */
    goto L_004DA3;  /* 004D5E JMP */
L_004D61: TICK();
    mflag = 0;  /* 004D61 CLM */
    A = rd16(DP(0x00));  /* 004D62 LDA */
    C = 0;  /* 004D64 CLC */
    mflag = 1;  /* 004D65 SEM */
    SETLO(A, adc8(A, 0xF2));  /* 004D66 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 004D68 STA */
    mflag = 0;  /* 004D6A CLM */
    Y = A;  /* 004D6B TAY */
    S -= 3; channel_is_programmed_0087A7();  /* 004D6C JSRL */
    X = A;  /* 004D70 TAX */
    if (X != 0) goto L_004D83;  /* 004D71 BNE */
    Y = 0x0002;  /* 004D73 LDY */
    S -= 3; F_0080AA();  /* 004D76 JSRL */
    Y = rd16(DP(0x00));  /* 004D7A LDY */
    S -= 3; channel_record_copy_00350F();  /* 004D7C JSRL */
    goto L_004DA3;  /* 004D80 JMP */
L_004D83: TICK();
    Y = rd16(DP(0x00));  /* 004D83 LDY */
    mflag = 0;  /* 004D85 CLM */
    S -= 3; F_008D14();  /* 004D86 JSRL */
    wr16(DP(0x01), 0x0370);  /* 004D8A LDM */
    wr16(DP(0x03), 0x0363);  /* 004D8E LDM */
    A = 0x0001;  /* 004D92 LDA */
    Y = 0x000D;  /* 004D95 LDY */
    S -= 3; rt_memcpy_01E466();  /* 004D98 JSRL */
    Y = 0x0003;  /* 004D9C LDY */
    S -= 3; F_00927F();  /* 004D9F JSRL */
L_004DA3: TICK();
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 004DA3 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 004DA8 CLB */
L_004DAD: TICK();
    mflag = 0;  /* 004DAD CLM */
    if ((rd16(ABS(0x0246)) & 0x0002) == 0x0002) goto L_004DBB;  /* 004DAE BBS */
    Y = 0x002C;  /* 004DB4 LDY */
    S -= 3; F_003310();  /* 004DB7 JSRL */
L_004DBB: TICK();
    mflag = 1;  /* 004DBB SEM */
    wr8(ABS(0x0293), 0x00);  /* 004DBC LDM */
    wr8(ABS(0x0230), 0x00);  /* 004DC0 LDM */
    wr8(ABS(0x0231), 0x00);  /* 004DC4 LDM */
    mflag = 0;  /* 004DC8 CLM */
    Y = pull16();  /* 004DC9 PLY */
    D = pull16();  /* 004DCA PLD */
    S += 3; return;  /* 004DCB RTL */
}

/* 004DCC  callers: 004E61, 004E88, 004ED0, 004F18, 004F63 */
void F_004DCC(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004DCC LDA */
    push16(D);  /* 004DCE PHD */
    D = A;  /* 004DCF TAD */
    C = 0;  /* 004DD0 CLC */
    A = adc16(A, 0x0004);  /* 004DD1 ADC */
    push16(A);  /* 004DD4 PHA */
    Y = 0x03B6;  /* 004DD5 LDY */
    S -= 3; F_007D7F();  /* 004DD8 JSRL */
    wr16(DP(0x00), 0x0000);  /* 004DDC LDM */
    wr16(DP(0x02), 0x0000);  /* 004DE0 LDM */
L_004DE4: TICK();
    X = rd16(DP(0x02));  /* 004DE4 LDX */
    /* 004DE6 CPX */
    if (X >= 0x000A) goto L_004E0B;  /* 004DE9 BCS */
    Y = 0x0032;  /* 004DEB LDY */
    S -= 3; F_007D7F();  /* 004DEE JSRL */
    Y = 0x0005;  /* 004DF2 LDY */
    S -= 3; F_008DC6();  /* 004DF5 JSRL */
    mflag = 1;  /* 004DF9 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 004DFA LDA [AN5 RSSI] */
    mflag = 0;  /* 004DFD CLM */
    A &= 0x00FF;  /* 004DFE AND */
    C = 0;  /* 004E01 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 004E02 ADC */
    wr16(DP(0x00), A);  /* 004E04 STA */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 004E06 INC */
    goto L_004DE4;  /* 004E08 JMP */
L_004E0B: TICK();
    mflag = 0;  /* 004E0B CLM */
    B = 0x0000;  /* 004E0C LDA */
    A = rd16(DP(0x00));  /* 004E10 LDA */
    div16(0x000A);  /* 004E12 DIV */
    mflag = 1;  /* 004E16 SEM */
    Y = pull16();  /* 004E17 PLY */
    D = pull16();  /* 004E18 PLD */
    S += 3; return;  /* 004E19 RTL */
}

/* 004E1A  callers:  */
void F_004E1A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004E1A LDA */
    push16(D);  /* 004E1C PHD */
    D = A;  /* 004E1D TAD */
    C = 0;  /* 004E1E CLC */
    A = adc16(A, 0x000B);  /* 004E1F ADC */
    push16(A);  /* 004E22 PHA */
    A = rd16(ABS(0x0226));  /* 004E23 LDA */
    mflag = 1;  /* 004E26 SEM */
    /* 004E27 CMP */
    if ((A & 0xFF) == 0x73) goto L_004E3A;  /* 004E29 BEQ */
    wr8(ABS(0x0231), 0x14);  /* 004E2B LDM */
    wr8(ABS(0x0230), 0x00);  /* 004E2F LDM */
    wr8(ABS(0x01EB), 0x02);  /* 004E33 LDM */
    goto L_004FF2;  /* 004E37 JMP */
L_004E3A: TICK();
    mflag = 0;  /* 004E3A CLM */
    A = rd16(ABS(0x0231));  /* 004E3B LDA */
    mflag = 1;  /* 004E3E SEM */
    /* 004E3F CMP */
    if ((A & 0xFF) != 0x14) goto L_004E5B;  /* 004E41 BNE */
    wr8(ABS(0x0231), 0x19);  /* 004E43 LDM */
    wr8(ABS(0x0230), 0xFB);  /* 004E47 LDM */
    Y = 0x0026;  /* 004E4B LDY */
    mflag = 0;  /* 004E4E CLM */
    S -= 3; F_003310();  /* 004E4F JSRL */
    mflag = 1;  /* 004E53 SEM */
    wr8(ABS(0x01EB), 0x02);  /* 004E54 LDM */
    goto L_004FF2;  /* 004E58 JMP */
L_004E5B: TICK();
    mflag = 1;  /* 004E5B SEM */
    /* 004E5C CMP */
    if ((A & 0xFF) != 0x19) goto L_004E82;  /* 004E5E BNE */
    mflag = 0;  /* 004E60 CLM */
    S -= 3; F_004DCC();  /* 004E61 JSRL */
    mflag = 0;  /* 004E65 CLM */
    A &= 0x00FF;  /* 004E66 AND */
    wr16(DP(0x07), A);  /* 004E69 STA */
    mflag = 1;  /* 004E6B SEM */
    wr8(ABS(0x01EC), (uint8_t)A);  /* 004E6C STA */
    wr8(ABS(0x0231), 0x1A);  /* 004E6F LDM */
    wr8(ABS(0x0230), 0xFB);  /* 004E73 LDM */
    Y = 0x0027;  /* 004E77 LDY */
    mflag = 0;  /* 004E7A CLM */
    S -= 3; F_003310();  /* 004E7B JSRL */
    goto L_004FF2;  /* 004E7F JMP */
L_004E82: TICK();
    mflag = 1;  /* 004E82 SEM */
    /* 004E83 CMP */
    if ((A & 0xFF) != 0x1A) goto L_004ECA;  /* 004E85 BNE */
    mflag = 0;  /* 004E87 CLM */
    S -= 3; F_004DCC();  /* 004E88 JSRL */
    mflag = 0;  /* 004E8C CLM */
    A &= 0x00FF;  /* 004E8D AND */
    wr16(DP(0x07), A);  /* 004E90 STA */
    A = rd16(ABS(0x01EC));  /* 004E92 LDA */
    A &= 0x00FF;  /* 004E95 AND */
    t = rd16(DP(0x07));  /* 004E98 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_004EB3;  /* 004E9A BCC */
    if (Z) goto L_004EB3;  /* 004E9C BEQ */
    A = rd16(DP(0x07));  /* 004E9E LDA */
    mflag = 1;  /* 004EA0 SEM */
    wr8(ABS(0x01ED), (uint8_t)A);  /* 004EA1 STA */
    wr8(ABS(0x0231), 0x1B);  /* 004EA4 LDM */
    Y = 0x0028;  /* 004EA8 LDY */
    mflag = 0;  /* 004EAB CLM */
    S -= 3; F_003310();  /* 004EAC JSRL */
    goto L_004EC2;  /* 004EB0 JMP */
L_004EB3: TICK();
    Y = 0x0019;  /* 004EB3 LDY */
    mflag = 0;  /* 004EB6 CLM */
    S -= 3; F_0080AA();  /* 004EB7 JSRL */
    Y = 0x0027;  /* 004EBB LDY */
    S -= 3; F_003310();  /* 004EBE JSRL */
L_004EC2: TICK();
    mflag = 1;  /* 004EC2 SEM */
    wr8(ABS(0x0230), 0xFB);  /* 004EC3 LDM */
    goto L_004FF2;  /* 004EC7 JMP */
L_004ECA: TICK();
    mflag = 1;  /* 004ECA SEM */
    /* 004ECB CMP */
    if ((A & 0xFF) != 0x1B) goto L_004F12;  /* 004ECD BNE */
    mflag = 0;  /* 004ECF CLM */
    S -= 3; F_004DCC();  /* 004ED0 JSRL */
    mflag = 0;  /* 004ED4 CLM */
    A &= 0x00FF;  /* 004ED5 AND */
    wr16(DP(0x07), A);  /* 004ED8 STA */
    A = rd16(ABS(0x01ED));  /* 004EDA LDA */
    A &= 0x00FF;  /* 004EDD AND */
    t = rd16(DP(0x07));  /* 004EE0 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_004EFB;  /* 004EE2 BCC */
    if (Z) goto L_004EFB;  /* 004EE4 BEQ */
    A = rd16(DP(0x07));  /* 004EE6 LDA */
    mflag = 1;  /* 004EE8 SEM */
    wr8(ABS(0x01EE), (uint8_t)A);  /* 004EE9 STA */
    wr8(ABS(0x0231), 0x1C);  /* 004EEC LDM */
    Y = 0x0029;  /* 004EF0 LDY */
    mflag = 0;  /* 004EF3 CLM */
    S -= 3; F_003310();  /* 004EF4 JSRL */
    goto L_004F0A;  /* 004EF8 JMP */
L_004EFB: TICK();
    Y = 0x0019;  /* 004EFB LDY */
    mflag = 0;  /* 004EFE CLM */
    S -= 3; F_0080AA();  /* 004EFF JSRL */
    Y = 0x0028;  /* 004F03 LDY */
    S -= 3; F_003310();  /* 004F06 JSRL */
L_004F0A: TICK();
    mflag = 1;  /* 004F0A SEM */
    wr8(ABS(0x0230), 0xFB);  /* 004F0B LDM */
    goto L_004FF2;  /* 004F0F JMP */
L_004F12: TICK();
    mflag = 1;  /* 004F12 SEM */
    /* 004F13 CMP */
    if ((A & 0xFF) != 0x1C) goto L_004F5A;  /* 004F15 BNE */
    mflag = 0;  /* 004F17 CLM */
    S -= 3; F_004DCC();  /* 004F18 JSRL */
    mflag = 0;  /* 004F1C CLM */
    A &= 0x00FF;  /* 004F1D AND */
    wr16(DP(0x07), A);  /* 004F20 STA */
    A = rd16(ABS(0x01EE));  /* 004F22 LDA */
    A &= 0x00FF;  /* 004F25 AND */
    t = rd16(DP(0x07));  /* 004F28 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_004F43;  /* 004F2A BCC */
    if (Z) goto L_004F43;  /* 004F2C BEQ */
    A = rd16(DP(0x07));  /* 004F2E LDA */
    mflag = 1;  /* 004F30 SEM */
    wr8(ABS(0x01EF), (uint8_t)A);  /* 004F31 STA */
    wr8(ABS(0x0231), 0x1D);  /* 004F34 LDM */
    Y = 0x002A;  /* 004F38 LDY */
    mflag = 0;  /* 004F3B CLM */
    S -= 3; F_003310();  /* 004F3C JSRL */
    goto L_004F52;  /* 004F40 JMP */
L_004F43: TICK();
    Y = 0x0019;  /* 004F43 LDY */
    mflag = 0;  /* 004F46 CLM */
    S -= 3; F_0080AA();  /* 004F47 JSRL */
    Y = 0x0029;  /* 004F4B LDY */
    S -= 3; F_003310();  /* 004F4E JSRL */
L_004F52: TICK();
    mflag = 1;  /* 004F52 SEM */
    wr8(ABS(0x0230), 0xFB);  /* 004F53 LDM */
    goto L_004FF2;  /* 004F57 JMP */
L_004F5A: TICK();
    mflag = 1;  /* 004F5A SEM */
    /* 004F5B CMP */
    if ((A & 0xFF) == 0x1D) goto L_004F62;  /* 004F5D BEQ */
    goto L_004FF2;  /* 004F5F JMP */
L_004F62: TICK();
    mflag = 0;  /* 004F62 CLM */
    S -= 3; F_004DCC();  /* 004F63 JSRL */
    mflag = 0;  /* 004F67 CLM */
    A &= 0x00FF;  /* 004F68 AND */
    wr16(DP(0x07), A);  /* 004F6B STA */
    A = rd16(ABS(0x01EF));  /* 004F6D LDA */
    A &= 0x00FF;  /* 004F70 AND */
    t = rd16(DP(0x07));  /* 004F73 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_004FDB;  /* 004F75 BCC */
    if (Z) goto L_004FDB;  /* 004F77 BEQ */
    A = rd16(DP(0x07));  /* 004F79 LDA */
    mflag = 1;  /* 004F7B SEM */
    wr8(ABS(0x01F0), (uint8_t)A);  /* 004F7C STA */
    mflag = 0;  /* 004F7F CLM */
    wr16(DP(0x07), 0x0000);  /* 004F80 LDM */
    wr16(DP(0x09), 0x0000);  /* 004F84 LDM */
L_004F88: TICK();
    X = rd16(DP(0x09));  /* 004F88 LDX */
    C = (X >= 0x0005);  /* 004F8A CPX */
    if (C) goto L_004FA5;  /* 004F8D BCS */
    A = rd16((ABS(0x01EC) + X));  /* 004F8F LDA */
    mflag = 1;  /* 004F92 SEM */
    wr8(DP(0x00 + X), (uint8_t)A);  /* 004F93 STA */
    SETLO(A, rd8(DP(0x00 + X)));  /* 004F95 LDA */
    mflag = 0;  /* 004F97 CLM */
    A &= 0x00FF;  /* 004F98 AND */
    A = adc16(A, rd16(DP(0x07)));  /* 004F9B ADC */
    wr16(DP(0x07), A);  /* 004F9D STA */
    X += 1;  /* 004F9F INX */
    wr16(DP(0x09), X);  /* 004FA0 STX */
    goto L_004F88;  /* 004FA2 JMP */
L_004FA5: TICK();
    mflag = 0;  /* 004FA5 CLM */
    A = rd16(DP(0x08));  /* 004FA6 LDA */
    A &= 0x00FF;  /* 004FA8 AND */
    mflag = 1;  /* 004FAB SEM */
    wr8(DP(0x05), (uint8_t)A);  /* 004FAC STA */
    SETLO(A, rd8(DP(0x07)));  /* 004FAE LDA */
    wr8(DP(0x06), (uint8_t)A);  /* 004FB0 STA */
    wr8(DP(0x0B), 0x07);  /* 004FB2 LDM */
    mflag = 0;  /* 004FB5 CLM */
    A = D;  /* 004FB6 TDA */
    B = 0x0000;  /* 004FB7 LDA */
    wr16(DP(0x0C), A);  /* 004FBB STA */
    wr16(DP(0x0E), B);  /* 004FBD STA */
    wr16(DP(0x0F), 0x0571);  /* 004FC0 LDM */
    Y = 0x00A0;  /* 004FC4 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 004FC7 JSRL */
    mflag = 1;  /* 004FCB SEM */
    wr8(ABS(0x01EB), 0x01);  /* 004FCC LDM */
    wr8(ABS(0x0231), 0x14);  /* 004FD0 LDM */
    wr8(ABS(0x0230), 0x00);  /* 004FD4 LDM */
    goto L_004FEF;  /* 004FD8 JMP */
L_004FDB: TICK();
    Y = 0x0019;  /* 004FDB LDY */
    mflag = 0;  /* 004FDE CLM */
    S -= 3; F_0080AA();  /* 004FDF JSRL */
    Y = 0x002A;  /* 004FE3 LDY */
    S -= 3; F_003310();  /* 004FE6 JSRL */
    mflag = 1;  /* 004FEA SEM */
    wr8(ABS(0x0230), 0xFB);  /* 004FEB LDM */
L_004FEF: TICK();
    goto L_004FF2;  /* 004FEF JMP */
L_004FF2: TICK();
    mflag = 0;  /* 004FF2 CLM */
    Y = pull16();  /* 004FF3 PLY */
    D = pull16();  /* 004FF4 PLD */
    S += 3; return;  /* 004FF5 RTL */
}

/* 004FF6  callers: 0054AA, 0054DC, 00585E, 005882, 005A6E, 005B3F */
void F_004FF6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 004FF6 LDA */
    push16(D);  /* 004FF8 PHD */
    D = A;  /* 004FF9 TAD */
    A += 1;  /* 004FFA INC */
    A += 1;  /* 004FFB INC */
    A += 1;  /* 004FFC INC */
    push16(A);  /* 004FFD PHA */
    wr16(DP(0x00), Y);  /* 004FFE STY */
    mflag = 1;  /* 005000 SEM */
    wr8(DP(0x01), 0x00);  /* 005001 LDM */
    mflag = 0;  /* 005004 CLM */
    S -= 3; F_01E110();  /* 005005 JSRL */
    X = A;  /* 005009 TAX */
    if (X == 0) goto L_00500F;  /* 00500A BEQ */
    goto L_0050D3;  /* 00500C JMP */
L_00500F: TICK();
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_005018;  /* 00500F BBS */
    goto L_0050BC;  /* 005015 JMP */
L_005018: TICK();
    if ((rd16(ABS(0x0240)) & 0x0002) == 0x0002) goto L_005021;  /* 005018 BBS */
    goto L_0050B9;  /* 00501E JMP */
L_005021: TICK();
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0002);  /* 005021 CLB */
    mflag = 1;  /* 005026 SEM */
    wr8(DP(0x01), 0x01);  /* 005027 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_005035;  /* 00502A BBC */
    SETLO(A, rd8(DP(0x00)));  /* 00502F LDA */
    /* 005031 CMP */
    if ((A & 0xFF) == 0x71) goto L_00503E;  /* 005033 BEQ */
L_005035: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0050B1;  /* 005035 BBS */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_0050B1;  /* 00503A BBS */
L_00503E: TICK();
    mflag = 0;  /* 00503E CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_00504F;  /* 00503F BBC */
    Y = 0x0005;  /* 005045 LDY */
    S -= 3; F_00927F();  /* 005048 JSRL */
    goto L_0050AE;  /* 00504C JMP */
L_00504F: TICK();
    mflag = 0;  /* 00504F CLM */
    A = rd16(ABS(0x0365));  /* 005050 LDA */
    mflag = 1;  /* 005053 SEM */
    SETLO(A, A + 1);  /* 005054 INC */
    if ((A & 0xFF) == 0) goto L_0050A6;  /* 005055 BEQ */
    mflag = 0;  /* 005057 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0200);  /* 005058 CLB */
    A = rd16(ABS(0x0375));  /* 00505D LDA */
    mflag = 1;  /* 005060 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 005061 STA */
    mflag = 0;  /* 005063 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_005071;  /* 005064 BBC */
    A = rd16(ABS(0x0368));  /* 00506A LDA */
    mflag = 1;  /* 00506D SEM */
    wr8(ABS(0x0375), (uint8_t)A);  /* 00506E STA */
L_005071: TICK();
    mflag = 1;  /* 005071 SEM */
    if ((rd8(ABS(0x033F)) & 0x40) == 0) goto L_00507E;  /* 005072 BBC */
    SETLO(A, rd8(ABS(0x0375)));  /* 005077 LDA */
    SETLO(A, A & 0x3F);  /* 00507A AND */
    if ((A & 0xFF) != 0) goto L_005095;  /* 00507C BNE */
L_00507E: TICK();
    mflag = 0;  /* 00507E CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x2000);  /* 00507F SEB */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00508E;  /* 005084 BBC */
    S -= 3; F_0095FA();  /* 00508A JSRL */
L_00508E: TICK();
    S -= 3; F_00CB52();  /* 00508E JSRL */
    goto L_00509D;  /* 005092 JMP */
L_005095: TICK();
    Y = 0x000D;  /* 005095 LDY */
    mflag = 0;  /* 005098 CLM */
    S -= 3; F_0080AA();  /* 005099 JSRL */
L_00509D: TICK();
    A = rd16(DP(0x02));  /* 00509D LDA */
    mflag = 1;  /* 00509F SEM */
    wr8(ABS(0x0375), (uint8_t)A);  /* 0050A0 STA */
    goto L_0050AE;  /* 0050A3 JMP */
L_0050A6: TICK();
    Y = 0x0002;  /* 0050A6 LDY */
    mflag = 0;  /* 0050A9 CLM */
    S -= 3; F_0080AA();  /* 0050AA JSRL */
L_0050AE: TICK();
    goto L_0050B9;  /* 0050AE JMP */
L_0050B1: TICK();
    Y = 0x0005;  /* 0050B1 LDY */
    mflag = 0;  /* 0050B4 CLM */
    S -= 3; F_00927F();  /* 0050B5 JSRL */
L_0050B9: TICK();
    goto L_0050D3;  /* 0050B9 JMP */
L_0050BC: TICK();
    mflag = 1;  /* 0050BC SEM */
    if ((rd8(ABS(0x033F)) & 0x01) == 0) goto L_0050CD;  /* 0050BD BBC */
    mflag = 0;  /* 0050C2 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x2000);  /* 0050C3 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0200);  /* 0050C8 SEB */
L_0050CD: TICK();
    mflag = 0;  /* 0050CD CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0002);  /* 0050CE CLB */
L_0050D3: TICK();
    mflag = 0;  /* 0050D3 CLM */
    A = rd16(DP(0x01));  /* 0050D4 LDA */
    mflag = 1;  /* 0050D6 SEM */
    Y = pull16();  /* 0050D7 PLY */
    D = pull16();  /* 0050D8 PLD */
    S += 3; return;  /* 0050D9 RTL */
}

/* 0050DA  callers: 00546A, 0054CE, 0065A3, 0158E4, 015930, 017041, 01CC51 */
void F_0050DA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0050DA LDA */
    push16(D);  /* 0050DC PHD */
    D = A;  /* 0050DD TAD */
    A += 1;  /* 0050DE INC */
    A += 1;  /* 0050DF INC */
    A += 1;  /* 0050E0 INC */
    push16(A);  /* 0050E1 PHA */
    mflag = 1;  /* 0050E2 SEM */
    if ((rd8(ABS(0x033F)) & 0x08) == 0x08) goto L_0050F0;  /* 0050E3 BBS */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0050F0;  /* 0050E8 BBS */
    goto L_005243;  /* 0050ED JMP */
L_0050F0: TICK();
    mflag = 0;  /* 0050F0 CLM */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0x0001) goto L_0050FA;  /* 0050F1 BBS */
    goto L_005243;  /* 0050F7 JMP */
L_0050FA: TICK();
    mflag = 1;  /* 0050FA SEM */
    SETLO(A, 0x01);  /* 0050FB LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0050FD PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0050FE STA */
    setP(pull16());  /* 005100 PLP */
    mflag = 0;  /* 005101 CLM */
    S -= 3; i2c_start_007172();  /* 005102 JSRL */
    Y = 0x0042;  /* 005106 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 005109 JSRL */
    Y = 0x00F0;  /* 00510D LDY */
    mflag = 0;  /* 005110 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 005111 JSRL */
    mflag = 0;  /* 005115 CLM */
    S -= 3; i2c_stop_00715B();  /* 005116 JSRL */
    S -= 3; i2c_start_007172();  /* 00511A JSRL */
    Y = 0x0043;  /* 00511E LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 005121 JSRL */
    Y = 0x0001;  /* 005125 LDY */
    mflag = 0;  /* 005128 CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 005129 JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 00512D STA */
    mflag = 0;  /* 00512F CLM */
    S -= 3; i2c_stop_00715B();  /* 005130 JSRL */
    mflag = 1;  /* 005134 SEM */
    SETLO(A, 0x00);  /* 005135 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 005137 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 005138 STA */
    setP(pull16());  /* 00513A PLP */
    mflag = 0;  /* 00513B CLM */
    A = rd16(DP(0x02));  /* 00513C LDA */
    mflag = 1;  /* 00513E SEM */
    C = ((A & 0xFF) >= 0xE0);  /* 00513F CMP */
    Z = ((A & 0xFF) == 0xE0);
    if (!Z) goto L_005149;  /* 005141 BNE */
    wr8(DP(0x01), 0x00);  /* 005143 LDM */
    goto L_005177;  /* 005146 JMP */
L_005149: TICK();
    mflag = 0;  /* 005149 CLM */
    A = rd16(DP(0x02));  /* 00514A LDA */
    mflag = 1;  /* 00514C SEM */
    C = ((A & 0xFF) >= 0xD0);  /* 00514D CMP */
    Z = ((A & 0xFF) == 0xD0);
    if (!Z) goto L_005157;  /* 00514F BNE */
    wr8(DP(0x01), 0x01);  /* 005151 LDM */
    goto L_005177;  /* 005154 JMP */
L_005157: TICK();
    mflag = 0;  /* 005157 CLM */
    A = rd16(DP(0x02));  /* 005158 LDA */
    mflag = 1;  /* 00515A SEM */
    C = ((A & 0xFF) >= 0xB0);  /* 00515B CMP */
    Z = ((A & 0xFF) == 0xB0);
    if (!Z) goto L_005165;  /* 00515D BNE */
    wr8(DP(0x01), 0x02);  /* 00515F LDM */
    goto L_005177;  /* 005162 JMP */
L_005165: TICK();
    mflag = 0;  /* 005165 CLM */
    A = rd16(DP(0x02));  /* 005166 LDA */
    mflag = 1;  /* 005168 SEM */
    C = ((A & 0xFF) >= 0x70);  /* 005169 CMP */
    Z = ((A & 0xFF) == 0x70);
    if (!Z) goto L_005173;  /* 00516B BNE */
    wr8(DP(0x01), 0x03);  /* 00516D LDM */
    goto L_005177;  /* 005170 JMP */
L_005173: TICK();
    mflag = 1;  /* 005173 SEM */
    wr8(DP(0x01), 0x04);  /* 005174 LDM */
L_005177: TICK();
    SETLO(A, 0x01);  /* 005177 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 005179 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00517A STA */
    setP(pull16());  /* 00517C PLP */
    mflag = 0;  /* 00517D CLM */
    S -= 3; i2c_start_007172();  /* 00517E JSRL */
    Y = 0x0042;  /* 005182 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 005185 JSRL */
    Y = 0x000F;  /* 005189 LDY */
    mflag = 0;  /* 00518C CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00518D JSRL */
    mflag = 0;  /* 005191 CLM */
    S -= 3; i2c_stop_00715B();  /* 005192 JSRL */
    S -= 3; i2c_start_007172();  /* 005196 JSRL */
    Y = 0x0043;  /* 00519A LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00519D JSRL */
    Y = 0x0001;  /* 0051A1 LDY */
    mflag = 0;  /* 0051A4 CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 0051A5 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 0051A9 STA */
    mflag = 0;  /* 0051AB CLM */
    S -= 3; i2c_stop_00715B();  /* 0051AC JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0002);  /* 0051B0 CLB */
    mflag = 1;  /* 0051B5 SEM */
    SETLO(A, 0x00);  /* 0051B6 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0051B8 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0051B9 STA */
    setP(pull16());  /* 0051BB PLP */
    mflag = 0;  /* 0051BC CLM */
    A = rd16(DP(0x00));  /* 0051BD LDA */
    mflag = 1;  /* 0051BF SEM */
    /* 0051C0 CMP */
    if ((A & 0xFF) != 0x0E) goto L_0051CA;  /* 0051C2 BNE */
    wr8(DP(0x00), 0x03);  /* 0051C4 LDM */
    goto L_0051F8;  /* 0051C7 JMP */
L_0051CA: TICK();
    mflag = 0;  /* 0051CA CLM */
    A = rd16(DP(0x00));  /* 0051CB LDA */
    mflag = 1;  /* 0051CD SEM */
    /* 0051CE CMP */
    if ((A & 0xFF) != 0x0D) goto L_0051D8;  /* 0051D0 BNE */
    wr8(DP(0x00), 0x02);  /* 0051D2 LDM */
    goto L_0051F8;  /* 0051D5 JMP */
L_0051D8: TICK();
    mflag = 0;  /* 0051D8 CLM */
    A = rd16(DP(0x00));  /* 0051D9 LDA */
    mflag = 1;  /* 0051DB SEM */
    /* 0051DC CMP */
    if ((A & 0xFF) != 0x0B) goto L_0051E6;  /* 0051DE BNE */
    wr8(DP(0x00), 0x01);  /* 0051E0 LDM */
    goto L_0051F8;  /* 0051E3 JMP */
L_0051E6: TICK();
    mflag = 0;  /* 0051E6 CLM */
    A = rd16(DP(0x00));  /* 0051E7 LDA */
    mflag = 1;  /* 0051E9 SEM */
    /* 0051EA CMP */
    if ((A & 0xFF) != 0x07) goto L_0051F4;  /* 0051EC BNE */
    wr8(DP(0x00), 0x00);  /* 0051EE LDM */
    goto L_0051F8;  /* 0051F1 JMP */
L_0051F4: TICK();
    mflag = 1;  /* 0051F4 SEM */
    wr8(DP(0x01), 0x04);  /* 0051F5 LDM */
L_0051F8: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0051F8 LDA */
    /* 0051FA CMP */
    if ((A & 0xFF) != 0x04) goto L_005205;  /* 0051FC BNE */
    wr8(ABS(0x0226), 0x0F);  /* 0051FE LDM */
    goto L_005220;  /* 005202 JMP */
L_005205: TICK();
    mflag = 0;  /* 005205 CLM */
    A = rd16(DP(0x00));  /* 005206 LDA */
    A &= 0x00FF;  /* 005208 AND */
    A = asl16(A);  /* 00520B ASL */
    A = asl16(A);  /* 00520C ASL */
    wr16(DP(0x03), A);  /* 00520D STA */
    A = rd16(DP(0x01));  /* 00520F LDA */
    A &= 0x00FF;  /* 005211 AND */
    C = 0;  /* 005214 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 005215 ADC */
    X = A;  /* 005217 TAX */
    A = rd16((0x0031EC + X));  /* 005218 LDA */
    mflag = 1;  /* 00521C SEM */
    wr8(ABS(0x0226), (uint8_t)A);  /* 00521D STA */
L_005220: TICK();
    SETLO(A, rd8(ABS(0x0226)));  /* 005220 LDA */
    /* 005223 CMP */
    if ((A & 0xFF) >= 0x3A) goto L_005243;  /* 005225 BCS */
    /* 005227 CMP */
    if ((A & 0xFF) < 0x30) goto L_005243;  /* 005229 BCC */
    SETLO(A, rd8(ABS(0x0293)));  /* 00522B LDA */
    t = (rd8(ABS(0x0293)) + 1) & 0xFF; wr8(ABS(0x0293), t);  /* 00522E INC */
    SETLO(A, A & 0x0F);  /* 005231 AND */
    mflag = 0;  /* 005233 CLM */
    A &= 0x00FF;  /* 005234 AND */
    X = A;  /* 005237 TAX */
    A = rd16(ABS(0x0226));  /* 005238 LDA */
    mflag = 1;  /* 00523B SEM */
    wr8((ABS(0x0283) + X), (uint8_t)A);  /* 00523C STA */
    wr8(ABS(0x0226), 0x64);  /* 00523F LDM */
L_005243: TICK();
    mflag = 0;  /* 005243 CLM */
    Y = pull16();  /* 005244 PLY */
    D = pull16();  /* 005245 PLD */
    S += 3; return;  /* 005246 RTL */
}

/* 005247  callers: 0053FC */
void F_005247(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005247 LDA */
    push16(D);  /* 005249 PHD */
    D = A;  /* 00524A TAD */
    C = 0;  /* 00524B CLC */
    A = adc16(A, 0x0005);  /* 00524C ADC */
    push16(A);  /* 00524F PHA */
    wr16(DP(0x02), Y);  /* 005250 STY */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0200);  /* 005252 SEB */
L_005257: TICK();
    mflag = 1;  /* 005257 SEM */
    wr8(ABS(WDT), 0x00);  /* 005258 LDM */
    SETLO(A, rd8(ABS(0x0238)));  /* 00525C LDA */
    if ((A & 0xFF) != 0) goto L_005257;  /* 00525F BNE */
    mflag = 0;  /* 005261 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0800);  /* 005262 CLB */
    X = rd16(ABS(0x029A));  /* 005267 LDX */
    wr16(DP(0x03), X);  /* 00526A STX */
    wr16(ABS(0x029A), rd16(ABS(0x029A)) | 0x8000);  /* 00526C SEB */
    wr16(ABS(0x029A), rd16(ABS(0x029A)) & ~0x4000);  /* 005271 CLB */
    mflag = 1;  /* 005276 SEM */
    SETLO(A, rd8(ABS(0x029B)));  /* 005277 LDA */
    SETLO(A, A & 0xF8);  /* 00527A AND */
    SETLO(A, A | 0x06);  /* 00527C ORA */
    wr8(ABS(0x029B), (uint8_t)A);  /* 00527E STA */
    mflag = 0;  /* 005281 CLM */
    S -= 3; F_00CB52();  /* 005282 JSRL */
    X = rd16(DP(0x03));  /* 005286 LDX */
    wr16(ABS(0x029A), X);  /* 005288 STX */
    mflag = 1;  /* 00528B SEM */
    wr8(ABS(TA1IC), 0x00);  /* 00528C LDM */
    wr8(ABS(TA1MOD), 0x06);  /* 005290 LDM */
    wr8(ABS(TA1IC), rd8(ABS(TA1IC)) & ~0x08);  /* 005294 CLB */
    wr8(ABS(TB1IC), 0x06);  /* 005298 LDM */
    wr8(ABS(TB1MOD), 0x00);  /* 00529C LDM */
    wr8(ABS(TB1IC), rd8(ABS(TB1IC)) & ~0x08);  /* 0052A0 CLB */
    wr8(ABS(TA4IC), 0x00);  /* 0052A4 LDM */
    wr8(ABS(TA4MOD), 0x80);  /* 0052A8 LDM */
    mflag = 0;  /* 0052AC CLM */
    wr16(ABS(TA4), 0x189C);  /* 0052AD LDM */
    mflag = 1;  /* 0052B2 SEM */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 0052B3 CLB */
    SETLO(A, 0x02);  /* 0052B7 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0052B9 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0052BA STA */
    setP(pull16());  /* 0052BC PLP */
    mflag = 0;  /* 0052BD CLM */
    A = rd16(DP(0x02));  /* 0052BE LDA */
    A &= 0x00FF;  /* 0052C0 AND */
    A = asl16(A);  /* 0052C3 ASL */
    A = asl16(A);  /* 0052C4 ASL */
    A += 1;  /* 0052C5 INC */
    A += 1;  /* 0052C6 INC */
    X = A;  /* 0052C7 TAX */
    A = rd16((0x002443 + X));  /* 0052C8 LDA */
    mflag = 1;  /* 0052CC SEM */
    wr8(ABS(0x0260), (uint8_t)A);  /* 0052CD STA */
    SETLO(A, rd8(DP(0x02)));  /* 0052D0 LDA */
    mflag = 0;  /* 0052D2 CLM */
    A &= 0x00FF;  /* 0052D3 AND */
    A = asl16(A);  /* 0052D6 ASL */
    A = asl16(A);  /* 0052D7 ASL */
    A += 1;  /* 0052D8 INC */
    A += 1;  /* 0052D9 INC */
    A += 1;  /* 0052DA INC */
    X = A;  /* 0052DB TAX */
    A = rd16((0x002443 + X));  /* 0052DC LDA */
    mflag = 1;  /* 0052E0 SEM */
    wr8(ABS(0x0262), (uint8_t)A);  /* 0052E1 STA */
    SETLO(A, rd8(DP(0x02)));  /* 0052E4 LDA */
    mflag = 0;  /* 0052E6 CLM */
    A &= 0x00FF;  /* 0052E7 AND */
    A = asl16(A);  /* 0052EA ASL */
    A = asl16(A);  /* 0052EB ASL */
    X = A;  /* 0052EC TAX */
    A = rd16((0x002443 + X));  /* 0052ED LDA */
    wr16(ABS(TB1), A);  /* 0052F1 STA */
    mflag = 1;  /* 0052F4 SEM */
    wr8(ABS(0x025F), 0x00);  /* 0052F5 LDM */
    wr8(ABS(0x0261), 0x00);  /* 0052F9 LDM */
    mflag = 0;  /* 0052FD CLM */
    A = rd16(0x002473);  /* 0052FE LDA */
    C = 0;  /* 005302 CLC */
    A = adc16(A, rd16(0x002673));  /* 005303 ADC */
    wr16(ABS(TA1), A);  /* 005307 STA */
    mflag = 1;  /* 00530A SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x10);  /* 00530B SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x02);  /* 00530F SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x40);  /* 005313 SEB */
    wr8(ABS(TOSTART), rd8(ABS(TOSTART)) | 0x02);  /* 005317 SEB */
    SETLO(A, rd8(ABS(0x0261)));  /* 00531B LDA */
    C = 0;  /* 00531E CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0262))));  /* 00531F ADC */
    wr8(ABS(0x0261), (uint8_t)A);  /* 005322 STA */
    mflag = 0;  /* 005325 CLM */
    A &= 0x00FF;  /* 005326 AND */
    A = asl16(A);  /* 005329 ASL */
    X = A;  /* 00532A TAX */
    A = rd16(ABS(0x025F));  /* 00532B LDA */
    C = 0;  /* 00532E CLC */
    mflag = 1;  /* 00532F SEM */
    SETLO(A, adc8(A, rd8(ABS(0x0260))));  /* 005330 ADC */
    wr8(ABS(0x025F), (uint8_t)A);  /* 005333 STA */
    mflag = 0;  /* 005336 CLM */
    A &= 0x00FF;  /* 005337 AND */
    A = asl16(A);  /* 00533A ASL */
    wr16(DP(0x05), A);  /* 00533B STA */
    A = rd16((0x002673 + X));  /* 00533D LDA */
    wr16(DP(0x07), A);  /* 005341 STA */
    X = rd16(DP(0x05));  /* 005343 LDX */
    A = rd16(DP(0x07));  /* 005345 LDA */
    C = 0;  /* 005347 CLC */
    A = adc16(A, rd16((0x002473 + X)));  /* 005348 ADC */
    wr16(ABS(TA1), A);  /* 00534C STA */
L_00534F: TICK();
    mflag = 1;  /* 00534F SEM */
    wr8(ABS(WDT), 0x00);  /* 005350 LDM */
    if ((rd8(ABS(TA4IC)) & 0x08) == 0) goto L_00534F;  /* 005354 BBC */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 005359 CLB */
    mflag = 0;  /* 00535D CLM */
    t = (rd16(DP(0x00)) - 1) & 0xFFFF; wr16(DP(0x00), t);  /* 00535E DEC */
    X = rd16(DP(0x00));  /* 005360 LDX */
    if (X != 0) goto L_00534F;  /* 005362 BNE */
    mflag = 1;  /* 005364 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x02);  /* 005365 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x40);  /* 005369 CLB */
    SETLO(A, X);  /* 00536D TXA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00536E PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00536F STA */
    setP(pull16());  /* 005371 PLP */
    mflag = 1;  /* 005372 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x10);  /* 005373 CLB */
    wr8(ABS(TB1IC), 0x00);  /* 005377 LDM */
    Y = 0x0064;  /* 00537B LDY */
    mflag = 0;  /* 00537E CLM */
    S -= 3; F_007D7F();  /* 00537F JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0200);  /* 005383 CLB */
    A = rd16(ABS(0x0229));  /* 005388 LDA */
    mflag = 1;  /* 00538B SEM */
    /* 00538C CMP */
    if ((A & 0xFF) != 0x04) goto L_005396;  /* 00538E BNE */
    mflag = 0;  /* 005390 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0800);  /* 005391 SEB */
L_005396: TICK();
    mflag = 0;  /* 005396 CLM */
    S -= 3; F_00CB52();  /* 005397 JSRL */
    mflag = 1;  /* 00539B SEM */
    wr8(ABS(0x0238), 0x04);  /* 00539C LDM */
    mflag = 0;  /* 0053A0 CLM */
    Y = pull16();  /* 0053A1 PLY */
    D = pull16();  /* 0053A2 PLD */
    S += 3; return;  /* 0053A3 RTL */
}

/* 0053A4  callers: 0054B2, 015918 */
void F_0053A4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0053A4 LDA */
    push16(D);  /* 0053A6 PHD */
    D = A;  /* 0053A7 TAD */
    A += 1;  /* 0053A8 INC */
    push16(A);  /* 0053A9 PHA */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_005400;  /* 0053AA BBS */
    A = rd16(ABS(0x0228));  /* 0053B0 LDA */
    mflag = 1;  /* 0053B3 SEM */
    /* 0053B4 CMP */
    if ((A & 0xFF) != 0x08) goto L_005400;  /* 0053B6 BNE */
    wr8(DP(0x00), 0x00);  /* 0053B8 LDM */
    SETLO(A, rd8(ABS(0x0226)));  /* 0053BB LDA */
    /* 0053BE CMP */
    if ((A & 0xFF) != 0x64) goto L_0053D2;  /* 0053C0 BNE */
    SETLO(A, rd8(ABS(0x0283)));  /* 0053C2 LDA */
    C = 0;  /* 0053C5 CLC */
    SETLO(A, adc8(A, 0xD0));  /* 0053C6 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 0053C8 STA */
    if (!Z) goto L_0053CF;  /* 0053CA BNE */
    wr8(DP(0x00), 0x0B);  /* 0053CC LDM */
L_0053CF: TICK();
    goto L_0053ED;  /* 0053CF JMP */
L_0053D2: TICK();
    mflag = 0;  /* 0053D2 CLM */
    A = rd16(ABS(0x0226));  /* 0053D3 LDA */
    mflag = 1;  /* 0053D6 SEM */
    /* 0053D7 CMP */
    if ((A & 0xFF) != 0x2A) goto L_0053E1;  /* 0053D9 BNE */
    wr8(DP(0x00), 0x0A);  /* 0053DB LDM */
    goto L_0053ED;  /* 0053DE JMP */
L_0053E1: TICK();
    mflag = 0;  /* 0053E1 CLM */
    A = rd16(ABS(0x0226));  /* 0053E2 LDA */
    mflag = 1;  /* 0053E5 SEM */
    /* 0053E6 CMP */
    if ((A & 0xFF) != 0x23) goto L_0053ED;  /* 0053E8 BNE */
    wr8(DP(0x00), 0x0C);  /* 0053EA LDM */
L_0053ED: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0053ED LDA */
    t = (rd8(DP(0x00)) - 1) & 0xFF; wr8(DP(0x00), t);  /* 0053EF DEC */
    /* 0053F1 CMP */
    if ((A & 0xFF) == 0x00) goto L_005400;  /* 0053F3 BEQ */
    mflag = 0;  /* 0053F5 CLM */
    wr16(DP(0x01), 0x0002);  /* 0053F6 LDM */
    Y = rd16(DP(0x00));  /* 0053FA LDY */
    S -= 3; F_005247();  /* 0053FC JSRL */
L_005400: TICK();
    mflag = 0;  /* 005400 CLM */
    Y = pull16();  /* 005401 PLY */
    D = pull16();  /* 005402 PLD */
    S += 3; return;  /* 005403 RTL */
}

/* 005404  callers: 005701, 005B2E */
void F_005404(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005404 LDA */
    push16(A);  /* 005406 PHA */
    mflag = 1;  /* 005407 SEM */
    if ((rd8(ABS(0x0334)) & 0x04) == 0) goto L_00545D;  /* 005408 BBC */
    mflag = 0;  /* 00540D CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_00541C;  /* 00540E BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0004);  /* 005414 CLB */
    goto L_005422;  /* 005419 JMP */
L_00541C: TICK();
    mflag = 0;  /* 00541C CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0004);  /* 00541D SEB */
L_005422: TICK();
    if ((rd16(ABS(0x0248)) & 0x0800) == 0) goto L_00545D;  /* 005422 BBC */
    Y = 0x000B;  /* 005428 LDY */
    S -= 3; F_0080AA();  /* 00542B JSRL */
    Y = 0x000A;  /* 00542F LDY */
    S -= 3; F_003310();  /* 005432 JSRL */
    mflag = 1;  /* 005436 SEM */
    wr8(ABS(0x0230), 0x9D);  /* 005437 LDM */
    wr8(ABS(0x0231), 0x14);  /* 00543B LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00545D;  /* 00543F BBC */
    mflag = 0;  /* 005444 CLM */
    S -= 3; F_00828C();  /* 005445 JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 005449 SEB */
    mflag = 1;  /* 00544E SEM */
    wr8(ABS(0x0232), 0x3F);  /* 00544F LDM */
    mflag = 0;  /* 005453 CLM */
    S -= 3; F_007A06();  /* 005454 JSRL */
    mflag = 1;  /* 005458 SEM */
    wr8(ABS(0x0226), 0x0F);  /* 005459 LDM */
L_00545D: TICK();
    mflag = 0;  /* 00545D CLM */
    Y = pull16();  /* 00545E PLY */
    S += 3; return;  /* 00545F RTL */
}

/* 005460  callers: 00681D */
void F_005460(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005460 LDA */
    push16(A);  /* 005462 PHA */
    mflag = 1;  /* 005463 SEM */
    if ((rd8(ABS(0x023A)) & 0x02) == 0) goto L_00546E;  /* 005464 BBC */
    mflag = 0;  /* 005469 CLM */
    S -= 3; F_0050DA();  /* 00546A JSRL */
L_00546E: TICK();
    mflag = 0;  /* 00546E CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_0054C2;  /* 00546F BBS */
    A = rd16(ABS(0x0226));  /* 005475 LDA */
    mflag = 1;  /* 005478 SEM */
    /* 005479 CMP */
    if ((A & 0xFF) == 0x0F) goto L_0054BF;  /* 00547B BEQ */
    mflag = 0;  /* 00547D CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 00547E SEB */
    mflag = 1;  /* 005483 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 005484 LDM */
    mflag = 0;  /* 005488 CLM */
    S -= 3; F_007A06();  /* 005489 JSRL */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0) goto L_00549D;  /* 00548D BBC */
    Y = 0x000D;  /* 005493 LDY */
    S -= 3; F_0080AA();  /* 005496 JSRL */
    goto L_0054B6;  /* 00549A JMP */
L_00549D: TICK();
    mflag = 0;  /* 00549D CLM */
    A = rd16(ABS(0x0226));  /* 00549E LDA */
    mflag = 1;  /* 0054A1 SEM */
    /* 0054A2 CMP */
    if ((A & 0xFF) != 0x71) goto L_0054B1;  /* 0054A4 BNE */
    Y = rd16(ABS(0x0226));  /* 0054A6 LDY */
    mflag = 0;  /* 0054A9 CLM */
    S -= 3; F_004FF6();  /* 0054AA JSRL */
    goto L_0054B6;  /* 0054AE JMP */
L_0054B1: TICK();
    mflag = 0;  /* 0054B1 CLM */
    S -= 3; F_0053A4();  /* 0054B2 JSRL */
L_0054B6: TICK();
    mflag = 1;  /* 0054B6 SEM */
    wr8(ABS(0x0293), 0x00);  /* 0054B7 LDM */
    wr8(ABS(0x0226), 0x0F);  /* 0054BB LDM */
L_0054BF: TICK();
    goto L_00581F;  /* 0054BF JMP */
L_0054C2: TICK();
    mflag = 1;  /* 0054C2 SEM */
    if ((rd8(ABS(0x0242)) & 0x02) == 0) goto L_005513;  /* 0054C3 BBC */
    SETLO(A, rd8(ABS(0x0233)));  /* 0054C8 LDA */
    if ((A & 0xFF) != 0) goto L_005513;  /* 0054CB BNE */
    mflag = 0;  /* 0054CD CLM */
    S -= 3; F_0050DA();  /* 0054CE JSRL */
    mflag = 1;  /* 0054D2 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_0054EB;  /* 0054D3 BBC */
    Y = rd16(ABS(0x0226));  /* 0054D8 LDY */
    mflag = 0;  /* 0054DB CLM */
    S -= 3; F_004FF6();  /* 0054DC JSRL */
    /* 0054E0 CMP */
    if ((A & 0xFF) == 0x00) goto L_0054E8;  /* 0054E2 BEQ */
    wr8(ABS(0x0226), 0x71);  /* 0054E4 LDM */
L_0054E8: TICK();
    goto L_00550A;  /* 0054E8 JMP */
L_0054EB: TICK();
    mflag = 0;  /* 0054EB CLM */
    if ((rd16(ABS(0x0242)) & 0x0020) == 0) goto L_00550A;  /* 0054EC BBC */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0020);  /* 0054F2 CLB */
    A = rd16(ABS(0x0226));  /* 0054F7 LDA */
    mflag = 1;  /* 0054FA SEM */
    /* 0054FB CMP */
    if ((A & 0xFF) != 0x71) goto L_005505;  /* 0054FD BNE */
    mflag = 0;  /* 0054FF CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0010);  /* 005500 CLB */
L_005505: TICK();
    mflag = 1;  /* 005505 SEM */
    wr8(ABS(0x0226), 0x71);  /* 005506 LDM */
L_00550A: TICK();
    mflag = 0;  /* 00550A CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0002);  /* 00550B CLB */
    goto L_005791;  /* 005510 JMP */
L_005513: TICK();
    mflag = 0;  /* 005513 CLM */
    A = rd16(ABS(0x0226));  /* 005514 LDA */
    mflag = 1;  /* 005517 SEM */
    /* 005518 CMP */
    if ((A & 0xFF) != 0x0F) goto L_00551F;  /* 00551A BNE */
    goto L_005791;  /* 00551C JMP */
L_00551F: TICK();
    mflag = 0;  /* 00551F CLM */
    S -= 3; F_00328B();  /* 005520 JSRL */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0) goto L_00552D;  /* 005524 BBC */
    goto L_005765;  /* 00552A JMP */
L_00552D: TICK();
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_005571;  /* 00552D BBC */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0x0100) goto L_005571;  /* 005533 BBS */
    A = rd16(ABS(0x0226));  /* 005539 LDA */
    mflag = 1;  /* 00553C SEM */
    /* 00553D CMP */
    if ((A & 0xFF) == 0x64) goto L_005561;  /* 00553F BEQ */
    SETLO(A, rd8(ABS(0x0226)));  /* 005541 LDA */
    /* 005544 CMP */
    if ((A & 0xFF) == 0x2A) goto L_005561;  /* 005546 BEQ */
    /* 005548 CMP */
    if ((A & 0xFF) == 0x23) goto L_005561;  /* 00554A BEQ */
    /* 00554C CMP */
    if ((A & 0xFF) != 0x73) goto L_005555;  /* 00554E BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 005550 LDA */
    if ((A & 0xFF) == 0) goto L_005561;  /* 005553 BEQ */
L_005555: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_005571;  /* 005555 BBS */
    SETLO(A, rd8(ABS(0x0226)));  /* 00555A LDA */
    /* 00555D CMP */
    if ((A & 0xFF) != 0x6D) goto L_005571;  /* 00555F BNE */
L_005561: TICK();
    wr8(ABS(0x0293), 0x00);  /* 005561 LDM */
    wr8(ABS(0x0226), 0x0F);  /* 005565 LDM */
    Y = 0x000D;  /* 005569 LDY */
    mflag = 0;  /* 00556C CLM */
    S -= 3; F_0080AA();  /* 00556D JSRL */
L_005571: TICK();
    mflag = 0;  /* 005571 CLM */
    A = rd16(ABS(0x0226));  /* 005572 LDA */
    mflag = 1;  /* 005575 SEM */
    /* 005576 CMP */
    if ((A & 0xFF) == 0x6D) goto L_00557D;  /* 005578 BEQ */
    goto L_00567F;  /* 00557A JMP */
L_00557D: TICK();
    SETLO(A, rd8(ABS(0x0231)));  /* 00557D LDA */
    if ((A & 0xFF) == 0) goto L_005589;  /* 005580 BEQ */
    /* 005582 CMP */
    if ((A & 0xFF) == 0x14) goto L_005589;  /* 005584 BEQ */
    goto L_00562A;  /* 005586 JMP */
L_005589: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 005589 LDA */
    mflag = 0;  /* 00558C CLM */
    A &= 0x00FF;  /* 00558D AND */
    X = A;  /* 005590 TAX */
    A = rd16((0x003188 + X));  /* 005591 LDA */
    mflag = 1;  /* 005595 SEM */
    wr8(ABS(0x0227), (uint8_t)A);  /* 005596 STA */
    mflag = 0;  /* 005599 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_0055B9;  /* 00559A BBC */
    mflag = 1;  /* 0055A0 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_0055B4;  /* 0055A1 BBC */
    SETLO(A, rd8(ABS(0x0227)));  /* 0055A6 LDA */
    /* 0055A9 CMP */
    if ((A & 0xFF) != 0x02) goto L_0055B1;  /* 0055AB BNE */
    wr8(ABS(0x0227), 0x00);  /* 0055AD LDM */
L_0055B1: TICK();
    goto L_0055B9;  /* 0055B1 JMP */
L_0055B4: TICK();
    mflag = 1;  /* 0055B4 SEM */
    wr8(ABS(0x0227), 0x00);  /* 0055B5 LDM */
L_0055B9: TICK();
    mflag = 1;  /* 0055B9 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0055CD;  /* 0055BA BBS */
    SETLO(A, rd8(ABS(0x0227)));  /* 0055BF LDA */
    /* 0055C2 CMP */
    if ((A & 0xFF) != 0x01) goto L_0055CA;  /* 0055C4 BNE */
    wr8(ABS(0x0227), 0x02);  /* 0055C6 LDM */
L_0055CA: TICK();
    goto L_0055DA;  /* 0055CA JMP */
L_0055CD: TICK();
    mflag = 0;  /* 0055CD CLM */
    A = rd16(ABS(0x0227));  /* 0055CE LDA */
    mflag = 1;  /* 0055D1 SEM */
    /* 0055D2 CMP */
    if ((A & 0xFF) != 0x0B) goto L_0055DA;  /* 0055D4 BNE */
    wr8(ABS(0x0227), 0x0E);  /* 0055D6 LDM */
L_0055DA: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 0055DA LDA */
    /* 0055DD CMP */
    if ((A & 0xFF) != 0x11) goto L_0055EF;  /* 0055DF BNE */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_0055EB;  /* 0055E1 BBC */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0055EF;  /* 0055E6 BBS */
L_0055EB: TICK();
    wr8(ABS(0x0227), 0x04);  /* 0055EB LDM */
L_0055EF: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 0055EF LDA */
    /* 0055F2 CMP */
    if ((A & 0xFF) != 0x04) goto L_005604;  /* 0055F4 BNE */
    if ((rd8(ABS(0x033F)) & 0x02) == 0) goto L_005600;  /* 0055F6 BBC */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_005604;  /* 0055FB BBS */
L_005600: TICK();
    wr8(ABS(0x0227), 0x08);  /* 005600 LDM */
L_005604: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 005604 LDA */
    /* 005607 CMP */
    if ((A & 0xFF) != 0x12) goto L_00561A;  /* 005609 BNE */
    SETLO(A, rd8(ABS(0x01EB)));  /* 00560B LDA */
    /* 00560E CMP */
    if ((A & 0xFF) >= 0x03) goto L_005615;  /* 005610 BCS */
    goto L_00561A;  /* 005612 JMP */
L_005615: TICK();
    mflag = 1;  /* 005615 SEM */
    wr8(ABS(0x0227), 0x08);  /* 005616 LDM */
L_00561A: TICK();
    SETLO(A, rd8(ABS(0x0227)));  /* 00561A LDA */
    /* 00561D CMP */
    if ((A & 0xFF) != 0x08) goto L_00562A;  /* 00561F BNE */
    if ((rd8(ABS(0x0340)) & 0x20) == 0x20) goto L_00562A;  /* 005621 BBS */
    wr8(ABS(0x0227), 0x00);  /* 005626 LDM */
L_00562A: TICK();
    mflag = 0;  /* 00562A CLM */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_005636;  /* 00562B BBC */
    mflag = 1;  /* 005631 SEM */
    wr8(ABS(0x0227), 0x00);  /* 005632 LDM */
L_005636: TICK();
    mflag = 1;  /* 005636 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 005637 LDA */
    if ((A & 0xFF) == 0) goto L_005659;  /* 00563A BEQ */
    mflag = 0;  /* 00563C CLM */
    A &= 0x00FF;  /* 00563D AND */
    X = A;  /* 005640 TAX */
    A = rd16((0x003188 + X));  /* 005641 LDA */
    mflag = 1;  /* 005645 SEM */
    t = rd8(ABS(0x0227));  /* 005646 CMP */
    if ((A & 0xFF) == t) goto L_005659;  /* 005649 BEQ */
    mflag = 0;  /* 00564B CLM */
    wr16(ABS(0x02DF), 0x3280);  /* 00564C LDM */
    mflag = 1;  /* 005651 SEM */
    wr8(ABS(0x02E1), 0x00);  /* 005652 LDM */
    goto L_005674;  /* 005656 JMP */
L_005659: TICK();
    mflag = 0;  /* 005659 CLM */
    A = rd16(ABS(0x0227));  /* 00565A LDA */
    A &= 0x00FF;  /* 00565D AND */
    A = asl16(A);  /* 005660 ASL */
    A = asl16(A);  /* 005661 ASL */
    A += 1;  /* 005662 INC */
    X = A;  /* 005663 TAX */
    A = rd16((0x00319E + X));  /* 005664 LDA */
    mflag = 1;  /* 005668 SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 005669 STA */
    mflag = 0;  /* 00566C CLM */
    A = rd16((0x00319C + X));  /* 00566D LDA */
    wr16(ABS(0x02DF), A);  /* 005671 STA */
L_005674: TICK();
    Y = 0x002B;  /* 005674 LDY */
    mflag = 0;  /* 005677 CLM */
    S -= 3; F_003310();  /* 005678 JSRL */
    goto L_005762;  /* 00567C JMP */
L_00567F: TICK();
    mflag = 0;  /* 00567F CLM */
    A = rd16(ABS(0x0226));  /* 005680 LDA */
    mflag = 1;  /* 005683 SEM */
    /* 005684 CMP */
    if ((A & 0xFF) != 0x73) goto L_0056E9;  /* 005686 BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 005688 LDA */
    mflag = 0;  /* 00568B CLM */
    A &= 0x00FF;  /* 00568C AND */
    A = asl16(A);  /* 00568F ASL */
    A = asl16(A);  /* 005690 ASL */
    A += 1;  /* 005691 INC */
    X = A;  /* 005692 TAX */
    A = rd16((0x00319E + X));  /* 005693 LDA */
    mflag = 1;  /* 005697 SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 005698 STA */
    mflag = 0;  /* 00569B CLM */
    A = rd16((0x00319C + X));  /* 00569C LDA */
    wr16(ABS(0x02DF), A);  /* 0056A0 STA */
    A = rd16(ABS(0x0227));  /* 0056A3 LDA */
    A &= 0x00FF;  /* 0056A6 AND */
    A = asl16(A);  /* 0056A9 ASL */
    A = asl16(A);  /* 0056AA ASL */
    X = A;  /* 0056AB TAX */
    A = rd16((0x00319C + X));  /* 0056AC LDA */
    mflag = 1;  /* 0056B0 SEM */
    wr8(ABS(0x0227), (uint8_t)A);  /* 0056B1 STA */
    mflag = 0;  /* 0056B4 CLM */
    A &= 0x00FF;  /* 0056B5 AND */
    X = A;  /* 0056B8 TAX */
    A = rd16((0x003188 + X));  /* 0056B9 LDA */
    mflag = 1;  /* 0056BD SEM */
    t = rd8(ABS(0x0227));  /* 0056BE CMP */
    if ((A & 0xFF) != t) goto L_0056DE;  /* 0056C1 BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 0056C3 LDA */
    mflag = 0;  /* 0056C6 CLM */
    A &= 0x00FF;  /* 0056C7 AND */
    A = asl16(A);  /* 0056CA ASL */
    A = asl16(A);  /* 0056CB ASL */
    A += 1;  /* 0056CC INC */
    X = A;  /* 0056CD TAX */
    A = rd16((0x00319E + X));  /* 0056CE LDA */
    mflag = 1;  /* 0056D2 SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 0056D3 STA */
    mflag = 0;  /* 0056D6 CLM */
    A = rd16((0x00319C + X));  /* 0056D7 LDA */
    wr16(ABS(0x02DF), A);  /* 0056DB STA */
L_0056DE: TICK();
    Y = 0x002B;  /* 0056DE LDY */
    mflag = 0;  /* 0056E1 CLM */
    S -= 3; F_003310();  /* 0056E2 JSRL */
    goto L_005762;  /* 0056E6 JMP */
L_0056E9: TICK();
    mflag = 0;  /* 0056E9 CLM */
    A = rd16(ABS(0x0226));  /* 0056EA LDA */
    mflag = 1;  /* 0056ED SEM */
    /* 0056EE CMP */
    if ((A & 0xFF) != 0x70) goto L_00571B;  /* 0056F0 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_005708;  /* 0056F2 BBC */
    mflag = 0;  /* 0056F7 CLM */
    S -= 3; F_01691C();  /* 0056F8 JSRL */
    /* 0056FC CMP */
    if ((A & 0xFF) != 0x00) goto L_005705;  /* 0056FE BNE */
    mflag = 0;  /* 005700 CLM */
    S -= 3; F_005404();  /* 005701 JSRL */
L_005705: TICK();
    goto L_005718;  /* 005705 JMP */
L_005708: TICK();
    mflag = 1;  /* 005708 SEM */
    wr8(ABS(0x0227), 0x00);  /* 005709 LDM */
    mflag = 0;  /* 00570D CLM */
    wr16(ABS(0x02DF), 0x4A9A);  /* 00570E LDM */
    mflag = 1;  /* 005713 SEM */
    wr8(ABS(0x02E1), 0x00);  /* 005714 LDM */
L_005718: TICK();
    goto L_005762;  /* 005718 JMP */
L_00571B: TICK();
    mflag = 0;  /* 00571B CLM */
    A = rd16(ABS(0x0226));  /* 00571C LDA */
    mflag = 1;  /* 00571F SEM */
    /* 005720 CMP */
    if ((A & 0xFF) != 0x71) goto L_005744;  /* 005722 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00573B;  /* 005724 BBC */
    wr8(ABS(0x0233), 0x10);  /* 005729 LDM */
    mflag = 0;  /* 00572D CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0002);  /* 00572E SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0002);  /* 005733 SEB */
    goto L_005741;  /* 005738 JMP */
L_00573B: TICK();
    mflag = 0;  /* 00573B CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0010);  /* 00573C SEB */
L_005741: TICK();
    goto L_005762;  /* 005741 JMP */
L_005744: TICK();
    mflag = 0;  /* 005744 CLM */
    A = rd16(ABS(0x0226));  /* 005745 LDA */
    mflag = 1;  /* 005748 SEM */
    /* 005749 CMP */
    if ((A & 0xFF) != 0x23) goto L_005750;  /* 00574B BNE */
    goto L_005762;  /* 00574D JMP */
L_005750: TICK();
    mflag = 0;  /* 005750 CLM */
    A = rd16(ABS(0x0226));  /* 005751 LDA */
    mflag = 1;  /* 005754 SEM */
    /* 005755 CMP */
    if ((A & 0xFF) != 0x2A) goto L_005762;  /* 005757 BNE */
    mflag = 0;  /* 005759 CLM */
    S -= 3; F_01CA08();  /* 00575A JSRL */
    S -= 3; F_014A76();  /* 00575E JSRL */
L_005762: TICK();
    goto L_005791;  /* 005762 JMP */
L_005765: TICK();
    mflag = 0;  /* 005765 CLM */
    A = rd16(ABS(0x0226));  /* 005766 LDA */
    mflag = 1;  /* 005769 SEM */
    /* 00576A CMP */
    if ((A & 0xFF) != 0x71) goto L_005780;  /* 00576C BNE */
    wr8(ABS(0x0233), 0x10);  /* 00576E LDM */
    mflag = 0;  /* 005772 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0020);  /* 005773 SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0002);  /* 005778 SEB */
    goto L_005791;  /* 00577D JMP */
L_005780: TICK();
    mflag = 1;  /* 005780 SEM */
    wr8(ABS(0x0293), 0x00);  /* 005781 LDM */
    wr8(ABS(0x0226), 0x0F);  /* 005785 LDM */
    Y = 0x000D;  /* 005789 LDY */
    mflag = 0;  /* 00578C CLM */
    S -= 3; F_0080AA();  /* 00578D JSRL */
L_005791: TICK();
    mflag = 0;  /* 005791 CLM */
    A = rd16(ABS(0x0226));  /* 005792 LDA */
    mflag = 1;  /* 005795 SEM */
    /* 005796 CMP */
    if ((A & 0xFF) != 0x0F) goto L_00579D;  /* 005798 BNE */
    goto L_00581F;  /* 00579A JMP */
L_00579D: TICK();
    wr8(ABS(0x0230), 0x9D);  /* 00579D LDM */
    mflag = 0;  /* 0057A1 CLM */
    if ((rd16(ABS(0x0242)) & 0x0010) == 0) goto L_0057B8;  /* 0057A2 BBC */
    if ((rd16(ABS(0x0242)) & 0x0020) == 0x0020) goto L_0057B5;  /* 0057A8 BBS */
    Y = 0x000D;  /* 0057AE LDY */
    S -= 3; F_0080AA();  /* 0057B1 JSRL */
L_0057B5: TICK();
    goto L_0057F0;  /* 0057B5 JMP */
L_0057B8: TICK();
    mflag = 0;  /* 0057B8 CLM */
    X = rd16(ABS(0x0214));  /* 0057B9 LDX */
    if (X != 0) goto L_0057DD;  /* 0057BC BNE */
    mflag = 1;  /* 0057BE SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_0057CB;  /* 0057BF BBC */
    mflag = 0;  /* 0057C4 CLM */
    if ((rd16(ABS(0x0240)) & 0x0002) == 0) goto L_0057D8;  /* 0057C5 BBC */
L_0057CB: TICK();
    mflag = 1;  /* 0057CB SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0057DD;  /* 0057CC BBS */
    mflag = 0;  /* 0057D1 CLM */
    if ((rd16(ABS(0x0242)) & 0x0020) == 0x0020) goto L_0057DD;  /* 0057D2 BBS */
L_0057D8: TICK();
    Y = X;  /* 0057D8 TXY */
    S -= 3; F_0080AA();  /* 0057D9 JSRL */
L_0057DD: TICK();
    mflag = 0;  /* 0057DD CLM */
    mflag = 1;  /* 0057DE SEM */
    SETLO(A, rd8(ABS(0x02E1)));  /* 0057DF LDA */
    mflag = 0;  /* 0057E2 CLM */
    push16(A);  /* 0057E3 PHA */
    A = rd16(ABS(0x02DF));  /* 0057E4 LDA */
    push16(A);  /* 0057E7 PHA */
    S -= 3; rt_call_ptr_01E4C2();  /* 0057E8 JSRL */
    S -= 3; F_00828C();  /* 0057EC JSRL */
L_0057F0: TICK();
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 0057F0 SEB */
    mflag = 1;  /* 0057F5 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 0057F6 LDM */
    mflag = 0;  /* 0057FA CLM */
    S -= 3; F_007A06();  /* 0057FB JSRL */
    mflag = 1;  /* 0057FF SEM */
    SETLO(A, rd8(ABS(0x0231)));  /* 005800 LDA */
    if ((A & 0xFF) != 0) goto L_00581A;  /* 005803 BNE */
    SETLO(A, rd8(ABS(0x02E1)));  /* 005805 LDA */
    Z = ((A & 0xFF) == 0x00);  /* 005808 CMP */
    if (!Z) goto L_005813;  /* 00580A BNE */
    mflag = 0;  /* 00580C CLM */
    A = rd16(ABS(0x02DF));  /* 00580D LDA */
    Z = (A == 0x374A);  /* 005810 CMP */
L_005813: TICK();
    if (Z) goto L_00581A;  /* 005813 BEQ */
    mflag = 1;  /* 005815 SEM */
    wr8(ABS(0x0231), 0x14);  /* 005816 LDM */
L_00581A: TICK();
    mflag = 1;  /* 00581A SEM */
    wr8(ABS(0x0226), 0x0F);  /* 00581B LDM */
L_00581F: TICK();
    mflag = 0;  /* 00581F CLM */
    Y = pull16();  /* 005820 PLY */
    S += 3; return;  /* 005821 RTL */
}

/* 005822  callers: 01A1C3 */
void F_005822(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005822 LDA */
    push16(D);  /* 005824 PHD */
    D = A;  /* 005825 TAD */
    A += 1;  /* 005826 INC */
    push16(A);  /* 005827 PHA */
    wr16(DP(0x00), Y);  /* 005828 STY */
    A = Y;  /* 00582A TYA */
    mflag = 1;  /* 00582B SEM */
    /* 00582C CMP */
    if ((A & 0xFF) != 0x67) goto L_00584D;  /* 00582E BNE */
    mflag = 0;  /* 005830 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0040);  /* 005831 SEB */
    if ((rd16(ABS(0x023C)) & 0x0010) == 0) goto L_005844;  /* 005836 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 00583C CLB */
    goto L_00584A;  /* 005841 JMP */
L_005844: TICK();
    mflag = 0;  /* 005844 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 005845 SEB */
L_00584A: TICK();
    goto L_005886;  /* 00584A JMP */
L_00584D: TICK();
    mflag = 0;  /* 00584D CLM */
    A = rd16(DP(0x00));  /* 00584E LDA */
    mflag = 1;  /* 005850 SEM */
    /* 005851 CMP */
    if ((A & 0xFF) != 0x6E) goto L_005865;  /* 005853 BNE */
    mflag = 0;  /* 005855 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0002);  /* 005856 SEB */
    Y = 0x0008;  /* 00585B LDY */
    S -= 3; F_004FF6();  /* 00585E JSRL */
    goto L_005886;  /* 005862 JMP */
L_005865: TICK();
    mflag = 0;  /* 005865 CLM */
    A = rd16(DP(0x00));  /* 005866 LDA */
    mflag = 1;  /* 005868 SEM */
    /* 005869 CMP */
    if ((A & 0xFF) != 0x6F) goto L_005886;  /* 00586B BNE */
    mflag = 0;  /* 00586D CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0002);  /* 00586E SEB */
    mflag = 1;  /* 005873 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00587E;  /* 005874 BBC */
    SETLO(A, 0x71);  /* 005879 LDA */
    goto L_005880;  /* 00587B JMP */
L_00587E: TICK();
    SETLO(A, 0x00);  /* 00587E LDA */
L_005880: TICK();
    mflag = 0;  /* 005880 CLM */
    Y = A;  /* 005881 TAY */
    S -= 3; F_004FF6();  /* 005882 JSRL */
L_005886: TICK();
    mflag = 0;  /* 005886 CLM */
    S -= 3; F_00828C();  /* 005887 JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 00588B SEB */
    mflag = 1;  /* 005890 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 005891 LDM */
    mflag = 0;  /* 005895 CLM */
    S -= 3; F_007A06();  /* 005896 JSRL */
    Y = pull16();  /* 00589A PLY */
    D = pull16();  /* 00589B PLD */
    S += 3; return;  /* 00589C RTL */
}

/* 00589D  callers: 005919, 005AAD, 015BE0, 015D4D, 01687D, 0168B3, 01739E */
void F_00589D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00589D LDA */
    push16(D);  /* 00589F PHD */
    D = A;  /* 0058A0 TAD */
    C = 0;  /* 0058A1 CLC */
    A = adc16(A, 0x0004);  /* 0058A2 ADC */
    push16(A);  /* 0058A5 PHA */
    wr16(DP(0x00), Y);  /* 0058A6 STY */
    mflag = 1;  /* 0058A8 SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 0058A9 STA */
    mflag = 0;  /* 0058AC CLM */
    S -= 3; F_0074D8();  /* 0058AD JSRL */
    wr8(DP(0x03), (uint8_t)A);  /* 0058B1 STA */
    SETLO(A, 0x01);  /* 0058B3 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0058B5 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0058B6 STA */
    setP(pull16());  /* 0058B8 PLP */
    mflag = 0;  /* 0058B9 CLM */
    S -= 3; i2c_start_007172();  /* 0058BA JSRL */
    Y = 0x0046;  /* 0058BE LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0058C1 JSRL */
    Y = rd16(DP(0x03));  /* 0058C5 LDY */
    mflag = 0;  /* 0058C7 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0058C8 JSRL */
    mflag = 0;  /* 0058CC CLM */
    S -= 3; i2c_stop_00715B();  /* 0058CD JSRL */
    S -= 3; i2c_start_007172();  /* 0058D1 JSRL */
    Y = 0x0047;  /* 0058D5 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0058D8 JSRL */
    wr8(LIND(0x00), (uint8_t)A);  /* 0058DC STA */
    Y = 0x0001;  /* 0058DE LDY */
    mflag = 0;  /* 0058E1 CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 0058E2 JSRL */
    wr8(DP(0x03), (uint8_t)A);  /* 0058E6 STA */
    mflag = 0;  /* 0058E8 CLM */
    S -= 3; i2c_stop_00715B();  /* 0058E9 JSRL */
    mflag = 1;  /* 0058ED SEM */
    SETLO(A, 0x00);  /* 0058EE LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0058F0 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0058F1 STA */
    setP(pull16());  /* 0058F3 PLP */
    mflag = 0;  /* 0058F4 CLM */
    A = rd16(DP(0x03));  /* 0058F5 LDA */
    mflag = 1;  /* 0058F7 SEM */
    Y = pull16();  /* 0058F8 PLY */
    D = pull16();  /* 0058F9 PLD */
    S += 3; return;  /* 0058FA RTL */
}

/* 0058FB  callers: 006821 */
void F_0058FB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0058FB LDA */
    push16(D);  /* 0058FD PHD */
    D = A;  /* 0058FE TAD */
    C = 0;  /* 0058FF CLC */
    A = adc16(A, 0x0006);  /* 005900 ADC */
    push16(A);  /* 005903 PHA */
    mflag = 1;  /* 005904 SEM */
    if ((rd8(ABS(0x023A)) & 0x04) == 0x04) goto L_00590D;  /* 005905 BBS */
    goto L_005A72;  /* 00590A JMP */
L_00590D: TICK();
    mflag = 0;  /* 00590D CLM */
    S -= 3; F_00328B();  /* 00590E JSRL */
    A = D;  /* 005912 TDA */
    A += 1;  /* 005913 INC */
    B = 0x0000;  /* 005914 LDA */
    Y = A;  /* 005918 TAY */
    S -= 3; F_00589D();  /* 005919 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00591D STA */
    mflag = 0;  /* 00591F CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0004);  /* 005920 CLB */
    mflag = 1;  /* 005925 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00592E;  /* 005926 BBS */
    goto L_0059FF;  /* 00592B JMP */
L_00592E: TICK();
    if ((rd8(ABS(0x0240)) & 0x20) == 0) goto L_005938;  /* 00592E BBC */
    SETLO(A, 0x01);  /* 005933 LDA */
    goto L_00593A;  /* 005935 JMP */
L_005938: TICK();
    SETLO(A, 0x00);  /* 005938 LDA */
L_00593A: TICK();
    wr8(DP(0x02), (uint8_t)A);  /* 00593A STA */
    if ((rd8(ABS(0x0244)) & 0x10) == 0) goto L_005946;  /* 00593C BBC */
    SETLO(A, 0x01);  /* 005941 LDA */
    goto L_005948;  /* 005943 JMP */
L_005946: TICK();
    SETLO(A, 0x00);  /* 005946 LDA */
L_005948: TICK();
    wr8(DP(0x03), (uint8_t)A);  /* 005948 STA */
    if ((rd8(ABS(0x033F)) & 0x02) == 0) goto L_005965;  /* 00594A BBC */
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_00595C;  /* 00594F BBC */
    mflag = 0;  /* 005953 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0020);  /* 005954 CLB */
    goto L_005962;  /* 005959 JMP */
L_00595C: TICK();
    mflag = 0;  /* 00595C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0020);  /* 00595D SEB */
L_005962: TICK();
    goto L_005979;  /* 005962 JMP */
L_005965: TICK();
    mflag = 1;  /* 005965 SEM */
    if ((rd8(DP(0x00)) & 0x80) == 0) goto L_005973;  /* 005966 BBC */
    mflag = 0;  /* 00596A CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0020);  /* 00596B SEB */
    goto L_005979;  /* 005970 JMP */
L_005973: TICK();
    mflag = 0;  /* 005973 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0020);  /* 005974 CLB */
L_005979: TICK();
    mflag = 1;  /* 005979 SEM */
    if ((rd8(DP(0x00)) & 0x40) == 0) goto L_00598B;  /* 00597A BBC */
    SETLO(A, rd8(DP(0x01)));  /* 00597E LDA */
    if ((A & 0xFF) != 0) goto L_00598B;  /* 005980 BNE */
    mflag = 0;  /* 005982 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0010);  /* 005983 SEB */
    goto L_005991;  /* 005988 JMP */
L_00598B: TICK();
    mflag = 0;  /* 00598B CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0010);  /* 00598C CLB */
L_005991: TICK();
    A = rd16(DP(0x02));  /* 005991 LDA */
    A &= 0x00FF;  /* 005993 AND */
    wr16(DP(0x06), A);  /* 005996 STA */
    A = rd16(ABS(0x0240));  /* 005998 LDA */
    A &= 0x0020;  /* 00599B AND */
    if (A == 0) goto L_0059A3;  /* 00599E BEQ */
    A = 0x0001;  /* 0059A0 LDA */
L_0059A3: TICK();
    t = rd16(DP(0x06));  /* 0059A3 CMP */
    if (A == t) goto L_0059AE;  /* 0059A5 BEQ */
    Y = 0x0001;  /* 0059A7 LDY */
    S -= 3; F_00927F();  /* 0059AA JSRL */
L_0059AE: TICK();
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_0059C1;  /* 0059AE BBC */
    mflag = 1;  /* 0059B4 SEM */
    if ((rd8(DP(0x00)) & 0x20) == 0x20) goto L_0059BE;  /* 0059B5 BBS */
    mflag = 0;  /* 0059B9 CLM */
    S -= 3; F_00973E();  /* 0059BA JSRL */
L_0059BE: TICK();
    goto L_0059FC;  /* 0059BE JMP */
L_0059C1: TICK();
    mflag = 1;  /* 0059C1 SEM */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_0059FC;  /* 0059C2 BBC */
    if ((rd8(ABS(0x0240)) & 0x20) == 0) goto L_0059D1;  /* 0059C7 BBC */
    SETLO(A, 0x01);  /* 0059CC LDA */
    goto L_0059D3;  /* 0059CE JMP */
L_0059D1: TICK();
    SETLO(A, 0x00);  /* 0059D1 LDA */
L_0059D3: TICK();
    wr8(DP(0x04), (uint8_t)A);  /* 0059D3 STA */
    if ((rd8(ABS(0x0244)) & 0x10) == 0) goto L_0059DF;  /* 0059D5 BBC */
    SETLO(A, 0x01);  /* 0059DA LDA */
    goto L_0059E1;  /* 0059DC JMP */
L_0059DF: TICK();
    SETLO(A, 0x00);  /* 0059DF LDA */
L_0059E1: TICK();
    wr8(DP(0x05), (uint8_t)A);  /* 0059E1 STA */
    SETLO(A, rd8(DP(0x04)));  /* 0059E3 LDA */
    t = rd8(DP(0x02));  /* 0059E5 CMP */
    if ((A & 0xFF) != t) goto L_0059FC;  /* 0059E7 BNE */
    SETLO(A, rd8(DP(0x05)));  /* 0059E9 LDA */
    t = rd8(DP(0x03));  /* 0059EB CMP */
    if ((A & 0xFF) != t) goto L_0059FC;  /* 0059ED BNE */
    if ((rd8(DP(0x00)) & 0x20) == 0) goto L_0059FC;  /* 0059EF BBC */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_0059FC;  /* 0059F3 BBS [P45 ACIL_CAGRI] */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 0059F8 SEB */
L_0059FC: TICK();
    goto L_005A72;  /* 0059FC JMP */
L_0059FF: TICK();
    mflag = 1;  /* 0059FF SEM */
    SETLO(B, 0x00);  /* 005A00 LDA */
    SETLO(A, rd8(DP(0x00)));  /* 005A03 LDA */
    div8(0x10);  /* 005A05 DIV */
    SETLO(A, A ^ 0xFF);  /* 005A08 EOR */
    SETLO(A, A + 1);  /* 005A0A INC */
    C = 0;  /* 005A0B CLC */
    SETLO(A, adc8(A, 0x10));  /* 005A0C ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 005A0E STA */
    t = rd8(ABS(0x0206));  /* 005A10 CMP */
    if ((A & 0xFF) == t) goto L_005A2E;  /* 005A13 BEQ */
    mflag = 0;  /* 005A15 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0004);  /* 005A16 SEB */
    mflag = 1;  /* 005A1B SEM */
    wr8(ABS(0x0233), 0x04);  /* 005A1C LDM */
    SETLO(A, rd8(DP(0x02)));  /* 005A20 LDA */
    wr8(ABS(0x0206), (uint8_t)A);  /* 005A22 STA */
    mflag = 0;  /* 005A25 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0040);  /* 005A26 SEB */
    goto L_005A72;  /* 005A2B JMP */
L_005A2E: TICK();
    mflag = 1;  /* 005A2E SEM */
    if ((rd8(DP(0x00)) & 0x04) == 0) goto L_005A37;  /* 005A2F BBC */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_005A72;  /* 005A33 BBS */
L_005A37: TICK();
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_005A66;  /* 005A37 BBC */
    if ((rd8(ABS(0x033F)) & 0x80) == 0) goto L_005A46;  /* 005A3C BBC */
    SETLO(A, 0x02);  /* 005A41 LDA */
    goto L_005A48;  /* 005A43 JMP */
L_005A46: TICK();
    SETLO(A, 0x10);  /* 005A46 LDA */
L_005A48: TICK();
    wr8(ABS(0x0233), (uint8_t)A);  /* 005A48 STA */
    if ((rd8(DP(0x00)) & 0x04) == 0x04) goto L_005A58;  /* 005A4B BBS */
    mflag = 0;  /* 005A4F CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0004);  /* 005A50 SEB */
    goto L_005A63;  /* 005A55 JMP */
L_005A58: TICK();
    mflag = 1;  /* 005A58 SEM */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_005A63;  /* 005A59 BBS */
    mflag = 0;  /* 005A5D CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0002);  /* 005A5E SEB */
L_005A63: TICK();
    goto L_005A72;  /* 005A63 JMP */
L_005A66: TICK();
    mflag = 1;  /* 005A66 SEM */
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_005A72;  /* 005A67 BBS */
    Y = rd16(DP(0x00));  /* 005A6B LDY */
    mflag = 0;  /* 005A6D CLM */
    S -= 3; F_004FF6();  /* 005A6E JSRL */
L_005A72: TICK();
    mflag = 1;  /* 005A72 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_005A7B;  /* 005A73 BBC */
    goto L_005B71;  /* 005A78 JMP */
L_005A7B: TICK();
    if ((rd8(ABS(0x023A)) & 0x01) == 0x01) goto L_005A83;  /* 005A7B BBS */
    goto L_005B71;  /* 005A80 JMP */
L_005A83: TICK();
    if ((rd8(ABS(0x023E)) & 0x10) == 0x10) goto L_005A8B;  /* 005A83 BBS */
    goto L_005B71;  /* 005A88 JMP */
L_005A8B: TICK();
    SETLO(A, rd8(ABS(0x0233)));  /* 005A8B LDA */
    if ((A & 0xFF) == 0) goto L_005A93;  /* 005A8E BEQ */
    goto L_005B71;  /* 005A90 JMP */
L_005A93: TICK();
    if ((rd8(ABS(0x0242)) & 0x04) == 0x04) goto L_005AA5;  /* 005A93 BBS */
    if ((rd8(ABS(0x0240)) & 0x02) == 0x02) goto L_005AA5;  /* 005A98 BBS */
    if ((rd8(ABS(0x023A)) & 0x40) == 0x40) goto L_005AA5;  /* 005A9D BBS */
    goto L_005B71;  /* 005AA2 JMP */
L_005AA5: TICK();
    mflag = 0;  /* 005AA5 CLM */
    A = D;  /* 005AA6 TDA */
    A += 1;  /* 005AA7 INC */
    B = 0x0000;  /* 005AA8 LDA */
    Y = A;  /* 005AAC TAY */
    S -= 3; F_00589D();  /* 005AAD JSRL */
    SETLO(B, 0x00);  /* 005AB1 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 005AB4 STA */
    div8(0x10);  /* 005AB6 DIV */
    SETLO(A, A ^ 0xFF);  /* 005AB9 EOR */
    SETLO(A, A + 1);  /* 005ABB INC */
    C = 0;  /* 005ABC CLC */
    SETLO(A, adc8(A, 0x10));  /* 005ABD ADC */
    wr8(ABS(0x0206), (uint8_t)A);  /* 005ABF STA */
    mflag = 0;  /* 005AC2 CLM */
    if ((rd16(ABS(0x023A)) & 0x0040) == 0) goto L_005AFD;  /* 005AC3 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x8000);  /* 005AC9 CLB */
    mflag = 1;  /* 005ACE SEM */
    wr8(ABS(0x0227), 0x00);  /* 005ACF LDM */
    mflag = 0;  /* 005AD3 CLM */
    wr16(ABS(0x02DF), 0x374A);  /* 005AD4 LDM */
    mflag = 1;  /* 005AD9 SEM */
    wr8(ABS(0x02E1), 0x00);  /* 005ADA LDM */
    if ((rd8(ABS(0x0334)) & 0x10) == 0x10) goto L_005AF5;  /* 005ADE BBS */
    mflag = 0;  /* 005AE3 CLM */
    mflag = 1;  /* 005AE4 SEM */
    SETLO(A, rd8(ABS(0x02E1)));  /* 005AE5 LDA */
    mflag = 0;  /* 005AE8 CLM */
    push16(A);  /* 005AE9 PHA */
    A = rd16(ABS(0x02DF));  /* 005AEA LDA */
    push16(A);  /* 005AED PHA */
    S -= 3; rt_call_ptr_01E4C2();  /* 005AEE JSRL */
    goto L_005AFA;  /* 005AF2 JMP */
L_005AF5: TICK();
    mflag = 0;  /* 005AF5 CLM */
    S -= 3; F_004D03();  /* 005AF6 JSRL */
L_005AFA: TICK();
    goto L_005B43;  /* 005AFA JMP */
L_005AFD: TICK();
    mflag = 0;  /* 005AFD CLM */
    if ((rd16(ABS(0x0242)) & 0x0004) == 0) goto L_005B35;  /* 005AFE BBC */
    S -= 3; F_00FC09();  /* 005B04 JSRL */
    X = A;  /* 005B08 TAX */
    if (X != 0) goto L_005B32;  /* 005B09 BNE */
    mflag = 1;  /* 005B0B SEM */
    if ((rd8(DP(0x00)) & 0x04) == 0x04) goto L_005B2D;  /* 005B0C BBS */
    mflag = 0;  /* 005B10 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0040);  /* 005B11 SEB */
    if ((rd16(ABS(0x023C)) & 0x0010) == 0) goto L_005B24;  /* 005B16 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 005B1C CLB */
    goto L_005B2A;  /* 005B21 JMP */
L_005B24: TICK();
    mflag = 0;  /* 005B24 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 005B25 SEB */
L_005B2A: TICK();
    goto L_005B32;  /* 005B2A JMP */
L_005B2D: TICK();
    mflag = 0;  /* 005B2D CLM */
    S -= 3; F_005404();  /* 005B2E JSRL */
L_005B32: TICK();
    goto L_005B43;  /* 005B32 JMP */
L_005B35: TICK();
    mflag = 0;  /* 005B35 CLM */
    S -= 3; F_00FB79();  /* 005B36 JSRL */
    X = A;  /* 005B3A TAX */
    if (X != 0) goto L_005B43;  /* 005B3B BNE */
    Y = rd16(DP(0x00));  /* 005B3D LDY */
    S -= 3; F_004FF6();  /* 005B3F JSRL */
L_005B43: TICK();
    mflag = 0;  /* 005B43 CLM */
    X = rd16(ABS(0x0214));  /* 005B44 LDX */
    if (X != 0) goto L_005B54;  /* 005B47 BNE */
    if ((rd16(ABS(0x023A)) & 0x0040) == 0x0040) goto L_005B54;  /* 005B49 BBS */
    Y = X;  /* 005B4F TXY */
    S -= 3; F_0080AA();  /* 005B50 JSRL */
L_005B54: TICK();
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0040);  /* 005B54 CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0004);  /* 005B59 CLB */
    S -= 3; F_00828C();  /* 005B5E JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 005B62 SEB */
    mflag = 1;  /* 005B67 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 005B68 LDM */
    mflag = 0;  /* 005B6C CLM */
    S -= 3; F_007A06();  /* 005B6D JSRL */
L_005B71: TICK();
    mflag = 0;  /* 005B71 CLM */
    Y = pull16();  /* 005B72 PLY */
    D = pull16();  /* 005B73 PLD */
    S += 3; return;  /* 005B74 RTL */
}

/* 005B75  callers: 005C28, 005C3C */
void F_005B75(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005B75 LDA */
    push16(D);  /* 005B77 PHD */
    D = A;  /* 005B78 TAD */
    A += 1;  /* 005B79 INC */
    A += 1;  /* 005B7A INC */
    A += 1;  /* 005B7B INC */
    push16(A);  /* 005B7C PHA */
    wr16(DP(0x00), Y);  /* 005B7D STY */
    A = Y;  /* 005B7F TYA */
    A &= 0x00FF;  /* 005B80 AND */
    A -= 1;  /* 005B83 DEC */
    mpy16(0x000D);  /* 005B84 MPY */
    C = 0;  /* 005B88 CLC */
    A = adc16(A, 0x002E);  /* 005B89 ADC */
    Y = A;  /* 005B8C TAY */
    S -= 3; eeprom_read_word_0071F0();  /* 005B8D JSRL */
    wr16(DP(0x01), A);  /* 005B91 STA */
    A &= 0x3FFF;  /* 005B93 AND */
    /* 005B96 CMP */
    if (A != 0x3FFF) goto L_005BA2;  /* 005B99 BNE */
    wr16(DP(0x01), 0x0000);  /* 005B9B LDM */
    goto L_005BA7;  /* 005B9F JMP */
L_005BA2: TICK();
    mflag = 0;  /* 005BA2 CLM */
    wr16(DP(0x01), 0x0001);  /* 005BA3 LDM */
L_005BA7: TICK();
    A = rd16(DP(0x01));  /* 005BA7 LDA */
    mflag = 1;  /* 005BA9 SEM */
    Y = pull16();  /* 005BAA PLY */
    D = pull16();  /* 005BAB PLD */
    S += 3; return;  /* 005BAC RTL */
}

/* 005BAD  callers: 005D5F, 005E3B, 006810 */
void F_005BAD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005BAD LDA */
    push16(D);  /* 005BAF PHD */
    D = A;  /* 005BB0 TAD */
    A += 1;  /* 005BB1 INC */
    A += 1;  /* 005BB2 INC */
    A += 1;  /* 005BB3 INC */
    push16(A);  /* 005BB4 PHA */
    S -= 3; F_01E0C6();  /* 005BB5 JSRL */
    X = A;  /* 005BB9 TAX */
    if (X == 0) goto L_005BBF;  /* 005BBA BEQ */
    goto L_005CA2;  /* 005BBC JMP */
L_005BBF: TICK();
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_005BC8;  /* 005BBF BBC */
    goto L_005CA2;  /* 005BC5 JMP */
L_005BC8: TICK();
    mflag = 1;  /* 005BC8 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 005BC9 STA */
L_005BCB: TICK();
    Y = rd16(DP(0x00));  /* 005BCB LDY */
    mflag = 0;  /* 005BCD CLM */
    S -= 3; F_0069BB();  /* 005BCE JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 005BD2 STA */
    /* 005BD4 CMP */
    if ((A & 0xFF) == 0x0F) goto L_005BDA;  /* 005BD6 BEQ */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 005BD8 INC */
L_005BDA: TICK();
    /* 005BDA CMP */
    if ((A & 0xFF) == 0x0F) goto L_005BE4;  /* 005BDC BEQ */
    SETLO(A, rd8(DP(0x00)));  /* 005BDE LDA */
    /* 005BE0 CMP */
    if ((A & 0xFF) < 0x06) goto L_005BCB;  /* 005BE2 BCC */
L_005BE4: TICK();
    if ((rd8(ABS(0x0341)) & 0x80) == 0x80) goto L_005BEC;  /* 005BE4 BBS */
    goto L_005CA2;  /* 005BE9 JMP */
L_005BEC: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 005BEC LDA */
    if ((A & 0xFF) != 0) goto L_005BF3;  /* 005BEE BNE */
    goto L_005CA2;  /* 005BF0 JMP */
L_005BF3: TICK();
    SETLO(A, rd8(ABS(0x0341)));  /* 005BF3 LDA */
    SETLO(A, A & 0x7F);  /* 005BF6 AND */
    wr8(DP(0x01), (uint8_t)A);  /* 005BF8 STA */
    mflag = 0;  /* 005BFA CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_005C08;  /* 005BFB BBC */
    A = rd16(ABS(0x035C));  /* 005C01 LDA */
    mflag = 1;  /* 005C04 SEM */
    wr8(ABS(0x0369), (uint8_t)A);  /* 005C05 STA */
L_005C08: TICK();
    mflag = 1;  /* 005C08 SEM */
    SETLO(A, rd8(DP(0x01)));  /* 005C09 LDA */
    if ((A & 0xFF) == 0) goto L_005C1D;  /* 005C0B BEQ */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_005C17;  /* 005C0D BBC */
    SETLO(A, 0x0A);  /* 005C12 LDA */
    goto L_005C19;  /* 005C14 JMP */
L_005C17: TICK();
    SETLO(A, 0x63);  /* 005C17 LDA */
L_005C19: TICK();
    t = rd8(DP(0x01));  /* 005C19 CMP */
    if ((A & 0xFF) >= t) goto L_005C25;  /* 005C1B BCS */
L_005C1D: TICK();
    SETLO(A, rd8(ABS(0x0369)));  /* 005C1D LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 005C20 STA */
    goto L_005C35;  /* 005C22 JMP */
L_005C25: TICK();
    Y = rd16(DP(0x01));  /* 005C25 LDY */
    mflag = 0;  /* 005C27 CLM */
    S -= 3; F_005B75();  /* 005C28 JSRL */
    /* 005C2C CMP */
    if ((A & 0xFF) != 0x00) goto L_005C35;  /* 005C2E BNE */
    SETLO(A, rd8(ABS(0x0369)));  /* 005C30 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 005C33 STA */
L_005C35: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 005C35 LDA */
    if ((A & 0xFF) == 0) goto L_005C9A;  /* 005C37 BEQ */
    Y = rd16(DP(0x01));  /* 005C39 LDY */
    mflag = 0;  /* 005C3B CLM */
    S -= 3; F_005B75();  /* 005C3C JSRL */
    /* 005C40 CMP */
    if ((A & 0xFF) == 0x00) goto L_005C8F;  /* 005C42 BEQ */
    mflag = 0;  /* 005C44 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0800);  /* 005C45 SEB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x8000);  /* 005C4A CLB */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0400);  /* 005C4F CLB */
    Y = rd16(DP(0x01));  /* 005C54 LDY */
    S -= 3; F_008D14();  /* 005C56 JSRL */
    A = rd16(ABS(0x0369));  /* 005C5A LDA */
    mflag = 1;  /* 005C5D SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 005C5E STA */
    mflag = 0;  /* 005C60 CLM */
    wr16(DP(0x03), 0x0370);  /* 005C61 LDM */
    wr16(DP(0x05), 0x0363);  /* 005C65 LDM */
    A = 0x0003;  /* 005C69 LDA */
    Y = 0x000D;  /* 005C6C LDY */
    S -= 3; rt_memcpy_01E466();  /* 005C6F JSRL */
    A = rd16(DP(0x01));  /* 005C73 LDA */
    mflag = 1;  /* 005C75 SEM */
    wr8(ABS(0x0376), (uint8_t)A);  /* 005C76 STA */
    wr8(ABS(0x0369), (uint8_t)A);  /* 005C79 STA */
    mflag = 0;  /* 005C7C CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0020);  /* 005C7D SEB */
    wr16(ABS(0x021E), 0x0139);  /* 005C82 LDM */
    wr16(ABS(0x021C), 0x24A4);  /* 005C87 LDM */
    goto L_005C97;  /* 005C8C JMP */
L_005C8F: TICK();
    Y = 0x0005;  /* 005C8F LDY */
    mflag = 0;  /* 005C92 CLM */
    S -= 3; F_0080AA();  /* 005C93 JSRL */
L_005C97: TICK();
    goto L_005CA2;  /* 005C97 JMP */
L_005C9A: TICK();
    Y = 0x0002;  /* 005C9A LDY */
    mflag = 0;  /* 005C9D CLM */
    S -= 3; F_0080AA();  /* 005C9E JSRL */
L_005CA2: TICK();
    mflag = 0;  /* 005CA2 CLM */
    Y = pull16();  /* 005CA3 PLY */
    D = pull16();  /* 005CA4 PLD */
    S += 3; return;  /* 005CA5 RTL */
}

/* 005CA6  callers: 006825 */
void F_005CA6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005CA6 LDA */
    push16(A);  /* 005CA8 PHA */
    mflag = 1;  /* 005CA9 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_005CB2;  /* 005CAA BBS */
    goto L_005D7A;  /* 005CAF JMP */
L_005CB2: TICK();
    mflag = 0;  /* 005CB2 CLM */
    if ((rd16(ABS(0x023A)) & 0x0080) == 0) goto L_005CBC;  /* 005CB3 BBC */
    goto L_005D4E;  /* 005CB9 JMP */
L_005CBC: TICK();
    if ((rd16(ABS(0x0240)) & 0x0200) == 0x0200) goto L_005D16;  /* 005CBC BBS */
    mflag = 1;  /* 005CC2 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0x10) goto L_005CD1;  /* 005CC3 BBS [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 005CC8 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0001);  /* 005CC9 CLB */
    goto L_005CE2;  /* 005CCE JMP */
L_005CD1: TICK();
    mflag = 0;  /* 005CD1 CLM */
    if ((rd16(ABS(0x0242)) & 0x0001) == 0x0001) goto L_005CE2;  /* 005CD2 BBS */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 005CD8 SEB */
    mflag = 1;  /* 005CDD SEM */
    wr8(ABS(0x0233), 0x02);  /* 005CDE LDM */
L_005CE2: TICK();
    mflag = 1;  /* 005CE2 SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0x80) goto L_005CF1;  /* 005CE3 BBS [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 005CE8 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0020);  /* 005CE9 SEB */
    goto L_005D02;  /* 005CEE JMP */
L_005CF1: TICK();
    mflag = 0;  /* 005CF1 CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0) goto L_005D02;  /* 005CF2 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 005CF8 SEB */
    mflag = 1;  /* 005CFD SEM */
    wr8(ABS(0x0233), 0x02);  /* 005CFE LDM */
L_005D02: TICK();
    mflag = 1;  /* 005D02 SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_005D13;  /* 005D03 BBC [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005D08 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 005D09 SEB */
    mflag = 1;  /* 005D0E SEM */
    wr8(ABS(0x0233), 0x02);  /* 005D0F LDM */
L_005D13: TICK();
    goto L_005D4B;  /* 005D13 JMP */
L_005D16: TICK();
    mflag = 1;  /* 005D16 SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 005D17 LDA */
    if ((A & 0xFF) != 0) goto L_005D4B;  /* 005D1A BNE */
    mflag = 0;  /* 005D1C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 005D1D CLB */
    mflag = 1;  /* 005D22 SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0) goto L_005D2E;  /* 005D23 BBC [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 005D28 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0001);  /* 005D29 SEB */
L_005D2E: TICK();
    mflag = 1;  /* 005D2E SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0) goto L_005D3A;  /* 005D2F BBC [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 005D34 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0020);  /* 005D35 CLB */
L_005D3A: TICK();
    mflag = 1;  /* 005D3A SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_005D4B;  /* 005D3B BBC [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005D40 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0080);  /* 005D41 SEB */
    mflag = 1;  /* 005D46 SEM */
    wr8(ABS(0x0233), 0x3F);  /* 005D47 LDM */
L_005D4B: TICK();
    goto L_005D77;  /* 005D4B JMP */
L_005D4E: TICK();
    mflag = 1;  /* 005D4E SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 005D4F LDA */
    if ((A & 0xFF) != 0) goto L_005D66;  /* 005D52 BNE */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_005D63;  /* 005D54 BBC [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005D59 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0080);  /* 005D5A CLB */
    S -= 3; F_005BAD();  /* 005D5F JSRL */
L_005D63: TICK();
    goto L_005D77;  /* 005D63 JMP */
L_005D66: TICK();
    mflag = 1;  /* 005D66 SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_005D77;  /* 005D67 BBS [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005D6C CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0080);  /* 005D6D CLB */
    mflag = 1;  /* 005D72 SEM */
    wr8(ABS(0x0233), 0x00);  /* 005D73 LDM */
L_005D77: TICK();
    goto L_005E53;  /* 005D77 JMP */
L_005D7A: TICK();
    mflag = 0;  /* 005D7A CLM */
    if ((rd16(ABS(0x023A)) & 0x0080) == 0) goto L_005D84;  /* 005D7B BBC */
    goto L_005E21;  /* 005D81 JMP */
L_005D84: TICK();
    if ((rd16(ABS(0x0240)) & 0x0200) == 0x0200) goto L_005DDE;  /* 005D84 BBS */
    mflag = 1;  /* 005D8A SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0x10) goto L_005D99;  /* 005D8B BBS [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 005D90 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0010);  /* 005D91 SEB */
    goto L_005DAA;  /* 005D96 JMP */
L_005D99: TICK();
    mflag = 0;  /* 005D99 CLM */
    if ((rd16(ABS(0x0244)) & 0x0010) == 0) goto L_005DAA;  /* 005D9A BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 005DA0 SEB */
    mflag = 1;  /* 005DA5 SEM */
    wr8(ABS(0x0233), 0x02);  /* 005DA6 LDM */
L_005DAA: TICK();
    mflag = 1;  /* 005DAA SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0x80) goto L_005DB9;  /* 005DAB BBS [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 005DB0 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0020);  /* 005DB1 SEB */
    goto L_005DCA;  /* 005DB6 JMP */
L_005DB9: TICK();
    mflag = 0;  /* 005DB9 CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0) goto L_005DCA;  /* 005DBA BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 005DC0 SEB */
    mflag = 1;  /* 005DC5 SEM */
    wr8(ABS(0x0233), 0x02);  /* 005DC6 LDM */
L_005DCA: TICK();
    mflag = 1;  /* 005DCA SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_005DDB;  /* 005DCB BBS [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005DD0 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 005DD1 SEB */
    mflag = 1;  /* 005DD6 SEM */
    wr8(ABS(0x0233), 0x02);  /* 005DD7 LDM */
L_005DDB: TICK();
    goto L_005E1E;  /* 005DDB JMP */
L_005DDE: TICK();
    mflag = 1;  /* 005DDE SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 005DDF LDA */
    if ((A & 0xFF) != 0) goto L_005E1E;  /* 005DE2 BNE */
    mflag = 0;  /* 005DE4 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 005DE5 CLB */
    mflag = 1;  /* 005DEA SEM */
    if ((rd8(ABS(P4)) & 0x10) == 0) goto L_005DF6;  /* 005DEB BBC [P44 KONTAK_ANAHTARI] */
    mflag = 0;  /* 005DF0 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0010);  /* 005DF1 CLB */
L_005DF6: TICK();
    mflag = 1;  /* 005DF6 SEM */
    if ((rd8(ABS(P4)) & 0x80) == 0) goto L_005E02;  /* 005DF7 BBC [P47 DIS_BAS_KONUS] */
    mflag = 0;  /* 005DFC CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0020);  /* 005DFD CLB */
L_005E02: TICK();
    mflag = 1;  /* 005E02 SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_005E1E;  /* 005E03 BBS [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005E08 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0080);  /* 005E09 SEB */
    mflag = 1;  /* 005E0E SEM */
    if ((rd8(ABS(0x033F)) & 0x80) == 0) goto L_005E19;  /* 005E0F BBC */
    SETLO(A, 0x0A);  /* 005E14 LDA */
    goto L_005E1B;  /* 005E16 JMP */
L_005E19: TICK();
    SETLO(A, 0x3F);  /* 005E19 LDA */
L_005E1B: TICK();
    wr8(ABS(0x0233), (uint8_t)A);  /* 005E1B STA */
L_005E1E: TICK();
    goto L_005E53;  /* 005E1E JMP */
L_005E21: TICK();
    mflag = 1;  /* 005E21 SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 005E22 LDA */
    if ((A & 0xFF) != 0) goto L_005E42;  /* 005E25 BNE */
    mflag = 0;  /* 005E27 CLM */
    S -= 3; F_00FD57();  /* 005E28 JSRL */
    X = A;  /* 005E2C TAX */
    if (X != 0) goto L_005E3F;  /* 005E2D BNE */
    mflag = 1;  /* 005E2F SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_005E3F;  /* 005E30 BBS [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005E35 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0080);  /* 005E36 CLB */
    S -= 3; F_005BAD();  /* 005E3B JSRL */
L_005E3F: TICK();
    goto L_005E53;  /* 005E3F JMP */
L_005E42: TICK();
    mflag = 1;  /* 005E42 SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_005E53;  /* 005E43 BBC [P45 ACIL_CAGRI] */
    mflag = 0;  /* 005E48 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0080);  /* 005E49 CLB */
    mflag = 1;  /* 005E4E SEM */
    wr8(ABS(0x0233), 0x00);  /* 005E4F LDM */
L_005E53: TICK();
    mflag = 0;  /* 005E53 CLM */
    Y = pull16();  /* 005E54 PLY */
    S += 3; return;  /* 005E55 RTL */
}

/* 005E56  callers: 00682D */
void F_005E56(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005E56 LDA */
    push16(A);  /* 005E58 PHA */
    if ((rd16(ABS(0x0240)) & 0x4000) == 0) goto L_005E62;  /* 005E59 BBC */
    goto L_005F94;  /* 005E5F JMP */
L_005E62: TICK();
    A = rd16(ABS(0x0244));  /* 005E62 LDA */
    mflag = 1;  /* 005E65 SEM */
    SETLO(A, A & 0x3F);  /* 005E66 AND */
    if ((A & 0xFF) != 0) goto L_005E6D;  /* 005E68 BNE */
    goto L_005F69;  /* 005E6A JMP */
L_005E6D: TICK();
    mflag = 0;  /* 005E6D CLM */
    S -= 3; F_00328B();  /* 005E6E JSRL */
    X = rd16(ABS(0x0214));  /* 005E72 LDX */
    if (X == 0) goto L_005E7A;  /* 005E75 BEQ */
    goto L_005F66;  /* 005E77 JMP */
L_005E7A: TICK();
    A = rd16(ABS(0x0365));  /* 005E7A LDA */
    mflag = 1;  /* 005E7D SEM */
    SETLO(A, A + 1);  /* 005E7E INC */
    if ((A & 0xFF) != 0) goto L_005E8C;  /* 005E7F BNE */
    Y = 0x0002;  /* 005E81 LDY */
    mflag = 0;  /* 005E84 CLM */
    S -= 3; F_0080AA();  /* 005E85 JSRL */
    goto L_005F66;  /* 005E89 JMP */
L_005E8C: TICK();
    mflag = 0;  /* 005E8C CLM */
    A = rd16(ABS(0x036C));  /* 005E8D LDA */
    mflag = 1;  /* 005E90 SEM */
    SETLO(A, A + 1);  /* 005E91 INC */
    if ((A & 0xFF) != 0) goto L_005E9F;  /* 005E92 BNE */
    Y = 0x0005;  /* 005E94 LDY */
    mflag = 0;  /* 005E97 CLM */
    S -= 3; F_0080AA();  /* 005E98 JSRL */
    goto L_005F66;  /* 005E9C JMP */
L_005E9F: TICK();
    mflag = 0;  /* 005E9F CLM */
    if ((rd16(ABS(0x0244)) & 0x0008) == 0) goto L_005EA9;  /* 005EA0 BBC */
    goto L_005F56;  /* 005EA6 JMP */
L_005EA9: TICK();
    if ((rd16(ABS(0x0242)) & 0x0001) == 0) goto L_005EBE;  /* 005EA9 BBC */
    Y = 0x0006;  /* 005EAF LDY */
    S -= 3; F_0080AA();  /* 005EB2 JSRL */
    mflag = 1;  /* 005EB6 SEM */
    wr8(ABS(0x0229), 0x02);  /* 005EB7 LDM */
    goto L_005F53;  /* 005EBB JMP */
L_005EBE: TICK();
    mflag = 0;  /* 005EBE CLM */
    if ((rd16(ABS(0x0244)) & 0x0040) == 0) goto L_005ECF;  /* 005EBF BBC */
    Y = 0x0010;  /* 005EC5 LDY */
    S -= 3; F_0080AA();  /* 005EC8 JSRL */
    goto L_005F53;  /* 005ECC JMP */
L_005ECF: TICK();
    mflag = 1;  /* 005ECF SEM */
    if ((rd8(ABS(0x034E)) & 0x08) == 0) goto L_005EE5;  /* 005ED0 BBC */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_005EE5;  /* 005ED5 BBC */
    Y = 0x0003;  /* 005EDA LDY */
    mflag = 0;  /* 005EDD CLM */
    S -= 3; F_0080AA();  /* 005EDE JSRL */
    goto L_005F53;  /* 005EE2 JMP */
L_005EE5: TICK();
    mflag = 0;  /* 005EE5 CLM */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0x0001) goto L_005F43;  /* 005EE6 BBS */
    if ((rd16(ABS(0x0244)) & 0x0002) == 0x0002) goto L_005F43;  /* 005EEC BBS */
    mflag = 1;  /* 005EF2 SEM */
    if ((rd8(ABS(0x0340)) & 0x10) == 0) goto L_005F19;  /* 005EF3 BBC */
    if ((rd8(ABS(0x0240)) & 0x01) == 0) goto L_005F19;  /* 005EF8 BBC */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_005F19;  /* 005EFD BBC */
    mflag = 0;  /* 005F02 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_005F0F;  /* 005F03 BBC */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_005F19;  /* 005F09 BBC */
L_005F0F: TICK();
    Y = 0x0003;  /* 005F0F LDY */
    S -= 3; F_0080AA();  /* 005F12 JSRL */
    goto L_005F40;  /* 005F16 JMP */
L_005F19: TICK();
    mflag = 0;  /* 005F19 CLM */
    X = rd16(ABS(0x0222));  /* 005F1A LDX */
    if (X != 0) goto L_005F25;  /* 005F1D BNE */
    mflag = 1;  /* 005F1F SEM */
    if ((rd8(ABS(0x0244)) & 0x80) == 0) goto L_005F30;  /* 005F20 BBC */
L_005F25: TICK();
    Y = 0x0004;  /* 005F25 LDY */
    mflag = 0;  /* 005F28 CLM */
    S -= 3; F_0080AA();  /* 005F29 JSRL */
    goto L_005F40;  /* 005F2D JMP */
L_005F30: TICK();
    mflag = 1;  /* 005F30 SEM */
    SETLO(A, rd8(ABS(0x04E6)));  /* 005F31 LDA */
    if ((A & 0xFF) == 0) goto L_005F3B;  /* 005F34 BEQ */
    SETLO(A, 0x03);  /* 005F36 LDA */
    goto L_005F3D;  /* 005F38 JMP */
L_005F3B: TICK();
    SETLO(A, 0x04);  /* 005F3B LDA */
L_005F3D: TICK();
    wr8(ABS(0x0229), (uint8_t)A);  /* 005F3D STA */
L_005F40: TICK();
    goto L_005F53;  /* 005F40 JMP */
L_005F43: TICK();
    mflag = 1;  /* 005F43 SEM */
    SETLO(A, rd8(ABS(0x04E6)));  /* 005F44 LDA */
    if ((A & 0xFF) == 0) goto L_005F4E;  /* 005F47 BEQ */
    SETLO(A, 0x03);  /* 005F49 LDA */
    goto L_005F50;  /* 005F4B JMP */
L_005F4E: TICK();
    SETLO(A, 0x04);  /* 005F4E LDA */
L_005F50: TICK();
    wr8(ABS(0x0229), (uint8_t)A);  /* 005F50 STA */
L_005F53: TICK();
    goto L_005F66;  /* 005F53 JMP */
L_005F56: TICK();
    mflag = 1;  /* 005F56 SEM */
    SETLO(A, rd8(ABS(0x04E6)));  /* 005F57 LDA */
    if ((A & 0xFF) == 0) goto L_005F61;  /* 005F5A BEQ */
    SETLO(A, 0x03);  /* 005F5C LDA */
    goto L_005F63;  /* 005F5E JMP */
L_005F61: TICK();
    SETLO(A, 0x04);  /* 005F61 LDA */
L_005F63: TICK();
    wr8(ABS(0x0229), (uint8_t)A);  /* 005F63 STA */
L_005F66: TICK();
    goto L_005F91;  /* 005F66 JMP */
L_005F69: TICK();
    mflag = 0;  /* 005F69 CLM */
    S -= 3; F_01E178();  /* 005F6A JSRL */
    X = A;  /* 005F6E TAX */
    if (X != 0) goto L_005F76;  /* 005F6F BNE */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0080);  /* 005F71 CLB */
L_005F76: TICK();
    A = rd16(ABS(0x0365));  /* 005F76 LDA */
    mflag = 1;  /* 005F79 SEM */
    SETLO(A, A + 1);  /* 005F7A INC */
    if ((A & 0xFF) != 0) goto L_005F84;  /* 005F7B BNE */
    mflag = 0;  /* 005F7D CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_005F8C;  /* 005F7E BBC */
L_005F84: TICK();
    mflag = 1;  /* 005F84 SEM */
    wr8(ABS(0x0229), 0x02);  /* 005F85 LDM */
    goto L_005F91;  /* 005F89 JMP */
L_005F8C: TICK();
    mflag = 1;  /* 005F8C SEM */
    wr8(ABS(0x0229), 0x00);  /* 005F8D LDM */
L_005F91: TICK();
    goto L_005F99;  /* 005F91 JMP */
L_005F94: TICK();
    mflag = 1;  /* 005F94 SEM */
    wr8(ABS(0x0229), 0x00);  /* 005F95 LDM */
L_005F99: TICK();
    mflag = 0;  /* 005F99 CLM */
    Y = pull16();  /* 005F9A PLY */
    S += 3; return;  /* 005F9B RTL */
}

/* 005F9C  callers: 006829 */
void F_005F9C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 005F9C LDA */
    push16(A);  /* 005F9E PHA */
    mflag = 1;  /* 005F9F SEM */
    if ((rd8(ABS(0x023A)) & 0x20) == 0x20) goto L_005FA8;  /* 005FA0 BBS */
    goto L_006049;  /* 005FA5 JMP */
L_005FA8: TICK();
    mflag = 0;  /* 005FA8 CLM */
    X = rd16(ABS(0x021C));  /* 005FA9 LDX */
    if (X != 0) goto L_005FBA;  /* 005FAC BNE */
    S -= 3; F_00968D();  /* 005FAE JSRL */
    mflag = 1;  /* 005FB2 SEM */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 005FB3 SEB */
    goto L_006049;  /* 005FB7 JMP */
L_005FBA: TICK();
    mflag = 0;  /* 005FBA CLM */
    if ((rd16(ABS(0x023A)) & 0x0010) == 0) goto L_005FEF;  /* 005FBB BBC */
    if ((rd16(ABS(0x023E)) & 0x8000) == 0) goto L_005FE1;  /* 005FC1 BBC */
    wr16(ABS(0x021A), 0x0754);  /* 005FC7 LDM */
    X = rd16(ABS(0x021C));  /* 005FCC LDX */
    /* 005FCF CPX */
    if (X >= 0x0754) goto L_005FD9;  /* 005FD2 BCS */
    wr16(ABS(0x021C), 0x0754);  /* 005FD4 LDM */
L_005FD9: TICK();
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 005FD9 SEB */
    goto L_005FEC;  /* 005FDE JMP */
L_005FE1: TICK();
    mflag = 0;  /* 005FE1 CLM */
    X = rd16(ABS(0x021A));  /* 005FE2 LDX */
    if (X != 0) goto L_005FEC;  /* 005FE5 BNE */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 005FE7 CLB */
L_005FEC: TICK();
    goto L_006049;  /* 005FEC JMP */
L_005FEF: TICK();
    mflag = 0;  /* 005FEF CLM */
    if ((rd16(ABS(0x023E)) & 0x8000) == 0) goto L_006015;  /* 005FF0 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0010);  /* 005FF6 SEB */
    wr16(ABS(0x021A), 0x0754);  /* 005FFB LDM */
    X = rd16(ABS(0x021C));  /* 006000 LDX */
    /* 006003 CPX */
    if (X >= 0x0754) goto L_00600D;  /* 006006 BCS */
    wr16(ABS(0x021C), 0x0754);  /* 006008 LDM */
L_00600D: TICK();
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 00600D SEB */
    goto L_006049;  /* 006012 JMP */
L_006015: TICK();
    mflag = 0;  /* 006015 CLM */
    X = rd16(ABS(0x021A));  /* 006016 LDX */
    if (X != 0) goto L_006049;  /* 006019 BNE */
    if ((rd16(ABS(0x023A)) & 0x0008) == 0) goto L_006038;  /* 00601B BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0008);  /* 006021 CLB */
    wr16(ABS(0x021A), 0x061B);  /* 006026 LDM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 00602B CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x1000);  /* 006030 SEB */
    goto L_006049;  /* 006035 JMP */
L_006038: TICK();
    mflag = 0;  /* 006038 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0008);  /* 006039 SEB */
    X = rd16(ABS(0x021E));  /* 00603E LDX */
    wr16(ABS(0x021A), X);  /* 006041 STX */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 006044 SEB */
L_006049: TICK();
    mflag = 0;  /* 006049 CLM */
    Y = pull16();  /* 00604A PLY */
    S += 3; return;  /* 00604B RTL */
}

/* 00604C  callers: 0068AE */
void F_00604C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00604C LDA */
    push16(D);  /* 00604E PHD */
    D = A;  /* 00604F TAD */
    C = 0;  /* 006050 CLC */
    A = adc16(A, 0x0008);  /* 006051 ADC */
    push16(A);  /* 006054 PHA */
    mflag = 1;  /* 006055 SEM */
    SETLO(A, rd8(ABS(0x0230)));  /* 006056 LDA */
    if ((A & 0xFF) == 0) goto L_00605E;  /* 006059 BEQ */
    goto L_0062A7;  /* 00605B JMP */
L_00605E: TICK();
    SETLO(A, rd8(ABS(0x01EB)));  /* 00605E LDA */
    SETLO(A, A - 1);  /* 006061 DEC */
    if ((A & 0xFF) != 0) goto L_006078;  /* 006062 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 006064 LDA */
    if ((A & 0xFF) != 0) goto L_006075;  /* 006067 BNE */
    SETLO(A, rd8(ABS(0x0227)));  /* 006069 LDA */
    if ((A & 0xFF) != 0) goto L_006075;  /* 00606C BNE */
    wr8(ABS(0x01EB), (uint8_t)A);  /* 00606E STA */
    wr8(ABS(0x0230), 0x3F);  /* 006071 LDM */
L_006075: TICK();
    goto L_0062A7;  /* 006075 JMP */
L_006078: TICK();
    mflag = 1;  /* 006078 SEM */
    /* 006079 CMP */
    if ((A & 0xFF) == 0xFF) goto L_006080;  /* 00607B BEQ */
    goto L_0062A4;  /* 00607D JMP */
L_006080: TICK();
    wr8(ABS(0x0230), 0x20);  /* 006080 LDM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0x10) goto L_00608C;  /* 006084 BBS */
    goto L_0062A1;  /* 006089 JMP */
L_00608C: TICK();
    mflag = 0;  /* 00608C CLM */
    if ((rd16(ABS(0x0240)) & 0x0001) == 0) goto L_00609E;  /* 00608D BBC */
    A = rd16(ABS(0x0228));  /* 006093 LDA */
    mflag = 1;  /* 006096 SEM */
    /* 006097 CMP */
    if ((A & 0xFF) == 0x03) goto L_00609E;  /* 006099 BEQ */
    goto L_0062A1;  /* 00609B JMP */
L_00609E: TICK();
    Y = 0x0025;  /* 00609E LDY */
    mflag = 0;  /* 0060A1 CLM */
    S -= 3; F_003310();  /* 0060A2 JSRL */
    Y = 0x0005;  /* 0060A6 LDY */
    S -= 3; F_008DC6();  /* 0060A9 JSRL */
    A = rd16(ABS(0x01ED));  /* 0060AD LDA */
    mflag = 1;  /* 0060B0 SEM */
    t = rd8(ABS(AD5));  /* 0060B1 CMP [AN5 RSSI] */
    if ((A & 0xFF) >= t) goto L_0060BE;  /* 0060B4 BCS */
    mflag = 0;  /* 0060B6 CLM */
    wr16(DP(0x06), 0x0000);  /* 0060B7 LDM */
    goto L_0060E7;  /* 0060BB JMP */
L_0060BE: TICK();
    mflag = 0;  /* 0060BE CLM */
    A = rd16(ABS(0x01EE));  /* 0060BF LDA */
    mflag = 1;  /* 0060C2 SEM */
    t = rd8(ABS(AD5));  /* 0060C3 CMP [AN5 RSSI] */
    if ((A & 0xFF) >= t) goto L_0060D0;  /* 0060C6 BCS */
    mflag = 0;  /* 0060C8 CLM */
    wr16(DP(0x06), 0x0001);  /* 0060C9 LDM */
    goto L_0060E7;  /* 0060CD JMP */
L_0060D0: TICK();
    mflag = 0;  /* 0060D0 CLM */
    A = rd16(ABS(0x01EF));  /* 0060D1 LDA */
    mflag = 1;  /* 0060D4 SEM */
    t = rd8(ABS(AD5));  /* 0060D5 CMP [AN5 RSSI] */
    if ((A & 0xFF) >= t) goto L_0060E2;  /* 0060D8 BCS */
    mflag = 0;  /* 0060DA CLM */
    wr16(DP(0x06), 0x0002);  /* 0060DB LDM */
    goto L_0060E7;  /* 0060DF JMP */
L_0060E2: TICK();
    mflag = 0;  /* 0060E2 CLM */
    wr16(DP(0x06), 0x0003);  /* 0060E3 LDM */
L_0060E7: TICK();
    X = rd16(DP(0x06));  /* 0060E7 LDX */
    A = rd16((ABS(0x01ED) + X));  /* 0060E9 LDA */
    A &= 0x00FF;  /* 0060EC AND */
    wr16(DP(0x08), A);  /* 0060EF STA */
    A = rd16((ABS(0x01EC) + X));  /* 0060F1 LDA */
    A &= 0x00FF;  /* 0060F4 AND */
    C = 1;  /* 0060F7 SEC */
    A = sbc16(A, rd16(DP(0x08)));  /* 0060F8 SBC */
    wr16(DP(0x04), A);  /* 0060FA STA */
    mflag = 1;  /* 0060FC SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 0060FD LDA [AN5 RSSI] */
    t = rd8((ABS(0x01EC) + X));  /* 006100 CMP */
    if ((A & 0xFF) >= t) goto L_00616F;  /* 006103 BCS */
    mflag = 0;  /* 006105 CLM */
    A = rd16(DP(0x04));  /* 006106 LDA */
    B = 0x0000;  /* 006108 LDA */
    /* 00610C CMP */
    if (A < 0x8000) goto L_006113;  /* 00610F BCC */
    B -= 1;  /* 006111 DEC */
L_006113: TICK();
    wr16(DP(0x08), A);  /* 006113 STA */
    mflag = 1;  /* 006115 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 006116 LDA [AN5 RSSI] */
    mflag = 0;  /* 006119 CLM */
    A &= 0x00FF;  /* 00611A AND */
    wr16(DP(0x0A), A);  /* 00611D STA */
    A = rd16((ABS(0x01EC) + X));  /* 00611F LDA */
    A &= 0x00FF;  /* 006122 AND */
    C = 1;  /* 006125 SEC */
    A = sbc16(A, rd16(DP(0x0A)));  /* 006126 SBC */
    wr16(DP(0x0A), B);  /* 006128 STA */
    B = 0x0000;  /* 00612B LDA */
    /* 00612F CMP */
    if (A < 0x8000) goto L_006136;  /* 006132 BCC */
    B -= 1;  /* 006134 DEC */
L_006136: TICK();
    wr16(DP(0x0E), 0x0000);  /* 006136 LDM */
    wr16(DP(0x0C), 0x000A);  /* 00613A LDM */
    X = 0x000C;  /* 00613E LDX */
    S -= 3; rt_lmul_01E3EA();  /* 006141 JSRL */
    X = 0x0008;  /* 006145 LDX */
    S -= 3; rt_lmod_01E214();  /* 006148 JSRL */
    wr16(DP(0x02), A);  /* 00614C STA */
    A = rd16(DP(0x04));  /* 00614E LDA */
    B = 0x0002;  /* 006150 LDA */
    S -= 3; F_00FF8C();  /* 006154 JSRL */
    C = 1;  /* 006158 SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 006159 SBC */
    if (!V) goto L_006160;  /* 00615B BVC */
    A ^= 0x8000;  /* 00615D EOR */
    N = ((A & 0x8000) != 0);
L_006160: TICK();
    if (!N) goto L_006167;  /* 006160 BPL */
    A = rd16(DP(0x04));  /* 006162 LDA */
    goto L_00616A;  /* 006164 JMP */
L_006167: TICK();
    A = 0x0000;  /* 006167 LDA */
L_00616A: TICK();
    wr16(DP(0x04), A);  /* 00616A STA */
    goto L_0061E0;  /* 00616C JMP */
L_00616F: TICK();
    mflag = 0;  /* 00616F CLM */
    A = rd16(DP(0x04));  /* 006170 LDA */
    B = 0x0000;  /* 006172 LDA */
    /* 006176 CMP */
    if (A < 0x8000) goto L_00617D;  /* 006179 BCC */
    B -= 1;  /* 00617B DEC */
L_00617D: TICK();
    X = rd16(DP(0x06));  /* 00617D LDX */
    wr16(DP(0x08), A);  /* 00617F STA */
    A = rd16((ABS(0x01EC) + X));  /* 006181 LDA */
    A &= 0x00FF;  /* 006184 AND */
    A ^= 0xFFFF;  /* 006187 EOR */
    A += 1;  /* 00618A INC */
    wr16(DP(0x0A), A);  /* 00618B STA */
    mflag = 1;  /* 00618D SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00618E LDA [AN5 RSSI] */
    mflag = 0;  /* 006191 CLM */
    A &= 0x00FF;  /* 006192 AND */
    C = 0;  /* 006195 CLC */
    A = adc16(A, rd16(DP(0x0A)));  /* 006196 ADC */
    wr16(DP(0x0A), B);  /* 006198 STA */
    B = 0x0000;  /* 00619B LDA */
    /* 00619F CMP */
    if (A < 0x8000) goto L_0061A6;  /* 0061A2 BCC */
    B -= 1;  /* 0061A4 DEC */
L_0061A6: TICK();
    wr16(DP(0x0E), 0x0000);  /* 0061A6 LDM */
    wr16(DP(0x0C), 0x000A);  /* 0061AA LDM */
    X = 0x000C;  /* 0061AE LDX */
    S -= 3; rt_lmul_01E3EA();  /* 0061B1 JSRL */
    X = 0x0008;  /* 0061B5 LDX */
    S -= 3; rt_lmod_01E214();  /* 0061B8 JSRL */
    wr16(DP(0x02), A);  /* 0061BC STA */
    A = rd16(DP(0x04));  /* 0061BE LDA */
    B = 0x0002;  /* 0061C0 LDA */
    S -= 3; F_00FF8C();  /* 0061C4 JSRL */
    C = 1;  /* 0061C8 SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 0061C9 SBC */
    if (!V) goto L_0061D0;  /* 0061CB BVC */
    A ^= 0x8000;  /* 0061CD EOR */
    N = ((A & 0x8000) != 0);
L_0061D0: TICK();
    if (!N) goto L_0061DB;  /* 0061D0 BPL */
    A = 0x0000;  /* 0061D2 LDA */
    C = 1;  /* 0061D5 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 0061D6 SBC */
    goto L_0061DE;  /* 0061D8 JMP */
L_0061DB: TICK();
    A = 0x0000;  /* 0061DB LDA */
L_0061DE: TICK();
    wr16(DP(0x04), A);  /* 0061DE STA */
L_0061E0: TICK();
    X = rd16(DP(0x06));  /* 0061E0 LDX */
    A = rd16((ABS(0x01ED) + X));  /* 0061E2 LDA */
    A &= 0x00FF;  /* 0061E5 AND */
    wr16(DP(0x08), A);  /* 0061E8 STA */
    A = rd16((ABS(0x01EC) + X));  /* 0061EA LDA */
    A &= 0x00FF;  /* 0061ED AND */
    C = 1;  /* 0061F0 SEC */
    A = sbc16(A, rd16(DP(0x08)));  /* 0061F1 SBC */
    B = 0x0000;  /* 0061F3 LDA */
    /* 0061F7 CMP */
    if (A < 0x8000) goto L_0061FE;  /* 0061FA BCC */
    B -= 1;  /* 0061FC DEC */
L_0061FE: TICK();
    wr16(DP(0x08), A);  /* 0061FE STA */
    mflag = 1;  /* 006200 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 006201 LDA [AN5 RSSI] */
    mflag = 0;  /* 006204 CLM */
    A &= 0x00FF;  /* 006205 AND */
    wr16(DP(0x0A), A);  /* 006208 STA */
    A = rd16((ABS(0x01EC) + X));  /* 00620A LDA */
    A &= 0x00FF;  /* 00620D AND */
    C = 1;  /* 006210 SEC */
    A = sbc16(A, rd16(DP(0x0A)));  /* 006211 SBC */
    wr16(DP(0x0A), B);  /* 006213 STA */
    B = 0x0000;  /* 006216 LDA */
    /* 00621A CMP */
    if (A < 0x8000) goto L_006221;  /* 00621D BCC */
    B -= 1;  /* 00621F DEC */
L_006221: TICK();
    wr16(DP(0x0E), 0x0000);  /* 006221 LDM */
    wr16(DP(0x0C), 0x000A);  /* 006225 LDM */
    X = 0x000C;  /* 006229 LDX */
    S -= 3; rt_lmul_01E3EA();  /* 00622C JSRL */
    wr16(DP(0x0C), A);  /* 006230 STA */
    A = rd16(DP(0x04));  /* 006232 LDA */
    wr16(DP(0x0E), B);  /* 006234 STA */
    B = 0x0000;  /* 006237 LDA */
    /* 00623B CMP */
    if (A < 0x8000) goto L_006242;  /* 00623E BCC */
    B -= 1;  /* 006240 DEC */
L_006242: TICK();
    C = 0;  /* 006242 CLC */
    A = adc16(A, rd16(DP(0x0C)));  /* 006243 ADC */
    B = adc16(B, rd16(DP(0x0E)));  /* 006245 ADC */
    X = 0x0008;  /* 006248 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 00624B JSRL */
    wr16(DP(0x08), A);  /* 00624F STA */
    A = rd16(DP(0x06));  /* 006251 LDA */
    mpy16(0x000A);  /* 006253 MPY */
    C = 0;  /* 006257 CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 006258 ADC */
    C = 0;  /* 00625A CLC */
    A = adc16(A, 0x0050);  /* 00625B ADC */
    wr16(DP(0x00), A);  /* 00625E STA */
    if (!N) goto L_006272;  /* 006260 BPL */
    A = 0x0000;  /* 006262 LDA */
    C = 1;  /* 006265 SEC */
    A = sbc16(A, rd16(DP(0x00)));  /* 006266 SBC */
    wr16(DP(0x00), A);  /* 006268 STA */
    mflag = 1;  /* 00626A SEM */
    wr8(ABS(0x0252), 0x2B);  /* 00626B LDM */
    goto L_006277;  /* 00626F JMP */
L_006272: TICK();
    mflag = 1;  /* 006272 SEM */
    wr8(ABS(0x0252), 0x2D);  /* 006273 LDM */
L_006277: TICK();
    mflag = 0;  /* 006277 CLM */
    A = rd16(DP(0x00));  /* 006278 LDA */
    B = 0x0064;  /* 00627A LDA */
    S -= 3; F_00FF8C();  /* 00627E JSRL */
    C = 0;  /* 006282 CLC */
    mflag = 1;  /* 006283 SEM */
    SETLO(A, adc8(A, 0x30));  /* 006284 ADC */
    wr8(ABS(0x0253), (uint8_t)A);  /* 006286 STA */
    mflag = 0;  /* 006289 CLM */
    A = rd16(DP(0x00));  /* 00628A LDA */
    B = 0x0064;  /* 00628C LDA */
    S -= 3; F_01E350();  /* 006290 JSRL */
    wr16(DP(0x08), A);  /* 006294 STA */
    Y = 0x0002;  /* 006296 LDY */
    S -= 3; F_0032CE();  /* 006299 JSRL */
    S -= 3; F_00828C();  /* 00629D JSRL */
L_0062A1: TICK();
    goto L_0062A7;  /* 0062A1 JMP */
L_0062A4: TICK();
    goto L_0062A7;  /* 0062A4 JMP */
L_0062A7: TICK();
    mflag = 0;  /* 0062A7 CLM */
    Y = pull16();  /* 0062A8 PLY */
    D = pull16();  /* 0062A9 PLD */
    S += 3; return;  /* 0062AA RTL */
}

/* 0062AB  callers: 0068C4, 00EC9B */
void F_0062AB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0062AB LDA */
    push16(D);  /* 0062AD PHD */
    D = A;  /* 0062AE TAD */
    C = 0;  /* 0062AF CLC */
    A = adc16(A, 0x000E);  /* 0062B0 ADC */
    push16(A);  /* 0062B3 PHA */
    A = rd16(ABS(0x026C));  /* 0062B4 LDA */
    A &= 0x00FF;  /* 0062B7 AND */
    wr16(DP(0x00), A);  /* 0062BA STA */
    mflag = 1;  /* 0062BC SEM */
    wr8(DP(0x0D), 0x00);  /* 0062BD LDM */
    SETLO(A, rd8(ABS(0x0264)));  /* 0062C0 LDA */
    /* 0062C3 CMP */
    if ((A & 0xFF) != 0x0E) goto L_0062CA;  /* 0062C5 BNE */
    goto L_00651F;  /* 0062C7 JMP */
L_0062CA: TICK();
    mflag = 0;  /* 0062CA CLM */
    X = rd16(DP(0x00));  /* 0062CB LDX */
    /* 0062CD CPX */
    if (X >= 0x0002) goto L_0062D5;  /* 0062D0 BCS */
    goto L_00651F;  /* 0062D2 JMP */
L_0062D5: TICK();
    A = rd16(ABS(0x0343));  /* 0062D5 LDA */
    mflag = 1;  /* 0062D8 SEM */
    SETLO(A, A & 0x0F);  /* 0062D9 AND */
    mflag = 0;  /* 0062DB CLM */
    A &= 0x00FF;  /* 0062DC AND */
    wr16(DP(0x0E), A);  /* 0062DF STA */
    A = X;  /* 0062E1 TXA */
    A += 1;  /* 0062E2 INC */
    t = rd16(DP(0x0E));  /* 0062E3 CMP */
    if (A >= t) goto L_0062EA;  /* 0062E5 BCS */
    goto L_00651F;  /* 0062E7 JMP */
L_0062EA: TICK();
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x8000);  /* 0062EA CLB */
    wr16(DP(0x02), 0x0000);  /* 0062EF LDM */
L_0062F3: TICK();
    X = rd16(DP(0x02));  /* 0062F3 LDX */
    /* 0062F5 CPX */
    if (X >= 0x0008) goto L_00631E;  /* 0062F8 BCS */
    A = rd16((ABS(0x0264) + X));  /* 0062FA LDA */
    mflag = 1;  /* 0062FD SEM */
    wr8(DP(0x0C), (uint8_t)A);  /* 0062FE STA */
    mflag = 0;  /* 006300 CLM */
    Y = X;  /* 006301 TXY */
    if (Y == 0) goto L_00630F;  /* 006302 BEQ */
    A = rd16(DP(0x0C));  /* 006304 LDA */
    mflag = 1;  /* 006306 SEM */
    /* 006307 CMP */
    if ((A & 0xFF) != 0x0E) goto L_00630F;  /* 006309 BNE */
    SETLO(A, rd8(DP(0x03 + X)));  /* 00630B LDA */
    wr8(DP(0x0C), (uint8_t)A);  /* 00630D STA */
L_00630F: TICK();
    X = rd16(DP(0x02));  /* 00630F LDX */
    mflag = 0;  /* 006311 CLM */
    A = rd16(DP(0x0C));  /* 006312 LDA */
    mflag = 1;  /* 006314 SEM */
    wr8(DP(0x04 + X), (uint8_t)A);  /* 006315 STA */
    X += 1;  /* 006317 INX */
    wr16(DP(0x02), X);  /* 006318 STX */
    mflag = 0;  /* 00631A CLM */
    goto L_0062F3;  /* 00631B JMP */
L_00631E: TICK();
    mflag = 0;  /* 00631E CLM */
    A = rd16(ABS(0x0343));  /* 00631F LDA */
    mflag = 1;  /* 006322 SEM */
    SETLO(A, A & 0x0F);  /* 006323 AND */
    mflag = 0;  /* 006325 CLM */
    A &= 0x00FF;  /* 006326 AND */
    wr16(DP(0x0E), A);  /* 006329 STA */
    A = rd16(DP(0x00));  /* 00632B LDA */
    A -= 1;  /* 00632D DEC */
    t = rd16(DP(0x0E));  /* 00632E CMP */
    if (A != t) goto L_00634B;  /* 006330 BNE */
    X = rd16(DP(0x00));  /* 006332 LDX */
    A = rd16(DP(0x03 + X));  /* 006334 LDA */
    mflag = 1;  /* 006336 SEM */
    /* 006337 CMP */
    if ((A & 0xFF) != 0x0A) goto L_00634B;  /* 006339 BNE */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_006348;  /* 00633B BBC */
    if ((rd8(ABS(0x033F)) & 0x02) == 0) goto L_006348;  /* 006340 BBC */
    wr8(DP(0x0D), 0x02);  /* 006345 LDM */
L_006348: TICK();
    goto L_0063C4;  /* 006348 JMP */
L_00634B: TICK();
    mflag = 0;  /* 00634B CLM */
    A = rd16(ABS(0x0343));  /* 00634C LDA */
    mflag = 1;  /* 00634F SEM */
    SETLO(A, A & 0x0F);  /* 006350 AND */
    mflag = 0;  /* 006352 CLM */
    A &= 0x00FF;  /* 006353 AND */
    wr16(DP(0x0E), A);  /* 006356 STA */
    A = rd16(DP(0x00));  /* 006358 LDA */
    A += 1;  /* 00635A INC */
    t = rd16(DP(0x0E));  /* 00635B CMP */
    if (A != t) goto L_00636C;  /* 00635D BNE */
    if ((rd16(ABS(0x023E)) & 0x0040) == 0x0040) goto L_00636C;  /* 00635F BBS */
    mflag = 1;  /* 006365 SEM */
    wr8(DP(0x0D), 0x01);  /* 006366 LDM */
    goto L_0063C4;  /* 006369 JMP */
L_00636C: TICK();
    mflag = 0;  /* 00636C CLM */
    A = rd16(ABS(0x0343));  /* 00636D LDA */
    mflag = 1;  /* 006370 SEM */
    SETLO(A, A & 0x0F);  /* 006371 AND */
    mflag = 0;  /* 006373 CLM */
    A &= 0x00FF;  /* 006374 AND */
    t = rd16(DP(0x00));  /* 006377 CMP */
    if (A != t) goto L_0063C4;  /* 006379 BNE */
    mflag = 1;  /* 00637B SEM */
    wr8(DP(0x0D), 0x04);  /* 00637C LDM */
    mflag = 0;  /* 00637F CLM */
    wr16(DP(0x02), 0x0000);  /* 006380 LDM */
L_006384: TICK();
    X = rd16(DP(0x02));  /* 006384 LDX */
    t = rd16(DP(0x00));  /* 006386 CPX */
    if (X >= t) goto L_0063B3;  /* 006388 BCS */
    A = rd16(DP(0x0D));  /* 00638A LDA */
    mflag = 1;  /* 00638C SEM */
    /* 00638D CMP */
    if ((A & 0xFF) == 0x05) goto L_0063AD;  /* 00638F BEQ */
    Y = X;  /* 006391 TXY */
    mflag = 0;  /* 006392 CLM */
    S -= 3; F_0069EF();  /* 006393 JSRL */
    X = rd16(DP(0x02));  /* 006397 LDX */
    t = rd8(DP(0x04 + X));  /* 006399 CMP */
    if ((A & 0xFF) == t) goto L_0063AD;  /* 00639B BEQ */
    SETLO(A, rd8(DP(0x04 + X)));  /* 00639D LDA */
    t = rd8(ABS(0x020E));  /* 00639F CMP */
    if ((A & 0xFF) == t) goto L_0063A9;  /* 0063A2 BEQ */
    SETLO(A, 0x05);  /* 0063A4 LDA */
    goto L_0063AB;  /* 0063A6 JMP */
L_0063A9: TICK();
    SETLO(A, 0x03);  /* 0063A9 LDA */
L_0063AB: TICK();
    wr8(DP(0x0D), (uint8_t)A);  /* 0063AB STA */
L_0063AD: TICK();
    mflag = 0;  /* 0063AD CLM */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 0063AE INC */
    goto L_006384;  /* 0063B0 JMP */
L_0063B3: TICK();
    mflag = 1;  /* 0063B3 SEM */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_0063C4;  /* 0063B4 BBC */
    mflag = 0;  /* 0063B9 CLM */
    if ((rd16(ABS(0x023A)) & 0x1000) == 0x1000) goto L_0063C4;  /* 0063BA BBS */
    mflag = 1;  /* 0063C0 SEM */
    wr8(DP(0x0D), 0x01);  /* 0063C1 LDM */
L_0063C4: TICK();
    mflag = 1;  /* 0063C4 SEM */
    SETLO(A, rd8(DP(0x0D)));  /* 0063C5 LDA */
    if ((A & 0xFF) != 0) goto L_0063CC;  /* 0063C7 BNE */
    goto L_006518;  /* 0063C9 JMP */
L_0063CC: TICK();
    /* 0063CC CMP */
    if ((A & 0xFF) != 0x01) goto L_0063D3;  /* 0063CE BNE */
    goto L_006518;  /* 0063D0 JMP */
L_0063D3: TICK();
    /* 0063D3 CMP */
    if ((A & 0xFF) != 0x05) goto L_0063DA;  /* 0063D5 BNE */
    goto L_006486;  /* 0063D7 JMP */
L_0063DA: TICK();
    mflag = 0;  /* 0063DA CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x8000);  /* 0063DB SEB */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_0063ED;  /* 0063E0 BBC */
    Y = 0x0000;  /* 0063E6 LDY */
    S -= 3; F_00927F();  /* 0063E9 JSRL */
L_0063ED: TICK();
    A = rd16(DP(0x0D));  /* 0063ED LDA */
    mflag = 1;  /* 0063EF SEM */
    /* 0063F0 CMP */
    if ((A & 0xFF) != 0x02) goto L_006404;  /* 0063F2 BNE */
    Y = 0x000C;  /* 0063F4 LDY */
    mflag = 0;  /* 0063F7 CLM */
    S -= 3; F_0080AA();  /* 0063F8 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x4000);  /* 0063FC SEB */
    goto L_006471;  /* 006401 JMP */
L_006404: TICK();
    mflag = 0;  /* 006404 CLM */
    A = rd16(DP(0x0D));  /* 006405 LDA */
    mflag = 1;  /* 006407 SEM */
    /* 006408 CMP */
    if ((A & 0xFF) != 0x03) goto L_00641C;  /* 00640A BNE */
    Y = 0x000F;  /* 00640C LDY */
    mflag = 0;  /* 00640F CLM */
    S -= 3; F_0080AA();  /* 006410 JSRL */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0010);  /* 006414 SEB */
    goto L_006471;  /* 006419 JMP */
L_00641C: TICK();
    mflag = 0;  /* 00641C CLM */
    A = rd16(DP(0x0D));  /* 00641D LDA */
    mflag = 1;  /* 00641F SEM */
    /* 006420 CMP */
    if ((A & 0xFF) != 0x04) goto L_006471;  /* 006422 BNE */
    mflag = 0;  /* 006424 CLM */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_006471;  /* 006425 BBS */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0010);  /* 00642B SEB */
    Y = 0x0000;  /* 006430 LDY */
    S -= 3; F_006A25();  /* 006433 JSRL */
    /* 006437 CMP */
    if ((A & 0xFF) == 0x0F) goto L_006469;  /* 006439 BEQ */
    if ((rd8(ABS(0x034E)) & 0x08) == 0) goto L_006460;  /* 00643B BBC */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_006460;  /* 006440 BBC */
    mflag = 0;  /* 006445 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0080);  /* 006446 SEB */
    if ((rd16(ABS(0x023E)) & 0x0040) == 0) goto L_006457;  /* 00644B BBC */
    A = 0x000A;  /* 006451 LDA */
    goto L_00645A;  /* 006454 JMP */
L_006457: TICK();
    A = 0x0010;  /* 006457 LDA */
L_00645A: TICK();
    wr16(ABS(0x0222), A);  /* 00645A STA */
    goto L_006466;  /* 00645D JMP */
L_006460: TICK();
    mflag = 0;  /* 006460 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0002);  /* 006461 SEB */
L_006466: TICK();
    goto L_006471;  /* 006466 JMP */
L_006469: TICK();
    Y = 0x000E;  /* 006469 LDY */
    mflag = 0;  /* 00646C CLM */
    S -= 3; F_0080AA();  /* 00646D JSRL */
L_006471: TICK();
    mflag = 1;  /* 006471 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_006486;  /* 006472 BBC */
    mflag = 0;  /* 006477 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0800);  /* 006478 SEB */
    S -= 3; cbus_write_sel2_00755E();  /* 00647D JSRL */
    mflag = 1;  /* 006481 SEM */
    wr8(ABS(0x01B7), 0x3F);  /* 006482 LDM */
L_006486: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00648E;  /* 006486 BBS */
    goto L_006518;  /* 00648B JMP */
L_00648E: TICK();
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_006496;  /* 00648E BBS */
    goto L_006518;  /* 006493 JMP */
L_006496: TICK();
    mflag = 0;  /* 006496 CLM */
    wr16(DP(0x02), 0x0000);  /* 006497 LDM */
L_00649B: TICK();
    X = rd16(DP(0x02));  /* 00649B LDX */
    /* 00649D CPX */
    if (X >= 0x0008) goto L_0064F0;  /* 0064A0 BCS */
    A = rd16(DP(0x04 + X));  /* 0064A2 LDA */
    mflag = 1;  /* 0064A4 SEM */
    wr8(DP(0x0C), (uint8_t)A);  /* 0064A5 STA */
    /* 0064A7 CMP */
    if ((A & 0xFF) == 0x0F) goto L_0064D6;  /* 0064A9 BEQ */
    t = rd8(ABS(0x020E));  /* 0064AB CMP */
    if ((A & 0xFF) != t) goto L_0064B5;  /* 0064AE BNE */
    SETLO(A, 0x47);  /* 0064B0 LDA */
    goto L_0064CF;  /* 0064B2 JMP */
L_0064B5: TICK();
    SETLO(A, rd8(DP(0x0C)));  /* 0064B5 LDA */
    C = ((A & 0xFF) >= 0x0A);  /* 0064B7 CMP */
    if (!C) goto L_0064C5;  /* 0064B9 BCC */
    mflag = 0;  /* 0064BB CLM */
    A &= 0x00FF;  /* 0064BC AND */
    A = adc16(A, 0x0036);  /* 0064BF ADC */
    goto L_0064CF;  /* 0064C2 JMP */
L_0064C5: TICK();
    mflag = 0;  /* 0064C5 CLM */
    A = rd16(DP(0x0C));  /* 0064C6 LDA */
    A &= 0x00FF;  /* 0064C8 AND */
    C = 0;  /* 0064CB CLC */
    A = adc16(A, 0x0030);  /* 0064CC ADC */
L_0064CF: TICK();
    mflag = 1;  /* 0064CF SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 0064D0 STA */
    goto L_0064DD;  /* 0064D3 JMP */
L_0064D6: TICK();
    X = rd16(DP(0x02));  /* 0064D6 LDX */
    mflag = 1;  /* 0064D8 SEM */
    wr8((ABS(0x0252) + X), 0x20);  /* 0064D9 LDM */
L_0064DD: TICK();
    mflag = 0;  /* 0064DD CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0) goto L_0064EB;  /* 0064DE BBC */
    Y = rd16((ABS(0x0252) + X));  /* 0064E4 LDY */
    S -= 3; F_007627();  /* 0064E7 JSRL */
L_0064EB: TICK();
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 0064EB INC */
    goto L_00649B;  /* 0064ED JMP */
L_0064F0: TICK();
    mflag = 1;  /* 0064F0 SEM */
    wr8(ABS(0x0231), 0x14);  /* 0064F1 LDM */
    wr8(ABS(0x0230), 0xFB);  /* 0064F5 LDM */
    wr8(ABS(0x0293), 0x00);  /* 0064F9 LDM */
    mflag = 0;  /* 0064FD CLM */
    S -= 3; F_00828C();  /* 0064FE JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 006502 SEB */
    mflag = 1;  /* 006507 SEM */
    wr8(ABS(0x0232), 0x3F);  /* 006508 LDM */
    mflag = 0;  /* 00650C CLM */
    S -= 3; F_007A06();  /* 00650D JSRL */
    Y = 0x0000;  /* 006511 LDY */
    S -= 3; F_007809();  /* 006514 JSRL */
L_006518: TICK();
    Y = rd16(DP(0x0D));  /* 006518 LDY */
    mflag = 0;  /* 00651A CLM */
    S -= 3; F_01E08A();  /* 00651B JSRL */
L_00651F: TICK();
    mflag = 0;  /* 00651F CLM */
    wr16(DP(0x02), 0x0000);  /* 006520 LDM */
L_006524: TICK();
    X = rd16(DP(0x02));  /* 006524 LDX */
    /* 006526 CPX */
    if (X >= 0x0008) goto L_006537;  /* 006529 BCS */
    mflag = 1;  /* 00652B SEM */
    wr8((ABS(0x0264) + X), 0x0F);  /* 00652C LDM */
    X += 1;  /* 006530 INX */
    wr16(DP(0x02), X);  /* 006531 STX */
    mflag = 0;  /* 006533 CLM */
    goto L_006524;  /* 006534 JMP */
L_006537: TICK();
    mflag = 0;  /* 006537 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0010);  /* 006538 CLB */
    mflag = 1;  /* 00653D SEM */
    wr8(ABS(0x026C), 0x00);  /* 00653E LDM */
    wr8(ABS(0x026D), 0x0F);  /* 006542 LDM */
    wr8(ABS(0x026E), 0x0F);  /* 006546 LDM */
    wr8(ABS(0x026F), 0x0F);  /* 00654A LDM */
    wr8(ABS(0x0270), 0x0F);  /* 00654E LDM */
    wr8(ABS(0x0271), 0x0F);  /* 006552 LDM */
    wr8(ABS(0x039B), 0x0F);  /* 006556 LDM */
    wr8(ABS(0x039C), 0x00);  /* 00655A LDM */
    mflag = 0;  /* 00655E CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0100);  /* 00655F CLB */
    Y = pull16();  /* 006564 PLY */
    D = pull16();  /* 006565 PLD */
    S += 3; return;  /* 006566 RTL */
}

/* 006567  callers: 006783 */
void F_006567(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006567 LDA */
    push16(D);  /* 006569 PHD */
    D = A;  /* 00656A TAD */
    C = 0;  /* 00656B CLC */
    A = adc16(A, 0x0007);  /* 00656C ADC */
    push16(A);  /* 00656F PHA */
    wr16(DP(0x00), Y);  /* 006570 STY */
    wr16(DP(0x02), B);  /* 006572 STA */
    if ((rd16(ABS(0x0240)) & 0x0004) == 0x0004) goto L_00657E;  /* 006575 BBS */
    goto L_00668B;  /* 00657B JMP */
L_00657E: TICK();
    mflag = 1;  /* 00657E SEM */
    wr8(ABS(0x0231), 0x15);  /* 00657F LDM */
    wr8(ABS(0x0230), 0x00);  /* 006583 LDM */
    wr8(ABS(0x0227), 0x0E);  /* 006587 LDM */
L_00658B: TICK();
    SETLO(A, rd8(LIND(0x00)));  /* 00658B LDA */
    /* 00658D CMP */
    if ((A & 0xFF) == 0x0D) goto L_006594;  /* 00658F BEQ */
    goto L_00668B;  /* 006591 JMP */
L_006594: TICK();
    wr8(ABS(WDT), 0x00);  /* 006594 LDM */
    mflag = 0;  /* 006598 CLM */
    ram_trampoline_087A();  /* 006599 JSRL */
    if ((rd16(ABS(0x023A)) & 0x0002) == 0) goto L_0065B7;  /* 00659D BBC */
    S -= 3; F_0050DA();  /* 0065A3 JSRL */
    A = rd16(ABS(0x0226));  /* 0065A7 LDA */
    mflag = 1;  /* 0065AA SEM */
    /* 0065AB CMP */
    if ((A & 0xFF) == 0x0F) goto L_0065B7;  /* 0065AD BEQ */
    Y = 0x0000;  /* 0065AF LDY */
    mflag = 0;  /* 0065B2 CLM */
    S -= 3; F_0080AA();  /* 0065B3 JSRL */
L_0065B7: TICK();
    mflag = 0;  /* 0065B7 CLM */
    A = rd16(ABS(0x0226));  /* 0065B8 LDA */
    mflag = 1;  /* 0065BB SEM */
    /* 0065BC CMP */
    if ((A & 0xFF) != 0x0F) goto L_0065D2;  /* 0065BE BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 0065C0 LDA */
    /* 0065C3 CMP */
    if ((A & 0xFF) == 0x15) goto L_0065CA;  /* 0065C5 BEQ */
    goto L_00666F;  /* 0065C7 JMP */
L_0065CA: TICK();
    SETLO(A, rd8(ABS(0x0230)));  /* 0065CA LDA */
    if ((A & 0xFF) == 0) goto L_0065D2;  /* 0065CD BEQ */
    goto L_00666F;  /* 0065CF JMP */
L_0065D2: TICK();
    Y = 0x0009;  /* 0065D2 LDY */
    mflag = 0;  /* 0065D5 CLM */
    S -= 3; F_003310();  /* 0065D6 JSRL */
    A = rd16(ABS(0x0293));  /* 0065DA LDA */
    A &= 0x00FF;  /* 0065DD AND */
    wr16(DP(0x05), A);  /* 0065E0 STA */
    wr16(DP(0x03), 0x0000);  /* 0065E2 LDM */
L_0065E6: TICK();
    A = rd16(DP(0x03));  /* 0065E6 LDA */
    C = 1;  /* 0065E8 SEC */
    A = sbc16(A, rd16(DP(0x05)));  /* 0065E9 SBC */
    if (!V) goto L_0065F0;  /* 0065EB BVC */
    A ^= 0x8000;  /* 0065ED EOR */
    N = ((A & 0x8000) != 0);
L_0065F0: TICK();
    if (!N) goto L_00660B;  /* 0065F0 BPL */
    X = rd16(DP(0x03));  /* 0065F2 LDX */
    A = X;  /* 0065F4 TXA */
    C = 1;  /* 0065F5 SEC */
    A = sbc16(A, rd16(DP(0x05)));  /* 0065F6 SBC */
    C = 0;  /* 0065F8 CLC */
    A = adc16(A, 0x0008);  /* 0065F9 ADC */
    Y = A;  /* 0065FC TAY */
    A = rd16((ABS(0x0283) + X));  /* 0065FD LDA */
    mflag = 1;  /* 006600 SEM */
    wr8((ABS(0x0252) + Y), (uint8_t)A);  /* 006601 STA */
    X += 1;  /* 006604 INX */
    wr16(DP(0x03), X);  /* 006605 STX */
    mflag = 0;  /* 006607 CLM */
    goto L_0065E6;  /* 006608 JMP */
L_00660B: TICK();
    mflag = 0;  /* 00660B CLM */
    A = rd16(ABS(0x0226));  /* 00660C LDA */
    mflag = 1;  /* 00660F SEM */
    /* 006610 CMP */
    if ((A & 0xFF) != 0x64) goto L_006639;  /* 006612 BNE */
    wr8(ABS(0x0231), 0x15);  /* 006614 LDM */
    wr8(ABS(0x0230), 0x3F);  /* 006618 LDM */
    mflag = 0;  /* 00661C CLM */
    X = rd16(DP(0x05));  /* 00661D LDX */
    /* 00661F CPX */
    if (X != 0x0005) goto L_006636;  /* 006622 BNE */
    Y = X;  /* 006624 TXY */
    S -= 3; F_004721();  /* 006625 JSRL */
    if ((rd16(ABS(0x0240)) & 0x0004) == 0) goto L_006636;  /* 006629 BBC */
    Y = 0x002B;  /* 00662F LDY */
    S -= 3; F_003310();  /* 006632 JSRL */
L_006636: TICK();
    goto L_006665;  /* 006636 JMP */
L_006639: TICK();
    mflag = 0;  /* 006639 CLM */
    A = rd16(ABS(0x0226));  /* 00663A LDA */
    mflag = 1;  /* 00663D SEM */
    /* 00663E CMP */
    if ((A & 0xFF) != 0x73) goto L_006659;  /* 006640 BNE */
    Y = rd16(DP(0x05));  /* 006642 LDY */
    mflag = 0;  /* 006644 CLM */
    S -= 3; F_004721();  /* 006645 JSRL */
    if ((rd16(ABS(0x0240)) & 0x0004) == 0) goto L_006656;  /* 006649 BBC */
    Y = 0x002B;  /* 00664F LDY */
    S -= 3; F_003310();  /* 006652 JSRL */
L_006656: TICK();
    goto L_006665;  /* 006656 JMP */
L_006659: TICK();
    mflag = 0;  /* 006659 CLM */
    S -= 3; F_004558();  /* 00665A JSRL */
    Y = 0x002B;  /* 00665E LDY */
    S -= 3; F_003310();  /* 006661 JSRL */
L_006665: TICK();
    mflag = 1;  /* 006665 SEM */
    wr8(ABS(0x0226), 0x0F);  /* 006666 LDM */
    mflag = 0;  /* 00666A CLM */
    S -= 3; F_00828C();  /* 00666B JSRL */
L_00666F: TICK();
    mflag = 0;  /* 00666F CLM */
    S -= 3; measurements_0198D5();  /* 006670 JSRL */
    mflag = 0;  /* 006674 CLM */
    if ((rd16(ABS(0x0240)) & 0x0004) == 0x0004) goto L_006687;  /* 006675 BBS */
    A = 0x0000;  /* 00667B LDA */
    mflag = 1;  /* 00667E SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 00667F STA */
    mflag = 0;  /* 006681 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0004);  /* 006682 SEB */
L_006687: TICK();
    mflag = 1;  /* 006687 SEM */
    goto L_00658B;  /* 006688 JMP */
L_00668B: TICK();
    mflag = 0;  /* 00668B CLM */
    A = 0x0000;  /* 00668C LDA */
    mflag = 1;  /* 00668F SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 006690 STA */
    mflag = 0;  /* 006692 CLM */
    Y = pull16();  /* 006693 PLY */
    D = pull16();  /* 006694 PLD */
    S += 3; return;  /* 006695 RTL */
}

/* 006696  callers: 01A758 */
void F_006696(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006696 LDA */
    push16(D);  /* 006698 PHD */
    D = A;  /* 006699 TAD */
    A += 1;  /* 00669A INC */
    A += 1;  /* 00669B INC */
    push16(A);  /* 00669C PHA */
    S -= 3; F_00FC70();  /* 00669D JSRL */
    if ((rd16(ABS(0x0246)) & 0x0002) == 0) goto L_0066AA;  /* 0066A1 BBC */
    goto L_00674B;  /* 0066A7 JMP */
L_0066AA: TICK();
    Y = 0x0000;  /* 0066AA LDY */
    S -= 3; F_0069EF();  /* 0066AD JSRL */
    /* 0066B1 CMP */
    if ((A & 0xFF) != 0x0F) goto L_0066B8;  /* 0066B3 BNE */
    goto L_00674B;  /* 0066B5 JMP */
L_0066B8: TICK();
    mflag = 0;  /* 0066B8 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x8000);  /* 0066B9 SEB */
    mflag = 1;  /* 0066BE SEM */
    wr8(ABS(0x0252), 0x43);  /* 0066BF LDM */
    wr8(ABS(0x0253), 0x4E);  /* 0066C3 LDM */
    SETLO(A, rd8(ABS(0x0343)));  /* 0066C7 LDA */
    SETLO(A, A & 0x0F);  /* 0066CA AND */
    SETLO(A, A + 1);  /* 0066CC INC */
    SETLO(A, A + 1);  /* 0066CD INC */
    wr8(DP(0x00), (uint8_t)A);  /* 0066CE STA */
L_0066D0: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0066D0 LDA */
    /* 0066D2 CMP */
    if ((A & 0xFF) >= 0x08) goto L_0066E5;  /* 0066D4 BCS */
    mflag = 0;  /* 0066D6 CLM */
    A &= 0x00FF;  /* 0066D7 AND */
    X = A;  /* 0066DA TAX */
    mflag = 1;  /* 0066DB SEM */
    wr8((ABS(0x0252) + X), 0x20);  /* 0066DC LDM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 0066E0 INC */
    goto L_0066D0;  /* 0066E2 JMP */
L_0066E5: TICK();
    mflag = 1;  /* 0066E5 SEM */
    wr8(DP(0x00), 0x00);  /* 0066E6 LDM */
L_0066E9: TICK();
    SETLO(A, rd8(ABS(0x0343)));  /* 0066E9 LDA */
    SETLO(A, A & 0x0F);  /* 0066EC AND */
    t = rd8(DP(0x00));  /* 0066EE CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_006738;  /* 0066F0 BCC */
    if (Z) goto L_006738;  /* 0066F2 BEQ */
    Y = rd16(DP(0x00));  /* 0066F4 LDY */
    mflag = 0;  /* 0066F6 CLM */
    S -= 3; F_0069EF();  /* 0066F7 JSRL */
    wr8(DP(0x01), (uint8_t)A);  /* 0066FB STA */
    t = rd8(ABS(0x020E));  /* 0066FD CMP */
    if ((A & 0xFF) != t) goto L_006707;  /* 006700 BNE */
    SETLO(A, 0x47);  /* 006702 LDA */
    goto L_006721;  /* 006704 JMP */
L_006707: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 006707 LDA */
    C = ((A & 0xFF) >= 0x0A);  /* 006709 CMP */
    if (C) goto L_006717;  /* 00670B BCS */
    mflag = 0;  /* 00670D CLM */
    A &= 0x00FF;  /* 00670E AND */
    A = adc16(A, 0x0030);  /* 006711 ADC */
    goto L_006721;  /* 006714 JMP */
L_006717: TICK();
    mflag = 0;  /* 006717 CLM */
    A = rd16(DP(0x01));  /* 006718 LDA */
    A &= 0x00FF;  /* 00671A AND */
    C = 0;  /* 00671D CLC */
    A = adc16(A, 0x0037);  /* 00671E ADC */
L_006721: TICK();
    mflag = 1;  /* 006721 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 006722 STA */
    SETLO(A, rd8(DP(0x00)));  /* 006724 LDA */
    mflag = 0;  /* 006726 CLM */
    A &= 0x00FF;  /* 006727 AND */
    A += 1;  /* 00672A INC */
    A += 1;  /* 00672B INC */
    X = A;  /* 00672C TAX */
    A = rd16(DP(0x02));  /* 00672D LDA */
    mflag = 1;  /* 00672F SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 006730 STA */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 006733 INC */
    goto L_0066E9;  /* 006735 JMP */
L_006738: TICK();
    mflag = 0;  /* 006738 CLM */
    S -= 3; F_00828C();  /* 006739 JSRL */
    mflag = 1;  /* 00673D SEM */
    wr8(ABS(0x0233), 0x10);  /* 00673E LDM */
L_006742: TICK();
    wr8(ABS(WDT), 0x00);  /* 006742 LDM */
    SETLO(A, rd8(ABS(0x0233)));  /* 006746 LDA */
    if ((A & 0xFF) != 0) goto L_006742;  /* 006749 BNE */
L_00674B: TICK();
    mflag = 1;  /* 00674B SEM */
    wr8(ABS(0x0226), 0x0F);  /* 00674C LDM */
    mflag = 0;  /* 006750 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0004);  /* 006751 SEB */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 006756 SEB */
    S -= 3; F_007A06();  /* 00675B JSRL */
    mflag = 1;  /* 00675F SEM */
    wr8(ABS(0x0232), 0x9D);  /* 006760 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_006772;  /* 006764 BBC */
    mflag = 0;  /* 006769 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0400);  /* 00676A SEB */
    goto L_00677C;  /* 00676F JMP */
L_006772: TICK();
    mflag = 0;  /* 006772 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x2000);  /* 006773 SEB */
    S -= 3; F_007529();  /* 006778 JSRL */
L_00677C: TICK();
    B = 0x0000;  /* 00677C LDA */
    Y = 0x0228;  /* 006780 LDY */
    S -= 3; F_006567();  /* 006783 JSRL */
    mflag = 1;  /* 006787 SEM */
    wr8(ABS(0x0231), 0x16);  /* 006788 LDM */
    wr8(ABS(0x0227), 0x00);  /* 00678C LDM */
    mflag = 0;  /* 006790 CLM */
    A = 0x0000;  /* 006791 LDA */
    A = asl16(A);  /* 006794 ASL */
    A = asl16(A);  /* 006795 ASL */
    A += 1;  /* 006796 INC */
    X = A;  /* 006797 TAX */
    A = rd16((0x00319E + X));  /* 006798 LDA */
    mflag = 1;  /* 00679C SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 00679D STA */
    mflag = 0;  /* 0067A0 CLM */
    A = rd16((0x00319C + X));  /* 0067A1 LDA */
    wr16(ABS(0x02DF), A);  /* 0067A5 STA */
    Y = 0x0006;  /* 0067A8 LDY */
    S -= 3; F_00927F();  /* 0067AB JSRL */
    mflag = 1;  /* 0067AF SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0067D4;  /* 0067B0 BBS */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_0067D4;  /* 0067B5 BBC */
    SETLO(A, rd8(ABS(0x0206)));  /* 0067BA LDA */
    /* 0067BD CMP */
    if ((A & 0xFF) >= 0x0F) goto L_0067CF;  /* 0067BF BCS */
    mflag = 0;  /* 0067C1 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0002);  /* 0067C2 SEB */
    mflag = 1;  /* 0067C7 SEM */
    wr8(ABS(0x0231), 0x00);  /* 0067C8 LDM */
    goto L_0067D4;  /* 0067CC JMP */
L_0067CF: TICK();
    mflag = 0;  /* 0067CF CLM */
    S -= 3; F_004D03();  /* 0067D0 JSRL */
L_0067D4: TICK();
    mflag = 0;  /* 0067D4 CLM */
    if ((rd16(ABS(0x0246)) & 0x0002) == 0x0002) goto L_006814;  /* 0067D5 BBS */
    Y = 0x002B;  /* 0067DB LDY */
    S -= 3; F_003310();  /* 0067DE JSRL */
    mflag = 1;  /* 0067E2 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0067ED;  /* 0067E3 BBS */
    if ((rd8(ABS(0x0334)) & 0x10) == 0x10) goto L_0067FC;  /* 0067E8 BBS */
L_0067ED: TICK();
    mflag = 0;  /* 0067ED CLM */
    mflag = 1;  /* 0067EE SEM */
    SETLO(A, rd8(ABS(0x02E1)));  /* 0067EF LDA */
    mflag = 0;  /* 0067F2 CLM */
    push16(A);  /* 0067F3 PHA */
    A = rd16(ABS(0x02DF));  /* 0067F4 LDA */
    push16(A);  /* 0067F7 PHA */
    S -= 3; rt_call_ptr_01E4C2();  /* 0067F8 JSRL */
L_0067FC: TICK();
    mflag = 0;  /* 0067FC CLM */
    S -= 3; F_01DFDF();  /* 0067FD JSRL */
    S -= 3; F_00828C();  /* 006801 JSRL */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_006814;  /* 006805 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0020);  /* 00680B CLB */
    S -= 3; F_005BAD();  /* 006810 JSRL */
L_006814: TICK();
    Y = pull16();  /* 006814 PLY */
    D = pull16();  /* 006815 PLD */
    S += 3; return;  /* 006816 RTL */
}

/* 006817  callers: 01A7B0 */
void F_006817(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006817 LDA */
    push16(D);  /* 006819 PHD */
    D = A;  /* 00681A TAD */
    A += 1;  /* 00681B INC */
    push16(A);  /* 00681C PHA */
    S -= 3; F_005460();  /* 00681D JSRL */
    S -= 3; F_0058FB();  /* 006821 JSRL */
    S -= 3; F_005CA6();  /* 006825 JSRL */
    S -= 3; F_005F9C();  /* 006829 JSRL */
    S -= 3; F_005E56();  /* 00682D JSRL */
    mflag = 1;  /* 006831 SEM */
    SETLO(A, rd8(ABS(0x0230)));  /* 006832 LDA */
    if ((A & 0xFF) != 0) goto L_006896;  /* 006835 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 006837 LDA */
    if ((A & 0xFF) == 0) goto L_006896;  /* 00683A BEQ */
    wr8(DP(0x00), (uint8_t)A);  /* 00683C STA */
    /* 00683E CMP */
    if ((A & 0xFF) != 0x14) goto L_006878;  /* 006840 BNE */
    SETLO(A, rd8(ABS(0x0294)));  /* 006842 LDA */
    if ((A & 0xFF) == 0) goto L_00684C;  /* 006845 BEQ */
    mflag = 0;  /* 006847 CLM */
    S -= 3; F_004313();  /* 006848 JSRL */
L_00684C: TICK();
    mflag = 1;  /* 00684C SEM */
    wr8(ABS(0x0227), 0x00);  /* 00684D LDM */
    mflag = 0;  /* 006851 CLM */
    A = 0x0000;  /* 006852 LDA */
    A = asl16(A);  /* 006855 ASL */
    A = asl16(A);  /* 006856 ASL */
    A += 1;  /* 006857 INC */
    X = A;  /* 006858 TAX */
    A = rd16((0x00319E + X));  /* 006859 LDA */
    mflag = 1;  /* 00685D SEM */
    wr8(ABS(0x02E1), (uint8_t)A);  /* 00685E STA */
    mflag = 0;  /* 006861 CLM */
    A = rd16((0x00319C + X));  /* 006862 LDA */
    wr16(ABS(0x02DF), A);  /* 006866 STA */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 006869 SEB */
    mflag = 1;  /* 00686E SEM */
    wr8(ABS(0x0232), 0x3F);  /* 00686F LDM */
    mflag = 0;  /* 006873 CLM */
    S -= 3; F_007A06();  /* 006874 JSRL */
L_006878: TICK();
    mflag = 0;  /* 006878 CLM */
    mflag = 1;  /* 006879 SEM */
    SETLO(A, rd8(ABS(0x02E1)));  /* 00687A LDA */
    mflag = 0;  /* 00687D CLM */
    push16(A);  /* 00687E PHA */
    A = rd16(ABS(0x02DF));  /* 00687F LDA */
    push16(A);  /* 006882 PHA */
    S -= 3; rt_call_ptr_01E4C2();  /* 006883 JSRL */
    S -= 3; F_00828C();  /* 006887 JSRL */
    A = rd16(DP(0x00));  /* 00688B LDA */
    mflag = 1;  /* 00688D SEM */
    /* 00688E CMP */
    if ((A & 0xFF) != 0x14) goto L_006896;  /* 006890 BNE */
    wr8(ABS(0x0231), 0x00);  /* 006892 LDM */
L_006896: TICK();
    mflag = 0;  /* 006896 CLM */
    if ((rd16(ABS(0x023A)) & 0x0100) == 0) goto L_0068AD;  /* 006897 BBC */
    mflag = 1;  /* 00689D SEM */
    SETLO(A, rd8(ABS(0x0232)));  /* 00689E LDA */
    if ((A & 0xFF) != 0) goto L_0068AD;  /* 0068A1 BNE */
    mflag = 0;  /* 0068A3 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0100);  /* 0068A4 CLB */
    S -= 3; F_007A06();  /* 0068A9 JSRL */
L_0068AD: TICK();
    mflag = 0;  /* 0068AD CLM */
    S -= 3; F_00604C();  /* 0068AE JSRL */
    mflag = 1;  /* 0068B2 SEM */
    if ((rd8(ABS(0x023C)) & 0x01) == 0) goto L_0068BD;  /* 0068B3 BBC */
    mflag = 0;  /* 0068B8 CLM */
    S -= 3; F_007A06();  /* 0068B9 JSRL */
L_0068BD: TICK();
    mflag = 0;  /* 0068BD CLM */
    if ((rd16(ABS(0x023E)) & 0x0100) == 0) goto L_0068CC;  /* 0068BE BBC */
    S -= 3; F_0062AB();  /* 0068C4 JSRL */
    S -= 3; F_00828C();  /* 0068C8 JSRL */
L_0068CC: TICK();
    mflag = 1;  /* 0068CC SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_0068E9;  /* 0068CD BBC */
    mflag = 0;  /* 0068D2 CLM */
    if ((rd16(ABS(0x0242)) & 0x0800) == 0) goto L_0068E9;  /* 0068D3 BBC */
    mflag = 1;  /* 0068D9 SEM */
    SETLO(A, rd8(ABS(0x01B7)));  /* 0068DA LDA */
    if ((A & 0xFF) != 0) goto L_0068E9;  /* 0068DD BNE */
    mflag = 0;  /* 0068DF CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0800);  /* 0068E0 CLB */
    S -= 3; cbus_write_sel2_00755E();  /* 0068E5 JSRL */
L_0068E9: TICK();
    mflag = 1;  /* 0068E9 SEM */
    if ((rd8(ABS(0x0246)) & 0x80) == 0) goto L_00691A;  /* 0068EA BBC */
    mflag = 0;  /* 0068EF CLM */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0x0020) goto L_006908;  /* 0068F0 BBS */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0080);  /* 0068F6 CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0002);  /* 0068FB SEB */
    wr16(ABS(0x0222), 0x0000);  /* 006900 LDM */
    goto L_00691A;  /* 006905 JMP */
L_006908: TICK();
    mflag = 0;  /* 006908 CLM */
    X = rd16(ABS(0x0222));  /* 006909 LDX */
    if (X != 0) goto L_00691A;  /* 00690C BNE */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0080);  /* 00690E CLB */
    Y = 0x000E;  /* 006913 LDY */
    S -= 3; F_0080AA();  /* 006916 JSRL */
L_00691A: TICK();
    mflag = 0;  /* 00691A CLM */
    Y = pull16();  /* 00691B PLY */
    D = pull16();  /* 00691C PLD */
    S += 3; return;  /* 00691D RTL */
}

/* 00691E  callers: 006960, 006975, 00698C, 0069A3 */
void F_00691E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00691E LDA */
    push16(D);  /* 006920 PHD */
    D = A;  /* 006921 TAD */
    wr16(DP(0x02), Y);  /* 006922 STY */
    mflag = 1;  /* 006924 SEM */
    wr8(DP(0x04), (uint8_t)B);  /* 006925 STA */
    mflag = 0;  /* 006928 CLM */
    B = 0x0000;  /* 006929 LDA */
    A = rd16(LIND(0x02));  /* 00692D LDA */
    div16(rd16(DP(0x00)));  /* 00692F DIV */
    C = 0;  /* 006932 CLC */
    A = adc16(A, 0x0030);  /* 006933 ADC */
    B = 0x0000;  /* 006936 LDA */
    wr16(DP(0x05), A);  /* 00693A STA */
    A = rd16(LIND(0x02));  /* 00693C LDA */
    div16(rd16(DP(0x00)));  /* 00693E DIV */
    wr16(LIND(0x02), B);  /* 006941 STA */
    A = rd16(DP(0x05));  /* 006944 LDA */
    D = pull16();  /* 006946 PLD */
    S += 3; return;  /* 006947 RTL */
}

/* 006948  callers: 00E22C, 01553E */
void F_006948(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006948 LDA */
    push16(D);  /* 00694A PHD */
    D = A;  /* 00694B TAD */
    C = 0;  /* 00694C CLC */
    A = adc16(A, 0x0005);  /* 00694D ADC */
    push16(A);  /* 006950 PHA */
    wr16(DP(0x03), Y);  /* 006951 STY */
    wr16(DP(0x05), 0x2710);  /* 006953 LDM */
    A = D;  /* 006957 TDA */
    A += 1;  /* 006958 INC */
    A += 1;  /* 006959 INC */
    A += 1;  /* 00695A INC */
    B = 0x0000;  /* 00695B LDA */
    Y = A;  /* 00695F TAY */
    S -= 3; F_00691E();  /* 006960 JSRL */
    mflag = 1;  /* 006964 SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 006965 STA */
    mflag = 0;  /* 006967 CLM */
    wr16(DP(0x05), 0x03E8);  /* 006968 LDM */
    A = D;  /* 00696C TDA */
    A += 1;  /* 00696D INC */
    A += 1;  /* 00696E INC */
    A += 1;  /* 00696F INC */
    B = 0x0000;  /* 006970 LDA */
    Y = A;  /* 006974 TAY */
    S -= 3; F_00691E();  /* 006975 JSRL */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 006979 INC */
    mflag = 1;  /* 00697B SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 00697C STA */
    mflag = 0;  /* 00697E CLM */
    wr16(DP(0x05), 0x0064);  /* 00697F LDM */
    A = D;  /* 006983 TDA */
    A += 1;  /* 006984 INC */
    A += 1;  /* 006985 INC */
    A += 1;  /* 006986 INC */
    B = 0x0000;  /* 006987 LDA */
    Y = A;  /* 00698B TAY */
    S -= 3; F_00691E();  /* 00698C JSRL */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 006990 INC */
    mflag = 1;  /* 006992 SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 006993 STA */
    mflag = 0;  /* 006995 CLM */
    wr16(DP(0x05), 0x000A);  /* 006996 LDM */
    A = D;  /* 00699A TDA */
    A += 1;  /* 00699B INC */
    A += 1;  /* 00699C INC */
    A += 1;  /* 00699D INC */
    B = 0x0000;  /* 00699E LDA */
    Y = A;  /* 0069A2 TAY */
    S -= 3; F_00691E();  /* 0069A3 JSRL */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 0069A7 INC */
    mflag = 1;  /* 0069A9 SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 0069AA STA */
    SETLO(A, rd8(DP(0x03)));  /* 0069AC LDA */
    C = 0;  /* 0069AE CLC */
    SETLO(A, adc8(A, 0x30));  /* 0069AF ADC */
    mflag = 0;  /* 0069B1 CLM */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 0069B2 INC */
    mflag = 1;  /* 0069B4 SEM */
    wr8(LIND(0x00), (uint8_t)A);  /* 0069B5 STA */
    mflag = 0;  /* 0069B7 CLM */
    Y = pull16();  /* 0069B8 PLY */
    D = pull16();  /* 0069B9 PLD */
    S += 3; return;  /* 0069BA RTL */
}

/* 0069BB  callers: 005BCE, 00797E, 00E73C, 016E3A */
void F_0069BB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0069BB LDA */
    push16(D);  /* 0069BD PHD */
    D = A;  /* 0069BE TAD */
    wr16(DP(0x00), Y);  /* 0069BF STY */
    mflag = 1;  /* 0069C1 SEM */
    SETLO(B, 0x00);  /* 0069C2 LDA */
    SETLO(A, Y);  /* 0069C5 TYA */
    div8(0x02);  /* 0069C6 DIV */
    mflag = 0;  /* 0069C9 CLM */
    A &= 0x00FF;  /* 0069CA AND */
    A ^= 0xFFFF;  /* 0069CD EOR */
    A += 1;  /* 0069D0 INC */
    A += 1;  /* 0069D1 INC */
    A += 1;  /* 0069D2 INC */
    X = A;  /* 0069D3 TAX */
    A = rd16((ABS(0x0336) + X));  /* 0069D4 LDA */
    mflag = 1;  /* 0069D7 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 0069D8 STA */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_0069E7;  /* 0069DA BBC */
    SETLO(A, lsr8(A));  /* 0069DE LSR */
    SETLO(A, lsr8(A));  /* 0069DF LSR */
    SETLO(A, lsr8(A));  /* 0069E0 LSR */
    SETLO(A, lsr8(A));  /* 0069E1 LSR */
    wr8(DP(0x01), (uint8_t)A);  /* 0069E2 STA */
    goto L_0069EB;  /* 0069E4 JMP */
L_0069E7: TICK();
    mflag = 1;  /* 0069E7 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) & ~0xF0);  /* 0069E8 CLB */
L_0069EB: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0069EB LDA */
    D = pull16();  /* 0069ED PLD */
    S += 3; return;  /* 0069EE RTL */
}

/* 0069EF  callers: 003811, 006393, 0066AD, 0066F7, 00D8F4, 01510E, 016C65 */
void F_0069EF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0069EF LDA */
    push16(D);  /* 0069F1 PHD */
    D = A;  /* 0069F2 TAD */
    wr16(DP(0x00), Y);  /* 0069F3 STY */
    mflag = 1;  /* 0069F5 SEM */
    SETLO(B, 0x00);  /* 0069F6 LDA */
    SETLO(A, Y);  /* 0069F9 TYA */
    div8(0x02);  /* 0069FA DIV */
    mflag = 0;  /* 0069FD CLM */
    A &= 0x00FF;  /* 0069FE AND */
    A ^= 0xFFFF;  /* 006A01 EOR */
    A += 1;  /* 006A04 INC */
    C = 0;  /* 006A05 CLC */
    A = adc16(A, 0x0005);  /* 006A06 ADC */
    X = A;  /* 006A09 TAX */
    A = rd16((ABS(0x0336) + X));  /* 006A0A LDA */
    mflag = 1;  /* 006A0D SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 006A0E STA */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_006A1D;  /* 006A10 BBC */
    SETLO(A, lsr8(A));  /* 006A14 LSR */
    SETLO(A, lsr8(A));  /* 006A15 LSR */
    SETLO(A, lsr8(A));  /* 006A16 LSR */
    SETLO(A, lsr8(A));  /* 006A17 LSR */
    wr8(DP(0x01), (uint8_t)A);  /* 006A18 STA */
    goto L_006A21;  /* 006A1A JMP */
L_006A1D: TICK();
    mflag = 1;  /* 006A1D SEM */
    wr8(DP(0x01), rd8(DP(0x01)) & ~0xF0);  /* 006A1E CLB */
L_006A21: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 006A21 LDA */
    D = pull16();  /* 006A23 PLD */
    S += 3; return;  /* 006A24 RTL */
}

/* 006A25  callers: 006433, 0079EC, 00E79F */
void F_006A25(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006A25 LDA */
    push16(D);  /* 006A27 PHD */
    D = A;  /* 006A28 TAD */
    wr16(DP(0x00), Y);  /* 006A29 STY */
    mflag = 1;  /* 006A2B SEM */
    SETLO(B, 0x00);  /* 006A2C LDA */
    SETLO(A, Y);  /* 006A2F TYA */
    div8(0x02);  /* 006A30 DIV */
    mflag = 0;  /* 006A33 CLM */
    A &= 0x00FF;  /* 006A34 AND */
    A ^= 0xFFFF;  /* 006A37 EOR */
    A += 1;  /* 006A3A INC */
    C = 0;  /* 006A3B CLC */
    A = adc16(A, 0x0008);  /* 006A3C ADC */
    X = A;  /* 006A3F TAX */
    A = rd16((ABS(0x0336) + X));  /* 006A40 LDA */
    mflag = 1;  /* 006A43 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 006A44 STA */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_006A53;  /* 006A46 BBC */
    SETLO(A, lsr8(A));  /* 006A4A LSR */
    SETLO(A, lsr8(A));  /* 006A4B LSR */
    SETLO(A, lsr8(A));  /* 006A4C LSR */
    SETLO(A, lsr8(A));  /* 006A4D LSR */
    wr8(DP(0x01), (uint8_t)A);  /* 006A4E STA */
    goto L_006A57;  /* 006A50 JMP */
L_006A53: TICK();
    mflag = 1;  /* 006A53 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) & ~0xF0);  /* 006A54 CLB */
L_006A57: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 006A57 LDA */
    D = pull16();  /* 006A59 PLD */
    S += 3; return;  /* 006A5A RTL */
}

/* 006A5B  callers:  */
void F_006A5B(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 006A5B SEM */
    wr8(ABS(WDTFS), rd8(ABS(WDTFS)) & ~0x01);  /* 006A5C CLB */
    wr8(ABS(WDT), 0xFF);  /* 006A60 LDM */
    cpu_wait();  /* 006A64 WIT */
    mflag = 0;  /* 006A65 CLM */
    S += 3; return;  /* 006A66 RTL */
}

/* 006A67  callers: 00DF83, 00DF99 */
void F_006A67(void)
{
    uint32_t t; (void)t;
    S += 3; return;  /* 006A67 RTL */
}

/* 006A68  callers: 0071AF, 0071B6, 0071CC, 007216, 00721D, 007233, 007297, 00729E ... */
void i2c_send_byte_006A68(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006A68 LDA */
    push16(D);  /* 006A6A PHD */
    D = A;  /* 006A6B TAD */
    wr16(DP(0x00), Y);  /* 006A6C STY */
    mflag = 1;  /* 006A6E SEM */
    wr8(DP(0x01), 0x07);  /* 006A6F LDM */
L_006A72: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 006A72 LDA */
    mflag = 0;  /* 006A74 CLM */
    A &= 0x00FF;  /* 006A75 AND */
    X = A;  /* 006A78 TAX */
    A = 0x0001;  /* 006A79 LDA */
    mflag = 1;  /* 006A7C SEM */
    S -= 3; rt_shl_01E4E0();  /* 006A7D JSRL */
    SETLO(A, A & rd8(DP(0x00)));  /* 006A81 AND */
    if ((A & 0xFF) == 0) goto L_006A8C;  /* 006A83 BEQ */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006A85 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    goto L_006A95;  /* 006A89 JMP */
L_006A8C: TICK();
    mflag = 1;  /* 006A8C SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006A8D CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006A91 SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006A95: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006A95 SEB [P61 GOSTERGE_SAAT(SCL)] */
    t = (rd8(DP(0x01)) - 1) & 0xFF; wr8(DP(0x01), t);  /* 006A99 DEC */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006A9B CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(DP(0x01)));  /* 006A9F LDA */
    if ((A & 0x80) == 0) goto L_006A72;  /* 006AA1 BPL */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006AA3 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006AA7 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006AAB SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006AAF LDM */
    SETLO(A, rd8(ABS(P6)));  /* 006AB3 LDA */
    SETLO(A, A & 0x04);  /* 006AB6 AND */
    SETLO(A, lsr8(A));  /* 006AB8 LSR */
    SETLO(A, lsr8(A));  /* 006AB9 LSR */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006ABA CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(DP(0x00), (uint8_t)A);  /* 006ABE STA */
    D = pull16();  /* 006AC0 PLD */
    S += 3; return;  /* 006AC1 RTL */
}

/* 006AC2  callers: 005109, 005111, 005121, 005185, 00518D, 00519D, 0058C1, 0058C8 ... */
void i2c_send_byte_fast_006AC2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006AC2 LDA */
    push16(D);  /* 006AC4 PHD */
    D = A;  /* 006AC5 TAD */
    wr16(DP(0x00), Y);  /* 006AC6 STY */
    mflag = 1;  /* 006AC8 SEM */
    if ((rd8(DP(0x00)) & 0x80) == 0x80) goto L_006AD0;  /* 006AC9 BBS */
    goto L_006BBF;  /* 006ACD JMP */
L_006AD0: TICK();
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006AD0 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006AD4 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006AD8 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006ADC LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006AE0 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x40) == 0x40) goto L_006B57;  /* 006AE4 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006AE8 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006AEC SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006AF0 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006AF4 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006AF8 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x20) == 0) goto L_006B2F;  /* 006AFC BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006B00 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006B04 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B08 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B0C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B10 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0x10) goto L_006B20;  /* 006B14 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006B18 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006B1C SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006B20: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B20 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B24 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B28 CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006B54;  /* 006B2C JMP */
L_006B2F: TICK();
    mflag = 1;  /* 006B2F SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B30 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B34 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B38 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0) goto L_006B48;  /* 006B3C BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006B40 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006B44 LDM */
L_006B48: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B48 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B4C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B50 CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006B54: TICK();
    goto L_006BBC;  /* 006B54 JMP */
L_006B57: TICK();
    mflag = 1;  /* 006B57 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B58 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B5C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B60 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x20) == 0x20) goto L_006B97;  /* 006B64 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006B68 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006B6C SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B70 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B74 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B78 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0) goto L_006B88;  /* 006B7C BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006B80 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006B84 LDM */
L_006B88: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B88 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B8C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006B90 CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006BBC;  /* 006B94 JMP */
L_006B97: TICK();
    mflag = 1;  /* 006B97 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006B98 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006B9C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006BA0 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0x10) goto L_006BB0;  /* 006BA4 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006BA8 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006BAC SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006BB0: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006BB0 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006BB4 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006BB8 CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006BBC: TICK();
    goto L_006CAC;  /* 006BBC JMP */
L_006BBF: TICK();
    mflag = 1;  /* 006BBF SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006BC0 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006BC4 SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006BC8 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006BCC LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006BD0 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x40) == 0) goto L_006C47;  /* 006BD4 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006BD8 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006BDC LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006BE0 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006BE4 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006BE8 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x20) == 0x20) goto L_006C1F;  /* 006BEC BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006BF0 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006BF4 SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006BF8 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006BFC LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C00 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0) goto L_006C10;  /* 006C04 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006C08 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006C0C LDM */
L_006C10: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C10 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C14 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C18 CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006C44;  /* 006C1C JMP */
L_006C1F: TICK();
    mflag = 1;  /* 006C1F SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C20 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C24 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C28 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0x10) goto L_006C38;  /* 006C2C BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006C30 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006C34 SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006C38: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C38 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C3C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C40 CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006C44: TICK();
    goto L_006CAC;  /* 006C44 JMP */
L_006C47: TICK();
    mflag = 1;  /* 006C47 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C48 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C4C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C50 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x20) == 0) goto L_006C87;  /* 006C54 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006C58 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006C5C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C60 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C64 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C68 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0x10) goto L_006C78;  /* 006C6C BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006C70 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006C74 SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006C78: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C78 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C7C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C80 CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006CAC;  /* 006C84 JMP */
L_006C87: TICK();
    mflag = 1;  /* 006C87 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006C88 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006C8C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006C90 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x10) == 0) goto L_006CA0;  /* 006C94 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006C98 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006C9C LDM */
L_006CA0: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006CA0 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006CA4 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006CA8 CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006CAC: TICK();
    if ((rd8(DP(0x00)) & 0x08) == 0x08) goto L_006CB3;  /* 006CAC BBS */
    goto L_006DA2;  /* 006CB0 JMP */
L_006CB3: TICK();
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006CB3 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006CB7 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006CBB SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006CBF LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006CC3 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x04) == 0x04) goto L_006D3A;  /* 006CC7 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006CCB CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006CCF SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006CD3 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006CD7 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006CDB CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x02) == 0) goto L_006D12;  /* 006CDF BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006CE3 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006CE7 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006CEB SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006CEF LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006CF3 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0x01) goto L_006D03;  /* 006CF7 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006CFB CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006CFF SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006D03: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D03 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D07 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D0B CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006D37;  /* 006D0F JMP */
L_006D12: TICK();
    mflag = 1;  /* 006D12 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D13 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D17 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D1B CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_006D2B;  /* 006D1F BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006D23 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006D27 LDM */
L_006D2B: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D2B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D2F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D33 CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006D37: TICK();
    goto L_006D9F;  /* 006D37 JMP */
L_006D3A: TICK();
    mflag = 1;  /* 006D3A SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D3B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D3F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D43 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x02) == 0x02) goto L_006D7A;  /* 006D47 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006D4B CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006D4F SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D53 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D57 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D5B CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_006D6B;  /* 006D5F BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006D63 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006D67 LDM */
L_006D6B: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D6B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D6F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D73 CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006D9F;  /* 006D77 JMP */
L_006D7A: TICK();
    mflag = 1;  /* 006D7A SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D7B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D7F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D83 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0x01) goto L_006D93;  /* 006D87 BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006D8B CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006D8F SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006D93: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006D93 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006D97 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006D9B CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006D9F: TICK();
    goto L_006E8F;  /* 006D9F JMP */
L_006DA2: TICK();
    mflag = 1;  /* 006DA2 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006DA3 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006DA7 SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006DAB SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006DAF LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006DB3 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x04) == 0) goto L_006E2A;  /* 006DB7 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006DBB CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006DBF LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006DC3 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006DC7 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006DCB CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x02) == 0x02) goto L_006E02;  /* 006DCF BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006DD3 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006DD7 SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006DDB SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006DDF LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006DE3 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_006DF3;  /* 006DE7 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006DEB CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006DEF LDM */
L_006DF3: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006DF3 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006DF7 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006DFB CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006E27;  /* 006DFF JMP */
L_006E02: TICK();
    mflag = 1;  /* 006E02 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E03 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E07 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E0B CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0x01) goto L_006E1B;  /* 006E0F BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006E13 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006E17 SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006E1B: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E1B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E1F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E23 CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006E27: TICK();
    goto L_006E8F;  /* 006E27 JMP */
L_006E2A: TICK();
    mflag = 1;  /* 006E2A SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E2B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E2F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E33 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x02) == 0) goto L_006E6A;  /* 006E37 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006E3B CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006E3F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E43 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E47 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E4B CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0x01) goto L_006E5B;  /* 006E4F BBS */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006E53 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006E57 SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006E5B: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E5B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E5F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E63 CLB [P61 GOSTERGE_SAAT(SCL)] */
    goto L_006E8F;  /* 006E67 JMP */
L_006E6A: TICK();
    mflag = 1;  /* 006E6A SEM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E6B SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E6F LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E73 CLB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(DP(0x00)) & 0x01) == 0) goto L_006E83;  /* 006E77 BBC */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006E7B CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 006E7F LDM */
L_006E83: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E83 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006E87 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006E8B CLB [P61 GOSTERGE_SAAT(SCL)] */
L_006E8F: TICK();
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006E8F CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006E93 SEB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_006EA4;  /* 006E97 BBC */
    SETLO(A, rd8(ABS(P6)));  /* 006E9C LDA */
    SETLO(A, A & 0x01);  /* 006E9F AND */
    goto L_006EAB;  /* 006EA1 JMP */
L_006EA4: TICK();
    SETLO(A, rd8(ABS(P6)));  /* 006EA4 LDA */
    SETLO(A, A & 0x04);  /* 006EA7 AND */
    SETLO(A, lsr8(A));  /* 006EA9 LSR */
    SETLO(A, lsr8(A));  /* 006EAA LSR */
L_006EAB: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006EAB CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(DP(0x00), (uint8_t)A);  /* 006EAF STA */
    D = pull16();  /* 006EB1 PLD */
    S += 3; return;  /* 006EB2 RTL */
}

/* 006EB3  callers: 0071D4, 00723B, 007248, 0072BC, 0072C9, 007376, 009C5A */
void i2c_recv_byte_006EB3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006EB3 LDA */
    push16(D);  /* 006EB5 PHD */
    D = A;  /* 006EB6 TAD */
    wr16(DP(0x00), Y);  /* 006EB7 STY */
    mflag = 1;  /* 006EB9 SEM */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006EBA CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(DP(0x01), 0x00);  /* 006EBE LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006EC1 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006EC5 LDA */
    SETLO(A, A & 0x04);  /* 006EC8 AND */
    SETLO(A, lsr8(A));  /* 006ECA LSR */
    SETLO(A, lsr8(A));  /* 006ECB LSR */
    if (Z) goto L_006ED1;  /* 006ECC BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x80);  /* 006ECE SEB */
L_006ED1: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006ED1 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006ED5 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006ED9 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006EDD LDA */
    SETLO(A, A & 0x04);  /* 006EE0 AND */
    SETLO(A, lsr8(A));  /* 006EE2 LSR */
    SETLO(A, lsr8(A));  /* 006EE3 LSR */
    if (Z) goto L_006EE9;  /* 006EE4 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x40);  /* 006EE6 SEB */
L_006EE9: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006EE9 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006EED LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006EF1 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006EF5 LDA */
    SETLO(A, A & 0x04);  /* 006EF8 AND */
    SETLO(A, lsr8(A));  /* 006EFA LSR */
    SETLO(A, lsr8(A));  /* 006EFB LSR */
    if (Z) goto L_006F01;  /* 006EFC BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x20);  /* 006EFE SEB */
L_006F01: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F01 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006F05 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006F09 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006F0D LDA */
    SETLO(A, A & 0x04);  /* 006F10 AND */
    SETLO(A, lsr8(A));  /* 006F12 LSR */
    SETLO(A, lsr8(A));  /* 006F13 LSR */
    if (Z) goto L_006F19;  /* 006F14 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x10);  /* 006F16 SEB */
L_006F19: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F19 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006F1D LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006F21 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006F25 LDA */
    SETLO(A, A & 0x04);  /* 006F28 AND */
    SETLO(A, lsr8(A));  /* 006F2A LSR */
    SETLO(A, lsr8(A));  /* 006F2B LSR */
    if (Z) goto L_006F31;  /* 006F2C BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x08);  /* 006F2E SEB */
L_006F31: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F31 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006F35 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006F39 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006F3D LDA */
    SETLO(A, A & 0x04);  /* 006F40 AND */
    SETLO(A, lsr8(A));  /* 006F42 LSR */
    SETLO(A, lsr8(A));  /* 006F43 LSR */
    if (Z) goto L_006F49;  /* 006F44 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x04);  /* 006F46 SEB */
L_006F49: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F49 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006F4D LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006F51 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006F55 LDA */
    SETLO(A, A & 0x04);  /* 006F58 AND */
    SETLO(A, lsr8(A));  /* 006F5A LSR */
    SETLO(A, lsr8(A));  /* 006F5B LSR */
    if (Z) goto L_006F61;  /* 006F5C BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x02);  /* 006F5E SEB */
L_006F61: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F61 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006F65 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006F69 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006F6D LDA */
    SETLO(A, A & 0x04);  /* 006F70 AND */
    SETLO(A, lsr8(A));  /* 006F72 LSR */
    SETLO(A, lsr8(A));  /* 006F73 LSR */
    if (Z) goto L_006F79;  /* 006F74 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x01);  /* 006F76 SEB */
L_006F79: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F79 CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, Y);  /* 006F7D TYA */
    if ((A & 0xFF) == 0) goto L_006F87;  /* 006F7E BEQ */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006F80 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    goto L_006F90;  /* 006F84 JMP */
L_006F87: TICK();
    mflag = 1;  /* 006F87 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 006F88 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 006F8C SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_006F90: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006F90 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006F94 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006F98 CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(DP(0x01)));  /* 006F9C LDA */
    D = pull16();  /* 006F9E PLD */
    S += 3; return;  /* 006F9F RTL */
}

/* 006FA0  callers: 005129, 0051A5, 0058E2, 0073B0, 00E00A, 01C919, 01C944 */
void i2c_recv_byte_fast_006FA0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 006FA0 LDA */
    push16(D);  /* 006FA2 PHD */
    D = A;  /* 006FA3 TAD */
    wr16(DP(0x00), Y);  /* 006FA4 STY */
    mflag = 1;  /* 006FA6 SEM */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 006FA7 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(DP(0x01), 0x00);  /* 006FAB LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006FAE SEB [P61 GOSTERGE_SAAT(SCL)] */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_006FBA;  /* 006FB2 BBS */
    goto L_007061;  /* 006FB7 JMP */
L_006FBA: TICK();
    SETLO(A, rd8(ABS(P6)));  /* 006FBA LDA */
    SETLO(A, A & 0x01);  /* 006FBD AND */
    if ((A & 0xFF) == 0) goto L_006FC4;  /* 006FBF BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x80);  /* 006FC1 SEB */
L_006FC4: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006FC4 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006FC8 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006FCC SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006FD0 LDA */
    SETLO(A, A & 0x01);  /* 006FD3 AND */
    if ((A & 0xFF) == 0) goto L_006FDA;  /* 006FD5 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x40);  /* 006FD7 SEB */
L_006FDA: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006FDA CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006FDE LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006FE2 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006FE6 LDA */
    SETLO(A, A & 0x01);  /* 006FE9 AND */
    if ((A & 0xFF) == 0) goto L_006FF0;  /* 006FEB BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x20);  /* 006FED SEB */
L_006FF0: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 006FF0 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 006FF4 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 006FF8 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 006FFC LDA */
    SETLO(A, A & 0x01);  /* 006FFF AND */
    if ((A & 0xFF) == 0) goto L_007006;  /* 007001 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x10);  /* 007003 SEB */
L_007006: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007006 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 00700A LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 00700E SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 007012 LDA */
    SETLO(A, A & 0x01);  /* 007015 AND */
    if ((A & 0xFF) == 0) goto L_00701C;  /* 007017 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x08);  /* 007019 SEB */
L_00701C: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 00701C CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 007020 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 007024 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 007028 LDA */
    SETLO(A, A & 0x01);  /* 00702B AND */
    if ((A & 0xFF) == 0) goto L_007032;  /* 00702D BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x04);  /* 00702F SEB */
L_007032: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007032 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 007036 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 00703A SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 00703E LDA */
    SETLO(A, A & 0x01);  /* 007041 AND */
    if ((A & 0xFF) == 0) goto L_007048;  /* 007043 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x02);  /* 007045 SEB */
L_007048: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007048 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 00704C LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 007050 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 007054 LDA */
    SETLO(A, A & 0x01);  /* 007057 AND */
    if ((A & 0xFF) == 0) goto L_00705E;  /* 007059 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x01);  /* 00705B SEB */
L_00705E: TICK();
    goto L_007116;  /* 00705E JMP */
L_007061: TICK();
    mflag = 1;  /* 007061 SEM */
    SETLO(A, rd8(ABS(P6)));  /* 007062 LDA */
    SETLO(A, A & 0x04);  /* 007065 AND */
    SETLO(A, lsr8(A));  /* 007067 LSR */
    SETLO(A, lsr8(A));  /* 007068 LSR */
    if (Z) goto L_00706E;  /* 007069 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x80);  /* 00706B SEB */
L_00706E: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 00706E CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 007072 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 007076 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 00707A LDA */
    SETLO(A, A & 0x04);  /* 00707D AND */
    SETLO(A, lsr8(A));  /* 00707F LSR */
    SETLO(A, lsr8(A));  /* 007080 LSR */
    if (Z) goto L_007086;  /* 007081 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x40);  /* 007083 SEB */
L_007086: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007086 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 00708A LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 00708E SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 007092 LDA */
    SETLO(A, A & 0x04);  /* 007095 AND */
    SETLO(A, lsr8(A));  /* 007097 LSR */
    SETLO(A, lsr8(A));  /* 007098 LSR */
    if (Z) goto L_00709E;  /* 007099 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x20);  /* 00709B SEB */
L_00709E: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 00709E CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 0070A2 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 0070A6 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 0070AA LDA */
    SETLO(A, A & 0x04);  /* 0070AD AND */
    SETLO(A, lsr8(A));  /* 0070AF LSR */
    SETLO(A, lsr8(A));  /* 0070B0 LSR */
    if (Z) goto L_0070B6;  /* 0070B1 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x10);  /* 0070B3 SEB */
L_0070B6: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 0070B6 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 0070BA LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 0070BE SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 0070C2 LDA */
    SETLO(A, A & 0x04);  /* 0070C5 AND */
    SETLO(A, lsr8(A));  /* 0070C7 LSR */
    SETLO(A, lsr8(A));  /* 0070C8 LSR */
    if (Z) goto L_0070CE;  /* 0070C9 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x08);  /* 0070CB SEB */
L_0070CE: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 0070CE CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 0070D2 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 0070D6 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 0070DA LDA */
    SETLO(A, A & 0x04);  /* 0070DD AND */
    SETLO(A, lsr8(A));  /* 0070DF LSR */
    SETLO(A, lsr8(A));  /* 0070E0 LSR */
    if (Z) goto L_0070E6;  /* 0070E1 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x04);  /* 0070E3 SEB */
L_0070E6: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 0070E6 CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 0070EA LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 0070EE SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 0070F2 LDA */
    SETLO(A, A & 0x04);  /* 0070F5 AND */
    SETLO(A, lsr8(A));  /* 0070F7 LSR */
    SETLO(A, lsr8(A));  /* 0070F8 LSR */
    if (Z) goto L_0070FE;  /* 0070F9 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x02);  /* 0070FB SEB */
L_0070FE: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 0070FE CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 007102 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 007106 SEB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(ABS(P6)));  /* 00710A LDA */
    SETLO(A, A & 0x04);  /* 00710D AND */
    SETLO(A, lsr8(A));  /* 00710F LSR */
    SETLO(A, lsr8(A));  /* 007110 LSR */
    if (Z) goto L_007116;  /* 007111 BEQ */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x01);  /* 007113 SEB */
L_007116: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007116 CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(DP(0x00)));  /* 00711A LDA */
    if ((A & 0xFF) == 0) goto L_007125;  /* 00711C BEQ */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 00711E CLB [P62 GOSTERGE_VERI(SDA) yon] */
    goto L_00712E;  /* 007122 JMP */
L_007125: TICK();
    mflag = 1;  /* 007125 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 007126 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 00712A SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_00712E: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 00712E SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 007132 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007136 CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(DP(0x01)));  /* 00713A LDA */
    D = pull16();  /* 00713C PLD */
    S += 3; return;  /* 00713D RTL */
}

