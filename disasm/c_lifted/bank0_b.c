/* bank0_b.c - lifted from YA4000S.HEX by tools/lift.py. Do not edit by hand;
 * cleaned-up versions live in ../c_clean/. */
#include "m7700.h"
#include "functions.h"

/* 00713E  callers: 01A720, 01B9D4 */
void i2c_init_00713E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00713E LDA */
    push16(D);  /* 007140 PHD */
    D = A;  /* 007141 TAD */
    A += 1;  /* 007142 INC */
    A += 1;  /* 007143 INC */
    push16(A);  /* 007144 PHA */
    wr16(DP(0x00), 0x000A);  /* 007145 LDM */
L_007149: TICK();
    A = rd16(DP(0x00));  /* 007149 LDA */
    t = (rd16(DP(0x00)) - 1) & 0xFFFF; wr16(DP(0x00), t);  /* 00714B DEC */
    X = A;  /* 00714D TAX */
    if (X == 0) goto L_007157;  /* 00714E BEQ */
    S -= 3; i2c_stop_00715B();  /* 007150 JSRL */
    goto L_007149;  /* 007154 JMP */
L_007157: TICK();
    mflag = 0;  /* 007157 CLM */
    Y = pull16();  /* 007158 PLY */
    D = pull16();  /* 007159 PLD */
    S += 3; return;  /* 00715A RTL */
}

/* 00715B  callers: 005116, 005130, 005192, 0051AC, 0058CD, 0058E9, 007150, 00719A ... */
void i2c_stop_00715B(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00715B SEM */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 00715C CLB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 007160 CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 007164 SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 007168 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 00716C CLB [P62 GOSTERGE_VERI(SDA) yon] */
    mflag = 0;  /* 007170 CLM */
    S += 3; return;  /* 007171 RTL */
}

/* 007172  callers: 005102, 00511A, 00517E, 005196, 0058BA, 0058D1, 00719E, 0071BB ... */
void i2c_start_007172(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 007172 SEM */
    wr8(ABS(P6D), rd8(ABS(P6D)) & ~0x04);  /* 007173 CLB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) | 0x02);  /* 007177 SEB [P61 GOSTERGE_SAAT(SCL)] */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x04);  /* 00717B CLB [P62 GOSTERGE_VERI(SDA)] */
    wr8(ABS(P6D), rd8(ABS(P6D)) | 0x04);  /* 00717F SEB [P62 GOSTERGE_VERI(SDA) yon] */
    wr8(ABS(P6), rd8(ABS(P6)) & ~0x02);  /* 007183 CLB [P61 GOSTERGE_SAAT(SCL)] */
    mflag = 0;  /* 007187 CLM */
    S += 3; return;  /* 007188 RTL */
}

/* 007189  callers: 004BCC, 0091D7, 00A63F, 00A652, 00A686, 00B9C6, 00BA1A, 00BA28 ... */
void eeprom_read_byte_007189(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007189 LDA */
    push16(D);  /* 00718B PHD */
    D = A;  /* 00718C TAD */
    A += 1;  /* 00718D INC */
    A += 1;  /* 00718E INC */
    push16(A);  /* 00718F PHA */
    mflag = 1;  /* 007190 SEM */
    SETLO(A, 0x01);  /* 007191 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007193 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007194 STA */
    setP(pull16());  /* 007196 PLP */
    wr16(DP(0x00), Y);  /* 007197 STY */
    mflag = 0;  /* 007199 CLM */
    S -= 3; i2c_stop_00715B();  /* 00719A JSRL */
    S -= 3; i2c_start_007172();  /* 00719E JSRL */
    A = rd16(DP(0x01));  /* 0071A2 LDA */
    A &= 0x00FF;  /* 0071A4 AND */
    mflag = 1;  /* 0071A7 SEM */
    SETLO(A, A & 0x07);  /* 0071A8 AND */
    SETLO(A, asl8(A));  /* 0071AA ASL */
    SETLO(A, A | 0xA0);  /* 0071AB ORA */
    mflag = 0;  /* 0071AD CLM */
    Y = A;  /* 0071AE TAY */
    S -= 3; i2c_send_byte_006A68();  /* 0071AF JSRL */
    Y = rd16(DP(0x00));  /* 0071B3 LDY */
    mflag = 0;  /* 0071B5 CLM */
    S -= 3; i2c_send_byte_006A68();  /* 0071B6 JSRL */
    mflag = 0;  /* 0071BA CLM */
    S -= 3; i2c_start_007172();  /* 0071BB JSRL */
    A = rd16(DP(0x01));  /* 0071BF LDA */
    A &= 0x00FF;  /* 0071C1 AND */
    mflag = 1;  /* 0071C4 SEM */
    SETLO(A, A & 0x07);  /* 0071C5 AND */
    SETLO(A, asl8(A));  /* 0071C7 ASL */
    SETLO(A, A | 0xA1);  /* 0071C8 ORA */
    mflag = 0;  /* 0071CA CLM */
    Y = A;  /* 0071CB TAY */
    S -= 3; i2c_send_byte_006A68();  /* 0071CC JSRL */
    Y = 0x0001;  /* 0071D0 LDY */
    mflag = 0;  /* 0071D3 CLM */
    S -= 3; i2c_recv_byte_006EB3();  /* 0071D4 JSRL */
    mflag = 0;  /* 0071D8 CLM */
    A &= 0x00FF;  /* 0071D9 AND */
    wr16(DP(0x00), A);  /* 0071DC STA */
    S -= 3; i2c_stop_00715B();  /* 0071DE JSRL */
    mflag = 1;  /* 0071E2 SEM */
    SETLO(A, 0x00);  /* 0071E3 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0071E5 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0071E6 STA */
    setP(pull16());  /* 0071E8 PLP */
    mflag = 0;  /* 0071E9 CLM */
    A = rd16(DP(0x00));  /* 0071EA LDA */
    mflag = 1;  /* 0071EC SEM */
    Y = pull16();  /* 0071ED PLY */
    D = pull16();  /* 0071EE PLD */
    S += 3; return;  /* 0071EF RTL */
}

/* 0071F0  callers: 005B8D, 0087BF, 00D961, 013453, 013475, 01347F, 013489, 0134A2 ... */
void eeprom_read_word_0071F0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0071F0 LDA */
    push16(D);  /* 0071F2 PHD */
    D = A;  /* 0071F3 TAD */
    A += 1;  /* 0071F4 INC */
    A += 1;  /* 0071F5 INC */
    push16(A);  /* 0071F6 PHA */
    mflag = 1;  /* 0071F7 SEM */
    SETLO(A, 0x01);  /* 0071F8 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0071FA PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0071FB STA */
    setP(pull16());  /* 0071FD PLP */
    wr16(DP(0x00), Y);  /* 0071FE STY */
    mflag = 0;  /* 007200 CLM */
    S -= 3; i2c_stop_00715B();  /* 007201 JSRL */
    S -= 3; i2c_start_007172();  /* 007205 JSRL */
    A = rd16(DP(0x01));  /* 007209 LDA */
    A &= 0x00FF;  /* 00720B AND */
    mflag = 1;  /* 00720E SEM */
    SETLO(A, A & 0x07);  /* 00720F AND */
    SETLO(A, asl8(A));  /* 007211 ASL */
    SETLO(A, A | 0xA0);  /* 007212 ORA */
    mflag = 0;  /* 007214 CLM */
    Y = A;  /* 007215 TAY */
    S -= 3; i2c_send_byte_006A68();  /* 007216 JSRL */
    Y = rd16(DP(0x00));  /* 00721A LDY */
    mflag = 0;  /* 00721C CLM */
    S -= 3; i2c_send_byte_006A68();  /* 00721D JSRL */
    mflag = 0;  /* 007221 CLM */
    S -= 3; i2c_start_007172();  /* 007222 JSRL */
    A = rd16(DP(0x01));  /* 007226 LDA */
    A &= 0x00FF;  /* 007228 AND */
    mflag = 1;  /* 00722B SEM */
    SETLO(A, A & 0x07);  /* 00722C AND */
    SETLO(A, asl8(A));  /* 00722E ASL */
    SETLO(A, A | 0xA1);  /* 00722F ORA */
    mflag = 0;  /* 007231 CLM */
    Y = A;  /* 007232 TAY */
    S -= 3; i2c_send_byte_006A68();  /* 007233 JSRL */
    Y = 0x0000;  /* 007237 LDY */
    mflag = 0;  /* 00723A CLM */
    S -= 3; i2c_recv_byte_006EB3();  /* 00723B JSRL */
    mflag = 0;  /* 00723F CLM */
    A &= 0x00FF;  /* 007240 AND */
    Y = 0x0001;  /* 007243 LDY */
    wr16(DP(0x00), A);  /* 007246 STA */
    S -= 3; i2c_recv_byte_006EB3();  /* 007248 JSRL */
    mflag = 0;  /* 00724C CLM */
    A &= 0x00FF;  /* 00724D AND */
    wr16(DP(0x02), A);  /* 007250 STA */
    A = rd16(DP(0x00));  /* 007252 LDA */
    A = rla16(A, 0x0008);  /* 007254 RLA */
    A &= 0xFF00;  /* 007258 AND */
    C = 0;  /* 00725B CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00725C ADC */
    wr16(DP(0x00), A);  /* 00725E STA */
    S -= 3; i2c_stop_00715B();  /* 007260 JSRL */
    mflag = 1;  /* 007264 SEM */
    SETLO(A, 0x00);  /* 007265 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007267 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007268 STA */
    setP(pull16());  /* 00726A PLP */
    mflag = 0;  /* 00726B CLM */
    A = rd16(DP(0x00));  /* 00726C LDA */
    Y = pull16();  /* 00726E PLY */
    D = pull16();  /* 00726F PLD */
    S += 3; return;  /* 007270 RTL */
}

/* 007271  callers: 00A31C, 00A357, 00A625, 00AA47, 00AA5A, 00B9F0, 00BA71, 00BA8F ... */
void F_007271(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007271 LDA */
    push16(D);  /* 007273 PHD */
    D = A;  /* 007274 TAD */
    A += 1;  /* 007275 INC */
    A += 1;  /* 007276 INC */
    push16(A);  /* 007277 PHA */
    mflag = 1;  /* 007278 SEM */
    SETLO(A, 0x01);  /* 007279 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00727B PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00727C STA */
    setP(pull16());  /* 00727E PLP */
    wr16(DP(0x00), Y);  /* 00727F STY */
    mflag = 0;  /* 007281 CLM */
    S -= 3; i2c_stop_00715B();  /* 007282 JSRL */
    S -= 3; i2c_start_007172();  /* 007286 JSRL */
    A = rd16(DP(0x01));  /* 00728A LDA */
    A &= 0x00FF;  /* 00728C AND */
    mflag = 1;  /* 00728F SEM */
    SETLO(A, A & 0x07);  /* 007290 AND */
    SETLO(A, asl8(A));  /* 007292 ASL */
    SETLO(A, A | 0xA0);  /* 007293 ORA */
    mflag = 0;  /* 007295 CLM */
    Y = A;  /* 007296 TAY */
    S -= 3; i2c_send_byte_006A68();  /* 007297 JSRL */
    Y = rd16(DP(0x00));  /* 00729B LDY */
    mflag = 0;  /* 00729D CLM */
    S -= 3; i2c_send_byte_006A68();  /* 00729E JSRL */
    mflag = 0;  /* 0072A2 CLM */
    S -= 3; i2c_start_007172();  /* 0072A3 JSRL */
    A = rd16(DP(0x01));  /* 0072A7 LDA */
    A &= 0x00FF;  /* 0072A9 AND */
    mflag = 1;  /* 0072AC SEM */
    SETLO(A, A & 0x07);  /* 0072AD AND */
    SETLO(A, asl8(A));  /* 0072AF ASL */
    SETLO(A, A | 0xA1);  /* 0072B0 ORA */
    mflag = 0;  /* 0072B2 CLM */
    Y = A;  /* 0072B3 TAY */
    S -= 3; i2c_send_byte_006A68();  /* 0072B4 JSRL */
    Y = 0x0000;  /* 0072B8 LDY */
    mflag = 0;  /* 0072BB CLM */
    S -= 3; i2c_recv_byte_006EB3();  /* 0072BC JSRL */
    mflag = 0;  /* 0072C0 CLM */
    A &= 0x00FF;  /* 0072C1 AND */
    Y = 0x0001;  /* 0072C4 LDY */
    wr16(DP(0x00), A);  /* 0072C7 STA */
    S -= 3; i2c_recv_byte_006EB3();  /* 0072C9 JSRL */
    mflag = 0;  /* 0072CD CLM */
    A &= 0x00FF;  /* 0072CE AND */
    A = rla16(A, 0x0008);  /* 0072D1 RLA */
    A &= 0xFF00;  /* 0072D5 AND */
    C = 0;  /* 0072D8 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 0072D9 ADC */
    wr16(DP(0x00), A);  /* 0072DB STA */
    S -= 3; i2c_stop_00715B();  /* 0072DD JSRL */
    mflag = 1;  /* 0072E1 SEM */
    SETLO(A, 0x00);  /* 0072E2 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0072E4 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0072E5 STA */
    setP(pull16());  /* 0072E7 PLP */
    mflag = 0;  /* 0072E8 CLM */
    A = rd16(DP(0x00));  /* 0072E9 LDA */
    Y = pull16();  /* 0072EB PLY */
    D = pull16();  /* 0072EC PLD */
    S += 3; return;  /* 0072ED RTL */
}

/* 0072EE  callers: 0033A2, 004905, 0089CA, 008A73, 008D67, 008EE8, 008F02, 009151 ... */
void i2c_block_xfer_a_0072EE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0072EE LDA */
    push16(D);  /* 0072F0 PHD */
    D = A;  /* 0072F1 TAD */
    C = 0;  /* 0072F2 CLC */
    A = adc16(A, 0x0008);  /* 0072F3 ADC */
    push16(A);  /* 0072F6 PHA */
    wr16(DP(0x06), Y);  /* 0072F7 STY */
    mflag = 1;  /* 0072F9 SEM */
    wr8(DP(0x07), 0x00);  /* 0072FA LDM */
    SETLO(A, 0x01);  /* 0072FD LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0072FF PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007300 STA */
    setP(pull16());  /* 007302 PLP */
    mflag = 0;  /* 007303 CLM */
    S -= 3; i2c_stop_00715B();  /* 007304 JSRL */
    A = rd16(DP(0x06));  /* 007308 LDA */
    mflag = 1;  /* 00730A SEM */
    SETLO(A, A & 0xF0);  /* 00730B AND */
    /* 00730D CMP */
    if ((A & 0xFF) == 0xA0) goto L_007314;  /* 00730F BEQ */
    goto L_007396;  /* 007311 JMP */
L_007314: TICK();
    mflag = 0;  /* 007314 CLM */
    S -= 3; i2c_start_007172();  /* 007315 JSRL */
    A = rd16(DP(0x05));  /* 007319 LDA */
    A &= 0x00FF;  /* 00731B AND */
    mflag = 1;  /* 00731E SEM */
    SETLO(A, A & 0x07);  /* 00731F AND */
    SETLO(A, asl8(A));  /* 007321 ASL */
    SETLO(A, A | 0xA0);  /* 007322 ORA */
    wr8(DP(0x06), (uint8_t)A);  /* 007324 STA */
    mflag = 0;  /* 007326 CLM */
    Y = A;  /* 007327 TAY */
    S -= 3; i2c_send_byte_006A68();  /* 007328 JSRL */
    /* 00732C CMP */
    if ((A & 0xFF) == 0x00) goto L_007333;  /* 00732E BEQ */
    wr8(DP(0x07), 0x01);  /* 007330 LDM */
L_007333: TICK();
    Y = rd16(DP(0x04));  /* 007333 LDY */
    mflag = 0;  /* 007335 CLM */
    S -= 3; i2c_send_byte_006A68();  /* 007336 JSRL */
    /* 00733A CMP */
    if ((A & 0xFF) == 0x00) goto L_007341;  /* 00733C BEQ */
    wr8(DP(0x07), 0x01);  /* 00733E LDM */
L_007341: TICK();
    mflag = 0;  /* 007341 CLM */
    S -= 3; i2c_start_007172();  /* 007342 JSRL */
    A = rd16(DP(0x06));  /* 007346 LDA */
    mflag = 1;  /* 007348 SEM */
    SETLO(A, A + 1);  /* 007349 INC */
    mflag = 0;  /* 00734A CLM */
    Y = A;  /* 00734B TAY */
    S -= 3; i2c_send_byte_006A68();  /* 00734C JSRL */
    /* 007350 CMP */
    if ((A & 0xFF) == 0x00) goto L_007357;  /* 007352 BEQ */
    wr8(DP(0x07), 0x01);  /* 007354 LDM */
L_007357: TICK();
    wr8(ABS(WDT), 0x00);  /* 007357 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 00735B LDA */
    /* 00735D CMP */
    if ((A & 0xFF) == 0x01) goto L_007371;  /* 00735F BEQ */
    mflag = 0;  /* 007361 CLM */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 007362 INC */
    A = rd16(DP(0x04));  /* 007364 LDA */
    A &= 0x00FF;  /* 007366 AND */
    if (A == 0) goto L_007371;  /* 007369 BEQ */
    A = 0x0000;  /* 00736B LDA */
    goto L_007375;  /* 00736E JMP */
L_007371: TICK();
    mflag = 0;  /* 007371 CLM */
    A = 0x0001;  /* 007372 LDA */
L_007375: TICK();
    Y = A;  /* 007375 TAY */
    S -= 3; i2c_recv_byte_006EB3();  /* 007376 JSRL */
    wr8(LIND(0x01), (uint8_t)A);  /* 00737A STA */
    t = (rd8(DP(0x00)) - 1) & 0xFF; wr8(DP(0x00), t);  /* 00737C DEC */
    mflag = 0;  /* 00737E CLM */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 00737F INC */
    mflag = 1;  /* 007381 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 007382 LDA */
    if ((A & 0xFF) == 0) goto L_00738F;  /* 007384 BEQ */
    mflag = 0;  /* 007386 CLM */
    A = rd16(DP(0x04));  /* 007387 LDA */
    A &= 0x00FF;  /* 007389 AND */
    Z = (A == 0);
    mflag = 1;  /* 00738C SEM */
    if (!Z) goto L_007357;  /* 00738D BNE */
L_00738F: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00738F LDA */
    if ((A & 0xFF) != 0) goto L_007314;  /* 007391 BNE */
    goto L_0073B6;  /* 007393 JMP */
L_007396: TICK();
    mflag = 0;  /* 007396 CLM */
    S -= 3; i2c_start_007172();  /* 007397 JSRL */
    A = rd16(DP(0x06));  /* 00739B LDA */
    mflag = 1;  /* 00739D SEM */
    SETLO(A, A + 1);  /* 00739E INC */
    mflag = 0;  /* 00739F CLM */
    Y = A;  /* 0073A0 TAY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0073A1 JSRL */
    /* 0073A5 CMP */
    if ((A & 0xFF) == 0x00) goto L_0073AC;  /* 0073A7 BEQ */
    wr8(DP(0x07), 0x01);  /* 0073A9 LDM */
L_0073AC: TICK();
    Y = 0x0001;  /* 0073AC LDY */
    mflag = 0;  /* 0073AF CLM */
    S -= 3; i2c_recv_byte_fast_006FA0();  /* 0073B0 JSRL */
    wr8(LIND(0x01), (uint8_t)A);  /* 0073B4 STA */
L_0073B6: TICK();
    mflag = 0;  /* 0073B6 CLM */
    S -= 3; i2c_stop_00715B();  /* 0073B7 JSRL */
    mflag = 1;  /* 0073BB SEM */
    SETLO(A, 0x00);  /* 0073BC LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0073BE PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0073BF STA */
    setP(pull16());  /* 0073C1 PLP */
    mflag = 0;  /* 0073C2 CLM */
    Y = pull16();  /* 0073C3 PLY */
    D = pull16();  /* 0073C4 PLD */
    S += 3; return;  /* 0073C5 RTL */
}

/* 0073C6  callers: 004357, 004FC7, 008EC5, 00912D, 009BEB, 00A1E7, 00B6CC, 00B6FD ... */
void i2c_block_xfer_b_0073C6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0073C6 LDA */
    push16(D);  /* 0073C8 PHD */
    D = A;  /* 0073C9 TAD */
    C = 0;  /* 0073CA CLC */
    A = adc16(A, 0x0009);  /* 0073CB ADC */
    push16(A);  /* 0073CE PHA */
    wr16(DP(0x06), Y);  /* 0073CF STY */
    mflag = 1;  /* 0073D1 SEM */
    SETLO(A, 0x01);  /* 0073D2 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0073D4 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0073D5 STA */
    setP(pull16());  /* 0073D7 PLP */
    mflag = 1;  /* 0073D8 SEM */
    wr8(DP(0x08), 0x00);  /* 0073D9 LDM */
    mflag = 0;  /* 0073DC CLM */
    S -= 3; i2c_stop_00715B();  /* 0073DD JSRL */
    A = rd16(DP(0x06));  /* 0073E1 LDA */
    mflag = 1;  /* 0073E3 SEM */
    SETLO(A, A & 0xF0);  /* 0073E4 AND */
    /* 0073E6 CMP */
    if ((A & 0xFF) == 0xA0) goto L_0073ED;  /* 0073E8 BEQ */
    goto L_007488;  /* 0073EA JMP */
L_0073ED: TICK();
    wr8(DP(0x07), 0x00);  /* 0073ED LDM */
L_0073F0: TICK();
    SETLO(A, rd8(DP(0x07)));  /* 0073F0 LDA */
    t = rd8(DP(0x00));  /* 0073F2 CMP */
    if ((A & 0xFF) < t) goto L_0073F9;  /* 0073F4 BCC */
    goto L_007485;  /* 0073F6 JMP */
L_0073F9: TICK();
    mflag = 0;  /* 0073F9 CLM */
    S -= 3; i2c_start_007172();  /* 0073FA JSRL */
    A = rd16(DP(0x05));  /* 0073FE LDA */
    A &= 0x00FF;  /* 007400 AND */
    mflag = 1;  /* 007403 SEM */
    SETLO(A, A & 0x07);  /* 007404 AND */
    SETLO(A, asl8(A));  /* 007406 ASL */
    SETLO(A, A | 0xA0);  /* 007407 ORA */
    wr8(DP(0x06), (uint8_t)A);  /* 007409 STA */
    mflag = 0;  /* 00740B CLM */
    Y = A;  /* 00740C TAY */
    S -= 3; i2c_send_byte_006A68();  /* 00740D JSRL */
    /* 007411 CMP */
    if ((A & 0xFF) == 0x00) goto L_007418;  /* 007413 BEQ */
    wr8(DP(0x08), 0x01);  /* 007415 LDM */
L_007418: TICK();
    Y = rd16(DP(0x04));  /* 007418 LDY */
    mflag = 0;  /* 00741A CLM */
    S -= 3; i2c_send_byte_006A68();  /* 00741B JSRL */
    /* 00741F CMP */
    if ((A & 0xFF) == 0x00) goto L_007426;  /* 007421 BEQ */
    wr8(DP(0x08), 0x01);  /* 007423 LDM */
L_007426: TICK();
    SETLO(A, rd8(DP(0x07)));  /* 007426 LDA */
    t = (rd8(DP(0x07)) + 1) & 0xFF; wr8(DP(0x07), t);  /* 007428 INC */
    mflag = 0;  /* 00742A CLM */
    A &= 0x00FF;  /* 00742B AND */
    C = 0;  /* 00742E CLC */
    A = adc16(A, rd16(DP(0x01)));  /* 00742F ADC */
    wr16(DP(0x09), A);  /* 007431 STA */
    A = rd16(DP(0x03));  /* 007433 LDA */
    mflag = 1;  /* 007435 SEM */
    wr8(DP(0x0B), (uint8_t)A);  /* 007436 STA */
    mflag = 0;  /* 007438 CLM */
    A = rd16(LIND(0x09));  /* 007439 LDA */
    Y = A;  /* 00743B TAY */
    S -= 3; i2c_send_byte_006A68();  /* 00743C JSRL */
    /* 007440 CMP */
    if ((A & 0xFF) == 0x00) goto L_007447;  /* 007442 BEQ */
    wr8(DP(0x08), 0x01);  /* 007444 LDM */
L_007447: TICK();
    mflag = 0;  /* 007447 CLM */
    t = (rd16(DP(0x04)) + 1) & 0xFFFF; wr16(DP(0x04), t);  /* 007448 INC */
    A = rd16(DP(0x07));  /* 00744A LDA */
    mflag = 1;  /* 00744C SEM */
    t = rd8(DP(0x00));  /* 00744D CMP */
    if ((A & 0xFF) >= t) goto L_00745A;  /* 00744F BCS */
    mflag = 0;  /* 007451 CLM */
    A = rd16(DP(0x04));  /* 007452 LDA */
    A &= 0x0007;  /* 007454 AND */
    Z = (A == 0);
    mflag = 1;  /* 007457 SEM */
    if (!Z) goto L_007426;  /* 007458 BNE */
L_00745A: TICK();
    wr8(ABS(0x022A), 0x02);  /* 00745A LDM */
L_00745E: TICK();
    wr8(ABS(WDT), 0x00);  /* 00745E LDM */
    mflag = 0;  /* 007462 CLM */
    S -= 3; i2c_stop_00715B();  /* 007463 JSRL */
    S -= 3; i2c_start_007172();  /* 007467 JSRL */
    mflag = 1;  /* 00746B SEM */
    SETLO(A, rd8(ABS(0x022A)));  /* 00746C LDA */
    if ((A & 0xFF) == 0) goto L_00747C;  /* 00746F BEQ */
    Y = rd16(DP(0x06));  /* 007471 LDY */
    mflag = 0;  /* 007473 CLM */
    S -= 3; i2c_send_byte_006A68();  /* 007474 JSRL */
    /* 007478 CMP */
    if ((A & 0xFF) != 0x00) goto L_00745E;  /* 00747A BNE */
L_00747C: TICK();
    mflag = 0;  /* 00747C CLM */
    S -= 3; i2c_stop_00715B();  /* 00747D JSRL */
    mflag = 1;  /* 007481 SEM */
    goto L_0073F0;  /* 007482 JMP */
L_007485: TICK();
    goto L_0074C8;  /* 007485 JMP */
L_007488: TICK();
    mflag = 0;  /* 007488 CLM */
    S -= 3; i2c_start_007172();  /* 007489 JSRL */
    Y = rd16(DP(0x06));  /* 00748D LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00748F JSRL */
    /* 007493 CMP */
    if ((A & 0xFF) == 0x00) goto L_00749A;  /* 007495 BEQ */
    wr8(DP(0x08), 0x01);  /* 007497 LDM */
L_00749A: TICK();
    wr8(DP(0x07), 0x00);  /* 00749A LDM */
L_00749D: TICK();
    SETLO(A, rd8(DP(0x07)));  /* 00749D LDA */
    t = rd8(DP(0x00));  /* 00749F CMP */
    C = ((A & 0xFF) >= t);
    if (C) goto L_0074C8;  /* 0074A1 BCS */
    wr8(ABS(WDT), 0x00);  /* 0074A3 LDM */
    mflag = 0;  /* 0074A7 CLM */
    A &= 0x00FF;  /* 0074A8 AND */
    A = adc16(A, rd16(DP(0x01)));  /* 0074AB ADC */
    wr16(DP(0x09), A);  /* 0074AD STA */
    A = rd16(DP(0x03));  /* 0074AF LDA */
    mflag = 1;  /* 0074B1 SEM */
    wr8(DP(0x0B), (uint8_t)A);  /* 0074B2 STA */
    mflag = 0;  /* 0074B4 CLM */
    A = rd16(LIND(0x09));  /* 0074B5 LDA */
    Y = A;  /* 0074B7 TAY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0074B8 JSRL */
    /* 0074BC CMP */
    if ((A & 0xFF) == 0x00) goto L_0074C3;  /* 0074BE BEQ */
    wr8(DP(0x08), 0x01);  /* 0074C0 LDM */
L_0074C3: TICK();
    t = (rd8(DP(0x07)) + 1) & 0xFF; wr8(DP(0x07), t);  /* 0074C3 INC */
    goto L_00749D;  /* 0074C5 JMP */
L_0074C8: TICK();
    mflag = 0;  /* 0074C8 CLM */
    S -= 3; i2c_stop_00715B();  /* 0074C9 JSRL */
    mflag = 1;  /* 0074CD SEM */
    SETLO(A, 0x00);  /* 0074CE LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0074D0 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0074D1 STA */
    setP(pull16());  /* 0074D3 PLP */
    mflag = 0;  /* 0074D4 CLM */
    Y = pull16();  /* 0074D5 PLY */
    D = pull16();  /* 0074D6 PLD */
    S += 3; return;  /* 0074D7 RTL */
}

/* 0074D8  callers: 0058AD, 00752F */
void F_0074D8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0074D8 LDA */
    push16(D);  /* 0074DA PHD */
    D = A;  /* 0074DB TAD */
    mflag = 1;  /* 0074DC SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_007509;  /* 0074DD BBC */
    wr8(DP(0x00), 0xF8);  /* 0074E2 LDM */
    mflag = 0;  /* 0074E5 CLM */
    if ((rd16(ABS(0x0242)) & 0x0040) == 0x0040) goto L_0074F0;  /* 0074E6 BBS */
    mflag = 1;  /* 0074EC SEM */
    wr8(DP(0x00), rd8(DP(0x00)) | 0x01);  /* 0074ED SEB */
L_0074F0: TICK();
    mflag = 0;  /* 0074F0 CLM */
    if ((rd16(ABS(0x0242)) & 0x0080) == 0x0080) goto L_0074FB;  /* 0074F1 BBS */
    mflag = 1;  /* 0074F7 SEM */
    wr8(DP(0x00), rd8(DP(0x00)) | 0x02);  /* 0074F8 SEB */
L_0074FB: TICK();
    mflag = 0;  /* 0074FB CLM */
    if ((rd16(ABS(0x0242)) & 0x0100) == 0x0100) goto L_007506;  /* 0074FC BBS */
    mflag = 1;  /* 007502 SEM */
    wr8(DP(0x00), rd8(DP(0x00)) | 0x04);  /* 007503 SEB */
L_007506: TICK();
    goto L_007523;  /* 007506 JMP */
L_007509: TICK();
    mflag = 1;  /* 007509 SEM */
    wr8(DP(0x00), 0xFC);  /* 00750A LDM */
    mflag = 0;  /* 00750D CLM */
    if ((rd16(ABS(0x0242)) & 0x2000) == 0) goto L_007518;  /* 00750E BBC */
    mflag = 1;  /* 007514 SEM */
    wr8(DP(0x00), rd8(DP(0x00)) | 0x01);  /* 007515 SEB */
L_007518: TICK();
    mflag = 0;  /* 007518 CLM */
    if ((rd16(ABS(0x0242)) & 0x1000) == 0) goto L_007523;  /* 007519 BBC */
    mflag = 1;  /* 00751F SEM */
    wr8(DP(0x00), rd8(DP(0x00)) | 0x02);  /* 007520 SEB */
L_007523: TICK();
    mflag = 0;  /* 007523 CLM */
    A = rd16(DP(0x00));  /* 007524 LDA */
    mflag = 1;  /* 007526 SEM */
    D = pull16();  /* 007527 PLD */
    S += 3; return;  /* 007528 RTL */
}

/* 007529  callers: 006778, 007645, 007B68, 00818C, 0099D3, 00E262, 01A63C */
void F_007529(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007529 LDA */
    push16(D);  /* 00752B PHD */
    D = A;  /* 00752C TAD */
    A += 1;  /* 00752D INC */
    push16(A);  /* 00752E PHA */
    S -= 3; F_0074D8();  /* 00752F JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 007533 STA */
    SETLO(A, 0x01);  /* 007535 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007537 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007538 STA */
    setP(pull16());  /* 00753A PLP */
    mflag = 0;  /* 00753B CLM */
    S -= 3; i2c_start_007172();  /* 00753C JSRL */
    Y = 0x0046;  /* 007540 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 007543 JSRL */
    Y = rd16(DP(0x00));  /* 007547 LDY */
    mflag = 0;  /* 007549 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00754A JSRL */
    mflag = 0;  /* 00754E CLM */
    S -= 3; i2c_stop_00715B();  /* 00754F JSRL */
    mflag = 1;  /* 007553 SEM */
    SETLO(A, 0x00);  /* 007554 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007556 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007557 STA */
    setP(pull16());  /* 007559 PLP */
    mflag = 0;  /* 00755A CLM */
    Y = pull16();  /* 00755B PLY */
    D = pull16();  /* 00755C PLD */
    S += 3; return;  /* 00755D RTL */
}

/* 00755E  callers: 00647D, 0068E5, 007B70, 00806E, 0080A3, 008194, 00CC6C, 00E2ED ... */
void cbus_write_sel2_00755E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00755E LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    push16(D);  /* 007560 PHD */
    D = A;  /* 007561 TAD */
    push16(getP());  /* 007562 PHP */
    mflag = 1;  /* 007563 SEM */
    SETLO(A, pull8());  /* 007564 PLA */
    SETLO(A, pull8());  /* 007565 PLA */
    wr8(DP(0x00), (uint8_t)A);  /* 007566 STA */
    C = ((A & 0xFF) >= 0x03);  /* 007568 CMP */
    if (C) goto L_007572;  /* 00756A BCS */
    SETLO(A, 0x03);  /* 00756C LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 00756E PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 00756F STA */
    setP(pull16());  /* 007571 PLP */
L_007572: TICK();
    mflag = 1;  /* 007572 SEM */
    wr8(DP(0x01), 0x00);  /* 007573 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_007594;  /* 007576 BBC */
    mflag = 0;  /* 00757B CLM */
    if ((rd16(ABS(0x0242)) & 0x1000) == 0) goto L_007586;  /* 00757C BBC */
    mflag = 1;  /* 007582 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x20);  /* 007583 SEB */
L_007586: TICK();
    mflag = 0;  /* 007586 CLM */
    if ((rd16(ABS(0x0242)) & 0x0800) == 0) goto L_007591;  /* 007587 BBC */
    mflag = 1;  /* 00758D SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x08);  /* 00758E SEB */
L_007591: TICK();
    goto L_0075CC;  /* 007591 JMP */
L_007594: TICK();
    mflag = 1;  /* 007594 SEM */
    if ((rd8(ABS(0x0242)) & 0x40) == 0) goto L_00759D;  /* 007595 BBC */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x80);  /* 00759A SEB */
L_00759D: TICK();
    if ((rd8(ABS(0x0242)) & 0x80) == 0) goto L_0075A5;  /* 00759D BBC */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x40);  /* 0075A2 SEB */
L_0075A5: TICK();
    mflag = 0;  /* 0075A5 CLM */
    if ((rd16(ABS(0x0242)) & 0x0100) == 0) goto L_0075B0;  /* 0075A6 BBC */
    mflag = 1;  /* 0075AC SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x20);  /* 0075AD SEB */
L_0075B0: TICK();
    mflag = 0;  /* 0075B0 CLM */
    if ((rd16(ABS(0x0240)) & 0x0001) == 0) goto L_0075BD;  /* 0075B1 BBC */
    mflag = 1;  /* 0075B7 SEM */
    if ((rd8(ABS(0x0246)) & 0x01) == 0) goto L_0075C1;  /* 0075B8 BBC */
L_0075BD: TICK();
    mflag = 1;  /* 0075BD SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x10);  /* 0075BE SEB */
L_0075C1: TICK();
    mflag = 0;  /* 0075C1 CLM */
    if ((rd16(ABS(0x0242)) & 0x4000) == 0) goto L_0075CC;  /* 0075C2 BBC */
    mflag = 1;  /* 0075C8 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x08);  /* 0075C9 SEB */
L_0075CC: TICK();
    mflag = 0;  /* 0075CC CLM */
    if ((rd16(ABS(0x0242)) & 0x0400) == 0) goto L_0075D7;  /* 0075CD BBC */
    mflag = 1;  /* 0075D3 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x02);  /* 0075D4 SEB */
L_0075D7: TICK();
    mflag = 0;  /* 0075D7 CLM */
    if ((rd16(ABS(0x0242)) & 0x0200) == 0) goto L_0075E2;  /* 0075D8 BBC */
    mflag = 1;  /* 0075DE SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x01);  /* 0075DF SEB */
L_0075E2: TICK();
    mflag = 1;  /* 0075E2 SEM */
    wr8(ABS(U1MOD), 0x01);  /* 0075E3 LDM */
    wr8(ABS(U1C0), 0x00);  /* 0075E7 LDM */
    wr8(ABS(U1BRG), 0x01);  /* 0075EB LDM */
    wr8(ABS(U1C1), 0x01);  /* 0075EF LDM */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x08);  /* 0075F3 CLB [P53 TDSEC2] */
    SETLO(A, rd8(DP(0x01)));  /* 0075F7 LDA */
    wr8(ABS(U1TB), (uint8_t)A);  /* 0075F9 STA */
L_0075FC: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_007604;  /* 0075FC BBS */
    goto L_0075FC;  /* 007601 JMP */
L_007604: TICK();
    mflag = 1;  /* 007604 SEM */
    if ((rd8(ABS(U1C0)) & 0x08) == 0x08) goto L_00760D;  /* 007605 BBS */
    goto L_007604;  /* 00760A JMP */
L_00760D: TICK();
    mflag = 1;  /* 00760D SEM */
    wr8(ABS(P5), rd8(ABS(P5)) | 0x08);  /* 00760E SEB [P53 TDSEC2] */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x08);  /* 007612 CLB [P53 TDSEC2] */
    wr8(ABS(U1C1), rd8(ABS(U1C1)) & ~0x05);  /* 007616 CLB */
    wr8(ABS(U1MOD), 0x00);  /* 00761A LDM */
    SETLO(A, rd8(DP(0x00)));  /* 00761E LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007620 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007621 STA */
    setP(pull16());  /* 007623 PLP */
    mflag = 0;  /* 007624 CLM */
    D = pull16();  /* 007625 PLD */
    S += 3; return;  /* 007626 RTL */
}

/* 007627  callers: 0064E7, 0077F6, 0077FD, 007828, 00782F, 00787A, 007891, 007898 ... */
void F_007627(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007627 LDA */
    push16(D);  /* 007629 PHD */
    D = A;  /* 00762A TAD */
    A += 1;  /* 00762B INC */
    push16(A);  /* 00762C PHA */
    wr16(DP(0x00), Y);  /* 00762D STY */
    I = 1;  /* 00762F SEI */
    A = rd16(ABS(0x0332));  /* 007630 LDA */
    mflag = 1;  /* 007633 SEM */
    t = rd8(ABS(0x0333));  /* 007634 CMP */
    if ((A & 0xFF) != t) goto L_00765A;  /* 007637 BNE */
    mflag = 0;  /* 007639 CLM */
    if ((rd16(ABS(0x0242)) & 0x2000) == 0) goto L_007649;  /* 00763A BBC */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x2000);  /* 007640 CLB */
    S -= 3; F_007529();  /* 007645 JSRL */
L_007649: TICK();
    mflag = 1;  /* 007649 SEM */
    wr8(ABS(0x0332), 0x00);  /* 00764A LDM */
    wr8(ABS(0x0333), 0x01);  /* 00764E LDM */
    SETLO(A, rd8(DP(0x00)));  /* 007652 LDA */
    wr8(ABS(U0TB), (uint8_t)A);  /* 007654 STA */
    goto L_007697;  /* 007657 JMP */
L_00765A: TICK();
    mflag = 0;  /* 00765A CLM */
    A = rd16(ABS(0x0333));  /* 00765B LDA */
    mflag = 1;  /* 00765E SEM */
    /* 00765F CMP */
    if ((A & 0xFF) >= 0x50) goto L_007677;  /* 007661 BCS */
    SETLO(A, rd8(ABS(0x0333)));  /* 007663 LDA */
    t = (rd8(ABS(0x0333)) + 1) & 0xFF; wr8(ABS(0x0333), t);  /* 007666 INC */
    mflag = 0;  /* 007669 CLM */
    A &= 0x00FF;  /* 00766A AND */
    X = A;  /* 00766D TAX */
    A = rd16(DP(0x00));  /* 00766E LDA */
    mflag = 1;  /* 007670 SEM */
    wr8((ABS(0x02E2) + X), (uint8_t)A);  /* 007671 STA */
    goto L_007697;  /* 007674 JMP */
L_007677: TICK();
    I = 0;  /* 007677 CLI */
L_007678: TICK();
    mflag = 0;  /* 007678 CLM */
    A = rd16(ABS(0x0332));  /* 007679 LDA */
    mflag = 1;  /* 00767C SEM */
    t = rd8(ABS(0x0333));  /* 00767D CMP */
    if ((A & 0xFF) == t) goto L_007689;  /* 007680 BEQ */
    wr8(ABS(WDT), 0x00);  /* 007682 LDM */
    goto L_007678;  /* 007686 JMP */
L_007689: TICK();
    mflag = 1;  /* 007689 SEM */
    wr8(ABS(0x0332), 0x00);  /* 00768A LDM */
    wr8(ABS(0x0333), 0x01);  /* 00768E LDM */
    SETLO(A, rd8(DP(0x00)));  /* 007692 LDA */
    wr8(ABS(U0TB), (uint8_t)A);  /* 007694 STA */
L_007697: TICK();
    I = 0;  /* 007697 CLI */
    mflag = 0;  /* 007698 CLM */
    Y = pull16();  /* 007699 PLY */
    D = pull16();  /* 00769A PLD */
    S += 3; return;  /* 00769B RTL */
}

/* 00769C  callers: 007984, 0079C5, 0079F2 */
void F_00769C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00769C LDA */
    push16(D);  /* 00769E PHD */
    D = A;  /* 00769F TAD */
    wr16(DP(0x00), Y);  /* 0076A0 STY */
    A = Y;  /* 0076A2 TYA */
    mflag = 1;  /* 0076A3 SEM */
    /* 0076A4 CMP */
    if ((A & 0xFF) != 0x0F) goto L_0076AE;  /* 0076A6 BNE */
    wr8(DP(0x01), 0x20);  /* 0076A8 LDM */
    goto L_0076D7;  /* 0076AB JMP */
L_0076AE: TICK();
    mflag = 0;  /* 0076AE CLM */
    A = rd16(DP(0x00));  /* 0076AF LDA */
    mflag = 1;  /* 0076B1 SEM */
    t = rd8(ABS(0x020E));  /* 0076B2 CMP */
    if ((A & 0xFF) != t) goto L_0076BD;  /* 0076B5 BNE */
    wr8(DP(0x01), 0x47);  /* 0076B7 LDM */
    goto L_0076D7;  /* 0076BA JMP */
L_0076BD: TICK();
    mflag = 0;  /* 0076BD CLM */
    A = rd16(DP(0x00));  /* 0076BE LDA */
    mflag = 1;  /* 0076C0 SEM */
    C = ((A & 0xFF) >= 0x0A);  /* 0076C1 CMP */
    if (!C) goto L_0076CE;  /* 0076C3 BCC */
    SETLO(A, rd8(DP(0x00)));  /* 0076C5 LDA */
    SETLO(A, adc8(A, 0x36));  /* 0076C7 ADC */
    wr8(DP(0x01), (uint8_t)A);  /* 0076C9 STA */
    goto L_0076D7;  /* 0076CB JMP */
L_0076CE: TICK();
    mflag = 0;  /* 0076CE CLM */
    A = rd16(DP(0x00));  /* 0076CF LDA */
    C = 0;  /* 0076D1 CLC */
    mflag = 1;  /* 0076D2 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0076D3 ADC */
    wr8(DP(0x01), (uint8_t)A);  /* 0076D5 STA */
L_0076D7: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0076D7 LDA */
    D = pull16();  /* 0076D9 PLD */
    S += 3; return;  /* 0076DA RTL */
}

/* 0076DB  callers: 003DC1, 00413D, 00783D */
void F_0076DB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0076DB LDA */
    push16(D);  /* 0076DD PHD */
    D = A;  /* 0076DE TAD */
    wr16(DP(0x00), Y);  /* 0076DF STY */
    wr16(DP(0x02), B);  /* 0076E1 STA */
    wr16(DP(0x09), 0x01E1);  /* 0076E4 LDM */
    wr16(DP(0x07), 0x3380);  /* 0076E8 LDM */
    A = rd16(ABS(0x01F1));  /* 0076EC LDA */
    B = rd16(ABS(0x01F3));  /* 0076EF LDA */
    X = 0x0007;  /* 0076F3 LDX */
    S -= 3; rt_lumod_01E43E();  /* 0076F6 JSRL */
    wr16(ABS(0x01F1), A);  /* 0076FA STA */
    wr16(ABS(0x01F3), B);  /* 0076FD STA */
    wr16(DP(0x09), 0x0001);  /* 007701 LDM */
    wr16(DP(0x07), 0x5180);  /* 007705 LDM */
    X = 0x0007;  /* 007709 LDX */
    S -= 3; rt_ludiv_01E414();  /* 00770C JSRL */
    A += 1;  /* 007710 INC */
    wr16(DP(0x03), A);  /* 007711 STA */
    mflag = 1;  /* 007713 SEM */
    wr8(DP(0x05), 0x00);  /* 007714 LDM */
L_007717: TICK();
    SETLO(A, rd8(DP(0x05)));  /* 007717 LDA */
    mflag = 0;  /* 007719 CLM */
    A &= 0x00FF;  /* 00771A AND */
    X = A;  /* 00771D TAX */
    A = rd16((0x002AFA + X));  /* 00771E LDA */
    A &= 0x00FF;  /* 007722 AND */
    t = rd16(DP(0x03));  /* 007725 CMP */
    C = (A >= t);
    if (C) goto L_007746;  /* 007727 BCS */
    A = rd16(DP(0x05));  /* 007729 LDA */
    mflag = 1;  /* 00772B SEM */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 00772C INC */
    mflag = 0;  /* 00772E CLM */
    A &= 0x00FF;  /* 00772F AND */
    X = A;  /* 007732 TAX */
    A = rd16((0x002AFA + X));  /* 007733 LDA */
    A &= 0x00FF;  /* 007737 AND */
    A ^= 0xFFFF;  /* 00773A EOR */
    A += 1;  /* 00773D INC */
    A = adc16(A, rd16(DP(0x03)));  /* 00773E ADC */
    wr16(DP(0x03), A);  /* 007740 STA */
    mflag = 1;  /* 007742 SEM */
    goto L_007717;  /* 007743 JMP */
L_007746: TICK();
    mflag = 0;  /* 007746 CLM */
    A = rd16(DP(0x03));  /* 007747 LDA */
    mflag = 1;  /* 007749 SEM */
    wr8(DP(0x06), (uint8_t)A);  /* 00774A STA */
    mflag = 0;  /* 00774C CLM */
    A = D;  /* 00774D TDA */
    C = 0;  /* 00774E CLC */
    A = adc16(A, 0x0005);  /* 00774F ADC */
    wr16(DP(0x07), A);  /* 007752 STA */
    wr16(DP(0x09), A);  /* 007754 STA */
    mflag = 1;  /* 007756 SEM */
    wr8(DP(0x0B), 0x00);  /* 007757 LDM */
    mflag = 0;  /* 00775A CLM */
    A = rd16(DP(0x00));  /* 00775B LDA */
    wr16(DP(0x0C), A);  /* 00775D STA */
    A = rd16(DP(0x02));  /* 00775F LDA */
    mflag = 1;  /* 007761 SEM */
    wr8(DP(0x0E), (uint8_t)A);  /* 007762 STA */
    mflag = 0;  /* 007764 CLM */
    A = 0x0009;  /* 007765 LDA */
    Y = 0x0002;  /* 007768 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00776B JSRL */
    D = pull16();  /* 00776F PLD */
    S += 3; return;  /* 007770 RTL */
}

/* 007771  callers: 003EE9, 004014, 0078DD */
void F_007771(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007771 LDA */
    push16(D);  /* 007773 PHD */
    D = A;  /* 007774 TAD */
    wr16(DP(0x07), 0x0000);  /* 007775 LDM */
    wr16(DP(0x05), 0x003C);  /* 007779 LDM */
    wr16(DP(0x00), Y);  /* 00777D STY */
    mflag = 1;  /* 00777F SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 007780 STA */
    mflag = 0;  /* 007783 CLM */
    A = rd16(ABS(0x01F1));  /* 007784 LDA */
    B = rd16(ABS(0x01F3));  /* 007787 LDA */
    X = 0x0005;  /* 00778B LDX */
    S -= 3; rt_ludiv_01E414();  /* 00778E JSRL */
    wr16(DP(0x07), 0x0000);  /* 007792 LDM */
    wr16(DP(0x05), 0x003C);  /* 007796 LDM */
    X = 0x0005;  /* 00779A LDX */
    S -= 3; rt_lumod_01E43E();  /* 00779D JSRL */
    mflag = 1;  /* 0077A1 SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 0077A2 STA */
    mflag = 0;  /* 0077A4 CLM */
    wr16(DP(0x07), 0x0000);  /* 0077A5 LDM */
    wr16(DP(0x05), 0x0E10);  /* 0077A9 LDM */
    A = rd16(ABS(0x01F1));  /* 0077AD LDA */
    B = rd16(ABS(0x01F3));  /* 0077B0 LDA */
    X = 0x0005;  /* 0077B4 LDX */
    S -= 3; rt_ludiv_01E414();  /* 0077B7 JSRL */
    wr16(DP(0x07), 0x0000);  /* 0077BB LDM */
    wr16(DP(0x05), 0x0018);  /* 0077BF LDM */
    X = 0x0005;  /* 0077C3 LDX */
    S -= 3; rt_lumod_01E43E();  /* 0077C6 JSRL */
    mflag = 1;  /* 0077CA SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 0077CB STA */
    mflag = 0;  /* 0077CD CLM */
    A = D;  /* 0077CE TDA */
    A += 1;  /* 0077CF INC */
    A += 1;  /* 0077D0 INC */
    A += 1;  /* 0077D1 INC */
    wr16(DP(0x05), A);  /* 0077D2 STA */
    wr16(DP(0x07), A);  /* 0077D4 STA */
    mflag = 1;  /* 0077D6 SEM */
    wr8(DP(0x09), 0x00);  /* 0077D7 LDM */
    mflag = 0;  /* 0077DA CLM */
    A = Y;  /* 0077DB TYA */
    wr16(DP(0x0A), A);  /* 0077DC STA */
    A = rd16(DP(0x02));  /* 0077DE LDA */
    mflag = 1;  /* 0077E0 SEM */
    wr8(DP(0x0C), (uint8_t)A);  /* 0077E1 STA */
    mflag = 0;  /* 0077E3 CLM */
    A = 0x0007;  /* 0077E4 LDA */
    Y = 0x0002;  /* 0077E7 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 0077EA JSRL */
    D = pull16();  /* 0077EE PLD */
    S += 3; return;  /* 0077EF RTL */
}

/* 0077F0  callers: 007963, 0099B7, 014339, 014477, 014652, 0147A6, 014808, 014848 ... */
void F_0077F0(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0077F0 LDA */
    push16(A);  /* 0077F2 PHA */
    Y = 0x000D;  /* 0077F3 LDY */
    S -= 3; F_007627();  /* 0077F6 JSRL */
    Y = 0x000A;  /* 0077FA LDY */
    S -= 3; F_007627();  /* 0077FD JSRL */
    mflag = 1;  /* 007801 SEM */
    wr8(ABS(WDT), 0x00);  /* 007802 LDM */
    mflag = 0;  /* 007806 CLM */
    Y = pull16();  /* 007807 PLY */
    S += 3; return;  /* 007808 RTL */
}

/* 007809  callers: 006514, 00F55D, 00F568, 00F57E, 00F5D1, 00F5F4, 00F657, 00F664 ... */
void F_007809(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007809 LDA */
    push16(D);  /* 00780B PHD */
    D = A;  /* 00780C TAD */
    C = 0;  /* 00780D CLC */
    A = adc16(A, 0x0008);  /* 00780E ADC */
    push16(A);  /* 007811 PHA */
    wr16(DP(0x00), Y);  /* 007812 STY */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0x0100) goto L_00781D;  /* 007814 BBS */
    goto L_007A02;  /* 00781A JMP */
L_00781D: TICK();
    mflag = 1;  /* 00781D SEM */
    SETLO(A, Y);  /* 00781E TYA */
    if ((A & 0xFF) == 0) goto L_007824;  /* 00781F BEQ */
    goto L_00796A;  /* 007821 JMP */
L_007824: TICK();
    Y = 0x0020;  /* 007824 LDY */
    mflag = 0;  /* 007827 CLM */
    S -= 3; F_007627();  /* 007828 JSRL */
    Y = 0x0009;  /* 00782C LDY */
    S -= 3; F_007627();  /* 00782F JSRL */
    A = D;  /* 007833 TDA */
    C = 0;  /* 007834 CLC */
    A = adc16(A, 0x0006);  /* 007835 ADC */
    B = 0x0000;  /* 007838 LDA */
    Y = A;  /* 00783C TAY */
    S -= 3; F_0076DB();  /* 00783D JSRL */
    wr16(DP(0x08), A);  /* 007841 STA */
    A = D;  /* 007843 TDA */
    C = 0;  /* 007844 CLC */
    A = adc16(A, 0x0004);  /* 007845 ADC */
    wr16(DP(0x0A), A);  /* 007848 STA */
    mflag = 1;  /* 00784A SEM */
    wr8(DP(0x0C), (uint8_t)B);  /* 00784B STA */
    mflag = 0;  /* 00784E CLM */
    A = rd16(DP(0x08));  /* 00784F LDA */
    wr16(DP(0x0D), A);  /* 007851 STA */
    A = rd16(DP(0x0C));  /* 007853 LDA */
    mflag = 1;  /* 007855 SEM */
    wr8(DP(0x0F), (uint8_t)A);  /* 007856 STA */
    mflag = 0;  /* 007858 CLM */
    A = rd16(DP(0x0A));  /* 007859 LDA */
    wr16(DP(0x10), A);  /* 00785B STA */
    mflag = 1;  /* 00785D SEM */
    wr8(DP(0x12), 0x00);  /* 00785E LDM */
    mflag = 0;  /* 007861 CLM */
    A = 0x000D;  /* 007862 LDA */
    Y = 0x0002;  /* 007865 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 007868 JSRL */
    mflag = 1;  /* 00786C SEM */
    SETLO(B, 0x00);  /* 00786D LDA */
    SETLO(A, rd8(DP(0x05)));  /* 007870 LDA */
    div8(0x0A);  /* 007872 DIV */
    C = 0;  /* 007875 CLC */
    SETLO(A, adc8(A, 0x30));  /* 007876 ADC */
    mflag = 0;  /* 007878 CLM */
    Y = A;  /* 007879 TAY */
    S -= 3; F_007627();  /* 00787A JSRL */
    mflag = 1;  /* 00787E SEM */
    SETLO(B, 0x00);  /* 00787F LDA */
    SETLO(A, rd8(DP(0x05)));  /* 007882 LDA */
    div8(0x0A);  /* 007884 DIV */
    mflag = 0;  /* 007887 CLM */
    push16(B);  /* 007888 PHB */
    A = pull16();  /* 00788A PLA */
    C = 0;  /* 00788B CLC */
    mflag = 1;  /* 00788C SEM */
    SETLO(A, adc8(A, 0x30));  /* 00788D ADC */
    mflag = 0;  /* 00788F CLM */
    Y = A;  /* 007890 TAY */
    S -= 3; F_007627();  /* 007891 JSRL */
    Y = 0x002F;  /* 007895 LDY */
    S -= 3; F_007627();  /* 007898 JSRL */
    A = rd16(DP(0x04));  /* 00789C LDA */
    A &= 0x00FF;  /* 00789E AND */
    A += 1;  /* 0078A1 INC */
    B = 0x000A;  /* 0078A2 LDA */
    S -= 3; F_00FF8C();  /* 0078A6 JSRL */
    C = 0;  /* 0078AA CLC */
    mflag = 1;  /* 0078AB SEM */
    SETLO(A, adc8(A, 0x30));  /* 0078AC ADC */
    mflag = 0;  /* 0078AE CLM */
    Y = A;  /* 0078AF TAY */
    S -= 3; F_007627();  /* 0078B0 JSRL */
    A = rd16(DP(0x04));  /* 0078B4 LDA */
    A &= 0x00FF;  /* 0078B6 AND */
    A += 1;  /* 0078B9 INC */
    B = 0x000A;  /* 0078BA LDA */
    S -= 3; F_01E350();  /* 0078BE JSRL */
    C = 0;  /* 0078C2 CLC */
    mflag = 1;  /* 0078C3 SEM */
    SETLO(A, adc8(A, 0x30));  /* 0078C4 ADC */
    mflag = 0;  /* 0078C6 CLM */
    Y = A;  /* 0078C7 TAY */
    S -= 3; F_007627();  /* 0078C8 JSRL */
    Y = 0x0020;  /* 0078CC LDY */
    S -= 3; F_007627();  /* 0078CF JSRL */
    A = D;  /* 0078D3 TDA */
    C = 0;  /* 0078D4 CLC */
    A = adc16(A, 0x0006);  /* 0078D5 ADC */
    B = 0x0000;  /* 0078D8 LDA */
    Y = A;  /* 0078DC TAY */
    S -= 3; F_007771();  /* 0078DD JSRL */
    wr16(DP(0x08), A);  /* 0078E1 STA */
    A = D;  /* 0078E3 TDA */
    A += 1;  /* 0078E4 INC */
    A += 1;  /* 0078E5 INC */
    wr16(DP(0x0A), A);  /* 0078E6 STA */
    mflag = 1;  /* 0078E8 SEM */
    wr8(DP(0x0C), (uint8_t)B);  /* 0078E9 STA */
    mflag = 0;  /* 0078EC CLM */
    A = rd16(DP(0x08));  /* 0078ED LDA */
    wr16(DP(0x0D), A);  /* 0078EF STA */
    A = rd16(DP(0x0C));  /* 0078F1 LDA */
    mflag = 1;  /* 0078F3 SEM */
    wr8(DP(0x0F), (uint8_t)A);  /* 0078F4 STA */
    mflag = 0;  /* 0078F6 CLM */
    A = rd16(DP(0x0A));  /* 0078F7 LDA */
    wr16(DP(0x10), A);  /* 0078F9 STA */
    mflag = 1;  /* 0078FB SEM */
    wr8(DP(0x12), 0x00);  /* 0078FC LDM */
    mflag = 0;  /* 0078FF CLM */
    A = 0x000D;  /* 007900 LDA */
    Y = 0x0002;  /* 007903 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 007906 JSRL */
    mflag = 1;  /* 00790A SEM */
    SETLO(B, 0x00);  /* 00790B LDA */
    SETLO(A, rd8(DP(0x02)));  /* 00790E LDA */
    div8(0x0A);  /* 007910 DIV */
    C = 0;  /* 007913 CLC */
    SETLO(A, adc8(A, 0x30));  /* 007914 ADC */
    mflag = 0;  /* 007916 CLM */
    Y = A;  /* 007917 TAY */
    S -= 3; F_007627();  /* 007918 JSRL */
    mflag = 1;  /* 00791C SEM */
    SETLO(B, 0x00);  /* 00791D LDA */
    SETLO(A, rd8(DP(0x02)));  /* 007920 LDA */
    div8(0x0A);  /* 007922 DIV */
    mflag = 0;  /* 007925 CLM */
    push16(B);  /* 007926 PHB */
    A = pull16();  /* 007928 PLA */
    C = 0;  /* 007929 CLC */
    mflag = 1;  /* 00792A SEM */
    SETLO(A, adc8(A, 0x30));  /* 00792B ADC */
    mflag = 0;  /* 00792D CLM */
    Y = A;  /* 00792E TAY */
    S -= 3; F_007627();  /* 00792F JSRL */
    Y = 0x003A;  /* 007933 LDY */
    S -= 3; F_007627();  /* 007936 JSRL */
    mflag = 1;  /* 00793A SEM */
    SETLO(B, 0x00);  /* 00793B LDA */
    SETLO(A, rd8(DP(0x03)));  /* 00793E LDA */
    div8(0x0A);  /* 007940 DIV */
    C = 0;  /* 007943 CLC */
    SETLO(A, adc8(A, 0x30));  /* 007944 ADC */
    mflag = 0;  /* 007946 CLM */
    Y = A;  /* 007947 TAY */
    S -= 3; F_007627();  /* 007948 JSRL */
    mflag = 1;  /* 00794C SEM */
    SETLO(B, 0x00);  /* 00794D LDA */
    SETLO(A, rd8(DP(0x03)));  /* 007950 LDA */
    div8(0x0A);  /* 007952 DIV */
    mflag = 0;  /* 007955 CLM */
    push16(B);  /* 007956 PHB */
    A = pull16();  /* 007958 PLA */
    C = 0;  /* 007959 CLC */
    mflag = 1;  /* 00795A SEM */
    SETLO(A, adc8(A, 0x30));  /* 00795B ADC */
    mflag = 0;  /* 00795D CLM */
    Y = A;  /* 00795E TAY */
    S -= 3; F_007627();  /* 00795F JSRL */
    S -= 3; F_0077F0();  /* 007963 JSRL */
    goto L_007A02;  /* 007967 JMP */
L_00796A: TICK();
    mflag = 0;  /* 00796A CLM */
    A = rd16(DP(0x00));  /* 00796B LDA */
    mflag = 1;  /* 00796D SEM */
    /* 00796E CMP */
    if ((A & 0xFF) != 0x01) goto L_0079AC;  /* 007970 BNE */
    wr8(DP(0x01), 0x00);  /* 007972 LDM */
L_007975: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 007975 LDA */
    /* 007977 CMP */
    if ((A & 0xFF) >= 0x06) goto L_007994;  /* 007979 BCS */
    Y = rd16(DP(0x01));  /* 00797B LDY */
    mflag = 0;  /* 00797D CLM */
    S -= 3; F_0069BB();  /* 00797E JSRL */
    mflag = 0;  /* 007982 CLM */
    Y = A;  /* 007983 TAY */
    S -= 3; F_00769C();  /* 007984 JSRL */
    mflag = 0;  /* 007988 CLM */
    Y = A;  /* 007989 TAY */
    S -= 3; F_007627();  /* 00798A JSRL */
    mflag = 1;  /* 00798E SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 00798F INC */
    goto L_007975;  /* 007991 JMP */
L_007994: TICK();
    mflag = 0;  /* 007994 CLM */
    if ((rd16(ABS(0x023A)) & 0x0008) == 0) goto L_0079A1;  /* 007995 BBC */
    A = 0x0041;  /* 00799B LDA */
    goto L_0079A4;  /* 00799E JMP */
L_0079A1: TICK();
    A = 0x0020;  /* 0079A1 LDA */
L_0079A4: TICK();
    Y = A;  /* 0079A4 TAY */
    S -= 3; F_007627();  /* 0079A5 JSRL */
    goto L_007A02;  /* 0079A9 JMP */
L_0079AC: TICK();
    mflag = 0;  /* 0079AC CLM */
    A = rd16(DP(0x00));  /* 0079AD LDA */
    mflag = 1;  /* 0079AF SEM */
    /* 0079B0 CMP */
    if ((A & 0xFF) != 0x02) goto L_0079D8;  /* 0079B2 BNE */
    wr8(DP(0x01), 0x00);  /* 0079B4 LDM */
L_0079B7: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0079B7 LDA */
    /* 0079B9 CMP */
    if ((A & 0xFF) >= 0x06) goto L_0079D5;  /* 0079BB BCS */
    mflag = 0;  /* 0079BD CLM */
    A &= 0x00FF;  /* 0079BE AND */
    X = A;  /* 0079C1 TAX */
    Y = rd16((ABS(0x02A4) + X));  /* 0079C2 LDY */
    S -= 3; F_00769C();  /* 0079C5 JSRL */
    mflag = 0;  /* 0079C9 CLM */
    Y = A;  /* 0079CA TAY */
    S -= 3; F_007627();  /* 0079CB JSRL */
    mflag = 1;  /* 0079CF SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0079D0 INC */
    goto L_0079B7;  /* 0079D2 JMP */
L_0079D5: TICK();
    goto L_007A02;  /* 0079D5 JMP */
L_0079D8: TICK();
    mflag = 0;  /* 0079D8 CLM */
    A = rd16(DP(0x00));  /* 0079D9 LDA */
    mflag = 1;  /* 0079DB SEM */
    /* 0079DC CMP */
    if ((A & 0xFF) != 0x03) goto L_007A02;  /* 0079DE BNE */
    wr8(DP(0x01), 0x00);  /* 0079E0 LDM */
L_0079E3: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0079E3 LDA */
    /* 0079E5 CMP */
    if ((A & 0xFF) >= 0x06) goto L_007A02;  /* 0079E7 BCS */
    Y = rd16(DP(0x01));  /* 0079E9 LDY */
    mflag = 0;  /* 0079EB CLM */
    S -= 3; F_006A25();  /* 0079EC JSRL */
    mflag = 0;  /* 0079F0 CLM */
    Y = A;  /* 0079F1 TAY */
    S -= 3; F_00769C();  /* 0079F2 JSRL */
    mflag = 0;  /* 0079F6 CLM */
    Y = A;  /* 0079F7 TAY */
    S -= 3; F_007627();  /* 0079F8 JSRL */
    mflag = 1;  /* 0079FC SEM */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0079FD INC */
    goto L_0079E3;  /* 0079FF JMP */
L_007A02: TICK();
    mflag = 0;  /* 007A02 CLM */
    Y = pull16();  /* 007A03 PLY */
    D = pull16();  /* 007A04 PLD */
    S += 3; return;  /* 007A05 RTL */
}

/* 007A06  callers: 005454, 005489, 0057FB, 005896, 005B6D, 00650D, 00675B, 006874 ... */
void F_007A06(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007A06 LDA */
    push16(D);  /* 007A08 PHD */
    D = A;  /* 007A09 TAD */
    C = 0;  /* 007A0A CLC */
    A = adc16(A, 0x0006);  /* 007A0B ADC */
    push16(A);  /* 007A0E PHA */
    if ((rd16(ABS(0x023E)) & 0x0080) == 0) goto L_007A18;  /* 007A0F BBC */
    goto L_007B74;  /* 007A15 JMP */
L_007A18: TICK();
    mflag = 1;  /* 007A18 SEM */
    if ((rd8(ABS(0x0242)) & 0x40) == 0) goto L_007A23;  /* 007A19 BBC */
    SETLO(A, 0x01);  /* 007A1E LDA */
    goto L_007A25;  /* 007A20 JMP */
L_007A23: TICK();
    SETLO(A, 0x00);  /* 007A23 LDA */
L_007A25: TICK();
    wr8(DP(0x01), (uint8_t)A);  /* 007A25 STA */
    wr8(DP(0x00), (uint8_t)A);  /* 007A27 STA */
    if ((rd8(ABS(0x0242)) & 0x80) == 0) goto L_007A33;  /* 007A29 BBC */
    SETLO(A, 0x01);  /* 007A2E LDA */
    goto L_007A35;  /* 007A30 JMP */
L_007A33: TICK();
    SETLO(A, 0x00);  /* 007A33 LDA */
L_007A35: TICK();
    wr8(DP(0x03), (uint8_t)A);  /* 007A35 STA */
    wr8(DP(0x02), (uint8_t)A);  /* 007A37 STA */
    mflag = 0;  /* 007A39 CLM */
    if ((rd16(ABS(0x0242)) & 0x0100) == 0) goto L_007A46;  /* 007A3A BBC */
    A = 0x0001;  /* 007A40 LDA */
    goto L_007A49;  /* 007A43 JMP */
L_007A46: TICK();
    A = 0x0000;  /* 007A46 LDA */
L_007A49: TICK();
    mflag = 1;  /* 007A49 SEM */
    wr8(DP(0x05), (uint8_t)A);  /* 007A4A STA */
    wr8(DP(0x04), (uint8_t)A);  /* 007A4C STA */
    mflag = 0;  /* 007A4E CLM */
    if ((rd16(ABS(0x023C)) & 0x1000) == 0x1000) goto L_007A5B;  /* 007A4F BBS */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0x0001) goto L_007A68;  /* 007A55 BBS */
L_007A5B: TICK();
    mflag = 1;  /* 007A5B SEM */
    wr8(DP(0x00), 0x00);  /* 007A5C LDM */
    wr8(DP(0x02), 0x00);  /* 007A5F LDM */
    wr8(DP(0x04), 0x00);  /* 007A62 LDM */
    goto L_007B14;  /* 007A65 JMP */
L_007A68: TICK();
    mflag = 1;  /* 007A68 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_007A74;  /* 007A69 BBC */
    wr8(DP(0x04), 0x01);  /* 007A6E LDM */
    goto L_007A87;  /* 007A71 JMP */
L_007A74: TICK();
    mflag = 0;  /* 007A74 CLM */
    if ((rd16(ABS(0x023A)) & 0x0100) == 0) goto L_007A81;  /* 007A75 BBC */
    A = 0x0001;  /* 007A7B LDA */
    goto L_007A84;  /* 007A7E JMP */
L_007A81: TICK();
    A = 0x0000;  /* 007A81 LDA */
L_007A84: TICK();
    mflag = 1;  /* 007A84 SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 007A85 STA */
L_007A87: TICK();
    mflag = 0;  /* 007A87 CLM */
    if ((rd16(ABS(0x023E)) & 0x0010) == 0x0010) goto L_007A98;  /* 007A88 BBS */
    mflag = 1;  /* 007A8E SEM */
    wr8(DP(0x00), 0x01);  /* 007A8F LDM */
    wr8(DP(0x02), 0x00);  /* 007A92 LDM */
    goto L_007B14;  /* 007A95 JMP */
L_007A98: TICK();
    mflag = 1;  /* 007A98 SEM */
    wr8(DP(0x00), 0x00);  /* 007A99 LDM */
    if ((rd8(ABS(0x0246)) & 0x02) == 0) goto L_007ADE;  /* 007A9C BBC */
    SETLO(A, rd8(ABS(0x0228)));  /* 007AA1 LDA */
    /* 007AA4 CMP */
    if ((A & 0xFF) == 0x03) goto L_007AAE;  /* 007AA6 BEQ */
    wr8(DP(0x02), 0x00);  /* 007AA8 LDM */
    goto L_007ADB;  /* 007AAB JMP */
L_007AAE: TICK();
    mflag = 1;  /* 007AAE SEM */
    SETLO(A, rd8(ABS(0x03A8)));  /* 007AAF LDA */
    if ((A & 0xFF) == 0) goto L_007ABA;  /* 007AB2 BEQ */
    wr8(DP(0x02), 0x01);  /* 007AB4 LDM */
    goto L_007ADB;  /* 007AB7 JMP */
L_007ABA: TICK();
    mflag = 1;  /* 007ABA SEM */
    if ((rd8(ABS(0x023C)) & 0x20) == 0) goto L_007ADB;  /* 007ABB BBC */
    SETLO(A, rd8(ABS(0x022F)));  /* 007AC0 LDA */
    if ((A & 0xFF) != 0) goto L_007ADB;  /* 007AC3 BNE */
    SETLO(A, rd8(DP(0x03)));  /* 007AC5 LDA */
    if ((A & 0xFF) == 0) goto L_007AD3;  /* 007AC7 BEQ */
    wr8(ABS(0x022F), 0x10);  /* 007AC9 LDM */
    wr8(DP(0x02), 0x00);  /* 007ACD LDM */
    goto L_007ADB;  /* 007AD0 JMP */
L_007AD3: TICK();
    mflag = 1;  /* 007AD3 SEM */
    wr8(ABS(0x022F), 0x02);  /* 007AD4 LDM */
    wr8(DP(0x02), 0x01);  /* 007AD8 LDM */
L_007ADB: TICK();
    goto L_007B14;  /* 007ADB JMP */
L_007ADE: TICK();
    mflag = 1;  /* 007ADE SEM */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_007AEA;  /* 007ADF BBC */
    wr8(DP(0x02), 0x01);  /* 007AE4 LDM */
    goto L_007B14;  /* 007AE7 JMP */
L_007AEA: TICK();
    mflag = 0;  /* 007AEA CLM */
    if ((rd16(ABS(0x023C)) & 0x0001) == 0x0001) goto L_007AF8;  /* 007AEB BBS */
    mflag = 1;  /* 007AF1 SEM */
    wr8(DP(0x02), 0x00);  /* 007AF2 LDM */
    goto L_007B14;  /* 007AF5 JMP */
L_007AF8: TICK();
    mflag = 1;  /* 007AF8 SEM */
    SETLO(A, rd8(ABS(0x022F)));  /* 007AF9 LDA */
    if ((A & 0xFF) != 0) goto L_007B14;  /* 007AFC BNE */
    SETLO(A, rd8(DP(0x03)));  /* 007AFE LDA */
    if ((A & 0xFF) == 0) goto L_007B0C;  /* 007B00 BEQ */
    wr8(ABS(0x022F), 0x10);  /* 007B02 LDM */
    wr8(DP(0x02), 0x00);  /* 007B06 LDM */
    goto L_007B14;  /* 007B09 JMP */
L_007B0C: TICK();
    mflag = 1;  /* 007B0C SEM */
    wr8(ABS(0x022F), 0x02);  /* 007B0D LDM */
    wr8(DP(0x02), 0x01);  /* 007B11 LDM */
L_007B14: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 007B14 LDA */
    t = rd8(DP(0x01));  /* 007B16 CMP */
    if ((A & 0xFF) != t) goto L_007B26;  /* 007B18 BNE */
    SETLO(A, rd8(DP(0x02)));  /* 007B1A LDA */
    t = rd8(DP(0x03));  /* 007B1C CMP */
    if ((A & 0xFF) != t) goto L_007B26;  /* 007B1E BNE */
    SETLO(A, rd8(DP(0x04)));  /* 007B20 LDA */
    t = rd8(DP(0x05));  /* 007B22 CMP */
    if ((A & 0xFF) == t) goto L_007B74;  /* 007B24 BEQ */
L_007B26: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 007B26 LDA */
    if ((A & 0xFF) == 0) goto L_007B33;  /* 007B28 BEQ */
    mflag = 0;  /* 007B2A CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0040);  /* 007B2B SEB */
    goto L_007B39;  /* 007B30 JMP */
L_007B33: TICK();
    mflag = 0;  /* 007B33 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0040);  /* 007B34 CLB */
L_007B39: TICK();
    mflag = 1;  /* 007B39 SEM */
    SETLO(A, rd8(DP(0x02)));  /* 007B3A LDA */
    if ((A & 0xFF) == 0) goto L_007B47;  /* 007B3C BEQ */
    mflag = 0;  /* 007B3E CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0080);  /* 007B3F SEB */
    goto L_007B4D;  /* 007B44 JMP */
L_007B47: TICK();
    mflag = 0;  /* 007B47 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0080);  /* 007B48 CLB */
L_007B4D: TICK();
    mflag = 1;  /* 007B4D SEM */
    SETLO(A, rd8(DP(0x04)));  /* 007B4E LDA */
    if ((A & 0xFF) == 0) goto L_007B5B;  /* 007B50 BEQ */
    mflag = 0;  /* 007B52 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0100);  /* 007B53 SEB */
    goto L_007B61;  /* 007B58 JMP */
L_007B5B: TICK();
    mflag = 0;  /* 007B5B CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0100);  /* 007B5C CLB */
L_007B61: TICK();
    mflag = 1;  /* 007B61 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_007B6F;  /* 007B62 BBC */
    mflag = 0;  /* 007B67 CLM */
    S -= 3; F_007529();  /* 007B68 JSRL */
    goto L_007B74;  /* 007B6C JMP */
L_007B6F: TICK();
    mflag = 0;  /* 007B6F CLM */
    S -= 3; cbus_write_sel2_00755E();  /* 007B70 JSRL */
L_007B74: TICK();
    mflag = 0;  /* 007B74 CLM */
    Y = pull16();  /* 007B75 PLY */
    D = pull16();  /* 007B76 PLD */
    S += 3; return;  /* 007B77 RTL */
}

/* 007B78  callers: 00429C, 0042F4, 00430B */
void F_007B78(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007B78 LDA */
    push16(D);  /* 007B7A PHD */
    D = A;  /* 007B7B TAD */
    C = 0;  /* 007B7C CLC */
    A = adc16(A, 0x0009);  /* 007B7D ADC */
    push16(A);  /* 007B80 PHA */
    wr16(DP(0x00), Y);  /* 007B81 STY */
    wr16(DP(0x09), 0x0008);  /* 007B83 LDM */
    A = Y;  /* 007B87 TYA */
    A &= 0x00FF;  /* 007B88 AND */
    A -= 1;  /* 007B8B DEC */
    wr16(DP(0x0B), A);  /* 007B8C STA */
    A = D;  /* 007B8E TDA */
    C = 0;  /* 007B8F CLC */
    A = adc16(A, 0x0005);  /* 007B90 ADC */
    B = 0x0000;  /* 007B93 LDA */
    Y = A;  /* 007B97 TAY */
    S -= 3; F_01E30B();  /* 007B98 JSRL */
    wr16(DP(0x09), A);  /* 007B9C STA */
    A = D;  /* 007B9E TDA */
    A += 1;  /* 007B9F INC */
    wr16(DP(0x0B), A);  /* 007BA0 STA */
    mflag = 1;  /* 007BA2 SEM */
    wr8(DP(0x0D), (uint8_t)B);  /* 007BA3 STA */
    mflag = 0;  /* 007BA6 CLM */
    A = rd16(DP(0x09));  /* 007BA7 LDA */
    wr16(DP(0x0E), A);  /* 007BA9 STA */
    A = rd16(DP(0x0D));  /* 007BAB LDA */
    mflag = 1;  /* 007BAD SEM */
    wr8(DP(0x10), (uint8_t)A);  /* 007BAE STA */
    mflag = 0;  /* 007BB0 CLM */
    A = rd16(DP(0x0B));  /* 007BB1 LDA */
    wr16(DP(0x11), A);  /* 007BB3 STA */
    mflag = 1;  /* 007BB5 SEM */
    wr8(DP(0x13), 0x00);  /* 007BB6 LDM */
    mflag = 0;  /* 007BB9 CLM */
    A = 0x000E;  /* 007BBA LDA */
    Y = 0x0004;  /* 007BBD LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 007BC0 JSRL */
    X = rd16(DP(0x03));  /* 007BC4 LDX */
    A = 0x0001;  /* 007BC6 LDA */
    mflag = 1;  /* 007BC9 SEM */
    S -= 3; rt_shl_01E4E0();  /* 007BCA JSRL */
    X = rd16(DP(0x01));  /* 007BCE LDX */
    wr8(DP(0x09), (uint8_t)A);  /* 007BD0 STA */
    SETLO(A, rd8((ABS(0x02D2) + X)));  /* 007BD2 LDA */
    SETLO(A, A | rd8(DP(0x09)));  /* 007BD5 ORA */
    wr8((ABS(0x02D2) + X), (uint8_t)A);  /* 007BD7 STA */
    mflag = 0;  /* 007BDA CLM */
    Y = pull16();  /* 007BDB PLY */
    D = pull16();  /* 007BDC PLD */
    S += 3; return;  /* 007BDD RTL */
}

/* 007BDE  callers: 0042C6 */
void F_007BDE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007BDE LDA */
    push16(D);  /* 007BE0 PHD */
    D = A;  /* 007BE1 TAD */
    C = 0;  /* 007BE2 CLC */
    A = adc16(A, 0x0009);  /* 007BE3 ADC */
    push16(A);  /* 007BE6 PHA */
    wr16(DP(0x00), Y);  /* 007BE7 STY */
    wr16(DP(0x09), 0x0008);  /* 007BE9 LDM */
    A = Y;  /* 007BED TYA */
    A &= 0x00FF;  /* 007BEE AND */
    A -= 1;  /* 007BF1 DEC */
    wr16(DP(0x0B), A);  /* 007BF2 STA */
    A = D;  /* 007BF4 TDA */
    C = 0;  /* 007BF5 CLC */
    A = adc16(A, 0x0005);  /* 007BF6 ADC */
    B = 0x0000;  /* 007BF9 LDA */
    Y = A;  /* 007BFD TAY */
    S -= 3; F_01E30B();  /* 007BFE JSRL */
    wr16(DP(0x09), A);  /* 007C02 STA */
    A = D;  /* 007C04 TDA */
    A += 1;  /* 007C05 INC */
    wr16(DP(0x0B), A);  /* 007C06 STA */
    mflag = 1;  /* 007C08 SEM */
    wr8(DP(0x0D), (uint8_t)B);  /* 007C09 STA */
    mflag = 0;  /* 007C0C CLM */
    A = rd16(DP(0x09));  /* 007C0D LDA */
    wr16(DP(0x0E), A);  /* 007C0F STA */
    A = rd16(DP(0x0D));  /* 007C11 LDA */
    mflag = 1;  /* 007C13 SEM */
    wr8(DP(0x10), (uint8_t)A);  /* 007C14 STA */
    mflag = 0;  /* 007C16 CLM */
    A = rd16(DP(0x0B));  /* 007C17 LDA */
    wr16(DP(0x11), A);  /* 007C19 STA */
    mflag = 1;  /* 007C1B SEM */
    wr8(DP(0x13), 0x00);  /* 007C1C LDM */
    mflag = 0;  /* 007C1F CLM */
    A = 0x000E;  /* 007C20 LDA */
    Y = 0x0004;  /* 007C23 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 007C26 JSRL */
    X = rd16(DP(0x03));  /* 007C2A LDX */
    A = 0x0001;  /* 007C2C LDA */
    mflag = 1;  /* 007C2F SEM */
    S -= 3; rt_shl_01E4E0();  /* 007C30 JSRL */
    SETLO(A, A ^ 0xFF);  /* 007C34 EOR */
    X = rd16(DP(0x01));  /* 007C36 LDX */
    wr8(DP(0x09), (uint8_t)A);  /* 007C38 STA */
    SETLO(A, rd8((ABS(0x02D2) + X)));  /* 007C3A LDA */
    SETLO(A, A & rd8(DP(0x09)));  /* 007C3D AND */
    wr8((ABS(0x02D2) + X), (uint8_t)A);  /* 007C3F STA */
    mflag = 0;  /* 007C42 CLM */
    Y = pull16();  /* 007C43 PLY */
    D = pull16();  /* 007C44 PLD */
    S += 3; return;  /* 007C45 RTL */
}

/* 007C46  callers: 00EBF2 */
void F_007C46(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007C46 LDA */
    push16(D);  /* 007C48 PHD */
    D = A;  /* 007C49 TAD */
    wr16(DP(0x00), 0x0000);  /* 007C4A LDM */
L_007C4E: TICK();
    X = rd16(DP(0x00));  /* 007C4E LDX */
    /* 007C50 CPX */
    if (X >= 0x0005) goto L_007C61;  /* 007C53 BCS */
    mflag = 1;  /* 007C55 SEM */
    wr8((ABS(0x026D) + X), 0x0F);  /* 007C56 LDM */
    X += 1;  /* 007C5A INX */
    wr16(DP(0x00), X);  /* 007C5B STX */
    mflag = 0;  /* 007C5D CLM */
    goto L_007C4E;  /* 007C5E JMP */
L_007C61: TICK();
    mflag = 0;  /* 007C61 CLM */
    wr16(DP(0x00), 0x0000);  /* 007C62 LDM */
L_007C66: TICK();
    X = rd16(DP(0x00));  /* 007C66 LDX */
    /* 007C68 CPX */
    if (X >= 0x0008) goto L_007C79;  /* 007C6B BCS */
    mflag = 1;  /* 007C6D SEM */
    wr8((ABS(0x0264) + X), 0x0F);  /* 007C6E LDM */
    X += 1;  /* 007C72 INX */
    wr16(DP(0x00), X);  /* 007C73 STX */
    mflag = 0;  /* 007C75 CLM */
    goto L_007C66;  /* 007C76 JMP */
L_007C79: TICK();
    mflag = 0;  /* 007C79 CLM */
    A = rd16(ABS(0x0340));  /* 007C7A LDA */
    mflag = 1;  /* 007C7D SEM */
    SETLO(A, A & 0x03);  /* 007C7E AND */
    SETLO(A, A - 1);  /* 007C80 DEC */
    if ((A & 0xFF) != 0) goto L_007C8B;  /* 007C81 BNE */
    mflag = 0;  /* 007C83 CLM */
    wr16(DP(0x02), 0x0064);  /* 007C84 LDM */
    goto L_007CAE;  /* 007C88 JMP */
L_007C8B: TICK();
    mflag = 0;  /* 007C8B CLM */
    A = rd16(ABS(0x0340));  /* 007C8C LDA */
    mflag = 1;  /* 007C8F SEM */
    SETLO(A, A & 0x03);  /* 007C90 AND */
    /* 007C92 CMP */
    if ((A & 0xFF) != 0x02) goto L_007C9E;  /* 007C94 BNE */
    mflag = 0;  /* 007C96 CLM */
    wr16(DP(0x02), 0x0046);  /* 007C97 LDM */
    goto L_007CAE;  /* 007C9B JMP */
L_007C9E: TICK();
    mflag = 0;  /* 007C9E CLM */
    A = rd16(ABS(0x0340));  /* 007C9F LDA */
    mflag = 1;  /* 007CA2 SEM */
    SETLO(A, A & 0x03);  /* 007CA3 AND */
    /* 007CA5 CMP */
    if ((A & 0xFF) != 0x03) goto L_007CAE;  /* 007CA7 BNE */
    mflag = 0;  /* 007CA9 CLM */
    wr16(DP(0x02), 0x0028);  /* 007CAA LDM */
L_007CAE: TICK();
    mflag = 1;  /* 007CAE SEM */
    wr8(ABS(0x026C), 0x00);  /* 007CAF LDM */
    mflag = 0;  /* 007CB3 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x8000);  /* 007CB4 CLB */
    mflag = 1;  /* 007CB9 SEM */
    wr8(ABS(0x039B), 0x0F);  /* 007CBA LDM */
    wr8(ABS(0x039A), 0x00);  /* 007CBE LDM */
    wr8(ABS(0x0399), 0x00);  /* 007CC2 LDM */
    wr8(ABS(0x0263), 0x00);  /* 007CC6 LDM */
    wr8(ABS(TA1IC), 0x04);  /* 007CCA LDM */
    wr8(ABS(TA1MOD), 0x40);  /* 007CCE LDM */
    mflag = 0;  /* 007CD2 CLM */
    A = rd16(DP(0x02));  /* 007CD3 LDA */
    mpy16(0x01F8);  /* 007CD5 MPY */
    A = lsr16(A);  /* 007CD9 LSR */
    A = lsr16(A);  /* 007CDA LSR */
    wr16(ABS(TA1), A);  /* 007CDB STA */
    mflag = 1;  /* 007CDE SEM */
    wr8(ABS(TA1IC), rd8(ABS(TA1IC)) & ~0x08);  /* 007CDF CLB */
    wr8(ABS(TB1IC), 0x05);  /* 007CE3 LDM */
    wr8(ABS(TB1MOD), 0x42);  /* 007CE7 LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x40);  /* 007CEB SEB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x02);  /* 007CEF SEB */
    mflag = 0;  /* 007CF3 CLM */
    D = pull16();  /* 007CF4 PLD */
    S += 3; return;  /* 007CF5 RTL */
}

/* 007CF6  callers: 00EA33, 00EA80, 00EC90 */
void F_007CF6(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 007CF6 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x40);  /* 007CF7 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x02);  /* 007CFB CLB */
    wr8(ABS(TA1IC), 0x00);  /* 007CFF LDM */
    wr8(ABS(TB1IC), 0x00);  /* 007D03 LDM */
    mflag = 0;  /* 007D07 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x8000);  /* 007D08 CLB */
    S += 3; return;  /* 007D0D RTL */
}

/* 007D0E  callers: 007D6B, 007D79, 00F46C, 01BB9D */
void F_007D0E(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 007D0E SEM */
    wr8(ABS(TA3IC), 0x00);  /* 007D0F LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 007D13 CLB */
    wr8(ABS(TA3MOD), 0x40);  /* 007D17 LDM */
    mflag = 0;  /* 007D1B CLM */
    wr16(ABS(TA3), 0x01F7);  /* 007D1C LDM */
    mflag = 1;  /* 007D21 SEM */
    wr8(ABS(TA3IC), 0x05);  /* 007D22 LDM */
    SETLO(A, rd8(ABS(0x022C)));  /* 007D26 LDA */
    wr8(ABS(0x022E), (uint8_t)A);  /* 007D29 STA */
    wr8(ABS(0x022D), (uint8_t)A);  /* 007D2C STA */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 007D2F SEB */
    mflag = 0;  /* 007D33 CLM */
    S += 3; return;  /* 007D34 RTL */
}

/* 007D35  callers: 0189FC, 01BBBE, 01BBDB */
void F_007D35(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 007D35 SEM */
    wr8(ABS(0x01E9), 0x00);  /* 007D36 LDM */
    wr8(ABS(TA3IC), 0x00);  /* 007D3A LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 007D3E CLB */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_007D4C;  /* 007D42 BBC */
    SETLO(A, 0x01);  /* 007D47 LDA */
    goto L_007D4E;  /* 007D49 JMP */
L_007D4C: TICK();
    SETLO(A, 0x09);  /* 007D4C LDA */
L_007D4E: TICK();
    wr8(ABS(TA3MOD), (uint8_t)A);  /* 007D4E STA */
    mflag = 0;  /* 007D51 CLM */
    wr16(ABS(TA3), 0x0000);  /* 007D52 LDM */
    mflag = 1;  /* 007D57 SEM */
    wr8(ABS(TA3IC), 0x05);  /* 007D58 LDM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 007D5C SEB */
    if ((rd8(ABS(0x023E)) & 0x20) == 0) goto L_007D72;  /* 007D60 BBC */
    if ((rd8(ABS(P5)) & 0x80) == 0x80) goto L_007D6F;  /* 007D65 BBS [P57 SUSTURMA] */
    mflag = 0;  /* 007D6A CLM */
    S -= 3; F_007D0E();  /* 007D6B JSRL */
L_007D6F: TICK();
    goto L_007D7D;  /* 007D6F JMP */
L_007D72: TICK();
    mflag = 1;  /* 007D72 SEM */
    if ((rd8(ABS(P5)) & 0x80) == 0) goto L_007D7D;  /* 007D73 BBC [P57 SUSTURMA] */
    mflag = 0;  /* 007D78 CLM */
    S -= 3; F_007D0E();  /* 007D79 JSRL */
L_007D7D: TICK();
    mflag = 0;  /* 007D7D CLM */
    S += 3; return;  /* 007D7E RTL */
}

/* 007D7F  callers: 004DD8, 004DEE, 00537F, 007F4A, 007FFF, 009764, 00A842, 00BC39 ... */
void F_007D7F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007D7F LDA */
    push16(D);  /* 007D81 PHD */
    D = A;  /* 007D82 TAD */
    wr16(DP(0x00), Y);  /* 007D83 STY */
    X = rd16(ABS(TB2));  /* 007D85 LDX */
    wr16(DP(0x02), X);  /* 007D88 STX */
    wr16(DP(0x06), 0x0000);  /* 007D8A LDM */
L_007D8E: TICK();
    X = rd16(DP(0x00));  /* 007D8E LDX */
    N = ((X & 0x8000) != 0);
    Z = (X == 0);
    if (N) goto L_007D94;  /* 007D90 BMI */
    if (!Z) goto L_007D97;  /* 007D92 BNE */
L_007D94: TICK();
    goto L_007E36;  /* 007D94 JMP */
L_007D97: TICK();
    mflag = 1;  /* 007D97 SEM */
    wr8(ABS(WDT), 0x00);  /* 007D98 LDM */
    mflag = 0;  /* 007D9C CLM */
    X = rd16(DP(0x02));  /* 007D9D LDX */
    /* 007D9F CPX */
    if (X >= 0x007F) goto L_007DC3;  /* 007DA2 BCS */
L_007DA4: TICK();
    mflag = 1;  /* 007DA4 SEM */
    wr8(ABS(WDT), 0x00);  /* 007DA5 LDM */
    mflag = 0;  /* 007DA9 CLM */
    X = rd16(ABS(TB2));  /* 007DAA LDX */
    t = rd16(DP(0x02));  /* 007DAD CPX */
    C = (X >= t);
    if (!C) goto L_007DA4;  /* 007DAF BCC */
    A = rd16(DP(0x02));  /* 007DB1 LDA */
    A = adc16(A, 0x0FBE);  /* 007DB3 ADC */
    X = rd16(ABS(TB2));  /* 007DB6 LDX */
    wr16(DP(0x02), X);  /* 007DB9 STX */
    C = 1;  /* 007DBB SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 007DBC SBC */
    wr16(DP(0x04), A);  /* 007DBE STA */
    goto L_007E09;  /* 007DC0 JMP */
L_007DC3: TICK();
    mflag = 0;  /* 007DC3 CLM */
    A = rd16(DP(0x02));  /* 007DC4 LDA */
    C = 0;  /* 007DC6 CLC */
    A = adc16(A, 0xFF82);  /* 007DC7 ADC */
    wr16(DP(0x04), A);  /* 007DCA STA */
L_007DCC: TICK();
    mflag = 1;  /* 007DCC SEM */
    wr8(ABS(WDT), 0x00);  /* 007DCD LDM */
    mflag = 0;  /* 007DD1 CLM */
    X = rd16(DP(0x04));  /* 007DD2 LDX */
    t = rd16(ABS(TB2));  /* 007DD4 CPX */
    if (X >= t) goto L_007DE0;  /* 007DD7 BCS */
    X = rd16(ABS(TB2));  /* 007DD9 LDX */
    t = rd16(DP(0x02));  /* 007DDC CPX */
    if (X < t) goto L_007DCC;  /* 007DDE BCC */
L_007DE0: TICK();
    X = rd16(ABS(TB2));  /* 007DE0 LDX */
    wr16(DP(0x02), X);  /* 007DE3 STX */
    t = rd16(DP(0x04));  /* 007DE5 CPX */
    C = (X >= t);
    Z = (X == t);
    if (!C) goto L_007DFB;  /* 007DE7 BCC */
    if (Z) goto L_007DFB;  /* 007DE9 BEQ */
    A = X;  /* 007DEB TXA */
    A = adc16(A, 0xF040);  /* 007DEC ADC */
    A ^= 0xFFFF;  /* 007DEF EOR */
    A += 1;  /* 007DF2 INC */
    C = 0;  /* 007DF3 CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 007DF4 ADC */
    wr16(DP(0x04), A);  /* 007DF6 STA */
    goto L_007E03;  /* 007DF8 JMP */
L_007DFB: TICK();
    mflag = 0;  /* 007DFB CLM */
    A = rd16(DP(0x04));  /* 007DFC LDA */
    C = 1;  /* 007DFE SEC */
    A = sbc16(A, rd16(DP(0x02)));  /* 007DFF SBC */
    wr16(DP(0x04), A);  /* 007E01 STA */
L_007E03: TICK();
    C = 0;  /* 007E03 CLC */
    A = adc16(A, 0x007E);  /* 007E04 ADC */
    wr16(DP(0x04), A);  /* 007E07 STA */
L_007E09: TICK();
    A = rd16(DP(0x06));  /* 007E09 LDA */
    C = 0;  /* 007E0B CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 007E0C ADC */
    wr16(DP(0x06), A);  /* 007E0E STA */
    /* 007E10 CMP */
    if (A < 0x007E) goto L_007E33;  /* 007E13 BCC */
    B = 0x0000;  /* 007E15 LDA */
    div16(0x007E);  /* 007E19 DIV */
    A ^= 0xFFFF;  /* 007E1D EOR */
    A += 1;  /* 007E20 INC */
    C = 0;  /* 007E21 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 007E22 ADC */
    wr16(DP(0x00), A);  /* 007E24 STA */
    B = 0x0000;  /* 007E26 LDA */
    A = rd16(DP(0x06));  /* 007E2A LDA */
    div16(0x007E);  /* 007E2C DIV */
    wr16(DP(0x06), B);  /* 007E30 STA */
L_007E33: TICK();
    goto L_007D8E;  /* 007E33 JMP */
L_007E36: TICK();
    mflag = 0;  /* 007E36 CLM */
    D = pull16();  /* 007E37 PLD */
    S += 3; return;  /* 007E38 RTL */
}

/* 007E39  callers: 009744, 00974C */
void F_007E39(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007E39 LDA */
    push16(D);  /* 007E3B PHD */
    D = A;  /* 007E3C TAD */
    wr16(DP(0x00), Y);  /* 007E3D STY */
    A = Y;  /* 007E3F TYA */
    mflag = 1;  /* 007E40 SEM */
    SETLO(A, A + 1);  /* 007E41 INC */
    wr8(DP(0x01), (uint8_t)A);  /* 007E42 STA */
    SETLO(A, A - 1);  /* 007E44 DEC */
    wr8(ABS(0x039D), (uint8_t)A);  /* 007E45 STA */
    SETLO(A, rd8(DP(0x01)));  /* 007E48 LDA */
    D = pull16();  /* 007E4A PLD */
    S += 3; return;  /* 007E4B RTL */
}

/* 007E4C  callers: 004C71, 004C87, 007F04, 007F1E, 007F39, 007F61, 007F88, 007FC1 ... */
void cbus_write_sel1_007E4C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007E4C LDA */
    N = ((A & 0x8000) != 0);
    Z = (A == 0);
    push16(D);  /* 007E4E PHD */
    D = A;  /* 007E4F TAD */
    wr16(DP(0x00), Y);  /* 007E50 STY */
    push16(getP());  /* 007E52 PHP */
    mflag = 1;  /* 007E53 SEM */
    SETLO(A, pull8());  /* 007E54 PLA */
    SETLO(A, pull8());  /* 007E55 PLA */
    wr8(DP(0x02), (uint8_t)A);  /* 007E56 STA */
    C = ((A & 0xFF) >= 0x03);  /* 007E58 CMP */
    if (C) goto L_007E62;  /* 007E5A BCS */
    SETLO(A, 0x03);  /* 007E5C LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007E5E PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007E5F STA */
    setP(pull16());  /* 007E61 PLP */
L_007E62: TICK();
    mflag = 0;  /* 007E62 CLM */
    A = D;  /* 007E63 TDA */
    wr16(DP(0x03), A);  /* 007E64 STA */
    mflag = 1;  /* 007E66 SEM */
    wr8(DP(0x05), 0x00);  /* 007E67 LDM */
    wr8(ABS(U1MOD), 0x01);  /* 007E6A LDM */
    wr8(ABS(U1C0), 0x00);  /* 007E6E LDM */
    wr8(ABS(U1BRG), 0x03);  /* 007E72 LDM */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x02);  /* 007E76 CLB [P51 TDSEC1] */
    wr8(ABS(U1C1), 0x05);  /* 007E7A LDM */
L_007E7E: TICK();
    mflag = 0;  /* 007E7E CLM */
    A = rd16(DP(0x03));  /* 007E7F LDA */
    A -= 1;  /* 007E81 DEC */
    A -= 1;  /* 007E82 DEC */
    wr16(DP(0x03), A);  /* 007E83 STA */
    wr16(DP(0x08), A);  /* 007E85 STA */
    A = rd16(DP(0x05));  /* 007E87 LDA */
    mflag = 1;  /* 007E89 SEM */
    wr8(DP(0x0A), (uint8_t)A);  /* 007E8A STA */
    mflag = 0;  /* 007E8C CLM */
    A = rd16(LIND(0x08));  /* 007E8D LDA */
    X = A;  /* 007E8F TAX */
    A = rd16((0x002B06 + X));  /* 007E90 LDA */
    wr16(DP(0x06), X);  /* 007E94 STX */
    mflag = 1;  /* 007E96 SEM */
    wr8(ABS(U1TB), (uint8_t)A);  /* 007E97 STA */
L_007E9A: TICK();
    if ((rd8(ABS(U1C1)) & 0x02) == 0x02) goto L_007EA2;  /* 007E9A BBS */
    goto L_007E9A;  /* 007E9F JMP */
L_007EA2: TICK();
    mflag = 1;  /* 007EA2 SEM */
    if ((rd8(ABS(U1C1)) & 0x08) == 0x08) goto L_007EAB;  /* 007EA3 BBS */
    goto L_007EA2;  /* 007EA8 JMP */
L_007EAB: TICK();
    mflag = 1;  /* 007EAB SEM */
    SETLO(A, rd8(ABS(U1RB)));  /* 007EAC LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 007EAF STA */
    t = (rd8(DP(0x00)) - 1) & 0xFF; wr8(DP(0x00), t);  /* 007EB1 DEC */
    SETLO(A, rd8(DP(0x00)));  /* 007EB3 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    if (N) goto L_007EB9;  /* 007EB5 BMI */
    if (!Z) goto L_007E7E;  /* 007EB7 BNE */
L_007EB9: TICK();
    if ((rd8(ABS(U1C0)) & 0x08) == 0x08) goto L_007EC1;  /* 007EB9 BBS */
    goto L_007EB9;  /* 007EBE JMP */
L_007EC1: TICK();
    mflag = 1;  /* 007EC1 SEM */
    wr8(ABS(U1C1), rd8(ABS(U1C1)) & ~0x05);  /* 007EC2 CLB */
    wr8(ABS(P5), rd8(ABS(P5)) | 0x02);  /* 007EC6 SEB [P51 TDSEC1] */
    wr8(ABS(U1MOD), 0x00);  /* 007ECA LDM */
    SETLO(A, rd8(DP(0x02)));  /* 007ECE LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 007ED0 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 007ED1 STA */
    setP(pull16());  /* 007ED3 PLP */
    mflag = 0;  /* 007ED4 CLM */
    A = rd16(DP(0x01));  /* 007ED5 LDA */
    A &= 0x00FF;  /* 007ED7 AND */
    X = A;  /* 007EDA TAX */
    A = rd16((0x002B06 + X));  /* 007EDB LDA */
    mflag = 1;  /* 007EDF SEM */
    D = pull16();  /* 007EE0 PLD */
    S += 3; return;  /* 007EE1 RTL */
}

/* 007EE2  callers: 01863C, 01DAD3 */
void F_007EE2(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007EE2 LDA */
    push16(D);  /* 007EE4 PHD */
    D = A;  /* 007EE5 TAD */
    C = 0;  /* 007EE6 CLC */
    A = adc16(A, 0x0006);  /* 007EE7 ADC */
    push16(A);  /* 007EEA PHA */
    mflag = 1;  /* 007EEB SEM */
    wr8(DP(0x00), 0x01);  /* 007EEC LDM */
    SETLO(A, rd8(ABS(INT2IC)));  /* 007EEF LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 007EF2 STA */
    wr8(ABS(INT2IC), 0x00);  /* 007EF4 LDM */
    mflag = 0;  /* 007EF8 CLM */
    wr16(DP(0x02), 0x0004);  /* 007EF9 LDM */
    wr16(DP(0x04), 0x0040);  /* 007EFD LDM */
    Y = 0x0002;  /* 007F01 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 007F04 JSRL */
    mflag = 0;  /* 007F08 CLM */
    wr16(DP(0x02), 0x003F);  /* 007F09 LDM */
    wr16(DP(0x04), 0x0004);  /* 007F0D LDM */
    wr16(DP(0x06), 0x0060);  /* 007F11 LDM */
    Y = 0x0003;  /* 007F15 LDY */
    A = D;  /* 007F18 TDA */
    C = 0;  /* 007F19 CLC */
    A = adc16(A, 0x0008);  /* 007F1A ADC */
    push16(A);  /* 007F1D PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 007F1E JSRL */
    Y = pull16();  /* 007F22 PLY */
    mflag = 0;  /* 007F23 CLM */
    wr16(DP(0x02), 0x0000);  /* 007F24 LDM */
    wr16(DP(0x04), 0x0000);  /* 007F28 LDM */
    wr16(DP(0x06), 0x0062);  /* 007F2C LDM */
    Y = 0x0003;  /* 007F30 LDY */
    A = D;  /* 007F33 TDA */
    C = 0;  /* 007F34 CLC */
    A = adc16(A, 0x0008);  /* 007F35 ADC */
    push16(A);  /* 007F38 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 007F39 JSRL */
    Y = pull16();  /* 007F3D PLY */
    wr8(ABS(INT2IC), rd8(ABS(INT2IC)) & ~0x08);  /* 007F3E CLB */
    Y = 0x0012;  /* 007F42 LDY */
    mflag = 0;  /* 007F45 CLM */
    A = D;  /* 007F46 TDA */
    A += 1;  /* 007F47 INC */
    A += 1;  /* 007F48 INC */
    push16(A);  /* 007F49 PHA */
    S -= 3; F_007D7F();  /* 007F4A JSRL */
    Y = pull16();  /* 007F4E PLY */
    mflag = 1;  /* 007F4F SEM */
    if ((rd8(ABS(INT2IC)) & 0x08) == 0) goto L_007F6E;  /* 007F50 BBC */
    mflag = 0;  /* 007F55 CLM */
    wr16(DP(0x02), 0x0000);  /* 007F56 LDM */
    wr16(DP(0x04), 0x0061);  /* 007F5A LDM */
    Y = 0x0002;  /* 007F5E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 007F61 JSRL */
    SETLO(A, A & 0x40);  /* 007F65 AND */
    if ((A & 0xFF) != 0) goto L_007F6B;  /* 007F67 BNE */
    wr8(DP(0x00), (uint8_t)A);  /* 007F69 STA */
L_007F6B: TICK();
    goto L_007F72;  /* 007F6B JMP */
L_007F6E: TICK();
    mflag = 1;  /* 007F6E SEM */
    wr8(DP(0x00), 0x00);  /* 007F6F LDM */
L_007F72: TICK();
    mflag = 0;  /* 007F72 CLM */
    wr16(DP(0x02), 0x0010);  /* 007F73 LDM */
    wr16(DP(0x04), 0x0058);  /* 007F77 LDM */
    wr16(DP(0x06), 0x0060);  /* 007F7B LDM */
    Y = 0x0003;  /* 007F7F LDY */
    A = D;  /* 007F82 TDA */
    C = 0;  /* 007F83 CLC */
    A = adc16(A, 0x0008);  /* 007F84 ADC */
    push16(A);  /* 007F87 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 007F88 JSRL */
    Y = pull16();  /* 007F8C PLY */
    wr8(ABS(INT2IC), rd8(ABS(INT2IC)) & ~0x08);  /* 007F8D CLB */
    SETLO(A, rd8(DP(0x01)));  /* 007F91 LDA */
    wr8(ABS(INT2IC), (uint8_t)A);  /* 007F93 STA */
    SETLO(A, rd8(DP(0x00)));  /* 007F96 LDA */
    Y = pull16();  /* 007F98 PLY */
    D = pull16();  /* 007F99 PLD */
    S += 3; return;  /* 007F9A RTL */
}

/* 007F9B  callers: 01328C, 016944, 018645 */
void F_007F9B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 007F9B LDA */
    push16(D);  /* 007F9D PHD */
    D = A;  /* 007F9E TAD */
    C = 0;  /* 007F9F CLC */
    A = adc16(A, 0x0008);  /* 007FA0 ADC */
    push16(A);  /* 007FA3 PHA */
    mflag = 1;  /* 007FA4 SEM */
    wr8(DP(0x00), 0x01);  /* 007FA5 LDM */
    SETLO(A, rd8(ABS(INT2IC)));  /* 007FA8 LDA */
    wr8(DP(0x01), (uint8_t)A);  /* 007FAB STA */
    wr8(ABS(INT2IC), 0x00);  /* 007FAD LDM */
    mflag = 0;  /* 007FB1 CLM */
    wr16(DP(0x02), 0x0010);  /* 007FB2 LDM */
    wr16(DP(0x04), 0x0058);  /* 007FB6 LDM */
    wr16(DP(0x06), 0x0060);  /* 007FBA LDM */
    Y = 0x0003;  /* 007FBE LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 007FC1 JSRL */
    mflag = 0;  /* 007FC5 CLM */
    wr16(DP(0x02), 0x0009);  /* 007FC6 LDM */
    wr16(DP(0x04), 0x0040);  /* 007FCA LDM */
    Y = 0x0002;  /* 007FCE LDY */
    A = D;  /* 007FD1 TDA */
    C = 0;  /* 007FD2 CLC */
    A = adc16(A, 0x0006);  /* 007FD3 ADC */
    push16(A);  /* 007FD6 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 007FD7 JSRL */
    Y = pull16();  /* 007FDB PLY */
    mflag = 0;  /* 007FDC CLM */
    wr16(DP(0x02), 0x00AA);  /* 007FDD LDM */
    wr16(DP(0x04), 0x0043);  /* 007FE1 LDM */
    Y = 0x0002;  /* 007FE5 LDY */
    A = D;  /* 007FE8 TDA */
    C = 0;  /* 007FE9 CLC */
    A = adc16(A, 0x0006);  /* 007FEA ADC */
    push16(A);  /* 007FED PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 007FEE JSRL */
    Y = pull16();  /* 007FF2 PLY */
    wr8(ABS(INT2IC), rd8(ABS(INT2IC)) & ~0x08);  /* 007FF3 CLB */
    Y = 0x0008;  /* 007FF7 LDY */
    mflag = 0;  /* 007FFA CLM */
    A = D;  /* 007FFB TDA */
    A += 1;  /* 007FFC INC */
    A += 1;  /* 007FFD INC */
    push16(A);  /* 007FFE PHA */
    S -= 3; F_007D7F();  /* 007FFF JSRL */
    Y = pull16();  /* 008003 PLY */
    mflag = 1;  /* 008004 SEM */
    if ((rd8(ABS(INT2IC)) & 0x08) == 0) goto L_00802A;  /* 008005 BBC */
    mflag = 0;  /* 00800A CLM */
    wr16(DP(0x02), 0x0000);  /* 00800B LDM */
    wr16(DP(0x04), 0x0041);  /* 00800F LDM */
    Y = 0x0002;  /* 008013 LDY */
    A = D;  /* 008016 TDA */
    C = 0;  /* 008017 CLC */
    A = adc16(A, 0x0006);  /* 008018 ADC */
    push16(A);  /* 00801B PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 00801C JSRL */
    Y = pull16();  /* 008020 PLY */
    SETLO(A, A & 0x04);  /* 008021 AND */
    if ((A & 0xFF) != 0) goto L_008027;  /* 008023 BNE */
    wr8(DP(0x00), (uint8_t)A);  /* 008025 STA */
L_008027: TICK();
    goto L_00802E;  /* 008027 JMP */
L_00802A: TICK();
    mflag = 1;  /* 00802A SEM */
    wr8(DP(0x00), 0x00);  /* 00802B LDM */
L_00802E: TICK();
    mflag = 0;  /* 00802E CLM */
    wr16(DP(0x02), 0x0004);  /* 00802F LDM */
    wr16(DP(0x04), 0x0040);  /* 008033 LDM */
    Y = 0x0002;  /* 008037 LDY */
    A = D;  /* 00803A TDA */
    C = 0;  /* 00803B CLC */
    A = adc16(A, 0x0006);  /* 00803C ADC */
    push16(A);  /* 00803F PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 008040 JSRL */
    Y = pull16();  /* 008044 PLY */
    wr8(ABS(INT2IC), rd8(ABS(INT2IC)) & ~0x08);  /* 008045 CLB */
    SETLO(A, rd8(DP(0x01)));  /* 008049 LDA */
    wr8(ABS(INT2IC), (uint8_t)A);  /* 00804B STA */
    SETLO(A, rd8(DP(0x00)));  /* 00804E LDA */
    Y = pull16();  /* 008050 PLY */
    D = pull16();  /* 008051 PLD */
    S += 3; return;  /* 008052 RTL */
}

/* 008053  callers: 01BB48, 01BB69 */
void F_008053(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008053 LDA */
    push16(A);  /* 008055 PHA */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0002);  /* 008056 CLB */
    mflag = 1;  /* 00805B SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00807C;  /* 00805C BBS */
    mflag = 0;  /* 008061 CLM */
    X = rd16(ABS(0x0214));  /* 008062 LDX */
    if (X != 0) goto L_00807C;  /* 008065 BNE */
    mflag = 1;  /* 008067 SEM */
    if ((rd8(ABS(0x0240)) & 0x01) == 0) goto L_008072;  /* 008068 BBC */
    mflag = 0;  /* 00806D CLM */
    S -= 3; cbus_write_sel2_00755E();  /* 00806E JSRL */
L_008072: TICK();
    mflag = 1;  /* 008072 SEM */
    if ((rd8(ABS(0x0240)) & 0x80) == 0) goto L_00807C;  /* 008073 BBC */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 008078 CLB [P71 GUC_ACMA] */
L_00807C: TICK();
    mflag = 1;  /* 00807C SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x01);  /* 00807D CLB */
    mflag = 0;  /* 008081 CLM */
    Y = pull16();  /* 008082 PLY */
    S += 3; return;  /* 008083 RTL */
}

/* 008084  callers: 01BB71, 01BB7F */
void F_008084(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008084 LDA */
    push16(A);  /* 008086 PHA */
    mflag = 1;  /* 008087 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x01);  /* 008088 SEB */
    mflag = 0;  /* 00808C CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0001);  /* 00808D SEB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0002);  /* 008092 SEB */
    mflag = 1;  /* 008097 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0080A7;  /* 008098 BBS */
    if ((rd8(ABS(0x0240)) & 0x01) == 0) goto L_0080A7;  /* 00809D BBC */
    mflag = 0;  /* 0080A2 CLM */
    S -= 3; cbus_write_sel2_00755E();  /* 0080A3 JSRL */
L_0080A7: TICK();
    mflag = 0;  /* 0080A7 CLM */
    Y = pull16();  /* 0080A8 PLY */
    S += 3; return;  /* 0080A9 RTL */
}

/* 0080AA  callers: 003639, 003996, 003A17, 003A8B, 003B01, 003BAE, 003C0A, 004408 ... */
void F_0080AA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0080AA LDA */
    push16(D);  /* 0080AC PHD */
    D = A;  /* 0080AD TAD */
    A += 1;  /* 0080AE INC */
    push16(A);  /* 0080AF PHA */
    wr16(DP(0x00), Y);  /* 0080B0 STY */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x4000);  /* 0080B2 CLB */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_0080C0;  /* 0080B7 BBC */
    goto L_00814D;  /* 0080BD JMP */
L_0080C0: TICK();
    if ((rd16(ABS(0x023A)) & 0x0001) == 0x0001) goto L_0080C9;  /* 0080C0 BBS */
    goto L_00814D;  /* 0080C6 JMP */
L_0080C9: TICK();
    mflag = 1;  /* 0080C9 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0080E3;  /* 0080CA BBS */
    SETLO(A, rd8(ABS(P7)));  /* 0080CF LDA */
    SETLO(A, A & 0x02);  /* 0080D2 AND */
    SETLO(A, lsr8(A));  /* 0080D4 LSR */
    if (!Z) goto L_0080E3;  /* 0080D5 BNE */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x02);  /* 0080D7 SEB [P71 GUC_ACMA] */
    Y = 0x0001;  /* 0080DB LDY */
    mflag = 0;  /* 0080DE CLM */
    S -= 3; wait_synth_lock_00D3B4();  /* 0080DF JSRL */
L_0080E3: TICK();
    wr8(ABS(TA0MOD), 0x04);  /* 0080E3 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 0080E7 LDA */
    mpy8(0x05);  /* 0080E9 MPY */
    wr8(DP(0x01), (uint8_t)A);  /* 0080EC STA */
    wr8(DP(0x02), (uint8_t)B);  /* 0080EE STA */
    X = rd16(DP(0x01));  /* 0080F1 LDX */
    mflag = 0;  /* 0080F3 CLM */
    wr16(DP(0x03), 0x0000);  /* 0080F4 LDM */
    A = rd16((0x002C06 + X));  /* 0080F8 LDA */
    wr16(DP(0x01), A);  /* 0080FC STA */
    A = 0xC300;  /* 0080FE LDA */
    B = 0x001E;  /* 008101 LDA */
    X = 0x0001;  /* 008105 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 008108 JSRL */
    wr16(ABS(TA0), A);  /* 00810C STA */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0002);  /* 00810F CLB */
    A = rd16(DP(0x00));  /* 008114 LDA */
    mflag = 1;  /* 008116 SEM */
    mpy8(0x05);  /* 008117 MPY */
    wr8(DP(0x01), (uint8_t)A);  /* 00811A STA */
    wr8(DP(0x02), (uint8_t)B);  /* 00811C STA */
    mflag = 0;  /* 00811F CLM */
    A = rd16(DP(0x01));  /* 008120 LDA */
    C = 0;  /* 008122 CLC */
    A = adc16(A, 0x0004);  /* 008123 ADC */
    X = A;  /* 008126 TAX */
    A = rd16((0x002C06 + X));  /* 008127 LDA */
    mflag = 1;  /* 00812B SEM */
    wr8(ABS(0x0234), (uint8_t)A);  /* 00812C STA */
    wr8(ABS(0x0235), 0x01);  /* 00812F LDM */
    SETLO(A, rd8(DP(0x00)));  /* 008133 LDA */
    mpy8(0x05);  /* 008135 MPY */
    wr8(DP(0x01), (uint8_t)A);  /* 008138 STA */
    wr8(DP(0x02), (uint8_t)B);  /* 00813A STA */
    mflag = 0;  /* 00813D CLM */
    A = rd16(DP(0x01));  /* 00813E LDA */
    A += 1;  /* 008140 INC */
    A += 1;  /* 008141 INC */
    X = A;  /* 008142 TAX */
    A = rd16((0x002C06 + X));  /* 008143 LDA */
    wr16(ABS(0x0214), A);  /* 008147 STA */
    goto L_008152;  /* 00814A JMP */
L_00814D: TICK();
    mflag = 1;  /* 00814D SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x01);  /* 00814E CLB */
L_008152: TICK();
    mflag = 0;  /* 008152 CLM */
    Y = pull16();  /* 008153 PLY */
    D = pull16();  /* 008154 PLD */
    S += 3; return;  /* 008155 RTL */
}

/* 008156  callers: 0096F9, 00E47F, 00E4A7 */
void F_008156(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008156 LDA */
    push16(D);  /* 008158 PHD */
    D = A;  /* 008159 TAD */
    A += 1;  /* 00815A INC */
    push16(A);  /* 00815B PHA */
    wr16(DP(0x00), Y);  /* 00815C STY */
    mflag = 1;  /* 00815E SEM */
    SETLO(A, Y);  /* 00815F TYA */
    if ((A & 0xFF) == 0) goto L_008175;  /* 008160 BEQ */
    mflag = 0;  /* 008162 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0100);  /* 008163 SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0040);  /* 008168 SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x0080);  /* 00816D SEB */
    goto L_008185;  /* 008172 JMP */
L_008175: TICK();
    mflag = 0;  /* 008175 CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0100);  /* 008176 CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0040);  /* 00817B CLB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x0080);  /* 008180 CLB */
L_008185: TICK();
    mflag = 1;  /* 008185 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_008193;  /* 008186 BBC */
    mflag = 0;  /* 00818B CLM */
    S -= 3; F_007529();  /* 00818C JSRL */
    goto L_008198;  /* 008190 JMP */
L_008193: TICK();
    mflag = 0;  /* 008193 CLM */
    S -= 3; cbus_write_sel2_00755E();  /* 008194 JSRL */
L_008198: TICK();
    Y = pull16();  /* 008198 PLY */
    D = pull16();  /* 008199 PLD */
    S += 3; return;  /* 00819A RTL */
}

/* 00819B  callers: 00879F, 009703, 00E3F5, 00E42D */
void F_00819B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00819B LDA */
    push16(D);  /* 00819D PHD */
    D = A;  /* 00819E TAD */
    A += 1;  /* 00819F INC */
    A += 1;  /* 0081A0 INC */
    A += 1;  /* 0081A1 INC */
    push16(A);  /* 0081A2 PHA */
    wr16(DP(0x00), Y);  /* 0081A3 STY */
    mflag = 1;  /* 0081A5 SEM */
    SETLO(A, 0x01);  /* 0081A6 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 0081A8 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 0081A9 STA */
    setP(pull16());  /* 0081AB PLP */
    mflag = 0;  /* 0081AC CLM */
    S -= 3; i2c_start_007172();  /* 0081AD JSRL */
    Y = 0x0070;  /* 0081B1 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0081B4 JSRL */
    Y = 0x00E4;  /* 0081B8 LDY */
    mflag = 0;  /* 0081BB CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0081BC JSRL */
    Y = 0x00C8;  /* 0081C0 LDY */
    mflag = 0;  /* 0081C3 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0081C4 JSRL */
    Y = 0x00F0;  /* 0081C8 LDY */
    mflag = 0;  /* 0081CB CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0081CC JSRL */
    Y = 0x0000;  /* 0081D0 LDY */
    mflag = 0;  /* 0081D3 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0081D4 JSRL */
    wr8(DP(0x01), 0x00);  /* 0081D8 LDM */
L_0081DB: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0081DB LDA */
    /* 0081DD CMP */
    if ((A & 0xFF) >= 0x14) goto L_0081F9;  /* 0081DF BCS */
    SETLO(A, rd8(DP(0x00)));  /* 0081E1 LDA */
    if ((A & 0xFF) == 0) goto L_0081EA;  /* 0081E3 BEQ */
    SETLO(A, 0xFF);  /* 0081E5 LDA */
    goto L_0081EC;  /* 0081E7 JMP */
L_0081EA: TICK();
    SETLO(A, 0x00);  /* 0081EA LDA */
L_0081EC: TICK();
    wr8(DP(0x02), (uint8_t)A);  /* 0081EC STA */
    mflag = 0;  /* 0081EE CLM */
    Y = A;  /* 0081EF TAY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 0081F0 JSRL */
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 0081F4 INC */
    goto L_0081DB;  /* 0081F6 JMP */
L_0081F9: TICK();
    mflag = 0;  /* 0081F9 CLM */
    S -= 3; i2c_stop_00715B();  /* 0081FA JSRL */
    mflag = 1;  /* 0081FE SEM */
    SETLO(A, 0x00);  /* 0081FF LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 008201 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 008202 STA */
    setP(pull16());  /* 008204 PLP */
    mflag = 0;  /* 008205 CLM */
    Y = pull16();  /* 008206 PLY */
    D = pull16();  /* 008207 PLD */
    S += 3; return;  /* 008208 RTL */
}

/* 008209  callers: 008309, 00831C, 008354 */
void F_008209(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008209 LDA */
    push16(D);  /* 00820B PHD */
    D = A;  /* 00820C TAD */
    wr16(DP(0x00), Y);  /* 00820D STY */
    if ((rd16(ABS(0x023C)) & 0x0100) == 0) goto L_008251;  /* 00820F BBC */
    mflag = 1;  /* 008215 SEM */
    SETLO(B, 0x00);  /* 008216 LDA */
    SETLO(A, Y);  /* 008219 TYA */
    div8(0x0A);  /* 00821A DIV */
    mflag = 0;  /* 00821D CLM */
    push16(B);  /* 00821E PHB */
    A = pull16();  /* 008220 PLA */
    A &= 0x00FF;  /* 008221 AND */
    X = A;  /* 008224 TAX */
    A = rd16((0x002D62 + X));  /* 008225 LDA */
    A &= 0x00FF;  /* 008229 AND */
    mflag = 1;  /* 00822C SEM */
    SETLO(B, 0x00);  /* 00822D LDA */
    mflag = 0;  /* 008230 CLM */
    wr16(DP(0x01), A);  /* 008231 STA */
    A = Y;  /* 008233 TYA */
    mflag = 1;  /* 008234 SEM */
    div8(0x0A);  /* 008235 DIV */
    mflag = 0;  /* 008238 CLM */
    A &= 0x00FF;  /* 008239 AND */
    X = A;  /* 00823C TAX */
    A = rd16((0x002D62 + X));  /* 00823D LDA */
    A &= 0x00FF;  /* 008241 AND */
    A = rla16(A, 0x0008);  /* 008244 RLA */
    A &= 0xFF00;  /* 008248 AND */
    C = 0;  /* 00824B CLC */
    A = adc16(A, rd16(DP(0x01)));  /* 00824C ADC */
    goto L_00828A;  /* 00824E JMP */
L_008251: TICK();
    mflag = 1;  /* 008251 SEM */
    SETLO(B, 0x00);  /* 008252 LDA */
    SETLO(A, Y);  /* 008255 TYA */
    div8(0x0A);  /* 008256 DIV */
    mflag = 0;  /* 008259 CLM */
    A &= 0x00FF;  /* 00825A AND */
    X = A;  /* 00825D TAX */
    A = rd16((0x002D00 + X));  /* 00825E LDA */
    A &= 0x00FF;  /* 008262 AND */
    mflag = 1;  /* 008265 SEM */
    SETLO(B, 0x00);  /* 008266 LDA */
    mflag = 0;  /* 008269 CLM */
    wr16(DP(0x01), A);  /* 00826A STA */
    A = Y;  /* 00826C TYA */
    mflag = 1;  /* 00826D SEM */
    div8(0x0A);  /* 00826E DIV */
    mflag = 0;  /* 008271 CLM */
    push16(B);  /* 008272 PHB */
    A = pull16();  /* 008274 PLA */
    A &= 0x00FF;  /* 008275 AND */
    X = A;  /* 008278 TAX */
    A = rd16((0x002D00 + X));  /* 008279 LDA */
    A &= 0x00FF;  /* 00827D AND */
    A = rla16(A, 0x0008);  /* 008280 RLA */
    A &= 0xFF00;  /* 008284 AND */
    C = 0;  /* 008287 CLC */
    A = adc16(A, rd16(DP(0x01)));  /* 008288 ADC */
L_00828A: TICK();
    D = pull16();  /* 00828A PLD */
    S += 3; return;  /* 00828B RTL */
}

/* 00828C  callers: 005445, 0057EC, 005887, 005B5E, 00629D, 0064FE, 00666B, 006739 ... */
void F_00828C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00828C LDA */
    push16(D);  /* 00828E PHD */
    D = A;  /* 00828F TAD */
    C = 0;  /* 008290 CLC */
    A = adc16(A, 0x0018);  /* 008291 ADC */
    push16(A);  /* 008294 PHA */
    if ((rd16(ABS(0x0246)) & 0x0800) == 0) goto L_00829E;  /* 008295 BBC */
    goto L_0087A3;  /* 00829B JMP */
L_00829E: TICK();
    mflag = 1;  /* 00829E SEM */
    if ((rd8(ABS(0x023A)) & 0x01) == 0x01) goto L_0082A7;  /* 00829F BBS */
    goto L_00879B;  /* 0082A4 JMP */
L_0082A7: TICK();
    mflag = 0;  /* 0082A7 CLM */
    wr16(DP(0x02), 0xF000);  /* 0082A8 LDM */
    if ((rd16(ABS(0x023C)) & 0x0400) == 0) goto L_0082B6;  /* 0082AC BBC */
    wr16(DP(0x02), rd16(DP(0x02)) | 0x0100);  /* 0082B2 SEB */
L_0082B6: TICK();
    mflag = 1;  /* 0082B6 SEM */
    if ((rd8(ABS(0x023C)) & 0x80) == 0) goto L_0082C0;  /* 0082B7 BBC */
    wr8(ABS(0x0259), 0x3C);  /* 0082BC LDM */
L_0082C0: TICK();
    if ((rd8(ABS(0x0240)) & 0x08) == 0) goto L_0082C9;  /* 0082C0 BBC */
    wr8(ABS(0x0258), 0x5E);  /* 0082C5 LDM */
L_0082C9: TICK();
    if ((rd8(ABS(0x0240)) & 0x10) == 0) goto L_0082D2;  /* 0082C9 BBC */
    wr8(ABS(0x0259), 0x76);  /* 0082CE LDM */
L_0082D2: TICK();
    mflag = 0;  /* 0082D2 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0040);  /* 0082D3 CLB */
    wr16(DP(0x06), 0x0000);  /* 0082D8 LDM */
    A = rd16(ABS(0x0228));  /* 0082DC LDA */
    mflag = 1;  /* 0082DF SEM */
    /* 0082E0 CMP */
    if ((A & 0xFF) != 0x0D) goto L_0082EC;  /* 0082E2 BNE */
    mflag = 0;  /* 0082E4 CLM */
    wr16(DP(0x04), 0x0000);  /* 0082E5 LDM */
    goto L_00835A;  /* 0082E9 JMP */
L_0082EC: TICK();
    mflag = 1;  /* 0082EC SEM */
    if ((rd8(ABS(0x0246)) & 0x02) == 0) goto L_008325;  /* 0082ED BBC */
    mflag = 0;  /* 0082F2 CLM */
    wr16(DP(0x04), 0x0000);  /* 0082F3 LDM */
    mflag = 1;  /* 0082F7 SEM */
    if ((rd8(ABS(0x023E)) & 0x10) == 0) goto L_008312;  /* 0082F8 BBC */
    mflag = 0;  /* 0082FD CLM */
    X = rd16(ABS(0x03B6));  /* 0082FE LDX */
    /* 008301 CPX */
    if (X != 0x0004) goto L_00830F;  /* 008304 BNE */
    Y = rd16(ABS(0x0239));  /* 008306 LDY */
    S -= 3; F_008209();  /* 008309 JSRL */
    wr16(DP(0x04), A);  /* 00830D STA */
L_00830F: TICK();
    goto L_008322;  /* 00830F JMP */
L_008312: TICK();
    mflag = 1;  /* 008312 SEM */
    SETLO(A, rd8(ABS(0x03B1)));  /* 008313 LDA */
    if ((A & 0xFF) == 0) goto L_008322;  /* 008316 BEQ */
    Y = rd16(ABS(0x0423));  /* 008318 LDY */
    mflag = 0;  /* 00831B CLM */
    S -= 3; F_008209();  /* 00831C JSRL */
    wr16(DP(0x04), A);  /* 008320 STA */
L_008322: TICK();
    goto L_00835A;  /* 008322 JMP */
L_008325: TICK();
    mflag = 0;  /* 008325 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_008332;  /* 008326 BBC */
    A = rd16(ABS(0x0369));  /* 00832C LDA */
    goto L_008335;  /* 00832F JMP */
L_008332: TICK();
    A = rd16(ABS(0x0376));  /* 008332 LDA */
L_008335: TICK();
    mflag = 1;  /* 008335 SEM */
    wr8(DP(0x00), (uint8_t)A);  /* 008336 STA */
    /* 008338 CMP */
    if ((A & 0xFF) == 0x00) goto L_008349;  /* 00833A BEQ */
    SETLO(A, rd8(ABS(0x0365)));  /* 00833C LDA */
    SETLO(A, A + 1);  /* 00833F INC */
    if ((A & 0xFF) == 0) goto L_008349;  /* 008340 BEQ */
    mflag = 0;  /* 008342 CLM */
    if ((rd16(ABS(0x023C)) & 0x2000) == 0) goto L_008351;  /* 008343 BBC */
L_008349: TICK();
    mflag = 0;  /* 008349 CLM */
    wr16(DP(0x04), 0x4040);  /* 00834A LDM */
    goto L_00835A;  /* 00834E JMP */
L_008351: TICK();
    Y = rd16(DP(0x00));  /* 008351 LDY */
    mflag = 0;  /* 008353 CLM */
    S -= 3; F_008209();  /* 008354 JSRL */
    wr16(DP(0x04), A);  /* 008358 STA */
L_00835A: TICK();
    mflag = 1;  /* 00835A SEM */
    if ((rd8(ABS(0x023C)) & 0x02) == 0) goto L_008365;  /* 00835B BBC */
    mflag = 0;  /* 008360 CLM */
    wr16(DP(0x06), rd16(DP(0x06)) | 0x0010);  /* 008361 SEB */
L_008365: TICK();
    mflag = 1;  /* 008365 SEM */
    wr8(DP(0x00), 0x03);  /* 008366 LDM */
L_008369: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008369 LDA */
    C = ((A & 0xFF) >= 0x0B);  /* 00836B CMP */
    if (!C) goto L_008372;  /* 00836D BCC */
    goto L_0086FB;  /* 00836F JMP */
L_008372: TICK();
    mflag = 0;  /* 008372 CLM */
    if ((rd16(ABS(0x023C)) & 0x0100) == 0x0100) goto L_00837C;  /* 008373 BBS */
    goto L_00853B;  /* 008379 JMP */
L_00837C: TICK();
    A = rd16(DP(0x00));  /* 00837C LDA */
    A &= 0x00FF;  /* 00837E AND */
    A ^= 0xFFFF;  /* 008381 EOR */
    A += 1;  /* 008384 INC */
    A = adc16(A, 0x025C);  /* 008385 ADC */
    X = A;  /* 008388 TAX */
    A = rd16((ABS(0x0000) + X));  /* 008389 LDA */
    mflag = 1;  /* 00838C SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 00838D STA */
    /* 00838F CMP */
    if ((A & 0xFF) >= 0x5B) goto L_0083B8;  /* 008391 BCS */
    /* 008393 CMP */
    if ((A & 0xFF) < 0x41) goto L_0083B8;  /* 008395 BCC */
    mflag = 0;  /* 008397 CLM */
    A &= 0x00FF;  /* 008398 AND */
    A = asl16(A);  /* 00839B ASL */
    C = 0;  /* 00839C CLC */
    A = adc16(A, 0xFF7E);  /* 00839D ADC */
    X = A;  /* 0083A0 TAX */
    A = rd16(DP(0x00));  /* 0083A1 LDA */
    A &= 0x00FF;  /* 0083A3 AND */
    A = asl16(A);  /* 0083A6 ASL */
    wr16(DP(0x18), A);  /* 0083A7 STA */
    A = rd16((0x002D0A + X));  /* 0083A9 LDA */
    wr16(DP(0x1A), A);  /* 0083AD STA */
    X = rd16(DP(0x18));  /* 0083AF LDX */
    A = rd16(DP(0x1A));  /* 0083B1 LDA */
    wr16(DP(0x02 + X), A);  /* 0083B3 STA */
    goto L_008538;  /* 0083B5 JMP */
L_0083B8: TICK();
    mflag = 0;  /* 0083B8 CLM */
    A = rd16(DP(0x01));  /* 0083B9 LDA */
    mflag = 1;  /* 0083BB SEM */
    /* 0083BC CMP */
    if ((A & 0xFF) >= 0x3A) goto L_0083E7;  /* 0083BE BCS */
    SETLO(A, rd8(DP(0x01)));  /* 0083C0 LDA */
    /* 0083C2 CMP */
    if ((A & 0xFF) < 0x30) goto L_0083E7;  /* 0083C4 BCC */
    mflag = 0;  /* 0083C6 CLM */
    A &= 0x00FF;  /* 0083C7 AND */
    A = asl16(A);  /* 0083CA ASL */
    C = 0;  /* 0083CB CLC */
    A = adc16(A, 0xFFA0);  /* 0083CC ADC */
    X = A;  /* 0083CF TAX */
    A = rd16(DP(0x00));  /* 0083D0 LDA */
    A &= 0x00FF;  /* 0083D2 AND */
    A = asl16(A);  /* 0083D5 ASL */
    wr16(DP(0x18), A);  /* 0083D6 STA */
    A = rd16((0x002D3E + X));  /* 0083D8 LDA */
    wr16(DP(0x1A), A);  /* 0083DC STA */
    X = rd16(DP(0x18));  /* 0083DE LDX */
    A = rd16(DP(0x1A));  /* 0083E0 LDA */
    wr16(DP(0x02 + X), A);  /* 0083E2 STA */
    goto L_008538;  /* 0083E4 JMP */
L_0083E7: TICK();
    mflag = 0;  /* 0083E7 CLM */
    A = rd16(DP(0x01));  /* 0083E8 LDA */
    mflag = 1;  /* 0083EA SEM */
    /* 0083EB CMP */
    if ((A & 0xFF) != 0x2A) goto L_008400;  /* 0083ED BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0083EF LDA */
    mflag = 0;  /* 0083F1 CLM */
    A &= 0x00FF;  /* 0083F2 AND */
    A = asl16(A);  /* 0083F5 ASL */
    X = A;  /* 0083F6 TAX */
    A = rd16(0x002CE4);  /* 0083F7 LDA */
    wr16(DP(0x02 + X), A);  /* 0083FB STA */
    goto L_008538;  /* 0083FD JMP */
L_008400: TICK();
    mflag = 0;  /* 008400 CLM */
    A = rd16(DP(0x01));  /* 008401 LDA */
    mflag = 1;  /* 008403 SEM */
    /* 008404 CMP */
    if ((A & 0xFF) != 0x3C) goto L_008419;  /* 008406 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008408 LDA */
    mflag = 0;  /* 00840A CLM */
    A &= 0x00FF;  /* 00840B AND */
    A = asl16(A);  /* 00840E ASL */
    X = A;  /* 00840F TAX */
    A = rd16(0x002D52);  /* 008410 LDA */
    wr16(DP(0x02 + X), A);  /* 008414 STA */
    goto L_008538;  /* 008416 JMP */
L_008419: TICK();
    mflag = 0;  /* 008419 CLM */
    A = rd16(DP(0x01));  /* 00841A LDA */
    mflag = 1;  /* 00841C SEM */
    /* 00841D CMP */
    if ((A & 0xFF) != 0x5F) goto L_008432;  /* 00841F BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008421 LDA */
    mflag = 0;  /* 008423 CLM */
    A &= 0x00FF;  /* 008424 AND */
    A = asl16(A);  /* 008427 ASL */
    X = A;  /* 008428 TAX */
    A = rd16(0x002D54);  /* 008429 LDA */
    wr16(DP(0x02 + X), A);  /* 00842D STA */
    goto L_008538;  /* 00842F JMP */
L_008432: TICK();
    mflag = 0;  /* 008432 CLM */
    A = rd16(DP(0x01));  /* 008433 LDA */
    mflag = 1;  /* 008435 SEM */
    /* 008436 CMP */
    if ((A & 0xFF) != 0x2D) goto L_00844B;  /* 008438 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 00843A LDA */
    mflag = 0;  /* 00843C CLM */
    A &= 0x00FF;  /* 00843D AND */
    A = asl16(A);  /* 008440 ASL */
    X = A;  /* 008441 TAX */
    A = rd16(0x002CEA);  /* 008442 LDA */
    wr16(DP(0x02 + X), A);  /* 008446 STA */
    goto L_008538;  /* 008448 JMP */
L_00844B: TICK();
    mflag = 0;  /* 00844B CLM */
    A = rd16(DP(0x01));  /* 00844C LDA */
    mflag = 1;  /* 00844E SEM */
    /* 00844F CMP */
    if ((A & 0xFF) != 0x3A) goto L_008464;  /* 008451 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008453 LDA */
    mflag = 0;  /* 008455 CLM */
    A &= 0x00FF;  /* 008456 AND */
    A = asl16(A);  /* 008459 ASL */
    X = A;  /* 00845A TAX */
    A = rd16(0x002CEC);  /* 00845B LDA */
    wr16(DP(0x02 + X), A);  /* 00845F STA */
    goto L_008538;  /* 008461 JMP */
L_008464: TICK();
    mflag = 0;  /* 008464 CLM */
    A = rd16(DP(0x01));  /* 008465 LDA */
    mflag = 1;  /* 008467 SEM */
    /* 008468 CMP */
    if ((A & 0xFF) != 0x2F) goto L_00847D;  /* 00846A BNE */
    SETLO(A, rd8(DP(0x00)));  /* 00846C LDA */
    mflag = 0;  /* 00846E CLM */
    A &= 0x00FF;  /* 00846F AND */
    A = asl16(A);  /* 008472 ASL */
    X = A;  /* 008473 TAX */
    A = rd16(0x002CEE);  /* 008474 LDA */
    wr16(DP(0x02 + X), A);  /* 008478 STA */
    goto L_008538;  /* 00847A JMP */
L_00847D: TICK();
    mflag = 0;  /* 00847D CLM */
    A = rd16(DP(0x01));  /* 00847E LDA */
    mflag = 1;  /* 008480 SEM */
    /* 008481 CMP */
    if ((A & 0xFF) != 0x2B) goto L_008496;  /* 008483 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008485 LDA */
    mflag = 0;  /* 008487 CLM */
    A &= 0x00FF;  /* 008488 AND */
    A = asl16(A);  /* 00848B ASL */
    X = A;  /* 00848C TAX */
    A = rd16(0x002CF0);  /* 00848D LDA */
    wr16(DP(0x02 + X), A);  /* 008491 STA */
    goto L_008538;  /* 008493 JMP */
L_008496: TICK();
    mflag = 0;  /* 008496 CLM */
    A = rd16(DP(0x01));  /* 008497 LDA */
    mflag = 1;  /* 008499 SEM */
    /* 00849A CMP */
    if ((A & 0xFF) != 0x5E) goto L_0084AF;  /* 00849C BNE */
    SETLO(A, rd8(DP(0x00)));  /* 00849E LDA */
    mflag = 0;  /* 0084A0 CLM */
    A &= 0x00FF;  /* 0084A1 AND */
    A = asl16(A);  /* 0084A4 ASL */
    X = A;  /* 0084A5 TAX */
    A = rd16(0x002D56);  /* 0084A6 LDA */
    wr16(DP(0x02 + X), A);  /* 0084AA STA */
    goto L_008538;  /* 0084AC JMP */
L_0084AF: TICK();
    mflag = 0;  /* 0084AF CLM */
    A = rd16(DP(0x01));  /* 0084B0 LDA */
    mflag = 1;  /* 0084B2 SEM */
    /* 0084B3 CMP */
    if ((A & 0xFF) != 0x76) goto L_0084C8;  /* 0084B5 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0084B7 LDA */
    mflag = 0;  /* 0084B9 CLM */
    A &= 0x00FF;  /* 0084BA AND */
    A = asl16(A);  /* 0084BD ASL */
    X = A;  /* 0084BE TAX */
    A = rd16(0x002D58);  /* 0084BF LDA */
    wr16(DP(0x02 + X), A);  /* 0084C3 STA */
    goto L_008538;  /* 0084C5 JMP */
L_0084C8: TICK();
    mflag = 0;  /* 0084C8 CLM */
    A = rd16(DP(0x01));  /* 0084C9 LDA */
    mflag = 1;  /* 0084CB SEM */
    /* 0084CC CMP */
    if ((A & 0xFF) != 0x6A) goto L_0084E1;  /* 0084CE BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0084D0 LDA */
    mflag = 0;  /* 0084D2 CLM */
    A &= 0x00FF;  /* 0084D3 AND */
    A = asl16(A);  /* 0084D6 ASL */
    X = A;  /* 0084D7 TAX */
    A = rd16(0x002D5A);  /* 0084D8 LDA */
    wr16(DP(0x02 + X), A);  /* 0084DC STA */
    goto L_008538;  /* 0084DE JMP */
L_0084E1: TICK();
    mflag = 0;  /* 0084E1 CLM */
    A = rd16(DP(0x01));  /* 0084E2 LDA */
    mflag = 1;  /* 0084E4 SEM */
    /* 0084E5 CMP */
    if ((A & 0xFF) != 0x64) goto L_0084FA;  /* 0084E7 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0084E9 LDA */
    mflag = 0;  /* 0084EB CLM */
    A &= 0x00FF;  /* 0084EC AND */
    A = asl16(A);  /* 0084EF ASL */
    X = A;  /* 0084F0 TAX */
    A = rd16(0x002D5C);  /* 0084F1 LDA */
    wr16(DP(0x02 + X), A);  /* 0084F5 STA */
    goto L_008538;  /* 0084F7 JMP */
L_0084FA: TICK();
    mflag = 0;  /* 0084FA CLM */
    A = rd16(DP(0x01));  /* 0084FB LDA */
    mflag = 1;  /* 0084FD SEM */
    /* 0084FE CMP */
    if ((A & 0xFF) != 0x6D) goto L_008513;  /* 008500 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008502 LDA */
    mflag = 0;  /* 008504 CLM */
    A &= 0x00FF;  /* 008505 AND */
    A = asl16(A);  /* 008508 ASL */
    X = A;  /* 008509 TAX */
    A = rd16(0x002D5E);  /* 00850A LDA */
    wr16(DP(0x02 + X), A);  /* 00850E STA */
    goto L_008538;  /* 008510 JMP */
L_008513: TICK();
    mflag = 0;  /* 008513 CLM */
    A = rd16(DP(0x01));  /* 008514 LDA */
    mflag = 1;  /* 008516 SEM */
    /* 008517 CMP */
    if ((A & 0xFF) != 0x65) goto L_00852C;  /* 008519 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 00851B LDA */
    mflag = 0;  /* 00851D CLM */
    A &= 0x00FF;  /* 00851E AND */
    A = asl16(A);  /* 008521 ASL */
    X = A;  /* 008522 TAX */
    A = rd16(0x002D60);  /* 008523 LDA */
    wr16(DP(0x02 + X), A);  /* 008527 STA */
    goto L_008538;  /* 008529 JMP */
L_00852C: TICK();
    mflag = 0;  /* 00852C CLM */
    A = rd16(DP(0x00));  /* 00852D LDA */
    A &= 0x00FF;  /* 00852F AND */
    A = asl16(A);  /* 008532 ASL */
    X = A;  /* 008533 TAX */
    wr16(DP(0x02 + X), 0x0000);  /* 008534 LDM */
L_008538: TICK();
    goto L_0086F5;  /* 008538 JMP */
L_00853B: TICK();
    mflag = 0;  /* 00853B CLM */
    A = rd16(DP(0x00));  /* 00853C LDA */
    A &= 0x00FF;  /* 00853E AND */
    C = 0;  /* 008541 CLC */
    A = adc16(A, 0xFFFD);  /* 008542 ADC */
    X = A;  /* 008545 TAX */
    A = rd16((ABS(0x0252) + X));  /* 008546 LDA */
    mflag = 1;  /* 008549 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 00854A STA */
    /* 00854C CMP */
    if ((A & 0xFF) >= 0x5B) goto L_008575;  /* 00854E BCS */
    /* 008550 CMP */
    if ((A & 0xFF) < 0x41) goto L_008575;  /* 008552 BCC */
    mflag = 0;  /* 008554 CLM */
    A &= 0x00FF;  /* 008555 AND */
    A = asl16(A);  /* 008558 ASL */
    C = 0;  /* 008559 CLC */
    A = adc16(A, 0xFF7E);  /* 00855A ADC */
    X = A;  /* 00855D TAX */
    A = rd16(DP(0x00));  /* 00855E LDA */
    A &= 0x00FF;  /* 008560 AND */
    A = asl16(A);  /* 008563 ASL */
    wr16(DP(0x18), A);  /* 008564 STA */
    A = rd16((0x002C9C + X));  /* 008566 LDA */
    wr16(DP(0x1A), A);  /* 00856A STA */
    X = rd16(DP(0x18));  /* 00856C LDX */
    A = rd16(DP(0x1A));  /* 00856E LDA */
    wr16(DP(0x02 + X), A);  /* 008570 STA */
    goto L_0086F5;  /* 008572 JMP */
L_008575: TICK();
    mflag = 0;  /* 008575 CLM */
    A = rd16(DP(0x01));  /* 008576 LDA */
    mflag = 1;  /* 008578 SEM */
    /* 008579 CMP */
    if ((A & 0xFF) >= 0x3A) goto L_0085A4;  /* 00857B BCS */
    SETLO(A, rd8(DP(0x01)));  /* 00857D LDA */
    /* 00857F CMP */
    if ((A & 0xFF) < 0x30) goto L_0085A4;  /* 008581 BCC */
    mflag = 0;  /* 008583 CLM */
    A &= 0x00FF;  /* 008584 AND */
    A = asl16(A);  /* 008587 ASL */
    C = 0;  /* 008588 CLC */
    A = adc16(A, 0xFFA0);  /* 008589 ADC */
    X = A;  /* 00858C TAX */
    A = rd16(DP(0x00));  /* 00858D LDA */
    A &= 0x00FF;  /* 00858F AND */
    A = asl16(A);  /* 008592 ASL */
    wr16(DP(0x18), A);  /* 008593 STA */
    A = rd16((0x002CD0 + X));  /* 008595 LDA */
    wr16(DP(0x1A), A);  /* 008599 STA */
    X = rd16(DP(0x18));  /* 00859B LDX */
    A = rd16(DP(0x1A));  /* 00859D LDA */
    wr16(DP(0x02 + X), A);  /* 00859F STA */
    goto L_0086F5;  /* 0085A1 JMP */
L_0085A4: TICK();
    mflag = 0;  /* 0085A4 CLM */
    A = rd16(DP(0x01));  /* 0085A5 LDA */
    mflag = 1;  /* 0085A7 SEM */
    /* 0085A8 CMP */
    if ((A & 0xFF) != 0x2A) goto L_0085BD;  /* 0085AA BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0085AC LDA */
    mflag = 0;  /* 0085AE CLM */
    A &= 0x00FF;  /* 0085AF AND */
    A = asl16(A);  /* 0085B2 ASL */
    X = A;  /* 0085B3 TAX */
    A = rd16(0x002CE4);  /* 0085B4 LDA */
    wr16(DP(0x02 + X), A);  /* 0085B8 STA */
    goto L_0086F5;  /* 0085BA JMP */
L_0085BD: TICK();
    mflag = 0;  /* 0085BD CLM */
    A = rd16(DP(0x01));  /* 0085BE LDA */
    mflag = 1;  /* 0085C0 SEM */
    /* 0085C1 CMP */
    if ((A & 0xFF) != 0x3C) goto L_0085D6;  /* 0085C3 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0085C5 LDA */
    mflag = 0;  /* 0085C7 CLM */
    A &= 0x00FF;  /* 0085C8 AND */
    A = asl16(A);  /* 0085CB ASL */
    X = A;  /* 0085CC TAX */
    A = rd16(0x002CE6);  /* 0085CD LDA */
    wr16(DP(0x02 + X), A);  /* 0085D1 STA */
    goto L_0086F5;  /* 0085D3 JMP */
L_0085D6: TICK();
    mflag = 0;  /* 0085D6 CLM */
    A = rd16(DP(0x01));  /* 0085D7 LDA */
    mflag = 1;  /* 0085D9 SEM */
    /* 0085DA CMP */
    if ((A & 0xFF) != 0x5F) goto L_0085EF;  /* 0085DC BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0085DE LDA */
    mflag = 0;  /* 0085E0 CLM */
    A &= 0x00FF;  /* 0085E1 AND */
    A = asl16(A);  /* 0085E4 ASL */
    X = A;  /* 0085E5 TAX */
    A = rd16(0x002CE8);  /* 0085E6 LDA */
    wr16(DP(0x02 + X), A);  /* 0085EA STA */
    goto L_0086F5;  /* 0085EC JMP */
L_0085EF: TICK();
    mflag = 0;  /* 0085EF CLM */
    A = rd16(DP(0x01));  /* 0085F0 LDA */
    mflag = 1;  /* 0085F2 SEM */
    /* 0085F3 CMP */
    if ((A & 0xFF) != 0x2D) goto L_008608;  /* 0085F5 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0085F7 LDA */
    mflag = 0;  /* 0085F9 CLM */
    A &= 0x00FF;  /* 0085FA AND */
    A = asl16(A);  /* 0085FD ASL */
    X = A;  /* 0085FE TAX */
    A = rd16(0x002CEA);  /* 0085FF LDA */
    wr16(DP(0x02 + X), A);  /* 008603 STA */
    goto L_0086F5;  /* 008605 JMP */
L_008608: TICK();
    mflag = 0;  /* 008608 CLM */
    A = rd16(DP(0x01));  /* 008609 LDA */
    mflag = 1;  /* 00860B SEM */
    /* 00860C CMP */
    if ((A & 0xFF) != 0x3A) goto L_008621;  /* 00860E BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008610 LDA */
    mflag = 0;  /* 008612 CLM */
    A &= 0x00FF;  /* 008613 AND */
    A = asl16(A);  /* 008616 ASL */
    X = A;  /* 008617 TAX */
    A = rd16(0x002CEC);  /* 008618 LDA */
    wr16(DP(0x02 + X), A);  /* 00861C STA */
    goto L_0086F5;  /* 00861E JMP */
L_008621: TICK();
    mflag = 0;  /* 008621 CLM */
    A = rd16(DP(0x01));  /* 008622 LDA */
    mflag = 1;  /* 008624 SEM */
    /* 008625 CMP */
    if ((A & 0xFF) != 0x2F) goto L_00863A;  /* 008627 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008629 LDA */
    mflag = 0;  /* 00862B CLM */
    A &= 0x00FF;  /* 00862C AND */
    A = asl16(A);  /* 00862F ASL */
    X = A;  /* 008630 TAX */
    A = rd16(0x002CEE);  /* 008631 LDA */
    wr16(DP(0x02 + X), A);  /* 008635 STA */
    goto L_0086F5;  /* 008637 JMP */
L_00863A: TICK();
    mflag = 0;  /* 00863A CLM */
    A = rd16(DP(0x01));  /* 00863B LDA */
    mflag = 1;  /* 00863D SEM */
    /* 00863E CMP */
    if ((A & 0xFF) != 0x2B) goto L_008653;  /* 008640 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008642 LDA */
    mflag = 0;  /* 008644 CLM */
    A &= 0x00FF;  /* 008645 AND */
    A = asl16(A);  /* 008648 ASL */
    X = A;  /* 008649 TAX */
    A = rd16(0x002CF0);  /* 00864A LDA */
    wr16(DP(0x02 + X), A);  /* 00864E STA */
    goto L_0086F5;  /* 008650 JMP */
L_008653: TICK();
    mflag = 0;  /* 008653 CLM */
    A = rd16(DP(0x01));  /* 008654 LDA */
    mflag = 1;  /* 008656 SEM */
    /* 008657 CMP */
    if ((A & 0xFF) != 0x5E) goto L_00866C;  /* 008659 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 00865B LDA */
    mflag = 0;  /* 00865D CLM */
    A &= 0x00FF;  /* 00865E AND */
    A = asl16(A);  /* 008661 ASL */
    X = A;  /* 008662 TAX */
    A = rd16(0x002CF4);  /* 008663 LDA */
    wr16(DP(0x02 + X), A);  /* 008667 STA */
    goto L_0086F5;  /* 008669 JMP */
L_00866C: TICK();
    mflag = 0;  /* 00866C CLM */
    A = rd16(DP(0x01));  /* 00866D LDA */
    mflag = 1;  /* 00866F SEM */
    /* 008670 CMP */
    if ((A & 0xFF) != 0x76) goto L_008685;  /* 008672 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 008674 LDA */
    mflag = 0;  /* 008676 CLM */
    A &= 0x00FF;  /* 008677 AND */
    A = asl16(A);  /* 00867A ASL */
    X = A;  /* 00867B TAX */
    A = rd16(0x002CF6);  /* 00867C LDA */
    wr16(DP(0x02 + X), A);  /* 008680 STA */
    goto L_0086F5;  /* 008682 JMP */
L_008685: TICK();
    mflag = 0;  /* 008685 CLM */
    A = rd16(DP(0x01));  /* 008686 LDA */
    mflag = 1;  /* 008688 SEM */
    /* 008689 CMP */
    if ((A & 0xFF) != 0x6A) goto L_00869E;  /* 00868B BNE */
    SETLO(A, rd8(DP(0x00)));  /* 00868D LDA */
    mflag = 0;  /* 00868F CLM */
    A &= 0x00FF;  /* 008690 AND */
    A = asl16(A);  /* 008693 ASL */
    X = A;  /* 008694 TAX */
    A = rd16(0x002CF8);  /* 008695 LDA */
    wr16(DP(0x02 + X), A);  /* 008699 STA */
    goto L_0086F5;  /* 00869B JMP */
L_00869E: TICK();
    mflag = 0;  /* 00869E CLM */
    A = rd16(DP(0x01));  /* 00869F LDA */
    mflag = 1;  /* 0086A1 SEM */
    /* 0086A2 CMP */
    if ((A & 0xFF) != 0x64) goto L_0086B7;  /* 0086A4 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0086A6 LDA */
    mflag = 0;  /* 0086A8 CLM */
    A &= 0x00FF;  /* 0086A9 AND */
    A = asl16(A);  /* 0086AC ASL */
    X = A;  /* 0086AD TAX */
    A = rd16(0x002CFA);  /* 0086AE LDA */
    wr16(DP(0x02 + X), A);  /* 0086B2 STA */
    goto L_0086F5;  /* 0086B4 JMP */
L_0086B7: TICK();
    mflag = 0;  /* 0086B7 CLM */
    A = rd16(DP(0x01));  /* 0086B8 LDA */
    mflag = 1;  /* 0086BA SEM */
    /* 0086BB CMP */
    if ((A & 0xFF) != 0x6D) goto L_0086D0;  /* 0086BD BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0086BF LDA */
    mflag = 0;  /* 0086C1 CLM */
    A &= 0x00FF;  /* 0086C2 AND */
    A = asl16(A);  /* 0086C5 ASL */
    X = A;  /* 0086C6 TAX */
    A = rd16(0x002CFC);  /* 0086C7 LDA */
    wr16(DP(0x02 + X), A);  /* 0086CB STA */
    goto L_0086F5;  /* 0086CD JMP */
L_0086D0: TICK();
    mflag = 0;  /* 0086D0 CLM */
    A = rd16(DP(0x01));  /* 0086D1 LDA */
    mflag = 1;  /* 0086D3 SEM */
    /* 0086D4 CMP */
    if ((A & 0xFF) != 0x65) goto L_0086E9;  /* 0086D6 BNE */
    SETLO(A, rd8(DP(0x00)));  /* 0086D8 LDA */
    mflag = 0;  /* 0086DA CLM */
    A &= 0x00FF;  /* 0086DB AND */
    A = asl16(A);  /* 0086DE ASL */
    X = A;  /* 0086DF TAX */
    A = rd16(0x002CFE);  /* 0086E0 LDA */
    wr16(DP(0x02 + X), A);  /* 0086E4 STA */
    goto L_0086F5;  /* 0086E6 JMP */
L_0086E9: TICK();
    mflag = 0;  /* 0086E9 CLM */
    A = rd16(DP(0x00));  /* 0086EA LDA */
    A &= 0x00FF;  /* 0086EC AND */
    A = asl16(A);  /* 0086EF ASL */
    X = A;  /* 0086F0 TAX */
    wr16(DP(0x02 + X), 0x0000);  /* 0086F1 LDM */
L_0086F5: TICK();
    mflag = 1;  /* 0086F5 SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 0086F6 INC */
    goto L_008369;  /* 0086F8 JMP */
L_0086FB: TICK();
    mflag = 1;  /* 0086FB SEM */
    if ((rd8(ABS(0x023C)) & 0x04) == 0) goto L_008706;  /* 0086FC BBC */
    mflag = 0;  /* 008701 CLM */
    wr16(DP(0x08), rd16(DP(0x08)) | 0x0001);  /* 008702 SEB */
L_008706: TICK();
    mflag = 1;  /* 008706 SEM */
    if ((rd8(ABS(0x023C)) & 0x01) == 0) goto L_008711;  /* 008707 BBC */
    mflag = 0;  /* 00870C CLM */
    wr16(DP(0x08), rd16(DP(0x08)) | 0x0800);  /* 00870D SEB */
L_008711: TICK();
    mflag = 1;  /* 008711 SEM */
    if ((rd8(ABS(0x023C)) & 0x08) == 0) goto L_00871C;  /* 008712 BBC */
    mflag = 0;  /* 008717 CLM */
    wr16(DP(0x0A), rd16(DP(0x0A)) | 0x0001);  /* 008718 SEB */
L_00871C: TICK();
    mflag = 1;  /* 00871C SEM */
    if ((rd8(ABS(0x023C)) & 0x10) == 0) goto L_008727;  /* 00871D BBC */
    mflag = 0;  /* 008722 CLM */
    wr16(DP(0x0C), rd16(DP(0x0C)) | 0x0001);  /* 008723 SEB */
L_008727: TICK();
    mflag = 1;  /* 008727 SEM */
    if ((rd8(ABS(0x023C)) & 0x20) == 0) goto L_008732;  /* 008728 BBC */
    mflag = 0;  /* 00872D CLM */
    wr16(DP(0x0E), rd16(DP(0x0E)) | 0x0001);  /* 00872E SEB */
L_008732: TICK();
    mflag = 1;  /* 008732 SEM */
    if ((rd8(ABS(0x023C)) & 0x40) == 0) goto L_00873D;  /* 008733 BBC */
    mflag = 0;  /* 008738 CLM */
    wr16(DP(0x10), rd16(DP(0x10)) | 0x0001);  /* 008739 SEB */
L_00873D: TICK();
    mflag = 1;  /* 00873D SEM */
    SETLO(A, 0x01);  /* 00873E LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 008740 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 008741 STA */
    setP(pull16());  /* 008743 PLP */
    mflag = 0;  /* 008744 CLM */
    S -= 3; i2c_start_007172();  /* 008745 JSRL */
    Y = 0x0070;  /* 008749 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00874C JSRL */
    Y = 0x00E4;  /* 008750 LDY */
    mflag = 0;  /* 008753 CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 008754 JSRL */
    Y = 0x00C8;  /* 008758 LDY */
    mflag = 0;  /* 00875B CLM */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 00875C JSRL */
    wr8(DP(0x00), 0x00);  /* 008760 LDM */
L_008763: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008763 LDA */
    /* 008765 CMP */
    if ((A & 0xFF) >= 0x0B) goto L_00878C;  /* 008767 BCS */
    mflag = 0;  /* 008769 CLM */
    A &= 0x00FF;  /* 00876A AND */
    A = asl16(A);  /* 00876D ASL */
    X = A;  /* 00876E TAX */
    A = rd16(DP(0x03 + X));  /* 00876F LDA */
    A &= 0x00FF;  /* 008771 AND */
    Y = A;  /* 008774 TAY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 008775 JSRL */
    SETLO(A, rd8(DP(0x00)));  /* 008779 LDA */
    mflag = 0;  /* 00877B CLM */
    A &= 0x00FF;  /* 00877C AND */
    A = asl16(A);  /* 00877F ASL */
    X = A;  /* 008780 TAX */
    Y = rd16(DP(0x02 + X));  /* 008781 LDY */
    S -= 3; i2c_send_byte_fast_006AC2();  /* 008783 JSRL */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008787 INC */
    goto L_008763;  /* 008789 JMP */
L_00878C: TICK();
    mflag = 0;  /* 00878C CLM */
    S -= 3; i2c_stop_00715B();  /* 00878D JSRL */
    mflag = 1;  /* 008791 SEM */
    SETLO(A, 0x00);  /* 008792 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 008794 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 008795 STA */
    setP(pull16());  /* 008797 PLP */
    goto L_0087A3;  /* 008798 JMP */
L_00879B: TICK();
    Y = 0x0000;  /* 00879B LDY */
    mflag = 0;  /* 00879E CLM */
    S -= 3; F_00819B();  /* 00879F JSRL */
L_0087A3: TICK();
    mflag = 0;  /* 0087A3 CLM */
    Y = pull16();  /* 0087A4 PLY */
    D = pull16();  /* 0087A5 PLD */
    S += 3; return;  /* 0087A6 RTL */
}

/* 0087A7  callers: 0035A0, 0035FA, 003619, 0036A2, 00443E, 004D6C, 00D627, 00D86B ... */
void channel_is_programmed_0087A7(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0087A7 LDA */
    push16(D);  /* 0087A9 PHD */
    D = A;  /* 0087AA TAD */
    A += 1;  /* 0087AB INC */
    A += 1;  /* 0087AC INC */
    A += 1;  /* 0087AD INC */
    push16(A);  /* 0087AE PHA */
    wr16(DP(0x00), Y);  /* 0087AF STY */
    A = Y;  /* 0087B1 TYA */
    A &= 0x00FF;  /* 0087B2 AND */
    A -= 1;  /* 0087B5 DEC */
    mpy16(0x000D);  /* 0087B6 MPY */
    C = 0;  /* 0087BA CLC */
    A = adc16(A, 0x0030);  /* 0087BB ADC */
    Y = A;  /* 0087BE TAY */
    S -= 3; eeprom_read_word_0071F0();  /* 0087BF JSRL */
    wr16(DP(0x01), A);  /* 0087C3 STA */
    A &= 0x3FFF;  /* 0087C5 AND */
    /* 0087C8 CMP */
    if (A != 0x3FFF) goto L_0087D4;  /* 0087CB BNE */
    wr16(DP(0x01), 0x0000);  /* 0087CD LDM */
    goto L_0087D9;  /* 0087D1 JMP */
L_0087D4: TICK();
    mflag = 0;  /* 0087D4 CLM */
    wr16(DP(0x01), 0x0001);  /* 0087D5 LDM */
L_0087D9: TICK();
    A = rd16(DP(0x01));  /* 0087D9 LDA */
    Y = pull16();  /* 0087DB PLY */
    D = pull16();  /* 0087DC PLD */
    S += 3; return;  /* 0087DD RTL */
}

/* 0087DE  callers: 008A20, 008B4C, 00A06F, 00A09E, 016D33, 016DD4, 0193F1, 01940E */
void F_0087DE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0087DE LDA */
    push16(D);  /* 0087E0 PHD */
    D = A;  /* 0087E1 TAD */
    A = rd16(ABS(0x020B));  /* 0087E2 LDA */
    A &= 0x00FF;  /* 0087E5 AND */
    wr16(DP(0x08), A);  /* 0087E8 STA */
    wr16(DP(0x0A), 0x0000);  /* 0087EA LDM */
    wr16(DP(0x00), Y);  /* 0087EE STY */
    wr16(DP(0x02), B);  /* 0087F0 STA */
    A = Y;  /* 0087F3 TYA */
    X = 0x0008;  /* 0087F4 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 0087F7 JSRL */
    wr16(DP(0x04), A);  /* 0087FB STA */
    A &= 0x00FF;  /* 0087FD AND */
    X = A;  /* 008800 TAX */
    A = rd16((0x002B06 + X));  /* 008801 LDA */
    A &= 0x00FF;  /* 008805 AND */
    wr16(DP(0x08), A);  /* 008808 STA */
    A = rd16(DP(0x05));  /* 00880A LDA */
    A &= 0x00FF;  /* 00880C AND */
    X = A;  /* 00880F TAX */
    A = rd16((0x002B06 + X));  /* 008810 LDA */
    A &= 0x00FF;  /* 008814 AND */
    A = rla16(A, 0x0008);  /* 008817 RLA */
    A &= 0xFF00;  /* 00881B AND */
    C = 0;  /* 00881E CLC */
    A = adc16(A, rd16(DP(0x08)));  /* 00881F ADC */
    wr16(DP(0x06), A);  /* 008821 STA */
    D = pull16();  /* 008823 PLD */
    S += 3; return;  /* 008824 RTL */
}

/* 008825  callers: 008A2C, 008B58, 00A07B, 00A0AA, 016D3F, 016DE0, 0193FD, 01941A */
void F_008825(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008825 LDA */
    push16(D);  /* 008827 PHD */
    D = A;  /* 008828 TAD */
    A = rd16(ABS(0x020B));  /* 008829 LDA */
    A &= 0x00FF;  /* 00882C AND */
    wr16(DP(0x06), A);  /* 00882F STA */
    wr16(DP(0x08), 0x0000);  /* 008831 LDM */
    wr16(DP(0x00), Y);  /* 008835 STY */
    wr16(DP(0x02), B);  /* 008837 STA */
    A = Y;  /* 00883A TYA */
    X = 0x0006;  /* 00883B LDX */
    S -= 3; rt_lmod_01E214();  /* 00883E JSRL */
    mflag = 1;  /* 008842 SEM */
    SETLO(A, asl8(A));  /* 008843 ASL */
    wr8(DP(0x04), (uint8_t)A);  /* 008844 STA */
    mflag = 0;  /* 008846 CLM */
    A &= 0x00FF;  /* 008847 AND */
    X = A;  /* 00884A TAX */
    A = rd16((0x002B06 + X));  /* 00884B LDA */
    mflag = 1;  /* 00884F SEM */
    wr8(DP(0x05), (uint8_t)A);  /* 008850 STA */
    D = pull16();  /* 008852 PLD */
    S += 3; return;  /* 008853 RTL */
}

/* 008854  callers: 008A36, 008B62, 00A028, 016D49, 016DEA, 0193C5 */
void F_008854(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008854 LDA */
    push16(D);  /* 008856 PHD */
    D = A;  /* 008857 TAD */
    wr16(DP(0x00), Y);  /* 008858 STY */
    A = Y;  /* 00885A TYA */
    A &= 0x00FF;  /* 00885B AND */
    wr16(DP(0x05), A);  /* 00885E STA */
    wr16(DP(0x07), 0x0000);  /* 008860 LDM */
    A = rd16(ABS(0x01F5));  /* 008864 LDA */
    B = rd16(ABS(0x01F7));  /* 008867 LDA */
    X = 0x0005;  /* 00886B LDX */
    S -= 3; rt_ludiv_01E414();  /* 00886E JSRL */
    A = asl16(A);  /* 008872 ASL */
    wr16(DP(0x01), A);  /* 008873 STA */
    A = rd16(ABS(0x020B));  /* 008875 LDA */
    mflag = 1;  /* 008878 SEM */
    /* 008879 CMP */
    if ((A & 0xFF) != 0x80) goto L_008884;  /* 00887B BNE */
    mflag = 0;  /* 00887D CLM */
    A = 0x0001;  /* 00887E LDA */
    goto L_008888;  /* 008881 JMP */
L_008884: TICK();
    mflag = 0;  /* 008884 CLM */
    A = 0x8001;  /* 008885 LDA */
L_008888: TICK();
    A |= rd16(DP(0x01));  /* 008888 ORA */
    wr16(DP(0x01), A);  /* 00888A STA */
    A = rla16(A, 0x0008);  /* 00888C RLA */
    A &= 0x00FF;  /* 008890 AND */
    X = A;  /* 008893 TAX */
    A = rd16((0x002B06 + X));  /* 008894 LDA */
    A &= 0x00FF;  /* 008898 AND */
    wr16(DP(0x05), A);  /* 00889B STA */
    A = rd16(DP(0x01));  /* 00889D LDA */
    A &= 0x00FF;  /* 00889F AND */
    X = A;  /* 0088A2 TAX */
    A = rd16((0x002B06 + X));  /* 0088A3 LDA */
    A &= 0x00FF;  /* 0088A7 AND */
    A = rla16(A, 0x0008);  /* 0088AA RLA */
    A &= 0xFF00;  /* 0088AE AND */
    C = 0;  /* 0088B1 CLC */
    A = adc16(A, rd16(DP(0x05)));  /* 0088B2 ADC */
    wr16(DP(0x03), A);  /* 0088B4 STA */
    D = pull16();  /* 0088B6 PLD */
    S += 3; return;  /* 0088B7 RTL */
}

/* 0088B8  callers: 0089EC, 008A9C, 008B18, 016CFF, 016DA0 */
void F_0088B8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0088B8 LDA */
    push16(D);  /* 0088BA PHD */
    D = A;  /* 0088BB TAD */
    wr16(DP(0x04), Y);  /* 0088BC STY */
    wr16(DP(0x06), B);  /* 0088BE STA */
    A = rd16(ABS(0x0224));  /* 0088C1 LDA */
    mpy16(0x000A);  /* 0088C4 MPY */
    wr16(DP(0x0C), A);  /* 0088C8 STA */
    wr16(DP(0x0E), B);  /* 0088CA STA */
    A = rd16(DP(0x03));  /* 0088CD LDA */
    mflag = 1;  /* 0088CF SEM */
    SETLO(A, A & 0x3F);  /* 0088D0 AND */
    mflag = 0;  /* 0088D2 CLM */
    A &= 0x00FF;  /* 0088D3 AND */
    A = rla16(A, 0x0008);  /* 0088D6 RLA */
    A &= 0xFF00;  /* 0088DA AND */
    wr16(DP(0x12), A);  /* 0088DD STA */
    A = rd16(DP(0x02));  /* 0088DF LDA */
    A &= 0x00FF;  /* 0088E1 AND */
    C = 0;  /* 0088E4 CLC */
    A = adc16(A, rd16(DP(0x12)));  /* 0088E5 ADC */
    wr16(DP(0x10), A);  /* 0088E7 STA */
    A = rd16(DP(0x00));  /* 0088E9 LDA */
    B = 0x0000;  /* 0088EB LDA */
    /* 0088EF CMP */
    if (A < 0x8000) goto L_0088F6;  /* 0088F2 BCC */
    B -= 1;  /* 0088F4 DEC */
L_0088F6: TICK();
    C = 0;  /* 0088F6 CLC */
    A = adc16(A, rd16(DP(0x0C)));  /* 0088F7 ADC */
    B = adc16(B, rd16(DP(0x0E)));  /* 0088F9 ADC */
    C = 0;  /* 0088FC CLC */
    A = adc16(A, rd16(DP(0x10)));  /* 0088FD ADC */
    B = adc16(B, 0x0000);  /* 0088FF ADC */
    wr16(DP(0x07), A);  /* 008903 STA */
    wr16(DP(0x09), B);  /* 008905 STA */
    B = 0x0000;  /* 008908 LDA */
    A = rd16(DP(0x10));  /* 00890C LDA */
    div16(0x0005);  /* 00890E DIV */
    push16(B);  /* 008912 PHB */
    A = pull16();  /* 008914 PLA */
    if (A != 0) goto L_008935;  /* 008915 BNE */
    wr16(DP(0x14), A);  /* 008917 STA */
    wr16(DP(0x12), 0x0005);  /* 008919 LDM */
    A = rd16(DP(0x07));  /* 00891D LDA */
    B = rd16(DP(0x09));  /* 00891F LDA */
    X = 0x0012;  /* 008922 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 008925 JSRL */
    wr16(DP(0x07), A);  /* 008929 STA */
    wr16(DP(0x09), B);  /* 00892B STA */
    mflag = 1;  /* 00892E SEM */
    wr8(DP(0x0B), 0x7D);  /* 00892F LDM */
    goto L_008979;  /* 008932 JMP */
L_008935: TICK();
    mflag = 0;  /* 008935 CLM */
    A = rd16(DP(0x10));  /* 008936 LDA */
    A &= 0x0003;  /* 008938 AND */
    if (A != 0) goto L_00895B;  /* 00893B BNE */
    wr16(DP(0x14), A);  /* 00893D STA */
    wr16(DP(0x12), 0x0004);  /* 00893F LDM */
    A = rd16(DP(0x07));  /* 008943 LDA */
    B = rd16(DP(0x09));  /* 008945 LDA */
    X = 0x0012;  /* 008948 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 00894B JSRL */
    wr16(DP(0x07), A);  /* 00894F STA */
    wr16(DP(0x09), B);  /* 008951 STA */
    mflag = 1;  /* 008954 SEM */
    wr8(DP(0x0B), 0x64);  /* 008955 LDM */
    goto L_008979;  /* 008958 JMP */
L_00895B: TICK();
    mflag = 0;  /* 00895B CLM */
    wr16(DP(0x14), 0x0000);  /* 00895C LDM */
    wr16(DP(0x12), 0x0002);  /* 008960 LDM */
    A = rd16(DP(0x07));  /* 008964 LDA */
    B = rd16(DP(0x09));  /* 008966 LDA */
    X = 0x0012;  /* 008969 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 00896C JSRL */
    wr16(DP(0x07), A);  /* 008970 STA */
    wr16(DP(0x09), B);  /* 008972 STA */
    mflag = 1;  /* 008975 SEM */
    wr8(DP(0x0B), 0x32);  /* 008976 LDM */
L_008979: TICK();
    mflag = 0;  /* 008979 CLM */
    A = D;  /* 00897A TDA */
    C = 0;  /* 00897B CLC */
    A = adc16(A, 0x0007);  /* 00897C ADC */
    wr16(DP(0x12), A);  /* 00897F STA */
    wr16(DP(0x14), A);  /* 008981 STA */
    mflag = 1;  /* 008983 SEM */
    wr8(DP(0x16), 0x00);  /* 008984 LDM */
    mflag = 0;  /* 008987 CLM */
    A = rd16(DP(0x04));  /* 008988 LDA */
    wr16(DP(0x17), A);  /* 00898A STA */
    A = rd16(DP(0x06));  /* 00898C LDA */
    mflag = 1;  /* 00898E SEM */
    wr8(DP(0x19), (uint8_t)A);  /* 00898F STA */
    mflag = 0;  /* 008991 CLM */
    A = 0x0014;  /* 008992 LDA */
    Y = 0x0005;  /* 008995 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 008998 JSRL */
    D = pull16();  /* 00899C PLD */
    S += 3; return;  /* 00899D RTL */
}

/* 00899E  callers: 004997, 0049B6, 008CFB, 008D28 */
void F_00899E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00899E LDA */
    push16(D);  /* 0089A0 PHD */
    D = A;  /* 0089A1 TAD */
    C = 0;  /* 0089A2 CLC */
    A = adc16(A, 0x000E);  /* 0089A3 ADC */
    push16(A);  /* 0089A6 PHA */
    wr16(DP(0x00), Y);  /* 0089A7 STY */
    wr16(DP(0x0E), 0x0003);  /* 0089A9 LDM */
    A = D;  /* 0089AD TDA */
    A += 1;  /* 0089AE INC */
    B = 0x0000;  /* 0089AF LDA */
    wr16(DP(0x0F), A);  /* 0089B3 STA */
    wr16(DP(0x11), B);  /* 0089B5 STA */
    A = Y;  /* 0089B8 TYA */
    A &= 0x00FF;  /* 0089B9 AND */
    A -= 1;  /* 0089BC DEC */
    mpy16(0x000D);  /* 0089BD MPY */
    C = 0;  /* 0089C1 CLC */
    A = adc16(A, 0x0030);  /* 0089C2 ADC */
    wr16(DP(0x12), A);  /* 0089C5 STA */
    Y = 0x00A0;  /* 0089C7 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 0089CA JSRL */
    A = rd16(DP(0x00));  /* 0089CE LDA */
    mflag = 1;  /* 0089D0 SEM */
    wr8(ABS(0x0398), (uint8_t)A);  /* 0089D1 STA */
    mflag = 0;  /* 0089D4 CLM */
    X = rd16(ABS(0x01F9));  /* 0089D5 LDX */
    wr16(DP(0x0E), X);  /* 0089D8 STX */
    A = rd16(DP(0x02));  /* 0089DA LDA */
    wr16(DP(0x10), A);  /* 0089DC STA */
    A = rd16(DP(0x01));  /* 0089DE LDA */
    wr16(DP(0x11), A);  /* 0089E0 STA */
    A = D;  /* 0089E2 TDA */
    C = 0;  /* 0089E3 CLC */
    A = adc16(A, 0x0009);  /* 0089E4 ADC */
    B = 0x0000;  /* 0089E7 LDA */
    Y = A;  /* 0089EB TAY */
    S -= 3; F_0088B8();  /* 0089EC JSRL */
    wr16(DP(0x0E), A);  /* 0089F0 STA */
    A = D;  /* 0089F2 TDA */
    C = 0;  /* 0089F3 CLC */
    A = adc16(A, 0x0004);  /* 0089F4 ADC */
    wr16(DP(0x10), A);  /* 0089F7 STA */
    mflag = 1;  /* 0089F9 SEM */
    wr8(DP(0x12), (uint8_t)B);  /* 0089FA STA */
    mflag = 0;  /* 0089FD CLM */
    A = rd16(DP(0x0E));  /* 0089FE LDA */
    wr16(DP(0x13), A);  /* 008A00 STA */
    A = rd16(DP(0x12));  /* 008A02 LDA */
    mflag = 1;  /* 008A04 SEM */
    wr8(DP(0x15), (uint8_t)A);  /* 008A05 STA */
    mflag = 0;  /* 008A07 CLM */
    A = rd16(DP(0x10));  /* 008A08 LDA */
    wr16(DP(0x16), A);  /* 008A0A STA */
    mflag = 1;  /* 008A0C SEM */
    wr8(DP(0x18), 0x00);  /* 008A0D LDM */
    mflag = 0;  /* 008A10 CLM */
    A = 0x0013;  /* 008A11 LDA */
    Y = 0x0005;  /* 008A14 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 008A17 JSRL */
    B = rd16(DP(0x06));  /* 008A1B LDA */
    Y = rd16(DP(0x04));  /* 008A1E LDY */
    S -= 3; F_0087DE();  /* 008A20 JSRL */
    wr16(ABS(0x0392), A);  /* 008A24 STA */
    B = rd16(DP(0x06));  /* 008A27 LDA */
    Y = rd16(DP(0x04));  /* 008A2A LDY */
    S -= 3; F_008825();  /* 008A2C JSRL */
    wr8(ABS(0x0394), (uint8_t)A);  /* 008A30 STA */
    Y = rd16(DP(0x08));  /* 008A33 LDY */
    mflag = 0;  /* 008A35 CLM */
    S -= 3; F_008854();  /* 008A36 JSRL */
    wr16(ABS(0x0395), A);  /* 008A3A STA */
    A = rd16(DP(0x03));  /* 008A3D LDA */
    mflag = 1;  /* 008A3F SEM */
    wr8(ABS(0x0397), (uint8_t)A);  /* 008A40 STA */
    mflag = 0;  /* 008A43 CLM */
    Y = pull16();  /* 008A44 PLY */
    D = pull16();  /* 008A45 PLD */
    S += 3; return;  /* 008A46 RTL */
}

/* 008A47  callers: 004A29, 008D40, 00F0D6, 00FE28 */
void F_008A47(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008A47 LDA */
    push16(D);  /* 008A49 PHD */
    D = A;  /* 008A4A TAD */
    C = 0;  /* 008A4B CLC */
    A = adc16(A, 0x001C);  /* 008A4C ADC */
    push16(A);  /* 008A4F PHA */
    wr16(DP(0x00), Y);  /* 008A50 STY */
    wr16(DP(0x1C), 0x000D);  /* 008A52 LDM */
    A = D;  /* 008A56 TDA */
    A += 1;  /* 008A57 INC */
    B = 0x0000;  /* 008A58 LDA */
    wr16(DP(0x1D), A);  /* 008A5C STA */
    wr16(DP(0x1F), B);  /* 008A5E STA */
    A = Y;  /* 008A61 TYA */
    A &= 0x00FF;  /* 008A62 AND */
    A -= 1;  /* 008A65 DEC */
    mpy16(0x000D);  /* 008A66 MPY */
    C = 0;  /* 008A6A CLC */
    A = adc16(A, 0x002E);  /* 008A6B ADC */
    wr16(DP(0x20), A);  /* 008A6E STA */
    Y = 0x00A0;  /* 008A70 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 008A73 JSRL */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_008A86;  /* 008A77 BBC */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0) goto L_008A86;  /* 008A7D BBC */
    goto L_008BAE;  /* 008A83 JMP */
L_008A86: TICK();
    wr16(DP(0x1C), 0x0000);  /* 008A86 LDM */
    A = rd16(DP(0x02));  /* 008A8A LDA */
    wr16(DP(0x1E), A);  /* 008A8C STA */
    A = rd16(DP(0x01));  /* 008A8E LDA */
    wr16(DP(0x1F), A);  /* 008A90 STA */
    A = D;  /* 008A92 TDA */
    C = 0;  /* 008A93 CLC */
    A = adc16(A, 0x0017);  /* 008A94 ADC */
    B = 0x0000;  /* 008A97 LDA */
    Y = A;  /* 008A9B TAY */
    S -= 3; F_0088B8();  /* 008A9C JSRL */
    wr16(DP(0x1C), A);  /* 008AA0 STA */
    A = D;  /* 008AA2 TDA */
    C = 0;  /* 008AA3 CLC */
    A = adc16(A, 0x0012);  /* 008AA4 ADC */
    wr16(DP(0x1E), A);  /* 008AA7 STA */
    mflag = 1;  /* 008AA9 SEM */
    wr8(DP(0x20), (uint8_t)B);  /* 008AAA STA */
    mflag = 0;  /* 008AAD CLM */
    A = rd16(DP(0x1C));  /* 008AAE LDA */
    wr16(DP(0x21), A);  /* 008AB0 STA */
    A = rd16(DP(0x20));  /* 008AB2 LDA */
    mflag = 1;  /* 008AB4 SEM */
    wr8(DP(0x23), (uint8_t)A);  /* 008AB5 STA */
    mflag = 0;  /* 008AB7 CLM */
    A = rd16(DP(0x1E));  /* 008AB8 LDA */
    wr16(DP(0x24), A);  /* 008ABA STA */
    mflag = 1;  /* 008ABC SEM */
    wr8(DP(0x26), 0x00);  /* 008ABD LDM */
    mflag = 0;  /* 008AC0 CLM */
    A = 0x0021;  /* 008AC1 LDA */
    Y = 0x0005;  /* 008AC4 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 008AC7 JSRL */
    A = rd16(DP(0x01));  /* 008ACB LDA */
    mflag = 1;  /* 008ACD SEM */
    SETLO(A, A & 0x3F);  /* 008ACE AND */
    mflag = 0;  /* 008AD0 CLM */
    A &= 0x00FF;  /* 008AD1 AND */
    A = rla16(A, 0x0008);  /* 008AD4 RLA */
    A &= 0xFF00;  /* 008AD8 AND */
    wr16(DP(0x1C), A);  /* 008ADB STA */
    A = rd16(DP(0x02));  /* 008ADD LDA */
    A &= 0x00FF;  /* 008ADF AND */
    C = 0;  /* 008AE2 CLC */
    A = adc16(A, rd16(DP(0x1C)));  /* 008AE3 ADC */
    wr16(DP(0x10), A);  /* 008AE5 STA */
    /* 008AE7 CMP */
    if (A != 0x3FFF) goto L_008AF4;  /* 008AEA BNE */
    mflag = 1;  /* 008AEC SEM */
    wr8(ABS(0x0379), 0xFF);  /* 008AED LDM */
    goto L_008B69;  /* 008AF1 JMP */
L_008AF4: TICK();
    mflag = 0;  /* 008AF4 CLM */
    B = 0x0000;  /* 008AF5 LDA */
    A = rd16(DP(0x10));  /* 008AF9 LDA */
    div16(0x0028);  /* 008AFB DIV */
    wr16(ABS(0x0220), A);  /* 008AFF STA */
    wr16(DP(0x1C), 0x0000);  /* 008B02 LDM */
    A = rd16(DP(0x02));  /* 008B06 LDA */
    wr16(DP(0x1E), A);  /* 008B08 STA */
    A = rd16(DP(0x01));  /* 008B0A LDA */
    wr16(DP(0x1F), A);  /* 008B0C STA */
    A = D;  /* 008B0E TDA */
    C = 0;  /* 008B0F CLC */
    A = adc16(A, 0x0017);  /* 008B10 ADC */
    B = 0x0000;  /* 008B13 LDA */
    Y = A;  /* 008B17 TAY */
    S -= 3; F_0088B8();  /* 008B18 JSRL */
    wr16(DP(0x1C), A);  /* 008B1C STA */
    A = D;  /* 008B1E TDA */
    C = 0;  /* 008B1F CLC */
    A = adc16(A, 0x0012);  /* 008B20 ADC */
    wr16(DP(0x1E), A);  /* 008B23 STA */
    mflag = 1;  /* 008B25 SEM */
    wr8(DP(0x20), (uint8_t)B);  /* 008B26 STA */
    mflag = 0;  /* 008B29 CLM */
    A = rd16(DP(0x1C));  /* 008B2A LDA */
    wr16(DP(0x21), A);  /* 008B2C STA */
    A = rd16(DP(0x20));  /* 008B2E LDA */
    mflag = 1;  /* 008B30 SEM */
    wr8(DP(0x23), (uint8_t)A);  /* 008B31 STA */
    mflag = 0;  /* 008B33 CLM */
    A = rd16(DP(0x1E));  /* 008B34 LDA */
    wr16(DP(0x24), A);  /* 008B36 STA */
    mflag = 1;  /* 008B38 SEM */
    wr8(DP(0x26), 0x00);  /* 008B39 LDM */
    mflag = 0;  /* 008B3C CLM */
    A = 0x0021;  /* 008B3D LDA */
    Y = 0x0005;  /* 008B40 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 008B43 JSRL */
    B = rd16(DP(0x14));  /* 008B47 LDA */
    Y = rd16(DP(0x12));  /* 008B4A LDY */
    S -= 3; F_0087DE();  /* 008B4C JSRL */
    wr16(ABS(0x0377), A);  /* 008B50 STA */
    B = rd16(DP(0x14));  /* 008B53 LDA */
    Y = rd16(DP(0x12));  /* 008B56 LDY */
    S -= 3; F_008825();  /* 008B58 JSRL */
    wr8(ABS(0x0379), (uint8_t)A);  /* 008B5C STA */
    Y = rd16(DP(0x16));  /* 008B5F LDY */
    mflag = 0;  /* 008B61 CLM */
    S -= 3; F_008854();  /* 008B62 JSRL */
    wr16(ABS(0x037A), A);  /* 008B66 STA */
L_008B69: TICK();
    mflag = 1;  /* 008B69 SEM */
    wr8(DP(0x0E), 0x00);  /* 008B6A LDM */
    if ((rd8(DP(0x06)) & 0x80) == 0) goto L_008B9B;  /* 008B6D BBC */
    if ((rd8(DP(0x07)) & 0x80) == 0) goto L_008B78;  /* 008B71 BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x20);  /* 008B75 SEB */
L_008B78: TICK();
    if ((rd8(DP(0x08)) & 0x80) == 0) goto L_008B7F;  /* 008B78 BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x10);  /* 008B7C SEB */
L_008B7F: TICK();
    if ((rd8(DP(0x09)) & 0x80) == 0) goto L_008B86;  /* 008B7F BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x08);  /* 008B83 SEB */
L_008B86: TICK();
    if ((rd8(DP(0x0A)) & 0x80) == 0) goto L_008B8D;  /* 008B86 BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x04);  /* 008B8A SEB */
L_008B8D: TICK();
    if ((rd8(DP(0x0B)) & 0x80) == 0) goto L_008B94;  /* 008B8D BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x02);  /* 008B91 SEB */
L_008B94: TICK();
    if ((rd8(DP(0x0C)) & 0x80) == 0) goto L_008B9B;  /* 008B94 BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x01);  /* 008B98 SEB */
L_008B9B: TICK();
    if ((rd8(DP(0x01)) & 0x80) == 0) goto L_008BA2;  /* 008B9B BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x80);  /* 008B9F SEB */
L_008BA2: TICK();
    if ((rd8(DP(0x01)) & 0x40) == 0) goto L_008BA9;  /* 008BA2 BBC */
    wr8(DP(0x0E), rd8(DP(0x0E)) | 0x40);  /* 008BA6 SEB */
L_008BA9: TICK();
    SETLO(A, rd8(DP(0x0E)));  /* 008BA9 LDA */
    wr8(ABS(0x037C), (uint8_t)A);  /* 008BAB STA */
L_008BAE: TICK();
    mflag = 1;  /* 008BAE SEM */
    wr8(DP(0x0E), 0x00);  /* 008BAF LDM */
L_008BB2: TICK();
    SETLO(A, rd8(DP(0x0E)));  /* 008BB2 LDA */
    C = ((A & 0xFF) >= 0x08);  /* 008BB4 CMP */
    if (C) goto L_008C01;  /* 008BB6 BCS */
    mflag = 0;  /* 008BB8 CLM */
    A &= 0x00FF;  /* 008BB9 AND */
    A = adc16(A, 0x0005);  /* 008BBC ADC */
    X = A;  /* 008BBF TAX */
    A = rd16(DP(0x01 + X));  /* 008BC0 LDA */
    mflag = 1;  /* 008BC2 SEM */
    SETLO(A, A & 0x7F);  /* 008BC3 AND */
    wr8(DP(0x0F), (uint8_t)A);  /* 008BC5 STA */
    /* 008BC7 CMP */
    if ((A & 0xFF) < 0x41) goto L_008BCF;  /* 008BC9 BCC */
    /* 008BCB CMP */
    if ((A & 0xFF) < 0x5B) goto L_008BE7;  /* 008BCD BCC */
L_008BCF: TICK();
    /* 008BCF CMP */
    if ((A & 0xFF) < 0x30) goto L_008BD7;  /* 008BD1 BCC */
    /* 008BD3 CMP */
    if ((A & 0xFF) < 0x3A) goto L_008BE7;  /* 008BD5 BCC */
L_008BD7: TICK();
    /* 008BD7 CMP */
    if ((A & 0xFF) == 0x20) goto L_008BE7;  /* 008BD9 BEQ */
    /* 008BDB CMP */
    if ((A & 0xFF) == 0x5F) goto L_008BE7;  /* 008BDD BEQ */
    /* 008BDF CMP */
    if ((A & 0xFF) == 0x2D) goto L_008BE7;  /* 008BE1 BEQ */
    /* 008BE3 CMP */
    if ((A & 0xFF) != 0x2F) goto L_008BF7;  /* 008BE5 BNE */
L_008BE7: TICK();
    SETLO(A, rd8(DP(0x0E)));  /* 008BE7 LDA */
    mflag = 0;  /* 008BE9 CLM */
    A &= 0x00FF;  /* 008BEA AND */
    X = A;  /* 008BED TAX */
    A = rd16(DP(0x0F));  /* 008BEE LDA */
    mflag = 1;  /* 008BF0 SEM */
    wr8((ABS(0x024A) + X), (uint8_t)A);  /* 008BF1 STA */
    goto L_008BFC;  /* 008BF4 JMP */
L_008BF7: TICK();
    mflag = 1;  /* 008BF7 SEM */
    wr8(ABS(0x024A), 0xFF);  /* 008BF8 LDM */
L_008BFC: TICK();
    t = (rd8(DP(0x0E)) + 1) & 0xFF; wr8(DP(0x0E), t);  /* 008BFC INC */
    goto L_008BB2;  /* 008BFE JMP */
L_008C01: TICK();
    mflag = 0;  /* 008C01 CLM */
    Y = pull16();  /* 008C02 PLY */
    D = pull16();  /* 008C03 PLD */
    S += 3; return;  /* 008C04 RTL */
}

/* 008C05  callers: 0049D5, 00D43C */
void F_008C05(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008C05 LDA */
    push16(D);  /* 008C07 PHD */
    D = A;  /* 008C08 TAD */
    C = 0;  /* 008C09 CLC */
    A = adc16(A, 0x000A);  /* 008C0A ADC */
    push16(A);  /* 008C0D PHA */
    wr16(DP(0x00), Y);  /* 008C0E STY */
    A = Y;  /* 008C10 TYA */
    mflag = 1;  /* 008C11 SEM */
    wr8(DP(0x01), (uint8_t)A);  /* 008C12 STA */
L_008C14: TICK();
    t = (rd8(DP(0x01)) + 1) & 0xFF; wr8(DP(0x01), t);  /* 008C14 INC */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_008C20;  /* 008C16 BBC */
    SETLO(A, 0x0A);  /* 008C1B LDA */
    goto L_008C22;  /* 008C1D JMP */
L_008C20: TICK();
    SETLO(A, 0x63);  /* 008C20 LDA */
L_008C22: TICK();
    t = rd8(DP(0x01));  /* 008C22 CMP */
    if ((A & 0xFF) >= t) goto L_008C29;  /* 008C24 BCS */
    wr8(DP(0x01), 0x01);  /* 008C26 LDM */
L_008C29: TICK();
    mflag = 0;  /* 008C29 CLM */
    wr16(DP(0x0A), 0x0008);  /* 008C2A LDM */
    A = rd16(DP(0x01));  /* 008C2E LDA */
    A &= 0x00FF;  /* 008C30 AND */
    A -= 1;  /* 008C33 DEC */
    wr16(DP(0x0C), A);  /* 008C34 STA */
    A = D;  /* 008C36 TDA */
    C = 0;  /* 008C37 CLC */
    A = adc16(A, 0x0006);  /* 008C38 ADC */
    B = 0x0000;  /* 008C3B LDA */
    Y = A;  /* 008C3F TAY */
    S -= 3; F_01E30B();  /* 008C40 JSRL */
    wr16(DP(0x0A), A);  /* 008C44 STA */
    A = D;  /* 008C46 TDA */
    A += 1;  /* 008C47 INC */
    A += 1;  /* 008C48 INC */
    wr16(DP(0x0C), A);  /* 008C49 STA */
    mflag = 1;  /* 008C4B SEM */
    wr8(DP(0x0E), (uint8_t)B);  /* 008C4C STA */
    mflag = 0;  /* 008C4F CLM */
    A = rd16(DP(0x0A));  /* 008C50 LDA */
    wr16(DP(0x0F), A);  /* 008C52 STA */
    A = rd16(DP(0x0E));  /* 008C54 LDA */
    mflag = 1;  /* 008C56 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 008C57 STA */
    mflag = 0;  /* 008C59 CLM */
    A = rd16(DP(0x0C));  /* 008C5A LDA */
    wr16(DP(0x12), A);  /* 008C5C STA */
    mflag = 1;  /* 008C5E SEM */
    wr8(DP(0x14), 0x00);  /* 008C5F LDM */
    mflag = 0;  /* 008C62 CLM */
    A = 0x000F;  /* 008C63 LDA */
    Y = 0x0004;  /* 008C66 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 008C69 JSRL */
L_008C6D: TICK();
    X = rd16(DP(0x02));  /* 008C6D LDX */
    mflag = 1;  /* 008C6F SEM */
    SETLO(A, rd8((ABS(0x02D2) + X)));  /* 008C70 LDA */
    if ((A & 0xFF) != 0) goto L_008CDC;  /* 008C73 BNE */
    SETLO(A, rd8(DP(0x04)));  /* 008C75 LDA */
    SETLO(A, A ^ 0xFF);  /* 008C77 EOR */
    SETLO(A, A + 1);  /* 008C79 INC */
    C = 0;  /* 008C7A CLC */
    SETLO(A, adc8(A, 0x08));  /* 008C7B ADC */
    C = 0;  /* 008C7D CLC */
    SETLO(A, adc8(A, rd8(DP(0x01))));  /* 008C7E ADC */
    wr8(DP(0x01), (uint8_t)A);  /* 008C80 STA */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_008C8C;  /* 008C82 BBC */
    SETLO(A, 0x0A);  /* 008C87 LDA */
    goto L_008C8E;  /* 008C89 JMP */
L_008C8C: TICK();
    SETLO(A, 0x63);  /* 008C8C LDA */
L_008C8E: TICK();
    t = rd8(DP(0x01));  /* 008C8E CMP */
    if ((A & 0xFF) >= t) goto L_008C95;  /* 008C90 BCS */
    wr8(DP(0x01), 0x01);  /* 008C92 LDM */
L_008C95: TICK();
    mflag = 0;  /* 008C95 CLM */
    wr16(DP(0x0A), 0x0008);  /* 008C96 LDM */
    A = rd16(DP(0x01));  /* 008C9A LDA */
    A &= 0x00FF;  /* 008C9C AND */
    A -= 1;  /* 008C9F DEC */
    wr16(DP(0x0C), A);  /* 008CA0 STA */
    A = D;  /* 008CA2 TDA */
    C = 0;  /* 008CA3 CLC */
    A = adc16(A, 0x0006);  /* 008CA4 ADC */
    B = 0x0000;  /* 008CA7 LDA */
    Y = A;  /* 008CAB TAY */
    S -= 3; F_01E30B();  /* 008CAC JSRL */
    wr16(DP(0x0A), A);  /* 008CB0 STA */
    A = D;  /* 008CB2 TDA */
    A += 1;  /* 008CB3 INC */
    A += 1;  /* 008CB4 INC */
    wr16(DP(0x0C), A);  /* 008CB5 STA */
    mflag = 1;  /* 008CB7 SEM */
    wr8(DP(0x0E), (uint8_t)B);  /* 008CB8 STA */
    mflag = 0;  /* 008CBB CLM */
    A = rd16(DP(0x0A));  /* 008CBC LDA */
    wr16(DP(0x0F), A);  /* 008CBE STA */
    A = rd16(DP(0x0E));  /* 008CC0 LDA */
    mflag = 1;  /* 008CC2 SEM */
    wr8(DP(0x11), (uint8_t)A);  /* 008CC3 STA */
    mflag = 0;  /* 008CC5 CLM */
    A = rd16(DP(0x0C));  /* 008CC6 LDA */
    wr16(DP(0x12), A);  /* 008CC8 STA */
    mflag = 1;  /* 008CCA SEM */
    wr8(DP(0x14), 0x00);  /* 008CCB LDM */
    mflag = 0;  /* 008CCE CLM */
    A = 0x000F;  /* 008CCF LDA */
    Y = 0x0004;  /* 008CD2 LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 008CD5 JSRL */
    goto L_008C6D;  /* 008CD9 JMP */
L_008CDC: TICK();
    X = rd16(DP(0x02));  /* 008CDC LDX */
    mflag = 0;  /* 008CDE CLM */
    Y = X;  /* 008CDF TXY */
    X = rd16(DP(0x04));  /* 008CE0 LDX */
    A = 0x0001;  /* 008CE2 LDA */
    mflag = 1;  /* 008CE5 SEM */
    S -= 3; rt_shl_01E4E0();  /* 008CE6 JSRL */
    SETLO(A, A & rd8((ABS(0x02D2) + Y)));  /* 008CEA AND */
    if ((A & 0xFF) != 0) goto L_008CF8;  /* 008CED BNE */
    SETLO(A, rd8(DP(0x01)));  /* 008CEF LDA */
    t = rd8(DP(0x00));  /* 008CF1 CMP */
    if ((A & 0xFF) == t) goto L_008CF8;  /* 008CF3 BEQ */
    goto L_008C14;  /* 008CF5 JMP */
L_008CF8: TICK();
    Y = rd16(DP(0x01));  /* 008CF8 LDY */
    mflag = 0;  /* 008CFA CLM */
    S -= 3; F_00899E();  /* 008CFB JSRL */
    wr16(DP(0x0A), 0x0392);  /* 008CFF LDM */
    wr16(DP(0x0C), 0x037D);  /* 008D03 LDM */
    A = 0x000A;  /* 008D07 LDA */
    Y = 0x0007;  /* 008D0A LDY */
    S -= 3; rt_memcpy_01E466();  /* 008D0D JSRL */
    Y = pull16();  /* 008D11 PLY */
    D = pull16();  /* 008D12 PLD */
    S += 3; return;  /* 008D13 RTL */
}

/* 008D14  callers: 0036B9, 004D86, 005C56, 00F227, 00FD95, 00FDE7, 01939D */
void F_008D14(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008D14 LDA */
    push16(D);  /* 008D16 PHD */
    D = A;  /* 008D17 TAD */
    A += 1;  /* 008D18 INC */
    push16(A);  /* 008D19 PHA */
    wr16(DP(0x00), Y);  /* 008D1A STY */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0) goto L_008D28;  /* 008D1C BBC */
    if ((rd16(ABS(0x023A)) & 0x0001) == 0x0001) goto L_008D3E;  /* 008D22 BBS */
L_008D28: TICK();
    S -= 3; F_00899E();  /* 008D28 JSRL */
    wr16(DP(0x01), 0x0392);  /* 008D2C LDM */
    wr16(DP(0x03), 0x0370);  /* 008D30 LDM */
    A = 0x0001;  /* 008D34 LDA */
    Y = 0x0007;  /* 008D37 LDY */
    S -= 3; rt_memcpy_01E466();  /* 008D3A JSRL */
L_008D3E: TICK();
    Y = rd16(DP(0x00));  /* 008D3E LDY */
    S -= 3; F_008A47();  /* 008D40 JSRL */
    Y = pull16();  /* 008D44 PLY */
    D = pull16();  /* 008D45 PLD */
    S += 3; return;  /* 008D46 RTL */
}

/* 008D47  callers: 0046ED, 0090B3 */
void F_008D47(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008D47 LDA */
    push16(D);  /* 008D49 PHD */
    D = A;  /* 008D4A TAD */
    C = 0;  /* 008D4B CLC */
    A = adc16(A, 0x000A);  /* 008D4C ADC */
    push16(A);  /* 008D4F PHA */
    wr16(DP(0x0A), 0x0007);  /* 008D50 LDM */
    A = D;  /* 008D54 TDA */
    A += 1;  /* 008D55 INC */
    A += 1;  /* 008D56 INC */
    B = 0x0000;  /* 008D57 LDA */
    wr16(DP(0x0B), A);  /* 008D5B STA */
    wr16(DP(0x0D), B);  /* 008D5D STA */
    wr16(DP(0x0E), 0x0571);  /* 008D60 LDM */
    Y = 0x00A0;  /* 008D64 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 008D67 JSRL */
    wr16(DP(0x00), 0x0000);  /* 008D6B LDM */
    mflag = 1;  /* 008D6F SEM */
    wr8(DP(0x09), 0x00);  /* 008D70 LDM */
L_008D73: TICK();
    SETLO(A, rd8(DP(0x09)));  /* 008D73 LDA */
    C = ((A & 0xFF) >= 0x05);  /* 008D75 CMP */
    if (C) goto L_008D9E;  /* 008D77 BCS */
    mflag = 0;  /* 008D79 CLM */
    A &= 0x00FF;  /* 008D7A AND */
    X = A;  /* 008D7D TAX */
    A = rd16(DP(0x02 + X));  /* 008D7E LDA */
    A &= 0x00FF;  /* 008D80 AND */
    A = adc16(A, rd16(DP(0x00)));  /* 008D83 ADC */
    wr16(DP(0x00), A);  /* 008D85 STA */
    A = rd16(DP(0x09));  /* 008D87 LDA */
    A &= 0x00FF;  /* 008D89 AND */
    X = A;  /* 008D8C TAX */
    A = rd16(DP(0x09));  /* 008D8D LDA */
    A &= 0x00FF;  /* 008D8F AND */
    Y = A;  /* 008D92 TAY */
    A = rd16(DP(0x02 + X));  /* 008D93 LDA */
    mflag = 1;  /* 008D95 SEM */
    wr8((ABS(0x01EC) + Y), (uint8_t)A);  /* 008D96 STA */
    t = (rd8(DP(0x09)) + 1) & 0xFF; wr8(DP(0x09), t);  /* 008D99 INC */
    goto L_008D73;  /* 008D9B JMP */
L_008D9E: TICK();
    mflag = 0;  /* 008D9E CLM */
    A = rd16(DP(0x07));  /* 008D9F LDA */
    A &= 0x00FF;  /* 008DA1 AND */
    A = rla16(A, 0x0008);  /* 008DA4 RLA */
    A &= 0xFF00;  /* 008DA8 AND */
    wr16(DP(0x0A), A);  /* 008DAB STA */
    A = rd16(DP(0x08));  /* 008DAD LDA */
    A &= 0x00FF;  /* 008DAF AND */
    C = 0;  /* 008DB2 CLC */
    A = adc16(A, rd16(DP(0x0A)));  /* 008DB3 ADC */
    t = rd16(DP(0x00));  /* 008DB5 CMP */
    if (A != t) goto L_008DBF;  /* 008DB7 BNE */
    A = 0x0001;  /* 008DB9 LDA */
    goto L_008DC2;  /* 008DBC JMP */
L_008DBF: TICK();
    A = 0x0000;  /* 008DBF LDA */
L_008DC2: TICK();
    mflag = 1;  /* 008DC2 SEM */
    Y = pull16();  /* 008DC3 PLY */
    D = pull16();  /* 008DC4 PLD */
    S += 3; return;  /* 008DC5 RTL */
}

/* 008DC6  callers: 004DF5, 0060A9, 00C0A8, 00C64C, 00C7E4, 00D05D, 00D1EA, 00D22E ... */
void F_008DC6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008DC6 LDA */
    push16(D);  /* 008DC8 PHD */
    D = A;  /* 008DC9 TAD */
    wr16(DP(0x00), Y);  /* 008DCA STY */
    mflag = 1;  /* 008DCC SEM */
    wr8(ABS(ADCON), 0x80);  /* 008DCD LDM */
    SETLO(A, rd8(ABS(ADCON)));  /* 008DD1 LDA */
    SETLO(A, A | rd8(DP(0x00)));  /* 008DD4 ORA */
    wr8(ABS(ADCON), (uint8_t)A);  /* 008DD6 STA */
    wr8(ABS(ADCON), rd8(ABS(ADCON)) | 0x40);  /* 008DD9 SEB */
L_008DDD: TICK();
    if ((rd8(ABS(ADIC)) & 0x08) == 0x08) goto L_008DE5;  /* 008DDD BBS */
    goto L_008DDD;  /* 008DE2 JMP */
L_008DE5: TICK();
    mflag = 1;  /* 008DE5 SEM */
    wr8(ABS(ADIC), rd8(ABS(ADIC)) & ~0x08);  /* 008DE6 CLB */
    mflag = 0;  /* 008DEA CLM */
    D = pull16();  /* 008DEB PLD */
    S += 3; return;  /* 008DEC RTL */
}

/* 008DED  callers: 003C83, 003CDE, 0041AE, 0041C6, 00471A, 004B20, 004CF5, 0090F6 ... */
void F_008DED(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008DED LDA */
    push16(D);  /* 008DEF PHD */
    D = A;  /* 008DF0 TAD */
    C = 0;  /* 008DF1 CLC */
    A = adc16(A, 0x000C);  /* 008DF2 ADC */
    push16(A);  /* 008DF5 PHA */
    mflag = 1;  /* 008DF6 SEM */
    wr8(DP(0x00), 0x00);  /* 008DF7 LDM */
L_008DFA: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008DFA LDA */
    /* 008DFC CMP */
    if ((A & 0xFF) >= 0x05) goto L_008E19;  /* 008DFE BCS */
    mflag = 0;  /* 008E00 CLM */
    A &= 0x00FF;  /* 008E01 AND */
    X = A;  /* 008E04 TAX */
    A = rd16(DP(0x00));  /* 008E05 LDA */
    A &= 0x00FF;  /* 008E07 AND */
    A += 1;  /* 008E0A INC */
    A += 1;  /* 008E0B INC */
    Y = X;  /* 008E0C TXY */
    X = A;  /* 008E0D TAX */
    A = rd16((ABS(0x025A) + Y));  /* 008E0E LDA */
    mflag = 1;  /* 008E11 SEM */
    wr8(DP(0x01 + X), (uint8_t)A);  /* 008E12 STA */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008E14 INC */
    goto L_008DFA;  /* 008E16 JMP */
L_008E19: TICK();
    mflag = 1;  /* 008E19 SEM */
    wr8(DP(0x02), 0x00);  /* 008E1A LDM */
    wr8(DP(0x01), 0x00);  /* 008E1D LDM */
    mflag = 0;  /* 008E20 CLM */
    if ((rd16(ABS(0x023C)) & 0x1000) == 0) goto L_008E2B;  /* 008E21 BBC */
    mflag = 1;  /* 008E27 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x01);  /* 008E28 SEB */
L_008E2B: TICK();
    mflag = 0;  /* 008E2B CLM */
    if ((rd16(ABS(0x023A)) & 0x0800) == 0) goto L_008E36;  /* 008E2C BBC */
    mflag = 1;  /* 008E32 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x02);  /* 008E33 SEB */
L_008E36: TICK();
    mflag = 0;  /* 008E36 CLM */
    if ((rd16(ABS(0x0240)) & 0x4000) == 0) goto L_008E41;  /* 008E37 BBC */
    mflag = 1;  /* 008E3D SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x04);  /* 008E3E SEB */
L_008E41: TICK();
    mflag = 0;  /* 008E41 CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0) goto L_008E4C;  /* 008E42 BBC */
    mflag = 1;  /* 008E48 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x08);  /* 008E49 SEB */
L_008E4C: TICK();
    mflag = 1;  /* 008E4C SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_008E5A;  /* 008E4D BBC */
    if ((rd8(ABS(0x0246)) & 0x02) == 0) goto L_008E5A;  /* 008E52 BBC */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x10);  /* 008E57 SEB */
L_008E5A: TICK();
    mflag = 0;  /* 008E5A CLM */
    if ((rd16(ABS(0x023C)) & 0x0100) == 0) goto L_008E65;  /* 008E5B BBC */
    mflag = 1;  /* 008E61 SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x20);  /* 008E62 SEB */
L_008E65: TICK();
    mflag = 0;  /* 008E65 CLM */
    A = rd16(ABS(0x01EB));  /* 008E66 LDA */
    mflag = 1;  /* 008E69 SEM */
    /* 008E6A CMP */
    if ((A & 0xFF) >= 0x03) goto L_008E74;  /* 008E6C BCS */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x40);  /* 008E6E SEB */
    goto L_008E74;  /* 008E71 JMP */
L_008E74: TICK();
    mflag = 0;  /* 008E74 CLM */
    if ((rd16(ABS(0x0246)) & 0x0020) == 0) goto L_008E7F;  /* 008E75 BBC */
    mflag = 1;  /* 008E7B SEM */
    wr8(DP(0x01), rd8(DP(0x01)) | 0x80);  /* 008E7C SEB */
L_008E7F: TICK();
    mflag = 0;  /* 008E7F CLM */
    wr16(DP(0x0A), 0x0000);  /* 008E80 LDM */
    mflag = 1;  /* 008E84 SEM */
    wr8(DP(0x00), 0x00);  /* 008E85 LDM */
L_008E88: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008E88 LDA */
    C = ((A & 0xFF) >= 0x07);  /* 008E8A CMP */
    if (C) goto L_008EA2;  /* 008E8C BCS */
    mflag = 0;  /* 008E8E CLM */
    A &= 0x00FF;  /* 008E8F AND */
    X = A;  /* 008E92 TAX */
    A = rd16(DP(0x01 + X));  /* 008E93 LDA */
    A &= 0x00FF;  /* 008E95 AND */
    A = adc16(A, rd16(DP(0x0A)));  /* 008E98 ADC */
    wr16(DP(0x0A), A);  /* 008E9A STA */
    mflag = 1;  /* 008E9C SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008E9D INC */
    goto L_008E88;  /* 008E9F JMP */
L_008EA2: TICK();
    mflag = 0;  /* 008EA2 CLM */
    A = rd16(DP(0x0B));  /* 008EA3 LDA */
    A &= 0x00FF;  /* 008EA5 AND */
    mflag = 1;  /* 008EA8 SEM */
    wr8(DP(0x08), (uint8_t)A);  /* 008EA9 STA */
    SETLO(A, rd8(DP(0x0A)));  /* 008EAB LDA */
    wr8(DP(0x09), (uint8_t)A);  /* 008EAD STA */
    wr8(DP(0x0C), 0x09);  /* 008EAF LDM */
    mflag = 0;  /* 008EB2 CLM */
    A = D;  /* 008EB3 TDA */
    A += 1;  /* 008EB4 INC */
    B = 0x0000;  /* 008EB5 LDA */
    wr16(DP(0x0D), A);  /* 008EB9 STA */
    wr16(DP(0x0F), B);  /* 008EBB STA */
    wr16(DP(0x10), 0x054B);  /* 008EBE LDM */
    Y = 0x00A0;  /* 008EC2 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 008EC5 JSRL */
    Y = pull16();  /* 008EC9 PLY */
    D = pull16();  /* 008ECA PLD */
    S += 3; return;  /* 008ECB RTL */
}

/* 008ECC  callers: 00E464 */
void F_008ECC(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 008ECC LDA */
    push16(D);  /* 008ECE PHD */
    D = A;  /* 008ECF TAD */
    C = 0;  /* 008ED0 CLC */
    A = adc16(A, 0x000E);  /* 008ED1 ADC */
    push16(A);  /* 008ED4 PHA */
    wr16(DP(0x0E), 0x0001);  /* 008ED5 LDM */
    wr16(DP(0x0F), 0x033F);  /* 008ED9 LDM */
    wr16(DP(0x11), 0x0000);  /* 008EDD LDM */
    wr16(DP(0x12), 0x0014);  /* 008EE1 LDM */
    Y = 0x00A0;  /* 008EE5 LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 008EE8 JSRL */
    wr16(DP(0x0E), 0x0009);  /* 008EEC LDM */
    A = D;  /* 008EF0 TDA */
    A += 1;  /* 008EF1 INC */
    B = 0x0000;  /* 008EF2 LDA */
    wr16(DP(0x0F), A);  /* 008EF6 STA */
    wr16(DP(0x11), B);  /* 008EF8 STA */
    wr16(DP(0x12), 0x054B);  /* 008EFB LDM */
    Y = 0x00A0;  /* 008EFF LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 008F02 JSRL */
    wr16(DP(0x0C), 0x0000);  /* 008F06 LDM */
    mflag = 1;  /* 008F0A SEM */
    wr8(DP(0x0A), 0x00);  /* 008F0B LDM */
    wr8(DP(0x00), 0x00);  /* 008F0E LDM */
L_008F11: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008F11 LDA */
    C = ((A & 0xFF) >= 0x07);  /* 008F13 CMP */
    if (C) goto L_008F2B;  /* 008F15 BCS */
    mflag = 0;  /* 008F17 CLM */
    A &= 0x00FF;  /* 008F18 AND */
    X = A;  /* 008F1B TAX */
    A = rd16(DP(0x01 + X));  /* 008F1C LDA */
    A &= 0x00FF;  /* 008F1E AND */
    A = adc16(A, rd16(DP(0x0C)));  /* 008F21 ADC */
    wr16(DP(0x0C), A);  /* 008F23 STA */
    mflag = 1;  /* 008F25 SEM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008F26 INC */
    goto L_008F11;  /* 008F28 JMP */
L_008F2B: TICK();
    mflag = 0;  /* 008F2B CLM */
    A = rd16(DP(0x08));  /* 008F2C LDA */
    A &= 0x00FF;  /* 008F2E AND */
    A = rla16(A, 0x0008);  /* 008F31 RLA */
    A &= 0xFF00;  /* 008F35 AND */
    wr16(DP(0x0E), A);  /* 008F38 STA */
    A = rd16(DP(0x09));  /* 008F3A LDA */
    A &= 0x00FF;  /* 008F3C AND */
    C = 0;  /* 008F3F CLC */
    A = adc16(A, rd16(DP(0x0E)));  /* 008F40 ADC */
    t = rd16(DP(0x0C));  /* 008F42 CMP */
    if (A == t) goto L_008F8B;  /* 008F44 BEQ */
    mflag = 1;  /* 008F46 SEM */
    wr8(DP(0x0A), 0x01);  /* 008F47 LDM */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x01);  /* 008F4A CLB */
    wr8(ABS(0x0241), rd8(ABS(0x0241)) & ~0x01);  /* 008F4E CLB */
    wr8(ABS(0x0241), rd8(ABS(0x0241)) & ~0x40);  /* 008F52 CLB */
    wr8(ABS(0x023B), rd8(ABS(0x023B)) & ~0x08);  /* 008F56 CLB */
    wr8(ABS(0x023D), rd8(ABS(0x023D)) & ~0x10);  /* 008F5A CLB */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_008F69;  /* 008F5E BBC */
    mflag = 0;  /* 008F63 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0002);  /* 008F64 SEB */
L_008F69: TICK();
    mflag = 0;  /* 008F69 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) & ~0x0020);  /* 008F6A CLB */
    mflag = 1;  /* 008F6F SEM */
    wr8(DP(0x00), 0x00);  /* 008F70 LDM */
L_008F73: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008F73 LDA */
    /* 008F75 CMP */
    if ((A & 0xFF) >= 0x05) goto L_008F88;  /* 008F77 BCS */
    mflag = 0;  /* 008F79 CLM */
    A &= 0x00FF;  /* 008F7A AND */
    X = A;  /* 008F7D TAX */
    mflag = 1;  /* 008F7E SEM */
    wr8((ABS(0x025A) + X), 0xFF);  /* 008F7F LDM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008F83 INC */
    goto L_008F73;  /* 008F85 JMP */
L_008F88: TICK();
    goto L_0090F0;  /* 008F88 JMP */
L_008F8B: TICK();
    mflag = 1;  /* 008F8B SEM */
    wr8(DP(0x0B), 0x00);  /* 008F8C LDM */
    wr8(DP(0x00), 0x00);  /* 008F8F LDM */
L_008F92: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008F92 LDA */
    mflag = 0;  /* 008F94 CLM */
    A &= 0x00FF;  /* 008F95 AND */
    A += 1;  /* 008F98 INC */
    A += 1;  /* 008F99 INC */
    X = A;  /* 008F9A TAX */
    A = rd16(DP(0x01 + X));  /* 008F9B LDA */
    mflag = 1;  /* 008F9D SEM */
    /* 008F9E CMP */
    if ((A & 0xFF) < 0x0A) goto L_008FB4;  /* 008FA0 BCC */
    SETLO(A, rd8(DP(0x00)));  /* 008FA2 LDA */
    mflag = 0;  /* 008FA4 CLM */
    A &= 0x00FF;  /* 008FA5 AND */
    A += 1;  /* 008FA8 INC */
    A += 1;  /* 008FA9 INC */
    X = A;  /* 008FAA TAX */
    A = rd16(DP(0x01 + X));  /* 008FAB LDA */
    mflag = 1;  /* 008FAD SEM */
    SETLO(A, A + 1);  /* 008FAE INC */
    if ((A & 0xFF) == 0) goto L_008FB4;  /* 008FAF BEQ */
    wr8(DP(0x0B), 0x01);  /* 008FB1 LDM */
L_008FB4: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008FB4 INC */
    SETLO(A, rd8(DP(0x00)));  /* 008FB6 LDA */
    /* 008FB8 CMP */
    if ((A & 0xFF) >= 0x05) goto L_008FC0;  /* 008FBA BCS */
    SETLO(A, rd8(DP(0x0B)));  /* 008FBC LDA */
    if ((A & 0xFF) == 0) goto L_008F92;  /* 008FBE BEQ */
L_008FC0: TICK();
    SETLO(A, rd8(DP(0x0B)));  /* 008FC0 LDA */
    if ((A & 0xFF) == 0) goto L_008FE2;  /* 008FC2 BEQ */
    wr8(DP(0x0A), 0x01);  /* 008FC4 LDM */
    wr8(DP(0x00), 0x00);  /* 008FC7 LDM */
L_008FCA: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008FCA LDA */
    /* 008FCC CMP */
    if ((A & 0xFF) >= 0x05) goto L_008FDF;  /* 008FCE BCS */
    mflag = 0;  /* 008FD0 CLM */
    A &= 0x00FF;  /* 008FD1 AND */
    X = A;  /* 008FD4 TAX */
    mflag = 1;  /* 008FD5 SEM */
    wr8((ABS(0x025A) + X), 0xFF);  /* 008FD6 LDM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 008FDA INC */
    goto L_008FCA;  /* 008FDC JMP */
L_008FDF: TICK();
    goto L_00906B;  /* 008FDF JMP */
L_008FE2: TICK();
    mflag = 1;  /* 008FE2 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_008FED;  /* 008FE3 BBS */
    if ((rd8(ABS(0x033F)) & 0x08) == 0) goto L_00906B;  /* 008FE8 BBC */
L_008FED: TICK();
    wr8(DP(0x00), 0x00);  /* 008FED LDM */
L_008FF0: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 008FF0 LDA */
    /* 008FF2 CMP */
    if ((A & 0xFF) >= 0x05) goto L_009020;  /* 008FF4 BCS */
    mflag = 0;  /* 008FF6 CLM */
    A &= 0x00FF;  /* 008FF7 AND */
    A += 1;  /* 008FFA INC */
    A += 1;  /* 008FFB INC */
    X = A;  /* 008FFC TAX */
    A = rd16(DP(0x00));  /* 008FFD LDA */
    A &= 0x00FF;  /* 008FFF AND */
    Y = A;  /* 009002 TAY */
    A = rd16(DP(0x01 + X));  /* 009003 LDA */
    mflag = 1;  /* 009005 SEM */
    wr8((ABS(0x025A) + Y), (uint8_t)A);  /* 009006 STA */
    SETLO(A, rd8(DP(0x00)));  /* 009009 LDA */
    mflag = 0;  /* 00900B CLM */
    A &= 0x00FF;  /* 00900C AND */
    A += 1;  /* 00900F INC */
    A += 1;  /* 009010 INC */
    X = A;  /* 009011 TAX */
    A = rd16(DP(0x01 + X));  /* 009012 LDA */
    mflag = 1;  /* 009014 SEM */
    SETLO(A, A + 1);  /* 009015 INC */
    if ((A & 0xFF) == 0) goto L_00901B;  /* 009016 BEQ */
    wr8(DP(0x0B), 0x01);  /* 009018 LDM */
L_00901B: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00901B INC */
    goto L_008FF0;  /* 00901D JMP */
L_009020: TICK();
    mflag = 1;  /* 009020 SEM */
    SETLO(A, rd8(DP(0x0B)));  /* 009021 LDA */
    if ((A & 0xFF) == 0) goto L_00906B;  /* 009023 BEQ */
    wr8(DP(0x00), 0x00);  /* 009025 LDM */
L_009028: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 009028 LDA */
    /* 00902A CMP */
    if ((A & 0xFF) >= 0x05) goto L_009042;  /* 00902C BCS */
    mflag = 0;  /* 00902E CLM */
    A &= 0x00FF;  /* 00902F AND */
    A += 1;  /* 009032 INC */
    A += 1;  /* 009033 INC */
    X = A;  /* 009034 TAX */
    mflag = 1;  /* 009035 SEM */
    SETLO(A, rd8(DP(0x01 + X)));  /* 009036 LDA */
    if ((A & 0xFF) == 0) goto L_00903D;  /* 009038 BEQ */
    wr8(DP(0x0B), 0x00);  /* 00903A LDM */
L_00903D: TICK();
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00903D INC */
    goto L_009028;  /* 00903F JMP */
L_009042: TICK();
    mflag = 1;  /* 009042 SEM */
    SETLO(A, rd8(DP(0x0B)));  /* 009043 LDA */
    if ((A & 0xFF) == 0) goto L_009065;  /* 009045 BEQ */
    wr8(DP(0x0A), 0x01);  /* 009047 LDM */
    wr8(DP(0x00), 0x00);  /* 00904A LDM */
L_00904D: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00904D LDA */
    /* 00904F CMP */
    if ((A & 0xFF) >= 0x05) goto L_009062;  /* 009051 BCS */
    mflag = 0;  /* 009053 CLM */
    A &= 0x00FF;  /* 009054 AND */
    X = A;  /* 009057 TAX */
    mflag = 1;  /* 009058 SEM */
    wr8((ABS(0x025A) + X), 0xFF);  /* 009059 LDM */
    t = (rd8(DP(0x00)) + 1) & 0xFF; wr8(DP(0x00), t);  /* 00905D INC */
    goto L_00904D;  /* 00905F JMP */
L_009062: TICK();
    goto L_00906B;  /* 009062 JMP */
L_009065: TICK();
    mflag = 0;  /* 009065 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0004);  /* 009066 SEB */
L_00906B: TICK();
    mflag = 1;  /* 00906B SEM */
    if ((rd8(DP(0x01)) & 0x01) == 0) goto L_009076;  /* 00906C BBC */
    mflag = 0;  /* 009070 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x1000);  /* 009071 SEB */
L_009076: TICK();
    mflag = 1;  /* 009076 SEM */
    if ((rd8(DP(0x01)) & 0x02) == 0) goto L_009081;  /* 009077 BBC */
    mflag = 0;  /* 00907B CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0800);  /* 00907C SEB */
L_009081: TICK();
    mflag = 1;  /* 009081 SEM */
    if ((rd8(DP(0x01)) & 0x04) == 0) goto L_00908C;  /* 009082 BBC */
    mflag = 0;  /* 009086 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x4000);  /* 009087 SEB */
L_00908C: TICK();
    mflag = 1;  /* 00908C SEM */
    if ((rd8(DP(0x01)) & 0x08) == 0) goto L_009097;  /* 00908D BBC */
    mflag = 0;  /* 009091 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0100);  /* 009092 SEB */
L_009097: TICK();
    mflag = 1;  /* 009097 SEM */
    if ((rd8(DP(0x01)) & 0x10) == 0) goto L_0090A2;  /* 009098 BBC */
    mflag = 0;  /* 00909C CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0002);  /* 00909D SEB */
L_0090A2: TICK();
    mflag = 1;  /* 0090A2 SEM */
    if ((rd8(DP(0x01)) & 0x20) == 0) goto L_0090AD;  /* 0090A3 BBC */
    mflag = 0;  /* 0090A7 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0100);  /* 0090A8 SEB */
L_0090AD: TICK();
    mflag = 1;  /* 0090AD SEM */
    if ((rd8(DP(0x01)) & 0x40) == 0) goto L_0090C5;  /* 0090AE BBC */
    mflag = 0;  /* 0090B2 CLM */
    S -= 3; F_008D47();  /* 0090B3 JSRL */
    /* 0090B7 CMP */
    if ((A & 0xFF) == 0x00) goto L_0090C0;  /* 0090B9 BEQ */
    SETLO(A, 0x01);  /* 0090BB LDA */
    goto L_0090C2;  /* 0090BD JMP */
L_0090C0: TICK();
    SETLO(A, 0x02);  /* 0090C0 LDA */
L_0090C2: TICK();
    wr8(ABS(0x01EB), (uint8_t)A);  /* 0090C2 STA */
L_0090C5: TICK();
    if ((rd8(DP(0x01)) & 0x80) == 0) goto L_0090CF;  /* 0090C5 BBC */
    mflag = 0;  /* 0090C9 CLM */
    wr16(ABS(0x0246), rd16(ABS(0x0246)) | 0x0020);  /* 0090CA SEB */
L_0090CF: TICK();
    mflag = 1;  /* 0090CF SEM */
    if ((rd8(ABS(0x033F)) & 0x02) == 0x02) goto L_0090DA;  /* 0090D0 BBS */
    if ((rd8(ABS(0x0334)) & 0x20) == 0) goto L_0090DF;  /* 0090D5 BBC */
L_0090DA: TICK();
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0090F0;  /* 0090DA BBS */
L_0090DF: TICK();
    mflag = 0;  /* 0090DF CLM */
    if ((rd16(ABS(0x0240)) & 0x0100) == 0) goto L_0090F0;  /* 0090E0 BBC */
    mflag = 1;  /* 0090E6 SEM */
    wr8(DP(0x0A), 0x01);  /* 0090E7 LDM */
    mflag = 0;  /* 0090EA CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) & ~0x0100);  /* 0090EB CLB */
L_0090F0: TICK();
    mflag = 1;  /* 0090F0 SEM */
    SETLO(A, rd8(DP(0x0A)));  /* 0090F1 LDA */
    if ((A & 0xFF) == 0) goto L_0090FA;  /* 0090F3 BEQ */
    mflag = 0;  /* 0090F5 CLM */
    S -= 3; F_008DED();  /* 0090F6 JSRL */
L_0090FA: TICK();
    mflag = 0;  /* 0090FA CLM */
    Y = pull16();  /* 0090FB PLY */
    D = pull16();  /* 0090FC PLD */
    S += 3; return;  /* 0090FD RTL */
}

/* 0090FE  callers: 0036E3, 0091B0 */
void F_0090FE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0090FE LDA */
    push16(D);  /* 009100 PHD */
    D = A;  /* 009101 TAD */
    C = 0;  /* 009102 CLC */
    A = adc16(A, 0x0004);  /* 009103 ADC */
    push16(A);  /* 009106 PHA */
    wr16(DP(0x00), Y);  /* 009107 STY */
    if ((rd16(ABS(0x023A)) & 0x0400) == 0) goto L_009131;  /* 009109 BBC */
    A = Y;  /* 00910F TYA */
    mflag = 1;  /* 009110 SEM */
    wr8(DP(0x03), (uint8_t)A);  /* 009111 STA */
    wr8(DP(0x02), (uint8_t)A);  /* 009113 STA */
    wr8(DP(0x01), (uint8_t)A);  /* 009115 STA */
    wr8(DP(0x04), 0x03);  /* 009117 LDM */
    mflag = 0;  /* 00911A CLM */
    A = D;  /* 00911B TDA */
    A += 1;  /* 00911C INC */
    B = 0x0000;  /* 00911D LDA */
    wr16(DP(0x05), A);  /* 009121 STA */
    wr16(DP(0x07), B);  /* 009123 STA */
    wr16(DP(0x08), 0x0548);  /* 009126 LDM */
    Y = 0x00A0;  /* 00912A LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00912D JSRL */
L_009131: TICK();
    Y = pull16();  /* 009131 PLY */
    D = pull16();  /* 009132 PLD */
    S += 3; return;  /* 009133 RTL */
}

/* 009134  callers: 004D4C, 00E468, 016C16 */
void F_009134(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 009134 LDA */
    push16(D);  /* 009136 PHD */
    D = A;  /* 009137 TAD */
    A += 1;  /* 009138 INC */
    A += 1;  /* 009139 INC */
    A += 1;  /* 00913A INC */
    push16(A);  /* 00913B PHA */
    wr16(DP(0x03), 0x0003);  /* 00913C LDM */
    A = D;  /* 009140 TDA */
    B = 0x0000;  /* 009141 LDA */
    wr16(DP(0x04), A);  /* 009145 STA */
    wr16(DP(0x06), B);  /* 009147 STA */
    wr16(DP(0x07), 0x0548);  /* 00914A LDM */
    Y = 0x00A0;  /* 00914E LDY */
    S -= 3; i2c_block_xfer_a_0072EE();  /* 009151 JSRL */
    mflag = 1;  /* 009155 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 009156 LDA */
    if ((A & 0xFF) == 0) goto L_00916A;  /* 009158 BEQ */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_009164;  /* 00915A BBC */
    SETLO(A, 0x0A);  /* 00915F LDA */
    goto L_009166;  /* 009161 JMP */
L_009164: TICK();
    SETLO(A, 0x63);  /* 009164 LDA */
L_009166: TICK();
    t = rd8(DP(0x00));  /* 009166 CMP */
    if ((A & 0xFF) >= t) goto L_00916D;  /* 009168 BCS */
L_00916A: TICK();
    wr8(DP(0x00), 0x01);  /* 00916A LDM */
L_00916D: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 00916D LDA */
    if ((A & 0xFF) == 0) goto L_009181;  /* 00916F BEQ */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00917B;  /* 009171 BBC */
    SETLO(A, 0x0A);  /* 009176 LDA */
    goto L_00917D;  /* 009178 JMP */
L_00917B: TICK();
    SETLO(A, 0x63);  /* 00917B LDA */
L_00917D: TICK();
    t = rd8(DP(0x01));  /* 00917D CMP */
    if ((A & 0xFF) >= t) goto L_009184;  /* 00917F BCS */
L_009181: TICK();
    wr8(DP(0x01), 0x01);  /* 009181 LDM */
L_009184: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 009184 LDA */
    if ((A & 0xFF) == 0) goto L_009198;  /* 009186 BEQ */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_009192;  /* 009188 BBC */
    SETLO(A, 0x0A);  /* 00918D LDA */
    goto L_009194;  /* 00918F JMP */
L_009192: TICK();
    SETLO(A, 0x63);  /* 009192 LDA */
L_009194: TICK();
    t = rd8(DP(0x02));  /* 009194 CMP */
    if ((A & 0xFF) >= t) goto L_00919B;  /* 009196 BCS */
L_009198: TICK();
    wr8(DP(0x02), 0x01);  /* 009198 LDM */
L_00919B: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 00919B LDA */
    t = rd8(DP(0x01));  /* 00919D CMP */
    if ((A & 0xFF) != t) goto L_0091A5;  /* 00919F BNE */
    t = rd8(DP(0x02));  /* 0091A1 CMP */
    if ((A & 0xFF) == t) goto L_0091B4;  /* 0091A3 BEQ */
L_0091A5: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0091A5 LDA */
    t = rd8(DP(0x02));  /* 0091A7 CMP */
    if ((A & 0xFF) != t) goto L_0091AD;  /* 0091A9 BNE */
    wr8(DP(0x00), (uint8_t)A);  /* 0091AB STA */
L_0091AD: TICK();
    Y = rd16(DP(0x00));  /* 0091AD LDY */
    mflag = 0;  /* 0091AF CLM */
    S -= 3; F_0090FE();  /* 0091B0 JSRL */
L_0091B4: TICK();
    mflag = 0;  /* 0091B4 CLM */
    A = rd16(DP(0x00));  /* 0091B5 LDA */
    mflag = 1;  /* 0091B7 SEM */
    Y = pull16();  /* 0091B8 PLY */
    D = pull16();  /* 0091B9 PLD */
    S += 3; return;  /* 0091BA RTL */
}

/* 0091BB  callers: 0044E7, 009258, 00D642 */
void F_0091BB(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0091BB LDA */
    push16(D);  /* 0091BD PHD */
    D = A;  /* 0091BE TAD */
    A += 1;  /* 0091BF INC */
    A += 1;  /* 0091C0 INC */
    A += 1;  /* 0091C1 INC */
    push16(A);  /* 0091C2 PHA */
    wr16(DP(0x00), Y);  /* 0091C3 STY */
    wr16(DP(0x01), 0x0000);  /* 0091C5 LDM */
    A = Y;  /* 0091C9 TYA */
    A &= 0x00FF;  /* 0091CA AND */
    A -= 1;  /* 0091CD DEC */
    mpy16(0x000D);  /* 0091CE MPY */
    C = 0;  /* 0091D2 CLC */
    A = adc16(A, 0x0032);  /* 0091D3 ADC */
    Y = A;  /* 0091D6 TAY */
    S -= 3; eeprom_read_byte_007189();  /* 0091D7 JSRL */
    SETLO(A, A & 0x3F);  /* 0091DB AND */
    if ((A & 0xFF) == 0) goto L_0091E4;  /* 0091DD BEQ */
    mflag = 0;  /* 0091DF CLM */
    wr16(DP(0x01), 0x0001);  /* 0091E0 LDM */
L_0091E4: TICK();
    mflag = 0;  /* 0091E4 CLM */
    A = rd16(DP(0x01));  /* 0091E5 LDA */
    Y = pull16();  /* 0091E7 PLY */
    D = pull16();  /* 0091E8 PLD */
    S += 3; return;  /* 0091E9 RTL */
}

/* 0091EA  callers: 004975 */
void F_0091EA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0091EA LDA */
    push16(D);  /* 0091EC PHD */
    D = A;  /* 0091ED TAD */
    C = 0;  /* 0091EE CLC */
    A = adc16(A, 0x000C);  /* 0091EF ADC */
    push16(A);  /* 0091F2 PHA */
    wr16(DP(0x00), 0x0000);  /* 0091F3 LDM */
    wr16(DP(0x02), 0x0001);  /* 0091F7 LDM */
L_0091FB: TICK();
    mflag = 1;  /* 0091FB SEM */
    wr8(ABS(WDT), 0x00);  /* 0091FC LDM */
    mflag = 0;  /* 009200 CLM */
    wr16(DP(0x0C), 0x0008);  /* 009201 LDM */
    A = rd16(DP(0x02));  /* 009205 LDA */
    A -= 1;  /* 009207 DEC */
    wr16(DP(0x0E), A);  /* 009208 STA */
    A = D;  /* 00920A TDA */
    C = 0;  /* 00920B CLC */
    A = adc16(A, 0x0008);  /* 00920C ADC */
    B = 0x0000;  /* 00920F LDA */
    Y = A;  /* 009213 TAY */
    S -= 3; F_01E30B();  /* 009214 JSRL */
    wr16(DP(0x0C), A);  /* 009218 STA */
    A = D;  /* 00921A TDA */
    C = 0;  /* 00921B CLC */
    A = adc16(A, 0x0004);  /* 00921C ADC */
    wr16(DP(0x0E), A);  /* 00921F STA */
    mflag = 1;  /* 009221 SEM */
    wr8(DP(0x10), (uint8_t)B);  /* 009222 STA */
    mflag = 0;  /* 009225 CLM */
    A = rd16(DP(0x0C));  /* 009226 LDA */
    wr16(DP(0x11), A);  /* 009228 STA */
    A = rd16(DP(0x10));  /* 00922A LDA */
    mflag = 1;  /* 00922C SEM */
    wr8(DP(0x13), (uint8_t)A);  /* 00922D STA */
    mflag = 0;  /* 00922F CLM */
    A = rd16(DP(0x0E));  /* 009230 LDA */
    wr16(DP(0x14), A);  /* 009232 STA */
    mflag = 1;  /* 009234 SEM */
    wr8(DP(0x16), 0x00);  /* 009235 LDM */
    mflag = 0;  /* 009238 CLM */
    A = 0x0011;  /* 009239 LDA */
    Y = 0x0004;  /* 00923C LDY */
    S -= 3; rt_far_memcpy_01E3BA();  /* 00923F JSRL */
    X = rd16(DP(0x04));  /* 009243 LDX */
    Y = X;  /* 009245 TXY */
    X = rd16(DP(0x06));  /* 009246 LDX */
    A = 0x0001;  /* 009248 LDA */
    mflag = 1;  /* 00924B SEM */
    S -= 3; rt_shl_01E4E0();  /* 00924C JSRL */
    SETLO(A, A & rd8((ABS(0x02D2) + Y)));  /* 009250 AND */
    if ((A & 0xFF) == 0) goto L_00925E;  /* 009253 BEQ */
    Y = rd16(DP(0x02));  /* 009255 LDY */
    mflag = 0;  /* 009257 CLM */
    S -= 3; F_0091BB();  /* 009258 JSRL */
    wr16(DP(0x00), A);  /* 00925C STA */
L_00925E: TICK();
    mflag = 0;  /* 00925E CLM */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 00925F INC */
    mflag = 1;  /* 009261 SEM */
    if ((rd8(ABS(0x0334)) & 0x10) == 0) goto L_00926C;  /* 009262 BBC */
    SETLO(A, 0x0A);  /* 009267 LDA */
    goto L_00926E;  /* 009269 JMP */
L_00926C: TICK();
    SETLO(A, 0x63);  /* 00926C LDA */
L_00926E: TICK();
    mflag = 0;  /* 00926E CLM */
    A &= 0x00FF;  /* 00926F AND */
    t = rd16(DP(0x02));  /* 009272 CMP */
    if (A < t) goto L_00927A;  /* 009274 BCC */
    X = rd16(DP(0x00));  /* 009276 LDX */
    if (X == 0) goto L_0091FB;  /* 009278 BEQ */
L_00927A: TICK();
    A = rd16(DP(0x00));  /* 00927A LDA */
    Y = pull16();  /* 00927C PLY */
    D = pull16();  /* 00927D PLD */
    S += 3; return;  /* 00927E RTL */
}

/* 00927F  callers: 003742, 004A0B, 004A55, 004D9F, 005048, 0050B5, 0059AA, 0063E9 ... */
void F_00927F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00927F LDA */
    push16(D);  /* 009281 PHD */
    D = A;  /* 009282 TAD */
    C = 0;  /* 009283 CLC */
    A = adc16(A, 0x0008);  /* 009284 ADC */
    push16(A);  /* 009287 PHA */
    wr16(DP(0x00), Y);  /* 009288 STY */
    S -= 3; F_01E110();  /* 00928A JSRL */
    X = A;  /* 00928E TAX */
    if (X == 0) goto L_009294;  /* 00928F BEQ */
    goto L_0095F3;  /* 009291 JMP */
L_009294: TICK();
    mflag = 1;  /* 009294 SEM */
    if ((rd8(ABS(0x023C)) & 0x01) == 0) goto L_0092A1;  /* 009295 BBC */
    mflag = 0;  /* 00929A CLM */
    A = 0x0001;  /* 00929B LDA */
    goto L_0092A3;  /* 00929E JMP */
L_0092A1: TICK();
    mflag = 0;  /* 0092A1 CLM */
    A = X;  /* 0092A2 TXA */
L_0092A3: TICK();
    wr16(DP(0x04), A);  /* 0092A3 STA */
    wr16(DP(0x02), A);  /* 0092A5 STA */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_0092B3;  /* 0092A7 BBC */
    A = 0x0001;  /* 0092AD LDA */
    goto L_0092B4;  /* 0092B0 JMP */
L_0092B3: TICK();
    A = X;  /* 0092B3 TXA */
L_0092B4: TICK();
    wr16(DP(0x06), A);  /* 0092B4 STA */
    A = rd16(DP(0x00));  /* 0092B6 LDA */
    /* 0092B8 CMP */
    if (A < 0x0007) goto L_0092C0;  /* 0092BB BCC */
    goto L_009555;  /* 0092BD JMP */
L_0092C0: TICK();
    A = asl16(A);  /* 0092C0 ASL */
    X = A;  /* 0092C1 TAX */
    switch (X) {   /* jump table 0092C5 */  /* 0092C2 JMP */
        case 0x00: goto L_0092F3;
        case 0x02: goto L_009308;
        case 0x04: goto L_009386;
        case 0x06: goto L_009398;
        case 0x08: goto L_0093C9;
        case 0x0A: goto L_009414;
        case 0x0C: goto L_0092D3;
        default: bad_dispatch(0x0092C2); return;
    }
L_0092D3: TICK();
    mflag = 1;  /* 0092D3 SEM */
    if ((rd8(ABS(0x033F)) & 0x20) == 0) goto L_0092F0;  /* 0092D4 BBC */
    mflag = 0;  /* 0092D9 CLM */
    if ((rd16(ABS(0x023A)) & 0x0020) == 0x0020) goto L_0092F0;  /* 0092DA BBS */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0800);  /* 0092E0 SEB */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0) goto L_0092F0;  /* 0092E5 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 0092EB SEB */
L_0092F0: TICK();
    goto L_009555;  /* 0092F0 JMP */
L_0092F3: TICK();
    mflag = 0;  /* 0092F3 CLM */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0) goto L_009300;  /* 0092F4 BBC */
    if ((rd16(ABS(0x023A)) & 0x1000) == 0) goto L_009305;  /* 0092FA BBC */
L_009300: TICK();
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 009300 CLB */
L_009305: TICK();
    goto L_009555;  /* 009305 JMP */
L_009308: TICK();
    mflag = 0;  /* 009308 CLM */
    A = rd16(ABS(0x0244));  /* 009309 LDA */
    A &= 0x003F;  /* 00930C AND */
    if (A != 0) goto L_009383;  /* 00930F BNE */
    if ((rd16(ABS(0x0240)) & 0x0020) == 0x0020) goto L_009324;  /* 009311 BBS */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 009317 CLB */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 00931C CLB */
    goto L_009383;  /* 009321 JMP */
L_009324: TICK();
    mflag = 0;  /* 009324 CLM */
    if ((rd16(ABS(0x023C)) & 0x0800) == 0) goto L_009383;  /* 009325 BBC */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_00934D;  /* 00932B BBS */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0) goto L_00933C;  /* 009331 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 009337 SEB */
L_00933C: TICK();
    A = rd16(ABS(0x0375));  /* 00933C LDA */
    mflag = 1;  /* 00933F SEM */
    SETLO(A, A & 0x3F);  /* 009340 AND */
    if ((A & 0xFF) == 0) goto L_00934A;  /* 009342 BEQ */
    mflag = 0;  /* 009344 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009345 SEB */
L_00934A: TICK();
    goto L_009383;  /* 00934A JMP */
L_00934D: TICK();
    mflag = 0;  /* 00934D CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_00936E;  /* 00934E BBC */
    A = rd16(ABS(0x0368));  /* 009354 LDA */
    mflag = 1;  /* 009357 SEM */
    SETLO(A, A & 0x3F);  /* 009358 AND */
    if ((A & 0xFF) != 0) goto L_009365;  /* 00935A BNE */
    mflag = 0;  /* 00935C CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00935D CLB */
    goto L_00936B;  /* 009362 JMP */
L_009365: TICK();
    mflag = 0;  /* 009365 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009366 SEB */
L_00936B: TICK();
    goto L_009383;  /* 00936B JMP */
L_00936E: TICK();
    mflag = 1;  /* 00936E SEM */
    if ((rd8(ABS(0x0246)) & 0x10) == 0) goto L_00937D;  /* 00936F BBC */
    mflag = 0;  /* 009374 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009375 SEB */
    goto L_009383;  /* 00937A JMP */
L_00937D: TICK();
    mflag = 0;  /* 00937D CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00937E CLB */
L_009383: TICK();
    goto L_009555;  /* 009383 JMP */
L_009386: TICK();
    mflag = 0;  /* 009386 CLM */
    A = rd16(ABS(0x0244));  /* 009387 LDA */
    mflag = 1;  /* 00938A SEM */
    SETLO(A, A & 0x3F);  /* 00938B AND */
    if ((A & 0xFF) == 0) goto L_009395;  /* 00938D BEQ */
    mflag = 0;  /* 00938F CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 009390 CLB */
L_009395: TICK();
    goto L_009555;  /* 009395 JMP */
L_009398: TICK();
    mflag = 0;  /* 009398 CLM */
    A = rd16(ABS(0x0229));  /* 009399 LDA */
    mflag = 1;  /* 00939C SEM */
    /* 00939D CMP */
    if ((A & 0xFF) == 0x04) goto L_0093C6;  /* 00939F BEQ */
    SETLO(A, rd8(ABS(0x0229)));  /* 0093A1 LDA */
    /* 0093A4 CMP */
    if ((A & 0xFF) == 0x03) goto L_0093C6;  /* 0093A6 BEQ */
    mflag = 0;  /* 0093A8 CLM */
    if ((rd16(ABS(0x023C)) & 0x0800) == 0) goto L_0093C6;  /* 0093A9 BBC */
    A = rd16(ABS(0x0375));  /* 0093AF LDA */
    mflag = 1;  /* 0093B2 SEM */
    SETLO(A, A & 0x3F);  /* 0093B3 AND */
    if ((A & 0xFF) != 0) goto L_0093C0;  /* 0093B5 BNE */
    mflag = 0;  /* 0093B7 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 0093B8 CLB */
    goto L_0093C6;  /* 0093BD JMP */
L_0093C0: TICK();
    mflag = 0;  /* 0093C0 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 0093C1 SEB */
L_0093C6: TICK();
    goto L_009555;  /* 0093C6 JMP */
L_0093C9: TICK();
    mflag = 0;  /* 0093C9 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_0093F0;  /* 0093CA BBS */
    if ((rd16(ABS(0x023C)) & 0x0800) == 0) goto L_0093ED;  /* 0093D0 BBC */
    A = rd16(ABS(0x0375));  /* 0093D6 LDA */
    mflag = 1;  /* 0093D9 SEM */
    SETLO(A, A & 0x3F);  /* 0093DA AND */
    if ((A & 0xFF) != 0) goto L_0093E7;  /* 0093DC BNE */
    mflag = 0;  /* 0093DE CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 0093DF CLB */
    goto L_0093ED;  /* 0093E4 JMP */
L_0093E7: TICK();
    mflag = 0;  /* 0093E7 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 0093E8 SEB */
L_0093ED: TICK();
    goto L_009411;  /* 0093ED JMP */
L_0093F0: TICK();
    mflag = 0;  /* 0093F0 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 0093F1 CLB */
    if ((rd16(ABS(0x023C)) & 0x0800) == 0) goto L_009411;  /* 0093F6 BBC */
    mflag = 1;  /* 0093FC SEM */
    if ((rd8(ABS(0x0246)) & 0x10) == 0) goto L_00940B;  /* 0093FD BBC */
    mflag = 0;  /* 009402 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009403 SEB */
    goto L_009411;  /* 009408 JMP */
L_00940B: TICK();
    mflag = 0;  /* 00940B CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00940C CLB */
L_009411: TICK();
    goto L_009555;  /* 009411 JMP */
L_009414: TICK();
    mflag = 0;  /* 009414 CLM */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0) goto L_00941E;  /* 009415 BBC */
    goto L_0094C0;  /* 00941B JMP */
L_00941E: TICK();
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_009440;  /* 00941E BBC */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0) goto L_00942F;  /* 009424 BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 00942A SEB */
L_00942F: TICK();
    A = rd16(ABS(0x0375));  /* 00942F LDA */
    mflag = 1;  /* 009432 SEM */
    SETLO(A, A & 0x3F);  /* 009433 AND */
    if ((A & 0xFF) == 0) goto L_00943D;  /* 009435 BEQ */
    mflag = 0;  /* 009437 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009438 SEB */
L_00943D: TICK();
    goto L_0094BD;  /* 00943D JMP */
L_009440: TICK();
    mflag = 0;  /* 009440 CLM */
    A = rd16(ABS(0x0375));  /* 009441 LDA */
    mflag = 1;  /* 009444 SEM */
    SETLO(A, A & 0x3F);  /* 009445 AND */
    if ((A & 0xFF) != 0) goto L_009475;  /* 009447 BNE */
    mflag = 0;  /* 009449 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00944A CLB */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0x8000) goto L_00945D;  /* 00944F BBS */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 009455 CLB */
    goto L_009472;  /* 00945A JMP */
L_00945D: TICK();
    mflag = 0;  /* 00945D CLM */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_00946C;  /* 00945E BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 009464 CLB */
    goto L_009472;  /* 009469 JMP */
L_00946C: TICK();
    mflag = 0;  /* 00946C CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 00946D SEB */
L_009472: TICK();
    goto L_0094BD;  /* 009472 JMP */
L_009475: TICK();
    mflag = 0;  /* 009475 CLM */
    if ((rd16(ABS(0x023A)) & 0x8000) == 0x8000) goto L_009498;  /* 009476 BBS */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 00947C CLB */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0) goto L_00948F;  /* 009481 BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 009487 CLB */
    goto L_009495;  /* 00948C JMP */
L_00948F: TICK();
    mflag = 0;  /* 00948F CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009490 SEB */
L_009495: TICK();
    goto L_0094BD;  /* 009495 JMP */
L_009498: TICK();
    mflag = 0;  /* 009498 CLM */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_0094B2;  /* 009499 BBC */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0) goto L_0094B2;  /* 00949F BBC */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x0200);  /* 0094A5 CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 0094AA CLB */
    goto L_0094BD;  /* 0094AF JMP */
L_0094B2: TICK();
    mflag = 0;  /* 0094B2 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) | 0x0200);  /* 0094B3 SEB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 0094B8 SEB */
L_0094BD: TICK();
    goto L_009532;  /* 0094BD JMP */
L_0094C0: TICK();
    mflag = 0;  /* 0094C0 CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_0094FF;  /* 0094C1 BBC */
    A = rd16(ABS(0x0368));  /* 0094C7 LDA */
    mflag = 1;  /* 0094CA SEM */
    SETLO(A, A & 0x3F);  /* 0094CB AND */
    if ((A & 0xFF) == 0) goto L_0094F6;  /* 0094CD BEQ */
    mflag = 0;  /* 0094CF CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_0094DE;  /* 0094D0 BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 0094D6 SEB */
    goto L_0094F3;  /* 0094DB JMP */
L_0094DE: TICK();
    mflag = 0;  /* 0094DE CLM */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0) goto L_0094ED;  /* 0094DF BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 0094E5 CLB */
    goto L_0094F3;  /* 0094EA JMP */
L_0094ED: TICK();
    mflag = 0;  /* 0094ED CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 0094EE SEB */
L_0094F3: TICK();
    goto L_0094FC;  /* 0094F3 JMP */
L_0094F6: TICK();
    mflag = 0;  /* 0094F6 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 0094F7 CLB */
L_0094FC: TICK();
    goto L_009532;  /* 0094FC JMP */
L_0094FF: TICK();
    mflag = 1;  /* 0094FF SEM */
    if ((rd8(ABS(0x0246)) & 0x10) == 0) goto L_00952C;  /* 009500 BBC */
    mflag = 0;  /* 009505 CLM */
    if ((rd16(ABS(0x023A)) & 0x2000) == 0) goto L_009514;  /* 009506 BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 00950C SEB */
    goto L_009529;  /* 009511 JMP */
L_009514: TICK();
    mflag = 0;  /* 009514 CLM */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0) goto L_009523;  /* 009515 BBC */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00951B CLB */
    goto L_009529;  /* 009520 JMP */
L_009523: TICK();
    mflag = 0;  /* 009523 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009524 SEB */
L_009529: TICK();
    goto L_009532;  /* 009529 JMP */
L_00952C: TICK();
    mflag = 0;  /* 00952C CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00952D CLB */
L_009532: TICK();
    mflag = 0;  /* 009532 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x2000);  /* 009533 CLB */
    if ((rd16(ABS(0x023E)) & 0x0001) == 0x0001) goto L_009544;  /* 009538 BBS */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_00954C;  /* 00953E BBC */
L_009544: TICK();
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0800);  /* 009544 SEB */
    goto L_009552;  /* 009549 JMP */
L_00954C: TICK();
    mflag = 0;  /* 00954C CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0800);  /* 00954D CLB */
L_009552: TICK();
    goto L_009555;  /* 009552 JMP */
L_009555: TICK();
    mflag = 1;  /* 009555 SEM */
    if ((rd8(ABS(0x033F)) & 0x40) == 0) goto L_0095BD;  /* 009556 BBC */
    mflag = 0;  /* 00955B CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0001);  /* 00955C CLB */
    if ((rd16(ABS(0x0240)) & 0x0400) == 0x0400) goto L_009578;  /* 009561 BBS */
    A = rd16(ABS(0x0375));  /* 009567 LDA */
    mflag = 1;  /* 00956A SEM */
    SETLO(A, A & 0x3F);  /* 00956B AND */
    if ((A & 0xFF) == 0) goto L_009575;  /* 00956D BEQ */
    mflag = 0;  /* 00956F CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009570 SEB */
L_009575: TICK();
    goto L_00959C;  /* 009575 JMP */
L_009578: TICK();
    mflag = 0;  /* 009578 CLM */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_009590;  /* 009579 BBC */
    A = rd16(ABS(0x0368));  /* 00957F LDA */
    mflag = 1;  /* 009582 SEM */
    SETLO(A, A & 0x3F);  /* 009583 AND */
    if ((A & 0xFF) == 0) goto L_00958D;  /* 009585 BEQ */
    mflag = 0;  /* 009587 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009588 SEB */
L_00958D: TICK();
    goto L_00959C;  /* 00958D JMP */
L_009590: TICK();
    mflag = 1;  /* 009590 SEM */
    if ((rd8(ABS(0x0246)) & 0x10) == 0) goto L_00959C;  /* 009591 BBC */
    mflag = 0;  /* 009596 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) | 0x0001);  /* 009597 SEB */
L_00959C: TICK();
    mflag = 1;  /* 00959C SEM */
    if ((rd8(ABS(0x023E)) & 0x01) == 0) goto L_0095A8;  /* 00959D BBC */
    mflag = 0;  /* 0095A2 CLM */
    wr16(ABS(0x023A), rd16(ABS(0x023A)) & ~0x2000);  /* 0095A3 CLB */
L_0095A8: TICK();
    mflag = 0;  /* 0095A8 CLM */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_0095B5;  /* 0095A9 BBC */
    A = 0x0001;  /* 0095AF LDA */
    goto L_0095B8;  /* 0095B2 JMP */
L_0095B5: TICK();
    A = 0x0000;  /* 0095B5 LDA */
L_0095B8: TICK();
    wr16(DP(0x02), A);  /* 0095B8 STA */
    goto L_0095D6;  /* 0095BA JMP */
L_0095BD: TICK();
    mflag = 1;  /* 0095BD SEM */
    if ((rd8(ABS(0x023E)) & 0x01) == 0x01) goto L_0095CA;  /* 0095BE BBS */
    mflag = 0;  /* 0095C3 CLM */
    if ((rd16(ABS(0x023A)) & 0x0200) == 0) goto L_0095D1;  /* 0095C4 BBC */
L_0095CA: TICK();
    mflag = 0;  /* 0095CA CLM */
    A = 0x0001;  /* 0095CB LDA */
    goto L_0095D4;  /* 0095CE JMP */
L_0095D1: TICK();
    A = 0x0000;  /* 0095D1 LDA */
L_0095D4: TICK();
    wr16(DP(0x02), A);  /* 0095D4 STA */
L_0095D6: TICK();
    t = rd16(DP(0x04));  /* 0095D6 CMP */
    if (A == t) goto L_0095F3;  /* 0095D8 BEQ */
    X = A;  /* 0095DA TAX */
    if (X == 0) goto L_0095E5;  /* 0095DB BEQ */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0001);  /* 0095DD SEB */
    goto L_0095EB;  /* 0095E2 JMP */
L_0095E5: TICK();
    mflag = 0;  /* 0095E5 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0001);  /* 0095E6 CLB */
L_0095EB: TICK();
    S -= 3; F_007A06();  /* 0095EB JSRL */
    S -= 3; F_00828C();  /* 0095EF JSRL */
L_0095F3: TICK();
    S -= 3; F_00CB52();  /* 0095F3 JSRL */
    Y = pull16();  /* 0095F7 PLY */
    D = pull16();  /* 0095F8 PLD */
    S += 3; return;  /* 0095F9 RTL */
}

/* 0095FA  callers: 0049F9, 00508A, 00EEFE, 00FBDE */
void F_0095FA(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0095FA LDA */
    push16(D);  /* 0095FC PHD */
    D = A;  /* 0095FD TAD */
    push16(A);  /* 0095FE PHA */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0) goto L_00960B;  /* 0095FF BBC */
    if ((rd16(ABS(0x0244)) & 0x0400) == 0) goto L_009665;  /* 009605 BBC */
L_00960B: TICK();
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0400);  /* 00960B CLB */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 009610 CLB */
    if ((rd16(ABS(0x0244)) & 0x0100) == 0x0100) goto L_00962D;  /* 009615 BBS */
    wr16(DP(0x00), 0x0356);  /* 00961B LDM */
    wr16(DP(0x02), 0x0363);  /* 00961F LDM */
    A = 0x0000;  /* 009623 LDA */
    Y = 0x000D;  /* 009626 LDY */
    S -= 3; rt_memcpy_01E466();  /* 009629 JSRL */
L_00962D: TICK();
    wr16(DP(0x00), 0x0363);  /* 00962D LDM */
    wr16(DP(0x02), 0x0370);  /* 009631 LDM */
    A = 0x0000;  /* 009635 LDA */
    Y = 0x000D;  /* 009638 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00963B JSRL */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0100);  /* 00963F SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) | 0x0200);  /* 009644 SEB */
    wr16(ABS(0x0244), rd16(ABS(0x0244)) & ~0x0800);  /* 009649 CLB */
    Y = 0x002C;  /* 00964E LDY */
    S -= 3; F_003310();  /* 009651 JSRL */
    S -= 3; F_00828C();  /* 009655 JSRL */
    Y = 0x0003;  /* 009659 LDY */
    S -= 3; F_00927F();  /* 00965C JSRL */
    mflag = 1;  /* 009660 SEM */
    wr8(ABS(0x020A), 0x00);  /* 009661 LDM */
L_009665: TICK();
    mflag = 0;  /* 009665 CLM */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0080);  /* 009666 CLB */
    A = rd16(ABS(0x0344));  /* 00966B LDA */
    mflag = 1;  /* 00966E SEM */
    SETLO(A, A & 0x0F);  /* 00966F AND */
    mflag = 0;  /* 009671 CLM */
    A &= 0x00FF;  /* 009672 AND */
    mpy16(0x01F4);  /* 009675 MPY */
    A = lsr16(A);  /* 009679 LSR */
    A = lsr16(A);  /* 00967A LSR */
    A = lsr16(A);  /* 00967B LSR */
    A = lsr16(A);  /* 00967C LSR */
    A = lsr16(A);  /* 00967D LSR */
    mflag = 1;  /* 00967E SEM */
    SETLO(A, A + 1);  /* 00967F INC */
    wr8(ABS(0x0207), (uint8_t)A);  /* 009680 STA */
    SETLO(A, rd8(ABS(0x034D)));  /* 009683 LDA */
    wr8(ABS(0x0209), (uint8_t)A);  /* 009686 STA */
    mflag = 0;  /* 009689 CLM */
    Y = pull16();  /* 00968A PLY */
    D = pull16();  /* 00968B PLD */
    S += 3; return;  /* 00968C RTL */
}

/* 00968D  callers: 005FAE, 009751, 00E54F, 0105DE, 01A60F, 01B9D8 */
void F_00968D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00968D LDA */
    push16(D);  /* 00968F PHD */
    D = A;  /* 009690 TAD */
    A += 1;  /* 009691 INC */
    A += 1;  /* 009692 INC */
    push16(A);  /* 009693 PHA */
    I = 1;  /* 009694 SEI */
    wr16(DP(0x00), 0x0001);  /* 009695 LDM */
    Y = 0x0001;  /* 009699 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00969C JSRL */
    mflag = 0;  /* 0096A0 CLM */
    wr16(DP(0x00), 0x0010);  /* 0096A1 LDM */
    wr16(DP(0x02), 0x0058);  /* 0096A5 LDM */
    wr16(DP(0x04), 0x0060);  /* 0096A9 LDM */
    Y = 0x0003;  /* 0096AD LDY */
    A = D;  /* 0096B0 TDA */
    C = 0;  /* 0096B1 CLC */
    A = adc16(A, 0x0006);  /* 0096B2 ADC */
    push16(A);  /* 0096B5 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0096B6 JSRL */
    Y = pull16();  /* 0096BA PLY */
    mflag = 0;  /* 0096BB CLM */
    wr16(DP(0x00), 0x0004);  /* 0096BC LDM */
    wr16(DP(0x02), 0x0040);  /* 0096C0 LDM */
    Y = 0x0002;  /* 0096C4 LDY */
    A = D;  /* 0096C7 TDA */
    C = 0;  /* 0096C8 CLC */
    A = adc16(A, 0x0004);  /* 0096C9 ADC */
    push16(A);  /* 0096CC PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0096CD JSRL */
    Y = pull16();  /* 0096D1 PLY */
    mflag = 0;  /* 0096D2 CLM */
    wr16(DP(0x00), 0x0020);  /* 0096D3 LDM */
    wr16(DP(0x02), 0x0013);  /* 0096D7 LDM */
    Y = 0x0002;  /* 0096DB LDY */
    A = D;  /* 0096DE TDA */
    C = 0;  /* 0096DF CLC */
    A = adc16(A, 0x0004);  /* 0096E0 ADC */
    push16(A);  /* 0096E3 PHA */
    S -= 3; cbus_write_sel1_007E4C();  /* 0096E4 JSRL */
    Y = pull16();  /* 0096E8 PLY */
    mflag = 0;  /* 0096E9 CLM */
    wr16(ABS(0x0240), rd16(ABS(0x0240)) | 0x0001);  /* 0096EA SEB */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) | 0x4000);  /* 0096EF SEB */
    Y = 0x0000;  /* 0096F4 LDY */
    A = D;  /* 0096F7 TDA */
    push16(A);  /* 0096F8 PHA */
    S -= 3; F_008156();  /* 0096F9 JSRL */
    Y = pull16();  /* 0096FD PLY */
    Y = 0x0000;  /* 0096FE LDY */
    A = D;  /* 009701 TDA */
    push16(A);  /* 009702 PHA */
    S -= 3; F_00819B();  /* 009703 JSRL */
    Y = pull16();  /* 009707 PLY */
    mflag = 1;  /* 009708 SEM */
    wr8(ABS(P7), rd8(ABS(P7)) | 0x08);  /* 009709 SEB [P73 SICAKLIK(AN3)] */
    wr8(ABS(P4), rd8(ABS(P4)) & ~0x40);  /* 00970D CLB [P46 SUSTURMA_BILGISI] */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x04);  /* 009711 CLB [P72 GONDERME_SES(AN2)] */
    wr8(ABS(P5), rd8(ABS(P5)) & ~0x20);  /* 009715 CLB [P55 ALMA_SEC] */
    wr8(ABS(P7), rd8(ABS(P7)) & ~0x02);  /* 009719 CLB [P71 GUC_ACMA] */
    wr8(ABS(0x0228), 0x0D);  /* 00971D LDM */
    mflag = 0;  /* 009721 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0004);  /* 009722 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0001);  /* 009727 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0010);  /* 00972C CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0020);  /* 009731 CLB */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0040);  /* 009736 CLB */
    Y = pull16();  /* 00973B PLY */
    D = pull16();  /* 00973C PLD */
    S += 3; return;  /* 00973D RTL */
}

/* 00973E  callers: 0059BA, 00E2A5, 00E585, 015CA6, 01740A, 01DD4A */
void F_00973E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00973E LDA */
    push16(A);  /* 009740 PHA */
    Y = 0x0000;  /* 009741 LDY */
    S -= 3; F_007E39();  /* 009744 JSRL */
    Y = 0x0000;  /* 009748 LDY */
    mflag = 0;  /* 00974B CLM */
    S -= 3; F_007E39();  /* 00974C JSRL */
    mflag = 0;  /* 009750 CLM */
    S -= 3; F_00968D();  /* 009751 JSRL */
    mflag = 1;  /* 009755 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_009775;  /* 009756 BBC */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_009775;  /* 00975B BBC [P45 ACIL_CAGRI] */
    Y = 0x000A;  /* 009760 LDY */
    mflag = 0;  /* 009763 CLM */
    S -= 3; F_007D7F();  /* 009764 JSRL */
    mflag = 1;  /* 009768 SEM */
    if ((rd8(ABS(P4)) & 0x20) == 0) goto L_009772;  /* 009769 BBC [P45 ACIL_CAGRI] */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 00976E SEB */
L_009772: TICK();
    goto L_00977A;  /* 009772 JMP */
L_009775: TICK();
    mflag = 1;  /* 009775 SEM */
    wr8(ABS(0x087B), 0xDB);  /* 009776 LDM */
L_00977A: TICK();
    mflag = 0;  /* 00977A CLM */
    ram_trampoline_087A();  /* 00977B JSRL */
    goto L_00977A;  /* 00977F JMP */
}

/* 009782  callers: 00D5A5, 00D5C3, 0132A4, 01698A, 018691, 01DAEC */
void F_009782(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 009782 LDA */
    push16(A);  /* 009784 PHA */
    if ((rd16(ABS(0x0246)) & 0x0008) == 0x0008) goto L_009791;  /* 009785 BBS */
    if ((rd16(ABS(0x0246)) & 0x0004) == 0) goto L_0097A0;  /* 00978B BBC */
L_009791: TICK();
    Y = 0x0012;  /* 009791 LDY */
    S -= 3; F_0080AA();  /* 009794 JSRL */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) | 0x0008);  /* 009798 SEB */
    goto L_0097A6;  /* 00979D JMP */
L_0097A0: TICK();
    mflag = 0;  /* 0097A0 CLM */
    wr16(ABS(0x023C), rd16(ABS(0x023C)) & ~0x0008);  /* 0097A1 CLB */
L_0097A6: TICK();
    S -= 3; F_00828C();  /* 0097A6 JSRL */
    Y = pull16();  /* 0097AA PLY */
    S += 3; return;  /* 0097AB RTL */
}

/* 0097AC  callers: 009D15, 009D21, 009D2E, 009D45, 009D59 */
void F_0097AC(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0097AC LDA */
    push16(D);  /* 0097AE PHD */
    D = A;  /* 0097AF TAD */
    A += 1;  /* 0097B0 INC */
    A += 1;  /* 0097B1 INC */
    push16(A);  /* 0097B2 PHA */
    wr16(DP(0x00), Y);  /* 0097B3 STY */
    A = Y;  /* 0097B5 TYA */
    mflag = 1;  /* 0097B6 SEM */
    SETLO(A, lsr8(A));  /* 0097B7 LSR */
    SETLO(A, lsr8(A));  /* 0097B8 LSR */
    SETLO(A, lsr8(A));  /* 0097B9 LSR */
    SETLO(A, lsr8(A));  /* 0097BA LSR */
    wr8(DP(0x01), (uint8_t)A);  /* 0097BB STA */
    C = ((A & 0xFF) >= 0x0A);  /* 0097BD CMP */
    if (C) goto L_0097C6;  /* 0097BF BCS */
    SETLO(A, adc8(A, 0x30));  /* 0097C1 ADC */
    goto L_0097CB;  /* 0097C3 JMP */
L_0097C6: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 0097C6 LDA */
    C = 0;  /* 0097C8 CLC */
    SETLO(A, adc8(A, 0x37));  /* 0097C9 ADC */
L_0097CB: TICK();
    mflag = 0;  /* 0097CB CLM */
    Y = A;  /* 0097CC TAY */
    S -= 3; F_01058E();  /* 0097CD JSRL */
    mflag = 1;  /* 0097D1 SEM */
    wr8(DP(0x00), rd8(DP(0x00)) & ~0xF0);  /* 0097D2 CLB */
    SETLO(A, rd8(DP(0x00)));  /* 0097D5 LDA */
    C = ((A & 0xFF) >= 0x0A);  /* 0097D7 CMP */
    if (C) goto L_0097E0;  /* 0097D9 BCS */
    SETLO(A, adc8(A, 0x30));  /* 0097DB ADC */
    goto L_0097E5;  /* 0097DD JMP */
L_0097E0: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0097E0 LDA */
    C = 0;  /* 0097E2 CLC */
    SETLO(A, adc8(A, 0x37));  /* 0097E3 ADC */
L_0097E5: TICK();
    mflag = 0;  /* 0097E5 CLM */
    Y = A;  /* 0097E6 TAY */
    S -= 3; F_01058E();  /* 0097E7 JSRL */
    Y = pull16();  /* 0097EB PLY */
    D = pull16();  /* 0097EC PLD */
    S += 3; return;  /* 0097ED RTL */
}

/* 0097EE  callers: 0097F3, 0097FC, 009809, 009812, 009829, 00988E, 009893, 009A28 */
void F_0097EE(void)
{
    uint32_t t; (void)t;
L_0097EE: TICK();
    mflag = 1;  /* 0097EE SEM */
    wr8(ABS(WDT), 0x00);  /* 0097EF LDM */
    if ((rd8(ABS(U0C1)) & 0x08) == 0) goto L_0097EE;  /* 0097F3 BBC */
    SETLO(A, rd8(ABS(U0RB)));  /* 0097F8 LDA */
    S += 3; return;  /* 0097FB RTL */
}

/* 0097FC  callers: 009805, 009A37, 009B85, 009D6B, 009E04 */
void F_0097FC(void)
{
    uint32_t t; (void)t;
L_0097FC: TICK();
    S -= 3; F_0097EE();  /* 0097FC JSRL */
    /* 009800 CMP */
    if ((A & 0xFF) == 0x53) goto L_009808;  /* 009802 BEQ */
    mflag = 0;  /* 009804 CLM */
    goto L_0097FC;  /* 009805 JMP */
L_009808: TICK();
    mflag = 0;  /* 009808 CLM */
    S -= 3; F_0097EE();  /* 009809 JSRL */
    S += 3; return;  /* 00980D RTL */
}

/* 00980E  callers: 00986E, 00992D, 009A47, 009D78, 009E11 */
void F_00980E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00980E LDA */
    push16(D);  /* 009810 PHD */
    D = A;  /* 009811 TAD */
    S -= 3; F_0097EE();  /* 009812 JSRL */
    wr8(DP(0x00), (uint8_t)A);  /* 009816 STA */
    C = ((A & 0xFF) >= 0x41);  /* 009818 CMP */
    if (C) goto L_009821;  /* 00981A BCS */
    SETLO(A, adc8(A, 0xD0));  /* 00981C ADC */
    goto L_009826;  /* 00981E JMP */
L_009821: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 009821 LDA */
    C = 0;  /* 009823 CLC */
    SETLO(A, adc8(A, 0xC9));  /* 009824 ADC */
L_009826: TICK();
    wr8(DP(0x00), (uint8_t)A);  /* 009826 STA */
    mflag = 0;  /* 009828 CLM */
    S -= 3; F_0097EE();  /* 009829 JSRL */
    wr8(DP(0x01), (uint8_t)A);  /* 00982D STA */
    C = ((A & 0xFF) >= 0x41);  /* 00982F CMP */
    if (C) goto L_009843;  /* 009831 BCS */
    SETLO(A, adc8(A, 0xD0));  /* 009833 ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 009835 STA */
    SETLO(A, rd8(DP(0x00)));  /* 009837 LDA */
    SETLO(A, asl8(A));  /* 009839 ASL */
    SETLO(A, asl8(A));  /* 00983A ASL */
    SETLO(A, asl8(A));  /* 00983B ASL */
    SETLO(A, asl8(A));  /* 00983C ASL */
    C = 0;  /* 00983D CLC */
    SETLO(A, adc8(A, rd8(DP(0x02))));  /* 00983E ADC */
    goto L_009853;  /* 009840 JMP */
L_009843: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 009843 LDA */
    C = 0;  /* 009845 CLC */
    SETLO(A, adc8(A, 0xC9));  /* 009846 ADC */
    wr8(DP(0x02), (uint8_t)A);  /* 009848 STA */
    SETLO(A, rd8(DP(0x00)));  /* 00984A LDA */
    SETLO(A, asl8(A));  /* 00984C ASL */
    SETLO(A, asl8(A));  /* 00984D ASL */
    SETLO(A, asl8(A));  /* 00984E ASL */
    SETLO(A, asl8(A));  /* 00984F ASL */
    C = 0;  /* 009850 CLC */
    SETLO(A, adc8(A, rd8(DP(0x02))));  /* 009851 ADC */
L_009853: TICK();
    D = pull16();  /* 009853 PLD */
    S += 3; return;  /* 009854 RTL */
}

/* 009855  callers: 00993D, 009A5A, 009D8B, 009E24 */
void F_009855(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 009855 LDA */
    push16(D);  /* 009857 PHD */
    D = A;  /* 009858 TAD */
    C = 0;  /* 009859 CLC */
    A = adc16(A, 0x0005);  /* 00985A ADC */
    push16(A);  /* 00985D PHA */
    wr16(DP(0x01), Y);  /* 00985E STY */
    wr16(DP(0x03), B);  /* 009860 STA */
    mflag = 1;  /* 009863 SEM */
    wr8(DP(0x04), 0x00);  /* 009864 LDM */
L_009867: TICK();
    SETLO(A, rd8(DP(0x04)));  /* 009867 LDA */
    t = rd8(DP(0x00));  /* 009869 CMP */
    if ((A & 0xFF) >= t) goto L_00988D;  /* 00986B BCS */
    mflag = 0;  /* 00986D CLM */
    S -= 3; F_00980E();  /* 00986E JSRL */
    wr8(DP(0x05), (uint8_t)A);  /* 009872 STA */
    SETLO(A, rd8(DP(0x04)));  /* 009874 LDA */
    mflag = 0;  /* 009876 CLM */
    A &= 0x00FF;  /* 009877 AND */
    C = 0;  /* 00987A CLC */
    A = adc16(A, rd16(DP(0x01)));  /* 00987B ADC */
    wr16(DP(0x06), A);  /* 00987D STA */
    A = rd16(DP(0x03));  /* 00987F LDA */
    mflag = 1;  /* 009881 SEM */
    wr8(DP(0x08), (uint8_t)A);  /* 009882 STA */
    SETLO(A, rd8(DP(0x05)));  /* 009884 LDA */
    wr8(LIND(0x06), (uint8_t)A);  /* 009886 STA */
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 009888 INC */
    goto L_009867;  /* 00988A JMP */
L_00988D: TICK();
    mflag = 0;  /* 00988D CLM */
    S -= 3; F_0097EE();  /* 00988E JSRL */
    mflag = 0;  /* 009892 CLM */
    S -= 3; F_0097EE();  /* 009893 JSRL */
    mflag = 0;  /* 009897 CLM */
    Y = pull16();  /* 009898 PLY */
    D = pull16();  /* 009899 PLD */
    S += 3; return;  /* 00989A RTL */
}

/* 00989B  callers: 00994D, 009A82, 009DA9, 009E39 */
void F_00989B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00989B LDA */
    push16(D);  /* 00989D PHD */
    D = A;  /* 00989E TAD */
    wr16(DP(0x01), Y);  /* 00989F STY */
    wr16(DP(0x03), B);  /* 0098A1 STA */
    A = rd16(DP(0x00));  /* 0098A4 LDA */
    mflag = 1;  /* 0098A6 SEM */
    SETLO(A, A + 1);  /* 0098A7 INC */
    wr8(DP(0x04), (uint8_t)A);  /* 0098A8 STA */
    wr8(DP(0x05), 0x00);  /* 0098AA LDM */
L_0098AD: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0098AD LDA */
    t = rd8(DP(0x05));  /* 0098AF CMP */
    if ((A & 0xFF) < t) goto L_0098CD;  /* 0098B1 BCC */
    SETLO(B, rd8(DP(0x03)));  /* 0098B3 LDA */
    mflag = 0;  /* 0098B6 CLM */
    A = rd16(DP(0x01));  /* 0098B7 LDA */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 0098B9 INC */
    wr16(DP(0x06), A);  /* 0098BB STA */
    mflag = 1;  /* 0098BD SEM */
    wr8(DP(0x08), (uint8_t)B);  /* 0098BE STA */
    SETLO(A, rd8(DP(0x04)));  /* 0098C1 LDA */
    C = 0;  /* 0098C3 CLC */
    SETLO(A, adc8(A, rd8(LIND(0x06))));  /* 0098C4 ADC */
    wr8(DP(0x04), (uint8_t)A);  /* 0098C6 STA */
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 0098C8 INC */
    goto L_0098AD;  /* 0098CA JMP */
L_0098CD: TICK();
    mflag = 0;  /* 0098CD CLM */
    A = rd16(DP(0x04));  /* 0098CE LDA */
    mflag = 1;  /* 0098D0 SEM */
    /* 0098D1 CMP */
    if ((A & 0xFF) != 0xFF) goto L_0098DC;  /* 0098D3 BNE */
    mflag = 0;  /* 0098D5 CLM */
    A = 0x0001;  /* 0098D6 LDA */
    goto L_0098E0;  /* 0098D9 JMP */
L_0098DC: TICK();
    mflag = 0;  /* 0098DC CLM */
    A = 0x0000;  /* 0098DD LDA */
L_0098E0: TICK();
    mflag = 1;  /* 0098E0 SEM */
    D = pull16();  /* 0098E1 PLD */
    S += 3; return;  /* 0098E2 RTL */
}

/* 0098E3  callers: 00995F, 009974, 009A94, 009AAE, 009AE5, 009B09, 009B1E, 009BFB ... */
void F_0098E3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0098E3 LDA */
    push16(D);  /* 0098E5 PHD */
    D = A;  /* 0098E6 TAD */
    C = 0;  /* 0098E7 CLC */
    A = adc16(A, 0x0005);  /* 0098E8 ADC */
    push16(A);  /* 0098EB PHA */
    wr16(DP(0x01), Y);  /* 0098EC STY */
    wr16(DP(0x03), B);  /* 0098EE STA */
    mflag = 1;  /* 0098F1 SEM */
    wr8(DP(0x04), 0x00);  /* 0098F2 LDM */
L_0098F5: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 0098F5 LDA */
    t = rd8(DP(0x04));  /* 0098F7 CMP */
    if ((A & 0xFF) < t) goto L_00990B;  /* 0098F9 BCC */
    mflag = 0;  /* 0098FB CLM */
    A = rd16(LIND(0x01));  /* 0098FC LDA */
    Y = A;  /* 0098FE TAY */
    S -= 3; F_01058E();  /* 0098FF JSRL */
    t = (rd16(DP(0x01)) + 1) & 0xFFFF; wr16(DP(0x01), t);  /* 009903 INC */
    mflag = 1;  /* 009905 SEM */
    t = (rd8(DP(0x04)) + 1) & 0xFF; wr8(DP(0x04), t);  /* 009906 INC */
    goto L_0098F5;  /* 009908 JMP */
L_00990B: TICK();
    Y = 0x000D;  /* 00990B LDY */
    mflag = 0;  /* 00990E CLM */
    S -= 3; F_01058E();  /* 00990F JSRL */
    Y = 0x000A;  /* 009913 LDY */
    S -= 3; F_01058E();  /* 009916 JSRL */
    Y = pull16();  /* 00991A PLY */
    D = pull16();  /* 00991B PLD */
    S += 3; return;  /* 00991C RTL */
}

/* 00991D  callers: 009BA0 */
void F_00991D(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00991D LDA */
    push16(D);  /* 00991F PHD */
    D = A;  /* 009920 TAD */
    C = 0;  /* 009921 CLC */
    A = adc16(A, 0x0006);  /* 009922 ADC */
    push16(A);  /* 009925 PHA */
    wr16(DP(0x03), Y);  /* 009926 STY */
    mflag = 1;  /* 009928 SEM */
    wr8(DP(0x05), (uint8_t)B);  /* 009929 STA */
    mflag = 0;  /* 00992C CLM */
    S -= 3; F_00980E();  /* 00992D JSRL */
    wr8(LIND(0x00), (uint8_t)A);  /* 009931 STA */
    SETLO(A, rd8(LIND(0x00)));  /* 009933 LDA */
    wr8(DP(0x06), (uint8_t)A);  /* 009935 STA */
    SETLO(B, rd8(DP(0x05)));  /* 009937 LDA */
    Y = rd16(DP(0x03));  /* 00993A LDY */
    mflag = 0;  /* 00993C CLM */
    S -= 3; F_009855();  /* 00993D JSRL */
    A = rd16(LIND(0x00));  /* 009941 LDA */
    mflag = 1;  /* 009943 SEM */
    SETLO(A, A - 1);  /* 009944 DEC */
    wr8(DP(0x06), (uint8_t)A);  /* 009945 STA */
    SETLO(B, rd8(DP(0x05)));  /* 009947 LDA */
    Y = rd16(DP(0x03));  /* 00994A LDY */
    mflag = 0;  /* 00994C CLM */
    S -= 3; F_00989B();  /* 00994D JSRL */
    /* 009951 CMP */
    if ((A & 0xFF) == 0x00) goto L_00996A;  /* 009953 BEQ */
    wr8(DP(0x06), 0x0F);  /* 009955 LDM */
    SETLO(B, 0x00);  /* 009958 LDA */
    Y = 0x2D6C;  /* 00995B LDY */
    mflag = 0;  /* 00995E CLM */
    S -= 3; F_0098E3();  /* 00995F JSRL */
    A = 0x0001;  /* 009963 LDA */
    mflag = 1;  /* 009966 SEM */
    Y = pull16();  /* 009967 PLY */
    D = pull16();  /* 009968 PLD */
    S += 3; return;  /* 009969 RTL */
L_00996A: TICK();
    wr8(DP(0x06), 0x0F);  /* 00996A LDM */
    SETLO(B, 0x00);  /* 00996D LDA */
    Y = 0x2D7C;  /* 009970 LDY */
    mflag = 0;  /* 009973 CLM */
    S -= 3; F_0098E3();  /* 009974 JSRL */
    A = 0x0000;  /* 009978 LDA */
    mflag = 1;  /* 00997B SEM */
    Y = pull16();  /* 00997C PLY */
    D = pull16();  /* 00997D PLD */
    S += 3; return;  /* 00997E RTL */
}

/* 00997F  callers: 0099B3, 009A0D, 009AEC, 019893 */
void F_00997F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00997F LDA */
    push16(D);  /* 009981 PHD */
    D = A;  /* 009982 TAD */
    wr16(DP(0x00), Y);  /* 009983 STY */
    mflag = 1;  /* 009985 SEM */
    wr8(ABS(U0C1), 0x00);  /* 009986 LDM */
    wr8(ABS(U0RIC), 0x00);  /* 00998A LDM */
    wr8(ABS(U0TIC), 0x00);  /* 00998E LDM */
    wr8(ABS(U0MOD), 0x05);  /* 009992 LDM */
    wr8(ABS(U0C0), 0x00);  /* 009996 LDM */
    SETLO(A, Y);  /* 00999A TYA */
    wr8(ABS(U0BRG), (uint8_t)A);  /* 00999B STA */
    wr8(ABS(U0TIC), 0x07);  /* 00999E LDM */
    wr8(ABS(U0RIC), 0x07);  /* 0099A2 LDM */
    wr8(ABS(U0C1), 0x05);  /* 0099A6 LDM */
    mflag = 0;  /* 0099AA CLM */
    D = pull16();  /* 0099AB PLD */
    S += 3; return;  /* 0099AC RTL */
}

/* 0099AD  callers: 004B01, 01A772 */
void F_0099AD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0099AD LDA */
    push16(A);  /* 0099AF PHA */
    Y = 0x0068;  /* 0099B0 LDY */
    S -= 3; F_00997F();  /* 0099B3 JSRL */
    S -= 3; F_0077F0();  /* 0099B7 JSRL */
    Y = pull16();  /* 0099BB PLY */
    S += 3; return;  /* 0099BC RTL */
}

/* 0099BD  callers: 0194C6 */
void F_0099BD(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 0099BD LDA */
    push16(D);  /* 0099BF PHD */
    D = A;  /* 0099C0 TAD */
    C = 0;  /* 0099C1 CLC */
    A = adc16(A, 0x0041);  /* 0099C2 ADC */
    push16(A);  /* 0099C5 PHA */
    I = 1;  /* 0099C6 SEI */
    mflag = 1;  /* 0099C7 SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_0099D7;  /* 0099C8 BBS */
    mflag = 0;  /* 0099CD CLM */
    wr16(ABS(0x0242), rd16(ABS(0x0242)) & ~0x2000);  /* 0099CE CLB */
    S -= 3; F_007529();  /* 0099D3 JSRL */
L_0099D7: TICK();
    mflag = 1;  /* 0099D7 SEM */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) & ~0x08);  /* 0099D8 CLB */
    wr8(ABS(TA3IC), 0x00);  /* 0099DC LDM */
    wr8(ABS(TA3MOD), 0x00);  /* 0099E0 LDM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0) goto L_0099F0;  /* 0099E4 BBC */
    mflag = 0;  /* 0099E9 CLM */
    A = 0x2C4C;  /* 0099EA LDA */
    goto L_0099F4;  /* 0099ED JMP */
L_0099F0: TICK();
    mflag = 0;  /* 0099F0 CLM */
    A = 0x2587;  /* 0099F1 LDA */
L_0099F4: TICK();
    wr16(ABS(TA3), A);  /* 0099F4 STA */
    mflag = 1;  /* 0099F7 SEM */
    wr8(ABS(TA3IC), rd8(ABS(TA3IC)) & ~0x08);  /* 0099F8 CLB */
    wr8(ABS(TCSTART), rd8(ABS(TCSTART)) | 0x08);  /* 0099FC SEB */
L_009A00: TICK();
    wr8(ABS(WDT), 0x00);  /* 009A00 LDM */
    if ((rd8(ABS(TA3IC)) & 0x08) == 0) goto L_009A00;  /* 009A04 BBC */
    Y = 0x0068;  /* 009A09 LDY */
    mflag = 0;  /* 009A0C CLM */
    S -= 3; F_00997F();  /* 009A0D JSRL */
    mflag = 1;  /* 009A11 SEM */
    wr8(ABS(U0RIC), 0x00);  /* 009A12 LDM */
    wr8(ABS(U0TIC), 0x00);  /* 009A16 LDM */
    wr8(DP(0x01), 0x00);  /* 009A1A LDM */
    wr8(DP(0x05), 0x00);  /* 009A1D LDM */
    wr8(DP(0x08), 0x01);  /* 009A20 LDM */
L_009A23: TICK();
    SETLO(A, rd8(DP(0x08)));  /* 009A23 LDA */
    if ((A & 0xFF) == 0) goto L_009A36;  /* 009A25 BEQ */
L_009A27: TICK();
    mflag = 0;  /* 009A27 CLM */
    S -= 3; F_0097EE();  /* 009A28 JSRL */
    /* 009A2C CMP */
    if ((A & 0xFF) == 0x30) goto L_009A33;  /* 009A2E BEQ */
    goto L_009A27;  /* 009A30 JMP */
L_009A33: TICK();
    goto L_009A42;  /* 009A33 JMP */
L_009A36: TICK();
    mflag = 0;  /* 009A36 CLM */
    S -= 3; F_0097FC();  /* 009A37 JSRL */
    /* 009A3B CMP */
    if ((A & 0xFF) == 0x30) goto L_009A42;  /* 009A3D BEQ */
    goto L_009A36;  /* 009A3F JMP */
L_009A42: TICK();
    mflag = 1;  /* 009A42 SEM */
    wr8(DP(0x08), 0x00);  /* 009A43 LDM */
    mflag = 0;  /* 009A46 CLM */
    S -= 3; F_00980E();  /* 009A47 JSRL */
    wr8(DP(0x10), (uint8_t)A);  /* 009A4B STA */
    wr8(DP(0x41), (uint8_t)A);  /* 009A4D STA */
    mflag = 0;  /* 009A4F CLM */
    A = D;  /* 009A50 TDA */
    C = 0;  /* 009A51 CLC */
    A = adc16(A, 0x0011);  /* 009A52 ADC */
    B = 0x0000;  /* 009A55 LDA */
    Y = A;  /* 009A59 TAY */
    S -= 3; F_009855();  /* 009A5A JSRL */
    A = rd16(DP(0x13));  /* 009A5E LDA */
    mflag = 1;  /* 009A60 SEM */
    wr8(DP(0x02), (uint8_t)A);  /* 009A61 STA */
    mflag = 0;  /* 009A63 CLM */
    wr16(DP(0x0E), 0x0000);  /* 009A64 LDM */
    mflag = 1;  /* 009A68 SEM */
    wr8(DP(0x09), 0x14);  /* 009A69 LDM */
    wr8(DP(0x07), 0xFF);  /* 009A6C LDM */
    wr8(DP(0x00), 0x00);  /* 009A6F LDM */
    SETLO(A, rd8(DP(0x10)));  /* 009A72 LDA */
    SETLO(A, A - 1);  /* 009A74 DEC */
    wr8(DP(0x41), (uint8_t)A);  /* 009A75 STA */
    mflag = 0;  /* 009A77 CLM */
    A = D;  /* 009A78 TDA */
    C = 0;  /* 009A79 CLC */
    A = adc16(A, 0x0011);  /* 009A7A ADC */
    B = 0x0000;  /* 009A7D LDA */
    Y = A;  /* 009A81 TAY */
    S -= 3; F_00989B();  /* 009A82 JSRL */
    /* 009A86 CMP */
    if ((A & 0xFF) != 0x00) goto L_009A9C;  /* 009A88 BNE */
    wr8(DP(0x41), 0x0F);  /* 009A8A LDM */
    SETLO(B, 0x00);  /* 009A8D LDA */
    Y = 0x2D7C;  /* 009A90 LDY */
    mflag = 0;  /* 009A93 CLM */
    S -= 3; F_0098E3();  /* 009A94 JSRL */
    mflag = 1;  /* 009A98 SEM */
    wr8(DP(0x10), 0x00);  /* 009A99 LDM */
L_009A9C: TICK();
    SETLO(A, rd8(DP(0x10)));  /* 009A9C LDA */
    if ((A & 0xFF) == 0) goto L_009A23;  /* 009A9E BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 009AA0 LDA */
    if ((A & 0xFF) != 0) goto L_009AB5;  /* 009AA2 BNE */
    wr8(DP(0x41), 0x13);  /* 009AA4 LDM */
    SETLO(B, 0x00);  /* 009AA7 LDA */
    Y = 0x2D8C;  /* 009AAA LDY */
    mflag = 0;  /* 009AAD CLM */
    S -= 3; F_0098E3();  /* 009AAE JSRL */
    goto L_009B22;  /* 009AB2 JMP */
L_009AB5: TICK();
    mflag = 0;  /* 009AB5 CLM */
    A = rd16(DP(0x02));  /* 009AB6 LDA */
    mflag = 1;  /* 009AB8 SEM */
    /* 009AB9 CMP */
    if ((A & 0xFF) != 0x01) goto L_009B10;  /* 009ABB BNE */
    SETLO(A, rd8(DP(0x14)));  /* 009ABD LDA */
    mflag = 0;  /* 009ABF CLM */
    A &= 0x00FF;  /* 009AC0 AND */
    A = rla16(A, 0x0008);  /* 009AC3 RLA */
    A &= 0xFF00;  /* 009AC7 AND */
    wr16(DP(0x41), A);  /* 009ACA STA */
    A = rd16(DP(0x15));  /* 009ACC LDA */
    A &= 0x00FF;  /* 009ACE AND */
    C = 0;  /* 009AD1 CLC */
    A = adc16(A, rd16(DP(0x41)));  /* 009AD2 ADC */
    /* 009AD4 CMP */
    if (A != 0x5161) goto L_009AFC;  /* 009AD7 BNE */
    wr16(DP(0x41), 0x000F);  /* 009AD9 LDM */
    mflag = 1;  /* 009ADD SEM */
    SETLO(B, 0x00);  /* 009ADE LDA */
    Y = 0x2DA0;  /* 009AE1 LDY */
    mflag = 0;  /* 009AE4 CLM */
    S -= 3; F_0098E3();  /* 009AE5 JSRL */
    Y = 0x000B;  /* 009AE9 LDY */
    S -= 3; F_00997F();  /* 009AEC JSRL */
    mflag = 1;  /* 009AF0 SEM */
    wr8(ABS(U0RIC), 0x00);  /* 009AF1 LDM */
    wr8(ABS(U0TIC), 0x00);  /* 009AF5 LDM */
    goto L_009B0D;  /* 009AF9 JMP */
L_009AFC: TICK();
    mflag = 0;  /* 009AFC CLM */
    wr16(DP(0x41), 0x000F);  /* 009AFD LDM */
    mflag = 1;  /* 009B01 SEM */
    SETLO(B, 0x00);  /* 009B02 LDA */
    Y = 0x2D6C;  /* 009B05 LDY */
    mflag = 0;  /* 009B08 CLM */
    S -= 3; F_0098E3();  /* 009B09 JSRL */
L_009B0D: TICK();
    goto L_009B22;  /* 009B0D JMP */
L_009B10: TICK();
    mflag = 1;  /* 009B10 SEM */
    wr8(DP(0x01), 0x01);  /* 009B11 LDM */
    wr8(DP(0x41), 0x0F);  /* 009B14 LDM */
    SETLO(B, 0x00);  /* 009B17 LDA */
    Y = 0x2D6C;  /* 009B1A LDY */
    mflag = 0;  /* 009B1D CLM */
    S -= 3; F_0098E3();  /* 009B1E JSRL */
L_009B22: TICK();
    mflag = 0;  /* 009B22 CLM */
    A = rd16(DP(0x02));  /* 009B23 LDA */
    mflag = 1;  /* 009B25 SEM */
    /* 009B26 CMP */
    if ((A & 0xFF) == 0x32) goto L_009B30;  /* 009B28 BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 009B2A LDA */
    /* 009B2C CMP */
    if ((A & 0xFF) != 0x34) goto L_009B64;  /* 009B2E BNE */
L_009B30: TICK();
    SETLO(A, rd8(DP(0x10)));  /* 009B30 LDA */
    /* 009B32 CMP */
    if ((A & 0xFF) != 0x07) goto L_009B64;  /* 009B34 BNE */
    SETLO(A, rd8(DP(0x14)));  /* 009B36 LDA */
    mflag = 0;  /* 009B38 CLM */
    A &= 0x00FF;  /* 009B39 AND */
    A = rla16(A, 0x0008);  /* 009B3C RLA */
    A &= 0xFF00;  /* 009B40 AND */
    wr16(DP(0x41), A);  /* 009B43 STA */
    A = rd16(DP(0x15));  /* 009B45 LDA */
    A &= 0x00FF;  /* 009B47 AND */
    C = 0;  /* 009B4A CLC */
    A = adc16(A, rd16(DP(0x41)));  /* 009B4B ADC */
    C = 0;  /* 009B4D CLC */
    A = adc16(A, 0x0600);  /* 009B4E ADC */
    wr16(DP(0x0E), A);  /* 009B51 STA */
    A = rd16(DP(0x16));  /* 009B53 LDA */
    mflag = 1;  /* 009B55 SEM */
    SETLO(A, A - 1);  /* 009B56 DEC */
    wr8(DP(0x07), (uint8_t)A);  /* 009B57 STA */
    SETLO(A, rd8(DP(0x16)));  /* 009B59 LDA */
    /* 009B5B CMP */
    if ((A & 0xFF) >= 0x15) goto L_009B61;  /* 009B5D BCS */
    wr8(DP(0x09), (uint8_t)A);  /* 009B5F STA */
L_009B61: TICK();
    wr8(DP(0x00), 0x01);  /* 009B61 LDM */
L_009B64: TICK();
    SETLO(A, rd8(DP(0x01)));  /* 009B64 LDA */
    if ((A & 0xFF) != 0) goto L_009B6B;  /* 009B66 BNE */
    goto L_009A23;  /* 009B68 JMP */
L_009B6B: TICK();
    SETLO(A, rd8(DP(0x02)));  /* 009B6B LDA */
    /* 009B6D CMP */
    if ((A & 0xFF) == 0x31) goto L_009B78;  /* 009B6F BEQ */
    /* 009B71 CMP */
    if ((A & 0xFF) == 0x33) goto L_009B78;  /* 009B73 BEQ */
    goto L_009C15;  /* 009B75 JMP */
L_009B78: TICK();
    wr8(DP(0x03), 0x31);  /* 009B78 LDM */
L_009B7B: TICK();
    SETLO(A, rd8(DP(0x03)));  /* 009B7B LDA */
    /* 009B7D CMP */
    if ((A & 0xFF) == 0x31) goto L_009B84;  /* 009B7F BEQ */
    goto L_009C12;  /* 009B81 JMP */
L_009B84: TICK();
    mflag = 0;  /* 009B84 CLM */
    S -= 3; F_0097FC();  /* 009B85 JSRL */
    wr8(DP(0x03), (uint8_t)A);  /* 009B89 STA */
    mflag = 0;  /* 009B8B CLM */
    A = D;  /* 009B8C TDA */
    C = 0;  /* 009B8D CLC */
    A = adc16(A, 0x0010);  /* 009B8E ADC */
    B = 0x0000;  /* 009B91 LDA */
    wr16(DP(0x41), A);  /* 009B95 STA */
    wr16(DP(0x43), B);  /* 009B97 STA */
    A = D;  /* 009B9A TDA */
    C = 0;  /* 009B9B CLC */
    A = adc16(A, 0x0011);  /* 009B9C ADC */
    Y = A;  /* 009B9F TAY */
    S -= 3; F_00991D();  /* 009BA0 JSRL */
    /* 009BA4 CMP */
    if ((A & 0xFF) == 0x00) goto L_009B84;  /* 009BA6 BEQ */
    SETLO(A, rd8(DP(0x03)));  /* 009BA8 LDA */
    /* 009BAA CMP */
    if ((A & 0xFF) != 0x31) goto L_009C02;  /* 009BAC BNE */
    SETLO(A, rd8(DP(0x11)));  /* 009BAE LDA */
    mflag = 0;  /* 009BB0 CLM */
    A &= 0x00FF;  /* 009BB1 AND */
    A = rla16(A, 0x0008);  /* 009BB4 RLA */
    A &= 0xFF00;  /* 009BB8 AND */
    wr16(DP(0x41), A);  /* 009BBB STA */
    A = rd16(DP(0x12));  /* 009BBD LDA */
    A &= 0x00FF;  /* 009BBF AND */
    C = 0;  /* 009BC2 CLC */
    A = adc16(A, rd16(DP(0x41)));  /* 009BC3 ADC */
    C = 0;  /* 009BC5 CLC */
    A = adc16(A, 0x0600);  /* 009BC6 ADC */
    wr16(DP(0x0A), A);  /* 009BC9 STA */
    A = rd16(DP(0x10));  /* 009BCB LDA */
    C = 0;  /* 009BCD CLC */
    mflag = 1;  /* 009BCE SEM */
    SETLO(A, adc8(A, 0xFD));  /* 009BCF ADC */
    wr8(DP(0x41), (uint8_t)A);  /* 009BD1 STA */
    mflag = 0;  /* 009BD3 CLM */
    A = D;  /* 009BD4 TDA */
    C = 0;  /* 009BD5 CLC */
    A = adc16(A, 0x0011);  /* 009BD6 ADC */
    A += 1;  /* 009BD9 INC */
    A += 1;  /* 009BDA INC */
    B = 0x0000;  /* 009BDB LDA */
    wr16(DP(0x42), A);  /* 009BDF STA */
    wr16(DP(0x44), B);  /* 009BE1 STA */
    X = rd16(DP(0x0A));  /* 009BE4 LDX */
    wr16(DP(0x45), X);  /* 009BE6 STX */
    Y = 0x00A0;  /* 009BE8 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 009BEB JSRL */
    wr16(DP(0x41), 0x000F);  /* 009BEF LDM */
    mflag = 1;  /* 009BF3 SEM */
    SETLO(B, 0x00);  /* 009BF4 LDA */
    Y = 0x2DA0;  /* 009BF7 LDY */
    mflag = 0;  /* 009BFA CLM */
    S -= 3; F_0098E3();  /* 009BFB JSRL */
    goto L_009C0E;  /* 009BFF JMP */
L_009C02: TICK();
    mflag = 0;  /* 009C02 CLM */
    A = rd16(DP(0x03));  /* 009C03 LDA */
    mflag = 1;  /* 009C05 SEM */
    /* 009C06 CMP */
    if ((A & 0xFF) != 0x39) goto L_009C0E;  /* 009C08 BNE */
    wr8(ABS(PMOD), rd8(ABS(PMOD)) | 0x08);  /* 009C0A SEB */
L_009C0E: TICK();
    mflag = 1;  /* 009C0E SEM */
    goto L_009B7B;  /* 009C0F JMP */
L_009C12: TICK();
    goto L_009E41;  /* 009C12 JMP */
L_009C15: TICK();
    mflag = 0;  /* 009C15 CLM */
    A = rd16(DP(0x02));  /* 009C16 LDA */
    mflag = 1;  /* 009C18 SEM */
    C = ((A & 0xFF) >= 0x32);  /* 009C19 CMP */
    Z = ((A & 0xFF) == 0x32);
    if (Z) goto L_009C26;  /* 009C1B BEQ */
    SETLO(A, rd8(DP(0x02)));  /* 009C1D LDA */
    C = ((A & 0xFF) >= 0x34);  /* 009C1F CMP */
    Z = ((A & 0xFF) == 0x34);
    if (Z) goto L_009C26;  /* 009C21 BEQ */
    goto L_009E41;  /* 009C23 JMP */
L_009C26: TICK();
    wr8(DP(0x05), 0x00);  /* 009C26 LDM */
    SETLO(A, rd8(DP(0x00)));  /* 009C29 LDA */
    if ((A & 0xFF) != 0) goto L_009C77;  /* 009C2B BNE */
    SETLO(A, A + 1);  /* 009C2D INC */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 009C2E PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 009C2F STA */
    setP(pull16());  /* 009C31 PLP */
    mflag = 0;  /* 009C32 CLM */
    S -= 3; i2c_stop_00715B();  /* 009C33 JSRL */
    S -= 3; i2c_start_007172();  /* 009C37 JSRL */
    Y = 0x00A4;  /* 009C3B LDY */
    S -= 3; i2c_send_byte_006A68();  /* 009C3E JSRL */
    Y = 0x0000;  /* 009C42 LDY */
    mflag = 0;  /* 009C45 CLM */
    S -= 3; i2c_send_byte_006A68();  /* 009C46 JSRL */
    mflag = 0;  /* 009C4A CLM */
    S -= 3; i2c_start_007172();  /* 009C4B JSRL */
    Y = 0x00A5;  /* 009C4F LDY */
    S -= 3; i2c_send_byte_006A68();  /* 009C52 JSRL */
    Y = 0x0001;  /* 009C56 LDY */
    mflag = 0;  /* 009C59 CLM */
    S -= 3; i2c_recv_byte_006EB3();  /* 009C5A JSRL */
    /* 009C5E CMP */
    if ((A & 0xFF) == 0x00) goto L_009C67;  /* 009C60 BEQ */
    SETLO(A, 0x06);  /* 009C62 LDA */
    goto L_009C69;  /* 009C64 JMP */
L_009C67: TICK();
    SETLO(A, 0x00);  /* 009C67 LDA */
L_009C69: TICK();
    wr8(DP(0x06), (uint8_t)A);  /* 009C69 STA */
    mflag = 0;  /* 009C6B CLM */
    S -= 3; i2c_stop_00715B();  /* 009C6C JSRL */
    mflag = 1;  /* 009C70 SEM */
    SETLO(A, 0x00);  /* 009C71 LDA */
    N = ((A & 0x80) != 0);
    Z = ((A & 0xFF) == 0);
    push16(getP());  /* 009C73 PHP */
    wr8(SR(0x02), (uint8_t)A);  /* 009C74 STA */
    setP(pull16());  /* 009C76 PLP */
L_009C77: TICK();
    mflag = 0;  /* 009C77 CLM */
    A = rd16(DP(0x06));  /* 009C78 LDA */
    mflag = 1;  /* 009C7A SEM */
    /* 009C7B CMP */
    if ((A & 0xFF) >= 0x08) goto L_009C83;  /* 009C7D BCS */
    SETLO(A, rd8(DP(0x00)));  /* 009C7F LDA */
    if ((A & 0xFF) == 0) goto L_009C93;  /* 009C81 BEQ */
L_009C83: TICK();
    SETLO(A, rd8(DP(0x00)));  /* 009C83 LDA */
    if ((A & 0xFF) != 0) goto L_009C8A;  /* 009C85 BNE */
    goto L_009DF3;  /* 009C87 JMP */
L_009C8A: TICK();
    SETLO(A, rd8(DP(0x07)));  /* 009C8A LDA */
    /* 009C8C CMP */
    if ((A & 0xFF) != 0xFF) goto L_009C93;  /* 009C8E BNE */
    goto L_009DF3;  /* 009C90 JMP */
L_009C93: TICK();
    wr8(DP(0x29), 0x00);  /* 009C93 LDM */
    wr8(DP(0x2A), 0x01);  /* 009C96 LDM */
    wr8(DP(0x2B), 0x00);  /* 009C99 LDM */
    mflag = 0;  /* 009C9C CLM */
    A = rd16(DP(0x0F));  /* 009C9D LDA */
    A &= 0x00FF;  /* 009C9F AND */
    mflag = 1;  /* 009CA2 SEM */
    wr8(DP(0x41), (uint8_t)A);  /* 009CA3 STA */
    SETLO(A, rd8(DP(0x09)));  /* 009CA5 LDA */
    mflag = 0;  /* 009CA7 CLM */
    A &= 0x00FF;  /* 009CA8 AND */
    X = A;  /* 009CAB TAX */
    A = rd16(DP(0x41));  /* 009CAC LDA */
    mflag = 1;  /* 009CAE SEM */
    wr8(DP(0x29 + X), (uint8_t)A);  /* 009CAF STA */
    SETLO(A, rd8(DP(0x09)));  /* 009CB1 LDA */
    mflag = 0;  /* 009CB3 CLM */
    A &= 0x00FF;  /* 009CB4 AND */
    A += 1;  /* 009CB7 INC */
    X = A;  /* 009CB8 TAX */
    A = rd16(DP(0x0E));  /* 009CB9 LDA */
    mflag = 1;  /* 009CBB SEM */
    wr8(DP(0x29 + X), (uint8_t)A);  /* 009CBC STA */
    SETLO(A, rd8(DP(0x09)));  /* 009CBE LDA */
    SETLO(A, A + 1);  /* 009CC0 INC */
    SETLO(A, A + 1);  /* 009CC1 INC */
    SETLO(A, A + 1);  /* 009CC2 INC */
    wr8(DP(0x04), (uint8_t)A);  /* 009CC3 STA */
    mflag = 0;  /* 009CC5 CLM */
    wr16(DP(0x0C), 0x0000);  /* 009CC6 LDM */
L_009CCA: TICK();
    A = rd16(DP(0x09));  /* 009CCA LDA */
    A &= 0x00FF;  /* 009CCC AND */
    A += 1;  /* 009CCF INC */
    A += 1;  /* 009CD0 INC */
    t = rd16(DP(0x0C));  /* 009CD1 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_009CE8;  /* 009CD3 BCC */
    if (Z) goto L_009CE8;  /* 009CD5 BEQ */
    X = rd16(DP(0x0C));  /* 009CD7 LDX */
    A = rd16(DP(0x04));  /* 009CD9 LDA */
    C = 0;  /* 009CDB CLC */
    mflag = 1;  /* 009CDC SEM */
    SETLO(A, adc8(A, rd8(DP(0x29 + X))));  /* 009CDD ADC */
    wr8(DP(0x04), (uint8_t)A);  /* 009CDF STA */
    X += 1;  /* 009CE1 INX */
    wr16(DP(0x0C), X);  /* 009CE2 STX */
    mflag = 0;  /* 009CE4 CLM */
    goto L_009CCA;  /* 009CE5 JMP */
L_009CE8: TICK();
    mflag = 0;  /* 009CE8 CLM */
    A = rd16(DP(0x04));  /* 009CE9 LDA */
    mflag = 1;  /* 009CEB SEM */
    SETLO(A, A ^ 0xFF);  /* 009CEC EOR */
    wr8(DP(0x41), (uint8_t)A);  /* 009CEE STA */
    SETLO(A, rd8(DP(0x09)));  /* 009CF0 LDA */
    mflag = 0;  /* 009CF2 CLM */
    A &= 0x00FF;  /* 009CF3 AND */
    A += 1;  /* 009CF6 INC */
    A += 1;  /* 009CF7 INC */
    X = A;  /* 009CF8 TAX */
    A = rd16(DP(0x41));  /* 009CF9 LDA */
    mflag = 1;  /* 009CFB SEM */
    wr8(DP(0x29 + X), (uint8_t)A);  /* 009CFC STA */
    Y = 0x0053;  /* 009CFE LDY */
    mflag = 0;  /* 009D01 CLM */
    S -= 3; F_01058E();  /* 009D02 JSRL */
    Y = 0x0031;  /* 009D06 LDY */
    S -= 3; F_01058E();  /* 009D09 JSRL */
    A = rd16(DP(0x09));  /* 009D0D LDA */
    mflag = 1;  /* 009D0F SEM */
    SETLO(A, A + 1);  /* 009D10 INC */
    SETLO(A, A + 1);  /* 009D11 INC */
    SETLO(A, A + 1);  /* 009D12 INC */
    mflag = 0;  /* 009D13 CLM */
    Y = A;  /* 009D14 TAY */
    S -= 3; F_0097AC();  /* 009D15 JSRL */
    A = rd16(DP(0x09));  /* 009D19 LDA */
    A &= 0x00FF;  /* 009D1B AND */
    X = A;  /* 009D1E TAX */
    Y = rd16(DP(0x29 + X));  /* 009D1F LDY */
    S -= 3; F_0097AC();  /* 009D21 JSRL */
    A = rd16(DP(0x09));  /* 009D25 LDA */
    A &= 0x00FF;  /* 009D27 AND */
    A += 1;  /* 009D2A INC */
    X = A;  /* 009D2B TAX */
    Y = rd16(DP(0x29 + X));  /* 009D2C LDY */
    S -= 3; F_0097AC();  /* 009D2E JSRL */
    wr16(DP(0x0C), 0x0000);  /* 009D32 LDM */
L_009D36: TICK();
    A = rd16(DP(0x09));  /* 009D36 LDA */
    A &= 0x00FF;  /* 009D38 AND */
    t = rd16(DP(0x0C));  /* 009D3B CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_009D4E;  /* 009D3D BCC */
    if (Z) goto L_009D4E;  /* 009D3F BEQ */
    X = rd16(DP(0x0C));  /* 009D41 LDX */
    Y = rd16(DP(0x29 + X));  /* 009D43 LDY */
    S -= 3; F_0097AC();  /* 009D45 JSRL */
    t = (rd16(DP(0x0C)) + 1) & 0xFFFF; wr16(DP(0x0C), t);  /* 009D49 INC */
    goto L_009D36;  /* 009D4B JMP */
L_009D4E: TICK();
    mflag = 0;  /* 009D4E CLM */
    A = rd16(DP(0x09));  /* 009D4F LDA */
    A &= 0x00FF;  /* 009D51 AND */
    A += 1;  /* 009D54 INC */
    A += 1;  /* 009D55 INC */
    X = A;  /* 009D56 TAX */
    Y = rd16(DP(0x29 + X));  /* 009D57 LDY */
    S -= 3; F_0097AC();  /* 009D59 JSRL */
    Y = 0x000D;  /* 009D5D LDY */
    S -= 3; F_01058E();  /* 009D60 JSRL */
    Y = 0x000A;  /* 009D64 LDY */
    S -= 3; F_01058E();  /* 009D67 JSRL */
L_009D6B: TICK();
    S -= 3; F_0097FC();  /* 009D6B JSRL */
    /* 009D6F CMP */
    if ((A & 0xFF) == 0x30) goto L_009D77;  /* 009D71 BEQ */
    mflag = 0;  /* 009D73 CLM */
    goto L_009D6B;  /* 009D74 JMP */
L_009D77: TICK();
    mflag = 0;  /* 009D77 CLM */
    S -= 3; F_00980E();  /* 009D78 JSRL */
    wr8(DP(0x10), (uint8_t)A);  /* 009D7C STA */
    wr8(DP(0x41), (uint8_t)A);  /* 009D7E STA */
    mflag = 0;  /* 009D80 CLM */
    A = D;  /* 009D81 TDA */
    C = 0;  /* 009D82 CLC */
    A = adc16(A, 0x0011);  /* 009D83 ADC */
    B = 0x0000;  /* 009D86 LDA */
    Y = A;  /* 009D8A TAY */
    S -= 3; F_009855();  /* 009D8B JSRL */
    A = rd16(DP(0x13));  /* 009D8F LDA */
    mflag = 1;  /* 009D91 SEM */
    /* 009D92 CMP */
    if ((A & 0xFF) != 0x4E) goto L_009D99;  /* 009D94 BNE */
    goto L_009C93;  /* 009D96 JMP */
L_009D99: TICK();
    SETLO(A, rd8(DP(0x10)));  /* 009D99 LDA */
    SETLO(A, A - 1);  /* 009D9B DEC */
    wr8(DP(0x41), (uint8_t)A);  /* 009D9C STA */
    mflag = 0;  /* 009D9E CLM */
    A = D;  /* 009D9F TDA */
    C = 0;  /* 009DA0 CLC */
    A = adc16(A, 0x0011);  /* 009DA1 ADC */
    B = 0x0000;  /* 009DA4 LDA */
    Y = A;  /* 009DA8 TAY */
    S -= 3; F_00989B();  /* 009DA9 JSRL */
    /* 009DAD CMP */
    if ((A & 0xFF) != 0x00) goto L_009DB4;  /* 009DAF BNE */
    goto L_009C93;  /* 009DB1 JMP */
L_009DB4: TICK();
    t = (rd8(DP(0x05)) + 1) & 0xFF; wr8(DP(0x05), t);  /* 009DB4 INC */
    SETLO(A, rd8(DP(0x09)));  /* 009DB6 LDA */
    mflag = 0;  /* 009DB8 CLM */
    A &= 0x00FF;  /* 009DB9 AND */
    C = 0;  /* 009DBC CLC */
    A = adc16(A, rd16(DP(0x0E)));  /* 009DBD ADC */
    wr16(DP(0x0E), A);  /* 009DBF STA */
    mflag = 1;  /* 009DC1 SEM */
    SETLO(A, rd8(DP(0x00)));  /* 009DC2 LDA */
    if ((A & 0xFF) == 0) goto L_009DD7;  /* 009DC4 BEQ */
    SETLO(A, rd8(DP(0x07)));  /* 009DC6 LDA */
    C = 1;  /* 009DC8 SEC */
    SETLO(A, sbc8(A, rd8(DP(0x09))));  /* 009DC9 SBC */
    wr8(DP(0x07), (uint8_t)A);  /* 009DCB STA */
    t = rd8(DP(0x09));  /* 009DCD CMP */
    if ((A & 0xFF) >= t) goto L_009DD4;  /* 009DCF BCS */
    SETLO(A, A + 1);  /* 009DD1 INC */
    wr8(DP(0x09), (uint8_t)A);  /* 009DD2 STA */
L_009DD4: TICK();
    goto L_009DF0;  /* 009DD4 JMP */
L_009DD7: TICK();
    mflag = 0;  /* 009DD7 CLM */
    A = rd16(DP(0x05));  /* 009DD8 LDA */
    mflag = 1;  /* 009DDA SEM */
    /* 009DDB CMP */
    if ((A & 0xFF) != 0x0C) goto L_009DE2;  /* 009DDD BNE */
    wr8(DP(0x09), 0x10);  /* 009DDF LDM */
L_009DE2: TICK();
    SETLO(A, rd8(DP(0x05)));  /* 009DE2 LDA */
    /* 009DE4 CMP */
    if ((A & 0xFF) < 0x0D) goto L_009DF0;  /* 009DE6 BCC */
    t = (rd8(DP(0x06)) + 1) & 0xFF; wr8(DP(0x06), t);  /* 009DE8 INC */
    wr8(DP(0x05), 0x00);  /* 009DEA LDM */
    wr8(DP(0x09), 0x14);  /* 009DED LDM */
L_009DF0: TICK();
    goto L_009C77;  /* 009DF0 JMP */
L_009DF3: TICK();
    mflag = 0;  /* 009DF3 CLM */
    wr16(DP(0x41), 0x0009);  /* 009DF4 LDM */
    mflag = 1;  /* 009DF8 SEM */
    SETLO(B, 0x00);  /* 009DF9 LDA */
    Y = 0x2DC0;  /* 009DFC LDY */
    mflag = 0;  /* 009DFF CLM */
    S -= 3; F_0098E3();  /* 009E00 JSRL */
L_009E04: TICK();
    S -= 3; F_0097FC();  /* 009E04 JSRL */
    /* 009E08 CMP */
    if ((A & 0xFF) == 0x30) goto L_009E10;  /* 009E0A BEQ */
    mflag = 0;  /* 009E0C CLM */
    goto L_009E04;  /* 009E0D JMP */
L_009E10: TICK();
    mflag = 0;  /* 009E10 CLM */
    S -= 3; F_00980E();  /* 009E11 JSRL */
    wr8(DP(0x10), (uint8_t)A);  /* 009E15 STA */
    wr8(DP(0x41), (uint8_t)A);  /* 009E17 STA */
    mflag = 0;  /* 009E19 CLM */
    A = D;  /* 009E1A TDA */
    C = 0;  /* 009E1B CLC */
    A = adc16(A, 0x0011);  /* 009E1C ADC */
    B = 0x0000;  /* 009E1F LDA */
    Y = A;  /* 009E23 TAY */
    S -= 3; F_009855();  /* 009E24 JSRL */
    A = rd16(DP(0x10));  /* 009E28 LDA */
    mflag = 1;  /* 009E2A SEM */
    SETLO(A, A - 1);  /* 009E2B DEC */
    wr8(DP(0x41), (uint8_t)A);  /* 009E2C STA */
    mflag = 0;  /* 009E2E CLM */
    A = D;  /* 009E2F TDA */
    C = 0;  /* 009E30 CLC */
    A = adc16(A, 0x0011);  /* 009E31 ADC */
    B = 0x0000;  /* 009E34 LDA */
    Y = A;  /* 009E38 TAY */
    S -= 3; F_00989B();  /* 009E39 JSRL */
    /* 009E3D CMP */
    if ((A & 0xFF) == 0x00) goto L_009DF3;  /* 009E3F BEQ */
L_009E41: TICK();
    mflag = 1;  /* 009E41 SEM */
    goto L_009A23;  /* 009E42 JMP */
}

/* 009E45  callers: 009FFF, 00A459 */
void F_009E45(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 009E45 SEM */
    wr8(ABS(0x03A0), 0x00);  /* 009E46 LDM */
    wr8(ABS(0x03A1), 0x00);  /* 009E4A LDM */
    mflag = 0;  /* 009E4E CLM */
    S += 3; return;  /* 009E4F RTL */
}

/* 009E50  callers: 01C9B6 */
void F_009E50(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 009E50 LDA */
    push16(D);  /* 009E52 PHD */
    D = A;  /* 009E53 TAD */
    C = 0;  /* 009E54 CLC */
    A = adc16(A, 0x0006);  /* 009E55 ADC */
    push16(A);  /* 009E58 PHA */
    X = rd16(DP(0x00));  /* 009E59 LDX */
    wr16(ABS(0x0443), X);  /* 009E5B STX */
    if ((rd16(ABS(0x0443)) & 0x0004) == 0) goto L_009EBC;  /* 009E5E BBC */
    X = rd16(ABS(0x0445));  /* 009E64 LDX */
    if (X == 0) goto L_009E7E;  /* 009E67 BEQ */
    t = (rd16(ABS(0x0445)) - 1) & 0xFFFF; wr16(ABS(0x0445), t);  /* 009E69 DEC */
    wr16(DP(0x02), 0x00AA);  /* 009E6C LDM */
    wr16(DP(0x04), 0x0043);  /* 009E70 LDM */
    Y = 0x0002;  /* 009E74 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009E77 JSRL */
    goto L_009EBC;  /* 009E7B JMP */
L_009E7E: TICK();
    mflag = 0;  /* 009E7E CLM */
    A = rd16(ABS(0x0409));  /* 009E7F LDA */
    mflag = 1;  /* 009E82 SEM */
    /* 009E83 CMP */
    if ((A & 0xFF) >= 0x08) goto L_009EA5;  /* 009E85 BCS */
    SETLO(A, rd8(ABS(0x0409)));  /* 009E87 LDA */
    t = (rd8(ABS(0x0409)) + 1) & 0xFF; wr8(ABS(0x0409), t);  /* 009E8A INC */
    mflag = 0;  /* 009E8D CLM */
    A &= 0x00FF;  /* 009E8E AND */
    X = A;  /* 009E91 TAX */
    A = rd16((ABS(0x03C7) + X));  /* 009E92 LDA */
    A &= 0x00FF;  /* 009E95 AND */
    wr16(DP(0x02), A);  /* 009E98 STA */
    wr16(DP(0x04), 0x0043);  /* 009E9A LDM */
    Y = 0x0002;  /* 009E9E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009EA1 JSRL */
L_009EA5: TICK();
    SETLO(A, rd8(ABS(0x0409)));  /* 009EA5 LDA */
    /* 009EA8 CMP */
    if ((A & 0xFF) != 0x03) goto L_009EBC;  /* 009EAA BNE */
    mflag = 0;  /* 009EAC CLM */
    wr16(DP(0x02), 0x000B);  /* 009EAD LDM */
    wr16(DP(0x04), 0x0040);  /* 009EB1 LDM */
    Y = 0x0002;  /* 009EB5 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009EB8 JSRL */
L_009EBC: TICK();
    mflag = 0;  /* 009EBC CLM */
    if ((rd16(ABS(0x0443)) & 0x0001) == 0x0001) goto L_009EC6;  /* 009EBD BBS */
    goto L_009FD8;  /* 009EC3 JMP */
L_009EC6: TICK();
    A = rd16(ABS(0x040A));  /* 009EC6 LDA */
    mflag = 1;  /* 009EC9 SEM */
    /* 009ECA CMP */
    if ((A & 0xFF) >= 0x06) goto L_009F04;  /* 009ECC BCS */
    SETLO(A, rd8(ABS(0x0433)));  /* 009ECE LDA */
    if ((A & 0xFF) == 0) goto L_009EDD;  /* 009ED1 BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 009ED3 LDA */
    /* 009ED6 CMP */
    if ((A & 0xFF) >= 0x09) goto L_009EDD;  /* 009ED8 BCS */
    goto L_009F01;  /* 009EDA JMP */
L_009EDD: TICK();
    mflag = 0;  /* 009EDD CLM */
    wr16(DP(0x02), 0x0000);  /* 009EDE LDM */
    wr16(DP(0x04), 0x0042);  /* 009EE2 LDM */
    Y = 0x0002;  /* 009EE6 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009EE9 JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 009EED STA */
    SETLO(A, rd8(ABS(0x040A)));  /* 009EEF LDA */
    mflag = 0;  /* 009EF2 CLM */
    A &= 0x00FF;  /* 009EF3 AND */
    C = 0;  /* 009EF6 CLC */
    A = adc16(A, 0x03C9);  /* 009EF7 ADC */
    X = A;  /* 009EFA TAX */
    A = rd16(DP(0x02));  /* 009EFB LDA */
    mflag = 1;  /* 009EFD SEM */
    wr8((ABS(0x0000) + X), (uint8_t)A);  /* 009EFE STA */
L_009F01: TICK();
    goto L_009F38;  /* 009F01 JMP */
L_009F04: TICK();
    mflag = 0;  /* 009F04 CLM */
    A = rd16(ABS(0x040A));  /* 009F05 LDA */
    mflag = 1;  /* 009F08 SEM */
    SETLO(A, A & 0x07);  /* 009F09 AND */
    /* 009F0B CMP */
    if ((A & 0xFF) < 0x06) goto L_009F12;  /* 009F0D BCC */
    goto L_009F38;  /* 009F0F JMP */
L_009F12: TICK();
    mflag = 0;  /* 009F12 CLM */
    wr16(DP(0x02), 0x0000);  /* 009F13 LDM */
    wr16(DP(0x04), 0x0042);  /* 009F17 LDM */
    Y = 0x0002;  /* 009F1B LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009F1E JSRL */
    wr8(DP(0x02), (uint8_t)A);  /* 009F22 STA */
    SETLO(A, rd8(ABS(0x040A)));  /* 009F24 LDA */
    mflag = 0;  /* 009F27 CLM */
    A &= 0x00FF;  /* 009F28 AND */
    C = 0;  /* 009F2B CLC */
    A = adc16(A, 0x03C9);  /* 009F2C ADC */
    A -= 1;  /* 009F2F DEC */
    A -= 1;  /* 009F30 DEC */
    X = A;  /* 009F31 TAX */
    A = rd16(DP(0x02));  /* 009F32 LDA */
    mflag = 1;  /* 009F34 SEM */
    wr8((ABS(0x0000) + X), (uint8_t)A);  /* 009F35 STA */
L_009F38: TICK();
    t = (rd8(ABS(0x040A)) + 1) & 0xFF; wr8(ABS(0x040A), t);  /* 009F38 INC */
    SETLO(A, rd8(ABS(0x040A)));  /* 009F3B LDA */
    /* 009F3E CMP */
    if ((A & 0xFF) != 0x08) goto L_009F88;  /* 009F40 BNE */
    wr8(ABS(0x03A9), 0x01);  /* 009F42 LDM */
    wr8(ABS(0x03AA), 0x01);  /* 009F46 LDM */
    SETLO(A, rd8(ABS(0x0438)));  /* 009F4A LDA */
    if ((A & 0xFF) == 0) goto L_009F52;  /* 009F4D BEQ */
    t = (rd8(ABS(0x0438)) - 1) & 0xFF; wr8(ABS(0x0438), t);  /* 009F4F DEC */
L_009F52: TICK();
    SETLO(A, rd8(ABS(0x0435)));  /* 009F52 LDA */
    if ((A & 0xFF) == 0) goto L_009F62;  /* 009F55 BEQ */
    t = (rd8(ABS(0x0435)) - 1) & 0xFF; wr8(ABS(0x0435), t);  /* 009F57 DEC */
    SETLO(A, rd8(ABS(0x0435)));  /* 009F5A LDA */
    if ((A & 0xFF) != 0) goto L_009F62;  /* 009F5D BNE */
    wr8(ABS(0x0437), (uint8_t)A);  /* 009F5F STA */
L_009F62: TICK();
    SETLO(A, rd8(ABS(0x0433)));  /* 009F62 LDA */
    if ((A & 0xFF) == 0) goto L_009F6C;  /* 009F65 BEQ */
    SETLO(A, A - 1);  /* 009F67 DEC */
    SETLO(A, A - 1);  /* 009F68 DEC */
    wr8(ABS(0x0433), (uint8_t)A);  /* 009F69 STA */
L_009F6C: TICK();
    SETLO(A, rd8(ABS(0x03A8)));  /* 009F6C LDA */
    if ((A & 0xFF) == 0) goto L_009F85;  /* 009F6F BEQ */
    wr8(ABS(0x040A), 0x00);  /* 009F71 LDM */
    mflag = 0;  /* 009F75 CLM */
    wr16(DP(0x02), 0x003A);  /* 009F76 LDM */
    wr16(DP(0x04), 0x0040);  /* 009F7A LDM */
    Y = 0x0002;  /* 009F7E LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009F81 JSRL */
L_009F85: TICK();
    goto L_009FD8;  /* 009F85 JMP */
L_009F88: TICK();
    mflag = 1;  /* 009F88 SEM */
    wr8(ABS(0x03A9), 0x00);  /* 009F89 LDM */
    SETLO(A, rd8(ABS(0x040A)));  /* 009F8D LDA */
    /* 009F90 CMP */
    if ((A & 0xFF) != 0x0C) goto L_009FB7;  /* 009F92 BNE */
    SETLO(A, rd8(ABS(0x0433)));  /* 009F94 LDA */
    if ((A & 0xFF) != 0) goto L_009FB7;  /* 009F97 BNE */
    wr8(ABS(0x040A), (uint8_t)A);  /* 009F99 STA */
    mflag = 0;  /* 009F9C CLM */
    wr16(DP(0x02), 0x003A);  /* 009F9D LDM */
    wr16(DP(0x04), 0x0040);  /* 009FA1 LDM */
    Y = 0x0002;  /* 009FA5 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009FA8 JSRL */
    wr8(ABS(0x03A0), 0x02);  /* 009FAC LDM */
    wr8(ABS(0x03A1), 0x01);  /* 009FB0 LDM */
    goto L_009FD8;  /* 009FB4 JMP */
L_009FB7: TICK();
    mflag = 0;  /* 009FB7 CLM */
    A = rd16(ABS(0x040A));  /* 009FB8 LDA */
    mflag = 1;  /* 009FBB SEM */
    /* 009FBC CMP */
    if ((A & 0xFF) != 0x10) goto L_009FD8;  /* 009FBE BNE */
    wr8(ABS(0x03AA), 0x01);  /* 009FC0 LDM */
    wr8(ABS(0x040A), 0x00);  /* 009FC4 LDM */
    mflag = 0;  /* 009FC8 CLM */
    wr16(DP(0x02), 0x003A);  /* 009FC9 LDM */
    wr16(DP(0x04), 0x0040);  /* 009FCD LDM */
    Y = 0x0002;  /* 009FD1 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009FD4 JSRL */
L_009FD8: TICK();
    mflag = 0;  /* 009FD8 CLM */
    if ((rd16(ABS(0x0443)) & 0x0008) == 0) goto L_009FEE;  /* 009FD9 BBC */
    wr16(DP(0x02), 0x003A);  /* 009FDF LDM */
    wr16(DP(0x04), 0x0040);  /* 009FE3 LDM */
    Y = 0x0002;  /* 009FE7 LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 009FEA JSRL */
L_009FEE: TICK();
    mflag = 0;  /* 009FEE CLM */
    if ((rd16(ABS(0x0443)) & 0x0010) == 0) goto L_00A008;  /* 009FEF BBC */
    mflag = 1;  /* 009FF5 SEM */
    wr8(ABS(0x03AA), 0x01);  /* 009FF6 LDM */
    wr8(ABS(0x03A9), 0x00);  /* 009FFA LDM */
    mflag = 0;  /* 009FFE CLM */
    S -= 3; F_009E45();  /* 009FFF JSRL */
    mflag = 1;  /* 00A003 SEM */
    wr8(ABS(0x040A), 0x00);  /* 00A004 LDM */
L_00A008: TICK();
    mflag = 0;  /* 00A008 CLM */
    if ((rd16(ABS(0x0443)) & 0x0020) == 0) goto L_00A018;  /* 00A009 BBC */
    mflag = 1;  /* 00A00F SEM */
    wr8(ABS(0x03A9), 0x00);  /* 00A010 LDM */
    wr8(ABS(0x040A), 0x00);  /* 00A014 LDM */
L_00A018: TICK();
    mflag = 0;  /* 00A018 CLM */
    Y = pull16();  /* 00A019 PLY */
    D = pull16();  /* 00A01A PLD */
    S += 3; return;  /* 00A01B RTL */
}

/* 00A01C  callers: 00A392, 00A504, 00B4EB, 00B583, 00BF0A, 00C86A */
void F_00A01C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A01C LDA */
    push16(D);  /* 00A01E PHD */
    D = A;  /* 00A01F TAD */
    C = 0;  /* 00A020 CLC */
    A = adc16(A, 0x0008);  /* 00A021 ADC */
    push16(A);  /* 00A024 PHA */
    Y = 0x007D;  /* 00A025 LDY */
    S -= 3; F_008854();  /* 00A028 JSRL */
    wr16(ABS(0x0373), A);  /* 00A02C STA */
    wr16(ABS(0x037A), A);  /* 00A02F STA */
    A = rd16(ABS(0x01F9));  /* 00A032 LDA */
    B = 0x0005;  /* 00A035 LDA */
    S -= 3; F_00FF8C();  /* 00A039 JSRL */
    B = 0x0000;  /* 00A03D LDA */
    /* 00A041 CMP */
    if (A < 0x8000) goto L_00A048;  /* 00A044 BCC */
    B -= 1;  /* 00A046 DEC */
L_00A048: TICK();
    wr16(DP(0x08), A);  /* 00A048 STA */
    A = rd16(ABS(0x0224));  /* 00A04A LDA */
    A = asl16(A);  /* 00A04D ASL */
    C = 0;  /* 00A04E CLC */
    A = adc16(A, rd16(ABS(0x03F5)));  /* 00A04F ADC */
    A -= 1;  /* 00A052 DEC */
    wr16(DP(0x04), A);  /* 00A053 STA */
    wr16(DP(0x06), 0x0000);  /* 00A055 LDM */
    A = rd16(DP(0x08));  /* 00A059 LDA */
    C = 0;  /* 00A05B CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 00A05C ADC */
    B = adc16(B, rd16(DP(0x06)));  /* 00A05E ADC */
    C = 0;  /* 00A061 CLC */
    A = adc16(A, rd16(ABS(0x01DB)));  /* 00A062 ADC */
    B = adc16(B, 0x0000);  /* 00A065 ADC */
    wr16(DP(0x00), A);  /* 00A069 STA */
    wr16(DP(0x02), B);  /* 00A06B STA */
    Y = A;  /* 00A06E TAY */
    S -= 3; F_0087DE();  /* 00A06F JSRL */
    wr16(ABS(0x0370), A);  /* 00A073 STA */
    B = rd16(DP(0x02));  /* 00A076 LDA */
    Y = rd16(DP(0x00));  /* 00A079 LDY */
    S -= 3; F_008825();  /* 00A07B JSRL */
    wr8(ABS(0x0372), (uint8_t)A);  /* 00A07F STA */
    wr8(ABS(0x0375), 0x00);  /* 00A082 LDM */
    wr8(ABS(0x037C), 0x80);  /* 00A086 LDM */
    mflag = 0;  /* 00A08A CLM */
    A = rd16(DP(0x04));  /* 00A08B LDA */
    B = rd16(DP(0x06));  /* 00A08D LDA */
    C = 0;  /* 00A090 CLC */
    A = adc16(A, rd16(ABS(0x01D9)));  /* 00A091 ADC */
    B = adc16(B, 0x0000);  /* 00A094 ADC */
    wr16(DP(0x04), A);  /* 00A098 STA */
    wr16(DP(0x06), B);  /* 00A09A STA */
    Y = A;  /* 00A09D TAY */
    S -= 3; F_0087DE();  /* 00A09E JSRL */
    wr16(ABS(0x0377), A);  /* 00A0A2 STA */
    B = rd16(DP(0x06));  /* 00A0A5 LDA */
    Y = rd16(DP(0x04));  /* 00A0A8 LDY */
    S -= 3; F_008825();  /* 00A0AA JSRL */
    wr8(ABS(0x0379), (uint8_t)A);  /* 00A0AE STA */
    mflag = 0;  /* 00A0B1 CLM */
    A = rd16(ABS(0x0224));  /* 00A0B2 LDA */
    A = lsr16(A);  /* 00A0B5 LSR */
    A = lsr16(A);  /* 00A0B6 LSR */
    wr16(DP(0x0A), 0x0000);  /* 00A0B7 LDM */
    wr16(DP(0x08), 0x0008);  /* 00A0BB LDM */
    wr16(DP(0x0C), A);  /* 00A0BF STA */
    A = rd16(DP(0x04));  /* 00A0C1 LDA */
    B = rd16(DP(0x06));  /* 00A0C3 LDA */
    X = 0x0008;  /* 00A0C6 LDX */
    S -= 3; rt_ldiv_01E1A6();  /* 00A0C9 JSRL */
    C = 1;  /* 00A0CD SEC */
    A = sbc16(A, rd16(DP(0x0C)));  /* 00A0CE SBC */
    wr16(ABS(0x0220), A);  /* 00A0D0 STA */
    wr16(ABS(0x023E), rd16(ABS(0x023E)) & ~0x0004);  /* 00A0D3 CLB */
    wr16(DP(0x08), 0x0370);  /* 00A0D8 LDM */
    wr16(DP(0x0A), 0x0363);  /* 00A0DC LDM */
    A = 0x0008;  /* 00A0E0 LDA */
    Y = 0x000D;  /* 00A0E3 LDY */
    S -= 3; rt_memcpy_01E466();  /* 00A0E6 JSRL */
    Y = pull16();  /* 00A0EA PLY */
    D = pull16();  /* 00A0EB PLD */
    S += 3; return;  /* 00A0EC RTL */
}

/* 00A0ED  callers: 00ADA5, 00C572, 00C72C */
void F_00A0ED(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A0ED LDA */
    push16(D);  /* 00A0EF PHD */
    D = A;  /* 00A0F0 TAD */
    X = 0x03C9;  /* 00A0F1 LDX */
    A = rd16((ABS(0x0007) + X));  /* 00A0F4 LDA */
    A &= 0x00FF;  /* 00A0F7 AND */
    wr16(DP(0x00), A);  /* 00A0FA STA */
    A = rd16((ABS(0x0006) + X));  /* 00A0FC LDA */
    A &= 0x00FF;  /* 00A0FF AND */
    A = rla16(A, 0x0008);  /* 00A102 RLA */
    A &= 0xFF00;  /* 00A106 AND */
    C = 0;  /* 00A109 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00A10A ADC */
    D = pull16();  /* 00A10C PLD */
    S += 3; return;  /* 00A10D RTL */
}

/* 00A10E  callers: 00B790, 00C22F, 00C331, 00C3C1, 00C3DF, 00C440, 00C46B, 00C49D ... */
void F_00A10E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A10E LDA */
    push16(D);  /* 00A110 PHD */
    D = A;  /* 00A111 TAD */
    wr16(DP(0x00), Y);  /* 00A112 STY */
    A = rd16(ABS(0x03FE));  /* 00A114 LDA */
    A &= 0x00FF;  /* 00A117 AND */
    C = (A >= 0x0040);  /* 00A11A CMP */
    if (C) goto L_00A13B;  /* 00A11D BCS */
    A = Y;  /* 00A11F TYA */
    mflag = 1;  /* 00A120 SEM */
    SETLO(A, A ^ 0xFF);  /* 00A121 EOR */
    SETLO(A, A + 1);  /* 00A123 INC */
    SETLO(A, adc8(A, 0x04));  /* 00A124 ADC */
    mflag = 0;  /* 00A126 CLM */
    A &= 0x00FF;  /* 00A127 AND */
    X = A;  /* 00A12A TAX */
    A = rd16(ABS(0x03FD));  /* 00A12B LDA */
    A &= 0x0078;  /* 00A12E AND */
    A = lsr16(A);  /* 00A131 LSR */
    A = lsr16(A);  /* 00A132 LSR */
    A = lsr16(A);  /* 00A133 LSR */
    S -= 3; rt_lshr_01E49E();  /* 00A134 JSRL */
    goto L_00A155;  /* 00A138 JMP */
L_00A13B: TICK();
    A = Y;  /* 00A13B TYA */
    mflag = 1;  /* 00A13C SEM */
    SETLO(A, A ^ 0xFF);  /* 00A13D EOR */
    SETLO(A, A + 1);  /* 00A13F INC */
    C = 0;  /* 00A140 CLC */
    SETLO(A, adc8(A, 0x09));  /* 00A141 ADC */
    mflag = 0;  /* 00A143 CLM */
    A &= 0x00FF;  /* 00A144 AND */
    X = A;  /* 00A147 TAX */
    A = rd16(ABS(0x03FD));  /* 00A148 LDA */
    A &= 0x0FF8;  /* 00A14B AND */
    A = lsr16(A);  /* 00A14E LSR */
    A = lsr16(A);  /* 00A14F LSR */
    A = lsr16(A);  /* 00A150 LSR */
    S -= 3; rt_lshr_01E49E();  /* 00A151 JSRL */
L_00A155: TICK();
    D = pull16();  /* 00A155 PLD */
    S += 3; return;  /* 00A156 RTL */
}

/* 00A157  callers: 00C3E4, 00C5A4, 00C740 */
void F_00A157(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A157 LDA */
    push16(D);  /* 00A159 PHD */
    D = A;  /* 00A15A TAD */
    wr16(DP(0x00), Y);  /* 00A15B STY */
    X = Y;  /* 00A15D TYX */
    if (X == 0) goto L_00A193;  /* 00A15E BEQ */
    t = rd16(ABS(0x03FF));  /* 00A160 CPY */
    if (Y == t) goto L_00A193;  /* 00A163 BEQ */
    X = rd16(ABS(0x0405));  /* 00A165 LDX */
    if (X == 0) goto L_00A177;  /* 00A168 BEQ */
    t = rd16(ABS(0x0401));  /* 00A16A CPY */
    if (Y != t) goto L_00A177;  /* 00A16D BNE */
    A = rd16(ABS(0x0428));  /* 00A16F LDA */
    mflag = 1;  /* 00A172 SEM */
    /* 00A173 CMP */
    if ((A & 0xFF) >= 0x02) goto L_00A193;  /* 00A175 BCS */
L_00A177: TICK();
    mflag = 0;  /* 00A177 CLM */
    X = rd16(ABS(0x0407));  /* 00A178 LDX */
    if (X == 0) goto L_00A18C;  /* 00A17B BEQ */
    X = rd16(ABS(0x0403));  /* 00A17D LDX */
    t = rd16(DP(0x00));  /* 00A180 CPX */
    if (X != t) goto L_00A18C;  /* 00A182 BNE */
    A = rd16(ABS(0x0428));  /* 00A184 LDA */
    mflag = 1;  /* 00A187 SEM */
    /* 00A188 CMP */
    if ((A & 0xFF) == 0x03) goto L_00A193;  /* 00A18A BEQ */
L_00A18C: TICK();
    mflag = 0;  /* 00A18C CLM */
    A = 0x0000;  /* 00A18D LDA */
    goto L_00A197;  /* 00A190 JMP */
L_00A193: TICK();
    mflag = 0;  /* 00A193 CLM */
    A = 0x0001;  /* 00A194 LDA */
L_00A197: TICK();
    mflag = 1;  /* 00A197 SEM */
    D = pull16();  /* 00A198 PLD */
    S += 3; return;  /* 00A199 RTL */
}

/* 00A19A  callers: 00C234, 00C336 */
void F_00A19A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A19A LDA */
    push16(D);  /* 00A19C PHD */
    D = A;  /* 00A19D TAD */
    A += 1;  /* 00A19E INC */
    A += 1;  /* 00A19F INC */
    push16(A);  /* 00A1A0 PHA */
    wr16(DP(0x00), Y);  /* 00A1A1 STY */
    t = rd16(ABS(0x03FF));  /* 00A1A3 CPY */
    if (Y == t) goto L_00A1EB;  /* 00A1A6 BEQ */
    X = rd16(ABS(0x0401));  /* 00A1A8 LDX */
    t = rd16(ABS(0x03FF));  /* 00A1AB CPX */
    if (X == t) goto L_00A1CB;  /* 00A1AE BEQ */
    X = rd16(ABS(0x0405));  /* 00A1B0 LDX */
    wr16(ABS(0x0407), X);  /* 00A1B3 STX */
    A = rd16(ABS(0x0418));  /* 00A1B6 LDA */
    A &= 0x00FF;  /* 00A1B9 AND */
    wr16(ABS(0x0405), A);  /* 00A1BC STA */
    X = rd16(ABS(0x0401));  /* 00A1BF LDX */
    wr16(ABS(0x0403), X);  /* 00A1C2 STX */
    X = rd16(ABS(0x03FF));  /* 00A1C5 LDX */
    wr16(ABS(0x0401), X);  /* 00A1C8 STX */
L_00A1CB: TICK();
    wr16(ABS(0x03FF), Y);  /* 00A1CB STY */
    if ((rd16(ABS(0x039E)) & 0x2000) == 0) goto L_00A1EB;  /* 00A1CE BBC */
    wr16(DP(0x02), 0x0002);  /* 00A1D4 LDM */
    wr16(DP(0x03), 0x03FF);  /* 00A1D8 LDM */
    wr16(DP(0x05), 0x0000);  /* 00A1DC LDM */
    wr16(DP(0x06), 0x055A);  /* 00A1E0 LDM */
    Y = 0x00A0;  /* 00A1E4 LDY */
    S -= 3; i2c_block_xfer_b_0073C6();  /* 00A1E7 JSRL */
L_00A1EB: TICK();
    Y = pull16();  /* 00A1EB PLY */
    D = pull16();  /* 00A1EC PLD */
    S += 3; return;  /* 00A1ED RTL */
}

/* 00A1EE  callers: 00C3C6, 00C5EF */
void F_00A1EE(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A1EE LDA */
    push16(D);  /* 00A1F0 PHD */
    D = A;  /* 00A1F1 TAD */
    wr16(DP(0x00), Y);  /* 00A1F2 STY */
    X = Y;  /* 00A1F4 TYX */
    if (X == 0) goto L_00A23D;  /* 00A1F5 BEQ */
    wr16(DP(0x02), 0x0000);  /* 00A1F7 LDM */
L_00A1FB: TICK();
    X = rd16(DP(0x02));  /* 00A1FB LDX */
    /* 00A1FD CPX */
    if (X >= 0x0008) goto L_00A23D;  /* 00A200 BCS */
    A = rd16((ABS(0x0429) + X));  /* 00A202 LDA */
    mflag = 1;  /* 00A205 SEM */
    t = rd8(DP(0x00));  /* 00A206 CMP */
    if ((A & 0xFF) != t) goto L_00A237;  /* 00A208 BNE */
    mflag = 0;  /* 00A20A CLM */
    A = rd16(DP(0x01));  /* 00A20B LDA */
    A &= 0x00FF;  /* 00A20D AND */
    wr16(DP(0x04), A);  /* 00A210 STA */
    A = X;  /* 00A212 TXA */
    mflag = 1;  /* 00A213 SEM */
    SETLO(A, A ^ 0xFF);  /* 00A214 EOR */
    SETLO(A, A + 1);  /* 00A216 INC */
    C = 0;  /* 00A217 CLC */
    SETLO(A, adc8(A, 0x07));  /* 00A218 ADC */
    mflag = 0;  /* 00A21A CLM */
    A &= 0x00FF;  /* 00A21B AND */
    X = A;  /* 00A21E TAX */
    A = rd16(ABS(0x0431));  /* 00A21F LDA */
    mflag = 1;  /* 00A222 SEM */
    S -= 3; F_01E4B7();  /* 00A223 JSRL */
    SETLO(A, A & 0x01);  /* 00A227 AND */
    mflag = 0;  /* 00A229 CLM */
    A &= 0x00FF;  /* 00A22A AND */
    t = rd16(DP(0x04));  /* 00A22D CMP */
    if (A != t) goto L_00A237;  /* 00A22F BNE */
    A = 0x0001;  /* 00A231 LDA */
    mflag = 1;  /* 00A234 SEM */
    D = pull16();  /* 00A235 PLD */
    S += 3; return;  /* 00A236 RTL */
L_00A237: TICK();
    mflag = 0;  /* 00A237 CLM */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 00A238 INC */
    goto L_00A1FB;  /* 00A23A JMP */
L_00A23D: TICK();
    mflag = 0;  /* 00A23D CLM */
    A = 0x0000;  /* 00A23E LDA */
    mflag = 1;  /* 00A241 SEM */
    D = pull16();  /* 00A242 PLD */
    S += 3; return;  /* 00A243 RTL */
}

/* 00A244  callers: 00B795 */
void F_00A244(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A244 LDA */
    push16(D);  /* 00A246 PHD */
    D = A;  /* 00A247 TAD */
    wr16(DP(0x00), Y);  /* 00A248 STY */
    wr16(DP(0x02), 0x0000);  /* 00A24A LDM */
L_00A24E: TICK();
    X = rd16(DP(0x02));  /* 00A24E LDX */
    /* 00A250 CPX */
    if (X >= 0x0007) goto L_00A263;  /* 00A253 BCS */
    A = rd16((ABS(0x042A) + X));  /* 00A255 LDA */
    mflag = 1;  /* 00A258 SEM */
    wr8((ABS(0x0429) + X), (uint8_t)A);  /* 00A259 STA */
    X += 1;  /* 00A25C INX */
    wr16(DP(0x02), X);  /* 00A25D STX */
    mflag = 0;  /* 00A25F CLM */
    goto L_00A24E;  /* 00A260 JMP */
L_00A263: TICK();
    mflag = 0;  /* 00A263 CLM */
    A = rd16(DP(0x00));  /* 00A264 LDA */
    mflag = 1;  /* 00A266 SEM */
    wr8(ABS(0x0430), (uint8_t)A);  /* 00A267 STA */
    mflag = 0;  /* 00A26A CLM */
    A = rd16(DP(0x01));  /* 00A26B LDA */
    A &= 0x00FF;  /* 00A26D AND */
    mflag = 1;  /* 00A270 SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 00A271 STA */
    SETLO(A, rd8(ABS(0x0431)));  /* 00A273 LDA */
    SETLO(A, asl8(A));  /* 00A276 ASL */
    C = 0;  /* 00A277 CLC */
    SETLO(A, adc8(A, rd8(DP(0x04))));  /* 00A278 ADC */
    wr8(ABS(0x0431), (uint8_t)A);  /* 00A27A STA */
    mflag = 0;  /* 00A27D CLM */
    D = pull16();  /* 00A27E PLD */
    S += 3; return;  /* 00A27F RTL */
}

/* 00A280  callers: 00A375, 00BB69 */
void F_00A280(void)
{
    uint32_t t; (void)t;
    wr16(ABS(0x043F), 0x0000);  /* 00A280 LDM */
    wr16(ABS(0x043D), 0x0000);  /* 00A285 LDM */
    wr16(ABS(0x043B), 0x0000);  /* 00A28A LDM */
    wr16(ABS(0x0439), 0x0000);  /* 00A28F LDM */
    mflag = 1;  /* 00A294 SEM */
    wr8(ABS(0x0441), 0x00);  /* 00A295 LDM */
    wr8(ABS(0x0442), 0x00);  /* 00A299 LDM */
    mflag = 0;  /* 00A29D CLM */
    S += 3; return;  /* 00A29E RTL */
}

/* 00A29F  callers: 00A3A2, 00A3BB, 00C74E, 00C8C0 */
void F_00A29F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A29F LDA */
    push16(D);  /* 00A2A1 PHD */
    D = A;  /* 00A2A2 TAD */
    A += 1;  /* 00A2A3 INC */
    A += 1;  /* 00A2A4 INC */
    push16(A);  /* 00A2A5 PHA */
    X = rd16(ABS(0x03F5));  /* 00A2A6 LDX */
    wr16(DP(0x00), X);  /* 00A2A9 STX */
    mflag = 1;  /* 00A2AB SEM */
    wr8(ABS(0x03BA), 0xC0);  /* 00A2AC LDM */
L_00A2B0: TICK();
    mflag = 0;  /* 00A2B0 CLM */
    wr16(ABS(0x03F7), 0x0006);  /* 00A2B1 LDM */
    mflag = 1;  /* 00A2B6 SEM */
    SETLO(A, rd8(ABS(0x03B0)));  /* 00A2B7 LDA */
    if ((A & 0xFF) == 0) goto L_00A33B;  /* 00A2BA BEQ */
    SETLO(A, rd8(ABS(0x0432)));  /* 00A2BC LDA */
    t = rd8(ABS(0x03C6));  /* 00A2BF CMP */
    if ((A & 0xFF) != t) goto L_00A338;  /* 00A2C2 BNE */
    SETLO(A, rd8(ABS(0x03A6)));  /* 00A2C4 LDA */
    if ((A & 0xFF) != 0) goto L_00A2DF;  /* 00A2C7 BNE */
    mflag = 0;  /* 00A2C9 CLM */
    if ((rd16(ABS(0x039E)) & 0x0010) == 0x0010) goto L_00A2DF;  /* 00A2CA BBS */
    mflag = 1;  /* 00A2D0 SEM */
    wr8(ABS(0x03A6), 0x01);  /* 00A2D1 LDM */
    mflag = 0;  /* 00A2D5 CLM */
    X = rd16(ABS(0x03EF));  /* 00A2D6 LDX */
    wr16(ABS(0x03F5), X);  /* 00A2D9 STX */
    goto L_00A391;  /* 00A2DC JMP */
L_00A2DF: TICK();
    mflag = 0;  /* 00A2DF CLM */
    X = rd16(ABS(0x03F5));  /* 00A2E0 LDX */
    t = rd16(ABS(0x03ED));  /* 00A2E3 CPX */
    if (X == t) goto L_00A2EE;  /* 00A2E6 BEQ */
    if ((rd16(ABS(0x039E)) & 0x0010) == 0) goto L_00A32C;  /* 00A2E8 BBC */
L_00A2EE: TICK();
    mflag = 1;  /* 00A2EE SEM */
    wr8(ABS(0x03A6), 0x00);  /* 00A2EF LDM */
    wr8(ABS(0x03B0), 0x00);  /* 00A2F3 LDM */
    wr8(ABS(0x03A7), 0x01);  /* 00A2F7 LDM */
    SETLO(A, rd8(ABS(0x03AF)));  /* 00A2FB LDA */
    if ((A & 0xFF) == 0) goto L_00A30F;  /* 00A2FE BEQ */
    mflag = 0;  /* 00A300 CLM */
    X = rd16(ABS(0x03F9));  /* 00A301 LDX */
    wr16(ABS(0x03F5), X);  /* 00A304 STX */
    mflag = 1;  /* 00A307 SEM */
    wr8(ABS(0x03A5), 0x01);  /* 00A308 LDM */
    goto L_00A391;  /* 00A30C JMP */
L_00A30F: TICK();
    mflag = 0;  /* 00A30F CLM */
    A = rd16(ABS(0x03C6));  /* 00A310 LDA */
    A &= 0x00FF;  /* 00A313 AND */
    A = asl16(A);  /* 00A316 ASL */
    C = 0;  /* 00A317 CLC */
    A = adc16(A, 0x0161);  /* 00A318 ADC */
    Y = A;  /* 00A31B TAY */
    S -= 3; F_007271();  /* 00A31C JSRL */
    wr16(ABS(0x03F5), A);  /* 00A320 STA */
    if ((rd16(ABS(0x039E)) & 0x0010) == 0x0010) goto L_00A32C;  /* 00A323 BBS */
    Y = pull16();  /* 00A329 PLY */
    D = pull16();  /* 00A32A PLD */
    S += 3; return;  /* 00A32B RTL */
L_00A32C: TICK();
    if ((rd16(ABS(0x039E)) & 0x0010) == 0x0010) goto L_00A338;  /* 00A32C BBS */
    t = (rd16(ABS(0x03F5)) + 1) & 0xFFFF; wr16(ABS(0x03F5), t);  /* 00A332 INC */
    goto L_00A391;  /* 00A335 JMP */
L_00A338: TICK();
    goto L_00A346;  /* 00A338 JMP */
L_00A33B: TICK();
    mflag = 1;  /* 00A33B SEM */
    wr8(ABS(0x03B0), 0x01);  /* 00A33C LDM */
    SETLO(A, rd8(ABS(0x03C6)));  /* 00A340 LDA */
    wr8(ABS(0x0432), (uint8_t)A);  /* 00A343 STA */
L_00A346: TICK();
    mflag = 1;  /* 00A346 SEM */
    t = (rd8(ABS(0x03C6)) + 1) & 0xFF; wr8(ABS(0x03C6), t);  /* 00A347 INC */
    SETLO(A, rd8(ABS(0x03C6)));  /* 00A34A LDA */
    mflag = 0;  /* 00A34D CLM */
    A &= 0x00FF;  /* 00A34E AND */
    A = asl16(A);  /* 00A351 ASL */
    C = 0;  /* 00A352 CLC */
    A = adc16(A, 0x015F);  /* 00A353 ADC */
    Y = A;  /* 00A356 TAY */
    S -= 3; F_007271();  /* 00A357 JSRL */
    wr16(ABS(0x03F5), A);  /* 00A35B STA */
    A = rd16(ABS(0x03C6));  /* 00A35E LDA */
    mflag = 1;  /* 00A361 SEM */
    /* 00A362 CMP */
    if ((A & 0xFF) != 0x20) goto L_00A388;  /* 00A364 BNE */
    wr8(ABS(0x03C6), 0x00);  /* 00A366 LDM */
    mflag = 0;  /* 00A36A CLM */
    X = rd16(ABS(0x0439));  /* 00A36B LDX */
    if (X == 0) goto L_00A379;  /* 00A36E BEQ */
    X = rd16(ABS(0x043B));  /* 00A370 LDX */
    if (X == 0) goto L_00A379;  /* 00A373 BEQ */
    S -= 3; F_00A280();  /* 00A375 JSRL */
L_00A379: TICK();
    mflag = 1;  /* 00A379 SEM */
    SETLO(A, rd8(ABS(0x0441)));  /* 00A37A LDA */
    if ((A & 0xFF) == 0) goto L_00A384;  /* 00A37D BEQ */
    SETLO(A, 0x00);  /* 00A37F LDA */
    goto L_00A385;  /* 00A381 JMP */
L_00A384: TICK();
    SETLO(A, A + 1);  /* 00A384 INC */
L_00A385: TICK();
    wr8(ABS(0x0441), (uint8_t)A);  /* 00A385 STA */
L_00A388: TICK();
    mflag = 0;  /* 00A388 CLM */
    X = rd16(ABS(0x03F5));  /* 00A389 LDX */
    if (X != 0) goto L_00A391;  /* 00A38C BNE */
    goto L_00A2B0;  /* 00A38E JMP */
L_00A391: TICK();
    mflag = 0;  /* 00A391 CLM */
    S -= 3; F_00A01C();  /* 00A392 JSRL */
    Y = pull16();  /* 00A396 PLY */
    D = pull16();  /* 00A397 PLD */
    S += 3; return;  /* 00A398 RTL */
}

/* 00A399  callers: 00A434, 00A48B, 00A4B5, 00AF88, 00B0E1, 00B4A4, 00B754, 00B79E ... */
void F_00A399(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A399 LDA */
    push16(A);  /* 00A39B PHA */
    mflag = 1;  /* 00A39C SEM */
    wr8(ABS(0x01BE), 0x0B);  /* 00A39D LDM */
    mflag = 0;  /* 00A3A1 CLM */
    S -= 3; F_00A29F();  /* 00A3A2 JSRL */
    Y = pull16();  /* 00A3A6 PLY */
    S += 3; return;  /* 00A3A7 RTL */
}

/* 00A3A8  callers: 00A516, 00AF96, 00B6E6, 00C006, 00C136, 00C238, 00C28A, 00C33A ... */
void F_00A3A8(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A3A8 LDA */
    push16(A);  /* 00A3AA PHA */
    A = rd16(ABS(0x03B8));  /* 00A3AB LDA */
    mflag = 1;  /* 00A3AE SEM */
    /* 00A3AF CMP */
    if ((A & 0xFF) != 0x54) goto L_00A3C1;  /* 00A3B1 BNE */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00A3B3 LDA */
    /* 00A3B6 CMP */
    if ((A & 0xFF) == 0x82) goto L_00A3C1;  /* 00A3B8 BEQ */
    mflag = 0;  /* 00A3BA CLM */
    S -= 3; F_00A29F();  /* 00A3BB JSRL */
    Y = pull16();  /* 00A3BF PLY */
    S += 3; return;  /* 00A3C0 RTL */
L_00A3C1: TICK();
    mflag = 0;  /* 00A3C1 CLM */
    wr16(ABS(0x03B6), 0x0004);  /* 00A3C2 LDM */
    mflag = 1;  /* 00A3C7 SEM */
    wr8(ABS(0x01BE), 0x05);  /* 00A3C8 LDM */
    wr8(ABS(0x03B8), 0x00);  /* 00A3CC LDM */
    wr8(ABS(0x03A3), 0x01);  /* 00A3D0 LDM */
    wr8(ABS(0x03B0), 0x00);  /* 00A3D4 LDM */
    wr8(ABS(0x03A6), 0x00);  /* 00A3D8 LDM */
    wr8(ABS(0x03AD), 0x00);  /* 00A3DC LDM */
    wr8(ABS(0x03A2), 0x00);  /* 00A3E0 LDM */
    wr8(ABS(0x03A7), 0x00);  /* 00A3E4 LDM */
    SETLO(A, rd8(ABS(0x040E)));  /* 00A3E8 LDA */
    wr8(ABS(0x040B), (uint8_t)A);  /* 00A3EB STA */
    SETLO(A, rd8(ABS(0x040F)));  /* 00A3EE LDA */
    wr8(ABS(0x040C), (uint8_t)A);  /* 00A3F1 STA */
    SETLO(A, rd8(ABS(0x0410)));  /* 00A3F4 LDA */
    wr8(ABS(0x040D), (uint8_t)A);  /* 00A3F7 STA */
    SETLO(A, rd8(ABS(0x03B5)));  /* 00A3FA LDA */
    /* 00A3FD CMP */
    if ((A & 0xFF) != 0x10) goto L_00A407;  /* 00A3FF BNE */
    mflag = 0;  /* 00A401 CLM */
    wr16(ABS(0x03B6), 0x0005);  /* 00A402 LDM */
L_00A407: TICK();
    mflag = 0;  /* 00A407 CLM */
    Y = pull16();  /* 00A408 PLY */
    S += 3; return;  /* 00A409 RTL */
}

/* 00A40A  callers: 00C96F */
void F_00A40A(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A40A LDA */
    push16(A);  /* 00A40C PHA */
    mflag = 1;  /* 00A40D SEM */
    wr8(ABS(0x03AA), 0x00);  /* 00A40E LDM */
    SETLO(A, rd8(ABS(0x03BA)));  /* 00A412 LDA */
    /* 00A415 CMP */
    if ((A & 0xFF) == 0x02) goto L_00A41C;  /* 00A417 BEQ */
    goto L_00A4D3;  /* 00A419 JMP */
L_00A41C: TICK();
    mflag = 0;  /* 00A41C CLM */
    X = rd16(ABS(0x03B6));  /* 00A41D LDX */
    /* 00A420 CPX */
    if (X == 0x0004) goto L_00A42D;  /* 00A423 BEQ */
    /* 00A425 CPX */
    if (X == 0x0002) goto L_00A42D;  /* 00A428 BEQ */
    goto L_00A4D3;  /* 00A42A JMP */
L_00A42D: TICK();
    mflag = 1;  /* 00A42D SEM */
    SETLO(A, rd8(ABS(0x0421)));  /* 00A42E LDA */
    if ((A & 0xFF) != 0) goto L_00A438;  /* 00A431 BNE */
    mflag = 0;  /* 00A433 CLM */
    S -= 3; F_00A399();  /* 00A434 JSRL */
L_00A438: TICK();
    mflag = 1;  /* 00A438 SEM */
    SETLO(A, rd8(ABS(0x03A1)));  /* 00A439 LDA */
    if ((A & 0xFF) == 0) goto L_00A443;  /* 00A43C BEQ */
    SETLO(A, rd8(ABS(0x03A0)));  /* 00A43E LDA */
    if ((A & 0xFF) == 0) goto L_00A458;  /* 00A441 BEQ */
L_00A443: TICK();
    SETLO(A, rd8(ABS(0x040A)));  /* 00A443 LDA */
    /* 00A446 CMP */
    if ((A & 0xFF) == 0x08) goto L_00A451;  /* 00A448 BEQ */
    mflag = 0;  /* 00A44A CLM */
    if ((rd16(ABS(0x0443)) & 0x0010) == 0x0010) goto L_00A468;  /* 00A44B BBS */
L_00A451: TICK();
    mflag = 0;  /* 00A451 CLM */
    if ((rd16(ABS(0x0443)) & 0x0002) == 0x0002) goto L_00A468;  /* 00A452 BBS */
L_00A458: TICK();
    mflag = 0;  /* 00A458 CLM */
    S -= 3; F_009E45();  /* 00A459 JSRL */
    A = rd16(ABS(0x040C));  /* 00A45D LDA */
    mflag = 1;  /* 00A460 SEM */
    /* 00A461 CMP */
    if ((A & 0xFF) == 0xFF) goto L_00A468;  /* 00A463 BEQ */
    t = (rd8(ABS(0x040C)) - 1) & 0xFF; wr8(ABS(0x040C), t);  /* 00A465 DEC */
L_00A468: TICK();
    mflag = 1;  /* 00A468 SEM */
    t = (rd8(ABS(0x040B)) - 1) & 0xFF; wr8(ABS(0x040B), t);  /* 00A469 DEC */
    mflag = 0;  /* 00A46C CLM */
    X = rd16(ABS(0x03B6));  /* 00A46D LDX */
    /* 00A470 CPX */
    if (X != 0x0004) goto L_00A4AB;  /* 00A473 BNE */
    mflag = 1;  /* 00A475 SEM */
    SETLO(A, rd8(ABS(0x040B)));  /* 00A476 LDA */
    if ((A & 0xFF) != 0) goto L_00A4A8;  /* 00A479 BNE */
    SETLO(A, rd8(ABS(0x040C)));  /* 00A47B LDA */
    /* 00A47E CMP */
    if ((A & 0xFF) != 0xFF) goto L_00A492;  /* 00A480 BNE */
    t = (rd8(ABS(0x040D)) - 1) & 0xFF; wr8(ABS(0x040D), t);  /* 00A482 DEC */
    SETLO(A, rd8(ABS(0x040D)));  /* 00A485 LDA */
    if ((A & 0xFF) != 0) goto L_00A48F;  /* 00A488 BNE */
    mflag = 0;  /* 00A48A CLM */
    S -= 3; F_00A399();  /* 00A48B JSRL */
L_00A48F: TICK();
    goto L_00A49A;  /* 00A48F JMP */
L_00A492: TICK();
    mflag = 0;  /* 00A492 CLM */
    A = rd16(ABS(0x0410));  /* 00A493 LDA */
    mflag = 1;  /* 00A496 SEM */
    wr8(ABS(0x040D), (uint8_t)A);  /* 00A497 STA */
L_00A49A: TICK();
    mflag = 0;  /* 00A49A CLM */
    A = rd16(ABS(0x040E));  /* 00A49B LDA */
    mflag = 1;  /* 00A49E SEM */
    wr8(ABS(0x040B), (uint8_t)A);  /* 00A49F STA */
    SETLO(A, rd8(ABS(0x040F)));  /* 00A4A2 LDA */
    wr8(ABS(0x040C), (uint8_t)A);  /* 00A4A5 STA */
L_00A4A8: TICK();
    goto L_00A4D3;  /* 00A4A8 JMP */
L_00A4AB: TICK();
    mflag = 0;  /* 00A4AB CLM */
    A = rd16(ABS(0x040C));  /* 00A4AC LDA */
    mflag = 1;  /* 00A4AF SEM */
    /* 00A4B0 CMP */
    if ((A & 0xFF) != 0xFF) goto L_00A4B9;  /* 00A4B2 BNE */
    mflag = 0;  /* 00A4B4 CLM */
    S -= 3; F_00A399();  /* 00A4B5 JSRL */
L_00A4B9: TICK();
    mflag = 1;  /* 00A4B9 SEM */
    SETLO(A, rd8(ABS(0x040B)));  /* 00A4BA LDA */
    if ((A & 0xFF) != 0) goto L_00A4D3;  /* 00A4BD BNE */
    SETLO(A, rd8(ABS(0x0411)));  /* 00A4BF LDA */
    wr8(ABS(0x040B), (uint8_t)A);  /* 00A4C2 STA */
    SETLO(A, rd8(ABS(0x0412)));  /* 00A4C5 LDA */
    wr8(ABS(0x040C), (uint8_t)A);  /* 00A4C8 STA */
    SETLO(A, rd8(ABS(0x040D)));  /* 00A4CB LDA */
    if ((A & 0xFF) == 0) goto L_00A4D3;  /* 00A4CE BEQ */
    t = (rd8(ABS(0x040D)) - 1) & 0xFF; wr8(ABS(0x040D), t);  /* 00A4D0 DEC */
L_00A4D3: TICK();
    mflag = 0;  /* 00A4D3 CLM */
    Y = pull16();  /* 00A4D4 PLY */
    S += 3; return;  /* 00A4D5 RTL */
}

/* 00A4D6  callers: 00A512, 00ADAE, 00C5D3 */
void F_00A4D6(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A4D6 SEM */
    SETLO(A, rd8(ABS(0x03A5)));  /* 00A4D7 LDA */
    if ((A & 0xFF) == 0) goto L_00A4E1;  /* 00A4DA BEQ */
    SETLO(A, 0xB4);  /* 00A4DC LDA */
    goto L_00A4E4;  /* 00A4DE JMP */
L_00A4E1: TICK();
    SETLO(A, rd8(ABS(0x041B)));  /* 00A4E1 LDA */
L_00A4E4: TICK();
    wr8(ABS(0x0422), (uint8_t)A);  /* 00A4E4 STA */
    mflag = 0;  /* 00A4E7 CLM */
    S += 3; return;  /* 00A4E8 RTL */
}

/* 00A4E9  callers: 00B389, 00B39E, 00B3B6, 00B430, 00B43D, 00B8D3, 00B8E5, 00BC6B ... */
void F_00A4E9(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A4E9 LDA */
    push16(A);  /* 00A4EB PHA */
    mflag = 1;  /* 00A4EC SEM */
    wr8(ABS(0x01BD), 0x00);  /* 00A4ED LDM */
    wr8(ABS(0x03B8), 0x00);  /* 00A4F1 LDM */
    wr8(ABS(0x03AE), 0x00);  /* 00A4F5 LDM */
    wr8(ABS(0x03A8), 0x00);  /* 00A4F9 LDM */
    mflag = 0;  /* 00A4FD CLM */
    X = rd16(ABS(0x03F1));  /* 00A4FE LDX */
    wr16(ABS(0x03F5), X);  /* 00A501 STX */
    S -= 3; F_00A01C();  /* 00A504 JSRL */
    mflag = 1;  /* 00A508 SEM */
    wr8(ABS(0x0229), 0x01);  /* 00A509 LDM */
    wr8(ABS(0x03BA), 0x02);  /* 00A50D LDM */
    mflag = 0;  /* 00A511 CLM */
    S -= 3; F_00A4D6();  /* 00A512 JSRL */
    S -= 3; F_00A3A8();  /* 00A516 JSRL */
    Y = pull16();  /* 00A51A PLY */
    S += 3; return;  /* 00A51B RTL */
}

/* 00A51C  callers: 00B732, 00B916, 00C1E3 */
void F_00A51C(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x03B8));  /* 00A51C LDA */
    mflag = 1;  /* 00A51F SEM */
    /* 00A520 CMP */
    if ((A & 0xFF) == 0x54) goto L_00A52F;  /* 00A522 BEQ */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00A524 LDA */
    /* 00A527 CMP */
    if ((A & 0xFF) == 0x58) goto L_00A52F;  /* 00A529 BEQ */
    /* 00A52B CMP */
    if ((A & 0xFF) != 0x5C) goto L_00A538;  /* 00A52D BNE */
L_00A52F: TICK();
    SETLO(A, rd8(ABS(0x0419)));  /* 00A52F LDA */
    wr8(ABS(0x0420), (uint8_t)A);  /* 00A532 STA */
    goto L_00A53D;  /* 00A535 JMP */
L_00A538: TICK();
    mflag = 1;  /* 00A538 SEM */
    wr8(ABS(0x0420), 0x3C);  /* 00A539 LDM */
L_00A53D: TICK();
    mflag = 0;  /* 00A53D CLM */
    S += 3; return;  /* 00A53E RTL */
}

/* 00A53F  callers: 00AAFE */
void F_00A53F(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A53F SEM */
    SETLO(A, rd8(ABS(0x03A5)));  /* 00A540 LDA */
    if ((A & 0xFF) == 0) goto L_00A54A;  /* 00A543 BEQ */
    SETLO(A, 0xB4);  /* 00A545 LDA */
    goto L_00A54D;  /* 00A547 JMP */
L_00A54A: TICK();
    SETLO(A, rd8(ABS(0x0417)));  /* 00A54A LDA */
L_00A54D: TICK();
    wr8(ABS(0x0420), (uint8_t)A);  /* 00A54D STA */
    mflag = 0;  /* 00A550 CLM */
    S += 3; return;  /* 00A551 RTL */
}

/* 00A552  callers: 00BD81, 00BE6E, 00BEAC */
void F_00A552(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A552 SEM */
    wr8(ABS(0x0420), 0x03);  /* 00A553 LDM */
    mflag = 0;  /* 00A557 CLM */
    S += 3; return;  /* 00A558 RTL */
}

/* 00A559  callers: 00BD20, 00BDD5 */
void F_00A559(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A559 LDA */
    push16(D);  /* 00A55B PHD */
    D = A;  /* 00A55C TAD */
    A = rd16(ABS(0x03B3));  /* 00A55D LDA */
    mflag = 1;  /* 00A560 SEM */
    mpy8(0x56);  /* 00A561 MPY */
    wr8(DP(0x00), (uint8_t)A);  /* 00A564 STA */
    wr8(DP(0x01), (uint8_t)B);  /* 00A566 STA */
    mflag = 0;  /* 00A569 CLM */
    A = rd16(DP(0x00));  /* 00A56A LDA */
    C = 0;  /* 00A56C CLC */
    A = adc16(A, 0x007D);  /* 00A56D ADC */
    B = 0x0020;  /* 00A570 LDA */
    S -= 3; F_00FF8C();  /* 00A574 JSRL */
    mflag = 1;  /* 00A578 SEM */
    wr8(ABS(0x0424), (uint8_t)A);  /* 00A579 STA */
    mflag = 0;  /* 00A57C CLM */
    D = pull16();  /* 00A57D PLD */
    S += 3; return;  /* 00A57E RTL */
}

/* 00A57F  callers: 00A846, 00BF41, 00CA4A */
void F_00A57F(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A57F LDA */
    push16(D);  /* 00A581 PHD */
    D = A;  /* 00A582 TAD */
    C = 0;  /* 00A583 CLC */
    A = adc16(A, 0x0004);  /* 00A584 ADC */
    push16(A);  /* 00A587 PHA */
    wr16(ABS(0x0445), 0x0001);  /* 00A588 LDM */
    mflag = 1;  /* 00A58D SEM */
    wr8(ABS(0x0409), 0x00);  /* 00A58E LDM */
    mflag = 0;  /* 00A592 CLM */
    wr16(DP(0x00), 0x0009);  /* 00A593 LDM */
    wr16(DP(0x02), 0x0040);  /* 00A597 LDM */
    Y = 0x0002;  /* 00A59B LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00A59E JSRL */
    mflag = 0;  /* 00A5A2 CLM */
    wr16(DP(0x00), 0x00AA);  /* 00A5A3 LDM */
    wr16(DP(0x02), 0x0043);  /* 00A5A7 LDM */
    Y = 0x0002;  /* 00A5AB LDY */
    S -= 3; cbus_write_sel1_007E4C();  /* 00A5AE JSRL */
    mflag = 0;  /* 00A5B2 CLM */
    wr16(ABS(0x0443), rd16(ABS(0x0443)) & ~0x0008);  /* 00A5B3 CLB */
    mflag = 1;  /* 00A5B8 SEM */
    SETLO(A, rd8(ABS(0x03A8)));  /* 00A5B9 LDA */
    if ((A & 0xFF) == 0) goto L_00A5C9;  /* 00A5BC BEQ */
    wr8(ABS(0x03C7), 0x3B);  /* 00A5BE LDM */
    wr8(ABS(0x03C8), 0x28);  /* 00A5C2 LDM */
    goto L_00A5D2;  /* 00A5C6 JMP */
L_00A5C9: TICK();
    mflag = 1;  /* 00A5C9 SEM */
    wr8(ABS(0x03C7), 0xC4);  /* 00A5CA LDM */
    wr8(ABS(0x03C8), 0xD7);  /* 00A5CE LDM */
L_00A5D2: TICK();
    mflag = 0;  /* 00A5D2 CLM */
    Y = pull16();  /* 00A5D3 PLY */
    D = pull16();  /* 00A5D4 PLD */
    S += 3; return;  /* 00A5D5 RTL */
}

/* 00A5D6  callers: 00A752, 00A764, 00A775, 00A787, 00A798, 00A7F9, 00A80B, 00A81C ... */
void F_00A5D6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A5D6 LDA */
    push16(D);  /* 00A5D8 PHD */
    D = A;  /* 00A5D9 TAD */
    wr16(DP(0x00), Y);  /* 00A5DA STY */
    mflag = 1;  /* 00A5DC SEM */
    wr8(DP(0x02), (uint8_t)B);  /* 00A5DD STA */
    mflag = 0;  /* 00A5E0 CLM */
    A = rd16(LIND(0x00));  /* 00A5E1 LDA */
    A += 1;  /* 00A5E3 INC */
    wr16(LIND(0x00), A);  /* 00A5E4 STA */
    A = rd16(ABS(0x01C3));  /* 00A5E6 LDA */
    A &= 0x00FF;  /* 00A5E9 AND */
    t = rd16(LIND(0x00));  /* 00A5EC CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_00A5FE;  /* 00A5EE BCC */
    if (Z) goto L_00A5FE;  /* 00A5F0 BEQ */
    A = rd16(LIND(0x00));  /* 00A5F2 LDA */
    X = A;  /* 00A5F4 TAX */
    A = rd16((ABS(0x0283) + X));  /* 00A5F5 LDA */
    mflag = 1;  /* 00A5F8 SEM */
    SETLO(A, adc8(A, 0xCF));  /* 00A5F9 ADC */
    goto L_00A602;  /* 00A5FB JMP */
L_00A5FE: TICK();
    mflag = 0;  /* 00A5FE CLM */
    A = 0x000F;  /* 00A5FF LDA */
L_00A602: TICK();
    mflag = 1;  /* 00A602 SEM */
    D = pull16();  /* 00A603 PLD */
    S += 3; return;  /* 00A604 RTL */
}

/* 00A605  callers: 00B81C */
void F_00A605(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A605 LDA */
    push16(D);  /* 00A607 PHD */
    D = A;  /* 00A608 TAD */
    C = 0;  /* 00A609 CLC */
    A = adc16(A, 0x0006);  /* 00A60A ADC */
    push16(A);  /* 00A60D PHA */
    mflag = 1;  /* 00A60E SEM */
    wr8(ABS(0x03AE), 0x01);  /* 00A60F LDM */
    wr8(ABS(0x03CC), 0x80);  /* 00A613 LDM */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00A617 LDA */
    /* 00A61A CMP */
    if ((A & 0xFF) == 0x09) goto L_00A621;  /* 00A61C BEQ */
    goto L_00A6DE;  /* 00A61E JMP */
L_00A621: TICK();
    Y = 0x0001;  /* 00A621 LDY */
    mflag = 0;  /* 00A624 CLM */
    S -= 3; F_007271();  /* 00A625 JSRL */
    wr16(DP(0x02), A);  /* 00A629 STA */
    A = rla16(A, 0x0008);  /* 00A62B RLA */
    A &= 0x00FF;  /* 00A62F AND */
    mflag = 1;  /* 00A632 SEM */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00A633 STA */
    SETLO(A, rd8(DP(0x02)));  /* 00A636 LDA */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A638 STA */
    Y = 0x021E;  /* 00A63B LDY */
    mflag = 0;  /* 00A63E CLM */
    S -= 3; eeprom_read_byte_007189();  /* 00A63F JSRL */
    mflag = 0;  /* 00A643 CLM */
    A &= 0x00FF;  /* 00A644 AND */
    wr16(DP(0x06), A);  /* 00A647 STA */
    Y = 0x021C;  /* 00A649 LDY */
    A = D;  /* 00A64C TDA */
    C = 0;  /* 00A64D CLC */
    A = adc16(A, 0x0008);  /* 00A64E ADC */
    push16(A);  /* 00A651 PHA */
    S -= 3; eeprom_read_byte_007189();  /* 00A652 JSRL */
    Y = pull16();  /* 00A656 PLY */
    mflag = 0;  /* 00A657 CLM */
    A &= 0x00FF;  /* 00A658 AND */
    A = rla16(A, 0x0008);  /* 00A65B RLA */
    A &= 0xFF00;  /* 00A65F AND */
    C = 0;  /* 00A662 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 00A663 ADC */
    wr16(DP(0x04), A);  /* 00A665 STA */
    A = rla16(A, 0x0008);  /* 00A667 RLA */
    A &= 0x00FF;  /* 00A66B AND */
    mflag = 1;  /* 00A66E SEM */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A66F STA */
    SETLO(A, rd8(DP(0x04)));  /* 00A672 LDA */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A674 STA */
    SETLO(B, 0x00);  /* 00A677 LDA */
    div8(0x40);  /* 00A67A DIV */
    SETLO(A, A | 0x80);  /* 00A67D ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A67F STA */
    Y = 0x0220;  /* 00A682 LDY */
    mflag = 0;  /* 00A685 CLM */
    S -= 3; eeprom_read_byte_007189();  /* 00A686 JSRL */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A68A STA */
    mflag = 0;  /* 00A68D CLM */
    wr16(DP(0x04), rd16(DP(0x04)) & ~0x00FF);  /* 00A68E CLB */
    mflag = 1;  /* 00A692 SEM */
    SETLO(B, 0x00);  /* 00A693 LDA */
    SETLO(A, rd8(ABS(0x03CB)));  /* 00A696 LDA */
    div8(0x10);  /* 00A699 DIV */
    wr8(DP(0x06), (uint8_t)A);  /* 00A69C STA */
    SETLO(A, rd8(ABS(0x03CA)));  /* 00A69E LDA */
    SETLO(A, asl8(A));  /* 00A6A1 ASL */
    SETLO(A, asl8(A));  /* 00A6A2 ASL */
    SETLO(A, asl8(A));  /* 00A6A3 ASL */
    SETLO(A, asl8(A));  /* 00A6A4 ASL */
    C = 0;  /* 00A6A5 CLC */
    SETLO(A, adc8(A, rd8(DP(0x06))));  /* 00A6A6 ADC */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A6A8 STA */
    mflag = 0;  /* 00A6AB CLM */
    A &= 0x00FF;  /* 00A6AC AND */
    C = 0;  /* 00A6AF CLC */
    A = adc16(A, rd16(DP(0x04)));  /* 00A6B0 ADC */
    wr16(DP(0x04), A);  /* 00A6B2 STA */
    A = rd16(ABS(0x03CB));  /* 00A6B4 LDA */
    mflag = 1;  /* 00A6B7 SEM */
    SETLO(A, asl8(A));  /* 00A6B8 ASL */
    SETLO(A, asl8(A));  /* 00A6B9 ASL */
    SETLO(A, asl8(A));  /* 00A6BA ASL */
    C = 0;  /* 00A6BB CLC */
    SETLO(A, adc8(A, 0x04));  /* 00A6BC ADC */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A6BE STA */
    mflag = 0;  /* 00A6C1 CLM */
    A = rd16(DP(0x04));  /* 00A6C2 LDA */
    A = lsr16(A);  /* 00A6C4 LSR */
    C = 0;  /* 00A6C5 CLC */
    A = adc16(A, 0x8000);  /* 00A6C6 ADC */
    wr16(DP(0x04), A);  /* 00A6C9 STA */
    A = rla16(A, 0x0008);  /* 00A6CB RLA */
    A &= 0x00FF;  /* 00A6CF AND */
    mflag = 1;  /* 00A6D2 SEM */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A6D3 STA */
    SETLO(A, rd8(DP(0x04)));  /* 00A6D6 LDA */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A6D8 STA */
    goto L_00A838;  /* 00A6DB JMP */
L_00A6DE: TICK();
    mflag = 0;  /* 00A6DE CLM */
    A = rd16(ABS(0x03B9));  /* 00A6DF LDA */
    mflag = 1;  /* 00A6E2 SEM */
    /* 00A6E3 CMP */
    if ((A & 0xFF) != 0x0B) goto L_00A71C;  /* 00A6E5 BNE */
    SETLO(A, rd8(ABS(0x01B8)));  /* 00A6E7 LDA */
    SETLO(A, A | 0x80);  /* 00A6EA ORA */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A6EC STA */
    mflag = 0;  /* 00A6EF CLM */
    A = rd16(ABS(0x01CF));  /* 00A6F0 LDA */
    A = asl16(A);  /* 00A6F3 ASL */
    A = asl16(A);  /* 00A6F4 ASL */
    A = asl16(A);  /* 00A6F5 ASL */
    C = 0;  /* 00A6F6 CLC */
    A = adc16(A, 0x0004);  /* 00A6F7 ADC */
    A = rla16(A, 0x0008);  /* 00A6FA RLA */
    A &= 0x00FF;  /* 00A6FE AND */
    mflag = 1;  /* 00A701 SEM */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A702 STA */
    SETLO(A, rd8(ABS(0x01CF)));  /* 00A705 LDA */
    SETLO(A, asl8(A));  /* 00A708 ASL */
    SETLO(A, asl8(A));  /* 00A709 ASL */
    SETLO(A, asl8(A));  /* 00A70A ASL */
    C = 0;  /* 00A70B CLC */
    SETLO(A, adc8(A, 0x04));  /* 00A70C ADC */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A70E STA */
    wr8(ABS(0x03CE), 0x00);  /* 00A711 LDM */
    wr8(ABS(0x03CD), 0x00);  /* 00A715 LDM */
    goto L_00A838;  /* 00A719 JMP */
L_00A71C: TICK();
    mflag = 0;  /* 00A71C CLM */
    A = rd16(ABS(0x03B9));  /* 00A71D LDA */
    mflag = 1;  /* 00A720 SEM */
    /* 00A721 CMP */
    if ((A & 0xFF) == 0x0C) goto L_00A72F;  /* 00A723 BEQ */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00A725 LDA */
    /* 00A728 CMP */
    if ((A & 0xFF) == 0x0D) goto L_00A72F;  /* 00A72A BEQ */
    goto L_00A838;  /* 00A72C JMP */
L_00A72F: TICK();
    mflag = 0;  /* 00A72F CLM */
    wr16(DP(0x00), 0x0000);  /* 00A730 LDM */
L_00A734: TICK();
    X = rd16(DP(0x00));  /* 00A734 LDX */
    /* 00A736 CPX */
    if (X >= 0x000A) goto L_00A74B;  /* 00A739 BCS */
    A = rd16((ABS(0x0283) + X));  /* 00A73B LDA */
    mflag = 1;  /* 00A73E SEM */
    /* 00A73F CMP */
    if ((A & 0xFF) == 0x30) goto L_00A74B;  /* 00A741 BEQ */
    mflag = 0;  /* 00A743 CLM */
    t = (rd16(DP(0x00)) + 1) & 0xFFFF; wr16(DP(0x00), t);  /* 00A744 INC */
    wr16(DP(0x00), X);  /* 00A746 STX */
    goto L_00A734;  /* 00A748 JMP */
L_00A74B: TICK();
    mflag = 0;  /* 00A74B CLM */
    A = D;  /* 00A74C TDA */
    B = 0x0000;  /* 00A74D LDA */
    Y = A;  /* 00A751 TAY */
    S -= 3; F_00A5D6();  /* 00A752 JSRL */
    SETLO(A, asl8(A));  /* 00A756 ASL */
    SETLO(A, asl8(A));  /* 00A757 ASL */
    SETLO(A, asl8(A));  /* 00A758 ASL */
    SETLO(A, asl8(A));  /* 00A759 ASL */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A75A STA */
    mflag = 0;  /* 00A75D CLM */
    A = D;  /* 00A75E TDA */
    B = 0x0000;  /* 00A75F LDA */
    Y = A;  /* 00A763 TAY */
    S -= 3; F_00A5D6();  /* 00A764 JSRL */
    SETLO(A, A | rd8(ABS(0x03C9)));  /* 00A768 ORA */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A76B STA */
    mflag = 0;  /* 00A76E CLM */
    A = D;  /* 00A76F TDA */
    B = 0x0000;  /* 00A770 LDA */
    Y = A;  /* 00A774 TAY */
    S -= 3; F_00A5D6();  /* 00A775 JSRL */
    SETLO(A, asl8(A));  /* 00A779 ASL */
    SETLO(A, asl8(A));  /* 00A77A ASL */
    SETLO(A, asl8(A));  /* 00A77B ASL */
    SETLO(A, asl8(A));  /* 00A77C ASL */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A77D STA */
    mflag = 0;  /* 00A780 CLM */
    A = D;  /* 00A781 TDA */
    B = 0x0000;  /* 00A782 LDA */
    Y = A;  /* 00A786 TAY */
    S -= 3; F_00A5D6();  /* 00A787 JSRL */
    SETLO(A, A | rd8(ABS(0x03CA)));  /* 00A78B ORA */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A78E STA */
    mflag = 0;  /* 00A791 CLM */
    A = D;  /* 00A792 TDA */
    B = 0x0000;  /* 00A793 LDA */
    Y = A;  /* 00A797 TAY */
    S -= 3; F_00A5D6();  /* 00A798 JSRL */
    SETLO(A, asl8(A));  /* 00A79C ASL */
    SETLO(A, asl8(A));  /* 00A79D ASL */
    SETLO(A, asl8(A));  /* 00A79E ASL */
    wr8(DP(0x06), (uint8_t)A);  /* 00A79F STA */
    SETLO(A, rd8(ABS(0x03CA)));  /* 00A7A1 LDA */
    SETLO(A, A & 0x01);  /* 00A7A4 AND */
    SETLO(A, lsr8(A));  /* 00A7A6 LSR */
    SETLO(A, ror8(A));  /* 00A7A7 ROR */
    C = 0;  /* 00A7A8 CLC */
    SETLO(A, adc8(A, rd8(DP(0x06))));  /* 00A7A9 ADC */
    C = 0;  /* 00A7AB CLC */
    SETLO(A, adc8(A, 0x04));  /* 00A7AC ADC */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A7AE STA */
    SETLO(A, rd8(ABS(0x03C9)));  /* 00A7B1 LDA */
    mflag = 0;  /* 00A7B4 CLM */
    A &= 0x00FF;  /* 00A7B5 AND */
    A = rla16(A, 0x0008);  /* 00A7B8 RLA */
    A &= 0xFF00;  /* 00A7BC AND */
    wr16(DP(0x06), A);  /* 00A7BF STA */
    A = rd16(ABS(0x03CA));  /* 00A7C1 LDA */
    A &= 0x00FF;  /* 00A7C4 AND */
    C = 0;  /* 00A7C7 CLC */
    A = adc16(A, rd16(DP(0x06)));  /* 00A7C8 ADC */
    A = lsr16(A);  /* 00A7CA LSR */
    C = 0;  /* 00A7CB CLC */
    A = adc16(A, 0x8000);  /* 00A7CC ADC */
    wr16(DP(0x04), A);  /* 00A7CF STA */
    A = rla16(A, 0x0008);  /* 00A7D1 RLA */
    A &= 0x00FF;  /* 00A7D5 AND */
    mflag = 1;  /* 00A7D8 SEM */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A7D9 STA */
    SETLO(A, rd8(DP(0x04)));  /* 00A7DC LDA */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A7DE STA */
    SETLO(A, rd8(ABS(0x03B9)));  /* 00A7E1 LDA */
    /* 00A7E4 CMP */
    if ((A & 0xFF) != 0x0C) goto L_00A7ED;  /* 00A7E6 BNE */
    SETLO(A, 0x84);  /* 00A7E8 LDA */
    goto L_00A7EF;  /* 00A7EA JMP */
L_00A7ED: TICK();
    SETLO(A, 0x88);  /* 00A7ED LDA */
L_00A7EF: TICK();
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A7EF STA */
    mflag = 0;  /* 00A7F2 CLM */
    A = D;  /* 00A7F3 TDA */
    B = 0x0000;  /* 00A7F4 LDA */
    Y = A;  /* 00A7F8 TAY */
    S -= 3; F_00A5D6();  /* 00A7F9 JSRL */
    SETLO(A, asl8(A));  /* 00A7FD ASL */
    SETLO(A, asl8(A));  /* 00A7FE ASL */
    SETLO(A, asl8(A));  /* 00A7FF ASL */
    SETLO(A, asl8(A));  /* 00A800 ASL */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00A801 STA */
    mflag = 0;  /* 00A804 CLM */
    A = D;  /* 00A805 TDA */
    B = 0x0000;  /* 00A806 LDA */
    Y = A;  /* 00A80A TAY */
    S -= 3; F_00A5D6();  /* 00A80B JSRL */
    SETLO(A, A | rd8(ABS(0x03CD)));  /* 00A80F ORA */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00A812 STA */
    mflag = 0;  /* 00A815 CLM */
    A = D;  /* 00A816 TDA */
    B = 0x0000;  /* 00A817 LDA */
    Y = A;  /* 00A81B TAY */
    S -= 3; F_00A5D6();  /* 00A81C JSRL */
    SETLO(A, asl8(A));  /* 00A820 ASL */
    SETLO(A, asl8(A));  /* 00A821 ASL */
    SETLO(A, asl8(A));  /* 00A822 ASL */
    SETLO(A, asl8(A));  /* 00A823 ASL */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A824 STA */
    mflag = 0;  /* 00A827 CLM */
    A = D;  /* 00A828 TDA */
    B = 0x0000;  /* 00A829 LDA */
    Y = A;  /* 00A82D TAY */
    S -= 3; F_00A5D6();  /* 00A82E JSRL */
    SETLO(A, A | rd8(ABS(0x03CE)));  /* 00A832 ORA */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A835 STA */
L_00A838: TICK();
    mflag = 0;  /* 00A838 CLM */
    Y = pull16();  /* 00A839 PLY */
    D = pull16();  /* 00A83A PLD */
    S += 3; return;  /* 00A83B RTL */
}

/* 00A83C  callers: 00A939, 00BC9C, 00BCFD */
void F_00A83C(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A83C LDA */
    push16(A);  /* 00A83E PHA */
    Y = 0x000F;  /* 00A83F LDY */
    S -= 3; F_007D7F();  /* 00A842 JSRL */
    S -= 3; F_00A57F();  /* 00A846 JSRL */
    A = rd16(ABS(0x0425));  /* 00A84A LDA */
    mflag = 1;  /* 00A84D SEM */
    SETLO(A, A | 0x80);  /* 00A84E ORA */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A850 STA */
    mflag = 0;  /* 00A853 CLM */
    A = rd16(ABS(0x03FB));  /* 00A854 LDA */
    A = asl16(A);  /* 00A857 ASL */
    A = asl16(A);  /* 00A858 ASL */
    A = asl16(A);  /* 00A859 ASL */
    A |= 0x0004;  /* 00A85A ORA */
    A = rla16(A, 0x0008);  /* 00A85D RLA */
    A &= 0x00FF;  /* 00A861 AND */
    mflag = 1;  /* 00A864 SEM */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A865 STA */
    SETLO(A, rd8(ABS(0x03FB)));  /* 00A868 LDA */
    SETLO(A, asl8(A));  /* 00A86B ASL */
    SETLO(A, asl8(A));  /* 00A86C ASL */
    SETLO(A, asl8(A));  /* 00A86D ASL */
    SETLO(A, A | 0x04);  /* 00A86E ORA */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A870 STA */
    mflag = 0;  /* 00A873 CLM */
    A = rd16(ABS(0x03F6));  /* 00A874 LDA */
    A &= 0x00FF;  /* 00A877 AND */
    mflag = 1;  /* 00A87A SEM */
    SETLO(A, A | 0x64);  /* 00A87B ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A87D STA */
    SETLO(A, rd8(ABS(0x03F5)));  /* 00A880 LDA */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00A883 STA */
    mflag = 0;  /* 00A886 CLM */
    Y = pull16();  /* 00A887 PLY */
    S += 3; return;  /* 00A888 RTL */
}

/* 00A889  callers: 00B70A, 00B7F2, 00B88A, 00B8AE, 00B934, 00BE9D, 00C351, 00C3B6 */
void F_00A889(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A889 SEM */
    wr8(ABS(0x03AE), 0x01);  /* 00A88A LDM */
    SETLO(A, rd8(ABS(0x03CB)));  /* 00A88E LDA */
    SETLO(A, A & 0xF8);  /* 00A891 AND */
    SETLO(A, A | 0x04);  /* 00A893 ORA */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A895 STA */
    SETLO(A, rd8(ABS(0x03CC)));  /* 00A898 LDA */
    SETLO(A, A & 0x03);  /* 00A89B AND */
    SETLO(A, A | 0x20);  /* 00A89D ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A89F STA */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) & ~0x1F);  /* 00A8A2 CLB */
    mflag = 0;  /* 00A8A6 CLM */
    S += 3; return;  /* 00A8A7 RTL */
}

/* 00A8A8  callers: 00B892 */
void F_00A8A8(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A8A8 SEM */
    wr8(ABS(0x01BE), 0x12);  /* 00A8A9 LDM */
    wr8(ABS(0x041E), 0x3C);  /* 00A8AD LDM */
    wr8(ABS(0x03AE), 0x01);  /* 00A8B1 LDM */
    SETLO(A, rd8(ABS(0x03CB)));  /* 00A8B5 LDA */
    SETLO(A, A & 0xF8);  /* 00A8B8 AND */
    SETLO(A, A | 0x04);  /* 00A8BA ORA */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A8BC STA */
    SETLO(A, rd8(ABS(0x03CC)));  /* 00A8BF LDA */
    SETLO(A, A & 0x03);  /* 00A8C2 AND */
    SETLO(A, A | 0x24);  /* 00A8C4 ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A8C6 STA */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) & ~0x1F);  /* 00A8C9 CLB */
    mflag = 0;  /* 00A8CD CLM */
    S += 3; return;  /* 00A8CE RTL */
}

/* 00A8CF  callers: 00B85C, 00B93C, 00C07A */
void F_00A8CF(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A8CF SEM */
    wr8(ABS(0x03AE), 0x01);  /* 00A8D0 LDM */
    SETLO(A, rd8(ABS(0x03CB)));  /* 00A8D4 LDA */
    SETLO(A, A & 0xF8);  /* 00A8D7 AND */
    SETLO(A, A | 0x04);  /* 00A8D9 ORA */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A8DB STA */
    SETLO(A, rd8(ABS(0x03CC)));  /* 00A8DE LDA */
    SETLO(A, A & 0x03);  /* 00A8E1 AND */
    SETLO(A, A | 0x2C);  /* 00A8E3 ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A8E5 STA */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) & ~0x1F);  /* 00A8E8 CLB */
    mflag = 0;  /* 00A8EC CLM */
    S += 3; return;  /* 00A8ED RTL */
}

/* 00A8EE  callers: 00B02E */
void F_00A8EE(void)
{
    uint32_t t; (void)t;
    mflag = 1;  /* 00A8EE SEM */
    wr8(ABS(0x03AE), 0x01);  /* 00A8EF LDM */
    SETLO(A, rd8(ABS(0x0425)));  /* 00A8F3 LDA */
    SETLO(A, A | 0x80);  /* 00A8F6 ORA */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A8F8 STA */
    wr8(ABS(0x03CA), 0xFF);  /* 00A8FB LDM */
    wr8(ABS(0x03CB), 0xF4);  /* 00A8FF LDM */
    mflag = 0;  /* 00A903 CLM */
    A = rd16(ABS(0x03FC));  /* 00A904 LDA */
    A &= 0x00FF;  /* 00A907 AND */
    A = lsr16(A);  /* 00A90A LSR */
    A = lsr16(A);  /* 00A90B LSR */
    A = lsr16(A);  /* 00A90C LSR */
    mflag = 1;  /* 00A90D SEM */
    SETLO(A, A | 0x2C);  /* 00A90E ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A910 STA */
    mflag = 0;  /* 00A913 CLM */
    A = rd16(ABS(0x03FB));  /* 00A914 LDA */
    A = asl16(A);  /* 00A917 ASL */
    A = asl16(A);  /* 00A918 ASL */
    A = asl16(A);  /* 00A919 ASL */
    A = asl16(A);  /* 00A91A ASL */
    A = asl16(A);  /* 00A91B ASL */
    A = rla16(A, 0x0008);  /* 00A91C RLA */
    A &= 0x00FF;  /* 00A920 AND */
    mflag = 1;  /* 00A923 SEM */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00A924 STA */
    SETLO(A, rd8(ABS(0x03FB)));  /* 00A927 LDA */
    SETLO(A, asl8(A));  /* 00A92A ASL */
    SETLO(A, asl8(A));  /* 00A92B ASL */
    SETLO(A, asl8(A));  /* 00A92C ASL */
    SETLO(A, asl8(A));  /* 00A92D ASL */
    SETLO(A, asl8(A));  /* 00A92E ASL */
    SETLO(A, A & 0xE0);  /* 00A92F AND */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A931 STA */
    mflag = 0;  /* 00A934 CLM */
    S += 3; return;  /* 00A935 RTL */
}

/* 00A936  callers: 00BC78, 00BCE0, 00BD53, 00BF73 */
void F_00A936(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A936 LDA */
    push16(A);  /* 00A938 PHA */
    S -= 3; F_00A83C();  /* 00A939 JSRL */
    mflag = 1;  /* 00A93D SEM */
    t = (rd8(ABS(0x0414)) - 1) & 0xFF; wr8(ABS(0x0414), t);  /* 00A93E DEC */
    wr8(ABS(0x03CE), 0x60);  /* 00A941 LDM */
    mflag = 0;  /* 00A945 CLM */
    Y = pull16();  /* 00A946 PLY */
    S += 3; return;  /* 00A947 RTL */
}

/* 00A948  callers: 00B001, 00BD8F, 00C27F */
void F_00A948(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00A948 LDA */
    push16(D);  /* 00A94A PHD */
    D = A;  /* 00A94B TAD */
    A += 1;  /* 00A94C INC */
    A += 1;  /* 00A94D INC */
    push16(A);  /* 00A94E PHA */
    mflag = 1;  /* 00A94F SEM */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00A950 LDA */
    if ((A & 0xFF) == 0) goto L_00A95C;  /* 00A953 BEQ */
    wr8(ABS(0x03AE), 0x01);  /* 00A955 LDM */
    goto L_00A960;  /* 00A959 JMP */
L_00A95C: TICK();
    mflag = 0;  /* 00A95C CLM */
    Y = pull16();  /* 00A95D PLY */
    D = pull16();  /* 00A95E PLD */
    S += 3; return;  /* 00A95F RTL */
L_00A960: TICK();
    SETLO(A, rd8(ABS(0x0425)));  /* 00A960 LDA */
    SETLO(A, A | 0x80);  /* 00A963 ORA */
    wr8(ABS(0x03C9), (uint8_t)A);  /* 00A965 STA */
    SETLO(A, rd8(ABS(0x01B8)));  /* 00A968 LDA */
    t = rd8(ABS(0x0425));  /* 00A96B CMP */
    if ((A & 0xFF) != t) goto L_00A995;  /* 00A96E BNE */
    mflag = 0;  /* 00A970 CLM */
    A = rd16(ABS(0x01CF));  /* 00A971 LDA */
    A = asl16(A);  /* 00A974 ASL */
    A = asl16(A);  /* 00A975 ASL */
    A = asl16(A);  /* 00A976 ASL */
    C = 0;  /* 00A977 CLC */
    A = adc16(A, 0x0004);  /* 00A978 ADC */
    A = rla16(A, 0x0008);  /* 00A97B RLA */
    A &= 0x00FF;  /* 00A97F AND */
    mflag = 1;  /* 00A982 SEM */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00A983 STA */
    SETLO(A, rd8(ABS(0x01CF)));  /* 00A986 LDA */
    SETLO(A, asl8(A));  /* 00A989 ASL */
    SETLO(A, asl8(A));  /* 00A98A ASL */
    SETLO(A, asl8(A));  /* 00A98B ASL */
    C = 0;  /* 00A98C CLC */
    SETLO(A, adc8(A, 0x04));  /* 00A98D ADC */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00A98F STA */
    goto L_00A99E;  /* 00A992 JMP */
L_00A995: TICK();
    mflag = 1;  /* 00A995 SEM */
    wr8(ABS(0x03CA), 0xFF);  /* 00A996 LDM */
    wr8(ABS(0x03CB), 0xEC);  /* 00A99A LDM */
L_00A99E: TICK();
    mflag = 0;  /* 00A99E CLM */
    A = rd16(ABS(0x03FC));  /* 00A99F LDA */
    A &= 0x00FF;  /* 00A9A2 AND */
    A = lsr16(A);  /* 00A9A5 LSR */
    A = lsr16(A);  /* 00A9A6 LSR */
    A = lsr16(A);  /* 00A9A7 LSR */
    mflag = 1;  /* 00A9A8 SEM */
    SETLO(A, A | rd8(ABS(0x03B8)));  /* 00A9A9 ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00A9AC STA */
    mflag = 0;  /* 00A9AF CLM */
    A = rd16(ABS(0x03FB));  /* 00A9B0 LDA */
    A = asl16(A);  /* 00A9B3 ASL */
    A = asl16(A);  /* 00A9B4 ASL */
    A = asl16(A);  /* 00A9B5 ASL */
    A = asl16(A);  /* 00A9B6 ASL */
    A = asl16(A);  /* 00A9B7 ASL */
    A = rla16(A, 0x0008);  /* 00A9B8 RLA */
    A &= 0x00FF;  /* 00A9BC AND */
    mflag = 1;  /* 00A9BF SEM */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00A9C0 STA */
    SETLO(A, rd8(ABS(0x03FB)));  /* 00A9C3 LDA */
    SETLO(A, asl8(A));  /* 00A9C6 ASL */
    SETLO(A, asl8(A));  /* 00A9C7 ASL */
    SETLO(A, asl8(A));  /* 00A9C8 ASL */
    SETLO(A, asl8(A));  /* 00A9C9 ASL */
    SETLO(A, asl8(A));  /* 00A9CA ASL */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A9CB STA */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00A9CE LDA */
    /* 00A9D1 CMP */
    if ((A & 0xFF) != 0x58) goto L_00A9DE;  /* 00A9D3 BNE */
    SETLO(A, rd8(ABS(0x03CE)));  /* 00A9D5 LDA */
    SETLO(A, A | rd8(ABS(0x0434)));  /* 00A9D8 ORA */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A9DB STA */
L_00A9DE: TICK();
    SETLO(A, rd8(ABS(0x01E2)));  /* 00A9DE LDA */
    if ((A & 0xFF) == 0) goto L_00A9EC;  /* 00A9E1 BEQ */
    SETLO(A, rd8(ABS(0x03CE)));  /* 00A9E3 LDA */
    C = 0;  /* 00A9E6 CLC */
    SETLO(A, adc8(A, 0x10));  /* 00A9E7 ADC */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00A9E9 STA */
L_00A9EC: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00A9EC LDA */
    /* 00A9EF CMP */
    if ((A & 0xFF) != 0x40) goto L_00A9FC;  /* 00A9F1 BNE */
    SETLO(A, rd8(ABS(0x01E3)));  /* 00A9F3 LDA */
    if ((A & 0xFF) == 0) goto L_00A9FC;  /* 00A9F6 BEQ */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) | 0x08);  /* 00A9F8 SEB */
L_00A9FC: TICK();
    SETLO(A, rd8(ABS(0x01E4)));  /* 00A9FC LDA */
    if ((A & 0xFF) == 0) goto L_00AA05;  /* 00A9FF BEQ */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) | 0x04);  /* 00AA01 SEB */
L_00AA05: TICK();
    SETLO(A, rd8(ABS(0x01E0)));  /* 00AA05 LDA */
    if ((A & 0xFF) == 0) goto L_00AA0E;  /* 00AA08 BEQ */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) | 0x02);  /* 00AA0A SEB */
L_00AA0E: TICK();
    SETLO(A, rd8(ABS(0x01E1)));  /* 00AA0E LDA */
    if ((A & 0xFF) == 0) goto L_00AA17;  /* 00AA11 BEQ */
    wr8(ABS(0x03CE), rd8(ABS(0x03CE)) | 0x01);  /* 00AA13 SEB */
L_00AA17: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00AA17 LDA */
    /* 00AA1A CMP */
    if ((A & 0xFF) == 0x54) goto L_00AA21;  /* 00AA1C BEQ */
    goto L_00AAA8;  /* 00AA1E JMP */
L_00AA21: TICK();
    mflag = 0;  /* 00AA21 CLM */
    A = rd16(ABS(0x03FB));  /* 00AA22 LDA */
    A = asl16(A);  /* 00AA25 ASL */
    A = asl16(A);  /* 00AA26 ASL */
    A = asl16(A);  /* 00AA27 ASL */
    C = 0;  /* 00AA28 CLC */
    A = adc16(A, 0x0004);  /* 00AA29 ADC */
    A = rla16(A, 0x0008);  /* 00AA2C RLA */
    A &= 0x00FF;  /* 00AA30 AND */
    mflag = 1;  /* 00AA33 SEM */
    wr8(ABS(0x03CA), (uint8_t)A);  /* 00AA34 STA */
    SETLO(A, rd8(ABS(0x03FB)));  /* 00AA37 LDA */
    SETLO(A, asl8(A));  /* 00AA3A ASL */
    SETLO(A, asl8(A));  /* 00AA3B ASL */
    SETLO(A, asl8(A));  /* 00AA3C ASL */
    C = 0;  /* 00AA3D CLC */
    SETLO(A, adc8(A, 0x04));  /* 00AA3E ADC */
    wr8(ABS(0x03CB), (uint8_t)A);  /* 00AA40 STA */
    Y = 0x0216;  /* 00AA43 LDY */
    mflag = 0;  /* 00AA46 CLM */
    S -= 3; F_007271();  /* 00AA47 JSRL */
    A = rla16(A, 0x0008);  /* 00AA4B RLA */
    A &= 0x00FF;  /* 00AA4F AND */
    mflag = 1;  /* 00AA52 SEM */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00AA53 STA */
    Y = 0x0216;  /* 00AA56 LDY */
    mflag = 0;  /* 00AA59 CLM */
    S -= 3; F_007271();  /* 00AA5A JSRL */
    mflag = 1;  /* 00AA5E SEM */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00AA5F STA */
    SETLO(A, rd8(ABS(0x03CD)));  /* 00AA62 LDA */
    mflag = 0;  /* 00AA65 CLM */
    A &= 0x00FF;  /* 00AA66 AND */
    A = rla16(A, 0x0008);  /* 00AA69 RLA */
    A &= 0xFF00;  /* 00AA6D AND */
    wr16(DP(0x02), A);  /* 00AA70 STA */
    A = rd16(ABS(0x03CE));  /* 00AA72 LDA */
    A &= 0x00FF;  /* 00AA75 AND */
    C = 0;  /* 00AA78 CLC */
    A = adc16(A, rd16(DP(0x02)));  /* 00AA79 ADC */
    wr16(DP(0x00), A);  /* 00AA7B STA */
    A = rla16(A, 0x0008);  /* 00AA7D RLA */
    A &= 0x00FF;  /* 00AA81 AND */
    A = lsr16(A);  /* 00AA84 LSR */
    A = lsr16(A);  /* 00AA85 LSR */
    A = lsr16(A);  /* 00AA86 LSR */
    A = lsr16(A);  /* 00AA87 LSR */
    A = lsr16(A);  /* 00AA88 LSR */
    mflag = 1;  /* 00AA89 SEM */
    SETLO(A, A | 0x54);  /* 00AA8A ORA */
    wr8(ABS(0x03CC), (uint8_t)A);  /* 00AA8C STA */
    mflag = 0;  /* 00AA8F CLM */
    A = rd16(DP(0x00));  /* 00AA90 LDA */
    A = asl16(A);  /* 00AA92 ASL */
    A = asl16(A);  /* 00AA93 ASL */
    A = asl16(A);  /* 00AA94 ASL */
    A = rla16(A, 0x0008);  /* 00AA95 RLA */
    A &= 0x00FF;  /* 00AA99 AND */
    mflag = 1;  /* 00AA9C SEM */
    wr8(ABS(0x03CD), (uint8_t)A);  /* 00AA9D STA */
    SETLO(A, rd8(DP(0x00)));  /* 00AAA0 LDA */
    SETLO(A, asl8(A));  /* 00AAA2 ASL */
    SETLO(A, asl8(A));  /* 00AAA3 ASL */
    SETLO(A, asl8(A));  /* 00AAA4 ASL */
    wr8(ABS(0x03CE), (uint8_t)A);  /* 00AAA5 STA */
L_00AAA8: TICK();
    mflag = 0;  /* 00AAA8 CLM */
    Y = pull16();  /* 00AAA9 PLY */
    D = pull16();  /* 00AAAA PLD */
    S += 3; return;  /* 00AAAB RTL */
}

/* 00AAAC  callers: 00AFF8, 00B946 */
void F_00AAAC(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x03BB));  /* 00AAAC LDA */
    mflag = 1;  /* 00AAAF SEM */
    /* 00AAB0 CMP */
    if ((A & 0xFF) != 0x22) goto L_00AABB;  /* 00AAB2 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00AAB4 LDA */
    /* 00AAB7 CMP */
    if ((A & 0xFF) == 0x40) goto L_00AAE9;  /* 00AAB9 BEQ */
L_00AABB: TICK();
    SETLO(A, rd8(ABS(0x03BB)));  /* 00AABB LDA */
    /* 00AABE CMP */
    if ((A & 0xFF) != 0x23) goto L_00AAC9;  /* 00AAC0 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00AAC2 LDA */
    /* 00AAC5 CMP */
    if ((A & 0xFF) != 0x50) goto L_00AAE9;  /* 00AAC7 BNE */
L_00AAC9: TICK();
    SETLO(A, rd8(ABS(0x03BB)));  /* 00AAC9 LDA */
    /* 00AACC CMP */
    if ((A & 0xFF) != 0x24) goto L_00AADB;  /* 00AACE BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00AAD0 LDA */
    /* 00AAD3 CMP */
    if ((A & 0xFF) == 0x50) goto L_00AADB;  /* 00AAD5 BEQ */
    /* 00AAD7 CMP */
    if ((A & 0xFF) != 0x54) goto L_00AAE9;  /* 00AAD9 BNE */
L_00AADB: TICK();
    SETLO(A, rd8(ABS(0x03BB)));  /* 00AADB LDA */
    /* 00AADE CMP */
    if ((A & 0xFF) != 0x25) goto L_00AAEC;  /* 00AAE0 BNE */
    SETLO(A, rd8(ABS(0x03B8)));  /* 00AAE2 LDA */
    /* 00AAE5 CMP */
    if ((A & 0xFF) != 0x54) goto L_00AAEC;  /* 00AAE7 BNE */
L_00AAE9: TICK();
    SETLO(A, 0x00);  /* 00AAE9 LDA */
    S += 3; return;  /* 00AAEB RTL */
L_00AAEC: TICK();
    SETLO(A, rd8(ABS(0x03B8)));  /* 00AAEC LDA */
    if ((A & 0xFF) == 0) goto L_00AAF8;  /* 00AAEF BEQ */
    mflag = 0;  /* 00AAF1 CLM */
    A = 0x0001;  /* 00AAF2 LDA */
    goto L_00AAFC;  /* 00AAF5 JMP */
L_00AAF8: TICK();
    mflag = 0;  /* 00AAF8 CLM */
    A = 0x0000;  /* 00AAF9 LDA */
L_00AAFC: TICK();
    mflag = 1;  /* 00AAFC SEM */
    S += 3; return;  /* 00AAFD RTL */
}

/* 00AAFE  callers: 00B012, 00BFD8, 00BFF6, 00C14F, 00C192, 00C2AC, 00C401 */
void F_00AAFE(void)
{
    uint32_t t; (void)t;
    S -= 3; F_00A53F();  /* 00AAFE JSRL */
    wr16(ABS(0x03B6), 0x0001);  /* 00AB02 LDM */
    mflag = 1;  /* 00AB07 SEM */
    wr8(ABS(0x03A4), 0x01);  /* 00AB08 LDM */
    mflag = 0;  /* 00AB0C CLM */
    S += 3; return;  /* 00AB0D RTL */
}

/* 00AB0E  callers: 00ACF0, 00AD3F, 00AD7D, 00AE11, 00B605 */
void F_00AB0E(void)
{
    uint32_t t; (void)t;
    A = rd16(ABS(0x03C9));  /* 00AB0E LDA */
    mflag = 1;  /* 00AB11 SEM */
    SETLO(A, A & 0x7F);  /* 00AB12 AND */
    S += 3; return;  /* 00AB14 RTL */
}

/* 00AB15  callers: 00AF1B, 00B532 */
void F_00AB15(void)
{
    uint32_t t; (void)t;
    X = 0x03C9;  /* 00AB15 LDX */
    mflag = 1;  /* 00AB18 SEM */
    SETLO(B, 0x00);  /* 00AB19 LDA */
    SETLO(A, rd8((ABS(0x0003) + X)));  /* 00AB1C LDA */
    div8(0x20);  /* 00AB1F DIV */
    SETLO(A, A & 0x03);  /* 00AB22 AND */
    S += 3; return;  /* 00AB24 RTL */
}

/* 00AB25  callers: 00AE63, 00B543, 00B553 */
void F_00AB25(void)
{
    uint32_t t; (void)t;
    X = 0x03C9;  /* 00AB25 LDX */
    mflag = 1;  /* 00AB28 SEM */
    SETLO(B, 0x00);  /* 00AB29 LDA */
    SETLO(A, rd8((ABS(0x0003) + X)));  /* 00AB2C LDA */
    div8(0x04);  /* 00AB2F DIV */
    SETLO(A, A & 0x07);  /* 00AB32 AND */
    S += 3; return;  /* 00AB34 RTL */
}

/* 00AB35  callers: 00AED3, 00AEDC, 00B573, 00BF00 */
void F_00AB35(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00AB35 LDA */
    push16(D);  /* 00AB37 PHD */
    D = A;  /* 00AB38 TAD */
    X = 0x03C9;  /* 00AB39 LDX */
    mflag = 1;  /* 00AB3C SEM */
    SETLO(B, 0x00);  /* 00AB3D LDA */
    SETLO(A, rd8((ABS(0x0004) + X)));  /* 00AB40 LDA */
    div8(0x80);  /* 00AB43 DIV */
    mflag = 0;  /* 00AB46 CLM */
    A &= 0x00FF;  /* 00AB47 AND */
    wr16(DP(0x00), A);  /* 00AB4A STA */
    A = rd16((ABS(0x0003) + X));  /* 00AB4C LDA */
    A &= 0x00FF;  /* 00AB4F AND */
    A = asl16(A);  /* 00AB52 ASL */
    C = 0;  /* 00AB53 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00AB54 ADC */
    wr16(DP(0x00), A);  /* 00AB56 STA */
    A = rd16((ABS(0x0002) + X));  /* 00AB58 LDA */
    mflag = 1;  /* 00AB5B SEM */
    SETLO(A, A & 0x01);  /* 00AB5C AND */
    mflag = 0;  /* 00AB5E CLM */
    A &= 0x00FF;  /* 00AB5F AND */
    A = rla16(A, 0x0009);  /* 00AB62 RLA */
    A &= 0xFE00;  /* 00AB66 AND */
    C = 0;  /* 00AB69 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00AB6A ADC */
    D = pull16();  /* 00AB6C PLD */
    S += 3; return;  /* 00AB6D RTL */
}

/* 00AB6E  callers: 00AE74, 00B5B2, 00B5DB, 00B619 */
void F_00AB6E(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00AB6E LDA */
    push16(D);  /* 00AB70 PHD */
    D = A;  /* 00AB71 TAD */
    X = 0x03C9;  /* 00AB72 LDX */
    A = rd16((ABS(0x0002) + X));  /* 00AB75 LDA */
    mflag = 1;  /* 00AB78 SEM */
    SETLO(A, A & 0x04);  /* 00AB79 AND */
    if ((A & 0xFF) != 0) goto L_00AB9C;  /* 00AB7B BNE */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00AB7D LDA */
    mflag = 0;  /* 00AB80 CLM */
    A &= 0x00FF;  /* 00AB81 AND */
    wr16(DP(0x00), A);  /* 00AB84 STA */
    A = rd16((ABS(0x0004) + X));  /* 00AB86 LDA */
    A &= 0x00FF;  /* 00AB89 AND */
    A = rla16(A, 0x0008);  /* 00AB8C RLA */
    A &= 0xFF00;  /* 00AB90 AND */
    C = 0;  /* 00AB93 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00AB94 ADC */
    A = asl16(A);  /* 00AB96 ASL */
    A = lsr16(A);  /* 00AB97 LSR */
    A = lsr16(A);  /* 00AB98 LSR */
    A = lsr16(A);  /* 00AB99 LSR */
    D = pull16();  /* 00AB9A PLD */
    S += 3; return;  /* 00AB9B RTL */
L_00AB9C: TICK();
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00AB9C LDA */
    SETLO(A, A & 0xE0);  /* 00AB9F AND */
    SETLO(B, 0x00);  /* 00ABA1 LDA */
    div8(0x20);  /* 00ABA4 DIV */
    mflag = 0;  /* 00ABA7 CLM */
    A &= 0x00FF;  /* 00ABA8 AND */
    wr16(DP(0x00), A);  /* 00ABAB STA */
    A = rd16((ABS(0x0004) + X));  /* 00ABAD LDA */
    A &= 0x00FF;  /* 00ABB0 AND */
    A = asl16(A);  /* 00ABB3 ASL */
    A = asl16(A);  /* 00ABB4 ASL */
    A = asl16(A);  /* 00ABB5 ASL */
    C = 0;  /* 00ABB6 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00ABB7 ADC */
    wr16(DP(0x00), A);  /* 00ABB9 STA */
    A = rd16((ABS(0x0003) + X));  /* 00ABBB LDA */
    mflag = 1;  /* 00ABBE SEM */
    SETLO(A, A & 0x03);  /* 00ABBF AND */
    mflag = 0;  /* 00ABC1 CLM */
    A &= 0x00FF;  /* 00ABC2 AND */
    A &= 0x001F;  /* 00ABC5 AND */
    A = lsr16(A);  /* 00ABC8 LSR */
    A = ror16(A);  /* 00ABC9 ROR */
    A = ror16(A);  /* 00ABCA ROR */
    A = ror16(A);  /* 00ABCB ROR */
    A = ror16(A);  /* 00ABCC ROR */
    A = ror16(A);  /* 00ABCD ROR */
    C = 0;  /* 00ABCE CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00ABCF ADC */
    D = pull16();  /* 00ABD1 PLD */
    S += 3; return;  /* 00ABD2 RTL */
}

/* 00ABD3  callers: 00AE97, 00AFA4, 00B0F1 */
void F_00ABD3(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00ABD3 LDA */
    push16(D);  /* 00ABD5 PHD */
    D = A;  /* 00ABD6 TAD */
    mflag = 1;  /* 00ABD7 SEM */
    SETLO(A, rd8(ABS(0x0438)));  /* 00ABD8 LDA */
    if ((A & 0xFF) != 0) goto L_00AC35;  /* 00ABDB BNE */
    X = 0x03C9;  /* 00ABDD LDX */
    SETLO(A, rd8((ABS(0x0002) + X)));  /* 00ABE0 LDA */
    SETLO(A, A & 0x04);  /* 00ABE3 AND */
    if ((A & 0xFF) != 0) goto L_00ABF8;  /* 00ABE5 BNE */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00ABE7 LDA */
    SETLO(A, A & 0x03);  /* 00ABEA AND */
    mflag = 0;  /* 00ABEC CLM */
    A &= 0x00FF;  /* 00ABED AND */
    X = A;  /* 00ABF0 TAX */
    A = rd16((0x002DDA + X));  /* 00ABF1 LDA */
    goto L_00AC0B;  /* 00ABF5 JMP */
L_00ABF8: TICK();
    X = 0x03C9;  /* 00ABF8 LDX */
    mflag = 0;  /* 00ABFB CLM */
    A = rd16((ABS(0x0005) + X));  /* 00ABFC LDA */
    mflag = 1;  /* 00ABFF SEM */
    SETLO(A, A & 0x0F);  /* 00AC00 AND */
    mflag = 0;  /* 00AC02 CLM */
    A &= 0x00FF;  /* 00AC03 AND */
    X = A;  /* 00AC06 TAX */
    A = rd16((0x002DCA + X));  /* 00AC07 LDA */
L_00AC0B: TICK();
    mflag = 1;  /* 00AC0B SEM */
    wr8(ABS(0x0438), (uint8_t)A);  /* 00AC0C STA */
    /* 00AC0F CMP */
    if ((A & 0xFF) < 0x02) goto L_00AC2F;  /* 00AC11 BCC */
    mflag = 0;  /* 00AC13 CLM */
    A &= 0x00FF;  /* 00AC14 AND */
    A -= 1;  /* 00AC17 DEC */
    wr16(DP(0x00), A);  /* 00AC18 STA */
    A = rd16(ABS(TB2));  /* 00AC1A LDA */
    A &= 0x001F;  /* 00AC1D AND */
    B = 0x0000;  /* 00AC20 LDA */
    div16(rd16(DP(0x00)));  /* 00AC24 DIV */
    push16(B);  /* 00AC27 PHB */
    A = pull16();  /* 00AC29 PLA */
    mflag = 1;  /* 00AC2A SEM */
    SETLO(A, A + 1);  /* 00AC2B INC */
    goto L_00AC32;  /* 00AC2C JMP */
L_00AC2F: TICK();
    SETLO(A, rd8(ABS(0x0438)));  /* 00AC2F LDA */
L_00AC32: TICK();
    wr8(ABS(0x0437), (uint8_t)A);  /* 00AC32 STA */
L_00AC35: TICK();
    mflag = 0;  /* 00AC35 CLM */
    D = pull16();  /* 00AC36 PLD */
    S += 3; return;  /* 00AC37 RTL */
}

/* 00AC38  callers: 00B10D, 00B12B, 00B158, 00B176, 00B18C */
void F_00AC38(void)
{
    uint32_t t; (void)t;
    X = 0x03C9;  /* 00AC38 LDX */
    A = rd16((ABS(0x0005) + X));  /* 00AC3B LDA */
    mflag = 1;  /* 00AC3E SEM */
    SETLO(A, A & 0x10);  /* 00AC3F AND */
    /* 00AC41 CMP */
    if ((A & 0xFF) != 0x10) goto L_00AC4C;  /* 00AC43 BNE */
    mflag = 0;  /* 00AC45 CLM */
    A = 0x0001;  /* 00AC46 LDA */
    goto L_00AC50;  /* 00AC49 JMP */
L_00AC4C: TICK();
    mflag = 0;  /* 00AC4C CLM */
    A = 0x0000;  /* 00AC4D LDA */
L_00AC50: TICK();
    mflag = 1;  /* 00AC50 SEM */
    S += 3; return;  /* 00AC51 RTL */
}

/* 00AC52  callers: 00BE7C, 00C9E7 */
void F_00AC52(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00AC52 LDA */
    push16(D);  /* 00AC54 PHD */
    D = A;  /* 00AC55 TAD */
    X = 0x03C9;  /* 00AC56 LDX */
    mflag = 1;  /* 00AC59 SEM */
    SETLO(B, 0x00);  /* 00AC5A LDA */
    SETLO(A, rd8((ABS(0x0001) + X)));  /* 00AC5D LDA */
    div8(0x20);  /* 00AC60 DIV */
    mflag = 0;  /* 00AC63 CLM */
    A &= 0x00FF;  /* 00AC64 AND */
    wr16(DP(0x00), A);  /* 00AC67 STA */
    A = rd16(ABS(0x03C9));  /* 00AC69 LDA */
    mflag = 1;  /* 00AC6C SEM */
    SETLO(A, A & 0x7F);  /* 00AC6D AND */
    mflag = 0;  /* 00AC6F CLM */
    A &= 0x00FF;  /* 00AC70 AND */
    A = asl16(A);  /* 00AC73 ASL */
    A = asl16(A);  /* 00AC74 ASL */
    A = asl16(A);  /* 00AC75 ASL */
    C = 0;  /* 00AC76 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00AC77 ADC */
    D = pull16();  /* 00AC79 PLD */
    S += 3; return;  /* 00AC7A RTL */
}

/* 00AC7B  callers: 00B8C3, 00B8DB */
void F_00AC7B(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00AC7B LDA */
    push16(D);  /* 00AC7D PHD */
    D = A;  /* 00AC7E TAD */
    X = 0x03C9;  /* 00AC7F LDX */
    mflag = 1;  /* 00AC82 SEM */
    SETLO(B, 0x00);  /* 00AC83 LDA */
    SETLO(A, rd8((ABS(0x0002) + X)));  /* 00AC86 LDA */
    div8(0x08);  /* 00AC89 DIV */
    mflag = 0;  /* 00AC8C CLM */
    A &= 0x00FF;  /* 00AC8D AND */
    wr16(DP(0x00), A);  /* 00AC90 STA */
    A = rd16((ABS(0x0001) + X));  /* 00AC92 LDA */
    mflag = 1;  /* 00AC95 SEM */
    SETLO(A, A & 0x1F);  /* 00AC96 AND */
    mflag = 0;  /* 00AC98 CLM */
    A &= 0x00FF;  /* 00AC99 AND */
    A = asl16(A);  /* 00AC9C ASL */
    A = asl16(A);  /* 00AC9D ASL */
    A = asl16(A);  /* 00AC9E ASL */
    A = asl16(A);  /* 00AC9F ASL */
    A = asl16(A);  /* 00ACA0 ASL */
    C = 0;  /* 00ACA1 CLC */
    A = adc16(A, rd16(DP(0x00)));  /* 00ACA2 ADC */
    D = pull16();  /* 00ACA4 PLD */
    S += 3; return;  /* 00ACA5 RTL */
}

/* 00ACA6  callers: 00AEA3, 00B380 */
void F_00ACA6(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00ACA6 LDA */
    push16(D);  /* 00ACA8 PHD */
    D = A;  /* 00ACA9 TAD */
    wr16(DP(0x00), Y);  /* 00ACAA STY */
    /* 00ACAC CPY */
    if (Y != 0x1FFF) goto L_00ACBA;  /* 00ACAF BNE */
    mflag = 1;  /* 00ACB1 SEM */
    wr8(ABS(0x03AD), 0x01);  /* 00ACB2 LDM */
    SETLO(A, 0x01);  /* 00ACB6 LDA */
    D = pull16();  /* 00ACB8 PLD */
    S += 3; return;  /* 00ACB9 RTL */
L_00ACBA: TICK();
    mflag = 1;  /* 00ACBA SEM */
    if ((rd8(ABS(0x0335)) & 0x02) == 0x02) goto L_00AD12;  /* 00ACBB BBS */
    mflag = 0;  /* 00ACC0 CLM */
    if ((rd16(ABS(0x039E)) & 0x4000) == 0) goto L_00AD0F;  /* 00ACC1 BBC */
    A = rd16(ABS(0x0206));  /* 00ACC7 LDA */
    mflag = 1;  /* 00ACCA SEM */
    /* 00ACCB CMP */
    if ((A & 0xFF) >= 0x0B) goto L_00AD04;  /* 00ACCD BCS */
    SETLO(A, rd8(ABS(0x0206)));  /* 00ACCF LDA */
    mflag = 0;  /* 00ACD2 CLM */
    A &= 0x00FF;  /* 00ACD3 AND */
    A = asl16(A);  /* 00ACD6 ASL */
    A -= 1;  /* 00ACD7 DEC */
    A -= 1;  /* 00ACD8 DEC */
    X = A;  /* 00ACD9 TAX */
    A = rd16((ABS(0x03D9) + X));  /* 00ACDA LDA */
    t = rd16(DP(0x00));  /* 00ACDD CMP */
    if (A != t) goto L_00ACFD;  /* 00ACDF BNE */
    A = rd16(ABS(0x0206));  /* 00ACE1 LDA */
    A &= 0x00FF;  /* 00ACE4 AND */
    A -= 1;  /* 00ACE7 DEC */
    X = A;  /* 00ACE8 TAX */
    A = rd16((ABS(0x03BC) + X));  /* 00ACE9 LDA */
    mflag = 1;  /* 00ACEC SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 00ACED STA */
    mflag = 0;  /* 00ACEF CLM */
    S -= 3; F_00AB0E();  /* 00ACF0 JSRL */
    t = rd8(DP(0x04));  /* 00ACF4 CMP */
    if ((A & 0xFF) != t) goto L_00ACFD;  /* 00ACF6 BNE */
    SETLO(A, 0x01);  /* 00ACF8 LDA */
    goto L_00AD01;  /* 00ACFA JMP */
L_00ACFD: TICK();
    mflag = 0;  /* 00ACFD CLM */
    A = 0x0000;  /* 00ACFE LDA */
L_00AD01: TICK();
    mflag = 1;  /* 00AD01 SEM */
    D = pull16();  /* 00AD02 PLD */
    S += 3; return;  /* 00AD03 RTL */
L_00AD04: TICK();
    SETLO(A, rd8(ABS(0x0206)));  /* 00AD04 LDA */
    /* 00AD07 CMP */
    if ((A & 0xFF) < 0x0C) goto L_00AD0F;  /* 00AD09 BCC */
    SETLO(A, 0x00);  /* 00AD0B LDA */
    D = pull16();  /* 00AD0D PLD */
    S += 3; return;  /* 00AD0E RTL */
L_00AD0F: TICK();
    goto L_00AD5E;  /* 00AD0F JMP */
L_00AD12: TICK();
    mflag = 0;  /* 00AD12 CLM */
    if ((rd16(ABS(0x039E)) & 0x4000) == 0) goto L_00AD5E;  /* 00AD13 BBC */
    A = rd16(ABS(0x01C1));  /* 00AD19 LDA */
    mflag = 1;  /* 00AD1C SEM */
    /* 00AD1D CMP */
    if ((A & 0xFF) >= 0x0B) goto L_00AD53;  /* 00AD1F BCS */
    SETLO(A, rd8(ABS(0x01C1)));  /* 00AD21 LDA */
    mflag = 0;  /* 00AD24 CLM */
    A &= 0x00FF;  /* 00AD25 AND */
    A = asl16(A);  /* 00AD28 ASL */
    X = A;  /* 00AD29 TAX */
    A = rd16((ABS(0x03D9) + X));  /* 00AD2A LDA */
    t = rd16(DP(0x00));  /* 00AD2D CMP */
    if (A != t) goto L_00AD4C;  /* 00AD2F BNE */
    A = rd16(ABS(0x01C1));  /* 00AD31 LDA */
    A &= 0x00FF;  /* 00AD34 AND */
    X = A;  /* 00AD37 TAX */
    A = rd16((ABS(0x03BC) + X));  /* 00AD38 LDA */
    mflag = 1;  /* 00AD3B SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 00AD3C STA */
    mflag = 0;  /* 00AD3E CLM */
    S -= 3; F_00AB0E();  /* 00AD3F JSRL */
    t = rd8(DP(0x04));  /* 00AD43 CMP */
    if ((A & 0xFF) != t) goto L_00AD4C;  /* 00AD45 BNE */
    SETLO(A, 0x01);  /* 00AD47 LDA */
    goto L_00AD50;  /* 00AD49 JMP */
L_00AD4C: TICK();
    mflag = 0;  /* 00AD4C CLM */
    A = 0x0000;  /* 00AD4D LDA */
L_00AD50: TICK();
    mflag = 1;  /* 00AD50 SEM */
    D = pull16();  /* 00AD51 PLD */
    S += 3; return;  /* 00AD52 RTL */
L_00AD53: TICK();
    SETLO(A, rd8(ABS(0x01C1)));  /* 00AD53 LDA */
    /* 00AD56 CMP */
    if ((A & 0xFF) != 0x0C) goto L_00AD5E;  /* 00AD58 BNE */
    SETLO(A, 0x00);  /* 00AD5A LDA */
    D = pull16();  /* 00AD5C PLD */
    S += 3; return;  /* 00AD5D RTL */
L_00AD5E: TICK();
    mflag = 0;  /* 00AD5E CLM */
    wr16(DP(0x02), 0x0000);  /* 00AD5F LDM */
L_00AD63: TICK();
    X = rd16(DP(0x02));  /* 00AD63 LDX */
    /* 00AD65 CPX */
    if (X >= 0x000A) goto L_00AD8F;  /* 00AD68 BCS */
    A = X;  /* 00AD6A TXA */
    A = asl16(A);  /* 00AD6B ASL */
    X = A;  /* 00AD6C TAX */
    A = rd16((ABS(0x03D9) + X));  /* 00AD6D LDA */
    t = rd16(DP(0x00));  /* 00AD70 CMP */
    if (A != t) goto L_00AD89;  /* 00AD72 BNE */
    X = rd16(DP(0x02));  /* 00AD74 LDX */
    A = rd16((ABS(0x03BC) + X));  /* 00AD76 LDA */
    mflag = 1;  /* 00AD79 SEM */
    wr8(DP(0x04), (uint8_t)A);  /* 00AD7A STA */
    mflag = 0;  /* 00AD7C CLM */
    S -= 3; F_00AB0E();  /* 00AD7D JSRL */
    t = rd8(DP(0x04));  /* 00AD81 CMP */
    if ((A & 0xFF) != t) goto L_00AD89;  /* 00AD83 BNE */
    SETLO(A, 0x01);  /* 00AD85 LDA */
    D = pull16();  /* 00AD87 PLD */
    S += 3; return;  /* 00AD88 RTL */
L_00AD89: TICK();
    mflag = 0;  /* 00AD89 CLM */
    t = (rd16(DP(0x02)) + 1) & 0xFFFF; wr16(DP(0x02), t);  /* 00AD8A INC */
    goto L_00AD63;  /* 00AD8C JMP */
L_00AD8F: TICK();
    mflag = 0;  /* 00AD8F CLM */
    A = 0x0000;  /* 00AD90 LDA */
    mflag = 1;  /* 00AD93 SEM */
    D = pull16();  /* 00AD94 PLD */
    S += 3; return;  /* 00AD95 RTL */
}

/* 00AD96  callers: 00BE61, 00C97E */
void F_00AD96(void)
{
    uint32_t t; (void)t;
    A = rd16(SR(0x04));  /* 00AD96 LDA */
    push16(D);  /* 00AD98 PHD */
    D = A;  /* 00AD99 TAD */
    C = 0;  /* 00AD9A CLC */
    A = adc16(A, 0x000F);  /* 00AD9B ADC */
    push16(A);  /* 00AD9E PHA */
    mflag = 1;  /* 00AD9F SEM */
    wr8(ABS(0x03A9), 0x00);  /* 00ADA0 LDM */
    mflag = 0;  /* 00ADA4 CLM */
    S -= 3; F_00A0ED();  /* 00ADA5 JSRL */
    t = rd16(ABS(0x03FD));  /* 00ADA9 CMP */
    if (A != t) goto L_00ADB5;  /* 00ADAC BNE */
    S -= 3; F_00A4D6();  /* 00ADAE JSRL */
    goto L_00ADBF;  /* 00ADB2 JMP */
L_00ADB5: TICK();
    mflag = 1;  /* 00ADB5 SEM */
    if ((rd8(ABS(0x03C9)) & 0x80) == 0x80) goto L_00ADBF;  /* 00ADB6 BBS */
    mflag = 0;  /* 00ADBB CLM */
    Y = pull16();  /* 00ADBC PLY */
    D = pull16();  /* 00ADBD PLD */
    S += 3; return;  /* 00ADBE RTL */
L_00ADBF: TICK();
    mflag = 0;  /* 00ADBF CLM */
    if ((rd16(ABS(0x0443)) & 0x0002) == 0x0002) goto L_00ADC9;  /* 00ADC0 BBS */
    Y = pull16();  /* 00ADC6 PLY */
    D = pull16();  /* 00ADC7 PLD */
    S += 3; return;  /* 00ADC8 RTL */
L_00ADC9: TICK();
    A = rd16(ABS(0x03B9));  /* 00ADC9 LDA */
    mflag = 1;  /* 00ADCC SEM */
    /* 00ADCD CMP */
    if ((A & 0xFF) >= 0x09) goto L_00ADE4;  /* 00ADCF BCS */
    X = 0x03C9;  /* 00ADD1 LDX */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00ADD4 LDA */
    SETLO(A, A & 0x01);  /* 00ADD7 AND */
    if ((A & 0xFF) == 0) goto L_00ADE4;  /* 00ADD9 BEQ */
    SETLO(A, rd8(ABS(0x0433)));  /* 00ADDB LDA */
    if ((A & 0xFF) != 0) goto L_00ADE4;  /* 00ADDE BNE */
    mflag = 0;  /* 00ADE0 CLM */
    Y = pull16();  /* 00ADE1 PLY */
    D = pull16();  /* 00ADE2 PLD */
    S += 3; return;  /* 00ADE3 RTL */
L_00ADE4: TICK();
    wr8(ABS(0x03B9), 0x10);  /* 00ADE4 LDM */
    X = 0x03C9;  /* 00ADE8 LDX */
    SETLO(A, rd8((ABS(0x0002) + X)));  /* 00ADEB LDA */
    mflag = 0;  /* 00ADEE CLM */
    A &= 0x00FF;  /* 00ADEF AND */
    wr16(DP(0x0F), A);  /* 00ADF2 STA */
    A = rd16((ABS(0x0001) + X));  /* 00ADF4 LDA */
    A &= 0x00FF;  /* 00ADF7 AND */
    A = rla16(A, 0x0008);  /* 00ADFA RLA */
    A &= 0xFF00;  /* 00ADFE AND */
    C = 0;  /* 00AE01 CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 00AE02 ADC */
    A = lsr16(A);  /* 00AE04 LSR */
    A = lsr16(A);  /* 00AE05 LSR */
    A = lsr16(A);  /* 00AE06 LSR */
    wr16(DP(0x00), A);  /* 00AE07 STA */
    mflag = 1;  /* 00AE09 SEM */
    wr8(DP(0x0D), 0x00);  /* 00AE0A LDM */
    wr8(DP(0x0C), 0x00);  /* 00AE0D LDM */
    mflag = 0;  /* 00AE10 CLM */
    S -= 3; F_00AB0E();  /* 00AE11 JSRL */
    wr8(DP(0x09), (uint8_t)A);  /* 00AE15 STA */
    t = rd8(ABS(0x0425));  /* 00AE17 CMP */
    if ((A & 0xFF) != t) goto L_00AE2E;  /* 00AE1A BNE */
    wr8(DP(0x0A), 0x01);  /* 00AE1C LDM */
    mflag = 0;  /* 00AE1F CLM */
    X = rd16(DP(0x00));  /* 00AE20 LDX */
    t = rd16(ABS(0x03FB));  /* 00AE22 CPX */
    if (X != t) goto L_00AE2B;  /* 00AE25 BNE */
    mflag = 1;  /* 00AE27 SEM */
    wr8(DP(0x0C), 0x01);  /* 00AE28 LDM */
L_00AE2B: TICK();
    goto L_00AE32;  /* 00AE2B JMP */
L_00AE2E: TICK();
    mflag = 1;  /* 00AE2E SEM */
    wr8(DP(0x0A), 0x00);  /* 00AE2F LDM */
L_00AE32: TICK();
    X = 0x03C9;  /* 00AE32 LDX */
    mflag = 0;  /* 00AE35 CLM */
    A = rd16((ABS(0x0004) + X));  /* 00AE36 LDA */
    A &= 0x00FF;  /* 00AE39 AND */
    wr16(DP(0x0F), A);  /* 00AE3C STA */
    A = rd16((ABS(0x0003) + X));  /* 00AE3E LDA */
    mflag = 1;  /* 00AE41 SEM */
    SETLO(A, A & 0x03);  /* 00AE42 AND */
    mflag = 0;  /* 00AE44 CLM */
    A &= 0x00FF;  /* 00AE45 AND */
    A = rla16(A, 0x0008);  /* 00AE48 RLA */
    A &= 0xFF00;  /* 00AE4C AND */
    C = 0;  /* 00AE4F CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 00AE50 ADC */
    wr16(DP(0x06), A);  /* 00AE52 STA */
    A = rd16((ABS(0x0005) + X));  /* 00AE54 LDA */
    A &= 0x00FF;  /* 00AE57 AND */
    A = lsr16(A);  /* 00AE5A LSR */
    A = lsr16(A);  /* 00AE5B LSR */
    A = lsr16(A);  /* 00AE5C LSR */
    A = lsr16(A);  /* 00AE5D LSR */
    A = lsr16(A);  /* 00AE5E LSR */
    mflag = 1;  /* 00AE5F SEM */
    wr8(DP(0x08), (uint8_t)A);  /* 00AE60 STA */
    mflag = 0;  /* 00AE62 CLM */
    S -= 3; F_00AB25();  /* 00AE63 JSRL */
    wr8(DP(0x0B), (uint8_t)A);  /* 00AE67 STA */
    X = 0x03C9;  /* 00AE69 LDX */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00AE6C LDA */
    SETLO(A, A & 0x07);  /* 00AE6F AND */
    wr8(DP(0x0E), (uint8_t)A);  /* 00AE71 STA */
    mflag = 0;  /* 00AE73 CLM */
    S -= 3; F_00AB6E();  /* 00AE74 JSRL */
    wr16(DP(0x02), A);  /* 00AE78 STA */
    t = rd16(ABS(0x03FB));  /* 00AE7A CMP */
    if (A != t) goto L_00AE83;  /* 00AE7D BNE */
    mflag = 1;  /* 00AE7F SEM */
    wr8(DP(0x0D), 0x01);  /* 00AE80 LDM */
L_00AE83: TICK();
    mflag = 1;  /* 00AE83 SEM */
    if ((rd8(ABS(0x03C9)) & 0x80) == 0x80) goto L_00AE8C;  /* 00AE84 BBS */
    goto L_00B55F;  /* 00AE89 JMP */
L_00AE8C: TICK();
    X = 0x03C9;  /* 00AE8C LDX */
    SETLO(A, rd8((ABS(0x0002) + X)));  /* 00AE8F LDA */
    SETLO(A, A & 0x04);  /* 00AE92 AND */
    if ((A & 0xFF) != 0) goto L_00AEEF;  /* 00AE94 BNE */
    mflag = 0;  /* 00AE96 CLM */
    S -= 3; F_00ABD3();  /* 00AE97 JSRL */
    mflag = 1;  /* 00AE9B SEM */
    wr8(ABS(0x03B9), 0x43);  /* 00AE9C LDM */
    Y = rd16(DP(0x00));  /* 00AEA0 LDY */
    mflag = 0;  /* 00AEA2 CLM */
    S -= 3; F_00ACA6();  /* 00AEA3 JSRL */
    /* 00AEA7 CMP */
    if ((A & 0xFF) == 0x00) goto L_00AEAF;  /* 00AEA9 BEQ */
    wr8(ABS(0x03B9), 0x42);  /* 00AEAB LDM */
L_00AEAF: TICK();
    SETLO(A, rd8(DP(0x0A)));  /* 00AEAF LDA */
    if ((A & 0xFF) == 0) goto L_00AECB;  /* 00AEB1 BEQ */
    SETLO(A, rd8(DP(0x0D)));  /* 00AEB3 LDA */
    if ((A & 0xFF) == 0) goto L_00AEC2;  /* 00AEB5 BEQ */
    wr8(ABS(0x03B9), 0x40);  /* 00AEB7 LDM */
    wr8(ABS(0x03AD), 0x00);  /* 00AEBB LDM */
    goto L_00AECB;  /* 00AEBF JMP */
L_00AEC2: TICK();
    mflag = 1;  /* 00AEC2 SEM */
    SETLO(A, rd8(DP(0x0C)));  /* 00AEC3 LDA */
    if ((A & 0xFF) == 0) goto L_00AECB;  /* 00AEC5 BEQ */
    wr8(ABS(0x03B9), 0x41);  /* 00AEC7 LDM */
L_00AECB: TICK();
    SETLO(A, rd8(ABS(0x03B9)));  /* 00AECB LDA */
    /* 00AECE CMP */
    if ((A & 0xFF) == 0x43) goto L_00AEEC;  /* 00AED0 BEQ */
    mflag = 0;  /* 00AED2 CLM */
    S -= 3; F_00AB35();  /* 00AED3 JSRL */
    t = rd16(ABS(0x03EF));  /* 00AED7 CMP */
    if (A < t) goto L_00AEE7;  /* 00AEDA BCC */
    S -= 3; F_00AB35();  /* 00AEDC JSRL */
    t = rd16(ABS(0x03ED));  /* 00AEE0 CMP */
    C = (A >= t);
    Z = (A == t);
    if (!C) goto L_00AEEC;  /* 00AEE3 BCC */
    if (Z) goto L_00AEEC;  /* 00AEE5 BEQ */
L_00AEE7: TICK();
    mflag = 1;  /* 00AEE7 SEM */
    wr8(ABS(0x03B9), 0x43);  /* 00AEE8 LDM */
L_00AEEC: TICK();
    goto L_00B55F;  /* 00AEEC JMP */
L_00AEEF: TICK();
    X = 0x03C9;  /* 00AEEF LDX */
    mflag = 0;  /* 00AEF2 CLM */
    A = rd16((ABS(0x0003) + X));  /* 00AEF3 LDA */
    A &= 0x00FF;  /* 00AEF6 AND */
    wr16(DP(0x0F), A);  /* 00AEF9 STA */
    A = rd16((ABS(0x0002) + X));  /* 00AEFB LDA */
    A &= 0x00FF;  /* 00AEFE AND */
    A = rla16(A, 0x0008);  /* 00AF01 RLA */
    A &= 0xFF00;  /* 00AF05 AND */
    C = 0;  /* 00AF08 CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 00AF09 ADC */
    A = asl16(A);  /* 00AF0B ASL */
    A = rla16(A, 0x0008);  /* 00AF0C RLA */
    A &= 0x00FF;  /* 00AF10 AND */
    A &= 0x0007;  /* 00AF13 AND */
    if (A == 0) goto L_00AF1B;  /* 00AF16 BEQ */
    goto L_00B506;  /* 00AF18 JMP */
L_00AF1B: TICK();
    S -= 3; F_00AB15();  /* 00AF1B JSRL */
    /* 00AF1F CMP */
    if ((A & 0xFF) == 0x00) goto L_00AF26;  /* 00AF21 BEQ */
    goto L_00B0E8;  /* 00AF23 JMP */
L_00AF26: TICK();
    mflag = 0;  /* 00AF26 CLM */
    A = rd16(ABS(0x03F1));  /* 00AF27 LDA */
    A &= 0x000F;  /* 00AF2A AND */
    X = 0x03C9;  /* 00AF2D LDX */
    wr16(DP(0x0F), A);  /* 00AF30 STA */
    A = rd16((ABS(0x0004) + X));  /* 00AF32 LDA */
    mflag = 1;  /* 00AF35 SEM */
    SETLO(A, A & 0xC0);  /* 00AF36 AND */
    mflag = 0;  /* 00AF38 CLM */
    A &= 0x00FF;  /* 00AF39 AND */
    A = lsr16(A);  /* 00AF3C LSR */
    A = lsr16(A);  /* 00AF3D LSR */
    A = lsr16(A);  /* 00AF3E LSR */
    A = lsr16(A);  /* 00AF3F LSR */
    A = lsr16(A);  /* 00AF40 LSR */
    A = lsr16(A);  /* 00AF41 LSR */
    A &= 0x00FF;  /* 00AF42 AND */
    wr16(DP(0x11), A);  /* 00AF45 STA */
    A = rd16((ABS(0x0003) + X));  /* 00AF47 LDA */
    mflag = 1;  /* 00AF4A SEM */
    SETLO(A, A & 0x03);  /* 00AF4B AND */
    SETLO(A, asl8(A));  /* 00AF4D ASL */
    SETLO(A, asl8(A));  /* 00AF4E ASL */
    mflag = 0;  /* 00AF4F CLM */
    A &= 0x00FF;  /* 00AF50 AND */
    C = 0;  /* 00AF53 CLC */
    A = adc16(A, rd16(DP(0x11)));  /* 00AF54 ADC */
    t = rd16(DP(0x0F));  /* 00AF56 CMP */
    if (A == t) goto L_00AF5D;  /* 00AF58 BEQ */
    goto L_00B0DA;  /* 00AF5A JMP */
L_00AF5D: TICK();
    A = rd16(DP(0x0B));  /* 00AF5D LDA */
    C = 0;  /* 00AF5F CLC */
    mflag = 1;  /* 00AF60 SEM */
    SETLO(A, adc8(A, 0x20));  /* 00AF61 ADC */
    wr8(ABS(0x03BB), (uint8_t)A);  /* 00AF63 STA */
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00AF66 STA */
    /* 00AF69 CMP */
    if ((A & 0xFF) != 0x26) goto L_00AF8F;  /* 00AF6B BNE */
    SETLO(A, rd8(ABS(0x03AF)));  /* 00AF6D LDA */
    if ((A & 0xFF) != 0) goto L_00AF7A;  /* 00AF70 BNE */
    wr8(ABS(0x03AF), 0x01);  /* 00AF72 LDM */
    wr8(ABS(0x01BE), 0x0B);  /* 00AF76 LDM */
L_00AF7A: TICK();
    SETLO(A, rd8(ABS(0x03A5)));  /* 00AF7A LDA */
    if ((A & 0xFF) != 0) goto L_00AF8C;  /* 00AF7D BNE */
    mflag = 0;  /* 00AF7F CLM */
    X = rd16(ABS(0x03B6));  /* 00AF80 LDX */
    /* 00AF83 CPX */
    if (X == 0x0002) goto L_00AF8C;  /* 00AF86 BEQ */
    S -= 3; F_00A399();  /* 00AF88 JSRL */
L_00AF8C: TICK();
    goto L_00AFA3;  /* 00AF8C JMP */
L_00AF8F: TICK();
    mflag = 1;  /* 00AF8F SEM */
    SETLO(A, rd8(ABS(0x03A5)));  /* 00AF90 LDA */
    if ((A & 0xFF) == 0) goto L_00AF9A;  /* 00AF93 BEQ */
    mflag = 0;  /* 00AF95 CLM */
    S -= 3; F_00A3A8();  /* 00AF96 JSRL */
L_00AF9A: TICK();
    mflag = 1;  /* 00AF9A SEM */
    wr8(ABS(0x03A5), 0x00);  /* 00AF9B LDM */
    wr8(ABS(0x03AF), 0x00);  /* 00AF9F LDM */
L_00AFA3: TICK();
    mflag = 0;  /* 00AFA3 CLM */
    S -= 3; F_00ABD3();  /* 00AFA4 JSRL */
    X = 0x03C9;  /* 00AFA8 LDX */
    A = rd16((ABS(0x0004) + X));  /* 00AFAB LDA */
    mflag = 1;  /* 00AFAE SEM */
    SETLO(A, A & 0x38);  /* 00AFAF AND */
    mflag = 0;  /* 00AFB1 CLM */
    A &= 0x00FF;  /* 00AFB2 AND */
    A = lsr16(A);  /* 00AFB5 LSR */
    A = lsr16(A);  /* 00AFB6 LSR */
    A = lsr16(A);  /* 00AFB7 LSR */
    A &= 0x00FF;  /* 00AFB8 AND */
    X = A;  /* 00AFBB TAX */
    A = rd16((0x002DDE + X));  /* 00AFBC LDA */
    mflag = 1;  /* 00AFC0 SEM */
    wr8(ABS(0x0436), (uint8_t)A);  /* 00AFC1 STA */
    X = 0x03C9;  /* 00AFC4 LDX */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00AFC7 LDA */
    mflag = 0;  /* 00AFCA CLM */
    A &= 0x00FF;  /* 00AFCB AND */
    wr16(DP(0x0F), A);  /* 00AFCE STA */
    A = rd16((ABS(0x0004) + X));  /* 00AFD0 LDA */
    A &= 0x00FF;  /* 00AFD3 AND */
    A = rla16(A, 0x0008);  /* 00AFD6 RLA */
    A &= 0xFF00;  /* 00AFDA AND */
    C = 0;  /* 00AFDD CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 00AFDE ADC */
    A = lsr16(A);  /* 00AFE0 LSR */
    A = lsr16(A);  /* 00AFE1 LSR */
    A = lsr16(A);  /* 00AFE2 LSR */
    A = lsr16(A);  /* 00AFE3 LSR */
    mflag = 1;  /* 00AFE4 SEM */
    SETLO(A, A & 0x1F);  /* 00AFE5 AND */
    mflag = 0;  /* 00AFE7 CLM */
    A &= 0x00FF;  /* 00AFE8 AND */
    wr16(DP(0x04), A);  /* 00AFEB STA */
    /* 00AFED CMP */
    if (A != 0x0014) goto L_00B03D;  /* 00AFF0 BNE */
    mflag = 1;  /* 00AFF2 SEM */
    SETLO(A, rd8(DP(0x0C)));  /* 00AFF3 LDA */
    if ((A & 0xFF) == 0) goto L_00B035;  /* 00AFF5 BEQ */
    mflag = 0;  /* 00AFF7 CLM */
    S -= 3; F_00AAAC();  /* 00AFF8 JSRL */
    /* 00AFFC CMP */
    if ((A & 0xFF) == 0x00) goto L_00B008;  /* 00AFFE BEQ */
    mflag = 0;  /* 00B000 CLM */
    S -= 3; F_00A948();  /* 00B001 JSRL */
    goto L_00B032;  /* 00B005 JMP */
L_00B008: TICK();
    mflag = 0;  /* 00B008 CLM */
    A = rd16(ABS(0x03B9));  /* 00B009 LDA */
    mflag = 1;  /* 00B00C SEM */
    /* 00B00D CMP */
    if ((A & 0xFF) != 0x24) goto L_00B02D;  /* 00B00F BNE */
    mflag = 0;  /* 00B011 CLM */
    S -= 3; F_00AAFE();  /* 00B012 JSRL */
    mflag = 1;  /* 00B016 SEM */
    wr8(ABS(0x03B8), 0x54);  /* 00B017 LDM */
    wr8(ABS(0x01BE), 0x0C);  /* 00B01B LDM */
    mflag = 0;  /* 00B01F CLM */
    wr16(ABS(0x03FF), 0x0000);  /* 00B020 LDM */
    mflag = 1;  /* 00B025 SEM */
    wr8(ABS(0x0415), 0x00);  /* 00B026 LDM */
    goto L_00B032;  /* 00B02A JMP */
L_00B02D: TICK();
    mflag = 0;  /* 00B02D CLM */
    S -= 3; F_00A8EE();  /* 00B02E JSRL */
L_00B032: TICK();
    goto L_00B03A;  /* 00B032 JMP */
L_00B035: TICK();
    mflag = 1;  /* 00B035 SEM */
    wr8(ABS(0x03B9), 0x03);  /* 00B036 LDM */
L_00B03A: TICK();
    goto L_00B0D7;  /* 00B03A JMP */
L_00B03D: TICK();
    mflag = 0;  /* 00B03D CLM */
    X = rd16(DP(0x04));  /* 00B03E LDX */
    /* 00B040 CPX */
    if (X < 0x000E) goto L_00B049;  /* 00B043 BCC */
    wr16(DP(0x04), 0x000D);  /* 00B045 LDM */
L_00B049: TICK();
    X = rd16(DP(0x04));  /* 00B049 LDX */
    A = 0x0001;  /* 00B04B LDA */
    S -= 3; F_01E4EA();  /* 00B04E JSRL */
    A -= 1;  /* 00B052 DEC */
    A &= rd16(DP(0x00));  /* 00B053 AND */
    wr16(DP(0x00), A);  /* 00B055 STA */
    X = rd16(ABS(0x03FB));  /* 00B057 LDX */
    if (X == 0) goto L_00B065;  /* 00B05A BEQ */
    X = A;  /* 00B05C TAX */
    if (X == 0) goto L_00B065;  /* 00B05D BEQ */
    A = 0x0001;  /* 00B05F LDA */
    goto L_00B068;  /* 00B062 JMP */
L_00B065: TICK();
    A = 0x0000;  /* 00B065 LDA */
L_00B068: TICK();
    A &= 0x00FF;  /* 00B068 AND */
    t = rd16(DP(0x00));  /* 00B06B CMP */
    if (A != t) goto L_00B0D2;  /* 00B06D BNE */
    X = 0x03C9;  /* 00B06F LDX */
    A = rd16((ABS(0x0005) + X));  /* 00B072 LDA */
    A &= 0x00FF;  /* 00B075 AND */
    wr16(DP(0x0F), A);  /* 00B078 STA */
    A = rd16((ABS(0x0004) + X));  /* 00B07A LDA */
    A &= 0x00FF;  /* 00B07D AND */
    A = rla16(A, 0x0008);  /* 00B080 RLA */
    A &= 0xFF00;  /* 00B084 AND */
    C = 0;  /* 00B087 CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 00B088 ADC */
    A = lsr16(A);  /* 00B08A LSR */
    A = lsr16(A);  /* 00B08B LSR */
    A = lsr16(A);  /* 00B08C LSR */
    A = lsr16(A);  /* 00B08D LSR */
    mflag = 1;  /* 00B08E SEM */
    SETLO(A, A & 0x1F);  /* 00B08F AND */
    mflag = 0;  /* 00B091 CLM */
    A &= 0x00FF;  /* 00B092 AND */
    C = 1;  /* 00B095 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 00B096 SBC */
    wr16(DP(0x04), A);  /* 00B098 STA */
    if (Z) goto L_00B0CF;  /* 00B09A BEQ */
    X = A;  /* 00B09C TAX */
    A = 0x0001;  /* 00B09D LDA */
    mflag = 1;  /* 00B0A0 SEM */
    S -= 3; rt_shl_01E4E0();  /* 00B0A1 JSRL */
    SETLO(A, A - 1);  /* 00B0A5 DEC */
    SETLO(A, A & rd8(DP(0x09)));  /* 00B0A6 AND */
    mflag = 0;  /* 00B0A8 CLM */
    A &= 0x00FF;  /* 00B0A9 AND */
    wr16(DP(0x04), A);  /* 00B0AC STA */
    mflag = 1;  /* 00B0AE SEM */
    SETLO(A, rd8(ABS(0x0425)));  /* 00B0AF LDA */
    if ((A & 0xFF) == 0) goto L_00B0BF;  /* 00B0B2 BEQ */
    mflag = 0;  /* 00B0B4 CLM */
    X = rd16(DP(0x04));  /* 00B0B5 LDX */
    if (X == 0) goto L_00B0BF;  /* 00B0B7 BEQ */
    A = 0x0001;  /* 00B0B9 LDA */
    goto L_00B0C3;  /* 00B0BC JMP */
L_00B0BF: TICK();
    mflag = 0;  /* 00B0BF CLM */
    A = 0x0000;  /* 00B0C0 LDA */
L_00B0C3: TICK();
    A &= 0x00FF;  /* 00B0C3 AND */
    t = rd16(DP(0x04));  /* 00B0C6 CMP */
    if (A == t) goto L_00B0CF;  /* 00B0C8 BEQ */
    mflag = 1;  /* 00B0CA SEM */
    wr8(ABS(0x03BB), 0x23);  /* 00B0CB LDM */
L_00B0CF: TICK();
    goto L_00B0D7;  /* 00B0CF JMP */
L_00B0D2: TICK();
    mflag = 1;  /* 00B0D2 SEM */
    wr8(ABS(0x03BB), 0x23);  /* 00B0D3 LDM */
L_00B0D7: TICK();
    goto L_00B0E5;  /* 00B0D7 JMP */
L_00B0DA: TICK();
    mflag = 1;  /* 00B0DA SEM */
    SETLO(A, rd8(ABS(0x03A8)));  /* 00B0DB LDA */
    if ((A & 0xFF) != 0) goto L_00B0E5;  /* 00B0DE BNE */
    mflag = 0;  /* 00B0E0 CLM */
    S -= 3; F_00A399();  /* 00B0E1 JSRL */
L_00B0E5: TICK();
    goto L_00B503;  /* 00B0E5 JMP */
L_00B0E8: TICK();
    mflag = 1;  /* 00B0E8 SEM */
    /* 00B0E9 CMP */
    if ((A & 0xFF) == 0x01) goto L_00B0F0;  /* 00B0EB BEQ */
    goto L_00B1BF;  /* 00B0ED JMP */
L_00B0F0: TICK();
    mflag = 0;  /* 00B0F0 CLM */
    S -= 3; F_00ABD3();  /* 00B0F1 JSRL */
    mflag = 1;  /* 00B0F5 SEM */
    wr8(ABS(0x03B9), 0x80);  /* 00B0F6 LDM */
    SETLO(A, rd8(DP(0x0A)));  /* 00B0FA LDA */
    if ((A & 0xFF) != 0) goto L_00B101;  /* 00B0FC BNE */
    goto L_00B1BC;  /* 00B0FE JMP */
L_00B101: TICK();
    SETLO(A, rd8(DP(0x0D)));  /* 00B101 LDA */
    if ((A & 0xFF) != 0) goto L_00B108;  /* 00B103 BNE */
    goto L_00B1BC;  /* 00B105 JMP */
L_00B108: TICK();
    SETLO(A, rd8(DP(0x0B)));  /* 00B108 LDA */
    if ((A & 0xFF) != 0) goto L_00B122;  /* 00B10A BNE */
    mflag = 0;  /* 00B10C CLM */
    S -= 3; F_00AC38();  /* 00B10D JSRL */
    /* 00B111 CMP */
    if ((A & 0xFF) == 0x00) goto L_00B11A;  /* 00B113 BEQ */
    SETLO(A, 0x81);  /* 00B115 LDA */
    goto L_00B11C;  /* 00B117 JMP */
L_00B11A: TICK();
    SETLO(A, 0x82);  /* 00B11A LDA */
L_00B11C: TICK();
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00B11C STA */
    goto L_00B1BC;  /* 00B11F JMP */
L_00B122: TICK();
    mflag = 0;  /* 00B122 CLM */
    A = rd16(DP(0x0B));  /* 00B123 LDA */
    mflag = 1;  /* 00B125 SEM */
    /* 00B126 CMP */
    if ((A & 0xFF) != 0x01) goto L_00B140;  /* 00B128 BNE */
    mflag = 0;  /* 00B12A CLM */
    S -= 3; F_00AC38();  /* 00B12B JSRL */
    /* 00B12F CMP */
    if ((A & 0xFF) == 0x00) goto L_00B138;  /* 00B131 BEQ */
    SETLO(A, 0x84);  /* 00B133 LDA */
    goto L_00B13A;  /* 00B135 JMP */
L_00B138: TICK();
    SETLO(A, 0x83);  /* 00B138 LDA */
L_00B13A: TICK();
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00B13A STA */
    goto L_00B1BC;  /* 00B13D JMP */
L_00B140: TICK();
    mflag = 0;  /* 00B140 CLM */
    A = rd16(DP(0x0B));  /* 00B141 LDA */
    mflag = 1;  /* 00B143 SEM */
    /* 00B144 CMP */
    if ((A & 0xFF) != 0x02) goto L_00B14F;  /* 00B146 BNE */
    wr8(ABS(0x03B9), 0x85);  /* 00B148 LDM */
    goto L_00B1BC;  /* 00B14C JMP */
L_00B14F: TICK();
    mflag = 0;  /* 00B14F CLM */
    A = rd16(DP(0x0B));  /* 00B150 LDA */
    mflag = 1;  /* 00B152 SEM */
    /* 00B153 CMP */
    if ((A & 0xFF) != 0x03) goto L_00B16D;  /* 00B155 BNE */
    mflag = 0;  /* 00B157 CLM */
    S -= 3; F_00AC38();  /* 00B158 JSRL */
    /* 00B15C CMP */
    if ((A & 0xFF) == 0x00) goto L_00B165;  /* 00B15E BEQ */
    SETLO(A, 0x86);  /* 00B160 LDA */
    goto L_00B167;  /* 00B162 JMP */
L_00B165: TICK();
    SETLO(A, 0x87);  /* 00B165 LDA */
L_00B167: TICK();
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00B167 STA */
    goto L_00B1BC;  /* 00B16A JMP */
L_00B16D: TICK();
    mflag = 0;  /* 00B16D CLM */
    A = rd16(DP(0x0B));  /* 00B16E LDA */
    mflag = 1;  /* 00B170 SEM */
    /* 00B171 CMP */
    if ((A & 0xFF) != 0x04) goto L_00B18B;  /* 00B173 BNE */
    mflag = 0;  /* 00B175 CLM */
    S -= 3; F_00AC38();  /* 00B176 JSRL */
    /* 00B17A CMP */
    if ((A & 0xFF) == 0x00) goto L_00B183;  /* 00B17C BEQ */
    SETLO(A, 0x88);  /* 00B17E LDA */
    goto L_00B185;  /* 00B180 JMP */
L_00B183: TICK();
    SETLO(A, 0x89);  /* 00B183 LDA */
L_00B185: TICK();
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00B185 STA */
    goto L_00B1BC;  /* 00B188 JMP */
L_00B18B: TICK();
    mflag = 0;  /* 00B18B CLM */
    S -= 3; F_00AC38();  /* 00B18C JSRL */
    /* 00B190 CMP */
    if ((A & 0xFF) != 0x00) goto L_00B1BC;  /* 00B192 BNE */
    SETLO(A, rd8(DP(0x0B)));  /* 00B194 LDA */
    /* 00B196 CMP */
    if ((A & 0xFF) != 0x05) goto L_00B1A1;  /* 00B198 BNE */
    wr8(ABS(0x03B9), 0x8A);  /* 00B19A LDM */
    goto L_00B1BC;  /* 00B19E JMP */
L_00B1A1: TICK();
    mflag = 0;  /* 00B1A1 CLM */
    A = rd16(DP(0x0B));  /* 00B1A2 LDA */
    mflag = 1;  /* 00B1A4 SEM */
    /* 00B1A5 CMP */
    if ((A & 0xFF) != 0x06) goto L_00B1B0;  /* 00B1A7 BNE */
    wr8(ABS(0x03B9), 0x8B);  /* 00B1A9 LDM */
    goto L_00B1BC;  /* 00B1AD JMP */
L_00B1B0: TICK();
    mflag = 0;  /* 00B1B0 CLM */
    A = rd16(DP(0x0B));  /* 00B1B1 LDA */
    mflag = 1;  /* 00B1B3 SEM */
    /* 00B1B4 CMP */
    if ((A & 0xFF) != 0x07) goto L_00B1BC;  /* 00B1B6 BNE */
    wr8(ABS(0x03B9), 0x8C);  /* 00B1B8 LDM */
L_00B1BC: TICK();
    goto L_00B503;  /* 00B1BC JMP */
L_00B1BF: TICK();
    mflag = 1;  /* 00B1BF SEM */
    /* 00B1C0 CMP */
    if ((A & 0xFF) == 0x02) goto L_00B1C7;  /* 00B1C2 BEQ */
    goto L_00B2FC;  /* 00B1C4 JMP */
L_00B1C7: TICK();
    wr8(ABS(0x03B9), 0x03);  /* 00B1C7 LDM */
    SETLO(A, rd8(DP(0x0A)));  /* 00B1CB LDA */
    if ((A & 0xFF) != 0) goto L_00B1D3;  /* 00B1CD BNE */
    mflag = 0;  /* 00B1CF CLM */
    Y = pull16();  /* 00B1D0 PLY */
    D = pull16();  /* 00B1D1 PLD */
    S += 3; return;  /* 00B1D2 RTL */
L_00B1D3: TICK();
    SETLO(A, rd8(DP(0x0B)));  /* 00B1D3 LDA */
    if ((A & 0xFF) != 0) goto L_00B235;  /* 00B1D5 BNE */
    X = 0x03C9;  /* 00B1D7 LDX */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B1DA LDA */
    SETLO(A, A & 0x01);  /* 00B1DD AND */
    if ((A & 0xFF) == 0) goto L_00B1E5;  /* 00B1DF BEQ */
    wr8(ABS(0x0433), 0x02);  /* 00B1E1 LDM */
L_00B1E5: TICK();
    SETLO(A, rd8(DP(0x0D)));  /* 00B1E5 LDA */
    if ((A & 0xFF) == 0) goto L_00B1FD;  /* 00B1E7 BEQ */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B1E9 LDA */
    SETLO(A, A & 0x08);  /* 00B1EC AND */
    if ((A & 0xFF) == 0) goto L_00B1F5;  /* 00B1EE BEQ */
    SETLO(A, 0x00);  /* 00B1F0 LDA */
    goto L_00B1F7;  /* 00B1F2 JMP */
L_00B1F5: TICK();
    SETLO(A, 0x04);  /* 00B1F5 LDA */
L_00B1F7: TICK();
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00B1F7 STA */
    goto L_00B232;  /* 00B1FA JMP */
L_00B1FD: TICK();
    mflag = 1;  /* 00B1FD SEM */
    SETLO(A, rd8(DP(0x0C)));  /* 00B1FE LDA */
    if ((A & 0xFF) == 0) goto L_00B232;  /* 00B200 BEQ */
    X = 0x03C9;  /* 00B202 LDX */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B205 LDA */
    SETLO(A, A & 0x08);  /* 00B208 AND */
    if ((A & 0xFF) != 0) goto L_00B232;  /* 00B20A BNE */
    mflag = 0;  /* 00B20C CLM */
    X = rd16(DP(0x02));  /* 00B20D LDX */
    wr16(ABS(0x03D5), X);  /* 00B20F STX */
    X = 0x03C9;  /* 00B212 LDX */
    A = rd16((ABS(0x0005) + X));  /* 00B215 LDA */
    mflag = 1;  /* 00B218 SEM */
    SETLO(A, A & 0x04);  /* 00B219 AND */
    if ((A & 0xFF) == 0) goto L_00B222;  /* 00B21B BEQ */
    SETLO(A, 0x02);  /* 00B21D LDA */
    goto L_00B224;  /* 00B21F JMP */
L_00B222: TICK();
    SETLO(A, 0x01);  /* 00B222 LDA */
L_00B224: TICK();
    wr8(ABS(0x03B9), (uint8_t)A);  /* 00B224 STA */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B227 LDA */
    SETLO(A, A & 0x02);  /* 00B22A AND */
    if ((A & 0xFF) == 0) goto L_00B232;  /* 00B22C BEQ */
    wr8(ABS(0x03A2), 0x01);  /* 00B22E LDM */
L_00B232: TICK();
    goto L_00B2F9;  /* 00B232 JMP */
L_00B235: TICK();
    mflag = 0;  /* 00B235 CLM */
    A = rd16(DP(0x0B));  /* 00B236 LDA */
    mflag = 1;  /* 00B238 SEM */
    /* 00B239 CMP */
    if ((A & 0xFF) != 0x02) goto L_00B248;  /* 00B23B BNE */
    SETLO(A, rd8(DP(0x0C)));  /* 00B23D LDA */
    if ((A & 0xFF) == 0) goto L_00B245;  /* 00B23F BEQ */
    wr8(ABS(0x03B9), 0x07);  /* 00B241 LDM */
L_00B245: TICK();
    goto L_00B2F9;  /* 00B245 JMP */
L_00B248: TICK();
    mflag = 0;  /* 00B248 CLM */
    A = rd16(DP(0x0B));  /* 00B249 LDA */
    mflag = 1;  /* 00B24B SEM */
    /* 00B24C CMP */
    if ((A & 0xFF) != 0x06) goto L_00B282;  /* 00B24E BNE */
    SETLO(A, rd8(DP(0x0C)));  /* 00B250 LDA */
    if ((A & 0xFF) == 0) goto L_00B276;  /* 00B252 BEQ */
    wr8(ABS(0x03B9), 0x08);  /* 00B254 LDM */
    mflag = 0;  /* 00B258 CLM */
    X = rd16(DP(0x02));  /* 00B259 LDX */
    wr16(ABS(0x03D5), X);  /* 00B25B STX */
    Y = 0x03C9;  /* 00B25E LDY */
    A = rd16((ABS(0x0005) + Y));  /* 00B261 LDA */
    mflag = 1;  /* 00B264 SEM */
    SETLO(A, A & 0x1F);  /* 00B265 AND */
    wr8(ABS(0x01BF), (uint8_t)A);  /* 00B267 STA */
    /* 00B26A CPX */
    if (X != 0x1FFD) goto L_00B273;  /* 00B26D BNE */
    wr8(ABS(0x0433), 0x02);  /* 00B26F LDM */
L_00B273: TICK();
    goto L_00B27F;  /* 00B273 JMP */
L_00B276: TICK();
    mflag = 1;  /* 00B276 SEM */
    SETLO(A, rd8(DP(0x0D)));  /* 00B277 LDA */
    if ((A & 0xFF) == 0) goto L_00B27F;  /* 00B279 BEQ */
    wr8(ABS(0x03B9), 0x04);  /* 00B27B LDM */
L_00B27F: TICK();
    goto L_00B2F9;  /* 00B27F JMP */
L_00B282: TICK();
    mflag = 0;  /* 00B282 CLM */
    A = rd16(DP(0x0B));  /* 00B283 LDA */
    mflag = 1;  /* 00B285 SEM */
    /* 00B286 CMP */
    if ((A & 0xFF) != 0x07) goto L_00B2F9;  /* 00B288 BNE */
    SETLO(A, rd8(DP(0x0C)));  /* 00B28A LDA */
    if ((A & 0xFF) == 0) goto L_00B2A2;  /* 00B28C BEQ */
    SETLO(A, rd8(DP(0x0E)));  /* 00B28E LDA */
    if ((A & 0xFF) != 0) goto L_00B29F;  /* 00B290 BNE */
    mflag = 0;  /* 00B292 CLM */
    X = rd16(DP(0x02));  /* 00B293 LDX */
    /* 00B295 CPX */
    if (X != 0x1FFE) goto L_00B29F;  /* 00B298 BNE */
    mflag = 1;  /* 00B29A SEM */
    wr8(ABS(0x03B9), 0x09);  /* 00B29B LDM */
L_00B29F: TICK();
    goto L_00B2F9;  /* 00B29F JMP */
L_00B2A2: TICK();
    mflag = 1;  /* 00B2A2 SEM */
    SETLO(A, rd8(DP(0x0D)));  /* 00B2A3 LDA */
    if ((A & 0xFF) == 0) goto L_00B2F9;  /* 00B2A5 BEQ */
    mflag = 0;  /* 00B2A7 CLM */
    X = rd16(DP(0x00));  /* 00B2A8 LDX */
    /* 00B2AA CPX */
    if (X != 0x1FFC) goto L_00B2B7;  /* 00B2AD BNE */
    mflag = 1;  /* 00B2AF SEM */
    wr8(ABS(0x03B9), 0x0A);  /* 00B2B0 LDM */
    goto L_00B2F9;  /* 00B2B4 JMP */
L_00B2B7: TICK();
    mflag = 1;  /* 00B2B7 SEM */
    SETLO(A, rd8(DP(0x0E)));  /* 00B2B8 LDA */
    if ((A & 0xFF) != 0) goto L_00B2CC;  /* 00B2BA BNE */
    mflag = 0;  /* 00B2BC CLM */
    X = rd16(DP(0x00));  /* 00B2BD LDX */
    /* 00B2BF CPX */
    if (X != 0x1FFD) goto L_00B2CC;  /* 00B2C2 BNE */
    mflag = 1;  /* 00B2C4 SEM */
    wr8(ABS(0x03B9), 0x0B);  /* 00B2C5 LDM */
    goto L_00B2F9;  /* 00B2C9 JMP */
L_00B2CC: TICK();
    mflag = 0;  /* 00B2CC CLM */
    A = rd16(DP(0x0E));  /* 00B2CD LDA */
    mflag = 1;  /* 00B2CF SEM */
    /* 00B2D0 CMP */
    if ((A & 0xFF) != 0x01) goto L_00B2E4;  /* 00B2D2 BNE */
    mflag = 0;  /* 00B2D4 CLM */
    X = rd16(DP(0x00));  /* 00B2D5 LDX */
    /* 00B2D7 CPX */
    if (X != 0x1FA5) goto L_00B2E4;  /* 00B2DA BNE */
    mflag = 1;  /* 00B2DC SEM */
    wr8(ABS(0x03B9), 0x0C);  /* 00B2DD LDM */
    goto L_00B2F9;  /* 00B2E1 JMP */
L_00B2E4: TICK();
    mflag = 0;  /* 00B2E4 CLM */
    A = rd16(DP(0x0E));  /* 00B2E5 LDA */
    mflag = 1;  /* 00B2E7 SEM */
    /* 00B2E8 CMP */
    if ((A & 0xFF) != 0x02) goto L_00B2F9;  /* 00B2EA BNE */
    mflag = 0;  /* 00B2EC CLM */
    X = rd16(DP(0x00));  /* 00B2ED LDX */
    /* 00B2EF CPX */
    if (X != 0x1FA6) goto L_00B2F9;  /* 00B2F2 BNE */
    mflag = 1;  /* 00B2F4 SEM */
    wr8(ABS(0x03B9), 0x0D);  /* 00B2F5 LDM */
L_00B2F9: TICK();
    goto L_00B503;  /* 00B2F9 JMP */
L_00B2FC: TICK();
    mflag = 1;  /* 00B2FC SEM */
    /* 00B2FD CMP */
    if ((A & 0xFF) == 0x03) goto L_00B304;  /* 00B2FF BEQ */
    goto L_00B503;  /* 00B301 JMP */
L_00B304: TICK();
    SETLO(A, rd8(DP(0x0B)));  /* 00B304 LDA */
    /* 00B306 CMP */
    if ((A & 0xFF) != 0x02) goto L_00B324;  /* 00B308 BNE */
    X = 0x03C9;  /* 00B30A LDX */
    SETLO(A, rd8((ABS(0x0004) + X)));  /* 00B30D LDA */
    SETLO(A, A & 0x0F);  /* 00B310 AND */
    /* 00B312 CMP */
    if ((A & 0xFF) != 0x0A) goto L_00B324;  /* 00B314 BNE */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B316 LDA */
    /* 00B319 CMP */
    if ((A & 0xFF) != 0xAA) goto L_00B324;  /* 00B31B BNE */
    wr8(ABS(0x03B9), 0x16);  /* 00B31D LDM */
    goto L_00B503;  /* 00B321 JMP */
L_00B324: TICK();
    mflag = 0;  /* 00B324 CLM */
    A = rd16(DP(0x0B));  /* 00B325 LDA */
    mflag = 1;  /* 00B327 SEM */
    /* 00B328 CMP */
    if ((A & 0xFF) == 0x01) goto L_00B32F;  /* 00B32A BEQ */
    goto L_00B3BD;  /* 00B32C JMP */
L_00B32F: TICK();
    mflag = 0;  /* 00B32F CLM */
    X = rd16(DP(0x06));  /* 00B330 LDX */
    t = rd16(ABS(0x03F3));  /* 00B332 CPX */
    if (X == t) goto L_00B33A;  /* 00B335 BEQ */
    goto L_00B3BD;  /* 00B337 JMP */
L_00B33A: TICK();
    A = rd16(DP(0x08));  /* 00B33A LDA */
    mflag = 1;  /* 00B33C SEM */
    /* 00B33D CMP */
    if ((A & 0xFF) == 0x01) goto L_00B345;  /* 00B33F BEQ */
    SETLO(A, rd8(DP(0x08)));  /* 00B341 LDA */
    if ((A & 0xFF) != 0) goto L_00B358;  /* 00B343 BNE */
L_00B345: TICK();
    wr8(ABS(0x01BE), 0x0D);  /* 00B345 LDM */
    mflag = 0;  /* 00B349 CLM */
    X = rd16(DP(0x00));  /* 00B34A LDX */
    wr16(ABS(0x03D7), X);  /* 00B34C STX */
    A = rd16(DP(0x09));  /* 00B34F LDA */
    mflag = 1;  /* 00B351 SEM */
    wr8(ABS(0x01B8), (uint8_t)A);  /* 00B352 STA */
    goto L_00B3BA;  /* 00B355 JMP */
L_00B358: TICK();
    mflag = 0;  /* 00B358 CLM */
    A = rd16(DP(0x08));  /* 00B359 LDA */
    mflag = 1;  /* 00B35B SEM */
    /* 00B35C CMP */
    if ((A & 0xFF) != 0x07) goto L_00B370;  /* 00B35E BNE */
    SETLO(A, rd8(DP(0x0C)));  /* 00B360 LDA */
    if ((A & 0xFF) != 0) goto L_00B369;  /* 00B362 BNE */
    SETLO(A, rd8(ABS(0x0414)));  /* 00B364 LDA */
    if ((A & 0xFF) != 0) goto L_00B370;  /* 00B367 BNE */
L_00B369: TICK();
    wr8(ABS(0x03AD), 0x01);  /* 00B369 LDM */
    goto L_00B3BA;  /* 00B36D JMP */
L_00B370: TICK();
    mflag = 0;  /* 00B370 CLM */
    A = rd16(DP(0x08));  /* 00B371 LDA */
    mflag = 1;  /* 00B373 SEM */
    /* 00B374 CMP */
    if ((A & 0xFF) != 0x06) goto L_00B3BA;  /* 00B376 BNE */
    SETLO(A, rd8(ABS(0x0414)));  /* 00B378 LDA */
    if ((A & 0xFF) != 0) goto L_00B390;  /* 00B37B BNE */
    Y = rd16(DP(0x00));  /* 00B37D LDY */
    mflag = 0;  /* 00B37F CLM */
    S -= 3; F_00ACA6();  /* 00B380 JSRL */
    /* 00B384 CMP */
    if ((A & 0xFF) != 0x00) goto L_00B38D;  /* 00B386 BNE */
    mflag = 0;  /* 00B388 CLM */
    S -= 3; F_00A4E9();  /* 00B389 JSRL */
L_00B38D: TICK();
    goto L_00B3BA;  /* 00B38D JMP */
L_00B390: TICK();
    mflag = 0;  /* 00B390 CLM */
    A = rd16(ABS(0x0414));  /* 00B391 LDA */
    mflag = 1;  /* 00B394 SEM */
    /* 00B395 CMP */
    if ((A & 0xFF) != 0x03) goto L_00B3A5;  /* 00B397 BNE */
    SETLO(A, rd8(DP(0x0C)));  /* 00B399 LDA */
    if ((A & 0xFF) != 0) goto L_00B3A2;  /* 00B39B BNE */
    mflag = 0;  /* 00B39D CLM */
    S -= 3; F_00A4E9();  /* 00B39E JSRL */
L_00B3A2: TICK();
    goto L_00B3BA;  /* 00B3A2 JMP */
L_00B3A5: TICK();
    mflag = 0;  /* 00B3A5 CLM */
    X = rd16(DP(0x00));  /* 00B3A6 LDX */
    t = rd16(ABS(0x01CF));  /* 00B3A8 CPX */
    if (X != t) goto L_00B3B5;  /* 00B3AB BNE */
    A = rd16(DP(0x09));  /* 00B3AD LDA */
    mflag = 1;  /* 00B3AF SEM */
    t = rd8(ABS(0x01B8));  /* 00B3B0 CMP */
    if ((A & 0xFF) == t) goto L_00B3BA;  /* 00B3B3 BEQ */
L_00B3B5: TICK();
    mflag = 0;  /* 00B3B5 CLM */
    S -= 3; F_00A4E9();  /* 00B3B6 JSRL */
L_00B3BA: TICK();
    goto L_00B503;  /* 00B3BA JMP */
L_00B3BD: TICK();
    mflag = 0;  /* 00B3BD CLM */
    A = rd16(DP(0x0B));  /* 00B3BE LDA */
    mflag = 1;  /* 00B3C0 SEM */
    /* 00B3C1 CMP */
    if ((A & 0xFF) != 0x03) goto L_00B444;  /* 00B3C3 BNE */
    X = 0x03C9;  /* 00B3C5 LDX */
    SETLO(B, 0x00);  /* 00B3C8 LDA */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B3CB LDA */
    div8(0x08);  /* 00B3CE DIV */
    mflag = 0;  /* 00B3D1 CLM */
    A &= 0x00FF;  /* 00B3D2 AND */
    wr16(DP(0x04), A);  /* 00B3D5 STA */
    /* 00B3D7 CMP */
    if (A < 0x000E) goto L_00B3E0;  /* 00B3DA BCC */
    wr16(DP(0x04), 0x000D);  /* 00B3DC LDM */
L_00B3E0: TICK();
    X = rd16(DP(0x04));  /* 00B3E0 LDX */
    A = 0x0001;  /* 00B3E2 LDA */
    S -= 3; F_01E4EA();  /* 00B3E5 JSRL */
    A -= 1;  /* 00B3E9 DEC */
    A &= rd16(DP(0x00));  /* 00B3EA AND */
    wr16(DP(0x00), A);  /* 00B3EC STA */
    A &= rd16(ABS(0x03FB));  /* 00B3EE AND */
    t = rd16(DP(0x00));  /* 00B3F1 CMP */
    if (A != t) goto L_00B441;  /* 00B3F3 BNE */
    X = 0x03C9;  /* 00B3F5 LDX */
    mflag = 1;  /* 00B3F8 SEM */
    SETLO(B, 0x00);  /* 00B3F9 LDA */
    SETLO(A, rd8((ABS(0x0005) + X)));  /* 00B3FC LDA */
    div8(0x08);  /* 00B3FF DIV */
    mflag = 0;  /* 00B402 CLM */
    A &= 0x00FF;  /* 00B403 AND */
    C = 1;  /* 00B406 SEC */
    A = sbc16(A, rd16(DP(0x04)));  /* 00B407 SBC */
    wr16(DP(0x04), A);  /* 00B409 STA */
    if (Z) goto L_00B437;  /* 00B40B BEQ */
    X = A;  /* 00B40D TAX */
    A = 0x0001;  /* 00B40E LDA */
    mflag = 1;  /* 00B411 SEM */
    S -= 3; rt_shl_01E4E0();  /* 00B412 JSRL */
    SETLO(A, A - 1);  /* 00B416 DEC */
    SETLO(A, A & rd8(DP(0x09)));  /* 00B417 AND */
    mflag = 0;  /* 00B419 CLM */
    A &= 0x00FF;  /* 00B41A AND */
    wr16(DP(0x04), A);  /* 00B41D STA */
    mflag = 1;  /* 00B41F SEM */
    SETLO(A, A & rd8(ABS(0x0425)));  /* 00B420 AND */
    mflag = 0;  /* 00B423 CLM */
    A &= 0x00FF;  /* 00B424 AND */
    t = rd16(DP(0x04));  /* 00B427 CMP */
    if (A != t) goto L_00B434;  /* 00B429 BNE */
    X = rd16(DP(0x06));  /* 00B42B LDX */
    wr16(ABS(0x03F1), X);  /* 00B42D STX */
    S -= 3; F_00A4E9();  /* 00B430 JSRL */
L_00B434: TICK();
    goto L_00B441;  /* 00B434 JMP */
L_00B437: TICK();
    mflag = 0;  /* 00B437 CLM */
    X = rd16(DP(0x06));  /* 00B438 LDX */
    wr16(ABS(0x03F1), X);  /* 00B43A STX */
    S -= 3; F_00A4E9();  /* 00B43D JSRL */
L_00B441: TICK();
    goto L_00B503;  /* 00B441 JMP */
L_00B444: TICK();
    mflag = 0;  /* 00B444 CLM */
    A = rd16(DP(0x0B));  /* 00B445 LDA */
    mflag = 1;  /* 00B447 SEM */
    /* 00B448 CMP */
    if ((A & 0xFF) == 0x04) goto L_00B44F;  /* 00B44A BEQ */
    goto L_00B503;  /* 00B44C JMP */
L_00B44F: TICK();
    SETLO(A, rd8(ABS(0x03C9)));  /* 00B44F LDA */
    SETLO(A, A & 0x7C);  /* 00B452 AND */
    /* 00B454 CMP */
    if ((A & 0xFF) != 0x08) goto L_00B476;  /* 00B456 BNE */
    X = 0x03C9;  /* 00B458 LDX */
    SETLO(A, rd8((ABS(0x0004) + X)));  /* 00B45B LDA */
    SETLO(A, A & 0x08);  /* 00B45E AND */
    /* 00B460 CMP */
    if ((A & 0xFF) == 0x08) goto L_00B46B;  /* 00B462 BEQ */
    mflag = 0;  /* 00B464 CLM */
    A = 0x0001;  /* 00B465 LDA */
    goto L_00B46F;  /* 00B468 JMP */
L_00B46B: TICK();
    mflag = 0;  /* 00B46B CLM */
    A = 0x0000;  /* 00B46C LDA */
L_00B46F: TICK();
    mflag = 1;  /* 00B46F SEM */
    wr8(ABS(0x03AB), (uint8_t)A);  /* 00B470 STA */
    goto L_00B503;  /* 00B473 JMP */
L_00B476: TICK();
    mflag = 0;  /* 00B476 CLM */
    A = rd16(ABS(0x03C9));  /* 00B477 LDA */
    mflag = 1;  /* 00B47A SEM */
    SETLO(A, A & 0x7C);  /* 00B47B AND */
    /* 00B47D CMP */
    if ((A & 0xFF) != 0x0C) goto L_00B4C8;  /* 00B47F BNE */
    X = 0x03C9;  /* 00B481 LDX */
    SETLO(A, rd8((ABS(0x0004) + X)));  /* 00B484 LDA */
    SETLO(A, A & 0x30);  /* 00B487 AND */
    SETLO(B, 0x00);  /* 00B489 LDA */
    div8(0x10);  /* 00B48C DIV */
    wr8(ABS(0x0428), (uint8_t)A);  /* 00B48F STA */
    SETLO(A, rd8(ABS(0x03AC)));  /* 00B492 LDA */
    if ((A & 0xFF) == 0) goto L_00B4A8;  /* 00B495 BEQ */
    SETLO(A, rd8((ABS(0x0004) + X)));  /* 00B497 LDA */
    SETLO(A, A & 0x08);  /* 00B49A AND */
    if ((A & 0xFF) != 0) goto L_00B4A8;  /* 00B49C BNE */
    mflag = 0;  /* 00B49E CLM */
    wr16(ABS(0x03FF), 0x0000);  /* 00B49F LDM */
    S -= 3; F_00A399();  /* 00B4A4 JSRL */
L_00B4A8: TICK();
    X = 0x03C9;  /* 00B4A8 LDX */
    mflag = 0;  /* 00B4AB CLM */
    A = rd16((ABS(0x0004) + X));  /* 00B4AC LDA */
    mflag = 1;  /* 00B4AF SEM */
    SETLO(A, A & 0x08);  /* 00B4B0 AND */
    /* 00B4B2 CMP */
    if ((A & 0xFF) != 0x08) goto L_00B4BD;  /* 00B4B4 BNE */
    mflag = 0;  /* 00B4B6 CLM */
    A = 0x0001;  /* 00B4B7 LDA */
    goto L_00B4C1;  /* 00B4BA JMP */
L_00B4BD: TICK();
    mflag = 0;  /* 00B4BD CLM */
    A = 0x0000;  /* 00B4BE LDA */
L_00B4C1: TICK();
    mflag = 1;  /* 00B4C1 SEM */
    wr8(ABS(0x03AC), (uint8_t)A);  /* 00B4C2 STA */
    goto L_00B503;  /* 00B4C5 JMP */
L_00B4C8: TICK();
    mflag = 0;  /* 00B4C8 CLM */
    A = rd16(ABS(0x03C9));  /* 00B4C9 LDA */
    mflag = 1;  /* 00B4CC SEM */
    SETLO(A, A & 0x7C);  /* 00B4CD AND */
    /* 00B4CF CMP */
    if ((A & 0xFF) != 0x14) goto L_00B503;  /* 00B4D1 BNE */
    SETLO(A, rd8(ABS(0x03BA)));  /* 00B4D3 LDA */
    /* 00B4D6 CMP */
    if ((A & 0xFF) != 0x02) goto L_00B503;  /* 00B4D8 BNE */
    mflag = 0;  /* 00B4DA CLM */
    X = rd16(ABS(0x03B6));  /* 00B4DB LDX */
    /* 00B4DE CPX */
    if (X != 0x0004) goto L_00B503;  /* 00B4E1 BNE */
    X = rd16(DP(0x06));  /* 00B4E3 LDX */
    wr16(ABS(0x03F1), X);  /* 00B4E5 STX */
    wr16(ABS(0x03F5), X);  /* 00B4E8 STX */
    S -= 3; F_00A01C();  /* 00B4EB JSRL */
    mflag = 1;  /* 00B4EF SEM */
    wr8(ABS(0x0229), 0x01);  /* 00B4F0 LDM */
    wr8(ABS(0x03BA), 0xC0);  /* 00B4F4 LDM */
    mflag = 0;  /* 00B4F8 CLM */
    wr16(ABS(0x03F7), 0x0006);  /* 00B4F9 LDM */
    mflag = 1;  /* 00B4FE SEM */
    wr8(ABS(0x0442), 0x01);  /* 00B4FF LDM */
L_00B503: TICK();
    goto L_00B55F;  /* 00B503 JMP */
L_00B506: TICK();
    X = 0x03C9;  /* 00B506 LDX */
    mflag = 0;  /* 00B509 CLM */
    A = rd16((ABS(0x0003) + X));  /* 00B50A LDA */
    A &= 0x00FF;  /* 00B50D AND */
    wr16(DP(0x0F), A);  /* 00B510 STA */
    A = rd16((ABS(0x0002) + X));  /* 00B512 LDA */
    A &= 0x00FF;  /* 00B515 AND */
    A = rla16(A, 0x0008);  /* 00B518 RLA */
    A &= 0xFF00;  /* 00B51C AND */
    C = 0;  /* 00B51F CLC */
    A = adc16(A, rd16(DP(0x0F)));  /* 00B520 ADC */
    A = asl16(A);  /* 00B522 ASL */
    A = rla16(A, 0x0008);  /* 00B523 RLA */
    A &= 0x00FF;  /* 00B527 AND */
    A &= 0x0007;  /* 00B52A AND */
    /* 00B52D CMP */
    if (A != 0x0003) goto L_00B55F;  /* 00B530 BNE */
    S -= 3; F_00AB15();  /* 00B532 JSRL */
    /* 00B536 CMP */
    if ((A & 0xFF) != 0x02) goto L_00B55F;  /* 00B538 BNE */
    wr8(ABS(0x03B9), 0x13);  /* 00B53A LDM */
    SETLO(A, rd8(DP(0x0C)));  /* 00B53E LDA */
    if ((A & 0xFF) == 0) goto L_00B55F;  /* 00B540 BEQ */
    mflag = 0;  /* 00B542 CLM */
    S -= 3; F_00AB25();  /* 00B543 JSRL */
    /* 00B547 CMP */
    if ((A & 0xFF) != 0x00) goto L_00B552;  /* 00B549 BNE */
    wr8(ABS(0x03B9), 0x12);  /* 00B54B LDM */
    goto L_00B55F;  /* 00B54F JMP */
L_00B552: TICK();
    mflag = 0;  /* 00B552 CLM */
    S -= 3; F_00AB25();  /* 00B553 JSRL */
    /* 00B557 CMP */
    if ((A & 0xFF) != 0x07) goto L_00B55F;  /* 00B559 BNE */
    wr8(ABS(0x03B9), 0x14);  /* 00B55B LDM */
L_00B55F: TICK();
    mflag = 0;  /* 00B55F CLM */
    Y = pull16();  /* 00B560 PLY */
    D = pull16();  /* 00B561 PLD */
    S += 3; return;  /* 00B562 RTL */
}

