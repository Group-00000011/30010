#include <stdlib.h>
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


/*
volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;
*/

uint8_t update_flag = 0; // [0]=update enemies; [1]=update player
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
	init_timer_16();
	enable_timer_16(1);
	joystick_conf();

	// Create spaceship
	spaceship = *entity_init(Spaceship, fixp_fromint(9), fixp_fromint(10), 0);


	bgcolor(SPACE_COLOR);
	clrscr();
	printf("Hello\n");
/*
	draw_menu_screen();
	draw_menu_title("TITLTLTLTLTLEE");
	draw_main_menu(1);
	draw_help_menu();
*/
//	gfx_draw_background();

  	while (1) {

  		spaceship.draw(&spaceship);

  		spaceship_input();
  	}


}

void spaceship_input(){
	x = joystick_hori();
	y = joystick_vert();

	gotoxy(1,1);
	fixp_print(x);
	gotoxy(1,2);
	fixp_print(y);
/*
	if(x > 0) rot = 1;
	if(x < 0) rot = 3;
	if(y > 0) rot = 2;
	if(y < 0) rot = 0;


	if(x != 0 && y != 0){
		x = fixp_add((&spaceship)->x, x);
		y = fixp_sub((&spaceship)->y, y);
		spaceship.update_position(&spaceship, x, y);
		spaceship.update_rotation(&spaceship, rot);
	}*/
}

void TIM1_BRK_TIM15_IRQHandler(void) {
	/*TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk_begin;*/

	update_flag |= 1<<1;
	TIM15->SR &= ~0x0001;
}

void TIM1_UP_TIM16_IRQHandler(void) {
	update_flag |= 1;
	TIM16->SR &= ~0x0001;
}
