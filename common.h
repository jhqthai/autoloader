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
    

    
#define HMI_ANIMATION   (1000)

// Global HMI variable
extern char hmiTask;

// Global system control variables
extern char scrn_state, lmp_state, uiState;

// Global timer variables
extern volatile unsigned long t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;

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

// System UI states
typedef enum{
    HMI_BOOT = 0,
    HMI_READY,
    HMI_RUN,
    HMI_SETUP,
    HMI_ERROR
}UI_STATE;

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

