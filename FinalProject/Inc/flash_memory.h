/*
 * flash_memory.h
 *
 *  Created on: 22 Jun 2023
 *      Author: asbjorn
 */

#ifndef FLASH_MEMORY_H_
#define FLASH_MEMORY_H_


#include "stm32f30x_conf.h"
#include <stdint.h>


uint16_t flash_read_halfword(uint16_t offset);


void flash_write_halfword(uint16_t offset, uint16_t data);


#endif /* FLASH_MEMORY_H_ */
