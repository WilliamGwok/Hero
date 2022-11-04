#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can_drv.h"
#include "Motor.h"
#include "shooting_task.h"


void Start_shooting_task(void const * argument)
{
  /* USER CODE BEGIN Start_shooting_task */
  /* Infinite loop */
  for(;;)
  {
		Shooting_Ctrl();
    osDelay(1);
  }
  /* USER CODE END Start_shooting_task */
}









