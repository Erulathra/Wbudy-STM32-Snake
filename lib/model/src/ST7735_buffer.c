#include "ST7735_buffer.h"

void DrawImageIntroBuffer(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t * data) {
    uint16_t bufferY = bufferIndex * BUFFER_HEIGHT;

    int8_t originalWidth = w;
    int8_t originalY = y;


    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if ((x + w) <= 0 || (y + h) <= bufferY) return;

    if (originalY > 0) {
        y = y - bufferY;
    }

    if (x < 0) {
        offsetX = -x;
        w = x + w;
        x = 0;
    }
    else if (x + w >= ST7735_SCREEN_WIDTH) {
        w = ST7735_SCREEN_WIDTH - x;
    }

    if (originalY < bufferY) {
        offsetY = bufferY - originalY;
        h = originalY + h - bufferY;
        y = 0;
    }
    else if (originalY + h >= bufferY + BUFFER_HEIGHT) {
        h = (bufferY + BUFFER_HEIGHT) - originalY;

    }

    if (h > BUFFER_HEIGHT) {
        h = BUFFER_HEIGHT;
    }


    for (uint16_t i = 0; i < h; ++i) {
        for (uint16_t j = 0; j < w; ++j) {
            uint16_t test = data[(j + offsetX) + ((i + offsetY) * originalWidth)];
            buffer[i + y][j + x] = ST7735_SWAP_BYTES(test);
        }
    }

}

void FillBufferWithColor(uint16_t color) {
    for (uint16_t i = 0; i < ST7735_SCREEN_WIDTH; ++i) {
        for (uint16_t j = 0; j < BUFFER_HEIGHT; ++j) {
            buffer[j][i] = ST7735_SWAP_BYTES(color);
        }
    }
}

void FillBufferWithGradient() {
    for (uint16_t i = 0; i < ST7735_SCREEN_WIDTH; ++i) {
        for (uint16_t j = 0; j < BUFFER_HEIGHT; ++j) {
            buffer[j][i] = ST7735_SWAP_BYTES(ST7735_RGB565(2*i,0,0));

        }
    }
}
