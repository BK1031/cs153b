/*
 * ECE 153B - Winter 2024
 *
 * Name(s): Bharat Kathi, Michael Wong
 * Section: Tuesday 7:00PM
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
    RCC->AHB2ENR |= (uint32_t)RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= (uint32_t)RCC_AHB2ENR_GPIOCEN;

    // ==== Initialize Green LED ====
    // Set Mode to Output (01)
    GPIOA -> MODER |= (uint32_t)GPIO_MODER_MODE5_0;
    GPIOA -> MODER &= ~(uint32_t)GPIO_MODER_MODE5_1;
    // Set PUPD Type to Pull-Up (00)
	GPIOA -> PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD5_0;
	GPIOA -> PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD5_0;
    // Set Output type to Push-Pull (0, reset)
    GPIOA -> OTYPER &= ~(uint32_t)GPIO_OTYPER_OT5;

    // ==== Initialize User Button ====
    // Set Mode to Input (00)
    GPIOC -> MODER &= ~(uint32_t)GPIO_MODER_MODE13_0;
	GPIOC -> MODER &= ~(uint32_t)GPIO_MODER_MODE13_1;
    // Set PUPD Type to Pull-Down (10)
    GPIOC -> PUPDR &= ~(uint32_t)GPIO_PUPDR_PUPD13_0;
    GPIOC -> PUPDR &= (uint32_t)GPIO_PUPDR_PUPD13_1;
}

int main(void){
    // Initialization
    Init();

    // Polling to Check for User Button Presses
    while (1) {
        if (GPIOC -> IDR & GPIO_IDR_ID13 == 0) {
            // Debounce the button, basically wait for the button to appear as pressed again within
            // 10 cycles to prevent multiple presses or noise from being registered
            int x = 0;
            while (x < 10) {
                if ((GPIOC -> IDR & GPIO_IDR_ID13) == 0) {
                    x++;
                }
            }
            GPIOA -> ODR ^= GPIO_ODR_OD5;
            x = 0;
	    }
    }

    return 0;
}
