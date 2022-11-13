#ifndef __MEAN_FILTER_H
#define __MEAN_FILTER_H

#include "stm32f4xx_hal.h"

#define FILTER_LEN  5

typedef struct Mean_Filter_Struct{
	char len;          
	char index;         
	float cnt;          
	float SPEED[FILTER_LEN];   
}Mean_Filter_t;

typedef struct Low_Pass_Struct{
	
}Low_Pass_t;

typedef struct Filter_Struct{
	
	Mean_Filter_t Mean_Filter;
	
	Low_Pass_t Low_Pass;
	
}Filter_t;

float Mean_Filter_Ctrl(Mean_Filter_t * filter,float mea);
void Mean_Filter_Init(Mean_Filter_t * filter);

#endif

