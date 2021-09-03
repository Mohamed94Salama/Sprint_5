/*****************************************************************************
* Task: Car Project
* File Name: DIO_registers.h
* Description: header file for the dio memory mapped registers
* Author: TEAM2
* Date: 8/7/2021
******************************************************************************/

#ifndef MCAL_DIO_DIO_REGISTERS_H_
#define MCAL_DIO_DIO_REGISTERS_H_

/*- DEFINES -----------------------------------------------*/

/*- PORTA Registers----------------------------------------*/
#define PORTA_R		(*(volatile unsigned char*)0x3B)
#define DDRA_R		(*(volatile unsigned char*)0x3A)
#define PINA_R		(*(volatile unsigned char*)0x39)

/*- PORTB Registers----------------------------------------*/
#define PORTB_R		(*(volatile unsigned char*)0x38)
#define DDRB_R		(*(volatile unsigned char*)0x37)
#define PINB_R		(*(volatile unsigned char*)0x36)

/*- PORTC Registers----------------------------------------*/
#define PORTC_R		(*(volatile unsigned char*)0x35)
#define DDRC_R		(*(volatile unsigned char*)0x34)
#define PINC_R  	(*(volatile unsigned char*)0x33)

/*- PORTD Registers----------------------------------------*/
#define PORTD_R		(*(volatile unsigned char*)0x32)
#define DDRD_R		(*(volatile unsigned char*)0x31)
#define PIND_R		(*(volatile unsigned char*)0x30)


#endif /* MCAL_DIO_DIO_REGISTERS_H_ */
