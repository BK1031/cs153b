#include "SysClock.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source (8 MHz)
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_MSI;
	
	// Set MSI clock range
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	RCC->CR &= ~RCC_CR_MSION; // MSI off to modify MSIRANGE
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7; // 8 MHz
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= RCC_CR_MSIRGSEL;
	// Enable MSI oscillator
	RCC->CR |= RCC_CR_MSION;
	// Wait until MSI is ready
	while(!(RCC->CR & RCC_CR_MSIRDY));
	// System clock switch status set by hardware to MSI (00)
	while(RCC->CFGR & RCC_CFGR_SWS);
}
