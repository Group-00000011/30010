/*
 * entities.h
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */

#ifndef ENTITIES_H_
#define ENTITIES_H_
#include "ansi.h"
#include "graphics.h"
#include <string.h>


typedef enum EntityType {Spaceship, Enemy, Bullet, Bomb, Nuke, Powerup} EntityType;

typedef struct entity {
	EntityType type;
	uint8_t x, y, rotation, last_x, last_y, last_rotation;
	void (*draw)(struct entity* self);
	void (*update_position)(struct entity* self, uint8_t x, uint8_t y);
	void (*update_rotation)(struct entity* self, uint8_t rotation);
} entity_t;


void initialise_entity(entity_t * entity, EntityType type, uint8_t x, uint8_t y, uint8_t rotation);


#endif /* ENTITIES_H_ */
