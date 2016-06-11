/*
 * DD_TIMER.h
 *
 * Created: 1/6/2014 10:41:29 AM
 *  Author: Geekay
 */ 


#ifndef DD_TIMER_H_
#define DD_TIMER_H_

#include "asf.h"


void DD_INIT_TIMER(void);
uint8_t DD_SET_TIMER(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t rc);
//uint8_t DD_TOP_TIMER(Tc *tc, uint32_t channel, IRQn_Type irq);

#endif /* DD_TIMER_H_ */