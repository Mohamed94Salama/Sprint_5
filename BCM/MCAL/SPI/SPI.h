/*********************************************************************************
 * Module: SPI
 * File Name: spi.h
 * Description:  Header file for the SPI SVR driver
 * Author: Kariman Karam
  *******************************************************************************/

#ifndef SPI_H_
#define SPI_H_

/*- INCLUDES -----------------------------------------------*/
#include "BIT_MANIPULATE.h"
#include "STD_TYPES.h"


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void SPI_initSlave(void);
void SPI_initMaster(void);
/***********************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize SPT
************************************************************************************************/
void SPI_init(void);

/***********************************************************************************************
* Parameters (in) : u8Data
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send Byte via SPI
************************************************************************************************/
void SPI_sendData(const uint8_t u8Data);

/***********************************************************************************************
* Parameters (in) : None
* Parameters (out): pu8Data
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive Byte via SP
************************************************************************************************/
void SPI_receiveData(uint8_t* pu8Data);

/***********************************************************************************************
* Parameters (in) : pu8String
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send string via SPI
************************************************************************************************/
void SPI_sendString(const uint8_t *Str);

/***********************************************************************************************
* Parameters (in) : None
* Parameters (out): pu8String
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive string via SP
************************************************************************************************/
void SPI_receiveString(uint8_t *Str);

#endif /* SPI_H_ */
