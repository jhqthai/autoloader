/* 
 * File:   backlight.h
 * Author: John
 *
 * Created on 13 May 2019, 9:53 PM
 */

#ifndef BACKLIGHT_H
#define	BACKLIGHT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//                  Backlight
#define BACKLIGHT_0     (0)
#define BACKLIGHT_1     (1)
#define BACKLIGHT_2     (2)
#define BACKLIGHT_3     (3)
#define BACKLIGHT_4     (4)
#define BACKLIGHT_5     (5)
#define BACKLIGHT_6     (6)
#define BACKLIGHT_7     (7)
#define BACKLIGHT_8     (8)
#define BACKLIGHT_9     (9)
#define BACKLIGHT10     (10)

#define MAX_BL_LEVS     (11)
#define BACKLIGHTSTROBE (10) // Require by process_Backlight())
#define BL_LEV_MASK     0x0001
    
#define BACKLIGHTFADE_DEL   (10000)
#define BACKLIGHTFADE_LEV   (6)
    
/* Backlight */
extern char BL_LEV;
/* This function is called by system_userInterface() and process_KeyController() */
extern void set_backlight_level(char level);
extern void process_Backlight();





#ifdef	__cplusplus
}
#endif

#endif	/* BACKLIGHT_H */

