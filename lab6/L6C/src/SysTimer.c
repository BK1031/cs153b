/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "SysTimer.h"

static uint32_t volatile step;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter

	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	// TODO
	
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
}

void SysTick_Handler(void) {
	++msTicks;
}

void delay(uint32_t ms) {
	// a) Reset the counter
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	// b) Reset VAL to 0 for SysTick
	SysTick->VAL = 0;

	// c) Set LOAD to a desirable value for SysTick (ARR = 1ms/T_clk - 1)
	SysTick->LOAD = 79999; 

	// d) enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// e) busy waiting for counter to reach desired value
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

	// f) disable SysTick for future use.
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void startTimer(void) {
	// i. Reset the counter
	step = 0;

	// ii. Reset VAL to 0 for SysTick
	SysTick->VAL = 0;

	// iii. Set LOAD to a desirable value for SysTick
	SysTick->LOAD = 79999; 

	// iv. Enable SysTick to start timing
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t endTimer(void) {
	// i. Disable SysTick first to pause the timer
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;


	// ii. Read the values from VAL and the counter
	uint32_t VALvalue = SysTick->VAL;
	uint32_t counterValue = step;

	// iii. Calculate the time using both numbers
	uint32_t timeDiff = (1/80000)*(counterValue + (1/80000)VALvalue); // in seconds

	return timeDiff;
}
