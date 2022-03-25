#ifndef WBUDY_STM32_SNAKE_DVD_H
#define WBUDY_STM32_SNAKE_DVD_H

#include "sprites.h"
#include "stm32f1xx.h"
#include "ST7735_buffer.h"

#define LOGO_SIZE 32
const extern uint16_t * LOGO;

struct DVD {
    uint8_t x;
    uint8_t y;
    int8_t speedX;
    int8_t speedY;
    uint8_t speedFactor;
} dvd;

void InitializeDVD(int16_t startX, int16_t startY, int8_t speedX, int8_t speedY, uint8_t speedFactor);
void MoveDVD();
void DrawDVD();

#endif //WBUDY_STM32_SNAKE_DVD_H
