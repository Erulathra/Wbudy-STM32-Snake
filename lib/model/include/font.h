#ifndef WBUDY_STM32_SNAKE_FONT_H
#define WBUDY_STM32_SNAKE_FONT_H

#include <stdint.h>

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;


#endif //WBUDY_STM32_SNAKE_FONT_H
