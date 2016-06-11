/*
 * SV_SYS.c
 *
 * Created: 1/6/2014 10:32:46 AM
 *  Author: Geekay
 */

#include "SV_SYS.h"
#include "Platform/Service/SV_TIMER/SV_TIMER.h"

extern void DD_INIT_IIC(void);

void SV_SYSTEM_INIT()
{
	//uint32_t prd_val=1000,dty_val = 100;
	DD_INIT_CLOCK();
	DD_INIT_PIO();
	DD_INIT_UART();
	DD_INIT_TIMER();
	DD_INIT_ADC();
	DD_INIT_PWM();
	DD_INIT_NVM();
	DD_INIT_INT();
	DD_INIT_IIC();
	
	SV_START_TIMER(0,1);
	//printf("1");
	//DD_SET_PRDY(CH_0,prd_val);
	//DD_SET_DUTY(CH_1,dty_val);
}