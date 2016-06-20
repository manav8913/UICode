/*
 * cl_heatercontroller.c
 *
 * Created: 10/31/2014 10:54:30 AM
 *  Author: user
 */ 

#include "inc/cl_heatercontroller.h""
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_testharness/inc/cl_th.h"
#include "cl_app/cl_bc_cntrl/inc/Cl_bc_controller.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_cal/calibration.h"
#define HEATER_CONSTANT 1380
#define TS2_REF 1300

extern uint8_t sv_cntrl_poweronheater(void);
extern uint8_t sv_cntrl_poweroffheater(void);
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint8_t sv_cntrl_incheater(int32_t dty_val);
extern Cl_ReturnCodes        Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event);
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t sv_cntrl_enable_loopback(void);
extern uint8_t sv_cntrl_disable_loopback(void);

extern bool heater_update_disble ;
extern bool cl_temp3_stable;
extern bool cl_temp3_range_stable;


Cl_ReturnCodes UpdateHeaterControls(void);

typedef struct
{
	uint16_t duty;
	float temp;
}heater_temp_historytype;

Cl_ReturnCodes cl_heater_heat(void);


static HeaterStateType HeaterState = CL_HEATER_STATE_INACTIVE, HeaterPrevState = CL_HEATER_STATE_INACTIVE;
float dummy1,dummy2,dummy3,dummy4,dummy5,dummy6,dummy7;
float temp_n,long_avg_duty = 0;

uint32_t heater_duty = 0x555;
static int16_t avg_duty = 0 ,fisrt_dip_duty=0, duty_min = 0,duty_max = 0,avg_duty_bak = 0,stable_avg_dty = 0,duty_temp = 0;
static float avg_heat_loss = 0,avg_hl_factor = 0,avgtmp1=0,avgtmp2=0,avgtmp3=0,longavg_temp2=0,longavg_temp3=0,avgtmp3max=0,prev_avgtmp2=0,avgtmp2max=0,avgtmp2min=0,ts2_ref = TS2_REF;
static int16_t  avg_rawtemp1 = 0,avg_rawtemp2 = 0, avg_rawtemp3 = 0, prev_avg_rawtemp2 = 0, prev_avg_rawtemp3 = 0,max_avgtemp2=0,min_avgtemp2=0,max_avgtemp3 = 0,min_avgtemp3=0,mid_meantemp2=0,mid_meantemp3 = 0,prev_mid_meantemp3 = 0;
static int8_t count = 0, sec_count = 0;
static heat_directiontype temp2avg_direction = UP,temp3avg_direction = UP,prev_temp2avg_direction = UP,prev_temp3avg_direction = UP,midtemp3_direction = UP;
static heat_directiontype temp2_tracking__direction = UP;
static ClRinseTs2State ts2history = CL_TS2_STATE_BELOW_THRESHOLD;
static HeaterSubStateType heater_substate = CL_HEATER_SUBSTATE_OFF,prev_heater_substate = CL_HEATER_SUBSTATE_OFF;
static HeaterDistributionSubStateType  DistributionSubState = CL_DISTRIB_HEATER_SUBSTATE_IDLE;
static float initial_temp1 =0,Ks = 1.1, Kf = 1.3 , kmean = 1.2,k_avg = 1.2 ,heat_rate_ambiance_comp = 0,heat_rate_dissipation_comp=0, temp2_rate = 0 ,temp2_prediction = 0, prev_avgtmp2_cel = 0, temp3_last_correction = 36;
static int16_t prev_temp1val = 0, prev_temp2val = 0, prev_temp3val = 0;
static float avgtmp1_cel =0, avgtmp2_cel = 0,longavgtmp2_cel=0, avgtmp3_cel = 0,long_avgtmp3_cel = 0, avgtmp2_target_cel = 36.5,prescribed_temp = 37,prev_pres_temp = 37, temp3_cel_backup = 37;
float temp1_celsius = 0,dummy =0;
static uint16_t sec_clock = 0,sec_clock_total = 0;
heater_temp_historytype heater_temp_history[10]; 
extern volatile float temprature_final_value_1,temprature_final_value_2,temprature_final_value_3;


Cl_ReturnCodes Cl_Heater_GetHeaterStableStatus(bool* Stable)
{
	if((avgtmp3_cel > prescribed_temp - 0.2) &&(avgtmp3_cel < prescribed_temp + 0.2))
	{
		*Stable =  cl_temp3_stable;
	}
	else
	{
		*Stable =  false;
	}
}
Cl_ReturnCodes Cl_Heater_Event_Handler(HeaterEventsType HeaterEvent)
{
	Cl_ReturnCodes Cl_heeater_retcode = CL_OK;

switch(HeaterEvent)
{
	
	case CL_HEATER_EVENT_500MS:
	UpdateHeaterControls();
	break;
	case CL_HEATER_EVENT_1SEC:

							
							
							if(!(sec_clock%20))
							{
								if(((temp3_cel_backup < avgtmp3_cel) &&(temp3_cel_backup > avgtmp3_cel - 0.3)) || ((temp3_cel_backup < avgtmp3_cel + 0.3)&&(temp3_cel_backup > avgtmp3_cel)))
								{
									cl_temp3_stable = true;
								}
								else
								{
									cl_temp3_stable =false;
								}
								if(((temp3_cel_backup < avgtmp3_cel) &&(temp3_cel_backup > avgtmp3_cel - 1)) || ((temp3_cel_backup < avgtmp3_cel + 1)&&(temp3_cel_backup > avgtmp3_cel)))
								{
									cl_temp3_range_stable = true;
								}
								else
								{
									cl_temp3_range_stable =false;
								}
								
								temp3_cel_backup = avgtmp3_cel;
							}
							
							
	break;
	case CL_HEATER_EVENT_TARGET_TEMP:
	break;
	case CL_HEATER_EVENT_ON:
	sec_clock  = 0;
	sec_clock_total = 0;
	cl_temp3_stable = false;
	break;
	default:break;
}
	
	return (Cl_heeater_retcode);
}

Cl_ReturnCodes SetHeaterState(HeaterStateType Param_HeaterState)
{
	Cl_ReturnCodes Cl_rinseretcode = CL_OK;
	int16_t temp;
	
	HeaterPrevState = HeaterState;
	HeaterState = Param_HeaterState;
	if(Param_HeaterState == CL_HEATER_STATE_OFF)
	{
		sv_cntrl_poweroffheater();
	}
		Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP1STATUS,&temp);
		initial_temp1 =  0.8056 * temp;
		uint32_t ret_temp;
		//initial_temp1 = 0.042 * initial_temp1 - 23.68;
		//y = 0.032x - 6.829
		//initial_temp1 = 0.032 * initial_temp1 - 6.829;
		/*ret_temp = 
		initial_temp1 = 0.0000116 * initial_temp1 *initial_temp1 + 0.0035 *initial_temp1 + 11.157;*/
		calibration_tmp(initial_temp1,TS1);
		initial_temp1 = temprature_final_value_1;
		
		 avg_duty = 16.607 * initial_temp1 * initial_temp1 - 1099  * initial_temp1 + 20028.0;
		 
		 avg_duty  = -84.24 * initial_temp1 + 5298;


		 avg_duty_bak = avg_duty;
		 
		 avg_duty = avg_duty ;
		 
		 	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP1STATUS,&temp);
		 	prev_temp1val =  0.8056 * temp;
		 	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&temp);
		 	prev_temp2val =  0.8056 * temp;
		 	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
		 	prev_temp3val =  0.8056 * temp;
		 	
	if (HeaterState == CL_HEATER_STATE_CLOSED_HEATING)
	{
		stable_avg_dty = avg_duty;
		avg_duty = 100;
		
	}
	if((HeaterPrevState == CL_HEATER_STATE_CLOSED_HEATING))
	{
		avg_duty = stable_avg_dty;
	}

	
	return (Cl_rinseretcode);
}

Cl_ReturnCodes UpdateHeaterControls(void)
{
	Cl_ReturnCodes Cl_rinseretcode = CL_OK;
	int16_t temp,temp1,heat_loss;
	float Tmp1Val=0,Tmp2Val = 0,Tmp3Val=0 ;
	
	if(HeaterState == CL_HEATER_STATE_OFF)
	{
		sv_cntrl_poweroffheater();
		return;
	}
	if((HeaterState == CL_HEATER_STATE_INACTIVE) || (HeaterState == CL_HEATER_SUBSTATE_OFF))
	{
		return;
	}

	//dummy1 = avg_duty;
	
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP1STATUS,&temp);
	Tmp1Val =  0.8056 * temp;
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&temp);
	Tmp2Val =  0.8056 * temp;
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
	Tmp3Val =  0.8056 * temp;
	
	
	
	if (sec_clock_total > 60)
	{
			avg_rawtemp1 = (avg_rawtemp1*10 + Tmp1Val)/11;
			avg_rawtemp2 = (avg_rawtemp2*10 + Tmp2Val)/11;
			avg_rawtemp3 = (avg_rawtemp3*10 + Tmp3Val)/11;
			
		if ((Tmp1Val -  prev_temp1val ) > 60) 
		{
			if(avg_rawtemp1 < prev_temp1val )
			{
				Tmp1Val = prev_temp1val;
				}
		}
		if ((  prev_temp1val - Tmp1Val ) > 60)
		{
			if(avg_rawtemp1 > prev_temp1val )
			{
				Tmp1Val = prev_temp1val;
				}
		}
		
		
		if ((Tmp2Val -  avgtmp2 ) > 30)
		{
			if(avg_rawtemp2 < prev_temp2val )
			{
				Tmp2Val = prev_temp2val;
				}
		}
		if ((  avgtmp2 - Tmp2Val ) > 30)
		{
			if(avg_rawtemp2 > prev_temp2val )
			{
				Tmp2Val = prev_temp2val;
				}
		}
		
		if ((Tmp3Val -  avgtmp3 ) > 30)
		{
			if(avg_rawtemp3 < prev_temp3val )
			{
				Tmp3Val = prev_temp3val;
				}
		}
		if ((  avgtmp3 - Tmp3Val ) > 30)
		
		{
			if(avg_rawtemp3 > prev_temp3val )
			{
				Tmp3Val = prev_temp2val;
				}
		}
	}
	
	prev_temp1val = Tmp1Val;
	prev_temp2val = Tmp2Val;
	prev_temp3val = Tmp3Val;
	
	avgtmp1 =	(avgtmp1*10 + Tmp1Val)/11;
	avgtmp2 =	(avgtmp2*14 + Tmp2Val)/15;
	avgtmp3 =	(avgtmp3*14 + Tmp3Val)/15;
	longavg_temp2 = (avgtmp2 * 29  + Tmp2Val)/30;
	longavg_temp3 = (avgtmp3 * 29  + Tmp3Val)/30;
	prev_temp2avg_direction = temp2avg_direction;
	prev_temp3avg_direction = temp3avg_direction;
	

		//avgtmp1_cel = 0.0000116 * avgtmp1 *avgtmp1 + 0.0035 *avgtmp1 + 11.157;

		/*float temp_n;
		temp_n = avgtmp3/1000;
		avgtmp3_cel =1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;
									temp_n = avgtmp1/1000;
									avgtmp1_cel = 1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;
									dummy = 0.0000116 * avgtmp2 *avgtmp2 + 0.0035 *avgtmp2 + 11.157;*/
		calibration_tmp(avgtmp1,TS1);
		avgtmp1_cel = temprature_final_value_1;
		calibration_tmp(avgtmp2,TS2);
		dummy = temprature_final_value_2;
		calibration_tmp(avgtmp3,TS3);
		avgtmp3_cel = temprature_final_value_3;
		temp2_prediction = avgtmp2_cel + ( dummy - avgtmp2_cel) * 20;
		avgtmp2_cel = dummy;
		
		avgtmp2_cel = dummy;
		calibration_tmp(longavg_temp2,TS2);
		longavgtmp2_cel = temprature_final_value_2;
		calibration_tmp(longavg_temp3,TS3);
		long_avgtmp3_cel = temprature_final_value_3;
		
		if(avgtmp2_cel > 45)
		{
			//sv_cntrl_incheater(avg_duty );
			sv_cntrl_poweroffheater();
			return;
		}
	if (prev_avg_rawtemp2 < avg_rawtemp2)
	{
		
		temp2avg_direction = UP;
		//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UP",2);
	}else
	{
		temp2avg_direction = DOWN;
		//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DOWN",4);
	}
	if (prev_avg_rawtemp3 < avg_rawtemp3)
	{
		
		temp3avg_direction = UP;
		
	}else
	{
		temp3avg_direction = DOWN;
	}
	prev_avg_rawtemp2 = avg_rawtemp2;
	prev_avg_rawtemp3 = avg_rawtemp3;
	
	if(max_avgtemp2 < avg_rawtemp2)
	{
		max_avgtemp2 = avg_rawtemp2;
	}
	if(max_avgtemp3 < avg_rawtemp3)
	{
		max_avgtemp3 = avg_rawtemp3;
	}
	if(min_avgtemp2 > avg_rawtemp2)
	{
		min_avgtemp2 = avg_rawtemp2;
	}
	if(min_avgtemp3 > avg_rawtemp3)
	{
		min_avgtemp3 = avg_rawtemp3;
	}

	
	if((temp2avg_direction == DOWN) &&(prev_temp2avg_direction == UP))
	{
		max_avgtemp2 = avg_rawtemp2;
	}
	if((temp3avg_direction == DOWN) &&(prev_temp3avg_direction == UP))
	{
		max_avgtemp3 = avg_rawtemp3;
	}
	if((temp2avg_direction == UP) &&(prev_temp2avg_direction == DOWN))
	{
		min_avgtemp2 = avg_rawtemp2;
	}
	if((temp3avg_direction == UP) &&(prev_temp3avg_direction == DOWN))
	{
		min_avgtemp3 = avg_rawtemp3;
	}
	
	prev_mid_meantemp3 = mid_meantemp3;
	mid_meantemp3 = ( max_avgtemp3 + min_avgtemp3 )/2;
	mid_meantemp2 = ( max_avgtemp2 + min_avgtemp2 )/2;
	
	if (prev_mid_meantemp3 > mid_meantemp3)
	{
		midtemp3_direction = DOWN;
		
	}else
	{
		midtemp3_direction = UP;
	}

	if(HeaterState == CL_HEATER_STATE_ON)
	{
		if  ((( avgtmp2 > Tmp2Val) && ( avgtmp2 - Tmp2Val) < 30) || (( avgtmp2 < Tmp2Val) && ( Tmp2Val - avgtmp2  ) > 30))
		{
			if ((( avgtmp3 > Tmp3Val) && ( avgtmp3 - Tmp3Val) < 30) || (( avgtmp3 < Tmp3Val) && ( Tmp3Val - avgtmp3  ) > 30))
			{
				if ((( avgtmp1 > Tmp1Val) && ( avgtmp1 - Tmp1Val) < 30) || (( avgtmp1 < Tmp3Val) && ( Tmp1Val - avgtmp2  ) > 30))
				{
					
					HeaterState = CL_HEATER_STATE_ON_RAMPING;
					Cl_bc_controller(BC_EVENT_STOP);
					sv_cntrl_incheater(avg_duty );
					sv_cntrl_poweronheater();
					
					sv_cntrl_deactivate_valve(VALVE_ID1);
					Cl_bc_controller(BC_EVENT_RESUME);
					sv_cntrl_activatepump(DCMOTOR1);
					sv_cntrl_activatepump(DCMOTOR2);
					sv_cntrl_enable_loopback();
					sv_cntrl_setpumpspeed(DCMOTOR2,1300);
									
									
					
					heater_substate = CL_HEATER_SUBSTATE_CLOSED_HEATING;
					Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_HEATER_STATE,(void*)&heater_substate,1);
					prev_heater_substate = CL_HEATER_SUBSTATE_OFF;
					//avg_duty = avg_duty + 200;
					avgtmp2 = Tmp2Val;
					avgtmp3 = Tmp3Val;
					avg_rawtemp2 = Tmp2Val;
					avg_rawtemp3 = Tmp3Val;
					max_avgtemp2 = Tmp2Val;
					max_avgtemp3 = Tmp3Val;
					min_avgtemp2 = Tmp2Val;
					min_avgtemp3 = Tmp3Val;
				}
				
			}
			
		}
		
 

	}
	
	
	if (HeaterState == CL_HEATER_STATE_ON_RAMPING)
	{
		
		//when it becomes stable change the state 

		
	}
	
	
	
	switch (heater_substate)
	{
		case CL_HEATER_SUBSTATE_OFF:
		break;
		case CL_HEATER_SUBSTATE_DISTRIBUTION:
		case CL_HEATER_SUBSTATE_CLOSED_HEATING:
		case CL_HEATER_SUBSTATE_INITIALRAMPING:
		case CL_HEATER_SUBSTATE_RAMPINGUP:
		case CL_HEATER_SUBSTATE_RAMPINGDN:
		case CL_HEATER_SUBSTATE_THROWBACK:
		case CL_HEATER_SUBSTATE_THROWBACKRECOVERY:
		case CL_HEATER_SUBSTATE_STEP_36:
		case CL_HEATER_SUBSTATE_TRACKING:
		case CL_HEATER_SUBSTATE_TRACKING_1:
		cl_heater_heat();
		break;
		default: break;
	}
}




Cl_ReturnCodes cl_heater_heat(void)
{
	int16_t temp,temp1;
	float percent_duty = 0;
	int16_t Tmp2Val1 = 0;
	Cl_ReturnCodes Cl_rinseretcode = CL_OK;
	static int16_t prev_temp = 0;
	static bool imm_action = false;
	static bool imm_action_window = true;
	
	static heat_directiontype correction_dir  = NUETRAL;
	static sec_count = 0;
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&temp);
	Tmp2Val1 =  0.8056 * temp;
	
	sec_count++;
	sec_clock++;
	sec_clock_total++;
	
	
	temp1 = Tmp2Val1 - ts2_ref;
	//temp1 = (Tmp2Val1 + prev_temp)/2  - ts2_ref;
	
	//temp1 = avgtmp2 - ts2_ref;
	prev_temp = Tmp2Val1;
	if(long_avg_duty == 0)
	{
		long_avg_duty = avg_duty;
	}else
	{
		long_avg_duty = (39*long_avg_duty + avg_duty )/40;
	}
	
	if(!(sec_clock_total %5))
	{
		uint16_t temp = long_avg_duty;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
	}
	
	if ((temp1 > 100 ) || (ts2_ref > HEATER_CONSTANT + 60))
	{
		// temperature above 37.5 shut off the heater

		

	}
	#if 0
	cl_Datastreamtype cl_tdata;
		cl_tdata.word = 0;
		//cl_tdata.Twobyte = sensordata;
		//cl_tdata.Twobyte = mid_meantemp2;
		cl_tdata.Twobyte = avgtmp1;
		cl_tdata.bytearray[2] = 4;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
		cl_tdata.word = 0;
		//cl_tdata.Twobyte = sensordata;
		//cl_tdata.Twobyte = mid_meantemp2;
		cl_tdata.Twobyte = avgtmp2;
		cl_tdata.bytearray[2] = 5;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
		cl_tdata.word = 0;
		//cl_tdata.Twobyte = sensordata;
		//cl_tdata.Twobyte = mid_meantemp2;
		cl_tdata.Twobyte = avgtmp3;
		cl_tdata.bytearray[2] = 6;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			
		cl_tdata.word = 0;
		//cl_tdata.Twobyte = sensordata;
		cl_tdata.Twobyte = avg_duty;
		cl_tdata.bytearray[2] = 7;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);		
	
	
		cl_tdata.word = 0;
		cl_tdata.Twobyte = 	avgtmp2max ;
		cl_tdata.bytearray[2] = 8;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
	

	
		cl_tdata.word = 0;
		cl_tdata.Twobyte = avgtmp2min;
		cl_tdata.bytearray[2] = 9;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);	
		
		cl_tdata.word = 0;
		//cl_tdata.Twobyte = 100 + heat_rate_ambiance_comp* 10;
		cl_tdata.Twobyte = sec_clock_total;
		cl_tdata.bytearray[2] = 10;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
		cl_tdata.word = 0;
		cl_tdata.Twobyte = 10 *initial_temp1;
		cl_tdata.bytearray[2] = 11;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
				
		cl_tdata.word = 0;
		cl_tdata.Twobyte = kmean * 100;
		cl_tdata.bytearray[2] = 12;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
						
		cl_tdata.word = 0;
		cl_tdata.Twobyte = avgtmp2_target_cel * 10;
		cl_tdata.bytearray[2] = 13;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
		cl_tdata.word = 0;
		cl_tdata.Twobyte = k_avg * 100;
		cl_tdata.bytearray[2] = 14;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);	
		
		cl_tdata.word = 0;
		cl_tdata.Twobyte = Kf * 100;
		cl_tdata.bytearray[2] = 15;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
		
		cl_tdata.word = 0;
		cl_tdata.Twobyte = Ks * 100;
		cl_tdata.bytearray[2] = 16;
		Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		
	if( heater_update_disble == true)
	{
		return;
	}
		
	//cl_tdata.word = 0;
	///cl_tdata.Twobyte = 10 * (0.032 * avgtmp2 - 6.829); 
	////cl_tdata.bytearray[2] = 2;
	
	// Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
						
	cl_tdata.word = 0;
	//cl_tdata.Twobyte = sensordata;
	//
	cl_tdata.Twobyte = 10 * (0.032 * avgtmp2 - 6.829);
	cl_tdata.bytearray[2] = 100;
	Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
	cl_tdata.word = 0;
	//cl_tdata.Twobyte = sensordata;
	//cl_tdata.Twobyte = duty_max;
//	cl_tdata.Twobyte = heat_rate_ambiance_comp + 200;
	cl_tdata.Twobyte = kmean * 100;
	cl_tdata.bytearray[2] = 101;
	Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
	
#endif
	

	switch(heater_substate)
	{
		case CL_HEATER_SUBSTATE_CLOSED_HEATING:
		//	sv_cntrl_deactivate_valve(VALVE_ID1);
			
		//	sv_cntrl_deactivate_valve(VALVE_ID6);
		//	sv_cntrl_deactivate_valve(VALVE_ID8);
					/*temp_n = avgtmp3/1000;
					avgtmp3_cel =1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;
			//avgtmp2_cel = (0.0000116 * avgtmp2 *avgtmp2 + 0.0035 *avgtmp2 + 11.157 -1  );
			temp_n = avgtmp2/1000;
			avgtmp2_cel = 1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;*/
			calibration_tmp(avgtmp3,TS3);
			avgtmp3_cel = temprature_final_value_3;
			calibration_tmp(avgtmp2,TS2);
			avgtmp2_cel = temprature_final_value_2;		
			if(avgtmp2_cel > prescribed_temp-1  &&  avgtmp2_cel < prescribed_temp)
			{
				if(((avgtmp2_cel - longavgtmp2_cel ) > 0.5) && !(sec_count % 20))
				{
					avg_duty = avg_duty - 25;
				//	avg_duty = avg_duty + 100;
				//	avg_duty = avg_duty + (avgtmp2_cel - avgtmp1_cel ) * 60;
				}
			}
			if(avgtmp2_cel > prescribed_temp) 
			{
				sv_cntrl_disable_loopback(); // not reqd as we were never in a loop back. can be commeneted
				heater_substate = CL_HEATER_SUBSTATE_DISTRIBUTION;
				Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_HEATER_STATE,(void*)&heater_substate,1);
				DistributionSubState = CL_DISTRIB_HEATER_SUBSTATE_FIRST_PEAK;
				//heater_substate = CL_HEATER_SUBSTATE_INITIALRAMPING;
			//	sv_cntrl_deactivate_valve(VALVE_ID1);
			//	Cl_bc_controller(BC_EVENT_RESUME);
			//	sv_cntrl_activatepump(DCMOTOR1);
			//	sv_cntrl_activatepump(DCMOTOR2);
			//	sv_cntrl_activate_valve(VALVE_ID16);
			//	sv_cntrl_enable_loopback();
			//	sv_cntrl_deactivate_valve(VALVE_ID18);
			//	sv_cntrl_setpumpspeed(DCMOTOR2,1300);
				sec_clock =0;
				sec_count = 0;

				heater_temp_history[0].temp = avgtmp2_cel;
				//avg_duty = avg_duty + 100;
			}
		break;
		case CL_HEATER_SUBSTATE_DISTRIBUTION:
						/*	temp_n = avgtmp3/1000;
							avgtmp3_cel =1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;
					dummy3 = avgtmp3_cel;
										temp_n = avgtmp2/1000;
										avgtmp2_cel = 1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;*/
		calibration_tmp(avgtmp3,TS3);
		avgtmp3_cel = temprature_final_value_3;
		dummy3 = avgtmp3_cel;
		calibration_tmp(avgtmp2,TS2);
		avgtmp2_cel = temprature_final_value_2;
	 
						//#if 0	
						if (avgtmp2_cel < prescribed_temp )
						{
							
							DistributionSubState = CL_DISTRIB_HEATER_SUBSTATE_FIRST_DIP;
						//	sv_cntrl_enable_loopback();
							if(imm_action_window == true)
							{
								fisrt_dip_duty = avg_duty;
								avg_duty = long_avg_duty;
								imm_action = true;
								imm_action_window = false;
							}
			
						}
						if ((avgtmp2_cel> prescribed_temp ) && (imm_action_window == true))
						{
							if(DistributionSubState == CL_DISTRIB_HEATER_SUBSTATE_FIRST_DIP)
							{
								DistributionSubState = CL_DISTRIB_HEATER_SUBSTATE_SECOND_PEAK; // again 'first peak' we are marking for testing
								//sv_cntrl_disable_loopback();
								//avg_duty = fisrt_dip_duty + 100;
							}else
							{
							//	if(DistributionSubState == CL_DISTRIB_HEATER_SUBSTATE_SECOND_PEAK)
								
							}
						}
						if(!(sec_count % 20))
						{
							imm_action_window = true;
															uint8_t* pdataarray;
															int16_t temp1;
															cl_Datastreamtype cl_tdata;
															cl_tdata.word =0;
															cl_tdata.Twobyte = long_avg_duty;
															cl_tdata.bytearray[2] = 23;
															Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
															cl_tdata.word =0;
															cl_tdata.Twobyte = longavgtmp2_cel * 10;
															cl_tdata.bytearray[2] = 24;
															Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
															
															cl_tdata.word =0;
															cl_tdata.Twobyte = avgtmp2_target_cel * 10;
															cl_tdata.bytearray[2] = 13;
															Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
							
						}
						heat_rate_ambiance_comp = prescribed_temp - avgtmp2_cel ;
						
						if((avgtmp2_cel > prescribed_temp-1) && (avgtmp2_cel < prescribed_temp))
						{ 
							if(avgtmp2_cel - longavgtmp2_cel  > 0.25)
							{
								heat_rate_ambiance_comp = 0;
							}
						}
						else if(avgtmp2_cel < prescribed_temp-1)
						{
							if(avgtmp2_cel - longavgtmp2_cel  > 1)
							{
								heat_rate_ambiance_comp = 0;
							}
						}
						
						if((avgtmp2_cel > prescribed_temp) && (avgtmp2_cel < prescribed_temp+1))
						{ 
							if(longavgtmp2_cel - avgtmp2_cel    > 0.25)
							{
								heat_rate_ambiance_comp = 0;
							}
						}
						else if(avgtmp2_cel > prescribed_temp +1)
						{
							if(longavgtmp2_cel - avgtmp2_cel  > 1)
							{
								heat_rate_ambiance_comp = 0;
							}
						}					
						
						if( (!(sec_clock%40)  || (imm_action == true)))
							{
								
								imm_action = false;
								
								if((heat_rate_ambiance_comp < 0.5) && (heat_rate_ambiance_comp > -0.5))
								{
									avg_duty = avg_duty + heat_rate_ambiance_comp * 60;
								}
								else
								{
									avg_duty = avg_duty + heat_rate_ambiance_comp * 80;
								}
								
								if(avg_duty > 3500)
								{
									avg_duty = 3500;
								}
								sv_cntrl_incheater(avg_duty );
								sv_cntrl_poweronheater();
							//	sec_clock = 0;
								//sv_cntrl_incheater(avg_duty );
								//sv_cntrl_poweronheater();
								//sec_clock = 0;
								sec_count = 0;
							}
							else
							{
								if((heat_rate_ambiance_comp < 0.075) && (heat_rate_ambiance_comp > -0.075))
								{
									duty_temp = avg_duty + 0.5 * heat_rate_ambiance_comp * ( 40 - sec_count);
									sv_cntrl_incheater(duty_temp  );
								}
								else
								{	duty_temp = avg_duty +  heat_rate_ambiance_comp * ( 40 - sec_count);
									sv_cntrl_incheater(duty_temp  );
								}
								
								
							}
							//#endif
		//	if ((sec_clock > 480) ) // to be uncommented
			{
					//heat_rate_ambiance_comp = 37 - avgtmp3_cel ;
			//		if(cl_temp3_range_stable )
			
			if(avgtmp2_cel > prescribed_temp)
					{ 
						//if (( sec_clock > 360  ) || (DistributionSubState == CL_DISTRIB_HEATER_SUBSTATE_SECOND_PEAK))
						if (( sec_clock > 240  ) && (DistributionSubState == CL_DISTRIB_HEATER_SUBSTATE_SECOND_PEAK))
						{
							cl_temp3_range_stable = false;
							heat_rate_ambiance_comp =  avgtmp2_cel - avgtmp3_cel;
							avgtmp2_target_cel = prescribed_temp + heat_rate_ambiance_comp;
							//avg_duty = avg_duty_bak;
							
							if(avg_duty > 3500)
							{
								avg_duty = 3500;
							}
							
							
							sv_cntrl_incheater(avg_duty );
							sv_cntrl_poweronheater();
							sec_clock = 0;
							sec_count = 0;
							imm_action = false;
							sv_cntrl_deactivate_valve(VALVE_ID16);
							sv_cntrl_activate_valve(VALVE_ID18);
							sv_cntrl_activate_valve(VALVE_ID1);
							sv_cntrl_disable_loopback();
							prev_pres_temp = prescribed_temp;
							heater_substate = CL_HEATER_SUBSTATE_TRACKING;
							Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_HEATER_STATE,(void*)&heater_substate,1);
						}
						
					}
	
			}

		break;

		case CL_HEATER_SUBSTATE_TRACKING:
		
					/*temp_n = avgtmp3/1000;
					avgtmp3_cel =1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;
					temp_n = avgtmp2/1000;
			avgtmp2_cel = 1.830098345 * temp_n *temp_n +  22.62524406 *temp_n + 0.581851503  ;*/
			
			calibration_tmp(avgtmp3,TS3);
			calibration_tmp(avgtmp2,TS2);
			
			avgtmp3_cel = temprature_final_value_3;
			avgtmp2_cel = temprature_final_value_2;
																uint8_t* pdataarray;
																int16_t temp_5;
																temp_5 = avgtmp2_cel * 10;
																pdataarray = (uint8_t *)"D=";
															//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,2);
															//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp_5,2);
		//	dummy2 = avgtmp2_cel;
			dummy3 = avgtmp3_cel;
			dummy4 = avgtmp2_cel;
			
			dummy5 = avgtmp2_target_cel;
			
			
			if((temp2_tracking__direction == UP) && (avgtmp3_cel >= prescribed_temp + 0.1   ))
						{
							if(avgtmp2_target_cel < longavgtmp2_cel)
							{
													uint8_t* pdataarray;
													int16_t temp1;
													if(imm_action_window == true)
													{
														imm_action = true;
														imm_action_window = false;
													}
													
													sec_count = 0;
								//avgtmp2_target_cel = (4 * avgtmp2_cel + avgtmp2_target_cel)/5;
												//	avgtmp2_target_cel = avgtmp2_cel ;
													avgtmp2_target_cel = longavgtmp2_cel ;
													temp1 = avgtmp2_target_cel * 10;
													pdataarray = (uint8_t *)"B=";
													Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,2);
													Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
							}
							else
							{
										avgtmp2_target_cel = longavgtmp2_cel ;
							}
							
						}
						if((temp2_tracking__direction == DOWN) && (avgtmp3_cel <= prescribed_temp - 0.1   ))
						{
							if(avgtmp2_target_cel > longavgtmp2_cel)
							{
								uint8_t* pdataarray;
								int16_t temp1;
								if(imm_action_window == true)
								{
									imm_action = true;
									imm_action_window = false;
								}
					
								sec_count = 0;
								//avgtmp2_target_cel = (4 * avgtmp2_cel + avgtmp2_target_cel)/5;
								//	avgtmp2_target_cel = avgtmp2_cel ;
								avgtmp2_target_cel = longavgtmp2_cel ;
								temp1 = avgtmp2_target_cel * 10;
								pdataarray = (uint8_t *)"B=";
								Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,2);
								Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
							}
							else
							{
								avgtmp2_target_cel = longavgtmp2_cel ;
							}
				
						}
						
							if( !(sec_count % 5))
							{
								imm_action_window = true;
							}
							heat_rate_ambiance_comp = avgtmp2_target_cel - longavgtmp2_cel ;
							
							if( (!(sec_clock%10) ) || imm_action == true)
							{
								uint8_t* pdataarray;
								int16_t temp1;
								cl_Datastreamtype cl_tdata;
									cl_tdata.word =0;
									cl_tdata.Twobyte = long_avg_duty;
									cl_tdata.bytearray[2] = 23;
								Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
									cl_tdata.word =0;
									cl_tdata.Twobyte = longavgtmp2_cel * 10;
									cl_tdata.bytearray[2] = 24;
								Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
								
								cl_tdata.word =0;
								cl_tdata.Twobyte = avgtmp2_target_cel * 10;
								cl_tdata.bytearray[2] = 13;
								Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
								imm_action = false;
								temp1 = avgtmp2_target_cel * 10;
;
												
								
								//dummy4 = heat_rate_ambiance_comp;
								if((heat_rate_ambiance_comp < 0.15) && (heat_rate_ambiance_comp > -0.15))
								{
									if(((avgtmp2 < Tmp2Val1) && (avgtmp2_cel < avgtmp2_target_cel)) || ((avgtmp2 > Tmp2Val1) && (avgtmp2_cel > avgtmp2_target_cel)))
									{
										
									}
									else
									{
										avg_duty = long_avg_duty;
										avg_duty = avg_duty + heat_rate_ambiance_comp * 5;
										
									}
									
								}
								else if((heat_rate_ambiance_comp < 0.3) && (heat_rate_ambiance_comp > -0.3))
								{
									if(((avgtmp2 < Tmp2Val1) && (avgtmp2_cel < avgtmp2_target_cel)) || ((avgtmp2 > Tmp2Val1) && (avgtmp2_cel > avgtmp2_target_cel)))
									{
										
									}
									else
									{
										avg_duty = long_avg_duty;
										avg_duty = avg_duty + heat_rate_ambiance_comp * 10;
										
									}
									
								}
								else if((heat_rate_ambiance_comp < 0.5) && (heat_rate_ambiance_comp > -0.5))
								{
									if(((avgtmp2 < Tmp2Val1) && (avgtmp2_cel < avgtmp2_target_cel)) || ((avgtmp2 > Tmp2Val1) && (avgtmp2_cel > avgtmp2_target_cel)))
									{
										
									}
									else  
									{
										avg_duty = long_avg_duty;
										avg_duty = avg_duty + heat_rate_ambiance_comp * 20;
										
									}
									
								}
								else if((heat_rate_ambiance_comp < 1) && (heat_rate_ambiance_comp > 1))
								{
									avg_duty = long_avg_duty;
									avg_duty = avg_duty + heat_rate_ambiance_comp * 20;
									
								}
								
								else
								{
									avg_duty = avg_duty + heat_rate_ambiance_comp * 40;
								}
								//avg_duty = avg_duty + heat_rate_ambiance_comp * 80;
								if(avg_duty > 4000)
								{
									avg_duty = 4000;
								}
								
								if( avg_duty > avg_duty_bak * 1.2)
								{
									avg_duty = avg_duty_bak * 1.2;
									avg_duty_bak = avg_duty;
								}
								
								if(avg_duty < 100)
								{
									avg_duty = 100;
								}
								sv_cntrl_incheater(avg_duty  );
								sv_cntrl_poweronheater();
								sec_count = 0;
							//	sec_clock = 0;
								//sv_cntrl_incheater(avg_duty );
								//sv_cntrl_poweronheater();
								//sec_clock = 0;
								dummy7 = avg_duty;
							}
							else
							{
								if((heat_rate_ambiance_comp < 0.075) && (heat_rate_ambiance_comp > -0.075))
								{
									duty_temp = avg_duty + 2 * heat_rate_ambiance_comp * ( 5 - sec_count);
									sv_cntrl_incheater(duty_temp  );
								}
								else
								{	duty_temp = avg_duty + 5 * heat_rate_ambiance_comp * ( 5 - sec_count);
									sv_cntrl_incheater(duty_temp  );
								}
								
								
							}
						dummy6 = sec_clock;
						if ((sec_clock > 240) )
						{
								 if((cl_temp3_stable == 1) || (((temp2_tracking__direction == UP) && (avgtmp3_cel > prescribed_temp - 0.2 ) )||((temp2_tracking__direction == DOWN) && (avgtmp3_cel < prescribed_temp + 0.2 )) ))
								{
									cl_temp3_stable = false;
									heat_rate_ambiance_comp = prescribed_temp - long_avgtmp3_cel ;
									//dummy6 = heat_rate_ambiance_comp;
									
									
									avgtmp2_target_cel = longavgtmp2_cel + heat_rate_ambiance_comp;
									if(avgtmp2_target_cel > longavgtmp2_cel)
									{
										temp2_tracking__direction = UP;
									}
									else{
										temp2_tracking__direction = DOWN;
									}
									uint8_t* pdataarray;
									int16_t temp1;
																						
									//avgtmp2_target_cel = (4 * avgtmp2_cel + avgtmp2_target_cel)/5;
									
									temp1 = avgtmp2_target_cel * 10;
									pdataarray = (uint8_t *)"C=";
									Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,2);
									Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);

									sec_clock = 0;
									
								}

						}
				
			
		break;

		default:break;
	}
	
	return (Cl_rinseretcode);
	
}


