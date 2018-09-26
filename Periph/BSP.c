/*
 */
#include "stm32l011xx.h"
#include "BSP.h"



// LD3 Init
void LED_Init(void)
{
	RCC->IOPENR   |= RCC_IOPENR_GPIOBEN;// ENABLE GPIOB Clock
	GPIOB->MODER  |= (1 << 6);
	GPIOB->MODER  &= ~(1 << 7);         // LD3 Output
	GPIOB->OTYPER &= ~(1 << 3);         // LD3 PP
	GPIOB->PUPDR  |= (1 << 6);
	GPIOB->PUPDR  &= ~(1 << 7);         // Pull-up
}

