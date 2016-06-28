/*
 * cl_heparinfeedback.c
 *
 * Created: 11/6/2014 9:05:08 AM
 *  Author: user
 */ 



#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "inc/cl_heparinfeedback.h"
#include "cl_app/inc/cl_types.h"

extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

extern uint8_t sv_cntrl_resetHepa_dir(void);
extern uint8_t sv_cntrl_setHepa_dir(void);
Cl_ReturnCodes cl_hep_pumpFeedback_timer(void);
Cl_ReturnCodes cl_hep_pumpFeedback_start(void);
Cl_ReturnCodes cl_hep_pumpFeedback_stop(void);
Cl_ReturnCodes cl_hep_pumpFeedback_get_delta(int16_t* delta);
Cl_ReturnCodes cl_hep_pumpFeedback_get_state(cl_hep_pump_states* pstate);
Cl_ReturnCodes cl_hep_pump_enddetction_timer(void); // 20 ms clock


static uint16_t hep_expected_period = 0;
static cl_hep_pump_states  cl_hep_pump_state = CL_HEP_P_STATE_IDLE;
static uint16_t hep_rotataion_marker = 1,prev_hep_rotataion_marker = 1;
static uint16_t fivemscounter=0,hep_period = 0,hep_avg_period = 0, hep_freq_in_min = 0;
static uint16_t hep_rotation_counter = 0;
static bool count_state = false;

static uint16_t pin_status = 1,hp_start_status = 0,prev_hp_start_status = 0,hp_end_status = 0,prev_hp_end_status = 0;
static uint16_t counter = 0;

Cl_ReturnCodes cl_hep_pumpFeedback_start(void)
{
	cl_hep_pump_state = CL_HEP_P_STATE_STARTED;
	return 0;
}
Cl_ReturnCodes cl_hep_pumpFeedback_stop(void)
{
	
	return 0;
}
Cl_ReturnCodes cl_hep_pumpFeedback_get_state(cl_hep_pump_states* pstate)
{
	*pstate = cl_hep_pump_state;
	return 0;
}
Cl_ReturnCodes cl_hep_pumpFeedback_get_delta(int16_t* pdelta)
{
	*pdelta = hep_period - hep_expected_period;
	return 0;
}
Cl_ReturnCodes cl_hep_pumpFeedback_set_expected_period(int16_t period)
{
	hep_expected_period = period;
	return 0;
}

Cl_ReturnCodes cl_hep_pumpFeedback_timer(void) // 5 ms clock
{

	Cl_SysStat_GetSensor_Status_Query(SENSOR_HP_FB, &hep_rotataion_marker);

	fivemscounter++;
	if((cl_hep_pump_state ==  CL_HEP_P_STATE_STARTED) && (hep_rotation_counter > 2))
	{
		cl_hep_pump_state = CL_HEP_P_STATE_RUNNING;
	}

	if(count_state == false)
	{
		if(hep_rotataion_marker == 0 )
		{
			if(prev_hep_rotataion_marker != hep_rotataion_marker)
			{
				count_state = true;
				//hep_rotation_counter++;
				fivemscounter = 0;
			}
		}
	}
	else
	{
		if(hep_rotataion_marker == 0 )
		{
			if(prev_hep_rotataion_marker != hep_rotataion_marker)
			{
				count_state = true;
				hep_rotation_counter++;
				hep_period = fivemscounter*5;
				
					
					//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&hep_period,2);
					//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&hep_expected_period,2);
				
				if(cl_hep_pump_state != CL_HEP_P_STATE_STARTED)
				{
					if(hep_period > (1.01 *hep_expected_period))
					{
						cl_hep_pump_state = CL_HEP_P_STATE_RUNNING_SLOW;
					}
					else if(hep_period < (0.99 *hep_expected_period))
					{
						cl_hep_pump_state = CL_HEP_P_STATE_RUNNING_FAST;
					}
					else
					{
						cl_hep_pump_state = CL_HEP_P_STATE_RUNNING_NORMAL;
					}
				}

				
				hep_freq_in_min  = ( 60000) / hep_period;
				fivemscounter = 0;

				
			}
		}
		
	}
	prev_hep_rotataion_marker = hep_rotataion_marker;
	

	
	return 0;
}

Cl_ReturnCodes cl_hep_pump_enddetction_timer(void) // 20 ms clock
{
		// START END  DETECTION
		{
			Cl_SysStat_GetSensor_Status_Query(SENSOR_HP_START,&hp_start_status);
			
			if(	prev_hp_start_status != hp_start_status)
			{
				prev_hp_start_status = hp_start_status;
				if(hp_start_status == 0)
				{
					sv_cntrl_resetHepa_dir();
					
				}
			}
			



			Cl_SysStat_GetSensor_Status_Query(SENSOR_HP_END,&hp_end_status);
			
			
			if(	prev_hp_end_status != hp_end_status)
			{
				prev_hp_end_status = hp_end_status;
				if(hp_end_status == 1)
				{
						sv_cntrl_setHepa_dir();
					
					//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_END",8);
					//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&hp_end_status,1);
				}
			}
			
		}
		
		return 0;
}