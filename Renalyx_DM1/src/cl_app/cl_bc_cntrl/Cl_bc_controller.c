/*
 * Cl_balancing_chamber_controller.c
 *
 * Created: 11/2/2014 4:13:35 PM
 *  Author: user
 */ 
#include "cl_app/inc/cl_types.h"
#include "inc/Cl_bc_controller.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_testharness/inc/cl_th.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"

extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
extern Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_NewAlarmIdType cl_alarm_id , bool cl_status);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

extern uint8_t sv_cntrl_setflowpath(sv_flowpathtype);
extern void sv_prop_startmixing();
extern void sv_prop_stopopenfill();
extern Cl_ReturnCodes SetHeaterState(HeaterStateType Param_HeaterState);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );

static Cl_BC_States bc_state = CL_BC_STATE_IDLE;
static Cl_BC_States bc_laststate = CL_BC_STATE_IDLE;
static bool bc_wait = false;
static int16_t wait_counter = 0;
static int16_t openfillcounter = 0;
volatile uint16_t switch_time1=0,switch_time2=0;
extern bool current_sense ;

sv_flowpathtype cl_flowpathtable[CL_BC_STATE_MAXSTATE] =
{
	FLOW_PATH_NULL,		//CL_BC_STATE_IDLE,
	FLOW_PATH_NULL,		//CL_BC_STATE_NC,
	BC_FLUSH_OPEN,		//CL_BC_STATE_OPENFILL_TRANSITION,
	BC_FLUSH_OPEN,		//CL_BC_STATE_OPENFILL,
	V15_FLUSH,			//CL_BC_STATE_V15OPEN_TRANSITION,
	V15_FLUSH,			//CL_BC_STATE_V15OPEN,
	V2_FLUSH,			//CL_BC_STATE_V2OPEN_TRANSITION,
	V2_FLUSH,			//CL_BC_STATE_V2OPEN,
	BC_FLUSH_1_V15,		//CL_BC_STATE_BO1_V4_TRANSITION,
	BC_FLUSH_1_V15,		//CL_BC_STATE_BO1_V4,
	BC_FLUSH_2_V15,		//CL_BC_STATE_BO2_V4_TRANSITION,
	BC_FLUSH_2_V15,		//CL_BC_STATE_BO2_V4,
	BC_FLUSH_1_V13_14,	//CL_BC_STATE_BO1_V13V14_TRANSITION,
	BC_FLUSH_1_V13_14,	//CL_BC_STATE_BO1_V13V14,
	BC_FLUSH_2_V13_14,	//CL_BC_STATE_BO2_V13V14_TRANSITION,
	BC_FLUSH_2_V13_14,  //CL_BC_STATE_BO2_V13V14,

};



Cl_ReturnCodes  Cl_stop_bc_controller(void)
{
	
	return 1;
}
Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event)
{
	Cl_ReturnCodes cl_bc_returncode = CL_OK;
	static int16_t fill_time = 0;
	cl_Datastreamtype cl_tdata;
	
	switch(cl_bc_event)
	{
		case	BC_EVENT_START:
				cl_bc_returncode = sv_cntrl_setflowpath(BC_FLUSH_OPEN);
				bc_laststate = bc_state;
				bc_state = CL_BC_STATE_OPENFILL_TRANSITION;
		break;
		case BC_EVENT_RESUME:
				cl_bc_returncode = sv_cntrl_setflowpath(BC_FLUSH_OPEN);
				cl_bc_returncode = sv_cntrl_setflowpath(cl_flowpathtable[bc_laststate]);
				//bc_laststate = bc_state;
				bc_state = bc_laststate;
		break;
		case	BC_EVENT_STOP:
				sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
				bc_state = CL_BC_STATE_IDLE;
			
		break;
		case	BC_EVENT_PAUSE:
		break;
		case	BC_EVENT_RESET:
		break;
	}
	switch (bc_state)
	{
			case CL_BC_STATE_IDLE:
			// wait for commands
			break;
			case CL_BC_STATE_NC:
			break;
			case CL_BC_STATE_OPENFILL_TRANSITION:
							switch(cl_bc_event)
							{							
								case BC_EVENT_500MS:				
									bc_wait = true;				
								break;
								case BC_EVENT_SECOND:
								if(bc_wait == true)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_OPENFILL;
									bc_wait = false;
								}
								break;
								case BC_EVENT_STOP_FOR_BC1:
								openfillcounter = 0;
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
								if(cl_bc_returncode == CL_OK)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_STOPPED_FOR_BC1;
								}
								break;
								case BC_EVENT_STOP_FOR_BC2:
								openfillcounter = 0;
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V13_14);
								if(cl_bc_returncode == CL_OK)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_STOPPED_FOR_BC2;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_OPENFILL:
							switch(cl_bc_event)
							{
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
								if(openfillcounter++ > CL_BC_OPENFILL_TIMEOUT * 60)
								{
									sv_prop_stopopenfill();
									openfillcounter = 0;
								//	sv_prop_startmixing();
								//	SetHeaterState(CL_HEATER_STATE_START);
									sv_cntrl_setpumpspeed(DCMOTOR2,900);
									sv_cntrl_setpumpspeed(DCMOTOR1,960);
									
									
									cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
									cl_bc_returncode = sv_cntrl_deactivate_valve(VALVE_ID19);
									cl_bc_returncode = Cl_mac_apprequesthandler(MACREQ_BC_OPENFILL_COMPLETED);
									if(cl_bc_returncode == CL_OK)
									{
										bc_laststate = bc_state;
										bc_state = CL_BC_STATE_BO1_V13V14_TRANSITION;
									}
								}
								break;
								case	BC_EVENT_CS:
								break;
								case BC_EVENT_STOP_FOR_BC1:
								openfillcounter = 0;
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
								if(cl_bc_returncode == CL_OK)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_STOPPED_FOR_BC1;
								}
								break;
								case BC_EVENT_STOP_FOR_BC2:
								openfillcounter = 0;
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V13_14);
								if(cl_bc_returncode == CL_OK)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_STOPPED_FOR_BC2;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_V15OPEN_TRANSITION:
							switch(cl_bc_event)
							{
								
								case BC_EVENT_500MS:
									bc_wait = true;
								
								break;
								case BC_EVENT_SECOND:
								if(bc_wait == true)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_V15OPEN;
									bc_wait = false;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_V15OPEN:
							switch(cl_bc_event)
							{
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
										
								break;
								case	BC_EVENT_CS:
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_V2OPEN_TRANSITION:
							switch(cl_bc_event)
							{
								
								case BC_EVENT_500MS:
								bc_wait = true;
								
								break;
								case BC_EVENT_SECOND:
								if(bc_wait == true)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_V2OPEN;
									bc_wait = false;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_V2OPEN:
							switch(cl_bc_event)
							{
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
								break;
								case	BC_EVENT_CS:
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_BO1_V4_TRANSITION:
							switch(cl_bc_event)
							{
								
								case BC_EVENT_500MS:
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_BO1_V4;
									bc_wait = false;
								break;
								case BC_EVENT_SECOND:
							//	if(bc_wait == true)
								{
							//		bc_laststate = bc_state;
							//		bc_state = CL_BC_STATE_BO1_V4;
							//		bc_wait = false;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_BO1_V4:
							switch(cl_bc_event)
							{
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
								break;
								case	BC_EVENT_CS:
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_BO2_V4_TRANSITION:
							switch(cl_bc_event)
							{
								
								case BC_EVENT_500MS:
								bc_wait = true;
								
								break;
								case BC_EVENT_SECOND:
								if(bc_wait == true)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_BO2_V4;
									bc_wait = false;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_BO2_V4:
							switch(cl_bc_event)
							{
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
								break;
								case	BC_EVENT_CS:
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_BO1_V13V14_TRANSITION:
							switch(cl_bc_event)
							{
								case BC_EVENT_50MS:
								fill_time++;
								if(wait_counter < 20)
								{
									if(wait_counter == 2)
									{
										sv_cntrl_deactivate_valve(VALVE_ID19);
									}
									wait_counter ++;	// 1 sec wait
									
								}
								else
								{
									wait_counter = 0;
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_BO1_V13V14;
									
								}
								break;
								case BC_EVENT_SECOND:
						
								break;
				
							}
			break;
			case CL_BC_STATE_BO1_V13V14:
							switch(cl_bc_event)
							{
								case BC_EVENT_50MS:
								fill_time++;
								break;
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
								
								break;
								case	BC_EVENT_CS:
									//switch_time1 = fill_time;
									current_sense = true;
									cl_tdata.word =0;
									cl_tdata.Twobyte = fill_time;
									cl_tdata.bytearray[2] = 22;
								Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V13_14);
								sv_cntrl_activate_valve(VALVE_ID19);
								
								if(cl_bc_returncode == CL_OK)
								{
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_BO2_V13V14_TRANSITION;	
									fill_time = 0;															
								}
								break;
								default:break;								
							}
			break;
			case CL_BC_STATE_BO2_V13V14_TRANSITION:

							switch(cl_bc_event)
							{
								case BC_EVENT_50MS:
								fill_time++;
								if(wait_counter < 20)
								{
									if(wait_counter == 2)
									{
										sv_cntrl_deactivate_valve(VALVE_ID19);
									}
									
									wait_counter ++;	// 1 sec wait
									
								}
								else
								{
									wait_counter = 0;
									bc_laststate = bc_state;
									bc_state = CL_BC_STATE_BO2_V13V14;
								}
								break;case BC_EVENT_SECOND:
								
								break;
								
							}
			break;
			case CL_BC_STATE_BO2_V13V14:
							switch(cl_bc_event)
							{
								case BC_EVENT_50MS:
								fill_time++;
								break;
								case 	BC_EVENT_500MS:
								break;
								case	BC_EVENT_SECOND:
								
								break;
								case	BC_EVENT_CS:
								//switch_time2 = fill_time;
								 current_sense = true;
									cl_tdata.word =0;
									cl_tdata.Twobyte = fill_time;
									cl_tdata.bytearray[2] = 21;
								Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
								sv_cntrl_activate_valve(VALVE_ID19);
								if(cl_bc_returncode == CL_OK)
								{
									bc_laststate = bc_state;
									fill_time = 0;
									bc_state = CL_BC_STATE_BO1_V13V14_TRANSITION;
								}
								break;
								default:break;
							}
			break;
			case CL_BC_STATE_STOPPED_FOR_BC1:
							switch(cl_bc_event)
							{
								case BC_EVENT_START_BC1:
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
								bc_state = CL_BC_STATE_BC1_TRANSITION;
								break;
								case BC_EVENT_START:
								cl_bc_returncode = sv_cntrl_setflowpath(BC_FLUSH_OPEN);
								bc_laststate = bc_state;
								bc_state = CL_BC_STATE_OPENFILL_TRANSITION;
								cl_bc_returncode = sv_cntrl_activate_valve(VALVE_ID19);
								break;
								default:break;
						
							}
						
			break;

			case CL_BC_STATE_STOPPED_FOR_BC2:
							switch(cl_bc_event)
							{
								case BC_EVENT_START_BC2:
								cl_bc_returncode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V13_14);
								bc_state = CL_BC_STATE_BC2_TRANSITION;
								break;
								case BC_EVENT_START:
								cl_bc_returncode = sv_cntrl_setflowpath(BC_FLUSH_OPEN);
								bc_laststate = bc_state;
								cl_bc_returncode = sv_cntrl_activate_valve(VALVE_ID19);
								bc_state = CL_BC_STATE_OPENFILL_TRANSITION;
								break;
								default:break;
								
							}
			break;
			case CL_BC_STATE_COMPLETED_BC1:
			break;
			case CL_BC_STATE_COMPLETED_BC2:
			break;
			default:break;
	}

	
	return 0;
}
