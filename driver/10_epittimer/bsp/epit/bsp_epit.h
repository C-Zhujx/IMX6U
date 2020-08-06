#ifndef __BSP_EPIT_H
#define __BSP_EPIT_H


#include "imx6u.h"

void epit1_irqhandler(void);
void epit_init(unsigned int frac, unsigned int value);
#endif