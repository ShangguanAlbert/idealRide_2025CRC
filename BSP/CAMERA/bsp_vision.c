#include "bsp_vision.h"

int openmv[4];
int8_t OpenMV;

/**
 * @brief 串口1初始化函数, 并且开启NVIC中断配置 115200
 */
void USART1_OpenMV_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void Openmv_Data(void)
{
    OpenMV = openmv[2];
}

void Openmv_Receive_Data(int16_t data)
{
    static uint8_t state = 0;
    if (state == 0 && data == 0xb3) {
        state     = 1;
        openmv[0] = data;
    } else if (state == 1 && data == 0xb2) {
        state     = 2;
        openmv[1] = data;
    } else if (state == 2) {
        state     = 3;
        openmv[2] = data;
    } else if (state == 3) {
        if (data == 0xb5) {
            state     = 0;
            openmv[3] = data;
            Openmv_Data();
            data = 0;
        } else if (data != 0xb5) {
            state = 0;
            for (int m = 0; m < 4; m++) {
                openmv[m] = 0x00;
            }
        }
    } else {
        state = 0;
        data  = 0;
        for (int m = 0; m < 4; m++) {
            openmv[m] = 0x00;
        }
    }
}

void USART1_IRQHandler(void)
{
    uint8_t com_data;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        com_data = USART_ReceiveData(USART1);
        Openmv_Receive_Data(com_data);
        Openmv_Data();
        com_data = 0;
    }
}

/**
 * @brief 获取OpenMV数据
 * @return OpenMV数据
 */
int Get_OpenMV_Data(void)
{
    return openmv[2]; // 返回OpenMV数据
}
