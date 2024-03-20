#include "SPI.h"
#include "SysTimer.h"
#include "accelerometer.h"

void accWrite(uint8_t addr, uint8_t val) {
	uint16_t mosiData = 0x0000;
	
	mosiData = (uint16_t)(0 << 7UL) | (0 << 6UL) | addr;
	mosiData = (uint16_t)(mosiData << 8) | val;
	
	SPI_Transfer_Data(mosiData);
}

uint8_t accRead(uint8_t addr) {
	
	uint16_t mosiData = (1 << 7UL) | (uint16_t)(0 << 6UL) | (uint16_t)addr;
	mosiData = (uint16_t)(mosiData << 8);
	
	uint16_t readData = SPI_Transfer_Data(mosiData);
	return (uint8_t)(readData); // keep least significant byte from SPIx_DR
}

void initAcc(void) {
	// 100 Hz output data rate
	accWrite(0x2C, 0x0A);
	
	// set full range mode
	// Implicitly sets 2g range
	accWrite(0x31, 0x08);
	
	// enable measurement
	accWrite(0x2D, 0x08);
}

void readValues(double* x, double* y, double* z){
	// find scaler from data sheet (3.9 mg/LSB)
	// read values into x,y,z using accRead
	uint8_t DATAX0 = accRead(0x32);
	uint8_t DATAX1 = accRead(0x33);
	uint8_t DATAY0 = accRead(0x34);
	uint8_t DATAY1 = accRead(0x35);
	uint8_t DATAZ0 = accRead(0x36);
	uint8_t DATAZ1 = accRead(0x37);
	
	*x = ((short)(DATAX0 | (DATAX1 << 8))) * 0.004;
	*y = ((short)(DATAY0 | (DATAY1 << 8))) * 0.004;
	*z = ((short)(DATAZ0 | (DATAZ1 << 8))) * 0.004;
}
