#include "receiveMedicineMenu.h"

static char receiveStr[STR_MAX_LENTH];
static uint16 receivePointer;
static cJSON *json, *json_value;

void receiveFinish()
{
	Time systemTime;
	MeidcalTime meidcalTime;
		
	uart_rx_irq_dis(BT_UART);
		
	if(receiveStr[0] == 'X'
	&& receiveStr[1] == '@'
	&& receiveStr[receivePointer] == '\0'
	&& receiveStr[receivePointer-2] == '@'
	&& receiveStr[receivePointer-1] == 'X')
	{
		receiveStr[receivePointer-2] = '\0';
		json = cJSON_Parse(&receiveStr[2]);
		if(!json)
			printf("Error before:[%s]\n", cJSON_GetErrorPtr());
		else
		{		
			//printf("%s\n", &receiveStr[2]);
			
			json_value = cJSON_GetObjectItem(json, SYSTEM_YEAR);
			if(json_value->type == cJSON_Number)
				systemTime.year = json_value->valueint%100;			
			json_value = cJSON_GetObjectItem(json, SYSTEM_MONTH);
			if(json_value->type == cJSON_Number)
				systemTime.month = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, SYSTEM_DAY);
			if(json_value->type == cJSON_Number)
				systemTime.day = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, SYSTEM_WEEK);
			if(json_value->type == cJSON_Number)
				systemTime.week = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, SYSTEM_HOUR);
			if(json_value->type == cJSON_Number)
				systemTime.hour = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, SYSTEM_MINUTE);
			if(json_value->type == cJSON_Number)
				systemTime.minute = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, SYSTEM_SECOND);
			if(json_value->type == cJSON_Number)
				systemTime.second = json_value->valueint;
			setTime(CURRENT_TIME, systemTime);
			saveSystemTime();
			
			json_value = cJSON_GetObjectItem(json, MEDICINE_HOUR_01);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_1_hour = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, MEDICINE_MINUTE_01);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_1_miunte = json_value->valueint;
			json_value = cJSON_GetObjectItem(json, MEDICINE_HOUR_02);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_2_hour = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, MEDICINE_MINUTE_02);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_2_miunte = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, MEDICINE_HOUR_03);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_3_hour = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, MEDICINE_MINUTE_03);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_3_miunte = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, MEDICINE_HOUR_04);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_4_hour = json_value->valueint;			
			json_value = cJSON_GetObjectItem(json, MEDICINE_MINUTE_04);
			if(json_value->type == cJSON_Number)
				meidcalTime.meidcal_4_miunte = json_value->valueint;
			setMeidcalTime(meidcalTime);
			sortMeidcalTime();
			
			json_value = cJSON_GetObjectItem(json, MEDICINE_COUNT);
			if(json_value->type == cJSON_Number)
				setMeidcineCount(json_value->valueint);	
			
			json_value = cJSON_GetObjectItem(json, MEDICINE_MENU);
			if(json_value->type == cJSON_String)
				setMeidcalMeun((uint8 *)json_value->valuestring);
			
			cJSON_Delete(json);
			
			// 发送回馈信息
			uart_putstr(BT_UART, "X@OK@X");
			// 数据更新成功提示音
			play_music(BT_SOUND);
		}
	}
	receiveStr[0] = 0;
	receiveStr[1] = 0;
	receivePointer = 0;
	
	uart_rx_irq_en(BT_UART);
}

void bt_handler()
{
	char ch;
	if(UART_S1_REG(UARTN[BT_UART]) & UART_S1_RDRF_MASK)
	{
		uart_getchar(BT_UART, &ch);
		
		if(receivePointer == 0 && ch == 'X' && receiveStr[0] != 'X')
			receiveStr[receivePointer++] = ch;
		else if(receivePointer == 1 && ch == '@' && receiveStr[0] == 'X')
			receiveStr[receivePointer++] = ch;
		else if(receivePointer == 1 && (ch != '@' || receiveStr[0] != 'X'))
		{
			receivePointer = 0;
			receiveStr[receivePointer] = 0;	
		}
		else if(receivePointer >= 2 && receivePointer < STR_MAX_LENTH-2 && ch != 'X')
		   receiveStr[receivePointer++] = ch;
		else if(receivePointer >= 2 && receivePointer < STR_MAX_LENTH-2 && ch == 'X')
		{
			if(receiveStr[receivePointer-1] == '@')
			{
				receiveStr[receivePointer++] = ch;
				receiveStr[receivePointer] = '\0';
				receiveFinish();
			}
			else
				receiveStr[receivePointer++] = ch;
		}
		else if(receivePointer == STR_MAX_LENTH-2)
		{
			receiveStr[receivePointer++] = ch;
			receiveStr[receivePointer] = '\0';
			receiveFinish();
		}
		else;
	}
}

void receive_init()
{
	receivePointer = 0;
	uart_init(BT_UART,115200);
	set_vector_handler(BT_VECTORn, bt_handler);
}

void bt_start()
{
	uart_rx_irq_en(BT_UART);
}

void bt_stop()
{
	uart_rx_irq_dis(BT_UART);
}