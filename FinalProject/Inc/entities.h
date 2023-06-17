/*
 * entities.h
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */

#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <string.h>
#include "ansi.h"
#include "graphics.h"
#include "data_structures.h"


typedef enum EntityType {Spaceship, Enemy, Bullet, Bomb, Nuke, Powerup} EntityType;

typedef struct entity {
	EntityType type;
	fixp_t x, y, rotation, last_x, last_y, last_rotation;
	void (*draw)(struct entity* self);
	void (*update_position)(struct entity* self, fixp_t x, fixp_t y);
	void (*update_rotation)(struct entity* self, fixp_t rotation);
	uint8_t (*check_collision)(struct entity* self, uint8_t type, uint8_t* heightmap);
} entity_t;


entity_t* entity_init(EntityType type, fixp_t x, fixp_t y, fixp_t rotation);


#endif /* ENTITIES_H_ */
