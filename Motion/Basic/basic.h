#ifndef __BASIC_H
#define __BASIC_H
#include <stm32f4xx.h>

void Run(int run_speed);
void Run_delay(int run_speed, uint16_t last_time);
void run(int left_speed, int right_speed);
void run_delay(int left_speed, int right_speed, uint16_t last_time);
void stop(void);
void Stop(uint16_t times);
void turnR_speed_up(int Low, int High, uint16_t delay_time);
void turnR_speed_down(int High, int Low, uint16_t delay_time);
void turnL_speed_up(int Low, int High, uint16_t delay_time);
void turnL_speed_down(int High, int Low, uint16_t delay_time);

#endif // __BASIC_H