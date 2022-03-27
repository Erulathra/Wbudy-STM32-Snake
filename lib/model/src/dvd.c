#include "dvd.h"

const uint16_t *LOGO = sprite_logo;
uint16_t RandomColor();

static uint8_t bounceCount;
static uint8_t frameCount;

void InitializeDVD(int16_t startX, int16_t startY, int8_t speedX, int8_t speedY, uint8_t speedFactor) {
    dvd.x = startX;
    dvd.y = startY;
    dvd.speedX = speedX;
    dvd.speedY = speedY;
    dvd.speedFactor = speedFactor;
}

void MoveDVD() {
    frameCount++;
    if (frameCount < dvd.speedFactor)
        return;

    if ((dvd.x + LOGO_SIZE) >= ST7735_SCREEN_WIDTH) {
        dvd.speedX *= -1;
        bounceCount++;
    }

    if ((dvd.y + LOGO_SIZE) >= ST7735_SCREEN_WIDTH) {
        dvd.speedY *= -1;
        bounceCount++;
    }

    dvd.x += dvd.speedX;
    dvd.y += dvd.speedY;
    frameCount = 0;
}

void DrawDVD() {
    DrawRectangleIntroBuffer(dvd.x, dvd.y, LOGO_SIZE, LOGO_SIZE, RandomColor());
    DrawSpriteIntroBuffer(dvd.x, dvd.y, LOGO_SIZE, LOGO_SIZE, LOGO, NORMAL, NORMAL);
}


uint16_t RandomColor() {

    bounceCount %= 7;

    switch (bounceCount) {
        case 0:
            return ST7735_RED;
        case 1:
            return ST7735_GREEN;
        case 2:
            return ST7735_ORANGE;
        case 3:
            return ST7735_BLUE;
        case 4:
            return ST7735_MAGENTA;
        case 5:
            return ST7735_YELLOW;
        default:
            return ST7735_CYAN;
    }
}
