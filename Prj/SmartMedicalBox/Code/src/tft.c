#include "tft.h"
#include <stdio.h>

static TftPage currentPage;

uint8 *floatToStr(float value);
uint8 *intToStr(int value);

void tft_init()
{
	currentPage = PAGE_NORMAL;
	TFT_Init();
	switch_page(currentPage);
}

void switch_page(TftPage tftPage)
{	
	switch(tftPage)
	{
	/* 常规页面 */
	case PAGE_NORMAL:
		TFT_ClearScreen(WHITE);
		GUI_Write32CnChar(10, 15, "当前时间", BLACK, WHITE);
		GUI_WriteASCII(40, 55, "20  -  -  ", BLACK, WHITE);
		GUI_WriteASCII(20, 90, "  :  :  ", BLACK, WHITE);
		GUI_Write14CnChar(180, 90, "周", BLACK, WHITE);
		
		GUI_Write14CnChar(10, 140, "空气质量：", BLACK, WHITE);
		showAirLevel(PAGE_NORMAL, false);
		GUI_Write14CnChar(10, 170, "温度：", BLACK, WHITE);
		GUI_WriteASCII(77, 170, "  ", BLACK, WHITE);			
		GUI_Write14CnChar(128, 170, "湿度：", BLACK, WHITE);
		GUI_WriteASCII(195, 170, "  ", BLACK, WHITE);
		showHumAndTemp(PAGE_NORMAL, false);

		showSystemTime(PAGE_NORMAL, false);
		GUI_Line(0, 205, 239, 205, BLACK);		
		
		GUI_Write32CnChar(10, 215, "下次吃药时间", BLACK, WHITE);		
		GUI_WriteASCII(80, 255, "  :  ", BLACK, WHITE);
		showMeidcalTime(PAGE_NORMAL);
		GUI_Line(0, 295, 239, 295, BLACK);
		
		GUI_Write32CnChar(10, 305, "药单", BLACK, WHITE);
		showMeidcalCount(PAGE_NORMAL);
		GUI_WriteASCII(30, 355, getMeidcalMeun(), BLACK, WHITE);
		break;
		
	/* 设置页面 */
	case PAGE_SETTING:
		TFT_ClearScreen(BLACK);
		GUI_Write32CnChar(60, 10, "智能药箱", WHITE, BLACK);
		GUI_Line(0, 55, 239, 55, WHITE);
		GUI_Line(0, 175, 239, 175, WHITE);
		GUI_Line(0, 355, 239, 355, WHITE);
		showSetText(SETTING_NORMAL, SELECT_MODE);
		
		GUI_WriteASCII(40, 105, "20  -  -  ", WHITE, BLACK);
		GUI_WriteASCII(20, 140, "  :  :  ", WHITE, BLACK);
		GUI_Write14CnChar(180, 140, "周", WHITE, BLACK);
		
		GUI_WriteASCII(80, 225, "  :  ", WHITE, BLACK);
		GUI_WriteASCII(80, 255, "  :  ", WHITE, BLACK);
		GUI_WriteASCII(80, 285, "  :  ", WHITE, BLACK);
		GUI_WriteASCII(80, 315, "  :  ", WHITE, BLACK);
		
		showSystemTime(PAGE_SETTING, false);		
		showMeidcalTime(PAGE_SETTING);
		showMeidcalCount(PAGE_SETTING);
		
		break;
		
	/* 提醒页面 */
	case PAGE_REMIND:
		TFT_ClearScreen(RED);
		GUI_Write32CnChar(38, 50, "吃药时间到", WHITE, RED);
		GUI_Line(35, 85, 195, 85, WHITE);
		GUI_Write32CnChar(10, 150, "药单：", WHITE, RED);
		GUI_WriteASCII(30, 200, getMeidcalMeun(), WHITE, RED);
		break;
		
	/* 蓝牙接收页面 */
	case PAGE_BLUETOOTH:
		TFT_ClearScreen(BLUE);
		GUI_Write32CnChar(25, 150, "手机数据同步", WHITE, BLUE);
		GUI_Line(25, 188, 217, 188, WHITE);
		break;
		
	/* 体检页面 */
	case PAGE_HEALTH_EXAM:
		TFT_ClearScreen(GREEN);
		GUI_Write32CnChar(60, 40, "健康检测", RED, GREEN);
		GUI_Line(60, 73, 188, 73, RED);	
		GUI_Write14CnChar(40, 120, "心率：", RED, GREEN);
		GUI_WriteASCII(112, 120, "   ", RED, GREEN);
		GUI_Write14CnChar(160, 120, "次", RED, GREEN);
		GUI_WriteASCII(176, 120, "/", RED, GREEN);
		GUI_Write14CnChar(192, 120, "分", RED, GREEN);
		
		GUI_Write14CnChar(40, 220, "体温：", RED, GREEN);
		GUI_WriteASCII(112, 220, "  .  ", RED, GREEN);
		GUI_Write14CnChar(192, 220, "°", RED, GREEN);
		
		GUI_Write14CnChar(40, 320, "室温：", RED, GREEN);
		GUI_WriteASCII(112, 320, "  .  ", RED, GREEN);
		GUI_Write14CnChar(192, 320, "°", RED, GREEN);
		break;
		
	default:
		return;
	}
}

void showSystemTime(TftPage tftPage, bool compareTime)
{
	Time currentTime = getTime(CURRENT_TIME);
	Time preTime = getTime(PRE_TIME);
	
	/* 常规页面 */
	if(tftPage == PAGE_NORMAL)
	{
		if(compareTime)
		{
			if(currentTime.year != preTime.year)
				GUI_WriteASCII(72, 55, intToStr(currentTime.year), BLACK, WHITE);
			if(currentTime.month != preTime.month)
				GUI_WriteASCII(120, 55, intToStr(currentTime.month), BLACK, WHITE);
			if(currentTime.day != preTime.day)
				GUI_WriteASCII(168, 55, intToStr(currentTime.day), BLACK, WHITE);
			if(currentTime.hour != preTime.hour)
				GUI_WriteASCII(20, 90, intToStr(currentTime.hour), BLACK, WHITE);
			if(currentTime.minute != preTime.minute)
				GUI_WriteASCII(68, 90, intToStr(currentTime.minute), BLACK, WHITE);
			if(currentTime.second != preTime.second)
				GUI_WriteASCII(116, 90, intToStr(currentTime.second), BLACK, WHITE);
			if(currentTime.week != preTime.week)
				switch(currentTime.week)
				{
					case 1:	GUI_Write14CnChar(204, 90, "一", BLACK, WHITE);	break;
					case 2:	GUI_Write14CnChar(204, 90, "二", BLACK, WHITE);	break;
					case 3:	GUI_Write14CnChar(204, 90, "三", BLACK, WHITE);	break;
					case 4:	GUI_Write14CnChar(204, 90, "四", BLACK, WHITE);	break;
					case 5:	GUI_Write14CnChar(204, 90, "五", BLACK, WHITE);	break;
					case 6:	GUI_Write14CnChar(204, 90, "六", BLACK, WHITE);	break;
					case 7:	GUI_Write14CnChar(204, 90, "日", BLACK, WHITE);	break;
					default:	break;
				}
		}
		else
		{
			GUI_WriteASCII(72, 55, intToStr(currentTime.year), BLACK, WHITE);
			GUI_WriteASCII(120, 55, intToStr(currentTime.month), BLACK, WHITE);
			GUI_WriteASCII(168, 55, intToStr(currentTime.day), BLACK, WHITE);
			GUI_WriteASCII(20, 90, intToStr(currentTime.hour), BLACK, WHITE);
			GUI_WriteASCII(68, 90, intToStr(currentTime.minute), BLACK, WHITE);
			GUI_WriteASCII(116, 90, intToStr(currentTime.second), BLACK, WHITE);
			switch(currentTime.week)
			{
				case 1:	GUI_Write14CnChar(204, 90, "一", BLACK, WHITE);	break;
				case 2:	GUI_Write14CnChar(204, 90, "二", BLACK, WHITE);	break;
				case 3:	GUI_Write14CnChar(204, 90, "三", BLACK, WHITE);	break;
				case 4:	GUI_Write14CnChar(204, 90, "四", BLACK, WHITE);	break;
				case 5:	GUI_Write14CnChar(204, 90, "五", BLACK, WHITE);	break;
				case 6:	GUI_Write14CnChar(204, 90, "六", BLACK, WHITE);	break;
				case 7:	GUI_Write14CnChar(204, 90, "日", BLACK, WHITE);	break;
				default:	break;
			}
		
		}
	}	
	/* 设置页面 */
	if(tftPage == PAGE_SETTING)
	{
		GUI_WriteASCII(72, 105, intToStr(currentTime.year), WHITE, BLACK);
		GUI_WriteASCII(120, 105, intToStr(currentTime.month), WHITE, BLACK);	
		GUI_WriteASCII(168, 105, intToStr(currentTime.day), WHITE, BLACK);
		GUI_WriteASCII(20, 140, intToStr(currentTime.hour), WHITE, BLACK);
		GUI_WriteASCII(68, 140, intToStr(currentTime.minute), WHITE, BLACK);
		GUI_WriteASCII(116, 140, intToStr(currentTime.second), WHITE, BLACK);
		switch(currentTime.week)
		{
			case 1:	GUI_Write14CnChar(204, 140, "一", WHITE, BLACK);	break;
			case 2:	GUI_Write14CnChar(204, 140, "二", WHITE, BLACK);	break;
			case 3:	GUI_Write14CnChar(204, 140, "三", WHITE, BLACK);	break;
			case 4:	GUI_Write14CnChar(204, 140, "四", WHITE, BLACK);	break;
			case 5:	GUI_Write14CnChar(204, 140, "五", WHITE, BLACK);	break;
			case 6:	GUI_Write14CnChar(204, 140, "六", WHITE, BLACK);	break;
			case 7:	GUI_Write14CnChar(204, 140, "日", WHITE, BLACK);	break;
			default:	break;
		}
	}
}

void showMeidcalTime(TftPage tftPage)
{
	uint8 tmp;
	uint16 currentTime;
	uint16 *meidcalTimeArr;
	MeidcalTime meidcalTime;
	
	/* 常规页面 */
	if(tftPage == PAGE_NORMAL)
	{
		currentTime = getTime(CURRENT_TIME).hour*100 + getTime(CURRENT_TIME).minute;
		meidcalTimeArr = getMeidcalTimeArr();
		for(tmp = 0; tmp < 4; tmp++)
			if(meidcalTimeArr[tmp] > currentTime && meidcalTimeArr[tmp] < 2400)
			{
				GUI_WriteASCII(80, 255, intToStr(meidcalTimeArr[tmp]/100), BLACK, WHITE);
				GUI_WriteASCII(128, 255, intToStr(meidcalTimeArr[tmp]%100), BLACK, WHITE);
				break;
			}
		if(tmp >= 4 && meidcalTimeArr[0] < 2400)
		{
			GUI_WriteASCII(80, 255, intToStr(meidcalTimeArr[0]/100), BLACK, WHITE);
			GUI_WriteASCII(128, 255, intToStr(meidcalTimeArr[0]%100), BLACK, WHITE);
		}
		if(tmp >= 4 && meidcalTimeArr[0] >= 2400)
			GUI_WriteASCII(80, 255, "--:--", BLACK, WHITE);
	}		
	/* 设置页面 */
	if(tftPage == PAGE_SETTING)
	{
		meidcalTime = getMeidcalTime();
		GUI_WriteASCII(80, 225, intToStr(meidcalTime.meidcal_1_hour), WHITE, BLACK);
		GUI_WriteASCII(128, 225, intToStr(meidcalTime.meidcal_1_miunte), WHITE, BLACK);
		GUI_WriteASCII(80, 255, intToStr(meidcalTime.meidcal_2_hour), WHITE, BLACK);
		GUI_WriteASCII(128, 255, intToStr(meidcalTime.meidcal_2_miunte), WHITE, BLACK);
		GUI_WriteASCII(80, 285, intToStr(meidcalTime.meidcal_3_hour), WHITE, BLACK);
		GUI_WriteASCII(128, 285, intToStr(meidcalTime.meidcal_3_miunte), WHITE, BLACK);
		GUI_WriteASCII(80, 315, intToStr(meidcalTime.meidcal_4_hour), WHITE, BLACK);
		GUI_WriteASCII(128, 315, intToStr(meidcalTime.meidcal_4_miunte), WHITE, BLACK);
	}
}

void showMeidcalCount(TftPage tftPage)
{
	if(tftPage == PAGE_NORMAL)
	{
		GUI_WriteASCII(120, 305, "(  /  )", BLACK, WHITE);	
		GUI_WriteASCII(136, 305, intToStr(getTimeValue(CURRENT_MEIDCAL_COUNT)), BLACK, WHITE);	
		GUI_WriteASCII(184, 305, intToStr(getTimeValue(ALL_MEIDCAL_COUNT)), BLACK, WHITE);	
	}
	if(tftPage == PAGE_SETTING)
		GUI_WriteASCII(180, 375, intToStr(getTimeValue(ALL_MEIDCAL_COUNT)), WHITE, BLACK);
}

void showSetText(SettingMode settingMode, SelectMode selectMode)
{
	switch(settingMode)
	{
	case SETTING_NORMAL:
		GUI_Write32CnChar(10, 65, "系统时间设定", WHITE, BLACK);		
		GUI_Write32CnChar(10, 185, "吃药时间设定", WHITE, BLACK);
		GUI_Write14CnChar(10, 380, "药量设置", WHITE, BLACK);
		GUI_Write14CnChar(215, 380, "份", WHITE, BLACK);
		break;
	case SETTING_TIME:
		GUI_Write32CnChar(10, 65, "系统时间设定", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK);		
		GUI_Write32CnChar(10, 185, "吃药时间设定", WHITE, BLACK);
		GUI_Write14CnChar(10, 380, "药量设置", WHITE, BLACK);
		GUI_Write14CnChar(215, 380, "份", WHITE, BLACK);
		break;
	case SETTING_MEIDCAL:
		GUI_Write32CnChar(10, 65, "系统时间设定", WHITE, BLACK);		
		GUI_Write32CnChar(10, 185, "吃药时间设定", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK);
		GUI_Write14CnChar(10, 380, "药量设置", WHITE, BLACK);
		GUI_Write14CnChar(215, 380, "份", WHITE, BLACK);
		break;
	case SETTING_MEIDCAL_COUNT:
		GUI_Write32CnChar(10, 65, "系统时间设定", WHITE, BLACK);		
		GUI_Write32CnChar(10, 185, "吃药时间设定", WHITE, BLACK);
		GUI_Write14CnChar(10, 380, "药量设置", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK);
		GUI_Write14CnChar(215, 380, "份", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK);
		break;
	default:
		break;
	}
}

void showTimeSetting(SelectMode selectMode, SettingTimeCode settingTimeCode, uint8 value)
{
	uint16 x, y;
	if(selectMode == SELECT_MODE)
	{
		showSystemTime(PAGE_SETTING, true);
		showMeidcalTime(PAGE_SETTING);
	}
	switch(settingTimeCode)
	{
	case SYS_SECOND:
		x = 116;
		y = 140;
		break;
	case SYS_MINUTE:
		x = 68;
		y = 140;
		break;
	case SYS_HOUR:
		x = 20;
		y = 140;
		break;	
	case SYS_DAY:
		x = 168;
		y = 105;
		break;
	case SYS_MONTH:
		x = 120;
		y = 105;
		break;
	case SYS_YEAR:
		x = 72;
		y = 105;
		break;
	case SYS_WEEK:
		switch(value)
		{
			case 1: GUI_Write14CnChar(204, 140, "一", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			case 2: GUI_Write14CnChar(204, 140, "二", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			case 3: GUI_Write14CnChar(204, 140, "三", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			case 4: GUI_Write14CnChar(204, 140, "四", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			case 5: GUI_Write14CnChar(204, 140, "五", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			case 6: GUI_Write14CnChar(204, 140, "六", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			case 7: GUI_Write14CnChar(204, 140, "日", (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK); break;
			default: break;
		}
		return;
	case EAT_1_MINUTE:
		x = 128;
		y = 225;
		break;
	case EAT_1_HOUR:
		x = 80;
		y = 225;
		break;
	case EAT_2_MINUTE:
		x = 128;
		y = 255;
		break;
	case EAT_2_HOUR:
		x = 80;
		y = 255;
		break;
	case EAT_3_MINUTE:
		x = 128;
		y = 285;
		break;
	case EAT_3_HOUR:
		x = 80;
		y = 285;
		break;
	case EAT_4_MINUTE:
		x = 128;
		y = 315;
		break;
	case EAT_4_HOUR:
		x = 80;
		y = 315;
		break;
	case ALL_MEIDCAL_COUNT:
		x = 180;
		y = 375;
	default:
		break;
	}
	GUI_WriteASCII(x, y, intToStr(value), (selectMode == SELECT_MODE) ? YELLOW : RED, BLACK);
}

void showHumAndTemp(TftPage tftPage, bool isCompare)
{		
	TempAndHum tempAndHum;
	if(tftPage == PAGE_NORMAL)
	{
		tempAndHum = undateTempAndHum();
		if(!isCompare || (isCompare && tempAndHum.preTemp != tempAndHum.currentTemp))
		{
			GUI_WriteASCII(76, 170, "  ", BLACK, WHITE);
			GUI_WriteASCII(76, 170, intToStr((int)tempAndHum.currentTemp), BLACK, WHITE);
		}
		if(!isCompare || (isCompare && tempAndHum.preHum != tempAndHum.currentHum))
		{
			GUI_WriteASCII(195, 170, "  ", BLACK, WHITE);
			GUI_WriteASCII(195, 170, intToStr((int)tempAndHum.currentHum), BLACK, WHITE);
		}
	}
}

void showHealthData(uint8 heartRate, Mpu90615Data mpu90615Data)
{
	GUI_WriteASCII(112, 120, "   ", RED, GREEN);
	GUI_WriteASCII(112, 120, intToStr(heartRate), RED, GREEN);
		
	GUI_WriteASCII(112, 220, "  ", RED, GREEN);
	GUI_WriteASCII(112, 220, intToStr(mpu90615Data.targetH), RED, GREEN);
	GUI_WriteASCII(160, 220, "  ", RED, GREEN);
	GUI_WriteASCII(160, 220, intToStr(mpu90615Data.targetL), RED, GREEN);
	
	GUI_WriteASCII(112, 320, "  ", RED, GREEN);
	GUI_WriteASCII(112, 320, intToStr(mpu90615Data.environmentH), RED, GREEN);
	GUI_WriteASCII(160, 320, "  ", RED, GREEN);
	GUI_WriteASCII(160, 320, intToStr(mpu90615Data.environmentL), RED, GREEN);

}

static AirLevel preAirLevel;
void showAirLevel(TftPage tftPage, bool isCompare)
{
	AirLevel currentAirLevel;
	if(tftPage == PAGE_NORMAL)
	{
		currentAirLevel = gp2y1010au_get_level();
		if(!isCompare || (isCompare && currentAirLevel != preAirLevel))
		{
			switch(currentAirLevel)
			{
			case AIR_LEVEL_1:
				GUI_Write14CnChar(130, 140, "优", BLACK, WHITE);
				break;
			case AIR_LEVEL_2:
				GUI_Write14CnChar(130, 140, "良", BLACK, WHITE);
				break;
			case AIR_LEVEL_3:
				GUI_Write14CnChar(130, 140, "中", BLACK, WHITE);
				break;
			case AIR_LEVEL_4:
				GUI_Write14CnChar(130, 140, "差", BLACK, WHITE);
				break;
			default:
				GUI_Write14CnChar(130, 140, "无", BLACK, WHITE);
				break;
			}
		}
	}
	preAirLevel = currentAirLevel;
}

uint8 *floatToStr(float value)
{
	static uint8 floatStr[15];
	uint8 tmp;
	sprintf((char *)floatStr, "%d", (int)(value*10));
	
	for(tmp = 0; tmp < 13; tmp++)
	{
		if(floatStr[tmp] == '\0')
		{
			if((tmp > 1 && floatStr[0] != '-') || (floatStr[0] == '-' && tmp > 2))
			{
				floatStr[tmp+1] = '\0';
				floatStr[tmp] = floatStr[tmp-1];
				floatStr[tmp-1] = '.';
			}
			else
			{
				floatStr[tmp+2] = '\0';
				floatStr[tmp+1] = floatStr[tmp-1];
				floatStr[tmp] = '.';
				floatStr[tmp-1] = '0';
			}
			break;
		}
	}
	return floatStr;
}

uint8 *intToStr(int value)
{
	static uint8 intStr[15];
	sprintf((char *)intStr, "%d", value);
	if(value == 0)
	{
		intStr[0] = '0';
		intStr[1] = '0';
		intStr[2] = '\0';
	}
	if(value < 10 && value > 0)
	{
		intStr[2] = intStr[1];
		intStr[1] = intStr[0];
		intStr[0] = '0';
	}
	return intStr;
}


