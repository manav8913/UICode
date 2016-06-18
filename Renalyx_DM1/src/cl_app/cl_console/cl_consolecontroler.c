/*
 * cl_consolehandler.c
 *
 * Created: 12/27/2013 5:21:35 PM
 *  Author: user
 */ 

#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "inc/cl_consolecontroller.h"
#include "sv_stubs/inc/sv_stub.h"
#define  IIC_CONSOLE_ADDR 0x55
extern Cl_Mac_EventType Cl_MacEvent3;
extern bool Cl_MacNewEvent;
extern  uint8_t testbool ;
extern cl_console_bufferType Cl_Console_buffer,Cl_Console_buffer1,Cl_Console_buffer2;
extern cl_console_msgbufferType Cl_Console_msgbuffer;
extern bool  check_endofsequence_is_received(uint8_t  );
extern bool  check_startofsequence_is_received(uint8_t  );
extern Cl_ReturnCodes cl_testharnesscommandhandler(Cl_ConsoleMsgType*);
extern void DD_IIC_CONSOLE_SEND(uint8_t iic_address, uint8_t* data,uint8_t length);
uint8_t sv_console_bytedatawrite(uint8_t sv_consoleTxdatabyte);
extern MAC_EVENTS Cl_MacEvent;
Cl_ReturnCodes Cl_Console_ConvertConsoleEvent_toMacEvent(Cl_ConsoleRxEventsType , MAC_EVENTS*);
Cl_Cl_ConsoleStateType Cl_ConsoleState = CON_STATE_INIT;
bool syncdone = false;

static uint16_t test_counter = 0;
extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern uint8_t Cl_ConsoleTxMsg[40];
extern uint8_t Cl_IIC_ConsoleTxMsg[40];
extern array_command_type cmd_backup;
//extern  DD_SET_PIO(pio_type peripheral,uint32_t pio);

static Cl_ConsoleMsg_readStateType Cl_ConsoleMsg_readState = MSG_RX_IDLE;


int8_t test_count = 0 ;
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);


Cl_ReturnCodes		Cl_Console_Read_ConsoleMessage_Status(void);
Cl_ReturnCodes		 Cl_Console_Read_Message_Console(void);
Cl_ReturnCodes        Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
Cl_ReturnCodes		 Cl_MacWrite_Message_Console(void);
Cl_ReturnCodes	 Cl_Mac_Create_ConsoleEvent(void);
Cl_ReturnCodes  Cl_ScanForMsgStart(void);
Cl_ReturnCodes Cl_Cosnsole_MsgAcknlowledge(void);
Cl_ReturnCodes  cl_console_translatemacevent(MAC_EVENTS ,Cl_ConsoleRxEventsType );
Cl_ReturnCodes	 Cl_ConsoleStateHandler(MAC_EVENTS );
Cl_ReturnCodes 	Cl_Console_Console_Sync(void);
Cl_ReturnCodes Cl_Console_init(void);
Cl_ReturnCodes Cl_Console_SyncGetMessage(void);
uint8_t Cl_increment_msgbufferindex(void);
uint8_t Cl_increment_consolebufferindex(void);
Cl_ReturnCodes  Cl_Console_Read_Message_Ready_Status(void);
Cl_ReturnCodes Cl_SendDatatoconsoleDummycommand(Cl_ConsoleTxCommandtype command, uint8_t* data,uint8_t datasize);

uint8_t memcopy(uint8_t* dest, uint8_t* source, uint32_t size);


uint8_t loopackflag = false;



Cl_ReturnCodes Cl_Console_Read_ConsoleMessage_Status(void)
{
	
	//Cl_ReturnCodes Cl_ConsoleRetval;
	Cl_ReturnCodes Cl_ConsoleRetval;
	Cl_ConsoleRetval = CL_ERROR;
	
	

	if(Cl_Console_buffer1.ConsoleDatabufferReady)
	{
			Cl_ConsoleRetval = CL_OK;
	}
	else if(Cl_Console_buffer2.ConsoleDatabufferReady)
	{
		Cl_ConsoleRetval = CL_OK;
	}
	else
	{
			Cl_ConsoleRetval =  CL_WAIT;
			//Cl_ConsoleRetval =  10;
			
	}
	return Cl_ConsoleRetval;
}
Cl_ReturnCodes  Cl_Console_Read_Message_Ready_Status(void)
{
	
	if(Cl_Console_msgbuffer.ConsoleNewMessageReady == true)
	{

//printf("RDY\n");
	//for (tempcount=0;tempcount<Cl_Console_msgbuffer.Console_databuffer_write_idx;tempcount++)

		return CL_OK;
	}
	else
	{
//		printf("WT\n");
		return CL_WAIT;
	}
	
}

Cl_ReturnCodes		Cl_Console_Read_Message_Console()
{

	static uint8_t tempcount=0;
	uint8_t temp;

if((Cl_Console_buffer1.ConsoleDatabufferReady == true) && (Cl_Console_buffer1.ConsoleDatabufferprocessed == false))
{
						
	for( tempcount=0,temp=0;temp < Cl_Console_buffer1.Console_databuffer_write_idx;tempcount ++,temp++ )
	{
		Cl_Console_msgbuffer.Console_databuffer[tempcount]=Cl_Console_buffer1.Console_databuffer[temp];
							

	}
	if(!Cl_Console_buffer1.Data_incomplete)
	{
			Cl_Console_msgbuffer.ConsoleNewMessageReady = true;
	}
			
			Cl_Console_buffer1.ConsoleDatabufferprocessed = true;
				if (Cl_Console_buffer1.Console_databuffer[0] == 0x05)
				{
		//			printf("p");
				}
			Cl_Console_buffer1.ConsoleDatabufferReady=false;
			Cl_Console_buffer1.StartSequenceDetected = false;
			Cl_Console_buffer1.Console_databuffer_write_idx =0;
			Cl_Console_buffer1.BufferEmpty = true;
	
}
if((Cl_Console_buffer2.ConsoleDatabufferReady == true) && (Cl_Console_buffer2.ConsoleDatabufferprocessed == false))
{
	if(!Cl_Console_buffer1.Data_incomplete)
	{
		tempcount=0;
	}
	else
	{
		// carry on appending to prevdata.
	}
	for( temp=0;temp < Cl_Console_buffer2.Console_databuffer_write_idx;tempcount ++,temp++ )
	{
		Cl_Console_msgbuffer.Console_databuffer[tempcount]=Cl_Console_buffer2.Console_databuffer[temp];

	}
		if(!Cl_Console_buffer2.Data_incomplete)
		{
			Cl_Console_msgbuffer.ConsoleNewMessageReady = true;
		}
	
		
			Cl_Console_buffer2.ConsoleDatabufferprocessed = true;
			Cl_Console_buffer2.ConsoleDatabufferReady=false;
			Cl_Console_buffer2.Console_databuffer_write_idx =0;
			Cl_Console_buffer2.StartSequenceDetected = false;
			Cl_Console_buffer2.BufferEmpty = true;
			if(Cl_Console_buffer1.Data_incomplete)
			{
				Cl_Console_buffer1.ConsoleDatabufferprocessed = true;
				Cl_Console_buffer1.ConsoleDatabufferReady=false;
				Cl_Console_buffer1.StartSequenceDetected = false;
				Cl_Console_buffer1.Data_incomplete = false;
				Cl_Console_buffer2.Data_incomplete = false;
				Cl_Console_buffer1.Console_databuffer_write_idx =0;
				Cl_Console_buffer1.BufferEmpty = true;
			}
}


	return CL_OK;
}
Cl_ReturnCodes		 Cl_MacWrite_Message_Console(void)
{
	return CL_ACCEPTED;
}

Cl_ReturnCodes	 Cl_Mac_Create_ConsoleEvent(void)
{
//	uint8_t start1,start2,command,datasize,data1,data2,end1,end2;
	//Cl_ConsoleMsgType Cl_ConsoleMsg;
	static uint8_t* Cl_cmdptr;
	uint8_t tempcount,packet_size =0, *dataptr= NULL, *msgdataptr = NULL;
	MAC_EVENTS MacEvent;
	//printf("#/n");
	
	
			
			Cl_cmdptr = (uint8_t*)&Cl_Console_msgbuffer.Console_databuffer;
				
			Cl_ConsoleRxMsg.command = Cl_Console_msgbuffer.Console_databuffer[0];
			Cl_ConsoleRxMsg.datasize = Cl_Console_msgbuffer.Console_databuffer[1];
			msgdataptr = &Cl_Console_msgbuffer.Console_databuffer[2];
			dataptr = &Cl_ConsoleRxMsg.data.byte[0];
			for(tempcount=0;tempcount<Cl_ConsoleRxMsg.datasize;tempcount++)
			{
				 *dataptr = *msgdataptr ;
				 dataptr++;
				 msgdataptr++;
			
			}
			

						

			if(Cl_ConsoleRxMsg.command != CON_RX_COMMAND_ACK)
			{
			//	test_count--;
			//	if(test_count  != 0)
			//	{
			//		PIOC->PIO_SODR = 1 << 28; //test
			//	}
				Cl_Cosnsole_MsgAcknlowledge();
				test_counter++;
				if (test_counter >= 1000)
				{

				//	DD_SET_PIO(PD,7); // test
				//	PIOB->PIO_SODR = 1 << 25; //test
				//	PIOC->PIO_CODR = 1 << 29; //test
					test_counter = 0;
				}
				else
				if(test_counter > 2)
				{
					//DD_RESET_PIO(PD,7); // test
				//	PIOC->PIO_SODR = 1 << 29; //test
				}
			}
			else
			{
				// do nothing
				//	return CL_OK;
			}

			 if(Cl_ConsoleRxMsg.command == CON_RX_COMMAND_TEST_HARNESS)
			 		{
					
					cl_testharnesscommandhandler(&Cl_ConsoleRxMsg);	 
					}
			else
			{
			
			Cl_ConsoleRxMsg.msgready = true;
			Cl_Console_msgbuffer.ConsoleNewMessageReady = false;
		
			Cl_Console_ConvertConsoleEvent_toMacEvent(Cl_ConsoleRxMsg.command,&MacEvent);
			Cl_MacEvent = MacEvent;
			Cl_MacNewEvent = true;
			if( Cl_MacEvent3.Cl_MacNewEventcount <= MAC_EVENT_COUNT_MAX )	
			 {
				 Cl_MacEvent3.Cl_MacEvent[Cl_MacEvent3.Cl_MacNewEventcount] = MacEvent ;
				 Cl_MacEvent3.Cl_MacNewEvent = true;
				 Cl_MacEvent3.Cl_MacNewEventcount++;
				 
				msgdataptr = &Cl_Console_msgbuffer.Console_databuffer[2];
				dataptr = &Cl_MacEvent3.Cl_MacEventData[Cl_MacEvent3.Cl_MacNewEventcount][0];
				for(tempcount=0;tempcount<Cl_ConsoleRxMsg.datasize;tempcount++)
				{
					*dataptr = *msgdataptr ;
					dataptr++;
					msgdataptr++;
					 			
				}
				 			
			 }
			 
				
			}
			
	if(syncdone)
	{
		
	
	
			Cl_IIC_ConsoleTxMsg[0] = 0x99 ;
	
			Cl_IIC_ConsoleTxMsg[1] = 0xCC ;
			Cl_IIC_ConsoleTxMsg[2] = 0xDD ;
			msgdataptr = &Cl_Console_msgbuffer.Console_databuffer[0];
			for(tempcount = 0 ; tempcount< Cl_ConsoleRxMsg.datasize+3;tempcount++)
			{
				Cl_IIC_ConsoleTxMsg[tempcount+3] = *msgdataptr++;
		
			}
			//Cl_IIC_ConsoleTxMsg[tempcount+3] = 0xEE;
			Cl_IIC_ConsoleTxMsg[tempcount+3] = 0xBB;
			packet_size = Cl_ConsoleRxMsg.datasize + 7;
			{
			//	if ( (command != CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT) && (command != CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT) &&(command != CON_TX_COMMAND_PRINTDATA) && (command != CON_TX_COMMAND_PRINTTEXT))
				{
			//		DD_IIC_CONSOLE_SEND(0x77, &Cl_IIC_ConsoleTxMsg, packet_size);
				}
		
			}

			 		
	}
		
		return CL_OK;
}



Cl_ReturnCodes memcopy(uint8_t* dest, uint8_t* source, uint32_t size)
{
	uint32_t count = 0;
	for (count = 0;count< size; count++ )
	{
		dest[count]= source[count];
	}
	
	return 1;
}

Cl_ReturnCodes Cl_increment_msgbufferindex()
{
	
	if(Cl_Console_msgbuffer.Console_databuffer_write_idx < CONSOLE_BUFFER_SIZE)
	Cl_Console_msgbuffer.Console_databuffer_write_idx++;
	else
	Cl_Console_msgbuffer.Console_databuffer_write_idx = 0;
	return 1;
}

Cl_ReturnCodes Cl_increment_consolebufferindex()
{
	
	if(Cl_Console_buffer.Console_databuffer_read_idx < CONSOLE_BUFFER_SIZE)
	Cl_Console_buffer.Console_databuffer_read_idx++;
	else
	Cl_Console_buffer.Console_databuffer_read_idx = 0;
		return 1;
}

Cl_ReturnCodes Cl_Cosnsole_MsgAcknlowledge()
   
	{
		uint8_t temp;

		cl_wait(1);
		Cl_ConsoleTxMsg[0] =	CL_CONSOLEMSG_STARTBYTE1;
		Cl_ConsoleTxMsg[1] =	CL_CONSOLEMSG_STARTBYTE2;
		Cl_ConsoleTxMsg[2]=		CON_TX_COMMAND_ACK;
		Cl_ConsoleTxMsg[3] = 1; //size of data
	//	Cl_ConsoleTxMsg[2]= 170;

		Cl_ConsoleTxMsg[4]= Cl_ConsoleRxMsg.command;
	//	Cl_ConsoleTxMsg[4]= CON_START_RINSE;
		Cl_ConsoleTxMsg[5] =	CL_CONSOLEMSG_STOPBYTE1;
		Cl_ConsoleTxMsg[6] =	CL_CONSOLEMSG_STOPBYTE2;
		for (temp=0;temp<7;)
		{
			
		//if(temp == 0)
		//{
		//		PIOC->PIO_SODR = 1 << 29; //test
		//}
	//	else if (temp == ( 6 ))
	//	{
	//			PIOC->PIO_CODR = 1 << 29; //test
	//	}
		
			
			
			if(! uart_write(CONF_UART,Cl_ConsoleTxMsg[temp]))
			{
				temp++;
				
				}
				
			
		//	printf("%s",Cl_ConsoleTxMsg);
		}
	return CL_OK;
}
Cl_ReturnCodes Cl_SendDatatoconsole_dummy(Cl_ConsoleTxCommandtype command, uint8_t* data,uint8_t datasize)
{
	uint8_t   temp =4 ;
	uint8_t packet_size = 0;
	uint8_t *dataptr=NULL, *msgdataptr =NULL;
	
	*dataptr = *data;
	packet_size = datasize + 6;
	msgdataptr = &Cl_ConsoleTxMsg[4];
	Cl_ConsoleTxMsg[0] = CL_CONSOLEMSG_STARTBYTE1 ;//(uint8_t) 'X';
	Cl_ConsoleTxMsg[1] = CL_CONSOLEMSG_STARTBYTE2 ;// (uint8_t)'Q';
	Cl_ConsoleTxMsg[2]= command;// command
	Cl_ConsoleTxMsg[3] = datasize; //size of data
	//	Cl_ConsoleTxMsg[2]= 170;
	Cl_ConsoleTxMsg[temp++] = CL_CONSOLEMSG_STOPBYTE1; // (uint8_t)'D';
	Cl_ConsoleTxMsg[temp++] = CL_CONSOLEMSG_STOPBYTE2 ;// (uint8_t)'F';

//	printf("\n");
	return CL_OK;
	
}
Cl_ReturnCodes Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype command, uint8_t* data,uint8_t datasize)
{
	uint8_t tempcount=0 , temp =4 ;
	uint8_t packet_size = 0;
	uint8_t *dataptr=NULL, *msgdataptr =NULL;
	static uint8_t testcnt =0;
	


		
			if(( CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT == command)|| ( CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT == command) ||( command == CON_TX_COMMAND_PRINTDATA) ||(command == CON_TX_COMMAND_PRINTTEXT))
			{
				return;
			}
		
	if( ( command != CON_TX_COMMAND_PRINTDATA) && (command != CON_TX_COMMAND_PRINTTEXT) )
	{
	//	cmd_backup.cmdtype[cmd_backup.array_commands_index].commands_inouttype = 1;
	//	cmd_backup.cmdtype[cmd_backup.array_commands_index].command =  command;
	//	cmd_backup.array_commands_index++;
	//	if(cmd_backup.array_commands_index == 100)
		{
	//		cmd_backup.array_commands_index = 0;
		}
		
		
	}
	
//	if (CON_TX_COMMAND_ALARM == command )
//	{
// if ( (*data == 5) || (*data == 6))
// {
//	 Cl_SendDatatoconsoleDummycommand(CON_TX_COMMAND_PRINTTEXT,"H",1);
// }
//	}
	
	dataptr = data;
	packet_size = datasize + 6;
	msgdataptr = &Cl_ConsoleTxMsg[4];
	Cl_ConsoleTxMsg[0] = CL_CONSOLEMSG_STARTBYTE1 ;//(uint8_t) 'X';
	Cl_ConsoleTxMsg[1] = CL_CONSOLEMSG_STARTBYTE2 ;// (uint8_t)'Q';
	Cl_ConsoleTxMsg[2]= command;// command
	Cl_ConsoleTxMsg[3] = datasize; //size of data
	//	Cl_ConsoleTxMsg[2]= 170;
	for(tempcount = 0 ; tempcount< datasize;tempcount++)
	{
		Cl_ConsoleTxMsg[temp] = data[tempcount];
		temp++;
	}
	
	Cl_ConsoleTxMsg[temp++] = CL_CONSOLEMSG_STOPBYTE1; // (uint8_t)'D';
	Cl_ConsoleTxMsg[temp++] = CL_CONSOLEMSG_STOPBYTE2 ;// (uint8_t)'F';
	
		if(syncdone)
		{
			
			
			Cl_IIC_ConsoleTxMsg[0] = 0xAA ;
			for(tempcount = 0 ; tempcount< packet_size;tempcount++)
			{
				Cl_IIC_ConsoleTxMsg[tempcount+1] = Cl_ConsoleTxMsg[tempcount];
				
			}
			
			{
				//if ( (command != CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT) && (command != CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT) &&(command != CON_TX_COMMAND_PRINTDATA) && (command != CON_TX_COMMAND_PRINTTEXT))
				//if ( (command != CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT) && (command != CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT) &&(command != CON_TX_COMMAND_PRINTDATA) && (command != CON_TX_COMMAND_PRINTTEXT))
				if ( (command != CON_TX_COMMAND_COMMAND_IIC_HANGUP) )
				{
				//	DD_IIC_CONSOLE_SEND(0x77, &Cl_IIC_ConsoleTxMsg, packet_size+1);
				}

			}
			
			
		}
		
		
	for (tempcount=0;tempcount<packet_size;)
	{
	//	if(tempcount == 0)
	//	{
	//			PIOC->PIO_SODR = 1 << 29; //test
	//	}
	//	else if (tempcount == ( packet_size - 1 ))
	//	{
	//			PIOC->PIO_CODR = 1 << 29; //test
	//	}
		
	
	//	if(! uart_write(CONF_UART,Cl_ConsoleTxMsg[tempcount]))
	if(! sv_console_bytedatawrite(Cl_ConsoleTxMsg[tempcount]))
	
		{
			tempcount++;
			
		}
		
		
		//	printf("%s",Cl_ConsoleTxMsg);
		//printf("\n");
		
	}
	

//	printf("\n");
	return CL_OK;
}


Cl_ReturnCodes Cl_SendDatatoconsoleDummycommand(Cl_ConsoleTxCommandtype command, uint8_t* data,uint8_t datasize)
{
	uint8_t tempcount=0 , temp =4 ;
	uint8_t packet_size = 0;
	uint8_t *dataptr=NULL, *msgdataptr =NULL;
	
	dataptr = data;
	packet_size = 8;
	msgdataptr = &Cl_ConsoleTxMsg[4];
	Cl_ConsoleTxMsg[0] = command ;//(uint8_t) 'X';
	Cl_ConsoleTxMsg[1] = CL_CONSOLEMSG_STARTBYTE1 ;//(uint8_t) 'X';
	Cl_ConsoleTxMsg[2] = CL_CONSOLEMSG_STARTBYTE2 ;// (uint8_t)'Q';
	if( command == 0xAA)
	{
		Cl_ConsoleTxMsg[3]=  0x66;
	}
	else
	{
		Cl_ConsoleTxMsg[3]=  0x77;
	}
	
	Cl_ConsoleTxMsg[4] = 0x01; //size of data
	Cl_ConsoleTxMsg[5] = command + 0x01;
	Cl_ConsoleTxMsg[6] = CL_CONSOLEMSG_STOPBYTE1; // (uint8_t)'D';
	Cl_ConsoleTxMsg[7] = CL_CONSOLEMSG_STOPBYTE2 ;// (uint8_t)'F';
	for (tempcount=0;tempcount<packet_size;)
	{
		if(! uart_write(CONF_UART,Cl_ConsoleTxMsg[tempcount]))
		{
			tempcount++;
			
		}
		
		
		//	printf("%s",Cl_ConsoleTxMsg);
		//printf("\n");
		
	}
	//printf("\n");
	return CL_OK;
}
Cl_ReturnCodes Cl_Console_ConvertConsoleEvent_toMacEvent(Cl_ConsoleRxEventsType command ,MAC_EVENTS* MacEvent)
{
//	cmd_backup.cmdtype[cmd_backup.array_commands_index].commands_inouttype = 0;
//	cmd_backup.cmdtype[cmd_backup.array_commands_index].command =  command;
		
		

//	cmd_backup.array_commands_index++;
//	if(cmd_backup.array_commands_index == 100)
	{
//		cmd_backup.array_commands_index = 0;
	}
	switch(command)
	{
		case CON_RX_COMMAND_SYNC:
		//printf("J/n");
		    *MacEvent = EVT_CONSOLE_COMMAND_SYNC;
		break;
		case CON_RX_COMMAND_ACK:
		  *MacEvent = EVT_CONSOLE_COMMAND_ACK;
		break;
		case CON_RX_COMMAND_RINSE_START:
//		printf("J/n");
			*MacEvent = EVT_CONSOLE_COMMAND_RINSE_START;
		break;
		case CON_RX_EVT_COMMAND_START_DISINF:
		
		*MacEvent = EVT_CONSOLE_COMMAND_DISINF_START;
		break;
		case CON_RX_EVT_COMMAND_RESUME_DISINF_STAGE:
		*MacEvent = EVT_CONSOLE_RESUME_DISINF_STAGE;
		break;
		case CON_RX_COMMAND_SET_BLDPMP_ON:
		*MacEvent = EVT_CONSOLE_COMMAND_SET_BLDPMP_ON;
		break;
		case CON_RX_COMMAND_SET_BLDPMP_OFF:
		*MacEvent = EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF;
		break;
		case CON_RX_COMMAND_SET_BLDPUMPRATE:
		*MacEvent = EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE;
		break;
		case CON_RX_COMMAND_START_DIALYSATE_FILL:
		*MacEvent = EVT_CONSOLE_COMMAND_START_DIALYSATE_FILLING;
		break;
		case CON_RX_COMMAND_SET_DATA:
		*MacEvent = EVT_CONSOLE_COMMAND_SET_DATA;
		break;
		case CON_RX_COMMAND_GET_DATA:
		*MacEvent = EVT_CONSOLE_COMMAND_GET_DATA;
		break;
		case CON_RX_COMMAND_START_PRIME:
		*MacEvent = EVT_CONSOLE_COMMAND_START_PRIME;
		break;
		
		case CON_RX_COMMAND_STOP_PRIME:
		*MacEvent = EVT_CONSOLE_COMMAND_STOP_PRIME;
		break;
		
		case CON_RX_COMMAND_START_DIALISER_PRIME:
		*MacEvent = EVT_CONSOLE_COMMAND_START_DIALISER_PRIME;
		break;
		
		case CON_RX_EVT_COMMAND_STOP_DIALYSER_PRIME:
		*MacEvent = EVT_CONSOLE_COMMAND_STOP_DIALISER_PRIME;
		break;
		case CON_RX_COMMAND_HEP_PMP_SET:
		*MacEvent = EVT_CONSOLE_COMMAND_HEP_PMP_SET;
		
		break;
		case CON_RX_COMMAND_DIALYSIS_PREP:
			*MacEvent = EVT_CONSOLE_COMMAND_DIALYSIS_PREP;
		break;
		
		case CON_RX_COMMAND_DIALYSER_CONNECTED:
		*MacEvent = EVT_CONSOLE_COMMAND_DILYSER_CONNECTED;
		break;
		case CON_RX_COMMAND_MAN_PREP_COMPLETED:
			*MacEvent = EVT_CONSOLE_COMMAND_MAN_PREP_COMPLETED;
		break;
		case CON_RX_COMMAND_DIALYSIS_START:
			*MacEvent = EVT_CONSOLE_COMMAND_DIALYSIS_START;
		break;
		case CON_RX_COMMAND_DIALYSIS_STOP:
			*MacEvent = EVT_CONSOLE_COMMAND_DIALYSIS_STOP;
		break;
		case CON_RX_COMMAND_DIALYSIS_PAUSE:
			*MacEvent = EVT_CONSOLE_COMMAND_DIALYSIS_PAUSE;
		break;
		
		case CON_RX_COMMAND_DIALYSIS_BYPASS:
			*MacEvent = EVT_CONSOLE_COMMAND_DIALYSIS_BYPASS;
		break;
		case CON_RX_COMMAND_RINSE_STOP:
			*MacEvent = EVT_CONSOLE_COMMAND_STOP_RINSE;
		break;
		
		case		CON_RX_COMMAND_USER_MODE_ENTER://1A
					*MacEvent =  EVT_CONSOLE_COMMAND_USER_MODE_ENTER;
		break;
		case		CON_RX_COMMAND_USER_MODE_EXIT://1B
					*MacEvent =  EVT_CONSOLE_COMMAND_USER_MODE_EXIT;
		break;
		case		CON_RX_COMMAND_SERVICEMODE_ENTER://1C
					*MacEvent =  EVT_CONSOLE_COMMAND_SERVICEMODE_ENTER ;
		break;
		case		CON_RX_COMMAND_SERVICEMODE_EXIT://1D
					*MacEvent = EVT_CONSOLE_COMMAND_SERVICEMODE_EXIT;
		break;
					
		case    CON_RX_EVT_COMMAND_PATIENT_CONNECTED: //1E
					*MacEvent = EVT_CONSOLE_COMMAND_PATIENT_CONNECTED;
		break;
		case 	CON_RX_EVT_COMMAND_MIXING_PREP_START://0x21
					*MacEvent = EVT_SEND_MIXING_PREP_START;
		break;
		case	CON_RX_EVT_COMMAND_MIXING_PREP_STOP://0x22
			*MacEvent = EVT_SEND_MIXING_PREP_STOP;
		break;

		case CON_RX_EVT_COMMAND_RCIRC_START: 
					*MacEvent = EVT_CONSOLE_COMMAND_RCIRC_START;
		break;
		case CON_RX_EVT_COMMAND_RCIRC_STOP: 
					*MacEvent = EVT_CONSOLE_COMMAND_RCIRC_STOP;
		break;
		case CON_RX_EVT_COMMAND_PATIENT_READY:
		*MacEvent = EVT_CONSOLE_COMMAND_PATIENT_READY;
		break;
		default:
		break;
	}
	return CL_OK;
}

Cl_ReturnCodes	 Cl_ConsoleStateHandler(MAC_EVENTS Cl_MacRinseEvent)
{
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_ConsoleRxEventsType cl_console_event = CON_RX_COMMANDMAX;
	uint8_t data =0;
	cl_console_translatemacevent( Cl_MacRinseEvent, cl_console_event);
	switch (Cl_ConsoleState)
	{
		case CON_STATE_INIT:
		switch(Cl_MacRinseEvent)
		{
			case CON_RX_COMMAND_SYNC:
			command = CON_TX_COMMAND_SYNC;
			 Cl_SendDatatoconsole(command,&data,0);
			break;
			default:
			break;
		}
		break;
		case CON_STATE_IDLE:
		break;
		case CON_STATE_ACTIVE:
			case CON_RX_COMMAND_SYNC:
			command = CON_TX_COMMAND_SYNC;
			Cl_SendDatatoconsole(command,&data,0);
			break;
		default:
		break;
	}
	
	return CL_OK;
}

Cl_ReturnCodes  cl_console_translatemacevent(MAC_EVENTS Cl_MacRinseEvent,Cl_ConsoleRxEventsType cl_consoleeevent)
{
	switch(Cl_MacRinseEvent)
	{
		case EVT_CONSOLE_COMMAND_SYNC:
				cl_consoleeevent= CON_RX_COMMAND_SYNC;
		break;
		case EVT_CONSOLE_COMMAND_SET_DATA:
				cl_consoleeevent=CON_RX_COMMAND_SET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_GET_DATA:
				cl_consoleeevent=CON_RX_COMMAND_GET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_RINSE_START:
				cl_consoleeevent=CON_RX_COMMAND_RINSE_START;
		break;
		case EVT_CONSOLE_COMMAND_DIALYSIS_PREP:
				cl_consoleeevent=CON_RX_COMMAND_DIALYSIS_PREP;
		break;

		case EVT_CONSOLE_COMMAND_DIALYSIS_START:
				cl_consoleeevent=CON_RX_COMMAND_DIALYSIS_START;
		break;


		
		default:
		break;
	}
	return CL_OK;
}

Cl_ReturnCodes 	Cl_Console_Console_Sync(void)
{
	Cl_ReturnCodes Cl_MacRetval = CL_ERROR;
	Cl_ConsoleSyncStatesType Cl_SyncState = CON_SYNC_STATE_WAIT_FOR_SYNC;
	
	uint8_t CmdData =0;
	

	while(	Cl_ConsoleState == CON_STATE_INIT )
	{
		switch(Cl_SyncState)
		{
			case CON_SYNC_STATE_WAIT_FOR_SYNC:
			
			Cl_MacRetval = Cl_Console_SyncGetMessage();

						if(Cl_MacRetval== CL_OK)
						{							
							Cl_ConsoleRxMsg.command = Cl_Console_msgbuffer.Console_databuffer[0];
							//Cl_ConsoleRxMsg.command = CON_HELLO;// for testing
							if(Cl_ConsoleRxMsg.command == CON_RX_COMMAND_SYNC )
							{
								
									Cl_Cosnsole_MsgAcknlowledge();
							//		loopackflag = true;
								
								
							//	count = 0;

								Cl_ConsoleRxMsg.msgready = true;
								Cl_Console_msgbuffer.ConsoleNewMessageReady = false;
								Cl_SyncState = CON_SYNC_STATE_SYNC_RECVD;
								//delay and send sync
								
							
							}
						}		
			Cl_MacRetval = CL_WAIT;
								
			break;
			case CON_SYNC_STATE_SYNC_RECVD:
				cl_wait(10);
				Cl_SendDatatoconsole( CON_TX_COMMAND_SYNC,&CmdData ,0);
				Cl_SyncState = CON_SYNC_STATE_WAIT_FOR_SYNC_ACK;
			Cl_MacRetval = CL_WAIT;
				break;
				
			case CON_SYNC_STATE_WAIT_FOR_SYNC_ACK:
			Cl_MacRetval = Cl_Console_SyncGetMessage();
				if(Cl_MacRetval== CL_OK)
				{
					Cl_ConsoleRxMsg.command = Cl_Console_msgbuffer.Console_databuffer[0];
					Cl_ConsoleRxMsg.datasize = Cl_Console_msgbuffer.Console_databuffer[1];
					Cl_ConsoleRxMsg.data.byte[0] = Cl_Console_msgbuffer.Console_databuffer[2];
					if(Cl_ConsoleRxMsg.command == CON_RX_COMMAND_ACK )
					{
						if(Cl_ConsoleRxMsg.data.byte[0] == CON_TX_COMMAND_SYNC )
						{
							Cl_SyncState = CON_SYNC_STATE_SYNC_ACK_RECVD;

							if( Cl_MacEvent3.Cl_MacNewEventcount <= MAC_EVENT_COUNT_MAX )	
							 {
								// Cl_MacEvent3.Cl_MacEvent[Cl_MacEvent3.Cl_MacNewEventcount] = EVT_COMMAND_ENTER_STANDBY ;
								Cl_MacEvent3.Cl_MacEvent[Cl_MacEvent3.Cl_MacNewEventcount] = EVT_COMMAND_ENTER_INIT ;
								
								 Cl_MacEvent3.Cl_MacNewEvent = true;
								 Cl_MacEvent3.Cl_MacNewEventcount++;
							 }
							 
							 syncdone = true; //testing
							Cl_ConsoleState = CON_STATE_ACTIVE;
							Cl_MacRetval = CL_OK;
							
							test_count = 0; //test
														
						}
					}
					else
					{
							Cl_MacRetval = CL_WAIT;
					}
				}
			break;
			case CON_SYNC_STATE_SYNC_ACK_RECVD:
			
				Cl_ConsoleState = CON_STATE_ACTIVE;
				Cl_MacRetval = CL_OK;
			break;
			
			default:
			break;
		}
	}
	/******************************************************************/

	return Cl_MacRetval;
 }
 
 
 Cl_ReturnCodes Cl_Console_init(void)
 {
	 Cl_ConsoleState = CON_STATE_INIT;
	 
	 return CL_OK;
 }
 
 
  Cl_ReturnCodes Cl_Console_SyncGetMessage()
  {
	 Cl_ReturnCodes  Cl_ConsoleRetval1 = CL_WAIT;
	 
 				Cl_ConsoleRetval1 = Cl_Console_Read_ConsoleMessage_Status(); // Reads the status of the data buffer populated by the timer console read interrupt.
 				if(Cl_ConsoleRetval1 == CL_OK)
 				{
	 				Cl_ConsoleRetval1 = Cl_Console_Read_Message_Console();
	 				if(Cl_ConsoleRetval1 == CL_OK)
	 				{
		 				Cl_ConsoleRetval1 = Cl_Console_Read_Message_Ready_Status(); // Reads the status of the data buffer populated by the timer console read interrupt.
					}
				 }
		return Cl_ConsoleRetval1;
				 
  }
  
