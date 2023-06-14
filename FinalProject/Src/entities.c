/*
 * entities.c
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */
#include "entities.h"


static void draw_spaceship(entity_t * self) {
	gotoxy(self->x,self->y);
	printf("s");
}
static void draw_enemy(entity_t * self) {
	gotoxy(self->x,self->y);
	printf("e");
}
static void draw_bullet(entity_t * self) {
	gotoxy(self->x,self->y);
	printf("b");
}
static void draw_bomb(entity_t * self) {
	gotoxy(self->x,self->y);
	printf("B");
}
static void draw_nuke(entity_t * self) {
	gotoxy(self->x,self->y);
	printf("n");
}
static void draw_powerup(entity_t * self) {
	gotoxy(self->x,self->y);
	printf("p");
}



void initialise_entity(entity_t * entity, EntityType type, uint8_t x, uint8_t y, uint8_t rotation) {
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
}


