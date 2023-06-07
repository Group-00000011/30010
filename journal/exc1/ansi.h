#ifndef _ANSI_H_
#define _ANSI_H_
#define ESC 0x1B
#include <stdint.h>
#include <stdio.h>

void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgColor();
void clrscr();
void clreol();
void gotoxy(uint8_t x, uint8_t y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void moveup(uint8_t n);
void movedown(uint8_t n);
void moveright(uint8_t n);
void moveleft(uint8_t n);
void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char title[], uint8_t doublestroke);



#endif
