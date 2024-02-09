/*
 * ECE 153B
 * 
 * Name(s):
 * Section:
 * Lab: 3B
 */

#include "stm32l476xx.h"

void LED_Pin_Init(void);
void TIM2_CH1_Init(void);
void SERVO_Pin_Init(void);
void TIM5_CH1_Init(void);

void LED_Pin_Init(void){
	// Enable GPIO Clocks
  	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// GPIO Mode
	// Alternative Function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
	GPIOA->MODER |= GPIO_MODER_MODE5_1; 

	//Set I/O output speed value as very high speed (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;

	// GPIO Push-Pull: No pull-up, no pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_1;

	//configure an select the alt f'n for PA5 (0001 for AF1 - timer 2 channel 1 af)
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5; //reset
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0; //set to 0001
}
	

void TIM2_CH1_Init(void){
	// Enable the timer clock
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR; //bit set to 0

	// Prescaler
	TIM2->PSC = 39;
	TIM2->ARR = 1999;

	// clear output compare mode bits and set bits to PWM mode 1 (0110)
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M; //clear output compare mode bits
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
	// enable output preload
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	// Enable output for ch1
	TIM2->CCER |= TIM_CCER_CC1P; // Output Compare Register for channel 1 
	TIM2->CCER |= TIM_CCER_CC1E; //enable  channel 1 output

	TIM2->CCR1 = 0; //set cc value s.t. duty cycle of PWM output = 0.5
	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN;
}

void SERVO_Pin_Init(void){
	// Set mode as Alternative Function 1 (COMMENT: SET TO AF2 bc that's for timer 5 ch 1)
	GPIOA->MODER |= GPIO_MODER_MODE0_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE0_0;

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0; //reset all AFSEL0 bits
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1; //set to AF2 0010

	//Set I/O output speed value as very high speed (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_1;

	// GPIO Push-Pull: No pull-up, no pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_1;
}

void TIM5_CH1_Init(void){
	//Enable timer clock 
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
    
	//up-counting
	TIM5->CR1 &= ~(TIM_CR1_DIR);

	//prescaler and auto reload reg
    TIM5->PSC = 39;
    TIM5->ARR = 1999;

	//PWM mode 1
    TIM5->CCMR1 &= ~(TIM_CCMR1_OC1M); //reset oc1m bits
    TIM5->CCMR1 |= TIM_CCMR1_OC1M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC1M_1;

	//output 1 preload enable
    TIM5->CCMR1 |= TIM_CCMR1_OC1PE;

	//output polarity active high, ch1 output enable
    TIM5->CCER &= ~TIM_CCER_CC1P; //og enabled, now reset
    TIM5->CCER |= TIM_CCER_CC1E;
	
	//ccr1 value and enable counter
    TIM5->CCR1 = 100;
    TIM5->CR1 |= TIM_CR1_CEN;
}

int main(void) {
	int i;
	int led = 0; //allan 100
	int dire = 1;
	int motorCCR = 150; //allan 0
	int servo = 0 ;
	
	LED_Pin_Init();
	SERVO_Pin_Init();
	TIM2_CH1_Init();
	TIM5_CH1_Init();
		
    while(1) {
		// TODO
		TIM2->CCR1 = led; //allan 0
		TIM5->CCR1 = servo;//motor
		//increment led 
		//if led reaches ARR value, it reset
		if (led >= 1999) {
			dire = -1;
			for (int i = 0; i < 100; i++);
		} else if (led <= 0) {
			dire = 1;
			for (int i = 0; i < 100; i++);
		}
		led += dire;
		servo = 0.02901 * led + 120;
		// servo = (led / 2000) * 100 + 100;
		
		for(i=0;i<300;i++);  		// delay
    }
}