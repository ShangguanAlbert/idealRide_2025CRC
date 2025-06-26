#include "bsp_motor.h"
#include "bsp_SysTick.h"

float mdif_res;

/**
 * @brief 直流减速电机初始化
 * @note PE9 11 13 14 TIM1
 */
void TIM1_Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;    // 下拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Prescaler     = 68 - 1;             // 定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_Period        = 250;                // 自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // 初始化定时器1

    // 初始化TIM1 Channel1 PWM模式
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;        // 选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse        = 0;                      // 初始脉冲为0
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;    // 输出极性:TIM输出比较极性低
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   // 互补输出开启
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);                       // 根据T指定的参数初始化外设TIM1 OC1

    // 初始化TIM1 Channel2 PWM模式
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;        // 选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse        = 0;                      // 初始脉冲为0
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;    // 输出极性:TIM输出比较极性低
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   // 互补输出开启
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);                       // 根据T指定的参数初始化外设TIM1 OC2

    // 初始化TIM1 Channel3 PWM模式
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;        // 选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse        = 0;                      // 初始脉冲为0
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;    // 输出极性:TIM输出比较极性低
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   // 互补输出开启
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);                       // 根据T指定的参数初始化外设TIM1 OC3

    // 初始化TIM1 Channel4 PWM模式
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;        // 选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse        = 0;                      // 初始脉冲为0
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;    // 输出极性:TIM输出比较极性低
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;   // 互补输出开启
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);                       // 根据T指定的参数初始化外设TIM1 OC4

    // 预装载寄存器使能
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);

    // 主输出使能; 注意：高级定时器相比通用定时器，程序上，需要增加这一行，否则不工作;
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, 0);
    TIM_SetCompare4(TIM1, 0);
}

/**
 * @brief 设定左右电机速度,可变轮差,用于盲走
 * @param port 1-右电机;2-左电机
 * @param speed 设置速度, 限幅210
 */
void Motor(uint8_t port, int speed)
{
    if (speed >= 0) {
        if (speed > 220) speed = 220;
        switch (port) {
            case 1: // 右边轮子正转
                TIM1->CCR1 = speed * m_diff(speed);
                TIM1->CCR2 = 0;
                break;
            case 2: // 左边轮子正转
                TIM1->CCR3 = 0;
                TIM1->CCR4 = speed;
                break;
        }
    } else {
        if (speed < -220) speed = -220;
        switch (port) {
            case 1: // 右边轮子反转
                TIM1->CCR1 = 0;
                TIM1->CCR2 = -speed * m_diff(speed);
                break;
            case 2: // 左边轮子反转
                TIM1->CCR3 = -speed;
                TIM1->CCR4 = 0;
                break;
        }
    }
}

/**
 * @brief 设定左右电机速度,动态可变轮差,用于高速巡线
 * @param port 1-右电机;2-左电机
 * @param speed 设置速度, 限幅245
 * @param set_speed 确定此速度下的轮差
 */
void set_pwm(uint8_t port, int speed, int set_speed)
{
    if (speed >= 0) {
        if (speed > 225) speed = 225;
        switch (port) {
            case 1: // 右边轮子正转
                TIM1->CCR1 = speed * m_diff(set_speed);
                TIM1->CCR2 = 0;
                break;
            case 2: // 左边轮子转
                TIM1->CCR3 = 0;
                TIM1->CCR4 = speed;
                break;
        }
    } else {
        if (speed < -225) speed = -225;
        switch (port) {
            case 1: // 右边轮子反转
                TIM1->CCR1 = 0;
                TIM1->CCR2 = -speed * m_diff(set_speed);
                break;
            case 2: // 左边轮子转
                TIM1->CCR3 = -speed;
                TIM1->CCR4 = 0;
                break;
        }
    }
}

/**
 * @brief 对应速度下的轮差系数
 * @param speed 设定的速度
 * @return float mdif_res 对应轮差系数
 */
float m_diff(int speed)
{
    if (speed <= 50) {
        mdif_res = 1.06;
    } else if (speed > 50 && speed <= 60) {
        mdif_res = 0.96;
    } else if (speed > 60 && speed <= 75) {
        mdif_res = 0.967;
    } else if (speed > 75 && speed <= 80) {
        mdif_res = 0.976; //
    } else if (speed > 80 && speed <= 90) {
        mdif_res = 0.968;
    } else if (speed > 90 && speed <= 95) {
        mdif_res = 0.966;
    } else if (speed > 95 && speed <= 105) {
        mdif_res = 0.965;
    } else if (speed > 105 && speed < 120) {
        mdif_res = 0.965;
    } else if (speed >= 120 && speed <= 130) {
        mdif_res = 0.958;
    } else if (speed > 130 && speed <= 140) {
        mdif_res = 0.96; //
    } else if (speed > 140 && speed < 160) {
        mdif_res = 0.96;
    } else if (speed >= 160 && speed < 170) {
        mdif_res = 0.9676;
    } else if (speed >= 170 && speed < 180) {
        mdif_res = 0.975;
    } else if (speed >= 180 && speed < 185) {
        mdif_res = 0.9792;
    } else if (speed >= 185 && speed < 190) {
        mdif_res = 0.978;
    } else if (speed >= 190 && speed < 200) {
        mdif_res = 0.985;
    }
    return mdif_res;
}
