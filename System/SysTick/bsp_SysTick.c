#include "bsp_SysTick.h"

static uint8_t fac_us  = 0; // us延时倍乘数
static uint16_t fac_ms = 0; // ms延时倍乘数

/**
 * @brief SysTick初始化，SYSTICK的时钟固定为AHB时钟的1/8
 * @param fac_ms 每个ms需要的systick时钟数
 * @note  SYSCLK 系统时钟频率 168M 8分频 保存1us所需的计数次数
 */
void SysTick_Init(void)
{
    static uint8_t SYSCLK = 168;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SYSCLK / 8;
    fac_ms = (uint16_t)fac_us * 1000;
}
/**
 * @brief us(微秒)级延时
 * @param nus 要延时的us(微秒)数
 * @note nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
 */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us;             // 时间加载
    SysTick->VAL  = 0x00;                     // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00;                       // 清空计数器
}
/**
 * @brief ms延时
 * @param nms 要延时的ms数
 * @note nms的值,SysTick->LOAD为24位寄存器，
         不能大于0xffffff*8*1000/SYSCLK
         对168M条件下,nms<=798ms
 */
void delay_xms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms;   // 时间加载(SysTick->LOAD为24bit)
    SysTick->VAL  = 0x00;                     // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    SysTick->VAL = 0X00;                       // 清空计数器
}
/**
 * @brief ms延时
 * @param nms 要延时的ms数 0~65535
 */
void delay_ms(uint16_t nms)
{
    uint8_t repeat = nms / 540; // 这里用540,是考虑到某些客户可能超频使用,
                                // 比如超频到248M的时候,delay_xms最大只能延时541ms左右了
    uint16_t remain = nms % 540;
    while (repeat) {
        delay_xms(540);
        repeat--;
    }
    if (remain) delay_xms(remain);
}
/**
 * @brief  微秒级延时
 * @param  xus 延时时长，范围：0~233015
 * @retval 无
 */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 168 * xus;             // 设置定时器重装值
    SysTick->VAL  = 0x00;                  // 清空当前计数值
    SysTick->CTRL = 0x00000005;            // 设置时钟源为HCLK，启动定时器
    while (!(SysTick->CTRL & 0x00010000)); // 等待计数到0
    SysTick->CTRL = 0x00000004;            // 关闭定时器
}
/**
 * @brief  毫秒级延时
 * @param  xms 延时时长，范围：0~4294967295
 * @retval 无
 */
void Delay_ms(uint32_t xms)
{
    while (xms--) {
        Delay_us(1000);
    }
}
/**
 * @brief  秒级延时
 * @param  xs 延时时长，范围：0~4294967295
 * @retval 无
 */
void Delay_s(uint32_t xs)
{
    while (xs--) {
        Delay_ms(1000);
    }
}
