#ifndef __SYSRESET_H
#define __SYSRESET_H
#include "stm32f4xx.h"
#include "lcd_spi_130.h"
#include "bsp_lcd.h"

/**
 * @brief 复位源类型枚举
 */
typedef enum {
    RESET_UNKNOWN = 0, // 未知复位源
    RESET_IWDG,        // 独立看门狗复位
    RESET_WWDG,        // 窗口看门狗复位
    RESET_SOFTWARE,    // 软件复位
    RESET_POWER_ON,    // 上电/掉电复位
    RESET_PIN,         // 引脚复位 (NRST)
    RESET_BOR,         // 棕色复位 (BOR)
    RESET_LOW_POWER    // 低功耗复位
} ResetSourceType;

/* 函数声明 */
void System_SOFT_reset(void);
ResetSourceType Get_Reset_Source(void);
char *Get_Reset_Source_Description(ResetSourceType reset_source);
char *Get_Reset_Source_Additional_Info(ResetSourceType reset_source);
void Check_Reset_Source(void);
void Display_Reset_Source(void);

#endif /* __SYS_RESET_H */