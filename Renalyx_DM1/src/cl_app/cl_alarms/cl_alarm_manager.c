/*
 * cl_alarm_manager.c
 *
 * Created: 2/12/2015 2:43:03 PM
 *  Author: user
 */ 
#if 0
#include "cl_alarmmanager.h"


RinseAlarmsType Cl_RinseAlarmTable[CL_RINSE_ALRM_MAX] =
{
	{BLOODDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{BYPASSDOOR_STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{LEVELSWITCH_OFF_TO_ON,CL_ALARM_ALERT,false,false,false},
	{LEVELSWITCH_ON_TO_OFF,CL_ALARM_ALERT,false,false,false},
	{HOLDER1STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{HOLDER2STATUS_OPEN,CL_ALARM_ALARM,false,false,false},
	{SENSOR_TEMP3STATUS,CL_ALARM_ALARM,false,false,false},
	{SENSOR_TEMP2STATUS,CL_ALARM_ALARM,false,false,false},
	{PS1STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{PS2STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{PS3STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{COND_STATUS_LOW,CL_ALARM_ALARM,false,false,false},
	{COND_STATUS_HIGH,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_OPEN,CL_ALARM_ALARM,false,false,false},
	{COND_DAC_RO,CL_ALARM_ALERT,false,false,false},
	{COND_DAC_HIGH,CL_ALARM_ALERT,false,false,false},
	{FLOWSTATUS_FLOWOFF,CL_ALARM_ALARM,false,false,false},
	{FLOW_LOW_FLOWRATE,CL_ALARM_ALARM,false,false,false},
	
};





Cl_ReturnCodes Cl_Rinse_UpdateAlarmTable(ClRinseAlarmIdType* ClRinseAlarmId )
{
	Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
	uint8_t tempcount = 0;
	uint8_t data[3] ={0, 0} ,*data1 = NULL;
	uint8_t data3;
	Cl_NewAlarmIdType cl_alarmId;
	
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	
	
	for (tempcount = 0 ; tempcount < CL_RINSE_ALRM_MAX ; tempcount++)
	{
		
		Cl_Alarm_GetAlarmStatus(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmId,&Cl_RinseAlarmTable[tempcount].IsActive);

		
		if (Cl_RinseAlarmTable[tempcount].IsActive)
		{
			if(!Cl_RinseAlarmTable[tempcount].IsRaised)
			{
				
				Cl_RinseAlarmTable[tempcount].IsRaised = true;
				
				//		Cl_rinseretcode = CL_RinseAlarmActon(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmId);
				
				
			}
			
		}
		else
		{
			// alarm was present before , but not active now.
			if(Cl_RinseAlarmTable[tempcount].IsRaised == true)
			{
				Cl_RinseAlarmTable[tempcount].IsRaised = false;
				command = CON_TX_COMMAND_ALARM;
				data[0] = (uint8_t)(Cl_RinseAlarmTable[tempcount].Cl_RinseAlarmId);
				data[1] = (uint8_t) CL_ALARM_CLEARED;
				//		Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,2);
			}

		}
		//}

	}

	return (Cl_rinseretcode );
}

Cl_ReturnCodes Cl_Rinse_ProcessAlarms(void )
{
	Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
	ClRinseAlarmIdType ClRinseAlarmId;
	uint8_t data;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_NewAlarmIdType cl_rinsealarmid;
	
	Cl_rinseretcode = Cl_Rinse_UpdateAlarmTable(&ClRinseAlarmId);

	
	
	return (Cl_rinseretcode);
}


Cl_ReturnCodes  CL_RinseAlarmActon(Cl_NewAlarmIdType cl_rinsealarmid)
{
	Cl_ReturnCodes 	Cl_rinseretcode = CL_OK;
	uint8_t data[3] ={0, 0} ,*data1 = NULL;
	uint8_t data3;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	uint16_t levelswitchstatus = 0;
	static uint8_t fillseccounter=0;
	Cl_NewAlarmIdType NewAlarmId = _NO_ALARM;
	

	

	
	switch(cl_rinsealarmid)
	{
		case BLOODDOOR_STATUS_OPEN:
		Cl_Rinse_StopRinse();
		NewAlarmId = BLOODDOOR_STATUS_OPEN;
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"BPD",3);
		Cl_SysStat_GetSensor_Status_Query(BLOODDOOR_STATUS_OPEN,&levelswitchstatus);
		
		break;
		case HOLDER1STATUS_OPEN:
		// stop rinsing
		Cl_Rinse_StopRinse();
		NewAlarmId = HOLDER1STATUS_OPEN;
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HLD1",4);
		Cl_SysStat_GetSensor_Status_Query(HOLDER1STATUS_OPEN,&levelswitchstatus);
		//enterl_saferinse_state();
		break;
		case HOLDER2STATUS_OPEN:
		// stop rinsing
		Cl_Rinse_StopRinse();
		NewAlarmId = HOLDER2STATUS_OPEN;
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HLD2",4);
		//enterl_saferinse_state();
		break;
		case LEVELSWITCH_OFF_TO_ON:
		// TURN OFF  WATER INLET
		NewAlarmId = _NO_ALARM;
		Cl_SysStat_GetSensor_Status_Query(LEVELSWITCH_OFF_TO_ON,&levelswitchstatus);
		{
			if(levelswitchstatus == 1)
			{
				
				//fillseccounter++;

				//	Cl_RinseFlowOff();
				cl_gfillinprogress = false;
				
			}
		}
		break;
		case LEVELSWITCH_ON_TO_OFF:
		// TURN ON WATER INLET
		NewAlarmId = _NO_ALARM;
		Cl_SysStat_GetSensor_Status_Query(LEVELSWITCH_ON_TO_OFF,&levelswitchstatus);
		{
			if(levelswitchstatus == 0)
			{
				
				//fillseccounter++;

				//	Cl_RinseFlowOn();
				cl_gfillinprogress = true;
				
			}
		}
		break;
		case TEMP1STATUS_HIGH:
		
		Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH,&TmpVal);
		
		temp1 = (0.805 * TmpVal) - 1004 ;
		temp2 = 3000 + (temp1 * 1000)/382;
		if(temp2 > 3680)
		{
			NewAlarmId = _TEMP1_HIGH_THRESHOLD;
			Cl_Rinse_StopRinse();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		}
		if(temp2 < 3500)
		{
			NewAlarmId = _TEMP1_LOW_THRESHOLD;
			Cl_Rinse_StopRinse();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		}
		//Cl_Rinse_StopRinse();
		//enterl_saferinse_state();

		break;
		case SENSOR_TEMP2STATUS:
		
		Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&TmpVal);
		
		temp1 = (0.805 * TmpVal) - 1004 ;
		temp2 = 3000 + (temp1 * 1000)/382;
		if(temp2 > 3680)
		{
			NewAlarmId = _TEMP2_HIGH_THRESHOLD;
			Cl_Rinse_StopRinse();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		}
		if(temp2 < 3500)
		{
			NewAlarmId = _TEMP2_LOW_THRESHOLD;
			Cl_Rinse_StopRinse();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		}
		//Cl_Rinse_StopRinse();
		//enterl_saferinse_state();

		break;
		case SENSOR_TEMP3STATUS:
		
		Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&TmpVal);
		int16_t temp3,temp4;
		temp3 = (0.805 * TmpVal) - 1004 ;
		temp4 = 3000 + (temp3 * 1000)/382;
		if(temp4 > 3680)
		{
			NewAlarmId = _TEMP3_HIGH_THRESHOLD;
			Cl_Rinse_StopRinse();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		}
		if(temp4 < 3500)
		{
			NewAlarmId = _TEMP3_LOW_THRESHOLD;
			Cl_Rinse_StopRinse();
			Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
			cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		}
		//Cl_Rinse_StopRinse();
		//enterl_saferinse_state();

		break;
		case 	PS1STATUS_HIGH:
		NewAlarmId = _PS1_HIGH_THRESHOLD;
		Cl_Rinse_StopRinse();
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1",3);
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		//enterl_saferinse_state();
		break;
		case	PS2STATUS_HIGH:
		NewAlarmId = _PS2_HIGH_THRESHOLD;
		Cl_Rinse_StopRinse();
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2",3);
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		//enterl_saferinse_state();
		break;
		case	PS3STATUS_HIGH:
		NewAlarmId = _NO_ALARM;
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS3",3);
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		//enterl_saferinse_state();
		break;
		case FLOWSTATUS_FLOWOFF:
		//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLW",3);
		Cl_Rinse_StopRinse();
		NewAlarmId = _FLOW_NO_FLOW;
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;
		case FLOW_LOW_FLOWRATE:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"FLW",3);
		NewAlarmId = _FLOW_LOW_FLOWRATE;
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;
		case COND_STATUS_LOW:
		//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_LOW",8);
		NewAlarmId = _COND_STATUS_LOW;
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;
		case COND_STATUS_HIGH:
		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_HIGH",9);
		NewAlarmId = _COND_STATUS_HIGH;
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;
		case COND_DAC_OPEN:
		// air trapped in DAC2 or no flow
		//				 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_OPEN",8);
		NewAlarmId = _COND_DAC_OPEN;
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;
		//	case COND_DAC_RO:
		// RO water filled DAC2
		//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_RO",6);
		//				NewAlarmId = _COND_DAC_RO;
		//			Cl_Rinse_StopRinse();
		//		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		//break;
		case COND_DAC_HIGH:
		//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"DAC_HIGH",8);
		//					NewAlarmId = _COND_DAC_HIGH;
		//					Cl_Rinse_StopRinse();
		//				cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		// Rinse incomplete.
		break;
		case 	UFP_OVERRUN:
		case	UFP_UNDERRUN:
		NewAlarmId = _UF_ALARM_FOR_RINSE;
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;
		case 	BC_OVERRUN:
		case	BC_UNDERRUN:
		case	BC_FAILED:
		NewAlarmId = _BC_ALARM;
		Cl_Rinse_StopRinse();
		cl_rinsestate = CL_RINSE_STATE_CRITICAL_ALARM;
		break;

		case DE_CHAMBER: //23
		break;
		case SYSTEM_NOT_READY: //24
		break;
		case POWER_FAILURE: //25
		break;

		case WATCHDOG_TIMER: //27
		NewAlarmId = _WATCHDOG_TIMER;
		break;
		default:
		break;

	}
	if(NewAlarmId != _NO_ALARM)
	{
		data[0] = (uint8_t)NewAlarmId;
		data[1] = (uint8_t) CL_ALARM_TRIGGERED;
		data[2] = (uint8_t)CRITICAL;
		command = CON_TX_COMMAND_ALARM;

		Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,3);
		
		
		command = CON_TX_COMMAND_SYS_STATE;
		data[0] = (uint8_t)CLEANING;
		data[1] = (uint8_t)CLEAN_CLEAN_ALARM_STOPPED;
		Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,2);
	}
	
}

#endif