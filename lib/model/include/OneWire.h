#ifndef WBUDY_STM32_SNAKE_ONEWIRE_H
#define WBUDY_STM32_SNAKE_ONEWIRE_H

#include "stm32f1xx.h"

#define ONE_WIRE_DATA_PIN GPIO_PIN_10
#define ONE_WIRE_DATA_PORT GPIOB
#define ONE_WIRE_DATA_SET HAL_GPIO_WritePin(ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_PIN, GPIO_PIN_SET)
#define ONE_WIRE_DATA_RESET HAL_GPIO_WritePin(ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_PIN, GPIO_PIN_RESET)
#define ONE_WIRE_DATA_READ HAL_GPIO_ReadPin(ONE_WIRE_DATA_PORT, ONE_WIRE_DATA_PIN)

void WireInit();
void WireReset();
uint8_t WireRead();

void WireWrite(uint8_t byte);
uint8_t WireCrc(const uint8_t *data, int len);

#endif //WBUDY_STM32_SNAKE_ONEWIRE_H
