#include "can_protocal.h"
#include "Motor.h"
#include "couple.h"


/**
 *	@brief	CAN1 接收数据 数据处理
 */

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	
	MOTOR_CAN1_RX(canId, rxBuf);
}

void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
#if COUPLE == 1
	
	Up_Receive_Data(canId,rxBuf);
	
#endif
	
	MOTOR_CAN2_RX(canId, rxBuf);
}
