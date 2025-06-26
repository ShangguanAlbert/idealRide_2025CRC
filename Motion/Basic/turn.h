#ifndef __TURN_H
#define __TURN_H
#include "stm32f4xx.h"
#include "bsp_SysTick.h"
#include "bsp_motor.h"
#include "basic.h"
#include "bsp_grayscale.h"

void drift_right(int speed);
void drift_left(int speed);

#endif // __TURN_H