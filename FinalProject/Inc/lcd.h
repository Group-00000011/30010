/*
 * lcd.h
 *
 *  Created on: 12 Jun 2023
 *      Author: asbjorn
 */

#ifndef LCD_H_
#define LCD_H_
#include "30010_io.h"
#include "charset.h"
#include "string.h"

void lcd_write_line(uint8_t * buffer, uint8_t line, char text[]);
void lcd_write_string(uint8_t buffer[], uint8_t line, uint8_t c, char text[]) ;


#endif /* LCD_H_ */
