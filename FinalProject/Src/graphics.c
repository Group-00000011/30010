/*
 * graphics.c
 *
 *  Created on: 14 Jun 2023
 *      Author: asbjorn
 */

#include "graphics.h"

uint8_t* gfx_draw_background() {
	static uint8_t ground[DISPLAY_WIDTH] = {2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,8,8,9,9,9,8,8,8,8,8,8,8,8,7,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,9,10,10,10,11,11,11,10,10,10,10,10,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,11,11,11,10,10,10,10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,11,11,11,11,11,11,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,8,8,8,8,8,8,7,7,7,7,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2};
						// 2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2
	clrscr();
	gotoxy(1,1);
	bgcolor(SPACE_COLOR);
	fgcolor(0);
	uint8_t is_drawing_space = 1;
	char hline[DISPLAY_WIDTH+2+5*8]; // 256+2+5*8

	for (uint8_t pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {
		uint16_t pixel_x = 0;
		uint16_t char_x = 0;

		while (pixel_x < DISPLAY_WIDTH) {
			if (pixel_y > DISPLAY_HEIGHT-1-ground[pixel_x]) { // Check if below surface
				if (is_drawing_space) { // We are below surface, check if drawing space
					// Set background color to ground
					sprintf(hline+char_x, "%c[%dm", ESC, PLANET_COLOR+40);
					is_drawing_space = 0;
					char_x += 5;

				}
				if (pixel_y == DISPLAY_HEIGHT-ground[pixel_x]) {
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

		if (pixel_y < DISPLAY_HEIGHT-1) {
			hline[char_x] = '\n';
			++char_x;
		}

		hline[char_x] = '\0';

		printf(hline);
	}

	return ground;
	bgcolor(SPACE_COLOR);
}


void gfx_clear_area(uint8_t * ground, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	if (x1 < 0) {
		x1 = 0;
	} else if (x1 > 255) {
		x1 = 255;
	}

	if (x2 < 0) {
		x2 = 0;
	} else if (x2 > 255) {
		x2 = 255;
	}

	if (x2 < x1) {
		x2 = x1;
	}

	gotoxy(x1, y1);
	bgcolor(SPACE_COLOR);
	fgcolor(0);

	uint8_t is_drawing_space = 1;
	char hline[x2 - x1 + 2 + 5*8];

	for (uint8_t pixel_y = y1; pixel_y <= y2; pixel_y++) {
		uint16_t pixel_x = x1;
		uint16_t char_x = 0;

		while (pixel_x <= x2) {
			if (pixel_y > DISPLAY_HEIGHT-ground[pixel_x - 1]) { // Check if below surface
				if (is_drawing_space) { // We are below surface, check if drawing space
					// Set background color to ground
					sprintf(hline+char_x, "%c[%dm", ESC, PLANET_COLOR+40);
					is_drawing_space = 0;
					char_x += 5;

				}
				if (pixel_y == DISPLAY_HEIGHT-ground[pixel_x]) {
					hline[char_x] = '*';
				} else {
					hline[char_x] = ' ';
				}
				++char_x;
				++pixel_x;
			} else { // We are above surface
				if (!is_drawing_space) {  // Check if drawing ground
					//	 Set background color to space
					sprintf(hline+char_x, "%c[%dm", ESC, SPACE_COLOR+40);
					is_drawing_space = 1;
					char_x += 5;
				}
				hline[char_x] = ' ';
				++char_x;
				++pixel_x;
			}
		}
		if (pixel_y < DISPLAY_HEIGHT-1) {
			gotoxy(x1, pixel_y);
		}

		hline[char_x] = '\0';

		printf(hline);
	}
}


void draw_menu_screen() {
	bgcolor(MENU_SHADOW_COLOR);
	clrscr();
	gotoxy(1,1);
	char blankline[DISPLAY_WIDTH + 1];
	memset(blankline, ' ', DISPLAY_WIDTH);
	blankline[DISPLAY_WIDTH] = '\0';

	char sidebar[50];
	memset(sidebar, ' ', 49);
	sidebar[49] = '\0';


	char window[DISPLAY_WIDTH - 99];
	memset(window, 196, DISPLAY_WIDTH - 100);
	window[0] = 218;
	window[DISPLAY_WIDTH - 101] = 191;
	window[DISPLAY_WIDTH - 100] = '\0';

	bgcolor(MENU_BG_COLOR);

	for (int i = 0; i < 10; i++) {
		printf("%s\n", blankline);
	}

	printf("%s ", sidebar);
	moveright(DISPLAY_WIDTH - 100);
	printf(" %s\n", sidebar);


	for (int i = 1 ; i < DISPLAY_HEIGHT - 20; i++) {
		printf("%s ", sidebar);
		moveright(DISPLAY_WIDTH - 99);
		printf("%s\n", sidebar);
	}

	printf(" %s ", sidebar);
	moveright(DISPLAY_WIDTH - 100);
	printf("%s\n", sidebar);


	gotoxy(1, DISPLAY_HEIGHT - 8);

	for (int i = 0; i < 8; i++) {
		printf("%s\n", blankline);
	}
	printf("%s", blankline);

	bgcolor(MENU_WINDOW_COLOR);
	fgcolor(MENU_FG_COLOR);

	gotoxy(51, 11);
	printf("%s", window);
	memset(window, ' ', DISPLAY_WIDTH - 100);
	window[0] = 179;
	window[DISPLAY_WIDTH - 101] = 179;

	for (int i = 1; i < DISPLAY_HEIGHT - 21 ; i++) {
		gotoxy(51, 11 + i);
		printf("%s", window);
	}

	gotoxy(51, DISPLAY_HEIGHT - 10);

	memset(window, 196, DISPLAY_WIDTH - 100);
	window[0] = 192;
	window[DISPLAY_WIDTH - 101] = 217;

	printf("%s", window);

	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 20);
	printf("--- MENU CONTROLS ---");
	gotoxy((DISPLAY_WIDTH >> 1) - 13, DISPLAY_HEIGHT - 18);
	printf("Joystick to change selection");


	fgcolor(GRAY_BTN_FG_COLOR);
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 16);
	printf("BTN");
	fgcolor(0);
	printf(" to select");

	fgcolor(RED_BTN_FG_COLOR);
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 14);
	printf("BTN");
	fgcolor(0);
	printf(" to go back");

}

void draw_menu_title(char title[]) {
	fgcolor(MENU_FG_COLOR);
	bgcolor(MENU_WINDOW_COLOR);

	char window[DISPLAY_WIDTH - 99];
	gotoxy(51, 11);

	memset(window, 196, DISPLAY_WIDTH - 100);
	window[0] = 218;
	window[DISPLAY_WIDTH - 101] = 191;

	printf("%s", window);

	gotoxy(54, 11);
	printf("%c  %s  %c", 180, title, 195);

}


static void clear_menu_content() {
	char window[DISPLAY_WIDTH - 104];
	memset(window, ' ', DISPLAY_WIDTH - 105);
	window[DISPLAY_WIDTH - 105] = '\0';
	bgcolor(MENU_WINDOW_COLOR);

	for (int i = 0; i < DISPLAY_HEIGHT - 32; i++) {
		gotoxy(53, 12 + i);
		printf("%s", window);
	}
}


void draw_main_menu(uint8_t sel) {

	// 0 to show START GAME as selected
	// 1 to show help as selected
	clear_menu_content();
	if (sel == 0) {
		bgcolor(MENU_SHADOW_COLOR);
		fgcolor(MENU_WINDOW_COLOR);
	} else {
		bgcolor(MENU_WINDOW_COLOR);
		fgcolor(MENU_FG_COLOR);
	}

	gotoxy((DISPLAY_WIDTH >> 1) - 8, 25);
	printf(" * START GAME ");

	if (sel == 1) {
		bgcolor(MENU_SHADOW_COLOR);
		fgcolor(MENU_WINDOW_COLOR);
	} else {
		bgcolor(MENU_WINDOW_COLOR);
		fgcolor(MENU_FG_COLOR);
	}

	gotoxy((DISPLAY_WIDTH >> 1) - 8, 29);
	printf(" * HELP ");
}

void draw_help_menu() {
	clear_menu_content();
	bgcolor(MENU_WINDOW_COLOR);
	fgcolor(MENU_FG_COLOR);


	// GAME CONTROLS
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 34);
	printf("--- GAME CONTROLS ---");

	gotoxy((DISPLAY_WIDTH >> 1) - 13, DISPLAY_HEIGHT - 32);
	printf("Joystick to steer spaceship");


	fgcolor(GRAY_BTN_FG_COLOR);
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 30);
	printf("BTN");
	fgcolor(MENU_FG_COLOR);
	printf(" to drop bomb");

	fgcolor(RED_BTN_FG_COLOR);
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 28);
	printf("BTN");
	fgcolor(MENU_FG_COLOR);
	printf(" to drop nuke");
}


void draw_death_menu() {
	clear_menu_content();
	bgcolor(MENU_WINDOW_COLOR);
	fgcolor(MENU_FG_COLOR);

	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 40);
	printf("You  Lost :(");

	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 38);
	printf("You got the following stats");

	gotoxy((DISPLAY_WIDTH >> 1) - 15, DISPLAY_HEIGHT - 36);
	printf("Level: ");

	gotoxy((DISPLAY_WIDTH >> 1) - 15, DISPLAY_HEIGHT - 34);
	printf("Score: ");

	gotoxy((DISPLAY_WIDTH >> 1) - 15, DISPLAY_HEIGHT - 32);
	printf("Enemies Killed: ");


	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 20);
	printf("                     ");

	gotoxy((DISPLAY_WIDTH >> 1) - 13, DISPLAY_HEIGHT - 18);
	printf("                              ");

	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 18);
	printf("--- MENU CONTROLS ---");



	fgcolor(GRAY_BTN_FG_COLOR);
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 16);
	printf("BTN");
	fgcolor(0);
	printf(" to play again");

	fgcolor(RED_BTN_FG_COLOR);
	gotoxy((DISPLAY_WIDTH >> 1) - 8, DISPLAY_HEIGHT - 14);
	printf("BTN");
	fgcolor(0);
	printf(" to go to main menu");


}


void draw_boss_screen() {
	bgcolor(0);
	fgcolor(7);
	clrscr();
	gotoxy(1,1);
	printf("Something productive");
}


