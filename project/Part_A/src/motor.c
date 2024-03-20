/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "motor.h"

static const uint32_t MASK = ~(GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9);//cleras all motor pin bits
static uint32_t HalfStep[8] = {0x100,0x120,0x20,0xA0,0x80,0x90,0x10,0x110};//cw direction

static volatile int8_t dire = 0;
static volatile uint8_t step = 0;

void Motor_Init(void) {	
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
	GPIOC->PUPDR &= !(uint32_t) GPIO_PUPDR_PUPD9_1;}


//halfstep cooresponds with all the state elements
void rotate(void) {
	//changes states (8 diff states from the steppign sequence)
	int i = 0;
	while (i++ < 7) {
		GPIO->ODR &= MASK; //clear all bits
		GPIO->ODR |= HalfStep[i]; //set state

		//STEP 5: ???keep track of sequence with static counter (increment for CW, decrement for CCW)
		if (dire == 1) {
			step += 1;
		}
		if (dire == -1) {
			step -= 1;
		}

		for (int i=0; i < 6000; i++) {} //delay
	}
}

void setDire(int8_t direction) {
	if (direction == 1 ) {
		//CW - normal array
		HalfStep[0] = 0x100;
		HalfStep[1] = 0x120;
		HalfStep[2] = 0x20;
		HalfStep[3] = 0xA0;
		HalfStep[4] = 0x80;
		HalfStep[5] = 0x90;
		HalfStep[6] = 0x10;
		HalfStep[7] = 0x110;
	}
	else if (direction == 2) {
		//CCW - flip array
		HalfStep[7] = 0x100;
		HalfStep[6] = 0x120;
		HalfStep[5] = 0x20;
		HalfStep[4] = 0xA0;
		HalfStep[3] = 0x80;
		HalfStep[2] = 0x90;
		HalfStep[1] = 0x10;
		HalfStep[0] = 0x110;
	}
	else {
		//stop - all states are cleared
		HalfStep[0] = 0x0;
		HalfStep[1] = 0x0;
		HalfStep[2] = 0x0;
		HalfStep[3] = 0x0;
		HalfStep[4] = 0x0;
		HalfStep[5] = 0x0;
		HalfStep[6] = 0x0;
		HalfStep[7] = 0x0;
	}
}
