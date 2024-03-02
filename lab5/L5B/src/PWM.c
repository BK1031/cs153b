#include "PWM.h"

void LED_Pin_Init(void) {
  // Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // PA5 for green LED (LD2)
	
	// Initialize Green LED
	// Set PA5 mode to alternative function (10)
	GPIOA->MODER |= GPIO_MODER_MODE5_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
	
	// Set PA5 I/O output speed to very high speed (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
	
	// Set PA5 PUPD to no pull-up/pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
	
	// Select PA5 alternative function to AF1 (0001) (TIM2_CH1)
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
}
	

void TIM2_CH1_Init(void) {
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	// Counting direction: 0 = up-counting, 1 = down-counting
  TIM2->CR1 &= ~TIM_CR1_DIR; // up-counting

  // Prescaler
	TIM2->PSC = 7; // 8 MHz /(PSC+1) = 1 MHz, so 1 us ticks
	
  // Auto-reload
	TIM2->ARR = 4094; // 4.095 ms period PWM, so 250Hz frequency
	
	// Disable output compare mode
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	// PWM mode 1
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
	// Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	
	// Select output polarity: active high
	TIM2->CCER &= ~TIM_CCER_CC1P;
  // Enable output for ch1
	TIM2->CCER |= TIM_CCER_CC1E;
	
	// Output Compare Register for channel 1 
	TIM2->CCR1 = 0; // For PWM mode 1 (low-true) and up-counting, duty cycle = CCR/(ARR+1)
	
	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN;
}
