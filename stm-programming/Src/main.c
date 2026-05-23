/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Andru Gomez
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"

int main(void){

// Exercise 0.1 Variables
uint8_t my_variable = 42;

//headers

int main(void)
{

}
	// turn on clock register for GPIOA
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

		// set GPIOA P5 as output
		GPIOA->MODER |= (0b1 << GPIO_MODER_MODE5_Pos);
		// set GPIOA P5 as push-pull
		GPIOA->OTYPER &= ~(0b0 << GPIO_OTYPER_OT5_Pos);
		// set GPIOA P5 as fast speed
		GPIOA->OSPEEDR |= (0b10 << GPIO_OSPEEDR_OSPEED5_Pos);
		// set GPIOA P5 ouput data register as 1 (turn on led)
		GPIOA->ODR |= (0b1 << GPIO_ODR_OD5_Pos);

    /* Loop forever */
	while(1){

	}



	while(1){

	}
	return 0;

}
