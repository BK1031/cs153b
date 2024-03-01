#include "EXTI.h"

#include "DAC.h"

void EXTI_Init(void) {
	// GPIO C clock enable
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	/* Initialize User Button (PC13) */
  // Set mode of PC13 to input (00)
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	// Set PUPD of PC13 to no pull-up/pull-down (00)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13;
	
	/* Configure SYSCFG EXTI */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	/* Configure EXTI Trigger */
	// Disable rising edge
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
	// Enable falling edge
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
	/* Enable EXTI */
	// Set Interrupt Mask Register
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	/* Configure and Enable in NVIC */
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;
	
	// Do something
	if (direction == UP) {
		if (dac_value + DAC_INCREMENT < DAC_MAX) {
			dac_value += DAC_INCREMENT;
		} else {
			dac_value = DAC_MAX;
			direction = DOWN;
		}
	} else if (direction == DOWN) {
		if (dac_value - DAC_MIN > DAC_INCREMENT) { // mathematically same as (dac_value - DAC_INCREMENT > DAC_MIN) but coded like this to avoid subtraction underflow with uint32_t
			dac_value -= DAC_INCREMENT;
		} else {
			dac_value = DAC_MIN;
			direction = UP;
		}
	}
	DAC_Write_Value(dac_value);
}
