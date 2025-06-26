/**
 * @file pid_turn.c
 * @author sgfz
 * @brief 指南针PID转向
 * @version 0.1
 * @date 2024-09-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "pid_turn.h"

pid_t_robomaster pid_yaw = {0};

extern float JD;
extern float XJD;
float GJD;
float MJD;
float WJD;
int speed_adj;
extern uint32_t t3_i;

/**
 * @brief 绝对值限幅函数
 * @param a 输入值
 * @param ABS_MAX 待比较的绝对值
 */
static void abs_limit(float *a, float ABS_MAX)
{
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}

/**
 * @brief     PID 参数复位函数
 * @param[in] pid: PID 结构体
 * @param[in] kp/ki/kd: 具体 PID 参数
 */
void pid_reset(pid_t_robomaster *pid, float kp, float ki, float kd)
{
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;

    pid->pout = 0;
    pid->iout = 0;
    pid->dout = 0;
    pid->out  = 0;
}

/**
 * @brief     PID 初始化函数
 * @param[in] pid: PID 结构体
 * @param[in] max_out: 最大输出
 * @param[in] intergral_limit: 积分限幅
 * @param[in] kp/ki/kd: 具体 PID 参数
 */
void pid_init(pid_t_robomaster *pid, uint32_t max_out, uint32_t intergral_limit, float kp, float ki, float kd)
{
    pid->integral_limit = intergral_limit;
    pid->max_output     = max_out;

    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}

/**
 * @brief     PID 计算函数，使用位置式 PID 计算
 * @param[in] pid: PID 结构体
 * @param[in] get: 反馈数据
 * @param[in] set: 目标数据
 * @retval    PID 计算输出
 */
float pid_calc(pid_t_robomaster *pid, float get, float set)
{
    pid->get      = get;
    pid->set      = set;
    pid->err[NOW] = set - get;

    pid->pout = pid->p * pid->err[NOW];
    pid->iout += pid->i * pid->err[NOW];
    pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]);

    abs_limit(&(pid->iout), pid->integral_limit);
    pid->out = pid->pout + pid->iout + pid->dout;
    abs_limit(&(pid->out), pid->max_output);

    pid->err[LAST] = pid->err[NOW];

    return pid->out;
}
/**
 * @brief 角度传参
 */
void gqJD(void)
{
    GJD = JD;
}
/**
 * @brief 角度越界处理1
 *
 */
void ZWCJD(void)
{
    if (GJD > 0 && GJD < 180) {
        MJD = GJD + 180;
    }
    if (GJD > 180 && GJD < 360) {
        MJD = GJD - 180;
    }
    if (GJD == 0 || GJD == 360) {
        MJD = 180;
    }
}
/**
 * @brief 角度越界处理2
 *
 */
void FWCJD(void)
{
    if (GJD > 0 && GJD < 180) {
        WJD = XJD - 180;
    }
    if (GJD > 180 && GJD < 360) {
        WJD = XJD + 180;
    }
    if (GJD == 180) {
        WJD = 0;
    }
}
/**
 * @brief HWT101角度读取并越界处理
 *
 */
void Deg_IN(void)
{
    gqJD();
    ZWCJD();
    FWCJD();
}
/**
 * @brief 非越界角度转动 PID
 * @param time_turn 要转的时间
 */
void zpid(int time_turn)
{
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    do {
        speed_adj = pid_calc(&pid_yaw, JD, MJD);
        run(-speed_adj * 0.9, speed_adj);
    } while (t3_i < time_turn);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
}
/**
 * @brief 越界角度转动 PID
 * @param time_turn 要转的时间
 */
void fpid(int time_turn)
{
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    do {
        speed_adj = pid_calc(&pid_yaw, XJD, MJD);
        run(-speed_adj * 0.9, speed_adj);
    } while (t3_i < time_turn);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
}
/**
 * @brief 90到270之间角度PID处理
 * @param turn_time 要转的时间
 */
void xpid(int turn_time)
{
    if (GJD >= 90 && GJD <= 270) {
        fpid(turn_time);
    } else
        zpid(turn_time);
}
/**
 * @brief 平台掉头180° PID
 * @param turn_time 要转的时间
 * @note 在使用前车辆读取角度必须大于90度!!!!!否则无效
 */
void pid_Turn(int turn_time)
{
    if (MJD > 90 && MJD < 270) {
        zpid(turn_time);
    } else if (MJD > 270 && MJD < 360) {
        fpid(turn_time);
    } else if (MJD > 0 && MJD < 90) {
        fpid(turn_time);
    } else
        fpid(turn_time);
}

/* - PID转向 2024.7.9 - */
PID_Turn_Params pid_comp_params;

float angle_cnt = 0; // 给此时角度赋初值

/**
 * @brief PID结构体初始化函数
 * @param pid_comp PID结构体
 * @param p P参数
 * @param i I参数
 * @param d D参数
 * @param max_sum_error 最大累计误差
 */
void Set_PID_turn_params(PID_Turn_Params *pid_comp, float p, float i, float d, float max_sum_error)
{
    pid_comp->kp          = p;
    pid_comp->ki          = i;
    pid_comp->kd          = d;
    pid_comp->sum_err     = 0;
    pid_comp->max_sum_err = max_sum_error;
}
/**
 * @brief PID右转计算
 * @param pid_comp PID结构体
 * @param angle_target 目标角度
 * @param vmax 最大转速
 * @param vmin 最小转速
 */
void PID_Calculate_Right(PID_Turn_Params *pid_comp, int angle_target, int vmax, int vmin)
{
    // 从左往右转 -- 0-360
    int angle_now;
    angle_now     = compass_b();
    pid_comp->err = angle_now - angle_target;
    if (fabs(pid_comp->err) > 40.0) {
        // 不调用PID
        run(vmax, -vmax);
    } else if (fabs(pid_comp->err) < 4.0) {
        stop();
    } else {
        // 调用PID
        pid_comp->sum_err += pid_comp->err; // 偏左正，偏右负
        pid_comp->sum_err  = pid_comp->sum_err > pid_comp->max_sum_err ? pid_comp->max_sum_err : pid_comp->sum_err;
        pid_comp->out      = pid_comp->kp * pid_comp->err + pid_comp->ki * pid_comp->sum_err + pid_comp->kd * (pid_comp->err - pid_comp->err_last);
        pid_comp->err_last = pid_comp->err;
        run(vmin + pid_comp->out, -vmin - pid_comp->out);
    }
}
/**
 * @brief PID左转计算
 * @param pid_comp PID结构体
 * @param angle_target 目标角度
 * @param vmax 最大转速
 * @param vmin 最小转速
 */
void PID_Calculate_Left(PID_Turn_Params *pid_comp, int angle_target, int vmax, int vmin)
{
    // 从右往左转 -- 0-360
    int angle_now;
    angle_now     = compass_b();
    pid_comp->err = angle_target - angle_now;
    if (fabs(pid_comp->err) > 40.0) {
        // 不调用PID
        run(-vmax, vmax);
    } else if (fabs(pid_comp->err) < 4.0) {
        stop();
    } else {
        // 调用PID
        pid_comp->sum_err += pid_comp->err;                                                                         // 偏左负，偏右正
        pid_comp->sum_err  = pid_comp->sum_err > pid_comp->max_sum_err ? pid_comp->max_sum_err : pid_comp->sum_err; // 负值限幅?还没写
        pid_comp->out      = pid_comp->kp * pid_comp->err + pid_comp->ki * pid_comp->sum_err + pid_comp->kd * (pid_comp->err - pid_comp->err_last);
        pid_comp->err_last = pid_comp->err;
        run(-vmin - pid_comp->out, vmin + pid_comp->out);
    }
}
/**
 * @brief PID平台回转180计算
 * @param pid_comp PID结构体
 * @param angle_target 目标角度
 * @param vmax 最大转速
 * @param vmin 最小转速
 */
void PID_Calculate_Around(PID_Turn_Params *pid_comp, int angle_target, int vmax, int vmin)
{
    // 从左往右转 -- 0-360
    int angle_now;
    angle_now     = compass_b();
    pid_comp->err = angle_now - angle_target;
    if (fabs(pid_comp->err) > 35.0) {
        // 不调用PID
        run(vmax, -vmax + 15);
    } else if (fabs(pid_comp->err) < 5.0) {
        stop();
    } else {
        // 调用PID
        pid_comp->sum_err += pid_comp->err; // 偏左正，偏右负
        pid_comp->sum_err  = pid_comp->sum_err > pid_comp->max_sum_err ? pid_comp->max_sum_err : pid_comp->sum_err;
        pid_comp->out      = pid_comp->kp * pid_comp->err + pid_comp->ki * pid_comp->sum_err + pid_comp->kd * (pid_comp->err - pid_comp->err_last);
        pid_comp->err_last = pid_comp->err;
        run(vmin + pid_comp->out, -vmin - pid_comp->out + 15);
    }
}
/**
 * @brief PID右转 要多写10度
 *
 * @param angle_target 目标角度
 * @param time 时间
 * @param speed 最大速度
 */
void PID_TurnRight(int angle_target, int time, int speed)
{
    Stop(100);
    HWT101_to_0();
    Stop(250);
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    do {
        PID_Calculate_Right(&pid_comp_params, angle_target, speed, 20);
    } while (t3_i < time);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
    stop();
    pid_comp_params.err      = 0;
    pid_comp_params.err_last = 0;
    pid_comp_params.sum_err  = 0;
    pid_comp_params.out      = 0;
}
/**
 * @brief PID左转 要少写十度
 *
 * @param angle_target 目标角度
 * @param time 时间
 * @param speed 最大速度
 */
void PID_TurnLeft(int angle_target, int time, int speed)
{
    Stop(100);
    HWT101_to_0();
    Stop(250);
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    do {
        PID_Calculate_Left(&pid_comp_params, angle_target, speed, 20);
    } while (t3_i < time);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
    stop();
    pid_comp_params.err      = 0;
    pid_comp_params.err_last = 0;
    pid_comp_params.sum_err  = 0;
    pid_comp_params.out      = 0;
}
/**
 * @brief PID平台转180度
 *
 * @param deg 目标角度
 * @param time 时间
 */
void PID_TurnAround(int deg, int time)
{
    Stop(100);
    HWT101_to_0();
    Stop(250);
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    do {
        PID_Calculate_Around(&pid_comp_params, deg, 100, 35);
    } while (t3_i < time);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
    stop();
    pid_comp_params.err      = 0;
    pid_comp_params.err_last = 0;
    pid_comp_params.sum_err  = 0;
    pid_comp_params.out      = 0;
}