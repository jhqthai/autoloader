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
    
//                  Tone generator
#define KEY_DUR         (200)
#define ACK_DUR         (1000)

#define PITCH_MAX  (2)
#define A5      ((113) / 2)     // 880 Hertz 
#define A5b     ((120) / 2)     // 830.6094 Hertz
#define B5      ((101) / 2)     // 987.7666 Hertz 
#define B5b     ((107) / 2)     // 932.3275 Hertz
#define C5      ((191) / 2)     // 523.2511 Hertz
#define D5      ((170) / 2)     // 587.3295 Hertz
#define D5b     ((180) / 2)     // 554.3653 Hertz
#define E5      ((151) / 2)     // 659.2551 Hertz
#define F5      ((143) / 2)     // 698.4565 Hertz 
#define G5      ((127) / 2)     // 783.9909 Hertz
#define G5b     ((135) / 2)     // 739.9888 Hertz

#define A4      ((227) / 2)     // 440 Hertz 
#define A4b     ((240) / 2)     // 415.3047 Hertz
#define B4      ((202) / 2)     // 493.8833 Hertz 
#define B4b     ((214) / 2)     // 466.1638 Hertz
#define C4      ((382) / 2)     // 261.6256 Hertz
#define D4      ((340) / 2)     // 293.6648 Hertz
#define D4b     ((360) / 2)     // 277.1826 Hertz
#define E4      ((303) / 2)     // 329.6276 Hertz
#define F4      ((286) / 2)     // 349.2282 Hertz 
#define G4      ((255) / 2)     // 391.9954 Hertz
#define G4b     ((270) / 2)     // 369.9944 Hertz

#define A3      ((454) / 2)     // 220 Hertz
#define A3b     ((481) / 2)     // 207.6523 Hertz
#define B3      ((405) / 2)     // 246.9417 Hertz
#define B3b     ((429) / 2)     // 233.0819 Hertz

// Tone states
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

