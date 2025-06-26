#include "bsp_qr.h"
#include "bsp_lcd.h"
#include "lcd_spi_130.h"
#include "timer.h"

#define QR_END_CHAR 0X0D // 结束符

// 简化的数据结构
uint8_t qr_buffer[4];  // 只需要3位数字+结束符
uint8_t qr_index  = 0; // 当前索引
uint8_t qr_flag   = 0; // 接收完成标志
uint16_t qr_value = 0; // 二维码值

void UART4_QRCode_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = 9600;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel                   = UART4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(UART4, ENABLE);
}

void UART4_IRQHandler(void)
{
    uint8_t temp;

    if (USART_GetITStatus(UART4, USART_IT_RXNE)) {
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        temp = USART_ReceiveData(UART4);

        if (temp == QR_END_CHAR && qr_index == 3) {
            // 收到结束符且已收到3位数字，计算值
            qr_value = (qr_buffer[0] - '0') * 100 +
                       (qr_buffer[1] - '0') * 10 +
                       (qr_buffer[2] - '0');
            qr_flag  = 1;
            qr_index = 0; // 重置索引准备下次接收
        } else if (qr_index < 3 && temp >= '0' && temp <= '9') {
            // 收到数字字符
            qr_buffer[qr_index++] = temp;
        } else {
            // 接收错误，重置
            qr_index = 0;
        }
    }
}

// 获取二维码值
uint16_t QR_GetValue(void)
{
    return qr_value;
}

// 检查是否接收到新的二维码
uint8_t QR_IsReady(void)
{
    if (qr_flag) {
        qr_flag = 0; // 清除标志
        return 1;
    }
    return 0;
}

// 重置接收状态
void QR_Reset(void)
{
    qr_index = 0;
    qr_flag  = 0;
    qr_value = 0;
}

/**
 * @brief 二维码限时检测，5秒
 * @return 检测到的平台编号，0表示超时
 */
uint16_t Start_QR_Detection(void)
{
    QR_Reset();

    LCD_Clear();
    LCD_DisplayString(10, 10, "QR Scanning...");

    t6_i = 0;
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    while (t6_i < 5000) { // 5秒超时
        if (QR_IsReady()) {
            uint16_t platform = QR_GetValue();
            LCD_Clear();
            LCD_DisplayString(10, 10, "Platform:");
            LCD_DisplayNumber(100, 10, platform, 3);

            TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
            t6_i = 0;
            return platform;
        }
        Delay_ms(5);
    }

    // 超时
    LCD_Clear();
    LCD_DisplayString(10, 10, "QR Timeout!");
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    t6_i = 0;
    return 0;
}
