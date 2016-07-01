/*
 * cl_mac_controller.h
 *
 * Created: 12/27/2013 12:17:48 PM
 *  Author: user
 */ 


#ifndef CL_MAC_CONTROLLER_H_
#define CL_MAC_CONTROLLER_H_
#include "asf.h"

#define MAC_EVENT_COUNT_MAX   10
#define MAC_EVENT_DATA_MAX   4

typedef enum {
	MAC_DUMMY,
	MAC_START,
	MAC_INIT,
	MAC_IDLE,
	MAC_CONSOLE_SYNC,
	MAC_STANDBY,
	MAC_SAFE_STANDBY,
	MAC_RINSE,
	MAC_DISINFECT,
	MAC_SAFE_RINSE,
	MAC_POST_RINSE_STANDBY,
	MAC_DPREP,
	MAC_DPREP_PRIME,
	MAC_SAFE_DPREP,
	MAC_POST_DPREP_STANDBY,
	MAC_DIALYSIS,
	MAC_SAFE_DIALYSIS,
	MAC_POST_DIALYSIS_STANDBY,
	MAC_SERVICE,
	MAC_STATE_MAX
} MAC_STATES;
#if 0
typedef enum {

	MACREQ_SYNC_DONE,
	MACREQ_RINSE_CONFIRMED,
	MACREQ_RINSE_STARTED,
	MACREQ_RINSE_COMPLETED,
	MACREQ_DIALYSIS_CONFIRMED,
	MACREQ_DIALYSIS_STARTED,
	MACREQ_DIALYSIS_COMPLETED,
	MACREQ_INVALID,
} MAC_INTREQ;
#endif

typedef enum {
	EVT_NULL,
	EVT_CONSOLE_COMMAND_SYNC,
	EVT_CONSOLE_COMMAND_ACK,
	EVT_CONSOLE_COMMAND_USER_MODE_ENTER,
	EVT_CONSOLE_COMMAND_USER_MODE_EXIT,
	EVT_CONSOLE_COMMAND_SERVICEMODE_ENTER,
	EVT_CONSOLE_COMMAND_SERVICEMODE_EXIT,
	EVT_CONSOLE_COMMAND_RINSE_START,
	EVT_CONSOLE_COMMAND_STOP_RINSE,
	EVT_CONSOLE_COMMAND_DIALYSIS_PREP,
	EVT_CONSOLE_COMMAND_SET_BLDPMP_ON,
	EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF,
	EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE,
	EVT_CONSOLE_COMMAND_SET_DATA,
	EVT_CONSOLE_COMMAND_GET_DATA,
	EVT_CONSOLE_COMMAND_START_PRIME,
	EVT_CONSOLE_COMMAND_STOP_PRIME,
	EVT_CONSOLE_COMMAND_START_DIALYSATE_FILLING,
	EVT_CONSOLE_COMMAND_DILYSER_CONNECTED,
	EVT_CONSOLE_COMMAND_START_DIALISER_PRIME,
	EVT_CONSOLE_COMMAND_STOP_DIALISER_PRIME,
	EVT_CONSOLE_COMMAND_HEP_PMP_SET,
	EVT_CONSOLE_COMMAND_MAN_PREP_COMPLETED,
	EVT_CONSOLE_COMMAND_DIALYSIS_START,
	EVT_CONSOLE_COMMAND_DIALYSIS_STOP,
	EVT_CONSOLE_COMMAND_DIALYSIS_PAUSE,
	EVT_CONSOLE_COMMAND_BYPASS_ON,
	EVT_CONSOLE_COMMAND_BYPASS_OFF,
	EVT_CONSOLE_COMMAND_PATIENT_CONNECTED,
	EVT_SEND_MIXING_PREP_START,
	EVT_SEND_MIXING_PREP_STOP,
	EVT_CONSOLE_COMMAND_RCIRC_START,
	EVT_CONSOLE_COMMAND_RCIRC_STOP,
	EVT_CONSOLE_COMMAND_PATIENT_READY,
	EVT_CONSOLE_COMMAND_START_RETURN,
	EVT_CONSOLE_COMMAND_STOP_RETURN,
	EVT_CONSOLE_COMMAND_MINUF_ON,
	EVT_CONSOLE_COMMAND_MINUF_OFF,
	EVT_COMMAND_ENTER_INIT,
	EVT_COMMAND_ENTER_STANDBY,
	EVT_SYS_STATUS_DATA,
	EVT_ALERT_TRIGGERED,
	EVT_ALARM_TRIGGERED,
	EVT_ALERT_CS,
	EVT_SYS_DATA_CHANGED,
	EVT_TIMER_EXPIRED,
	EVT_TICK_INSTANT,
	EVT_TICK_1M,
	EVT_TICK_2M,
	EVT_TICK_5M,
	EVT_TICK_10M,
	EVT_TICK_20M,
	EVT_TICK_50M,
	EVT_TICK_100M,
	EVT_TICK_500M,
	EVT_TICK_SEC,
	EVT_TICK_MIN,
	EVT_TICK_HOUR,
	EVT_SEND_CONSOLE_COMMAND_SYNC,
	EVT_CONSOLE_COMMAND_DISINF_START,
	EVT_CONSOLE_RESUME_DISINF_STAGE,
	EVT_CONSOLE_COMMAND_STOP_DISINF,
	MACREQ_SYNC_DONE,
	MACREQ_ENTER_STANDBY,
	MACREQ_START_PRE_DISINF_RINSE,
	MACREQ_START_POST_DISINF_RINSE,
	MACREQ_ENTER_SAFE_STANDBY,
	MACREQ_ENTER_SAFE_RINSE,
	MACREQ_RINSE_COMPLETED,
	MACREQ_PRIME_COMPLETED,
	MACREQ_ENTER_SAFE_PRIME,
	MACREQ_ENTER_SAFE_PREP,
	MACREQ_DIALYSER_PRIME_COMPLETED,
	MACREQ_DIALYSIS_CONFIRMED,
	MACREQ_DIALYSIS_STARTED,
	MACREQ_ENTER_SAFE_DIALYSIS,
	MACREQ_DIALYSIS_COMPLETED,
	MACREQ_BC_OPENFILL_COMPLETED,
	MACREQ_PRE_DISINFECT_RINSE_COMPLETED,
	MACREQ_POST_DISINFECT_RINSE_COMPLETED,
	MACREQ_DISINF_COMPLETED,
	MACREQ_HEPARIN_BOLUS_COMPLETED,
	
	
	
	EVT_MAX
} MAC_EVENTS;


typedef enum {
	MAC_ALARM_EVT_NULL,
	
	MAC_ALARM_EVT_ALERT_TRIGGERED,
	MAC_ALARM_EVT_ALARM_TRIGGERED,
	MAC_ALARM_EVT_SYS_DATA_CHANGED,
	MAC_ALARM_EVT_TIMER_EXPIRED,

	
	MAC_ALARM_EVT_MAX
} MAC_ALARM_EVENTS;


typedef struct  
{
uint8_t	Cl_MacNewEventcount;
bool Cl_MacNewEvent;
MAC_EVENTS Cl_MacEvent[MAC_EVENT_COUNT_MAX];
uint16_t Cl_MacEventData[MAC_EVENT_COUNT_MAX][MAC_EVENT_DATA_MAX];
	
}Cl_Mac_EventType;

typedef struct
{
	uint8_t	Cl_MacNewEventcount;
	bool Cl_MacNewEvent;
	MAC_EVENTS Cl_MacEvent[MAC_EVENT_COUNT_MAX];
	uint16_t Cl_MacEventData[MAC_EVENT_COUNT_MAX][MAC_EVENT_DATA_MAX];
	
}Cl_Mac_AlarmEventType;

#endif /* CL_MAC_CONTROLLER_H_ */
