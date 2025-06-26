#ifndef __RESET_H
#define __RESET_H
#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_motor.h"
#include "trace.h"
#include "basic.h"

void TIM6_Reset_Init(void);
void TIM7_Aid_Init(void);
void Reset(int time_last, int goal_speed);
void Reset_bridge(int time_last, int goal_speed, uint8_t mode);
void Reset_drift_left(int speed, uint8_t model, int time_last);
void Reset_drift_right(int speed, uint8_t model, int time_last);
void Reset_Aid(int time_last, int goal_speed);

#endif // !__RESET_H