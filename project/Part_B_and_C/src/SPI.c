#include "SPI.h"
#include "SysTimer.h"

void SPI1_GPIO_Init(void) {
	// Enable the GPIO Clock
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN);
	// Set PA4, PB3, PB4, and PB5 to Alternative Functions, and configure their AFR to SPI1
	GPIOA->MODER &=~(GPIO_MODER_MODE4);		// clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE4_1); 	// alt func mode <10>
	GPIOA->AFR[0]&=~(GPIO_AFRL_AFSEL4);		// set af <0000>
	GPIOA->AFR[0]|= (GPIO_AFRL_AFSEL4_0 | GPIO_AFRL_AFSEL4_2);	// set af <0101>

	GPIOB->MODER &=~(GPIO_MODER_MODE3   | GPIO_MODER_MODE4   | GPIO_MODER_MODE5);	// clear mode bits
	GPIOB->MODER |= (GPIO_MODER_MODE3_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1); // alt func mode <10>
	GPIOB->AFR[0]&=~(GPIO_AFRL_AFSEL3   | GPIO_AFRL_AFSEL4   | GPIO_AFRL_AFSEL5);	// set af <0000>
	GPIOB->AFR[0]|= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_2 | GPIO_AFRL_AFSEL4_0 
				| GPIO_AFRL_AFSEL4_2 | GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2);	// set af <0101>
	// Set GPIO Pins to: Very High Output speed <11>, Output Type Push-Pull <0>, and No Pull-Up/Down
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED4);												// set ospeed bits <11>
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3 | GPIO_OSPEEDR_OSPEED4 | GPIO_OSPEEDR_OSPEED4);	// set ospeed bits <11>
	//GPIOA->OTYPER  &=~(GPIO_OTYPER_OT4);													// set otype bit <0>
	//GPIOB->OTYPER  &=~(GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5);				// set otype bit <0>
	GPIOA->PUPDR   &=~(GPIO_PUPDR_PUPD4); 													// clear PUPD bits <00>
	GPIOB->PUPDR   &=~(GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD4 | GPIO_PUPDR_PUPD5); 			// clear PUPD bits <00>
}

void SPI1_Init(void){
	// Enable SPI clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	// Reset SPI
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	
	// Disable SPI
	SPI1->CR1 &= ~SPI_CR1_SPE;
	
	// Configure for Full Duplex Communication (0)
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	
	// Configure for 2-line Unidirectional Data Mode (0)
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
	
	// Disable Output in Bidirectional Mode
	SPI1->CR1 &= ~SPI_CR1_BIDIOE;
	
	// Set Frame Format: MSB First, 16-bit, Motorola Mode
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1->CR2 |= SPI_CR2_DS;
	SPI1->CR2 &= ~SPI_CR2_FRF;
	
	// Configure Clock. Read DataSheet for required value
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	
	// Set Baud Rate Prescaler to 16 (011)
	SPI1->CR1 &= ~SPI_CR1_BR_2;
	SPI1->CR1 |= SPI_CR1_BR_1;
	SPI1->CR1 |= SPI_CR1_BR_0;
	
	// Disable Hardware CRC Calculation
	SPI1->CR1 &= ~SPI_CR1_CRCEN;
	
	// Set as Master (1)
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	// Disable Software Slave Management (0)
	SPI1->CR1 &= ~SPI_CR1_SSM;
	
	// Enable NSS Pulse Management (1)
	SPI1->CR2 |= SPI_CR2_NSSP;
	
	// Enable Output
	SPI1->CR2 |= SPI_CR2_SSOE;
	
	// Set FIFO Reception Threshold to 1/2 (0)
	SPI1->CR2 &= ~SPI_CR2_FRXTH;
	
	// Enable SPI
	SPI1->CR1 |= SPI_CR1_SPE;
}


uint16_t SPI_Transfer_Data(uint16_t write_data){
	// Wait for TXE (Transmit buffer empty)
	while ((SPI1->SR & SPI_SR_TXE) == 0);
	
	// Write data
	SPI1->DR = write_data;
	
	// Wait for not busy
	while (SPI1->SR & SPI_SR_BSY);
	
	// Wait for RXNE (Receive buffer not empty)
	while ((SPI1->SR & SPI_SR_RXNE) == 0);
	
	// Return data
	return SPI1->DR;
}
