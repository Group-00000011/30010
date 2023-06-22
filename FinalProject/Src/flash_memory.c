/*
 * flash_memory.c
 *
 *  Created on: 22 Jun 2023
 *      Author: asbjorn
 */
#include "flash_memory.h"



uint16_t flash_read_halfword(uint16_t offset) {
	// Base adress of flash memory
	uint32_t baseAddress = 0x0800F800;

	// Read at address + offset
	return *(uint16_t *)(baseAddress + offset * 2);
}


uint16_t flash_write_halfword(uint16_t offset, uint16_t data) {
	// This function writes a halfword to flash memory at the location baseaddress + offset.
	// The offset is a bit useless since it erases all the other data in that page, so could just as well write without offset


	// Base adress of flash memory
	uint32_t baseAddress = 0x0800F800;

	// Unlock flash memory
	FLASH_Unlock();
	FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );

	// Erase page at base address
	FLASH_ErasePage( baseAddress );


	// Write half word data to offset
	FLASH_ProgramHalfWord(baseAddress+2*offset, data);

	// Lock flash memory
	FLASH_Lock();
}
