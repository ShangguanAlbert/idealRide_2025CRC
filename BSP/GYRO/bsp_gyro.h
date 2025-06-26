#ifndef __BSP_GYRO_H
#define __BSP_GYRO_H

#include "stm32f4xx.h"

#define ARM_MATH_CM4

extern float gyro_angle_z;
extern float gyro_angle_z_raw;
extern float gyro_angle_x_u5;     // X轴角度（度）
extern float gyro_angle_x_raw_u5; // X轴角度原始值
extern float gyro_angle_y_u5;     // Y轴角度（度）
extern float gyro_angle_y_raw_u5; // Y轴角度原始值
extern float gyro_angle_z_u5;     // Z轴角度（度）
extern float gyro_angle_z_raw_u5; // Z轴角度原始值

void USART2_HWT101_Init(void);    // 初始化USART2用于HWT101陀螺仪通信
void HWT101_AngleReset(void);     // HWT101角度归零命令
float HWT101_GetAngleZ(void);     // 获取Z轴角度值
float HWT101_GetAngleZ_Raw(void); // 获取Z轴角度原始值

void HWT101_ParseData(uint8_t data_byte);
void HWT101_ProcessFrame(void);

void HWT101_ON(void);
void HWT101_OFF(void);

void UART5_HWT906_Init(void);
float HWT906_GetAngleX(void);     // 获取HWT906 X轴角度
float HWT906_GetAngleX_Raw(void); // 获取HWT906 X轴角
float HWT906_GetAngleY(void);     // 获取HWT906 Y轴角度
float HWT906_GetAngleY_Raw(void); // 获取HWT906 Y轴角
float HWT906_GetAngleZ(void);     // 获取HWT906 Z轴角度
float HWT906_GetAngleZ_Raw(void); // 获取HWT906 Z轴角
void HWT906_ParseData(uint8_t data_byte);
void HWT906_ProcessFrame(void);
void HWT906_ON(void);
void HWT906_OFF(void);

#endif