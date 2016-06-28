/*
 * SV_PROP.c
 *
 * Created: 3/19/2015 3:25:33 PM
 *  Author: user
 */ 
#include "asf.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"

#ifndef CPU_PROP_IIC_ADDR
#define  CPU_PROP_IIC_ADDR 0x70
#endif
//#define  CPU_PROP_IIC_ADDR 0x70
extern void DD_IIC_SEND_PROP(uint8_t , uint8_t* );
extern void DD_IIC_CONSOLE_SEND(uint8_t iic_address, uint8_t* data,uint8_t length);
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
extern void DD_IIC_READ_PROP(uint8_t iic_address, uint16_t* data);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
void sv_prop_startmixing();
void sv_prop_stopmixing();
void sv_prop_startopenfill();
void sv_prop_stopopenfill();
void sv_prop_start_disinfect_intake(void);
void sv_prop_stop_disinfect_intake(void);
void sv_prop_set_propro_rc(uint8_t data);
static uint8_t data = 0;
uint16_t count = 0;

void sv_prop_startpropeo_aligning()
{
uint8_t dataarray[16] = { 0xaa,0xcc,0xdd,1,2,3,4,5,6,7,8,9,0xA,0xB,0xee,0xbb};
	//	cl_wait(20);
		DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 0);
		//DD_IIC_CONSOLE_SEND(0x77, &dataarray ,16);

}
void sv_prop_startmixing()
{

	{
	//	cl_wait(20);
		DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 1);
	//	DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 0);
	}
	
}

void sv_prop_set_propro_rc(uint8_t data)
{
	DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, data);
}

void sv_prop_stopmixing()
{
	DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 2);
}

void sv_prop_start_disinfect_intake(void)
{


	{
	//	cl_wait(20);
		DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 3);


	}
	
}
void sv_prop_stop_disinfect_intake(void)
{


	{
	//	cl_wait(20);
		DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 4);


	}
	
}
void sv_prop_startopenfill()
{
	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"OPENFIL",6);
	DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 5);
}
void sv_prop_stopopenfill()
{
	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"STOPFIL",6);
	DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 6);
}
