#include "control_task.h"


//底盘左前电机又隐患，会掉线2022/11/26
//8000转底盘打滑严重

void control_task(void const * argument)
{
	static int16_t cnt = 0;
	
	car_init(&Car);
	
  for(;;)
  {
		rc_ctrl(&rc);
		
		imu_update(&imu);
		
		Gimbal.imu_update(&Gimbal);
		
		car_ctrl(&Car);
		
		Cal_Pulse();
		
		#ifndef VISION_OFF
		if(cnt > 3)
		{
			vision_send(0,Gimbal.info->yaw_angle_imu_measure,Gimbal.info->pitch_angle_imu_measure,0);
		}
		else
		{
			cnt++;
		}
		#endif

		
		if(rc.info->status == DEV_ONLINE)
		{
			Shooting_Test();
			
      Gimbal_Ctrl(&Gimbal);
			
			Chassis_Ctrl(&Chassis_Mode);
			
			CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
		  CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
			CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
	    CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
		}
		else
		{
      Car_Sleep();
		}
		
    osDelay(1);
  }
}

void realtime_task(void const * argument)
{
  for(;;)
  {
		vision_tick_task(&vision);
		
    osDelay(1);
  }
}
