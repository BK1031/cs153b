#include "SysTimer.h"
#include "motor.h"

static volatile uint32_t msTicks;


//******************************************************************************************
// Initialize SysTick	
//******************************************************************************************	
void SysTick_Init(void){
	// The RCC feeds the Cortex System Timer (SysTick) external clock with the AHB clock
	// (HCLK) divided by 8. The SysTick can work either with this clock or with the Cortex clock
	// (HCLK), configurable in the SysTick Control and Status Register.
	
	//  SysTick Control and Status Register
	SysTick->CTRL = 0;										// Disable SysTick IRQ and SysTick Counter
	
	// SysTick Reload Value Register
	// Reload value = (time interval) x (clock frequency) - 1 = 1 ms x 80 MHz - 1 
	SysTick->LOAD = 79999;    // 1ms, 80MHz clock
	
	// SysTick Current Value Register
	SysTick->VAL = 0;

	NVIC_SetPriority(SysTick_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);					// Enable SysTick interrupt in NVIC

	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
}

//******************************************************************************************
// SysTick Interrupt Handler
//******************************************************************************************
void SysTick_Handler(void){
	msTicks++;

	
	// if (msTicks % 0 == 0) { //min delay 
		// step += dire; //im gonna move this down below rotate so rotat() starts from 0
		
			
		rotate();
		// step += dire;
	// }


}
	
//******************************************************************************************
// Delay in ms
//******************************************************************************************
void delay (uint32_t T){
	uint32_t currentTicks; // Hint: It may be helpful to keep track of what the current tick count is
	
	// Implement function that waits until a time specified by argument T
	currentTicks = msTicks;
	while (msTicks != currentTicks + T);
}
