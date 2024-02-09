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
uint32_t volatile distance = 0;

void Trigger_Setup() {
	RCC->AHB2ENR |= (uint32_t)RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(uint32_t)GPIO_MODER_MODE9_0;
	GPIOA->MODER |= (uint32_t)GPIO_MODER_MODE9_1;
	
	GPIOA->AFR[1] &= ~(uint32_t)GPIO_AFRH_AFSEL9;
	GPIOA->AFR[1] |= (uint32_t)GPIO_AFRH_AFSEL9_0;
	
	GPIOA->PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD9_0;
	GPIOA->PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD9_1;
	
	GPIOA->OTYPER &= ~(uint32_t)GPIO_OTYPER_OT_9;
	
	GPIOA->OSPEEDR |= (uint32_t)GPIO_OSPEEDER_OSPEEDR9_0;
	GPIOA->OSPEEDR |= (uint32_t)GPIO_OSPEEDER_OSPEEDR9_1;
	
	RCC->APB2ENR |= (uint32_t)RCC_APB2ENR_TIM1EN;
	
	TIM1->PSC = 15;
	
	TIM1->CR1 |= TIM_CR1_ARPE;
	TIM1->ARR = 0xFFFF;

	TIM1->CCR2 = 10;
	
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2;
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1;

	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	TIM1->CCER |= TIM_CCER_CC2E;
	
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->EGR |= TIM_EGR_UG;
	
	TIM1->DIER |= TIM_DIER_UIE;
	TIM1->SR &= ~TIM_SR_UIF;
	
	TIM1->CR1 &= ~(uint32_t)TIM_CR1_DIR;
	TIM1->CR1 |= (uint32_t)TIM_CR1_CEN;
	
}

void TIM4_IRQHandler(void) {
	if (TIM4->SR & TIM_SR_UIF) {
		TIM4->SR &= ~TIM_SR_UIF;
		overflowCount++;
	}
	
	if (TIM4->SR & TIM_SR_CC1IF) {
		TIM4->SR &= ~TIM_SR_CC1IF;
		
		if ((GPIOB->IDR & GPIO_IDR_ID6)) {
			lastValue = TIM4->CCR1;
		}
		else {
			currentValue = TIM4->CCR1;
			overflowCount = 0;
		}
	}
	TIM1->SR &= ~TIM_SR_UIF;
	TIM1->SR &= ~TIM_SR_CC2IF;
}

void Input_Capture_Setup() {
	RCC->AHB2ENR |= (uint32_t)RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= ~(uint32_t)GPIO_MODER_MODE6_0;
  	GPIOB->MODER |= (uint32_t)GPIO_MODER_MODE6_1;
	
	GPIOB->AFR[0] &= ~(uint32_t)GPIO_AFRL_AFSEL6;
	GPIOB->AFR[0] |= (uint32_t)GPIO_AFRL_AFSEL6_1;
	
	GPIOB->PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD6_0;
  	GPIOB->PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD6_1;
	
	RCC->APB1ENR1 |= (uint32_t)RCC_APB1ENR1_TIM4EN;
	
	TIM4->PSC = 15;
	
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->ARR = 0xFFFF;
	
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	TIM4->CCER |= TIM_CCER_CC1P;
	TIM4->CCER |= TIM_CCER_CC1NP;
	TIM4->CCER |= TIM_CCER_CC1E;
	
	TIM4->DIER |= TIM_DIER_CC1IE;
	TIM4->DIER |= TIM_DIER_UDE;
	TIM4->DIER |= TIM_DIER_UIE;
	
	TIM4->EGR |= TIM_EGR_UG;
	
	TIM4->SR &= ~TIM_SR_UIF;
	
	TIM4->CR1 &= ~(uint32_t)TIM_CR1_DIR;
	TIM4->CR1 |= (uint32_t)TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
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

	volatile uint32_t temp1 = 0;
	volatile uint32_t temp2 = 0;
	
	while(1) {
		timeInterval = (currentValue - lastValue + (1 + 0xFFFF) * overflowCount);
		distance = timeInterval / 58;
		if (distance > 50) { 
			distance = 0;
		}
		
	}
}