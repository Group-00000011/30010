/*
 * entities.c
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */

#include <stdlib.h>
#include "entities.h"
#include "graphics.h"


static void draw_spaceship(entity_t * self, uint8_t redraw) { // If !redraw -> only erase

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


static void draw_enemy(entity_t * self, uint8_t redraw) {
	gotoxy((self->last_x>>14)+1, (self->last_y>>14)+1);
	printf(" ");
	if (redraw) {
		gotoxy((self->x>>14)+1, (self->y>>14)+1);
		printf("e");
	}
}


static void draw_bullet(entity_t * self, uint8_t redraw) {
	gotoxy((self->last_x>>14)+1, (self->last_y>>14)+1);
	printf(" ");
	if (redraw) {
		gotoxy((self->x>>14)+1, (self->y>>14)+1);
		printf("b");
	}

}

static void draw_bomb(entity_t * self, uint8_t redraw) {
	gotoxy(self->x>>14,self->y>>14);
	printf("B");
}


static void draw_nuke(entity_t * self, uint8_t redraw) {
	gotoxy(self->x>>14,self->y>>14);
	printf("n");
}


static void draw_powerup(entity_t * self, uint8_t redraw) {
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

static uint8_t check_collision(fixp_t x, fixp_t y, uint8_t type, uint8_t* heightmap, entity_t* player) { // [0]=walls, [1]=roof, [2]=ground, [3]=player
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
		} else if (type & 1<<2 && y > fixp_fromint(DISPLAY_HEIGHT-1-heightmap[fixp_toint(x)])) { // Ground
			collision |= 1<<3;
		}
	}

	if (type && 1<<3) {
		// Check collision with player
	}

	return collision;
}

entity_t* entity_init(EntityType type, fixp_t x, fixp_t y, fixp_t vel_x, fixp_t vel_y) {
	entity_t* entity = malloc(sizeof (entity_t));

	entity->type = type;
	entity->x = entity->last_x = x;
	entity->y = entity->last_y = y;
	entity->vel_x = vel_x;
	entity->vel_y = vel_y;
	entity->counter = 0;

	switch (type) {
	case Spaceship:
		entity->draw = &draw_spaceship;
		break;
	case Enemy:
		entity->draw = &draw_enemy;
		entity->rotation = vel_x < 0;
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
	fixp_t new_x = self->x + self->vel_x;
	fixp_t new_y = fixp_fromint(DISPLAY_HEIGHT-1-heightmap[fixp_toint(new_x)]);

	uint8_t collisions = self->check_collision(new_x, new_y, 1, NULL, NULL); // Check collision with walls only

	if (collisions) {
		if (collisions & 1) {
			new_x = 0;
		} else if (collisions & 0b10) {
			new_x = fixp_fromint(DISPLAY_WIDTH-1);
		}
		new_y = fixp_fromint(DISPLAY_HEIGHT-1-heightmap[fixp_toint(new_x)]);
		self->vel_x = -self->vel_x;
		self->rotation = self->vel_x > 0;
	}

	self->update_position(self, new_x, new_y);
}

void entity_move (entity_t* self) {
	self->update_position(self, self->x + self->vel_x, self->y+self->vel_y);
	//self->x = self->x + self->vel_x;
	//self->y = self->y + self->vel_y;
}
