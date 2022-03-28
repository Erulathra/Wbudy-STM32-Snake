#ifndef WBUDY_STM32_SNAKE_ST7735_BUFFER_H
#define WBUDY_STM32_SNAKE_ST7735_BUFFER_H

#include "ST7735.h"
#include "font.h"
#include "ST7735_buffer_settings.h"

#define NORMAL 1
#define FLIPPED -1

uint8_t bufferIndex;
uint16_t buffer[BUFFER_HEIGHT][ST7735_SCREEN_WIDTH];

void DrawImageIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data);
void DrawSpriteIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data, int8_t flipHorizontally, int8_t flipVertically);
void FillBufferWithColor(uint16_t color);
void DrawRectangleIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t color);
void DrawPixelIntroBuffer(int16_t x, int16_t y, uint16_t color);
void DrawHorizontalLineIntroBuffer(int16_t x, int16_t y, uint8_t w, uint16_t color);
void DrawVerticalLineIntroBuffer(int16_t x, int16_t y, uint8_t h, uint16_t color);

void DrawCharIntroBuffer(int16_t x, int16_t y, char ch, uint16_t textColor, FontDef font);
void DrawStringIntroBuffer(int16_t x, int16_t y, char *pt, uint16_t textColor, FontDef font);

#endif //WBUDY_STM32_SNAKE_ST7735_BUFFER_H
