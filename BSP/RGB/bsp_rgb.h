#ifndef __BSP_RGB_H
#define __BSP_RGB_H
#include "stm32f4xx.h"

void MyI2C_Init(void);
void I2C_RGB_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

void RGB_Read(void);
uint8_t RGB_GetRed(void);
uint8_t RGB_GetGreen(void);
uint8_t RGB_GetBlue(void);

#endif