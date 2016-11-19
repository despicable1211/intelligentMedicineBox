#ifndef _TFT_H_
#define _TFT_H_

#include "TFT_Include.h"
#include "comData.h"
#include "mpu90615.h"
#include "gp2y1010au.h"

typedef enum
{
	PAGE_NORMAL,
	PAGE_SETTING,
	PAGE_REMIND,
	PAGE_BLUETOOTH,
	PAGE_HEALTH_EXAM,
	PAGE_NULL
} TftPage;

typedef enum
{
	SETTING_NORMAL,
	SETTING_TIME,
	SETTING_MEIDCAL,
	SETTING_MEIDCAL_COUNT,
	SETTING_EXIT
} SettingMode;

typedef enum
{
	SELECT_MODE,
	SETTING_MODE
} SelectMode;

void tft_init();							// tft ≥ı ºªØ
void switch_page(TftPage tftPage);			// «–ªª tft œ‘ æ
void showSystemTime(TftPage tftPage, bool compareTime);
void showMeidcalTime(TftPage tftPage);
void showSetText(SettingMode settingMode, SelectMode selectMode);
void showTimeSetting(SelectMode selectMode, SettingTimeCode settingTimeCode, uint8 value);
void showHumAndTemp(TftPage tftPage, bool isCompare);
void showMeidcalCount(TftPage tftPage);
void showHealthData(uint8 heartRate, Mpu90615Data mpu90615Data);
void showAirLevel(TftPage tftPage, bool isCompare);

#endif