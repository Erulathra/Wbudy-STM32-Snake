#include <stdlib.h>
#include "math.h"
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
    snake.tailLength = STARTING_TAIL_LENGTH;

    for (;;) {
        if (FALSE && !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
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
    snake.tailLength = STARTING_TAIL_LENGTH;

    for (int8_t i = 0; i<16; i++)
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
        Snake_RemoveLastPart();
    }

    for (int j = 0; j < BUFFER_COUNT; ++j) {
        bufferIndex = j;
        FillBufferWithColor(ST7735_BLACK);

        Snake_DrawSnakeHead();
        Snake_DrawSnakeTail();

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
    // Every segment of snake is stored on 2 bits.
    // It's true position is known by moving along the tail.
    // Tail can have assigned one of 4 states, witch
    // determine in witch direction the head is.

    // Check the state of last segment of tail.
    uint8_t buf1;
    uint8_t buf2 = CheckBit(0, tail[0]);

    tail[0] = tail[0] >> 2;

    // Put in value of new tail instead of 00 from after ">> 2".
    tail[0] += (((snake.direction + 2) % 4) << 6);

    // This loop is for bringing tail segments from previous
    // 8-bit block of tails to next one.
    for(uint8_t i = 1; i <= (snake.tailLength) / 4; i++){
        // Check state of tail segment located last in this 8-bit segment.
        buf1 = CheckBit(0, tail[i]);

        // Move first 4 segments further from head.
        tail[i] = tail[i] >> 2;
        // Put in remembered value instead of 00 from after ">> 2".
        tail[i] += (buf2 << 6);
        // Get ready for another loop.
        buf2 = buf1;
    }
}

void Snake_RemoveLastPart() {
    uint8_t bit = ~(3 << (6 - ((snake.tailLength % 4)) * 2));
    tail[snake.tailLength / 4] = tail[snake.tailLength / 4] & bit;
}

void Snake_DrawSnakeHead() {
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

void Snake_DrawSnakeTail(){
    // Variables to follow position of tail.
    uint8_t pos_x = snake.x;
    uint8_t pos_y = snake.y;
    uint8_t next_tail_direction;


    // Max amount of tails can be 63.
    for(uint8_t i = 0; i < 64; i++){
        // If there is no more tails, end the loop.
        if(i >= snake.tailLength)
            return;

        // Check the direction saved on tail position.
        switch (CheckBit( 6 - (i % 4) * 2, tail[(int)floorf(i / 4)]) >> (6 - (i % 4) * 2)) {
            case NORTH:
                pos_y -= 1;
                if(i + 1 >= snake.tailLength)
                    next_tail_direction = 4;
                else
                    next_tail_direction = CheckBit( 6 - ((i+1) % 4) * 2, tail[(int)floorf((i+1) / 4)]) >> (6 - ((i+1) % 4) * 2);
                if(next_tail_direction == EAST)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, NORMAL, FLIPPED);
                else if(next_tail_direction == WEST)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, FLIPPED, FLIPPED);
                else if(next_tail_direction == 4)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          snakeTailVertical, NORMAL, NORMAL);
                else
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                      sprite_snakeVertical, NORMAL, NORMAL);


                break;
            case EAST:
                pos_x += 1;
                if(i + 1 >= snake.tailLength)
                    next_tail_direction = 4;
                else
                    next_tail_direction = CheckBit( 6 - ((i+1) % 4) * 2, tail[(int)floorf((i+1) / 4)]) >> (6 - ((i+1) % 4) * 2);
                if(next_tail_direction == NORTH)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, FLIPPED, NORMAL);
                else if(next_tail_direction == SOUTH)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, FLIPPED, FLIPPED);
                else if(next_tail_direction == 4)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          snakeTailHorizontal, FLIPPED, NORMAL);
                else
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                      sprite_snakeHorizontal, NORMAL, NORMAL);
                break;
            case SOUTH:
                pos_y += 1;
                if(i + 1 >= snake.tailLength)
                    next_tail_direction = 4;
                else
                    next_tail_direction = CheckBit( 6 - ((i+1) % 4) * 2, tail[(int)floorf((i+1) / 4)]) >> (6 - ((i+1) % 4) * 2);
                if(next_tail_direction == EAST)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, NORMAL, NORMAL);
                else if(next_tail_direction == WEST)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, FLIPPED, NORMAL);
                else if(next_tail_direction == 4)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          snakeTailVertical, NORMAL, FLIPPED);
                else
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                      sprite_snakeVertical, NORMAL, NORMAL);
                break;
            case WEST:
                pos_x -= 1;
                if(i + 1 >= snake.tailLength)
                    next_tail_direction = 4;
                else
                    next_tail_direction = CheckBit( 6 - ((i+1) % 4) * 2, tail[(int)floorf((i+1) / 4)]) >> (6 - ((i+1) % 4) * 2);
                if(next_tail_direction == NORTH)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, NORMAL, NORMAL);
                else if(next_tail_direction == SOUTH)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          sprite_snakeCorner, NORMAL, FLIPPED);
                else if(next_tail_direction == 4)
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                          snakeTailHorizontal, NORMAL, FLIPPED);
                else
                    DrawSpriteIntroBuffer(pos_x * SEGMENT_SIZE, pos_y * SEGMENT_SIZE, SEGMENT_SIZE, SEGMENT_SIZE,
                                            sprite_snakeHorizontal, NORMAL, NORMAL);
                break;
        }
    }
}

uint8_t CheckBit(uint8_t bit, uint8_t byte){
    bit = 3 << bit;
    return(bit & byte);
}

int8_t Snake_GameOver() {
    if (snake.x == BOARD_SIZE || snake.x == 255 || snake.y == BOARD_SIZE || snake.y == 255 || Snake_TailCollision() == TRUE)
        return TRUE;
    else
        return FALSE;
}

int8_t Snake_TailCollision() {
    uint8_t pos_x = snake.x;
    uint8_t pos_y = snake.y;


    for(uint8_t i = 0; i < snake.tailLength; i++) {
        switch (CheckBit(6 - (i % 4) * 2, tail[(int) floorf(i / 4)]) >> (6 - (i % 4) * 2)) {
            case NORTH:
                pos_y -= 1;
                if(pos_y == snake.y && pos_x == snake.x)
                    return TRUE;
                break;
            case EAST:
                pos_x += 1;
                if(pos_y == snake.y && pos_x == snake.x)
                    return TRUE;
                break;
            case SOUTH:
                pos_y += 1;
                if(pos_y == snake.y && pos_x == snake.x)
                    return TRUE;
                break;
            case WEST:
                pos_x -= 1;
                if(pos_y == snake.y && pos_x == snake.x)
                    return TRUE;
                break;
        }
    }
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

