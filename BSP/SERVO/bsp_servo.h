#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H
#include "stm32f4xx.h"

void TIM2_SERVO_Init(void);
void TIM8_SERVO_Init(void);
void servo(uint8_t num, uint32_t pulse);
void human_down(void);
void human_up(void);
void front_down(void);
void front_mid(void);
void front_up(void);

#endif
