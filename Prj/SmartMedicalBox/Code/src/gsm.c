#include "gsm.h"

void gms_init()
{
	uart_init(GSM_UART, 115200);
}

void gms_send_msg()
{
	uart_putstr(GSM_UART, "AT+CSMP=17,167,0,8");
	uart_putchar(GSM_UART, 0x0D);
	DELAY_MS(100);
	
	uart_putstr(GSM_UART, "AT+CMGF=1");
	uart_putchar(GSM_UART, 0x0D);
	DELAY_MS(100);
	
	uart_putstr(GSM_UART, "AT+CMGS=");
	uart_putstr(GSM_UART, GSM_PHONE_NUM);
	uart_putchar(GSM_UART, 0x0D);
	DELAY_MS(100);
	
	uart_putstr(GSM_UART, GSM_MSG_TEXT);
	uart_putchar(GSM_UART, 0x1A);
}