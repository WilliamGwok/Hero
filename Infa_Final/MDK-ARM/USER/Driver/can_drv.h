#ifndef __CAN_DRV_H
#define __CAN_DRV_H

#include "stm32f4xx_hal.h"
#include "rp_driver.h"

typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t 			data[8];
} CAN_RxFrameTypeDef;

typedef struct {
	CAN_TxHeaderTypeDef header;
	uint8_t				data[8];
} CAN_TxFrameTypeDef;

void CAN1_Init(void);
void CAN2_Init(void);
uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint32_t stdId, int16_t *dat);
void CAN_SendSingleData(drv_can_t *drv, int16_t txData);
uint8_t CAN1_SendData(uint32_t stdId, int16_t *dat);
uint8_t CAN2_SendData(uint32_t stdId, int16_t *dat);
void CAN_Transmit(uint32_t std, uint8_t *data,char can,uint32_t DL);

#endif




