#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "ansi.h"
#include "hal.h"
#include "string.h"
#include "lcd.h"
#include "daftpunk8bit.h"
#include "data_structures.h"


volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;




int main(void)
{
	// Initialise hardware
	uart_init(9600);
	led_init();
	lcd_init();
	clrscr();
	gotoxy(1,1);
	printf("Hello\n");

	//init_timer_2();
	//init_timer_15();
	//enable_timer_2(1);
	//enable_timer_15(1);

	listnode_t* enemies = NULL;
	printf("%2d\n", list_length(enemies));
	list_push(&enemies, (int32_t*) 234567);
	printf("%2d\n", list_length(enemies));
	list_push(&enemies, (int32_t*) 234567);
	printf("%2d\n", list_length(enemies));
	list_push(&enemies, (int32_t*) 234567);
	printf("%2d\n", list_length(enemies));
	list_push(&enemies, (int32_t*) 234567);
	printf("%2d\n", list_length(enemies));
	list_remove(&enemies, 2);
	printf("%2d\n", list_length(enemies));
	list_remove(&enemies, 0);
	printf("%2d\n", list_length(enemies));


	while(1){}
}

uint8_t c = 0;

void TIM1_BRK_TIM15_IRQHandler(void) {
	TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk_begin;
	TIM15->SR &= ~0x0001;
}
