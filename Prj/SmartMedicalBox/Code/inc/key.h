#ifndef _KEY_H_
#define _KEY_H_

#include "include.h"
#include "comData.h"
#include "tft.h"
#include "mpu90615.h"
#include "pulsesensor.h"
#include "receiveMedicineMenu.h"

void key_event_init();			// 按键初始化
void key_start_scan();			// 开始按键扫描
void key_stop_scan();			// 停止按键扫描

TftPage timeUpdateControl();
void setTftPage(TftPage page);

#endif