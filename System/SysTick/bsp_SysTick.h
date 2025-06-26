#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H
#include "stm32f4xx.h"

void SysTick_Init(void);
void delay_us(uint32_t nus);
void delay_xms(uint16_t nms);
void delay_ms(uint16_t nms);
void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);
void Delay_s(uint32_t xs);

#endif