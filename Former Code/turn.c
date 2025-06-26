/**
 * @file turn.c
 * @author sgfz
 * @brief 转向
 * @version 0.1
 * @date 2024-09-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "turn.h"

/*指南针平台掉头*/

/**
 * @brief 低平台掉头转180
 */
void Tai1_6_zhuan(uint8_t Tai_Num)
{
    Stop(100);
    HWT101_to_0();
    Front_up_High();
    Stop(250);
    Deg_IN();
    if (Tai_Num == 1) {
        for (int x = 50; x < 100; x++) {
            run(x * 1.0, -x * 0.96);
            delay_ms(8);
        }
        for (int x = 50; x > 100; x--) {
            run(x * 1.0, -x * 0.96);
            delay_ms(8);
        }
        Front_down();
        pid_Turn(195);
    } else if (Tai_Num == 2 || Tai_Num == 3 || Tai_Num == 4 || Tai_Num == 5 || Tai_Num == 6) {
        for (int x = 50; x < 105; x++) {
            run(x * 1.0, -x * 0.96);
            delay_ms(8);
        }
        for (int x = 50; x > 105; x--) {
            run(x * 1.0, -x * 0.96);
            delay_ms(8);
        }
        Front_down();
        pid_Turn(195);
    }
    Stop(100);
}
/**
 * @brief 中平台掉头转180
 *
 */
void Tai7_zhuan(void)
{
    Front_up_High();
    Stop(100);
    HWT101_to_0();
    Stop(250);
    Deg_IN();
    for (int x = 40; x < 95; x++) {
        run(x * 1.04, -x * 0.93);
        delay_ms(8);
    }
    for (int x = 40; x > 95; x--) {
        run(x * 1.04, -x * 0.93);
        delay_ms(8);
    }
    Front_down();
    pid_Turn(208);
    stop();
}
/**
 * @brief 高平台掉头转180
 *
 */
void Tai8_zhuan(void)
{
    Front_up_High();
    Stop(100);
    HWT101_to_0();
    Stop(250);

    Deg_IN();
    for (int x = 40; x < 95; x++) {
        run(x * 1.03, -x * 1.00);
        delay_ms(9);
    }
    for (int x = 40; x > 95; x--) {
        run(x * 1.03, -x * 1.00);
        delay_ms(9);
    }
    Front_down();
    pid_Turn(220);
    stop();
}

/**
 * @brief 右转90度
 */
void TurnRight_90_Rdetect(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(0) > white || Huidu_va(1) > white || Huidu_va(2) > white) {
            break;
        }
    }
    Left_Speed_Up(50, 90, 5);
    Right_Speed_Down(50, -85, 5);
    while (1) {
        run(68, -75);
        if (Huidu_va(6) > white || Huidu_va(5) > white) {
            break;
        }
    }
}
/**
 * @brief 右转90度进景点
 *
 */
void TurnRightJD_90(void)
{
    // 检测右转
    while (1) {
        slow_run(50);
        if (Huidu_va(0) > white || Huidu_va(1) > white || Huidu_va(2) > white) {
            break;
        }
    }
    // 右转
    Left_Speed_Up(50, 96, 5);
    Right_Speed_Down(50, -90, 5);
    while (1) {
        run(70, -73);
        if (Huidu_va(4) > white || Huidu_va(5) > white) {
            break;
        }
    }
}

/**
 * @brief 左转90度 右灰度检测
 *
 */
void TurnLeft_90_Rdetect(void)
{
    // 检测左转
    while (1) {
        slow_run(50);
        if (Huidu_va(11) > white || Huidu_va(10) > white || Huidu_va(9) > white) {
            break;
        }
    }
    // 左转
    Right_Speed_Up(50, 95, 5);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-75, 63);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
}

/**
 * @brief 左转90度 左灰度检测
 *
 */
void TurnLeft_90_Ldetect(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(0) > white || Huidu_va(1) > white || Huidu_va(2) > white) {
            break;
        }
    }
    Right_Speed_Up(50, 90, 5);
    Left_Speed_Down(50, -85, 5);
    while (1) {
        run(-75, 65);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
}

/**
 * @brief 左转大角度 左灰度检测
 */
void TurnLeft_155(void)
{
    // 下8平台转
    while (Huidu_va(10) < white) {
        slow_run(50);
    }
    Right_Speed_Up(50, 120, 4);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-70, 86);
        if (Huidu_va(8) > white || Huidu_va(9) > white) {
            break;
        }
    }
}

/**
 * @brief 原地差速左转
 */
void TurnLeft_Place_90(void)
{
    while (1) {
        run(-60, 60);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    while (1) {
        run(-55, 50);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}

/**
 * @brief 景点1 2掉头
 *
 */
void JD1_2_TurnAround(void)
{
    turnL_speed_up(0, 80, 3);
    while (1) {
        run(-80, 82);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    while (1) {
        run(-55, 50);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}
/**
 * @brief 转门
 *
 */
void Door_TurnAround(void)
{
    turnL_speed_up(0, 80, 3);
    while (1) {
        run(-80, 70);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    while (1) {
        run(-55, 50);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}

/**
 * @brief 左转进景点1
 *
 */
void TurnLeft_inJD1(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    Right_Speed_Up(50, 120, 4);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-70, 80);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
}

/**
 * @brief 右转上梯形山
 *
 */
void TurnRight_45_TXS(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(0) > white || Huidu_va(1) > white) {
            break;
        }
    }
    // 右转
    Right_Speed_Down(50, -90, 5);
    Left_Speed_Up(50, 90, 5);
}

/**
 * @brief 左转进长直线
 *
 */
void TurnLeft_45_Longline(void)
{
    // while (1) {
    //     slow_run(50);
    //     if (Huidu_va(11) > white || Huidu_va(10) > white) {
    //         break;
    //     }
    // }
    // while (hdr == 1) {
    //     slow_run(50);
    // }
    // Stop(10);
    // while (1) {
    //     run(-45, 55);
    //     if (Huidu_va(5) > white || Huidu_va(6) > white) {
    //         break;
    //     }
    // }
    while (1) {
        slow_run(50);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    while (hdl == 1) {
        drift_left(60, 0);
    }
}

/**
 * @brief 左转进景点2
 *
 */
void TurnLeft_inJD2(void)
{
    while (1) {
        slow_run(55);
        if (qhdr == 0) {
            break;
        }
    }
    // 左转
    Left_Speed_Down(55, -85, 9);

    while (1) {
        run(-45, 55);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}

/**
 * @brief 景点1顺拐进门2
 *
 */
void Small_Turn_inDoor2(void)
{
    while (1) {
        slow_run(50);
        if (hdr == 0) break;
    }
    // Right_Speed_Down(50,40,3);
    Left_Speed_Up(50, 75, 4);
    delay_ms(350);
    Reset(60, 60);
}
/**
 * @brief 转45度进台5直线
 *
 */
void TurnLeft_45_inTai5(void)
{
    while (qhdl == 1) {
        slow_run(60);
    }
    Left_Speed_Down(60, -90, 6);
    Stop(30);
    /*
    备用方案
    while (1) {
            slow_run(60);
            if (Huidu_va(10) > white || Huidu_va(11) > white) break;
        }
        while (1) {
            slow_run(50);
            if (Huidu_va(10) < white && Huidu_va(11) < white) break;
        }
        Right_Speed_Up(50, 70, 3);
        Left_Speed_Down(50, 30, 3);
        delay_ms(180);
        while (1) {
            run(-30, 70);
            if (Huidu_va(5) > white || Huidu_va(6) > white) break;
        }
    */
}
/**
 * @brief 原地差速右转
 */
void TurnRight_Place_90(void)
{
    while (1) {
        run(60, -60);
        if (Huidu_va(1) > white || Huidu_va(0) > white) {
            break;
        }
    }
    while (1) {
        run(50, -55);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
}
/**
 * @brief 右转45度进交叉口
 */
void TurnRight_45_inCross(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(0) > white || Huidu_va(1) > white || Huidu_va(2) > white) {
            break;
        }
    }
    while (hdr == 1) {
        drift_right(60, 0);
    }
}

void TurnLeft_155_atCross(void)
{
    while (Huidu_va(10) < white) {
        slow_run(50);
    }
    Right_Speed_Up(50, 110, 4);
    Left_Speed_Down(50, -80, 5);
    while (1) {
        run(-70, 90);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
}
/**
 * @brief 门2右转大角度出门区进入长直线
 *
 */
void TurnRight_155_outCross(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(1) > white || Huidu_va(0) > white) {
            break;
        }
    }
    Left_Speed_Up(50, 120, 4);
    Right_Speed_Down(50, -90, 5);
    while (1) {
        run(90, -70);
        if (Huidu_va(4) > white || Huidu_va(5) > white) {
            break;
        }
    }
}
/**
 * @brief 门12转入波浪板
 *
 */
void door12_Turn_BLB(void)
{
    while (qhdl == 1) {
        slow_run(60);
    }
    while (hdr == 1) {
        drift_left(60, 0);
    }
}

void TurnRight_45_Tai1(void)
{
    while (hdl == 1) {
        drift_right(60, 0);
    }
}

void TurnRight_90_Ldetect(void)
{

    while (1) {
        slow_run(50);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    Left_Speed_Up(50, 96, 5);
    Right_Speed_Down(50, -85, 5);
    while (1) {
        run(70, -74);
        if (Huidu_va(4) > white || Huidu_va(5) > white) {
            break;
        }
    }
}

void TurnRight_90_hdrDetect(void)
{
    while (hdr == 1) {
        slow_run(50);
    }

    Right_Speed_Down(50, -80, 6);
    Left_Speed_Up(50, 75, 5);

    while (1) {
        run(65, -50);
        if (Huidu_va(6) > white || Huidu_va(5) > white) {
            break;
        }
    }
}

void TurnLeft_155_hdlDetect(void)
{
    while (hdl == 1) {
        slow_run(50);
    }
    Left_Speed_Down(50, -90, 5);
    Right_Speed_Up(50, 85, 4);

    while (1) {
        run(-60, 65);
        if (Huidu_va(0) > white || Huidu_va(1) > white) {
            break;
        }
    }
    while (1) {
        run(-60, 70);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}

void TurnLeft_Door1_inCross(void)
{
    while (hdl == 1) {
        slow_run(50);
    }
    Left_Speed_Down(50, -90, 5);
    Right_Speed_Up(50, 85, 4);

    while (1) {
        run(-60, 65);
        if (Huidu_va(10) > white || Huidu_va(11) > white) {
            break;
        }
    }
    while (1) {
        run(-60, 65);
        if (Huidu_va(0) > white || Huidu_va(1) > white) {
            break;
        }
    }
    while (1) {
        run(-60, 70);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
}

void door34_Turn_BLB(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(1) > white || Huidu_va(0) > white) {
            break;
        }
    }
    // while (1) {
    //     slow_run(50);
    //     if (qhdr == 0) {
    //         break;
    //     }
    // }
    Left_Speed_Up(50, 105, 6);
    Right_Speed_Down(50, -90, 7);
    while (1) {
        run(75, -75);
        if (Huidu_va(4) > white || Huidu_va(5) > white) {
            break;
        }
    }
}

void TurnLeft_155_door4(void)
{
    while (1) {
        slow_run(50);
        if (Huidu_va(0) > white || Huidu_va(1) > white) {
            break;
        }
    }
    Right_Speed_Up(50, 110, 5);
    Left_Speed_Down(50, -90, 6);

    while (1) {
        run(-70, 90);
        if (Huidu_va(6) > white || Huidu_va(5) > white) {
            break;
        }
    }
}

void TurnRight_inDoor3(void)
{
    while (1) {
        slow_run(60);
        if (hdr == 0) {
            break;
        }
    }
    Stop(50);
    turnR_speed_up(60, 100, 20);
    turnR_speed_down(100, 60, 20);
    while (1) {
        run(60, -60);
        if (Huidu_va(5) > white || Huidu_va(6) > white) break;
    }
}

void Small_Turn_toDoor34(void)
{
    // while (1) {
    //     slow_run(60);
    //     if (Huidu_va(11) > white || Huidu_va(10) > white) {
    //         break;
    //     }
    // }
    // // 左转
    // run_delay(-50, 60, 160);
    // // 停车
    // Stop(50);
    while (hdr == 1) {
        slow_run(60);
    }
    Reset_drift_left(60, 0, 500);
}

void TurnLeft_155_Tai4(void)
{
    while (Huidu_va(10) < white) {
        slow_run(50);
    }
    Right_Speed_Up(50, 110, 5);
    Left_Speed_Down(50, -80, 6);
    while (1) {
        run(-70, 80);
        if (Huidu_va(8) > white || Huidu_va(7) > white) {
            break;
        }
    }
}
void drift_right_jd1_door2(int speed, int model)
{
    while (1) {
        slow_run(50);
        if (hdr == 0) {
            break;
        }
    }
    while (1) {
        if (Huidu_va(10) > white) {
            run(speed + 15, 0);
        } else if (Huidu_va(11) > white) {
            run(speed + 15, 10);
        } else if (Huidu_va(8) > white) {
            run(speed + 15, 20);
        } else if (Huidu_va(9) > white) {
            run(speed + 15, 30);
        } else if (Huidu_va(7) > white) {
            run(speed + 15, 30);
        } else {
            if (model == 0)
                slow_run(speed);
            else
                high_run(speed);
        }
        if (hdl == 0) {
            break;
        }
    }
}