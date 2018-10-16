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

// Bridge CTRL
#define A_OFF   {PWM_A_OFF; LIN1_OFF; }
#define B_OFF   {PWM_B_OFF; LIN2_OFF; }
#define C_OFF   {PWM_C_OFF; LIN3_OFF; }
#define ALL_OFF {A_OFF; B_OFF; C_OFF; }

#define BRIDGE_AH(X) { PWM_B_OFF; PWM_C_OFF; PWM_A_ON(X); }
#define BRIDGE_AL    { LIN2_OFF; LIN3_OFF; LIN1_ON; }
#define BRIDGE_BH(X) { PWM_A_OFF; PWM_C_OFF; PWM_B_ON(X); }
#define BRIDGE_BL    { LIN1_OFF; LIN3_OFF; LIN2_ON; }
#define BRIDGE_CH(X) { PWM_A_OFF; PWM_B_OFF; PWM_C_ON(X); }
#define BRIDGE_CL    { LIN1_OFF; LIN2_OFF; LIN3_ON; }

#define AH_BL_ON(X) {ALL_OFF; PWM_A_ON(X); LIN2_ON; }
#define AH_CL_ON(X) {ALL_OFF; PWM_A_ON(X); LIN3_ON; }
#define BH_AL_ON(X) {ALL_OFF; PWM_B_ON(X); LIN1_ON; }
#define BH_CL_ON(X) {ALL_OFF; PWM_B_ON(X); LIN3_ON; }
#define CH_AL_ON(X) {ALL_OFF; PWM_C_ON(X); LIN1_ON; }
#define CH_BL_ON(X) {ALL_OFF; PWM_C_ON(X); LIN2_ON; }

#define SENSE_A_ON   EXTI->IMR |= (1 << 5)
#define SENSE_B_ON   EXTI->IMR |= (1 << 6)
#define SENSE_C_ON   EXTI->IMR |= (1 << 7)
#define SENSE_A_OFF  EXTI->IMR &= ~(1 << 5)
#define SENSE_B_OFF  EXTI->IMR &= ~(1 << 6)
#define SENSE_C_OFF  EXTI->IMR &= ~(1 << 7)

#define SENSE_A       { SENSE_A_ON; SENSE_B_OFF; SENSE_C_OFF; }
#define SENSE_A_INT_F { EXTI->FTSR |= (1 << 5); EXTI->RTSR &= ~(1 << 5); }
#define SENSE_A_INT_R { EXTI->RTSR |= (1 << 5); EXTI->FTSR &= ~(1 << 5); }
#define SENSE_B       { SENSE_B_ON; SENSE_A_OFF; SENSE_C_OFF; }
#define SENSE_B_INT_F { EXTI->FTSR |= (1 << 6); EXTI->RTSR &= ~(1 << 6); }
#define SENSE_B_INT_R { EXTI->RTSR |= (1 << 6); EXTI->FTSR &= ~(1 << 6); }
#define SENSE_C       { SENSE_C_ON; SENSE_A_OFF; SENSE_B_OFF; }
#define SENSE_C_INT_F { EXTI->FTSR |= (1 << 7); EXTI->RTSR &= ~(1 << 7); }
#define SENSE_C_INT_R { EXTI->RTSR |= (1 << 7); EXTI->FTSR &= ~(1 << 7); }
#define SENSE_OFF      EXTI->IMR &= ~((1 << 5) | (1 << 6) | (1 << 7))

#define SENSE_A_VAL  ( GPIOA->IDR & (1 << 5) )
#define SENSE_B_VAL  ( GPIOA->IDR & (1 << 6) )
#define SENSE_C_VAL  ( GPIOA->IDR & (1 << 7) )

void FD6288T_Init(void);
void Fake_Change_Phase(void);
uint8_t BLDC_Start(void);
uint8_t BLDC_Stop(void);
void COMPARE_Init(void);
uint8_t COMPARE_Get_Val(void);

#endif /* MDRV_H_ */
