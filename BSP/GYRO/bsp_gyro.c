#include "bsp_gyro.h"
#include "stdio.h"
#include <string.h>
#include "stdlib.h"
#include "arm_math.h"

// HWT101 协议定义
#define HWT101_FRAME_HEADER   0x55
#define HWT101_ANGLE_TYPE     0x53
#define HWT101_FRAME_LENGTH   11
#define HWT101_RX_BUFFER_SIZE 250

// HWT101 数据解析状态
typedef enum {
    HWT101_IDLE = 0,
    HWT101_RECEIVING
} HWT101_RxState_t;

// HWT906 协议定义
#define HWT906_FRAME_HEADER   0x55
#define HWT906_ANGLE_TYPE     0x53
#define HWT906_FRAME_LENGTH   11
#define HWT906_RX_BUFFER_SIZE 250

// HWT906 数据解析状态
typedef enum {
    HWT906_IDLE = 0,
    HWT906_RECEIVING
} HWT906_RxState_t;

// 私有变量
static uint8_t rx_data_byte;
static uint8_t rx_data_byte_u5;
static HWT101_RxState_t rx_state    = HWT101_IDLE;
static HWT906_RxState_t rx_state_u5 = HWT906_IDLE;
static uint8_t rx_frame_buffer[HWT101_FRAME_LENGTH];
static uint8_t rx_frame_buffer_u5[HWT906_FRAME_LENGTH];
static uint16_t rx_frame_index    = 0;
static uint16_t rx_frame_index_u5 = 0;

// HWT101 陀螺仪角度数据（全局变量，供外部使用）
float gyro_angle_z;     // Z轴角度（度）
float gyro_angle_z_raw; // Z轴角度原始值

// HWT906 陀螺仪角度数据（全局变量，供外部使用）
float gyro_angle_x_u5;     // X轴角度（度）
float gyro_angle_x_raw_u5; // X轴角度原始值
float gyro_angle_y_u5;     // Y轴角度（度）
float gyro_angle_y_raw_u5; // Y轴角度原始值
float gyro_angle_z_u5;     // Z轴角度（度）
float gyro_angle_z_raw_u5; // Z轴角度原始值

/**
 * @brief 初始化USART2用于HWT101陀螺仪通信
 */
void USART2_HWT101_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 配置GPIO复用功能
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2); // TX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); // RX

    // 配置GPIO
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // 配置USART
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);

    // 配置中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
}

/**
 * @brief USART2中断服务函数
 */
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);

        rx_data_byte = USART_ReceiveData(USART2);
        HWT101_ParseData(rx_data_byte);
    }

    // 清除溢出中断标志
    if (USART_GetITStatus(USART2, USART_IT_ORE) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_ORE);
    }
}

/**
 * @brief 解析HWT101数据帧
 * @param data_byte 接收到的数据字节
 */
void HWT101_ParseData(uint8_t data_byte)
{
    switch (rx_state) {
        case HWT101_IDLE:
            if (data_byte == HWT101_FRAME_HEADER) {
                rx_state           = HWT101_RECEIVING;
                rx_frame_buffer[0] = data_byte;
                rx_frame_index     = 1;
            }
            break;

        case HWT101_RECEIVING:
            rx_frame_buffer[rx_frame_index++] = data_byte;

            if (rx_frame_index >= HWT101_FRAME_LENGTH) {
                // 一帧数据接收完成
                HWT101_ProcessFrame();
                rx_state       = HWT101_IDLE;
                rx_frame_index = 0;
            }
            break;

        default:
            rx_state       = HWT101_IDLE;
            rx_frame_index = 0;
            break;
    }
}

/**
 * @brief 处理完整的HWT101数据帧
 */
void HWT101_ProcessFrame(void)
{
    // 验证帧头
    if (rx_frame_buffer[0] != HWT101_FRAME_HEADER) {
        return;
    }

    // 处理角度数据帧
    if (rx_frame_buffer[1] == HWT101_ANGLE_TYPE) {
        // 转换为角度值（0～360）
        gyro_angle_z = (float)((rx_frame_buffer[7] << 8) | rx_frame_buffer[6]) / 32768 * 180;
        // 转换为角度值（-180～180）
        gyro_angle_z_raw = (short)((rx_frame_buffer[7] << 8) | rx_frame_buffer[6]) / 32768.0 * 180;
    }
}

/**
 * @brief HWT101角度归零命令
 */
void HWT101_AngleReset(void)
{
    const uint8_t reset_cmd[] = {0xFF, 0xAA, 0x76, 0x00, 0x00};

    for (int i = 0; i < sizeof(reset_cmd); i++) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, reset_cmd[i]);
    }
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

/**
 * @brief 获取Z轴角度值
 * @return Z轴角度（度）
 */
float HWT101_GetAngleZ(void)
{
    return gyro_angle_z;
}

/**
 * @brief 获取Z轴角度原始值
 * @return Z轴角度原始值
 */
float HWT101_GetAngleZ_Raw(void)
{
    return gyro_angle_z_raw;
}

/**
 * @brief 开启 HWT101
 *
 */
void HWT101_ON(void)
{
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/**
 * @brief 关闭 HWT101
 *
 */
void HWT101_OFF(void)
{
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
}

/**
 * @brief 初始化UART5用于HWT906陀螺仪通信
 * @note PC12-TX, PD2-RX
 */
void UART5_HWT906_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    // 配置GPIO复用功能
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); // TX - PC12
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);  // RX - PD2

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    // 配置TX引脚 - PC12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 配置RX引脚 - PD2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // 配置UART5参数
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5, &USART_InitStructure);

    // 配置中断
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel                   = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能UART5
    USART_Cmd(UART5, ENABLE);
}

/**
 * @brief UART5中断服务函数
 */
void UART5_IRQHandler(void)
{
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);

        rx_data_byte_u5 = USART_ReceiveData(UART5);
        HWT906_ParseData(rx_data_byte_u5);
    }
    // 清除溢出中断标志
    if (USART_GetITStatus(UART5, USART_IT_ORE) != RESET) {
        USART_ClearITPendingBit(UART5, USART_IT_ORE);
    }
}

/**
 * @brief 解析HWT906数据帧
 * @param data_byte 接收到的数据字节
 */
void HWT906_ParseData(uint8_t data_byte)
{
    switch (rx_state_u5) {
        case HWT906_IDLE:
            if (data_byte == HWT906_FRAME_HEADER) {
                rx_state_u5           = HWT906_RECEIVING;
                rx_frame_buffer_u5[0] = data_byte;
                rx_frame_index_u5     = 1;
            }
            break;
        case HWT906_RECEIVING:
            rx_frame_buffer_u5[rx_frame_index_u5++] = data_byte;
            if (rx_frame_index_u5 >= HWT906_FRAME_LENGTH) {
                // 一帧数据接收完成
                HWT906_ProcessFrame();
                rx_state_u5       = HWT906_IDLE;
                rx_frame_index_u5 = 0;
            }
            break;
        default:
            rx_state_u5       = HWT906_IDLE;
            rx_frame_index_u5 = 0;
            break;
    }
}

/**
 * @brief 处理完整的HWT906数据帧
 */
void HWT906_ProcessFrame(void)
{
    // 验证帧头
    if (rx_frame_buffer_u5[0] != HWT906_FRAME_HEADER) {
        return;
    }

    // 处理角度数据帧
    if (rx_frame_buffer_u5[1] == HWT906_ANGLE_TYPE) {
        // 转换为角度值（0～360）
        gyro_angle_x_u5 = (float)((rx_frame_buffer_u5[3] << 8) | rx_frame_buffer_u5[2]) / 32768 * 180;
        gyro_angle_y_u5 = (float)((rx_frame_buffer_u5[5] << 8) | rx_frame_buffer_u5[4]) / 32768 * 180;
        gyro_angle_z_u5 = (float)((rx_frame_buffer_u5[7] << 8) | rx_frame_buffer_u5[6]) / 32768 * 180;

        // 转换为角度原始值（-180～180）
        gyro_angle_x_raw_u5 = (short)((rx_frame_buffer_u5[3] << 8) | rx_frame_buffer_u5[2]) / 32768.0 * 180;
        gyro_angle_y_raw_u5 = (short)((rx_frame_buffer_u5[5] << 8) | rx_frame_buffer_u5[4]) / 32768.0 * 180;
        gyro_angle_z_raw_u5 = (short)((rx_frame_buffer_u5[7] << 8) | rx_frame_buffer_u5[6]) / 32768.0 * 180;
    }
}

/**
 * @brief 获取HWT906 X轴角度值
 * @return X轴角度（度）
 */
float HWT906_GetAngleX(void)
{
    return gyro_angle_x_u5;
}

/**
 * @brief 获取HWT906 X轴角度原始值
 * @return X轴角度原始值
 */
float HWT906_GetAngleX_Raw(void)
{
    return gyro_angle_x_raw_u5;
}

/**
 * @brief 获取HWT906 Y轴角度值
 * @return Y轴角度（度）
 */
float HWT906_GetAngleY(void)
{
    return gyro_angle_y_u5;
}

/**
 * @brief 获取HWT906 Y轴角度原始值
 * @return Y轴角度原始值
 */
float HWT906_GetAngleY_Raw(void)
{
    return gyro_angle_y_raw_u5;
}

/**
 * @brief 获取HWT906 Z轴角度值
 * @return Z轴角度（度）
 */
float HWT906_GetAngleZ(void)
{
    return gyro_angle_z_u5;
}

/**
 * @brief 获取HWT906 Z轴角度原始值
 * @return Z轴角度原始值
 */
float HWT906_GetAngleZ_Raw(void)
{
    return gyro_angle_z_raw_u5;
}

/**
 * @brief 开启 HWT906
 *
 */
void HWT906_ON(void)
{
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
}

/**
 * @brief 关闭 HWT906
 *
 */
void HWT906_OFF(void)
{
    USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
}