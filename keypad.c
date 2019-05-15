/*
 * Keypad driver
 */
#include <string.h>

#include "common.h"
#include "keypad.h"
#include "backlight.h"
#include "lcd.h"

// Local process parameters
#define DEBOUNCE_DEL            (80)
#define SHUTDOWN_DEL            (30000)
#define KEYSTUCK_DEL            (60000)
#define KEYSTUCK_DETECTED       (keyValue == 0x10)
#define KEYPRESS_DETECTED      ((PORTCbits.RC5 == 1) || (PORTCbits.RC4 == 1) || (PORTCbits.RC3 == 1) || (PORTAbits.RA9 == 1))
#define NO_KEYS_PRESSED        ((PORTCbits.RC5 == 0) && (PORTCbits.RC4 == 0) && (PORTCbits.RC3 == 0) && (PORTAbits.RA9 == 0))

// Local variables
static char keyValue = 0;

/* Scan for keypad input
 * Caller: hmi_scanSetupCommand()
 */
unsigned char hmi_scanKeypad(void)
{
    char x;
    if(keyValue > 0)
    {
        x = keyValue; 
        keyValue = 0;
        return (x);
    }
    //return -1; //? maybe
}

/* This function controls key state
 * Caller: main
 */
void process_KeyController()
{
    static char keyState, keyBuff = 0;
    switch(keyState)
    {
        case KEY_INI:
        t12 = 0;
        keyBuff  = 0;
        keyValue = 0;
        keyState = KEY_SCAN;
        break;
        
        case KEY_SCAN:
        if(KEYPRESS_DETECTED)
        {
            if(t12 >= DEBOUNCE_DEL)
            {
                set_backlight_level(BACKLIGHT10);
                keyState = KEY_REL;
                if(PORTCbits.RC5 == 1)
                {
                    keyBuff |=  0x01;
                }
                else
                {
                    keyBuff &= ~0x01;
                }
                if(PORTCbits.RC4 == 1)
                {
                    keyBuff |=  0x02;
                }
                else
                {
                    keyBuff &= ~0x02;
                }
                if(PORTCbits.RC3 == 1)
                {
                    keyBuff |=  0x04;
                }
                else
                {
                    keyBuff &= ~0x04;
                }
                if(PORTAbits.RA9 == 1)
                {
                    keyBuff |=  0x08;
                }
                else
                {
                    keyBuff &= ~0x08;
                }
            }
        }
        else
        {
            t12 = 0;
        }
        break;
            
        case KEY_REL:
        if(NO_KEYS_PRESSED)
        {
            t12 = 0;
            keyValue = keyBuff;
            keyState = KEY_SCAN;
        }
        if(t12 >= KEYSTUCK_DEL)
        {
            keyValue |= 0x10;
        }
        break;
    }
}


