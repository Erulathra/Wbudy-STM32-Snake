//
// Created by masterktos on 12.05.22.
//

#ifndef WBUDY_STM32_SNAKE_FLASH_H
#define WBUDY_STM32_SNAKE_FLASH_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal_flash_ex.h"

//#define EEPROM_START_ADDRESS   ((uint32_t)0x0801 9000) // EEPROM emulation start address: after 100KByte of used Flash memory
//#define EEPROM_START_ADDRESS   ((uint32_t)0x0801C000) // EEPROM emulation start address: after 112KByte of used Flash memory
#define EEPROM_START_ADDRESS   ((uint32_t)0x0801D500) // EEPROM emulation start address: after 120KByte of used Flash memory

void FLASH_PageErase(uint32_t PageAddress);

// Setup functions
void enableEEPROMWriting();
void disableEEPROMWriting();

// Write functions
HAL_StatusTypeDef writeEEPROMHalfWord(uint32_t address, uint16_t data);
HAL_StatusTypeDef writeEEPROMWord(uint32_t address, uint32_t data);

// Read functions
uint16_t readEEPROMHalfWord(uint32_t address);
uint32_t readEEPROMWord(uint32_t address);

#endif //WBUDY_STM32_SNAKE_FLASH_H
