/*
 * graphics.c
 *
 *  Created on: 14 Jun 2023
 *      Author: asbjorn
 */

#include "graphics.h"



void gfx_draw_background() {
	clrscr();
	gotoxy(1,1);
	bgcolor(0);
	char blankline[258];
	memset(blankline, ' ', 258);
	blankline[256] = '\n';
	blankline[257] = '\0';

	for (int i = 0; i < 50; i++) {
		printf("%s", blankline);
	}
	bgcolor(7);

	for (int i = 0; i < 14; i++) {
		printf("%s", blankline);
	}
	bgcolor(0);
}


void gfx_clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	char blankline[x2 - x1 + 1];
	memset(blankline, ' ', x2 - x1);
	blankline[x2 - x1] = '\0';
	bgcolor(0);

	for (int i = 0; i < y2 - y1; i++) {
		gotoxy(x1, y1 + i);
		if (y1 + i > 50) {
			bgcolor(7);
		}
		printf("%s", blankline);
	}
	bgcolor(0);
}



