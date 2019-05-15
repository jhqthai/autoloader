// Tone driver module
#include <xc.h>
#include "common.h"
#include "tone.h"

// Tone array
const unsigned int notes[]   = {(191),(191),(170),(191),(143),(151)};
const unsigned int tempo[]   = {(10000),(8000),(12000),(10000),(10000),(40000)};

// Define global variables
char wav_tasks; // Event variable

static void wav_playTone(unsigned int tone)
{
    static unsigned int freq; // TODO: Why is this not defined as 0 initially?
    unsigned int i;
    i = tone;
    if(IFS0bits.T3IF)
    {
        IFS0bits.T3IF = 0;
        if(freq++ >= i)
        {
            freq = 0;
            LATBbits.LATB8 = ~LATBbits.LATB8; // Oscillate the tone pin
        }
    }
}

/* This function generates tone
 * Caller: main
 */
void process_ToneGenerator()
{
    static int tune = 0;
    switch(wav_tasks)
    {
        case WAVE_INI:
        t11 = 0, tune = 0;
        LATBbits.LATB8 = 0;
        wav_tasks = WAVE_IDLE;
        break;
        
        case WAVE_IDLE:
        
        break;     
        
        case WAVE_LOAD:
            
        LATBbits.LATB8 = 0;
        break;
        
        case WAVE_PLAY:
        wav_playTone(notes[tune]);
        if(t11 >= tempo[tune])    
        {
            t11 = 0;
            if(tune++ >= 5)
            {
                t11 = 0, tune = 0;
                LATBbits.LATB8 = 0;
                wav_tasks = WAVE_INI;
            }
        }
        break;  
    }
}