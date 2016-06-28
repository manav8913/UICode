/*
 * sv_interface.h
 *
 * Created: 1/8/2014 12:03:24 PM
 *  Author: intel
 */ 

#include "asf.h"
#include "Platform/Service/SV_CONSOLE/SV_CONSOLE.h"

#ifndef SV_INTERFACE_H_
#define SV_INTERFACE_H_


typedef enum {
	FLOW_PATH_NULL ,
	V15_FLUSH  ,
	V2_FLUSH ,
	BC_FLUSH_1_V15 ,
	BC_FLUSH_2_V15 ,
	BC_FLUSH_1_V13_14 ,
	BC_FLUSH_2_V13_14 ,
	BC_FLUSH_OPEN = 7,
	BC_FLUSH_1_V15_LOOP ,
	BC_FLUSH_2_V15_LOOP ,
	BC_FLUSH_1_V13_14_LOOP ,
	BC_FLUSH_2_V13_14_LOOP ,
	FLOW_PATH_FP8 ,
	FLOW_PATH_FP9 ,
	FLOW_PATH_FP10 ,
	FLOW_PATH_FP11 ,
	FLOW_PATH_FP12 ,
	FLOW_PATH_IDLE_RINSE,
	FLOW_PATH_ISO_UF,
	FLOW_PATH_BC_ALARM,
	FLOW_PATH_MAX,
}sv_flowpathtype;

typedef enum {
	ALARM_OUT_NULL = 0,
	ALARM_OUT_ID1 =1,
	ALARM_OUT_ID2 =2,
	ALARM_OUT_ID3 =3
}sv_alarmtype;

typedef enum {
	IND_ID_NULL = 0,
	IND_ID1 =1,
	IND_ID2 =2,
	IND_ID3 =3,
	IND_ID4 =4,
	IND_ID5 =5
}sv_indtype;

typedef enum {
	VALVE_NULL = 0,
	VALVE_ID1 = 1,
	VALVE_ID2 = 2,
	VALVE_ID3 = 3,
	VALVE_ID4 = 4,
	VALVE_ID5 = 5,
	VALVE_ID6 = 6,
	VALVE_ID7 = 7,
	VALVE_ID8 = 8,
	VALVE_ID9 = 9,
	VALVE_ID10 = 10,
	VALVE_ID11= 11,
	VALVE_ID12 = 12,
	VALVE_ID13 = 13,
	VALVE_ID14 = 14,
	VALVE_ID15 = 15,
	VALVE_ID16 = 16,
	VALVE_ID17 = 17,
	VALVE_ID18 = 18,
	VALVE_ID19 = 19,
	VALVE_ID20 = 20,
	VLA_VALVE = 21,
	VALVE_MAX = 22
	
}sv_valvetype;

typedef enum {
	SV_PUMP_NULL = 0,
	DCMOTOR1 = 1,
	DCMOTOR2 = 2,
	BLOODPUMP = 3,
	UFPUMP = 4,
	HEPARINPUMP = 5,
	VLA_PUMP = 6,
//	SV_PUMP_ID6 = 6
}sv_pumptype;

typedef enum {
	SV_SENSOR_NULL = 0,
	SV_SENSOR_ID1 ,  //PS1
	SV_SENSOR_ID2 ,	//PS2
	SV_SENSOR_ID3 ,	//PS3
	SV_SENSOR_ID4 , //TMP1
	SV_SENSOR_ID5 , //TMP2
	SV_SENSOR_ID6 , // TMP3
	SV_SENSOR_ID7 , //APT
	SV_SENSOR_ID8 ,	//VPT
	SV_SENSOR_ID9 ,	// XXXXXXXXXXXXXXX
	SV_SENSOR_ID10, // COND
	SV_SENSOR_ID11, //COND_DAC
	SV_SENSOR_ID12, // COND_WEINBRIDGE
	SV_SENSOR_ID13, // THERMO_COUPLE
}sv_sensortype;

typedef struct{
	uint8_t bloodpumpdoor;
	uint16_t BPstatus;
	uint16_t BP_INC;
	uint16_t BP_DEC;
	uint16_t Heparin_rotation_marker;
	uint16_t Heparin_full_marker;
	uint16_t Heparin_empty_marker;
	uint16_t aptstatus;
	uint16_t vptstatus;
	uint16_t VC_status;
	uint16_t VC_door_status;
	uint16_t bdstatus;
	uint16_t abdstatus;
	uint16_t FPcurrentstatus;
	uint16_t UFPstatus;
	uint16_t ps1status;
	uint16_t ps2status;
	uint16_t ps3status;
	uint16_t Temp1status;
	uint16_t Temp2status;
	uint16_t Temp3status;
	uint16_t Temp4status;
	uint16_t cond_status;
	uint16_t cond_status_dac;
	uint16_t cond_wien_status;
	uint8_t levelswitch1;
	uint16_t Flowstatus;
	uint16_t bldstatus;	
	uint8_t holderstatus1;
	uint8_t holderstatus2;
	uint8_t calibration;
	uint8_t acid_inlet_status;
	uint8_t bicarb_inlet_status;
	uint8_t CPU2_interupt;
	uint8_t CPU3_interrupt;
	uint8_t Redundancy_IRQ;



	uint16_t ps4status;



	uint16_t DGPcurrentstatus;
	


}Sys_statusType;

typedef enum {
	SV_SENSIN_NULL = 0,
	SV_SENSIN_ID1 = 1,	 //levelswitch1
	SV_SENSIN_ID2 = 2,	// DGP current status
	SV_SENSIN_ID3 = 3,	// FP current status
	SV_SENSIN_ID4 = 4,	//UFP fb status
	SV_SENSIN_ID5 = 5,	// BP fb status
	SV_SENSIN_ID6 = 6,	// HP full
	SV_SENSIN_ID7 = 7,	//HP emty
	SV_SENSIN_ID8 = 8,	//abd status
	SV_SENSIN_ID9 = 9,	//HLDR 1
	SV_SENSIN_ID10 = 10, //HLDR 2
	SV_SENSIN_ID11 = 11,	//HP STATUS
	SV_SENSIN_ID12 = 12, //FLOW STATUS
	SV_SENSIN_ID13 = 13, //HP ROTATION MARKER
	SV_SENSIN_ID14 = 14, //BP DOOR
	SV_SENSIN_ID15 = 15, //BP INC
	SV_SENSIN_ID16 = 16, //BP DEC
	SV_SENSIN_ID17 = 17, //VC_status
	SV_SENSIN_ID18 = 18, //vc-door-status
	SV_SENSIN_ID19	= 19, // calibration
	SV_SENSIN_ID20	= 20, //acid inlet
	SV_SENSIN_ID21	= 21, // bi carb inlet
	SV_SENSIN_ID22	= 22, //CPU2
	SV_SENSIN_ID23	= 23, //CPU3
	SV_SENSIN_ID24	= 24, //Redundancy
	SV_SENSIN_ID25	= 25,
	SV_SENSIN_ID26	= 26,
	SV_SENSIN_ID27	= 27,
	SV_SENSIN_ID_MAX
	
}sv_sensintype;


typedef enum {
	SV_TIMER0 = 0,
	SV_TIMER1 = 1,
	SV_TIMER2 = 2,
	SV_TIMER3 = 3,
		
}sv_timer_ch;



#endif /* SV_INTERFACE_H_ */
