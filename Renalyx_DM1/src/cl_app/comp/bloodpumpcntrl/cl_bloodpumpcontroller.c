/*
 * cl_bloodpumpcontroller.c
 *
 * Created: 11/6/2014 9:07:00 AM
 *  Author: user
 */ 


#include "cl_app/inc/cl_types.h"
#include "inc/cl_bloodpumpcontroller.h"
#include "inc/cl_bloodpumpFeedback.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"



cl_bp_states bp_state = CL_BP_STATE_IDLE;

static uint16_t cl_bp_rate = 0;
static uint16_t cl_bp_seconds = 0;
static uint16_t cl_bp_minutes = 0;

extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern Cl_ReturnCodes cl_bp_pumpFeedback_get_state(cl_bp_pump_states* pstate);
extern Cl_ReturnCodes cl_bp_pumpFeedback_stop(void);
extern Cl_ReturnCodes cl_bp_pumpFeedback_get_delta(int16_t* pdelta);

extern Cl_ReturnCodes cl_bp_pumpFeedback_start(void);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes cl_bp_pumpFeedback_set_expected_period(int16_t period);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

Cl_ReturnCodes cl_bp_controller(cl_bp_events bp_event , int16_t data)
{
	Cl_ReturnCodes cl_bp_retcode = CL_OK;
	cl_bp_pump_states bp_pumpstate = CL_BP_P_STATE_IDLE;
	uint16_t bp_delta_value = 0 ;
	static uint16_t pwm_period = 0;
	
	switch(bp_state)
	{
		case CL_BP_STATE_IDLE:
		switch (bp_event)
		{
			case CL_BP_EVENT_SET_BP_RATE:
			cl_bp_rate = data;
			pwm_period = 12 * cl_bp_rate ;
	//		sv_cntrl_setpumpspeed(BLOODPUMP,pwm_period);
			break;
			case CL_BP_EVENT_START:
			
		//	cl_bp_pumpFeedback_set_expected_period(cl_bp_rate);
			cl_bp_retcode =  sv_cntrl_activatepump(BLOODPUMP);
			cl_bp_retcode = Cl_SendDatatoconsole(CONT_TX_COMMAND_BLOODPUMP_ON,&data,0);
			
		//	cl_bp_pumpFeedback_start();
			bp_state = CL_BP_STATE_STARTED;
			break;
			default:break;
			
		}
		break;
		case CL_BP_STATE_STARTED:
		switch (bp_event)
		{
			case CL_BP_EVENT_SECOND:
	//		cl_bp_pumpFeedback_get_state(&bp_pumpstate);
	//		cl_bp_pumpFeedback_get_delta(&bp_delta_value);
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_RPM",6);
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&bp_delta_value,2);

			if(bp_pumpstate == CL_BP_P_STATE_RUNNING_FAST )
			{
				//apply correction
				//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_FAST",7);
			//	pwm_period = pwm_period + 100;
			//	sv_cntrl_setpumpspeed(BLOODPUMP,pwm_period );
			}
			else if (bp_pumpstate == CL_BP_P_STATE_RUNNING_SLOW )
			{
				//apply correction
				//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_SLOW",7);
			//	pwm_period = pwm_period - 100;
			//	sv_cntrl_setpumpspeed(BLOODPUMP,pwm_period);
			}
			else
			{
				
			}
			break;
			case CL_BP_EVENT_SET_BP_RATE:
			cl_bp_rate = data;
	//		sv_cntrl_setpumpspeed(BLOODPUMP,data);
			break;
			case CL_BP_EVENT_PAUSE:
			break;
			case CL_BP_EVENT_STOP:
			cl_bp_retcode =  sv_cntrl_deactivatepump(BLOODPUMP);
			cl_bp_retcode = Cl_SendDatatoconsole(CONT_TX_COMMAND_BLOODPUMP_OFF,&data,0);
			cl_bp_retcode =  cl_bp_pumpFeedback_stop();
			bp_state = CL_BP_STATE_STOPPED;
			break;
			case CL_BP_EVENT_COMPLETED:
			bp_state = CL_BP_STATE_IDLE;
			break;
			default:break;
			
		}
		break;
		case CL_BP_STATE_STOPPED:
		switch (bp_event)
		{
			case CL_BP_EVENT_SET_BP_RATE:
			cl_bp_rate = data;
	//		sv_cntrl_setpumpspeed(BLOODPUMP,data);
			break;
			case CL_BP_EVENT_START:
			cl_bp_retcode =  sv_cntrl_activatepump(BLOODPUMP);
			cl_bp_retcode = Cl_SendDatatoconsole(CONT_TX_COMMAND_BLOODPUMP_ON,&data,0);
			bp_state = CL_BP_STATE_STARTED;
			break;
			default:break;
			
		}
		break;
		case CL_BP_STATE_PAUSED:
		switch (bp_event)
		{
			case CL_BP_EVENT_SET_BP_RATE:
			cl_bp_rate = data;
	//		sv_cntrl_setpumpspeed(BLOODPUMP,data);
			break;
			case CL_BP_EVENT_START:
			cl_bp_retcode =  sv_cntrl_activatepump(BLOODPUMP);
			cl_bp_retcode = Cl_SendDatatoconsole(CONT_TX_COMMAND_BLOODPUMP_ON,&data,0);
			bp_state = CL_BP_STATE_STARTED;
			break;
			case CL_BP_EVENT_STOP:
			cl_bp_retcode =  sv_cntrl_deactivatepump(BLOODPUMP);
			bp_state = CL_BP_STATE_STOPPED;
			cl_bp_retcode = Cl_SendDatatoconsole(CONT_TX_COMMAND_BLOODPUMP_OFF,&data,0);
			break;
			default:break;
			
		}
		break;
		case CL_BP_STATE_COMPLETED:
		break;
		default:break;
		
		
	}
}