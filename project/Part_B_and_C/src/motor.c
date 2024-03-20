/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "stm32l476xx.h"
#include "motor.h"
#include "math.h" // for round()
#include "stdlib.h" // for abs()

static const uint32_t MASK = GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9;
static const uint32_t HalfStep[8] = {
	GPIO_ODR_OD5 | GPIO_ODR_OD9, // 1001
	GPIO_ODR_OD5, // 1000
	GPIO_ODR_OD5 | GPIO_ODR_OD8, // 1010
	GPIO_ODR_OD8, // 0010
	GPIO_ODR_OD6 | GPIO_ODR_OD8, // 0110
	GPIO_ODR_OD6, // 0100
	GPIO_ODR_OD6 | GPIO_ODR_OD9, // 0101
	GPIO_ODR_OD9, // 0001
};

static volatile int8_t dire = 0; // -1 for ccw, 0 for stop, 1 for cw
static volatile uint8_t stepIndex = 0;
//static volatile uint32_t nSteps = 0; // counter for rotateDegrees()
//static volatile uint8_t nStepsEnable = 0; // counter enable for rotateDegrees()

/*****************************************************************************/
/*                             Initializations                               */
/*****************************************************************************/

void Motor_Init(void) {
	// PC5: A
	// PC6: ~A
	// PC8: B
	// PC9: ~B
	
	// Enable GPIO Clock of Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	// Set mode of PC5 to output (01)
	GPIOC->MODER &= ~GPIO_MODER_MODE5;
	GPIOC->MODER |= GPIO_MODER_MODE5_0;
	// Set mode of PC6 to output (01)
	GPIOC->MODER &= ~GPIO_MODER_MODE6;
	GPIOC->MODER |= GPIO_MODER_MODE6_0;
	// Set mode of PC8 to output (01)
	GPIOC->MODER &= ~GPIO_MODER_MODE8;
	GPIOC->MODER |= GPIO_MODER_MODE8_0;
	// Set mode of PC9 to output (01)
	GPIOC->MODER &= ~GPIO_MODER_MODE9;
	GPIOC->MODER |= GPIO_MODER_MODE9_0;
	
	// Set output speed of PC5 to high speed (10)
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;
	// Set output speed of PC6 to high speed (10)
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;
	// Set output speed of PC8 to high speed (10)
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1;
	// Set output speed of PC9 to high speed (10)
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1;
	
	// Set output type of PC5 to push-pull (0)
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT5;
	// Set output type of PC6 to push-pull (0)
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT6;
	// Set output type of PC8 to push-pull (0)
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT8;
	// Set output type of PC9 to push-pull (0)
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT9;
	
	// Set PUPD of PC5 to no pull-up/pull-down (00)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD5;
	// Set PUPD of PC6 to no pull-up/pull-down (00)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD6;
	// Set PUPD of PC8 to no pull-up/pull-down (00)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD8;
	// Set PUPD of PC9 to no pull-up/pull-down (00)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD9;
}

/*****************************************************************************/
/*                         Motor rotate functions                            */
/*****************************************************************************/

void rotateStep(void) {
	/*
	 * Perform one half step in stepper motor
	 * This function is called in SysTick interrupt
	 */
	
	if (dire == -1) { // ccw, closing door
		GPIOC->ODR &= ~MASK;
		GPIOC->ODR |= HalfStep[stepIndex];
		if (stepIndex == 0) stepIndex = 7;
		else --stepIndex;
		if (getDoorActual() == 1) { // if door closed
			setDire(0);
			closeDoorDone();
		}
	} else if (dire == 1) { // cw, opening door
		GPIOC->ODR &= ~MASK;
		GPIOC->ODR |= HalfStep[stepIndex];
		if (stepIndex == 7) stepIndex = 0;
		else ++stepIndex;
		if (getDoorActual() == 0) { // if door open
			setDire(0);
			openDoorDone();
		}
	}
	
	// Step counter for rotateDegrees()
//	if (nStepsEnable && dire != 0) {
//		--nSteps;
//	}
}

void setDire(int8_t direction) {
	/*
	 * Use this function for continuous motor rotation
	 * -1 for ccw, 0 for stop, 1 for cw
	 */
	
	dire = direction;
}

//void rotateDegrees(int32_t deg) {
//	/*
//	 * Use this function to rotate motor by deg degrees
//	 * Positive value for cw, negative value for ccw, 0 for no rotation
//	 */
//	
//	if (deg == 0) return;
//	
//	nSteps = round(4096.0 * abs(deg) / 360);
//	nStepsEnable = 1;
//	setDire((deg > 0) - (deg < 0)); // auto extract sign from deg
//}

/*****************************************************************************/
/*                Variable getters/setters for rotateDegrees()               */
/*****************************************************************************/

//uint32_t get_nSteps(void) {
//	return nSteps;
//}

//uint8_t get_nStepsEnable(void) {
//	return nStepsEnable;
//}

//void set_nStepsEnable(uint8_t en) {
//	nStepsEnable = en;
//}
