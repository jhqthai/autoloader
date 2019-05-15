/* Some description about the file 
 * Two global functions are void hmi_lcdController();
 * and void process_LcdController();
 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/mcc.h" // pin_manager.h
#include "common.h"
#include "lcd.h"
#include "led.h"


// Local LCD prints
const char *lcd_frame   = "   Auto-loader  \xC0     Ver        \x02\0";
const char *lcd_load    = "    Loading     \xC0                \x02\0";
const char *lcd_decode  = "     DECODE     \xC0   AUTOMATION   \x02\0";
const char *config_1602A_2L_Blk = "\x38\x38\x0D\x06\x01\x02\0";     // Configuration of the 1602A LCD display with blink
const char *config_1602A_2LnBLK = "\x38\x38\x0C\x06\x01\x02\0";     // Configuration of the 1602A LCD display with no blink and no underscore
const char *config_1602A_2L_UL  = "\x38\x38\x0E\x06\x01\x02\0";     // Configuration of the 1602A LCD display with underscore
const char *lcd_start   = "  Starting      \xC0                \x02\0";
const char *lcd_setup   = "   SETUP MODE   \xC0<-  (+)  (-)  ->\x02\0";
const char *lcd_fillMax = " Fill Timeout   \xC0                \x02\0";
const char *lcd_lowMax  = " Low Detect     \xC0  Sec           \x02\0";

// Define global LCD prints
const char *lcd_ready   = "  SYSTEM READY  \xC0                \x02\0";
const char *lcd_setP1   = " SET P1         \xC0 Low Debnc  Del \x02\0";
const char *lcd_setP2   = " SET P2         \xC0 Lid Close  Del \x02\0";
const char *lcd_setP3   = " SET P3         \xC0 Drop Away  Del \x02\0";
const char *lcd_setP4   = " SET P4         \xC0 Slide Open Del \x02\0";
const char *lcd_setP5   = " SET P5         \xC0 Full Debnc Del \x02\0";
const char *lcd_setP6   = " SET P6         \xC0 Its Bob & Rosie\x02\0";

// Define global variables
int fill_cntr; // Process fill counter
char lcdBuff[LCD_BUFFER];
char lcdState, lcd_index, lcd_msg = 0;

// Define global message variable
char msgBuff[MSG_BUFFER];

/* This function generates boot sequence for lcd
 * Caller: hmi_GenerateBootScreen() 
 */
void lcd_bootSeq()
{
    switch(scrn_state++)
    {
        case 5:                         // LCD display power-up delay
        strcpy(lcdBuff, lcd_decode);    // Decode Automation
        lcd_msg = strlen(lcdBuff); 
            break;

        case (20):
        strcpy(lcdBuff, lcd_frame);     // Auto-loader
        lcd_msg = strlen(lcdBuff);
            break;

        case (30):
        lcdBuff[(26)] = 'A';
        refresh_display(); 
            break;

        case (40):
        strcpy(lcdBuff, lcd_start);     // Starting 
        lcd_msg = strlen(lcdBuff);
            break;

        case (45):
        lcdBuff[(10)] = '.';
        refresh_display();       
            break;

        case (50):
        lcdBuff[(11)] = '.';
        refresh_display();    
            break;

        case (55):
        lcdBuff[(12)] = '.';
        refresh_display();  
            break;

        case (70):
        LED1_OFF();
        LED2_OFF();
        LED3_OFF();
        LED4_OFF();
        hmiTask++;
            break;

        default:
            break;
    }
}
/********************************** LCD HMI **********************************/
/* Display temperature to LCD
 * Caller: hmi_lcdController()
 */
void displayTemperatureFunction()
{
    char x;
    if((runTasks == MC_STDBY) && (t8 == (5000)))
    {
        x = (inputDevices & INPUT_TEMP_OK);
        if(x != INPUT_TEMP_OK)
        {
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_GLUECOLD,LCD_CHARS);
            refresh_display();
        }
    }
    if((runTasks == MC_STDBY) && (t8 >= (25000)))
    {
        memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
        refresh_display();
        t8 = 0;
    }
}
/* Generate an imminent screen
 * Caller: hmi_lcdController()
 */
void hmi_GenerateImminentScreen(unsigned long tmr)
{
    unsigned long i;
    i = tmr;
    if(t8 >= i)
    {
        t8 = 0;
        switch(scrn_state++)
        {
            case 0:
            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STANDBY,LCD_CHARS);
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_IMMINENT,LCD_CHARS);
            refresh_display();
            break;

            case (7):           // 700mS
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
            refresh_display();        
            break;

            case (10):          // 1 sec
            scrn_state = 0; 
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_IMMINENT,LCD_CHARS);
//            refresh_display();   
            break;
            
            default:
                break;
        }
    }
}

/* Generates Fill Screen
 * Caller: hmi_lcdController()
 */
void hmi_GenerateFillScreen(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        // Multiples of 100mS
    if(t8 >= i)
    {
        t8 = 0;
        switch(scrn_state++)
        {
            case 0:
            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STANDBY,LCD_CHARS);
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_FILLING,LCD_CHARS);
            refresh_display();
            break;

            case (4):
            lcdBuff[(28)] = '>';       // = '>';
            refresh_display();        
            break;

            case (8):     
            lcdBuff[(29)] = '>';       // = '>';
            refresh_display();   
            break;

            case (12):
            lcdBuff[(28)] = ' ';       // = ' ';
            lcdBuff[(30)] = '>';       // = '>';
            refresh_display(); 
            break;

            case (16):
            lcdBuff[(29)] = ' ';       // = ' ';
            lcdBuff[(31)] = '>';       // = '>';
            refresh_display(); 
            break;

            case (20):
            lcdBuff[(30)] = ' ';
            lcdBuff[(32)] = '>';       // = '>';
            refresh_display(); 
            break;

            case (24):
            scrn_state = 0;    
                break;
                
            default:
                break;
        }
    }
}

/*
 * Generate Lid Error Screen
 * Caller: hmi_lcdController()
 */
void hmi_GenerateLidErrorScreen(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        // Multiples of 100mS
    if(t8 >= i)
    {
        t8 = 0;
        switch(scrn_state++)
        {
            case 0:
            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_SYSHALT,LCD_CHARS);
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_LIDOPEN,LCD_CHARS);
            refresh_display();    
                break;

            case (20):
        //    memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_HOPPER,LCD_CHARS);
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
            refresh_display(); 
                break;

            case (25):   
            scrn_state = 0; 
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_LIDOPEN,LCD_CHARS);
//            refresh_display(); 
                break;

            case (40):

            default:
                break;
        }
    }
}


/********************************** LCD HMI END ******************************/

/* This function controls the state of the LCD controller.
 * lcdState - requires by process_OutputDevices
 * This function modify lcd_index, lcd_msg and lcdState which affect LCD_IDLE
 * and LCD_RDY_STATE, LCD_RDY_STATE is required by system_userInterface()
 * 
 * Variables: 
 * lcd_strobe - only used by this function 
 * t7 - only used by this function
 * config_1602A_2LnBLK - only use by this function
 * lcd_index - only use by this function
 * lcdState - use by this function and process_OutputDevices()
 * lcd_msg - use by many other functions
 * lcdbuff - use by many other functions
 * 
 * 
 * Definitions: 
 * LCD_PWRUP - only use by this function
 * 
 */
extern volatile unsigned char lcd_strobe;
void process_LcdController()
{
    char i;
    switch(lcdState)
    {
        case LCD_INITIALIZE:
        if(t7 >= LCD_PWRUP)
        {
            lcd_index = 0, lcd_msg = 0;
            strcpy(lcdBuff, config_1602A_2LnBLK);
            lcd_msg = strlen(lcdBuff);
            lcdState = LCD_CONFIG;
            out_nWR2_SetLow();
            lcd_RS_SetLow();
            lcd_strobe = 0;
            t7 = 0;
        }
        break;
        
        case LCD_CONFIG:
        if(t7 >= (60))
        {
            t7 = 0;
            i = lcdBuff[lcd_index];
            LATB = (i & 0x00FF);
            lcd_strobe = (30);
            if(lcd_index++ >= lcd_msg+1)
            {
                lcd_msg = 0;
                lcd_index = 0;
                lcdState = LCD_RDY;
            }
            out_nWR2_SetHigh();
        }
        if(lcd_strobe == 0)  
        {
            out_nWR2_SetLow();
        }
        break;

        case LCD_RDY:
        if(lcd_msg > 0)
        {
            t7 = 0;
            lcdState = LCD_BUSY;
        }
        break;
            
        case LCD_BUSY:  
        switch(lcd_index)
        {            
            case (16):
            lcd_index++;
            lcd_RS_SetLow();
            i = LCD_LINE2_HM;       // Home display line-2
            LATB = (i & 0x00FF);
            out_nWR2_SetHigh();
            lcdState = LCD_WRITE;
            lcd_strobe = LCD_WR_LATENCY;
            break;
                
            case (33):
            lcd_index++;
            lcd_RS_SetLow();
            i = LCD_LINE1_HM;       // Home display line-1
            LATB = (i & 0x00FF);
            out_nWR2_SetHigh();
            lcdState = LCD_WRITE;
            lcd_strobe = LCD_WR_LATENCY;
            break;
                
            case (34):
            lcd_msg = 0;
            lcd_index = 0;
            lcdState = LCD_RDY;
            break;
                
            default:
            lcd_RS_SetHigh();
            i = lcdBuff[lcd_index++];
            LATB = (i & 0x00FF);
            out_nWR2_SetHigh();
            lcdState = LCD_WRITE;
            lcd_strobe = 1;
            break;
        }
        break;
        
        case LCD_WRITE:
        if(lcd_strobe == 0)  
        {
            out_nWR2_SetLow();
            lcdState = LCD_BUSY;
        }
        break;
    }
}