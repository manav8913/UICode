/*
 * cl_sys_status.c
 *
 * Created: 1/7/2014 2:55:10 PM
 *  Author: user
 */ 
#include "cl_app/inc/cl_types.h"
#include "cl_app/inc/cl_stubs.h"
#include "Platform/Service/sv_interface.h"
#include "sv_stubs/inc/sv_stub.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

extern uint16_t sv_status_getsystemstatus(Sys_statusType* );
Cl_ReturnCodes 		Cl_SysStat_System_Status_Query(void);



extern Cl_Sys_statusType cl_sys_statbuffer;
extern Cl_ReturnCodes cl_memset(uint8_t* , uint8_t );

extern Cl_ReturnCodes sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );//testing
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );//testing
extern Cl_ReturnCodes Cl_SysStat_Get_Flowcount( uint16_t* pdata);

Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType , uint16_t*);


Cl_ReturnCodes 		Cl_SysStat_System_Status_Query(void)
{
	Cl_ReturnCodes RetVal;
	Sys_statusType sv_sys_statbuffer;
	int16_t temp,temp1;
	
	 RetVal = cl_memset((uint8_t*)&sv_sys_statbuffer , sizeof(sv_sys_statbuffer)) ;
	 #ifndef PLATFORMSTUB
	 cl_sys_statbuffer.FPcurrentstatus = 1;
	 cl_sys_statbuffer.DGPcurrentstatus = 1;


	sv_status_getsystemstatus(&sv_sys_statbuffer);

	cl_sys_statbuffer.cond_wien_status = sv_sys_statbuffer.cond_wien_status;
	cl_sys_statbuffer.abdstatus = sv_sys_statbuffer.abdstatus;
	cl_sys_statbuffer.aptstatus = sv_sys_statbuffer.aptstatus;
	cl_sys_statbuffer.bdstatus = sv_sys_statbuffer.bdstatus;
	cl_sys_statbuffer.bldstatus = sv_sys_statbuffer.bldstatus;
	cl_sys_statbuffer.BPstatus = sv_sys_statbuffer.BPstatus;
	cl_sys_statbuffer.vptstatus = sv_sys_statbuffer.vptstatus;
	cl_sys_statbuffer.cond_status = sv_sys_statbuffer.cond_status;
	cl_sys_statbuffer.cond_status_dac = sv_sys_statbuffer.cond_status_dac;
	cl_sys_statbuffer.FPcurrentstatus = sv_sys_statbuffer.FPcurrentstatus;
	cl_sys_statbuffer.DGPcurrentstatus = sv_sys_statbuffer.DGPcurrentstatus;
	cl_sys_statbuffer.UFPstatus = sv_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.holder1status = 	sv_sys_statbuffer.holderstatus1;
	cl_sys_statbuffer.holder2status =   sv_sys_statbuffer.holderstatus2;
	cl_sys_statbuffer.bloodpumpdoor = sv_sys_statbuffer.bloodpumpdoor;
	cl_sys_statbuffer.levelswitch1 = sv_sys_statbuffer.levelswitch1;

	cl_sys_statbuffer.ps1status = sv_sys_statbuffer.ps1status;
	cl_sys_statbuffer.ps2status = sv_sys_statbuffer.ps2status;
	cl_sys_statbuffer.ps3status = sv_sys_statbuffer.ps3status;
	cl_sys_statbuffer.ps4status = sv_sys_statbuffer.ps4status;
	cl_sys_statbuffer.UFPstatus = sv_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.Temp1status = sv_sys_statbuffer.Temp1status;
	cl_sys_statbuffer.Temp2status = sv_sys_statbuffer.Temp2status;
	cl_sys_statbuffer.Temp3status = sv_sys_statbuffer.Temp3status;
	cl_sys_statbuffer.Temp4status = sv_sys_statbuffer.Temp4status;
	cl_sys_statbuffer.Flowstatus = sv_sys_statbuffer.Flowstatus;
	cl_sys_statbuffer.Heparin_full_marker = sv_sys_statbuffer.Heparin_full_marker;
	cl_sys_statbuffer.Heparin_empty_marker = sv_sys_statbuffer.Heparin_empty_marker;
	cl_sys_statbuffer.Heparin_rotation_marker = sv_sys_statbuffer.Heparin_rotation_marker;
	#else
	
	cl_sys_statbuffer.abdstatus = sv_test_sys_statbuffer.abdstatus;
	cl_sys_statbuffer.aptstatus = sv_test_sys_statbuffer.aptstatus;
	cl_sys_statbuffer.bdstatus = sv_test_sys_statbuffer.bdstatus;
	cl_sys_statbuffer.bldstatus = sv_test_sys_statbuffer.bldstatus;
	cl_sys_statbuffer.BPstatus = sv_test_sys_statbuffer.BPstatus;
	cl_sys_statbuffer.vptstatus = sv_test_sys_statbuffer.vptstatus;
	cl_sys_statbuffer.cond_status = sv_test_sys_statbuffer.cond_status;
	cl_sys_statbuffer.FPcurrentstatus = sv_test_sys_statbuffer.FPcurrentstatus;
	cl_sys_statbuffer.DGPcurrentstatus = sv_test_sys_statbuffer.DGPcurrentstatus;
	cl_sys_statbuffer.UFPstatus = sv_test_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.holder1status = 	sv_test_sys_statbuffer.holderstatus;
	cl_sys_statbuffer.holder2status = sv_test_sys_statbuffer.holderstatus;
	cl_sys_statbuffer.Hpstatus = sv_test_sys_statbuffer.Hpstatus;
	cl_sys_statbuffer.levelswitch1 = sv_test_sys_statbuffer.levelswitch1;
	//cl_sys_statbuffer.levelswitch2 = sv_test_sys_statbuffer.levelswitch2;
	cl_sys_statbuffer.ps1status = sv_test_sys_statbuffer.ps1status;
	cl_sys_statbuffer.ps2status = sv_test_sys_statbuffer.ps2status;
	cl_sys_statbuffer.ps3status = sv_test_sys_statbuffer.ps3status;
	cl_sys_statbuffer.ps4status = sv_test_sys_statbuffer.ps4status;
	cl_sys_statbuffer.UFPstatus = sv_test_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.Temp1status = sv_test_sys_statbuffer.Temp1status;
	cl_sys_statbuffer.Temp2status = sv_test_sys_statbuffer.Temp2status;
	cl_sys_statbuffer.Temp3status = sv_test_sys_statbuffer.Temp3status;
	cl_sys_statbuffer.Flowstatus = sv_test_sys_statbuffer.Flowstatus;
	cl_sys_statbuffer.Heparin_full_marker = sv_test_sys_statbuffer.Heparin_full_marker;
	cl_sys_statbuffer.Heparin_empty_marker = sv_test_sys_statbuffer.Heparin_empty_marker;
	
	#endif
	//testing
	static bool flag = true;
#if 0	
	if (flag)
	{
		cl_sys_statbuffer.Flowstatus = 0;
		flag =  false;
	}
	else{
		cl_sys_statbuffer.Flowstatus = 1;
		flag =  true;
	}
	#endif
//	cl_sys_statbuffer.Temp3status = 700;
	//cl_sys_statbuffer.Temp2status = 710;
	
	#if 0
	if(cl_sys_statbuffer.UFPstatus)
		{

					DD_SET_VALVE (VALVE_ID15);
		}

		else
		{
			DD_RESET_VALVE (VALVE_ID15);
		}
	
			
	#endif	
	if(cl_sys_statbuffer.Temp2status > 3300)

	{
		RetVal = sv_cntrl_poweroffheater();
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,( uint8_t*)"TS2_?",5);
	}
	if(cl_sys_statbuffer.Temp3status > 3300)

	{
		RetVal = sv_cntrl_poweroffheater();
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,( uint8_t*)"TS3_?",5);
	}
	
#if 0
	if(cl_sys_statbuffer.Flowstatus )
	{
		sv_cntrl_activate_valve((sv_valvetype) 1  );
	}
	else
	{
		sv_cntrl_deactivate_valve((sv_valvetype)1  );
		
	}
	//testing
#endif
	return CL_OK;
}


Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType dataID, uint16_t* pdata)
{
	
	Cl_ReturnCodes RetVal;
	uint16_t tempdata=0;
	
		switch(dataID)
		{
			case HOLDER1STATUS_OPEN:
			tempdata =  cl_sys_statbuffer.holder1status;
			*pdata  = tempdata;
			break;
			case HOLDER2STATUS_OPEN:
			tempdata =  cl_sys_statbuffer.holder2status;
			*pdata  = tempdata;
			break;
			case LEVELSWITCH_OFF_TO_ON:
			tempdata =  cl_sys_statbuffer.levelswitch1;
			*pdata  = tempdata;
			break;
			case LEVELSWITCH_ON_TO_OFF:
			tempdata =  cl_sys_statbuffer.levelswitch1;
			*pdata  = tempdata;
			break;
			case COND_STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.cond_status;
			*pdata  = tempdata;
			break;
			case COND_DAC_RO:
			tempdata =  cl_sys_statbuffer.cond_status_dac;
			*pdata  = tempdata;
			break;
			case FPCURRENTSTATUS:
			tempdata =  cl_sys_statbuffer.FPcurrentstatus;
			*pdata  = tempdata;
			break;
			case DGPCURRENTSTATUS:
			tempdata =  cl_sys_statbuffer.DGPcurrentstatus;
			*pdata  = tempdata;
			break;
			case UF_ROTATION_MARKER:
			tempdata =  cl_sys_statbuffer.UFPstatus;
			*pdata  = tempdata;
			break;
			case ABDSTATUS_ON:
			tempdata =  cl_sys_statbuffer.abdstatus;
			*pdata  = tempdata;
			break;
			case BDSTATUS_ON:
			tempdata =  cl_sys_statbuffer.bdstatus;
			*pdata  = tempdata;
			break;
			case APTSTATUS_HIGH:
			tempdata =  cl_sys_statbuffer.aptstatus;
			*pdata  = tempdata;
			break;
			case VPTSTATUS_HIGH:
			tempdata =  cl_sys_statbuffer.vptstatus;
			*pdata  = tempdata;
			break;
			case BLDSTATUS_ON:
			tempdata =  cl_sys_statbuffer.bldstatus;
			*pdata  = tempdata;
			break;
			case PS1STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.ps1status;
			*pdata  = tempdata;
			break;
			case PS2STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.ps2status;
			*pdata  = tempdata;
			break;
			case PS3STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.ps3status;
			*pdata  = tempdata;
			break;
			case PS4STATUS:
			tempdata =  cl_sys_statbuffer.ps4status;
			*pdata  = tempdata;
			break;
			case TEMP1STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.Temp1status;
			*pdata  = tempdata;
			break;
			case TEMP2STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.Temp2status;
			*pdata  = tempdata;
			break;
			case TEMP3STATUS_HIGH:
			tempdata =  cl_sys_statbuffer.Temp3status;
			*pdata  = tempdata;
			break;
			case HEATER_HIGH_TEMP:
			tempdata =  cl_sys_statbuffer.Temp4status;
			*pdata  = tempdata;
			break;
			case 	HP_START:
			tempdata =  cl_sys_statbuffer.Heparin_full_marker;
			*pdata  = tempdata;
			break;
			case HP_END:
			tempdata =  cl_sys_statbuffer.Heparin_empty_marker;
			*pdata  = tempdata;
			break;
			case HP_ROTATION_MARKER:
			tempdata =  cl_sys_statbuffer.Heparin_rotation_marker;
			*pdata  = tempdata;
			break;
			case BP_ROTATION_MARKER:
			tempdata =  cl_sys_statbuffer.BloodPump_rotation_marker;
			*pdata  = tempdata;
			break;
			case FLOWSTATUS_FLOWON:
			case FLOWSTATUS_FLOWOFF:
			tempdata =  cl_sys_statbuffer.Flowstatus;
			*pdata  = tempdata;
			break;
			case BLOODDOOR_STATUS_OPEN:
			tempdata =  cl_sys_statbuffer.bloodpumpdoor;
			*pdata  = tempdata;
			break;
			default:
			break;
			
			

	
		
		}
		
	return CL_OK;
	
}

