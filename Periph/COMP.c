/*
 *        COMPARE MODULE
 *     PA1  < -- >  MITTEL
 *     PA0  < -- >  A-Phase
 *     PA4  < -- >  B-Phase
 *     PA5  < -- >  C-Phase
 */

#include "COMP.h"
#include "stm32l011xx.h"

void COMP1_IT_Init(void)
{
	NVIC->ISER[0] |= (1 << 12);          // ENABLE EXTI_COMP
	NVIC->IP[12]  = 0x40;                // 1
}

void COMP1_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	COMP1_IT_Init();
	COMP1->CSR |= COMP_CSR_COMP1EN;      // ENABLE COMP1
	// GPIOA Init - Analog Mode
	RCC->IOPENR   |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER  |= 0x00000f0f;         // PA0, PA1, PA4, PA5, Input Mode
	GPIOA->PUPDR  &= 0xfffff0f0;         // PA0, PA1, PA4, PA5, Floating
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

