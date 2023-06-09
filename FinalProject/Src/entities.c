/*
 * entities.c
 *
 *  Created on: 13 Jun 2023
 *      Author: asbjorn
 */

#include <stdlib.h>
#include <stdint.h>
#include "entities.h"
#include "graphics.h"


static void draw_spaceship(entity_t* self, uint8_t* ground, uint8_t redraw) {

	fixp_t last_x = fixp_toint(self->last_x);
	fixp_t last_y = fixp_toint(self->last_y);

	fixp_t x = fixp_toint(self->x);
	fixp_t y = fixp_toint(self->y);

	// Clears last spaceship and sets spaceship colour to white.
	gfx_clear_area(ground, last_x, last_y,last_x+5,last_y+2);
	bgcolor(0);
	fgcolor(7);

	//Switches through possible rotations and draws spaceship accordingly
	//0 = up, 1 = right, 2 = down, 3 = left
	switch (self->rotation){
		case 0:
			gotoxy(x, y);
			printf("/%c%c%c", 0xDB,0xDB,0x5C);
			gotoxy(x, y+1);
			printf("%c  %c", 0xDB,0xDB);
			gotoxy(x, y+2);
			printf("/%c%c%c", 0xDF,0xDF,0x5C);
			break;
		case 1:
			gotoxy(x, y);
			printf("%c%c%c%c%c%c", 0x5C,0xDB,0xDF,0xDF,0xDB,0x5C);
			gotoxy(x, y+1);
			printf("/%c%c%c%c/", 0xDB,0xDC,0xDC,0xDB);
			break;
		case 2:
			gotoxy(x, y);
			printf("%c%c%c/", 0x5C,0xDC,0xDC);
			gotoxy(x, y+1);
			printf("%c  %c", 0xDB,0xDB);
			gotoxy(x, y+2);
			printf("%c%c%c/", 0x5C, 0xDB,0xDB);
			break;
		case 3:
			gotoxy(x,y);
			printf("/%c%c%c%c/", 0xDB,0xDF,0xDF,0xDB);
			gotoxy(x,y+1);
			printf("%c%c%c%c%c%c", 0x5C,0xDB,0xDC,0xDC,0xDB,0x5C);
			break;
		default:
			printf("ERROR");
			break;
	}
}

static void draw_enemy(entity_t * self, uint8_t  * ground, uint8_t redraw) {

	fixp_t last_x = (self->last_x) >> 14;
	fixp_t last_y = (self->last_y) >> 14;
	fixp_t x = (self->x) >> 14;
	fixp_t y = (self->y) >> 14;

	// Doesnt draw if at border to avoid overflows
	if (x<2||x>254) redraw=0;

	// Clear background at last position.
	if( last_x < 255 && last_x > 1) gfx_clear_area(ground, last_x - 1, last_y, last_x + 1, last_y + 1);
	// If at right border dont clear to avoid overflow
	else if (last_x > 254) {
		gfx_clear_area(ground, last_x - 1, last_y, last_x, last_y + 1);
		redraw = 0 ;
	}
	// If at left border dont clear to avoid overflow
	else if(last_x < 2) gfx_clear_area(ground, last_x , last_y, last_x + 1, last_y + 1);

	// Draw enemy
	if(redraw){
		bgcolor(0);
		fgcolor(2); //Green

		gotoxy(x-1, y);
		printf("/%c%c",0xCA,0x5C);
		gotoxy(x-1, y+1);
		printf("000");
		}
}


static void draw_bullet(entity_t * self, uint8_t  * ground, uint8_t redraw) {
	// Clear last bullet
	gotoxy((self->last_x>>14)+1, (self->last_y>>14)+1);
	printf(" ");
	// Draw bullet
	if (redraw) {
		bgcolor(0);
		fgcolor((self->counter)++);//Cycle through every colour
		if((self->counter) == 7) self->counter = 1;
		gotoxy((self->x>>14)+1, (self->y>>14)+1);
		printf("%c", 0xDF);

	}

}

static void draw_bomb(entity_t * self, uint8_t  * ground, uint8_t redraw) {
	gotoxy(self->last_x>>14, self->last_y>>14);
	printf(" ");
	gotoxy(self->x>>14,self->y>>14);
	bgcolor(0);
	fgcolor(7);

	switch(self->rotation){
	case 0b0001: printf("^"); // up
		break;
	case 0b0101: printf("%c", 0xBF);// right up
		break;
	case 0b0100: printf(">");//right
		break;
	case 0b0110: printf("%c", 0xD9);// right down
		break;
	case 0b0010: printf("v");// down
		break;
	case 0b1010: printf("%c", 0xC0);// left down
		break;
	case 0b1000: printf("<"); //left
		break;
	case 0b1001: printf("%c", 0xDA); //left up
		break;
	case 0b0000: printf("-");
		break;
	default:	 printf("ERROR");
		break;
	}

}


static void draw_nuke(entity_t * self, uint8_t  * ground, uint8_t redraw) {
	gotoxy(self->last_x>>14, self->last_y>>14);
	bgcolor(0);
	fgcolor(7);
	printf(" ");
	gotoxy(self->x>>14,self->y>>14);
	printf("%c",0xDB);
}


static void draw_powerup(entity_t * self, uint8_t  * ground, uint8_t redraw) {
	gotoxy(self->last_x>>14, self->last_y>>14);
	bgcolor(0);
	fgcolor(7);
	printf(" ");
	gotoxy(self->x>>14,self->y>>14);
	printf("%c", 0x24);
}

static void update_position(entity_t * self, fixp_t x, fixp_t y) {
	self->last_x = self->x;
	self->last_y = self->y;

	self->x = x;
	self->y = y;
}

static void update_velocity(entity_t * self, fixp_t vel_x, fixp_t vel_y) {
	self->vel_x = vel_x;
	self->vel_y = vel_y;
}

static void update_rotation(entity_t * self, fixp_t rotation) {
	self->last_rotation = self->rotation;


	switch (self->type) {
		case Spaceship:
			if(self->vel_y > 0)
			{
				if(self->vel_x > self->vel_y && self->vel_x > 0){
							rotation = 1;		// right
				}
				else if(self->vel_x < (-self->vel_y) && self->vel_x < 0){
							rotation = 3;		// left
				}

			}
			else{
				if(self->vel_x > (-self->vel_y)    && self->vel_x > 0){
							rotation = 1;		// right
				}
				else if(self->vel_x < self->vel_y && self->vel_x < 0){
							rotation = 3;		// left<
				}

			}
			if(self->vel_x > 0)
						{
				if( self->vel_y > self->vel_x && self->vel_y > 0){
							rotation = 2;		// down
				}
				else if(self->vel_y < (-self->vel_x) && self->vel_y < 0){
							rotation = 0;		// up
				}
			}
			else{
				if(self->vel_y > (-self->vel_x) && self->vel_y > 0){
							rotation = 2;		// down
				}
				else if(self->vel_y < self->vel_x && self->vel_y < 0){
							rotation = 0;		// up
				}
			}

			break;
		case Bomb:
			if(self->vel_x < (1<<11) && self->vel_x > (-1<<11) && self->vel_y < (1<<11) && self->vel_y > (-1<<11)){
				rotation = 0;
				break;
			}

			if (self->vel_y > -(self->vel_x << 1) && self->vel_y < self->vel_x << 1) {
				rotation |= (0b0100);		// Sets horizontal direction to positive
			} else if (self->vel_y < -(self->vel_x << 1) && self->vel_y > self->vel_x << 1) {
				rotation |= (0b1000);		// Sets horizontal direction to negative
			}
			if(self->vel_x > -(self->vel_y << 1) && self->vel_x < self->vel_y << 1) {
				rotation |= (0b0010);		// Sets vertical direction to negative
			} else if (self->vel_x < -(self->vel_y << 1) && self->vel_x > self->vel_y << 1) {
				rotation |= (0b0001);		// Sets vertical direction to positive
			}
			break;
		default:
			printf("ERROR");
	}
	self->rotation = rotation;
}

static uint8_t check_collision(fixp_t x, fixp_t y, uint8_t type, uint8_t* heightmap, entity_t* player) { // [0]=walls, [1]=roof, [2]=ground, [3]=player
	// Bits of return value:
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


	if (type & 1<<1) { // Check collisions with roof
		if (y < 0) { // Roof
			collision |= 1<<2;
		}
	}
	if (type & 1<<2) {  // Check collisions with ground

		// Works if split into variables, not if put together in expression for some reason idk
		uint8_t a = x>>14;
		uint8_t b = heightmap[a];
		uint8_t c = DISPLAY_HEIGHT - 1 - b;
		fixp_t d = c << 14;

		if (y > d) {
			collision |= 1<<3;
		}
	}

	if (type & 1<<3) {
		if ((x > player->x) && (x < player->x + (5<<14)) && (y > player->y) && (y < player->y + (2<<14))) { // Spaceship
			collision |= 1<<4;
		}
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
	entity->rotation = 0;
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
	entity->update_velocity = &update_velocity;
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

void gravity_move (entity_t* self, fixp_t g) {
	self->vel_y += g;
	self->update_position(self, self->x + self->vel_x, self->y+self->vel_y);
}

uint8_t player_move (entity_t* self, uint8_t* heightmap) {
	fixp_t new_x = self->x + self->vel_x;
	fixp_t new_y = self->y + self->vel_y;

	uint8_t collisions_tl = self->check_collision(new_x, new_y, 0b0111, heightmap, NULL); // Top-left
	uint8_t collisions_br = self->check_collision(new_x + (5<<14), new_y + (2<<14), 0b0111, heightmap, NULL); // Bottom-right


	if (collisions_tl) {
		if (collisions_tl & 0b1) { // Collision with left wall
			new_x = (DISPLAY_WIDTH - 6) << 14;
		}
		if (collisions_tl & 0b100) { // Ceiling
			self->vel_y = -self->vel_y;
			new_y = 0;
		} /*else if (collisions & 0b1000) { // Ground

		}*/
	}

	if (collisions_br) {
		if (collisions_br & 0b10) { // Collision with right wall
			new_x = 0;
		}
		if(collisions_br & 0b1000) { // Collision with floor
			new_y = ((DISPLAY_HEIGHT - 1 - heightmap[(new_x >> 14) + 5]) - 2) << 14 ;
		}
	}

	self->update_position(self, new_x, new_y);
	return collisions_tl | collisions_br;
}
