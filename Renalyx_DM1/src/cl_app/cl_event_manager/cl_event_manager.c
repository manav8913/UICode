/*
 * cl_event_manager.c
 *
 * Created: 3/9/2015 11:18:06 AM
 *  Author: user
 */ 
#include "inc/cl_event_manager.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/comp/levelswichcntrl/inc/cl_level_switchcontroller.h"
#include "cl_app/comp/flowswitchcntrl/inc/flowswitchcntrl.h"
#include "cl_app/comp/deaeration/inc/cl_deaeration.h"
#include "cl_app//cl_dprep/inc/cl_dprep_primecontroller.h"
#include "cl_app/cl_bc_cntrl//inc/Cl_bc_controller.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.c.h"
#include "cl_app/cl_status/comp/inc/Cl_temperature_sensor.h"
#include "cl_app/cl_status/comp/inc/Cl_PressureSensors.h"


extern Cl_ReturnCodes cl_ufpumpFeedback_timer(void);
extern Cl_ReturnCodes cl_hep_pumpFeedback_timer(void) ;
extern Cl_ReturnCodes cl_level_switchcontroller(levelswitchcntrl_event_type cntrl_event );
extern Cl_ReturnCodes cl_flow_switchcontroller(flowswitchcntrl_event_type cntrl_event);
extern Cl_ReturnCodes cl_dprep_primecontroller(Cl_Dprep_PrimeEvents prime_event , int16_t data);
extern Cl_ReturnCodes cl_deaeration_controller(cl_deaeration_event_type cntrl_event);
extern void Cl_SysStat_mean_status_update(void);
extern  Cl_ReturnCodes UpdateHeaterControls(void);
extern Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event);
extern Cl_ReturnCodes Cl_Heater_Event_Handler(HeaterEventsType HeaterEvent);
extern Cl_ReturnCodes  Cl_Temperature_Controller_Handler(Cl_Temp_EventType Cl_Tc_Event);
extern Cl_ReturnCodes  Cl_Pressure_Controller_Handler(Cl_PressureSensor_EventType Cl_Ps_Event);
extern Cl_ReturnCodes cl_bp_pumpFeedback_timer(void);
extern Cl_ReturnCodes Cl_Mac_CheckforAlarms(void);// check if there is any alarms , if there is one generate an event. ALSO This can me moved to respective sub  state handlers to be acted up on a much smaller time tick (on a 10- 100 ms event..???)
Cl_ReturnCodes cl_event_handler(MAC_EVENTS cl_event)
{
	
	switch(cl_event)
	{
		case EVT_TICK_INSTANT:
		cl_level_switchcontroller(LEVEL_SWITCH_CNTRLER_EVENT_INSTANT);
		cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_INSTANT);
		break;
		

		
		case EVT_TICK_5M:
				cl_ufpumpFeedback_timer();
				cl_hep_pumpFeedback_timer();
		
		break;
		case EVT_TICK_20M:
		//Cl_SysStat_mean_status_update();
		break;		
		case EVT_TICK_50M:
		 Cl_bc_controller(BC_EVENT_50MS);		
		break;
		case EVT_TICK_100M:
		Cl_Mac_CheckforAlarms();// check if there is any alarms , if there is one generate an event. ALSO This can me moved to respective sub  state handlers to be acted up on a much smaller time tick (on a 10- 100 ms event..???)
		Cl_Temperature_Controller_Handler(TS_EVENT_100MS);
		Cl_Pressure_Controller_Handler(TS_EVENT_100MS);
		cl_bp_pumpFeedback_timer();
		break;
		case EVT_TICK_500M:
		//UpdateHeaterControls();
		Cl_Heater_Event_Handler(CL_HEATER_EVENT_500MS);
		Cl_bc_controller(BC_EVENT_500MS);
		break;
		case EVT_TICK_SEC:
		
		cl_level_switchcontroller(LEVEL_SWITCH_CNTRLER_EVENT_SEC);
		cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_SEC);
		cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_TICK_SEC,0);
		Cl_bc_controller(BC_EVENT_SECOND);
		Cl_Heater_Event_Handler(CL_HEATER_EVENT_1SEC);
		Cl_SysStat_mean_status_update();
		break;
		case EVT_TICK_MIN:
		
		break;
		case EVT_ALERT_CS:
		Cl_bc_controller(BC_EVENT_CS);
		break;
		default:
		break;
	}
	
	return 0;
}
