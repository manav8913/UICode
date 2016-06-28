/*
 * calibration.h
 *
 * Created: 4/19/2016 11:11:26 AM
 *  Author: renalyx
 */ 


#ifndef CALIBRATION_H_
#define CALIBRATION_H_

typedef struct
{
	uint32_t volts;
	uint32_t temperature;
}temptre;

typedef struct
{
	uint32_t volts;
	uint32_t conductivity;
}conduc;

typedef struct
{
	uint32_t ml;
	uint32_t speed;
}hep;

typedef enum
{
	TS1,
	TS2,
	TS3
}temp_state;

void calibration_apt(uint16_t sensordata);
void calibration_ps1(uint16_t sensordata);
void calibration_ps2(uint16_t sensordata);
void calibration_ps3(uint16_t sensordata);
void calibration_vpt(uint16_t sensordata);
void calibration_cond(float millivolts);
void calibration_tmp(float millivolts , temp_state temp_var);
void hep_calibration(uint32_t inpt_ml);



#endif /* CALIBRATION_H_ */
