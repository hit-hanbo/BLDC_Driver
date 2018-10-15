#include "BSP.h"

/* @Brief: Init SysTick Timer
 * @Param: LOAD - Int Time Interval
 * @Retur: None
 * */
void SysTick_Init(uint32_t LOAD)
{
	SysTick->VAL  &= 0x00000000;       //  Reset Value
	SysTick->LOAD  = LOAD;             //  SysTick Freq
    SysTick->CTRL |= 0x00000005;       //  SysTick DISABLE IRQ, CLK = SYS_CLK / 8
}

/* @Brief: Init BSP LD3
 * @Resou: GPIOB3 <--> D13/LD3
 * @Param: None
 * @Retur: None
 * */
void LED_Init(void)
{
	RCC->IOPENR   |= RCC_IOPENR_GPIOBEN;// ENABLE GPIOB Clock
	GPIOB->MODER  |= (1 << 6);
	GPIOB->MODER  &= ~(1 << 7);         // LD3 Output
	GPIOB->OTYPER &= ~(1 << 3);         // LD3 PP
	GPIOB->PUPDR  |= (1 << 6);
	GPIOB->PUPDR  &= ~(1 << 7);         // Pull-up
}

/*
 *        COMPARE MODULE
 *     PA1  < -- >  MITTEL     <A1>
 *     PA0  < -- >  A-Phase    <A0>
 *     PA4  < -- >  B-Phase    <A3>
 *     PA5  < -- >  C-Phase    <A4>
 */

/* @Brief: Init COMP1
 * @Resou:
 * @Param: None
 * @Retur: None
 * */
void COMP1_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->IOPENR   |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER  |= 0x00000f0f;         // PA0, PA1, PA4, PA5, Analog Mode
	EXTI->IMR  |= (1 << 21);             // COMP1 <--> EXTI21 Int Enable
	EXTI->FTSR |= (1 << 21);             // EXTI21 Falling Edge Int
	COMP1->CSR &= ~((1 << 15) | (1 << 12) | (1 << 8));
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
	NVIC_SetPriority(ADC1_COMP_IRQn, 0); // NVIC Enable IRQn Priority(0,0)
	COMP1->CSR |= 0x01;                  // ENABLE COMP1
}

/* @Brief: Get Compare Value
 * @Resou: COMP1_VAL COMP1->CSR[30:30]
 * @Param: None
 * @Retur:    0 - Input < Vref
 * 			  1 - Input > Vref
 * */
uint8_t COMP1_Get_Value(void)
{
	uint32_t tmp, INV;
	tmp = COMP1->CSR & (0x40000000);
	INV = COMP1->CSR & COMP_CSR_COMP1POLARITY;
	if(tmp == 0)
	{
		if(INV) return 1;
		else    return 0;
	}
	else
	{
		if(INV) return 0;
		else    return 1;
	}
}

/*
 * TIM MODULE
 * Periph - TIM2 : Output 3 Channels PWM
 *     A-Phase < -Channel 1- > PA8    <D9>
 *     B-Phase < -Channel 2- > PB0    <D3>
 *     C-Phase < -Channel 3- > PA2    <A7>
 */
/* @Brief: Init TIM2
 * @Resou:
 * @Param:    PSC - PreScale Value
 * 			  ARR - AutoReload Value
 * @Retur: None
 * */
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
	GPIOA->AFR[0] |= 0x00000200;            // PA2 <--> TIM2_CH3
	GPIOA->AFR[1] |= 0x00000005;            // PA8 <--> TIM2_CH1
	RCC->IOPENR  |= RCC_IOPENR_GPIOBEN;     // ENABLE GPIOB Clock
	GPIOB->MODER  |= (1 << 1);
	GPIOB->MODER  &= ~0x01;                 // GPIOB0 AF
	GPIOB->OTYPER &= ~0x01;                 // GPIOB0 PP
	GPIOB->OSPEEDR|= 0x03;                  // GPIOB0 Very High
	GPIOB->PUPDR  |= 0x01;
	GPIOB->AFR[0] |= 0x2;                   // GPIOB0 TIM2_CH2
	TIM2->CR1 |= 0x01;                      // ENABLE TIM2_Base
}
/* @Brief: ENABLE or DISABLE TIM2_ PWM_CHANNEL 1~3
 * @Resou:
 * @Param: None
 * @Retur: None
 * */
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

/* @Brief: Delay
 * @Resou:
 * @Param: None
 * @Retur: None
 * */
void Delay_ms(uint16_t nms)
{
	uint16_t tmp   = 2000*nms;
	SysTick->CTRL &= ~(0x01);
	SysTick->LOAD  = tmp;
	SysTick->VAL  &= 0x000000;
	SysTick->CTRL |= 0x01;
	while(SysTick->VAL);
}

void Delay_us(uint16_t nus)
{
	uint16_t tmp   = 2*nus;
	SysTick->CTRL &= ~(0x01);
	SysTick->LOAD  = tmp;
	SysTick->VAL  &= 0x000000;
	SysTick->CTRL |= 0x01;
	while(SysTick->VAL);
}
