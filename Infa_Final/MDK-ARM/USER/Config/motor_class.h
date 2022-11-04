#ifndef __MOTOR_CLASS_H
#define __MOTOR_CLASS_H

#include "stm32f4xx_hal.h"
#include "pid_algo.h"
#include "rp_device.h"
#include "rp_driver.h"

//发送邮箱ID号  std_id
//RM2006 RM3508
#define RM_F_ID 0x200
#define RM_B_ID 0x1FF
//GM6020
#define GM_F_ID 0x1FF
#define GM_B_ID 0x2FF

#define POWER_ID RM_F_ID
#define GIMBA_ID GM_F_ID
#define SHOOT_ID RM_F_ID

#define POWER_CAN 1 //4
#define GIMBA_CAN 1 //2
#define SHOOT_CAN 2 //3

//CAN1
#define CAN_ID_POWER_0	0x201U//202
#define CAN_ID_POWER_1	0x202U//201
#define CAN_ID_POWER_2	0x203U//前轮
#define CAN_ID_POWER_3	0x204U//后轮

#define CAN_ID_GIMB_Y	0x205U//
#define CAN_ID_GIMB_P	0x206U//

//CAN2
#define CAN_ID_FRIC_L	0x201U//
#define CAN_ID_FRIC_R	0x202U//
#define CAN_ID_BOX	  0x203U//
#define CAN_ID_BARREL	0x204U//


/* 电机类型索引 */
typedef enum {
	CHASSIS_POWER,
	CHASSIS_TURN,
	SHOOT_FRIC,
	SHOOT_FEED,	
	SHOOT_COVER,
	GIMBAL,	
} motor_type_t;

/*电机设备索引*/
typedef enum {		
	POWER_0,
	POWER_1,
	POWER_2,
	POWER_3,	
	
  CHAS_MOTOR_CNT,
} chas_motor_cnt_t;

typedef enum {		
	GIMB_Y,
	GIMB_P,
	
  GIMB_MOTOR_CNT,
} gimb_motor_cnt_t;

typedef enum {		
	Left_Fric,
	Right_Fric,	
	Feed,
	Cover,
	
  SHOT_MOTOR_CNT,
} shot_motor_cnt_t;
/* PID类型索引 */

typedef struct {
	pid_ctrl_t	speed;
	pid_ctrl_t	angle;
	
	pid_ctrl_t  Mec_Speed_Pid;
	pid_ctrl_t  Mec_Angle_Pid;
	
	pid_ctrl_t  Imu_Speed_Pid;
	pid_ctrl_t  Imu_Angle_Pid;
	
	pid_ctrl_t  For_Chas_Speed_Pid;
	pid_ctrl_t  For_Chas_Angle_Pid;
	
	pid_ctrl_t  Friction_Pid;
	
	pid_ctrl_t  Feed_Speed_Pid;
	
	pid_ctrl_t  Feed_Single_Speed_Pid;
	pid_ctrl_t  Feed_Single_Angle_Pid;

  float       out;
} motor_pid_t;

typedef enum {
	MOTOR_F,
	MOTOR_B
} motor_dir_t;

typedef struct motor_info_struct {
	uint16_t	  angle;
	int16_t		  speed;
	int16_t		  current;
	uint8_t     temperature;
	int16_t     Torque;
	uint16_t	  last_angle;
	int32_t		  total_angle;
	uint8_t		  init_flag;
	motor_dir_t dir;
	int16_t     MID;
	uint8_t		  offline_cnt;
	uint8_t		  offline_max_cnt;	
} motor_info_t;

typedef struct motor_struct{
	motor_info_t       *info;
	drv_can_t			     *driver;
	motor_pid_t        *pid;
	motor_type_t       type;
	dev_id_t				   id;
	dev_work_state_t	 work_state;
} motor_t;

#endif


