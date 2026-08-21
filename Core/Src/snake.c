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
	// Check to see if snake is running into itself
    if (dir == DIR_UP    && direction == DIR_DOWN)  return;
    if (dir == DIR_DOWN  && direction == DIR_UP)    return;
    if (dir == DIR_LEFT  && direction == DIR_RIGHT) return;
    if (dir == DIR_RIGHT && direction == DIR_LEFT)  return;

	direction = dir;
}

uint8_t Snake_IsGameOver(void) {
	return gameOver;
}

void Snake_Update(void) {
	if (gameOver) return;

	// Decide which direction to move on the matrix
	int8_t dx = 0, dy = 0;
	switch (direction) {
	case DIR_UP: dy = -1; break;
	case DIR_DOWN: dy = 1; break;
	case DIR_LEFT: dx = -1; break;
	case DIR_RIGHT: dx = 1; break;
	}

	// Position of the new head
	// Signed integer in case of collision
	int8_t newX = body[0].x + dx;
	int8_t newY = body[0].y + dy;

	// Collision check
	if (newX < 0 || newX >= GRID_SIZE || newY < 0 || newY >= GRID_SIZE) {
		gameOver = 1;
		return;
	}

	for (uint8_t i = 0; i < length; i++) {
		if (body[i].x == newX && body[i].y == newY) {
			gameOver = 1;
			return;
		}
	}

	// Boolean to check if the new head eats the food
	uint8_t ateFood = (newX == food.x && newY == food.y);

	// Shift every part of the body ahead one block
	for (uint8_t i = length; i > 0; i--) {
		body[i] = body[i - 1];
	}

	body[0].x = newX;
	body[0].y = newY;

	// Grow the body if food was eaten
	if (ateFood) {
		if (length < MAX_LENGTH -1) {
			length++;
		}
		PlaceFood();
	}
}

void Snake_Draw(void) {
	uint8_t rowData[GRID_SIZE] = {0};

	// Draw the snake body
	for (uint8_t i = 0; i < length; i++) {
		rowData[body[i].y] |= columnBit[body[i].x];
	}

	// Draw the food bit
	rowData[food.y] |= columnBit[food.x];

	// Send data to max7219 to be drawn on matrix
	for (uint8_t row = 0; row < GRID_SIZE; row++) {
		MAX7219_Send(row + 1, rowData[row]);
	}
}
