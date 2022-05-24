//
// Created by MasterKtos on 24.05.2022.
//

#ifndef WBUDY_STM32_SNAKE_BUTTONS_H
#define WBUDY_STM32_SNAKE_BUTTONS_H

#include "stm32f1xx.h"
#include "game_engine.h"

#define NORTH_PIN GPIO_PIN_1
#define NORTH_GPIO GPIOA

#define EAST_PIN GPIO_PIN_2
#define EAST_GPIO GPIOA

#define SOUTH_PIN GPIO_PIN_0
#define SOUTH_GPIO GPIOB

#define WEST_PIN GPIO_PIN_3
#define WEST_GPIO GPIOA

#define MENU_PIN GPIO_PIN_1
#define MENU_GPIO GPIOB

#define BRIGHT_PIN GPIO_PIN_6
#define BRIGHT_GPIO GPIOA

#define DARK_PIN GPIO_PIN_4
#define DARK_GPIO GPIOA


#define CHECK_INPUT(gpio, pin) !HAL_GPIO_ReadPin(gpio, pin)

#define CHECK_INPUT_NORTH() !HAL_GPIO_ReadPin(NORTH_GPIO, NORTH_PIN)
#define CHECK_INPUT_EAST() !HAL_GPIO_ReadPin(EAST_GPIO, EAST_PIN)
#define CHECK_INPUT_SOUTH() !HAL_GPIO_ReadPin(SOUTH_GPIO, SOUTH_PIN)
#define CHECK_INPUT_WEST() !HAL_GPIO_ReadPin(WEST_GPIO, WEST_PIN)

#define CHECK_INPUT_MENU() !HAL_GPIO_ReadPin(MENU_GPIO, MENU_PIN)
#define CHECK_INPUT_BRIGHT() !HAL_GPIO_ReadPin(BRIGHT_GPIO, BRIGHT_PIN)
#define CHECK_INPUT_DARK() !HAL_GPIO_ReadPin(DARK_GPIO, DARK_PIN)

#endif //WBUDY_STM32_SNAKE_BUTTONS_H