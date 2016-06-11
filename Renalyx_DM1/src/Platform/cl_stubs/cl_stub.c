/*
 * cl_stub.c
 *
 * Created: 1/16/2014 2:46:04 PM
 *  Author: Geekay
 */ 

#include "asf.h"

uint8_t cl_console_receivedbyte(uint8_t sv_consoleRxdatabyte)
{
	printf("%c", sv_consoleRxdatabyte);
	
}
