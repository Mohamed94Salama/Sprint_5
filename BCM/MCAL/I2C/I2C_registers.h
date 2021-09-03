/*****************************************************************************
* Module     : I2C(TWI)
* File Name  : I2C_registers.h
* Description: Header file for the I2C(TWI) registers
* Author     : Kariman Karam
* Date       : 25/7/2021
******************************************************************************/

#ifndef MCAL_I2C_I2C_REGISTERS_H_
#define MCAL_I2C_I2C_REGISTERS_H_

/* TWI Control Register – TWCR */
#define	TWCR_R	(*(volatile unsigned char*)0x56)
#define TWIE_B	0
#define TWEN_B	2
#define TWWC_B	3
#define TWSTO_B	4
#define TWSTA_B	5
#define TWEA_B	6
#define TWINT_B	7

/* TWI Data Register – TWDR */
#define	TWDR_R		(*(volatile unsigned char*)0x23)
#define TWD0_B	0
#define TWD1_B	1
#define TWD2_B	2
#define TWD3_B	3
#define TWD4_B	4
#define TWD5_B	5
#define TWD6_B	6
#define TWD7_B	7

/* TWI (Slave) Address Register – TWAR */
#define	TWAR_R	(*(volatile unsigned char*)0x22)
#define TWGCE_B	0
#define TWA0_B	1
#define TWA1_B	2
#define TWA2_B	3
#define TWA3_B	4
#define TWA4_B	5
#define TWA5_B	6
#define TWA6_B	7

/* TWI Status Register – TWSR */
#define	TWSR_R		(*(volatile unsigned char*)0x21)
#define TWPS0_B		0
#define TWPS1_B	1
#define TWS3_B	3
#define TWS4_B	4
#define TWS5_B	5
#define TWS6_B	6
#define TWS7_B	7

/* TWI Bit Rate Register – TWBR */
#define	TWBR_R		(*(volatile unsigned char*)0x20)

#endif /* MCAL_I2C_I2C_REGISTERS_H_ */
