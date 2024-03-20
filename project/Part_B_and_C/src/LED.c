/*
 * ECE 153B
 *
 * Name(s): 
 * Section: 
 * Project
 */

#include "LED.h"

void LED_Init(void) {
	/* Enable GPIO Clocks */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // PA5 for green LED (LD2)
	
	/* Initialize Green LED */
	// Set mode of PA5 to output (01)
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	// Set output type of PA5 to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
	// Set PUPD of PA5 to no pull-up/pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
}

void LED_Off(void) {
	GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void LED_On(void) {
	GPIOA->ODR |= GPIO_ODR_OD5;
}

void LED_Toggle(void) {
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
