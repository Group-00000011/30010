/*
 * graphics.c
 *
 *  Created on: 14 Jun 2023
 *      Author: asbjorn
 */

#include "graphics.h"

uint8_t ground[256] = {2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,8,8,9,9,9,8,8,8,8,8,8,8,8,7,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,9,10,10,10,11,11,11,10,10,10,10,10,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,11,11,11,10,10,10,10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,11,11,11,11,11,11,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,8,8,8,8,8,8,7,7,7,7,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2};
					// 2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2

void gfx_draw_background() {
	clrscr();
	gotoxy(1,1);
	bgcolor(SPACE_COLOR);
	fgcolor(0);
	uint8_t is_drawing_space = 1;
	char hline[298]; // 256+2+5*8

	for (uint8_t pixel_y = 0; pixel_y < 64; ++pixel_y) {
		uint16_t pixel_x = 0;
		uint16_t char_x = 0;

		while (pixel_x < 256) {
			if (pixel_y > 63-ground[pixel_x]) { // Check if below surface
				if (is_drawing_space) { // We are below surface, check if drawing space
					// Set background color to ground
					sprintf(hline+char_x, "%c[%dm", ESC, PLANET_COLOR+40);
					is_drawing_space = 0;
					char_x += 5;

				}
				if (pixel_y == 64-ground[pixel_x]) {
					hline[char_x] = '*';
				} else {
					hline[char_x] = ' ';
				}
				++char_x;
				++pixel_x;
			} else { // We are above surface
				if (!is_drawing_space) {  // Check if drawing ground
//				 Set background color to space
					sprintf(hline+char_x, "%c[%dm", ESC, SPACE_COLOR+40);
					is_drawing_space = 1;
					char_x += 5;
				}
				hline[char_x] = ' ';
				++char_x;
				++pixel_x;
			}

//			hline[char_x] = ' ';
//			++char_x;
//			++pixel_x;

		}

		if (pixel_y < 63) {
			hline[char_x] = '\n';
			++char_x;
		}

		hline[char_x] = '\0';

		printf(hline);
	}

	bgcolor(SPACE_COLOR);
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



