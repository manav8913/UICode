/*
 * DD_CLOCK.c
 *
 * Created: 1/6/2014 11:42:37 AM
 *  Author: Geekay
 */ 
#include "DD_CLOCK.h"

void DD_INIT_CLOCK()
{
	
	sysclk_init();
	PMC->PMC_WPMR  = 0x504D4300;
	PMC->PMC_PCER0 = 0xF8007900;
	PMC->PMC_PCER1 = 0x0000003F;
	
}