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
#include "cl_app/cl_cal/calibration.h"

extern uint8_t sv_cs_setpotvalue(uint32_t resistance) ;
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint32_t Treatdata[ID_MAX_TREAT_PARAM] ;
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
extern uint8_t sv_cntrl_setredalarm(void);
extern uint8_t sv_cntrl_setgreenalarm(void);
extern uint8_t sv_cntrl_setyellowalarm(void);
extern uint8_t sv_cntrl_buzzer(void);
extern uint8_t sv_cntrl_resetredalarm(void);
extern uint8_t sv_cntrl_resetgreenalarm(void);
extern uint8_t sv_cntrl_resetyellowalarm(void);
extern uint8_t sv_cntrl_nobuzzer(void);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t sv_cntrl_disable_loopback(void);
extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
extern void calibration_apt(uint16_t sensordata);
Cl_ReturnCodes Cl_Dlsis_ResetAlertsforReassertion(void );
Cl_ReturnCodes dlsis_check_alarm(Cl_NewAlarmIdType dlsisalarm);
extern Cl_AlarmThresholdType  Cl_alarmThresholdTable;
extern Cl_Sys_statusType cl_sys_statbuffer;
extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern bool BC_window; //test
extern bool g_testbcfreeze; //test
extern bool Current_sense_trigger; // test
extern float dummy3;
extern  Cl_Dlsis_States cl_dlsis_state_dummy;
//extern volatile int16_t pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;
Cl_Dlsis_States cl_dlsis_state = CL_DLSIS_STATE_IDLE;
//Cl_Rinse_States cl_dlsis_state = CL_DLSIS_STATE_IDLE;
volatile uint8_t flag_bc = 1;
 DlsisTimeType DlsisTime ;
 extern float apt;
extern float vpt;
 int Cl_dlsissecondscounter =0 ,Cl_dlsisMinutescounter=0, Cl_dlsishourscounter=0;
 int Cl_dlsisTotalMinutescounter=0, Cl_dlsisTotalhourscounter=0;
bool Cl_dlsisOpenFillTimeOut = false, heater_stable_status = false;
 uint16_t dlis_wait_cnt;

 uint16_t wait_cnt_dls = 0,TmpVal_dls =0;
 int16_t temp1_dls ,temp2_dls;
extern volatile float temprature_final_value_1,temprature_final_value_2,temprature_final_value_3;
extern volatile float pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;
extern volatile float cond_final_cs3;
// uint16_t wait_cnt_dls = 0,TmpVal_dls =0;
// int16_t temp1_dls ,temp2_dls;

float dlsis_ps1 = 0, dlsis_ps2=0, dlsis_ps3=0,dlsis_apt = 0, dlsis_vpt=0;
float dlsis_temp1=0,dlsis_temp2=0,dlsis_temp3=0,dlsis_cond=0;
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
 extern uint8_t sv_cntrl_activatevenousclamp(void);
 extern uint8_t sv_cntrl_deactivatevenousclamp(void);
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
Cl_ReturnCodes	Cl_Dlsis_recoveredsate(void);
Cl_ReturnCodes	Cl_Dlsis_safesate(void);
Cl_ReturnCodes cl_temp_safestate(void);
Cl_ReturnCodes cl_temp_recoveredstate(void);
Cl_ReturnCodes cl_cond_safestate(void);
Cl_ReturnCodes cl_cond_recoveredstate(void);
Cl_NewAlarmIdType dlsisalarm;	
Cl_NewAlarmIdType dlsisalarm1;	
Cl_ReturnCodes Cl_Dlsis_BypassOff(void);
DlsisAlarmsType Cl_DlsisAlarmTable[CL_DLSIS_ALRM_MAX] = 
{
{HOLDER1STATUS_CLOSED,CL_ALARM_ALARM,false,false,false},
{HOLDER2STATUS_CLOSED,CL_ALARM_ALARM,false,false,false},		
{BLOODDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
{PS1_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{PS1_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{PS2_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{PS2_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{PS3_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{PS3_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{BD_EVENT,CL_ALARM_ALARM,false,false,false},
{ABD_EVENT,CL_ALARM_ALARM,false,false,false},
{BLD_EVENT,CL_ALARM_ALARM,false,false,false},
{APTSTATUS_HIGH,CL_ALARM_ALARM,false,false,false},
{APTSTATUS_LOW,CL_ALARM_ALARM,false,false,false},
{VPTSTATUS_HIGH,CL_ALARM_ALARM,false,false,false},
{VPTSTATUS_LOW,CL_ALARM_ALARM,false,false,false},
{COND_STATUS_LOW,CL_ALARM_ALARM,false,false,true},
{COND_STATUS_HIGH,CL_ALARM_ALARM,false,false,true},
{TEMP3_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{TEMP3_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{TEMP2_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{TEMP2_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{TEMP1_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
{TEMP1_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
//{PS4STATUS,false,false,false},
/*{TEMP1_HIGH_THRESHOLD,false,false,false},
{TEMP1_LOW_THRESHOLD,false,false,false},
{TEMP2_HIGH_THRESHOLD,false,false,false},
{TEMP2_LOW_THRESHOLD,false,false,false},
{TEMP3_HIGH_THRESHOLD,false,false,false},
{TEMP3_LOW_THRESHOLD,false,false,false},
{FPCURRENTSTATUS,false,false,false},
*/
};
volatile uint8_t var=0;
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
uint16_t tempcount=0;
float temp;
//static float apt=0, vpt=0;

Cl_dlsisretcode =  cl_dlsis_translatemacevent( Cl_MacDlsisEvent, &cl_dlsis_event);
	
	switch(cl_dlsis_event)
	{
		case EVENT_DLSIS_TICK_SECOND:
		Cl_dlsisretcode = Cl_bc_controller(BC_EVENT_SECOND);
		break;
		case EVENT_DLSIS_TICK_50MS:
		//Cl_dlsisretcode = Cl_bc_controller(BC_EVENT_50MS);
		break;
		default:break;
		
	}
	if(cl_dlsis_event == EVENT_DLSIS_TICK_SECOND)
	{

		//Cl_dlsisretcode =  Cl_AlarmResetAlarm( TEMP3_HIGH_THRESHOLD);
		//Cl_dlsisretcode =  Cl_AlarmResetAlarm( TEMP3_LOW_THRESHOLD);
		//Cl_dlsisretcode =  Cl_AlarmResetAlarm( TEMP2_HIGH_THRESHOLD);
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( FLOW_NO_FLOW );
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( FLOW_HIGH_FLOWRATE );
		Cl_dlsisretcode =  Cl_AlarmResetAlarm( FLOW_LOW_FLOWRATE );
		
		cl_dlsis_state_dummy = cl_dlsis_state;
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
						
						Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BLOOD_DL",8);
						//	cl_bp_controller(CL_BP_EVENT_START,0);
							//cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,500);
							//cl_wait(100);
							//cl_hep_controller(CL_HEP_EVENT_SET_NORMAL_DELIVERY_RATE,1000);
							//cl_wait(100);
							cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,1000);
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
			Cl_Dlsis_ResetAlertsforReassertion();
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
			Cl_Dlsis_ResetAlertsforReassertion();
			//	UpdateHeaterControls();
				cl_uf_controller(CL_UF_EVENT_SECOND,0);
				cl_hep_controller(CL_HEP_EVENT_SEC_TICK,0);
				Cl_Dlsis_SenddlsisData();
				DlsisTime.Cl_dlsissecondscounter++;
				if ((DlsisTime.Cl_dlsissecondscounter == 30)&&(flag_bc == 1))
				{
					sv_cs_setpotvalue(1700);
					flag_bc =0 ;
				}
				if(DlsisTime.Cl_dlsissecondscounter == 60)
				{
					
					Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
					DlsisTime.Cl_dlsissecondscounter = 0;
					DlsisTime.Cl_dlsisMinutescounter++;
						if ((DlsisTime.Cl_dlsisMinutescounter == 3)) 
						{
							sv_cs_setpotvalue(2000);
							flag_bc =0 ;
						}
                          if (DlsisTime.Cl_dlsisMinutescounter == 1)
			                {
						
									uint16_t temp_ps=0, low_threshold=0, high_threshold=0;
									temp_ps=apt;
									Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"APT",4);
									Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp_ps,2);
						
										Cl_alarmThresholdTable.apt_high_threshold = apt + 50;
										Cl_alarmThresholdTable.apt_low_threshold = apt - 50;
										low_threshold = Cl_alarmThresholdTable.apt_low_threshold;
										high_threshold = Cl_alarmThresholdTable.apt_high_threshold;
						
										Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&low_threshold,2);
										Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&high_threshold,2);
						
										Cl_alarmThresholdTable.vpt_high_threshold = vpt + 50;
										Cl_alarmThresholdTable.vpt_low_threshold = vpt - 50;
							}
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
			case EVENT_DLSIS_BYPASS_ON:
			Cl_Dlsis_BypassDialysis();
			break;
			case EVENT_DLSIS_BYPASS_OFF:
			Cl_Dlsis_BypassOff();
			break;

			case EVENT_DLSIS__MINUF_ON:
			break;
			case EVENT_DLSIS__MINUF_OFF:
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
		case CL_DLSIS_STATE_BYPASS:
									switch (cl_dlsis_event)
									{
										case EVENT_DLSIS_ALARM:
										Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
										break;
										case EVENT_DLSIS_BYPASS_OFF:
										Cl_Dlsis_BypassOff();
										break;
										default:break;
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
						case EVENT_DLSIS__START_RETURN:
						break;
						case EVENT_DLSIS__STOP_RETURN:
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
						case EVENT_DLSIS_TICK_SECOND:
						
							for (tempcount = 0 ; tempcount < CL_DLSIS_ALRM_MAX ; tempcount++)
							{
								//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER",8);
								Cl_Alarm_GetAlarmStatus(Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId,&Cl_DlsisAlarmTable[tempcount].IsActive);
								if (Cl_DlsisAlarmTable[tempcount].IsActive)
								{
										Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_1",8);
										dlsisalarm = Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId;
										dlsis_check_alarm(dlsisalarm);
										dlsisalarm1 = dlsisalarm;
										var=1;
										cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
										tempcount=0;
										break;	
								}
								if (var == 1)
								{
									Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RECOVERY",8);
									dlsis_check_alarm(dlsisalarm1);
									var = 0;
								}
								if (tempcount == CL_DLSIS_ALRM_MAX - 1)
								{
									if(!Cl_DlsisAlarmTable[tempcount].IsActive)
									{
										Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"SAFE",8);
										sv_cntrl_resetredalarm();
										sv_cntrl_resetyellowalarm();
										sv_cntrl_nobuzzer();
										cl_bp_controller(CL_BP_EVENT_START,0);
										sv_cntrl_activatevenousclamp();
										//Cl_Dlsis_StartDialysis();
										cl_dlsis_state = CL_DLSIS_STATE_DIALYSIS;
									}
								}
							}
						/*	if (dlsisalarm != _NO_ALARM)
							{
								Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_1",8);
								dlsis_check_alarm(dlsisalarm);
							}*/
						break;
						case EVENT_DLSIS_ALARM:
						Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
						break;
						
						default:break;
					}
		break;
		case CL_DLSIS_STATE_POST_DIALYSIS_STANDBY:
		switch(cl_dlsis_event)
					{
						case EVENT_DLSIS__START_RETURN:
						break;
						case EVENT_DLSIS__STOP_RETURN:
						break;
						
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


Cl_ReturnCodes dlsis_check_alarm(Cl_NewAlarmIdType dlsisalarm)
{
	float temp;
	uint8_t temp1;
	uint8_t dlsis;
	switch (dlsisalarm)
	{
		//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_2",8);
		
		case BLOODDOOR_STATUS_OPEN:
				Cl_SysStat_GetSensor_Status_Query(SENSOR_BLOODDOOR_STATUS,&temp1);
				if (temp1 == 1)
				{
					Cl_Dlsis_safesate();
				}
				else
				{
					Cl_Dlsis_recoveredsate();
					cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
				}	
		break;
		
		case HOLDER1STATUS_CLOSED:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_2",8);
			Cl_SysStat_GetSensor_Status_Query(SENSOR_HOLDER1STATUS,&dlsis);
			if (dlsis == 0)
			{
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
				cl_temp_safestate();
			}
			else
			{
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
				cl_temp_recoveredstate();
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
		break;
		
		case HOLDER2STATUS_CLOSED:
		Cl_SysStat_GetSensor_Status_Query(SENSOR_HOLDER2STATUS,&dlsis);
		if (dlsis == 0)
		{
			cl_temp_safestate();
		}
		else
		{
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case BD_EVENT:
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
			Cl_SysStat_GetSensor_Status_Query(SENSOR_BDSTATUS,&temp1);
			if (temp1 == 0)
			{
				Cl_Dlsis_StartDialysis();
				cl_dlsis_state = CL_DLSIS_STATE_DIALYSIS;
			}
			else
			{
				Cl_Dlsis_StopDialysis();
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
		break;
		
		case ABD_EVENT:
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
			Cl_SysStat_GetSensor_Status_Query(SENSOR_ABDSTATUS,&temp1);
			if (temp1 == 0)
			{
				Cl_Dlsis_safesate();
			}
			else
			{
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
				Cl_Dlsis_recoveredsate();
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
		break;
		
		case BLD_EVENT:
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
			Cl_SysStat_GetSensor_Status_Query(SENSOR_BLDSTATUS,&temp1);
			if (temp1 == 0)
			{
				Cl_Dlsis_safesate();
			}
			else
			{
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
				Cl_Dlsis_recoveredsate();
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			}
		break;
		
		case COND_STATUS_HIGH:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.cond_status * 3300/4096);
		calibration_cond(temp);
		temp = (cl_sys_statbuffer.Temp3status * 3300/4096);
		calibration_tmp(temp,TS3);
		dlsis_cond= cond_final_cs3/(1+(temprature_final_value_3-25.0)*0.021);
		dlsis_cond=dlsis_cond/10;
		uint16_t var1;
		var1=dlsis_cond;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
		if (dlsis_cond > Cl_alarmThresholdTable.cond_high_threshold)
		{
			cl_cond_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_cond_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case COND_STATUS_LOW:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.cond_status * 3300/4096);
		calibration_cond(temp);
		temp = (cl_sys_statbuffer.Temp3status * 3300/4096);
		calibration_tmp(temp,TS3);
		dlsis_cond= cond_final_cs3/(1+(temprature_final_value_3-25.0)*0.021);
		dlsis_cond=dlsis_cond/10;
		if (dlsis_cond < Cl_alarmThresholdTable.cond_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_cond;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_cond_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_cond_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case PS1_HIGH_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.ps1status * 3300/4096);
		calibration_ps1(temp);
		dlsis_ps1 = pressure_final_ps1;
		if (dlsis_ps1 > Cl_alarmThresholdTable.ps1_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_ps1;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case PS1_LOW_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.ps1status * 3300/4096);
		calibration_ps1(temp);
		dlsis_ps1 = pressure_final_ps1;
		if (dlsis_ps1 < Cl_alarmThresholdTable.ps1_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_ps1;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case PS2_HIGH_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.ps2status * 3300/4096);
		calibration_ps2(temp);
		dlsis_ps2 = pressure_final_ps2;
		if (dlsis_ps2 > Cl_alarmThresholdTable.ps2_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_ps2;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case PS2_LOW_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.ps2status * 3300/4096);
		calibration_ps2(temp);
		dlsis_ps2 = pressure_final_ps2;
		if (dlsis_ps2 < Cl_alarmThresholdTable.ps2_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_ps2;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case PS3_HIGH_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.ps3status * 3300/4096);
		calibration_ps3(temp);
		dlsis_ps3 = pressure_final_ps3;
		if (dlsis_ps3 > Cl_alarmThresholdTable.ps3_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_ps3;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case PS3_LOW_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.ps3status * 3300/4096);
		calibration_ps3(temp);
		dlsis_ps3 = pressure_final_ps3;
		if (dlsis_ps3 < Cl_alarmThresholdTable.ps3_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_ps3;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		case APTSTATUS_HIGH:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.aptstatus * 3300/4096);
		calibration_apt(temp);
		dlsis_apt = pressure_final_apt;
		if (dlsis_apt > Cl_alarmThresholdTable.apt_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_apt;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			Cl_Dlsis_safesate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			Cl_Dlsis_recoveredsate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case APTSTATUS_LOW:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.aptstatus * 3300/4096);
		calibration_apt(temp);
		dlsis_apt = pressure_final_apt;
		if (dlsis_apt < Cl_alarmThresholdTable.apt_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_apt;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			Cl_Dlsis_safesate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			Cl_Dlsis_recoveredsate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case VPTSTATUS_HIGH:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.vptstatus * 3300/4096);
		calibration_vpt(temp);
		dlsis_vpt = pressure_final_vpt;
		if (dlsis_vpt > Cl_alarmThresholdTable.vpt_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_vpt;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			Cl_Dlsis_safesate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			Cl_Dlsis_recoveredsate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case VPTSTATUS_LOW:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.vptstatus * 3300/4096);
		calibration_vpt(temp);
		dlsis_vpt = pressure_final_vpt;
		if (dlsis_vpt < Cl_alarmThresholdTable.vpt_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_vpt;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			Cl_Dlsis_safesate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			Cl_Dlsis_recoveredsate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		case TEMP3_HIGH_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.Temp3status * 3300/4096);
		calibration_tmp(temp,TS3);
		dlsis_temp3 = temprature_final_value_3;
		if (dlsis_temp3 > Cl_alarmThresholdTable.temp3_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_temp3;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		case TEMP3_LOW_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.Temp3status * 3300/4096);
		calibration_tmp(temp,TS3);
		dlsis_temp3 = temprature_final_value_3;
		if (dlsis_temp3 < Cl_alarmThresholdTable.temp3_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_temp3;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		
		case TEMP2_HIGH_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.Temp2status * 3300/4096);
		calibration_tmp(temp,TS2);
		dlsis_temp2 = temprature_final_value_2;
		if (dlsis_temp2 > Cl_alarmThresholdTable.temp2_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_temp2;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		case TEMP2_LOW_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.Temp2status * 3300/4096);
		calibration_tmp(temp,TS2);
		dlsis_temp2 = temprature_final_value_2;
		if (dlsis_temp2 < Cl_alarmThresholdTable.temp2_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_temp2;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		case TEMP1_HIGH_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = (cl_sys_statbuffer.Temp1status * 3300/4096);
		calibration_tmp(temp,TS1);
		dlsis_temp1 = temprature_final_value_1;
		if (dlsis_temp1 > Cl_alarmThresholdTable.temp1_high_threshold)
		{
			uint16_t var1;
			var1=dlsis_temp1;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
		case TEMP1_LOW_THRESHOLD:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_3",8);
		temp = cl_sys_statbuffer.Temp1status * 0.805;
		calibration_tmp(temp,TS1);
		dlsis_temp1 = temprature_final_value_1;
		if (dlsis_temp1 < Cl_alarmThresholdTable.temp1_low_threshold)
		{
			uint16_t var1;
			var1=dlsis_temp1;
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&var1,2);
			cl_temp_safestate();
		}
		else
		{
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ENTER_4",8);
			cl_temp_recoveredstate();
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
		}
		break;
	}
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
		case EVT_CONSOLE_COMMAND_BYPASS_ON:
		*cl_dlsis_event= EVENT_DLSIS_BYPASS_ON;
		break;
		case EVT_CONSOLE_COMMAND_BYPASS_OFF:
		*cl_dlsis_event= EVENT_DLSIS_BYPASS_OFF;
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
		break;
		case EVT_CONSOLE_COMMAND_PATIENT_READY:
		
			*cl_dlsis_event =   EVENT_DLSIS__START_RETURN;

		break;
		case EVT_CONSOLE_COMMAND_START_RETURN:
			*cl_dlsis_event =   EVENT_DLSIS__STOP_RETURN;
		break;
		case EVT_CONSOLE_COMMAND_STOP_RETURN:
			*cl_dlsis_event =   EVENT_DLSIS__STOP_RETURN;
		break;
		case EVT_CONSOLE_COMMAND_MINUF_ON:
			*cl_dlsis_event =   EVENT_DLSIS__MINUF_ON;
		break;
		case EVT_CONSOLE_COMMAND_MINUF_OFF:
			*cl_dlsis_event =   EVENT_DLSIS__MINUF_OFF;
		break;
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
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				Cl_Dlsis_safesate();	
			break;
			
			
			case HOLDER1STATUS_CLOSED:
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case HOLDER2STATUS_CLOSED:
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;

			case TEMP1_HIGH_THRESHOLD:
			case TEMP1_LOW_THRESHOLD:
				sv_cntrl_setyellowalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case TEMP2_HIGH_THRESHOLD:
			case TEMP2_LOW_THRESHOLD:
				sv_cntrl_setyellowalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case TEMP3_HIGH_THRESHOLD:
			case TEMP3_LOW_THRESHOLD:
				sv_cntrl_setyellowalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case 	PS1_HIGH_THRESHOLD:
			case    PS1_LOW_THRESHOLD:
				sv_cntrl_setyellowalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case 	PS2_HIGH_THRESHOLD:
			case    PS2_LOW_THRESHOLD:
				sv_cntrl_setyellowalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case 	PS3_HIGH_THRESHOLD:
			case    PS3_LOW_THRESHOLD:
				sv_cntrl_setyellowalarm();
				sv_cntrl_buzzer();
				cl_temp_safestate();
			break;
			
			case APTSTATUS_LOW:
			case APTSTATUS_HIGH:
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				Cl_Dlsis_safesate();
			break;
			
			case VPTSTATUS_LOW:
			case VPTSTATUS_HIGH:
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				Cl_Dlsis_safesate();
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
			case COND_STATUS_HIGH:
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				cl_cond_safestate();
			break;
			
			case COND_DAC_OPEN:
				// air trapped in DAC2 or no flow
	//				 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_OPEN",8);
			//		NewAlarmId = _COND_DAC_OPEN;
					Cl_Dlsis_StopDialysis();
					cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
		
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
			case BD_EVENT:
				Cl_Dlsis_StopDialysis();
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BD",2);
				cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;
			
			case ABD_EVENT:
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"ABD",3);
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				Cl_Dlsis_safesate();
			break;
			
			case BLD_EVENT:
				sv_cntrl_setredalarm();
				sv_cntrl_buzzer();
				Cl_Dlsis_safesate();
			break;
			
			
			/*case COND_STATUS_LOW:
			case COND_STATUS_HIGH:
			Cl_Dlsis_StopDialysis();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND",4);
			cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
			break;*/
			
			default:
			break;

		}
		 if(cl_dlsisalarmid != _NO_ALARM)
		 {
			 dlsisalarm = cl_dlsisalarmid;
			 dlsisalarm1 = dlsisalarm;
			data[0] = (uint8_t)cl_dlsisalarmid;
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

					
					 	//DlsisTime.Cl_dlsissecondscounter = 0;
					 	//DlsisTime.Cl_dlsisMinutescounter= 0;
					 	//DlsisTime.Cl_dlsishourscounter= 0;
					 	//DlsisTime.Cl_dlsisTotalMinutescounter= 0;
					 	//DlsisTime.Cl_dlsisTotalhourscounter=0;

				Cl_dlsisretcode =  sv_cntrl_deactivatevenousclamp();
				Cl_dlsisretcode =  sv_cntrl_deactivatepump(UFPUMP);
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
			Cl_dlsisretcode =  sv_cntrl_deactivatepump(UFPUMP);
			

			cl_dlsis_state = CL_DLSIS_STATE_BYPASS;
						
			Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_BYPASS_ON,&data,0);
	
			Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_PAUSED,&data,0);
	
}

Cl_ReturnCodes Cl_Dlsis_BypassOff(void)
{
	uint8_t data;
	Cl_ReturnCodes Cl_dlsisretcode = CL_OK;
	Cl_dlsisretcode = sv_cntrl_disable_bypass();
	
	Cl_dlsisretcode =  sv_cntrl_activatepump(UFPUMP);
	cl_dlsis_state = CL_DLSIS_STATE_DIALYSIS;
	Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_BYPASS_OFF,&data,0);
	Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_RESUMED,&data,0);
	
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
Cl_ReturnCodes Cl_Dlsis_ResetAlertsforReassertion(void )
{
	Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
	ClDlsisAlarmIdType ClDlsisAlarmId;
	uint8_t data;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_NewAlarmIdType cl_dlsisalarmid;
	
	Cl_DlsisAlarmTable[CL_DLSIS_ALARM_HOLDER1STATUS_CLOSE].IsRaised = false;
	Cl_DlsisAlarmTable[CL_DLSIS_ALARM_HOLDER2STATUS_CLOSE].IsRaised = false;
	Cl_AlarmResetAlarm(HOLDER1STATUS_CLOSED);
	Cl_AlarmResetAlarm(HOLDER2STATUS_CLOSED);
	Cl_DlsisAlarmTable[CL_DLSIS_ALARM_BLOODDOOR_STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(BLOODDOOR_STATUS_OPEN);
	//Cl_DlsisAlarmTable[CL_RINSE_ALARM_BYPASSDOOR_STATUS_OPEN].IsRaised = false;
	//Cl_AlarmResetAlarm(BYPASSDOOR_STATUS_OPEN);

	//Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_STATUS_LOW].IsRaised = false;
	//Cl_AlarmResetAlarm(COND_STATUS_LOW);
	//Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_STATUS_HIGH].IsRaised = false;
	//Cl_AlarmResetAlarm(COND_STATUS_HIGH);
	//Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_DAC_OPEN].IsRaised = false;
	//Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_DAC_RO].IsRaised = false;
	//Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_DAC_HIGH].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_PS1_HIGH_THRESHOLD].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_PS1_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS1_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS1_LOW_THRESHOLD);
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_PS2_HIGH_THRESHOLD].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_PS2_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS2_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS2_LOW_THRESHOLD);
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_PS3_HIGH_THRESHOLD].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_PS3_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS3_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS3_LOW_THRESHOLD);
	Cl_DlsisAlarmTable[CL_DLSIS_BDSTATUS].IsRaised = false;
	Cl_AlarmResetAlarm(BD_EVENT);
	Cl_DlsisAlarmTable[CL_DLSIS_ABDSTATUS].IsRaised = false;
	Cl_AlarmResetAlarm(ABD_EVENT);
	Cl_DlsisAlarmTable[CL_DLSIS_BLDSTATUS].IsRaised = false;
	Cl_AlarmResetAlarm(BLD_EVENT);
	Cl_DlsisAlarmTable[CL_DLSIS_APTSTATUS_HIGH].IsRaised = false;
	Cl_DlsisAlarmTable[CL_DLSIS_APTSTATUS_LOW].IsRaised = false;
	Cl_AlarmResetAlarm(APTSTATUS_HIGH);
	Cl_AlarmResetAlarm(APTSTATUS_LOW);
	Cl_DlsisAlarmTable[CL_DLSIS_VPTSTATUS_HIGH].IsRaised = false;
	Cl_DlsisAlarmTable[CL_DLSIS_VPTSTATUS_LOW].IsRaised = false;
	Cl_AlarmResetAlarm(VPTSTATUS_HIGH);
	Cl_AlarmResetAlarm(VPTSTATUS_LOW);
	Cl_DlsisAlarmTable[CL_DLSIS_COND_STATUS_LOW].IsRaised = false;
	Cl_DlsisAlarmTable[CL_DLSIS_COND_STATUS_HIGH].IsRaised = false;
	Cl_AlarmResetAlarm(COND_STATUS_LOW);
	Cl_AlarmResetAlarm(COND_STATUS_HIGH);
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_TEMP3_HIGH_THRESHOLD].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_TEMP3_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP3_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP3_LOW_THRESHOLD);
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_TEMP2_HIGH_THRESHOLD].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_TEMP2_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP2_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP2_LOW_THRESHOLD);
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_TEMP1_HIGH_THRESHOLD].IsRaised = false;
	Cl_DlsisAlarmTable[CL_Dlsis_ALARM_TEMP1_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP1_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP1_LOW_THRESHOLD);
	/*Cl_RinseAlarmTable[CL_RINSE_ALARM_TEMP2_HIGH_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP2_LOW_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP2_HIGH_THRESHOLD);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_TEMP3_HIGH_THRESHOLD].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_TEMP3_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP2_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP2_LOW_THRESHOLD);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_HOLDER1STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(HOLDER1STATUS_OPEN);
	Cl_AlarmResetAlarm(HOLDER1STATUS_CLOSED);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_HOLDER2STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(HOLDER2STATUS_OPEN);
	Cl_AlarmResetAlarm(HOLDER2STATUS_CLOSED);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_FLOW_NO_FLOW].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_FLOW_LOW_FLOWRATE].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_LEVEL_SWITCH_LOW_TOGGLERATE].IsRaised = false;*/
	
	
	
	return (Cl_dlsisretcode);
}

Cl_ReturnCodes Cl_Dlsis_SenddlsisData(void)
{
	static float avgcond = 0;
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
						int16_t sensordata=0;
						sensordata = temp;
							float cond_comp;
					if( sensordata < 0)
					{
						//temp = 0;
						avgcond = 0;
					}
					if( sensordata > 2400)
					{
						float temp,temp1;
						temp = sensordata * 0.805;
						//avgcond = temp1/29.6 + 11;
						//avgcond = avgcond * (1- ((avgtmp3- 25) * 0.02));
						/*temp=sensordata  * 0.805;
						avgcond=temp*4.48;
						avgcond = (avgcond)/100;*/
						calibration_cond(temp);
						avgcond =(avgcond*10 + cond_final_cs3)/11;
						Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
						{
							float temp_comp;
							temp_comp = temp * 0.805;
							calibration_tmp(temp_comp,TS3);
							avgtmp3 =(avgtmp3*5 + temprature_final_value_3)/6;
							cond_comp= avgcond/(1+(avgtmp3-25.0)*0.021);
						}
						
					}
					else
					{
						//avgcond = dummy1;
						avgcond=100;
					}
						tempdata.word = (cond_comp/10);
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
			calibration_tmp(ftemp,TS3);
			avgtmp3 =(avgtmp3*5 + temprature_final_value_3)/6;
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
				sensordatamillivolts = (tempdata.Twobyte * 0.805) ;
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
				sensordatamillivolts = (tempdata.Twobyte * 0.805) ;
				calibration_apt(sensordatamillivolts);
				tempdata.word	 = pressure_final_vpt;
				tempdata.word	 = 30 * 100;
				systemdataarray[count++] = tempdata.bytearray[0] ;
				systemdataarray[count++] = tempdata.bytearray[1] ;
				systemdataarray[count++] = tempdata.bytearray[2] ;
				systemdataarray[count++] = tempdata.bytearray[3] ;
			}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS1STATUS,&tempdata.Twobyte);
			sensordatamillivolts = (tempdata.Twobyte * 0.805) ;
			calibration_apt(sensordatamillivolts);
	
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS2STATUS,&tempdata.Twobyte);
			sensordatamillivolts = (tempdata.Twobyte * 0.805) ;
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
									
									Cl_dlsisretcode =  Cl_AlarmConfigureAlarmType(BLOODDOOR_STATUS_OPEN,LOGIC_HIGH,0,0,0);
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
									sv_cntrl_disable_bypass();
									
									cl_bp_controller(CL_BP_EVENT_START,0);
									//sv_cntrl_activatepump(HEPARINPUMP);
									//cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,1000);
									//cl_wait(100);
									//cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,3000);
									//cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,0);
									uint32_t temp = Treatdata[ID_dflow];
									switch (temp)
									{
										case 800:
										sv_cntrl_setpumpspeed(DCMOTOR2,900);
										sv_cntrl_setpumpspeed(DCMOTOR1,960);
										sv_cs_setpotvalue(2600);
										break;
										case 500:
										Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"500",3);
										sv_cntrl_setpumpspeed(DCMOTOR2,900);
										sv_cntrl_setpumpspeed(DCMOTOR1,650);
										sv_cs_setpotvalue(1700);
										break;
										case 300:
										sv_cntrl_setpumpspeed(DCMOTOR2,900);
										sv_cntrl_setpumpspeed(DCMOTOR1,360);
										sv_cs_setpotvalue(1400);
										break;
										default:
										break;
									}
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

Cl_ReturnCodes	Cl_Dlsis_safesate(void)
{
	
	sv_cntrl_enable_bypass();
	cl_uf_controller(CL_UF_EVENT_STOP,0);
	cl_bp_controller(CL_BP_EVENT_STOP,0);
	sv_cntrl_deactivatevenousclamp();
	cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
	
	return 0;
}
Cl_ReturnCodes	Cl_Dlsis_recoveredsate(void)
{
	
	sv_cntrl_disable_bypass();
	cl_uf_controller(CL_UF_EVENT_START,0);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,10000);
	cl_wait(100);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,5000);
	cl_wait(100);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,3500);
	cl_wait(100);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,2500);
	//sv_cntrl_resetredalarm();
	//sv_cntrl_nobuzzer();
	
	return 0;
}
Cl_ReturnCodes cl_temp_safestate(void)
{
	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BYPASS",6);
	sv_cntrl_enable_bypass();
	cl_uf_controller(CL_UF_EVENT_STOP,0);
	cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
	return 0;
}
Cl_ReturnCodes cl_temp_recoveredstate(void)
{
	sv_cntrl_disable_bypass();
	cl_uf_controller(CL_UF_EVENT_START,0);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,10000);
	cl_wait(100);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,5000);
	cl_wait(100);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,3500);
	cl_wait(100);
	cl_uf_controller(CL_UF_EVENT_SET_UF_RATE,2500);
	return 0;
}

Cl_ReturnCodes cl_cond_safestate(void)
{
	//sv_cntrl_enable_bypass();
	sv_cntrl_deactivate_valve(VALVE_ID13);
	sv_cntrl_deactivatepump(DCMOTOR1);
	sv_cntrl_deactivatepump(DCMOTOR2);
	//Cl_bc_controller(BC_EVENT_STOP);
	sv_cntrl_setflowpath(FLOW_PATH_BC_ALARM);
	SetHeaterState(CL_HEATER_STATE_CLOSED_HEATING);
	cl_dlsis_state = CL_DLSIS_STATE_CRITICAL_ALARM;
	return 0;
}

Cl_ReturnCodes cl_cond_recoveredstate(void)
{
	//sv_cntrl_enable_bypass();
	sv_cntrl_activate_valve(VALVE_ID13);
	sv_cntrl_activatepump(DCMOTOR1);
	sv_cntrl_activatepump(DCMOTOR2);
	Cl_bc_controller(BC_EVENT_RESUME);
	SetHeaterState(CL_HEATER_STATE_CLOSED_HEATING);
	return 0;
}
