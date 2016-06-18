/*
 * SV_CNTRL.c
 *
 * Created: 1/6/2014 10:35:04 AM
 *  Author: Geekay
 */ 

#include "SV_CNTRL.h"
#include "cl_app/inc/cl_types.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
//#define BLOODPUMP_IIC_ADR 0x60
#ifndef BLOODPUMP_IIC_ADR
#define BLOODPUMP_IIC_ADR 0x0E
#endif
//#define BLOODPUMP_IIC_ADR 0x61
void DD_IIC_SET_BLOODPUP(uint8_t iic_address, uint8_t* data,uint8_t length);

extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );
extern 	uint32_t ul_cur_ticks, delayticks;//testing
extern volatile uint32_t g_ul_ms_ticks ;
extern float dummy1;

uint8_t sv_cntrl_activatepump(sv_pumptype sv_pump_id)
{
	uint8_t bloodpumpspeed[3];
	
	if(sv_pump_id != BLOODPUMP)
	{
		DD_SET_PUMP(sv_pump_id);
	
	#if 0	
	if(sv_pump_id == HEPARINPUMP  )
	{
		bloodpumpspeed[0] = 5;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
	}
	else if(sv_pump_id == UFPUMP  ) {
				bloodpumpspeed[0] = 3;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
	} else if(sv_pump_id == DCMOTOR2  )
	{
				bloodpumpspeed[0] = 7;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
		
	}else if(sv_pump_id == DCMOTOR1  )
	{
						bloodpumpspeed[0] = 9;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
		
	}
	#endif
	
	} 
	else	
	{
		bloodpumpspeed[0] = 1;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
		
	}
	
	return 0;
}

uint8_t sv_cntrl_deactivatepump(sv_pumptype sv_pump_id)
{
	uint8_t bloodpumpspeed[3];
	if(sv_pump_id != BLOODPUMP)
	{
		DD_RESET_PUMP(sv_pump_id);

#if 0
	if(sv_pump_id == HEPARINPUMP  )
	{
		bloodpumpspeed[0] = 6;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
	}
	else if(sv_pump_id == UFPUMP  ) {
				bloodpumpspeed[0] = 4;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
	} else if(sv_pump_id == DCMOTOR2  )
	{
				bloodpumpspeed[0] = 8;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
		
	}else if(sv_pump_id == DCMOTOR1  )
	{
						bloodpumpspeed[0] = 0xa;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
		
	}
	
	#endif
	} else
	
	
	{
		bloodpumpspeed[0] = 2;
		DD_IIC_SET_BLOODPUP(BLOODPUMP_IIC_ADR,&bloodpumpspeed,1);
	}
	
	
	return 0;
}
uint8_t sv_cntrl_poweronheater(void)
{
	 DD_SET_HEATER();
	 return 0;
}
uint8_t sv_cntrl_poweroffheater(void)
{
	//Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,( uint8_t*)"HEAT_OFF",8);
	DD_RESET_HEATER();
	return 0;
}



uint8_t sv_cntrl_activate_valve(sv_valvetype sv_valve_id)
{
	//if(sv_valve_id == VALVE_ID4)
	//{
	//	DD_RESET_VALVE(sv_valve_id);
	//}
	//else
	{
		DD_SET_VALVE(sv_valve_id);
	}
	
	
	return 0;
}

uint8_t sv_cntrl_deactivate_valve(sv_valvetype sv_valve_id)
{
	
	if(sv_valve_id == VALVE_ID16)
	{
		DD_RESET_HEATER();
	}
	DD_RESET_VALVE(sv_valve_id);
	
	return 0;
}


uint8_t sv_cntrl_setHepa_dir(void)
{
	DD_SET_DIR();
	
	return 0;
}



uint8_t sv_cntrl_resetHepa_dir()
{
	DD_RESET_DIR();
	
	return 0;	
}

uint8_t sv_cntrl_incpumpspeed(sv_pumptype sv_pump_id,uint32_t sv_unit)
{
	DD_INC_SPEED(sv_pump_id,sv_unit);
	
	return 0;
}

uint8_t sv_cntrl_decpumpspeed(sv_pumptype sv_pump_id,uint32_t sv_unit)
{
	DD_DEC_SPEED(sv_pump_id,sv_unit);
	return 0;
}

uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed)
{
	uint8_t bloodpumpspeed[3];
	
		bloodpumpspeed[0] = 0x04 ;
		bloodpumpspeed[1] = 0xff & speed ;
		bloodpumpspeed[2] = 0xff & speed >> 8 ;
		
		if(sv_pump_id != BLOODPUMP)
		{
				DD_SET_SPEED(sv_pump_id,speed);
		} else
		
		
		{
			

			   DD_IIC_SET_BLOODPUP( BLOODPUMP_IIC_ADR,  &bloodpumpspeed , 0x03);
		}
		
	
	return 0;
}


uint8_t sv_cntrl_incheater(int32_t dty_val)


{
//	dty_val = 2900;
	
	dummy1 = dty_val; 
	
	if(dty_val > 3500)
	{
		dty_val = 3500;
	}
	 dty_val = dty_val * 0.66;
	 
	 if ( dty_val > 0)
	 {
		 DD_CHNG_HTR(dty_val);
	 }
	
	
	return 0;
}


uint8_t sv_cntrl_decheater(int32_t dty_val)
{
		 if ( dty_val > 0)
	 {
		 DD_CHNG_HTR(dty_val);
	 }
	
	
	return 0;
}

uint8_t sv_cntrl_activatevenousclamp(void)
{
	DD_SET_CLAMP();
	
	return 0;
}

uint8_t sv_cntrl_deactivatevenousclamp(void)
{
	DD_RESET_CLAMP();
	
	return 0;
}

uint8_t sv_cntrl_enable_loopback()
{

	DD_ENABLE_LOOPBACK();	
	return 0;

}


uint8_t sv_cntrl_disable_loopback()
{

	DD_DISABLE_LOOPBACK();
	return 0;

}


uint8_t sv_cntrl_enable_bypass()
{

	DD_ENABLE_BYPASS();
	sv_cntrl_deactivatepump(UFPUMP);
	return 0;

}


uint8_t sv_cntrl_disable_bypass()
{

	DD_DISABLE_BYPASS();
	return 0;

}





uint8_t sv_cntrl_setflowpath(sv_flowpathtype sv_flowpath)
{
		delayticks = g_ul_ms_ticks - ul_cur_ticks;
		ul_cur_ticks = g_ul_ms_ticks;
	DD_SET_FLOW_PATH(sv_flowpath);
	
	return 0;

}


uint8_t sv_cntrl_setpumpfreq(sv_pumptype sv_pump_id,uint32_t freq)
{
	DD_SET_PUMPFREQ(sv_pump_id,freq);
	return 0;
}