#ifndef SYNTH_H
#define SYNTH_H
#include <stdint.h>
#include "eeprom.h"

typedef struct {
    uint8_t r[2];     /* reference word bytes in transmit order (RAM 0x0216, 0x0217) */
    uint8_t n[3];     /* divider word bytes in transmit order (RAM 0x0213, 0x0212, 0x0208) */
} synth_words_t;

/* Band configuration, set up at power on from EEPROM 0x58B.. (F_00DA32) */
typedef struct {
    uint16_t base;        /* band base in 25 kHz units (RAM 0x0224) */
    int16_t  rx_offset;   /* RX injection in 2.5 kHz units: +18000 (VHF) or -18000 (RAM 0x01F9) */
    uint8_t  prescaler;   /* 64 or 128 (RAM 0x020B) */
    uint32_t fref_param;  /* reference / 100: 128000 (12.8 MHz) or 130000 (13.0 MHz) (RAM 0x01F5) */
} synth_cfg_t;

void     synth_cfg_from_eeprom(uint16_t base, uint8_t refsel, synth_cfg_t *cfg);
void     synth_compute(const synth_cfg_t *cfg, uint16_t word, int16_t offset, synth_words_t *w);
void     synth_words_for_channel(const synth_cfg_t *cfg, const channel_t *ch, int tx, synth_words_t *w);
uint32_t synth_word_to_hz(const synth_cfg_t *cfg, uint16_t word);
void     synth_load(const synth_words_t *w);
uint8_t  synth_lock_stable(uint8_t mode, uint16_t timeout_ticks);
#endif
