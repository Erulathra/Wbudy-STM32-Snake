#ifndef WBUDY_STM32_SNAKE_INTERRUPTS_HANDLERS_H
#define WBUDY_STM32_SNAKE_INTERRUPTS_HANDLERS_H

#include "stm32f1xx.h"
#include "interrupts_handlers.h"

void SysTick_Handler(void);
void Error_Handler();
void HardFault_Handler(void);
void DMA1_Channel3_IRQHandler(void);
void SPI1_IRQHandler(void);

#endif //WBUDY_STM32_SNAKE_INTERRUPTS_HANDLERS_H
