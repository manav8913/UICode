/*
 * DD_PWM.h
 *
 * Created: 1/6/2014 10:40:53 AM
 *  Author: Geekay
 */ 


#ifndef DD_PWM_H_
#define DD_PWM_H_

#include "asf.h"
#include "compiler.h"


typedef enum _pwm_channel
{
	#if (SAM3XA)
	CH_0	= PWM_CHANNEL_0,
	CH_1	= PWM_CHANNEL_1,
	CH_2	= PWM_CHANNEL_2,
	CH_4	= PWM_CHANNEL_4,
	CH_5    = PWM_CHANNEL_5,
	CH_6    = PWM_CHANNEL_6,
	CH_7    = PWM_CHANNEL_7,
	#endif
} pwm_chnl;


void DD_INIT_PWM(void);
uint32_t DD_SET_PRDY(const pwm_chnl CHANNEL_NUMBER,uint32_t prd_val);
uint32_t DD_SET_DUTY(const pwm_chnl CHANNEL_NUMBER,uint32_t dty_val);



#endif /* DD_PWM_H_ */