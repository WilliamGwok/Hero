#include "shooting_motor.h"
#include "config_pid.h"
#include "3508_motor.h"
#include "2006_motor.h"

extern drv_can_t feed_motor_can_driver;
extern drv_can_t position_motor_can_driver;
extern drv_can_t friction_left_motor_can_driver;
extern drv_can_t friction_right_motor_can_driver;

motor_3508_base_info_t feed_motor_base_info;
motor_3508_info_t feed_motor_info;
motor_2006_base_info_t position_motor_base_info;
motor_2006_info_t position_motor_info;
motor_3508_base_info_t friction_left_motor_base_info;
motor_3508_info_t friction_left_motor_info;
motor_3508_base_info_t friction_right_motor_base_info;
motor_3508_info_t friction_right_motor_info;

motor_3508_t feed_motor =
{
	.base_info = &feed_motor_base_info,
	.info      = &feed_motor_info,
	.pid_speed = &feed_motor_pid_speed,
	.pid_angle = &feed_motor_pid_angle,
	.can       = &feed_motor_can_driver,
	.init      = motor_3508_init,
	.update    = motor_3508_update,
};

motor_2006_t position_motor =
{
	.base_info = &position_motor_base_info,
	.info      = &position_motor_info,
	.pid_speed = &position_motor_pid_speed,
	.can       = &position_motor_can_driver,
	.init      = motor_2006_init,
	.update    = motor_2006_update,
};

motor_3508_t friction_left_motor =
{
	.base_info = &friction_left_motor_base_info,
	.info      = &friction_left_motor_info,
	.pid_speed = &friction_left_motor_pid_speed,
	.can       = &friction_left_motor_can_driver,
	.init      = motor_3508_init,
	.update    = motor_3508_update,
};

motor_3508_t friction_right_motor =
{
	.base_info = &friction_right_motor_base_info,
	.info      = &friction_right_motor_info,
	.pid_speed = &friction_right_motor_pid_speed,
	.can       = &friction_right_motor_can_driver,
	.init      = motor_3508_init,
	.update    = motor_3508_update,
};



