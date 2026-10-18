/* ramcode.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 000080  callers: 010624 */
void ramcode_flash_loader(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 000080 LDA */
    push16(D);  /* 000082 PHD */
    D = A;  /* 000083 TAD */
    C = 0;  /* 000084 CLC */
    A = adc16(A, 0x009A);  /* 000085 ADC */
    push16(A);  /* 000088 PHA */
    wr16(DP(0x01), Y);  /* 000089 STY */
    mflag = 1;  /* 00008B SEM */
    wr8(DP(0x8F), 0x01);  /* 00008C LDM */
    wr8(DP(0x90), 0x0A);  /* 00008F LDM */
    mflag = 0;  /* 000092 CLM */
    wr16(DP(0x93), 0x0000);  /* 000093 LDM */
    mflag = 1;  /* 000097 SEM */
    wr8(DP(0x92), 0x00);  /* 000098 LDM */
    mflag = 0;  /* 00009B CLM */
    S -= 3; F_00052D();  /* 00009C JSRL */
L_0000A0: TICK();
    mflag = 1;  /* 0000A0 SEM */
    wr8(DP(0x91), 0x00);  /* 0000A1 LDM */
L_0000A4: TICK();
    wr8(ABS(WDT), 0x00);  /* 0000A4 LDM */
    if ((rd8(ABS(U0C1)) & 0x08) == 0) goto L_0000A4;  /* 0000A8 BBC */
    if ((rd8(ABS(U0C1)) & 0x80) == 0) goto L_0000BA;  /* 0000AD BBC */
    SETLO(A, rd8(ABS(U0RB)));  /* 0000B2 LDA */
    wr8(DP(0x8D), (uint8_t)A);  /* 0000B5 STA */
    goto L_000179;  /* 0000B7 BRAL */
L_0000BA: TICK();
    mflag = 1;  /* 0000BA SEM */
    SETLO(A, rd8(ABS(U0RB)));  /* 0000BB LDA */
    wr8(DP(0x8D), (uint8_t)A);  /* 0000BE STA */
    SETLO(A, rd8(DP(0x8F)));  /* 0000C0 LDA */
    /* 0000C2 CMP */
    if ((A & 0xFF) != 0x01) goto L_0000D8;  /* 0000C4 BNE */
    mflag = 0;  /* 0000C6 CLM */
    wr16(DP(0x93), 0x0000);  /* 0000C7 LDM */
    A = rd16(DP(0x8D));  /* 0000CB LDA */
    mflag = 1;  /* 0000CD SEM */
    /* 0000CE CMP */
    if ((A & 0xFF) != 0x16) goto L_0000D5;  /* 0000D0 BNE */
    wr8(DP(0x8F), 0x02);  /* 0000D2 LDM */
L_0000D5: TICK();
    goto L_000179;  /* 0000D5 BRAL */
L_0000D8: TICK();
    mflag = 0;  /* 0000D8 CLM */
    A = rd16(DP(0x8F));  /* 0000D9 LDA */
    mflag = 1;  /* 0000DB SEM */
    /* 0000DC CMP */
    if ((A & 0xFF) != 0x02) goto L_0000F1;  /* 0000DE BNE */
    SETLO(A, rd8(DP(0x8D)));  /* 0000E0 LDA */
    /* 0000E2 CMP */
    if ((A & 0xFF) != 0x10) goto L_0000EA;  /* 0000E4 BNE */
    SETLO(A, 0x03);  /* 0000E6 LDA */
    goto L_0000EC;  /* 0000E8 BRA */
L_0000EA: TICK();
    SETLO(A, 0x01);  /* 0000EA LDA */
L_0000EC: TICK();
    wr8(DP(0x8F), (uint8_t)A);  /* 0000EC STA */
    goto L_000179;  /* 0000EE BRAL */
L_0000F1: TICK();
    mflag = 0;  /* 0000F1 CLM */
    A = rd16(DP(0x8F));  /* 0000F2 LDA */
    mflag = 1;  /* 0000F4 SEM */
    /* 0000F5 CMP */
    if ((A & 0xFF) != 0x03) goto L_00010A;  /* 0000F7 BNE */
    SETLO(A, rd8(DP(0x8D)));  /* 0000F9 LDA */
    /* 0000FB CMP */
    if ((A & 0xFF) != 0x02) goto L_000103;  /* 0000FD BNE */
    SETLO(A, 0x04);  /* 0000FF LDA */
    goto L_000105;  /* 000101 BRA */
L_000103: TICK();
    SETLO(A, 0x01);  /* 000103 LDA */
L_000105: TICK();
    wr8(DP(0x8F), (uint8_t)A);  /* 000105 STA */
    goto L_000179;  /* 000107 BRAL */
L_00010A: TICK();
    mflag = 0;  /* 00010A CLM */
    A = rd16(DP(0x8F));  /* 00010B LDA */
    mflag = 1;  /* 00010D SEM */
    /* 00010E CMP */
    if ((A & 0xFF) != 0x04) goto L_000129;  /* 000110 BNE */
    SETLO(A, rd8(DP(0x8D)));  /* 000112 LDA */
    /* 000114 CMP */
    if ((A & 0xFF) != 0x10) goto L_00011D;  /* 000116 BNE */
    wr8(DP(0x8F), 0x05);  /* 000118 LDM */
    goto L_000127;  /* 00011B BRA */
L_00011D: TICK();
    X = rd16(DP(0x93));  /* 00011D LDX */
    mflag = 0;  /* 00011F CLM */
    t = (rd16(DP(0x93)) + 1) & 0xFFFF; wr16(DP(0x93), t);  /* 000120 INC */
    A = rd16(DP(0x8D));  /* 000122 LDA */
    mflag = 1;  /* 000124 SEM */
    wr8(DP(0x02 + X), (uint8_t)A);  /* 000125 STA */
L_000127: TICK();
    goto L_000179;  /* 000127 BRA */
L_000129: TICK();
    mflag = 0;  /* 000129 CLM */
    A = rd16(DP(0x8F));  /* 00012A LDA */
    mflag = 1;  /* 00012C SEM */
    /* 00012D CMP */
    if ((A & 0xFF) != 0x05) goto L_00015E;  /* 00012F BNE */
    SETLO(A, rd8(DP(0x8D)));  /* 000131 LDA */
    /* 000133 CMP */
    if ((A & 0xFF) != 0x10) goto L_000146;  /* 000135 BNE */
    wr8(DP(0x8F), 0x04);  /* 000137 LDM */
    X = rd16(DP(0x93));  /* 00013A LDX */
    mflag = 0;  /* 00013C CLM */
    t = (rd16(DP(0x93)) + 1) & 0xFFFF; wr16(DP(0x93), t);  /* 00013D INC */
    A = rd16(DP(0x8D));  /* 00013F LDA */
    mflag = 1;  /* 000141 SEM */
    wr8(DP(0x02 + X), (uint8_t)A);  /* 000142 STA */
    goto L_00015C;  /* 000144 BRA */
L_000146: TICK();
    mflag = 0;  /* 000146 CLM */
    A = rd16(DP(0x8D));  /* 000147 LDA */
    mflag = 1;  /* 000149 SEM */
    /* 00014A CMP */
    if ((A & 0xFF) != 0x03) goto L_000153;  /* 00014C BNE */
    wr8(DP(0x8F), 0x06);  /* 00014E LDM */
    goto L_00015C;  /* 000151 BRA */
L_000153: TICK();
    mflag = 1;  /* 000153 SEM */
    wr8(DP(0x8F), 0x01);  /* 000154 LDM */
    mflag = 0;  /* 000157 CLM */
    wr16(DP(0x93), 0x0000);  /* 000158 LDM */
L_00015C: TICK();
    goto L_000179;  /* 00015C BRA */
L_00015E: TICK();
    mflag = 0;  /* 00015E CLM */
    A = rd16(DP(0x8F));  /* 00015F LDA */
    mflag = 1;  /* 000161 SEM */
    /* 000162 CMP */
    if ((A & 0xFF) != 0x06) goto L_00016B;  /* 000164 BNE */
    wr8(DP(0x8F), 0x07);  /* 000166 LDM */
    goto L_000179;  /* 000169 BRA */
L_00016B: TICK();
    mflag = 0;  /* 00016B CLM */
    A = rd16(DP(0x8F));  /* 00016C LDA */
    mflag = 1;  /* 00016E SEM */
    /* 00016F CMP */
    if ((A & 0xFF) != 0x07) goto L_000179;  /* 000171 BNE */
    wr8(DP(0x8F), 0x01);  /* 000173 LDM */
    wr8(DP(0x91), 0x01);  /* 000176 LDM */
L_000179: TICK();
    mflag = 1;  /* 000179 SEM */
    SETLO(A, rd8(DP(0x91)));  /* 00017A LDA */
    if ((A & 0xFF) != 0) goto L_000181;  /* 00017C BNE */
    goto L_0000A4;  /* 00017E BRAL */
L_000181: TICK();
    wr8(ABS(WDT), 0x00);  /* 000181 LDM */
    SETLO(A, rd8(DP(0x90)));  /* 000185 LDA */
    /* 000187 CMP */
    if ((A & 0xFF) != 0x09) goto L_0001BF;  /* 000189 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 00018B LDA */
    SETLO(A, A - 1);  /* 00018D DEC */
    if ((A & 0xFF) != 0) goto L_0001A4;  /* 00018E BNE */
    wr8(DP(0x01), 0x01);  /* 000190 LDM */
    wr8(DP(0x00), 0x01);  /* 000193 LDM */
    Y = 0x0001;  /* 000196 LDY */
    mflag = 0;  /* 000199 CLM */
    S -= 3; F_0004E0();  /* 00019A JSRL */
    mflag = 1;  /* 00019E SEM */
    wr8(DP(0x90), 0x0A);  /* 00019F LDM */
    goto L_0001BC;  /* 0001A2 BRA */
L_0001A4: TICK();
    mflag = 0;  /* 0001A4 CLM */
    A = rd16(DP(0x02));  /* 0001A5 LDA */
    mflag = 1;  /* 0001A7 SEM */
    /* 0001A8 CMP */
    if ((A & 0xFF) != 0x02) goto L_0001B1;  /* 0001AA BNE */
    wr8(DP(0x90), 0x0A);  /* 0001AC LDM */
    goto L_0001BC;  /* 0001AF BRA */
L_0001B1: TICK();
    mflag = 0;  /* 0001B1 CLM */
    A = rd16(DP(0x02));  /* 0001B2 LDA */
    mflag = 1;  /* 0001B4 SEM */
    /* 0001B5 CMP */
    if ((A & 0xFF) != 0x04) goto L_0001BC;  /* 0001B7 BNE */
    wr8(DP(0x90), 0x08);  /* 0001B9 LDM */
L_0001BC: TICK();
    goto L_000444;  /* 0001BC BRAL */
L_0001BF: TICK();
    mflag = 0;  /* 0001BF CLM */
    A = rd16(DP(0x90));  /* 0001C0 LDA */
    mflag = 1;  /* 0001C2 SEM */
    /* 0001C3 CMP */
    if ((A & 0xFF) == 0x0A) goto L_0001CA;  /* 0001C5 BEQ */
    goto L_000444;  /* 0001C7 BRAL */
L_0001CA: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 0001CA LDA */
    SETLO(A, A - 1);  /* 0001CC DEC */
    if ((A & 0xFF) != 0) goto L_0001E0;  /* 0001CD BNE */
    wr8(DP(0x01), 0x01);  /* 0001CF LDM */
    wr8(DP(0x00), 0x01);  /* 0001D2 LDM */
    Y = 0x0001;  /* 0001D5 LDY */
    mflag = 0;  /* 0001D8 CLM */
    S -= 3; F_0004E0();  /* 0001D9 JSRL */
    goto L_000444;  /* 0001DD BRAL */
L_0001E0: TICK();
    mflag = 0;  /* 0001E0 CLM */
    A = rd16(DP(0x02));  /* 0001E1 LDA */
    mflag = 1;  /* 0001E3 SEM */
    /* 0001E4 CMP */
    if ((A & 0xFF) != 0x02) goto L_000200;  /* 0001E6 BNE */
    SETLO(A, rd8(DP(0x03)));  /* 0001E8 LDA */
    mflag = 0;  /* 0001EA CLM */
    A &= 0x00FF;  /* 0001EB AND */
    wr16(DP(0x9A), A);  /* 0001EE STA */
    A = rd16(DP(0x01));  /* 0001F0 LDA */
    A &= 0x00FF;  /* 0001F2 AND */
    A += 1;  /* 0001F5 INC */
    t = rd16(DP(0x9A));  /* 0001F6 CMP */
    if (A != t) goto L_0001FD;  /* 0001F8 BNE */
    mflag = 1;  /* 0001FA SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0001FB INC */
L_0001FD: TICK();
    goto L_000444;  /* 0001FD BRAL */
L_000200: TICK();
    mflag = 0;  /* 000200 CLM */
    A = rd16(DP(0x02));  /* 000201 LDA */
    mflag = 1;  /* 000203 SEM */
    /* 000204 CMP */
    if ((A & 0xFF) == 0x04) goto L_00020B;  /* 000206 BEQ */
    goto L_000444;  /* 000208 BRAL */
L_00020B: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 00020B LDA */
    t = rd8(DP(0x00));  /* 00020D CMP */
    if ((A & 0xFF) == t) goto L_000214;  /* 00020F BEQ */
    goto L_00043D;  /* 000211 BRAL */
L_000214: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 000214 INC */
    SETLO(A, rd8(DP(0x06)));  /* 000216 LDA */
    /* 000218 CMP */
    if ((A & 0xFF) == 0x03) goto L_00021F;  /* 00021A BEQ */
    goto L_0003FE;  /* 00021C BRAL */
L_00021F: TICK();
    SETLO(A, rd8(DP(0x08)));  /* 00021F LDA */
    /* 000221 CMP */
    if ((A & 0xFF) == 0x02) goto L_000228;  /* 000223 BEQ */
    goto L_0003F5;  /* 000225 BRAL */
L_000228: TICK();
    SETLO(A, rd8(DP(0x0B)));  /* 000228 LDA */
    wr8(DP(0x97), (uint8_t)A);  /* 00022A STA */
    SETLO(A, rd8(DP(0x0A)));  /* 00022C LDA */
    wr8(DP(0x98), (uint8_t)A);  /* 00022E STA */
    SETLO(A, rd8(DP(0x09)));  /* 000230 LDA */
    wr8(DP(0x99), (uint8_t)A);  /* 000232 STA */
    SETLO(A, rd8(DP(0x07)));  /* 000234 LDA */
    SETLO(A, A - 1);  /* 000236 DEC */
    if ((A & 0xFF) == 0) goto L_00023C;  /* 000237 BEQ */
    goto L_0002BD;  /* 000239 BRAL */
L_00023C: TICK();
    Y = rd16(DP(0x00));  /* 00023C LDY */
    mflag = 0;  /* 00023E CLM */
    S -= 3; F_0004E0();  /* 00023F JSRL */
    S -= 3; F_0004A5();  /* 000243 JSRL */
    Y = 0x0004;  /* 000247 LDY */
    S -= 3; F_00050E();  /* 00024A JSRL */
    Y = rd16(DP(0x01));  /* 00024E LDY */
    S -= 3; F_000486();  /* 000250 JSRL */
    Y = 0x0001;  /* 000254 LDY */
    S -= 3; F_00050E();  /* 000257 JSRL */
    Y = 0x0000;  /* 00025B LDY */
    S -= 3; F_00050E();  /* 00025E JSRL */
    Y = rd16(DP(0x06));  /* 000262 LDY */
    S -= 3; F_000486();  /* 000264 JSRL */
    Y = 0x0004;  /* 000268 LDY */
    S -= 3; F_000486();  /* 00026B JSRL */
    Y = rd16(DP(0x08));  /* 00026F LDY */
    S -= 3; F_000486();  /* 000271 JSRL */
    Y = rd16(DP(0x09));  /* 000275 LDY */
    S -= 3; F_000486();  /* 000277 JSRL */
    Y = rd16(DP(0x0A));  /* 00027B LDY */
    S -= 3; F_000486();  /* 00027D JSRL */
    Y = rd16(DP(0x0B));  /* 000281 LDY */
    S -= 3; F_000486();  /* 000283 JSRL */
    wr16(DP(0x95), 0x0000);  /* 000287 LDM */
L_00028B: TICK();
    A = rd16(DP(0x0C));  /* 00028B LDA */
    A &= 0x00FF;  /* 00028D AND */
    t = rd16(DP(0x95));  /* 000290 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_0002B5;  /* 000292 BCC */
    if (Z) goto L_0002B5;  /* 000294 BEQ */
    mflag = 1;  /* 000296 SEM */
    wr8(ABS(WDT), 0x00);  /* 000297 LDM */
    SETLO(B, rd8(DP(0x99)));  /* 00029B LDA */
    mflag = 0;  /* 00029E CLM */
    A = rd16(DP(0x97));  /* 00029F LDA */
    t = (rd16(DP(0x97)) + 1) & 0xFFFF; wr16(DP(0x97), t);  /* 0002A1 INC */
    wr16(DP(0x9A), A);  /* 0002A3 STA */
    mflag = 1;  /* 0002A5 SEM */
    wr8(DP(0x9C), (uint8_t)B);  /* 0002A6 STA */
    mflag = 0;  /* 0002A9 CLM */
    A = rd16(LIND(0x9A));  /* 0002AA LDA */
    Y = A;  /* 0002AC TAY */
    S -= 3; F_000486();  /* 0002AD JSRL */
    t = (rd16(DP(0x95)) + 1) & 0xFFFF; wr16(DP(0x95), t);  /* 0002B1 INC */
    goto L_00028B;  /* 0002B3 BRA */
L_0002B5: TICK();
    mflag = 0;  /* 0002B5 CLM */
    S -= 3; F_0004BF();  /* 0002B6 JSRL */
    goto L_0003F3;  /* 0002BA BRAL */
L_0002BD: TICK();
    mflag = 1;  /* 0002BD SEM */
    /* 0002BE CMP */
    if ((A & 0xFF) == 0x02) goto L_0002C5;  /* 0002C0 BEQ */
    goto L_0003EC;  /* 0002C2 BRAL */
L_0002C5: TICK();
    SETLO(A, rd8(DP(0x92)));  /* 0002C5 LDA */
    if ((A & 0xFF) == 0) goto L_0002CC;  /* 0002C7 BEQ */
    goto L_00037F;  /* 0002C9 BRAL */
L_0002CC: TICK();
    mflag = 0;  /* 0002CC CLM */
    wr16(DP(0x95), 0x0000);  /* 0002CD LDM */
L_0002D1: TICK();
    A = rd16(DP(0x93));  /* 0002D1 LDA */
    C = 0;  /* 0002D3 CLC */
    A = adc16(A, 0xFFF6);  /* 0002D4 ADC */
    t = rd16(DP(0x95));  /* 0002D7 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_0002DD;  /* 0002D9 BCC */
    if (!Z) goto L_0002E0;  /* 0002DB BNE */
L_0002DD: TICK();
    goto L_00037D;  /* 0002DD BRAL */
L_0002E0: TICK();
    mflag = 1;  /* 0002E0 SEM */
    wr8(ABS(0x5555), 0xAA);  /* 0002E1 LDM */
    wr8(ABS(0x2AAA), 0x55);  /* 0002E5 LDM */
    wr8(ABS(0x5555), 0xA0);  /* 0002E9 LDM */
    X = rd16(DP(0x95));  /* 0002ED LDX */
    SETLO(A, rd8(DP(0x0C + X)));  /* 0002EF LDA */
    wr8(DP(0x8E), (uint8_t)A);  /* 0002F1 STA */
    wr8(LIND(0x97), (uint8_t)A);  /* 0002F3 STA */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 0002F5 CLB */
    wr8(ABS(TA3IC), 0x00);  /* 0002F9 LDM */
    wr8(ABS(TA3MOD), 0x80);  /* 0002FD LDM */
    mflag = 0;  /* 000301 CLM */
    wr16(ABS(TA3), 0x9600);  /* 000302 LDM */
    mflag = 1;  /* 000307 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 000308 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 00030C SEB */
L_000310: TICK();
    wr8(ABS(WDT), 0x00);  /* 000310 LDM */
    SETLO(A, rd8(LIND(0x97)));  /* 000314 LDA */
    SETLO(A, A & 0x20);  /* 000316 AND */
    if ((A & 0xFF) == 0) goto L_00033C;  /* 000318 BEQ */
    wr8(ABS(0x5555), 0xAA);  /* 00031A LDM */
    wr8(ABS(0x2AAA), 0x55);  /* 00031E LDM */
    wr8(ABS(0x5555), 0xF0);  /* 000322 LDM */
    SETLO(A, rd8(LIND(0x97)));  /* 000326 LDA */
    t = rd8(DP(0x8E));  /* 000328 CMP */
    if ((A & 0xFF) == t) goto L_00033C;  /* 00032A BEQ */
    wr8(ABS(0x5555), 0xAA);  /* 00032C LDM */
    wr8(ABS(0x2AAA), 0x55);  /* 000330 LDM */
    wr8(ABS(0x5555), 0xA0);  /* 000334 LDM */
    SETLO(A, rd8(DP(0x8E)));  /* 000338 LDA */
    wr8(LIND(0x97), (uint8_t)A);  /* 00033A STA */
L_00033C: TICK();
    SETLO(A, rd8(LIND(0x97)));  /* 00033C LDA */
    t = rd8(DP(0x8E));  /* 00033E CMP */
    if ((A & 0xFF) == t) goto L_000347;  /* 000340 BEQ */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_000310;  /* 000342 BBC */
L_000347: TICK();
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_000375;  /* 000347 BBC */
    wr8(DP(0x92), 0x01);  /* 00034C LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00034F CLB */
    mflag = 0;  /* 000353 CLM */
    wr16(ABS(TA3), 0x0640);  /* 000354 LDM */
    mflag = 1;  /* 000359 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 00035A CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 00035E SEB */
L_000362: TICK();
    wr8(ABS(WDT), 0x00);  /* 000362 LDM */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_000362;  /* 000366 BBC */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 00036B CLB */
    SETLO(A, rd8(DP(0x0B)));  /* 00036F LDA */
    wr8(DP(0x97), (uint8_t)A);  /* 000371 STA */
    goto L_00037F;  /* 000373 BRA */
L_000375: TICK();
    mflag = 0;  /* 000375 CLM */
    t = (rd16(DP(0x97)) + 1) & 0xFFFF; wr16(DP(0x97), t);  /* 000376 INC */
    t = (rd16(DP(0x95)) + 1) & 0xFFFF; wr16(DP(0x95), t);  /* 000378 INC */
    goto L_0002D1;  /* 00037A BRAL */
L_00037D: TICK();
    goto L_0003EC;  /* 00037D BRA */
L_00037F: TICK();
    mflag = 1;  /* 00037F SEM */
    wr8(ABS(0x5555), 0xAA);  /* 000380 LDM */
    wr8(ABS(0x2AAA), 0x55);  /* 000384 LDM */
    wr8(ABS(0x5555), 0xA0);  /* 000388 LDM */
    mflag = 0;  /* 00038C CLM */
    wr16(DP(0x95), 0x0000);  /* 00038D LDM */
L_000391: TICK();
    A = rd16(DP(0x93));  /* 000391 LDA */
    C = 0;  /* 000393 CLC */
    A = adc16(A, 0xFFF6);  /* 000394 ADC */
    t = rd16(DP(0x95));  /* 000397 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_0003AE;  /* 000399 BCC */
    if (Z) goto L_0003AE;  /* 00039B BEQ */
    X = rd16(DP(0x95));  /* 00039D LDX */
    A = rd16(DP(0x0C + X));  /* 00039F LDA */
    mflag = 1;  /* 0003A1 SEM */
    wr8(DP(0x8E), (uint8_t)A);  /* 0003A2 STA */
    wr8(LIND(0x97), (uint8_t)A);  /* 0003A4 STA */
    mflag = 0;  /* 0003A6 CLM */
    t = (rd16(DP(0x97)) + 1) & 0xFFFF; wr16(DP(0x97), t);  /* 0003A7 INC */
    X += 1;  /* 0003A9 INX */
    wr16(DP(0x95), X);  /* 0003AA STX */
    goto L_000391;  /* 0003AC BRA */
L_0003AE: TICK();
    mflag = 0;  /* 0003AE CLM */
    wr16(ABS(TA3), 0x0140);  /* 0003AF LDM */
    mflag = 1;  /* 0003B4 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 0003B5 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 0003B9 SEB */
L_0003BD: TICK();
    wr8(ABS(WDT), 0x00);  /* 0003BD LDM */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_0003BD;  /* 0003C1 BBC */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 0003C6 CLB */
    mflag = 0;  /* 0003CA CLM */
    wr16(ABS(TA3), 0x0C80);  /* 0003CB LDM */
    mflag = 1;  /* 0003D0 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 0003D1 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 0003D5 SEB */
L_0003D9: TICK();
    wr8(ABS(WDT), 0x00);  /* 0003D9 LDM */
    SETLO(A, rd8(LIND(0x97)));  /* 0003DD LDA */
    t = rd8(DP(0x8E));  /* 0003DF CMP */
    if ((A & 0xFF) == t) goto L_0003E8;  /* 0003E1 BEQ */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_0003D9;  /* 0003E3 BBC */
L_0003E8: TICK();
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 0003E8 CLB */
L_0003EC: TICK();
    Y = rd16(DP(0x00));  /* 0003EC LDY */
    mflag = 0;  /* 0003EE CLM */
    S -= 3; F_0004E0();  /* 0003EF JSRL */
L_0003F3: TICK();
    goto L_0003FC;  /* 0003F3 BRA */
L_0003F5: TICK();
    Y = rd16(DP(0x00));  /* 0003F5 LDY */
    mflag = 0;  /* 0003F7 CLM */
    S -= 3; F_0004E0();  /* 0003F8 JSRL */
L_0003FC: TICK();
    goto L_00043B;  /* 0003FC BRA */
L_0003FE: TICK();
    mflag = 0;  /* 0003FE CLM */
    A = rd16(DP(0x06));  /* 0003FF LDA */
    mflag = 1;  /* 000401 SEM */
    /* 000402 CMP */
    if ((A & 0xFF) != 0x04) goto L_000414;  /* 000404 BNE */
    Y = rd16(DP(0x00));  /* 000406 LDY */
    mflag = 0;  /* 000408 CLM */
    S -= 3; F_0004E0();  /* 000409 JSRL */
    mflag = 1;  /* 00040D SEM */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 00040E SEB */
    goto L_00043B;  /* 000412 BRA */
L_000414: TICK();
    mflag = 0;  /* 000414 CLM */
    A = rd16(DP(0x06));  /* 000415 LDA */
    mflag = 1;  /* 000417 SEM */
    /* 000418 CMP */
    if ((A & 0xFF) != 0x0D) goto L_000434;  /* 00041A BNE */
    Y = rd16(DP(0x00));  /* 00041C LDY */
    mflag = 0;  /* 00041E CLM */
    S -= 3; F_0004E0();  /* 00041F JSRL */
    mflag = 1;  /* 000423 SEM */
    wr8(DP(0x92), 0x00);  /* 000424 LDM */
    SETLO(A, rd8(DP(0x07)));  /* 000427 LDA */
    /* 000429 CMP */
    if ((A & 0xFF) != 0x03) goto L_000432;  /* 00042B BNE */
    mflag = 0;  /* 00042D CLM */
    S -= 3; F_00052D();  /* 00042E JSRL */
L_000432: TICK();
    goto L_00043B;  /* 000432 BRA */
L_000434: TICK();
    Y = rd16(DP(0x00));  /* 000434 LDY */
    mflag = 0;  /* 000436 CLM */
    S -= 3; F_0004E0();  /* 000437 JSRL */
L_00043B: TICK();
    goto L_000444;  /* 00043B BRA */
L_00043D: TICK();
    Y = rd16(DP(0x00));  /* 00043D LDY */
    mflag = 0;  /* 00043F CLM */
    S -= 3; F_0004E0();  /* 000440 JSRL */
L_000444: TICK();
    mflag = 0;  /* 000444 CLM */
    A = rd16(DP(0x90));  /* 000445 LDA */
    mflag = 1;  /* 000447 SEM */
    /* 000448 CMP */
    if ((A & 0xFF) != 0x08) goto L_000483;  /* 00044A BNE */
    mflag = 0;  /* 00044C CLM */
    wr16(DP(0x93), 0x0000);  /* 00044D LDM */
    mflag = 1;  /* 000451 SEM */
    wr8(DP(0x8F), 0x01);  /* 000452 LDM */
    wr8(DP(0x00), 0x01);  /* 000455 LDM */
    wr8(DP(0x01), 0x01);  /* 000458 LDM */
    wr8(DP(0x90), 0x09);  /* 00045B LDM */
    mflag = 0;  /* 00045E CLM */
    S -= 3; F_0004A5();  /* 00045F JSRL */
    Y = 0x0001;  /* 000463 LDY */
    S -= 3; F_00050E();  /* 000466 JSRL */
    Y = 0x0001;  /* 00046A LDY */
    S -= 3; F_000486();  /* 00046D JSRL */
    Y = 0x0001;  /* 000471 LDY */
    S -= 3; F_000486();  /* 000474 JSRL */
    Y = 0x0080;  /* 000478 LDY */
    S -= 3; F_000486();  /* 00047B JSRL */
    S -= 3; F_0004BF();  /* 00047F JSRL */
L_000483: TICK();
    goto L_0000A0;  /* 000483 BRAL */
}

/* 000486  callers: 000250, 000264, 00026B, 000271, 000277, 00027D, 000283, 0002AD ... */
void F_000486(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 000486 LDA */
    push16(D);  /* 000488 PHD */
    D = A;  /* 000489 TAD */
    A += 1;  /* 00048A INC */
    push16(A);  /* 00048B PHA */
    wr16(DP(0x00), Y);  /* 00048C STY */
    S -= 3; F_00050E();  /* 00048E JSRL */
    A = rd16(DP(0x00));  /* 000492 LDA */
    mflag = 1;  /* 000494 SEM */
    /* 000495 CMP */
    if ((A & 0xFF) != 0x10) goto L_0004A1;  /* 000497 BNE */
    Y = 0x0010;  /* 000499 LDY */
    mflag = 0;  /* 00049C CLM */
    S -= 3; F_00050E();  /* 00049D JSRL */
L_0004A1: TICK();
    mflag = 0;  /* 0004A1 CLM */
    Y = pull16();  /* 0004A2 PLY */
    D = pull16();  /* 0004A3 PLD */
    S += 3; return;  /* 0004A4 RTL */
}

/* 0004A5  callers: 000243, 00045F, 0004E8 */
void F_0004A5(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0004A5 LDA */
    push16(A);  /* 0004A7 PHA */
    Y = 0x0016;  /* 0004A8 LDY */
    S -= 3; F_00050E();  /* 0004AB JSRL */
    Y = 0x0010;  /* 0004AF LDY */
    S -= 3; F_00050E();  /* 0004B2 JSRL */
    Y = 0x0002;  /* 0004B6 LDY */
    S -= 3; F_00050E();  /* 0004B9 JSRL */
    Y = pull16();  /* 0004BD PLY */
    S += 3; return;  /* 0004BE RTL */
}

/* 0004BF  callers: 0002B6, 00047F, 000507 */
void F_0004BF(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0004BF LDA */
    push16(A);  /* 0004C1 PHA */
    Y = 0x0010;  /* 0004C2 LDY */
    S -= 3; F_00050E();  /* 0004C5 JSRL */
    Y = 0x0003;  /* 0004C9 LDY */
    S -= 3; F_00050E();  /* 0004CC JSRL */
    Y = 0x0000;  /* 0004D0 LDY */
    S -= 3; F_00050E();  /* 0004D3 JSRL */
    Y = 0x0000;  /* 0004D7 LDY */
    S -= 3; F_00050E();  /* 0004DA JSRL */
    Y = pull16();  /* 0004DE PLY */
    S += 3; return;  /* 0004DF RTL */
}

/* 0004E0  callers: 00019A, 0001D9, 00023F, 0003EF, 0003F8, 000409, 00041F, 000437 ... */
void F_0004E0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0004E0 LDA */
    push16(D);  /* 0004E2 PHD */
    D = A;  /* 0004E3 TAD */
    A += 1;  /* 0004E4 INC */
    push16(A);  /* 0004E5 PHA */
    wr16(DP(0x00), Y);  /* 0004E6 STY */
    S -= 3; F_0004A5();  /* 0004E8 JSRL */
    Y = 0x0002;  /* 0004EC LDY */
    S -= 3; F_00050E();  /* 0004EF JSRL */
    Y = rd16(DP(0x00));  /* 0004F3 LDY */
    S -= 3; F_000486();  /* 0004F5 JSRL */
    Y = 0x0001;  /* 0004F9 LDY */
    S -= 3; F_000486();  /* 0004FC JSRL */
    Y = 0x0000;  /* 000500 LDY */
    S -= 3; F_00050E();  /* 000503 JSRL */
    S -= 3; F_0004BF();  /* 000507 JSRL */
    Y = pull16();  /* 00050B PLY */
    D = pull16();  /* 00050C PLD */
    S += 3; return;  /* 00050D RTL */
}

/* 00050E  callers: 00024A, 000257, 00025E, 000466, 00048E, 00049D, 0004AB, 0004B2 ... */
void F_00050E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00050E LDA */
    push16(D);  /* 000510 PHD */
    D = A;  /* 000511 TAD */
    wr16(DP(0x00), Y);  /* 000512 STY */
    A = Y;  /* 000514 TYA */
    mflag = 1;  /* 000515 SEM */
    wr8(ABS(U0TB), (uint8_t)A);  /* 000516 STA */
L_000519: TICK();
    wr8(ABS(WDT), 0x00);  /* 000519 LDM */
L_00051D: TICK();
    if ((rd8(ABS(U0C1)) & 0x02) == 0x02) goto L_000524;  /* 00051D BBS */
    goto L_00051D;  /* 000522 BRA */
L_000524: TICK();
    mflag = 1;  /* 000524 SEM */
    if ((rd8(ABS(U0C0)) & 0x08) == 0) goto L_000519;  /* 000525 BBC */
    mflag = 0;  /* 00052A CLM */
    D = pull16();  /* 00052B PLD */
    S += 3; return;  /* 00052C RTL */
}

/* 00052D  callers: 00009C, 00042E */
void F_00052D(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00052D SEM */
    wr8(ABS(0x5555), 0xAA);  /* 00052E LDM */
    wr8(ABS(0x2AAA), 0x55);  /* 000532 LDM */
    wr8(ABS(0x5555), 0x80);  /* 000536 LDM */
    wr8(ABS(0x5555), 0xAA);  /* 00053A LDM */
    wr8(ABS(0x2AAA), 0x55);  /* 00053E LDM */
    wr8(ABS(0x5555), 0x10);  /* 000542 LDM */
    mflag = 0;  /* 000546 CLM */
    S += 3; return;  /* 000547 RTL */
}

