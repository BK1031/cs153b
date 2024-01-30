#include "stm32l476xx.h"

void GPIO_Init(void){	
	// Enable HSI
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );
	//enable clock for GPIO port C
	RCC->AHB2ENR |= (uint32_t) RCC_AHB2ENR_GPIOCEN;

	//configure port c pins 5, 6, 8, 9 as output (01)
	GPIOC->MODER |= (uint32_t) GPIO_MODER_MODE5_0;
	GPIOC->MODER &= ~(uint32_t) GPIO_MODER_MODE5_1;
	GPIOC->MODER |= (uint32_t) GPIO_MODER_MODE6_0;
	GPIOC->MODER &= ~(uint32_t) GPIO_MODER_MODE6_1;	
	GPIOC->MODER |= (uint32_t) GPIO_MODER_MODE8_0;
	GPIOC->MODER &= ~(uint32_t) GPIO_MODER_MODE8_1;
	GPIOC->MODER |= (uint32_t) GPIO_MODER_MODE9_0;
	GPIOC->MODER &= ~(uint32_t) GPIO_MODER_MODE9_1;	

	//set output speed of pins to fast (10)
	GPIOC->OSPEEDR &= ~(uint32_t) GPIO_OSPEEDR_OSPEED5_0;
	GPIOC->OSPEEDR |= (uint32_t) GPIO_OSPEEDR_OSPEED5_1;
	GPIOC->OSPEEDR &= ~(uint32_t) GPIO_OSPEEDR_OSPEED6_0;
	GPIOC->OSPEEDR |= (uint32_t) GPIO_OSPEEDR_OSPEED6_1;
	GPIOC->OSPEEDR &= ~(uint32_t) GPIO_OSPEEDR_OSPEED8_0;
	GPIOC->OSPEEDR |= (uint32_t) GPIO_OSPEEDR_OSPEED8_1;
	GPIOC->OSPEEDR &= ~(uint32_t) GPIO_OSPEEDR_OSPEED9_0;
	GPIOC->OSPEEDR |= (uint32_t) GPIO_OSPEEDR_OSPEED9_1;

	//set output type of pins to push-pull (0, reset)
	GPIOC->OTYPER &= ~(uint32_t) GPIO_OTYPER_OT5;
	GPIOC->OTYPER &= ~(uint32_t) GPIO_OTYPER_OT6;
	GPIOC->OTYPER &= ~(uint32_t) GPIO_OTYPER_OT8;
	GPIOC->OTYPER &= ~(uint32_t) GPIO_OTYPER_OT9;
	

	//set pins to no pull-up, no pull-down (00, reset)
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD5_0;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD5_1;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD6_0;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD6_1;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD8_0;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD8_1;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD9_0;
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD9_1;
	
}


#define DELAY 6000	// delay between steps of the sequences

/*
A - PC5
A' - PC6
B - PC8
B' - PC9

*/
void Full_Stepping_Clockwise(void){
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}
	
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}
}

void Full_Stepping_CounterClockwise(void){
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}
	
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}
}

void Half_Stepping_Clockwise(void){
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}


}

void Half_Stepping_CounterClockwise(void){
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}

	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD5;
	GPIOC->ODR &= ~(uint32_t)GPIO_ODR_OD6;
	GPIOC->ODR &=~(uint32_t)GPIO_ODR_OD8;
	GPIOC->ODR |= (uint32_t)GPIO_ODR_OD9;
	for (int i=0; i < DELAY; i++) {}
}


int main(void){
	GPIO_Init();
	int i = 0;
	while (i++ < 512)  {
		//Full_Stepping_Clockwise();
		//Full_Stepping_CounterClockwise();
		//Half_Stepping_Clockwise();
		Half_Stepping_CounterClockwise();
		
	}
}