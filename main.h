#ifndef __MAIN_H_
#define __MAIN_H_
// Main header file for the System Core
#include <string.h>
#include "stm32f4xx.h"
#include "bsp_SysTick.h"
#include "sysReset.h"
#include "timer.h"
// Peripheral Drivers
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_lcd.h"
#include "lcd_spi_130.h"
#include "lcd_fonts.h"
#include "lcd_buffer.h"
#include "bsp_grayscale.h"
#include "bsp_pe.h"
#include "bsp_qr.h"
#include "bsp_gyro.h"
#include "bsp_motor.h"
#include "bsp_servo.h"
#include "bsp_speaker.h"
#include "bsp_vision.h"
#include "bsp_ccd.h"
#include "bsp_rgb.h"
#include "basic.h"
#include "turn.h"

void Robot_Init(void);
void ExecuteProgram(uint8_t programNumber);

void Program_1(void);
void Program_2(void);
void Program_3(void);
void Program_4(void);
void Program_5(void);
void Program_6(void);
void Program_7(void);
void Program_8(void);
void Program_9(void);

#endif // __MAIN_H_