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

typedef enum{
    KEY_INI = 0,
    KEY_SCAN, 
    KEY_REL
}KEY_STATES;

// Global keypad parameters
#define BUTTON1                 (0x01)
#define BUTTON2                 (0x02)
#define BUTTON3                 (0x04)
#define BUTTON4                 (0x08)
#define KEYPRESSED              (BUTTON1 || BUTTON2 || BUTTON3 || BUTTON4)

// Declare global functions
extern unsigned char hmi_scanKeypad(void);
extern void process_KeyController();

#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */

