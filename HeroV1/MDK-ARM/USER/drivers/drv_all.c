#include "drv_all.h"
#include "drv_can.h"
#include "drv_uart.h"


void Driver_Init(void)
{
  CAN1_Init();
  CAN2_Init();
//	USART2_Init();
}



