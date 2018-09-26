/*
 * Motor.h
 *
 *  Created on: Sep 20, 2018
 *      Author: hitha
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define A_PHASE_ON     GPIOB->ODR |= (1 << 7)
#define A_PHASE_OFF    GPIOB->ODR &= ~(1 << 7)
#define B_PHASE_ON     GPIOC->ODR |= (1 << 14)
#define B_PHASE_OFF    GPIOC->ODR &= ~(1 << 14)
#define C_PHASE_ON     GPIOC->ODR |= (1 << 15)
#define C_PHASE_OFF    GPIOC->ODR &= ~(1 << 15)

#define A_PHASE_LOW_ON  { A_PHASE_ON; TIM2->CCR1 = 0x0000; }
#define B_PHASE_LOW_ON  { B_PHASE_ON; TIM2->CCR2 = 0x0000; }
#define C_PHASE_LOW_ON  { C_PHASE_ON; TIM2->CCR3 = 0x0000; }

#define SHUTDOWN { A_PHASE_OFF; B_PHASE_OFF; C_PHASE_OFF; }

void Change_Phase(void);

#endif /* MOTOR_H_ */
