/* 
 * File:   keypad.h
 * Author: John
 *
 * Created on 13 May 2019, 9:19 PM
 */
#include <xc.h>


#ifndef KEYPAD_H
#define	KEYPAD_H

#ifdef	__cplusplus
extern "C" {
#endif
extern char keyBuff, keyValue, keyState;

typedef enum{
    KEY_INI = 0,
    KEY_SCAN, 
    KEY_REL
}KEY_STATES;

#define DEBOUNCE_DEL            (80)
#define SHUTDOWN_DEL            (30000)
#define KEYSTUCK_DEL            (60000)
#define BUTTON1                 (0x01)
#define BUTTON2                 (0x02)
#define BUTTON3                 (0x04)
#define BUTTON4                 (0x08)
#define KEYPRESSED              (BUTTON1 || BUTTON2 || BUTTON3 || BUTTON4)
#define KEYSTUCK_DETECTED       (keyValue == 0x10)
#define KEYPRESS_DETECTED      ((PORTCbits.RC5 == 1) || (PORTCbits.RC4 == 1) || (PORTCbits.RC3 == 1) || (PORTAbits.RA9 == 1))
#define NO_KEYS_PRESSED        ((PORTCbits.RC5 == 0) && (PORTCbits.RC4 == 0) && (PORTCbits.RC3 == 0) && (PORTAbits.RA9 == 0))

extern void hmi_scanSetupCommand(char val);
extern void hmi_setupL1KeyCommand();
extern void hmi_setupL2KeyCommand();
extern void hmi_setupL3KeyCommand();
extern void hmi_setupL4KeyCommand();
extern void hmi_setupL5KeyCommand();
extern void hmi_setupL6KeyCommand();

extern void process_KeyController();

#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */

