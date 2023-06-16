#include "hal.h"

#define AUDIO_BIT_DEPTH 0xff

void init_timer_2 () { // Timer 2 is used for PWM for buzzer
	RCC->APB1ENR |= RCC_APB1Periph_TIM2;
	//RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
	TIM2->CR1 &= 0xF400;	// Configure and disable timer
	TIM2->ARR = AUDIO_BIT_DEPTH;		// Set resolution to 8 bits
	TIM2->PSC = 4;		// Set prescaling/frequency to 568~440Hz, 4=62.5kHz, 2=125kHz
	TIM2->DIER |= 1;		// Enable timer 2 interrupts

	//NVIC_SetPriority(TIM2_IRQn, 0); // Set priority of interrupt
	//NVIC_EnableIRQ(TIM2_IRQn); 		// Enable the interrupt

	TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
	TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output
	TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
	TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
	TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
	TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
	TIM2->CCMR2 |= TIM_OCPreload_Enable;
	TIM2->CCR3 = 0;//AUDIO_BIT_DEPTH >> 2; // Set duty cycle to 50 %

	// Set pin PB10 to output
	//GPIOB->OSPEEDR &= ~(0x00000003 << (10 * 2)); // Clear speed register
	//GPIOB->OSPEEDR |= (0x00000002 << (10 * 2)); // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER &= ~(1 << 10); // Clear output type register
	GPIOB->OTYPER |= (0 << 10); // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER &= ~(0b11 << (10*2)); // Clear mode register
	GPIOB->MODER |= (0x02 << (10*2)); // Set mode register (0x00 – Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->AFR[1] &= ~(0xf << (2*4));
	GPIOB->AFR[1] |= (0x0001 << (2*4));
	// If above AFR doesn't work use this:
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);

	TIM2->CR1 |= 1;
}

void buzzer_set_pwm (uint8_t value) { // Sets the buzzer PWM pulse width to value/255
	TIM2->CCR3 = value;
}

void init_timer_15() {
	RCC->APB2ENR |= RCC_APB2Periph_TIM15;
	TIM15->CR1 &= 0xF470;	// Configure and disable timer
	TIM15->ARR = 16000;		// Set auto reload value to 100Hz (original:64000)
	TIM15->PSC = 0;			// Set prescaling to 1/(9+1) (original:9)
	TIM15->DIER |= 1;		// Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 1); // Set priority of interrupt
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); 		// Enable the interrupt

	TIM15->CR1 |= 1;
}

uint16_t joystick_vert(){
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_1Cycles5);

	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); // Wait for ADC read

	return ADC_GetConversionValue(ADC1); // Read the ADC value
}

uint16_t joystick_hori(){
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_1Cycles5);

	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); // Wait for ADC read

	return ADC_GetConversionValue(ADC1); // Read the ADC value
}

void joystick_conf(){
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port A

	// Set pin PC0 to input
	GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 - 	No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	// Set pin PC1 to input
	GPIOC->MODER &= ~(0x00000003 << (1 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (1 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (1 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (1 * 2)); // Set push/pull register (0x00 - 	No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	// Configure clock source
	RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12; // Clear ADC12 prescaler bits
	RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6; // Set ADC12 prescaler to 6
	RCC->AHBENR |= RCC_AHBPeriph_ADC12; // Enable clock for ADC12

	// Reset ADC to use standard reset configuration
	ADC1->CR = 0x00000000; // Clear CR register
	ADC1->CFGR &= 0xFDFFC007; // Clear ADC1 config register
	ADC1->SQR1 &= ~ADC_SQR1_L; // Clear regular sequence register 1

	// Prep for calibration
	ADC1->CR |= 0x10000000; // Enable internal ADC voltage regulator
	for (int i = 0 ; i < 1000 ; i++) {} // Wait for about 16 microseconds

	// Calibration
	ADC1->CR |= 0x80000000; // Start ADC1 calibration
	while (!(ADC1->CR & 0x80000000)); // Wait for calibration to finish
	for (int i = 0 ; i < 100 ; i++) {} // Wait for a little while

	// Finally enables ADC
	ADC1->CR |= 0x00000001; // (0x01 - Enable, 0x02 - Disable)
	while (!(ADC1->ISR & 0x00000001)); // Wait until ready


}


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
