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


/* Serial Comm's*/
extern volatile unsigned long t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
extern char lamp_flash_count, scrn_state, lmp_state, scrn_hdr, scrn_ftr; //, led_state

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
    

#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

