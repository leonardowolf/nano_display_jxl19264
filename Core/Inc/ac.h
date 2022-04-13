/*
 * ac.h
 *
 *  Created on: Jan 6, 2022
 *      Author: hugin
 */

#ifndef INC_AC_H_
#define INC_AC_H_
#include "u8g2.h"
#define DEBOUNCE_TIME 500

uint8_t u8x8_stm32_gpio_and_delay( U8X8_UNUSED u8x8_t *u8x8,
U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
U8X8_UNUSED void *arg_ptr);

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

void start(void);
#endif /* INC_AC_H_ */
