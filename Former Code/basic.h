#ifndef __BASIC_H
#define __BASIC_H
#include "stm32f4xx.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_servo.h"
#include "bsp_sensor.h"
#include "bsp_adc.h"
#include "bsp_voice.h"
#include "reset.h"
#include "trace.h"

/* 电机基础动作 */

void Run(int run_speed);
void Run_delay(int run_speed, uint16_t last_time);
void run(int left_speed, int right_speed);
void run_delay(int left_speed, int right_speed, uint16_t last_time);
void stop(void);
void Stop(uint16_t times);
void Right_Speed_Down(int Fir, int End, uint16_t delay_time);
void Left_Speed_Down(int Fir, int End, uint16_t delay_time);
void Right_Speed_Up(int Fir, int End, uint16_t delay_time);
void Left_Speed_Up(int Fir, int End, uint16_t delay_time);
void turnR_speed_up(int Low, int High, uint16_t delay_time);
void turnR_speed_down(int High, int Low, uint16_t delay_time);
void turnL_speed_up(int Low, int High, uint16_t delay_time);
void turnL_speed_down(int High, int Low, uint16_t delay_time);
void drift_right(int speed, uint8_t model);
void drift_left(int speed, uint8_t model);

/* 基础动作 */
/**
 * @brief 禁行板
 */
void Start(void);
/**
 * @brief 出发资格动作 同时举起双臂 人形倒下
 */
void Departure_Action(void);

#endif