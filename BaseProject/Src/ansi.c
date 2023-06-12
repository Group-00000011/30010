#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ansi.h"
#include <stdlib.h>

void fgcolor(uint8_t foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(uint8_t background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf("%c[%dm", ESC, background+40);
}

void color(uint8_t foreground, uint8_t background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
  printf("%c[m", ESC);
}


void clrscr() {
	printf("%c[2J", ESC);
}

void clreol() {
	printf("%c[K", ESC);
}

void gotoxy(uint8_t x, uint8_t y) {
	printf("%c[%d;%dH", ESC, y, x);
}

void underline(uint8_t on) {
	printf("%c[%dm", ESC, on ? 4 : 0);
}

void blink(uint8_t on) {
	printf("%c[%dm", ESC, on ? 5 : 0);
}

void inverse(uint8_t on) {
	printf("%c[%dm", ESC, on ? 7 : 0);
}

void moveup(uint8_t n) {
	printf("%c[%dA", ESC, n);
}

void movedown(uint8_t n) {
	printf("%c[%dB", ESC, n);
}

void moveright(uint8_t n) {
	printf("%c[%dC", ESC, n);
}

void moveleft(uint8_t n) {
	printf("%c[%dD", ESC, n);
}


void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char title[], uint8_t doublestroke) {
	// Calculate width and height of window from coordinates.
	// Force size to be big enough to draw the smallest possible window
	uint8_t width = abs(x2 - x1) > 8 ? abs(x2 - x1) : 8;
	uint8_t height = abs(y2 - y1) > 1 ? abs(y2 - y1) : 2;

	// Get x and y of top left corner where drawing of window will start
	uint8_t startx = x1 < x2 ? x1 : x2;
	uint8_t starty = y1 < y2 ? y1 : y2;

	// If length of title is too big for the window shorten the title to fit
	uint8_t titlelen = strlen(title);
	if (titlelen > width - 8) {
		width = titlelen + 8;
	}

	// Define frame segments based on style.
	char ver = doublestroke ? 186 : 179;
	char hor = doublestroke ? 205 : 196;
	char tl = doublestroke ? 201 : 218;
	char tr = doublestroke ? 187 : 191;
	char br = doublestroke ? 188 : 217;
	char bl = doublestroke ? 200 : 192;
	char titleleft = doublestroke ? 185 : 180;
	char titleright = doublestroke ? 204 : 195;


	// Draw top of frame
	gotoxy(startx, starty);
	printf("%c%c", tl, titleleft);
	inverse(1);
	printf("  %s  ", title);
	inverse(0);
	printf("%c", titleright);
	// Loop through remaining of top part except for corner
	for (int i = 0; i < width - (titlelen + 8) - 1; i++) {
		printf("%c", hor);
	}
	printf("%c", tr);


	// Draw right of frame
	moveleft(1);
	movedown(1);
	for (int i = 0; i < height - 2; i++) {
		printf("%c", ver);
		moveleft(1);
		movedown(1);
	}
	printf("%c", br);


	// Draw bottom of frame
	moveleft(2);
	for (int i = 0; i < width - 2; i++) {
		printf("%c", hor);
		moveleft(2);
	}
	printf("%c", bl);


	// Draw left of frame
	moveup(1);
	moveleft(1);
	for (int i = 0; i < height - 2; i++) {
		printf("%c", ver);
		moveup(1);
		moveleft(1);
	}
}


void notitlewindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t doublestroke) {
	// Calculate width and height of window from coordinates.
	// Force size to be big enough to draw the smallest possible window
	uint8_t width = abs(x2 - x1) > 8 ? abs(x2 - x1) : 8;
	uint8_t height = abs(y2 - y1) > 1 ? abs(y2 - y1) : 2;

	// Get x and y of top left corner where drawing of window will start
	uint8_t startx = x1 < x2 ? x1 : x2;
	uint8_t starty = y1 < y2 ? y1 : y2;

	// Define frame segments based on style.
	char ver = doublestroke ? 186 : 179;
	char hor = doublestroke ? 205 : 196;
	char tl = doublestroke ? 201 : 218;
	char tr = doublestroke ? 187 : 191;
	char br = doublestroke ? 188 : 217;
	char bl = doublestroke ? 200 : 192;


	// Draw top of frame
	gotoxy(startx, starty);
	printf("%c", tl);
	// Loop through remaining of top part except for corner
	for (int i = 0; i < width - 2; i++) {
		printf("%c", hor);
	}
	printf("%c", tr);


	// Draw right of frame
	moveleft(1);
	movedown(1);
	for (int i = 0; i < height - 2; i++) {
		printf("%c", ver);
		moveleft(1);
		movedown(1);
	}
	printf("%c", br);


	// Draw bottom of frame
	moveleft(2);
	for (int i = 0; i < width - 2; i++) {
		printf("%c", hor);
		moveleft(2);
	}
	printf("%c", bl);


	// Draw left of frame
	moveup(1);
	moveleft(1);
	for (int i = 0; i < height - 2; i++) {
		printf("%c", ver);
		moveup(1);
		moveleft(1);
	}
}


