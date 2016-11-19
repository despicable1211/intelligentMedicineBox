#ifndef _DHT11_H_
#define _DHT11_H_

#include "include.h"

#define DATA_PORT	PTB4

typedef struct
{
	float temperature;
	float humidity;
} Dht11Data;

void dht11_init();					// dht11 ��ʼ��
Dht11Data dth11_read_data();		// ��ȡ dht11 ��ʪ������

#endif