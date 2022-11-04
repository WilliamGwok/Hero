#include "Imu_task.h"
#include "cmsis_os.h"
#include "main.h"


void Start_imu_task(void const * argument)
{
  /* USER CODE BEGIN Start_imu_task */
	imu_sensor.init(&imu_sensor);
  /* Infinite loop */
  for(;;)
  {
    imu_sensor.update(&imu_sensor);
    osDelay(1);
  }
  /* USER CODE END Start_imu_task */
}
