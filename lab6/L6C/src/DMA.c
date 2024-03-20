/*
 * ECE 153B
 *
 * Name(s): 
 * Section: 
 * Lab: 6C
 */
 
#include "DMA.h"
#include "CRC.h"
#include "SysTimer.h"

void DMA_Init(void) {
	// Enable DMA1 clock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	// Wait 20 us for setup
	delay(1);
	
	// Disable DMA1 channel 6
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	// Enable Memory to memory mode (1)
	DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM;
	// Priority level high (10)
	DMA1_Channel6->CCR |= DMA_CCR_PL_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_PL_0;
	// Peripheral size 32-bits (10)
	DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE_0;
	// Memory size 32-bits (10)
	DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1;
	DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE_0;
	// Disable peripheral increment mode (0)
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
	// Enable memory increment mode (1)
	DMA1_Channel6->CCR |= DMA_CCR_MINC;
	// Disable circular mode (0)
	DMA1_Channel6->CCR &= ~DMA_CCR_CIRC;
	// Data transfer direction memory-to-peripheral (1)
	DMA1_Channel6->CCR |= DMA_CCR_DIR;
	
	// DMA1_Channel6->CNDTR = BUFFER_SIZE;
	
	// Data source: DataBuffer in "crc.h"
	DMA1_Channel6->CMAR = (uint32_t)DataBuffer;
	// Data destination: CRC_DR (Address offset: 0x00)
	DMA1_Channel6->CPAR = (uint32_t)&(CRC->DR);
	
	// Disable half transfer interrupt (0)
	DMA1_Channel6->CCR &= ~DMA_CCR_HTIE;
	// Disable transfer error interrupt (0)
	DMA1_Channel6->CCR &= ~DMA_CCR_TEIE;
	// Enable transfer complete interrupt (1)
	DMA1_Channel6->CCR |= DMA_CCR_TCIE;
	// Set NVIC interrupt priority to 0
	NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
	// Enable NVIC interrupt
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void DMA1_Channel6_IRQHandler(void){
	// NVIC interrupt flag clear
	NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
	
	// Check if DMA1 channel 6 transfer complete
	if (DMA1->ISR & DMA_ISR_TCIF6) {
		// Clear DMA1 channel 6 transfer complete flag
		DMA1->IFCR |= DMA_IFCR_CTCIF6;
		// Mark CRC computation as complete
		completeCRC(CRC->DR);
	}
	
	// Clear DMA1 channel 6 global interrupt flag
	DMA1->IFCR |= DMA_IFCR_CGIF6;
}
