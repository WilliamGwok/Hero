#include "drv_all.h"
#include "drv_can.h"
#include "drv_uart.h"
#include "remote.h"
#include "shooting.h"
#include "gimbal.h"
#include "chassis.h"
#include "imu.h"

void Driver_Init(void)
{
  CAN1_Init();
	
  CAN2_Init();
	
	USART2_Init();
}

void Device_Init(void)
{
	imu_init(&imu);
	
	rc_init(&rc);
	
	Shooting_Init_All();
	
	Chassis_Init_All();
	
	Gimbal_Init_All();
}

