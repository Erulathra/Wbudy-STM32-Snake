#include <stdlib.h>
#include "game_engine.h"
#include "ST7735_buffer.h"
#include "sprites.h"
#include "images.h"
#include "dvd.h"

extern TIM_HandleTypeDef tim2;


_Noreturn void GameEngineLoop() {
    InitializeDVD(10, 10, 3, 2, 1);

    uint64_t frameCount = 0;
    uint8_t lastFrameDuration = 0;

    snake.x = 2;
    snake.y = 3;

    for (;;) {
        MoveDVD();
        if (!(frameCount % 60)) {
            //Snake_MoveSnake();
        }
        Snake_ChangeDirection(NORTH);

        static char stringBuffer[10];
        itoa(lastFrameDuration, stringBuffer, 10);
        for (int j = 0; j < BUFFER_COUNT; ++j) {
            bufferIndex = j;
            FillBufferWithColor(ST7735_BLACK);
            DrawImageIntroBuffer(32, 32, 64, 64, epd_bitmap_allArray[(frameCount / 6) % 7]);
            //Snake_DrawSnake();


            DrawSpriteIntroBuffer(100, 100, 16, 16, sprite_snake_head_horizontal, FLIPPED, NORMAL);

            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                  sprite_snake_head_vertical, NORMAL, FLIPPED);
//            DrawSpriteIntroBuffer((snake.x + 1) * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
//                                  sprite_snake_head_horizontal, NORMAL, NORMAL);
//            DrawSpriteIntroBuffer((snake.x + 2) * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
//                                  sprite_snake_head_vertical, NORMAL, NORMAL);
//            DrawSpriteIntroBuffer((snake.x + 3) * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
//                                  sprite_snake_head_horizontal, NORMAL, FLIPPED);

            ST7735_DrawBuffer(bufferIndex);
        }

        lastFrameDuration = __HAL_TIM_GET_COUNTER(&tim2) / 10;
        if (lastFrameDuration < 16) {
            HAL_Delay(16 - lastFrameDuration);
        }
        __HAL_TIM_SET_COUNTER(&tim2, 0);
        frameCount++;
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

void Snake_ChangeDirection(uint8_t newDirection) {
    snake.direction = newDirection;
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


