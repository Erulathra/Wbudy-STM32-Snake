#include "interrupts_handlers.h"

extern SPI_HandleTypeDef spi;
extern DMA_HandleTypeDef dma3;
extern TIM_HandleTypeDef tim2;
extern TIM_HandleTypeDef tim3;

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

void SPI1_IRQHandler(void) {
    HAL_SPI_IRQHandler(&spi);
}
