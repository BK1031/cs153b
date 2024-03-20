/*
 * ECE 153B
 *
 * Name(s): 
 * Section: 
 * Project
 */
 
#include "SysTimer.h"
#include "motor.h"
#include "LED.h"

static uint32_t volatile msTicks = 0; // global millisecond ticks
static uint32_t volatile stepperTicks = 0; // clock for stepper motor
static uint32_t volatile waitTicks = 0; // counter for wait()
static uint8_t volatile waitEnable = 0; // counter enable for wait()

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter

	SysTick->LOAD = 79999;
	SysTick->VAL = 0;
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1

	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
	++msTicks;
	
	// Clock for rotateStep()
	++stepperTicks;
	if (stepperTicks == 2) {
		rotateStep();
		stepperTicks = 0;
		
//		// Stop condition for rotateDegrees()
//		if (get_nStepsEnable() && get_nSteps() == 0) {
//			setDire(0);
//			set_nStepsEnable(0);
//		}
	}
	
	// Counter for wait()
	if (waitEnable)	{
		waitTicks--;
		
		// Stop condition for wait()
		if (waitTicks == 0) {
			waitEnable = 0;
			waitDone();
		}
	}
}

void delay(uint32_t ms) {
	uint32_t endTick = msTicks + ms;
	while (msTicks < endTick);
}

void wait(uint32_t ms) {
	waitTicks = ms;
	waitEnable = 1;
}
