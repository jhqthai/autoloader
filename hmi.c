
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

// LED States
#define SYSTEM_RDY        (sysStatus = 0x01)
#define LED1_ON()         (LATDbits.LATD8  = 1)
#define LED2_ON()         (LATCbits.LATC10 = 1)
#define LED3_ON()         (LATCbits.LATC12 = 1)
#define LED4_ON()         (LATCbits.LATC15 = 1)
#define LED1_OFF()        (LATDbits.LATD8  = 0)
#define LED2_OFF()        (LATCbits.LATC10 = 0)
#define LED3_OFF()        (LATCbits.LATC12 = 0)
#define LED4_OFF()        (LATCbits.LATC15 = 0)
#define ALL_LEDS_ON()    ((LATDbits.LATD8  = 1) && (LATCbits.LATC10 = 1) && (LATCbits.LATC12 = 1) && (LATCbits.LATC15 = 1))


/* This function is called by hmi_GenerateBootScreen() */
static void led_bootSeq()
{
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

/* This function is called by hmi_GenerateBootScreen() only */
static void lcd_bootSeq()
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

///********************************** LCD! ************************************/
///* Display temperature to LCD I think
// * Caller: hmi_lcdController()
// * MOVE to lcd.c probably
// */
//static void displayTemperatureFunction()
//{
//    char x;
//    if((runTasks == MC_STDBY) && (t8 == (5000)))
//    {
//        x = (inputDevices & INPUT_TEMP_OK);
//        if(x != INPUT_TEMP_OK)
//        {
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_GLUECOLD,LCD_CHARS);
//            refresh_display();
//        }
//    }
//    if((runTasks == MC_STDBY) && (t8 >= (25000)))
//    {
//        memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
//        refresh_display();
//        t8 = 0;
//    }
//}
///* Generate an imminent screen
// * Caller: hmi_lcdController()
// * Move: can maybe move a level lower than hmi.c may into LCD?
// */
//static void hmi_GenerateImminentScreen(unsigned long tmr)
//{
//    char x;
//    unsigned long i;
//    i = tmr;
//    if(t8 >= i)
//    {
//        t8 = 0;
//        switch(scrn_state++)
//        {
//            case 0:
//            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STANDBY,LCD_CHARS);
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_IMMINENT,LCD_CHARS);
//            refresh_display();
//            break;
//
//            case (7):           // 700mS
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
//            refresh_display();        
//            break;
//
//            case (10):          // 1 sec
//            scrn_state = 0; 
////            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_IMMINENT,LCD_CHARS);
////            refresh_display();   
//            break;
//            
//            default:
//                break;
//        }
//    }
//}
//
///* Generates Fill Screen
// * Caller: hmi_lcdController()
// * Move to: Probably lcd.c
// */
//static void hmi_GenerateFillScreen(unsigned long tmr)
//{
//    unsigned long i;
//    i = tmr;                                        // Multiples of 100mS
//    if(t8 >= i)
//    {
//        t8 = 0;
//        switch(scrn_state++)
//        {
//            case 0:
//            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STANDBY,LCD_CHARS);
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_FILLING,LCD_CHARS);
//            refresh_display();
//            break;
//
//            case (4):
//            lcdBuff[(28)] = '>';       // = '>';
//            refresh_display();        
//            break;
//
//            case (8):     
//            lcdBuff[(29)] = '>';       // = '>';
//            refresh_display();   
//            break;
//
//            case (12):
//            lcdBuff[(28)] = ' ';       // = ' ';
//            lcdBuff[(30)] = '>';       // = '>';
//            refresh_display(); 
//            break;
//
//            case (16):
//            lcdBuff[(29)] = ' ';       // = ' ';
//            lcdBuff[(31)] = '>';       // = '>';
//            refresh_display(); 
//            break;
//
//            case (20):
//            lcdBuff[(30)] = ' ';
//            lcdBuff[(32)] = '>';       // = '>';
//            refresh_display(); 
//            break;
//
//            case (24):
//            scrn_state = 0;    
//                break;
//                
//            default:
//                break;
//        }
//    }
//}
//
///*
// * Generate Lid Error Screen
// * Caller: hmi_lcdController()
// * Move to: lcd.c maybe
// */
//static void hmi_GenerateLidErrorScreen(unsigned long tmr)
//{
//    unsigned long i;
//    i = tmr;                                        // Multiples of 100mS
//    if(t8 >= i)
//    {
//        t8 = 0;
//        switch(scrn_state++)
//        {
//            case 0:
//            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_SYSHALT,LCD_CHARS);
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_LIDOPEN,LCD_CHARS);
//            refresh_display();    
//                break;
//
//            case (20):
//        //    memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_HOPPER,LCD_CHARS);
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
//            refresh_display(); 
//                break;
//
//            case (25):   
//            scrn_state = 0; 
////            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_LIDOPEN,LCD_CHARS);
////            refresh_display(); 
//                break;
//
//            case (40):
//
//            default:
//                break;
//        }
//    }
//}
//
//
//
///* THIS IS THE HIGHER LVL HMI WOOOOWWWW
// * Control what to display on the LCD
// * Caller: system_userInterface()
// * Callees: displayTemperatureFunction(), hmi_GenerateImminentScreen(),
// * hmi_GenerateFillScreen, word2DecConverter(), hmi_GenerateLidErrorScreen()
// * 
// * Variable:
// * runTasks - use by mainly controllers; set by system_eventHandler() 
// * and check_HopperLidClosed()
// */
//char lcd_update;
//void hmi_lcdController()
//{
//    char x; // DECLARED BUT NOT USED?
//    switch(runTasks)
//    {
//        case MC_READY:
//        if(runTasks != lcd_update)
//        {
//            t8 = 0;
//            scrn_state = 0;
//            lcd_update = runTasks;
//            strcpy(lcdBuff, lcd_ready);
//            lcd_msg = strlen(lcdBuff);
//        }
//        break;
//            
//        case MC_STDBY:
//        if(runTasks != lcd_update)
//        {
//            t8 = 0;
//            scrn_state = 0;
//            lcd_update = runTasks;
//            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STANDBY,LCD_CHARS);
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
//            refresh_display();
//        }
//        displayTemperatureFunction();
//        break;
//           
//        case MC_IMNT:
//        if(runTasks != lcd_update)
//        {
//            t8 = 0;
//            scrn_state = 0;
//            lcd_update = runTasks;
//        }
//        hmi_GenerateImminentScreen(HMI_ANIMATION);
//        break;
//            
//        case MC_FILL:
//        if(runTasks != lcd_update)
//        {
//            t8 = 0;
//            scrn_state = 0;
//            lcd_update = runTasks;
//        }
//        hmi_GenerateFillScreen(HMI_ANIMATION);
//        break;
//        
//        case MC_CHK:
//        if(runTasks != lcd_update)
//        {
//            t8 = 0;
//            scrn_state = 0;
//            lcd_update = runTasks;
//            memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_FILL_CNT,LCD_CHARS);
//            memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
//            word2DecConverter(fill_cntr, &lcdBuff[(23)], 5);
//            refresh_display();
//        }
//        break;
//                
//        case MC_LID:            // 5
//        if(runTasks != lcd_update)
//        {
//            t8 = 0;
//            scrn_state = 0;
//            lcd_update = runTasks;
//        }
//        hmi_GenerateLidErrorScreen(HMI_ANIMATION);
//        break;
//                
//        default:
//            break;
//    }
//}
///********************************** LCD END **********************************/

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

