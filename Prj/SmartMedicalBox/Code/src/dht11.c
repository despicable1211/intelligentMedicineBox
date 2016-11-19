#include "dht11.h"

static Dht11Data dht11Data;
uint8 com();

/* dht11 初始化 */
void dht11_init()
{
	dht11Data.temperature = 0;
	dht11Data.humidity = 0;
	gpio_init(DATA_PORT, GPO, 0);
}

/* 读取 dht11 温湿度数据 */
Dht11Data dth11_read_data()
{
	uint8 flag;
	int humH, humL, tempH, tempL, dataCheck;
	gpio_ddr(DATA_PORT, GPO);
	gpio_set(DATA_PORT, 0);
	DELAY_MS(18);
	gpio_set(DATA_PORT, 1);
	DELAY_US(20);
	gpio_ddr(DATA_PORT, GPI);
	if(!gpio_get(DATA_PORT))
	{
		flag = 2;
		while(!gpio_get(DATA_PORT) && flag++);
		flag = 2;
		while(gpio_get(DATA_PORT) && flag++);
		
		humH = com();
		humL = com();
		tempH = com();
		tempL = com();
		dataCheck = com();
		
		gpio_ddr(DATA_PORT, GPO);
		gpio_set(DATA_PORT, 1);
		
		if(dataCheck == humH + humL + tempH + tempL)
		{
			/* 保留一位小数点 */
			humL = (humL + 5)/10;			
			dht11Data.humidity = (humH * 10 + humL)/10;
			tempL = (tempL + 5)/10;			
			dht11Data.temperature = (tempH * 10 + tempL)/10;
		}
		/*else
		{
			dht11Data.temperature = 0;
			dht11Data.humidity = 0;
		}*/
	}
	return dht11Data;
}

uint8 com()
{
	uint8 i, data, flag, tmp;
	gpio_ddr(DATA_PORT, GPI);
	for(i = 0; i < 8; i++)
	{
		flag = 2;
		while(!gpio_get(DATA_PORT) && flag++);
		DELAY_US(30);
		
		tmp = 0;
		if(gpio_get(DATA_PORT))
			tmp = 1;
		flag = 2;
		while(gpio_get(DATA_PORT) && flag++);
		if(flag == 1)
			break;
		data <<= 1;
		data |= tmp;
	}
	return data;
}










