/*
 * DD_GPIO.c
 *
 * Created: 1/6/2014 10:45:11 AM
 *  Author: Geekay
 */ 
#include "DD_GPIO.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
void DD_INIT_PIO()
{
	
	
	PIOA->PIO_WPMR = 0x50494F00;
	PIOA->PIO_PER  = 0x0E00A800;
	

	//PIOA->PIO_OER  = 0x0E1EA801;
	//PIOA->PIO_ODR  = 0x00004080;

	PIOA->PIO_OER  = 0x0E00A800;
	PIOA->PIO_PDR =  0x00060000;
	PIOA->PIO_PUER = 0x00060000;
	//PIOA->PIO_ODR  = 0xF1F9D7FF;
	PIOA->PIO_ODR  = 0xF1F957FF;
	PIOA->PIO_ABSR = 0x00090000;

	//PIOA->PIO_CODR = 0x0E002800;
	PIOA->PIO_CODR = 0x0E00A800;
	
	PIOB->PIO_WPMR = 0x50494F00;

	PIOB->PIO_PER	 = 0x06004000;

	
	//PIOB->PIO_OER  = 0x06104000;
	PIOB->PIO_OER  = 0x06004000;
	PIOB->PIO_ODR  = 0xF9FFBFFF;
	
	//PIOB->PIO_CODR = 0x00607000;
	PIOB->PIO_CODR = 0x06004000;
	

	PIOC->PIO_WPMR = 0x50494F00;
	//PIOC->PIO_PER  = 0x36083082;
	//PIOC->PIO_OER  = 0x36083082;
	PIOC->PIO_PER  = 0x360830C6;
	PIOC->PIO_OER  = 0x360830C6;
	PIOC->PIO_ODR  = 0xC9F7CF39;
	PIOC->PIO_CODR = 0x360830C6;
	
	PIOD->PIO_WPMR = 0x50494F00;
	
	PIOD->PIO_PER  = 0x000007C7;
	PIOD->PIO_OER  = 0x00000586;
	PIOD->PIO_ODR  = 0xFFFFFA79;
	PIOD->PIO_CODR = 0x00000586;
	
	
	
	//pio_set_input(PIOA, PIO_PA19, PIO_PULLUP); 
	
//	if ( PIOA->PIO_PUSR
	
}

uint8_t DD_DIS_PERI_AND_RESET_PIO(pio_type peripheral,uint32_t pio)
{
				if((peripheral == PD) && ((pio == 7)||(pio == 8) ))
				{
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PD_7_8",6);
				}
	switch (peripheral)
	{
		case PA:
		PIOA->PIO_PER = 1<<pio;
		PIOA->PIO_CODR = 1<<pio;
		break;
		case PB:
		PIOB->PIO_PER = 1<<pio;
		PIOB->PIO_CODR = 1<<pio;
		break;
		case PC:
		PIOC->PIO_PER = 1<<pio;
		PIOC->PIO_CODR = 1<<pio;
		break;
		case PD:
		PIOD->PIO_PER = 1<<pio;
		PIOD->PIO_CODR = 1<<pio;
		break;
				

		
	}
}

uint8_t DD_EN_PERI_AND_SET_PIO(pio_type peripheral,uint32_t pio)
{
			if((peripheral == PD) && ((pio == 7)||(pio == 8) ))
		{
		//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PD_7_8",6);
		}
	switch (peripheral)
	{
		case PA:
		PIOA->PIO_PDR = 1<<pio;
	//	PIOA->PIO_SCDR = 1<<pio;
		break;
		case PB:
		PIOB->PIO_PDR = 1<<pio;
	//	PIOB->PIO_SODR = 1<<pio;
		break;
		case PC:
		PIOC->PIO_PDR = 1<<pio;
	//	PIOC->PIO_CODR = 1<<pio;
		break;
		case PD:
		PIOD->PIO_PDR = 1<<pio;
	//	PIOD->PIO_CODR = 1<<pio;
		break;
		

		
	}
}

uint8_t DD_SET_PIO(pio_type peripheral,uint32_t pio)
{
					if((peripheral == PD) && ((pio == 7)||(pio == 8) ))
				{
				//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PD_7_8",6);
				}
	switch (peripheral)
	{
		case PA:
		PIOA->PIO_SODR = 1<<pio;
		break;
		case PB:
		PIOB->PIO_SODR = 1<<pio;
		break;
		case PC:
		PIOC->PIO_SODR = 1<<pio;
		break;
		case PD:
		PIOD->PIO_SODR = 1<<pio;
		break;
				

	}
	
	return 0;
}

uint8_t DD_RESET_PIO(const pio_type peripheral,uint32_t pio)
  {
	  		  		if((peripheral == PD) && ((pio == 7)||(pio == 8) ))
		  		{
			//  		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PD_7_8",6);
		  		}
	  switch (peripheral)
	  {
		  case PA:
		  PIOA->PIO_CODR = 1<<pio;
		  break;
		  case PB:
		  PIOB->PIO_CODR = 1<<pio;
		  break;
		  case PC:
		  PIOC->PIO_CODR = 1<<pio;
		  break;
		  case PD:
		  PIOD->PIO_CODR = 1<<pio;
		  break;
		  		

	  }
	  
	  return 0;
  }
  
  uint16_t DD_READ_PIO( pio_type peripheral,uint32_t pio)
  {
	  uint32_t read = 0;
	  uint16_t ret_val=0;
	  /*read= PIOC->PIO_ODSR;
	  printf("%d",pio);
	  printf("%d", read);*/
	  switch (peripheral)
	  {
		  case PA:
		  
		  if( (PIOA->PIO_PDSR & (1<<pio)) == 0)
		  {
			//  printf("A0");
			  return 0;
		  }
		  else
		  {
		//	  printf("A1");
			  return 1;
		  } 
		  break;
		  case PB:
		  if( (PIOB->PIO_PDSR & (1<<pio)) == 0)
		  {
		//	  printf("B0");
			  return 0;
		  }
		  else
		  {
			//  printf("B1");
			  return 1;
		  }
		  break;
		  case PC:
		 //   if(pio == 15)
		//	{
				
		//	read = PIOC->PIO_PDSR;
		//	if(read & pio)
		//		{
			//	printf("c");
			//	}
		//	}
			
		  if( (PIOC->PIO_PDSR & (1<<pio)) == 0)
		  {
			  
			
			//  printf("C0");
			  return 0;
		  }
		  else
		  {
			//  printf("C1");
			 // return 1;
			 ret_val = 1;
		//	 				printf("%d",ret_val);
			 return(ret_val);
			 
		  }
		  break;
		  case PD:
		  if( (PIOD->PIO_PDSR & (1<<pio)) == 0)
		  {
			//  printf("D0");
			  return 0;
		  }
		  else
		  {
			//  printf("D1");
			  return 1;
		  }
		  break;
		  
	  }
	  
	
  }