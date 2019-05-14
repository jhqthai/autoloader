#ifndef __hmi_h__
#define __hmi_h__



//#define TASK_NUM 10         // Maximum number of allowed tasks

// Task routine signature

//typedef void (*TTaskRoutine)(void *userData);
//
//// Task run state
//
//typedef enum {
//    ETaskState_Halt,
//    ETaskState_Run,
//} ETaskState;

// The task handle is the task routine address
// This defines an invalid handle used for error reporting

#define TTaskRoutine_Invalid	NULL



#ifdef	__cplusplus
extern "C" {
#endif

/* UI States */
//extern char hmi_stack; // TODO: unused!

/* TODO: WHERE IS THIS USED?????*/
typedef enum{
    HMI_BOOT = 0,
    HMI_READY,
    HMI_RUN,
    HMI_SETUP,
    HMI_ERROR
}UI_STATE;

extern void hmi_ConfigSystemState(char state); // Since we are just calling it.

/* LCD */
extern void hmi_GenerateBootScreen(unsigned long tmr); // Probably don't need extern for these functions
extern void load_defaults(); // We are not making any changes to it anyway
//extern void hmi_lcdController();
//extern void hmi_lampStackController();

/* Keypad */
extern void hmi_scanSetupCommand(char val);
extern void hmi_SetupStateTimeout(unsigned long timeout);
extern void hmi_setupL1KeyCommand();
extern void hmi_setupL2KeyCommand();
extern void hmi_setupL3KeyCommand();
extern void hmi_setupL4KeyCommand();
extern void hmi_setupL5KeyCommand();
extern void hmi_setupL6KeyCommand();




#ifdef	__cplusplus
}
#endif

#endif	// __hmi_h__
