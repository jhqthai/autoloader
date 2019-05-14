/* 
 * File:   led.h
 * Author: John
 *
 * Created on 14 May 2019, 12:53 PM
 */
#include <xc.h>

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

// LED States
#define SYSTEM_RDY        (sysStatus = 0x01)
#define LED1_ON()         (LATDbits.LATD8  = 1)
#define LED2_ON()         (LATCbits.LATC10 = 1)
#define LED3_ON()         (LATCbits.LATC12 = 1)
#define LED4_ON()         (LATCbits.LATC15 = 1)
#define LED1_OFF()        (LATDbits.LATD8  = 0)
#define LED2_OFF()        (LATCbits.LATC10 = 0)
#define LED3_OFF()        (LATCbits.LATC12 = 0)
#define LED4_OFF()        (LATCbits.LATC15 = 0)
#define ALL_LEDS_ON()    ((LATDbits.LATD8  = 1) && (LATCbits.LATC10 = 1) && (LATCbits.LATC12 = 1) && (LATCbits.LATC15 = 1))

extern char led_state; // TODO: confirm if led_state is controlled by other function beside the one in led.c

extern void led_bootSeq();


#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

