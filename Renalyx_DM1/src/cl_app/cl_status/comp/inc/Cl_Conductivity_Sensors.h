/*
 * Cl_Conductivity_Sensors.h
 *
 * Created: 6/6/2016 5:57:08 PM
 *  Author: user
 */ 


#ifndef CL_CONDUCTIVITY_SENSORS_H_
#define CL_CONDUCTIVITY_SENSORS_H_




typedef enum {
	COND_EVENT_INIT,
	COND_EVENT_100MS,
	COND_EVENT_1SEC,
	COND_EVENT_MAX,
} Cl_CondSensor_EventType; 

typedef enum {
	COND_STATE_IDLE,
	COND_STATE_INITIALISING,
	COND_STATE_ACTIVE,
	COND_STATE_MAX,
} Cl_CondSensor_StateType;

#endif /* CL_CONDUCTIVITY_SENSORS_H_ */