#ifndef _MPU90615_H_
#define _MPU90615_H_

#include "include.h"

#define MUP90615_UART		UART4
#define MUP90515_VECTORn	UART4_RX_TX_VECTORn

#define	FRAME_HANDER		0x5a
#define	DATA_TYPE			0x45
#define	DATA_SIZE			0x04

typedef struct
{
	uint8 targetH;
	uint8 targetL;
	uint8 environmentH;
	uint8 environmentL;
} Mpu90615Data;

void mpu90615_init();
void mpu90615_start();
void mpu90615_stop();
Mpu90615Data getMpu90615Data();

#endif