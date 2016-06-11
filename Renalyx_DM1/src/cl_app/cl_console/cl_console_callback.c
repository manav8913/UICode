/*
 * cl_console_callback.c
 *
 * Created: 12/30/2013 3:21:02 PM
 *  Author: sanjeer_k
 */ 
#include "cl_app/inc/cl_types.h"
#include "inc/cl_consolecontroller.h"
extern cl_console_bufferType Cl_Console_buffer,Cl_Console_buffer1,Cl_Console_buffer2;
extern  uint8_t testbool ;
extern int8_t test_count;
//Cl_ReturnCodes  cl_console_copy_buffer_from_uart(uint8_t);
Cl_ReturnCodes  cl_console_receivedbyte(uint8_t console_databyte);

//Cl_ReturnCodes  cl_console_copy_buffer_from_uart(uint8_t console_databyte)
Cl_ReturnCodes  cl_console_receivedbyte(uint8_t console_databyte)
{
	static uint8_t prev_databyte =0 ;
	static unsigned int temp = 0;
//	printf("1");

								if (console_databyte == 0xbb)
								{
							//		printf("d");
									temp = 10;
								}
								if((console_databyte == 0xcc) && (temp == 10) )
								{

							//		printf("g")	;								;
								}
	if(Cl_Console_buffer1.BufferEmpty)
	{
	//	printf("2");
		if(Cl_Console_buffer2.BufferEmpty)
			{
		//		printf("3");
				//if both buffers are empty always use buffer 1 for copying.(if previous fill was complete esle use 2nd buffer)
			
				Cl_Console_buffer1.CurreltyActive = true;
				Cl_Console_buffer2.CurreltyActive = false;					
			}
		
	}


	//Cl_Console_buffer1.CurreltyActive = false;//testing
	//Cl_Console_buffer2.CurreltyActive = true;//testing
	
	if(Cl_Console_buffer1.CurreltyActive == true )
	{
	//	printf("6");
		// Write to the buffer only when buffer is currently active and not full or packet ready
		
								
				if(Cl_Console_buffer1.StartSequenceDetected == false)
				{
					//printf("7");
				
					//		if(((char)prev_databyte == 'X') && ((char)console_databyte == 'Q'))
					if((prev_databyte == CL_CONSOLEMSG_STARTBYTE1) && (console_databyte == CL_CONSOLEMSG_STARTBYTE2))
							{
						//		printf("8");
								Cl_Console_buffer1.StartSequenceDetected = true;
								//	PIOB->PIO_SODR = 1 << 25; //test
							}
				}
				else
				{
		//			printf("9");
							Cl_Console_buffer1.Console_databuffer[Cl_Console_buffer1.Console_databuffer_write_idx] = console_databyte;
							//printf("%c",Cl_Console_buffer.Console_databuffer[Cl_Console_buffer.Console_databuffer_write_idx] );
							Cl_Console_buffer1.ConsoleNewData = true;
							Cl_Console_buffer1.BufferEmpty = false;
							//if(((char)prev_databyte == 'D') && ((char)console_databyte == 'F'))
							if((prev_databyte == CL_CONSOLEMSG_STOPBYTE1) && (console_databyte == CL_CONSOLEMSG_STOPBYTE2))
							{
								
							//		PIOB->PIO_CODR = 1 << 25; //test

								Cl_Console_buffer1.ConsoleDatabufferReady = true;
								Cl_Console_buffer1.ConsoleDatabufferprocessed = false;
								Cl_Console_buffer1.Data_incomplete = false;
								Cl_Console_buffer1.CurreltyActive = false;
								Cl_Console_buffer2.CurreltyActive =true;
								
								//	test_count++;
							}
				
							Cl_Console_buffer1.Console_databuffer_write_idx++;
					
				}
				
				if(Cl_Console_buffer1.Console_databuffer_write_idx >= CONSOLE_BUFFER_SIZE)
				{
					//Cl_Console_buffer1.Console_databuffer_write_idx = 0;
					Cl_Console_buffer1.ConsoleDatabufferReady = true ;
						Cl_Console_buffer1.ConsoleDatabufferprocessed = false;
					Cl_Console_buffer1.CurreltyActive = false;
					Cl_Console_buffer1.Data_incomplete = true;
					
				}

		//testbool = 1;
		//	uart_write(CONF_UART, console_databyte);
		//	printf("console calllback");
		//printf("%c",console_databyte);
		//printf("= %d\n",testbool);

		//	printf("w= %d\n",Cl_Console_buffer.Console_databuffer_write_idx);
		//		printf("r= %d\n",Cl_Console_buffer.Console_databuffer_read_idx);
			prev_databyte = console_databyte; // we may have to compare start and stop sequences
			
				return CL_ACCEPTED; // if we have just filled 
	}
	
	if(Cl_Console_buffer2.CurreltyActive == true )
	{
		if(Cl_Console_buffer1.Data_incomplete == true)
		{
			Cl_Console_buffer2.StartSequenceDetected = true;
		}
				
		// Write to the buffer only when buffer is currently active and not full or packet ready
	//	prev_databyte = console_databyte; // we may have to compare start and stop sequences
		
		
		if(Cl_Console_buffer2.StartSequenceDetected == false)
		{
			
		//	if(((char)prev_databyte == CL_CONSOLEMSG_STARTBYTE1) && ((char)console_databyte == CL_CONSOLEMSG_STARTBYTE2))
			if((prev_databyte == CL_CONSOLEMSG_STARTBYTE1) && (console_databyte == CL_CONSOLEMSG_STARTBYTE2))
			{
				Cl_Console_buffer2.StartSequenceDetected = true;
				Cl_Console_buffer2.Console_databuffer_write_idx = 0;
			}
		}
		else
		{
			Cl_Console_buffer2.Console_databuffer[Cl_Console_buffer2.Console_databuffer_write_idx] = console_databyte;
			//printf("%c",Cl_Console_buffer.Console_databuffer[Cl_Console_buffer.Console_databuffer_write_idx] );
			Cl_Console_buffer2.ConsoleNewData = true;
			Cl_Console_buffer2.BufferEmpty = false;
		//	if(((char)prev_databyte == 'D') && ((char)console_databyte == 'F'))
		if((prev_databyte == CL_CONSOLEMSG_STOPBYTE1) && (console_databyte == CL_CONSOLEMSG_STOPBYTE2))
			{
				Cl_Console_buffer2.ConsoleDatabufferReady = true;
				Cl_Console_buffer2.ConsoleDatabufferprocessed = false;
				Cl_Console_buffer1.CurreltyActive = true;
				Cl_Console_buffer2.CurreltyActive = false;

			}
			//	Cl_Console_buffer2.Data_incomplete = false;
			//	Cl_Console_buffer1.Data_incomplete = false;
			Cl_Console_buffer2.Console_databuffer_write_idx++;
				
		}
			
			
			
		
		
		if(Cl_Console_buffer2.Console_databuffer_write_idx > CONSOLE_BUFFER_SIZE)
		{
			//Cl_Console_buffer2.Console_databuffer_write_idx = 0;
			Cl_Console_buffer2.ConsoleDatabufferReady = true ;
			Cl_Console_buffer2.CurreltyActive = false;
			Cl_Console_buffer2.Data_incomplete = true;
			Cl_Console_buffer1.Data_incomplete = false;
			
		}

	if (Cl_Console_buffer2.Data_incomplete && Cl_Console_buffer1.Data_incomplete )
	{
		//printf("buffer overflow\n");
	}
		//testbool = 1;
		//	uart_write(CONF_UART, console_databyte);
		//	printf("console calllback");
		//printf("%c",console_databyte);
		//printf("= %d\n",testbool);

		//	printf("w= %d\n",Cl_Console_buffer.Console_databuffer_write_idx);
		//		printf("r= %d\n",Cl_Console_buffer.Console_databuffer_read_idx);
			prev_databyte = console_databyte; // we may have to compare start and stop sequences
	}
	
	return CL_ACCEPTED;
}
