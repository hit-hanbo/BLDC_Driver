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

void FD6288T_Init(void);
void Fake_Change_Phase(void);
uint8_t BLDC_Start(void);
uint8_t BLDC_Stop(void);

#endif /* MDRV_H_ */
