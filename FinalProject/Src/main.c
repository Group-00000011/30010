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

uint8_t update_enemy_flag = 0;

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
	bgcolor(SPACE_COLOR);
	clrscr();
	gotoxy(1,1);
	printf("Hello\n");

	uint8_t* planet_heightmap = gfx_draw_background(); // gfx_draw_background return pointer to heightmap

	listnode_t* enemies = NULL; // Initialise empty list of enemies
	list_push(&enemies, entity_init(Enemy, 240<<14, 10<<14, fixp_fromint(1)));
	list_push(&enemies, entity_init(Enemy, 25<<14, 10<<14, fixp_fromint(-1)));
	list_push(&enemies, entity_init(Enemy, 50<<14, 35<<14, fixp_fromint(1)));
	//free(list_remove(&enemies, 1)); // This is the syntax to pop or remove items from a list

	while (1) {
		if (update_enemy_flag) {
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

			update_enemy_flag = 0;
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
	update_enemy_flag = 1;
	TIM16->SR &= ~0x0001;
}
