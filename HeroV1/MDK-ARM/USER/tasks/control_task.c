#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "motor.h"
#include "drv_can.h"
#include "control_task.h"
#include "shooting_motor.h"
#include "shooting.h"
#include "chassis.h"
#include "rp_chassis.h"
#include "remote.h"
#include "config_status.h"
extern chassis_t Chassis;
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

//void control_task(void const * argument)
//{
//  Shooting_Init();	
////	Chassis_All_Init();
//  for(;;)
//  {
////    Shooting_Ctrl();

//    osDelay(1);
//  }
//}

void control_task(void const * argument)
{
	Shooting_Init();
	Chassis_Init_All();
  for(;;)
  {
		if(rc.info->status == DEV_ONLINE)
		{
			Chassis_Ctrl_All();
		  Chassis.work(&Chassis);

		  Chassis.base_info.target.front_speed = rc.base_info->ch3 * 2;
		  Chassis.base_info.target.right_speed = rc.base_info->ch2 * 2;
		  Chassis.base_info.target.cycle_speed = rc.base_info->ch0 * 2;
		  Shooting_Ctrl();
		}
		else
		{
			Chassis.work(&Chassis);
//¸Ä
		  Chassis.base_info.target.front_speed = 0;
		  Chassis.base_info.target.right_speed = 0;
		  Chassis.base_info.target.cycle_speed = 0;
		}
		
	
    osDelay(1);
  }

}


