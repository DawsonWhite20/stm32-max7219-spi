#ifndef MAX7219_H
#define MAX7219_H

#include "main.h"

/**
 * @brief Sends a 16-bit command ton the MAX7219 over SPI
 * @param address Register address
 * @param data data bit for that register
 */

void MAX7219_Send(uint8_t address, uint8_t data);

/**
 * @brief Initializes the max7219 and clears all rows
 *
 */

void MAX7219_Init(void);

#endif
