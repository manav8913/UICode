/*
 * DD_NVM.h
 *
 * Created: 1/6/2014 4:29:05 PM
 *  Author: Geekay
 */ 


#ifndef DD_NVM_H_
#define DD_NVM_H_

#include "asf.h"


/* Internal Flash 0 base address. */
#define IFLASH_ADDR     IFLASH0_ADDR
/* Internal flash page size. */
#define IFLASH_PAGE_SIZE     IFLASH0_PAGE_SIZE

/* Last page start address. */
#define LAST_PAGE_ADDRESS (IFLASH1_ADDR + IFLASH1_SIZE - IFLASH1_PAGE_SIZE)

typedef unsigned long UL;

void DD_INIT_NVM(void);
uint8_t Write_NVRAM(void);


#endif /* DD_NVM_H_ */