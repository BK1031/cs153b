/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */
 
/*
 * Termite: USART2, DMA1 channel 7
 * Bluetooth: USART1, DMA1 channel 4
 */
 
#include "DMA.h"
#include "SysTimer.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef * DMA1_Channelx, USART_TypeDef * USARTx) {
	// Enable DMA1 clock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	// Wait 20 us for setup
	delay(1);
	
	// Disable DMA1 channel x
	DMA1_Channelx->CCR &= ~DMA_CCR_EN;
	// Disable Memory to memory mode (0)
	DMA1_Channelx->CCR &= ~DMA_CCR_MEM2MEM;
	// Priority level high (10)
	DMA1_Channelx->CCR |= DMA_CCR_PL_1;
	DMA1_Channelx->CCR &= ~DMA_CCR_PL_0;
	// Peripheral size 8-bits (00)
	DMA1_Channelx->CCR &= ~DMA_CCR_PSIZE_1;
	DMA1_Channelx->CCR &= ~DMA_CCR_PSIZE_0;
	// Memory size 8-bits (00)
	DMA1_Channelx->CCR &= ~DMA_CCR_MSIZE_1;
	DMA1_Channelx->CCR &= ~DMA_CCR_MSIZE_0;
	// Disable peripheral increment mode (0)
	DMA1_Channelx->CCR &= ~DMA_CCR_PINC;
	// Enable memory increment mode (1)
	DMA1_Channelx->CCR |= DMA_CCR_MINC;
	// Disable circular mode (0)
	DMA1_Channelx->CCR &= ~DMA_CCR_CIRC;
	// Data transfer direction memory-to-peripheral (1)
	DMA1_Channelx->CCR |= DMA_CCR_DIR;
	
	// Data destination: USARTx_TDR
	DMA1_Channelx->CPAR = (uint32_t)&(USARTx->TDR);
	
	// Disable half transfer interrupt (0)
	DMA1_Channelx->CCR &= ~DMA_CCR_HTIE;
	// Disable transfer error interrupt (0)
	DMA1_Channelx->CCR &= ~DMA_CCR_TEIE;
	// Enable transfer complete interrupt (1)
	DMA1_Channelx->CCR |= DMA_CCR_TCIE;
	
	if (DMA1_Channelx == DMA1_Channel7) {
		DMA1_CSELR->CSELR |= (1UL << 25); // Channel 7 mapped on USART2_TX
		NVIC_SetPriority(DMA1_Channel7_IRQn, 2); // Set NVIC interrupt priority
		NVIC_EnableIRQ(DMA1_Channel7_IRQn); // Enable NVIC interrupt
	} else if (DMA1_Channelx == DMA1_Channel4) {
		DMA1_CSELR->CSELR |= (1UL << 13); // Channel 4 mapped on USART1_TX
		NVIC_SetPriority(DMA1_Channel4_IRQn, 2); // Set NVIC interrupt priority
		NVIC_EnableIRQ(DMA1_Channel4_IRQn); // Enable NVIC interrupt
	}
}

void DMA1_Channel7_IRQHandler(void) {
	// NVIC interrupt flag clear
	NVIC_ClearPendingIRQ(DMA1_Channel7_IRQn);
		
	// Check if DMA1 channel 7 transfer complete
	if (DMA1->ISR & DMA_ISR_TCIF7) {
		DMA1->IFCR |= DMA_IFCR_CTCIF7; // Clear DMA1 channel 7 transfer complete flag
	}
	// Clear DMA1 channel 7 global interrupt flag
	DMA1->IFCR |= DMA_IFCR_CGIF7;
	
	// Disable DMA1 channel 7
	DMA1_Channel7->CCR &= ~DMA_CCR_EN;
}

void DMA1_Channel4_IRQHandler(void) {
	// NVIC interrupt flag clear
	NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
	
	// Check if DMA1 channel 4 transfer complete
	if (DMA1->ISR & DMA_ISR_TCIF4) {
		DMA1->IFCR |= DMA_IFCR_CTCIF4; // Clear DMA1 channel 7 transfer complete flag
	}
	// Clear DMA1 channel 4 global interrupt flag
	DMA1->IFCR |= DMA_IFCR_CGIF4;
	
	// Disable DMA1 channel 4
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
}
