/*
 * UFPumpController.c
 *
 * Created: 11/5/2014 3:36:33 PM
 *  Author: user
 */ 


#include "cl_app/inc/cl_types.h"
#include "inc/cl_ufPumpController.h"
#include "inc/cl_ufPumpFeedback.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"



cl_uf_states uf_state = CL_UF_STATE_IDLE;

static uint16_t cl_uf_rate = 0;
static uint16_t cl_uf_seconds = 0;
static uint16_t cl_uf_minutes = 0;

extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern Cl_ReturnCodes cl_ufpumpFeedback_get_state(cl_ufpump_states* pstate);
extern Cl_ReturnCodes cl_ufpumpFeedback_stop(void);
extern Cl_ReturnCodes cl_ufpumpFeedback_get_delta(int16_t* pdelta);

extern Cl_ReturnCodes cl_ufpumpFeedback_start(void);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes cl_ufpumpFeedback_set_expected_period(int16_t period);

Cl_ReturnCodes cl_uf_controller(cl_uf_events uf_event , int16_t data)
{
	Cl_ReturnCodes cl_uf_retcode = CL_OK;
	cl_ufpump_states uf_pumpstate = CL_UFP_STATE_IDLE;
	uint16_t uf_delta_value = 0 ;
	static uint16_t pwm_period = 0;
	
	switch(uf_state)
	{
			case CL_UF_STATE_IDLE:
			switch (uf_event)
				{
					case CL_UF_EVENT_SET_UF_RATE:
					cl_uf_rate = data;
					pwm_period =  cl_uf_rate ;
					sv_cntrl_setpumpspeed(UFPUMP,pwm_period);
					break;
					case CL_UF_EVENT_START:
					
					cl_ufpumpFeedback_set_expected_period(cl_uf_rate);
					cl_uf_retcode =  sv_cntrl_activatepump(UFPUMP);
					cl_ufpumpFeedback_start();
					uf_state = CL_UF_STATE_STARTED;
					break;
					default:break;
					
				}
			break;
			case CL_UF_STATE_STARTED:
			switch (uf_event)
			{
				case CL_UF_EVENT_SECOND:
				cl_ufpumpFeedback_get_state(&uf_pumpstate);
				cl_ufpumpFeedback_get_delta(&uf_delta_value);
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_RPM",6);
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&uf_delta_value,2);

				if(uf_pumpstate == CL_UFP_STATE_RUNNING_FAST )
				{
					//apply correction
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_FAST",7);
				//	pwm_period = pwm_period + 100;
				//	sv_cntrl_setpumpspeed(UFPUMP,pwm_period );
				}
				else if (uf_pumpstate == CL_UFP_STATE_RUNNING_SLOW )
				{
					//apply correction
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_SLOW",7);
				//	pwm_period = pwm_period - 100;
				//	sv_cntrl_setpumpspeed(UFPUMP,pwm_period);
				}
				else
				{
					
				}
				break;
				case CL_UF_EVENT_SET_UF_RATE:
					cl_uf_rate = data;
					sv_cntrl_setpumpspeed(UFPUMP,data);
				break;
				case CL_UF_EVENT_PAUSE:
				break;
				case CL_UF_EVENT_STOP:
				cl_uf_retcode =  sv_cntrl_deactivatepump(UFPUMP);
				cl_uf_retcode =  cl_ufpumpFeedback_stop();
				uf_state = CL_UF_STATE_STOPPED;
				break;
				case CL_UF_EVENT_COMPLETED:
				uf_state = CL_UF_STATE_IDLE;
				break;
				default:break;
				
			}
			break;
			case CL_UF_STATE_STOPPED:
			switch (uf_event)
			{
				case CL_UF_EVENT_SET_UF_RATE:
					cl_uf_rate = data;
					sv_cntrl_setpumpspeed(UFPUMP,data);
				break;
				case CL_UF_EVENT_START:
				cl_uf_retcode =  sv_cntrl_activatepump(UFPUMP);
				uf_state = CL_UF_STATE_STARTED;
				break;
				default:break;
				
			}
			break;
			case CL_UF_STATE_PAUSED:
			switch (uf_event)
			{
				case CL_UF_EVENT_SET_UF_RATE:
					cl_uf_rate = data;
					sv_cntrl_setpumpspeed(UFPUMP,data);
				break;
				case CL_UF_EVENT_START:
				cl_uf_retcode =  sv_cntrl_activatepump(UFPUMP);
				uf_state = CL_UF_STATE_STARTED;
				break;
				case CL_UF_EVENT_STOP:
				cl_uf_retcode =  sv_cntrl_deactivatepump(UFPUMP);
				uf_state = CL_UF_STATE_STOPPED;
				break;
				default:break;
				
			}
			break;
			case CL_UF_STATE_COMPLETED:
			break;
			default:break;
			
			
	}
}