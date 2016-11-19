#include "main.h"

void main()
{	
	initAll();
	LOOP
	{
		key_stop_scan();		
		
		if(timeUpdateControl() != PAGE_NULL)
		{
			undateCurrentTime();
			if(isRemindTime())
			{
				switch_page(PAGE_REMIND);
				setTftPage(PAGE_REMIND);
				setPlayFlag(true);
			}
			else
			{
				if(isMeidcalNotEat())
				{
					setPlayFlag(false);
					gms_send_msg();
				}
			}
		}
		
		if(timeUpdateControl() == PAGE_NORMAL)
		{
			showSystemTime(PAGE_NORMAL, true);
			showAirLevel(PAGE_NORMAL, true);
			showHumAndTemp(PAGE_NORMAL, true);
			showMeidcalTime(PAGE_NORMAL);
		}
		else if(timeUpdateControl() == PAGE_SETTING)
		{
			showSystemTime(PAGE_SETTING, true);
		}
		else if(timeUpdateControl() == PAGE_REMIND)
		{
			if(getPlayFlag())
			{
				if(getPlayState() == STOP)
					play_music(MEDICINE_SOUND);
			}
			else
			{
				if(getPlayState() != STOP)
					stop_music();
			}
		}
		else if(timeUpdateControl() == PAGE_HEALTH_EXAM)
		{
			showHealthData(pulsesensor_get_times(), getMpu90615Data());
		}
		else if(timeUpdateControl() == PAGE_BLUETOOTH);
		else;
		
		key_start_scan();
		DELAY_MS(100);
	}
}

void initAll()
{
	ds1302_init();
	dht11_init();
	mpu90615_init();
	pulsesensor_init();
	gp2y1010au_init();
	receive_init();
	data_init();
	tft_init();
	mp3_init();
	gms_init();
	key_event_init();
}
