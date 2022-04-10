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
#define STARTING_DIRECTION_SNAKE 0

#define GAME_SPEED 30

struct Snake {
    uint8_t x;
    uint8_t y;
    uint8_t direction;
    uint8_t tailLength;
} snake;

uint8_t tail[16];


_Noreturn void GameEngineLoop();

void Mode_Temperature();

void Mode_GameOver();

void Mode_Snake(uint64_t);

void Snake_MoveSnake();

void Snake_MoveTail();

void Snake_DrawSnake();

int8_t Snake_GameOver();

int8_t CheckInput(int8_t);



#endif //WBUDY_STM32_SNAKE_GAME_ENGINE_H