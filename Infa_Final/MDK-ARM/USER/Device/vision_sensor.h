#ifndef __VISION_SENSOR_H
#define __VISION_SENSOR_H

#include "rp_config.h"
#include "uart_drv.h"

#define VISION_ONLINE  vision_sensor.work_state == DEV_ONLINE
#define VISION_OFFLINE vision_sensor.work_state == DEV_OFFLINE

#define 	VISION_SOF					(0xA5)

typedef enum {
	CMD_AIM_OFF 		    = 0x00,	
	CMD_AIM_AUTO		    = 0x01,	
	CMD_AIM_SMALL_BUFF	= 0x02,	
	CMD_AIM_BIG_BUFF	  = 0x03,	
	CMD_AIM_ANTOP	   	  = 0x04,	
	CMD_AIM_ANDF		    = 0x05	
} Vision_Cmd_Id_t;

//data length
typedef enum{
	Len_Vision_Rx_Pack  = 16,
	Len_Vision_Tx_Pack  = 15,
	
	Len_Rx_Data         = Len_Vision_Rx_Pack - 5,
	Len_Tx_Data         = Len_Vision_Tx_Pack - 5,
	
	Len_Frame_Header    = 3,
	Len_Frame_Tailer    = 2,
}Vision_Data_Length_t;

typedef enum{
	Hand_Mode   = 0,
	Auto_Mode   = 1,
	Large_Buff  = 2,
	Small_Buff  = 3,
}Vision_Mode_t;

typedef enum {
	sof			  = 0,
	Cmd_ID		= 1,
	Crc8		  = 2,
	Data		  = Len_Frame_Header,
	TX_CRC16	=Len_Frame_Header + Len_Tx_Data,
} Vision_Frame_Header_Offset_t;



/*.....................帧头帧尾.....................*/

typedef __packed struct
{
	uint8_t  			    sof;		
	Vision_Cmd_Id_t  	cmd_id;	
	uint8_t  			    crc8;		
} Vision_Frame_Header_t;

typedef __packed struct 
{
	uint16_t crc16;					
} Vision_Frame_Tailer_t;




/*.....................接收数据包.....................*/

typedef __packed struct {

	float   pitch_angle;
	float   yaw_angle;
	
	uint8_t is_find_target;
	uint8_t is_find_dafu;
	uint8_t is_hit_enable;
	
	char    data[Len_Vision_Rx_Pack];
	float   jiesuan[(Len_Vision_Rx_Pack-5)/4];
}Receive_Data_t;

typedef __packed struct{
	Vision_Frame_Header_t FrameHeader;
	Receive_Data_t        RxData;
	Vision_Frame_Tailer_t FrameTailer;
}Vision_Rx_Pack_t;



/*.....................发送数据包.....................*/

typedef __packed struct {

	float   yaw;
	float   pitch;
	
	uint8_t shoot_speed;
	uint8_t my_color;
	
	
}Transmit_Data_t;

typedef __packed struct
{
	Vision_Frame_Header_t FrameHeader;	
	Transmit_Data_t	    TxData;		    
	Vision_Frame_Tailer_t FrameTailer;	
} Vision_Tx_Pack_t;


/*.....................状态包.....................*/

typedef struct{
	uint8_t 		   my_color;
	Vision_Mode_t	 mode;
	uint8_t  		   rx_data_valid;
	uint16_t 		   rx_err_cnt;
	uint32_t		   rx_cnt;
	bool		       rx_data_update;
	uint32_t 		  rx_time_prev;	
	uint32_t 		  rx_time_now;	 
	uint16_t 		  rx_time_fps;	 
	int16_t		     offline_cnt;
	int16_t		     offline_max_cnt;
}Vision_State_t;



/*.....................总信息包.....................*/
typedef struct{
	Vision_Rx_Pack_t   RxPack;
	Vision_Tx_Pack_t   TxPack;
	Vision_State_t     State;
}vision_info_t;



/*.....................视觉包.....................*/
typedef struct vision_sensor_struct {
	vision_info_t    *info;
	drv_uart_t       *driver;
	dev_work_state_t work_state;
	dev_id_t         id;
	dev_errno_t			  errno;
	
	void             (*init)(struct vision_sensor_struct *self);
	void             (*update)(struct vision_sensor_struct *self, uint8_t *rxBuf);
	void             (*check)(struct vision_sensor_struct *self);
	void             (*heart_beat)(struct vision_sensor_struct *self);
	
}vision_sensor_t;

extern vision_info_t    vision_sensor_info;
extern vision_sensor_t	vision_sensor;

#endif
