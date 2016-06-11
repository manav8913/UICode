/*
 * SV_CS.c
 *
 * Created: 9/19/2014 4:11:18 PM
 *  Author: user
 */ 
#include "asf.h"

#define  CS_POT_IIC_ADDR 0x2c
extern void DD_READ_IIC(uint8_t iic_address, uint16_t* data);
extern void DD_WRITE_IIC(uint8_t iic_address, uint16_t data);
void DD_IIC_SET_POT(uint8_t iic_address, uint16_t* data);
void DD_IIC_SEND_PROP(uint8_t iic_address, uint8_t* data);
uint8_t sv_cs_readpotvalue(uint16_t* resistance);
uint8_t sv_cs_setpotvalue(uint16_t resistance) ;
uint8_t sv_cs_setpotvalue(uint16_t resistance) 
{
	 uint8_t iic_address;
	 uint16_t data;
	 DD_IIC_SET_POT( CS_POT_IIC_ADDR, resistance);
}

uint8_t sv_cs_readpotvalue(uint16_t* resistance)
{
		uint8_t iic_address;
		uint8_t data;
		//DD_SET_POT( CS_POT_IIC_ADDR, &data);
		*resistance = data;
	
}