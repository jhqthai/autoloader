/*
 * Keypad driver
 */
#include "common.h"
#include "keypad.h"
#include "backlight.h"

char keyBuff, keyValue, keyState = 0;

// TODO: unused function!!!!????
static char read_KeyBuff()      // KEYPRESSED
{
    if(keyValue > 0)
    {
        return keyValue;
    }
    else
    {
        return 0;
    }
}

void process_KeyController()
{
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


