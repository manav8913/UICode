/*
 * DD_INT.c
 *
 * Created: 1/7/2014 5:57:40 PM
 *  Author: Geekay
 */ 

#include "DD_INT.h"

void DD_INIT_INT()
{
	
	
	NVIC_DisableIRQ(UART_IRQn);
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_interrupt(UART,UART_SR_RXRDY);
		
	NVIC_DisableIRQ(TC0_IRQn);
	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn, 0);
	NVIC_EnableIRQ(TC0_IRQn);
/*
	NVIC_DisableIRQ(TC1_IRQn);
	NVIC_ClearPendingIRQ(TC1_IRQn);
	NVIC_SetPriority(TC1_IRQn, 0);
	
	NVIC_DisableIRQ(TC2_IRQn);
	NVIC_ClearPendingIRQ(TC2_IRQn);
	NVIC_SetPriority(TC2_IRQn, 0);*/
	
	NVIC_DisableIRQ(TC3_IRQn);
	NVIC_ClearPendingIRQ(TC3_IRQn);
	NVIC_SetPriority(TC3_IRQn,0);
	NVIC_EnableIRQ(TC3_IRQn);
	
/*	
	NVIC_DisableIRQ(TC4_IRQn);
	NVIC_ClearPendingIRQ(TC4_IRQn);
	NVIC_SetPriority(TC4_IRQn, 0);
	
	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_SetPriority(TC5_IRQn, 0);
	*/
	NVIC_DisableIRQ(TC6_IRQn);
	NVIC_ClearPendingIRQ(TC6_IRQn);
	NVIC_SetPriority(TC6_IRQn, 0);
	NVIC_EnableIRQ(TC6_IRQn);
/*	
	NVIC_DisableIRQ(TC7_IRQn);
	NVIC_ClearPendingIRQ(TC7_IRQn);
	NVIC_SetPriority(TC7_IRQn, 0);
	
	NVIC_DisableIRQ(TC8_IRQn);
	NVIC_ClearPendingIRQ(TC8_IRQn);
	NVIC_SetPriority(TC8_IRQn, 0);
*/
	
	
	NVIC->ICER[6] = 0xFFFFFFFF;
	NVIC->ICPR[6] = 0XFFFFFFFF;
	NVIC_SetPriority(TC6_IRQn, 0);
	//NVIC_EnableIRQ(TC6_IRQn);
	NVIC->ISER[6] = 0xFFFFFFFF;
	
	
	// FOR TIMER2 OF 100ms MEANS 10HZ FREQUENCY
	NVIC->ICER[3] = 0xFFFFFFFF;
	NVIC->ICPR[3] = 0XFFFFFFFF;
	//NVIC_DisableIRQ(TC3_IRQn);
	//NVIC_ClearPendingIRQ(TC3_IRQn);
	NVIC_SetPriority(TC3_IRQn, 0);
	//NVIC_EnableIRQ(TC3_IRQn);
	NVIC->ISER[3] = 0xFFFFFFFF;
}