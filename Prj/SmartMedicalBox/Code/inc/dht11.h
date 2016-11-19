#ifndef _DHT11_H_
#define _DHT11_H_

#include "include.h"

#define DATA_PORT	PTB4

typedef struct
{
	float temperature;
	float humidity;
} Dht11Data;

void dht11_init();					// dht11 初始化
Dht11Data dth11_read_data();		// 读取 dht11 温湿度数据

#endif