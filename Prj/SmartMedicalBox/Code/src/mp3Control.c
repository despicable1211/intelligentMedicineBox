#include "mp3Control.h"

void mp3_init()
{
	uart_init(MP3_UART, 9600);
}

bool play_music(uint8 index)
{
	char ch;
	uint8 n, playCom[] = {0x7E, 0x04, 0x41, 0x00, index, 0xEF};
	
	for(n = 0; n < sizeof(playCom); n++)
		uart_putchar(MP3_UART, playCom[n]);
	
	if(uart_querychar(MP3_UART, &ch) != 0)
		if(ch != 'O')
			return false;
	if(uart_querychar(MP3_UART, &ch) != 0)
		if(ch != 'K')
			return false;
			
	return true;
}

bool stop_music()
{
	char ch;
	uint8 n, playCom[] = {0x7E, 0x02, 0x0E, 0xEF};
	
	for(n = 0; n < sizeof(playCom); n++)
		uart_putchar (MP3_UART, playCom[n]);
	
	if(uart_querychar(MP3_UART, &ch) != 0)
		if(ch != 'O')
			return false;
	if(uart_querychar(MP3_UART, &ch) != 0)
		if(ch != 'K')
			return false;
	
	return true;
}


PlayState getPlayState()
{
	char str[15];
	uint8 n,  playCom[] = {0x7E, 0x02, 0x10, 0xEF};
	
	for(n = 0; n < sizeof(playCom); n++)
		uart_putchar (MP3_UART, playCom[n]);
	
	if(uart_querystr(MP3_UART, str, sizeof(str)-1) != 0)
	{
		for(n = 0; n < sizeof(str); n++)
			if(str[n] == '\0')
				break;
		
		if(str[n-8] == '0'
		&& str[n-7] == '0'
		&& str[n-6] == '0'
		&& str[n-4] == 13
		&& str[n-3] == 10
		&& str[n-2] == 'O'
		&& str[n-1] == 'K')
		{
			if(str[n-5] < '0' && (str[n-5]-'0') > UNKNOW)
				return UNKNOW;
			else
				return (PlayState)(str[n-5]-'0');
		}
	}
	
	return UNKNOW;
}











