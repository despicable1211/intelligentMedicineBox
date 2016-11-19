#ifndef _COM_DATA_H_
#define _COM_DATA_H_

#include "include.h"
#include "ds1302.h"
#include "dht11.h"

#define MAX_MEIDCINE_COUNT	99

typedef enum
{
	CURRENT_TIME,
	PRE_TIME
} TimeType;

typedef struct {
	unsigned char meidcal_1_miunte;
	unsigned char meidcal_1_hour;
	unsigned char meidcal_2_miunte;
	unsigned char meidcal_2_hour;
	unsigned char meidcal_3_miunte;
	unsigned char meidcal_3_hour;
	unsigned char meidcal_4_miunte;
	unsigned char meidcal_4_hour;
} MeidcalTime;

typedef struct {
	float preTemp;
	float preHum;
	float currentTemp;
	float currentHum;
} TempAndHum;

typedef enum
{
	SYS_SECOND,
	SYS_MINUTE,
	SYS_HOUR,
	SYS_DAY,
	SYS_MONTH,
	SYS_YEAR,
	SYS_WEEK,
	
	EAT_1_MINUTE,
	EAT_1_HOUR,
	EAT_2_MINUTE,
	EAT_2_HOUR,
	EAT_3_MINUTE,
	EAT_3_HOUR,
	EAT_4_MINUTE,
	EAT_4_HOUR,
	
	CURRENT_MEIDCAL_COUNT,
	ALL_MEIDCAL_COUNT
} SettingTimeCode;

typedef enum
{
	ADD,
	SUB
} ChangeType;

void data_init();
void sortMeidcalTime();
void saveSystemTime();
uint16 *getMeidcalTimeArr();
MeidcalTime getMeidcalTime();
void setMeidcalTime(MeidcalTime _meidcalTime);
Time getTime(TimeType timeType);
void setTime(TimeType timeType, Time time);
Time undateCurrentTime();
TempAndHum undateTempAndHum();
uint8 getTimeValue(SettingTimeCode settingTimeCode);
uint8 changeTimeData(SettingTimeCode settingTimeCode, ChangeType type);
void setMeidcalMeun(uint8 *meidcalMeun);
uint8 *getMeidcalMeun();
bool isMeidcalTime();
bool isRemindTime();
bool isMeidcalNotEat();
void setPlayFlag(bool _playFlag);
bool getPlayFlag();
void setMeidcineCount(uint8 count);

#endif