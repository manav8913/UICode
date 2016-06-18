#include "cl_app/cl_status/inc/cl_status.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_mac/inc/cl_sys_status.h"
#include "cl_app/inc/cl_types.h"
#include "inc/cl_rinse_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "sv_stubs/inc/sv_stub.h"
#include "sv_stubs/inc/sv_types.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_debug/inc/cl_debug.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.h"
#include "cl_app//cl_bc_cntrl/inc/Cl_bc_controller.h"
#include "cl_app/comp/flowswitchcntrl/inc/flowswitchcntrl.h"
#include "cl_app//cl_dprep/inc/cl_dprep_primecontroller.h"
#include "cl_app/comp/bloodpumpcntrl/inc/cl_bloodpumpcontroller.h"
#include "cl_app/comp/deaeration/inc/cl_deaeration.h"


typedef union {
	int16_t TMP;
	int16_t twobyte;
	uint8_t byte[2] ;
}Datatype;

typedef union {
	int fourbytedata;
	int16_t twobytedata;
	uint8_t bytedata[4] ;
}StateDatatype;
extern uint8_t Cl_ConsoleTxMsg[40];
extern Sys_statusType cl_sys_statbuffer;// test
extern bool g_testbcfreeze; //test
extern bool BC_window; //test
extern bool Current_sense_trigger; // test
extern bool gtest_short_rinse ; //test
extern Cl_Dprep_PrimeStates cl_dprep_prime_state;
extern Cl_AlarmThresholdType  Cl_alarmThresholdTable;

extern uint8_t DD_RESET_VALVE(sv_valvetype );
extern uint8_t DD_SET_VALVE(sv_valvetype );
extern Cl_ReturnCodes        Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes     Cl_SendDatatoconsole_dummy(Cl_ConsoleTxCommandtype,uint8_t*,uint8_t);
extern Cl_ReturnCodes cl_wait(uint32_t );
extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_NewAlarmIdType,bool );
extern Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_NewAlarmIdType  );
extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern Cl_ReturnCodes  sv_nvmgetdata(uint8_t,uint8_t*);
extern Cl_ReturnCodes Cl_Alarm_GetLastAlarm(Cl_NewAlarmIdType*);
extern Cl_ReturnCodes Cl_Rinse_UpdateAlarmTable(ClRinseAlarmIdType*);
extern Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_NewAlarmIdType  , bool* );
extern Cl_ReturnCodes  sv_nvmsetdata(uint8_t ,uint8_t* ,uint8_t datasize);
extern uint8_t sv_cntrl_setflowpath(sv_flowpathtype sv_flowpath);
extern Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_NewAlarmIdType, uint16_t*);
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t sv_cntrl_poweronheater(void);
extern uint8_t sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_incheater(int32_t dty_val);
extern Cl_ReturnCodes Cl_AlarmConfigureAlarmType(Cl_NewAlarmIdType,Cl_AlarmTriggerType,uint16_t,uint16_t,uint8_t);
extern Cl_ReturnCodes UpdateHeaterControls(void);
extern Cl_ReturnCodes SetHeaterState(HeaterStateType Param_HeaterState);
extern Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event);
extern Cl_ReturnCodes cl_flow_switchcontroller(flowswitchcntrl_event_type cntrl_event);
extern Cl_ReturnCodes cl_dprep_primecontroller(Cl_Dprep_PrimeEvents prime_event , int16_t data);
extern Cl_ReturnCodes cl_bp_controller(cl_bp_events bp_event , int16_t data);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern Cl_ReturnCodes cl_deaeration_controller(cl_deaeration_event_type cntrl_event);
extern uint8_t sv_cs_setpotvalue(uint32_t resistance) ;
extern Cl_Dlsis_SenddlsisData(void);
extern uint8_t sv_cntrl_activatevenousclamp(void); 
Cl_ReturnCodes cl_rinse_processtimeoutevets(Cl_Rinse_Eevents cl_rinseevent);
Cl_ReturnCodes  cl_rinse_translatemacevent(MAC_EVENTS ,Cl_Rinse_Eevents* );
Cl_ReturnCodes Cl_rinse_init(void);
Cl_ReturnCodes Cl_Rinse_UpdateTimeInfo(void);
Cl_ReturnCodes Cl_Rinse_CheckforRinseCompletion(void);
Cl_ReturnCodes cl_rinse_notifydacandgotorinsestandby(void);
Cl_ReturnCodes Cl_Rinse_Controller(MAC_EVENTS);
Cl_ReturnCodes  Cl_Rinse_ProcessAlarms(void);
Cl_ReturnCodes Cl_RinseConvertalarmidtouint8data(Cl_NewAlarmIdType cl_alarmId,uint8_t* data );
Cl_ReturnCodes  CL_RinseAlarmActon(Cl_NewAlarmIdType cl_rinsealarmid);
Cl_ReturnCodes  Cl_RinseFlowOn(void );
Cl_ReturnCodes  Cl_RinseFlowOff(void );
Cl_ReturnCodes  Cl_RinseStop(void);
Cl_ReturnCodes Cl_Rinse_ResetAlertsforReassertion(void );
void Cl_Rinse_100msTMP(void);
Cl_ReturnCodes Cl_Rinse_UpdateSecTick(void);
Cl_ReturnCodes Cl_Rinse_StartRinse(void);
Cl_ReturnCodes Cl_Rinse_StopRinse(void);
Cl_ReturnCodes UpdateRinseMinuteTick(void);
Cl_ReturnCodes Cl_Rinse_SendRinseStateData(void);
Cl_ReturnCodes UpdateHeaterControls(void);
Cl_ReturnCodes SetHeaterState(HeaterStateType Param_HeaterState);
Cl_ReturnCodes cl_heater_heat(void);
extern void sv_prop_startpropeo_aligning();
extern void sv_prop_startmixing();
extern void sv_prop_stopmixing();
extern  Cl_ReturnCodes cl_dprep_activate_prime_related_alarms(void);
//static void mdelay(uint32_t );

//extern volatile uint32_t g_ul_ms_ticks ;
Cl_Rinse_States cl_rinsestate = CL_RINSE_STATE_IDLE;
Cl_Rinsing_SubStates cl_rinsing_substate = CL_RINSE_RINSING_IDLE;
//HeaterStateType HeaterState = CL_HEATER_STATE_OFF;
//Cl_Rinse_Eevents cl_rinseevent;
//extern SV_SetflowPath();

 int Cl_Rinsesecondscounter =0 ,Cl_RinseMinutescounter=0, Cl_Rinseshourscounter=0,Cl_RinseOpenFillsecondscounter=0;
 int Cl_RinseTotalMinutescounter=0, Cl_RinsesTotalhourscounter=0;
 bool Cl_RinseOpenFillTimeOut = false;
 bool cl_gfillinprogress = false;
 uint16_t wait_cnt = 0,TmpVal=0;
 	int16_t temp1,temp2;
	 static uint16_t cl_50ms_timer = 0;

RinseAlarmsType Cl_RinseAlarmTable[CL_RINSE_ALRM_MAX] = 
{	
	{BLOODDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false}, 
	{BYPASSDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false}, 	
	{LEVELSWITCH_OFF_TO_ON,CL_ALARM_ALERT,false,false,false}, 
	{LEVELSWITCH_ON_TO_OFF,CL_ALARM_ALERT,false,false,false},
	{HOLDER1STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{HOLDER2STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{TEMP1_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{TEMP2_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{TEMP3_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{TEMP1_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{TEMP2_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{TEMP3_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{PS1_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{PS1_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{PS2_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{PS2_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{PS3_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{PS3_LOW_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	{COND_STATUS_LOW,CL_ALARM_ALARM,false,false,false},
	{COND_STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_OPEN,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_RO,CL_ALARM_ALERT,false,false,false},
	{COND_DAC_HIGH,CL_ALARM_ALERT,false,false,false},
	{FLOW_NO_FLOW,CL_ALARM_ALARM,false,false,false},
	{FLOW_LOW_FLOWRATE,CL_ALARM_ALARM,false,false,false},
	{FLOW_HIGH_FLOWRATE,CL_ALARM_ALARM,false,false,false},
	{LEVEL_SWITCH_LOW_TOGGLERATE,CL_ALARM_ALARM,false,false,false},
			
};

Cl_ReturnCodes Cl_Rinse_Controller(MAC_EVENTS Cl_MacRinseEvent   )
{
	
	Cl_ReturnCodes  Cl_rinseretcode = CL_OK;
	Cl_Rinse_Eevents cl_rinseevent = EVENT_NULL_EVENT;
	Cl_NewAlarmIdType cl_rinsealarmid;
	ClRinseAlarmIdType ClRinseAlarmId;

	
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t dataarray[4] = {0,0,0,0};
	uint8_t data = 0;
	bool alarmstatus1 = false,alarmstatus2 = false,alarmstatus3 = false, flowstatus = false;




cl_rinse_translatemacevent( Cl_MacRinseEvent, &cl_rinseevent);

if(cl_rinseevent == EVENT_RINSE_TICK_SECOND)
{

	//Cl_rinseretcode =  Cl_AlarmResetAlarm( SENSOR_TEMP3STATUS );
	Cl_rinseretcode =  Cl_AlarmResetAlarm( LEVELSWITCH_OFF_TO_ON );
	Cl_rinseretcode =  Cl_AlarmResetAlarm( LEVELSWITCH_ON_TO_OFF );


}


switch(cl_rinsestate)
{

		case CL_RINSE_STATE_INIT:
		break;
		case CL_RINSE_STATE_IDLE:
		switch (cl_rinseevent)
			{
				case EVT_RINSE_GET_DATA:
						if(Cl_ConsoleRxMsg.msgready == true)
						{
							
							switch(Cl_ConsoleRxMsg.data.byte[0])
							{
								case CON_RX_PARAM_DATA_RINSE_STATUS:
								command = CON_TX_COMMAND_SYSDATA;
								dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
								Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
								Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
								
								break;
								case CON_RX_PARAM_DATA_PRIME_STATUS:
								command = CON_TX_COMMAND_SYSDATA;
								dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
								Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
								Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);	
								break;
								case CON_RX_PARAM_DATA_DISINF_STATUS:
								command = CON_TX_COMMAND_SYSDATA;
								dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
								Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DISINF_STATUS, &dataarray[1]);
								Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);	
								break;							
								default:
								break;
							}
						}
				break;
				case EVENT_START_RINSE:
						Cl_Rinse_StartRinse();
					command = CON_TX_COMMAND_RINSE_CONFIRM;
					data = (uint8_t)COMMAND_RESULT_SUCCESS;
					Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,1);					
					command = CON_TX_COMMAND_RINSE_STARTED;
					Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
					Cl_rinseretcode = Cl_Rinse_UpdateTimeInfo();
						cl_rinsestate = CL_RINSE_STATE_RINSING;
						cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
				break;
				case EVENT_START_PRE_DISINF_RINSE:
					Cl_Rinse_StartRinse();
					
					cl_rinsestate = CL_RINSE_STATE_PRE_DISINFECT_RINSING;
					cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
				break;
				case EVENT_START_POST_DISINF_RINSE:
						Cl_Rinse_StartRinse();
						cl_rinsestate = CL_RINSE_STATE_POST_DISINFECT_RINSING;
						cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
				break;
				case EVENT_RINSE_ALARM:
				// check for rinse alarms and update rinse alarm table
					Cl_rinseretcode = Cl_Rinse_ProcessAlarms();
				break;
				default:
				break;
			}                                 	
		break;
		
		case CL_RINSE_STATE_RINSING:
		case CL_RINSE_STATE_PRE_DISINFECT_RINSING:
		case CL_RINSE_STATE_POST_DISINFECT_RINSING:
				switch (cl_rinseevent)
				{
						case EVT_RINSE_SET_DATA:
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
						case EVT_RINSE_GET_DATA:
								if(Cl_ConsoleRxMsg.msgready == true)
								{
							
									switch(Cl_ConsoleRxMsg.data.byte[0])
									{

										case CON_RX_PARAM_DATA_PRIME_STATUS:
										command = CON_TX_COMMAND_SYSDATA;
										dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
										Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
										Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
										break;

										case CON_RX_PARAM_DATA_RECIRC_STATUS:
										command = CON_TX_COMMAND_SYSDATA;
										dataarray[0] = CON_TX_PARAM_DATA_RCIRC;
										//Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
										dataarray[1] = 0;
										Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
										break;		
										case CON_RX_PARAM_DATA_DISINF_STATUS:
										command = CON_TX_COMMAND_SYSDATA;
										dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
										Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DISINF_STATUS, &dataarray[1]);
										Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);	
										break;			
															
										default:
										break;
									}
								}
						break;
						case EVENT_RINSE_TICK_50MS:
						if(cl_rinsing_substate == CL_RINSE_RINSING_BC_SWITCHING)
						{
									cl_50ms_timer++;
									if(cl_50ms_timer == 2)
									{
										cl_50ms_timer =0;
									//	sv_cntrl_deactivate_valve(VALVE_ID19);
									}
						}
																	
						break;
						case EVENT_RINSE_TICK_500MS:
						//UpdateHeaterControls();
						break;
						case EVENT_RINSE_TICK_SECOND:
						
						if(cl_rinsestate == CL_RINSE_STATE_RINSING)
						{
							Cl_Rinse_SendRinseStateData();	
						}
						
						

					//	UpdateHeaterControls();
						Cl_Rinsesecondscounter++;
						if(Cl_Rinsesecondscounter == 60)
						{

											
							Cl_Rinsesecondscounter = 0;
							Cl_RinseMinutescounter++;
							Cl_RinseTotalMinutescounter++;
							if(cl_rinsestate == CL_RINSE_STATE_RINSING)
							{
								Cl_rinseretcode = Cl_Rinse_UpdateTimeInfo();
							}
	
						}
						//	Cl_Rinse_ResetAlertsforReassertion();
							//Cl_rinseretcode = Cl_Rinse_ProcessAlarms();
					
						break;
						case EVENT_RINSE_TICK_MINUTE:
						//	Cl_Rinsesecondscounter = 0;
						//	Cl_RinseMinutescounter++;
						//	Cl_RinseTotalMinutescounter++;
							Cl_Rinse_ResetAlertsforReassertion();
							if(Cl_Rinse_CheckforRinseCompletion() == CL_OK )
							{

								Cl_rinseretcode = sv_cntrl_poweroffheater();
								Cl_rinseretcode = cl_rinse_notifydacandgotorinsestandby();
								if(cl_rinsestate == CL_RINSE_STATE_PRE_DISINFECT_RINSING)
								{
									Cl_rinseretcode = Cl_mac_apprequesthandler(MACREQ_PRE_DISINFECT_RINSE_COMPLETED);
								}
								else if(cl_rinsestate == CL_RINSE_STATE_POST_DISINFECT_RINSING)
								{
									Cl_rinseretcode = Cl_mac_apprequesthandler(MACREQ_POST_DISINFECT_RINSE_COMPLETED);
								}
								
								
								cl_rinsestate = CL_RINSE_STATE_POST_RINSE_STANDBY;
								//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
	
							}
						break;
						case EVENT_RINSE_TICK_HOUR:
							Cl_Rinseshourscounter++;
							Cl_RinsesTotalhourscounter++;
						break;
						case EVENT_STOP_RINSE:
							Cl_Rinse_StopRinse();
						break;
						break;
						case EVENT_RINSE_ALARM:
							Cl_rinseretcode = Cl_Rinse_ProcessAlarms();
						break;
						case EVENT_RINSE_ALERT:
							Cl_Alarm_GetAlarmStatus(FPCURRENTSTATUS,&alarmstatus1);
							if(alarmstatus1)
							{
							//	Cl_rinseretcode = Cl_bc_controller(BC_EVENT_CS);
							//	sv_cntrl_activate_valve(VALVE_ID19);
								cl_50ms_timer  = 1;
							}
							Cl_Alarm_GetAlarmStatus(SENSOR_TEMP2STATUS,&alarmstatus3);
							if(alarmstatus3)
							{
								//	UpdateHeaterControls();
							}
					
							Cl_Alarm_GetAlarmStatus(FLOW_NO_FLOW,&flowstatus);
							//	if(flowstatus)
							//	{
							//		Cl_rinseretcode = sv_cntrl_poweroffheater();
							//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLOW_OFF",8);
							//	}
						break;
						case EVT_RINSE_START_PRIME:
						
					//	if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
						{
					//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PRIME1",6);
							cl_dprep_activate_prime_related_alarms();
							cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_START,0);
						}
						
						break;
						case EVT_RINSE_STOP_PRIME:
						cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_STOP,0);
						break;
						case EVT_RINSE_RCIRC_START:
						if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
						{
							Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RCIRC",6);
							cl_dprep_primecontroller(CL_DPREP_PRIME_RCIRC_START,0);
						}
						
						break;
						case EVT_RINSE_RCIRC_STOP:

							Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RCIRC_STOP",6);
							cl_dprep_primecontroller(CL_DPREP_PRIME_RCIRC_STOP,0);

						
						break;
						case EVT_RINSE_COMMAND_SET_BLDPMP_ON:
						cl_bp_controller(CL_BP_EVENT_START,0);
						break;
						case EVT_RINSE_COMMAND_SET_BLDPMP_OFF:
						cl_bp_controller(CL_BP_EVENT_STOP,0);
						break;
						case EVENT_MAC_RINSE_COMPLETED:
						if(cl_rinsestate != CL_RINSE_STATE_POST_RINSE_STANDBY)
						{
							cl_rinsestate = CL_RINSE_STATE_POST_RINSE_STANDBY;
						}
						break;
						case EVENT_RINSE_SET_BLDPMP_ON:
						cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_START,0);
						break;
						case EVENT_RINSE_SET_BLDPMP_OFF:
						cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_STOP,0);
						break;
						case EVENT_RINSE_SET_BLDPUMPRATE:
						if(Cl_ConsoleRxMsg.msgready == true)
						{
							
						cl_Datastreamtype cl_temp  ;
						cl_temp.bytearray[0] = Cl_ConsoleRxMsg.data.byte[0];
						cl_temp.bytearray[1] = Cl_ConsoleRxMsg.data.byte[0];
						cl_temp.bytearray[2] = Cl_ConsoleRxMsg.data.byte[0];
						cl_temp.bytearray[3] = Cl_ConsoleRxMsg.data.byte[0];
						cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_SETRATE,cl_temp.Twobyte);
						}
						break;
						
						case EVENT_RINSE_BC_OPENFILL_COMPLETED:
						cl_rinsing_substate = CL_RINSE_RINSING_BC_SWITCHING;
						break;
				}
		break;
		case CL_RINSE_STATE_CRITICAL_ALARM:
				switch (cl_rinseevent)
				{
					case EVENT_START_RINSE:
					Cl_Rinse_StartRinse();
					command = CON_TX_COMMAND_RINSE_CONFIRM;
					data = (uint8_t)COMMAND_RESULT_SUCCESS;
					Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,1);					
					command = CON_TX_COMMAND_RINSE_STARTED;
					Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
						cl_rinsestate = CL_RINSE_STATE_RINSING;
						cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
					break;
					case EVENT_START_PRE_DISINF_RINSE:
						Cl_Rinse_StartRinse();
					
						cl_rinsestate = CL_RINSE_STATE_PRE_DISINFECT_RINSING;
						cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
					break;
					case EVENT_START_POST_DISINF_RINSE:
							Cl_Rinse_StartRinse();
							cl_rinsestate = CL_RINSE_STATE_POST_DISINFECT_RINSING;
							cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
					break;
					case EVENT_STOP_RINSE:
					Cl_Rinse_StopRinse();
					break;
					case EVT_RINSE_GET_DATA:
					if(Cl_ConsoleRxMsg.msgready == true)
					{
					
						switch(Cl_ConsoleRxMsg.data.byte[0])
						{
							case CON_RX_PARAM_DATA_RINSE_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
							Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						
							break;
							case CON_RX_PARAM_DATA_DISINF_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
							Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DISINF_STATUS, &dataarray[1]);
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);	
							break;
							default:
							break;
						}
					}
					break;
					case EVENT_RINSE_ALARM:
					Cl_rinseretcode = Cl_Rinse_ProcessAlarms();
					break;
					
					case EVENT_RINSE_TICK_MINUTE:
					Cl_rinseretcode = Cl_Rinse_ResetAlertsforReassertion();
					break;
					case EVENT_RINSE_SET_BLDPUMPRATE:
						if(Cl_ConsoleRxMsg.msgready == true)
						{
							
							cl_Datastreamtype cl_temp  ;
							cl_temp.bytearray[0] = Cl_ConsoleRxMsg.data.byte[0];
							cl_temp.bytearray[1] = Cl_ConsoleRxMsg.data.byte[0];
							cl_temp.bytearray[2] = Cl_ConsoleRxMsg.data.byte[0];
							cl_temp.bytearray[3] = Cl_ConsoleRxMsg.data.byte[0];
							cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_SETRATE,cl_temp.Twobyte);
						}
					break;
					default:break;
				}
		break;
		case CL_RINSE_STATE_POST_RINSE_STANDBY:
				switch (cl_rinseevent)
				{
					case EVENT_START_RINSE:
					Cl_Rinse_StartRinse();
					command = CON_TX_COMMAND_RINSE_CONFIRM;
					data = (uint8_t)COMMAND_RESULT_SUCCESS;
					Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,1);					
					command = CON_TX_COMMAND_RINSE_STARTED;
					Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
						cl_rinsestate = CL_RINSE_STATE_RINSING;
						cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
					break;
					case EVENT_START_PRE_DISINF_RINSE:
						Cl_Rinse_StartRinse();
					
						cl_rinsestate = CL_RINSE_STATE_PRE_DISINFECT_RINSING;
						cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
					break;
					case EVENT_START_POST_DISINF_RINSE:
							Cl_Rinse_StartRinse();
							cl_rinsestate = CL_RINSE_STATE_POST_DISINFECT_RINSING;
							cl_rinsing_substate = CL_RINSE_RINSING_OPENFILL;
					break;
					case EVT_RINSE_GET_DATA:
					if(Cl_ConsoleRxMsg.msgready == true)
					{
						switch(Cl_ConsoleRxMsg.data.byte[0])
						{
							case CON_RX_PARAM_DATA_RINSE_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
							Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
							break;
							case CON_RX_PARAM_DATA_PRIME_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
							Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
							break;
							case CON_RX_PARAM_DATA_MIXING_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_MIXING;
							//Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
							dataarray[1] = 0;
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
							break;
							case CON_RX_PARAM_DATA_RECIRC_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_RCIRC;
							//Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
							dataarray[1] = 0;
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);
							break;
							case CON_RX_PARAM_DATA_DISINF_STATUS:
							command = CON_TX_COMMAND_SYSDATA;
							dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
							Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DISINF_STATUS, &dataarray[1]);
							Cl_rinseretcode = Cl_SendDatatoconsole(command,&dataarray,2);	
							break;
							
							default:
							break;
						}
					}
					break;
					case EVT_RINSE_START_PRIME:
						
					if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
					{
						Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PRIME1",6);
						cl_dprep_activate_prime_related_alarms();
						cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_START,0);
					}
						
					break;
					case EVT_RINSE_STOP_PRIME:
					cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_STOP,0);
					break;
					case EVT_RINSE_RCIRC_START:
					if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
					{
						Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RCIRC",6);
						cl_dprep_primecontroller(CL_DPREP_PRIME_RCIRC_START,0);
					}
						
					break;
					case EVT_RINSE_RCIRC_STOP:

						Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RCIRC_STOP",6);
						cl_dprep_primecontroller(CL_DPREP_PRIME_RCIRC_STOP,0);

						
					break;
					case EVENT_MAC_RINSE_COMPLETED:
					return(CL_OK);
					break;
					case EVENT_RINSE_SET_BLDPUMPRATE:
					if(Cl_ConsoleRxMsg.msgready == true)
					{
						
						cl_Datastreamtype cl_temp  ;
						cl_temp.bytearray[0] = Cl_ConsoleRxMsg.data.byte[0];
						cl_temp.bytearray[1] = Cl_ConsoleRxMsg.data.byte[0];
						cl_temp.bytearray[2] = Cl_ConsoleRxMsg.data.byte[0];
						cl_temp.bytearray[3] = Cl_ConsoleRxMsg.data.byte[0];
						cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_SETRATE,cl_temp.Twobyte);
					}
					break;
					default: 
					
					return(CL_REJECTED);
					break;
				}
					
		break;

	
	default:
			switch(cl_rinseevent)
			{
				case EVENT_START_RINSE:
				break;
				case EVENT_STOP_RINSE:
				Cl_Rinse_StopRinse();
				break;
				case EVENT_RINSE_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_RINSE_ALARM:
				// update console 
				
					Cl_rinseretcode = Cl_Rinse_ProcessAlarms();
				
				break;
				case EVENT_MAC_RINSE_COMPLETED:
				if(cl_rinsestate != CL_RINSE_STATE_POST_RINSE_STANDBY)
				{
					cl_rinsestate = CL_RINSE_STATE_POST_RINSE_STANDBY;
				}
				break;
				case EVENT_RINSE_DIALYSIS_PREP:
				if (cl_rinsestate != CL_RINSE_STATE_POST_RINSE_STANDBY )
				{
					Cl_rinseretcode = CL_ERROR;
				}
				break;
				default:
				break;
			}
	break;
}


return Cl_rinseretcode;
}

Cl_ReturnCodes  cl_rinse_translatemacevent(MAC_EVENTS Cl_MacRinseEvt,Cl_Rinse_Eevents* cl_rinse_event)
{
	switch(Cl_MacRinseEvt)
	{

			
		case  EVT_CONSOLE_COMMAND_RINSE_START:
		
		*cl_rinse_event= EVENT_START_RINSE;
		break;
		case  MACREQ_START_PRE_DISINF_RINSE:
		*cl_rinse_event= EVENT_START_PRE_DISINF_RINSE;
		break;
		case  MACREQ_START_POST_DISINF_RINSE:
		*cl_rinse_event= EVENT_START_POST_DISINF_RINSE;
		break;
		case EVT_CONSOLE_COMMAND_STOP_RINSE:
		*cl_rinse_event= EVENT_STOP_RINSE;
		break;
	
		case MACREQ_RINSE_COMPLETED:
		*cl_rinse_event = EVENT_MAC_RINSE_COMPLETED;
		break;
		case MACREQ_BC_OPENFILL_COMPLETED:
		*cl_rinse_event = EVENT_RINSE_BC_OPENFILL_COMPLETED;
		break;
		case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
		*cl_rinse_event = EVENT_RINSE_DIALYSIS_PREP;
		break; 
		
		case EVT_TICK_50M:
		*cl_rinse_event =   EVENT_RINSE_TICK_50MS;
		break;
 		break;
		case EVT_TICK_100M:
		*cl_rinse_event =   EVENT_RINSE_TICK_100MS;
		break;
		case EVT_TICK_500M:
		  		*cl_rinse_event =   EVENT_RINSE_TICK_500MS;
		break;
		case EVT_TICK_SEC:
		*cl_rinse_event =   EVENT_RINSE_TICK_SECOND;
		break;
		case EVT_TICK_MIN:
		*cl_rinse_event =   EVENT_RINSE_TICK_MINUTE;
		break;			 
		case EVT_TICK_HOUR:
		*cl_rinse_event =   EVENT_RINSE_TICK_HOUR;
		break;
		case EVT_ALARM_TRIGGERED:
		*cl_rinse_event =  EVENT_RINSE_ALARM;
		break;
		case  EVT_ALERT_TRIGGERED:
		*cl_rinse_event =  EVENT_RINSE_ALERT;
		break;
		
		case EVT_CONSOLE_COMMAND_SET_DATA:
		*cl_rinse_event = EVT_RINSE_SET_DATA;
		break;
		
		
		
		case EVT_CONSOLE_COMMAND_GET_DATA:
		*cl_rinse_event =  EVT_RINSE_GET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_START_PRIME:
		*cl_rinse_event =  EVT_RINSE_START_PRIME;
		break;
		case EVT_CONSOLE_COMMAND_STOP_PRIME:
		*cl_rinse_event =  EVT_RINSE_STOP_PRIME;
		break;
		case EVT_CONSOLE_COMMAND_RCIRC_START:
		*cl_rinse_event = EVT_RINSE_RCIRC_START;
		break;
		case EVT_CONSOLE_COMMAND_RCIRC_STOP:
		*cl_rinse_event = EVT_RINSE_RCIRC_STOP;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
		*cl_rinse_event = EVT_RINSE_COMMAND_SET_BLDPMP_ON;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
		*cl_rinse_event = EVT_RINSE_COMMAND_SET_BLDPMP_OFF;
		break;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
		*cl_rinse_event = EVT_RINSE_COMMAND_SET_BLDPUMPRATE;
		break;
		
		default:
		break;
	}
		 return CL_OK;
}
 Cl_ReturnCodes Cl_rinse_init(void)
 {
	 
	 return CL_OK;
 }



 Cl_ReturnCodes cl_rinse_processtimeoutevets(Cl_Rinse_Eevents cl_rinseevent)
 {
	 
	 switch(cl_rinseevent)
	 {
		 case EVENT_RINSE_TICK_SECOND:
		 // update console with time info /system info..?
		 break;
		 case EVENT_RINSE_TICK_MINUTE:
		 // check for state transition time outs
		 break;
		 case EVENT_RINSE_TICK_HOUR:
		 break;
		 default:
		 break;

	 }
	 
	 return CL_OK;
 }
 
 
 Cl_ReturnCodes Cl_Rinse_UpdateTimeInfo(void)
 {
	 
	 Cl_ReturnCodes Cl_rinseretcode = CL_ERROR;
	 Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	 uint8_t data[7] = {0,0,0,0,0 ,0,0};
	 
	command = CON_TX_COMMAND_REM_TIME;

	
		data[0]= (uint8_t)CLEAN_DATA;
		data[1]= (uint8_t)Cl_RinseTotalMinutescounter;

		
		Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,2);
		
		
//	Cl_Rinse_SendRinseStateData();                                                          commented on 28.05.2014
	return CL_OK;
	
 }
 
  Cl_ReturnCodes Cl_Rinse_UpdateSecTick(void)
  {
	  
	  Cl_ReturnCodes Cl_rinseretcode = CL_ERROR;
	  Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	  uint8_t data[7] = {0,0,0,0,0 ,0,0};
	  
	  command = CON_TX_COMMAND_TICK_SEC;
	  

	  
	  Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
	  //Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,1);
	  Cl_Rinse_SendRinseStateData();
	  return CL_OK;
	  
  }
 
 
 Cl_ReturnCodes Cl_Rinse_CheckforRinseCompletion(void)
 {
	 Cl_ReturnCodes Cl_rinseretcode = CL_ERROR;
	 if(gtest_short_rinse)
	 {
		if(Cl_RinseTotalMinutescounter > 5 ) 
		Cl_rinseretcode = CL_OK;
		return Cl_rinseretcode;
	 }
	
	 else if(Cl_RinseTotalMinutescounter >= CL_RINSE_TIMEOUT_MIN )
	{
			
		Cl_rinseretcode = CL_OK;
	}

	return Cl_rinseretcode;
	 
 }
 
Cl_ReturnCodes cl_rinse_notifydacandgotorinsestandby(void)
{
	Cl_ReturnCodes 	Cl_rinseretcode2 = CL_OK;
	
	uint8_t data =0;
	Cl_RinseStop();
	Cl_RinseFlowOff();
	Cl_rinseretcode2 = sv_cntrl_poweroffheater();
	//inform DAC about rinse completed state.
	Cl_Rinsesecondscounter = 0;
	Cl_RinseMinutescounter= 0;
	Cl_Rinseshourscounter= 0;
	Cl_RinseTotalMinutescounter= 0;
	Cl_RinsesTotalhourscounter=0;
	
	data = 1;
	if((cl_rinsestate == CL_RINSE_STATE_RINSING)  )
	{
		Cl_rinseretcode2 = sv_nvmsetdata(NV_NVM_RINSE_STATUS,&data,1);
		Cl_rinseretcode2 = Cl_SendDatatoconsole(CON_TX_COMMAND_RINSE_COMPLETED,&data,0);
		Cl_rinseretcode2 = Cl_mac_apprequesthandler(MACREQ_RINSE_COMPLETED);
		
	}
	

						
	return Cl_rinseretcode2;
}

Cl_ReturnCodes Cl_Rinse_UpdateAlarmTable(ClRinseAlarmIdType* ClRinseAlarmId )
{
	Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
	uint8_t tempcount = 0;
	uint8_t data[3] ={0, 0} ,*data1 = NULL;
	uint8_t data3;
	Cl_NewAlarmIdType cl_alarmId;
	
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	
	for (tempcount = 0 ; tempcount < CL_RINSE_ALRM_MAX ; tempcount++)
	{
		
		Cl_Alarm_GetAlarmStatus(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmId,&Cl_RinseAlarmTable[tempcount].IsActive);

		
			if (Cl_RinseAlarmTable[tempcount].IsActive)
			{
				if(!Cl_RinseAlarmTable[tempcount].IsRaised)
					{
	
						Cl_RinseAlarmTable[tempcount].IsRaised = true;
					
						Cl_rinseretcode = CL_RinseAlarmActon(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmId);
					
			
					}
			
			}
			else
			{
				// alarm was present before , but not active now.
				if(Cl_RinseAlarmTable[tempcount].IsRaised == true)
				{
					Cl_RinseAlarmTable[tempcount].IsRaised = false;
					command = CON_TX_COMMAND_ALARM;
					data[0] = (uint8_t)(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmId);
					data[1] = (uint8_t) CL_ALARM_CLEARED;
			//		Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,2);
				}

			}
		//}

	}

return (Cl_rinseretcode );
}

Cl_ReturnCodes Cl_Rinse_ProcessAlarms(void )
{
		Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
		ClRinseAlarmIdType ClRinseAlarmId;
		uint8_t data;
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		Cl_NewAlarmIdType cl_rinsealarmid;
	
	   Cl_rinseretcode = Cl_Rinse_UpdateAlarmTable(&ClRinseAlarmId);

		   
		  
	   	return (Cl_rinseretcode);
}

Cl_ReturnCodes Cl_Rinse_ResetAlertsforReassertion(void )
{
	Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
	ClRinseAlarmIdType ClRinseAlarmId;
	uint8_t data;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_NewAlarmIdType cl_rinsealarmid;
	
		
	Cl_RinseAlarmTable[CL_RINSE_ALARM_BLOODDOOR_STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(BLOODDOOR_STATUS_OPEN);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_BYPASSDOOR_STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(BYPASSDOOR_STATUS_OPEN);

	Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_STATUS_LOW].IsRaised = false;
	Cl_AlarmResetAlarm(COND_STATUS_LOW);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_STATUS_HIGH].IsRaised = false;
	Cl_AlarmResetAlarm(COND_STATUS_HIGH);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_DAC_OPEN].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_DAC_RO].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_COND_DAC_HIGH].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_PS1_HIGH_THRESHOLD].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_PS1_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS1_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS1_LOW_THRESHOLD);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_PS2_HIGH_THRESHOLD].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_PS2_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS2_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS2_LOW_THRESHOLD);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_PS3_HIGH_THRESHOLD].IsRaised = false;
	Cl_RinseAlarmTable[CL_RINSE_ALARM_PS2_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS3_LOW_THRESHOLD);
	Cl_AlarmResetAlarm(PS3_HIGH_THRESHOLD);
	Cl_RinseAlarmTable[CL_RINSE_ALARM_TEMP2_HIGH_THRESHOLD].IsRaised = false;
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
	Cl_RinseAlarmTable[CL_RINSE_ALARM_LEVEL_SWITCH_LOW_TOGGLERATE].IsRaised = false;
	
	
	
	return (Cl_rinseretcode);
}


Cl_ReturnCodes  CL_RinseAlarmActon(Cl_NewAlarmIdType cl_rinsealarmid)
{
		Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
			uint8_t data[3] ={0, 0} ,*data1 = NULL;
			uint8_t data3;
			Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
			uint16_t levelswitchstatus = 0;	
			static uint8_t fillseccounter=0;
			Cl_NewAlarmIdType NewAlarmId = _NO_ALARM;
		

			

							
		switch(cl_rinsealarmid)
		{
			
			
			case BLOODDOOR_STATUS_OPEN:
				Cl_Rinse_StopRinse();
		//		NewAlarmId = _BLOODDOOR_STATUS_OPEN;
				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
				//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BPD",3);
				Cl_SysStat_GetSensor_Status_Query(BLOODDOOR_STATUS_OPEN,&levelswitchstatus);
			
			break;
			case HOLDER1STATUS_OPEN:
			// stop rinsing
			Cl_Rinse_StopRinse();
		//	NewAlarmId = _HOLDER1STATUS_OPEN;
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HLD1",4);
			Cl_SysStat_GetSensor_Status_Query(HOLDER1STATUS_OPEN,&levelswitchstatus);
			//enterl_saferinse_state();
			break;
			case HOLDER2STATUS_OPEN:
			// stop rinsing
			Cl_Rinse_StopRinse();
		//	NewAlarmId = _HOLDER2STATUS_OPEN;
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HLD2",4);
			//enterl_saferinse_state();
			break;

			case TEMP1_HIGH_THRESHOLD:
		
			Cl_SysStat_GetSensor_Status_Query(TEMP1_HIGH_THRESHOLD,&TmpVal);
		//
			temp1 = (0.805 * TmpVal) - 1004 ;

			{
		//		NewAlarmId = _TEMP1_HIGH_THRESHOLD;
				Cl_Rinse_StopRinse();
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			}


			break;
			
			case PS1_HIGH_THRESHOLD:
			//	NewAlarmId = _TEMP1_LOW_THRESHOLD;
				Cl_Rinse_StopRinse();
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			
			break;
			case TEMP2_HIGH_THRESHOLD:
		
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&TmpVal);
	
			temp1 = (0.805 * TmpVal) - 1004 ;
			temp2 = 3000 + (temp1 * 1000)/382;
			if(temp2 > 3680)
			{
			//	NewAlarmId = _TEMP2_HIGH_THRESHOLD;
				Cl_Rinse_StopRinse();
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			}


			break;
			case TEMP2_LOW_THRESHOLD:
			
			//	NewAlarmId = _TEMP2_LOW_THRESHOLD;
				Cl_Rinse_StopRinse();
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;

			case TEMP3_HIGH_THRESHOLD:
				
			{
			//		NewAlarmId = _TEMP3_HIGH_THRESHOLD;
					Cl_Rinse_StopRinse();
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
					cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			}


			break;
			case TEMP3_LOW_THRESHOLD:
			//		NewAlarmId = _TEMP3_LOW_THRESHOLD;
					Cl_Rinse_StopRinse();
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
					cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;

			case	PS2_HIGH_THRESHOLD:
		//	NewAlarmId = _PS2_HIGH_THRESHOLD;
			Cl_Rinse_StopRinse();
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			
			break;
			case	PS3_HIGH_THRESHOLD:
		//	NewAlarmId = _NO_ALARM;
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS3",3);
			Cl_Rinse_StopRinse();
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			
			break;
			case FLOW_NO_FLOW:
			
			Cl_Rinse_StopRinse();
		//	NewAlarmId = _FLOW_NO_FLOW;
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
			case FLOW_LOW_FLOWRATE:
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLW",3);
		//	NewAlarmId = _FLOW_LOW_FLOWRATE;
			Cl_Rinse_StopRinse();
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
			case FLOW_HIGH_FLOWRATE:
		//	NewAlarmId = _FLOW_HIGH_FLOWRATE;
			Cl_Rinse_StopRinse();
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
			case COND_STATUS_LOW:
	//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_LOW",8);
		//		NewAlarmId = _COND_STATUS_LOW;
				Cl_Rinse_StopRinse();
				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
			case COND_STATUS_HIGH:
			//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_HIGH",9);
				//			NewAlarmId = _COND_STATUS_HIGH;
							Cl_Rinse_StopRinse();
							cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
			case COND_DAC_OPEN:
				// air trapped in DAC2 or no flow
	//				 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_OPEN",8);
				//	NewAlarmId = _COND_DAC_OPEN;
					Cl_Rinse_StopRinse();
					cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
		//	case COND_DAC_RO:
				// RO water filled DAC2
			//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_RO",6);
			//				NewAlarmId = _COND_DAC_RO;
				//			Cl_Rinse_StopRinse();
					//		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			//break;
			case COND_DAC_HIGH:
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_HIGH",8);
				//					NewAlarmId = _COND_DAC_HIGH;
				//					Cl_Rinse_StopRinse();
					//				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
				// Rinse incomplete.
			break;
			case 	UFP_OVERRUN:
			case	UFP_UNDERRUN:
				//	NewAlarmId = _UF_ALARM_FOR_RINSE;
					Cl_Rinse_StopRinse();
					cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;
			case 	BC_OVERRUN:
			case	BC_UNDERRUN:
			case	BC_FAILED:
				//	NewAlarmId = _BC_ALARM;
					Cl_Rinse_StopRinse();
					cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
			break;

			case DE_CHAMBER_LOW: //23
			break;
			case SYSTEM_NOT_READY: //24
			break;
			case POWER_FAILURE: //25
			break;
	//		case LEVEL_SWITCH_LOW_TOGGLERATE:
		//	NewAlarmId = _LEVEL_SWITCH_STOPPED;
		//	break;
			case LEVEL_SWITCH_LOW_TOGGLERATE:
		//	NewAlarmId = _LEVEL_SWITCH_LOW_TOGGLERATE;
			break;
			case LEVEL_SWITCH_HIGH_TOGGLERATE:
		//	NewAlarmId = _LEVEL_SWITCH_HIGH_TOGGLERATE;
			break;

			case WATCHDOG_TIMER: //27
		//		NewAlarmId = _WATCHDOG_TIMER;
			break;
			default:
			break;

		}
		
		 if(cl_rinsealarmid != _NO_ALARM)
		 {
			data[0] = (uint8_t)cl_rinsealarmid;
			data[1] = (uint8_t) CL_ALARM_TRIGGERED;
			data[2] = (uint8_t)CRITICAL;
			command = CON_TX_COMMAND_ALARM;
			
			Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,3);
			cl_wait(10);
					
			command = CON_TX_COMMAND_SYS_STATE;
			data[0] = (uint8_t)CLEANING;
			data[1] = (uint8_t)CLEAN_CLEAN_ALARM_STOPPED;
			Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,2);
		 }
			
}

Cl_ReturnCodes  Cl_RinseFlowOn(void )
{
	Cl_ReturnCodes Cl_RetVal = CL_OK;
	sv_cntrl_activate_valve(VALVE_ID1);
	cl_gfillinprogress = true;
	return Cl_RetVal;
	
}

Cl_ReturnCodes  Cl_RinseFlowOff(void )
{
	Cl_ReturnCodes Cl_RetVal = CL_OK;
	sv_cntrl_deactivate_valve(VALVE_ID1);
	cl_gfillinprogress = false;
	return Cl_RetVal;
	
}

Cl_ReturnCodes  Cl_RinseStop(void)
{
	Cl_ReturnCodes Cl_RetVal = CL_OK;
	//sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
	Cl_bc_controller(BC_EVENT_STOP);
	
	sv_cntrl_poweroffheater();
	return Cl_RetVal;
	
}
Cl_ReturnCodes Cl_Rinse_SendRinseStateData(void)
{
	Cl_ReturnCodes  Cl_rinseretcode = CL_OK;
	StateDatatype data;
	uint8_t count=0;
	int16_t ts2,temp,temp1;
	uint8_t dataarray[12] =  {0,0,0,0,0,0,0,0,0,0,0,0};
	static timecount = 0;
	static float avgtmp3 = 0;
	
	//Cl_Console_bulkdatatype 
	
	timecount++;

	if (timecount < 5)
	{
		return;
	}
	else
	{
		timecount = 0;
	}
	
	dataarray[0] = CLEAN_DATA;
	count++;
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
	//data.twobytedata = ts2;
	
	//temp = (0.805 * data.twobytedata) - 526 ;
	temp = (0.8056 * data.twobytedata) - 1450 ;
	//temp1 = 3000 + (temp * 100)/19;

	temp1 = 3700 + (temp * 1000)/340;
	

	//data.twobytedata = (uint16_t)temp1;
	//dataarray[count++] = data.bytedata[0];
	//dataarray[count++] = data.bytedata[1];
	
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
			{
				
				float ftemp,ftemp1;
				data.fourbytedata = 0;
				ftemp = temp * 0.805;
				ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.6;
				avgtmp3 =	(avgtmp3*5 + ftemp1)/6;
				data.twobytedata = (uint16_t)(avgtmp3 * 10);
				dataarray[count++] = data.bytedata[0];
				dataarray[count++] = data.bytedata[1];
				dataarray[count++] = data.bytedata[2];
				dataarray[count++] = data.bytedata[3];
			}
	
		
		Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&data.twobytedata);
		//data.twobytedata = ts2;
		
		//temp = (0.805 * data.twobytedata) - 526 ;
		temp = (0.8056 * data.twobytedata) - 1450 ;
		//temp1 = 3000 + (temp * 100)/19;

		temp1 = 3700 + (temp * 1000)/340;
		

	//	data.twobytedata = (uint16_t)temp1;
	//	dataarray[count++] = data.bytedata[0];
	//	dataarray[count++] = data.bytedata[1];


	//	Cl_rinseretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_STATE_DATA,&dataarray,count);
		
					Cl_SysStat_GetSensor_Status_Query(COND_STATUS_HIGH,&temp);
					{
						
		
				
				if( temp < 0)
				{
					temp = 0;
					//	avgcond = 0;
				}
				
				

	
				if( temp > 1000)
				{
					temp = temp/20 + 11;
				}
				else
				{
					temp = temp/5;
				}
				
	
	
								data.fourbytedata = 0;
								data.twobytedata = temp;
								dataarray[count++] = data.bytedata[0];
								dataarray[count++] = data.bytedata[1];
								dataarray[count++] = data.bytedata[2];
								dataarray[count++] = data.bytedata[3];
					}
				
					

		
		Cl_rinseretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_STATE_DATA,&dataarray,count);
		

}

void Cl_Rinse_100msTMP(void)
{
	
	int16_t Ps1,Ps2,Ps3,Atp,Vtp;
	int16_t sensordata=0;
	int16_t Pressuredata=0;
	int16_t sensordatamillivolts =0;
	int16_t tempdata =0;
	int16_t Pressuredatainmillibars =0,ps1Pressuredatainmillibars=0,ps2Pressuredatainmillibars=0,ps3Pressuredatainmillibars=0,aptPressuredatainmillibars=0,vptPressuredatainmillibars=0;
	uint8_t* pdataarray;
	uint8_t dataarray[12] =  {0,0,0,0,0,0,0,0,0,0,0,0};
	Datatype data;
	//Datatype temp;
	static timecount = 0;
	
	static int16_t counter,ps1avg,ps2avg,ps3avg,aptavg,vptavg;
	Cl_ReturnCodes Cl_rinseretcode = CL_ERROR;
	
	
	timecount++;
//	#if 0
	if (timecount < 20)
	{
		return;
	}
	else
	{
		timecount = 0;
	}
	//#endif
	
	//cl_testgetsensordata(PS1STATUS_HIGH,&Ps1);
	Cl_SysStat_GetSensor_Status_Query(SENSOR_PS1STATUS,&Ps1);
	sensordatamillivolts = ((Ps1* 3300 /4096) ) ;
	Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
	if (  sensordatamillivolts < 1360 )
	{
		Pressuredatainmillibars = Pressuredatainmillibars- 50 ;
	}
	else if(  sensordatamillivolts < 1650 )
	{
		Pressuredatainmillibars = Pressuredatainmillibars- 25 ;
	}
	
	ps1Pressuredatainmillibars = Pressuredatainmillibars; // - 1000;
	ps1avg = ((ps1avg* 9) + ps1Pressuredatainmillibars) /10;
	
	//cl_testgetsensordata(PS2STATUS_HIGH,&Ps2);
	Cl_SysStat_GetSensor_Status_Query(SENSOR_PS2STATUS, &Ps2);
	sensordatamillivolts = ((Ps2* 3300 /4096) ) ;
	Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
	if (  sensordatamillivolts < 1360 )
	{
		Pressuredatainmillibars = Pressuredatainmillibars- 50 ;
	}
	else if(  sensordatamillivolts < 1650 )
	{
		Pressuredatainmillibars = Pressuredatainmillibars- 25 ;
	}
	ps2Pressuredatainmillibars = Pressuredatainmillibars;
	ps2avg = ((ps2avg* 9) + ps2Pressuredatainmillibars) /10;
	
	Cl_SysStat_GetSensor_Status_Query(SENSOR_PS3STATUS, &Ps3);
	sensordatamillivolts = ((Ps3* 3300 /4096) ) ;
	//Pressuredatainmillibars =  (sensordatamillivolts/1.485 ) ;
	Pressuredatainmillibars = (sensordatamillivolts - 1.1)/1.1;
	ps3Pressuredatainmillibars = Pressuredatainmillibars;// - 1000;
	
	ps3avg = ((ps3avg* 9) + ps3Pressuredatainmillibars) /10;
	//ps3avg = Ps3;
	
	//cl_testgetsensordata(APTSTATUS_HIGH,&Atp);
	Cl_SysStat_GetSensor_Status_Query(SENSOR_APTSTATUS, &Atp);
	sensordatamillivolts = ((Atp* 3300 /4096) ) ;
	Pressuredatainmillibars = (sensordatamillivolts - 1640)/0.6;
	aptPressuredatainmillibars = Pressuredatainmillibars;  // - 1000;
	//Atp = Pressuredatainmillibars;
	aptavg = (( aptavg *9) + aptPressuredatainmillibars) /10;
	//cl_testgetsensordata(VPTSTATUS_HIGH,&Vtp);
	Cl_SysStat_GetSensor_Status_Query(SENSOR_VPTSTATUS, &Vtp);
	sensordatamillivolts = ((Vtp* 3300 /4096) ) ;
	Pressuredatainmillibars = (sensordatamillivolts - 1640)/0.6;
	vptPressuredatainmillibars = Pressuredatainmillibars;  // - 1000;
	//Vtp = Pressuredatainmillibars;
	vptavg = (( vptavg *9) + vptPressuredatainmillibars) /10;
 
	
		data.TMP = (vptavg + aptavg)/2 - (ps1avg+ps2avg)/2;
		
		dataarray[0] = CON_TX_PARAM_DATA_PRESSURE_DATA;
		dataarray[1] = data.byte[0];
		dataarray[2] = data.byte[1];
		data.twobyte = ps1Pressuredatainmillibars;
		dataarray[3] = data.byte[0];
		dataarray[4] = data.byte[1];
		data.twobyte = ps2Pressuredatainmillibars;
		dataarray[5] = data.byte[0];
		dataarray[6] = data.byte[1];
		data.twobyte = aptPressuredatainmillibars;
		dataarray[7] = data.byte[0];
		dataarray[8] = data.byte[1];
		data.twobyte = vptPressuredatainmillibars;
		dataarray[9] = data.byte[0];
		dataarray[10] = data.byte[1];
		data.twobyte = ps1avg;
		dataarray[11] = data.byte[0];
		dataarray[12] = data.byte[1];
		data.twobyte = ps2avg;
	//	data.TMP = ps3avg;
		dataarray[13] = data.byte[0];
		dataarray[14] = data.byte[1];
		data.twobyte = aptavg;
		dataarray[15] = data.byte[0];
		dataarray[16] = data.byte[1];
		data.twobyte = vptavg;
		dataarray[17] = data.byte[0];
		dataarray[18] = data.byte[1];
	Cl_rinseretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_SYSDATA,&dataarray,19);
	

	
}
Cl_ReturnCodes Cl_Rinse_StartRinse(void)
{
				Cl_ReturnCodes Cl_rinseretcode = CL_OK;
				Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
				uint8_t dataarray[4] = {0,0,0,0};
				uint8_t data = 0;
					
				//Check for  rinse readiness and alarm status
				
				Cl_RinseFlowOn();
				//	Cl_rinseretcode = sv_cntrl_setflowpath(BC_FLUSH_OPEN); // sanjeer BC_NEW
				Cl_rinseretcode =  sv_cntrl_activatepump(DCMOTOR1);
				Cl_rinseretcode =  sv_cntrl_activatepump(DCMOTOR2);
				Cl_rinseretcode = sv_cntrl_setpumpspeed(DCMOTOR2,800);
				Cl_rinseretcode = sv_cntrl_setpumpspeed(DCMOTOR1,1050);
				sv_cs_setpotvalue(3800);
				sv_prop_startpropeo_aligning();
				Cl_rinseretcode =  cl_deaeration_controller(CL_DEAER_EVENT_ACTIVATE);
				
				Cl_rinseretcode = Cl_bc_controller(BC_EVENT_START);
				//sv_prop_startmixing();
				// sv_prop_startpropeo_aligning();
			
				if(Cl_rinseretcode == CL_OK)
				{
					

					//Reset the  OPENFILL time count
					Cl_RinseMinutescounter = 0;
					Cl_RinseTotalMinutescounter = 0;
					Cl_RinseOpenFillTimeOut = false;

					

					
					Cl_rinseretcode =  cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_START);
					Cl_Rinse_ResetAlertsforReassertion();
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(BLOODDOOR_STATUS_OPEN,LOGIC_HIGH,0,0,0);					
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(HOLDER1STATUS_OPEN,LOGIC_HIGH,0,0,0);
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(HOLDER2STATUS_OPEN,LOGIC_HIGH,0,0,0);
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(LEVELSWITCH_OFF_TO_ON,LOGIC_LOW,0,0,0);
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(LEVELSWITCH_ON_TO_OFF,LOGIC_HIGH,0,0,0);
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(FLOW_LOW_FLOWRATE,0,0,0,0);
					Cl_rinseretcode =  Cl_AlarmConfigureAlarmType(FLOW_NO_FLOW,0,0,0,0);
					
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(BLOODDOOR_STATUS_OPEN,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(HOLDER1STATUS_OPEN,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(HOLDER2STATUS_OPEN,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );					
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(FLOW_LOW_FLOWRATE,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(FLOW_NO_FLOW,true );					
				//	Cl_rinseretcode =  Cl_AlarmActivateAlarms(FLOW_LOW_FLOWRATE,true );
				//	Cl_rinseretcode =  Cl_AlarmActivateAlarms(FLOW_HIGH_FLOWRATE,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_STATUS_LOW,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(PS2_HIGH_THRESHOLD,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(PS1_HIGH_THRESHOLD,true );
					

					

					
					
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(TEMP3_HIGH_THRESHOLD,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(TEMP3_LOW_THRESHOLD,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(TEMP2_HIGH_THRESHOLD,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(TEMP2_LOW_THRESHOLD,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(TEMP1_HIGH_THRESHOLD,true );
					Cl_rinseretcode =  Cl_AlarmActivateAlarms(TEMP1_LOW_THRESHOLD,true );
					
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(PS1STATUS_HIGH,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(PS2STATUS_HIGH,true );
					
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(PS3STATUS_HIGH,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_STATUS_LOW,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_DAC_OPEN,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_DAC_RO,true );
					//Cl_rinseretcode =  Cl_AlarmActivateAlarms(COND_DAC_HIGH,true );
					

					
					if(Current_sense_trigger)
					{
						Cl_rinseretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
					}
					else
					{
						//			Cl_rinseretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );
					}
											
					Cl_rinseretcode = Cl_Rinse_ResetAlertsforReassertion();
					//Cl_rinseretcode = SetHeaterState(CL_HEATER_STATE_START);
					

					
				}
}
Cl_ReturnCodes Cl_Rinse_StopRinse(void)
{
				Cl_ReturnCodes Cl_rinseretcode = CL_OK;
				uint8_t data;
				if(!((cl_rinsestate == CL_RINSE_STATE_IDLE ) || (cl_rinsestate == CL_RINSE_STATE_STOPPED ) ||(cl_rinsestate == CL_RINSE_STATE_CRITICAL_ALARM )  ))
				{
					
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DOOR",4);
				sv_cntrl_activatevenousclamp();
				Cl_Rinsesecondscounter = 0;
				Cl_RinseMinutescounter = 0;
				Cl_RinseTotalMinutescounter = 0;
				Cl_RinsesTotalhourscounter = 0 ;
				Cl_rinseretcode = sv_cntrl_poweroffheater();
				Cl_bc_controller(BC_EVENT_STOP);
				sv_prop_stopmixing();
				//Cl_rinseretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
				cl_rinsestate = CL_RINSE_STATE_IDLE;
				Cl_rinseretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_RINSE_STOPPED,&data,0);
				
					
				}
			
				return Cl_rinseretcode;
}


Cl_ReturnCodes UpdateRinseMinuteTick(void)
{
					Cl_ReturnCodes Cl_rinseretcode = CL_OK;
					Cl_Rinsesecondscounter = 0;
					Cl_RinseMinutescounter++;
					Cl_RinseTotalMinutescounter++;
					Cl_rinseretcode = Cl_Rinse_UpdateTimeInfo();
					return Cl_rinseretcode;
}
