/*
 * ECE 153B
 *
 * Name(s): Michael Wong, Bharat Kathi
 * Section: Tuesday, 7 PM
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Set_Alarm(void) {
    RTC->CR &= ~(RTC_CR_ALRBE);
    RTC->CR &= ~(RTC_CR_ALRAE);
    RTC->WPR |= (uint8_t)0xCA;
    RTC->WPR |= (uint8_t)0x53;
    RTC->CR &= ~(RTC_CR_ALRBE);
    RTC->CR &= ~(RTC_CR_ALRAE);
    RTC->CR &= ~(RTC_CR_ALRAIE);
    RTC->CR &= ~(RTC_CR_ALRBIE);
    
    while(RTC_ISR_ALRAWF == 0) {}
    
    RTC->ALRMAR = 3 << 4 | 0;
    RTC->ALRMAR &= ~(RTC_ALRMAR_MSK1);
    RTC->ALRMAR |= RTC_ALRMAR_MSK2;
    RTC->ALRMAR |= RTC_ALRMAR_MSK3;
    RTC->ALRMAR |= RTC_ALRMAR_MSK4;
    RTC->ALRMBR |= RTC_ALRMBR_MSK1;
    RTC->ALRMBR |= RTC_ALRMBR_MSK2;
    RTC->ALRMBR |= RTC_ALRMBR_MSK3;
    RTC->ALRMBR |= RTC_ALRMBR_MSK4;
    RTC->CR |= RTC_CR_ALRBE;
    RTC->CR |= RTC_CR_ALRAE;
    RTC->CR |= RTC_CR_ALRAIE;
    RTC->CR |= RTC_CR_ALRBIE;
    
    RTC->WPR |= (uint8_t)0x69;
}

void RTC_Alarm_Enable(void) {
    EXTI->RTSR1 |= EXTI_RTSR1_RT18;
    EXTI->IMR1 |= EXTI_IMR1_IM18;
    EXTI->EMR1 |= EXTI_EMR1_EM18;
    EXTI->PR1 |= EXTI_PR1_PIF18;

    NVIC_EnableIRQ(RTC_Alarm_IRQn);
    NVIC_SetPriority(RTC_Alarm_IRQn, 0);
}

void Enable_Alarm_A(void) {
    // Enable A, Disable Alarm B
    RTC->CR |= RTC_CR_ALRAE;
    RTC->CR &= ~RTC_CR_ALRBE;
}

void Enable_Alarm_B(void) {
    // Enable B, Disable Alarm A
    RTC->CR |= RTC_CR_ALRBE;
    RTC->CR &= ~RTC_CR_ALRAE;
}

void RTC_Alarm_IRQHandler(void) {

    RTC -> ISR &= ~RTC_ISR_ALRAF;
    EXTI -> PR1 |= EXTI_PR1_PIF18;
    RTC -> ISR &= ~RTC_ISR_ALRBF;
    Green_LED_Toggle();
}

int main(void) {    
    System_Clock_Init(); // Switch System Clock = 80 MHz
    
    LED_Init();
    
    RTC_Init();
    RTC_Alarm_Enable();
    RTC_Set_Alarm();
    
    char time[100] = "";
    char date[100] = "";

    while(1) {
        Get_RTC_Calendar(time, date);
    }
}