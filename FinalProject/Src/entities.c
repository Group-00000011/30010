/*
 * entities.c
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */
#include "entities.h"


static void draw_spaceship(entity_t * self) {

	if(fixp_toint(self->last_x) == fixp_toint(self->x)
			&& fixp_toint(self->last_y) == fixp_toint(self->y)
			&& self->last_rotation == self->rotation)
		return;

switch (self->rotation){
	case 0:
		gfx_clear_area(fixp_toint(self->last_x),fixp_toint(self->last_y),fixp_toint(self->last_x)+6,fixp_toint(self->last_y)+3);
		gotoxy(fixp_toint(self->x),fixp_toint(self->y));
		printf("/%c%c%c", 0xDB,0xDB,0x5C);
		gotoxy(fixp_toint(self->x),fixp_toint(self->y)+1);
		printf("%c  %c", 0xDB,0xDB);
		gotoxy(fixp_toint(self->x),fixp_toint(self->y)+2);
		printf("/%c%c%c", 0xDF,0xDF,0x5C);
		break;
	case 1:
		gfx_clear_area(self->last_x,self->last_y,(self->last_x)+6,(self->last_y)+3);
		gotoxy(self->x,self->y);
		printf("%c%c%c%c%c%c", 0x5C,0xDB,0xDF,0xDF,0xDB,0x5C);
		gotoxy(self->x,self->y+1);
		printf("/%c%c%c%c/", 0xDB,0xDC,0xDC,0xDB);
		break;
	case 2:
		gfx_clear_area(self->last_x,self->last_y,(self->last_x)+6,(self->last_y)+3);
			gotoxy(self->x,self->y);
			printf("%c%c%c/", 0x5C,0xDC,0xDC);
			gotoxy(self->x,self->y+1);
			printf("%c  %c", 0xDB,0xDB);
			gotoxy(self->x,self->y+2);
			printf("%c%c%c/", 0x5C, 0xDB,0xDB);
			break;
		break;
	case 3:
		gfx_clear_area(self->last_x,self->last_y,(self->last_x)+6,(self->last_y)+3);
		gotoxy(self->x,self->y);
		printf("/%c%c%c%c/", 0xDB,0xDF,0xDF,0xDB);
		gotoxy(self->x,self->y+1);
		printf("%c%c%c%c%c%c", 0x5C,0xDB,0xDC,0xDC,0xDB,0x5C);
		break;
	default:
		printf("ERROR");
		break;
}


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

void initialise_entity(entity_t * entity, EntityType type, fixp_t x, fixp_t y, fixp_t rotation) {
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
}


