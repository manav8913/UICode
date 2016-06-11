/*
 * cl_utilities.c
 *
 * Created: 1/15/2014 10:09:34 AM
 *  Author: user
 */ 
#include "cl_app/inc/cl_types.h"

Cl_ReturnCodes cl_systemtickcallback(void);
Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
Cl_ReturnCodes Cl_MilliTickcallBack(void);
Cl_ReturnCodes cl_memset(uint8_t* , uint8_t );

volatile uint32_t g_ul_ms_ticks = 0;

static void mdelay(uint32_t ul_dly_ticks)
{
	uint32_t ul_cur_ticks;

	ul_cur_ticks = g_ul_ms_ticks;
	while ((g_ul_ms_ticks - ul_cur_ticks) < ul_dly_ticks);
}

Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks)
{
	mdelay(ul_dly_ticks);
	return CL_OK;
}

Cl_ReturnCodes cl_systemtickcallback(void)
{
	Cl_ReturnCodes Cl_retVal;
	Cl_retVal = Cl_MilliTickcallBack();
	g_ul_ms_ticks++;
	return  CL_OK;
}


Cl_ReturnCodes cl_memset(uint8_t* pdata, uint8_t datasize)
{
	uint8_t tempcnt = 0 ;
	uint8_t* pdata1;
	
	pdata1 = (uint8_t*)pdata;
	
	for (tempcnt = 0 ; tempcnt < datasize ; tempcnt++)
	{
		pdata[tempcnt] = 0;
	//	*pdata = 1;
	//	pdata++;
	//	*pdata1 = (uint8_t*)0;
	//	pdata1 =  pdata1 +1;
	}
	return CL_OK;
}