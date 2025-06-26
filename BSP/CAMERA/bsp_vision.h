#ifndef __BSP_VISION_H
#define __BSP_VISION_H

#include "stm32f4xx.h"
#include <string.h>

extern int openmv[]; // OpenMV数据数组

void USART1_OpenMV_Init(void);
void Openmv_Data(void);
void Openmv_Receive_Data(int16_t data);
int Get_OpenMV_Data(void);

#endif