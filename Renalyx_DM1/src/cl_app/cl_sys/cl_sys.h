/*
 * cl_sys.h
 *
 * Created: 8/7/2014 11:17:52 AM
 *  Author: user
 */ 


#ifndef CL_SYS_H_
#define CL_SYS_H_






typedef enum 
{
	SYS_STATE_IDLE,
	SYS_STATE_WAIT_FOR_MODE,
	SYS_STATE_NORMAL,
	SYS_STATE_CALIBRATE,
	SYS_STATE_MAX,
}SYS_STATE;

#endif /* CL_SYS_H_ */