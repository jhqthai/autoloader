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
    
// Backlight - only BACKLIGHT10 is used (globally as well). The rest are unused.
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

    
//#define BACKLIGHTFADE_DEL   (10000)
//#define BACKLIGHTFADE_LEV   (6)
    

extern void set_backlight_level(char level);
extern void process_Backlight();





#ifdef	__cplusplus
}
#endif

#endif	/* BACKLIGHT_H */

