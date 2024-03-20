/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#ifndef __STM32L476R_NUCLEO_UART_H
#define __STM32L476R_NUCLEO_UART_H

#include "stm32l476xx.h"

#define IO_SIZE 256

void UART1_Init(void);
void UART2_Init(void);
void UART1_GPIO_Init(void);
void UART2_GPIO_Init(void);
void USART_Init(USART_TypeDef* USARTx);

void UART_print(char* str);
void DMA_USART_TX(void);

void Accumulate_USART_RX(char ch);
void UART_onInput(char* inputs, uint32_t size); // definition in "main.c"

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#endif
