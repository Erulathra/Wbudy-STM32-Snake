//
// Created by MasterKtos on 21.05.2022.
//

#include "game_save.h"

//  Combine uint8_t variables to one uint16_t variable
// to make saving more efficient.
//  It does it in such a way that, given two variables (in binary format):
//      uint8_t one = xxxxxxxx(b);
//      uint8_t two = yyyyyyyy(b);
// outputs:
//      uint16_t one_two = xxxxxxxxyyyyyy(b);
// (in memory view values appear flipped, but while reading they are correct)
#define combine_uint_16t(x, y) ((x << 8) | y)
// Provides mask of given size for reading from combined value.
// How it works:
//      res = 1 >> 8 (100000000(b))
//      res - 1 = 11111111(b)
#define mask(x) ((1 << x) - 1)

void save_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail)
{
    //Snake = 4 words
    //Apple = 3 words
    //tail = 16 words
    uint32_t currentAddress = 0;

    enableEEPROMWriting();

    // save Snake
    writeEEPROMHalfWord(currentAddress,
                        combine_uint_16t(savedSnake->x, savedSnake->y));
    currentAddress += HALF_WORD_LENGTH;

    writeEEPROMHalfWord(currentAddress,
                        combine_uint_16t(savedSnake->direction, savedSnake->tailLength));
    currentAddress += HALF_WORD_LENGTH;

    // save Apple
    writeEEPROMHalfWord(currentAddress,
                        combine_uint_16t(savedApple->x, savedApple->y));
    currentAddress += HALF_WORD_LENGTH;

    //save Tail
    for(uint8_t i = 0; i < SEGMENT_SIZE; i+=2, savedTail++) {
        uint8_t prev_val = *savedTail;
        uint8_t next_value = *(++savedTail);
        writeEEPROMHalfWord(currentAddress,
                            combine_uint_16t(prev_val, next_value));
        currentAddress += HALF_WORD_LENGTH;
    }

    // TODO:write checksum

    disableEEPROMWriting();
}

void load_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail)
{
    uint32_t currentAddress = 0;

    //TODO:check checksum

    // read Snake
    uint16_t snake_x_y = readEEPROMHalfWord(currentAddress);
    // bitwise shift gets rid of last 8 bytes
    // need to cast to uint16_t first because of floating point rounding
    savedSnake->x = (uint16_t) snake_x_y >> 8;
    savedSnake->y = snake_x_y & mask(8);
    currentAddress += HALF_WORD_LENGTH;

    uint16_t snake_dir_len = readEEPROMHalfWord(currentAddress);
    savedSnake->direction = (uint16_t) snake_dir_len >> 8;
    savedSnake->tailLength = snake_dir_len & mask(8);
    currentAddress += HALF_WORD_LENGTH;

    // read Apple
    uint16_t apple_x_y = readEEPROMHalfWord(currentAddress);
    savedApple->x = (uint16_t) apple_x_y >> 8;
    savedApple->y = apple_x_y & mask(8);
    currentAddress += HALF_WORD_LENGTH;

    // read Tail
    for(uint8_t i = 0; i < SEGMENT_SIZE; i+=2, savedTail++) {
        uint16_t two_segments = readEEPROMHalfWord(currentAddress);
        //TODO: check in online compiler
        *savedTail = (uint16_t) (two_segments >> 8);
        *(++savedTail) = two_segments & mask(8);
        currentAddress += HALF_WORD_LENGTH;
    }
}