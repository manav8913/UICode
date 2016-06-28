/*
 * DD_IIC.h
 *
 * Created: 9/19/2014 3:04:01 PM
 *  Author: user
 */ 


#ifndef DD_IIC_H_
#define DD_IIC_H_

typedef enum
{
	DD_CS_POT,
	DD_COND_POT,
	DD_POT_MAX
	};

typedef struct
{
	uint8_t command;
	uint8_t len;
	bool pending;
	
}IIC_PROPO_CommandType;
typedef struct
{
	uint32_t command;
	uint8_t len;
	bool pending;
	
}IIC_BP_CommandType;

void DD_INIT_IIC(void);
void DD_READ_IIC(uint8_t iic_address, uint16_t* data);
void DD_WRITE_IIC(uint8_t iic_address, uint16_t data, uint8_t len);


#endif /* DD_IIC_H_ */