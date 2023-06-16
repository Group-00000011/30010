/*
 * graphics.c
 *
 *  Created on: 14 Jun 2023
 *      Author: asbjorn
 */

#include "graphics.h"

uint8_t ground[256] = {1,2,3,4,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void gfx_draw_background() {
	clrscr();
	gotoxy(1,1);
	bgcolor(SPACE_COLOR);
	uint8_t is_drawing_space = 1;
	char hline[298]; // 256+2+5*8

	for (uint8_t pixel_y = 50; pixel_y < 64; ++pixel_y) {
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
			} else if (!is_drawing_space) { // We are above surface, check if drawing ground
				// Set background color to space
				sprintf(hline+char_x, "%c[%dm", ESC, SPACE_COLOR+40);
				is_drawing_space = 1;
				char_x += 5;
			}

			/*if (pixel_x == 3) {
				sprintf(hline+char_x, "%c[%dm", ESC, 43);//SPACE_COLOR+40);
				char_x += 5;
			}

			if (pixel_x == 5) {
				sprintf(hline+char_x, "%c[%dm", ESC, 45);//SPACE_COLOR+40);
				char_x += 5;
			}*/

			hline[char_x] = ' ';
			++char_x;
			++pixel_x;
		}
		hline[char_x] = '\n';
		hline[char_x+1] = '\0';
		printf(hline);
	}


	/*blankline[256] = '\n';
	blankline[257] = '\0';

	for (int i = 0; i < 50; i++) {
		printf("%s", blankline);
	}
	bgcolor(PLANET_COLOR);

	for (int i = 0; i < 14; i++) {
		printf("%s", blankline);
	}*/
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



