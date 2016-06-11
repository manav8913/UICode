/*
 * cl_init.c
 *
 * Created: 3/24/2014 12:41:38 PM
 *  Author: user
 */ 

#include "cl_app/cl_init/inc/cl_init.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/inc/cl_types.h"
Cl_ReturnCodes  Cl_Init_Controller(MAC_EVENTS Cl_MacInitEvent);
Cl_ReturnCodes  cl_init_translatemacevent(MAC_EVENTS ,Cl_Init_Events* );
extern Cl_ReturnCodes Cl_mac_apprequesthandler(MAC_EVENTS);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

extern Cl_Mac_EventType Cl_MacEvent1;

Cl_ReturnCodes  Cl_Init_Controller(MAC_EVENTS Cl_MacInitEvent)
{
	
	Cl_ReturnCodes  Cl_initcode = CL_OK;
	Cl_Init_Events cl_initevent = EVENT_NULL_EVENT;
	static  Cl_Init_StatesType Cl_Init_States = CL_INIT_STATE_IDLE;
	uint8_t data[2];
	Cl_ConsoleTxCommandtype command;
	int count = 0;
	int lram_test_buffer[10];
	
	static int gram_test_buffer[10] = {0xAAAA,0x5555,0xAAAA,0x5555,0xAAAA,0x5555,0xAAAA,0x5555,0xAAAA,};
	
	
	cl_init_translatemacevent( Cl_MacInitEvent, &cl_initevent);
	switch (Cl_Init_States)
	{
		
		case CL_INIT_STATE_IDLE:
					switch(cl_initevent)
					{
						case EVENT_INIT_INIT:
						Cl_Init_States = CL_INIT_STATE_INIT;
						
						data[0] = CL_INIT_ROM_TEST_ON;//rom test
						command = CON_TX_COMMAND_INIT_STATE;
						Cl_initcode = Cl_SendDatatoconsole(command,&data,1);
						//while( //rom test )		
						{
							// algo rom test
						}	
						Cl_Init_States = CL_INIT_RAM_TEST_ON;
						data[0] = CL_INIT_RAM_TEST_ON;//ram test
						command = CON_TX_COMMAND_INIT_STATE;
						Cl_initcode = Cl_SendDatatoconsole(command,&data,1);
						while( count <10)//ram test )	
						{
							//algo ram test
							lram_test_buffer[count] = gram_test_buffer[count];
							count++;
						}
						count = 0;
						while( count <10)//ram test verification )
						{
							if (lram_test_buffer[count] != gram_test_buffer[count])
							{
								// test FAILed
								break;
							}
							count++;
						}
						
						Cl_Init_States = CL_INIT_RAM_TEST_PASSED;
						//TEST PASSED
						
						data[0] = CL_INIT_RAM_TEST_PASSED;//ram test
						command = CON_TX_COMMAND_INIT_STATE;
						Cl_initcode = Cl_SendDatatoconsole(command,&data,1);
						
						Cl_Init_States = CL_INIT_READY;
						data[0] = CL_INIT_READY;
						command = CON_TX_COMMAND_INIT_DONE;
						Cl_initcode = Cl_SendDatatoconsole(command,&data,0);
							
						break;
						default:break;
					}
		break;
		case	CL_INIT_STATE_INIT:
					switch(cl_initevent)
					{

						case EVENT_INIT_TICK_100MS:
						break;
						case EVENT_INIT_TICK_500MS:
						break;
						case EVENT_INIT_TICK_SECOND:
					//	Cl_Init_States = CL_INIT_ROM_TEST_ON;
					//	data[0] = CL_INIT_ROM_TEST_ON;//ram test
					//	command = CON_TX_COMMAND_INIT_STATE;
					//	Cl_initcode = Cl_SendDatatoconsole(command,&data,1);
						
					//	while (// rom test)

						{
							//Cl_SendDatatoconsole(command,&data,1);
						}
						break;
						case EVENT_INIT_TICK_MINUTE:
						break;
						case EVENT_INIT_TICK_HOUR:
						break;
						default:
						break;
					}
		break;
		case CL_INIT_ROM_TEST_ON:
					switch(cl_initevent)
					{

						case EVENT_INIT_TICK_100MS:
						break;
						case EVENT_INIT_TICK_500MS:
						break;
						case EVENT_INIT_TICK_SECOND:
					//	Cl_Init_States = CL_INIT_RAM_TEST_ON;
					//	data[0] = CL_INIT_RAM_TEST_ON;//ram test
					//	command = CON_TX_COMMAND_INIT_STATE;
					//	Cl_initcode = Cl_SendDatatoconsole(command,&data,1);
						break;
						case EVENT_INIT_TICK_MINUTE:
						break;
						case EVENT_INIT_TICK_HOUR:
						break;
						default:
						break;
					}
		break;
		case CL_INIT_RAM_TEST_ON:
					switch(cl_initevent)
					{

						case EVENT_INIT_TICK_100MS:
						break;
						case EVENT_INIT_TICK_500MS:
						break;
						case EVENT_INIT_TICK_SECOND:
						Cl_Init_States = CL_INIT_READY;
						
				//		command = CON_TX_COMMAND_INIT_DONE;
					//	Cl_initcode = Cl_SendDatatoconsole(command,&data,0);
						break;
						case EVENT_INIT_TICK_MINUTE:
						break;
						case EVENT_INIT_TICK_HOUR:
						break;
						default:
						break;
					}
		break;
		case CL_INIT_READY:
							switch(cl_initevent)
							{

								case EVENT_INIT_TICK_100MS:
								break;
								case EVENT_INIT_TICK_500MS:
								break;
								case EVENT_INIT_TICK_SECOND:
								Cl_initcode = Cl_mac_apprequesthandler(MACREQ_ENTER_STANDBY);
								break;
								case EVENT_INIT_TICK_MINUTE:
								break;
								case EVENT_INIT_TICK_HOUR:
								break;
								default:
								break;
							}
					

		break;

			
			default: break;
	}
	
}

Cl_ReturnCodes  cl_init_translatemacevent(MAC_EVENTS Cl_MacInitEvt,Cl_Init_Events* cl_init_event)
{
	switch(Cl_MacInitEvt)
	{
		case EVT_COMMAND_ENTER_INIT:
		*cl_init_event = EVENT_INIT_INIT;
		break;
		case EVT_TICK_100M:
		*cl_init_event = EVENT_INIT_TICK_100MS;
		break;
		case EVT_TICK_500M:
		*cl_init_event = EVENT_INIT_TICK_500MS;
		break;
		case EVT_TICK_SEC:
		*cl_init_event = EVENT_INIT_TICK_SECOND;
		break;		
		case EVT_TICK_MIN:
		*cl_init_event = EVENT_INIT_TICK_MINUTE;
		break;	
		case EVT_TICK_HOUR:
		*cl_init_event = EVENT_INIT_TICK_HOUR;
		break;
		default:
		break;
	}
	return CL_OK;
}
