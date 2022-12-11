#include "vision.h"
#include "string.h"
#include "config_status.h"
#include "gimbal.h"

vision_base_info_t vision_base_info;
vision_info_t vision_info;
vision_process_info_t vision_process_info;
vision_config_t vision_config = 
{
	.offline_cnt_max = 50,
};

vision_t vision = 
{
	.base_info = &vision_base_info,
	.info = &vision_info,
	.config = &vision_config,
	.pro_info = &vision_process_info,
	.tx_info = &vision_tx_info,
	.rx_info = &vision_rx_info,
};

void vision_tick_task(vision_t *vision)
{
	vision_info_t *info = vision->info;
	
	info->offline_cnt++;
	if(info->offline_cnt >= vision->config->offline_cnt_max)
	{
		info->offline_cnt = vision->config->offline_cnt_max + 1;
		info->status = DEV_OFFLINE;
	}
}

bool vision_send(uint8_t datau8_1,float dataf_1,float dataf_2,uint8_t datau8_2)
{
	memcpy(&vision_tx_info.datau8_1, &datau8_1, 1);
	memcpy((void*)&vision_tx_info.datas_1, &dataf_1, 4);
	memcpy((void*)&vision_tx_info.datas_2, &dataf_2, 4);
	memcpy(&vision_tx_info.datau8_2, &datau8_2, 1);
	if(vision_send_data() == true)
	{
			return true;
	}
	return false;
}

bool vision_receive(uint8_t *rxBuf)
{
	vision_base_info_t *base_info = vision.base_info;
	vision_info_t *info = vision.info;
	vision_process_info_t *pro_info = vision.pro_info;
	
	info->offline_cnt = 0;
	
	if(vision_recieve_data(rxBuf) == true)
	{
		memcpy(&base_info->pitch_receive,(void*)&vision_rx_info.dataf_1,4);
		memcpy(&base_info->yaw_receive,(void*)&vision_rx_info.dataf_2,4);
		memcpy(&base_info->distance_receive,(void*)&vision_rx_info.dataf_3,4);
		memcpy(&base_info->capture_Y_O_N,&vision_rx_info.datau8_2,1);
		memcpy(&base_info->target_num,&vision_rx_info.datau8_3,1);
		
		pro_info->pitch_move = base_info->pitch_receive - Gimbal.info->pitch_angle_imu_measure;
		pro_info->yaw_move = base_info->yaw_receive - Gimbal.info->yaw_angle_imu_measure;
		
		return true;
	}
	return false;
}


