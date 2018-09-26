/*
 * Motor.c
 */

#include "stm32l011xx.h"
#include "Motor.h"
#include "COMP.h"

extern uint8_t Rotate_Direct;
extern uint8_t Current_State;

/*
 *   SD1 < -- > PB7
 *   SD2 < -- > PC14
 *   SD3 < -- > PC15
 */

void GPIO_SDx_Init(void)
{
	/* GPIOB / GPIOC Init */
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;          // ENABLE GPIOB Clock
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;          // ENABLE GPIOC Clock
	GPIOB->MODER |= (1 << 14);
	GPIOB->MODER &= ~(1 << 15);                 // GPIOB7 Output
	GPIOB->OTYPER&= ~(1 << 7);                  // GPIOB7 PP
	GPIOB->PUPDR |= (1 << 15);
	GPIOB->PUPDR &= ~(1 <<14);                  // GPIOB7 Pull-Down
	A_PHASE_OFF;
	GPIOC->MODER |= (1 << 30) | (1 << 28);
	GPIOC->MODER &= ~((1 << 31) | (1 << 29));   // PC14 PC15 Output
	GPIOC->OTYPER &= ~((1 << 14) | (1 << 15));   // PC14 PC15 PP
	GPIOC->PUPDR |= (1 << 31) | (1 << 29);
	GPIOC->PUPDR &= ~((1 << 30) | (1 << 28));    //PC14 PC15 Pull-Down
	B_PHASE_OFF;
	C_PHASE_OFF;
}

void Bridge_A_B(void)
{
	A_PHASE_ON;
	B_PHASE_LOW_ON;
	C_PHASE_OFF;
	COMP_C_PHASE;
}

void Bridge_B_C(void)
{
	B_PHASE_ON;
	C_PHASE_LOW_ON;
	A_PHASE_OFF;
	COMP_A_PHASE;
}

void Bridge_C_A(void)
{
	C_PHASE_ON;
	A_PHASE_LOW_ON;
	B_PHASE_OFF;
	COMP_B_PHASE;
}

void Bridge_A_C(void)
{
	A_PHASE_ON;
	C_PHASE_LOW_ON;
	B_PHASE_OFF;
	COMP_B_PHASE;
}

void Bridge_C_B(void)
{
	C_PHASE_ON;
	B_PHASE_LOW_ON;
	A_PHASE_OFF;
	COMP_A_PHASE;
}

void Bridge_B_A(void)
{
	B_PHASE_ON;
	A_PHASE_LOW_ON;
	C_PHASE_OFF;
	COMP_C_PHASE;
}

// Change Phase
void Change_Phase(void)
{
	switch(Current_State)
	{
		case 1: Current_State = 1; Bridge_C_B(); break;
		case 2: Current_State = 2; Bridge_C_A(); break;
		case 3: Current_State = 3; Bridge_B_A(); break;
		case 4: Current_State = 4; Bridge_B_C(); break;
		case 5: Current_State = 5; Bridge_A_C(); break;
		case 6: Current_State = 0; Bridge_A_B(); break;
		default: SHUTDOWN; break;
	}
}

void ADC1_COMP_IRQHandler(void)
{
	/* Change ENABLE Phase */
	/* Rewrite Status Flag */
}
