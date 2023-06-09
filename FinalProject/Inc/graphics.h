/*
 * graphics.h
 *
 *  Created on: 14 Jun 2023
 *      Author: asbjorn
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>
#include "ansi.h"
#include <string.h>


#define SPACE_COLOR 0
#define PLANET_COLOR 7
#define MENU_BG_COLOR 6
#define MENU_WINDOW_COLOR 7
#define MENU_SHADOW_COLOR 0
#define MENU_FG_COLOR 0
#define RED_BTN_FG_COLOR 1
#define GRAY_BTN_FG_COLOR 8
#define DISPLAY_WIDTH 256
#define DISPLAY_HEIGHT 64


uint8_t* gfx_draw_background();
void gfx_clear_area(uint8_t * ground, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void draw_menu_screen();
void draw_menu_title(char title[]);
void draw_main_menu(uint8_t sel);
void draw_help_menu();
void draw_boss_screen();
void draw_death_menu(uint16_t level, uint16_t score, uint16_t kills, uint16_t high_score);



extern uint8_t ground[DISPLAY_WIDTH];

#endif /* GRAPHICS_H_ */
