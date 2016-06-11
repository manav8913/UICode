/*
 * SV_ALARM.c
 *
 * Created: 1/6/2014 10:37:55 AM
 *  Author: Geekay
 */ 

#include "SV_ALARM.h"



uint8_t sv_alind_setalarm_on(sv_alarmtype sv_alarm)
{
	DD_SET_ALARM(sv_alarm);
	return 0;
}

uint8_t sv_alind_resetalarm_on(sv_alarmtype sv_alarm)
{
	DD_RESET_ALARM(sv_alarm);
	return 0;
}