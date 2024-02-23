#include "UART.h"

void UART1_Init(void) {
	// Enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// Set USART1 clock source to system clock (01)
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_1;
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART1_GPIO_Init(void) {
	// AF7 of PB6 is USART1_TX
	// AF7 of PB7 is USART1_RX
	
	// Enable GPIO B clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// Set PB6 mode to alternative function (10)
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE6_0;
	// Set PB7 mode to alternative function (10)
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE7_0;
	
	// Set PB6 alternative function to AF7 (0111) (USART1_TX)
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_0;
	// Set PB7 alternative function to AF7 (0111) (USART1_RX)
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_3;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_0;
	
	// Set PB6 I/O output speed to high speed (11)
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
	// Set PB7 I/O output speed to high speed (11)
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;
	
	// Set PB6 output type to push-pull (0)
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	// Set PB7 output type to push-pull (0)
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;
	
	// Set PB6 PUPD to pull-up (01)
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6_1;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;
	// Set PB7 PUPD to pull-up (01)
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD7_1;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0;
}

void UART2_Init(void) {
	// Enable USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	// Set USART2 clock source to system clock (01)
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_1;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART2_GPIO_Init(void) {
	// AF7 of PA2 is USART2_TX
	// AF7 of PA3 is USART2_RX
	
	// Set PA2 mode to alternative function (10)
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
	// Set PA3 mode to alternative function (10)
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3_0;
	
	// Set PA2 alternative function to AF7 (0111) (USART2_TX)
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
	// Set PA3 alternative function to AF7 (0111) (USART2_RX)
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
	
	// Set PA2 I/O output speed to high speed (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;
	// Set PA3 I/O output speed to high speed (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;
	
	// Set PA2 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	// Set PA3 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	
	// Set PA2 PUPD to pull-up (01)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	// Set PA3 PUPD to pull-up (01)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	// Disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
	
	// Set word length to 8 data bits (00)
	USARTx->CR1 &= ~USART_CR1_M;
	
	// Set oversampling mode to oversampling by 16 (0)
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	// Set STOP bits to 1 stop bit (00)
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	// Set baud rate to 9600
	USARTx->BRR = 0x208D; // USARTDIV = f_CK / baud rate = 80 Mhz / 9.6kBps = 0d8333 = 0x208D
	
	// Enable transmitter
	USARTx->CR1 |= USART_CR1_TE;
	
	// Enable receiver
	USARTx->CR1 |= USART_CR1_RE;
	
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300); // Increase from 300 to 40000 for bluetooth
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}