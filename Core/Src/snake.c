#include "snake.h"
#include "max7219.h"
#include <stdlib.h>

#define GRID_SIZE 8
#define MAX_LENGTH 64 // Maximum length of the snake

typedef struct {
	uint8_t x; // x-coordinate (columns)
	uint8_t y; // y-coordinate (rows)
} Point; // Used to point to both body of snake and food positioning

static Point body[MAX_LENGTH];
static uint8_t length;
static Direction direction;
static Point food;
static uint8_t gameOver;

// Mapping of columns
static const uint8_t columnBit[GRID_SIZE] = {
		0b01000000, // SEG A
		0b00100000, // SEG B
		0b00010000, // SEG C
		0b00001000, // SEG D
		0b00000100, // SEG E
		0b00000010, // SEG F
		0b00000001, // SEG G
		0b10000000 // SEG DP
};

static void PlaceFood(void) { // Only to be called in this file
	food.x = rand() % GRID_SIZE; // Modulo calculation used to make sure x and y coordinates stay inside 0-7
	food.y = rand() % GRID_SIZE;
}

void Snake_Init(void) {
	length = 1;
	body[0].x = 3;
	body[0].y = 3;
	direction = DIR_RIGHT;
	gameOver = 0;

	PlaceFood();
}

void Snake_SetDirection(Direction dir) {
	direction = dir;
}

uint8_t Snake_IsGameOver(void) {
	return gameOver;
}
