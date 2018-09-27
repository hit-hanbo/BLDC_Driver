/*
 * Motor.h
 *
 *  Created on: Sep 20, 2018
 *      Author: hitha
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define PWM_ON_ALL { TIM2->CCR1=250; TIM2->CCR2=250; TIM2->CCR3=250; }
#define PWM_A_ON  TIM2->CCR1 = 250
#define PWM_B_ON  TIM2->CCR2 = 250
#define PWM_C_ON  TIM2->CCR3 = 250

#define A_PHASE_ON     GPIOB->ODR |= (1 << 7)
#define A_PHASE_OFF    GPIOB->ODR &= ~(1 << 7)
#define B_PHASE_ON     GPIOA->ODR |= (1 << 11)
#define B_PHASE_OFF    GPIOA->ODR &= ~(1 << 11)
#define C_PHASE_ON     GPIOA->ODR |= (1 << 12)
#define C_PHASE_OFF    GPIOA->ODR &= ~(1 << 12)

#define A_PHASE_LOW_ON  { A_PHASE_ON; TIM2->CCR1 = 0x0000; }
#define B_PHASE_LOW_ON  { B_PHASE_ON; TIM2->CCR2 = 0x0000; }
#define C_PHASE_LOW_ON  { C_PHASE_ON; TIM2->CCR3 = 0x0000; }

#define SHUTDOWN { A_PHASE_OFF; B_PHASE_OFF; C_PHASE_OFF; }

void GPIO_SDx_Init(void);
void Bridge_A_B(void);
void Bridge_A_C(void);
void Bridge_C_B(void);
void Bridge_B_A(void);
void Bridge_B_C(void);
void Bridge_C_A(void);

void Change_Phase(void);

#endif /* MOTOR_H_ */
