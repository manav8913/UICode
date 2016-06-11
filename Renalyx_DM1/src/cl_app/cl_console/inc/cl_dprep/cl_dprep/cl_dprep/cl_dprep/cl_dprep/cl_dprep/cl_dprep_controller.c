/*
 * cl_dprep_controller.c
 *
 * Created: 12/27/2013 12:24:08 PM
 *  Author: user
 */ 
#include "cl_app/inc/cl_types.h"
#include "inc/cl_dprep_controller.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "sv_stubs/inc/sv_types.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.c.h"
#include "cl_app/cl_bc_cntrl/inc/Cl_bc_controller.h"
#include "cl_app//cl_dprep/inc/cl_dprep_primecontroller.h"
#include "asf.h"


Cl_ReturnCodes Cl_dprep_init(void);
Cl_ReturnCodes Cl_dprep_controller(MAC_EVENTS);
Cl_ReturnCodes  cl_dprep_translatemacevent(MAC_EVENTS ,Cl_Dprep_Events* );
Cl_ReturnCodes Cl_Dprep_UpdateTimeInfo(void);
Cl_ReturnCodes cl_dprep_notifydacandgotodpreptandby(void);
Cl_ReturnCodes cl_dprep_notifydacandgotodprep_postprimetandby(void);
Cl_ReturnCodes Cl_Dprep_ProcessAlarms(void );
Cl_ReturnCodes Cl_Dprep_UpdateAlarmTable( void);
Cl_ReturnCodes  CL_DrepAlarmActon(Cl_AlarmIdType cl_dprepalarmid);
Cl_ReturnCodes Cl_Dprep_CheckforfillingCompletion(void);
Cl_ReturnCodes Cl_DprepFillingFlowOff(void);
Cl_ReturnCodes Cl_DprepFillingFlowOn(void);
Cl_ReturnCodes Cl_dprep_filling_FSM(Cl_Dprep_Events cl_dprepevent);
Cl_ReturnCodes  Cl_DprepSelectDialysateInlet( void);
Cl_ReturnCodes  Cl_DprepFlowOn(void );
Cl_ReturnCodes  Cl_DprepFlowOff(void );
Cl_ReturnCodes UpdateDprepMinuteTick(void);
Cl_ReturnCodes Cl_Dprep_UpdateFillingTimeInfo(void);
Cl_ReturnCodes UpdateDprepFillingMinuteTick(void);
Cl_ReturnCodes Cl_Dprep_UpdatePrimeTimeInfo(void);
Cl_ReturnCodes UpdateDprepPrimeMinuteTick(void);
Cl_ReturnCodes UpdateDprepDialyserPrimeMinuteTick(void);
Cl_ReturnCodes Cl_Dprep_UpdateDialyserPrimeTimeInfo(void);
Cl_ReturnCodes cl_prep_checkforprimecompletion(void);
Cl_ReturnCodes Cl_dprep_filling_FSM_Init(Cl_Dprep_Events cl_dprepevent);
Cl_ReturnCodes Cl_Dprep_SendPrepStateData(Cl_Console_bulkdatatype);
Cl_ReturnCodes Cl_Dprep_Stoppreparation(void);
Cl_ReturnCodes Cl_Dprep_ResetAlertsforReassertion(void );
Cl_ReturnCodes Cl_Dprep_SendtreatementData(void);
Cl_ReturnCodes Cl_Dprep_setdata(Cl_ConsoleRxDataType DataId,cl_PrepDatatype cl_PrepData , uint8_t size);
Cl_ReturnCodes Cl_Dprep_Get_treatmentdata(Cl_ConsoleRxDataType DataId,cl_PrepDatatype cl_PrepData , uint8_t size);
Cl_ReturnCodes Cl_Dprep_Get_data(Cl_ConsoleRxDataType DataId,   uint8_t size);

Cl_ReturnCodes Cl_drepUpdateHeaterControls(void);
Cl_ReturnCodes	Cl_Dprep_StartPreparation(void);
Cl_ReturnCodes	Cl_Dprep_StartDialyserPrime(void);
Cl_ReturnCodes Cl_dprep_StopMixing(void);

Cl_ReturnCodes cl_dprep_activate_prime_related_alarms(void);


extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS );
extern Cl_ReturnCodes  sv_nvmgetdata(uint8_t,uint8_t*);
extern Cl_ReturnCodes  sv_nvmsetdata(uint8_t ,uint8_t* ,uint8_t datasize);
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_AlarmIdType  , bool* );
extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType, uint16_t*);
extern uint8_t sv_cntrl_setflowpath(sv_flowpathtype sv_flowpath);
extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_AlarmIdType,bool );
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
extern Cl_ReturnCodes Cl_AlarmConfigureAlarmType(Cl_AlarmIdType,Cl_AlarmTriggerType,uint16_t,uint16_t,uint8_t);
extern Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_AlarmIdType  );
extern uint8_t sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_incheater(uint32_t dty_val);
extern uint8_t sv_cntrl_poweronheater(void);
extern Cl_ReturnCodes UpdateHeaterControls(void);
extern Cl_ReturnCodes SetHeaterState(RinseHeaterStateType Param_HeaterState);
extern Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event);
extern Cl_ReturnCodes cl_dprep_primecontroller(Cl_Dprep_PrimeEvents prime_event , int16_t data);
extern uint8_t sv_cntrl_activatevenousclamp(void);
extern uint8_t sv_cntrl_deactivatevenousclamp(void);
extern void sv_prop_startmixing();

extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern Cl_Dprep_PrimeStates cl_dprep_prime_state;
extern bool BC_window; //test
extern bool g_testbcfreeze; //test
extern bool Current_sense_trigger; // test
extern float dummy3 ;

	int 	Cl_Dprepsecondscounter = 0;
	int 	Cl_DprepMinutescounter= 0;
	int 	Cl_Dprephourscounter= 0;
	int 	Cl_DprepTotalMinutescounter= 0;
	int 	Cl_DprepTotalhourscounter=0;
	int16_t		Cl_Dprep_filling_secondscounter = 0;
	int16_t		Cl_Dprep_filling_Minutescounter = 0;
	int16_t		Cl_Dprep_filling_TotalMinutescounter = 0;
	
	int16_t		Cl_Dprep_Prime_secondscounter = 0;
	int16_t		Cl_Dprep_Prime_Minutescounter = 0;
	int16_t		Cl_Dprep_Prime_TotalMinutescounter = 0;
	
	int16_t		Cl_Dprep_DialyserPrime_secondscounter = 0;
	int16_t		Cl_Dprep_DialyserPrime_Minutescounter = 0;
	int16_t		Cl_Dprep_DialyserPrime_TotalMinutescounter = 0;
	
	
Bool cl_temp3_stable_confirm = false;
Bool cl_temp3_stable = false;
Bool Cl_Prime_state ;
static uint16_t cl_50ms_timer = 0;
Cl_PatientStateType Cl_PatientState = CL_DPREP_PATIENT_STATE_NOT_CONNECTED;
Cl_Dprep_States cl_dprepstate = CL_DPREP_STATE_IDLE;
Cl_Dprep_filling_States cl_dprepfillingState = CL_DPREP_FILLING_IDLE;
DprepAlarmsType Cl_DprepAlarmTable[CL_DPREP_ALRM_MAX] =
{
	
	{BLOODDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{LEVELSWITCH_OFF_TO_ON,CL_ALARM_ALERT,false,false,false},
	{LEVELSWITCH_ON_TO_OFF,CL_ALARM_ALERT,false,false,false},
	{HOLDER1STATUS_CLOSED,CL_ALARM_ALARM,false,false,false},
	{HOLDER2STATUS_CLOSED,CL_ALARM_ALARM,false,false,false},
	{COND_STATUS_LOW,CL_ALARM_ALARM,false,false,false},
	{COND_STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_OPEN,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_RO,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_HIGH,CL_ALARM_ALARM,false,false,false},
	{FLOWSTATUS_FLOWOFF,CL_ALARM_ALARM,false,false,false},
	{FLOW_LOW_FLOWRATE,CL_ALARM_ALARM,false,false,false},
	{HP_START,CL_ALARM_ALARM,false,false,false},
	{ABDSTATUS_ON,CL_ALARM_ALARM,false,false,false},
	{BDSTATUS_ON,CL_ALARM_ALARM,false,false,false},
	{APTSTATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{VPTSTATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{BLDSTATUS_ON,CL_ALARM_ALARM,false,false,false},
	{PS1STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{PS2STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{PS3STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{PS4STATUS,CL_ALARM_ALARM,false,false,false},
	{TEMP1STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{TEMP2STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{TEMP3STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{FPCURRENTSTATUS,CL_ALARM_ALARM,false,false,false},
	{UF_ROTATION_MARKER,CL_ALARM_ALARM,false,false,false},
};

 int Cl_dprepsecondscounter =0 ,Cl_dprepMinutescounter=0, Cl_dprephourscounter=0;
 int Cl_dprepTotalMinutescounter=0, Cl_dprepTotalhourscounter=0;

 Cl_ReturnCodes Cl_dprep_init(void)
 {
	 
	 return CL_OK;
 }
 

 
Cl_ReturnCodes Cl_dprep_controller(MAC_EVENTS Cl_MacDprepEvent)
{
	
	Cl_ReturnCodes  Cl_dprepretcode = CL_OK;
	Cl_Dprep_Events cl_dprepevent = EVENT_DPREP_EVENT_NULL;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data = 0;
	uint8_t datasize = 0;
	uint8_t dataarray[4] =  {0,0,0,0};
	uint8_t systemdataarray[40] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cl_PrepDatatype tempdata;
	Bool cl_status;
	cl_PrepDatatype cl_PrepData;															
	bool alarmstatus1 = false,alarmstatus2 = false,alarmstatus3 = false,flowstatus = false;
	uint16_t static cl_temp3;
	uint16_t temp_temp3;
	float temp3_cel;
	
	cl_dprep_translatemacevent( Cl_MacDprepEvent, &cl_dprepevent);
	
	switch(cl_dprepevent)
	{
		case EVENT_DPREP_TICK_SECOND:
		Cl_dprepretcode = Cl_bc_controller(BC_EVENT_SECOND);
		//Cl_dprepretcode = cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_TICK_SEC,0);
		break;
		case EVENT_DPREP_TICK_50MS:
		Cl_dprepretcode = Cl_bc_controller(BC_EVENT_50MS);
		break;
		default:break;
		
	}
	if(cl_dprepevent == EVENT_DPREP_TICK_SECOND)
	{

		Cl_dprepretcode =  Cl_AlarmResetAlarm( TEMP3STATUS_HIGH );
		Cl_dprepretcode =  Cl_AlarmResetAlarm( TEMP2STATUS_HIGH );
		Cl_dprepretcode =  Cl_AlarmResetAlarm( FLOWSTATUS_FLOWOFF );
		//	Cl_rinseretcode =  Cl_AlarmResetAlarm( FLOWSTATUS_FLOWOFF );
	}


	switch(cl_dprepstate)
	{

		case CL_DPREP_STATE_INIT:
		break;
		case CL_DPREP_STATE_IDLE:
		case CL_DPREP_STATE_STOPPED:
		switch (cl_dprepevent)
		{
			case EVENT_DPREP_COMMAND_GET_DATA:

				if(Cl_ConsoleRxMsg.msgready == true)
				{
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case	CON_RX_PARAM_DATA_PRIME_STATUS:
						command = CON_TX_COMMAND_SYSDATA;
						dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
						Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						break;
						default:
						break;
					}
				}
			break;
			case EVENT_DPREP_COMMAND_SET_DATA:
					 if(Cl_ConsoleRxMsg.msgready == true)
					 {
						 
						 
						 Cl_ConsoleRxDataType dataId;
						 uint8_t count;
						 dataId = Cl_ConsoleRxMsg.data.byte[0];
						 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
						 {
							 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
							 
						 }
						 Cl_Dprep_setdata(dataId, cl_PrepData,count);
						 
						 
						 
					 }
	
			break;
			case EVENT_DPREP_DIALYSIS_PREP:
			
			Cl_Dprep_StartPreparation();
			break;
			case EVENT_DPREP_MIXING_PREP_START:
			Cl_Dprep_StartPreparation();
			break;
			case EVENT_DPREP_ALARM:
			Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
			break;
			case EVENT_DPREP_START_PRIME:
			if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
			{
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PRIME1",6);
				cl_dprep_activate_prime_related_alarms();
				cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_START,0);
			}
			
			break;
			default:
			break;
		}
		break;
		case CL_DPREP_STATE_DPREP_FILLING:
		switch (cl_dprepevent)
		{
			case EVENT_DPREP_START_RECIRC:
						if((cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING) || (cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIME_RCIRC_STARTED) || (cl_dprep_prime_state !=  CL_DPREP_PRIME_STATE_DIALYSER_PRIMING))
						{
							Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RCIRC",6);
							cl_dprep_primecontroller(CL_DPREP_PRIME_RCIRC_START,0);
						}
			break;
			case EVENT_DPREP_COMMAND_GET_DATA:

				if(Cl_ConsoleRxMsg.msgready == true)
				{
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case	CON_RX_PARAM_DATA_PRIME_STATUS:
						command = CON_TX_COMMAND_SYSDATA;
						dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
						Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						break;
						default:
						break;
					}
				}
			break;
			case EVENT_DPREP_COMMAND_SET_DATA:
					 if(Cl_ConsoleRxMsg.msgready == true)
					 {
						 
						 
						 Cl_ConsoleRxDataType dataId;
						 uint8_t count;
						 dataId = Cl_ConsoleRxMsg.data.byte[0];
						 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
						 {
							 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
							 
						 }
						 Cl_Dprep_setdata(dataId, cl_PrepData,count);
						 
						 
						 
					 }
	
			break;
			case EVENT_DPREP_TICK_50MS:
			
			cl_50ms_timer++;
			if(cl_50ms_timer == 2)
			{
				cl_50ms_timer =0;
				sv_cntrl_deactivate_valve(VALVE_ID19);
			}
			
			break;
			case EVENT_DPREP_TICK_500MS:
				UpdateHeaterControls();
			break;
			case EVENT_DPREP_TICK_MINUTE:
				Cl_Dprep_ResetAlertsforReassertion();
				Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&temp_temp3);
					temp_temp3 = temp_temp3 * 0.805;
					temp3_cel = 0.0000116 * temp_temp3 *temp_temp3 + 0.0035 *temp_temp3 + 11.157;
				if((temp3_cel) > 36.8 && (temp3_cel < 37.2))
				{
					if( cl_temp3_stable == true) 
					{
					 cl_temp3_stable_confirm = true;	
					}
					else
					{
					 cl_temp3_stable = true;
					 cl_temp3_stable_confirm = false;
					}
				}
			break;
			case EVENT_DPREP_TICK_SECOND:
					
					Cl_Dprep_filling_secondscounter++;
					if(Cl_Dprep_filling_secondscounter == 60)
					{
						UpdateDprepFillingMinuteTick();
					}
					Cl_Dprep_SendPrepStateData(DIALYSIS_PREP_DATA);
				//	Cl_Dprep_ResetAlertsforReassertion();
					Cl_dprepretcode = Cl_Dprep_ProcessAlarms();

					if(Cl_Dprep_CheckforfillingCompletion() == CL_OK )
					{
						//if(cl_dprep_prime_state == CL_DPREP_PRIME_STATE_PRIME_COMPLETED)
						if(cl_dprep_prime_state == CL_DPREP_PRIME_STATE_PRIME_RCIRC_COMPLETED)
						{
							cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
									 
						}
						else
						{
							cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE;
							
						}
						
						//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
						Cl_dprepretcode = sv_cntrl_poweroffheater();							
						Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
						Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
						command = CON_TX_COMMAND_DIALYSATE_FILLING_COMPLETED;
						data = (uint8_t)COMMAND_RESULT_SUCCESS;
						Cl_Dprep_filling_secondscounter = 0;
						Cl_Dprep_filling_Minutescounter = 0;
						Cl_Dprep_filling_TotalMinutescounter = 0;
										
						Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,1);
					}
			break;
			case EVENT_DPREP_STOP_DIALYSATE_FILL:
					Cl_dprep_StopMixing();
			break;
			break;
			case EVENT_DPREP_ALARM:
					Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
			break;
			case EVENT_DPREP_ALERT:
					Cl_Alarm_GetAlarmStatus(FPCURRENTSTATUS,&alarmstatus1);
					if(alarmstatus1)
					{
						Cl_dprepretcode = Cl_bc_controller(BC_EVENT_CS);
						sv_cntrl_activate_valve(VALVE_ID19);
						cl_50ms_timer  = 1;
					
					}
					Cl_Alarm_GetAlarmStatus(TEMP2STATUS_HIGH,&alarmstatus3);
					if(alarmstatus3)
					{
						//	UpdateHeaterControls();
					}
			
					Cl_Alarm_GetAlarmStatus(FLOWSTATUS_FLOWOFF,&flowstatus);
					//	if(flowstatus)
					//	{
					//		Cl_rinseretcode = sv_cntrl_poweroffheater();
					//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLOW_OFF",8);
					//	}
			break;
			case EVENT_DPREP_START_PRIME:
			if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
			{
				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PRIME1",6);
				cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_START,0);
			}
			
			break;

		}
		break;

		case CL_DPREP_STATE_CRITICAL_ALARM:
					switch (cl_dprepevent)
					{
						case EVENT_DPREP_DIALYSIS_PREP:
						Cl_Dprep_StartPreparation();
						break;
						case EVENT_DPREP_MIXING_PREP_START:
						Cl_Dprep_StartPreparation();
						break;
									default:break;
					}
		break;
		case CL_DPREP_STATE_DPREP_FILLING_DONE:
					switch (cl_dprepevent)
					{
						case EVENT_DPREP_START_RECIRC:
									if((cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING) || (cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIME_RCIRC_STARTED) || (cl_dprep_prime_state !=  CL_DPREP_PRIME_STATE_DIALYSER_PRIMING))
									{
										Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"RCIRC",6);
										cl_dprep_primecontroller(CL_DPREP_PRIME_RCIRC_START,0);
									}
						break;
						case EVENT_DPREP_START_PRIME:
						if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_PRIMING)
						{
							Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PRIME1",6);
							cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_START,0);
						}
						
						break;
						case EVENT_DPREP_MAC_PRIME_COMPLETED:
					//	cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
						break;

						case EVENT_DPREP_TICK_MINUTE:
							Cl_Dprep_ResetAlertsforReassertion();
						break;
						
						case EVENT_DPREP_TICK_SECOND:
					//	if(cl_dprep_prime_state == CL_DPREP_PRIME_STATE_PRIME_COMPLETED)
						if(cl_dprep_prime_state == CL_DPREP_PRIME_STATE_PRIME_RCIRC_COMPLETED)
					
						{
							cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
									 
						}
						break;
						case EVENT_DPREP_ALARM:
						Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
						break;
						case EVENT_DPREP_DILYSER_CONNECTED:
		
						// check for micro switches
						Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSER_CONNECTED_CONFIRMED,&data,0);
		
						break;
						default:break;
					}

		break;
		case CL_DPREP_STATE_POST_PRIME_STANDBY:
		switch(cl_dprepevent)
					{
						case EVENT_DPREP_COMMAND_GET_DATA:
							if(Cl_ConsoleRxMsg.msgready == true)
							{
								switch(Cl_ConsoleRxMsg.data.byte[0])
								{
									case CON_RX_PARAM_DATA_DIALYSER_PRIME_STATUS:
									command = CON_TX_COMMAND_SYSDATA;
									dataarray[0] = CON_TX_PARAM_DATA_DIALYSER_PRIME_STATUS;
									dataarray[1] = 0;									
									Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
									break;
								}
							}
						break;
						case EVENT_DPREP_COMMAND_SET_DATA:
								 if(Cl_ConsoleRxMsg.msgready == true)
								 {
						 
						 
									 Cl_ConsoleRxDataType dataId;
									 uint8_t count;
									 dataId = Cl_ConsoleRxMsg.data.byte[0];
									 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
									 {
										 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
							 
									 }
									 Cl_Dprep_setdata(dataId, cl_PrepData,count);
						 
						 
						 
								 }
	
						break;
						case EVENT_DPREP_DILYSER_CONNECTED:
		
						// check for micro switches
						Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSER_CONNECTED_CONFIRMED,&data,0);
		
						break;
						case EVENT_DPREP_START_DIALISER_PRIME:
								Cl_Dprep_StartDialyserPrime();
						break;
						case EVENT_DPREP_ALARM:
						Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
						break;
						default:
						break;
					}
		break;

		case CL_DPREP_STATE_DIALISER_PRIME:
					switch(cl_dprepevent)
					{
						
						case EVENT_DPREP_TICK_50MS:
									
						break;

						case EVENT_DPREP_TICK_MINUTE:
							Cl_Dprep_ResetAlertsforReassertion();
						break;
						case EVENT_DPREP_TICK_SECOND:
							UpdateHeaterControls();

							Cl_Dprep_DialyserPrime_secondscounter++;
							if(Cl_Dprep_DialyserPrime_secondscounter == 60)
							{
								UpdateDprepDialyserPrimeMinuteTick();
							
							}
							Cl_Dprep_SendPrepStateData(DIALYSER_PRIME_DATA);
						//	Cl_Dprep_ResetAlertsforReassertion();
							Cl_dprepretcode = Cl_Dprep_ProcessAlarms();

					// check for sub state time out and transition
					

							if( Cl_Dprep_DialyserPrime_Minutescounter >= CL_DPREP_DIALISER_PRIME_TIMEOUT_MIN)
							{
								Cl_Dprep_DialyserPrime_Minutescounter = 0;
							//	sv_cntrl_deactivatepump(BLOODPUMP);
							//	cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_STOP,0);
								cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_STOP_DIALYSER_PRIMING, 1); //1 == STOPPED BECAUSE COMPLTED

								//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
								Cl_dprepretcode = sv_cntrl_poweroffheater();
								Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
								Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
								Cl_dprepretcode =  	cl_dprep_notifydacandgotodpreptandby();	
								cl_dprepstate = CL_DPREP_STATE_POST_DPREP_STANDBY;	
							}
						break;
						case EVENT_DPREP_STOP_DIALYSER_PRIME:
						//	Cl_Rinse_StopRinse();
						break;
						break;
						case EVENT_DPREP_ALARM:
						Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
						break;
						case EVENT_DPREP_ALERT:
						Cl_Alarm_GetAlarmStatus(FPCURRENTSTATUS,&alarmstatus1);
						if(alarmstatus1)
						{
							Cl_dprepretcode = Cl_bc_controller(BC_EVENT_CS);
						}
						Cl_Alarm_GetAlarmStatus(TEMP2STATUS_HIGH,&alarmstatus3);
						if(alarmstatus3)
						{
							//	UpdateHeaterControls();
						}
									
						Cl_Alarm_GetAlarmStatus(FLOWSTATUS_FLOWOFF,&flowstatus);
						//	if(flowstatus)
						//	{
						//		Cl_rinseretcode = sv_cntrl_poweroffheater();
						//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLOW_OFF",8);
						//	}
						break;
					}
		break;
		
		case CL_DPREP_STATE_POST_DPREP_STANDBY:
		
			switch(cl_dprepevent)
			{
				case EVENT_DPREP_ALARM:
				Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
				break;
				case EVENT_DPREP_PATIENT_READY:
				Cl_PatientState = CL_DPREP_PATIENT_STATE_WAITING_FOR_BD;
				break;
				case EVENT_DPREP_PATIENT_CONNECTED:
				if(Cl_PatientState == CL_DPREP_PATIENT_STATE_BLOOD_DETECTED )
				{
					cl_dprepstate = CL_DPREP_STATE_READY_FOR_DALYSIS;
				}
				
				break;

				case EVENT_DPREP_TICK_MINUTE:
					Cl_Dprep_ResetAlertsforReassertion();
				break;
				case EVENT_DPREP_TICK_SECOND:
				//Cl_Dprep_SendPrepStateData();
			//	Cl_Dprep_ResetAlertsforReassertion();
			if(Cl_PatientState == CL_DPREP_PATIENT_STATE_WAITING_FOR_BD )
			{
				Cl_Alarm_GetAlarmStatus(BDSTATUS_ON  , &cl_status);
				
				//	if(cl_status == true)
				{
					//	Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_READY_FOR_DIALYSIS,NULL,0);
					//	cl_dprepstate = CL_DPREP_STATE_READY_FOR_DALYSIS;
									
									Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_BLOOD_DETECTED,NULL,0);
									Cl_PatientState = CL_DPREP_PATIENT_STATE_BLOOD_DETECTED;
				}
			}
				
				break;
			}
		
		break;
		case CL_DPREP_STATE_READY_FOR_DALYSIS:
				switch(cl_dprepevent)
				{

					case EVENT_DPREP_TICK_MINUTE:
						Cl_Dprep_ResetAlertsforReassertion();
					break;
					case EVENT_DPREP_TICK_SECOND:
					break;
					case 	EVENT_DPREP_COMMAND_GET_DATA:
						if(Cl_ConsoleRxMsg.msgready == true)
						{
							Cl_Dprep_Get_data(Cl_ConsoleRxMsg.data.byte[0],  1);			
					
						}
				
						break;
						case EVENT_DPREP_COMMAND_SET_DATA:
								 if(Cl_ConsoleRxMsg.msgready == true)
								 {
						 
						 
									 Cl_ConsoleRxDataType dataId;
									 uint8_t count;
									 dataId = Cl_ConsoleRxMsg.data.byte[0];
									 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
									 {
										 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
							 
									 }
									 Cl_Dprep_setdata(dataId, cl_PrepData,count);
						 
						 
						 
								 }
	
						break;
			
						case EVENT_DPREP_ALARM:
						Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
						break;
					default:
					break;
				}
		break;

		}
	#if 0
	
	
	if(cl_dprepevent == EVENT_DPREP_TICK_SECOND)
	{

		Cl_dprepretcode =  Cl_AlarmResetAlarm( LEVELSWITCH_OFF_TO_ON );
		Cl_dprepretcode =  Cl_AlarmResetAlarm( LEVELSWITCH_ON_TO_OFF );
		Cl_dprepretcode =  Cl_AlarmResetAlarm( FLOWSTATUS_FLOWOFF );

	}
	switch(cl_dprepstate)
	{

		case CL_DPREP_STATE_IDLE:
		 switch(cl_dprepevent)
		 {
			 case EVENT_DPREP_DIALYSIS_PREP:
			 // if everything is okay , check for if rinsed properly
				
				Cl_dprepretcode = Cl_DprepSelectDialysateInlet();
				//Cl_DprepFlowOn();
				Cl_dprepretcode = Cl_dprep_filling_FSM_Init(EVENT_DPREP_START_DIALYSATE_FILL);
				Cl_dprepretcode = Cl_dprep_filling_FSM(EVENT_DPREP_START_DIALYSATE_FILL);
				
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_PREP_CNFRM,&data,0);
							Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER1STATUS_OPEN,LOGIC_LOW,0,0,0);
							Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER2STATUS_OPEN,LOGIC_LOW,0,0,0);
							

							Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );
							

							Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER1STATUS_OPEN,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER2STATUS_OPEN,true );
							
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP3STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP2STATUS_HIGH,true );
							
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS1STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS2STATUS_HIGH,true );
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS3STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS,true );
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(APTSTATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_HIGH,true );
							if(Current_sense_trigger)
							{
							Cl_dprepretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
							}
							else
							{
								//			Cl_rinseretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );
							}
				
				cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_READY_FOR_PRIME;
			 break;

			 case EVENT_DPREP_COMMAND_GET_DATA:

					if(Cl_ConsoleRxMsg.msgready == true)
					{
	
						switch(Cl_ConsoleRxMsg.data.byte[0])
						{

							case	CON_RX_PARAM_DATA_PRIME_STATUS:
														//cl_wait(10);
														
														command = CON_TX_COMMAND_SYSDATA;
														//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
														dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
														Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
														
														//Reset the  OPENFILL time count
														Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
														//cl_wait(10);
							break;



							default:
							break;
						}
					}
				

			 break;
				case EVENT_DPREP_ALARM:
				break;
			 default:
			 break;
		 }
		 
		break;
		case CL_DPREP_STATE_INIT:
			 switch(cl_dprepevent)
			 {
				 case EVENT_DPREP_COMMAND_SET_DATA:
				 				 if(Cl_ConsoleRxMsg.msgready == true)
				 				 {
					 				 
					 				 
					 				 Cl_ConsoleRxDataType dataId;
					 				 uint8_t count;
					 				 dataId = Cl_ConsoleRxMsg.data.byte[0];
					 				 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
					 				 {
						 				 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
						 				 
					 				 }
					 				 Cl_Dprep_setdata(dataId, cl_PrepData,count);
					 				 
					 				 
					 				 
				 				 }
				 break;
				 case EVENT_DPREP_COMMAND_GET_DATA:
				 break;
				case EVENT_DPREP_ALARM:
				
				break;
				 default:
				 break;
			 }
		break;
		case CL_DPREP_STATE_DPREP_FILLING_READY_FOR_PRIME:
			switch(cl_dprepevent)
			{
				case EVENT_DPREP_START_DIALYSATE_FILL:
				Cl_dprepretcode = Cl_DprepSelectDialysateInlet();
				Cl_DprepFlowOn();
				Cl_dprepretcode = Cl_dprep_filling_FSM(EVENT_DPREP_START_DIALYSATE_FILL);
				break;

				 case EVENT_DPREP_COMMAND_SET_DATA:

				 if(Cl_ConsoleRxMsg.msgready == true)
				 {
					 
	
						 	Cl_ConsoleRxDataType dataId; 
							 uint8_t count;
							 dataId = Cl_ConsoleRxMsg.data.byte[0];
							 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
							 {
								 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
								 
							 }
							 Cl_Dprep_setdata(dataId, cl_PrepData,count);
			 
			
					 
				 }
	
				 break;
				case EVENT_DPREP_COMMAND_GET_DATA:
				

				
				if(Cl_ConsoleRxMsg.msgready == true)
				{
					Cl_Dprep_Get_data(Cl_ConsoleRxMsg.data.byte[0],  1);			
					
				}
				
	
				break;
				case EVENT_DPREP_COMMAND_SET_BLDPMP_ON:
				// sv_cntrl_activatepump(sv_pumpid);
				Cl_dprepretcode =  sv_cntrl_activatepump(BLOODPUMP);
		//		printf("k");

				break;
				
				case EVENT_DPREP_COMMAND_SET_BLDPMP_OFF:
				// sv_cntrl_deactivatepump(sv_pumpid);
				Cl_dprepretcode =  sv_cntrl_deactivatepump(BLOODPUMP);
				//printf("k");

				 break;
				case EVENT_DPREP_COMMAND_SET_BLDPUMPRATE:
				//printf("k");
				// sv_cntrl_setpumpspeed(sv_pumpid, sv_speed);
				cl_PrepData.word = 0;
				if(Cl_ConsoleRxMsg.datasize == 1)
				{
					//rateval = TestMsg.data[1];
					cl_PrepData.bytearray[0] = Cl_ConsoleRxMsg.data.byte[0];
				}
				else if(Cl_ConsoleRxMsg.datasize == 2)
				{
					//rateval = (uint16_t)TestMsg.data[1];
					cl_PrepData.Twobyte = Cl_ConsoleRxMsg.data.dataTwobyte;
				
								 	
				}
				else if(Cl_ConsoleRxMsg.datasize == 3)
				{
					cl_PrepData.bytearray[0] = Cl_ConsoleRxMsg.data.byte[0];
					cl_PrepData.bytearray[1] = Cl_ConsoleRxMsg.data.byte[1];
					cl_PrepData.bytearray[2] = Cl_ConsoleRxMsg.data.byte[2];
				}
				
			//	Cl_dprepretcode = sv_cntrl_setpumpspeed(BLOODPUMP,cl_PrepData.word);
				break;
				case EVENT_DPREP_START_PRIME:
			 // if everything is okay , check for if rinsed properly
			 	 sv_cntrl_activatepump(BLOODPUMP);
				// sv_cntrl_setpumpspeed(sv_pumpid, sv_speed);
				data = (uint8_t)COMMAND_RESULT_SUCCESS;
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_START_PRIME_CNFRM,&data,1);
				cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_PRIME;
				data = 0;
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_PRIME_STARTED,&data,0);
			//	Cl_dprepretcode = Cl_Dprep_UpdatePrimeTimeInfo();
				
				break;

				case EVENT_DPREP_HEP_PMP_SET:
				// sv_cntrl_activatepump(sv_pumpid);
				// sv_cntrl_setpumpspeed(sv_pumpid, sv_speed);
				break;

				case EVENT_DPREP_MAN_PREP_COMPLETED:

				break;
				
				case EVENT_DPREP_TICK_50MS:
				case EVENT_DPREP_TICK_100MS:

				case EVENT_DPREP_TICK_MINUTE:
				Cl_dprep_filling_FSM(cl_dprepevent);
				break;
				case EVENT_DPREP_TICK_SECOND:
				Cl_Dprep_SendPrepStateData();
				Cl_Dprep_ResetAlertsforReassertion();
				Cl_dprep_filling_FSM(cl_dprepevent);
				Cl_dprepsecondscounter++;
				if(Cl_dprepsecondscounter == 60)
				{
					Cl_Dprep_UpdateFillingTimeInfo();
				}
				

				break;
				case EVENT_DPREP_ALERT:
				case EVENT_DPREP_ALARM:
				Cl_dprep_filling_FSM(cl_dprepevent);
				break;
				

				default:
				break;
			}
		break;
		case CL_DPREP_STATE_DPREP_FILLING_DONE_READY_FOR_PRIME:
				switch(cl_dprepevent)
					{
						case EVENT_DPREP_START_PRIME:
						sv_cntrl_activatepump(BLOODPUMP);
						Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_START_PRIME_CNFRM,&data,0);
						cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME;
						Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_PRIME_STARTED,&data,0);
//						Cl_dprepretcode = Cl_Dprep_UpdatePrimeTimeInfo();
						break;
						 case EVENT_DPREP_COMMAND_GET_DATA:

								 if(Cl_ConsoleRxMsg.msgready == true)
								 {
			 
									 switch(Cl_ConsoleRxMsg.data.byte[0])
									 {

										 case	CON_RX_PARAM_DATA_PRIME_STATUS:
										 //cl_wait(10);
				 
										 command = CON_TX_COMMAND_SYSDATA;
										 //cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
										 dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
										 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
				 
										 //Reset the  OPENFILL time count
										 Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
										 //cl_wait(10);
										 break;



										 default:
										 break;
									 }
								 }
		 

						 break;
						case EVENT_DPREP_COMMAND_SET_DATA:
				 				 if(Cl_ConsoleRxMsg.msgready == true)
				 				 {
					 				 
					 				 
					 				 Cl_ConsoleRxDataType dataId;
					 				 uint8_t count;
					 				 dataId = Cl_ConsoleRxMsg.data.byte[0];
					 				 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
					 				 {
						 				 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
						 				 
					 				 }
					 				 Cl_Dprep_setdata(dataId, cl_PrepData,count);
					 				 
					 				 
					 				 
				 				 }
						break;
						default:
						break;
					}
				
				
		break;
		
		case CL_DPREP_STATE_DPREP_FILLING_PRIME:
		case CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE:
		case CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME:
		switch(cl_dprepevent)
			{
				case EVENT_DPREP_STOP_PRIME:
								sv_cntrl_deactivatepump(BLOODPUMP);
								switch(cl_dprepstate)
								{
									case  CL_DPREP_STATE_DPREP_FILLING_PRIME:
									cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_READY_FOR_PRIME;
									break;
									case CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME:
									cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_READY_FOR_PRIME;
									default:
									break;
								}
									
	
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_PRIME_STOPPED,&data,0);
				
				break;
				
				case EVENT_DPREP_TICK_50MS:
				case EVENT_DPREP_TICK_100MS:
				
				case EVENT_DPREP_TICK_MINUTE:
				if( cl_dprepfillingState != CL_DPREP_STATE_POST_FILLING_STANDBY)
				{
					Cl_dprep_filling_FSM(cl_dprepevent);
				}
				
				break;
				case EVENT_DPREP_TICK_SECOND:
				Cl_Dprep_SendPrepStateData();
				Cl_Dprep_ResetAlertsforReassertion();
				
				if( cl_dprepfillingState != CL_DPREP_STATE_POST_FILLING_STANDBY)
					{
						Cl_dprep_filling_FSM(cl_dprepevent);
					}
				Cl_Dprep_filling_secondscounter++;
				 if(!((cl_dprepstate == CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE) ||(cl_dprepstate == CL_DPREP_STATE_POST_PRIME_STANDBY)))
					 {
						 Cl_Dprep_Prime_secondscounter++;
					 					
						if(Cl_Dprep_Prime_secondscounter == 60)
						{
							UpdateDprepPrimeMinuteTick();
						}
				
									if (cl_prep_checkforprimecompletion() == CL_OK)
									{
										//stop prime
										//stop blood pump
										sv_cntrl_deactivatepump(BLOODPUMP);
										if(CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME == cl_dprepstate )
										{
										cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
										}
										else
										{
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE;
										}
							
										Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_PRIME_COMPLETED,&data,0);
						
									}
					
					 }
					if( cl_dprepfillingState != CL_DPREP_STATE_POST_FILLING_STANDBY)
					{
						if(Cl_Dprep_filling_secondscounter == 60)
						{
							UpdateDprepFillingMinuteTick();
						}
					}

					

					
		
				break;
				case EVENT_DPREP_ALERT:
				case EVENT_DPREP_ALARM:
				if( cl_dprepfillingState != CL_DPREP_STATE_POST_FILLING_STANDBY)
					{
						Cl_dprep_filling_FSM(cl_dprepevent);
					}
					else
					{
						Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
					}

				break;		
				
				case EVENT_DPREP_COMMAND_GET_DATA:

				if(Cl_ConsoleRxMsg.msgready == true)
				{
					
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{

						case CON_RX_PARAM_DATA_TREATMENT_DATA:
						Cl_Dprep_SendtreatementData();
						break;
						case	CON_RX_PARAM_DATA_PRIME_STATUS:
						//	cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_PRIME_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						//	cl_wait(10);
						break;
						default: break;
					}
				}
				default:
				break;
			}
		break;


		case CL_DPREP_STATE_PAUSE:
			switch(cl_dprepevent)
			{
				case EVENT_DPREP_COMMAND_SET_DATA:
				 if(Cl_ConsoleRxMsg.msgready == true)
				 {
					 
					 
					 Cl_ConsoleRxDataType dataId;
					 uint8_t count;
					 dataId = Cl_ConsoleRxMsg.data.byte[0];
					 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
					 {
						 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
						 
					 }
					 Cl_Dprep_setdata(dataId, cl_PrepData,count);
					 
					 
					 
				 }
				break;
				case EVENT_DPREP_COMMAND_GET_DATA:
				
											// check for limits if applicable
											if(Cl_ConsoleRxMsg.msgready == true)
											{
												if(Cl_ConsoleRxMsg.data.byte[0] == CON_RX_PARAM_DATA_RINSE_STATUS)
												{
													//	cl_dprepstate = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
												}
											}
				break;
				case EVENT_DPREP_COMMAND_SET_BLDPMP_ON:
				break;
				case EVENT_DPREP_COMMAND_SET_BLDPUMPRATE:
				break;
				case EVENT_DPREP_START_PRIME:
				break;
				case EVENT_DPREP_START_DIALISER_PRIME:
				
				break;
				case EVENT_DPREP_HEP_PMP_SET:
				break;
				case EVENT_DPREP_ALARM:
				break;
				default:
				break;
			}
		break;

		case CL_DPREP_STATE_POST_PRIME_STANDBY:
		switch(cl_dprepevent)
		{
			case EVENT_DPREP_COMMAND_GET_DATA:
						if(Cl_ConsoleRxMsg.msgready == true)
						{
																
							switch(Cl_ConsoleRxMsg.data.byte[0])
							{
								case CON_RX_PARAM_DATA_TREATMENT_DATA:
									Cl_Dprep_SendtreatementData();
								break;
								case CON_RX_PARAM_DATA_DIALYSER_PRIME_STATUS:
								command = CON_TX_COMMAND_SYSDATA;
								//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
								dataarray[0] = CON_TX_PARAM_DATA_DIALYSER_PRIME_STATUS;
							//	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_DIALYSER_PRIME_STATUS, &dataarray[1]);
								dataarray[1] = 0;									
								//Reset the  OPENFILL time count
								Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
								break;
								default:
								break;
							}
						}
			break;
			case EVENT_DPREP_DILYSER_CONNECTED:
			
				// check for micro switches
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSER_CONNECTED_CONFIRMED,&data,0);
			
			break;
			case EVENT_DPREP_START_DIALISER_PRIME:
								Cl_dprepMinutescounter = 0;
								Cl_dprepsecondscounter = 0;
								cl_dprepstate = CL_DPREP_STATE_DIALISER_PRIME;
					//			Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_START_DIALISER_PRIME_CNFRM,&data,0);
					
					//check bypass switches
					sv_cntrl_activatepump(BLOODPUMP);
					Cl_dprepretcode = Cl_DprepSelectDialysateInlet();			
					Cl_dprepretcode = Cl_dprep_filling_FSM_Init(EVENT_DPREP_START_DIALISER_PRIME);
					Cl_dprepretcode = Cl_dprep_filling_FSM(EVENT_DPREP_START_DIALISER_PRIME);
					Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER1STATUS_OPEN,LOGIC_LOW,0,0,0);
							Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER2STATUS_OPEN,LOGIC_LOW,0,0,0);
							

							Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );
							

							Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER1STATUS_OPEN,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER2STATUS_OPEN,true );
							
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP3STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP2STATUS_HIGH,true );
							
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS1STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS2STATUS_HIGH,true );
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS3STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS,true );
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(APTSTATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_HIGH,true );
							if(Current_sense_trigger)
							{
							Cl_dprepretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
							}
							else
							{
								//			Cl_rinseretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );
							}
				
				
				cl_dprepstate = CL_DPREP_STATE_DIALISER_PRIME;
			break;
			default:
			break;
		}
		
		break;
		case CL_DPREP_STATE_DIALISER_PRIME:
			switch(cl_dprepevent)
			{
				
				case EVENT_DPREP_STOP_DIALYSER_PRIME:
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSER_PRIME_STOPPED,&dataarray,0);
				cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
					Cl_Dprep_DialyserPrime_secondscounter = 0;
					Cl_Dprep_DialyserPrime_Minutescounter = 0;
					Cl_Dprep_DialyserPrime_TotalMinutescounter = 0;			
					sv_cntrl_poweroffheater();
				break;
				case EVENT_DPREP_MAC_PRIME_COMPLETED:
				break;
				case EVENT_DPREP_MAC_DIALYSER_PRIME_COMPLETED:
				
				cl_dprepstate = CL_DPREP_STATE_POST_DPREP_STANDBY;

				
				break;
				case EVENT_DPREP_COMMAND_SET_DATA:
											
				 if(Cl_ConsoleRxMsg.msgready == true)
				 {
					 
	
						 	Cl_ConsoleRxDataType dataId; 
							 uint8_t count;
							 dataId = Cl_ConsoleRxMsg.data.byte[0];
							 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
							 {
								 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
								 
							 }
							 Cl_Dprep_setdata(dataId, cl_PrepData,count);
			 
			
					 
				 }
				break;
				case EVENT_DPREP_COMMAND_GET_DATA:
											// check for limits if applicable
											
											
						if(Cl_ConsoleRxMsg.msgready == true)
						{
																
							switch(Cl_ConsoleRxMsg.data.byte[0])
							{
								
								case CON_RX_PARAM_DATA_TREATMENT_DATA:
								Cl_Dprep_SendtreatementData();
								break;
								case CON_RX_PARAM_DATA_RINSE_STATUS:
							//	cl_wait(10);
																	
								command = CON_TX_COMMAND_SYSDATA;
								//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
								dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
								Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
																	
								//Reset the  OPENFILL time count
								Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,2);
							//	cl_wait(10);
								break;
								default:
								break;
							}
						}
	
				break;
				case EVENT_DPREP_COMMAND_SET_BLDPMP_ON:
				sv_cntrl_activatepump(BLOODPUMP);
			//	printf("k");
				break;
				case EVENT_DPREP_COMMAND_SET_BLDPMP_OFF:
				sv_cntrl_deactivatepump(BLOODPUMP);
				break;
				
				case EVENT_DPREP_COMMAND_SET_BLDPUMPRATE:
				// sv_cntrl_setpumpspeed(sv_pumpid, sv_speed);
				break;
				case EVENT_DPREP_HEP_PMP_SET:
				// sv_cntrl_activatepump(sv_pumpid);
				break;
				case EVENT_DPREP_TICK_SECOND:
				Cl_Dprep_SendPrepStateData();
				Cl_Dprep_ResetAlertsforReassertion();
				
					Cl_dprep_filling_FSM(cl_dprepevent);
					Cl_dprepsecondscounter++;
					if(Cl_dprepsecondscounter == 60)
					{
						UpdateDprepDialyserPrimeMinuteTick();
					}
				
				break;
				case EVENT_DPREP_TICK_50MS:
				case EVENT_DPREP_TICK_100MS:
				Cl_dprep_filling_FSM(cl_dprepevent);
				break;
				
				case EVENT_DPREP_TICK_MINUTE:
					Cl_dprepsecondscounter = 0;
					Cl_dprepMinutescounter++;
					Cl_dprepTotalMinutescounter++;
					// check for sub state time out and transition
				

					if( Cl_dprepMinutescounter > CL_DPREP_DIALISER_PRIME_TIMEOUT_MIN)
					{
						Cl_dprepMinutescounter = 0;
						sv_cntrl_deactivatepump(BLOODPUMP);
						sv_cntrl_poweroffheater();
						cl_dprepstate = CL_DPREP_STATE_POST_DPREP_STANDBY;
						Cl_dprepretcode =  	cl_dprep_notifydacandgotodpreptandby();

					}
				break;
				case EVENT_DPREP_TICK_HOUR:
				Cl_dprephourscounter++;
				Cl_dprepTotalhourscounter++;
				break;
				case EVENT_DPREP_ALARM:
				case EVENT_DPREP_ALERT:

						Cl_dprep_filling_FSM(cl_dprepevent);
		
				break;
				case EVENT_DPREP_MAN_PREP_COMPLETED:
				Cl_dprepretcode = CL_OK;
				break;
				default:
				break;
			}
		break;
		case CL_DPREP_STATE_STOP:
		break;

		case CL_DPREP_STATE_POST_DPREP_STANDBY:
		switch(cl_dprepevent)
		{
			case EVENT_DPREP_PATIENT_CONNECTED:
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_PATIENT_CONNECTED_CONFIRMED,NULL,0);
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_READY_FOR_DIALYSIS,NULL,0);
				cl_dprepstate = CL_DPREP_STATE_READY_FOR_DALYSIS;
			break;
			case EVENT_DPREP_TICK_SECOND:
			Cl_Dprep_SendPrepStateData();
				Cl_Dprep_ResetAlertsforReassertion();
			
			Cl_Alarm_GetAlarmStatus(BDSTATUS_ON  , &cl_status);
		//	if(cl_status == true)
			{
			//	Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_READY_FOR_DIALYSIS,NULL,0);
			//	cl_dprepstate = CL_DPREP_STATE_READY_FOR_DALYSIS;
			}

				
			break;

			case EVENT_DPREP_ALARM:
			break;

			case EVENT_DPREP_COMMAND_SET_DATA:
					 if(Cl_ConsoleRxMsg.msgready == true)
					 {
						 
						 
						 Cl_ConsoleRxDataType dataId;
						 uint8_t count;
						 dataId = Cl_ConsoleRxMsg.data.byte[0];
						 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
						 {
							 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
							 
						 }
						 Cl_Dprep_setdata(dataId, cl_PrepData,count);
						 
						 
						 
					 }
	
			break;
			
			case EVENT_DPREP_COMMAND_GET_DATA:
			
			if(Cl_ConsoleRxMsg.msgready == true)
			{
				
				switch(Cl_ConsoleRxMsg.data.byte[0])
				{
				
				case CON_RX_PARAM_DATA_TREATMENT_DATA:
										
				command = CON_TX_COMMAND_SYS_STATE_DATA ;
				systemdataarray[0] = PRIMING_DATA;



				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &tempdata);
				systemdataarray[1] = tempdata.bytearray[0];
				systemdataarray[2] = tempdata.bytearray[1];
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &tempdata);
				systemdataarray[3] = tempdata.bytearray[0];
				systemdataarray[4] = tempdata.bytearray[1];
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &tempdata);
				systemdataarray[5] = tempdata.bytearray[0];
				systemdataarray[6] = tempdata.bytearray[1];
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &tempdata);

				systemdataarray[7] = tempdata.bytearray[0];
				systemdataarray[8] = tempdata.bytearray[1];
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &tempdata);
				
				systemdataarray[9] = tempdata.bytearray[0];
				systemdataarray[10] = tempdata.bytearray[1];
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &tempdata);
				systemdataarray[11] = tempdata.bytearray[0];
				systemdataarray[12] = tempdata.bytearray[1];
				//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &tempdata);
				Cl_SysStat_GetSensor_Status_Query(COND_STATUS,&tempdata);
				systemdataarray[7] = tempdata.bytearray[15];
				systemdataarray[8] = tempdata.bytearray[16];
				//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &tempdata);
				Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&tempdata);
				systemdataarray[17] = tempdata.bytearray[0];
				systemdataarray[18] = tempdata.bytearray[1];
				
				//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(APTSTATUS_HIGH, &tempdata);
				Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH,&tempdata);
				systemdataarray[19] = tempdata.bytearray[0];
				systemdataarray[20] = tempdata.bytearray[1];
				//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(VPTSTATUS_HIGH, &tempdata);
				Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH,&tempdata);
				systemdataarray[21] = tempdata.bytearray[0];
				systemdataarray[22] = tempdata.bytearray[1];
				
				systemdataarray[23] = 0xAA;
				systemdataarray[24] = 0xAA;


				Cl_dprepretcode = Cl_SendDatatoconsole(command,&systemdataarray,25);
				break;

			case	CON_RX_PARAM_DATA_DIALYSIS_FLOW:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
				break;
				case	CON_RX_PARAM_DATA_DIALYSIS_TEMP:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_HEPARIN_RATE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_APT_VALUE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_VPT_VALUE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_TMP_VALUE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_TMP_VALUE;
				//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_COND_VALUE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_UF_RATE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_UF_RATE;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_ABF_RATE:
			//	cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			#if 0
			case	CON_RX_PARAM_DATA_ABD_THRESHOLD:
			//cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_ABD_THRESHOLD;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABD_THRESHOLD, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			case	CON_RX_PARAM_DATA_BLD_THRESHOLD:
			//cl_wait(10);
						
				command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_DATA_BLD_THRESHOLD;
				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BLD_THRESHOLD, &dataarray[1]);
						
				//Reset the  OPENFILL time count
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,4);
			//	cl_wait(10);
			break;
			#endif
				default: break;
				}
			}
			break;
			
			default:
			break;
			
		}
		

		break;
		case CL_DPREP_STATE_READY_FOR_DALYSIS:
		switch(cl_dprepevent)
		{

			case EVENT_DPREP_TICK_MINUTE:
				Cl_Dprep_ResetAlertsforReassertion();
			break;
			case EVENT_DPREP_TICK_SECOND:
			break;
			case 	EVENT_DPREP_COMMAND_GET_DATA:
				if(Cl_ConsoleRxMsg.msgready == true)
				{
					Cl_Dprep_Get_data(Cl_ConsoleRxMsg.data.byte[0],  1);			
					
				}
				
				break;
					case EVENT_DPREP_COMMAND_SET_DATA:
								 if(Cl_ConsoleRxMsg.msgready == true)
								 {
						 
						 
									 Cl_ConsoleRxDataType dataId;
									 uint8_t count;
									 dataId = Cl_ConsoleRxMsg.data.byte[0];
									 for (count =0; count < Cl_ConsoleRxMsg.datasize;count++)
									 {
										 cl_PrepData.bytearray[count] = Cl_ConsoleRxMsg.data.byte[count+1];
							 
									 }
									 Cl_Dprep_setdata(dataId, cl_PrepData,count);
						 
						 
						 
								 }
	
						break;
			default:
			break;
		}
		break;
		default:break;
		
	}
	//printf("DP");
	
	#endif
	return Cl_dprepretcode;
}
 
 
Cl_ReturnCodes  cl_dprep_translatemacevent(MAC_EVENTS Cl_MacDprepEvt,Cl_Dprep_Events* cl_dprepevent)
{
	switch(Cl_MacDprepEvt)
	{
	
		case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
		*cl_dprepevent = EVENT_DPREP_DIALYSIS_PREP;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
		*cl_dprepevent = EVENT_DPREP_COMMAND_SET_BLDPMP_ON;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
		*cl_dprepevent = EVENT_DPREP_COMMAND_SET_BLDPMP_OFF;
		break;
		case 	EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
		*cl_dprepevent = EVENT_DPREP_COMMAND_SET_BLDPUMPRATE;
		break;
		case EVT_CONSOLE_COMMAND_SET_DATA:
		*cl_dprepevent =  EVENT_DPREP_COMMAND_SET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_GET_DATA:
		*cl_dprepevent = EVENT_DPREP_COMMAND_GET_DATA;
		break;
		
		case EVT_CONSOLE_COMMAND_START_DIALYSATE_FILLING:
				*cl_dprepevent = EVENT_DPREP_START_DIALYSATE_FILL;
				break;
		case EVT_CONSOLE_COMMAND_START_PRIME:
		*cl_dprepevent = EVENT_DPREP_START_PRIME;
		break;
		
		case EVT_CONSOLE_COMMAND_STOP_PRIME:
		*cl_dprepevent = EVENT_DPREP_STOP_PRIME;
		break;
		case EVT_CONSOLE_COMMAND_DILYSER_CONNECTED:
		*cl_dprepevent = EVENT_DPREP_DILYSER_CONNECTED;
		break;
		case EVT_CONSOLE_COMMAND_START_DIALISER_PRIME:
		*cl_dprepevent = EVENT_DPREP_START_DIALISER_PRIME;
		break;
		case EVT_CONSOLE_COMMAND_STOP_DIALISER_PRIME:
		*cl_dprepevent = EVENT_DPREP_STOP_DIALYSER_PRIME;
		break;	
		
		case EVT_CONSOLE_COMMAND_HEP_PMP_SET:
		*cl_dprepevent = EVENT_DPREP_HEP_PMP_SET;
		break;
		case EVT_CONSOLE_COMMAND_MAN_PREP_COMPLETED:
		*cl_dprepevent =   EVENT_DPREP_MAN_PREP_COMPLETED;
		break;
		
		case EVT_TICK_50M:
		*cl_dprepevent =   EVENT_DPREP_TICK_50MS;
		break;
		case  EVT_TICK_100M:
		*cl_dprepevent =   EVENT_DPREP_TICK_100MS;
		break;
		
		case EVT_TICK_500M:
		*cl_dprepevent =   EVENT_DPREP_TICK_500MS;
		break;
		case EVT_TICK_SEC:
		*cl_dprepevent =   EVENT_DPREP_TICK_SECOND;
		break;
		case EVT_TICK_MIN:
		*cl_dprepevent =   EVENT_DPREP_TICK_MINUTE;
		break;			 
		case EVT_TICK_HOUR:
		*cl_dprepevent =   EVENT_DPREP_TICK_HOUR;
		break;
		
		case MACREQ_PRIME_COMPLETED:
		*cl_dprepevent = EVENT_DPREP_MAC_PRIME_COMPLETED;
		break;
		case MACREQ_DIALYSER_PRIME_COMPLETED:
			*cl_dprepevent = EVENT_DPREP_MAC_DIALYSER_PRIME_COMPLETED;
		break;
		case EVT_ALARM_TRIGGERED:
		*cl_dprepevent =  EVENT_DPREP_ALARM;
		break;
		case EVT_ALERT_TRIGGERED:
		*cl_dprepevent =  EVENT_DPREP_ALERT;
		break;
		case EVT_CONSOLE_COMMAND_PATIENT_CONNECTED:
		*cl_dprepevent =  EVENT_DPREP_PATIENT_CONNECTED;
		break;
		case EVT_CONSOLE_COMMAND_PATIENT_READY:
		*cl_dprepevent = EVENT_DPREP_PATIENT_READY;
		break;
		case EVT_SEND_MIXING_PREP_START:
		*cl_dprepevent =  EVENT_DPREP_MIXING_PREP_START;
		break;
		case EVT_CONSOLE_COMMAND_RCIRC_START:
		*cl_dprepevent = EVENT_DPREP_START_RECIRC;
		break;
		default:
		break;
	}
		 return CL_OK;
}
Cl_ReturnCodes Cl_Dprep_UpdateTimeInfo(void)
{
	
	Cl_ReturnCodes  Cl_dprepretcode = CL_ERROR;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data[7] = {0,0,0,0,0,0,0};
	
	command = CON_TX_COMMAND_REM_TIME;

	
	data[1]= (uint8_t)Cl_dprepTotalMinutescounter;
	data[2]= (uint8_t)Cl_dprepTotalhourscounter;
	data[3]= (uint8_t)Cl_dprepsecondscounter;
	data[4]= (uint8_t) (CL_DPREP_PRIME_TIMEOUT_MIN - Cl_dprepTotalMinutescounter );
	data[5]= (uint8_t) (CL_DPREP_PRIME_TIMEOUT_HRS - Cl_dprepTotalhourscounter );
	data[6]= (uint8_t) (60 - Cl_dprepsecondscounter );
	
	Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,7);
	
	return CL_OK;
	}
	
	Cl_ReturnCodes Cl_Dprep_UpdateFillingTimeInfo(void)
{
	
	Cl_ReturnCodes  Cl_dprepretcode = CL_ERROR;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data[7] = {0,0,0,0,0,0,0};
	#if 0
	command = CON_TX_COMMAND_REM_TIME;

	data[0]= (uint8_t) DIALYSATE_FILLING_DATA;
	data[1]= (uint8_t)Cl_Dprep_filling_Minutescounter;
	data[2]= (uint8_t)0;
	data[3]= (uint8_t)Cl_Dprep_filling_secondscounter;
	data[4]= (uint8_t) (CL_DPREP_PRIME_TIMEOUT_MIN - Cl_Dprep_filling_TotalMinutescounter );
	data[5]= (uint8_t) 0;
	data[6]= (uint8_t) 0;
	
	Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,7);
	#endif
	return CL_OK;
	}
	
#if 0
		Cl_ReturnCodes Cl_Dprep_UpdatePrimeTimeInfo(void)	
			{
	
				Cl_ReturnCodes  Cl_dprepretcode = CL_ERROR;
				Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
				uint8_t data[7] = {0,0,0,0,0,0,0};
	
				command = CON_TX_COMMAND_REM_TIME;

				data[0] = (uint8_t) PRIMING_DATA;

				data[1]= (uint8_t)Cl_Dprep_Prime_Minutescounter;
				data[2]= (uint8_t)0;
				data[3]= (uint8_t)Cl_Dprep_Prime_secondscounter;
				data[4]= (uint8_t) (CL_DPREP_PRIME_TIMEOUT_MIN - Cl_Dprep_Prime_TotalMinutescounter );
				data[5]= (uint8_t) 0;
				data[6]= (uint8_t) 0;
	
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,7);
	
				return CL_OK;
			}
		Cl_ReturnCodes Cl_Dprep_UpdateDialyserPrimeTimeInfo(void)	
			{
				
				Cl_ReturnCodes  Cl_dprepretcode = CL_ERROR;
				Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
				uint8_t data[7] = {0,0,0,0,0,0,0};
				
				command = CON_TX_COMMAND_REM_TIME;
				data[0] = (uint8_t)DIALYSER_PRIME_DATA;

				data[1]= (uint8_t)Cl_Dprep_DialyserPrime_Minutescounter;
				data[2]= (uint8_t)0;
				data[3]= (uint8_t)Cl_Dprep_DialyserPrime_secondscounter;
				data[4]= (uint8_t) (CL_DPREP_PRIME_TIMEOUT_MIN - Cl_Dprep_DialyserPrime_TotalMinutescounter );
				data[5]= (uint8_t) 0;
				data[6]= (uint8_t) 0;
				
				Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,7);
				
				return CL_OK;
			}	
		
	#endif
	
	Cl_ReturnCodes cl_dprep_notifydacandgotodprep_postprimetandby(void)
	{
		Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
		
		uint8_t data =0;
		//inform DAC about rinse completed state.
		Cl_dprepsecondscounter = 0;
		Cl_dprepMinutescounter= 0;
		Cl_dprephourscounter= 0;
		Cl_dprepTotalMinutescounter= 0;
		Cl_dprepTotalhourscounter=0;
		data = 1;
		Cl_dprepretcode = sv_nvmsetdata(NV_NVM_PRIME_STATUS,&data,1);
		
		Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_PRIME_COMPLETED,&data,0);
	//	Cl_dprepretcode = Cl_mac_apprequesthandler(MACREQ_PRIME_COMPLETED);
		
		return Cl_dprepretcode;
		
	}
	Cl_ReturnCodes cl_dprep_notifydacandgotodpreptandby(void)
	{
		Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
		
		uint8_t data =0;
		//inform DAC about rinse completed state.
		Cl_dprepsecondscounter = 0;
		Cl_dprepMinutescounter= 0;
		Cl_dprephourscounter= 0;
		Cl_dprepTotalMinutescounter= 0;
		Cl_dprepTotalhourscounter=0;
		data = 1;
		Cl_dprepretcode = sv_nvmsetdata(NV_NVM_PRIME_STATUS,&data,1);
		
		Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALISYS_PRIME_COMPLETED,&data,0);
		Cl_dprepretcode = Cl_mac_apprequesthandler(MACREQ_DIALYSER_PRIME_COMPLETED);
		
		
		return Cl_dprepretcode;
	}
	
	Cl_ReturnCodes Cl_dprep_prime_FSM(Cl_Dprep_Events cl_dprepevent)
	{
	}
	
	Cl_ReturnCodes Cl_dprep_filling_FSM_Init(Cl_Dprep_Events cl_dprepevent)
	{
		cl_dprepfillingState = CL_DPREP_FILLING_IDLE;
			Cl_Dprep_filling_Minutescounter = 0;
			Cl_Dprep_filling_TotalMinutescounter = 0;
										
	}
	#if 0
	Cl_ReturnCodes Cl_dprep_filling_FSM(Cl_Dprep_Events cl_dprepevent)
	{
		
		Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
		
		static uint16_t dprep_wait_cnt = 0, bcs_wait = false ;
		uint8_t data = 0;
		uint16_t levelswitchstatus = 0;

								
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		
		bool alarmstatus1 = false,alarmstatus2 = false,alarmstatus3 = false,flowstatus = false;
	
			switch(cl_dprepfillingState)
			{
							case CL_DPREP_FILLING_IDLE:
										switch(cl_dprepevent)
										{
											case EVENT_DPREP_START_DIALYSATE_FILL:
											case EVENT_DPREP_START_DIALISER_PRIME:
											
											if(cl_dprepevent == EVENT_DPREP_START_DIALYSATE_FILL)
											{
												command = CON_TX_COMMAND_DIALYSATE_FILLING_CONFIRM;
											}
											else
											{
												command = CON_TX_COMMAND_START_DIALISER_PRIME_CNFRM;
											}
											
											data = (uint8_t)COMMAND_RESULT_SUCCESS;
												//Reset the  OPENFILL time count
								
											Cl_Dprep_filling_Minutescounter = 0;
											Cl_Dprep_filling_TotalMinutescounter = 0;
										
											Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,1);
											if(cl_dprepevent == EVENT_DPREP_START_DIALYSATE_FILL)
											{	
							
												command = CON_TX_COMMAND_DIALYSATE_FILLING_STARTED;
											}
											
											else
											{
												command =	CON_TX_COMMAND_DIALYSER_PRIME_STARTED;
											}																
											Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,0);
											
											if(cl_dprepevent == EVENT_DPREP_START_DIALYSATE_FILL)
											{	
												Cl_dprepretcode = Cl_Dprep_UpdateFillingTimeInfo();	
											}
											else
											{
												Cl_dprepretcode = Cl_Dprep_UpdateDialyserPrimeTimeInfo();
											}
											

											Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
											Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );

											if(Current_sense_trigger)
											{
												Cl_dprepretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );	
											}
											else
											{
												Cl_dprepretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );	
											}
											
											if(!g_testbcfreeze)
											{
												Cl_dprepretcode =  sv_cntrl_activatepump(DCMOTOR1);
												Cl_dprepretcode =  sv_cntrl_activatepump(DCMOTOR2);

												Cl_dprepretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(	BC_FLUSH_OPEN );
												Cl_dprepretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
												!BC_window;
											}
											else
											{
												Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
												Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
											}
												Cl_SysStat_GetSensor_Status_Query(LEVELSWITCH_ON_TO_OFF,&levelswitchstatus);
		
												if(levelswitchstatus == 0)
												{
													Cl_DprepFlowOn();
												}

												Cl_dprepretcode = SetHeaterState(CL_HEATER_STATE_START);

												bcs_wait = false;
												cl_dprepfillingState = CL_DPREP_STATE_BO1_V13V14_TRANSITION;
												default:
												break;
											}
										
									
									break;
									case CL_DPREP_STATE_BO1_V13V14_TRANSITION:
									switch(cl_dprepevent)
									{	
										case EVENT_DPREP_TICK_50MS:
										//Cl_Dprep_100msTMP();
										if(dprep_wait_cnt < 10)
										{
											dprep_wait_cnt ++;	// 1 sec wait
										}
										else
										{
											dprep_wait_cnt = 0;
											cl_dprepfillingState = CL_DPREP_STATE_BO1_V13V14;
												//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BO1",3);
										}
								
								
										break;

										case EVENT_DPREP_ALARM:
										// update console
								
										Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
								
										break;
										default:
										break;
									}
									break;
							case CL_DPREP_STATE_BO1_V13V14:
							switch(cl_dprepevent)
							{			
								case EVENT_DPREP_TICK_50MS:
								break;
								case EVENT_DPREP_TICK_SECOND:
								UpdateHeaterControls();
								Cl_Dprep_filling_secondscounter++;
								if(Cl_Dprep_filling_secondscounter == 60)
									{		if(cl_dprepstate != CL_DPREP_STATE_DIALISER_PRIME)
											{	
												Cl_dprepretcode = Cl_Dprep_UpdateFillingTimeInfo();	
											}
											else
											{
												Cl_dprepretcode = Cl_Dprep_UpdateDialyserPrimeTimeInfo();
											}											
									}
								Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
								Cl_dprepretcode = Cl_Dprep_CheckforfillingCompletion();

									if( Cl_dprepretcode == CL_OK )
									{
										switch(cl_dprepstate)
										{
											case CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE:
											cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
											break;
											case CL_DPREP_STATE_DPREP_FILLING_READY_FOR_PRIME:
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_READY_FOR_PRIME;
											break;
											case CL_DPREP_STATE_DPREP_FILLING_PRIME:
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME;
											break;
											default:
											break;
										}

											Cl_DprepFlowOff();
											sv_cntrl_deactivatepump(DCMOTOR1);
											sv_cntrl_deactivatepump(DCMOTOR2);
											sv_cntrl_poweroffheater();
											cl_dprepfillingState = CL_DPREP_STATE_POST_FILLING_STANDBY;
										//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
										//Cl_dprepretcode = cl_dprep_notifydacandgotopostfillingstandby();
											command = CON_TX_COMMAND_DIALYSATE_FILLING_COMPLETED;
											data = (uint8_t)COMMAND_RESULT_SUCCESS;
												//Reset the  OPENFILL time count
								

											
											Cl_Dprep_filling_secondscounter = 0;
											Cl_Dprep_filling_Minutescounter = 0;
											Cl_Dprep_filling_TotalMinutescounter = 0;
										
											Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,1);
										
										
									}
									else if(Cl_dprepretcode == CL_REJECTED )
									{
										
										if( CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE == cl_dprepstate )
										{
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME;
										}
										else
										{
											cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
										}
										
										cl_dprepfillingState = CL_DPREP_STATE_POST_FILLING_STANDBY;

											command = CON_TX_COMMAND_DIALYSATE_FILLING_COMPLETED;
											data = (uint8_t)COMMAND_RESULT_SUCCESS;
												//Reset the  OPENFILL time count
								
											Cl_Dprep_filling_secondscounter = 0;
											Cl_Dprep_filling_Minutescounter = 0;
											Cl_Dprep_filling_TotalMinutescounter = 0;
										
											Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,1);
										
									}
									else
									{
										
									}

								break;
								case EVENT_DPREP_TICK_MINUTE:
								//Cl_dprepretcode = Cl_Dprep_UpdateTimeInfo();

											Cl_Dprep_filling_secondscounter = 0;
											Cl_Dprep_filling_Minutescounter++ ;
											Cl_Dprep_filling_TotalMinutescounter++;
								
								

								

								break;
								case EVENT_DPREP_TICK_HOUR:
								


								
								
								break;
								case EVENT_DPREP_STOP_DIALYSATE_FILL:
								Cl_Dprep_filling_secondscounter = 0;
								Cl_Dprep_filling_Minutescounter = 0;
								Cl_Dprep_filling_TotalMinutescounter = 0;
								Cl_dprepretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
								cl_dprepfillingState = CL_DPREP_FILLING_IDLE;
								//
								break;

								case EVENT_DPREP_ALARM:
								
								Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
								
								break;
								case EVENT_DPREP_ALERT:
								Cl_Alarm_GetAlarmStatus(FPCURRENTSTATUS,&alarmstatus1);

										if(alarmstatus1 )
										{
									
											Cl_DprepMinutescounter = 0;

											if(!g_testbcfreeze)
											{
												Cl_dprepretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V13_14);
												BC_window =	!BC_window;
											}
											else
											{
												Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
										
												Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
											}
											if(Cl_dprepretcode == CL_OK)
											{
												cl_dprepfillingState = CL_DPREP_STATE_BO2_V13V14_TRANSITION;
												bcs_wait = false;
											}
									
										break;

											default:
											break;
										}
										
										Cl_Alarm_GetAlarmStatus(TEMP2STATUS_HIGH,&alarmstatus3);
										{
											if(alarmstatus3)
												{
											//	UpdateHeaterControls();
												}
											
										}
								}
								break;
								case CL_DPREP_STATE_BO2_V13V14_TRANSITION:
								switch(cl_dprepevent)
								{
									case EVENT_DPREP_TICK_50MS:
									
									//Cl_Dprep_100msTMP();										
									
									if(dprep_wait_cnt < 10)
									{
										dprep_wait_cnt ++; // 1 sec wait
									}
									else
									{
										dprep_wait_cnt = 0;
										cl_dprepfillingState = CL_DPREP_STATE_BO2_V13V14;
									//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BO2",3);
									}
									
									break;

									case EVENT_DPREP_ALARM:
									// update console
									
									Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
									
									break;
									default:
									break;
								}
								break;
								case CL_DPREP_STATE_BO2_V13V14:
								switch(cl_dprepevent)
								{
									case EVENT_DPREP_TICK_50MS:
									//Cl_Dprep_100msTMP();										
									break;
									
									case EVENT_DPREP_TICK_SECOND:
									UpdateHeaterControls();
									Cl_Dprep_filling_secondscounter++;
									if(Cl_Dprep_filling_secondscounter == 60)
									{
										if(cl_dprepstate != CL_DPREP_STATE_DIALISER_PRIME)
											{	
												Cl_dprepretcode = Cl_Dprep_UpdateFillingTimeInfo();	
											}
											else
											{
												Cl_dprepretcode = Cl_Dprep_UpdateDialyserPrimeTimeInfo();
											}
											
									}
								Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
								
								Cl_dprepretcode = Cl_Dprep_CheckforfillingCompletion();
									
									if( Cl_dprepretcode == CL_OK )
									{
										switch(cl_dprepstate)
										{
											case CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE:
											cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
											break;
											case CL_DPREP_STATE_DPREP_FILLING_READY_FOR_PRIME:
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_READY_FOR_PRIME;
											break;
											case CL_DPREP_STATE_DPREP_FILLING_PRIME:
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME;
											break;
											default:
											break;
										}

										Cl_DprepFlowOff();
										sv_cntrl_deactivatepump(DCMOTOR1);
										sv_cntrl_deactivatepump(DCMOTOR2);
										sv_cntrl_poweroffheater();
										cl_dprepfillingState = CL_DPREP_STATE_POST_FILLING_STANDBY;
										//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
										//Cl_dprepretcode = cl_dprep_notifydacandgotopostfillingstandby();
										command = CON_TX_COMMAND_DIALYSATE_FILLING_COMPLETED;
										data = (uint8_t)COMMAND_RESULT_SUCCESS;
										//Reset the  OPENFILL time count
										

										
										Cl_Dprep_filling_secondscounter = 0;
										Cl_Dprep_filling_Minutescounter = 0;
										Cl_Dprep_filling_TotalMinutescounter = 0;
										
										Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,1);
										
										
									}
									else if(Cl_dprepretcode == CL_REJECTED )
									{
										
										if( CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE == cl_dprepstate )
										{
											cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING_DONE_PRIME;
										}
										else
										{
											cl_dprepstate = CL_DPREP_STATE_POST_PRIME_STANDBY;
										}
										
										cl_dprepfillingState = CL_DPREP_STATE_POST_FILLING_STANDBY;

										command = CON_TX_COMMAND_DIALYSATE_FILLING_COMPLETED;
										data = (uint8_t)COMMAND_RESULT_SUCCESS;
										//Reset the  OPENFILL time count
										
										Cl_Dprep_filling_secondscounter = 0;
										Cl_Dprep_filling_Minutescounter = 0;
										Cl_Dprep_filling_TotalMinutescounter = 0;
										
										Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,1);
										
									}
									else
									{
										
									}


								break;
								case EVENT_DPREP_TICK_MINUTE:
								//Cl_dprepretcode = Cl_Dprep_UpdateTimeInfo();

											Cl_Dprep_filling_secondscounter = 0;
											Cl_Dprep_filling_Minutescounter++ ;
											Cl_Dprep_filling_TotalMinutescounter++;
								
								

								

								break;
									case EVENT_DPREP_TICK_HOUR:
									
									Cl_Dprephourscounter++;
									Cl_DprepTotalhourscounter++;

									
									
									break;
									case EVENT_DPREP_STOP_DIALYSATE_FILL:
												Cl_Dprep_filling_secondscounter = 0;
											Cl_Dprep_filling_Minutescounter = 0;
											Cl_Dprep_filling_TotalMinutescounter = 0;
										
									Cl_dprepretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
									cl_dprepfillingState = CL_DPREP_STATE_IDLE;
									//
									break;

									case EVENT_DPREP_ALARM:
									
									
									Cl_dprepretcode = Cl_Dprep_ProcessAlarms();
									
									break;
									case EVENT_DPREP_ALERT:
									Cl_Alarm_GetAlarmStatus(FPCURRENTSTATUS,&alarmstatus1);

									if(alarmstatus1 )
									{
										
									

										if(!g_testbcfreeze)
										{
											Cl_dprepretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
											!BC_window;
										}
										else
										{
											Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
											
											Cl_dprepretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
										}
										if(Cl_dprepretcode == CL_OK)
										{
											
											cl_dprepfillingState = CL_DPREP_STATE_BO1_V13V14_TRANSITION;
											bcs_wait = false;

										}
									}
									
										Cl_Alarm_GetAlarmStatus(TEMP2STATUS_HIGH,&alarmstatus3);
										{
											if(alarmstatus3)
											{
											//	UpdateHeaterControls();
											}
											
										}


									default:
									break;
								}
								break;
								
								default:
								break;
			}
				

	}
#endif	
	
	Cl_ReturnCodes Cl_Dprep_ProcessAlarms(void )
	{
		Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
		ClDprepAlarmIdType CldprepAlarmId;
		uint8_t data;
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		Cl_AlarmIdType cl_dprepalarmid;
		//	cl_wait(200);
		Cl_dprepretcode = Cl_Dprep_UpdateAlarmTable();
		//   	Cl_rinseretcode =	Cl_Alarm_GetLastAlarm(&cl_rinsealarmid);
		//   	data = (uint8_t)cl_rinsealarmid;
		//   	command = CON_TX_COMMAND_ALARM;
		//   	Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
		
		
		return (Cl_dprepretcode);
	}
	
	
	Cl_ReturnCodes Cl_Dprep_UpdateAlarmTable( void)
	{
		Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
		uint8_t tempcount = 0;
		uint8_t data[2] ={0, 0} ,*data1 = NULL;
		uint8_t data3;
		Cl_AlarmIdType cl_alarmId;
		
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		
		
		for (tempcount = 0 ; tempcount < CL_DPREP_ALRM_MAX ; tempcount++)
		{
			
			Cl_Alarm_GetAlarmStatus(Cl_DprepAlarmTable[tempcount].Cl_DprepAlarmId,&Cl_DprepAlarmTable[tempcount].IsActive);
			//	if(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmType == CL_ALARM_ALARM)
			//	{
			
			
			if (Cl_DprepAlarmTable[tempcount].IsActive)
			{
				if(!Cl_DprepAlarmTable[tempcount].IsRaised)
				{
				//	cl_alarmId = Cl_DprepAlarmTable[tempcount].Cl_DprepAlarmId;

				//	data[0] = (uint8_t)(Cl_DprepAlarmTable[tempcount].Cl_DprepAlarmId);
				//	data[1] = (uint8_t) CL_ALARM_TRIGGERED;
				//	data[1] = (uint8_t) CRITICAL;
					//	data1 = (uint8_t) cl_alarmId;

				//	command = CON_TX_COMMAND_ALARM;

				//	Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,3);
					Cl_DprepAlarmTable[tempcount].IsRaised = true;
					
					Cl_dprepretcode = CL_DrepAlarmActon(Cl_DprepAlarmTable[tempcount].Cl_DprepAlarmId);
					
					
				}
				
			}
			else
			{
				// alarm was present before , but not active now.
				if(Cl_DprepAlarmTable[tempcount].IsRaised == true)
				{
					Cl_DprepAlarmTable[tempcount].IsRaised = false;
					command = CON_TX_COMMAND_ALARM;
					data[0] = (uint8_t)(Cl_DprepAlarmTable[tempcount].Cl_DprepAlarmId);
					data[1] = (uint8_t) 0;
				//	Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,2);
				}
				Cl_DprepAlarmTable[tempcount].IsActive = true;

			}
			//}

		}

		return (Cl_dprepretcode );
	}

Cl_ReturnCodes  CL_DrepAlarmActon(Cl_AlarmIdType cl_dprepalarmid)
{
	Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
	uint16_t levelswitchstatus = 0;
	uint16_t wait_cnt = 0,TmpVal=0;
	static uint8_t fillseccounter=0;
	uint8_t data[3] ={0, 0};
	Cl_NewAlarmIdType NewAlarmId = _NO_ALARM;
				uint8_t data3;
				Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	switch(cl_dprepalarmid)
	{
			case BLOODDOOR_STATUS_OPEN:
			Cl_Dprep_Stoppreparation();
			NewAlarmId = BLOODDOOR_STATUS_OPEN;
			 Cl_Dprep_Stoppreparation();
			cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			Cl_SysStat_GetSensor_Status_Query(HOLDER1STATUS_OPEN,&levelswitchstatus);
			
			break;

			case HOLDER1STATUS_OPEN:
			// stop rinsing
			NewAlarmId = HOLDER1STATUS_OPEN;
			 Cl_Dprep_Stoppreparation();
			cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			Cl_SysStat_GetSensor_Status_Query(HOLDER1STATUS_OPEN,&levelswitchstatus);
			//enterl_saferinse_state();
			break;
			case HOLDER2STATUS_OPEN:
			// stop rinsing
			NewAlarmId = HOLDER2STATUS_OPEN;
			 Cl_Dprep_Stoppreparation();
			cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			break;
			case LEVELSWITCH_OFF_TO_ON:
			// TURN OFF  WATER INLET
			Cl_SysStat_GetSensor_Status_Query(LEVELSWITCH_OFF_TO_ON,&levelswitchstatus);
			{
				if(levelswitchstatus == 1)
				{
	
					//fillseccounter++;

					Cl_DprepFlowOff();
					//cl_gfillinprogress = false;		
					
				}
			}
			break;
			case LEVELSWITCH_ON_TO_OFF:
			// TURN ON WATER INLET
			Cl_SysStat_GetSensor_Status_Query(LEVELSWITCH_ON_TO_OFF,&levelswitchstatus);
			{
				if(levelswitchstatus == 0)
				{
					
					//fillseccounter++;

					Cl_DprepFlowOn();
					//cl_gfillinprogress = true;
					
				}
			}
			break;
			case TEMP3STATUS_HIGH:
	
			Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&TmpVal);
			int16_t temp3,temp4;
			temp3 = (0.805 * TmpVal) - 1004 ;
			temp4 = 3000 + (temp3 * 1000)/382;
			if(temp4 > 3680)
			{
					NewAlarmId = _TEMP3_HIGH_THRESHOLD;
					Cl_Dprep_Stoppreparation();
	
					cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			}
			if(temp4 < 3500)
			{
				NewAlarmId = _TEMP3_LOW_THRESHOLD;
				Cl_Dprep_Stoppreparation();

				cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			}
			//Cl_Rinse_StopRinse();
			//enterl_saferinse_state();

			break;
			case 	PS1STATUS_HIGH:
				NewAlarmId = _PS1_HIGH_THRESHOLD;
			 Cl_Dprep_Stoppreparation();
			cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			case	PS2STATUS_HIGH:
				NewAlarmId = _PS2_HIGH_THRESHOLD;
			 Cl_Dprep_Stoppreparation();
			cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			case	PS3STATUS_HIGH:
			//	NewAlarmId = _PS3_HIGH_THRESHOLD;
			 Cl_Dprep_Stoppreparation();
			cl_dprepstate = CL_DPREP_STATE_CRITICAL_ALARM;
			//enterl_saferinse_state();
			break;
			
			case APTSTATUS_HIGH:
					//	 Cl_Dprep_Stoppreparation();
			break;
			case VPTSTATUS_HIGH:
					//	 Cl_Dprep_Stoppreparation();
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

			Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,3);
			
					
			command = CON_TX_COMMAND_SYS_STATE;
			data[0] = (uint8_t)POST_CLEAN_STANDBY;
			data[1] = (uint8_t)PRIME_IDLE;
			Cl_dprepretcode = Cl_SendDatatoconsole(command,&data,2);
		 }
			
	
}
	
	
	Cl_ReturnCodes Cl_Dprep_CheckforfillingCompletion(void)
	{
		Cl_ReturnCodes Cl_dprepretcode = CL_ERROR;
		uint16_t cl_cond,cl_temp3,temp;
		float cl_cond_ms,cl_temp3_cel;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Cl_Dprep_filling_TotalMinutescounter,2);
		if(Cl_Dprep_filling_TotalMinutescounter > CL_DPREP_FILLING_TIMEOUT_MIN )
		{
			
		//	Cl_dprepretcode = CL_REJECTED;
			Cl_dprepretcode = CL_OK;
		}
		else
		{
			Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&cl_temp3);
			Cl_SysStat_GetSensor_Status_Query(COND_STATUS_HIGH,&cl_cond);
							temp = cl_temp3 * 0.805;
							cl_temp3_cel = 0.0000116 * temp *temp + 0.0035 *temp + 11.157;
			  
			//if(cl_cond > 13000000)
			{
				if ( cl_temp3_cel > 36.8 && cl_temp3_cel < 37.2)
				{
					if(cl_temp3_stable_confirm == true)
							Cl_dprepretcode = CL_OK;
				}
				
			}

			
		}
		
		return Cl_dprepretcode;
		
	}
	
	
	Cl_ReturnCodes cl_prep_checkforprimecompletion(void)
	{
		Cl_ReturnCodes Cl_dprepretcode = CL_ERROR;

		
		if(Cl_Dprep_Prime_TotalMinutescounter > CL_DPREP_PRIME_TIMEOUT_MIN )
		{
			
			Cl_dprepretcode = CL_OK;
		}

		
		return Cl_dprepretcode;
		
	}
	
	
	Cl_ReturnCodes  Cl_DprepSelectDialysateInlet(void)
	{
		Cl_ReturnCodes Cl_RetVal = CL_OK;
		//sv_cntrl_deactivate_valve(VALVE_ID18);
		//sv_cntrl_activate_valve(VALVE_ID19);
		sv_cntrl_activate_valve(VALVE_ID18);
		
	}
	Cl_ReturnCodes  Cl_DprepFlowOn(void )
	{
		Cl_ReturnCodes Cl_RetVal = CL_OK;
		sv_cntrl_activate_valve(VALVE_ID1);
		
		return Cl_RetVal;
	
	}

	Cl_ReturnCodes  Cl_DprepFlowOff(void )
	{
		Cl_ReturnCodes Cl_RetVal = CL_OK;
		sv_cntrl_deactivate_valve(VALVE_ID1);
		
		return Cl_RetVal;
	
	}
		//	#endif
		
		
Cl_ReturnCodes UpdateDprepMinuteTick(void)
{
					Cl_ReturnCodes Cl_dpreptcode = CL_OK;
					Cl_Dprepsecondscounter = 0;
					Cl_DprepMinutescounter++;
					Cl_DprepTotalMinutescounter++;
					Cl_dpreptcode = Cl_Dprep_UpdateTimeInfo();
					return Cl_dpreptcode;
}

Cl_ReturnCodes UpdateDprepFillingMinuteTick(void)
{
					Cl_ReturnCodes Cl_dpreptcode = CL_OK;

					Cl_Dprep_filling_secondscounter = 0;
					Cl_Dprep_filling_Minutescounter++;
					Cl_Dprep_filling_TotalMinutescounter++;

			//		Cl_dpreptcode = Cl_Dprep_UpdateFillingTimeInfo();
					return Cl_dpreptcode;
					

}

Cl_ReturnCodes UpdateDprepPrimeMinuteTick(void)
{
	Cl_ReturnCodes Cl_dpreptcode = CL_OK;

	Cl_Dprep_Prime_secondscounter = 0;
	Cl_Dprep_Prime_Minutescounter++;
	Cl_Dprep_Prime_TotalMinutescounter++;

//	Cl_dpreptcode = Cl_Dprep_UpdatePrimeTimeInfo();
	return Cl_dpreptcode;
	

}

Cl_ReturnCodes UpdateDprepDialyserPrimeMinuteTick(void)
{
	Cl_ReturnCodes Cl_dpreptcode = CL_OK;

	Cl_Dprep_DialyserPrime_secondscounter = 0;
	Cl_Dprep_DialyserPrime_Minutescounter++;
	Cl_Dprep_DialyserPrime_TotalMinutescounter++;

	Cl_dpreptcode = Cl_Dprep_UpdateDialyserPrimeTimeInfo();
	return Cl_dpreptcode;
	

}


Cl_ReturnCodes Cl_Dprep_SendtreatementData(void)
{

	uint8_t systemdataarray[40] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cl_PrepDatatype tempdata;
	static timecount = 0;
	Cl_ReturnCodes Cl_dprepretcode = CL_OK;
	uint8_t count = 0;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	//Cl_Console_bulkdatatype
	
	
						
	command = CON_TX_COMMAND_SYS_STATE_DATA ;
	systemdataarray[0] = TREATMENT_DATA;
	count++;


/*
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &tempdata);

	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &tempdata);
						
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &tempdata);
	Cl_SysStat_GetSensor_Status_Query(COND_STATUS_HIGH,&tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &tempdata);
	Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
						
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(APTSTATUS_HIGH, &tempdata);
	Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH,&tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(VPTSTATUS_HIGH, &tempdata);
	Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH,&tempdata);
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
						
	systemdataarray[count++] = 0xAA;
	systemdataarray[count++] = 0xAA;
*/

//	Cl_dprepretcode = Cl_SendDatatoconsole(command,&systemdataarray,count);
	
	
}
Cl_ReturnCodes Cl_Dprep_SendPrepStateData(Cl_Console_bulkdatatype datatype)
{

	uint8_t systemdataarray[40] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cl_PrepDatatype tempdata;
	int16_t temp, temp1;
	static timecount = 0;
	static float avgtmp3=0;
	Cl_ReturnCodes Cl_dprepretcode = CL_OK;
	uint8_t count = 0;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	//Cl_Console_bulkdatatype
	
	
						
	command = CON_TX_COMMAND_SYS_STATE_DATA ;
	systemdataarray[0] = datatype;
	count++;



//	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &tempdata);
	//systemdataarray[1] = tempdata.bytearray[0];
	//systemdataarray[2] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &tempdata);
	////systemdataarray[3] = tempdata.bytearray[0];
	//systemdataarray[4] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &tempdata);
	//systemdataarray[5] = tempdata.bytearray[0];
	//systemdataarray[6] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &tempdata);

	//systemdataarray[7] = tempdata.bytearray[0];
	//systemdataarray[8] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &tempdata);
						
	//systemdataarray[9] = tempdata.bytearray[0];
	//systemdataarray[10] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &tempdata);
	//systemdataarray[11] = tempdata.bytearray[0];
	//systemdataarray[12] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &tempdata);
	
	//Cl_SysStat_GetSensor_Status_Query(COND_STATUS,&tempdata.Twobyte);
	//temp = (0.805 * tempdata.Twobyte) - 949 ;
	//temp1 = 640 + (temp * 100)/121;
	//tempdata.Twobyte = (uint16_t)temp1;
	//systemdataarray[count++] = tempdata.bytearray[0];
	//systemdataarray[count++] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &tempdata);
	/*
		Cl_SysStat_GetSensor_Status_Query(TEMP2STATUS_HIGH,&tempdata);
		
		
		temp = (0.8056 * tempdata.Twobyte) - 1450 ;
		temp1 = 3700 + (temp * 1000)/340;
		

		
		tempdata.Twobyte = (uint16_t)temp1;
		systemdataarray[count++] = tempdata.bytearray[0];
		systemdataarray[count++] = tempdata.bytearray[1];
		
		
	Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&tempdata);
		
		
		temp = (0.8056 * tempdata.Twobyte) - 1450 ;
		temp1 = 3700 + (temp * 1000)/340;
		

	
	tempdata.Twobyte = (uint16_t)temp1;
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	*/
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
						
						systemdataarray[count++] = tempdata.bytearray[0];
						systemdataarray[count++] = tempdata.bytearray[1];
						systemdataarray[count++] = tempdata.bytearray[2];
						systemdataarray[count++] = tempdata.bytearray[3];
					}
					

		
		Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&temp);
		{
			tempdata.word = temp;
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
		
							Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH,&tempdata.Twobyte);
					{
						
					//	float ftemp,ftemp1;
					//	ftemp = data.twobytedata * 0.805;
					//	ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.6;
					//	avgtmp3 =	(avgtmp3*5 + ftemp1)/6;
					//	data.twobytedata = (uint16_t)(avgtmp3 * 100);
					tempdata.word	 = 100 * 100;
			systemdataarray[count++] = tempdata.bytearray[0] ;
			systemdataarray[count++] = tempdata.bytearray[1] ;
			systemdataarray[count++] = tempdata.bytearray[2] ;
			systemdataarray[count++] = tempdata.bytearray[3] ;
					}
					Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH,&tempdata.Twobyte);
					{
						
						//	float ftemp,ftemp1;
						//	ftemp = data.twobytedata * 0.805;
						//	ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.6;
						//	avgtmp3 =	(avgtmp3*5 + ftemp1)/6;
						//	data.twobytedata = (uint16_t)(avgtmp3 * 100);
					tempdata.word	 = 150 * 100;
			systemdataarray[count++] = tempdata.bytearray[0] ;
			systemdataarray[count++] = tempdata.bytearray[1] ;
			systemdataarray[count++] = tempdata.bytearray[2] ;
			systemdataarray[count++] = tempdata.bytearray[3] ;
					}

if( datatype == DIALYSIS_PREP_DATA)
{
						tempdata.word	 = 200 * 100;
						systemdataarray[count++] = tempdata.bytearray[0] ;
						systemdataarray[count++] = tempdata.bytearray[1] ;
						systemdataarray[count++] = tempdata.bytearray[2] ;
						systemdataarray[count++] = tempdata.bytearray[3] ;
}
		
		Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_STATE_DATA,&systemdataarray,count);
		
						
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(APTSTATUS_HIGH, &tempdata);
	Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH,&tempdata);
	temp = (0.805 * tempdata.Twobyte) - 1580 ;
	if(temp > 0)
	{
		temp1 = 0 + (temp * 100)/52;
	}
	else
	{
		temp1 = 0 + (temp * 100)/65;
	}
	tempdata.Twobyte = (uint16_t)temp1;
	
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
	//Cl_dprepretcode = (uint8_t)sv_nvmgetdata(VPTSTATUS_HIGH, &tempdata);
	Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH,&tempdata);
		temp = (0.805 * tempdata.Twobyte) - 1580 ;
	if(temp > 0)
	{
		temp1 = 0 + (temp * 100)/52;
	}
	else
	{
		temp1 = 0 + (temp * 100)/65;
	}
	
	tempdata.Twobyte = (uint16_t)temp1;
	systemdataarray[count++] = tempdata.bytearray[0];
	systemdataarray[count++] = tempdata.bytearray[1];
						
	systemdataarray[count++] = 0xAA;
	systemdataarray[count++] = 0xAA;


	//Cl_dprepretcode = Cl_SendDatatoconsole(command,&systemdataarray,count);
	
	
}


Cl_ReturnCodes Cl_Dprep_Stoppreparation(void)
{
	
	Cl_ReturnCodes Cl_dprepretcode = CL_OK;
	
				if(!((cl_dprepstate == CL_DPREP_STATE_IDLE ) || (cl_dprepstate == CL_DPREP_STATE_STOPPED ) ||(cl_dprepstate == CL_DPREP_STATE_CRITICAL_ALARM )  ))
				{
					
					
					 	Cl_Dprepsecondscounter = 0;
					 	Cl_DprepMinutescounter= 0;
					 	Cl_Dprephourscounter= 0;
					 	Cl_DprepTotalMinutescounter= 0;
					 	Cl_DprepTotalhourscounter=0;
							Cl_Dprep_filling_secondscounter = 0;
							Cl_Dprep_filling_Minutescounter = 0;
							Cl_Dprep_filling_TotalMinutescounter = 0;
					
							Cl_Dprep_Prime_secondscounter = 0;
							Cl_Dprep_Prime_Minutescounter = 0;
							Cl_Dprep_Prime_TotalMinutescounter = 0;
					
							Cl_Dprep_DialyserPrime_secondscounter = 0;
							Cl_Dprep_DialyserPrime_Minutescounter = 0;
							Cl_Dprep_DialyserPrime_TotalMinutescounter = 0;
				Cl_dprepretcode =  sv_cntrl_deactivatevenousclamp();
				Cl_dprepretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
				Cl_dprepretcode = sv_cntrl_poweroffheater();
				cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_STOP,0);

				
			//	cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_STOP_DIALYSER_PRIMING,0);
				
			
				cl_dprepstate = CL_DPREP_STATE_IDLE;
				//Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_RINSE_STOPPED,&data,0);
				
					
				}
}


Cl_ReturnCodes Cl_Dprep_ResetAlertsforReassertion(void )
{
	Cl_ReturnCodes 	Cl_dprepretcode = CL_OK;
	ClDprepAlarmIdType CldprepAlarmId;
	uint8_t data;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_AlarmIdType cl_dprepalarmid;
	
	Cl_DprepAlarmTable[CL_DPREP_ALARM_LEVELSWITCHON].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_LEVELSWITCHOFF].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_COND_STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_PS1STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_PS2STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_PS3STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_COND_STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_TEMP2STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_TEMP3STATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_HOLDER1].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_HOLDER2].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_APTSTATUS].IsRaised = false;
	Cl_DprepAlarmTable[CL_DPREP_ALARM_VPTSTATUS].IsRaised = false;
	
	
		
	return (Cl_dprepretcode);
	
}


Cl_ReturnCodes Cl_Dprep_Get_data(Cl_ConsoleRxDataType DataId, uint8_t size)
{
	Cl_ReturnCodes Cl_dprepretcode = CL_OK;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t dataarray[8] =  {0,0,0,0};
	uint8_t  datasize = 0;
	
	if(DataId == CON_RX_PARAM_DATA_TREATMENT_DATA )
	{
		Cl_Dprep_SendtreatementData();
	}
	else
	{
			switch (DataId)
	{

						case	CON_RX_PARAM_DATA_RINSE_STATUS:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
							datasize = 2;
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							break;
							case	CON_RX_PARAM_DATA_PRIME_STATUS:
						//	cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
							datasize = 2;
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
						break;
									
						case	CON_RX_PARAM_DATA_DIALYSIS_STATUS:
						//cl_wait(10);
			
						command = CON_TX_COMMAND_SYSDATA;

						dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_STATUS;
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DIALYSIS_STATUS, &dataarray[1]);
						datasize = 2;

						break;
						case	CON_RX_PARAM_DATA_DIALYSIS_FLOW:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);
							datasize = 2;
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,3);
							//cl_wait(10);
							break;
							case	CON_RX_PARAM_DATA_DIALYSIS_TEMP:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);
							datasize = 2;
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
						break;
						case	CON_RX_PARAM_DATA_HEPARIN_RATE:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE;
							//				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE, &dataarray[1]);
						
						datasize = 2;
						break;
						case	CON_RX_PARAM_DATA_APT_VALUE:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE;
							//			Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							datasize = 2;
						break;
						case	CON_RX_PARAM_DATA_VPT_VALUE:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE;
							//	Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							datasize = 2;
						break;
						case	CON_RX_PARAM_DATA_TMP_VALUE:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_TMP_VALUE;
							//			Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							datasize = 2;
						break;
						case	CON_RX_PARAM_DATA_COND_VALUE:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE;
							//				Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							datasize = 2;
						break;
						case	CON_RX_PARAM_DATA_UF_RATE:
						//	cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_UF_RATE;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
						//	cl_wait(10);
						datasize = 2;
						break;
						case	CON_RX_PARAM_DATA_ABF_RATE:
							//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							datasize = 2;
						break;
						#if 0
						case	CON_RX_PARAM_DATA_ABD_THRESHOLD:
						//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_ABD_THRESHOLD;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABD_THRESHOLD, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
						//	cl_wait(10);
						break;
						case	CON_RX_PARAM_DATA_BLD_THRESHOLD:
						//cl_wait(10);
						
							command = CON_TX_COMMAND_SYSDATA;
							//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
							dataarray[0] = CON_TX_PARAM_DATA_BLD_THRESHOLD;
							Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BLD_THRESHOLD, &dataarray[1]);
						
							//Reset the  OPENFILL time count
							//Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,1);
							//cl_wait(10);
							datasize = 2;
						break;
						#endif
						
						
						case CON_RX_PARAM_DATA_DIALYSIS_FLOW_CONFIG: //0x13
						
						
			
						
						dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW_CONFIG;
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);	
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_FLOW_CONFIG_UPPER, &dataarray[3]);
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_FLOW_CONFIG_LOWER, &dataarray[5]);	
						
						datasize = 	7;
							
					break;
					case CON_RX_PARAM_DATA_DIALYSIS_TEMP_CONFIG://0x14
											

											dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP_CONFIG;
											Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);
											Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_TEMP_CONFIG_UPPER, &dataarray[3]);
											Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_TEMP_CONFIG_LOWER, &dataarray[5]);
											datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_HEPARIN_RATE_CONFIG://0x15

									dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_APT_VALUE_CONFIG://0x16
							
									dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
									
					break;
					case CON_RX_PARAM_DATA_VPT_VALUE_CONFIG://0x17

									dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_TMP_VALUE_CONFIG://0x18

									dataarray[0] = CON_TX_PARAM_DATA_TMP_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_TMP, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_TMP_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_TMP_VALUE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_COND_VALUE_CONFIG://0x19

									dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_COND_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_COND_VALUE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_UF_RATE_CONFIG://0x1A

									dataarray[0] = CON_TX_PARAM_DATA_UF_RATE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_RATE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_RATE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_UF_GOAL_CONFIG://0x1B,

									dataarray[0] = CON_TX_PARAM_DATA_UF_GOAL_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_BOLUS_VOLUME_CONFIG://1C

									dataarray[0] = CON_TX_PARAM_DATA_BOLUS_VOLUME_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BOLUS_VOLUME_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BOLUS_VOLUME_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
					case CON_RX_PARAM_DATA_ABF_RATE_CONFIG://0X1D

									dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABF_RATE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABF_RATE_CONFIG_LOWER, &dataarray[5]);
									datasize = 	7;
					break;
						default:
						break;
	}
					command = CON_TX_COMMAND_SYSDATA;
					Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,datasize);
		
	}

}
Cl_ReturnCodes Cl_Dprep_setdata(Cl_ConsoleRxDataType DataId,cl_PrepDatatype cl_PrepData , uint8_t size)
{
	Cl_ReturnCodes Cl_dprepretcode = CL_OK;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t dataarray[8] =  {0,0,0,0};
	uint8_t  databytecnt = 0;

				switch(DataId)
				{
	 
					 case	CON_RX_PARAM_DATA_DIALYSIS_FLOW:
					 Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSATE_FLOW,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);					 
					 dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);
					
					 break;
					 case	CON_RX_PARAM_DATA_DIALYSIS_TEMP:
					 Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSATE_TEMP,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP; 
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);

					 break;
	 
					 case	CON_RX_PARAM_DATA_HEPARIN_RATE:
					 Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_FLOW_RATE,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &dataarray[1]);
					 break;
					 
					case CON_RX_PARAM_DATA_BOLUS_VOLUME:

					 Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_BOLUS,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_BOLUS_VOLUME;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &dataarray[1]);

					break;
					 case	CON_RX_PARAM_DATA_APT_VALUE:
					 Cl_dprepretcode =sv_nvmsetdata(NV_APT_VALUE,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE;	 
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);

					 break;
					 case	CON_RX_PARAM_DATA_VPT_VALUE:
					 Cl_dprepretcode =sv_nvmsetdata(NV_VPT_VALUE,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE;	 
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);

					 break;
					 case	CON_RX_PARAM_DATA_TMP_VALUE:

					 break;
					 case	CON_RX_PARAM_DATA_COND_VALUE:
					 Cl_dprepretcode =sv_nvmsetdata(NV_CONDUCTIVITY,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &dataarray[1]);

					 break;

					 case	CON_RX_PARAM_DATA_UF_RATE:
					 Cl_dprepretcode =sv_nvmsetdata(NV_UF_REMOVAL_RATE,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_UF_RATE;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);

					 break;
					 case CON_RX_PARAM_DATA_UF_GOAL:
					 Cl_dprepretcode =sv_nvmsetdata(NV_UF_GOAL,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 dataarray[0] = CON_TX_PARAM_DATA_UF_GOAL;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &dataarray[1]);

					 break;
					 case	CON_RX_PARAM_DATA_ABF_RATE:
					 Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_FLOW_RATE,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);					 
					 command = CON_TX_COMMAND_SYSDATA;					 
					 dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &dataarray[1]);					 
					 break;
					
					#if 0
					 case CON_RX_PARAM_DATA_ABD_THRESHOLD://0x0C
					 Cl_dprepretcode =sv_nvmsetdata(NV_ABD_THRESHOLD,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 command = CON_TX_COMMAND_SYSDATA;
					 dataarray[0] = CON_TX_PARAM_DATA_ABD_THRESHOLD;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABD_THRESHOLD, &dataarray[1]);		
					 
					 break;
					 case CON_RX_PARAM_DATA_BLD_THRESHOLD://0x0D
					 Cl_dprepretcode =sv_nvmsetdata(NV_BLD_THRESHOLD,&cl_PrepData.bytearray[0],Cl_ConsoleRxMsg.datasize);
					 command = CON_TX_COMMAND_SYSDATA;
					 dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE;
					 Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BLD_THRESHOLD, &dataarray[1]);
					 break;
					 
					#endif
					case CON_RX_PARAM_DATA_DIALYSIS_FLOW_CONFIG: //0x13
						
						
						
						Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSATE_FLOW,&cl_PrepData.bytearray[0],2);
						Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSIS_FLOW_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
						Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSIS_FLOW_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
						
						dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_FLOW_CONFIG;
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_FLOW, &dataarray[1]);	
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_FLOW_CONFIG_UPPER, &dataarray[3]);
						Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_FLOW_CONFIG_LOWER, &dataarray[5]);		
							
					break;
					case CON_RX_PARAM_DATA_DIALYSIS_TEMP_CONFIG://0x14
											
											Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSATE_TEMP,&cl_PrepData.bytearray[0],2);
											Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSIS_TEMP_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
											Cl_dprepretcode =sv_nvmsetdata(NV_DIALYSIS_TEMP_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
											
											dataarray[0] = CON_TX_PARAM_DATA_DIALYSIS_TEMP_CONFIG;
											Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSATE_TEMP, &dataarray[1]);
											Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_TEMP_CONFIG_UPPER, &dataarray[3]);
											Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_DIALYSIS_TEMP_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_HEPARIN_RATE_CONFIG://0x15
									Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_FLOW_RATE,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_RATE_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_RATE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_HEPARIN_RATE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_FLOW_RATE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_RATE_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_APT_VALUE_CONFIG://0x16
									Cl_dprepretcode =sv_nvmsetdata(NV_APT_VALUE,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_APT_VALUE_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_APT_VALUE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
																
									dataarray[0] = CON_TX_PARAM_DATA_APT_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_APT_VALUE_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_VPT_VALUE_CONFIG://0x17
									Cl_dprepretcode =sv_nvmsetdata(NV_VPT_VALUE,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_VPT_VALUE_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_VPT_VALUE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_VPT_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_VPT_VALUE_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_TMP_VALUE_CONFIG://0x18
									Cl_dprepretcode =sv_nvmsetdata(NV_TMP,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_TMP_VALUE_CONFIG_UPPER,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_TMP_VALUE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_TMP_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_TMP, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_TMP_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_TMP_VALUE_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_COND_VALUE_CONFIG://0x19
									Cl_dprepretcode =sv_nvmsetdata(NV_CONDUCTIVITY,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_COND_VALUE_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_COND_VALUE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_COND_VALUE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_CONDUCTIVITY, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_COND_VALUE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_COND_VALUE_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_UF_RATE_CONFIG://0x1A
									Cl_dprepretcode =sv_nvmsetdata(NV_UF_REMOVAL_RATE,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_UF_RATE_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_UF_RATE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_UF_RATE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_REMOVAL_RATE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_RATE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_RATE_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_UF_GOAL_CONFIG://0x1B,
									Cl_dprepretcode =sv_nvmsetdata(NV_UF_GOAL,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_UF_GOAL_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_UF_GOAL_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_UF_GOAL_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_UF_GOAL_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_BOLUS_VOLUME_CONFIG://1C
									Cl_dprepretcode =sv_nvmsetdata(NV_HEPARIN_BOLUS,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_BOLUS_VOLUME_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_BOLUS_VOLUME_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_BOLUS_VOLUME_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_HEPARIN_BOLUS, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BOLUS_VOLUME_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_BOLUS_VOLUME_CONFIG_LOWER, &dataarray[5]);
					break;
					case CON_RX_PARAM_DATA_ABF_RATE_CONFIG://0X1D
									Cl_dprepretcode =sv_nvmsetdata(NV_ARTERIAL_BLOODFLOW_RATE,&cl_PrepData.bytearray[0],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_ABF_RATE_CONFIG_UPPER,&cl_PrepData.bytearray[2],2);
									Cl_dprepretcode =sv_nvmsetdata(NV_ABF_RATE_CONFIG_LOWER,&cl_PrepData.bytearray[4],2);
									
									dataarray[0] = CON_TX_PARAM_DATA_ABF_RATE_CONFIG;
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ARTERIAL_BLOODFLOW_RATE, &dataarray[1]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABF_RATE_CONFIG_UPPER, &dataarray[3]);
									Cl_dprepretcode = (uint8_t)sv_nvmgetdata(NV_ABF_RATE_CONFIG_LOWER, &dataarray[5]);
					break;
					
					 default:
					 break;
				}
					command = CON_TX_COMMAND_SYSDATA;
					Cl_dprepretcode = Cl_SendDatatoconsole(command,&dataarray,size);
}

Cl_ReturnCodes	Cl_Dprep_StartPreparation(void)
	{
		
			Cl_ReturnCodes Cl_dprepretcode = CL_OK;
			uint8_t data =0;
				
							Cl_dprepretcode = Cl_DprepSelectDialysateInlet();
							//Cl_DprepFlowOn();

							data = 1;
							Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DLSIS_PREP_CNFRM,&data,0);	
							Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSATE_FILLING_STARTED,&data,0);																			

						
					//		Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER1STATUS_OPEN,LOGIC_LOW,0,0,0);
					//		Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER2STATUS_OPEN,LOGIC_LOW,0,0,0);
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(BLOODDOOR_STATUS_OPEN,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );
							Cl_dprepretcode =  sv_cntrl_activatevenousclamp();
							

							Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER1STATUS_OPEN,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER2STATUS_OPEN,true );
							
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(FLOWSTATUS_FLOWON,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(FLOWSTATUS_FLOWOFF,true );					
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(FLOW_LOW_FLOWRATE,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(FLOW_HIGH_FLOWRATE,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
							Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS_LOW,true );
							
							
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP3STATUS_HIGH,true );
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP2STATUS_HIGH,true );
							
							
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS1STATUS_HIGH,true );
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS2STATUS_HIGH,true );
							
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS3STATUS_HIGH,true );
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
							
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(APTSTATUS_HIGH,true );
						//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_HIGH,true );
							if(Current_sense_trigger)
							{
								Cl_dprepretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
							}
							else
							{
								//			Cl_rinseretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );
							}
							Cl_dprepretcode =  sv_cntrl_activatepump(DCMOTOR1);
							Cl_dprepretcode =  sv_cntrl_activatepump(DCMOTOR2);
							Cl_dprepretcode = Cl_bc_controller(BC_EVENT_RESUME);
							sv_prop_startmixing();
							Cl_dprepretcode = SetHeaterState(CL_HEATER_STATE_START);
							cl_dprepstate = CL_DPREP_STATE_DPREP_FILLING;
							
							return 0;
				
	}
	
	
	Cl_ReturnCodes	Cl_Dprep_StartDialyserPrime(void)
	{
		
		
			Cl_ReturnCodes Cl_dprepretcode = CL_OK;
			uint8_t data =0;
				
			Cl_dprepMinutescounter = 0;
			Cl_dprepsecondscounter = 0;
			cl_dprepstate = CL_DPREP_STATE_DIALISER_PRIME;
			//Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_START_DIALISER_PRIME_CNFRM,&data,0);
			
			//check bypass switches
		//	cl_dprep_primecontroller(CL_DPREP_PRIME_BLOODPUMP_START,0);
			Cl_dprepretcode = Cl_DprepSelectDialysateInlet();
			Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(BLOODDOOR_STATUS_OPEN,LOGIC_LOW,0,0,0);
			Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER1STATUS_OPEN,LOGIC_LOW,0,0,0);
			Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(HOLDER2STATUS_OPEN,LOGIC_LOW,0,0,0);
			

			Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
			Cl_dprepretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );
			

	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER1STATUS_OPEN,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(HOLDER2STATUS_OPEN,true );
			
			
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP3STATUS_HIGH,true );
		//	Cl_dprepretcode =  Cl_AlarmActivateAlarms(TEMP2STATUS_HIGH,true );
			
			
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS1STATUS_HIGH,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS2STATUS_HIGH,true );
			
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(PS3STATUS_HIGH,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_STATUS_LOW,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_DAC_OPEN,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_DAC_RO,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(COND_DAC_HIGH,true );
			
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(APTSTATUS_HIGH,true );
	//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_HIGH,true );
			if(Current_sense_trigger)
			{
				Cl_dprepretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
			}
			else
			{
				//			Cl_rinseretcode =  Cl_AlarmActivateAlarms( PS3STATUS_HIGH,true );
			}
			
			Cl_dprepretcode =  sv_cntrl_activatepump(DCMOTOR1);
			Cl_dprepretcode =  sv_cntrl_activatepump(DCMOTOR2);
			if(cl_dprep_prime_state != CL_DPREP_PRIME_STATE_DIALYSER_PRIMING)
			{
				cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_START_DIALYSER_PRIMING,0);
			}
			
			Cl_dprepretcode = Cl_bc_controller(BC_EVENT_RESUME);
			Cl_dprepretcode = SetHeaterState(CL_HEATER_STATE_START);
			
			UpdateDprepFillingMinuteTick();
			
			cl_dprepstate = CL_DPREP_STATE_DIALISER_PRIME;
							
			return 0;
	}
	
Cl_ReturnCodes Cl_dprep_StopMixing(void)
{
				Cl_ReturnCodes Cl_dprepretcode = CL_OK;
				uint8_t data;
				if(cl_dprepstate == CL_DPREP_STATE_DPREP_FILLING)
				{
					
				Cl_dprepMinutescounter = 0;
				Cl_dprepsecondscounter = 0;
;
				Cl_dprepretcode = sv_cntrl_poweroffheater();
				Cl_bc_controller(BC_EVENT_STOP);
				//Cl_rinseretcode = sv_cntrl_setflowpath(FLOW_PATH_IDLE_RINSE);
				cl_dprepstate = CL_DPREP_STATE_STOPPED;
				Cl_dprepretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSATE_FILLING_STOPPED,&data,0);
				
					
				}

				return Cl_dprepretcode;
}


	
Cl_ReturnCodes cl_dprep_activate_prime_related_alarms(void)
{
			Cl_ReturnCodes Cl_dprepretcode = CL_OK;
			//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(APTSTATUS_HIGH,true );
			//		Cl_dprepretcode =  Cl_AlarmActivateAlarms(VPTSTATUS_HIGH,true );
			Cl_dprepretcode =  Cl_AlarmConfigureAlarmType(BLOODDOOR_STATUS_OPEN,LOGIC_LOW,0,0,0);
	
}