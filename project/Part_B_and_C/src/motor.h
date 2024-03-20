#ifndef __STM32L476G_DISCOVERY_MOTOR_H
#define __STM32L476G_DISCOVERY_MOTOR_H

#include "stm32l476xx.h"

void Motor_Init(void);
void rotateStep(void);
void setDire(int8_t direction);
//void rotateDegrees(int32_t deg);

//uint32_t get_nSteps(void);
//uint8_t get_nStepsEnable(void);
//void set_nStepsEnable(uint8_t en);

uint8_t getDoorActual(void);
void openDoorDone(void);
void closeDoorDone(void);

#endif /* __STM32L476G_DISCOVERY_MOTOR_H */
