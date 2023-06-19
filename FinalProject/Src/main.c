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



typedef enum State {MainMenu, HelpMenu, Game, DeathMenu, BossScreen} State;


/*
volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;
*/

uint8_t update_flag = 0; // [0]=update enemies; [1]=update player

void spaceship_input();
uint8_t rot;

fixp_t x;
fixp_t y;

int main(void)
{
	// Initialise state machine
	State state = MainMenu;
	uint8_t state_transition = 1; // Flag to set true when changing state, the flag can then be set false to run code only when entering state.
	uint8_t menu_selection = 0;

	// Initialise hardware
	uart_init(500000);
	led_init();
	lcd_init();
	init_timer_2();
	init_timer_15();
	init_timer_16();
	enable_timer_16(1);
	joystick_conf();

	// Create spaceship
	entity_t * spaceship;
	spaceship = entity_init(Spaceship, fixp_fromint(9), fixp_fromint(10), 0);


	bgcolor(SPACE_COLOR);
	clrscr();


//	gfx_draw_background();

  	while (1) {

  		// Handle user input from joystick/buttons

  		switch (state) {


  		// ------------------------------
  		// |  MAIN MENU STATE			|
  		// ------------------------------

  		case MainMenu:
  			if (state_transition) {
  				draw_menu_screen();
  				draw_menu_title("Main Menu");
  				draw_main_menu(menu_selection);
  				state_transition = 0;
  			}

  			// Chek if user input is select/move up/move down


  			break;

		// ------------------------------
		// |  HELP MENU STATE			|
		// ------------------------------

  		case HelpMenu:

  			break;

		// ------------------------------
		// |  GAME LOOP STATE			|
		// ------------------------------

  		case Game:

  			break;

		// ------------------------------
		// |  DEATH MENU STATE			|
		// ------------------------------
  		case DeathMenu:

  			break;

		// ------------------------------
		// |  DEATH MENU STATE			|
		// ------------------------------

  		case BossScreen:

  			break;

  		// DEFAULT TO MAIN MENU
  		default:
  			state = MainMenu;
  			break;

  		}
  	}
}
/*
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


	x &= 0xFFFFF000;

	y &= 0xFFFFF000;


	if(x != 0 && y != 0){
		x = fixp_add((&spaceship)->x, x);
		y = fixp_sub((&spaceship)->y, y);
		spaceship.update_position(&spaceship, x, y);
	}
}*/


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
