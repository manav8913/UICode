/*
 * cl_datamanager.h
 *
 * Created: 6/17/2016 5:15:03 PM
 *  Author: user
 */ 


#ifndef CL_DATAMANAGER_H_
#define CL_DATAMANAGER_H_


typedef union 
{
	uint8_t bytearray[4];
	uint16_t twoByte;
	uint32_t word;
	
}rxstreamtype;



#endif /* CL_DATAMANAGER_H_ */