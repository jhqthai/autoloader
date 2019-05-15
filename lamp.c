/********************************** LAMP! ************************************/
#include "lamp.h"
#include "common.h"

/* Generate lamp imminent sequence 
 * Caller: hmi_lampStackController()
 */
void lampStack_imminentSeq(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        
    if(t10 >= i)
    {   
        t10 = 0;
        switch(lmp_state++)
        {
            case 0:
            device_outputState(LAMP_RED,STATE_OFF);
            device_outputState(LAMP_GRN,STATE_ON);  
                break;

            case (20):
            device_outputState(LAMP_GRN,STATE_OFF);
                break;

            case (40):
            lmp_state = 0;
                break;

            default:
                break;
        }
    }
}

/* Generate lamp fill sequence 
 * Caller: hmi_lampStackController()
 */
void lampStack_fillSeq(unsigned long tmr)
{
    unsigned long i;
    i = tmr;              
    if(t10 >= i)
    {
        t10 = 0;
        switch(lmp_state++)
        {
            case 0:
            device_outputState(LAMP_GRN,STATE_ON);  
                break;

            case (5):
            device_outputState(LAMP_GRN,STATE_OFF);
                break;

            case (10):
            lmp_state = 0;
                break;
                
            default:
                break;
        }
    }
}

/* Generate lamp error sequence 
 * Caller: hmi_lampStackController()
 */
void lampStack_errorSeq(unsigned long tmr)
{
    unsigned long i;
    i = tmr;                                        
    if(t10 >= i)
    {   
        t10 = 0;
        switch(lmp_state++)
        {
            case 0:
            device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (7):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (10):
            lmp_state = 0;
            //device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (45):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (50):
            device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (55):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (60):
            device_outputState(LAMP_RED,STATE_ON);  
                break;

            case (65):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;
                
            case (70):
            device_outputState(LAMP_RED,STATE_ON);  
                break;
                
            case (75):
            device_outputState(LAMP_RED,STATE_OFF);  
                break;

            case (80):
                lmp_state = 0;   
                break;
                
            default:
                break;
        }
    }
}


/* This function is setup the lamp boot sequence
 * Caller: hmi_GenerateBootScreen() 
 */
void lamp_bootSeq()
{
    switch(lmp_state++)
    {
        case 0:
        device_outputState(LAMP_RED,STATE_ON);  
        device_outputState(LAMP_GRN,STATE_OFF); 
            break;

        case (4):
        device_outputState(LAMP_RED,STATE_OFF);  
        device_outputState(LAMP_GRN,STATE_ON); 
            break;

        case (8):
        lmp_state = 0;
            break;
        default:
            break;
    }
}