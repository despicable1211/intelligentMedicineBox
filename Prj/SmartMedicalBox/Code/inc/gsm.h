#ifndef _GSM_H_
#define _GSM_H_

#include "include.h"

#define GSM_UART			UART1
#define GSM_PHONE_NUM		"15013950258"
#define GSM_MSG_TEXT		"���˳�ҩʱ�䣬�뾡���ҩ������"

void gms_init();
void gms_send_msg();

#endif