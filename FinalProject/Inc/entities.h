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
	fixp_t x, y, vel_x, vel_y, last_x, last_y;
	uint8_t rotation, last_rotation;
	uint16_t counter;
	uint8_t is_dead;
	void (*draw)(struct entity* self, uint8_t * ground, uint8_t redraw);
	void (*move)(struct entity* self);
	void (*update_position)(struct entity* self, fixp_t x, fixp_t y);
	void (*update_rotation)(struct entity* self, fixp_t rotation);
	void (*update_velocity)(struct entity* self, fixp_t vel_x, fixp_t vel_y);
	uint8_t (*check_collision)(fixp_t x, fixp_t y, uint8_t type, uint8_t* heightmap, struct entity* player);
} entity_t;


entity_t* entity_init(EntityType type, fixp_t x, fixp_t y, fixp_t vel_x, fixp_t vel_y);
void update_entities (listnode_t* head);
void draw_entities (listnode_t* head);

void enemy_move (entity_t* self, uint8_t* heightmap);
void entity_move (entity_t* self);
void gravity_move (entity_t* self, fixp_t g);
uint8_t player_move (entity_t* self, uint8_t* heightmap);

#endif /* ENTITIES_H_ */
