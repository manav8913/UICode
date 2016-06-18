/*
 * flowswitchcntrl.c
 *
 * Created: 11/9/2014 2:08:25 PM
 *  Author: user
 */ 
#include "inc/flowswitchcntrl.h"
#include "Platform/Service/sv_interface.h"
#include "cl_app/cl_alarms/inc/cl_alarmdetector.h"
#include "cl_app/cl_console/inc/cl_consolecontroller.h"
#include "cl_app/cl_testharness/inc/cl_th.h"

extern Cl_ReturnCodes Cl_AlarmResetAlarm(Cl_NewAlarmIdType  );
extern Cl_ReturnCodes Cl_Alarm_TriggerAlarm(Cl_NewAlarmIdType cl_alarm_id , bool cl_status);
extern Cl_ReturnCodes  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , uint8_t* ,uint8_t );

Flowswitch_msgType Flowswitch_msg = {FLOW_SWITCH_EVENT_UNDEFINED,0 };
flowswitchstate_type flow_switch_state = FLOW_SWITCH_UNDEFINED;
flowswitchcntrlstate_type  flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
static uint16_t toggle_count = 0;
	
	Cl_ReturnCodes cl_flow_switchcontroller(flowswitchcntrl_event_type cntrl_event)
	{
		cl_Datastreamtype cl_tdata;
		
		switch(flowswitchcntrlstate)
		{
			case FLOW_SWITCH_CNTRL_ACTIVE:
					switch(cntrl_event)
					{
						case FLOW_SWITCH_CNTRLER_EVENT_START:
			
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_STOP:
						flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_SEC:
						Cl_AlarmResetAlarm(FLOW_NO_FLOW);
						Cl_AlarmResetAlarm(FLOW_LOW_FLOWRATE);
						
										cl_tdata.word = 0;

										
										cl_tdata.Twobyte = toggle_count;
										cl_tdata.bytearray[2] = 21;
										
										
										
									//	 Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
						if(toggle_count < 5 )
						{
							// low  flow alarm 
							Cl_Alarm_TriggerAlarm(FLOW_LOW_FLOWRATE,1);
							flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
						}
						if(toggle_count > 1000 )
						{
							//  high  flow alarm 
							Cl_Alarm_TriggerAlarm(FLOW_HIGH_FLOWRATE,1);
							flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
						}

						toggle_count = 0;
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_100MS:
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_10MS:
						//break;
						case FLOW_SWITCH_CNTRLER_EVENT_INSTANT:
			
							if(Flowswitch_msg.flowswitch_event == FLOW_SWITCH_EVENT_LEVEL_HIGH)
							{
								Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;
				
								if(flow_switch_state != FLOW_SWITCH_LEVEL_HIGH )
								{
									
									toggle_count++;
									flow_switch_state = FLOW_SWITCH_LEVEL_HIGH;
								}

							}
							else if(Flowswitch_msg.flowswitch_event ==  FLOW_SWITCH_EVENT_LEVEL_LOW)
							{

								Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;

								if(flow_switch_state != FLOW_SWITCH_LEVEL_LOW )
								{
									toggle_count++;
									flow_switch_state = FLOW_SWITCH_LEVEL_LOW;
								}

					
							}
						break;
						default:break;
					}
			break;
			case FLOW_SWITCH_CNTRL_IDLE:
								switch(cntrl_event)
								{
									case FLOW_SWITCH_CNTRLER_EVENT_START:
									flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_READY;
									break;
									case FLOW_SWITCH_CNTRLER_EVENT_STOP:
									break;
								}
			break;
			
			case FLOW_SWITCH_CNTRL_READY:
								switch(cntrl_event)
								{			
									case FLOW_SWITCH_CNTRLER_EVENT_SEC:
									flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_ACTIVE;
									break;
									case FLOW_SWITCH_CNTRLER_EVENT_INSTANT:
			
									if(Flowswitch_msg.flowswitch_event == FLOW_SWITCH_EVENT_LEVEL_HIGH)
									{
										Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;
				
										if(flow_switch_state != FLOW_SWITCH_LEVEL_HIGH )
										{
									
											toggle_count++;
											flow_switch_state = FLOW_SWITCH_LEVEL_HIGH;
										}

									}
									else if(Flowswitch_msg.flowswitch_event ==  FLOW_SWITCH_EVENT_LEVEL_LOW)
									{

										Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;

										if(flow_switch_state != FLOW_SWITCH_LEVEL_LOW )
										{
											toggle_count++;
											flow_switch_state = FLOW_SWITCH_LEVEL_LOW;
										}

					
									}
								break;
									default:break;
								}
			break;
			case FLOW_SWITCH_CNTRL_STOPPED:
								switch(cntrl_event)
								{
									case FLOW_SWITCH_CNTRLER_EVENT_START:
									flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_ACTIVE;
									
									break;
									case FLOW_SWITCH_CNTRLER_EVENT_STOP:
									break;
								}
			break;
		}
	}