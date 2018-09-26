/*
 * TIM.h
 *
 *  Created on: Sep 20, 2018
 *      Author: hitha
 */

#ifndef TIM_H_
#define TIM_H_

void TIM2_Init(uint16_t PSC, uint16_t ARR);
void TIM2_PWM_Start(void);
void TIM2_PWM_Stop(void);
#endif /* TIM_H_ */
