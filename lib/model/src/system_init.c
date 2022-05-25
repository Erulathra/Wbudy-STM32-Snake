#include "system_init.h"
#include "ST7735.h"

void TIM_INIT() {
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    tim1.Instance = TIM1;
    tim1.Init.Period = 1000 - 1;
    tim1.Init.Prescaler = 640 - 1;
    tim1.Init.ClockDivision = 0;
    tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1.Init.RepetitionCounter = 0;
    tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    tim2.Instance = TIM2;
#ifdef clock128
    tim2.Init.Period = 65535;
    tim2.Init.Prescaler = 12800 - 1;
#else
    tim2.Init.Period = 65535;
    tim2.Init.Prescaler = 7200-1;
#endif
    tim2.Init.ClockDivision = 0;
    tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim2.Init.RepetitionCounter = 0;
    tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&tim2);

    tim3.Instance = TIM3;
#ifdef clock128
    tim3.Init.Period = 65535;
    tim3.Init.Prescaler = 128 - 1;
#else
    tim3.Init.Period = 65535;
    tim3.Init.Prescaler = 72-1;
#endif

    tim3.Init.ClockDivision = 0;
    tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim3.Init.RepetitionCounter = 0;
    tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&tim3);
    HAL_TIM_Base_Start(&tim3);
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&tim2);
}

void GPIO_INIT() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef gpio;
    __HAL_RCC_SPI1_CLK_ENABLE();

    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8; // SCK, MOSI, PWM
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

    //Button NORTH setup - PA1
    gpio.Pin = GPIO_PIN_1;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    //Button EAST setup - PA2
    gpio.Pin = GPIO_PIN_2;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    //Button WEST setup - PA3
    gpio.Pin = GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    //Button SOUTH setup - PB0
    gpio.Pin = GPIO_PIN_0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpio);

    //Button Temperature setup - PB1
    gpio.Pin = GPIO_PIN_1;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpio);

    //Button Brightness- setup - PA4
    gpio.Pin = GPIO_PIN_4;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    //Button Brightness+ setup - PA6
    gpio.Pin = GPIO_PIN_6;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);
}

void SystemClock_Config72(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {

    }

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

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

void SPI_INIT() {
    spi.Instance = SPI1;
    spi.Init.Mode = SPI_MODE_MASTER;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    spi.Init.Direction = SPI_DIRECTION_1LINE;
    spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi.Init.TIMode = SPI_TIMODE_DISABLE;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi.Init.CRCPolynomial = 7;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle) {

    if (spiHandle->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();

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

        HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
}

void MX_DMA_Init(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

void DMA_Init(void) {
    dma1.Instance = DMA1_Channel1;
    dma1.Init.Direction = DMA_MEMORY_TO_MEMORY;
    dma1.Init.PeriphInc = DMA_PINC_DISABLE;
    dma1.Init.MemInc = DMA_MINC_ENABLE;
    dma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dma1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    dma1.Init.Mode = DMA_NORMAL;
    dma1.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&dma1);
}

void PWM_INIT(void) {
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
}

void ADC_INIT(void) {
    __HAL_RCC_ADC1_CLK_ENABLE();

    RCC_PeriphCLKInitTypeDef adc_clk;
    adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV8;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk);

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
}