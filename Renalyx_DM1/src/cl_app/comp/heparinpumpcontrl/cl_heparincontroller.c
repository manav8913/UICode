/*
 * cl_heparincontroller.c
 *
 * Created: 11/5/2014 9:47:32 AM
 *  Author: user
 */ 
#include "cl_app/inc/cl_types.h"
#include "inc/cl_heparincontroller.h"
#include "Platform/Service/sv_interface.h"
#include "inc/cl_heparinfeedback.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes cl_hep_pumpFeedback_set_expected_period(int16_t period);
extern Cl_ReturnCodes cl_hep_pumpFeedback_start(void);

Cl_ReturnCodes cl_hep_controller(cl_hep_events hp_event , int32_t data);

extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t  sv_cntrl_setHepa_dir(void);
extern uint8_t sv_cntrl_resetHepa_dir(void);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t  sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern Cl_ReturnCodes cl_hep_pumpFeedback_get_state(cl_hep_pump_states* pstate);
extern Cl_ReturnCodes cl_hep_pumpFeedback_get_delta(int16_t* pdelta);
extern Cl_ReturnCodes cl_wait(uint32_t );
cl_hep_states hp_State = CL_HEP_STATE_IDLE;

static uint16_t cl_hep_bolusrate;
static uint32_t cl_hep_deliveryrate = 100;
static uint16_t cl_hep_seconds = 0;
static uint16_t cl_hep_minutes = 0;

Cl_ReturnCodes cl_hep_start_heparindelivery(void)
{
	
}
Cl_ReturnCodes cl_hep_set_heparin_deliveryrate(void)
{
	
}
Cl_ReturnCodes cl_hep_start_heparin_bolus(void)
{
	
}

Cl_ReturnCodes cl_hep_set_heparin_bolusrate(void)
{
	
}
Cl_ReturnCodes cl_hep_stop_heparindelivery(void)
{
	
}
Cl_ReturnCodes cl_hep_set_heparindeliveryperiod(void)
{
	
}
Cl_ReturnCodes cl_hep_set_heparinbolusperiod(void)
{
	
}

Cl_ReturnCodes cl_hep_controller(cl_hep_events hp_event , int32_t data)
{
	Cl_ReturnCodes cl_hep_retcode = CL_OK;
		cl_hep_pump_states hep_pumpstate = CL_HEP_STATE_IDLE;
		uint16_t hep_delta_value = 0 ;
		static uint32_t pwm_period = 0;
	
	switch(hp_State)
	{	
		case	CL_HEP_STATE_IDLE:
		switch(hp_event)
			{
				case CL_HEP_EVENT_STARTBOLUS:
					pwm_period =  cl_hep_deliveryrate ;
					sv_cntrl_setpumpspeed(HEPARINPUMP,pwm_period);
					cl_wait(100);
					sv_cntrl_setHepa_dir();
					cl_wait(100);
					sv_cntrl_activatepump(HEPARINPUMP);
				hp_State = CL_HEP_STATE_BOLUS_ON;
				break;
				case CL_HEP_EVENT_STOPBOLUS:
					cl_hep_retcode =  sv_cntrl_deactivatepump(HEPARINPUMP);
					hp_State = CL_HEP_STATE_IDLE;
				break;
				case CL_HEP_EVENT_START_NORMAL_DELIVERY:
						pwm_period =  cl_hep_deliveryrate ;
						sv_cntrl_setpumpspeed(HEPARINPUMP,pwm_period);
						cl_wait(100);
						sv_cntrl_setHepa_dir();
						cl_wait(100);
						sv_cntrl_activatepump(HEPARINPUMP);
					//	cl_hep_pumpFeedback_set_expected_period(cl_hep_deliveryrate);
					//	cl_hep_pumpFeedback_start();

				//hp_State = CL_HEP_STATE_NORMAL_DELIVERY_ON;

				break;
				case CL_HEP_EVENT_GO_TO_LEFTEND:
				hp_State = CL_HEP_STATE_MOVING_LEFT;
				break;
				case CL_HEP_EVENT_GO_TO_RIGHTEND:
				hp_State = CL_HEP_STATE_MOVING_RIGHT;
				break;
				case CL_HEP_EVENT_LEFT_END_REACHED:
				hp_State = CL_HEP_STATE_REACHED_LEFTEND;
				break;
				case CL_HEP_EVENT_RIGHT_END_REACHED:
				hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
				break;
				case CL_HEP_EVENT_SET_BOLUS_RATE:
				
				cl_hep_bolusrate = data;
				pwm_period =  cl_hep_bolusrate ;
				//sv_cntrl_setpumpspeed(HEPARINPUMP,pwm_period);
				break;
				case CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE:
				cl_hep_deliveryrate = data;
				pwm_period =  cl_hep_deliveryrate ;
				//sv_cntrl_setpumpspeed(HEPARINPUMP,pwm_period);
				
				break;
			}
		break;
		case	CL_HEP_STATE_REACHED_LEFTEND:
		switch(hp_event)
		{
			case CL_HEP_EVENT_STARTBOLUS:
			hp_State = CL_HEP_STATE_BOLUS_ON;
			break;
			case CL_HEP_EVENT_START_NORMAL_DELIVERY:
			hp_State = CL_HEP_STATE_NORMAL_DELIVERY_ON;
			break;
			case CL_HEP_EVENT_GO_TO_RIGHTEND:
			sv_cntrl_setHepa_dir();
			sv_cntrl_activatepump(HEPARINPUMP);
			hp_State = CL_HEP_STATE_MOVING_RIGHT;
			break;
			case CL_HEP_EVENT_LEFT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_LEFTEND;
			break;
			case CL_HEP_EVENT_RIGHT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
			break;
			case CL_HEP_EVENT_SET_BOLUS_RATE:
			cl_hep_bolusrate = data ;
			break;
			case CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE:
			cl_hep_deliveryrate = data;
			break;
		}
		break;
		case	CL_HEP_STATE_REACHED_RIGHTEND:
		switch(hp_event)
		{
			case CL_HEP_EVENT_STARTBOLUS:
			hp_State = CL_HEP_STATE_BOLUS_ON;
			break;
			case CL_HEP_EVENT_START_NORMAL_DELIVERY:
			hp_State = CL_HEP_STATE_NORMAL_DELIVERY_ON;
			break;
			case CL_HEP_EVENT_GO_TO_LEFTEND:
			sv_cntrl_resetHepa_dir();
			sv_cntrl_activatepump(HEPARINPUMP);
			hp_State = CL_HEP_STATE_MOVING_LEFT;
			break;
			case CL_HEP_EVENT_LEFT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_LEFTEND;
			break;
			case CL_HEP_EVENT_RIGHT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
			break;
			case CL_HEP_EVENT_SET_BOLUS_RATE:
			cl_hep_bolusrate = data ;
			break;
			case CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE:
			cl_hep_deliveryrate = data;
			break;
		}
		break;
		case CL_HEP_STATE_MOVING_LEFT:
		switch(hp_event)
		{
			case CL_HEP_EVENT_SEC_TICK:
			cl_hep_pumpFeedback_get_state(&hep_pumpstate);
			switch(hep_pumpstate)
			{
				case CL_HEP_P_STATE_READY_AT_START:
		////		sv_cntrl_setHepa_dir();
			//	sv_cntrl_deactivatepump(HEPARINPUMP);
			//	hp_State = CL_HEP_STATE_REACHED_LEFTEND;
				break;
			}
			break;

		}
		break;
		case	CL_HEP_STATE_READY_FORBOLUS:
		switch(hp_event)
		{
			case CL_HEP_EVENT_STARTBOLUS:
			hp_State = CL_HEP_STATE_BOLUS_ON;
			break;
			case CL_HEP_EVENT_GO_TO_LEFTEND:
			hp_State = CL_HEP_STATE_MOVING_LEFT;
			break;
			case CL_HEP_EVENT_GO_TO_RIGHTEND:
			hp_State = CL_HEP_STATE_MOVING_RIGHT;
			break;
			case CL_HEP_EVENT_LEFT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_LEFTEND;
			break;
			case CL_HEP_EVENT_RIGHT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
			break;
			case CL_HEP_EVENT_SET_BOLUS_RATE:
			cl_hep_bolusrate = data ;
			break;
			case CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE:
			cl_hep_deliveryrate = data;
			break;
		}
		break;
		case	CL_HEP_STATE_BOLUS_ON:
		switch(hp_event)
		{
			case CL_HEP_EVENT_SEC_TICK:
			if(cl_hep_seconds++ > 60)
			{
				cl_hep_seconds = 0;
				if(cl_hep_minutes++ > HEPARIN_BOLUS_TIMEOUT)
				{
					hp_State = CL_HEP_STATE_IDLE;
					cl_hep_minutes = 0;
					//Cl_mac_apprequesthandler(MACREQ_HEPARIN_BOLUS_COMPLETED);
				}
			}

			break;
			case CL_HEP_EVENT_LEFT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_LEFTEND;
			break;
			case CL_HEP_EVENT_RIGHT_END_REACHED:
			hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
			break;
			case CL_HEP_EVENT_SET_BOLUS_RATE:
			cl_hep_bolusrate = data ;
			break;
			case CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE:
			cl_hep_deliveryrate = data;
			break;
			case 	CL_HEP_EVENT_PAUSEBOLUS:
			hp_State = CL_HEP_STATE_PAUSED;
			break;
			case	CL_HEP_EVENT_STOPBOLUS:
			hp_State = CL_HEP_STATE_STOPPED;
			break;
		}
		break;
		case	CL_HEP_STATE_READY_FOR_DELIVERY:
		break;
		case	CL_HEP_STATE_NORMAL_DELIVERY_ON:
			switch(hp_event)
			{
				case CL_HEP_EVENT_SEC_TICK:

					cl_hep_pumpFeedback_get_state(&hep_pumpstate);
					cl_hep_pumpFeedback_get_delta(&hep_delta_value);
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_RPM",6);
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&hep_delta_value,2);
					switch(hep_pumpstate)
					{
						case CL_HEP_P_STATE_RUNNING_FAST:
							//apply correction
					//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_FAST",7);
						//	pwm_period = pwm_period + 5;
						//	sv_cntrl_setpumpspeed(HEPARINPUMP,pwm_period );
						break;
						case CL_HEP_P_STATE_RUNNING_SLOW:
							//apply correction
						//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_SLOW",7);
						//	pwm_period = pwm_period - 5;
						//	sv_cntrl_setpumpspeed(HEPARINPUMP,pwm_period);
						break;
						case CL_HEP_P_STATE_STOPPED_AT_END:
							//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_END",6);
							//sv_cntrl_deactivatepump(HEPARINPUMP);
							//hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
						break;
						default:break;
					}


				break;
				case CL_HEP_EVENT_LEFT_END_REACHED:
				cl_hep_retcode =  sv_cntrl_deactivatepump(HEPARINPUMP);
				hp_State = CL_HEP_STATE_REACHED_LEFTEND;
				break;
				case CL_HEP_EVENT_RIGHT_END_REACHED:
				cl_hep_retcode =  sv_cntrl_deactivatepump(HEPARINPUMP);
				hp_State = CL_HEP_STATE_REACHED_RIGHTEND;
				break;
				case CL_HEP_EVENT_SET_BOLUS_RATE:
				cl_hep_bolusrate = data ;
				break;
				case CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE:
				cl_hep_deliveryrate = data;
				break;
				case 	CL_HEP_EVENT_PAUSEBOLUS:
				hp_State = CL_HEP_STATE_PAUSED;
				break;
				case	CL_HEP_EVENT_STOPBOLUS:
				hp_State = CL_HEP_STATE_STOPPED;
				break;
			}
		
		break;
		case	CL_HEP_STATE_BOLUS_PAUSED:
		break;
		case	CL_HEP_STATE_BOLUS_STOPPED:
		break;
		case	CL_HEP_STATE_NORMAL_PAUSED:
		break;
		case	CL_HEP_STATE_NORMAL_STOPPED:
		break;
		case	CL_HEP_STATE_BOLUS_COMPLETED:
		break;
		case	CL_HEP_STATE_PAUSED:
		break;
		case	CL_HEP_STATE_STOPPED:
		break;

		case	CL_HEP_STATE_COMPLETED:
		break;
		default: break;
	}
	
}