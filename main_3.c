// The file that I rip to pieces

#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "comm.h"
#include "hmi.h"
#include "keypad.h"
#include "backlight.h"
#include "tone.h"
#include "lcd.h"
#include "lamp.h"

/* Local main variables */
// Event handler variables
char sysState;
//// Toggle state variables
//char lamp_flash_count;

//// Communication variables 
//char comTasks, cmdTasks, rxIndx, txIndx, rxMsg, txMsg;

//// Serial comm's 
//#define TX_BUFFER       (128)
//#define RX_BUFFER       (255)
//#define UART1_BRG       (((FCY / UART1_BAUD)/16) - 1)
//char txbuff[TX_BUFFER], rxbuff[RX_BUFFER];

// Operational parameters
#define ONE_HERTZ       (10000)  
#define TWO_HERTZ       (5000)
#define SETUP_TIMEOUT   (600000)

#define SCAN_PERIOD     (1000)          // 100mS debounce on input port.
#define MIN_FILLTIME    (30000)         // Minimum delay on fill solenoid activation
#define RLY_CHG_WT      (10000)
#define VALID_GLUELOW   (30000)         // Will be changed in menu
#define VALID_GLUEFILL  (50000)
#define VALID_FILLERR   (600000)        // Hopper fill error timeout - to be save in NV

//// Serial comm. states
//typedef enum{
//    INI = 0,
//    RDY,
//    BUSY
//}COM_STATE;

//// Command intepreter states
//typedef enum{
//    CONNECT = 0,
//    MENU,
//    IO,
//    SEND,
//    ERROR
//}HOST_CMD;

// Process control
#define PROCESS_DELAY   (30000)
#define SETUP_DELAY     (30000)
#define FILL_TIMEOUT    (100000)
#define MIN_FILL_DEL    (20000)
#define CMD_SETUP       'S'
#define DEBOUNCE_LID    (10000)

// HB LED flash times
#define HB_LED_INI      (1000)
#define HB_LED_RUN      (10000)

#define CLEAR_PORT      0x00
#define RLY1            0x01
#define RLY2            0x02
#define RLY3            0x04
#define RLY4            0x08
#define RLY5            0x10
#define RLY6            0x20
#define RLY7            0x40
#define RLY8            0x80

#define BUS_IDLE        (buff == outputDevices)
#define BUS_TRANSACTION (buff != outputDevices)

// System States
#define STATUS_SYS_RDY      0x0001
#define STATUS_TEMP_OK      0x0002
#define STATUS_FILLERR      0x0004
#define STATUS_LIDCLOSED    0x0008
#define STATUS_STANDBY      0x0010
#define STATUS_PRE_LOAD     0x0020
#define STATUS_HOPRFILL     0x0040
#define STATUS_RUNMODE      0x007E

#define STATUS_PRE_TMR1     0x0100
#define STATUS_PRE_TMR2     0x0200
#define STATUS_PRE_TMR3     0x0400
#define STATUS_PRE_TMR4     0x0800

#define P1_LEV_DEBOUNCE    (100000)     // 10 seconds
#define P2_CLOSE_TIME      (10000)      // 1 seconds
#define P3_DROP_AWAY       (200000)     // 20 seconds
#define FILL_ON_MIN        (100000)     // 10 seconds

// TODO: Probs unused
typedef struct{
//    char stack;
//    char t1;
//    char t2;
//    char t3;
//    char t4;
//    char t5;
//    char t6;
//    
//    char cmd;
//    int fills;
    int status;
    char temp;
}SYSTEM;
SYSTEM sys;


//// USB interface host command messages - Communication stuff
//const char *msg_startup     = "\n\r *** DECODE AUTOMATION ***      \0";
//const char *msg_version     = "\n\r    Autoloader Ver-1.00         \0";
//const char *msg_emptyDel    = "\n\r Press 'E' enter 'empty' delay  \0";
//const char *msg_lowLev      = "\n\r Press 'L' to adjust low level  \0";
//const char *msg_contact     = "\n\r Press 'C' for contact details  \0";
//const char *msg_monitor     = "\n\r Press 'M' for debug monitoring \0";
//const char *msg_io_test     = "\n\r Press 'T' for I/O test         \0";
//const char *msg_userErr     = "\n\r    Invalid selection!          \0";
//const char *msg_debugRdy    = "\n\r READY.    The system will run when start is pressed              \0";
//const char *msg_debugRun    = "\n\r MONTIORING.    Monitoring the glue level, engage host 'dry' contact         \0";
//const char *msg_debugFill   = "\n\r HOPPER FILL.   Detected a low hopper glue level. Engage 'fill' solenoid     \0";
//const char *msg_debugStFl   = "\n\r FILL STOP.     Disengage 'fill' solenoid     \0";
//const char *msg_debugLid    = "\n\r SYSTEM HALT.   Detected the lid open. All outputs disengaged. Close to continue\0";
//const char *msg_debugErr    = "\n\r SYSTEM ERROR.  Hopper fill timeout failure. Check hopper before proceeding  \0";
//const char *msg_debugCls    = "\n\r Resuming...    Hopper lid closed  \0";

const char *autoLoaderMsgList      =   "                . CHECK HOPPER! . Fill Imminent .  Fill Attempt .   STAND-BY    .  Filling      .  Under-Temp   .  Level Low    .  Lid is open  .    CHECK      .   INSPECT     . SYSTEM HALTED .               .               .....";

char display_offset, msg_flash, animation_count,animation_step = 0; // TODO: UNUSED?

/* Scan machine inputs
 * Caller: Main
 * Move to: Stays in main
 */
static void scan_MachineInputs()
{
    if(t3 >= (3500))
    {
        t3 = 0;
        if(PORTEbits.RE13 == 1)
        {
            inputDevices |=  INPUT_GLUELEVEL;
        }
        else
        {
            inputDevices &= ~INPUT_GLUELEVEL;
        }
        if(PORTEbits.RE12 == 1)
        {
            inputDevices |=  INPUT_HOPPERLID;
        }
        else
        {
            inputDevices &= ~INPUT_HOPPERLID;
        }
        if(PORTCbits.RC11 == 1)
        {
            inputDevices |=  INPUT_TEMP_OK;
        }
        else
        {
            inputDevices &= ~INPUT_TEMP_OK;
        }
        if(PORTAbits.RA12 == 1)
        {
            inputDevices |=  INPUT_5;
        }
        else
        {
            inputDevices &= ~INPUT_5;
        }
        if(PORTAbits.RA11 == 1)
        {
            inputDevices |=  INPUT_4;
        }
        else
        {
            inputDevices &= ~INPUT_4;
        }
        if(PORTCbits.RC2 == 1)
        {
            inputDevices |=  INPUT_3;
        }
        else
        {
            inputDevices &= ~INPUT_3;
        }
        if(PORTCbits.RC1 == 1)
        {
            inputDevices |=  INPUT_2;
        }
        else
        {
            inputDevices &= ~INPUT_2;
        }
        if(PORTCbits.RC0 == 1)
        {
            inputDevices |=  INPUT_1;
        }
        else
        {
            inputDevices &= ~INPUT_1;
        }
    }
}
/* What does this function do???
 * Variables:
 * lcd_msg - use in many other functions
 * lcdState - use in this function and process_LcdController()
 * buff - use in this function
 * 
 * Definition:
 * BUS_TRANSACTION - use in this function
 */
static void process_OutputDevices()
{
    if((lcd_msg == 0) && (lcdState == LCD_RDY))             
    {
        if(BUS_TRANSACTION)
        {
            buff = outputDevices;
            LATB = (buff & 0x00FF);
            out_nWR1_SetHigh();         // LATAbits.LATA1 = 1;
            asm("nop");
            asm("nop");
            out_nWR1_SetLow();          // LATAbits.LATA1 = 0;
        }
        else
        {
            out_nWR1_SetLow();          // LATAbits.LATA1 = 0;
        }
    }
}

//static void process_SerialComs()
//{
//    char i;
//    switch(comTasks)
//    {
//        case INI:
//        comTasks = RDY;
//        rxMsg = 0, txMsg = 0;
//        rxIndx = 0, txIndx = 0;
//        break;
//            
//        case RDY:
//        if(U1STAbits.URXDA == 1) 
//        {
//            i = U1RXREG;
//            rxbuff[rxMsg++] = i;
//        }
//        if(txIndx != txMsg)    
//        {
//            comTasks = BUSY;
//        }
//        break;
//            
//        case BUSY:
//        if(U1STAbits.URXDA == 1) 
//        {
//            i = U1RXREG;
//            rxbuff[rxMsg++] = i;
//        }
//        if(U1STAbits.UTXBF == 0 && U1STAbits.TRMT == 1)
//        {
//            i = txbuff[txIndx++];
//            U1TXREG = i;
//            if(txIndx == txMsg)
//            {
//                txIndx = 0;
//                txMsg = 0;
//                comTasks = RDY;
//            }
//        }
//        break;
//    }
//}
//

/* This function requires LCD_RDY_STATE which is modified by 
 * process_LcdController()
 */
static void system_userInterface()
{
    int i;
    if(LCD_RDY_STATE)
    {
        switch(uiState)
        {
            case HMI_BOOT:    
            switch(hmiTask)
            {
                case 0:
                set_backlight_level(BACKLIGHT10);
                strcpy(msgBuff, autoLoaderMsgList);     // Load Autoloader message list   
                hmiTask++;
                break;
                
                case 1:
                hmi_GenerateBootScreen(HMI_ANIMATION);
                    break;
                    
                case 2:
                t8 = 0;
                lmp_state = 0;
                scrn_state = 0;
                uiState = HMI_READY;    
                    break;
            }
            break;
                
            case HMI_READY:
            i = (sys.status & STATUS_SYS_RDY);
            if(i)
            {
                load_defaults();
                hmi_ConfigSystemState(HMI_RUN);
            }
            break;
            
            case HMI_RUN:
            switch(hmiTask)             // runTasks
            {
                case MC_READY:          // 0
                hmi_lcdController();
                hmi_lampStackController();
                hmi_scanSetupCommand(BUTTON1);
                break;
                        
                case MC_STDBY:          // 1
                hmi_lcdController();
                hmi_lampStackController();
                hmi_scanSetupCommand(BUTTON1);
                break;
                
                case MC_IMNT:           // 2
                hmi_lcdController();
                hmi_lampStackController();
                hmi_scanSetupCommand(BUTTON1);
                break;
                 
                case MC_FILL:           // 3
                hmi_lcdController();
                hmi_lampStackController();
                hmi_scanSetupCommand(BUTTON1);
                break;
                    
                case MC_CHK:            // 4
                hmi_lcdController();
                hmi_lampStackController();
                hmi_scanSetupCommand(BUTTON1);
                break;
                    
                case MC_LID:            // 5
                hmi_lcdController();
                hmi_lampStackController();
                hmi_scanSetupCommand(BUTTON1);
                break;
            }
            break;
            
            case HMI_SETUP:
            switch(hmiTask) 
            {
                case 0:      
                strcpy(lcdBuff, lcd_setP1);
                word2DecConverter(P1_value, &lcdBuff[(12)], 4);
                lcd_msg = strlen(lcdBuff);  
                hmiTask++;
                break;

                case 1:  
                hmi_setupL1KeyCommand();                // Modify the fill time out value
                hmi_lampStackController();
                hmi_SetupStateTimeout(SETUP_TIMEOUT);   // One minute setup timeout
                break;

                case 2:
                hmi_setupL2KeyCommand();
                hmi_lampStackController();
                hmi_SetupStateTimeout(SETUP_TIMEOUT);
                break;

                case 3:
                hmi_setupL3KeyCommand();
                hmi_lampStackController();
                hmi_SetupStateTimeout(SETUP_TIMEOUT);
                break;

                case 4:
                hmi_setupL4KeyCommand();
                hmi_lampStackController();
                hmi_SetupStateTimeout(SETUP_TIMEOUT);
                break;
                
                case 5:
                hmi_setupL5KeyCommand();
                hmi_lampStackController();
                hmi_SetupStateTimeout(SETUP_TIMEOUT);
                break;
                
                case 6:
                hmi_setupL6KeyCommand();
                hmi_lampStackController();
                hmi_SetupStateTimeout(SETUP_TIMEOUT);
                break;
            }    
            break;
        }
    }
}

///* This function set the transmit buffer depending on the receive buffer processed
// * in process_SerialComs()
// */
//static void command_interpreter()
//{
//    char i;
//    switch(cmdTasks)
//    {
//        case CONNECT:
////        if(comTasks == RDY)   
////        {
////            strcpy(txbuff, msg_startup);
////            txMsg = strlen(txbuff);
////            cmdTasks = MENU;
////        }
//        break;
//            
//        case MENU:
//        if(comTasks == RDY)
//        {
//            strcpy(txbuff, msg_version);
//            txMsg = strlen(txbuff);
//            cmdTasks = IO;
//        }
//        break;
//        
//        case IO:
//        if(rxIndx != rxMsg)  
//        {
//            cmdTasks = SEND;
//            i = rxbuff[rxIndx++];
//            switch(i)
//            {
//                case 'E':case 'e':
//                strcpy(txbuff, msg_emptyDel);
//                
//                break;
//                
//                case 'L': case 'l':
//                strcpy(txbuff, msg_lowLev);
//                
//                break;
//                
//                case 'C': case 'c':
//                strcpy(txbuff, msg_contact);
//                
//                break;
//                
//                case 'M': case 'm':
//                strcpy(txbuff, msg_monitor);
//                break;
//                
//                case 'I': case 'i':
//                strcpy(txbuff, msg_io_test);
//                
//                break;
//                
//                default:
//                strcpy(txbuff, msg_userErr);   
//                cmdTasks = ERROR;
//                break;
//            }
//        }
//        break;
//        
//        case SEND:
//        if(comTasks == RDY)
//        {
//            txMsg = strlen(txbuff);
//            cmdTasks = IO;
//        }
//        break;
//        
//        case ERROR:
//        if(comTasks == RDY)
//        {
//            txMsg = strlen(txbuff);
//            cmdTasks = MENU;
//        }
//        break;
//    }
//}

typedef enum{
    SYS_INI = 0,
    SYS_CONFIG,             // Loads NV memory
    SYS_READY,
    RUN_PROGRAM
}SYSTEM_STATE;

static char check_eventState(unsigned long val)
{
    unsigned long i, x;
    i = val;
    x = ((10000) * i);
    if(t1 >= x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void check_HopperLidClosed(unsigned long val)
{
    char i;
    unsigned long t;
    t = val;
    i = (inputDevices & INPUT_HOPPERLID);  
    if(i != INPUT_HOPPERLID)
    {
        
        if(t5 >= t)
        {
            t5 = 0;
            runTasks = 5;
            device_outputState(SOLENOIDS,STATE_OFF);
        }
    }
    else
    {
        t5 = 0;
    }
}

static void system_eventHandler()
{
    char i, c;
    switch(sysState)
    {
        case SYS_INI:
        if((comTasks == RDY) && (uiState == HMI_READY) && (wav_tasks == WAVE_IDLE))        
        {
            t1 = 0;
            inputDevices = 0;
            outputDevices = 0;
            sysState = SYS_CONFIG;
        }
        break;
       
        case SYS_CONFIG:
        if(t1 >= (10000))
        {
            sysState = SYS_READY;   
            sys.status = STATUS_SYS_RDY;
        } 
        break;
            
        case SYS_READY:
        t1 = 0;             // t1 = process timer
        t5 = 0;             // t5 = hopper empty de-bounce timer
        t13 = 0;            // glue timer
        t14 = 0;            // fill solenoid 
        runTasks = 0;   
        sysState = RUN_PROGRAM;
        device_outputState(SOLENOIDS,STATE_OFF);
        break;
        
        case RUN_PROGRAM:                                       // Set IO scan at 200mS
        switch(runTasks)
        {
            case MC_READY:
            i = check_eventState(P2_value);                     // 1 second
            if(i)
            {
                t1 = 0;  
                t13 = 0;
                fill_cntr = 0;                                  // Clear attempt counter
                runTasks = MC_STDBY;
            }
            check_HopperLidClosed(DEBOUNCE_LID);
            break;
            
            case MC_STDBY:             
            c = (inputDevices & INPUT_GLUELEVEL);               // Valid low detection for hopper fill
            if(c)
            {
                t1 = 0;
            }
            else
            {
                i = check_eventState(P1_value);                 // wait for 5 seconds
                if(i)                                           // Hopper low de-bounce period  (10 seconds) was (60000)
                {
                    t1 = 0;
                    t13 = 0;
                    runTasks = MC_IMNT;
                    device_outputState(RLY_GATE,STATE_ON);
                }
            }
            check_HopperLidClosed(DEBOUNCE_LID);                // 1 second
            break;
            
            case MC_IMNT:             
            c = (inputDevices & INPUT_GLUELEVEL);               // Valid pre-fill state
            if(c)
            {
                t13 = 0;
                i = check_eventState(P6_value);                 // wait for 5 seconds
                if(i)                                           // 10 seconds
                {
                    t1 = 0;
                    runTasks = MC_READY;
                    device_outputState(RLY_GATE,STATE_OFF);
                }
            }
            else
            {   
                t1 = 0;
                if(t13 >= (40000))
                {
                    t1 = 0;
                    fill_cntr++;
                    runTasks = MC_FILL;
                    device_outputState(RLY_FILL,STATE_ON);
                }
            }
            check_HopperLidClosed(DEBOUNCE_LID);
            break;
                
            case MC_FILL:    
            i = check_eventState(P6_value);                     // ON for 6 seconds
            if(i)                                               // Minimum fill solenoid ON period (10 seconds)
            {
                t1 = 0;
                runTasks = MC_CHK;
                device_outputState(RLY_FILL,STATE_OFF);
            }
            check_HopperLidClosed(DEBOUNCE_LID);
            break;
            
            case MC_CHK:
            i = check_eventState(P4_value);                             // ON for 2 seconds
            if(i)
            {
                t1 = 0;
                t13 = 0;
                runTasks = MC_IMNT;
            }
            check_HopperLidClosed(DEBOUNCE_LID);
            break;
            
            case MC_LID:
            c = (inputDevices & INPUT_HOPPERLID);
            if(c)
            {
                i = check_eventState(P4_value);                 // wait for 2 seconds
                if(i)                                           // P4_value 2 seconds
                {
                    t1 = 0;
                    runTasks = MC_READY;
                }
            }    
            else
            {
                t1 = 0;
            }
            break;
        }
        break;
    }
}

// Empty
static void process_DebugOutput()
{
    
    
}

int main(void)
{
    SYSTEM_Initialize();
    sysState = 0;
    while (1)
    {
        scan_MachineInputs(); // stays in main
        command_interpreter(); // stays in main
        system_eventHandler(); // stays in main
        system_userInterface(); // probs in hmi? or maybe its in main since its system level
        
        process_ToneGenerator(); // maybe in tone generator? Also has speaker driver
        process_LcdController(); // probs in LCD
        process_KeyController(); // probs in HMI
        
        process_OutputDevices(); // stays in main
        process_DebugOutput(); // stays in main
        process_SerialComs(); // not sure, maybe just leave it in main
        
        process_Backlight(); // probs in LCD or in main
    }
    return 1; 
}

/**
 End of File
*/

