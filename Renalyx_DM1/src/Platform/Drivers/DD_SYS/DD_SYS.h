/*
 * DD_SYS.h
 *
 * Created: 1/6/2014 10:46:15 AM
 *  Author: Geekay
 */ 


#ifndef DD_SYS_H_
#define DD_SYS_H_

#include "asf.h"
#include "Platform/Drivers/DD_DRIVER_EXPORT_CALLS.h"
#include "Platform/Service/sv_interface.h"



uint8_t DD_SET_PUMP(sv_pumptype id_pump);
uint8_t DD_RESET_PUMP(sv_pumptype id_pump);
uint8_t DD_SET_HEATER(void);
uint8_t DD_RESET_HEATER(void);
uint8_t DD_SET_VALVE(sv_valvetype id_valve);
uint8_t DD_RESET_VALVE(sv_valvetype id_valve);
uint8_t DD_SET_DIR(void);
uint8_t DD_RESET_DIR(void);
uint8_t DD_SET_CLAMP(void);
uint8_t DD_RESET_CLAMP(void);
uint8_t DD_INC_SPEED(sv_pumptype id_pump,uint32_t sv_unit);
uint8_t DD_DEC_SPEED(sv_pumptype id_pump,uint32_t sv_unit);
uint8_t DD_SET_SPEED(sv_pumptype id_pump,uint32_t speed);
uint8_t DD_CHNG_HTR(uint32_t dty_val);
uint16_t DD_READ_SENSORS(sv_sensortype ID_SENSOR);
uint8_t DD_RD_SENSIN(sv_sensintype );
uint8_t DD_SET_ALARM(sv_alarmtype sv_alarm);
uint8_t DD_RESET_ALARM(sv_alarmtype sv_alarm);
uint8_t DD_CONSOLE_TX(uint8_t sv_consoleTxdatabyte);
uint8_t DD_CONFIG_TIMER(uint8_t timer_no, uint8_t timer_res);
uint8_t DD_STOP_TIMER(uint8_t timer_no);
uint8_t DD_SET_PUMPFREQ(sv_pumptype id_pump,uint32_t freq);
uint8_t DD_SET_RED(void);
uint8_t DD_SET_YELLOW(void);
uint8_t DD_SET_GREEN(void);
uint8_t DD_SET_BUZZER(void);
uint8_t DD_RESET_RED(void);
uint8_t DD_RESET_YELLOW(void);
uint8_t DD_RESET_GREEN(void);
uint8_t DD_RESET_BUZZER(void);
uint8_t DD_SET_FLOW_PATH(sv_flowpathtype sv_flowpath);

uint8_t DD_ENABLE_LOOPBACK(void);
uint8_t DD_ENABLE_BYPASS(void);
uint8_t DD_DISABLE_LOOPBACK(void);
uint8_t DD_DISABLE_BYPASS(void);



//extern uint8_t sv_console_receivedbyte(uint8_t usv_consoleRxdatabyte);
extern uint8_t sv_console_receivedbyte(uint8_t sv_consoleRxdatabyte);

#endif /* DD_SYS_H_ */
