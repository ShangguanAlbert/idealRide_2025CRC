#include "lcd_buffer.h"
#include "bsp_lcd.h"
#include "lcd_spi_130.h"
#include "lcd_fonts.h"
#include <string.h>
#include <stdlib.h>

// 声明两个帧缓冲区
static uint16_t frameBuffer[2][BUFFER_WIDTH * BUFFER_HEIGHT];
static uint8_t currentBuffer = 0; // 当前正在绘制的缓冲区

// 初始化缓冲区
void BufferInit(void)
{
    // 可以在这里实现缓冲区初始化逻辑
    memset(frameBuffer[0], 0, sizeof(frameBuffer[0]));
    memset(frameBuffer[1], 0, sizeof(frameBuffer[1]));
    currentBuffer = 0;
}

// 获取当前活动的缓冲区
uint16_t *GetCurrentBuffer(void)
{
    return frameBuffer[currentBuffer];
}

// 交换前后缓冲区
void SwapBuffers(void)
{
    currentBuffer = 1 - currentBuffer;
}

// 清空缓冲区
void ClearBuffer(uint16_t *buffer, uint16_t color)
{
    for (uint32_t i = 0; i < BUFFER_WIDTH * BUFFER_HEIGHT; i++) {
        buffer[i] = color;
    }
}

// 在缓冲区中画点
void DrawPointToBuffer(uint16_t *buffer, int x, int y, uint16_t color)
{
    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
        buffer[y * BUFFER_WIDTH + x] = color;
    }
}

// 在缓冲区中绘制线条
void DrawLineToBuffer(uint16_t *buffer, int x1, int y1, int x2, int y2, uint16_t color)
{
    int dx  = abs(x2 - x1);
    int dy  = abs(y2 - y1);
    int sx  = (x1 < x2) ? 1 : -1;
    int sy  = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        DrawPointToBuffer(buffer, x1, y1, color);

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// 在缓冲区中填充矩形
void FillRectToBuffer(uint16_t *buffer, int x, int y, int width, int height, uint16_t color)
{
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            DrawPointToBuffer(buffer, x + i, y + j, color);
        }
    }
}

// 在缓冲区中画矩形边框
void DrawRectToBuffer(uint16_t *buffer, int x, int y, int width, int height, uint16_t color)
{
    // 绘制水平线
    for (int i = 0; i < width; i++) {
        DrawPointToBuffer(buffer, x + i, y, color);
        DrawPointToBuffer(buffer, x + i, y + height - 1, color);
    }
    // 绘制垂直线
    for (int i = 0; i < height; i++) {
        DrawPointToBuffer(buffer, x, y + i, color);
        DrawPointToBuffer(buffer, x + width - 1, y + i, color);
    }
}

// 在缓冲区中绘制圆
void DrawCircleToBuffer(uint16_t *buffer, int xc, int yc, int r, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        DrawPointToBuffer(buffer, xc + x, yc + y, color);
        DrawPointToBuffer(buffer, xc - x, yc + y, color);
        DrawPointToBuffer(buffer, xc + x, yc - y, color);
        DrawPointToBuffer(buffer, xc - x, yc - y, color);
        DrawPointToBuffer(buffer, xc + y, yc + x, color);
        DrawPointToBuffer(buffer, xc - y, yc + x, color);
        DrawPointToBuffer(buffer, xc + y, yc - x, color);
        DrawPointToBuffer(buffer, xc - y, yc - x, color);

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 在缓冲区中绘制填充圆
void FillCircleToBuffer(uint16_t *buffer, int xc, int yc, int r, uint16_t color)
{
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                DrawPointToBuffer(buffer, xc + x, yc + y, color);
            }
        }
    }
}

// 在缓冲区中绘制数字 - 简化版本，仅支持数字
void DrawDigitToBuffer(uint16_t *buffer, int x, int y, char digit, uint16_t color)
{
    // 简单的5x7点阵字体数字
    const uint8_t digits[10][5] = {
        {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
        {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
        {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
        {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
        {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
        {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
        {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
        {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
        {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
        {0x06, 0x49, 0x49, 0x29, 0x1E}  // 9
    };

    if (digit >= '0' && digit <= '9') {
        int idx = digit - '0';
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 8; j++) {
                if ((digits[idx][i] >> j) & 0x01) {
                    DrawPointToBuffer(buffer, x + i, y + j, color);
                }
            }
        }
    }
}

// 在缓冲区中绘制文本
void DrawTextToBuffer(uint16_t *buffer, int x, int y, const char *text, uint16_t color)
{
    int xOffset = 0;
    while (*text) {
        DrawDigitToBuffer(buffer, x + xOffset, y, *text, color);
        xOffset += 6; // 字符宽度+间距
        text++;
    }
}

// 在缓冲区中绘制字符 - 使用ASCII_Font32，修复版本
void DrawASCII32CharToBuffer(uint16_t *buffer, int x, int y, char ch, uint16_t color)
{
    // 计算字符在字体表中的偏移
    uint8_t offset = ch - ' '; // 空格是第一个字符

    // 使用字体结构体已有的成员
    const uint8_t *pCharData = ASCII_Font32.pTable + (offset * ASCII_Font32.Sizes);

    // 遍历字符的每一行 (32行)
    for (uint16_t row = 0; row < ASCII_Font32.Height; row++) {
        // 尝试反向字节顺序
        uint16_t lineData = (pCharData[row * 2 + 1] << 8) | pCharData[row * 2];

        // 遍历该行的每一位 (16列)
        for (uint16_t col = 0; col < ASCII_Font32.Width; col++) {
            // 从低位开始检查 (LSB优先)
            if ((lineData >> col) & 0x0001) {
                DrawPointToBuffer(buffer, x + col, y + row, color);
            }
        }
    }
}

// 在缓冲区中绘制ASCII_Font32字符串
void DrawASCII32TextToBuffer(uint16_t *buffer, int x, int y, const char *text, uint16_t color)
{
    int xOffset = 0;
    while (*text) {
        DrawASCII32CharToBuffer(buffer, x + xOffset, y, *text, color);
        xOffset += ASCII_Font32.Width + 1; // 字符间距为1像素
        text++;
    }
}

// 修改显示数字的函数，使用ASCII_Font32字体在屏幕中央显示
void DisplayCenteredNumber(uint16_t *buffer, const char *numStr, uint16_t color)
{
    // 计算文本宽度
    int textLength = strlen(numStr);
    int textWidth  = textLength * ASCII_Font32.Width + (textLength - 1) * 1; // 加上字符间距
    int textHeight = ASCII_Font32.Height;

    // 计算居中位置
    int centerX = (BUFFER_WIDTH - textWidth) / 2;
    int centerY = (BUFFER_HEIGHT - textHeight) / 2;

    // 在缓冲区中绘制文本
    DrawASCII32TextToBuffer(buffer, centerX, centerY, numStr, color);
}

// 将缓冲区内容刷新到LCD屏幕
void FlushBufferToLCD(uint16_t *buffer, int startX, int startY, int width, int height)
{
    // 设置显示区域并写入数据
    LCD_SetAddress(startX,
                   startY,
                   startX + width - 1,
                   startY + height - 1);

    LCD_WriteBuff(buffer, width * height);
}