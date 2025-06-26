#ifndef __LCD_BUFFER_H
#define __LCD_BUFFER_H
#include "stm32f4xx.h"
#include <stdint.h>

// 缓冲区配置
#define BUFFER_WIDTH   140 // 缓冲区宽度
#define BUFFER_HEIGHT  140 // 缓冲区高度
#define BUFFER_START_X ((240 - BUFFER_WIDTH) / 2)
#define BUFFER_START_Y ((240 - BUFFER_HEIGHT) / 2)

// 绘图颜色定义
#define BACKGROUND_COLOR 0x000000 // 黑色背景
// RGB888转RGB565
#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

// 缓冲区管理函数
void BufferInit(void);
uint16_t *GetCurrentBuffer(void);
void SwapBuffers(void);
void ClearBuffer(uint16_t *buffer, uint16_t color);

// 基本绘图函数
void DrawPointToBuffer(uint16_t *buffer, int x, int y, uint16_t color);
void DrawLineToBuffer(uint16_t *buffer, int x1, int y1, int x2, int y2, uint16_t color);
void FillRectToBuffer(uint16_t *buffer, int x, int y, int width, int height, uint16_t color);
void DrawRectToBuffer(uint16_t *buffer, int x, int y, int width, int height, uint16_t color);
void DrawCircleToBuffer(uint16_t *buffer, int xc, int yc, int r, uint16_t color);
void FillCircleToBuffer(uint16_t *buffer, int xc, int yc, int r, uint16_t color);
// 字符绘制函数
void DrawDigitToBuffer(uint16_t *buffer, int x, int y, char digit, uint16_t color);
void DrawTextToBuffer(uint16_t *buffer, int x, int y, const char *text, uint16_t color);
void DisplayCenteredNumber(uint16_t *buffer, const char *numStr, uint16_t color);
// 缓冲区提交函数
void FlushBufferToLCD(uint16_t *buffer, int startX, int startY, int width, int height);

#endif /* __LCD_BUFFER_H */