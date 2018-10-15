#ifndef BSP_H_
#define BSP_H_

#include "stm32l011xx.h"
#include "core_cm0plus.h"

// LD3 Option
#define LD3_ON      GPIOB->ODR |= (1 << 3)
#define LD3_OFF     GPIOB->ODR &= ~(1 << 3)
#define LD3_TOGGLE  GPIOB->ODR ^= (1 << 3)

// COMP Option
#define COMP_ON  COMP1->CSR |= 0x01
#define COMP_OFF COMP1->CSR &= ~(0x01)
#define COMP_CHANNEL_MSB 0x00000020        // Bit Address
#define COMP_CHANNEL_LSB 0x00000010        // Bit Address
#define COMP_A_PHASE { COMP1->CSR &= ~COMP_CHANNEL_MSB; COMP1->CSR |= COMP_CHANNEL_LSB; }  // Compare Channel Phase-A
#define COMP_B_PHASE { COMP1->CSR |= COMP_CHANNEL_MSB; COMP1->CSR &= ~COMP_CHANNEL_LSB; }  // Compare Channel Phase-B
#define COMP_C_PHASE { COMP1->CSR |= COMP_CHANNEL_MSB; COMP1->CSR |= COMP_CHANNEL_LSB;  }  // Compare Channel Phase-C
#define COMP_VAL (COMP1->CSR & 0x40000000)>>30
#define COMP_INT_ON  EXTI->IMR  |= (1 << 21);
#define COMP_INT_OFF EXTI->IMR  &= ~(1 << 21);
#define COMP_FALLING_INT { EXTI->RTSR |= (1 << 21); EXTI->FTSR &= ~(1 << 21); }
#define COMP_RISING_INT  { EXTI->FTSR |= (1 << 21); EXTI->RTSR &= ~(1 << 21); }

// TIM Option
#define PWM_OFF      TIM2->CR1 &= ~0x01
#define PWM_A_ON(X)  TIM2->CCR1 = X
#define PWM_A_OFF    TIM2->CCR1 = 0
#define PWM_B_ON(X)  TIM2->CCR2 = X
#define PWM_B_OFF    TIM2->CCR2 = 0
#define PWM_C_ON(X)  TIM2->CCR3 = X
#define PWM_C_OFF    TIM2->CCR3 = 0
#define SHUTDOWN     { PWM_A_OFF; PWM_B_OFF; PWM_C_OFF; }

void SysTick_Init(uint32_t LOAD);
void LED_Init(void);
void COMP1_Init(void);
uint8_t COMP1_Get_Value(void);
void TIM2_Init(uint16_t PSC, uint16_t ARR);
void TIM2_PWM_Start(void);
void TIM2_PWM_Stop(void);
void Delay_ms(uint16_t nms);
void Delay_us(uint16_t nus);

#endif
