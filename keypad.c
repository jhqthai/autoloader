/*
 * Keypad driver
 */
#include <string.h>

#include "common.h"
#include "keypad.h"
#include "backlight.h"
#include "lcd.h"

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
/********************************** Keypad ***********************************/
/* Scan for keypad input
 * Caller: hmi_scanSetupCommand()
 * Move to: keypad.c later probably or not..
 */
static unsigned char hmi_scanKeypad(void)
{
    char x;
    if(keyValue > 0)
    {
        x = keyValue; 
        keyValue = 0;
        return (x);
    }
}

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

/********************************** End of Keypad HMI **********************/


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


