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

/********************************** LCD HMI **********************************/
/* Display temperature to LCD I think
 * Caller: hmi_lcdController()
 * MOVE to lcd.c probably
 */
static void displayTemperatureFunction()
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
 * Move: can maybe move a level lower than hmi.c may into LCD?
 */
static void hmi_GenerateImminentScreen(unsigned long tmr)
{
    char x;
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
 * Move to: Probably lcd.c
 */
static void hmi_GenerateFillScreen(unsigned long tmr)
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
 * Move to: lcd.c maybe
 */
static void hmi_GenerateLidErrorScreen(unsigned long tmr)
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



/* THIS IS THE HIGHER LVL HMI WOOOOWWWW
 * Control what to display on the LCD
 * Caller: system_userInterface()
 * Callees: displayTemperatureFunction(), hmi_GenerateImminentScreen(),
 * hmi_GenerateFillScreen, word2DecConverter(), hmi_GenerateLidErrorScreen()
 * 
 * Variable:
 * runTasks - use by mainly controllers; set by system_eventHandler() 
 * and check_HopperLidClosed()
 */
char lcd_update;
void hmi_lcdController()
{
    char x; // DECLARED BUT NOT USED?
    switch(runTasks)
    {
        case MC_READY:
        if(runTasks != lcd_update)
        {
            t8 = 0;
            scrn_state = 0;
            lcd_update = runTasks;
            strcpy(lcdBuff, lcd_ready);
            lcd_msg = strlen(lcdBuff);
        }
        break;
            
        case MC_STDBY:
        if(runTasks != lcd_update)
        {
            t8 = 0;
            scrn_state = 0;
            lcd_update = runTasks;
            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STANDBY,LCD_CHARS);
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
            refresh_display();
        }
        displayTemperatureFunction();
        break;
           
        case MC_IMNT:
        if(runTasks != lcd_update)
        {
            t8 = 0;
            scrn_state = 0;
            lcd_update = runTasks;
        }
        hmi_GenerateImminentScreen(HMI_ANIMATION);
        break;
            
        case MC_FILL:
        if(runTasks != lcd_update)
        {
            t8 = 0;
            scrn_state = 0;
            lcd_update = runTasks;
        }
        hmi_GenerateFillScreen(HMI_ANIMATION);
        break;
        
        case MC_CHK:
        if(runTasks != lcd_update)
        {
            t8 = 0;
            scrn_state = 0;
            lcd_update = runTasks;
            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_FILL_CNT,LCD_CHARS);
            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
            word2DecConverter(fill_cntr, &lcdBuff[(23)], 5);
            refresh_display();
        }
        break;
                
        case MC_LID:            // 5
        if(runTasks != lcd_update)
        {
            t8 = 0;
            scrn_state = 0;
            lcd_update = runTasks;
        }
        hmi_GenerateLidErrorScreen(HMI_ANIMATION);
        break;
                
        default:
            break;
    }
}
/********************************** LCD HMI END **********************************/

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