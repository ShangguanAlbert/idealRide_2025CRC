/**
 * @file route.c
 * @author sgfz
 * @brief 平台间完整路线
 * @version 0.1
 * @date 2024-09-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "route.h"

extern uint32_t t3_i;
extern uint32_t t6_i;
extern uint8_t cnt_whiteline;

/**
 * @brief 门2转门1
 *
 */
void door2_door1(void)
{
    // 后退
    run_delay(-40, -40, 200);
    // 掉头
    Door_TurnAround();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 左转进门1
    // TurnLeft_155();
    // 下8平台转
    while (Huidu_va(10) < white) {
        slow_run(50);
    }
    Right_Speed_Up(50, 110, 5);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-70, 80);
        if (Huidu_va(8) > white || Huidu_va(9) > white) {
            break;
        }
    }

    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 115);
    speed_down(115, 60);
    // 停止线
    Stop_Line();
}
/**
 * @brief 门1转门4
 *
 */
void door1_door4(void)
{
    // 后退
    run_delay(-40, -40, 190);
    // 掉头
    Door_TurnAround();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 右转
    TurnRight_90_Ldetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 185);
    speed_down(185, 60);
    // 右转
    TurnRight_90_hdrDetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 110);
    speed_down(110, 60);
    // 停止线
    Stop_Line();
}
/**
 * @brief 门4转门3
 *
 */
void door4_door3(void)
{
    // 后退
    run_delay(-40, -40, 180);
    // 掉头
    Door_TurnAround();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 左转
    TurnLeft_155_hdlDetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 停止线
    Stop_Line();
}
/**
 * @brief 平台1到平台2
 */
void Tai1_Tai2(void)
{
    // 下台
    down_pt1_6();
    // 加速
    speed_up(50, 120);
    speed_down(120, 60);
    // 矫正
    Reset(50, 60);
    // 过桥
    Bridge_Travel();
    // 加速
    speed_up(50, 115);
    speed_down(115, 60);
    // 上台2
    UP_Tai2_6(2);
}
/**
 * @brief 台2到景点2到台4
 *
 */
void Tai2_jd2_Tai4(void)
{
    // 下台2
    down_pt1_6();
    // 拉前铲
    Drift_Rightpass_TMountain();
    // 加速
    speed_up(60, 90);
    speed_down(90, 60);
    // 检测左转
    TurnLeft_45_Longline();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 200);
    speed_down(200, 60);
    // 检测左转进景点2
    TurnLeft_inJD2();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 撞景点2
    Crash_JD(2);
    // 掉头
    JD1_2_TurnAround();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 130);
    speed_down(130, 60);
    // 左转上台4
    TurnLeft_155_Tai4();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 降前铲
    Front_down();
    // 上台2
    UP_Tai2_6(4);
}
/**
 * @brief 台2到台4到景点2
 *
 */
void Tai2_Tai4_jd2_Tai3(void)
{
    // 下台2
    down_pt1_6();
    // 拉前铲
    Drift_Rightpass_TMountain();
    // 加速
    speed_up(60, 90);
    speed_down(90, 60);
    Human_down();
    // 检测左转
    TurnLeft_45_Longline();
    // Stop(40);
    // 矫正
    Reset(70, 60);
    // 加速
    speed_up(60, 220);
    Reset_Aid(10, 220);
    speed_down(220, 60);
    UP_Tai2_6(4);

    down_pt1_6();
    speed_up(60, 100);
    speed_down(100, 60);
    Human_down();
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
    Stop(40);
    Reset(60, 60);
    // 加速
    speed_up(60, 140);
    speed_down(140, 60);
    // 撞景点2
    Crash_JD(2);
    // 掉头
    JD1_2_TurnAround();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 125);
    speed_down(125, 60);
    while (1) {
        slow_run(60);
        if (Huidu_va(11) > white || Huidu_va(10) > white) {
            break;
        }
    }
    while (hdl == 1) {
        drift_right(60, 0);
    }
    Reset_drift_right(60, 0, 50);
    Stop(30);
    Reset(60, 60);
    speed_up(60, 220);
    Reset_Aid(190, 220);
    speed_down(220, 60);
    UP_Tai2_6(3);
}

/**
 * @brief 台4到台3
 *
 */
void Tai4_Tai3(void)
{
    // 下台4
    down_pt1_6();
    // 加速
    speed_up(60, 220);
    Reset(270, 220);
    speed_down(220, 60);
    Front_down();
    // 上台3
    UP_Tai2_6(3);
}

void Tai4_Tai3_new(void)
{
    // 下台4
    down_pt1_6();
    // 加速
    speed_up(60, 220);
    LED_ON;
    Reset_Aid(260, 220);
    LED_OFF;
    speed_down(220, 60);
    Front_down();
    // 上台3
    UP_Tai2_6(3);
}
/**
 * @brief 台3到景点1转出加速 未进门区 一二趟通用
 *
 */
void Tai3_jd1_OUT(void)
{
    // 下台3
    down_pt1_6();
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    Human_down();
    // 左转进景点1
    TurnLeft_inJD1();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 195);
    speed_down(195, 60);
    // 撞景点1
    Crash_JD(1);
    // 掉头
    JD1_2_TurnAround();
    // 停一下
    Stop(50);
    // 矫正
    Reset(70, 60);
    // 加速
    speed_up(60, 190);
    speed_down(190, 70);
}
/**
 * @brief 台3到景点1到门1
 *
 */
void Tai3_jd1_door1(void)
{
    Tai3_jd1_OUT();
    while (hdr == 1) {
        slow_run(60);
    }
    while (1) {
        slow_run(60);
        if (Huidu_va(0) > white || Huidu_va(1) > white) {
            break;
        }
    }
    Left_Speed_Up(60, 90, 6);
    Right_Speed_Down(60, -90, 5);
    while (1) {
        run(80, -60);
        if (Huidu_va(0) > white || Huidu_va(1) > white) {
            break;
        }
    }
    while (1) {
        run(70, -60);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
    Reset(60, 70);
    speed_up(70, 110);
    speed_down(110, 60);
    Stop_Line();
}
/**
 * @brief 台3到景点1到门2
 *
 */
void Tai3_jd1_door2(void)
{
    // 出景点进门区前
    Tai3_jd1_OUT();
    // 顺拐进门2
    Small_Turn_inDoor2();
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 停止线
    Stop_Line();
}
/**
 * @brief 台3到景点1到门3
 *
 */
void Tai3_jd1_door3(void)
{
    // 出景点1
    Tai3_jd1_OUT();
    // 检测左转进长直线
    Small_Turn_toDoor34();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 187);
    speed_down(187, 60);
    // 右转进门3
    TurnRight_inDoor3();
    Stop(30);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 停止线
    Stop_Line();
}
/**
 * @brief 台3到景点1到门4
 *
 */
void Tai3_jd1_door4(void)
{
    // 出景点1
    Tai3_jd1_OUT();
    // 检测左转进长直线
    Small_Turn_toDoor34();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 187);
    speed_down(187, 60);
    // 右转
    TurnRight_90_hdrDetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 110);
    speed_down(110, 60);
    // 停止线
    Stop_Line();
}

void Tai3_jd1_door1_Tai5(void)
{
    Tai3_jd1_OUT();
    while (hdr == 1) {
        slow_run(60);
    }
    while (hdl == 1) {
        drift_right(60, 0);
    }
    Reset_drift_right(60, 0, 50);
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 左转
    TurnLeft_Door1_inCross();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(30);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}

void Tai3_jd1_door2_Tai5(void)
{
    Tai3_jd1_OUT();
    // 顺拐进门2
    Small_Turn_inDoor2();
    Reset(60, 60);
    speed_up(60, 210);
    speed_down(210, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}

void Tai3_jd1_door3_Tai5(void)
{
    Tai3_jd1_OUT();
    while (hdr == 1) {
        slow_run(60);
    }
    Reset_drift_left(60, 0, 75);
    Stop(30);
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 右转进门3
    TurnRight_inDoor3();
    Stop(40);
    Reset(60, 60);
    speed_up(60, 145);
    speed_down(145, 60);
    // 左转
    while (Huidu_va(10) < white) {
        slow_run(50);
    }
    Right_Speed_Up(50, 110, 5);
    Left_Speed_Down(50, -80, 5);
    while (1) {
        run(-70, 65);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
    Stop(40);
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}

void Tai3_jd1_door4_Tai5(void)
{
    Tai3_jd1_OUT();
    while (hdr == 1) {
        slow_run(60);
    }
    Reset_drift_left(60, 0, 75);
    Stop(30);
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 右转
    TurnRight_90_hdrDetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 165);
    speed_down(165, 60);
    // 左转
    TurnLeft_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(60, 50);
    // 加速
    speed_up(50, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}

/**
 * @brief 门1到台5
 *
 */
void door1_Tai5(void)
{
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 115);
    speed_down(115, 60);
    // 左转
    TurnLeft_Door1_inCross();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(30);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}
/**
 * @brief 门2到台5
 *
 */
void door2_Tai5(void)
{
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    Reset(10, 180);
    speed_down(180, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 上台5
    UP_Tai2_6(5);
}
/**
 * @brief 门3到台5
 *
 */
void door3_Tai5(void)
{
    // 矫正
    Reset(50, 50);
    // 加速
    speed_up(50, 90);
    speed_down(90, 50);
    // 左转
    TurnLeft_155_atCross();
    Stop(40);
    Reset(60, 60);
    // 加速
    speed_up(60, 160);
    speed_down(160, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}
/**
 * @brief 门4到台5
 *
 */
void door4_Tai5(void)
{
    // 矫正
    Reset(50, 50);
    // 加速
    speed_up(50, 120);
    speed_down(120, 50);
    // 左转
    TurnLeft_90_Rdetect();
    Stop(30);
    // 矫正
    Reset(60, 50);
    // 加速
    speed_up(50, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}
/**
 * @brief 门4转门3冲台5 不需要判最后一个门
 *
 */
void door4_door3_Tai5(void)
{
    // 后退
    run_delay(-40, -40, 150);
    // 掉头
    Door_TurnAround();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 左转
    TurnLeft_155_hdlDetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 左转
    // TurnLeft_155_atCross();
    while (Huidu_va(10) < white) {
        slow_run(50);
    }
    Right_Speed_Up(50, 110, 5);
    Left_Speed_Down(50, -80, 5);
    while (1) {
        run(-70, 65);
        if (Huidu_va(6) > white || Huidu_va(7) > white) {
            break;
        }
    }
    Stop(40);
    Reset(60, 60);
    // 加速
    speed_up(60, 150);
    speed_down(150, 60);
    // 转进景点5
    TurnLeft_45_inTai5();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 180);
    speed_down(180, 60);
    // 放前铲
    Front_down();
    // 上台5
    UP_Tai2_6(5);
}

/**
 * @brief 台5到景点5到台7
 *
 */
void Tai5_jd5_Tai7(void)
{
    // 下台5
    down_pt1_6();
    // 加速
    speed_up(50, 185);
    speed_down(185, 50);
    Human_down();
    // 左转
    TurnLeft_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(50, 50);
    // 右转进景点5
    TurnRightJD_90();
    // 撞景点5
    Crash_JD(5);
    // 左转
    TurnLeft_Place_90();
    // 矫正
    Reset(70, 60);
    // 左转
    TurnLeft_90_Ldetect();
    Stop(45);
    // 矫正
    Reset(60, 50);
    // 加速
    speed_up(50, 135);
    speed_down(135, 50);
    // 梯形山
    T_Moutain_Slow();
    // 加速
    Reset(30, 60);
    speed_up(60, 110);
    speed_down(110, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    while (hwr == 0) {
        slow_run(60);
    }
    Reset(180, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 135);
    speed_down(135, 50);
    // 梯形山
    T_Moutain_Slow();
    // 加速
    speed_up(60, 135);
    speed_down(135, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(80, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 100);
    speed_down(100, 60);
    // 长波浪板
    IN_Long_BLB();
    Reset(70, 60);
    // 上台7
    UP_Tai7();
}
/**
 * @brief 台7到台8
 *
 */
void Tai7_Tai8(void)
{
    // 下坡
    Front_down();
    Down_Tai7();
    // 长波浪板
    OUT_Long_BLB();
    // 加速
    speed_up(60, 140);
    speed_down(140, 50);
    // 梯形山
    T_Moutain_Slow();
    // 速度复位
    Reset(50, 60);
    // 加速
    speed_up(60, 135);
    speed_down(135, 60);
    // 上台8
    UP_Tai8();
}
/**
 * @brief 台8到景点3
 *
 */
void Tai8_jd3(void)
{
    // 下台8
    Front_down();
    Down_Tai8();
    // 加速
    speed_up(50, 110);
    speed_down(110, 50);
    // 左转
    TurnLeft_155();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 160);
    speed_down(160, 60);
    // 左转
    TurnLeft_155();
    Stop(40);
    Reset(80, 60);

    while (hwr == 0) {
        slow_run(60);
    }
    Reset(180, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    Reset(60, 60);
    // 加速
    speed_up(60, 122);
    speed_down(122, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    while (hwr == 0) {
        slow_run(60);
    }
    Reset(180, 60);
    // 加速
    speed_up(60, 120);
    speed_down(120, 60);
    // 右转进景点3
    TurnRightJD_90();
    Stop(40);
    // 撞景点3
    Crash_JD(3);
    // 左转
    TurnLeft_Place_90();
    Stop(30);
    // 矫正
    Reset(60, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
}
/**
 * @brief 景点3到景点4
 */
void jd3_jd4(void)
{
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 200);
    speed_down(200, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(70, 60);
    // 右转进景点4
    TurnRightJD_90();
    Stop(40);
    // 矫正
    Reset(50, 60);
    // 撞景点
    Crash_JD(4);
    // 右转
    TurnRight_Place_90();
    Stop(30);
    // 矫正
    Reset(70, 60);
}
/**
 * @brief 景点4到门1回家
 *
 */
void jd4_door1_Home(void)
{
    // 检测路口
    Detect_OUT_jd4();
    // 矫正
    Reset(100, 70);
    // 加速
    speed_up(70, 160);
    speed_down(160, 60);
    // 右转
    TurnRight_90_Ldetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 120);
    speed_down(120, 60);
    // 转入波浪板
    door12_Turn_BLB();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 80);
    speed_down(80, 60);
    // 过波浪板
    Pass_Short_BLB();
    // 右转
    TurnRight_45_Tai1();
    // 上台1
    UP_Tai1();
}
/**
 * @brief 景点4到门2回家
 *
 */
void jd4_door2_Home(void)
{
    // 右转
    TurnRight_45_inCross();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 145);
    speed_down(145, 60);
    // 左转
    TurnLeft_155_atCross();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 140);
    speed_down(140, 60);
    // 右转大角度
    TurnRight_155_outCross();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 120);
    speed_down(120, 60);
    // 转入波浪板
    door12_Turn_BLB();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 80);
    speed_down(80, 60);
    // 过波浪板
    Pass_Short_BLB();
    // 右转
    TurnRight_45_Tai1();
    // 上台1
    UP_Tai1();
}
/**
 * @brief 景点4到门3回家
 *
 */
void jd4_door3_Home(void)
{
    // 右转
    TurnRight_45_inCross();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up_new(60, 205);
    speed_down_new(205, 60);
    // 左转
    TurnLeft_155_atCross();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 125);
    speed_down(125, 60);
    // 门34转波浪板
    door34_Turn_BLB();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 80);
    speed_down(80, 60);
    // 过波浪板
    Pass_Short_BLB();
    // 右转
    TurnRight_45_Tai1();
    // 上台1
    UP_Tai1();
}
/**
 * @brief 景点4到门4回家
 *
 */
void jd4_door4_Home(void)
{
    // 右转
    TurnRight_45_inCross();
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 145);
    speed_down(145, 60);
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 145);
    speed_down(145, 60);
    // 左转大角度冲门4
    TurnLeft_155_door4();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 165);
    speed_down(165, 60);
    // 左转
    TurnLeft_90_Ldetect();
    Stop(40);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 125);
    speed_down(125, 60);
    // 转波浪板
    door34_Turn_BLB();
    Stop(50);
    // 矫正
    Reset(60, 60);
    // 加速
    speed_up(60, 80);
    speed_down(80, 60);
    // 过波浪板
    Pass_Short_BLB();
    // 右转
    TurnRight_45_Tai1();
    // 上台1
    UP_Tai1();
}
/**
 * @brief 走台6跷跷板圆圈
 *
 */
void Tai6_seesaw(void)
{
    while (1) {
        drift_left(60, 0);
        if (hdr == 0) break;
    }
    Touch_Seesaw_adjust();
    Past_Seesaw_adjust(170, 260);
    Land_Protect_adjust();
    // 检测左转
    while (1) {
        slow_run(50);
        if (Huidu_va(10) > white || Huidu_va(9) > white) {
            break;
        }
    }
    // 左转
    Right_Speed_Up(50, 96, 5);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-75, 80);
        if (Huidu_va(6) > white || Huidu_va(5) > white) {
            break;
        }
    }
    Stop(40);
    UP_Tai2_6(6);

    down_pt1_6();
    Front_mid();
    TurnLeft_90_Rdetect();
    Stop(40);
    Touch_Seesaw_adjust();
    Past_Seesaw_adjust(170, 260);
    Land_Protect_adjust();
    while (1) {
        drift_left(60, 0);
        if (hdr == 0) {
            break;
        }
    }
    while (1) {
        drift_right(60, 0);
        if (bhwl == 0) {
            break;
        }
    }
    while (1) {
        run(-30, 30);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
    Stop(40);
    Reset(110, 60);
    speed_up(60, 130);
    speed_down(130, 60);
}

void jd3_Tai6_jd4(void)
{
    Stop(40);
    Reset(60, 60);
    speed_up(60, 140);
    speed_down(140, 60);
    newnew_seesaw();
    // 右转
    TurnRight_90_Rdetect();
    Stop(40);
    // 矫正
    Reset(70, 60);
    // 右转进景点4
    TurnRightJD_90();
    Stop(40);
    // 矫正
    Reset(50, 60);
    // 撞景点
    Crash_JD(4);
    // 右转
    TurnRight_Place_90();
    Stop(30);
    // 矫正
    Reset(70, 60);
}

void New_Seesaw(void)
{
    // 过跷跷板
    Up_Down_Seesaw();
    // 检测左转
    while (1) {
        slow_run(50);
        if (Huidu_va(10) > white || Huidu_va(9) > white) {
            break;
        }
    }
    // 左转
    Right_Speed_Up(50, 96, 5);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-75, 80);
        if (Huidu_va(6) > white || Huidu_va(5) > white) {
            break;
        }
    }
    Stop(40);
    // 上台6
    UP_Tai2_6(6);
    // 下台6
    down_pt1_6();
    // 拉前铲
    Front_mid();
    // 左转
    TurnLeft_90_Rdetect();
    Stop(40);
    // 过跷跷板
    Up_Down_Seesaw();
    //
    while (1) {
        drift_left(60, 0);
        if (hdr == 0) {
            break;
        }
    }
    while (1) {
        drift_right(60, 0);
        if (bhwl == 0) {
            break;
        }
    }
    while (1) {
        run(-30, 30);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
    Stop(40);
    Reset(110, 60);
    speed_up(60, 130);
    speed_down(130, 60);
}

void Up_Down_Seesaw(void)
{
    while (1) {
        drift_left(70, 0);
        if (hwr == 1) break;
    }
    while ((bhwl == 1)) {
        run(0, 45);
    }

    t6_i = 0;
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    while (1) {
        if (t6_i > 225) {
            run(25, 35);
            delay_ms(5);
            // stop(20);
            // break;
        } else if ((bhwl == 1) && t6_i < 225) {
            run(35, 40);
        } else {
            while ((bhwl == 0) && t6_i < 225) {
                run(45, 30);
            }
        }
        if (hwr != 0 && t6_i > 230) {
            Front_mid();
            stop();
            break;
        }
    }
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    t6_i = 0;
    stop();
    delay_ms(650);

    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    while (1) {
        get_huidu_va();
        if (cnt_whiteline != 0) {
            if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white) {
                while (1) {
                    run(0, 50);
                    if (Huidu_va(7) > white || Huidu_va(6) > white) {
                        break;
                    }
                }
            }
            Reset_drift_left(60, 0, 80);
            break;
        } else if (cnt_whiteline == 0) {
            while (1) {
                run(0, 50);
                if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white || Huidu_va(7) > white) {
                    break;
                }
            }
            Stop(100);
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

void newnew_seesaw(void)
{
    Touch_Seesaw_adjust();
    t6_i = 0;
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    while (1) {
        if (t6_i > 400) {
            run(30, 40);
            delay_ms(5);
            // stop(20);
            // break;
        } else if ((bhwl == 1) && t6_i < 400) {
            run(35, 48);
        } else {
            while ((bhwl == 0) && t6_i < 400) {
                run(45, 30);
            }
        }
        if (hwr != 0 && t6_i > 400) {
            Front_mid();
            stop();
            break;
        }
    }
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    t6_i = 0;
    stop();
    delay_ms(650);

    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    while (1) {
        get_huidu_va();
        if (cnt_whiteline != 0) {
            if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white) {
                while (1) {
                    run(0, 50);
                    if (Huidu_va(7) > white || Huidu_va(6) > white) {
                        break;
                    }
                }
            }
            Reset_drift_left(60, 0, 80);
            break;
        } else if (cnt_whiteline == 0) {
            while (1) {
                run(0, 50);
                if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white || Huidu_va(7) > white) {
                    break;
                }
            }
            Stop(100);
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

    // 检测左转
    while (1) {
        slow_run(50);
        if (Huidu_va(10) > white || Huidu_va(9) > white) {
            break;
        }
    }
    // 左转
    Right_Speed_Up(50, 96, 5);
    Left_Speed_Down(50, -90, 5);
    while (1) {
        run(-75, 67);
        if (Huidu_va(6) > white || Huidu_va(5) > white) {
            break;
        }
    }
    Stop(40);
    // 上台6
    UP_Tai2_6(6);
    // 下台6
    down_pt1_6();
    // 拉前铲
    Front_mid();
    // 左转
    TurnLeft_90_Rdetect();
    Human_down();
    Stop(40);
    Touch_Seesaw_adjust();
    t6_i = 0;
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    while (1) {
        if (t6_i > 500) {
            run(30, 40);
            delay_ms(5);
            // stop(20);
            // break;
        } else if ((bhwl == 1) && t6_i < 500) {
            run(35, 45);
        } else {
            while ((bhwl == 0) && t6_i < 500) {
                run(45, 30);
            }
        }
        if (hwr != 0 && t6_i > 500) {
            Front_mid();
            stop();
            break;
        }
    }
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    t6_i = 0;
    stop();
    delay_ms(650);

    t3_i = 0;
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    while (1) {
        get_huidu_va();
        if (cnt_whiteline != 0) {
            if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white) {
                while (1) {
                    run(0, 50);
                    if (Huidu_va(7) > white || Huidu_va(6) > white) {
                        break;
                    }
                }
            }
            Reset_drift_left(60, 0, 80);
            break;
        } else if (cnt_whiteline == 0) {
            while (1) {
                run(0, 50);
                if (Huidu_va(10) > white || Huidu_va(9) > white || Huidu_va(8) > white || Huidu_va(11) > white || Huidu_va(7) > white) {
                    break;
                }
            }
            Stop(100);
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

    //
    while (1) {
        drift_left(60, 0);
        if (hdr == 0) {
            break;
        }
    }
    while (1) {
        drift_right(60, 0);
        if (bhwl == 0) {
            break;
        }
    }
    while (1) {
        run(-30, 30);
        if (Huidu_va(5) > white || Huidu_va(6) > white) {
            break;
        }
    }
    Stop(40);
    Reset(110, 60);
    speed_up(60, 130);
    speed_down(130, 60);
}