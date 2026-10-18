#ifndef TONE_TABLES_H
#define TONE_TABLES_H
#include <stdint.h>

#define CTCSS_TONES 50
typedef struct { uint16_t tx_div; uint8_t tx_inc; uint16_t rx_div; } ctcss_entry_t;
typedef struct { uint16_t tb1_div; uint8_t inc_low, inc_high; } dtmf_entry_t;
typedef struct { const char *name; uint8_t tone_ms; uint16_t tb1_div[15]; uint8_t inc[15]; } selcall_std_t;

extern const uint8_t       ctcss_sine[256];
extern const ctcss_entry_t ctcss_table[CTCSS_TONES];
extern const dtmf_entry_t  dtmf_table[12];
extern const uint16_t      dtmf_sine_low[256], dtmf_sine_high[256];
extern const uint16_t      selcall_sine[256];
extern const selcall_std_t selcall_std[3];
#endif
