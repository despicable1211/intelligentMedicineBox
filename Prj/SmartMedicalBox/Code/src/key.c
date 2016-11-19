#include "key.h"

#define KEY_0 					0
#define KEY_1 					1
#define KEY_2 					2
#define KEY_3 					3

KEY_MSG_t keymsg;
void lptmr_hander(void);
void keyHander();
void key_0_hander();
void key_1_hander();
void key_2_hander();
void key_3_hander();

/* ������ʼ�� */
void key_event_init()
{
	PTXn_e keyMap[4] = {PTB20, PTB21, PTB22, PTB23};
	
	// ��ʼ��ȫ������
	key_map_init(keyMap);
	// LPTMR ��ʱ 10 ms	
	lptmr_timing_ms(10);  
	// �����жϷ��������ж���������					
	set_vector_handler(LPTMR_VECTORn,lptmr_hander);   
	// ʹ�� LPTMR �ж�
	enable_irq(LPTMR_IRQn); 
}

/* ��������ɨ�� */
void key_start_scan()
{
	// ʹ�� LPTMR �ж�
	enable_irq(LPTMR_IRQn); 
}

/* ֹͣ����ɨ�� */
void key_stop_scan()
{
	// ��ֹ LPTMR �ж�
	disable_irq(LPTMR_IRQn); 
}

void lptmr_hander(void)
{
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   // ��� LPT �Ƚϱ�־λ
	key_IRQHandler();					// �Ѱ���ɨ�������뵽��ʱ�жϷ��������ʱִ��
	if(get_key_msg(&keymsg) == 1)
		keyHander();
}

void keyHander()
{
	if(keymsg.status == KEY_DOWN)
	{
		switch(keymsg.key)
		{
		case KEY_0:
			key_0_hander();
			break;
		case KEY_1:
			key_1_hander();
			break;
		case KEY_2:
			key_2_hander();
			break;
		case KEY_3:
			key_3_hander();
			break;
		default:
			return;
		}
	}
	else if(keymsg.status == KEY_UP);	// ��������
	else;								// ��������
}

typedef enum
{
	SETTING_LEVEL_0,
	SETTING_LEVEL_1,
	SETTING_LEVEL_2,
	SETTING_LEVEL_3
} SettingLevel;

static TftPage tftPage = PAGE_NORMAL;
static SettingLevel settingLevel = SETTING_LEVEL_0;
static SettingMode settingMode = SETTING_EXIT;
static SettingTimeCode settingTimeCode = SYS_SECOND;
uint8 timeCodeTmp;

/* ���� 0 �����¼� */
void key_0_hander()
{
	if(tftPage == PAGE_NORMAL);
	else if(tftPage == PAGE_SETTING)
		switch(settingLevel)
		{
		case SETTING_LEVEL_0:
			tftPage = PAGE_NORMAL;
			switch_page(tftPage);
			break;
		case SETTING_LEVEL_1:
			settingLevel = SETTING_LEVEL_0;
			settingMode = SETTING_NORMAL;
			showSetText(settingMode, SELECT_MODE);
			break;
		case SETTING_LEVEL_2:
			settingLevel = SETTING_LEVEL_1;
			if(settingMode == SETTING_TIME)
				saveSystemTime();
			if(settingMode == SETTING_MEIDCAL)
				sortMeidcalTime();
			if(settingMode == SETTING_MEIDCAL_COUNT)
				setMeidcineCount(getTimeValue(ALL_MEIDCAL_COUNT));
			showSetText(settingMode, SELECT_MODE);
			showSystemTime(PAGE_SETTING, true);
			showMeidcalTime(PAGE_SETTING);
			showMeidcalCount(PAGE_SETTING);
			break;
		case SETTING_LEVEL_3:
			settingLevel = SETTING_LEVEL_2;
			showTimeSetting(SELECT_MODE, settingTimeCode, getTimeValue(settingTimeCode));
			break;
		default:
			break;
		}
	else if(tftPage == PAGE_REMIND)
	{
		changeTimeData(CURRENT_MEIDCAL_COUNT, SUB);
		if(getPlayFlag())
			setPlayFlag(false);
		stop_music();
		tftPage = PAGE_NORMAL;
		switch_page(tftPage);
	}
	else if(tftPage == PAGE_BLUETOOTH)
	{
		tftPage = PAGE_NORMAL;
		bt_stop();
		switch_page(tftPage);
	}
	else //if(tftPage == PAGE_HEALTH_EXAM)
	{
		pulsesensor_stop();
		mpu90615_stop();
		tftPage = PAGE_NORMAL;
		switch_page(tftPage);	
	}
}

/* ���� 1 �����¼� */
void key_1_hander()
{
	if(tftPage == PAGE_NORMAL)
	{
		pulsesensor_start();
		mpu90615_start();
		tftPage = PAGE_HEALTH_EXAM;
		switch_page(tftPage);	
	}
	else if(tftPage == PAGE_SETTING)
		switch(settingLevel)
		{
		case SETTING_LEVEL_0:
			break;
		case SETTING_LEVEL_1:
			if(settingMode == SETTING_TIME)
				settingMode = SETTING_MEIDCAL_COUNT;
			else if(settingMode == SETTING_MEIDCAL)
				settingMode = SETTING_TIME;
			else if(settingMode == SETTING_MEIDCAL_COUNT)
				settingMode = SETTING_MEIDCAL;
			else;
			showSetText(settingMode, SELECT_MODE);
			break;
		case SETTING_LEVEL_2:
			timeCodeTmp = (uint8)settingTimeCode;
			if(settingMode == SETTING_TIME)
				settingTimeCode = (SettingTimeCode)((timeCodeTmp == SYS_WEEK) ? SYS_SECOND : (++timeCodeTmp));
			else if(settingMode == SETTING_MEIDCAL)
				settingTimeCode = (SettingTimeCode)((timeCodeTmp == EAT_4_HOUR) ? EAT_1_MINUTE : (++timeCodeTmp));
			else if(settingMode == SETTING_MEIDCAL_COUNT)
				settingTimeCode = ALL_MEIDCAL_COUNT;
			else;
			showTimeSetting(SELECT_MODE, settingTimeCode, getTimeValue(settingTimeCode));			
			break;
		case SETTING_LEVEL_3:
			showTimeSetting(SETTING_MODE, settingTimeCode, changeTimeData(settingTimeCode, SUB));
			break;
		default:
			break;
		}
	else if(tftPage == PAGE_REMIND)
	{
		changeTimeData(CURRENT_MEIDCAL_COUNT, SUB);
		if(getPlayFlag())
			setPlayFlag(false);
		stop_music();
		tftPage = PAGE_NORMAL;
		switch_page(tftPage);	
	}
	else if(tftPage == PAGE_BLUETOOTH);
	else; //if(tftPage == PAGE_HEALTH_EXAM)
}

/* ���� 2 �����¼� */
void key_2_hander()
{
	if(tftPage == PAGE_NORMAL)
	{
		tftPage = PAGE_BLUETOOTH;
		bt_start();
		switch_page(tftPage);	
	}
	else if(tftPage == PAGE_SETTING)
		switch(settingLevel)
		{
		case SETTING_LEVEL_0:
			break;
		case SETTING_LEVEL_1:
			if(settingMode == SETTING_TIME)
				settingMode = SETTING_MEIDCAL;
			else if(settingMode == SETTING_MEIDCAL)
				settingMode = SETTING_MEIDCAL_COUNT;
			else if(settingMode == SETTING_MEIDCAL_COUNT)
				settingMode = SETTING_TIME;
			else;
			showSetText(settingMode, SELECT_MODE);
			break;
		case SETTING_LEVEL_2:
			timeCodeTmp = (uint8)settingTimeCode;
			if(settingMode == SETTING_TIME)
				settingTimeCode = (SettingTimeCode)((timeCodeTmp == SYS_SECOND) ? SYS_WEEK : (--timeCodeTmp));
			else if(settingMode == SETTING_MEIDCAL)
				settingTimeCode = (SettingTimeCode)((timeCodeTmp == EAT_1_MINUTE) ? EAT_4_HOUR : (--timeCodeTmp));
			else if(settingMode == SETTING_MEIDCAL_COUNT)
				settingTimeCode = ALL_MEIDCAL_COUNT;
			else;
			showTimeSetting(SELECT_MODE, settingTimeCode, getTimeValue(settingTimeCode));
			break;
		case SETTING_LEVEL_3:
			showTimeSetting(SETTING_MODE, settingTimeCode, changeTimeData(settingTimeCode, ADD));
			break;
		default:
			break;
		}
	else if(tftPage == PAGE_REMIND)
	{	
		changeTimeData(CURRENT_MEIDCAL_COUNT, SUB);
		if(getPlayFlag())
			setPlayFlag(false);
		stop_music();
		tftPage = PAGE_NORMAL;
		switch_page(tftPage);
	}
	else if(tftPage == PAGE_BLUETOOTH);
	else; //if(tftPage == PAGE_HEALTH_EXAM)
}

/* ���� 3 �����¼� */
void key_3_hander()
{
	if(tftPage == PAGE_NORMAL)
	{
		tftPage = PAGE_SETTING;
		settingLevel = SETTING_LEVEL_0;
		settingMode = SETTING_NORMAL;
		switch_page(tftPage);
	}
	else if(tftPage == PAGE_SETTING)
		switch(settingLevel)
		{
		case SETTING_LEVEL_0:
			settingLevel = SETTING_LEVEL_1;
			settingMode = SETTING_TIME;
			showSetText(settingMode, SELECT_MODE);
			break;
		case SETTING_LEVEL_1:
			settingLevel = SETTING_LEVEL_2;
			if(settingMode == SETTING_TIME)
				settingTimeCode = SYS_SECOND;
			else if(settingMode == SETTING_MEIDCAL)
				settingTimeCode = EAT_1_MINUTE;	
			else if(settingMode == SETTING_MEIDCAL_COUNT)
				settingTimeCode = ALL_MEIDCAL_COUNT;	
			else;
			showSetText(settingMode, SETTING_MODE);
			showTimeSetting(SELECT_MODE, settingTimeCode, getTimeValue(settingTimeCode));
			break;
		case SETTING_LEVEL_2:
			settingLevel = SETTING_LEVEL_3;
			showTimeSetting(SETTING_MODE, settingTimeCode, getTimeValue(settingTimeCode));
			break;
		case SETTING_LEVEL_3:
			settingLevel = SETTING_LEVEL_2;
			showTimeSetting(SELECT_MODE, settingTimeCode, getTimeValue(settingTimeCode));
			break;
		default:
			break;
		}
	else if(tftPage == PAGE_REMIND)
	{	
		changeTimeData(CURRENT_MEIDCAL_COUNT, SUB);
		if(getPlayFlag())
			setPlayFlag(false);
		stop_music();
		tftPage = PAGE_NORMAL;
		switch_page(tftPage);
	}
	else if(tftPage == PAGE_BLUETOOTH);
	else; //if(tftPage == PAGE_HEALTH_EXAM)
}

TftPage timeUpdateControl()
{
	if(tftPage == PAGE_SETTING && settingLevel >= SETTING_LEVEL_2 && settingTimeCode <= SYS_WEEK)
		return PAGE_NULL;
	else
		return tftPage;
}

void setTftPage(TftPage page)
{
	tftPage = page;
}










