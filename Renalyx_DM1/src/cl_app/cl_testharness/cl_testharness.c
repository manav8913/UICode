/*
 * cl_testharness.c
 *
 * Created: 1/22/2014 3:17:23 PM
 *  Author: user
 */ 




#include "cl_app/inc/cl_stubs.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_dprep/inc/cl_dprep_primecontroller.h"
#include "cl_app/cl_dlsis/inc/cl_dlsis_controller.h"
#include "cl_app/cl_dprep/inc/cl_dprep_controller.h"
#include "cl_app/cl_mac/inc/cl_mac_controller.h"
#include "cl_app/cl_testharness/inc/cl_th.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "sv_stubs/inc/sv_stub.h"
#include "Platform/Service/sv_interface.h"
#include "Platform/Drivers/DD_IIC/DD_IIC.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_rinse/inc/cl_rinse_controller.h"
#include "cl_app/cl_heatcntrl/inc/cl_heatercontroller.h"
#include "cl_app/cl_cal/calibration.h"

extern void DD_IIC_READ_PROP(uint8_t iic_address, uint16_t* data);

extern void DD_IIC_SEND_PROP(uint8_t iic_address, uint8_t* data);
extern Sv_ReturnCodesType sv_nvmsetdata(uint8_t dataItemId,uint8_t* data,uint8_t);
extern Sv_ReturnCodesType  sv_nvmgetdata(uint8_t,uint8_t*);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern Cl_ReturnCodes cl_wait(uint32_t ul_dly_ticks);
extern Cl_ReturnCodes Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType, uint16_t*);
extern uint8_t sv_cntrl_activate_valve(sv_valvetype );
extern uint8_t sv_cntrl_deactivate_valve(sv_valvetype );
extern uint8_t sv_cntrl_activatepump(sv_pumptype );
extern uint8_t  sv_cntrl_deactivatepump(sv_pumptype);
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
extern uint8_t sv_cntrl_setpumpfreq(sv_pumptype sv_pump_id,uint32_t freq);
extern uint8_t sv_cntrl_activatevenousclamp(void);
extern uint8_t sv_cntrl_deactivatevenousclamp(void);
extern uint8_t sv_cntrl_poweronheater(void);
extern uint8_t sv_cntrl_poweroffheater(void);
extern uint8_t sv_cntrl_incheater(int32_t dty_val);
extern Cl_AlarmActivateAlarms(Cl_NewAlarmIdType,bool);
extern Cl_ReturnCodes Cl_Alarm_TriggerDummyAlarm(Cl_NewAlarmIdType cl_alarm_id , bool cl_status);
extern uint8_t sv_cntrl_resetHepa_dir(void);
extern uint8_t sv_cntrl_setHepa_dir(void);
extern uint8_t sv_cs_setpotvalue(uint32_t resistance) ;
extern uint8_t sv_cs_readpotvalue(uint16_t* resistance);
extern Cl_ReturnCodes 	Cl_Mac_GetSystem_Status_Query(void);
extern void sv_prop_startmixing();
extern void sv_prop_stopmixing();
extern void sv_prop_startpropeo_aligning();
extern void sv_prop_set_propro_rc(uint8_t data);

extern uint8_t sv_cntrl_enable_bypass(void);
extern uint8_t sv_cntrl_disable_bypass(void);
extern uint8_t sv_cntrl_enable_loopback(void);
extern uint8_t sv_cntrl_disable_loopback(void);

extern uint8_t sv_cs_setcondpotvalue(uint16_t resistance);

extern Cl_ReturnCodes SetHeaterState(HeaterStateType Param_HeaterState);

extern float avgtmp2_target_cel;
extern float prescribed_temp;
extern float prev_pres_temp;
extern Cl_Sys_statusType cl_sys_statbuffer;
extern int Cl_RinseMinutescounter;
extern uint8_t sys_sw_version;
extern bool syncdone;
extern IIC_PROPO_CommandType PROPO_Command_Queue[];
extern IIC_BP_CommandType BP_Command_Queue[];

extern Cl_AlarmThresholdType  Cl_alarmThresholdTable;
extern volatile float temprature_final_value_2;
extern Cl_AlarmStructType Cl_alarms_alarms[_ALARM_MAX_ID];

 bool current_sense = false;
 bool test_enabled = false;
 	
 MAC_STATES MacStateDummy;
 Cl_Rinse_States Rinsestatedummy;
 Cl_Dprep_PrimeStates cl_dprep_prime_stateDummy;
 Cl_Dprep_States cl_dprepstatedummy;
 Cl_Dlsis_States cl_dlsis_state_dummy;
  bool Loopbackstatus;
  bool Bypassstatus;

Cl_ReturnCodes cl_testharnesscommandhandler(Cl_ConsoleMsgType*);
Cl_ReturnCodes  cl_testgetsensordata(uint8_t dataId, uint16_t* dataItem);
Cl_ReturnCodes  cl_testgetversion( uint8_t* );
uint16_t lookup_condtable(uint8_t cond_base,uint16_t cond_mvolt);
bool TEST_STUB_ENABLEGATE = 1;
bool prev_gate_status =1;
int array_commands[100] = {};
Cl_NewAlarmIdType cl_testalarm_id ;
 bool heater_update_disble = false ;
 Cl_Sys_statusType  cl_sys_statbuffer_test;

void DD_IIC_SET_BLOODPUP(uint8_t iic_address, uint32_t data,uint8_t length);

void Cl_tg_prick_100ms(void)	;
void Cl_tg_prick_50ms(void)	;
void pinclock(void);
void pinclock1(void);
void testsectclock(void);
	static uint16_t cond_lookup[20] =
	{
		780,//3
		1010,//4
		1210,//5
		1390,//6
		1550,//7
		1700,//8
		1850,//9
		1980,//10
		2110,//11
		2250,//12
		2370,//13
		2480,//14
		2600,//15
		2660,//16


	};
	
Cl_NewAlarmIdType alarmmap[_ALARM_MAX_ID] =
{
	_NO_ALARM,//0x00, //0
	BLOODDOOR_STATUS_OPEN,//0x01
	BYPASSDOOR_STATUS_OPEN,//0x02
	HOLDER1STATUS_OPEN, // 0x3
	HOLDER2STATUS_OPEN,// 0x04
	HOLDER1STATUS_CLOSED, // 0x3
	HOLDER2STATUS_CLOSED,// 0x04
	COND_STATUS_LOW,//0x7
	COND_STATUS_HIGH,//0x8
	COND_DAC_OPEN,//0x9
	COND_DAC_RO,//0xA
	COND_DAC_HIGH,//0xB
	FLOW_LOW_FLOWRATE,//ox23
	FLOW_LOW_FLOWRATE,//ox23
	FLOW_HIGH_FLOWRATE,//0x24
	//LEVEL_SWITCH_STOPPED,//0x25
	LEVEL_SWITCH_LOW_TOGGLERATE,//0x26
	LEVEL_SWITCH_HIGH_TOGGLERATE,
	TEMP1_HIGH_THRESHOLD,//0x16
	TEMP1_LOW_THRESHOLD,//0x16
	SENSOR_TEMP2STATUS,//0x17
	//TEMP2STATUS_LOW,//0x17
	SENSOR_TEMP3STATUS,//0x18
//	TEMP3STATUS_LOW,//0x18
//	PS1STATUS_HIGH,//0x12
	//PS1STATUS_LOW,//0x122
	//PS2STATUS_HIGH,//0x13
	//PS2STATUS_LOW,//0x13
	//UF_ROTATION_MARKER,//0x1B
	///ALARM_MAX_ID, //26
	//ALARM_MAX_ID, //27
	SYSTEM_NOT_READY,//28
	POWER_FAILURE,//29
	POWER_FAILURE , //30
	WATCHDOG_TIMER , //31
};
uint16_t mix_count = 0;
bool BC_window = false;
bool start_pressurecapture = false;
bool pressuredataready = false;
bool g_testbcfreeze = false;
bool Current_sense_trigger = true;
bool dummy_currentsence = false;
bool dummy_levelswitchstatus = false;
bool gflow_en = false;
int16_t PS3_threshold = 0 ;
extern int16_t ps3_threshold ;
static int16_t ps1avg=0, ps2avg = 0,ps3avg = 0, aptavg=0,vptavg=0;
bool gtest_short_rinse = false ;
extern uint32_t heater_duty ;
bool iic_nack = false;
static uint16_t uf_rotation_counter = 0;
static int16_t avgtmp3   = 0;
//array_command_type cmd_backup;
testsensorType test_sensor_status[SENSOR_MAX_INPUT];

Cl_ReturnCodes cl_testharnesscommandhandler(Cl_ConsoleMsgType* pCl_ConsoleMsg)
{
	Cl_ReturnCodes cl_thretval = CL_ERROR;
	cl_Datastreamtype cl_indata  ;
	Cl_ConsoleMsgType TestMsg;
	uint8_t datasize;
	uint8_t version;
	int16_t sensordata=0;
	int16_t Pressuredata=0;
	int16_t sensordatamillivolts =0;
	int16_t tempdata =0;
	int16_t Pressuredatainmillibars =0;
	int16_t Ps1,Ps2,Atp,Vtp,TMP = 0;
	uint8_t dataarray[20];
	uint8_t datatxarray[20];
	uint8_t* pdataarray;
	uint8_t temp;
	uint32_t rateval;
	Cl_Testcommandtype test;
	uint32_t count1;
	
	cl_indata.bytearray[0] = 0;
	cl_indata.bytearray[1] = 0;
	cl_indata.bytearray[2] = 0;
	cl_indata.bytearray[3] = 0;
	cl_indata.Twobyte = 0;
	cl_indata.word = 0;
	
	
	for( temp =0; temp < MAX_DATA_SIZE ;temp++ )
	{
		TestMsg.data.byte[temp] = 0;
		dataarray[temp]=0;
		datatxarray[temp]=0;
				
	}
	
	TestMsg.datasize = pCl_ConsoleMsg->datasize;
	
	for( temp =0; temp <TestMsg.datasize ;temp++ )
	{
		TestMsg.data.byte[temp] = pCl_ConsoleMsg->data.byte[temp];
		dataarray[temp]= pCl_ConsoleMsg->data.byte[temp];
				
	}
	




	switch ((Cl_Testcommandtype)TestMsg.data.byte[0])
	//switch (test)
	{
		case TEST_HEATER_UPDATE_DISABLE:
		heater_update_disble = true;
		break;
		case TEST_HEATER_UPDATE_ENABLE:
		heater_update_disble = false;
		break;
		case TEST_ENABLE_ALARM:
		prev_gate_status = TEST_STUB_ENABLEGATE;
		TEST_STUB_ENABLEGATE = 1;
		cl_testalarm_id = TestMsg.data.byte[1];
		//Cl_AlarmActivateAlarms(alarmmap[cl_testalarm_id],true);
		Cl_AlarmActivateAlarms(cl_testalarm_id,true);
		TEST_STUB_ENABLEGATE = prev_gate_status;
		break;
		case TEST_DISABLE_ALARM:
		prev_gate_status = TEST_STUB_ENABLEGATE;
		TEST_STUB_ENABLEGATE = 1;
			cl_testalarm_id = TestMsg.data.byte[1];
		Cl_AlarmActivateAlarms(cl_testalarm_id,false);
		TEST_STUB_ENABLEGATE = prev_gate_status;
		
		break;
		case TEST_DISABLE_ALL_ALARMS:
		prev_gate_status = TEST_STUB_ENABLEGATE;
		TEST_STUB_ENABLEGATE = 1;
		temp = 0;
		while ( temp < _ALARM_MAX_ID )
		Cl_AlarmActivateAlarms(temp++,false);
		TEST_STUB_ENABLEGATE = 0;
			static int16_t ms_count = 0;
			cl_Datastreamtype cl_tdata;
			ms_count++;

				
			//	cl_tdata.word = 0;
				//cl_tdata.Twobyte = sensordata;
			//	cl_tdata.Twobyte = 0xFABC;
			//	cl_tdata.bytearray[2] = 17;
			//	current_sense = false;
			//	Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				
			//	ms_count = 0;
				
			
		break;
		
		case TEST_SET_BP_DC_DAC:
		cl_indata.bytearray[0] = TestMsg.data.byte[1];
		
		cl_indata.bytearray[0] = TestMsg.data.byte[1];
		if(cl_indata.bytearray[0] & 0x01)
		{
			PIOC->PIO_SODR = 1 <<28;
			
		}
		else
		{
			PIOC->PIO_CODR = 1 <<28;
		}
		if(cl_indata.bytearray[0] & 0x02)
		{
			PIOC->PIO_SODR = 1 <<26;
			
		}
		else
		{
			PIOC->PIO_CODR = 1 <<26;
		}
		if(cl_indata.bytearray[0] & 0x04)
		{
			PIOC->PIO_SODR = 1 <<25;
			
		}
		else
		{
			PIOC->PIO_CODR = 1 <<25;
		}
		if(cl_indata.bytearray[0] & 0x08)
		{
			PIOC->PIO_SODR = 1 <<29;
		}
		else
		{
			PIOC->PIO_CODR = 1 <<29;
		}
		break;
		case TEST_SET_ALARM_COND:
		
		cl_sys_statbuffer.cond_status = TestMsg.data.byte[1];
		
		cl_thretval = CL_OK;
		break;
		case TEST_SET_ALARM_CS1:
		
		cl_sys_statbuffer.cs1status = TestMsg.data.byte[1];
				cl_thretval = CL_OK;
		break;
		case TEST_SET_ALARM_CS2:
		cl_sys_statbuffer.cs2status = TestMsg.data.byte[1];
				cl_thretval = CL_OK;
		break;
		case TEST_SET_ALARM_CS3:

		break;
		case TEST_SET_ALARM_PS1:
		break;
		case TEST_SET_ALARM_PS2:
		break;
		case TEST_SET_ALARM_PS3:
		break;
		case TEST_SET_ALARM_FL1:
		
		if(TestMsg.data.byte[1] == 0 )
		cl_sys_statbuffer.levelswitch1 = false;
		else
		cl_sys_statbuffer.levelswitch1 = true;
		break;
		
		break;

		case TEST_SET_ALARM_FL2:
		if(TestMsg.data.byte[1] == 0 )
		cl_sys_statbuffer.levelswitch2 = false;
		else
		cl_sys_statbuffer.levelswitch2 = true;
		break;
		case TEST_SET_ALARM_HOLD1:
		if(TestMsg.data.byte[1] == 0 )
		cl_sys_statbuffer.holder1status = false;
		else 
		cl_sys_statbuffer.holder1status = true;
		break;
		case TEST_SET_ALARM_HOLD2:
		if(TestMsg.data.byte[1] == 0 )
		cl_sys_statbuffer.holder2status = false;
		else
		cl_sys_statbuffer.holder2status = true;
		break;
		case TEST_SET_ALARM_TEMP1:
		cl_sys_statbuffer.Temp1status = TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_TEMP2:
		cl_sys_statbuffer.Temp2status = TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_TEMP3:
		cl_sys_statbuffer.Temp3status = TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_CURRENT_SENSE_ON:
		dummy_currentsence = true;
		break;
		case TEST_SET_ALARM_CURRENT_SENSE_OFF:
		dummy_currentsence = false;
		break;
		case TEST_SETDATA:
		cl_thretval = sv_nvmsetdata( TestMsg.data.byte[1], &(dataarray[2]),TestMsg.datasize);
	//	cl_thretval = sv_nvmsetdata( dataarray[1], &dataarray[2],TestMsg.datasize);
		cl_thretval = sv_nvmgetdata( dataarray[1], &datatxarray[0]);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"SET",3);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&datatxarray[0],2);
		break;
		case TEST_GETDATA:

	if( dataarray[1] == NV_DIALYSATE_TEMP || dataarray[1] == NV_APT_VALUE ||dataarray[1] == NV_VPT_VALUE ||  dataarray[1] == NV_CONDUCTIVITY   )
	{
		
		cl_thretval = cl_testgetsensordata( dataarray[1], &datatxarray[0]);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND=",5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&datatxarray[0],1);
	}
	else
	{
		cl_thretval = sv_nvmgetdata( dataarray[1], &datatxarray[0]);
	}
		//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_TEST_SYSDATA,&datatxarray,2);
		
	
	

		break;
		case TEST_SET_ALARM_ABDSTATUS://14
		cl_sys_statbuffer.abdstatus =  TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_BDSTATUS://15
		test_enabled = false;
		cl_sys_statbuffer_test.bdstatus =  TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_APTSTATUS://16
		cl_sys_statbuffer.aptstatus =  TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_BPTSTATUS://17
		cl_sys_statbuffer.vptstatus =  TestMsg.data.byte[1];
		break;
		case TEST_SET_ALARM_BLDSTATUS://18
		cl_sys_statbuffer.bldstatus =  TestMsg.data.byte[1];
		break;
		case TEST_SET_HEATERON://63 0x3F
		cl_wait(100);
		pdataarray = (uint8_t *)"HE_ON";
		cl_thretval = sv_cntrl_poweronheater();
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_HEATEROFF://64 0x40
		cl_wait(100);
		pdataarray = (uint8_t *)"HE_OFF";
		cl_thretval = sv_cntrl_poweroffheater();
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
		//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_FLOW_PUMPON://21 0x15
		cl_wait(100);
		pdataarray = (uint8_t *)"FP_ON";
		cl_thretval = sv_cntrl_activatepump(DCMOTOR1);
		//cl_thretval =  sv_cntrl_activatepump(BLOODPUMP);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
	case TEST_SET_FLOW_PUMPON_ONESEC://21 0x15
	cl_wait(100);
	pdataarray = (uint8_t *)"FP_ON";
	//cl_thretval = sv_cntrl_activatepump(DCMOTOR1);
	uint32_t t = 300;
	//sv_cntrl_setpumpspeed(BLOODPUMP,t);
//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
	cl_wait(2000);
	cl_thretval = sv_cntrl_deactivatepump(DCMOTOR1);
	break;
	
	case TEST_SET_FLOW_PUMPOFF://22 0x16
	cl_wait(100);
	pdataarray = (uint8_t *)"FP_OFF";
	cl_thretval =  sv_cntrl_deactivatepump(DCMOTOR1);
	//cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
	
//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
	cl_wait(100);
	break;	
	case TEST_SET_CS_POT_VALUE:
//while (1)
{
	cl_wait(10);
			if(iic_nack)
			{
//				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"NACK",4);
				iic_nack = false;
			}
	 if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
		
			
		}
		else if(TestMsg.datasize == 4)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[2];
			cl_indata.bytearray[0] = TestMsg.data.byte[3];
			
		}
		if(TestMsg.data.byte[1] == 2)
		{
			uint16_t potvalue = 0;
				pdataarray = (uint8_t *)"CS_POT_SET";
				potvalue = (cl_indata.Twobyte * 1024)/10000;
			sv_cs_setpotvalue(potvalue);
			
		}
		if(TestMsg.data.byte[1] == 1)
		{
			//set conductivity pot 
		}
		
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,10);
	//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_indata.Twobyte,2);
//	cl_wait(20);
	cl_indata.word = 0;
//	sv_cs_readpotvalue(&cl_indata.Twobyte);
}
	break;
	case TEST_SET_COND_POT_VALUE:
	//while (1)
	{
		cl_wait(10);
		if(iic_nack)
		{
			//				Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"NACK",4);
			iic_nack = false;
		}
		if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
			
		}
		else if(TestMsg.datasize == 4)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[2];
			cl_indata.bytearray[0] = TestMsg.data.byte[3];
			
		}
		if(TestMsg.data.byte[1] == 2)
		{
			uint16_t potvalue = 0;
			pdataarray = (uint8_t *)"CS_POT_SET";
			potvalue = (cl_indata.Twobyte * 1024)/10000;
			sv_cs_setcondpotvalue(potvalue);
			
		}
		if(TestMsg.data.byte[1] == 1)
		{
			//set conductivity pot
		}
		
		
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,10);
		//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_indata.Twobyte,2);
		//	cl_wait(20);
		cl_indata.word = 0;
		//	sv_cs_readpotvalue(&cl_indata.Twobyte);
	}
	break;
	
	case TEST_GET_SYSTEM_STATE:
	{
		
	
	uint8_t system_status[10];
	 system_status[0] = MacStateDummy;
	 system_status[1] =  Rinsestatedummy;
	 system_status[2] =  cl_dprep_prime_stateDummy;
	 system_status[3] =  cl_dprepstatedummy;
	 system_status[4] =  cl_dlsis_state_dummy;
	 system_status[5] =  Loopbackstatus;
	 system_status[6] =  Bypassstatus;
	 
	 cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SYSTEM_STATUS,system_status,7);
	}
	
	break;
	
	case TEST_GET_ALARM_TABLE:
	{
		
	
		uint8_t temparray[4];
		uint8_t cntr = 0, cntr_1 =0;
	
		for ( cntr =0; cntr <  _ALARM_MAX_ID ; cntr++)
		{
				temparray[cntr_1++] = cntr;
				temparray[cntr_1++] = Cl_alarms_alarms[cntr].cl_is_enabled;
				temparray[cntr_1++] = Cl_alarms_alarms[cntr].cl_is_raised;
				temparray[cntr_1] = Cl_alarms_alarms[cntr].cl_alarmstate;
			
				cntr_1 = 0;
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_ALARM_STATUS,temparray,4);
		
		}
			
	}
	
	break;
	case TEST_GET_IIC_BUFFER:
	
	break;
	case TEST_GET_ALARM_LIMIT:
	
	break;
	
	
		case TEST_SET_FLOW_PUMPRATE ://23 0x17
		cl_wait(100);
		pdataarray = (uint8_t *)"FP_RATE";
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR1,cl_indata.word);
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_indata.word,TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_FLOW_PUMP_FREQ:
		cl_wait(100);
		pdataarray = (uint8_t *)"FP_FRQ";
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		//cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR2,cl_indata.word);
		sv_cntrl_setpumpfreq(DCMOTOR1 , cl_indata.word);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DEGAS_PUMPON://24 0x18
		cl_wait(100);
		cl_thretval =  sv_cntrl_activatepump(DCMOTOR2);
		pdataarray = (uint8_t *)"DGP_ON";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DEGAS_PUMPOFF://25 0x19
		cl_wait(100);
		cl_thretval =  sv_cntrl_deactivatepump(DCMOTOR2);
		pdataarray = (uint8_t *)"DGP_OFF";
//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DEGAS_PUMPRATE://26 0x1A
		cl_wait(100);
		pdataarray = (uint8_t *)"DGP_RATE";
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR2,cl_indata.word);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_indata.word,TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DG_PUMP_FREQ:
		cl_wait(100);
		pdataarray = (uint8_t *)"DGP_FRQ";
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		//cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR2,cl_indata.word);
		 sv_cntrl_setpumpfreq(DCMOTOR2 , cl_indata.word);
	///	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		
		case 	TEST_SET_BC_PS_THRESHOLD:
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
			ps3_threshold = cl_indata.word *  1.8432;
			pdataarray = (uint8_t*) "PS_T";
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_indata.word,TestMsg.datasize-1);
			

			
		
		
		break;
		case TEST_SET_UF_PUMPON://27 0x1B
		cl_wait(100);
		cl_thretval =  sv_cntrl_activatepump(UFPUMP);
		sv_cntrl_setpumpspeed(UFPUMP,10000);
		break;
		pdataarray = (uint8_t *)"UFP_ON";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_UF_PUMPOFF://28 0x1C
		cl_wait(100);
		
		cl_thretval =  sv_cntrl_deactivatepump(UFPUMP);
		pdataarray = (uint8_t *)"UFP_OFF";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_UF_PUMPRATE://29 0x1D
		cl_wait(100);
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		sv_cntrl_setpumpspeed(UFPUMP,cl_indata.word);
		uf_rotation_counter = 0;
		pdataarray = (uint8_t *)"UFP_RATE";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
				
		case TEST_SET_VCLAMP_ON://32 0x20
		cl_wait(100);
		//sv_cntrl_activatevenousclamp();
	//	pdataarray = (uint8_t *)"VC_ON";
					sv_cntrl_activate_valve( VALVE_ID4);
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
					cl_wait(50);
					sv_cntrl_deactivate_valve(VALVE_ID4);
					cl_wait(50);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;

				
		case TEST_SET_VLA_PUMP_PUMPON://81 0x51
		cl_wait(100);
				
		cl_thretval =  sv_cntrl_activatepump(VLA_PUMP);
				
		pdataarray = (uint8_t *)"VLA_PUMP_ON";
				
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,10);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_VLA_PUMP_PUMPOFF://82 0x52
		cl_wait(100);
				
		cl_thretval =  sv_cntrl_deactivatepump(VLA_PUMP);
				
		pdataarray = pdataarray = (uint8_t *)"VLA_PUMP_OFF";
				
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
				
		case TEST_SET_BLOOD_PUMPON://33 0x21
		cl_wait(100);
				

				
		//pdataarray = (uint8_t *)"BP_ON";
		uint16_t temp_10;
		uint16_t potvalue1,read_data = 0;
		//for ( temp_10 = 0 ; temp_10 < 10000; temp_10++)
		{
		// cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSER_PRIME_STARTED,0,0);
	//	for ( temp_10 = 0 ; temp_10 < 5000; temp_10++)
		{
		//	cl_wait(1);
		cl_thretval =  sv_cntrl_activatepump(BLOODPUMP);
			//		read_data = 0;
			//		cl_wait(1);
			//		 DD_IIC_READ_PROP(0x60,&read_data);
					
			//	   if(	read_data == (temp_10 +1) 	)
				   {
					 //  cl_wait(100);
					//   cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
				   }//else
				   {
					   
			//		   cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALYSER_PRIME_STOPPED,0,0);
			//		   cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_DIALISYS_PRIME_COMPLETED,0,0);
				   }
	//	cl_wait(100);
		
			
		}

		//cl_wait(1);
	//	DD_IIC_SEND_PROP( 0x60, 3);
		
	//	potvalue1 = (temp_10 * 1024)/10000;
	//	cl_thretval =  	sv_cs_setpotvalue(potvalue1);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
	//	cl_wait(1);
	//	cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
		//potvalue1 = 200;
	//	cl_thretval =  	sv_cs_setpotvalue(potvalue1);
	//	if(!(temp_10 % 50 ))
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp_10,2);
	//	cl_wait(1);
		}
		
		break;		

		case TEST_SET_BLOOD_PUMPOFF://34 0x22
		cl_wait(100);
		
		cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
		
	//	pdataarray = (uint8_t *)"BP_OFF";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_BLOOD_PUMPSRATE://35 0x23
		cl_wait(100);
				if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		sv_cntrl_setpumpspeed(BLOODPUMP,cl_indata.word);
		pdataarray = (uint8_t *)"BP_RATE";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		
		case TEST_SET_PIC_COUNT:
				cl_wait(100);
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
		//uint8_t bloodpumpspeed[3];
	
		//bloodpumpspeed[0] = 0x04 ;
		//bloodpumpspeed[1] = 0xff & cl_indata.word ;
		//bloodpumpspeed[2] = 0xff & cl_indata.word >> 8 ;
		



			   //DD_IIC_SET_BLOODPUP( 0x0E,  &bloodpumpspeed , 0x03);

	
	
		break;
		case TEST_SET_PIC_CURRENT:
			cl_wait(100);
		if(TestMsg.datasize == 2)
		{
			//rateval = TestMsg.data[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[1];
		}
		else if(TestMsg.datasize == 3)
		{
			//rateval = (uint16_t)TestMsg.data[1];
			cl_indata.bytearray[1] = TestMsg.data.byte[1];
			cl_indata.bytearray[0] = TestMsg.data.byte[2];
			
		}
		
	//	uint8_t bloodpumpspeed1[3];
	
	//	bloodpumpspeed1[0] = 0x05 ;
	//	bloodpumpspeed1[1] = 0xff & cl_indata.word ;
	//	bloodpumpspeed1[2] = 0xff & cl_indata.word >> 8 ;
		



		//	   DD_IIC_SET_BLOODPUP( 0x0E,  &bloodpumpspeed1 , 0x03);
		break;
		case  TEST_SET_CPU2_RESUME:
				//cl_wait(100);
				//uint8_t cpu2command[3];
				
				//cpu2command[0] = 0x0A ;
			//	bloodpumpspeed1[1] = 0xff & cl_indata.word ;
			//	bloodpumpspeed1[2] = 0xff & cl_indata.word >> 8 ;
				



			//	DD_IIC_SET_BLOODPUP( 0x0E,  &cpu2command , 0x01);
		break;
		case TEST_SET_VCLAMP_OFF://36 0x24
		cl_wait(100);
		sv_cntrl_deactivatevenousclamp();
		pdataarray = (uint8_t *)"VC_OFF";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_WATERINLET_ON://35 0x23
		cl_wait(100);
		sv_cntrl_setHepa_dir();
		pdataarray = (uint8_t *)"WI_ON";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_WATERINLET_OFF://36 0x24
		cl_wait(100);
		sv_cntrl_resetHepa_dir();
		pdataarray = (uint8_t *)"WI_OFF";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DRAIN_ON://39 0x27
		cl_wait(100);
		pdataarray = (uint8_t *)"DRAIN_ON";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DRAIN_OFF://40 0x28
		cl_wait(100);
		pdataarray = (uint8_t *)"DRAIN_OFF";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_DAC2_VALVE_OPEN: // 42 0x30
		//cl_wait(100);
		pdataarray = (uint8_t *)"V_OPEN";
		sv_cntrl_activate_valve( VALVE_ID4);
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		sv_cntrl_deactivate_valve(VALVE_ID4);
		cl_wait(100);
		break;
		
		case TEST_SET_FLOWRATE:
		
		if(TestMsg.data.byte[1] == 0)
		{
			sv_cs_setpotvalue(3800);
			cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR1,1050);
			
		}
		else if(TestMsg.data.byte[1] == 1)
		{
						sv_cs_setpotvalue(2450);
			cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR1,745);
		}
		else if(TestMsg.data.byte[1] == 2)
		{
						sv_cs_setpotvalue(1090);
			cl_thretval = sv_cntrl_setpumpspeed(DCMOTOR1,370);
		}
		break;
		case TEST_SET_VALVE_OPEN: // 42 0x30	
		{
								cl_Datastreamtype cl_tdata;
								cl_tdata.word = 0;
		switch ((sv_valvetype)TestMsg.data.byte[1] )
		{
			#if 0
			case VALVE_ID2:

						cl_tdata.Twobyte = 10;
						cl_tdata.bytearray[2] = 4;
						Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
						
			break;
			case VALVE_ID3:
									cl_tdata.Twobyte = 20;
									cl_tdata.bytearray[2] = 4;
									Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			break;
			case VALVE_ID4:
									cl_tdata.Twobyte = 30;
									cl_tdata.bytearray[2] = 4;
									Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			break;
			case VALVE_ID5:
									cl_tdata.Twobyte = 11;
									cl_tdata.bytearray[2] = 4;
									Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			break;
			case VALVE_ID6:
									cl_tdata.Twobyte = 21;
									cl_tdata.bytearray[2] = 4;
									Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			break;
			case VALVE_ID7:
									cl_tdata.Twobyte = 31;
									cl_tdata.bytearray[2] = 4;
									Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			break;
			default:break;
			
			#endif
		}



		{
					cl_wait(100);
					pdataarray = (uint8_t *)"V_OPEN";
					sv_cntrl_activate_valve((sv_valvetype)TestMsg.data.byte[1]);
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
					cl_wait(100);
					
		}
		}
		break;
		case TEST_SET_VALVE_CLOSE: // 43 0c31
		cl_wait(100);
		sv_cntrl_deactivate_valve((sv_valvetype)TestMsg.data.byte[1]);
		pdataarray = (uint8_t *)"V_CLOSE";
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_ALL_VALVES_OPEN:
		cl_wait(100);
		pdataarray = (uint8_t *)"V_ALLOPEN";
		 for(temp = 1;temp<=20; temp++)
		 {
				sv_cntrl_activate_valve((sv_valvetype)temp);
				cl_wait(10);
		 }
				//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
				
		cl_wait(100);
		break;
		case TEST_SET_HP_PUMPON:
		cl_wait(100);
		
		cl_thretval =  sv_cntrl_activatepump(HEPARINPUMP);
		
		pdataarray = (uint8_t *)"HP_ON";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_HP_PUMPOFF:
		cl_wait(100);
		
		cl_thretval =  sv_cntrl_deactivatepump(HEPARINPUMP);
		
		pdataarray = (uint8_t *)"HP_OFF";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_HP_PUMPRATE:
		cl_wait(100);
						if(TestMsg.datasize == 2)
						{
							//rateval = TestMsg.data[1];
							cl_indata.bytearray[0] = TestMsg.data.byte[1];
						}
						else if(TestMsg.datasize == 3)
						{
							//rateval = (uint16_t)TestMsg.data[1];
							cl_indata.bytearray[1] = TestMsg.data.byte[1];
							cl_indata.bytearray[0] = TestMsg.data.byte[2];
							
						}
						

						sv_cntrl_setpumpspeed(HEPARINPUMP,cl_indata.word);
						pdataarray = (uint8_t *)"HP_RATE";
			//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
			//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
		cl_wait(100);
		break;
		case TEST_SET_HP_PUMPDIR:
		if(TestMsg.data.byte[1])
		{
			sv_cntrl_setHepa_dir();
		}
		else
		{
			sv_cntrl_resetHepa_dir();
		}
		break;
		case TEST_SET_ALL_PUMPS_OFF:
		cl_wait(50);
		
		cl_thretval =  sv_cntrl_deactivatepump(DCMOTOR1);
		cl_wait(10);
		cl_thretval =  sv_cntrl_deactivatepump(DCMOTOR2);
		cl_wait(10);
		cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
		cl_wait(10);
		cl_thretval =  sv_cntrl_deactivatepump(UFPUMP);
		cl_wait(10);
		cl_thretval =  sv_cntrl_deactivatepump(HEPARINPUMP);
		
				
		cl_wait(50);
		break;
		case TEST_SET_BC_V1:
		sv_cntrl_activate_valve(6);
		sv_cntrl_activate_valve(7);
		sv_cntrl_activate_valve(12);
		sv_cntrl_activate_valve(9);
		
		sv_cntrl_deactivate_valve(5);
		sv_cntrl_deactivate_valve(8);
		sv_cntrl_deactivate_valve(10);
		sv_cntrl_deactivate_valve(11);
		pdataarray = (uint8_t *)"BC_V1 SET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
		break;
		case TEST_SET_BC_V2:
		sv_cntrl_activate_valve(5);
		sv_cntrl_activate_valve(8);
		sv_cntrl_activate_valve(10);
		sv_cntrl_activate_valve(11);
		
		sv_cntrl_deactivate_valve(12);
		sv_cntrl_deactivate_valve(9);
		sv_cntrl_deactivate_valve(6);
		sv_cntrl_deactivate_valve(7);
		pdataarray = (uint8_t *)"BC_V2 SET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
		break;
		case TEST_SET_BC_V1_1314:
		sv_cntrl_activate_valve(6);
		sv_cntrl_activate_valve(7);
		sv_cntrl_activate_valve(12);
		sv_cntrl_activate_valve(9);
		
		sv_cntrl_deactivate_valve(5);
		sv_cntrl_deactivate_valve(8);
		sv_cntrl_deactivate_valve(10);
		sv_cntrl_deactivate_valve(11);
		sv_cntrl_activate_valve(13);
		sv_cntrl_activate_valve(14);
		pdataarray = (uint8_t *)"BC_V1_1314 SET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,14);
		break;
		case TEST_SET_BC_V2_1314:
		sv_cntrl_activate_valve(5);
		sv_cntrl_activate_valve(8);
		sv_cntrl_activate_valve(10);
		sv_cntrl_activate_valve(11);
		
		sv_cntrl_deactivate_valve(12);
		sv_cntrl_deactivate_valve(9);
		sv_cntrl_deactivate_valve(6);
		sv_cntrl_deactivate_valve(7);
		sv_cntrl_activate_valve(13);
		sv_cntrl_activate_valve(14);
		pdataarray = (uint8_t *)"BC_V2_1314 SET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,14);
		break;
		case TEST_SET_BC_V1_4:
		sv_cntrl_activate_valve(6);
		sv_cntrl_activate_valve(7);
		sv_cntrl_activate_valve(12);
		sv_cntrl_activate_valve(9);
		
		sv_cntrl_deactivate_valve(5);
		sv_cntrl_deactivate_valve(8);
		sv_cntrl_deactivate_valve(10);
		sv_cntrl_deactivate_valve(11);
		sv_cntrl_activate_valve(4);
		sv_cntrl_deactivate_valve(13);
		sv_cntrl_deactivate_valve(14);
		
		pdataarray = (uint8_t *)"BC_V1_4 SET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);
		break;
		case TEST_SET_BC_V2_4:
		sv_cntrl_activate_valve(5);
		sv_cntrl_activate_valve(8);
		sv_cntrl_activate_valve(10);
		sv_cntrl_activate_valve(11);
		
		sv_cntrl_deactivate_valve(12);
		sv_cntrl_deactivate_valve(9);
		sv_cntrl_deactivate_valve(6);
		sv_cntrl_deactivate_valve(7);
		sv_cntrl_activate_valve(4);
		sv_cntrl_deactivate_valve(13);
		sv_cntrl_deactivate_valve(14);
		
		pdataarray = (uint8_t *)"BC_V2_4 SET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);
		break;
		case TEST_RESET_BC:
		sv_cntrl_deactivate_valve(5);
		sv_cntrl_deactivate_valve(8);
		sv_cntrl_deactivate_valve(10);
		sv_cntrl_deactivate_valve(11);
		
		sv_cntrl_deactivate_valve(12);
		sv_cntrl_deactivate_valve(9);
		sv_cntrl_deactivate_valve(6);
		sv_cntrl_deactivate_valve(7);
		
		cl_thretval =  sv_cntrl_deactivatepump(DCMOTOR1);

		cl_thretval=  sv_cntrl_deactivatepump(DCMOTOR2);
		
		pdataarray = (uint8_t *)"BC_RESET";
		
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
		break;
		case TEST_SET_BC_FREEZE:
		g_testbcfreeze = true;
				sv_cntrl_deactivate_valve(5);
				sv_cntrl_deactivate_valve(8);
				sv_cntrl_deactivate_valve(10);
				sv_cntrl_deactivate_valve(11);
				
				sv_cntrl_deactivate_valve(12);
				sv_cntrl_deactivate_valve(9);
				sv_cntrl_deactivate_valve(6);
				sv_cntrl_deactivate_valve(7);

				cl_thretval =  sv_cntrl_deactivatepump(DCMOTOR1);

				cl_thretval=  sv_cntrl_deactivatepump(DCMOTOR2);
				pdataarray = (uint8_t *)"BC_FREEZE";
	//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
		break;
		case TEST_SET_BC_UNFREEZE:
		
		g_testbcfreeze = false;
				pdataarray = (uint8_t *)"BC_UNFREEZE";

				cl_thretval =  sv_cntrl_activatepump(DCMOTOR1);
				
				cl_thretval=  sv_cntrl_activatepump(DCMOTOR2);
										
	//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);
		g_testbcfreeze = false;
		break;
		
					case TEST_SET_OPENFILL:
					
					
					pdataarray = (uint8_t *)"OPENFILL";

				sv_cntrl_activate_valve(5);
				sv_cntrl_activate_valve(8);
				sv_cntrl_activate_valve(10);
				sv_cntrl_activate_valve(11);
				
				sv_cntrl_activate_valve(12);
				sv_cntrl_activate_valve(9);
				sv_cntrl_activate_valve(6);
				sv_cntrl_activate_valve(7);
				cl_thretval =  sv_cntrl_activatepump(DCMOTOR1);
				cl_thretval=  sv_cntrl_activatepump(DCMOTOR2);
					
		//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
					g_testbcfreeze = false;
					break;
				
			case TEST_SKIPOPENFILL:
			
			
			pdataarray = (uint8_t *)"JTR";

			Cl_RinseMinutescounter = CL_RINSE_OPENFILL_TIMEOUT;
			
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,3);
			g_testbcfreeze = false;
			break;
			case TEST_HEATER_FAST:
			sv_cntrl_incheater(0x900);
			
			break;
			
			case TEST_HEATER_SLOW:
			sv_cntrl_incheater(0x600);
			break;
			case TEST_SET_HEATER_RATE:
			
				cl_wait(100);
				if(TestMsg.datasize == 2)
				{
					//rateval = TestMsg.data[1];
					cl_indata.bytearray[0] = TestMsg.data.byte[1];
				}
				else if(TestMsg.datasize == 3)
				{
					//rateval = (uint16_t)TestMsg.data[1];
					cl_indata.bytearray[1] = TestMsg.data.byte[1];
					cl_indata.bytearray[0] = TestMsg.data.byte[2];
					
				}
				heater_duty = cl_indata.word;
			
				//sv_cntrl_incheater(4096 * heater_duty/100);
				sv_cntrl_incheater( heater_duty);
			break;
			case TEST_VCLAMP_ON:
		//	sv_cntrl_activatevenousclamp();
			
			pdataarray = (uint8_t *)"VC_ON";
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
	
						sv_cntrl_activate_valve( VALVE_ID4);
						//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
						//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TestMsg.data.byte[1],TestMsg.datasize-1);
						cl_wait(50);
						sv_cntrl_deactivate_valve(VALVE_ID4);
						cl_wait(50);
			break;
			case TEST_VCLAMP_OFF:
			sv_cntrl_deactivatevenousclamp();
			pdataarray = (uint8_t *)"VC_OFF";
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,5);
			break;
						
			case TEST_RAPIDFIRE:
			for ( count1 = 0 ; count1 < 16 ; count1++)
			{
							cl_indata.word = count1;
		//					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_indata.word,4);
				
			}
			break;
			case TEST_SCAN_PRESSURE:
				start_pressurecapture = true;
			break;
			case TEST_BC_TRIGGER:
				 if(TestMsg.data.byte[1] == 1)
				{
					Current_sense_trigger = true;
					pdataarray = (uint8_t*) "CS_TRIGGER";
					
					Cl_AlarmActivateAlarms( PS3_HIGH_THRESHOLD,false );
					Cl_AlarmActivateAlarms( FPCURRENTSTATUS,true );
	//				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);
				}
				else
				{
					Current_sense_trigger = false;
					pdataarray = (uint8_t*) "PS_TRIGGER";
					 Cl_AlarmActivateAlarms( FPCURRENTSTATUS,false );
					 Cl_AlarmActivateAlarms( PS3_HIGH_THRESHOLD,true );
		//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);

				} 
					
				break;
				
			case TEST_SYS_VERSION:
					
			pdataarray = (uint8_t *)"Ver:T.4";
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
			//cl_testgetversion(&version);
			//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_VERSION,&version,1);
			//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_SYS_VERSION,"K",1);
			break;

						case TEST_LEVEL_SWITCH_ON:
						
																			#ifdef PLATFORMSTUB //defined in cl_stubs.h

										cl_sys_statbuffer.levelswitch1 = false;
		
	
							#endif
						

						break;

						case TEST_LEVEL_SWITCH_OFF:
						
					//	Cl_Alarm_TriggerDummyAlarm(LEVELSWITCH_ON_TO_OFF , 1);		
					//	Cl_Alarm_TriggerDummyAlarm(LEVELSWITCH_OFF_TO_ON , 0);	
											#ifdef PLATFORMSTUB //defined in cl_stubs.h

								//		cl_sys_statbuffer.levelswitch1 = true;
		
	
							#endif
							sv_cntrl_activate_valve(VALVE_ID4);
							cl_wait(50);
							sv_cntrl_deactivate_valve(VALVE_ID4);
						break;
			case TEST_SHORT_RINSE:
			#if 0
			// gtest_short_rinse  = (bool) TestMsg.data.byte[1];
			{
							 uint8_t  test_hstate;
							test_hstate = CL_HEATER_SUBSTATE_DISTRIBUTION;
							Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_HEATER_STATE,(void*)&test_hstate,1);
							Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_HEATER_STATE,(uint8_t*)&test_hstate,1);
							
							cl_Datastreamtype cl_tdata;
							cl_tdata.word =0;
							cl_tdata.Twobyte = test_hstate;
							cl_tdata.bytearray[2] = 23;
							Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&test_hstate,1);
						}
							
			#endif
			break;
			 case TEST_SET_FLOW_SENSE_ENABLE:
			 
			 	 if(TestMsg.data.byte[1] == 1)
				{
						gflow_en = true;
					pdataarray = (uint8_t*) "FLOW_EN";
					
	
		//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
				}
				else
				{
				
					gflow_en = false;
					pdataarray = (uint8_t*) "FLOW_DIS";

		//			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,11);

				} 
				break;
		case TEST_START_PROPO:
		sv_prop_startmixing();
		break;
		
		case TEST_SET_PROPO_RC:
		 sv_prop_set_propro_rc( TestMsg.data.byte[1]);
		break;
		case TEST_START_PROPO_ALIGN:
		
		 mix_count = 0;
		//	for(mix_count = 0; mix_count <1000 ; mix_count++)
			{
				
			
		//	cl_wait(1);
			

			
		//	pdataarray = (uint8_t *)"BP_ON";
		//	int16_t temp_10;
		//	int16_t potvalue1;
		//	for ( temp_10 = 0 ; temp_10 < 10; temp_10++)
			{
				uint16_t read_data = 0;
				
				//  sv_prop_startmixing();
				
			//	sv_cs_setpotvalue(600);
			//for ( temp_10 = 0 ; temp_10 < 1000; temp_10++)
				sv_prop_startpropeo_aligning();
			//		cl_thretval =  sv_cntrl_activatepump(BLOODPUMP);
				// cl_wait(2);
				 
			//	  DD_IIC_READ_PROP(0x61,&read_data);
				//  cl_wait(2);
				//   if(	read_data == (mix_count +1) 	)
				   {
				//	   sv_prop_stopmixing();
				   }
			//	  	cl_wait(1);
				  
				//	potvalue1 = (temp_10 * 1024)/10000;
				//	cl_thretval =  	sv_cs_setpotvalue(potvalue1);
				//	cl_wait(1);
				//	cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
				//potvalue1 = 200;
				//	cl_thretval =  	sv_cs_setpotvalue(potvalue1);
				//	if(!(temp_10 % 50 ))
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp_10,2);
				//	cl_wait(1);
			}
			}
		
	//	cl_wait(2);
		//	DD_IIC_SEND_PROP( 0x61, 3);
		
			
		break;
		case TEST_STOP_PROPO:
			cl_wait(100);
			

			
	//		pdataarray = (uint8_t *)"BP_ON";
		//	int16_t temp_10;
		//	int16_t potvalue1;
			//for ( temp_10 = 0 ; temp_10 < 10000; temp_10++)
			{
				
				  sv_prop_stopmixing();
			//	  	cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
					  
					  
				//	potvalue1 = (temp_10 * 1024)/10000;
				//	cl_thretval =  	sv_cs_setpotvalue(potvalue1);

				//	cl_thretval =  sv_cntrl_deactivatepump(BLOODPUMP);
				//potvalue1 = 200;
				//	cl_thretval =  	sv_cs_setpotvalue(potvalue1);
				//	if(!(temp_10 % 50 ))
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp_10,2);
				//	cl_wait(1);
			}
		break;
		case TEST_SET_BYON:
		sv_cntrl_enable_bypass();
		break;
		case TEST_SET_BYOFF:
		sv_cntrl_disable_bypass();
		break;
		case TEST_SET_LON:
		sv_cntrl_enable_loopback();
		 sv_prop_stopmixing();
		break;
		case TEST_SET_LOFF:
		sv_cntrl_disable_loopback();
		 sv_prop_startmixing();
		break;
		case TEST_SET_ALARM_LIMIT:
		
						if(TestMsg.datasize == 2)
						{
							//rateval = TestMsg.data[1];
							cl_indata.bytearray[0] = TestMsg.data.byte[1];
						}
						else if(TestMsg.datasize == 3)
						{
							//rateval = (uint16_t)TestMsg.data[1];
							cl_indata.bytearray[1] = TestMsg.data.byte[1];
							cl_indata.bytearray[0] = TestMsg.data.byte[2];
							
						}
							else if(TestMsg.datasize == 4)
						{
							//rateval = (uint16_t)TestMsg.data[1];
							cl_indata.bytearray[2] = TestMsg.data.byte[1];
							cl_indata.bytearray[1] = TestMsg.data.byte[2];
							cl_indata.bytearray[0] = TestMsg.data.byte[3];
							
						}
							else if(TestMsg.datasize == 5)
						{
							//rateval = (uint16_t)TestMsg.data[1];
							cl_indata.bytearray[3] = TestMsg.data.byte[1];
							cl_indata.bytearray[2] = TestMsg.data.byte[2];
							cl_indata.bytearray[1] = TestMsg.data.byte[3];
							cl_indata.bytearray[0] = TestMsg.data.byte[4];
							
						}
						switch(cl_indata.bytearray[3])
						{
							case TEST_SENSOR_TEMP1:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.temp1_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP1_HIGH",10);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if(cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								Cl_alarmThresholdTable.temp1_low_threshold =cl_indata.Twobyte;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP1_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_TEMP1STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_TEMP1STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP1_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_TEMP1STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP1",5);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							case TEST_SENSOR_TEMP2:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.temp2_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP2_HIGH",10);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if (cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								Cl_alarmThresholdTable.temp2_low_threshold =cl_indata.Twobyte;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP2_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_TEMP2STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_TEMP2STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_TEMP2STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP2",5);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							
							
							case TEST_SENSOR_TEMP3:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.temp3_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP_HIGH",8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);								
							}
							else if (cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								Cl_alarmThresholdTable.temp3_low_threshold =cl_indata.Twobyte;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP_LOW",7);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_TEMP3STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_TEMP3STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_TEMP3STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEMP3",5);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							
							case TEST_SENSOR_APT:
							if(cl_indata.bytearray[2]== 1)
								{
								Cl_alarmThresholdTable.apt_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"APT_HIGH",8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								}
								else if (cl_indata.bytearray[2]== 0)
								{
									int16_t temp=0,temp1=0;
									temp = cl_indata.Twobyte-1000;
									//temp = cl_indata.Twobyte;
									Cl_alarmThresholdTable.apt_low_threshold =temp;
									temp1=temp+1000;
									cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"APT_LOW= (-)",12);
									//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp1,2);
									//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp,2);
								}
								else if(cl_indata.bytearray[2]== 2)
								{
									int16_t temp=0,temp1=0;
									//temp = cl_indata.Twobyte-1000;
									test_sensor_status[SENSOR_APTSTATUS].test_sensord_data =cl_indata.Twobyte;
									test_sensor_status[SENSOR_APTSTATUS].test_flag = true;
									//temp1=temp+1000;
									//uint16_t temp;
									temp = cl_indata.Twobyte;
									cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
									cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
									//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
								}
								else if(cl_indata.bytearray[2]== 3)
								{
									int16_t temp=0,temp1=0;
									//temp = cl_indata.Twobyte-1000;
									//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
									test_sensor_status[SENSOR_APTSTATUS].test_flag = false;
									//temp1=temp+1000;
									//uint16_t temp;
									temp = cl_indata.Twobyte;
									cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"APT_LOW",9);
									cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
									//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
								}
								break;
							break;
							case TEST_SENSOR_VPT:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.vpt_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"VPT_HIGH",8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if (cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								temp = cl_indata.Twobyte-1000;
								//temp = cl_indata.Twobyte;
								Cl_alarmThresholdTable.vpt_low_threshold =temp;
								temp1=temp+1000;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"VPT_LOW= (-)",12);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp1,2);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp,2);
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_VPTSTATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_VPTSTATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_VPTSTATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							break;
							case TEST_SENSOR_COND:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.cond_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_HIGH",8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if(cl_indata.bytearray[2]== 0)
							{
								
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								Cl_alarmThresholdTable.cond_low_threshold = cl_indata.Twobyte;
								temp1=cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"COND_LOW",8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_COND_STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_COND_STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_COND_STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"CON_LOW",7);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							
							case TEST_SENSOR_PS1:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.ps1_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1_HIGH",8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if (cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								temp = cl_indata.Twobyte-1000;
								//temp = cl_indata.Twobyte;
								Cl_alarmThresholdTable.ps1_low_threshold =temp;
								temp1=temp+1000;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS1_LOW= (-)",12);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp1,2);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp,2);
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_PS1STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_PS1STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_PS1STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							case TEST_SENSOR_PS2:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.ps2_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2",3);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if (cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								temp = cl_indata.Twobyte-1000;
								Cl_alarmThresholdTable.ps2_low_threshold = temp;
								temp1=temp+1000;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2_LOW= (-)",12);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp1,2);
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_PS2STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_PS2STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_PS2STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS2_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							case TEST_SENSOR_PS3:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.ps3_high_threshold = cl_indata.Twobyte;
								uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS3",3);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
							}
							else if(cl_indata.bytearray[2]== 0)
							{
								int16_t temp=0,temp1=0;
								temp = cl_indata.Twobyte-1000;
								//temp = cl_indata.Twobyte;
								Cl_alarmThresholdTable.ps3_low_threshold = temp;
								temp1=temp+1000;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS3_LOW= (-)",12);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTNDATA,&temp1,2);
								//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.ps3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 2)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								test_sensor_status[SENSOR_PS3STATUS].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_PS3STATUS].test_flag = true;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"TEST",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							else if(cl_indata.bytearray[2]== 3)
							{
								int16_t temp=0,temp1=0;
								//temp = cl_indata.Twobyte-1000;
								//test_sensor_status[TEST_SENSOR_TEMP1].test_sensord_data =cl_indata.Twobyte;
								test_sensor_status[SENSOR_PS3STATUS].test_flag = false;
								//temp1=temp+1000;
								//uint16_t temp;
								temp = cl_indata.Twobyte;
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"PS3_LOW",9);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp,2);
								//Cl_alarmThresholdTable.temp3_low_threshold = cl_indata.Twobyte;
							}
							break;
							
							case 255:
							if(cl_indata.bytearray[2]== 1)
							{
								Cl_alarmThresholdTable.cond_high_threshold = cl_indata.Twobyte;
							}else
							{
								Cl_alarmThresholdTable.cond_low_threshold = cl_indata.Twobyte;
							}
							break;
							default:break;
						}
		break;
		
		case TEST_GET_SENSORDATA:
//		sensordata =0;
		switch(TestMsg.data.byte[1] )
		{
			
			case TEST_SENSOR_HP_1:
			pdataarray = (uint8_t *)"HP_START=";
			//int16_t heparin_start;
			cl_testgetsensordata(SENSOR_HP_START,&sensordata);
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,9);
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&sensordata,2);
			
			break;
			case  TEST_SENSOR_HP_2:
			cl_testgetsensordata(SENSOR_HP_END,&sensordata);
						pdataarray = (uint8_t *)"HP_END=";
			//int16_t heparin_start;
			
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,7);
		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&sensordata,2);
			break;
			case TEST_SENSOR_PS1:
			
			cl_testgetsensordata(SENSOR_PS1STATUS,&sensordata);
			pdataarray = (uint8_t *)"PS1=";
		//	sensordatamillivolts = ((sensordata* 3300 /4096) ) - 870;
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			
				Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
				if (  sensordatamillivolts < 1360 )
				{
					Pressuredatainmillibars = Pressuredatainmillibars- 50 ;
				}
				else if(  sensordatamillivolts < 1650 )
				{
					Pressuredatainmillibars = Pressuredatainmillibars- 25 ;
				}
			//if(Pressuredatainmillibars> 1000)
			{
							
			//				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
			//				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
			}
			#if 0
			else
			{
				Pressuredatainmillibars = 1000-Pressuredatainmillibars;
				pdataarray = (uint8_t *)"PS1= (-)";
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
				
			}
			#endif

			
			break;
			case TEST_SENSOR_PS2:
			cl_testgetsensordata(SENSOR_PS2STATUS,&sensordata);
					pdataarray = (uint8_t *)"PS2=";
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
			if (  sensordatamillivolts < 1360 )
			{
				Pressuredatainmillibars = Pressuredatainmillibars- 50 ;
			}
			else if(  sensordatamillivolts < 1650 )
			{
				Pressuredatainmillibars = Pressuredatainmillibars- 25 ;
			}
		//	if(Pressuredatainmillibars> 1000)
			{
						
					//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
					//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
			}
			
			#if 0
			else
			{
				Pressuredatainmillibars = 1000-Pressuredatainmillibars;
				pdataarray = (uint8_t *)"PS2= (-)";
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
				
			}
			#endif

			break;
			case TEST_SENSOR_PS3:
			cl_testgetsensordata(PS3_HIGH_THRESHOLD,&sensordata);
					pdataarray = (uint8_t *)"PS3=";
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
		//	Pressuredatainmillibars =  (sensordatamillivolts/1.485 ) ;
			Pressuredatainmillibars = 1000 * (sensordatamillivolts - 1100)/1100;
		
			// if(Pressuredatainmillibars> 0)
			{
						//	Pressuredatainmillibars = Pressuredatainmillibars - 1000;
							cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
							cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
			}
			
			#if 0
			else
			{
				Pressuredatainmillibars = 1000-Pressuredatainmillibars;
				pdataarray = (uint8_t *)"PS3= (-)";
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
				
			}
			#endif
			break;
			case TEST_SENSOR_APT:
			cl_testgetsensordata(APTSTATUS_HIGH,&sensordata);
					
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			
			Pressuredatainmillibars = (sensordatamillivolts - 1640)/0.6;
			//if(sensordatamillivolts > 1600 )
			{
				pdataarray = (uint8_t *)"APT=";
				//Pressuredatainmillibars = (sensordatamillivolts - 1600)/0.578;
			//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
			//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
			}
			//Pressuredatainmillibars =  (sensordatamillivolts/0.6 ) ;
			#if 0
			else
			{					Pressuredatainmillibars = (1600 - sensordatamillivolts )/0.6;
								pdataarray = (uint8_t *)"APT= (-)";
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
				
			}
			#endif

			break;
			case TEST_SENSOR_VPT:
			cl_testgetsensordata(VPTSTATUS_HIGH,&sensordata);
					pdataarray = (uint8_t *)"VPT=";
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			Pressuredatainmillibars = (sensordatamillivolts - 1640)/0.6;
			//if(sensordatamillivolts > 1600 )
			{
				pdataarray = (uint8_t *)"VPT=";
				//Pressuredatainmillibars = (sensordatamillivolts - 1600)/0.578;
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
			}
			//Pressuredatainmillibars =  (sensordatamillivolts/0.6 ) ;
			
			#if 0
			else
			{					Pressuredatainmillibars = (1600 - sensordatamillivolts )/0.6;
				pdataarray = (uint8_t *)"VPT= (-)";
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pressuredatainmillibars,2);
				
			}
			#endif
			break;
			case TEST_SENSOR_TMP:
			
			#if 0
			cl_testgetsensordata(PS1STATUS_HIGH,&Ps1);
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts/0.8 ) ;
			Ps1 = Pressuredatainmillibars;
			cl_testgetsensordata(PS2STATUS_HIGH,&Ps2);
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts/0.8 ) ;
			Ps2 = Pressuredatainmillibars;
			cl_testgetsensordata(APTSTATUS_HIGH,&Atp);
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts/0.6 ) ;
			Atp = Pressuredatainmillibars;
			cl_testgetsensordata(VPTSTATUS_HIGH,&Vtp);
			sensordatamillivolts = ((sensordata* 3300 /4096) ) ;
			Pressuredatainmillibars =  (sensordatamillivolts/0.6 ) ;
			Vtp = Pressuredatainmillibars;
			
			pdataarray = (uint8_t *)"TMP1=";
			TMP = Vtp - Ps1;
		#endif
		
					TMP = vptavg - ps1avg;
				//	if(TMP > 0)
					{
						Pressuredatainmillibars = Pressuredatainmillibars - 1000;
						pdataarray = (uint8_t *)"TMP1=";
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TMP,2);
					}
					
					#if 0
					else
					{
						Pressuredatainmillibars = 1000-Pressuredatainmillibars;
						pdataarray = (uint8_t *)"TMP1= (-)";
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TMP,2);
						
					}
					
					#endif
					TMP = (vptavg + aptavg)/2 - (ps1avg+ps2avg)/2;
					// if(TMP > 0)
					{
						pdataarray = (uint8_t *)"TMP2=";
						Pressuredatainmillibars = Pressuredatainmillibars - 1000;
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,4);
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TMP,2);
					}
					#if 0
					else
					{
						Pressuredatainmillibars = 1000-Pressuredatainmillibars;
						pdataarray = (uint8_t *)"TMP2= (-)";
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,8);
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&TMP,2);
						
					}
					
					#endif


			break;
			case TEST_SENSOR_TEMP1:

			cl_testgetsensordata(SENSOR_TEMP1STATUS,&sensordata);
			{
							int16_t temp,temp1;
							temp = (0.8056 * sensordata) - 1450 ;
							temp1 = 3700 + (temp * 1000)/340;
							
							pdataarray = (uint8_t *)"TEMP1=";
							cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
							cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
			}

			break;
			case TEST_SENSOR_TEMP2:
			{
				int16_t temp,temp1;
			cl_testgetsensordata(SENSOR_TEMP2STATUS,&sensordata);
					
					temp = (0.8056 * sensordata) - 1450 ;
					temp1 = 3700 + (temp * 1000)/340;
									
			pdataarray = (uint8_t *)"TEMP2=";
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
			cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
			}
			break;
			case TEST_SENSOR_TEMP3:
			{
				int16_t temp,temp1;
				cl_testgetsensordata(SENSOR_TEMP3STATUS,&sensordata);
				
				
				temp = (0.8056 * sensordata) - 1450 ;
				temp1 = 3700 + (temp * 1000)/340;
				
				pdataarray = (uint8_t *)"TEMP3=";
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
				
			}

			break;


			
			default: break;
		}

		break;
		default:
		
		break;
	}
	
	
	
	return CL_OK;
	
}


Cl_ReturnCodes  cl_testgetsensordata(uint8_t dataId, uint16_t* dataItem)
{
	Cl_ReturnCodes cl_thretval = CL_ERROR;
	uint8_t bytedata =0 ;
	uint16_t twobytedatal=0;
	uint32_t worddata=0;
	
	uint8_t pbytedata;
	uint16_t p2bytedatal;
	uint32_t pworddata;
	
	pbytedata = &bytedata;
	p2bytedatal = &twobytedatal;
	pworddata =  &worddata;
	

		
	Cl_SysStat_GetSensor_Status_Query(dataId, dataItem);
	
	
}

Cl_ReturnCodes  cl_testgetversion( uint8_t* version)
{
	Cl_ReturnCodes cl_thretval = CL_ERROR;
	
	*version = sys_sw_version;

	
	
}

void Cl_tg_prick_100ms(void)
{
	int16_t Ps1,Ps2,Ps3,Atp,Vtp,TMP = 0;
		int16_t sensordata=0;
		static int16_t prev_flow_status=0;
		int16_t Pressuredata=0;
		int16_t sensordatamillivolts =0;
		int16_t tempdata =0,count1;
		int16_t Pressuredatainmillibars =0;
		uint8_t* pdataarray;
		cl_Datastreamtype cl_tdata;
		static uint8_t  flow_counter =0 ;
if(syncdone)
{
	


	static uint16_t counter=0,threeseccounter=0,CS_ontimecnter=0,cnt = 0,levelsw_cnter = 0;
	 
	Cl_ReturnCodes cl_thretval = CL_ERROR;
	//Cl_SysStat_GetSensor_Status_Query(PS3STATUS_HIGH, &sensordata);


	sensordata = sensordata * 3300/4096;
	//cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&sensordata,2);
	//DD_IIC_SEND_PROP( CPU_PROP_IIC_ADDR, 0); 
	
	//levelsw_cnter++;
	//if(levelsw_cnter > 10000)
	//levelsw_cnter = 0;
	
	
	//if (!levelsw_cnter % 20 )
	{
		
		
	}
	//else if (!levelsw_cnter % 10 )
	{
		
	}
	//if(syncdone) //testing
	{
		//	counter++;//testing
		//	threeseccounter++;
		//	CS_ontimecnter++;
			
	}
	

#ifdef PLATFORMSTUB //defined in cl_stubs.h

if(dummy_currentsence)
{
	if(threeseccounter> 30)
	{
		threeseccounter = 0;
	}
	if((threeseccounter == 30) && (syncdone) )//testing
	{
		cl_sys_statbuffer.FPcurrentstatus = 0;
		
		cl_thretval = CL_OK;
		threeseccounter = 0;
		CS_ontimecnter = 0;
	}

	if(CS_ontimecnter == 4)
	{
		cl_sys_statbuffer.FPcurrentstatus = 1;
	}
}
#endif
#if 0
	if((counter == 10) && (syncdone) )//testing
	{
		cnt++;
		 counter = 0;
					if (cnt < 50)
					{
						
					
					for ( count1 = 0 ; count1 < 30 ; count1++)//testing
					{
						
						cl_tdata.word = count1;
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&cl_tdata.word,4);
						
					}
					}
		
	}
	
#endif

if(syncdone)
{
				cl_testgetsensordata(SENSOR_TEMP1STATUS,&sensordata);
				{
					
				int16_t temp,temp1;
				temp = sensordata * 0.805;
				//	temp = (0.8056 * sensordata) - 1450 ;
				//	temp1 = 3700 + (temp * 1000)/340;

				//	pdataarray = (uint8_t *)"TEMP1=";
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
				cl_tdata.word = 0;
			//	cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = temp;
				cl_tdata.bytearray[2] = 1;
				
			//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				}
				

		
		
				cl_testgetsensordata(SENSOR_TEMP2STATUS,&sensordata);
				{
					
				int16_t temp,temp1;
				temp = sensordata * 0.805;
				//	temp = (0.8056 * sensordata) - 1450 ;
				//	temp1 = 3700 + (temp * 1000)/340;
					
				//	pdataarray = (uint8_t *)"TEMP1=";
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
				cl_tdata.word = 0;
				//cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = temp;
				cl_tdata.bytearray[2] = 2;
				
			//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				}
				cl_testgetsensordata(SENSOR_TEMP3STATUS,&sensordata);
				{
					
				int16_t temp,temp1;
				temp = sensordata * 0.805;
				
				avgtmp3 =	(avgtmp3*5 + temp)/6;
				
				//	temp = (0.8056 * sensordata) - 1450 ;
				//	temp1 = 3700 + (temp * 1000)/340;
					
				//	pdataarray = (uint8_t *)"TEMP1=";
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
				cl_tdata.word = 0;
				//cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = temp;
				cl_tdata.bytearray[2] = 3;
				
			//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
					
				cl_tdata.word = 0;
				cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = avgtmp3;					
				cl_tdata.bytearray[2] = 6;
			//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				}
				cl_testgetsensordata(COND_STATUS_HIGH,&sensordata);
				{
					//	int16_t temp,temp1;
					//	temp = (0.8056 * sensordata) - 1450 ;
					//	temp1 = 3700 + (temp * 1000)/340;
					
					//	pdataarray = (uint8_t *)"TEMP1=";
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
				//	cl_tdata.word = 0;
				//	cl_tdata.Twobyte = sensordata;
				//	cl_tdata.bytearray[2] = 2;
					
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				}
				cl_testgetsensordata(SENSOR_FLOW_SWITCH,&sensordata);
				{
					//	int16_t temp,temp1;
					//	temp = (0.8056 * sensordata) - 1450 ;
					//	temp1 = 3700 + (temp * 1000)/340;
					
					//	pdataarray = (uint8_t *)"TEMP1=";
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,pdataarray,6);
					//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp1,2);
					cl_tdata.word = 0;
					cl_tdata.Twobyte = sensordata;
					cl_tdata.bytearray[2] = 3;
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
				cl_testgetsensordata(HEATER_HIGH_TEMP,&sensordata);
				{
					//temp = sensordata * 0.805;
				//	if(temp > )
					
				}
				
			//	cl_testgetsensordata(APTSTATUS_HIGH, &Atp);//0x0F
			cl_testgetsensordata(VPTSTATUS_HIGH, &Atp);
				sensordatamillivolts = ((Atp* 3300 /4096) ) ;
				
			//	Pressuredatainmillibars =  (sensordatamillivolts - 1580) * 1.71;
			Pressuredatainmillibars =  (sensordatamillivolts - 1580) * 0.602;
				


				 
				
				aptavg = ((aptavg* 9) + Pressuredatainmillibars) /10;
			//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&aptavg,2);

	
				cl_testgetsensordata(SENSOR_PS3STATUS,&Ps3);
				sensordatamillivolts = ((Ps3* 3300 /4096) ) ;
				
				//Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
				Pressuredatainmillibars =  (sensordatamillivolts * 0.892) - 1004  ;
				
				ps3avg = ((ps3avg* 9) + Pressuredatainmillibars) /10;
				

					cl_tdata.word = 0;
					cl_tdata.Twobyte = 1000 + ps3avg;
					cl_tdata.bytearray[2] = 20;
					
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				/////////////////
				
				cl_testgetsensordata(SENSOR_PS2STATUS,&Ps2);
			sensordatamillivolts = ((Ps2* 3300 /4096) ) ;
				
				//Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
				Pressuredatainmillibars =  (sensordatamillivolts * 0.892) - 1004  ;
				
				ps2avg = ((ps2avg* 4) + Pressuredatainmillibars) /5;
				

					cl_tdata.word = 0;

					
					cl_tdata.Twobyte = 1000 + ps2avg;
					cl_tdata.bytearray[2] = 19;
				
				
				
			//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				
				
				
				cl_testgetsensordata(SENSOR_PS1STATUS,&Ps1);
				sensordatamillivolts = ((Ps1* 3300 /4096) ) ;
				
				//Pressuredatainmillibars =  ((sensordatamillivolts - 860)/0.78 ) ;
				Pressuredatainmillibars =  (sensordatamillivolts * 0.892) - 1004  ;
				
					ps1avg = ((ps1avg* 4) + Pressuredatainmillibars) /5;
				

					cl_tdata.word = 0;

					
					cl_tdata.Twobyte = 1000 + ps1avg;
					cl_tdata.bytearray[2] = 18;
				
				
				
	//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);			
				
				
				
				cl_testgetsensordata(APTSTATUS_HIGH,&Atp);
				sensordatamillivolts = ((Atp* 3300 /4096) ) ;
				Pressuredatainmillibars = (sensordatamillivolts - 1640)/0.6;
				//Pressuredatainmillibars =  ((sensordatamillivolts -1000)/0.578 ) ;
				Atp = Pressuredatainmillibars;
				aptavg = (( aptavg *9) + Pressuredatainmillibars) /10;
				cl_testgetsensordata(VPTSTATUS_HIGH,&Vtp);
				sensordatamillivolts = ((Vtp* 3300 /4096) ) ;
				Pressuredatainmillibars = (sensordatamillivolts - 1640)/0.6;
				//Pressuredatainmillibars =  ((sensordatamillivolts -1000)/0.578 ) ;
				Vtp = Pressuredatainmillibars;
				vptavg = (( vptavg *9) + Pressuredatainmillibars) /10;
				
		}		
	
}
}



void Cl_tg_prick_50ms(void)
{
	Cl_ReturnCodes cl_thretval = CL_ERROR;
	int16_t Ps = 0;
	static uint16_t pcnt =0;
	static uint8_t cnt = 0;
	int16_t Pressuredatainmillibars =0,ps1Pressuredatainmillibars=0, sensordatamillivolts =0;
	static int16_t Pdata[100];
	static bool capture_started = false;
	static bool Pre_BC_window = false;
	static bool BC_FLAG = false;
    static bool start_flag = false;
	
	if(start_pressurecapture)
	{
		

		
		if(!capture_started)
		{
			// wait for the BC_window to toggle
			if(Pre_BC_window != BC_window)
			{
				//start capture
				for(cnt = 0; cnt < 100 ; cnt++)
				{
					Pdata[cnt] = 0;
					pcnt = 0;
				}
				capture_started = true;
			}

		}
		else
		{
			
			if (BC_window)
				BC_FLAG = true;
			if(pcnt!=0 && BC_FLAG == false)
			{
				//stop capture
				capture_started = false;
				pressuredataready = true;
				start_pressurecapture = false;
				
				cnt = 0;
				
				#if 0
				if(pcnt > 60 )
				
				{
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pdata,30);
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pdata[30],30);
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pdata[60],pcnt-30);
				}else
				if(pcnt > 30)
				{
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pdata,30);
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pdata[30],pcnt-30);
				}
				else
				{
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Pdata,pcnt);
				}
				#endif
				
				while(cnt < pcnt)
				{
					Ps = Pdata[cnt];
					//if(Ps > 0)
					cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Ps,2);
					
					#if 0
					else
					{
						Ps = -Ps;
						
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"(-)",3);
						cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Ps,2);
					}
					#endif
					cnt++;	
				}
				
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"(CNT)",5);
				cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&pcnt,2);
				
				
				
			}
		//	if ((BC_window) && (capture_started))
		
		if ((BC_FLAG) && (capture_started))
			{
				cl_testgetsensordata(SENSOR_PS3STATUS,&Ps);
				sensordatamillivolts = ((Ps* 3300 /4096) ) ;
				Pressuredatainmillibars =  (sensordatamillivolts/1.485) ;
		ps1Pressuredatainmillibars = Pressuredatainmillibars;//1000;
				if(pcnt < 100)
				{
				Pdata[pcnt] = ps1Pressuredatainmillibars;
				//Pdata[pcnt] = Ps;
				pcnt++;
					
				}
				
				if(pcnt >= 100)
					BC_FLAG = false;
				    start_flag = true;

			}
			
		}
		

			Pre_BC_window = BC_window;

	}
	#if 0
	if(pressuredataready)
	{
							Ps = Pdata[cnt];
							if(Ps > 0)
							cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Ps,2);
							else
							{
								Ps = -Ps;
								
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"(-)",3);
								cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&Ps,2);
							}
		
	}
	#endif

}

void pinclock(void)
{
	static uint16_t pin_status = 1,hp_start_status = 0,prev_hp_start_status = 0,hp_end_status = 0,prev_hp_end_status = 0;
	static uint16_t counter = 0;


#if 0
				cl_testgetsensordata(HP_START,&hp_start_status);
				
				if(	prev_hp_start_status != hp_start_status)
				{
						prev_hp_start_status = hp_start_status;
						if(hp_start_status == 1)
						{							
					//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_START",8);
					//	 Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&hp_start_status,1);
				
						}
				}
				



				cl_testgetsensordata(HP_END,&hp_end_status);
				if(	prev_hp_end_status != hp_end_status)
				{
					prev_hp_end_status = hp_end_status;
					if(hp_end_status == 1)
					{
					// Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_END",8);
					// Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&hp_end_status,1);
					}
				}
#endif
	
}

void pinclock1(void) // 5 ms clock
{
	static uint16_t pin_status = 1,uf_rotataion_marker = 1,prev_uf_rotataion_marker = 1;
	static uint32_t fivemscounter=0,long_counter=0,uf_period = 0,uf_freq_in_min = 0, flow_count = 0, prev_flow_status=0;
	static bool count_state = false;
	static toggle = false;
	uint8_t dataarray[3];
	cl_Datastreamtype cl_data;
		

	//cl_testgetsensordata(UF_ROTATION_MARKER,&uf_rotataion_marker);
cl_testgetsensordata(SENSOR_UFP_FB,&uf_rotataion_marker);
if(prev_uf_rotataion_marker != uf_rotataion_marker)
{
	
	//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"HP_PERIOD",9);
	fivemscounter++;
}




	prev_uf_rotataion_marker = uf_rotataion_marker;	
}

void Cl_tg_prick_1ms()
{
	static int16_t ms_count = 0;
	cl_Datastreamtype cl_tdata;
				ms_count++;
				if(current_sense)
				{
					
				
				cl_tdata.word = 0;
				//cl_tdata.Twobyte = sensordata;
				cl_tdata.Twobyte = ms_count;
				cl_tdata.bytearray[2] = 17;
				current_sense = false;
	//			Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
				
				ms_count = 0;
				
				}

}
void tg_timer_minute(void)
{
	static uint minute_count = 0;
	uint8_t dataarray[3];
	cl_Datastreamtype cl_data;
	uint32_t temp3;
	
	
	
	
//	if(!(minute_count % 5 ))
//	{
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,"UF_R_COUNT",10);
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&uf_rotation_counter,2);
		temp3 = uf_rotation_counter * 1000;
//		Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&temp3,4);
	//	dataarray[0] = CON_TX_PARAM_DATA_UF_HR_RPM;
	//	dataarray[1] = cl_data.bytearray[0];
	//	dataarray[2] = cl_data.bytearray[1];
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_SYSDATA,&dataarray,3);

//	}
	if(minute_count == 180)
	{
		uf_rotation_counter =0;
		minute_count = 0;
	}
	
	minute_count++;
}


void testsectclock(void)
{
	int16_t cond_status = 0,hold1_status=0,hold2_status=0; 
	int16_t hold1 = 0, hold2=0,blood_door = 0;
	float temp = 0,temp1;
	uint16_t int_temp;

				Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS, &int_temp);
				temp = int_temp * 0.805;
				calibration_tmp(temp,TS2);

	if(temprature_final_value_2 > 45)
	{
		sv_cntrl_poweroffheater();
		SetHeaterState(CL_HEATER_STATE_OFF);
	}
		
	
}

uint16_t lookup_condtable(uint8_t cond_base,uint16_t cond_mvolt)
{
	uint8_t counter = 0;
	for (counter = cond_base; counter < 20; counter++)
	{
		if (cond_lookup[counter] > cond_mvolt)
		{
			return(counter * (10 + 10* ((cond_mvolt - cond_lookup[counter]) / cond_lookup[counter])) );
		}
	}
}
	
	
