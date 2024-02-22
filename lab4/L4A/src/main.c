/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 4A
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(2);
	
	char rxByte;
	while(1) {
		scanf("%c", &rxByte);

		if (rxByte == "y" || rxByte == "Y") {
			Green_LED_On();
			printf("LED is on.");
		} else if (rxByte == "n" || rxByte == "N") {
			Green_LED_Off();
			printf("LED is off.");
		} else {
			printf("Invalid input. Please enter 'y' or 'n'.");
		}
	}
}
