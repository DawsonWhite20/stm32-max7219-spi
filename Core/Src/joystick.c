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

	if (x < 1500) return DIR_LEFT;
	if (x > 2500) return DIR_RIGHT;
	if (y < 1500) return DIR_UP;
	if (y > 2500) return DIR_DOWN;

	return DIR_NONE;
}

uint8_t Joystick_ButtonPressed(void) {
	return (HAL_GPIO_ReadPin(SW_JOYSTICK_GPIO_Port, SW_JOYSTICK_Pin) == GPIO_PIN_RESET);
}
