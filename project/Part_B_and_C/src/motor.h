#ifndef __STM32L476G_DISCOVERY_MOTOR_H
#define __STM32L476G_DISCOVERY_MOTOR_H

#include "stm32l476xx.h"

void Motor_Init(void);
void rotateStep(void);
void setDire(int8_t direction);

void openDoorDone(void);
void closeDoorDone(void);

extern volatile uint8_t doorState;
extern volatile uint8_t doorAccelerometerState;
extern volatile uint8_t doorBlocked;

#endif /* __STM32L476G_DISCOVERY_MOTOR_H */
