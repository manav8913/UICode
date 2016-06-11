/*
 * cl_conductivity_detector.h
 *
 * Created: 11/11/2014 3:57:32 PM
 *  Author: user
 */ 


#ifndef CL_CONDUCTIVITY_DETECTOR_H_
#define CL_CONDUCTIVITY_DETECTOR_H_
typedef enum 
{
	CL_COND_STATE_NOSTATE,
	CL_COND_STATE_NORMAL,
	CL_COND_STATE_LOW,
	CL_COND_STATE_HIGH
	
	}Sys_cond_statetype;




typedef enum
{
	CL_DAC_COND_STATE_NOSTATE,
	CL_DAC_COND_STATE_RO,
	CL_DAC_COND_STATE_OPEN,
	CL_DAC_COND_STATE_HIGH
	
}Sys_dac_cond_statetype;


typedef enum
{
	CL_COND_ALARM_NO_ALARM,
	CL_COND_ALARM_LOW,
	CL_COND_ALARM_HIGH,
	CL_COND_ALARM_MAX
	
}Sys_cond_alarmtype;
typedef enum
{
	CL_DAC_COND_ALARM_NO_ALARM,
	CL_DAC_COND_ALARM_RO,
	CL_DAC_COND_ALARM_LOW,
	CL_DAC_COND_ALARM_HIGH,
	CL_DAC_COND_ALARM_MAX
	
}Sys_dac_cond_alarmtype;


#endif /* CL_CONDUCTIVITY_DETECTOR_H_ */