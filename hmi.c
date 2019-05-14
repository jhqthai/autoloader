
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
//-------------------------------------------------------------------
// Local Includes
//-------------------------------------------------------------------
#include "common.h"
#include "hmi.h"
#include "keypad.h"
#include "lcd.h"
#include "led.h"
#include "lamp.h"



void hmi_GenerateBootScreen(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        // Multiples of 100mS
    if(t8 >= i)
    {
        t8 = 0;
        led_bootSeq();
        lcd_bootSeq();
        lamp_bootSeq();
    }
}


/* Load default state maybe? */
void load_defaults()
{
    P1_value = DEFAULT_P1;
    P2_value = DEFAULT_P2;
    P3_value = DEFAULT_P3;
    P4_value = DEFAULT_P4;
    P5_value = DEFAULT_P5;
    P6_value = DEFAULT_P6;
}


void hmi_SetupStateTimeout(unsigned long timeout)
{
    unsigned long i;
    i = timeout;
    if(t6 >= i)    
    {
        hmi_ConfigSystemState(HMI_RUN);
    }
}


