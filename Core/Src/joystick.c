#include <stdlib.h>
#include "joystick.h"

extern ADC_HandleTypeDef hadc1;

static uint32_t ADC_Read(uint32_t channel) {
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint32_t value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	return value;
}

Direction Joystick_ReadDirection(void) {
	uint32_t x = ADC_Read(ADC_CHANNEL_0);
	uint32_t y = ADC_Read(ADC_CHANNEL_1);

    const int32_t center = 2048;
    const int32_t deadzone = 600;

    int32_t dx = (int32_t)x - center;
    int32_t dy = (int32_t)y - center;

    // Ignore small movements near center
    if (abs(dx) < deadzone && abs(dy) < deadzone) {
        return DIR_NONE;
    }

    // Whichever axis moved further from center is the configured direction
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? DIR_RIGHT : DIR_LEFT;
    } else {
        return (dy > 0) ? DIR_DOWN : DIR_UP;
    }
}
