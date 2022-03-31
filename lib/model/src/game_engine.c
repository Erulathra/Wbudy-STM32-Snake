#include "game_engine.h"
#include "ST7735_buffer.h"
#include "sprites.h"


void MoveSnake() {
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

void ChangeDirection(uint8_t newDirection) {
    snake.direction = newDirection;
}

void DrawSnake() {
    switch (snake.direction) {
        case NORTH:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE,SEGMENT_SIZE, sprite_snake_head, NORMAL, NORMAL);
            break;
        case EAST:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE,SEGMENT_SIZE, sprite_snake_head, NORMAL, NORMAL);
            break;
        case SOUTH:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE,SEGMENT_SIZE, sprite_snake_head, NORMAL, NORMAL);
            break;
        case WEST:
            DrawSpriteIntroBuffer(snake.x * SEGMENT_SIZE, snake.y * SEGMENT_SIZE, SEGMENT_SIZE,SEGMENT_SIZE, sprite_snake_head, NORMAL, NORMAL);
            break;
    }
}


