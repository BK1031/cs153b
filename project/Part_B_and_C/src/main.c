/*
 * ECE 153B
 *
 * Name(s): 
 * Section: 
 * Project
 */

/* Pin connections:
 * PA5: LED
 * PC5: Stepper motor A
 * PC6: Stepper motor ~A
 * PC8: Stepper motor B
 * PC9: Stepper motor ~B
 * PA2: USART2_TX
 * PA3: USART2_RX
 * PB6: USART1_TX
 * PB7: USART1_RX
 * PB8: I2C1_SCL
 * PB9: I2C1_SDA
 * PA4: SPI1_NSS
 * PB3: SPI1_SCK
 * PB4: SPI1_MISO
 * PB5: SPI1_MOSI
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "DMA.h"
#include "UART.h"
#include "motor.h"
#include "SPI.h"
#include "I2C.h"
#include "accelerometer.h"
#include <stdio.h>
#include "string.h"

#define TEMP_HIGH_THRESHOLD 27 // Celsius
#define TEMP_LOW_THRESHOLD 23 // Celsius

static char buffer[IO_SIZE];
static volatile uint8_t prevTemperature;
static volatile	uint8_t currTemperature;

volatile uint8_t doorState = 0; // 0 for closed, 1 for open, 2 for in between
volatile uint8_t = 0; // 0 for closed, 1 for open, 2 for in between
volatile bool doorBlocked = 0; // 1 if door is blocked, 0 otherwise

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	Motor_Init();
	SysTick_Init();
	UART1_Init(); // UART2 for Termite, UART1 for Bluetooth
	SPI1_GPIO_Init();
	SPI1_Init();
	initAcc();
	I2C_GPIO_Init();
	I2C_Initialization();
	
	const uint8_t TC74_Address = 0b1001000 << 1; // "<< 1" is because bit 0 is treated as don't care in 7-bit addressing mode
	const uint8_t CommandByte = 0x00; // Command byte for Read Temperature in TC74
	double x, y, z;
	
	sprintf(buffer, "Program start...\r\n");
	UART_print(buffer);

	while(1) {		
		// Idle LED
		LED_Toggle();
				
		// Ping TC74 temperature sensor
		prevTemperature = currTemperature;
		I2C_SendData(I2C1, TC74_Address, &CommandByte, 1);
		I2C_ReceiveData(I2C1, TC74_Address, &currTemperature, 1);
		// Print temperature if it changed
		if (currTemperature != prevTemperature) {
			sprintf(buffer, "Temperature: %d C\n", currTemperature);
			UART_print(buffer);
		}
		
		// Ping accelerometer, update doorActual
		readValues(&x, &y, &z);
		if (-1.25 <= y && y <= -0.75 && -0.25 <= z && z <= 0.25) { 
			doorAccelerometerState = 0;
		} else if (-0.25 <= y && y <= 0.25 && 0.75 <= z && z <= 1.25) { 
			doorAccelerometerState = 1;
		} else {
			doorAccelerometerState = 2;
		}
		sprintf(buffer, "doorAccelerometerState: %d\n", doorAccelerometerState);
		UART_print(buffer);
		sprintf(buffer, "Acceleration: %.2f, %.2f, %.2f\r\n", x, y, z);
		UART_print(buffer);

		if (!doorBlocked && doorState == 0 && currTemperature >= TEMP_HIGH_THRESHOLD) {
			openDoor(0);
		} else if (!doorBlocked && doorState == 1 && currTemperature <= TEMP_LOW_THRESHOLD) {
			closeDoor(0);
		}

		// Some delay
		delay(100);
	}
}

void openDoor(bool consoleOverride) {
	doorState = 1;
	if (!consoleOverride) {
		sprintf(buffer, "Temperature too high! Opening door...\n");
		UART_print(buffer);
	} else if{
		sprintf(buffer, "Console override! Opening door...\n");
		UART_print(buffer);
	}	
	setDire(1);
}

void closeDoor(bool consoleOverride) {
	doorState = 0;
	if (!consoleOverride) {
		sprintf(buffer, "Temperature too low! Closing door...\n");
		UART_print(buffer);
	} else {
		sprintf(buffer, "Console override! Closing door...\n");
		UART_print(buffer);
	}
	setDire(-1);
}

void UART_onInput(char* inputs, uint32_t size) {
	sprintf(buffer, "Your input: %s", inputs);
	UART_print(buffer);
	
	if (!strcmp(inputs, "open\n")) { // Command is "/open"
		doorBlocked = 1;
		openDoor(1);
	} else if (!strcmp(inputs, "close\n")) { // Command is "/close"
		doorBlocked = 1;
		closeDoor(1);
	} else if (!strcmp(inputs, "high\n")) { // Command is "/temp high"
		currTemperature = TEMP_HIGH_THRESHOLD;
		sprintf(buffer, "Temperature set to %d\n", currTemperature);
		UART_print(buffer);
	} else if (!strcmp(inputs, "low\n")) { // Command is "/temp low"
		currTemperature = TEMP_LOW_THRESHOLD;
		sprintf(buffer, "Temperature set to %d\n", currTemperature);
		UART_print(buffer);
	}	else {
		sprintf(buffer, "Invalid input - Try again\n");
		UART_print(buffer);
	}
	
	// Clear input buffer
	memset(inputs, 0, size);
}

void openDoorDone(void) {
	sprintf(buffer, "Door is now open\n");
	UART_print(buffer);
	wait(3000); // clear cmdOverride after 3s
}

void closeDoorDone(void) {
	sprintf(buffer, "Door is now closed\n");
	UART_print(buffer);
	wait(3000); // clear cmdOverride after 3s
}
