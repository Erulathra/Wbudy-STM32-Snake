#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "ST7735.h"
#include "ST7735_buffer.h"
#include "DS18B20.h"
#include "ftoa.h"
#include "system_init.h"
#include "interrupts_handlers.h"
#include "game_engine.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

SPI_HandleTypeDef spi;
DMA_HandleTypeDef dma1;
DMA_HandleTypeDef dma3;
TIM_HandleTypeDef tim1;
TIM_HandleTypeDef tim2;
TIM_HandleTypeDef tim3;
ADC_HandleTypeDef adc;

void GameLoop();

int main(void) {
#ifdef clock128
    SystemClock_Config128();
#else
    SystemClock_Config72();
#endif
    HAL_Init();

    DMA_Init();
    MX_DMA_Init();
    GPIO_INIT();
    SPI_INIT();
    TIM_INIT();
    HAL_SPI_MspInit(&spi);
    PWM_INIT();

    if (HAL_SPI_Init(&spi) != HAL_OK) {
    }

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLUE);

    DS18B20_Init();
    double temp = ReadTemp();
    srand(temp);
    HAL_TIM_Base_Start(&tim2);
    __HAL_TIM_SET_COUNTER(&tim2, 0);

    GameEngineLoop();

}
