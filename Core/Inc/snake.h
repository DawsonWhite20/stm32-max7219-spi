#ifndef SNAKE_H
#define SNAKE_H

#include "main.h"

typedef enum {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} Direction;

void Snake_Init(void);

void Snake_Update(void);

void Snake_Draw(void);

void Snake_SetDirection(Direction dir);

uint8_t Snake_IsGameOver(void);

#endif
