/*
 * lcd.c
 *
 *  Created on: 12 Jun 2023
 *      Author: asbjorn
 */
#include "lcd.h"


void lcd_write_line(uint8_t buffer[], uint8_t line, char text[]) {
	uint16_t lineoffset;
	uint8_t charoffset;
	uint8_t len = strlen(text);
	if (line > 3) {
		lineoffset = 3*128;
	} else {
		lineoffset = line*128;
	}


	for (int i = 0; i < 25; i++) {
		charoffset = i*5;
		for (int j = 0; j < 5; j++) {
			*(buffer + lineoffset + charoffset + j) = i < len ? character_data[text[i] - 0x20][j] : 0x00;
		}
	}

	for (int k = 0; k < 3; k++) {
		*(buffer + lineoffset + 125 + k) = 0x00;
	}


}

void lcd_write_string(uint8_t buffer[], uint8_t line, uint8_t c, char text[]) {
	uint16_t lineoffset;
	uint8_t charoffset;
	uint8_t len = strlen(text);

	if (c > 25) {
		charoffset = 0;
		line++;
	} else {
		charoffset = c * 5;
	}
	if (line > 3) {
		lineoffset = 3*128;
	} else {
		lineoffset = line*128;
	}

	for (int i = 0; i < len; i++) {
		if (charoffset > 124) {
			charoffset = 0;
			lineoffset += 128;
			if (lineoffset > 511) break;
		}

		for (int j = 0; j < 5; j++) {
			*(buffer + lineoffset + charoffset + j) = character_data[text[i]  - 0x20][j];
		}

		charoffset += 5;
	}
}

