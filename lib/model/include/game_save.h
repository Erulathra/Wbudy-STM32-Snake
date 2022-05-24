//
// Created by MasterKtos on 21.05.2022.
//

#ifndef WBUDY_STM32_SNAKE_GAME_SAVE_H
#define WBUDY_STM32_SNAKE_GAME_SAVE_H

#define HALF_WORD_LENGTH (uint32_t) 0x00000002

#include "game_engine.h"
#include "flash.h"

void save_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail);
uint8_t load_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail);
uint8_t ones(uint8_t number);
uint8_t do_checksum(uint8_t calc_checksum, uint16_t read_checksum);

#endif //WBUDY_STM32_SNAKE_GAME_SAVE_H
