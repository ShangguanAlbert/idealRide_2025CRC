#ifndef BSP_CCD_H
#define BSP_CCD_H
#include "stm32f4xx.h"

extern uint16_t ADC_CCD_Value[]; // 存储CCD采集的ADC值

// CCD初始化
void CCD_Init(void);
// CCD定时器初始化
void CCD_TIM5_Init(void);
// CCD读取数据
uint16_t CCD_Read_AO(void);
// CCD写入CLK
void CCD_W_CLK(uint8_t BitValue);
// CCD写入SI
void CCD_W_SI(uint8_t BitValue);
// CCD采集触发
void CCD_Trigger_Collection(void);
// CCD采集数据
void CCD_GetValue(void);


#endif // BSP_CCD_H