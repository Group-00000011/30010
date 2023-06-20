/*
 * lcd.c
 *
 *  Created on: 12 Jun 2023
 *      Author: asbjorn
 */
#include "lcd.h"


void lcd_write_line(uint8_t buffer[], lcd_text_t * text) {
	uint16_t lineoffset;
	uint8_t charoffset = text->column;
	uint8_t current_char = 0;
	uint8_t len = strlen(text->content);
	if (text->row > 3) {
		lineoffset = 3<<7;
	} else {
		lineoffset = text->row<<7;
	}

	for (int i = 0; i < charoffset; i++) {
		current_char = i*5;
		for (int j = 0; j < 5; j++) {
			*(buffer + lineoffset + current_char + j) = 0x00;
		}
	}

	for (int k = 0; k < 25; k++) {
		current_char = k*5 + charoffset;
		for (int l = 0; l < 5; l++) {
			*(buffer + lineoffset + current_char + l) = k < len ? character_data[text->content[k] - 0x20][l] : 0x00;
		}
	}

	for (int m = 0; m < 3; m++) {
		*(buffer + lineoffset + 125 + m) = 0x00;
	}


}

void lcd_write_string(uint8_t* fb, lcd_text_t* t) {
	uint8_t current_column = t->column;
	uint8_t char_offset = t->scroll_offset/5;
	uint8_t count_to_five = t->scroll_offset % 5;

	fb += t->row<<7;

	for (uint8_t i = 0; i <t->text_width; i++) {
		*(fb+current_column) = character_data[(*(t->content+char_offset))-0x20][count_to_five];

		count_to_five++;
		if (count_to_five == 5) {
			count_to_five = 0;
			char_offset++;
			if (!*(t->content+char_offset)) {
				char_offset = 0;
			}
		}
		current_column++;
	}
}


void lcd_init_text(lcd_text_t* text, char* content, uint8_t column, uint8_t row, uint16_t text_width) {
    text->scroll_offset = 0;
    text->content = malloc(26);
    text->column = column;
    text->row = row;
    text->text_width = text_width;
}

void lcd_scroll_text(lcd_text_t* text) {
    text->scroll_offset++;
    if (text->scroll_offset == strlen(text->content)*5)
        text->scroll_offset = 0;

}
