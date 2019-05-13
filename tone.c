#include <xc.h>

#include "common.h"
#include "tone.h"

char wav_tasks;

unsigned int wav[(255)], wav_index, wav_msg, wav_tempo = 0; // TODO: UNUSED
unsigned int notes[]   = {(191),(191),(170),(191),(143),(151)}; // TODO: Make const
unsigned int tempo[]   = {(10000),(8000),(12000),(10000),(10000),(40000)}; // TODO: Make const

unsigned int freq, pitch; // TODO: Make freq local variable, delete pitch -> no use
static void wav_playTone(unsigned int tone)
{
    unsigned int i;
    i = tone;
    if(IFS0bits.T3IF)
    {
        IFS0bits.T3IF = 0;
        if(freq++ >= i)
        {
            freq = 0;
            LATBbits.LATB8 = ~LATBbits.LATB8;       // Oscillate the tone pin
        }
    }
}

/* Generate tone
 * Caller: main
 * Callee:
 * 
 * Variables: wave_tasks - modified by process_ToneGenerator() and system_eventHandler()
 */
int tune = 0;
void process_ToneGenerator()
{
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