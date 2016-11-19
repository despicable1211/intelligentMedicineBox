#include "ds18b20.h"

/* 初始化 ds18b20 */
void ds18b20_init()
{
	gpio_init(DSPORT, GPO, 0);
}

/* 准备 ds18b20 总线 */
bool ds18b20_wait()
{
	int temp = 0;
	
	gpio_ddr(DSPORT, GPO);				// 拉低总线 480us ~ 960us
	gpio_set(DSPORT, 0);
	DELAY_US(640);
	gpio_set(DSPORT, 1);
	
	gpio_ddr(DSPORT, GPI);
	/* 等待 ds18b20 拉低总线 */
	while(gpio_get(DSPORT))
	{
		DELAY_US(1);
		if(++temp > 5000)
			return false;
	}
	return true;
}

/* 向 ds18b20 写入一个字节 */
void ds18b20_write_byte(uint8 data)
{
	uint8 i;
	gpio_ddr(DSPORT, GPO);
	for(i = 0; i < 8; i++)
	{
		gpio_set(DSPORT, 0);			// 每写入一位数据之前先把总线拉低1us
		DELAY_US(1);
		gpio_set(DSPORT, data&0x01);	// 写入一个数据，从最低位开始
		DELAY_US(70);					// 持续时间最少 60us
		gpio_set(DSPORT, 1);			// 释放总线，至少 1us 给总线恢复时间
		DELAY_US(1);
		data >>= 1;
	}
}

/* 读取 ds18b20 一个字节 */
uint8 ds18b20_read_byte()
{
	uint8 i, byte, byteTemp;
	for(i = 0; i < 8; i++)
	{
		gpio_ddr(DSPORT, GPO);
		gpio_set(DSPORT, 0);			// 每写入一位数据之前先把总线拉低1us
		DELAY_US(1);
		gpio_set(DSPORT, 1);			// 释放总线
		DELAY_US(6);					// 延时 6us 等待数据稳定
		
		gpio_ddr(DSPORT, GPI);
		byteTemp = gpio_get(DSPORT);	// 读取数据，从最低位开始读取
		byte = (byte >> 1)|(byteTemp << 7);	
		DELAY_US(48);
	}
	return byte;
}

/* 让 ds18b20 开始转换温度 */
void ds18b20_change_temp()
{
	ds18b20_wait();
	DELAY_MS(1);
	ds18b20_write_byte(0xcc);			// 跳过ROM操作命令		 
	ds18b20_write_byte(0x44);			// 温度转换命令
	DELAY_MS(100);
}

/* 发送读取的温度命令 */
void ds18b20_read_temp_com()
{
	ds18b20_wait();
	DELAY_MS(1);
	ds18b20_write_byte(0xcc);			// 跳过ROM操作命令		 
	ds18b20_write_byte(0xbe);			// 发送读取温度命令
}

/* 读取温度 */
float ds18b20_read_temp()
{
	int data;
	float temp;
	uint8 tmh, tml;
	ds18b20_change_temp();				// 写入转换命令
	ds18b20_read_temp_com();			// 等待转换完后发送读取温度命令
	tml = ds18b20_read_byte();			// 读取温度低 8 位
	tmh = ds18b20_read_byte();			// 读取温度高 8 位
	data = tmh;
	data <<= 8;
	data |= tml;
	data = (int)(data * 0.625 + 0.5);
	temp = data / 10;
	return temp;
}















