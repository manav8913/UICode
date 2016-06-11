/*
 * Cl_temprature_sensor.c
 *
 * Created: 4/13/2016 5:24:31 PM
 *  Author: user
 */ 

#include "inc/Cl_temperature_sensor.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"

static uint16_t Temp1 = 0, Temp2 = 0, Temp3 = 0, counter =0;

static Cl_Temp_StateType Cl_Temp_State = TS_STATE_IDLE;
extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType, uint16_t*);
Cl_ReturnCodes  Cl_Temperature_Controller_Handler(Cl_Temp_EventType Cl_Tc_Event);

Cl_ReturnCodes  Cl_Temperature_Controller_Handler(Cl_Temp_EventType Cl_Tc_Event)
{
	Cl_ReturnCodes cl_retval = CL_OK;
	 uint16_t temp_val  = 0;
	
	if(Cl_Temp_State == TS_STATE_INITIALISING)
	{
			switch(Cl_Tc_Event)
			{
				
				case TS_EVENT_50MS:
				break;
				case TS_EVENT_100MS:
				if ( counter == 0 )
				{
					Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH, &Temp1);
					Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH, &Temp2);
					Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH, &Temp3);
				}
				else
				{
					Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH, &temp_val);
					Temp1 = ( Temp1 +temp_val)/counter+1 ;
					Cl_SysStat_GetSensor_Status_Query(TEMP2STATUS_HIGH, &temp_val);
					Temp2 = ( Temp2 +temp_val)/counter+1 ;
					Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH, &temp_val);
					Temp3 = ( Temp3 +temp_val)/counter+1 ;
				}

				counter++;
				if(counter > 10)
				{
					counter = 0;
					Cl_Temp_State = TS_STATE_ACTIVE;
				}
				break;
				default:break;
			}
	}
	else if(Cl_Temp_State == TS_STATE_ACTIVE)
	{
			switch(Cl_Tc_Event)
			{
				
				case TS_EVENT_50MS:
				break;
				case TS_EVENT_100MS:
				Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH, &temp_val);
				Temp1 = (Temp1 *9 + temp_val)/10;
				Cl_SysStat_GetSensor_Status_Query(TEMP2STATUS_HIGH, &temp_val);
				Temp2 = (Temp2 *9 + temp_val)/10;
				Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH, &temp_val);
				Temp3 = (Temp3 *9 + temp_val)/10;

				
				
				break;
				default:break;
			}
			
	}

	
	
	return cl_retval;
}

Cl_ReturnCodes  Cl_Temperature_Controller_Init(void)
{

	Cl_ReturnCodes cl_retval = CL_OK;
		 Temp1 = 0;
		 Temp2 = 0;
		 Temp3 = 0;
		 Cl_Temp_State = TS_STATE_INITIALISING;
		 
		 return cl_retval;
}