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
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.h"
#include "cl_app/cl_status/comp/inc/Cl_temperature_sensor.h"
#include "cl_app/cl_status/comp/inc/Cl_PressureSensors.h"
#include "cl_app/cl_status/comp/inc/Cl_Conductivity_Sensors.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app\cl_status\inc\cl_status.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"


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
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern Cl_ReturnCodes cl_wait(uint32_t);
extern uint8_t sv_cs_setpotvalue(uint16_t);
extern void sv_prop_stopmixing();
extern void sv_prop_startmixing();
extern Cl_ReturnCodes  Cl_Conductivity_Sensor_Controller(Cl_CondSensor_EventType Cl_cond_Event);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern void DD_RETRY_IIC(voidn);
extern Cl_ReturnCodes cl_datamamanager(MAC_EVENTS cl_data_event);

extern volatile int func_timer ;
extern uint32_t write_count , read_count , success_count ,fail_count, clock_count  ,reset_count ;
extern uint32_t bp_write_count , bp_read_count , bp_success_count ,bp_fail_count;
extern volatile uint32_t g_ul_ms_ticks ;


static test_counter =0;
Cl_ReturnCodes cl_event_handler(MAC_EVENTS cl_event)
{
	
	static volatile bool toggle = false;
	
	switch(cl_event)
	{
		case EVT_TICK_INSTANT:
		cl_level_switchcontroller(LEVEL_SWITCH_CNTRLER_EVENT_INSTANT);
		cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_INSTANT);

		//	sv_cs_setpotvalue(250);
		
		break;
		

		
		case EVT_TICK_5M:
				cl_ufpumpFeedback_timer();
				cl_hep_pumpFeedback_timer();
				


		break;
		case EVT_TICK_20M:
		#if 0
				if(func_timer > 10)
				{
					//sv_prop_startmixing();
				}
				//sv_cs_setpotvalue(225);
				if( toggle == false)
				{
					toggle = true;
					sv_cntrl_activatepump(BLOODPUMP);
					sv_prop_startmixing();
				//	sv_cs_setpotvalue(225);
				}
				else
				{
					sv_cntrl_deactivatepump(BLOODPUMP);
					sv_prop_stopmixing();
				//	sv_cs_setpotvalue(225);
					toggle = false;
				}
			
		#endif
	//	#if 0

			
		//	#endif
		//Cl_SysStat_mean_status_update();

//		if( toggle == false)
{
	//	sv_cntrl_activatepump(BLOODPUMP);
	//	sv_cs_setpotvalue(250);
//	sv_cntrl_activatepump(BLOODPUMP);
//	sv_cs_setpotvalue(225);
//	sv_cntrl_deactivatepump(BLOODPUMP);
//	sv_prop_stopmixing();
//	sv_cs_setpotvalue(450);
//	sv_cs_setpotvalue(550);
//	sv_cs_setpotvalue(250);
	;


//	toggle = true;
}
//else
{

//		sv_cntrl_activatepump(BLOODPUMP);
	//	sv_cs_setpotvalue(250);
	//	sv_cntrl_activatepump(BLOODPUMP);
//	sv_cs_setpotvalue(225);
//	sv_cntrl_deactivatepump(BLOODPUMP);
//	sv_prop_stopmixing();
//	sv_cs_setpotvalue(450);
//	sv_cs_setpotvalue(550);
//	sv_cs_setpotvalue(250);


//	toggle = false;
}
		
	
	
		break;		
		case EVT_TICK_50M:
		
		
					
		 Cl_bc_controller(BC_EVENT_50MS);		
		break;
		case EVT_TICK_100M:
		Cl_Mac_CheckforAlarms();// check if there is any alarms , if there is one generate an event. ALSO This can me moved to respective sub  state handlers to be acted up on a much smaller time tick (on a 10- 100 ms event..???)
		Cl_Temperature_Controller_Handler(TS_EVENT_100MS);
		Cl_Pressure_Controller_Handler(PS_EVENT_100MS);
		cl_bp_pumpFeedback_timer();
		
		#if 0
		uint32_t init_ticks =0, spent_ticks=0;
		init_ticks = g_ul_ms_ticks;


				if( toggle == false)
				{
					toggle = true;
				//	sv_cs_setpotvalue(225);
					sv_cntrl_activatepump(BLOODPUMP);
					sv_prop_startmixing();
					//sv_cs_setpotvalue(550);
				}
				else
				{
				//	sv_cs_setpotvalue(225);
					sv_cntrl_deactivatepump(BLOODPUMP);
					sv_prop_stopmixing();
					toggle = false;
				}
				spent_ticks = 	g_ul_ms_ticks - init_ticks;	
			#endif
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
		Cl_Conductivity_Sensor_Controller(COND_EVENT_1SEC);
		Cl_SysStat_mean_status_update();
		DD_RETRY_IIC();

		test_counter++;
		
		
		break;
		case EVT_TICK_MIN:
		{
			
		
		cl_Datastreamtype cl_tdata;
		uint16_t t_wr_cnt,t_succ_cnt,t_fail_cnt,t_bpfail_cnt;
		
		if (write_count > 65535)
		{
			t_wr_cnt = write_count % 65535;
			
		}
		else {t_wr_cnt = write_count ;}
			
		
		if (success_count > 65535)
		{
			t_succ_cnt = success_count % 65535;
			
		}	else {t_succ_cnt = success_count;}	
			
		if (fail_count > 65535)
		{
			t_fail_cnt = fail_count % 65535;
			
		}	else {t_fail_cnt = fail_count;}
			
		if (bp_fail_count > 65535)
		{
			t_bpfail_cnt = bp_fail_count % 65535;
			
		} else {t_bpfail_cnt = bp_fail_count;}
			
		cl_tdata.word =0;
		cl_tdata.Twobyte = t_wr_cnt;
		cl_tdata.bytearray[2] = 25;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		cl_tdata.word =0;
		cl_tdata.Twobyte = t_succ_cnt;
		cl_tdata.bytearray[2] = 26;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		cl_tdata.word =0;
		cl_tdata.Twobyte = t_fail_cnt;
		cl_tdata.bytearray[2] = 27;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		cl_tdata.word =0;
		cl_tdata.Twobyte = t_bpfail_cnt;
		cl_tdata.bytearray[2] = 28;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
	
		}
		break;
		case EVT_ALERT_CS:
		Cl_bc_controller(BC_EVENT_CS);
		break;
		
		case EVT_CONSOLE_COMMAND_GET_DATA:
		case EVT_CONSOLE_COMMAND_SET_DATA:
		cl_datamamanager(cl_event);
		break;
		default:
		break;
	}
	
	return 0;
}
