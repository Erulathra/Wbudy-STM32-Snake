#ifndef WBUDY_STM32_SNAKE_GAME_ENGINE_H
#define WBUDY_STM32_SNAKE_GAME_ENGINE_H

#include "stm32f1xx.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define TRUE 1
#define FALSE 0

#define BOARD_SIZE 8
#define SEGMENT_SIZE 16

struct Snake {
    uint8_t x;
    uint8_t y;
    uint8_t direction;
} snake;

void Snake_MoveSnake();
//TODO: to cza zmienic
void Snake_ChangeDirection(uint8_t newDirection);
void Snake_DrawSnake();
void Snake_GameOver();

_Noreturn void GameEngineLoop();

#endif //WBUDY_STM32_SNAKE_GAME_ENGINE_H