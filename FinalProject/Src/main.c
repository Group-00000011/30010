#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "ansi.h"
#include "hal.h"
#include "string.h"
#include "lcd.h"


int main(void)
{
	// Initialise hardware
	uart_init(9600);
	led_init();
	lcd_init();


	while(1){}
}
