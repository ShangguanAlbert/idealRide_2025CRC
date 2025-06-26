/**
 * @file trace.c
 * @author sgfz
 * @brief PD权值巡线 桥上巡线 PD加减速 三角形加减速
 * @version 0.1
 * @date 2024-09-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "trace.h"

int Gray_sum;
int speed;
int lastError;
int motorSpeed;
int lastm1Speed; // 上次速度
int lastm2Speed;
int mSpeed_right;
int mSpeed_left;
uint8_t cnt_whiteline;
double sum = 0;
float error;
float KP;
float KD;

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
 * @brief PD权值法巡线+干扰线条件+限幅
 * @note 部分条件维持原判
 */
void Trace_new(void)
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
 * @brief 高速巡线, 速度可调范围90 - 220
 * @param N 设定速度
 */
void high_run(int N)
{
    get_huidu_va();
    speed = N;
    // if (speed > 90 && speed < 100) {
    //     KP = 0.00413;
    //     KD = 0.005565;
    // } else if (speed >= 100 && speed < 120) {
    //     KP = 0.0020;
    //     KD = 0.0264;
    // } else if (speed >= 120 && speed <= 140) {
    //     KP = 0.00205;
    //     KD = 0.0264;
    // } else if (speed > 140 && speed < 160) {
    //     KP = 0.0021;
    //     KD = 0.0264;
    // } else if (speed >= 160 && speed <= 180) {
    //     KP = 0.0023;
    //     KD = 0.0264;
    // } else if (speed > 180 && speed <= 200) {
    //     KP = 0.0025;
    //     KD = 0.0264;
    // } else if (speed > 200 && speed <= 220) {
    //     KP = 0.00251;
    //     KD = 0.0315;
    // }
    if (speed > 90 && speed < 100) {
        KP = 0.00413;  // p大会抽会扭
        KD = 0.005565; // d小回到白线的速度就慢
    } else if (speed >= 100 && speed < 110) {
        KP = 0.0020;
        KD = 0.0264;
    } else if (speed >= 110 && speed < 120) {
        KP = 0.001988;
        KD = 0.02644;
    } else if (speed >= 120 && speed < 130) {
        KP = 0.0019889;
        KD = 0.02644;
    } else if (speed >= 130 && speed < 140) {
        KP = 0.0019845;
        KD = 0.026422;
    } else if (speed >= 140 && speed < 150) {
        KP = 0.001984;
        KD = 0.02643;
    } else if (speed >= 150 && speed < 160) {
        KP = 0.001980;
        KD = 0.02643;
    } else if (speed >= 160 && speed < 170) {
        KP = 0.0019808;
        KD = 0.026441;
    } else if (speed >= 170 && speed < 180) {
        KP = 0.00197;
        KD = 0.026477;
    } else if (speed >= 180 && speed < 190) {
        KP = 0.002315;
        KD = 0.0264;
    } else if (speed >= 190 && speed < 200) {
        KP = 0.002360;
        KD = 0.02653;
    } else if (speed >= 200 && speed < 210) {
        KP = 0.002345;
        KD = 0.026555;
    } else if (speed >= 210 && speed <= 220) {
        KP = 0.00166;
        KD = 0.03052; // 0.03045
    }
    Trace();
}
/**
 * @brief 新高速巡线, 速度可调范围90 - 220
 * @param N 设定速度
 */
void high_run_new(int N)
{
    get_huidu_va();
    speed = N;
    if (speed > 90 && speed < 100) {
        KP = 0.00413;  // p大会抽会扭
        KD = 0.005565; // d小回到白线的速度就慢
    } else if (speed >= 100 && speed < 110) {
        KP = 0.0020;
        KD = 0.0264;
    } else if (speed >= 110 && speed < 120) {
        KP = 0.001988;
        KD = 0.02644;
    } else if (speed >= 120 && speed < 130) {
        KP = 0.0019889;
        KD = 0.02644;
    } else if (speed >= 130 && speed < 140) {
        KP = 0.0019845;
        KD = 0.026422;
    } else if (speed >= 140 && speed < 150) {
        KP = 0.001984;
        KD = 0.02643;
    } else if (speed >= 150 && speed < 160) {
        KP = 0.001980;
        KD = 0.02643;
    } else if (speed >= 160 && speed < 170) {
        KP = 0.0019808;
        KD = 0.026441;
    } else if (speed >= 170 && speed < 180) {
        KP = 0.00197;
        KD = 0.026477;
    } else if (speed >= 180 && speed < 190) {
        KP = 0.002315;
        KD = 0.0264;
    } else if (speed >= 190 && speed < 200) {
        KP = 0.002360;
        KD = 0.02653;
    } else if (speed >= 200 && speed < 210) {
        KP = 0.002345;
        KD = 0.026555;
    } else if (speed >= 210 && speed <= 220) {
        KP = 0.00166;
        KD = 0.03052; // 0.03045
    }
    Trace_new();
}

/**
 * @brief 低速巡线, 速度可调范围30 - 100
 * @param N 设定速度
 */
void slow_run(int N)
{
    get_huidu_va();
    speed = N;
    if (speed < 50) {
        KP = 0.0077;
        KD = 0.05;
    } else if (speed >= 50 && speed < 60) { // 50
        KP = 0.0070;
        KD = 0.005;
    } else if (speed >= 60 && speed < 70) { // 60
        KP = 0.0045;
        KD = 0.005;
    } else if (speed >= 70 && speed < 80) { // 70
        KP = 0.00428;
        KD = 0.0055;
    } else if (speed >= 80 && speed < 90) { // 80
        KP = 0.00425;
        KD = 0.005565;
    } else { // 90
        KP = 0.00413;
        KD = 0.005565;
    }
    Trace();
}
void speed_up(int start, int end)
{
    KP = 0.001;
    KD = 0.025;
    for (; start < end; start++) {
        if (speed < 100) {
            KP = 0.007;
            KD = 0.018;
        } else {
            KP = 0.003;
            KD = 0.08;
        }
        speed = start;
        get_huidu_va();
        Trace();
        if (start % 10 == 0) {
            delay_ms(1);
        }
    }
}

void speed_up_new(int start, int end)
{
    KP = 0.001;
    KD = 0.025;
    for (; start < end; start++) {
        if (speed < 100) {
            KP = 0.007;
            KD = 0.018;
        } else {
            KP = 0.003;
            KD = 0.08;
        }
        speed = start;
        get_huidu_va();
        Trace();
        if (start % 10 == 0) {
            delay_ms(1);
        }
    }
}
/**
 * @brief 老减速 使用速度50-190
 *
 * @param high 最大速度
 * @param low 最小速度
 */
void speed_down(int high, int low)
{
    for (; high > low; high--) {
        if (speed < 50) {
            KP = 0.004;  // 0.005
            KD = 0.0025; // 0.05
        } else if (speed >= 50 && speed < 70) {
            KP = 0.00197; // 0.025
            KD = 0.0130;
        } else if (speed >= 70 && speed <= 85) {
            KP = 0.00186;
            KD = 0.0130;
        } else if (speed >= 86 && speed < 100) {
            KP = 0.00172;
            KD = 0.0140;
        } else if (speed >= 100 && speed < 140) {
            KP = 0.0016;
            KD = 0.0160;
        } else if (speed >= 140 && speed < 160) {
            KP = 0.00157;
            KD = 0.0180;
        } else if (speed >= 160 && speed <= 180) {
            KP = 0.00154;
            KD = 0.0200;
        } else if (speed > 180 && speed <= 200) {
            KP = 0.00148;
            KD = 0.0255;
        } else if (speed > 200 && speed <= 220) {
            KP = 0.00055;
            KD = 0.04;
        }
        speed = high;
        get_huidu_va();
        if (high % 5 == 0) Trace();
        delay_ms(1);
    }
}
/**
 * @brief 新减速 使用速度50-220
 *
 * @param high 最大速度
 * @param low 最小速度
 */
void speed_down_new(int high, int low)
{
    for (; high > low; high--) {
        if (speed < 50) {
            KP = 0.004;  // 0.005
            KD = 0.0025; // 0.05
        } else if (speed >= 50 && speed < 70) {
            KP = 0.00197; // 0.025
            KD = 0.0130;
        } else if (speed >= 70 && speed <= 85) {
            KP = 0.00186;
            KD = 0.0130;
        } else if (speed >= 86 && speed < 100) {
            KP = 0.00172;
            KD = 0.0140;
        } else if (speed >= 100 && speed < 140) {
            KP = 0.0016;
            KD = 0.0160;
        } else if (speed >= 140 && speed < 160) {
            KP = 0.00157;
            KD = 0.0180;
        } else if (speed >= 160 && speed <= 180) {
            KP = 0.00154;
            KD = 0.0200;
        } else if (speed > 180 && speed <= 200) {
            KP = 0.00148;
            KD = 0.0255;
        } else if (speed > 200 && speed <= 220) {
            KP = 0.00055;
            KD = 0.04;
        }
        speed = high;
        get_huidu_va();
        if (high % 5 == 0) Trace();
        delay_ms(1);
    }
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
/**
 * @brief 桥上巡线
 *
 * @param N 速度
 * @param mode 模式,不同权值
 */
void bridge_PD(int N, uint8_t mode)
{
    get_huidu_va();
    speed = N;
    if (mode == 1) {
        KP = 0.015;
        KD = 0.008;
    } else if (mode == 2) {
        KP = 0.009;
        KD = 0.2;
    }
    bridge_Trace(mode);
}