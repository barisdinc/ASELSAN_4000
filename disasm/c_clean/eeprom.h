#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>

#define EE_SIZE        2048
#define EE_CHANNELS    99
#define EE_CHANNEL_LEN 13
#define EE_CHANNEL(n)  ((uint16_t)(0x2E + ((n) - 1) * EE_CHANNEL_LEN))   /* F_005B75, F_008A47 */

/* 13-byte channel record at EEPROM 0x2E + (n-1)*13 (F_008A47 / F_00899E).
 * It holds 14-bit frequency words, not synthesizer words: the radio computes
 * the MB1502 words itself (see synth.c).  f = (base*10 + word) * 2.5 kHz. */
typedef struct {
    uint8_t tx_hi;       /* +0  b5..0 TX word bits 13..8, b7/b6 -> RAM 0x037C b7/b6 */
    uint8_t tx_lo;       /* +1  TX word bits 7..0 */
    uint8_t rx_hi;       /* +2  b5..0 RX word bits 13..8 */
    uint8_t rx_lo;       /* +3  RX word bits 7..0 */
    uint8_t rx_flags;    /* +4  b5..0 RX CTCSS tone 1..50 (0 = off), b6 crystal shift, b7 selcall (RAM 0x0375) */
    uint8_t name[8];     /* +5..+12 channel name, 8 chars, 7-bit ASCII -> RAM 0x024A (F_008A47).
                            Only A-Z 0-9 space _ - / are accepted; any other char and the
                            radio shows no name.  Bit 7 of +5 enables the TX tone,
                            bit 7 of +6..+11 = TX CTCSS tone, MSB first (RAM 0x037C) */
} channel_t;

#define CH_WORD_EMPTY   0x3FFF
#define CH_TX_WORD(c)   ((uint16_t)(((c)->tx_hi & 0x3F) << 8 | (c)->tx_lo))
#define CH_RX_WORD(c)   ((uint16_t)(((c)->rx_hi & 0x3F) << 8 | (c)->rx_lo))
#define CH_TONE(f)      ((f) & 0x3F)
#define CH_XSHIFT(f)    (((f) >> 6) & 1)
#define CH_SELCALL(f)   (((f) >> 7) & 1)
uint8_t channel_tx_tone(const channel_t *c);       /* 0 = off, 1..50 */

/* Radio configuration block, EEPROM 0x58B (50 bytes, read by F_00DA32) */
#define EE_CFG          0x58B
#define EE_CFG_OPTIONS  (EE_CFG + 0)   /* RAM 0x0335 */
#define EE_CFG_BASE     (EE_CFG + 1)   /* band base, big endian, 25 kHz units -> RAM 0x0224 */
#define EE_CFG_REFSEL   (EE_CFG + 4)   /* != 0: 13.0 MHz reference, else 12.8 MHz */
#define EE_PWR_INIT     0x591          /* 10 bytes: initial TX power PWM (TA3) */
#define EE_PWR_TARGET1  0x59B          /* 10 bytes: AN7 targets */
#define EE_PWR_TARGET2  0x5A5          /* 10 bytes */
/* Radio parameters EEPROM 0x018..0x02D -> RAM 0x0340..0x0355 */
#define EE_PAR          0x018          /* b1..0 selcall standard (1 CCIR, 2 ZVEI1, 3 EEA), b7 language */
/* Checksummed blocks: 16-bit byte sum stored big endian after the block (F_00DCFD) */
#define EE_SUM_BLOCKS   { {0x578, 0x45}, {0x000, 0x05}, {0x008, 0x0E}, {0x018, 0x51D} }

uint8_t  eeprom_read_byte(uint16_t addr);
uint16_t eeprom_read_word(uint16_t addr);
void     eeprom_read(uint16_t addr, uint8_t *buf, uint16_t n);
int      eeprom_write(uint16_t addr, const uint8_t *buf, uint16_t n);
int      channel_is_programmed(uint8_t n);
int      channel_load(uint8_t n, channel_t *ch);
int      eeprom_checksums_ok(void);
#endif
