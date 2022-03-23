#include "ST7735_buffer.h"

void DrawImageIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t * data) {
    uint16_t bufferY = bufferIndex * BUFFER_HEIGHT;

    uint8_t originalWidth = w;
    uint8_t originalHeight = h;

    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    if (x > ST7735_SCREEN_WIDTH || y > bufferY + BUFFER_HEIGHT) return;
    if (x + w < 0 || y + h < bufferY) return;

    if (x < 0) {
        offsetX = -x;
        w = x + w - ST7735_SCREEN_WIDTH;
        x = 0;
    }
    else if (x + w > ST7735_SCREEN_WIDTH) {
        w = ST7735_SCREEN_WIDTH - x;
    }

    if (y < bufferY) {
        offsetY = bufferY - y;
        h = y + h - bufferY;
        x = 0;
    }
    else if (y + h > bufferY + BUFFER_HEIGHT) {
        h = bufferY + BUFFER_HEIGHT - y;
    }


    for (uint16_t i = 0; i < w; ++i) {
        for (uint16_t j = 0; j < h; ++j) {
            buffer[j + x][i + y] = data[j + offsetX + (ST7735_SCREEN_HEIGHT * (i + offsetY))];
        }
    }

}

void FillBufferWithColor(uint16_t color) {
    for (uint16_t i = 0; i < ST7735_SCREEN_WIDTH; ++i) {
        for (uint16_t j = 0; j < BUFFER_HEIGHT; ++j) {
            //buffer[i][j] = ST7735_SWAP_BYTES(color);
        }
    }
}
