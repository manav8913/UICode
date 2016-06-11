/*
 * cl_level_switchcontroller.h
 *
 * Created: 11/9/2014 11:42:25 AM
 *  Author: user
 */ 


#ifndef CL_LEVEL_SWITCHCONTROLLER_H_
#define CL_LEVEL_SWITCHCONTROLLER_H_
#include "cl_app/inc/cl_types.h"

typedef enum
{
	 LEVEL_SWITCH_UNDEFINED,
	 LEVEL_SWITCH_LEVEL_HIGH,
	 LEVEL_SWITCH_LEVEL_LOW,
	 LEVEL_SWITCH_MAX
	
	}levelswitchstate_type;
typedef enum
{
	LEVEL_SWITCH_EVENT_UNDEFINED,
	LEVEL_SWITCH_EVENT_ON_TO_OFF,
	LEVEL_SWITCH_EVENT_OFF_TO_ON,
	LEVEL_SWITCH_EVENT_MAX
	
}levelswitchevent_type;
typedef enum
{
	LEVEL_SWITCH_CNTRLER_EVENT_NULL,
	LEVEL_SWITCH_CNTRLER_EVENT_SEC,
	LEVEL_SWITCH_CNTRLER_EVENT_100MS,
	LEVEL_SWITCH_CNTRLER_EVENT_10MS,
	LEVEL_SWITCH_CNTRLER_EVENT_INSTANT,
	LEVEL_SWITCH_CNTRLER_EVENT_MAX
	
}levelswitchcntrl_event_type;

typedef enum
{
	V1_UNDEFINED,
	V1_ON,
	V1_OFF,
	V1_MAX
	
}InletValvestate_type;

typedef struct{
	levelswitchevent_type levelswitch_event;
	int8_t event_ID;
	
}Levelswitch_msgType;
#endif /* CL_LEVEL_SWITCHCONTROLLER_H_ */