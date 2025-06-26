#ifndef __TRACE_H
#define __TRACE_H
#include "stm32f4xx.h"
#include "bsp_grayscale.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"

#define thr_whiteline 290
#define white         250
#define green         140

void Trace(void);
void Trace_new(void);
void high_run(int N);
void high_run_new(int N);
void slow_run(int N);
void speed_up(int start, int end);
void speed_up_new(int start, int end);
void speed_down(int high, int low);
void speed_down_new(int high, int low);
void bridge_Trace(uint8_t mode);
void bridge_PD(int N, uint8_t mode);
#endif