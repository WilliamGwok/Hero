#ifndef __CAN_PROTOCAL_H
#define __CAN_PROTOCAL_H

#include "stm32f4xx_hal.h"

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif

