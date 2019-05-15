
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

/****** Keypad HMI ******/
/* Scan setup command
 * Caller: system_userInterface()
 * Move to: hmi.c probably...
 */
void hmi_scanSetupCommand(char val)
{
    char x, y;
    y = val;
    x = hmi_scanKeypad();
    if(x == y)
    {
        t6 = 0;
        hmi_ConfigSystemState(HMI_SETUP);
        return;
    }
}

/* Move to keypad.c maybe*/
void hmi_setupL1KeyCommand()
{
    char x;
    x = hmi_scanKeypad();
    if(x > 0) 
    {
        switch(x)
        {
            case BUTTON1:
            hmi_ConfigSystemState(HMI_RUN);
                break;
                
            case BUTTON2:
            P1_value++;
            strcpy(lcdBuff, lcd_setP1);
            word2DecConverter(P1_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);      
                break;
                
            case BUTTON3:
            P1_value--;
            strcpy(lcdBuff, lcd_setP1);
            word2DecConverter(P1_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);    
                break;
                       
            case BUTTON4:
            hmiTask++;    
            strcpy(lcdBuff, lcd_setP2);
            word2DecConverter(P2_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);  
            break;
                
            default:                
                break;
        }
    }
}

void hmi_setupL2KeyCommand()
{
    char x;
    x = hmi_scanKeypad();
    if(x > 0)
    {
        switch(x)
        {
            case BUTTON1:
            hmi_ConfigSystemState(HMI_RUN);  
                break;
                
            case BUTTON2:
            P2_value++;  
            strcpy(lcdBuff, lcd_setP2);
            word2DecConverter(P2_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);   
                break;
                
            case BUTTON3:
            P2_value--;    
            strcpy(lcdBuff, lcd_setP2);
            word2DecConverter(P2_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);     
                break;
                       
            case BUTTON4:
            hmiTask++;    
            strcpy(lcdBuff, lcd_setP3);
            word2DecConverter(P3_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff); 
                break;
                
            default:                
                break;
        }
    }
}

void hmi_setupL3KeyCommand()
{
    char x;
    x = hmi_scanKeypad();
    if(x > 0)
    {
        switch(x)
        {
            case BUTTON1:
            hmi_ConfigSystemState(HMI_RUN); 
                break;
                
            case BUTTON2:
            P3_value++;    
            strcpy(lcdBuff, lcd_setP3);
            word2DecConverter(P3_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);   
                break;
                
            case BUTTON3:
            P3_value--;    
            strcpy(lcdBuff, lcd_setP3);
            word2DecConverter(P3_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);      
                break;
                       
            case BUTTON4:
            hmiTask++;    
            strcpy(lcdBuff, lcd_setP4);
            word2DecConverter(P4_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff); 
                break;
                
            default:                
                break;
        }
    }
}

void hmi_setupL4KeyCommand()
{
    char x;
    x = hmi_scanKeypad();
    if(x > 0)
    {
        switch(x)
        {
            case BUTTON1:
            hmi_ConfigSystemState(HMI_RUN);
                break;
                
            case BUTTON2:
            P4_value++;    
            strcpy(lcdBuff, lcd_setP4);
            word2DecConverter(P4_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);   
                break;
                
            case BUTTON3:
            P4_value--;    
            strcpy(lcdBuff, lcd_setP4);
            word2DecConverter(P4_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);      
                break;
                       
            case BUTTON4:
            hmiTask++;    
            strcpy(lcdBuff, lcd_setP5);
            word2DecConverter(P5_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff); 
                break;
                
            default:                
                break;
        }
    }
}

void hmi_setupL5KeyCommand()
{
    char x;
    x = hmi_scanKeypad();
    if(x > 0)
    {
        switch(x)
        {
            case BUTTON1:
            hmi_ConfigSystemState(HMI_RUN);
                break;
                
            case BUTTON2:
            P5_value++;    
            strcpy(lcdBuff, lcd_setP5);
            word2DecConverter(P5_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);   
                break;
                
            case BUTTON3:
            P5_value--;    
            strcpy(lcdBuff, lcd_setP5);
            word2DecConverter(P5_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);      
                break;
                       
            case BUTTON4:
            hmiTask++;    
            strcpy(lcdBuff, lcd_setP6);
            word2DecConverter(P6_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff); 
                break;
                
            default:                
                break;
        }
    }
}

void hmi_setupL6KeyCommand()
{
    char x;
    x = hmi_scanKeypad();
    if(x > 0)
    {
        switch(x)
        {
            case BUTTON1:
            hmi_ConfigSystemState(HMI_RUN);
            break;
                
            case BUTTON2:
            P6_value++;    
            strcpy(lcdBuff, lcd_setP6);
            word2DecConverter(P6_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);   
            break;
                
            case BUTTON3:
            P6_value--;    
            strcpy(lcdBuff, lcd_setP6);
            word2DecConverter(P6_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff);      
                break;
                       
            case BUTTON4:
            hmiTask = 1;    
            strcpy(lcdBuff, lcd_setP1);
            word2DecConverter(P1_value, &lcdBuff[(12)], 4);
            lcd_msg = strlen(lcdBuff); 
                break;
                
            default:                
                break;
        }
    }
}


