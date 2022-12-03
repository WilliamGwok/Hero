#include "car.h"
#include "stdbool.h"
#include "config_status.h"
#include "chassis.h"

bool top_car_on = false;
bool top_car_off = false;
bool top_switch = false;

bool gim_left_turn_90 = false;
bool gim_right_turn_90 = false;
bool gim_head_turn = false;

bool shoot_start = false;
bool shoot_stop = false;
bool shoot_standby = false;
bool shoot_wake = false;
bool feed_reload = false;
bool feed_unload = false;

bool car_mode_change = false;
bool car_init_ok = false;

extern bool gimbal_init_ok;
extern bool imu_init_Y_O_N;

extern Chassis_Mode_t Chassis_Mode;

car_t Car;

void car_init(car_t* car)
{
	car->car_move_command = sleep_car;
	
	car->car_move_status = sleep_car;
	
	car->ctrl_mode = RC_CTRL;
	
	car_command_init();
}

void car_command_init(void)
{
	top_car_on = false;
  top_car_off = false;
	top_switch = false;
	shoot_start = false;
  shoot_stop = false;
	shoot_standby = false;
  shoot_wake = false;
  feed_reload = false;
  feed_unload = false;
	gim_left_turn_90 = false;
  gim_right_turn_90 = false;
	gim_head_turn = false;
}

void car_ctrl(car_t* car)
{
	car_status_update(car);
	
	car_ctrl_mode_update(car);
}

void car_ctrl_mode_update(car_t* car)
{
	Rc_S2_Status_Check(car);
	
	switch(car->ctrl_mode)
	{
		case KEY_CTRL:
			Key_Status_Scan(car);
			break;
		case RC_CTRL:
			Rc_Status_Scan(car);
			break;
		default:
			break;
	}
}

void car_init_judge(car_t* car)
{
	if(gimbal_init_ok == true && imu_init_Y_O_N == true)
	{
		car_init_ok = true;
	}
}

void car_status_update(car_t* car)
{
	car_init_judge(car);
	
	if(rc.info->status == DEV_OFFLINE)
	{
		car->car_move_status = sleep_car;
		car_mode_change = true;
		gimbal_init_ok = false;
		car_init_ok = false;
		car_command_init();
	}
	else if(car_init_ok == false)
	{
	  car->car_move_status = init_car;
		car_mode_change = true;
		car_command_init();
	}
	else
	{
		if(car->car_move_status != car->car_move_command)
		{
			car->car_move_status = car->car_move_command;
			
			car_mode_change = true;
		}
		else
		{
			car_mode_change = false;
		}
	}
}

void Rc_S2_Status_Check(car_t* car)
{
	switch(rc.base_info->s2.status)
	{
		case up_R:
			car->ctrl_mode = KEY_CTRL;
//		  car->ctrl_mode =RC_CTRL;
		  car->car_move_command = imu_car;
			break;
		case mid_R:
			car->ctrl_mode =RC_CTRL;
		  car->car_move_command = mec_car;
			break;
		case down_R:
			car->ctrl_mode =RC_CTRL;
		  car->car_move_command = imu_car;
			break;
		default:
			break;
	}
}

void Rc_Status_Scan(car_t* car)
{
	Rc_S1_Status_Check();
	
	Rc_Thumbwheel_Check(car);
}

void Rc_S1_Status_Check(void)
{
	switch(rc.base_info->s1.status)
	{
		case up_R:
      shoot_start = true;
			break;
		case mid_R:
      shoot_standby = true;
			break;
		case down_R:
      shoot_stop = true;
			break;
		default:
			
			break;
	}
}

void Rc_Thumbwheel_Check(car_t* car)
{
	switch(rc.base_info->thumbwheel.status)
	{
		case up_R:
			switch(car->car_move_command)
			{
				case mec_car:
					break;
				case imu_car:
					top_car_on = true;
				  break;
			}
			break;
		case mid_R:
			break;
		case down_R:
			switch(car->car_move_command)
			{
				case mec_car:
					break;
				case imu_car:
					top_car_off = true;
				  break;
			}
			break;
			default:
			break;
	}
}




void Key_Mouse_l_Status_Check(car_t *car)
{
  switch(rc.base_info->mouse_btn_l.status)
	{
		case relax_K:
			shoot_standby = true;
			break;
		case down_K:
			shoot_start = true;
			break;
		case up_K:
			shoot_standby = true;
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}	

void Key_Mouse_r_Status_Check(car_t *car)
{
  switch(rc.base_info->mouse_btn_r.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Q_Status_Check(car_t *car)
{
  switch(rc.base_info->Q.status)
	{
		case relax_K:
			break;
		case down_K:
			gim_left_turn_90 = true;
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_W_Status_Check(car_t *car)
{
  switch(rc.base_info->W.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_E_Status_Check(car_t *car)
{
  switch(rc.base_info->E.status)
	{
		case relax_K:
			break;
		case down_K:
			gim_right_turn_90 = true;
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_S_Status_Check(car_t *car)
{
  switch(rc.base_info->S.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_D_Status_Check(car_t *car)
{
  switch(rc.base_info->D.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_F_Status_Check(car_t *car)
{
  switch(rc.base_info->F.status)
	{
		case relax_K:
			break;
		case down_K:
			top_switch = true;
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Z_Status_Check(car_t *car)
{
  switch(rc.base_info->Z.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_C_Status_Check(car_t *car)
{
  switch(rc.base_info->C.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_V_Status_Check(car_t *car)
{
  switch(rc.base_info->V.status)
	{
		case relax_K:
			break;
		case down_K:
			if(car->car_move_status == imu_car)
			{
				gim_head_turn = true;
			}
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_B_Status_Check(car_t *car)
{
  switch(rc.base_info->B.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Shift_Status_Check(car_t *car)
{
  switch(rc.base_info->Shift.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Ctrl_Status_Check(car_t *car)
{
  switch(rc.base_info->Ctrl.status)
	{
		case relax_K:
			break;
		case down_K:
			
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Status_Scan(car_t* car)
{
	Key_Mouse_l_Status_Check(car);
	Key_Mouse_r_Status_Check(car);
	Key_Q_Status_Check(car);
  Key_W_Status_Check(car);
  Key_E_Status_Check(car);
  Key_S_Status_Check(car);
  Key_D_Status_Check(car);
  Key_F_Status_Check(car);
  Key_Z_Status_Check(car);
  Key_C_Status_Check(car);
  Key_V_Status_Check(car);
  Key_B_Status_Check(car);
  Key_Shift_Status_Check(car);
  Key_Ctrl_Status_Check(car);
}

