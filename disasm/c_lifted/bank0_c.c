/* bank0_c.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 00B563  callers: 00B5EE, 00B601, 00B66B, 00C04C */
void F_00B563(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B563 LDA */
    push16(D);  /* 00B565 PHD */
    D = A;  /* 00B566 TAD */
    push16(A);  /* 00B567 PHA */
    mflag = 1;  /* 00B568 SEM */
    wr8(ABS(0x03BA), 0x05);  /* 00B569 LDM */
    mflag = 0;  /* 00B56D CLM */
    wr16(ABS(0x03B6), 0x0009);  /* 00B56E LDM */
    S -= 3; F_00AB35();  /* 00B573 JSRL */
    wr16(ABS(0x03F3), A);  /* 00B577 STA */
    wr16(ABS(0x03F5), A);  /* 00B57A STA */
    mflag = 1;  /* 00B57D SEM */
    wr8(ABS(0x0229), 0x02);  /* 00B57E LDM */
    mflag = 0;  /* 00B582 CLM */
    S -= 3; F_00A01C();  /* 00B583 JSRL */
    mflag = 1;  /* 00B587 SEM */
    wr8(ABS(0x01DE), 0x00);  /* 00B588 LDM */
    wr8(ABS(0x03A3), 0x01);  /* 00B58C LDM */
    wr8(ABS(0x03A8), 0x01);  /* 00B590 LDM */
    wr8(ABS(0x03B8), 0x00);  /* 00B594 LDM */
    wr8(ABS(0x041E), 0x00);  /* 00B598 LDM */
    SETLO(A, rd8(ABS(0x041A)));  /* 00B59C LDA */
    wr8(ABS(0x0421), (uint8_t)A);  /* 00B59F STA */
    wr8(ABS(0x0414), 0x03);  /* 00B5A2 LDM */
    wr8(ABS(0x01BE), 0x01);  /* 00B5A6 LDM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B5AA LDA */
    /* 00B5AD CMP */
    if ((A & 0xFF) != 0x41) goto L_00B5B9;  /* 00B5AF BNE */
    mflag = 0;  /* 00B5B1 CLM */
    S -= 3; F_00AB6E();  /* 00B5B2 JSRL */
    wr16(ABS(0x03D5), A);  /* 00B5B6 STA */
L_00B5B9: TICK();
    X = 0x03C9;  /* 00B5B9 LDX */
    mflag = 0;  /* 00B5BC CLM */
    A = rd16((ABS(0x0002) + X));  /* 00B5BD LDA */
    A &= 0x00FF;  /* 00B5C0 AND */
    wr16(DP(0x00), A);  /* 00B5C3 STA */
    A = rd16((ABS(0x0001) + X));  /* 00B5C5 LDA */
    A &= 0x00FF;  /* 00B5C8 AND */
    A = rla16(A, 0x0008);  /* 00B5CB RLA */
    A &= 0xFF00;  /* 00B5CF AND */
    C = 0;  /* 00B5D2 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00B5D3 ADC */
    A = lsr16(A);  /* 00B5D5 LSR */
    A = lsr16(A);  /* 00B5D6 LSR */
    A = lsr16(A);  /* 00B5D7 LSR */
    wr16(ABS(0x01D3), A);  /* 00B5D8 STA */
    S -= 3; F_00AB6E();  /* 00B5DB JSRL */
    wr16(ABS(0x01D5), A);  /* 00B5DF STA */
    mflag = 1;  /* 00B5E2 SEM */
    wr8(ABS(0x03B9), 0x10);  /* 00B5E3 LDM */
    mflag = 0;  /* 00B5E7 CLM */
    Y = pull16();  /* 00B5E8 PLY */
    D = pull16();  /* 00B5E9 PLD */
    S += 3; return;  /* 00B5EA RTL */
}

/* 00B5EB  callers: 00B64A, 00C03B */
void F_00B5EB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B5EB LDA */
    push16(A);  /* 00B5ED PHA */
    S -= 3; F_00B563();  /* 00B5EE JSRL */
    mflag = 1;  /* 00B5F2 SEM */
    wr8(ABS(0x0414), 0x05);  /* 00B5F3 LDM */
    wr8(ABS(0x01BE), 0x09);  /* 00B5F7 LDM */
    mflag = 0;  /* 00B5FB CLM */
    Y = pull16();  /* 00B5FC PLY */
    S += 3; return;  /* 00B5FD RTL */
}

/* 00B5FE  callers: 00B69B, 00C06C */
void F_00B5FE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B5FE LDA */
    push16(A);  /* 00B600 PHA */
    S -= 3; F_00B563();  /* 00B601 JSRL */
    S -= 3; F_00AB0E();  /* 00B605 JSRL */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 00B609 STA */
    t = rd8(ABS(0x0425));  /* 00B60C CMP */
    if ((A & 0xFF) == t) goto L_00B618;  /* 00B60F BEQ */
    mflag = 0;  /* 00B611 CLM */
    A = 0x1FFD;  /* 00B612 LDA */
    goto L_00B61D;  /* 00B615 JMP */
L_00B618: TICK();
    mflag = 0;  /* 00B618 CLM */
    S -= 3; F_00AB6E();  /* 00B619 JSRL */
L_00B61D: TICK();
    wr16(ABS(0x01CD), A);  /* 00B61D STA */
    mflag = 1;  /* 00B620 SEM */
    wr8(ABS(0x0414), 0x00);  /* 00B621 LDM */
    mflag = 0;  /* 00B625 CLM */
    wr16(ABS(0x03D5), 0x0000);  /* 00B626 LDM */
    mflag = 1;  /* 00B62B SEM */
    wr8(ABS(0x01BE), 0x02);  /* 00B62C LDM */
    mflag = 0;  /* 00B630 CLM */
    Y = pull16();  /* 00B631 PLY */
    S += 3; return;  /* 00B632 RTL */
}

/* 00B633  callers: 00C261, 00C2DA */
void F_00B633(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B633 LDA */
    push16(A);  /* 00B635 PHA */
    A = rd16(ABS(0x03B9));  /* 00B636 LDA */
    mflag = 1;  /* 00B639 SEM */
    /* 00B63A CMP */
    if ((A & 0xFF) != 0x40) goto L_00B651;  /* 00B63C BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B63E LDA */
    /* 00B641 CMP */
    if ((A & 0xFF) == 0x40) goto L_00B649;  /* 00B643 BEQ */
    /* 00B645 CMP */
    if ((A & 0xFF) != 0x50) goto L_00B651;  /* 00B647 BNE */
L_00B649: TICK();
    mflag = 0;  /* 00B649 CLM */
    S -= 3; F_00B5EB();  /* 00B64A JSRL */
    goto L_00B69F;  /* 00B64E JMP */
L_00B651: TICK();
    mflag = 0;  /* 00B651 CLM */
    A = rd16(ABS(0x03B9));  /* 00B652 LDA */
    mflag = 1;  /* 00B655 SEM */
    /* 00B656 CMP */
    if ((A & 0xFF) != 0x41) goto L_00B672;  /* 00B658 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B65A LDA */
    /* 00B65D CMP */
    if ((A & 0xFF) == 0x40) goto L_00B66A;  /* 00B65F BEQ */
    /* 00B661 CMP */
    if ((A & 0xFF) != 0x58) goto L_00B672;  /* 00B663 BNE */
    SETLO(A, rd8(ABS(0x0434)));  /* 00B665 LDA */
    if ((A & 0xFF) != 0) goto L_00B672;  /* 00B668 BNE */
L_00B66A: TICK();
    mflag = 0;  /* 00B66A CLM */
    S -= 3; F_00B563();  /* 00B66B JSRL */
    goto L_00B69F;  /* 00B66F JMP */
L_00B672: TICK();
    mflag = 0;  /* 00B672 CLM */
    A = rd16(ABS(0x03B9));  /* 00B673 LDA */
    mflag = 1;  /* 00B676 SEM */
    /* 00B677 CMP */
    if ((A & 0xFF) != 0x42) goto L_00B69F;  /* 00B679 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B67B LDA */
    /* 00B67E CMP */
    if ((A & 0xFF) != 0x58) goto L_00B689;  /* 00B680 BNE */
    SETLO(A, rd8(ABS(0x0434)));  /* 00B682 LDA */
    /* 00B685 CMP */
    if ((A & 0xFF) == 0x1F) goto L_00B69A;  /* 00B687 BEQ */
L_00B689: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B689 LDA */
    /* 00B68C CMP */
    if ((A & 0xFF) == 0x50) goto L_00B69F;  /* 00B68E BEQ */
    SETLO(A, rd8(ABS(0x03AD)));  /* 00B690 LDA */
    if ((A & 0xFF) == 0) goto L_00B69F;  /* 00B693 BEQ */
    SETLO(A, rd8(ABS(0x03A2)));  /* 00B695 LDA */
    if ((A & 0xFF) != 0) goto L_00B69F;  /* 00B698 BNE */
L_00B69A: TICK();
    mflag = 0;  /* 00B69A CLM */
    S -= 3; F_00B5FE();  /* 00B69B JSRL */
L_00B69F: TICK();
    mflag = 0;  /* 00B69F CLM */
    A = rd16(ABS(0x03A8));  /* 00B6A0 LDA */
    mflag = 1;  /* 00B6A3 SEM */
    Y = pull16();  /* 00B6A4 PLY */
    S += 3; return;  /* 00B6A5 RTL */
}

/* 00B6A6  callers: 00CA8D */
void F_00B6A6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B6A6 LDA */
    push16(D);  /* 00B6A8 PHD */
    D = A;  /* 00B6A9 TAD */
    push16(A);  /* 00B6AA PHA */
    mflag = 1;  /* 00B6AB SEM */
    SETLO(A, rd8(ABS(0x03B5)));  /* 00B6AC LDA */
    if ((A & 0xFF) != 0) goto L_00B6D0;  /* 00B6AF BNE */
    wr8(ABS(0x03B5), 0x10);  /* 00B6B1 LDM */
    wr8(ABS(0x01BE), 0x03);  /* 00B6B5 LDM */
    wr8(DP(0x00), 0x01);  /* 00B6B9 LDM */
    mflag = 0;  /* 00B6BC CLM */
    wr16(DP(0x01), 0x03B5);  /* 00B6BD LDM */
    wr16(DP(0x03), 0x0000);  /* 00B6C1 LDM */
    wr16(DP(0x04), 0x055E);  /* 00B6C5 LDM */
    Y = 0x00A0;  /* 00B6C9 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00B6CC JSRL */
L_00B6D0: TICK();
    mflag = 0;  /* 00B6D0 CLM */
    Y = pull16();  /* 00B6D1 PLY */
    D = pull16();  /* 00B6D2 PLD */
    S += 3; return;  /* 00B6D3 RTL */
}

/* 00B6D4  callers: 00CA7F */
void F_00B6D4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B6D4 LDA */
    push16(D);  /* 00B6D6 PHD */
    D = A;  /* 00B6D7 TAD */
    push16(A);  /* 00B6D8 PHA */
    A = rd16(ABS(0x03B5));  /* 00B6D9 LDA */
    mflag = 1;  /* 00B6DC SEM */
    /* 00B6DD CMP */
    if ((A & 0xFF) != 0x10) goto L_00B701;  /* 00B6DF BNE */
    wr8(ABS(0x03B5), 0x00);  /* 00B6E1 LDM */
    mflag = 0;  /* 00B6E5 CLM */
    S -= 3; F_00A3A8();  /* 00B6E6 JSRL */
    wr16(DP(0x00), 0x0001);  /* 00B6EA LDM */
    wr16(DP(0x01), 0x03B5);  /* 00B6EE LDM */
    wr16(DP(0x03), 0x0000);  /* 00B6F2 LDM */
    wr16(DP(0x04), 0x055E);  /* 00B6F6 LDM */
    Y = 0x00A0;  /* 00B6FA LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00B6FD JSRL */
L_00B701: TICK();
    mflag = 0;  /* 00B701 CLM */
    Y = pull16();  /* 00B702 PLY */
    D = pull16();  /* 00B703 PLD */
    S += 3; return;  /* 00B704 RTL */
}

/* 00B705  callers: 00B71B, 00C025 */
void F_00B705(void)
{
    uint32_t t; (void)t;
    wr16(ABS(0x03B6), 0x0005);  /* 00B705 LDM */
    S -= 3; F_00A889();  /* 00B70A JSRL */
    S += 3; return;  /* 00B70E RTL */
}

/* 00B70F  callers: 00C26A, 00C2E3 */
void F_00B70F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B70F LDA */
    push16(A);  /* 00B711 PHA */
    A = rd16(ABS(0x03B9));  /* 00B712 LDA */
    mflag = 1;  /* 00B715 SEM */
    /* 00B716 CMP */
    if ((A & 0xFF) != 0x12) goto L_00B725;  /* 00B718 BNE */
    mflag = 0;  /* 00B71A CLM */
    S -= 3; F_00B705();  /* 00B71B JSRL */
    A = 0x0001;  /* 00B71F LDA */
    mflag = 1;  /* 00B722 SEM */
    Y = pull16();  /* 00B723 PLY */
    S += 3; return;  /* 00B724 RTL */
L_00B725: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B725 LDA */
    /* 00B728 CMP */
    if ((A & 0xFF) != 0x85) goto L_00B73F;  /* 00B72A BNE */
    mflag = 0;  /* 00B72C CLM */
    wr16(ABS(0x03B6), 0x0003);  /* 00B72D LDM */
    S -= 3; F_00A51C();  /* 00B732 JSRL */
    mflag = 1;  /* 00B736 SEM */
    wr8(ABS(0x01BE), 0x04);  /* 00B737 LDM */
    SETLO(A, 0x01);  /* 00B73B LDA */
    Y = pull16();  /* 00B73D PLY */
    S += 3; return;  /* 00B73E RTL */
L_00B73F: TICK();
    /* 00B73F CMP */
    if ((A & 0xFF) != 0x86) goto L_00B772;  /* 00B741 BNE */
    wr8(ABS(0x01BE), 0x06);  /* 00B743 LDM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B747 LDA */
    /* 00B74A CMP */
    if ((A & 0xFF) != 0x54) goto L_00B75B;  /* 00B74C BNE */
    mflag = 0;  /* 00B74E CLM */
    wr16(ABS(0x03FF), 0x0000);  /* 00B74F LDM */
    S -= 3; F_00A399();  /* 00B754 JSRL */
    goto L_00B76B;  /* 00B758 JMP */
L_00B75B: TICK();
    mflag = 1;  /* 00B75B SEM */
    wr8(ABS(0x03B8), 0x00);  /* 00B75C LDM */
    mflag = 0;  /* 00B760 CLM */
    wr16(ABS(0x03B6), 0x0004);  /* 00B761 LDM */
    mflag = 1;  /* 00B766 SEM */
    wr8(ABS(0x041F), 0x3F);  /* 00B767 LDM */
L_00B76B: TICK();
    mflag = 0;  /* 00B76B CLM */
    A = 0x0001;  /* 00B76C LDA */
    mflag = 1;  /* 00B76F SEM */
    Y = pull16();  /* 00B770 PLY */
    S += 3; return;  /* 00B771 RTL */
L_00B772: TICK();
    /* 00B772 CMP */
    if ((A & 0xFF) != 0x87) goto L_00B7B1;  /* 00B774 BNE */
    mflag = 0;  /* 00B776 CLM */
    wr16(ABS(0x03B6), 0x0004);  /* 00B777 LDM */
    mflag = 1;  /* 00B77C SEM */
    wr8(ABS(0x041F), 0x3F);  /* 00B77D LDM */
    wr8(ABS(0x01BE), 0x07);  /* 00B781 LDM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B785 LDA */
    /* 00B788 CMP */
    if ((A & 0xFF) != 0x54) goto L_00B7A5;  /* 00B78A BNE */
    Y = rd16(ABS(0x0426));  /* 00B78C LDY */
    mflag = 0;  /* 00B78F CLM */
    S -= 3; F_00A10E();  /* 00B790 JSRL */
    Y = A;  /* 00B794 TAY */
    S -= 3; F_00A244();  /* 00B795 JSRL */
    wr16(ABS(0x03FF), 0x0000);  /* 00B799 LDM */
    S -= 3; F_00A399();  /* 00B79E JSRL */
    goto L_00B7AA;  /* 00B7A2 JMP */
L_00B7A5: TICK();
    mflag = 1;  /* 00B7A5 SEM */
    wr8(ABS(0x03B8), 0x00);  /* 00B7A6 LDM */
L_00B7AA: TICK();
    mflag = 0;  /* 00B7AA CLM */
    A = 0x0001;  /* 00B7AB LDA */
    mflag = 1;  /* 00B7AE SEM */
    Y = pull16();  /* 00B7AF PLY */
    S += 3; return;  /* 00B7B0 RTL */
L_00B7B1: TICK();
    /* 00B7B1 CMP */
    if ((A & 0xFF) == 0x88) goto L_00B7C1;  /* 00B7B3 BEQ */
    /* 00B7B5 CMP */
    if ((A & 0xFF) == 0x89) goto L_00B7C1;  /* 00B7B7 BEQ */
    /* 00B7B9 CMP */
    if ((A & 0xFF) == 0x8C) goto L_00B7C1;  /* 00B7BB BEQ */
    /* 00B7BD CMP */
    if ((A & 0xFF) != 0x8B) goto L_00B7E5;  /* 00B7BF BNE */
L_00B7C1: TICK();
    mflag = 0;  /* 00B7C1 CLM */
    wr16(ABS(0x03B6), 0x0004);  /* 00B7C2 LDM */
    mflag = 1;  /* 00B7C7 SEM */
    wr8(ABS(0x041F), 0x3F);  /* 00B7C8 LDM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B7CC LDA */
    /* 00B7CF CMP */
    if ((A & 0xFF) != 0x88) goto L_00B7D8;  /* 00B7D1 BNE */
    SETLO(A, 0x15);  /* 00B7D3 LDA */
    goto L_00B7DA;  /* 00B7D5 JMP */
L_00B7D8: TICK();
    SETLO(A, 0x08);  /* 00B7D8 LDA */
L_00B7DA: TICK();
    wr8(ABS(0x01BE), (uint8_t)A);  /* 00B7DA STA */
    wr8(ABS(0x03B8), 0x00);  /* 00B7DD LDM */
    SETLO(A, 0x01);  /* 00B7E1 LDA */
    Y = pull16();  /* 00B7E3 PLY */
    S += 3; return;  /* 00B7E4 RTL */
L_00B7E5: TICK();
    SETLO(A, 0x00);  /* 00B7E5 LDA */
    Y = pull16();  /* 00B7E7 PLY */
    S += 3; return;  /* 00B7E8 RTL */
}

/* 00B7E9  callers: 00BF7F, 00C1B7, 00C2C8 */
void F_00B7E9(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x03B9));  /* 00B7E9 LDA */
    mflag = 1;  /* 00B7EC SEM */
    /* 00B7ED CMP */
    if ((A & 0xFF) != 0x07) goto L_00B7FE;  /* 00B7EF BNE */
    mflag = 0;  /* 00B7F1 CLM */
    S -= 3; F_00A889();  /* 00B7F2 JSRL */
    mflag = 1;  /* 00B7F6 SEM */
    wr8(ABS(0x01BE), 0x0A);  /* 00B7F7 LDM */
    SETLO(A, 0x01);  /* 00B7FB LDA */
    S += 3; return;  /* 00B7FD RTL */
L_00B7FE: TICK();
    SETLO(A, 0x00);  /* 00B7FE LDA */
    S += 3; return;  /* 00B800 RTL */
}

/* 00B801  callers: 00BE65, 00C094, 00C1A5, 00C2D1 */
void F_00B801(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B801 LDA */
    push16(A);  /* 00B803 PHA */
    A = rd16(ABS(0x03B9));  /* 00B804 LDA */
    mflag = 1;  /* 00B807 SEM */
    /* 00B808 CMP */
    if ((A & 0xFF) == 0x09) goto L_00B81B;  /* 00B80A BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B80C LDA */
    /* 00B80F CMP */
    if ((A & 0xFF) == 0x0B) goto L_00B81B;  /* 00B811 BEQ */
    /* 00B813 CMP */
    if ((A & 0xFF) == 0x0C) goto L_00B81B;  /* 00B815 BEQ */
    /* 00B817 CMP */
    if ((A & 0xFF) != 0x0D) goto L_00B826;  /* 00B819 BNE */
L_00B81B: TICK();
    mflag = 0;  /* 00B81B CLM */
    S -= 3; F_00A605();  /* 00B81C JSRL */
    A = 0x0001;  /* 00B820 LDA */
    mflag = 1;  /* 00B823 SEM */
    Y = pull16();  /* 00B824 PLY */
    S += 3; return;  /* 00B825 RTL */
L_00B826: TICK();
    /* 00B826 CMP */
    if ((A & 0xFF) != 0x0A) goto L_00B82E;  /* 00B828 BNE */
    SETLO(A, 0x01);  /* 00B82A LDA */
    Y = pull16();  /* 00B82C PLY */
    S += 3; return;  /* 00B82D RTL */
L_00B82E: TICK();
    SETLO(A, 0x00);  /* 00B82E LDA */
    Y = pull16();  /* 00B830 PLY */
    S += 3; return;  /* 00B831 RTL */
}

/* 00B832  callers: 00C082, 00C1AE, 00C2BF */
void F_00B832(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00B832 SEM */
    SETLO(A, rd8(ABS(0x0433)));  /* 00B833 LDA */
    if ((A & 0xFF) != 0) goto L_00B89E;  /* 00B836 BNE */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B838 LDA */
    /* 00B83B CMP */
    if ((A & 0xFF) == 0x01) goto L_00B843;  /* 00B83D BEQ */
    /* 00B83F CMP */
    if ((A & 0xFF) != 0x02) goto L_00B89E;  /* 00B841 BNE */
L_00B843: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B843 LDA */
    /* 00B846 CMP */
    if ((A & 0xFF) == 0x50) goto L_00B85B;  /* 00B848 BEQ */
    /* 00B84A CMP */
    if ((A & 0xFF) != 0x58) goto L_00B855;  /* 00B84C BNE */
    SETLO(A, rd8(ABS(0x0434)));  /* 00B84E LDA */
    /* 00B851 CMP */
    if ((A & 0xFF) == 0x1F) goto L_00B85B;  /* 00B853 BEQ */
L_00B855: TICK();
    mflag = 0;  /* 00B855 CLM */
    X = rd16(ABS(0x03D5));  /* 00B856 LDX */
    if (X != 0) goto L_00B863;  /* 00B859 BNE */
L_00B85B: TICK();
    mflag = 0;  /* 00B85B CLM */
    S -= 3; F_00A8CF();  /* 00B85C JSRL */
    goto L_00B896;  /* 00B860 JMP */
L_00B863: TICK();
    mflag = 0;  /* 00B863 CLM */
    wr16(ABS(0x03B6), 0x0004);  /* 00B864 LDM */
    mflag = 1;  /* 00B869 SEM */
    wr8(ABS(0x03A3), 0x00);  /* 00B86A LDM */
    wr8(ABS(0x041E), 0x3C);  /* 00B86E LDM */
    wr8(ABS(0x01BE), 0x0E);  /* 00B872 LDM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B876 LDA */
    /* 00B879 CMP */
    if ((A & 0xFF) == 0x01) goto L_00B889;  /* 00B87B BEQ */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B87D LDA */
    /* 00B880 CMP */
    if ((A & 0xFF) != 0x58) goto L_00B891;  /* 00B882 BNE */
    SETLO(A, rd8(ABS(0x0434)));  /* 00B884 LDA */
    if ((A & 0xFF) != 0) goto L_00B891;  /* 00B887 BNE */
L_00B889: TICK();
    mflag = 0;  /* 00B889 CLM */
    S -= 3; F_00A889();  /* 00B88A JSRL */
    goto L_00B896;  /* 00B88E JMP */
L_00B891: TICK();
    mflag = 0;  /* 00B891 CLM */
    S -= 3; F_00A8A8();  /* 00B892 JSRL */
L_00B896: TICK();
    mflag = 1;  /* 00B896 SEM */
    wr8(ABS(0x03B9), 0x10);  /* 00B897 LDM */
    SETLO(A, 0x01);  /* 00B89B LDA */
    S += 3; return;  /* 00B89D RTL */
L_00B89E: TICK();
    SETLO(A, 0x00);  /* 00B89E LDA */
    S += 3; return;  /* 00B8A0 RTL */
}

/* 00B8A1  callers: 00C08B */
void F_00B8A1(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x03B9));  /* 00B8A1 LDA */
    mflag = 1;  /* 00B8A4 SEM */
    /* 00B8A5 CMP */
    if ((A & 0xFF) != 0x08) goto L_00B8B7;  /* 00B8A7 BNE */
    wr8(ABS(0x01BE), 0x14);  /* 00B8A9 LDM */
    mflag = 0;  /* 00B8AD CLM */
    S -= 3; F_00A889();  /* 00B8AE JSRL */
    A = 0x0001;  /* 00B8B2 LDA */
    mflag = 1;  /* 00B8B5 SEM */
    S += 3; return;  /* 00B8B6 RTL */
L_00B8B7: TICK();
    SETLO(A, 0x00);  /* 00B8B7 LDA */
    S += 3; return;  /* 00B8B9 RTL */
}

/* 00B8BA  callers: 00BE85, 00C014, 00C9F0 */
void F_00B8BA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B8BA LDA */
    push16(A);  /* 00B8BC PHA */
    mflag = 1;  /* 00B8BD SEM */
    wr8(ABS(0x03B9), 0x10);  /* 00B8BE LDM */
    mflag = 0;  /* 00B8C2 CLM */
    S -= 3; F_00AC7B();  /* 00B8C3 JSRL */
    X = A;  /* 00B8C7 TAX */
    if (X != 0) goto L_00B8DA;  /* 00B8C8 BNE */
    A = rd16(ABS(0x03BA));  /* 00B8CA LDA */
    mflag = 1;  /* 00B8CD SEM */
    /* 00B8CE CMP */
    if ((A & 0xFF) == 0x02) goto L_00B8D7;  /* 00B8D0 BEQ */
    mflag = 0;  /* 00B8D2 CLM */
    S -= 3; F_00A4E9();  /* 00B8D3 JSRL */
L_00B8D7: TICK();
    goto L_00B8E9;  /* 00B8D7 JMP */
L_00B8DA: TICK();
    mflag = 0;  /* 00B8DA CLM */
    S -= 3; F_00AC7B();  /* 00B8DB JSRL */
    wr16(ABS(0x03F1), A);  /* 00B8DF STA */
    wr16(ABS(0x03F5), A);  /* 00B8E2 STA */
    S -= 3; F_00A4E9();  /* 00B8E5 JSRL */
L_00B8E9: TICK();
    mflag = 0;  /* 00B8E9 CLM */
    Y = pull16();  /* 00B8EA PLY */
    S += 3; return;  /* 00B8EB RTL */
}

/* 00B8EC  callers: 00BD85, 00BDBE, 00BDFB, 00BEA1 */
void F_00B8EC(void)
{
    uint32_t t; (void)t;
    wr16(ABS(0x03B6), 0x000A);  /* 00B8EC LDM */
    mflag = 1;  /* 00B8F1 SEM */
    wr8(ABS(0x041D), 0x02);  /* 00B8F2 LDM */
    wr8(ABS(0x0229), 0x03);  /* 00B8F6 LDM */
    mflag = 0;  /* 00B8FA CLM */
    S += 3; return;  /* 00B8FB RTL */
}

/* 00B8FC  callers: 00C19C, 00C2B6 */
void F_00B8FC(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00B8FC SEM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00B8FD LDA */
    if ((A & 0xFF) != 0) goto L_00B940;  /* 00B900 BNE */
    mflag = 0;  /* 00B902 CLM */
    X = rd16(ABS(0x03B6));  /* 00B903 LDX */
    /* 00B906 CPX */
    if (X != 0x0003) goto L_00B910;  /* 00B909 BNE */
    mflag = 1;  /* 00B90B SEM */
    wr8(ABS(0x01BE), 0x10);  /* 00B90C LDM */
L_00B910: TICK();
    mflag = 0;  /* 00B910 CLM */
    wr16(ABS(0x03B6), 0x0003);  /* 00B911 LDM */
    S -= 3; F_00A51C();  /* 00B916 JSRL */
    A = rd16(ABS(0x03B8));  /* 00B91A LDA */
    mflag = 1;  /* 00B91D SEM */
    /* 00B91E CMP */
    if ((A & 0xFF) == 0x50) goto L_00B933;  /* 00B920 BEQ */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00B922 LDA */
    /* 00B925 CMP */
    if ((A & 0xFF) != 0x40) goto L_00B93B;  /* 00B927 BNE */
    X = 0x03C9;  /* 00B929 LDX */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B92C LDA */
    SETLO(A, A & 0x02);  /* 00B92F AND */
    if ((A & 0xFF) != 0) goto L_00B93B;  /* 00B931 BNE */
L_00B933: TICK();
    mflag = 0;  /* 00B933 CLM */
    S -= 3; F_00A889();  /* 00B934 JSRL */
    goto L_00B940;  /* 00B938 JMP */
L_00B93B: TICK();
    mflag = 0;  /* 00B93B CLM */
    S -= 3; F_00A8CF();  /* 00B93C JSRL */
L_00B940: TICK();
    mflag = 0;  /* 00B940 CLM */
    A = rd16(ABS(0x03AE));  /* 00B941 LDA */
    mflag = 1;  /* 00B944 SEM */
    S += 3; return;  /* 00B945 RTL */
}

/* 00B946  callers: 00C276 */
void F_00B946(void)
{
    uint32_t t; (void)t;
    S -= 3; F_00AAAC();  /* 00B946 JSRL */
    /* 00B94A CMP */
    if ((A & 0xFF) != 0x00) goto L_00B94F;  /* 00B94C BNE */
    S += 3; return;  /* 00B94E RTL */
L_00B94F: TICK();
    SETLO(A, rd8(ABS(0x03A4)));  /* 00B94F LDA */
    if ((A & 0xFF) == 0) goto L_00B967;  /* 00B952 BEQ */
    SETLO(A, rd8(ABS(0x0438)));  /* 00B954 LDA */
    if ((A & 0xFF) != 0) goto L_00B95A;  /* 00B957 BNE */
    S += 3; return;  /* 00B959 RTL */
L_00B95A: TICK();
    wr8(ABS(0x03A4), 0x00);  /* 00B95A LDM */
    SETLO(A, rd8(ABS(0x0436)));  /* 00B95E LDA */
    wr8(ABS(0x0435), (uint8_t)A);  /* 00B961 STA */
    SETLO(A, 0x01);  /* 00B964 LDA */
    S += 3; return;  /* 00B966 RTL */
L_00B967: TICK();
    SETLO(A, rd8(ABS(0x0435)));  /* 00B967 LDA */
    if ((A & 0xFF) != 0) goto L_00B97E;  /* 00B96A BNE */
    SETLO(A, rd8(ABS(0x0438)));  /* 00B96C LDA */
    t = rd8(ABS(0x0437));  /* 00B96F CMP */
    if ((A & 0xFF) != t) goto L_00B97E;  /* 00B972 BNE */
    SETLO(A, rd8(ABS(0x0437)));  /* 00B974 LDA */
    if ((A & 0xFF) == 0) goto L_00B97E;  /* 00B977 BEQ */
    SETLO(A, rd8(ABS(0x0415)));  /* 00B979 LDA */
    if ((A & 0xFF) != 0) goto L_00B981;  /* 00B97C BNE */
L_00B97E: TICK();
    SETLO(A, 0x00);  /* 00B97E LDA */
    S += 3; return;  /* 00B980 RTL */
L_00B981: TICK();
    SETLO(A, rd8(ABS(0x0436)));  /* 00B981 LDA */
    wr8(ABS(0x0435), (uint8_t)A);  /* 00B984 STA */
    t = (rd8(ABS(0x0415)) - 1) & 0xFF; wr8(ABS(0x0415), t);  /* 00B987 DEC */
    SETLO(A, 0x01);  /* 00B98A LDA */
    S += 3; return;  /* 00B98C RTL */
}

/* 00B98D  callers: 00C9D9, 00CAFE, 01A744 */
void F_00B98D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00B98D LDA */
    push16(D);  /* 00B98F PHD */
    D = A;  /* 00B990 TAD */
    C = 0;  /* 00B991 CLC */
    A = adc16(A, 0x0004);  /* 00B992 ADC */
    push16(A);  /* 00B995 PHA */
    wr16(DP(0x00), 0x0000);  /* 00B996 LDM */
    wr16(DP(0x02), 0x0040);  /* 00B99A LDM */
    Y = 0x0002;  /* 00B99E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00B9A1 JSRL */
    wr8(ABS(0x0435), 0x04);  /* 00B9A5 LDM */
    wr8(ABS(0x0438), 0x00);  /* 00B9A9 LDM */
L_00B9AD: TICK();
    SETLO(A, rd8(ABS(0x0438)));  /* 00B9AD LDA */
    /* 00B9B0 CMP */
    if ((A & 0xFF) >= 0x0A) goto L_00BA0B;  /* 00B9B2 BCS */
    mpy8(0x03);  /* 00B9B4 MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 00B9B7 STA */
    wr8(DP(0x01), (uint8_t)B);  /* 00B9B9 STA */
    mflag = 0;  /* 00B9BC CLM */
    A = rd16(DP(0x00));  /* 00B9BD LDA */
    C = 0;  /* 00B9BF CLC */
    A = adc16(A, 0x01D6);  /* 00B9C0 ADC */
    Y = A;  /* 00B9C3 TAY */
    A = D;  /* 00B9C4 TDA */
    push16(A);  /* 00B9C5 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00B9C6 JSRL */
    Y = pull16();  /* 00B9CA PLY */
    wr8(DP(0x00), (uint8_t)A);  /* 00B9CB STA */
    SETLO(A, rd8(ABS(0x0438)));  /* 00B9CD LDA */
    mflag = 0;  /* 00B9D0 CLM */
    A &= 0x00FF;  /* 00B9D1 AND */
    X = A;  /* 00B9D4 TAX */
    A = rd16(DP(0x00));  /* 00B9D5 LDA */
    mflag = 1;  /* 00B9D7 SEM */
    wr8((ABS(0x03BC) + X), (uint8_t)A);  /* 00B9D8 STA */
    SETLO(A, rd8(ABS(0x0438)));  /* 00B9DB LDA */
    mpy8(0x03);  /* 00B9DE MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 00B9E1 STA */
    wr8(DP(0x01), (uint8_t)B);  /* 00B9E3 STA */
    mflag = 0;  /* 00B9E6 CLM */
    A = rd16(DP(0x00));  /* 00B9E7 LDA */
    C = 0;  /* 00B9E9 CLC */
    A = adc16(A, 0x01D7);  /* 00B9EA ADC */
    Y = A;  /* 00B9ED TAY */
    A = D;  /* 00B9EE TDA */
    push16(A);  /* 00B9EF PHA */
    S -= 3; F_007271();  /* 00B9F0 JSRL */
    Y = pull16();  /* 00B9F4 PLY */
    wr16(DP(0x00), A);  /* 00B9F5 STA */
    A = rd16(ABS(0x0438));  /* 00B9F7 LDA */
    A &= 0x00FF;  /* 00B9FA AND */
    A = asl16(A);  /* 00B9FD ASL */
    X = A;  /* 00B9FE TAX */
    A = rd16(DP(0x00));  /* 00B9FF LDA */
    wr16((ABS(0x03D9) + X), A);  /* 00BA01 STA */
    mflag = 1;  /* 00BA04 SEM */
    t = (rd8(ABS(0x0438)) + 1) & 0xFF; wr8(ABS(0x0438), t);  /* 00BA05 INC */
    goto L_00B9AD;  /* 00BA08 JMP */
L_00BA0B: TICK();
    mflag = 1;  /* 00BA0B SEM */
    wr8(ABS(0x0438), 0x00);  /* 00BA0C LDM */
    wr8(ABS(0x0428), 0x01);  /* 00BA10 LDM */
    Y = 0x01BC;  /* 00BA14 LDY */
    mflag = 0;  /* 00BA17 CLM */
    A = D;  /* 00BA18 TDA */
    push16(A);  /* 00BA19 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA1A JSRL */
    Y = pull16();  /* 00BA1E PLY */
    wr8(ABS(0x0417), (uint8_t)A);  /* 00BA1F STA */
    Y = 0x01BD;  /* 00BA22 LDY */
    mflag = 0;  /* 00BA25 CLM */
    A = D;  /* 00BA26 TDA */
    push16(A);  /* 00BA27 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA28 JSRL */
    Y = pull16();  /* 00BA2C PLY */
    wr8(ABS(0x0418), (uint8_t)A);  /* 00BA2D STA */
    Y = 0x01BE;  /* 00BA30 LDY */
    mflag = 0;  /* 00BA33 CLM */
    A = D;  /* 00BA34 TDA */
    push16(A);  /* 00BA35 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA36 JSRL */
    Y = pull16();  /* 00BA3A PLY */
    wr8(ABS(0x0419), (uint8_t)A);  /* 00BA3B STA */
    Y = 0x01BF;  /* 00BA3E LDY */
    mflag = 0;  /* 00BA41 CLM */
    A = D;  /* 00BA42 TDA */
    push16(A);  /* 00BA43 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA44 JSRL */
    Y = pull16();  /* 00BA48 PLY */
    wr8(ABS(0x041A), (uint8_t)A);  /* 00BA49 STA */
    Y = 0x01C0;  /* 00BA4C LDY */
    mflag = 0;  /* 00BA4F CLM */
    A = D;  /* 00BA50 TDA */
    push16(A);  /* 00BA51 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA52 JSRL */
    Y = pull16();  /* 00BA56 PLY */
    wr8(ABS(0x041B), (uint8_t)A);  /* 00BA57 STA */
    Y = 0x01C1;  /* 00BA5A LDY */
    mflag = 0;  /* 00BA5D CLM */
    A = D;  /* 00BA5E TDA */
    push16(A);  /* 00BA5F PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA60 JSRL */
    Y = pull16();  /* 00BA64 PLY */
    mpy8(0x0A);  /* 00BA65 MPY */
    wr8(ABS(0x041C), (uint8_t)A);  /* 00BA68 STA */
    Y = 0x0218;  /* 00BA6B LDY */
    mflag = 0;  /* 00BA6E CLM */
    A = D;  /* 00BA6F TDA */
    push16(A);  /* 00BA70 PHA */
    S -= 3; F_007271();  /* 00BA71 JSRL */
    Y = pull16();  /* 00BA75 PLY */
    wr16(ABS(0x03F9), A);  /* 00BA76 STA */
    Y = 0x01C6;  /* 00BA79 LDY */
    A = D;  /* 00BA7C TDA */
    push16(A);  /* 00BA7D PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BA7E JSRL */
    Y = pull16();  /* 00BA82 PLY */
    wr8(ABS(0x0425), (uint8_t)A);  /* 00BA83 STA */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 00BA86 STA */
    Y = 0x01D4;  /* 00BA89 LDY */
    mflag = 0;  /* 00BA8C CLM */
    A = D;  /* 00BA8D TDA */
    push16(A);  /* 00BA8E PHA */
    S -= 3; F_007271();  /* 00BA8F JSRL */
    Y = pull16();  /* 00BA93 PLY */
    wr16(ABS(0x03FB), A);  /* 00BA94 STA */
    Y = 0x01A3;  /* 00BA97 LDY */
    A = D;  /* 00BA9A TDA */
    push16(A);  /* 00BA9B PHA */
    S -= 3; F_007271();  /* 00BA9C JSRL */
    Y = pull16();  /* 00BAA0 PLY */
    wr16(ABS(0x03ED), A);  /* 00BAA1 STA */
    Y = 0x015F;  /* 00BAA4 LDY */
    A = D;  /* 00BAA7 TDA */
    push16(A);  /* 00BAA8 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BAA9 JSRL */
    Y = pull16();  /* 00BAAD PLY */
    wr8(ABS(0x0427), (uint8_t)A);  /* 00BAAE STA */
    Y = 0x0160;  /* 00BAB1 LDY */
    mflag = 0;  /* 00BAB4 CLM */
    A = D;  /* 00BAB5 TDA */
    push16(A);  /* 00BAB6 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BAB7 JSRL */
    Y = pull16();  /* 00BABB PLY */
    wr8(ABS(0x0426), (uint8_t)A);  /* 00BABC STA */
    Y = 0x01B1;  /* 00BABF LDY */
    mflag = 0;  /* 00BAC2 CLM */
    A = D;  /* 00BAC3 TDA */
    push16(A);  /* 00BAC4 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BAC5 JSRL */
    Y = pull16();  /* 00BAC9 PLY */
    wr8(ABS(0x040E), (uint8_t)A);  /* 00BACA STA */
    Y = 0x01B8;  /* 00BACD LDY */
    mflag = 0;  /* 00BAD0 CLM */
    A = D;  /* 00BAD1 TDA */
    push16(A);  /* 00BAD2 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BAD3 JSRL */
    Y = pull16();  /* 00BAD7 PLY */
    wr8(ABS(0x040F), (uint8_t)A);  /* 00BAD8 STA */
    Y = 0x01BB;  /* 00BADB LDY */
    mflag = 0;  /* 00BADE CLM */
    A = D;  /* 00BADF TDA */
    push16(A);  /* 00BAE0 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BAE1 JSRL */
    Y = pull16();  /* 00BAE5 PLY */
    wr8(ABS(0x0410), (uint8_t)A);  /* 00BAE6 STA */
    Y = 0x01A1;  /* 00BAE9 LDY */
    mflag = 0;  /* 00BAEC CLM */
    A = D;  /* 00BAED TDA */
    push16(A);  /* 00BAEE PHA */
    S -= 3; F_007271();  /* 00BAEF JSRL */
    Y = pull16();  /* 00BAF3 PLY */
    wr16(ABS(0x03EF), A);  /* 00BAF4 STA */
    Y = 0x01AF;  /* 00BAF7 LDY */
    A = D;  /* 00BAFA TDA */
    push16(A);  /* 00BAFB PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BAFC JSRL */
    Y = pull16();  /* 00BB00 PLY */
    wr8(ABS(0x0411), (uint8_t)A);  /* 00BB01 STA */
    Y = 0x01B6;  /* 00BB04 LDY */
    mflag = 0;  /* 00BB07 CLM */
    A = D;  /* 00BB08 TDA */
    push16(A);  /* 00BB09 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BB0A JSRL */
    Y = pull16();  /* 00BB0E PLY */
    wr8(ABS(0x0412), (uint8_t)A);  /* 00BB0F STA */
    Y = 0x01BA;  /* 00BB12 LDY */
    mflag = 0;  /* 00BB15 CLM */
    A = D;  /* 00BB16 TDA */
    push16(A);  /* 00BB17 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BB18 JSRL */
    Y = pull16();  /* 00BB1C PLY */
    wr8(ABS(0x0413), (uint8_t)A);  /* 00BB1D STA */
    Y = 0x01B4;  /* 00BB20 LDY */
    mflag = 0;  /* 00BB23 CLM */
    A = D;  /* 00BB24 TDA */
    push16(A);  /* 00BB25 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BB26 JSRL */
    Y = pull16();  /* 00BB2A PLY */
    wr8(ABS(0x03B2), (uint8_t)A);  /* 00BB2B STA */
    Y = 0x01B3;  /* 00BB2E LDY */
    mflag = 0;  /* 00BB31 CLM */
    A = D;  /* 00BB32 TDA */
    push16(A);  /* 00BB33 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BB34 JSRL */
    Y = pull16();  /* 00BB38 PLY */
    wr8(ABS(0x03B3), (uint8_t)A);  /* 00BB39 STA */
    Y = 0x013E;  /* 00BB3C LDY */
    mflag = 0;  /* 00BB3F CLM */
    A = D;  /* 00BB40 TDA */
    push16(A);  /* 00BB41 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00BB42 JSRL */
    Y = pull16();  /* 00BB46 PLY */
    wr8(ABS(0x03B4), (uint8_t)A);  /* 00BB47 STA */
    Y = 0x055A;  /* 00BB4A LDY */
    mflag = 0;  /* 00BB4D CLM */
    A = D;  /* 00BB4E TDA */
    push16(A);  /* 00BB4F PHA */
    S -= 3; F_007271();  /* 00BB50 JSRL */
    Y = pull16();  /* 00BB54 PLY */
    A &= 0x01FF;  /* 00BB55 AND */
    wr16(ABS(0x03FF), A);  /* 00BB58 STA */
    mflag = 1;  /* 00BB5B SEM */
    wr8(ABS(0x0432), 0x00);  /* 00BB5C LDM */
    wr8(ABS(0x03C6), 0x00);  /* 00BB60 LDM */
    wr8(ABS(0x03BA), 0x00);  /* 00BB64 LDM */
    mflag = 0;  /* 00BB68 CLM */
    S -= 3; F_00A280();  /* 00BB69 JSRL */
    Y = pull16();  /* 00BB6D PLY */
    D = pull16();  /* 00BB6E PLD */
    S += 3; return;  /* 00BB6F RTL */
}

/* 00BB70  callers: 00CB09 */
void F_00BB70(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BB70 LDA */
    push16(D);  /* 00BB72 PHD */
    D = A;  /* 00BB73 TAD */
    C = 0;  /* 00BB74 CLC */
    A = adc16(A, 0x0004);  /* 00BB75 ADC */
    push16(A);  /* 00BB78 PHA */
    wr16(DP(0x00), 0x0000);  /* 00BB79 LDM */
    wr16(DP(0x02), 0x0040);  /* 00BB7D LDM */
    Y = 0x0002;  /* 00BB81 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00BB84 JSRL */
    wr8(ABS(0x0409), 0x08);  /* 00BB88 LDM */
    wr8(ABS(0x03B9), 0x11);  /* 00BB8C LDM */
    wr8(ABS(0x0229), 0x01);  /* 00BB90 LDM */
    mflag = 0;  /* 00BB94 CLM */
    A = D;  /* 00BB95 TDA */
    push16(A);  /* 00BB96 PHA */
    S -= 3; F_00A399();  /* 00BB97 JSRL */
    Y = pull16();  /* 00BB9B PLY */
    A = rd16(ABS(0x0425));  /* 00BB9C LDA */
    mflag = 1;  /* 00BB9F SEM */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 00BBA0 STA */
    mflag = 0;  /* 00BBA3 CLM */
    wr16(ABS(0x0443), 0x0000);  /* 00BBA4 LDM */
    mflag = 1;  /* 00BBA9 SEM */
    wr8(ABS(0x0433), 0x00);  /* 00BBAA LDM */
    wr8(ABS(0x040A), 0x00);  /* 00BBAE LDM */
    wr8(ABS(0x0422), 0x00);  /* 00BBB2 LDM */
    wr8(ABS(0x041F), 0x00);  /* 00BBB6 LDM */
    wr8(ABS(0x041E), 0x00);  /* 00BBBA LDM */
    wr8(ABS(0x03A8), 0x00);  /* 00BBBE LDM */
    wr8(ABS(0x03AE), 0x00);  /* 00BBC2 LDM */
    wr8(ABS(0x03A9), 0x00);  /* 00BBC6 LDM */
    wr8(ABS(0x03AA), 0x00);  /* 00BBCA LDM */
    wr8(ABS(0x03A5), 0x00);  /* 00BBCE LDM */
    wr8(ABS(0x03B0), 0x00);  /* 00BBD2 LDM */
    wr8(ABS(0x03A6), 0x00);  /* 00BBD6 LDM */
    wr8(ABS(0x03A7), 0x00);  /* 00BBDA LDM */
    wr8(ABS(0x03AF), 0x00);  /* 00BBDE LDM */
    wr8(ABS(0x03AD), 0x00);  /* 00BBE2 LDM */
    wr8(ABS(0x03A2), 0x00);  /* 00BBE6 LDM */
    mflag = 0;  /* 00BBEA CLM */
    wr16(DP(0x00), 0x00D7);  /* 00BBEB LDM */
    wr16(DP(0x02), 0x00C4);  /* 00BBEF LDM */
    wr16(DP(0x04), 0x0044);  /* 00BBF3 LDM */
    Y = 0x0003;  /* 00BBF7 LDY */
    A = D;  /* 00BBFA TDA */
    C = 0;  /* 00BBFB CLC */
    A = adc16(A, 0x0006);  /* 00BBFC ADC */
    push16(A);  /* 00BBFF PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00BC00 JSRL */
    Y = pull16();  /* 00BC04 PLY */
    mflag = 0;  /* 00BC05 CLM */
    wr16(DP(0x00), 0x003A);  /* 00BC06 LDM */
    wr16(DP(0x02), 0x0040);  /* 00BC0A LDM */
    Y = 0x0002;  /* 00BC0E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00BC11 JSRL */
    wr8(ABS(0x03A3), 0x01);  /* 00BC15 LDM */
    mflag = 0;  /* 00BC19 CLM */
    Y = pull16();  /* 00BC1A PLY */
    D = pull16();  /* 00BC1B PLD */
    S += 3; return;  /* 00BC1C RTL */
}

/* 00BC1D  callers: 00C937 */
void F_00BC1D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BC1D LDA */
    push16(A);  /* 00BC1F PHA */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0) goto L_00BC4B;  /* 00BC20 BBC */
    mflag = 1;  /* 00BC26 SEM */
    wr8(ABS(0x03BA), 0x03);  /* 00BC27 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00BC35;  /* 00BC2B BBS */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_00BC3D;  /* 00BC30 BBC */
L_00BC35: TICK();
    Y = 0x000F;  /* 00BC35 LDY */
    mflag = 0;  /* 00BC38 CLM */
    S -= 3; F_007D7F();  /* 00BC39 JSRL */
L_00BC3D: TICK();
    mflag = 1;  /* 00BC3D SEM */
    wr8(ABS(0x022A), 0x00);  /* 00BC3E LDM */
    mflag = 0;  /* 00BC42 CLM */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00BC43 CLB */
    goto L_00BC59;  /* 00BC48 JMP */
L_00BC4B: TICK();
    mflag = 1;  /* 00BC4B SEM */
    SETLO(A, rd8(ABS(0x041D)));  /* 00BC4C LDA */
    if ((A & 0xFF) != 0) goto L_00BC59;  /* 00BC4F BNE */
    wr8(ABS(0x03BA), 0x02);  /* 00BC51 LDM */
    wr8(ABS(0x0229), 0x01);  /* 00BC55 LDM */
L_00BC59: TICK();
    mflag = 0;  /* 00BC59 CLM */
    Y = pull16();  /* 00BC5A PLY */
    S += 3; return;  /* 00BC5B RTL */
}

/* 00BC5C  callers: 00CAA6 */
void F_00BC5C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BC5C LDA */
    push16(A);  /* 00BC5E PHA */
    mflag = 1;  /* 00BC5F SEM */
    SETLO(A, rd8(ABS(0x01DE)));  /* 00BC60 LDA */
    if ((A & 0xFF) == 0) goto L_00BC84;  /* 00BC63 BEQ */
    SETLO(A, rd8(ABS(0x0414)));  /* 00BC65 LDA */
    if ((A & 0xFF) != 0) goto L_00BC72;  /* 00BC68 BNE */
    mflag = 0;  /* 00BC6A CLM */
    S -= 3; F_00A4E9();  /* 00BC6B JSRL */
    goto L_00BC84;  /* 00BC6F JMP */
L_00BC72: TICK();
    mflag = 0;  /* 00BC72 CLM */
    wr16(ABS(0x03B6), 0x000B);  /* 00BC73 LDM */
    S -= 3; F_00A936();  /* 00BC78 JSRL */
    mflag = 1;  /* 00BC7C SEM */
    wr8(ABS(0x0229), 0x03);  /* 00BC7D LDM */
    mflag = 0;  /* 00BC81 CLM */
    Y = pull16();  /* 00BC82 PLY */
    S += 3; return;  /* 00BC83 RTL */
L_00BC84: TICK();
    mflag = 1;  /* 00BC84 SEM */
    SETLO(A, rd8(ABS(0x0423)));  /* 00BC85 LDA */
    if ((A & 0xFF) == 0) goto L_00BC96;  /* 00BC88 BEQ */
    SETLO(A, rd8(ABS(0x03B1)));  /* 00BC8A LDA */
    if ((A & 0xFF) == 0) goto L_00BC96;  /* 00BC8D BEQ */
    mflag = 0;  /* 00BC8F CLM */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0x0001) goto L_00BCAC;  /* 00BC90 BBS */
L_00BC96: TICK();
    mflag = 0;  /* 00BC96 CLM */
    wr16(ABS(0x03B6), 0x0008);  /* 00BC97 LDM */
    S -= 3; F_00A83C();  /* 00BC9C JSRL */
    mflag = 1;  /* 00BCA0 SEM */
    wr8(ABS(0x03CE), 0x20);  /* 00BCA1 LDM */
    wr8(ABS(0x0229), 0x03);  /* 00BCA5 LDM */
    mflag = 0;  /* 00BCA9 CLM */
    Y = pull16();  /* 00BCAA PLY */
    S += 3; return;  /* 00BCAB RTL */
L_00BCAC: TICK();
    if ((rd16(ABS(0x0443)) & 0x0008) == 0) goto L_00BCBC;  /* 00BCAC BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00BCB2 CLB */
    mflag = 1;  /* 00BCB7 SEM */
    wr8(ABS(0x0229), 0x04);  /* 00BCB8 LDM */
L_00BCBC: TICK();
    mflag = 0;  /* 00BCBC CLM */
    Y = pull16();  /* 00BCBD PLY */
    S += 3; return;  /* 00BCBE RTL */
}

/* 00BCBF  callers: 00CAD6 */
void F_00BCBF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BCBF LDA */
    push16(A);  /* 00BCC1 PHA */
    mflag = 1;  /* 00BCC2 SEM */
    SETLO(A, rd8(ABS(0x01DE)));  /* 00BCC3 LDA */
    if ((A & 0xFF) == 0) goto L_00BCEC;  /* 00BCC6 BEQ */
    SETLO(A, rd8(ABS(0x0414)));  /* 00BCC8 LDA */
    if ((A & 0xFF) != 0) goto L_00BCD5;  /* 00BCCB BNE */
    mflag = 0;  /* 00BCCD CLM */
    S -= 3; F_00A4E9();  /* 00BCCE JSRL */
    goto L_00BCEC;  /* 00BCD2 JMP */
L_00BCD5: TICK();
    mflag = 1;  /* 00BCD5 SEM */
    wr8(ABS(0x01DE), 0x00);  /* 00BCD6 LDM */
    mflag = 0;  /* 00BCDA CLM */
    wr16(ABS(0x03B6), 0x000B);  /* 00BCDB LDM */
    S -= 3; F_00A936();  /* 00BCE0 JSRL */
    mflag = 1;  /* 00BCE4 SEM */
    wr8(ABS(0x0229), 0x03);  /* 00BCE5 LDM */
    mflag = 0;  /* 00BCE9 CLM */
    Y = pull16();  /* 00BCEA PLY */
    S += 3; return;  /* 00BCEB RTL */
L_00BCEC: TICK();
    mflag = 1;  /* 00BCEC SEM */
    SETLO(A, rd8(ABS(0x03B1)));  /* 00BCED LDA */
    if ((A & 0xFF) == 0) goto L_00BD0D;  /* 00BCF0 BEQ */
    SETLO(A, rd8(ABS(0x03AB)));  /* 00BCF2 LDA */
    if ((A & 0xFF) == 0) goto L_00BD0D;  /* 00BCF5 BEQ */
    mflag = 0;  /* 00BCF7 CLM */
    wr16(ABS(0x03B6), 0x0006);  /* 00BCF8 LDM */
    S -= 3; F_00A83C();  /* 00BCFD JSRL */
    mflag = 1;  /* 00BD01 SEM */
    wr8(ABS(0x03CE), 0x00);  /* 00BD02 LDM */
    wr8(ABS(0x0229), 0x03);  /* 00BD06 LDM */
    mflag = 0;  /* 00BD0A CLM */
    Y = pull16();  /* 00BD0B PLY */
    S += 3; return;  /* 00BD0C RTL */
L_00BD0D: TICK();
    mflag = 0;  /* 00BD0D CLM */
    if ((rd16(ABS(0x0443)) & 0x0008) == 0) goto L_00BD34;  /* 00BD0E BBC */
    mflag = 1;  /* 00BD14 SEM */
    SETLO(A, rd8(ABS(0x0415)));  /* 00BD15 LDA */
    if ((A & 0xFF) == 0) goto L_00BD24;  /* 00BD18 BEQ */
    SETLO(A, rd8(ABS(0x03A8)));  /* 00BD1A LDA */
    if ((A & 0xFF) == 0) goto L_00BD24;  /* 00BD1D BEQ */
    mflag = 0;  /* 00BD1F CLM */
    S -= 3; F_00A559();  /* 00BD20 JSRL */
L_00BD24: TICK();
    mflag = 0;  /* 00BD24 CLM */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00BD25 CLB */
    wr16(ABS(0x03B6), 0x0009);  /* 00BD2A LDM */
    mflag = 1;  /* 00BD2F SEM */
    wr8(ABS(0x0229), 0x02);  /* 00BD30 LDM */
L_00BD34: TICK();
    mflag = 0;  /* 00BD34 CLM */
    Y = pull16();  /* 00BD35 PLY */
    S += 3; return;  /* 00BD36 RTL */
}

/* 00BD37  callers: 00CAB6 */
void F_00BD37(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BD37 LDA */
    push16(A);  /* 00BD39 PHA */
    mflag = 1;  /* 00BD3A SEM */
    SETLO(A, rd8(ABS(0x0423)));  /* 00BD3B LDA */
    if ((A & 0xFF) != 0) goto L_00BD5F;  /* 00BD3E BNE */
    SETLO(A, rd8(ABS(0x0414)));  /* 00BD40 LDA */
    if ((A & 0xFF) != 0) goto L_00BD4D;  /* 00BD43 BNE */
    mflag = 0;  /* 00BD45 CLM */
    S -= 3; F_00A4E9();  /* 00BD46 JSRL */
    goto L_00BD5F;  /* 00BD4A JMP */
L_00BD4D: TICK();
    mflag = 0;  /* 00BD4D CLM */
    wr16(ABS(0x03B6), 0x000B);  /* 00BD4E LDM */
    S -= 3; F_00A936();  /* 00BD53 JSRL */
    mflag = 1;  /* 00BD57 SEM */
    wr8(ABS(0x0229), 0x03);  /* 00BD58 LDM */
    mflag = 0;  /* 00BD5C CLM */
    Y = pull16();  /* 00BD5D PLY */
    S += 3; return;  /* 00BD5E RTL */
L_00BD5F: TICK();
    mflag = 0;  /* 00BD5F CLM */
    if ((rd16(ABS(0x0443)) & 0x0008) == 0) goto L_00BD7B;  /* 00BD60 BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00BD66 CLB */
    wr16(ABS(0x03B6), 0x0009);  /* 00BD6B LDM */
    mflag = 1;  /* 00BD70 SEM */
    wr8(ABS(0x0229), 0x02);  /* 00BD71 LDM */
    SETLO(A, rd8(ABS(0x041A)));  /* 00BD75 LDA */
    wr8(ABS(0x0421), (uint8_t)A);  /* 00BD78 STA */
L_00BD7B: TICK();
    mflag = 0;  /* 00BD7B CLM */
    Y = pull16();  /* 00BD7C PLY */
    S += 3; return;  /* 00BD7D RTL */
}

/* 00BD7E  callers: 00BE30, 00BE4F */
void F_00BD7E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BD7E LDA */
    push16(A);  /* 00BD80 PHA */
    S -= 3; F_00A552();  /* 00BD81 JSRL */
    S -= 3; F_00B8EC();  /* 00BD85 JSRL */
    mflag = 1;  /* 00BD89 SEM */
    wr8(ABS(0x01E3), 0x01);  /* 00BD8A LDM */
    mflag = 0;  /* 00BD8E CLM */
    S -= 3; F_00A948();  /* 00BD8F JSRL */
    mflag = 1;  /* 00BD93 SEM */
    wr8(ABS(0x03AE), 0x00);  /* 00BD94 LDM */
    mflag = 0;  /* 00BD98 CLM */
    Y = pull16();  /* 00BD99 PLY */
    S += 3; return;  /* 00BD9A RTL */
}

/* 00BD9B  callers: 00CAC6 */
void F_00BD9B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BD9B LDA */
    push16(A);  /* 00BD9D PHA */
    mflag = 1;  /* 00BD9E SEM */
    SETLO(A, rd8(ABS(0x0421)));  /* 00BD9F LDA */
    if ((A & 0xFF) != 0) goto L_00BDAB;  /* 00BDA2 BNE */
    mflag = 0;  /* 00BDA4 CLM */
    S -= 3; F_00A4E9();  /* 00BDA5 JSRL */
    Y = pull16();  /* 00BDA9 PLY */
    S += 3; return;  /* 00BDAA RTL */
L_00BDAB: TICK();
    SETLO(A, rd8(ABS(0x01DE)));  /* 00BDAB LDA */
    if ((A & 0xFF) == 0) goto L_00BDC4;  /* 00BDAE BEQ */
    SETLO(A, rd8(ABS(0x0414)));  /* 00BDB0 LDA */
    if ((A & 0xFF) != 0) goto L_00BDBD;  /* 00BDB3 BNE */
    mflag = 0;  /* 00BDB5 CLM */
    S -= 3; F_00A4E9();  /* 00BDB6 JSRL */
    goto L_00BDC4;  /* 00BDBA JMP */
L_00BDBD: TICK();
    mflag = 0;  /* 00BDBD CLM */
    S -= 3; F_00B8EC();  /* 00BDBE JSRL */
    Y = pull16();  /* 00BDC2 PLY */
    S += 3; return;  /* 00BDC3 RTL */
L_00BDC4: TICK();
    mflag = 1;  /* 00BDC4 SEM */
    wr8(ABS(0x01DE), 0x00);  /* 00BDC5 LDM */
    mflag = 0;  /* 00BDC9 CLM */
    if ((rd16(ABS(0x0443)) & 0x0020) == 0) goto L_00BDD9;  /* 00BDCA BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0020);  /* 00BDD0 CLB */
    S -= 3; F_00A559();  /* 00BDD5 JSRL */
L_00BDD9: TICK();
    mflag = 1;  /* 00BDD9 SEM */
    SETLO(A, rd8(ABS(0x0420)));  /* 00BDDA LDA */
    if ((A & 0xFF) != 0) goto L_00BE0F;  /* 00BDDD BNE */
    SETLO(A, rd8(ABS(0x03AD)));  /* 00BDDF LDA */
    if ((A & 0xFF) != 0) goto L_00BE0F;  /* 00BDE2 BNE */
    wr8(ABS(0x0415), (uint8_t)A);  /* 00BDE4 STA */
    wr8(ABS(0x03B8), (uint8_t)A);  /* 00BDE7 STA */
    SETLO(A, rd8(ABS(0x03B1)));  /* 00BDEA LDA */
    if ((A & 0xFF) == 0) goto L_00BE0F;  /* 00BDED BEQ */
    SETLO(A, rd8(ABS(0x041C)));  /* 00BDEF LDA */
    wr8(ABS(0x0423), (uint8_t)A);  /* 00BDF2 STA */
    SETLO(A, rd8(ABS(0x03AB)));  /* 00BDF5 LDA */
    if ((A & 0xFF) == 0) goto L_00BE01;  /* 00BDF8 BEQ */
    mflag = 0;  /* 00BDFA CLM */
    S -= 3; F_00B8EC();  /* 00BDFB JSRL */
    Y = pull16();  /* 00BDFF PLY */
    S += 3; return;  /* 00BE00 RTL */
L_00BE01: TICK();
    mflag = 0;  /* 00BE01 CLM */
    wr16(ABS(0x03B6), 0x0006);  /* 00BE02 LDM */
    mflag = 1;  /* 00BE07 SEM */
    wr8(ABS(0x0229), 0x04);  /* 00BE08 LDM */
    mflag = 0;  /* 00BE0C CLM */
    Y = pull16();  /* 00BE0D PLY */
    S += 3; return;  /* 00BE0E RTL */
L_00BE0F: TICK();
    SETLO(A, rd8(ABS(0x01BD)));  /* 00BE0F LDA */
    if ((A & 0xFF) == 0) goto L_00BE37;  /* 00BE12 BEQ */
    SETLO(A, rd8(ABS(0x03AD)));  /* 00BE14 LDA */
    if ((A & 0xFF) != 0) goto L_00BE37;  /* 00BE17 BNE */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00BE19 LDA */
    wr8(ABS(0x03B8), (uint8_t)A);  /* 00BE1C STA */
    wr8(ABS(0x01BD), 0x00);  /* 00BE1F LDM */
    wr8(ABS(0x01BE), 0x10);  /* 00BE23 LDM */
    wr8(ABS(0x03A4), 0x01);  /* 00BE27 LDM */
    wr8(ABS(0x0415), 0x03);  /* 00BE2B LDM */
    mflag = 0;  /* 00BE2F CLM */
    S -= 3; F_00BD7E();  /* 00BE30 JSRL */
    goto L_00BE57;  /* 00BE34 JMP */
L_00BE37: TICK();
    mflag = 1;  /* 00BE37 SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00BE38 LDA */
    if ((A & 0xFF) != 0) goto L_00BE43;  /* 00BE3B BNE */
    wr8(ABS(0x0420), (uint8_t)A);  /* 00BE3D STA */
    goto L_00BE57;  /* 00BE40 JMP */
L_00BE43: TICK();
    mflag = 1;  /* 00BE43 SEM */
    SETLO(A, rd8(ABS(0x0415)));  /* 00BE44 LDA */
    if ((A & 0xFF) == 0) goto L_00BE57;  /* 00BE47 BEQ */
    SETLO(A, rd8(ABS(0x0424)));  /* 00BE49 LDA */
    if ((A & 0xFF) != 0) goto L_00BE57;  /* 00BE4C BNE */
    mflag = 0;  /* 00BE4E CLM */
    S -= 3; F_00BD7E();  /* 00BE4F JSRL */
    mflag = 1;  /* 00BE53 SEM */
    t = (rd8(ABS(0x0415)) - 1) & 0xFF; wr8(ABS(0x0415), t);  /* 00BE54 DEC */
L_00BE57: TICK();
    mflag = 1;  /* 00BE57 SEM */
    SETLO(A, rd8(ABS(0x03A9)));  /* 00BE58 LDA */
    if ((A & 0xFF) != 0) goto L_00BE60;  /* 00BE5B BNE */
    goto L_00BF0E;  /* 00BE5D JMP */
L_00BE60: TICK();
    mflag = 0;  /* 00BE60 CLM */
    S -= 3; F_00AD96();  /* 00BE61 JSRL */
    S -= 3; F_00B801();  /* 00BE65 JSRL */
    /* 00BE69 CMP */
    if ((A & 0xFF) == 0x00) goto L_00BE72;  /* 00BE6B BEQ */
    mflag = 0;  /* 00BE6D CLM */
    S -= 3; F_00A552();  /* 00BE6E JSRL */
L_00BE72: TICK();
    mflag = 0;  /* 00BE72 CLM */
    A = rd16(ABS(0x03B9));  /* 00BE73 LDA */
    mflag = 1;  /* 00BE76 SEM */
    /* 00BE77 CMP */
    if ((A & 0xFF) != 0x16) goto L_00BE8E;  /* 00BE79 BNE */
    mflag = 0;  /* 00BE7B CLM */
    S -= 3; F_00AC52();  /* 00BE7C JSRL */
    t = rd16(ABS(0x03F3));  /* 00BE80 CMP */
    if (A != t) goto L_00BE8E;  /* 00BE83 BNE */
    S -= 3; F_00B8BA();  /* 00BE85 JSRL */
    mflag = 1;  /* 00BE89 SEM */
    wr8(ABS(0x03A8), 0x00);  /* 00BE8A LDM */
L_00BE8E: TICK();
    mflag = 0;  /* 00BE8E CLM */
    A = rd16(ABS(0x03B9));  /* 00BE8F LDA */
    mflag = 1;  /* 00BE92 SEM */
    /* 00BE93 CMP */
    if ((A & 0xFF) == 0x01) goto L_00BE9C;  /* 00BE95 BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00BE97 LDA */
    if ((A & 0xFF) != 0) goto L_00BEB0;  /* 00BE9A BNE */
L_00BE9C: TICK();
    mflag = 0;  /* 00BE9C CLM */
    S -= 3; F_00A889();  /* 00BE9D JSRL */
    S -= 3; F_00B8EC();  /* 00BEA1 JSRL */
    mflag = 1;  /* 00BEA5 SEM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00BEA6 LDA */
    if ((A & 0xFF) != 0) goto L_00BEB0;  /* 00BEA9 BNE */
    mflag = 0;  /* 00BEAB CLM */
    S -= 3; F_00A552();  /* 00BEAC JSRL */
L_00BEB0: TICK();
    mflag = 0;  /* 00BEB0 CLM */
    A = rd16(ABS(0x03B9));  /* 00BEB1 LDA */
    mflag = 1;  /* 00BEB4 SEM */
    /* 00BEB5 CMP */
    if ((A & 0xFF) == 0x82) goto L_00BEDC;  /* 00BEB7 BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00BEB9 LDA */
    /* 00BEBC CMP */
    if ((A & 0xFF) == 0x83) goto L_00BEDC;  /* 00BEBE BEQ */
    /* 00BEC0 CMP */
    if ((A & 0xFF) == 0x84) goto L_00BEDC;  /* 00BEC2 BEQ */
    /* 00BEC4 CMP */
    if ((A & 0xFF) == 0x85) goto L_00BEDC;  /* 00BEC6 BEQ */
    /* 00BEC8 CMP */
    if ((A & 0xFF) == 0x86) goto L_00BEDC;  /* 00BECA BEQ */
    /* 00BECC CMP */
    if ((A & 0xFF) == 0x87) goto L_00BEDC;  /* 00BECE BEQ */
    /* 00BED0 CMP */
    if ((A & 0xFF) == 0x88) goto L_00BEDC;  /* 00BED2 BEQ */
    /* 00BED4 CMP */
    if ((A & 0xFF) == 0x89) goto L_00BEDC;  /* 00BED6 BEQ */
    /* 00BED8 CMP */
    if ((A & 0xFF) != 0x8B) goto L_00BEE4;  /* 00BEDA BNE */
L_00BEDC: TICK();
    wr8(ABS(0x0415), 0x00);  /* 00BEDC LDM */
    wr8(ABS(0x03B8), 0x00);  /* 00BEE0 LDM */
L_00BEE4: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00BEE4 LDA */
    /* 00BEE7 CMP */
    if ((A & 0xFF) == 0x40) goto L_00BEF3;  /* 00BEE9 BEQ */
    /* 00BEEB CMP */
    if ((A & 0xFF) == 0x41) goto L_00BEF3;  /* 00BEED BEQ */
    /* 00BEEF CMP */
    if ((A & 0xFF) != 0x42) goto L_00BF0E;  /* 00BEF1 BNE */
L_00BEF3: TICK();
    SETLO(A, rd8(ABS(0x03A5)));  /* 00BEF3 LDA */
    if ((A & 0xFF) == 0) goto L_00BEFF;  /* 00BEF6 BEQ */
    wr8(ABS(0x03B9), 0x10);  /* 00BEF8 LDM */
    goto L_00BF0E;  /* 00BEFC JMP */
L_00BEFF: TICK();
    mflag = 0;  /* 00BEFF CLM */
    S -= 3; F_00AB35();  /* 00BF00 JSRL */
    wr16(ABS(0x03F3), A);  /* 00BF04 STA */
    wr16(ABS(0x03F5), A);  /* 00BF07 STA */
    S -= 3; F_00A01C();  /* 00BF0A JSRL */
L_00BF0E: TICK();
    mflag = 0;  /* 00BF0E CLM */
    Y = pull16();  /* 00BF0F PLY */
    S += 3; return;  /* 00BF10 RTL */
}

/* 00BF11  callers: 00CAE6 */
void F_00BF11(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BF11 LDA */
    push16(A);  /* 00BF13 PHA */
    A = rd16(ABS(0x0228));  /* 00BF14 LDA */
    mflag = 1;  /* 00BF17 SEM */
    /* 00BF18 CMP */
    if ((A & 0xFF) != 0x08) goto L_00BF48;  /* 00BF1A BNE */
    mflag = 0;  /* 00BF1C CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0) goto L_00BF48;  /* 00BF1D BBC */
    wr16(ABS(0x03B6), 0x0007);  /* 00BF23 LDM */
    A = rd16(ABS(0x03B9));  /* 00BF28 LDA */
    mflag = 1;  /* 00BF2B SEM */
    /* 00BF2C CMP */
    if ((A & 0xFF) == 0x01) goto L_00BF35;  /* 00BF2E BEQ */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00BF30 LDA */
    if ((A & 0xFF) == 0) goto L_00BF45;  /* 00BF33 BEQ */
L_00BF35: TICK();
    wr8(ABS(0x03B9), 0x11);  /* 00BF35 LDM */
    Y = 0x000F;  /* 00BF39 LDY */
    mflag = 0;  /* 00BF3C CLM */
    S -= 3; F_007D7F();  /* 00BF3D JSRL */
    S -= 3; F_00A57F();  /* 00BF41 JSRL */
L_00BF45: TICK();
    mflag = 0;  /* 00BF45 CLM */
    Y = pull16();  /* 00BF46 PLY */
    S += 3; return;  /* 00BF47 RTL */
L_00BF48: TICK();
    mflag = 1;  /* 00BF48 SEM */
    SETLO(A, rd8(ABS(0x041D)));  /* 00BF49 LDA */
    if ((A & 0xFF) != 0) goto L_00BF53;  /* 00BF4C BNE */
    mflag = 0;  /* 00BF4E CLM */
    S -= 3; F_00A4E9();  /* 00BF4F JSRL */
L_00BF53: TICK();
    mflag = 0;  /* 00BF53 CLM */
    Y = pull16();  /* 00BF54 PLY */
    S += 3; return;  /* 00BF55 RTL */
}

/* 00BF56  callers: 00CAF6 */
void F_00BF56(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BF56 LDA */
    push16(A);  /* 00BF58 PHA */
    if ((rd16(ABS(0x0443)) & 0x0008) == 0) goto L_00BF77;  /* 00BF59 BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00BF5F CLB */
    mflag = 1;  /* 00BF64 SEM */
    SETLO(A, rd8(ABS(0x0414)));  /* 00BF65 LDA */
    if ((A & 0xFF) != 0) goto L_00BF72;  /* 00BF68 BNE */
    mflag = 0;  /* 00BF6A CLM */
    S -= 3; F_00A4E9();  /* 00BF6B JSRL */
    goto L_00BF77;  /* 00BF6F JMP */
L_00BF72: TICK();
    mflag = 0;  /* 00BF72 CLM */
    S -= 3; F_00A936();  /* 00BF73 JSRL */
L_00BF77: TICK();
    Y = pull16();  /* 00BF77 PLY */
    S += 3; return;  /* 00BF78 RTL */
}

/* 00BF79  callers: 00C991 */
void F_00BF79(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00BF79 LDA */
    push16(D);  /* 00BF7B PHD */
    D = A;  /* 00BF7C TAD */
    A += 1;  /* 00BF7D INC */
    push16(A);  /* 00BF7E PHA */
    S -= 3; F_00B7E9();  /* 00BF7F JSRL */
    /* 00BF83 CMP */
    if ((A & 0xFF) == 0x00) goto L_00BF93;  /* 00BF85 BEQ */
    wr8(ABS(0x01BD), 0x00);  /* 00BF87 LDM */
    wr8(ABS(0x03B8), 0x00);  /* 00BF8B LDM */
    wr8(ABS(0x01E7), 0x00);  /* 00BF8F LDM */
L_00BF93: TICK();
    SETLO(A, rd8(ABS(0x01E7)));  /* 00BF93 LDA */
    if ((A & 0xFF) == 0) goto L_00BFDC;  /* 00BF96 BEQ */
    SETLO(A, rd8(ABS(0x01DF)));  /* 00BF98 LDA */
    if ((A & 0xFF) != 0) goto L_00BFA9;  /* 00BF9B BNE */
    SETLO(A, rd8(ABS(0x01DE)));  /* 00BF9D LDA */
    if ((A & 0xFF) != 0) goto L_00BFA9;  /* 00BFA0 BNE */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00BFA2 LDA */
    /* 00BFA5 CMP */
    if ((A & 0xFF) != 0x48) goto L_00BFDC;  /* 00BFA7 BNE */
L_00BFA9: TICK();
    wr8(ABS(0x01E7), 0x00);  /* 00BFA9 LDM */
    wr8(ABS(0x01BE), 0x13);  /* 00BFAD LDM */
    SETLO(A, rd8(ABS(0x01DF)));  /* 00BFB1 LDA */
    if ((A & 0xFF) == 0) goto L_00BFBA;  /* 00BFB4 BEQ */
    wr8(ABS(0x0434), 0x00);  /* 00BFB6 LDM */
L_00BFBA: TICK();
    SETLO(A, rd8(ABS(0x01DE)));  /* 00BFBA LDA */
    if ((A & 0xFF) != 0) goto L_00BFC6;  /* 00BFBD BNE */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00BFBF LDA */
    /* 00BFC2 CMP */
    if ((A & 0xFF) != 0x48) goto L_00BFCA;  /* 00BFC4 BNE */
L_00BFC6: TICK();
    wr8(ABS(0x0434), 0x1F);  /* 00BFC6 LDM */
L_00BFCA: TICK();
    wr8(ABS(0x01BD), 0x00);  /* 00BFCA LDM */
    wr8(ABS(0x03B8), 0x58);  /* 00BFCE LDM */
    mflag = 0;  /* 00BFD2 CLM */
    wr16(ABS(0x01CF), 0x1FFE);  /* 00BFD3 LDM */
    S -= 3; F_00AAFE();  /* 00BFD8 JSRL */
L_00BFDC: TICK();
    mflag = 1;  /* 00BFDC SEM */
    wr8(ABS(0x01DF), 0x00);  /* 00BFDD LDM */
    wr8(ABS(0x01DE), 0x00);  /* 00BFE1 LDM */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00BFE5 LDA */
    if ((A & 0xFF) == 0) goto L_00BFFA;  /* 00BFE8 BEQ */
    wr8(ABS(0x03B8), (uint8_t)A);  /* 00BFEA STA */
    wr8(ABS(0x01BD), 0x00);  /* 00BFED LDM */
    wr8(ABS(0x01BE), 0x10);  /* 00BFF1 LDM */
    mflag = 0;  /* 00BFF5 CLM */
    S -= 3; F_00AAFE();  /* 00BFF6 JSRL */
L_00BFFA: TICK();
    mflag = 1;  /* 00BFFA SEM */
    SETLO(A, rd8(ABS(0x041E)));  /* 00BFFB LDA */
    if ((A & 0xFF) != 0) goto L_00C00A;  /* 00BFFE BNE */
    SETLO(A, rd8(ABS(0x03A3)));  /* 00C000 LDA */
    if ((A & 0xFF) != 0) goto L_00C00A;  /* 00C003 BNE */
    mflag = 0;  /* 00C005 CLM */
    S -= 3; F_00A3A8();  /* 00C006 JSRL */
L_00C00A: TICK();
    mflag = 0;  /* 00C00A CLM */
    A = rd16(ABS(0x03B9));  /* 00C00B LDA */
    mflag = 1;  /* 00C00E SEM */
    /* 00C00F CMP */
    if ((A & 0xFF) != 0x16) goto L_00C01B;  /* 00C011 BNE */
    mflag = 0;  /* 00C013 CLM */
    S -= 3; F_00B8BA();  /* 00C014 JSRL */
    goto L_00C09C;  /* 00C018 JMP */
L_00C01B: TICK();
    mflag = 0;  /* 00C01B CLM */
    A = rd16(ABS(0x03B9));  /* 00C01C LDA */
    mflag = 1;  /* 00C01F SEM */
    /* 00C020 CMP */
    if ((A & 0xFF) != 0x12) goto L_00C02C;  /* 00C022 BNE */
    mflag = 0;  /* 00C024 CLM */
    S -= 3; F_00B705();  /* 00C025 JSRL */
    goto L_00C09C;  /* 00C029 JMP */
L_00C02C: TICK();
    mflag = 0;  /* 00C02C CLM */
    A = rd16(ABS(0x03B9));  /* 00C02D LDA */
    mflag = 1;  /* 00C030 SEM */
    /* 00C031 CMP */
    if ((A & 0xFF) != 0x40) goto L_00C042;  /* 00C033 BNE */
    SETLO(A, rd8(ABS(0x03A3)));  /* 00C035 LDA */
    if ((A & 0xFF) == 0) goto L_00C042;  /* 00C038 BEQ */
    mflag = 0;  /* 00C03A CLM */
    S -= 3; F_00B5EB();  /* 00C03B JSRL */
    goto L_00C09C;  /* 00C03F JMP */
L_00C042: TICK();
    mflag = 0;  /* 00C042 CLM */
    A = rd16(ABS(0x03B9));  /* 00C043 LDA */
    mflag = 1;  /* 00C046 SEM */
    /* 00C047 CMP */
    if ((A & 0xFF) != 0x41) goto L_00C053;  /* 00C049 BNE */
    mflag = 0;  /* 00C04B CLM */
    S -= 3; F_00B563();  /* 00C04C JSRL */
    goto L_00C09C;  /* 00C050 JMP */
L_00C053: TICK();
    mflag = 0;  /* 00C053 CLM */
    A = rd16(ABS(0x03B9));  /* 00C054 LDA */
    mflag = 1;  /* 00C057 SEM */
    /* 00C058 CMP */
    if ((A & 0xFF) != 0x42) goto L_00C073;  /* 00C05A BNE */
    SETLO(A, rd8(ABS(0x03A3)));  /* 00C05C LDA */
    if ((A & 0xFF) != 0) goto L_00C06B;  /* 00C05F BNE */
    SETLO(A, rd8(ABS(0x03AD)));  /* 00C061 LDA */
    if ((A & 0xFF) == 0) goto L_00C073;  /* 00C064 BEQ */
    SETLO(A, rd8(ABS(0x03A2)));  /* 00C066 LDA */
    if ((A & 0xFF) != 0) goto L_00C073;  /* 00C069 BNE */
L_00C06B: TICK();
    mflag = 0;  /* 00C06B CLM */
    S -= 3; F_00B5FE();  /* 00C06C JSRL */
    goto L_00C09C;  /* 00C070 JMP */
L_00C073: TICK();
    mflag = 1;  /* 00C073 SEM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C074 LDA */
    if ((A & 0xFF) != 0) goto L_00C081;  /* 00C077 BNE */
    mflag = 0;  /* 00C079 CLM */
    S -= 3; F_00A8CF();  /* 00C07A JSRL */
    goto L_00C09C;  /* 00C07E JMP */
L_00C081: TICK();
    mflag = 0;  /* 00C081 CLM */
    S -= 3; F_00B832();  /* 00C082 JSRL */
    /* 00C086 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C09C;  /* 00C088 BNE */
    mflag = 0;  /* 00C08A CLM */
    S -= 3; F_00B8A1();  /* 00C08B JSRL */
    /* 00C08F CMP */
    if ((A & 0xFF) != 0x00) goto L_00C09C;  /* 00C091 BNE */
    mflag = 0;  /* 00C093 CLM */
    S -= 3; F_00B801();  /* 00C094 JSRL */
    /* 00C098 CMP */
    if ((A & 0xFF) == 0x00) goto L_00C09C;  /* 00C09A BEQ */
L_00C09C: TICK();
    mflag = 0;  /* 00C09C CLM */
    X = rd16(ABS(0x03B6));  /* 00C09D LDX */
    /* 00C0A0 CPX */
    if (X != 0x0004) goto L_00C105;  /* 00C0A3 BNE */
    Y = 0x0005;  /* 00C0A5 LDY */
    S -= 3; F_008DC6();  /* 00C0A8 JSRL */
    mflag = 1;  /* 00C0AC SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C0AD LDA [AN5 RSSI] */
    /* 00C0B0 CMP */
    if ((A & 0xFF) >= 0x20) goto L_00C0BA;  /* 00C0B2 BCS */
    wr8(DP(0x00), 0x32);  /* 00C0B4 LDM */
    goto L_00C0F6;  /* 00C0B7 JMP */
L_00C0BA: TICK();
    mflag = 1;  /* 00C0BA SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C0BB LDA [AN5 RSSI] */
    /* 00C0BE CMP */
    if ((A & 0xFF) >= 0x4E) goto L_00C0C8;  /* 00C0C0 BCS */
    wr8(DP(0x00), 0x33);  /* 00C0C2 LDM */
    goto L_00C0F6;  /* 00C0C5 JMP */
L_00C0C8: TICK();
    mflag = 1;  /* 00C0C8 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C0C9 LDA [AN5 RSSI] */
    /* 00C0CC CMP */
    if ((A & 0xFF) >= 0x69) goto L_00C0D6;  /* 00C0CE BCS */
    wr8(DP(0x00), 0x34);  /* 00C0D0 LDM */
    goto L_00C0F6;  /* 00C0D3 JMP */
L_00C0D6: TICK();
    mflag = 1;  /* 00C0D6 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C0D7 LDA [AN5 RSSI] */
    /* 00C0DA CMP */
    if ((A & 0xFF) >= 0x7B) goto L_00C0E4;  /* 00C0DC BCS */
    wr8(DP(0x00), 0x35);  /* 00C0DE LDM */
    goto L_00C0F6;  /* 00C0E1 JMP */
L_00C0E4: TICK();
    mflag = 1;  /* 00C0E4 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C0E5 LDA [AN5 RSSI] */
    /* 00C0E8 CMP */
    if ((A & 0xFF) >= 0x99) goto L_00C0F2;  /* 00C0EA BCS */
    wr8(DP(0x00), 0x36);  /* 00C0EC LDM */
    goto L_00C0F6;  /* 00C0EF JMP */
L_00C0F2: TICK();
    mflag = 1;  /* 00C0F2 SEM */
    wr8(DP(0x00), 0x37);  /* 00C0F3 LDM */
L_00C0F6: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00C0F6 LDA */
    t = rd8(ABS(0x0239));  /* 00C0F8 CMP */
    if ((A & 0xFF) == t) goto L_00C105;  /* 00C0FB BEQ */
    wr8(ABS(0x0239), (uint8_t)A);  /* 00C0FD STA */
    mflag = 0;  /* 00C100 CLM */
    S -= 3; F_00828C();  /* 00C101 JSRL */
L_00C105: TICK();
    mflag = 0;  /* 00C105 CLM */
    Y = pull16();  /* 00C106 PLY */
    D = pull16();  /* 00C107 PLD */
    S += 3; return;  /* 00C108 RTL */
}

/* 00C109  callers: 00C9A1 */
void F_00C109(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C109 LDA */
    push16(A);  /* 00C10B PHA */
    mflag = 1;  /* 00C10C SEM */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00C10D LDA */
    if ((A & 0xFF) != 0) goto L_00C117;  /* 00C110 BNE */
    SETLO(A, rd8(ABS(0x01DE)));  /* 00C112 LDA */
    if ((A & 0xFF) == 0) goto L_00C196;  /* 00C115 BEQ */
L_00C117: TICK();
    SETLO(A, rd8(ABS(0x03A4)));  /* 00C117 LDA */
    if ((A & 0xFF) == 0) goto L_00C13D;  /* 00C11A BEQ */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00C11C LDA */
    /* 00C11F CMP */
    if ((A & 0xFF) != 0x48) goto L_00C13D;  /* 00C121 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C123 LDA */
    /* 00C126 CMP */
    if ((A & 0xFF) == 0x48) goto L_00C13D;  /* 00C128 BEQ */
    /* 00C12A CMP */
    if ((A & 0xFF) != 0x58) goto L_00C135;  /* 00C12C BNE */
    SETLO(A, rd8(ABS(0x0434)));  /* 00C12E LDA */
    /* 00C131 CMP */
    if ((A & 0xFF) == 0x1F) goto L_00C13D;  /* 00C133 BEQ */
L_00C135: TICK();
    mflag = 0;  /* 00C135 CLM */
    S -= 3; F_00A3A8();  /* 00C136 JSRL */
    goto L_00C196;  /* 00C13A JMP */
L_00C13D: TICK();
    mflag = 0;  /* 00C13D CLM */
    A = rd16(ABS(0x01BD));  /* 00C13E LDA */
    mflag = 1;  /* 00C141 SEM */
    /* 00C142 CMP */
    if ((A & 0xFF) != 0x50) goto L_00C156;  /* 00C144 BNE */
    wr8(ABS(0x03B8), 0x50);  /* 00C146 LDM */
    wr8(ABS(0x01BE), 0x10);  /* 00C14A LDM */
    mflag = 0;  /* 00C14E CLM */
    S -= 3; F_00AAFE();  /* 00C14F JSRL */
    goto L_00C196;  /* 00C153 JMP */
L_00C156: TICK();
    mflag = 0;  /* 00C156 CLM */
    A = rd16(ABS(0x01BD));  /* 00C157 LDA */
    mflag = 1;  /* 00C15A SEM */
    /* 00C15B CMP */
    if ((A & 0xFF) == 0x48) goto L_00C164;  /* 00C15D BEQ */
    SETLO(A, rd8(ABS(0x01DE)));  /* 00C15F LDA */
    if ((A & 0xFF) == 0) goto L_00C196;  /* 00C162 BEQ */
L_00C164: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C164 LDA */
    /* 00C167 CMP */
    if ((A & 0xFF) == 0x48) goto L_00C196;  /* 00C169 BEQ */
    /* 00C16B CMP */
    if ((A & 0xFF) != 0x58) goto L_00C176;  /* 00C16D BNE */
    SETLO(A, rd8(ABS(0x0434)));  /* 00C16F LDA */
    /* 00C172 CMP */
    if ((A & 0xFF) == 0x1F) goto L_00C196;  /* 00C174 BEQ */
L_00C176: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C176 LDA */
    /* 00C179 CMP */
    if ((A & 0xFF) != 0x58) goto L_00C184;  /* 00C17B BNE */
    wr8(ABS(0x0434), 0x1F);  /* 00C17D LDM */
    goto L_00C18D;  /* 00C181 JMP */
L_00C184: TICK();
    mflag = 1;  /* 00C184 SEM */
    wr8(ABS(0x03B8), 0x48);  /* 00C185 LDM */
    wr8(ABS(0x01BE), 0x0A);  /* 00C189 LDM */
L_00C18D: TICK();
    wr8(ABS(0x01DE), 0x00);  /* 00C18D LDM */
    mflag = 0;  /* 00C191 CLM */
    S -= 3; F_00AAFE();  /* 00C192 JSRL */
L_00C196: TICK();
    mflag = 1;  /* 00C196 SEM */
    wr8(ABS(0x01BD), 0x00);  /* 00C197 LDM */
    mflag = 0;  /* 00C19B CLM */
    S -= 3; F_00B8FC();  /* 00C19C JSRL */
    /* 00C1A0 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C1BF;  /* 00C1A2 BNE */
    mflag = 0;  /* 00C1A4 CLM */
    S -= 3; F_00B801();  /* 00C1A5 JSRL */
    /* 00C1A9 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C1BF;  /* 00C1AB BNE */
    mflag = 0;  /* 00C1AD CLM */
    S -= 3; F_00B832();  /* 00C1AE JSRL */
    /* 00C1B2 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C1BF;  /* 00C1B4 BNE */
    mflag = 0;  /* 00C1B6 CLM */
    S -= 3; F_00B7E9();  /* 00C1B7 JSRL */
    /* 00C1BB CMP */
    if ((A & 0xFF) == 0x00) goto L_00C1C2;  /* 00C1BD BEQ */
L_00C1BF: TICK();
    mflag = 0;  /* 00C1BF CLM */
    Y = pull16();  /* 00C1C0 PLY */
    S += 3; return;  /* 00C1C1 RTL */
L_00C1C2: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C1C2 LDA */
    /* 00C1C5 CMP */
    if ((A & 0xFF) == 0x04) goto L_00C1D5;  /* 00C1C7 BEQ */
    /* 00C1C9 CMP */
    if ((A & 0xFF) == 0x8A) goto L_00C1D5;  /* 00C1CB BEQ */
    /* 00C1CD CMP */
    if ((A & 0xFF) == 0x83) goto L_00C1D5;  /* 00C1CF BEQ */
    /* 00C1D1 CMP */
    if ((A & 0xFF) != 0x84) goto L_00C1E9;  /* 00C1D3 BNE */
L_00C1D5: TICK();
    /* 00C1D5 CMP */
    if ((A & 0xFF) != 0x83) goto L_00C1DD;  /* 00C1D7 BNE */
    wr8(ABS(0x01BE), 0x11);  /* 00C1D9 LDM */
L_00C1DD: TICK();
    mflag = 0;  /* 00C1DD CLM */
    wr16(ABS(0x03B6), 0x0003);  /* 00C1DE LDM */
    S -= 3; F_00A51C();  /* 00C1E3 JSRL */
    Y = pull16();  /* 00C1E7 PLY */
    S += 3; return;  /* 00C1E8 RTL */
L_00C1E9: TICK();
    SETLO(A, rd8(ABS(0x0420)));  /* 00C1E9 LDA */
    if ((A & 0xFF) == 0) goto L_00C212;  /* 00C1EC BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C1EE LDA */
    /* 00C1F1 CMP */
    if ((A & 0xFF) != 0x81) goto L_00C1FC;  /* 00C1F3 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C1F5 LDA */
    /* 00C1F8 CMP */
    if ((A & 0xFF) == 0x48) goto L_00C212;  /* 00C1FA BEQ */
L_00C1FC: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C1FC LDA */
    /* 00C1FF CMP */
    if ((A & 0xFF) != 0x82) goto L_00C23F;  /* 00C201 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C203 LDA */
    /* 00C206 CMP */
    if ((A & 0xFF) == 0x58) goto L_00C212;  /* 00C208 BEQ */
    /* 00C20A CMP */
    if ((A & 0xFF) == 0x54) goto L_00C212;  /* 00C20C BEQ */
    /* 00C20E CMP */
    if ((A & 0xFF) != 0x5C) goto L_00C23F;  /* 00C210 BNE */
L_00C212: TICK();
    SETLO(A, rd8(ABS(0x0420)));  /* 00C212 LDA */
    if ((A & 0xFF) != 0) goto L_00C22B;  /* 00C215 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C217 LDA */
    /* 00C21A CMP */
    if ((A & 0xFF) != 0x54) goto L_00C22B;  /* 00C21C BNE */
    mflag = 0;  /* 00C21E CLM */
    S -= 3; F_00A399();  /* 00C21F JSRL */
    mflag = 1;  /* 00C223 SEM */
    wr8(ABS(0x01BE), 0x00);  /* 00C224 LDM */
    goto L_00C23C;  /* 00C228 JMP */
L_00C22B: TICK();
    Y = rd16(ABS(0x0426));  /* 00C22B LDY */
    mflag = 0;  /* 00C22E CLM */
    S -= 3; F_00A10E();  /* 00C22F JSRL */
    Y = A;  /* 00C233 TAY */
    S -= 3; F_00A19A();  /* 00C234 JSRL */
    S -= 3; F_00A3A8();  /* 00C238 JSRL */
L_00C23C: TICK();
    mflag = 0;  /* 00C23C CLM */
    Y = pull16();  /* 00C23D PLY */
    S += 3; return;  /* 00C23E RTL */
L_00C23F: TICK();
    SETLO(A, rd8(ABS(0x03A4)));  /* 00C23F LDA */
    if ((A & 0xFF) == 0) goto L_00C255;  /* 00C242 BEQ */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C244 LDA */
    /* 00C247 CMP */
    if ((A & 0xFF) != 0x50) goto L_00C250;  /* 00C249 BNE */
    SETLO(A, 0x0F);  /* 00C24B LDA */
    goto L_00C252;  /* 00C24D JMP */
L_00C250: TICK();
    SETLO(A, 0x07);  /* 00C250 LDA */
L_00C252: TICK();
    wr8(ABS(0x0415), (uint8_t)A);  /* 00C252 STA */
L_00C255: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C255 LDA */
    /* 00C258 CMP */
    if ((A & 0xFF) == 0x03) goto L_00C272;  /* 00C25A BEQ */
    /* 00C25C CMP */
    if ((A & 0xFF) == 0x13) goto L_00C272;  /* 00C25E BEQ */
    mflag = 0;  /* 00C260 CLM */
    S -= 3; F_00B633();  /* 00C261 JSRL */
    /* 00C265 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C272;  /* 00C267 BNE */
    mflag = 0;  /* 00C269 CLM */
    S -= 3; F_00B70F();  /* 00C26A JSRL */
    /* 00C26E CMP */
    if ((A & 0xFF) == 0x00) goto L_00C275;  /* 00C270 BEQ */
L_00C272: TICK();
    mflag = 0;  /* 00C272 CLM */
    Y = pull16();  /* 00C273 PLY */
    S += 3; return;  /* 00C274 RTL */
L_00C275: TICK();
    mflag = 0;  /* 00C275 CLM */
    S -= 3; F_00B946();  /* 00C276 JSRL */
    /* 00C27A CMP */
    if ((A & 0xFF) == 0x00) goto L_00C283;  /* 00C27C BEQ */
    mflag = 0;  /* 00C27E CLM */
    S -= 3; F_00A948();  /* 00C27F JSRL */
L_00C283: TICK();
    mflag = 1;  /* 00C283 SEM */
    SETLO(A, rd8(ABS(0x0415)));  /* 00C284 LDA */
    if ((A & 0xFF) != 0) goto L_00C28E;  /* 00C287 BNE */
    mflag = 0;  /* 00C289 CLM */
    S -= 3; F_00A3A8();  /* 00C28A JSRL */
L_00C28E: TICK();
    mflag = 0;  /* 00C28E CLM */
    Y = pull16();  /* 00C28F PLY */
    S += 3; return;  /* 00C290 RTL */
}

/* 00C291  callers: 00C9C1 */
void F_00C291(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C291 LDA */
    push16(A);  /* 00C293 PHA */
    mflag = 1;  /* 00C294 SEM */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00C295 LDA */
    if ((A & 0xFF) == 0) goto L_00C2B0;  /* 00C298 BEQ */
    wr8(ABS(0x03B8), (uint8_t)A);  /* 00C29A STA */
    /* 00C29D CMP */
    if ((A & 0xFF) != 0x48) goto L_00C2A6;  /* 00C29F BNE */
    SETLO(A, 0x0A);  /* 00C2A1 LDA */
    goto L_00C2A8;  /* 00C2A3 JMP */
L_00C2A6: TICK();
    SETLO(A, 0x10);  /* 00C2A6 LDA */
L_00C2A8: TICK();
    wr8(ABS(0x01BE), (uint8_t)A);  /* 00C2A8 STA */
    mflag = 0;  /* 00C2AB CLM */
    S -= 3; F_00AAFE();  /* 00C2AC JSRL */
L_00C2B0: TICK();
    mflag = 1;  /* 00C2B0 SEM */
    wr8(ABS(0x01BD), 0x00);  /* 00C2B1 LDM */
    mflag = 0;  /* 00C2B5 CLM */
    S -= 3; F_00B8FC();  /* 00C2B6 JSRL */
    /* 00C2BA CMP */
    if ((A & 0xFF) != 0x00) goto L_00C2EB;  /* 00C2BC BNE */
    mflag = 0;  /* 00C2BE CLM */
    S -= 3; F_00B832();  /* 00C2BF JSRL */
    /* 00C2C3 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C2EB;  /* 00C2C5 BNE */
    mflag = 0;  /* 00C2C7 CLM */
    S -= 3; F_00B7E9();  /* 00C2C8 JSRL */
    /* 00C2CC CMP */
    if ((A & 0xFF) != 0x00) goto L_00C2EB;  /* 00C2CE BNE */
    mflag = 0;  /* 00C2D0 CLM */
    S -= 3; F_00B801();  /* 00C2D1 JSRL */
    /* 00C2D5 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C2EB;  /* 00C2D7 BNE */
    mflag = 0;  /* 00C2D9 CLM */
    S -= 3; F_00B633();  /* 00C2DA JSRL */
    /* 00C2DE CMP */
    if ((A & 0xFF) != 0x00) goto L_00C2EB;  /* 00C2E0 BNE */
    mflag = 0;  /* 00C2E2 CLM */
    S -= 3; F_00B70F();  /* 00C2E3 JSRL */
    /* 00C2E7 CMP */
    if ((A & 0xFF) == 0x00) goto L_00C2EE;  /* 00C2E9 BEQ */
L_00C2EB: TICK();
    mflag = 0;  /* 00C2EB CLM */
    Y = pull16();  /* 00C2EC PLY */
    S += 3; return;  /* 00C2ED RTL */
L_00C2EE: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C2EE LDA */
    /* 00C2F1 CMP */
    if ((A & 0xFF) != 0x83) goto L_00C2F9;  /* 00C2F3 BNE */
    wr8(ABS(0x01BE), 0x11);  /* 00C2F5 LDM */
L_00C2F9: TICK();
    SETLO(A, rd8(ABS(0x0420)));  /* 00C2F9 LDA */
    if ((A & 0xFF) == 0) goto L_00C314;  /* 00C2FC BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C2FE LDA */
    /* 00C301 CMP */
    if ((A & 0xFF) != 0x82) goto L_00C33E;  /* 00C303 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C305 LDA */
    /* 00C308 CMP */
    if ((A & 0xFF) == 0x58) goto L_00C314;  /* 00C30A BEQ */
    /* 00C30C CMP */
    if ((A & 0xFF) == 0x54) goto L_00C314;  /* 00C30E BEQ */
    /* 00C310 CMP */
    if ((A & 0xFF) != 0x5C) goto L_00C33E;  /* 00C312 BNE */
L_00C314: TICK();
    SETLO(A, rd8(ABS(0x0420)));  /* 00C314 LDA */
    if ((A & 0xFF) != 0) goto L_00C32D;  /* 00C317 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C319 LDA */
    /* 00C31C CMP */
    if ((A & 0xFF) != 0x54) goto L_00C32D;  /* 00C31E BNE */
    mflag = 0;  /* 00C320 CLM */
    S -= 3; F_00A399();  /* 00C321 JSRL */
    mflag = 1;  /* 00C325 SEM */
    wr8(ABS(0x01BE), 0x00);  /* 00C326 LDM */
    goto L_00C33E;  /* 00C32A JMP */
L_00C32D: TICK();
    Y = rd16(ABS(0x0426));  /* 00C32D LDY */
    mflag = 0;  /* 00C330 CLM */
    S -= 3; F_00A10E();  /* 00C331 JSRL */
    Y = A;  /* 00C335 TAY */
    S -= 3; F_00A19A();  /* 00C336 JSRL */
    S -= 3; F_00A3A8();  /* 00C33A JSRL */
L_00C33E: TICK();
    mflag = 0;  /* 00C33E CLM */
    Y = pull16();  /* 00C33F PLY */
    S += 3; return;  /* 00C340 RTL */
}

/* 00C341  callers: 00C9D1 */
void F_00C341(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x03B9));  /* 00C341 LDA */
    mflag = 1;  /* 00C344 SEM */
    /* 00C345 CMP */
    if ((A & 0xFF) == 0x12) goto L_00C350;  /* 00C347 BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C349 LDA */
    /* 00C34C CMP */
    if ((A & 0xFF) != 0x14) goto L_00C355;  /* 00C34E BNE */
L_00C350: TICK();
    mflag = 0;  /* 00C350 CLM */
    S -= 3; F_00A889();  /* 00C351 JSRL */
L_00C355: TICK();
    mflag = 0;  /* 00C355 CLM */
    S += 3; return;  /* 00C356 RTL */
}

/* 00C357  callers: 00C9B1 */
void F_00C357(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C357 LDA */
    push16(A);  /* 00C359 PHA */
    A = rd16(ABS(0x03B9));  /* 00C35A LDA */
    mflag = 1;  /* 00C35D SEM */
    /* 00C35E CMP */
    if ((A & 0xFF) != 0x26) goto L_00C3AC;  /* 00C360 BNE */
    SETLO(A, rd8(ABS(0x03A5)));  /* 00C362 LDA */
    if ((A & 0xFF) == 0) goto L_00C380;  /* 00C365 BEQ */
    mflag = 0;  /* 00C367 CLM */
    X = rd16(ABS(0x03F1));  /* 00C368 LDX */
    t = rd16(ABS(0x03F9));  /* 00C36B CPX */
    if (X == t) goto L_00C378;  /* 00C36E BEQ */
    mflag = 1;  /* 00C370 SEM */
    wr8(ABS(0x03BA), 0x00);  /* 00C371 LDM */
    goto L_00C37D;  /* 00C375 JMP */
L_00C378: TICK();
    mflag = 0;  /* 00C378 CLM */
    S -= 3; F_00A3A8();  /* 00C379 JSRL */
L_00C37D: TICK();
    goto L_00C3A9;  /* 00C37D JMP */
L_00C380: TICK();
    mflag = 0;  /* 00C380 CLM */
    X = rd16(ABS(0x03F9));  /* 00C381 LDX */
    t = rd16(ABS(0x03F5));  /* 00C384 CPX */
    if (X != t) goto L_00C3A4;  /* 00C387 BNE */
    mflag = 1;  /* 00C389 SEM */
    SETLO(A, rd8(ABS(0x03A7)));  /* 00C38A LDA */
    if ((A & 0xFF) == 0) goto L_00C397;  /* 00C38D BEQ */
    mflag = 0;  /* 00C38F CLM */
    S -= 3; F_00A3A8();  /* 00C390 JSRL */
    goto L_00C3A1;  /* 00C394 JMP */
L_00C397: TICK();
    mflag = 0;  /* 00C397 CLM */
    S -= 3; F_00A399();  /* 00C398 JSRL */
    mflag = 1;  /* 00C39C SEM */
    wr8(ABS(0x03AF), 0x01);  /* 00C39D LDM */
L_00C3A1: TICK();
    goto L_00C3A9;  /* 00C3A1 JMP */
L_00C3A4: TICK();
    mflag = 0;  /* 00C3A4 CLM */
    S -= 3; F_00A399();  /* 00C3A5 JSRL */
L_00C3A9: TICK();
    goto L_00C405;  /* 00C3A9 JMP */
L_00C3AC: TICK();
    mflag = 0;  /* 00C3AC CLM */
    A = rd16(ABS(0x03B9));  /* 00C3AD LDA */
    mflag = 1;  /* 00C3B0 SEM */
    /* 00C3B1 CMP */
    if ((A & 0xFF) != 0x01) goto L_00C3BD;  /* 00C3B3 BNE */
    mflag = 0;  /* 00C3B5 CLM */
    S -= 3; F_00A889();  /* 00C3B6 JSRL */
    goto L_00C3D5;  /* 00C3BA JMP */
L_00C3BD: TICK();
    Y = rd16(ABS(0x0426));  /* 00C3BD LDY */
    mflag = 0;  /* 00C3C0 CLM */
    S -= 3; F_00A10E();  /* 00C3C1 JSRL */
    Y = A;  /* 00C3C5 TAY */
    S -= 3; F_00A1EE();  /* 00C3C6 JSRL */
    /* 00C3CA CMP */
    if ((A & 0xFF) == 0x00) goto L_00C3D5;  /* 00C3CC BEQ */
    mflag = 0;  /* 00C3CE CLM */
    S -= 3; F_00A399();  /* 00C3CF JSRL */
    Y = pull16();  /* 00C3D3 PLY */
    S += 3; return;  /* 00C3D4 RTL */
L_00C3D5: TICK();
    mflag = 1;  /* 00C3D5 SEM */
    SETLO(A, rd8(ABS(0x040D)));  /* 00C3D6 LDA */
    if ((A & 0xFF) != 0) goto L_00C405;  /* 00C3D9 BNE */
    Y = rd16(ABS(0x0426));  /* 00C3DB LDY */
    mflag = 0;  /* 00C3DE CLM */
    S -= 3; F_00A10E();  /* 00C3DF JSRL */
    Y = A;  /* 00C3E3 TAY */
    S -= 3; F_00A157();  /* 00C3E4 JSRL */
    /* 00C3E8 CMP */
    if ((A & 0xFF) == 0x00) goto L_00C3FB;  /* 00C3EA BEQ */
    SETLO(A, rd8(ABS(0x03B5)));  /* 00C3EC LDA */
    /* 00C3EF CMP */
    if ((A & 0xFF) == 0x10) goto L_00C3FB;  /* 00C3F1 BEQ */
    mflag = 0;  /* 00C3F3 CLM */
    S -= 3; F_00A3A8();  /* 00C3F4 JSRL */
    goto L_00C405;  /* 00C3F8 JMP */
L_00C3FB: TICK();
    mflag = 1;  /* 00C3FB SEM */
    wr8(ABS(0x03B8), 0x54);  /* 00C3FC LDM */
    mflag = 0;  /* 00C400 CLM */
    S -= 3; F_00AAFE();  /* 00C401 JSRL */
L_00C405: TICK();
    mflag = 0;  /* 00C405 CLM */
    Y = pull16();  /* 00C406 PLY */
    S += 3; return;  /* 00C407 RTL */
}

/* 00C408  callers: 00C593 */
void F_00C408(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C408 LDA */
    push16(D);  /* 00C40A PHD */
    D = A;  /* 00C40B TAD */
    A += 1;  /* 00C40C INC */
    A += 1;  /* 00C40D INC */
    A += 1;  /* 00C40E INC */
    push16(A);  /* 00C40F PHA */
    mflag = 1;  /* 00C410 SEM */
    wr8(DP(0x00), 0x00);  /* 00C411 LDM */
L_00C414: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00C414 LDA */
    /* 00C416 CMP */
    if ((A & 0xFF) < 0x10) goto L_00C41D;  /* 00C418 BCC */
    goto L_00C4D6;  /* 00C41A JMP */
L_00C41D: TICK();
    mflag = 0;  /* 00C41D CLM */
    A &= 0x00FF;  /* 00C41E AND */
    A = asl16(A);  /* 00C421 ASL */
    C = 0;  /* 00C422 CLC */
    A = adc16(A, 0x013F);  /* 00C423 ADC */
    Y = A;  /* 00C426 TAY */
    S -= 3; F_007271();  /* 00C427 JSRL */
    wr16(DP(0x01), A);  /* 00C42B STA */
    mflag = 1;  /* 00C42D SEM */
    SETLO(A, rd8(ABS(0x0427)));  /* 00C42E LDA */
    if ((A & 0xFF) == 0) goto L_00C456;  /* 00C431 BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 00C433 LDA */
    SETLO(A, lsr8(A));  /* 00C435 LSR */
    mflag = 0;  /* 00C436 CLM */
    A &= 0x0003;  /* 00C437 AND */
    A -= 1;  /* 00C43A DEC */
    if (A != 0) goto L_00C456;  /* 00C43B BNE */
    Y = rd16(ABS(0x0427));  /* 00C43D LDY */
    S -= 3; F_00A10E();  /* 00C440 JSRL */
    wr16(DP(0x03), A);  /* 00C444 STA */
    A = rd16(DP(0x01));  /* 00C446 LDA */
    A &= 0x01FF;  /* 00C448 AND */
    t = rd16(DP(0x03));  /* 00C44B CMP */
    if (A != t) goto L_00C456;  /* 00C44D BNE */
    A = 0x0001;  /* 00C44F LDA */
    mflag = 1;  /* 00C452 SEM */
    Y = pull16();  /* 00C453 PLY */
    D = pull16();  /* 00C454 PLD */
    S += 3; return;  /* 00C455 RTL */
L_00C456: TICK();
    mflag = 1;  /* 00C456 SEM */
    SETLO(A, rd8(ABS(0x0426)));  /* 00C457 LDA */
    if ((A & 0xFF) == 0) goto L_00C481;  /* 00C45A BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 00C45C LDA */
    SETLO(A, lsr8(A));  /* 00C45E LSR */
    mflag = 0;  /* 00C45F CLM */
    A &= 0x0003;  /* 00C460 AND */
    /* 00C463 CMP */
    if (A != 0x0002) goto L_00C481;  /* 00C466 BNE */
    Y = rd16(ABS(0x0426));  /* 00C468 LDY */
    S -= 3; F_00A10E();  /* 00C46B JSRL */
    wr16(DP(0x03), A);  /* 00C46F STA */
    A = rd16(DP(0x01));  /* 00C471 LDA */
    A &= 0x01FF;  /* 00C473 AND */
    t = rd16(DP(0x03));  /* 00C476 CMP */
    if (A != t) goto L_00C481;  /* 00C478 BNE */
    A = 0x0001;  /* 00C47A LDA */
    mflag = 1;  /* 00C47D SEM */
    Y = pull16();  /* 00C47E PLY */
    D = pull16();  /* 00C47F PLD */
    S += 3; return;  /* 00C480 RTL */
L_00C481: TICK();
    mflag = 0;  /* 00C481 CLM */
    A = rd16(DP(0x02));  /* 00C482 LDA */
    mflag = 1;  /* 00C484 SEM */
    SETLO(A, lsr8(A));  /* 00C485 LSR */
    mflag = 0;  /* 00C486 CLM */
    A &= 0x0003;  /* 00C487 AND */
    /* 00C48A CMP */
    if (A != 0x0003) goto L_00C4D0;  /* 00C48D BNE */
    A = rd16(ABS(0x03FE));  /* 00C48F LDA */
    A &= 0x00FF;  /* 00C492 AND */
    /* 00C495 CMP */
    if (A >= 0x0040) goto L_00C4B6;  /* 00C498 BCS */
    Y = 0x0004;  /* 00C49A LDY */
    S -= 3; F_00A10E();  /* 00C49D JSRL */
    wr16(DP(0x03), A);  /* 00C4A1 STA */
    A = rd16(DP(0x01));  /* 00C4A3 LDA */
    A &= 0x01FF;  /* 00C4A5 AND */
    t = rd16(DP(0x03));  /* 00C4A8 CMP */
    if (A != t) goto L_00C4B3;  /* 00C4AA BNE */
    A = 0x0001;  /* 00C4AC LDA */
    mflag = 1;  /* 00C4AF SEM */
    Y = pull16();  /* 00C4B0 PLY */
    D = pull16();  /* 00C4B1 PLD */
    S += 3; return;  /* 00C4B2 RTL */
L_00C4B3: TICK();
    goto L_00C4D0;  /* 00C4B3 JMP */
L_00C4B6: TICK();
    Y = 0x0009;  /* 00C4B6 LDY */
    mflag = 0;  /* 00C4B9 CLM */
    S -= 3; F_00A10E();  /* 00C4BA JSRL */
    wr16(DP(0x03), A);  /* 00C4BE STA */
    A = rd16(DP(0x01));  /* 00C4C0 LDA */
    A &= 0x01FF;  /* 00C4C2 AND */
    t = rd16(DP(0x03));  /* 00C4C5 CMP */
    if (A != t) goto L_00C4D0;  /* 00C4C7 BNE */
    A = 0x0001;  /* 00C4C9 LDA */
    mflag = 1;  /* 00C4CC SEM */
    Y = pull16();  /* 00C4CD PLY */
    D = pull16();  /* 00C4CE PLD */
    S += 3; return;  /* 00C4CF RTL */
L_00C4D0: TICK();
    mflag = 1;  /* 00C4D0 SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00C4D1 INC */
    goto L_00C414;  /* 00C4D3 JMP */
L_00C4D6: TICK();
    mflag = 1;  /* 00C4D6 SEM */
    wr8(DP(0x00), 0x00);  /* 00C4D7 LDM */
L_00C4DA: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00C4DA LDA */
    /* 00C4DC CMP */
    if ((A & 0xFF) >= 0x10) goto L_00C4FC;  /* 00C4DE BCS */
    mflag = 0;  /* 00C4E0 CLM */
    A &= 0x00FF;  /* 00C4E1 AND */
    A = asl16(A);  /* 00C4E4 ASL */
    C = 0;  /* 00C4E5 CLC */
    A = adc16(A, 0x013F);  /* 00C4E6 ADC */
    Y = A;  /* 00C4E9 TAY */
    S -= 3; eeprom_read_byte_007189();  /* 00C4EA JSRL */
    SETLO(A, A & 0xC0);  /* 00C4EE AND */
    if ((A & 0xFF) == 0) goto L_00C4F7;  /* 00C4F0 BEQ */
    SETLO(A, 0x00);  /* 00C4F2 LDA */
    Y = pull16();  /* 00C4F4 PLY */
    D = pull16();  /* 00C4F5 PLD */
    S += 3; return;  /* 00C4F6 RTL */
L_00C4F7: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00C4F7 INC */
    goto L_00C4DA;  /* 00C4F9 JMP */
L_00C4FC: TICK();
    mflag = 0;  /* 00C4FC CLM */
    A = 0x0001;  /* 00C4FD LDA */
    mflag = 1;  /* 00C500 SEM */
    Y = pull16();  /* 00C501 PLY */
    D = pull16();  /* 00C502 PLD */
    S += 3; return;  /* 00C503 RTL */
}

/* 00C504  callers: 00C5DE */
void F_00C504(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C504 LDA */
    push16(D);  /* 00C506 PHD */
    D = A;  /* 00C507 TAD */
    A = rd16(ABS(0x03FD));  /* 00C508 LDA */
    mflag = 1;  /* 00C50B SEM */
    SETLO(A, A & 0x07);  /* 00C50C AND */
    wr8(DP(0x00), (uint8_t)A);  /* 00C50E STA */
    mflag = 0;  /* 00C510 CLM */
    A = rd16(ABS(0x039E));  /* 00C511 LDA */
    A &= 0x0003;  /* 00C514 AND */
    mflag = 1;  /* 00C517 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 00C518 STA */
    SETLO(A, rd8(DP(0x00)));  /* 00C51A LDA */
    C = ((A & 0xFF) >= 0x04);  /* 00C51C CMP */
    if (C) goto L_00C548;  /* 00C51E BCS */
    /* 00C520 CMP */
    if ((A & 0xFF) == 0x01) goto L_00C540;  /* 00C522 BEQ */
    /* 00C524 CMP */
    if ((A & 0xFF) != 0x02) goto L_00C530;  /* 00C526 BNE */
    SETLO(A, rd8(DP(0x01)));  /* 00C528 LDA */
    if ((A & 0xFF) == 0) goto L_00C540;  /* 00C52A BEQ */
    /* 00C52C CMP */
    if ((A & 0xFF) == 0x01) goto L_00C540;  /* 00C52E BEQ */
L_00C530: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00C530 LDA */
    /* 00C532 CMP */
    if ((A & 0xFF) != 0x03) goto L_00C544;  /* 00C534 BNE */
    SETLO(A, rd8(DP(0x01)));  /* 00C536 LDA */
    /* 00C538 CMP */
    if ((A & 0xFF) == 0x02) goto L_00C540;  /* 00C53A BEQ */
    /* 00C53C CMP */
    if ((A & 0xFF) != 0x03) goto L_00C544;  /* 00C53E BNE */
L_00C540: TICK();
    SETLO(A, 0x00);  /* 00C540 LDA */
    D = pull16();  /* 00C542 PLD */
    S += 3; return;  /* 00C543 RTL */
L_00C544: TICK();
    SETLO(A, 0x01);  /* 00C544 LDA */
    D = pull16();  /* 00C546 PLD */
    S += 3; return;  /* 00C547 RTL */
L_00C548: TICK();
    SETLO(A, adc8(A, 0xFB));  /* 00C548 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00C54A STA */
    t = rd8(DP(0x01));  /* 00C54C CMP */
    if ((A & 0xFF) == t) goto L_00C557;  /* 00C54E BEQ */
    mflag = 0;  /* 00C550 CLM */
    A = 0x0001;  /* 00C551 LDA */
    goto L_00C55B;  /* 00C554 JMP */
L_00C557: TICK();
    mflag = 0;  /* 00C557 CLM */
    A = 0x0000;  /* 00C558 LDA */
L_00C55B: TICK();
    mflag = 1;  /* 00C55B SEM */
    D = pull16();  /* 00C55C PLD */
    S += 3; return;  /* 00C55D RTL */
}

/* 00C55E  callers: 00CB4A */
void F_00C55E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C55E LDA */
    push16(D);  /* 00C560 PHD */
    D = A;  /* 00C561 TAD */
    A += 1;  /* 00C562 INC */
    push16(A);  /* 00C563 PHA */
    if ((rd16(ABS(0x0443)) & 0x0010) == 0x0010) goto L_00C56D;  /* 00C564 BBS */
    goto L_00C6EC;  /* 00C56A JMP */
L_00C56D: TICK();
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0010);  /* 00C56D CLB */
    S -= 3; F_00A0ED();  /* 00C572 JSRL */
    t = rd16(ABS(0x03FD));  /* 00C576 CMP */
    if (A != t) goto L_00C582;  /* 00C579 BNE */
    mflag = 1;  /* 00C57B SEM */
    t = (rd8(ABS(0x0416)) + 1) & 0xFF; wr8(ABS(0x0416), t);  /* 00C57C INC */
    goto L_00C587;  /* 00C57F JMP */
L_00C582: TICK();
    mflag = 1;  /* 00C582 SEM */
    wr8(ABS(0x0416), 0x00);  /* 00C583 LDM */
L_00C587: TICK();
    SETLO(A, rd8(ABS(0x0416)));  /* 00C587 LDA */
    t = rd8(ABS(0x03B2));  /* 00C58A CMP */
    if ((A & 0xFF) == t) goto L_00C592;  /* 00C58D BEQ */
    goto L_00C6E9;  /* 00C58F JMP */
L_00C592: TICK();
    mflag = 0;  /* 00C592 CLM */
    S -= 3; F_00C408();  /* 00C593 JSRL */
    /* 00C597 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C5AF;  /* 00C599 BNE */
    Y = rd16(ABS(0x0426));  /* 00C59B LDY */
    mflag = 0;  /* 00C59E CLM */
    S -= 3; F_00A10E();  /* 00C59F JSRL */
    Y = A;  /* 00C5A3 TAY */
    S -= 3; F_00A157();  /* 00C5A4 JSRL */
    /* 00C5A8 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C5AF;  /* 00C5AA BNE */
    goto L_00C6E9;  /* 00C5AC JMP */
L_00C5AF: TICK();
    wr8(ABS(0x03BA), 0x02);  /* 00C5AF LDM */
    mflag = 0;  /* 00C5B3 CLM */
    wr16(ABS(0x03B6), 0x0002);  /* 00C5B4 LDM */
    A = rd16(ABS(0x0412));  /* 00C5B9 LDA */
    mflag = 1;  /* 00C5BC SEM */
    wr8(ABS(0x040C), (uint8_t)A);  /* 00C5BD STA */
    SETLO(A, rd8(ABS(0x0413)));  /* 00C5C0 LDA */
    wr8(ABS(0x040D), (uint8_t)A);  /* 00C5C3 STA */
    SETLO(A, rd8(ABS(0x0411)));  /* 00C5C6 LDA */
    wr8(ABS(0x040B), (uint8_t)A);  /* 00C5C9 STA */
    mflag = 0;  /* 00C5CC CLM */
    X = rd16(ABS(0x03F5));  /* 00C5CD LDX */
    wr16(ABS(0x03F1), X);  /* 00C5D0 STX */
    S -= 3; F_00A4D6();  /* 00C5D3 JSRL */
    A = rd16(ABS(0x03B4));  /* 00C5D7 LDA */
    mflag = 1;  /* 00C5DA SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00C5DB STA */
    mflag = 0;  /* 00C5DD CLM */
    S -= 3; F_00C504();  /* 00C5DE JSRL */
    /* 00C5E2 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C5F7;  /* 00C5E4 BNE */
    Y = rd16(ABS(0x0426));  /* 00C5E6 LDY */
    mflag = 0;  /* 00C5E9 CLM */
    S -= 3; F_00A10E();  /* 00C5EA JSRL */
    Y = A;  /* 00C5EE TAY */
    S -= 3; F_00A1EE();  /* 00C5EF JSRL */
    /* 00C5F3 CMP */
    if ((A & 0xFF) == 0x00) goto L_00C5FF;  /* 00C5F5 BEQ */
L_00C5F7: TICK();
    mflag = 0;  /* 00C5F7 CLM */
    S -= 3; F_00A399();  /* 00C5F8 JSRL */
    goto L_00C6E9;  /* 00C5FC JMP */
L_00C5FF: TICK();
    mflag = 0;  /* 00C5FF CLM */
    A = rd16(ABS(0x03FE));  /* 00C600 LDA */
    A &= 0x00FF;  /* 00C603 AND */
    /* 00C606 CMP */
    if (A >= 0x0040) goto L_00C621;  /* 00C609 BCS */
    A = rd16(DP(0x00));  /* 00C60B LDA */
    A &= 0x00FF;  /* 00C60D AND */
    wr16(DP(0x01), A);  /* 00C610 STA */
    A = rd16(ABS(0x03FD));  /* 00C612 LDA */
    A = asl16(A);  /* 00C615 ASL */
    A = rla16(A, 0x0008);  /* 00C616 RLA */
    A &= 0x00FF;  /* 00C61A AND */
    t = rd16(DP(0x01));  /* 00C61D CMP */
    if (A == t) goto L_00C648;  /* 00C61F BEQ */
L_00C621: TICK();
    A = rd16(ABS(0x03FE));  /* 00C621 LDA */
    A &= 0x00FF;  /* 00C624 AND */
    /* 00C627 CMP */
    if (A >= 0x0040) goto L_00C62F;  /* 00C62A BCS */
    goto L_00C6E4;  /* 00C62C JMP */
L_00C62F: TICK();
    A = rd16(DP(0x00));  /* 00C62F LDA */
    mflag = 1;  /* 00C631 SEM */
    SETLO(A, A & 0xE0);  /* 00C632 AND */
    wr8(DP(0x01), (uint8_t)A);  /* 00C634 STA */
    mflag = 0;  /* 00C636 CLM */
    A = rd16(ABS(0x03FE));  /* 00C637 LDA */
    A &= 0x00FF;  /* 00C63A AND */
    mflag = 1;  /* 00C63D SEM */
    SETLO(A, A & 0x70);  /* 00C63E AND */
    SETLO(A, asl8(A));  /* 00C640 ASL */
    t = rd8(DP(0x01));  /* 00C641 CMP */
    if ((A & 0xFF) == t) goto L_00C648;  /* 00C643 BEQ */
    goto L_00C6E4;  /* 00C645 JMP */
L_00C648: TICK();
    Y = 0x0005;  /* 00C648 LDY */
    mflag = 0;  /* 00C64B CLM */
    S -= 3; F_008DC6();  /* 00C64C JSRL */
    A = rd16(ABS(0x0441));  /* 00C650 LDA */
    A &= 0x00FF;  /* 00C653 AND */
    A ^= 0xFFFF;  /* 00C656 EOR */
    A += 1;  /* 00C659 INC */
    A += 1;  /* 00C65A INC */
    A = asl16(A);  /* 00C65B ASL */
    X = A;  /* 00C65C TAX */
    A = rd16((ABS(0x0439) + X));  /* 00C65D LDA */
    t = rd16(ABS(0x03F5));  /* 00C660 CMP */
    if (A != t) goto L_00C697;  /* 00C663 BNE */
    mflag = 1;  /* 00C665 SEM */
    wr8(ABS(0x01BE), 0x0C);  /* 00C666 LDM */
    SETLO(A, rd8(ABS(0x0441)));  /* 00C66A LDA */
    mflag = 0;  /* 00C66D CLM */
    A &= 0x00FF;  /* 00C66E AND */
    A = asl16(A);  /* 00C671 ASL */
    X = A;  /* 00C672 TAX */
    A = rd16(ABS(0x03F5));  /* 00C673 LDA */
    wr16((ABS(0x0439) + X), A);  /* 00C676 STA */
    mflag = 1;  /* 00C679 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C67A LDA [AN5 RSSI] */
    mflag = 0;  /* 00C67D CLM */
    A &= 0x00FF;  /* 00C67E AND */
    wr16(DP(0x01), A);  /* 00C681 STA */
    A = rd16(ABS(0x0441));  /* 00C683 LDA */
    A &= 0x00FF;  /* 00C686 AND */
    A = asl16(A);  /* 00C689 ASL */
    C = 0;  /* 00C68A CLC */
    A = adc16(A, 0x0004);  /* 00C68B ADC */
    X = A;  /* 00C68E TAX */
    A = rd16(DP(0x01));  /* 00C68F LDA */
    wr16((ABS(0x0439) + X), A);  /* 00C691 STA */
    goto L_00C6E1;  /* 00C694 JMP */
L_00C697: TICK();
    mflag = 1;  /* 00C697 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C698 LDA [AN5 RSSI] */
    mflag = 0;  /* 00C69B CLM */
    A &= 0x00FF;  /* 00C69C AND */
    wr16(DP(0x01), A);  /* 00C69F STA */
    A = rd16(ABS(0x0441));  /* 00C6A1 LDA */
    A &= 0x00FF;  /* 00C6A4 AND */
    A = asl16(A);  /* 00C6A7 ASL */
    C = 0;  /* 00C6A8 CLC */
    A = adc16(A, 0x0004);  /* 00C6A9 ADC */
    X = A;  /* 00C6AC TAX */
    A = rd16((ABS(0x0439) + X));  /* 00C6AD LDA */
    t = rd16(DP(0x01));  /* 00C6B0 CMP */
    if (A >= t) goto L_00C6DD;  /* 00C6B2 BCS */
    A = rd16(ABS(0x0441));  /* 00C6B4 LDA */
    A &= 0x00FF;  /* 00C6B7 AND */
    A = asl16(A);  /* 00C6BA ASL */
    X = A;  /* 00C6BB TAX */
    A = rd16(ABS(0x03F5));  /* 00C6BC LDA */
    wr16((ABS(0x0439) + X), A);  /* 00C6BF STA */
    mflag = 1;  /* 00C6C2 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C6C3 LDA [AN5 RSSI] */
    mflag = 0;  /* 00C6C6 CLM */
    A &= 0x00FF;  /* 00C6C7 AND */
    wr16(DP(0x01), A);  /* 00C6CA STA */
    A = rd16(ABS(0x0441));  /* 00C6CC LDA */
    A &= 0x00FF;  /* 00C6CF AND */
    A = asl16(A);  /* 00C6D2 ASL */
    C = 0;  /* 00C6D3 CLC */
    A = adc16(A, 0x0004);  /* 00C6D4 ADC */
    X = A;  /* 00C6D7 TAX */
    A = rd16(DP(0x01));  /* 00C6D8 LDA */
    wr16((ABS(0x0439) + X), A);  /* 00C6DA STA */
L_00C6DD: TICK();
    S -= 3; F_00A399();  /* 00C6DD JSRL */
L_00C6E1: TICK();
    goto L_00C6E9;  /* 00C6E1 JMP */
L_00C6E4: TICK();
    mflag = 0;  /* 00C6E4 CLM */
    S -= 3; F_00A399();  /* 00C6E5 JSRL */
L_00C6E9: TICK();
    goto L_00C6F6;  /* 00C6E9 JMP */
L_00C6EC: TICK();
    mflag = 0;  /* 00C6EC CLM */
    X = rd16(ABS(0x03F7));  /* 00C6ED LDX */
    if (X != 0) goto L_00C6F6;  /* 00C6F0 BNE */
    S -= 3; F_00A399();  /* 00C6F2 JSRL */
L_00C6F6: TICK();
    mflag = 0;  /* 00C6F6 CLM */
    Y = pull16();  /* 00C6F7 PLY */
    D = pull16();  /* 00C6F8 PLD */
    S += 3; return;  /* 00C6F9 RTL */
}

/* 00C6FA  callers: 00CB39 */
void F_00C6FA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C6FA LDA */
    push16(D);  /* 00C6FC PHD */
    D = A;  /* 00C6FD TAD */
    push16(A);  /* 00C6FE PHA */
    if ((rd16(ABS(0x0443)) & 0x0010) == 0) goto L_00C755;  /* 00C6FF BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0010);  /* 00C705 CLB */
    mflag = 1;  /* 00C70A SEM */
    wr8(ABS(0x03BA), 0x41);  /* 00C70B LDM */
    SETLO(A, rd8(ABS(0x03B2)));  /* 00C70F LDA */
    mpy8(0x0B);  /* 00C712 MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 00C715 STA */
    wr8(DP(0x01), (uint8_t)B);  /* 00C717 STA */
    mflag = 0;  /* 00C71A CLM */
    X = rd16(DP(0x00));  /* 00C71B LDX */
    wr16(ABS(0x03F7), X);  /* 00C71D STX */
    mflag = 1;  /* 00C720 SEM */
    wr8(ABS(0x0416), 0x00);  /* 00C721 LDM */
    SETLO(A, rd8(ABS(0x041A)));  /* 00C725 LDA */
    wr8(ABS(0x0421), (uint8_t)A);  /* 00C728 STA */
    mflag = 0;  /* 00C72B CLM */
    S -= 3; F_00A0ED();  /* 00C72C JSRL */
    wr16(ABS(0x03FD), A);  /* 00C730 STA */
    X = rd16(ABS(0x03FF));  /* 00C733 LDX */
    if (X == 0) goto L_00C752;  /* 00C736 BEQ */
    Y = rd16(ABS(0x0426));  /* 00C738 LDY */
    S -= 3; F_00A10E();  /* 00C73B JSRL */
    Y = A;  /* 00C73F TAY */
    S -= 3; F_00A157();  /* 00C740 JSRL */
    /* 00C744 CMP */
    if ((A & 0xFF) != 0x00) goto L_00C752;  /* 00C746 BNE */
    SETLO(A, rd8(ABS(0x03A7)));  /* 00C748 LDA */
    if ((A & 0xFF) != 0) goto L_00C752;  /* 00C74B BNE */
    mflag = 0;  /* 00C74D CLM */
    S -= 3; F_00A29F();  /* 00C74E JSRL */
L_00C752: TICK();
    goto L_00C764;  /* 00C752 JMP */
L_00C755: TICK();
    mflag = 0;  /* 00C755 CLM */
    X = rd16(ABS(0x03F7));  /* 00C756 LDX */
    if (X != 0) goto L_00C764;  /* 00C759 BNE */
    S -= 3; F_00A399();  /* 00C75B JSRL */
    mflag = 1;  /* 00C75F SEM */
    wr8(ABS(0x01BE), 0x00);  /* 00C760 LDM */
L_00C764: TICK();
    mflag = 0;  /* 00C764 CLM */
    Y = pull16();  /* 00C765 PLY */
    D = pull16();  /* 00C766 PLD */
    S += 3; return;  /* 00C767 RTL */
}

/* 00C768  callers: 00CB28 */
void F_00C768(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C768 LDA */
    push16(A);  /* 00C76A PHA */
    if ((rd16(ABS(0x0443)) & 0x0010) == 0) goto L_00C781;  /* 00C76B BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0010);  /* 00C771 CLB */
    mflag = 1;  /* 00C776 SEM */
    wr8(ABS(0x03BA), 0x40);  /* 00C777 LDM */
    mflag = 0;  /* 00C77B CLM */
    wr16(ABS(0x03F7), 0x000B);  /* 00C77C LDM */
L_00C781: TICK();
    if ((rd16(ABS(0x0443)) & 0x0020) == 0) goto L_00C7A5;  /* 00C781 BBC */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0020);  /* 00C787 CLB */
    mflag = 1;  /* 00C78C SEM */
    SETLO(A, rd8(ABS(0x03A5)));  /* 00C78D LDA */
    if ((A & 0xFF) == 0) goto L_00C7A5;  /* 00C790 BEQ */
    mflag = 0;  /* 00C792 CLM */
    X = rd16(ABS(0x03F5));  /* 00C793 LDX */
    t = rd16(ABS(0x03F9));  /* 00C796 CPX */
    if (X != t) goto L_00C7A5;  /* 00C799 BNE */
    wr16(ABS(0x03F7), 0x1388);  /* 00C79B LDM */
    mflag = 1;  /* 00C7A0 SEM */
    wr8(ABS(0x03A7), 0x00);  /* 00C7A1 LDM */
L_00C7A5: TICK();
    mflag = 0;  /* 00C7A5 CLM */
    X = rd16(ABS(0x03F7));  /* 00C7A6 LDX */
    if (X == 0) goto L_00C7B6;  /* 00C7A9 BEQ */
    /* 00C7AB CPX */
    if (X >= 0x0006) goto L_00C7C6;  /* 00C7AE BCS */
    mflag = 1;  /* 00C7B0 SEM */
    if ((rd8(ABS(P5)) & 0x80) == 0x80) goto L_00C7C6;  /* 00C7B1 BBS [P57 SUSTURMA] */
L_00C7B6: TICK();
    mflag = 1;  /* 00C7B6 SEM */
    SETLO(A, rd8(ABS(0x0227)));  /* 00C7B7 LDA */
    if ((A & 0xFF) != 0) goto L_00C7C6;  /* 00C7BA BNE */
    mflag = 0;  /* 00C7BC CLM */
    S -= 3; F_00A399();  /* 00C7BD JSRL */
    mflag = 1;  /* 00C7C1 SEM */
    wr8(ABS(0x01BE), 0x00);  /* 00C7C2 LDM */
L_00C7C6: TICK();
    mflag = 0;  /* 00C7C6 CLM */
    Y = pull16();  /* 00C7C7 PLY */
    S += 3; return;  /* 00C7C8 RTL */
}

/* 00C7C9  callers: 00CB17 */
void F_00C7C9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C7C9 LDA */
    push16(D);  /* 00C7CB PHD */
    D = A;  /* 00C7CC TAD */
    push16(A);  /* 00C7CD PHA */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_00C7D7;  /* 00C7CE BBS */
    goto L_00C87C;  /* 00C7D4 JMP */
L_00C7D7: TICK();
    mflag = 1;  /* 00C7D7 SEM */
    SETLO(A, rd8(ABS(0x0442)));  /* 00C7D8 LDA */
    if ((A & 0xFF) != 0) goto L_00C7E0;  /* 00C7DB BNE */
    goto L_00C87C;  /* 00C7DD JMP */
L_00C7E0: TICK();
    Y = 0x0005;  /* 00C7E0 LDY */
    mflag = 0;  /* 00C7E3 CLM */
    S -= 3; F_008DC6();  /* 00C7E4 JSRL */
    A = rd16(ABS(0x0441));  /* 00C7E8 LDA */
    A &= 0x00FF;  /* 00C7EB AND */
    A = asl16(A);  /* 00C7EE ASL */
    X = A;  /* 00C7EF TAX */
    A = rd16((ABS(0x0439) + X));  /* 00C7F0 LDA */
    t = rd16(ABS(0x03F5));  /* 00C7F3 CMP */
    if (A != t) goto L_00C80A;  /* 00C7F6 BNE */
    wr16(ABS(0x03B6), 0x0004);  /* 00C7F8 LDM */
    mflag = 1;  /* 00C7FD SEM */
    wr8(ABS(0x03BA), 0x02);  /* 00C7FE LDM */
    wr8(ABS(0x0442), 0x00);  /* 00C802 LDM */
    mflag = 0;  /* 00C806 CLM */
    Y = pull16();  /* 00C807 PLY */
    D = pull16();  /* 00C808 PLD */
    S += 3; return;  /* 00C809 RTL */
L_00C80A: TICK();
    mflag = 1;  /* 00C80A SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C80B LDA [AN5 RSSI] */
    mflag = 0;  /* 00C80E CLM */
    A &= 0x00FF;  /* 00C80F AND */
    wr16(DP(0x00), A);  /* 00C812 STA */
    A = rd16(ABS(0x0441));  /* 00C814 LDA */
    A &= 0x00FF;  /* 00C817 AND */
    A = asl16(A);  /* 00C81A ASL */
    C = 0;  /* 00C81B CLC */
    A = adc16(A, 0x0004);  /* 00C81C ADC */
    X = A;  /* 00C81F TAX */
    A = rd16((ABS(0x0439) + X));  /* 00C820 LDA */
    t = rd16(DP(0x00));  /* 00C823 CMP */
    if (A >= t) goto L_00C858;  /* 00C825 BCS */
    A = rd16(ABS(0x0441));  /* 00C827 LDA */
    A &= 0x00FF;  /* 00C82A AND */
    A = asl16(A);  /* 00C82D ASL */
    X = A;  /* 00C82E TAX */
    A = rd16(ABS(0x03F5));  /* 00C82F LDA */
    wr16((ABS(0x0439) + X), A);  /* 00C832 STA */
    mflag = 1;  /* 00C835 SEM */
    SETLO(A, rd8(ABS(AD5)));  /* 00C836 LDA [AN5 RSSI] */
    mflag = 0;  /* 00C839 CLM */
    A &= 0x00FF;  /* 00C83A AND */
    wr16(DP(0x00), A);  /* 00C83D STA */
    A = rd16(ABS(0x0441));  /* 00C83F LDA */
    A &= 0x00FF;  /* 00C842 AND */
    A = asl16(A);  /* 00C845 ASL */
    C = 0;  /* 00C846 CLC */
    A = adc16(A, 0x0004);  /* 00C847 ADC */
    X = A;  /* 00C84A TAX */
    A = rd16(DP(0x00));  /* 00C84B LDA */
    wr16((ABS(0x0439) + X), A);  /* 00C84D STA */
    mflag = 1;  /* 00C850 SEM */
    wr8(ABS(0x0442), 0x00);  /* 00C851 LDM */
    goto L_00C87C;  /* 00C855 JMP */
L_00C858: TICK();
    mflag = 0;  /* 00C858 CLM */
    A = rd16(ABS(0x0441));  /* 00C859 LDA */
    A &= 0x00FF;  /* 00C85C AND */
    A = asl16(A);  /* 00C85F ASL */
    X = A;  /* 00C860 TAX */
    A = rd16((ABS(0x0439) + X));  /* 00C861 LDA */
    wr16(ABS(0x03F1), A);  /* 00C864 STA */
    wr16(ABS(0x03F5), A);  /* 00C867 STA */
    S -= 3; F_00A01C();  /* 00C86A JSRL */
    mflag = 1;  /* 00C86E SEM */
    wr8(ABS(0x0229), 0x01);  /* 00C86F LDM */
    mflag = 0;  /* 00C873 CLM */
    wr16(ABS(0x03F7), 0x0006);  /* 00C874 LDM */
    Y = pull16();  /* 00C879 PLY */
    D = pull16();  /* 00C87A PLD */
    S += 3; return;  /* 00C87B RTL */
L_00C87C: TICK();
    mflag = 1;  /* 00C87C SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00C87D LDA */
    if ((A & 0xFF) != 0) goto L_00C891;  /* 00C880 BNE */
    SETLO(A, rd8(ABS(0x01BD)));  /* 00C882 LDA */
    if ((A & 0xFF) != 0) goto L_00C891;  /* 00C885 BNE */
    SETLO(A, rd8(ABS(0x03AE)));  /* 00C887 LDA */
    if ((A & 0xFF) != 0) goto L_00C891;  /* 00C88A BNE */
    SETLO(A, rd8(ABS(0x03A8)));  /* 00C88C LDA */
    if ((A & 0xFF) == 0) goto L_00C89A;  /* 00C88F BEQ */
L_00C891: TICK();
    SETLO(A, rd8(ABS(0x0422)));  /* 00C891 LDA */
    if ((A & 0xFF) != 0) goto L_00C89A;  /* 00C894 BNE */
    wr8(ABS(0x01BE), 0x0B);  /* 00C896 LDM */
L_00C89A: TICK();
    wr8(ABS(0x01BD), 0x00);  /* 00C89A LDM */
    wr8(ABS(0x03B8), 0x00);  /* 00C89E LDM */
    wr8(ABS(0x03AE), 0x00);  /* 00C8A2 LDM */
    wr8(ABS(0x03A8), 0x00);  /* 00C8A6 LDM */
    SETLO(A, rd8(ABS(0x0229)));  /* 00C8AA LDA */
    /* 00C8AD CMP */
    if ((A & 0xFF) == 0x01) goto L_00C8D0;  /* 00C8AF BEQ */
    wr8(ABS(0x0229), 0x01);  /* 00C8B1 LDM */
    mflag = 0;  /* 00C8B5 CLM */
    X = rd16(ABS(0x03F1));  /* 00C8B6 LDX */
    if (X == 0) goto L_00C8C0;  /* 00C8B9 BEQ */
    /* 00C8BB CPX */
    if (X < 0x0400) goto L_00C8CA;  /* 00C8BE BCC */
L_00C8C0: TICK();
    S -= 3; F_00A29F();  /* 00C8C0 JSRL */
    X = rd16(ABS(0x03F5));  /* 00C8C4 LDX */
    wr16(ABS(0x03F1), X);  /* 00C8C7 STX */
L_00C8CA: TICK();
    wr16(ABS(0x03F5), X);  /* 00C8CA STX */
    Y = pull16();  /* 00C8CD PLY */
    D = pull16();  /* 00C8CE PLD */
    S += 3; return;  /* 00C8CF RTL */
L_00C8D0: TICK();
    mflag = 0;  /* 00C8D0 CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0) goto L_00C904;  /* 00C8D1 BBC */
    mflag = 1;  /* 00C8D7 SEM */
    wr8(ABS(0x03BA), 0x80);  /* 00C8D8 LDM */
    SETLO(A, rd8(ABS(0x03A5)));  /* 00C8DC LDA */
    if ((A & 0xFF) == 0) goto L_00C8F0;  /* 00C8DF BEQ */
    mflag = 0;  /* 00C8E1 CLM */
    X = rd16(ABS(0x03F5));  /* 00C8E2 LDX */
    t = rd16(ABS(0x03F9));  /* 00C8E5 CPX */
    if (X != t) goto L_00C8F0;  /* 00C8E8 BNE */
    A = 0x1388;  /* 00C8EA LDA */
    goto L_00C8F4;  /* 00C8ED JMP */
L_00C8F0: TICK();
    mflag = 0;  /* 00C8F0 CLM */
    A = 0x0006;  /* 00C8F1 LDA */
L_00C8F4: TICK();
    wr16(ABS(0x03F7), A);  /* 00C8F4 STA */
    mflag = 1;  /* 00C8F7 SEM */
    SETLO(A, rd8(ABS(0x03A7)));  /* 00C8F8 LDA */
    if ((A & 0xFF) == 0) goto L_00C901;  /* 00C8FB BEQ */
    wr8(ABS(0x0422), 0xB4);  /* 00C8FD LDM */
L_00C901: TICK();
    goto L_00C918;  /* 00C901 JMP */
L_00C904: TICK();
    mflag = 0;  /* 00C904 CLM */
    X = rd16(ABS(0x03F7));  /* 00C905 LDX */
    if (X != 0) goto L_00C918;  /* 00C908 BNE */
    wr16(ABS(0x03F7), 0x0006);  /* 00C90A LDM */
    S -= 3; F_00A399();  /* 00C90F JSRL */
    mflag = 1;  /* 00C913 SEM */
    wr8(ABS(0x01BE), 0x00);  /* 00C914 LDM */
L_00C918: TICK();
    mflag = 0;  /* 00C918 CLM */
    Y = pull16();  /* 00C919 PLY */
    D = pull16();  /* 00C91A PLD */
    S += 3; return;  /* 00C91B RTL */
}

/* 00C91C  callers: 01A792 */
void F_00C91C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00C91C LDA */
    push16(D);  /* 00C91E PHD */
    D = A;  /* 00C91F TAD */
    push16(A);  /* 00C920 PHA */
    A = rd16(ABS(0x0228));  /* 00C921 LDA */
    mflag = 1;  /* 00C924 SEM */
    /* 00C925 CMP */
    if ((A & 0xFF) != 0x03) goto L_00C92F;  /* 00C927 BNE */
    SETLO(A, rd8(ABS(0x041A)));  /* 00C929 LDA */
    wr8(ABS(0x0421), (uint8_t)A);  /* 00C92C STA */
L_00C92F: TICK();
    SETLO(A, rd8(ABS(0x03BA)));  /* 00C92F LDA */
    /* 00C932 CMP */
    if ((A & 0xFF) != 0x04) goto L_00C93E;  /* 00C934 BNE */
    mflag = 0;  /* 00C936 CLM */
    S -= 3; F_00BC1D();  /* 00C937 JSRL */
    goto L_00CB02;  /* 00C93B JMP */
L_00C93E: TICK();
    mflag = 0;  /* 00C93E CLM */
    A = rd16(ABS(0x03BA));  /* 00C93F LDA */
    mflag = 1;  /* 00C942 SEM */
    /* 00C943 CMP */
    if ((A & 0xFF) == 0x02) goto L_00C94A;  /* 00C945 BEQ */
    goto L_00CA3B;  /* 00C947 JMP */
L_00C94A: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00C94A LDA */
    /* 00C94D CMP */
    if ((A & 0xFF) != 0x07) goto L_00C95A;  /* 00C94F BNE */
    wr8(ABS(0x03B9), 0x11);  /* 00C951 LDM */
    mflag = 0;  /* 00C955 CLM */
    S -= 3; F_00A3A8();  /* 00C956 JSRL */
L_00C95A: TICK();
    mflag = 1;  /* 00C95A SEM */
    SETLO(A, rd8(ABS(0x03A1)));  /* 00C95B LDA */
    if ((A & 0xFF) == 0) goto L_00C969;  /* 00C95E BEQ */
    SETLO(A, rd8(ABS(0x03A0)));  /* 00C960 LDA */
    if ((A & 0xFF) != 0) goto L_00C969;  /* 00C963 BNE */
    wr8(ABS(0x03AA), 0x01);  /* 00C965 LDM */
L_00C969: TICK();
    SETLO(A, rd8(ABS(0x03AA)));  /* 00C969 LDA */
    if ((A & 0xFF) == 0) goto L_00C973;  /* 00C96C BEQ */
    mflag = 0;  /* 00C96E CLM */
    S -= 3; F_00A40A();  /* 00C96F JSRL */
L_00C973: TICK();
    mflag = 1;  /* 00C973 SEM */
    SETLO(A, rd8(ABS(0x03A9)));  /* 00C974 LDA */
    if ((A & 0xFF) == 0) goto L_00C9F4;  /* 00C977 BEQ */
    wr8(ABS(0x041D), 0x02);  /* 00C979 LDM */
    mflag = 0;  /* 00C97D CLM */
    S -= 3; F_00AD96();  /* 00C97E JSRL */
    mflag = 1;  /* 00C982 SEM */
    SETLO(A, rd8(ABS(0x03AE)));  /* 00C983 LDA */
    if ((A & 0xFF) != 0) goto L_00C9DD;  /* 00C986 BNE */
    mflag = 0;  /* 00C988 CLM */
    X = rd16(ABS(0x03B6));  /* 00C989 LDX */
    /* 00C98C CPX */
    if (X != 0x0004) goto L_00C998;  /* 00C98F BNE */
    S -= 3; F_00BF79();  /* 00C991 JSRL */
    goto L_00C9DD;  /* 00C995 JMP */
L_00C998: TICK();
    mflag = 0;  /* 00C998 CLM */
    X = rd16(ABS(0x03B6));  /* 00C999 LDX */
    /* 00C99C CPX */
    if (X != 0x0001) goto L_00C9A8;  /* 00C99F BNE */
    S -= 3; F_00C109();  /* 00C9A1 JSRL */
    goto L_00C9DD;  /* 00C9A5 JMP */
L_00C9A8: TICK();
    mflag = 0;  /* 00C9A8 CLM */
    X = rd16(ABS(0x03B6));  /* 00C9A9 LDX */
    /* 00C9AC CPX */
    if (X != 0x0002) goto L_00C9B8;  /* 00C9AF BNE */
    S -= 3; F_00C357();  /* 00C9B1 JSRL */
    goto L_00C9DD;  /* 00C9B5 JMP */
L_00C9B8: TICK();
    mflag = 0;  /* 00C9B8 CLM */
    X = rd16(ABS(0x03B6));  /* 00C9B9 LDX */
    /* 00C9BC CPX */
    if (X != 0x0003) goto L_00C9C8;  /* 00C9BF BNE */
    S -= 3; F_00C291();  /* 00C9C1 JSRL */
    goto L_00C9DD;  /* 00C9C5 JMP */
L_00C9C8: TICK();
    mflag = 0;  /* 00C9C8 CLM */
    X = rd16(ABS(0x03B6));  /* 00C9C9 LDX */
    /* 00C9CC CPX */
    if (X != 0x0005) goto L_00C9D8;  /* 00C9CF BNE */
    S -= 3; F_00C341();  /* 00C9D1 JSRL */
    goto L_00C9DD;  /* 00C9D5 JMP */
L_00C9D8: TICK();
    mflag = 0;  /* 00C9D8 CLM */
    S -= 3; F_00B98D();  /* 00C9D9 JSRL */
L_00C9DD: TICK();
    mflag = 0;  /* 00C9DD CLM */
    A = rd16(ABS(0x03B9));  /* 00C9DE LDA */
    mflag = 1;  /* 00C9E1 SEM */
    /* 00C9E2 CMP */
    if ((A & 0xFF) != 0x16) goto L_00C9F4;  /* 00C9E4 BNE */
    mflag = 0;  /* 00C9E6 CLM */
    S -= 3; F_00AC52();  /* 00C9E7 JSRL */
    t = rd16(ABS(0x03F1));  /* 00C9EB CMP */
    if (A != t) goto L_00C9F4;  /* 00C9EE BNE */
    S -= 3; F_00B8BA();  /* 00C9F0 JSRL */
L_00C9F4: TICK();
    mflag = 1;  /* 00C9F4 SEM */
    SETLO(A, rd8(ABS(0x03AE)));  /* 00C9F5 LDA */
    if ((A & 0xFF) == 0) goto L_00CA1D;  /* 00C9F8 BEQ */
    mflag = 0;  /* 00C9FA CLM */
    wr16(DP(0x00), 0x0009);  /* 00C9FB LDM */
    wr16(DP(0x02), 0x0040);  /* 00C9FF LDM */
    Y = 0x0002;  /* 00CA03 LDY */
    A = D;  /* 00CA06 TDA */
    C = 0;  /* 00CA07 CLC */
    A = adc16(A, 0x0004);  /* 00CA08 ADC */
    push16(A);  /* 00CA0B PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00CA0C JSRL */
    Y = pull16();  /* 00CA10 PLY */
    wr8(ABS(0x03AE), 0x00);  /* 00CA11 LDM */
    wr8(ABS(0x03BA), 0x04);  /* 00CA15 LDM */
    wr8(ABS(0x0229), 0x03);  /* 00CA19 LDM */
L_00CA1D: TICK();
    SETLO(A, rd8(ABS(0x0422)));  /* 00CA1D LDA */
    if ((A & 0xFF) != 0) goto L_00CA38;  /* 00CA20 BNE */
    SETLO(A, rd8(ABS(0x03A6)));  /* 00CA22 LDA */
    if ((A & 0xFF) != 0) goto L_00CA2C;  /* 00CA25 BNE */
    SETLO(A, rd8(ABS(0x03A7)));  /* 00CA27 LDA */
    if ((A & 0xFF) == 0) goto L_00CA33;  /* 00CA2A BEQ */
L_00CA2C: TICK();
    wr8(ABS(0x03BA), 0xC0);  /* 00CA2C LDM */
    goto L_00CA38;  /* 00CA30 JMP */
L_00CA33: TICK();
    mflag = 1;  /* 00CA33 SEM */
    wr8(ABS(0x03BA), 0x00);  /* 00CA34 LDM */
L_00CA38: TICK();
    goto L_00CB02;  /* 00CA38 JMP */
L_00CA3B: TICK();
    mflag = 0;  /* 00CA3B CLM */
    A = rd16(ABS(0x03BA));  /* 00CA3C LDA */
    mflag = 1;  /* 00CA3F SEM */
    /* 00CA40 CMP */
    if ((A & 0xFF) != 0x03) goto L_00CA94;  /* 00CA42 BNE */
    SETLO(A, rd8(ABS(0x022A)));  /* 00CA44 LDA */
    if ((A & 0xFF) != 0) goto L_00CA57;  /* 00CA47 BNE */
    mflag = 0;  /* 00CA49 CLM */
    S -= 3; F_00A57F();  /* 00CA4A JSRL */
    mflag = 1;  /* 00CA4E SEM */
    wr8(ABS(0x022A), 0x64);  /* 00CA4F LDM */
    mflag = 0;  /* 00CA53 CLM */
    Y = pull16();  /* 00CA54 PLY */
    D = pull16();  /* 00CA55 PLD */
    S += 3; return;  /* 00CA56 RTL */
L_00CA57: TICK();
    mflag = 0;  /* 00CA57 CLM */
    if ((rd16(ABS(0x0443)) & 0x0008) == 0x0008) goto L_00CA64;  /* 00CA58 BBS */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_00CA76;  /* 00CA5E BBS */
L_00CA64: TICK();
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00CA64 CLB */
    mflag = 1;  /* 00CA69 SEM */
    wr8(ABS(0x03BA), 0x02);  /* 00CA6A LDM */
    wr8(ABS(0x0229), 0x01);  /* 00CA6E LDM */
    mflag = 0;  /* 00CA72 CLM */
    Y = pull16();  /* 00CA73 PLY */
    D = pull16();  /* 00CA74 PLD */
    S += 3; return;  /* 00CA75 RTL */
L_00CA76: TICK();
    A = rd16(ABS(0x03B9));  /* 00CA76 LDA */
    mflag = 1;  /* 00CA79 SEM */
    /* 00CA7A CMP */
    if ((A & 0xFF) != 0x14) goto L_00CA83;  /* 00CA7C BNE */
    mflag = 0;  /* 00CA7E CLM */
    S -= 3; F_00B6D4();  /* 00CA7F JSRL */
L_00CA83: TICK();
    mflag = 0;  /* 00CA83 CLM */
    A = rd16(ABS(0x03B9));  /* 00CA84 LDA */
    mflag = 1;  /* 00CA87 SEM */
    /* 00CA88 CMP */
    if ((A & 0xFF) != 0x12) goto L_00CA91;  /* 00CA8A BNE */
    mflag = 0;  /* 00CA8C CLM */
    S -= 3; F_00B6A6();  /* 00CA8D JSRL */
L_00CA91: TICK();
    goto L_00CB02;  /* 00CA91 JMP */
L_00CA94: TICK();
    mflag = 0;  /* 00CA94 CLM */
    A = rd16(ABS(0x03BA));  /* 00CA95 LDA */
    mflag = 1;  /* 00CA98 SEM */
    /* 00CA99 CMP */
    if ((A & 0xFF) != 0x05) goto L_00CB02;  /* 00CA9B BNE */
    mflag = 0;  /* 00CA9D CLM */
    X = rd16(ABS(0x03B6));  /* 00CA9E LDX */
    /* 00CAA1 CPX */
    if (X != 0x0006) goto L_00CAAD;  /* 00CAA4 BNE */
    S -= 3; F_00BC5C();  /* 00CAA6 JSRL */
    goto L_00CB02;  /* 00CAAA JMP */
L_00CAAD: TICK();
    mflag = 0;  /* 00CAAD CLM */
    X = rd16(ABS(0x03B6));  /* 00CAAE LDX */
    /* 00CAB1 CPX */
    if (X != 0x0008) goto L_00CABD;  /* 00CAB4 BNE */
    S -= 3; F_00BD37();  /* 00CAB6 JSRL */
    goto L_00CB02;  /* 00CABA JMP */
L_00CABD: TICK();
    mflag = 0;  /* 00CABD CLM */
    X = rd16(ABS(0x03B6));  /* 00CABE LDX */
    /* 00CAC1 CPX */
    if (X != 0x0009) goto L_00CACD;  /* 00CAC4 BNE */
    S -= 3; F_00BD9B();  /* 00CAC6 JSRL */
    goto L_00CB02;  /* 00CACA JMP */
L_00CACD: TICK();
    mflag = 0;  /* 00CACD CLM */
    X = rd16(ABS(0x03B6));  /* 00CACE LDX */
    /* 00CAD1 CPX */
    if (X != 0x0007) goto L_00CADD;  /* 00CAD4 BNE */
    S -= 3; F_00BCBF();  /* 00CAD6 JSRL */
    goto L_00CB02;  /* 00CADA JMP */
L_00CADD: TICK();
    mflag = 0;  /* 00CADD CLM */
    X = rd16(ABS(0x03B6));  /* 00CADE LDX */
    /* 00CAE1 CPX */
    if (X != 0x000A) goto L_00CAED;  /* 00CAE4 BNE */
    S -= 3; F_00BF11();  /* 00CAE6 JSRL */
    goto L_00CB02;  /* 00CAEA JMP */
L_00CAED: TICK();
    mflag = 0;  /* 00CAED CLM */
    X = rd16(ABS(0x03B6));  /* 00CAEE LDX */
    /* 00CAF1 CPX */
    if (X != 0x000B) goto L_00CAFD;  /* 00CAF4 BNE */
    S -= 3; F_00BF56();  /* 00CAF6 JSRL */
    goto L_00CB02;  /* 00CAFA JMP */
L_00CAFD: TICK();
    mflag = 0;  /* 00CAFD CLM */
    S -= 3; F_00B98D();  /* 00CAFE JSRL */
L_00CB02: TICK();
    mflag = 1;  /* 00CB02 SEM */
    SETLO(A, rd8(ABS(0x03BA)));  /* 00CB03 LDA */
    if ((A & 0xFF) != 0) goto L_00CB0D;  /* 00CB06 BNE */
    mflag = 0;  /* 00CB08 CLM */
    S -= 3; F_00BB70();  /* 00CB09 JSRL */
L_00CB0D: TICK();
    mflag = 0;  /* 00CB0D CLM */
    A = rd16(ABS(0x03BA));  /* 00CB0E LDA */
    mflag = 1;  /* 00CB11 SEM */
    /* 00CB12 CMP */
    if ((A & 0xFF) != 0xC0) goto L_00CB1E;  /* 00CB14 BNE */
    mflag = 0;  /* 00CB16 CLM */
    S -= 3; F_00C7C9();  /* 00CB17 JSRL */
    goto L_00CB4E;  /* 00CB1B JMP */
L_00CB1E: TICK();
    mflag = 0;  /* 00CB1E CLM */
    A = rd16(ABS(0x03BA));  /* 00CB1F LDA */
    mflag = 1;  /* 00CB22 SEM */
    /* 00CB23 CMP */
    if ((A & 0xFF) != 0x80) goto L_00CB2F;  /* 00CB25 BNE */
    mflag = 0;  /* 00CB27 CLM */
    S -= 3; F_00C768();  /* 00CB28 JSRL */
    goto L_00CB4E;  /* 00CB2C JMP */
L_00CB2F: TICK();
    mflag = 0;  /* 00CB2F CLM */
    A = rd16(ABS(0x03BA));  /* 00CB30 LDA */
    mflag = 1;  /* 00CB33 SEM */
    /* 00CB34 CMP */
    if ((A & 0xFF) != 0x40) goto L_00CB40;  /* 00CB36 BNE */
    mflag = 0;  /* 00CB38 CLM */
    S -= 3; F_00C6FA();  /* 00CB39 JSRL */
    goto L_00CB4E;  /* 00CB3D JMP */
L_00CB40: TICK();
    mflag = 0;  /* 00CB40 CLM */
    A = rd16(ABS(0x03BA));  /* 00CB41 LDA */
    mflag = 1;  /* 00CB44 SEM */
    /* 00CB45 CMP */
    if ((A & 0xFF) != 0x41) goto L_00CB4E;  /* 00CB47 BNE */
    mflag = 0;  /* 00CB49 CLM */
    S -= 3; F_00C55E();  /* 00CB4A JSRL */
L_00CB4E: TICK();
    mflag = 0;  /* 00CB4E CLM */
    Y = pull16();  /* 00CB4F PLY */
    D = pull16();  /* 00CB50 PLD */
    S += 3; return;  /* 00CB51 RTL */
}

/* 00CB52  callers: 00508E, 005282, 005397, 0095F3, 00E5D7, 00E690, 00EA9B, 00EBD3 ... */
void F_00CB52(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00CB52 LDA */
    push16(D);  /* 00CB54 PHD */
    D = A;  /* 00CB55 TAD */
    C = 0;  /* 00CB56 CLC */
    A = adc16(A, 0x0009);  /* 00CB57 ADC */
    push16(A);  /* 00CB5A PHA */
    mflag = 1;  /* 00CB5B SEM */
    if ((rd8(ABS(0x023E)) & 0x80) == 0) goto L_00CBB6;  /* 00CB5C BBC */
    mflag = 0;  /* 00CB61 CLM */
    if ((rd16(ABS(0x0244)) & 0x0400) == 0) goto L_00CB80;  /* 00CB62 BBC */
    if ((rd16(ABS(0x0240)) & 0x0001) == 0x0001) goto L_00CB7D;  /* 00CB68 BBS */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0001);  /* 00CB6E SEB */
    wr16(ABS(0x0296), rd16(ABS(0x0296)) & ~0x0080);  /* 00CB73 CLB */
    wr16(ABS(0x0296), rd16(ABS(0x0296)) & ~0x000F);  /* 00CB78 CLB */
L_00CB7D: TICK();
    goto L_00CB9F;  /* 00CB7D JMP */
L_00CB80: TICK();
    mflag = 0;  /* 00CB80 CLM */
    if ((rd16(ABS(0x0240)) & 0x0001) == 0x0001) goto L_00CB9F;  /* 00CB81 BBS */
    wr16(ABS(0x0296), rd16(ABS(0x0296)) | 0x0080);  /* 00CB87 SEB */
    A = rd16(ABS(0x0298));  /* 00CB8C LDA */
    A &= 0x000F;  /* 00CB8F AND */
    mflag = 1;  /* 00CB92 SEM */
    SETLO(A, A & 0x0F);  /* 00CB93 AND */
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x0F);  /* 00CB95 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 00CB99 ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 00CB9C STA */
L_00CB9F: TICK();
    mflag = 0;  /* 00CB9F CLM */
    A = rd16(ABS(0x0296));  /* 00CBA0 LDA */
    A &= 0x00FF;  /* 00CBA3 AND */
    wr16(DP(0x05), A);  /* 00CBA6 STA */
    wr16(DP(0x07), 0x0010);  /* 00CBA8 LDM */
    Y = 0x0002;  /* 00CBAC LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00CBAF JSRL */
    goto L_00CDE4;  /* 00CBB3 JMP */
L_00CBB6: TICK();
    mflag = 1;  /* 00CBB6 SEM */
    if ((rd8(ABS(0x0240)) & 0x01) == 0) goto L_00CBC1;  /* 00CBB7 BBC */
    SETLO(A, 0x01);  /* 00CBBC LDA */
    goto L_00CBC3;  /* 00CBBE JMP */
L_00CBC1: TICK();
    SETLO(A, 0x00);  /* 00CBC1 LDA */
L_00CBC3: TICK();
    wr8(DP(0x01), (uint8_t)A);  /* 00CBC3 STA */
    wr8(DP(0x00), 0x00);  /* 00CBC5 LDM */
    SETLO(A, rd8(ABS(0x0365)));  /* 00CBC8 LDA */
    SETLO(A, A + 1);  /* 00CBCB INC */
    if ((A & 0xFF) == 0) goto L_00CBDB;  /* 00CBCC BEQ */
    mflag = 0;  /* 00CBCE CLM */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_00CBDB;  /* 00CBCF BBC */
    if ((rd16(ABS(0x023E)) & 0x0008) == 0x0008) goto L_00CBE2;  /* 00CBD5 BBS */
L_00CBDB: TICK();
    mflag = 1;  /* 00CBDB SEM */
    wr8(DP(0x00), 0x01);  /* 00CBDC LDM */
    goto L_00CC44;  /* 00CBDF JMP */
L_00CBE2: TICK();
    mflag = 0;  /* 00CBE2 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_00CBFC;  /* 00CBE3 BBS */
    if ((rd16(ABS(0x023E)) & 0x0800) == 0x0800) goto L_00CBF5;  /* 00CBE9 BBS */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_00CBF9;  /* 00CBEF BBC */
L_00CBF5: TICK();
    mflag = 1;  /* 00CBF5 SEM */
    wr8(DP(0x00), 0x01);  /* 00CBF6 LDM */
L_00CBF9: TICK();
    goto L_00CC44;  /* 00CBF9 JMP */
L_00CBFC: TICK();
    mflag = 0;  /* 00CBFC CLM */
    A = rd16(ABS(0x0229));  /* 00CBFD LDA */
    mflag = 1;  /* 00CC00 SEM */
    /* 00CC01 CMP */
    if ((A & 0xFF) != 0x01) goto L_00CC0B;  /* 00CC03 BNE */
    wr8(DP(0x00), 0x01);  /* 00CC05 LDM */
    goto L_00CC44;  /* 00CC08 JMP */
L_00CC0B: TICK();
    mflag = 0;  /* 00CC0B CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0x2000) goto L_00CC44;  /* 00CC0C BBS */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0x0020) goto L_00CC1F;  /* 00CC12 BBS */
    mflag = 1;  /* 00CC18 SEM */
    wr8(DP(0x00), 0x01);  /* 00CC19 LDM */
    goto L_00CC44;  /* 00CC1C JMP */
L_00CC1F: TICK();
    mflag = 0;  /* 00CC1F CLM */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0) goto L_00CC2C;  /* 00CC20 BBC */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0x0200) goto L_00CC40;  /* 00CC26 BBS */
L_00CC2C: TICK();
    A = rd16(ABS(0x0375));  /* 00CC2C LDA */
    mflag = 1;  /* 00CC2F SEM */
    SETLO(A, A & 0x3F);  /* 00CC30 AND */
    if ((A & 0xFF) == 0) goto L_00CC44;  /* 00CC32 BEQ */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_00CC44;  /* 00CC34 BBC */
    mflag = 0;  /* 00CC39 CLM */
    if ((rd16(ABS(0x023A)) & 0x1000) == 0x1000) goto L_00CC44;  /* 00CC3A BBS */
L_00CC40: TICK();
    mflag = 1;  /* 00CC40 SEM */
    wr8(DP(0x00), 0x01);  /* 00CC41 LDM */
L_00CC44: TICK();
    mflag = 0;  /* 00CC44 CLM */
    A = rd16(DP(0x00));  /* 00CC45 LDA */
    mflag = 1;  /* 00CC47 SEM */
    t = rd8(DP(0x01));  /* 00CC48 CMP */
    if ((A & 0xFF) == t) goto L_00CC71;  /* 00CC4A BEQ */
    SETLO(A, rd8(DP(0x00)));  /* 00CC4C LDA */
    if ((A & 0xFF) == 0) goto L_00CC59;  /* 00CC4E BEQ */
    mflag = 0;  /* 00CC50 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0001);  /* 00CC51 SEB */
    goto L_00CC5F;  /* 00CC56 JMP */
L_00CC59: TICK();
    mflag = 0;  /* 00CC59 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0001);  /* 00CC5A CLB */
L_00CC5F: TICK();
    mflag = 1;  /* 00CC5F SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00CC71;  /* 00CC60 BBS */
    mflag = 0;  /* 00CC65 CLM */
    A = D;  /* 00CC66 TDA */
    C = 0;  /* 00CC67 CLC */
    A = adc16(A, 0x0005);  /* 00CC68 ADC */
    push16(A);  /* 00CC6B PHA */
    S -= 3; cbus_write_sel2_00755E();  /* 00CC6C JSRL */
    Y = pull16();  /* 00CC70 PLY */
L_00CC71: TICK();
    mflag = 1;  /* 00CC71 SEM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_00CC86;  /* 00CC72 BBC */
    mflag = 0;  /* 00CC77 CLM */
    X = rd16(ABS(0x0298));  /* 00CC78 LDX */
    wr16(DP(0x03), X);  /* 00CC7B STX */
    A = rd16(ABS(0x0200));  /* 00CC7D LDA */
    mflag = 1;  /* 00CC80 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 00CC81 STA */
    goto L_00CC92;  /* 00CC83 JMP */
L_00CC86: TICK();
    mflag = 0;  /* 00CC86 CLM */
    X = rd16(ABS(0x029A));  /* 00CC87 LDX */
    wr16(DP(0x03), X);  /* 00CC8A STX */
    A = rd16(ABS(0x0201));  /* 00CC8C LDA */
    mflag = 1;  /* 00CC8F SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 00CC90 STA */
L_00CC92: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00CC92 LDA */
    if ((A & 0xFF) == 0) goto L_00CCA5;  /* 00CC94 BEQ */
    if ((rd8(ABS(0x023E)) & 0x20) == 0x20) goto L_00CCA5;  /* 00CC96 BBS */
    mflag = 0;  /* 00CC9B CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0) goto L_00CCA5;  /* 00CC9C BBC */
    goto L_00CD3F;  /* 00CCA2 JMP */
L_00CCA5: TICK();
    mflag = 0;  /* 00CCA5 CLM */
    if ((rd16(ABS(0x023E)) & 0x1000) == 0) goto L_00CCB0;  /* 00CCA6 BBC */
    mflag = 1;  /* 00CCAC SEM */
    wr8(DP(0x02), rd8(DP(0x02)) | 0x40);  /* 00CCAD SEB */
L_00CCB0: TICK();
    mflag = 1;  /* 00CCB0 SEM */
    if ((rd8(ABS(0x0334)) & 0x08) == 0x08) goto L_00CCD0;  /* 00CCB1 BBS */
    mflag = 0;  /* 00CCB6 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_00CCC3;  /* 00CCB7 BBS */
    mflag = 1;  /* 00CCBD SEM */
    if ((rd8(ABS(0x023C)) & 0x04) == 0x04) goto L_00CCD0;  /* 00CCBE BBS */
L_00CCC3: TICK();
    mflag = 1;  /* 00CCC3 SEM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_00CCD5;  /* 00CCC4 BBC */
    mflag = 0;  /* 00CCC9 CLM */
    if ((rd16(ABS(0x023E)) & 0x0400) == 0) goto L_00CCD5;  /* 00CCCA BBC */
L_00CCD0: TICK();
    mflag = 0;  /* 00CCD0 CLM */
    wr16(DP(0x03), rd16(DP(0x03)) | 0x0800);  /* 00CCD1 SEB */
L_00CCD5: TICK();
    mflag = 0;  /* 00CCD5 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_00CD32;  /* 00CCD6 BBS */
    mflag = 1;  /* 00CCDC SEM */
    if ((rd8(ABS(0x034E)) & 0x10) == 0) goto L_00CCFC;  /* 00CCDD BBC */
    mflag = 0;  /* 00CCE2 CLM */
    if ((rd16(ABS(0x0244)) & 0x0020) == 0) goto L_00CCFC;  /* 00CCE3 BBC */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0x000F);  /* 00CCE9 CLB */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0x0800);  /* 00CCED CLB */
    wr16(DP(0x03), rd16(DP(0x03)) | 0x8000);  /* 00CCF1 SEB */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0x4000);  /* 00CCF5 CLB */
    goto L_00CD2F;  /* 00CCF9 JMP */
L_00CCFC: TICK();
    mflag = 0;  /* 00CCFC CLM */
    if ((rd16(ABS(0x023E)) & 0x0800) == 0x0800) goto L_00CD2F;  /* 00CCFD BBS */
    mflag = 1;  /* 00CD03 SEM */
    if ((rd8(ABS(0x0244)) & 0x01) == 0x01) goto L_00CD20;  /* 00CD04 BBS */
    if ((rd8(ABS(0x0244)) & 0x02) == 0x02) goto L_00CD20;  /* 00CD09 BBS */
    if ((rd8(ABS(0x023A)) & 0x08) == 0x08) goto L_00CD20;  /* 00CD0E BBS */
    mflag = 0;  /* 00CD13 CLM */
    if ((rd16(ABS(0x023C)) & 0x0200) == 0x0200) goto L_00CD20;  /* 00CD14 BBS */
    mflag = 1;  /* 00CD1A SEM */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00CD2F;  /* 00CD1B BBC */
L_00CD20: TICK();
    mflag = 0;  /* 00CD20 CLM */
    wr16(DP(0x03), rd16(DP(0x03)) | 0x8000);  /* 00CD21 SEB */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0x0800) goto L_00CD2F;  /* 00CD25 BBS */
    wr16(DP(0x03), rd16(DP(0x03)) | 0x0080);  /* 00CD2B SEB */
L_00CD2F: TICK();
    goto L_00CD3C;  /* 00CD2F JMP */
L_00CD32: TICK();
    mflag = 1;  /* 00CD32 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00CD33 LDA */
    if ((A & 0xFF) != 0) goto L_00CD3C;  /* 00CD35 BNE */
    mflag = 0;  /* 00CD37 CLM */
    wr16(DP(0x03), rd16(DP(0x03)) | 0x0080);  /* 00CD38 SEB */
L_00CD3C: TICK();
    goto L_00CD76;  /* 00CD3C JMP */
L_00CD3F: TICK();
    mflag = 1;  /* 00CD3F SEM */
    wr8(DP(0x02), 0x00);  /* 00CD40 LDM */
    mflag = 0;  /* 00CD43 CLM */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0x0040);  /* 00CD44 CLB */
    if ((rd16(ABS(0x023E)) & 0x0200) == 0) goto L_00CD66;  /* 00CD48 BBC */
    A = rd16(ABS(0x029A));  /* 00CD4E LDA */
    A &= 0x000F;  /* 00CD51 AND */
    mflag = 1;  /* 00CD54 SEM */
    SETLO(A, A & 0x0F);  /* 00CD55 AND */
    wr8(DP(0x03), rd8(DP(0x03)) & ~0x0F);  /* 00CD57 CLB */
    SETLO(A, A | rd8(DP(0x03)));  /* 00CD5A ORA */
    wr8(DP(0x03), (uint8_t)A);  /* 00CD5C STA */
    mflag = 0;  /* 00CD5E CLM */
    wr16(DP(0x03), rd16(DP(0x03)) | 0x0010);  /* 00CD5F SEB */
    goto L_00CD76;  /* 00CD63 JMP */
L_00CD66: TICK();
    mflag = 1;  /* 00CD66 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00CD76;  /* 00CD67 BBS */
    SETLO(A, rd8(ABS(0x0229)));  /* 00CD6C LDA */
    /* 00CD6F CMP */
    if ((A & 0xFF) == 0x03) goto L_00CD76;  /* 00CD71 BEQ */
    wr8(DP(0x02), rd8(DP(0x02)) | 0x20);  /* 00CD73 SEB */
L_00CD76: TICK();
    mflag = 0;  /* 00CD76 CLM */
    A = rd16(DP(0x02));  /* 00CD77 LDA */
    mflag = 1;  /* 00CD79 SEM */
    t = rd8(ABS(0x0276));  /* 00CD7A CMP */
    if ((A & 0xFF) == t) goto L_00CD95;  /* 00CD7D BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 00CD7F LDA */
    wr8(ABS(0x0276), (uint8_t)A);  /* 00CD81 STA */
    mflag = 0;  /* 00CD84 CLM */
    A &= 0x00FF;  /* 00CD85 AND */
    wr16(DP(0x05), A);  /* 00CD88 STA */
    wr16(DP(0x07), 0x0013);  /* 00CD8A LDM */
    Y = 0x0002;  /* 00CD8E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00CD91 JSRL */
L_00CD95: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00CDB2;  /* 00CD95 BBS */
    if ((rd8(DP(0x02)) & 0x20) == 0) goto L_00CDA3;  /* 00CD9A BBC */
    SETLO(A, 0x00);  /* 00CD9E LDA */
    goto L_00CDA5;  /* 00CDA0 JMP */
L_00CDA3: TICK();
    SETLO(A, 0x01);  /* 00CDA3 LDA */
L_00CDA5: TICK();
    SETLO(A, lsr8(A));  /* 00CDA5 LSR */
    SETLO(A, rd8(ABS(P7)));  /* 00CDA6 LDA */
    SETLO(A, A & 0xF7);  /* 00CDA9 AND */
    if (!C) goto L_00CDAF;  /* 00CDAB BCC */
    SETLO(A, A | 0x08);  /* 00CDAD ORA */
L_00CDAF: TICK();
    wr8(ABS(P7), (uint8_t)A);  /* 00CDAF STA */
L_00CDB2: TICK();
    mflag = 0;  /* 00CDB2 CLM */
    X = rd16(ABS(0x0296));  /* 00CDB3 LDX */
    t = rd16(DP(0x03));  /* 00CDB6 CPX */
    if (X == t) goto L_00CDE4;  /* 00CDB8 BEQ */
    X = rd16(DP(0x03));  /* 00CDBA LDX */
    wr16(ABS(0x0296), X);  /* 00CDBC STX */
    A = rd16(DP(0x04));  /* 00CDBF LDA */
    A &= 0x00FF;  /* 00CDC1 AND */
    wr16(DP(0x05), A);  /* 00CDC4 STA */
    wr16(DP(0x07), 0x0011);  /* 00CDC6 LDM */
    Y = 0x0002;  /* 00CDCA LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00CDCD JSRL */
    SETLO(A, rd8(DP(0x03)));  /* 00CDD1 LDA */
    mflag = 0;  /* 00CDD3 CLM */
    A &= 0x00FF;  /* 00CDD4 AND */
    wr16(DP(0x05), A);  /* 00CDD7 STA */
    wr16(DP(0x07), 0x0010);  /* 00CDD9 LDM */
    Y = 0x0002;  /* 00CDDD LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00CDE0 JSRL */
L_00CDE4: TICK();
    mflag = 0;  /* 00CDE4 CLM */
    Y = pull16();  /* 00CDE5 PLY */
    D = pull16();  /* 00CDE6 PLD */
    S += 3; return;  /* 00CDE7 RTL */
}

/* 00CDE8  callers: 00E369, 00ED45, 00EEBA */
void F_00CDE8(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00CDE8 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00CDE9 CLB */
    mflag = 0;  /* 00CDED CLM */
    S += 3; return;  /* 00CDEE RTL */
}

/* 00CDEF  callers: 00CE8C, 00CECC */
void F_00CDEF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00CDEF LDA */
    push16(D);  /* 00CDF1 PHD */
    D = A;  /* 00CDF2 TAD */
    wr16(DP(0x01), Y);  /* 00CDF3 STY */
    mflag = 1;  /* 00CDF5 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00CDF6 LDA */
    if ((A & 0x80) == 0) goto L_00CE0D;  /* 00CDF8 BPL */
    mflag = 0;  /* 00CDFA CLM */
    A &= 0x00FF;  /* 00CDFB AND */
    /* 00CDFE CMP */
    if (A < 0x0080) goto L_00CE06;  /* 00CE01 BCC */
    A |= 0xFF00;  /* 00CE03 ORA */
L_00CE06: TICK();
    A ^= 0xFFFF;  /* 00CE06 EOR */
    A += 1;  /* 00CE09 INC */
    goto L_00CE1B;  /* 00CE0A JMP */
L_00CE0D: TICK();
    mflag = 0;  /* 00CE0D CLM */
    A = rd16(DP(0x00));  /* 00CE0E LDA */
    A &= 0x00FF;  /* 00CE10 AND */
    /* 00CE13 CMP */
    if (A < 0x0080) goto L_00CE1B;  /* 00CE16 BCC */
    A |= 0xFF00;  /* 00CE18 ORA */
L_00CE1B: TICK();
    mpy16(rd16(ABS(0x0220)));  /* 00CE1B MPY */
    B = 0x0000;  /* 00CE1F LDA */
    wr16(DP(0x02), A);  /* 00CE23 STA */
    div16(0x03E8);  /* 00CE25 DIV */
    C = (B >= 0x01F4);  /* 00CE29 CMP */
    if (!C) goto L_00CE36;  /* 00CE2D BCC */
    A = rd16(DP(0x02));  /* 00CE2F LDA */
    A = adc16(A, 0x03E7);  /* 00CE31 ADC */
    wr16(DP(0x02), A);  /* 00CE34 STA */
L_00CE36: TICK();
    B = 0x0000;  /* 00CE36 LDA */
    A = rd16(DP(0x02));  /* 00CE3A LDA */
    div16(0x03E8);  /* 00CE3C DIV */
    wr16(DP(0x02), A);  /* 00CE40 STA */
    mflag = 1;  /* 00CE42 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00CE43 LDA */
    if ((A & 0x80) == 0) goto L_00CE65;  /* 00CE45 BPL */
    SETLO(A, Y);  /* 00CE47 TYA */
    mflag = 0;  /* 00CE48 CLM */
    A &= 0x00FF;  /* 00CE49 AND */
    t = rd16(DP(0x02));  /* 00CE4C CMP */
    if (A >= t) goto L_00CE57;  /* 00CE4E BCS */
    wr16(DP(0x02), 0x0000);  /* 00CE50 LDM */
    goto L_00CE62;  /* 00CE54 JMP */
L_00CE57: TICK();
    mflag = 0;  /* 00CE57 CLM */
    A = rd16(DP(0x01));  /* 00CE58 LDA */
    A &= 0x00FF;  /* 00CE5A AND */
    C = 1;  /* 00CE5D SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 00CE5E SBC */
    wr16(DP(0x02), A);  /* 00CE60 STA */
L_00CE62: TICK();
    goto L_00CE79;  /* 00CE62 JMP */
L_00CE65: TICK();
    mflag = 0;  /* 00CE65 CLM */
    A = rd16(DP(0x01));  /* 00CE66 LDA */
    A &= 0x00FF;  /* 00CE68 AND */
    C = 0;  /* 00CE6B CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00CE6C ADC */
    wr16(DP(0x02), A);  /* 00CE6E STA */
    /* 00CE70 CMP */
    if (A < 0x0020) goto L_00CE79;  /* 00CE73 BCC */
    wr16(DP(0x02), 0x001F);  /* 00CE75 LDM */
L_00CE79: TICK();
    A = rd16(DP(0x02));  /* 00CE79 LDA */
    mflag = 1;  /* 00CE7B SEM */
    D = pull16();  /* 00CE7C PLD */
    S += 3; return;  /* 00CE7D RTL */
}

/* 00CE7E  callers: 00D2EE */
void F_00CE7E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00CE7E LDA */
    push16(D);  /* 00CE80 PHD */
    D = A;  /* 00CE81 TAD */
    A += 1;  /* 00CE82 INC */
    push16(A);  /* 00CE83 PHA */
    A = rd16(ABS(0x0204));  /* 00CE84 LDA */
    wr16(DP(0x01), A);  /* 00CE87 STA */
    Y = rd16(ABS(0x01FE));  /* 00CE89 LDY */
    S -= 3; F_00CDEF();  /* 00CE8C JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00CE90 STA */
    SETLO(A, rd8(ABS(0x037C)));  /* 00CE92 LDA */
    SETLO(A, A & 0x3F);  /* 00CE95 AND */
    if ((A & 0xFF) == 0) goto L_00CEC3;  /* 00CE97 BEQ */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_00CEAE;  /* 00CE99 BBC */
    SETLO(A, rd8(DP(0x00)));  /* 00CE9E LDA */
    /* 00CEA0 CMP */
    if ((A & 0xFF) < 0x04) goto L_00CEA9;  /* 00CEA2 BCC */
    SETLO(A, 0x03);  /* 00CEA4 LDA */
    goto L_00CEAB;  /* 00CEA6 JMP */
L_00CEA9: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00CEA9 LDA */
L_00CEAB: TICK();
    goto L_00CEBB;  /* 00CEAB JMP */
L_00CEAE: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00CEAE LDA */
    /* 00CEB0 CMP */
    if ((A & 0xFF) < 0x06) goto L_00CEB9;  /* 00CEB2 BCC */
    SETLO(A, 0x05);  /* 00CEB4 LDA */
    goto L_00CEBB;  /* 00CEB6 JMP */
L_00CEB9: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00CEB9 LDA */
L_00CEBB: TICK();
    SETLO(A, A ^ 0xFF);  /* 00CEBB EOR */
    SETLO(A, A + 1);  /* 00CEBD INC */
    C = 0;  /* 00CEBE CLC */
    SETLO(A, adc8(A, rd8(DP(0x00))));  /* 00CEBF ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00CEC1 STA */
L_00CEC3: TICK();
    SETLO(A, rd8(ABS(0x0205)));  /* 00CEC3 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 00CEC6 STA */
    Y = rd16(ABS(0x01FF));  /* 00CEC8 LDY */
    mflag = 0;  /* 00CECB CLM */
    S -= 3; F_00CDEF();  /* 00CECC JSRL */
    mflag = 0;  /* 00CED0 CLM */
    A &= 0x00FF;  /* 00CED1 AND */
    wr16(DP(0x01), A);  /* 00CED4 STA */
    A = rd16(DP(0x00));  /* 00CED6 LDA */
    A &= 0x00FF;  /* 00CED8 AND */
    wr16(DP(0x03), A);  /* 00CEDB STA */
    wr16(DP(0x05), 0x0012);  /* 00CEDD LDM */
    Y = 0x0003;  /* 00CEE1 LDY */
    A = D;  /* 00CEE4 TDA */
    C = 0;  /* 00CEE5 CLC */
    A = adc16(A, 0x0007);  /* 00CEE6 ADC */
    push16(A);  /* 00CEE9 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00CEEA JSRL */
    Y = pull16();  /* 00CEEE PLY */
    mflag = 0;  /* 00CEEF CLM */
    Y = pull16();  /* 00CEF0 PLY */
    D = pull16();  /* 00CEF1 PLD */
    S += 3; return;  /* 00CEF2 RTL */
}

/* 00CEF3  callers: 00CFEB, 00D166 */
void F_00CEF3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00CEF3 LDA */
    push16(D);  /* 00CEF5 PHD */
    D = A;  /* 00CEF6 TAD */
    if ((rd16(ABS(0x023C)) & 0x2000) == 0) goto L_00CF04;  /* 00CEF7 BBC */
    mflag = 1;  /* 00CEFD SEM */
    wr8(DP(0x00), 0x02);  /* 00CEFE LDM */
    goto L_00CF2F;  /* 00CF01 JMP */
L_00CF04: TICK();
    mflag = 0;  /* 00CF04 CLM */
    if ((rd16(ABS(0x0240)) & 0x0040) == 0) goto L_00CF1F;  /* 00CF05 BBC */
    if ((rd16(ABS(0x023C)) & 0x0010) == 0) goto L_00CF18;  /* 00CF0B BBC */
    mflag = 1;  /* 00CF11 SEM */
    wr8(DP(0x00), 0x01);  /* 00CF12 LDM */
    goto L_00CF1C;  /* 00CF15 JMP */
L_00CF18: TICK();
    mflag = 1;  /* 00CF18 SEM */
    wr8(DP(0x00), 0x00);  /* 00CF19 LDM */
L_00CF1C: TICK();
    goto L_00CF2F;  /* 00CF1C JMP */
L_00CF1F: TICK();
    mflag = 1;  /* 00CF1F SEM */
    if ((rd8(ABS(0x037C)) & 0x80) == 0) goto L_00CF2B;  /* 00CF20 BBC */
    wr8(DP(0x00), 0x01);  /* 00CF25 LDM */
    goto L_00CF2F;  /* 00CF28 JMP */
L_00CF2B: TICK();
    mflag = 1;  /* 00CF2B SEM */
    wr8(DP(0x00), 0x00);  /* 00CF2C LDM */
L_00CF2F: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00CF2F LDA */
    D = pull16();  /* 00CF31 PLD */
    S += 3; return;  /* 00CF32 RTL */
}

/* 00CF33  callers: 00D032, 00D17F, 00D19B */
void F_00CF33(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00CF33 LDA */
    push16(D);  /* 00CF35 PHD */
    D = A;  /* 00CF36 TAD */
    wr16(DP(0x01), Y);  /* 00CF37 STY */
    mflag = 1;  /* 00CF39 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00CF3A LDA */
    if ((A & 0x80) == 0) goto L_00CF51;  /* 00CF3C BPL */
    mflag = 0;  /* 00CF3E CLM */
    A &= 0x00FF;  /* 00CF3F AND */
    /* 00CF42 CMP */
    if (A < 0x0080) goto L_00CF4A;  /* 00CF45 BCC */
    A |= 0xFF00;  /* 00CF47 ORA */
L_00CF4A: TICK();
    A ^= 0xFFFF;  /* 00CF4A EOR */
    A += 1;  /* 00CF4D INC */
    goto L_00CF5F;  /* 00CF4E JMP */
L_00CF51: TICK();
    mflag = 0;  /* 00CF51 CLM */
    A = rd16(DP(0x00));  /* 00CF52 LDA */
    A &= 0x00FF;  /* 00CF54 AND */
    /* 00CF57 CMP */
    if (A < 0x0080) goto L_00CF5F;  /* 00CF5A BCC */
    A |= 0xFF00;  /* 00CF5C ORA */
L_00CF5F: TICK();
    mpy16(rd16(ABS(0x0220)));  /* 00CF5F MPY */
    B = 0x0000;  /* 00CF63 LDA */
    wr16(DP(0x02), A);  /* 00CF67 STA */
    div16(0x03E8);  /* 00CF69 DIV */
    C = (B >= 0x01F4);  /* 00CF6D CMP */
    if (!C) goto L_00CF7A;  /* 00CF71 BCC */
    A = rd16(DP(0x02));  /* 00CF73 LDA */
    A = adc16(A, 0x03E7);  /* 00CF75 ADC */
    wr16(DP(0x02), A);  /* 00CF78 STA */
L_00CF7A: TICK();
    B = 0x0000;  /* 00CF7A LDA */
    A = rd16(DP(0x02));  /* 00CF7E LDA */
    div16(0x03E8);  /* 00CF80 DIV */
    wr16(DP(0x02), A);  /* 00CF84 STA */
    mflag = 1;  /* 00CF86 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00CF87 LDA */
    if ((A & 0x80) == 0) goto L_00CFAB;  /* 00CF89 BPL */
    SETLO(A, Y);  /* 00CF8B TYA */
    mflag = 0;  /* 00CF8C CLM */
    A &= 0x00FF;  /* 00CF8D AND */
    t = rd16(DP(0x02));  /* 00CF90 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_00CF96;  /* 00CF92 BCC */
    if (!Z) goto L_00CF9D;  /* 00CF94 BNE */
L_00CF96: TICK();
    wr16(DP(0x02), 0x0000);  /* 00CF96 LDM */
    goto L_00CFA8;  /* 00CF9A JMP */
L_00CF9D: TICK();
    mflag = 0;  /* 00CF9D CLM */
    A = rd16(DP(0x01));  /* 00CF9E LDA */
    A &= 0x00FF;  /* 00CFA0 AND */
    C = 1;  /* 00CFA3 SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 00CFA4 SBC */
    wr16(DP(0x02), A);  /* 00CFA6 STA */
L_00CFA8: TICK();
    goto L_00CFBF;  /* 00CFA8 JMP */
L_00CFAB: TICK();
    mflag = 0;  /* 00CFAB CLM */
    A = rd16(DP(0x01));  /* 00CFAC LDA */
    A &= 0x00FF;  /* 00CFAE AND */
    C = 0;  /* 00CFB1 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00CFB2 ADC */
    wr16(DP(0x02), A);  /* 00CFB4 STA */
    /* 00CFB6 CMP */
    if (A < 0x00FF) goto L_00CFBF;  /* 00CFB9 BCC */
    wr16(DP(0x02), 0x00FE);  /* 00CFBB LDM */
L_00CFBF: TICK();
    A = rd16(DP(0x02));  /* 00CFBF LDA */
    mflag = 1;  /* 00CFC1 SEM */
    D = pull16();  /* 00CFC2 PLD */
    S += 3; return;  /* 00CFC3 RTL */
}

/* 00CFC4  callers: 00ED72 */
void tx_power_loop_00CFC4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00CFC4 LDA */
    push16(D);  /* 00CFC6 PHD */
    D = A;  /* 00CFC7 TAD */
    C = 0;  /* 00CFC8 CLC */
    A = adc16(A, 0x0004);  /* 00CFC9 ADC */
    push16(A);  /* 00CFCC PHA */
    mflag = 1;  /* 00CFCD SEM */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_00CFD6;  /* 00CFCE BBC */
    goto L_00D165;  /* 00CFD3 JMP */
L_00CFD6: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00CFDE;  /* 00CFD6 BBC */
    goto L_00D165;  /* 00CFDB JMP */
L_00CFDE: TICK();
    wr8(ABS(TA3IC), 0x00);  /* 00CFDE LDM */
    wr8(ABS(TA3MOD), 0x27);  /* 00CFE2 LDM */
    wr8(ABS(TA3), 0x01);  /* 00CFE6 LDM */
    mflag = 0;  /* 00CFEA CLM */
    S -= 3; F_00CEF3();  /* 00CFEB JSRL */
    wr8(DP(0x03), (uint8_t)A);  /* 00CFEF STA */
    mflag = 0;  /* 00CFF1 CLM */
    A &= 0x00FF;  /* 00CFF2 AND */
    X = A;  /* 00CFF5 TAX */
    A = rd16((ABS(0x0277) + X));  /* 00CFF6 LDA */
    mflag = 1;  /* 00CFF9 SEM */
    wr8(ABS(0x020F), (uint8_t)A);  /* 00CFFA STA */
    wr8(DP(0x02), (uint8_t)A);  /* 00CFFD STA */
    wr8(ABS(TA3_H), (uint8_t)A);  /* 00CFFF STA */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00D002 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 00D006 SEB */
    wr8(ABS(TA4IC), 0x00);  /* 00D00A LDM */
    wr8(ABS(TA4MOD), 0x00);  /* 00D00E LDM */
    mflag = 0;  /* 00D012 CLM */
    wr16(ABS(TA4), 0x4EC0);  /* 00D013 LDM */
    mflag = 1;  /* 00D018 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x10);  /* 00D019 SEB */
    SETLO(A, rd8(DP(0x03)));  /* 00D01D LDA */
    mflag = 0;  /* 00D01F CLM */
    A &= 0x00FF;  /* 00D020 AND */
    X = A;  /* 00D023 TAX */
    A = rd16((ABS(0x027D) + X));  /* 00D024 LDA */
    wr16(DP(0x04), A);  /* 00D027 STA */
    A = rd16(DP(0x03));  /* 00D029 LDA */
    A &= 0x00FF;  /* 00D02B AND */
    X = A;  /* 00D02E TAX */
    Y = rd16((ABS(0x027A) + X));  /* 00D02F LDY */
    S -= 3; F_00CF33();  /* 00D032 JSRL */
    wr8(ABS(0x0210), (uint8_t)A);  /* 00D036 STA */
L_00D039: TICK();
    if ((rd8(ABS(TA4IC)) & 0x08) == 0x08) goto L_00D041;  /* 00D039 BBS */
    goto L_00D039;  /* 00D03E JMP */
L_00D041: TICK();
    mflag = 1;  /* 00D041 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x10);  /* 00D042 CLB */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 00D046 CLB */
    mflag = 0;  /* 00D04A CLM */
    wr16(ABS(TA4), 0x1F80);  /* 00D04B LDM */
    wr16(DP(0x00), 0x000A);  /* 00D050 LDM */
    mflag = 1;  /* 00D054 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x10);  /* 00D055 SEB */
L_00D059: TICK();
    Y = 0x0007;  /* 00D059 LDY */
    mflag = 0;  /* 00D05C CLM */
    S -= 3; F_008DC6();  /* 00D05D JSRL */
    mflag = 1;  /* 00D061 SEM */
    SETLO(A, rd8(ABS(AD7)));  /* 00D062 LDA [AN7 GIDEN_GUC] */
    t = rd8(ABS(0x0210));  /* 00D065 CMP */
    if ((A & 0xFF) >= t) goto L_00D0CD;  /* 00D068 BCS */
    SETLO(A, rd8(ABS(AD7)));  /* 00D06A LDA [AN7 GIDEN_GUC] */
    mflag = 0;  /* 00D06D CLM */
    A &= 0x00FF;  /* 00D06E AND */
    wr16(DP(0x04), A);  /* 00D071 STA */
    A = rd16(ABS(0x0210));  /* 00D073 LDA */
    A &= 0x00FF;  /* 00D076 AND */
    C = 1;  /* 00D079 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 00D07A SBC */
    A ^= 0x8000;  /* 00D07C EOR */
    /* 00D07F CMP */
    if (A < 0x800B) goto L_00D094;  /* 00D082 BCC */
    A = rd16(DP(0x02));  /* 00D084 LDA */
    mflag = 1;  /* 00D086 SEM */
    C = ((A & 0xFF) >= 0xFA);  /* 00D087 CMP */
    if (C) goto L_00D091;  /* 00D089 BCS */
    SETLO(A, rd8(DP(0x02)));  /* 00D08B LDA */
    SETLO(A, adc8(A, 0x05));  /* 00D08D ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 00D08F STA */
L_00D091: TICK();
    goto L_00D0CA;  /* 00D091 JMP */
L_00D094: TICK();
    mflag = 1;  /* 00D094 SEM */
    SETLO(A, rd8(ABS(AD7)));  /* 00D095 LDA [AN7 GIDEN_GUC] */
    mflag = 0;  /* 00D098 CLM */
    A &= 0x00FF;  /* 00D099 AND */
    wr16(DP(0x04), A);  /* 00D09C STA */
    A = rd16(ABS(0x0210));  /* 00D09E LDA */
    A &= 0x00FF;  /* 00D0A1 AND */
    C = 1;  /* 00D0A4 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 00D0A5 SBC */
    A ^= 0x8000;  /* 00D0A7 EOR */
    /* 00D0AA CMP */
    if (A < 0x8006) goto L_00D0C0;  /* 00D0AD BCC */
    A = rd16(DP(0x02));  /* 00D0AF LDA */
    mflag = 1;  /* 00D0B1 SEM */
    /* 00D0B2 CMP */
    if ((A & 0xFF) >= 0xFC) goto L_00D0BD;  /* 00D0B4 BCS */
    SETLO(A, rd8(DP(0x02)));  /* 00D0B6 LDA */
    SETLO(A, A + 1);  /* 00D0B8 INC */
    SETLO(A, A + 1);  /* 00D0B9 INC */
    SETLO(A, A + 1);  /* 00D0BA INC */
    wr8(DP(0x02), (uint8_t)A);  /* 00D0BB STA */
L_00D0BD: TICK();
    goto L_00D0CA;  /* 00D0BD JMP */
L_00D0C0: TICK();
    mflag = 0;  /* 00D0C0 CLM */
    A = rd16(DP(0x02));  /* 00D0C1 LDA */
    mflag = 1;  /* 00D0C3 SEM */
    /* 00D0C4 CMP */
    if ((A & 0xFF) >= 0xFE) goto L_00D0CA;  /* 00D0C6 BCS */
    t = (rd8(DP(0x02)) + 1) & 0xFF; wr8(DP(0x02), t);  /* 00D0C8 INC */
L_00D0CA: TICK();
    goto L_00D136;  /* 00D0CA JMP */
L_00D0CD: TICK();
    mflag = 0;  /* 00D0CD CLM */
    A = rd16(ABS(0x0210));  /* 00D0CE LDA */
    mflag = 1;  /* 00D0D1 SEM */
    t = rd8(ABS(AD7));  /* 00D0D2 CMP [AN7 GIDEN_GUC] */
    if ((A & 0xFF) >= t) goto L_00D136;  /* 00D0D5 BCS */
    SETLO(A, rd8(ABS(0x0210)));  /* 00D0D7 LDA */
    mflag = 0;  /* 00D0DA CLM */
    A &= 0x00FF;  /* 00D0DB AND */
    wr16(DP(0x04), A);  /* 00D0DE STA */
    mflag = 1;  /* 00D0E0 SEM */
    SETLO(A, rd8(ABS(AD7)));  /* 00D0E1 LDA [AN7 GIDEN_GUC] */
    mflag = 0;  /* 00D0E4 CLM */
    A &= 0x00FF;  /* 00D0E5 AND */
    C = 1;  /* 00D0E8 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 00D0E9 SBC */
    A ^= 0x8000;  /* 00D0EB EOR */
    /* 00D0EE CMP */
    if (A < 0x800B) goto L_00D103;  /* 00D0F1 BCC */
    A = rd16(DP(0x02));  /* 00D0F3 LDA */
    mflag = 1;  /* 00D0F5 SEM */
    C = ((A & 0xFF) >= 0x05);  /* 00D0F6 CMP */
    if (!C) goto L_00D100;  /* 00D0F8 BCC */
    SETLO(A, rd8(DP(0x02)));  /* 00D0FA LDA */
    SETLO(A, adc8(A, 0xFA));  /* 00D0FC ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 00D0FE STA */
L_00D100: TICK();
    goto L_00D136;  /* 00D100 JMP */
L_00D103: TICK();
    mflag = 0;  /* 00D103 CLM */
    A = rd16(ABS(0x0210));  /* 00D104 LDA */
    A &= 0x00FF;  /* 00D107 AND */
    wr16(DP(0x04), A);  /* 00D10A STA */
    mflag = 1;  /* 00D10C SEM */
    SETLO(A, rd8(ABS(AD7)));  /* 00D10D LDA [AN7 GIDEN_GUC] */
    mflag = 0;  /* 00D110 CLM */
    A &= 0x00FF;  /* 00D111 AND */
    C = 1;  /* 00D114 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 00D115 SBC */
    A ^= 0x8000;  /* 00D117 EOR */
    /* 00D11A CMP */
    if (A < 0x8006) goto L_00D12F;  /* 00D11D BCC */
    A = rd16(DP(0x02));  /* 00D11F LDA */
    mflag = 1;  /* 00D121 SEM */
    C = ((A & 0xFF) >= 0x03);  /* 00D122 CMP */
    if (!C) goto L_00D12C;  /* 00D124 BCC */
    SETLO(A, rd8(DP(0x02)));  /* 00D126 LDA */
    SETLO(A, adc8(A, 0xFC));  /* 00D128 ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 00D12A STA */
L_00D12C: TICK();
    goto L_00D136;  /* 00D12C JMP */
L_00D12F: TICK();
    mflag = 1;  /* 00D12F SEM */
    SETLO(A, rd8(DP(0x02)));  /* 00D130 LDA */
    if ((A & 0xFF) == 0) goto L_00D136;  /* 00D132 BEQ */
    t = (rd8(DP(0x02)) - 1) & 0xFF; wr8(DP(0x02), t);  /* 00D134 DEC */
L_00D136: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 00D136 LDA */
    wr8(ABS(TA3_H), (uint8_t)A);  /* 00D138 STA */
    wr8(ABS(0x020F), (uint8_t)A);  /* 00D13B STA */
L_00D13E: TICK();
    if ((rd8(ABS(TA4IC)) & 0x08) == 0x08) goto L_00D146;  /* 00D13E BBS */
    goto L_00D13E;  /* 00D143 JMP */
L_00D146: TICK();
    mflag = 1;  /* 00D146 SEM */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 00D147 CLB */
    mflag = 0;  /* 00D14B CLM */
    t = (rd16(DP(0x00)) - 1) & 0xFFFF; wr16(DP(0x00), t);  /* 00D14C DEC */
    X = rd16(DP(0x00));  /* 00D14E LDX */
    if (X == 0) goto L_00D155;  /* 00D150 BEQ */
    goto L_00D059;  /* 00D152 JMP */
L_00D155: TICK();
    mflag = 1;  /* 00D155 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x10);  /* 00D156 CLB */
    wr8(ABS(0x0237), 0x02);  /* 00D15A LDM */
    wr8(ABS(0x0188), 0x00);  /* 00D15E LDM */
    goto L_00D1C4;  /* 00D162 JMP */
L_00D165: TICK();
    mflag = 0;  /* 00D165 CLM */
    S -= 3; F_00CEF3();  /* 00D166 JSRL */
    wr8(DP(0x03), (uint8_t)A);  /* 00D16A STA */
    mflag = 0;  /* 00D16C CLM */
    A &= 0x00FF;  /* 00D16D AND */
    X = A;  /* 00D170 TAX */
    A = rd16((ABS(0x0185) + X));  /* 00D171 LDA */
    wr16(DP(0x04), A);  /* 00D174 STA */
    A = rd16(DP(0x03));  /* 00D176 LDA */
    A &= 0x00FF;  /* 00D178 AND */
    X = A;  /* 00D17B TAX */
    Y = rd16((ABS(0x0181) + X));  /* 00D17C LDY */
    S -= 3; F_00CF33();  /* 00D17F JSRL */
    wr8(ABS(0x020F), (uint8_t)A);  /* 00D183 STA */
    SETLO(A, rd8(DP(0x03)));  /* 00D186 LDA */
    mflag = 0;  /* 00D188 CLM */
    A &= 0x00FF;  /* 00D189 AND */
    X = A;  /* 00D18C TAX */
    A = rd16((ABS(0x027D) + X));  /* 00D18D LDA */
    wr16(DP(0x04), A);  /* 00D190 STA */
    A = rd16(DP(0x03));  /* 00D192 LDA */
    A &= 0x00FF;  /* 00D194 AND */
    X = A;  /* 00D197 TAX */
    Y = rd16((ABS(0x027A) + X));  /* 00D198 LDY */
    S -= 3; F_00CF33();  /* 00D19B JSRL */
    wr8(ABS(0x0210), (uint8_t)A);  /* 00D19F STA */
    wr8(ABS(TA3IC), 0x00);  /* 00D1A2 LDM */
    wr8(ABS(TA3MOD), 0x27);  /* 00D1A6 LDM */
    wr8(ABS(TA3), 0x01);  /* 00D1AA LDM */
    SETLO(A, rd8(ABS(0x020F)));  /* 00D1AE LDA */
    wr8(ABS(TA3_H), (uint8_t)A);  /* 00D1B1 STA */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00D1B4 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 00D1B8 SEB */
    wr8(ABS(0x0237), 0x02);  /* 00D1BC LDM */
    wr8(ABS(0x0188), 0x00);  /* 00D1C0 LDM */
L_00D1C4: TICK();
    mflag = 0;  /* 00D1C4 CLM */
    Y = pull16();  /* 00D1C5 PLY */
    D = pull16();  /* 00D1C6 PLD */
    S += 3; return;  /* 00D1C7 RTL */
}

/* 00D1C8  callers: 00E63F */
void tx_power_protect_00D1C8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D1C8 LDA */
    push16(D);  /* 00D1CA PHD */
    D = A;  /* 00D1CB TAD */
    A += 1;  /* 00D1CC INC */
    A += 1;  /* 00D1CD INC */
    A += 1;  /* 00D1CE INC */
    push16(A);  /* 00D1CF PHA */
    A = rd16(ABS(0x020F));  /* 00D1D0 LDA */
    mflag = 1;  /* 00D1D3 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00D1D4 STA */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00D22A;  /* 00D1D6 BBS */
    if ((rd8(ABS(0x0335)) & 0x04) == 0x04) goto L_00D227;  /* 00D1DB BBS */
    mflag = 0;  /* 00D1E0 CLM */
    if ((rd16(ABS(0x023C)) & 0x2000) == 0x2000) goto L_00D227;  /* 00D1E1 BBS */
    Y = 0x0007;  /* 00D1E7 LDY */
    S -= 3; F_008DC6();  /* 00D1EA JSRL */
    A = rd16(ABS(0x0210));  /* 00D1EE LDA */
    mflag = 1;  /* 00D1F1 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 00D1F2 STA */
    /* 00D1F4 CMP */
    if ((A & 0xFF) >= 0x03) goto L_00D1FE;  /* 00D1F6 BCS */
    wr8(DP(0x02), 0x00);  /* 00D1F8 LDM */
    goto L_00D206;  /* 00D1FB JMP */
L_00D1FE: TICK();
    mflag = 0;  /* 00D1FE CLM */
    A = rd16(DP(0x01));  /* 00D1FF LDA */
    mflag = 1;  /* 00D201 SEM */
    SETLO(A, A - 1);  /* 00D202 DEC */
    SETLO(A, A - 1);  /* 00D203 DEC */
    wr8(DP(0x02), (uint8_t)A);  /* 00D204 STA */
L_00D206: TICK();
    SETLO(A, rd8(ABS(AD7)));  /* 00D206 LDA [AN7 GIDEN_GUC] */
    t = rd8(DP(0x02));  /* 00D209 CMP */
    if ((A & 0xFF) >= t) goto L_00D218;  /* 00D20B BCS */
    SETLO(A, rd8(DP(0x00)));  /* 00D20D LDA */
    /* 00D20F CMP */
    if ((A & 0xFF) >= 0xFE) goto L_00D215;  /* 00D211 BCS */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00D213 INC */
L_00D215: TICK();
    goto L_00D227;  /* 00D215 JMP */
L_00D218: TICK();
    mflag = 0;  /* 00D218 CLM */
    A = rd16(DP(0x01));  /* 00D219 LDA */
    mflag = 1;  /* 00D21B SEM */
    t = rd8(ABS(AD7));  /* 00D21C CMP [AN7 GIDEN_GUC] */
    if ((A & 0xFF) >= t) goto L_00D227;  /* 00D21F BCS */
    SETLO(A, rd8(DP(0x00)));  /* 00D221 LDA */
    if ((A & 0xFF) == 0) goto L_00D227;  /* 00D223 BEQ */
    t = (rd8(DP(0x00)) - 1) & 0xFF; wr8(DP(0x00), t);  /* 00D225 DEC */
L_00D227: TICK();
    goto L_00D28E;  /* 00D227 JMP */
L_00D22A: TICK();
    Y = 0x0006;  /* 00D22A LDY */
    mflag = 0;  /* 00D22D CLM */
    S -= 3; F_008DC6();  /* 00D22E JSRL */
    mflag = 1;  /* 00D232 SEM */
    SETLO(A, rd8(ABS(AD6)));  /* 00D233 LDA [AN6 YANSIYAN_GUC] */
    t = rd8(ABS(0x01FD));  /* 00D236 CMP */
    if ((A & 0xFF) < t) goto L_00D257;  /* 00D239 BCC */
    wr8(ABS(0x0237), 0x04);  /* 00D23B LDM */
    SETLO(A, rd8(DP(0x00)));  /* 00D23F LDA */
    /* 00D241 CMP */
    if ((A & 0xFF) >= 0x0A) goto L_00D24B;  /* 00D243 BCS */
    wr8(DP(0x00), 0x00);  /* 00D245 LDM */
    goto L_00D254;  /* 00D248 JMP */
L_00D24B: TICK();
    mflag = 0;  /* 00D24B CLM */
    A = rd16(DP(0x00));  /* 00D24C LDA */
    C = 0;  /* 00D24E CLC */
    mflag = 1;  /* 00D24F SEM */
    SETLO(A, adc8(A, 0xF6));  /* 00D250 ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00D252 STA */
L_00D254: TICK();
    goto L_00D28E;  /* 00D254 JMP */
L_00D257: TICK();
    mflag = 1;  /* 00D257 SEM */
    SETLO(A, rd8(ABS(0x0188)));  /* 00D258 LDA */
    if ((A & 0xFF) == 0) goto L_00D260;  /* 00D25B BEQ */
    t = (rd8(ABS(0x0188)) - 1) & 0xFF; wr8(ABS(0x0188), t);  /* 00D25D DEC */
L_00D260: TICK();
    Y = 0x0003;  /* 00D260 LDY */
    mflag = 0;  /* 00D263 CLM */
    S -= 3; F_008DC6();  /* 00D264 JSRL */
    A = rd16(ABS(0x01FC));  /* 00D268 LDA */
    mflag = 1;  /* 00D26B SEM */
    t = rd8(ABS(AD3));  /* 00D26C CMP [AN3 SICAKLIK] */
    if ((A & 0xFF) < t) goto L_00D28E;  /* 00D26F BCC */
    SETLO(A, rd8(ABS(0x0188)));  /* 00D271 LDA */
    if ((A & 0xFF) != 0) goto L_00D28E;  /* 00D274 BNE */
    wr8(ABS(0x0188), 0x0A);  /* 00D276 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 00D27A LDA */
    /* 00D27C CMP */
    if ((A & 0xFF) >= 0x02) goto L_00D286;  /* 00D27E BCS */
    wr8(DP(0x00), 0x00);  /* 00D280 LDM */
    goto L_00D28E;  /* 00D283 JMP */
L_00D286: TICK();
    mflag = 0;  /* 00D286 CLM */
    A = rd16(DP(0x00));  /* 00D287 LDA */
    mflag = 1;  /* 00D289 SEM */
    SETLO(A, A - 1);  /* 00D28A DEC */
    SETLO(A, A - 1);  /* 00D28B DEC */
    wr8(DP(0x00), (uint8_t)A);  /* 00D28C STA */
L_00D28E: TICK();
    mflag = 0;  /* 00D28E CLM */
    A = rd16(DP(0x00));  /* 00D28F LDA */
    mflag = 1;  /* 00D291 SEM */
    t = rd8(ABS(0x020F));  /* 00D292 CMP */
    if ((A & 0xFF) == t) goto L_00D29F;  /* 00D295 BEQ */
    SETLO(A, rd8(DP(0x00)));  /* 00D297 LDA */
    wr8(ABS(0x020F), (uint8_t)A);  /* 00D299 STA */
    wr8(ABS(TA3_H), (uint8_t)A);  /* 00D29C STA */
L_00D29F: TICK();
    mflag = 0;  /* 00D29F CLM */
    Y = pull16();  /* 00D2A0 PLY */
    D = pull16();  /* 00D2A1 PLD */
    S += 3; return;  /* 00D2A2 RTL */
}

/* 00D2A3  callers: 00EA54, 00ED41 */
void synth_select_rx_tx_words_00D2A3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D2A3 LDA */
    push16(D);  /* 00D2A5 PHD */
    D = A;  /* 00D2A6 TAD */
    A += 1;  /* 00D2A7 INC */
    push16(A);  /* 00D2A8 PHA */
    wr16(DP(0x00), Y);  /* 00D2A9 STY */
    mflag = 1;  /* 00D2AB SEM */
    SETLO(A, Y);  /* 00D2AC TYA */
    if ((A & 0xFF) != 0) goto L_00D2D9;  /* 00D2AD BNE */
    mflag = 0;  /* 00D2AF CLM */
    X = rd16(ABS(0x0370));  /* 00D2B0 LDX */
    wr16(ABS(0x0212), X);  /* 00D2B3 STX */
    A = rd16(ABS(0x0372));  /* 00D2B6 LDA */
    mflag = 1;  /* 00D2B9 SEM */
    wr8(ABS(0x0208), (uint8_t)A);  /* 00D2BA STA */
    mflag = 0;  /* 00D2BD CLM */
    X = rd16(ABS(0x0373));  /* 00D2BE LDX */
    wr16(ABS(0x0216), X);  /* 00D2C1 STX */
    mflag = 1;  /* 00D2C4 SEM */
    if ((rd8(ABS(0x0375)) & 0x40) == 0) goto L_00D2D1;  /* 00D2C5 BBC */
    wr8(ABS(P4), rd8(ABS(P4)) | 0x08);  /* 00D2CA SEB [P43 KRISTAL_KAYDIRMA] */
    goto L_00D2D6;  /* 00D2CE JMP */
L_00D2D1: TICK();
    mflag = 1;  /* 00D2D1 SEM */
    wr8(ABS(P4), rd8(ABS(P4)) & ~0x08);  /* 00D2D2 CLB [P43 KRISTAL_KAYDIRMA] */
L_00D2D6: TICK();
    goto L_00D2F2;  /* 00D2D6 JMP */
L_00D2D9: TICK();
    mflag = 0;  /* 00D2D9 CLM */
    X = rd16(ABS(0x0377));  /* 00D2DA LDX */
    wr16(ABS(0x0212), X);  /* 00D2DD STX */
    A = rd16(ABS(0x0379));  /* 00D2E0 LDA */
    mflag = 1;  /* 00D2E3 SEM */
    wr8(ABS(0x0208), (uint8_t)A);  /* 00D2E4 STA */
    mflag = 0;  /* 00D2E7 CLM */
    X = rd16(ABS(0x037A));  /* 00D2E8 LDX */
    wr16(ABS(0x0216), X);  /* 00D2EB STX */
    S -= 3; F_00CE7E();  /* 00D2EE JSRL */
L_00D2F2: TICK();
    mflag = 0;  /* 00D2F2 CLM */
    Y = pull16();  /* 00D2F3 PLY */
    D = pull16();  /* 00D2F4 PLD */
    S += 3; return;  /* 00D2F5 RTL */
}

/* 00D2F6  callers: 00EC58, 00ECE9, 00EE27 */
void synth_check_lock_00D2F6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D2F6 LDA */
    push16(D);  /* 00D2F8 PHD */
    D = A;  /* 00D2F9 TAD */
    wr16(DP(0x00), Y);  /* 00D2FA STY */
    X = rd16(ABS(TB2));  /* 00D2FC LDX */
    wr16(DP(0x01), X);  /* 00D2FF STX */
    wr16(DP(0x03), 0x0000);  /* 00D301 LDM */
    mflag = 1;  /* 00D305 SEM */
    SETLO(A, Y);  /* 00D306 TYA */
    if ((A & 0xFF) != 0) goto L_00D30C;  /* 00D307 BNE */
    goto L_00D3B0;  /* 00D309 JMP */
L_00D30C: TICK();
    wr8(ABS(WDT), 0x00);  /* 00D30C LDM */
    mflag = 0;  /* 00D310 CLM */
    X = rd16(DP(0x01));  /* 00D311 LDX */
    /* 00D313 CPX */
    if (X >= 0x0021) goto L_00D33C;  /* 00D316 BCS */
L_00D318: TICK();
    mflag = 1;  /* 00D318 SEM */
    if ((rd8(ABS(P6)) & 0x80) == 0x80) goto L_00D322;  /* 00D319 BBS [P67 SENTEZOR_KILIT] */
    SETLO(A, 0x01);  /* 00D31E LDA */
    D = pull16();  /* 00D320 PLD */
    S += 3; return;  /* 00D321 RTL */
L_00D322: TICK();
    mflag = 0;  /* 00D322 CLM */
    X = rd16(ABS(TB2));  /* 00D323 LDX */
    t = rd16(DP(0x01));  /* 00D326 CPX */
    C = (X >= t);
    if (!C) goto L_00D318;  /* 00D328 BCC */
    A = rd16(DP(0x01));  /* 00D32A LDA */
    A = adc16(A, 0x0FBE);  /* 00D32C ADC */
    X = rd16(ABS(TB2));  /* 00D32F LDX */
    wr16(DP(0x01), X);  /* 00D332 STX */
    C = 1;  /* 00D334 SEC */
    A = sbc16(A, rd16(DP(0x01)));  /* 00D335 SBC */
    wr16(DP(0x05), A);  /* 00D337 STA */
    goto L_00D387;  /* 00D339 JMP */
L_00D33C: TICK();
    mflag = 0;  /* 00D33C CLM */
    A = rd16(DP(0x01));  /* 00D33D LDA */
    C = 0;  /* 00D33F CLC */
    A = adc16(A, 0xFFE0);  /* 00D340 ADC */
    wr16(DP(0x05), A);  /* 00D343 STA */
L_00D345: TICK();
    mflag = 1;  /* 00D345 SEM */
    if ((rd8(ABS(P6)) & 0x80) == 0x80) goto L_00D34F;  /* 00D346 BBS [P67 SENTEZOR_KILIT] */
    SETLO(A, 0x01);  /* 00D34B LDA */
    D = pull16();  /* 00D34D PLD */
    S += 3; return;  /* 00D34E RTL */
L_00D34F: TICK();
    mflag = 0;  /* 00D34F CLM */
    X = rd16(DP(0x05));  /* 00D350 LDX */
    t = rd16(ABS(TB2));  /* 00D352 CPX */
    if (X >= t) goto L_00D35E;  /* 00D355 BCS */
    X = rd16(ABS(TB2));  /* 00D357 LDX */
    t = rd16(DP(0x01));  /* 00D35A CPX */
    if (X < t) goto L_00D345;  /* 00D35C BCC */
L_00D35E: TICK();
    X = rd16(ABS(TB2));  /* 00D35E LDX */
    wr16(DP(0x01), X);  /* 00D361 STX */
    t = rd16(DP(0x05));  /* 00D363 CPX */
    C = (X >= t);
    Z = (X == t);
    if (!C) goto L_00D379;  /* 00D365 BCC */
    if (Z) goto L_00D379;  /* 00D367 BEQ */
    A = X;  /* 00D369 TXA */
    A = adc16(A, 0xF040);  /* 00D36A ADC */
    A ^= 0xFFFF;  /* 00D36D EOR */
    A += 1;  /* 00D370 INC */
    C = 0;  /* 00D371 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 00D372 ADC */
    wr16(DP(0x05), A);  /* 00D374 STA */
    goto L_00D381;  /* 00D376 JMP */
L_00D379: TICK();
    mflag = 0;  /* 00D379 CLM */
    A = rd16(DP(0x05));  /* 00D37A LDA */
    C = 1;  /* 00D37C SEC */
    A = sbc16(A, rd16(DP(0x01)));  /* 00D37D SBC */
    wr16(DP(0x05), A);  /* 00D37F STA */
L_00D381: TICK();
    C = 0;  /* 00D381 CLC */
    A = adc16(A, 0x0020);  /* 00D382 ADC */
    wr16(DP(0x05), A);  /* 00D385 STA */
L_00D387: TICK();
    A = rd16(DP(0x03));  /* 00D387 LDA */
    C = 0;  /* 00D389 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 00D38A ADC */
    wr16(DP(0x03), A);  /* 00D38C STA */
    /* 00D38E CMP */
    if (A < 0x0020) goto L_00D3A6;  /* 00D391 BCC */
    A = lsr16(A);  /* 00D393 LSR */
    A = lsr16(A);  /* 00D394 LSR */
    A = lsr16(A);  /* 00D395 LSR */
    A = lsr16(A);  /* 00D396 LSR */
    A = lsr16(A);  /* 00D397 LSR */
    mflag = 1;  /* 00D398 SEM */
    SETLO(A, A ^ 0xFF);  /* 00D399 EOR */
    SETLO(A, A + 1);  /* 00D39B INC */
    C = 0;  /* 00D39C CLC */
    SETLO(A, adc8(A, rd8(DP(0x00))));  /* 00D39D ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00D39F STA */
    mflag = 0;  /* 00D3A1 CLM */
    wr16(DP(0x03), rd16(DP(0x03)) & ~0xFFE0);  /* 00D3A2 CLB */
L_00D3A6: TICK();
    mflag = 1;  /* 00D3A6 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00D3A7 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    if (N) goto L_00D3B0;  /* 00D3A9 BMI */
    if (Z) goto L_00D3B0;  /* 00D3AB BEQ */
    goto L_00D30C;  /* 00D3AD JMP */
L_00D3B0: TICK();
    SETLO(A, 0x00);  /* 00D3B0 LDA */
    D = pull16();  /* 00D3B2 PLD */
    S += 3; return;  /* 00D3B3 RTL */
}

/* 00D3B4  callers: 0080DF, 00D544, 00EA2A, 00ED28 */
void wait_synth_lock_00D3B4(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D3B4 LDA */
    push16(D);  /* 00D3B6 PHD */
    D = A;  /* 00D3B7 TAD */
    wr16(DP(0x00), Y);  /* 00D3B8 STY */
    mflag = 1;  /* 00D3BA SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00D3BB CLB */
    wr8(ABS(TA3IC), 0x00);  /* 00D3BF LDM */
    wr8(ABS(TA3MOD), 0x80);  /* 00D3C3 LDM */
    SETLO(A, Y);  /* 00D3C7 TYA */
    if ((A & 0xFF) != 0) goto L_00D3D0;  /* 00D3C8 BNE */
    SETLO(A, rd8(ABS(0x0346)));  /* 00D3CA LDA */
    goto L_00D3D3;  /* 00D3CD JMP */
L_00D3D0: TICK();
    SETLO(A, rd8(ABS(0x034B)));  /* 00D3D0 LDA */
L_00D3D3: TICK();
    mflag = 0;  /* 00D3D3 CLM */
    A &= 0x00FF;  /* 00D3D4 AND */
    A = asl16(A);  /* 00D3D7 ASL */
    A = asl16(A);  /* 00D3D8 ASL */
    A = asl16(A);  /* 00D3D9 ASL */
    A = asl16(A);  /* 00D3DA ASL */
    A = asl16(A);  /* 00D3DB ASL */
    wr16(ABS(TA3), A);  /* 00D3DC STA */
    mflag = 1;  /* 00D3DF SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00D3E0 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 00D3E4 SEB */
L_00D3E8: TICK();
    wr8(ABS(WDT), 0x00);  /* 00D3E8 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 00D3EC LDA */
    if ((A & 0xFF) != 0) goto L_00D3F7;  /* 00D3EE BNE */
    if ((rd8(ABS(P6)) & 0x80) == 0) goto L_00D3F7;  /* 00D3F0 BBC [P67 SENTEZOR_KILIT] */
    D = pull16();  /* 00D3F5 PLD */
    S += 3; return;  /* 00D3F6 RTL */
L_00D3F7: TICK();
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_00D3E8;  /* 00D3F7 BBC */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00D3FC CLB */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00D400 CLB */
    SETLO(A, 0x01);  /* 00D404 LDA */
    D = pull16();  /* 00D406 PLD */
    S += 3; return;  /* 00D407 RTL */
}

/* 00D408  callers: 00D56E */
void F_00D408(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D408 LDA */
    push16(D);  /* 00D40A PHD */
    D = A;  /* 00D40B TAD */
    A += 1;  /* 00D40C INC */
    A += 1;  /* 00D40D INC */
    push16(A);  /* 00D40E PHA */
    mflag = 1;  /* 00D40F SEM */
    wr8(ABS(TA3IC), 0x00);  /* 00D410 LDM */
    wr8(ABS(TA3MOD), 0x80);  /* 00D414 LDM */
    mflag = 0;  /* 00D418 CLM */
    A = Y;  /* 00D419 TYA */
    wr16(DP(0x00), Y);  /* 00D41A STY */
    mpy16(0x007E);  /* 00D41C MPY */
    wr16(ABS(TA3), A);  /* 00D420 STA */
    mflag = 1;  /* 00D423 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00D424 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 00D428 SEB */
    mflag = 0;  /* 00D42C CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00D440;  /* 00D42D BBC */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0x0100) goto L_00D440;  /* 00D433 BBS */
    Y = rd16(ABS(0x0383));  /* 00D439 LDY */
    S -= 3; F_008C05();  /* 00D43C JSRL */
L_00D440: TICK();
    mflag = 1;  /* 00D440 SEM */
    wr8(ABS(WDT), 0x00);  /* 00D441 LDM */
    mflag = 0;  /* 00D445 CLM */
    ram_trampoline_087A();  /* 00D446 JSRL */
    mflag = 1;  /* 00D44A SEM */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_00D440;  /* 00D44B BBC */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00D450 CLB */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00D454 CLB */
    mflag = 0;  /* 00D458 CLM */
    Y = pull16();  /* 00D459 PLY */
    D = pull16();  /* 00D45A PLD */
    S += 3; return;  /* 00D45B RTL */
}

/* 00D45C  callers: 00EA5C, 00ED49 */
void synth_load_00D45C(void)
{
    uint32_t t; (void)t;
    I = 1;  /* 00D45C SEI */
    mflag = 1;  /* 00D45D SEM */
    wr8(ABS(U1MOD), 0x01);  /* 00D45E LDM */
    wr8(ABS(U1C0), 0x00);  /* 00D462 LDM */
    wr8(ABS(U1BRG), 0x03);  /* 00D466 LDM */
    wr8(ABS(U1C1), 0x01);  /* 00D46A LDM */
    wr8(ABS(P8), rd8(ABS(P8)) & ~0x02);  /* 00D46E CLB [P81 SENTEZOR_SEC] */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x01);  /* 00D472 CLB [P70 SENTEZOR_YUKLE] */
    SETLO(A, rd8(ABS(0x0216)));  /* 00D476 LDA */
    wr8(ABS(U1TB), (uint8_t)A);  /* 00D479 STA */
L_00D47C: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_00D484;  /* 00D47C BBS */
    goto L_00D47C;  /* 00D481 JMP */
L_00D484: TICK();
    mflag = 0;  /* 00D484 CLM */
    A = rd16(ABS(0x0217));  /* 00D485 LDA */
    A &= 0x00FF;  /* 00D488 AND */
    mflag = 1;  /* 00D48B SEM */
    wr8(ABS(U1TB), (uint8_t)A);  /* 00D48C STA */
L_00D48F: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_00D497;  /* 00D48F BBS */
    goto L_00D48F;  /* 00D494 JMP */
L_00D497: TICK();
    mflag = 1;  /* 00D497 SEM */
    if ((rd8(ABS(U1C0)) & 0x08) == 0x08) goto L_00D4A0;  /* 00D498 BBS */
    goto L_00D497;  /* 00D49D JMP */
L_00D4A0: TICK();
    mflag = 1;  /* 00D4A0 SEM */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x01);  /* 00D4A1 SEB [P70 SENTEZOR_YUKLE] */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x01);  /* 00D4A5 CLB [P70 SENTEZOR_YUKLE] */
    mflag = 0;  /* 00D4A9 CLM */
    A = rd16(ABS(0x0213));  /* 00D4AA LDA */
    A &= 0x00FF;  /* 00D4AD AND */
    mflag = 1;  /* 00D4B0 SEM */
    wr8(ABS(U1TB), (uint8_t)A);  /* 00D4B1 STA */
L_00D4B4: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_00D4BC;  /* 00D4B4 BBS */
    goto L_00D4B4;  /* 00D4B9 JMP */
L_00D4BC: TICK();
    mflag = 0;  /* 00D4BC CLM */
    A = rd16(ABS(0x0212));  /* 00D4BD LDA */
    mflag = 1;  /* 00D4C0 SEM */
    wr8(ABS(U1TB), (uint8_t)A);  /* 00D4C1 STA */
L_00D4C4: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_00D4CC;  /* 00D4C4 BBS */
    goto L_00D4C4;  /* 00D4C9 JMP */
L_00D4CC: TICK();
    mflag = 0;  /* 00D4CC CLM */
    A = rd16(ABS(0x0208));  /* 00D4CD LDA */
    mflag = 1;  /* 00D4D0 SEM */
    wr8(ABS(U1TB), (uint8_t)A);  /* 00D4D1 STA */
L_00D4D4: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_00D4DC;  /* 00D4D4 BBS */
    goto L_00D4D4;  /* 00D4D9 JMP */
L_00D4DC: TICK();
    mflag = 1;  /* 00D4DC SEM */
    if ((rd8(ABS(U1C0)) & 0x08) == 0x08) goto L_00D4E5;  /* 00D4DD BBS */
    goto L_00D4DC;  /* 00D4E2 JMP */
L_00D4E5: TICK();
    mflag = 1;  /* 00D4E5 SEM */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x01);  /* 00D4E6 SEB [P70 SENTEZOR_YUKLE] */
    wr8(ABS(P8), rd8(ABS(P8)) | 0x02);  /* 00D4EA SEB [P81 SENTEZOR_SEC] */
    wr8(ABS(U1C1), rd8(ABS(U1C1)) & ~0x05);  /* 00D4EE CLB */
    wr8(ABS(U1MOD), 0x00);  /* 00D4F2 LDM */
    I = 0;  /* 00D4F6 CLI */
    mflag = 0;  /* 00D4F7 CLM */
    S += 3; return;  /* 00D4F8 RTL */
}

/* 00D4F9  callers: 00EA58 */
void F_00D4F9(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00D4F9 SEM */
    wr8(ABS(TA2IC), 0x00);  /* 00D4FA LDM */
    wr8(ABS(TA2MOD), 0x27);  /* 00D4FE LDM */
    wr8(ABS(TA2), 0x01);  /* 00D502 LDM */
    SETLO(A, rd8(ABS(0x01FB)));  /* 00D506 LDA */
    wr8(ABS(TA2_H), (uint8_t)A);  /* 00D509 STA */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x04);  /* 00D50C SEB */
    mflag = 0;  /* 00D510 CLM */
    S += 3; return;  /* 00D511 RTL */
}

/* 00D512  callers: 00EA88 */
void F_00D512(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00D512 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00D513 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x04);  /* 00D517 CLB */
    mflag = 0;  /* 00D51B CLM */
    S += 3; return;  /* 00D51C RTL */
}

/* 00D51D  callers: 00EA60, 00ED4D */
void synth_load_and_wait_00D51D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D51D LDA */
    push16(D);  /* 00D51F PHD */
    D = A;  /* 00D520 TAD */
    push16(A);  /* 00D521 PHA */
    mflag = 1;  /* 00D522 SEM */
    wr8(ABS(0x022A), 0x02);  /* 00D523 LDM */
    mflag = 0;  /* 00D527 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0008);  /* 00D528 CLB */
L_00D52D: TICK();
    mflag = 1;  /* 00D52D SEM */
    SETLO(A, rd8(ABS(0x022A)));  /* 00D52E LDA */
    if ((A & 0xFF) == 0) goto L_00D57F;  /* 00D531 BEQ */
    mflag = 0;  /* 00D533 CLM */
    if ((rd16(ABS(0x023E)) & 0x0008) == 0x0008) goto L_00D57F;  /* 00D534 BBS */
    mflag = 1;  /* 00D53A SEM */
    if ((rd8(ABS(P6)) & 0x80) == 0x80) goto L_00D577;  /* 00D53B BBS [P67 SENTEZOR_KILIT] */
    Y = 0x0000;  /* 00D540 LDY */
    mflag = 0;  /* 00D543 CLM */
    S -= 3; wait_synth_lock_00D3B4();  /* 00D544 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00D548 STA */
    SETLO(A, A & 0x01);  /* 00D54A AND */
    SETLO(A, asl8(A));  /* 00D54C ASL */
    SETLO(A, asl8(A));  /* 00D54D ASL */
    SETLO(A, asl8(A));  /* 00D54E ASL */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x08);  /* 00D54F CLB */
    SETLO(A, A | rd8(ABS(0x023E)));  /* 00D553 ORA */
    wr8(ABS(0x023E), (uint8_t)A);  /* 00D556 STA */
    mflag = 0;  /* 00D559 CLM */
    A = rd16(DP(0x00));  /* 00D55A LDA */
    A &= 0x0001;  /* 00D55C AND */
    if (A == 0) goto L_00D577;  /* 00D55F BEQ */
    mflag = 1;  /* 00D561 SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x01);  /* 00D562 CLB [P70 SENTEZOR_YUKLE] */
    SETLO(A, rd8(ABS(0x0184)));  /* 00D566 LDA */
    mflag = 0;  /* 00D569 CLM */
    A &= 0x00FF;  /* 00D56A AND */
    Y = A;  /* 00D56D TAY */
    S -= 3; F_00D408();  /* 00D56E JSRL */
    mflag = 1;  /* 00D572 SEM */
    wr8(ABS(0x022A), 0x00);  /* 00D573 LDM */
L_00D577: TICK();
    mflag = 1;  /* 00D577 SEM */
    wr8(ABS(WDT), 0x00);  /* 00D578 LDM */
    goto L_00D52D;  /* 00D57C JMP */
L_00D57F: TICK();
    mflag = 1;  /* 00D57F SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x01);  /* 00D580 CLB [P70 SENTEZOR_YUKLE] */
    mflag = 0;  /* 00D584 CLM */
    if ((rd16(ABS(0x023E)) & 0x0008) == 0x0008) goto L_00D5AC;  /* 00D585 BBS */
    if ((rd16(ABS(0x0242)) & 0x8000) == 0x8000) goto L_00D599;  /* 00D58B BBS */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x8000);  /* 00D591 SEB */
    goto L_00D5A9;  /* 00D596 JMP */
L_00D599: TICK();
    mflag = 0;  /* 00D599 CLM */
    if ((rd16(ABS(0x0246)) & 0x0008) == 0x0008) goto L_00D5A9;  /* 00D59A BBS */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0008);  /* 00D5A0 SEB */
    S -= 3; F_009782();  /* 00D5A5 JSRL */
L_00D5A9: TICK();
    goto L_00D5C7;  /* 00D5A9 JMP */
L_00D5AC: TICK();
    mflag = 0;  /* 00D5AC CLM */
    if ((rd16(ABS(0x0242)) & 0x8000) == 0) goto L_00D5C7;  /* 00D5AD BBC */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x8000);  /* 00D5B3 CLB */
    if ((rd16(ABS(0x0246)) & 0x0008) == 0) goto L_00D5C7;  /* 00D5B8 BBC */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0008);  /* 00D5BE CLB */
    S -= 3; F_009782();  /* 00D5C3 JSRL */
L_00D5C7: TICK();
    mflag = 1;  /* 00D5C7 SEM */
    SETLO(A, rd8(ABS(0x02A3)));  /* 00D5C8 LDA */
    if ((A & 0xFF) == 0) goto L_00D5E5;  /* 00D5CB BEQ */
    mflag = 0;  /* 00D5CD CLM */
    if ((rd16(ABS(0x023E)) & 0x0008) == 0) goto L_00D5DA;  /* 00D5CE BBC */
    A = 0x0000;  /* 00D5D4 LDA */
    goto L_00D5DD;  /* 00D5D7 JMP */
L_00D5DA: TICK();
    A = 0x0002;  /* 00D5DA LDA */
L_00D5DD: TICK();
    mflag = 1;  /* 00D5DD SEM */
    wr8(ABS(0x029E), (uint8_t)A);  /* 00D5DE STA */
    wr8(ABS(0x02A3), 0x02);  /* 00D5E1 LDM */
L_00D5E5: TICK();
    mflag = 0;  /* 00D5E5 CLM */
    Y = pull16();  /* 00D5E6 PLY */
    D = pull16();  /* 00D5E7 PLD */
    S += 3; return;  /* 00D5E8 RTL */
}

/* 00D5E9  callers: 00D7E8, 00D7FB */
void F_00D5E9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D5E9 LDA */
    push16(D);  /* 00D5EB PHD */
    D = A;  /* 00D5EC TAD */
    C = 0;  /* 00D5ED CLC */
    A = adc16(A, 0x000F);  /* 00D5EE ADC */
    push16(A);  /* 00D5F1 PHA */
    A = rd16(DP(0x01));  /* 00D5F2 LDA */
    A &= 0x00FF;  /* 00D5F4 AND */
    wr16(DP(0x02), Y);  /* 00D5F7 STY */
    mflag = 1;  /* 00D5F9 SEM */
    wr8(DP(0x04), (uint8_t)B);  /* 00D5FA STA */
    C = 0;  /* 00D5FD CLC */
    mflag = 0;  /* 00D5FE CLM */
    A = adc16(A, rd16(DP(0x02)));  /* 00D5FF ADC */
    C = 0;  /* 00D601 CLC */
    A = adc16(A, 0x000C);  /* 00D602 ADC */
    wr16(DP(0x0F), A);  /* 00D605 STA */
    mflag = 1;  /* 00D607 SEM */
    wr8(DP(0x11), (uint8_t)B);  /* 00D608 STA */
    SETLO(A, rd8(LIND(0x0F)));  /* 00D60B LDA */
    Z = ((A & 0xFF) == 0);
    wr8(DP(0x06), 0x00);  /* 00D60D LDM */
    wr8(DP(0x05), (uint8_t)A);  /* 00D610 STA */
    if (Z) goto L_00D66A;  /* 00D612 BEQ */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00D61E;  /* 00D614 BBC */
    SETLO(A, 0x0A);  /* 00D619 LDA */
    goto L_00D620;  /* 00D61B JMP */
L_00D61E: TICK();
    SETLO(A, 0x63);  /* 00D61E LDA */
L_00D620: TICK();
    t = rd8(DP(0x05));  /* 00D620 CMP */
    if ((A & 0xFF) < t) goto L_00D666;  /* 00D622 BCC */
    Y = rd16(DP(0x05));  /* 00D624 LDY */
    mflag = 0;  /* 00D626 CLM */
    S -= 3; channel_is_programmed_0087A7();  /* 00D627 JSRL */
    X = A;  /* 00D62B TAX */
    if (X != 0) goto L_00D639;  /* 00D62C BNE */
    A = rd16(DP(0x05));  /* 00D62E LDA */
    mflag = 1;  /* 00D630 SEM */
    wr8(DP(0x06), (uint8_t)A);  /* 00D631 STA */
    wr8(DP(0x05), 0x00);  /* 00D633 LDM */
    goto L_00D663;  /* 00D636 JMP */
L_00D639: TICK();
    mflag = 1;  /* 00D639 SEM */
    if ((rd8(ABS(0x033F)) & 0x40) == 0) goto L_00D663;  /* 00D63A BBC */
    Y = rd16(DP(0x05));  /* 00D63F LDY */
    mflag = 0;  /* 00D641 CLM */
    S -= 3; F_0091BB();  /* 00D642 JSRL */
    X = A;  /* 00D646 TAX */
    if (X == 0) goto L_00D663;  /* 00D647 BEQ */
    A = rd16(DP(0x01));  /* 00D649 LDA */
    A &= 0x00FF;  /* 00D64B AND */
    C = 0;  /* 00D64E CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00D64F ADC */
    C = 0;  /* 00D651 CLC */
    A = adc16(A, 0x000C);  /* 00D652 ADC */
    wr16(DP(0x0F), A);  /* 00D655 STA */
    A = rd16(DP(0x04));  /* 00D657 LDA */
    mflag = 1;  /* 00D659 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 00D65A STA */
    SETLO(A, 0x00);  /* 00D65C LDA */
    wr8(LIND(0x0F), (uint8_t)A);  /* 00D65E STA */
    wr8(DP(0x00), 0x01);  /* 00D660 LDM */
L_00D663: TICK();
    goto L_00D66A;  /* 00D663 JMP */
L_00D666: TICK();
    mflag = 1;  /* 00D666 SEM */
    wr8(DP(0x05), 0x00);  /* 00D667 LDM */
L_00D66A: TICK();
    mflag = 1;  /* 00D66A SEM */
    SETLO(A, rd8(DP(0x05)));  /* 00D66B LDA */
    if ((A & 0xFF) == 0) goto L_00D672;  /* 00D66D BEQ */
    goto L_00D734;  /* 00D66F JMP */
L_00D672: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 00D672 LDA */
    mflag = 0;  /* 00D674 CLM */
    A &= 0x00FF;  /* 00D675 AND */
    C = 0;  /* 00D678 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00D679 ADC */
    C = 0;  /* 00D67B CLC */
    A = adc16(A, 0x000C);  /* 00D67C ADC */
    wr16(DP(0x0F), A);  /* 00D67F STA */
    A = rd16(DP(0x04));  /* 00D681 LDA */
    mflag = 1;  /* 00D683 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 00D684 STA */
    SETLO(A, rd8(LIND(0x0F)));  /* 00D686 LDA */
    t = rd8(DP(0x05));  /* 00D688 CMP */
    if ((A & 0xFF) != t) goto L_00D68F;  /* 00D68A BNE */
    goto L_00D731;  /* 00D68C JMP */
L_00D68F: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 00D68F LDA */
    mflag = 0;  /* 00D691 CLM */
    A &= 0x00FF;  /* 00D692 AND */
    C = 0;  /* 00D695 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00D696 ADC */
    C = 0;  /* 00D698 CLC */
    A = adc16(A, 0x000C);  /* 00D699 ADC */
    wr16(DP(0x0F), A);  /* 00D69C STA */
    A = rd16(DP(0x04));  /* 00D69E LDA */
    mflag = 1;  /* 00D6A0 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 00D6A1 STA */
    SETLO(A, rd8(DP(0x05)));  /* 00D6A3 LDA */
    wr8(LIND(0x0F), (uint8_t)A);  /* 00D6A5 STA */
    wr8(DP(0x00), 0x01);  /* 00D6A7 LDM */
    SETLO(A, rd8(DP(0x06)));  /* 00D6AA LDA */
    if ((A & 0xFF) != 0) goto L_00D6B1;  /* 00D6AC BNE */
    goto L_00D731;  /* 00D6AE JMP */
L_00D6B1: TICK();
    mflag = 0;  /* 00D6B1 CLM */
    wr16(DP(0x0F), 0x0008);  /* 00D6B2 LDM */
    A = rd16(DP(0x06));  /* 00D6B6 LDA */
    A &= 0x00FF;  /* 00D6B8 AND */
    A -= 1;  /* 00D6BB DEC */
    wr16(DP(0x11), A);  /* 00D6BC STA */
    A = D;  /* 00D6BE TDA */
    C = 0;  /* 00D6BF CLC */
    A = adc16(A, 0x000B);  /* 00D6C0 ADC */
    B = 0x0000;  /* 00D6C3 LDA */
    Y = A;  /* 00D6C7 TAY */
    S -= 3; F_01E30B();  /* 00D6C8 JSRL */
    wr16(DP(0x0F), A);  /* 00D6CC STA */
    A = D;  /* 00D6CE TDA */
    C = 0;  /* 00D6CF CLC */
    A = adc16(A, 0x0007);  /* 00D6D0 ADC */
    wr16(DP(0x11), A);  /* 00D6D3 STA */
    mflag = 1;  /* 00D6D5 SEM */
    wr8(DP(0x13), (uint8_t)B);  /* 00D6D6 STA */
    mflag = 0;  /* 00D6D9 CLM */
    A = rd16(DP(0x0F));  /* 00D6DA LDA */
    wr16(DP(0x14), A);  /* 00D6DC STA */
    A = rd16(DP(0x13));  /* 00D6DE LDA */
    mflag = 1;  /* 00D6E0 SEM */
    wr8(DP(0x16), (uint8_t)A);  /* 00D6E1 STA */
    mflag = 0;  /* 00D6E3 CLM */
    A = rd16(DP(0x11));  /* 00D6E4 LDA */
    wr16(DP(0x17), A);  /* 00D6E6 STA */
    mflag = 1;  /* 00D6E8 SEM */
    wr8(DP(0x19), 0x00);  /* 00D6E9 LDM */
    mflag = 0;  /* 00D6EC CLM */
    A = 0x0014;  /* 00D6ED LDA */
    Y = 0x0004;  /* 00D6F0 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00D6F3 JSRL */
    A = rd16(DP(0x02));  /* 00D6F7 LDA */
    C = 0;  /* 00D6F9 CLC */
    A = adc16(A, rd16(DP(0x07)));  /* 00D6FA ADC */
    wr16(DP(0x0F), A);  /* 00D6FC STA */
    A = rd16(DP(0x04));  /* 00D6FE LDA */
    mflag = 1;  /* 00D700 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 00D701 STA */
    X = rd16(DP(0x09));  /* 00D703 LDX */
    SETLO(A, 0x01);  /* 00D705 LDA */
    S -= 3; rt_shl_01E4E0();  /* 00D707 JSRL */
    SETLO(A, A & rd8(LIND(0x0F)));  /* 00D70B AND */
    if ((A & 0xFF) == 0) goto L_00D731;  /* 00D70D BEQ */
    X = rd16(DP(0x09));  /* 00D70F LDX */
    SETLO(A, 0x01);  /* 00D711 LDA */
    S -= 3; rt_shl_01E4E0();  /* 00D713 JSRL */
    SETLO(A, A ^ 0xFF);  /* 00D717 EOR */
    wr8(DP(0x0F), (uint8_t)A);  /* 00D719 STA */
    mflag = 0;  /* 00D71B CLM */
    A = rd16(DP(0x02));  /* 00D71C LDA */
    C = 0;  /* 00D71E CLC */
    A = adc16(A, rd16(DP(0x07)));  /* 00D71F ADC */
    wr16(DP(0x10), A);  /* 00D721 STA */
    A = rd16(DP(0x04));  /* 00D723 LDA */
    mflag = 1;  /* 00D725 SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 00D726 STA */
    SETLO(A, rd8(LIND(0x10)));  /* 00D728 LDA */
    SETLO(A, A & rd8(DP(0x0F)));  /* 00D72A AND */
    wr8(LIND(0x10), (uint8_t)A);  /* 00D72C STA */
    wr8(DP(0x00), 0x01);  /* 00D72E LDM */
L_00D731: TICK();
    goto L_00D7B1;  /* 00D731 JMP */
L_00D734: TICK();
    mflag = 0;  /* 00D734 CLM */
    wr16(DP(0x0F), 0x0008);  /* 00D735 LDM */
    A = rd16(DP(0x05));  /* 00D739 LDA */
    A &= 0x00FF;  /* 00D73B AND */
    A -= 1;  /* 00D73E DEC */
    wr16(DP(0x11), A);  /* 00D73F STA */
    A = D;  /* 00D741 TDA */
    C = 0;  /* 00D742 CLC */
    A = adc16(A, 0x000B);  /* 00D743 ADC */
    B = 0x0000;  /* 00D746 LDA */
    Y = A;  /* 00D74A TAY */
    S -= 3; F_01E30B();  /* 00D74B JSRL */
    wr16(DP(0x0F), A);  /* 00D74F STA */
    A = D;  /* 00D751 TDA */
    C = 0;  /* 00D752 CLC */
    A = adc16(A, 0x0007);  /* 00D753 ADC */
    wr16(DP(0x11), A);  /* 00D756 STA */
    mflag = 1;  /* 00D758 SEM */
    wr8(DP(0x13), (uint8_t)B);  /* 00D759 STA */
    mflag = 0;  /* 00D75C CLM */
    A = rd16(DP(0x0F));  /* 00D75D LDA */
    wr16(DP(0x14), A);  /* 00D75F STA */
    A = rd16(DP(0x13));  /* 00D761 LDA */
    mflag = 1;  /* 00D763 SEM */
    wr8(DP(0x16), (uint8_t)A);  /* 00D764 STA */
    mflag = 0;  /* 00D766 CLM */
    A = rd16(DP(0x11));  /* 00D767 LDA */
    wr16(DP(0x17), A);  /* 00D769 STA */
    mflag = 1;  /* 00D76B SEM */
    wr8(DP(0x19), 0x00);  /* 00D76C LDM */
    mflag = 0;  /* 00D76F CLM */
    A = 0x0014;  /* 00D770 LDA */
    Y = 0x0004;  /* 00D773 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00D776 JSRL */
    A = rd16(DP(0x02));  /* 00D77A LDA */
    C = 0;  /* 00D77C CLC */
    A = adc16(A, rd16(DP(0x07)));  /* 00D77D ADC */
    wr16(DP(0x0F), A);  /* 00D77F STA */
    A = rd16(DP(0x04));  /* 00D781 LDA */
    mflag = 1;  /* 00D783 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 00D784 STA */
    X = rd16(DP(0x09));  /* 00D786 LDX */
    SETLO(A, 0x01);  /* 00D788 LDA */
    S -= 3; rt_shl_01E4E0();  /* 00D78A JSRL */
    SETLO(A, A & rd8(LIND(0x0F)));  /* 00D78E AND */
    if ((A & 0xFF) != 0) goto L_00D7B1;  /* 00D790 BNE */
    X = rd16(DP(0x09));  /* 00D792 LDX */
    SETLO(A, A + 1);  /* 00D794 INC */
    S -= 3; rt_shl_01E4E0();  /* 00D795 JSRL */
    wr8(DP(0x0F), (uint8_t)A);  /* 00D799 STA */
    mflag = 0;  /* 00D79B CLM */
    A = rd16(DP(0x02));  /* 00D79C LDA */
    C = 0;  /* 00D79E CLC */
    A = adc16(A, rd16(DP(0x07)));  /* 00D79F ADC */
    wr16(DP(0x10), A);  /* 00D7A1 STA */
    A = rd16(DP(0x04));  /* 00D7A3 LDA */
    mflag = 1;  /* 00D7A5 SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 00D7A6 STA */
    SETLO(A, rd8(LIND(0x10)));  /* 00D7A8 LDA */
    SETLO(A, A | rd8(DP(0x0F)));  /* 00D7AA ORA */
    wr8(LIND(0x10), (uint8_t)A);  /* 00D7AC STA */
    wr8(DP(0x00), 0x01);  /* 00D7AE LDM */
L_00D7B1: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00D7B1 LDA */
    Y = pull16();  /* 00D7B3 PLY */
    D = pull16();  /* 00D7B4 PLD */
    S += 3; return;  /* 00D7B5 RTL */
}

/* 00D7B6  callers: 00E4BE */
void F_00D7B6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D7B6 LDA */
    push16(D);  /* 00D7B8 PHD */
    D = A;  /* 00D7B9 TAD */
    C = 0;  /* 00D7BA CLC */
    A = adc16(A, 0x0019);  /* 00D7BB ADC */
    push16(A);  /* 00D7BE PHA */
    wr16(DP(0x19), 0x000F);  /* 00D7BF LDM */
    A = D;  /* 00D7C3 TDA */
    A += 1;  /* 00D7C4 INC */
    B = 0x0000;  /* 00D7C5 LDA */
    wr16(DP(0x1A), A);  /* 00D7C9 STA */
    wr16(DP(0x1C), B);  /* 00D7CB STA */
    wr16(DP(0x1D), 0x0537);  /* 00D7CE LDM */
    Y = 0x00A0;  /* 00D7D2 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00D7D5 JSRL */
    wr16(DP(0x19), 0x0000);  /* 00D7D9 LDM */
    wr16(DP(0x1A), 0x0001);  /* 00D7DD LDM */
    A = D;  /* 00D7E1 TDA */
    A += 1;  /* 00D7E2 INC */
    B = 0x0000;  /* 00D7E3 LDA */
    Y = A;  /* 00D7E7 TAY */
    S -= 3; F_00D5E9();  /* 00D7E8 JSRL */
    wr8(DP(0x19), (uint8_t)A);  /* 00D7EC STA */
    wr8(DP(0x1A), 0x02);  /* 00D7EE LDM */
    wr8(DP(0x00), (uint8_t)A);  /* 00D7F1 STA */
    mflag = 0;  /* 00D7F3 CLM */
    A = D;  /* 00D7F4 TDA */
    A += 1;  /* 00D7F5 INC */
    B = 0x0000;  /* 00D7F6 LDA */
    Y = A;  /* 00D7FA TAY */
    S -= 3; F_00D5E9();  /* 00D7FB JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00D7FF STA */
    SETLO(A, rd8(DP(0x0E)));  /* 00D801 LDA */
    wr8(ABS(0x038A), (uint8_t)A);  /* 00D803 STA */
    SETLO(A, rd8(DP(0x0F)));  /* 00D806 LDA */
    wr8(ABS(0x0391), (uint8_t)A);  /* 00D808 STA */
    wr8(DP(0x10), 0x00);  /* 00D80B LDM */
L_00D80E: TICK();
    mflag = 0;  /* 00D80E CLM */
    wr16(DP(0x19), 0x0008);  /* 00D80F LDM */
    A = rd16(DP(0x10));  /* 00D813 LDA */
    A &= 0x00FF;  /* 00D815 AND */
    wr16(DP(0x1B), A);  /* 00D818 STA */
    A = D;  /* 00D81A TDA */
    C = 0;  /* 00D81B CLC */
    A = adc16(A, 0x0015);  /* 00D81C ADC */
    B = 0x0000;  /* 00D81F LDA */
    Y = A;  /* 00D823 TAY */
    S -= 3; F_01E30B();  /* 00D824 JSRL */
    wr16(DP(0x19), A);  /* 00D828 STA */
    A = D;  /* 00D82A TDA */
    C = 0;  /* 00D82B CLC */
    A = adc16(A, 0x0011);  /* 00D82C ADC */
    wr16(DP(0x1B), A);  /* 00D82F STA */
    mflag = 1;  /* 00D831 SEM */
    wr8(DP(0x1D), (uint8_t)B);  /* 00D832 STA */
    mflag = 0;  /* 00D835 CLM */
    A = rd16(DP(0x19));  /* 00D836 LDA */
    wr16(DP(0x1E), A);  /* 00D838 STA */
    A = rd16(DP(0x1D));  /* 00D83A LDA */
    mflag = 1;  /* 00D83C SEM */
    wr8(DP(0x20), (uint8_t)A);  /* 00D83D STA */
    mflag = 0;  /* 00D83F CLM */
    A = rd16(DP(0x1B));  /* 00D840 LDA */
    wr16(DP(0x21), A);  /* 00D842 STA */
    mflag = 1;  /* 00D844 SEM */
    wr8(DP(0x23), 0x00);  /* 00D845 LDM */
    mflag = 0;  /* 00D848 CLM */
    A = 0x001E;  /* 00D849 LDA */
    Y = 0x0004;  /* 00D84C LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00D84F JSRL */
    X = rd16(DP(0x11));  /* 00D853 LDX */
    A = rd16(DP(0x01 + X));  /* 00D855 LDA */
    mflag = 1;  /* 00D857 SEM */
    wr8(DP(0x19), (uint8_t)A);  /* 00D858 STA */
    X = rd16(DP(0x13));  /* 00D85A LDX */
    SETLO(A, 0x01);  /* 00D85C LDA */
    S -= 3; rt_shl_01E4E0();  /* 00D85E JSRL */
    SETLO(A, A & rd8(DP(0x19)));  /* 00D862 AND */
    if ((A & 0xFF) == 0) goto L_00D889;  /* 00D864 BEQ */
    SETLO(A, rd8(DP(0x10)));  /* 00D866 LDA */
    SETLO(A, A + 1);  /* 00D868 INC */
    mflag = 0;  /* 00D869 CLM */
    Y = A;  /* 00D86A TAY */
    S -= 3; channel_is_programmed_0087A7();  /* 00D86B JSRL */
    X = A;  /* 00D86F TAX */
    if (X != 0) goto L_00D889;  /* 00D870 BNE */
    X = rd16(DP(0x13));  /* 00D872 LDX */
    A += 1;  /* 00D874 INC */
    mflag = 1;  /* 00D875 SEM */
    S -= 3; rt_shl_01E4E0();  /* 00D876 JSRL */
    SETLO(A, A ^ 0xFF);  /* 00D87A EOR */
    X = rd16(DP(0x11));  /* 00D87C LDX */
    wr8(DP(0x19), (uint8_t)A);  /* 00D87E STA */
    SETLO(A, rd8(DP(0x01 + X)));  /* 00D880 LDA */
    SETLO(A, A & rd8(DP(0x19)));  /* 00D882 AND */
    wr8(DP(0x01 + X), (uint8_t)A);  /* 00D884 STA */
    wr8(DP(0x00), 0x01);  /* 00D886 LDM */
L_00D889: TICK();
    mflag = 1;  /* 00D889 SEM */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00D894;  /* 00D88A BBC */
    SETLO(A, 0x0A);  /* 00D88F LDA */
    goto L_00D896;  /* 00D891 JMP */
L_00D894: TICK();
    SETLO(A, 0x63);  /* 00D894 LDA */
L_00D896: TICK();
    t = (rd8(DP(0x10)) + 1) & 0xFF; wr8(DP(0x10), t);  /* 00D896 INC */
    t = rd8(DP(0x10));  /* 00D898 CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_00D8A1;  /* 00D89A BCC */
    if (Z) goto L_00D8A1;  /* 00D89C BEQ */
    goto L_00D80E;  /* 00D89E JMP */
L_00D8A1: TICK();
    wr8(DP(0x10), 0x00);  /* 00D8A1 LDM */
L_00D8A4: TICK();
    SETLO(A, rd8(DP(0x10)));  /* 00D8A4 LDA */
    /* 00D8A6 CMP */
    if ((A & 0xFF) >= 0x0D) goto L_00D8C0;  /* 00D8A8 BCS */
    mflag = 0;  /* 00D8AA CLM */
    A &= 0x00FF;  /* 00D8AB AND */
    X = A;  /* 00D8AE TAX */
    A = rd16(DP(0x10));  /* 00D8AF LDA */
    A &= 0x00FF;  /* 00D8B1 AND */
    Y = A;  /* 00D8B4 TAY */
    A = rd16(DP(0x01 + X));  /* 00D8B5 LDA */
    mflag = 1;  /* 00D8B7 SEM */
    wr8((ABS(0x02D2) + Y), (uint8_t)A);  /* 00D8B8 STA */
    t = (rd8(DP(0x10)) + 1) & 0xFF; wr8(DP(0x10), t);  /* 00D8BB INC */
    goto L_00D8A4;  /* 00D8BD JMP */
L_00D8C0: TICK();
    mflag = 1;  /* 00D8C0 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00D8C1 LDA */
    if ((A & 0xFF) == 0) goto L_00D8DF;  /* 00D8C3 BEQ */
    wr8(DP(0x19), 0x0F);  /* 00D8C5 LDM */
    mflag = 0;  /* 00D8C8 CLM */
    A = D;  /* 00D8C9 TDA */
    A += 1;  /* 00D8CA INC */
    B = 0x0000;  /* 00D8CB LDA */
    wr16(DP(0x1A), A);  /* 00D8CF STA */
    wr16(DP(0x1C), B);  /* 00D8D1 STA */
    wr16(DP(0x1D), 0x0537);  /* 00D8D4 LDM */
    Y = 0x00A0;  /* 00D8D8 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00D8DB JSRL */
L_00D8DF: TICK();
    mflag = 0;  /* 00D8DF CLM */
    Y = pull16();  /* 00D8E0 PLY */
    D = pull16();  /* 00D8E1 PLD */
    S += 3; return;  /* 00D8E2 RTL */
}

/* 00D8E3  callers: 00E4A0 */
void F_00D8E3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00D8E3 LDA */
    push16(D);  /* 00D8E5 PHD */
    D = A;  /* 00D8E6 TAD */
    A += 1;  /* 00D8E7 INC */
    A += 1;  /* 00D8E8 INC */
    push16(A);  /* 00D8E9 PHA */
    S -= 3; set_options_010630();  /* 00D8EA JSRL */
    wr16(DP(0x00), 0x0000);  /* 00D8EE LDM */
L_00D8F2: TICK();
    Y = rd16(DP(0x00));  /* 00D8F2 LDY */
    S -= 3; F_0069EF();  /* 00D8F4 JSRL */
    /* 00D8F8 CMP */
    if ((A & 0xFF) == 0x0F) goto L_00D909;  /* 00D8FA BEQ */
    mflag = 0;  /* 00D8FC CLM */
    X = rd16(DP(0x00));  /* 00D8FD LDX */
    /* 00D8FF CPX */
    if (X >= 0x0006) goto L_00D909;  /* 00D902 BCS */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 00D904 INC */
    goto L_00D8F2;  /* 00D906 JMP */
L_00D909: TICK();
    mflag = 0;  /* 00D909 CLM */
    A = rd16(ABS(0x0343));  /* 00D90A LDA */
    mflag = 1;  /* 00D90D SEM */
    SETLO(A, A & 0x0F);  /* 00D90E AND */
    mflag = 0;  /* 00D910 CLM */
    A &= 0x00FF;  /* 00D911 AND */
    t = rd16(DP(0x00));  /* 00D914 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_00D927;  /* 00D916 BCC */
    if (Z) goto L_00D927;  /* 00D918 BEQ */
    mflag = 1;  /* 00D91A SEM */
    wr8(ABS(0x0343), rd8(ABS(0x0343)) & ~0x0F);  /* 00D91B CLB */
    SETLO(A, rd8(ABS(0x0343)));  /* 00D91F LDA */
    SETLO(A, A | rd8(DP(0x00)));  /* 00D922 ORA */
    wr8(ABS(0x0343), (uint8_t)A);  /* 00D924 STA */
L_00D927: TICK();
    mflag = 1;  /* 00D927 SEM */
    SETLO(B, 0x00);  /* 00D928 LDA */
    SETLO(A, rd8(ABS(0x0344)));  /* 00D92B LDA */
    div8(0x10);  /* 00D92E DIV */
    wr8(DP(0x02), (uint8_t)A);  /* 00D931 STA */
    SETLO(A, rd8(ABS(0x0343)));  /* 00D933 LDA */
    SETLO(A, A & 0x0F);  /* 00D936 AND */
    t = rd8(DP(0x02));  /* 00D938 CMP */
    if ((A & 0xFF) >= t) goto L_00D958;  /* 00D93A BCS */
    SETLO(A, rd8(ABS(0x0343)));  /* 00D93C LDA */
    SETLO(A, A & 0x0F);  /* 00D93F AND */
    mflag = 0;  /* 00D941 CLM */
    A &= 0x00FF;  /* 00D942 AND */
    A = asl16(A);  /* 00D945 ASL */
    A = asl16(A);  /* 00D946 ASL */
    A = asl16(A);  /* 00D947 ASL */
    A = asl16(A);  /* 00D948 ASL */
    mflag = 1;  /* 00D949 SEM */
    wr8(ABS(0x0344), rd8(ABS(0x0344)) & ~0xF0);  /* 00D94A CLB */
    mflag = 0;  /* 00D94E CLM */
    wr16(DP(0x00), A);  /* 00D94F STA */
    mflag = 1;  /* 00D951 SEM */
    SETLO(A, A | rd8(ABS(0x0344)));  /* 00D952 ORA */
    wr8(ABS(0x0344), (uint8_t)A);  /* 00D955 STA */
L_00D958: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00D98F;  /* 00D958 BBC */
    Y = 0x057C;  /* 00D95D LDY */
    mflag = 0;  /* 00D960 CLM */
    S -= 3; eeprom_read_word_0071F0();  /* 00D961 JSRL */
    /* 00D965 CMP */
    if (A < 0x3034) goto L_00D972;  /* 00D968 BCC */
    mflag = 1;  /* 00D96A SEM */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) | 0x04);  /* 00D96B SEB */
    goto L_00D977;  /* 00D96F JMP */
L_00D972: TICK();
    mflag = 1;  /* 00D972 SEM */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) & ~0x04);  /* 00D973 CLB */
L_00D977: TICK();
    wr8(ABS(0x033F), rd8(ABS(0x033F)) | 0x08);  /* 00D977 SEB */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x80);  /* 00D97B CLB */
    wr8(ABS(0x0340), rd8(ABS(0x0340)) & ~0x40);  /* 00D97F CLB */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_00D98C;  /* 00D983 BBS */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) & ~0x10);  /* 00D988 CLB */
L_00D98C: TICK();
    goto L_00D998;  /* 00D98C JMP */
L_00D98F: TICK();
    mflag = 1;  /* 00D98F SEM */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x02);  /* 00D990 CLB */
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0x20);  /* 00D994 CLB */
L_00D998: TICK();
    if ((rd8(ABS(0x033F)) & 0x80) == 0) goto L_00D9A9;  /* 00D998 BBC */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) | 0x08);  /* 00D99D SEB */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x60);  /* 00D9A1 CLB */
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0xF7);  /* 00D9A5 CLB */
L_00D9A9: TICK();
    mflag = 0;  /* 00D9A9 CLM */
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_00D9C2;  /* 00D9AA BBC */
    mflag = 1;  /* 00D9B0 SEM */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_00D9C2;  /* 00D9B1 BBS */
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0xF7);  /* 00D9B6 CLB */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x01);  /* 00D9BA CLB */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) | 0x08);  /* 00D9BE SEB */
L_00D9C2: TICK();
    mflag = 1;  /* 00D9C2 SEM */
    if ((rd8(ABS(0x0334)) & 0x10) == 0x10) goto L_00D9CE;  /* 00D9C3 BBS */
    mflag = 0;  /* 00D9C8 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0002);  /* 00D9C9 CLB */
L_00D9CE: TICK();
    mflag = 1;  /* 00D9CE SEM */
    if ((rd8(ABS(0x033F)) & 0x08) == 0x08) goto L_00D9DC;  /* 00D9CF BBS */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) & ~0x04);  /* 00D9D4 CLB */
    wr8(ABS(0x0340), rd8(ABS(0x0340)) & ~0x20);  /* 00D9D8 CLB */
L_00D9DC: TICK();
    if ((rd8(ABS(0x0335)) & 0x10) == 0x10) goto L_00D9E6;  /* 00D9DC BBS */
    if ((rd8(ABS(0x0335)) & 0x01) == 0x01) goto L_00D9EA;  /* 00D9E1 BBS */
L_00D9E6: TICK();
    wr8(ABS(0x0334), rd8(ABS(0x0334)) & ~0x07);  /* 00D9E6 CLB */
L_00D9EA: TICK();
    if ((rd8(ABS(0x0335)) & 0x10) == 0) goto L_00D9F3;  /* 00D9EA BBC */
    wr8(ABS(0x0340), rd8(ABS(0x0340)) & ~0x40);  /* 00D9EF CLB */
L_00D9F3: TICK();
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_00D9FD;  /* 00D9F3 BBS */
    if ((rd8(ABS(0x0334)) & 0x20) == 0) goto L_00DA2E;  /* 00D9F8 BBC */
L_00D9FD: TICK();
    mflag = 0;  /* 00D9FD CLM */
    A = rd16(ABS(0x029A));  /* 00D9FE LDA */
    A &= 0x000F;  /* 00DA01 AND */
    Y = 0x05D6;  /* 00DA04 LDY */
    wr16(DP(0x00), A);  /* 00DA07 STA */
    S -= 3; eeprom_read_byte_007189();  /* 00DA09 JSRL */
    mflag = 0;  /* 00DA0D CLM */
    A &= 0x00FF;  /* 00DA0E AND */
    C = 0;  /* 00DA11 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00DA12 ADC */
    wr16(DP(0x00), A);  /* 00DA14 STA */
    /* 00DA16 CMP */
    if (A < 0x0010) goto L_00DA1F;  /* 00DA19 BCC */
    wr16(DP(0x00), 0x000F);  /* 00DA1B LDM */
L_00DA1F: TICK();
    A = rd16(DP(0x00));  /* 00DA1F LDA */
    mflag = 1;  /* 00DA21 SEM */
    SETLO(A, A & 0x0F);  /* 00DA22 AND */
    wr8(ABS(0x029A), rd8(ABS(0x029A)) & ~0x0F);  /* 00DA24 CLB */
    SETLO(A, A | rd8(ABS(0x029A)));  /* 00DA28 ORA */
    wr8(ABS(0x029A), (uint8_t)A);  /* 00DA2B STA */
L_00DA2E: TICK();
    mflag = 0;  /* 00DA2E CLM */
    Y = pull16();  /* 00DA2F PLY */
    D = pull16();  /* 00DA30 PLD */
    S += 3; return;  /* 00DA31 RTL */
}

/* 00DA32  callers: 00E49C */
void F_00DA32(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00DA32 LDA */
    push16(D);  /* 00DA34 PHD */
    D = A;  /* 00DA35 TAD */
    C = 0;  /* 00DA36 CLC */
    A = adc16(A, 0x0036);  /* 00DA37 ADC */
    push16(A);  /* 00DA3A PHA */
    Y = 0x0000;  /* 00DA3B LDY */
    S -= 3; eeprom_read_byte_007189();  /* 00DA3E JSRL */
    wr8(ABS(0x0334), (uint8_t)A);  /* 00DA42 STA */
    wr8(DP(0x36), 0x0B);  /* 00DA45 LDM */
    mflag = 0;  /* 00DA48 CLM */
    A = D;  /* 00DA49 TDA */
    B = 0x0000;  /* 00DA4A LDA */
    wr16(DP(0x37), A);  /* 00DA4E STA */
    wr16(DP(0x39), B);  /* 00DA50 STA */
    wr16(DP(0x3A), 0x000B);  /* 00DA53 LDM */
    Y = 0x00A0;  /* 00DA57 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DA5A JSRL */
    mflag = 1;  /* 00DA5E SEM */
    wr8(DP(0x33), 0x00);  /* 00DA5F LDM */
L_00DA62: TICK();
    SETLO(A, rd8(DP(0x33)));  /* 00DA62 LDA */
    /* 00DA64 CMP */
    if ((A & 0xFF) >= 0x0A) goto L_00DA7E;  /* 00DA66 BCS */
    mflag = 0;  /* 00DA68 CLM */
    A &= 0x00FF;  /* 00DA69 AND */
    X = A;  /* 00DA6C TAX */
    A = rd16(DP(0x33));  /* 00DA6D LDA */
    A &= 0x00FF;  /* 00DA6F AND */
    Y = A;  /* 00DA72 TAY */
    A = rd16(DP(0x00 + X));  /* 00DA73 LDA */
    mflag = 1;  /* 00DA75 SEM */
    wr8((ABS(0x0336) + Y), (uint8_t)A);  /* 00DA76 STA */
    t = (rd8(DP(0x33)) + 1) & 0xFF; wr8(DP(0x33), t);  /* 00DA79 INC */
    goto L_00DA62;  /* 00DA7B JMP */
L_00DA7E: TICK();
    mflag = 0;  /* 00DA7E CLM */
    A = rd16(DP(0x0A));  /* 00DA7F LDA */
    mflag = 1;  /* 00DA81 SEM */
    wr8(DP(0x32), (uint8_t)A);  /* 00DA82 STA */
    wr8(DP(0x36), 0x16);  /* 00DA84 LDM */
    mflag = 0;  /* 00DA87 CLM */
    wr16(DP(0x37), 0x0340);  /* 00DA88 LDM */
    wr16(DP(0x39), 0x0000);  /* 00DA8C LDM */
    wr16(DP(0x3A), 0x0018);  /* 00DA90 LDM */
    Y = 0x00A0;  /* 00DA94 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DA97 JSRL */
    wr16(DP(0x36), 0x0032);  /* 00DA9B LDM */
    A = D;  /* 00DA9F TDA */
    B = 0x0000;  /* 00DAA0 LDA */
    wr16(DP(0x37), A);  /* 00DAA4 STA */
    wr16(DP(0x39), B);  /* 00DAA6 STA */
    wr16(DP(0x3A), 0x058B);  /* 00DAA9 LDM */
    Y = 0x00A0;  /* 00DAAD LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DAB0 JSRL */
    mflag = 1;  /* 00DAB4 SEM */
    if ((rd8(DP(0x00)) & 0x02) == 0) goto L_00DABE;  /* 00DAB5 BBC */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00DAC7;  /* 00DAB9 BBC */
L_00DABE: TICK();
    if ((rd8(DP(0x00)) & 0x02) == 0x02) goto L_00DAD3;  /* 00DABE BBS */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00DAD3;  /* 00DAC2 BBC */
L_00DAC7: TICK();
    Y = 0x0004;  /* 00DAC7 LDY */
    mflag = 0;  /* 00DACA CLM */
    S -= 3; F_003310();  /* 00DACB JSRL */
    S -= 3; F_01A60C();  /* 00DACF JSRL */
L_00DAD3: TICK();
    mflag = 0;  /* 00DAD3 CLM */
    A = rd16(DP(0x00));  /* 00DAD4 LDA */
    mflag = 1;  /* 00DAD6 SEM */
    wr8(ABS(0x0335), (uint8_t)A);  /* 00DAD7 STA */
    SETLO(A, rd8(DP(0x01)));  /* 00DADA LDA */
    mflag = 0;  /* 00DADC CLM */
    A &= 0x00FF;  /* 00DADD AND */
    A = rla16(A, 0x0008);  /* 00DAE0 RLA */
    A &= 0xFF00;  /* 00DAE4 AND */
    wr16(DP(0x36), A);  /* 00DAE7 STA */
    A = rd16(DP(0x02));  /* 00DAE9 LDA */
    A &= 0x00FF;  /* 00DAEB AND */
    C = 0;  /* 00DAEE CLC */
    A = adc16(A, rd16(DP(0x36)));  /* 00DAEF ADC */
    wr16(ABS(0x0224), A);  /* 00DAF1 STA */
    mflag = 1;  /* 00DAF4 SEM */
    SETLO(A, rd8(DP(0x04)));  /* 00DAF5 LDA */
    if ((A & 0xFF) == 0) goto L_00DB04;  /* 00DAF7 BEQ */
    mflag = 0;  /* 00DAF9 CLM */
    A = 0xFBD0;  /* 00DAFA LDA */
    B = 0x0001;  /* 00DAFD LDA */
    goto L_00DB0C;  /* 00DB01 JMP */
L_00DB04: TICK();
    mflag = 0;  /* 00DB04 CLM */
    A = 0xF400;  /* 00DB05 LDA */
    B = 0x0001;  /* 00DB08 LDA */
L_00DB0C: TICK();
    wr16(ABS(0x01F5), A);  /* 00DB0C STA */
    wr16(ABS(0x01F7), B);  /* 00DB0F STA */
    A = rd16(DP(0x05));  /* 00DB13 LDA */
    mflag = 1;  /* 00DB15 SEM */
    wr8(ABS(0x01FB), (uint8_t)A);  /* 00DB16 STA */
    SETLO(A, rd8(DP(0x24)));  /* 00DB19 LDA */
    wr8(ABS(0x01FC), (uint8_t)A);  /* 00DB1B STA */
    SETLO(A, rd8(DP(0x11)));  /* 00DB1E LDA */
    wr8(ABS(0x01FD), (uint8_t)A);  /* 00DB20 STA */
    SETLO(A, rd8(DP(0x2B)));  /* 00DB23 LDA */
    wr8(ABS(0x01FE), (uint8_t)A);  /* 00DB25 STA */
    SETLO(A, rd8(DP(0x2C)));  /* 00DB28 LDA */
    wr8(ABS(0x01FF), (uint8_t)A);  /* 00DB2A STA */
    SETLO(A, rd8(DP(0x2D)));  /* 00DB2D LDA */
    wr8(ABS(0x0204), (uint8_t)A);  /* 00DB2F STA */
    SETLO(A, rd8(DP(0x2E)));  /* 00DB32 LDA */
    wr8(ABS(0x0205), (uint8_t)A);  /* 00DB34 STA */
    SETLO(A, rd8(DP(0x30)));  /* 00DB37 LDA */
    wr8(ABS(0x0202), (uint8_t)A);  /* 00DB39 STA */
    SETLO(A, rd8(DP(0x31)));  /* 00DB3C LDA */
    wr8(ABS(0x0203), (uint8_t)A);  /* 00DB3E STA */
    SETLO(A, rd8(DP(0x2F)));  /* 00DB41 LDA */
    wr8(ABS(0x0200), (uint8_t)A);  /* 00DB43 STA */
    C = ((A & 0xFF) >= 0x07);  /* 00DB46 CMP */
    if (!C) goto L_00DB4F;  /* 00DB48 BCC */
    SETLO(A, adc8(A, 0xFA));  /* 00DB4A ADC */
    goto L_00DB51;  /* 00DB4C JMP */
L_00DB4F: TICK();
    SETLO(A, 0x01);  /* 00DB4F LDA */
L_00DB51: TICK();
    wr8(ABS(0x0201), (uint8_t)A);  /* 00DB51 STA */
    mflag = 0;  /* 00DB54 CLM */
    wr16(ABS(0x01F9), 0x4650);  /* 00DB55 LDM */
    X = rd16(ABS(0x0224));  /* 00DB5A LDX */
    /* 00DB5D CPX */
    if (X < 0x2711) goto L_00DB75;  /* 00DB60 BCC */
    mflag = 1;  /* 00DB62 SEM */
    wr8(ABS(0x020B), 0x80);  /* 00DB63 LDM */
    mflag = 0;  /* 00DB67 CLM */
    A = rd16(ABS(0x01F9));  /* 00DB68 LDA */
    mpy16(0xFFFF);  /* 00DB6B MPY */
    wr16(ABS(0x01F9), A);  /* 00DB6F STA */
    goto L_00DB7A;  /* 00DB72 JMP */
L_00DB75: TICK();
    mflag = 1;  /* 00DB75 SEM */
    wr8(ABS(0x020B), 0x40);  /* 00DB76 LDM */
L_00DB7A: TICK();
    mflag = 0;  /* 00DB7A CLM */
    wr16(DP(0x34), 0x0000);  /* 00DB7B LDM */
    mflag = 1;  /* 00DB7F SEM */
    if ((rd8(DP(0x28)) & 0x04) == 0x04) goto L_00DB89;  /* 00DB80 BBS */
    mflag = 0;  /* 00DB84 CLM */
    wr16(DP(0x34), rd16(DP(0x34)) | 0x1000);  /* 00DB85 SEB */
L_00DB89: TICK();
    mflag = 0;  /* 00DB89 CLM */
    A = rd16(DP(0x26));  /* 00DB8A LDA */
    mflag = 1;  /* 00DB8C SEM */
    SETLO(A, A & 0x0F);  /* 00DB8D AND */
    wr8(DP(0x34), rd8(DP(0x34)) & ~0x0F);  /* 00DB8F CLB */
    SETLO(A, A | rd8(DP(0x34)));  /* 00DB92 ORA */
    wr8(DP(0x34), (uint8_t)A);  /* 00DB94 STA */
    SETLO(A, rd8(DP(0x29)));  /* 00DB96 LDA */
    SETLO(A, A & 0x07);  /* 00DB98 AND */
    wr8(DP(0x35), rd8(DP(0x35)) & ~0x07);  /* 00DB9A CLB */
    SETLO(A, A | rd8(DP(0x35)));  /* 00DB9D ORA */
    wr8(DP(0x35), (uint8_t)A);  /* 00DB9F STA */
    mflag = 0;  /* 00DBA1 CLM */
    X = rd16(DP(0x34));  /* 00DBA2 LDX */
    wr16(ABS(0x0298), X);  /* 00DBA4 STX */
    wr16(DP(0x34), 0x0000);  /* 00DBA7 LDM */
    mflag = 1;  /* 00DBAB SEM */
    if ((rd8(DP(0x28)) & 0x01) == 0) goto L_00DBB5;  /* 00DBAC BBC */
    mflag = 0;  /* 00DBB0 CLM */
    wr16(DP(0x34), rd16(DP(0x34)) | 0x2000);  /* 00DBB1 SEB */
L_00DBB5: TICK();
    mflag = 1;  /* 00DBB5 SEM */
    if ((rd8(DP(0x28)) & 0x02) == 0) goto L_00DBBF;  /* 00DBB6 BBC */
    mflag = 0;  /* 00DBBA CLM */
    wr16(DP(0x34), rd16(DP(0x34)) | 0x4000);  /* 00DBBB SEB */
L_00DBBF: TICK();
    mflag = 1;  /* 00DBBF SEM */
    if ((rd8(DP(0x28)) & 0x08) == 0) goto L_00DBC9;  /* 00DBC0 BBC */
    mflag = 0;  /* 00DBC4 CLM */
    wr16(DP(0x34), rd16(DP(0x34)) | 0x0020);  /* 00DBC5 SEB */
L_00DBC9: TICK();
    mflag = 0;  /* 00DBC9 CLM */
    wr16(DP(0x34), rd16(DP(0x34)) | 0x0010);  /* 00DBCA SEB */
    wr16(DP(0x34), rd16(DP(0x34)) | 0x0040);  /* 00DBCE SEB */
    A = rd16(DP(0x27));  /* 00DBD2 LDA */
    mflag = 1;  /* 00DBD4 SEM */
    SETLO(A, A & 0x0F);  /* 00DBD5 AND */
    wr8(DP(0x34), rd8(DP(0x34)) & ~0x0F);  /* 00DBD7 CLB */
    SETLO(A, A | rd8(DP(0x34)));  /* 00DBDA ORA */
    wr8(DP(0x34), (uint8_t)A);  /* 00DBDC STA */
    SETLO(A, rd8(DP(0x2A)));  /* 00DBDE LDA */
    SETLO(A, A & 0x07);  /* 00DBE0 AND */
    wr8(DP(0x35), rd8(DP(0x35)) & ~0x07);  /* 00DBE2 CLB */
    SETLO(A, A | rd8(DP(0x35)));  /* 00DBE5 ORA */
    wr8(DP(0x35), (uint8_t)A);  /* 00DBE7 STA */
    mflag = 0;  /* 00DBE9 CLM */
    X = rd16(DP(0x34));  /* 00DBEA LDX */
    wr16(ABS(0x029A), X);  /* 00DBEC STX */
    mflag = 1;  /* 00DBEF SEM */
    SETLO(B, 0x00);  /* 00DBF0 LDA */
    SETLO(A, rd8(DP(0x32)));  /* 00DBF3 LDA */
    div8(0x10);  /* 00DBF5 DIV */
    wr8(DP(0x33), (uint8_t)A);  /* 00DBF8 STA */
    /* 00DBFA CMP */
    if ((A & 0xFF) < 0x0A) goto L_00DC01;  /* 00DBFC BCC */
    wr8(DP(0x33), 0x09);  /* 00DBFE LDM */
L_00DC01: TICK();
    wr8(DP(0x32), rd8(DP(0x32)) & ~0xF0);  /* 00DC01 CLB */
    SETLO(A, rd8(DP(0x32)));  /* 00DC04 LDA */
    /* 00DC06 CMP */
    if ((A & 0xFF) < 0x0A) goto L_00DC0D;  /* 00DC08 BCC */
    wr8(DP(0x32), 0x09);  /* 00DC0A LDM */
L_00DC0D: TICK();
    SETLO(A, rd8(DP(0x32)));  /* 00DC0D LDA */
    mflag = 0;  /* 00DC0F CLM */
    A &= 0x00FF;  /* 00DC10 AND */
    C = 0;  /* 00DC13 CLC */
    A = adc16(A, 0x0006);  /* 00DC14 ADC */
    X = A;  /* 00DC17 TAX */
    A = rd16(DP(0x00 + X));  /* 00DC18 LDA */
    mflag = 1;  /* 00DC1A SEM */
    wr8(ABS(0x0277), (uint8_t)A);  /* 00DC1B STA */
    SETLO(A, rd8(DP(0x33)));  /* 00DC1E LDA */
    mflag = 0;  /* 00DC20 CLM */
    A &= 0x00FF;  /* 00DC21 AND */
    C = 0;  /* 00DC24 CLC */
    A = adc16(A, 0x0006);  /* 00DC25 ADC */
    X = A;  /* 00DC28 TAX */
    A = rd16(DP(0x00 + X));  /* 00DC29 LDA */
    mflag = 1;  /* 00DC2B SEM */
    wr8(ABS(0x0278), (uint8_t)A);  /* 00DC2C STA */
    SETLO(A, rd8(DP(0x0D)));  /* 00DC2F LDA */
    wr8(ABS(0x0279), (uint8_t)A);  /* 00DC31 STA */
    SETLO(A, rd8(DP(0x32)));  /* 00DC34 LDA */
    mflag = 0;  /* 00DC36 CLM */
    A &= 0x00FF;  /* 00DC37 AND */
    C = 0;  /* 00DC3A CLC */
    A = adc16(A, 0x0010);  /* 00DC3B ADC */
    X = A;  /* 00DC3E TAX */
    A = rd16(DP(0x00 + X));  /* 00DC3F LDA */
    mflag = 1;  /* 00DC41 SEM */
    wr8(ABS(0x027A), (uint8_t)A);  /* 00DC42 STA */
    SETLO(A, rd8(DP(0x33)));  /* 00DC45 LDA */
    mflag = 0;  /* 00DC47 CLM */
    A &= 0x00FF;  /* 00DC48 AND */
    C = 0;  /* 00DC4B CLC */
    A = adc16(A, 0x0010);  /* 00DC4C ADC */
    X = A;  /* 00DC4F TAX */
    A = rd16(DP(0x00 + X));  /* 00DC50 LDA */
    mflag = 1;  /* 00DC52 SEM */
    wr8(ABS(0x027B), (uint8_t)A);  /* 00DC53 STA */
    SETLO(A, rd8(DP(0x17)));  /* 00DC56 LDA */
    wr8(ABS(0x027C), (uint8_t)A);  /* 00DC58 STA */
    SETLO(A, rd8(DP(0x32)));  /* 00DC5B LDA */
    mflag = 0;  /* 00DC5D CLM */
    A &= 0x00FF;  /* 00DC5E AND */
    C = 0;  /* 00DC61 CLC */
    A = adc16(A, 0x001A);  /* 00DC62 ADC */
    X = A;  /* 00DC65 TAX */
    A = rd16(DP(0x00 + X));  /* 00DC66 LDA */
    mflag = 1;  /* 00DC68 SEM */
    wr8(ABS(0x027D), (uint8_t)A);  /* 00DC69 STA */
    SETLO(A, rd8(DP(0x33)));  /* 00DC6C LDA */
    mflag = 0;  /* 00DC6E CLM */
    A &= 0x00FF;  /* 00DC6F AND */
    C = 0;  /* 00DC72 CLC */
    A = adc16(A, 0x001A);  /* 00DC73 ADC */
    X = A;  /* 00DC76 TAX */
    A = rd16(DP(0x00 + X));  /* 00DC77 LDA */
    mflag = 1;  /* 00DC79 SEM */
    wr8(ABS(0x027E), (uint8_t)A);  /* 00DC7A STA */
    SETLO(A, rd8(DP(0x21)));  /* 00DC7D LDA */
    wr8(ABS(0x027F), (uint8_t)A);  /* 00DC7F STA */
    wr8(DP(0x36), 0x17);  /* 00DC82 LDM */
    mflag = 0;  /* 00DC85 CLM */
    A = D;  /* 00DC86 TDA */
    B = 0x0000;  /* 00DC87 LDA */
    wr16(DP(0x37), A);  /* 00DC8B STA */
    wr16(DP(0x39), B);  /* 00DC8D STA */
    wr16(DP(0x3A), 0x05BF);  /* 00DC90 LDM */
    Y = 0x00A0;  /* 00DC94 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DC97 JSRL */
    A = rd16(DP(0x32));  /* 00DC9B LDA */
    A &= 0x00FF;  /* 00DC9D AND */
    X = A;  /* 00DCA0 TAX */
    A = rd16(DP(0x00 + X));  /* 00DCA1 LDA */
    mflag = 1;  /* 00DCA3 SEM */
    wr8(ABS(0x0181), (uint8_t)A);  /* 00DCA4 STA */
    SETLO(A, rd8(DP(0x33)));  /* 00DCA7 LDA */
    mflag = 0;  /* 00DCA9 CLM */
    A &= 0x00FF;  /* 00DCAA AND */
    X = A;  /* 00DCAD TAX */
    A = rd16(DP(0x00 + X));  /* 00DCAE LDA */
    mflag = 1;  /* 00DCB0 SEM */
    wr8(ABS(0x0182), (uint8_t)A);  /* 00DCB1 STA */
    SETLO(A, rd8(DP(0x07)));  /* 00DCB4 LDA */
    wr8(ABS(0x0183), (uint8_t)A);  /* 00DCB6 STA */
    SETLO(A, rd8(DP(0x32)));  /* 00DCB9 LDA */
    mflag = 0;  /* 00DCBB CLM */
    A &= 0x00FF;  /* 00DCBC AND */
    C = 0;  /* 00DCBF CLC */
    A = adc16(A, 0x000A);  /* 00DCC0 ADC */
    X = A;  /* 00DCC3 TAX */
    A = rd16(DP(0x00 + X));  /* 00DCC4 LDA */
    mflag = 1;  /* 00DCC6 SEM */
    wr8(ABS(0x0185), (uint8_t)A);  /* 00DCC7 STA */
    SETLO(A, rd8(DP(0x33)));  /* 00DCCA LDA */
    mflag = 0;  /* 00DCCC CLM */
    A &= 0x00FF;  /* 00DCCD AND */
    C = 0;  /* 00DCD0 CLC */
    A = adc16(A, 0x000A);  /* 00DCD1 ADC */
    X = A;  /* 00DCD4 TAX */
    A = rd16(DP(0x00 + X));  /* 00DCD5 LDA */
    mflag = 1;  /* 00DCD7 SEM */
    wr8(ABS(0x0186), (uint8_t)A);  /* 00DCD8 STA */
    SETLO(A, rd8(DP(0x11)));  /* 00DCDB LDA */
    wr8(ABS(0x0187), (uint8_t)A);  /* 00DCDD STA */
    SETLO(A, rd8(DP(0x14)));  /* 00DCE0 LDA */
    wr8(ABS(0x0184), (uint8_t)A);  /* 00DCE2 STA */
    SETLO(A, rd8(DP(0x15)));  /* 00DCE5 LDA */
    wr8(ABS(0x0280), (uint8_t)A);  /* 00DCE7 STA */
    SETLO(A, rd8(DP(0x16)));  /* 00DCEA LDA */
    mpy8(0x05);  /* 00DCEC MPY */
    wr8(DP(0x36), (uint8_t)A);  /* 00DCEF STA */
    wr8(DP(0x37), (uint8_t)B);  /* 00DCF1 STA */
    mflag = 0;  /* 00DCF4 CLM */
    X = rd16(DP(0x36));  /* 00DCF5 LDX */
    wr16(ABS(0x0281), X);  /* 00DCF7 STX */
    Y = pull16();  /* 00DCFA PLY */
    D = pull16();  /* 00DCFB PLD */
    S += 3; return;  /* 00DCFC RTL */
}

/* 00DCFD  callers: 00E498 */
void F_00DCFD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00DCFD LDA */
    push16(D);  /* 00DCFF PHD */
    D = A;  /* 00DD00 TAD */
    C = 0;  /* 00DD01 CLC */
    A = adc16(A, 0x0008);  /* 00DD02 ADC */
    push16(A);  /* 00DD05 PHA */
    wr16(DP(0x06), 0x0000);  /* 00DD06 LDM */
    wr16(DP(0x02), 0x0000);  /* 00DD0A LDM */
    wr16(DP(0x08), 0x0047);  /* 00DD0E LDM */
    wr16(DP(0x09), 0x02E2);  /* 00DD12 LDM */
    wr16(DP(0x0B), 0x0000);  /* 00DD16 LDM */
    wr16(DP(0x0C), 0x0578);  /* 00DD1A LDM */
    Y = 0x00A0;  /* 00DD1E LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DD21 JSRL */
    wr16(DP(0x00), 0x0000);  /* 00DD25 LDM */
L_00DD29: TICK();
    X = rd16(DP(0x00));  /* 00DD29 LDX */
    C = (X >= 0x0045);  /* 00DD2B CPX */
    if (C) goto L_00DD40;  /* 00DD2E BCS */
    A = rd16((ABS(0x02E2) + X));  /* 00DD30 LDA */
    A &= 0x00FF;  /* 00DD33 AND */
    A = adc16(A, rd16(DP(0x02)));  /* 00DD36 ADC */
    wr16(DP(0x02), A);  /* 00DD38 STA */
    X += 1;  /* 00DD3A INX */
    wr16(DP(0x00), X);  /* 00DD3B STX */
    goto L_00DD29;  /* 00DD3D JMP */
L_00DD40: TICK();
    mflag = 0;  /* 00DD40 CLM */
    A = rd16(ABS(0x0327));  /* 00DD41 LDA */
    A &= 0x00FF;  /* 00DD44 AND */
    A = rla16(A, 0x0008);  /* 00DD47 RLA */
    A &= 0xFF00;  /* 00DD4B AND */
    wr16(DP(0x08), A);  /* 00DD4E STA */
    A = rd16(ABS(0x0328));  /* 00DD50 LDA */
    A &= 0x00FF;  /* 00DD53 AND */
    C = 0;  /* 00DD56 CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 00DD57 ADC */
    t = rd16(DP(0x02));  /* 00DD59 CMP */
    if (A == t) goto L_00DD61;  /* 00DD5B BEQ */
    wr16(DP(0x06), 0x0001);  /* 00DD5D LDM */
L_00DD61: TICK();
    X = rd16(DP(0x06));  /* 00DD61 LDX */
    if (X == 0) goto L_00DD68;  /* 00DD63 BEQ */
    goto L_00DEC0;  /* 00DD65 JMP */
L_00DD68: TICK();
    wr16(DP(0x02), X);  /* 00DD68 STX */
    wr16(DP(0x08), 0x0007);  /* 00DD6A LDM */
    wr16(DP(0x09), 0x02E2);  /* 00DD6E LDM */
    wr16(DP(0x0B), X);  /* 00DD72 STX */
    wr16(DP(0x0C), X);  /* 00DD74 STX */
    Y = 0x00A0;  /* 00DD76 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DD79 JSRL */
    wr16(DP(0x00), 0x0000);  /* 00DD7D LDM */
L_00DD81: TICK();
    X = rd16(DP(0x00));  /* 00DD81 LDX */
    C = (X >= 0x0005);  /* 00DD83 CPX */
    if (C) goto L_00DD98;  /* 00DD86 BCS */
    A = rd16((ABS(0x02E2) + X));  /* 00DD88 LDA */
    A &= 0x00FF;  /* 00DD8B AND */
    A = adc16(A, rd16(DP(0x02)));  /* 00DD8E ADC */
    wr16(DP(0x02), A);  /* 00DD90 STA */
    X += 1;  /* 00DD92 INX */
    wr16(DP(0x00), X);  /* 00DD93 STX */
    goto L_00DD81;  /* 00DD95 JMP */
L_00DD98: TICK();
    mflag = 0;  /* 00DD98 CLM */
    A = rd16(ABS(0x02E7));  /* 00DD99 LDA */
    A &= 0x00FF;  /* 00DD9C AND */
    A = rla16(A, 0x0008);  /* 00DD9F RLA */
    A &= 0xFF00;  /* 00DDA3 AND */
    wr16(DP(0x08), A);  /* 00DDA6 STA */
    A = rd16(ABS(0x02E8));  /* 00DDA8 LDA */
    A &= 0x00FF;  /* 00DDAB AND */
    C = 0;  /* 00DDAE CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 00DDAF ADC */
    t = rd16(DP(0x02));  /* 00DDB1 CMP */
    if (A == t) goto L_00DDB9;  /* 00DDB3 BEQ */
    wr16(DP(0x06), 0x0002);  /* 00DDB5 LDM */
L_00DDB9: TICK();
    X = rd16(DP(0x06));  /* 00DDB9 LDX */
    if (X == 0) goto L_00DDC0;  /* 00DDBB BEQ */
    goto L_00DEC0;  /* 00DDBD JMP */
L_00DDC0: TICK();
    wr16(DP(0x02), X);  /* 00DDC0 STX */
    wr16(DP(0x08), 0x0010);  /* 00DDC2 LDM */
    wr16(DP(0x09), 0x02E2);  /* 00DDC6 LDM */
    wr16(DP(0x0B), X);  /* 00DDCA STX */
    wr16(DP(0x0C), 0x0008);  /* 00DDCC LDM */
    Y = 0x00A0;  /* 00DDD0 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DDD3 JSRL */
    wr16(DP(0x00), 0x0000);  /* 00DDD7 LDM */
L_00DDDB: TICK();
    X = rd16(DP(0x00));  /* 00DDDB LDX */
    C = (X >= 0x000E);  /* 00DDDD CPX */
    if (C) goto L_00DDF2;  /* 00DDE0 BCS */
    A = rd16((ABS(0x02E2) + X));  /* 00DDE2 LDA */
    A &= 0x00FF;  /* 00DDE5 AND */
    A = adc16(A, rd16(DP(0x02)));  /* 00DDE8 ADC */
    wr16(DP(0x02), A);  /* 00DDEA STA */
    X += 1;  /* 00DDEC INX */
    wr16(DP(0x00), X);  /* 00DDED STX */
    goto L_00DDDB;  /* 00DDEF JMP */
L_00DDF2: TICK();
    mflag = 0;  /* 00DDF2 CLM */
    A = rd16(ABS(0x02F0));  /* 00DDF3 LDA */
    A &= 0x00FF;  /* 00DDF6 AND */
    A = rla16(A, 0x0008);  /* 00DDF9 RLA */
    A &= 0xFF00;  /* 00DDFD AND */
    wr16(DP(0x08), A);  /* 00DE00 STA */
    A = rd16(ABS(0x02F1));  /* 00DE02 LDA */
    A &= 0x00FF;  /* 00DE05 AND */
    C = 0;  /* 00DE08 CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 00DE09 ADC */
    t = rd16(DP(0x02));  /* 00DE0B CMP */
    if (A == t) goto L_00DE13;  /* 00DE0D BEQ */
    wr16(DP(0x06), 0x0003);  /* 00DE0F LDM */
L_00DE13: TICK();
    X = rd16(DP(0x06));  /* 00DE13 LDX */
    if (X == 0) goto L_00DE1A;  /* 00DE15 BEQ */
    goto L_00DEC0;  /* 00DE17 JMP */
L_00DE1A: TICK();
    wr16(DP(0x02), X);  /* 00DE1A STX */
    wr16(DP(0x04), X);  /* 00DE1C STX */
L_00DE1E: TICK();
    X = rd16(DP(0x04));  /* 00DE1E LDX */
    /* 00DE20 CPX */
    if (X >= 0x0010) goto L_00DE64;  /* 00DE23 BCS */
    wr16(DP(0x08), 0x0050);  /* 00DE25 LDM */
    wr16(DP(0x09), 0x02E2);  /* 00DE29 LDM */
    wr16(DP(0x0B), 0x0000);  /* 00DE2D LDM */
    A = X;  /* 00DE31 TXA */
    mpy16(0x0050);  /* 00DE32 MPY */
    C = 0;  /* 00DE36 CLC */
    A = adc16(A, 0x0018);  /* 00DE37 ADC */
    wr16(DP(0x0C), A);  /* 00DE3A STA */
    Y = 0x00A0;  /* 00DE3C LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DE3F JSRL */
    wr16(DP(0x00), 0x0000);  /* 00DE43 LDM */
L_00DE47: TICK();
    X = rd16(DP(0x00));  /* 00DE47 LDX */
    C = (X >= 0x0050);  /* 00DE49 CPX */
    if (C) goto L_00DE5E;  /* 00DE4C BCS */
    A = rd16((ABS(0x02E2) + X));  /* 00DE4E LDA */
    A &= 0x00FF;  /* 00DE51 AND */
    A = adc16(A, rd16(DP(0x02)));  /* 00DE54 ADC */
    wr16(DP(0x02), A);  /* 00DE56 STA */
    X += 1;  /* 00DE58 INX */
    wr16(DP(0x00), X);  /* 00DE59 STX */
    goto L_00DE47;  /* 00DE5B JMP */
L_00DE5E: TICK();
    mflag = 0;  /* 00DE5E CLM */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 00DE5F INC */
    goto L_00DE1E;  /* 00DE61 JMP */
L_00DE64: TICK();
    mflag = 0;  /* 00DE64 CLM */
    wr16(DP(0x08), 0x001F);  /* 00DE65 LDM */
    wr16(DP(0x09), 0x02E2);  /* 00DE69 LDM */
    wr16(DP(0x0B), 0x0000);  /* 00DE6D LDM */
    A = rd16(DP(0x04));  /* 00DE71 LDA */
    mpy16(0x0050);  /* 00DE73 MPY */
    C = 0;  /* 00DE77 CLC */
    A = adc16(A, 0x0018);  /* 00DE78 ADC */
    wr16(DP(0x0C), A);  /* 00DE7B STA */
    Y = 0x00A0;  /* 00DE7D LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00DE80 JSRL */
    wr16(DP(0x00), 0x0000);  /* 00DE84 LDM */
L_00DE88: TICK();
    X = rd16(DP(0x00));  /* 00DE88 LDX */
    C = (X >= 0x001D);  /* 00DE8A CPX */
    if (C) goto L_00DE9F;  /* 00DE8D BCS */
    A = rd16((ABS(0x02E2) + X));  /* 00DE8F LDA */
    A &= 0x00FF;  /* 00DE92 AND */
    A = adc16(A, rd16(DP(0x02)));  /* 00DE95 ADC */
    wr16(DP(0x02), A);  /* 00DE97 STA */
    X += 1;  /* 00DE99 INX */
    wr16(DP(0x00), X);  /* 00DE9A STX */
    goto L_00DE88;  /* 00DE9C JMP */
L_00DE9F: TICK();
    mflag = 0;  /* 00DE9F CLM */
    A = rd16(ABS(0x02FF));  /* 00DEA0 LDA */
    A &= 0x00FF;  /* 00DEA3 AND */
    A = rla16(A, 0x0008);  /* 00DEA6 RLA */
    A &= 0xFF00;  /* 00DEAA AND */
    wr16(DP(0x08), A);  /* 00DEAD STA */
    A = rd16(ABS(0x0300));  /* 00DEAF LDA */
    A &= 0x00FF;  /* 00DEB2 AND */
    C = 0;  /* 00DEB5 CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 00DEB6 ADC */
    t = rd16(DP(0x02));  /* 00DEB8 CMP */
    if (A == t) goto L_00DEC0;  /* 00DEBA BEQ */
    wr16(DP(0x06), 0x0004);  /* 00DEBC LDM */
L_00DEC0: TICK();
    X = rd16(DP(0x06));  /* 00DEC0 LDX */
    if (X == 0) goto L_00DECF;  /* 00DEC2 BEQ */
    Y = 0x0003;  /* 00DEC4 LDY */
    S -= 3; F_003310();  /* 00DEC7 JSRL */
    S -= 3; F_01A60C();  /* 00DECB JSRL */
L_00DECF: TICK();
    Y = pull16();  /* 00DECF PLY */
    D = pull16();  /* 00DED0 PLD */
    S += 3; return;  /* 00DED1 RTL */
}

/* 00DED2  callers: 00DF62, 00DF6A, 00E04C, 00E054 */
void F_00DED2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00DED2 LDA */
    push16(D);  /* 00DED4 PHD */
    D = A;  /* 00DED5 TAD */
    wr16(DP(0x00), Y);  /* 00DED6 STY */
    mflag = 1;  /* 00DED8 SEM */
    wr8(DP(0x01), 0x07);  /* 00DED9 LDM */
L_00DEDC: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 00DEDC LDA */
    mflag = 0;  /* 00DEDE CLM */
    A &= 0x00FF;  /* 00DEDF AND */
    X = A;  /* 00DEE2 TAX */
    A = 0x0001;  /* 00DEE3 LDA */
    mflag = 1;  /* 00DEE6 SEM */
    S -= 3; rt_shl_01E4E0();  /* 00DEE7 JSRL */
    SETLO(A, A & rd8(DP(0x00)));  /* 00DEEB AND */
    if ((A & 0xFF) == 0) goto L_00DEF6;  /* 00DEED BEQ */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 00DEEF CLB [P62 GOSTERGE_VERI(SDA) yon] */
    goto L_00DEFF;  /* 00DEF3 JMP */
L_00DEF6: TICK();
    mflag = 1;  /* 00DEF6 SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 00DEF7 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 00DEFB SEB [P62 GOSTERGE_VERI(SDA) yon] */
L_00DEFF: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 00DEFF SEB [P61 GOSTERGE_SAAT(SCL)] */
    t = (rd8(DP(0x01)) - 1) & 0xFF; wr8(DP(0x01), t);  /* 00DF03 DEC */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 00DF05 CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(DP(0x01)));  /* 00DF09 LDA */
    if ((A & 0x80) == 0) goto L_00DEDC;  /* 00DF0B BPL */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 00DF0D CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(0x0211), 0x00);  /* 00DF11 LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 00DF15 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(0x0211), 0x00);  /* 00DF19 LDM */
    SETLO(A, rd8(ABS(P6)));  /* 00DF1D LDA */
    SETLO(A, A & 0x04);  /* 00DF20 AND */
    SETLO(A, lsr8(A));  /* 00DF22 LSR */
    SETLO(A, lsr8(A));  /* 00DF23 LSR */
    wr8(DP(0x02), (uint8_t)A);  /* 00DF24 STA */
    SETLO(A, rd8(ABS(P6)));  /* 00DF26 LDA */
    SETLO(A, A & 0x01);  /* 00DF29 AND */
    wr8(DP(0x03), (uint8_t)A);  /* 00DF2B STA */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 00DF2D CLB [P61 GOSTERGE_SAAT(SCL)] */
    SETLO(A, rd8(DP(0x02)));  /* 00DF31 LDA */
    if ((A & 0xFF) != 0) goto L_00DF45;  /* 00DF33 BNE */
    SETLO(A, rd8(DP(0x03)));  /* 00DF35 LDA */
    if ((A & 0xFF) == 0) goto L_00DF3E;  /* 00DF37 BEQ */
    SETLO(A, 0x01);  /* 00DF39 LDA */
    goto L_00DF40;  /* 00DF3B JMP */
L_00DF3E: TICK();
    SETLO(A, 0x00);  /* 00DF3E LDA */
L_00DF40: TICK();
    wr8(DP(0x01), (uint8_t)A);  /* 00DF40 STA */
    goto L_00DF53;  /* 00DF42 JMP */
L_00DF45: TICK();
    mflag = 1;  /* 00DF45 SEM */
    SETLO(A, rd8(DP(0x03)));  /* 00DF46 LDA */
    if ((A & 0xFF) == 0) goto L_00DF4F;  /* 00DF48 BEQ */
    SETLO(A, 0x03);  /* 00DF4A LDA */
    goto L_00DF51;  /* 00DF4C JMP */
L_00DF4F: TICK();
    SETLO(A, 0x02);  /* 00DF4F LDA */
L_00DF51: TICK();
    wr8(DP(0x01), (uint8_t)A);  /* 00DF51 STA */
L_00DF53: TICK();
    D = pull16();  /* 00DF53 PLD */
    S += 3; return;  /* 00DF54 RTL */
}

/* 00DF55  callers: 00E378 */
void F_00DF55(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00DF55 LDA */
    push16(D);  /* 00DF57 PHD */
    D = A;  /* 00DF58 TAD */
    A += 1;  /* 00DF59 INC */
    push16(A);  /* 00DF5A PHA */
    S -= 3; i2c_start_007172();  /* 00DF5B JSRL */
    Y = 0x0042;  /* 00DF5F LDY */
    S -= 3; F_00DED2();  /* 00DF62 JSRL */
    Y = 0x00F0;  /* 00DF66 LDY */
    mflag = 0;  /* 00DF69 CLM */
    S -= 3; F_00DED2();  /* 00DF6A JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00DF6E STA */
    mflag = 0;  /* 00DF70 CLM */
    S -= 3; i2c_stop_00715B();  /* 00DF71 JSRL */
    mflag = 1;  /* 00DF75 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00DF76 LDA */
    if ((A & 0xFF) != 0) goto L_00DFAD;  /* 00DF78 BNE */
    wr8(ABS(P6D), 0x03);  /* 00DF7A LDM */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x01);  /* 00DF7E SEB [P60 UZAK_VERI_GIRIS] */
    mflag = 0;  /* 00DF82 CLM */
    S -= 3; F_006A67();  /* 00DF83 JSRL */
    mflag = 1;  /* 00DF87 SEM */
    SETLO(A, rd8(ABS(P6)));  /* 00DF88 LDA */
    SETLO(A, A & 0x04);  /* 00DF8B AND */
    SETLO(A, lsr8(A));  /* 00DF8D LSR */
    SETLO(A, lsr8(A));  /* 00DF8E LSR */
    if (Z) goto L_00DF94;  /* 00DF8F BEQ */
    wr8(DP(0x00), 0x02);  /* 00DF91 LDM */
L_00DF94: TICK();
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x01);  /* 00DF94 CLB [P60 UZAK_VERI_GIRIS] */
    mflag = 0;  /* 00DF98 CLM */
    S -= 3; F_006A67();  /* 00DF99 JSRL */
    mflag = 1;  /* 00DF9D SEM */
    SETLO(A, rd8(ABS(P6)));  /* 00DF9E LDA */
    SETLO(A, A & 0x04);  /* 00DFA1 AND */
    SETLO(A, lsr8(A));  /* 00DFA3 LSR */
    SETLO(A, lsr8(A));  /* 00DFA4 LSR */
    if (!Z) goto L_00DFAA;  /* 00DFA5 BNE */
    wr8(DP(0x00), 0x03);  /* 00DFA7 LDM */
L_00DFAA: TICK();
    goto L_00E03C;  /* 00DFAA JMP */
L_00DFAD: TICK();
    mflag = 0;  /* 00DFAD CLM */
    A = rd16(DP(0x00));  /* 00DFAE LDA */
    mflag = 1;  /* 00DFB0 SEM */
    /* 00DFB1 CMP */
    if ((A & 0xFF) != 0x01) goto L_00DFBB;  /* 00DFB3 BNE */
    wr8(DP(0x00), 0x02);  /* 00DFB5 LDM */
    goto L_00E03C;  /* 00DFB8 JMP */
L_00DFBB: TICK();
    mflag = 0;  /* 00DFBB CLM */
    A = rd16(DP(0x00));  /* 00DFBC LDA */
    mflag = 1;  /* 00DFBE SEM */
    C = ((A & 0xFF) >= 0x03);  /* 00DFBF CMP */
    Z = ((A & 0xFF) == 0x03);
    if (!Z) goto L_00DFD7;  /* 00DFC1 BNE */
    SETLO(A, rd8(ABS(P6)));  /* 00DFC3 LDA */
    SETLO(A, A & 0x01);  /* 00DFC6 AND */
    if ((A & 0xFF) == 0) goto L_00DFD0;  /* 00DFC8 BEQ */
    wr8(DP(0x00), 0x01);  /* 00DFCA LDM */
    goto L_00DFD4;  /* 00DFCD JMP */
L_00DFD0: TICK();
    mflag = 1;  /* 00DFD0 SEM */
    wr8(DP(0x00), 0x00);  /* 00DFD1 LDM */
L_00DFD4: TICK();
    goto L_00E03C;  /* 00DFD4 JMP */
L_00DFD7: TICK();
    mflag = 1;  /* 00DFD7 SEM */
    wr8(ABS(0x0335), 0x02);  /* 00DFD8 LDM */
    SETLO(A, 0x01);  /* 00DFDC LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00DFDE PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00DFDF STA */
    setP(pull16());  /* 00DFE1 PLP */
    mflag = 0;  /* 00DFE2 CLM */
    S -= 3; i2c_start_007172();  /* 00DFE3 JSRL */
    Y = 0x0042;  /* 00DFE7 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00DFEA JSRL */
    Y = 0x00F0;  /* 00DFEE LDY */
    mflag = 0;  /* 00DFF1 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00DFF2 JSRL */
    mflag = 0;  /* 00DFF6 CLM */
    S -= 3; i2c_stop_00715B();  /* 00DFF7 JSRL */
    S -= 3; i2c_start_007172();  /* 00DFFB JSRL */
    Y = 0x0043;  /* 00DFFF LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00E002 JSRL */
    Y = 0x0001;  /* 00E006 LDY */
    mflag = 0;  /* 00E009 CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 00E00A JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00E00E STA */
    mflag = 0;  /* 00E010 CLM */
    S -= 3; i2c_stop_00715B();  /* 00E011 JSRL */
    mflag = 1;  /* 00E015 SEM */
    SETLO(A, 0x00);  /* 00E016 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00E018 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00E019 STA */
    setP(pull16());  /* 00E01B PLP */
    mflag = 0;  /* 00E01C CLM */
    A = rd16(DP(0x00));  /* 00E01D LDA */
    mflag = 1;  /* 00E01F SEM */
    /* 00E020 CMP */
    if ((A & 0xFF) != 0xF0) goto L_00E02A;  /* 00E022 BNE */
    wr8(DP(0x00), 0x04);  /* 00E024 LDM */
    goto L_00E03C;  /* 00E027 JMP */
L_00E02A: TICK();
    mflag = 1;  /* 00E02A SEM */
    SETLO(A, rd8(ABS(P6)));  /* 00E02B LDA */
    SETLO(A, A & 0x01);  /* 00E02E AND */
    if ((A & 0xFF) == 0) goto L_00E038;  /* 00E030 BEQ */
    wr8(DP(0x00), 0x05);  /* 00E032 LDM */
    goto L_00E03C;  /* 00E035 JMP */
L_00E038: TICK();
    mflag = 1;  /* 00E038 SEM */
    wr8(DP(0x00), 0x00);  /* 00E039 LDM */
L_00E03C: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00E03C LDA */
    if ((A & 0xFF) != 0) goto L_00E079;  /* 00E03E BNE */
    wr8(ABS(P6D), 0x02);  /* 00E040 LDM */
    mflag = 0;  /* 00E044 CLM */
    S -= 3; i2c_start_007172();  /* 00E045 JSRL */
    Y = 0x0046;  /* 00E049 LDY */
    S -= 3; F_00DED2();  /* 00E04C JSRL */
    Y = 0x00FC;  /* 00E050 LDY */
    mflag = 0;  /* 00E053 CLM */
    S -= 3; F_00DED2();  /* 00E054 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00E058 STA */
    mflag = 0;  /* 00E05A CLM */
    S -= 3; i2c_stop_00715B();  /* 00E05B JSRL */
    A = rd16(DP(0x00));  /* 00E05F LDA */
    mflag = 1;  /* 00E061 SEM */
    /* 00E062 CMP */
    if ((A & 0xFF) >= 0x02) goto L_00E06B;  /* 00E064 BCS */
    SETLO(A, 0x02);  /* 00E066 LDA */
    goto L_00E06D;  /* 00E068 JMP */
L_00E06B: TICK();
    SETLO(A, 0x00);  /* 00E06B LDA */
L_00E06D: TICK();
    wr8(DP(0x00), (uint8_t)A);  /* 00E06D STA */
    /* 00E06F CMP */
    if ((A & 0xFF) != 0x02) goto L_00E079;  /* 00E071 BNE */
    mflag = 0;  /* 00E073 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0800);  /* 00E074 SEB */
L_00E079: TICK();
    mflag = 0;  /* 00E079 CLM */
    A = rd16(DP(0x00));  /* 00E07A LDA */
    mflag = 1;  /* 00E07C SEM */
    /* 00E07D CMP */
    if ((A & 0xFF) >= 0x02) goto L_00E0A2;  /* 00E07F BCS */
    wr8(ABS(P7D), 0x0F);  /* 00E081 LDM */
    wr8(ABS(P6D), 0x07);  /* 00E085 LDM */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x08);  /* 00E089 SEB [P73 SICAKLIK(AN3)] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x01);  /* 00E08D SEB [P60 UZAK_VERI_GIRIS] */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 00E091 CLB [P71 GUC_ACMA] */
L_00E095: TICK();
    wr8(ABS(WDT), 0x00);  /* 00E095 LDM */
    mflag = 0;  /* 00E099 CLM */
    ram_trampoline_087A();  /* 00E09A JSRL */
    mflag = 1;  /* 00E09E SEM */
    goto L_00E095;  /* 00E09F JMP */
L_00E0A2: TICK();
    mflag = 0;  /* 00E0A2 CLM */
    A = rd16(DP(0x00));  /* 00E0A3 LDA */
    mflag = 1;  /* 00E0A5 SEM */
    /* 00E0A6 CMP */
    if ((A & 0xFF) != 0x05) goto L_00E0B9;  /* 00E0A8 BNE */
    Y = 0x0006;  /* 00E0AA LDY */
    mflag = 0;  /* 00E0AD CLM */
    S -= 3; F_003310();  /* 00E0AE JSRL */
    S -= 3; F_01A60C();  /* 00E0B2 JSRL */
    goto L_00E0DD;  /* 00E0B6 JMP */
L_00E0B9: TICK();
    mflag = 0;  /* 00E0B9 CLM */
    A = rd16(DP(0x00));  /* 00E0BA LDA */
    mflag = 1;  /* 00E0BC SEM */
    /* 00E0BD CMP */
    if ((A & 0xFF) != 0x02) goto L_00E0D0;  /* 00E0BF BNE */
    wr8(ABS(P7D), 0x0F);  /* 00E0C1 LDM */
    wr8(ABS(P6D), 0x03);  /* 00E0C5 LDM */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) & ~0x02);  /* 00E0C9 CLB */
    goto L_00E0DD;  /* 00E0CD JMP */
L_00E0D0: TICK();
    mflag = 1;  /* 00E0D0 SEM */
    wr8(ABS(P7D), 0x07);  /* 00E0D1 LDM */
    wr8(ABS(P6D), 0x02);  /* 00E0D5 LDM */
    wr8(ABS(0x0335), rd8(ABS(0x0335)) | 0x02);  /* 00E0D9 SEB */
L_00E0DD: TICK();
    mflag = 0;  /* 00E0DD CLM */
    Y = pull16();  /* 00E0DE PLY */
    D = pull16();  /* 00E0DF PLD */
    S += 3; return;  /* 00E0E0 RTL */
}

/* 00E0E1  callers: 00E431 */
void F_00E0E1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E0E1 LDA */
    push16(D);  /* 00E0E3 PHD */
    D = A;  /* 00E0E4 TAD */
    A += 1;  /* 00E0E5 INC */
    push16(A);  /* 00E0E6 PHA */
    Y = 0x0023;  /* 00E0E7 LDY */
    S -= 3; F_003310();  /* 00E0EA JSRL */
    mflag = 1;  /* 00E0EE SEM */
    SETLO(A, rd8(0x010009));  /* 00E0EF LDA */
    wr8(ABS(0x0252), (uint8_t)A);  /* 00E0F3 STA */
    SETLO(A, rd8(0x01000A));  /* 00E0F6 LDA */
    wr8(ABS(0x0253), (uint8_t)A);  /* 00E0FA STA */
    SETLO(A, rd8(0x010000));  /* 00E0FD LDA */
    wr8(ABS(0x0255), (uint8_t)A);  /* 00E101 STA */
    SETLO(A, rd8(0x010001));  /* 00E104 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 00E108 STA */
    C = ((A & 0xFF) >= 0x0A);  /* 00E10A CMP */
    if (C) goto L_00E113;  /* 00E10C BCS */
    SETLO(A, adc8(A, 0x30));  /* 00E10E ADC */
    wr8(ABS(0x0256), (uint8_t)A);  /* 00E110 STA */
L_00E113: TICK();
    wr8(ABS(0x0257), 0x2D);  /* 00E113 LDM */
    SETLO(A, rd8(0x010002));  /* 00E117 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 00E11B STA */
    /* 00E11D CMP */
    if ((A & 0xFF) >= 0x64) goto L_00E14B;  /* 00E11F BCS */
    SETLO(B, 0x00);  /* 00E121 LDA */
    div8(0x0A);  /* 00E124 DIV */
    C = 0;  /* 00E127 CLC */
    SETLO(A, adc8(A, 0x30));  /* 00E128 ADC */
    wr8(ABS(0x0258), (uint8_t)A);  /* 00E12A STA */
    SETLO(B, 0x00);  /* 00E12D LDA */
    SETLO(A, rd8(DP(0x00)));  /* 00E130 LDA */
    div8(0x0A);  /* 00E132 DIV */
    mflag = 0;  /* 00E135 CLM */
    push16(B);  /* 00E136 PHB */
    A = pull16();  /* 00E138 PLA */
    C = 0;  /* 00E139 CLC */
    mflag = 1;  /* 00E13A SEM */
    SETLO(A, adc8(A, 0x30));  /* 00E13B ADC */
    wr8(DP(0x00), (uint8_t)A);  /* 00E13D STA */
    /* 00E13F CMP */
    if ((A & 0xFF) == 0x30) goto L_00E146;  /* 00E141 BEQ */
    goto L_00E148;  /* 00E143 JMP */
L_00E146: TICK();
    SETLO(A, 0x20);  /* 00E146 LDA */
L_00E148: TICK();
    wr8(ABS(0x0259), (uint8_t)A);  /* 00E148 STA */
L_00E14B: TICK();
    mflag = 0;  /* 00E14B CLM */
    S -= 3; F_00828C();  /* 00E14C JSRL */
    Y = pull16();  /* 00E150 PLY */
    D = pull16();  /* 00E151 PLD */
    S += 3; return;  /* 00E152 RTL */
}

/* 00E153  callers: 00E460 */
void F_00E153(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E153 LDA */
    push16(D);  /* 00E155 PHD */
    D = A;  /* 00E156 TAD */
    C = 0;  /* 00E157 CLC */
    A = adc16(A, 0x0005);  /* 00E158 ADC */
    push16(A);  /* 00E15B PHA */
    Y = 0x0023;  /* 00E15C LDY */
    S -= 3; F_003310();  /* 00E15F JSRL */
    mflag = 1;  /* 00E163 SEM */
    wr8(ABS(0x0252), 0x50);  /* 00E164 LDM */
    wr8(ABS(0x0253), 0x4E);  /* 00E168 LDM */
    wr8(DP(0x05), 0x03);  /* 00E16C LDM */
    mflag = 0;  /* 00E16F CLM */
    A = D;  /* 00E170 TDA */
    B = 0x0000;  /* 00E171 LDA */
    wr16(DP(0x06), A);  /* 00E175 STA */
    wr16(DP(0x08), B);  /* 00E177 STA */
    wr16(DP(0x09), 0x0588);  /* 00E17A LDM */
    Y = 0x00A0;  /* 00E17E LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00E181 JSRL */
    mflag = 1;  /* 00E185 SEM */
    wr8(DP(0x04), 0x00);  /* 00E186 LDM */
L_00E189: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 00E189 LDA */
    /* 00E18B CMP */
    if ((A & 0xFF) >= 0x06) goto L_00E1D1;  /* 00E18D BCS */
    SETLO(B, 0x00);  /* 00E18F LDA */
    div8(0x02);  /* 00E192 DIV */
    mflag = 0;  /* 00E195 CLM */
    A &= 0x00FF;  /* 00E196 AND */
    X = A;  /* 00E199 TAX */
    A = rd16(DP(0x00 + X));  /* 00E19A LDA */
    mflag = 1;  /* 00E19C SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 00E19D STA */
    if ((rd8(DP(0x04)) & 0x01) == 0) goto L_00E1A9;  /* 00E19F BBC */
    wr8(DP(0x03), rd8(DP(0x03)) & ~0xF0);  /* 00E1A3 CLB */
    goto L_00E1B3;  /* 00E1A6 JMP */
L_00E1A9: TICK();
    mflag = 0;  /* 00E1A9 CLM */
    A = rd16(DP(0x03));  /* 00E1AA LDA */
    mflag = 1;  /* 00E1AC SEM */
    SETLO(A, lsr8(A));  /* 00E1AD LSR */
    SETLO(A, lsr8(A));  /* 00E1AE LSR */
    SETLO(A, lsr8(A));  /* 00E1AF LSR */
    SETLO(A, lsr8(A));  /* 00E1B0 LSR */
    wr8(DP(0x03), (uint8_t)A);  /* 00E1B1 STA */
L_00E1B3: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 00E1B3 LDA */
    C = ((A & 0xFF) >= 0x0A);  /* 00E1B5 CMP */
    if (C) goto L_00E1CC;  /* 00E1B7 BCS */
    SETLO(A, adc8(A, 0x30));  /* 00E1B9 ADC */
    wr8(DP(0x05), (uint8_t)A);  /* 00E1BB STA */
    SETLO(A, rd8(DP(0x04)));  /* 00E1BD LDA */
    mflag = 0;  /* 00E1BF CLM */
    A &= 0x00FF;  /* 00E1C0 AND */
    A += 1;  /* 00E1C3 INC */
    A += 1;  /* 00E1C4 INC */
    X = A;  /* 00E1C5 TAX */
    A = rd16(DP(0x05));  /* 00E1C6 LDA */
    mflag = 1;  /* 00E1C8 SEM */
    wr8((ABS(0x0252) + X), (uint8_t)A);  /* 00E1C9 STA */
L_00E1CC: TICK();
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 00E1CC INC */
    goto L_00E189;  /* 00E1CE JMP */
L_00E1D1: TICK();
    mflag = 0;  /* 00E1D1 CLM */
    S -= 3; F_00828C();  /* 00E1D2 JSRL */
    Y = pull16();  /* 00E1D6 PLY */
    D = pull16();  /* 00E1D7 PLD */
    S += 3; return;  /* 00E1D8 RTL */
}

/* 00E1D9  callers: 00E4BA */
void F_00E1D9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E1D9 LDA */
    push16(D);  /* 00E1DB PHD */
    D = A;  /* 00E1DC TAD */
    C = 0;  /* 00E1DD CLC */
    A = adc16(A, 0x0004);  /* 00E1DE ADC */
    push16(A);  /* 00E1E1 PHA */
    Y = 0x0023;  /* 00E1E2 LDY */
    S -= 3; F_003310();  /* 00E1E5 JSRL */
    mflag = 1;  /* 00E1E9 SEM */
    wr8(ABS(0x0252), 0x53);  /* 00E1EA LDM */
    wr8(ABS(0x0253), 0x4E);  /* 00E1EE LDM */
    wr8(DP(0x04), 0x02);  /* 00E1F2 LDM */
    mflag = 0;  /* 00E1F5 CLM */
    A = D;  /* 00E1F6 TDA */
    B = 0x0000;  /* 00E1F7 LDA */
    wr16(DP(0x05), A);  /* 00E1FB STA */
    wr16(DP(0x07), B);  /* 00E1FD STA */
    wr16(DP(0x08), 0x0001);  /* 00E200 LDM */
    Y = 0x00A0;  /* 00E204 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00E207 JSRL */
    A = rd16(DP(0x00));  /* 00E20B LDA */
    A &= 0x00FF;  /* 00E20D AND */
    A = rla16(A, 0x0008);  /* 00E210 RLA */
    A &= 0xFF00;  /* 00E214 AND */
    wr16(DP(0x04), A);  /* 00E217 STA */
    A = rd16(DP(0x01));  /* 00E219 LDA */
    A &= 0x00FF;  /* 00E21B AND */
    C = 0;  /* 00E21E CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 00E21F ADC */
    wr16(DP(0x04), 0x0255);  /* 00E221 LDM */
    wr16(DP(0x06), 0x0000);  /* 00E225 LDM */
    Y = A;  /* 00E229 TAY */
    wr16(DP(0x02), Y);  /* 00E22A STY */
    S -= 3; F_006948();  /* 00E22C JSRL */
    S -= 3; F_00828C();  /* 00E230 JSRL */
    Y = pull16();  /* 00E234 PLY */
    D = pull16();  /* 00E235 PLD */
    S += 3; return;  /* 00E236 RTL */
}

/* 00E237  callers: 00E424, 00E440 */
void F_00E237(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E237 LDA */
    push16(D);  /* 00E239 PHD */
    D = A;  /* 00E23A TAD */
    wr16(DP(0x02), Y);  /* 00E23B STY */
    wr16(DP(0x04), B);  /* 00E23D STA */
    wr16(DP(0x06), 0x0000);  /* 00E240 LDM */
L_00E244: TICK();
    A = rd16(LIND(0x02));  /* 00E244 LDA */
    A &= 0x00FF;  /* 00E246 AND */
    C = 0;  /* 00E249 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 00E24A ADC */
    wr16(DP(0x06), A);  /* 00E24C STA */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 00E24E INC */
    if (t != 0) goto L_00E254;  /* 00E250 BNE */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 00E252 INC */
L_00E254: TICK();
    t = (rd16(DP(0x00)) - 1) & 0xFFFF; wr16(DP(0x00), t);  /* 00E254 DEC */
    X = rd16(DP(0x00));  /* 00E256 LDX */
    if (X != 0) goto L_00E244;  /* 00E258 BNE */
    D = pull16();  /* 00E25A PLD */
    S += 3; return;  /* 00E25B RTL */
}

/* 00E25C  callers: 00E3E7, 00E532 */
void F_00E25C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E25C LDA */
    push16(D);  /* 00E25E PHD */
    D = A;  /* 00E25F TAD */
    A += 1;  /* 00E260 INC */
    push16(A);  /* 00E261 PHA */
    S -= 3; F_007529();  /* 00E262 JSRL */
    wr16(DP(0x01), 0x0001);  /* 00E266 LDM */
    A = D;  /* 00E26A TDA */
    B = 0x0000;  /* 00E26B LDA */
    wr16(DP(0x02), A);  /* 00E26F STA */
    wr16(DP(0x04), B);  /* 00E271 STA */
    wr16(DP(0x05), 0x0000);  /* 00E274 LDM */
    Y = 0x0046;  /* 00E278 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00E27B JSRL */
    mflag = 1;  /* 00E27F SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00E2AC;  /* 00E280 BBC */
    mflag = 0;  /* 00E285 CLM */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_00E2A9;  /* 00E286 BBS */
    mflag = 1;  /* 00E28C SEM */
    if ((rd8(DP(0x00)) & 0x20) == 0) goto L_00E2A4;  /* 00E28D BBC */
    mflag = 0;  /* 00E291 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0001);  /* 00E292 SEB */
    mflag = 1;  /* 00E297 SEM */
    if ((rd8(ABS(0x0335)) & 0x04) == 0) goto L_00E2A1;  /* 00E298 BBC */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x02);  /* 00E29D SEB [P71 GUC_ACMA] */
L_00E2A1: TICK();
    goto L_00E2A9;  /* 00E2A1 JMP */
L_00E2A4: TICK();
    mflag = 0;  /* 00E2A4 CLM */
    S -= 3; F_00973E();  /* 00E2A5 JSRL */
L_00E2A9: TICK();
    goto L_00E2C9;  /* 00E2A9 JMP */
L_00E2AC: TICK();
    mflag = 1;  /* 00E2AC SEM */
    SETLO(B, 0x00);  /* 00E2AD LDA */
    SETLO(A, rd8(DP(0x00)));  /* 00E2B0 LDA */
    div8(0x10);  /* 00E2B2 DIV */
    SETLO(A, A ^ 0xFF);  /* 00E2B5 EOR */
    SETLO(A, A + 1);  /* 00E2B7 INC */
    C = 0;  /* 00E2B8 CLC */
    SETLO(A, adc8(A, 0x10));  /* 00E2B9 ADC */
    wr8(ABS(0x0206), (uint8_t)A);  /* 00E2BB STA */
    mflag = 0;  /* 00E2BE CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0001);  /* 00E2BF SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x4000);  /* 00E2C4 SEB */
L_00E2C9: TICK();
    mflag = 1;  /* 00E2C9 SEM */
    if ((rd8(ABS(0x0334)) & 0x04) == 0) goto L_00E2DD;  /* 00E2CA BBC */
    mflag = 0;  /* 00E2CF CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0200);  /* 00E2D0 CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0400);  /* 00E2D5 CLB */
    goto L_00E2E8;  /* 00E2DA JMP */
L_00E2DD: TICK();
    mflag = 0;  /* 00E2DD CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0200);  /* 00E2DE SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0400);  /* 00E2E3 CLB */
L_00E2E8: TICK();
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0001);  /* 00E2E8 SEB */
    S -= 3; cbus_write_sel2_00755E();  /* 00E2ED JSRL */
    mflag = 1;  /* 00E2F1 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00E2FC;  /* 00E2F2 BBC */
    SETLO(A, 0xF8);  /* 00E2F7 LDA */
    goto L_00E2FE;  /* 00E2F9 JMP */
L_00E2FC: TICK();
    SETLO(A, 0xFC);  /* 00E2FC LDA */
L_00E2FE: TICK();
    SETLO(A, A & rd8(DP(0x00)));  /* 00E2FE AND */
    wr8(DP(0x00), (uint8_t)A);  /* 00E300 STA */
    Y = pull16();  /* 00E302 PLY */
    D = pull16();  /* 00E303 PLD */
    S += 3; return;  /* 00E304 RTL */
}

/* 00E305  callers: 019BAB, 01A735 */
void sys_init_00E305(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E305 LDA */
    push16(D);  /* 00E307 PHD */
    D = A;  /* 00E308 TAD */
    C = 0;  /* 00E309 CLC */
    A = adc16(A, 0x0005);  /* 00E30A ADC */
    push16(A);  /* 00E30D PHA */
    mflag = 1;  /* 00E30E SEM */
    wr8(ABS(0x087A), 0xEA);  /* 00E30F LDM */
    wr8(ABS(0x087B), 0xCB);  /* 00E313 LDM */
    wr8(ABS(0x087C), 0x6B);  /* 00E317 LDM */
    SETLO(A, 0x00);  /* 00E31B LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00E31D PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00E31E STA */
    setP(pull16());  /* 00E320 PLP */
    I = 0;  /* 00E321 CLI */
    mflag = 1;  /* 00E322 SEM */
    wr8(ABS(TB2MOD), 0x80);  /* 00E323 LDM */
    mflag = 0;  /* 00E327 CLM */
    wr16(ABS(TB2), 0x0FBF);  /* 00E328 LDM */
    mflag = 1;  /* 00E32D SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x80);  /* 00E32E SEB */
    wr8(ABS(TB2IC), 0x02);  /* 00E332 LDM */
    mflag = 0;  /* 00E336 CLM */
    wr16(ABS(0x023A), 0x0000);  /* 00E337 LDM */
    wr16(ABS(0x023C), 0x0000);  /* 00E33C LDM */
    wr16(ABS(0x023E), 0x0000);  /* 00E341 LDM */
    wr16(ABS(0x0240), 0x0000);  /* 00E346 LDM */
    wr16(ABS(0x0242), 0x0000);  /* 00E34B LDM */
    wr16(ABS(0x0244), 0x0000);  /* 00E350 LDM */
    wr16(ABS(0x0246), 0x0000);  /* 00E355 LDM */
    wr16(ABS(0x0248), 0x0000);  /* 00E35A LDM */
    mflag = 1;  /* 00E35F SEM */
    wr8(ABS(0x01EB), 0x04);  /* 00E360 LDM */
    wr8(ABS(0x01E9), 0x00);  /* 00E364 LDM */
    mflag = 0;  /* 00E368 CLM */
    S -= 3; F_00CDE8();  /* 00E369 JSRL */
    mflag = 1;  /* 00E36D SEM */
    wr8(ABS(0x0228), 0x0D);  /* 00E36E LDM */
    mflag = 0;  /* 00E372 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0010);  /* 00E373 SEB */
    S -= 3; F_00DF55();  /* 00E378 JSRL */
    mflag = 1;  /* 00E37C SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00E392;  /* 00E37D BBC */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_00E392;  /* 00E382 BBC [P45 ACIL_CAGRI] */
    mflag = 0;  /* 00E387 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0200);  /* 00E388 SEB */
    mflag = 1;  /* 00E38D SEM */
    wr8(ABS(0x0233), 0x3F);  /* 00E38E LDM */
L_00E392: TICK();
    mflag = 0;  /* 00E392 CLM */
    wr16(DP(0x01), 0x0000);  /* 00E393 LDM */
L_00E397: TICK();
    if ((rd16(ABS(0x0240)) & 0x0200) == 0) goto L_00E3D0;  /* 00E397 BBC */
    mflag = 1;  /* 00E39D SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 00E39E LDA */
    if ((A & 0xFF) == 0) goto L_00E3D0;  /* 00E3A1 BEQ */
    Y = 0x0005;  /* 00E3A3 LDY */
    mflag = 0;  /* 00E3A6 CLM */
    S -= 3; F_007D7F();  /* 00E3A7 JSRL */
    mflag = 1;  /* 00E3AB SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00E3BC;  /* 00E3AC BBC */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_00E3BC;  /* 00E3B1 BBS [P45 ACIL_CAGRI] */
    mflag = 0;  /* 00E3B6 CLM */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 00E3B7 INC */
    goto L_00E3C1;  /* 00E3B9 JMP */
L_00E3BC: TICK();
    mflag = 0;  /* 00E3BC CLM */
    wr16(DP(0x01), 0x0000);  /* 00E3BD LDM */
L_00E3C1: TICK();
    X = rd16(DP(0x01));  /* 00E3C1 LDX */
    /* 00E3C3 CPX */
    if (X < 0x000B) goto L_00E3CD;  /* 00E3C6 BCC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 00E3C8 CLB */
L_00E3CD: TICK();
    goto L_00E397;  /* 00E3CD JMP */
L_00E3D0: TICK();
    mflag = 0;  /* 00E3D0 CLM */
    if ((rd16(ABS(0x0240)) & 0x0200) == 0) goto L_00E3E6;  /* 00E3D1 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0200);  /* 00E3D7 CLB */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0020);  /* 00E3DC SEB */
    mflag = 1;  /* 00E3E1 SEM */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x02);  /* 00E3E2 SEB [P71 GUC_ACMA] */
L_00E3E6: TICK();
    mflag = 0;  /* 00E3E6 CLM */
    S -= 3; F_00E25C();  /* 00E3E7 JSRL */
    mflag = 0;  /* 00E3EB CLM */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_00E3F9;  /* 00E3EC BBC */
    Y = 0x0001;  /* 00E3F2 LDY */
    S -= 3; F_00819B();  /* 00E3F5 JSRL */
L_00E3F9: TICK();
    mflag = 1;  /* 00E3F9 SEM */
    SETLO(A, rd8(0x010004));  /* 00E3FA LDA */
    mflag = 0;  /* 00E3FE CLM */
    A &= 0x00FF;  /* 00E3FF AND */
    wr16(DP(0x05), A);  /* 00E402 STA */
    mflag = 1;  /* 00E404 SEM */
    SETLO(A, rd8(0x010003));  /* 00E405 LDA */
    mflag = 0;  /* 00E409 CLM */
    A &= 0x00FF;  /* 00E40A AND */
    A = rla16(A, 0x0008);  /* 00E40D RLA */
    A &= 0xFF00;  /* 00E411 AND */
    C = 0;  /* 00E414 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 00E415 ADC */
    wr16(DP(0x01), A);  /* 00E417 STA */
    wr16(DP(0x05), 0x8000);  /* 00E419 LDM */
    B = 0x0000;  /* 00E41D LDA */
    Y = 0x2000;  /* 00E421 LDY */
    S -= 3; F_00E237();  /* 00E424 JSRL */
    Y = 0x0000;  /* 00E428 LDY */
    wr16(DP(0x03), A);  /* 00E42B STA */
    S -= 3; F_00819B();  /* 00E42D JSRL */
    S -= 3; F_00E0E1();  /* 00E431 JSRL */
    wr16(DP(0x05), 0x6000);  /* 00E435 LDM */
    B = 0x0000;  /* 00E439 LDA */
    Y = 0xA000;  /* 00E43D LDY */
    S -= 3; F_00E237();  /* 00E440 JSRL */
    C = 0;  /* 00E444 CLC */
    A = adc16(A, rd16(DP(0x03)));  /* 00E445 ADC */
    wr16(DP(0x03), A);  /* 00E447 STA */
    wr16(DP(0x05), 0x0001);  /* 00E449 LDM */
    wr16(DP(0x06), 0x0340);  /* 00E44D LDM */
    wr16(DP(0x08), 0x0000);  /* 00E451 LDM */
    wr16(DP(0x09), 0x0018);  /* 00E455 LDM */
    Y = 0x00A0;  /* 00E459 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00E45C JSRL */
    S -= 3; F_00E153();  /* 00E460 JSRL */
    S -= 3; F_008ECC();  /* 00E464 JSRL */
    S -= 3; F_009134();  /* 00E468 JSRL */
    wr8(ABS(0x0369), (uint8_t)A);  /* 00E46C STA */
    mflag = 0;  /* 00E46F CLM */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_00E483;  /* 00E470 BBC */
    if ((rd16(ABS(0x023C)) & 0x1000) == 0x1000) goto L_00E483;  /* 00E476 BBS */
    Y = 0x0001;  /* 00E47C LDY */
    S -= 3; F_008156();  /* 00E47F JSRL */
L_00E483: TICK();
    X = rd16(DP(0x03));  /* 00E483 LDX */
    t = rd16(DP(0x01));  /* 00E485 CPX */
    if (X == t) goto L_00E498;  /* 00E487 BEQ */
    Y = 0x0008;  /* 00E489 LDY */
    S -= 3; F_003310();  /* 00E48C JSRL */
    S -= 3; F_00828C();  /* 00E490 JSRL */
    S -= 3; F_01A60C();  /* 00E494 JSRL */
L_00E498: TICK();
    S -= 3; F_00DCFD();  /* 00E498 JSRL */
    S -= 3; F_00DA32();  /* 00E49C JSRL */
    S -= 3; F_00D8E3();  /* 00E4A0 JSRL */
    Y = 0x0000;  /* 00E4A4 LDY */
    S -= 3; F_008156();  /* 00E4A7 JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0100);  /* 00E4AB SEB */
    S -= 3; F_007A06();  /* 00E4B0 JSRL */
    mflag = 1;  /* 00E4B4 SEM */
    wr8(ABS(0x0233), 0x10);  /* 00E4B5 LDM */
    mflag = 0;  /* 00E4B9 CLM */
    S -= 3; F_00E1D9();  /* 00E4BA JSRL */
    S -= 3; F_00D7B6();  /* 00E4BE JSRL */
L_00E4C2: TICK();
    mflag = 1;  /* 00E4C2 SEM */
    SETLO(A, rd8(ABS(0x0233)));  /* 00E4C3 LDA */
    if ((A & 0xFF) == 0) goto L_00E4CB;  /* 00E4C6 BEQ */
    goto L_00E4C2;  /* 00E4C8 JMP */
L_00E4CB: TICK();
    mflag = 0;  /* 00E4CB CLM */
    wr16(DP(0x05), 0x0010);  /* 00E4CC LDM */
    wr16(DP(0x07), 0x0058);  /* 00E4D0 LDM */
    wr16(DP(0x09), 0x0060);  /* 00E4D4 LDM */
    Y = 0x0003;  /* 00E4D8 LDY */
    A = D;  /* 00E4DB TDA */
    C = 0;  /* 00E4DC CLC */
    A = adc16(A, 0x000B);  /* 00E4DD ADC */
    push16(A);  /* 00E4E0 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00E4E1 JSRL */
    Y = pull16();  /* 00E4E5 PLY */
    mflag = 0;  /* 00E4E6 CLM */
    wr16(DP(0x05), 0x0004);  /* 00E4E7 LDM */
    wr16(DP(0x07), 0x0040);  /* 00E4EB LDM */
    Y = 0x0002;  /* 00E4EF LDY */
    A = D;  /* 00E4F2 TDA */
    C = 0;  /* 00E4F3 CLC */
    A = adc16(A, 0x0009);  /* 00E4F4 ADC */
    push16(A);  /* 00E4F7 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00E4F8 JSRL */
    Y = pull16();  /* 00E4FC PLY */
    wr8(DP(0x00), 0x0F);  /* 00E4FD LDM */
    wr8(DP(0x05), 0x01);  /* 00E500 LDM */
    mflag = 0;  /* 00E503 CLM */
    A = D;  /* 00E504 TDA */
    B = 0x0000;  /* 00E505 LDA */
    wr16(DP(0x06), A);  /* 00E509 STA */
    wr16(DP(0x08), B);  /* 00E50B STA */
    wr16(DP(0x09), 0x0000);  /* 00E50E LDM */
    Y = 0x0042;  /* 00E512 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00E515 JSRL */
    wr16(DP(0x05), 0x0001);  /* 00E519 LDM */
    A = D;  /* 00E51D TDA */
    B = 0x0000;  /* 00E51E LDA */
    wr16(DP(0x06), A);  /* 00E522 STA */
    wr16(DP(0x08), B);  /* 00E524 STA */
    wr16(DP(0x09), 0x0000);  /* 00E527 LDM */
    Y = 0x0042;  /* 00E52B LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00E52E JSRL */
    S -= 3; F_00E25C();  /* 00E532 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00E536 STA */
    mflag = 0;  /* 00E538 CLM */
    S -= 3; F_01E006();  /* 00E539 JSRL */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_00E589;  /* 00E53D BBC */
    mflag = 1;  /* 00E543 SEM */
    if ((rd8(ABS(0x0341)) & 0x80) == 0x80) goto L_00E589;  /* 00E544 BBS */
    mflag = 0;  /* 00E549 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0020);  /* 00E54A CLB */
    S -= 3; F_00968D();  /* 00E54F JSRL */
    Y = 0x000A;  /* 00E553 LDY */
    S -= 3; F_007D7F();  /* 00E556 JSRL */
    wr16(DP(0x05), 0x0001);  /* 00E55A LDM */
    A = D;  /* 00E55E TDA */
    B = 0x0000;  /* 00E55F LDA */
    wr16(DP(0x06), A);  /* 00E563 STA */
    wr16(DP(0x08), B);  /* 00E565 STA */
    wr16(DP(0x09), 0x0000);  /* 00E568 LDM */
    Y = 0x0046;  /* 00E56C LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00E56F JSRL */
    mflag = 1;  /* 00E573 SEM */
    if ((rd8(DP(0x00)) & 0x20) == 0) goto L_00E584;  /* 00E574 BBC */
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_00E584;  /* 00E578 BBS [P45 ACIL_CAGRI] */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 00E57D SEB */
    goto L_00E589;  /* 00E581 JMP */
L_00E584: TICK();
    mflag = 0;  /* 00E584 CLM */
    S -= 3; F_00973E();  /* 00E585 JSRL */
L_00E589: TICK();
    mflag = 1;  /* 00E589 SEM */
    wr8(ABS(INT1IC), 0x31);  /* 00E58A LDM */
    wr8(ABS(INT2IC), 0x33);  /* 00E58E LDM */
    wr8(ABS(INT0IC), 0x00);  /* 00E592 LDM */
    wr8(ABS(TA0IC), 0x00);  /* 00E596 LDM */
    wr8(ABS(TB1IC), 0x00);  /* 00E59A LDM */
    wr8(ABS(TB0IC), 0x00);  /* 00E59E LDM */
    wr8(ABS(TA4IC), 0x00);  /* 00E5A2 LDM */
    wr8(ABS(TA3IC), 0x00);  /* 00E5A6 LDM */
    wr8(ABS(TA2IC), 0x00);  /* 00E5AA LDM */
    wr8(ABS(TA1IC), 0x00);  /* 00E5AE LDM */
    wr8(ABS(U1RIC), 0x00);  /* 00E5B2 LDM */
    wr8(ABS(U1TIC), 0x00);  /* 00E5B6 LDM */
    wr8(ABS(U0RIC), 0x00);  /* 00E5BA LDM */
    wr8(ABS(U0TIC), 0x00);  /* 00E5BE LDM */
    wr8(ABS(ADIC), 0x00);  /* 00E5C2 LDM */
    wr8(ABS(WDT), 0x00);  /* 00E5C6 LDM */
    mflag = 0;  /* 00E5CA CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x4000);  /* 00E5CB SEB */
    Y = 0x0007;  /* 00E5D0 LDY */
    S -= 3; F_0080AA();  /* 00E5D3 JSRL */
    S -= 3; F_00CB52();  /* 00E5D7 JSRL */
    mflag = 1;  /* 00E5DB SEM */
    wr8(ABS(0x0229), 0x00);  /* 00E5DC LDM */
    if ((rd8(ABS(0x0340)) & 0x04) == 0) goto L_00E5EA;  /* 00E5E0 BBC */
    SETLO(A, 0x0A);  /* 00E5E5 LDA */
    goto L_00E5EC;  /* 00E5E7 JMP */
L_00E5EA: TICK();
    SETLO(A, 0x00);  /* 00E5EA LDA */
L_00E5EC: TICK();
    wr8(ABS(0x020E), (uint8_t)A);  /* 00E5EC STA */
    mflag = 0;  /* 00E5EF CLM */
    Y = pull16();  /* 00E5F0 PLY */
    D = pull16();  /* 00E5F1 PLD */
    S += 3; return;  /* 00E5F2 RTL */
}

/* 00E5F3  callers: 00E654 */
void F_00E5F3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E5F3 LDA */
    push16(A);  /* 00E5F5 PHA */
    Y = 0x0005;  /* 00E5F6 LDY */
    S -= 3; F_008DC6();  /* 00E5F9 JSRL */
    A = rd16(ABS(0x034A));  /* 00E5FD LDA */
    mflag = 1;  /* 00E600 SEM */
    t = rd8(ABS(AD5));  /* 00E601 CMP [AN5 RSSI] */
    if ((A & 0xFF) >= t) goto L_00E616;  /* 00E604 BCS */
    mflag = 0;  /* 00E606 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0040);  /* 00E607 SEB */
    A = rd16(ABS(0x0349));  /* 00E60C LDA */
    mflag = 1;  /* 00E60F SEM */
    wr8(ABS(0x022C), (uint8_t)A);  /* 00E610 STA */
    goto L_00E623;  /* 00E613 JMP */
L_00E616: TICK();
    mflag = 0;  /* 00E616 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0040);  /* 00E617 CLB */
    A = rd16(ABS(0x0348));  /* 00E61C LDA */
    mflag = 1;  /* 00E61F SEM */
    wr8(ABS(0x022C), (uint8_t)A);  /* 00E620 STA */
L_00E623: TICK();
    mflag = 0;  /* 00E623 CLM */
    Y = pull16();  /* 00E624 PLY */
    S += 3; return;  /* 00E625 RTL */
}

/* 00E626  callers: 00F84D */
void F_00E626(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E626 LDA */
    push16(A);  /* 00E628 PHA */
    S -= 3; F_01DC35();  /* 00E629 JSRL */
    mflag = 1;  /* 00E62D SEM */
    SETLO(A, rd8(ABS(0x0237)));  /* 00E62E LDA */
    if ((A & 0xFF) != 0) goto L_00E658;  /* 00E631 BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 00E633 LDA */
    /* 00E636 CMP */
    if ((A & 0xFF) != 0x08) goto L_00E646;  /* 00E638 BNE */
    wr8(ABS(0x0237), 0x02);  /* 00E63A LDM */
    mflag = 0;  /* 00E63E CLM */
    S -= 3; tx_power_protect_00D1C8();  /* 00E63F JSRL */
    goto L_00E658;  /* 00E643 JMP */
L_00E646: TICK();
    mflag = 0;  /* 00E646 CLM */
    A = rd16(ABS(0x0228));  /* 00E647 LDA */
    mflag = 1;  /* 00E64A SEM */
    /* 00E64B CMP */
    if ((A & 0xFF) != 0x03) goto L_00E658;  /* 00E64D BNE */
    wr8(ABS(0x0237), 0x20);  /* 00E64F LDM */
    mflag = 0;  /* 00E653 CLM */
    S -= 3; F_00E5F3();  /* 00E654 JSRL */
L_00E658: TICK();
    mflag = 0;  /* 00E658 CLM */
    Y = pull16();  /* 00E659 PLY */
    S += 3; return;  /* 00E65A RTL */
}

/* 00E65B  callers: 00F4E4, 00F628, 00F67A */
void selcall_send_00E65B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E65B LDA */
    push16(D);  /* 00E65D PHD */
    D = A;  /* 00E65E TAD */
    C = 0;  /* 00E65F CLC */
    A = adc16(A, 0x0015);  /* 00E660 ADC */
    push16(A);  /* 00E663 PHA */
    A = rd16(ABS(0x0340));  /* 00E664 LDA */
    mflag = 1;  /* 00E667 SEM */
    SETLO(A, A & 0x03);  /* 00E668 AND */
    if ((A & 0xFF) != 0) goto L_00E66F;  /* 00E66A BNE */
    goto L_00E9CE;  /* 00E66C JMP */
L_00E66F: TICK();
    mflag = 0;  /* 00E66F CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0800);  /* 00E670 CLB */
    X = rd16(ABS(0x029A));  /* 00E675 LDX */
    wr16(DP(0x13), X);  /* 00E678 STX */
    wr16(ABS(0x029A), rd16(ABS(0x029A)) | 0x8000);  /* 00E67A SEB */
    wr16(ABS(0x029A), rd16(ABS(0x029A)) & ~0x4000);  /* 00E67F CLB */
    mflag = 1;  /* 00E684 SEM */
    SETLO(A, rd8(ABS(0x029B)));  /* 00E685 LDA */
    SETLO(A, A & 0xF8);  /* 00E688 AND */
    SETLO(A, A | 0x06);  /* 00E68A ORA */
    wr8(ABS(0x029B), (uint8_t)A);  /* 00E68C STA */
    mflag = 0;  /* 00E68F CLM */
    S -= 3; F_00CB52();  /* 00E690 JSRL */
    X = rd16(DP(0x13));  /* 00E694 LDX */
    wr16(ABS(0x029A), X);  /* 00E696 STX */
    mflag = 1;  /* 00E699 SEM */
    wr8(DP(0x0A), 0x00);  /* 00E69A LDM */
L_00E69D: TICK();
    SETLO(A, rd8(DP(0x0A)));  /* 00E69D LDA */
    /* 00E69F CMP */
    if ((A & 0xFF) >= 0x08) goto L_00E6B1;  /* 00E6A1 BCS */
    mflag = 0;  /* 00E6A3 CLM */
    A &= 0x00FF;  /* 00E6A4 AND */
    X = A;  /* 00E6A7 TAX */
    mflag = 1;  /* 00E6A8 SEM */
    wr8(DP(0x02 + X), 0x0F);  /* 00E6A9 LDM */
    t = (rd8(DP(0x0A)) + 1) & 0xFF; wr8(DP(0x0A), t);  /* 00E6AC INC */
    goto L_00E69D;  /* 00E6AE JMP */
L_00E6B1: TICK();
    mflag = 1;  /* 00E6B1 SEM */
    SETLO(B, 0x00);  /* 00E6B2 LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00E6B5 LDA */
    div8(0x10);  /* 00E6B8 DIV */
    wr8(DP(0x0C), (uint8_t)A);  /* 00E6BB STA */
    mflag = 0;  /* 00E6BD CLM */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0) goto L_00E6DE;  /* 00E6BE BBC */
    mflag = 1;  /* 00E6C4 SEM */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00E6DE;  /* 00E6C5 BBC */
    SETLO(A, rd8(DP(0x0C)));  /* 00E6CA LDA */
    /* 00E6CC CMP */
    if ((A & 0xFF) == 0x01) goto L_00E6D8;  /* 00E6CE BEQ */
    /* 00E6D0 CMP */
    if ((A & 0xFF) == 0x04) goto L_00E6D8;  /* 00E6D2 BEQ */
    /* 00E6D4 CMP */
    if ((A & 0xFF) != 0x05) goto L_00E6DE;  /* 00E6D6 BNE */
L_00E6D8: TICK();
    wr8(DP(0x0C), 0x01);  /* 00E6D8 LDM */
    goto L_00E6E2;  /* 00E6DB JMP */
L_00E6DE: TICK();
    mflag = 1;  /* 00E6DE SEM */
    wr8(DP(0x0C), 0x00);  /* 00E6DF LDM */
L_00E6E2: TICK();
    mflag = 0;  /* 00E6E2 CLM */
    if ((rd16(ABS(0x0244)) & 0x0002) == 0) goto L_00E6F1;  /* 00E6E3 BBC */
    mflag = 1;  /* 00E6E9 SEM */
    wr8(ABS(0x0189), 0x03);  /* 00E6EA LDM */
    goto L_00E72D;  /* 00E6EE JMP */
L_00E6F1: TICK();
    mflag = 0;  /* 00E6F1 CLM */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0) goto L_00E72D;  /* 00E6F2 BBC */
    mflag = 1;  /* 00E6F8 SEM */
    SETLO(A, rd8(DP(0x0C)));  /* 00E6F9 LDA */
    if ((A & 0xFF) == 0) goto L_00E728;  /* 00E6FB BEQ */
    if ((rd8(ABS(0x0340)) & 0x08) == 0) goto L_00E710;  /* 00E6FD BBC */
    SETLO(A, rd8(ABS(0x0228)));  /* 00E702 LDA */
    /* 00E705 CMP */
    if ((A & 0xFF) != 0x07) goto L_00E70D;  /* 00E707 BNE */
    wr8(ABS(0x0189), 0x02);  /* 00E709 LDM */
L_00E70D: TICK();
    goto L_00E725;  /* 00E70D JMP */
L_00E710: TICK();
    mflag = 0;  /* 00E710 CLM */
    A = rd16(ABS(0x0189));  /* 00E711 LDA */
    mflag = 1;  /* 00E714 SEM */
    /* 00E715 CMP */
    if ((A & 0xFF) != 0x01) goto L_00E720;  /* 00E717 BNE */
    wr8(ABS(0x0189), 0x02);  /* 00E719 LDM */
    goto L_00E725;  /* 00E71D JMP */
L_00E720: TICK();
    mflag = 1;  /* 00E720 SEM */
    wr8(ABS(0x0189), 0x01);  /* 00E721 LDM */
L_00E725: TICK();
    goto L_00E72D;  /* 00E725 JMP */
L_00E728: TICK();
    mflag = 1;  /* 00E728 SEM */
    wr8(ABS(0x0189), 0x02);  /* 00E729 LDM */
L_00E72D: TICK();
    mflag = 0;  /* 00E72D CLM */
    A = rd16(ABS(0x0189));  /* 00E72E LDA */
    mflag = 1;  /* 00E731 SEM */
    /* 00E732 CMP */
    if ((A & 0xFF) != 0x01) goto L_00E78A;  /* 00E734 BNE */
    wr8(DP(0x0A), 0x00);  /* 00E736 LDM */
L_00E739: TICK();
    Y = rd16(DP(0x0A));  /* 00E739 LDY */
    mflag = 0;  /* 00E73B CLM */
    S -= 3; F_0069BB();  /* 00E73C JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00E740 STA */
    SETLO(A, rd8(DP(0x0A)));  /* 00E742 LDA */
    if ((A & 0xFF) == 0) goto L_00E756;  /* 00E744 BEQ */
    mflag = 0;  /* 00E746 CLM */
    A &= 0x00FF;  /* 00E747 AND */
    A -= 1;  /* 00E74A DEC */
    X = A;  /* 00E74B TAX */
    A = rd16(DP(0x02 + X));  /* 00E74C LDA */
    mflag = 1;  /* 00E74E SEM */
    t = rd8(DP(0x00));  /* 00E74F CMP */
    if ((A & 0xFF) != t) goto L_00E756;  /* 00E751 BNE */
    wr8(DP(0x00), 0x0E);  /* 00E753 LDM */
L_00E756: TICK();
    SETLO(A, rd8(DP(0x0A)));  /* 00E756 LDA */
    mflag = 0;  /* 00E758 CLM */
    A &= 0x00FF;  /* 00E759 AND */
    X = A;  /* 00E75C TAX */
    A = rd16(DP(0x00));  /* 00E75D LDA */
    mflag = 1;  /* 00E75F SEM */
    wr8(DP(0x02 + X), (uint8_t)A);  /* 00E760 STA */
    /* 00E762 CMP */
    if ((A & 0xFF) != 0x0F) goto L_00E768;  /* 00E764 BNE */
    t = (rd8(DP(0x0A)) - 1) & 0xFF; wr8(DP(0x0A), t);  /* 00E766 DEC */
L_00E768: TICK();
    t = (rd8(DP(0x0A)) + 1) & 0xFF; wr8(DP(0x0A), t);  /* 00E768 INC */
    SETLO(A, rd8(DP(0x0A)));  /* 00E76A LDA */
    /* 00E76C CMP */
    if ((A & 0xFF) >= 0x06) goto L_00E776;  /* 00E76E BCS */
    SETLO(A, rd8(DP(0x00)));  /* 00E770 LDA */
    /* 00E772 CMP */
    if ((A & 0xFF) != 0x0F) goto L_00E739;  /* 00E774 BNE */
L_00E776: TICK();
    mflag = 0;  /* 00E776 CLM */
    if ((rd16(ABS(0x023A)) & 0x0008) == 0) goto L_00E787;  /* 00E777 BBC */
    A = rd16(DP(0x0A));  /* 00E77D LDA */
    A &= 0x00FF;  /* 00E77F AND */
    X = A;  /* 00E782 TAX */
    mflag = 1;  /* 00E783 SEM */
    wr8(DP(0x02 + X), 0x0A);  /* 00E784 LDM */
L_00E787: TICK();
    goto L_00E816;  /* 00E787 JMP */
L_00E78A: TICK();
    mflag = 0;  /* 00E78A CLM */
    A = rd16(ABS(0x0189));  /* 00E78B LDA */
    mflag = 1;  /* 00E78E SEM */
    /* 00E78F CMP */
    if ((A & 0xFF) != 0x03) goto L_00E7CD;  /* 00E791 BNE */
    wr8(DP(0x0A), 0x00);  /* 00E793 LDM */
L_00E796: TICK();
    SETLO(A, rd8(DP(0x0A)));  /* 00E796 LDA */
    /* 00E798 CMP */
    if ((A & 0xFF) >= 0x06) goto L_00E7CA;  /* 00E79A BCS */
    Y = rd16(DP(0x0A));  /* 00E79C LDY */
    mflag = 0;  /* 00E79E CLM */
    S -= 3; F_006A25();  /* 00E79F JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00E7A3 STA */
    SETLO(A, rd8(DP(0x0A)));  /* 00E7A5 LDA */
    if ((A & 0xFF) == 0) goto L_00E7B9;  /* 00E7A7 BEQ */
    mflag = 0;  /* 00E7A9 CLM */
    A &= 0x00FF;  /* 00E7AA AND */
    A -= 1;  /* 00E7AD DEC */
    X = A;  /* 00E7AE TAX */
    A = rd16(DP(0x02 + X));  /* 00E7AF LDA */
    mflag = 1;  /* 00E7B1 SEM */
    t = rd8(DP(0x00));  /* 00E7B2 CMP */
    if ((A & 0xFF) != t) goto L_00E7B9;  /* 00E7B4 BNE */
    wr8(DP(0x00), 0x0E);  /* 00E7B6 LDM */
L_00E7B9: TICK();
    SETLO(A, rd8(DP(0x0A)));  /* 00E7B9 LDA */
    mflag = 0;  /* 00E7BB CLM */
    A &= 0x00FF;  /* 00E7BC AND */
    X = A;  /* 00E7BF TAX */
    A = rd16(DP(0x00));  /* 00E7C0 LDA */
    mflag = 1;  /* 00E7C2 SEM */
    wr8(DP(0x02 + X), (uint8_t)A);  /* 00E7C3 STA */
    t = (rd8(DP(0x0A)) + 1) & 0xFF; wr8(DP(0x0A), t);  /* 00E7C5 INC */
    goto L_00E796;  /* 00E7C7 JMP */
L_00E7CA: TICK();
    goto L_00E816;  /* 00E7CA JMP */
L_00E7CD: TICK();
    mflag = 0;  /* 00E7CD CLM */
    A = rd16(ABS(0x0189));  /* 00E7CE LDA */
    mflag = 1;  /* 00E7D1 SEM */
    /* 00E7D2 CMP */
    if ((A & 0xFF) != 0x02) goto L_00E816;  /* 00E7D4 BNE */
    wr8(DP(0x0A), 0x00);  /* 00E7D6 LDM */
L_00E7D9: TICK();
    SETLO(A, rd8(ABS(0x0343)));  /* 00E7D9 LDA */
    SETLO(A, A & 0x0F);  /* 00E7DC AND */
    t = rd8(DP(0x0A));  /* 00E7DE CMP */
    C = ((A & 0xFF) >= t);
    Z = ((A & 0xFF) == t);
    if (!C) goto L_00E816;  /* 00E7E0 BCC */
    if (Z) goto L_00E816;  /* 00E7E2 BEQ */
    SETLO(A, rd8(DP(0x0A)));  /* 00E7E4 LDA */
    mflag = 0;  /* 00E7E6 CLM */
    A &= 0x00FF;  /* 00E7E7 AND */
    X = A;  /* 00E7EA TAX */
    A = rd16((ABS(0x02A4) + X));  /* 00E7EB LDA */
    mflag = 1;  /* 00E7EE SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00E7EF STA */
    SETLO(A, rd8(DP(0x0A)));  /* 00E7F1 LDA */
    if ((A & 0xFF) == 0) goto L_00E805;  /* 00E7F3 BEQ */
    mflag = 0;  /* 00E7F5 CLM */
    A &= 0x00FF;  /* 00E7F6 AND */
    A -= 1;  /* 00E7F9 DEC */
    X = A;  /* 00E7FA TAX */
    A = rd16(DP(0x02 + X));  /* 00E7FB LDA */
    mflag = 1;  /* 00E7FD SEM */
    t = rd8(DP(0x00));  /* 00E7FE CMP */
    if ((A & 0xFF) != t) goto L_00E805;  /* 00E800 BNE */
    wr8(DP(0x00), 0x0E);  /* 00E802 LDM */
L_00E805: TICK();
    SETLO(A, rd8(DP(0x0A)));  /* 00E805 LDA */
    mflag = 0;  /* 00E807 CLM */
    A &= 0x00FF;  /* 00E808 AND */
    X = A;  /* 00E80B TAX */
    A = rd16(DP(0x00));  /* 00E80C LDA */
    mflag = 1;  /* 00E80E SEM */
    wr8(DP(0x02 + X), (uint8_t)A);  /* 00E80F STA */
    t = (rd8(DP(0x0A)) + 1) & 0xFF; wr8(DP(0x0A), t);  /* 00E811 INC */
    goto L_00E7D9;  /* 00E813 JMP */
L_00E816: TICK();
    mflag = 0;  /* 00E816 CLM */
    A = rd16(ABS(0x0340));  /* 00E817 LDA */
    mflag = 1;  /* 00E81A SEM */
    SETLO(A, A & 0x03);  /* 00E81B AND */
    SETLO(A, A - 1);  /* 00E81D DEC */
    if ((A & 0xFF) != 0) goto L_00E838;  /* 00E81E BNE */
    wr8(DP(0x0B), 0x64);  /* 00E820 LDM */
    mflag = 0;  /* 00E823 CLM */
    wr16(DP(0x0D), 0x2891);  /* 00E824 LDM */
    mflag = 1;  /* 00E828 SEM */
    wr8(DP(0x0F), 0x00);  /* 00E829 LDM */
    mflag = 0;  /* 00E82C CLM */
    wr16(DP(0x10), 0x2873);  /* 00E82D LDM */
    mflag = 1;  /* 00E831 SEM */
    wr8(DP(0x12), 0x00);  /* 00E832 LDM */
    goto L_00E87B;  /* 00E835 JMP */
L_00E838: TICK();
    mflag = 0;  /* 00E838 CLM */
    A = rd16(ABS(0x0340));  /* 00E839 LDA */
    mflag = 1;  /* 00E83C SEM */
    SETLO(A, A & 0x03);  /* 00E83D AND */
    /* 00E83F CMP */
    if ((A & 0xFF) != 0x02) goto L_00E85B;  /* 00E841 BNE */
    wr8(DP(0x0B), 0x46);  /* 00E843 LDM */
    mflag = 0;  /* 00E846 CLM */
    wr16(DP(0x0D), 0x28BE);  /* 00E847 LDM */
    mflag = 1;  /* 00E84B SEM */
    wr8(DP(0x0F), 0x00);  /* 00E84C LDM */
    mflag = 0;  /* 00E84F CLM */
    wr16(DP(0x10), 0x28A0);  /* 00E850 LDM */
    mflag = 1;  /* 00E854 SEM */
    wr8(DP(0x12), 0x00);  /* 00E855 LDM */
    goto L_00E87B;  /* 00E858 JMP */
L_00E85B: TICK();
    mflag = 0;  /* 00E85B CLM */
    A = rd16(ABS(0x0340));  /* 00E85C LDA */
    mflag = 1;  /* 00E85F SEM */
    SETLO(A, A & 0x03);  /* 00E860 AND */
    /* 00E862 CMP */
    if ((A & 0xFF) != 0x03) goto L_00E87B;  /* 00E864 BNE */
    wr8(DP(0x0B), 0x28);  /* 00E866 LDM */
    mflag = 0;  /* 00E869 CLM */
    wr16(DP(0x0D), 0x28EB);  /* 00E86A LDM */
    mflag = 1;  /* 00E86E SEM */
    wr8(DP(0x0F), 0x00);  /* 00E86F LDM */
    mflag = 0;  /* 00E872 CLM */
    wr16(DP(0x10), 0x28CD);  /* 00E873 LDM */
    mflag = 1;  /* 00E877 SEM */
    wr8(DP(0x12), 0x00);  /* 00E878 LDM */
L_00E87B: TICK();
    SETLO(B, 0x00);  /* 00E87B LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00E87E LDA */
    div8(0x10);  /* 00E881 DIV */
    wr8(DP(0x0C), (uint8_t)A);  /* 00E884 STA */
    SETLO(A, rd8(ABS(0x0228)));  /* 00E886 LDA */
    /* 00E889 CMP */
    if ((A & 0xFF) == 0x07) goto L_00E8B1;  /* 00E88B BEQ */
    SETLO(B, 0x00);  /* 00E88D LDA */
    SETLO(A, rd8(ABS(0x0343)));  /* 00E890 LDA */
    div8(0x10);  /* 00E893 DIV */
    /* 00E896 CMP */
    if ((A & 0xFF) == 0x00) goto L_00E8B1;  /* 00E898 BEQ */
    SETLO(A, rd8(DP(0x0C)));  /* 00E89A LDA */
    /* 00E89C CMP */
    if ((A & 0xFF) == 0x03) goto L_00E8A4;  /* 00E89E BEQ */
    /* 00E8A0 CMP */
    if ((A & 0xFF) != 0x05) goto L_00E8B8;  /* 00E8A2 BNE */
L_00E8A4: TICK();
    mflag = 0;  /* 00E8A4 CLM */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0x0001) goto L_00E8B8;  /* 00E8A5 BBS */
    if ((rd16(ABS(0x0244)) & 0x0002) == 0x0002) goto L_00E8B8;  /* 00E8AB BBS */
L_00E8B1: TICK();
    mflag = 0;  /* 00E8B1 CLM */
    A = 0x0001;  /* 00E8B2 LDA */
    goto L_00E8C2;  /* 00E8B5 JMP */
L_00E8B8: TICK();
    mflag = 1;  /* 00E8B8 SEM */
    SETLO(B, 0x00);  /* 00E8B9 LDA */
    SETLO(A, rd8(ABS(0x0343)));  /* 00E8BC LDA */
    div8(0x10);  /* 00E8BF DIV */
L_00E8C2: TICK();
    mflag = 1;  /* 00E8C2 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 00E8C3 STA */
    wr8(DP(0x0A), 0x00);  /* 00E8C5 LDM */
    SETLO(A, rd8(DP(0x02)));  /* 00E8C8 LDA */
    wr8(DP(0x00), (uint8_t)A);  /* 00E8CA STA */
    C = ((A & 0xFF) >= 0x0F);  /* 00E8CC CMP */
    Z = ((A & 0xFF) == 0x0F);
    if (Z) goto L_00E910;  /* 00E8CE BEQ */
    wr8(ABS(TA1IC), 0x00);  /* 00E8D0 LDM */
    wr8(ABS(TA1MOD), 0x06);  /* 00E8D4 LDM */
    wr8(ABS(TA1IC), rd8(ABS(TA1IC)) & ~0x08);  /* 00E8D8 CLB */
    wr8(ABS(TB1IC), 0x06);  /* 00E8DC LDM */
    wr8(ABS(TB1MOD), 0x00);  /* 00E8E0 LDM */
    wr8(ABS(TB1IC), rd8(ABS(TB1IC)) & ~0x08);  /* 00E8E4 CLB */
    wr8(ABS(TA4IC), 0x00);  /* 00E8E8 LDM */
    wr8(ABS(TA4MOD), 0x80);  /* 00E8EC LDM */
    SETLO(A, rd8(DP(0x0B)));  /* 00E8F0 LDA */
    mpy8(0x7E);  /* 00E8F2 MPY */
    wr8(DP(0x15), (uint8_t)A);  /* 00E8F5 STA */
    wr8(DP(0x16), (uint8_t)B);  /* 00E8F7 STA */
    mflag = 0;  /* 00E8FA CLM */
    X = rd16(DP(0x15));  /* 00E8FB LDX */
    wr16(ABS(TA4), X);  /* 00E8FD STX */
    mflag = 1;  /* 00E900 SEM */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 00E901 CLB */
    SETLO(A, 0x01);  /* 00E905 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00E907 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00E908 STA */
    setP(pull16());  /* 00E90A PLP */
    mflag = 1;  /* 00E90B SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x10);  /* 00E90C SEB */
L_00E910: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00E910 LDA */
    C = ((A & 0xFF) >= 0x0F);  /* 00E912 CMP */
    Z = ((A & 0xFF) == 0x0F);
    if (!Z) goto L_00E919;  /* 00E914 BNE */
    goto L_00E9A4;  /* 00E916 JMP */
L_00E919: TICK();
    mflag = 0;  /* 00E919 CLM */
    A &= 0x00FF;  /* 00E91A AND */
    C = 0;  /* 00E91D CLC */
    A = adc16(A, rd16(DP(0x0D)));  /* 00E91E ADC */
    wr16(DP(0x15), A);  /* 00E920 STA */
    A = rd16(DP(0x0F));  /* 00E922 LDA */
    mflag = 1;  /* 00E924 SEM */
    wr8(DP(0x17), (uint8_t)A);  /* 00E925 STA */
    SETLO(A, rd8(LIND(0x15)));  /* 00E927 LDA */
    wr8(ABS(0x0260), (uint8_t)A);  /* 00E929 STA */
    SETLO(A, rd8(DP(0x00)));  /* 00E92C LDA */
    mflag = 0;  /* 00E92E CLM */
    A &= 0x00FF;  /* 00E92F AND */
    A = asl16(A);  /* 00E932 ASL */
    C = 0;  /* 00E933 CLC */
    A = adc16(A, rd16(DP(0x10)));  /* 00E934 ADC */
    wr16(DP(0x15), A);  /* 00E936 STA */
    A = rd16(DP(0x12));  /* 00E938 LDA */
    mflag = 1;  /* 00E93A SEM */
    wr8(DP(0x17), (uint8_t)A);  /* 00E93B STA */
    mflag = 0;  /* 00E93D CLM */
    A = rd16(LIND(0x15));  /* 00E93E LDA */
    wr16(ABS(TB1), A);  /* 00E940 STA */
    mflag = 1;  /* 00E943 SEM */
    wr8(ABS(0x025F), 0x00);  /* 00E944 LDM */
    mflag = 0;  /* 00E948 CLM */
    A = rd16(0x0028FA);  /* 00E949 LDA */
    wr16(ABS(TA1), A);  /* 00E94D STA */
    mflag = 1;  /* 00E950 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x02);  /* 00E951 SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x40);  /* 00E955 SEB */
    wr8(ABS(TOSTART), rd8(ABS(TOSTART)) | 0x02);  /* 00E959 SEB */
    SETLO(A, rd8(ABS(0x025F)));  /* 00E95D LDA */
    C = 0;  /* 00E960 CLC */
    SETLO(A, adc8(A, rd8(ABS(0x0260))));  /* 00E961 ADC */
    wr8(ABS(0x025F), (uint8_t)A);  /* 00E964 STA */
    mflag = 0;  /* 00E967 CLM */
    A &= 0x00FF;  /* 00E968 AND */
    A = asl16(A);  /* 00E96B ASL */
    X = A;  /* 00E96C TAX */
    A = rd16((0x0028FA + X));  /* 00E96D LDA */
    wr16(ABS(TA1), A);  /* 00E971 STA */
    mflag = 1;  /* 00E974 SEM */
    t = (rd8(DP(0x0A)) + 1) & 0xFF; wr8(DP(0x0A), t);  /* 00E975 INC */
    SETLO(A, rd8(DP(0x0A)));  /* 00E977 LDA */
    mflag = 0;  /* 00E979 CLM */
    A &= 0x00FF;  /* 00E97A AND */
    X = A;  /* 00E97D TAX */
    A = rd16(DP(0x02 + X));  /* 00E97E LDA */
    mflag = 1;  /* 00E980 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00E981 STA */
L_00E983: TICK();
    wr8(ABS(WDT), 0x00);  /* 00E983 LDM */
    if ((rd8(ABS(TA4IC)) & 0x08) == 0) goto L_00E983;  /* 00E987 BBC */
    wr8(ABS(TA4IC), rd8(ABS(TA4IC)) & ~0x08);  /* 00E98C CLB */
    t = (rd8(DP(0x01)) - 1) & 0xFF; wr8(DP(0x01), t);  /* 00E990 DEC */
    SETLO(A, rd8(DP(0x01)));  /* 00E992 LDA */
    if ((A & 0xFF) != 0) goto L_00E983;  /* 00E994 BNE */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x02);  /* 00E996 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x40);  /* 00E99A CLB */
    wr8(DP(0x01), 0x01);  /* 00E99E LDM */
    goto L_00E910;  /* 00E9A1 JMP */
L_00E9A4: TICK();
    mflag = 1;  /* 00E9A4 SEM */
    SETLO(A, 0x00);  /* 00E9A5 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00E9A7 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00E9A8 STA */
    setP(pull16());  /* 00E9AA PLP */
    mflag = 1;  /* 00E9AB SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x10);  /* 00E9AC CLB */
    wr8(ABS(TB1IC), 0x00);  /* 00E9B0 LDM */
    SETLO(A, rd8(DP(0x0B)));  /* 00E9B4 LDA */
    mpy8(0x05);  /* 00E9B6 MPY */
    wr8(DP(0x15), (uint8_t)A);  /* 00E9B9 STA */
    wr8(DP(0x16), (uint8_t)B);  /* 00E9BB STA */
    mflag = 0;  /* 00E9BE CLM */
    A = rd16(DP(0x15));  /* 00E9BF LDA */
    B = 0x0004;  /* 00E9C1 LDA */
    S -= 3; F_00FF8C();  /* 00E9C5 JSRL */
    Y = A;  /* 00E9C9 TAY */
    S -= 3; F_007D7F();  /* 00E9CA JSRL */
L_00E9CE: TICK();
    mflag = 0;  /* 00E9CE CLM */
    Y = pull16();  /* 00E9CF PLY */
    D = pull16();  /* 00E9D0 PLD */
    S += 3; return;  /* 00E9D1 RTL */
}

/* 00E9D2  callers: 00EA4D, 00EAA2, 00EB7A, 00EC86 */
void F_00E9D2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00E9D2 LDA */
    push16(D);  /* 00E9D4 PHD */
    D = A;  /* 00E9D5 TAD */
    wr16(DP(0x00), Y);  /* 00E9D6 STY */
    A = Y;  /* 00E9D8 TYA */
    mflag = 1;  /* 00E9D9 SEM */
    /* 00E9DA CMP */
    if ((A & 0xFF) != 0x00) goto L_00E9ED;  /* 00E9DC BNE */
    mflag = 0;  /* 00E9DE CLM */
    if ((rd16(ABS(0x023E)) & 0x0080) == 0x0080) goto L_00E9EA;  /* 00E9DF BBS */
    mflag = 1;  /* 00E9E5 SEM */
    wr8(ABS(P4), rd8(ABS(P4)) & ~0x40);  /* 00E9E6 CLB [P46 SUSTURMA_BILGISI] */
L_00E9EA: TICK();
    goto L_00EA0E;  /* 00E9EA JMP */
L_00E9ED: TICK();
    mflag = 1;  /* 00E9ED SEM */
    /* 00E9EE CMP */
    if ((A & 0xFF) != 0x01) goto L_00EA0E;  /* 00E9F0 BNE */
    if ((rd8(ABS(0x023E)) & 0x20) == 0x20) goto L_00E9FE;  /* 00E9F2 BBS */
    mflag = 0;  /* 00E9F7 CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_00EA06;  /* 00E9F8 BBC */
L_00E9FE: TICK();
    mflag = 1;  /* 00E9FE SEM */
    wr8(ABS(P4), rd8(ABS(P4)) | 0x40);  /* 00E9FF SEB [P46 SUSTURMA_BILGISI] */
    goto L_00EA0B;  /* 00EA03 JMP */
L_00EA06: TICK();
    mflag = 1;  /* 00EA06 SEM */
    wr8(ABS(P4), rd8(ABS(P4)) & ~0x40);  /* 00EA07 CLB [P46 SUSTURMA_BILGISI] */
L_00EA0B: TICK();
    goto L_00EA0E;  /* 00EA0B JMP */
L_00EA0E: TICK();
    mflag = 0;  /* 00EA0E CLM */
    D = pull16();  /* 00EA0F PLD */
    S += 3; return;  /* 00EA10 RTL */
}

/* 00EA11  callers: 00F42F */
void F_00EA11(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EA11 LDA */
    push16(A);  /* 00EA13 PHA */
    mflag = 1;  /* 00EA14 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00EA2E;  /* 00EA15 BBS */
    SETLO(A, rd8(ABS(P7)));  /* 00EA1A LDA */
    SETLO(A, A & 0x02);  /* 00EA1D AND */
    SETLO(A, lsr8(A));  /* 00EA1F LSR */
    if (!Z) goto L_00EA2E;  /* 00EA20 BNE */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x02);  /* 00EA22 SEB [P71 GUC_ACMA] */
    Y = 0x0001;  /* 00EA26 LDY */
    mflag = 0;  /* 00EA29 CLM */
    S -= 3; wait_synth_lock_00D3B4();  /* 00EA2A JSRL */
L_00EA2E: TICK();
    wr8(ABS(TA3IC), 0x00);  /* 00EA2E LDM */
    mflag = 0;  /* 00EA32 CLM */
    S -= 3; F_007CF6();  /* 00EA33 JSRL */
    S -= 3; F_00F934();  /* 00EA37 JSRL */
    mflag = 1;  /* 00EA3B SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x04);  /* 00EA3C CLB [P72 GONDERME_SES(AN2)] */
    wr8(ABS(P5), rd8(ABS(P5)) | 0x20);  /* 00EA40 SEB [P55 ALMA_SEC] */
    mflag = 0;  /* 00EA44 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 00EA45 CLB */
    Y = 0x0000;  /* 00EA4A LDY */
    S -= 3; F_00E9D2();  /* 00EA4D JSRL */
    Y = 0x0000;  /* 00EA51 LDY */
    S -= 3; synth_select_rx_tx_words_00D2A3();  /* 00EA54 JSRL */
    S -= 3; F_00D4F9();  /* 00EA58 JSRL */
    S -= 3; synth_load_00D45C();  /* 00EA5C JSRL */
    S -= 3; synth_load_and_wait_00D51D();  /* 00EA60 JSRL */
    Y = pull16();  /* 00EA64 PLY */
    S += 3; return;  /* 00EA65 RTL */
}

/* 00EA66  callers: 004C8F, 00EB28, 00F2E3, 00F307, 00F31A, 016ACE, 016BED, 01CA45 */
void F_00EA66(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EA66 LDA */
    push16(A);  /* 00EA68 PHA */
    mflag = 1;  /* 00EA69 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00EA7F;  /* 00EA6A BBS */
    mflag = 0;  /* 00EA6F CLM */
    if ((rd16(ABS(0x023E)) & 0x0080) == 0x0080) goto L_00EA7F;  /* 00EA70 BBS */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x4000);  /* 00EA76 SEB */
    S -= 3; cbus_write_sel2_00755E();  /* 00EA7B JSRL */
L_00EA7F: TICK();
    mflag = 0;  /* 00EA7F CLM */
    S -= 3; F_007CF6();  /* 00EA80 JSRL */
    S -= 3; F_00F934();  /* 00EA84 JSRL */
    S -= 3; F_00D512();  /* 00EA88 JSRL */
    mflag = 1;  /* 00EA8C SEM */
    wr8(ABS(TA3IC), 0x00);  /* 00EA8D LDM */
    mflag = 0;  /* 00EA91 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 00EA92 CLB */
    S -= 3; F_007A06();  /* 00EA97 JSRL */
    S -= 3; F_00CB52();  /* 00EA9B JSRL */
    Y = 0x0000;  /* 00EA9F LDY */
    S -= 3; F_00E9D2();  /* 00EAA2 JSRL */
    Y = pull16();  /* 00EAA6 PLY */
    S += 3; return;  /* 00EAA7 RTL */
}

/* 00EAA8  callers: 00F344, 00F849 */
void F_00EAA8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EAA8 LDA */
    push16(A);  /* 00EAAA PHA */
    mflag = 1;  /* 00EAAB SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_00EAB4;  /* 00EAAC BBC */
    goto L_00EB6D;  /* 00EAB1 JMP */
L_00EAB4: TICK();
    mflag = 0;  /* 00EAB4 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00EABE;  /* 00EAB5 BBC */
    goto L_00EB6D;  /* 00EABB JMP */
L_00EABE: TICK();
    if ((rd16(ABS(0x0246)) & 0x0002) == 0) goto L_00EAC7;  /* 00EABE BBC */
    goto L_00EB6D;  /* 00EAC4 JMP */
L_00EAC7: TICK();
    mflag = 1;  /* 00EAC7 SEM */
    if ((rd8(ABS(0x0340)) & 0x40) == 0x40) goto L_00EAD0;  /* 00EAC8 BBS */
    goto L_00EB6D;  /* 00EACD JMP */
L_00EAD0: TICK();
    SETLO(A, rd8(ABS(0x0229)));  /* 00EAD0 LDA */
    /* 00EAD3 CMP */
    if ((A & 0xFF) == 0x04) goto L_00EAE8;  /* 00EAD5 BEQ */
    /* 00EAD7 CMP */
    if ((A & 0xFF) == 0x03) goto L_00EAE8;  /* 00EAD9 BEQ */
    mflag = 0;  /* 00EADB CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0x2000) goto L_00EAE8;  /* 00EADC BBS */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0) goto L_00EAF6;  /* 00EAE2 BBC */
L_00EAE8: TICK();
    mflag = 1;  /* 00EAE8 SEM */
    wr8(ABS(0x0207), 0x3F);  /* 00EAE9 LDM */
    mflag = 0;  /* 00EAED CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0080);  /* 00EAEE CLB */
    goto L_00EB6D;  /* 00EAF3 JMP */
L_00EAF6: TICK();
    mflag = 0;  /* 00EAF6 CLM */
    A = rd16(ABS(0x0228));  /* 00EAF7 LDA */
    mflag = 1;  /* 00EAFA SEM */
    /* 00EAFB CMP */
    if ((A & 0xFF) == 0x01) goto L_00EB06;  /* 00EAFD BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 00EAFF LDA */
    /* 00EB02 CMP */
    if ((A & 0xFF) != 0x02) goto L_00EB51;  /* 00EB04 BNE */
L_00EB06: TICK();
    SETLO(A, rd8(ABS(0x0207)));  /* 00EB06 LDA */
    if ((A & 0xFF) != 0) goto L_00EB4E;  /* 00EB09 BNE */
    SETLO(A, rd8(ABS(0x022A)));  /* 00EB0B LDA */
    if ((A & 0xFF) != 0) goto L_00EB17;  /* 00EB0E BNE */
    mflag = 0;  /* 00EB10 CLM */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0) goto L_00EB1D;  /* 00EB11 BBC */
L_00EB17: TICK();
    mflag = 1;  /* 00EB17 SEM */
    SETLO(A, rd8(ABS(0x022E)));  /* 00EB18 LDA */
    if ((A & 0xFF) != 0) goto L_00EB4E;  /* 00EB1B BNE */
L_00EB1D: TICK();
    mflag = 0;  /* 00EB1D CLM */
    X = rd16(ABS(0x0214));  /* 00EB1E LDX */
    if (X != 0) goto L_00EB4E;  /* 00EB21 BNE */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0080);  /* 00EB23 SEB */
    S -= 3; F_00EA66();  /* 00EB28 JSRL */
    mflag = 1;  /* 00EB2C SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 00EB2D CLB [P71 GUC_ACMA] */
    wr8(ABS(0x0229), 0x00);  /* 00EB31 LDM */
    wr8(ABS(0x0228), 0x00);  /* 00EB35 LDM */
    SETLO(A, rd8(ABS(0x0355)));  /* 00EB39 LDA */
    mflag = 0;  /* 00EB3C CLM */
    A &= 0x00FF;  /* 00EB3D AND */
    C = 0;  /* 00EB40 CLC */
    A = adc16(A, 0x0010);  /* 00EB41 ADC */
    A = lsr16(A);  /* 00EB44 LSR */
    A = lsr16(A);  /* 00EB45 LSR */
    A = lsr16(A);  /* 00EB46 LSR */
    A = lsr16(A);  /* 00EB47 LSR */
    A = lsr16(A);  /* 00EB48 LSR */
    mflag = 1;  /* 00EB49 SEM */
    SETLO(A, A + 1);  /* 00EB4A INC */
    wr8(ABS(0x0207), (uint8_t)A);  /* 00EB4B STA */
L_00EB4E: TICK();
    goto L_00EB6D;  /* 00EB4E JMP */
L_00EB51: TICK();
    mflag = 1;  /* 00EB51 SEM */
    SETLO(A, rd8(ABS(0x0228)));  /* 00EB52 LDA */
    if ((A & 0xFF) != 0) goto L_00EB6D;  /* 00EB55 BNE */
    SETLO(A, rd8(ABS(0x0207)));  /* 00EB57 LDA */
    if ((A & 0xFF) != 0) goto L_00EB6D;  /* 00EB5A BNE */
    mflag = 0;  /* 00EB5C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0080);  /* 00EB5D CLB */
    mflag = 1;  /* 00EB62 SEM */
    wr8(ABS(0x0228), 0x01);  /* 00EB63 LDM */
    mflag = 0;  /* 00EB67 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00EB68 CLB */
L_00EB6D: TICK();
    mflag = 0;  /* 00EB6D CLM */
    Y = pull16();  /* 00EB6E PLY */
    S += 3; return;  /* 00EB6F RTL */
}

/* 00EB70  callers: 00F4A3 */
void F_00EB70(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EB70 LDA */
    push16(D);  /* 00EB72 PHD */
    D = A;  /* 00EB73 TAD */
    A += 1;  /* 00EB74 INC */
    A += 1;  /* 00EB75 INC */
    push16(A);  /* 00EB76 PHA */
    Y = 0x0001;  /* 00EB77 LDY */
    S -= 3; F_00E9D2();  /* 00EB7A JSRL */
    mflag = 1;  /* 00EB7E SEM */
    if ((rd8(ABS(0x023E)) & 0x20) == 0x20) goto L_00EB87;  /* 00EB7F BBS */
    goto L_00EC4B;  /* 00EB84 JMP */
L_00EB87: TICK();
    mflag = 0;  /* 00EB87 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x2000);  /* 00EB88 CLB */
    mflag = 1;  /* 00EB8D SEM */
    wr8(ABS(0x0228), 0x03);  /* 00EB8E LDM */
    SETLO(A, rd8(ABS(0x0229)));  /* 00EB92 LDA */
    /* 00EB95 CMP */
    if ((A & 0xFF) != 0x01) goto L_00EBA2;  /* 00EB97 BNE */
    mflag = 0;  /* 00EB99 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x1000);  /* 00EB9A SEB */
    goto L_00EBB1;  /* 00EB9F JMP */
L_00EBA2: TICK();
    mflag = 0;  /* 00EBA2 CLM */
    A = rd16(ABS(0x0229));  /* 00EBA3 LDA */
    mflag = 1;  /* 00EBA6 SEM */
    /* 00EBA7 CMP */
    if ((A & 0xFF) != 0x02) goto L_00EBB1;  /* 00EBA9 BNE */
    mflag = 0;  /* 00EBAB CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x1000);  /* 00EBAC CLB */
L_00EBB1: TICK();
    mflag = 1;  /* 00EBB1 SEM */
    wr8(DP(0x00), 0x00);  /* 00EBB2 LDM */
    if ((rd8(ABS(0x0276)) & 0x20) == 0) goto L_00EBD2;  /* 00EBB5 BBC */
    if ((rd8(ABS(0x0335)) & 0x04) == 0x04) goto L_00EBD2;  /* 00EBBA BBS */
    wr8(DP(0x00), 0x01);  /* 00EBBF LDM */
    mflag = 0;  /* 00EBC2 CLM */
    A = rd16(ABS(0x0298));  /* 00EBC3 LDA */
    A &= 0x000F;  /* 00EBC6 AND */
    mflag = 1;  /* 00EBC9 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 00EBCA STA */
    mflag = 0;  /* 00EBCC CLM */
    wr16(ABS(0x0298), rd16(ABS(0x0298)) & ~0x000F);  /* 00EBCD CLB */
L_00EBD2: TICK();
    mflag = 0;  /* 00EBD2 CLM */
    S -= 3; F_00CB52();  /* 00EBD3 JSRL */
    mflag = 1;  /* 00EBD7 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00EBE7;  /* 00EBD8 BBS */
    mflag = 0;  /* 00EBDD CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x4000);  /* 00EBDE CLB */
    S -= 3; cbus_write_sel2_00755E();  /* 00EBE3 JSRL */
L_00EBE7: TICK();
    mflag = 0;  /* 00EBE7 CLM */
    S -= 3; ctcss_rx_start_00F8BD();  /* 00EBE8 JSRL */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0) goto L_00EBF6;  /* 00EBEC BBC */
    S -= 3; F_007C46();  /* 00EBF2 JSRL */
L_00EBF6: TICK();
    S -= 3; F_007A06();  /* 00EBF6 JSRL */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0040);  /* 00EBFA CLB */
    A = rd16(ABS(0x0348));  /* 00EBFF LDA */
    mflag = 1;  /* 00EC02 SEM */
    wr8(ABS(0x022C), (uint8_t)A);  /* 00EC03 STA */
    wr8(ABS(0x0237), 0x02);  /* 00EC06 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 00EC0A LDA */
    if ((A & 0xFF) == 0) goto L_00EC4B;  /* 00EC0C BEQ */
    Y = 0x000A;  /* 00EC0E LDY */
    mflag = 0;  /* 00EC11 CLM */
    S -= 3; F_007D7F();  /* 00EC12 JSRL */
    A = rd16(DP(0x01));  /* 00EC16 LDA */
    mflag = 1;  /* 00EC18 SEM */
    SETLO(A, A & 0x0F);  /* 00EC19 AND */
    wr8(ABS(0x0298), rd8(ABS(0x0298)) & ~0x0F);  /* 00EC1B CLB */
    SETLO(A, A | rd8(ABS(0x0298)));  /* 00EC1F ORA */
    wr8(ABS(0x0298), (uint8_t)A);  /* 00EC22 STA */
    SETLO(A, rd8(DP(0x01)));  /* 00EC25 LDA */
    SETLO(A, A & 0x0F);  /* 00EC27 AND */
    wr8(ABS(0x0296), rd8(ABS(0x0296)) & ~0x0F);  /* 00EC29 CLB */
    SETLO(A, A | rd8(ABS(0x0296)));  /* 00EC2D ORA */
    wr8(ABS(0x0296), (uint8_t)A);  /* 00EC30 STA */
    mflag = 0;  /* 00EC33 CLM */
    A &= 0x00FF;  /* 00EC34 AND */
    wr16(DP(0x02), A);  /* 00EC37 STA */
    wr16(DP(0x04), 0x0010);  /* 00EC39 LDM */
    Y = 0x0002;  /* 00EC3D LDY */
    A = D;  /* 00EC40 TDA */
    C = 0;  /* 00EC41 CLC */
    A = adc16(A, 0x0006);  /* 00EC42 ADC */
    push16(A);  /* 00EC45 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00EC46 JSRL */
    Y = pull16();  /* 00EC4A PLY */
L_00EC4B: TICK();
    if ((rd8(ABS(P6)) & 0x80) == 0) goto L_00EC7A;  /* 00EC4B BBC [P67 SENTEZOR_KILIT] */
    SETLO(A, rd8(ABS(0x034C)));  /* 00EC50 LDA */
    mflag = 0;  /* 00EC53 CLM */
    A &= 0x00FF;  /* 00EC54 AND */
    Y = A;  /* 00EC57 TAY */
    S -= 3; synth_check_lock_00D2F6();  /* 00EC58 JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 00EC5C STA */
    SETLO(A, A & 0x01);  /* 00EC5E AND */
    SETLO(A, asl8(A));  /* 00EC60 ASL */
    SETLO(A, asl8(A));  /* 00EC61 ASL */
    SETLO(A, asl8(A));  /* 00EC62 ASL */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x08);  /* 00EC63 CLB */
    SETLO(A, A | rd8(ABS(0x023E)));  /* 00EC67 ORA */
    wr8(ABS(0x023E), (uint8_t)A);  /* 00EC6A STA */
    mflag = 0;  /* 00EC6D CLM */
    A = rd16(DP(0x02));  /* 00EC6E LDA */
    A &= 0x0001;  /* 00EC70 AND */
    if (A != 0) goto L_00EC7A;  /* 00EC73 BNE */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00EC75 CLB */
L_00EC7A: TICK();
    mflag = 0;  /* 00EC7A CLM */
    Y = pull16();  /* 00EC7B PLY */
    D = pull16();  /* 00EC7C PLD */
    S += 3; return;  /* 00EC7D RTL */
}

/* 00EC7E  callers: 00F4B1 */
void F_00EC7E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EC7E LDA */
    push16(D);  /* 00EC80 PHD */
    D = A;  /* 00EC81 TAD */
    push16(A);  /* 00EC82 PHA */
    Y = 0x0001;  /* 00EC83 LDY */
    S -= 3; F_00E9D2();  /* 00EC86 JSRL */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0x0020) goto L_00ECCB;  /* 00EC8A BBS */
    S -= 3; F_007CF6();  /* 00EC90 JSRL */
    mflag = 1;  /* 00EC94 SEM */
    SETLO(A, rd8(ABS(0x026C)));  /* 00EC95 LDA */
    if ((A & 0xFF) == 0) goto L_00EC9F;  /* 00EC98 BEQ */
    mflag = 0;  /* 00EC9A CLM */
    S -= 3; F_0062AB();  /* 00EC9B JSRL */
L_00EC9F: TICK();
    mflag = 0;  /* 00EC9F CLM */
    S -= 3; F_00F934();  /* 00ECA0 JSRL */
    S -= 3; F_00CB52();  /* 00ECA4 JSRL */
    mflag = 1;  /* 00ECA8 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00ECB8;  /* 00ECA9 BBS */
    mflag = 0;  /* 00ECAE CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x4000);  /* 00ECAF SEB */
    S -= 3; cbus_write_sel2_00755E();  /* 00ECB4 JSRL */
L_00ECB8: TICK();
    mflag = 0;  /* 00ECB8 CLM */
    S -= 3; F_007A06();  /* 00ECB9 JSRL */
    A = rd16(ABS(0x0347));  /* 00ECBD LDA */
    mflag = 1;  /* 00ECC0 SEM */
    wr8(ABS(0x022C), (uint8_t)A);  /* 00ECC1 STA */
    wr8(ABS(0x0228), 0x02);  /* 00ECC4 LDM */
    goto L_00ECDB;  /* 00ECC8 JMP */
L_00ECCB: TICK();
    mflag = 0;  /* 00ECCB CLM */
    if ((rd16(ABS(0x023E)) & 0x2000) == 0) goto L_00ECDB;  /* 00ECCC BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x2000);  /* 00ECD2 CLB */
    S -= 3; F_00CB52();  /* 00ECD7 JSRL */
L_00ECDB: TICK();
    mflag = 1;  /* 00ECDB SEM */
    if ((rd8(ABS(P6)) & 0x80) == 0) goto L_00ED0B;  /* 00ECDC BBC [P67 SENTEZOR_KILIT] */
    SETLO(A, rd8(ABS(0x034C)));  /* 00ECE1 LDA */
    mflag = 0;  /* 00ECE4 CLM */
    A &= 0x00FF;  /* 00ECE5 AND */
    Y = A;  /* 00ECE8 TAY */
    S -= 3; synth_check_lock_00D2F6();  /* 00ECE9 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00ECED STA */
    SETLO(A, A & 0x01);  /* 00ECEF AND */
    SETLO(A, asl8(A));  /* 00ECF1 ASL */
    SETLO(A, asl8(A));  /* 00ECF2 ASL */
    SETLO(A, asl8(A));  /* 00ECF3 ASL */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x08);  /* 00ECF4 CLB */
    SETLO(A, A | rd8(ABS(0x023E)));  /* 00ECF8 ORA */
    wr8(ABS(0x023E), (uint8_t)A);  /* 00ECFB STA */
    mflag = 0;  /* 00ECFE CLM */
    A = rd16(DP(0x00));  /* 00ECFF LDA */
    A &= 0x0001;  /* 00ED01 AND */
    if (A != 0) goto L_00ED0B;  /* 00ED04 BNE */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00ED06 CLB */
L_00ED0B: TICK();
    mflag = 0;  /* 00ED0B CLM */
    Y = pull16();  /* 00ED0C PLY */
    D = pull16();  /* 00ED0D PLD */
    S += 3; return;  /* 00ED0E RTL */
}

/* 00ED0F  callers: 00F4BF */
void F_00ED0F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00ED0F LDA */
    push16(A);  /* 00ED11 PHA */
    mflag = 1;  /* 00ED12 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00ED2C;  /* 00ED13 BBS */
    SETLO(A, rd8(ABS(P7)));  /* 00ED18 LDA */
    SETLO(A, A & 0x02);  /* 00ED1B AND */
    SETLO(A, lsr8(A));  /* 00ED1D LSR */
    if (!Z) goto L_00ED2C;  /* 00ED1E BNE */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x02);  /* 00ED20 SEB [P71 GUC_ACMA] */
    Y = 0x0001;  /* 00ED24 LDY */
    mflag = 0;  /* 00ED27 CLM */
    S -= 3; wait_synth_lock_00D3B4();  /* 00ED28 JSRL */
L_00ED2C: TICK();
    wr8(ABS(TA3IC), 0x00);  /* 00ED2C LDM */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x04);  /* 00ED30 SEB [P72 GONDERME_SES(AN2)] */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x20);  /* 00ED34 CLB [P55 ALMA_SEC] */
    mflag = 0;  /* 00ED38 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0800);  /* 00ED39 CLB */
    Y = 0x0001;  /* 00ED3E LDY */
    S -= 3; synth_select_rx_tx_words_00D2A3();  /* 00ED41 JSRL */
    S -= 3; F_00CDE8();  /* 00ED45 JSRL */
    S -= 3; synth_load_00D45C();  /* 00ED49 JSRL */
    S -= 3; synth_load_and_wait_00D51D();  /* 00ED4D JSRL */
    Y = pull16();  /* 00ED51 PLY */
    S += 3; return;  /* 00ED52 RTL */
}

/* 00ED53  callers: 00F4C9 */
void F_00ED53(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00ED53 LDA */
    push16(D);  /* 00ED55 PHD */
    D = A;  /* 00ED56 TAD */
    A += 1;  /* 00ED57 INC */
    A += 1;  /* 00ED58 INC */
    A += 1;  /* 00ED59 INC */
    push16(A);  /* 00ED5A PHA */
    mflag = 1;  /* 00ED5B SEM */
    SETLO(B, 0x00);  /* 00ED5C LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00ED5F LDA */
    div8(0x10);  /* 00ED62 DIV */
    wr8(DP(0x00), (uint8_t)A);  /* 00ED65 STA */
    mflag = 0;  /* 00ED67 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0010);  /* 00ED68 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0004);  /* 00ED6D SEB */
    S -= 3; tx_power_loop_00CFC4();  /* 00ED72 JSRL */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x4000);  /* 00ED76 CLB */
    mflag = 1;  /* 00ED7B SEM */
    if ((rd8(ABS(0x0334)) & 0x04) == 0) goto L_00ED8F;  /* 00ED7C BBC */
    mflag = 0;  /* 00ED81 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0200);  /* 00ED82 CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0400);  /* 00ED87 CLB */
    goto L_00ED9A;  /* 00ED8C JMP */
L_00ED8F: TICK();
    mflag = 0;  /* 00ED8F CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0200);  /* 00ED90 CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0400);  /* 00ED95 SEB */
L_00ED9A: TICK();
    S -= 3; cbus_write_sel2_00755E();  /* 00ED9A JSRL */
    S -= 3; F_007A06();  /* 00ED9E JSRL */
    if ((rd16(ABS(0x0244)) & 0x0002) == 0x0002) goto L_00EDC4;  /* 00EDA2 BBS */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0x0001) goto L_00EDC4;  /* 00EDA8 BBS */
    if ((rd16(ABS(0x023A)) & 0x0008) == 0x0008) goto L_00EDC4;  /* 00EDAE BBS */
    mflag = 1;  /* 00EDB4 SEM */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00EDE5;  /* 00EDB5 BBC */
    SETLO(A, rd8(DP(0x00)));  /* 00EDBA LDA */
    /* 00EDBC CMP */
    if ((A & 0xFF) == 0x02) goto L_00EDC4;  /* 00EDBE BEQ */
    /* 00EDC0 CMP */
    if ((A & 0xFF) != 0x04) goto L_00EDE5;  /* 00EDC2 BNE */
L_00EDC4: TICK();
    mflag = 1;  /* 00EDC4 SEM */
    wr8(ABS(0x0228), 0x05);  /* 00EDC5 LDM */
    SETLO(A, rd8(ABS(0x0342)));  /* 00EDC9 LDA */
    SETLO(A, A & 0x0F);  /* 00EDCC AND */
    mflag = 0;  /* 00EDCE CLM */
    A &= 0x00FF;  /* 00EDCF AND */
    wr16(DP(0x01), A);  /* 00EDD2 STA */
    mpy16(0x0064);  /* 00EDD4 MPY */
    A = lsr16(A);  /* 00EDD8 LSR */
    A = lsr16(A);  /* 00EDD9 LSR */
    A = lsr16(A);  /* 00EDDA LSR */
    A = lsr16(A);  /* 00EDDB LSR */
    A = lsr16(A);  /* 00EDDC LSR */
    mflag = 1;  /* 00EDDD SEM */
    SETLO(A, A + 1);  /* 00EDDE INC */
    wr8(ABS(0x022A), (uint8_t)A);  /* 00EDDF STA */
    goto L_00EDF0;  /* 00EDE2 JMP */
L_00EDE5: TICK();
    mflag = 0;  /* 00EDE5 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0800);  /* 00EDE6 SEB */
    mflag = 1;  /* 00EDEB SEM */
    wr8(ABS(0x0228), 0x08);  /* 00EDEC LDM */
L_00EDF0: TICK();
    Y = 0x0002;  /* 00EDF0 LDY */
    mflag = 0;  /* 00EDF3 CLM */
    S -= 3; F_00927F();  /* 00EDF4 JSRL */
    S -= 3; ctcss_tx_start_00F854();  /* 00EDF8 JSRL */
    mflag = 1;  /* 00EDFC SEM */
    SETLO(B, 0x00);  /* 00EDFD LDA */
    SETLO(A, rd8(ABS(0x0345)));  /* 00EE00 LDA */
    div8(0x10);  /* 00EE03 DIV */
    mflag = 0;  /* 00EE06 CLM */
    A &= 0x00FF;  /* 00EE07 AND */
    mpy16(0x01D5);  /* 00EE0A MPY */
    wr16(ABS(0x0218), A);  /* 00EE0E STA */
    Y = pull16();  /* 00EE11 PLY */
    D = pull16();  /* 00EE12 PLD */
    S += 3; return;  /* 00EE13 RTL */
}

/* 00EE14  callers: 00F753 */
void F_00EE14(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EE14 LDA */
    push16(D);  /* 00EE16 PHD */
    D = A;  /* 00EE17 TAD */
    push16(A);  /* 00EE18 PHA */
    mflag = 1;  /* 00EE19 SEM */
    if ((rd8(ABS(P6)) & 0x80) == 0) goto L_00EE49;  /* 00EE1A BBC [P67 SENTEZOR_KILIT] */
    SETLO(A, rd8(ABS(0x034C)));  /* 00EE1F LDA */
    mflag = 0;  /* 00EE22 CLM */
    A &= 0x00FF;  /* 00EE23 AND */
    Y = A;  /* 00EE26 TAY */
    S -= 3; synth_check_lock_00D2F6();  /* 00EE27 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 00EE2B STA */
    SETLO(A, A & 0x01);  /* 00EE2D AND */
    SETLO(A, asl8(A));  /* 00EE2F ASL */
    SETLO(A, asl8(A));  /* 00EE30 ASL */
    SETLO(A, asl8(A));  /* 00EE31 ASL */
    wr8(ABS(0x023E), rd8(ABS(0x023E)) & ~0x08);  /* 00EE32 CLB */
    SETLO(A, A | rd8(ABS(0x023E)));  /* 00EE36 ORA */
    wr8(ABS(0x023E), (uint8_t)A);  /* 00EE39 STA */
    mflag = 0;  /* 00EE3C CLM */
    A = rd16(DP(0x00));  /* 00EE3D LDA */
    A &= 0x0001;  /* 00EE3F AND */
    if (A != 0) goto L_00EE49;  /* 00EE42 BNE */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00EE44 CLB */
L_00EE49: TICK();
    mflag = 0;  /* 00EE49 CLM */
    Y = pull16();  /* 00EE4A PLY */
    D = pull16();  /* 00EE4B PLD */
    S += 3; return;  /* 00EE4C RTL */
}

/* 00EE4D  callers: 00F77F, 00F79F */
void F_00EE4D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EE4D LDA */
    push16(D);  /* 00EE4F PHD */
    D = A;  /* 00EE50 TAD */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0) goto L_00EE63;  /* 00EE51 BBC */
    mflag = 1;  /* 00EE57 SEM */
    wr8(ABS(0x0228), 0x09);  /* 00EE58 LDM */
    wr8(ABS(0x022A), 0x10);  /* 00EE5C LDM */
    goto L_00EE88;  /* 00EE60 JMP */
L_00EE63: TICK();
    mflag = 1;  /* 00EE63 SEM */
    SETLO(B, 0x00);  /* 00EE64 LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00EE67 LDA */
    div8(0x10);  /* 00EE6A DIV */
    wr8(DP(0x00), (uint8_t)A);  /* 00EE6D STA */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00EE83;  /* 00EE6F BBC */
    /* 00EE74 CMP */
    if ((A & 0xFF) == 0x03) goto L_00EE7C;  /* 00EE76 BEQ */
    /* 00EE78 CMP */
    if ((A & 0xFF) != 0x05) goto L_00EE83;  /* 00EE7A BNE */
L_00EE7C: TICK();
    wr8(ABS(0x0228), 0x06);  /* 00EE7C LDM */
    goto L_00EE88;  /* 00EE80 JMP */
L_00EE83: TICK();
    mflag = 1;  /* 00EE83 SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00EE84 LDM */
L_00EE88: TICK();
    mflag = 0;  /* 00EE88 CLM */
    D = pull16();  /* 00EE89 PLD */
    S += 3; return;  /* 00EE8A RTL */
}

/* 00EE8B  callers: 00F367, 00F37A, 00F39D, 00F3B8, 00F3CB, 00F824 */
void F_00EE8B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EE8B LDA */
    push16(A);  /* 00EE8D PHA */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x4000);  /* 00EE8E SEB */
    mflag = 1;  /* 00EE93 SEM */
    if ((rd8(ABS(0x0334)) & 0x04) == 0) goto L_00EEA7;  /* 00EE94 BBC */
    mflag = 0;  /* 00EE99 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0200);  /* 00EE9A CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0400);  /* 00EE9F CLB */
    goto L_00EEB2;  /* 00EEA4 JMP */
L_00EEA7: TICK();
    mflag = 0;  /* 00EEA7 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0200);  /* 00EEA8 SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0400);  /* 00EEAD CLB */
L_00EEB2: TICK();
    S -= 3; cbus_write_sel2_00755E();  /* 00EEB2 JSRL */
    S -= 3; ctcss_tx_stop_00F8AE();  /* 00EEB6 JSRL */
    S -= 3; F_00CDE8();  /* 00EEBA JSRL */
    mflag = 1;  /* 00EEBE SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x04);  /* 00EEBF CLB [P72 GONDERME_SES(AN2)] */
    wr8(ABS(P5), rd8(ABS(P5)) | 0x20);  /* 00EEC3 SEB [P55 ALMA_SEC] */
    wr8(ABS(0x0244), rd8(ABS(0x0244)) & ~0x01);  /* 00EEC7 CLB */
    wr8(ABS(0x0244), rd8(ABS(0x0244)) & ~0x02);  /* 00EECB CLB */
    mflag = 0;  /* 00EECF CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0800);  /* 00EED0 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x1000);  /* 00EED5 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0010);  /* 00EEDA SEB */
    S -= 3; F_007A06();  /* 00EEDF JSRL */
    S -= 3; F_00CB52();  /* 00EEE3 JSRL */
    Y = pull16();  /* 00EEE7 PLY */
    S += 3; return;  /* 00EEE8 RTL */
}

/* 00EEE9  callers: 00F3FB */
void F_00EEE9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00EEE9 LDA */
    push16(D);  /* 00EEEB PHD */
    D = A;  /* 00EEEC TAD */
    push16(A);  /* 00EEED PHA */
    A = rd16(ABS(0x0229));  /* 00EEEE LDA */
    mflag = 1;  /* 00EEF1 SEM */
    /* 00EEF2 CMP */
    if ((A & 0xFF) == 0x04) goto L_00EEFD;  /* 00EEF4 BEQ */
    SETLO(A, rd8(ABS(0x0229)));  /* 00EEF6 LDA */
    /* 00EEF9 CMP */
    if ((A & 0xFF) != 0x03) goto L_00EF05;  /* 00EEFB BNE */
L_00EEFD: TICK();
    mflag = 0;  /* 00EEFD CLM */
    S -= 3; F_0095FA();  /* 00EEFE JSRL */
    goto L_00F291;  /* 00EF02 JMP */
L_00EF05: TICK();
    mflag = 0;  /* 00EF05 CLM */
    if ((rd16(ABS(0x0244)) & 0x0400) == 0) goto L_00EF6B;  /* 00EF06 BBC */
    A = rd16(ABS(0x0228));  /* 00EF0C LDA */
    mflag = 1;  /* 00EF0F SEM */
    /* 00EF10 CMP */
    if ((A & 0xFF) == 0x01) goto L_00EF1B;  /* 00EF12 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 00EF14 LDA */
    /* 00EF17 CMP */
    if ((A & 0xFF) != 0x02) goto L_00EF59;  /* 00EF19 BNE */
L_00EF1B: TICK();
    SETLO(A, rd8(ABS(0x022A)));  /* 00EF1B LDA */
    if ((A & 0xFF) != 0) goto L_00EF27;  /* 00EF1E BNE */
    mflag = 0;  /* 00EF20 CLM */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0) goto L_00EF2D;  /* 00EF21 BBC */
L_00EF27: TICK();
    mflag = 1;  /* 00EF27 SEM */
    SETLO(A, rd8(ABS(0x022E)));  /* 00EF28 LDA */
    if ((A & 0xFF) != 0) goto L_00EF56;  /* 00EF2B BNE */
L_00EF2D: TICK();
    mflag = 0;  /* 00EF2D CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0400);  /* 00EF2E CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00EF33 CLB */
    wr16(DP(0x00), 0x0363);  /* 00EF38 LDM */
    wr16(DP(0x02), 0x0370);  /* 00EF3C LDM */
    A = 0x0000;  /* 00EF40 LDA */
    Y = 0x0007;  /* 00EF43 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00EF46 JSRL */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0800);  /* 00EF4A SEB */
    A = rd16(ABS(0x0350));  /* 00EF4F LDA */
    mflag = 1;  /* 00EF52 SEM */
    wr8(ABS(0x020A), (uint8_t)A);  /* 00EF53 STA */
L_00EF56: TICK();
    goto L_00EF68;  /* 00EF56 JMP */
L_00EF59: TICK();
    mflag = 0;  /* 00EF59 CLM */
    A = rd16(ABS(0x0228));  /* 00EF5A LDA */
    mflag = 1;  /* 00EF5D SEM */
    /* 00EF5E CMP */
    if ((A & 0xFF) != 0x03) goto L_00EF68;  /* 00EF60 BNE */
    mflag = 0;  /* 00EF62 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0200);  /* 00EF63 CLB */
L_00EF68: TICK();
    goto L_00F217;  /* 00EF68 JMP */
L_00EF6B: TICK();
    mflag = 0;  /* 00EF6B CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_00EF75;  /* 00EF6C BBC */
    goto L_00F041;  /* 00EF72 JMP */
L_00EF75: TICK();
    A = rd16(ABS(0x0228));  /* 00EF75 LDA */
    mflag = 1;  /* 00EF78 SEM */
    /* 00EF79 CMP */
    if ((A & 0xFF) == 0x01) goto L_00EF84;  /* 00EF7B BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 00EF7D LDA */
    /* 00EF80 CMP */
    if ((A & 0xFF) != 0x02) goto L_00EFB6;  /* 00EF82 BNE */
L_00EF84: TICK();
    SETLO(A, rd8(ABS(0x022A)));  /* 00EF84 LDA */
    if ((A & 0xFF) != 0) goto L_00EF90;  /* 00EF87 BNE */
    mflag = 0;  /* 00EF89 CLM */
    if ((rd16(ABS(0x023E)) & 0x0020) == 0) goto L_00EF96;  /* 00EF8A BBC */
L_00EF90: TICK();
    mflag = 1;  /* 00EF90 SEM */
    SETLO(A, rd8(ABS(0x022E)));  /* 00EF91 LDA */
    if ((A & 0xFF) != 0) goto L_00EFB3;  /* 00EF94 BNE */
L_00EF96: TICK();
    mflag = 0;  /* 00EF96 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0200);  /* 00EF97 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00EF9C CLB */
    wr16(DP(0x00), 0x037D);  /* 00EFA1 LDM */
    wr16(DP(0x02), 0x0370);  /* 00EFA5 LDM */
    A = 0x0000;  /* 00EFA9 LDA */
    Y = 0x0007;  /* 00EFAC LDY */
    S -= 3; rt_memcpy_01E466();  /* 00EFAF JSRL */
L_00EFB3: TICK();
    goto L_00F03E;  /* 00EFB3 JMP */
L_00EFB6: TICK();
    mflag = 0;  /* 00EFB6 CLM */
    A = rd16(ABS(0x0228));  /* 00EFB7 LDA */
    mflag = 1;  /* 00EFBA SEM */
    /* 00EFBB CMP */
    if ((A & 0xFF) != 0x03) goto L_00F03E;  /* 00EFBD BNE */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_00F033;  /* 00EFBF BBC */
    SETLO(A, rd8(ABS(0x0375)));  /* 00EFC4 LDA */
    SETLO(A, A & 0x3F);  /* 00EFC7 AND */
    if ((A & 0xFF) == 0) goto L_00F033;  /* 00EFC9 BEQ */
    mflag = 0;  /* 00EFCB CLM */
    if ((rd16(ABS(0x0244)) & 0x0200) == 0x0200) goto L_00EFF9;  /* 00EFCC BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0200);  /* 00EFD2 SEB */
    A = rd16(ABS(0x0375));  /* 00EFD7 LDA */
    mflag = 1;  /* 00EFDA SEM */
    SETLO(A, A & 0x3F);  /* 00EFDB AND */
    mflag = 0;  /* 00EFDD CLM */
    A &= 0x00FF;  /* 00EFDE AND */
    A = asl16(A);  /* 00EFE1 ASL */
    A -= 1;  /* 00EFE2 DEC */
    A -= 1;  /* 00EFE3 DEC */
    X = A;  /* 00EFE4 TAX */
    B = 0x0000;  /* 00EFE5 LDA */
    A = rd16((0x002385 + X));  /* 00EFE9 LDA */
    div16(0x03F0);  /* 00EFED DIV */
    mflag = 1;  /* 00EFF1 SEM */
    SETLO(A, A + 1);  /* 00EFF2 INC */
    wr8(ABS(0x020A), (uint8_t)A);  /* 00EFF3 STA */
    goto L_00F030;  /* 00EFF6 JMP */
L_00EFF9: TICK();
    mflag = 0;  /* 00EFF9 CLM */
    if ((rd16(ABS(0x023A)) & 0x1000) == 0) goto L_00F00D;  /* 00EFFA BBC */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0100);  /* 00F000 SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0200);  /* 00F005 CLB */
    goto L_00F030;  /* 00F00A JMP */
L_00F00D: TICK();
    mflag = 1;  /* 00F00D SEM */
    SETLO(A, rd8(ABS(0x020A)));  /* 00F00E LDA */
    if ((A & 0xFF) != 0) goto L_00F030;  /* 00F011 BNE */
    mflag = 0;  /* 00F013 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0200);  /* 00F014 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00F019 CLB */
    wr16(DP(0x00), 0x037D);  /* 00F01E LDM */
    wr16(DP(0x02), 0x0370);  /* 00F022 LDM */
    A = 0x0000;  /* 00F026 LDA */
    Y = 0x0007;  /* 00F029 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F02C JSRL */
L_00F030: TICK();
    goto L_00F03E;  /* 00F030 JMP */
L_00F033: TICK();
    mflag = 0;  /* 00F033 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0100);  /* 00F034 SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0200);  /* 00F039 CLB */
L_00F03E: TICK();
    goto L_00F217;  /* 00F03E JMP */
L_00F041: TICK();
    mflag = 0;  /* 00F041 CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0x2000) goto L_00F072;  /* 00F042 BBS */
    A = rd16(ABS(0x0228));  /* 00F048 LDA */
    mflag = 1;  /* 00F04B SEM */
    /* 00F04C CMP */
    if ((A & 0xFF) != 0x03) goto L_00F08B;  /* 00F04E BNE */
    mflag = 0;  /* 00F050 CLM */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0) goto L_00F072;  /* 00F051 BBC */
    A = rd16(ABS(0x0375));  /* 00F057 LDA */
    mflag = 1;  /* 00F05A SEM */
    SETLO(A, A & 0x3F);  /* 00F05B AND */
    if ((A & 0xFF) == 0) goto L_00F072;  /* 00F05D BEQ */
    mflag = 0;  /* 00F05F CLM */
    if ((rd16(ABS(0x023A)) & 0x1000) == 0x1000) goto L_00F072;  /* 00F060 BBS */
    if ((rd16(ABS(0x0244)) & 0x4000) == 0) goto L_00F08B;  /* 00F066 BBC */
    mflag = 1;  /* 00F06C SEM */
    SETLO(A, rd8(ABS(0x020A)));  /* 00F06D LDA */
    if ((A & 0xFF) == 0) goto L_00F08B;  /* 00F070 BEQ */
L_00F072: TICK();
    mflag = 0;  /* 00F072 CLM */
    A = rd16(ABS(0x0344));  /* 00F073 LDA */
    mflag = 1;  /* 00F076 SEM */
    SETLO(A, A & 0x0F);  /* 00F077 AND */
    mflag = 0;  /* 00F079 CLM */
    A &= 0x00FF;  /* 00F07A AND */
    mpy16(0x01F4);  /* 00F07D MPY */
    A = lsr16(A);  /* 00F081 LSR */
    A = lsr16(A);  /* 00F082 LSR */
    A = lsr16(A);  /* 00F083 LSR */
    A = lsr16(A);  /* 00F084 LSR */
    A = lsr16(A);  /* 00F085 LSR */
    mflag = 1;  /* 00F086 SEM */
    SETLO(A, A + 1);  /* 00F087 INC */
    wr8(ABS(0x0207), (uint8_t)A);  /* 00F088 STA */
L_00F08B: TICK();
    mflag = 0;  /* 00F08B CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0080);  /* 00F08C CLB */
    mflag = 1;  /* 00F091 SEM */
    SETLO(A, rd8(ABS(0x0207)));  /* 00F092 LDA */
    if ((A & 0xFF) != 0) goto L_00F102;  /* 00F095 BNE */
    SETLO(A, rd8(ABS(0x0231)));  /* 00F097 LDA */
    /* 00F09A CMP */
    if ((A & 0xFF) == 0x15) goto L_00F102;  /* 00F09C BEQ */
    wr8(ABS(0x0230), 0x00);  /* 00F09E LDM */
    mflag = 0;  /* 00F0A2 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0100);  /* 00F0A3 CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0200);  /* 00F0A8 CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0800);  /* 00F0AD CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00F0B2 CLB */
    A = rd16(ABS(0x035C));  /* 00F0B7 LDA */
    mflag = 1;  /* 00F0BA SEM */
    t = rd8(ABS(0x0369));  /* 00F0BB CMP */
    if ((A & 0xFF) == t) goto L_00F0DA;  /* 00F0BE BEQ */
    mflag = 0;  /* 00F0C0 CLM */
    wr16(DP(0x00), 0x0356);  /* 00F0C1 LDM */
    wr16(DP(0x02), 0x0363);  /* 00F0C5 LDM */
    A = 0x0000;  /* 00F0C9 LDA */
    Y = 0x000D;  /* 00F0CC LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F0CF JSRL */
    Y = rd16(ABS(0x035C));  /* 00F0D3 LDY */
    S -= 3; F_008A47();  /* 00F0D6 JSRL */
L_00F0DA: TICK();
    mflag = 0;  /* 00F0DA CLM */
    wr16(DP(0x00), 0x037D);  /* 00F0DB LDM */
    wr16(DP(0x02), 0x0370);  /* 00F0DF LDM */
    A = 0x0000;  /* 00F0E3 LDA */
    Y = 0x0007;  /* 00F0E6 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F0E9 JSRL */
    Y = 0x002C;  /* 00F0ED LDY */
    S -= 3; F_003310();  /* 00F0F0 JSRL */
    S -= 3; F_00828C();  /* 00F0F4 JSRL */
    Y = 0x0004;  /* 00F0F8 LDY */
    S -= 3; F_00927F();  /* 00F0FB JSRL */
    goto L_00F217;  /* 00F0FF JMP */
L_00F102: TICK();
    mflag = 1;  /* 00F102 SEM */
    SETLO(A, rd8(ABS(0x0209)));  /* 00F103 LDA */
    if ((A & 0xFF) == 0) goto L_00F10B;  /* 00F106 BEQ */
    goto L_00F217;  /* 00F108 JMP */
L_00F10B: TICK();
    SETLO(A, rd8(ABS(0x038A)));  /* 00F10B LDA */
    t = rd8(ABS(0x0369));  /* 00F10E CMP */
    if ((A & 0xFF) != t) goto L_00F116;  /* 00F111 BNE */
    goto L_00F20F;  /* 00F113 JMP */
L_00F116: TICK();
    mflag = 0;  /* 00F116 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0400);  /* 00F117 SEB */
    if ((rd16(ABS(0x0244)) & 0x8000) == 0x8000) goto L_00F170;  /* 00F11C BBS */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x8000);  /* 00F122 SEB */
    mflag = 1;  /* 00F127 SEM */
    SETLO(A, rd8(ABS(0x038A)));  /* 00F128 LDA */
    if ((A & 0xFF) == 0) goto L_00F143;  /* 00F12B BEQ */
    mflag = 0;  /* 00F12D CLM */
    wr16(DP(0x00), 0x0384);  /* 00F12E LDM */
    wr16(DP(0x02), 0x0370);  /* 00F132 LDM */
    A = 0x0000;  /* 00F136 LDA */
    Y = 0x0007;  /* 00F139 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F13C JSRL */
    goto L_00F16D;  /* 00F140 JMP */
L_00F143: TICK();
    mflag = 1;  /* 00F143 SEM */
    SETLO(A, rd8(ABS(0x0391)));  /* 00F144 LDA */
    if ((A & 0xFF) == 0) goto L_00F167;  /* 00F147 BEQ */
    SETLO(A, rd8(ABS(0x0369)));  /* 00F149 LDA */
    t = rd8(ABS(0x0391));  /* 00F14C CMP */
    if ((A & 0xFF) == t) goto L_00F167;  /* 00F14F BEQ */
    mflag = 0;  /* 00F151 CLM */
    wr16(DP(0x00), 0x038B);  /* 00F152 LDM */
    wr16(DP(0x02), 0x0370);  /* 00F156 LDM */
    A = 0x0000;  /* 00F15A LDA */
    Y = 0x0007;  /* 00F15D LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F160 JSRL */
    goto L_00F16D;  /* 00F164 JMP */
L_00F167: TICK();
    mflag = 0;  /* 00F167 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0400);  /* 00F168 CLB */
L_00F16D: TICK();
    goto L_00F1BC;  /* 00F16D JMP */
L_00F170: TICK();
    mflag = 0;  /* 00F170 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x8000);  /* 00F171 CLB */
    mflag = 1;  /* 00F176 SEM */
    SETLO(A, rd8(ABS(0x0391)));  /* 00F177 LDA */
    if ((A & 0xFF) == 0) goto L_00F19A;  /* 00F17A BEQ */
    SETLO(A, rd8(ABS(0x0369)));  /* 00F17C LDA */
    t = rd8(ABS(0x0391));  /* 00F17F CMP */
    if ((A & 0xFF) == t) goto L_00F19A;  /* 00F182 BEQ */
    mflag = 0;  /* 00F184 CLM */
    wr16(DP(0x00), 0x038B);  /* 00F185 LDM */
    wr16(DP(0x02), 0x0370);  /* 00F189 LDM */
    A = 0x0000;  /* 00F18D LDA */
    Y = 0x0007;  /* 00F190 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F193 JSRL */
    goto L_00F1BC;  /* 00F197 JMP */
L_00F19A: TICK();
    mflag = 1;  /* 00F19A SEM */
    SETLO(A, rd8(ABS(0x038A)));  /* 00F19B LDA */
    if ((A & 0xFF) == 0) goto L_00F1B6;  /* 00F19E BEQ */
    mflag = 0;  /* 00F1A0 CLM */
    wr16(DP(0x00), 0x0384);  /* 00F1A1 LDM */
    wr16(DP(0x02), 0x0370);  /* 00F1A5 LDM */
    A = 0x0000;  /* 00F1A9 LDA */
    Y = 0x0007;  /* 00F1AC LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F1AF JSRL */
    goto L_00F1BC;  /* 00F1B3 JMP */
L_00F1B6: TICK();
    mflag = 0;  /* 00F1B6 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0400);  /* 00F1B7 CLB */
L_00F1BC: TICK();
    if ((rd16(ABS(0x0244)) & 0x0400) == 0) goto L_00F20F;  /* 00F1BC BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00F1C2 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0080);  /* 00F1C7 SEB */
    mflag = 1;  /* 00F1CC SEM */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_00F1DB;  /* 00F1CD BBC */
    mflag = 0;  /* 00F1D2 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x1000);  /* 00F1D3 SEB */
    goto L_00F1E1;  /* 00F1D8 JMP */
L_00F1DB: TICK();
    mflag = 0;  /* 00F1DB CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x1000);  /* 00F1DC CLB */
L_00F1E1: TICK();
    mflag = 1;  /* 00F1E1 SEM */
    if ((rd8(ABS(0x0240)) & 0x01) == 0) goto L_00F1F0;  /* 00F1E2 BBC */
    mflag = 0;  /* 00F1E7 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x2000);  /* 00F1E8 SEB */
    goto L_00F1F6;  /* 00F1ED JMP */
L_00F1F0: TICK();
    mflag = 0;  /* 00F1F0 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x2000);  /* 00F1F1 CLB */
L_00F1F6: TICK();
    if ((rd16(ABS(0x023A)) & 0x1000) == 0) goto L_00F204;  /* 00F1F6 BBC */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x4000);  /* 00F1FC SEB */
    goto L_00F20A;  /* 00F201 JMP */
L_00F204: TICK();
    mflag = 0;  /* 00F204 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x4000);  /* 00F205 CLB */
L_00F20A: TICK();
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0800);  /* 00F20A CLB */
L_00F20F: TICK();
    mflag = 0;  /* 00F20F CLM */
    A = rd16(ABS(0x034D));  /* 00F210 LDA */
    mflag = 1;  /* 00F213 SEM */
    wr8(ABS(0x0209), (uint8_t)A);  /* 00F214 STA */
L_00F217: TICK();
    mflag = 0;  /* 00F217 CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_00F291;  /* 00F218 BBC */
    if ((rd16(ABS(0x0244)) & 0x0200) == 0x0200) goto L_00F291;  /* 00F21E BBS */
    Y = rd16(ABS(0x0376));  /* 00F224 LDY */
    S -= 3; F_008D14();  /* 00F227 JSRL */
    if ((rd16(ABS(0x0244)) & 0x0400) == 0) goto L_00F236;  /* 00F22B BBC */
    mflag = 1;  /* 00F231 SEM */
    wr8(ABS(0x0375), rd8(ABS(0x0375)) & ~0x3F);  /* 00F232 CLB */
L_00F236: TICK();
    mflag = 0;  /* 00F236 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0080);  /* 00F237 CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0400);  /* 00F23C CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0200);  /* 00F241 SEB */
    wr16(DP(0x00), 0x0370);  /* 00F246 LDM */
    wr16(DP(0x02), 0x0363);  /* 00F24A LDM */
    A = 0x0000;  /* 00F24E LDA */
    Y = 0x000D;  /* 00F251 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00F254 JSRL */
    Y = 0x002C;  /* 00F258 LDY */
    S -= 3; F_003310();  /* 00F25B JSRL */
    S -= 3; F_00828C();  /* 00F25F JSRL */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0800);  /* 00F263 CLB */
    S -= 3; F_007A06();  /* 00F268 JSRL */
    Y = 0x0003;  /* 00F26C LDY */
    S -= 3; F_00927F();  /* 00F26F JSRL */
    A = rd16(ABS(0x0344));  /* 00F273 LDA */
    mflag = 1;  /* 00F276 SEM */
    SETLO(A, A & 0x0F);  /* 00F277 AND */
    mflag = 0;  /* 00F279 CLM */
    A &= 0x00FF;  /* 00F27A AND */
    mpy16(0x01F4);  /* 00F27D MPY */
    A = lsr16(A);  /* 00F281 LSR */
    A = lsr16(A);  /* 00F282 LSR */
    A = lsr16(A);  /* 00F283 LSR */
    A = lsr16(A);  /* 00F284 LSR */
    A = lsr16(A);  /* 00F285 LSR */
    mflag = 1;  /* 00F286 SEM */
    SETLO(A, A + 1);  /* 00F287 INC */
    wr8(ABS(0x0207), (uint8_t)A);  /* 00F288 STA */
    SETLO(A, rd8(ABS(0x034D)));  /* 00F28B LDA */
    wr8(ABS(0x0209), (uint8_t)A);  /* 00F28E STA */
L_00F291: TICK();
    mflag = 0;  /* 00F291 CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_00F29D;  /* 00F292 BBS */
    mflag = 1;  /* 00F298 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 00F299 LDM */
L_00F29D: TICK();
    mflag = 0;  /* 00F29D CLM */
    Y = pull16();  /* 00F29E PLY */
    D = pull16();  /* 00F29F PLD */
    S += 3; return;  /* 00F2A0 RTL */
}

/* 00F2A1  callers: 00F422 */
void F_00F2A1(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00F2A1 LDA */
    push16(A);  /* 00F2A3 PHA */
    mflag = 1;  /* 00F2A4 SEM */
    SETLO(A, rd8(ABS(0x0228)));  /* 00F2A5 LDA */
    if ((A & 0xFF) == 0) goto L_00F2B9;  /* 00F2A8 BEQ */
    /* 00F2AA CMP */
    if ((A & 0xFF) == 0x01) goto L_00F2B9;  /* 00F2AC BEQ */
    /* 00F2AE CMP */
    if ((A & 0xFF) == 0x02) goto L_00F2B9;  /* 00F2B0 BEQ */
    /* 00F2B2 CMP */
    if ((A & 0xFF) == 0x03) goto L_00F2B9;  /* 00F2B4 BEQ */
    goto L_00F34B;  /* 00F2B6 JMP */
L_00F2B9: TICK();
    SETLO(A, rd8(ABS(0x0229)));  /* 00F2B9 LDA */
    /* 00F2BC CMP */
    if ((A & 0xFF) == 0x04) goto L_00F2C4;  /* 00F2BE BEQ */
    /* 00F2C0 CMP */
    if ((A & 0xFF) != 0x03) goto L_00F2EF;  /* 00F2C2 BNE */
L_00F2C4: TICK();
    if ((rd8(ABS(0x04C2)) & 0x08) == 0) goto L_00F2E2;  /* 00F2C4 BBC */
    mflag = 0;  /* 00F2C9 CLM */
    if ((rd16(ABS(0x023C)) & 0x0004) == 0x0004) goto L_00F2E2;  /* 00F2CA BBS */
    Y = 0x0008;  /* 00F2D0 LDY */
    S -= 3; F_0080AA();  /* 00F2D3 JSRL */
L_00F2D7: TICK();
    mflag = 1;  /* 00F2D7 SEM */
    wr8(ABS(WDT), 0x00);  /* 00F2D8 LDM */
    mflag = 0;  /* 00F2DC CLM */
    X = rd16(ABS(0x0214));  /* 00F2DD LDX */
    if (X != 0) goto L_00F2D7;  /* 00F2E0 BNE */
L_00F2E2: TICK();
    mflag = 0;  /* 00F2E2 CLM */
    S -= 3; F_00EA66();  /* 00F2E3 JSRL */
    mflag = 1;  /* 00F2E7 SEM */
    wr8(ABS(0x0228), 0x04);  /* 00F2E8 LDM */
    goto L_00F348;  /* 00F2EC JMP */
L_00F2EF: TICK();
    mflag = 1;  /* 00F2EF SEM */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F2F0 LDA */
    if ((A & 0xFF) != 0) goto L_00F313;  /* 00F2F3 BNE */
    mflag = 0;  /* 00F2F5 CLM */
    X = rd16(ABS(0x0214));  /* 00F2F6 LDX */
    if (X != 0) goto L_00F300;  /* 00F2F9 BNE */
    mflag = 1;  /* 00F2FB SEM */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 00F2FC CLB [P71 GUC_ACMA] */
L_00F300: TICK();
    mflag = 1;  /* 00F300 SEM */
    SETLO(A, rd8(ABS(0x0228)));  /* 00F301 LDA */
    if ((A & 0xFF) == 0) goto L_00F310;  /* 00F304 BEQ */
    mflag = 0;  /* 00F306 CLM */
    S -= 3; F_00EA66();  /* 00F307 JSRL */
    mflag = 1;  /* 00F30B SEM */
    wr8(ABS(0x0228), 0x00);  /* 00F30C LDM */
L_00F310: TICK();
    goto L_00F348;  /* 00F310 JMP */
L_00F313: TICK();
    mflag = 0;  /* 00F313 CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_00F326;  /* 00F314 BBS */
    S -= 3; F_00EA66();  /* 00F31A JSRL */
    mflag = 1;  /* 00F31E SEM */
    wr8(ABS(0x0228), 0x01);  /* 00F31F LDM */
    goto L_00F348;  /* 00F323 JMP */
L_00F326: TICK();
    mflag = 0;  /* 00F326 CLM */
    A = rd16(ABS(0x0229));  /* 00F327 LDA */
    mflag = 1;  /* 00F32A SEM */
    /* 00F32B CMP */
    if ((A & 0xFF) != 0x02) goto L_00F348;  /* 00F32D BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 00F32F LDA */
    if ((A & 0xFF) != 0) goto L_00F348;  /* 00F332 BNE */
    mflag = 0;  /* 00F334 CLM */
    if ((rd16(ABS(0x0240)) & 0x0080) == 0x0080) goto L_00F343;  /* 00F335 BBS */
    mflag = 1;  /* 00F33B SEM */
    wr8(ABS(0x0228), 0x01);  /* 00F33C LDM */
    goto L_00F348;  /* 00F340 JMP */
L_00F343: TICK();
    mflag = 0;  /* 00F343 CLM */
    S -= 3; F_00EAA8();  /* 00F344 JSRL */
L_00F348: TICK();
    goto L_00F3E5;  /* 00F348 JMP */
L_00F34B: TICK();
    mflag = 0;  /* 00F34B CLM */
    A = rd16(ABS(0x0228));  /* 00F34C LDA */
    mflag = 1;  /* 00F34F SEM */
    /* 00F350 CMP */
    if ((A & 0xFF) == 0x04) goto L_00F35B;  /* 00F352 BEQ */
    SETLO(A, rd8(ABS(0x0228)));  /* 00F354 LDA */
    /* 00F357 CMP */
    if ((A & 0xFF) != 0x0C) goto L_00F3A9;  /* 00F359 BNE */
L_00F35B: TICK();
    SETLO(A, rd8(ABS(0x0229)));  /* 00F35B LDA */
    /* 00F35E CMP */
    if ((A & 0xFF) == 0x02) goto L_00F366;  /* 00F360 BEQ */
    /* 00F362 CMP */
    if ((A & 0xFF) != 0x01) goto L_00F373;  /* 00F364 BNE */
L_00F366: TICK();
    mflag = 0;  /* 00F366 CLM */
    S -= 3; F_00EE8B();  /* 00F367 JSRL */
    mflag = 1;  /* 00F36B SEM */
    wr8(ABS(0x0228), 0x01);  /* 00F36C LDM */
    goto L_00F3A6;  /* 00F370 JMP */
L_00F373: TICK();
    mflag = 1;  /* 00F373 SEM */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F374 LDA */
    if ((A & 0xFF) != 0) goto L_00F386;  /* 00F377 BNE */
    mflag = 0;  /* 00F379 CLM */
    S -= 3; F_00EE8B();  /* 00F37A JSRL */
    mflag = 1;  /* 00F37E SEM */
    wr8(ABS(0x0228), 0x00);  /* 00F37F LDM */
    goto L_00F3A6;  /* 00F383 JMP */
L_00F386: TICK();
    mflag = 0;  /* 00F386 CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0) goto L_00F39C;  /* 00F387 BBC */
    A = rd16(ABS(0x0229));  /* 00F38D LDA */
    mflag = 1;  /* 00F390 SEM */
    /* 00F391 CMP */
    if ((A & 0xFF) != 0x04) goto L_00F3A6;  /* 00F393 BNE */
    SETLO(A, rd8(ABS(0x0228)));  /* 00F395 LDA */
    /* 00F398 CMP */
    if ((A & 0xFF) != 0x0C) goto L_00F3A6;  /* 00F39A BNE */
L_00F39C: TICK();
    mflag = 0;  /* 00F39C CLM */
    S -= 3; F_00EE8B();  /* 00F39D JSRL */
    mflag = 1;  /* 00F3A1 SEM */
    wr8(ABS(0x0228), 0x04);  /* 00F3A2 LDM */
L_00F3A6: TICK();
    goto L_00F3E5;  /* 00F3A6 JMP */
L_00F3A9: TICK();
    mflag = 0;  /* 00F3A9 CLM */
    A = rd16(ABS(0x0228));  /* 00F3AA LDA */
    mflag = 1;  /* 00F3AD SEM */
    /* 00F3AE CMP */
    if ((A & 0xFF) != 0x08) goto L_00F3E5;  /* 00F3B0 BNE */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F3B2 LDA */
    if ((A & 0xFF) != 0) goto L_00F3C4;  /* 00F3B5 BNE */
    mflag = 0;  /* 00F3B7 CLM */
    S -= 3; F_00EE8B();  /* 00F3B8 JSRL */
    mflag = 1;  /* 00F3BC SEM */
    wr8(ABS(0x0228), 0x00);  /* 00F3BD LDM */
    goto L_00F3E5;  /* 00F3C1 JMP */
L_00F3C4: TICK();
    mflag = 0;  /* 00F3C4 CLM */
    if ((rd16(ABS(0x023E)) & 0x0004) == 0x0004) goto L_00F3E5;  /* 00F3C5 BBS */
    S -= 3; F_00EE8B();  /* 00F3CB JSRL */
    A = rd16(ABS(0x0229));  /* 00F3CF LDA */
    mflag = 1;  /* 00F3D2 SEM */
    /* 00F3D3 CMP */
    if ((A & 0xFF) == 0x04) goto L_00F3DB;  /* 00F3D5 BEQ */
    /* 00F3D7 CMP */
    if ((A & 0xFF) != 0x03) goto L_00F3E0;  /* 00F3D9 BNE */
L_00F3DB: TICK();
    SETLO(A, 0x04);  /* 00F3DB LDA */
    goto L_00F3E2;  /* 00F3DD JMP */
L_00F3E0: TICK();
    SETLO(A, 0x01);  /* 00F3E0 LDA */
L_00F3E2: TICK();
    wr8(ABS(0x0228), (uint8_t)A);  /* 00F3E2 STA */
L_00F3E5: TICK();
    mflag = 0;  /* 00F3E5 CLM */
    Y = pull16();  /* 00F3E6 PLY */
    S += 3; return;  /* 00F3E7 RTL */
}

/* 00F3E8  callers: 016FD1, 01A796, 01A7B4 */
void radio_state_machine_00F3E8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00F3E8 LDA */
    push16(D);  /* 00F3EA PHD */
    D = A;  /* 00F3EB TAD */
    A += 1;  /* 00F3EC INC */
    push16(A);  /* 00F3ED PHA */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00F421;  /* 00F3EE BBC */
    mflag = 1;  /* 00F3F4 SEM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_00F402;  /* 00F3F5 BBC */
    mflag = 0;  /* 00F3FA CLM */
    S -= 3; F_00EEE9();  /* 00F3FB JSRL */
    goto L_00F421;  /* 00F3FF JMP */
L_00F402: TICK();
    mflag = 0;  /* 00F402 CLM */
    A = rd16(ABS(0x0344));  /* 00F403 LDA */
    mflag = 1;  /* 00F406 SEM */
    SETLO(A, A & 0x0F);  /* 00F407 AND */
    mflag = 0;  /* 00F409 CLM */
    A &= 0x00FF;  /* 00F40A AND */
    mpy16(0x01F4);  /* 00F40D MPY */
    A = lsr16(A);  /* 00F411 LSR */
    A = lsr16(A);  /* 00F412 LSR */
    A = lsr16(A);  /* 00F413 LSR */
    A = lsr16(A);  /* 00F414 LSR */
    A = lsr16(A);  /* 00F415 LSR */
    mflag = 1;  /* 00F416 SEM */
    SETLO(A, A + 1);  /* 00F417 INC */
    wr8(ABS(0x0207), (uint8_t)A);  /* 00F418 STA */
    SETLO(A, rd8(ABS(0x034D)));  /* 00F41B LDA */
    wr8(ABS(0x0209), (uint8_t)A);  /* 00F41E STA */
L_00F421: TICK();
    mflag = 0;  /* 00F421 CLM */
    S -= 3; F_00F2A1();  /* 00F422 JSRL */
    A = rd16(ABS(0x0228));  /* 00F426 LDA */
    mflag = 1;  /* 00F429 SEM */
    /* 00F42A CMP */
    if ((A & 0xFF) != 0x01) goto L_00F499;  /* 00F42C BNE */
    mflag = 0;  /* 00F42E CLM */
    S -= 3; F_00EA11();  /* 00F42F JSRL */
    A = rd16(ABS(0x0347));  /* 00F433 LDA */
    mflag = 1;  /* 00F436 SEM */
    wr8(ABS(0x022C), (uint8_t)A);  /* 00F437 STA */
    mflag = 0;  /* 00F43A CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 00F43B CLB */
    mflag = 1;  /* 00F440 SEM */
    if ((rd8(ABS(0x023E)) & 0x08) == 0) goto L_00F499;  /* 00F441 BBC */
    mflag = 0;  /* 00F446 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0004);  /* 00F447 SEB */
    mflag = 1;  /* 00F44C SEM */
    wr8(ABS(0x0228), 0x02);  /* 00F44D LDM */
    if ((rd8(ABS(P5)) & 0x80) == 0) goto L_00F46B;  /* 00F451 BBC [P57 SUSTURMA] */
    SETLO(A, rd8(ABS(0x0347)));  /* 00F456 LDA */
    mflag = 0;  /* 00F459 CLM */
    A &= 0x00FF;  /* 00F45A AND */
    C = 0;  /* 00F45D CLC */
    A = adc16(A, 0x0010);  /* 00F45E ADC */
    A = lsr16(A);  /* 00F461 LSR */
    A = lsr16(A);  /* 00F462 LSR */
    A = lsr16(A);  /* 00F463 LSR */
    A = lsr16(A);  /* 00F464 LSR */
    A = lsr16(A);  /* 00F465 LSR */
    mflag = 1;  /* 00F466 SEM */
    SETLO(A, A + 1);  /* 00F467 INC */
    wr8(ABS(0x022A), (uint8_t)A);  /* 00F468 STA */
L_00F46B: TICK();
    mflag = 0;  /* 00F46B CLM */
    S -= 3; F_007D0E();  /* 00F46C JSRL */
    if ((rd16(ABS(0x0244)) & 0x0800) == 0) goto L_00F499;  /* 00F470 BBC */
    if ((rd16(ABS(0x0244)) & 0x1000) == 0) goto L_00F481;  /* 00F476 BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0020);  /* 00F47C SEB */
L_00F481: TICK();
    if ((rd16(ABS(0x0244)) & 0x2000) == 0) goto L_00F48F;  /* 00F481 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0001);  /* 00F487 SEB */
    goto L_00F495;  /* 00F48C JMP */
L_00F48F: TICK();
    mflag = 0;  /* 00F48F CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0001);  /* 00F490 CLB */
L_00F495: TICK();
    S -= 3; F_00CB52();  /* 00F495 JSRL */
L_00F499: TICK();
    mflag = 0;  /* 00F499 CLM */
    A = rd16(ABS(0x0228));  /* 00F49A LDA */
    mflag = 1;  /* 00F49D SEM */
    /* 00F49E CMP */
    if ((A & 0xFF) != 0x02) goto L_00F4A7;  /* 00F4A0 BNE */
    mflag = 0;  /* 00F4A2 CLM */
    S -= 3; F_00EB70();  /* 00F4A3 JSRL */
L_00F4A7: TICK();
    mflag = 0;  /* 00F4A7 CLM */
    A = rd16(ABS(0x0228));  /* 00F4A8 LDA */
    mflag = 1;  /* 00F4AB SEM */
    /* 00F4AC CMP */
    if ((A & 0xFF) != 0x03) goto L_00F4B5;  /* 00F4AE BNE */
    mflag = 0;  /* 00F4B0 CLM */
    S -= 3; F_00EC7E();  /* 00F4B1 JSRL */
L_00F4B5: TICK();
    mflag = 0;  /* 00F4B5 CLM */
    A = rd16(ABS(0x0228));  /* 00F4B6 LDA */
    mflag = 1;  /* 00F4B9 SEM */
    /* 00F4BA CMP */
    if ((A & 0xFF) != 0x04) goto L_00F4CD;  /* 00F4BC BNE */
    mflag = 0;  /* 00F4BE CLM */
    S -= 3; F_00ED0F();  /* 00F4BF JSRL */
    if ((rd16(ABS(0x023E)) & 0x0008) == 0) goto L_00F4CD;  /* 00F4C3 BBC */
    S -= 3; F_00ED53();  /* 00F4C9 JSRL */
L_00F4CD: TICK();
    mflag = 0;  /* 00F4CD CLM */
    A = rd16(ABS(0x0228));  /* 00F4CE LDA */
    mflag = 1;  /* 00F4D1 SEM */
    /* 00F4D2 CMP */
    if ((A & 0xFF) != 0x05) goto L_00F4E8;  /* 00F4D4 BNE */
    SETLO(A, rd8(ABS(0x022A)));  /* 00F4D6 LDA */
    if ((A & 0xFF) != 0) goto L_00F4E8;  /* 00F4D9 BNE */
    wr8(ABS(0x0228), 0x06);  /* 00F4DB LDM */
    wr8(ABS(0x0189), 0x01);  /* 00F4DF LDM */
    mflag = 0;  /* 00F4E3 CLM */
    S -= 3; selcall_send_00E65B();  /* 00F4E4 JSRL */
L_00F4E8: TICK();
    mflag = 0;  /* 00F4E8 CLM */
    A = rd16(ABS(0x0228));  /* 00F4E9 LDA */
    mflag = 1;  /* 00F4EC SEM */
    /* 00F4ED CMP */
    if ((A & 0xFF) == 0x06) goto L_00F4F4;  /* 00F4EF BEQ */
    goto L_00F668;  /* 00F4F1 JMP */
L_00F4F4: TICK();
    mflag = 0;  /* 00F4F4 CLM */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0x0001) goto L_00F4FE;  /* 00F4F5 BBS */
    goto L_00F585;  /* 00F4FB JMP */
L_00F4FE: TICK();
    mflag = 1;  /* 00F4FE SEM */
    SETLO(B, 0x00);  /* 00F4FF LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00F502 LDA */
    div8(0x10);  /* 00F505 DIV */
    wr8(DP(0x00), (uint8_t)A);  /* 00F508 STA */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00F56F;  /* 00F50A BBC */
    /* 00F50F CMP */
    if ((A & 0xFF) == 0x01) goto L_00F51B;  /* 00F511 BEQ */
    /* 00F513 CMP */
    if ((A & 0xFF) == 0x04) goto L_00F51B;  /* 00F515 BEQ */
    /* 00F517 CMP */
    if ((A & 0xFF) != 0x05) goto L_00F56F;  /* 00F519 BNE */
L_00F51B: TICK();
    wr8(ABS(0x0228), 0x07);  /* 00F51B LDM */
    SETLO(A, rd8(ABS(0x0340)));  /* 00F51F LDA */
    SETLO(A, A & 0x03);  /* 00F522 AND */
    SETLO(A, A - 1);  /* 00F524 DEC */
    if ((A & 0xFF) != 0) goto L_00F52C;  /* 00F525 BNE */
    SETLO(A, 0x64);  /* 00F527 LDA */
    goto L_00F53C;  /* 00F529 JMP */
L_00F52C: TICK();
    SETLO(A, rd8(ABS(0x0340)));  /* 00F52C LDA */
    SETLO(A, A & 0x03);  /* 00F52F AND */
    /* 00F531 CMP */
    if ((A & 0xFF) != 0x02) goto L_00F53A;  /* 00F533 BNE */
    SETLO(A, 0x46);  /* 00F535 LDA */
    goto L_00F53C;  /* 00F537 JMP */
L_00F53A: TICK();
    SETLO(A, 0x28);  /* 00F53A LDA */
L_00F53C: TICK();
    mflag = 0;  /* 00F53C CLM */
    A &= 0x00FF;  /* 00F53D AND */
    A ^= 0xFFFF;  /* 00F540 EOR */
    A += 1;  /* 00F543 INC */
    C = 0;  /* 00F544 CLC */
    A = adc16(A, 0x012C);  /* 00F545 ADC */
    B = 0x0020;  /* 00F548 LDA */
    S -= 3; F_00FF8C();  /* 00F54C JSRL */
    mflag = 1;  /* 00F550 SEM */
    wr8(ABS(0x022A), (uint8_t)A);  /* 00F551 STA */
    if ((rd8(ABS(0x0340)) & 0x08) == 0) goto L_00F564;  /* 00F554 BBC */
    Y = 0x0001;  /* 00F559 LDY */
    mflag = 0;  /* 00F55C CLM */
    S -= 3; F_007809();  /* 00F55D JSRL */
    goto L_00F56C;  /* 00F561 JMP */
L_00F564: TICK();
    Y = 0x0002;  /* 00F564 LDY */
    mflag = 0;  /* 00F567 CLM */
    S -= 3; F_007809();  /* 00F568 JSRL */
L_00F56C: TICK();
    goto L_00F582;  /* 00F56C JMP */
L_00F56F: TICK();
    mflag = 0;  /* 00F56F CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0001);  /* 00F570 CLB */
    mflag = 1;  /* 00F575 SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00F576 LDM */
    Y = 0x0002;  /* 00F57A LDY */
    mflag = 0;  /* 00F57D CLM */
    S -= 3; F_007809();  /* 00F57E JSRL */
L_00F582: TICK();
    goto L_00F65B;  /* 00F582 JMP */
L_00F585: TICK();
    mflag = 0;  /* 00F585 CLM */
    if ((rd16(ABS(0x023A)) & 0x0008) == 0) goto L_00F5D8;  /* 00F586 BBC */
    S -= 3; F_01E0E2();  /* 00F58C JSRL */
    X = A;  /* 00F590 TAX */
    if (X != 0) goto L_00F5D5;  /* 00F591 BNE */
    mflag = 1;  /* 00F593 SEM */
    wr8(ABS(0x0228), 0x07);  /* 00F594 LDM */
    SETLO(A, rd8(ABS(0x0340)));  /* 00F598 LDA */
    SETLO(A, A & 0x03);  /* 00F59B AND */
    SETLO(A, A - 1);  /* 00F59D DEC */
    if ((A & 0xFF) != 0) goto L_00F5A5;  /* 00F59E BNE */
    SETLO(A, 0x64);  /* 00F5A0 LDA */
    goto L_00F5B5;  /* 00F5A2 JMP */
L_00F5A5: TICK();
    SETLO(A, rd8(ABS(0x0340)));  /* 00F5A5 LDA */
    SETLO(A, A & 0x03);  /* 00F5A8 AND */
    /* 00F5AA CMP */
    if ((A & 0xFF) != 0x02) goto L_00F5B3;  /* 00F5AC BNE */
    SETLO(A, 0x46);  /* 00F5AE LDA */
    goto L_00F5B5;  /* 00F5B0 JMP */
L_00F5B3: TICK();
    SETLO(A, 0x28);  /* 00F5B3 LDA */
L_00F5B5: TICK();
    mflag = 0;  /* 00F5B5 CLM */
    A &= 0x00FF;  /* 00F5B6 AND */
    A ^= 0xFFFF;  /* 00F5B9 EOR */
    A += 1;  /* 00F5BC INC */
    C = 0;  /* 00F5BD CLC */
    A = adc16(A, 0x012C);  /* 00F5BE ADC */
    B = 0x0020;  /* 00F5C1 LDA */
    S -= 3; F_00FF8C();  /* 00F5C5 JSRL */
    mflag = 1;  /* 00F5C9 SEM */
    wr8(ABS(0x022A), (uint8_t)A);  /* 00F5CA STA */
    Y = 0x0001;  /* 00F5CD LDY */
    mflag = 0;  /* 00F5D0 CLM */
    S -= 3; F_007809();  /* 00F5D1 JSRL */
L_00F5D5: TICK();
    goto L_00F65B;  /* 00F5D5 JMP */
L_00F5D8: TICK();
    mflag = 0;  /* 00F5D8 CLM */
    if ((rd16(ABS(0x0244)) & 0x0002) == 0) goto L_00F5FB;  /* 00F5D9 BBC */
    Y = 0x000E;  /* 00F5DF LDY */
    S -= 3; F_0080AA();  /* 00F5E2 JSRL */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0002);  /* 00F5E6 CLB */
    mflag = 1;  /* 00F5EB SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00F5EC LDM */
    Y = 0x0003;  /* 00F5F0 LDY */
    mflag = 0;  /* 00F5F3 CLM */
    S -= 3; F_007809();  /* 00F5F4 JSRL */
    goto L_00F65B;  /* 00F5F8 JMP */
L_00F5FB: TICK();
    mflag = 0;  /* 00F5FB CLM */
    A = rd16(ABS(0x0229));  /* 00F5FC LDA */
    mflag = 1;  /* 00F5FF SEM */
    /* 00F600 CMP */
    if ((A & 0xFF) == 0x02) goto L_00F60B;  /* 00F602 BEQ */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F604 LDA */
    /* 00F607 CMP */
    if ((A & 0xFF) != 0x01) goto L_00F634;  /* 00F609 BNE */
L_00F60B: TICK();
    SETLO(B, 0x00);  /* 00F60B LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00F60E LDA */
    div8(0x10);  /* 00F611 DIV */
    wr8(DP(0x00), (uint8_t)A);  /* 00F614 STA */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00F62C;  /* 00F616 BBC */
    /* 00F61B CMP */
    if ((A & 0xFF) == 0x03) goto L_00F623;  /* 00F61D BEQ */
    /* 00F61F CMP */
    if ((A & 0xFF) != 0x05) goto L_00F62C;  /* 00F621 BNE */
L_00F623: TICK();
    wr8(ABS(0x0189), 0x01);  /* 00F623 LDM */
    mflag = 0;  /* 00F627 CLM */
    S -= 3; selcall_send_00E65B();  /* 00F628 JSRL */
L_00F62C: TICK();
    mflag = 1;  /* 00F62C SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00F62D LDM */
    goto L_00F653;  /* 00F631 JMP */
L_00F634: TICK();
    mflag = 0;  /* 00F634 CLM */
    A = rd16(ABS(0x0229));  /* 00F635 LDA */
    mflag = 1;  /* 00F638 SEM */
    /* 00F639 CMP */
    if ((A & 0xFF) == 0x04) goto L_00F644;  /* 00F63B BEQ */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F63D LDA */
    /* 00F640 CMP */
    if ((A & 0xFF) != 0x03) goto L_00F653;  /* 00F642 BNE */
L_00F644: TICK();
    mflag = 0;  /* 00F644 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0800);  /* 00F645 SEB */
    S -= 3; F_00CB52();  /* 00F64A JSRL */
    mflag = 1;  /* 00F64E SEM */
    wr8(ABS(0x0228), 0x08);  /* 00F64F LDM */
L_00F653: TICK();
    Y = 0x0001;  /* 00F653 LDY */
    mflag = 0;  /* 00F656 CLM */
    S -= 3; F_007809();  /* 00F657 JSRL */
L_00F65B: TICK();
    if ((rd16(ABS(0x0244)) & 0x0001) == 0x0001) goto L_00F668;  /* 00F65B BBS */
    Y = 0x0000;  /* 00F661 LDY */
    S -= 3; F_007809();  /* 00F664 JSRL */
L_00F668: TICK();
    mflag = 0;  /* 00F668 CLM */
    A = rd16(ABS(0x0228));  /* 00F669 LDA */
    mflag = 1;  /* 00F66C SEM */
    /* 00F66D CMP */
    if ((A & 0xFF) == 0x07) goto L_00F674;  /* 00F66F BEQ */
    goto L_00F6F5;  /* 00F671 JMP */
L_00F674: TICK();
    SETLO(A, rd8(ABS(0x022A)));  /* 00F674 LDA */
    if ((A & 0xFF) != 0) goto L_00F6F5;  /* 00F677 BNE */
    mflag = 0;  /* 00F679 CLM */
    S -= 3; selcall_send_00E65B();  /* 00F67A JSRL */
    if ((rd16(ABS(0x0244)) & 0x0001) == 0) goto L_00F6B0;  /* 00F67E BBC */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0001);  /* 00F684 CLB */
    mflag = 1;  /* 00F689 SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00F68A LDM */
    if ((rd8(ABS(0x0340)) & 0x08) == 0) goto L_00F69E;  /* 00F68E BBC */
    Y = 0x0002;  /* 00F693 LDY */
    mflag = 0;  /* 00F696 CLM */
    S -= 3; F_007809();  /* 00F697 JSRL */
    goto L_00F6A6;  /* 00F69B JMP */
L_00F69E: TICK();
    Y = 0x0001;  /* 00F69E LDY */
    mflag = 0;  /* 00F6A1 CLM */
    S -= 3; F_007809();  /* 00F6A2 JSRL */
L_00F6A6: TICK();
    Y = 0x0000;  /* 00F6A6 LDY */
    S -= 3; F_007809();  /* 00F6A9 JSRL */
    goto L_00F6F5;  /* 00F6AD JMP */
L_00F6B0: TICK();
    mflag = 0;  /* 00F6B0 CLM */
    if ((rd16(ABS(0x023A)) & 0x0008) == 0) goto L_00F6F0;  /* 00F6B1 BBC */
    A = rd16(ABS(0x0340));  /* 00F6B7 LDA */
    mflag = 1;  /* 00F6BA SEM */
    SETLO(A, A & 0x03);  /* 00F6BB AND */
    SETLO(A, A - 1);  /* 00F6BD DEC */
    if ((A & 0xFF) != 0) goto L_00F6C5;  /* 00F6BE BNE */
    SETLO(A, 0x64);  /* 00F6C0 LDA */
    goto L_00F6D5;  /* 00F6C2 JMP */
L_00F6C5: TICK();
    SETLO(A, rd8(ABS(0x0340)));  /* 00F6C5 LDA */
    SETLO(A, A & 0x03);  /* 00F6C8 AND */
    /* 00F6CA CMP */
    if ((A & 0xFF) != 0x02) goto L_00F6D3;  /* 00F6CC BNE */
    SETLO(A, 0x46);  /* 00F6CE LDA */
    goto L_00F6D5;  /* 00F6D0 JMP */
L_00F6D3: TICK();
    SETLO(A, 0x28);  /* 00F6D3 LDA */
L_00F6D5: TICK();
    mflag = 0;  /* 00F6D5 CLM */
    A &= 0x00FF;  /* 00F6D6 AND */
    A ^= 0xFFFF;  /* 00F6D9 EOR */
    A += 1;  /* 00F6DC INC */
    C = 0;  /* 00F6DD CLC */
    A = adc16(A, 0x012C);  /* 00F6DE ADC */
    B = 0x0020;  /* 00F6E1 LDA */
    S -= 3; F_00FF8C();  /* 00F6E5 JSRL */
    mflag = 1;  /* 00F6E9 SEM */
    wr8(ABS(0x022A), (uint8_t)A);  /* 00F6EA STA */
    goto L_00F6F5;  /* 00F6ED JMP */
L_00F6F0: TICK();
    mflag = 1;  /* 00F6F0 SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00F6F1 LDM */
L_00F6F5: TICK();
    mflag = 0;  /* 00F6F5 CLM */
    A = rd16(ABS(0x0228));  /* 00F6F6 LDA */
    mflag = 1;  /* 00F6F9 SEM */
    /* 00F6FA CMP */
    if ((A & 0xFF) == 0x08) goto L_00F701;  /* 00F6FC BEQ */
    goto L_00F783;  /* 00F6FE JMP */
L_00F701: TICK();
    mflag = 0;  /* 00F701 CLM */
    if ((rd16(ABS(0x023E)) & 0x1000) == 0) goto L_00F70E;  /* 00F702 BBC */
    A = 0x0001;  /* 00F708 LDA */
    goto L_00F711;  /* 00F70B JMP */
L_00F70E: TICK();
    A = 0x0000;  /* 00F70E LDA */
L_00F711: TICK();
    mflag = 1;  /* 00F711 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00F712 STA */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F714 LDA */
    /* 00F717 CMP */
    if ((A & 0xFF) != 0x03) goto L_00F724;  /* 00F719 BNE */
    mflag = 0;  /* 00F71B CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x1000);  /* 00F71C SEB */
    goto L_00F733;  /* 00F721 JMP */
L_00F724: TICK();
    mflag = 0;  /* 00F724 CLM */
    A = rd16(ABS(0x0229));  /* 00F725 LDA */
    mflag = 1;  /* 00F728 SEM */
    /* 00F729 CMP */
    if ((A & 0xFF) != 0x04) goto L_00F733;  /* 00F72B BNE */
    mflag = 0;  /* 00F72D CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x1000);  /* 00F72E CLB */
L_00F733: TICK();
    mflag = 0;  /* 00F733 CLM */
    A = rd16(DP(0x00));  /* 00F734 LDA */
    mflag = 1;  /* 00F736 SEM */
    /* 00F737 CMP */
    if ((A & 0xFF) != 0x01) goto L_00F742;  /* 00F739 BNE */
    mflag = 0;  /* 00F73B CLM */
    if ((rd16(ABS(0x023E)) & 0x1000) == 0) goto L_00F74E;  /* 00F73C BBC */
L_00F742: TICK();
    mflag = 1;  /* 00F742 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 00F743 LDA */
    if ((A & 0xFF) != 0) goto L_00F752;  /* 00F745 BNE */
    mflag = 0;  /* 00F747 CLM */
    if ((rd16(ABS(0x023E)) & 0x1000) == 0) goto L_00F752;  /* 00F748 BBC */
L_00F74E: TICK();
    S -= 3; F_00CB52();  /* 00F74E JSRL */
L_00F752: TICK();
    mflag = 0;  /* 00F752 CLM */
    S -= 3; F_00EE14();  /* 00F753 JSRL */
    if ((rd16(ABS(0x0244)) & 0x0008) == 0x0008) goto L_00F76E;  /* 00F757 BBS */
    mflag = 1;  /* 00F75D SEM */
    if ((rd8(ABS(0x037C)) & 0x40) == 0) goto L_00F76E;  /* 00F75E BBC */
    mflag = 0;  /* 00F763 CLM */
    X = rd16(ABS(0x0218));  /* 00F764 LDX */
    if (X != 0) goto L_00F76E;  /* 00F767 BNE */
    mflag = 1;  /* 00F769 SEM */
    wr8(ABS(0x0228), 0x0B);  /* 00F76A LDM */
L_00F76E: TICK();
    mflag = 0;  /* 00F76E CLM */
    A = rd16(ABS(0x0229));  /* 00F76F LDA */
    mflag = 1;  /* 00F772 SEM */
    /* 00F773 CMP */
    if ((A & 0xFF) == 0x02) goto L_00F77E;  /* 00F775 BEQ */
    SETLO(A, rd8(ABS(0x0229)));  /* 00F777 LDA */
    /* 00F77A CMP */
    if ((A & 0xFF) != 0x01) goto L_00F783;  /* 00F77C BNE */
L_00F77E: TICK();
    mflag = 0;  /* 00F77E CLM */
    S -= 3; F_00EE4D();  /* 00F77F JSRL */
L_00F783: TICK();
    mflag = 0;  /* 00F783 CLM */
    A = rd16(ABS(0x0228));  /* 00F784 LDA */
    mflag = 1;  /* 00F787 SEM */
    /* 00F788 CMP */
    if ((A & 0xFF) != 0x0B) goto L_00F7A3;  /* 00F78A BNE */
    Y = 0x0009;  /* 00F78C LDY */
    mflag = 0;  /* 00F78F CLM */
    S -= 3; F_0080AA();  /* 00F790 JSRL */
    mflag = 1;  /* 00F794 SEM */
    wr8(ABS(0x0229), 0x02);  /* 00F795 LDM */
    mflag = 0;  /* 00F799 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0080);  /* 00F79A SEB */
    S -= 3; F_00EE4D();  /* 00F79F JSRL */
L_00F7A3: TICK();
    mflag = 0;  /* 00F7A3 CLM */
    A = rd16(ABS(0x0228));  /* 00F7A4 LDA */
    mflag = 1;  /* 00F7A7 SEM */
    /* 00F7A8 CMP */
    if ((A & 0xFF) != 0x09) goto L_00F7D5;  /* 00F7AA BNE */
    SETLO(A, rd8(ABS(0x022A)));  /* 00F7AC LDA */
    if ((A & 0xFF) != 0) goto L_00F7D5;  /* 00F7AF BNE */
    SETLO(B, 0x00);  /* 00F7B1 LDA */
    SETLO(A, rd8(ABS(0x0342)));  /* 00F7B4 LDA */
    div8(0x10);  /* 00F7B7 DIV */
    wr8(DP(0x00), (uint8_t)A);  /* 00F7BA STA */
    if ((rd8(ABS(0x0375)) & 0x80) == 0) goto L_00F7D0;  /* 00F7BC BBC */
    /* 00F7C1 CMP */
    if ((A & 0xFF) == 0x03) goto L_00F7C9;  /* 00F7C3 BEQ */
    /* 00F7C5 CMP */
    if ((A & 0xFF) != 0x05) goto L_00F7D0;  /* 00F7C7 BNE */
L_00F7C9: TICK();
    wr8(ABS(0x0228), 0x06);  /* 00F7C9 LDM */
    goto L_00F7D5;  /* 00F7CD JMP */
L_00F7D0: TICK();
    mflag = 1;  /* 00F7D0 SEM */
    wr8(ABS(0x0228), 0x0A);  /* 00F7D1 LDM */
L_00F7D5: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 00F7D5 LDA */
    /* 00F7D8 CMP */
    if ((A & 0xFF) != 0x0A) goto L_00F80D;  /* 00F7DA BNE */
    SETLO(A, rd8(ABS(0x037C)));  /* 00F7DC LDA */
    SETLO(A, A & 0x3F);  /* 00F7DF AND */
    if ((A & 0xFF) == 0) goto L_00F808;  /* 00F7E1 BEQ */
    mflag = 0;  /* 00F7E3 CLM */
    if ((rd16(ABS(0x023A)) & 0x4000) == 0x4000) goto L_00F7FB;  /* 00F7E4 BBS */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x4000);  /* 00F7EA SEB */
    A = rd16(ABS(0x034E));  /* 00F7EF LDA */
    mflag = 1;  /* 00F7F2 SEM */
    SETLO(A, A & 0x07);  /* 00F7F3 AND */
    wr8(ABS(0x022A), (uint8_t)A);  /* 00F7F5 STA */
    goto L_00F805;  /* 00F7F8 JMP */
L_00F7FB: TICK();
    mflag = 1;  /* 00F7FB SEM */
    SETLO(A, rd8(ABS(0x022A)));  /* 00F7FC LDA */
    if ((A & 0xFF) != 0) goto L_00F805;  /* 00F7FF BNE */
    wr8(ABS(0x0228), 0x0C);  /* 00F801 LDM */
L_00F805: TICK();
    goto L_00F80D;  /* 00F805 JMP */
L_00F808: TICK();
    mflag = 1;  /* 00F808 SEM */
    wr8(ABS(0x0228), 0x0C);  /* 00F809 LDM */
L_00F80D: TICK();
    SETLO(A, rd8(ABS(0x0228)));  /* 00F80D LDA */
    /* 00F810 CMP */
    if ((A & 0xFF) != 0x0C) goto L_00F848;  /* 00F812 BNE */
    wr8(ABS(0x01E9), 0x00);  /* 00F814 LDM */
    mflag = 0;  /* 00F818 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x4000);  /* 00F819 CLB */
    if ((rd16(ABS(0x0244)) & 0x0080) == 0) goto L_00F848;  /* 00F81E BBC */
    S -= 3; F_00EE8B();  /* 00F824 JSRL */
    A = rd16(ABS(0x0345));  /* 00F828 LDA */
    mflag = 1;  /* 00F82B SEM */
    SETLO(A, A & 0x0F);  /* 00F82C AND */
    mflag = 0;  /* 00F82E CLM */
    A &= 0x00FF;  /* 00F82F AND */
    mpy16(0x03E8);  /* 00F832 MPY */
    A = lsr16(A);  /* 00F836 LSR */
    A = lsr16(A);  /* 00F837 LSR */
    A = lsr16(A);  /* 00F838 LSR */
    A = lsr16(A);  /* 00F839 LSR */
    A = lsr16(A);  /* 00F83A LSR */
    A += 1;  /* 00F83B INC */
    wr16(ABS(0x0222), A);  /* 00F83C STA */
    mflag = 1;  /* 00F83F SEM */
    wr8(ABS(0x0229), 0x02);  /* 00F840 LDM */
    wr8(ABS(0x0228), 0x01);  /* 00F844 LDM */
L_00F848: TICK();
    mflag = 0;  /* 00F848 CLM */
    S -= 3; F_00EAA8();  /* 00F849 JSRL */
    S -= 3; F_00E626();  /* 00F84D JSRL */
    Y = pull16();  /* 00F851 PLY */
    D = pull16();  /* 00F852 PLD */
    S += 3; return;  /* 00F853 RTL */
}

/* 00F854  callers: 00EDF8 */
void ctcss_tx_start_00F854(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x037C));  /* 00F854 LDA */
    mflag = 1;  /* 00F857 SEM */
    SETLO(A, A & 0x3F);  /* 00F858 AND */
    if ((A & 0xFF) == 0) goto L_00F8AC;  /* 00F85A BEQ */
    SETLO(A, rd8(ABS(0x037C)));  /* 00F85C LDA */
    SETLO(A, A & 0x3F);  /* 00F85F AND */
    mflag = 0;  /* 00F861 CLM */
    A &= 0x00FF;  /* 00F862 AND */
    A -= 1;  /* 00F865 DEC */
    X = A;  /* 00F866 TAX */
    A = rd16((0x002353 + X));  /* 00F867 LDA */
    mflag = 1;  /* 00F86B SEM */
    wr8(ABS(0x01AD), (uint8_t)A);  /* 00F86C STA */
    wr8(ABS(TA2IC), 0x00);  /* 00F86F LDM */
    wr8(ABS(TA2MOD), 0x27);  /* 00F873 LDM */
    wr8(ABS(TA2_H), 0x00);  /* 00F877 LDM */
    wr8(ABS(TA2), 0x00);  /* 00F87B LDM */
    wr8(ABS(TB0IC), 0x05);  /* 00F87F LDM */
    wr8(ABS(TB0MOD), 0x00);  /* 00F883 LDM */
    SETLO(A, rd8(ABS(0x037C)));  /* 00F887 LDA */
    SETLO(A, A & 0x3F);  /* 00F88A AND */
    mflag = 0;  /* 00F88C CLM */
    A &= 0x00FF;  /* 00F88D AND */
    A = asl16(A);  /* 00F890 ASL */
    A -= 1;  /* 00F891 DEC */
    A -= 1;  /* 00F892 DEC */
    X = A;  /* 00F893 TAX */
    A = rd16((0x0022EF + X));  /* 00F894 LDA */
    wr16(ABS(TB0), A);  /* 00F898 STA */
    mflag = 1;  /* 00F89B SEM */
    wr8(ABS(TA2IC), rd8(ABS(TA2IC)) & ~0x08);  /* 00F89C CLB */
    wr8(ABS(TB0IC), rd8(ABS(TB0IC)) & ~0x08);  /* 00F8A0 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x04);  /* 00F8A4 SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x20);  /* 00F8A8 SEB */
L_00F8AC: TICK();
    mflag = 0;  /* 00F8AC CLM */
    S += 3; return;  /* 00F8AD RTL */
}

/* 00F8AE  callers: 00EEB6 */
void ctcss_tx_stop_00F8AE(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00F8AE SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x20);  /* 00F8AF CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x04);  /* 00F8B3 CLB */
    wr8(ABS(TB0IC), 0x00);  /* 00F8B7 LDM */
    mflag = 0;  /* 00F8BB CLM */
    S += 3; return;  /* 00F8BC RTL */
}

/* 00F8BD  callers: 00EBE8 */
void ctcss_rx_start_00F8BD(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x0375));  /* 00F8BD LDA */
    mflag = 1;  /* 00F8C0 SEM */
    SETLO(A, A & 0x3F);  /* 00F8C1 AND */
    if ((A & 0xFF) == 0) goto L_00F932;  /* 00F8C3 BEQ */
    mflag = 0;  /* 00F8C5 CLM */
    if ((rd16(ABS(0x0244)) & 0x0800) == 0) goto L_00F8DA;  /* 00F8C6 BBC */
    if ((rd16(ABS(0x0244)) & 0x4000) == 0) goto L_00F8DA;  /* 00F8CC BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x1000);  /* 00F8D2 SEB */
    goto L_00F8E0;  /* 00F8D7 JMP */
L_00F8DA: TICK();
    mflag = 0;  /* 00F8DA CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x1000);  /* 00F8DB CLB */
L_00F8E0: TICK();
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x2000);  /* 00F8E0 CLB */
    wr16(ABS(0x01B2), 0x0000);  /* 00F8E5 LDM */
    wr16(ABS(0x01B4), 0x0000);  /* 00F8EA LDM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x1000);  /* 00F8EF SEB */
    mflag = 1;  /* 00F8F4 SEM */
    wr8(ABS(0x01AE), 0x28);  /* 00F8F5 LDM */
    wr8(ABS(0x01B1), 0x50);  /* 00F8F9 LDM */
    wr8(ABS(0x01AF), 0x00);  /* 00F8FD LDM */
    wr8(ABS(0x01B0), 0x00);  /* 00F901 LDM */
    wr8(ABS(TB0IC), 0x05);  /* 00F905 LDM */
    wr8(ABS(TB0MOD), 0x4A);  /* 00F909 LDM */
    wr8(ABS(TA4IC), 0x06);  /* 00F90D LDM */
    wr8(ABS(TA4MOD), 0x00);  /* 00F911 LDM */
    SETLO(A, rd8(ABS(0x0375)));  /* 00F915 LDA */
    SETLO(A, A & 0x3F);  /* 00F918 AND */
    mflag = 0;  /* 00F91A CLM */
    A &= 0x00FF;  /* 00F91B AND */
    A = asl16(A);  /* 00F91E ASL */
    A -= 1;  /* 00F91F DEC */
    A -= 1;  /* 00F920 DEC */
    X = A;  /* 00F921 TAX */
    A = rd16((0x002385 + X));  /* 00F922 LDA */
    wr16(ABS(TA4), A);  /* 00F926 STA */
    mflag = 1;  /* 00F929 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x10);  /* 00F92A SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x20);  /* 00F92E SEB */
L_00F932: TICK();
    mflag = 0;  /* 00F932 CLM */
    S += 3; return;  /* 00F933 RTL */
}

/* 00F934  callers: 00EA37, 00EA84, 00ECA0 */
void F_00F934(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00F934 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x20);  /* 00F935 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x10);  /* 00F939 CLB */
    wr8(ABS(TA4IC), 0x00);  /* 00F93D LDM */
    wr8(ABS(TB0IC), 0x00);  /* 00F941 LDM */
    mflag = 0;  /* 00F945 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x1000);  /* 00F946 CLB */
    S += 3; return;  /* 00F94B RTL */
}

/* 00FB79  callers: 005B36 */
void F_00FB79(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00FB79 LDA */
    push16(D);  /* 00FB7B PHD */
    D = A;  /* 00FB7C TAD */
    A += 1;  /* 00FB7D INC */
    push16(A);  /* 00FB7E PHA */
    mflag = 1;  /* 00FB7F SEM */
    if ((rd8(ABS(0x033F)) & 0x80) == 0) goto L_00FBFB;  /* 00FB80 BBC */
    mflag = 0;  /* 00FB85 CLM */
    if ((rd16(ABS(0x0240)) & 0x0002) == 0) goto L_00FBF8;  /* 00FB86 BBC */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0002);  /* 00FB8C CLB */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_00FBA3;  /* 00FB91 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x2000);  /* 00FB97 CLB */
    S -= 3; F_00CB52();  /* 00FB9C JSRL */
    goto L_00FBF8;  /* 00FBA0 JMP */
L_00FBA3: TICK();
    mflag = 0;  /* 00FBA3 CLM */
    A = rd16(ABS(0x0365));  /* 00FBA4 LDA */
    mflag = 1;  /* 00FBA7 SEM */
    SETLO(A, A + 1);  /* 00FBA8 INC */
    if ((A & 0xFF) == 0) goto L_00FBF0;  /* 00FBA9 BEQ */
    mflag = 0;  /* 00FBAB CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0200);  /* 00FBAC CLB */
    A = rd16(ABS(0x0375));  /* 00FBB1 LDA */
    mflag = 1;  /* 00FBB4 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 00FBB5 STA */
    mflag = 0;  /* 00FBB7 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00FBC5;  /* 00FBB8 BBC */
    A = rd16(ABS(0x0368));  /* 00FBBE LDA */
    mflag = 1;  /* 00FBC1 SEM */
    wr8(ABS(0x0375), (uint8_t)A);  /* 00FBC2 STA */
L_00FBC5: TICK();
    mflag = 1;  /* 00FBC5 SEM */
    if ((rd8(ABS(0x033F)) & 0x40) == 0) goto L_00FBD2;  /* 00FBC6 BBC */
    SETLO(A, rd8(ABS(0x0375)));  /* 00FBCB LDA */
    SETLO(A, A & 0x3F);  /* 00FBCE AND */
    if ((A & 0xFF) != 0) goto L_00FBE6;  /* 00FBD0 BNE */
L_00FBD2: TICK();
    mflag = 0;  /* 00FBD2 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x2000);  /* 00FBD3 SEB */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00FBE2;  /* 00FBD8 BBC */
    S -= 3; F_0095FA();  /* 00FBDE JSRL */
L_00FBE2: TICK();
    S -= 3; F_00CB52();  /* 00FBE2 JSRL */
L_00FBE6: TICK();
    mflag = 0;  /* 00FBE6 CLM */
    A = rd16(DP(0x00));  /* 00FBE7 LDA */
    mflag = 1;  /* 00FBE9 SEM */
    wr8(ABS(0x0375), (uint8_t)A);  /* 00FBEA STA */
    goto L_00FBF8;  /* 00FBED JMP */
L_00FBF0: TICK();
    Y = 0x0002;  /* 00FBF0 LDY */
    mflag = 0;  /* 00FBF3 CLM */
    S -= 3; F_0080AA();  /* 00FBF4 JSRL */
L_00FBF8: TICK();
    goto L_00FC02;  /* 00FBF8 JMP */
L_00FBFB: TICK();
    mflag = 0;  /* 00FBFB CLM */
    A = 0x0000;  /* 00FBFC LDA */
    Y = pull16();  /* 00FBFF PLY */
    D = pull16();  /* 00FC00 PLD */
    S += 3; return;  /* 00FC01 RTL */
L_00FC02: TICK();
    mflag = 0;  /* 00FC02 CLM */
    A = 0x0001;  /* 00FC03 LDA */
    Y = pull16();  /* 00FC06 PLY */
    D = pull16();  /* 00FC07 PLD */
    S += 3; return;  /* 00FC08 RTL */
}

/* 00FC09  callers: 005B04 */
void F_00FC09(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00FC09 SEM */
    if ((rd8(ABS(0x033F)) & 0x80) == 0) goto L_00FC2C;  /* 00FC0A BBC */
    mflag = 0;  /* 00FC0F CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0040);  /* 00FC10 SEB */
    if ((rd16(ABS(0x023C)) & 0x0010) == 0) goto L_00FC23;  /* 00FC15 BBC */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 00FC1B CLB */
    goto L_00FC29;  /* 00FC20 JMP */
L_00FC23: TICK();
    mflag = 0;  /* 00FC23 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0010);  /* 00FC24 SEB */
L_00FC29: TICK();
    goto L_00FC31;  /* 00FC29 JMP */
L_00FC2C: TICK();
    mflag = 0;  /* 00FC2C CLM */
    A = 0x0000;  /* 00FC2D LDA */
    S += 3; return;  /* 00FC30 RTL */
L_00FC31: TICK();
    A = 0x0001;  /* 00FC31 LDA */
    S += 3; return;  /* 00FC34 RTL */
}

/* 00FC35  callers: 00FCA9, 00FD7D */
void F_00FC35(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00FC35 LDA */
    push16(A);  /* 00FC37 PHA */
    A = rd16(ABS(0x0341));  /* 00FC38 LDA */
    mflag = 1;  /* 00FC3B SEM */
    SETLO(A, A & 0x7F);  /* 00FC3C AND */
    mflag = 0;  /* 00FC3E CLM */
    Y = A;  /* 00FC3F TAY */
    S -= 3; channel_is_programmed_0087A7();  /* 00FC40 JSRL */
    X = A;  /* 00FC44 TAX */
    if (X != 0) goto L_00FC6B;  /* 00FC45 BNE */
    Y = 0x000B;  /* 00FC47 LDY */
    S -= 3; F_003310();  /* 00FC4A JSRL */
    S -= 3; F_00828C();  /* 00FC4E JSRL */
    Y = 0x0002;  /* 00FC52 LDY */
    S -= 3; F_0080AA();  /* 00FC55 JSRL */
    Y = 0x0BB8;  /* 00FC59 LDY */
    S -= 3; F_007D7F();  /* 00FC5C JSRL */
    mflag = 1;  /* 00FC60 SEM */
    wr8(ABS(0x0231), 0x14);  /* 00FC61 LDM */
    mflag = 0;  /* 00FC65 CLM */
    A = 0x0000;  /* 00FC66 LDA */
    Y = pull16();  /* 00FC69 PLY */
    S += 3; return;  /* 00FC6A RTL */
L_00FC6B: TICK();
    A = 0x0001;  /* 00FC6B LDA */
    Y = pull16();  /* 00FC6E PLY */
    S += 3; return;  /* 00FC6F RTL */
}

/* 00FC70  callers: 00669D */
void F_00FC70(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00FC70 LDA */
    push16(D);  /* 00FC72 PHD */
    D = A;  /* 00FC73 TAD */
    C = 0;  /* 00FC74 CLC */
    A = adc16(A, 0x000A);  /* 00FC75 ADC */
    push16(A);  /* 00FC78 PHA */
    mflag = 1;  /* 00FC79 SEM */
    if ((rd8(ABS(0x033F)) & 0x80) == 0x80) goto L_00FC82;  /* 00FC7A BBS */
    goto L_00FD53;  /* 00FC7F JMP */
L_00FC82: TICK();
    wr8(ABS(0x0341), rd8(ABS(0x0341)) | 0x80);  /* 00FC82 SEB */
    SETLO(A, rd8(ABS(0x0341)));  /* 00FC86 LDA */
    SETLO(A, A & 0x7F);  /* 00FC89 AND */
    if ((A & 0xFF) == 0) goto L_00FCA4;  /* 00FC8B BEQ */
    SETLO(A, rd8(ABS(0x0341)));  /* 00FC8D LDA */
    SETLO(A, A & 0x7F);  /* 00FC90 AND */
    wr8(DP(0x0A), (uint8_t)A);  /* 00FC92 STA */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00FC9E;  /* 00FC94 BBC */
    SETLO(A, 0x0A);  /* 00FC99 LDA */
    goto L_00FCA0;  /* 00FC9B JMP */
L_00FC9E: TICK();
    SETLO(A, 0x63);  /* 00FC9E LDA */
L_00FCA0: TICK();
    t = rd8(DP(0x0A));  /* 00FCA0 CMP */
    if ((A & 0xFF) >= t) goto L_00FCA8;  /* 00FCA2 BCS */
L_00FCA4: TICK();
    wr8(ABS(0x0341), rd8(ABS(0x0341)) | 0x10);  /* 00FCA4 SEB */
L_00FCA8: TICK();
    mflag = 0;  /* 00FCA8 CLM */
    S -= 3; F_00FC35();  /* 00FCA9 JSRL */
    X = A;  /* 00FCAD TAX */
    if (X != 0) goto L_00FCB3;  /* 00FCAE BNE */
    goto L_00FD53;  /* 00FCB0 JMP */
L_00FCB3: TICK();
    wr16(DP(0x0A), 0x0008);  /* 00FCB3 LDM */
    A = rd16(ABS(0x0341));  /* 00FCB7 LDA */
    mflag = 1;  /* 00FCBA SEM */
    SETLO(A, A & 0x7F);  /* 00FCBB AND */
    mflag = 0;  /* 00FCBD CLM */
    A &= 0x00FF;  /* 00FCBE AND */
    A -= 1;  /* 00FCC1 DEC */
    wr16(DP(0x0C), A);  /* 00FCC2 STA */
    A = D;  /* 00FCC4 TDA */
    C = 0;  /* 00FCC5 CLC */
    A = adc16(A, 0x0006);  /* 00FCC6 ADC */
    B = 0x0000;  /* 00FCC9 LDA */
    Y = A;  /* 00FCCD TAY */
    S -= 3; F_01E30B();  /* 00FCCE JSRL */
    wr16(DP(0x0A), A);  /* 00FCD2 STA */
    A = D;  /* 00FCD4 TDA */
    A += 1;  /* 00FCD5 INC */
    A += 1;  /* 00FCD6 INC */
    wr16(DP(0x0C), A);  /* 00FCD7 STA */
    mflag = 1;  /* 00FCD9 SEM */
    wr8(DP(0x0E), (uint8_t)B);  /* 00FCDA STA */
    mflag = 0;  /* 00FCDD CLM */
    A = rd16(DP(0x0A));  /* 00FCDE LDA */
    wr16(DP(0x0F), A);  /* 00FCE0 STA */
    A = rd16(DP(0x0E));  /* 00FCE2 LDA */
    mflag = 1;  /* 00FCE4 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 00FCE5 STA */
    mflag = 0;  /* 00FCE7 CLM */
    A = rd16(DP(0x0C));  /* 00FCE8 LDA */
    wr16(DP(0x12), A);  /* 00FCEA STA */
    mflag = 1;  /* 00FCEC SEM */
    wr8(DP(0x14), 0x00);  /* 00FCED LDM */
    mflag = 0;  /* 00FCF0 CLM */
    A = 0x000F;  /* 00FCF1 LDA */
    Y = 0x0004;  /* 00FCF4 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00FCF7 JSRL */
    X = rd16(DP(0x04));  /* 00FCFB LDX */
    A = 0x0001;  /* 00FCFD LDA */
    mflag = 1;  /* 00FD00 SEM */
    S -= 3; rt_shl_01E4E0();  /* 00FD01 JSRL */
    wr8(DP(0x0A), 0x01);  /* 00FD05 LDM */
    wr8(DP(0x01), (uint8_t)A);  /* 00FD08 STA */
    mflag = 0;  /* 00FD0A CLM */
    A = D;  /* 00FD0B TDA */
    B = 0x0000;  /* 00FD0C LDA */
    wr16(DP(0x0B), A);  /* 00FD10 STA */
    wr16(DP(0x0D), B);  /* 00FD12 STA */
    A = rd16(DP(0x02));  /* 00FD15 LDA */
    C = 0;  /* 00FD17 CLC */
    A = adc16(A, 0x0537);  /* 00FD18 ADC */
    wr16(DP(0x0E), A);  /* 00FD1B STA */
    Y = 0x00A0;  /* 00FD1D LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 00FD20 JSRL */
    A = rd16(DP(0x00));  /* 00FD24 LDA */
    mflag = 1;  /* 00FD26 SEM */
    SETLO(A, A | rd8(DP(0x01)));  /* 00FD27 ORA */
    wr8(DP(0x00), (uint8_t)A);  /* 00FD29 STA */
    wr8(DP(0x0A), 0x01);  /* 00FD2B LDM */
    mflag = 0;  /* 00FD2E CLM */
    A = D;  /* 00FD2F TDA */
    B = 0x0000;  /* 00FD30 LDA */
    wr16(DP(0x0B), A);  /* 00FD34 STA */
    wr16(DP(0x0D), B);  /* 00FD36 STA */
    A = rd16(DP(0x02));  /* 00FD39 LDA */
    C = 0;  /* 00FD3B CLC */
    A = adc16(A, 0x0537);  /* 00FD3C ADC */
    wr16(DP(0x0E), A);  /* 00FD3F STA */
    Y = 0x00A0;  /* 00FD41 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00FD44 JSRL */
    X = rd16(DP(0x02));  /* 00FD48 LDX */
    A = rd16((ABS(0x02D2) + X));  /* 00FD4A LDA */
    mflag = 1;  /* 00FD4D SEM */
    SETLO(A, A | rd8(DP(0x01)));  /* 00FD4E ORA */
    wr8((ABS(0x02D2) + X), (uint8_t)A);  /* 00FD50 STA */
L_00FD53: TICK();
    mflag = 0;  /* 00FD53 CLM */
    Y = pull16();  /* 00FD54 PLY */
    D = pull16();  /* 00FD55 PLD */
    S += 3; return;  /* 00FD56 RTL */
}

/* 00FD57  callers: 005E28 */
void F_00FD57(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00FD57 LDA */
    push16(D);  /* 00FD59 PHD */
    D = A;  /* 00FD5A TAD */
    push16(A);  /* 00FD5B PHA */
    mflag = 1;  /* 00FD5C SEM */
    if ((rd8(ABS(0x033F)) & 0x80) == 0x80) goto L_00FD65;  /* 00FD5D BBS */
    goto L_00FE52;  /* 00FD62 JMP */
L_00FD65: TICK();
    if ((rd8(ABS(P4)) & 0x20) == 0x20) goto L_00FD6D;  /* 00FD65 BBS [P45 ACIL_CAGRI] */
    goto L_00FE4F;  /* 00FD6A JMP */
L_00FD6D: TICK();
    mflag = 0;  /* 00FD6D CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0080);  /* 00FD6E CLB */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00FD7D;  /* 00FD73 BBC */
    S -= 3; F_004A5C();  /* 00FD79 JSRL */
L_00FD7D: TICK();
    S -= 3; F_00FC35();  /* 00FD7D JSRL */
    X = A;  /* 00FD81 TAX */
    if (X != 0) goto L_00FD87;  /* 00FD82 BNE */
    goto L_00FE4F;  /* 00FD84 JMP */
L_00FD87: TICK();
    A = rd16(ABS(0x0365));  /* 00FD87 LDA */
    mflag = 1;  /* 00FD8A SEM */
    SETLO(A, A + 1);  /* 00FD8B INC */
    if ((A & 0xFF) != 0) goto L_00FDC0;  /* 00FD8C BNE */
    SETLO(A, rd8(ABS(0x0341)));  /* 00FD8E LDA */
    SETLO(A, A & 0x7F);  /* 00FD91 AND */
    mflag = 0;  /* 00FD93 CLM */
    Y = A;  /* 00FD94 TAY */
    S -= 3; F_008D14();  /* 00FD95 JSRL */
    wr16(DP(0x00), 0x0370);  /* 00FD99 LDM */
    wr16(DP(0x02), 0x0363);  /* 00FD9D LDM */
    A = 0x0000;  /* 00FDA1 LDA */
    Y = 0x000D;  /* 00FDA4 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00FDA7 JSRL */
    wr16(DP(0x00), 0x0363);  /* 00FDAB LDM */
    wr16(DP(0x02), 0x0356);  /* 00FDAF LDM */
    A = 0x0000;  /* 00FDB3 LDA */
    Y = 0x000D;  /* 00FDB6 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00FDB9 JSRL */
    goto L_00FE2C;  /* 00FDBD JMP */
L_00FDC0: TICK();
    mflag = 0;  /* 00FDC0 CLM */
    A = rd16(ABS(0x0341));  /* 00FDC1 LDA */
    mflag = 1;  /* 00FDC4 SEM */
    SETLO(A, A & 0x7F);  /* 00FDC5 AND */
    t = rd8(ABS(0x0376));  /* 00FDC7 CMP */
    if ((A & 0xFF) == t) goto L_00FE00;  /* 00FDCA BEQ */
    mflag = 0;  /* 00FDCC CLM */
    wr16(DP(0x00), 0x0370);  /* 00FDCD LDM */
    wr16(DP(0x02), 0x0356);  /* 00FDD1 LDM */
    A = 0x0000;  /* 00FDD5 LDA */
    Y = 0x000D;  /* 00FDD8 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00FDDB JSRL */
    A = rd16(ABS(0x0341));  /* 00FDDF LDA */
    mflag = 1;  /* 00FDE2 SEM */
    SETLO(A, A & 0x7F);  /* 00FDE3 AND */
    mflag = 0;  /* 00FDE5 CLM */
    Y = A;  /* 00FDE6 TAY */
    S -= 3; F_008D14();  /* 00FDE7 JSRL */
    wr16(DP(0x00), 0x0370);  /* 00FDEB LDM */
    wr16(DP(0x02), 0x0363);  /* 00FDEF LDM */
    A = 0x0000;  /* 00FDF3 LDA */
    Y = 0x000D;  /* 00FDF6 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00FDF9 JSRL */
    goto L_00FE2C;  /* 00FDFD JMP */
L_00FE00: TICK();
    mflag = 0;  /* 00FE00 CLM */
    wr16(DP(0x00), 0x0356);  /* 00FE01 LDM */
    wr16(DP(0x02), 0x0370);  /* 00FE05 LDM */
    A = 0x0000;  /* 00FE09 LDA */
    Y = 0x000D;  /* 00FE0C LDY */
    S -= 3; rt_memcpy_01E466();  /* 00FE0F JSRL */
    wr16(DP(0x00), 0x0370);  /* 00FE13 LDM */
    wr16(DP(0x02), 0x0363);  /* 00FE17 LDM */
    A = 0x0000;  /* 00FE1B LDA */
    Y = 0x000D;  /* 00FE1E LDY */
    S -= 3; rt_memcpy_01E466();  /* 00FE21 JSRL */
    Y = rd16(ABS(0x0376));  /* 00FE25 LDY */
    S -= 3; F_008A47();  /* 00FE28 JSRL */
L_00FE2C: TICK();
    Y = 0x0003;  /* 00FE2C LDY */
    S -= 3; F_00927F();  /* 00FE2F JSRL */
    Y = 0x002C;  /* 00FE33 LDY */
    S -= 3; F_003310();  /* 00FE36 JSRL */
    S -= 3; F_00828C();  /* 00FE3A JSRL */
    Y = 0x0000;  /* 00FE3E LDY */
    S -= 3; F_0080AA();  /* 00FE41 JSRL */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00FE45 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0020);  /* 00FE4A CLB */
L_00FE4F: TICK();
    goto L_00FE59;  /* 00FE4F JMP */
L_00FE52: TICK();
    mflag = 0;  /* 00FE52 CLM */
    A = 0x0000;  /* 00FE53 LDA */
    Y = pull16();  /* 00FE56 PLY */
    D = pull16();  /* 00FE57 PLD */
    S += 3; return;  /* 00FE58 RTL */
L_00FE59: TICK();
    mflag = 0;  /* 00FE59 CLM */
    A = 0x0001;  /* 00FE5A LDA */
    Y = pull16();  /* 00FE5D PLY */
    D = pull16();  /* 00FE5E PLD */
    S += 3; return;  /* 00FE5F RTL */
}

/* 00FE73  callers: 00FE68 */
void F_00FE73(void)
{
    uint32_t t; (void)t;
    wr16(DP(0x03), 0x046B);  /* 00FE73 LDM */
    wr16(DP(0x00), 0x0082);  /* 00FE77 LDM */
    mflag = 1;  /* 00FE7B SEM */
    wr8(DP(0x05), 0x00);  /* 00FE7C LDM */
    wr8(DP(0x02), 0x00);  /* 00FE7F LDM */
    mflag = 0;  /* 00FE82 CLM */
    S -= 2; F_00FF59();  /* 00FE83 JSR */
    A = 0x0000;  /* 00FE86 LDA */
    if (A == 0) goto L_00FE94;  /* 00FE89 BEQ */
    Y = 0x0082;  /* 00FE8B LDY */
    X = 0x2040;  /* 00FE8E LDX */
    mvn(0x00, 0x00);  /* 00FE91 MVN */
L_00FE94: TICK();
    wr16(DP(0x03), 0x0000);  /* 00FE94 LDM */
    wr16(DP(0x00), 0x0000);  /* 00FE98 LDM */
    mflag = 1;  /* 00FE9C SEM */
    wr8(DP(0x05), 0x00);  /* 00FE9D LDM */
    wr8(DP(0x02), 0x02);  /* 00FEA0 LDM */
    mflag = 0;  /* 00FEA3 CLM */
    S -= 2; F_00FF59();  /* 00FEA4 JSR */
    X = 0x0000;  /* 00FEA7 LDX */
    if (X != 0) goto L_00FEB1;  /* 00FEAA BNE */
    A = 0x0000;  /* 00FEAC LDA */
    if (A == 0) goto L_00FED0;  /* 00FEAF BEQ */
L_00FEB1: TICK();
    wr16(DP(0x00), 0x0000);  /* 00FEB1 LDM */
    wr16(DP(0x02), 0x0002);  /* 00FEB5 LDM */
    wr16(DP(0x03), 0x0000);  /* 00FEB9 LDM */
    wr16(DP(0x05), 0x0002);  /* 00FEBD LDM */
    wr16(DP(0x06), 0x0000);  /* 00FEC1 LDM */
    wr16(DP(0x08), 0x0000);  /* 00FEC5 LDM */
    A = 0x0000;  /* 00FEC9 LDA */
    S -= 3; rt_far_memcpy_init_01E388();  /* 00FECC JSRL */
L_00FED0: TICK();
    wr16(DP(0x03), 0x0000);  /* 00FED0 LDM */
    wr16(DP(0x00), 0x0000);  /* 00FED4 LDM */
    mflag = 1;  /* 00FED8 SEM */
    wr8(DP(0x05), 0x00);  /* 00FED9 LDM */
    wr8(DP(0x02), 0x02);  /* 00FEDC LDM */
    mflag = 0;  /* 00FEDF CLM */
    S -= 2; F_00FF59();  /* 00FEE0 JSR */
    X = 0x0000;  /* 00FEE3 LDX */
    if (X != 0) goto L_00FEED;  /* 00FEE6 BNE */
    X = 0x0000;  /* 00FEE8 LDX */
    if (X == 0) goto L_00FF0C;  /* 00FEEB BEQ */
L_00FEED: TICK();
    wr16(DP(0x00), 0x0640);  /* 00FEED LDM */
    wr16(DP(0x02), 0x0001);  /* 00FEF1 LDM */
    wr16(DP(0x03), 0x0000);  /* 00FEF5 LDM */
    wr16(DP(0x05), 0x0002);  /* 00FEF9 LDM */
    wr16(DP(0x06), 0x0000);  /* 00FEFD LDM */
    wr16(DP(0x08), 0x0000);  /* 00FF01 LDM */
    A = 0x0000;  /* 00FF05 LDA */
    S -= 3; rt_far_memcpy_init_01E388();  /* 00FF08 JSRL */
L_00FF0C: TICK();
    X = 0x0000;  /* 00FF0C LDX */
    if (X == 0) goto L_00FF47;  /* 00FF0F BEQ */
    DT = 0x00;  /* 00FF11 LDT */
    X = 0x0000;  /* 00FF14 LDX */
L_00FF17: TICK();
    /* 00FF17 CPX */
    if (X == 0x0000) goto L_00FF44;  /* 00FF1A BEQ */
    A = rd16((ABS(0x2043) + X));  /* 00FF1C LDA */
    C = 1;  /* 00FF1F SEC */
    A = sbc16(A, rd16((ABS(0x2040) + X)));  /* 00FF20 SBC */
    wr16(DP(0x03), A);  /* 00FF23 STA */
    mflag = 1;  /* 00FF25 SEM */
    SETLO(A, rd8((ABS(0x2045) + X)));  /* 00FF26 LDA */
    SETLO(A, sbc8(A, rd8((ABS(0x2042) + X))));  /* 00FF29 SBC */
    wr8(DP(0x05), (uint8_t)A);  /* 00FF2C STA */
    SETLO(A, rd8((ABS(0x2042) + X)));  /* 00FF2E LDA */
    wr8(DP(0x02), (uint8_t)A);  /* 00FF31 STA */
    mflag = 0;  /* 00FF33 CLM */
    A = rd16((ABS(0x2040) + X));  /* 00FF34 LDA */
    wr16(DP(0x00), A);  /* 00FF37 STA */
    S -= 2; F_00FF59();  /* 00FF39 JSR */
    A = X;  /* 00FF3C TXA */
    C = 0;  /* 00FF3D CLC */
    A = adc16(A, 0x0006);  /* 00FF3E ADC */
    X = A;  /* 00FF41 TAX */
    goto L_00FF17;  /* 00FF42 BRA */
L_00FF44: TICK();
    DT = 0x00;  /* 00FF44 LDT */
L_00FF47: TICK();
    A = 0x0000;  /* 00FF47 LDA */
    if (A == 0) goto L_00FF58;  /* 00FF4A BEQ */
    Y = 0x04EE;  /* 00FF4C LDY */
    X = 0x2088;  /* 00FF4F LDX */
    mvn(0x00, 0x00);  /* 00FF52 MVN */
    DT = 0x00;  /* 00FF55 LDT */
L_00FF58: TICK();
    S += 2; return;  /* 00FF58 RTS */
}

/* 00FF59  callers: 00FE83, 00FEA4, 00FEE0, 00FF39, 00FF6D */
void F_00FF59(void)
{
    uint32_t t; (void)t;
L_00FF59: TICK();
    mflag = 1;  /* 00FF59 SEM */
    SETLO(A, rd8(DP(0x05)));  /* 00FF5A LDA */
    Z = ((A & 0xFF) == 0);
    mflag = 0;  /* 00FF5C CLM */
    if (Z) goto L_00FF6F;  /* 00FF5D BEQ */
    Y = 0x0000;  /* 00FF5F LDY */
    A = Y;  /* 00FF62 TYA */
L_00FF63: TICK();
    wr16((LIND(0x00) + Y), A);  /* 00FF63 STA */
    Y += 1;  /* 00FF65 INY */
    Y += 1;  /* 00FF66 INY */
    if (Y != 0) goto L_00FF63;  /* 00FF67 BNE */
    t = (rd16(DP(0x05)) - 1) & 0xFFFF; wr16(DP(0x05), t);  /* 00FF69 DEC */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 00FF6B INC */
    goto L_00FF59;  /* 00FF6D BRA */
L_00FF6F: TICK();
    Y = rd16(DP(0x03));  /* 00FF6F LDY */
    A = Y;  /* 00FF71 TYA */
    A = lsr16(A);  /* 00FF72 LSR */
    A = 0x0000;  /* 00FF73 LDA */
    if (!C) goto L_00FF7D;  /* 00FF76 BCC */
    Y -= 1;  /* 00FF78 DEY */
    mflag = 1;  /* 00FF79 SEM */
    wr8((LIND(0x00) + Y), (uint8_t)A);  /* 00FF7A STA */
    mflag = 0;  /* 00FF7C CLM */
L_00FF7D: TICK();
    /* 00FF7D CPY */
    if (Y != 0x0000) goto L_00FF85;  /* 00FF80 BNE */
    S += 2; return;  /* 00FF82 RTS */
L_00FF83: TICK();
    wr16((LIND(0x00) + Y), A);  /* 00FF83 STA */
L_00FF85: TICK();
    Y -= 1;  /* 00FF85 DEY */
    Y -= 1;  /* 00FF86 DEY */
    if (Y != 0) goto L_00FF83;  /* 00FF87 BNE */
    wr16(LIND(0x00), A);  /* 00FF89 STA */
    S += 2; return;  /* 00FF8B RTS */
}

/* 00FF8C  callers: 006154, 0061C4, 00627E, 0078A6, 00A039, 00A574, 00E9C5, 00F54C ... */
void F_00FF8C(void)
{
    uint32_t t; (void)t;
    push16(X);  /* 00FF8C PHX */
    X = S;  /* 00FF8D TSX */
    push16(A);  /* 00FF8E PHA */
    push16(B);  /* 00FF8F PHB */
    B = 0x0000;  /* 00FF91 LDA */
    A |= rd16(SR(0x01));  /* 00FF95 ORA */
    if ((A & 0x8000) != 0) goto L_00FFA1;  /* 00FF97 BMI */
    A = rd16(SR(0x03));  /* 00FF99 LDA */
    div16(rd16(SR(0x01)));  /* 00FF9B DIV */
L_00FF9E: TICK();
    S = X;  /* 00FF9E TXS */
    X = pull16();  /* 00FF9F PLX */
    S += 3; return;  /* 00FFA0 RTL */
L_00FFA1: TICK();
    A = rd16(SR(0x01));  /* 00FFA1 LDA */
    A ^= rd16(SR(0x03));  /* 00FFA3 EOR */
    push16(A);  /* 00FFA5 PHA */
    A = rd16(SR(0x03));  /* 00FFA6 LDA */
    if ((A & 0x8000) == 0) goto L_00FFB0;  /* 00FFA8 BPL */
    A ^= 0xFFFF;  /* 00FFAA EOR */
    A += 1;  /* 00FFAD INC */
    wr16(SR(0x03), A);  /* 00FFAE STA */
L_00FFB0: TICK();
    A = rd16(SR(0x05));  /* 00FFB0 LDA */
    if ((A & 0x8000) == 0) goto L_00FFB8;  /* 00FFB2 BPL */
    A ^= 0xFFFF;  /* 00FFB4 EOR */
    A += 1;  /* 00FFB7 INC */
L_00FFB8: TICK();
    div16(rd16(SR(0x03)));  /* 00FFB8 DIV */
    B = rd16(SR(0x01));  /* 00FFBB LDA */
    if ((B & 0x8000) == 0) goto L_00FF9E;  /* 00FFBE BPL */
    A ^= 0xFFFF;  /* 00FFC0 EOR */
    A += 1;  /* 00FFC3 INC */
    goto L_00FF9E;  /* 00FFC4 BRA */
}

