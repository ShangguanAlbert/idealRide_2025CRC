#ifndef __POSTURE_H
#define __POSTURE_H
#include "stm32f4xx.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_servo.h"
#include "bsp_sensor.h"
#include "bsp_adc.h"
#include "bsp_voice.h"
#include "reset.h"
#include "trace.h"
#include "pid_turn.h"
#include "turn.h"
#include "basic.h"

void down_pt1_6(void);
void Crash_JD(uint8_t num);
void UP_Tai2_6(uint8_t Tai_Num);
void UP_Tai7(void);
void UP_Tai8(void);
void UP_Tai1(void);
void Bridge_Travel(void);
void T_Moutain_Fast(void);
void T_Moutain_Slow(void);
void IN_Long_BLB(void);
void OUT_Long_BLB(void);
void Down_Tai8(void);
void Down_Tai7(void);
void Stop_Line(void);
void Pass_Short_BLB(void);
void Detect_OUT_jd4(void);
void Drift_Rightpass_TMountain(void);
void Touch_Seesaw_adjust(void);
void Past_Seesaw_adjust(int time_stop, int time_Seesaw);
void Land_Protect_adjust(void);
#endif