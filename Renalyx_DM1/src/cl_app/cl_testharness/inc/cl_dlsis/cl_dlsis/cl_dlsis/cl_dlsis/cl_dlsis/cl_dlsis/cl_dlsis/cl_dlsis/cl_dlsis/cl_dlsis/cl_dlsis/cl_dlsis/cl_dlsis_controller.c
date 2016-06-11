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
extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_AlarmIdType,bool );
extern uint8_t sv_cntrl_setflowpath(sv_flowpathtype);

Cl_Dlsis_States cl_dlsis_state = CL_DLSIS_STATE_IDLE;
//Cl_Rinse_States cl_dlsis_state = CL_DLSIS_STATE_IDLE;

 int Cl_dlsissecondscounter =0 ,Cl_dlsisMinutescounter=0, Cl_dlsishourscounter=0;
 int Cl_dlsisTotalMinutescounter=0, Cl_dlsisTotalhourscounter=0;
 bool Cl_dlsisOpenFillTimeOut = false;
 
 extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
 extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
 extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
 extern Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_AlarmIdType  , bool* );

Cl_ReturnCodes Cl_dlsis_init(void);
Cl_ReturnCodes cl_dlsis_translatemacevent(MAC_EVENTS ,Cl_Dlsis_Eevents* );
Cl_ReturnCodes Cl_Dlsis_UpdateTimeInfo(void);
Cl_ReturnCodes Cl_Dlsis_controller(MAC_EVENTS );
Cl_ReturnCodes cl_Dlsis_CheckforDialysisCompletion(void);
Cl_ReturnCodes cl_Dlsis_notifydacandgotorinsestandby(void);
Cl_ReturnCodes Cl_Dlsis_ProcessAlarms( void);

DlsisAlarmsType Cl_DlsisAlarmTable[CL_DLSIS_ALRM_MAX] = 
{
{LEVELSWITCHON,false,false,false},
{LEVELSWITCHOFF,false,false,false},
{HOLDER1STATUS,false,false,false},
{HOLDER2STATUS,false,false,false},
{COND_STATUS,false,false,false},
{HPSTATUS,false,false,false},
{ABDSTATUS,false,false,false},
{BDSTATUS,false,false,false},
{APTSTATUS,false,false,false},
{BPTSTATUS,false,false,false},
{BLDSTATUS,false,false,false},
{PS1STATUS,false,false,false},
{PS2STATUS,false,false,false},
{PS3STATUS,false,false,false},
{PS4STATUS,false,false,false},
{TEMP1STATUS,false,false,false},
{TEMP2STATUS,false,false,false},
{TEMP3STATUS,false,false,false},
{FPCURRENTSTATUS,false,false,false},
{UFPCURRENTSTATUS,false,false,false},
};

 Cl_ReturnCodes Cl_dlsis_init(void)
 {
	 
	 return CL_OK;
 }
 
 
Cl_ReturnCodes Cl_Dlsis_controller(MAC_EVENTS Cl_MacDlsisEvent)
{
	
	Cl_ReturnCodes Cl_dlsisretcode;
	Cl_Dlsis_Eevents cl_dlsis_event = EVENT_NULL_EVENT;
	

	
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint8_t data = 0;
	


Cl_dlsisretcode =  cl_dlsis_translatemacevent( Cl_MacDlsisEvent, &cl_dlsis_event);
switch(cl_dlsis_state)
{
		case CL_DLSIS_STATE_INIT:
		//printf("K\n");
		break;
		case CL_DLSIS_STATE_IDLE:
		//printf("J\n");
		switch (cl_dlsis_event)
			{
				case EVENT_DLSIS_START_DIALYSIS:
				//printf("\n");
					//Cl_dlsisretcode = sv_setflowpath(SV_FLOW_PATH_OPENFILL);
					Cl_dlsisretcode = sv_cntrl_setflowpath(BC_FLUSH_OPEN);
					if(Cl_dlsisretcode == CL_OK)
						{
							cl_wait(10);
								//change state to CL_DLSIS_STATE_OPENFILL
							cl_dlsis_state = CL_DLSIS_STATE_OPENFILL;
				
							command = CON_TX_COMMAND_DLSIS_CNFRM;
							data = (uint8_t)COMMAND_RESULT_SUCCESS;
								//Reset the  OPENFILL time count
							Cl_dlsisMinutescounter = 0;
							Cl_dlsisTotalMinutescounter = 0;
							Cl_dlsisOpenFillTimeOut = false;
							Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,1);
				
			//				command = CON_TX_COMMAND_DLSIS_STARTED;
				//			data = (uint8_t)COMMAND_RESULT_SUCCESS;
				//			Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,1);
				
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(LEVELSWITCHON,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(LEVELSWITCHOFF ,true);
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( HOLDER1STATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( HOLDER2STATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(COND_STATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(HPSTATUS ,true);
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( ABDSTATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( BDSTATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(APTSTATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(BPTSTATUS ,true);
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( BLDSTATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS2STATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(PS3STATUS ,true);
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( PS4STATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(TEMP3STATUS,true );
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms(FPCURRENTSTATUS ,true);
							Cl_dlsisretcode =  Cl_AlarmActivateAlarms( UFPCURRENTSTATUS,true );				

				
						}
			
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				break;
				case EVENT_DLSIS_TIMEOUT:
				break;
				case EVENT_DLSIS_ALARM:
				Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				break;
				default:
				break;
			}	
		break;

		case CL_DLSIS_STATE_OPENFILL:
		
			switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_TICK_SECOND:
				Cl_dlsissecondscounter++;
				Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
						//	command = CON_TX_COMMAND_SYSINFO;
						//	data = (uint8_t)COMMAND_RESULT_SUCCESS;
						//	Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,1);
				// get system info
				// update console system info
				break;
				case EVENT_DLSIS_TICK_MINUTE:
				if(cl_Dlsis_CheckforDialysisCompletion() != CL_OK )
				{
				
					if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
					Cl_dlsissecondscounter = 0;
					Cl_dlsisMinutescounter++;
					Cl_dlsisTotalMinutescounter++;
					// check for sub state time out and transition
				
						if( Cl_dlsisMinutescounter > CL_DLSIS_OPENFILL_TIMEOUT)
						{
							Cl_dlsisMinutescounter = 0;
							// Filling over - change state and set new flow path
							//Cl_dlsisretcode = (Cl_ReturnCodes)sv_setflowpath(SV_FLOW_PATH_V2OPEN);
							//Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(SV_FLOW_PATH_V2OPEN);
							Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V4);
								if(Cl_dlsisretcode == CL_OK)
								{
									cl_dlsis_state = CL_DLSIS_STATE_V2OPEN;
									//printf("V2V2\n");
								}
						
						}
				}
				else
				{
					cl_dlsis_state = CL_DLSIS_STATE_POST_RINSE_STANDBY;
					//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
										
				}
				break;
				case EVENT_DLSIS_TICK_HOUR:
				
				Cl_dlsishourscounter++;
				Cl_dlsisTotalhourscounter++;
				//check for rinse process time out
				//Cl_dlsisretcode = cl_rinse_processtimeoutevets(cl_dlsis_event);
				
				
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				Cl_dlsissecondscounter = 0;
				Cl_dlsisMinutescounter = 0;
				Cl_dlsisTotalMinutescounter = 0;
				Cl_dlsisTotalhourscounter = 0 ;
				//
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
						//printf("RTO\n");
				Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				break;
				default:
				break;
			}
		break;
		case CL_DLSIS_STATE_V2OPEN:
			switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_TICK_SECOND:
				Cl_dlsissecondscounter++;
							Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
						//	command = CON_TX_COMMAND_SYSINFO;
						//	data = (uint8_t)COMMAND_RESULT_SUCCESS;
						//	Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,1);
				// get system info
				// update console system info
				break;
				case EVENT_DLSIS_TICK_MINUTE:
				if(cl_Dlsis_CheckforDialysisCompletion() != CL_OK )
				{
				
					if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
					Cl_dlsissecondscounter = 0;
					Cl_dlsisMinutescounter++;
					Cl_dlsisTotalMinutescounter++;
					// check for sub state time out and transition
				
					if( Cl_dlsisMinutescounter > CL_DLSIS_STATE_V2OPEN_TIMEOUT)
					{
						Cl_dlsisMinutescounter = 0;
						// Filling over - change state and set new flow path
						//Cl_dlsisretcode = (Cl_ReturnCodes)sv_setflowpath(SV_FLOW_PATH_BO1_V4);
						Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V4);
						
							if(Cl_dlsisretcode == CL_OK)
							{
								cl_dlsis_state = CL_DLSIS_STATE_BO1_V4;
								//printf("BO1_V4\n");
							}
						
					}
				}
				else
				{
					cl_dlsis_state = CL_DLSIS_STATE_POST_RINSE_STANDBY;
					//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
										
				}
				break;
				case EVENT_DLSIS_TICK_HOUR:
				
				Cl_dlsishourscounter++;
				Cl_dlsisTotalhourscounter++;
				//check for rinse process time out
				//Cl_dlsisretcode = cl_rinse_processtimeoutevets(cl_dlsis_event);
				
				
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				Cl_dlsissecondscounter = 0;
				Cl_dlsisMinutescounter = 0;
				Cl_dlsisTotalMinutescounter = 0;
				Cl_dlsisTotalhourscounter = 0 ;
				//
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
				//printf("RTO\n");
				Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				break;
				default:
				break;
			}
		break;
		case CL_DLSIS_STATE_BO1_V4:
		switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_TICK_SECOND:
				Cl_dlsissecondscounter++;
				Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
				// get system info
				// update console system info
				break;
				case EVENT_DLSIS_TICK_MINUTE:
				if(cl_Dlsis_CheckforDialysisCompletion() != CL_OK )
				{
					
					if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
					Cl_dlsissecondscounter = 0;
					Cl_dlsisMinutescounter++;
					Cl_dlsisTotalMinutescounter++;
					// check for sub state time out and transition
				
						if( Cl_dlsisMinutescounter > CL_DLSIS_PATH_BO1_V4_TIMEOUT)
						{
							Cl_dlsisMinutescounter = 0;
							// Filling over - change state and set new flow path
							//Cl_dlsisretcode = (Cl_ReturnCodes)sv_setflowpath(SV_FLOW_PATH_BO2_V4);
							Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V4);
								if(Cl_dlsisretcode == CL_OK)
								{
									cl_dlsis_state = CL_DLSIS_STATE_BO2_V4;
									//printf("BO2_V4\n");
								}
						
						}
				}
				else
				{
					cl_dlsis_state = CL_DLSIS_STATE_POST_RINSE_STANDBY;
					//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
				
				}
				break;
				case EVENT_DLSIS_TICK_HOUR:
				
				Cl_dlsishourscounter++;
				Cl_dlsisTotalhourscounter++;
				//check for rinse process time out
				//Cl_dlsisretcode = cl_rinse_processtimeoutevets(cl_dlsis_event);
				
				
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				Cl_dlsissecondscounter = 0;
				Cl_dlsisMinutescounter = 0;
				Cl_dlsisTotalMinutescounter = 0;
				Cl_dlsisTotalhourscounter = 0 ;
				//
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
				//printf("RTO\n");
				Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				break;
				default:
				break;
			}
		break;
		case CL_DLSIS_STATE_BO2_V4:
		switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_TICK_SECOND:
				Cl_dlsissecondscounter++;
				Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
				// get system info
				// update console system info
				break;
				case EVENT_DLSIS_TICK_MINUTE:
				if(cl_Dlsis_CheckforDialysisCompletion() != CL_OK )
				{
					
							if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
							Cl_dlsissecondscounter = 0;
							Cl_dlsisMinutescounter++;
							Cl_dlsisTotalMinutescounter++;
				
						if( Cl_dlsisMinutescounter > CL_DLSIS_PATH_BO2_V4_TIMEOUT)
						{
							Cl_dlsisMinutescounter = 0;
							// Filling over - change state and set new flow path
							//Cl_dlsisretcode = (Cl_ReturnCodes)sv_setflowpath(SV_FLOW_PATH_BO1_V13V14);
							Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_1_V13_14);
							
								if(Cl_dlsisretcode == CL_OK)
								{
									cl_dlsis_state = CL_DLSIS_STATE_BO1_V13V14;
									//printf("BO1_V13V14\n");
								}
						
						}
				}
				else
				{
					cl_dlsis_state = CL_DLSIS_STATE_POST_RINSE_STANDBY;
					//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
				
				}
				break;
				case EVENT_DLSIS_TICK_HOUR:
				
				Cl_dlsishourscounter++;
				Cl_dlsisTotalhourscounter++;
				//check for rinse process time out
				//Cl_dlsisretcode = cl_rinse_processtimeoutevets(cl_dlsis_event);
				
				
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				Cl_dlsissecondscounter = 0;
				Cl_dlsisMinutescounter = 0;
				Cl_dlsisTotalMinutescounter = 0;
				Cl_dlsisTotalhourscounter = 0 ;
				//
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
				Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				//printf("RTO\n");
				break;
				default:
				break;
			}
		break;
		case CL_DLSIS_STATE_BO1_V13V14:
		switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_TICK_SECOND:
				Cl_dlsissecondscounter++;
				Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
				// get system info
				// update console system info
				break;
				case EVENT_DLSIS_TICK_MINUTE:
				if(cl_Dlsis_CheckforDialysisCompletion() != CL_OK )
				{
					
							if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
							Cl_dlsissecondscounter = 0;
							Cl_dlsisMinutescounter++;
							Cl_dlsisTotalMinutescounter++;
				
						if( Cl_dlsisMinutescounter > CL_DLSIS_PATH_BO1_V13V14_TIMEOUT )
						{
							Cl_dlsisMinutescounter = 0;
							// Filling over - change state and set new flow path
							//Cl_dlsisretcode = (Cl_ReturnCodes)sv_setflowpath(SV_FLOW_PATH_BO2_V13V14);
							Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V13_14);
								if(Cl_dlsisretcode == CL_OK)
								{
									cl_dlsis_state = CL_DLSIS_STATE_BO2_V13V14;
									//printf("BO2_V13V14\n");
								}
						
						}
				}
				else
				{
					cl_dlsis_state = CL_DLSIS_STATE_POST_RINSE_STANDBY;
					//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
				
				}
				break;
				case EVENT_DLSIS_TICK_HOUR:
				
				Cl_dlsishourscounter++;
				Cl_dlsisTotalhourscounter++;
				//check for rinse process time out
				//Cl_dlsisretcode = cl_rinse_processtimeoutevets(cl_dlsis_event);
				
				
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				Cl_dlsissecondscounter = 0;
				Cl_dlsisMinutescounter = 0;
				Cl_dlsisTotalMinutescounter = 0;
				Cl_dlsisTotalhourscounter = 0 ;
				//
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
					//printf("RTO\n");
				Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				break;
				default:
				break;
			}
		break;
		case CL_DLSIS_STATE_BO2_V13V14:
		switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_TICK_SECOND:
				Cl_dlsissecondscounter++;
				Cl_dlsisretcode = Cl_Dlsis_UpdateTimeInfo();
				// get system info
				// update console system info
				break;
				case EVENT_DLSIS_TICK_MINUTE:
				if(cl_Dlsis_CheckforDialysisCompletion() != CL_OK )
				{
					
							if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
							Cl_dlsissecondscounter = 0;
							Cl_dlsisMinutescounter++;
							Cl_dlsisTotalMinutescounter++;
						// check for sub state time out and transition
				
						if( Cl_dlsisMinutescounter > CL_DLSIS_PATH_BO2_V13V14_TIMEOUT)
						{
							Cl_dlsisMinutescounter = 0;
							// Filling over - change state and set new flow path
							//Cl_dlsisretcode = (Cl_ReturnCodes)sv_setflowpath(SV_FLOW_PATH_V2OPEN);
							Cl_dlsisretcode = (Cl_ReturnCodes)sv_cntrl_setflowpath(BC_FLUSH_2_V4);
								if(Cl_dlsisretcode == CL_OK)
								{
									cl_dlsis_state = CL_DLSIS_STATE_V2OPEN;
										//printf("V2V2\n");
								}
						
						}
				
				}
				else
				{
					cl_dlsis_state = CL_DLSIS_STATE_POST_RINSE_STANDBY;
					//	Cl_rinseretcode = (Cl_ReturnCodes)sv_setflowpath(Default path ?);//set appropirate flow path configuration
					Cl_dlsisretcode = cl_Dlsis_notifydacandgotorinsestandby();
				
				}
				break;
				case EVENT_DLSIS_TICK_HOUR:
				
				Cl_dlsishourscounter++;
				Cl_dlsisTotalhourscounter++;
				//check for rinse process time out
				//Cl_dlsisretcode = cl_rinse_processtimeoutevets(cl_dlsis_event);
				
				
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				Cl_dlsissecondscounter = 0;
				Cl_dlsisMinutescounter = 0;
				Cl_dlsisTotalMinutescounter = 0;
				Cl_dlsisTotalhourscounter = 0 ;
				//
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
					//printf("RTO\n");
					Cl_dlsisretcode = Cl_Dlsis_ProcessAlarms();
				break;
				default:
				break;
			}
		break;
	default:
			switch(cl_dlsis_event)
			{
				case EVENT_DLSIS_START_DIALYSIS:
				break;
				case EVENT_DLSIS_STOP_DIALYSIS:
				break;
				case EVENT_DLSIS_TIMEOUT:
				//printf("RTO\n");
				break;
				case EVENT_DLSIS_ALARM:
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
		case  EVT_CONSOLE_COMMAND_DIALYSIS_START:
		*cl_dlsis_event= EVENT_DLSIS_START_DIALYSIS;
		break;
		case  EVT_TIMER_EXPIRED:
		
		//if(Cl_RinseTotalTime >  )
		switch(cl_dlsis_state)
		{
			case CL_DLSIS_STATE_OPENFILL:
			*cl_dlsis_event = EVENT_DLSIS_OPENFILL_TIMEOUT;
			break;
			case CL_DLSIS_STATE_V2OPEN:
			*cl_dlsis_event =  EVENT_DLSIS_V2OPEN_TIMEOUT;
			break;
			case	CL_DLSIS_STATE_BO1_V4:
			*cl_dlsis_event =  EVENT_DLSIS_BO1_V4_TIMEOUT;
			break;
			case CL_DLSIS_STATE_BO2_V4:
			*cl_dlsis_event =  EVENT_DLSIS_BO2_V4_TIMEOUT;
			break;
			case CL_DLSIS_STATE_BO1_V13V14:
			*cl_dlsis_event = EVENT_DLSIS_BO1_V13V14_TIMEOUT ;
			break;
			case CL_DLSIS_STATE_BO2_V13V14:
			*cl_dlsis_event =  EVENT_DLSIS_BO2_V13V14_TIMEOUT ;
			break;

			default:
			break;
		}
		
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
	data[0] = (uint8_t) DIALYSIS;
	data[1]= (uint8_t)Cl_dlsisTotalMinutescounter;
	data[2]= (uint8_t)Cl_dlsisTotalhourscounter;
	data[3]= (uint8_t)Cl_dlsissecondscounter;
	data[4]= (uint8_t) (CL_DLSIS_TIMEOUT_MIN - Cl_dlsisTotalMinutescounter );
	data[5]= (uint8_t) (CL_DLSIS_TIMEOUT_HRS - Cl_dlsisTotalhourscounter );
	data[6]= (uint8_t) (60 - Cl_dlsissecondscounter );
	
	Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,7);
	
	return CL_OK;
	}
	
	 Cl_ReturnCodes cl_Dlsis_CheckforDialysisCompletion(void)
	 {
		 Cl_ReturnCodes Cl_dlsisretcode = CL_ERROR;
		 
		 if(Cl_dlsisTotalMinutescounter > CL_DLSIS_TIMEOUT_MIN )
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
		Cl_dlsissecondscounter = 0;
		Cl_dlsisMinutescounter= 0;
		Cl_dlsishourscounter= 0;
		Cl_dlsisTotalMinutescounter= 0;
		Cl_dlsisTotalhourscounter=0;
		
		
		Cl_dlsisretcode = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYS_COMPLTED,&data,0);
		Cl_dlsisretcode = Cl_mac_apprequesthandler(MACREQ_DIALYSIS_COMPLETED);
		
		return Cl_dlsisretcode;
	}
	
	
Cl_ReturnCodes Cl_Dlsis_UpdateAlarmTable(ClDlsisAlarmIdType * ClRinseAlarmId )
{
	Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
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
					data[0] = (uint8_t)Cl_DlsisAlarmTable[tempcount].Cl_DlsisAlarmId;
					data[1] = 1;
					command = CON_TX_COMMAND_ALARM;
					Cl_dlsisretcode = Cl_SendDatatoconsole(command,&data,0);
					Cl_DlsisAlarmTable[tempcount].IsRaised = true;
				
				}
			
		}

	}

return (Cl_dlsisretcode );
}

Cl_ReturnCodes Cl_Dlsis_ProcessAlarms( )
{
		Cl_ReturnCodes 	Cl_dlsisretcode = CL_OK;
		ClDlsisAlarmIdType  ClRinseAlarmId;
		uint8_t data;
		Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
		Cl_AlarmIdType cl_rinsealarmid;
		
	   Cl_dlsisretcode = Cl_Dlsis_UpdateAlarmTable(&ClRinseAlarmId);
	//   	Cl_rinseretcode =	Cl_Alarm_GetLastAlarm(&cl_rinsealarmid);
	//   	data = (uint8_t)cl_rinsealarmid;
	//   	command = CON_TX_COMMAND_ALARM;
	//   	Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,0);
		   
		   return (Cl_dlsisretcode );
	   	
}
