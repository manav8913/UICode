/*
 * cl_deaeration.h
 *
 * Created: 7/30/2015 12:12:11 PM
 *  Author: user
 */ 


#ifndef CL_DEAERATION_H_
#define CL_DEAERATION_H_

typedef enum
{
	CL_DEAER_EVENT_ACTIVATE,
	CL_DEAER_EVENT_DEACTIVATE,
	CL_DEAER_EVENT_50MS,
	CL_DEAER_EVENT_500MS,
}cl_deaeration_event_type;


typedef enum
{
	CL_DEAER_STATE_IDLE,
	CL_DEAER_ACTIVE,
	CL_DEAER_ACTIVE_INPROGRESS,
	CL_DEAER_ACTIVE_FINISHED,
	CL_DEAER_NONE
	
}cl_deaeration_state_type;



#endif /* CL_DEAERATION_H_ */