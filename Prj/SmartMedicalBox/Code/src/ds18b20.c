#include "ds18b20.h"

/* ��ʼ�� ds18b20 */
void ds18b20_init()
{
	gpio_init(DSPORT, GPO, 0);
}

/* ׼�� ds18b20 ���� */
bool ds18b20_wait()
{
	int temp = 0;
	
	gpio_ddr(DSPORT, GPO);				// �������� 480us ~ 960us
	gpio_set(DSPORT, 0);
	DELAY_US(640);
	gpio_set(DSPORT, 1);
	
	gpio_ddr(DSPORT, GPI);
	/* �ȴ� ds18b20 �������� */
	while(gpio_get(DSPORT))
	{
		DELAY_US(1);
		if(++temp > 5000)
			return false;
	}
	return true;
}

/* �� ds18b20 д��һ���ֽ� */
void ds18b20_write_byte(uint8 data)
{
	uint8 i;
	gpio_ddr(DSPORT, GPO);
	for(i = 0; i < 8; i++)
	{
		gpio_set(DSPORT, 0);			// ÿд��һλ����֮ǰ�Ȱ���������1us
		DELAY_US(1);
		gpio_set(DSPORT, data&0x01);	// д��һ�����ݣ������λ��ʼ
		DELAY_US(70);					// ����ʱ������ 60us
		gpio_set(DSPORT, 1);			// �ͷ����ߣ����� 1us �����߻ָ�ʱ��
		DELAY_US(1);
		data >>= 1;
	}
}

/* ��ȡ ds18b20 һ���ֽ� */
uint8 ds18b20_read_byte()
{
	uint8 i, byte, byteTemp;
	for(i = 0; i < 8; i++)
	{
		gpio_ddr(DSPORT, GPO);
		gpio_set(DSPORT, 0);			// ÿд��һλ����֮ǰ�Ȱ���������1us
		DELAY_US(1);
		gpio_set(DSPORT, 1);			// �ͷ�����
		DELAY_US(6);					// ��ʱ 6us �ȴ������ȶ�
		
		gpio_ddr(DSPORT, GPI);
		byteTemp = gpio_get(DSPORT);	// ��ȡ���ݣ������λ��ʼ��ȡ
		byte = (byte >> 1)|(byteTemp << 7);	
		DELAY_US(48);
	}
	return byte;
}

/* �� ds18b20 ��ʼת���¶� */
void ds18b20_change_temp()
{
	ds18b20_wait();
	DELAY_MS(1);
	ds18b20_write_byte(0xcc);			// ����ROM��������		 
	ds18b20_write_byte(0x44);			// �¶�ת������
	DELAY_MS(100);
}

/* ���Ͷ�ȡ���¶����� */
void ds18b20_read_temp_com()
{
	ds18b20_wait();
	DELAY_MS(1);
	ds18b20_write_byte(0xcc);			// ����ROM��������		 
	ds18b20_write_byte(0xbe);			// ���Ͷ�ȡ�¶�����
}

/* ��ȡ�¶� */
float ds18b20_read_temp()
{
	int data;
	float temp;
	uint8 tmh, tml;
	ds18b20_change_temp();				// д��ת������
	ds18b20_read_temp_com();			// �ȴ�ת������Ͷ�ȡ�¶�����
	tml = ds18b20_read_byte();			// ��ȡ�¶ȵ� 8 λ
	tmh = ds18b20_read_byte();			// ��ȡ�¶ȸ� 8 λ
	data = tmh;
	data <<= 8;
	data |= tml;
	data = (int)(data * 0.625 + 0.5);
	temp = data / 10;
	return temp;
}















