#ifndef _PULSESENSOR_H_
#define _PULSESENSOR_H_

#include "include.h"

#define PULSESENSOR_MIN			245
#define PULSESENSOR_PORT		ADC1_SE16
#define PULSESENSOR_PIT			PIT0
#define PULSESENSOR_TIMER		1

void pulsesensor_init();
void pulsesensor_start();
void pulsesensor_stop();
uint8 pulsesensor_get_times();

#endif