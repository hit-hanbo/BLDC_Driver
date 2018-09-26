#include "stm32l0xx.h"

#include "../Periph/BSP.h"
#include "../Periph/COMP.h"
#include "../Periph/Motor.h"
#include "../Periph/TIM.h"

/* Global Variables
 *   Rotate_Direct : 0 = Normal ; 1 = Invert
 *   Current_State : 0 = RST;
 *                   1 = A_H --> B_L
 *                   2 = B_H --> C_L
 *                   3 = C_H --> A_L
 *                   4 = A_H --> C_L
 *                   5 = B_H --> A_L
 *                   6 = C_H --> B_L
 * */
uint8_t Rotate_Direct = 0;
uint8_t Current_State = 0;

void SysTick_Init(uint32_t LOAD);
void delay(void);

int main(void)
{
	SysTick_Init(10000);
	LED_Init();
	COMP1_Init();
	COMP_A_PHASE;
	TIM2_Init(16, 999);
	TIM2->CCR1 = 750;
	TIM2->CCR2 = 500;
	TIM2->CCR3 = 250;
	TIM2_PWM_Start();
	while(1);
	/*{
		delay();
		Change_Phase();
		delay();
	}*/
}

void delay(void)
{
	uint16_t tmp;
	for(tmp=0; tmp<0xffff; tmp++);
}

// Init Systick Timer
void SysTick_Init(uint32_t LOAD)
{
	SysTick->VAL  &= 0x00000000;
	SysTick->LOAD  = LOAD;             //  SysTick Freq
    SysTick->CTRL |= 0x00000007;       //  SysTick ENABLE IRQ, CLK = SYS_CLK / 8
}

void SysTick_Handler(void)
{
	/*
	 * Place Holder For SysTick Handler
	 */
}

