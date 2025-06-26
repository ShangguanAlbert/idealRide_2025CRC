#ifndef _BSP_QR_H
#define _BSP_QR_H
#include "stm32f4xx.h"

void UART4_QRCode_Init(void);
uint16_t Start_QR_Detection(void);
void QR_Reset(void);
uint16_t QR_GetValue(void);
uint8_t QR_IsReady(void);

#endif
