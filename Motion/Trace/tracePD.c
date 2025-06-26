#include "tracePD.h"
#include "trace.h"
#include "bsp_grayscale.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"

int Gray_sum;
int lastError;
int motorSpeed;
int lastm1Speed; // 上次速度
int lastm2Speed;
int mSpeed_right;
int mSpeed_left;
uint8_t cnt_whiteline;
double sum = 0;
float error;
int speed;
float KP;
float KD;

/**
 * @brief 获得灰度值, 判白线数量
 */
void get_huidu_va(void)
{
    uint8_t i;
    Gray_sum      = 0;
    sum           = 0.0;
    cnt_whiteline = 0;
    for (i = 0; i < 12; i++) {
        sum += Huidu_va(i);
        if (Huidu_va(i) > thr_whiteline) {
            cnt_whiteline++;
        }
    }
}
/**
 * @brief PD权值法巡线+干扰线条件+限幅
 */
void Trace(void)
{
    // 绿地都小于140
    // 白线都大于250
    if (Huidu_va(1) > white && Huidu_va(2) > white && Huidu_va(3) > white && Huidu_va(4) > white &&
        Huidu_va(5) > white && Huidu_va(6) > white && Huidu_va(7) > white && Huidu_va(8) > white &&
        Huidu_va(9) > white && Huidu_va(10) > white) {
        error = 0;
    } else if (Huidu_va(1) < green && Huidu_va(2) < green && Huidu_va(3) < green && Huidu_va(4) < green &&
               Huidu_va(5) < green && Huidu_va(6) < green && Huidu_va(7) < green && Huidu_va(8) < green &&
               Huidu_va(9) < green && Huidu_va(10) < green) {
        error = 0;
    } else if (Huidu_va(5) > white && Huidu_va(6) > white) {
        error = 0;
    } else if ((Huidu_va(5) > white || Huidu_va(6) > white) && (Huidu_va(0) > white || Huidu_va(1) > white)) {
        error = 0;
    } else if ((Huidu_va(5) > white || Huidu_va(6) > white) && (Huidu_va(11) > white || Huidu_va(10) > white)) {
        error = 0;
    } else if (cnt_whiteline >= 3) {
        error = 0;
    } else {
        Gray_sum = Huidu_va(0) * (3) + Huidu_va(1) * (5) + Huidu_va(2) * (4) + Huidu_va(3) * (3) + Huidu_va(4) * (2) + Huidu_va(5) * (1) + Huidu_va(11) * (-3) + Huidu_va(10) * (-5) + Huidu_va(9) * (-4) + Huidu_va(8) * (-3) + Huidu_va(7) * (-2) + Huidu_va(6) * (-1);
        error    = Gray_sum * 1.0 * (3000.0 / sum * 1.0);
    }

    motorSpeed   = KP * error + KD * (error - lastError);
    lastError    = error;
    mSpeed_right = speed - motorSpeed; // 右轮速度
    mSpeed_left  = speed + motorSpeed; // 左轮速度
    lastm1Speed  = mSpeed_right;
    lastm2Speed  = mSpeed_left;
    // 限幅
    if (speed < 160) {
        if (mSpeed_right < 0) {
            mSpeed_right = 0;
        } else if (mSpeed_right > (1.5 * speed)) {
            mSpeed_right = 1.5 * speed;
        }

        if (mSpeed_left < 0) {
            mSpeed_left = 0;
        } else if (mSpeed_left > (1.5 * speed)) {
            mSpeed_left = 1.5 * speed;
        }
    }
    if (speed >= 160) {
        if (mSpeed_right < 0) {
            mSpeed_right = 0;
        } else if (mSpeed_right > (1.35 * speed)) {
            mSpeed_right = 1.35 * speed;
        }

        if (mSpeed_left < 0) {
            mSpeed_left = 0;
        } else if (mSpeed_left > (1.35 * speed)) {
            mSpeed_left = 1.35 * speed;
        }
    }

    set_pwm(1, mSpeed_right, speed);
    set_pwm(2, mSpeed_left, speed);
}

/**
 * @brief 桥巡线函数
 * @param mode 模式
 */
void bridge_Trace(uint8_t mode)
{
    if (mode == 1) {
        Gray_sum = Huidu_va(0) * (1) + Huidu_va(1) * (2) + Huidu_va(2) * (3) + Huidu_va(3) * (4) + Huidu_va(4) * (5) + Huidu_va(5) * (6) +
                   Huidu_va(11) * (-1) + Huidu_va(10) * (-2) + Huidu_va(9) * (-3) + Huidu_va(8) * (-4) + Huidu_va(7) * (-5) + Huidu_va(6) * (-6);
        error = Gray_sum * 1.0 * (3000.0 / sum * 1.0);
    } else if (mode == 2) {
        Gray_sum = Huidu_va(0) * (2) + Huidu_va(1) * (2) + Huidu_va(2) * (2) + Huidu_va(3) * (2) + Huidu_va(4) * (3) + Huidu_va(5) * (3) +
                   Huidu_va(11) * (-2) + Huidu_va(10) * (-2) + Huidu_va(9) * (-2) + Huidu_va(8) * (-2) + Huidu_va(7) * (-3) + Huidu_va(6) * (-3);
        error = Gray_sum * 1.0 * (3000.0 / sum * 1.0);
    }

    motorSpeed = KP * error + KD * (error - lastError);
    lastError  = error;

    mSpeed_right = speed + motorSpeed; // 右轮速度
    mSpeed_left  = speed - motorSpeed; // 左轮速度
    lastm1Speed  = mSpeed_right;
    lastm2Speed  = mSpeed_left;
    // 限幅
    if (mSpeed_right < 0) {
        mSpeed_right = 0;
    } else if (mSpeed_right > (1.5 * speed)) {
        mSpeed_right = 1.5 * speed;
    }

    if (mSpeed_left < 0) {
        mSpeed_left = 0;
    } else if (mSpeed_left > (1.5 * speed)) {
        mSpeed_left = 1.5 * speed;
    }
    set_pwm(1, mSpeed_right, speed);
    set_pwm(2, mSpeed_left, speed);
}