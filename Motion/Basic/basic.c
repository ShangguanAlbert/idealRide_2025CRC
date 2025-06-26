#include "basic.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"

/**
 * @brief 左右轮走直线, 不带时间
 * @param run_speed 速度
 */
void Run(int run_speed)
{
    Motor(1, run_speed);
    Motor(2, run_speed);
}
/**
 * @brief 左右轮走直线, 带时间
 * @param run_speed 速度
 * @param last_time 延时
 */
void Run_delay(int run_speed, uint16_t last_time)
{
    Motor(1, run_speed);
    Motor(2, run_speed);
    Delay_ms(last_time);
}
/**
 * @brief 左右轮差速走,不带时间
 * @param left_speed 左轮速度
 * @param right_speed 右轮速度
 */
void run(int left_speed, int right_speed)
{
    Motor(2, left_speed);
    Motor(1, right_speed);
}
/**
 * @brief 左右轮差速走,带时间
 * @param left_speed 左轮速度
 * @param right_speed 右轮速度
 * @param last_time 延时
 */
void run_delay(int left_speed, int right_speed, uint16_t last_time)
{
    Motor(2, left_speed);
    Motor(1, right_speed);
    Delay_ms(last_time);
}
/**
 * @brief 停车无限长时间
 */
void stop(void)
{
    Motor(1, 0);
    Motor(2, 0);
    Motor(1, -0);
    Motor(2, -0);
}
/**
 * @brief 停车一段时间
 * @param times 时间
 */
void Stop(uint16_t times)
{
    Motor(1, 0);
    Motor(2, 0);
    Delay_ms(times);
}
/**
 * @brief 左轮维持原本速度,右轮速度减小
 *
 * @param Fir 起始速度
 * @param End 终止速度
 * @param delay_time 短延时时间 短的用3 长的用5
 */
void Right_Speed_Down(int Fir, int End, uint16_t delay_time)
{
    for (; End <= Fir; Fir--) {
        Motor(1, Fir);
        if (Fir % 5 == 0) Delay_ms(delay_time);
    }
}
/**
 * @brief 右轮维持原本速度,左轮速度减小
 *
 * @param Fir 起始速度
 * @param End 终止速度
 * @param delay_time 短延时时间 短的用3 长的用5
 */
void Left_Speed_Down(int Fir, int End, uint16_t delay_time)
{
    for (; End <= Fir; Fir--) {
        Motor(2, Fir);
        if (Fir % 5 == 0) Delay_ms(delay_time); // 短的用3 长的用5
    }
}
/**
 * @brief 左轮维持原本速度,右轮速度增大
 *
 * @param Fir 起始速度
 * @param End 终止速度
 * @param delay_time 短延时时间 短的用3 长的用5
 */
void Right_Speed_Up(int Fir, int End, uint16_t delay_time)
{
    for (; Fir <= End; Fir++) {
        Motor(1, Fir);
        if (Fir % 5 == 0) Delay_ms(delay_time); // 短的用3 长的用5
    }
}
/**
 * @brief 右轮维持原本速度,左轮速度增大
 *
 * @param Fir 起始速度
 * @param End 终止速度
 * @param delay_time 短延时时间 短的用3 长的用5
 */
void Left_Speed_Up(int Fir, int End, uint16_t delay_time)
{
    for (; Fir <= End; Fir++) {
        Motor(2, Fir);
        if (Fir % 5 == 0) Delay_ms(delay_time); // 短的用3 长的用5
    }
}
/**
 * @brief 原地加速右转,相反轮差
 *
 * @param Low 起始速度
 * @param High 终止速度
 * @param delay_time 短延时时间 用1
 */
void turnR_speed_up(int Low, int High, uint16_t delay_time)
{
    for (; Low < High; Low++) {
        Motor(2, Low);
        Motor(1, -Low);
        if (Low % 5 == 0) Delay_ms(delay_time); // 用1
    }
}
/**
 * @brief 原地减速右转,相反轮差
 *
 * @param Low 起始速度
 * @param High 终止速度
 * @param delay_time 短延时时间 用1
 */
void turnR_speed_down(int High, int Low, uint16_t delay_time)
{
    for (; High > Low; High--) {
        Motor(2, High);
        Motor(1, -High);
        if (High % 5 == 0) Delay_ms(delay_time); // 用1
    }
}
/**
 * @brief 原地加速左转,相反轮差
 *
 * @param Low 起始速度
 * @param High 终止速度
 * @param delay_time 短延时时间 用1
 */
void turnL_speed_up(int Low, int High, uint16_t delay_time)
{
    for (; Low < High; Low++) {
        Motor(2, -Low);
        Motor(1, Low);
        if (Low % 5 == 0) Delay_ms(delay_time); // 用1
    }
}
/**
 * @brief 原地减速左转,相反轮差
 *
 * @param Low 起始速度
 * @param High 终止速度
 * @param delay_time 短延时时间 用1
 */
void turnL_speed_down(int High, int Low, uint16_t delay_time)
{
    for (; High > Low; High--) {
        Motor(2, -High);
        Motor(1, High);
        if (High % 5 == 0) Delay_ms(delay_time); // 用1
    }
}