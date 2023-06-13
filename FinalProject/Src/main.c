#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "ansi.h"
#include "hal.h"
#include "daftpunk8bit.h"

int main(void)
{
	uart_init(9600);
	led_init();

	init_timer_2();

	//buzzer_set_pwm(128);
	//uint8_t on = 0;
	while (1) {
		TIM2->CCR3 = 0;
		TIM2->CCR3 = 255;
		/*buzzer_set_pwm(0);
		buzzer_set_pwm(255);*/
	}

	while(1){}
}
