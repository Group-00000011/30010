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
void gfx_draw_background();
void gfx_clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);


#endif /* GRAPHICS_H_ */
