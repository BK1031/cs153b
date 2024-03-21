/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */


#include "UART.h"
#include "DMA.h"
#include "string.h"

#define BUFFER_SIZE 32

static volatile DMA_Channel_TypeDef * DMA1_Channelx;
static volatile char inputs[IO_SIZE];
static volatile uint8_t input_size = 0;
static volatile uint8_t data_t[BUFFER_SIZE][IO_SIZE];
static volatile uint8_t * active;
static volatile uint8_t * pending[BUFFER_SIZE-1];
static volatile uint8_t pending_size[BUFFER_SIZE-1] = {0};

void UART1_Init(void) {
	// Enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// Set USART1 clock source to system clock (01)
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_1;
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
	
	// DMA init
	DMA1_Channelx = DMA1_Channel4;
	DMA_Init_UARTx(DMA1_Channelx, USART1);
	
	// Other UART inits
	UART1_GPIO_Init();
	USART_Init(USART1);
}

void UART2_Init(void) {
	// Enable USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	// Set USART2 clock source to system clock (01)
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_1;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	
	// DMA init
	DMA1_Channelx = DMA1_Channel7;
	DMA_Init_UARTx(DMA1_Channelx, USART2);
	
	// Other UART inits
	UART2_GPIO_Init();
	USART_Init(USART2);
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

void UART2_GPIO_Init(void) {
	// AF7 of PA2 is USART2_TX
	// AF7 of PA3 is USART2_RX
	
	// Enable GPIO A clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
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

void USART_Init(USART_TypeDef * USARTx) {
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
	
	// Enable DMA transmission
	USARTx->CR3 |= USART_CR3_DMAT;
	// Enable transmission complete (TC) interrupt
	USARTx->CR1 |= USART_CR1_TCIE;
	// Enable read data register not empty (RXNE) interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
	
	// Enable transmitter
	USARTx->CR1 |= USART_CR1_TE;
	// Enable receiver
	USARTx->CR1 |= USART_CR1_RE;
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
	
	if (USARTx == USART2) {
		NVIC_SetPriority(USART2_IRQn, 0);
		NVIC_EnableIRQ(USART2_IRQn);		
	} else if (USARTx == USART1) {
		NVIC_SetPriority(USART1_IRQn, 0);
		NVIC_EnableIRQ(USART1_IRQn);
	}
	
	// Initialize data buffer pointers
	active = data_t[0];
	for (uint8_t i = 0; i < BUFFER_SIZE-1; ++i) {
		pending[i] = data_t[i+1];
	}
}

void UART_print(char* str) {
	/*
	 * Transmit string through UART
	 */
	
	// Copy string to first available pending buffer
	for (uint8_t i = 0; i < BUFFER_SIZE-1; ++i) {
		if (pending_size[i] == 0) {
			strcpy(pending[i], str);
			pending_size[i] = strlen(pending[i]);
			break;
		}
		if (i == BUFFER_SIZE-2) { // if last buffer, then overwrite it no matter what
			strcpy(pending[i], str);
			pending_size[i] = strlen(pending[i]);
		}
	}
		
	// Start DMA transfer if none currently
	if (!(DMA1_Channelx->CCR & DMA_CCR_EN)) {
		DMA_USART_TX();
	}
	// Else, pending buffer will be DMA transferred when current USART transmission complete
}

void DMA_USART_TX(void) {
	/*
	 * Set pending buffer to active, then DMA transfer
	 * from active buffer to USARTx_TDR
	 */
	
	// Set pending[0] as active, shuffle pending[i+1] to pending[i], shuffle active to last pending
	// Set pending_size[0] as active_size, shuffle pending_size[i+1] to pending_size[i], set last pending_size to 0
	uint8_t* tmp = active;
	active = pending[0];
	uint8_t active_size = pending_size[0];
	for (uint8_t i = 0; i < BUFFER_SIZE-2; ++i) {
		pending[i] = pending[i+1];
		pending_size[i] = pending_size[i+1];
	}
	pending[BUFFER_SIZE-2] = tmp;
	pending_size[BUFFER_SIZE-2] = 0;
	
	// Setup DMA params and enable DMA transfer
	DMA1_Channelx->CMAR = (uint32_t)active;
	DMA1_Channelx->CNDTR = active_size;
	DMA1_Channelx->CCR |= DMA_CCR_EN;
}

void Accumulate_USART_RX(char ch) {
	/*
	 * Accumulate chars received by USARTx_RDR register
	 * USART reception complete when char is end-of-line
	 */
	
	// Append char to input buffer
	inputs[input_size] = ch;
	++input_size;
	
	// If char is end-of-line, then call UART_onInput() to act on input
	if (ch == '\n') {
		UART_onInput(inputs, input_size);
		input_size = 0;
	}
}

void USART1_IRQHandler(void){
	// For RX, RXNE = 1 if RDR register receives a char
	if (USART1->ISR & USART_ISR_RXNE) { // if RXNE = 1
		Accumulate_USART_RX((uint8_t)(USART1->RDR & 0xFF)); // read RDR register (auto clears RXNE)
	}
	
	// For TX, TC = 1 if DMA transfer complete and USART transmission complete
	if (USART1->ISR & USART_ISR_TC) { // if TC = 1
		USART1->ICR |= USART_ICR_TCCF; // clear TC
		if (pending_size[0] != 0) { // if first pending buffer nonempty
			DMA_USART_TX(); // start new DMA transfer
		}
	}
}

void USART2_IRQHandler(void){	
	// For RX, RXNE = 1 if RDR register receives a char
	if (USART2->ISR & USART_ISR_RXNE) { // if RXNE = 1
		Accumulate_USART_RX((uint8_t)(USART2->RDR & 0xFF)); // read RDR register (auto clears RXNE)
	}
	
	// For TX, TC = 1 if DMA transfer complete and USART transmission complete
	if (USART2->ISR & USART_ISR_TC) { // if TC = 1
		USART2->ICR |= USART_ICR_TCCF; // clear TC
		if (pending_size[0] != 0) { // if first pending buffer nonempty
			DMA_USART_TX(); // start new DMA transfer
		}
	}
}
