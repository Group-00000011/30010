/*
 * timer.c
 *
 *  Created on: 12 Jun 2023
 *      Author: thinkbox
 */

#include "timer.h"

void initTimer15(uint16_t ARR, uint16_t PSC){

	// Enable TIM15 clock
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable TIM15 clock


	//	1. Write to TIM15->CR1 to disable the timer and configure the mode.
	TIM15->CR1 = 0x0000; // Configure timer 15

	//	2. Write to TIM15->ARR to set the reload value.
		TIM15->ARR = ARR; // Set reload value

	//	3. Write to TIM15->PSC to set the prescaler.
		TIM15->PSC = PSC; // Set prescale value

	//	4. Configure the timer interrupt.
		TIM15->DIER |= 0x0001;

	//	5. Write to TIM15->CR1 to enable the timer and begin counting
		TIM15->CR1 |= 0x0001;

	// Enable intterupt
		NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0b0000); 	// Set interrupt priority
		NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); 				// Enable interrupt


}

void enableTimer15() {
        TIM15->CR1 |= 0x0001;
}

void TIM1_BRK_TIM15_IRQHandler(void) {

	// Do whatever you want here, but make sure it doesn’t take too much Time.

	TIM15->SR &= ~0x0001; // Clear interrupt bit

}

