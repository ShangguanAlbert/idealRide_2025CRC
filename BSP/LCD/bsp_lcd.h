#ifndef __BSP_LCD_H
#define __BSP_LCD_H
#include "stm32f4xx.h"
#include "lcd_spi_130.h"
#include "bsp_SysTick.h"
#include "bsp_key.h"

#define ARM_MATH_CM4

void DrawMenuToBuffer(uint16_t *buffer, uint8_t selectedIndex, uint8_t totalPrograms);
uint8_t Function_Mode(void);
void Progarm_Running_Info(char *prog_name, char *prog_desc);
uint8_t DetectKeyPress(uint8_t current_state, uint8_t *last_state);
void Show_SensorPage_All(void);
void Sensor_Test_Page1(void);
void Sensor_Test_Page2(void);
void Sensor_Test_Page3(void);
void Sensor_Test_Page4(void);

#endif
