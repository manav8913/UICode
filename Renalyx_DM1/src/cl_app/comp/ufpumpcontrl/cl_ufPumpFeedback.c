/*
 * cl_ufPumpFeedback.c
 *
 * Created: 11/6/2014 9:04:07 AM
 *  Author: user
 */ 

#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "inc/cl_ufPumpFeedback.h"
#include "cl_app/inc/cl_types.h"

extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );


Cl_ReturnCodes cl_ufpumpFeedback_timer(void);
Cl_ReturnCodes cl_ufpumpFeedback_start(void);
Cl_ReturnCodes cl_ufpumpFeedback_stop(void);
Cl_ReturnCodes cl_ufpumpFeedback_get_delta(int16_t* delta);
Cl_ReturnCodes cl_ufpumpFeedback_get_state(cl_ufpump_states* pstate);


static uint16_t uf_expected_period = 0;
static cl_ufpump_states  cl_ufpump_state = CL_UFP_STATE_IDLE;
static uint16_t uf_rotataion_marker = 1,prev_uf_rotataion_marker = 1;
static uint16_t fivemscounter=0,uf_period = 0,uf_avg_period = 0, uf_freq_in_min = 0;
static uint16_t uf_rotation_counter = 0;
static bool count_state = false;

Cl_ReturnCodes cl_ufpumpFeedback_start(void) 
{
	cl_ufpump_state = CL_UFP_STATE_STARTED;
	return 0;
}
Cl_ReturnCodes cl_ufpumpFeedback_stop(void)
{
	
	return 0;
}
Cl_ReturnCodes cl_ufpumpFeedback_get_state(cl_ufpump_states* pstate)
{
	*pstate = cl_ufpump_state;
	return 0;
}
Cl_ReturnCodes cl_ufpumpFeedback_get_delta(int16_t* pdelta)
{
	*pdelta = uf_period - uf_expected_period;
	return 0;
}
Cl_ReturnCodes cl_ufpumpFeedback_set_expected_period(int16_t period)
{
	uf_expected_period = period;
	return 0;
}

Cl_ReturnCodes cl_ufpumpFeedback_timer(void) // 5 ms clock
{
	//cl_Datastreamtype cl_data;	
	//cl_testgetsensordata(UF_ROTATION_MARKER,&uf_rotataion_marker);
	Cl_SysStat_GetSensor_Status_Query(SENSOR_UFP_FB, &uf_rotataion_marker);

	fivemscounter++;
	if((cl_ufpump_state ==  CL_UFP_STATE_STARTED) && (uf_rotation_counter > 2))
	{
		cl_ufpump_state = CL_UFP_STATE_RUNNING;
	}
	
	//#if 0
	if(count_state == false)
	{
		if(uf_rotataion_marker == 0 )
		{
			if(prev_uf_rotataion_marker != uf_rotataion_marker)
			{
				count_state = true;
				//uf_rotation_counter++;
				fivemscounter = 0;
			}
		}
	}
	else
	{
		if(uf_rotataion_marker == 0 )
		{
			if(prev_uf_rotataion_marker != uf_rotataion_marker)
			{
				count_state = true;
				uf_rotation_counter++;
				uf_period = fivemscounter*5;
				
				if(cl_ufpump_state != CL_UFP_STATE_STARTED)
				{
					if(uf_period > (1.01 *uf_expected_period))
					{
						cl_ufpump_state = CL_UFP_STATE_RUNNING_SLOW;
					}
					else if(uf_period < (0.99 *uf_expected_period))
					{
						cl_ufpump_state = CL_UFP_STATE_RUNNING_FAST;
					}
					else
					{
						cl_ufpump_state = CL_UFP_STATE_RUNNING_NORMAL;
					}
				}

				
				uf_freq_in_min  = ( 60000) / uf_period;
				fivemscounter = 0;
				
			//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_PERIOD",9);
			//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&uf_period,2);
			//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&uf_expected_period,2);
				 
				
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_RPM",6);
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&uf_freq_in_min,2);
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&uf_rotation_counter,2);
				
				//	cl_data.Twobyte = uf_freq_in_min;
				//	dataarray[0] = CON_TX_PARAM_DATA_UF_RPM;
				//	dataarray[1] = cl_data.bytearray[0];
				//	dataarray[2] = cl_data.bytearray[1];
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_SYSDATA,&dataarray,3);
				
			}
		}
		
	}
	prev_uf_rotataion_marker = uf_rotataion_marker;
	
	return 0;
}
