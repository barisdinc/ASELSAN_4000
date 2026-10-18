/* eeprom.c - 2 KB I2C EEPROM (24C16 addressing).
 *
 * 11-bit address: bits 8..10 go into the device byte (0xA0 | page << 1),
 * bits 0..7 are the word address.  The original firmware raises the CPU
 * interrupt priority level to 1 for the whole transaction (NC77 idiom
 * "LDA #1 / PHP / STA 2,S / PLP") so that the 32 ms tick cannot stretch the bus.
 *
 * Original routines: 0x007189 read byte, 0x007271 / 0x0071F0 read word,
 * 0x0072EE / 0x0073C6 generic block transfer.  Writes are implemented here
 * in the standard 24C16 way (page write + ACK polling); compare with 0x0073C6
 * before relying on the exact timing.
 */
#include "hal.h"
#include "i2c.h"
#include "eeprom.h"

static uint8_t dev(uint16_t addr) { return (uint8_t)(I2C_ADDR_EEPROM | ((addr >> 8) & 7) << 1); }

static void begin_read(uint16_t addr)
{
    i2c_stop();                       /* original always starts with STOP to reset the bus */
    i2c_start();
    i2c_write(dev(addr));
    i2c_write((uint8_t)addr);
    i2c_start();                      /* repeated start */
    i2c_write(dev(addr) | 1);
}

/* 0x007189 */
uint8_t eeprom_read_byte(uint16_t addr)
{
    hal_ipl_set(1);
    begin_read(addr);
    uint8_t v = i2c_read(1);
    i2c_stop();
    hal_ipl_set(0);
    return v;
}

/* 0x007271 - little endian word */
uint16_t eeprom_read_word(uint16_t addr)
{
    hal_ipl_set(1);
    begin_read(addr);
    uint16_t lo = i2c_read(0);
    uint16_t hi = i2c_read(1);
    i2c_stop();
    hal_ipl_set(0);
    return (uint16_t)(lo | hi << 8);
}

void eeprom_read(uint16_t addr, uint8_t *buf, uint16_t n)
{
    if (!n) return;
    hal_ipl_set(1);
    begin_read(addr);
    while (n--) *buf++ = i2c_read(n == 0);
    i2c_stop();
    hal_ipl_set(0);
}

/* Page write (16-byte pages on 24C16) followed by ACK polling. */
int eeprom_write(uint16_t addr, const uint8_t *buf, uint16_t n)
{
    while (n) {
        uint16_t room = 16 - (addr & 15);
        uint16_t chunk = n < room ? n : room;
        hal_ipl_set(1);
        i2c_start();
        if (i2c_write(dev(addr))) { i2c_stop(); hal_ipl_set(0); return -1; }
        i2c_write((uint8_t)addr);
        for (uint16_t k = 0; k < chunk; k++) i2c_write(buf[k]);
        i2c_stop();
        hal_ipl_set(0);
        for (int tries = 0; tries < 1000; tries++) {      /* write cycle ~10 ms */
            WDT_KICK();
            i2c_start();
            uint8_t nack = i2c_write(dev(addr));
            i2c_stop();
            if (!nack) break;
        }
        addr += chunk; buf += chunk; n -= chunk;
    }
    return 0;
}

/* 0x0087A7 - channel n is programmed unless its RX word reads 0x3FFF
 * (the original reads the word at 0x30 + (n-1)*13 = record + 2) */
int channel_is_programmed(uint8_t n)
{
    return (eeprom_read_word(EE_CHANNEL(n) + 2) & 0x3FFF) != CH_WORD_EMPTY;
}

/* 0x008A47 reads the whole 13-byte record */
int channel_load(uint8_t n, channel_t *ch)
{
    if (n < 1 || n > EE_CHANNELS || !channel_is_programmed(n)) return -1;
    eeprom_read(EE_CHANNEL(n), (uint8_t *)ch, sizeof *ch);
    return 0;
}

/* TX CTCSS tone, assembled from bit 7 of name[1..6] when bit 7 of name[0] is set (F_008A47) */
uint8_t channel_tx_tone(const channel_t *c)
{
    uint8_t t = 0;
    if (!(c->name[0] & 0x80)) return 0;
    for (int i = 1; i <= 6; i++) t = (uint8_t)(t << 1 | c->name[i] >> 7);
    return t;
}

/* F_00DCFD: the four checksummed blocks; a failure shows message 3 "ARZ PRG" */
int eeprom_checksums_ok(void)
{
    static const uint16_t blk[4][2] = EE_SUM_BLOCKS;
    for (int k = 0; k < 4; k++) {
        uint16_t sum = 0, a = blk[k][0], n = blk[k][1];
        for (uint16_t i = 0; i < n; i++) sum += eeprom_read_byte(a + i);
        uint16_t stored = (uint16_t)(eeprom_read_byte(a + n) << 8 | eeprom_read_byte(a + n + 1));
        if (stored != sum) return 0;
    }
    return 1;
}
