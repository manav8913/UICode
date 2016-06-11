/*
 * cl_testharness.h
 *
 * Created: 1/22/2014 3:24:52 PM
 *  Author: user
 */ 


#ifndef CL_TESTHARNESS_H_
#define CL_TESTHARNESS_H_

#define MAX_DATA_SIZE 20

typedef enum 
{
TEST_SET_ALARM_COND, //0
TEST_SET_ALARM_CS1, //1
TEST_SET_ALARM_CS2, //2
TEST_SET_ALARM_CS3, //3
TEST_SET_ALARM_PS1, //4
TEST_SET_ALARM_PS2, //5
TEST_SET_ALARM_PS3, //6
TEST_SET_ALARM_FL1, //7
TEST_SET_ALARM_FL2, //8
TEST_SET_ALARM_HOLD1, //9
TEST_SET_ALARM_HOLD2, //10 0x0A
TEST_SET_ALARM_TEMP1, //11 0x0B
TEST_SET_ALARM_TEMP2, //12 0x0C
TEST_SET_ALARM_TEMP3, //13 0x0D
TEST_SET_ALARM_ABDSTATUS,//14 x0E
TEST_SET_ALARM_BDSTATUS,//15 0x0F
TEST_SET_ALARM_APTSTATUS,//16 0x10
TEST_SET_ALARM_BPTSTATUS,//17 0x11
TEST_SET_ALARM_BLDSTATUS,//18 0x012
TEST_SETDATA, //19 0x13
TEST_GETDATA, //20 0x14
TEST_SET_FLOW_PUMPON,//21 0x15
TEST_SET_FLOW_PUMPOFF,//22 0x16
TEST_SET_FLOW_PUMPRATE ,//23 0x17
TEST_SET_DEGAS_PUMPON,//24 0x18
TEST_SET_DEGAS_PUMPOFF,//25 0x19
TEST_SET_DEGAS_PUMPRATE,//26 0x1A
TEST_SET_UF_PUMPON,//27 0x1B
TEST_SET_UF_PUMPOFF,//28 0x1C
TEST_SET_UF_PUMPRATE,//29 0x1D
TEST_SET_VCLAMP_ON,//30 0x1E
TEST_SET_BLOOD_PUMPON,//31 0x1F
TEST_SET_BLOOD_PUMPOFF,//32 0x20
TEST_SET_BLOOD_PUMPSRATE,//33 0x21
TEST_SET_VCLAMP_OFF,//34 0x22
TEST_SET_WATERINLET_ON,//35 0x23
TEST_SET_WATERINLET_OFF,//36 0x24
TEST_SET_DRAIN_ON,//37 0x25
TEST_SET_DRAIN_OFF,//38 0x26
TEST_SET_ALL_VALVES_OPEN,//39 0x27
TEST_SET_ALL_VALVES_CLOSE,//40 0x28
TEST_SET_VALVE_OPEN,//41 0x29
TEST_SET_VALVE_CLOSE,//42 0x2A
TEST_SET_HP_PUMPON,//43 0x2B
TEST_SET_HP_PUMPOFF,//44 0x2C
TEST_SET_HP_PUMPRATE,//45 0x2D
TEST_SET_ALL_PUMPS_OFF, //46 0x2E
TEST_SET_FLOW_PUMP_FREQ, //47 0x2F
TEST_SET_DG_PUMP_FREQ, //48 0x30
TEST_SET_BLOOP_PUMP_FREQ, //49 0x31
TEST_SET_UF_PUMP_FREQ, //50 0x32
TEST_SET_BC_V1, //51 0x33
TEST_SET_BC_V2, //52 0x34
TEST_RESET_BC, //53 0x35
TEST_SET_BC_V1_1314, //54 0x36
TEST_SET_BC_V2_1314, //55 0x37
TEST_SET_BC_V1_4,  //56 0x38
TEST_SET_BC_V2_4, //57 0x39
TEST_SET_BC_FREEZE, //58 0x3A
TEST_SET_BC_UNFREEZE, //59 0x3B
TEST_SKIPOPENFILL, //60 0x3C
TEST_SET_OPENFILL,//61 0x3D
TEST_GET_SENSORDATA, //62 0x3E
TEST_SET_HEATERON, //63 0x3F
TEST_SET_HEATEROFF, //64 0x40








TEST_COMMAND_MAX

}Cl_Testcommandtype;

typedef union{
	
	uint8_t bytearray[4] ;
	uint16_t Twobyte  ;
	uint32_t word ;
	
}cl_Datastreamtype;
	
typedef enum 
{
TEST_DATA_NODATA,
TEST_DATA_RINSESTATUS,
TEST_DATA_SETDATA_MAX,

}Cl_Testdatatype;

typedef enum
{
	TEST_SENSOR_NONE, //0
	TEST_SENSOR_COND, //1
	TEST_SENSOR_CS1, //2
	TEST_SENSOR_CS2, //3
	TEST_SENSOR_CS3, //4
	TEST_SENSOR_PS1, //5
	TEST_SENSOR_PS2, //6
	TEST_SENSOR_PS3, // 7
	TEST_SENSOR_FL1, //8
	TEST_SENSOR_FL2, //9
	TEST_SENSOR_HOLD1, //10 0A
	TEST_SENSOR_HOLD2, //11 0B
	TEST_SENSOR_TEMP1, //12 0C
	TEST_SENSOR_TEMP2, //13 0D
	TEST_SENSOR_TEMP3, //14 0E
	TEST_SENSOR_ABD, //15 0F
	TEST_SENSOR_BD, //16 10
	TEST_SENSOR_APT, // 17 11
	TEST_SENSOR_VPT, // 18 12
	TEST_SENSOR_BLD, //19 13
	TEST_SENSOR_TMP,  //20 14
	TEST_SYS_VERSION,  //21 15
	TEST_VCLAMP_ON, //22 16
	TEST_VCLAMP_OFF, //23 17
	TEST_SENSRO_MAX,

}Cl_TestSensortype;


#endif /* CL_TESTHARNESS_H_ */