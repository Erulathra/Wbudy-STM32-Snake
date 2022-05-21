//
// Created by MasterKtos on 21.05.2022.
//

#ifndef WBUDY_STM32_SNAKE_GAME_SAVE_H
#define WBUDY_STM32_SNAKE_GAME_SAVE_H

#include "game_engine.h"
#include "eeprom.h"

void save_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail[16]);
void load_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail[16]);

#endif //WBUDY_STM32_SNAKE_GAME_SAVE_H
