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

volatile uint8_t update_flag = 0; // [0]=update enemies; [1]=update player

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
	enable_timer_15(1);
	init_timer_16();
	enable_timer_16(1);
	joystick_conf();
	button_init();

	// Initialise state machine
	State state = Game;
	State last_state = NullState;
	State next_state = Game;
	uint8_t state_transition = 1; // Flag to set true when changing state, the flag can then be set false to run code only when entering state.
	uint8_t menu_selection = 0;
	uint8_t last_menu_sel = 0;

	uint8_t red_btn;
	uint8_t gray_btn;

	fixp_t js_vert;
	fixp_t js_hori;

	uint8_t* planet_heightmap;

	entity_t* player = entity_init(Spaceship, 100<<14, 20<<14, 0, 0);

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	list_push(&enemies, entity_init(Enemy, 240<<14, 10<<14, fixp_fromint(1), 0));
	list_push(&enemies, entity_init(Enemy, 25<<14, 10<<14, fixp_fromint(-1), 0));
	list_push(&enemies, entity_init(Enemy, 50<<14, 35<<14, fixp_fromint(1), 0));



	bgcolor(SPACE_COLOR);
	clrscr();
	gotoxy(1,1);
	printf("Hello\n");
/*
	draw_menu_screen();
	draw_menu_title("TITLTLTLTLTLEE");
	draw_main_menu(1);
	draw_help_menu();
*/
//	gfx_draw_background();

  	while (1) {

  		red_btn = buttonRed();
  		gray_btn = buttonGray();

  		js_vert = joystick_vert();
  		js_hori = joystick_hori();

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
  				draw_main_menu(menu_selection);
  				draw_menu_title("Main Menu");
  			}

  			if (js_vert > (0x3 << 13)) {
  				if (menu_selection) {
  					last_menu_sel = menu_selection;
  					menu_selection--;
  				}
  			}

  			if (js_vert < (0x1 << 13)) {
				if (!menu_selection) {
					last_menu_sel = menu_selection;
					menu_selection++;
				}
			}

  			if (menu_selection != last_menu_sel) {
  				draw_main_menu(menu_selection);
  			}

  			last_menu_sel = menu_selection;

  			if (gray_btn) {
  				if (menu_selection == 0) {
  					next_state = Game;
  				} else if (menu_selection == 1) {
  					next_state = HelpMenu;
  				}
  			}

  			// Check if user input is select/move up/move down
  			break;

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

  			if (red_btn) {
  				next_state = MainMenu;
  			}

  			break;

		// ------------------------------
		// |  GAME LOOP STATE			|
		// ------------------------------

  		case Game:
  			if (state_transition) {
  				planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap
  			}

  			if (update_flag & (1 << 1)){

  				// Update velocity

  				if(js_hori != fixp_fromint(1) && js_vert != fixp_fromint(1)){

					if(js_hori > fixp_fromint(1)) player->update_velocity(player, fixp_fromint(1), player->vel_y);
					else player->update_velocity(player, fixp_fromint(-1), player->vel_y);

					if(js_vert > fixp_fromint(1)) player->update_velocity(player, player->vel_x, fixp_fromint(1));
					else player->update_velocity(player, player->vel_x, fixp_fromint(-1));


  				}

  				else if(js_hori != fixp_fromint(1) && js_vert == fixp_fromint(1))
  				{
					if(js_hori>fixp_fromint(1)){
						player->update_velocity(player, fixp_fromint(1), fixp_fromint(0));
						player->update_rotation(player, 1);
					}
					else {
						player->update_velocity(player, fixp_fromint(-1), fixp_fromint(0));
						player->update_rotation(player, 3);
					}
  				}
  				else if(js_hori == fixp_fromint(1) && js_vert != fixp_fromint(1)){
  					if(js_vert>fixp_fromint(1)){
  						player->update_velocity(player, fixp_fromint(0), fixp_fromint(1));
  						player->update_rotation(player, 0);
  					}
					else{
						player->update_velocity(player, fixp_fromint(0), fixp_fromint(-1));
						player->update_rotation(player, 2);
					}
  				}

				gotoxy(2,2);
				printf("js_hori: ");
				fixp_print(js_hori);
				gotoxy(2,5);
				printf("veloc_x: ");
				fixp_print(player->vel_x);
				gotoxy(2,3);
				printf("js_vert: ");
				fixp_print(js_vert);
				gotoxy(2,6);
				printf("veloc_y: ");
				fixp_print(player->vel_y);
				gotoxy(30,2);
				fixp_print(player->x);
				gotoxy(30,3);
				fixp_print(player->y);


				// Update position with velocity
				fixp_t new_x = fixp_add(player->x, fixp_div(player->vel_x, fixp_fromint(50)));
				fixp_t new_y = fixp_sub(player->y, fixp_div(player->vel_y, fixp_fromint(50)));

				player->update_position(player, new_x, new_y);


				player->draw(player);
  			}


			if (update_flag & 1) {

  						listnode_t* current = enemies;
  						while (current != NULL) {
  							entity_t* current_entity = current->ptr;

  							if (current_entity->type == Enemy) {
  								enemy_move(current_entity, planet_heightmap);
  							}

  							current_entity->draw(current_entity);
  							current = current->next;
  						}
  						update_flag &= ~1;
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
