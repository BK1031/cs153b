#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Pin_Init(void) {
	// GPIO A clock enable
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Set PA1 mode to analog (11)
	GPIOA->MODER |= GPIO_MODER_MODE1;

    // Set PA1 PUPD to no pull-up/pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;

    // Connect PA1 analog switch to the ADC input (1)
    GPIOA->ASCR |= GPIO_ASCR_ASC1;
}

void ADC_Common_Configuration() {
    // I/O analog switch voltage booster enable (1)
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;

    // Vrefint enable (1)
    ADC123_COMMON->CCR |= ADC_CCR_VREFEN;

    // ADC prescaler: input ADC clock not divided (0000)
    ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;

    // ADC clock mode: HCLK/1 (Synchronous clock mode) (01)
    ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE_1;
    ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;

    // Dual ADC mode selection: All the ADCs independent (independent mode) (00000)
    ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
}

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Init(void) {
    // Enable the ADC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
    // Using one of the RCC reset registers, reset the ADC. Then, clear the reset bit
    RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;
    RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;

    // Other ADC Initialization
    ADC_Pin_Init();
    ADC_Common_Configuration();
    ADC_Wakeup();

    // Other Configuration
    // ADC disable
    ADC1->CR &= ~ADC_CR_ADEN;
    // 12-bit data resolution (00)
    ADC1->CFGR &= ~ADC_CFGR_RES;
    // Right data alighment (0)
    ADC1->CFGR &= ~ADC_CFGR_ALIGN;
    // Regular channel sequence length: 1 conversion (0000)
    ADC1->SQR1 &= ~ADC_SQR1_L;
    // Channel 6 assigned as 1st conversion in regular sequence (00110)
    ADC1->SQR1 &= ~ADC_SQR1_SQ1;
    ADC1->SQR1 |= ADC_SQR1_SQ1_2;
    ADC1->SQR1 |= ADC_SQR1_SQ1_1;
    // Channel 6 set to single ended mode (0)
    ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
    // Channel 6 sampling time set to 24.5 ADC clock cycles (011)
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP6_2;
    ADC1->SMPR1 |= ADC_SMPR1_SMP6_1;
    ADC1->SMPR1 |= ADC_SMPR1_SMP6_0;
    // ADC1 in single conversion mode (0)
    ADC1->CFGR &= ~ADC_CFGR_CONT;
    // Hardware trigger detection disabled (00)
    ADC1->CFGR &= ~ADC_CFGR_EXTEN;
    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    // Wait until ADC is ready
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
}
