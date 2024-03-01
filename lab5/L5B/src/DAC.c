#include "stm32l476xx.h"
#include "DAC.h"

static void DAC_Pin_Init(void) {
    // PA4 for analog input
    
    // GPIO A clock enable
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Set PA4 mode to analog (11)
    GPIOA->MODER |= GPIO_MODER_MODE4;

    // Set PA4 PUPD to no pull-up/pull-down (00)
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;
}

void DAC_Write_Value(uint32_t value) {
    // Write DAC1Ch1 right-aligned 12-bit value
    if (value > 4095) value = 4095;
    DAC1->DHR12R1 = value;
}

void DAC_Init(void) {
    DAC_Pin_Init();

    // Enable DAC clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
    
    // Disable DAC channel 1
    DAC1->CR &= ~DAC_CR_EN1;

    // Enable software trigger mode (111)
    DAC1->CR |= DAC_CR_TSEL1;

    // Disable trigger
    DAC1->CR &= ~DAC_CR_TEN1;

    // DAC1 connected in normal mode to external pin only with buffer enabled (000)
    DAC1->MCR &= ~DAC_MCR_MODE1;

    // Enable DAC channel 1
    DAC1->CR |= DAC_CR_EN1;

    DAC_Write_Value(0);
}
