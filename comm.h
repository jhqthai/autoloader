/* 
 * File:   comm.h
 * Author: John
 *
 * Created on 15 May 2019, 8:17 PM
 */

#ifndef COMM_H
#define	COMM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// Serial comm. states
typedef enum{
    INI = 0,
    RDY,
    BUSY
}COM_STATE;

// Declare global comm. variables
extern char comTasks;

// Declare global function
extern void process_SerialComs();
extern void command_interpreter();

#ifdef	__cplusplus
}
#endif

#endif	/* COMM_H */

