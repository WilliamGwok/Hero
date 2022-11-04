#ifndef __RP_DEVICE_H
#define __RP_DEVICE_H

#include "stm32f4xx_hal.h"





/* ??? --------------------------------------------------------------------*/
/**
 *	@brief	??id?? drv_id
 *	@class	device
 */
typedef enum {
		DEV_ID_NONE,
    DEV_ID_RC,   
    DEV_ID_IMU, 

		DEV_ID_JUDGE,
	  DEV_ID_VISION,
	
    DEV_ID_POWER_0,
    DEV_ID_POWER_1,
    DEV_ID_POWER_2,
    DEV_ID_POWER_3,
		
    DEV_ID_TURN_0,
    DEV_ID_TURN_1,
    DEV_ID_TURN_2,
    DEV_ID_TURN_3,		

    DEV_ID_FRIC_L,
    DEV_ID_FRIC_R,
    DEV_ID_BOX,
    DEV_ID_BARREL,
		
    DEV_ID_GIMB_Y,
    DEV_ID_GIMB_P,
		DEV_ID_CNT,
} dev_id_t;


/**
 *	@brief	??????
 *	@class	device
 */
typedef enum {
    DEV_ONLINE,
    DEV_OFFLINE,
} dev_work_state_t;

/**
 *	@brief	????
 *	@class	device
 */
typedef enum {
    NONE_ERR,		// ??(???)
		DEV_TYPE_ERR,
    DEV_ID_ERR,		// ??ID??	
    DEV_INIT_ERR,	// ???????
    DEV_DATA_ERR,	// ??????
} dev_errno_t;



/**
 *	@brief	?????????
 *	@class	device
 */


typedef struct device {
    void							*info;		// ????????????
    void							*driver;	// ????????????
    void							(*init)(struct device *self);	// ???????
    void							(*update)(struct device *self, uint8_t *rxBuf);	// ????????
    void							(*check)(struct device *self);	// ????????
    void							(*heart_beat)(struct device *self);	// ?????
    dev_work_state_t	work_state;	// ??????
    dev_errno_t				errno;		// ????????????
    dev_id_t					id;			// ??id
} device_t;



#endif

