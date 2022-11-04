#include  "vision_protocal.h"
#include  "vision_sensor.h"
#include  "crc.h"
#include  "string.h"
#include  "Freertos.h"
#include  "task.h"


void Init_vision(void)
{
	vision_sensor.init(&vision_sensor);
}


void vision_init(vision_sensor_t *vision)
{
	
	vision->info->State.offline_cnt = vision->info->State.offline_max_cnt + 1;
	vision->work_state = DEV_OFFLINE;
	
	if(vision->id == DEV_ID_VISION)
		vision->errno = NONE_ERR;
	else
		vision->errno = DEV_ID_ERR;	
}



void vision_update(vision_sensor_t *vision_sen, uint8_t *rxBuf)
{
	vision_info_t *vision_info = vision_sen->info;

	uint8_t res = false;
	vision_info->State.rx_cnt++;
	
	if(rxBuf[sof] == Vision_Frame_Header) 
	{	
		res = Verify_CRC8_Check_Sum( rxBuf, Len_Frame_Header );
		
		if(res == true)
		{
			res = Verify_CRC16_Check_Sum( rxBuf, Len_Vision_Rx_Pack );
			
			if(res == true) 
			{
				
				memcpy(vision_info->RxPack.RxData.data, rxBuf, Len_Vision_Rx_Pack);
				vision_info->State.rx_data_update = true;	
				{
						memcpy((void*)(vision_info->RxPack.RxData.jiesuan),vision_info->RxPack.RxData.data+3,Len_Vision_Rx_Pack-5);
				}
				
				vision_info->State.rx_time_now  = xTaskGetTickCountFromISR();
				vision_info->State.rx_time_fps  = vision_info->State.rx_time_now - vision_info->State.rx_time_prev;
				vision_info->State.rx_time_prev = vision_info->State.rx_time_now;		
				vision_info->State.offline_cnt  = 0;
			}
		}
	}	
	
	if(res == true) 
	{
		vision_info->State.rx_data_valid = true;
	} 
	else if(res == false) 
	{
		vision_info->State.rx_data_valid = false;
		vision_info->State.rx_err_cnt++;
	}
}

