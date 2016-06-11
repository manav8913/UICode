/*
 * DD_ADC.h
 *
 * Created: 1/6/2014 10:43:38 AM
 *  Author: Geekay
 */ 


#ifndef DD_ADC_H_
#define DD_ADC_H_

#include "asf.h"

typedef enum _adc_channel
{
	#if (SAM3XA)
	channel_0	= ADC_CHANNEL_0,
	channel_1	= ADC_CHANNEL_1,
	channel_2	= ADC_CHANNEL_2,
	channel_3	= ADC_CHANNEL_3,
	channel_4	= ADC_CHANNEL_4,
	channel_5	= ADC_CHANNEL_5,
	channel_6	= ADC_CHANNEL_6,
	channel_7	= ADC_CHANNEL_7,
	channel_8	= ADC_CHANNEL_8,
	channel_9	= ADC_CHANNEL_9,
	channel_10	= ADC_CHANNEL_10,
	channel_11	= ADC_CHANNEL_11,
	channel_12	= ADC_CHANNEL_12,
	channel_13	= ADC_CHANNEL_13,
	channel_14	= ADC_CHANNEL_14,
	channel_15	= ADC_TEMPERATURE_SENSOR,
	
	#endif
} adc_chnl;


uint16_t DD_READ_ADC(const adc_chnl channel_number);
void DD_INIT_ADC(void);


#endif /* DD_ADC_H_ */