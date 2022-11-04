#include "usart_protocal.h"
#include "vision_sensor.h"
#include "judge_sensor.h"

extern UART_HandleTypeDef huart2;

/*
	 更新检查数据
*/
void USART2_rxDataHandler(uint8_t *rxBuf)
{
 	Rc_Data(rxBuf);
}


void USART3_rxDataHandler(uint8_t *rxBuf)
{
	vision_sensor.update(&vision_sensor, rxBuf);
	vision_sensor.check(&vision_sensor);
}

void USART5_rxDataHandler(uint8_t *rxBuf)
{
	judge_sensor.update(&judge_sensor, rxBuf);
	judge_sensor.check(&judge_sensor);
}

