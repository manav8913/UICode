/*
 * cl_deaeration.c
 *
 * Created: 7/30/2015 11:52:47 AM
 *  Author: user
 */ 

#include "inc/cl_deaeration.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "Platform/Service/sv_interface.h"

extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType, uint16_t*);
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );

cl_deaeration_state_type cl_deaeration_state = CL_DEAER_STATE_IDLE;

Cl_ReturnCodes cl_deaeration_controller(cl_deaeration_event_type cntrl_event)
{
	Cl_ReturnCodes cl_ret_value = CL_OK;
	uint16_t dac2_status  ;
	
	switch (cl_deaeration_state)
	{
		case CL_DEAER_STATE_IDLE:
				switch(cntrl_event)
				{
					case CL_DEAER_EVENT_ACTIVATE:
					cl_deaeration_state = CL_DEAER_ACTIVE;
					break;
					default:break;
				}
		break;
		case CL_DEAER_ACTIVE:
			switch(cntrl_event)
			{
				

				case CL_DEAER_EVENT_DEACTIVATE:
				cl_deaeration_state = CL_DEAER_STATE_IDLE;
				break;
				case CL_DEAER_EVENT_50MS:
				// check for deaeartion 
				Cl_SysStat_GetSensor_Status_Query(COND_DAC_RO,&dac2_status);
				if (dac2_status < 10)
				{
					sv_cntrl_activate_valve(VALVE_ID19);
					cl_deaeration_state = CL_DEAER_ACTIVE_INPROGRESS;
				}
				else
				{
					sv_cntrl_deactivate_valve(VALVE_ID19);	
				}
				break;
				default:
				break;
			}
		break;
		case CL_DEAER_ACTIVE_INPROGRESS:
					switch(cntrl_event)
					{
						
						case CL_DEAER_EVENT_DEACTIVATE:
						cl_deaeration_state = CL_DEAER_STATE_IDLE;
						break;
						case CL_DEAER_EVENT_500MS:
						Cl_SysStat_GetSensor_Status_Query(COND_DAC_RO,&dac2_status);
						if (dac2_status > 5)
						{
						sv_cntrl_deactivate_valve(VALVE_ID19);
						cl_deaeration_state = CL_DEAER_ACTIVE;
						}
					}
		
		break;
		default:break;
	}

	
}