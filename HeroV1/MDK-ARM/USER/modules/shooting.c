#include "shooting.h"
#include "motor.h"
#include "shooting_motor.h"
#include "config_shooting.h"
#include "mathematics.h"
#include "config_can.h"

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/

float feed_motor_position_in_pid_param[7]       = {0,0,0,0,0,0,3000};

float feed_motor_position_pid_param[7]          = {0,0,0,0,0,0,10000};

float position_motor_speed_pid_param[7]         = {0,0,0,0,0,10000,9000};

float friction_left_motor_speed_pid_param[7]    = {27,3,0,0,0,6000,10000};

float friction_right_motor_speed_pid_param[7]   = {27,3,0,0,0,6000,10000};

Shoot_config_t shoot_config =
{
	.feed_reload_angle = feed_reload_work_angle,
	.fric_speed_init = friction_work_speed_init,
	.fric_10ms_speed = friction_10ms_work_speed,
	.fric_16ms_speed = friction_16ms_work_speed,
	.position_work_speed = position_work_speed_init,
	.feed_reload_times = feed_work_reload_time,
};

Feed_work_info_t work_info_feed;
shoot_work_info_t work_info_shoot = 
{
  .feed_work_info = &work_info_feed,
};

Shoot_info_t Shoot_Info = 
{
	.shoot_work_info = &work_info_shoot,
};

Shoot_t Shoot = 
{
	.config = &shoot_config,
	.info = &Shoot_Info,
	
	.feed_ctrl = Feed_Ctrl,
	.position_ctrl = Position_Ctrl,
	.l_fric_ctrl = Left_Friction_Ctrl,
	.r_fric_ctrl = Right_Friction_Ctrl,
};

void Shooting_Init_All(void)
{
	feed_motor.init(&feed_motor);
	position_motor.init(&position_motor);
	friction_left_motor.init(&friction_left_motor);
	friction_right_motor.init(&friction_right_motor);
	
	feed_motor.pid_init(&feed_motor.pid.position_in,feed_motor_position_in_pid_param);
	feed_motor.pid_init(&feed_motor.pid.position,feed_motor_position_pid_param);
	friction_left_motor.pid_init(&friction_left_motor.pid.speed,friction_left_motor_speed_pid_param);
	friction_right_motor.pid_init(&friction_right_motor.pid.speed,friction_right_motor_speed_pid_param);
	
	Shoot.feed_shoot = &feed_motor;
	Shoot.position_shoot = &position_motor;
	Shoot.left_friction_shoot = &friction_left_motor;
	Shoot.right_friction_shoot = &friction_right_motor;
	
	Shoot_Work_Init(&Shoot);
}

void Shooting_Heartbeat(void)
{
	feed_motor.heartbeat(&feed_motor);
	position_motor.heartbeat(&position_motor);
	friction_left_motor.heartbeat(&friction_left_motor);
	friction_right_motor.heartbeat(&friction_right_motor);
}

void Shoot_Commmand_React(Shoot_t* shoot)
{
	shoot_work_info_t* work_info = shoot->info->shoot_work_info;
	
	if(shoot_start == true)
	{
		work_info->shoot_work_command = S_Shoot;
	}
	if(shoot_stop == true)
	{
		work_info->shoot_work_command = S_Stop;
	}
	if(shoot_standby == true)
	{
		work_info->shoot_work_command = S_Standby;
	}
	if(shoot_wake == true)
	{
		work_info->shoot_work_command = S_Wake;
	}
	if(feed_reload == true)
	{
		work_info->feed_work_info->feed_work_command = F_reload;
	}
	if(feed_unload == true)
	{
		work_info->feed_work_info->feed_work_command = F_unload;
	}
}


void Shoot_Command_Init(Shoot_t* shoot)
{
  shoot_start = false;
  shoot_stop = false;
  shoot_standby = false;
  shoot_wake = false;
  feed_reload = false;
  feed_unload = false;
}

void Shoot_Work_Init(Shoot_t* shoot)
{
	Feed_Work_Init(shoot);
	
	shoot->info->shoot_work_info->shoot_work_status = S_Stop;
	shoot->info->shoot_work_info->shoot_work_command = S_Standby;
	
	shoot->info->shoot_work_info->standby_yes = 0;
}

void Feed_Work_Init(Shoot_t* shoot)
{
	shoot->info->shoot_work_info->feed_work_info->feed_work_status = F_static;
	shoot->info->shoot_work_info->feed_work_info->feed_work_command = F_static;
}

















void Shoot_Work(Shoot_t*shoot)
{
	
	switch(shoot->info->shoot_work_info->shoot_work_status)
	{
		case S_Stop:
			shoot->info->fric_speed_target = 0;
		  shoot->info->position_speed_target = 0;
		  shoot->info->shoot_work_info->standby_yes = 0;
		  shoot->info->shoot_work_info->shoot_work_status = S_Done;
		  shoot->info->shoot_work_info->feed_work_info->feed_work_status = F_static;
			break;
		case S_Shoot:
			if(shoot->info->shoot_work_info->shoot_work_command == S_Stop)
			{
				shoot->info->shoot_work_info->shoot_work_status = S_Stop;
			}
			break;
		case S_Wake:
			shoot->info->fric_speed_target = shoot->config->fric_speed_init;
			shoot->info->shoot_work_info->feed_work_info->feed_work_command = F_reload;
			if(abs(shoot->left_friction_shoot->pid.speed.info.measure) >= shoot->info->fric_speed_target - resonable_err && \
				 abs(shoot->right_friction_shoot->pid.speed.info.measure) >= shoot->info->fric_speed_target - resonable_err)
			{
				shoot->info->shoot_work_info->standby_yes = 1;
				shoot->info->shoot_work_info->shoot_work_status = S_Done;
			}
		  if(shoot->info->shoot_work_info->shoot_work_command == S_Stop)
			{
				shoot->info->shoot_work_info->shoot_work_status = S_Stop;
			}
			break;
		case S_Standby:
			switch(shoot->info->shoot_work_info->shoot_work_command)
			{
				case S_Stop:
					shoot->info->shoot_work_info->shoot_work_status = S_Stop;
				  break;
				case S_Wake:
					shoot->info->shoot_work_info->shoot_work_status = S_Wake;
				  break;
				case S_Shoot:
					if(shoot->info->shoot_work_info->standby_yes == 1)
					{
						shoot->info->shoot_work_info->shoot_work_status = S_Shoot;
					}
					else
					{
						shoot->info->shoot_work_info->shoot_work_status = S_Wake;
					}
				  break;
				default:
					break;
			}
			break;
		case S_Done:
			
			switch(shoot->info->shoot_work_info->shoot_work_command)
			{
				case S_Standby:
					shoot->info->shoot_work_info->shoot_work_status = S_Standby;
				  break;
				case S_Stop:
					shoot->info->shoot_work_info->shoot_work_status = S_Stop;
					break;
				default:
					break;
			}
		default:
			break;
	}
	
	shoot->l_fric_ctrl(shoot);
	shoot->r_fric_ctrl(shoot);
	
}

void Feed_Work(Shoot_t* shoot)
{
	uint8_t status = 0,command = 0;
	uint16_t angle_target = 0,angle_measure = 0;
	
	status = shoot->info->shoot_work_info->feed_work_info->feed_work_status;
	command = shoot->info->shoot_work_info->feed_work_info->feed_work_command;
	
	angle_target = shoot->info->feed_angle_target;
	angle_measure = shoot->feed_shoot->rx_info.angle_sum;
	
	switch(status)
	{
		case F_reload:
			if(angle_measure >= angle_target - 500)
			{
				if(shoot->info->shoot_work_info->feed_work_info->feed_work_times < shoot->config->feed_reload_times)
				{
					angle_target += shoot->config->feed_reload_angle;
				  shoot->info->shoot_work_info->feed_work_info->feed_work_times++;
				}
				else
				{
					angle_target = angle_measure;
					Feed_Stay_Static(shoot);
				}
			}
			break;
		case F_unload:
			break;
		case F_static:
			angle_target = angle_measure;
		  switch(command)
			{
				case F_reload:
					status = F_reload;
				  command = F_static;
					break;
				case F_unload:
          break;				
        default:
          break;					
			}
			break;
		default:
			break;
	}
	
	shoot->info->feed_angle_target = angle_target;
	shoot->info->shoot_work_info->feed_work_info->feed_work_status = status;
	shoot->info->shoot_work_info->feed_work_info->feed_work_command = command;
	
	shoot->feed_ctrl(shoot);
}

void Feed_Stay_Static(Shoot_t* shoot)
{
	shoot->info->shoot_work_info->feed_work_info->feed_work_status = F_static;
	shoot->info->shoot_work_info->feed_work_info->feed_work_times = 0;
	
}

void Shooting_Test(void)
{
//	Shoot.info->fric_speed_target = friction_work_speed_init;
//	Shoot.info->feed_angle_target = feed_reload_work_angle;
//	
//	if(Shoot.feed_shoot->pid.position.info.err <= 1)
//	{
//		Shoot.feed_shoot->rx_info.angle_sum = 0;
//	}
//	Shoot.l_fric_ctrl(&Shoot);
//	Shoot.r_fric_ctrl(&Shoot);
//	Shoot.feed_ctrl(&Shoot);
	Shoot_Commmand_React(&Shoot);
	
	Shoot_Work(&Shoot);
	Feed_Work(&Shoot);
	
//	Shooting_Send();
	
	Shoot_Command_Init(&Shoot);
	
}


void Shooting_Ctrl(Shoot_t* shoot)
{
	
	Shoot_Commmand_React(shoot);
	
	
	
	Shoot_Command_Init(shoot);
}

















//void Shooting_Send(motor_t* motor,int16_t *buff)
//{
//  if(motor->id.drive_type == M_CAN1)
//	{
//		CAN1_Send_With_int16_to_uint8(motor->id.tx_id,buff);
//	}
//	if(motor->id.drive_type == M_CAN2)
//	{
//		CAN2_Send_With_int16_to_uint8(motor->id.tx_id,buff);
//	}
//}

void Shooting_Send(void)
{
		CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
	
		CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
	
	  CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
}

void Feed_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->feed_shoot;
	
	can1_0x1FF_send_buff[motor->id.buff_p] = motor->c_posit(motor,shoot->info->feed_angle_target);//不能这样写
	
//  Shooting_Send(motor,can1_shoot_send_buff);
}

void Position_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->position_shoot;
	
	can2_0x200_send_buff[motor->id.buff_p] = motor->c_speed(motor,shoot->info->position_speed_target);
	
//	Shooting_Send(motor,can2_shoot_send_buff);
}

void Right_Friction_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->right_friction_shoot;
	
	can2_0x1FF_send_buff[motor->id.buff_p] = motor->c_speed(motor,shoot->info->fric_speed_target);
	
//	Shooting_Send(motor,can2_shoot_send_buff);
}

void Left_Friction_Ctrl(Shoot_t* shoot)
{
	motor_t* motor = shoot->left_friction_shoot;
	
	can2_0x1FF_send_buff[motor->id.buff_p] = motor->c_speed(motor,(-shoot->info->fric_speed_target));
	
//	Shooting_Send(motor,can2_shoot_send_buff);
}



