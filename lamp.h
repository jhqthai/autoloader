/* 
 * File:   lamp.h
 * Author: John
 *
 * Created on 14 May 2019, 1:22 PM
 */

#ifndef LAMP_H
#define	LAMP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/* Lamp parameters */
#define STATE_ON        (1)
#define STATE_OFF       (0)
#define LAMP_GRN        0x01
#define LAMP_RED        0x02
#define LAMPSTACK       0x03
#define RLY_FILL        0x04
#define RLY_GATE        0x08
#define SOLENOIDS       0x0C


extern void hmi_lampStackController();
extern void lamp_bootSeq();

#ifdef	__cplusplus
}
#endif

#endif	/* LAMP_H */

