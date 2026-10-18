/* hal_m37732.h - HAL for the original M37732S4 (K3 board).
 * Register addresses: M37732S4 datasheet Fig. 2.  Bit usage taken from the
 * original firmware (see comments with the original code addresses). */
#ifndef HAL_M37732_H
#define HAL_M37732_H
#include <stdint.h>

#ifndef REG8
#define REG8(a)  (*(volatile uint8_t  *)(uintptr_t)(a))
#endif
#ifndef REG16
#define REG16(a) (*(volatile uint16_t *)(uintptr_t)(a))
#endif

#define P4      REG8(0x0A)
#define P5      REG8(0x0B)
#define P4D     REG8(0x0C)
#define P5D     REG8(0x0D)
#define P6      REG8(0x0E)
#define P7      REG8(0x0F)
#define P6D     REG8(0x10)
#define P7D     REG8(0x11)
#define P8      REG8(0x12)
#define P8D     REG8(0x14)
#define ADREG(n) REG8(0x20 + 2 * (n))
#define U1MOD   REG8(0x38)
#define U1BRG   REG8(0x39)
#define U1TB    REG8(0x3A)
#define U1C0    REG8(0x3C)
#define U1C1    REG8(0x3D)
#define TCSTART REG8(0x40)
#define TOSTART REG8(0x42)
#define TA1     REG16(0x48)
#define TA2     REG16(0x4A)
#define TA2_H   REG8(0x4B)
#define TA3     REG16(0x4C)
#define TA4     REG16(0x4E)
#define TB0     REG16(0x50)
#define TB1     REG16(0x52)
#define TB2     REG16(0x54)
#define TA1MOD  REG8(0x57)
#define TA2MOD  REG8(0x58)
#define TA3MOD  REG8(0x59)
#define TA4MOD  REG8(0x5A)
#define TB0MOD  REG8(0x5B)
#define TB1MOD  REG8(0x5C)
#define TB2MOD  REG8(0x5D)
#define WDT     REG8(0x60)
#define TA1IC   REG8(0x76)
#define TA2IC   REG8(0x77)
#define TA3IC   REG8(0x78)
#define TA4IC   REG8(0x79)
#define TB0IC   REG8(0x7A)
#define TB1IC   REG8(0x7B)
#define TB2IC   REG8(0x7C)

/* count start flag bits (TCSTART) */
#define TS_TA1 0x02
#define TS_TA2 0x04
#define TS_TA3 0x08
#define TS_TA4 0x10
#define TS_TB0 0x20
#define TS_TB1 0x40
#define TS_TB2 0x80
#define IC_IR  0x08            /* interrupt request bit in xxIC */

extern volatile uint8_t hal_dummy;           /* original: RAM 0x0211 */
void hal_irq_disable(void);                  /* SEI */
void hal_irq_enable(void);                   /* CLI */
void hal_ipl_set(uint8_t level);             /* NC77 idiom PHP/STA 2,S/PLP */

/* I2C: P61 = SCL (push-pull), P62 = SDA (open drain via P6D) - 0x00715B..0x006FA0 */
#define I2C_SCL_HIGH()    (P6 |= 0x02)
#define I2C_SCL_LOW()     (P6 &= (uint8_t)~0x02)
#define I2C_SDA_RELEASE() (P6D &= (uint8_t)~0x04)
#define I2C_SDA_LOW()     (P6 &= (uint8_t)~0x04, P6D |= 0x04)
#define I2C_SDA_READ()    ((P6 >> 2) & 1)
#define I2C_DELAY()       (hal_dummy = 0)

/* MB1502 synthesizer - 0x00D45C */
#define SYNTH_LE(x)       ((x) ? (P7 |= 0x01) : (P7 &= (uint8_t)~0x01))   /* P70 */
#define SYNTH_SELECT(x)   ((x) ? (P8 &= (uint8_t)~0x02) : (P8 |= 0x02))   /* P81, active low */
#define SYNTH_LOCKED()    ((P6 & 0x80) == 0)                              /* P67, 0 = locked (see synth.c) */
#define SER1_OPEN_SYNC()  (U1MOD = 0x01, U1C0 = 0x00, U1BRG = 0x03, U1C1 = 0x01)
#define SER1_PUT(b)       (U1TB = (uint8_t)(b))
#define SER1_WAIT_EMPTY() do { } while (!(U1C1 & 0x02))   /* TX buffer empty */
#define SER1_WAIT_DONE()  do { } while (!(U1C0 & 0x08))   /* TX shift register empty */
#define SER1_CLOSE()      (U1C1 &= (uint8_t)~0x05, U1MOD = 0x00)

#define XSHIFT(x)         ((x) ? (P4 |= 0x08) : (P4 &= (uint8_t)~0x08))   /* P43 crystal shift */
#define WDT_KICK()        (WDT = 0)

#endif
