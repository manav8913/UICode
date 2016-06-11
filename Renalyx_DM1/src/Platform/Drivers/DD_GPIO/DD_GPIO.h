/*
 * DD_GPIO.h
 *
 * Created: 1/6/2014 10:44:55 AM
 *  Author: Geekay
 */ 


#ifndef DD_GPIO_H_
#define DD_GPIO_H_

#include "asf.h"

/* GPIO Support */
#define PIO_TYPE_Pos                  27
/* The pin is controlled by the peripheral A. */
#define PIO_TYPE_PIO_A         (0x1u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral B. */
#define PIO_TYPE_PIO_B         (0x2u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral C. */
#define PIO_TYPE_PIO_C         (0x3u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral D. */
#define PIO_TYPE_PIO_D         (0x4u << PIO_TYPE_Pos)

typedef enum _pio
{
	#if (SAM3XA)
	PA    = PIO_TYPE_PIO_A ,
	PB    = PIO_TYPE_PIO_B ,
	PC    = PIO_TYPE_PIO_C ,
	PD    = PIO_TYPE_PIO_D ,
	#endif
} pio_type;


void DD_INIT_PIO(void);
uint8_t DD_SET_PIO( pio_type peripheral,uint32_t pio);
uint8_t DD_RESET_PIO(const pio_type peripheral,uint32_t pio);
uint16_t DD_READ_PIO(pio_type peripheral,uint32_t pio);


#endif /* DD_GPIO_H_ */