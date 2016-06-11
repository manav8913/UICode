/*
 * SV_TIMER.h
 *
 * Created: 1/6/2014 10:36:50 AM
 *  Author: Geekay
 */ 


#ifndef SV_TIMER_H_
#define SV_TIMER_H_

#include "asf.h"
#include "Platform/Drivers/DD_DRIVER_EXPORT_CALLS.h"

uint8_t SV_START_TIMER(uint8_t timer_no, uint32_t timer_res);
uint8_t SV_STOP_TIMER(uint8_t timer_no);


#endif /* SV_TIMER_H_ */