/*
 * Cl_PressureSensors.h
 *
 * Created: 4/15/2016 2:03:06 PM
 *  Author: user
 */ 


#ifndef CL_PRESSURESENSORS_H_
#define CL_PRESSURESENSORS_H_

#include "cl_app/inc/cl_types.h"

typedef enum {
	PS_EVENT_50MS,
	PS_EVENT_100MS,
	PS_EVENT_MAX,
} Cl_PressureSensor_EventType;

typedef enum {
	PS_STATE_IDLE,
	PS_STATE_INITIALISING,
	PS_STATE_ACTIVE,
	PS_STATE_MAX,
} Cl_PressureSensor_StateType;




#endif /* CL_PRESSURESENSORS_H_ */