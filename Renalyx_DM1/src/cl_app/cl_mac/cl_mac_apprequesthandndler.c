/*
 * Cl_MacAppRequestHandler.c
 *
 * Created: 1/16/2014 3:21:40 PM
 *  Author: user
 */ 

#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"

extern MAC_EVENTS Cl_MacEvent;
extern bool Cl_MacNewEvent;
extern Cl_Mac_EventType Cl_MacEvent1;


Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);

Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS cl_macirq)
{
	Cl_ReturnCodes Cl_Mac_RetVal = CL_OK;
	
	//Cl_MacEvent = cl_macirq;
	//Cl_MacNewEvent = true;
	
	if( Cl_MacEvent1.Cl_MacNewEventcount <= MAC_EVENT_COUNT_MAX )
	{
		Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = cl_macirq ;
		Cl_MacEvent1.Cl_MacNewEvent = true;
		Cl_MacEvent1.Cl_MacNewEventcount++;
	}
	return Cl_Mac_RetVal;
}