#ifndef __hmi_h__
#define __hmi_h__

#ifdef	__cplusplus
extern "C" {
#endif

// HMI Animation - ?
#define HMI_ANIMATION   (1000)

// Default parameters
extern int P1_value, P2_value, P3_value, P4_value, P5_value, P6_value;

/* Declare global functions */
// Setup state
extern void hmi_ConfigSystemState(char state);
extern void load_defaults();
extern void hmi_SetupStateTimeout(unsigned long timeout);

// LCD
extern void hmi_GenerateBootScreen(unsigned long tmr);
extern void hmi_lcdController(); // LCD controller

// Lamp
extern void hmi_lampStackController();

// Keypad
extern void hmi_scanSetupCommand(char val);
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
