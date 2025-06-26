#include "trace.h"

extern int speed;
extern float KP;
extern float KD;

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
            Delay_ms(1);
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
        Delay_ms(1);
    }
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