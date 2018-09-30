/*
 * Motor.c
 */

#include "stm32l011xx.h"
#include "Motor.h"
#include "COMP.h"
#include "BSP.h"

extern uint8_t Rotate_Direct;
extern uint8_t Current_State;

/*
 *   SD1 < -- > PB7   <D4>
 *   SD2 < -- > PA11  <D10>
 *   SD3 < -- > PA12  <D2>
 */

void delay(void)
{
	uint16_t i=0xfff;
	while(i--);
}

void GPIO_SDx_Init(void)
{
	/* GPIOB / GPIOC Init */
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;          // ENABLE GPIOA Clock
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;          // ENABLE GPIOB Clock
	GPIOB->MODER |= (1 << 14);
	GPIOB->MODER &= ~(1 << 15);                 // GPIOB7 Output
	GPIOB->OTYPER&= ~(1 << 7);                  // GPIOB7 PP
	GPIOB->PUPDR |= (1 << 15);
	GPIOB->PUPDR &= ~(1 <<14);                  // GPIOB7 Pull-Down
	A_PHASE_OFF;
	GPIOA->MODER |= (1 << 24) | (1 << 22);
	GPIOA->MODER &= ~((1 << 25) | (1 << 23));    // PA11 PA12 Output
	GPIOA->OTYPER &= ~((1 << 12) | (1 << 11));   // PA11 PA12 PP
	GPIOA->PUPDR |= (1 << 25) | (1 << 23);
	GPIOA->PUPDR &= ~((1 << 24) | (1 << 22));    // PA11 PA12 Pull-Down
	B_PHASE_OFF;
	C_PHASE_OFF;
}

void Bridge_A_B(void)
{
	COMP_OFF;
	A_PHASE_ON;
	PWM_A_ON;
	B_PHASE_LOW_ON;
	C_PHASE_OFF;
	COMP_C_PHASE;
	COMP_ON;
}

void Bridge_B_C(void)
{
	COMP_OFF;
	B_PHASE_ON;
	PWM_B_ON;
	C_PHASE_LOW_ON;
	A_PHASE_OFF;
	COMP_A_PHASE;
	COMP_ON;
}

void Bridge_C_A(void)
{
	COMP_OFF;
	C_PHASE_ON;
	PWM_C_ON;
	A_PHASE_LOW_ON;
	B_PHASE_OFF;
	COMP_B_PHASE;
	COMP_ON;
}

void Bridge_A_C(void)
{
	COMP_OFF;
	A_PHASE_ON;
	PWM_A_ON;
	C_PHASE_LOW_ON;
	B_PHASE_OFF;
	COMP_B_PHASE;
	COMP_ON;
}

void Bridge_C_B(void)
{
	COMP_OFF;
	C_PHASE_ON;
	PWM_C_ON;
	B_PHASE_LOW_ON;
	A_PHASE_OFF;
	COMP_A_PHASE;
	COMP_ON;
}

void Bridge_B_A(void)
{
	COMP_OFF;
	B_PHASE_ON;
	PWM_B_ON;
	A_PHASE_LOW_ON;
	C_PHASE_OFF;
	COMP_C_PHASE;
	COMP_ON;
}

// Change Phase
void Change_Phase(void)
{
	switch(Current_State)
	{
		case 1: Current_State = 2; Bridge_A_B(); break;
		case 2: Current_State = 3; Bridge_A_C(); break;
		case 3: Current_State = 4; Bridge_B_C(); break;
		case 4: Current_State = 5; Bridge_B_A(); break;
		case 5: Current_State = 6; Bridge_C_A(); break;
		case 6: Current_State = 1; Bridge_C_B(); break;
		default: SHUTDOWN; break;
	}
}

void ADC1_COMP_IRQHandler(void)
{
	/* Clear IT Flag*/
	EXTI->PR |= (1 << 21);
	/* Change ENABLE Phase */
	/* Rewrite Status Flag */
	LD3_TOGGLE;
	Change_Phase();
}
