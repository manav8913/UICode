/*
 * cl_mac_controller.c
 *
 * Created: 12/27/2013 12:17:08 PM
 *  Author: sanjeer_k
 */ 
#ifdef WIN32
#include <stdio.h>
#endif
#include "asf.h"
/*****************/
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
/***********************/
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/inc/cl_types.h"

/********************************************************************/
#ifdef WIN32
#define _WIN32_WINNT 0x0500
#endif
#define UNICODE 1
#define _UNICODE 1
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#endif
#define _SECOND 10000000

#ifdef WIN32
typedef struct _MYDATA {
	TCHAR *szText;
	DWORD dwValue;
}MYDATA;
#endif


/********** GLOBALS****************/
//MAC_STATES Cl_MacState = MAC_STANDBY;
MAC_STATES Cl_MacState = MAC_START;
MAC_EVENTS Cl_MacEvent = EVT_NULL;
bool Cl_MacNewEvent = false;
Cl_Mac_EventType Cl_MacEvent1 ;

cl_console_bufferType Cl_Console_buffer,Cl_Console_buffer1,Cl_Console_buffer2;
cl_console_msgbufferType Cl_Console_msgbuffer;
Cl_ConsoleMsgType Cl_ConsoleRxMsg;
Cl_Sys_statusType sv_sys_statbuffer;
uint8_t Cl_ConsoleTxMsg[10];

uint8_t testbool = false;

Cl_Sys_statusType cl_sys_statbuffer,cl_sys_statbuffer_bak;

/******************* sanjeer := all protos  to be moved into res3epctive header file*/
Cl_ReturnCodes	Cl_MacInit(void);
Cl_ReturnCodes	Cl_MacConfigure(void);
Cl_ReturnCodes	Cl_Mac_GetSystem_Status_Query(void);
Cl_ReturnCodes	Cl_MacCheck_TimerExpired(void);
Cl_ReturnCodes 	Cl_MacCheckforConsoleMessage(void);
Cl_ReturnCodes  Cl_App_init(void);
Cl_ReturnCodes Cl_Mac_ClearEvents(void);
Cl_ReturnCodes Cl_MacCall_Newappreq_Query(void);
Cl_ReturnCodes Cl_Mac_CheckforAlarms(void);
Cl_ReturnCodes Cl_Mac_CheckforErrorCorrections(void);



/************** Extern declarations *********************/

extern void SV_SYSTEM_INIT(void);
extern  Cl_ReturnCodes		 Cl_Console_Read_Message_Console(void);
extern  Cl_ReturnCodes		 Cl_MacWrite_Message_Console(void);
extern  Cl_ReturnCodes		 Cl_MacCreate_Macevent(void);
extern  Cl_ReturnCodes        Cl_Mac_Create_ConsoleEvent(void);
extern  Cl_ReturnCodes        Cl_Console_Read_Message_Ready_Status(void); // Reads the status of the data buffer populated by the timer console read interrupt.
extern  Cl_ReturnCodes		 Cl_Console_Read_ConsoleMessage_Status(void);
extern  Cl_ReturnCodes 		 Cl_Console_Console_Sync(void);
extern	Cl_ReturnCodes		 Cl_Console_init(void);
extern  Cl_ReturnCodes		 Cl_dinf_init(void);
extern	Cl_ReturnCodes		 Cl_dlsis_init(void);
extern  Cl_ReturnCodes		 Cl_dprep_init(void);
extern	Cl_ReturnCodes		 Cl_rinse_init(void);
extern  Cl_ReturnCodes		 Cl_stby_init(void);
extern Cl_ReturnCodes 		Cl_SysStat_System_Status_Query(void);
extern Cl_ReturnCodes   Cl_SendDatatoconsoleDummycommand(Cl_ConsoleTxCommandtype,uint8_t*,uint8_t);
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
Cl_ReturnCodes Cl_alarmsscanforAlarms(void);
/********************** sanjeer*************************/
#ifdef WIN32
void __stdcall timerThread( void ) ;
#endif

extern		  Cl_ReturnCodes	Cl_Standby_Controller(MAC_EVENTS );
extern		  Cl_ReturnCodes	 Cl_Rinse_Controller(MAC_EVENTS   );
extern		  Cl_ReturnCodes	 Cl_Dlsis_controller(MAC_EVENTS);
extern		  Cl_ReturnCodes	 Cl_dprep_controller(MAC_EVENTS);
extern		  Cl_ReturnCodes	 Cl_ConsoleStateHandler(MAC_EVENTS );
extern        Cl_ReturnCodes    cl_console_proto_init(void);


int main(void)
{
	
	
Cl_ReturnCodes 	 Cl_MacRetval = CL_ERROR;
MAC_EVENTS Cl_MacstbEvent;
uint8_t Cl_MacEventData;
	
	
	sysclk_init();// tbr
	board_init();//tbr
	SV_SYSTEM_INIT();
	// Insert application code here, after the board has been initialized.
	//timerThread(  ) ;
//	Cl_MacRetval = sv_system_init(); 
	Cl_MacRetval = Cl_MacInit(); 
	Cl_MacRetval = Cl_App_init(); 

	Cl_MacRetval = Cl_MacConfigure();
		if ( Cl_MacRetval == CL_ACCEPTED)
		{
			
		// Configure the system for a default configuration
		}

				
		Cl_MacRetval = cl_console_proto_init();// cl_console_proto_init()  will init the uart console (prog port) for communication with the  console.It provide a test interface wrapper to the console. It will laso start a timer to read the data from console interface
//		 if ( Cl_MacRetval == CL_ACCEPTED)
		 {
		 // System is ready for sending and receiving the data from console . A timer will read the data to be fetched by the MAC main loop for further processing
			Cl_MacState = MAC_CONSOLE_SYNC;
			if(CL_OK == Cl_Console_Console_Sync())
			{
				//Cl_MacState =   MAC_STANDBY;
				Cl_MacState =   MAC_RINSE;
			}
			
		 }


	while (1)
	{
		
		Cl_MacRetval = Cl_Mac_GetSystem_Status_Query();// check if there is any system updates . ALSO may be this can be moved to a timer thread
		
		Cl_MacRetval = Cl_Mac_CheckforAlarms();// check if there is any alarms , if there is one generate an event. ALSO This can me moved to respective sub  state handlers to be acted up on a much smaller time tick (on a 10- 100 ms event..???)
		Cl_MacRetval = Cl_Mac_CheckforErrorCorrections();// check if there is any corrections, if there is one then generate an event.ALSO it can be moved to respective sub state handlers
		//cl_wait(100);
		Cl_MacRetval = Cl_MacCheckforConsoleMessage();// check if there is any updates from console .. to be shifted to a polling timer..???
	
		while(Cl_MacEvent1.Cl_MacNewEventcount > 0)
		
		{
			
		Cl_MacRetval = Cl_MacCreate_Macevent();

		if(Cl_MacNewEvent)
		
		{
				switch (Cl_MacState)
				{			
					case MAC_START:
					break;
					case MAC_IDLE:
					break;
					case MAC_INIT:
				
					switch ( Cl_MacEvent )
					{
						case EVT_CONSOLE_COMMAND_SYNC:
				
						Cl_MacRetval = Cl_ConsoleStateHandler(EVT_CONSOLE_COMMAND_SYNC);
						Cl_MacEvent = EVT_NULL;
						break;
	
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						break;
						default: break;
					}
					break;
					case MAC_CONSOLE_SYNC:
				
					break;
					case MAC_STANDBY:
				
					switch ( Cl_MacEvent )
				
					{
						case EVT_COMMAND_ENTER_STANDBY:
							Cl_MacRetval = Cl_Standby_Controller(EVT_COMMAND_ENTER_STANDBY);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_RINSE_START:
				
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);	
							}
							
							if( Cl_MacRetval == CL_OK)
							{
								Cl_MacState = MAC_RINSE;
								//Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
										
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if(Cl_MacRetval == CL_ACCEPTED)
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							}
							
							if(Cl_MacRetval == CL_ACCEPTED)
							{
								Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							}
							
							if( Cl_MacRetval == CL_OK)
							{
								Cl_MacState = MAC_DPREP;
							}
							Cl_MacEvent = EVT_NULL;
							
							
						break;
						case EVT_TICK_100M:
						Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
						Cl_MacEvent = EVT_NULL;
						break;
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
							
						
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALARM_TRIGGERED:
							Cl_MacstbEvent = Cl_MacEvent;
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALERT_TRIGGERED:
							Cl_MacstbEvent = Cl_MacEvent;
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_SET_DATA:
							Cl_MacstbEvent = Cl_MacEvent;
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						default: break;
					}
					break;
					case MAC_SAFE_STANDBY:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						break;
						case EVT_ALARM_TRIGGERED:
						break;
						case EVT_ALERT_TRIGGERED:

						break;
						default: break;
					}
					break;
					case MAC_RINSE:

					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
				
						 Cl_MacEvent = EVT_NULL;
						break;
						case EVT_TIMER_EXPIRED:
						//	Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
						//	Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						break;
						case EVT_TICK_100M:
						case EVT_TICK_500M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
							Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case MACREQ_RINSE_COMPLETED:
							Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
								Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALARM_TRIGGERED:
				//			Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
					//		if( Cl_MacRetval == CL_OK)
							{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
					//		Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALERT_TRIGGERED:
					//		Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
					//		if( Cl_MacRetval == CL_OK)
					//		{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
					//		}
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_GET_DATA:
						//	Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
						//	if( Cl_MacRetval == CL_OK)
						//	{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
						//	}
						//	Cl_MacEvent = EVT_NULL;
						break;
						
						default: break;
					}
					break;
					case MAC_POST_RINSE_STANDBY:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
																				
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							}
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							}
							if(Cl_MacRetval == CL_OK)
							{
									Cl_MacState = MAC_DPREP;
							}
						break;
						case EVT_ALARM_TRIGGERED:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALERT_TRIGGERED:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_GET_DATA:
										Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
										if( Cl_MacRetval == CL_OK)
										{
											//	Cl_MacState = MAC_POST_RINSE_STANDBY;
										}
										Cl_MacEvent = EVT_NULL;

						break;

						default: break;
					}
					break;
					case MAC_DPREP:
					switch ( Cl_MacEvent )
						{
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_START_PRIME:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = 	MAC_DPREP_PRIME;
						}
						break;
						case 	EVT_CONSOLE_COMMAND_START_DIALISER_PRIME:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_MAN_PREP_COMPLETED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
						//	Cl_MacState = MAC_POST_DPREP_STANDBY; // for now the prime is set after manual prwepartion is completed
							Cl_MacState = MAC_DPREP_PRIME;
						}
						break;
						case EVT_TICK_100M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						default: break;
					}
					break;
					case MAC_DPREP_PRIME:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_GET_DATA:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);

						Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_START_PRIME:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = 	MAC_DPREP_PRIME;
						}
						break;
						case 	EVT_CONSOLE_COMMAND_START_DIALISER_PRIME:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_MAN_PREP_COMPLETED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = MAC_POST_DPREP_STANDBY;
						}
						break;
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case MACREQ_PRIME_COMPLETED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);

						break;

						default: break;
					}
					break;
					case MAC_SAFE_DPREP:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						break;
						default: break;
					}
					break;
					case MAC_POST_DPREP_STANDBY:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_START:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							}
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							}
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							}

						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = MAC_DIALYSIS;
						}
						break;
						default: break;
					}
					break;
					case MAC_DIALYSIS:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						break;
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
							Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALARM_TRIGGERED:
							Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_ALERT_TRIGGERED:
							Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						case MACREQ_DIALYSIS_COMPLETED:
						
							Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
								Cl_MacState = MAC_POST_DIALYSIS_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;
						default: break;
					}
					break;
					case MAC_SAFE_DIALYSIS:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;

						default: break;
					}
					break;
					case MAC_POST_DIALYSIS_STANDBY:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_RINSE_START:
						break;

						default: break;
					}
					break;
					case MAC_SERVICE:
					switch ( Cl_MacEvent )
				
					{

						default: break;
					}
					break;
					case MAC_STATE_MAX:
					switch ( Cl_MacEvent )
				
					{

						default: break;
					}
					break;
					default :
					break;
				}
				

			
		
				Cl_MacRetval = Cl_Mac_ClearEvents();
		}
		
		}

	}


}

Cl_ReturnCodes 	Cl_MacInit() 
{
		Cl_ReturnCodes Cl_MacRetval = CL_OK;
		
		Cl_MacEvent1.Cl_MacEvent[0] =0;
		Cl_MacEvent1.Cl_MacNewEvent = false;
		Cl_MacEvent1.Cl_MacNewEventcount = 0;
		
		
		Cl_Console_buffer.BufferSize = CONSOLE_BUFFER_SIZE;
		Cl_Console_msgbuffer.BufferSize = CONSOLE_MSG_BUFFER_SIZE;
		Cl_Console_buffer1.BufferEmpty = true;
		Cl_Console_buffer1.ConsoleDatabufferprocessed= false;
		Cl_Console_buffer1.ConsoleDatabufferReady=false;
		Cl_Console_buffer1.CurreltyActive=true;
		Cl_Console_buffer1.StartSequenceDetected=false;

		Cl_Console_buffer2.BufferEmpty = true;
		Cl_Console_buffer2.ConsoleDatabufferprocessed= false;
		Cl_Console_buffer2.ConsoleDatabufferReady=false;
		Cl_Console_buffer2.CurreltyActive=false;
		Cl_Console_buffer2.StartSequenceDetected=false;
		
		Cl_MacState =	MAC_INIT;
		
		
		return Cl_MacRetval;
}

Cl_ReturnCodes  Cl_App_init(void)
{
	Cl_ReturnCodes  Cl_RetValue = CL_ERROR;
	
	Cl_RetValue = Cl_Console_init();
	Cl_RetValue = Cl_dinf_init();
	Cl_RetValue = Cl_dlsis_init();
	Cl_RetValue = Cl_dprep_init();
	Cl_RetValue = Cl_rinse_init();
	Cl_RetValue = Cl_stby_init();
	
	return CL_OK;
	
}
Cl_ReturnCodes 	Cl_MacConfigure(void){ /*printf("System Configuring Controls\n");*/return CL_OK; }

Cl_ReturnCodes 	Cl_MacCheck_TimerExpired(void){return CL_OK; }


Cl_ReturnCodes 	Cl_Mac_GetSystem_Status_Query(void)
{
	Cl_ReturnCodes Cl_MacRetval= CL_ERROR;
	Cl_MacRetval =	Cl_SysStat_System_Status_Query();
	return CL_OK;
}

Cl_ReturnCodes Cl_MacCall_Newappreq_Query(void)
{
		Cl_ReturnCodes Cl_MacRetval= CL_ERROR;
		Cl_MacRetval =	Cl_SysStat_System_Status_Query();
		return CL_OK;
	
}
Cl_ReturnCodes 	Cl_MacCheckforConsoleMessage(void)
{
			Cl_ReturnCodes Cl_MacRetval1;
			
			Cl_MacRetval1 = Cl_Console_Read_ConsoleMessage_Status(); // Reads the status of the data buffer populated by the timer console read interrupt.
			if(Cl_MacRetval1 == CL_OK)
			{
		
				Cl_MacRetval1 = Cl_Console_Read_Message_Console();
				
			}
			if(Cl_MacRetval1 == CL_OK)
			{
			Cl_MacRetval1 = Cl_Console_Read_Message_Ready_Status(); // Reads the status of the data buffer populated by the timer console read interrupt.
			}
			
			if(Cl_MacRetval1== CL_OK)
			{
		
				Cl_MacRetval1 = Cl_Mac_Create_ConsoleEvent();
				
			}
			
			return CL_OK; 
}


Cl_ReturnCodes	Cl_MacCreate_Macevent(void)
{
	Cl_ReturnCodes Cl_Mac_retVal = CL_OK;
	uint8_t tempcount;
	
	Cl_MacNewEvent  = false;
	Cl_MacEvent = EVT_NULL;
	
	if (Cl_MacEvent1.Cl_MacNewEvent)
	{
		if(Cl_MacEvent1.Cl_MacNewEventcount > 0)
		{
			Cl_MacNewEvent = true;
			Cl_MacEvent = Cl_MacEvent1.Cl_MacEvent[0];
			Cl_MacEvent1.Cl_MacNewEventcount--;
			
			for (tempcount =0 ;tempcount < Cl_MacEvent1.Cl_MacNewEventcount;tempcount++ )
			{
				if(tempcount < MAC_EVENT_COUNT_MAX)
				{
					Cl_MacEvent1.Cl_MacEvent[tempcount] = Cl_MacEvent1.Cl_MacEvent[tempcount+1];
				}
				else
				{
					if(tempcount == MAC_EVENT_COUNT_MAX)
					{
						Cl_MacEvent1.Cl_MacEvent[tempcount] = EVT_NULL;	
					}
					
					break; //handle exception
				}
			}
			
			
		}
		else
		{
			Cl_MacNewEvent = false;
			Cl_MacEvent = EVT_NULL;
			
			
			
		}
	}
	else
	{
			Cl_MacNewEvent = false;
			Cl_MacEvent = EVT_NULL;
		
	}
	return CL_OK;
}

/*********************  Cl_MacRetval Cl_Mac_ClearEvents();***************/

Cl_ReturnCodes Cl_Mac_ClearEvents(void)
{
	
	return CL_OK;
	
}





Cl_ReturnCodes Cl_Mac_CheckforAlarms(void)
{
	Cl_ReturnCodes Cl_mac_retval;
	
	Cl_mac_retval = Cl_alarmsscanforAlarms();
	
		return CL_OK;
}
Cl_ReturnCodes Cl_Mac_CheckforErrorCorrections()
{
		if((cl_sys_statbuffer.aptstatus - cl_sys_statbuffer.vptstatus) > 10)
		{}
		if(cl_sys_statbuffer.levelswitch1)
		{}
		if(cl_sys_statbuffer.levelswitch2)
		{}
		if(cl_sys_statbuffer.Temp1status > 100)
		{}
		if(cl_sys_statbuffer.Temp2status > 100)
		{}
	/*
	{
		Cl_MacEvent1.Cl_MacEvent[Cl_MacEvent1.Cl_MacNewEventcount] = EVT_SYS_DATA_CHANGED ;
		Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount]= 0;
		Cl_MacEvent1.Cl_MacNewEvent = true;
		Cl_MacEvent1.Cl_MacNewEventcount++;
	}*/
			
		return CL_OK;
}

















