#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "ansi.h"
#include "hal.h"
#include "string.h"
#include "lcd.h"
#include "entities.h"
#include "graphics.h"
#include "daftpunk8bit.h"
#include "data_structures.h"

//volatile uint8_t* punk_address = punk_long;
//uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
//uint8_t* punk_begin = punk_long + 18500;

	entity_t spaceship;
	void spaceship_input();
	uint8_t rot;

	fixp_t x;
	fixp_t y;

int main(void)
{
	// Initialise hardware
	uart_init(9600);
	led_init();
	lcd_init();
	init_timer_2();
	init_timer_15();
	joystick_conf();

	// Create spaceship

	initialise_entity(&spaceship, Spaceship, fixp_fromint(9), fixp_fromint(10), 0);

	clrscr();
	printf("Hello\n");
//


//	gfx_draw_background();

  	while (1) {

  		spaceship.draw(&spaceship);

  		spaceship_input();
  	}


}

void spaceship_input(){


	x = fixp_sub(joystick_hori(), fixp_fromint(1252));
	y = fixp_sub(joystick_vert(), fixp_fromint(1300));


	if(x > 0)
		x = fixp_div(x, fixp_fromint(2750));
	else
		x = fixp_div(x, fixp_fromint(1250));

	if(y > 0 )
		y = fixp_div(y, fixp_fromint(2590));
	else
		y = fixp_div(y, fixp_fromint(1280));


	if ((x & 0x80000000) != 0)
		x |= 0x00000FFF;
	else
		x &= 0xFFFFF000;

	if ((y & 0x80000000) != 0)
		y |= 0x00000FFF;
	else
		y &= 0xFFFFF000;


	gotoxy(10,1);
	fixp_print((&spaceship)->x);
	gotoxy(10,2);
	fixp_print((&spaceship)->y);

	if(x != 0 && y != 0){
		x = fixp_add((&spaceship)->x, x);
		y = fixp_sub((&spaceship)->y, y);
		spaceship.update_position(&spaceship, x, y);
	}
}


void TIM1_BRK_TIM15_IRQHandler(void) {
	/*
	TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk_begin;*/
	TIM15->SR &= ~0x0001;
}
