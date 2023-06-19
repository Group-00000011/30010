/*
 * entities.c
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */

#include <stdlib.h>
#include "entities.h"
#include "graphics.h"


static void draw_spaceship(entity_t * self) {
	gfx_clear_area(self->last_x,self->last_y,(self->last_x)+2,(self->last_y)+2);
	gotoxy(self->x,self->y);

	//gotoxy(self->x>>14,self->y>>14);

	printf("s");

}


static void draw_enemy(entity_t * self) {
	gotoxy((self->last_x>>14)+1, (self->last_y>>14)+1);
	printf(" ");
	gotoxy((self->x>>14)+1, (self->y>>14)+1);
	printf("e");
}


static void draw_bullet(entity_t * self) {
	gotoxy(self->x>>14,self->y>>14);
	printf("b");

}

static void draw_bomb(entity_t * self) {
	gotoxy(self->x>>14,self->y>>14);
	printf("B");
}


static void draw_nuke(entity_t * self) {
	gotoxy(self->x>>14,self->y>>14);
	printf("n");
}


static void draw_powerup(entity_t * self) {
	gotoxy(self->x>>14,self->y>>14);
	printf("p");
}

static void update_position(entity_t * self, fixp_t x, fixp_t y) {
	self->last_x = self->x;
	self->last_y = self->y;

	self->x = x;
	self->y = y;
}

static void update_rotation(entity_t * self, fixp_t rotation) {
	self->last_rotation = self->rotation;
	self->rotation = rotation;
}

static uint8_t check_collision(fixp_t x, fixp_t y, uint8_t type, uint8_t* heightmap) {
	// [0] collision with left wall
	// [1] collision with right wall
	// [2] collision with roof
	// [3] collision with ground
	// [4] collision with spaceship
	// [5] collision with bomb

	uint8_t collision = 0;

	if (type & 1) { // Check collisions with sides
		if (x < 0) { // Left wall
			collision |= 1;
		} else if (x > fixp_fromint(DISPLAY_WIDTH-1)) { // Right wall
			collision |= 1<<1;
		}
	}

	if (type & 1<<1) { // Check collisions with ground/roof
		if (y < 0) { // Roof
			collision |= 1<<2;
		} else if (y > fixp_fromint(DISPLAY_HEIGHT-1-heightmap[fixp_toint(x)])) { // Ground
			collision |= 1<<3;
		}
	}



	return collision;
}

entity_t* entity_init(EntityType type, fixp_t x, fixp_t y, fixp_t rotation) {
	entity_t* entity = malloc(sizeof (entity_t));

	entity->type = type;
	entity->x = entity->last_x = x;
	entity->y = entity->last_y = y;
	entity->rotation = entity->last_rotation = rotation;

	switch (type) {
	case Spaceship:
		entity->draw = &draw_spaceship;
		break;
	case Enemy:
		entity->draw = &draw_enemy;
		break;
	case Bullet:
		entity->draw = &draw_bullet;
		break;
	case Bomb:
		entity->draw = &draw_bomb;
		break;
	case Nuke:
		entity->draw = &draw_nuke;
		break;
	case Powerup:
		entity->draw = &draw_powerup;
		break;
	default:
		return NULL;
	}

	entity->update_position = &update_position;
	entity->update_rotation = &update_rotation;
	entity->check_collision = &check_collision;

	return entity;
}

void enemy_move (entity_t* self, uint8_t* heightmap) {
	fixp_t dir = self->rotation;
	fixp_t new_x = self->x;
	new_x += dir;
	fixp_t new_y = fixp_fromint(DISPLAY_HEIGHT-1-heightmap[fixp_toint(new_x)]);

	uint8_t collisions = self->check_collision(new_x, new_y, 1, NULL); // Check collision with walls only

	if (collisions) {
		if (collisions & 1) {
			new_x = 0;
		} else if (collisions & 0b10) {
			new_x = fixp_fromint(DISPLAY_WIDTH-1);
		}
		new_y = fixp_fromint(DISPLAY_HEIGHT-1-heightmap[fixp_toint(new_x)]);
		self->rotation = -dir;
	}

	self->update_position(self, new_x, new_y);
}


