/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */
 
#include "CRC.h"

/**
  * @brief  Algorithm implementation of the CRC
  * @param  Crc: specifies the previous Crc data
  * @param  Data: specifies the input data
  * @retval Crc: the CRC result of the input data
  */

uint32_t CrcSoftwareFunc(uint32_t Initial_Crc, uint32_t Input_Data, uint32_t POLY)
{
  uint8_t bindex = 0;
  uint32_t Crc = 0;
	Crc = Initial_Crc ^ Input_Data;
  uint8_t MSB = Crc & 0x80000000; //1 at leftmost bit
  if (bindex < sizeof(Input_Data)) {
    If (MSB == 1) {
      Crc = (Crc << 1) ^ POLY;
    }
    Crc = Crc << 1;
    bindex += 1;
  }
  return Crc;
}

