#ifndef __TURN_H
#define __TURN_H
#include "stm32f4xx.h"
#include "trace.h"
#include "bsp_adc.h"
#include "posture.h"
#include "bsp_SysTick.h"
#include "bsp_compass.h"
#include "reset.h"
#include "pid_turn.h"
#include "bsp_servo.h"
#include "bsp_sensor.h"
#include "basic.h"

void Tai1_6_zhuan(uint8_t Tai_Num);
void Tai7_zhuan(void);
void Tai8_zhuan(void);
void TurnRight_90_Rdetect(void);
void TurnRightJD_90(void);
void TurnLeft_90_Rdetect(void);
void TurnLeft_155(void);
void TurnLeft_Place_90(void);
void TurnLeft_90_Ldetect(void);
void JD1_2_TurnAround(void);
void Door_TurnAround(void);
void TurnLeft_inJD1(void);
void TurnRight_45_TXS(void);
void TurnLeft_45_Longline(void);
void TurnLeft_inJD2(void);
void Small_Turn_inDoor2(void);
void TurnLeft_45_inTai5(void);
void TurnRight_Place_90(void);
void TurnRight_45_inCross(void);
void TurnLeft_155_atCross(void);
void TurnRight_155_outCross(void);
void door12_Turn_BLB(void);
void TurnRight_45_Tai1(void);
void TurnRight_90_Ldetect(void);
void TurnRight_90_hdrDetect(void);
void TurnLeft_155_hdlDetect(void);
void TurnLeft_Door1_inCross(void);
void door34_Turn_BLB(void);
void TurnLeft_155_door4(void);
void TurnRight_inDoor3(void);
void Small_Turn_toDoor34(void);
void TurnLeft_155_Tai4(void);
void drift_right_jd1_door2(int speed, int model);
#endif // !__TURN_H
