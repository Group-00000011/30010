/*
 * hal.h
 *
 *  Created on: 12 Jun 2023
 *      Author: asbjorn
 */

#ifndef HAL_H_
#define HAL_H_

#include "stm32f30x_conf.h" // STM32 config
#include "data_structures.h"

void led_init();
void set_led(uint8_t color);
void init_timer_2();
void init_timer_15();
void init_timer_16();

//Joystick
void joystick_conf();
fixp_t joystick_hori();
fixp_t joystick_vert();
void enable_timer_2 (uint8_t on);
void enable_timer_15 (uint8_t on);
void enable_timer_16 (uint8_t on);

//Shouldnt we remove or outcomment it then?
void buzzer_set_pwm(uint8_t value); // Don't use this it is too slow. Write to the register directly.
//Shouldnt we remove or outcomment it then?


#endif /* HAL_H_ */
