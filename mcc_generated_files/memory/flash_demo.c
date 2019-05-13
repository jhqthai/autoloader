/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "flash.h"

extern __attribute__((space(prog))) uint32_t _PROGRAM_END;

void FlashDemo()
{
    uint32_t flash_storage_address;
    uint16_t result;
    uint32_t write_data[2];
    uint32_t read_data[2];
    uint32_t last_code_location;
    
//    int addrOffset;
//    int varWord1;
//    int varWord2;
//    TBLPAG = ((PROG_ADDR & 0x7F0000)>>16);
//    addrOffset = (PROG_ADDR & 0x00FFFE);
//    varWord1 = __builtin_tblrdl(addrOffset);
//    varWord2 = __builtin_tblrdh(addrOffset);

    // Get the address of last code in flash
    last_code_location = ((uint32_t)(__prog__ uint32_t*)&_PROGRAM_END) & 0xFFFFFF;

    // Goto next page after this.
    flash_storage_address = FLASH_GetErasePageAddress(last_code_location) + (2*FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS);

    FLASH_Unlock(FLASH_UNLOCK_KEY);

    // Erase the page of flash at this address
    result = MEMORY_SegmentErase(flash_storage_address);

    // Write 24 bit Data to the first location.
    // For this product we must write two adjacent words at a one time.
    write_data[0] = 0x00112233;
    write_data[1] = 0x00445566;
    result = FLASH_WriteDoubleWord24(flash_storage_address, write_data[0], write_data[1]);
    
    flash_storage_address += 4;
    write_data[0] = 0x00778899;
    write_data[1] = 0x00AABBCC;
    result = FLASH_WriteDoubleWord24(flash_storage_address, write_data[0], write_data[1]);
    
    // read the data to verify the data
    read_data[0] = FLASH_ReadWord24(flash_storage_address);
    read_data[1] = FLASH_ReadWord24(flash_storage_address+2);

    // Stop if the read data does not match the write data;
//    if ( (write_data[0] != read_data[0]) ||
//         (write_data[1] != read_data[1]) )
//    {
//    
//        while(1);
//    }
    //flash_storage_address += 6;
//    last_code_location = ((uint32_t)(__prog__ uint32_t*)&_PROGRAM_END) & 0xFFFFFF;
    // Goto next page after this.
    flash_storage_address = FLASH_GetErasePageAddress(last_code_location) + (2*FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS);
// _MEMORY_SegmentErase
//    FLASH_Unlock(FLASH_UNLOCK_KEY);

    // Erase the page of flash at this address
    result = MEMORY_SegmentErase(flash_storage_address);

    // Write 24 bit Data to the first location.
    // For this product we must write two adjacent words at a one time.
//    write_data[0] = 0x00778899;
//    write_data[1] = 0x00AABBCC;
//    result = FLASH_WriteDoubleWord24(flash_storage_address, write_data[0], write_data[1]);
    
    // read the data to verify the data
    read_data[0] = FLASH_ReadWord24(flash_storage_address);
    read_data[1] = FLASH_ReadWord24(flash_storage_address+2);
    if ( (write_data[0] != read_data[0]) ||
         (write_data[1] != read_data[1]) )
    {
    
        while(1);
    }
//    asm ("MOV #tblpage(flash_storage_address),W0");
//    asm ("MOV W0,TBLPAG");
//    asm ("MOV #tbloffset(flash_storage_address+2),W0");
//    asm ("TBLRDH [W0],W3");
//    asm ("TBLRDL [W0],W4");
        
//    ; Set up the address pointer to program space
//MOV #tblpage(PROG_ADDR),W0 ; get table page value
//MOV W0,TBLPAG ; load TBLPAG register
//MOV #tbloffset(PROG_ADDR),W0 ; load address LS word
//; Read the program memory location
//TBLRDH [W0],W3 ; Read high byte to W3
//TBLRDL [W0],W4 ; Read low word to W4
        
    
}

