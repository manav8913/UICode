/*
 * DD_HANDLER.c
 *
 * Created: 1/16/2014 2:07:23 PM
 *  Author: Geekay
 */ 

#include "DD_HANDLER.h"
#include "cl_app/inc/cl_types.h"

extern Cl_ReturnCodes Cl_TimerTickCallback(void);
extern Cl_ReturnCodes cl_systemtickcallback(void);

void UART_Handler(void)
{
	
	char rx_val ;
	uint32_t i,status;
	status= UART->UART_SR;
//	if(status & 0x01)
	{
		rx_val=UART->UART_RHR;
	
		DD_CONSOLE_RX(rx_val);
	
		for(i=0; i< 0xff;i++)
		{
			;
		}
	}

}


void TC0_Handler()
{
	if ((tc_get_status(TC0, 0) & TC_SR_CPCS) == TC_SR_CPCS)
	{
		//printf("i m in handler tc0\n");
		
			Cl_ReturnCodes RetVal;
			RetVal = Cl_TimerTickCallback();
	}
}

void TC6_Handler()
{
	NVIC_ClearPendingIRQ(TC6_IRQn);
	if ((tc_get_status(TC2, 0) & TC_SR_CPCS) == TC_SR_CPCS)
	{
		//printf("i m in handler tc6\n");
		
	}
}
	//printf("inside tc6\n");



void TC3_Handler()
{
	NVIC_ClearPendingIRQ(TC3_IRQn);
	if ((tc_get_status(TC1, 0) & TC_SR_CPCS) == TC_SR_CPCS)
	{
		//printf("i m in handler tc3\n");
		
	}
}

void SysTick_Handler(void)
{
	
	Cl_ReturnCodes RetVal;
	RetVal = cl_systemtickcallback();
}