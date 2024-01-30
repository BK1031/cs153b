/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 2B
 */


#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize User Button
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;


	// Set Mode to Input (00)
    GPIOC -> MODER &= ~(uint32_t)GPIO_MODER_MODE13_0;
	GPIOC -> MODER &= ~(uint32_t)GPIO_MODER_MODE13_1;
    // Set PUPD Type to Pull-Down (10)
    GPIOC -> PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD13_0;
    GPIOC -> PUPDR &= (uint32_t)GPIO_PUPDR_PUPD13_1;
	

	// Configure SYSCFG EXTI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Configure EXTI Trigger
	// EXTI->RTSR1 |= EXTI_RTSR1_RT3;
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);


	
}

void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;

	// Define behavior that occurs when interrupt occurs
	Green_LED_Toggle();
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
void ExampleInterruptHandler(void) {
}