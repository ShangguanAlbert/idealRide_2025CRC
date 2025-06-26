#ifndef __ROUTE_H
#define __ROUTE_H
#include "stm32f4xx.h"
#include "reset.h"
#include "posture.h"
#include "trace.h"
#include "bsp_adc.h"
#include "turn.h"
#include "bsp_servo.h"
#include "bsp_sensor.h"
#include "bsp_SysTick.h"
#include "bsp_timer.h"
#include "bsp_led.h"
#include "basic.h"

void door2_door1(void);
void door1_door4(void);
void door4_door3(void);

void Tai1_Tai2(void);
void Tai2_jd2_Tai4(void);
void Tai2_Tai4_jd2_Tai3(void);
void Tai4_Tai3(void);
void Tai4_Tai3_new(void);
void Tai3_jd1_OUT(void);

void Tai3_jd1_door1(void);
void Tai3_jd1_door2(void);
void Tai3_jd1_door3(void);
void Tai3_jd1_door4(void);

void Tai3_jd1_door1_Tai5(void);
void Tai3_jd1_door2_Tai5(void);
void Tai3_jd1_door3_Tai5(void);
void Tai3_jd1_door4_Tai5(void);

void door1_Tai5(void);
void door2_Tai5(void);
void door3_Tai5(void);
void door4_Tai5(void);
void door4_door3_Tai5(void);

void Tai5_jd5_Tai7(void);
void Tai7_Tai8(void);
void Tai8_jd3(void);
void jd3_jd4(void);

void jd4_door1_Home(void);
void jd4_door2_Home(void);
void jd4_door3_Home(void);
void jd4_door4_Home(void);

void Tai6_seesaw(void);

void jd3_Tai6_jd4(void);

void New_Seesaw(void);
void Up_Down_Seesaw(void);
void newnew_seesaw(void);
#endif