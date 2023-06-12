/*
 * lcd.h
 *
 *  Created on: 12 Jun 2023
 *      Author: asbjorn
 */

#ifndef LCD_H_
#define LCD_H_

#include "charset.h"
#include "string.h"
#include <stdint.h>

typedef struct {
    char* content;
    uint8_t column, row;
    uint16_t text_width, scroll_offset;
} lcd_text_t;

void lcd_write_line(uint8_t buffer[], lcd_text_t * text);
void lcd_write_string(uint8_t buffer[], lcd_text_t * text);
void lcd_init_text(lcd_text_t* text, char* content, uint8_t column, uint8_t row, uint16_t text_width);
void lcd_scroll_text (lcd_text_t* text);



#endif /* LCD_H_ */
