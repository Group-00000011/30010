#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"


int main(void)
{
	uart_init( 9600 ); // Initialize USB serial emulation at 9600 baud

	// --------------------------------------------------------------
	clrscr();
	window(0, 0, 20, 10, "Title 1", 0);
	window(5, 30, 20, 40, "Title 2", 1);



}

