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
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	printf("Program Starts.\r\n");
	char rxByte = '\0';
	while(1) {
		scanf("%c", &rxByte);
        if (rxByte == '1') {
            setDire(1);
			rotate();
            printf("Clockwise");
        } else if (rxByte == '2') {
            setDire(2);
			rotate();
            printf("CounterClockwise");
        } else {
			setDire(0);
            printf("Stopped");
		}
}
}