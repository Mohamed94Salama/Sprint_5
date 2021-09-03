/*****************************************************************************
* Module     : I2C(TWI)
* File Name  : I2C.c
* Description: Header file for the I2C(TWI) AVR driver
* Author     : Kariman Karam
* Date       : 25/7/2021
******************************************************************************/


#ifndef I2C_H_
#define I2C_H_

/*- INCLUDES -----------------------------------------------*/
#include "BIT_MANIPULATE.h"
#include "STD_TYPES.h"
#include "I2C_registers.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start 
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t u8Data);
void TWI_readWithACK(uint8_t* pu8Data); //read with send Ack
void TWI_readWithNACK(uint8_t* pu8Data); //read without send Ack
void TWI_getStatus(uint8_t* pu8Data);


#endif /* I2C_H_ */
