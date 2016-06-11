/*
 * SV_STATUS.h
 *
 * Created: 1/6/2014 10:35:59 AM
 *  Author: Geekay
 */ 


#ifndef SV_STATUS_H_
#define SV_STATUS_H_

#include "asf.h"
#include "Platform\Service\sv_interface.h"
#include "Platform/Drivers/DD_DRIVER_EXPORT_CALLS.h"


uint8_t  sv_status_getFPcurrentsense(void);
uint8_t  sv_status_getlevelswitchstatus(void);
uint8_t  sv_status_getDGPcurrentsense(void);
uint8_t  sv_status_getUFPstatus(void);
uint8_t  sv_status_getBPstatus(void);
uint8_t  sv_status_getbldstatus(void);
uint8_t  sv_status_getbdstatus(void);
uint8_t  sv_status_abdstatus(void);
uint8_t  sv_status_getdholderstatus(void);
uint8_t  sv_status_gethpumpstatus(void);
uint16_t sv_status_getcond_status(sv_sensortype sv_sensor);
uint16_t sv_status_getTs1temprature(sv_sensortype sv_sensor);
uint16_t sv_status_getTs2temprature(sv_sensortype sv_sensor);
uint16_t sv_status_getTs3temprature(sv_sensortype sv_sensor);
uint16_t sv_status_getPs1temprature(sv_sensortype sv_sensor);
uint16_t sv_status_getPs2temprature(sv_sensortype sv_sensor);
uint16_t sv_status_getPs3temprature(sv_sensortype sv_sensor);
uint16_t sv_status_getaptstatus(sv_sensortype sv_sensor);
uint16_t sv_status_getvptstatus(sv_sensortype sv_sensor);
uint16_t sv_status_getsystemstatus(Sys_statusType* sensor_struct);

#endif /* SV_STATUS_H_ */