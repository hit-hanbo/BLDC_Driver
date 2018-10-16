#include "MDRV.h"
/*
 * @Brief: Driver For FD6288T HVIC Driver
 * @Connection: LIN1 <--> PB7  <D4>
 *              LIN2 <--> PA11 <D10>
 *              LIN3 <--> PA12 <D2>
 */

extern uint8_t Rotate_Direct;
extern uint8_t Current_State;
extern uint16_t Duty;

void FD6288T_Init(void)
{
	//Init GPIO for LIN1~3
	/* GPIOB / GPIOC Init */
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;          // ENABLE GPIOA Clock
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;          // ENABLE GPIOB Clock
	GPIOB->ODR  &= ~(1 << 7);
	GPIOA->ODR  &= ~((1 << 11) | (1 << 12));    // PA11 PA12 PB7 LOW
	GPIOB->MODER |= (1 << 14);
	GPIOB->MODER &= ~(1 << 15);                 // GPIOB7 Output
	GPIOB->OTYPER&= ~(1 << 7);                  // GPIOB7 PP
	GPIOB->PUPDR |= (1 << 15);
	GPIOB->PUPDR &= ~(1 <<14);                  // GPIOB7 Pull-Down
	GPIOA->MODER |= (1 << 24) | (1 << 22);
	GPIOA->MODER &= ~((1 << 25) | (1 << 23));    // PA11 PA12 Output
	GPIOA->OTYPER &= ~((1 << 12) | (1 << 11));   // PA11 PA12 PP
	GPIOA->PUPDR |= (1 << 25) | (1 << 23);
	GPIOA->PUPDR &= ~((1 << 24) | (1 << 22));    // PA11 PA12 Pull-Down
}

void Fake_Change_Phase(void)
{
	switch(Current_State)
	{
	    case 0: SHUTDOWN; break;
	    case 1: BRIDGE_AH(500); BRIDGE_BL; break;
	    case 2: BRIDGE_AH(500); BRIDGE_CL; break;
	    case 3: BRIDGE_BH(500); BRIDGE_CL; break;
	    case 4: BRIDGE_BH(500); BRIDGE_AL; break;
	    case 5: BRIDGE_CH(500); BRIDGE_AL; break;
	    case 6: BRIDGE_CH(500); BRIDGE_BL; break;
	    default: SHUTDOWN; break;
	}
	if (Current_State == 6)
		Current_State = 1;
	else
		Current_State ++;
}

uint8_t BLDC_Start(void)
{
	uint8_t i;
	uint16_t timer = 300;
	Current_State = 1;         // Bridge_AH_BL
	Fake_Change_Phase();
	Delay_ms(300);
	while(1)
	{
		for(i=0; i<timer; i++)
			Delay_us(100);
		timer -= timer/15 + 1;
		if(timer < 25)
			return 1;
		Fake_Change_Phase();
		if(Current_State == 6)
			Current_State = 1;
		else
			Current_State ++;

	}
}

uint8_t BLDC_Stop(void)
{
	
}
/*  @Brief: IRQn Handler For COMP1, Change Phase
 *  
 * */
void ADC1_COMP_IRQHandler(void)
{
	uint8_t sensor;
	do
	{
		sensor = COMP1_Get_Value();            // Get Current VAL for COMP
		switch(Current_State)
		{
			case 0: SHUTDOWN; break;           // SHUTDOWN SIGNAL
											   // Current PHASE <-> Next PHASE
			case 1:                            //    AH-BL             AH-CL
				BRIDGE_AH(Duty);
				if(sensor == 1)
				{
					BRIDGE_CL;
					COMP_B_PHASE;
					COMP_FALLING_INT;
					Current_State ++;
				}
				else
					BRIDGE_BL;                 // Software Filter
				break;
			case 2:                            //    AH-CL             BH-CL
				BRIDGE_CL;
				if(sensor == 0)
				{
					BRIDGE_BH(Duty);
					COMP_A_PHASE;
					COMP_RISING_INT;
					Current_State ++;
				}
				else
					BRIDGE_AH(Duty);
				break;
			case 3:                            //    BH-CL             BH-AL
				BRIDGE_BH(Duty);
				if(sensor == 1)
				{
					BRIDGE_AL;
					COMP_C_PHASE;
					COMP_FALLING_INT;
					Current_State ++;
				}
				else
					BRIDGE_CL;
				break;
			case 4:                            //    BH-AL             CH-AL
				BRIDGE_AL;
				if(sensor == 0)
				{
					BRIDGE_CH(Duty);
					COMP_B_PHASE;
					COMP_RISING_INT;
					Current_State ++;
				}
				else
					BRIDGE_BH(Duty);
				break;
			case 5:                          //    CH-AL             CH-BL
				BRIDGE_CH(Duty);
				if(sensor == 1)
				{
					BRIDGE_BL;
					COMP_A_PHASE;
					COMP_FALLING_INT;
					Current_State ++;
				}
				else
					BRIDGE_AL;
				break;
			case 6:                         //    CH-BL             AH-BL
				BRIDGE_BL;
				if(sensor == 0)
				{
					BRIDGE_AH(Duty);
					COMP_C_PHASE;
					COMP_RISING_INT;
					Current_State = 1;
				}
				else
					BRIDGE_CH(Duty);
				break;
		}
	}while( ((COMP1_Get_Value() == 0) && (sensor == 1)) ||
			((COMP1_Get_Value() == 1) && (sensor == 0))
		);
}

