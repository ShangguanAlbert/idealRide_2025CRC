#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H
#include "stm32f4xx.h"

void TIM2_SERVO_Init(void);
void TIM8_SERVO_Init(void);
void servo(uint8_t num, uint32_t pulse);

#endif
