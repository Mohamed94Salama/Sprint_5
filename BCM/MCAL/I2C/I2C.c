/*****************************************************************************
* Module     : I2C(TWI)
* File Name  : I2C.c
* Description: Source file for the I2C(TWI) AVR driver
* Author     : Kariman Karam
* Date       : 25/7/2021
******************************************************************************/

#include "I2C.h"

void TWI_init(void)
{
    /* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */
    TWBR_R = 0x02;
	TWSR_R = 0x00;
	
    /* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
       General Call Recognition: Off */
    TWAR_R = 0b00000010; // my address = 0x01 :)
	
    TWCR_R = (1<<TWEN_B); /* enable TWI */
}

void TWI_start(void)
{
    /* 
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR_R = (1 << TWINT_B) | (1 << TWSTA_B) | (1 << TWEN_B);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(!GET_BIT(TWCR_R, TWINT_B));
}

void TWI_stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR_R = (1 << TWINT_B) | (1 << TWSTO_B) | (1 << TWEN_B);
}

void TWI_write(uint8_t u8Data)
{
    /* Put data On TWI data Register */
    TWDR_R = u8Data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR_R = (1 << TWINT_B) | (1 << TWEN_B);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(!GET_BIT(TWCR_R, TWINT_B));
}

void TWI_readWithACK(uint8_t* pu8Data)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR_R = (1 << TWINT_B) | (1 << TWEN_B) | (1 << TWEA_B);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(!GET_BIT(TWCR_R, TWINT_B));
    /* Read Data */
    *pu8Data = TWDR_R;
}

void TWI_readWithNACK(uint8_t* pu8Data)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR_R = (1 << TWINT_B) | (1 << TWEN_B);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(!GET_BIT(TWCR_R,TWINT_B));
    /* Read Data */
    *pu8Data = TWDR_R;
}

void TWI_getStatus(uint8_t* pu8Data)
{
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    *pu8Data = TWSR_R & 0xF8;
}
