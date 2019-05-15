/********************************** LAMP! ************************************/
#include "lamp.h"
#include "common.h"

/* Generate lamp imminent sequence 
 * Caller: hmi_lampStackController()
 */
static void lampStack_imminentSeq(unsigned long tmr)
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
static void lampStack_fillSeq(unsigned long tmr)
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
static void lampStack_errorSeq(unsigned long tmr)
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

/* Lamp control function
 * Caller: system_userInterface()
 */
void hmi_lampStackController()
{
    static char lmp_update;
    switch(runTasks)
    {
        case MC_READY:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
            device_outputState(LAMP_GRN,STATE_ON); 
        }
        break;
            
        case MC_STDBY:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
            device_outputState(LAMP_GRN,STATE_ON); 
        }
        break;
           
        case MC_IMNT:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
        }
        lampStack_imminentSeq(HMI_ANIMATION);
        break;
            
        case MC_FILL:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
        }
        lampStack_fillSeq(HMI_ANIMATION);    
        break;
        
        case MC_CHK:
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_RED,STATE_OFF);
        }
        lampStack_fillSeq(HMI_ANIMATION);
        break;
                
        case MC_LID:            // 5
        if(runTasks != lmp_update)
        {
            t10 = 0;
            lmp_state = 0;
            lmp_update = runTasks;
            device_outputState(LAMP_GRN,STATE_OFF);
        }
        lampStack_errorSeq(HMI_ANIMATION);
        break;
                
        default:
            break;
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