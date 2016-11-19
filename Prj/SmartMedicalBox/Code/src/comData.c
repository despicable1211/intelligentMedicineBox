#include "comData.h"

static Time currentTime;
static Time preTime;
static TempAndHum tempAndHum;

static MeidcalTime meidcalTime;
static uint16 meidcalTimeArr[4];

static uint8 meidcalMeun[100] = "XXXXXXXX";

static bool playFlag;
static uint8 allMeidcineCount, currentMeidcineCount;

void data_init()
{
	playFlag = false;
	
	meidcalTime.meidcal_1_miunte = 0;
	meidcalTime.meidcal_1_hour = 24;
	meidcalTime.meidcal_2_miunte = 0;
	meidcalTime.meidcal_2_hour = 24;
	meidcalTime.meidcal_3_miunte = 0;
	meidcalTime.meidcal_3_hour = 24;
	meidcalTime.meidcal_4_miunte = 0;
	meidcalTime.meidcal_4_hour = 24;
		
	sortMeidcalTime();	
	undateCurrentTime();
}

void sortMeidcalTime()
{
	uint8 i, j;
	uint16 tmp;
	
	meidcalTimeArr[0] = (meidcalTime.meidcal_1_hour * 100) + meidcalTime.meidcal_1_miunte;
	meidcalTimeArr[1] = (meidcalTime.meidcal_2_hour * 100) + meidcalTime.meidcal_2_miunte;
	meidcalTimeArr[2] = (meidcalTime.meidcal_3_hour * 100) + meidcalTime.meidcal_3_miunte;
	meidcalTimeArr[3] = (meidcalTime.meidcal_4_hour * 100) + meidcalTime.meidcal_4_miunte;
	
	for(i = 0; i < 4; i++)
	{
		for(j = 3; j > i; j--)
		{
			if(meidcalTimeArr[j] < meidcalTimeArr[j-1])
			{
				tmp = meidcalTimeArr[j];
				meidcalTimeArr[j] = meidcalTimeArr[j-1];
				meidcalTimeArr[j-1] = tmp;
			}
		}
	}
}

void saveSystemTime()
{
	ds1302_set_time(currentTime);
	undateCurrentTime();
}

uint16 *getMeidcalTimeArr()
{
	return meidcalTimeArr;
}

MeidcalTime getMeidcalTime()
{
	return meidcalTime;
}

void setMeidcalTime(MeidcalTime _meidcalTime)
{
	meidcalTime.meidcal_1_hour = _meidcalTime.meidcal_1_hour;
	meidcalTime.meidcal_1_miunte = _meidcalTime.meidcal_1_miunte;
	meidcalTime.meidcal_2_hour = _meidcalTime.meidcal_2_hour;
	meidcalTime.meidcal_2_miunte = _meidcalTime.meidcal_2_miunte;
	meidcalTime.meidcal_3_hour = _meidcalTime.meidcal_3_hour;
	meidcalTime.meidcal_3_miunte = _meidcalTime.meidcal_3_miunte;
	meidcalTime.meidcal_4_hour = _meidcalTime.meidcal_4_hour;
	meidcalTime.meidcal_4_miunte = _meidcalTime.meidcal_4_miunte;
}

Time getTime(TimeType timeType)
{
	if(timeType == CURRENT_TIME)
		return currentTime;
	else //if(timeType == PRE_TIME)
		return preTime;
}

void setTime(TimeType timeType, Time time)
{
	if(timeType == CURRENT_TIME)
	{
		currentTime.year = time.year;
		currentTime.month = time.month;
		currentTime.day = time.day;
		currentTime.week = time.week;
		currentTime.hour = time.hour;
		currentTime.minute = time.minute;
		currentTime.second = time.second;
	}
	else //if(timeType == PRE_TIME)
	{
		preTime.year = time.year;
		preTime.month = time.month;
		preTime.day = time.day;
		preTime.week = time.week;
		preTime.hour = time.hour;
		preTime.minute = time.minute;
		preTime.second = time.second;
	}
}

Time undateCurrentTime()
{
	setTime(PRE_TIME, currentTime);
	setTime(CURRENT_TIME, ds1302_get_time());
	return currentTime;
}

uint8 getTimeValue(SettingTimeCode settingTimeCode)
{
	switch(settingTimeCode)
	{
	case SYS_SECOND:
		return currentTime.second;
	case SYS_MINUTE:
		return currentTime.minute;
	case SYS_HOUR:
		return currentTime.hour;
	case SYS_DAY:
		return currentTime.day;
	case SYS_MONTH:
		return currentTime.month;
	case SYS_YEAR:
		return currentTime.year;
	case SYS_WEEK:
		return currentTime.week;
		
	case EAT_1_MINUTE:
		return meidcalTime.meidcal_1_miunte;
	case EAT_1_HOUR:
		return meidcalTime.meidcal_1_hour;
	case EAT_2_MINUTE:
		return meidcalTime.meidcal_2_miunte;
	case EAT_2_HOUR:
		return meidcalTime.meidcal_2_hour;
	case EAT_3_MINUTE:
		return meidcalTime.meidcal_3_miunte;
	case EAT_3_HOUR:
		return meidcalTime.meidcal_3_hour;
	case EAT_4_MINUTE:
		return meidcalTime.meidcal_4_miunte;
	case EAT_4_HOUR:
		return meidcalTime.meidcal_4_hour;
		
	case CURRENT_MEIDCAL_COUNT:
		return currentMeidcineCount;
	case ALL_MEIDCAL_COUNT:
		return allMeidcineCount;
		
	default:
		return 255;
	}
}

uint8 changeTimeData(SettingTimeCode settingTimeCode, ChangeType type)
{
	switch(settingTimeCode)
	{
	case SYS_SECOND:
		currentTime.second = (type == SUB) 
		? (currentTime.second <= 0 ? 59 : currentTime.second-1) 
		: (currentTime.second >= 59 ? 0 : currentTime.second+1);
		return currentTime.second;
	case SYS_MINUTE:
		currentTime.minute = (type == SUB) 
		? (currentTime.minute <= 0 ? 59 : currentTime.minute-1) 
		: (currentTime.minute >= 59 ? 0 : currentTime.minute+1);
		return currentTime.minute;
	case SYS_HOUR:
		currentTime.hour = (type == SUB) 
		? (currentTime.hour <= 0 ? 23 : currentTime.hour-1) 
		: (currentTime.hour >= 23 ? 0 : currentTime.hour+1);
		return currentTime.hour;
	case SYS_DAY:
		if(currentTime.month == 4 || currentTime.month == 6 || currentTime.month == 9 || currentTime.month == 11)
			currentTime.day = (type == SUB) 
			? (currentTime.day <= 1 ? 30 : currentTime.day-1) 
			: (currentTime.day >= 30 ? 1 : currentTime.day+1);
		else if(currentTime.month != 2)
			currentTime.day = (type == SUB) 
			? (currentTime.day <= 1 ? 31 : currentTime.day-1) 
			: (currentTime.day >= 31 ? 1 : currentTime.day+1);
		else if(currentTime.year % 4 == 0)
			currentTime.day = (type == SUB) 
			? (currentTime.day <= 1 ? 29 : currentTime.day-1) 
			: (currentTime.day >= 29 ? 1 : currentTime.day+1);
		else
			currentTime.day = (type == SUB) 
			? (currentTime.day <= 1 ? 28 : currentTime.day-1) 
			: (currentTime.day >= 28 ? 1 : currentTime.day+1);
		return currentTime.day;
	case SYS_MONTH:
		currentTime.month = (type == SUB) 
		? (currentTime.month <= 1 ? 12 : currentTime.month-1) 
		: (currentTime.month >= 12 ? 1 : currentTime.month+1);
		return currentTime.month;
	case SYS_YEAR:
		currentTime.year = (type == SUB) 
		? (currentTime.year <= 0 ? 99 : currentTime.year-1) 
		: (currentTime.year >= 99 ? 0 : currentTime.year+1);
		return currentTime.year;
	case SYS_WEEK:
		currentTime.week = (type == SUB) 
		? (currentTime.week <= 1 ? 7 : currentTime.week-1) 
		: (currentTime.week >= 7 ? 1 : currentTime.week+1);
		return currentTime.week;
		
	case EAT_1_MINUTE:
		meidcalTime.meidcal_1_miunte = (type == SUB) 
		? (meidcalTime.meidcal_1_miunte <= 0 ? 59 : meidcalTime.meidcal_1_miunte-1)
		: (meidcalTime.meidcal_1_miunte >= 59 ? 0 : meidcalTime.meidcal_1_miunte+1);
		return meidcalTime.meidcal_1_miunte;
	case EAT_1_HOUR:
		meidcalTime.meidcal_1_hour = (type == SUB) 
		? (meidcalTime.meidcal_1_hour <= 0 ? 24 : meidcalTime.meidcal_1_hour-1)
		: (meidcalTime.meidcal_1_hour >= 24 ? 0 : meidcalTime.meidcal_1_hour+1);
		return meidcalTime.meidcal_1_hour;
	case EAT_2_MINUTE:
		meidcalTime.meidcal_2_miunte = (type == SUB) 
		? (meidcalTime.meidcal_2_miunte <= 0 ? 59 : meidcalTime.meidcal_2_miunte-1)
		: (meidcalTime.meidcal_2_miunte >= 59 ? 0 : meidcalTime.meidcal_2_miunte+1);
		return meidcalTime.meidcal_2_miunte;
	case EAT_2_HOUR:
		meidcalTime.meidcal_2_hour = (type == SUB) 
		? (meidcalTime.meidcal_2_hour <= 0 ? 24 : meidcalTime.meidcal_2_hour-1)
		: (meidcalTime.meidcal_2_hour >= 24 ? 0 : meidcalTime.meidcal_2_hour+1);
		return meidcalTime.meidcal_2_hour;
	case EAT_3_MINUTE:
		meidcalTime.meidcal_3_miunte = (type == SUB) 
		? (meidcalTime.meidcal_3_miunte <= 0 ? 59 : meidcalTime.meidcal_3_miunte-1)
		: (meidcalTime.meidcal_3_miunte >= 59 ? 0 : meidcalTime.meidcal_3_miunte+1);
		return meidcalTime.meidcal_3_miunte;
	case EAT_3_HOUR:
		meidcalTime.meidcal_3_hour = (type == SUB) 
		? (meidcalTime.meidcal_3_hour <= 0 ? 24 : meidcalTime.meidcal_3_hour-1)
		: (meidcalTime.meidcal_3_hour >= 24 ? 0 : meidcalTime.meidcal_3_hour+1);
		return meidcalTime.meidcal_3_hour;
	case EAT_4_MINUTE:
		meidcalTime.meidcal_4_miunte = (type == SUB) 
		? (meidcalTime.meidcal_4_miunte <= 0 ? 59 : meidcalTime.meidcal_4_miunte-1)
		: (meidcalTime.meidcal_4_miunte >= 59 ? 0 : meidcalTime.meidcal_4_miunte+1);
		return meidcalTime.meidcal_4_miunte;
	case EAT_4_HOUR:
		meidcalTime.meidcal_4_hour = (type == SUB) 
		? (meidcalTime.meidcal_4_hour <= 0 ? 24 : meidcalTime.meidcal_4_hour-1)
		: (meidcalTime.meidcal_4_hour >= 24 ? 0 : meidcalTime.meidcal_4_hour+1);
		return meidcalTime.meidcal_4_hour;
		
	case ALL_MEIDCAL_COUNT:
		allMeidcineCount = (type == SUB) 
		? (allMeidcineCount > 0 ? allMeidcineCount - 1 : 0)
		: (allMeidcineCount < MAX_MEIDCINE_COUNT ? allMeidcineCount + 1 : MAX_MEIDCINE_COUNT);
		return allMeidcineCount;
		
	case CURRENT_MEIDCAL_COUNT:
		currentMeidcineCount = (type == SUB) 
		? (currentMeidcineCount > 0 ? currentMeidcineCount - 1 : 0)
		: (currentMeidcineCount < MAX_MEIDCINE_COUNT ? currentMeidcineCount + 1 : MAX_MEIDCINE_COUNT);
		return currentMeidcineCount;		
		
	default:
		return 255;
	}
}

TempAndHum undateTempAndHum()
{
	Dht11Data dht11Data = dth11_read_data();
	tempAndHum.preHum = tempAndHum.currentHum;
	tempAndHum.preTemp = tempAndHum.currentTemp;
	tempAndHum.currentHum = dht11Data.humidity;
	tempAndHum.currentTemp = dht11Data.temperature;
	return tempAndHum;
}

void setMeidcalMeun(uint8 *meun)
{
	uint8 tmp;
	if(sizeof(meun) < sizeof(meidcalMeun))
		for(tmp = 0; tmp < sizeof(meun); tmp++)
			meidcalMeun[tmp] = meun[tmp];
	else
		for(tmp = 0; tmp < sizeof(meidcalMeun)-1; tmp++)
			meidcalMeun[tmp] = meun[tmp];
	meidcalMeun[tmp] = '\0';
}

uint8 *getMeidcalMeun()
{
	return meidcalMeun;
}

bool isMeidcalTime()
{
	uint16 currintTimeInt = currentTime.hour*100+currentTime.minute;
	if(currintTimeInt == meidcalTimeArr[0]
	|| currintTimeInt == meidcalTimeArr[1]
	|| currintTimeInt == meidcalTimeArr[2]
	|| currintTimeInt == meidcalTimeArr[3])
		return true;
	else
		return false;
}

bool isRemindTime()
{
	if(isMeidcalTime() && (preTime.minute != currentTime.minute || preTime.hour != currentTime.hour))
		return true;
	else
		return false;
}

bool isMeidcalNotEat()
{
	uint16 preTimeInt = preTime.hour*100+preTime.minute;
	
	if(preTimeInt != meidcalTimeArr[0]
	&& preTimeInt != meidcalTimeArr[1]
	&& preTimeInt != meidcalTimeArr[2]
	&& preTimeInt != meidcalTimeArr[3])
		return false;
	if(!isMeidcalTime() && playFlag)
		return true;
	
	return false;
}

void setPlayFlag(bool _playFlag)
{
	playFlag = _playFlag;
}

bool getPlayFlag()
{
	return playFlag;
}

void setMeidcineCount(uint8 count)
{
	if(count > MAX_MEIDCINE_COUNT)
		count = MAX_MEIDCINE_COUNT;
	allMeidcineCount = count;
	currentMeidcineCount = allMeidcineCount;
}








