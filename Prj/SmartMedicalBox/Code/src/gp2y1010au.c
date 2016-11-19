#include "gp2y1010au.h"

static AirLevel airLevel;
static uint8 adcValue;

void PIT1_IRQHandler()
{
	PIT_Flag_Clear(GP2Y1010AU_PTI);
	
	gpio_set(GP2Y1010AU_LED, 0);
	DELAY_US(280);	
	adcValue = adc_once(GP2Y1010AU_ADC, ADC_8bit);
	DELAY_US(40);
	gpio_set(GP2Y1010AU_LED, 1);
	
	if(adcValue <= 108)								// 1.4 V
		airLevel = AIR_LEVEL_1;
	else if(adcValue > 108 && adcValue <= 155)		// 2.0 V
		airLevel = AIR_LEVEL_2;
	else if(adcValue > 155 && adcValue <= 216)		// 2.8 V
		airLevel = AIR_LEVEL_3;
	else //if(adcValue > 216)
		airLevel = AIR_LEVEL_4;
}

void gp2y1010au_init()
{
	airLevel = AIR_LEVEL_UNKNOW;
	
	gpio_init(GP2Y1010AU_LED, GPO, 1);
	adc_init(GP2Y1010AU_ADC);
	pit_init_ms(GP2Y1010AU_PTI, GP2Y1010AU_TIMER);
	set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);
	enable_irq(PIT1_IRQn);
}

void gp2y1010au_start()
{
	enable_irq(PIT1_IRQn);
}

void gp2y1010au_stop()
{
	
	disable_irq(PIT1_IRQn);
}

uint8 gp2y1010au_get_value()
{
	return adcValue;
}

AirLevel gp2y1010au_get_level()
{
	return airLevel;
}