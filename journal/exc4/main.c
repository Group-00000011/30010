#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "ansi.h"

void initports();
uint8_t readJoystick();
void printInput(uint8_t input);
void setLED(uint8_t color);

int main(void)
{
	uart_init(9600);
	clrscr();
	gotoxy(0,0);
	initports();
	setLED(0);


	uint8_t lastInput;
	uint8_t currentInput;
	while(1){

		currentInput = readJoystick();

		if (currentInput != lastInput) {
			printInput(currentInput);
		}

		lastInput = currentInput;
	}
}


uint8_t readJoystick() {
	uint16_t right = GPIOC->IDR & (0x0001 << 0);
	uint16_t up = GPIOA->IDR & (0x0001 << 4);
	uint16_t center = GPIOB->IDR & (0x0001 << 5);
	uint16_t left = GPIOC->IDR & (0x0001 << 1);
	uint16_t down = GPIOB->IDR & (0x0001 << 0);

	uint8_t out = 0;

	if (up) {
		out |= 0x01 << 0;
	}
	if (down) {
		out |= 0x01 << 1;
	}
	if (left) {
		out |= 0x01 << 2;
	}
	if (right) {
		out |= 0x01 << 3;
	}
	if (center) {
		out |= 0x01 << 4;
	}

	return out;
}

void printInput(uint8_t input) {
	uint8_t printed = 0;

	if (input & 0x01 << 0) {
		printf("%s", "Up");
		setLED(1);
		printed++;
	}
	if (input & 0x01 << 1) {
		printf("%s", printed ? ", down": "Down");
		setLED(2);
		printed++;
	}
	if (input & 0x01 << 2) {
		printf("%s", printed ? ", left": "Left");
		setLED(3);
		printed++;
	}
	if (input & 0x01 << 3) {
		printf("%s", printed ? ", right": "Right");
		setLED(4);
		printed++;
	}
	if (input & 0x01 << 4) {
		printf("%s", printed ? ", center": "Center");
		setLED(5);
		printed++;
	}
	if (printed) {
		printf("\n");
	} else {
		setLED(0);
	}
}


void setLED(uint8_t color) {
	// 0 - off / 1 - red / 2 - green / 3 - blue / 4 - cyan / 5 - magenta / 6 - yellow / 7 - white
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


void initports() {
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


	// Clear mode registers
	GPIOC->MODER &= ~(0x00000003 << (0 * 2));
	GPIOA->MODER &= ~(0x00000003 << (4 * 2));
	GPIOB->MODER &= ~(0x00000003 << (5 * 2));
	GPIOC->MODER &= ~(0x00000003 << (1 * 2));
	GPIOB->MODER &= ~(0x00000003 << (0 * 2));

	// Set pin PC0 to input (right)
	GPIOC->MODER |= (0x00000000 << (0 * 2));
	GPIOC->PUPDR &= ~(0x00000003 << (0 * 2));
	GPIOC->PUPDR |= (0x00000002 << (0 * 2));

	// Set pin PA4 to input (up)
	GPIOA->MODER |= (0x00000000 << (4 * 2));
	GPIOA->PUPDR &= ~(0x00000003 << (4 * 2));
	GPIOA->PUPDR |= (0x00000002 << (4 * 2));

	// Set pin PB5 to input (center)
	GPIOB->MODER |= (0x00000000 << (5 * 2));
	GPIOB->PUPDR &= ~(0x00000003 << (5 * 2));
	GPIOB->PUPDR |= (0x00000002 << (5 * 2));

	// Set pin PC1 to input (left)
	GPIOC->MODER |= (0x00000000 << (1 * 2));
	GPIOC->PUPDR &= ~(0x00000003 << (1 * 2));
	GPIOC->PUPDR |= (0x00000002 << (1 * 2));

	// Set pin PB0 to input (down)
	GPIOB->MODER |= (0x00000000 << (0 * 2));
	GPIOB->PUPDR &= ~(0x00000003 << (0 * 2));
	GPIOB->PUPDR |= (0x00000002 << (0 * 2));
}

