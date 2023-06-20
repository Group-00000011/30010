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

uint16_t total_bullets = 0;
uint16_t bullets_removed = 0;
uint16_t bullets_popped = 0;


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
	State next_state = state;
	State return_state = MainMenu;
	uint8_t state_transition = 1; // Flag to set true when changing state, the flag can then be set false to run code only when entering state.
	uint8_t menu_selection = 0;
	uint8_t last_menu_sel = 0;

	uint8_t red_btn;
	uint8_t gray_btn;

	fixp_t js_vert;
	fixp_t js_hori;

	uint8_t last_keypress;

	uint8_t lives = 0;
	uint8_t level = 0;
	uint16_t kills = 0;
	uint16_t score = 0;

	uint8_t* planet_heightmap;

	entity_t* player = entity_init(Spaceship, 128<<14, 30<<14, 0, 0);

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	list_push(&enemies, entity_init(Enemy, 240<<14, 10<<14, fixp_fromint(1), 0));
	list_push(&enemies, entity_init(Enemy, 25<<14, 10<<14, fixp_fromint(-1), 0));
	list_push(&enemies, entity_init(Enemy, 50<<14, 35<<14, fixp_fromint(1), 0));

	listnode_t* bullets = NULL;


	uint8_t lcd_buffer[512];
	memset(lcd_buffer, 0, 512);

	lcd_text_t lcd_lives;
	lcd_text_t lcd_level;
	lcd_text_t lcd_score;
	lcd_text_t lcd_kills;

	lcd_init_text(&lcd_lives, "", 0, 0, 25);
	lcd_init_text(&lcd_level, "", 0, 1, 25);
	lcd_init_text(&lcd_score, "", 0, 2, 25);
	lcd_init_text(&lcd_kills, "", 0, 3, 25);



	bgcolor(SPACE_COLOR);
	clrscr();
	gotoxy(1,1);
	printf("Hello\n");

  	while (1) {

  		red_btn = buttonRed();
  		gray_btn = buttonGray();

  		js_vert = joystick_vert();
  		js_hori = joystick_hori();

  		if (uart_get_count()) {
  			last_keypress = uart_get_char();
  		} else {
  			last_keypress = 0;
  		}

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
			bgcolor(0);
  			fgcolor(7);
  			if (update_flag & 1) {	// Update enemies and bullets
				listnode_t* current_node = enemies;
				while (current_node != NULL) { // Loop through enemies
					entity_t* current = current_node->ptr;

					enemy_move(current, planet_heightmap);

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


			
  			if (update_flag & 1) {	// Update enemies and bullets
				listnode_t* current_node = enemies;
				while (current_node != NULL) { // Loop through enemies
					entity_t* current = current_node->ptr;

					entity_move(current);

						++current->counter;
					if (current->counter == 25) { // If counter is ten, fire bullet
						current->counter = 0;

						fixp_t toplayer_x = fixp_div(player->x - current->x, fixp_fromint(300)); // Vector from enemy to player
						fixp_t toplayer_y = fixp_div(player->y - current->y, fixp_fromint(300));

//						fixp_t distance = fixp_sqrt(fixp_mult(toplayer_x, toplayer_x) + fixp_mult(toplayer_x, toplayer_y)); // Distance from enemy to player
//						toplayer_x = fixp_div(toplayer_x, distance); // Normalize vector
//						toplayer_y = fixp_div(toplayer_y, distance); // Normalize vector

						list_push(&bullets, entity_init(Bullet, current->x, current->y, toplayer_x, toplayer_y));
						++total_bullets;
					}

					current->draw(current, planet_heightmap, 1);
					current_node = current_node->next;
				}
				current_node = bullets;
				listnode_t* prev_node = NULL;
				while (current_node != NULL) { // Loop through bullets
					entity_t* current = current_node->ptr;

					entity_move(current);

					uint8_t collisions = current->check_collision(current->x, current->y, 0b00001011, NULL, player); // Check collision with walls/roof/player

					if (collisions) { // Collision with wall/roof/player
						// Kill the bullet
						current->draw(current, planet_heightmap, 0); // Erase bullet
						if (prev_node) {
							free(list_remove_next(prev_node));
							++bullets_removed;
						} else {
							free(list_pop(&bullets));
							++bullets_popped;
						}
						if (collisions & 1<<4) { // Collision with player
							// Also kill the player
						}
						current_node = prev_node->next;

					} else {
						current->draw(current, planet_heightmap, 1);
						prev_node = current_node;
						current_node = current_node->next;
					}
				}
				gotoxy(1,1);
				printf("atm: %d\ntotal: %d\npopped: %d\nremoved: %d\nremain: %d\n", list_length(bullets), total_bullets, bullets_popped, bullets_removed, total_bullets-(bullets_popped+bullets_removed));
				//player->draw(player, planet_heightmap, 1);
				update_flag &= ~1;
			}
  			if (update_flag & 1<<1) { // Update player

  			}
  			break;

		// ------------------------------
		// |  DEATH MENU STATE			|
		// ------------------------------
  		case DeathMenu:
  			if(state_transition) {
  				if (!(last_state == MainMenu || last_state == HelpMenu)) {
					draw_menu_screen();
				}
  				draw_menu_title("You Lost :(");
  	  			draw_death_menu();
  			}

  			if (gray_btn) {
  				next_state = Game;
  			} else if (red_btn) {
  				next_state = MainMenu;
  			}


  			break;

		// ------------------------------
		// |  DEATH MENU STATE			|
		// ------------------------------

  		case BossScreen:
  			if (state_transition) {
				draw_boss_screen();
				enable_timer_2 (0);
				enable_timer_15 (0);
				enable_timer_16 (0);
  			}

  			if (last_keypress == 'b') {
  				enable_timer_2 (1);
				enable_timer_15 (1);
				enable_timer_16 (1);
  				next_state = return_state;
  			}

  			break;

  		// DEFAULT TO MAIN MENU
  		default:
  			state = MainMenu;
  			break;

  		}

  		sprintf(lcd_lives.content, "Lives: %d", lives);
  		sprintf(lcd_level.content, "Level: %d", level);
  		sprintf(lcd_score.content, "Score: %d", score);
  		sprintf(lcd_kills.content, "Kills: %d", kills);


  		lcd_write_line(lcd_buffer, &lcd_lives);
  		lcd_write_line(lcd_buffer, &lcd_level);
  		lcd_write_line(lcd_buffer, &lcd_score);
  		lcd_write_line(lcd_buffer, &lcd_kills);


  		lcd_push_buffer(lcd_buffer);



  		if (last_keypress == 'b' && state != BossScreen) {
			next_state = BossScreen;
			return_state = state;
			uart_clear();
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
