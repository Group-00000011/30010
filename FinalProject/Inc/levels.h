/*
 * levels.h
 *
 *  Created on: 20 Jun 2023
 *      Author: asbjorn
 */

#ifndef LEVELS_H_
#define LEVELS_H_
#include <stdint.h>
#include "data_structures.h"
#include "entities.h"
#include "hal.h"

void level_setup(listnode_t** enemies_head, uint16_t level,  uint8_t* ground);

#endif /* LEVELS_H_ */
