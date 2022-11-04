#ifndef __RP_MATH_H
#define __RP_MATH_H

#include "stm32f4xx_hal.h"

#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define anti_constrain(x, min, max)	(((x<max) && (x>min))?(0):x)
#define abs(x) 					((x)>0? (x):(-(x)))


float DeathZoom(float input, float center, float death);

#endif


