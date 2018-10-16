#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"

#include "BSP.h"
#include "MDRV.h"

extern uint32_t Counter_Changing;

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
uint8_t flag;
uint16_t Duty;

void SysTick_Init(uint32_t LOAD);

int main(void)
{
	uint32_t i, j;
	SysTick_Init(50000);
	LED_Init();
	FD6288T_Init();

	TIM2_Init(1, 999);
	TIM2_PWM_Start();
	COMPARE_Init();

	Current_State = 1;

	for(j=0;j<1200;j++)
	{
		Fake_Change_Phase();
		Delay_ms(1000);
		if(j == 900)
		{
			EXTI->IMR |= (0x07 << 5);
			SENSE_A_INT_F;
			SENSE_B_INT_F;
			SENSE_C_INT_F;
		}
	}

	while(1);
}


void SysTick_Handler(void)
{
	/*
	 * Place Holder For SysTick Handler
	 */
}

