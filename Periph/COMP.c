/*
 *        COMPARE MODULE
 *     PA1  < -- >  MITTEL     <A1>
 *     PA0  < -- >  A-Phase    <A0>
 *     PA4  < -- >  B-Phase    <A3>
 *     PA5  < -- >  C-Phase    <A4>
 */

#include "COMP.h"
#include "stm32l011xx.h"
#include "stm32l0xx_hal_comp.h"

void COMP1_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->IOPENR   |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER  |= 0x00000f0f;         // PA0, PA1, PA4, PA5, Analog Mode

	EXTI->EMR  |= (1 << 21);
	EXTI->IMR  |= (1 << 21);
	EXTI->FTSR |= (1 << 21);

	COMP1->CSR &= ~((1 << 15) | (1 << 12) | (1 << 8));
	COMP_A_PHASE;

	NVIC_EnableIRQ(ADC1_COMP_IRQn);
	NVIC_SetPriority(ADC1_COMP_IRQn, 0);
	COMP1->CSR |= 0x01;
}


/*
 *  @Brief  Get Compare Result
 *  @Param  None
 *  @Ret    0 - Input < VRef
 *          1 - Input > VRef
 */
uint8_t COMP1_Get_Value(void)
{
	uint32_t tmp;
	tmp = COMP1->CSR & (0x40000000);
	if(tmp == 0)
		return 0;
	else
		return 1;
}


