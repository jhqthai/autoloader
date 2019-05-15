// Some description about the file
#include "led.h"


/* This function generate boot sequence for leds
 * Caller: hmi_GenerateBootScreen() */
void led_bootSeq()
{
    static char led_state;
    switch(led_state++)
    {
        case 0:
        LED1_ON();  
        LED2_OFF();  
        LED3_OFF();  
        LED4_OFF();  
        break;

        case 2:
        LED1_OFF();  
        LED2_ON();  
        LED3_OFF();  
        LED4_OFF();    
        break;

        case 4:
        LED1_OFF();  
        LED2_OFF();  
        LED3_ON();  
        LED4_OFF(); 
        break;

        case 6:
        LED1_OFF();  
        LED2_OFF();  
        LED3_OFF();  
        LED4_ON();    
        break;

        case 8:
        LED1_OFF();  
        LED2_OFF();  
        LED3_ON();  
        LED4_OFF(); 
        break;

        case (10):
        LED1_OFF();  
        LED2_ON();  
        LED3_OFF();  
        LED4_OFF(); 
        break;

        case (12):
        led_state = 0;    
            break;
        default:
            break;
    }
}
