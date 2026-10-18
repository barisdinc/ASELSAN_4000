#ifndef TONES_H
#define TONES_H
#include <stdint.h>

enum { SELCALL_CCIR = 0, SELCALL_ZVEI1 = 1, SELCALL_EEA = 2 };

void    tone_start_dtmf(uint8_t key, uint16_t ms);     /* key 0..11 = 1..9,*,0,# */
void    tone_start_selcall(uint8_t std, uint8_t digit); /* digit 0..14 */
int     tone_done(void);
void    tone_stop(void);
void    tones_tb1_isr(void);                            /* hook to TB1 vector in TX */
uint8_t selcall_apply_repeat(const uint8_t *in, uint8_t n, uint8_t *out);
void    selcall_send(uint8_t std, const uint8_t *digits, uint8_t n);
#endif
