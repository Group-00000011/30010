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
#include "levels.h"
#include "flash_memory.h"

#define GRAVITY 0x0400

typedef enum State {NullState, MainMenu, HelpMenu, Game, DeathMenu, BossScreen} State;

/*
volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;
*/

volatile uint8_t update_flag = 0; // [0]=update enemies; [1]=update player

void spaceship_input();


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
	State state = MainMenu;
	State last_state = NullState;
	State next_state = state;
	State return_state = MainMenu;
	uint8_t state_transition = 1; // Flag to set true when changing state, the flag can then be set false to run code only when entering state.
	uint8_t menu_selection = 0;
	uint8_t last_menu_sel = 0;

	uint8_t prev_red_btn;
	uint8_t prev_gray_btn;

	fixp_t js[2];

	uint8_t last_keypress;

	int8_t lives = 3;
	uint8_t level = 0;
	uint16_t kills = 0;
	uint16_t score = 0;
	uint8_t nukes_cnt = 0;
	uint8_t powerup_delay = 0;
	//Uncomment below line to reset highscore to 0.
	//flash_write_halfword(1, 0);
	uint16_t high_score = flash_read_halfword(0);
	uint16_t a = ~flash_read_halfword(1);
	if (a) { // If the halfword after high_score is not all 1s
		flash_write_halfword(0, 0);
		high_score = 0;
	}

	uint8_t* planet_heightmap;

	entity_t* player = entity_init(Spaceship, 200<<14, 25<<14, -2<<14, 0);

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	listnode_t* bullets = NULL;
	listnode_t* bombs = NULL;
	entity_t* powerup = NULL;
	//list_push(&enemies, entity_init(Enemy, 220<<14, 10<<14, fixp_fromint(1), 0));
	//list_push(&enemies, entity_init(Enemy, 25<<14, 10<<14, fixp_fromint(-1), 0));
	//list_push(&enemies, entity_init(Enemy, 50<<14, 35<<14, fixp_fromint(1), 0));
	//list_push(&bombs, entity_init(Bomb, 120<<14, 10<<14, fixp_fromint(1), 0));
	//list_push(&enemies, entity_init(Enemy, 17<<14, 0, 1<<14, 0));

	fixp_t bomb_blast_radius = 5<<14;
	fixp_t nuke_blast_radius = 15<<14;




	uint8_t lcd_buffer[512];
	memset(lcd_buffer, 0, 512);

	lcd_text_t lcd_lives;
	lcd_text_t lcd_level;
	lcd_text_t lcd_score;
	lcd_text_t lcd_kills;
	lcd_text_t lcd_nukes;

	lcd_init_text(&lcd_lives, "", 0, 0, 25*5);
	lcd_init_text(&lcd_level, "", 0, 1, 25*5);
	lcd_init_text(&lcd_score, "", 0, 2, 25*5);
	lcd_init_text(&lcd_kills, "", 0, 3, 25*5);
	lcd_init_text(&lcd_nukes, "", 12*5, 0, 13*5);

	bgcolor(SPACE_COLOR);
	clrscr();
	gotoxy(1,1);

  	while (1) {
  		uint8_t red_btn = buttonRed();
  		uint8_t gray_btn = buttonGray();

  		joystick_read(js);

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
  				set_led(3);
  				draw_main_menu(menu_selection);
  				draw_menu_title("Main Menu");

  			}

  			if (js[1] > 0) {
  				if (menu_selection) {
  					last_menu_sel = menu_selection;
  					menu_selection--;
  				}
  			}

  			if (js[1] < 0) {
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
  				set_led(2);
  				planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap
  				lives = 3;
  				level = 0;
  				score = 0;
  				kills = 0;
  			}

  			if (enemies == NULL) {
  				level++;
  				level_setup(&enemies, level, planet_heightmap);
  			}
			
  			if (update_flag & 1) {	// Update enemies and bullets
				listnode_t* current_node = enemies;
				entity_t* current_entity;
				while (current_node != NULL) { // Loop through enemies
					current_entity = current_node->ptr;

					enemy_move(current_entity, planet_heightmap);

					++current_entity->counter;
					if (current_entity->counter == (level < 15 ? 50 - level : 30)) { // If counter is reached fire bullet. max count decreases with higher level.
						current_entity->counter = 0;

						fixp_t toplayer_x = fixp_div(player->x - current_entity->x, fixp_fromint(150)); // Vector from enemy to player
						fixp_t toplayer_y = fixp_div(player->y - current_entity->y, fixp_fromint(150));

						list_push(&bullets, entity_init(Bullet, current_entity->x, current_entity->y, toplayer_x, toplayer_y));
					}

					current_entity->draw(current_entity, planet_heightmap, 1);
					current_node = current_node->next;
				}
				current_node = bullets;
				listnode_t* prev_node = NULL;
				while (current_node != NULL) { // Loop through bullets
					current_entity = current_node->ptr;

					entity_move(current_entity);

					uint8_t collisions = current_entity->check_collision(current_entity->x, current_entity->y, 0b1111, planet_heightmap, player); // Check collision with walls/roof/player

					if (collisions) { // Collision with wall/roof/player
						// Kill the bullet
						current_entity->draw(current_entity, planet_heightmap, 0); // Erase bullet
						current_node = current_node->next;
						if (prev_node) {
							free(list_remove_next(prev_node));
						} else {
							free(list_pop(&bullets));
						}
						if (collisions & 1<<4) { // Collision with player
							lives--;
						}

					} else {
						current_entity->draw(current_entity, planet_heightmap, 1);
						prev_node = current_node;
						current_node = current_node->next;
					}
				}
				
				if (lives < 1) {
					next_state = DeathMenu;

					// Delete all enemies
					while (enemies != NULL) {
						free(list_pop(&enemies));
					}

					// Delete all bullets
					while (bullets != NULL) {
						free(list_pop(&bullets));
					}

					// Delete bombs/nukes
					while (bombs != NULL) {
						free(list_pop(&bombs));
					}
				}


				if (powerup != NULL) {
					uint8_t collisions = powerup->check_collision(powerup->x, powerup->y, 0b1000, NULL, player);

					if (collisions) {
						nukes_cnt++;
						powerup->draw(powerup, NULL, 0);
						free(powerup);
						powerup = NULL;
					}
					if (powerup != NULL) {
						powerup->draw(powerup, NULL, 1);
					}
				} else {
					if (level > 3) {
						powerup_delay++;
						if (powerup_delay == 255) {
							powerup_delay = 0;
							fixp_t powerup_x = enemies ? ((entity_t *) enemies->ptr)->x : 30<<13;
							fixp_t powerup_y = enemies ? (((entity_t *) enemies->ptr)->y) - (20<<14) : 30<<14;
							powerup = entity_init(Powerup, powerup_x, powerup_y, 0 , 0);
						}
					}
				}

				update_flag &= ~1;
			}


  			if (update_flag & (1 << 1)){ // Update player and bombs
  				// Rising edge detection of input buttons
  		  		uint8_t red_btn_rising = red_btn && !prev_red_btn;
  		  		uint8_t gray_btn_rising = gray_btn && !prev_gray_btn;

  				// Update list of bombs
  				/*while (current_node != NULL) {
  					// TODO Check collision somehow
  					current_entity = current_node->ptr;
  					gravity_move(current_entity, GRAVITY);
  					current_node = current_node->next;
  				}
  				current = bombs;
  				while (current != NULL) {
  					entity_t* bomb = current->ptr;
  					bomb->update_rotation(bomb, 0);
  					bomb->draw(bomb, NULL, 1);
  					current = current->next;
  				}*/

  		  		listnode_t* current_node = bombs;
  		  		listnode_t* prev_node = NULL;
  				entity_t* current_entity;
				while (current_node != NULL) { // Loop through bombs
					current_entity = current_node->ptr;

					gravity_move(current_entity, GRAVITY);

					uint8_t collisions = current_entity->check_collision(current_entity->x, current_entity->y, 0b00000111, planet_heightmap, player); // Check collision with walls/roof/ground

					if (collisions & 1<<2) { // Check collision with roof
						current_entity->y = 0;
						current_entity->vel_y = 0;
					} else if (collisions) { // Collision with wall/ground but only if not roof
						// Kill the bomb
						gotoxy(1,5);
						printf("Bomb collision\n");


						if (collisions & 1<<3) { // Collision with ground
							// Find all enemies within radius and kill them
							listnode_t* enemy_node = enemies;
							listnode_t* prev_enemy_node = NULL;
							entity_t* enemy;
							while (enemy_node != NULL) {
								enemy = enemy_node->ptr;

								if (enemy->x > current_entity->x-bomb_blast_radius && enemy->x < current_entity->x+bomb_blast_radius) {
									// Current enemy is within blast radius, it should die
									//gotoxy(fixp_toint(enemy->x), fixp_toint(enemy->y));
									gotoxy(0,5);
									printf("Skrrt");
									kills++;
									if (prev_enemy_node) {
										free(list_remove_next(prev_enemy_node));
									} else {
										free(list_pop(&enemies));
									}
								}

								prev_enemy_node = enemy_node;
								enemy_node = enemy_node->next;
							}
						}
						current_entity->draw(current_entity, planet_heightmap, 0); // Erase bomb
						current_node = current_node->next;
						if (prev_node) {
							printf("remove\n");
							free(list_remove_next(prev_node));
						} else {
							printf("pop\n");
							free(list_pop(&bombs));
						}
					} else {
						current_entity->draw(current_entity, planet_heightmap, 1);
						prev_node = current_node;
						current_node = current_node->next;
					}
				}

  				//gotoxy(1,1);
  				//printf("red: %d\ngray: %d\n#bombs: %d", red_btn, gray_btn, list_length(bombs));

  				if (gray_btn_rising && !state_transition) { // Fire bomb?
  					// Fire bomb!
  					list_push(&bombs, entity_init(Bomb, player->x, player->y, player->vel_x, player->vel_y));
  				}

  				if (red_btn_rising && nukes_cnt && !state_transition) { // Fire nuke and have enough?
  					// Fire nuke!
  					nukes_cnt--;
  					list_push(&bombs, entity_init(Nuke, player->x, player->y, player->vel_x, player->vel_y));
  				}

				// Update position of player
				if (js[0] || js[1]) {
					player->update_velocity(player, js[0], -js[1]);
				}
				uint8_t collisions = player_move(player, planet_heightmap); // Returns collision from check_collision()

				if (collisions & 0b1000) {
					// Player has hit ground, game over. TODO
				}

				score = (level-1)*50 + kills*10;

				// Draw player
				player->draw(player, planet_heightmap, 1);

				// Rising edge detection of input buttons
				prev_red_btn = red_btn;
				prev_gray_btn = gray_btn;

				player->draw(player, planet_heightmap, 1);
				update_flag &= ~(1<<1);
			}

  			break;

		// ------------------------------
		// |  DEATH MENU STATE			|
		// ------------------------------
  		case DeathMenu:
  			if(state_transition) {
  				set_led(1);
  				if (!(last_state == MainMenu || last_state == HelpMenu)) {
					draw_menu_screen();
				}
  				draw_menu_title("You Lost :(");
  				if (score > high_score) {
  					high_score = score;
  					flash_write_halfword(0, high_score);
  				}
  	  			draw_death_menu(level, score, kills, high_score);
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
  				set_led(3);
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

  		// LCD information:
		sprintf(lcd_lives.content, "Lives: %d", lives);
		sprintf(lcd_level.content, "Level: %d", level);
		sprintf(lcd_score.content, "Score: %d", score);
		sprintf(lcd_kills.content, "Kills: %d", kills);
		sprintf(lcd_nukes.content, "Nukes: %d", nukes_cnt);

		lcd_write_line(lcd_buffer, &lcd_lives);
		lcd_write_line(lcd_buffer, &lcd_level);
		lcd_write_line(lcd_buffer, &lcd_score);
		lcd_write_line(lcd_buffer, &lcd_kills);
		uint8_t ndigits = 0;
		uint8_t temp_nukes = nukes_cnt;
		do {
			temp_nukes /= 10;
			ndigits++;
		} while (temp_nukes != 0);

		lcd_nukes.text_width = 7*5 + ndigits*5; // Set width to match digits in nukes cnt
		lcd_write_string(lcd_buffer, &lcd_nukes);


		lcd_push_buffer(lcd_buffer);


  		// State transitions:
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

	update_flag |= 1<<1; // Set player/bomb flag high
	TIM15->SR &= ~0x0001;
}

void TIM1_UP_TIM16_IRQHandler(void) {
	update_flag |= 1; // Set enemy/bullet flag high
	TIM16->SR &= ~0x0001;
}
