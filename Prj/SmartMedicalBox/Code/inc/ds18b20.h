#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "include.h"

#define DSPORT	PTB3

void ds18b20_init();					// 初始化 ds18b20
bool ds18b20_wait();					// 准备 ds18b20 总线
void ds18b20_write_byte(uint8 data);	// 向 ds18b20 写入一个字节
uint8 ds18b20_read_byte();				// 读取 ds18b20 一个字节
void ds18b20_change_temp();				// 让 ds18b20 开始转换温度
void ds18b20_read_temp_com();			// 发送读取的温度命令
float ds18b20_read_temp();				// 读取温度

#endif