#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "include.h"

#define DSPORT	PTB3

void ds18b20_init();					// ��ʼ�� ds18b20
bool ds18b20_wait();					// ׼�� ds18b20 ����
void ds18b20_write_byte(uint8 data);	// �� ds18b20 д��һ���ֽ�
uint8 ds18b20_read_byte();				// ��ȡ ds18b20 һ���ֽ�
void ds18b20_change_temp();				// �� ds18b20 ��ʼת���¶�
void ds18b20_read_temp_com();			// ���Ͷ�ȡ���¶�����
float ds18b20_read_temp();				// ��ȡ�¶�

#endif