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
  uint32_t index = 0;
  // write all data into data register
  for (index = 0; index < BufferLength; index++) {
    CRC->DR = pBuffer[index];
  }
  // read CRC from data register
  return CRC->DR;
}	

void CRC_Init(void) {
	// Enable CRC clock 
  RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
  // Configure the control register to use 32 bit words
  CRC->CR = CRC_CR_RESET;
	// Load the polynomial into the CRC polynomial register
  CRC->POL = 0x04C11DB7;
  // Load the initial value into the initial CRC value register
  CRC->INIT = 0xFFFFFFFF;
}

