#include "bsp_key.h"
#include "bsp_SysTick.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up = 1;
    if (mode) key_up = 1;
    if (key_up && (KeyUp == 0 || KeyDown == 0 || KeyConfirm == 0)) {
        Delay_ms(10);
        key_up = 0;
        if (KeyUp == 0)
            return 1;
        else if (KeyDown == 0)
            return 3;
        else if (KeyConfirm == 0)
            return 2;
    } else if (KeyUp == 1 && KeyDown == 1 && KeyConfirm == 1) {
        key_up = 1;
    }
    return 0;
}
