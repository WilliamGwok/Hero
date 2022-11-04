#include "can_drv.h"
#include "Motor.h"
#include "can_protocal.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
CAN_RxFrameTypeDef hcan1RxFrame;//��������֡�ṹ�壬��header�� uint8_t data[8]
CAN_TxFrameTypeDef hcan1TxFrame;//��������֡�ṹ�壬��header�� uint8_t data[8]
CAN_RxFrameTypeDef hcan2RxFrame;//��������֡�ṹ�壬��header�� uint8_t data[8]
CAN_TxFrameTypeDef hcan2TxFrame;//��������֡�ṹ�壬��header�� uint8_t data[8]
/**
 *	@brief	CAN��ʶ����������λΪĬ��
 */
static void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig)
{
	sFilterConfig->FilterIdHigh = 0;						
	sFilterConfig->FilterIdLow = 0;							
	sFilterConfig->FilterMaskIdHigh = 0;					
	sFilterConfig->FilterMaskIdLow = 0;						
	sFilterConfig->FilterFIFOAssignment = CAN_FILTER_FIFO0;	
	sFilterConfig->FilterBank = 0;							
	sFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;		
	sFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;		
	sFilterConfig->FilterActivation = ENABLE;				
	sFilterConfig->SlaveStartFilterBank = 0;
}

/**
 *	@brief	CAN1 ��ʼ��
 */
void CAN1_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	
	HAL_CAN_Start(&hcan1);
}

void CAN2_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
	
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	HAL_CAN_Start(&hcan2);
}
/**
 *	@brief	CAN1 �����жϻص�����
 */
static void CAN_Rx_Callback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame.header, hcan1RxFrame.data);
		
		//��������µ��������
		CAN1_rxDataHandler(hcan1RxFrame.header.StdId, hcan1RxFrame.data);
	}
	else if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan2RxFrame.header, hcan2RxFrame.data);
		
		CAN2_rxDataHandler(hcan2RxFrame.header.StdId, hcan2RxFrame.data);
	}
}

/**
 *	@brief	��дCAN RxFIFO�жϽ��պ���
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		CAN_Rx_Callback(hcan);
	}
	else if(hcan->Instance == CAN2)
	{
		CAN_Rx_Callback(hcan);
	}
}
/*************** ������Ϣ ***************/
uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint32_t stdId, int16_t *dat)
{
	uint32_t txMailBox;
	CAN_TxFrameTypeDef *txFrame;
	if(hcan->Instance == CAN1)
		txFrame = &hcan1TxFrame;
	else if(hcan->Instance == CAN2)
		txFrame = &hcan2TxFrame;
	//���ñ�����Ϣ
	txFrame->header.StdId = stdId;
	txFrame->header.IDE = CAN_ID_STD;
	txFrame->header.RTR = CAN_RTR_DATA;
	txFrame->header.DLC = 8;
	//�ȷ���8λ���ٷ���8λ����Ҫ����һ��4��int16������
	txFrame->data[0] = (uint8_t)((int16_t)dat[0] >> 8);
	txFrame->data[1] = (uint8_t)((int16_t)dat[0]);
	txFrame->data[2] = (uint8_t)((int16_t)dat[1] >> 8);
	txFrame->data[3] = (uint8_t)((int16_t)dat[1]);
	txFrame->data[4] = (uint8_t)((int16_t)dat[2] >> 8);
	txFrame->data[5] = (uint8_t)((int16_t)dat[2]);
	txFrame->data[6] = (uint8_t)((int16_t)dat[3] >> 8);
	txFrame->data[7] = (uint8_t)((int16_t)dat[3]);
	//��������
	HAL_CAN_AddTxMessage(hcan, &txFrame->header, &txFrame->data[0], &txMailBox);
	return HAL_OK;
}

uint8_t CAN1_SendData(uint32_t stdId, int16_t *dat)
{
	return CAN_SendData(&hcan1, stdId, dat);
}

uint8_t CAN2_SendData(uint32_t stdId, int16_t *dat)
{
	return CAN_SendData(&hcan2, stdId, dat);
}

/*************** ��Ϣ���ʹ��� ***************/

/**
 *	@brief	CAN ���͵�������
 */
void CAN_SendSingleData(drv_can_t *drv, int16_t txData)
{
	int16_t txArr[4] = {0, 0, 0, 0};
	
	txArr[drv->drv_id] = txData;
	if(drv->type == DRV_CAN1)
		CAN1_SendData(drv->std_id, txArr);
	else if(drv->type == DRV_CAN2)
		CAN2_SendData(drv->std_id, txArr);
}


void CAN_Transmit(uint32_t std, uint8_t *data,char can,uint32_t DL)
{
	uint8_t i;
	CAN_TxFrameTypeDef Msg;
	uint32_t CAN_Tx_Mailbox;
	
	Msg.header.IDE = CAN_ID_STD;
	Msg.header.RTR = CAN_RTR_DATA;
	Msg.header.DLC = DL;    
	Msg.header.StdId = std; 	  
	
	Msg.data[0] = data[0];
	Msg.data[1] = data[1];	
	Msg.data[2] = data[2];	
	Msg.data[3] = data[3];

	Msg.data[4] = data[4];
	Msg.data[5] = data[5];	
	Msg.data[6] = data[6];	
	Msg.data[7] = data[7];
	
	if(can == 1) HAL_CAN_AddTxMessage(&hcan1,&Msg.header,Msg.data,  &CAN_Tx_Mailbox);
	if(can == 2) HAL_CAN_AddTxMessage(&hcan2,&Msg.header,Msg.data,  &CAN_Tx_Mailbox);

}

