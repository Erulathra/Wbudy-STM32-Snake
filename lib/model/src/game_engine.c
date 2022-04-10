#include <stdlib.h>
#include "game_engine.h"
#include "ST7735_buffer.h"
#include "sprites.h"
#include "images.h"
#include "DS18B20.h"
#include "ftoa.h"

extern TIM_HandleTypeDef tim2;


_Noreturn void GameEngineLoop() {
    uint64_t frameCount = 0;
    uint8_t lastFrameDuration = 0;

    snake.x = STARTING_POINT_SNAKE_X;
    snake.y = STARTING_POINT_SNAKE_Y;
    snake.direction = STARTING_DIRECTION_SNAKE;
    snake.tailLength = 1;

    for (;;) {
        if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
            Mode_Temperature();
        }
        else if (Snake_GameOver() == TRUE) {
            Mode_GameOver();
        }
        else {
            Mode_Snake(frameCount);
        }

        lastFrameDuration = __HAL_TIM_GET_COUNTER(&tim2) / 10;
        if (lastFrameDuration < 16) {
            HAL_Delay(16 - lastFrameDuration);
        }
        __HAL_TIM_SET_COUNTER(&tim2, 0);
        frameCount++;
    }
}

void Mode_Temperature() {
    uint64_t frameCount = 0;
    uint8_t lastFrameDuration = 0;
    DS18B20_Init();
    char tempText[10];

    for (;;) {
        double testTemp = ReadTemp() * 0.0625;
        ftoa(testTemp, tempText, 4);

        for (int j = 0; j < BUFFER_COUNT; ++j) {
            bufferIndex = j;
            FillBufferWithColor(ST7735_BLACK);

            DrawStringIntroBuffer(5, 15, tempText, ST7735_WHITE, Font_7x10);

            ST7735_DrawBuffer(bufferIndex);
        }

        lastFrameDuration = __HAL_TIM_GET_COUNTER(&tim2) / 10;
        if (lastFrameDuration < 16) {
            HAL_Delay(16 - lastFrameDuration);
        }
        __HAL_TIM_SET_COUNTER(&tim2, 0);
        frameCount++;

        //TODO: Zacznij znowu po wciśnięciu przycisku
        if (frameCount == 60)
            return;
    }
}

void Mode_GameOver() {
    uint64_t frameCount = 0;
    uint8_t lastFrameDuration = 0;

    snake.x = STARTING_POINT_SNAKE_X;
    snake.y = STARTING_POINT_SNAKE_Y;
    snake.direction = STARTING_DIRECTION_SNAKE;
    snake.tailLength = 1;

    for (int8_t i; i<16; i++)
        tail[i] = 0;

    for (;;) {
        for (int j = 0; j < BUFFER_COUNT; ++j) {
            bufferIndex = j;
            FillBufferWithColor(ST7735_BLACK);
            DrawImageIntroBuffer(32, 32, 64, 64, epd_bitmap_allArray[(frameCount / 6) % 7]);

            ST7735_DrawBuffer(bufferIndex);
        }

        lastFrameDuration = __HAL_TIM_GET_COUNTER(&tim2) / 10;
        if (lastFrameDuration < 16) {
            HAL_Delay(16 - lastFrameDuration);
        }
        __HAL_TIM_SET_COUNTER(&tim2, 0);
        frameCount++;

        //TODO: Zacznij znowu po wciśnięciu przycisku
        if (frameCount == 60)
            return;
    }
}

void Mode_Snake(uint64_t frameCount) {
    snake.direction = CheckInput(snake.direction);

    if (!(frameCount % GAME_SPEED)) {
        Snake_MoveSnake();
        Snake_MoveTail();
    }

    for (int j = 0; j < BUFFER_COUNT; ++j) {
        bufferIndex = j;
        FillBufferWithColor(ST7735_BLACK);

        Snake_DrawSnake();

        ST7735_DrawBuffer(bufferIndex);
    }
}

void Snake_MoveSnake() {
    switch (snake.direction) {
        case NORTH:
            snake.y--;
            break;
        case EAST:
            snake.x++;
            break;
        case SOUTH:
            snake.y++;
            break;
        case WEST:
            snake.x--;
            break;
    }
}

void Snake_MoveTail() {
    //tail[0] << 1;
    return;
}

void Snake_DrawSnake() {
    switch (snake.direction) {
        case NORTH:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                  sprite_snake_head_vertical, NORMAL, FLIPPED);
            break;
        case EAST:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                  sprite_snake_head_horizontal, NORMAL, NORMAL);
            break;
        case SOUTH:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                  sprite_snake_head_vertical, NORMAL, NORMAL);
            break;
        case WEST:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                  sprite_snake_head_horizontal, FLIPPED, NORMAL);
            break;
    }
}

int8_t Snake_GameOver() {
    if (snake.x == BOARD_SIZE || snake.x == 255 || snake.y == BOARD_SIZE || snake.y == 255)
        return TRUE;
    else
        return FALSE;
}

int8_t CheckInput(int8_t def) {
    if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) {
        return NORTH;
    }
    else if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) {
        return EAST;
    }
    else if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) {
        return WEST;
    }
    else if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
        return SOUTH;
    }
    else {
        return def;
    }
}

