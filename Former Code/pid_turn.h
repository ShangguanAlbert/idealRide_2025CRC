#ifndef __PID_TURN_H
#define __PID_TURN_H
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <string.h>
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "bsp_compass.h"
#include "basic.h"
#include "bsp_timer.h"

enum {
    LAST = 0,
    NOW  = 1,
};

typedef struct
{
    /* p、i、d参数 */
    float p;
    float i;
    float d;
    /* 目标值、反馈值、误差值 */
    float set;
    float get;
    float err[2];
    /* p、i、d各项计算出的输出 */
    float pout;
    float iout;
    float dout;
    /* pid公式计算出的总输出 */
    float out;
    /* pid最大输出限制  */
    uint32_t max_output;
    /* pid积分输出项限幅 */
    uint32_t integral_limit;

} pid_t_robomaster;

// void abs_limit(float *a, float ABS_MAX);
void pid_init(pid_t_robomaster *pid, uint32_t max_out, uint32_t intergral_limit, float kp, float ki, float kd);
void pid_reset(pid_t_robomaster *pid, float kp, float ki, float kd);
float pid_calc(pid_t_robomaster *pid, float get, float set);

void gqJD(void);
void ZWCJD(void);
void FWCJD(void);
void Deg_IN(void);
void zpid(int time_turn);
void fpid(int time_turn);
void pid_Turn(int turn_time);
void xpid(int turn_time);
extern pid_t_robomaster pid_yaw;

typedef struct // 位置式PID结构体
{
    float kp;       // 比例
    float ki;       // 微分
    float kd;       // 积分
    float err;      // 误差
    float err_last; // 上次偏差
    float sum_err;  // 历史偏差之和，用于积分累加
    float max_sum_err;
    float out; // PID输出，可以是PWM占空比，可以是测距偏差，可以是输出温度（温控PID）
} PID_Turn_Params;

extern PID_Turn_Params pid_comp_params;

void Set_PID_turn_params(PID_Turn_Params *pid_comp, float p, float i, float d, float max_sum_error);
void PID_Calculate_Right(PID_Turn_Params *pid_comp, int angle_target, int vmax, int vmin);
void PID_Calculate_Left(PID_Turn_Params *pid_comp, int angle_target, int vmax, int vmin);
void PID_Calculate_Around(PID_Turn_Params *pid_comp, int angle_target, int vmax, int vmin);

void PID_TurnRight(int angle_target, int time, int speed);
void PID_TurnLeft(int angle_target, int time, int speed);
void PID_TurnAround(int deg, int time);

#endif