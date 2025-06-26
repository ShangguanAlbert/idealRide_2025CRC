#ifndef __BSP_PE_H_
#define __BSP_PE_H_
#include "stm32f4xx.h"

void PhotoElectric_Init(void);
uint8_t PE_Value(uint8_t channel);
#endif // __BSP_PE_H_