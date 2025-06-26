#include "bsp_speaker.h"
#include "bsp_SysTick.h"

// 语音控制引脚定义（使用5位二进制编码，支持32种语音）
#define VOICE_GPIO_PORT_E  GPIOE
#define VOICE_GPIO_PORT_C  GPIOC
#define VOICE_PIN_0        GPIO_Pin_8  // 二进制位0 - PE8
#define VOICE_PIN_1        GPIO_Pin_10 // 二进制位1 - PE10
#define VOICE_PIN_2        GPIO_Pin_12 // 二进制位2 - PE12
#define VOICE_PIN_3        GPIO_Pin_15 // 二进制位3 - PE15
#define VOICE_PIN_4        GPIO_Pin_13 // 二进制位4 - PC13

#define VOICE_TRIGGER_TIME 200 // 触发时间(ms)

// 语音编号对应的语音内容
typedef enum {
    Arrive_Platform_1 = 1, // 到达1号平台
    Arrive_Platform_2,     // 到达2号平台
    Arrive_Platform_3,     // 到达3号平台
    Arrive_Platform_4,     // 到达4号平台
    Arrive_Platform_5,     // 到达5号平台
    Arrive_Platform_6,     // 到达6号平台
    Arrive_Platform_7,     // 到达7号平台
    Arrive_Platform_8,     // 到达8号平台
    Ready,                 // 准备完毕
    VOICE_VERTICAL_SPOT,   // 到达直立景点
    VOICE_ClueA,           // 线索A是
    VOICE_ClueB,           // 线索B是
    Number_0,              // 数字0
    Number_1,              // 数字1
    Number_2,              // 数字2
    Number_3,              // 数字3
    Number_4,              // 数字4
    Number_5,              // 数字5
    Number_6,              // 数字6
    FIND_TREASURE,         // 发现宝物
} voice_id_t;

/**
 * @brief 语音模块GPIO初始化
 */
void GPIO_VOICE_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOE和GPIOC时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOC, ENABLE);

    // 初始化GPIOE引脚 (PE8, PE10, PE12, PE15)
    GPIO_InitStructure.GPIO_Pin   = VOICE_PIN_0 | VOICE_PIN_1 | VOICE_PIN_2 | VOICE_PIN_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(VOICE_GPIO_PORT_E, &GPIO_InitStructure);

    // 初始化GPIOC引脚 (PC13)
    GPIO_InitStructure.GPIO_Pin = VOICE_PIN_4;
    GPIO_Init(VOICE_GPIO_PORT_C, &GPIO_InitStructure);

    // 初始化为高电平（静音状态）
    GPIO_SetBits(VOICE_GPIO_PORT_E, VOICE_PIN_0 | VOICE_PIN_1 | VOICE_PIN_2 | VOICE_PIN_3);
    GPIO_SetBits(VOICE_GPIO_PORT_C, VOICE_PIN_4);
}

/**
 * @brief 设置语音控制引脚状态
 * @param voice_code 语音编码 (0-31)
 */
static void Set_Voice_Pins(uint8_t voice_code)
{
    // 先设置所有引脚为高电平
    GPIO_SetBits(VOICE_GPIO_PORT_E, VOICE_PIN_0 | VOICE_PIN_1 | VOICE_PIN_2 | VOICE_PIN_3);
    GPIO_SetBits(VOICE_GPIO_PORT_C, VOICE_PIN_4);

    // 根据二进制编码设置对应引脚为低电平
    if (voice_code & 0x01) GPIO_ResetBits(VOICE_GPIO_PORT_E, VOICE_PIN_0); // bit 0
    if (voice_code & 0x02) GPIO_ResetBits(VOICE_GPIO_PORT_E, VOICE_PIN_1); // bit 1
    if (voice_code & 0x04) GPIO_ResetBits(VOICE_GPIO_PORT_E, VOICE_PIN_2); // bit 2
    if (voice_code & 0x08) GPIO_ResetBits(VOICE_GPIO_PORT_E, VOICE_PIN_3); // bit 3
    if (voice_code & 0x10) GPIO_ResetBits(VOICE_GPIO_PORT_C, VOICE_PIN_4); // bit 4
}

/**
 * @brief 播报语音
 * @param voice_id 语音编号 (1-20)
 */
void sound(uint8_t voice_id)
{
    if (voice_id < 1 || voice_id > 20) {
        return; // 无效的语音编号
    }

    // 设置对应的引脚状态来触发语音
    Set_Voice_Pins(voice_id);

    // 保持触发状态
    Delay_ms(VOICE_TRIGGER_TIME);

    // 恢复到静音状态（所有引脚高电平）
    GPIO_SetBits(VOICE_GPIO_PORT_E, VOICE_PIN_0 | VOICE_PIN_1 | VOICE_PIN_2 | VOICE_PIN_3);
    GPIO_SetBits(VOICE_GPIO_PORT_C, VOICE_PIN_4);
}

/**
 * @brief 播报平台到达语音的便捷函数
 * @param platform_num 平台编号 (1-8)
 */
void sound_platform(uint8_t platform_num)
{
    if (platform_num >= 1 && platform_num <= 8) {
        sound(platform_num);
    }
}

/**
 * @brief 播报数字语音的便捷函数
 * @param number 数字 (0-6)
 */
void sound_number(uint8_t number)
{
    if (number <= 6) {
        sound(Number_0 + number); // Number_0是数字0的编号
    }
}

/**
 * @brief 播报准备完毕
 */
void sound_ready(void)
{
    sound(Ready);
}

/**
 * @brief 播报到达直立景点
 */
void sound_vertical_spot(void)
{
    sound(VOICE_VERTICAL_SPOT);
}

/**
 * @brief 播报线索A
 */
void sound_clue_a(void)
{
    sound(VOICE_ClueA);
}

/**
 * @brief 播报线索B
 */
void sound_clue_b(void)
{
    sound(VOICE_ClueB);
}

/**
 * @brief 播报发现宝物
 */
void sound_find_treasure(void)
{
    sound(FIND_TREASURE);
}