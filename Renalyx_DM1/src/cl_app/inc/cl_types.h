/*
 * Cl_types.h
 *
 * Created: 12/27/2013 5:32:24 PM
 *  Author: sanjeer_k
 */ 



#ifndef CL_TYPES_H_
#define CL_TYPES_H_
#include "asf.h"
#define CONSOLE_BUFFER_SIZE 20 
#define CONSOLE_MSG_BUFFER_SIZE 40

typedef enum 
{
	CL_OK,
	CL_WAIT,
	CL_ACCEPTED,
	CL_REJECTED,
	CL_ERROR
	
}Cl_ReturnCodes;

typedef struct {
	uint8_t Console_databuffer[CONSOLE_BUFFER_SIZE];
	uint8_t Console_databuffer_write_idx;
	uint8_t Console_databuffer_read_idx;
	bool ConsoleNewData;
	bool ConsoleDatabufferReady;
	bool ConsoleDatabufferprocessed;
	bool CurreltyActive;
	bool StartSequenceDetected;
	bool BufferEmpty;
	bool Data_incomplete;
	uint8_t BufferSize ;
	
	} cl_console_bufferType;
typedef struct {
	uint8_t Console_databuffer[CONSOLE_MSG_BUFFER_SIZE];
	uint8_t Console_databuffer_write_idx;
	uint8_t Console_databuffer_read_idx;
	bool ConsoleNewMessageStartReady;
	bool ConsoleNewMessageReady;
	bool ConsoleMsgRead;
	uint8_t Cmdptr;
	uint8_t BufferSize ;
	
} cl_console_msgbufferType;

typedef union
	{
		uint8_t byte[20] ;
		uint16_t dataTwobyte  ;
		uint32_t dataword ;
	
	}Cl_ConsoleDataType;
	
typedef struct{
	uint8_t start1;
	uint8_t start2;
	uint8_t command;
	uint8_t datasize;
	//uint8_t data[20];
	Cl_ConsoleDataType data;
	bool msgready;
	}Cl_ConsoleMsgType;


typedef struct{
	bool bloodpumpdoor;
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
	uint16_t abdstatus;
	uint16_t bdstatus;
	uint16_t FPcurrentstatus;
	uint16_t DGPcurrentstatus;
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

	bool levelswitch1;
	bool levelswitch2;
	uint16_t Flowstatus;
	uint16_t bldstatus;	

	bool holder1status;
	bool holder2status;
	bool acid_in;
	bool bicarb_in;
	uint8_t calibration;
	uint8_t acid_inlet_status;
	uint8_t bicarb_inlet_status;
	uint8_t CPU2_interupt;
	uint8_t CPU3_interrupt;
	uint8_t Redundancy_IRQ;

		
	uint16_t cs1status;
	uint16_t cs2status;


	uint16_t ps4status;



	uint16_t Flowcounter;

//	uint16_t BloodPump_rotation_marker;

}Cl_Sys_statusType;
	
#endif /* CL_TYPES_H_ */
