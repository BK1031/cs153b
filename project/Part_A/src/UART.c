#include "UART.h"

void UART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;
    RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART2_Init(void) {
    // (a) Enable the USART2 clock in the peripheral clock register.
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;      


    // (b) Select the system clock as the USART2 clock source in the peripheral independent clock configuration register.
    // RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
    // RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	RCC -> CFGR &= ~RCC_CFGR_MCOSEL;
	RCC -> CFGR |= RCC_CFGR_MCOSEL_0;
}

void UART1_GPIO_Init(void) {
    //Enable clock for GPIO port B
    // RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //OG!
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //friend's code

    // (0) set to alternative mode for both pins (10)
    GPIOB->MODER &= ~GPIO_MODER_MODE6;
    GPIOB->MODER |= GPIO_MODER_MODE6_1;
    GPIOB->MODER &= ~GPIO_MODER_MODE7;
    GPIOB->MODER |= GPIO_MODER_MODE7_1;

    // (02) set PB6 and PB7 to AF7 (USART1_TX and USART1_RX respectively)
    //AFR[0] bc pins 6,7 are in AFR[0] range of 0 to 7
    GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_3;

    //     (a) Both GPIO pins should operate at very high speed.
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;

    // (b) Both GPIO pins should have a push-pull output type.
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;

    // (c) Configure both GPIO pins to use pull-up resistors for I/O.
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD7;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0;
}

void UART2_GPIO_Init(void) {
	//PA2 and PA3 operate as UART transmitters and recievers
	//Enable clock for GPIO port A
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	// (0) set to alternative mode for both pins (10)
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3_0;

	// (02) set PA2 and PA3 to AF7 (USART2_TX and USART2_RX respectively)
	//AFR[0] bc pins 2,3 are in AFR[0] range of 0 to 7
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
	

	// 	(a) Both GPIO pins should operate at very high speed.
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;

	// (b) Both GPIO pins should have a push-pull output type.
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;

	// (c) Configure both GPIO pins to use pull-up resistors for I/O.
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3_1;
	
	}

void USART_Init(USART_TypeDef* USARTx) {
	//disable USART
	USARTx->CR1 &= ~USART_CR1_UE; 

	// (a) In the control registers, set word length to 8 bits, oversampling mode to oversample
	// by 16, and number of stop bits to 1.
	USARTx->CR1 &= ~USART_CR1_M1; 
	USARTx->CR1 &= ~USART_CR1_M0; 

	USARTx->CR1 &= ~USART_CR1_OVER8;
	USARTx->CR2 &= ~USART_CR2_STOP; 

	// (b) Set the baud rate to 9600. To generate the baud rate, you will have to write a value
	// into USARTx_BRR
	// USARTx->BRR = 8333; //og
	USARTx->BRR = 0x208D; //friends code

	// (c) In the control registers, enable both the transmitter and receiver.
	USARTx->CR1 |= USART_CR1_RE;
	USARTx->CR1 |= USART_CR1_TE;

	// (d) Now that everything has been set up, enable USART in the control registers.
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
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}