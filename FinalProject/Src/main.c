#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "ansi.h"
#include "hal.h"
<<<<<<< HEAD
#include "string.h"
#include "lcd.h"

=======
#include "daftpunk8bit.h"


volatile uint8_t* punk_address = punk_long;
uint8_t* punk_end = punk_long + sizeof punk_long / sizeof *punk_long;
uint8_t* punk_begin = punk_long + 18500;
>>>>>>> buzzer

int main(void)
{
	// Initialise hardware
	uart_init(9600);
	led_init();
	lcd_init();

	init_timer_2();
	init_timer_15();



	//buzzer_set_pwm(128);

	while (1) {

		//TIM2->CCR3 = 255;
		/*buzzer_set_pwm(0);
		buzzer_set_pwm(255);*/
	}

	while(1){}
}

void TIM1_BRK_TIM15_IRQHandler(void) {
	TIM2->CCR3 = *punk_address;
	punk_address++;
	if (punk_address == punk_end)
		punk_address = punk_begin;
	TIM15->SR &= ~0x0001;
}
