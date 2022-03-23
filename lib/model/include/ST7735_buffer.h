#ifndef WBUDY_STM32_SNAKE_ST7735_BUFFER_H
#define WBUDY_STM32_SNAKE_ST7735_BUFFER_H

#include "ST7735.h"

#define BUFFER_HEIGHT 16
#define BUFFER_COUNT 8
#define ST7735_SCREEN_WIDTH 128
#define ST7735_SCREEN_HEIGHT 128
#define ST7735_SWAP_BYTES(x) (((x & 0xff) << 8) | ((x & 0xff00) >> 8))


size_t bufferIndex;
uint16_t buffer[ST7735_SCREEN_WIDTH][BUFFER_HEIGHT];

void DrawImageIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data);
void FillBufferWithColor(uint16_t color);

#endif //WBUDY_STM32_SNAKE_ST7735_BUFFER_H
