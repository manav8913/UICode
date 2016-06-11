/*
 * cl_status.h
 *
 * Created: 12/17/2015 12:33:36 PM
 *  Author: user
 */ 


#ifndef CL_STATUS_H_
#define CL_STATUS_H_

#include "cl_app/inc/cl_types.h"
typedef union{
	
	uint8_t bytearray[4] ;
	uint16_t Twobyte  ;
	uint32_t word ;
	
}cl_Datastreamtype;


#endif /* CL_STATUS_H_ */