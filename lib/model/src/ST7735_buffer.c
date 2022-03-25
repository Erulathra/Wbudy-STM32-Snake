#include "ST7735_buffer.h"

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


void DrawSpriteIntroBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint16_t *data) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;

    int8_t originalWidth = w;

    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if ((x + w) <= 0 || (y + h) <= bufferY) return;

    CalculateVerticalWindow(bufferY, &y, &h, &offsetY);
    CalculateHorizontalWindow(&x, &w, &offsetX);


    for (uint16_t i = 0; i < h; ++i) {
        for (uint16_t j = 0; j < w; ++j) {
            uint16_t color = data[(j + offsetX) + ((i + offsetY) * originalWidth)];

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

    for (uint16_t i = 0; i < h; ++i) {
        for (uint16_t j = 0; j < w; ++j) {
            buffer[i + y][j + x] = ST7735_SWAP_BYTES(color);
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

void DrawHorizontalLineIntroBuffer(int16_t x, int16_t y, uint8_t w, uint16_t color) {
    DrawRectangleIntroBuffer(x, y, w, 1, color);
}

void DrawVerticalLineIntroBuffer(int16_t x, int16_t y, uint8_t h, uint16_t color) {
    DrawRectangleIntroBuffer(x, y, 1, h, color);
}

void DrawCharIntroBuffer(int16_t x, int16_t y, char character, int16_t textColor, uint8_t size) {
    uint8_t line;
    int16_t i, j;

    for (i = 0; i < 6; i++) {
        if (i == 5)
            line = 0x0;
        else
            line = Font[(character * 5) + i];

        for (j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (size == 1)
                    DrawPixelIntroBuffer(x + i, y + j, textColor);
                else {
                    DrawRectangleIntroBuffer(x + (i * size), y + (j * size), size, size, textColor);
                }
            }
            line >>= 1;
        }
    }
}

void DrawPixelIntroBuffer(int16_t x, int16_t y, uint16_t color) {
    uint8_t bufferY = bufferIndex * BUFFER_HEIGHT;

    if (x > ST7735_SCREEN_WIDTH - 1 || y > (bufferY + BUFFER_HEIGHT - 1)) return;
    if (x < 0 || y < bufferY) return;

    y -= bufferY;

    buffer[y][x] = ST7735_SWAP_BYTES(color);
}

void GoToNextLine(int16_t x, uint8_t size, char **pt, int16_t *characterX, int16_t *characterY);

void DrawStringIntroBuffer(int16_t x, int16_t y, char *pt, int16_t textColor, uint8_t size) {

    int16_t characterX = x;
    int16_t characterY = y;

    while (*pt) {
        DrawCharIntroBuffer(characterX, characterY, *pt, textColor, size);

        pt++;
        characterX += (6 * size);

        if (*pt == '\n') {
            pt++;
            GoToNextLine(x, size, &pt, &characterX, &characterY);
        }

        if (characterX + 6 >= ST7735_SCREEN_WIDTH) {
            GoToNextLine(x, size, &pt, &characterX, &characterY);
        }

        if (characterY >= ST7735_SCREEN_HEIGHT) {
            return;
        }
    }

}

//Transfer text to next line - It resets x and add font size to y, if first character in line is space, it will skip that character.
void GoToNextLine(int16_t x, uint8_t size, char **pt, int16_t *characterX, int16_t *characterY) {
    (*characterX) = x;
    (*characterY) += (10 * size);
    if (*(*pt) == ' ') {
        (*pt)++;
    }
}
