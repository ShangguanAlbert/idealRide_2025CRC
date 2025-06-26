#include "bsp_pe.h"

uint8_t Digital_Value;

void PhotoElectric_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;  // 输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 速度选择

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // PD0, PD1, PD2
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; // PE0, PE1, PE2
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

uint8_t PE_Value(uint8_t channel)
{
    if (channel == 0) {
        return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0); // 读取PD0
    }
    if (channel == 1) {
        return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1); // 读取PD1
    }
    if (channel == 2) {
        return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0); // 读取PE0
    }
    if (channel == 3) {
        return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1); // 读取PE
    }
    if (channel == 4) {
        return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2); // 读取PE2
    }
    if (channel == 5) {
        return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3); // 读取PE3
    }
    if (channel == 6) {
        return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4); // 读取PE4
    }
    if (channel == 7) {
        return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5); // 读取PE5
    }
    return 2;
}