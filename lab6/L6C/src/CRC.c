/*
 * ECE 153B
 *
 * Name(s): 
 * Section: 
 * Lab: 6C
 */
 
#include "CRC.h"

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed					
  * @retval 32-bit CRC
  */
uint32_t CRC_CalcBlockCRC(const uint32_t * pBuffer, uint32_t BufferLength) {
  // write all data into data register
	for (uint32_t i = 0; i < BufferLength; i++) {
		CRC->DR = pBuffer[i];
	}
	
  return CRC->DR;  // read CRC from data register
}	

void CRC_Init(void) {
	// Enable CRC clock 
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
	// Configure the control register to use 32 bit words (00)
	CRC->CR &= ~CRC_CR_POLYSIZE;
	// Default CRC Polynomial (CRC->POL): 0x04C1_1DB7
	CRC->POL = 0x04C11DB7;
	// Default initial CRC Value (CRC->INIT): 0xFFFF_FFFF
	CRC->INIT = 0xFFFFFFFF;
}

