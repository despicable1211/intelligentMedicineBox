#ifndef _RECEIVE_MEDICINE_MENU_H_
#define _RECEIVE_MEDICINE_MENU_H_

#include "include.h"
#include "cJSON.h"
#include "comData.h"
#include "mp3Control.h"

#define STR_MAX_LENTH 		1024
#define BT_UART				UART3
#define BT_VECTORn			UART3_RX_TX_VECTORn

#define SYSTEM_YEAR			"systemYear"
#define SYSTEM_MONTH		"systemMonth"
#define SYSTEM_DAY			"systemDay"
#define SYSTEM_WEEK			"systemWeek"
#define SYSTEM_HOUR			"systemHour"
#define SYSTEM_MINUTE		"systemMinute"
#define SYSTEM_SECOND		"systemSecond"
#define MEDICINE_HOUR_01	"medicineHour01"
#define MEDICINE_MINUTE_01	"medicineMinute01"
#define MEDICINE_HOUR_02	"medicineHour02"
#define MEDICINE_MINUTE_02	"medicineMinute02"
#define MEDICINE_HOUR_03	"medicineHour03"
#define MEDICINE_MINUTE_03	"medicineMinute03"
#define MEDICINE_HOUR_04	"medicineHour04"
#define MEDICINE_MINUTE_04	"medicineMinute04"
#define MEDICINE_COUNT		"medicineCount"
#define MEDICINE_MENU		"medicineMenu"


void receive_init();
void bt_start();
void bt_stop();

#endif