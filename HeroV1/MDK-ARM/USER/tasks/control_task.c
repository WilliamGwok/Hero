#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "motor.h"
#include "drv_can.h"
#include "control_task.h"
#include "shooting_motor.h"
#include "shooting.h"
#include "chassis.h"

//float feed_motor_speed_pid_param[7]           = {0,0,0,0,0,0,3000};
//float feed_motor_angle_pid_param[7]           = {0,0,0,0,0,0,10000};
//float position_motor_speed_pid_param[7]       = {0,0,0,0,0,10000,9000};
//float friction_left_motor_speed_pid_param[7]  = {0,0,0,0,0,6000,10000};
//float friction_right_motor_speed_pid_param[7] = {0,0,0,0,0,6000,10000};

//int16_t can1_send_buff[4];
//int16_t can2_send_buff[4];

//float feed_motor_tar = 500;
//float position_motor_tar = 0;
//float friction_left_motor_tar = 0;
//float friction_right_tar = 0;

void Control_Task(void const * argument)
{
  Shooting_Init();	
//	Chassis_All_Init();
  for(;;)
  {
//    Shooting_Ctrl();

    osDelay(1);
  }
}


