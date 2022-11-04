#include "drv_all.h"
#include "drv_can.h"



void Driver_Init(void)
{
  CAN1_Init();
  CAN2_Init();
}



