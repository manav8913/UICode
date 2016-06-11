/*
 * Cl_TimerManager.c
 *
 * Created: 1/14/2014 12:58:28 PM
 *  Author: user
 */ 
#include "cl_app/cl_timer/inc/Cl_TimerManager.h"
#include "cl_app//inc/cl_types.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"


extern MAC_EVENTS Cl_MacEvent;
extern bool Cl_MacNewEvent;

extern Cl_Mac_EventType  Cl_MacEvent2;
extern volatile int FUNC_ENTRY;
extern bool syncdone;

extern Cl_tg_prick_100ms(void);
extern Cl_tg_prick_50ms(void);
extern Cl_ReturnCodes Cl_Alarm_timmer_50millisec(void);
extern void pinclock(void);
extern void pinclock1(void);
extern Cl_ReturnCodes cl_ufpumpFeedback_timer(void);
extern void testsectclock(void);
extern void tg_timer_minute(void);
extern Cl_ReturnCodes 	Cl_Mac_GetSystem_Status_Query(void);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status);
extern void Cl_tg_prick_1ms();
 Cl_ReturnCodes  Cl_Timer_Create_Tick2Ms_Event(void);
 Cl_ReturnCodes  Cl_Timer_Create_Tick5Ms_Event(void);
 Cl_ReturnCodes  Cl_Timer_Create_Tick10Ms_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_Tick20Ms_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_Tick50Ms_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_Tick100Ms_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_Tick500Ms_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_TickSec_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_TickMin_Event(void);
Cl_ReturnCodes  Cl_Timer_Create_TickHour_Event(void);
Cl_ReturnCodes Cl_TimerTickCallback(void);
Cl_ReturnCodes cl_hep_pumpFeedback_timer(void);
Cl_ReturnCodes cl_hep_pump_enddetction_timer(void); // 20 ms clock



Cl_ReturnCodes Cl_RetValue;
int gmstick = 0;
volatile int func_timer =0;
Cl_ReturnCodes Cl_TimerTickCallback()
{
	static int Cl_ticks =0 , Cl_Seconds =0 , Cl_Minutes = 0 , Cl_Hours = 0;
	
	Cl_ticks ++;
	
	 if (Cl_ticks >= TICKS_PER_SEC)
	 {
		 Cl_ticks = 0;
		 Cl_Seconds++;
		// Cl_RetValue = Cl_Timer_Create_TickSec_Event();
	 }
	 
	// if(Cl_Seconds >= SIXTY)
	 if(Cl_Seconds >= SIXTY)
	 {
		 Cl_Seconds = 0;
		  Cl_Minutes++;
		// Cl_RetValue = Cl_Timer_Create_TickMin_Event();
	 }
	 
	//if(Cl_Minutes >= SIXTY)
	if(Cl_Minutes >= 10)
	{
		Cl_Minutes = 0;
		Cl_Hours++;
		Cl_RetValue = Cl_Timer_Create_TickHour_Event();
	}
	
	if(Cl_Hours >= 24)
	{
		Cl_Hours =0;
	}
	
	return CL_OK;
}





Cl_ReturnCodes Cl_MilliTickcallBack(void)
{
	Cl_ReturnCodes Cl_TimerRetValue;
	static int Cl_msticks =0;
//	static int Cl_ms50ticks = 0;
	static int Cl_ms100ticks = 0;
	static int Cl_ms500ticks = 0;
	static int Cl_ms1000ticks = 0;
	
//Cl_tg_prick_1ms();



if (FUNC_ENTRY == 1)
{
	func_timer++;
	if(func_timer > 1000)
	{
		// alarm
			
			if(syncdone)
			{
		//Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_IIC_HANGUP,(void*)0,0);
			}
		func_timer = 0;
	}
}else
{
	func_timer = 0;
}
			gmstick++;
			 if(gmstick > 50)
			 {
				 gmstick = 0;
			 }
			 
		Cl_msticks++;
		if(!(Cl_msticks % 20))
		{
		//	pinclock(); 
		Cl_TimerRetValue = Cl_Timer_Create_Tick20Ms_Event();
			
		cl_hep_pump_enddetction_timer();
		//Cl_tg_prick_100ms();
		


		}
		if(!(Cl_msticks % 2))
		{
			pinclock(); 
			//pinclock();
			//pinclock1();
			Cl_TimerRetValue = Cl_Timer_Create_Tick2Ms_Event();
			
			//	cl_ufpumpFeedback_timer();
			//	cl_hep_pumpFeedback_timer();
		}
		if(!(Cl_msticks % 5))
		{
			
			//pinclock();
			//pinclock1(); 
			Cl_TimerRetValue = Cl_Timer_Create_Tick5Ms_Event();
	
		//	cl_ufpumpFeedback_timer();
		//	cl_hep_pumpFeedback_timer();
		}
		if(!(Cl_msticks % 10))
		{
			Cl_TimerRetValue = Cl_Timer_Create_Tick10Ms_Event();
			//pinclock();
			//pinclock1(); 
			
			

		}
		if(!(Cl_msticks % 50))
		{
			Cl_TimerRetValue = Cl_Timer_Create_Tick50Ms_Event();
		//	Cl_tg_prick_50ms();
			//pinclock1();

		}

		
		
		if(Cl_msticks == 100)
		{
			Cl_msticks = 0;
			Cl_ms100ticks++;
			Cl_TimerRetValue = Cl_Timer_Create_Tick100Ms_Event();

			


		}


		if(Cl_ms100ticks == 5 )
		{
			Cl_ms100ticks = 0;
			Cl_ms500ticks++;
			if(Cl_ms500ticks == 2 )
			{
				Cl_ms500ticks = 0;
				Cl_TimerRetValue  = Cl_Timer_Create_TickSec_Event();
				Cl_ms1000ticks++;
				testsectclock();
				
			}
			Cl_TimerRetValue = Cl_Timer_Create_Tick500Ms_Event();
			
		}
		if(Cl_ms1000ticks == 2)
		{
			Cl_ms1000ticks =0;
		//	Cl_Alarm_TriggerAlarm(FPCURRENTSTATUS,1);
		}
		return CL_OK;
}


 Cl_ReturnCodes  Cl_Timer_Create_Tick2Ms_Event(void)
 {
	 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
	 
	 
	 {
		 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_2M;
		 Cl_MacEvent2.Cl_MacNewEvent = true;
		 Cl_MacEvent2.Cl_MacNewEventcount++;
	 }
	 return CL_OK;
	 
 }

 Cl_ReturnCodes  Cl_Timer_Create_Tick5Ms_Event(void)
 {
	 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
	 
	 
	 {
		 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_5M;
		 Cl_MacEvent2.Cl_MacNewEvent = true;
		 Cl_MacEvent2.Cl_MacNewEventcount++;
	 }
	 return CL_OK;
	 
 }

 Cl_ReturnCodes  Cl_Timer_Create_Tick10Ms_Event(void)
 {
	 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
	 
	 
	 {
		 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_10M;
		 Cl_MacEvent2.Cl_MacNewEvent = true;
		 Cl_MacEvent2.Cl_MacNewEventcount++;
	 }
	 return CL_OK;
	 
 }

 Cl_ReturnCodes  Cl_Timer_Create_Tick20Ms_Event(void)
 {
	 	 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
	 	 
	 	 
	 	 {
		 	 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_20M;
		 	 Cl_MacEvent2.Cl_MacNewEvent = true;
		 	 Cl_MacEvent2.Cl_MacNewEventcount++;
	 	 }
	 	 return CL_OK;
	 	 
 }

 Cl_ReturnCodes  Cl_Timer_Create_Tick50Ms_Event(void)
 {
	 // Cl_MacEvent = EVT_TICK_SEC;
	 // Cl_MacNewEvent = true;
	 Cl_Alarm_timmer_50millisec();// testing
	 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
	 
	 
	 {
		 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_50M;
		 Cl_MacEvent2.Cl_MacNewEvent = true;
		 Cl_MacEvent2.Cl_MacNewEventcount++;
	 }
	 return CL_OK;
	 
	 
 }
 

	 Cl_ReturnCodes  Cl_Timer_Create_Tick100Ms_Event(void)
	 {
		 // Cl_MacEvent = EVT_TICK_SEC;
		 // Cl_MacNewEvent = true;
		 
		 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
		 		 
		 {	
			 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_100M ;
			 Cl_MacEvent2.Cl_MacNewEvent = true;
			 Cl_MacEvent2.Cl_MacNewEventcount++;
		 }
		 return CL_OK;
		 
		 
	 }
	 
	 
	 Cl_ReturnCodes  Cl_Timer_Create_Tick500Ms_Event(void)
	 {
		 // Cl_MacEvent = EVT_TICK_SEC;
		 // Cl_MacNewEvent = true;
		 
		 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
		 
		 
		 {
			 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_500M ;
			 Cl_MacEvent2.Cl_MacNewEvent = true;
			 Cl_MacEvent2.Cl_MacNewEventcount++;
		 }
		 return CL_OK;
		 
		 
	 }



	 Cl_ReturnCodes  Cl_Timer_Create_TickSec_Event(void)
	 {
		// Cl_MacEvent = EVT_TICK_SEC;
		// Cl_MacNewEvent = true;
		static uint8_t Cl_Secondscnt = 0;
		
		Cl_Secondscnt++;
		// Cl_Mac_GetSystem_Status_Query();
		 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )
		
		
		 {
			 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_SEC ;
			 Cl_MacEvent2.Cl_MacNewEvent = true;
			 Cl_MacEvent2.Cl_MacNewEventcount++;
		 }
		 
	if(Cl_Secondscnt >= SIXTY)
	 {
		 Cl_Secondscnt = 0;
		//  Cl_Minutes++;
		 Cl_RetValue = Cl_Timer_Create_TickMin_Event();
		 tg_timer_minute();
	 }
		 return CL_OK;
		 
		
	 }

	 Cl_ReturnCodes  Cl_Timer_Create_TickMin_Event(void)
	 {
		 
		 if( Cl_MacEvent2.Cl_MacNewEventcount < MAC_EVENT_COUNT_MAX )	
		 {
			 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_MIN ;
			 Cl_MacEvent2.Cl_MacNewEvent = true;
			 Cl_MacEvent2.Cl_MacNewEventcount++;
			
		 }
		 return CL_OK;
		 
		 // Cl_Console_ConvertTimeoutEvent_toMacEvent(Cl_ConsoleRxEventsType command ,MAC_EVENTS* MacEvent)
	 }

	 Cl_ReturnCodes  Cl_Timer_Create_TickHour_Event(void)
	 {

		 if( Cl_MacEvent2.Cl_MacNewEventcount <= MAC_EVENT_COUNT_MAX )	
		 {
			 Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = EVT_TICK_HOUR ;
			 Cl_MacEvent2.Cl_MacNewEvent = true;
			 Cl_MacEvent2.Cl_MacNewEventcount++;
			 
		 }
		 return CL_OK;
		 
		 // Cl_Console_ConvertTimeoutEvent_toMacEvent(Cl_ConsoleRxEventsType command ,MAC_EVENTS* MacEvent)
	 }

