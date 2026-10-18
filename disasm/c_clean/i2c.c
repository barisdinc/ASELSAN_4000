/* i2c.c - bit-banged I2C master on P61 (SCL) / P62 (SDA).
 *
 * Shared bus on the K3 board: EEPROM (0xA0), front-panel LCD driver (0x70)
 * and the two front-panel port expanders (0x42, 0x46).
 * Clean rewrite of the original routines; each function names the original
 * address it replaces.  Bit order, ACK handling and pin sequence are the same.
 */
#include "hal.h"
#include "i2c.h"

/* 0x007172 - START: SDA falls while SCL is high, then SCL low. */
void i2c_start(void)
{
    I2C_SDA_RELEASE();
    I2C_SCL_HIGH();
    I2C_SDA_LOW();
    I2C_SCL_LOW();
}

/* 0x00715B - STOP: SDA rises while SCL is high (bus left idle). */
void i2c_stop(void)
{
    I2C_SCL_LOW();
    I2C_SDA_LOW();
    I2C_SCL_HIGH();
    I2C_SDA_RELEASE();
}

/* 0x006A68 (loop) / 0x006AC2 (unrolled) - send one byte MSB first.
 * Returns the ACK bit read from the slave: 0 = ACK, 1 = NACK. */
uint8_t i2c_write(uint8_t b)
{
    for (int8_t bit = 7; bit >= 0; bit--) {
        if (b & (1u << bit)) I2C_SDA_RELEASE();
        else                 I2C_SDA_LOW();
        I2C_SCL_HIGH();
        I2C_SCL_LOW();
    }
    I2C_SDA_RELEASE();           /* let the slave drive ACK */
    I2C_DELAY();
    I2C_SCL_HIGH();
    I2C_DELAY();
    uint8_t ack = I2C_SDA_READ();
    I2C_SCL_LOW();
    return ack;
}

/* 0x006EB3 (unrolled) / 0x006FA0 - receive one byte MSB first.
 * last != 0 -> send NACK (last byte of a read), else ACK. */
uint8_t i2c_read(uint8_t last)
{
    uint8_t v = 0;
    I2C_SDA_RELEASE();
    for (int8_t bit = 7; bit >= 0; bit--) {
        I2C_SCL_HIGH();
        if (I2C_SDA_READ()) v |= (uint8_t)(1u << bit);
        I2C_SCL_LOW();
        I2C_DELAY();
    }
    if (last) I2C_SDA_RELEASE();     /* NACK */
    else      I2C_SDA_LOW();         /* ACK  */
    I2C_SCL_HIGH();
    I2C_DELAY();
    I2C_SCL_LOW();
    return v;
}
