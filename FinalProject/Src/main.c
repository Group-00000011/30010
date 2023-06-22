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

#define GRAVITY 0x0400

typedef enum State {NullState, MainMenu, HelpMenu, Game, DeathMenu, BossScreen} State;

/*
volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;
*/

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

	uint8_t prev_red_btn;
	uint8_t prev_gray_btn;

	fixp_t js[2];

	uint8_t last_keypress;

	uint8_t lives = 3;
	uint8_t level = 0;
	uint16_t kills = 0;
	uint16_t score = 0;

	uint8_t* planet_heightmap;

	entity_t* player = entity_init(Spaceship, 200<<14, 25<<14, -2<<14, 0);

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	listnode_t* bullets = NULL;
	listnode_t* bombs = NULL;
	//list_push(&enemies, entity_init(Enemy, 220<<14, 10<<14, fixp_fromint(1), 0));
	//list_push(&enemies, entity_init(Enemy, 25<<14, 10<<14, fixp_fromint(-1), 0));
	//list_push(&enemies, entity_init(Enemy, 50<<14, 35<<14, fixp_fromint(1), 0));
	list_push(&bombs, entity_init(Nuke, 120<<14, 10<<14, fixp_fromint(-1), fixp_fromint(-1)));
	list_push(&enemies, entity_init(Enemy, 200<<14, 0, 2<<14, 0));
	list_push(&enemies, entity_init(Enemy, 30<<14, 0, 2<<14, 0));
	list_push(&enemies, entity_init(Enemy, 35<<14, 0, -2<<14, 0));
	list_push(&enemies, entity_init(Enemy, 210<<14, 0, 2<<14, 0));

	fixp_t blast_radius[2] = {20<<14, 64<<14};
	//fixp_t nuke_blast_radius = 128<<14;

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
  				draw_main_menu(menu_selection);
  				draw_menu_title("Main Menu");
  			}

  			if (js[1] > (0x3 << 13)) {
  				if (menu_selection) {
  					last_menu_sel = menu_selection;
  					menu_selection--;
  				}
  			}

  			if (js[1] < (0x1 << 13)) {
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



  		case HelpMenu:
  			// ------------------------------
  			// |  HELP MENU STATE			|
  			// ------------------------------
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



  		case Game:
  			// ------------------------------
  			// |  GAME LOOP STATE			|
  			// ------------------------------
  			if (state_transition) {
  				planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap
  				lives = 3;
  				level = 0;
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

				if (!lives) {
					next_state = DeathMenu;
				}

				update_flag &= ~1;
			}


  			if (update_flag & (1 << 1)){ // Update player and bombs

  		  		update_entities(bombs, &enemies, NULL, planet_heightmap, GRAVITY, blast_radius, 0);
  				cleanup_entities(&bombs, planet_heightmap);
  				draw_entities(bombs, planet_heightmap);

  		  		// Update position of player
				if (js[0] || js[1]) {
					player->update_velocity(player, 4*js[0], -2*js[1]);
				}
				uint8_t collisions = player->move(player, planet_heightmap, 0); // Returns collision from check_collision()


				if (collisions & 0b1000) {
					gotoxy(1,3);
					printf("Player hit ground, game over!");
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

  				if (red_btn_rising && !state_transition) { // Fire nuke?
  					// Fire nuke!
  					list_push(&bombs, entity_init(Nuke, player->x, player->y, player->vel_x, player->vel_y));
  				}

				// Rising edge detection of input buttons
				prev_red_btn = red_btn;
				prev_gray_btn = gray_btn;

				update_flag &= ~(1<<1);
			}

  			break;
  		case DeathMenu:
  			// ------------------------------
  			// |  DEATH MENU STATE			|
  			// ------------------------------
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
  		case BossScreen:
  			// ------------------------------
  			// |  BOSS SCREEN STATE			|
  			// ------------------------------
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

  		// LCD information:
  		sprintf(lcd_lives.content, "Lives: %d", lives);
  		sprintf(lcd_level.content, "Level: %d", level);
  		sprintf(lcd_score.content, "Score: %d", score);
  		sprintf(lcd_kills.content, "Kills: %d", kills);

  		lcd_write_line(lcd_buffer, &lcd_lives);
  		lcd_write_line(lcd_buffer, &lcd_level);
  		lcd_write_line(lcd_buffer, &lcd_score);
  		lcd_write_line(lcd_buffer, &lcd_kills);


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
