/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 5A
 */


#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

uint32_t data = 0;
int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

		// PWM-controlled LED init
    LED_Pin_Init();
    TIM2_CH1_Init();

    while (1) {
        // Trigger ADC and get result
        // Start regular conversion
        ADC1->CR |= ADC_CR_ADSTART;
        // End of regular conversion of the master ADC
        while (!(ADC123_COMMON->CSR & ADC_CSR_EOC_MST));
        // Read ADC data
        data = ADC1->DR;
			
        // LED duty cycle proportional to ADC value (0-4096)
        if (data > 4095) data = 4095;
        TIM2->CCR1 = data;
    }
}