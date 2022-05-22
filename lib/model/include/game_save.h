//
// Created by MasterKtos on 21.05.2022.
//

#ifndef WBUDY_STM32_SNAKE_GAME_SAVE_H
#define WBUDY_STM32_SNAKE_GAME_SAVE_H

#define HALF_WORD_LENGTH (uint32_t) 0x00000010

#include "game_engine.h"
#include "eeprom.h"

void save_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail);
void load_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail);

#endif //WBUDY_STM32_SNAKE_GAME_SAVE_H
