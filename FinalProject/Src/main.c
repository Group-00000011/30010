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

uint8_t update_flag = 0; // [0] = update enemies

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

	//joystick_conf();

	// Create spaceship
	//entity_t* spaceship = entity_init(Spaceship, 10<<14, 10<<14, 0);;

	bgcolor(SPACE_COLOR);
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
	/*gotoxy(2,2);
	printf("%c%c%c%c%c%c ", 0x5C,0xDB,0xDF,0xDF,0xDB,0x5C);
	gotoxy(2,3);
	printf("/%c%c%c%c/", 0xDB,0xDC,0xDC,0xDB);

	draw_menu_screen();
	//draw_menu_title("Main Menu");
	//draw_main_menu_opts(1);

	draw_menu_title("Help Menu");
	draw_help_menu();*/



	uint8_t* planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	list_push(&enemies, entity_init(Enemy, 240<<14, 10<<14, fixp_fromint(1)));
	list_push(&enemies, entity_init(Enemy, 25<<14, 10<<14, fixp_fromint(-1)));
	list_push(&enemies, entity_init(Enemy, 50<<14, 35<<14, fixp_fromint(1)));
	//free(list_remove(&enemies, 1)); // This is the syntax to pop or remove items from a list

	while (1) {
		if (update_flag & 1) {
			bgcolor(0);
			fgcolor(8);

			listnode_t* current = enemies;
			while (current != NULL) {
				entity_t* current_entity = current->ptr;

				if (current_entity->type == Enemy) {
					enemy_move(current_entity, planet_heightmap);
				}

				current_entity->draw(current_entity);
				current = current->next;
			}

			//spaceship->draw(spaceship);
			//spaceship.update_position(&spaceship, pos_x, 10);
  			//TIM2->CCR3 = 255;

      			/*printf("vert: %d    \n",joystick_vert());
			printf("hori: %d    ",joystick_hori());*/

			update_flag &= ~1;
		}
	}
}

void TIM1_BRK_TIM15_IRQHandler(void) {
	/*TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk_begin;*/

	TIM15->SR &= ~0x0001;
}

void TIM1_UP_TIM16_IRQHandler(void) {
	update_flag |= 1;
	TIM16->SR &= ~0x0001;
}
