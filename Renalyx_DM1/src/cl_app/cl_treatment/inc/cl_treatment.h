/*
 * cl_treatment.h
 *
 * Created: 5/17/2016 11:54:34 AM
 *  Author: user
 */ 


#ifndef CL_TREATMENT_H_
#define CL_TREATMENT_H_

#include "cl_app/inc/cl_types.h"

typedef struct  
{

uint32_t Val_dflow;
uint32_t Val_settemp;
uint32_t Val_heprate;
uint32_t Val_apt;
uint32_t Val_vpt;
uint32_t Val_tmp;
uint32_t Val_cond;
uint32_t Val_ufrate;
uint32_t Val_ufgoal;
uint32_t Val_bolusvol;
uint32_t Val_bloodrate;
uint32_t Val_bicarbcond;
uint32_t Val_hepstoptime;
uint32_t Val_syringetype;
uint32_t Val_heparincheck;
uint32_t Val_minufrate;
uint32_t Val_treattime;
uint32_t Val_bloodratereturn;
uint32_t Val_bloodratetreat;
uint32_t Val_tempulimit;
uint32_t Val_templlimit;
uint32_t Val_tmpllimit;
uint32_t Val_tmpulimit;
uint32_t Val_vptllimit;
uint32_t Val_vptulimit;
uint32_t Val_ufllimit;
uint32_t Val_ufulimit;
uint32_t Val_dflowllimit;
uint32_t Val_dflowulimit;
uint32_t Val_condllimit;
uint32_t Val_condulimit;
uint32_t Val_aptllimit;
uint32_t Val_aptulimit;



}TreatmentParamType;



typedef struct  
{
	uint32_t 	Val_dflow;
	uint32_t    Val_tmp;
	uint32_t 	Val_setcond;
}DilysateParamType;

#endif /* CL_TREATMENT_H_ */