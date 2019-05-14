/* 
 * File:   lcd.h
 * Author: John
 *
 * Created on 14 May 2019, 10:50 AM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

// TODO: Might be able to make this local    
typedef enum{
    LCD_INITIALIZE = 0,
    LCD_CONFIG, 
    LCD_RDY,
    LCD_BUSY,
    LCD_WRITE,
    LCD_LINE2HM
}LCD_STATES;
    
extern void hmi_lcdController();
extern void process_LcdController();


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

