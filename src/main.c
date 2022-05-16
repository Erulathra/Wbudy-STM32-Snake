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

    if (HAL_SPI_Init(&spi) != HAL_OK) {
    }

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLUE);

    __HAL_RCC_ADC1_CLK_ENABLE();

    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_2;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Mode = GPIO_MODE_AF_INPUT;
    gpio.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &gpio);

    RCC_PeriphCLKInitTypeDef adc_clk;
    adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV2;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk);

    ADC_HandleTypeDef adc;
    adc.Instance = ADC1;
    adc.Init.ContinuousConvMode = ENABLE;
    adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    adc.Init.NbrOfConversion = 1;
    adc.Init.DiscontinuousConvMode = DISABLE;
    adc.Init.NbrOfDiscConversion = 1;
    HAL_ADC_Init(&adc);

    HAL_ADCEx_Calibration_Start(&adc);

    ADC_ChannelConfTypeDef adc_ch;
    adc_ch.Channel = ADC_CHANNEL_VREFINT;
    adc_ch.Rank = ADC_REGULAR_RANK_1;
    adc_ch.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
    HAL_ADC_ConfigChannel(&adc, &adc_ch);

    HAL_ADC_Start(&adc);
    while (1)
    {
        uint32_t value = HAL_ADC_GetValue(&adc);
        printf("Adc = %ld (%.3fV)\r\n", value, value * 3.3f / 4096.0f);
    }

    DS18B20_Init();
    char tempText[10];
    double temp = ReadTemp();
    srand(temp);
    HAL_TIM_Base_Start(&tim2);
    __HAL_TIM_SET_COUNTER(&tim2, 0);

    GameEngineLoop();

}
