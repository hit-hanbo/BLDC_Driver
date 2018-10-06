/*
 * Motor.c
 */

#include "stm32l011xx.h"
#include "Motor.h"
#include "COMP.h"
#include "BSP.h"

extern uint8_t Rotate_Direct;
extern uint8_t Current_State;
uint32_t Counter_Changing;
/*
 *   SD1 < -- > PB7   <D4>
 *   SD2 < -- > PA11  <D10>
 *   SD3 < -- > PA12  <D2>
 */

void delay(void)
{
	uint16_t i=0xffe;
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
	uint8_t sensor;
	do
	{
		if(COMP1_Get_Value() == 1) sensor = 1;
		else sensor = 0;

		switch(Current_State)
		{
			case 1:
				A_PHASE_ON; PWM_A_ON;
				if(sensor == 1)
				{
					LD3_TOGGLE;
					C_PHASE_LOW_ON; B_PHASE_OFF;
					COMP_B_PHASE;
					COMP_FALLING_INT;
					Current_State = 2;
					Counter_Changing ++;
				}
				else
					B_PHASE_LOW_ON;
				break;
			case 2:
				C_PHASE_LOW_ON;
				if(sensor == 0)
				{
					LD3_TOGGLE;
					B_PHASE_ON; PWM_B_ON; A_PHASE_OFF;
					COMP_A_PHASE;
					COMP_RISING_INT;
					Current_State = 3;
					Counter_Changing ++;
				}
				else
					A_PHASE_ON; PWM_A_ON;
				break;
			case 3:
				B_PHASE_ON; PWM_B_ON;
				if(sensor == 1)
				{
					LD3_TOGGLE;
					A_PHASE_LOW_ON; C_PHASE_OFF;
					COMP_C_PHASE;
					COMP_FALLING_INT;
					Current_State = 4;
					Counter_Changing ++;
				}
				else
					C_PHASE_LOW_ON;
				break;
			case 4:
				A_PHASE_LOW_ON;
				if(sensor == 0)
				{
					LD3_TOGGLE;
					C_PHASE_ON; PWM_C_ON; B_PHASE_OFF;
					COMP_B_PHASE;
					COMP_RISING_INT;
					Current_State = 5;
					Counter_Changing ++;
				}
				else
					B_PHASE_ON; PWM_B_ON;
				break;
			case 5:
				C_PHASE_ON; PWM_C_ON;
				if(sensor == 1)
				{
					LD3_TOGGLE;
					B_PHASE_LOW_ON; A_PHASE_OFF;
					COMP_A_PHASE;
					COMP_FALLING_INT;
					Current_State = 6;
					Counter_Changing ++;
				}
				else
					A_PHASE_LOW_ON;
				break;
			case 6:
				B_PHASE_LOW_ON;
				if(sensor == 0)
				{
					LD3_TOGGLE;
					A_PHASE_ON; PWM_A_ON; C_PHASE_OFF;
					COMP_C_PHASE;
					COMP_RISING_INT;
					Current_State = 1;
					Counter_Changing ++;
				}
				else
					C_PHASE_ON; PWM_C_ON;
				break;
			default:
				SHUTDOWN;
				break;
		}
	}while(( (COMP_VAL == 0x00U)&&(sensor == 1) ) || ( (COMP_VAL == 0x01U)&&(sensor == 0)));
}

void ADC1_COMP_IRQHandler(void)
{
	/* Clear IT Flag*/
	EXTI->PR |= (1 << 21);
	/* Change ENABLE Phase */
	/* Rewrite Status Flag */
	Change_Phase();
}

void Fake_Change_Phase(void)
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
