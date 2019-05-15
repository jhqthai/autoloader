/* 
 * File:   tone.h
 * Author: John
 *
 * Created on 13 May 2019, 10:56 PM
 */

#ifndef TONE_H
#define	TONE_H

#ifdef	__cplusplus
extern "C" {
#endif

// Global tone states
typedef enum{
    WAVE_INI = 0,
    WAVE_IDLE,      
    WAVE_LOAD,
    WAVE_PLAY
}TONE_STATE;

extern char wav_tasks;
extern void process_ToneGenerator();


#ifdef	__cplusplus
}
#endif

#endif	/* TONE_H */

