/*
 * DD_TIMER.c
 *
 * Created: 1/6/2014 10:41:42 AM
 *  Author: Geekay
 */ 
#include "DD_TIMER.h"


void DD_INIT_TIMER()
{
	TC0->TC_WPMR = 0x54494D00;
	TC1->TC_WPMR = 0x54494D00;
	TC2->TC_WPMR = 0x54494D00;
	pmc_enable_periph_clk(ID_TC0);
	pmc_enable_periph_clk(ID_TC1);
	pmc_enable_periph_clk(ID_TC2);
	TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	//TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	//TC0->TC_CHANNEL[2].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	//TC1->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	//TC1->TC_CHANNEL[2].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	//TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	//TC2->TC_CHANNEL[2].TC_CMR = TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
	
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
	
	TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
//	TC1->TC_CHANNEL[0].TC_IDR = 0xef;		//TC_IDR_COVFS|TC_IDR_CPAS|TC_IDR_ETRGS|TC_IDR_LDRAS|TC_IDR_CPBS|TC_IDR_LDRBS|TC_IDR_LOVRS;
//	TC1->TC_CHANNEL[0].TC_IMR = 0x10;//0x00 | TC_IMR_CPCS;
	
	TC2->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
//	TC2->TC_CHANNEL[0].TC_IDR = TC_IDR_COVFS|TC_IDR_CPAS|TC_IDR_ETRGS|TC_IDR_LDRAS|TC_IDR_CPBS|TC_IDR_LDRBS|TC_IDR_LOVRS;
//	TC2->TC_CHANNEL[0].TC_IMR = 0x00 | TC_IMR_CPCS;
	
}




uint8_t DD_SET_TIMER(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t rc)
{
	//printf("inside timer");
	//uint32_t rc = 0; 
	pmc_set_writeprotect(0);
	channel=0;
//	pmc_enable_periph_clk(ID_TC6);
	//tc_init(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4);
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |TC_CMR_TCCLKS_TIMER_CLOCK4;
//	rc=656249; //value to get 1sec delay (mlk/128*interrupt frequency)-1 number counts to get 1sec
	tc_write_rc(tc, channel, rc);
	tc_start(tc, channel);
	tc_enable_interrupt(tc, channel, TC_IER_CPCS);
//	tc_write_rc(TC2, channel, rc);
//	tc_start(TC2, channel);
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("-F- Systick configuration error\r");
		while (1);
	}
	
}

