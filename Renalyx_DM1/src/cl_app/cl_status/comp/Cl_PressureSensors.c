/*
 * Cl_PressureSensors.c
 *
 * Created: 4/13/2016 5:24:52 PM
 *  Author: user
 */ 

#include "inc/Cl_PressureSensors.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"

static uint16_t Ps1 = 0, Ps2 = 0, Ps3 = 0,apt =0, vpt =0 ,counter =0;

static Cl_PressureSensor_StateType Cl_PressureSensor_State = PS_STATE_IDLE;
extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType, uint16_t*);
Cl_ReturnCodes  Cl_Pressure_Controller_Handler(Cl_PressureSensor_EventType Cl_Ps_Event);

Cl_ReturnCodes  Cl_Pressure_Controller_Handler(Cl_PressureSensor_EventType Cl_Ps_Event)
{
	Cl_ReturnCodes cl_retval = CL_OK;
	uint16_t temp_val  = 0;
	
	if(Cl_PressureSensor_State == PS_STATE_INITIALISING)
	{
		switch(Cl_Ps_Event)
		{
			
			case PS_EVENT_50MS:
			break;
			case PS_EVENT_100MS:
			if ( counter == 0 )
			{
				Cl_SysStat_GetSensor_Status_Query(PS1STATUS_HIGH, &Ps1);
				Cl_SysStat_GetSensor_Status_Query(PS2STATUS_HIGH, &Ps2);
				Cl_SysStat_GetSensor_Status_Query(PS3STATUS_HIGH, &Ps3);
				Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH, &apt);
				Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH, &vpt);
			}
			else
			{
				Cl_SysStat_GetSensor_Status_Query(PS1STATUS_HIGH, &temp_val);
				Ps1 = ( Ps1 +temp_val)/counter+1 ;
				Cl_SysStat_GetSensor_Status_Query(PS2STATUS_HIGH, &temp_val);
				Ps2 = ( Ps2 +temp_val)/counter+1 ;
				Cl_SysStat_GetSensor_Status_Query(PS3STATUS_HIGH, &temp_val);
				Ps3 = ( Ps3 +temp_val)/counter+1 ;
				Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH, &temp_val);
				apt = ( apt +temp_val)/counter+1 ;
				Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH, &temp_val);
				vpt = ( vpt +temp_val)/counter+1 ;
			}

			counter++;
			if(counter > 10)
			{
				counter = 0;
				Cl_PressureSensor_State = PS_STATE_ACTIVE;
			}
			break;
			default:break;
		}
	}
	else if(Cl_PressureSensor_State == PS_STATE_ACTIVE)
	{
		switch(Cl_Ps_Event)
		{
			
			case PS_EVENT_50MS:
			break;
			case PS_EVENT_100MS:
				Cl_SysStat_GetSensor_Status_Query(PS1STATUS_HIGH, &temp_val);
				Ps1 = ( 9 *Ps1 +temp_val)/10 ;
				Cl_SysStat_GetSensor_Status_Query(PS2STATUS_HIGH, &temp_val);
				Ps2 = ( 9 * Ps2 +temp_val)/10 ;
				Cl_SysStat_GetSensor_Status_Query(PS3STATUS_HIGH, &temp_val);
				Ps3 = ( 9*Ps3 +temp_val)/10 ;
				Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH, &temp_val);
				apt = ( 9* apt +temp_val)/10 ;
				Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH, &temp_val);
				vpt = ( 9* vpt +temp_val)/10 ;

			
			
			break;
			default:break;
		}
		
	}

	
	
	return cl_retval;
}

Cl_ReturnCodes  Cl_Pressure_Controller_Init(void)
{

	Cl_ReturnCodes cl_retval = CL_OK;
	 Ps1 = 0;
	Ps2 = 0;
	Ps3 = 0;
	apt =0 ;
	vpt = 0;
	Cl_PressureSensor_State = PS_STATE_INITIALISING;
	
	return cl_retval;
}