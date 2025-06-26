#include "main.h"

uint8_t prog;
uint8_t prog_num_hope = 1;

int main(void)
{
    // 系统初始化
    Robot_Init();
    // 选程序
    prog = Function_Mode();
    // 执行选中的程序
    ExecuteProgram(prog);
    while (1) {
    };
}

/**
 * @brief 系统初始化
 *
 */
void Robot_Init(void)
{
    SysTick_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 初始化灰度传感器
    ADC1_Init();
    // 初始化液晶屏
    SPI_LCD_Init();
    // 检查复位源
    // Check_Reset_Source();

    // 电机初始化
    TIM1_Motor_Init();
    // 舵机初始化
    TIM2_SERVO_Init();
    TIM8_SERVO_Init();

    // 初始化串口1用于OpenMV
    USART1_OpenMV_Init(); // 3-2
    // 初始化二维码扫描
    UART4_QRCode_Init(); // 3-3
    // 初始化HWT101陀螺仪
    USART2_HWT101_Init(); // 1-1
    // 初始化HWT906陀螺仪
    UART5_HWT906_Init(); // 1-2

    // 初始化定时器
    TIM6_Aid_Init();   // 2-1
    TIM7_Trace_Init(); // 2-2

    // 初始化CCD
    CCD_Init();

    // 初始化光电传感器
    PhotoElectric_Init();

    // 初始化 RGB 颜色传感器
    I2C_RGB_Init();
    // 初始化LED
    LED_Init();
    // 初始化按键
    KEY_Init();

    // 初始化语音模块
    GPIO_VOICE_Init();
}

/**
 * @brief 根据程序编号执行对应程序
 * @param programNumber 程序编号 (1-9)
 */
void ExecuteProgram(uint8_t programNumber)
{
    LED2_ON(); // 指示程序正在运行

    switch (programNumber) {
        case 1:
            Program_1();
            break;
        case 2:
            Program_2();
            break;
        case 3:
            Program_3();
            break;
        case 4:
            Program_4();
            break;
        case 5:
            Program_5();
            break;
        case 6:
            Program_6();
            break;
        case 7:
            Program_7();
            break;
        case 8:
            Program_8();
            break;
        case 9:
            Program_9();
        default:
            // 无效程序编号，显示错误信息
            LCD_Clear();
            LCD_SetAsciiFont(&ASCII_Font24);
            LCD_SetColor(0xF800); // 红色
            LCD_DisplayString(60, 100, "Invalid Program!");
            break;
    }
    LED2_OFF(); // 程序执行完毕
}

/**
 * @brief 程序1
 */
void Program_1(void)
{
    Progarm_Running_Info("Program 1", "Running");
    // 程序1的具体实现
    // while (1) { Start_QR_Detection(); }
    Start_QR_Detection(); // 启动二维码检测
}

/**
 * @brief 程序2
 */
void Program_2(void)
{
    Progarm_Running_Info("Program 2", "Running");
}

/**
 * @brief 程序3
 */
void Program_3(void)
{
    Progarm_Running_Info("Program 3", "Running");

    // CCD测试
    while (1) {
        CCD_Trigger_Collection();                               // 触发CCD采集
        CCD_GetValue();                                         // 获取CCD数据
        LCD_DisplayNumber(0, 20, (int16_t)ADC_CCD_Value[0], 4); // 显示CCD第一个通道的值
    }
}

/**
 * @brief 程序4
 */
void Program_4(void)
{
    Progarm_Running_Info("Program 4", "Running");
    // 程序4的具体实现
}

/**
 * @brief 程序5
 */
void Program_5(void)
{
    Progarm_Running_Info("Program 5", "Running");
    // 程序5的具体实现
}

/**
 * @brief 程序6
 */
void Program_6(void)
{
    Progarm_Running_Info("Program 6", "Running");
    // 程序6的具体实现
}

/**
 * @brief 程序7
 */
void Program_7(void)
{
    Progarm_Running_Info("Program 7", "Running");
    // 程序7的具体实现
}

/**
 * @brief 程序8
 */
void Program_8(void)
{
    Progarm_Running_Info("Program 8", "Running");
}

/**
 * @brief 程序9  - 传感器测试
 */
void Program_9(void)
{
    Progarm_Running_Info("Program 9", "Running");

    Show_SensorPage_All(); // 显示传感器测试页面
}