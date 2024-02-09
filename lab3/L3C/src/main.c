/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Input_Capture_Setup() {
	// [TODO]
}

void TIM4_IRQHandler(void) {
	// [TODO]
}

void Trigger_Setup() {
    // (a) Enable the clock for GPIO Port A
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    // (b) Configure PA9 to be used as alternative function TIM1 CH2
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
    
    // Set the alternative function type to TIM1_CH2 (AF1)
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_0;
    
    // (c) Set PA9 to no pull-up, no pull-down
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9_1;

    // (d) Set the output type of PA9 to push-pull
    GPIOA->OTYPER &= ~(uint32_t)GPIO_OTYPER_OT9;
    
    // (e) Set PA9 to very high output speed
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1;
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	
	while(1) {
		// [TODO] Store your measurements on Stack
		
	}
}
