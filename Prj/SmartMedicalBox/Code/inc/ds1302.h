#ifndef _DS1302_H_
#define _DS1302_H_

#include "include.h"

#define DSIO	PTB0
#define RST		PTB1
#define SCLK	PTB2

typedef enum {
	READ_ADDR_YEAR		= 0x8d,
	READ_ADDR_MONTH		= 0x89,
	READ_ADDR_WEEK		= 0x8b,
	READ_ADDR_DAY		= 0x87,
	READ_ADDR_HOUR		= 0x85,
	READ_ADDR_MINUTE	= 0x83,
	READ_ADDR_SECOND	= 0x81,

	WRITE_ADDR_YEAR		= 0x8c,
	WRITE_ADDR_MONTH	= 0x88,
	WRITE_ADDR_WEEK		= 0x8a,
	WRITE_ADDR_DAY		= 0x86,
	WRITE_ADDR_HOUR		= 0x84,
	WRITE_ADDR_MINUTE	= 0x82,
	WRITE_ADDR_SECOND	= 0x80,
	
	WRITE_PROTECT		= 0x8e
} Ds1302RegAddr;

typedef struct {
	uint8 year;
	uint8 month;
	uint8 day;
	uint8 week;
	uint8 hour;
	uint8 minute;
	uint8 second;
} Time;

void ds1302_init();										// 初始化 ds1302
void ds1302_write(Ds1302RegAddr addr, uint8 data);		// 向寄存器写数据
uint8 ds1302_read(Ds1302RegAddr addr);					// 从寄存器读数据
void ds1302_set_time(Time time);						// 设置 ds1302 时间
Time ds1302_get_time();									// 读取 ds1302 时间

#endif