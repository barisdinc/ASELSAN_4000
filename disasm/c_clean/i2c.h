#ifndef I2C_H
#define I2C_H
#include <stdint.h>

/* 8-bit I2C addresses used on the radio */
#define I2C_ADDR_EEPROM   0xA0   /* 24C16-style 2 KB EEPROM, page bits in the address */
#define I2C_ADDR_LCD      0x70   /* K4 TD1 LCD driver (PCF8576-type commands) */
#define I2C_ADDR_PANEL_A  0x42   /* K4 port expander (keypad) */
#define I2C_ADDR_PANEL_B  0x46   /* K4 port expander (PTT/on-off/monitor, LEDs) */

void    i2c_start(void);
void    i2c_stop(void);
uint8_t i2c_write(uint8_t b);         /* returns 0 = ACK */
uint8_t i2c_read(uint8_t last);       /* last != 0 -> NACK */
#endif
