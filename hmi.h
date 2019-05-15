#ifndef __hmi_h__
#define __hmi_h__

#ifdef	__cplusplus
extern "C" {
#endif


/* LCD */
extern void hmi_GenerateBootScreen(unsigned long tmr); // Probably don't need extern for these functions
extern void load_defaults(); // We are not making any changes to it anyway

/* Keypad */
extern void hmi_SetupStateTimeout(unsigned long timeout);





#ifdef	__cplusplus
}
#endif

#endif	// __hmi_h__
