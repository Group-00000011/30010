/*
 * levels.c
 *
 *  Created on: 20 Jun 2023
 *      Author: asbjorn
 */
#include "levels.h"

static int8_t sign_invert(int8_t a, int8_t b) { // Integer power
	for (int i = 0; i < b; i ++) {
		a = ~a + 1;
	}
	return a;
}

void level_setup(listnode_t** enemies_head, uint16_t level, uint8_t* ground) {
	// Set timer speed
	set_timer_15_speed(level < 33 ? 64000 - (level - 1)*1000 : 32000);

	level = level > 26 ? 26 : level;

	int16_t x;

	for (int i = 1; i <= level; i++) {
		x = 128 + sign_invert(-1, i) * 19 * i;
		while (x < 1 || x > 254) {
			if (x < 1) {
				x += 69;
			} else if (x > 254) {
				x -= 69;
			}
		}
		list_push(enemies_head, entity_init(Enemy, x << 14, ground[x] << 14, sign_invert(-1, i>>2)<<14, 0));
	}
}


