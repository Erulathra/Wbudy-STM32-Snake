#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "ST7735.h"
#include "ST7735_buffer.h"
#include "dvd.h"
#include "images.h"
#include "OneWire.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#define clock128

SPI_HandleTypeDef spi;
DMA_HandleTypeDef dma3;
TIM_HandleTypeDef tim2;
TIM_HandleTypeDef tim3;

void SystemClock_Config72(void);
void SystemClock_Config128(void);
void MX_DMA_Init(void);
void GPIO_INIT();
void SPI_INIT();
void TIM_INIT();

void Error_Handler();

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
    HAL_SYSTICK_Config(SystemCoreClock / 1000);

    if (HAL_SPI_Init(&spi) != HAL_OK) {
    }

    ST7735_Init();
    char testText[] = "Litwo! Ojczyzno moja! Ty jestes jak zdrowie, ile cie trzeba cenic ten tylko sie dowie co cie stracil.";
    ST7735_FillScreen(ST7735_BLUE);
    InitializeDVD(20, 50, 4, 3, 1);

    uint64_t frameCount = 0;
    uint8_t lastFrameDuration = 0;

    /*WireInit();
    WireReset();
    WireWrite(0xcc);
    WireWrite(0xbe);*/

    /*int i;
    uint8_t scratchpad[9];
    for (i = 0; i < 9; i++)
        scratchpad[i] = WireRead();

    int16_t temp = (int16_t) (scratchpad[0] << 8 | scratchpad[1]);*/

    HAL_TIM_Base_Start(&tim2);
    for (;;) {
        MoveDVD();

        for (int j = 0; j < BUFFER_COUNT; ++j) {
            bufferIndex = j;
            FillBufferWithColor(ST7735_BLACK);
            DrawImageIntroBuffer(32, 32, 64, 64, epd_bitmap_allArray[(frameCount/6) % 7]);
            DrawDVD();

            static char stringBuffer[10];
            itoa(lastFrameDuration, stringBuffer, 10);
            DrawStringIntroBuffer(5, 5, stringBuffer, ST7735_WHITE, Font_7x10);
            //itoa(temp, stringBuffer, 10);
            DrawStringIntroBuffer(20, 5, stringBuffer, ST7735_WHITE, Font_7x10);

            DrawStringIntroBuffer(5, 14, testText, ST7735_WHITE, Font_7x10);
            ST7735_DrawBuffer(bufferIndex, buffer);
        }

        lastFrameDuration = __HAL_TIM_GET_COUNTER(&tim2) / 10;
        if (lastFrameDuration < 16) {
            HAL_Delay(16 - lastFrameDuration);
        }
        __HAL_TIM_SET_COUNTER(&tim2, 0);
        frameCount++;
    }

}

void TIM_INIT() {
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    tim2.Instance = TIM2;
#ifdef clock128
    tim2.Init.Period = 9999;
    tim2.Init.Prescaler = 12799;
#else
    tim2.Init.Period = 9;
    tim2.Init.Prescaler = 7199;
#endif
    tim2.Init.ClockDivision = 0;
    tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim2.Init.RepetitionCounter = 0;
    tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&tim2);

    tim3.Instance = TIM3;
    tim3.Init.Period = 999;
    tim3.Init.Prescaler = 127;
    tim3.Init.ClockDivision = 0;
    tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim3.Init.RepetitionCounter = 0;
    tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&tim3);
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&tim2);
}

void SPI_INIT() {
    spi.Instance = SPI1;
    spi.Init.Mode = SPI_MODE_MASTER;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi.Init.TIMode = SPI_TIMODE_DISABLE;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi.Init.CRCPolynomial = 7;
}

void GPIO_INIT() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef gpio;
    __HAL_RCC_SPI1_CLK_ENABLE();

    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7; // SCK, MOSI
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = ST7735_CS; // CS
    HAL_GPIO_Init(ST7735_CS_PORT, &gpio);

    gpio.Pin = ST7735_RESET; // CS
    HAL_GPIO_Init(ST7735_RESET_PORT, &gpio);

    gpio.Pin = ST7735_DC; // CS
    HAL_GPIO_Init(ST7735_DC_PORT, &gpio);

    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &gpio);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

}

void SysTick_Handler(void) {
    HAL_IncTick();
}

void Error_Handler() {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void HardFault_Handler(void) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void DMA1_Channel3_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dma3);
}

#pragma clang diagnostic pop


void SystemClock_Config72(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {

    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {

    }
}

void SystemClock_Config128(void) {

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}



void MX_DMA_Init(void) {

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle) {

    if (spiHandle->Instance == SPI1) {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /* SPI1 DMA Init */
        /* SPI1_TX Init */

        dma3.Instance = DMA1_Channel3;
        dma3.Init.Direction = DMA_MEMORY_TO_PERIPH;
        dma3.Init.PeriphInc = DMA_PINC_DISABLE;
        dma3.Init.MemInc = DMA_MINC_ENABLE;
        dma3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dma3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        dma3.Init.Mode = DMA_NORMAL;
        dma3.Init.Priority = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&dma3) != HAL_OK) {
        }

        __HAL_LINKDMA(spiHandle, hdmatx, dma3);
    }
}

