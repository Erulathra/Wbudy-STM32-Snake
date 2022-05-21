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

    if (HAL_SPI_Init(&spi) != HAL_OK) {
    }

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLUE);

    __HAL_RCC_TIM1_CLK_ENABLE();

    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_8;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);

    tim1.Instance = TIM1;
    tim1.Init.Period = 1000 - 1;
    tim1.Init.Prescaler = 640 - 1;
    tim1.Init.ClockDivision = 0;
    tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1.Init.RepetitionCounter = 0;
    tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&tim1); // ustawienie że timer będzie działał w PWN

    TIM_OC_InitTypeDef oc;
    oc.OCMode = TIM_OCMODE_PWM1;
    oc.Pulse = 1000;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCNPolarity = TIM_OCNPOLARITY_LOW;
    oc.OCFastMode = TIM_OCFAST_ENABLE;
    oc.OCIdleState = TIM_OCIDLESTATE_SET;
    oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&tim1, &oc, TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_1);

    __HAL_RCC_ADC1_CLK_ENABLE();

    //ADC_HandleTypeDef adc;
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

//    while(1){
//        for(uint16_t i = 100; i <= 1000; i+=100){
//            HAL_ADC_Start(&adc);
//            HAL_ADC_PollForConversion(&adc, 10);
//
//            uint32_t value = HAL_ADC_GetValue(&adc);
//
//            HAL_Delay(250);
//
//            uint16_t val_2 = abs(value - 2400) * 2;
//
//            __HAL_TIM_SET_COMPARE(&tim1, TIM_CHANNEL_1, val_2);
//        }
//    }


    DS18B20_Init();
    char tempText[10];
    double temp = ReadTemp();
    srand(temp);
    HAL_TIM_Base_Start(&tim2);
    __HAL_TIM_SET_COUNTER(&tim2, 0);

    GameEngineLoop();

}
