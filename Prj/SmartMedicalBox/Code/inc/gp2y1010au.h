#ifndef _GP2Y1010AU_H_
#define _GP2Y1010AU_H_

#include "include.h"

#define GP2Y1010AU_ADC		ADC0_SE16
#define GP2Y1010AU_LED		PTE12
#define GP2Y1010AU_PTI		PIT1
#define GP2Y1010AU_TIMER	10

typedef enum{
	AIR_LEVEL_1,
	AIR_LEVEL_2,
	AIR_LEVEL_3,
	AIR_LEVEL_4,
	AIR_LEVEL_UNKNOW
} AirLevel;									// 空气质量级别

void gp2y1010au_init();
void gp2y1010au_start();
void gp2y1010au_stop();
uint8 gp2y1010au_get_value();
AirLevel gp2y1010au_get_level();

#endif