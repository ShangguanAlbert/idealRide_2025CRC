#include "bsp_ccd.h"
#include "bsp_SysTick.h"

#define CCD_AO_PIN   GPIO_Pin_4
#define CCD_AO_GPIO  GPIOA
#define CCD_CLK_PIN  GPIO_Pin_6
#define CCD_CLK_GPIO GPIOE
#define CCD_SI_PIN   GPIO_Pin_7
#define CCD_SI_GPIO  GPIOE

uint16_t ADC_CCD_Value[128]; // 存储ADC值

/**
 * @brief CCD初始化函数
 *
 */
void CCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    // 使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

    // 初始化AO引脚
    GPIO_InitStructure.GPIO_Pin  = CCD_AO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CCD_AO_GPIO, &GPIO_InitStructure);

    // 初始化CLK引脚
    GPIO_InitStructure.GPIO_Pin   = CCD_CLK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CCD_CLK_GPIO, &GPIO_InitStructure);

    // 初始化SI引脚
    GPIO_InitStructure.GPIO_Pin   = CCD_SI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CCD_SI_GPIO, &GPIO_InitStructure);

    // ADC2初始化
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);

    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div6;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode         = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode   = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

    ADC_InitStructure.ADC_ExternalTrigConv = 0;
    ADC_InitStructure.ADC_DataAlign        = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion  = 1;
    ADC_Init(ADC2, &ADC_InitStructure);

    // 配置ADC2的通道
    ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_144Cycles);

    ADC_Cmd(ADC2, ENABLE);
    ADC_SoftwareStartConv(ADC2);
}

/**
 * @brief CCD定时器初始化函数 TIM5
 *
 */
void CCD_TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    // 定时周期设置为129*10us，(9+1)*(71-1)/(72*10^6)
    TIM_TimeBaseStructure.TIM_Period        = 129 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = 84 - 1; // 84MHz/84 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM5, ENABLE);
}

/**
 * @brief 写入CLK引脚
 * @param BitValue 0或1
 */
void CCD_W_CLK(uint8_t BitValue)
{
    GPIO_WriteBit(CCD_CLK_GPIO, CCD_CLK_PIN, (BitAction)BitValue);
}

/**
 * @brief 写入SI引脚
 * @param BitValue 0或1
 */
void CCD_W_SI(uint8_t BitValue)
{
    GPIO_WriteBit(CCD_SI_GPIO, CCD_SI_PIN, (BitAction)BitValue);
}
/**
 * @brief 读取AO引脚的ADC值
 * @return uint16_t ADC转换结果
 */
uint16_t CCD_Read_AO(void)
{
    // ADC_SoftwareStartConv(ADC2); // 启动ADC转换
    while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC2); // 返回转换结果
}

/**
 * @brief CCD采集触发
 */
void CCD_Trigger_Collection(void)
{
    // 触发CCD采集
    CCD_W_CLK(0);
    CCD_W_SI(0);
    Delay_us(10);

    CCD_W_SI(1);
    Delay_us(25);

    CCD_W_CLK(1);
    Delay_us(10);
    CCD_W_SI(0);

    Delay_us(10);
}

/**
 * @brief CCD采集数据函数
 *
 */
void CCD_GetValue(void)
{
    uint8_t index = 0;
    for (uint8_t i = 0; i < 128; i++) {
        CCD_W_CLK(0);                         // 设置CLK为低电平
        Delay_us(500);                        // 曝光时间
        ADC_CCD_Value[index] = CCD_Read_AO(); // 读取AO引脚的ADC
        index++;
        CCD_W_CLK(1); // 设置CLK为高电平
        // Delay_ms(1);
    }
}