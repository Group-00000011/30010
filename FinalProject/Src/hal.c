#include "hal.h"


void led_init() {
	// Enable clock for used ports
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC;


	// Reset and set PB4 speed register (red)
	GPIOB->OSPEEDR &= ~(0x00000003 << (4 * 2));
	GPIOB->OSPEEDR |= (0x00000002 << (4 * 2));

	// Reset and set PB4 type register (red)
	GPIOB->OTYPER &= ~(0x0001 << (4));
	GPIOB->OTYPER |= (0x0000 << (4));

	// Reset and set PB4 mode register (red)
	GPIOB->MODER &= ~(0x00000003 << (4 * 2));
	GPIOB->MODER |= (0x00000001 << (4 * 2));


	// Reset and set PC7 speed register (green)
	GPIOC->OSPEEDR &= ~(0x00000003 << (7 * 2));
	GPIOC->OSPEEDR |= (0x00000002 << (7 * 2));

	// Reset and set PC7 type register (green)
	GPIOC->OTYPER &= ~(0x0001 << (7));
	GPIOC->OTYPER |= (0x0000 << (7));

	// Reset and set PC7 mode register (green)
	GPIOC->MODER &= ~(0x00000003 << (7 * 2));
	GPIOC->MODER |= (0x00000001 << (7 * 2));


	// Reset and set PA9 speed register (blue)
	GPIOA->OSPEEDR &= ~(0x00000003 << (9 * 2));
	GPIOA->OSPEEDR |= (0x00000002 << (9 * 2));

	// Reset and set PB4 type register (blue)
	GPIOA->OTYPER &= ~(0x0001 << (9));
	GPIOA->OTYPER |= (0x0000 << (9));

	// Reset and set PB4 mode register (blue)
	GPIOA->MODER &= ~(0x00000003 << (9 * 2));
	GPIOA->MODER |= (0x00000001 << (9 * 2));

}



void set_led(uint8_t color) {
	// 0 - off / 1 - red / 2 - green / 3 - blue / 4 - cyan / 5 - magenta / 6 - yellow / 7 - white
	if (color > 7) color = 0;

	switch (color) {
	case 0:
		GPIOB->ODR |= (0x0001 << 4);
		GPIOC->ODR |= (0x0001 << 7);
		GPIOA->ODR |= (0x0001 << 9);
		break;
	case 1:
		GPIOB->ODR &= ~(0x0001 << 4);
		GPIOC->ODR |= (0x0001 << 7);
		GPIOA->ODR |= (0x0001 << 9);
		break;
	case 2:
		GPIOB->ODR |= (0x0001 << 4);
		GPIOC->ODR &= ~(0x0001 << 7);
		GPIOA->ODR |= (0x0001 << 9);
		break;
	case 3:
		GPIOB->ODR |= (0x0001 << 4);
		GPIOC->ODR |= (0x0001 << 7);
		GPIOA->ODR &= ~(0x0001 << 9);
		break;
	case 4:
		GPIOB->ODR |= (0x0001 << 4);
		GPIOC->ODR &= ~(0x0001 << 7);
		GPIOA->ODR &= ~(0x0001 << 9);
		break;
	case 5:
		GPIOB->ODR &= ~(0x0001 << 4);
		GPIOC->ODR |= (0x0001 << 7);
		GPIOA->ODR &= ~(0x0001 << 9);
		break;
	case 6:
		GPIOB->ODR &= ~(0x0001 << 4);
		GPIOC->ODR &= ~(0x0001 << 7);
		GPIOA->ODR |= (0x0001 << 9);
		break;
	case 7:
		GPIOB->ODR &= ~(0x0001 << 4);
		GPIOC->ODR &= ~(0x0001 << 7);
		GPIOA->ODR &= ~(0x0001 << 9);
		break;

	}
}
