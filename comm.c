// Serial communication driver

#include <xc.h>
// Local includes
#include "comm.h"



// Local serial comm. parameters
#define TX_BUFFER       (128)
#define RX_BUFFER       (255)
#define UART1_BRG       (((FCY / UART1_BAUD)/16) - 1)

// USB interface host command messages - Communication stuff
const char *msg_startup     = "\n\r *** DECODE AUTOMATION ***      \0";
const char *msg_version     = "\n\r    Autoloader Ver-1.00         \0";
const char *msg_emptyDel    = "\n\r Press 'E' enter 'empty' delay  \0";
const char *msg_lowLev      = "\n\r Press 'L' to adjust low level  \0";
const char *msg_contact     = "\n\r Press 'C' for contact details  \0";
const char *msg_monitor     = "\n\r Press 'M' for debug monitoring \0";
const char *msg_io_test     = "\n\r Press 'T' for I/O test         \0";
const char *msg_userErr     = "\n\r    Invalid selection!          \0";
const char *msg_debugRdy    = "\n\r READY.    The system will run when start is pressed              \0";
const char *msg_debugRun    = "\n\r MONTIORING.    Monitoring the glue level, engage host 'dry' contact         \0";
const char *msg_debugFill   = "\n\r HOPPER FILL.   Detected a low hopper glue level. Engage 'fill' solenoid     \0";
const char *msg_debugStFl   = "\n\r FILL STOP.     Disengage 'fill' solenoid     \0";
const char *msg_debugLid    = "\n\r SYSTEM HALT.   Detected the lid open. All outputs disengaged. Close to continue\0";
const char *msg_debugErr    = "\n\r SYSTEM ERROR.  Hopper fill timeout failure. Check hopper before proceeding  \0";
const char *msg_debugCls    = "\n\r Resuming...    Hopper lid closed  \0";

// Command intepreter states
typedef enum{
    CONNECT = 0,
    MENU,
    IO,
    SEND,
    ERROR
}HOST_CMD;

// Define global comm. variables
char comTasks;

// Local communication variables
char cmdTasks, rxIndx, txIndx, rxMsg, txMsg;
char txbuff[TX_BUFFER], rxbuff[RX_BUFFER];

/* This function processes UART input
 * 
 */ 
void process_SerialComs()
{
    char i;
    switch(comTasks)
    {
        case INI:
        comTasks = RDY;
        rxMsg = 0, txMsg = 0;
        rxIndx = 0, txIndx = 0;
        break;
            
        case RDY:
        if(U1STAbits.URXDA == 1) 
        {
            i = U1RXREG;
            rxbuff[rxMsg++] = i;
        }
        if(txIndx != txMsg)    
        {
            comTasks = BUSY;
        }
        break;
            
        case BUSY:
        if(U1STAbits.URXDA == 1) 
        {
            i = U1RXREG;
            rxbuff[rxMsg++] = i;
        }
        if(U1STAbits.UTXBF == 0 && U1STAbits.TRMT == 1)
        {
            i = txbuff[txIndx++];
            U1TXREG = i;
            if(txIndx == txMsg)
            {
                txIndx = 0;
                txMsg = 0;
                comTasks = RDY;
            }
        }
        break;
    }
}

/* This function set the transmit buffer depending on the receive buffer processed
 * by process_SerialComs()
 */
void command_interpreter()
{
    char i;
    switch(cmdTasks)
    {
        case CONNECT:
//        if(comTasks == RDY)   
//        {
//            strcpy(txbuff, msg_startup);
//            txMsg = strlen(txbuff);
//            cmdTasks = MENU;
//        }
        break;
            
        case MENU:
        if(comTasks == RDY)
        {
            strcpy(txbuff, msg_version);
            txMsg = strlen(txbuff);
            cmdTasks = IO;
        }
        break;
        
        case IO:
        if(rxIndx != rxMsg)  
        {
            cmdTasks = SEND;
            i = rxbuff[rxIndx++];
            switch(i)
            {
                case 'E':case 'e':
                strcpy(txbuff, msg_emptyDel);
                
                break;
                
                case 'L': case 'l':
                strcpy(txbuff, msg_lowLev);
                
                break;
                
                case 'C': case 'c':
                strcpy(txbuff, msg_contact);
                
                break;
                
                case 'M': case 'm':
                strcpy(txbuff, msg_monitor);
                break;
                
                case 'I': case 'i':
                strcpy(txbuff, msg_io_test);
                
                break;
                
                default:
                strcpy(txbuff, msg_userErr);   
                cmdTasks = ERROR;
                break;
            }
        }
        break;
        
        case SEND:
        if(comTasks == RDY)
        {
            txMsg = strlen(txbuff);
            cmdTasks = IO;
        }
        break;
        
        case ERROR:
        if(comTasks == RDY)
        {
            txMsg = strlen(txbuff);
            cmdTasks = MENU;
        }
        break;
    }
}
