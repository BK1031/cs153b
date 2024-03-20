#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
#define uwExpectedCRCValue 0x5A60861E

static volatile uint32_t Software_ComputedCRC;

int main(void) {
	
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	// initialize modules
	SysTick_Init();
	LED_Init();
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
	
	while(1) {
		LED_Toggle();
		// initialize CRC
		Software_ComputedCRC = INITIAL_CRC_VALUE;
		// start timer
		startTimer();
		// compute CRC
		for (int i = 0; i < BUFFER_SIZE; i++) {
			Software_ComputedCRC = CrcSoftwareFunc(Software_ComputedCRC, DataBuffer[i], POLYNOME);
		}		
		// end timer
		uint32_t timespan = endTimer();
		// check 
		if (Software_ComputedCRC != uwExpectedCRCValue) {
			LED_Off();
			printf("Program stop...CRC error\n");
			break;
		}
		// print time
		printf("Execution time: %d us\n", timespan);
		// delay
		delay(1000); // 1 sec
	}
}