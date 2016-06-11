

#ifdef WIN32
#include<stdio.h>
#endif

#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "inc/cl_standby_controller.h"
#include "cl_app/cl_utils/inc/cl_versions.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "sv_stubs/inc/sv_stub.h"
#include "sv_stubs/inc/sv_types.h"
#include "Platform/Service/sv_interface.h"


extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
extern Cl_ReturnCodes   Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes Cl_AlarmActivateAlarms(Cl_AlarmIdType , bool);
extern Cl_ReturnCodes sv_nvmgetdata(uint8_t , uint8_t* );
extern Cl_ReturnCodes  sv_nvmsetdata(uint8_t ,uint8_t*,uint8_t datasize );
extern Cl_ReturnCodes Cl_Alarm_GetAlarmStatus(Cl_AlarmIdType  , bool* );
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
extern uint8_t sv_cs_setpotvalue(uint32_t resistance) ;
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype ); 



Cl_ReturnCodes Cl_Standby_Controller(MAC_EVENTS );
Cl_ReturnCodes Cl_stby_init(void);
Cl_ReturnCodes  cl_stby_translatemacevent(MAC_EVENTS ,Cl_Stby_Events* );
Cl_ReturnCodes Cl_StabyRinse_UpdateTimeInfo(void);
Cl_ReturnCodes Cl_standby_UpdateAlarmTable(ClStbyAlarmIdType*  );
Cl_ReturnCodes Cl_standby_ProcessAlarms(void );

StandbyAlarmsType Cl_StandbyAlarmTable[CL_STBY_ALRM_MAX] = {{LEVELSWITCH_OFF_TO_ON,false,false,false},{LEVELSWITCH_ON_TO_OFF,false,false,false},{HOLDER1STATUS_OPEN,false,false,false},{HOLDER2STATUS_OPEN,false,false,false}};
	
Cl_Stby_States cl_stbystate = CL_STBY_STATE_IDLE;

 int Cl_StbRinsesecondscounter =0 ,Cl_StbRinseMinutescounter=0, Cl_StbRinseshourscounter=0;
 int Cl_StbRinseTotalMinutescounter=0, Cl_StbRinseTotalhourscounter=0;
 bool Cl_StbRinseOpenFillTimeOut = false;

Cl_ReturnCodes Cl_Standby_Controller(MAC_EVENTS Cl_MacStandbyEvent)
{
		Cl_ReturnCodes cl_stby_retval;
		Cl_Stby_Events cl_stdby_event;
		Cl_ConsoleTxCommandtype con_command = CON_TX_COMMAND_COMMAND_MAX;
		uint8_t data =0;
		uint8_t dataarray[4];
		ClStbyAlarmIdType ClstbyAlarmId;
		
		// dummy = Cl_MacStandbyEvent;

	//	cl_stby_translatemacevent( dummy, &cl_stdby_event);
		cl_stby_retval = cl_stby_translatemacevent( Cl_MacStandbyEvent, &cl_stdby_event);
		switch(cl_stbystate)
		{
			case CL_STBY_STATE_INIT:
		//	printf("K\n");
			break;
			case CL_STBY_STATE_IDLE:
				switch (cl_stdby_event)
				{
				case EVENT_STBY_ENTER_STANDBY:
				
			//	cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON , true );
			//	cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH2 , true );
												
				con_command = CON_TX_COMMAND_SYSDATA;
				//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
				dataarray[0] = CON_TX_PARAM_SYS_SOFT_VERSION;
			//	cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &dataarray[1]);
			//	cl_stby_retval = cl_testgetversion(&dataarray[1]);								
				//Reset the  OPENFILL time count
				
				dataarray[1] = SYS_VER;
				cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
				
				cl_stbystate = CL_STBY_STATE_V1;
				break;
				default:
				break;	
				}


				break;
			break;
			case CL_STBY_STATE_STANDBY_RINSE:
			//printf("J\n");
			switch (cl_stdby_event)
			{
				case EVENT_STBY_START_RINSE:
				//check for alarms and then notify if needed
					cl_stbystate = CL_STBY_STATE_IDLE;
				break;
				case EVENT_STBY_DIALYSIS_PREP:
				// check for alarms and change hardware status as required before going to dialysis prepration
				break;
				case EVENT_STBY_GET_DATA:
				if(Cl_ConsoleRxMsg.msgready == true)
				{
					
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case CON_RX_PARAM_DATA_RINSE_STATUS:
							//	cl_wait(10);
													
								con_command = CON_TX_COMMAND_SYSDATA;
								//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
								dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
								cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
													
								//Reset the  OPENFILL time count
								cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,1);
								//cl_wait(100);
						break;
						default:
						break;
					}

				}
				break;
				
				case EVENT_STBY_SET_DATA:
				if(Cl_ConsoleRxMsg.msgready == true)
				{
					
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case CON_RX_PARAM_DATA_RINSE_STATUS:
						//		cl_wait(10);
													
								con_command = CON_TX_COMMAND_SYSDATA;
								//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
								dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
								//cl_stby_retval =sv_nvmsetdata(Cl_ConsoleRxMsg.data.byte[0],&Cl_ConsoleRxMsg.data.byte[1]);
								cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
													
								//Reset the  OPENFILL time count
								cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
								//cl_wait(100);
						break;
						default:
						break;
					}
							
				}
				break;
				
				case EVENT_STBY_ENTER_STANDBY:
				
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON , true );
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH2 , true );
				
				cl_stbystate = CL_STBY_STATE_V1;

				break;
				case EVENT_STBY_ALARM:
				//printf("\n");
				//sv_setflowpath(V1);
				//Activate V1 time count
				//change state to V1
				// send update to console
				//Cl_SendDatatoconsole();
				//command = CON_TX_HELLO;
				con_command = CON_TX_COMMAND_ALARM;
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON , false );
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH2 , false );
				cl_stby_retval = Cl_SendDatatoconsole(con_command,&data,0);

				break;
				case	EVENT_STBY_TICK_SECOND:
				Cl_StbRinsesecondscounter++;
				cl_stby_retval = Cl_StabyRinse_UpdateTimeInfo();
				break;
				case EVENT_STBY_TICK_MINUTES:
				Cl_StbRinsesecondscounter =0;
				cl_stbystate = CL_STBY_STATE_V1;
			//	printf("M");
				break;
				case EVENT_STBY_TICK_HOURS:
			//	printf("H");
				break;

				
				default:
				break;
			}
			break;

			case CL_STBY_STATE_V1:
			
			switch (cl_stdby_event)
			{
				case EVENT_STBY_DIALYSIS_PREP:
				// check for alarms and change hardware status as required before going to dialysis prepration
				break;
				case EVENT_STBY_GET_DATA:
				
								if(Cl_ConsoleRxMsg.msgready == true)
								{
									
									switch(Cl_ConsoleRxMsg.data.byte[0])
									{
										case CON_RX_PARAM_DATA_RINSE_STATUS:
									//	cl_wait(10);
										
										con_command = CON_TX_COMMAND_SYSDATA;
										//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
										dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
										cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
										
										//Reset the  OPENFILL time count
										cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,(uint8_t)2);
									//	cl_wait(100);
										break;
										default:
										break;
									}
								}

				break;
				case EVENT_STBY_SET_DATA:
				if(Cl_ConsoleRxMsg.msgready == true)
				{
					
					switch(Cl_ConsoleRxMsg.data.byte[0])
					{
						case CON_RX_PARAM_DATA_RINSE_STATUS:
					//	cl_wait(10);
						
						con_command = CON_TX_COMMAND_SYSDATA;
						//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
						dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
					//	cl_stby_retval =sv_nvmsetdata(Cl_ConsoleRxMsg.data.byte[0],&Cl_ConsoleRxMsg.data.byte[1]);
						cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
						
						//Reset the  OPENFILL time count
						cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
						//cl_wait(10);
						break;
						default:
						break;
					}
					
				}
				break;
				case EVENT_STBY_ALARM:
				//printf("\n");
				//sv_setflowpath(V1);
				//Activate V1 time count
				//change state to V1
				// send update to console
				//Cl_SendDatatoconsole();
				//command = CON_TX_HELLO;
				con_command = CON_TX_COMMAND_ALARM;
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON , false );
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH2 , false );
				//cl_stby_retval = Cl_SendDatatoconsole(con_command,&data,0);
				cl_stby_retval = Cl_standby_ProcessAlarms();

				break;
				case	EVENT_STBY_TICK_SECOND:
				Cl_StbRinsesecondscounter++;
				//cl_stby_retval = Cl_StabyRinse_UpdateTimeInfo();
				//cl_stby_retval = Cl_standby_ProcessAlarms();
				cl_stby_retval = Cl_standby_UpdateAlarmTable(&ClstbyAlarmId );
				
				
				break;
				case EVENT_STBY_TICK_MINUTES:
				Cl_StbRinsesecondscounter =0;
			//	printf("M");
				break;
				case EVENT_STBY_TICK_HOURS:
			//	printf("H");
				break;

				
				default:
				break;
			}
			break;
			case CL_STBY_STATE_V2:
			switch (cl_stdby_event)
			{
				case EVENT_STBY_DIALYSIS_PREP:
				// check for alarms and change hardware status as required before going to dialysis prepration
				break;
				case EVENT_STBY_GET_DATA:
								if(Cl_ConsoleRxMsg.msgready == true)
								{
									
									switch(Cl_ConsoleRxMsg.data.byte[0])
									{
										case CON_RX_PARAM_DATA_RINSE_STATUS:
										//	cl_wait(10);
										
										con_command = CON_TX_COMMAND_SYSDATA;
										//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
										dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
										cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
										
										//Reset the  OPENFILL time count
										cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
										//cl_wait(10);
										break;
										default:
										break;
									}
								}
				break;
				case EVENT_STBY_ALARM:
				//printf("\n");
				//sv_setflowpath(V1);
				//Activate V1 time count
				//change state to V1
				// send update to console
				//Cl_SendDatatoconsole();
				//command = CON_TX_HELLO;
				con_command = CON_TX_COMMAND_ALARM;
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH_OFF_TO_ON , false );
				//cl_stby_retval =  Cl_AlarmActivateAlarms(LEVELSWITCH2 , false );
				cl_stby_retval = Cl_SendDatatoconsole(con_command,&data,0);

				break;
				case	EVENT_STBY_TICK_SECOND:
				Cl_StbRinsesecondscounter++;
				cl_stby_retval = Cl_StabyRinse_UpdateTimeInfo();
				break;
				case EVENT_STBY_TICK_MINUTES:
				Cl_StbRinsesecondscounter =0;
			//	printf("M");
				break;
				case EVENT_STBY_TICK_HOURS:
			//	printf("H");
				break;

				
				default:
				break;
			}
			break;
			case CL_STBY_STATE_BO1_V4:
						switch (cl_stdby_event)
						{
							case EVENT_STBY_GET_DATA:
										if(Cl_ConsoleRxMsg.msgready == true)
										{
											
											switch(Cl_ConsoleRxMsg.data.byte[0])
											{
												case CON_RX_PARAM_DATA_RINSE_STATUS:
												//	cl_wait(10);
												
												con_command = CON_TX_COMMAND_SYSDATA;
												//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
												dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
												cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
												
												//Reset the  OPENFILL time count
												cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
												//cl_wait(10);
												break;
												default:
												break;
											}
										}
							break;

							case EVENT_STBY_DIALYSIS_PREP:
							// check for alarms and change hardware status as required before going to dialysis prepration
							break;
							default:
							break;
						}
			break;
			case CL_STBY_STATE_BO2_V4:
						switch (cl_stdby_event)
						{
							case EVENT_STBY_GET_DATA:
								if(Cl_ConsoleRxMsg.msgready == true)
								{
									
									switch(Cl_ConsoleRxMsg.data.byte[0])
									{
										case CON_RX_PARAM_DATA_RINSE_STATUS:
										//	cl_wait(10);
										
										con_command = CON_TX_COMMAND_SYSDATA;
										//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
										dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
										cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
										
										//Reset the  OPENFILL time count
										cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
										//cl_wait(10);
										break;
										default:
										break;
									}
								}
							break;

							case EVENT_STBY_DIALYSIS_PREP:
							// check for alarms and change hardware status as required before going to dialysis prepration
							break;
							default:
							break;
						}
			case CL_STBY_STATE_BO1_V13V14:
						switch (cl_stdby_event)
						{
							case EVENT_STBY_GET_DATA:
								if(Cl_ConsoleRxMsg.msgready == true)
								{
									
									switch(Cl_ConsoleRxMsg.data.byte[0])
									{
										case CON_RX_PARAM_DATA_RINSE_STATUS:
										//	cl_wait(10);
										
										con_command = CON_TX_COMMAND_SYSDATA;
										//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
										dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
										cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
										
										//Reset the  OPENFILL time count
										cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
										//cl_wait(10);
										break;
										default:
										break;
									}
								}
							break;

							case EVENT_STBY_DIALYSIS_PREP:
							// check for alarms and change hardware status as required before going to dialysis prepration
							break;
							default:
							break;
						}
			case CL_STBY_STATE_BO2_V13V14:
						switch (cl_stdby_event)
						{
							case EVENT_STBY_GET_DATA:
								if(Cl_ConsoleRxMsg.msgready == true)
								{
									
									switch(Cl_ConsoleRxMsg.data.byte[0])
									{
										case CON_RX_PARAM_DATA_RINSE_STATUS:
										//	cl_wait(10);
										
										con_command = CON_TX_COMMAND_SYSDATA;
										//cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_RINSE_NVM_RINSE_STATUS, &data);
										dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
										cl_stby_retval = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
										
										//Reset the  OPENFILL time count
										cl_stby_retval = Cl_SendDatatoconsole(con_command,&dataarray,2);
										//cl_wait(10);
										break;
										default:
										break;
									}
								}
							break;

							case EVENT_STBY_DIALYSIS_PREP:
							// check for alarms and change hardware status as required before going to dialysis prepration
							break;
							default:
							break;
						}
			break;
			case CL_STBY_STATE_PAUSE:
			break;
			case CL_STBY_STATE_STOP:
			break;

			default:
			break;
		}

	
return CL_OK;
}



Cl_ReturnCodes  cl_stby_translatemacevent(MAC_EVENTS Cl_MacStbyEvent,Cl_Stby_Events* cl_stdby_event)
{
	//dummy1  =  Cl_MacStbyEvent;
	
	//switch(dummy1)
	
		*cl_stdby_event =	EVENT_STBY_NULL;
		
	switch(Cl_MacStbyEvent)
	{
		case  EVT_COMMAND_ENTER_STANDBY:
		*cl_stdby_event = EVENT_STBY_ENTER_STANDBY;
		break;
		case EVT_TICK_100M:
		*cl_stdby_event = EVENT_STBY_TICK_100MS;
		break;
		case EVT_TICK_SEC:
		*cl_stdby_event = EVENT_STBY_TICK_SECOND;
		break;
		case EVT_TICK_MIN:
		*cl_stdby_event = EVENT_STBY_TICK_MINUTES;
		break;
		case EVT_TICK_HOUR:
		break;
		case  EVT_ALARM_TRIGGERED:
		*cl_stdby_event = EVENT_STBY_ALARM;
		break;
		case EVT_CONSOLE_COMMAND_GET_DATA:
		*cl_stdby_event = EVENT_STBY_GET_DATA;
		break;
		case  EVT_CONSOLE_COMMAND_RINSE_START:
		*cl_stdby_event= EVENT_STBY_START_RINSE;
		break;
		case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
		*cl_stdby_event = EVENT_STBY_DIALYSIS_PREP;
		break; 
		default:
		break;
	}
	return CL_OK;
}

 Cl_ReturnCodes Cl_stby_init(void)
 {
	 uint16_t temp;
	 	 Cl_ReturnCodes cl_stby_retval = CL_ERROR;
		  
		 
		  	 for(temp = 1;temp<=20; temp++)
		 {
				sv_cntrl_deactivate_valve((sv_valvetype)temp);
				cl_wait(10);
		 }
		  
	 	cl_stby_retval =  sv_cntrl_deactivatepump(DCMOTOR1);
	 	cl_stby_retval =  sv_cntrl_deactivatepump(DCMOTOR2);
	 //	cl_stby_retval =  sv_cntrl_deactivatepump(BLOODPUMP);
	 	cl_stby_retval =  sv_cntrl_deactivatepump(UFPUMP);
	 	cl_stby_retval =  sv_cntrl_deactivatepump(HEPARINPUMP);
		cl_stby_retval =  sv_cntrl_deactivatepump(VLA_PUMP);
		cl_stby_retval =  sv_cntrl_poweroffheater();

		
	
		//	cl_stby_retval =  sv_cs_setpotvalue((800 * 1024)/10000);	
		
		

				
	 return CL_OK;
 }
 
 Cl_ReturnCodes Cl_StabyRinse_UpdateTimeInfo(void)
 {
	 
	 Cl_ReturnCodes cl_stby_retval = CL_ERROR;
	 Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	 uint8_t data[7] = {0,0,0,0,0 ,0,0};
	 
	command = CON_TX_COMMAND_REM_TIME;
	
	data[0]= (uint8_t)STANDBY_DATA;
	data[1]= (uint8_t)Cl_StbRinseTotalhourscounter; 
	data[2]= (uint8_t)Cl_StbRinseTotalMinutescounter;
	data[3]= (uint8_t)Cl_StbRinsesecondscounter;
	data[4]= (uint8_t) (CL_STBYRINSE_TIMEOUT_HRS - Cl_StbRinseTotalhourscounter );
	data[5]= (uint8_t) (CL_STBYRINSE_TIMEOUT_MIN - Cl_StbRinseTotalMinutescounter );
	data[6]= (uint8_t) (60 - Cl_StbRinsesecondscounter );
	
	cl_stby_retval = Cl_SendDatatoconsole(command,&data,7);
	//Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,1);
	
	return CL_OK;
	
 }
 
 Cl_ReturnCodes Cl_standby_ProcessAlarms( void)
{

	Cl_ReturnCodes 	cl_stby_retval = CL_OK;
	uint8_t tempcount = 0;
	uint8_t data[2];
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	//cl_wait(10);
	
	for (tempcount = 0 ; tempcount < CL_STBY_ALRM_MAX ; tempcount++)
	{
		
		Cl_Alarm_GetAlarmStatus(Cl_StandbyAlarmTable[tempcount].Cl_RinseAlarmId,&Cl_StandbyAlarmTable[tempcount].IsActive);
		if (Cl_StandbyAlarmTable[tempcount].IsActive)
		{
			if(!Cl_StandbyAlarmTable[tempcount].IsRaised)
				{
					

				//	data[0] = (uint8_t)Cl_StandbyAlarmTable[tempcount].Cl_RinseAlarmId;
				//	data[1] = 1;
			
				//	command = CON_TX_COMMAND_ALARM;
				//	cl_stby_retval = Cl_SendDatatoconsole(command,&data,2);

					
					
			
				//	Cl_StandbyAlarmTable[tempcount].IsRaised = true;
				}
				else
				{
				}
			
		}

	}

return (cl_stby_retval );
	   	
}

Cl_ReturnCodes Cl_standby_UpdateAlarmTable(ClStbyAlarmIdType* ClstbyAlarmId )
{
	Cl_ReturnCodes 	cl_stby_retval = CL_OK;
	uint8_t tempcount = 0;
	uint8_t data[2];
	
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	for (tempcount = 0 ; tempcount < CL_STBY_ALRM_MAX ; tempcount++)
	{
		
		Cl_Alarm_GetAlarmStatus(Cl_StandbyAlarmTable[tempcount].Cl_RinseAlarmId,&Cl_StandbyAlarmTable[tempcount].IsActive);
		if (Cl_StandbyAlarmTable[tempcount].IsActive)
		{
			if(!Cl_StandbyAlarmTable[tempcount].IsRaised)
				{
					data[0] = (uint8_t)Cl_StandbyAlarmTable[tempcount].Cl_RinseAlarmId;
					data[1] = 1;
					command = CON_TX_COMMAND_ALARM;
					cl_stby_retval = Cl_SendDatatoconsole(command,&data,2);
					Cl_StandbyAlarmTable[tempcount].IsRaised = true;
					
			
				}
				else
				{
					Cl_StandbyAlarmTable[tempcount].alarmseccount++;
					Cl_StandbyAlarmTable[tempcount].alarm5seccount++;
					
					if (Cl_StandbyAlarmTable[tempcount].alarmseccount > 5 )
					{
						// do  5 sec action
						Cl_StandbyAlarmTable[tempcount].alarm5seccount = 0;
					}
					if (Cl_StandbyAlarmTable[tempcount].alarmseccount >= 60 )
					{
						// do 1 min action
						Cl_StandbyAlarmTable[tempcount].alarmseccount = 0;
					}
					if (Cl_StandbyAlarmTable[tempcount].alarmmincount > 5 )
					{
						 // do five min action
					}
				}
			
		}
		else
		{
			// alarm was present before , but not active now.
			if(Cl_StandbyAlarmTable[tempcount].IsRaised)
			{
				
				Cl_StandbyAlarmTable[tempcount].IsRaised = false;
				command = CON_TX_COMMAND_ALARM;
				data[0] = (uint8_t)(Cl_StandbyAlarmTable[tempcount].Cl_RinseAlarmId);
				data[1] = (uint8_t) 0;
				cl_stby_retval = Cl_SendDatatoconsole(command,&data,2);
			}

		}	

	}

return (cl_stby_retval );
}
