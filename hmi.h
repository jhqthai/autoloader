#ifndef __hmi_h__
#define __hmi_h__

#ifdef	__cplusplus
extern "C" {
#endif


// LCD
extern void hmi_GenerateBootScreen(unsigned long tmr); // Probably don't need extern for these functions
extern void load_defaults(); // We are not making any changes to it anyway

// Setup state
extern void hmi_SetupStateTimeout(unsigned long timeout);

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
