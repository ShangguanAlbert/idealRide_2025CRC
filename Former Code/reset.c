#include "reset.h"

uint32_t t6_i; // t6_i 加到200为1秒
uint32_t t7_i; // t7_i 加到200为1秒

/**
 * @brief 循迹主定时器 定时器6初始化
 */
void TIM6_Reset_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // 使能TIM6时钟

    TIM_TimeBaseInitStructure.TIM_Period        = 100 - 1;  // 自动装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler     = 4200 - 1; // 分频系数
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; // 设置向上计数模式
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE); // 关闭定时器中断
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM6_DAC_IRQn; // 定时器中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;             // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;             // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;        // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM6, ENABLE); // 使能定时器
}
/**
 * @brief 定时器6 中断服务函数
 *
 */
void TIM6_DAC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update)) {
        t6_i++;
    }
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}

/**
 * @brief 指定车辆通过延时确定时间达到目标速度,绿地巡线
 * @param time_last 延时时间 200为1秒.
 * @param goal_speed 目标速度,不超过220.
 */
void Reset(int time_last, int goal_speed)
{
    t6_i = 0;
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    do {
        if (goal_speed > 90) {
            high_run(goal_speed);
        } else {
            slow_run(goal_speed);
        }
    } while (t6_i < time_last);
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    t6_i = 0;
}

/**
 * @brief 桥上巡线
 * @param time_last 时间
 * @param goal_speed 目标速度
 * @param mode 权值模式
 */
void Reset_bridge(int time_last, int goal_speed, uint8_t mode)
{
    t6_i = 0;
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    do {
        bridge_PD(goal_speed, mode);
    } while (t6_i < time_last);
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    t6_i = 0;
}

/**
 * @brief 循迹辅助定时器 定时器7初始化
 * @note 1ms 定时时间=(预分频数+1）*（计数值+1) /TIM7时钟(84MHz)，单位(s)
 */
void TIM7_Aid_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseStructure.TIM_Period        = 100 - 1;  // 自动装载值
    TIM_TimeBaseStructure.TIM_Prescaler     = 4200 - 1; // 分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; // 设置向上计数模式
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM7, ENABLE);
}
/**
 * @brief 定时器7 中断服务函数
 *
 */
void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update)) {
        t7_i++;
    }
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); // 清除更新中断标志位
}
/**
 * @brief 卡时间向左飘
 *
 * @param speed 速度
 * @param model 模式，低速还是高速
 * @param time_last 持续时间 1000为1秒
 */
void Reset_drift_left(int speed, uint8_t model, int time_last)
{
    t7_i = 0;
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    do {
        drift_left(speed, model);
    } while (t7_i < time_last);
    TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
    t7_i = 0;
}

/**
 * @brief 卡时间向右飘
 *
 * @param speed 速度
 * @param model 模式，低速还是高速
 * @param time_last 持续时间 1000为1秒
 */
void Reset_drift_right(int speed, uint8_t model, int time_last)
{
    t7_i = 0;
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    do {
        drift_right(speed, model);
    } while (t7_i < time_last);
    TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
    t7_i = 0;
}
/**
 * @brief 维持原判的辅助新巡线
 *
 * @param time_last 时间
 * @param goal_speed 目标速度
 */
void Reset_Aid(int time_last, int goal_speed)
{
    t7_i = 0;
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    do {
        if (goal_speed > 90) {
            high_run_new(goal_speed);
        } else {
            slow_run(goal_speed);
        }
    } while (t7_i < time_last);
    TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
    t7_i = 0;
}