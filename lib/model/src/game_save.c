//
// Created by MasterKtos on 21.05.2022.
//

#include "game_save.h"

/** @brief Combine uint8_t variables to one uint16_t variable to make saving more efficient.
  * @details It does it in such a way that, given two variables (in binary format): \n
  * &#9  uint8_t one = xxxxxxxx(b); \n
  * &#9  uint8_t two = yyyyyyyy(b); \n
  * outputs: \n
  * &#9  uint16_t one_two = xxxxxxxxyyyyyyyy(b); \n
  * (in memory view values appear flipped, but while reading they are correct)
  */
#define combine_uint_16t(x, y) ((x << 8) | y)

/** @brief Provides mask of given size for reading from combined value.
  * @details How it works: \n
  * &#9    res = 1 << 8 (100000000(b)) \n
  * &#9    res -= 1     ( 11111111(b)) \n
  */
#define mask(x) ((1 << x) - 1)

/** @brief Save game to Flash
  * @param savedSnake Reference to initialized \b struct \b Snake variable (\&snake)
  * @param savedApple Reference to initialized \b struct \b Apple variable (\&apple)
  * @param savedTail  Initialized \b struct \b tail array (tail)
  */
void save_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail)
{
    uint32_t currentAddress = 0;
    uint8_t checksum = 0;

    enableEEPROMWriting();

    // save Snake
    writeEEPROMHalfWord(currentAddress,
                        combine_uint_16t(savedSnake->x, savedSnake->y));
    currentAddress += HALF_WORD_LENGTH;
    checksum += ones(savedSnake->x) + ones(savedSnake->y);

    writeEEPROMHalfWord(currentAddress,
                        combine_uint_16t(savedSnake->direction, savedSnake->tailLength));
    currentAddress += HALF_WORD_LENGTH;
    checksum += ones(savedSnake->direction) + ones(savedSnake->tailLength);

    // save Apple
    writeEEPROMHalfWord(currentAddress,
                        combine_uint_16t(savedApple->x, savedApple->y));
    currentAddress += HALF_WORD_LENGTH;
    checksum += ones(savedApple->x) + ones(savedApple->y);

    //save Tail
    for(uint8_t i = 0; i < TAIL_SIZE; i+=2, savedTail++) {
        uint8_t prev_val = *savedTail;
        uint8_t next_value = *(++savedTail);
        writeEEPROMHalfWord(currentAddress,
                            combine_uint_16t(prev_val, next_value));
        currentAddress += HALF_WORD_LENGTH;
        checksum += ones(prev_val) + ones(next_value);
    }

    //write checksum
    writeEEPROMHalfWord(currentAddress,checksum);

    disableEEPROMWriting();
}

/** @brief Save game to Flash
  * @details Checks if saved game is present in memory and if checksum is correct, \n
  *          then loads game
  * @param savedSnake Reference to initialized \b struct \b Snake variable (\&snake)
  * @param savedApple Reference to initialized \b struct \b Apple variable (\&apple)
  * @param savedTail  Initialized \b struct \b tail array (tail)
  */
void load_game(struct Snake *savedSnake, struct Apple *savedApple, uint8_t *savedTail)
{
    uint32_t currentAddress = 0;
    uint8_t checksum = 0;

    //read from memory
    uint16_t snake_x_y = readEEPROMHalfWord(currentAddress);
    currentAddress += HALF_WORD_LENGTH;
    uint16_t snake_dir_len = readEEPROMHalfWord(currentAddress);
    currentAddress += HALF_WORD_LENGTH;
    uint16_t apple_x_y = readEEPROMHalfWord(currentAddress);
    currentAddress += HALF_WORD_LENGTH;

    //check if save exists - if no, return
    if(snake_x_y == ERASED_HALF_WORD && snake_dir_len == ERASED_HALF_WORD && apple_x_y == ERASED_HALF_WORD)
        return;

    // calculate checksum
    checksum += ones(snake_x_y) + ones(snake_dir_len) + ones(apple_x_y);
    for(uint8_t i = 0; i < TAIL_SIZE; i+=2, savedTail++) {
        uint16_t two_segments = readEEPROMHalfWord(currentAddress);
        checksum += ones(two_segments);
        currentAddress += HALF_WORD_LENGTH;
    }

    //if checksum is incorrect, erase save and return
    if(do_checksum(checksum, readEEPROMHalfWord(currentAddress)) == 0) {
        enableEEPROMWriting();
        disableEEPROMWriting();
        return;
    }

    // read Snake
    // bitwise shift gets rid of last 8 bytes
    // need to cast to uint16_t first because of floating point "rounding"
    savedSnake->x = (uint16_t) snake_x_y >> 8;
    savedSnake->y = snake_x_y & mask(8);

    savedSnake->direction = (uint16_t) snake_dir_len >> 8;
    savedSnake->tailLength = snake_dir_len & mask(8);

    // read Apple
    savedApple->x = (uint16_t) apple_x_y >> 8;
    savedApple->y = apple_x_y & mask(8);

    // read Tail
    // move back currentAddress (previously moved for checksum)
    currentAddress -= HALF_WORD_LENGTH * (TAIL_SIZE/2 - 1);
    for(uint8_t i = 0; i < TAIL_SIZE; i+=2, savedTail++) {
        uint16_t two_segments = readEEPROMHalfWord(currentAddress);
        *savedTail = (uint16_t) (two_segments >> 8);
        *(++savedTail) = two_segments & mask(8);
        currentAddress += HALF_WORD_LENGTH;
    }
}

/** @brief Returns sum of bits with value 1 in number
  * @param number Number of which we want to calculate the sum (uint16_t)
  * @return Sum of ones
  */
uint16_t ones(uint16_t number) {
    uint16_t result = 0;
    for(;number > 0; number >>= 1) {
        result += number & 1;
    }
    return result;
}

/** @brief Does checksum by comparing numbers
  * @param calc_checksum Checksum calculated from read values
  * @param read_checksum Checksum read from memory
  * @return 1 if checksum is correct, 0 otherwise
  */
uint8_t do_checksum(uint8_t calc_checksum, uint16_t read_checksum) {
    if((uint16_t) calc_checksum == read_checksum) return 1;
    return 0;
}