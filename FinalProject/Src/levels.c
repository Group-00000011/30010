/*
 * levels.c
 *
 *  Created on: 20 Jun 2023
 *      Author: asbjorn
 */
#include "levels.h"

void level_setup(listnode_t** enemies_head, uint16_t level, uint8_t* ground) {
	// Set timer speed
	set_timer_16_speed(level < 25 ? 64000 - (level - 1)*2000 : 16000);

	int16_t x;

	for (int i = 1; i <= level; i++) {
		x = 128 + ((i&1) ? -1 : 1) * 19 * i;
		while (x < 1 || x > 254) {
			if (x < 1) {
				x += 69;
			} else if (x > 254) {
				x -= 69;
			}
		}
		list_push(enemies_head, entity_init(Enemy, fixp_fromint(x), fixp_fromint(ground[x]), fixp_fromint(2*((i&1) ? -1 : 1)), 0));
	}
}


