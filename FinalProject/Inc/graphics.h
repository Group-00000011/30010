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
#define DISPLAY_WIDTH 256
#define DISPLAY_HEIGHT 64


uint8_t* gfx_draw_background();
void gfx_clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void draw_menu_screen();
void draw_menu_title(char title[]);
void draw_main_menu(uint8_t sel);
void draw_help_menu();

void gfx_clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

extern uint8_t ground[DISPLAY_WIDTH];

#endif /* GRAPHICS_H_ */
