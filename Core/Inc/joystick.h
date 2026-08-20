#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "main.h"
#include "snake.h"

Direction Joystick_ReadDirection(void);
uint8_t Joystick_ButtonPressed(void);

#endif
