#include "rc.h"
#include "imu_sensor.h"
#include "Motor.h"
#include "rp_math.h"
#include "vision_sensor.h"
#include "vision_protocal.h"
#include "string.h"

extern void vision_update(vision_sensor_t *vision, uint8_t *rxBuf);
extern void vision_init(vision_sensor_t *vision);
static void vision_check(vision_sensor_t *vision);
static void vision_heart_beat(vision_sensor_t *vision);

drv_uart_t vision_sensor_driver = {
	.type = DRV_UART3,
};

vision_info_t vision_sensor_info = {
	.State.offline_max_cnt = 200,
};

vision_sensor_t	vision_sensor = {
	.info = &vision_sensor_info,
	.init = vision_init,
	.update = vision_update,
	.check = vision_check,
	.heart_beat = vision_heart_beat,
	.work_state = DEV_OFFLINE,
	.id = DEV_ID_VISION	,
};

static void vision_check(vision_sensor_t *vision)
{

	Receive_Data_t *Pack = &vision_sensor.info->RxPack.RxData;
    
	memcpy(&vision_sensor.info->RxPack,vision_sensor.info->RxPack.RxData.data, Len_Vision_Rx_Pack);
	
	if(Pack->pitch_angle == NULL)Pack->pitch_angle = imu_sensor.info->pitch;  
	if(Pack->yaw_angle   == NULL)Pack->yaw_angle   = imu_sensor.info->yaw;
	
	if(Pack->pitch_angle <= 8191 || Pack->pitch_angle >= 0)Pack->pitch_angle = Pack->pitch_angle;
	else                                                   Pack->pitch_angle = imu_sensor.info->pitch;
	if(Pack->yaw_angle   <= 8191 || Pack->yaw_angle   >= 0)Pack->yaw_angle   = Pack->yaw_angle  ;
	else                                                   Pack->yaw_angle   = imu_sensor.info->yaw;
	
	if(!Pack->is_find_target && !Pack->is_find_dafu)
	{
		Pack->pitch_angle = imu_sensor.info->pitch; 
		Pack->yaw_angle   = imu_sensor.info->yaw;
	}
	
	if(VISION_OFFLINE)
	{
		Pack->pitch_angle = imu_sensor.info->pitch; 
		Pack->yaw_angle   = imu_sensor.info->yaw;
	}
}



static void vision_heart_beat(vision_sensor_t *vision_sen)
{
	vision_info_t *vision_info = vision_sen->info;

	vision_info->State.offline_cnt++;
	
	if(vision_info->State.offline_cnt > vision_info->State.offline_max_cnt) 
	{
		vision_info->State.offline_cnt = vision_info->State.offline_max_cnt;
		vision_sen->work_state = DEV_OFFLINE;
	} 
	else 
	{
		if(vision_sen->work_state == DEV_OFFLINE)
			vision_sen->work_state = DEV_ONLINE;
	}	
	
}


