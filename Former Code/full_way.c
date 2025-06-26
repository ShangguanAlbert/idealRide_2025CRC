#include "full_way.h"

/**
 * @brief door1 2 3 4 用于存储四个门颜色, 默认返回错误0
 */
uint8_t door1 = 0, door2 = 0, door3 = 0, door4 = 0;
/**
 * @brief 初赛程序 不跑台6
 *
 */
void GOGO_Line1(void)
{
    Human_up();
    Start();
    Departure_Action();
    Tai1_Tai2();
    Tai2_Tai4_jd2_Tai3();
    Tai3_jd1_door2();
    Stop(350);
    if (openmv[2] == 3) {
        door2 = 1;
        door2_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_jd4();
        jd4_door2_Home();
    } else {
        door2_door1();
        Stop(350);
        if (openmv[2] == 3) {
            door1 = 1;
            door1_Tai5();
            Tai5_jd5_Tai7();
            Tai7_Tai8();
            Tai8_jd3();
            jd3_jd4();
            jd4_door1_Home();
        } else {
            door1_door4();
            Stop(350);
            if (openmv[2] == 3) {
                door4 = 1;
                door4_Tai5();
                Tai5_jd5_Tai7();
                Tai7_Tai8();
                Tai8_jd3();
                jd3_jd4();
                jd4_door4_Home();
            } else {
                door3 = 1;
                door4_door3_Tai5();
                Tai5_jd5_Tai7();
                Tai7_Tai8();
                Tai8_jd3();
                jd3_jd4();
                jd4_door3_Home();
            }
        }
    }
    Stop(500);

    if (door1 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door1_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_jd4();
        jd4_door1_Home();
    }
    if (door2 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door2_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_jd4();
        jd4_door2_Home();
    }
    if (door3 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door3_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_jd4();
        jd4_door3_Home();
    }
    if (door4 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door4_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_jd4();
        jd4_door4_Home();
    }
    Stop(300);
    Show_TravelScore();
}

/**
 * @brief 复赛程序 门2跑台6 第二趟都跑台6
 *
 */
void GOGO_Line2(void)
{
    Human_up();
    Start();
    Departure_Action();
    Tai1_Tai2();
    Tai2_Tai4_jd2_Tai3();
    Tai3_jd1_door2();
    Stop(400);
    if (openmv[2] == 3) {
        door2 = 1;
        door2_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_Tai6_jd4();
        jd4_door2_Home();
    } else {
        door2_door1();
        Stop(300);
        if (openmv[2] == 3) {
            door1 = 1;
            door1_Tai5();
            Tai5_jd5_Tai7();
            Tai7_Tai8();
            Tai8_jd3();
            jd3_Tai6_jd4();
            jd4_door1_Home();
        } else {
            door1_door4();
            Stop(300);
            if (openmv[2] == 3) {
                door4 = 1;
                door4_Tai5();
                Tai5_jd5_Tai7();
                Tai7_Tai8();
                Tai8_jd3();
                jd3_Tai6_jd4();
                jd4_door4_Home();
            } else {
                door3 = 1;
                door4_door3_Tai5();
                Tai5_jd5_Tai7();
                Tai7_Tai8();
                Tai8_jd3();
                jd3_Tai6_jd4();
                jd4_door3_Home();
            }
        }
    }
    Stop(500);
    if (door1 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door1_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_Tai6_jd4();
        jd4_door1_Home();
    }
    if (door2 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door2_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_Tai6_jd4();
        jd4_door2_Home();
    }
    if (door3 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door3_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_Tai6_jd4();
        jd4_door3_Home();
    }
    if (door4 == 1) {
        Human_up();
        Start();
        Departure_Action();
        Tai1_Tai2();
        Tai2_Tai4_jd2_Tai3();
        Tai3_jd1_door4_Tai5();
        Tai5_jd5_Tai7();
        Tai7_Tai8();
        Tai8_jd3();
        jd3_Tai6_jd4();
        jd4_door4_Home();
    }
    stop();
    Show_TravelScore();
}