#ifndef CTCSS_H
#define CTCSS_H
#include <stdint.h>

/* tone numbers are 1..50 in EIA order (see tone_tables.c); 0 = off */
void ctcss_tx_start(uint8_t tone);
void ctcss_tx_stop(void);
void ctcss_tx_reverse(int on);            /* phase-reverse burst at end of TX */
void ctcss_rx_start(uint8_t tone, int initially_present, int hang_enable);
int  ctcss_present(void);                 /* 1 = selected tone detected */
void ctcss_set_hang(uint8_t ticks);

/* hook these to the TB0 and TA4 interrupt vectors */
void ctcss_tb0_isr(void);
void ctcss_ta4_isr(void);
#endif
