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
	uint8_t (*move)(struct entity* self, uint8_t* heightmap, fixp_t gravity);
	void (*update_position)(struct entity* self, fixp_t x, fixp_t y);
	void (*update_rotation)(struct entity* self);
	void (*update_velocity)(struct entity* self, fixp_t vel_x, fixp_t vel_y);
	uint8_t (*check_collision)(fixp_t x, fixp_t y, uint8_t type, uint8_t* heightmap, struct entity* player);
} entity_t;


entity_t* entity_init(EntityType type, fixp_t x, fixp_t y, fixp_t vel_x, fixp_t vel_y);
uint8_t update_entities (listnode_t* head, listnode_t** aux_list, entity_t* aux_entity, uint8_t* heightmap, fixp_t gravity, fixp_t blast_radius[2], uint8_t enemy_fire_rate);
void cleanup_entities (listnode_t** head, uint8_t* heightmap);
void draw_entities (listnode_t* head, uint8_t* heightmap);

uint8_t enemy_move (entity_t* self, uint8_t* heightmap, fixp_t gravity);
uint8_t entity_move (entity_t* self, uint8_t* heightmap, fixp_t gravity);
uint8_t gravity_move (entity_t* self, uint8_t* heightmap, fixp_t gravity);
uint8_t player_move (entity_t* self, uint8_t* heightmap, fixp_t gravity);

#endif /* ENTITIES_H_ */
