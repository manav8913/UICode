/*
 * cl_debug.c
 *
 * Created: 2/5/2014 9:28:07 AM
 *  Author: user
 */ 

#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"

Cl_ReturnCodes Cl_Printf(uint8_t* pstring, uint8_t size);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );


Cl_ReturnCodes Cl_Printf(uint8_t* pstring, uint8_t size)
{
	
	Cl_ReturnCodes Cl_RetVal;
	
	Cl_RetVal = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pstring,8);
	
	return Cl_RetVal;
}