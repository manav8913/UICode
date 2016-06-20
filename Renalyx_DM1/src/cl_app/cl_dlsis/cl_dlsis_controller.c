/*
 * cl_dlsis_controller.c
 *
 * Created: 12/27/2013 12:26:22 PM
 *  Author: user
 */ 


#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_dlsis/inc/cl_dlsis_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "Platform/Service/sv_interface.h"
#include "sv_stubs/inc/sv_types.h"
#include "sv_stubs/inc/sv_stub.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.h"
#include "cl_app/cl_bc_cntrl/inc/Cl_bc_controller.h"
#include "cl_app/cl_dprep/inc/cl_dprep_primecontroller.h"
#include "../comp/heparinpumpcontrl/inc/cl_heparincontroller.h"
#include "../comp/ufpumpcontrl/inc/cl_ufPumpController.h"
#include "../comp/bloodpumpcntrl/inc/cl_bloodpumpcontroller.h"
#include "cl_app/cl_status/inc/cl_status.h"

extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_NewAlarmIdType,bool );
extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern uint8_t sv_cntrl_setflowpath(sv_flowpathtype);
extern Cl_ReturnCodes SetHeaterState(HeaterStateType Param_HeaterState);
extern Cl_ReturnCodes UpdateHeaterControls(void);
extern Cl_ReturnCodes cl_hep_controller(cl_hep_events hp_event , int16_t data);
extern Cl_ReturnCodes cl_uf_controller(cl_uf_events uf_event , int16_t data);
extern Cl_ReturnCodes cl_bp_controller(cl_bp_events bp_event , int16_t data);
extern uint8_t sv_cntrl_deactivatevenousclamp(void);
extern Cl_ReturnCodes Cl_Heater_GetHeaterStableStatus(bool* );
extern void sv_prop_startmixing();
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t sv_cntrl_disable_loopback(void);
extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
extern void calibration_apt(uint16_t sensordata);

extern Cl_AlarmThresholdType  Cl_alarmThresholdTable;

extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern bool BC_window; //test
extern bool g_testbcfreeze; //test
extern bool Current_sense_trigger; // test
extern float dummy3;
extern volatile int16_t pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;
Cl_Dlsis_States cl_dlsis_state = CL_DLSIS_STATE_IDLE;
//Cl_Rinse_States cl_dlsis_state = CL_DLSIS_STATE_IDLE;

 DlsisTimeType DlsisTime ;
 bool Cl_dlsisOpenFillTimeOut = false, heater_stable_status = false;
 uint16_t dlis_wait_cnt;

 uint16_t wait_cnt_dls = 0,TmpVal_dls =0;
 int16_t temp1_dls ,temp2_dls;


 extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
 extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
 
 extern Cl_ReturnCodes  sv_nvmgetdata(uint8_t,uint8_t*);
 
 extern Cl_ReturnCodes        Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
 extern Cl_ReturnCodes     Cl_SendDatatoconsole_dummy(Cl_ConsoleTxCommandtype,uint8_t*,uint8_t);
 extern Cl_ReturnCodes cl_wait(uint32_t );
 extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
 extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_NewAlarmIdType,bool );
 extern Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_NewAlarmIdType  );
 extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
 extern Cl_ReturnCodes  sv_nvmgetdata(uint8_t,uint8_t*);
 extern Cl_ReturnCodes Cl_Alarm_GetLastAlarm(ClDlsisAlarmIdType*);
 extern Cl_ReturnCodes Cl_Rinse_UpdateAlarmTable(ClDlsisAlarmIdType*);
 extern Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_NewAlarmIdType  , bool* );
 extern Cl_ReturnCodes  sv_nvmsetdata(uint8_t ,uint8_t* ,uint8_t datasize);
 extern uint8_t sv_cntrl_setflowpath(sv_flowpathtype sv_flowpath);
 extern Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
 extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
 extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
 extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
 extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
 extern uint8_t sv_cntrl_poweronheater(void);
 extern uint8_t sv_cntrl_poweroffheater(void);
 extern uint8_t sv_cntrl_incheater(int32_t dty_val);
 extern Cl_ReturnCodes Cl_AlarmConfigureAlarmType(Cl_NewAlarmIdType,Cl_AlarmTriggerType,uint16_t,uint16_t,uint8_t);
 extern Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event);
 extern uint8_t sv_cntrl_disable_loopback(void);
 extern uint8_t sv_cntrl_enable_bypass(void);
 extern uint8_t sv_cntrl_disable_bypass(void);

Cl_ReturnCodes Cl_dlsis_init(void);
Cl_ReturnCodes cl_dlsis_translatemacevent(MAC_EVENTS ,Cl_Dlsis_Eevents* );
Cl_ReturnCodes Cl_Dlsis_UpdateTimeInfo(void);
Cl_ReturnCodes Cl_Dlsis_controller(MAC_EVENTS );
Cl_ReturnCodes cl_Dlsis_CheckforDialysisCompletion(void);
Cl_ReturnCodes cl_Dlsis_notifydacandgotorinsestandby(void);
Cl_ReturnCodes Cl_Dlsis_ProcessAlarms( void);
Cl_ReturnCodes Cl_Dlsis_StopDialysis(void);
Cl_ReturnCodes Cl_Dlsis_PauseDialysis(void);
Cl_ReturnCodes  CL_DlsisAlarmActon(Cl_NewAlarmIdType cl_dlsisalarmid);
Cl_ReturnCodes  Cl_DlsisFillingFlowOn(void );
Cl_ReturnCodes  Cl_DlsisFillingFlowOff(void );
Cl_ReturnCodes Cl_Dlsis_SenddlsisData(void);
Cl_ReturnCodes Cl_Dlsis_Get_data(Cl_ConsoleRxDataType DataId, uint8_t size);
Cl_ReturnCodes	Cl_Dlsis_StartDialysis(void);
Cl_ReturnCodes	Cl_Dlsis_ResumeDialysis(void);
Cl_ReturnCodes Cl_Dlsis_BypassDialysis(void);

DlsisAlarmsType Cl_DlsisAlarmTable[CL_DLSIS_ALRM_MAX] = 
{
		
{BLOODDOOR_STATUS_OPEN,false,false,false},
{LEVELSWITCH_OFF_TO_ON,false,false,false},
{LEVELSWITCH_ON_TO_OFF,false,false,false},
{HOLDER1STATUS_CLOSED,false,false,false},
{HOLDER2STATUS_CLOSED,false,false,false},
{COND_STATUS_LOW,false,false,false},
{COND_STATUS_HIGH,false,false,false},
{COND_DAC_OPEN,false,false,false},
{COND_DAC_RO,false,false,false},
{COND_DAC_HIGH,false,false,false},
//{HP_START,false,false,false},
{ABD_EVENT,false,false,false},
{BD_EVENT,false,false,false},
{APTSTATUS_HIGH,false,false,false},
{VPTSTATUS_HIGH,false,false,false},
{BLD_EVENT,false,false,false},
{PS1_HIGH_THRESHOLD,false,false,false},
{PS1_LOW_THRESHOLD,false,false,false},
{PS2_HIGH_THRESHOLD,false,false,false},
{PS2_LOW_THRESHOLD,false,false,false},
{PS3_HIGH_THRESHOLD,false,false,false},
{PS3_LOW_THRESHOLD,false,false,false},
//{PS4STATUS,false,false,false},
{TEMP1_HIGH_THRESHOLD,false,false,false},
{TEMP1_LOW_THRESHOLD,false,false,false},
{TEMP2_HIGH_THRESHOLD,false,false,false},
{TEMP2_LOW_THRESHOLD,false,false,false},
{TEMP3_HIGH_THRESHOLD,false,false,false},
{TEMP3_LOW_THRESHOLD,false,false,false},
{FPCURRENTSTATUS,false,false,false},

};

 Cl_ReturnCodes Cl_dlsis_init(void)
 {
	 
	 return CL_OK;
 }
 
 
Cl_ReturnCodes Cl_Dlsis_controller(MAC_EVENTS Cl_MacDlsisEvent)
{
	
Cl_ReturnCodes Cl_dlsisretcode;
Cl_Dlsis_Eevents cl_dlsis_event = EVENT_NULL_EVENT;
bool alarmstatus1 = false,alarmstatus2 = false,alarmstatus3 = false, flowstatus = false;
static bool bcs_wait = false;
static bool V2_toggle = false;
uint16_t Tmp2Val = 0;
uint8_t dataarray[8] =  {0,0,0,0};
extern bool BC_window; //test
extern bool g_testbcfreeze; //test
extern bool Current_sense_trigger; // test
Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
uint8_t data = 0;
	


Cl_dlsisretcode =  cl_dlsis_translatemacevent( Cl_MacDlsisEvent, &cl_dlsis_event);
	
	switch(cl_dlsis_event)
	{
		case EVENT_DLSIS_TICK_SECOND:
		Cl_dlsisretcode = Cl_bc_controller(BC_EVENT_SECOND);
		break;
		case EVENT_DLSIS_TICK_50MS:
		Cl_dlsisretcode = Cl_bc_controller(BC_EVENT_50MS);
		break;
		default:break;
		
	}
	if(cl_dlsis_event == EVENT_DLSIS_TICK_SECOND)
	{

		Cl_dlsisretcode =  Cl_AlarmResetAlarm( TEMP3_HIGH_THRESHOLD);
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( TEMP3_LOW_THRESHOLD);
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( TEMP2_HIGH_THRESHOLD);
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( FLOW_NO_FLOW );
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( FLOW_HIGH_FLOWRATE );
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( FLOW_LOW_FLOWRATE );
	}


	switch(cl_dlsis_state)
	{

		case CL_DLSIS_STATE_INIT:
		break;
		case CL_DLSIS_STATE_IDLE:
		switch (cl_dlsis_event)
		{
			case EVENT_DLSIS_GET_DATA:

				if(Cl_ConsoleRxMsg.msgready == true)
				{
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case	CON_RX_PARAM_DATA_DIALYSIS_STATUS:
						command = CON_TX_COMMAND_SYSDATA;
						dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_STATUS;
						Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
						Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						break;

							
						default:
						break;
					}
				}
			break;
			case EVENT_DLSIS_SET_DATA:
			if(Cl_ConsoleRxMsg.msgready == true)
			{
				cl_Datastreamtype cl_temp  ;
				cl_temp.bytearray[0] = Cl_ConsoleRxMsg.data.byte[1];
				cl_temp.bytearray[1] = Cl_ConsoleRxMsg.data.byte[2];
				cl_temp.bytearray[2] = Cl_ConsoleRxMsg.data.byte[3];
				cl_temp.bytearray[3] = Cl_ConsoleRxMsg.data.byte[4];
				
				switch(Cl_ConsoleRxMsg.data.byte[0])
				{
					case ID_dflow:
					break;
					case ID_settemp:
					break;
					case ID_heprate:
					break;
					case ID_setcond:
					break;
					case ID_ufrate:
					break;
					case ID_ufgoal:
					break;
					case ID_bolusvol:
					break;
					case ID_bloodrate:
					break;
					case ID_hepstoptime:
					break;
					case ID_syringetype:
					break;
					case ID_heparincheck:
					break;
					case ID_minufrate:
					break;
					case ID_treattime:
					break;
					case ID_bloodratereturn:
					break;
					case ID_bloodratetreat:
					break;
					case ID_tempulimit:
					Cl_alarmThresholdTable.temp3_high_threshold =  (cl_temp.word)/10;
					break;
					case ID_templlimit:
					Cl_alarmThresholdTable.temp3_low_threshold =  (cl_temp.word)/10;
					break;
					case ID_tmpllimit:
					Cl_alarmThresholdTable.tmp_low_threshold =  cl_temp.word;
					break;
					case ID_tmpulimit:
					Cl_alarmThresholdTable.tmp_high_threshold =  cl_temp.word;
					break;
					case ID_vptllimit:
					Cl_alarmThresholdTable.vpt_low_threshold =  cl_temp.word;
					break;
					case ID_vptulimit:
					Cl_alarmThresholdTable.vpt_high_threshold =  cl_temp.word;
					break;
					case ID_ufllimit:
					break;
					case ID_ufulimit:
					break;
					case ID_dflowllimit:
					break;
					case ID_dflowulimit:
					break;
					case ID_condllimit:
					Cl_alarmThresholdTable.cond_low_threshold =  cl_temp.word;
					break;
					case ID_condulimit:
					Cl_alarmThresholdTable.cond_high_threshold =  cl_temp.word;
					break;
					case ID_aptllimit:
					Cl_alarmThresholdTable.apt_low_threshold =  cl_temp.word;
					break;
					case ID_aptulimit:
					Cl_alarmThresholdTable.apt_high_threshold =  cl_temp.word;
					break;

					default:break;
				}
			}
			
			
			break;
			case EVENT_DLSIS_START_DIALYSIS:
					Cl_Dlsis_StartDialysis();
			break;
			case EVENT_DLSIS_ALARM:
					Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
			break;
			default:
			break;
		}
		break;
		case CL_DLSIS_STATE_WAIT_FOR_TEMP_COND_STABILITY:
		
			switch(cl_dlsis_event)
			{
				
				case EVENT_DLSIS_TICK_500MS:
				
				
					Cl_Heater_GetHeaterStableStatus(&heater_stable_status);
					uint16_t temp = 0;
					Cl_SysStat_GetSensor_Status_Query(SENSOR_COND_STATUS , &temp);	
				//	if ((heater_stable_status) &&  (temp < 14) && (temp > 13.3))
					//if( (temp == 0))
					{
				
						cl_dlsis_state = CL_DLSIS_STATE_WAIT_FOR_BLOOD_DETECT;
				
				

				
					}
				break;
				
				
				default:break;
			}

		break;
		case CL_DLSIS_STATE_WAIT_FOR_BLOOD_DETECT:
		
		switch(cl_dlsis_event)
		{
			
			case EVENT_DLSIS_TICK_500MS:
					{
						uint16_t temp = 0;
					
					Cl_SysStat_GetSensor_Status_Query(SENSOR_BDSTATUS , &temp);
					if( temp == 0)
					{
						
						Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BLOOD",5);
						//	cl_bp_controller(CL_BP_EVENT_START,0);
						//	cl_hep_controller(CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE,5000);
						//	cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,0);
						sv_cntrl_disable_bypass();
			
						cl_uf_controller(CL_UF_EVENT_START,0);
						cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,10000);
						cl_wait(200);
						cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,5000);
						cl_wait(200);
						cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,3500);
						cl_wait(200);
						cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,2500);
			
						cl_dlsis_state = CL_DLSIS_STATE_UF_ACTIVATION;
					}
					}
			break;
			default:break;
		}
		break;
		case CL_DLSIS_STATE_UF_ACTIVATION:
		switch (cl_dlsis_event)
		{
			
			case EVENT_DLSIS_TICK_SECOND:
			cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,2500);
				cl_dlsis_state =CL_DLSIS_STATE_DIALYSIS;
			break;
			default:break;
		}
		
		break;
		
		case CL_DLSIS_STATE_DIALYSIS:
		switch (cl_dlsis_event)
		{
			case EVENT_DLSIS_GET_DATA:

				if(Cl_ConsoleRxMsg.msgready == true)
				{
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case	CON_RX_PARAM_DATA_DIALYSIS_STATUS:
						command = CON_TX_COMMAND_SYSDATA;
						dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_STATUS;
						Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
						Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						break;

						default:
						break;
					}
				}
			break;
			case EVENT_DLSIS_SET_DATA:
				if(Cl_ConsoleRxMsg.msgready == true)
										{
													cl_Datastreamtype cl_temp  ;	
													cl_temp.bytearray[0] = Cl_ConsoleRxMsg.data.byte[1];
													cl_temp.bytearray[1] = Cl_ConsoleRxMsg.data.byte[2];
													cl_temp.bytearray[2] = Cl_ConsoleRxMsg.data.byte[3];
													cl_temp.bytearray[3] = Cl_ConsoleRxMsg.data.byte[4];
											
											switch(Cl_ConsoleRxMsg.data.byte[0])
											{
													case ID_dflow:
													break;
													case ID_settemp:
													break;
													case ID_heprate:
													break;
													case ID_setcond:
													break;
													case ID_ufrate:
													break;
													case ID_ufgoal:
													break;
													case ID_bolusvol:
													break;
													case ID_bloodrate:
													break;
													case ID_hepstoptime:
													break;
													case ID_syringetype:
													break;
													case ID_heparincheck:
													break;
													case ID_minufrate:
													break;
													case ID_treattime:
													break;
													case ID_bloodratereturn:
													break;
													case ID_bloodratetreat:
													break;
													case ID_tempulimit:
														Cl_alarmThresholdTable.temp3_high_threshold =  (cl_temp.word)/10;
													break;
													case ID_templlimit:
														Cl_alarmThresholdTable.temp3_low_threshold =  (cl_temp.word)/10;
													break;
													case ID_tmpllimit:
													Cl_alarmThresholdTable.tmp_low_threshold =  cl_temp.word;
													break;
													case ID_tmpulimit:
													Cl_alarmThresholdTable.tmp_high_threshold =  cl_temp.word;
													break;
													case ID_vptllimit:
														Cl_alarmThresholdTable.vpt_low_threshold =  cl_temp.word;
													break;
													case ID_vptulimit:
														Cl_alarmThresholdTable.vpt_high_threshold =  cl_temp.word;
													break;
													case ID_ufllimit:
													break;
													case ID_ufulimit:
													break;
													case ID_dflowllimit:
													break;
													case ID_dflowulimit:
													break;
													case ID_condllimit:
													Cl_alarmThresholdTable.cond_low_threshold =  cl_temp.word;
													break;
													case ID_condulimit:
													Cl_alarmThresholdTable.cond_high_threshold =  cl_temp.word;
													break;
													case ID_aptllimit:
														Cl_alarmThresholdTable.apt_low_threshold =  cl_temp.word;
													break;
													case ID_aptulimit:
													Cl_alarmThresholdTable.apt_high_threshold =  cl_temp.word;
													break;

												default:break;
											}
										}
						
			break;
			case EVENT_DLSIS_TICK_50MS:
			
			break;
			
			case EVENT_DLSIS_TICK_500MS:
				//UpdateHeaterControls();
			break;
			case EVENT_DLSIS_TICK_SECOND:
			//	UpdateHeaterControls();
				cl_uf_controller(CL_UF_EVENT_SECOND,0);
				cl_hep_controller(CL_HEP_EVENT_SEC_TICK,0);
				Cl_Dlsis_SenddlsisData();
				DlsisTime.Cl_dlsissecondscounter++;
				if(DlsisTime.Cl_dlsissecondscounter == 60)
				{
					Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
					DlsisTime.Cl_dlsissecondscounter = 0;
					DlsisTime.Cl_dlsisMinutescounter++;
					DlsisTime.Cl_dlsisTotalMinutescounter++;
				}
				if(cl_Dlsis_CheckforDialysisCompletion() == CL_OK )
				{
					
					sv_cntrl_deactivatepump(BLOODPUMP);
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
					sv_cntrl_poweroffheater();
					SetHeaterState(CL_HEATER_STATE_OFF);
					sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
					Cl_DlsisFillingFlowOff();
					cl_uf_controller(CL_UF_EVENT_STOP,0);
					cl_hep_controller(CL_HEP_EVENT_STOP_NORMAL_DELIVERY,0);
					cl_dlsis_state = CL_DLSIS_STATE_POST_DIALYSIS_STANDBY;
				}
			break;
			case EVENT_DLSIS_STOP_DIALYSIS:
			Cl_Dlsis_StopDialysis();
			break;
			case EVENT_DLSIS_PAUSE_DIALYSIS:
			Cl_Dlsis_PauseDialysis();
			cl_dlsis_state = CL_DLSIS_STATE_PAUSED;
			break;
			case EVENT_DLSIS_PAUSE_BYPASS:
			Cl_Dlsis_BypassDialysis();
			break;
			case EVENT_DLSIS_ALARM:
					Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
			break;
			case EVENT_DLSIS_ALERT:
					Cl_Alarm_GetAlarmStatus(FPCURRENTSTATUS,&alarmstatus1);
					if(alarmstatus1)
					{
						//Cl_dlsisretcode = Cl_bc_controller(BC_EVENT_CS);
					}
					Cl_Alarm_GetAlarmStatus(TEMP2_HIGH_THRESHOLD,&alarmstatus3);
					if(alarmstatus3)
					{
						//	UpdateHeaterControls();
					}
			
					Cl_Alarm_GetAlarmStatus(FLOW_HIGH_FLOWRATE,&flowstatus);
					//	if(flowstatus)
					//	{
					//		Cl_rinseretcode = sv_cntrl_poweroffheater();
					//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLOW_OFF",8);
					//	}
			break;


		}
		break;
		
		case CL_DLSIS_STATE_PAUSED:
							switch (cl_dlsis_event)
							{
								case EVENT_DLSIS_START_DIALYSIS:
								Cl_Dlsis_ResumeDialysis();
								break;
								case  EVENT_DLSIS_STOP_DIALYSIS:
								Cl_Dlsis_StopDialysis();
								break;
								default:break;
							}
		
		break;
		case CL_DLSIS_STATE_STOPPED:
							switch (cl_dlsis_event)
							{
								case EVENT_DLSIS_START_DIALYSIS:
								Cl_Dlsis_StartDialysis();
								break;
								default:break;
							}
							
		break;

		case CL_DLSIS_STATE_CRITICAL_ALARM:
					switch (cl_dlsis_event)
					{
						case EVENT_DLSIS_START_DIALYSIS:
						Cl_Dlsis_ResumeDialysis();
						break;
						
						case EVENT_DLSIS_GET_DATA:

							if(Cl_ConsoleRxMsg.msgready == true)
							{
								switch(Cl_ConsoleRxMsg.data.byte[0])
								{
									case	CON_RX_PARAM_DATA_DIALYSIS_STATUS:
									command = CON_TX_COMMAND_SYSDATA;
									dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_STATUS;
									Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
									Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,2);
									break;

									default:
									break;
								}
							}
						break;
						default:break;
					}
		break;
		case CL_DLSIS_STATE_POST_DIALYSIS_STANDBY:
		switch(cl_dlsis_event)
					{
						case EVENT_DLSIS_GET_DATA:

							if(Cl_ConsoleRxMsg.msgready == true)
							{
								switch(Cl_ConsoleRxMsg.data.byte[0])
								{
									case	CON_RX_PARAM_DATA_DIALYSIS_STATUS:
									command = CON_TX_COMMAND_SYSDATA;
									dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
									Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
									Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,2);
									break;
									case	CON_RX_PARAM_DATA_DISINF_STATUS:
									command = CON_TX_COMMAND_SYSDATA;
									dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
									Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
									Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,2);
									break;
									default:
									break;
								}
							}
						break;
			case EVENT_DLSIS_SET_DATA:
			if(Cl_ConsoleRxMsg.msgready == true)
			{
				cl_Datastreamtype cl_temp  ;
				cl_temp.bytearray[0] = Cl_ConsoleRxMsg.data.byte[1];
				cl_temp.bytearray[1] = Cl_ConsoleRxMsg.data.byte[2];
				cl_temp.bytearray[2] = Cl_ConsoleRxMsg.data.byte[3];
				cl_temp.bytearray[3] = Cl_ConsoleRxMsg.data.byte[4];
				
				switch(Cl_ConsoleRxMsg.data.byte[0])
				{
					case ID_dflow:
					break;
					case ID_settemp:
					break;
					case ID_heprate:
					break;
					case ID_setcond:
					break;
					case ID_ufrate:
					break;
					case ID_ufgoal:
					break;
					case ID_bolusvol:
					break;
					case ID_bloodrate:
					break;
					case ID_hepstoptime:
					break;
					case ID_syringetype:
					break;
					case ID_heparincheck:
					break;
					case ID_minufrate:
					break;
					case ID_treattime:
					break;
					case ID_bloodratereturn:
					break;
					case ID_bloodratetreat:
					break;
					case ID_tempulimit:
					Cl_alarmThresholdTable.temp3_high_threshold =  (cl_temp.word)/10;
					break;
					case ID_templlimit:
					Cl_alarmThresholdTable.temp3_low_threshold =  (cl_temp.word)/10;
					break;
					case ID_tmpllimit:
					Cl_alarmThresholdTable.tmp_low_threshold =  cl_temp.word;
					break;
					case ID_tmpulimit:
					Cl_alarmThresholdTable.tmp_high_threshold =  cl_temp.word;
					break;
					case ID_vptllimit:
					Cl_alarmThresholdTable.vpt_low_threshold =  cl_temp.word;
					break;
					case ID_vptulimit:
					Cl_alarmThresholdTable.vpt_high_threshold =  cl_temp.word;
					break;
					case ID_ufllimit:
					break;
					case ID_ufulimit:
					break;
					case ID_dflowllimit:
					break;
					case ID_dflowulimit:
					break;
					case ID_condllimit:
					Cl_alarmThresholdTable.cond_low_threshold =  cl_temp.word;
					break;
					case ID_condulimit:
					Cl_alarmThresholdTable.cond_high_threshold =  cl_temp.word;
					break;
					case ID_aptllimit:
					Cl_alarmThresholdTable.apt_low_threshold =  cl_temp.word;
					break;
					case ID_aptulimit:
					Cl_alarmThresholdTable.apt_high_threshold =  cl_temp.word;
					break;

					default:break;
				}
			}
			
			break;
						default:
						break;
					}
		break;
	}



return CL_OK;
}



Cl_ReturnCodes  cl_dlsis_translatemacevent(MAC_EVENTS Cl_MacRinseEvt,Cl_Dlsis_Eevents* cl_dlsis_event)
{
	switch(Cl_MacRinseEvt)
	{
		case EVT_CONSOLE_COMMAND_GET_DATA:
		*cl_dlsis_event = EVENT_DLSIS_GET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_SET_DATA:
		*cl_dlsis_event = EVENT_DLSIS_SET_DATA;
		break;

		case  EVT_CONSOLE_COMMAND_DIALYSIS_START:
		*cl_dlsis_event= EVENT_DLSIS_START_DIALYSIS;
		break;
		case EVT_CONSOLE_COMMAND_DIALYSIS_STOP:
		*cl_dlsis_event= EVENT_DLSIS_STOP_DIALYSIS;
		break;
		case EVT_CONSOLE_COMMAND_DIALYSIS_PAUSE:
		*cl_dlsis_event= EVENT_DLSIS_PAUSE_DIALYSIS;
		break;
		case EVT_CONSOLE_COMMAND_DIALYSIS_BYPASS:
		*cl_dlsis_event= EVENT_DLSIS_PAUSE_BYPASS;
		break;
		case  EVT_TIMER_EXPIRED:
		
		break;
		case EVT_TICK_50M:
		*cl_dlsis_event =   EVENT_DLSIS_TICK_50MS;
		break;
		case EVT_TICK_500M:
		*cl_dlsis_event = EVENT_DLSIS_TICK_500MS;
		break;
		case EVT_TICK_SEC:
		*cl_dlsis_event =   EVENT_DLSIS_TICK_SECOND;
		break;
		case EVT_TICK_MIN:
		*cl_dlsis_event =   EVENT_DLSIS_TICK_MINUTE;
		break;
		case EVT_TICK_HOUR:
		*cl_dlsis_event =   EVENT_DLSIS_TICK_HOUR;
		break;
		case EVT_ALARM_TRIGGERED:
		*cl_dlsis_event =   EVENT_DLSIS_ALARM;		
		break;
		case EVT_ALERT_TRIGGERED:
		*cl_dlsis_event =   EVENT_DLSIS_ALERT;
		default:
		break;
	}
	return CL_OK;
}



Cl_ReturnCodes Cl_Dlsis_UpdateTimeInfo(void)
{
	
	Cl_ReturnCodes Cl_dlsisretcode = CL_ERROR;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data[7] = {0,0,0,0 ,0,0};
	
	command = CON_TX_COMMAND_REM_TIME;
	data[0] = (uint8_t) DIALYSIS_DATA;
	data[1]= (uint8_t)DlsisTime.Cl_dlsisTotalMinutescounter;
	data[2]= (uint8_t)DlsisTime.Cl_dlsisTotalhourscounter;
	data[3]= (uint8_t)DlsisTime.Cl_dlsissecondscounter;
	data[4]= (uint8_t) (CL_DLSIS_TIMEOUT_MIN - DlsisTime.Cl_dlsisTotalMinutescounter );
	data[5]= (uint8_t) (CL_DLSIS_TIMEOUT_HRS - DlsisTime.Cl_dlsisTotalhourscounter );
	data[6]= (uint8_t) (60 - DlsisTime.Cl_dlsissecondscounter );
	
	Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,7);
	
	return CL_OK;
	}
	
	 Cl_ReturnCodes cl_Dlsis_CheckforDialysisCompletion(void)
	 {
		 Cl_ReturnCodes Cl_dlsisretcode = CL_ERROR;
		 
		 if(DlsisTime.Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
		 {
			 
			 Cl_dlsisretcode = CL_OK;
		 }
		 
		 return (Cl_dlsisretcode);
		 
	 }
	
	Cl_ReturnCodes cl_Dlsis_notifydacandgotorinsestandby(void)
	{
		Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
		
		uint8_t data =0;
		//inform DAC about rinse completed state.
		DlsisTime.Cl_dlsissecondscounter = 0;
		DlsisTime.Cl_dlsisMinutescounter= 0;
		DlsisTime.Cl_dlsishourscounter= 0;
		DlsisTime.Cl_dlsisTotalMinutescounter= 0;
		DlsisTime.Cl_dlsisTotalhourscounter=0;
		
		
		Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYS_COMPLTED,&data,0);
		Cl_dlsisretcode = Cl_mac_apprequesthandler(MACREQ_DIALYSIS_COMPLETED);
		
		return Cl_dlsisretcode;
	}
	
	
Cl_ReturnCodes Cl_Dlsis_UpdateAlarmTable(ClDlsisAlarmIdType * ClRinseAlarmId )
{
	Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
			Cl_NewAlarmIdType cl_alarmId;
	uint8_t tempcount = 0;
			uint8_t data[2];
			Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	for (tempcount = 0 ; tempcount < CL_DLSIS_ALRM_MAX ; tempcount++)
	{
		
		Cl_Alarm_GetAlarmStatus(Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId,&Cl_DlsisAlarmTable[tempcount].IsActive);
		if (Cl_DlsisAlarmTable[tempcount].IsActive)
		{
			if(!Cl_DlsisAlarmTable[tempcount].IsRaised)
				{
				//	cl_alarmId = (uint8_t)Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId;

				//	data[0] = (uint8_t)Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId;
				//	data[1] = (uint8_t) CL_ALARM_TRIGGERED;
				//	data[1] = (uint8_t) CRITICAL;
				//	command = CON_TX_COMMAND_ALARM;
				//	Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,3);
					Cl_DlsisAlarmTable[tempcount].IsRaised = true;
					Cl_dlsisretcode = CL_DlsisAlarmActon(Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId);
					
				
				}
			
		}
		else
			{
				// alarm was present before , but not active now.
				if(Cl_DlsisAlarmTable[tempcount].IsRaised == true)
				{
					Cl_DlsisAlarmTable[tempcount].IsRaised = false;
					command = CON_TX_COMMAND_ALARM;
					data[0] = (uint8_t)(Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId);
					data[1] = (uint8_t) 0;
					Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,2);
				}

			}
		
	
		
		

	}
	

	

return (Cl_dlsisretcode );
}


Cl_ReturnCodes  CL_DlsisAlarmActon(Cl_NewAlarmIdType cl_dlsisalarmid)
{
		Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
			uint8_t data[3] ={0, 0} ,*data1 = NULL;
			uint8_t data3;
			Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
			uint16_t levelswitchstatus = 0;	
			static uint8_t fillseccounter = 0;
			Cl_NewAlarmIdType NewAlarmId = _NO_ALARM;
		

			

							
		switch(cl_dlsisalarmid)
		{
			case BLOODDOOR_STATUS_OPEN:
				Cl_Dlsis_StopDialysis();
			//	NewAlarmId = BLOODDOOR_STATUS_OPEN;
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BPD",3);
				Cl_SysStat_GetSensor_Status_Query(BLOODDOOR_STATUS_OPEN,&levelswitchstatus);
			
			break;
			case HOLDER1STATUS_CLOSED:
			// stop rinsing
			Cl_Dlsis_StopDialysis();
		//	NewAlarmId = HOLDER1STATUS_OPEN;
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HLD1",4);
		
			//enterl_saferinse_state();
			break;
			case HOLDER2STATUS_CLOSED:
			// stop rinsing
			Cl_Dlsis_StopDialysis();
		//	NewAlarmId = HOLDER2STATUS_OPEN;
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HLD2",4);
			//enterl_saferinse_state();
			break;

			case TEMP1_HIGH_THRESHOLD:
			case TEMP1_LOW_THRESHOLD:
		
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP1STATUS,&TmpVal_dls);
		
			temp1_dls = (0.805 * TmpVal_dls) - 1004 ;
			temp2_dls = 3000 + (temp1_dls * 1000)/382;
			if(temp2_dls > 3680)
			{
		//		NewAlarmId = _TEMP1_HIGH_THRESHOLD;
				Cl_Dlsis_StopDialysis();
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
			if(temp2_dls < 3500)
			{
			//	NewAlarmId = _TEMP1_LOW_THRESHOLD;
				Cl_Dlsis_StopDialysis();
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
			//Cl_Dlsis_StopDialysis();
			//enterl_saferinse_state();

			break;
			case TEMP2_HIGH_THRESHOLD:
			case TEMP2_LOW_THRESHOLD:
		
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&TmpVal_dls);
	
			temp1_dls = (0.805 * TmpVal_dls) - 1004 ;
			temp2_dls = 3000 + (temp1_dls * 1000)/382;
			if(temp2_dls > 3680)
			{
		//		NewAlarmId = _TEMP2_HIGH_THRESHOLD;
				Cl_Dlsis_StopDialysis();
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
			if(temp2_dls < 3500)
			{
		//		NewAlarmId = _TEMP2_LOW_THRESHOLD;
				Cl_Dlsis_StopDialysis();
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
			//Cl_Dlsis_StopDialysis();
			//enterl_saferinse_state();

			break;
			case TEMP3_HIGH_THRESHOLD:
			case TEMP3_LOW_THRESHOLD:
				
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&TmpVal_dls);
			int16_t temp3,temp4;
			temp3 = (0.805 * TmpVal_dls) - 1004 ;
			temp4 = 3000 + (temp3 * 1000)/382;
			if(temp4 > 3680)
			{
			//		NewAlarmId = _TEMP3_HIGH_THRESHOLD;
					Cl_Dlsis_StopDialysis();
					Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
					cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
			if(temp4 < 3500)
			{
			//	NewAlarmId = _TEMP3_LOW_THRESHOLD;
				Cl_Dlsis_StopDialysis();
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
			//Cl_Dlsis_StopDialysis();
			//enterl_saferinse_state();

			break;
			case 	PS1_HIGH_THRESHOLD:
			case    PS1_LOW_THRESHOLD:
		//	NewAlarmId = _PS1_HIGH_THRESHOLD;
			Cl_Dlsis_StopDialysis();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			break;
			case 	PS2_HIGH_THRESHOLD:
			case    PS2_LOW_THRESHOLD:
		//	NewAlarmId = _PS2_HIGH_THRESHOLD;
			Cl_Dlsis_StopDialysis();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2",3);
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			break;
			case 	PS3_HIGH_THRESHOLD:
			case    PS3_LOW_THRESHOLD:
			NewAlarmId = _NO_ALARM;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS3",3);
			Cl_Dlsis_StopDialysis();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			break;
			case FLOW_NO_FLOW:
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLW",3);
			Cl_Dlsis_StopDialysis();
		//	NewAlarmId = _FLOW_NO_FLOW;
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			case FLOW_LOW_FLOWRATE:
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLW",3);
		//	NewAlarmId = _FLOW_LOW_FLOWRATE;
			Cl_Dlsis_StopDialysis();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			case FLOW_HIGH_FLOWRATE:
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLW",3);
			//	NewAlarmId = _FLOW_LOW_FLOWRATE;
			Cl_Dlsis_StopDialysis();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			case COND_STATUS_LOW:
	//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_LOW",8);
			//	NewAlarmId = _COND_STATUS_LOW;
				Cl_Dlsis_StopDialysis();
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			case COND_STATUS_HIGH:
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_HIGH",9);
					//		NewAlarmId = _COND_STATUS_HIGH;
							Cl_Dlsis_StopDialysis();
							cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			case COND_DAC_OPEN:
				// air trapped in DAC2 or no flow
	//				 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_OPEN",8);
			//		NewAlarmId = _COND_DAC_OPEN;
					Cl_Dlsis_StopDialysis();
					cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
		//	case COND_DAC_RO:
				// RO water filled DAC2
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_RO",6);
			//				NewAlarmId = _COND_DAC_RO;
				//			Cl_Dlsis_StopDialysis();
					//		cl_rinsestate = CL_DLSIS_STATE_CRITICAL_ALARM;
			//break;
			case COND_DAC_HIGH:
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_HIGH",8);
				//					NewAlarmId = _COND_DAC_HIGH;
				//					Cl_Dlsis_StopDialysis();
					//				cl_rinsestate = CL_DLSIS_STATE_CRITICAL_ALARM;
				// Rinse incomplete.
			break;
			case 	UFP_OVERRUN:
			case	UFP_UNDERRUN:
			//		NewAlarmId = _UF_ALARM_FOR_RINSE;
					Cl_Dlsis_StopDialysis();
					cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			case 	BC_OVERRUN:
			case	BC_UNDERRUN:
			case	BC_FAILED:
				//	NewAlarmId = _BC_ALARM;
					Cl_Dlsis_StopDialysis();
					cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;

			case DE_CHAMBER_LOW: //23
			break;
			case SYSTEM_NOT_READY: //24
			break;
			case POWER_FAILURE: //25
			break;

			case WATCHDOG_TIMER: //27
			//	NewAlarmId = _WATCHDOG_TIMER;
			break;
			default:
			break;

		}
		 if(NewAlarmId != _NO_ALARM)
		 {
			data[0] = (uint8_t)NewAlarmId;
			data[1] = (uint8_t) CL_ALARM_TRIGGERED;
			data[2] = (uint8_t)CRITICAL;
			command = CON_TX_COMMAND_ALARM;
			cl_wait(10);
			Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,3);
			
					
			command = CON_TX_COMMAND_SYS_STATE;
			data[0] = (uint8_t)CLEANING;
			data[1] = (uint8_t)CLEAN_CLEAN_ALARM_STOPPED;
			Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,2);
		 }
			
}


Cl_ReturnCodes Cl_Dlsis_StopDialysis(void)
{
	uint8_t data;
	Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
	
				if(!((cl_dlsis_state == CL_DLSIS_STATE_IDLE ) || (cl_dlsis_state == CL_DLSIS_STATE_STOPPED )|| (cl_dlsis_state == CL_DLSIS_STATE_PAUSED ) ||(cl_dlsis_state == CL_DLSIS_STATE_CRITICAL_ALARM )  ))
				{

					
					 	DlsisTime.Cl_dlsissecondscounter = 0;
					 	DlsisTime.Cl_dlsisMinutescounter= 0;
					 	DlsisTime.Cl_dlsishourscounter= 0;
					 	DlsisTime.Cl_dlsisTotalMinutescounter= 0;
					 	DlsisTime.Cl_dlsisTotalhourscounter=0;

				Cl_dlsisretcode =  sv_cntrl_deactivatevenousclamp();
				Cl_dlsisretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
				Cl_dlsisretcode = sv_cntrl_poweroffheater();
				Cl_dlsisretcode = SetHeaterState(CL_HEATER_STATE_OFF);
				cl_bp_controller(CL_BP_EVENT_STOP,0);
				Cl_dlsisretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
				cl_dlsis_state = CL_DLSIS_STATE_IDLE;
				Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_STOPPED,&data,0);
				
					
				}
}

Cl_ReturnCodes Cl_Dlsis_BypassDialysis(void)
{
			uint8_t data;
			Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
			Cl_dlsisretcode = sv_cntrl_enable_bypass();
			

			cl_dlsis_state = CL_DLSIS_STATE_BYPASS;
			Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_PAUSED,&data,0);
	
}

Cl_ReturnCodes Cl_Dlsis_PauseDialysis(void)
{
		uint8_t data;
		Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
		
		if(!(cl_dlsis_state == CL_DLSIS_STATE_IDLE ) )
		
		switch (cl_dlsis_state)
		{
			case CL_DLSIS_STATE_WAIT_FOR_TEMP_COND_STABILITY:
			case CL_DLSIS_STATE_WAIT_FOR_BLOOD_DETECT:
			case CL_DLSIS_STATE_DIALYSIS:
			case CL_DLSIS_STATE_UF_ACTIVATION:
			
	
					Cl_dlsisretcode = sv_cntrl_enable_bypass();
					
					//	Cl_dlsisretcode =  sv_cntrl_deactivatevenousclamp();
					//	Cl_dlsisretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
					//	Cl_dlsisretcode = sv_cntrl_poweroffheater();
					//	Cl_dlsisretcode = SetHeaterState(CL_HEATER_STATE_OFF);
					//	cl_bp_controller(CL_BP_EVENT_STOP,0);
					//	Cl_dlsisretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
						cl_dlsis_state = CL_DLSIS_STATE_PAUSED;
						Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_PAUSED,&data,0);
			
			break;
			default:break;
		}

	
}
Cl_ReturnCodes Cl_Dlsis_ProcessAlarms( )
{
		Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
		ClDlsisAlarmIdType  ClDlsisAlarmId;
		uint8_t data;
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		Cl_NewAlarmIdType cl_alarmid;
		
	   Cl_dlsisretcode = Cl_Dlsis_UpdateAlarmTable(&ClDlsisAlarmId);
	//   	Cl_rinseretcode =	Cl_Alarm_GetLastAlarm(&cl_rinsealarmid);
	//   	data = (uint8_t)cl_rinsealarmid;
	//   	command = CON_TX_COMMAND_ALARM;
	//   	Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
		   
		   return (Cl_dlsisretcode );
	   	
}


Cl_ReturnCodes  Cl_DlsisFillingFlowOn(void )
{
	Cl_ReturnCodes Cl_RetVal = CL_OK;
	sv_cntrl_activate_valve(VALVE_ID1);
	//cl_gfillinprogress = true;
	return Cl_RetVal;
	
}

Cl_ReturnCodes  Cl_DlsisFillingFlowOff(void )
{
	Cl_ReturnCodes Cl_RetVal = CL_OK;
	sv_cntrl_deactivate_valve(VALVE_ID1);
	//cl_gfillinprogress = false;
	return Cl_RetVal;
	
}


Cl_ReturnCodes Cl_Dlsis_SenddlsisData(void)
{

	uint8_t systemdataarray[40] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cl_DlsisDatatype tempdata;
	uint16_t temp, temp1;
	static float avgtmp3;
	static timecount = 0;
	Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
	uint8_t count = 0;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint16_t sensordatamillivolts;
	//Cl_Console_bulkdatatype
	
	
						
	command = CON_TX_COMMAND_SYS_STATE_DATA ;
	systemdataarray[0] = DIALYSIS_DATA;
	count++;


					Cl_SysStat_GetSensor_Status_Query(COND_STATUS_HIGH,&temp);
					{
						tempdata.word = temp;

						if( temp < 0)
						{
							temp = 0;
							//	avgcond = 0;
						}
						if( temp > 2400)
						{
							tempdata.word = temp/20 + 9;
						}
						else
						{
							tempdata.word = 100;
						}
						tempdata.word = 139;
						systemdataarray[count++] = tempdata.bytearray[0];
						systemdataarray[count++] = tempdata.bytearray[1];
						systemdataarray[count++] = tempdata.bytearray[2];
						systemdataarray[count++] = tempdata.bytearray[3];
					}
					

		

		Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
		{
			tempdata.word = 0;
			float ftemp,ftemp1;
			ftemp = tempdata.word * 0.805;
			ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.2;
			//avgtmp3 =	dummy3(avgtmp3*5 + ftemp1)/6;
			avgtmp3 = dummy3 ;
			tempdata.word = (uint16_t)(avgtmp3 * 10);
			systemdataarray[count++] = tempdata.bytearray[0];
			systemdataarray[count++] = tempdata.bytearray[1];
			systemdataarray[count++] = tempdata.bytearray[2];
			systemdataarray[count++] = tempdata.bytearray[3];
		}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_APTSTATUS,&tempdata.Twobyte);
			{
		
				
				//	float ftemp,ftemp1;
				//	ftemp = data.twobytedata * 0.805;
				//	ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.6;
				//	avgtmp3 =	(avgtmp3*5 + ftemp1)/6;
				//	data.twobytedata = (uint16_t)(avgtmp3 * 100);
				sensordatamillivolts = (tempdata.Twobyte * 0.793) ;
				calibration_apt(sensordatamillivolts);
				tempdata.word	 = pressure_final_apt;
				systemdataarray[count++] = tempdata.bytearray[0] ;
				systemdataarray[count++] = tempdata.bytearray[1] ;
				systemdataarray[count++] = tempdata.bytearray[2] ;
				systemdataarray[count++] = tempdata.bytearray[3] ;
			}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_VPTSTATUS,&tempdata.Twobyte);
			{
				
				//	float ftemp,ftemp1;
				//	ftemp = data.twobytedata * 0.805;
				//	ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.6;
				//	avgtmp3 =	(avgtmp3*5 + ftemp1)/6;
				//	data.twobytedata = (uint16_t)(avgtmp3 * 100);
				sensordatamillivolts = (tempdata.Twobyte * 0.793) ;
				calibration_apt(sensordatamillivolts);
				tempdata.word	 = pressure_final_vpt;
				tempdata.word	 = 30 * 100;
				systemdataarray[count++] = tempdata.bytearray[0] ;
				systemdataarray[count++] = tempdata.bytearray[1] ;
				systemdataarray[count++] = tempdata.bytearray[2] ;
				systemdataarray[count++] = tempdata.bytearray[3] ;
			}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS1STATUS,&tempdata.Twobyte);
			sensordatamillivolts = (tempdata.Twobyte * 0.793) ;
			calibration_apt(sensordatamillivolts);
	
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS2STATUS,&tempdata.Twobyte);
			sensordatamillivolts = (tempdata.Twobyte * 0.793) ;
			calibration_apt(sensordatamillivolts);
			tempdata.word	 = ((pressure_final_apt + pressure_final_vpt ) - (pressure_final_ps1+pressure_final_ps2))/2;


			systemdataarray[count++] = tempdata.bytearray[0] ;
			systemdataarray[count++] = tempdata.bytearray[1] ;
			systemdataarray[count++] = tempdata.bytearray[2] ;
			systemdataarray[count++] = tempdata.bytearray[3] ;
			
			systemdataarray[count++] = 0x0A; // tmp
			systemdataarray[count++] = 0x0A; // tmp
			systemdataarray[count++] = 0x00;
			systemdataarray[count++] = 0x00;
Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_STATE_DATA,&systemdataarray,count);
	//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&systemdataarray,count);
	
	
}




Cl_ReturnCodes Cl_Dlsis_Get_data(Cl_ConsoleRxDataType DataId, uint8_t size)
{
	Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t dataarray[8] =  {0,0,0,0};
	uint8_t  datasize = 0;
	
	if(DataId == CON_RX_PARAM_DATA_TREATMENT_DATA )
	{
		//Cl_Dprep_SendtreatementData();
	}
	else
	{
		switch (DataId)
		{

			case	CON_RX_PARAM_DATA_RINSE_STATUS:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;

			dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
			datasize = 2;

			break;
			case	CON_RX_PARAM_DATA_PRIME_STATUS:
			//	cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;

			dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
			datasize = 2;

			break;
			
			case	CON_RX_PARAM_DATA_DIALYSIS_STATUS:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;

			dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_STATUS;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
			datasize = 2;

			break;
			case	CON_RX_PARAM_DATA_DIALYSIS_FLOW:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;

			dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);
			datasize = 2;
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,3);
			//cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_DIALYSIS_TEMP:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);
			datasize = 2;
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_HEPARIN_RATE:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE;
			//				Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE, &dataarray[1]);
			
			datasize = 2;
			break;
			case	CON_RX_PARAM_DATA_APT_VALUE:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE;
			//			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			datasize = 2;
			break;
			case	CON_RX_PARAM_DATA_VPT_VALUE:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE;
			//	Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			datasize = 2;
			break;
			case	CON_RX_PARAM_DATA_TMP_VALUE:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_TMP_VALUE;
			//			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			datasize = 2;
			break;
			case	CON_RX_PARAM_DATA_COND_VALUE:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE;
			//				Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			datasize = 2;
			break;
			case	CON_RX_PARAM_DATA_UF_RATE:
			//	cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_UF_RATE;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//	cl_wait(10);
			datasize = 2;
			break;
			case	CON_RX_PARAM_DATA_ABF_RATE:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			datasize = 2;
			break;
			#if 0
			case	CON_RX_PARAM_DATA_ABD_THRESHOLD:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_ABD_THRESHOLD;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_ABD_THRESHOLD, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_BLD_THRESHOLD:
			//cl_wait(10);
			
			command = CON_TX_COMMAND_SYSDATA;
			//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
			dataarray[0] = CON_TX_PARAM_DATA_BLD_THRESHOLD;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_BLD_THRESHOLD, &dataarray[1]);
			
			//Reset the  OPENFILL time count
			//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,1);
			//cl_wait(10);
			datasize = 2;
			break;
			#endif
			
			
			case CON_RX_PARAM_DATA_DIALYSIS_FLOW_CONFIG: //0x13
			
			
			
			
			dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_FLOW_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_FLOW_CONFIG_LOWER, &dataarray[5]);
			
			datasize = 	7;
			
			break;
			case CON_RX_PARAM_DATA_DIALYSIS_TEMP_CONFIG://0x14
			

			dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_TEMP_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_TEMP_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_HEPARIN_RATE_CONFIG://0x15

			dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_APT_VALUE_CONFIG://0x16
			
			dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			
			break;
			case CON_RX_PARAM_DATA_VPT_VALUE_CONFIG://0x17

			dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_TMP_VALUE_CONFIG://0x18

			dataarray[0] = CON_TX_PARAM_DATA_TMP_VALUE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_TMP, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_TMP_VALUE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_TMP_VALUE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_COND_VALUE_CONFIG://0x19

			dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_COND_VALUE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_COND_VALUE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_UF_RATE_CONFIG://0x1A

			dataarray[0] = CON_TX_PARAM_DATA_UF_RATE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_RATE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_RATE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_UF_GOAL_CONFIG://0x1B,

			dataarray[0] = CON_TX_PARAM_DATA_UF_GOAL_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_BOLUS_VOLUME_CONFIG://1C

			dataarray[0] = CON_TX_PARAM_DATA_BOLUS_VOLUME_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_BOLUS_VOLUME_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_BOLUS_VOLUME_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			case CON_RX_PARAM_DATA_ABF_RATE_CONFIG://0X1D

			dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE_CONFIG;
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &dataarray[1]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_ABF_RATE_CONFIG_UPPER, &dataarray[3]);
			Cl_dlsisretcode = (uint8_t)sv_nvmgetdata(NV_ABF_RATE_CONFIG_LOWER, &dataarray[5]);
			datasize = 	7;
			break;
			default:
			break;
		}
		command = CON_TX_COMMAND_SYSDATA;
		Cl_dlsisretcode = Cl_SendDatatoconsole(command,&dataarray,datasize);
		
	}

}



Cl_ReturnCodes	Cl_Dlsis_StartDialysis(void)
{
	
	
	Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data = 0;
	
	

	//Reset the  OPENFILL time count
	DlsisTime.Cl_dlsisMinutescounter = 0;
	DlsisTime.Cl_dlsisTotalMinutescounter = 0;
	Cl_dlsisOpenFillTimeOut = false;
	
		
	Cl_Dlsis_ResumeDialysis();
	
	return 0;

}

Cl_ReturnCodes	Cl_Dlsis_ResumeDialysis(void)
{
	
		Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		uint8_t data = 0;
									command = CON_TX_COMMAND_DLSIS_CNFRM;
									data = (uint8_t)COMMAND_RESULT_SUCCESS;
									Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,1);
									command = CON_TX_COMMAND_DLSIS_STARTED;
									data = (uint8_t)COMMAND_RESULT_SUCCESS;
									Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,1);
									
									Cl_dlsisretcode =  Cl_AlarmConfigureAlarmType(BLOODDOOR_STATUS_OPEN,LOGIC_LOW,0,0,0);
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(BLOODDOOR_STATUS_OPEN,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF ,true);
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms( HOLDER1STATUS_CLOSED,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms( HOLDER2STATUS_CLOSED,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(COND_STATUS_LOW,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(COND_DAC_OPEN,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(COND_DAC_RO,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(COND_DAC_HIGH,true );
									//	Cl_dlsisretcode =  Cl_AlarmActivateAlarms(HP_ ,true);
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms( ABD_EVENT,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms( BD_EVENT,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(APTSTATUS_HIGH,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_HIGH ,true);
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(APTSTATUS_LOW,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_LOW,true);
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms( BLD_EVENT,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS2_HIGH_THRESHOLD,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS2_LOW_THRESHOLD,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS1_HIGH_THRESHOLD,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS1_LOW_THRESHOLD,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS3_HIGH_THRESHOLD ,true);
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS3_LOW_THRESHOLD ,true);
									
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(SENSOR_TEMP3STATUS,true );
									Cl_dlsisretcode =  Cl_AlarmActivateAlarms(SENSOR_TEMP2STATUS,true );
									
									if(Current_sense_trigger)
									{
										Cl_dlsisretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
									}
									else
									{
										//			Cl_rinseretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );
									}
									sv_cntrl_disable_loopback();
									sv_cntrl_enable_bypass();
									
									cl_bp_controller(CL_BP_EVENT_START,0);
									cl_hep_controller(CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE,5000);
									cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,0);
									
									Cl_dlsisretcode =  sv_cntrl_activatepump(DCMOTOR1);
									Cl_dlsisretcode =  sv_cntrl_activatepump(DCMOTOR2);
									Cl_dlsisretcode = Cl_bc_controller(BC_EVENT_RESUME);
									Cl_dlsisretcode = SetHeaterState(CL_HEATER_STATE_ON);

									//	cl_bp_controller(CL_BP_EVENT_START,0);
									//cl_hep_controller(CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE,420);
									//	cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,0);
									//	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,2350);
									//	cl_uf_controller(CL_UF_EVENT_START,0);
									Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
									//	cl_dlsis_state = CL_DLSIS_STATE_DIALYSIS;
									cl_dlsis_state = CL_DLSIS_STATE_WAIT_FOR_TEMP_COND_STABILITY;
									
									return 0;
	
}