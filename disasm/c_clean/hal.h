/* hal.h - hardware abstraction used by the clean drivers.
 *
 * Every macro maps to one K3-board signal (pin names from the 4021/4025
 * maintenance manual, section 3.6.2.3).  hal_m37732.h implements them for the
 * original M37732S4 processor exactly as the original firmware does; a port to
 * another MCU (e.g. an MSP430 carrier board) only has to re-implement this file.
 */
#ifndef HAL_H
#define HAL_H
#include <stdint.h>

#if defined(HAL_CUSTOM)
#include "hal_custom.h"
#else
#include "hal_m37732.h"
#endif

/* Required by the drivers (provided by hal_*.h):
 *
 *   I2C (bit-banged, open drain)       P61 SCL, P62 SDA
 *     I2C_SCL_HIGH() I2C_SCL_LOW()
 *     I2C_SDA_RELEASE()  SDA = input, pulled high
 *     I2C_SDA_LOW()      SDA = output, driven low
 *     I2C_SDA_READ()     -> 0/1
 *     I2C_DELAY()        short delay (original: dummy write to RAM 0x0211)
 *
 *   Synthesizer MB1502                 P70 LE, P81 select, P67 lock
 *     SYNTH_LE(x) SYNTH_SELECT(x) SYNTH_LOCKED()
 *     SER1_OPEN_SYNC() SER1_PUT(b) SER1_WAIT_EMPTY() SER1_WAIT_DONE() SER1_CLOSE()
 *       (UART1 in clock-synchronous mode, shared with C-BUS)
 *
 *   Tone timers                        P54 TKS PWM (TA2), P52 SC tone (TA1),
 *                                      P65 TKS in (TB0/TA4), P66 SC in (TB1)
 *     see ctcss.c / tones.c
 */
#endif
