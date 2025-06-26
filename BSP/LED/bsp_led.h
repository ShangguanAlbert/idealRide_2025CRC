#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "stm32f4xx.h"

#define LED_PD14_CLK  RCC_AHB1Periph_GPIOD
#define LED_PD14_PORT GPIOD
#define LED_PD14_PIN  GPIO_Pin_14

#define LED_PD15_CLK  RCC_AHB1Periph_GPIOD
#define LED_PD15_PORT GPIOD
#define LED_PD15_PIN  GPIO_Pin_15

#define LED1_ON()     GPIO_SetBits(LED_PD14_PORT, LED_PD14_PIN)
#define LED1_OFF()    GPIO_ResetBits(LED_PD14_PORT, LED_PD14_PIN)
#define LED1_TOGGLE() GPIO_ToggleBits(LED_PD14_PORT, LED_PD14_PIN)

#define LED2_ON()     GPIO_SetBits(LED_PD15_PORT, LED_PD15_PIN)
#define LED2_OFF()    GPIO_ResetBits(LED_PD15_PORT, LED_PD15_PIN)
#define LED2_TOGGLE() GPIO_ToggleBits(LED_PD15_PORT, LED_PD15_PIN)

void LED_Init(void);

#endif