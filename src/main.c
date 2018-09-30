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

int main(void)
{
	uint8_t i;
	SysTick_Init(50000);
	LED_Init();

	GPIO_SDx_Init();
	COMP1_Init();

	TIM2_Init(1, 999);
	TIM2_PWM_Start();

	Current_State = 1;
	for(i=0;i<120;i++)
	{
		delay();
		Change_Phase();
	}
	while(1);
}

// Init Systick Timer
void SysTick_Init(uint32_t LOAD)
{
	SysTick->VAL  &= 0x00000000;       //  Reset Value
	SysTick->LOAD  = LOAD;             //  SysTick Freq
    SysTick->CTRL |= 0x00000007;       //  SysTick ENABLE IRQ, CLK = SYS_CLK / 8
}

void SysTick_Handler(void)
{
	/*
	 * Place Holder For SysTick Handler
	 */
	// Change_Phase();
}

