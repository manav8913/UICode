/*
 * SV_ALARM.h
 *
 * Created: 1/6/2014 10:38:15 AM
 *  Author: Geekay
 */ 


#ifndef SV_ALARM_H_
#define SV_ALARM_H_

#include "Platform\Service\sv_interface.h"
#include "asf.h"

#include "Platform/Drivers/DD_DRIVER_EXPORT_CALLS.h"


uint8_t sv_alind_setalarm_on(sv_alarmtype sv_alarm);
uint8_t sv_alind_resetalarm_on(sv_alarmtype sv_alarm);



#endif /* SV_ALARM_H_ */