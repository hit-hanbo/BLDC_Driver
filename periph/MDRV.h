#ifndef MDRV_H_
#define MDRV_H_

#include "BSP.h"
#include "stm32l011xx.h"
#include "core_cm0plus.h"

// Low Bridge CTRL
#define LIN1_ON  GPIOB->ODR |= (1 << 7)
#define LIN1_OFF GPIOB->ODR &= ~(1 << 7)
#define LIN2_ON  GPIOA->ODR |= (1 << 11)
#define LIN2_OFF GPIOA->ODR &= ~(1 << 11)
#define LIN3_ON  GPIOA->ODR |= (1 << 12)
#define LIN3_OFF GPIOA->ODR &= ~(1 << 12)

#endif /* MDRV_H_ */
