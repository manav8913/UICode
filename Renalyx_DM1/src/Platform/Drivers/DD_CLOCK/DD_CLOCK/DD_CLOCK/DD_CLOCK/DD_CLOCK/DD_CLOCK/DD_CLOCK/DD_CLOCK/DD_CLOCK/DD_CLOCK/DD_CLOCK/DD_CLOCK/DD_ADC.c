/*
 * DD_ADC.c
 *
 * Created: 1/6/2014 10:42:26 AM
 *  Author: Geekay
 */ 
#include "DD_ADC.h"

void DD_INIT_ADC()

{
	ADC->ADC_WPMR = 0x41444300;
	ADC->ADC_MR	  = 0x11040400;
	ADC->ADC_CHER = 0x000000FF;
}


uint16_t DD_READ_ADC(const adc_chnl channel_number)
{
	uint16_t ADC_data;	
	ADC->ADC_CR = 0x00000002;
	while ((ADC->ADC_ISR & (1<<channel_number)) == (1<<channel_number))
	{
		//delay_ms(100);
		ADC_data = ADC->ADC_CDR[channel_number];
		//delay_ms(1000);
	}
	
	return ADC_data;
}