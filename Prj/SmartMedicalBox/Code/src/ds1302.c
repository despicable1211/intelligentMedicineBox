#include "ds1302.h"

#define numToBcd(num)	((num/10)<<4)|(num%10)
#define bcdToNum(bcd)	(bcd>>4)*10+(bcd&0x0f)

static Time ds1302Time;

/* 初始化 ds1302 */
void ds1302_init()
{
	Time settingTime;
	
	gpio_init(DSIO, GPO, 0);
	gpio_init(RST, GPO, 0);
	gpio_init(SCLK, GPO, 0);
	
	settingTime.year = 16;
	settingTime.month = 8;
	settingTime.day = 8;
	settingTime.week = 1;
	settingTime.hour = 0;
	settingTime.minute = 0;
	settingTime.second = 0;
	
	ds1302_set_time(settingTime);
}

/* 向寄存器写数据 */
void ds1302_write(Ds1302RegAddr addr, uint8 data)
{
	uint8 n, addrTemp = addr;
	
	gpio_set(RST, 0);
	DELAY_US(1);
		
	gpio_set(SCLK, 0);
	DELAY_US(1);
	gpio_set(RST, 1);
	DELAY_US(1);
	
	gpio_ddr(DSIO, GPO);
	/* 发送地址 */
	for(n = 0; n < 8; n++)
	{
		gpio_set(DSIO, addrTemp & 0x01);		// 数据从低位开始发送
		addrTemp >>= 1;
		gpio_set(SCLK, 1);
		DELAY_US(1);
		gpio_set(SCLK, 0);
		DELAY_US(1);
	}
	/* 发送数据 */
	for(n = 0; n < 8; n++)
	{
		gpio_set(DSIO, data & 0x01);
		data >>= 1;
		gpio_set(SCLK, 1);
		DELAY_US(1);
		gpio_set(SCLK, 0);
		DELAY_US(1);
	}
	gpio_set(RST, 0);
	DELAY_US(1);
}

/* 从寄存器读取数据 */
uint8 ds1302_read(Ds1302RegAddr addr)
{
	uint8 n, data, dataTemp, addrTemp = addr;
	
	gpio_set(RST, 0);
	DELAY_US(1);
		
	gpio_set(SCLK, 0);
	DELAY_US(1);
	gpio_set(RST, 1);
	DELAY_US(1);
	
	gpio_ddr(DSIO, GPO);
	/* 发送地址 */
	for(n = 0; n < 8; n++)
	{
		gpio_set(DSIO, addrTemp & 0x01);		// 数据从低位开始发送
		addrTemp >>= 1;
		gpio_set(SCLK, 1);
		DELAY_US(1);
		gpio_set(SCLK, 0);
		DELAY_US(1);
	}
	gpio_ddr(DSIO, GPI);
	DELAY_US(1);
	/* 读取数据 */
	for(n = 0; n < 8; n++)
	{
		dataTemp = gpio_get(DSIO);
		data = (data >> 1) | (dataTemp << 7);
		gpio_set(SCLK, 1);
		DELAY_US(1);
		gpio_set(SCLK, 0);
		DELAY_US(1);
	}
	gpio_set(RST, 0);
	DELAY_US(1);
	
	/* DS1302复位的稳定时间 */
	gpio_set(SCLK, 1);
	DELAY_US(1);
	gpio_ddr(DSIO, GPO);
	gpio_set(DSIO, 0);
	DELAY_US(1);
	gpio_set(DSIO, 1);
	DELAY_US(1);
	
	return data;
}

/* 设置 ds1302 时间 */
void ds1302_set_time(Time time)
{
	ds1302_write(WRITE_PROTECT, 0x00);			// 禁止写保护
	ds1302_write(WRITE_ADDR_YEAR, numToBcd(time.year));
	ds1302_write(WRITE_ADDR_MONTH, numToBcd(time.month));
	ds1302_write(WRITE_ADDR_WEEK, numToBcd(time.week));
	ds1302_write(WRITE_ADDR_DAY, numToBcd(time.day));
	ds1302_write(WRITE_ADDR_HOUR, numToBcd(time.hour));
	ds1302_write(WRITE_ADDR_MINUTE, numToBcd(time.minute));
	ds1302_write(WRITE_ADDR_SECOND, numToBcd(time.second));
	ds1302_write(WRITE_PROTECT, 0x80);			// 打开写保护
}
	
/* 读取 ds1302 时间 */
Time ds1302_get_time()
{
	ds1302Time.year = ds1302_read(READ_ADDR_YEAR);
	ds1302Time.month = ds1302_read(READ_ADDR_MONTH);
	ds1302Time.week = ds1302_read(READ_ADDR_WEEK);
	ds1302Time.day = ds1302_read(READ_ADDR_DAY);
	ds1302Time.hour = ds1302_read(READ_ADDR_HOUR);
	ds1302Time.minute = ds1302_read(READ_ADDR_MINUTE);
	ds1302Time.second = ds1302_read(READ_ADDR_SECOND);
	
	ds1302Time.year = bcdToNum(ds1302Time.year);
	ds1302Time.month = bcdToNum(ds1302Time.month);
	ds1302Time.week = bcdToNum(ds1302Time.week);
	ds1302Time.day = bcdToNum(ds1302Time.day);
	ds1302Time.hour = bcdToNum(ds1302Time.hour);
	ds1302Time.minute = bcdToNum(ds1302Time.minute);
	ds1302Time.second = bcdToNum(ds1302Time.second);
	
	return ds1302Time;
}













