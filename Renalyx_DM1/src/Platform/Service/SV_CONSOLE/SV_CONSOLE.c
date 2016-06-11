/*
 * SV_CONSOLE.c
 *
 * Created: 1/13/2014 6:21:44 PM
 *  Author: Geekay
 */ 

#include "SV_CONSOLE.h"
/*
uint8_t sv_console_bytedatawrite(Uart *p_uart, const sv_consoleTxdatabyte)
{
	DD_CONSOLE_TX(Uart *p_uart, const sv_consoleTxdatabyte);
	
}

*/
uint32_t sv_console_bytedatawrite(uint8_t sv_consoleTxdatabyte)
{
	uint32_t console_stat;
	console_stat = DD_CONSOLE_TX(sv_consoleTxdatabyte);
	
	return console_stat;
	
}


uint8_t sv_console_receivedbyte(uint8_t sv_consoleRxdatabyte)
{
	//uint8_t sv_consoleRxdatabyte;
	
	
	cl_console_receivedbyte(sv_consoleRxdatabyte);
	//sv_consoleRxdatabyte = DD_CONSOLE_RX();
	
	return 0;
	
}

