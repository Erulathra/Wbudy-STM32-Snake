#ifndef WBUDY_STM32_SNAKE_ST7735_BUFFER_H
#define WBUDY_STM32_SNAKE_ST7735_BUFFER_H

#include "ST7735.h"

#define BUFFER_HEIGHT 16
#define BUFFER_COUNT 8
#define ST7735_SCREEN_WIDTH 128
#define ST7735_SCREEN_HEIGHT 128

#define TRANSPARENCY_COLOR 0xf81f

uint8_t bufferIndex;
uint16_t buffer[BUFFER_HEIGHT][ST7735_SCREEN_WIDTH];

void DrawImageIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data);
void DrawSpriteIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data);
void FillBufferWithColor(uint16_t color);
void DrawRectangleIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t color);
void DrawPixelIntroBuffer(int16_t x, int16_t y, uint16_t color);
void DrawHorizontalLineIntroBuffer(int16_t x, int16_t y, uint8_t w, uint16_t color);
void DrawVerticalLineIntroBuffer(int16_t x, int16_t y, uint8_t h, uint16_t color);

void DrawCharIntroBuffer(int16_t x, int16_t y, char character, int16_t textColor, uint8_t size);
void DrawStringIntroBuffer(int16_t x, int16_t y, char *pt, int16_t textColor, uint8_t size);

#endif //WBUDY_STM32_SNAKE_ST7735_BUFFER_H
