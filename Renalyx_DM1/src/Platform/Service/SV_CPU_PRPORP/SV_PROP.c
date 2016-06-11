/*
 * SV_PROP.c
 *
 * Created: 3/19/2015 3:25:33 PM
 *  Author: user
 */ 
#include "asf.h"
#include "cl_app/inc/cl_types.h"
#define  CPU_PROP_IIC_ADDR 0x55
extern void DD_IIC_SEND_PROP(uint8_t , uint8_t* );
extern void DD_IIC_CONSOLE_SEND(uint8_t iic_address, uint8_t* data,uint8_t length);
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
extern void DD_IIC_READ_PROP(uint8_t iic_address, uint16_t* data);
void sv_prop_startmixing();
void sv_prop_stopmixing();
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
	uint8_t read_data;
	data = 0;
	count = 0;
	//while ( count < 256)
	{
		cl_wait(20);
		DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 1);

		count++;
		data++;
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