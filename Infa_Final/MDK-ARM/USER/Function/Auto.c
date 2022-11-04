#include "Auto.h"
#include "Gimbal.h"
#include "Shooting.h"
#include "tim.h"
#include "imu_sensor.h"
#include "crc.h"
#include "string.h"

uint8_t vision_txbuf[Len_Vision_Tx_Pack];

Vision_Cmd_Id_t AUTO_CMD_MODE = CMD_AIM_AUTO;//CMD_AIM_OFF;

Vision_Cmd_Id_t Cmd_Last;

extern UART_HandleTypeDef huart3;

float Yaw_Tran = 0.0f;
float Pitch_Tran = 0.0f;

/*.....................角度信息处理.....................*/
void Angle_Transform(void)
{
	float yaw = 0.0f,pitch = 0.0f;
	
	yaw = imu_sensor.info->yaw;
	
	pitch = imu_sensor.info->pitch;
	
	if(yaw >=0 && yaw <= 180)
		Yaw_Tran = 8191 - (yaw/360.0f) * 8191;
	else
		Yaw_Tran = abs(yaw/360.0f*8191);

	
		Pitch_Tran = (pitch/360.0f)*8191 + 4096;
}




/*.....................发送信息.....................*/

//得到信息
void Vision_Get_Data(void)
{
	Angle_Transform();
	
	vision_sensor.info->TxPack.TxData.yaw = Yaw_Tran;
	
	vision_sensor.info->TxPack.TxData.pitch = Pitch_Tran;
	
	//还没有处理发射速度和己方颜色
	
	vision_txbuf[sof] = Vision_Frame_Header;
	
	vision_txbuf[Cmd_ID] = AUTO_CMD_MODE;
}

//发送---->
void Vision_Transmit_Data(void)
{
	Cmd_Last = vision_sensor.info->TxPack.FrameHeader.cmd_id;
	
	vision_sensor.info->TxPack.FrameHeader.sof = vision_txbuf[sof];
	vision_sensor.info->TxPack.FrameHeader.cmd_id = (Vision_Cmd_Id_t)vision_txbuf[Cmd_ID];
	vision_sensor.info->TxPack.FrameHeader.crc8 = vision_txbuf[Crc8];
	
	Append_CRC8_Check_Sum(vision_txbuf, Len_Frame_Header);
	
	memcpy(&vision_txbuf[Data],&vision_sensor.info->TxPack.TxData,Len_Tx_Data);
	
	Append_CRC16_Check_Sum(vision_txbuf, Len_Vision_Tx_Pack);
	
	HAL_UART_Transmit_DMA(&huart3, vision_txbuf, Len_Vision_Tx_Pack);
}















