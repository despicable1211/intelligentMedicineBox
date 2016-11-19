#ifndef _KEY_H_
#define _KEY_H_

#include "include.h"
#include "comData.h"
#include "tft.h"
#include "mpu90615.h"
#include "pulsesensor.h"
#include "receiveMedicineMenu.h"

void key_event_init();			// ������ʼ��
void key_start_scan();			// ��ʼ����ɨ��
void key_stop_scan();			// ֹͣ����ɨ��

TftPage timeUpdateControl();
void setTftPage(TftPage page);

#endif