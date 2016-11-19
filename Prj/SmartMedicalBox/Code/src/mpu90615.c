#include "mpu90615.h"

static uint8 receiveDate[9];
static uint8 receivePointer;
static Mpu90615Data mpu90615Data;

void mpu90615_handler(void)
{
	char ch;
	int16 tmp;
	if(UART_S1_REG(UARTN[MUP90615_UART]) & UART_S1_RDRF_MASK)
	{
		uart_getchar (MUP90615_UART, &ch);
		
		if((ch == FRAME_HANDER && receiveDate[0] != FRAME_HANDER && receiveDate[1] != FRAME_HANDER && receivePointer == 0)
		|| (ch == FRAME_HANDER && receiveDate[0] == FRAME_HANDER && receiveDate[1] != FRAME_HANDER && receivePointer == 1)
		|| (receiveDate[0] == FRAME_HANDER && receiveDate[1] == FRAME_HANDER && receivePointer >= 2 && receivePointer < sizeof(receiveDate)))
			receiveDate[receivePointer++] = ch;
		
		if(receivePointer >= sizeof(receiveDate))
		{
			receivePointer = 0;
			if(receiveDate[0] == FRAME_HANDER && receiveDate[1] == FRAME_HANDER)
			{
				if(receiveDate[2] == DATA_TYPE
				&& receiveDate[3] == DATA_SIZE					
				&& ((receiveDate[0]
				   + receiveDate[1]
				   + receiveDate[2]
				   + receiveDate[3]
				   + receiveDate[4]
				   + receiveDate[5]
				   + receiveDate[6]
				   + receiveDate[7]) & 0xff) == receiveDate[8])
				{
					tmp = receiveDate[4];
					tmp <<= 8;
					tmp |= receiveDate[5];					
					mpu90615Data.targetH = tmp/100;
					mpu90615Data.targetL = tmp%100;
					
					tmp = receiveDate[6];
					tmp <<= 8;
					tmp |= receiveDate[7];
					mpu90615Data.environmentH = tmp/100;
					mpu90615Data.environmentL = tmp%100;
				}
					
				receiveDate[0] = 0;
				receiveDate[1] = 0;
			}
		}
	}
}

void mpu90615_init()
{
	receivePointer = 0;
	uart_init(MUP90615_UART,115200);
	set_vector_handler(MUP90515_VECTORn,mpu90615_handler);
}

void mpu90615_start()
{
	uart_rx_irq_en(MUP90615_UART);
}

void mpu90615_stop()
{
	uart_rx_irq_dis(MUP90615_UART);
}

Mpu90615Data getMpu90615Data()
{
	return mpu90615Data;
}