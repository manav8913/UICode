/*
 * cl_bloodpumpFeedback.c
 *
 * Created: 11/6/2014 9:06:12 AM
 *  Author: user
 */ 

#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "inc/cl_bloodpumpFeedback.h"
#include "cl_app/inc/cl_types.h"

extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );


Cl_ReturnCodes cl_bp_pumpFeedback_timer(void);
Cl_ReturnCodes cl_bp_pumpFeedback_start(void);
Cl_ReturnCodes cl_bp_pumpFeedback_stop(void);
Cl_ReturnCodes cl_bp_pumpFeedback_get_delta(int16_t* delta);
Cl_ReturnCodes cl_bp_pumpFeedback_get_state(cl_bp_pump_states* pstate);


static uint16_t bp_expected_period = 0;
static cl_bp_pump_states  cl_bp_pump_state = CL_BP_P_STATE_IDLE;
static uint16_t bp_rotataion_marker = 1,prev_bp_rotataion_marker = 1;
static uint16_t fivemscounter=0,bp_period = 0,bp_avg_period = 0, bp_freq_in_min = 0;
static uint16_t bp_rotation_counter = 0;
static bool count_state = false;

Cl_ReturnCodes cl_bp_pumpFeedback_start(void)
{
	cl_bp_pump_state = CL_BP_P_STATE_STARTED;
	return 0;
}
Cl_ReturnCodes cl_bp_pumpFeedback_stop(void)
{
	
	return 0;
}
Cl_ReturnCodes cl_bp_pumpFeedback_get_state(cl_bp_pump_states* pstate)
{
	*pstate = cl_bp_pump_state;
	return 0;
}
Cl_ReturnCodes cl_bp_pumpFeedback_get_delta(int16_t* pdelta)
{
	*pdelta = bp_period - bp_expected_period;
	return 0;
}
Cl_ReturnCodes cl_bp_pumpFeedback_set_expected_period(int16_t period)
{
	bp_expected_period = period;
	return 0;
}

Cl_ReturnCodes cl_bp_pumpFeedback_timer(void) // 5 ms clock
{
	//cl_Datastreamtype cl_data;
	//cl_testgetsensordata(BP_ROTATION_MARKER,&bp_rotataion_marker);
	Cl_SysStat_GetSensor_Status_Query(SENSOR_BP_FB, &bp_rotataion_marker);

	fivemscounter++;
	if((cl_bp_pump_state ==  CL_BP_P_STATE_STARTED) && (bp_rotation_counter > 2))
	{
		cl_bp_pump_state = CL_BP_P_STATE_RUNNING;
	}
	
	//#if 0
	if(count_state == false)
	{
		if(bp_rotataion_marker == 0 )
		{
			if(prev_bp_rotataion_marker != bp_rotataion_marker)
			{
				count_state = true;
				//bp_rotation_counter++;
				fivemscounter = 0;
			}
		}
	}
	else
	{
		if(bp_rotataion_marker == 0 )
		{
			if(prev_bp_rotataion_marker != bp_rotataion_marker)
			{
				count_state = true;
				bp_rotation_counter++;
				bp_period = fivemscounter*100;
				
				if(cl_bp_pump_state != CL_BP_P_STATE_STARTED)
				{
					if(bp_period > (1.01 *bp_expected_period))
					{
				//		cl_bp_pump_state = CL_BP_P_STATE_RUNNING_SLOW;
					}
					else if(bp_period < (0.99 *bp_expected_period))
					{
				//		cl_bp_pump_state = CL_BP_P_STATE_RUNNING_FAST;
					}
					else
					{
				//		cl_bp_pump_state = CL_BP_P_STATE_RUNNING_NORMAL;
					}
				}

				
				bp_freq_in_min  = ( 60000) / bp_period;
				fivemscounter = 0;
				
				//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_PERIOD",9);
				//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&bp_period,2);
				//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&bp_expected_period,2);
				
				
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BP_RPM",6);
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&bp_freq_in_min,2);
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&bp_rotation_counter,2);
				
				//	cl_data.Twobyte = bp_freq_in_min;
				//	dataarray[0] = CON_TX_PARAM_DATA_BP_RPM;
				//	dataarray[1] = cl_data.bytearray[0];
				//	dataarray[2] = cl_data.bytearray[1];
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_SYSDATA,&dataarray,3);
				
			}
		}
		
	}
	prev_bp_rotataion_marker = bp_rotataion_marker;
	
	return 0;
}
