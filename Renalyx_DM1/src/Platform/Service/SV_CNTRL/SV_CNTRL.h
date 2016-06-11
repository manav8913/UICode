/*
 * SV_CNTRL.h
 *
 * Created: 1/6/2014 10:34:41 AM
 *  Author: Geekay
 */ 


#ifndef SV_CNTRL_H_
#define SV_CNTRL_H_

#include "Platform\Service\sv_interface.h"
#include "asf.h"

#include "Platform/Drivers/DD_DRIVER_EXPORT_CALLS.h"


uint8_t sv_cntrl_activatepump(sv_pumptype sv_pump_id);
uint8_t sv_cntrl_deactivatepump(sv_pumptype sv_pump_id);
uint8_t sv_cntrl_activate_valve(sv_valvetype sv_valve_id);
uint8_t sv_cntrl_deactivate_valve(sv_valvetype sv_valve_id);
uint8_t sv_cntrl_setHepa_dir(void);
uint8_t sv_cntrl_resetHepa_dir(void);
uint8_t sv_cntrl_incpumpspeed(sv_pumptype sv_pump_id,uint32_t sv_unit);
uint8_t sv_cntrl_decpumpspeed(sv_pumptype sv_pump_id,uint32_t sv_unit);
uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
uint8_t sv_cntrl_setpumpfreq(sv_pumptype sv_pump_id,uint32_t freq);
uint8_t sv_cntrl_activateheater(uint32_t);
uint8_t sv_cntrl_deactivateheater(uint32_t);
uint8_t sv_cntrl_incheater(int32_t dty_val);
uint8_t sv_cntrl_decheater(int32_t dty_val);
uint8_t sv_cntrl_activatevenousclamp(void);
uint8_t sv_cntrl_deactivatevenousclamp(void);
uint8_t sv_cntrl_setflowpath(sv_flowpathtype sv_flowpath);
uint8_t sv_cntrl_poweronheater(void);
uint8_t sv_cntrl_poweroffheater(void);
uint8_t sv_cntrl_enable_bypass(void);
uint8_t sv_cntrl_disable_bypass(void);
uint8_t sv_cntrl_enable_loopback(void);
uint8_t sv_cntrl_disable_loopback(void);




#endif /* SV_CNTRL_H_ */