/*
 * Cl_Conductivity_Sensors.c
 *
 * Created: 4/13/2016 5:25:09 PM
 *  Author: user
 */ 

#include "cl_app/inc/cl_types.h"
#include "inc/Cl_Conductivity_Sensors.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"

extern uint8_t sv_cs_setcondpotvalue(uint16_t resistance);
extern Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern Cl_ReturnCodes cl_wait(uint32_t );

Cl_CondSensor_StateType Cond_Sensor_State = COND_STATE_IDLE;

Cl_ReturnCodes  Cl_Conductivity_Sensor_Controller(Cl_CondSensor_EventType Cl_cond_Event);

Cl_ReturnCodes  Cl_Conductivity_Sensor_Controller(Cl_CondSensor_EventType Cl_cond_Event)
{
		Cl_ReturnCodes cl_retval = CL_OK;
		uint16_t temp_val  = 0, cond_millivolts = 0;
		static uint16_t cond_pot_val_2volts = (3970 * 1024)/10000; 
		static cond_millivolts_avg = 0;
		
		switch(Cond_Sensor_State)
		{
			
			case COND_STATE_IDLE:
				switch (Cl_cond_Event)
					{
						case COND_EVENT_INIT:
						sv_cs_setcondpotvalue(cond_pot_val_2volts);
						cl_wait(100);
						Cl_SysStat_GetSensor_Status_Query(SENSOR_COND_WIEN_STATUS,&temp_val);
						cond_millivolts = 5 * temp_val;
						if((cond_millivolts_avg > 1900) && (cond_millivolts_avg < 2100))
						{
							return CL_OK;
						}
						else
						{
							return CL_ERROR;
						}
						
						break;
		
						case COND_EVENT_1SEC:
		
						Cl_SysStat_GetSensor_Status_Query(SENSOR_COND_WIEN_STATUS,&temp_val);
						cond_millivolts = 0.805 * temp_val;
						cond_millivolts_avg = cond_millivolts;
						if((cond_millivolts_avg > 1900) && (cond_millivolts_avg < 2100))
						{
							return CL_OK;
						}
						else
						{
							sv_cs_setcondpotvalue(cond_pot_val_2volts);
							return CL_OK;
						}
						Cond_Sensor_State = COND_STATE_ACTIVE;
						break;
						default:break;
					}
			break;
			case COND_STATE_ACTIVE:
				switch (Cl_cond_Event)
				{

					case COND_EVENT_1SEC:
		
					Cl_SysStat_GetSensor_Status_Query(SENSOR_COND_WIEN_STATUS,&temp_val);
					cond_millivolts = 0.805 * temp_val;
					if((cond_millivolts_avg > 1900) && (cond_millivolts_avg < 2100))
					{
							cond_millivolts_avg = (cond_millivolts_avg * 19 +cond_millivolts)/20;
		
						if (cond_millivolts_avg > 2010 )
						{
							cond_pot_val_2volts = cond_pot_val_2volts -1;
							sv_cs_setcondpotvalue(cond_pot_val_2volts );
						}
						else if (cond_millivolts_avg < 1990 )
						{
							cond_pot_val_2volts = cond_pot_val_2volts + 1;
							sv_cs_setcondpotvalue(cond_pot_val_2volts );
						}
						else
						{
			
						}
					}
					else
					{
						return CL_ERROR;
					}
					break;
					default:break;
				}
				
			break;
			default:break;
		}
		

}