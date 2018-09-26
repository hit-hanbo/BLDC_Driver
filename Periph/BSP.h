#ifndef BSP_H_
#define BSP_H_

void LED_Init(void);

#define LD3_ON      GPIOB->ODR |= (1 << 3)
#define LD3_OFF     GPIOB->ODR &= ~(1 << 3)
#define LD3_TOGGLE  GPIOB->ODR ^= (1 << 3)

#endif
