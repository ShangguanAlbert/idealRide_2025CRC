#include "stm32f4xx.h"
#include "bsp_lcd.h"
#include "lcd_spi_130.h"
#include "sysReset.h"

/**
 * @brief STM32F407 系统软复位
 *
 */
void System_SOFT_reset(void)
{
    SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                  (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                  SCB_AIRCR_SYSRESETREQ_Msk);
}

/**
 * @brief 获取系统复位源类型
 * @param 无
 * @retval ResetSourceType 复位源类型枚举
 */
ResetSourceType Get_Reset_Source(void)
{
    // 读取复位标志
    uint32_t reset_flags         = RCC->CSR;
    ResetSourceType reset_source = RESET_UNKNOWN;

    // 按优先级检查各种复位源
    if (reset_flags & RCC_CSR_WDGRSTF) {
        reset_source = RESET_IWDG;
    } else if (reset_flags & RCC_CSR_WWDGRSTF) {
        reset_source = RESET_WWDG;
    } else if (reset_flags & RCC_CSR_SFTRSTF) {
        reset_source = RESET_SOFTWARE;
    } else if (reset_flags & RCC_CSR_PORRSTF) {
        reset_source = RESET_POWER_ON;
    } else if (reset_flags & RCC_CSR_PADRSTF) {
        reset_source = RESET_PIN;
    } else if (reset_flags & RCC_CSR_BORRSTF) {
        reset_source = RESET_BOR;
    } else if (reset_flags & RCC_CSR_LPWRRSTF) {
        reset_source = RESET_LOW_POWER;
    }

    // 清除复位标志
    RCC->CSR |= RCC_CSR_RMVF;

    return reset_source;
}

/**
 * @brief 获取复位源描述字符串
 * @param reset_source 复位源类型
 * @retval const char* 复位源描述字符串
 */
char *Get_Reset_Source_Description(ResetSourceType reset_source)
{
    switch (reset_source) {
        case RESET_IWDG:
            return "IWDG Reset";
        case RESET_WWDG:
            return "WWDG Reset";
        case RESET_SOFTWARE:
            return "Software Reset";
        case RESET_POWER_ON:
            return "Power-On Reset";
        case RESET_PIN:
            return "NRST Pin Reset";
        case RESET_BOR:
            return "BOR Reset";
        case RESET_LOW_POWER:
            return "Low Power Reset";
        default:
            return "Unknown Reset";
    }
}

/**
 * @brief 获取复位源附加说明字符串
 * @param reset_source 复位源类型
 * @retval const char* 复位源附加说明字符串，如果没有则返回NULL
 */
char *Get_Reset_Source_Additional_Info(ResetSourceType reset_source)
{
    switch (reset_source) {
        case RESET_IWDG:
            return "(Watchdog)";
        case RESET_WWDG:
            return "(Window WDG)";
        default:
            return NULL;
    }
}

/**
 * @brief 在LCD上显示复位源信息
 * @param 无
 * @retval 无
 */
void Display_Reset_Source(void)
{
    ResetSourceType reset_source = Get_Reset_Source();
    char *description            = Get_Reset_Source_Description(reset_source);
    char *additional_info        = Get_Reset_Source_Additional_Info(reset_source);

    // 显示方向
    LCD_SetDirection(Direction_V);
    // 清除显示区域
    LCD_ClearRect(0, 0, 130, 80);

    // 显示标题
    LCD_SetColor(0x00FFFF); // 青色标题
    LCD_DisplayString(0, 0, "Reset Source:");
    LCD_SetColor(0xFFFFFF); // 恢复白色文本

    // 显示复位源描述
    LCD_DisplayString(0, 30, description);

    // 显示附加信息（如果有）
    if (additional_info != NULL) {
        LCD_DisplayString(0, 60, additional_info);
    }
}

/**
 * @brief 检查系统复位源并在LCD上显示
 * @param 无
 * @retval 无
 */
void Check_Reset_Source(void)
{
    Display_Reset_Source();
    Delay_s(2);
    LCD_Clear();
}