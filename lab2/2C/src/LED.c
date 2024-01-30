/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	RCC->AHB2ENR |= (uint32_t)RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= (uint32_t)RCC_AHB2ENR_GPIOCEN;
	
	// Initialize Green LED
	// Set Mode to Output (01)
    GPIOA -> MODER |= (uint32_t)GPIO_MODER_MODE5_0;
    GPIOA -> MODER &= ~(uint32_t)GPIO_MODER_MODE5_1;
    // Set PUPD Type to Pull-Up (00)
	GPIOA -> PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD5_0;
	GPIOA -> PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD5_0;
    // Set Output type to Push-Pull (0, reset)
    GPIOA -> OTYPER &= ~(uint32_t)GPIO_OTYPER_OT5;
}

void Green_LED_Off(void) {
	GPIOA -> ODR &= ~(uint32_t)GPIO_ODR_OD5;
}

void Green_LED_On(void) {
	GPIOA -> ODR |= (uint32_t)GPIO_ODR_OD5;
}

void Green_LED_Toggle(void){
	GPIOA -> ODR ^= GPIO_ODR_OD5;
}
