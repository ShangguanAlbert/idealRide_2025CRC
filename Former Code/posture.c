/**
 * @file posture.c
 * @author sgfz
 * @brief 车辆基础动作
 * @version 0.1
 * @date 2024-09-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "posture.h"

extern uint32_t t3_i;
extern uint8_t cnt_whiteline;

/**
 * @brief 低速下平台
 */
void down_pt1_6(void)
{
    Front_down();
    while (1) {
        slow_run(40);
        if (hdl == 0 || hdr == 0) {
            break;
        }
    }
    while (hwr == 0) {
        slow_run(40);
    }
    Front_mid();
    Reset(60, 45);
    stop();
}
/**
 * @brief 撞景点
 * @param num 景点编号
 */
void Crash_JD(uint8_t num)
{
    if (num == 1 || num == 2) {
        Front_mid();
        while (hwl == 1) {
            slow_run(45);
        }
        Stop(80);
        sound(10);
        run_delay(-45, -45, 150);
        stop();
    } else if (num == 3 || num == 4 || num == 5) {
        Front_mid();
        while (hwl == 1) {
            slow_run(45);
        }
        sound(10);
        Stop(60);
        while (hdr == 1) {
            run(-40, -40);
        }
        stop();
    } else {
        stop();
    }
}
/**
 * @brief 上台2到台6+动作
 *
 */
void UP_Tai2_6(uint8_t Tai_Num)
{
    Human_up();
    Front_down();
    while (hwr == 0) {
        slow_run(50);
    }
    while (1) {
        slow_run(55);
        if (hdl == 0 || hdr == 0) {
            break;
        }
    }
    while (hwl == 1) {
        run(45, 45);
    }
    Stop(30);
    sound(Tai_Num);
    Wave_Hands();
    run_delay(-40, -40, 130);
    Tai1_6_zhuan(Tai_Num);
}
/**
 * @brief 上台7+动作
 *
 */
void UP_Tai7(void)
{
    while (hwr == 0) {
        slow_run(50);
    }
    Reset(100, 70);
    Front_down();
    speed_up(70, 120);
    speed_down(120, 60);
    while (1) {
        slow_run(60);
        if (hdl == 0 || hdr == 0) {
            break;
        }
    }
    while (hwl == 1) {
        run(45, 45);
    }
    Stop(150);
    sound(7);
    Wave_Hands();
    run_delay(-41, -40, 130);
    Tai7_zhuan();
}
/**
 * @brief 上台8+动作
 *
 */
void UP_Tai8(void)
{
    Human_up();
    // 上台8
    while (hwr == 0) {
        slow_run(60);
    }
    Reset(100, 60);
    Front_down();
    // 低斜坡加速
    speed_up(60, 110);
    speed_down(110, 60);
    // 检测衔接平台
    while (1) {
        slow_run(60);
        if (hdl == 0 || hdr == 0) {
            break;
        }
    }
    // 衔接平台盲走
    while (hwr == 0) {
        run(50, 50);
    }
    Reset(100, 60);
    Front_down();
    // 高斜坡加速
    speed_up(60, 105);
    speed_down(105, 60);
    // 检测高平台
    while (1) {
        slow_run(50);
        if (hdl == 0 || hdr == 0) {
            break;
        }
    }
    while (hwl == 1) {
        run(39, 40);
    }
    Stop(150);
    sound(8);
    Wave_Hands();
    run_delay(-35, -40, 140);
    Tai8_zhuan();
}
/**
 * @brief 上台1+动作
 *
 */
void UP_Tai1(void)
{
    Front_down();
    while (hwr == 0) {
        slow_run(50);
    }
    while (1) {
        slow_run(50);
        if (hdl == 0 || hdr == 0) {
            break;
        }
    }
    run_delay(40, 40, 850);
    stop();
    Wave_Hands();
    Tai1_6_zhuan(1);
    sound(1);
    Front_mid();
    Stop(500);
}
/**
 * @brief 过桥
 */
void Bridge_Travel(void)
{
    Front_down();
    while (hwr == 0) {
        slow_run(50);
    }
    // run_delay(50, 50, 850);
    Reset(220, 50);
    Reset_bridge(50, 80, 1);
    Reset_bridge(34, 145, 1);
    Reset_bridge(38, 75, 2);
    while (hwr == 0) {
        slow_run(50);
    }
    // land
    Reset(50, 50);
}
/**
 * @brief 快速过梯形山 不停车
 */
void T_Moutain_Fast(void)
{
    Front_down();
    while (hwr == 0) {
        slow_run(50);
    }
    Stop(50);
    Reset(340, 50);
    Front_mid();
}
/**
 * @brief 慢速过梯形山 停车
 *
 */
void T_Moutain_Slow(void)
{
    Front_down();
    while (hwr == 0) {
        slow_run(50);
    }
    Stop(60);
    Reset(200, 50);
    Stop(60);
    Reset(160, 50);
    Front_mid();
}
/**
 * @brief 进长波浪板
 *
 */
void IN_Long_BLB(void)
{
    Front_mid();
    while (hdr == 1) {
        slow_run(50);
    }
    Human_up();
    Reset(320, 50);
}
/**
 * @brief 出长波浪板
 *
 */
void OUT_Long_BLB(void)
{
    while (hdl == 1) {
        slow_run(50);
    }
    Front_mid();
    Reset(490, 50);
    Human_down();
}
/**
 * @brief 下台8
 *
 */
void Down_Tai8(void)
{
    Front_down();
    // 高斜坡巡线
    Reset(200, 50);
    // 检测衔接平台
    while (hwr == 0) {
        slow_run(50);
    }
    // 衔接平台盲走
    run_delay(40, 40, 400);
    // 检测低斜坡
    while (hdr == 1) {
        slow_run(40);
    }
    // 拉前铲
    Front_mid();
    // 检测绿地
    while (hwr == 0) {
        slow_run(50);
    }
    Human_down();
}
/**
 * @brief 下台7
 *
 */
void Down_Tai7(void)
{
    Reset(200, 50);
    while (hwr == 0) {
        slow_run(50);
    }
    Front_mid();
}
/**
 * @brief 停止线
 *
 */
void Stop_Line(void)
{
    while (1) {
        slow_run(60);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
}
/**
 * @brief 过短波浪板
 *
 */
void Pass_Short_BLB(void)
{
    while (hdl == 1) {
        slow_run(50);
    }
    Reset(240, 50);
    Human_up();
}
/**
 * @brief 离开景点4检测岔路口
 *
 */
void Detect_OUT_jd4(void)
{
    while (1) {
        slow_run(60);
        if (Huidu_va(0) > white || Huidu_va(1) > white || Huidu_va(2) > white) {
            break;
        }
    }
}

void Drift_Rightpass_TMountain(void)
{
    Front_mid();
    Reset(30, 60);
    while (hwr == 0) {
        drift_right(60, 0);
    }
    Front_down();
    Stop(60);
    Reset(170, 50);
    Stop(60);
    Reset(140, 50);
    Front_mid();
}
/**
 * @brief 检测到跷跷板并调整姿态到左色标扫到
 *
 */
void Touch_Seesaw_adjust(void)
{
    while (1) {
        drift_left(60, 0);
        if (hwr != 0) break;
    }
    Front_down();
    Stop(40);
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    // while ((bhwl == 1) || t3_i > 180) {
    //     run(0, 45);
    // }
    while (1) {
        run(-30, 0);
        if (bhwr == 0) {
            break;
        } else if (t3_i > 75) { // 没扫到
            while (1) {
                run(-35, 10);
                if (bhwl == 0 || t3_i > 180) break;
            }
            break;
        } else if (t3_i > 120) { // 没扫到且偏了
            while (1) {
                run(40, -40);
                if (bhwr == 0) break;
            }
            break;
        }
    }
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
    Stop(40);
}
/**
 * @brief 上跷跷板修正调整
 *
 * @param time_stop 后半程开始降速时间
 * @param time_Seesaw 总的跷跷板时间
 */
void Past_Seesaw_adjust(int time_stop, int time_Seesaw)
{
    int count_turn = 0;
    t3_i           = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    while (1) {
        Run(65);
        /*循环修正*/
        if ((bhwr == 0 && bhwl == 1) || Huidu_va(0) > 80) {
            run(40, 70);
            delay_ms(5);
            count_turn += 3;
        } else if ((bhwl == 0 && bhwr == 1) || Huidu_va(11) > 80) {
            run(70, 40);
            delay_ms(5);
            count_turn += 3;
        } else if ((bhwl == 1 && bhwr == 1) || (Huidu_va(0) > 90 && Huidu_va(11) > 90)) {
            Run(65);
        }
        if (t3_i > time_stop) {
            run(25, 35);
            delay_ms(5);
        }
        // /* 超时停车 */
        // if (t2_i >= (time_Seesaw + count_turn)) {
        //     stop();
        //     break;
        // }
        /* 下跷跷板停车 红外检测到 */
        if (hwr != 0 && t3_i > time_stop) {
            Front_mid();
            stop();
            break;
        }
        /* 检测到落地点有白线停车 */
        get_huidu_va();
        if (cnt_whiteline > 0 && (t3_i >= (time_Seesaw + count_turn))) {
            stop();
            break;
        }
    }
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
    Stop(100);
}
/**
 * @brief 跷跷板落地扫不到白线保护程序
 *
 */
void Land_Protect_adjust(void)
{
    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    while (1) {
        get_huidu_va();
        if (cnt_whiteline != 0) {
            Reset_drift_left(60, 0, 100);
            break;
        } else if (cnt_whiteline == 0) {
            while (1) {
                run(0, 50);
                if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white || Huidu_va(7) > white) break;
            }
            Stop(150);
            break;
        }
    }
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    t3_i = 0;
    while (1) {
        run(0, 40);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}