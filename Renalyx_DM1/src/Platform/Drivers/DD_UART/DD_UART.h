/*
 * DD_UART.h
 *
 * Created: 1/6/2014 11:10:21 AM
 *  Author: Geekay
 */ 


#ifndef DD_UART_H_
#define DD_UART_H_

#include "asf.h"




void DD_INIT_UART(void);

uint32_t DD_TX_UART(uint8_t sv_consoleTxdatabyte);

uint8_t DD_RX_UART(void);
#endif /* DD_UART_H_ */