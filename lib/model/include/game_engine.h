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

#define STARTING_POINT_SNAKE_X 0
#define STARTING_POINT_SNAKE_Y 7

struct Snake {
    uint8_t x;
    uint8_t y;
    uint8_t direction;
    uint8_t tailLength;
} snake;

uint8_t tail[16];


void Snake_MoveSnake();

//TODO: to cza zmienic
void Snake_ChangeDirection(uint8_t newDirection);

void Snake_DrawSnake();

int8_t Snake_GameOver();

void Snake_GameOverScreen();

void Snake_MoveTail();

_Noreturn void GameEngineLoop();

#endif //WBUDY_STM32_SNAKE_GAME_ENGINE_H