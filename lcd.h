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

// Global LCD States
typedef enum{
    LCD_INITIALIZE = 0,
    LCD_CONFIG, 
    LCD_RDY,
    LCD_BUSY,
    LCD_WRITE,
    LCD_LINE2HM
}LCD_STATES;

// Global setup LCD state    
#define LCD_RDY_STATE      ((lcd_msg == 0) && (lcd_index == 0) && (lcdState == LCD_RDY))

// Declare global LCD prints
extern const char *lcd_setP1;
extern const char *lcd_setP2;
extern const char *lcd_setP3;
extern const char *lcd_setP4;
extern const char *lcd_setP5;
extern const char *lcd_setP6;

// Declare global LCD variables
extern int fill_cntr; // process fill counter
extern char lcdBuff[];
extern char lcdState, lcd_index, lcd_msg;

// Declare global message variable
extern char msgBuff[];

// Declare global functions
extern void lcd_bootSeq();    
extern void hmi_lcdController();
extern void process_LcdController();


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

