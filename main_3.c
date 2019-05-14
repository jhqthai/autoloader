// The file that I rip to pieces

#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "hmi.h"
#include "keypad.h"
#include "backlight.h"
#include "tone.h"
#include "lcd.h"



// Serial comm's 
#define TX_BUFFER       (128)
#define RX_BUFFER       (255)
#define UART1_BRG       (((FCY / UART1_BAUD)/16) - 1)
char txbuff[TX_BUFFER], rxbuff[RX_BUFFER];
//char lampState, uiState, uiTasks, hmiTask, comTasks, cmdTasks, cmdBuff, sysState, rxIndx, txIndx, rxMsg, txMsg = 0;
//extern volatile unsigned long t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;

//#define DEFAULT_P1 (5)         // Empty low time   (10 seconds)
//#define DEFAULT_P2 (1)         // Lid close time   (1 second)
//#define DEFAULT_P3 (10)        // Drop away time   (10 seconds)
//#define DEFAULT_P4 (2)         // Slide open time  (1 second)
//#define DEFAULT_P5 (3)         // Fill high time   (3 seconds)
//#define DEFAULT_P6 (6)         // Solenoid minimum ON time   (6 seconds)
//int P1_value, P2_value, P3_value, P4_value, P5_value, P6_value;

//#define STATE_ON        (1)
//#define STATE_OFF       (0)
//#define LAMP_GRN        0x01
//#define LAMP_RED        0x02
//#define LAMPSTACK       0x03
//#define RLY_FILL        0x04
//#define RLY_GATE        0x08
//#define SOLENOIDS       0x0C

//                       Operational parameters
#define ONE_HERTZ       (10000)  
#define TWO_HERTZ       (5000)
#define SETUP_TIMEOUT   (600000)

#define SCAN_PERIOD     (1000)          // 100mS debounce on input port.
#define MIN_FILLTIME    (30000)         // Minimum delay on fill solenoid activation
#define RLY_CHG_WT      (10000)
#define VALID_GLUELOW   (30000)         // Will be changed in menu
#define VALID_GLUEFILL  (50000)
#define VALID_FILLERR   (600000)        // Hopper fill error timeout - to be save in NV

//#define HMI_ANIMATION   (1000)
//int lcd_flash_count, fill_cntr, lamp_tmr; // scrn_hdr, scrn_ftr;
//char lamp_flash_count, scrn_state, lmp_state, led_state, scrn_hdr, scrn_ftr;
typedef enum{
    INI = 0,
    RDY,
    BUSY
}COM_STATE;

typedef enum{
    CONNECT = 0,
    MENU,
    IO,
    SEND,
    ERROR
}HOST_CMD;

//char hmi_stack;
//typedef enum{
//    HMI_BOOT = 0,
//    HMI_READY,
//    HMI_RUN,
//    HMI_SETUP,
//    HMI_ERROR
//}UI_STATE;

//typedef enum{
//    MC_READY = 0,
//    MC_STDBY,
//    MC_IMNT,
//    MC_FILL,
//    MC_CHK,
//    MC_LID     
//}MACH_STATE;

//static void hmi_ConfigSystemState(char state)
//{
//    char i;
//    t8 = 0;
//    t10 = 0;
//    i = state;
//    uiState = i;
//    hmiTask = 0;
//    lmp_state = 0;
//    scrn_state = 0;
//}


// Process control
#define PROCESS_DELAY   (30000)
#define SETUP_DELAY     (30000)
#define FILL_TIMEOUT    (100000)
#define MIN_FILL_DEL    (20000)
#define CMD_SETUP       'S'
#define DEBOUNCE_LID    (10000)

//      HB LED flash times
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
//char outputDevices, buff, inputDevices = 0; 

//                 System States
//#define SYSTEM_RDY        (sysStatus = 0x01)
//#define LED1_ON()         (LATDbits.LATD8  = 1)
//#define LED2_ON()         (LATCbits.LATC10 = 1)
//#define LED3_ON()         (LATCbits.LATC12 = 1)
//#define LED4_ON()         (LATCbits.LATC15 = 1)
//#define LED1_OFF()        (LATDbits.LATD8  = 0)
//#define LED2_OFF()        (LATCbits.LATC10 = 0)
//#define LED3_OFF()        (LATCbits.LATC12 = 0)
//#define LED4_OFF()        (LATCbits.LATC15 = 0)
//#define ALL_LEDS_ON()    ((LATDbits.LATD8  = 1) && (LATCbits.LATC10 = 1) && (LATCbits.LATC12 = 1) && (LATCbits.LATC15 = 1))

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

typedef struct{
    char stack;
    char t1;
    char t2;
    char t3;
    char t4;
    char t5;
    char t6;
    
    char cmd;
    int fills;
    int status;
    char temp;
}SYSTEM;
SYSTEM sys;

unsigned char testCounter;

////                  Backlight
//#define BACKLIGHT_0     (0)
//#define BACKLIGHT_1     (1)
//#define BACKLIGHT_2     (2)
//#define BACKLIGHT_3     (3)
//#define BACKLIGHT_4     (4)
//#define BACKLIGHT_5     (5)
//#define BACKLIGHT_6     (6)
//#define BACKLIGHT_7     (7)
//#define BACKLIGHT_8     (8)
//#define BACKLIGHT_9     (9)
//#define BACKLIGHT10     (10)
//
//#define MAX_BL_LEVS     (11)
//#define BACKLIGHTSTROBE (10) // Require by process_Backlight())
//#define BL_LEV_MASK     0x0001

/* These vairables are require by LCD functions */
//char BL_LEV = 0;
//int backlight = 0;
//const int LCD_BLL[MAX_BL_LEVS]= {0x0000,0x0200,0x0300,0x0380,0x03C0,0x03E0,0x03F0,0x03F8,0x03FC,0x03FE,0x03FF};

//                          USB interface host command messages
const char *msg_startup     = "\n\r *** DECODE AUTOMATION ***      \0";
const char *msg_version     = "\n\r    Autoloader Ver-1.00         \0";
const char *msg_emptyDel    = "\n\r Press 'E' enter 'empty' delay  \0";
const char *msg_lowLev      = "\n\r Press 'L' to adjust low level  \0";
const char *msg_contact     = "\n\r Press 'C' for contact details  \0";
const char *msg_monitor     = "\n\r Press 'M' for debug monitoring \0";
const char *msg_io_test     = "\n\r Press 'T' for I/O test         \0";
const char *msg_userErr     = "\n\r    Invalid selection!          \0";
const char *msg_debugRdy    = "\n\r READY.    The system will run when start is pressed              \0";
const char *msg_debugRun    = "\n\r MONTIORING.    Monitoring the glue level, engage host 'dry' contact         \0";
const char *msg_debugFill   = "\n\r HOPPER FILL.   Detected a low hopper glue level. Engage 'fill' solenoid     \0";
const char *msg_debugStFl   = "\n\r FILL STOP.     Disengage 'fill' solenoid     \0";
const char *msg_debugLid    = "\n\r SYSTEM HALT.   Detected the lid open. All outputs disengaged. Close to continue\0";
const char *msg_debugErr    = "\n\r SYSTEM ERROR.  Hopper fill timeout failure. Check hopper before proceeding  \0";
const char *msg_debugCls    = "\n\r Resuming...    Hopper lid closed  \0";

//#define LCD_LINES           (2)
//#define LCD_CHARS           (16)
//#define LCD_SCRNS           (LCD_LINES * LCD_CHARS)
//#define LCD_PAGES           (4)
//#define LCD_MESSAGES        (LCD_PAGES * LCD_SCRNS)
//#define LCD_LINE1           (0)
//#define LCD_LINE2           (17)
//#define LCD_SCREEN          (3)
//#define LCD_BUFFER          (36)
//#define LCD_WR_LATENCY      (25)
//#define LCD_PWRUP           (10000)
//#define LCD_UPDATE_RQT      (lcd_msg > 0)
//#define LCD_IDLE            (lcdState == LCD_RDY)
//#define LCD_LINE1_HM         0x02
//#define LCD_LINE2_HM         0xC0
//#define LCD_RDY_STATE      ((lcd_msg == 0) && (lcd_index == 0) && (lcdState == LCD_RDY))
//#define refresh_display()   (lcd_msg = (33))

//const char *lcd_frame   = "   Auto-loader  \xC0     Ver        \x02\0";
//const char *lcd_load    = "    Loading     \xC0                \x02\0";
//const char *lcd_decode  = "     DECODE     \xC0   AUTOMATION   \x02\0";
//const char *config_1602A_2L_Blk = "\x38\x38\x0D\x06\x01\x02\0";     // Configuration of the 1602A LCD display with blink
//const char *config_1602A_2LnBLK = "\x38\x38\x0C\x06\x01\x02\0";     // Configuration of the 1602A LCD display with no blink and no underscore
//const char *config_1602A_2L_UL  = "\x38\x38\x0E\x06\x01\x02\0";     // Configuration of the 1602A LCD display with underscore
//const char *lcd_ready   = "  SYSTEM READY  \xC0                \x02\0";
//const char *lcd_start   = "  Starting      \xC0                \x02\0";
//const char *lcd_setup   = "   SETUP MODE   \xC0<-  (+)  (-)  ->\x02\0";
//const char *lcd_fillMax = " Fill Timeout   \xC0                \x02\0";
//const char *lcd_lowMax  = " Low Detect     \xC0  Sec           \x02\0";
//const char *lcd_setP1   = " SET P1         \xC0 Low Debnc  Del \x02\0";
//const char *lcd_setP2   = " SET P2         \xC0 Lid Close  Del \x02\0";
//const char *lcd_setP3   = " SET P3         \xC0 Drop Away  Del \x02\0";
//const char *lcd_setP4   = " SET P4         \xC0 Slide Open Del \x02\0";
//const char *lcd_setP5   = " SET P5         \xC0 Full Debnc Del \x02\0";
//const char *lcd_setP6   = " SET P6         \xC0 Its Bob & Rosie\x02\0";
//
//#define MSG_CLEAR       (0x00)
//#define MSG_HOPPER      (MSG_CLEAR       + LCD_CHARS)
//#define MSG_IMMINENT    (MSG_HOPPER      + LCD_CHARS)
//#define MSG_FILL_CNT    (MSG_IMMINENT    + LCD_CHARS)
//#define MSG_STANDBY     (MSG_FILL_CNT    + LCD_CHARS)
//#define MSG_FILLING     (MSG_STANDBY     + LCD_CHARS)
//#define MSG_GLUECOLD    (MSG_FILLING     + LCD_CHARS)
//#define MSG_GLUELOW     (MSG_GLUECOLD    + LCD_CHARS)
//#define MSG_LIDOPEN     (MSG_GLUELOW     + LCD_CHARS)
//#define MSG_CHECK       (MSG_LIDOPEN     + LCD_CHARS)
//#define MSG_INSPECT     (MSG_CHECK       + LCD_CHARS)
//#define MSG_SYSHALT     (MSG_INSPECT     + LCD_CHARS)
//#define MSG_STOREL1     (MSG_SYSHALT     + LCD_CHARS)
//#define MSG_STOREL2     (MSG_STOREL1     + LCD_CHARS)
//#define MSG_BUFFER      (MSG_STOREL2     + LCD_CHARS + 5)

//const char *autoLoaderMsgList    =   "                  .AUTO LOADER       HALTED         OPERATING        HOPPER          EMPTY        Glue is low     Lid is open     Glue is cold     Lid is open      SETTINGS     SYSTEM HALTED!                            .                .               .               . Fill Timeout  . Fill Timeout  . Low Debn      . Fill Timeout  . Fill Timeout  . Time    XX:xx . Fill Timeout  . Fill Timeout  .     ";
//const char *autoLoaderMsgList    =   "                .  HOPPER       . Fill Imminent .     CHECK     .   STAND-BY    .   Filling     .  Glue is low  .  Lid is open  . Glue is cold  .  Lid is open  .   SETTINGS    .  SYSTEM HALT  .                           .[1] = Exit/Save .[2]+ [3]-      .[4] = next ->  . Fill Timeout  . Empty Debn    . Inputs        . Outputs       . Time    XX:xx . Date  x/XX/xx . Outputs       .Seconds        .     ";
const char *autoLoaderMsgList      =   "                . CHECK HOPPER! . Fill Imminent .  Fill Attempt .   STAND-BY    .  Filling      .  Under-Temp   .  Level Low    .  Lid is open  .    CHECK      .   INSPECT     . SYSTEM HALTED .               .               .....";

//char msgBuff[MSG_BUFFER];
//char lcdBuff[LCD_BUFFER];
//char lcdState, lcd_index, lcd_msg = 0;

//char runTasks, devState, flashRate = 0;

//const char *hexAscii = "0123456789ABCDEF";
//
//static void word2DecConverter(unsigned int data, char *output, char digits)
//{
//    if (digits > 5)
//        return;
//
//    char d[5];
//    d[0] = (char)(data / 10000);
//    data -= (unsigned int)d[0] * 10000;
//    d[1] = (char)(data / 1000);
//    data -= (unsigned int)d[1] * 1000;
//    d[2] = (char)(data / 100);
//    data -= (unsigned int)d[2] * 100;
//    d[3] = (char)(data / 10);
//    data -= (unsigned int)d[3] * 10;
//    d[4] = (char)data;
//
//    // Convert to ascii (only 0..9 values)
//
//    char ascii[5];
//    int dIndex = 0;
//    for (dIndex = 0; dIndex < 5; dIndex++ )
//    {
//        ascii[dIndex] = hexAscii[d[dIndex]];
//    }
//
//    // Supress leading zeroes if required
////    if (!leadingZeroes)
////    {
////        if (ascii[0] == '0')
////            ascii[0] = ' ';
////        if (ascii[0] == ' ' && ascii[1] == '0')
////            ascii[1] = ' ';
////        if (ascii[0] == ' ' && ascii[1] == ' ' && ascii[2] == '0')
////            ascii[2] = ' ';
////        if (ascii[0] == ' ' && ascii[1] == ' '  && ascii[2] == ' ' && ascii[3] == '0')
////            ascii[3] = ' ';
////    }
//
//    // Copy over the result
//    for (dIndex = 0; dIndex < digits; dIndex++)
//    {
//        output[dIndex] = ascii[(5-digits)+dIndex];
//    }
//}

static unsigned char memory_write()
{
    unsigned char x;
    
    return x;
}

//static void device_outputState(char device, char state)
//{
//    char x,y;
//    x = device;
//    y = state;
//    if(y > 0)
//    {
//        outputDevices |=  x;
//    }
//    else
//    {
//        outputDevices &= ~x;
//    }
//}

static void device_ToggleState(char dev, int rate)
{
    int i;
    char x;
    i = rate;
    x = dev;
    if(t2 >= i)
    {
        t2 = 0;
        if(devState > 0)
        {
            devState = 0;
            lamp_flash_count++;
            device_outputState(x, STATE_ON);
        }
        else
        {
            devState++;
            device_outputState(x, STATE_OFF);
        }
    }
}


char display_offset, msg_flash, animation_count,animation_step = 0;
//extern volatile unsigned char lcd_strobe;
//extern volatile unsigned int keyDebn;
//
//typedef enum{
//    LCD_INITIALIZE = 0,
//    LCD_CONFIG, 
//    LCD_RDY,
//    LCD_BUSY,
//    LCD_WRITE,
//    LCD_LINE2HM
//}LCD_STATES;

        
//#define INPUT_GLUELEVEL 0x80            
//#define INPUT_HOPPERLID 0x40
//#define INPUT_TEMP_OK   0x20
//#define INPUT_5         0x10
//#define INPUT_4         0x08
//#define INPUT_3         0x04
//#define INPUT_2         0x02
//#define INPUT_1         0x01

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
 * 
 * 
 * 
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

static void process_SerialComs()
{
    char i;
    switch(comTasks)
    {
        case INI:
        comTasks = RDY;
        rxMsg = 0, txMsg = 0;
        rxIndx = 0, txIndx = 0;
        break;
            
        case RDY:
        if(U1STAbits.URXDA == 1) 
        {
            i = U1RXREG;
            rxbuff[rxMsg++] = i;
        }
        if(txIndx != txMsg)    
        {
            comTasks = BUSY;
        }
        break;
            
        case BUSY:
        if(U1STAbits.URXDA == 1) 
        {
            i = U1RXREG;
            rxbuff[rxMsg++] = i;
        }
        if(U1STAbits.UTXBF == 0 && U1STAbits.TRMT == 1)
        {
            i = txbuff[txIndx++];
            U1TXREG = i;
            if(txIndx == txMsg)
            {
                txIndx = 0;
                txMsg = 0;
                comTasks = RDY;
            }
        }
        break;
    }
}

/****************************************
 * Keypad driver section maybe?
 ****************************************/
//char keyBuff, keyValue, keyState = 0;
//typedef enum{
//    KEY_INI = 0,
//    KEY_SCAN, 
//    KEY_REL
//}KEY_STATES;

//#define DEBOUNCE_DEL            (80)
//#define SHUTDOWN_DEL            (30000)
//#define KEYSTUCK_DEL            (60000)
//#define BUTTON1                 (0x01)
//#define BUTTON2                 (0x02)
//#define BUTTON3                 (0x04)
//#define BUTTON4                 (0x08)
//#define KEYPRESSED              (BUTTON1 || BUTTON2 || BUTTON3 || BUTTON4)
//#define KEYSTUCK_DETECTED       (keyValue == 0x10)
//#define KEYPRESS_DETECTED      ((PORTCbits.RC5 == 1) || (PORTCbits.RC4 == 1) || (PORTCbits.RC3 == 1) || (PORTAbits.RA9 == 1))
//#define NO_KEYS_PRESSED        ((PORTCbits.RC5 == 0) && (PORTCbits.RC4 == 0) && (PORTCbits.RC3 == 0) && (PORTAbits.RA9 == 0))
//
//static char read_KeyBuff()      // KEYPRESSED
//{
//    if(keyValue > 0)
//    {
//        return keyValue;
//    }
//    else
//    {
//        return 0;
//    }
//}
//
//static void process_KeyController()
//{
//    switch(keyState)
//    {
//        case KEY_INI:
//        t12 = 0;
//        keyBuff  = 0;
//        keyValue = 0;
//        keyState = KEY_SCAN;
//        break;
//        
//        case KEY_SCAN:
//        if(KEYPRESS_DETECTED)
//        {
//            if(t12 >= DEBOUNCE_DEL)
//            {
//                set_backlight_level(BACKLIGHT10);
//                keyState = KEY_REL;
//                if(PORTCbits.RC5 == 1)
//                {
//                    keyBuff |=  0x01;
//                }
//                else
//                {
//                    keyBuff &= ~0x01;
//                }
//                if(PORTCbits.RC4 == 1)
//                {
//                    keyBuff |=  0x02;
//                }
//                else
//                {
//                    keyBuff &= ~0x02;
//                }
//                if(PORTCbits.RC3 == 1)
//                {
//                    keyBuff |=  0x04;
//                }
//                else
//                {
//                    keyBuff &= ~0x04;
//                }
//                if(PORTAbits.RA9 == 1)
//                {
//                    keyBuff |=  0x08;
//                }
//                else
//                {
//                    keyBuff &= ~0x08;
//                }
//            }
//        }
//        else
//        {
//            t12 = 0;
//        }
//        break;
//            
//        case KEY_REL:
//        if(NO_KEYS_PRESSED)
//        {
//            t12 = 0;
//            keyValue = keyBuff;
//            keyState = KEY_SCAN;
//        }
//        if(t12 >= KEYSTUCK_DEL)
//        {
//            keyValue |= 0x10;
//        }
//        break;
//    }
//}

/****************************************
 * Backlight process section
 ****************************************/
//
///* This function is currently not being used
// * However it requies BL_LEV which is used by other LCD functions
// * Therefore, move it to LCD as well
// */
//#define BACKLIGHTFADE_DEL   (10000)
//#define BACKLIGHTFADE_LEV   (6)
//static void lcd_backlight_autodim()
//{
//    if(BL_LEV > BACKLIGHTFADE_LEV)
//    {
//        if(t9 >= BACKLIGHTFADE_DEL)
//        {
//            t9 = 0;
//            BL_LEV--;
//        }
//    } 
//}
//
/////* This function is called by system_userInterface() and process_KeyController()
//// * Will probably move this to LCD as well.
//// */
////void set_backlight_level(char level)
////{
////    BL_LEV = level;
////}
//
//// TODO: Maybe move this to LCD
//static void process_Backlight()
//{
//    unsigned long i;
//    if(t4 >= BACKLIGHTSTROBE)
//    {
//        t4 = 0;
//        i = (backlight & BL_LEV_MASK);
//        if(i == 1)
//        {
//            lcd_BL_SetHigh();           
//        }
//        else
//        {
//            lcd_BL_SetLow();
//        }
//        backlight = (backlight >> 1);
//        if(backlight == 0)
//        {
//            backlight = LCD_BLL[BL_LEV];
//        }
//    }
//}

/* This function controls the state of the LCD controller.
 * Not sure if we can move this function to lcd.c file or not 
 * since it contains variables that are required in other function which 
 * we currently deemed should stay in main such as
 * lcdState - requires by process_OutputDevices
 * This function modify lcd_index, lcd_msg and lcdState which affect LCD_IDLE
 * and LCD_RDY_STATE, LCD_RDY_STATE is required by system_userInterface()
 * How does t7 increase?
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
//static void process_LcdController()
//{
//    char i;
//    switch(lcdState)
//    {
//        case LCD_INITIALIZE:
//        if(t7 >= LCD_PWRUP)
//        {
//            lcd_index = 0, lcd_msg = 0;
//            strcpy(lcdBuff, config_1602A_2LnBLK);
//            lcd_msg = strlen(lcdBuff);
//            lcdState = LCD_CONFIG;
//            out_nWR2_SetLow();
//            lcd_RS_SetLow();
//            lcd_strobe = 0;
//            t7 = 0;
//        }
//        break;
//        
//        case LCD_CONFIG:
//        if(t7 >= (60))
//        {
//            t7 = 0;
//            i = lcdBuff[lcd_index];
//            LATB = (i & 0x00FF);
//            lcd_strobe = (30);
//            if(lcd_index++ >= lcd_msg+1)
//            {
//                lcd_msg = 0;
//                lcd_index = 0;
//                lcdState = LCD_RDY;
//            }
//            out_nWR2_SetHigh();
//        }
//        if(lcd_strobe == 0)  
//        {
//            out_nWR2_SetLow();
//        }
//        break;
//
//        case LCD_RDY:
//        if(lcd_msg > 0)
//        {
//            t7 = 0;
//            lcdState = LCD_BUSY;
//        }
//        break;
//            
//        case LCD_BUSY:  
//        switch(lcd_index)
//        {            
//            case (16):
//            lcd_index++;
//            lcd_RS_SetLow();
//            i = LCD_LINE2_HM;       // Home display line-2
//            LATB = (i & 0x00FF);
//            out_nWR2_SetHigh();
//            lcdState = LCD_WRITE;
//            lcd_strobe = LCD_WR_LATENCY;
//            break;
//                
//            case (33):
//            lcd_index++;
//            lcd_RS_SetLow();
//            i = LCD_LINE1_HM;       // Home display line-1
//            LATB = (i & 0x00FF);
//            out_nWR2_SetHigh();
//            lcdState = LCD_WRITE;
//            lcd_strobe = LCD_WR_LATENCY;
//            break;
//                
//            case (34):
//            lcd_msg = 0;
//            lcd_index = 0;
//            lcdState = LCD_RDY;
//            break;
//                
//            default:
//            lcd_RS_SetHigh();
//            i = lcdBuff[lcd_index++];
//            LATB = (i & 0x00FF);
//            out_nWR2_SetHigh();
//            lcdState = LCD_WRITE;
//            lcd_strobe = 1;
//            break;
//        }
//        break;
//        
//        case LCD_WRITE:
//        if(lcd_strobe == 0)  
//        {
//            out_nWR2_SetLow();
//            lcdState = LCD_BUSY;
//        }
//        break;
//    }
//}

////                  Tone generator
//#define KEY_DUR         (200)
//#define ACK_DUR         (1000)
//
//#define PITCH_MAX  (2)
//#define A5      ((113) / 2)     // 880 Hertz 
//#define A5b     ((120) / 2)     // 830.6094 Hertz
//#define B5      ((101) / 2)     // 987.7666 Hertz 
//#define B5b     ((107) / 2)     // 932.3275 Hertz
//#define C5      ((191) / 2)     // 523.2511 Hertz
//#define D5      ((170) / 2)     // 587.3295 Hertz
//#define D5b     ((180) / 2)     // 554.3653 Hertz
//#define E5      ((151) / 2)     // 659.2551 Hertz
//#define F5      ((143) / 2)     // 698.4565 Hertz 
//#define G5      ((127) / 2)     // 783.9909 Hertz
//#define G5b     ((135) / 2)     // 739.9888 Hertz
//
//#define A4      ((227) / 2)     // 440 Hertz 
//#define A4b     ((240) / 2)     // 415.3047 Hertz
//#define B4      ((202) / 2)     // 493.8833 Hertz 
//#define B4b     ((214) / 2)     // 466.1638 Hertz
//#define C4      ((382) / 2)     // 261.6256 Hertz
//#define D4      ((340) / 2)     // 293.6648 Hertz
//#define D4b     ((360) / 2)     // 277.1826 Hertz
//#define E4      ((303) / 2)     // 329.6276 Hertz
//#define F4      ((286) / 2)     // 349.2282 Hertz 
//#define G4      ((255) / 2)     // 391.9954 Hertz
//#define G4b     ((270) / 2)     // 369.9944 Hertz
//
//#define A3      ((454) / 2)     // 220 Hertz
//#define A3b     ((481) / 2)     // 207.6523 Hertz
//#define B3      ((405) / 2)     // 246.9417 Hertz
//#define B3b     ((429) / 2)     // 233.0819 Hertz
//
//int tune = 0;
//char wav_tasks;
//unsigned int wav[(255)], wav_index, wav_msg, wav_tempo = 0; // TODO: UNUSED
//unsigned int notes[]   = {(191),(191),(170),(191),(143),(151)}; // TODO: Make const
//unsigned int tempo[]   = {(10000),(8000),(12000),(10000),(10000),(40000)}; // TODO: Make const
//
//typedef enum{
//    WAVE_INI = 0,
//    WAVE_IDLE,      
//    WAVE_LOAD,
//    WAVE_PLAY
//}TONE_STATE;
//
//unsigned int freq, pitch; // TODO: Make freq local variable, delete pitch -> no use
//static void wav_playTone(unsigned int tone)
//{
//    unsigned int i;
//    i = tone;
//    if(IFS0bits.T3IF)
//    {
//        IFS0bits.T3IF = 0;
//        if(freq++ >= i)
//        {
//            freq = 0;
//            LATBbits.LATB8 = ~LATBbits.LATB8;       // Oscillate the tone pin
//        }
//    }
//}
///* Generate tone
// * Caller: main
// * Callee:
// * 
// * Variables: wave_tasks - modified by process_ToneGenerator() and system_eventHandler()
// */
//static void process_ToneGenerator()
//{
//    switch(wav_tasks)
//    {
//        case WAVE_INI:
//        t11 = 0, tune = 0;
//        LATBbits.LATB8 = 0;
//        wav_tasks = WAVE_IDLE;
//        break;
//        
//        case WAVE_IDLE:
//        
//        break;     
//        
//        case WAVE_LOAD:
//            
//        LATBbits.LATB8 = 0;
//        break;
//        
//        case WAVE_PLAY:
//        wav_playTone(notes[tune]);
//        if(t11 >= tempo[tune])    
//        {
//            t11 = 0;
//            if(tune++ >= 5)
//            {
//                t11 = 0, tune = 0;
//                LATBbits.LATB8 = 0;
//                wav_tasks = WAVE_INI;
//            }
//        }
//        break;  
//    }
//}

/****************************
 * TODO: POTENTIAL UNUSED FUNCTIONS
 ****************************/

/* TODO: UNUSED FUNCTION! */
static void hmi_StateUpdate(char currentState, char newState)
{
    char i, x;
    i = newState;
    x = currentState;
    if(i != x)
    {
        scrn_state = 0;
        hmiTask = x;
        return;
    }
}

static void mem_clearDisplayL1_RAM()
{
    memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_CLEAR,LCD_CHARS);
}

static void mem_clearDisplayL2_RAM()
{
    memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);
}

static void mem_saveDisplayL1_RAM()
{
    memcpy(msgBuff+MSG_STOREL1,lcdBuff+LCD_LINE1,LCD_CHARS);
}

static void mem_saveDisplayL2_RAM()
{
    memcpy(msgBuff+MSG_STOREL2,lcdBuff+LCD_LINE2,LCD_CHARS);
}

static void mem_readDisplayL1_RAM()
{
    memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_STOREL1,LCD_CHARS);
}

static void mem_readDisplayL2_RAM()
{
    memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_STOREL2,LCD_CHARS);
}

/* TODO: UNUSED FUNCTION */
static void hmi_TemperatureAlert()
{
    char i;
    i = (inputDevices & INPUT_TEMP_OK);
    if(i != INPUT_TEMP_OK)
    {
        memcpy(lcdBuff+LCD_LINE1,msgBuff+MSG_HOPPER,LCD_CHARS);
        memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_GLUECOLD,LCD_CHARS); 
    }
    else
    {
        memcpy(lcdBuff+LCD_LINE2,msgBuff+MSG_CLEAR,LCD_CHARS);  
    }
}

static void display_FlashUpdate(char line)
{
    char i;
    i = line;
    switch(msg_flash)
    {
        case 0:
        msg_flash = 1;
        if(i == (1))                    //LCD_SCREEN)
        {
            mem_saveDisplayL1_RAM();
        }
        if(i == (2))
        {
            mem_saveDisplayL2_RAM();
        }
        if(i == (3))
        {
            mem_saveDisplayL1_RAM();
            mem_saveDisplayL2_RAM();
        }
        case 1:
        msg_flash = 2;
        if(i == (1))                    //LCD_SCREEN)
        {
            mem_readDisplayL1_RAM();
        }
        if(i == (2))
        {
            mem_readDisplayL2_RAM();
        }
        if(i == (3))
        {
            mem_readDisplayL1_RAM();
            mem_readDisplayL2_RAM();
        }
        break;

        case 2:
        lcd_flash_count++;
        msg_flash = 1;
        if(i == (1))                    //LCD_SCREEN)
        {
            mem_clearDisplayL1_RAM();
        }
        if(i == (2))
        {
            mem_clearDisplayL2_RAM();
        }
        if(i == (3))
        {
            mem_clearDisplayL1_RAM();
            mem_clearDisplayL2_RAM();
        }
        break;
    }
    refresh_display();
}

/* TODO: POTENTIAL UNUSED FUNCTION */
static void lcd_display_flash(char line, int on, int off)
{ 
    char r;
    int t,x;
    r = line;
    t = on, x = off;
    if(msg_flash == 2)        //HMI_HDR_FLASHRATE1)
    {
        if(t8 >= t)
        {
            t8 = 0;
            display_FlashUpdate(r);
        }
    }
    else
    {
        if(t8 >= x)
        {
            t8 = 0;
            display_FlashUpdate(r);
        }
    }
}
/***********************
 * END OF POTENTIAL UNUSED FUNCTIONS
 ***********************/

//static void lampStack_errorSeq(unsigned long tmr)
//{
//    unsigned long i;
//    i = tmr;                                        
//    if(t10 >= i)
//    {   
//        t10 = 0;
//        switch(lmp_state++)
//        {
//            case 0:
//            device_outputState(LAMP_RED,STATE_ON);  
//                break;
//
//            case (7):
//            device_outputState(LAMP_RED,STATE_OFF);  
//                break;
//
//            case (10):
//            lmp_state = 0;
//            //device_outputState(LAMP_RED,STATE_ON);  
//                break;
//
//            case (45):
//            device_outputState(LAMP_RED,STATE_OFF);  
//                break;
//
//            case (50):
//            device_outputState(LAMP_RED,STATE_ON);  
//                break;
//
//            case (55):
//            device_outputState(LAMP_RED,STATE_OFF);  
//                break;
//
//            case (60):
//            device_outputState(LAMP_RED,STATE_ON);  
//                break;
//
//            case (65):
//            device_outputState(LAMP_RED,STATE_OFF);  
//                break;
//                
//            case (70):
//            device_outputState(LAMP_RED,STATE_ON);  
//                break;
//                
//            case (75):
//            device_outputState(LAMP_RED,STATE_OFF);  
//                break;
//
//            case (80):
//                lmp_state = 0;   
//                break;
//                
//            default:
//                break;
//        }
//    }
//}

///* This function is called by hmi_GenerateBootScreen() */
//static void lcd_bootSeq()
//{
//    switch(scrn_state++)
//    {
//        case 5:                         // LCD display power-up delay
//        strcpy(lcdBuff, lcd_decode);    // Decode Automation
//        lcd_msg = strlen(lcdBuff); 
//        break;
//
//        case (20):
//        strcpy(lcdBuff, lcd_frame);     // Auto-loader
//        lcd_msg = strlen(lcdBuff);
//            break;
//
//        case (30):
//        lcdBuff[(26)] = 'A';
//        refresh_display(); 
//            break;
//
//        case (40):
//        strcpy(lcdBuff, lcd_start);     // Starting 
//        lcd_msg = strlen(lcdBuff);
//        break;
//
//        case (45):
//        lcdBuff[(10)] = '.';
//        refresh_display();       
//            break;
//
//        case (50):
//        lcdBuff[(11)] = '.';
//        refresh_display();    
//            break;
//
//        case (55):
//        lcdBuff[(12)] = '.';
//        refresh_display();  
//            break;
//
//        case (70):
//        LED1_OFF();
//        LED2_OFF();
//        LED3_OFF();
//        LED4_OFF();
//        hmiTask++;
//            break;
//
//        default:
//            break;
//    }
//}

//static void lampStack_fillSeq(unsigned long tmr)
//{
//    unsigned long i;
//    i = tmr;              
//    if(t10 >= i)
//    {
//        t10 = 0;
//        switch(lmp_state++)
//        {
//            case 0:
//            device_outputState(LAMP_GRN,STATE_ON);  
//                break;
//
//            case (5):
//            device_outputState(LAMP_GRN,STATE_OFF);
//                break;
//
//            case (10):
//            lmp_state = 0;
//                break;
//                
//            default:
//                break;
//        }
//    }
//}

//static void lamp_bootSeq()
//{
//    switch(lmp_state++)
//    {
//        case 0:
//        device_outputState(LAMP_RED,STATE_ON);  
//        device_outputState(LAMP_GRN,STATE_OFF); 
//            break;
//
//        case (4):
//        device_outputState(LAMP_RED,STATE_OFF);  
//        device_outputState(LAMP_GRN,STATE_ON); 
//            break;
//
//        case (8):
//        lmp_state = 0;
//            break;
//        default:
//            break;
//    }
//}

//static void led_bootSeq()
//{
//    switch(led_state++)
//    {
//        case 0:
//        LED1_ON();  
//        LED2_OFF();  
//        LED3_OFF();  
//        LED4_OFF();  
//        break;
//
//        case 2:
//        LED1_OFF();  
//        LED2_ON();  
//        LED3_OFF();  
//        LED4_OFF();    
//        break;
//
//        case 4:
//        LED1_OFF();  
//        LED2_OFF();  
//        LED3_ON();  
//        LED4_OFF(); 
//        break;
//
//        case 6:
//        LED1_OFF();  
//        LED2_OFF();  
//        LED3_OFF();  
//        LED4_ON();    
//        break;
//
//        case 8:
//        LED1_OFF();  
//        LED2_OFF();  
//        LED3_ON();  
//        LED4_OFF(); 
//        break;
//
//        case (10):
//        LED1_OFF();  
//        LED2_ON();  
//        LED3_OFF();  
//        LED4_OFF(); 
//        break;
//
//        case (12):
//        led_state = 0;    
//            break;
//        default:
//            break;
//    }
//}

//static void lampStack_imminentSeq(unsigned long tmr)
//{
//    unsigned long i;
//    i = tmr;                                        
//    if(t10 >= i)
//    {   
//        t10 = 0;
//        switch(lmp_state++)
//        {
//            case 0:
//            device_outputState(LAMP_RED,STATE_OFF);
//            device_outputState(LAMP_GRN,STATE_ON);  
//                break;
//
//            case (20):
//            device_outputState(LAMP_GRN,STATE_OFF);
//                break;
//
//            case (40):
//            lmp_state = 0;
//                break;
//
//            default:
//                break;
//        }
//    }
//}

///* Main screen generation routines for HMI
// * Caller: system_userInterface()
// * MOVE to lcd.c probably or maybe hmi?
// */
//static void hmi_GenerateBootScreen(unsigned long tmr)
//{
//    unsigned long i;
//    i = tmr;                                        // Multiples of 100mS
//    if(t8 >= i)
//    {
//        t8 = 0;
//        led_bootSeq();
//        lcd_bootSeq();
//        lamp_bootSeq();
//    }
//}

///* Generate an imminent screen
// * Caller: hmi_lcdController()
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

///* Generates Fill Screen
// * Caller: hmi_lcdController()
// * 
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

///*
// * Generate Lid Error Screen
// * Caller: hmi_lcdController()
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

//static void hmi_SetupStateTimeout(unsigned long timeout)
//{
//    unsigned long i;
//    i = timeout;
//    if(t6 >= i)    
//    {
//        hmi_ConfigSystemState(HMI_RUN);
//    }
//}

//static unsigned char hmi_scanKeypad(void)
//{
//    char x;
//    if(keyValue > 0)
//    {
//        x = keyValue; 
//        keyValue = 0;
//        return (x);
//    }
//}

///* Scan setup command
// * Caller: system_userInterface()
// * Move to: hmi.c probably...
// */
//static void hmi_scanSetupCommand(char val)
//{
//    char x, y;
//    y = val;
//    x = hmi_scanKeypad();
//    if(x == y)
//    {
//        t6 = 0;
//        hmi_ConfigSystemState(HMI_SETUP);
//        return;
//    }
//}

//static void hmi_setupL1KeyCommand()
//{
//    char x;
//    x = hmi_scanKeypad();
//    if(x > 0) 
//    {
//        switch(x)
//        {
//            case BUTTON1:
//            hmi_ConfigSystemState(HMI_RUN);
//                break;
//                
//            case BUTTON2:
//            P1_value++;
//            strcpy(lcdBuff, lcd_setP1);
//            word2DecConverter(P1_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);      
//                break;
//                
//            case BUTTON3:
//            P1_value--;
//            strcpy(lcdBuff, lcd_setP1);
//            word2DecConverter(P1_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);    
//                break;
//                       
//            case BUTTON4:
//            hmiTask++;    
//            strcpy(lcdBuff, lcd_setP2);
//            word2DecConverter(P2_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);  
//            break;
//                
//            default:                
//                break;
//        }
//    }
//}
//
//static void hmi_setupL2KeyCommand()
//{
//    char x;
//    x = hmi_scanKeypad();
//    if(x > 0)
//    {
//        switch(x)
//        {
//            case BUTTON1:
//            hmi_ConfigSystemState(HMI_RUN);  
//                break;
//                
//            case BUTTON2:
//            P2_value++;  
//            strcpy(lcdBuff, lcd_setP2);
//            word2DecConverter(P2_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);   
//                break;
//                
//            case BUTTON3:
//            P2_value--;    
//            strcpy(lcdBuff, lcd_setP2);
//            word2DecConverter(P2_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);     
//                break;
//                       
//            case BUTTON4:
//            hmiTask++;    
//            strcpy(lcdBuff, lcd_setP3);
//            word2DecConverter(P3_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff); 
//                break;
//                
//            default:                
//                break;
//        }
//    }
//}
//
//static void hmi_setupL3KeyCommand()
//{
//    char x;
//    x = hmi_scanKeypad();
//    if(x > 0)
//    {
//        switch(x)
//        {
//            case BUTTON1:
//            hmi_ConfigSystemState(HMI_RUN); 
//                break;
//                
//            case BUTTON2:
//            P3_value++;    
//            strcpy(lcdBuff, lcd_setP3);
//            word2DecConverter(P3_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);   
//                break;
//                
//            case BUTTON3:
//            P3_value--;    
//            strcpy(lcdBuff, lcd_setP3);
//            word2DecConverter(P3_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);      
//                break;
//                       
//            case BUTTON4:
//            hmiTask++;    
//            strcpy(lcdBuff, lcd_setP4);
//            word2DecConverter(P4_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff); 
//                break;
//                
//            default:                
//                break;
//        }
//    }
//}
//
//static void hmi_setupL4KeyCommand()
//{
//    char x;
//    x = hmi_scanKeypad();
//    if(x > 0)
//    {
//        switch(x)
//        {
//            case BUTTON1:
//            hmi_ConfigSystemState(HMI_RUN);
//                break;
//                
//            case BUTTON2:
//            P4_value++;    
//            strcpy(lcdBuff, lcd_setP4);
//            word2DecConverter(P4_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);   
//                break;
//                
//            case BUTTON3:
//            P4_value--;    
//            strcpy(lcdBuff, lcd_setP4);
//            word2DecConverter(P4_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);      
//                break;
//                       
//            case BUTTON4:
//            hmiTask++;    
//            strcpy(lcdBuff, lcd_setP5);
//            word2DecConverter(P5_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff); 
//                break;
//                
//            default:                
//                break;
//        }
//    }
//}
//
//static void hmi_setupL5KeyCommand()
//{
//    char x;
//    x = hmi_scanKeypad();
//    if(x > 0)
//    {
//        switch(x)
//        {
//            case BUTTON1:
//            hmi_ConfigSystemState(HMI_RUN);
//                break;
//                
//            case BUTTON2:
//            P5_value++;    
//            strcpy(lcdBuff, lcd_setP5);
//            word2DecConverter(P5_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);   
//                break;
//                
//            case BUTTON3:
//            P5_value--;    
//            strcpy(lcdBuff, lcd_setP5);
//            word2DecConverter(P5_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);      
//                break;
//                       
//            case BUTTON4:
//            hmiTask++;    
//            strcpy(lcdBuff, lcd_setP6);
//            word2DecConverter(P6_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff); 
//                break;
//                
//            default:                
//                break;
//        }
//    }
//}
//
//static void hmi_setupL6KeyCommand()
//{
//    char x;
//    x = hmi_scanKeypad();
//    if(x > 0)
//    {
//        switch(x)
//        {
//            case BUTTON1:
//            hmi_ConfigSystemState(HMI_RUN);
//            break;
//                
//            case BUTTON2:
//            P6_value++;    
//            strcpy(lcdBuff, lcd_setP6);
//            word2DecConverter(P6_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);   
//            break;
//                
//            case BUTTON3:
//            P6_value--;    
//            strcpy(lcdBuff, lcd_setP6);
//            word2DecConverter(P6_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff);      
//                break;
//                       
//            case BUTTON4:
//            hmiTask = 1;    
//            strcpy(lcdBuff, lcd_setP1);
//            word2DecConverter(P1_value, &lcdBuff[(12)], 4);
//            lcd_msg = strlen(lcdBuff); 
//                break;
//                
//            default:                
//                break;
//        }
//    }
//}

///* Control lamps 
// * Caller: system_userInterface()
// * Move to: hmi.c
// */
//char lmp_update;
//static void hmi_lampStackController()
//{
//    switch(runTasks)
//    {
//        case MC_READY:
//        if(runTasks != lmp_update)
//        {
//            t10 = 0;
//            lmp_state = 0;
//            lmp_update = runTasks;
//            device_outputState(LAMP_RED,STATE_OFF);
//            device_outputState(LAMP_GRN,STATE_ON); 
//        }
//        break;
//            
//        case MC_STDBY:
//        if(runTasks != lmp_update)
//        {
//            t10 = 0;
//            lmp_state = 0;
//            lmp_update = runTasks;
//            device_outputState(LAMP_RED,STATE_OFF);
//            device_outputState(LAMP_GRN,STATE_ON); 
//        }
//        break;
//           
//        case MC_IMNT:
//        if(runTasks != lmp_update)
//        {
//            t10 = 0;
//            lmp_state = 0;
//            lmp_update = runTasks;
//            device_outputState(LAMP_RED,STATE_OFF);
//        }
//        lampStack_imminentSeq(HMI_ANIMATION);
//        break;
//            
//        case MC_FILL:
//        if(runTasks != lmp_update)
//        {
//            t10 = 0;
//            lmp_state = 0;
//            lmp_update = runTasks;
//            device_outputState(LAMP_RED,STATE_OFF);
//        }
//        lampStack_fillSeq(HMI_ANIMATION);    
//        break;
//        
//        case MC_CHK:
//        if(runTasks != lmp_update)
//        {
//            t10 = 0;
//            lmp_state = 0;
//            lmp_update = runTasks;
//            device_outputState(LAMP_RED,STATE_OFF);
//        }
//        lampStack_fillSeq(HMI_ANIMATION);
//        break;
//                
//        case MC_LID:            // 5
//        if(runTasks != lmp_update)
//        {
//            t10 = 0;
//            lmp_state = 0;
//            lmp_update = runTasks;
//            device_outputState(LAMP_GRN,STATE_OFF);
//        }
//        lampStack_errorSeq(HMI_ANIMATION);
//        break;
//                
//        default:
//            break;
//    }
//}

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

///* Control what to display on the LCD
// * Caller: system_userInterface()
// * Callees: displayTemperatureFunction(), hmi_GenerateImminentScreen(),
// * hmi_GenerateFillScreen, word2DecConverter(), hmi_GenerateLidErrorScreen()
// * 
// * Variable:
// * runTasks - use by mainly controllers; set by system_eventHandler() 
// * and check_HopperLidClosed()
// */
//char lcd_update;
//static void hmi_lcdController()
//{
//    char x;
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

//static void load_defaults()
//{
//    P1_value = DEFAULT_P1;
//    P2_value = DEFAULT_P2;
//    P3_value = DEFAULT_P3;
//    P4_value = DEFAULT_P4;
//    P5_value = DEFAULT_P5;
//    P6_value = DEFAULT_P6;
//}

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

static void command_interpreter()
{
    char i;
    switch(cmdTasks)
    {
        case CONNECT:
//        if(comTasks == RDY)   
//        {
//            strcpy(txbuff, msg_startup);
//            txMsg = strlen(txbuff);
//            cmdTasks = MENU;
//        }
        break;
            
        case MENU:
        if(comTasks == RDY)
        {
            strcpy(txbuff, msg_version);
            txMsg = strlen(txbuff);
            cmdTasks = IO;
        }
        break;
        
        case IO:
        if(rxIndx != rxMsg)  
        {
            cmdTasks = SEND;
            i = rxbuff[rxIndx++];
            switch(i)
            {
                case 'E':case 'e':
                strcpy(txbuff, msg_emptyDel);
                
                break;
                
                case 'L': case 'l':
                strcpy(txbuff, msg_lowLev);
                
                break;
                
                case 'C': case 'c':
                strcpy(txbuff, msg_contact);
                
                break;
                
                case 'M': case 'm':
                strcpy(txbuff, msg_monitor);
                break;
                
                case 'I': case 'i':
                strcpy(txbuff, msg_io_test);
                
                break;
                
                default:
                strcpy(txbuff, msg_userErr);   
                cmdTasks = ERROR;
                break;
            }
        }
        break;
        
        case SEND:
        if(comTasks == RDY)
        {
            txMsg = strlen(txbuff);
            cmdTasks = IO;
        }
        break;
        
        case ERROR:
        if(comTasks == RDY)
        {
            txMsg = strlen(txbuff);
            cmdTasks = MENU;
        }
        break;
    }
}

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
    unsigned long x, y;
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

