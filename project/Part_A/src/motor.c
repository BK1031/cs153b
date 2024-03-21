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
static uint32_t HalfStep[8] = {
	GPIO_ODR_OD5 | GPIO_ODR_OD9, 
	GPIO_ODR_OD5, 
	GPIO_ODR_OD5 | GPIO_ODR_OD8,
	GPIO_ODR_OD8, 
	GPIO_ODR_OD6 | GPIO_ODR_OD8,
	GPIO_ODR_OD6, 
	GPIO_ODR_OD6 | GPIO_ODR_OD9,
	GPIO_ODR_OD9 
	};//ccw direction

static volatile int8_t dire = 0; //either -1 for ccw, 0 for stop, or 1 for cw
static volatile int8_t step = 0;

void Motor_Init(void) {	
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
	if (step > 7) { //CW
			step = 0;
	} else if (step < 0) { //CCW
		step = 7;
	}
	GPIOC->ODR &= MASK; //clear all bits
	GPIOC->ODR |= HalfStep[step]; //set state
	step += -dire;
}

void setDire(int8_t direction) {
	dire = direction;
}
	


