/*
 * ECE 153B - Winter 2024
 *
 * Name(s):
 * Section:
 * Lab: 1
 */
#include "stm32l476xx.h"

void Init(){
    // Enable HSI
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

    // Enable GPIO Clock
    // [TODO]

    // Initialize Green LED
    // [TODO]

    // Initialize User Button
    // [TODO]
}

int main(void){
    // Initialization
    Init();

    // Polling to Check for User Button Presses
    // [TODO]

    return 0;
}
