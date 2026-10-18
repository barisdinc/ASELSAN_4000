/* runtime.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 01E006  callers: 00E539 */
void F_01E006(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E011;  /* 01E006 BBC */
    mflag = 1;  /* 01E00C SEM */
    wr8(ABS(0x0341), rd8(ABS(0x0341)) & ~0x80);  /* 01E00D CLB */
L_01E011: TICK();
    mflag = 0;  /* 01E011 CLM */
    S += 3; return;  /* 01E012 RTL */
}

/* 01E013  callers: 01A75C */
void F_01E013(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01E013 LDA */
    push16(D);  /* 01E015 PHD */
    D = A;  /* 01E016 TAD */
    A += 1;  /* 01E017 INC */
    A += 1;  /* 01E018 INC */
    push16(A);  /* 01E019 PHA */
    mflag = 1;  /* 01E01A SEM */
    wr8(ABS(WDT), 0x00);  /* 01E01B LDM */
    mflag = 0;  /* 01E01F CLM */
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E086;  /* 01E020 BBC */
    mflag = 1;  /* 01E026 SEM */
    wr8(ABS(0x033F), rd8(ABS(0x033F)) | 0x04);  /* 01E027 SEB */
    if ((rd8(ABS(0x033F)) & 0x02) == 0) goto L_01E04B;  /* 01E02B BBC */
    mflag = 0;  /* 01E030 CLM */
    wr16(DP(0x00), 0x0000);  /* 01E031 LDM */
L_01E035: TICK();
    X = rd16(DP(0x00));  /* 01E035 LDX */
    C = (X >= 0x0003);  /* 01E037 CPX */
    if (C) goto L_01E048;  /* 01E03A BCS */
    mflag = 1;  /* 01E03C SEM */
    wr8((ABS(0x033C) + X), 0xFF);  /* 01E03D LDM */
    X += 1;  /* 01E041 INX */
    wr16(DP(0x00), X);  /* 01E042 STX */
    mflag = 0;  /* 01E044 CLM */
    goto L_01E035;  /* 01E045 JMP */
L_01E048: TICK();
    goto L_01E086;  /* 01E048 JMP */
L_01E04B: TICK();
    mflag = 1;  /* 01E04B SEM */
    wr8(ABS(0x0341), rd8(ABS(0x0341)) | 0x80);  /* 01E04C SEB */
    wr8(ABS(0x0342), rd8(ABS(0x0342)) & ~0xF0);  /* 01E050 CLB */
    wr8(ABS(0x0344), rd8(ABS(0x0344)) & ~0xF0);  /* 01E054 CLB */
    mflag = 0;  /* 01E058 CLM */
    wr16(DP(0x00), 0x0000);  /* 01E059 LDM */
L_01E05D: TICK();
    X = rd16(DP(0x00));  /* 01E05D LDX */
    /* 01E05F CPX */
    if (X >= 0x0003) goto L_01E072;  /* 01E062 BCS */
    A = rd16((ABS(0x0339) + X));  /* 01E064 LDA */
    mflag = 1;  /* 01E067 SEM */
    wr8((ABS(0x0336) + X), (uint8_t)A);  /* 01E068 STA */
    X += 1;  /* 01E06B INX */
    wr16(DP(0x00), X);  /* 01E06C STX */
    mflag = 0;  /* 01E06E CLM */
    goto L_01E05D;  /* 01E06F JMP */
L_01E072: TICK();
    mflag = 0;  /* 01E072 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x2000);  /* 01E073 CLB */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 01E078 SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0080);  /* 01E07D SEB */
    S -= 3; F_01E110();  /* 01E082 JSRL */
L_01E086: TICK();
    mflag = 0;  /* 01E086 CLM */
    Y = pull16();  /* 01E087 PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    D = pull16();  /* 01E088 PLD */
    S += 3; return;  /* 01E089 RTL */
}

/* 01E08A  callers: 00651B */
void F_01E08A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01E08A LDA */
    push16(D);  /* 01E08C PHD */
    D = A;  /* 01E08D TAD */
    A += 1;  /* 01E08E INC */
    push16(A);  /* 01E08F PHA */
    wr16(DP(0x00), Y);  /* 01E090 STY */
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E0C2;  /* 01E092 BBC */
    mflag = 1;  /* 01E098 SEM */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_01E0C2;  /* 01E099 BBS */
    mflag = 0;  /* 01E09E CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0080);  /* 01E09F SEB */
    A = Y;  /* 01E0A4 TYA */
    mflag = 1;  /* 01E0A5 SEM */
    /* 01E0A6 CMP */
    if ((A & 0xFF) == 0x04) goto L_01E0AF;  /* 01E0A8 BEQ */
    SETLO(A, Y);  /* 01E0AA TYA */
    /* 01E0AB CMP */
    if ((A & 0xFF) != 0x03) goto L_01E0B8;  /* 01E0AD BNE */
L_01E0AF: TICK();
    mflag = 0;  /* 01E0AF CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 01E0B0 CLB */
    goto L_01E0BE;  /* 01E0B5 JMP */
L_01E0B8: TICK();
    mflag = 0;  /* 01E0B8 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 01E0B9 SEB */
L_01E0BE: TICK();
    S -= 3; F_00CB52();  /* 01E0BE JSRL */
L_01E0C2: TICK();
    mflag = 0;  /* 01E0C2 CLM */
    Y = pull16();  /* 01E0C3 PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    D = pull16();  /* 01E0C4 PLD */
    S += 3; return;  /* 01E0C5 RTL */
}

/* 01E0C6  callers: 005BB5 */
void F_01E0C6(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E0D9;  /* 01E0C6 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0008);  /* 01E0CC SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0008);  /* 01E0D1 SEB */
    goto L_01E0DE;  /* 01E0D6 JMP */
L_01E0D9: TICK();
    mflag = 0;  /* 01E0D9 CLM */
    A = 0x0000;  /* 01E0DA LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    S += 3; return;  /* 01E0DD RTL */
L_01E0DE: TICK();
    A = 0x0001;  /* 01E0DE LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    S += 3; return;  /* 01E0E1 RTL */
}

/* 01E0E2  callers: 00F58C */
void F_01E0E2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01E0E2 LDA */
    push16(A);  /* 01E0E4 PHA */
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E104;  /* 01E0E5 BBC */
    Y = 0x0001;  /* 01E0EB LDY */
    S -= 3; F_007809();  /* 01E0EE JSRL */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0008);  /* 01E0F2 CLB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0008);  /* 01E0F7 CLB */
    mflag = 1;  /* 01E0FC SEM */
    wr8(ABS(0x0228), 0x0A);  /* 01E0FD LDM */
    goto L_01E10A;  /* 01E101 JMP */
L_01E104: TICK();
    mflag = 0;  /* 01E104 CLM */
    A = 0x0000;  /* 01E105 LDA */
    Y = pull16();  /* 01E108 PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    S += 3; return;  /* 01E109 RTL */
L_01E10A: TICK();
    mflag = 0;  /* 01E10A CLM */
    A = 0x0001;  /* 01E10B LDA */
    Y = pull16();  /* 01E10E PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    S += 3; return;  /* 01E10F RTL */
}

/* 01E110  callers: 005005, 00928A, 01E082 */
void F_01E110(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E138;  /* 01E110 BBC */
    mflag = 1;  /* 01E116 SEM */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_01E138;  /* 01E117 BBS */
    mflag = 0;  /* 01E11C CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0002);  /* 01E11D CLB */
    mflag = 1;  /* 01E122 SEM */
    if ((rd8(ABS(0x033F)) & 0x40) == 0) goto L_01E135;  /* 01E123 BBC */
    SETLO(A, rd8(ABS(0x0375)));  /* 01E128 LDA */
    SETLO(A, A & 0x3F);  /* 01E12B AND */
    if ((A & 0xFF) == 0) goto L_01E135;  /* 01E12D BEQ */
    mflag = 0;  /* 01E12F CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 01E130 SEB */
L_01E135: TICK();
    goto L_01E13D;  /* 01E135 JMP */
L_01E138: TICK();
    mflag = 0;  /* 01E138 CLM */
    A = 0x0000;  /* 01E139 LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    S += 3; return;  /* 01E13C RTL */
L_01E13D: TICK();
    mflag = 0;  /* 01E13D CLM */
    A = 0x0001;  /* 01E13E LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    S += 3; return;  /* 01E141 RTL */
}

/* 01E142  callers: 004AC3 */
void F_01E142(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01E142 LDA */
    push16(A);  /* 01E144 PHA */
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E16C;  /* 01E145 BBC */
    mflag = 1;  /* 01E14B SEM */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_01E16C;  /* 01E14C BBS */
    wr8(ABS(0x0293), 0x00);  /* 01E151 LDM */
    Y = 0x002C;  /* 01E155 LDY */
    mflag = 0;  /* 01E158 CLM */
    S -= 3; F_003310();  /* 01E159 JSRL */
    Y = 0x000D;  /* 01E15D LDY */
    S -= 3; F_0080AA();  /* 01E160 JSRL */
    mflag = 1;  /* 01E164 SEM */
    wr8(ABS(0x0230), 0x00);  /* 01E165 LDM */
    goto L_01E172;  /* 01E169 JMP */
L_01E16C: TICK();
    mflag = 0;  /* 01E16C CLM */
    A = 0x0000;  /* 01E16D LDA */
    Y = pull16();  /* 01E170 PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    S += 3; return;  /* 01E171 RTL */
L_01E172: TICK();
    mflag = 0;  /* 01E172 CLM */
    A = 0x0001;  /* 01E173 LDA */
    Y = pull16();  /* 01E176 PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    S += 3; return;  /* 01E177 RTL */
}

/* 01E178  callers: 005F6A */
void F_01E178(void)
{
    uint32_t t; (void)t;
    if ((rd16(ABS(0x0246)) & 0x0100) == 0) goto L_01E19C;  /* 01E178 BBC */
    mflag = 1;  /* 01E17E SEM */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_01E19C;  /* 01E17F BBS */
    mflag = 0;  /* 01E184 CLM */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_01E193;  /* 01E185 BBC */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0080);  /* 01E18B SEB */
    goto L_01E199;  /* 01E190 JMP */
L_01E193: TICK();
    mflag = 0;  /* 01E193 CLM */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0080);  /* 01E194 CLB */
L_01E199: TICK();
    goto L_01E1A1;  /* 01E199 JMP */
L_01E19C: TICK();
    mflag = 0;  /* 01E19C CLM */
    A = 0x0000;  /* 01E19D LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    S += 3; return;  /* 01E1A0 RTL */
L_01E1A1: TICK();
    A = 0x0001;  /* 01E1A1 LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    S += 3; return;  /* 01E1A4 RTL */
}

/* 01E1A6  callers: 00624B, 008108, 0087F7, 008925, 00894B, 00896C, 00A0C9, 019439 */
void rt_ldiv_01E1A6(void)
{
    uint32_t t; (void)t;
    push16(A);  /* 01E1A6 PHA */
    push16(B);  /* 01E1A7 PHB */
    B ^= rd16(DP(0x02 + X));  /* 01E1A9 EOR */
    wr16(SR(0x03), B);  /* 01E1AC STA */
    push16(A);  /* 01E1AF PHA */
    A = rd16(SR(0x03));  /* 01E1B0 LDA */
    if ((A & 0x8000) == 0) goto L_01E1C8;  /* 01E1B2 BPL */
    A ^= 0xFFFF;  /* 01E1B4 EOR */
    wr16(SR(0x03), A);  /* 01E1B7 STA */
    A = rd16(SR(0x01));  /* 01E1B9 LDA */
    A ^= 0xFFFF;  /* 01E1BB EOR */
    A += 1;  /* 01E1BE INC */
    Z = (A == 0);
    wr16(SR(0x01), A);  /* 01E1BF STA */
    if (!Z) goto L_01E1C8;  /* 01E1C1 BNE */
    A = rd16(SR(0x03));  /* 01E1C3 LDA */
    A += 1;  /* 01E1C5 INC */
    wr16(SR(0x03), A);  /* 01E1C6 STA */
L_01E1C8: TICK();
    A = rd16(DP(0x02 + X));  /* 01E1C8 LDA */
    if ((A & 0x8000) == 0) goto L_01E1DD;  /* 01E1CA BPL */
    A ^= 0xFFFF;  /* 01E1CC EOR */
    wr16(DP(0x02 + X), A);  /* 01E1CF STA */
    A = rd16(DP(0x00 + X));  /* 01E1D1 LDA */
    A ^= 0xFFFF;  /* 01E1D3 EOR */
    A += 1;  /* 01E1D6 INC */
    Z = (A == 0);
    wr16(DP(0x00 + X), A);  /* 01E1D7 STA */
    if (!Z) goto L_01E1DD;  /* 01E1D9 BNE */
    t = (rd16(DP(0x02 + X)) + 1) & 0xFFFF; wr16(DP(0x02 + X), t);  /* 01E1DB INC */
L_01E1DD: TICK();
    A = rd16(DP(0x02 + X));  /* 01E1DD LDA */
    if (A != 0) goto L_01E1FC;  /* 01E1DF BNE */
    B = rd16(SR(0x03));  /* 01E1E1 LDA */
    t = rd16(DP(0x00 + X));  /* 01E1E4 CMP */
    C = (B >= t);
    if (C) goto L_01E1FC;  /* 01E1E7 BCS */
    A = rd16(SR(0x01));  /* 01E1E9 LDA */
    div16(rd16(DP(0x00 + X)));  /* 01E1EB DIV */
    B = 0x0000;  /* 01E1EE LDA */
    X = pull16();  /* 01E1F2 PLX */
    X = pull16();  /* 01E1F3 PLX */
    X = pull16();  /* 01E1F4 PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!N) goto L_01E1FB;  /* 01E1F5 BPL */
    goto L_01E4D2;  /* 01E1F7 JMPL */
L_01E1FB: TICK();
    S += 3; return;  /* 01E1FB RTL */
L_01E1FC: TICK();
    A = rd16(DP(0x02 + X));  /* 01E1FC LDA */
    push16(A);  /* 01E1FE PHA */
    A = rd16(DP(0x00 + X));  /* 01E1FF LDA */
    push16(A);  /* 01E201 PHA */
    S -= 3; rt_ldiv_core_01E2C6();  /* 01E202 JSRL */
    A = pull16();  /* 01E206 PLA */
    B = pull16();  /* 01E207 PLB */
    X = pull16();  /* 01E209 PLX */
    X = pull16();  /* 01E20A PLX */
    X = pull16();  /* 01E20B PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!N) goto L_01E212;  /* 01E20C BPL */
    goto L_01E4D2;  /* 01E20E JMPL */
L_01E212: TICK();
    S += 3; return;  /* 01E212 RTL */
L_01E4D2: TICK();
    A ^= 0xFFFF;  /* 01E4D2 EOR */
    B ^= 0xFFFF;  /* 01E4D5 EOR */
    A += 1;  /* 01E4D9 INC */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    if (!Z) goto L_01E4DE;  /* 01E4DA BNE */
    B += 1;  /* 01E4DC INC */
    N = ((B & 0x8000) != 0);
    Z = (B == 0);
L_01E4DE: TICK();
    S += 3; return;  /* 01E4DE RTL */
}

/* 01E214  callers: 006148, 0061B8, 00883E */
void rt_lmod_01E214(void)
{
    uint32_t t; (void)t;
    push16(B);  /* 01E214 PHB */
    push16(B);  /* 01E216 PHB */
    push16(A);  /* 01E218 PHA */
    A = rd16(SR(0x03));  /* 01E219 LDA */
    if ((A & 0x8000) == 0) goto L_01E231;  /* 01E21B BPL */
    A ^= 0xFFFF;  /* 01E21D EOR */
    wr16(SR(0x03), A);  /* 01E220 STA */
    A = rd16(SR(0x01));  /* 01E222 LDA */
    A ^= 0xFFFF;  /* 01E224 EOR */
    A += 1;  /* 01E227 INC */
    Z = (A == 0);
    wr16(SR(0x01), A);  /* 01E228 STA */
    if (!Z) goto L_01E231;  /* 01E22A BNE */
    A = rd16(SR(0x03));  /* 01E22C LDA */
    A += 1;  /* 01E22E INC */
    wr16(SR(0x03), A);  /* 01E22F STA */
L_01E231: TICK();
    A = rd16(DP(0x02 + X));  /* 01E231 LDA */
    if ((A & 0x8000) == 0) goto L_01E246;  /* 01E233 BPL */
    A ^= 0xFFFF;  /* 01E235 EOR */
    wr16(DP(0x02 + X), A);  /* 01E238 STA */
    A = rd16(DP(0x00 + X));  /* 01E23A LDA */
    A ^= 0xFFFF;  /* 01E23C EOR */
    A += 1;  /* 01E23F INC */
    Z = (A == 0);
    wr16(DP(0x00 + X), A);  /* 01E240 STA */
    if (!Z) goto L_01E246;  /* 01E242 BNE */
    t = (rd16(DP(0x02 + X)) + 1) & 0xFFFF; wr16(DP(0x02 + X), t);  /* 01E244 INC */
L_01E246: TICK();
    B = rd16(DP(0x02 + X));  /* 01E246 LDA */
    if (B != 0) goto L_01E267;  /* 01E249 BNE */
    B = rd16(SR(0x03));  /* 01E24B LDA */
    t = rd16(DP(0x00 + X));  /* 01E24E CMP */
    C = (B >= t);
    if (C) goto L_01E267;  /* 01E251 BCS */
    A = rd16(SR(0x01));  /* 01E253 LDA */
    div16(rd16(DP(0x00 + X)));  /* 01E255 DIV */
    A = 0x0000;  /* 01E258 LDA */
    t = A; A = B; B = t;  /* 01E25B XAB */
    X = pull16();  /* 01E25D PLX */
    X = pull16();  /* 01E25E PLX */
    X = pull16();  /* 01E25F PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!N) goto L_01E266;  /* 01E260 BPL */
    goto L_01E4D2;  /* 01E262 JMPL */
L_01E266: TICK();
    S += 3; return;  /* 01E266 RTL */
L_01E267: TICK();
    A = rd16(DP(0x02 + X));  /* 01E267 LDA */
    push16(A);  /* 01E269 PHA */
    A = rd16(DP(0x00 + X));  /* 01E26A LDA */
    push16(A);  /* 01E26C PHA */
    S -= 3; rt_ldiv_core_01E2C6();  /* 01E26D JSRL */
    X = pull16();  /* 01E271 PLX */
    X = pull16();  /* 01E272 PLX */
    X = pull16();  /* 01E273 PLX */
    X = pull16();  /* 01E274 PLX */
    X = pull16();  /* 01E275 PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!N) goto L_01E266;  /* 01E276 BPL */
    goto L_01E4D2;  /* 01E278 JMPL */
L_01E4D2: TICK();
    A ^= 0xFFFF;  /* 01E4D2 EOR */
    B ^= 0xFFFF;  /* 01E4D5 EOR */
    A += 1;  /* 01E4D9 INC */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    if (!Z) goto L_01E4DE;  /* 01E4DA BNE */
    B += 1;  /* 01E4DC INC */
    N = ((B & 0x8000) != 0);
    Z = (B == 0);
L_01E4DE: TICK();
    S += 3; return;  /* 01E4DE RTL */
}

/* 01E2C6  callers: 01E202, 01E26D, 01E434, 01E45F */
void rt_ldiv_core_01E2C6(void)
{
    uint32_t t; (void)t;
    push16(Y);  /* 01E2C6 PHY */
    Y = 0x0020;  /* 01E2C7 LDY */
    A = rd16(DP(0x00 + X));  /* 01E2CA LDA */
    push16(A);  /* 01E2CC PHA */
    A = rd16(DP(0x02 + X));  /* 01E2CD LDA */
    push16(A);  /* 01E2CF PHA */
    A = rd16(SR(0x0E));  /* 01E2D0 LDA */
    wr16(DP(0x00 + X), A);  /* 01E2D2 STA */
    A = rd16(SR(0x10));  /* 01E2D4 LDA */
    wr16(DP(0x02 + X), A);  /* 01E2D6 STA */
    B = 0x0000;  /* 01E2D8 LDA */
    A = 0x0000;  /* 01E2DC LDA */
L_01E2DF: TICK();
    wr16(DP(0x00 + X), rol16(rd16(DP(0x00 + X))));  /* 01E2DF ROL */
    wr16(DP(0x02 + X), rol16(rd16(DP(0x02 + X))));  /* 01E2E1 ROL */
    A = rol16(A);  /* 01E2E3 ROL */
    B = rol16(B);  /* 01E2E4 ROL */
    C = 1;  /* 01E2E6 SEC */
    A = sbc16(A, rd16(SR(0x0A)));  /* 01E2E7 SBC */
    B = sbc16(B, rd16(SR(0x0C)));  /* 01E2E9 SBC */
    if (C) goto L_01E2F4;  /* 01E2EC BCS */
    A = adc16(A, rd16(SR(0x0A)));  /* 01E2EE ADC */
    B = adc16(B, rd16(SR(0x0C)));  /* 01E2F0 ADC */
    C = 0;  /* 01E2F3 CLC */
L_01E2F4: TICK();
    Y -= 1;  /* 01E2F4 DEY */
    if (Y != 0) goto L_01E2DF;  /* 01E2F5 BNE */
    push16(A);  /* 01E2F7 PHA */
    A = rd16(DP(0x00 + X));  /* 01E2F8 LDA */
    A = rol16(A);  /* 01E2FA ROL */
    wr16(SR(0x0C), A);  /* 01E2FB STA */
    A = rd16(DP(0x02 + X));  /* 01E2FD LDA */
    A = rol16(A);  /* 01E2FF ROL */
    wr16(SR(0x0E), A);  /* 01E300 STA */
    A = pull16();  /* 01E302 PLA */
    Y = pull16();  /* 01E303 PLY */
    wr16(DP(0x02 + X), Y);  /* 01E304 STY */
    Y = pull16();  /* 01E306 PLY */
    wr16(DP(0x00 + X), Y);  /* 01E307 STY */
    Y = pull16();  /* 01E309 PLY */
    N = ((Y & 0x8000) != 0);
    Z = (Y == 0);
    S += 3; return;  /* 01E30A RTL */
}

/* 01E30B  callers: 00420C, 007B98, 007BFE, 008C40, 008CAC, 009214, 00D6C8, 00D74B ... */
void F_01E30B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 01E30B LDA */
    push16(D);  /* 01E30D PHD */
    D = A;  /* 01E30E TAD */
    A = rd16(DP(0x02));  /* 01E30F LDA */
    wr16(DP(0x04), Y);  /* 01E311 STY */
    mflag = 1;  /* 01E313 SEM */
    wr8(DP(0x06), (uint8_t)B);  /* 01E314 STA */
    mflag = 0;  /* 01E317 CLM */
    B = rd16(DP(0x00));  /* 01E318 LDA */
    S -= 3; F_00FF8C();  /* 01E31B JSRL */
    wr16(DP(0x07), A);  /* 01E31F STA */
    mpy16(rd16(DP(0x00)));  /* 01E321 MPY */
    A ^= 0xFFFF;  /* 01E324 EOR */
    A += 1;  /* 01E327 INC */
    C = 0;  /* 01E328 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 01E329 ADC */
    wr16(DP(0x09), A);  /* 01E32B STA */
    A = D;  /* 01E32D TDA */
    C = 0;  /* 01E32E CLC */
    A = adc16(A, 0x0007);  /* 01E32F ADC */
    wr16(DP(0x0B), A);  /* 01E332 STA */
    wr16(DP(0x0D), A);  /* 01E334 STA */
    mflag = 1;  /* 01E336 SEM */
    wr8(DP(0x0F), 0x00);  /* 01E337 LDM */
    mflag = 0;  /* 01E33A CLM */
    A = Y;  /* 01E33B TYA */
    wr16(DP(0x10), A);  /* 01E33C STA */
    A = rd16(DP(0x06));  /* 01E33E LDA */
    mflag = 1;  /* 01E340 SEM */
    wr8(DP(0x12), (uint8_t)A);  /* 01E341 STA */
    mflag = 0;  /* 01E343 CLM */
    A = 0x000D;  /* 01E344 LDA */
    Y = 0x0004;  /* 01E347 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 01E34A JSRL */
    D = pull16();  /* 01E34E PLD */
    S += 3; return;  /* 01E34F RTL */
}

/* 01E350  callers: 006290, 0078BE, 014085, 014F8C, 01CD52, 01CF4D */
void F_01E350(void)
{
    uint32_t t; (void)t;
    push16(X);  /* 01E350 PHX */
    X = S;  /* 01E351 TSX */
    push16(A);  /* 01E352 PHA */
    push16(B);  /* 01E353 PHB */
    B = 0x0000;  /* 01E355 LDA */
    A |= rd16(SR(0x01));  /* 01E359 ORA */
    if ((A & 0x8000) != 0) goto L_01E367;  /* 01E35B BMI */
    A = rd16(SR(0x03));  /* 01E35D LDA */
    div16(rd16(SR(0x01)));  /* 01E35F DIV */
L_01E362: TICK();
    t = A; A = B; B = t;  /* 01E362 XAB */
L_01E364: TICK();
    S = X;  /* 01E364 TXS */
    X = pull16();  /* 01E365 PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    S += 3; return;  /* 01E366 RTL */
L_01E367: TICK();
    A = rd16(SR(0x01));  /* 01E367 LDA */
    if ((A & 0x8000) == 0) goto L_01E371;  /* 01E369 BPL */
    A ^= 0xFFFF;  /* 01E36B EOR */
    A += 1;  /* 01E36E INC */
    wr16(SR(0x01), A);  /* 01E36F STA */
L_01E371: TICK();
    A = rd16(SR(0x03));  /* 01E371 LDA */
    if ((A & 0x8000) == 0) goto L_01E379;  /* 01E373 BPL */
    A ^= 0xFFFF;  /* 01E375 EOR */
    A += 1;  /* 01E378 INC */
L_01E379: TICK();
    div16(rd16(SR(0x01)));  /* 01E379 DIV */
    A = rd16(SR(0x03));  /* 01E37C LDA */
    if ((A & 0x8000) == 0) goto L_01E362;  /* 01E37E BPL */
    t = A; A = B; B = t;  /* 01E380 XAB */
    A ^= 0xFFFF;  /* 01E382 EOR */
    A += 1;  /* 01E385 INC */
    goto L_01E364;  /* 01E386 BRA */
}

/* 01E388  callers: 00FECC, 00FF08 */
void rt_far_memcpy_init_01E388(void)
{
    uint32_t t; (void)t;
    push16(D);  /* 01E388 PHD */
    C = 0;  /* 01E389 CLC */
    A = adc16(A, rd16(SR(0x01)));  /* 01E38A ADC */
    D = A;  /* 01E38C TAD */
    A = rd16(DP(0x05));  /* 01E38D LDA */
    push16(A);  /* 01E38F PHA */
    A = rd16(DP(0x03));  /* 01E390 LDA */
    push16(A);  /* 01E392 PHA */
L_01E393: TICK();
    mflag = 1;  /* 01E393 SEM */
    SETLO(A, rd8(DP(0x08)));  /* 01E394 LDA */
    Z = ((A & 0xFF) == 0);
    mflag = 0;  /* 01E396 CLM */
    if (Z) goto L_01E3AC;  /* 01E397 BEQ */
    Y = 0x0000;  /* 01E399 LDY */
L_01E39C: TICK();
    A = rd16((LIND(0x00) + Y));  /* 01E39C LDA */
    wr16((LIND(0x02) + Y), A);  /* 01E39E STA */
    Y += 1;  /* 01E3A0 INY */
    Y += 1;  /* 01E3A1 INY */
    if (Y != 0) goto L_01E39C;  /* 01E3A2 BNE */
    t = (rd16(DP(0x08)) - 1) & 0xFFFF; wr16(DP(0x08), t);  /* 01E3A4 DEC */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 01E3A6 INC */
    t = (rd16(DP(0x05)) + 1) & 0xFFFF; wr16(DP(0x05), t);  /* 01E3A8 INC */
    goto L_01E393;  /* 01E3AA BRA */
L_01E3AC: TICK();
    Y = rd16(DP(0x06));  /* 01E3AC LDY */
    A = 0x0000;  /* 01E3AE LDA */
    S -= 3; rt_far_memcpy_01E3BA();  /* 01E3B1 JSRL */
    A = pull16();  /* 01E3B5 PLA */
    B = pull16();  /* 01E3B6 PLB */
    N = ((B & 0x8000) != 0);
    Z = (B == 0);
    D = pull16();  /* 01E3B8 PLD */
    S += 3; return;  /* 01E3B9 RTL */
}

/* 01E3BA  callers: 003DE8, 003F12, 00403B, 004166, 004235, 0045C0, 00776B, 0077EA ... */
void rt_far_memcpy_01E3BA(void)
{
    uint32_t t; (void)t;
    push16(D);  /* 01E3BA PHD */
    C = 0;  /* 01E3BB CLC */
    A = adc16(A, rd16(SR(0x01)));  /* 01E3BC ADC */
    D = A;  /* 01E3BE TAD */
    A = Y;  /* 01E3BF TYA */
    A = lsr16(A);  /* 01E3C0 LSR */
    if (!C) goto L_01E3CA;  /* 01E3C1 BCC */
    Y -= 1;  /* 01E3C3 DEY */
    mflag = 1;  /* 01E3C4 SEM */
    SETLO(A, rd8((LIND(0x00) + Y)));  /* 01E3C5 LDA */
    wr8((LIND(0x03) + Y), (uint8_t)A);  /* 01E3C7 STA */
    mflag = 0;  /* 01E3C9 CLM */
L_01E3CA: TICK();
    C = (Y >= 0x0000);  /* 01E3CA CPY */
    Z = (Y == 0x0000);
    if (!Z) goto L_01E3DE;  /* 01E3CD BNE */
L_01E3CF: TICK();
    A = rd16(DP(0x03));  /* 01E3CF LDA */
    B = rd16(DP(0x05));  /* 01E3D1 LDA */
    B &= 0x00FF;  /* 01E3D4 AND */
    N = ((B & 0x8000) != 0);
    Z = (B == 0);
    D = pull16();  /* 01E3D8 PLD */
    S += 3; return;  /* 01E3D9 RTL */
L_01E3DA: TICK();
    A = rd16((LIND(0x00) + Y));  /* 01E3DA LDA */
    wr16((LIND(0x03) + Y), A);  /* 01E3DC STA */
L_01E3DE: TICK();
    Y -= 1;  /* 01E3DE DEY */
    Y -= 1;  /* 01E3DF DEY */
    if (Y != 0) goto L_01E3DA;  /* 01E3E0 BNE */
    A = rd16(LIND(0x00));  /* 01E3E2 LDA */
    wr16(LIND(0x03), A);  /* 01E3E4 STA */
    goto L_01E3CF;  /* 01E3E6 JMP */
}

/* 01E3EA  callers: 003EA6, 003EBB, 003ED0, 0040E2, 004100, 00411E, 006141, 0061B1 ... */
void rt_lmul_01E3EA(void)
{
    uint32_t t; (void)t;
    push16(X);  /* 01E3EA PHX */
    push16(B);  /* 01E3EB PHB */
    push16(A);  /* 01E3ED PHA */
    mpy16(rd16(DP(0x00 + X)));  /* 01E3EE MPY */
    push16(B);  /* 01E3F1 PHB */
    push16(A);  /* 01E3F3 PHA */
    A = rd16(SR(0x05));  /* 01E3F4 LDA */
    if (A == 0) goto L_01E400;  /* 01E3F6 BEQ */
    mpy16(rd16(DP(0x02 + X)));  /* 01E3F8 MPY */
    C = 0;  /* 01E3FB CLC */
    A = adc16(A, rd16(SR(0x03)));  /* 01E3FC ADC */
    wr16(SR(0x03), A);  /* 01E3FE STA */
L_01E400: TICK();
    A = rd16(SR(0x07));  /* 01E400 LDA */
    if (A == 0) goto L_01E40C;  /* 01E402 BEQ */
    mpy16(rd16(DP(0x00 + X)));  /* 01E404 MPY */
    C = 0;  /* 01E407 CLC */
    A = adc16(A, rd16(SR(0x03)));  /* 01E408 ADC */
    wr16(SR(0x03), A);  /* 01E40A STA */
L_01E40C: TICK();
    A = pull16();  /* 01E40C PLA */
    B = pull16();  /* 01E40D PLB */
    X = pull16();  /* 01E40F PLX */
    X = pull16();  /* 01E410 PLX */
    X = pull16();  /* 01E411 PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    S += 3; return;  /* 01E412 RTL */
}

/* 01E414  callers: 00770C, 00778E, 0077B7, 00886E, 01D771 */
void rt_ludiv_01E414(void)
{
    uint32_t t; (void)t;
    push16(B);  /* 01E414 PHB */
    push16(A);  /* 01E416 PHA */
    A = rd16(DP(0x02 + X));  /* 01E417 LDA */
    Z = (A == 0);
    push16(A);  /* 01E419 PHA */
    if (!Z) goto L_01E431;  /* 01E41A BNE */
    B = rd16(SR(0x05));  /* 01E41C LDA */
    t = rd16(DP(0x00 + X));  /* 01E41F CMP */
    C = (B >= t);
    if (C) goto L_01E431;  /* 01E422 BCS */
    A = rd16(SR(0x03));  /* 01E424 LDA */
    div16(rd16(DP(0x00 + X)));  /* 01E426 DIV */
    B = 0x0000;  /* 01E429 LDA */
    X = pull16();  /* 01E42D PLX */
L_01E42E: TICK();
    X = pull16();  /* 01E42E PLX */
    X = pull16();  /* 01E42F PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    S += 3; return;  /* 01E430 RTL */
L_01E431: TICK();
    A = rd16(DP(0x00 + X));  /* 01E431 LDA */
    push16(A);  /* 01E433 PHA */
    S -= 3; rt_ldiv_core_01E2C6();  /* 01E434 JSRL */
    A = pull16();  /* 01E438 PLA */
    B = pull16();  /* 01E439 PLB */
    goto L_01E42E;  /* 01E43B BRA */
}

/* 01E43E  callers: 0040B7, 0076F6, 00779D, 0077C6, 01D74A */
void rt_lumod_01E43E(void)
{
    uint32_t t; (void)t;
    push16(B);  /* 01E43E PHB */
    push16(A);  /* 01E440 PHA */
    A = rd16(DP(0x02 + X));  /* 01E441 LDA */
    Z = (A == 0);
    push16(A);  /* 01E443 PHA */
    if (!Z) goto L_01E45C;  /* 01E444 BNE */
    B = rd16(SR(0x05));  /* 01E446 LDA */
    t = rd16(DP(0x00 + X));  /* 01E449 CMP */
    C = (B >= t);
    if (C) goto L_01E45C;  /* 01E44C BCS */
    A = rd16(SR(0x03));  /* 01E44E LDA */
    div16(rd16(DP(0x00 + X)));  /* 01E450 DIV */
    A = 0x0000;  /* 01E453 LDA */
    t = A; A = B; B = t;  /* 01E456 XAB */
L_01E458: TICK();
    X = pull16();  /* 01E458 PLX */
    X = pull16();  /* 01E459 PLX */
    X = pull16();  /* 01E45A PLX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    S += 3; return;  /* 01E45B RTL */
L_01E45C: TICK();
    A = rd16(DP(0x00 + X));  /* 01E45C LDA */
    push16(A);  /* 01E45E PHA */
    S -= 3; rt_ldiv_core_01E2C6();  /* 01E45F JSRL */
    X = pull16();  /* 01E463 PLX */
    goto L_01E458;  /* 01E464 BRA */
}

/* 01E466  callers: 003529, 00353B, 003575, 0036CB, 003728, 0048D8, 0048EA, 0049A9 ... */
void rt_memcpy_01E466(void)
{
    uint32_t t; (void)t;
    push16(D);  /* 01E466 PHD */
    C = 0;  /* 01E467 CLC */
    A = adc16(A, rd16(SR(0x01)));  /* 01E468 ADC */
    D = A;  /* 01E46A TAD */
    A = Y;  /* 01E46B TYA */
    A = lsr16(A);  /* 01E46C LSR */
    if (!C) goto L_01E476;  /* 01E46D BCC */
    Y -= 1;  /* 01E46F DEY */
    mflag = 1;  /* 01E470 SEM */
    SETLO(A, rd8((IND(0x00) + Y)));  /* 01E471 LDA */
    wr8((IND(0x02) + Y), (uint8_t)A);  /* 01E473 STA */
    mflag = 0;  /* 01E475 CLM */
L_01E476: TICK();
    C = (Y >= 0x0000);  /* 01E476 CPY */
    Z = (Y == 0x0000);
    if (!Z) goto L_01E483;  /* 01E479 BNE */
L_01E47B: TICK();
    A = rd16(DP(0x02));  /* 01E47B LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    D = pull16();  /* 01E47D PLD */
    S += 3; return;  /* 01E47E RTL */
L_01E47F: TICK();
    A = rd16((IND(0x00) + Y));  /* 01E47F LDA */
    wr16((IND(0x02) + Y), A);  /* 01E481 STA */
L_01E483: TICK();
    Y -= 1;  /* 01E483 DEY */
    Y -= 1;  /* 01E484 DEY */
    if (Y != 0) goto L_01E47F;  /* 01E485 BNE */
    A = rd16(IND(0x00));  /* 01E487 LDA */
    wr16(IND(0x02), A);  /* 01E489 STA */
    goto L_01E47B;  /* 01E48B JMP */
}

/* 01E48E  callers: 0149F4 */
void rt_lsar_01E48E(void)
{
    uint32_t t; (void)t;
    /* 01E48E CMP */
    if (((A - 0x0000) & 0x8000) == 0) goto L_01E49E;  /* 01E491 BPL */
    C = (X >= 0x0000);  /* 01E493 CPX */
    N = (((X - 0x0000) & 0x8000) != 0);
    Z = (X == 0x0000);
    if (Z) goto L_01E49D;  /* 01E496 BEQ */
L_01E498: TICK();
    C = 1;  /* 01E498 SEC */
    A = ror16(A);  /* 01E499 ROR */
    X -= 1;  /* 01E49A DEX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!Z) goto L_01E498;  /* 01E49B BNE */
L_01E49D: TICK();
    S += 3; return;  /* 01E49D RTL */
L_01E49E: TICK();
    C = (X >= 0x0000);  /* 01E49E CPX */
    N = (((X - 0x0000) & 0x8000) != 0);
    Z = (X == 0x0000);
    if (Z) goto L_01E49D;  /* 01E4A1 BEQ */
L_01E4A3: TICK();
    A = lsr16(A);  /* 01E4A3 LSR */
    X -= 1;  /* 01E4A4 DEX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!Z) goto L_01E4A3;  /* 01E4A5 BNE */
    S += 3; return;  /* 01E4A7 RTL */
}

/* 01E49E  callers: 00A134, 00A151, 01E491 */
void rt_lshr_01E49E(void)
{
    uint32_t t; (void)t;
    goto L_01E49E;
L_01E49D: TICK();
    S += 3; return;  /* 01E49D RTL */
L_01E49E: TICK();
    C = (X >= 0x0000);  /* 01E49E CPX */
    N = (((X - 0x0000) & 0x8000) != 0);
    Z = (X == 0x0000);
    if (Z) goto L_01E49D;  /* 01E4A1 BEQ */
L_01E4A3: TICK();
    A = lsr16(A);  /* 01E4A3 LSR */
    X -= 1;  /* 01E4A4 DEX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!Z) goto L_01E4A3;  /* 01E4A5 BNE */
    S += 3; return;  /* 01E4A7 RTL */
}

/* 01E4B7  callers: 00A223 */
void F_01E4B7(void)
{
    uint32_t t; (void)t;
    goto L_01E4B7;
L_01E4B6: TICK();
    S += 3; return;  /* 01E4B6 RTL */
L_01E4B7: TICK();
    C = (X >= 0x0000);  /* 01E4B7 CPX */
    N = (((X - 0x0000) & 0x8000) != 0);
    Z = (X == 0x0000);
    if (Z) goto L_01E4B6;  /* 01E4BA BEQ */
L_01E4BC: TICK();
    SETLO(A, lsr8(A));  /* 01E4BC LSR */
    X -= 1;  /* 01E4BD DEX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!Z) goto L_01E4BC;  /* 01E4BE BNE */
    S += 3; return;  /* 01E4C0 RTL */
}

/* 01E4C2  callers: 0057E8, 005AEE, 0067F8, 006883, 015B17, 015BA9 */
void rt_call_ptr_01E4C2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x05));  /* 01E4C2 LDA */
    X = A;  /* 01E4C4 TAX */
    A = pull16();  /* 01E4C5 PLA */
    wr16(SR(0x03), A);  /* 01E4C6 STA */
    mflag = 1;  /* 01E4C8 SEM */
    SETLO(A, pull8());  /* 01E4C9 PLA */
    wr8(SR(0x04), (uint8_t)A);  /* 01E4CA STA */
    SETLO(A, pull8());  /* 01E4CC PLA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(X);  /* 01E4CD PHX */
    push8(A);  /* 01E4CE PHA */
    mflag = 0;  /* 01E4CF CLM */
    t = rd16(SR(1)) | (uint32_t)rd8(SR(3)) << 16; S += 3;  /* RTL into the pushed function pointer */  /* 01E4D0 RTL */
    call_address(t); return;
}

/* 01E4E0  callers: 004242, 006A7D, 007BCA, 007C30, 008CE6, 00924C, 00B0A1, 00B412 ... */
void rt_shl_01E4E0(void)
{
    uint32_t t; (void)t;
    C = (X >= 0x0000);  /* 01E4E0 CPX */
    N = (((X - 0x0000) & 0x8000) != 0);
    Z = (X == 0x0000);
    if (Z) goto L_01E4E9;  /* 01E4E3 BEQ */
L_01E4E5: TICK();
    SETLO(A, asl8(A));  /* 01E4E5 ASL */
    X -= 1;  /* 01E4E6 DEX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!Z) goto L_01E4E5;  /* 01E4E7 BNE */
L_01E4E9: TICK();
    S += 3; return;  /* 01E4E9 RTL */
}

/* 01E4EA  callers: 00B04E, 00B3E5 */
void F_01E4EA(void)
{
    uint32_t t; (void)t;
    C = (X >= 0x0000);  /* 01E4EA CPX */
    N = (((X - 0x0000) & 0x8000) != 0);
    Z = (X == 0x0000);
    if (Z) goto L_01E4F3;  /* 01E4ED BEQ */
L_01E4EF: TICK();
    A = asl16(A);  /* 01E4EF ASL */
    X -= 1;  /* 01E4F0 DEX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (!Z) goto L_01E4EF;  /* 01E4F1 BNE */
L_01E4F3: TICK();
    S += 3; return;  /* 01E4F3 RTL */
}

