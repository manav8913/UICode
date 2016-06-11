/*
 * sv_stub.c
 *
 * Created: 1/13/2014 9:38:06 AM
 *  Author: user
 */ 

#include "inc/sv_types.h"
#include "uart.h"

Sv_ReturnCodesType sv_nvmsetdata(uint8_t ,uint8_t* ,uint8_t datasize);
Sv_ReturnCodesType sv_nvmgetdata(uint8_t , uint8_t* );
//Sv_ReturnCodesType sv_setflowpath(Sv_flowpathtype);
//Sv_ReturnCodesType  sv_console_bytedatawrite(Uart *p_uart ,const uint8_t Sv_ConsoleTxdataByte);

#if 0
uint8_t nvmarray[NV_NVM_MAXDATA * 4] = {
										0,0,0,0,// NV_RINSE_NVM_RINSE_STATUS
										1,0,0,0,//NV_PRIME_NVM_PRIME_STATUS
										2,0,0,0,//DIALYSER_PRIME_STATUS
										3,0,0,0,//NV_DIALYSATE_FLOW
										4,0,0,0,//NV_DIALYSATE_TEMP
										5,0,0,0,//NV_APT_VALUE
										6,0,0,0,//NV_VPT_VALUE
										7,0,0,0,//NV_TMP
										8,0,0,0,//NV_CONDUCTIVITY
										9,0,0,0,//NV_UF_REMOVAL_RATE
										10,0,0,0, // NV_UF_GOAL
										11,0,0,0,//NV_ARTERIAL_BLOODFLOW_RATE
										12,0,0,0,//NV_HEPARIN_FLOW_RATE
										13,0,0,0, // NV_HE{ARIN_BOLUS
										14,0,0,0,//NV_ABD_THRESHOLD
										15,0,0,0,//NV_BLD_THRESHOLD
										
										16,0,0,0,// NV_DIALYSIS_FLOW_CONFIG
										17,0,0,0,//NV_DIALYSIS_TEMP_CONFIG
										18,0,0,0,//NV_HEPARIN_RATE_CONFIG
										19,0,0,0,//NV_APT_VALUE_CONFIG
										20,0,0,0,//NV_VPT_VALUE_CONFIG
										21,0,0,0,//NV_TMP_VALUE_CONFIG
										22,0,0,0,//NV_COND_VALUE_CONFIG
										23,0,0,0,//NV_UF_RATE_CONFIG
										24,0,0,0,//NV_UF_GOAL_CONFIG
										25,0,0,0,//NV_BOLUS_VOLUME_CONFIG
										26,0,0,0, // NV_ABF_RATE_CONFIG
										};
										

										
	
#endif

uint16_t nvmarray[NV_NVM_MAXDATA] = {
	0,// NV_RINSE_NVM_RINSE_STATUS
	0,//NV_PRIME_NVM_PRIME_STATUS
	0,//DIALYSER_PRIME_STATUS
	0,//DIALYSER_STATUS
	-2,//NV_DIALYSATE_FLOW
	0,//NV_DIALYSATE_TEMP
	0,//NV_APT_VALUE
	0,//NV_VPT_VALUE
	0,//NV_TMP
	0,//NV_CONDUCTIVITY
	0,//NV_UF_REMOVAL_RATE
	0, // NV_UF_GOAL
	-4,//NV_ARTERIAL_BLOODFLOW_RATE
	0,//NV_HEPARIN_FLOW_RATE
	0, // NV_HEPARIN_BOLUS
	0,//NV_ABD_THRESHOLD
	0,//NV_BLD_THRESHOLD
	0,//	NV_DIALYSIS_FLOW_CONFIG_UPPER,//17
	0,//	NV_DIALYSIS_FLOW_CONFIG_LOWER,//17
	0,//	NV_DIALYSIS_TEMP_CONFIG_UPPER,//18
	0,//	NV_DIALYSIS_TEMP_CONFIG_LOWER,//18
	0,//	NV_HEPARIN_RATE_CONFIG_UPPER,//19
	0,//	NV_HEPARIN_RATE_CONFIG_LOWER,//19
	0,//	NV_APT_VALUE_CONFIG_UPPER,//20
	0,//	NV_APT_VALUE_CONFIG_LOWER,//20
	0,//	NV_VPT_VALUE_CONFIG_UPPER,//21
	0,//	NV_VPT_VALUE_CONFIG_LOWER,//21
	0,//	NV_TMP_VALUE_CONFIG_UPPER,//22
	0,//	NV_TMP_VALUE_CONFIG_LOWER,//22
	0,//	NV_COND_VALUE_CONFIG_UPPER,//23
	0,//	NV_COND_VALUE_CONFIG_LOWER,//23
	0,//	NV_UF_RATE_CONFIG_UPPER,//24
	0,//	NV_UF_RATE_CONFIG_LOWER,//24
	0,//	NV_UF_GOAL_CONFIG_UPPER,//25
	0,//	NV_UF_GOAL_CONFIG_LOWER,//25
	0,//	NV_BOLUS_VOLUME_CONFIG_UPPER,//26
	0,//	NV_BOLUS_VOLUME_CONFIG_LOWER,//26
	0,//	NV_ABF_RATE_CONFIG_UPPER,//27	
	0,//	NV_ABF_RATE_CONFIG_LOWER,//27
	};

	
uint8_t nvmdatasize[NV_NVM_MAXDATA] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
#if 0
Sv_ReturnCodesType sv_setflowpath(Sv_flowpathtype sv_fpath)
{
	return SV_OK;
}
#endif
#if 0
Sv_ReturnCodesType  sv_console_bytedatawrite(Uart *p_uart ,const uint8_t Sv_ConsoleTxdataByte)
{
	return (Sv_ReturnCodesType) (uart_write(p_uart,  Sv_ConsoleTxdataByte));
}
	
#endif

typedef union
{
	uint8_t byte[2];
	uint16_t twobyte;
	}nvdatatype;
	
Sv_ReturnCodesType sv_nvmgetdata(uint8_t dataItemId, uint8_t* pdata)
{
	uint8_t size;
	nvdatatype temp;
#if 0	
	for (size = 0 ; size < nvmdatasize[dataItemId] ; size ++)
	{
		pdata[size] = nvmarray[(dataItemId-1) * 4 + size] ;
	}
#endif
	
			temp.twobyte = nvmarray[dataItemId-1]  ;
			pdata[0] = temp.byte[0]  ;
			pdata[1] = temp.byte[1]  ;
			

#if 0
	switch(dataItemId)
	{	
		
		case NV_NVM_RINSE_STATUS:
		for (size = 0 ; size < nvmdatasize[NV_RINSE_NVM_RINSE_STATUS] ; size ++)
		{
			pdata[size] = nvmarray[(NV_RINSE_NVM_RINSE_STATUS-1) *4 + size];
			
		}
		
		break;
		case NV_RINSE_NVM_PRIME_STATUS:
		for (size = 0 ; size < nvmdatasize[NV_RINSE_NVM_PRIME_STATUS] ; size ++)
		{
			pdata[size] = nvmarray[(NV_RINSE_NVM_PRIME_STATUS-1) *4 + size];
			
		}
		break;
		case NV_DIALYSATE_FLOW:
		for (size = 0 ; size < nvmdatasize[NV_DIALYSATE_FLOW] ; size ++)
		{
			pdata[size] = nvmarray[(NV_DIALYSATE_FLOW -1) *4 + size];
		}
		
		break;
		
		
		case NV_DIALYSATE_TEMP:
		for (size = 0 ; size < nvmdatasize[NV_DIALYSATE_TEMP] ; size ++)
		{
			pdata[size] = nvmarray[(NV_DIALYSATE_TEMP -1) *4 + size];
		}
		
		break;
		
		case NV_APT_VALUE:
		for (size = 0 ; size < nvmdatasize[NV_APT_VALUE] ; size ++)
		{
			pdata[size] = nvmarray[(NV_APT_VALUE -1) *4 + size];
		}
		
		break;
		case NV_VPT_VALUE:
		for (size = 0 ; size < nvmdatasize[NV_VPT_VALUE] ; size ++)
		{
			pdata[size] = nvmarray[(NV_VPT_VALUE -1) *4 + size];
		}
		
		break;
		case NV_CONDUCTIVITY:
		for (size = 0 ; size < nvmdatasize[NV_CONDUCTIVITY] ; size ++)
		{
			pdata[size] = nvmarray[(NV_CONDUCTIVITY -1) *4 + size];
		}
		
		break;		
		
		case NV_UF_REMOVAL_RATE:
		for (size = 0 ; size < nvmdatasize[NV_UF_REMOVAL_RATE] ; size ++)
		{
			pdata[size] = nvmarray[(NV_UF_REMOVAL_RATE-1)  *4 + size];
		}
		
		break;
		case NV_ARTERIAL_BLOODFLOW_RATE:
		for (size = 0 ; size < nvmdatasize[NV_UF_REMOVAL_RATE] ; size ++)
		{
			pdata[size] = nvmarray[(NV_ARTERIAL_BLOODFLOW_RATE-1) *4 + size ];
		}
		
		break;
		case NV_HEPARIN_FLOW_RATE:
		for (size = 0 ; size < nvmdatasize[NV_HEPARIN_FLOW_RATE ] ; size ++)
		{
			pdata[size] = nvmarray[(NV_HEPARIN_FLOW_RATE-1) *4 + size];
		}
		
		break;
		default:
		break;
	}
	#endif
		return SV_OK;
}


Sv_ReturnCodesType sv_nvmsetdata(uint8_t dataItemId,uint8_t* pdata,uint8_t datasize)
{
	
	uint8_t size;
	nvdatatype temp;
	sv_nvm_dataidtype sv_nvm_dataid;
	#if 0
				for (size = 0 ; size < nvmdatasize[dataItemId] ; size ++)
				{
					//nvmarray[(dataItemId-1) * 4 + size] = 	pdata[size];
					
				}
				
				
					
	#endif
	
			temp.byte[0] = pdata[0];
			temp.byte[1] = pdata[1];
			nvmarray[dataItemId-1] = temp.twobyte;
			
		return SV_OK;
}