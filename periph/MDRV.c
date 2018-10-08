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
	GPIOA->ODR  &= ~((1 << 11) | (1 << 12))     // PA11 PA12 PB7 LOW
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
	}
}
