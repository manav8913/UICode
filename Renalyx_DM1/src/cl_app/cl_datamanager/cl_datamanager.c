/*
 * cl_datamanager.c
 *
 * Created: 6/17/2016 5:14:45 PM
 *  Author: user
 */ 


#include "cl_app/inc/cl_types.h"
#include "inc/cl_datamanager.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "sv_stubs/inc/sv_types.h"
#include "cl_app\cl_status\inc\cl_status.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"

extern Cl_ReturnCodes  sv_nvmgetdata(uint8_t,uint8_t*);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern uint32_t Treatdata[ID_MAX_TREAT_PARAM] ;
extern Cl_AlarmThresholdType  Cl_alarmThresholdTable;

Cl_ReturnCodes cl_datamamanager(MAC_EVENTS cl_data_event)
{
	Cl_ReturnCodes cl_data_ret_val = CL_ERROR;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	rxstreamtype datastream;
	uint8_t dataarray[20] = {};
	
	switch(cl_data_event)
	{
		case EVT_CONSOLE_COMMAND_GET_DATA:
		if(Cl_ConsoleRxMsg.msgready == true)
		{
			switch(Cl_ConsoleRxMsg.data.byte[0])
			{
				case CON_RX_PARAM_DATA_RINSE_STATUS:
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] = CON_TX_PARAM_DATA_RINSE_STATUS;
				cl_data_ret_val = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
				cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,2);
				break;
				case CON_RX_PARAM_DATA_PRIME_STATUS:
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] = CON_TX_PARAM_DATA_PRIME_STATUS;
				cl_data_ret_val = (uint8_t)sv_nvmgetdata(NV_NVM_PRIME_STATUS, &dataarray[1]);
				cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,2);
				break;
				case CON_RX_PARAM_DATA_MIXING_STATUS:
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] = CON_TX_PARAM_DATA_MIXING;
				//Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
				dataarray[1] = 0;
				cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,2);
				break;
				case CON_RX_PARAM_DATA_RECIRC_STATUS:
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] = CON_TX_PARAM_DATA_RCIRC;
				//Cl_rinseretcode = (uint8_t)sv_nvmgetdata(NV_NVM_RINSE_STATUS, &dataarray[1]);
				dataarray[1] = 0;
				cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,2);
				break;
				case CON_RX_PARAM_DATA_DISINF_STATUS:
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] = CON_TX_PARAM_DATA_DISINF_STATUS;
				
				cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,2);
				break;
				case CON_RX_PARAM_DATA_DIALYSER_PRIME_STATUS:
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] = CON_TX_PARAM_DATA_DIALYSER_PRIME_STATUS;
				dataarray[1] = 0;									
				cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,2);
				break;
				case CON_RX_PARAM_DATA_TREATMENT_DATA:
				
				command = CON_TX_COMMAND_SYSDATA;
				dataarray[0] =   CON_TX_PARAM_DATA_TREATMENT_DATA;
				if(Cl_ConsoleRxMsg.data.byte[1] < ID_b_Dlaystedata)
				{
					dataarray[1] = Cl_ConsoleRxMsg.data.byte[1];
					datastream.word = Treatdata[dataarray[1]];
					dataarray[2] = datastream.bytearray[0];
					dataarray[3] = datastream.bytearray[1];
					dataarray[4] = datastream.bytearray[2];
					dataarray[5] = datastream.bytearray[3];

					cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,6);
				}
				else if(Cl_ConsoleRxMsg.data.byte[1] < ID_MAX_TREAT_PARAM)
				{
					dataarray[1] = Cl_ConsoleRxMsg.data.byte[1];
					
					switch (Cl_ConsoleRxMsg.data.byte[1])
					{
						case ID_b_Dlaystedata:
											datastream.word = Treatdata[ID_dflow];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_settemp];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_setcond];
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,14);
						break;
						case ID_b_Conddata:
											datastream.word = Treatdata[ID_condllimit];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_condulimit];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_b_Conddata];;
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,14);
						break;
						case ID_b_TempData:
											datastream.word = Treatdata[ID_templlimit];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_tempulimit];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_settemp];
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,14);
						break;
						case ID_b_tmp:
											datastream.word = Treatdata[ID_tmpllimit];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_tmpulimit];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = 200;
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,14);
						break;
						case ID_b_treatmentdata:
											datastream.word = Treatdata[ID_b_Conddata];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_settemp];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_dflow];
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_ufgoal];
											dataarray[14] = datastream.bytearray[0];
											dataarray[15] = datastream.bytearray[1];
											dataarray[16] = datastream.bytearray[2];
											dataarray[17] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_ufrate];
											dataarray[18] = datastream.bytearray[0];
											dataarray[19] = datastream.bytearray[1];
											dataarray[20] = datastream.bytearray[2];
											dataarray[21] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_minufrate];
											dataarray[22] = datastream.bytearray[0];
											dataarray[23] = datastream.bytearray[1];
											dataarray[24] = datastream.bytearray[2];
											dataarray[25] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_bloodratetreat];
											dataarray[26] = datastream.bytearray[0];
											dataarray[27] = datastream.bytearray[1];
											dataarray[28] = datastream.bytearray[2];
											dataarray[29] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_bloodratereturn];
											dataarray[30] = datastream.bytearray[0];
											dataarray[31] = datastream.bytearray[1];
											dataarray[32] = datastream.bytearray[2];
											dataarray[33] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_treattime];
											dataarray[34] = datastream.bytearray[0];
											dataarray[35] = datastream.bytearray[1];
											dataarray[36] = datastream.bytearray[2];
											dataarray[37] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_heprate];
											dataarray[38] = datastream.bytearray[0];
											dataarray[39] = datastream.bytearray[1];
											dataarray[40] = datastream.bytearray[2];
											dataarray[41] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_bolusvol];
											dataarray[42] = datastream.bytearray[0];
											dataarray[43] = datastream.bytearray[1];
											dataarray[44] = datastream.bytearray[2];
											dataarray[45] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,46);
						break;
						case ID_b_vpt:
											datastream.word = Treatdata[ID_vptllimit];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_vptulimit];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = 400;
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,14);
						break;
						case ID_b_apt:
											datastream.word = Treatdata[ID_aptllimit];
											dataarray[2] = datastream.bytearray[0];
											dataarray[3] = datastream.bytearray[1];
											dataarray[4] = datastream.bytearray[2];
											dataarray[5] = datastream.bytearray[3];
											datastream.word = Treatdata[ID_aptulimit];
											dataarray[6] = datastream.bytearray[0];
											dataarray[7] = datastream.bytearray[1];
											dataarray[8] = datastream.bytearray[2];
											dataarray[9] = datastream.bytearray[3];
											datastream.word = 500;
											dataarray[10] = datastream.bytearray[0];
											dataarray[11] = datastream.bytearray[1];
											dataarray[12] = datastream.bytearray[2];
											dataarray[13] = datastream.bytearray[3];
											cl_data_ret_val = Cl_SendDatatoconsole(command,&dataarray,14);
						break;
						
						default:break;
					}
					
				}
		

				break;
				
				default:
				break;
			}
		}
		break;
		case EVT_CONSOLE_COMMAND_SET_DATA:
		if(Cl_ConsoleRxMsg.msgready == true)
		{
			cl_Datastreamtype cl_temp  ;

			switch(Cl_ConsoleRxMsg.data.byte[0])
				{
					
					case CON_RX_PARAM_DATA_TREATMENT_DATA:
			
					if(Cl_ConsoleRxMsg.data.byte[1] < ID_b_Dlaystedata)
					{
						dataarray[1] = Cl_ConsoleRxMsg.data.byte[1];
						
						datastream.bytearray[0] = Cl_ConsoleRxMsg.data.byte[2];
						datastream.bytearray[1] = Cl_ConsoleRxMsg.data.byte[3];
						datastream.bytearray[2] = Cl_ConsoleRxMsg.data.byte[4];
						datastream.bytearray[3] = Cl_ConsoleRxMsg.data.byte[5];

						Treatdata[dataarray[1]] = datastream.word ;
					}

			

			
					switch(Cl_ConsoleRxMsg.data.byte[1])
					{
						case ID_dflow:
						break;
						case ID_settemp:
						break;
						case ID_heprate:
						break;
						case ID_setcond:
						break;
						case ID_ufrate:
						break;
						case ID_ufgoal:
						break;
						case ID_bolusvol:
						break;
						case ID_bloodrate:
						break;
						case ID_hepstoptime:
						break;
						case ID_syringetype:
						break;
						case ID_heparincheck:
						break;
						case ID_minufrate:
						break;
						case ID_treattime:
						break;
						case ID_bloodratereturn:
						break;
						case ID_bloodratetreat:
						break;
						case ID_tempulimit:
						Cl_alarmThresholdTable.temp3_high_threshold =  (datastream.word)/10;
						break;
						case ID_templlimit:
						Cl_alarmThresholdTable.temp3_low_threshold =  (datastream.word)/10;
						break;
						case ID_tmpllimit:
						Cl_alarmThresholdTable.tmp_low_threshold =  datastream.word;
						break;
						case ID_tmpulimit:
						Cl_alarmThresholdTable.tmp_high_threshold =  datastream.word;
						break;
						case ID_vptllimit:
						Cl_alarmThresholdTable.vpt_low_threshold =  datastream.word;
						break;
						case ID_vptulimit:
						Cl_alarmThresholdTable.vpt_high_threshold =  datastream.word;
						break;
						case ID_ufllimit:
						break;
						case ID_ufulimit:
						break;
						case ID_dflowllimit:
						break;
						case ID_dflowulimit:
						break;
						case ID_condllimit:
						Cl_alarmThresholdTable.cond_low_threshold =  datastream.word;
						break;
						case ID_condulimit:
						Cl_alarmThresholdTable.cond_high_threshold =  datastream.word;
						break;
						case ID_aptllimit:
						Cl_alarmThresholdTable.apt_low_threshold =  datastream.word;
						break;
						case ID_aptulimit:
						Cl_alarmThresholdTable.apt_high_threshold =  datastream.word;
						break;

						default:break;
					}
				break;
				default:break;
			}
							
							
		}
		break;
		
		default:
		break;
		
	}
}