/* 
 * File:   common.h
 * Author: John
 *
 * Created on May 2, 2019, 1:43 PM
 */

#ifndef COMMON_H
#define	COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// Default's parameters
#define DEFAULT_P1 (5)         // Empty low time   (10 seconds)
#define DEFAULT_P2 (1)         // Lid close time   (1 second)
#define DEFAULT_P3 (10)        // Drop away time   (10 seconds)
#define DEFAULT_P4 (2)         // Slide open time  (1 second)
#define DEFAULT_P5 (3)         // Fill high time   (3 seconds)
#define DEFAULT_P6 (6)         // Solenoid minimum ON time   (6 seconds)
extern int P1_value, P2_value, P3_value, P4_value, P5_value, P6_value;

/* Operational parameter*/
#define HMI_ANIMATION   (1000)


/* LCD States*/    
#define LCD_LINES           (2)
#define LCD_CHARS           (16)
#define LCD_SCRNS           (LCD_LINES * LCD_CHARS)
#define LCD_PAGES           (4)
#define LCD_MESSAGES        (LCD_PAGES * LCD_SCRNS)
#define LCD_LINE1           (0)
#define LCD_LINE2           (17)
#define LCD_SCREEN          (3)
#define LCD_BUFFER          (36)
#define LCD_WR_LATENCY      (25)
#define LCD_PWRUP           (10000)
#define LCD_UPDATE_RQT      (lcd_msg > 0)
#define LCD_IDLE            (lcdState == LCD_RDY)
#define LCD_LINE1_HM         0x02
#define LCD_LINE2_HM         0xC0
#define LCD_RDY_STATE      ((lcd_msg == 0) && (lcd_index == 0) && (lcdState == LCD_RDY))
#define refresh_display()   (lcd_msg = (33))
    
/* LCD prints*/
extern const char *lcd_frame;
extern const char *lcd_load;
extern const char *lcd_decode;
extern const char *config_1602A_2L_Blk;     // Configuration of the 1602A LCD display with blink
extern const char *config_1602A_2LnBLK;     // Configuration of the 1602A LCD display with no blink and no underscore
extern const char *config_1602A_2L_UL;     // Configuration of the 1602A LCD display with underscore
extern const char *lcd_ready;
extern const char *lcd_start;
extern const char *lcd_setup;
extern const char *lcd_fillMax;
extern const char *lcd_lowMax;
extern const char *lcd_setP1;
extern const char *lcd_setP2;
extern const char *lcd_setP3;
extern const char *lcd_setP4;
extern const char *lcd_setP5;
extern const char *lcd_setP6;

extern char lcdBuff[]; // This might need to be declare in .c file instead?
extern char lcdState, lcd_index, lcd_msg;

/* LCD tasks*/
extern int lcd_flash_count, fill_cntr, lamp_tmr; // scrn_hdr, scrn_ftr;
extern char lampState, uiState, uiTasks, hmiTask, comTasks, cmdTasks, cmdBuff, sysState, rxIndx, txIndx, rxMsg, txMsg;


/* Lamp parameters */
#define STATE_ON        (1)
#define STATE_OFF       (0)
#define LAMP_GRN        0x01
#define LAMP_RED        0x02
#define LAMPSTACK       0x03
#define RLY_FILL        0x04
#define RLY_GATE        0x08
#define SOLENOIDS       0x0C

/* Message parameters */
#define MSG_CLEAR       (0x00)
#define MSG_HOPPER      (MSG_CLEAR       + LCD_CHARS)
#define MSG_IMMINENT    (MSG_HOPPER      + LCD_CHARS)
#define MSG_FILL_CNT    (MSG_IMMINENT    + LCD_CHARS)
#define MSG_STANDBY     (MSG_FILL_CNT    + LCD_CHARS)
#define MSG_FILLING     (MSG_STANDBY     + LCD_CHARS)
#define MSG_GLUECOLD    (MSG_FILLING     + LCD_CHARS)
#define MSG_GLUELOW     (MSG_GLUECOLD    + LCD_CHARS)
#define MSG_LIDOPEN     (MSG_GLUELOW     + LCD_CHARS)
#define MSG_CHECK       (MSG_LIDOPEN     + LCD_CHARS)
#define MSG_INSPECT     (MSG_CHECK       + LCD_CHARS)
#define MSG_SYSHALT     (MSG_INSPECT     + LCD_CHARS)
#define MSG_STOREL1     (MSG_SYSHALT     + LCD_CHARS)
#define MSG_STOREL2     (MSG_STOREL1     + LCD_CHARS)
#define MSG_BUFFER      (MSG_STOREL2     + LCD_CHARS + 5)

extern char msgBuff[];


/* Serial Comm's*/
extern volatile unsigned long t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
extern char lamp_flash_count, scrn_state, lmp_state, led_state, scrn_hdr, scrn_ftr;

/* Machine states */
typedef enum{
    MC_READY = 0,
    MC_STDBY,
    MC_IMNT,
    MC_FILL,
    MC_CHK,
    MC_LID     
}MACH_STATE;

/* Machine buffers ? */
extern char runTasks, devState, flashRate;

/* I/o Parameters */
#define INPUT_GLUELEVEL 0x80            
#define INPUT_HOPPERLID 0x40
#define INPUT_TEMP_OK   0x20
#define INPUT_5         0x10
#define INPUT_4         0x08
#define INPUT_3         0x04
#define INPUT_2         0x02
#define INPUT_1         0x01

/* I/O buffers?*/
extern char outputDevices, buff, inputDevices; 


// Function mainly used by lamp but is handled in system_eventHandler
extern void device_outputState(char device, char state);

/*
 * Handles word to decimal conversion
 * Caller: hmi_lcdController(), hmi_setLxKeyCommand(), system_userInterface()
 */
extern void word2DecConverter(unsigned int data, char *output, char digits);
    
/****************** 
 * Keypad section *
 ******************/
//extern char keyBuff, keyValue, keyState;
//
//typedef enum{
//    KEY_INI = 0,
//    KEY_SCAN, 
//    KEY_REL
//}KEY_STATES;

/**** Keypad section ends *****/

#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

