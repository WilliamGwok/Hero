#include "led_drv.h"
#include "main.h"
#include "cmsis_os.h"

/**
 *	@brief	�������������ˮ��
 */
void Float_LED(void)
{
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_BLUE_Pin, GPIO_PIN_RESET);
	osDelay(500);
	HAL_GPIO_WritePin(GPIOC, LED_BLUE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_ORANGE_Pin, GPIO_PIN_RESET);
	osDelay(500);
	HAL_GPIO_WritePin(GPIOC, LED_ORANGE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
	osDelay(500);
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_RESET);
	osDelay(500);
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
}

/**
 *	@brief	���ʧ�������Ƴ���
 */
void Alarm_LED(void)
{
	HAL_GPIO_WritePin(GPIOC, LED_BLUE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
}


