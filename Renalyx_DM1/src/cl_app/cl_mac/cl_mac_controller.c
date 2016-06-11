/*
 * cl_mac_controller.c
 *
 * Created: 12/27/2013 12:17:08 PM
 *  Author: sanjeer_k
 */
 
#include "asf.h"

#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "Platform/Service/sv_interface.h"

#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_sys/cl_sys.h"
#include "cl_app/comp/levelswichcntrl/inc/cl_level_switchcontroller.h"
#include "cl_app/comp/flowswitchcntrl/inc/flowswitchcntrl.h"
#include "cl_app/comp/deaeration/inc/cl_deaeration.h"
#include "cl_app//cl_dprep/inc/cl_dprep_primecontroller.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"

/**********************************/
#define NVRAM_BUS_ADDR       0x77        //!< TWI slave bus address (digital pot)
#define NVRAM_MEM_ADDR       0x0002        //!< TWI slave memory address

#define DIPOT_BUS_ADDR       0x2C        //!< TWI slave bus address (digital pot)
#define TWI_SPEED            100000       //!< TWI data transfer rate(100Khz)
#define TWI_EXAMPLE TWI0
#define PATTERN_TEST_LENGTH     sizeof(testpattern)
#define PATTERN_TEST_LENGTH     sizeof(testpattern)
const uint8_t testpattern[] = {
	0x10,
	0x02,
	0x30,
	0x44,
	0x55,
	0x66,
	0x77,
	0x88,
	0x99,
	0xAA,
	0xBB,
	0xCC,
	0xDD,
	0xEE,
	0xFF
};

/****************************************/

#define FOREVER 1
MAC_STATES Cl_MacState = MAC_START;
MAC_EVENTS Cl_MacEvent = EVT_NULL;
SYS_STATE Cl_sys_state = SYS_STATE_IDLE;
bool Cl_MacNewEvent = false;
bool ConsoleSync = true;
Cl_Mac_EventType Cl_MacEvent1 ;
Cl_Mac_EventType Cl_MacEvent2 ;
Cl_Mac_EventType Cl_MacEvent3 ;
cl_console_bufferType Cl_Console_buffer,Cl_Console_buffer1,Cl_Console_buffer2;
cl_console_msgbufferType Cl_Console_msgbuffer;
Cl_ConsoleMsgType Cl_ConsoleRxMsg;
Cl_Sys_statusType sv_sys_statbuffer;
uint8_t Cl_ConsoleTxMsg[40];
uint8_t Cl_IIC_ConsoleTxMsg[40];

uint8_t testbool = false;
Cl_Sys_statusType  cl_sys_statbuffer;
extern volatile uint32_t g_ul_ms_ticks ;
	uint32_t ul_cur_ticks, delayticks; // testing
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
Cl_ReturnCodes Cl_alarms_scanforAlarms(void);
Cl_ReturnCodes Cl_Enter_CalibrateMode(void);
Cl_ReturnCodes cl_mac_checkforsync(void);

/************** Extern declarations *********************/

extern void SV_SYSTEM_INIT(void);
extern Cl_ReturnCodes cl_event_handler(MAC_EVENTS cl_event);
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
extern Cl_ReturnCodes		Cl_Init_Alarms(void);
extern Cl_ReturnCodes 		Cl_SysStat_System_Status_Query(void);
extern Cl_ReturnCodes   Cl_SendDatatoconsoleDummycommand(Cl_ConsoleTxCommandtype,uint8_t*,uint8_t);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
extern Cl_ReturnCodes cl_level_switchcontroller(levelswitchcntrl_event_type cntrl_event );
extern Cl_ReturnCodes cl_flow_switchcontroller(flowswitchcntrl_event_type cntrl_event);
extern Cl_ReturnCodes cl_dprep_primecontroller(Cl_Dprep_PrimeEvents prime_event , int16_t data);
extern Cl_ReturnCodes cl_deaeration_controller(cl_deaeration_event_type cntrl_event);
extern uint8_t  sv_cntrl_activatepump(sv_pumptype);
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );

extern		  Cl_ReturnCodes	Cl_Standby_Controller(MAC_EVENTS );
extern		  Cl_ReturnCodes	 Cl_Rinse_Controller(MAC_EVENTS   );
extern		  Cl_ReturnCodes	 Cl_Dlsis_controller(MAC_EVENTS);
extern		  Cl_ReturnCodes	 Cl_dprep_controller(MAC_EVENTS);
extern		  Cl_ReturnCodes	 Cl_ConsoleStateHandler(MAC_EVENTS );
extern		  Cl_ReturnCodes     Cl_Init_Controller(MAC_EVENTS);
extern        Cl_ReturnCodes    cl_console_proto_init(void);
extern uint8_t sv_cntrl_poweronheater(void);
extern void Cl_SysStat_mean_status_update(void);
extern void DD_IIC_CONSOLE_SEND(uint8_t iic_address, uint8_t* data,uint8_t length);


int main(void)

		   {
		board_init();
		sysclk_init();
	


	
Cl_ReturnCodes 	 Cl_MacRetval = CL_ERROR;
MAC_EVENTS Cl_MacstbEvent;
uint8_t Cl_MacEventData;
static uint loopcountm = 0, loopcountl = 0 , loopcountinsec = 0;
	
	                                                                                           
	sysclk_init();// tbr
	board_init();//tbr
	SV_SYSTEM_INIT();
	// Insert application code here, after the board has been initialized.
	//timerThread(  ) ;
//	Cl_MacRetval = sv_system_init(); 
pmc_enable_periph_clk(ID_PIOA);
pmc_enable_periph_clk(ID_TWI0);
PIOA->PIO_WPMR = 0x50494F00;
PIOA->PIO_PUER = 0x00000300;
PIOA->PIO_PUER = 0x00060000;
PIOA->PIO_PDR =	 0x00060000;
PIOA->PIO_ABSR = 0x00090000;

	Cl_MacRetval = Cl_MacInit(); 
	
		 uint16_t temp100;
		 Cl_ReturnCodes cl_stby_retval = CL_ERROR;
		 
		 
		 for(temp100 = 1;temp100<=20; temp100++)
		 {
			 sv_cntrl_deactivate_valve((sv_valvetype)temp100);
			 cl_wait(10);
		 }
	
				  
	Cl_MacRetval = Cl_App_init(); 

	Cl_MacRetval = Cl_MacConfigure();
		if ( Cl_MacRetval == CL_ACCEPTED)
		{
			
		// Configure the system for a default configuration
		}

				


	//	 if ( Cl_MacRetval == CL_ACCEPTED)
		 {
		 // System is ready for sending and receiving the data from console . A timer will read the data to be fetched by the MAC main loop for further processing
			

							
							Cl_IIC_ConsoleTxMsg[0] = 0xAA ;
							Cl_IIC_ConsoleTxMsg[1] = 0xCC ;
							Cl_IIC_ConsoleTxMsg[2] = 0xDD ;
							Cl_IIC_ConsoleTxMsg[3] = 0x66 ;
							Cl_IIC_ConsoleTxMsg[4] = 0x02 ;
							
			
							
							Cl_IIC_ConsoleTxMsg[5] = 0x77 ;
							Cl_IIC_ConsoleTxMsg[6] = 0x44 ;
							Cl_IIC_ConsoleTxMsg[7] = 0xEE ;
							Cl_IIC_ConsoleTxMsg[8] = 0xBB ;

		//	while (1)
			{
		//		DD_IIC_CONSOLE_SEND(0x77, &Cl_IIC_ConsoleTxMsg, 9);
			}
			Cl_MacState = MAC_CONSOLE_SYNC;
			if(CL_OK == Cl_Console_Console_Sync())
			{
				Cl_sys_state = SYS_STATE_WAIT_FOR_MODE;
				Cl_MacState =   MAC_INIT;
				

			}
			
		 }
		 #if 0
		 if( Cl_sys_state == SYS_STATE_WAIT_FOR_MODE)
		 {
			 while (FOREVER)
			 {
				 

				 Cl_MacRetval = Cl_MacCheckforConsoleMessage();
					while(Cl_MacEvent1.Cl_MacNewEventcount > 0)

					{
						Cl_MacRetval = Cl_MacCreate_Macevent();
											
						if(Cl_MacNewEvent == EVT_COMMAND_ENTER_INIT)
						{
							Cl_MacRetval = Cl_Init_Controller(Cl_MacEvent);
							
							Cl_MacState =   MAC_STANDBY;
						}
						else
						{
						
							if(Cl_MacEvent == EVT_CONSOLE_COMMAND_SERVICEMODE_ENTER)
							{
								// call calibrate entry and loop there; else break and continue following flow
							}
							else if (Cl_MacEvent == EVT_CONSOLE_COMMAND_USER_MODE_ENTER)
							{
								break;
							
							}
						}
					}
					
							 if (Cl_MacEvent == EVT_CONSOLE_COMMAND_USER_MODE_ENTER)
							{
								break;
							}
			 }
		 }

#endif
	while (FOREVER)
	{



	ul_cur_ticks = g_ul_ms_ticks;
	
		Cl_MacRetval = Cl_Mac_GetSystem_Status_Query();// check if there is any system updates . ALSO may be this can be moved to a timer thread
		 
		//Cl_MacRetval = Cl_Mac_CheckforAlarms();// check if there is any alarms , if there is one generate an event. ALSO This can me moved to respective sub  state handlers to be acted up on a much smaller time tick (on a 10- 100 ms event..???)
		Cl_MacRetval = Cl_Mac_CheckforErrorCorrections();// check if there is any corrections, if there is one then generate an event.ALSO it can be moved to respective sub state handlers
		Cl_MacRetval = Cl_MacCheckforConsoleMessage();// check if there is any updates from console .. to be shifted to a polling timer..???
		delayticks = g_ul_ms_ticks - ul_cur_ticks;
		
		Cl_MacRetval = cl_event_handler(EVT_TICK_INSTANT);
	//	cl_level_switchcontroller(LEVEL_SWITCH_CNTRLER_EVENT_INSTANT);
		
		while((Cl_MacEvent1.Cl_MacNewEventcount > 0) ||(Cl_MacEvent2.Cl_MacNewEventcount > 0))
		
		{
		Cl_MacEvent = EVT_NULL;
		Cl_MacRetval = Cl_MacCreate_Macevent();

		if(Cl_MacNewEvent)
		
		{
			
			Cl_MacRetval = cl_event_handler(Cl_MacEvent);
				#if 0		
				if(Cl_MacEvent == EVT_TICK_2M)
						{
							
							static uint8_t status = 0;
							if(status == 0)
							{
								sv_cntrl_activatepump(BLOODPUMP);
								status = 1;
							}
							else
							{
								sv_cntrl_deactivatepump(BLOODPUMP);
								status = 0;
							}
							Cl_MacEvent = EVT_NULL;
							
						}
						else
						{
							Cl_MacEvent = EVT_NULL;
							
						}
						continue;
						
						#endif
#if 0			

			if(Cl_MacEvent == EVT_TICK_SEC)
			{
				cl_level_switchcontroller(LEVEL_SWITCH_CNTRLER_EVENT_SEC);
				cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_SEC);
				cl_dprep_primecontroller(CL_DPREP_PRIME_PRIME_TICK_SEC,0);
				
			//Cl_SendDatatoconsoleDummycommand(0x99,&Cl_IIC_ConsoleTxMsg,7);
				//cl_mac_checkforsync();
				
	
		
			}


			if(Cl_MacEvent == EVT_TICK_20M)
			{
				
				Cl_SysStat_mean_status_update();
			}

			if(Cl_MacEvent == EVT_TICK_50M)
			{
				//cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_INSTANT);
				//cl_deaeration_controller(CL_DEAER_EVENT_50MS);

								{
									//	if ( (command != CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT) && (command != CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT) &&(command != CON_TX_COMMAND_PRINTDATA) && (command != CON_TX_COMMAND_PRINTTEXT))
									{
										//DD_IIC_CONSOLE_SEND(0x77, &Cl_IIC_ConsoleTxMsg, 9);
										//Cl_SendDatatoconsoleDummycommand(0x99,&Cl_IIC_ConsoleTxMsg,7);
									}
									
								}
								
				
			}
			if(Cl_MacEvent == EVT_TICK_500M)
			{
			
			//	cl_deaeration_controller(CL_DEAER_EVENT_500MS);
			//	Cl_IIC_ConsoleTxMsg[0] = 0x99 ;
			//	Cl_IIC_ConsoleTxMsg[1] = 0xCC ;
			//	Cl_IIC_ConsoleTxMsg[2] = 0xDD ;
			//	Cl_IIC_ConsoleTxMsg[3] = 0x66 ;
			//	Cl_IIC_ConsoleTxMsg[4] = 0x01 ;
			//	Cl_IIC_ConsoleTxMsg[5] = 0x88 ;
			//	Cl_IIC_ConsoleTxMsg[6] = 0xEE ;
			//	Cl_IIC_ConsoleTxMsg[7] = 0xBB ;
				//DD_IIC_CONSOLE_SEND(0x55, &Cl_IIC_ConsoleTxMsg, 7);
			//	Cl_SendDatatoconsoleDummycommand(0xAA,&Cl_IIC_ConsoleTxMsg,7);
			
		
			}
#endif
			//continue;

			if(Cl_MacEvent == EVT_CONSOLE_COMMAND_ACK)
			{
				//send sync
				ConsoleSync = true;
			}
			
			if(Cl_MacEvent == EVT_CONSOLE_COMMAND_SYNC)
			{
				//send sync
				uint8_t data = CON_RX_COMMAND_SYNC;
				ConsoleSync = true;
				Cl_SendDatatoconsole(CON_TX_COMMAND_ACK,&data,1);
				
			}			
			
			
				switch (Cl_MacState)
				{			
					case MAC_START:
					break;
					case MAC_IDLE:
					break;
					case MAC_INIT:
				
					switch ( Cl_MacEvent )
					{
						case EVT_COMMAND_ENTER_INIT:
						Cl_MacRetval = Cl_Init_Controller(Cl_MacEvent);
			
						Cl_MacEvent = EVT_NULL;
						break;

						case MACREQ_ENTER_STANDBY:
						Cl_MacRetval = Cl_Init_Controller(EVT_COMMAND_ENTER_STANDBY);
						Cl_MacRetval = Cl_Standby_Controller(EVT_COMMAND_ENTER_STANDBY);
						Cl_MacEvent = EVT_NULL;
							Cl_MacState =   MAC_STANDBY;
						break;
						case EVT_TICK_100M:	
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						Cl_MacRetval = Cl_Init_Controller(Cl_MacEvent);
						Cl_MacEvent = EVT_NULL;

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
						case EVT_TICK_100M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:			
						case EVT_ALERT_TRIGGERED:	
						case EVT_ALARM_TRIGGERED:
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_SET_DATA:
						
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
							break;

						case EVT_CONSOLE_COMMAND_RINSE_START:
						loopcountinsec = 0;
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
							loopcountinsec =0;
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if(Cl_MacRetval == CL_OK)
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);	
							}
							
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_TIMER_EXPIRED:
							Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						case EVT_TICK_50M:
						case EVT_TICK_100M:
						case EVT_TICK_500M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						case EVT_ALARM_TRIGGERED:
						case EVT_ALERT_TRIGGERED:
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_SET_DATA:
						case MACREQ_BC_OPENFILL_COMPLETED:
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
						case EVT_CONSOLE_COMMAND_STOP_RINSE:
							Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
							//	Cl_MacState = MAC_POST_RINSE_STANDBY;
							}
							Cl_MacEvent = EVT_NULL;
						break;

						case EVT_CONSOLE_COMMAND_START_PRIME:
						case EVT_CONSOLE_COMMAND_STOP_PRIME:
						case EVT_CONSOLE_COMMAND_RCIRC_START:
						case EVT_CONSOLE_COMMAND_RCIRC_STOP:
						Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
						//Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
						case EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
						Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);						
						break;
						default: break;
					}
					break;
					case MAC_POST_RINSE_STANDBY:
				switch ( Cl_MacEvent )
				
					{
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
						case EVT_SEND_MIXING_PREP_START://0x21
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							if(Cl_MacRetval == CL_OK)
							{
									Cl_MacState = MAC_DPREP;
							}
						
						break;
						case EVT_SEND_MIXING_PREP_STOP://0x22
						break;
						case EVT_ALARM_TRIGGERED:
						case EVT_ALERT_TRIGGERED:
						case EVT_CONSOLE_COMMAND_GET_DATA:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
						//	Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						case EVT_CONSOLE_COMMAND_SET_DATA:
							Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						
						break;
						case EVT_CONSOLE_COMMAND_RCIRC_START:
						case EVT_CONSOLE_COMMAND_RCIRC_STOP:
						Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_START_PRIME:
						case EVT_CONSOLE_COMMAND_STOP_PRIME:
						Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
						//Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
						case EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
						Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);						
						break;
						default: break;
					}
					break;
					case MAC_DPREP:
					switch ( Cl_MacEvent )
						{
						case EVT_CONSOLE_COMMAND_SET_DATA:
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
						case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
						case EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
						case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
						case EVT_CONSOLE_COMMAND_START_DIALISER_PRIME:
						case EVT_CONSOLE_COMMAND_STOP_DIALISER_PRIME:
						case EVT_TICK_50M:
						case EVT_TICK_100M:
						case EVT_TICK_500M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						case EVT_CONSOLE_COMMAND_START_DIALYSATE_FILLING:
						case EVT_ALERT_TRIGGERED:
						case EVT_ALARM_TRIGGERED:
						case EVT_CONSOLE_COMMAND_STOP_PRIME:
						case EVT_SEND_MIXING_PREP_STOP://0x22
	
						
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_START_PRIME:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
						//	Cl_MacState = 	MAC_DPREP_PRIME;
						}
						break;
						case EVT_CONSOLE_COMMAND_RCIRC_START:
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
						
						
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case MACREQ_DIALYSER_PRIME_COMPLETED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = MAC_POST_DPREP_STANDBY;
						}
						break;
						
						case EVT_CONSOLE_COMMAND_DILYSER_CONNECTED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);

						break;

						default: break;
					}
					break;
					case MAC_DPREP_PRIME:
					switch ( Cl_MacEvent )
				
					{
						case EVT_CONSOLE_COMMAND_SET_DATA:
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_START_DIALYSATE_FILLING:
						case EVT_CONSOLE_COMMAND_DILYSER_CONNECTED:
						case EVT_CONSOLE_COMMAND_START_DIALISER_PRIME:
						case EVT_CONSOLE_COMMAND_STOP_DIALISER_PRIME:
						case EVT_CONSOLE_COMMAND_MAN_PREP_COMPLETED:
						case EVT_TICK_500M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						case EVT_ALERT_TRIGGERED:
						case EVT_ALARM_TRIGGERED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_START_PRIME:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = 	MAC_DPREP_PRIME;
						}
						break;

						

						case MACREQ_PRIME_COMPLETED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);

						break;
						case MACREQ_DIALYSER_PRIME_COMPLETED:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = MAC_POST_DPREP_STANDBY;
						}
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

						case EVT_ALERT_TRIGGERED:
						case EVT_ALARM_TRIGGERED:
						case EVT_TICK_SEC:
						case EVT_TICK_50M:
						case EVT_TICK_500M:
						case EVT_CONSOLE_COMMAND_PATIENT_CONNECTED:
						case EVT_CONSOLE_COMMAND_PATIENT_READY:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						
						case EVT_CONSOLE_COMMAND_SET_DATA:
						case EVT_CONSOLE_COMMAND_GET_DATA:
						Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_START:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							}
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							}
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
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
						case EVT_TICK_50M:
						case EVT_TICK_500M:
						case EVT_TICK_SEC:
						case EVT_TICK_MIN:
						case EVT_TICK_HOUR:
						case EVT_ALARM_TRIGGERED:
						case EVT_ALERT_TRIGGERED:
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_SET_DATA:
							Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
						break;
						
						case EVT_CONSOLE_COMMAND_DIALYSIS_STOP:
						Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							if( Cl_MacRetval == CL_OK)
							{
								Cl_MacState = MAC_SAFE_DIALYSIS;
							}
						break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_START:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							}
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							}
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							}

						if(Cl_MacRetval == CL_OK)
						{
							Cl_MacState = MAC_DIALYSIS;
						}
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
						case EVT_CONSOLE_COMMAND_GET_DATA:
						case EVT_CONSOLE_COMMAND_SET_DATA:
							Cl_MacRetval = Cl_Dlsis_controller(Cl_MacEvent);
							Cl_MacEvent = EVT_NULL;
							break;
						case EVT_CONSOLE_COMMAND_DIALYSIS_START:
							Cl_MacRetval = Cl_Standby_Controller(Cl_MacEvent);
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_Rinse_Controller(Cl_MacEvent);
							}
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
							{
								Cl_MacRetval = Cl_dprep_controller(Cl_MacEvent);
							}
							if((Cl_MacRetval == CL_OK)||(Cl_MacRetval == CL_REJECTED))
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
					case MAC_POST_DIALYSIS_STANDBY:
					switch ( Cl_MacEvent )
				
					{
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
		else
		{
			
		}
		
		}

	}

//#endif
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
	
	Cl_RetValue = Cl_Init_Alarms();
	Cl_RetValue = Cl_Console_init();
	Cl_RetValue = Cl_dinf_init();
	Cl_RetValue = Cl_dlsis_init();
	Cl_RetValue = Cl_dprep_init(	);
	Cl_RetValue = Cl_rinse_init();
	Cl_RetValue = Cl_stby_init();
	
	return CL_OK;
	
}
Cl_ReturnCodes 	Cl_MacConfigure(void){ /*printf("System Configuring Controls\n");*/

	
	

	
	return CL_OK; }

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
	
		if ((Cl_MacEvent3.Cl_MacNewEvent) && (Cl_MacEvent3.Cl_MacNewEventcount > 0))
		{
	

			Cl_MacNewEvent = true;
			Cl_MacEvent = Cl_MacEvent3.Cl_MacEvent[0];
			Cl_MacEvent3.Cl_MacNewEventcount--;
	
			for (tempcount =0 ;tempcount < Cl_MacEvent3.Cl_MacNewEventcount;tempcount++ )
			{
				if(tempcount < MAC_EVENT_COUNT_MAX)
				{
					Cl_MacEvent3.Cl_MacEvent[tempcount] = Cl_MacEvent3.Cl_MacEvent[tempcount+1];
			

			
				}
				else
				{
					if(tempcount == MAC_EVENT_COUNT_MAX)
					{
						Cl_MacEvent3.Cl_MacEvent[tempcount] = EVT_NULL;
					}
			
					break; //handle exception
				}
			}
	
	
		}
		else if ((Cl_MacEvent2.Cl_MacNewEvent) && (Cl_MacEvent2.Cl_MacNewEventcount > 0))
		{
			

			Cl_MacNewEvent = true;
			Cl_MacEvent = Cl_MacEvent2.Cl_MacEvent[0];
			Cl_MacEvent2.Cl_MacNewEventcount--;
			
			for (tempcount =0 ;tempcount < Cl_MacEvent2.Cl_MacNewEventcount;tempcount++ )
			{
				if(tempcount < MAC_EVENT_COUNT_MAX)
				{
					Cl_MacEvent2.Cl_MacEvent[tempcount] = Cl_MacEvent2.Cl_MacEvent[tempcount+1];
					

					
				}
				else
				{
					if(tempcount == MAC_EVENT_COUNT_MAX)
					{
						Cl_MacEvent2.Cl_MacEvent[tempcount] = EVT_NULL;
					}
					
					break; //handle exception
				}
			}
			Cl_MacEvent2.Cl_MacEvent[Cl_MacEvent2.Cl_MacNewEventcount] = 0;
			
			
		}

		else if ((Cl_MacEvent1.Cl_MacNewEvent) && (Cl_MacEvent1.Cl_MacNewEventcount > 0))
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
	
	Cl_mac_retval = Cl_alarms_scanforAlarms();
	
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
		Cl_MacEvent1.Cl_MacEventData[Cl_MacEvent1.Cl_MacNewEventcount][0]= 0;
		Cl_MacEvent1.Cl_MacNewEvent = true;
		Cl_MacEvent1.Cl_MacNewEventcount++;
	}*/
			
		return CL_OK;
}



Cl_ReturnCodes Cl_Enter_CalibrateMode(void)
{
	
	Cl_ReturnCodes  Cl_calibrate_Retcode = CL_OK;
	
	while(FOREVER)
	{
		
		Cl_calibrate_Retcode = Cl_Mac_GetSystem_Status_Query();// check if there is any system updates . ALSO may be this can be moved to a timer thread
		
		//
	}
	
	
}

Cl_ReturnCodes cl_mac_checkforsync(void)
{
	Cl_ReturnCodes  Cl_Retcode = CL_OK;
	if (ConsoleSync == true)
	{
		Cl_SendDatatoconsole( CON_TX_COMMAND_SYNC,NULL ,0);
		ConsoleSync = false;
	}
	else
	{
		// raise alarm
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"LOST",4);
	}
	
}