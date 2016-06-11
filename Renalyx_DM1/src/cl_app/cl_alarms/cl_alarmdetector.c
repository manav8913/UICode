/*
 * cl_alarmdetector.c
 *
 * Created: 1/22/2014 10:02:29 AM
 *  Author: user
 */ 

#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/comp/levelswichcntrl/inc/cl_level_switchcontroller.h"
#include "cl_app/comp/flowswitchcntrl/inc/flowswitchcntrl.h"
#include "cl_app/cl_alarms/comp/inc/cl_conductivity_detector.h"
#include "cl_app/cl_cal/calibration.h"



extern Cl_Mac_EventType Cl_MacEvent1;
extern Cl_Sys_statusType cl_sys_statbuffer;
extern bool gflow_en;
extern bool TEST_STUB_ENABLEGATE;

extern Levelswitch_msgType Levelswitch_msg;
extern Flowswitch_msgType Flowswitch_msg;

static uint32_t prev_flowstatus = 0;
static uint32_t flow_watchdog = 0,flow_count=0,flow_count_rate = 0;
Cl_AlarmStructType Cl_alarms_alarms[ALARM_MAX_ID];
Cl_AlarmIdType  cl_lastalarmid;
static levelswitchprevstatus = 1;
extern  uint32_t flowstatus_off_alarm_count = 0;// testing
Cl_AlarmIdType cond_alarm = CL_COND_ALARM_NO_ALARM;
Cl_AlarmIdType dac_cond_alarm = CL_DAC_COND_ALARM_NO_ALARM;
bool cl_alarm_triggered = false;
int16_t ps3_threshold = 4096;
Cl_AlarmThresholdType  Cl_alarmThresholdTable;

	float apt , prev_aptval = 0;
	float vpt = 0 , prev_vptval = 0;
	float ps1 = 0 , prev_ps1val = 0;
	static float ps2 = 0, prev_ps2val = 0;
	float ps3 = 0, prev_ps3val = 0;
	float temp1 =0,prev_temp1val = 0;
	float temp2 = 0,prev_temp2val = 0 ;
	float temp3 = 0 , prev_temp3val = 0;
	
	float cond1 = 0;
	
		
Cl_ReturnCodes Cl_Init_Alarms(void);

Cl_ReturnCodes Cl_alarms_scanforAlarms(void);
Cl_ReturnCodes Cl_translate_sensor_values(void);
Cl_ReturnCodes Cl_LoadAlarmThresholdTable(void);
Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_AlarmIdType, bool  );
Cl_ReturnCodes Cl_AlarmMuteAlarms(Cl_AlarmIdType cl_alarm_id);
Cl_ReturnCodes Cl_GetLastAlarm(Cl_AlarmIdType cl_alarm_id);
Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_AlarmIdType  , bool* );
Cl_ReturnCodes Cl_UpdateMacAlarmEventTable(void);
Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_AlarmIdType cl_alarm_id );
Cl_ReturnCodes Cl_Alarm_TriggerDummyAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status);
Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status);
Cl_ReturnCodes Cl_Alarm_timmer_50millisec(void);
Cl_ReturnCodes Cl_AlarmConfigureAlarmType(Cl_AlarmIdType,Cl_AlarmTriggerType,uint16_t,uint16_t,uint8_t);




extern uint8_t DD_SET_VALVE(sv_valvetype );// sanjeer -testing
extern uint8_t DD_RESET_VALVE(sv_valvetype );
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes cl_get_dac_conductivity_info(Sys_dac_cond_alarmtype* );
extern Cl_ReturnCodes cl_get_conductivity_info(Sys_cond_alarmtype* );
extern Cl_ReturnCodes Cl_Temperature_Controller_Init(void);
extern Cl_ReturnCodes  Cl_Pressure_Controller_Init(void);
extern volatile float temprature_final_value_1,temprature_final_value_2,temprature_final_value_3;
extern volatile int16_t pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;

Cl_ReturnCodes Cl_Init_Alarms(void)
{
	Cl_LoadAlarmThresholdTable();
	Cl_Temperature_Controller_Init();
	Cl_Pressure_Controller_Init();
}

Cl_ReturnCodes Cl_LoadAlarmThresholdTable(void)
{
		Cl_alarmThresholdTable.cond_low_threshold = 0;
		Cl_alarmThresholdTable.cond_high_threshold = 10000;
		Cl_alarmThresholdTable.cond_dac_high_threshold = 0;
		Cl_alarmThresholdTable.cond_dac_low_threshold = 10000;
		Cl_alarmThresholdTable.apt_high_threshold = 500;
		Cl_alarmThresholdTable. apt_low_threshold = -300;
		Cl_alarmThresholdTable.vpt_high_threshold = 500;
		Cl_alarmThresholdTable.vpt_low_threshold = -300;
		Cl_alarmThresholdTable.ps1_high_threshold = 1500;
		Cl_alarmThresholdTable.ps1_low_threshold = -300;
		Cl_alarmThresholdTable.ps2_high_threshold = 1500;
		Cl_alarmThresholdTable.ps2_low_threshold = -300;
		Cl_alarmThresholdTable.ps3_high_threshold = 1500;
		Cl_alarmThresholdTable.ps3_low_threshold = -300;
		Cl_alarmThresholdTable.temp1_high_threshold = 40;
		Cl_alarmThresholdTable.temp1_low_threshold = 5;
		Cl_alarmThresholdTable.temp2_high_threshold = 41;
		Cl_alarmThresholdTable.temp2_low_threshold = 5;
		Cl_alarmThresholdTable.temp3_high_threshold = 38;
		Cl_alarmThresholdTable.temp3_low_threshold = 3;
		Cl_alarmThresholdTable.heater_high_temp_threshold = 45;
		Cl_alarmThresholdTable.hp_high_speed_threshold = 10000;
		Cl_alarmThresholdTable.hp_low_speed_threshold = 0;
		Cl_alarmThresholdTable.bp_high_speed_threshold = 10000;
		Cl_alarmThresholdTable.bp_low_speed_threshold = 0;
		Cl_alarmThresholdTable.uf_high_speed_threshold = 10000;
		Cl_alarmThresholdTable.uf_low_speed_threshold = 0;
		Cl_alarmThresholdTable.flow_low_threshold = 5;
		Cl_alarmThresholdTable.flow_high_threshold = 20;
		Cl_alarmThresholdTable.ls_toggle_high_threshold = 30;
		Cl_alarmThresholdTable.ls_toggle_low_threshold = 5;
		Cl_alarmThresholdTable.bc_toggle_high_threshold = 30;
		Cl_alarmThresholdTable.bc_toggle_low_threshold = 5;
}
Cl_ReturnCodes Cl_translate_sensor_values(void)
{							
	
	float temp;
	
	#if 0
	if ((temp1 -  prev_temp1val ) > 1)
	{
		temp1 = prev_temp1val;
	}
	if ((  prev_temp1val - temp1 ) > 1)
	{
		temp1 = prev_temp1val;
	}
	
	
	if ((temp2 -  prev_temp2val ) > 1)
	{
		temp2 = prev_temp2val;
	}
	if ((  prev_temp2val - temp2 ) > 1)
	{
		temp2 = prev_temp2val;
	}
	if ((  prev_temp3val - temp3 ) > 1)
	{
		temp3 = prev_temp3val;
	}
	if ((  prev_aptval - apt ) > 1)
	{
		apt = prev_aptval;
	}
	
	if ((  prev_vptval - vpt ) > 200)
	{
		vpt = prev_vptval;
	}
	if ((  prev_ps1val - ps1 ) > 200)
	{
		ps1 = prev_ps1val;
	}
	if ((  prev_ps2val - ps2 ) > 200)
	{
		ps2 = prev_ps2val;
	}
	if ((  prev_ps3val - ps3 ) > 200)
	{
		ps3 = prev_ps3val;
	}
	#endif
	temp = ((cl_sys_statbuffer.ps1status* 3300 /4096) ) ;
	calibration_ps1(temp);
	ps1 =  pressure_final_ps1;
	
	temp = ((cl_sys_statbuffer.ps2status* 3300 /4096) ) ;
	calibration_ps2(temp);
	ps2 = ((ps2* 49) + pressure_final_ps2)/50;

	temp = ((cl_sys_statbuffer.ps3status* 3300 /4096) ) ;
	calibration_ps3(temp);
	ps3 =  pressure_final_ps3;

	temp = ((cl_sys_statbuffer.aptstatus * 3300 /4096) ) ;
	calibration_apt(temp);
	apt =  pressure_final_apt;

	temp = ((cl_sys_statbuffer.vptstatus * 3300 /4096) ) ;
	calibration_vpt(temp);
	vpt =  pressure_final_vpt;

	temp = cl_sys_statbuffer.Temp1status * 0.805;
	calibration_tmp(temp,TS1);
	temp1 = temprature_final_value_1;
	
	temp = cl_sys_statbuffer.Temp2status * 0.805;
	calibration_tmp(temp,TS2);
	temp2 = temprature_final_value_2;
	
	temp = cl_sys_statbuffer.Temp3status * 0.805;
	calibration_tmp(temp,TS3);
	temp3 = temprature_final_value_3;
				

}
Cl_ReturnCodes Cl_alarms_scanforAlarms(void)
{
	Cl_ReturnCodes Cl_alarms_retcode;
	
	 Cl_translate_sensor_values();
	
	
	if(Cl_alarms_alarms[ABDSTATUS_ON].cl_is_enabled)
	{
		if(cl_sys_statbuffer.abdstatus)
		{
			Cl_alarms_alarms[ABDSTATUS_ON].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=ABDSTATUS_ON; 
			Cl_alarms_alarms[ABDSTATUS_ON].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = ABDSTATUS_ON;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
		}
	
	}
	if(Cl_alarms_alarms[BDSTATUS_ON].cl_is_enabled)
	{
		if(cl_sys_statbuffer.bdstatus)
		{
			Cl_alarms_alarms[BDSTATUS_ON].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=BDSTATUS_ON; 
			Cl_alarms_alarms[BDSTATUS_ON].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = BDSTATUS_ON;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
			
		}
	}
	if(Cl_alarms_alarms[BLDSTATUS_ON].cl_is_enabled)
	{
				if(cl_sys_statbuffer.bldstatus)
				{
					Cl_alarms_alarms[BLDSTATUS_ON].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=BLDSTATUS_ON; 
					Cl_alarms_alarms[BLDSTATUS_ON].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = BLDSTATUS_ON;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
	}
	if(Cl_alarms_alarms[APTSTATUS_HIGH].cl_is_enabled)
	{
				if(apt > Cl_alarmThresholdTable.apt_high_threshold)
				{
					Cl_alarms_alarms[APTSTATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=APTSTATUS_HIGH; 
					Cl_alarms_alarms[APTSTATUS_HIGH].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = APTSTATUS_HIGH;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
	}
	if(Cl_alarms_alarms[VPTSTATUS_HIGH].cl_is_enabled)
	{
				if(vpt > Cl_alarmThresholdTable.vpt_high_threshold)
				{
						Cl_alarms_alarms[VPTSTATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;
						Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=VPTSTATUS_HIGH; 
						Cl_alarms_alarms[VPTSTATUS_HIGH].cl_detected_count++;
						cl_alarm_triggered = true;
						cl_lastalarmid = VPTSTATUS_HIGH;
						Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
						Cl_UpdateMacAlarmEventTable();
					
				}
	}

	if(Cl_alarms_alarms[VPTSTATUS_LOW].cl_is_enabled)
	{
		if(vpt < Cl_alarmThresholdTable.vpt_low_threshold)
		{
			Cl_alarms_alarms[VPTSTATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=VPTSTATUS_LOW;
			Cl_alarms_alarms[VPTSTATUS_LOW].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = VPTSTATUS_HIGH;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
			
		}
	}

	if(Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_is_enabled)
	{
		
		
				if(((Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_triggertype == LOGIC_HIGH) && (cl_sys_statbuffer.bloodpumpdoor )) || ((Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_triggertype == LOGIC_LOW) && (!cl_sys_statbuffer.bloodpumpdoor )) )
				{
					if(Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_alarmstate != CL_ALARM_DETECTED)
					{
						
						//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DOOR",4);
					Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=BLOODDOOR_STATUS_OPEN; 
					Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = BLOODDOOR_STATUS_OPEN;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();	
					
					}

				}
				else
				{

					Cl_alarms_alarms[BLOODDOOR_STATUS_OPEN].cl_alarmstate = CL_ALARM_INACTIVE;

					
		
				}
				
		
	
	}
	if(Cl_alarms_alarms[HOLDER1STATUS_OPEN].cl_is_enabled)
	{
		
		
				if(((Cl_alarms_alarms[HOLDER1STATUS_OPEN].cl_triggertype == LOGIC_HIGH) && (cl_sys_statbuffer.holder1status )) || ((Cl_alarms_alarms[HOLDER1STATUS_OPEN].cl_triggertype == LOGIC_LOW) && (!cl_sys_statbuffer.holder1status )) )
				{
					Cl_alarms_alarms[HOLDER1STATUS_OPEN].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=HOLDER1STATUS_OPEN; 
					Cl_alarms_alarms[HOLDER1STATUS_OPEN].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = HOLDER1STATUS_OPEN;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
				}
				else
				{

					Cl_alarms_alarms[HOLDER1STATUS_OPEN].cl_alarmstate = CL_ALARM_INACTIVE;				
		
				}
					
	}
	
	if(Cl_alarms_alarms[HOLDER1STATUS_CLOSED].cl_is_enabled)
	{
		
		
		if(((Cl_alarms_alarms[HOLDER1STATUS_CLOSED].cl_triggertype == LOGIC_HIGH) && (cl_sys_statbuffer.holder1status )) || ((Cl_alarms_alarms[HOLDER1STATUS_CLOSED].cl_triggertype == LOGIC_LOW) && (!cl_sys_statbuffer.holder1status )) )
		{
			Cl_alarms_alarms[HOLDER1STATUS_CLOSED].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=HOLDER1STATUS_OPEN;
			Cl_alarms_alarms[HOLDER1STATUS_CLOSED].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = HOLDER1STATUS_CLOSED;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
		}
		else
		{

			Cl_alarms_alarms[HOLDER1STATUS_CLOSED].cl_alarmstate = CL_ALARM_INACTIVE;
			
		}
		
	}
	
	
if(Cl_alarms_alarms[HOLDER2STATUS_OPEN].cl_is_enabled)
{
			
		if(((Cl_alarms_alarms[HOLDER2STATUS_OPEN].cl_triggertype == LOGIC_HIGH) && (cl_sys_statbuffer.holder2status )) || ((Cl_alarms_alarms[HOLDER2STATUS_OPEN].cl_triggertype == LOGIC_LOW) && (!cl_sys_statbuffer.holder2status )) )
				
			{
				Cl_alarms_alarms[HOLDER2STATUS_OPEN].cl_alarmstate = CL_ALARM_DETECTED;
				Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=HOLDER2STATUS_OPEN;
				Cl_alarms_alarms[HOLDER2STATUS_OPEN].cl_detected_count++;
				cl_alarm_triggered = true;
				cl_lastalarmid = HOLDER2STATUS_OPEN;
				Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
				Cl_UpdateMacAlarmEventTable();
			}
			else
			{

				Cl_alarms_alarms[HOLDER2STATUS_OPEN].cl_alarmstate = CL_ALARM_INACTIVE;

			}
					
		

}

	
	if(Cl_alarms_alarms[HOLDER2STATUS_CLOSED].cl_is_enabled)
	{
		
		if(((Cl_alarms_alarms[HOLDER2STATUS_CLOSED].cl_triggertype == LOGIC_HIGH) && (cl_sys_statbuffer.holder2status )) || ((Cl_alarms_alarms[HOLDER2STATUS_CLOSED].cl_triggertype == LOGIC_LOW) && (!cl_sys_statbuffer.holder2status )) )
		
		{
			Cl_alarms_alarms[HOLDER2STATUS_CLOSED].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=HOLDER2STATUS_CLOSED;
			Cl_alarms_alarms[HOLDER2STATUS_CLOSED].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = HOLDER2STATUS_CLOSED;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
		}
		else
		{

			Cl_alarms_alarms[HOLDER2STATUS_CLOSED].cl_alarmstate = CL_ALARM_INACTIVE;

		}
		
		

	}
	if(Cl_alarms_alarms[FPCURRENTSTATUS].cl_is_enabled)
	{
				if(cl_sys_statbuffer.FPcurrentstatus == 0)
				{
				//	DD_RESET_VALVE (2);//testing- setting valve Id 2 PIN for observing in CRO
					Cl_alarms_alarms[FPCURRENTSTATUS].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=FPCURRENTSTATUS; 
					Cl_alarms_alarms[FPCURRENTSTATUS].cl_detected_count++;
					cl_alarm_triggered = true;	
					cl_lastalarmid = FPCURRENTSTATUS;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_CS ;
					Cl_UpdateMacAlarmEventTable();
					
					 
				}
				else
				{

						Cl_alarms_alarms[FPCURRENTSTATUS].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[FPCURRENTSTATUS].cl_detected_count = 0;

						
				}
				
						
	}

	
	if(Cl_alarms_alarms[DGPCURRENTSTATUS].cl_is_enabled)
	{
				if(cl_sys_statbuffer.DGPcurrentstatus == 0)
				{
				
					Cl_alarms_alarms[DGPCURRENTSTATUS].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=DGPCURRENTSTATUS; 
					Cl_alarms_alarms[DGPCURRENTSTATUS].cl_detected_count++;
					cl_alarm_triggered = true;	
					cl_lastalarmid = DGPCURRENTSTATUS;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
				else
				
				{
					Cl_alarms_alarms[DGPCURRENTSTATUS].cl_alarmstate = CL_ALARM_INACTIVE;
					Cl_alarms_alarms[DGPCURRENTSTATUS].cl_detected_count = 0;

					
				}
	}
	if(Cl_alarms_alarms[UF_ROTATION_MARKER].cl_is_enabled)
	{
				if(cl_sys_statbuffer.UFPstatus == 1)
				{
					Cl_alarms_alarms[UF_ROTATION_MARKER].cl_alarmstate = CL_ALARM_DETECTED;	
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=UF_ROTATION_MARKER; 
					Cl_alarms_alarms[UF_ROTATION_MARKER].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = UF_ROTATION_MARKER;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
				else
					
					{
						Cl_alarms_alarms[UF_ROTATION_MARKER].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[UF_ROTATION_MARKER].cl_detected_count = 0;

									
					}
	}

	if(Cl_alarms_alarms[HP_START].cl_is_enabled)
	{

	}
	if(Cl_alarms_alarms[HP_END].cl_is_enabled)
	{

	}



			
	//if(Cl_alarms_alarms[FLOWSTATUS_FLOWON].cl_is_enabled)
	{
				
		if(cl_sys_statbuffer.Flowstatus) 
		{	
				Flowswitch_msg.flowswitch_event = FLOW_SWITCH_EVENT_LEVEL_HIGH;						
		}
	}


	//if(Cl_alarms_alarms[FLOWSTATUS_FLOWOFF].cl_is_enabled)
	{
				
		if(!cl_sys_statbuffer.Flowstatus)
		{
				Flowswitch_msg.flowswitch_event = FLOW_SWITCH_EVENT_LEVEL_LOW;
		}
	}


			
	if(Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_is_enabled)
	{
	
		if(((cl_sys_statbuffer.levelswitch1) && (Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_triggertype == LOGIC_HIGH)) || ((!cl_sys_statbuffer.levelswitch1) && (Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_triggertype == LOGIC_LOW)))// when water level goes above it is a logic zero
		{
				
					//if(levelswitchprevstatus != 0)
					if (Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_alarmstate != CL_ALARM_DETECTED)
					{
									

								Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_alarmstate = CL_ALARM_ACTIVE;	
								Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_alarmstate = CL_ALARM_DETECTED;
								Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=LEVELSWITCH_OFF_TO_ON; 
								//Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_detected_count++;
								cl_alarm_triggered = true;
								cl_lastalarmid = LEVELSWITCH_OFF_TO_ON;
								Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
								//Cl_UpdateMacAlarmEventTable();
								Levelswitch_msg.levelswitch_event = LEVEL_SWITCH_EVENT_ON_TO_OFF;
					
					}
					else
					{
						//		Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_alarmstate = CL_ALARM_INACTIVE;
								Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_detected_count = 0;
								cl_alarm_triggered = false;
					
					}
				
					levelswitchprevstatus = 0;
		}
	}


	if(Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_is_enabled)
	{
		if(((cl_sys_statbuffer.levelswitch1) && (Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_triggertype == LOGIC_HIGH)) || ((!cl_sys_statbuffer.levelswitch1) && (Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_triggertype == LOGIC_LOW)))	
		//if((!cl_sys_statbuffer.levelswitch1) && (Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_triggertype == LOGIC_LOW))
		{				
				//	if(levelswitchprevstatus != 1)
				if (Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_alarmstate != CL_ALARM_DETECTED)
					{
					
				
						if(Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_is_enabled)
						{
								Cl_alarms_alarms[LEVELSWITCH_OFF_TO_ON].cl_alarmstate = CL_ALARM_ACTIVE;	
									Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_alarmstate = CL_ALARM_DETECTED;	
							//	Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=LEVELSWITCH_ON_TO_OFF; 
							//	Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_detected_count++;
								cl_alarm_triggered = true;
								cl_lastalarmid = LEVELSWITCH_ON_TO_OFF;
								Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
								//Cl_UpdateMacAlarmEventTable();
								Levelswitch_msg.levelswitch_event = LEVEL_SWITCH_EVENT_OFF_TO_ON;
						}
					}
					else
					{
					//	Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[LEVELSWITCH_ON_TO_OFF].cl_detected_count = 0;
						cl_alarm_triggered = false;
					
					}
				
					levelswitchprevstatus = 1;
		
		}
	}

	
	if(Cl_alarms_alarms[PS1STATUS_HIGH].cl_is_enabled)
	{
		if (Cl_alarms_alarms[PS1STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)
		{
				if(ps1 > Cl_alarmThresholdTable.ps1_high_threshold)
				{
					Cl_alarms_alarms[PS1STATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;	
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=PS1STATUS_HIGH;
					Cl_alarms_alarms[PS1STATUS_HIGH].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = PS1STATUS_HIGH;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
				}
				else
				{
					
				}
		}
	}

if(Cl_alarms_alarms[PS1STATUS_LOW].cl_is_enabled)
{
	if (Cl_alarms_alarms[PS1STATUS_LOW].cl_alarmstate != CL_ALARM_DETECTED)
	{
		if(ps1 < Cl_alarmThresholdTable.ps1_low_threshold)
		{
			Cl_alarms_alarms[PS1STATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=PS1STATUS_LOW;
			Cl_alarms_alarms[PS1STATUS_LOW].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = PS1STATUS_LOW;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
		}
		else
		{
			
		}
	}
}
	if(Cl_alarms_alarms[PS2STATUS_HIGH].cl_is_enabled)
	{
				if (Cl_alarms_alarms[PS2STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)
		{
				if(ps2 > Cl_alarmThresholdTable.ps2_high_threshold)
				{
						uint16_t temp_ps;
						temp_ps=ps2;

							 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2=",4);
							 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp_ps,2);
												
					Cl_alarms_alarms[PS2STATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;	
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=PS2STATUS_HIGH;
					Cl_alarms_alarms[PS2STATUS_HIGH].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = PS2STATUS_HIGH;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
		}
	}
		if(Cl_alarms_alarms[PS2STATUS_LOW].cl_is_enabled)
		{
			if (Cl_alarms_alarms[PS2STATUS_LOW].cl_alarmstate != CL_ALARM_DETECTED)
			{
				if(ps2 < Cl_alarmThresholdTable.ps2_low_threshold)
				{
					Cl_alarms_alarms[PS2STATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=PS2STATUS_HIGH;
					Cl_alarms_alarms[PS2STATUS_LOW].cl_detected_count++;
					cl_alarm_triggered = true;
					cl_lastalarmid = PS2STATUS_LOW;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
			}
		}
		

	if(Cl_alarms_alarms[PS3STATUS_HIGH].cl_is_enabled)
		{
			//	if((cl_sys_statbuffer.ps3status > 1474) && (Cl_alarms_alarms[PS3STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)) // eqvt to 0.8 bar
				if((ps3 > Cl_alarmThresholdTable.ps3_high_threshold ) && (Cl_alarms_alarms[PS3STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)) // eqvt to 0.8 bar
				{
				Cl_alarms_alarms[PS3STATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;
				Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=PS3STATUS_HIGH;
				Cl_alarms_alarms[PS3STATUS_HIGH].cl_detected_count++;
				cl_alarm_triggered = true;
				cl_lastalarmid = PS3STATUS_HIGH;
				Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
				Cl_UpdateMacAlarmEventTable();
				}
				
				else if ((Cl_alarms_alarms[PS3STATUS_HIGH].cl_alarmstate == CL_ALARM_DETECTED) && (cl_sys_statbuffer.ps3status < Cl_alarmThresholdTable.ps3_high_threshold - 200  ))
				{
					Cl_alarms_alarms[PS3STATUS_HIGH].cl_alarmstate = CL_ALARM_INACTIVE;
				}
		}
	
	if(Cl_alarms_alarms[PS3STATUS_LOW].cl_is_enabled)
	{
		//	if((cl_sys_statbuffer.ps3status > 1474) && (Cl_alarms_alarms[PS3STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)) // eqvt to 0.8 bar
		if((ps3 < Cl_alarmThresholdTable.ps3_low_threshold ) && (Cl_alarms_alarms[PS3STATUS_LOW].cl_alarmstate != CL_ALARM_DETECTED)) // eqvt to 0.8 bar
		{
			Cl_alarms_alarms[PS3STATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=PS3STATUS_LOW;
			Cl_alarms_alarms[PS3STATUS_LOW].cl_detected_count++;
			cl_alarm_triggered = true;
			cl_lastalarmid = PS3STATUS_LOW;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
		}
		
		else if ((Cl_alarms_alarms[PS3STATUS_LOW].cl_alarmstate == CL_ALARM_DETECTED) && (cl_sys_statbuffer.ps3status < Cl_alarmThresholdTable.ps3_low_threshold - 200 ))
		{
			Cl_alarms_alarms[PS3STATUS_LOW].cl_alarmstate = CL_ALARM_INACTIVE;
		}
	}
	
	
	if(Cl_alarms_alarms[TEMP1STATUS_HIGH].cl_is_enabled)
	{
			//	int16_t temp1,temp2;
			//	temp1 = (0.805 * cl_sys_statbuffer.Temp1status) - 1004 ;
			//	temp2 = 3000 + (temp1 * 1000)/382;

			//if(cl_sys_statbuffer.Temp1status > 4096)
			if(temp1 > Cl_alarmThresholdTable.temp1_high_threshold)
			{

				Cl_alarms_alarms[TEMP1STATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;
				Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=TEMP1STATUS_HIGH;
				Cl_alarms_alarms[TEMP1STATUS_HIGH].cl_detected_count++;
				cl_alarm_triggered = true;
				cl_lastalarmid = TEMP1STATUS_HIGH;
				Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
				Cl_UpdateMacAlarmEventTable();
						
			}
	}
	
	
	if(Cl_alarms_alarms[TEMP1STATUS_LOW].cl_is_enabled)
	{
			//	int16_t temp1,temp2;
			//	temp1 = (0.805 * cl_sys_statbuffer.Temp1status) - 1004 ;
			//	temp2 = 3000 + (temp1 * 1000)/382;

			//if(cl_sys_statbuffer.Temp1status > 4096)
			if(temp1 < Cl_alarmThresholdTable.temp1_low_threshold)
			{

				Cl_alarms_alarms[TEMP1STATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
				Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=TEMP1STATUS_LOW;
				Cl_alarms_alarms[TEMP1STATUS_LOW].cl_detected_count++;
				cl_alarm_triggered = true;
				cl_lastalarmid = TEMP1STATUS_LOW;
				Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
				Cl_UpdateMacAlarmEventTable();
						
			}
			{	
				if(Cl_alarms_alarms[TEMP1STATUS_LOW].cl_alarmstate == CL_ALARM_DETECTED)
				{												
					Cl_alarms_alarms[TEMP1STATUS_LOW].cl_alarmstate = CL_ALARM_INACTIVE;
					Cl_alarms_alarms[TEMP1STATUS_LOW].cl_detected_count = 0;
					cl_alarm_triggered = false;	
				}						
			}
	}
	
	
	if(Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_is_enabled)
		{
			//	int16_t temp1,temp2;
			//	temp1 = (0.805 * cl_sys_statbuffer.Temp3status) - 1004 ;
			//	temp2 = 3000 + (temp1 * 1000)/382;
			if(temp3 > Cl_alarmThresholdTable.temp3_high_threshold)
		//	if(temp2 > Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_upper)
		//	if(cl_sys_statbuffer.Temp3status > 405)
			{
				if(Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)
				{
					Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=TEMP3STATUS_HIGH;
					Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_detected_count++;
					cl_alarm_triggered = true;
					
					cl_lastalarmid = TEMP3STATUS_HIGH;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
				}						
			}
	
				else 
			//	else if (cl_sys_statbuffer.Temp3status > 400)
				{	
					if(Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_alarmstate == CL_ALARM_DETECTED)
					{												
						Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[TEMP3STATUS_HIGH].cl_detected_count = 0;
						cl_alarm_triggered = false;	
					}						
				}
				}
	if(Cl_alarms_alarms[TEMP3STATUS_LOW].cl_is_enabled)
	{
			 if (temp3 < Cl_alarmThresholdTable.temp3_low_threshold)
		//	else if (cl_sys_statbuffer.Temp3status < 395)
				{
			
							if(Cl_alarms_alarms[TEMP3STATUS_LOW].cl_alarmstate != CL_ALARM_DETECTED)
							{
								Cl_alarms_alarms[TEMP3STATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
								Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=TEMP3STATUS_LOW;
								Cl_alarms_alarms[TEMP3STATUS_LOW].cl_detected_count++;
								cl_alarm_triggered = true;
						
								cl_lastalarmid = TEMP3STATUS_LOW;
								Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
								Cl_UpdateMacAlarmEventTable();
						
							}
				}
				else
				{
									
							if(Cl_alarms_alarms[TEMP3STATUS_LOW].cl_alarmstate == CL_ALARM_DETECTED)
							{
								Cl_alarms_alarms[TEMP3STATUS_LOW].cl_alarmstate = CL_ALARM_INACTIVE;
								Cl_alarms_alarms[TEMP3STATUS_LOW].cl_detected_count = 0;
								cl_alarm_triggered = false;
							}
									
					
				}

		}


if(Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_is_enabled)
{
	
	//		int16_t temp1,temp2;
	//		temp1 = (0.805 * cl_sys_statbuffer.Temp2status) - 1004 ;
	//		temp2 = 3000 + (temp1 * 1000)/382;
			if(temp2 > Cl_alarmThresholdTable.temp2_high_threshold)

//	if(cl_sys_statbuffer.Temp2status > 840)
	{
	
		if(Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)
		{
			Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=TEMP2STATUS_HIGH;
			Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_detected_count++;
			cl_alarm_triggered = true;
			
			cl_lastalarmid = TEMP2STATUS_HIGH;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
			
		}

	}
	else 
	//	else if (cl_sys_statbuffer.Temp3status > 400)
	{

			if(Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_alarmstate == CL_ALARM_DETECTED)
			{
				Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_alarmstate = CL_ALARM_INACTIVE;
				Cl_alarms_alarms[TEMP2STATUS_HIGH].cl_detected_count = 0;
				cl_alarm_triggered = false;
			}

		
		
	}
}
if(Cl_alarms_alarms[TEMP2STATUS_LOW].cl_is_enabled)
{
	
	 if (temp2 < Cl_alarmThresholdTable.temp2_low_threshold)
		{
			
		
		if(Cl_alarms_alarms[TEMP2STATUS_LOW].cl_alarmstate != CL_ALARM_DETECTED)
		{
			Cl_alarms_alarms[TEMP2STATUS_LOW].cl_alarmstate = CL_ALARM_DETECTED;
			Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=TEMP2STATUS_LOW;
			Cl_alarms_alarms[TEMP2STATUS_LOW].cl_detected_count++;
			cl_alarm_triggered = true;
			
			cl_lastalarmid = TEMP2STATUS_LOW;
			Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
			Cl_UpdateMacAlarmEventTable();
			
		}
		}

	
}
			
	if((Cl_alarms_alarms[COND_DAC_OPEN].cl_is_enabled) || (Cl_alarms_alarms[COND_DAC_RO].cl_is_enabled) || (Cl_alarms_alarms[COND_DAC_HIGH].cl_is_enabled) )
	{
			dac_cond_alarm = ALARM_NO_ALARM;
			cl_get_dac_conductivity_info(&dac_cond_alarm);
				switch(dac_cond_alarm)
				{
					case COND_DAC_OPEN:
						Cl_alarms_alarms[dac_cond_alarm].cl_alarmstate = CL_ALARM_DETECTED;
						Cl_alarms_alarms[COND_DAC_RO].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[COND_DAC_HIGH].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=dac_cond_alarm;
						Cl_alarms_alarms[dac_cond_alarm].cl_detected_count++;
						cl_alarm_triggered = true;
						cl_lastalarmid = dac_cond_alarm;
						Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
						Cl_UpdateMacAlarmEventTable();
					break;
					case COND_DAC_RO:
						Cl_alarms_alarms[dac_cond_alarm].cl_alarmstate = CL_ALARM_DETECTED;
						Cl_alarms_alarms[COND_DAC_OPEN].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[COND_DAC_HIGH].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=dac_cond_alarm;
						Cl_alarms_alarms[dac_cond_alarm].cl_detected_count++;
						cl_alarm_triggered = true;
						cl_lastalarmid = dac_cond_alarm;
						Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
						Cl_UpdateMacAlarmEventTable();
					break;
					case COND_DAC_HIGH:
						Cl_alarms_alarms[dac_cond_alarm].cl_alarmstate = CL_ALARM_DETECTED;
						Cl_alarms_alarms[COND_DAC_OPEN].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_alarms_alarms[COND_DAC_RO].cl_alarmstate = CL_ALARM_INACTIVE;
						Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=dac_cond_alarm;
						Cl_alarms_alarms[dac_cond_alarm].cl_detected_count++;
						cl_alarm_triggered = true;
						cl_lastalarmid = dac_cond_alarm;
						Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
						Cl_UpdateMacAlarmEventTable();

					break;
				}

	}
		
	if((Cl_alarms_alarms[COND_STATUS_LOW].cl_is_enabled) || (Cl_alarms_alarms[COND_STATUS_HIGH].cl_is_enabled))
	{
		cond_alarm = ALARM_NO_ALARM;
		cl_get_conductivity_info(&cond_alarm);
		switch(cond_alarm)
		{
			case COND_STATUS_LOW:
			if(Cl_alarms_alarms[COND_STATUS_LOW].cl_is_enabled)
			{
							if(Cl_alarms_alarms[COND_STATUS_LOW].cl_alarmstate != CL_ALARM_DETECTED)
							{
								Cl_alarms_alarms[cond_alarm].cl_alarmstate = CL_ALARM_DETECTED;
								Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=cond_alarm;
								Cl_alarms_alarms[cond_alarm].cl_detected_count++;
								cl_alarm_triggered = true;
								cl_lastalarmid = cond_alarm;
								Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
								Cl_UpdateMacAlarmEventTable();
							}
				
			}
			break;

			case COND_STATUS_HIGH:
			if(Cl_alarms_alarms[COND_STATUS_HIGH].cl_is_enabled)
			{
							if(Cl_alarms_alarms[COND_STATUS_HIGH].cl_alarmstate != CL_ALARM_DETECTED)
							{
								Cl_alarms_alarms[cond_alarm].cl_alarmstate = CL_ALARM_DETECTED;
								Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=cond_alarm;
								Cl_alarms_alarms[cond_alarm].cl_detected_count++;
								cl_alarm_triggered = true;
								cl_lastalarmid = cond_alarm;
								Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
								Cl_UpdateMacAlarmEventTable();
							}
			}
			break;
			
			default:break;
			
		}
	}	


if (  gflow_en)//testing
{
	if((Cl_alarms_alarms[FLOWSTATUS_FLOWON].cl_is_enabled)|| (Cl_alarms_alarms[FLOWSTATUS_FLOWOFF].cl_is_enabled))
	{

		if(cl_sys_statbuffer.Flowstatus != prev_flowstatus )
		{
			// flow_watchdog = 1000000 ;
			 flow_watchdog = 0 ;
			 flow_count++;
			 flow_count_rate++;
		}
		else
		{
		//	flow_watchdog = 60;
		//	if(flow_watchdog > 0)
		//	flow_watchdog--;
		}
		
			//if(flow_watchdog < 999400)static uint32_t flowstatus_off_alarm_count = 0;static uint32_t flowstatus_off_alarm_count = 0;
			if(flow_watchdog > 40 )
			{
				if(Cl_alarms_alarms[FLOWSTATUS_FLOWOFF].cl_alarmstate != CL_ALARM_DETECTED)
				{
					Cl_alarms_alarms[FLOWSTATUS_FLOWOFF].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=FLOWSTATUS_FLOWOFF;
					Cl_alarms_alarms[FLOWSTATUS_FLOWOFF].cl_detected_count++;
					
					Cl_alarms_alarms[FLOWSTATUS_FLOWON].cl_alarmstate = CL_ALARM_INACTIVE;
				
					cl_alarm_triggered = true;
					
					cl_lastalarmid = FLOWSTATUS_FLOWOFF;
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLOW_OFF",8);
					
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
			}
			if(flow_count  > 2)
			{
				
				
				
				if(Cl_alarms_alarms[FLOWSTATUS_FLOWON].cl_alarmstate != CL_ALARM_DETECTED)
				{
					Cl_alarms_alarms[FLOWSTATUS_FLOWON].cl_alarmstate = CL_ALARM_DETECTED;
					Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=FLOWSTATUS_FLOWON;
					Cl_alarms_alarms[FLOWSTATUS_FLOWON].cl_detected_count++;
					
					Cl_alarms_alarms[FLOWSTATUS_FLOWOFF].cl_alarmstate = CL_ALARM_INACTIVE;
					
					cl_alarm_triggered = true;
					
					cl_lastalarmid = FLOWSTATUS_FLOWON;
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
					Cl_UpdateMacAlarmEventTable();
					
				}
				
			}
		
		prev_flowstatus = cl_sys_statbuffer.Flowstatus;

	}
}
	
				 
		return CL_OK;
}


Cl_ReturnCodes Cl_UpdateMacAlarmEventTable(void)
{
	
	if((cl_alarm_triggered == true) &&( Cl_MacEvent1.Cl_MacNewEventcount <= MAC_EVENT_COUNT_MAX ))
	
	
	{		
		Cl_MacEvent1.Cl_MacNewEvent = true;
		Cl_MacEvent1.Cl_MacNewEventcount++; 
	}
}
Cl_ReturnCodes Cl_AlarmConfigureAlarmType(Cl_AlarmIdType cl_alarm_id , Cl_AlarmTriggerType alarmtriggertype,uint16_t cl_upper,uint16_t cl_lower,uint8_t  count_threshold )
{
	
Cl_alarms_alarms[cl_alarm_id].cl_triggertype = alarmtriggertype;
Cl_alarms_alarms[cl_alarm_id].cl_lower = cl_lower;
Cl_alarms_alarms[cl_alarm_id].cl_upper = cl_upper;
Cl_alarms_alarms[cl_alarm_id].count_threshold = count_threshold;		
}

Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_AlarmIdType cl_alarm_id , bool status )
{
	Cl_ReturnCodes cl_RinseRetCode = CL_OK;
	
	if (	(cl_alarm_id == LEVELSWITCH_OFF_TO_ON) || (cl_alarm_id == LEVELSWITCH_ON_TO_OFF) ||(cl_alarm_id == FPCURRENTSTATUS))
	{
			if ( status == true)
			{
			Cl_alarms_alarms[cl_alarm_id].cl_is_enabled = true;	
			Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_ACTIVE;
			Cl_alarms_alarms[cl_alarm_id].cl_detected_count = 0;
			
			}
			else
			{
				Cl_alarms_alarms[cl_alarm_id].cl_is_enabled = false;
				Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_INACTIVE;
				Cl_alarms_alarms[cl_alarm_id].cl_detected_count = 0;	
		
			}
	
		
	}
	if ( TEST_STUB_ENABLEGATE)
	{
		Cl_alarms_alarms[cl_alarm_id].cl_alarmID = cl_alarm_id;
			if ( status == true)
			{
			
			Cl_alarms_alarms[cl_alarm_id].cl_is_enabled = true;	
			Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_ACTIVE;
			Cl_alarms_alarms[cl_alarm_id].cl_detected_count = 0;
			
			}
			else
			{
				Cl_alarms_alarms[cl_alarm_id].cl_is_enabled = false;
				Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_INACTIVE;
				Cl_alarms_alarms[cl_alarm_id].cl_detected_count = 0;	
		
			}
	
		
	}

	
	return(cl_RinseRetCode);
	
	
}


Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_AlarmIdType cl_alarm_id )
{
	Cl_ReturnCodes cl_RinseRetCode = CL_OK;
	
		Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_ACTIVE;
	
	return(cl_RinseRetCode);
	
	
}

Cl_ReturnCodes Cl_AlarmMuteAlarms(Cl_AlarmIdType cl_alarm_id)
{
	Cl_ReturnCodes cl_alarmRetCode = CL_OK;
	
	
	Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_MUTE;	
	return(cl_alarmRetCode);
	
	
}

Cl_ReturnCodes Cl_Alarm_GetLastAlarm(Cl_AlarmIdType* cl_alarm_id)
{
		Cl_ReturnCodes cl_alarmRetCode = CL_OK;
		*cl_alarm_id = cl_lastalarmid;
		
		return cl_alarmRetCode;
	
}

Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_AlarmIdType cl_alarm_id , bool* cl_status)
{
	Cl_ReturnCodes cl_alarmRetCode = CL_OK;
	 if (Cl_alarms_alarms[cl_alarm_id].cl_alarmstate  == CL_ALARM_DETECTED ||  Cl_alarms_alarms[cl_alarm_id].cl_alarmstate == CL_ALARM_MUTE) 
	 {
		 *cl_status = true;
	 }
	 else
	 {
		 *cl_status = false;
	 }
	 
		
	
	return cl_alarmRetCode;
	
}

Cl_ReturnCodes Cl_Alarm_GetAlertStatus(Cl_AlarmIdType cl_alarm_id , bool* cl_status)
{
	Cl_ReturnCodes cl_alarmRetCode = CL_OK;
	if (Cl_alarms_alarms[cl_alarm_id].cl_alarmstate  == CL_ALARM_DETECTED ||  Cl_alarms_alarms[cl_alarm_id].cl_alarmstate == CL_ALARM_MUTE)
	{
		*cl_status = true;
	}
	else
	{
		*cl_status = false;
	}
	
	
	
	return cl_alarmRetCode;
	
}

Cl_ReturnCodes Cl_Alarm_TriggerDummyAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status)
{
	if(cl_status)
	{
						Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_DETECTED;
						Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=cl_alarm_id;
						Cl_alarms_alarms[cl_alarm_id].cl_detected_count++;
						cl_alarm_triggered = true;
						
						cl_lastalarmid = cl_alarm_id;
						Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
						Cl_UpdateMacAlarmEventTable();
	}
	else
	{
		
		if(Cl_alarms_alarms[cl_alarm_id].cl_alarmstate == CL_ALARM_DETECTED)
		{
			Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_INACTIVE;
			Cl_alarms_alarms[cl_alarm_id].cl_detected_count = 0;
			cl_alarm_triggered = false;
		}
						
	
	}
}
Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status)
{
	if(Cl_alarms_alarms[cl_alarm_id].cl_is_enabled)
	{
			if(cl_status)
			{
				Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_DETECTED;
				Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]=cl_alarm_id;
				Cl_alarms_alarms[cl_alarm_id].cl_detected_count++;
				cl_alarm_triggered = true;
				
				cl_lastalarmid = cl_alarm_id;
				if(FPCURRENTSTATUS == cl_alarm_id)
				{
					Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALERT_TRIGGERED ;
				}
				else
				{
						Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_ALARM_TRIGGERED ;
				}
			
				Cl_UpdateMacAlarmEventTable();
			}
			else
			{
				
				if(Cl_alarms_alarms[cl_alarm_id].cl_alarmstate == CL_ALARM_DETECTED)
				{
					Cl_alarms_alarms[cl_alarm_id].cl_alarmstate = CL_ALARM_INACTIVE;
					Cl_alarms_alarms[cl_alarm_id].cl_detected_count = 0;
					cl_alarm_triggered = false;
				}
				
				
			}
		
	}

}
Cl_ReturnCodes Cl_Alarm_timmer_50millisec(void) // testing
{
	Cl_ReturnCodes cl_alarm_retcode = CL_OK;
	static uint16_t fiftymscount = 0;
	fiftymscount++;
//	 flow_watchdog = 999900 ;
	flow_count = 0;
//	if(flow_watchdog >= 0)
	flow_watchdog++	;
	
	if(fiftymscount > 20)
	{
		if (  gflow_en)//testing
		{
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLOW_CNT",8);
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&flow_count_rate,2);
		}
		flow_count_rate =0;

		fiftymscount = 0;
	}
	
	
	
}