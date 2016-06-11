/*
 * cl_sys_status.c
 *
 * Created: 1/7/2014 2:55:10 PM
 *  Author: user
 */ 
#include "inc/cl_status.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/inc/cl_stubs.h"
#include "Platform/Service/sv_interface.h"
#include "sv_stubs/inc/sv_stub.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_cal/calibration.h"

extern Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_AlarmIdType cl_alarm_id , bool cl_status);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
#if 0
typedef union{
	
	uint8_t bytearray[4] ;
	uint16_t Twobyte  ;
	uint32_t word ;
	
}cl_Datastreamtype;

#endif

typedef union{
	
	uint8_t bytearray[4] ;
	uint16_t Twobyte  ;
	uint32_t word ;
	
}cl_Datastream_bulk_type;

extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

extern uint16_t sv_status_getsystemstatus(Sys_statusType* );
Cl_ReturnCodes 		Cl_SysStat_System_Status_Query(void);




extern Cl_Sys_statusType cl_sys_statbuffer;
extern Cl_ReturnCodes cl_memset(uint8_t* , uint8_t );

extern Cl_ReturnCodes sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );//testing
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );//testing
extern Cl_ReturnCodes Cl_SysStat_Get_Flowcount( uint16_t* pdata);

static float ps1avg=0, ps2avg = 0,ps3avg = 0,ps3avg_1sec = 0, aptavg=0,vptavg=0, ps3avg_max = 0;
static float avgtmp3   = 0 , avgtmp2   = 0, avgtmp1   = 0 , avgcond = 0;
extern volatile float temprature_final_value_1,temprature_final_value_2,temprature_final_value_3;
extern volatile int16_t pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;
uint16_t valve_4_count = 0;
extern bool syncdone;
extern float dummy1,dummy2,dummy3,dummy4,dummy5,dummy6,dummy7;

Cl_ReturnCodes 		Cl_SysStat_GetSensor_Status_Query(Cl_AlarmIdType , uint16_t*);
void Cl_SysStat_mean_status_update_mean_status_update(void);


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
	if(cl_sys_statbuffer.Temp2status > 3500)

	{
		RetVal = sv_cntrl_poweroffheater();
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,( uint8_t*)"TS2_?",5);
	}
	if(cl_sys_statbuffer.Temp3status > 3500)

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
			tempdata =  cl_sys_statbuffer.BPstatus;
			*pdata  = tempdata;
			break;
			case FLOWSTATUS_FLOWON:
			case FLOWSTATUS_FLOWOFF:
			tempdata =  cl_sys_statbuffer.Flowstatus;
			*pdata  = tempdata;
			break;
			case BLOODDOOR_STATUS_OPEN:
			tempdata =  cl_sys_statbuffer.BPstatus;
			*pdata  = tempdata;
			break;
			default:
			break;
			
			

	
		
		}
		
	return CL_OK;
	
}




void Cl_SysStat_mean_status_update(void)
{
	Cl_ReturnCodes cl_thretval = CL_ERROR;
	int16_t Ps1,Ps2,Ps3,Atp,Vtp,TMP = 0;
	int16_t sensordata=0;
	static int16_t prev_flow_status=0;
	int16_t Pressuredata=0;
	int16_t sensordatamillivolts =0;
	int16_t tempdata =0,count1;
	int16_t Pressuredatainmillibars =0;
	uint8_t* pdataarray;
	cl_Datastreamtype cl_tdata;
	int16_t bulk_data[10];
	static uint8_t  flow_counter =0 ;
	static uint16_t counter=0,threeseccounter=0,CS_ontimecnter=0,cnt = 0,levelsw_cnter = 0;
	if(syncdone)
	{
		
		

		
		//Cl_SysStat_GetSensor_Status_Query(PS3STATUS_HIGH, &sensordata);
		sensordata = sensordata * 3300/4096;
		if(syncdone)
		{
			//cl_testgetsensordata(TEMP1STATUS_HIGH,&sensordata);
			Cl_SysStat_GetSensor_Status_Query(TEMP1STATUS_HIGH, &sensordata);
			{
				
				/*float temp,temp1;
				temp = sensordata * 0.805/1000;
				temp1 = 1.830098345 * temp *temp +  22.62524406 *temp + 0.581851503  ;
				avgtmp1 =	(avgtmp1*14 + temp1)/15;
				cl_tdata.word = 0;
				//	cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = temp;
				cl_tdata.bytearray[2] = 1;
				bulk_data[0] = avgtmp1 * 10;*/
				float temp,temp1;
				temp = sensordata * 0.805;
				calibration_tmp(temp,TS1);
				avgtmp1 =	(avgtmp1*5 + temprature_final_value_1)/6;
				bulk_data[0] = avgtmp1*10;
				
				//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			}
			

			
			
			Cl_SysStat_GetSensor_Status_Query(TEMP2STATUS_HIGH,&sensordata);
			{
				
				/*float temp,temp1;
				temp = sensordata * 0.805/1000;
				temp1 =1.830098345 * temp *temp +  22.62524406 *temp + 0.581851503  ;
				avgtmp2 =	(avgtmp2*14 + temp1)/15;
				
				cl_tdata.word = 0;
				cl_tdata.Twobyte = temp;
				cl_tdata.bytearray[2] = 2;
				bulk_data[1] = avgtmp2 * 10;*/
				
				float temp,temp1;
				temp = sensordata * 0.805;
				calibration_tmp(temp,TS2);
				avgtmp2 =	(avgtmp2*5 + temprature_final_value_2)/6;
				bulk_data[1] = avgtmp2*10;
				//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			}
			Cl_SysStat_GetSensor_Status_Query(TEMP3STATUS_HIGH,&sensordata);
			{
				
				/*float temp,temp1;
				temp = sensordata * 0.805/1000;
			//	 y = 1.830098345�10-6 x2 + 2.262524406�10-2 x + 5.818515031�10-1
			//	y = 1.830098345 x2 + 22.62524406 x + 0.581851503
				temp1 = 1.830098345 * temp *temp +  22.62524406 *temp + 0.581851503  ;
				avgtmp3 =	(avgtmp3*14 + temp1)/15;
				if(avgtmp3 > 39)
				{
					cl_thretval = sv_cntrl_poweroffheater();
				}
				cl_tdata.word = 0;
				cl_tdata.Twobyte = avgtmp3;
				cl_tdata.bytearray[2] = 3;
				
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				
				cl_tdata.word = 0;
				cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = avgtmp3;
				cl_tdata.bytearray[2] = 6;
				bulk_data[2] = avgtmp3 * 10 ;*/
				float temp,temp1;
				temp = sensordata * 0.805;
				calibration_tmp(temp,TS3);
				avgtmp3 =(avgtmp3*5 + temprature_final_value_3)/6;
				bulk_data[2] = avgtmp3*10;
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			}
			Cl_SysStat_GetSensor_Status_Query(COND_STATUS_HIGH,&sensordata);
			{
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&sensordata,2);
					static int16_t temp,temp1=0;
					
				//	temp = 1350 + (  sensordata - 2438 ) * 387/126;
					
					temp1 =	(temp1 * 29 + sensordata)/30;
					temp = avgcond;
					Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
					//temp1 = 0.006 * avgcond * avgcond - 30.35 * avgcond + 37830;

				//	temp = (0.8056 * sensordata) - 1450 ;
				//	temp1 = 3700 + (temp * 1000)/340;
				
				//	pdataarray = (uint8_t *)"TEMP1=";
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
				
				if( temp < 0)
				{
					temp = 0;
				//	avgcond = 0;
				}
				
										

					/*	if( sensordata < 0)
						{
								temp = 0;
							//	avgcond = 0;
						}
						if( sensordata > 2500)
						{
							avgcond = temp1/29.6 + 11;
							//avgcond = avgcond * (1- ((avgtmp3- 25) * 0.02));
						}
						else
						{
							avgcond = dummy1;
						}
						
					cl_tdata.word = 0;
					cl_tdata.Twobyte = avgcond;
					cl_tdata.bytearray[2] = 12;
				bulk_data[3] = avgcond;*/
					if( sensordata < 0)
					{
						//temp = 0;
						avgcond = 0;
					}
					if( sensordata > 2400)
					{
						//avgcond = temp1/29.6 + 11;
						//avgcond = avgcond * (1- ((avgtmp3- 25) * 0.02));
						temp=sensordata  * 0.805;
						avgcond=temp*4.48;
						avgcond = (avgcond)/100;
						
					}
					else
					{
						//avgcond = dummy1;
						avgcond=100;
					}
					
					cl_tdata.word = 0;
					cl_tdata.Twobyte = avgcond;
					cl_tdata.bytearray[2] = 12;
					bulk_data[3] = avgcond;
			//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			}
			Cl_SysStat_GetSensor_Status_Query(FLOWSTATUS_FLOWON,&sensordata);
			{

				cl_tdata.word = 0;
				cl_tdata.Twobyte = sensordata;
				cl_tdata.bytearray[2] = 3;
				bulk_data[4] = sensordata;
				if(prev_flow_status == sensordata)
				{
					if(flow_counter++ > 10)
					{
						//	cl_thretval = sv_cntrl_poweroffheater();
					}
					
				}
				else
				{
					flow_counter = 0;
				}
				prev_flow_status = sensordata;
				
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			}

			
			//	cl_testgetsensordata(APTSTATUS_HIGH, &Atp);//0x0F
			
			Cl_SysStat_GetSensor_Status_Query(APTSTATUS_HIGH, &Atp);
			/*sensordatamillivolts = ((Atp* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts - 1580) * 0.602;
			aptavg = ((aptavg* 9) + Pressuredatainmillibars) /10;
			
								cl_tdata.word = 0;
								cl_tdata.Twobyte = aptavg -70  + 1000;
								cl_tdata.bytearray[2] = 14;
								
			//					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
						bulk_data[5] = cl_tdata.Twobyte;*/
				//		bulk_data[5] = dummy4 * 10;
				sensordatamillivolts = (Atp * 0.793) ;
				calibration_apt(sensordatamillivolts);
				aptavg = ((aptavg* 10) + pressure_final_apt)/11;
				cl_tdata.word = 0;
				cl_tdata.Twobyte = aptavg + 1000;
				cl_tdata.bytearray[2] = 14;
				bulk_data[5] = cl_tdata.Twobyte;
						
			Cl_SysStat_GetSensor_Status_Query(VPTSTATUS_HIGH, &Vtp);
			/*sensordatamillivolts = ((Vtp* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts - 1580) * 0.602;
			vptavg = ((vptavg* 9) + Pressuredatainmillibars) /10;
			
								cl_tdata.word = 0;
								cl_tdata.Twobyte = vptavg - 70 + 1000;
								cl_tdata.bytearray[2] = 15;
									bulk_data[6] = cl_tdata.Twobyte;
						//		bulk_data[6] = dummy3 * 10;
		//						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);*/
			sensordatamillivolts = (Vtp * 0.805) ;
			//sensordatamillivolts = ((Atp* 3300 /4096) ) ;
			calibration_vpt(sensordatamillivolts);
			vptavg = ((vptavg* 10) + pressure_final_vpt)/11;
			cl_tdata.word = 0;
			cl_tdata.Twobyte = vptavg + 1000;
			cl_tdata.bytearray[2] = 15;
			bulk_data[6] = cl_tdata.Twobyte;
			
			Cl_SysStat_GetSensor_Status_Query(PS3STATUS_HIGH,&Ps3);
			/*sensordatamillivolts = ((Ps3* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts * 0.892) - 1004  ;
			ps3avg = ((ps3avg* 19) + Pressuredatainmillibars) /20;
			ps3avg_1sec = ((ps3avg_1sec* 4) + Pressuredatainmillibars) /5;*/
			sensordatamillivolts = (Ps3 * 0.793) ;
			calibration_ps3(sensordatamillivolts);
			ps3avg = ((ps3avg* 10) + pressure_final_ps3)/11;
			ps3avg_1sec = ((ps3avg_1sec* 4) + Pressuredatainmillibars) /5;
			if ( ps3avg_1sec < (ps3avg ))
			{
				
				if(ps3avg_1sec < (ps3avg - 5) )
				{
					//switch chamber
				//	Cl_Alarm_TriggerAlarm(FPCURRENTSTATUS,1);
					
				}
				ps3avg_max = ps3avg;
			}
			if ( ps3avg < ps3avg_max)
			{
				

				ps3avg_max = ps3avg;
			}			
			cl_tdata.word = 0;
			cl_tdata.Twobyte = 1000 + ps3avg;
			cl_tdata.bytearray[2] = 20;
			bulk_data[7] = cl_tdata.Twobyte;
	//		bulk_data[7] = dummy7  + 1000;
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			/////////////////
			
			Cl_SysStat_GetSensor_Status_Query(PS2STATUS_HIGH,&Ps2);
		/*	sensordatamillivolts = ((Ps2* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts * 0.892) - 1004  ;
			ps2avg = ((ps2avg* 4) + Pressuredatainmillibars) /5;*/
		sensordatamillivolts = (Ps2 * 0.793) ;
		calibration_ps2(sensordatamillivolts);
		ps2avg = ((ps2avg* 5) + pressure_final_ps2)/6;
			

			cl_tdata.word = 0;
			cl_tdata.Twobyte = 1000 + ps2avg;
			cl_tdata.bytearray[2] = 19;
			bulk_data[8] = cl_tdata.Twobyte;
		//	bulk_data[8] = dummy6 * 100 + 1000;
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			
			
			Cl_SysStat_GetSensor_Status_Query(PS1STATUS_HIGH,&Ps1);
			/*sensordatamillivolts = ((Ps1* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts * 0.892) - 1004  ;
			ps1avg = ((ps1avg* 4) + Pressuredatainmillibars) /5;*/
			sensordatamillivolts = (Ps1 * 0.793) ;
			calibration_ps1(sensordatamillivolts);
			ps1avg = ((ps1avg* 10) + pressure_final_ps1)/11;
			
			cl_tdata.word = 0;
			cl_tdata.Twobyte = 1000 + ps1avg;
			cl_tdata.bytearray[2] = 18;
			bulk_data[9] = cl_tdata.Twobyte;
			
		//	bulk_data[9] = dummy2 + 1000;
			if(ps1avg > 300)
			{
				//  sv_cntrl_activate_valve(VALVE_ID4 );
				  valve_4_count++;
			}
			else
			{
				 valve_4_count++;
				if(valve_4_count >= 1)
				{
				//  sv_cntrl_deactivate_valve(VALVE_ID4 );
				  valve_4_count = 0;
					
				}
			}
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			
			
			/*
			
			TEMP1_IDX = 0
			TEMP2_IDX = 1
			TEMP3_IDX = 2
			CON_IDX = 3
			APT_IDX = 5
			VPT_IDX = 6

			PS1_IDX = 7
			PS2_IDX = 8
			PS3_IDX = 9
			*/
		//	bulk_data[0] = 100;
		//	bulk_data[1] = 101;
		//	bulk_data[2] = 102;
		//	bulk_data[3] = 103;
		//	bulk_data[4] = 104;
		//	bulk_data[5] = 105;
		//	bulk_data[6] = 106;
		//	bulk_data[7] = 107;
		//	bulk_data[8] = 108;
		//	bulk_data[9] = 109;

		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT,&bulk_data,20);
		}
		
	}
}