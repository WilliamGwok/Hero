#ifndef __MATHEMATICS_H
#define __MATHEMATICS_H

#include "stm32f4xx_hal.h"

#define abs(x) ((x)>0? (x):(-(x)))
#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))

#endif


