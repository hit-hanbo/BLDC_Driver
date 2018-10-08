#include <stddef.h>
#include "stm32l0xx.h"

/*  @Brief: This Dev Version For BLDC_Driver
 *  @HWver: v1.1  (STM32L0 + FD6288T)
 *  @Detailed info https://github.com/hit-hanbo/BLDC_Driver.git
 *
 * */

/* Global Variables
 *   Duty          : PWMx Duty Cycle ()
 *   Rotate_Direct : 0 = Normal ; 1 = Invert
 *   Current_State : 0 = RST;
 *                   1 = A_H --> B_L
 *                   2 = B_H --> C_L
 *                   3 = C_H --> A_L
 *                   4 = A_H --> C_L
 *                   5 = B_H --> A_L
 *                   6 = C_H --> B_L
 * */
uint8_t Rotate_Direct;
uint8_t Current_State;
uint16_t Duty;

int main(void)
{
	while(1);
	return 0;
}

void SysTick_Handler(void)
{

}

