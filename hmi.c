
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


/* This function is called by hmi_GenerateBootScreen() only 
 * but device_outputState() is called by other functions.
 */
static void lamp_bootSeq()
{
    switch(lmp_state++)
    {
        case 0:
        device_outputState(LAMP_RED,STATE_ON);  
        device_outputState(LAMP_GRN,STATE_OFF); 
            break;

        case (4):
        device_outputState(LAMP_RED,STATE_OFF);  
        device_outputState(LAMP_GRN,STATE_ON); 
            break;

        case (8):
        lmp_state = 0;
            break;
        default:
            break;
    }
}

/* Main screen generation routines for HMI
 * Caller: system_userInterface()
 * MOVE to HMI.c
 */
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

/* Function to configure the system's state 
 * Calls when user interacts with key or when HMI is ready.
 * Move: Should move this to common.h or something so there's no circular dependency
 */
void hmi_ConfigSystemState(char state)
{
    char i;
    t8 = 0;
    t10 = 0;
    i = state;
    uiState = i;
    hmiTask = 0;
    lmp_state = 0;
    scrn_state = 0;
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

/********************************** LAMP! ************************************/

/* Generate lamp imminent sequence 
 * Caller: hmi_lampStackController()
 * Move to: lamp.c probably
 */
static void lampStack_imminentSeq(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        
    if(t10 >= i)
    {   
        t10 = 0;
        switch(lmp_state++)
        {
            case 0:
            device_outputState(LAMP_RED,STATE_OFF);
            device_outputState(LAMP_GRN,STATE_ON);  
                break;

            case (20):
            device_outputState(LAMP_GRN,STATE_OFF);
                break;

            case (40):
            lmp_state = 0;
                break;

            default:
                break;
        }
    }
}

/* Generate lamp fill sequence 
 * Caller: hmi_lampStackController()
 * Move to: lamp.c probably
 */
static void lampStack_fillSeq(unsigned long tmr)
{
    unsigned long i;
    i = tmr;              
    if(t10 >= i)
    {
        t10 = 0;
        switch(lmp_state++)
        {
            case 0:
            device_outputState(LAMP_GRN,STATE_ON);  
                break;

            case (5):
            device_outputState(LAMP_GRN,STATE_OFF);
                break;

            case (10):
            lmp_state = 0;
                break;
                
            default:
                break;
        }
    }
}

/* Generate lamp error sequence 
 * Caller: hmi_lampStackController()
 * Move to: lamp.c or lcd.c probably
 */
static void lampStack_errorSeq(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        
    if(t10 >= i)
    {   
        t10 = 0;
        switch(lmp_state++)
        {
            case 0:
            device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (7):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (10):
            lmp_state = 0;
            //device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (45):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (50):
            device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (55):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (60):
            device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (65):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;
                
            case (70):
            device_outputState(LAMP_RED,STATE_ON);  
                break;
                
            case (75):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (80):
                lmp_state = 0;   
                break;
                
            default:
                break;
        }
    }
}

/* Control lamps 
 * Caller: system_userInterface()
 * Move to: hmi.c
 */
char lmp_update;
void hmi_lampStackController()
{
    switch(runTasks)
    {
        case MC_READY:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
            device_outputState(LAMP_GRN,STATE_ON); 
        }
        break;
            
        case MC_STDBY:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
            device_outputState(LAMP_GRN,STATE_ON); 
        }
        break;
           
        case MC_IMNT:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
        }
        lampStack_imminentSeq(HMI_ANIMATION);
        break;
            
        case MC_FILL:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
        }
        lampStack_fillSeq(HMI_ANIMATION);    
        break;
        
        case MC_CHK:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
        }
        lampStack_fillSeq(HMI_ANIMATION);
        break;
                
        case MC_LID:            // 5
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_GRN,STATE_OFF);
        }
        lampStack_errorSeq(HMI_ANIMATION);
        break;
                
        default:
            break;
    }
}

/********************************** LAMP END *********************************/

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
/* Setup State Timeout - haven't read into this yet.
 * Caller: system_userInterface()
 * Move to: hmi.c
 */
void hmi_SetupStateTimeout(unsigned long timeout)
{
    unsigned long i;
    i = timeout;
    if(t6 >= i)    
    {
        hmi_ConfigSystemState(HMI_RUN);
    }
}

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

