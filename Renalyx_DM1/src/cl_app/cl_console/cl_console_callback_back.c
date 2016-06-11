/*
 * cl_console_callback.c
 *
 * Created: 12/30/2013 3:21:02 PM
 *  Author: sanjeer_k
 */ 
#include "inc/cl_types.h"
extern cl_console_bufferType Cl_Console_buffer,Cl_Console_buffer1,Cl_Console_buffer2;
extern  uint8_t testbool ;
Cl_ReturnCodes  cl_console_copy_buffer_from_uart(uint8_t);

#if 0
Cl_ReturnCodes  cl_console_copy_buffer_from_uart(uint8_t console_databyte)
{
	
		if(Cl_Console_buffer.Console_databuffer_write_idx >= CONSOLE_BUFFER_SIZE)
		{
			Cl_Console_buffer.Console_databuffer_write_idx = 0;
		}
		Cl_Console_buffer.Console_databuffer[Cl_Console_buffer.Console_databuffer_write_idx] = console_databyte;
		//printf("%c",Cl_Console_buffer.Console_databuffer[Cl_Console_buffer.Console_databuffer_write_idx] );
		Cl_Console_buffer.Console_databuffer_write_idx++;
		if(Cl_Console_buffer.Console_databuffer_write_idx >= CONSOLE_BUFFER_SIZE)
		{
			Cl_Console_buffer.Console_databuffer_write_idx = 0;
		}
	
		Cl_Console_buffer.ConsoleNewData = true;
		//testbool = 1;
		//	uart_write(CONF_UART, console_databyte);
		//	printf("console calllback");
		//printf("%c",console_databyte);
		//printf("= %d\n",testbool);

	//	printf("w= %d\n",Cl_Console_buffer.Console_databuffer_write_idx);
//		printf("r= %d\n",Cl_Console_buffer.Console_databuffer_read_idx);
		
		return CL_ACCEPTED;
}

#endif
Cl_ReturnCodes  cl_console_copy_buffer_from_uart(uint8_t console_databyte)
{
	static uint8_t prev_databyte =0 ;
//	printf("1");
	if(Cl_Console_buffer1.BufferEmpty)
	{
	//	printf("2");
		if(Cl_Console_buffer2.BufferEmpty)
			{
		//		printf("3");
				//if both buffers are empty always use buffer 1 for copying.(if previous fill was complete esle use 2nd buffer)
				if(! Cl_Console_buffer1.Data_incomplete )
				{
				Cl_Console_buffer1.CurreltyActive = true;
				Cl_Console_buffer2.CurreltyActive = false;					
				}
				else
				{
				Cl_Console_buffer1.CurreltyActive = false;
				Cl_Console_buffer2.CurreltyActive = true;
					
				}

			}
		else if( Cl_Console_buffer2.ConsoleDatabufferReady)
			{
				// buffer 2 is not empty but it is data ready , so no further writing to buffer 2 from now on, instead write to buffer 1
				printf("4");
				Cl_Console_buffer1.CurreltyActive = true;
				Cl_Console_buffer2.CurreltyActive = false;

			}
		else
			{
				printf("5");
				// if buffer 1 is empty after read by app, but buffer 2 is not empty, then use buffer 2 till it is ready to read)
				Cl_Console_buffer2.CurreltyActive = true;
				Cl_Console_buffer1.CurreltyActive = false;
			}
		
	}
	else
	{
				printf("W");
		if ((Cl_Console_buffer1.ConsoleDatabufferReady == true )&&(Cl_Console_buffer1.ConsoleDatabufferprocessed == false))
		{
			
	
				Cl_Console_buffer2.CurreltyActive = true;
				Cl_Console_buffer1.CurreltyActive = false;
		}
		
	}

	
	
	
	if(Cl_Console_buffer1.CurreltyActive == true )
	{
	//	printf("6");
		// Write to the buffer only when buffer is currently active and not full or packet ready
		
								
				if(Cl_Console_buffer1.StartSequenceDetected == false)
				{
					printf("7");
				
							if(((char)prev_databyte == 'X') && ((char)console_databyte == 'Q'))
							{
								printf("8");
								Cl_Console_buffer1.StartSequenceDetected = true;
							}
				}
				else
				{
		//			printf("9");
							Cl_Console_buffer1.Console_databuffer[Cl_Console_buffer1.Console_databuffer_write_idx] = console_databyte;
							//printf("%c",Cl_Console_buffer.Console_databuffer[Cl_Console_buffer.Console_databuffer_write_idx] );
							Cl_Console_buffer1.ConsoleNewData = true;
							Cl_Console_buffer1.BufferEmpty = false;
							if(((char)prev_databyte == 'D') && ((char)console_databyte == 'F'))
							{
								Cl_Console_buffer1.ConsoleDatabufferReady = true;
								Cl_Console_buffer1.ConsoleDatabufferprocessed = false;
								Cl_Console_buffer1.Data_incomplete = false;
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
			
			if(((char)prev_databyte == 'X') && ((char)console_databyte == 'Q'))
			{
				Cl_Console_buffer2.StartSequenceDetected = true;
			}
		}
		else
		{
			Cl_Console_buffer2.Console_databuffer[Cl_Console_buffer2.Console_databuffer_write_idx] = console_databyte;
			//printf("%c",Cl_Console_buffer.Console_databuffer[Cl_Console_buffer.Console_databuffer_write_idx] );
			Cl_Console_buffer2.ConsoleNewData = true;
			Cl_Console_buffer2.BufferEmpty = false;
			if(((char)prev_databyte == 'D') && ((char)console_databyte == 'F'))
			{
				Cl_Console_buffer2.ConsoleDatabufferReady = true;
				Cl_Console_buffer2.ConsoleDatabufferprocessed = false;
			//	Cl_Console_buffer2.Data_incomplete = false;
			//	Cl_Console_buffer1.Data_incomplete = false;
				
			}
			
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
		printf("buffer overflow\n");
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
