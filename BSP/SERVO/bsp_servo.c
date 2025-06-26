/**
 * @file bsp_servo.c
 * @author sgfz
 * @brief 舵机定时器初始化和控制函数，定时器TIM2用于控制舵机1-4，TIM8用于控制舵机5-6
 *        TIM2_CH1和TIM2_CH2 PA15 and PB3 用于控制舵机1和舵机2
 *        TIM8_CH3和TIM8_CH4 PC8 and PC9 用于控制舵机3和舵机4
 *        TIM2_CH3和TIM2_CH4 PB10 and PB11 用于控制舵机5和舵机6
 * @version 0.1
 * @date 2025-06-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "bsp_servo.h"
#include "bsp_SysTick.h"

void TIM2_SERVO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE); // 开启相关的GPIO外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                         // 使能定时器时钟
    TIM_DeInit(TIM2);

    /** 配置所用TIM的时基 **/
    TIM_TimeBaseStructure.TIM_Prescaler     = 84 - 1;
    TIM_TimeBaseStructure.TIM_Period        = 20000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE); // ARPE使能

    TIM_Cmd(TIM2, ENABLE); // 使能定时器

    TIM_SetCompare1(TIM2, 0); // 初始化PWM占空比
    TIM_SetCompare2(TIM2, 0);
    TIM_SetCompare3(TIM2, 0);
    TIM_SetCompare4(TIM2, 0);
}
/**
 * @brief 舵机定时器8初始化
 * @note TIM8_CH3和TIM8_CH4 PC8 and PC9 用于控制舵机
 */
void TIM8_SERVO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 开启相关的GPIO外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  // 使能定时器时钟
    TIM_DeInit(TIM8);

    /** 配置所用TIM的时基 **/
    TIM_TimeBaseStructure.TIM_Prescaler         = 168 - 1;
    TIM_TimeBaseStructure.TIM_Period            = 20000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 高级定时器需要设置这个
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // TIM8_CH1和TIM8_CH2
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;

    TIM_OC3Init(TIM8, &TIM_OCInitStructure);
    TIM_OC4Init(TIM8, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM8, ENABLE); // ARPE使能

    // 主输出使能; 注意：高级定时器相比通用定时器，程序上，需要增加这一行，否则不工作;
    TIM_CtrlPWMOutputs(TIM8, ENABLE);

    // 使能定时器
    TIM_Cmd(TIM8, ENABLE);
}

/**
 * @brief 舵机控制函数
 * @note  按照PCB设计从上到下排列
 *        舵机1-PB11 TIM2_CH4
 *        舵机2-PB10 TIM2_CH3
 *        舵机3-PC8  TIM8_CH3
 *        舵机4-PC9  TIM8_CH4
 *        舵机5-PA15 TIM2_CH1
 *        舵机6-PB3  TIM2_CH2
 * @param num 舵机编号 (1-6)
 * @param pulse 舵机脉冲宽度 (单位: 微秒)
 */
void servo(uint8_t num, uint32_t pulse)
{
    if (pulse < 400) pulse = 400;
    if (pulse > 2700) pulse = 2700;

    switch (num) {
        case 1:
            TIM_SetCompare4(TIM2, pulse); // TIM2_CH4
            break;
        case 2:
            TIM_SetCompare3(TIM2, pulse); // TIM2_CH3
            break;
        case 3:
            TIM_SetCompare3(TIM8, pulse); // TIM8_CH3
            break;
        case 4:
            TIM_SetCompare4(TIM8, pulse); // TIM8_CH4
            break;
        case 5:
            TIM_SetCompare1(TIM2, pulse); // TIM2_CH1
            break;
        case 6:
            TIM_SetCompare2(TIM2, pulse); // TIM2_CH2
            break;
        default:
            break;
    }
}

void human_down(void)
{
    servo(2, 2500);
}
void human_up(void)
{
    servo(2, 1900);
}
void front_down(void)
{
    servo(1, 2500);
}
void front_mid(void)
{
    servo(1, 2000);
}
void front_up(void)
{
    servo(1, 1500);
}