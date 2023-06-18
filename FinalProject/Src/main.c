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



int main(void)
{
	// Initialise hardware
	uart_init(500000);
	led_init();
	lcd_init();

	init_timer_2();
	init_timer_15();

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

		for (int i = 0; i < 100000; ++i) {} // Wait a bit
	}

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
