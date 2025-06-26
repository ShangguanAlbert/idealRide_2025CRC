#include "bsp_lcd.h"
#include "lcd_buffer.h"
#include "arm_math.h"
#include "bsp_led.h"
#include "lcd_spi_130.h"
#include "bsp_grayscale.h"
#include "bsp_pe.h"
#include "bsp_qr.h"
#include "bsp_gyro.h"
#include "bsp_vision.h"
#include "bsp_rgb.h"
#include "bsp_key.h"
#include "bsp_SysTick.h"
#include "bsp_ccd.h"

// 菜单配置参数
#define MENU_CENTER_X     (BUFFER_WIDTH / 2)  // 相对于缓冲区的中心X坐标
#define MENU_CENTER_Y     (BUFFER_HEIGHT / 2) // 相对于缓冲区的中心Y坐标
#define MENU_ITEM_WIDTH   120
#define MENU_ITEM_HEIGHT  120
#define MENU_ITEM_SPACING 20

typedef struct {
    uint8_t id;
} MenuProgram;

uint8_t page = 1;
extern uint8_t prog_num_hope;

/**
 * @brief 绘制菜单到缓冲区
 *
 * @param buffer 缓冲区
 * @param selectedIndex 选择的程序编号
 * @param totalPrograms 总程序数量
 */
void DrawMenuToBuffer(uint16_t *buffer, uint8_t selectedIndex, uint8_t totalPrograms)
{
    // 清空缓冲区为背景色
    ClearBuffer(buffer, BACKGROUND_COLOR);

    // 绘制矩形框占据整个缓冲区
    // 外框
    DrawRectToBuffer(buffer,
                     0,
                     0,
                     BUFFER_WIDTH,
                     BUFFER_HEIGHT,
                     RGB565(0x00, 0xFF, 0xFF)); // 青色边框

    // 内框，稍微内缩2像素
    DrawRectToBuffer(buffer,
                     2,
                     2,
                     BUFFER_WIDTH - 4,
                     BUFFER_HEIGHT - 4,
                     RGB565(0x00, 0xFF, 0xFF)); // 青色边框

    // 绘制选中项背景，放在矩形框中央
    FillRectToBuffer(buffer,
                     BUFFER_WIDTH / 2 - MENU_ITEM_WIDTH / 2,
                     BUFFER_HEIGHT / 2 - MENU_ITEM_HEIGHT / 2,
                     MENU_ITEM_WIDTH,
                     MENU_ITEM_HEIGHT,
                     RGB565(0xA5, 0x2A, 0x2A)); // 选中项蓝色背景

    // 绘制数字 - 在选中项背景中央
    char numStr[4];
    sprintf(numStr, "%d", selectedIndex + 1);

    // 使用 ASCII_Font32 显示居中数字
    DisplayCenteredNumber(buffer, numStr, RGB565(0x00, 0xFF, 0xFF)); // 数字颜色为青色

    // 绘制指示器点 - 在底部居中
    int dotsWidth  = totalPrograms * 10 - 5; // 点的总宽度
    int dotsStartX = (BUFFER_WIDTH - dotsWidth) / 2;

    for (uint8_t i = 0; i < totalPrograms; i++) {
        int dotX = dotsStartX + i * 10;
        int dotY = BUFFER_HEIGHT - 20; // 底部上方20像素

        if (i == selectedIndex) {
            FillCircleToBuffer(buffer,
                               dotX,
                               dotY,
                               4,
                               0x00FFFF);
        } else {
            DrawCircleToBuffer(buffer,
                               dotX,
                               dotY,
                               3,
                               0x00FFFF);
        }
    }
}

uint8_t Function_Mode(void)
{
    // 初始化缓冲区
    BufferInit();

    LCD_SetDirection(Direction_V);

    // 程序菜单项定义
    MenuProgram programs[] = {
        {1},
        {2},
        {3},
        {4},
        {5},
        {6},
        {7},
        {8},
        {9}};

    const uint8_t totalPrograms = sizeof(programs) / sizeof(programs[0]);
    uint8_t selectedIndex       = prog_num_hope - 1; // 当前选中的索引
    if (selectedIndex >= totalPrograms) selectedIndex = 0;

    uint8_t Enter_Key_Press = 0;
    uint8_t needRedraw      = 1; // 是否需要重绘界面

    // 绘制标题
    LCD_Clear();
    LCD_SetColor(0xFFFFFF);
    LCD_DrawRect(1, 1, 238, 238);
    LCD_DrawLine(0, 1, 240, 1);
    LCD_SetAsciiFont(&ASCII_Font24);
    // 计算标题文本居中位置
    char *titleText = "CRC 2025 HZNU";
    int titleLength = strlen(titleText);
    int charWidth   = ASCII_Font24.Width;      // ASCII_Font20的字符宽度
    int titleWidth  = titleLength * charWidth; // 总文本宽度
    int centerX     = (240 - titleWidth) / 2;  // LCD宽度240，计算居中X坐标

    LCD_DisplayString(centerX, 5, titleText); // 使用计算出的居中X坐标
    LCD_DrawLine(0, 32, 240, 32);

    // 主循环
    while (Enter_Key_Press == 0) {
        // 处理按键输入
        uint8_t key = KEY_Scan(0);
        if (key != 0) {
            switch (key) {
                case 1: // 上一个菜单（左移）
                    if (selectedIndex > 0) {
                        selectedIndex--;
                    } else {
                        selectedIndex = totalPrograms - 1; // 从头跳到尾
                    }
                    needRedraw = 1;
                    break;

                case 3: // 下一个菜单（右移）
                    if (selectedIndex < totalPrograms - 1) {
                        selectedIndex++;
                    } else {
                        selectedIndex = 0; // 从尾跳到头
                    }
                    needRedraw = 1;
                    break;

                case 2: // 确认选择
                    Enter_Key_Press = 1;
                    break;
            }
        }

        // 只有在需要重绘时才更新屏幕
        if (needRedraw) {
            uint16_t *currentBuffer = GetCurrentBuffer();

            // 在当前缓冲区中绘制整个界面
            DrawMenuToBuffer(currentBuffer, selectedIndex, totalPrograms);

            // 将缓冲区内容刷新到LCD
            FlushBufferToLCD(currentBuffer,
                             BUFFER_START_X,
                             BUFFER_START_Y,
                             BUFFER_WIDTH,
                             BUFFER_HEIGHT);

            // 切换缓冲区
            SwapBuffers();
            needRedraw = 0;
        }

        // 短暂延时，减少CPU占用
        Delay_ms(10);
    }

    // 显示选择结果
    LCD_SetAsciiFont(&ASCII_Font24);
    LCD_ClearRect(0, 29, 240, 210);
    LCD_SetColor(0x00FF00);

    // 使用 ASCII_Font32 显示大号数字
    uint16_t *currentBuffer = GetCurrentBuffer();
    ClearBuffer(currentBuffer, BACKGROUND_COLOR);

    char numStr[4];
    sprintf(numStr, "%d", selectedIndex + 1);

    // 使用 ASCII_Font32 字体在缓冲区中心显示数字
    DisplayCenteredNumber(currentBuffer, numStr, 0x00FF00);

    FlushBufferToLCD(currentBuffer,
                     BUFFER_START_X,
                     BUFFER_START_Y,
                     BUFFER_WIDTH,
                     BUFFER_HEIGHT);

    LCD_SetAsciiFont(&ASCII_Font24);
    LCD_DisplayString(90, 180, "Ready!");

    Delay_ms(500);
    LCD_Clear();
    LCD_SetAsciiFont(&ASCII_Font24);

    return selectedIndex + 1; // 返回选中的程序ID
}

/**
 * @brief 显示程序运行信息
 *
 * @param prog_name 程序名称
 * @param prog_desc 程序描述
 */
void Progarm_Running_Info(char *prog_name, char *prog_desc)
{
    LCD_Clear();
    LCD_SetAsciiFont(&ASCII_Font24);
    LCD_SetColor(0x00FF00);

    // 程序名称居中位置
    int nameLength  = strlen(prog_name);
    int nameWidth   = nameLength * ASCII_Font24.Width;
    int nameCenterX = (240 - nameWidth) / 2;

    // 在顶部显示程序名称（居中）
    LCD_DisplayString(nameCenterX, 20, prog_name);

    // 计算程序描述居中位置
    int descLength  = strlen(prog_desc);
    int descWidth   = descLength * ASCII_Font24.Width;
    int descCenterX = (240 - descWidth) / 2;

    // 在程序名称下方显示描述（居中）
    LCD_DisplayString(descCenterX, 60, prog_desc);

    Delay_ms(500); // 等待500ms
    LCD_Clear();   // 清屏
}

/**
 * @brief 检测按键按下边沿
 * @param current_state 当前按键状态 (0=按下, 1=未按下)
 * @param last_state 上次按键状态的指针
 * @return 1表示检测到按下边沿, 0表示没有
 */
uint8_t DetectKeyPress(uint8_t current_state, uint8_t *last_state)
{
    uint8_t result = 0;
    if (current_state == 0 && *last_state == 1) {
        result = 1; // 检测到按下边沿
    }
    *last_state = current_state; // 更新状态记录
    return result;
}

void Show_SensorPage_All(void)
{
    LCD_SetDirection(Direction_V);
    LCD_ShowNumMode(Fill_Space);
    page = 1; // 初始化页面为第1页
    LCD_Clear();

    // 按键状态记录变量
    static uint8_t last_keydown_state = 1; // 记录上次KeyDown状态
    static uint8_t last_keyup_state   = 1; // 记录上次KeyUp状态

    while (1) {
        // 根据当前页面显示对应内容
        switch (page) {
            case 1:
                Sensor_Test_Page1();
                break;
            case 2:
                Sensor_Test_Page2();
                break;
            case 3:
                Sensor_Test_Page3();
                break;
            case 4:
                Sensor_Test_Page4();
                break;
            default:
                page = 1; // 防止页面值异常
                break;
        }

        // 检测下一页按键
        if (DetectKeyPress(KeyDown, &last_keydown_state)) {
            page++;
            if (page > 4) {
                page = 1; // 循环到第1页
            }
            LCD_Clear();
        }

        // 检测上一页按键
        if (DetectKeyPress(KeyUp, &last_keyup_state)) {
            page--;
            if (page < 1) {
                page = 4; // 循环到第4页
            }
            LCD_Clear();
        }

        Delay_ms(50); // 主循环延时，减少CPU占用
    }
}
/**
 * @brief 显示传感器测试页面1 - 前置灰度传感器
 */
void Sensor_Test_Page1(void)
{
    LCD_SetColor(0xFFFFFF); // 白色
    // 标题设置和居中显示
    LCD_SetAsciiFont(&ASCII_Font24);
    char *title      = "Front Huidu";
    int titleLength  = strlen(title);
    int titleWidth   = titleLength * ASCII_Font24.Width;
    int titleCenterX = (240 - titleWidth) / 2;
    LCD_DisplayString(titleCenterX, 5, title);

    // 绘制标题下方的分割线
    LCD_DrawLine(0, 32, 240, 32);

    // 显示页面指示器（右上角）
    LCD_SetAsciiFont(&ASCII_Font16);
    LCD_DisplayString(210, 5, "1/4");

    // 绘制中间的垂直分割线，将页面分为两栏
    LCD_DrawLine_V(120, 32, 240);

    // 设置数据显示字体
    LCD_SetAsciiFont(&ASCII_Font20);

    // 左栏显示FH0-FH5（6个传感器）
    int leftColumnX = 10;
    int valueX      = 65;
    int startY      = 45;
    int lineHeight  = 28;

    LCD_DisplayString(leftColumnX, startY, "FH0:");
    LCD_DisplayNumber(valueX, startY, Huidu_va(0), 4);

    LCD_DisplayString(leftColumnX, startY + lineHeight, "FH1:");
    LCD_DisplayNumber(valueX, startY + lineHeight, Huidu_va(1), 4);

    LCD_DisplayString(leftColumnX, startY + lineHeight * 2, "FH2:");
    LCD_DisplayNumber(valueX, startY + lineHeight * 2, Huidu_va(2), 4);

    LCD_DisplayString(leftColumnX, startY + lineHeight * 3, "FH3:");
    LCD_DisplayNumber(valueX, startY + lineHeight * 3, Huidu_va(3), 4);

    LCD_DisplayString(leftColumnX, startY + lineHeight * 4, "FH4:");
    LCD_DisplayNumber(valueX, startY + lineHeight * 4, Huidu_va(4), 4);

    LCD_DisplayString(leftColumnX, startY + lineHeight * 5, "FH5:");
    LCD_DisplayNumber(valueX, startY + lineHeight * 5, Huidu_va(5), 4);

    // 右栏显示FH6-FH11（6个传感器）
    int rightColumnX = 130;
    int rightValueX  = 185;

    LCD_DisplayString(rightColumnX, startY, "FH6:");
    LCD_DisplayNumber(rightValueX, startY, Huidu_va(6), 4);

    LCD_DisplayString(rightColumnX, startY + lineHeight, "FH7:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight, Huidu_va(7), 4);

    LCD_DisplayString(rightColumnX, startY + lineHeight * 2, "FH8:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 2, Huidu_va(8), 4);

    LCD_DisplayString(rightColumnX, startY + lineHeight * 3, "FH9:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 3, Huidu_va(9), 4);

    LCD_DisplayString(rightColumnX, startY + lineHeight * 4, "FH10:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 4, Huidu_va(10), 4);

    LCD_DisplayString(rightColumnX, startY + lineHeight * 5, "FH11:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 5, Huidu_va(11), 4);
}
/**
 * @brief 显示传感器测试页面2 - 数字传感器
 */
void Sensor_Test_Page2(void)
{
    LCD_SetColor(0xFFFFFF); // 白色
    // 标题设置和居中显示
    LCD_SetAsciiFont(&ASCII_Font24);
    char *title      = "PE Sensor";
    int titleLength  = strlen(title);
    int titleWidth   = titleLength * ASCII_Font24.Width;
    int titleCenterX = (240 - titleWidth) / 2;
    LCD_DisplayString(titleCenterX, 5, title);

    // 绘制标题下方的分割线
    LCD_DrawLine(0, 32, 240, 32);

    // 显示页面指示器（右上角）
    LCD_SetAsciiFont(&ASCII_Font16);
    LCD_DisplayString(210, 5, "2/4");

    // 绘制中间的垂直分割线，将页面分为两栏
    LCD_DrawLine_V(120, 32, 160);

    // 设置数据显示字体
    LCD_SetAsciiFont(&ASCII_Font20);

    // 左栏显示PE0-PE3（4个光电开关）
    int leftColumnX = 10;
    int leftValueX  = 65;
    int startY      = 45;
    int lineHeight  = 35;

    LCD_DisplayString(leftColumnX, startY, "PE0:");
    LCD_DisplayNumber(leftValueX, startY, PE_Value(0), 1);

    LCD_DisplayString(leftColumnX, startY + lineHeight, "PE1:");
    LCD_DisplayNumber(leftValueX, startY + lineHeight, PE_Value(1), 1);

    LCD_DisplayString(leftColumnX, startY + lineHeight * 2, "PE2:");
    LCD_DisplayNumber(leftValueX, startY + lineHeight * 2, PE_Value(2), 1);

    LCD_DisplayString(leftColumnX, startY + lineHeight * 3, "PE3:");
    LCD_DisplayNumber(leftValueX, startY + lineHeight * 3, PE_Value(3), 1);

    // 右栏显示PE4-PE7（4个光电开关）
    int rightColumnX = 130;
    int rightValueX  = 185;

    LCD_DisplayString(rightColumnX, startY, "PE4:");
    LCD_DisplayNumber(rightValueX, startY, PE_Value(4), 1);

    LCD_DisplayString(rightColumnX, startY + lineHeight, "PE5:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight, PE_Value(5), 1);

    LCD_DisplayString(rightColumnX, startY + lineHeight * 2, "PE6:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 2, PE_Value(6), 1);

    LCD_DisplayString(rightColumnX, startY + lineHeight * 3, "PE7:");
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 3, PE_Value(7), 1);

    // 在底部添加状态说明
    LCD_DrawLine(0, 190, 240, 190);
    LCD_SetAsciiFont(&ASCII_Font16);
    LCD_DisplayString(10, 200, "Status: 1=No Object");
    LCD_DisplayString(10, 220, "        0=Object Detected");
}

/**
 * @brief 显示传感器测试页面3 - 系统状态与其他传感器
 */
void Sensor_Test_Page3(void)
{
    LCD_ShowNumMode(Fill_Space);

    // 标题设置和居中显示
    LCD_SetAsciiFont(&ASCII_Font24);
    char *title      = "Gyroscope";
    int titleLength  = strlen(title);
    int titleWidth   = titleLength * ASCII_Font24.Width;
    int titleCenterX = (240 - titleWidth) / 2;
    LCD_DisplayString(titleCenterX, 5, title);

    // 绘制标题下方的分割线
    LCD_DrawLine(0, 32, 240, 32);

    // 显示页面指示器（右上角）
    LCD_SetAsciiFont(&ASCII_Font16);
    LCD_DisplayString(210, 5, "3/4");

    // ==================== HWT101 单轴陀螺仪显示区域 ====================
    LCD_SetAsciiFont(&ASCII_Font20);
    LCD_SetColor(0x00FFFF); // 青色
    LCD_DisplayString(10, 40, "HWT101:");
    LCD_SetColor(0xFFFFFF); // 恢复白色

    LCD_SetAsciiFont(&ASCII_Font20);
    // 显示Z轴角度（度）
    LCD_SetColor(0x00FFFF); // 青色
    LCD_DisplayString(10, 65, "Z-Angle:");
    LCD_SetColor(0xFFFFFF); // 恢复白色
    LCD_DisplayNumber(90, 65, gyro_angle_z, 4);

    // 显示Z轴原始角度值
    LCD_SetColor(0xFF8000); // 橙色
    LCD_DisplayString(10, 90, "Z-R:");
    LCD_SetColor(0xFFFFFF); // 恢复白色
    LCD_DisplayNumber(90, 90, gyro_angle_z_raw, 4);

    // ==================== HWT906 九轴陀螺仪显示区域 ====================
    LCD_DrawLine(0, 115, 240, 115); // 分割线

    LCD_SetAsciiFont(&ASCII_Font20);
    LCD_SetColor(0x00FF00); // 绿色
    LCD_DisplayString(10, 120, "HWT906:");
    LCD_SetColor(0xFFFFFF); // 恢复白色

    // 调整布局参数 - 修正起点位置，使用更大字体
    int leftColumnX  = 10;  // 左栏标签起始X
    int leftValueX   = 50;  // 左栏数值起始X
    int rightColumnX = 125; // 右栏标签起始X
    int rightValueX  = 180; // 右栏数值起始X
    int startY       = 150; // 修正起始Y位置，应该在标题下方
    int lineHeight   = 25;  // 增加行间距

    LCD_SetAsciiFont(&ASCII_Font20); // 使用更大的字体

    // X轴显示 - 左右分栏布局
    LCD_SetColor(0xFF0000); // 红色
    LCD_DisplayString(leftColumnX, startY, "X:");
    LCD_SetColor(0xFFFFFF); // 白色
    LCD_DisplayNumber(leftValueX, startY, gyro_angle_x_u5, 4);

    LCD_SetColor(0xFF8000); // 橙色
    LCD_DisplayString(rightColumnX, startY, "X-R:");
    LCD_SetColor(0xFFFFFF); // 白色
    LCD_DisplayNumber(rightValueX, startY, gyro_angle_x_raw_u5, 4);

    // Y轴显示
    LCD_SetColor(0x00FF00); // 绿色
    LCD_DisplayString(leftColumnX, startY + lineHeight, "Y:");
    LCD_SetColor(0xFFFFFF); // 白色
    LCD_DisplayNumber(leftValueX, startY + lineHeight, gyro_angle_y_u5, 4);

    LCD_SetColor(0x80FF00); // 黄绿色
    LCD_DisplayString(rightColumnX, startY + lineHeight, "Y-R:");
    LCD_SetColor(0xFFFFFF); // 白色
    LCD_DisplayNumber(rightValueX, startY + lineHeight, gyro_angle_y_raw_u5, 4);

    // Z轴显示
    LCD_SetColor(0x0080FF); // 蓝色
    LCD_DisplayString(leftColumnX, startY + lineHeight * 2, "Z:");
    LCD_SetColor(0xFFFFFF); // 白色
    LCD_DisplayNumber(leftValueX, startY + lineHeight * 2, gyro_angle_z_u5, 4);

    LCD_SetColor(0x8080FF); // 淡蓝色
    LCD_DisplayString(rightColumnX, startY + lineHeight * 2, "Z-R:");
    LCD_SetColor(0xFFFFFF); // 白色
    LCD_DisplayNumber(rightValueX, startY + lineHeight * 2, gyro_angle_z_raw_u5, 4);
}
/**
 * @brief 显示传感器测试页面4 - QRcode OpenMV
 */
void Sensor_Test_Page4(void)
{
    LCD_ShowNumMode(Fill_Space);
    LCD_SetColor(0xFFFFFF); // 白色

    // 标题设置和居中显示
    LCD_SetAsciiFont(&ASCII_Font24);
    char *title      = "Vision Sensors";
    int titleLength  = strlen(title);
    int titleWidth   = titleLength * ASCII_Font24.Width;
    int titleCenterX = (240 - titleWidth) / 2;
    LCD_DisplayString(titleCenterX, 5, title);

    // 绘制标题下方的分割线
    LCD_DrawLine(0, 32, 240, 32);

    // 显示页面指示器（右上角）
    LCD_SetAsciiFont(&ASCII_Font16);
    LCD_DisplayString(210, 5, "4/4");

    // ==================== 上部分：QR Code ====================
    LCD_SetAsciiFont(&ASCII_Font20);
    LCD_SetColor(0x00FFFF); // 青色
    char *qrTitle      = "QR Code:";
    int qrTitleLength  = strlen(qrTitle);
    int qrTitleWidth   = qrTitleLength * ASCII_Font24.Width;
    int qrTitleCenterX = (240 - qrTitleWidth) / 2;
    LCD_DisplayString(qrTitleCenterX, 40, qrTitle);

    // QR Code 数值 - 使用大字体居中显示
    LCD_SetAsciiFont(&ASCII_Font24);
    LCD_SetColor(0x00FF00); // 绿色
    LCD_DisplayNumber(115, 65, QR_GetValue(), 2);

    // 第一条分割线
    LCD_SetColor(0xFFFFFF);
    LCD_DrawLine(0, 90, 240, 90);

    // ==================== 中部分：OpenMV ====================
    LCD_SetAsciiFont(&ASCII_Font20);
    LCD_SetColor(0xFF8000); // 橙色
    char *mvTitle      = "OpenMV:";
    int mvTitleLength  = strlen(mvTitle);
    int mvTitleWidth   = mvTitleLength * ASCII_Font24.Width;
    int mvTitleCenterX = (240 - mvTitleWidth) / 2;
    LCD_DisplayString(mvTitleCenterX, 100, mvTitle);

    // OpenMV 数值
    LCD_SetAsciiFont(&ASCII_Font24);
    LCD_SetColor(0xFFFF00); // 黄色
    LCD_DisplayNumber(115, 125, Get_OpenMV_Data(), 2);

    // 第二条分割线
    LCD_SetColor(0xFFFFFF);
    LCD_DrawLine(0, 150, 240, 150);

    // ==================== 下部分：RGB传感器 ====================
    LCD_SetAsciiFont(&ASCII_Font20);
    LCD_SetColor(0xFF00FF); // 紫色
    char *rgbTitle      = "RGB Sensor:";
    int rgbTitleLength  = strlen(rgbTitle);
    int rgbTitleWidth   = rgbTitleLength * ASCII_Font24.Width;
    int rgbTitleCenterX = (240 - rgbTitleWidth) / 2;
    LCD_DisplayString(rgbTitleCenterX, 160, rgbTitle);
    
    // RGB 值显示 - 使用左右分栏布局
    LCD_SetAsciiFont(&ASCII_Font16);
    int startY = 185;
    int lineHeight = 20;
    RGB_Read(); // 读取RGB传感器数据
    // 左栏显示R和G
    LCD_SetColor(0xFF0000); // 红色
    LCD_DisplayString(20, startY, "R:");
    LCD_SetColor(0xFFFFFF);
    LCD_DisplayNumber(40, startY, RGB_GetRed(), 3);

    LCD_SetColor(0x00FF00); // 绿色  
    LCD_DisplayString(20, startY + lineHeight, "G:");
    LCD_SetColor(0xFFFFFF);
    LCD_DisplayNumber(40, startY + lineHeight, RGB_GetGreen(), 3);

    // 右栏显示B
    LCD_SetColor(0x0080FF); // 蓝色
    LCD_DisplayString(130, startY, "B:");
    LCD_SetColor(0xFFFFFF);
    LCD_DisplayNumber(150, startY, RGB_GetBlue(), 3);

    LCD_SetColor(0xFFFFFF); // 恢复白色
}
