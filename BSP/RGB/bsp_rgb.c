#include "bsp_rgb.h"
#include "bsp_SysTick.h"

uint8_t RGB[3] = {0, 0, 0};

void MyI2C_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9; // SDA and SCL
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; // Pull-up for I2C
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Set SDA and SCL high
    GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
}

void I2C_RGB_Init(void)
{
    MyI2C_Init();
}

void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);
    Delay_us(4);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);
    Delay_us(4);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
    Delay_us(4);
    return BitValue;
}

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i++) {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA() == 1) { Byte |= (0x80 >> i); }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}

void RGB_Read(void)
{
    MyI2C_Start();
    MyI2C_SendByte(0x9E);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(0xD0);
    MyI2C_ReceiveAck();

    MyI2C_Start();
    MyI2C_SendByte(0x9F);
    MyI2C_ReceiveAck();

    RGB[0] = MyI2C_ReceiveByte();
    MyI2C_SendAck(0);
    RGB[1] = MyI2C_ReceiveByte();
    MyI2C_SendAck(0);
    RGB[2] = MyI2C_ReceiveByte();
    MyI2C_SendAck(1); // Send NACK for the last byte

    MyI2C_Stop();

    // Delay_ms(10);
}

uint8_t RGB_GetRed(void)
{
    return RGB[0];
}

uint8_t RGB_GetGreen(void)
{
    return RGB[1];
}

uint8_t RGB_GetBlue(void)
{
    return RGB[2];
}
