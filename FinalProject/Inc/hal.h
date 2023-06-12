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



#endif /* HAL_H_ */
