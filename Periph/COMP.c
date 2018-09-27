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

void COMP1_IT_Init(void)
{
	EXTI->EMR  &= ~(1 << 21);
	EXTI->IMR  &= ~(1 << 21);
	EXTI->RTSR |= (1 << 21);
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
	NVIC_SetPriority(ADC1_COMP_IRQn, 0);
}

void COMP1_Init(void)
{
	COMP_HandleTypeDef hcomp1;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	COMP1_IT_Init();
	COMP1->CSR |= COMP_CSR_COMP1EN;      // ENABLE COMP1
	// GPIOA Init - Analog Mode
	RCC->IOPENR   |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER  |= 0x00000f0f;         // PA0, PA1, PA4, PA5, Input Mode
	GPIOA->PUPDR  &= 0xfffff0f0;         // PA0, PA1, PA4, PA5, Floating

	  hcomp1.Instance = COMP1;
	  hcomp1.Init.InvertingInput = COMP_INPUT_MINUS_DAC1_CH1;
	  hcomp1.Init.NonInvertingInput = COMP_INPUT_PLUS_IO1;
	  hcomp1.Init.LPTIMConnection = COMP_LPTIMCONNECTION_DISABLED;
	  hcomp1.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
	  hcomp1.Init.Mode = COMP_POWERMODE_ULTRALOWPOWER;
	  hcomp1.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
	  hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;

	  HAL_COMP_Init(&hcomp1);
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


