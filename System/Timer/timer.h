#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f4xx.h"

extern uint32_t t6_i;
extern uint32_t t7_i;

void TIM6_Aid_Init(void);
void TIM7_Trace_Init(void);

#endif