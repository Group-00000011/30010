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

	uint8_t pos_x;

int main(void)
{


	// Initialise hardware
	uart_init(9600);
	led_init();
	lcd_init();
	init_timer_2();
	init_timer_15();
	//joystick_conf();

	// Create spaceship
	pos_x=10;
	initialise_entity(&spaceship, Spaceship, pos_x, 10,0);


	clrscr();
	gotoxy(1,1);
	printf("Hello\n");
//
//	printf("%c>\n\n",0xDC);
//	printf("<%c\n\n",0xDC);
//	printf("%c\nV\n\n",0xDC);
//	printf("%c\nv\n\n",0xDC);
//	printf("^\n%c\n\n",0xDC);
//	printf("A\n%c\n\n",0xDC);
	gotoxy(2,2);
	printf("%c%c%c%c%c%c ", 0x5C,0xDB,0xDF,0xDF,0xDB,0x5C);
	gotoxy(2,3);
	printf("/%c%c%c%c/", 0xDB,0xDC,0xDC,0xDB);

	draw_menu_screen();
	//draw_menu_title("Main Menu");
	//draw_main_menu_opts(1);

	draw_menu_title("Help Menu");
	draw_help_menu();



	//buzzer_set_pwm(128);

	//init_timer_2();
	//init_timer_15();
	//enable_timer_2(1);
	//enable_timer_15(1);
  	while (1) {


    }

//  		spaceship.draw(&spaceship);
//  		spaceship.update_position(&spaceship, pos_x, 10);
  		//TIM2->CCR3 = 255;
		/*buzzer_set_pwm(0);
		buzzer_set_pwm(255);*/

      /*		printf("vert: %d    \n",joystick_vert());
		printf("hori: %d    ",joystick_hori());
*/

}

uint8_t c = 0;

void TIM1_BRK_TIM15_IRQHandler(void) {
	/*
	TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk_begin;*/
	TIM15->SR &= ~0x0001;
}
