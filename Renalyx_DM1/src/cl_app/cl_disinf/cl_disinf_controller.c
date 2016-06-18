/*
 * cl_disinf_controller.c
 *
 * Created: 12/27/2013 12:27:27 PM
 *  Author: user
 */ 

#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "inc/cl_disinf_controller.h"
#include "cl_app/inc/cl_types.h"
#include "sv_stubs/inc/sv_types.h"
#include "cl_app/comp/flowswitchcntrl/inc/flowswitchcntrl.h"
#include "cl_app/comp/deaeration/inc/cl_deaeration.h"
#include "cl_app/cl_bc_cntrl/inc/Cl_bc_controller.h"
#include "Platform/Service/sv_interface.h"


Cl_ReturnCodes  cl_disinf_translatemacevent(MAC_EVENTS Cl_MacdisinfEvt,Cl_disinf_Eevents* cl_disinf_event);

Cl_ReturnCodes Cl_disinf_init(void);
Cl_ReturnCodes Cl_disinf_start(void);
Cl_ReturnCodes Cl_disinf_controller(MAC_EVENTS Cl_MacDisinfEvent);
Cl_ReturnCodes  Cl_disinf_ProcessAlarms(void);
Cl_ReturnCodes Cl_Disinf_UpdateTimeInfo(void);
Cl_ReturnCodes Cl_disinf_ResetAlertsforReassertion(void );
Cl_ReturnCodes Cl_Disinf_StartDisinf(void);
Cl_ReturnCodes Cl_Disinf_StopDisinf(void);
 Cl_ReturnCodes Cl_Disinf_UpdateTimeInfo(void);
 Cl_ReturnCodes UpdateDisinfMinuteTick(void);
 Cl_ReturnCodes Cl_Disinf_SendDisinfStateData(void);

extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes  sv_nvmgetdata(uint8_t,uint8_t*);
extern void sv_prop_startpropeo_aligning();
extern void sv_prop_startmixing();
extern void sv_prop_stopmixing();
extern uint8_t sv_cs_setpotvalue(uint32_t resistance) ;
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
extern Cl_ReturnCodes cl_flow_switchcontroller(flowswitchcntrl_event_type cntrl_event);
extern Cl_ReturnCodes cl_deaeration_controller(cl_deaeration_event_type cntrl_event);
extern Cl_ReturnCodes  Cl_bc_controller(Cl_BC_EventType cl_bc_event);
extern Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_NewAlarmIdType  );
extern Cl_ReturnCodes Cl_AlarmConfigureAlarmType(Cl_NewAlarmIdType,Cl_AlarmTriggerType,uint16_t,uint16_t,uint8_t);
extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_NewAlarmIdType,bool );
extern void sv_prop_start_disinfect_intake(void);
extern void sv_prop_stop_disinfect_intake(void);
extern uint8_t sv_cntrl_enable_loopback(void);
extern uint8_t sv_cntrl_disable_loopback(void);
extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern Cl_ReturnCodes cl_wait(uint32_t );

Cl_disinf_StatesType cl_disinfstate = CL_DISINF_STATE_IDLE;
Cl_disinf_SubStatesType Cl_disinf_SubState = CL_DISINF_RINSING_IDLE;
int Cl_disinfsecondscounter =0 ,Cl_disinfMinutescounter=0, Cl_disinfshourscounter=0,Cl_disinfOpenFillsecondscounter=0;
int Cl_disinfTotalMinutescounter=0, Cl_disinfsTotalhourscounter=0;
bool Cl_disinfOpenFillTimeOut = true;

uint16_t sec_count=0;

extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;

 Cl_ReturnCodes Cl_disinf_init(void)
 {
	 Cl_ReturnCodes cl_disinf_retval;
	
	 return CL_OK;
 }
 
  Cl_ReturnCodes Cl_disinf_start(void)
  {
	  
	  return CL_OK;
  }
  
  
  DisinfAlarmsType Cl_DisinfAlarmTable[CL_DISINF_ALRM_MAX] =
  {
	  {BLOODDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	  {BYPASSDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	  {LEVELSWITCH_OFF_TO_ON,CL_ALARM_ALERT,false,false,false},
	  {LEVELSWITCH_ON_TO_OFF,CL_ALARM_ALERT,false,false,false},
	  {HOLDER1STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	  {HOLDER2STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	  {TEMP2_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
	  {TEMP3_HIGH_THRESHOLD,CL_ALARM_ALARM,false,false,false},
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



Cl_ReturnCodes Cl_disinf_controller(MAC_EVENTS Cl_MacDisinfEvent)
{
		Cl_ReturnCodes  Cl_disinfretcode = CL_OK;
		Cl_disinf_Eevents cl_disinfevent = EVENT_NULL_EVENT;
		Cl_NewAlarmIdType cl_disinfalarmid;
		CldisinfAlarmIdType CldisinfAlarmId;

		
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		uint8_t dataarray[4] = {0,0,0,0};
		uint8_t data = 0;
		bool alarmstatus1 = false,alarmstatus2 = false,alarmstatus3 = false, flowstatus = false;


		Cl_disinfretcode = cl_disinf_translatemacevent(Cl_MacDisinfEvent,&cl_disinfevent);
		
		
		
		switch(cl_disinfstate)
		{

			case CL_DISINF_STATE_INIT:
			break;
			case CL_DISINF_STATE_IDLE:
			switch (cl_disinfevent)
			{
				case EVT_DISINF_GET_DATA:
				if(Cl_ConsoleRxMsg.msgready == true)
				{
					
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case CON_RX_PARAM_DATA_DISINF_STATUS:
						command = CON_TX_COMMAND_SYSDATA;
						dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
						Cl_disinfretcode = (uint8_t)sv_nvmgetdata(NV_NVM_DISINF_STATUS, &dataarray[1]);
						Cl_disinfretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						
						break;
						case CON_RX_PARAM_DATA_PRIME_STATUS:
						command = CON_TX_COMMAND_SYSDATA;
						dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
						Cl_disinfretcode = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
						Cl_disinfretcode = Cl_SendDatatoconsole(command,&dataarray,2);
						default:
						break;
					}
				}
				break;
				case EVENT_START_DISINF:
				Cl_disinfretcode = Cl_mac_apprequesthandler(MACREQ_START_PRE_DISINF_RINSE);
				
				command = CON_TX_COMMAND_DISINF_STARTED;
				data = (uint8_t)COMMAND_RESULT_SUCCESS;
				Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,1);
									
									
				cl_disinfstate = CL_DISINF_STATE_FIRST_RO_RINSE;
				
				//Cl_Disinf_StartDisinf();
				
				break;
				case EVENT_DISINF_ALARM:
				// check for disinf alarms and update disinf alarm table
				
				Cl_disinfretcode =  Cl_disinf_ProcessAlarms();
				break;
				default:
				break;
				
				
			}
			break;
			case CL_DISINF_STATE_FIRST_RO_RINSE:
				switch ( cl_disinfevent)
				{
					case EVENT_PRE_DISINFECT_RINSE_COMPLETED:
					
					cl_disinfstate = CL_DISINF_STATE_FIRST_RO_RINSCOMPLETED;
					
					command = CON_TX_COMMAND_DISINF_STAGE_COMPLETED;
					data = (uint8_t)1;
					Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,1);
					
					cl_wait(5);

					command = CON_TX_COMMAND_DISINF_PAUSED;
					data = (uint8_t)0;
					Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);
					//Send Pause

					break;
					case EVENT_DISINF_TICK_SECOND:
									
					Cl_disinfsecondscounter++;
					Cl_Disinf_SendDisinfStateData();
					if(Cl_disinfsecondscounter == 60)
					{
	
						UpdateDisinfMinuteTick();
						
							
					}

					break;
					
					default:break;
				}
				
			break;
			
			case CL_DISINF_STATE_FIRST_RO_RINSCOMPLETED:
				switch ( cl_disinfevent)
				{ 
					case EVT_RESUME_DISINF_STAGE:
					 Cl_Disinf_StartDisinf();
					 cl_disinfstate = CL_DISINF_STATE_DISINF_OPENFILL;
					 
					 //Send resumed
					 
					 command = CON_TX_COMMAND_DISINF_RESUMED;
					 data = (uint8_t)0;
					 Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);

					break;
					default:break;
			
							
				}
			
			break;
			
			case CL_DISINF_STATE_DISINF_OPENFILL:
					
						switch ( cl_disinfevent)
						{
							case EVENT_DISINF_TICK_50MS:
							break;
							case EVENT_DISINF_TICK_100MS:
							break;
							case EVENT_DISINF_TICK_500MS:
							break;
							case EVENT_DISINF_TICK_SECOND:
							sec_count++;
							if(sec_count > 3)
							{
								Cl_disinfOpenFillTimeOut = true;
							}
							//wait for 1 min openfill before intake;
							if(Cl_disinfOpenFillTimeOut)
							{
								sv_prop_start_disinfect_intake();
								cl_disinfstate = CL_DISINF_STATE_DISINF_INTAKE;
								sec_count =0;
							}

							
							break;
							default:break;
						}
			break;
			
			case CL_DISINF_STATE_DISINF_INTAKE:
						switch ( cl_disinfevent)
						{
							case EVENT_DISINF_TICK_50MS:
							break;
							case EVENT_DISINF_TICK_100MS:
							break;
							case EVENT_DISINF_TICK_500MS:
							break;
							case EVENT_DISINF_TICK_SECOND:
							Cl_disinfsecondscounter++;
							Cl_Disinf_SendDisinfStateData();
							if(Cl_disinfsecondscounter == 60)
							{
						
								UpdateDisinfMinuteTick();
						
						
							}
							sec_count ++;
							
							if(sec_count > 20)
							{
								sv_prop_stop_disinfect_intake();
								Cl_disinfretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
								Cl_disinfretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
								Cl_bc_controller(BC_EVENT_STOP);
					
								command = CON_TX_COMMAND_DISINF_STAGE_COMPLETED;
								data = (uint8_t)2;
								Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,1);
								
								cl_wait(5);

								command = CON_TX_COMMAND_DISINF_PAUSED;
								data = (uint8_t)0;
								cl_disinfstate = CL_DISINF_STATE_DISINF_INTAKE_COMPLETED;
								
								Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);
								cl_disinfstate = CL_DISINF_STATE_DISINF_INTAKE_COMPLETED;

							}
				
							break;
							default:break;
						}
			
			break;
			case CL_DISINF_STATE_DISINF_INTAKE_COMPLETED:
			
						switch (cl_disinfevent)
						{
								case EVT_RESUME_DISINF_STAGE:
								
								 Cl_disinfretcode =  sv_cntrl_activatepump(DCMOTOR2);
								Cl_disinfretcode =  sv_cntrl_activatepump(DCMOTOR1);
								//Cl_disinfretcode =  sv_cntrl_activatepump(DCMOTOR2);
								Cl_disinfretcode = sv_cntrl_setpumpspeed(DCMOTOR1,1050);
								Cl_disinfretcode = sv_cntrl_setpumpspeed(DCMOTOR2,1200);
								Cl_bc_controller(BC_EVENT_START);
								sv_cntrl_enable_loopback();
								sec_count =0;
								cl_disinfstate = CL_DISINF_STATE_DISINF_RINSING;
								 
					 
								 //Send resumed
					 
								 command = CON_TX_COMMAND_DISINF_RESUMED;
								 data = (uint8_t)3;
								 Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);

								break;
								default:break;
						}
			
			break;
			case CL_DISINF_STATE_DISINF_RINSING:
						switch ( cl_disinfevent)
						{
							case EVENT_DISINF_TICK_50MS:
							break;
							case EVENT_DISINF_TICK_100MS:
							break;
							case EVENT_DISINF_TICK_500MS:
							break;
							case EVENT_DISINF_TICK_SECOND:
							Cl_disinfsecondscounter++;
							Cl_Disinf_SendDisinfStateData();
							if(Cl_disinfsecondscounter == 60)
							{
								UpdateDisinfMinuteTick();	
							}
							if(Cl_disinfMinutescounter > CL_DISINF_TIMEOUT )
							{
								Cl_Disinf_StopDisinf();
								Cl_disinfretcode =  sv_cntrl_deactivatepump(DCMOTOR2);
								Cl_disinfretcode =  sv_cntrl_deactivatepump(DCMOTOR1);
								Cl_bc_controller(BC_EVENT_STOP);
								command = CON_TX_COMMAND_DISINF_STAGE_COMPLETED;
								data = (uint8_t)4;
								Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,1);
								

							//	command = CON_TX_COMMAND_DISINF_PAUSED;
							//	data = (uint8_t)0;
							//	Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);
							//	cl_disinfstate = CL_DISINF_STATE_DISINF_RINSING_COMPLETED;
							
							
							
							//	command = CON_TX_COMMAND_DISINF_RESUMED;
							//	data = (uint8_t)0;
							//	Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);
								 Cl_disinfretcode = Cl_mac_apprequesthandler(MACREQ_START_POST_DISINF_RINSE);
								 cl_disinfstate = CL_DISINF_STATE_SECOND_RO_RINSE;
					 
							}
							break;
							default:break;
						}
						
				break;
				case CL_DISINF_STATE_DISINF_RINSING_COMPLETED:
						switch ( cl_disinfevent)
						{ 
							case EVT_RESUME_DISINF_STAGE:
							// Cl_Disinf_StartDisinf();
							 Cl_disinfretcode = Cl_mac_apprequesthandler(MACREQ_START_POST_DISINF_RINSE);
							 
							command = CON_TX_COMMAND_DISINF_RESUMED;
							data = (uint8_t)0;
							Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);
							
							 cl_disinfstate = CL_DISINF_STATE_SECOND_RO_RINSE;
					 
							 //Send resumed
					 

							break;
							default:break;
			
							
						}
			
				break;		
				case CL_DISINF_STATE_SECOND_RO_RINSE:
								switch ( cl_disinfevent)
								{
									case EVENT_POST_DISINFECT_RINSE_COMPLETED:
									
									//Send Complete
									 command = CON_TX_COMMAND_DISINF_COMPLETED;
									 data = (uint8_t)0;
									 Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,0);
									  Cl_disinfretcode = Cl_mac_apprequesthandler(MACREQ_DISINF_COMPLETED);
									 cl_disinfstate = CL_DISINF_STATE_POST_DISINF_STANDBY;
									 
									break;
									case EVENT_DISINF_TICK_SECOND:
									Cl_disinfsecondscounter++;
									Cl_Disinf_SendDisinfStateData();
									if(Cl_disinfsecondscounter == 60)
									{
										UpdateDisinfMinuteTick();
									}
									break;
									default:break;
								}
				
				
				break;
				case CL_DISINF_STATE_POST_DISINF_STANDBY:
				
				break;
			break;
			default:break;
			
		}
		

	return CL_OK;
}
    

Cl_ReturnCodes  cl_disinf_translatemacevent(MAC_EVENTS Cl_MacdisinfEvt,Cl_disinf_Eevents* cl_disinf_event)
{
	switch(Cl_MacdisinfEvt)
	{

		
		case  EVT_CONSOLE_COMMAND_DISINF_START:
		
		*cl_disinf_event= EVENT_START_DISINF;
		break;
		case EVT_CONSOLE_RESUME_DISINF_STAGE:
		*cl_disinf_event= EVT_RESUME_DISINF_STAGE;
		break;
		case EVT_CONSOLE_COMMAND_STOP_DISINF:
		*cl_disinf_event= EVENT_STOP_DISINF;
		break;

		case MACREQ_PRE_DISINFECT_RINSE_COMPLETED:
		*cl_disinf_event= EVENT_PRE_DISINFECT_RINSE_COMPLETED;
		break;


		case MACREQ_POST_DISINFECT_RINSE_COMPLETED:
		*cl_disinf_event = EVENT_POST_DISINFECT_RINSE_COMPLETED ;
		break;
		case MACREQ_DISINF_COMPLETED:
		*cl_disinf_event = EVENT_MAC_DISINF_COMPLETED;
		break;
		case MACREQ_BC_OPENFILL_COMPLETED:
		*cl_disinf_event = EVENT_DISINF_BC_OPENFILL_COMPLETED;
		break;

		
		case EVT_TICK_50M:
		*cl_disinf_event =   EVENT_DISINF_TICK_50MS;
		break;
		break;
		case EVT_TICK_100M:
		*cl_disinf_event =   EVENT_DISINF_TICK_100MS;
		break;
		case EVT_TICK_500M:
		*cl_disinf_event =   EVENT_DISINF_TICK_500MS;
		break;
		case EVT_TICK_SEC:
		*cl_disinf_event =   EVENT_DISINF_TICK_SECOND;
		break;
		case EVT_TICK_MIN:
		*cl_disinf_event =   EVENT_DISINF_TICK_MINUTE;
		break;
		case EVT_TICK_HOUR:
		*cl_disinf_event =   EVENT_DISINF_TICK_HOUR;
		break;
		case EVT_ALARM_TRIGGERED:
		*cl_disinf_event =  EVENT_DISINF_ALARM;
		break;
		case  EVT_ALERT_TRIGGERED:
		*cl_disinf_event =  EVENT_DISINF_ALERT;
		break;
		
		case EVT_CONSOLE_COMMAND_SET_DATA:
		*cl_disinf_event = EVT_DISINF_SET_DATA;
		break;
		
		
		
		case EVT_CONSOLE_COMMAND_GET_DATA:
		*cl_disinf_event =  EVT_DISINF_GET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_START_PRIME:
		*cl_disinf_event =  EVT_DISINF_START_PRIME;
		break;
		case EVT_CONSOLE_COMMAND_STOP_PRIME:
		*cl_disinf_event =  EVT_DISINF_STOP_PRIME;
		break;
		case EVT_CONSOLE_COMMAND_RCIRC_START:
		*cl_disinf_event = EVT_DISINF_RCIRC_START;
		break;
		case EVT_CONSOLE_COMMAND_RCIRC_STOP:
		*cl_disinf_event = EVT_DISINF_RCIRC_STOP;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
		*cl_disinf_event = EVT_DISINF_COMMAND_SET_BLDPMP_ON;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
		*cl_disinf_event = EVT_DISINF_COMMAND_SET_BLDPMP_OFF;
		break;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
		*cl_disinf_event = EVT_DISINF_COMMAND_SET_BLDPUMPRATE;
		break;
		
		default:
		break;
	}
	return CL_OK;
}

Cl_ReturnCodes Cl_Disinf_StartDisinf(void)
{
	Cl_ReturnCodes Cl_disinfretcode = CL_OK;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t dataarray[4] = {0,0,0,0};
	uint8_t data = 0;
	
	//Check for  rinse readiness and alarm status
	
	sv_cntrl_deactivate_valve(VALVE_ID1);
	sv_cs_setpotvalue(2200);
	//	Cl_disinfretcode = sv_cntrl_setflowpath(BC_FLUSH_OPEN); // sanjeer BC_NEW
	Cl_disinfretcode =  sv_cntrl_activatepump(DCMOTOR1);
	Cl_disinfretcode = sv_cntrl_setpumpspeed(DCMOTOR1,700);
	
	//sv_prop_startpropeo_aligning();
	//Cl_disinfretcode =  cl_deaeration_controller(CL_DEAER_EVENT_ACTIVATE);
	
	Cl_disinfretcode = Cl_bc_controller(BC_EVENT_START);
	


	// sv_prop_startpropeo_aligning();
	
	if(Cl_disinfretcode == CL_OK)
	{


		Cl_disinfOpenFillTimeOut = false;

		Cl_disinfretcode = Cl_Disinf_UpdateTimeInfo();
		
		Cl_disinfretcode =  cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_START);
		Cl_disinf_ResetAlertsforReassertion();
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(BLOODDOOR_STATUS_OPEN,LOGIC_HIGH,0,0,0);
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(HOLDER1STATUS_OPEN,LOGIC_HIGH,0,0,0);
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(HOLDER2STATUS_OPEN,LOGIC_HIGH,0,0,0);
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(LEVELSWITCH_OFF_TO_ON,LOGIC_LOW,0,0,0);
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(LEVELSWITCH_ON_TO_OFF,LOGIC_HIGH,0,0,0);
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(FLOW_LOW_FLOWRATE,0,0,0,0);
		Cl_disinfretcode =  Cl_AlarmConfigureAlarmType(FLOW_NO_FLOW,0,0,0,0);
		
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(BLOODDOOR_STATUS_OPEN,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(HOLDER1STATUS_OPEN,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(HOLDER2STATUS_OPEN,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(LEVELSWITCH_ON_TO_OFF,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(FLOW_LOW_FLOWRATE,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(FLOW_NO_FLOW,true );
		//	Cl_disinfretcode =  Cl_AlarmActivateAlarms(FLOW_LOW_FLOWRATE,true );
		//	Cl_disinfretcode =  Cl_AlarmActivateAlarms(FLOW_HIGH_FLOWRATE,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(COND_STATUS_HIGH,true );
		//Cl_disinfretcode =  Cl_AlarmActivateAlarms(COND_STATUS_LOW,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(PS2_HIGH_THRESHOLD,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(PS1_HIGH_THRESHOLD,true );
		

		

		
		
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(TEMP3_HIGH_THRESHOLD,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(TEMP3_LOW_THRESHOLD,true );
		Cl_disinfretcode =  Cl_AlarmActivateAlarms(TEMP2_HIGH_THRESHOLD,true );
		
		Cl_disinfretcode =  Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );

		
		Cl_disinfretcode = Cl_disinf_ResetAlertsforReassertion();
		//Cl_disinfretcode = SetHeaterState(CL_HEATER_STATE_START);
		

		
	}
}

Cl_ReturnCodes Cl_Disinf_StopDisinf(void)
{
	Cl_ReturnCodes Cl_disinf_retval = CL_OK;
	sv_cntrl_disable_loopback();
	
	
	return;
}

Cl_ReturnCodes Cl_Disinf_UpdateTimeInfo(void)
{
	
	Cl_ReturnCodes Cl_disinfretcode = CL_ERROR;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data[7] = {0,0,0,0,0 ,0,0};
	
	command = CON_TX_COMMAND_REM_TIME;

	
	data[0]= (uint8_t)DISINF_STATE;
	data[1]= (uint8_t)Cl_disinfTotalMinutescounter;

	
	Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,2);
	
	
	
	return CL_OK;
	
}

Cl_ReturnCodes  Cl_disinf_ProcessAlarms(void)
{
	return CL_ACCEPTED;
}


Cl_ReturnCodes Cl_disinf_ResetAlertsforReassertion(void )
{
	Cl_ReturnCodes 	Cl_disinfretcode = CL_OK;
	CldisinfAlarmIdType CldisinfAlarmId;
	uint8_t data;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_NewAlarmIdType cl_disinfalarmid;
	
	
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_BLOODDOOR_STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(BLOODDOOR_STATUS_OPEN);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_BYPASSDOOR_STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(BYPASSDOOR_STATUS_OPEN);

	Cl_DisinfAlarmTable[CL_DISINF_ALARM_COND_STATUS_LOW].IsRaised = false;
	Cl_AlarmResetAlarm(COND_STATUS_LOW);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_COND_STATUS_HIGH].IsRaised = false;
	Cl_AlarmResetAlarm(COND_STATUS_HIGH);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_COND_DAC_OPEN].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_COND_DAC_RO].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_COND_DAC_HIGH].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_PS1_HIGH_THRESHOLD].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_PS1_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS1_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS1_LOW_THRESHOLD);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_PS2_HIGH_THRESHOLD].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_PS2_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS2_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(PS2_LOW_THRESHOLD);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_PS3_HIGH_THRESHOLD].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_PS3_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(PS3_LOW_THRESHOLD);
	Cl_AlarmResetAlarm(PS3_HIGH_THRESHOLD);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_TEMP2_HIGH_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP2_LOW_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP2_HIGH_THRESHOLD);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_TEMP3_HIGH_THRESHOLD].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_TEMP3_LOW_THRESHOLD].IsRaised = false;
	Cl_AlarmResetAlarm(TEMP2_HIGH_THRESHOLD);
	Cl_AlarmResetAlarm(TEMP2_LOW_THRESHOLD);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_HOLDER1STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(HOLDER1STATUS_OPEN);
	Cl_AlarmResetAlarm(HOLDER1STATUS_CLOSED);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_HOLDER2STATUS_OPEN].IsRaised = false;
	Cl_AlarmResetAlarm(HOLDER2STATUS_OPEN);
	Cl_AlarmResetAlarm(HOLDER2STATUS_CLOSED);
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_FLOW_NO_FLOW].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_FLOW_LOW_FLOWRATE].IsRaised = false;
	Cl_DisinfAlarmTable[CL_DISINF_ALARM_LEVEL_SWITCH_LOW_TOGGLERATE].IsRaised = false;
	
	
	
	return (Cl_disinfretcode);
}

Cl_ReturnCodes Cl_Disinf_SendDisinfStateData(void)
{
	Cl_ReturnCodes  Cl_disinfretcode = CL_OK;
	cl_DlsInfDatatype data;
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
	
	dataarray[0] = DISINF_STATE;
	count++;

	
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
	{
		
		float ftemp,ftemp1;
		data.word = 0;
		ftemp = temp * 0.805;
		ftemp1 = 0.0000116 * ftemp *ftemp + 0.0035 *ftemp + 11.157 + 0.6;
		avgtmp3 =	(avgtmp3*5 + ftemp1)/6;
		data.Twobyte = (uint16_t)(avgtmp3 * 10);
		dataarray[count++] = data.bytearray[0];
		dataarray[count++] = data.bytearray[1];
		dataarray[count++] = data.bytearray[2];
		dataarray[count++] = data.bytearray[3];
	}
	
	
	Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&data.Twobyte);
	//data.twobytedata = ts2;
	
	//temp = (0.805 * data.twobytedata) - 526 ;
	temp = (0.8056 * data.Twobyte) - 1450 ;
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
		
		
		
		data.word = 0;
		data.Twobyte = temp;
		dataarray[count++] = data.bytearray[0];
		dataarray[count++] = data.bytearray[1];
		dataarray[count++] = data.bytearray[2];
		dataarray[count++] = data.bytearray[3];
	}
	
	

	
	Cl_disinfretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_STATE_DATA,&dataarray,count);
	
	
	}
	
	
	Cl_ReturnCodes UpdateDisinfMinuteTick(void)
	{
		Cl_ReturnCodes Cl_disinfretcode = CL_OK;
		Cl_disinfsecondscounter = 0;
		Cl_disinfMinutescounter++;
		Cl_disinfTotalMinutescounter++;
		Cl_disinfretcode = Cl_Disinf_UpdateTimeInfo();
		return Cl_disinfretcode;
	}
	
	
	#if 0

 Cl_ReturnCodes Cl_Disinf_UpdateTimeInfo(void)
 {
	 
	 Cl_ReturnCodes Cl_disinfretcode = CL_ERROR;
	 Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	 uint8_t data[7] = {0,0,0,0,0 ,0,0};
	 
	 command = CON_TX_COMMAND_REM_TIME;

	 
	 data[0]= (uint8_t)DISINF_STATE;
	 data[1]= (uint8_t)Cl_disinfTotalMinutescounter;

	 
	 Cl_disinfretcode = Cl_SendDatatoconsole(command,&data,2);
	 
	 
	 //	Cl_Rinse_SendRinseStateData();                                                          commented on 28.05.2014
	 return CL_OK;
	 
 }


#endif