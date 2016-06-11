/*
 * DD_IIC.c
 *
 * Created: 9/19/2014 3:03:44 PM
 *  Author: user
 */ 

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "ASF\sam\drivers\twi\twi.h"
#include "ASF\common\services\twi\sam_twi\twi_master.h"

#include "DD_IIC.h"

extern uint32_t twi_master_write(Twi *p_twi, twi_packet_t *p_packet);
extern void DD_SET_POT(uint8_t iic_address, uint16_t data);
extern bool iic_nack;
uint32_t twi_master_write1(Twi *p_twi, twi_packet_t *p_packet);

static uint32_t twi_mk_addr(const uint8_t *addr, int len);
volatile int FUNC_ENTRY = 0;
extern volatile int func_timer ;
#define DIPOT_BUS_ADDR1       0x2c       //!< TWI slave bus address (digital pot) (GND)
#define DIPOT_BUS_ADDR2       0x2F        //!< TWI slave bus address (digital pot) (VDD)
//#define DIPOT_MEM_ADDR       0xAAAA        //!< TWI slave memory address
#define TWI_SPEED            100000       //!< TWI data transfer rate(100Khz)
//#define TWI_SPEED            10000       //!< TWI data transfer rate(100Khz)

	twi_master_options_t opt = {
		.speed = TWI_SPEED,
		.chip  =DIPOT_BUS_ADDR1
	};
	


#define PATTERN_TEST_LENGTH     sizeof(test_pattern)
const uint8_t test_pattern[] = {
	0x1C,
	0x02,
	0x04,
0xCD};

#define PATTERN_TEST_LENGTH2     sizeof(test_pattern2)
const uint8_t test_pattern2[] = {
	0x1C,
	0x02,
	0x04,
0xAA};

#define TWI_EXAMPLE TWI0
//#define TWI_EXAMPLE TWI1
//void DD_INIT_IIC(void);
 void DD_READ_IIC(uint8_t iic_address, uint16_t* data);
 void DD_WRITE_IIC(uint8_t iic_address, uint16_t data, uint8_t len);
 void DD_IIC_READ_PROP(uint8_t iic_address, uint16_t* data);
 void DD_IIC_CONSOLE_SEND(uint8_t iic_address, uint8_t* data,uint8_t length);
int aa(void);


static uint8_t test_data=0xCD;
static uint8_t test_data2=0xAA;
//static void transfer_fun(void)
static void transfer_fun(void)
{
	
	uint16_t x;
	uint32_t y;
	uint8_t c;
	twi_master_options_t opt = {
		.speed = TWI_SPEED,
		.chip  =DIPOT_BUS_ADDR1
	};

	twi_master_setup(TWI_EXAMPLE, &opt);
	
		// twi_package_t packet = {
			 twi_packet_t packet = {
		.chip         = DIPOT_BUS_ADDR1,      // TWI slave bus address
		.buffer       = (void *)test_pattern, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH   // transfer data size (bytes)
	};
	// while (twi_master_write(TWI_EXAMPLE, &packet) != TWI_SUCCESS);
	//while (twi_master_write1(TWI_EXAMPLE,(twi_packet_t *) &packet) != TWI_SUCCESS);
	twi_master_write1(TWI_EXAMPLE,&packet) ;
	//twi_master_write1(TWI_EXAMPLE,&iic_packet) ;

}

uint32_t twi_master_write1(Twi *p_twi, twi_packet_t *p_packet)

{
	
 func_timer = 0;
FUNC_ENTRY++;

	uint32_t status, cnt = p_packet->length;
	uint8_t *buffer = (uint8_t*) p_packet->buffer;
	iic_nack = false;
	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	/* Set write mode, slave address and 3 internal address byte lengths */
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send all bytes */
	while (cnt > 0) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			iic_nack = true;
			return TWI_RECEIVE_NACK;
		}

		if (!(status & TWI_SR_TXRDY)) {
			continue;
		}
		p_twi->TWI_THR = *buffer++;

		cnt--;
	}

	while (1) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (status & TWI_SR_TXRDY) {
			break;
		}
	}

	p_twi->TWI_CR = TWI_CR_STOP;

	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}
FUNC_ENTRY = 0;
	return TWI_SUCCESS;
}
static uint32_t twi_mk_addr(const uint8_t *addr, int len)
{
	uint32_t val;

	if (len == 0)
	return 0;

	val = addr[0];
	if (len > 1) {
		val <<= 8;
		val |= addr[1];
	}
	if (len > 2) {
		val <<= 8;
		val |= addr[2];
	}
	return val;
}

static void increment_fun(void)
{
	#define PATTERN_TEST_LENGTH_INC     sizeof(test_pattern)
	uint8_t val = (test_data + 5);
	const uint8_t test_pattern_inc[] = 
	{
		0x1C,
		0x02,
		0x04,
		val
	};
	twi_package_t packet_inc = {
		.chip         = DIPOT_BUS_ADDR1,
		.buffer       = (void *)test_pattern_inc, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH_INC   // transfer data size (bytes)
	};
	printf("%d\n",val);
	while (twi_master_write(TWI_EXAMPLE, &packet_inc) != TWI_SUCCESS);	
	test_data = val;
}

static void decrement_fun(void)
{
	#define PATTERN_TEST_LENGTH_DEC     sizeof(test_pattern)

	uint8_t val = (test_data - 5);
	const uint8_t test_pattern_dec[] =
	{
		0x1C,
		0x02,
		0x04,
		val
	};
	twi_package_t packet_dec = {
		.chip         = DIPOT_BUS_ADDR1,
		.buffer       = (void *)test_pattern_dec, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH_DEC   // transfer data size (bytes)
	};
	printf("%d\n",val);
	while (twi_master_write(TWI_EXAMPLE, &packet_dec) != TWI_SUCCESS);
	test_data = val;
}
static void increment_fun2(void)
{
	#define PATTERN_TEST_LENGTH_INC     sizeof(test_pattern_inc)
	uint8_t val = (test_data2 + 5);
	const uint8_t test_pattern_inc[] =
	{
		0x1C,
		0x02,
		0x04,
		val
	};
	twi_package_t packet_inc = {
		.chip         = DIPOT_BUS_ADDR2,
		.buffer       = (void *)test_pattern_inc, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH_INC   // transfer data size (bytes)
	};
	
	
	printf("%d\n",val);
	while (twi_master_write(TWI_EXAMPLE, &packet_inc) != TWI_SUCCESS);
	test_data2 = val;
}

static void decrement_fun2(void)
{
	#define PATTERN_TEST_LENGTH_DEC     sizeof(test_pattern_dec)

	uint8_t val = (test_data2 - 5);
	const uint8_t test_pattern_dec[] =
	{
		0x1C,
		0x02,
		0x04,
		val
	};
	twi_package_t packet_dec = {
		.chip         = DIPOT_BUS_ADDR2,
		.buffer       = (void *)test_pattern_dec, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH_DEC   // transfer data size (bytes)
	};
	printf("%d\n",val);
	while (twi_master_write(TWI_EXAMPLE, &packet_dec) != TWI_SUCCESS);
	test_data2 = val;
}

static void anotherdevice_fun(void)
{
	twi_master_options_t opt2 = {
		.speed = TWI_SPEED,
		.chip  =DIPOT_BUS_ADDR2
	};

	twi_master_setup(TWI_EXAMPLE, &opt2);
	
	twi_package_t packet = {
		.chip         = DIPOT_BUS_ADDR2,      // TWI slave bus address
		.buffer       = (void *)test_pattern2, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH2   // transfer data size (bytes)
	};
	//while (twi_master_write(TWI_EXAMPLE, &packet) != TWI_SUCCESS);
	while (twi_master_write1(TWI_EXAMPLE, &packet) != TWI_SUCCESS);
	
}

/**
 * \brief Application entry point for spi_slave example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int aa(void)
{
	uint8_t uc_key;

	/* Initialize the SAM system. */
	sysclk_init();
	//board_init();
	//pmc_enable_periph_clk(ID_PIOA);

	/* Initialize the console UART. */
	//configure_console();
	//printf("a\n");

	 while (1)
	 {
		 scanf("%c", (char*)&uc_key);

		 switch (uc_key) {
			 
			 case 'a':
			 transfer_fun();
			 break;
			 
			 case 'i':
			 increment_fun();
			 break;
	 
			 case 'd':
			 decrement_fun();
			 break;
			 
			 case 's':
			 anotherdevice_fun();
			 break;
			 
			 case 'u':
			 increment_fun2();
			 break;
			  
			 case 'v':
			 decrement_fun2();
			 break;

			 default:
			 break;
		 }
	 }

 }
	


 void DD_INIT_IIC(void)
{
	

		PIOA->PIO_WPMR = 0x50494F00;
		//PIOA->PIO_PUER = 0x00000300;
		PIOA->PIO_PDR =	 0x00060000;
	//	PIOA->PIO_PUER = 0x00040000;
	//	PIOA->PIO_PUDR = 0x00060000;
		
		PIOA->PIO_ABSR = 0x00090000;
		twi_master_setup(TWI0, &opt);
		
			
	
}

void DD_READ_IIC(uint8_t iic_address, uint16_t* data)
{
		twi_packet_t iic_packet = {
			.chip         = DIPOT_BUS_ADDR1,
			.buffer       = NULL, // transfer data source buffer
			.length       = 4   // transfer data size (bytes)
		};

			iic_packet.buffer =  (void *)data;
			
			twi_master_read(TWI_EXAMPLE, &iic_packet)	;
	;
}

static void dd_write_data_on_iicbus(uint8_t iic_address, uint16_t data,uint8_t length)
{
	 uint8_t test_pattern_inc[] =
	{
		0x1C,
		0x02,
		0x04,
		0x00
	};
	twi_packet_t iic_packet = {
	//.chip         = DIPOT_BUS_ADDR1,
	.chip         = iic_address,
	.buffer       = (void *)test_pattern_inc, // transfer data source buffer
	//.length       = 4   // transfer data size (bytes)
	.length       = length   // transfer data size (bytes)
};
		if ( data > 1024)
		{
			return;
			
		}
		else
		{
		if(length == 2)
		{
			test_pattern_inc[3] = (Byte)(data & 0xFF);
			test_pattern_inc[2] = 0x04 | (data >> 8 ) & 0x03;
			
		}
		else if ( length == 1)
		{
			test_pattern_inc[3] = 0;
			test_pattern_inc[2] = (Byte)(data & 0xFF);
					
		}

		iic_packet.buffer =  (void *)test_pattern_inc;
		twi_master_write1(TWI_EXAMPLE, &iic_packet)	;
		}

	
}
void DD_IIC_READ_PROP(uint8_t iic_address, uint16_t* data)
{
			twi_packet_t iic_packet = {
				.chip         = iic_address,
				.buffer       = NULL, // transfer data source buffer
				.length       = 2   // transfer data size (bytes)
			};

			iic_packet.buffer =  (void *)data;
			
			twi_master_read(TWI_EXAMPLE, &iic_packet)	;
}
void DD_IIC_SEND_PROP(uint8_t iic_address, uint8_t data)
{
	
	uint8_t test_pattern_inc[] =
	{
		0x1C,
		0x02,
		0x04,
		0x00
	};
	twi_packet_t iic_packet = {
		//.chip         = DIPOT_BUS_ADDR1,
		.chip         = iic_address,
		.buffer       = (void *)&data, // transfer data source buffer
		.length       = 1   // transfer data size (bytes)
		// .length       = length   // transfer data size (bytes)
	};
	if ( data > 1024)
	{
		return;
		
	}
	else
	{

		

		iic_packet.buffer =  (void *)&data;
		twi_master_write1(TWI_EXAMPLE, &iic_packet)	;
	}
	
}

void DD_IIC_CONSOLE_SEND(uint8_t iic_address, uint8_t* data,uint8_t length)
{
	
	uint8_t test_pattern_inc[] =
	{
		0x1C,
		0x02,
		0x04,
		0x00
	};
	twi_packet_t iic_packet = {
		//.chip         = DIPOT_BUS_ADDR1,
		.chip         = iic_address,
		.buffer       = (void *)data, // transfer data source buffer
		.length       = 1   // transfer data size (bytes)
		// .length       = length   // transfer data size (bytes)
	};

	{

		
		iic_packet.length = length;
		iic_packet.buffer =  (void *)data;
		twi_master_write1(TWI_EXAMPLE, &iic_packet)	;
	}
	
}
void DD_IIC_SET_POT(uint8_t iic_address, uint16_t data)
{
	//DD_WRITE_IIC( iic_address,  *data , sizeof(uint16_t));
	
		 uint8_t test_pattern_inc[] =
		 {
			 0x1C,
			 0x02,
			 0x04,
			 0x00
		 };
		 twi_packet_t iic_packet = {
			 //.chip         = DIPOT_BUS_ADDR1,
			 .chip         = iic_address,
			 .buffer       = (void *)test_pattern_inc, // transfer data source buffer
			 .length       = 4   // transfer data size (bytes)
			// .length       = length   // transfer data size (bytes)
		 };
		 if ( data > 1024)
		 {
			 return;
			 
		 }
		 else
		 {

				 test_pattern_inc[3] = (Byte)(data & 0xFF);
				 test_pattern_inc[2] = 0x04 | (data >> 8 ) & 0x03;
	

			 iic_packet.buffer =  (void *)test_pattern_inc;
			 twi_master_write1(TWI_EXAMPLE, &iic_packet)	;
		 }

}

void DD_IIC_SET_BLOODPUP(uint8_t iic_address, uint8_t* data,uint8_t length)
{
	//DD_WRITE_IIC( iic_address,  *data , sizeof(uint16_t));
	

	twi_packet_t iic_packet = {
		.chip         = iic_address,
		.buffer       = (void *)data, // transfer data source buffer
		.length       = length   // transfer data size (bytes)
	};


		twi_master_write1(TWI_EXAMPLE, &iic_packet)	;


}

void DD_WRITE_IIC(uint8_t iic_address, uint16_t data, uint8_t len)
{
	dd_write_data_on_iicbus(iic_address,data,len);
}

#if 0

uint32_t twi_master_write(Twi *p_twi, twi_packet_t *p_packet)
{
	uint32_t status, cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;

	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}
/*	 uint32_t val=10000;
	while(val > 0)
	{
		val--;
	}*/
//printf("enter\n");
	/* Set write mode, slave address and 3 internal address byte lengths */
	//p_twi->TWI_MMR = 0;
	//p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip);
	//p_twi->TWI_MMR = 0;
	//p_twi->TWI_MMR = 0x00510200;
p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) |
	((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
	TWI_MMR_IADRSZ_Msk);
	

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send all bytes */
	while (cnt > 0) {
		
		status = p_twi->TWI_SR;
		//printf("%d\n", status);
		if (status & TWI_SR_NACK) {
			//printf("nack\n");
			return TWI_RECEIVE_NACK;
		}

		if (!(status & TWI_SR_TXRDY)) {
	//	printf("continue\n");
			continue;
		}
		//printf("%d\n",*buffer++);
		p_twi->TWI_THR = *buffer++;

		cnt--;
		
		
	}

	while (1) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
		
			return TWI_RECEIVE_NACK;
		}

		if (status & TWI_SR_TXRDY) {
			break;
		}
	}

	p_twi->TWI_CR = TWI_CR_STOP;

printf("exit\n");
	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}

	return TWI_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
uint32_t twi_master_read(Twi *p_twi, twi_packet_t *p_packet)
{
	uint32_t status, cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	
	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	/* Set read mode, slave address and 3 internal address byte lengths */
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_MREAD | TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send a START Condition */
	p_twi->TWI_CR = TWI_CR_START;

	while (cnt > 0) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		/* Last byte ? */
		if (cnt == 1) {
			p_twi->TWI_CR = TWI_CR_STOP;
		}

		if (!(status & TWI_SR_RXRDY)) {
			continue;
		}
		*buffer++ = p_twi->TWI_RHR;

		cnt--;
	}

	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}

	p_twi->TWI_SR;

	return TWI_SUCCESS;
}
////////////////////////////////////////////////////////////////
static uint32_t twi_mk_addr(const uint8_t *addr, int len)
{
	uint32_t val;

	if (len == 0)
		return 0;

	val = addr[0];
	if (len > 1) {
		val <<= 8;
		val |= addr[1];
	}
	if (len > 2) {
		val <<= 8;
		val |= addr[2];
	}
	return val;
}
///////////////////////////////////////////////////////////
uint32_t twi_master_init(Twi *p_twi, const twi_options_t *p_opt)
{
	uint32_t status = TWI_SUCCESS;

	/* Disable TWI interrupts */
	p_twi->TWI_IDR = ~0UL;

	/* Dummy read in status register */
	p_twi->TWI_SR;

	/* Reset TWI peripheral */
	twi_reset(p_twi);

	twi_enable_master_mode(p_twi);

	/* Select the speed */
	if (twi_set_speed(p_twi, p_opt->speed, p_opt->master_clk) == FAIL) {
		/* The desired speed setting is rejected */
		status = TWI_INVALID_ARGUMENT;
	}

	if (p_opt->smbus == 1) {
		p_twi->TWI_CR = TWI_CR_QUICK;
	}
//printf("master initialization done\n");
	return status;
	
}
/////////////////////////////////////////////
uint32_t twi_set_speed(Twi *p_twi, uint32_t ul_speed, uint32_t ul_mck)
{
	uint32_t ckdiv = 0;
	uint32_t c_lh_div;

	if (ul_speed > I2C_FAST_MODE_SPEED) {
		return FAIL;
	}

	c_lh_div = ul_mck / (ul_speed * TWI_CLK_DIVIDER) - TWI_CLK_CALC_ARGU;

	/* cldiv must fit in 8 bits, ckdiv must fit in 3 bits */
	while ((c_lh_div > TWI_CLK_DIV_MAX) && (ckdiv < TWI_CLK_DIV_MIN)) {
		/* Increase clock divider */
		ckdiv++;
		/* Divide cldiv value */
		c_lh_div /= TWI_CLK_DIVIDER;
	}

	/* set clock waveform generator register */
	p_twi->TWI_CWGR =
			TWI_CWGR_CLDIV(c_lh_div) | TWI_CWGR_CHDIV(c_lh_div) |
			TWI_CWGR_CKDIV(ckdiv);

	return PASS;
}

///////////////////////////////////////////////////////////////////////
static inline uint32_t twi_master_setup(twi_master_t p_twi,
		twi_master_options_t *p_opt)
{
	p_opt->master_clk = sysclk_get_cpu_hz();
	p_opt->smbus      = 0;
//printf("b\n");
	if (p_twi == TWI0) {
		sysclk_enable_peripheral_clock(ID_TWI0);
	} else if (p_twi == TWI1) {
		sysclk_enable_peripheral_clock(ID_TWI1);
#if SAM4N
	} else if (p_twi == TWI2) {
		sysclk_enable_peripheral_clock(ID_TWI2);
#endif
	} else {
		// Do Nothing
	}

	return (twi_master_init(p_twi, p_opt));
}

#endif