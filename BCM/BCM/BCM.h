 /*****************************************************************************
* Task       : BCM
* File Name  : BCM.h
* Description: header file for the BCM driver
* Author     : TEAM2
* Date       : Aug 26, 2021
******************************************************************************/

#ifndef BCM_BCM_H_
#define BCM_BCM_H_

/*- INCLUDES -----------------------------------------------*/
#include "BIT_MANIPULATE.h"
#include "STD_TYPES.h"
#include "DIO_init.h"

/*- DEFINES ----------------------------------------------*/
#define BCM_TWI_SLAVE_ADDRESS										(0xA0)

typedef void (*pfTaskCompleteNotify_t) (void);

/*- ENUMS --------------------------------------------------*/
typedef enum {
	BCM_UART_CHANNEL,
	BCM_SPI_CHANNEL,
	BCM_I2C_CHANNEL
}enuBCMChannel_t;

typedef enum {
	BCM_TX,
	BCM_RX,
	BCM_TX_RX
}enuBCMMode_t;

typedef enum {
	BCM_ID,
	DEVICE_ID
}enuBCMCommand_t;

/*- STRUCTS -----------------------------------------------*/
typedef struct {
	/* Selecting communication channel (uart, spi, i2c) */
	enuBCMChannel_t enuBCMChannel;
	/* Selecting communication mode (send, receive) */
	enuBCMMode_t    enuBCMMode;
}strBCMCfg_t;

typedef struct {
	/* Selcting if BCM will be used or not */
	enuBCMCommand_t		   enuBCMCommand;
	/* Selecting data size to be send or received */
	uint16_t 			   u16Size;
	/* Buffer to hold the data */
	uint8_t*			   pu8DataBuffer;
	/* Check sum to check the data is received correctly */
	uint64_t			   u64CheckSum;
	/* Call back function to be called when receive or send all data is complete */
	pfTaskCompleteNotify_t pfTaskCompleteNotify;
	/* Selecting communication channel (uart, spi, i2c) */
	enuBCMChannel_t 	   enuBCMChannel;
	/* Selecting communication mode (send, receive) */
	enuBCMMode_t           enuBCMMode;
}strBCMTaskCfg_t;

/*- FUNCTION PROTOTYPES ----------------------------------*/

/*************************************************************************************************
* Parameters (in) : pstrBCMCfg
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize BCM according to configurations
*************************************************************************************************/
enuErrorStatus_t BCM_Init(strBCMCfg_t *pstrBCMCfg);
/*************************************************************************************************
* Parameters (in) : pstrBCMTaskCfg
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to setup BCM according to configurations
*************************************************************************************************/
enuErrorStatus_t BCM_Setup(strBCMTaskCfg_t *pstrBCMTaskCfg);
/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive all the data
*************************************************************************************************/
enuErrorStatus_t BCM_RxDispatcher(void);
/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send all the data
*************************************************************************************************/
enuErrorStatus_t BCM_TxDispatcher(void);

void init (void);

#endif /* BCM_BCM_H_ */
