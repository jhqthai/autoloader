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


// Global setup LCD state 
#define LCD_LINES           (2)
#define LCD_CHARS           (16)
#define LCD_SCRNS           (LCD_LINES * LCD_CHARS)
#define LCD_PAGES           (4)
#define LCD_MESSAGES        (LCD_PAGES * LCD_SCRNS)
#define LCD_LINE1           (0)
#define LCD_LINE2           (17)
#define LCD_SCREEN          (3)
#define LCD_BUFFER          (36)
#define LCD_WR_LATENCY      (25)
#define LCD_PWRUP           (10000)
#define LCD_UPDATE_RQT      (lcd_msg > 0)
#define LCD_IDLE            (lcdState == LCD_RDY)
#define LCD_LINE1_HM         0x02
#define LCD_LINE2_HM         0xC0
#define refresh_display()   (lcd_msg = (33))
#define LCD_RDY_STATE      ((lcd_msg == 0) && (lcd_index == 0) && (lcdState == LCD_RDY))

// Global LCD States
typedef enum{
    LCD_INITIALIZE = 0,
    LCD_CONFIG, 
    LCD_RDY,
    LCD_BUSY,
    LCD_WRITE,
    LCD_LINE2HM
}LCD_STATES;

// Declare global LCD prints
extern const char *lcd_ready;
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

// Global message parameter
#define MSG_CLEAR       (0x00)
#define MSG_HOPPER      (MSG_CLEAR       + LCD_CHARS)
#define MSG_IMMINENT    (MSG_HOPPER      + LCD_CHARS)
#define MSG_FILL_CNT    (MSG_IMMINENT    + LCD_CHARS)
#define MSG_STANDBY     (MSG_FILL_CNT    + LCD_CHARS)
#define MSG_FILLING     (MSG_STANDBY     + LCD_CHARS)
#define MSG_GLUECOLD    (MSG_FILLING     + LCD_CHARS)
#define MSG_GLUELOW     (MSG_GLUECOLD    + LCD_CHARS)
#define MSG_LIDOPEN     (MSG_GLUELOW     + LCD_CHARS)
#define MSG_CHECK       (MSG_LIDOPEN     + LCD_CHARS)
#define MSG_INSPECT     (MSG_CHECK       + LCD_CHARS)
#define MSG_SYSHALT     (MSG_INSPECT     + LCD_CHARS)
#define MSG_STOREL1     (MSG_SYSHALT     + LCD_CHARS)
#define MSG_STOREL2     (MSG_STOREL1     + LCD_CHARS)
#define MSG_BUFFER      (MSG_STOREL2     + LCD_CHARS + 5)

// Declare global message variable
extern char msgBuff[];

// Declare global functions
extern void lcd_bootSeq();    
extern void process_LcdController();
extern void displayTemperatureFunction();
extern void hmi_GenerateImminentScreen(unsigned long tmr);
extern void hmi_GenerateFillScreen(unsigned long tmr);
extern void hmi_GenerateLidErrorScreen(unsigned long tmr);





#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

