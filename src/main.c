#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "ST7735.h"
#include "ST7735_buffer.h"
#include "dvd.h"
#include "images.h"
#include "DS18B20.h"
#include "ftoa.h"
#include "system_init.h"
#include "interrupts_handlers.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

SPI_HandleTypeDef spi;
DMA_HandleTypeDef dma3;
TIM_HandleTypeDef tim2;
TIM_HandleTypeDef tim3;

void GameLoop();

int main(void) {
#ifdef clock128
    SystemClock_Config128();
#else
    SystemClock_Config72();
#endif
    HAL_Init();

    MX_DMA_Init();
    GPIO_INIT();
    SPI_INIT();
    TIM_INIT();
    HAL_SPI_MspInit(&spi);

    if (HAL_SPI_Init(&spi) != HAL_OK) {
    }

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLUE);


    DS18B20_Init();
    char tempText[10];
    double testTemp = ReadTemp() * 0.0625;
    ftoa(testTemp, tempText, 4);
    HAL_TIM_Base_Start(&tim2);
    __HAL_TIM_SET_COUNTER(&tim2, 0);

    GameLoop();

}

void GameLoop() {
    char testText[] = "Litwo! Ojczyzno moja! Ty jestes jak zdrowie, ile cie trzeba cenic ten tylko sie dowie co cie stracil.";
    InitializeDVD(20, 50, 4, 3, 1);

    uint64_t frameCount = 0;
    uint8_t lastFrameDuration = 0;

    for (;;) {
        MoveDVD();

        static char stringBuffer[10];
        itoa(lastFrameDuration, stringBuffer, 10);
        for (int j = 0; j < BUFFER_COUNT; ++j) {
            bufferIndex = j;
            FillBufferWithColor(ST7735_BLACK);
            DrawImageIntroBuffer(32, 32, 64, 64, epd_bitmap_allArray[(frameCount / 6) % 7]);
            DrawDVD();

            DrawStringIntroBuffer(5, 5, stringBuffer, ST7735_WHITE, Font_7x10);
            //DrawStringIntroBuffer(5, 15, testText, ST7735_WHITE, Font_7x10);

            DrawSpriteIntroBuffer(20, 64, 16,16, sprite_snakeTail, NORMAL, NORMAL);
            DrawSpriteIntroBuffer(36, 64, 16, 16, sprite_snakeHorizontal, NORMAL, NORMAL);
            DrawSpriteIntroBuffer(52, 64, 16, 16, sprite_snakeHorizontal, NORMAL, NORMAL);
            DrawSpriteIntroBuffer(48, 64, 16, 16, sprite_snakeHorizontal, NORMAL, NORMAL);
            DrawSpriteIntroBuffer(48+16, 64, 16, 16, sprite_snake_head, NORMAL, NORMAL);
            DrawSpriteIntroBuffer(106, 64, 16, 16, sprite_apple, NORMAL, NORMAL);

            DrawSpriteIntroBuffer(100, 100, 16, 16, sprite_snake_head, FLIPPED, FLIPPED);
            ST7735_DrawBuffer(bufferIndex);
        }

        lastFrameDuration = __HAL_TIM_GET_COUNTER(&tim2) / 10;
        if (lastFrameDuration < 16) {
            HAL_Delay(16 - lastFrameDuration);
        }
        __HAL_TIM_SET_COUNTER(&tim2, 0);
        frameCount++;
    }
}
