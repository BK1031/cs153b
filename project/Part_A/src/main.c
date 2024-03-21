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

int main(void) {
	char ch;
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 

	Motor_Init();
	setDire(0);
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	
printf("Program Starts.\r\n");
	while(1) {
		printf("Enter 1 for clockwise, 2 for counterclockwise, or 3 to stop");
		scanf("%c", &ch);
		if (ch == '1') {
			setDire((int8_t) 1);
			printf("Rotating Clockwise\n");
		} else if (ch == '2') {
			setDire((int8_t) -1);
			printf("Rotating Counterclockwise\n");
		} else if (ch == '3') {
			setDire((int8_t) 0);
			printf("Stop Rotating\n");
		} else {
			printf("Invalid char\n");
		}
}
}

