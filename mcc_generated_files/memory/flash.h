/**
  FLASH Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    flash.h

  @Summary
    This is the generated header file for the FLASH driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for FLASH.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  dsPIC33EP64MC206
        Driver Version    :  1.00
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB 	          :  MPLAB X v5.10
*/

#ifndef FLASH_H
#define FLASH_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS 1024

#define FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS  (FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS*2)

#define FLASH_MIN_WRITE_SIZE 4 

#define FLASH_UNLOCK_KEY 0x00AA0055

#define PAGE_MASK (~((FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS*2) - 1)) 
void     FLASH_Unlock(uint32_t  key);
void     FLASH_Lock(void);

bool     FLASH_ErasePage(uint32_t address);    
bool     MEMORY_SegmentErase(uint32_t address); 
uint16_t FLASH_ReadWord16(uint32_t address);
uint32_t FLASH_ReadWord24(uint32_t address);

bool     FLASH_WriteDoubleWord16(uint32_t flashAddress, uint16_t Data0, uint16_t Data1);
bool     FLASH_WriteDoubleWord24(uint32_t address, uint32_t Data0, uint32_t Data1  );


uint16_t FLASH_GetErasePageOffset(uint32_t address);
uint32_t FLASH_GetErasePageAddress(uint32_t address);




#endif	/* FLASH_H */

