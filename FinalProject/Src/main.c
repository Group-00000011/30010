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



typedef enum State {NullState, MainMenu, HelpMenu, Game, DeathMenu, BossScreen} State;


/*
volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;
*/

uint8_t update_flag = 0; // [0]=update enemies; [1]=update player

void spaceship_input();
uint8_t rot;


int main(void)
{


	// Initialise hardware
	uart_init(500000);
	led_init();
	lcd_init();

	init_timer_2();
	init_timer_15();
	init_timer_16();
	enable_timer_16(1);
	joystick_conf();
	button_init();

	// Initialise state machine
	State state = MainMenu;
	State last_state = NullState;
	State next_state = NullState;
	uint8_t state_transition = 1; // Flag to set true when changing state, the flag can then be set false to run code only when entering state.
	uint8_t menu_selection = 0;

	// Create spaceship
	entity_t * spaceship;
	spaceship = entity_init(Spaceship, fixp_fromint(9), fixp_fromint(10), 0);

	uint8_t* planet_heightmap;

	listnode_t* enemies = NULL; // Initialise empty list of enemies



	bgcolor(SPACE_COLOR);
	clrscr();
	gotoxy(1,1);
	printf("Hello\n");

  	while (1) {

  		// Handle user input from joystick/buttons
  		if (state != last_state) {
  			state_transition = 1;
  		} else {
  			state_transition = 0;
  		}

  		switch (state) {

  		// ------------------------------
  		// |  MAIN MENU STATE			|
  		// ------------------------------

  		case MainMenu:
  			if (state_transition) {
  				if (!(last_state == HelpMenu || last_state == DeathMenu)) {
  					draw_menu_screen();
  				}
  				draw_menu_title("Main Menu");
  				draw_main_menu(menu_selection);
  			}

  			// Check if user input is select/move up/move down
  			next_state = HelpMenu;

		// ------------------------------
		// |  HELP MENU STATE			|
		// ------------------------------

  		case HelpMenu:
  			if (state_transition) {
  				if (!(last_state == MainMenu || last_state == DeathMenu)) {
					draw_menu_screen();
				}
  				draw_menu_title("Help");
  				draw_help_menu();
  			}

  			break;

		// ------------------------------
		// |  GAME LOOP STATE			|
		// ------------------------------

  		case Game:
  			if (state_transition) {
  				planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap
  				list_push(&enemies, entity_init(Enemy, 7<<14, 10<<14, fixp_fromint(-1)));
  			}
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
  			if (state_transition) {
				if (last_state != MainMenu || last_state != HelpMenu) {
					draw_menu_screen();
				}
  			}

  			break;

  		// DEFAULT TO MAIN MENU
  		default:
  			state = MainMenu;
  			break;

  		}


  		last_state = state;
  		state = next_state;
  	}
}
/*


>>>>>>> origin/main
void spaceship_input(){
	fixp_t x = joystick_hori();
	fixp_t y = joystick_vert();

	int16_t red = buttonRed();
	int16_t gray = buttonGray();

	gotoxy(2,4);
	printf("%d",red);
	gotoxy(2,5);
	printf("%d",gray);

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
