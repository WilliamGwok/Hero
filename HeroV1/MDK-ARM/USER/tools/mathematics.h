#ifndef __MATHEMATICS_H
#define __MATHEMATICS_H

#include "stm32f4xx_hal.h"

#define abs(x) ((x)>0? (x):(-(x)))
#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define sgn(x) (((x)>0)?1:((x)<0?-1:0))
#define one(x)					((x)>0? (1):(-1))

float lowpass(float X_last, float X_new, float K);

#endif


