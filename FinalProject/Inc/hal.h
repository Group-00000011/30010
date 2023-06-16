/*
 * hal.h
 *
 *  Created on: 12 Jun 2023
 *      Author: asbjorn
 */

#ifndef HAL_H_
#define HAL_H_

#include "stm32f30x_conf.h" // STM32 config

void led_init();
void set_led(uint8_t color);
void init_timer_15();
void init_timer_2();
void buzzer_set_pwm(uint8_t value);
void joystick_conf();
uint16_t joystick_hori();
uint16_t joystick_vert();


#endif /* HAL_H_ */
