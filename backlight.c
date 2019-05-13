/****************************************
 * Backlight process section
 ****************************************/

/* This function is currently not being used
 * However it requies BL_LEV which is used by other LCD functions
 * Therefore, move it to LCD as well
 */
#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "backlight.h"


// Backlight
char BL_LEV = 0;
int backlight = 0;
const int LCD_BLL[MAX_BL_LEVS]= {0x0000,0x0200,0x0300,0x0380,0x03C0,0x03E0,0x03F0,0x03F8,0x03FC,0x03FE,0x03FF};

static void lcd_backlight_autodim()
{
    if(BL_LEV > BACKLIGHTFADE_LEV)
    {
        if(t9 >= BACKLIGHTFADE_DEL)
        {
            t9 = 0;
            BL_LEV--;
        }
    } 
}

/* This function is called by system_userInterface() and process_KeyController()
 * Will probably move this to LCD as well.
 */
void set_backlight_level(char level)
{
    BL_LEV = level;
}


// This function set pin hight or low for backlight depending on the timer
void process_Backlight()
{
    unsigned long i;
    if(t4 >= BACKLIGHTSTROBE)
    {
        t4 = 0;
        i = (backlight & BL_LEV_MASK);
        if(i == 1)
        {
            lcd_BL_SetHigh();           
        }
        else
        {
            lcd_BL_SetLow();
        }
        backlight = (backlight >> 1);
        if(backlight == 0)
        {
            backlight = LCD_BLL[BL_LEV];
        }
    }
}
