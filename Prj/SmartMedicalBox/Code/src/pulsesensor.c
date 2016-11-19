#include "pulsesensor.h"

static uint32 timeFlag;
static uint16 preValue;
static uint16 currentValue;
static uint8 heartRate;
static bool heartFlag;

void value_init()
{
	timeFlag = 0;
	preValue = 0;
	currentValue = 0;
	heartRate = 0;
	heartFlag = false;
}

void PIT0_IRQHandler()
{
	PIT_Flag_Clear(PULSESENSOR_PIT);
	timeFlag++;
	currentValue = adc_once(PULSESENSOR_PORT, ADC_8bit);
	if(preValue < currentValue && currentValue >= PULSESENSOR_MIN && !heartFlag)
	{
		heartFlag = true;
		heartRate = 60 * (1000/PULSESENSOR_TIMER) / timeFlag;
		timeFlag = 0;
	}
	if(currentValue < PULSESENSOR_MIN && heartFlag)
		heartFlag = false;
	if(timeFlag > 2000/PULSESENSOR_TIMER)
		value_init();
	preValue = currentValue;
}

void pulsesensor_init()
{
	value_init();
	adc_init(PULSESENSOR_PORT);
	pit_init_ms(PULSESENSOR_PIT, PULSESENSOR_TIMER);
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
}

void pulsesensor_start()
{
	value_init();
	enable_irq(PIT0_IRQn);
}

void pulsesensor_stop()
{
	disable_irq(PIT0_IRQn);
}

uint8 pulsesensor_get_times()
{
	return heartRate;
}