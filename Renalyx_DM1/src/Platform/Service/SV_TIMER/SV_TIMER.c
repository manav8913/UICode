/*
 * SV_TIMER.c
 *
 * Created: 1/6/2014 10:37:04 AM
 *  Author: Geekay
 */ 

#include "SV_TIMER.h"

uint8_t SV_START_TIMER(uint8_t timer_no, uint32_t timer_res)
{
// 	timer_no = 2;
// 	timer_res = 100;
// 	printf("inside sv start");
	DD_CONFIG_TIMER(timer_no, timer_res);
	
	return 0;
}

uint8_t SV_STOP_TIMER(uint8_t timer_no)
{
	
	DD_STOP_TIMER(timer_no);
	
	return 0;
}



