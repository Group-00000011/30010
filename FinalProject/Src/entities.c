/*
 * entities.c
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */

#include <stdlib.h>
#include "entities.h"


static void draw_spaceship(entity_t * self) {
	gotoxy(self->x>>14,self->y>>14);
	printf("s");
}


static void draw_enemy(entity_t * self) {
	gotoxy(self->x>>14,self->y>>14);
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
	}

	entity->update_position = &update_position;
	entity->update_rotation = &update_rotation;

	return entity;
}


