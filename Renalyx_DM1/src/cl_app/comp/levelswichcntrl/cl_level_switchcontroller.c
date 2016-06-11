/*
 * cl_level_switchcontroller.c
 *
 * Created: 11/9/2014 11:42:11 AM
 *  Author: user
 */ 
#include "inc/cl_level_switchcontroller.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_testharness/inc/cl_th.h"

extern Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

extern Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_AlarmIdType  );

Levelswitch_msgType Levelswitch_msg = {LEVEL_SWITCH_EVENT_UNDEFINED,0 };
static uint16_t levelswitch_ontime =0, levelswitch_offtime = 0;
static uint16_t toggle_count = 0;
static InletValvestate_type v1_status = V1_UNDEFINED;
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );

Cl_ReturnCodes cl_get_ls_switch_time(int16_t* ls_ontime,int16_t* ls_offtime);
Cl_ReturnCodes cl_level_switchcontroller(levelswitchcntrl_event_type cntrl_event );
Cl_ReturnCodes cl_level_switchcontroller(levelswitchcntrl_event_type cntrl_event)
{
	cl_Datastreamtype cl_tdata;
	switch(cntrl_event)
	{
		case LEVEL_SWITCH_CNTRLER_EVENT_SEC:
		Cl_AlarmResetAlarm(LEVELSWITCH_ON_TO_OFF);
		Cl_AlarmResetAlarm(LEVELSWITCH_OFF_TO_ON);
		Cl_AlarmResetAlarm(LEVEL_SWITCH_LOW_TOGGLERATE);
		
		toggle_count++;
												cl_tdata.word = 0;

										
										cl_tdata.Twobyte = toggle_count;
										cl_tdata.bytearray[2] = 21;
										
										
										
									//	 Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		if(toggle_count > 20 )
		{
			// low inlet flow alarm or level switch  malfunction
			Cl_Alarm_TriggerAlarm(LEVEL_SWITCH_LOW_TOGGLERATE,1);
		}

	
		break;
		case LEVEL_SWITCH_CNTRLER_EVENT_100MS:
		break;
		case LEVEL_SWITCH_CNTRLER_EVENT_10MS:
		break;
		case LEVEL_SWITCH_CNTRLER_EVENT_INSTANT:
		if(Levelswitch_msg.levelswitch_event == LEVEL_SWITCH_EVENT_ON_TO_OFF)
		{
			//stop flow
			sv_cntrl_deactivate_valve(VALVE_ID1);
			Levelswitch_msg.levelswitch_event = LEVEL_SWITCH_EVENT_UNDEFINED;
			levelswitch_offtime = 0;
			if(v1_status != V1_OFF )
			{
				toggle_count = 0;
				v1_status = V1_OFF;
			}

		}
		else if(Levelswitch_msg.levelswitch_event == LEVEL_SWITCH_EVENT_OFF_TO_ON)
		{
			//start flow
			sv_cntrl_activate_valve(VALVE_ID1);
			Levelswitch_msg.levelswitch_event = LEVEL_SWITCH_EVENT_UNDEFINED;
			levelswitch_ontime = 0;
			if(v1_status != V1_ON )
			{
				toggle_count = 0;
				v1_status = V1_ON;
			}
			
		}
		break;
		default:break;
		
	}
}