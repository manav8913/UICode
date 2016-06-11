/*
 * SV_CONSOLE.h
 *
 * Created: 1/13/2014 6:22:09 PM
 *  Author: Geekay
 */ 


#ifndef SV_CONSOLE_H_
#define SV_CONSOLE_H_

#include "Platform\Service\sv_interface.h"
#include "asf.h"

#include "Platform/Drivers/DD_DRIVER_EXPORT_CALLS.h"

uint32_t sv_console_bytedatawrite(uint8_t sv_consoleTxdatabyte);
//uint8_t cl_console_receivedbyte(uint8_t);
//uint8_t sv_console_receivedbyte(uint8_t sv_consoleRxdatabyte);
//extern uint8_t cl_console_receivedbyte(uint8_t);

#endif /* SV_CONSOLE_H_ */