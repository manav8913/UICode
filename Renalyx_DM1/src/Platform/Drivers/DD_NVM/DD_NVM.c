/*
 * DD_NVM.c
 *
 * Created: 1/6/2014 4:28:45 PM
 *  Author: Geekay
 */ 

#include "DD_NVM.h"

void DD_INIT_NVM()
{
	
	uint32_t ul_last_page_addr = LAST_PAGE_ADDRESS;
	uint32_t *pul_last_page = (uint32_t *) ul_last_page_addr;
	uint32_t ul_rc;
	uint32_t ul_idx;
	uint8_t uc_key, cnt = 0;
	
	uint32_t ul_page_buffer[IFLASH_PAGE_SIZE / sizeof(uint32_t)];
	
	
	ul_rc = flash_init(FLASH_ACCESS_MODE_128, 6);
	if (ul_rc != FLASH_RC_OK)
	 {
		printf("-F- Initialization error %lu\n\r", (UL)ul_rc);
		return 0;
	}
		
	
	
	/* Unlock page */
	printf("-I- Unlocking last page: 0x%08x\r\n", ul_last_page_addr);
	ul_rc = flash_unlock(ul_last_page_addr,
	ul_last_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Unlock error %lu\n\r", (UL)ul_rc);
		return 0;
	}
	
	/* Write page */
	printf("-I- Writing last page with walking bit pattern\n\r");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++)
	 {
		ul_page_buffer[ul_idx] = cnt ;   //1 << (ul_idx % 32);
		cnt ++;
		}

// 	#if (SAM4S || SAM4E || SAM4N)
// 	/* The EWP command is not supported by SAM4S and SAM4E, SAM4N, so an erase
// 	 * command is requried before any write operation.
// 	 */
// 	ul_rc = flash_erase_sector(ul_last_page_addr);
// 	if (ul_rc != FLASH_RC_OK) {
// 		printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
// 		return 0;
// 	}
// 
// 	ul_rc = flash_write(ul_last_page_addr, ul_page_buffer,
// 			IFLASH_PAGE_SIZE, 0);
// #else
// 	ul_rc = flash_write(ul_last_page_addr, ul_page_buffer,
// 			IFLASH_PAGE_SIZE, 1);
// #endif
// 	if (ul_rc != FLASH_RC_OK) {
// 		printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
// 		return 0;
// 	}
	
	
	/* Validate page */
	printf("-I- Checking page contents ");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		printf(".");
		if (pul_last_page[ul_idx] != ul_page_buffer[ul_idx]) {
			printf("\n\r-F- data error\n\r");
			return 0;
		}
	}
	printf("OK\n\r");
	
	
	
		/* Lock page */
		printf("-I- Locking last page\n\r");
		ul_rc = flash_lock(ul_last_page_addr,
		ul_last_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
		if (ul_rc != FLASH_RC_OK) {
			printf("-F- Flash locking error %lu\n\r", (UL)ul_rc);
			return 0;
		}

		/* Check if the associated region is locked. */
		printf("-I- Try to program the locked page ...\n\r");
		ul_rc = flash_write(ul_last_page_addr, ul_page_buffer,
		IFLASH_PAGE_SIZE, 1);
		if (ul_rc != FLASH_RC_OK) {
			printf("-I- The page to be programmed belongs to locked region. Error %lu\n\r",
			(UL)ul_rc);
		}

		printf("-I- Please open Segger's JMem program \n\r");
		printf("-I- Read memory at address 0x%08lx to check contents\n\r",
		(UL)ul_last_page_addr);
		printf("-I- Press any key to continue...\n\r");
		while (0 != uart_read(CONSOLE_UART, &uc_key));

		printf("-I- Good job!\n\r"
		"-I- Now set the security bit \n\r"
		"-I- Press any key to continue to see what happened...\n\r");
		while (0 != uart_read(CONSOLE_UART, &uc_key));
		
		
		/* Set security bit */
		printf("-I- Setting security bit \n\r");
		ul_rc = flash_enable_security_bit();
		if (ul_rc != FLASH_RC_OK) {
			printf("-F- Set security bit error %lu\n\r", (UL)ul_rc);
		}

		printf("-I- All tests done\n\r");



}

uint8_t Write_NVRAM()
{
	
	uint32_t ul_last_page_addr = LAST_PAGE_ADDRESS;
	uint32_t *pul_last_page = (uint32_t *) ul_last_page_addr;
	uint32_t ul_rc;
	uint32_t ul_idx;
	uint8_t uc_key, cnt = 0;
	
	uint32_t ul_page_buffer[IFLASH_PAGE_SIZE / sizeof(uint32_t)];
	
	
	ul_rc = flash_init(FLASH_ACCESS_MODE_128, 6);
	if (ul_rc != FLASH_RC_OK)
	{
		printf("-F- Initialization error %lu\n\r", (UL)ul_rc);
		return 0;
	}
	
	
	
	/* Unlock page */
	printf("-I- Unlocking last page: 0x%08x\r\n", ul_last_page_addr);
	ul_rc = flash_unlock(ul_last_page_addr,
	ul_last_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK)
	 {
		printf("-F- Unlock error %lu\n\r", (UL)ul_rc);
		return 0;
	 }
	
	
		ul_rc = flash_write(ul_last_page_addr, ul_page_buffer,
		IFLASH_PAGE_SIZE, 1); 
		
		if (ul_rc != FLASH_RC_OK) 
		{
			printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
			return 0;
		}
	
	/* Validate page */	
	printf("-I- Checking page contents ");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		printf(".");
		if (pul_last_page[ul_idx] != ul_page_buffer[ul_idx]) {
			printf("\n\r-F- data error\n\r");
			return 0;
		}
	}
	
		/* Lock page */
		printf("-I- Locking last page\n\r");
		ul_rc = flash_lock(ul_last_page_addr,
		ul_last_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
		if (ul_rc != FLASH_RC_OK) {
			printf("-F- Flash locking error %lu\n\r", (UL)ul_rc);
			return 0;
		}
	
}