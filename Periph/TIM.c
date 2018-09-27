/*
 *   TIM.c
 *
 */
#include "stm32l011xx.h"
#include "TIM.h"
#include "stm32l0xx.h"

#define SYSCLK 16000000

/*
 * Periph - TIM2 : Output 3 Channels PWM
 *   @function TIM2_Init(uint16_t PSC)
 *   @connect  A-Phase < -Channel 1- > PA8    <D9>
 *             B-Phase < -Channel 2- > PB0    <D3>
 *             C-Phase < -Channel 3- > PA2    <A7>
 *   @param    Prescale
 *   @retvalue None
 */
void TIM2_Init(uint16_t PSC, uint16_t ARR)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     // ENABLE TIM2 Clock
	/* Time Base Init*/
	TIM2->PSC = PSC;                        // Set PreScale Value
	TIM2->ARR = ARR;                        // Set Auto Reload Value
	TIM2->CR1 &= ~(0x03 << 8);              // No Division
	TIM2->CR1 |= TIM_CR1_ARPE;              // ENABLE APRE
	TIM2->CR1 &= ~(TIM_CR1_DIR);            // Counter Up
	TIM2->CR1 &= ~(0x03 << 5);              // Edge Align Mode
	TIM2->CR1 &= ~(1 << 3);                 // Pulses Mode
	/* Output Compare Channel 1~3 Init */
	// Channel 1
	TIM2->CCMR1 &= ~(0x07);                 // CC1 Output Normal
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;         // Update Value ON Event
	TIM2->CCMR1 |= 0x60;                    // PWM Mode 1
	TIM2->CCER  &= ~TIM_CCER_CC1P;           // Active High
	// Channel 2
	TIM2->CCMR1 &= ~(0x07 << 8);            // CC2 Output Normal
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;         // Update when UEV
	TIM2->CCMR1 |= 0x6000;                  // PWM Mode 1
	TIM2->CCER  &= ~TIM_CCER_CC2P;           // Active High
	// Channel 3
	TIM2->CCMR2 &= ~(0x07);                 // CC3 Output Normal
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE;         // Update when UEV
	TIM2->CCMR2 |= 0x60;                    // PWM Mode 1
	TIM2->CCER  &= ~TIM_CCER_CC3P;         // Active High
	/* GPIO Init */
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;      // ENABLE GPIOA Clock
	GPIOA->MODER |= (1 << 17) | (1 << 5);   // PA2 PA8 AF
	GPIOA->MODER &= ~((1 << 16) | (1 << 4));
	GPIOA->OTYPER &= ~((1 << 2) | (1 << 8));// PA2 PA8 PP
	GPIOA->OSPEEDR |= (0x03 << 4) | (0x03 << 16);
	 	 	 	 	 	 	 	 	 	 	//PA2 PA8 High Speed
	GPIOA->PUPDR |= (1 << 16) | (1 << 4);
//	GPIOA->PUPDR &= ~((1 << 17) | (1 << 5));// PA2 PA8 Pull-Up
	GPIOA->AFR[0] |= 0x00000200;            // PA2 <--> TIM2_CH3
	GPIOA->AFR[1] |= 0x00000005;            // PA8 <--> TIM2_CH1
	RCC->IOPENR  |= RCC_IOPENR_GPIOBEN;     // ENABLE GPIOB Clock
	GPIOB->MODER  |= (1 << 1);
	GPIOB->MODER  &= ~0x01;                 // GPIOB0 AF
	GPIOB->OTYPER &= ~0x01;                 // GPIOB0 PP
	GPIOB->OSPEEDR|= 0x03;                  // GPIOB0 Very High
	GPIOB->PUPDR  |= 0x01;
//	GPIOB->PUPDR  &= ~(1 << 1);             // GPIOB0 Pull-Up
	GPIOB->AFR[0] |= 0x2;                   // GPIOB0 TIM2_CH2

	TIM2->CR1 |= 0x01;                      // ENABLE TIM2
}

void TIM2_PWM_Start(void)
{
	TIM2->CNT = 0x00;
	TIM2->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E);
}

void TIM2_PWM_Stop(void)
{
	TIM2->CNT = 0x00;
	TIM2->CCER &= ~( TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E );
}

/*
 * Periph - TIM21 : Input Capture PWM H-Time
 *   @function TIM21_Init(uint16_t PSC)
 *   @connect  IN < -Channel 1- > PA10
 *   @param    Prescale
 *   @retvalue None
 */
void TIM21_Init(uint16_t PSC)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM21EN;    // ENABLE TIM21 Clock
	TIM21->PSC = PSC;                       // Set PreScale Value
	TIM21->CR1;
}

