#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can_drv.h"
#include "Motor.h"
#include "motor_ctrl_task.h"

extern CAN_HandleTypeDef hcan1;

void Start_motor_ctrl_task(void const * argument)
{
  /* USER CODE BEGIN Start_motor_ctrl_task */
  /* Infinite loop */
  for(;;)
  {
		CHASSIS_CTRL();
    osDelay(2);
  }
  /* USER CODE END Start_motor_ctrl_task */
}


