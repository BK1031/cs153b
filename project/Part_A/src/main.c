/*
 * ECE 153B
 *
 * Name(s): 
 * Section: 
 * Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include "motor.h"
#include <stdio.h>

void Init_USARTx(int x);


int main(void) {
	char ch;
	// Switch System Clock = 80 MHz
	System_Clock_Init();
	SysTick_Init();
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
	Motor_Init();
	
	printf("Program start...\n");
	printf("L for ccw, R for cw, 0 for stop\r\n");
	while(1) {
		scanf("%c", &ch);
		// printf("Your input: %c\n", rxByte);
		
		if ((ch == 'l') || (ch == 'L')) {
			setDire(-1);
			printf("Motor status: ccw\n");
		} else if ((ch == 'r') || (ch == 'R')) {
			setDire(1);
			printf("Motor status: cw\n");
		} else if (ch == '0') {
			setDire(0);
			printf("Motor status: stopped\n");
		} else {
			printf("Invalid command - try again\n");
		}
	}
}