#include "stm32l011xx.h"

#ifndef COMP_H_
#define COMP_H_

#define COMP_CHANNEL_MSB 0x00000020        // Bit Address
#define COMP_CHANNEL_LSB 0x00000010        // Bit Address

#define COMP_A_PHASE { COMP1->CSR &= ~COMP_CHANNEL_MSB; COMP1->CSR |= COMP_CHANNEL_LSB; }  // Compare Channel Phase-A
#define COMP_B_PHASE { COMP1->CSR |= COMP_CHANNEL_MSB; COMP1->CSR &= ~COMP_CHANNEL_LSB; }  // Compare Channel Phase-B
#define COMP_C_PHASE { COMP1->CSR |= COMP_CHANNEL_MSB; COMP1->CSR |= COMP_CHANNEL_LSB;  }  // Compare Channel Phase-C

void COMP1_IT_Init(void);
void COMP1_Init(void);
uint8_t COMP1_Get_Value(void);

#endif
