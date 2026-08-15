#include "max7219.h"

extern SPI_HandleTypeDef hspi1;

void MAX7219_Send(uint8_t address, uint8_t modifier) {
	// Shift address in as MSB and modifier as LSB
	uint16_t data = (address << 8) | modifier;

	// Reset the status of the MAX7219
	HAL_GPIO_WritePin(CS_MAX7219_GPIO_Port, CS_MAX7219_Pin, GPIO_PIN_RESET);
	// Pass in 16-bit data to MAX7219 (address and modifier)
	HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, (uint8_t*)&data, 1, HAL_MAX_DELAY);
	if (status != HAL_OK) {
		while (1) {
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			HAL_Delay(200);
		}
	}
	// Set status of the MAX7219
	HAL_GPIO_WritePin(CS_MAX7219_GPIO_Port, CS_MAX7219_Pin, GPIO_PIN_SET);
}

void MAX7219_Init(void) {
	MAX7219_Send(0x0C, 0x01); // Shutdown Register; 0x01 is normal operation
	MAX7219_Send(0x0B, 0x07); // Scan Limit; Display all 8 rows
	MAX7219_Send(0x09, 0x00); // Decode Mode; 0x00 is no decode
	MAX7219_Send(0x0A, 0x0F); // Intensity; 0x0F is max brightness
	MAX7219_Send(0x0F, 0x00); // Display Test; 0x00 is normal operation

	// Clear all rows upon initialization
	for (uint8_t row = 1; row <=8; row++) {
		MAX7219_Send(row, 0x00);
	}
}
