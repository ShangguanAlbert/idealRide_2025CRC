/**
 * @file bsp_led.c
 * @author Fuze
 * @brief 板子led灯驱动，PD14和PD15
 * @version 0.1
 * @date 2025-03-30
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "bsp_led.h"
#include "bsp_SysTick.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED_PD14_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(LED_PD15_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = LED_PD14_PIN;
    GPIO_Init(LED_PD14_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_PD15_PIN;
    GPIO_Init(LED_PD15_PORT, &GPIO_InitStructure);
    
}