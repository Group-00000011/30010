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
#define PLAYER_DEFAULT_X fixp_fromint(200)
#define PLAYER_DEFAULT_Y fixp_fromint(25)
#define PLAYER_DEFAULT_VEL_X fixp_fromint(-4)

typedef enum {NullState, MainMenu, HelpMenu, Game, DeathMenu, BossScreen} State;

volatile const uint8_t* punk_address = punk;
const uint8_t* punk_end = punk + sizeof punk / sizeof *punk;

volatile uint8_t update_flag = 0; // [0]=update enemies/bullets; [1]=update player/bombs

void spaceship_input();


int main(void)
{
	// Initialise hardware
	uart_init(500000);
	led_init();
	lcd_init();
	init_timer_2();
	init_timer_15();
	init_timer_16();
	init_timer_17();
	enable_timer_15(1);
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
	// Uncomment below line to reset highscore in flash memory
	//flash_write_halfword(1, 0);
	uint16_t high_score = flash_read_halfword(0);
	uint16_t a = ~flash_read_halfword(1);
	if (a) { // If the halfword after high_score is not all 1's
		flash_write_halfword(0, 0);
		high_score = 0;
	}

	uint8_t* planet_heightmap;

	entity_t* player = entity_init(Spaceship, PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, PLAYER_DEFAULT_VEL_X, 0);

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	listnode_t* bullets = NULL; // Initialise empty list of bullets
	listnode_t* bombs = NULL; // Initialise empty list of bombs

	entity_t* powerup = NULL; // Initialise powerup

	fixp_t blast_radius[2] = {fixp_fromint(20), fixp_fromint(64)};

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
  		case MainMenu:
  	  		// ------------------------------
  	  		// |  MAIN MENU STATE			|
  	  		// ------------------------------
  			if (state_transition) {
  				if (!(last_state == HelpMenu || last_state == DeathMenu)) {
  					draw_menu_screen();
  				}
  				if (last_state == DeathMenu) {
  					draw_menu_controls();
  				}
  				set_led(3);
  				draw_main_menu(menu_selection);
  				draw_menu_title("Main Menu");

  				enable_timer_2(1); // Turn on the punk
  				enable_timer_17(1);// Turn on the punk
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

  			if (next_state == Game || next_state == BossScreen) {
  				enable_timer_2(0); // Turn off music
  				enable_timer_17(0);// Turn off music
  			}
  			break;
  		case HelpMenu:
  			// ------------------------------
  			// |  HELP MENU STATE			|
  			// ------------------------------
  			if (state_transition) {
  				if (!(last_state == MainMenu || last_state == DeathMenu)) {
					draw_menu_screen();
				}
  				draw_menu_title("Help");
  				set_led(3);
  				draw_help_menu();
  			}

  			if (red_btn) {
  				next_state = MainMenu;
  			}

  			if (next_state == BossScreen) {
  				enable_timer_2(0); // Turn off music
  				enable_timer_17(0);// Turn off music
  			}
  			break;
  		case Game:
  			// ------------------------------
  			// |  GAME LOOP STATE			|
  			// ------------------------------
  			if (state_transition) {
  				planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap
  				set_led(2);
  				if (last_state != BossScreen) {
  				lives = 3;
  				level = 0;
				score = 0;
				kills = 0;
				nukes_cnt = 0;
  				}

				player->x = PLAYER_DEFAULT_X;
				player->y = PLAYER_DEFAULT_Y;
				player->vel_x = PLAYER_DEFAULT_VEL_X;
				player->vel_y = 0;
  			}

  			if (enemies == NULL) {
  				level++;
  				level_setup(&enemies, level, planet_heightmap);
  			}

  			// TODO/note: kinda important, always update bombs after updating enemies, as enemies do not check for is_dead
			
  			if (update_flag & 1) {	// Update enemies and bullets

  				update_entities(enemies, &bullets, player, planet_heightmap, 0, blast_radius, (level < 20 ? 50 - level : 30));
  				lives -= update_entities(bullets, NULL, player, planet_heightmap, 0, NULL, (level < 20 ? 50 - level : 30));

  				cleanup_entities(&enemies, planet_heightmap);
  				cleanup_entities(&bullets, planet_heightmap);
  				draw_entities(enemies, planet_heightmap);
				draw_entities(bullets, planet_heightmap);

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
							fixp_t powerup_x = enemies ? ((entity_t *) enemies->ptr)->x : fixp_fromint(30); // TODO != NULL (also was preciously 30<<13)
							fixp_t powerup_y = enemies ? (((entity_t *) enemies->ptr)->y) - fixp_fromint(20) : fixp_fromint(30);
							powerup = entity_init(Powerup, powerup_x, powerup_y, 0 , 0);
						}
					}
				}

				update_flag &= ~1;
			}


  			if (update_flag & (1 << 1)){ // Update player and bombs

  		  		kills += update_entities(bombs, &enemies, NULL, planet_heightmap, GRAVITY, blast_radius, 0); // return number of enemies killed
  				cleanup_entities(&bombs, planet_heightmap);
  				draw_entities(bombs, planet_heightmap);

  		  		// Update position of player
				if (js[0] || js[1]) {
					player->update_velocity(player, 4*js[0], -2*js[1]);
				}
				uint8_t collisions = player->move(player, planet_heightmap, 0); // Returns collision from check_collision()


				if (collisions & 0b1000) {
					lives = 0;
					// Player has hit ground, game over. TODO
				}

				// Draw player
				player->draw(player, planet_heightmap, 1);

  				// Rising edge detection of input buttons
  		  		uint8_t red_btn_rising = red_btn && !prev_red_btn;
  		  		uint8_t gray_btn_rising = gray_btn && !prev_gray_btn;

  				if (gray_btn_rising && !state_transition) { // Fire bomb?
  					// Fire bomb!
  					list_push(&bombs, entity_init(Bomb, player->x, player->y, player->vel_x, player->vel_y));
  				}

  				if (red_btn_rising && nukes_cnt && !state_transition) { // Fire nuke if enough?
  					// Fire nuke!
  					nukes_cnt--;
  					list_push(&bombs, entity_init(Nuke, player->x, player->y, player->vel_x, player->vel_y));
  				}

				// Rising edge detection of input buttons
				prev_red_btn = red_btn;
				prev_gray_btn = gray_btn;
				if (level) {
					score = ((level-1)<<6 )+ (kills<<3); // Arbitrary score calculation
				}

				update_flag &= ~(1<<1);
			}

			if (lives < 1) {
				next_state = DeathMenu;
			}

			if (next_state != Game) {
				punk_address = punk; // Restart punk
			}

  			break;
  		case DeathMenu:
  			// ------------------------------
  			// |  DEATH MENU STATE			|
  			// ------------------------------
  			if(state_transition) {
  				if (!(last_state == MainMenu || last_state == HelpMenu)) {
					draw_menu_screen();
					while (bullets != NULL) {
						free(list_pop(&bullets));
					}
					while (bombs != NULL) {
						free(list_pop(&bombs));
					}
					while (enemies != NULL) {
						free(list_pop(&enemies));
					}
					if (powerup != NULL) {
						free(powerup);
						powerup = NULL;
					}

					enable_timer_2(1);  // Turn on the punk
					enable_timer_17(1); // Turn on the punk
				}
  				if (score > high_score) {
  					high_score = score;
  					flash_write_halfword(0, high_score);
				}

  				draw_menu_title("You Lost :(");
  	  			draw_death_menu(level, score, kills, high_score);
  	  			set_led(1);
  			}

  			if (gray_btn) {
  				next_state = Game;
  			} else if (red_btn) {
  				next_state = MainMenu;
  			}

  			if (next_state == Game || next_state == BossScreen) {
  				enable_timer_2(0); // Turn off music
  				enable_timer_17(0);// Turn off music
  			}
  			break;
  		case BossScreen:
  			// ------------------------------
  			// |  BOSS SCREEN STATE			|
  			// ------------------------------
  			if (state_transition) {
				draw_boss_screen();
				//enable_timer_2 (0);
				enable_timer_15 (0);
				enable_timer_16 (0);
				set_led(3);
  			}

  			if (last_keypress == 'b') {
  				//enable_timer_2 (1);
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

void TIM1_BRK_TIM15_IRQHandler (void) {
	update_flag |= 1<<1; // Set player/bomb flag high
	TIM15->SR &= ~0x0001;
}

void TIM1_UP_TIM16_IRQHandler (void) {
	update_flag |= 1; // Set enemy/bullet flag high
	TIM16->SR &= ~0x0001;
}

void TIM1_TRG_COM_TIM17_IRQHandler (void) {
	TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk;
	TIM17->SR &= ~0x0001;
}
