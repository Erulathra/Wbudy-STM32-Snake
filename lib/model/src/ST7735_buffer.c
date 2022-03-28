#include <malloc.h>
#include "ST7735_buffer.h"

extern DMA_HandleTypeDef dma1;

void CalculateVerticalWindow(uint16_t bufferY, int16_t *y, uint8_t *h, uint8_t *offsetY);

void CalculateHorizontalWindow(int16_t *x, uint8_t *w, uint8_t *offsetX);

void DrawImageIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if ((x + w) <= 0 || (y + h) <= bufferY) return;

    int8_t originalWidth = w;

    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    CalculateHorizontalWindow(&x, &w, &offsetX);
    CalculateVerticalWindow(bufferY, &y, &h, &offsetY);

    for (uint16_t i = 0; i < h; ++i) {
        for (uint16_t j = 0; j < w; ++j) {
            uint16_t color = data[(j + offsetX) + ((i + offsetY) * originalWidth)];
            buffer[i + y][j + x] = ST7735_SWAP_BYTES(color);
        }
    }

}

void CalculateHorizontalWindow(int16_t *x, uint8_t *w, uint8_t *offsetX) {
    if ((*x) < 0) {
        (*offsetX) = -(*x);
        (*w) = (*x) + (*w);
        (*x) = 0;
    }
    else if ((*x) + (*w) >= ST7735_SCREEN_WIDTH) {
        (*w) = ST7735_SCREEN_WIDTH - (*x);
    }
}

void CalculateVerticalWindow(uint16_t bufferY, int16_t *y, uint8_t *h, uint8_t *offsetY) {
    int16_t originalY = *y;

    if (originalY > 0) {
        (*y) = (*y) - bufferY;
    }

    if (originalY < bufferY) {
        (*offsetY) = bufferY - originalY;
        (*h) = originalY + (*h) - bufferY;
        (*y) = 0;
    }
    else if (originalY + (*h) >= bufferY + BUFFER_HEIGHT) {
        (*h) = (bufferY + BUFFER_HEIGHT) - originalY;

    }

    if ((*h) > BUFFER_HEIGHT) {
        (*h) = BUFFER_HEIGHT;
    }
}


void DrawSpriteIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data, int8_t flipHorizontally, int8_t flipVertically) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;

    int8_t originalWidth = w;
    int8_t originalHeight = h;

    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if ((x + w) <= 0 || (y + h) <= bufferY) return;

    CalculateVerticalWindow(bufferY, &y, &h, &offsetY);
    CalculateHorizontalWindow(&x, &w, &offsetX);

    uint8_t heightFlipFactor = 0;
    uint8_t widthFlipFactor = 0;

    if (flipVertically == FLIPPED) {
        heightFlipFactor = 1;
    }
    if (flipHorizontally == FLIPPED) {
        widthFlipFactor = 1;
    }

    for (uint16_t i = 0; i < h; ++i) {
        for (uint16_t j = 0; j < w; ++j) {
            uint8_t horizontalDataIndex = widthFlipFactor * (originalWidth - 1) + (j + offsetX) * flipVertically;
            uint8_t verticalDataIndex = heightFlipFactor * (originalHeight - 1) + (i + offsetY) * flipVertically;
            uint16_t color = data[horizontalDataIndex + (verticalDataIndex * originalWidth)];

            if (color == TRANSPARENCY_COLOR) {
                continue;
            }

            buffer[i + y][j + x] = ST7735_SWAP_BYTES(color);
        }
    }

}

void DrawRectangleIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint16_t color) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;
    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if ((x + w) <= 0 || (y + h) <= bufferY) return;

    CalculateVerticalWindow(bufferY, &y, &h, &offsetY);
    CalculateHorizontalWindow(&x, &w, &offsetX);

    color = ST7735_SWAP_BYTES(color);

    dma1.Init.PeriphInc = DMA_PINC_DISABLE;
    HAL_DMA_Init(&dma1);

    for (uint16_t i = 0; i < h; ++i) {
        HAL_DMA_Start(&dma1, (uint32_t)&color, ((uint32_t)buffer) + (x + (i + y) * ST7735_SCREEN_WIDTH) * 2,w);
        HAL_DMA_PollForTransfer(&dma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
    }


}

void FillBufferWithColor(uint16_t color) {
    dma1.Init.PeriphInc = DMA_PINC_DISABLE;
    HAL_DMA_Init(&dma1);

    color = ST7735_SWAP_BYTES(color);
    HAL_DMA_Start(&dma1, (uint32_t)&color, (uint32_t)buffer,ST7735_SCREEN_WIDTH * BUFFER_HEIGHT);
    HAL_DMA_PollForTransfer(&dma1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

}

void DrawHorizontalLineIntroBuffer(int16_t x, int16_t y, uint8_t w, uint16_t color) {
    DrawRectangleIntroBuffer(x, y, w, 1, color);
}

void DrawVerticalLineIntroBuffer(int16_t x, int16_t y, uint8_t h, uint16_t color) {
    DrawRectangleIntroBuffer(x, y, 1, h, color);
}

void DrawCharIntroBuffer(int16_t x, int16_t y, char ch, uint16_t textColor, FontDef font) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;

    if (x > ST7735_SCREEN_WIDTH || y > (bufferY + BUFFER_HEIGHT)) return;
    if ((x + font.width) <= 0 || (y + font.height) <= bufferY) return;

    uint16_t characterBuffer[16 * 26];

    for (uint16_t i = 0; i < font.height; i++) {
        uint16_t line = font.data[(ch - 32) * font.height + i];
        for (uint16_t j = 0; j < font.width; j++) {
            if ((line << j) & 0x8000) {
                characterBuffer[j + i * font.width] = textColor;
            }
            else {
                characterBuffer[j + i * font.width] = TRANSPARENCY_COLOR;
            }
        }
    }

    DrawSpriteIntroBuffer(x, y, font.width, font.height, (const uint16_t *) characterBuffer, NORMAL, NORMAL);
}


void DrawPixelIntroBuffer(int16_t x, int16_t y, uint16_t color) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if (x < 0 || y < bufferY) return;

    y -= bufferY;

    buffer[y][x] = ST7735_SWAP_BYTES(color);
}

void GoToNextLine(int16_t x, FontDef font, char **pt, int16_t *characterX, int16_t *characterY);

void DrawStringIntroBuffer(int16_t x, int16_t y, char *pt, uint16_t textColor, FontDef font) {

    int16_t characterX = x;
    int16_t characterY = y;

    while (*pt) {
        DrawCharIntroBuffer(characterX, characterY, *pt, textColor, font);

        pt++;
        characterX += (int8_t) font.width;

        if (*pt == '\n') {
            pt++;
            GoToNextLine(x, font, &pt, &characterX, &characterY);
        }

        if (characterX + font.width >= ST7735_SCREEN_WIDTH) {
            GoToNextLine(x, font, &pt, &characterX, &characterY);
        }

        if (characterY >= ST7735_SCREEN_HEIGHT) {
            return;
        }
    }

}

//Transfer text to next line - It resets x and add font size to y, if first character in line is space, it will skip that character.
void GoToNextLine(int16_t x, FontDef font, char **pt, int16_t *characterX, int16_t *characterY) {
    (*characterX) = x;
    (*characterY) += font.height;
    if (*(*pt) == ' ') {
        (*pt)++;
    }
}
