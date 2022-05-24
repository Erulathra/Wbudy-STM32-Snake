//
// Created by masterktos on 12.05.22.
//

#include "stm32f1xx.h"
#include "flash.h"

/** @brief  Enables flash write
  * @attention Call this before doing write operations
  * @details Enables flash write via HAL, erases page (sets every bit to 1)
  * and clears PER bit (PER bit, when set, states that flash is in Page Erase mode)
  */
void enableEEPROMWriting() {
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();
    // required to re-write
    FLASH_PageErase(EEPROM_START_ADDRESS);
    // Bug fix: bit PER has been set in Flash_PageErase(), must clear it here
    // (PER bit, when set, states that flash is in Page Erase mode)
    CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
}

/** @attention Call this when you finish writing
  * @brief Disables, via HAL, ability to write
  */
void disableEEPROMWriting() {
    HAL_FLASH_Lock();
}

/** @brief Write data of max size equivalent to uint16_t size
  * @attention Call enableEEPROMWriting() first, after write call disableERPROMWriting()
  *
  * @param address Points where to write specified data
  *                (address=0 points to EEPROM_START_ADDRESS)
  * @param data Data to save
  */
HAL_StatusTypeDef writeEEPROMHalfWord(uint32_t address, uint16_t data) {
    HAL_StatusTypeDef status;
    address = address + EEPROM_START_ADDRESS;

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);

    return status;
}
/** @brief Write data of max size equivalent to uint32_t size
  * @attention Call enableEEPROMWriting() first, after write call disableERPROMWriting()
  *
  * @param address Points where to write specified data
  *                (address=0 points to EEPROM_START_ADDRESS)
  * @param data Data to save
  */
HAL_StatusTypeDef writeEEPROMWord(uint32_t address, uint32_t data) {
    HAL_StatusTypeDef status;
    address = address + EEPROM_START_ADDRESS;

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);

    return status;
}

/** @brief Read data of max size equivalent to uint16_t size
 *
  * @param address Address to write specified data
  *                (address=0 points to EEPROM_START_ADDRESS)
  * @param data Data to save
  */
uint16_t readEEPROMHalfWord(uint32_t address) {
    uint16_t val = 0;
    address = address + EEPROM_START_ADDRESS;
    val = *(__IO uint16_t*)address;

    return val;
}

/** @brief Read data of max size equivalent to uint32_t size
 *
  * @param address Address to write specified data
  *                (address=0 points to EEPROM_START_ADDRESS)
  * @param data Data to save
  */
uint32_t readEEPROMWord(uint32_t address) {
    uint32_t val = 0;
    address = address + EEPROM_START_ADDRESS;
    val = *(__IO uint32_t*)address;

    return val;
}