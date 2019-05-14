#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "hmi.h"

// Default parameters
int P1_value, P2_value, P3_value, P4_value, P5_value, P6_value;

/* LCD tasks*/
char lampState, uiState, uiTasks, hmiTask, comTasks, cmdTasks, cmdBuff, sysState, rxIndx, txIndx, rxMsg, txMsg = 0;
int lcd_flash_count, fill_cntr, lamp_tmr; // scrn_hdr, scrn_ftr;
char lamp_flash_count, scrn_state, lmp_state, scrn_hdr, scrn_ftr; // , led_state

/* Machine states? */
char runTasks, devState, flashRate = 0;



/* I/O Buffers?*/
char outputDevices, buff, inputDevices = 0; 


void device_outputState(char device, char state)
{
    char x,y;
    x = device;
    y = state;
    if(y > 0)
    {
        outputDevices |=  x;
    }
    else
    {
        outputDevices &= ~x;
    }
}

const char *hexAscii = "0123456789ABCDEF";
void word2DecConverter(unsigned int data, char *output, char digits)
{
    if (digits > 5)
        return;

    char d[5];
    d[0] = (char)(data / 10000);
    data -= (unsigned int)d[0] * 10000;
    d[1] = (char)(data / 1000);
    data -= (unsigned int)d[1] * 1000;
    d[2] = (char)(data / 100);
    data -= (unsigned int)d[2] * 100;
    d[3] = (char)(data / 10);
    data -= (unsigned int)d[3] * 10;
    d[4] = (char)data;

    // Convert to ascii (only 0..9 values)

    char ascii[5];
    int dIndex = 0;
    for (dIndex = 0; dIndex < 5; dIndex++ )
    {
        ascii[dIndex] = hexAscii[d[dIndex]];
    }

    // Supress leading zeroes if required
//    if (!leadingZeroes)
//    {
//        if (ascii[0] == '0')
//            ascii[0] = ' ';
//        if (ascii[0] == ' ' && ascii[1] == '0')
//            ascii[1] = ' ';
//        if (ascii[0] == ' ' && ascii[1] == ' ' && ascii[2] == '0')
//            ascii[2] = ' ';
//        if (ascii[0] == ' ' && ascii[1] == ' '  && ascii[2] == ' ' && ascii[3] == '0')
//            ascii[3] = ' ';
//    }

    // Copy over the result
    for (dIndex = 0; dIndex < digits; dIndex++)
    {
        output[dIndex] = ascii[(5-digits)+dIndex];
    }
}