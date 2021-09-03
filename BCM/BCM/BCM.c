/*****************************************************************************
* Task       : BCM
* File Name  : BCM.c
* Description: source file for the BCM driver
* Author     : TEAM2
* Date       : Aug 26, 2021
******************************************************************************/

/*- INCLUDES -----------------------------------------------*/
#include "BCM.h"
#include "UART.h"
#include "SPI.h"
#include "I2C.h"

/*- ENUMS --------------------------------------------------*/
typedef enum {
	BCM_STATE_IDLE,
	BCM_STATE_TXCMD,
	BCM_STATE_TXSIZE,
	BCM_STATE_TXDATA,
	BCM_STATE_TXCS,
	BCM_STATE_RXCMD,
	BCM_STATE_RXSIZE,
	BCM_STATE_RXDATA,
	BCM_STATE_RXCS,
	BCM_STATE_FINISHED,
	BCM_STATE_ERROR
}enuBCMStateMachine_t;

/*- STRUCTS -----------------------------------------------*/
typedef struct{
	/* task configuration */
	strBCMTaskCfg_t*	     strBCMTaskCfg;
	/* To count the numbed of bytes sent or received */
	uint16_t			     u16ByteCounter;
	/* State Machine to hold the task state */
	enuBCMStateMachine_t     enuBCMStateMachine;
}strBCMPacket_t;

/*- Static Global Variables ---------------------------------------*/
static strBCMPacket_t gstrBCMPacket;
static uint16_t gu16Buffer=LOW;
static uint8_t gu8Flag=LOW;
static uint8_t gu8BCMInitState = LOW;
static uint64_t gu64LocalCheckSum=LOW;
static uint16_t u16RxSize=LOW;

/*- FUNCTION PROTOTYPES for static functions -----------------------*/

/* private function to TX interrupt handler */
static void BCM_TxHandler(void);
/* private function to RX interrupt handler */
static void BCM_RxHandler(void);
/* private function to send byte */
static void BCM_SendByte(uint16_t u16Data);
/* private function to receive byte */
static void BCM_ReceiveByte(uint16_t* pu16Data);

/*- APIs IMPLEMENTATION-----------------------------------*/

/*************************************************************************************************
* Parameters (in) : pstrBCMCfg
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize BCM according to configurations
*************************************************************************************************/
enuErrorStatus_t BCM_Init(strBCMCfg_t *strBCMCfg)
{
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus;
	/* if the input pointer equals null */
	if(strBCMCfg==NULLPTR)
	{
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		/* check for the channel (uart, spi, i2c) */
		switch(strBCMCfg->enuBCMChannel)
		{
			/* if uart channel is selected */
			case BCM_UART_CHANNEL:
				/* initialize uart */
				UART_Init();
				/* check for the mode (send, receive) */
				/* if send is selected */
				if(strBCMCfg->enuBCMMode == BCM_TX)
				{
					/* enable send interruot */
					UART_TX_Enable_Interrupt();
					/* set call back function */
					UART_TX_SetCallBack(BCM_TxHandler);
				}//end if
				/* if receive is selceted */
				else if(strBCMCfg->enuBCMMode == BCM_RX)
				{
					/* enable receive interruot */
					UART_RX_Enable_Interrupt();
					/* set call back function */
					UART_RX_SetCallBack(BCM_RxHandler);					
				}//end else if
				else
				{
					/* enable send interruot */
					UART_TX_Enable_Interrupt();
					/* set call back function */
					UART_TX_SetCallBack(BCM_TxHandler);	
					/* enable receive interruot */					
					UART_RX_Enable_Interrupt();
					/* set call back function */
					UART_RX_SetCallBack(BCM_RxHandler);
				}//end else
				/* Enable global interrupt */
				sei();
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
			/* if spi channel is selected */
			case BCM_SPI_CHANNEL:
				/* check for the mode (send, receive) */
				switch(strBCMCfg->enuBCMMode)
				{
					/* if send is selected */
					case BCM_TX:
					    /* initialize spi as master */
						SPI_initMaster();
						/* update function status to be E_OK */
						enuErrorStatus = E_OK;
						break;
					/* if receive is selected */
					case BCM_RX:
						/* initialize spi as slave */
						SPI_initSlave();
						/* update function status to be E_OK */
						enuErrorStatus = E_OK;
						break;
					/* if send while receive is selected */
					case BCM_TX_RX:
						/* update function status to be E_OK */
						enuErrorStatus = E_OK;
						break;
					default:
						/* update function status to be E_ERROR */
						enuErrorStatus = E_ERROR;
				}//end switch of mode
				break;
			/* if i2c channel is selected */
			case BCM_I2C_CHANNEL:
				/* initialize i2c */
				TWI_init();
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
			default:
				/* update function status to be E_ERROR */
				enuErrorStatus = E_ERROR;
		}//end switch of channel
	}//end else
	/* update init state to be high */
	gu8BCMInitState = HIGH;
	/* return function status */
	return enuErrorStatus;
}//end BCM_Init

/*************************************************************************************************
* Parameters (in) : pstrBCMTaskCfg
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to setup BCM according to configurations
*************************************************************************************************/
enuErrorStatus_t BCM_Setup(strBCMTaskCfg_t *pstrBCMTaskCfg)
{
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus;
	/* check if the input pointer equals null or BCM not initialized */
	if((pstrBCMTaskCfg==NULLPTR) || (gu8BCMInitState==LOW))
	{
		/* update function status to be E_ERROR */
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		/* update the global task configuration pointer struct with the input pointer struct */
		gstrBCMPacket.strBCMTaskCfg=pstrBCMTaskCfg;
		/* initialize the BCM state machine to be idle */
 		gstrBCMPacket.enuBCMStateMachine = BCM_STATE_IDLE;
		/* initialize the byte's counter to be zero */
		gstrBCMPacket.u16ByteCounter = 0;
		/* check for the mode */
		/* if selected mode is send */
		if(gstrBCMPacket.strBCMTaskCfg->enuBCMMode == BCM_TX)
		{
			/* update global flag to be HIGH */
			gu8Flag = HIGH;
		}//end if
		else 
		{
			/* update global flag to be LOW */
			gu8Flag = LOW;
		}//end else
		/* update function status to be E_OK */
		enuErrorStatus = E_OK;
	}//end else
	/* return function status */
	return enuErrorStatus;
}//end BCM_Setup

/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to send all the data
*************************************************************************************************/
enuErrorStatus_t BCM_TxDispatcher(void)
{
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus= E_ERROR;
	/* */
	static uint8_t u8ChecksumShiftValue = 56;
	/* check if BCM not initialized and the selcted mode is receive */
	if((gu8BCMInitState == LOW) && (gstrBCMPacket.strBCMTaskCfg->enuBCMMode == BCM_RX))
	{
		/* update function status to be E_ERROR */
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		/* check if the global TX flag is set */
		if(gu8Flag == HIGH)
		{
			/* update the global TX flag to be LOW */
			gu8Flag = LOW;
			/* check for the state machine */
			switch(gstrBCMPacket.enuBCMStateMachine)
			{
				/* if state is idle */
				case BCM_STATE_IDLE:
				/* send command (BCM or not) */
				BCM_SendByte(gstrBCMPacket.strBCMTaskCfg->enuBCMCommand);
				/* update state machine to be TX command */
				gstrBCMPacket.enuBCMStateMachine = BCM_STATE_TXCMD;
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
				/* if state is TX command */
				case BCM_STATE_TXCMD:
				/* send data size(first byte of size) */
				BCM_SendByte((gstrBCMPacket.strBCMTaskCfg->u16Size >> 8));
				/* update state machine to be TX size */
				gstrBCMPacket.enuBCMStateMachine = BCM_STATE_TXSIZE;
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
				/* if state is TX size */
				case BCM_STATE_TXSIZE:
				/* send data size(second byte of size) */
				BCM_SendByte(((gstrBCMPacket.strBCMTaskCfg->u16Size) & 0xFF));
				/* update state machine to be TX Data */
				gstrBCMPacket.enuBCMStateMachine = BCM_STATE_TXDATA;
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
				/* if state is TX Data */
				case BCM_STATE_TXDATA:
				/* send data */
				BCM_SendByte(gstrBCMPacket.strBCMTaskCfg->pu8DataBuffer[gstrBCMPacket.u16ByteCounter]);
				/* check if byte's counter is less than the buffer size */
				if (gstrBCMPacket.u16ByteCounter < gstrBCMPacket.strBCMTaskCfg->u16Size)
				{
					/* increase the byte's counter by 1 */
					gstrBCMPacket.u16ByteCounter++;
				}//end if
				else
				{
					/* set the byte's counter to be zero */
					gstrBCMPacket.u16ByteCounter = 0;
					/* update state machine to be TX complete */
					gstrBCMPacket.enuBCMStateMachine= BCM_STATE_TXCS;
				}//end else
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
				/* if state is TX complete */
				case BCM_STATE_TXCS:
				/* send check sum */
				BCM_SendByte(((gstrBCMPacket.strBCMTaskCfg->u64CheckSum) >> u8ChecksumShiftValue) & 0xFF);
				u8ChecksumShiftValue -= 8;
				/* check if byte's counter is less than 7 */
				if (gstrBCMPacket.u16ByteCounter < 7)
				{
					/* increase the byte's counter by 1 */
					gstrBCMPacket.u16ByteCounter++;
				}//end if
				else
				{
					/* set the byte's counter to be zero */
					gstrBCMPacket.u16ByteCounter = 0;
					/* call the notify function after send is complete */
					gstrBCMPacket.strBCMTaskCfg->pfTaskCompleteNotify();
					/* update state machine to be finished */
					gstrBCMPacket.enuBCMStateMachine = BCM_STATE_FINISHED;
				}//end else
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
				/* if state is finished */
				case BCM_STATE_FINISHED:
				/* update function status to be E_OK */
				enuErrorStatus = E_OK;
				break;
				default:
				/* update function status to be E_ERROR */
				enuErrorStatus = E_ERROR;
			}//end switch for state machine
		}//end if
		else
		{
			/* do nothing */
		}//end else
	}
	/* return function status */
	return enuErrorStatus;
}//end BCM_TxDispatcher

/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to receive all the data
*************************************************************************************************/
enuErrorStatus_t BCM_RxDispatcher(void)
{
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus= E_ERROR;
	/* check if BCM not initialized and the selcted mode is send */
	if((gu8BCMInitState == LOW) && (gstrBCMPacket.strBCMTaskCfg->enuBCMMode == BCM_TX))
	{
		/* update function status to be E_ERROR */
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		/* check if the global RX flag is set */
		if(gu8Flag == HIGH)
		{
			/* check for the state machine */
			switch(gstrBCMPacket.enuBCMStateMachine)
			{
				/* if state is idle */
				case BCM_STATE_IDLE:
					/* ISR will change state to BCM_STATE_RXBYTE */
					/* update function status to be E_OK */
					enuErrorStatus = E_OK;
					break;
				/* if state is RX command */
				case BCM_STATE_RXCMD:
					/* update the global BCM command */
					gstrBCMPacket.strBCMTaskCfg->enuBCMCommand = (uint8_t)(gu16Buffer - '0');
					/* if command is BCM_ID */
					if (gstrBCMPacket.strBCMTaskCfg->enuBCMCommand == BCM_ID)
					{
						/* update state machine to be RX size */
						gstrBCMPacket.enuBCMStateMachine = BCM_STATE_RXSIZE;
						/* update function status to be E_OK */
						enuErrorStatus = E_OK;
					}//end if
					else
					{
						/* update state machine to be ERROR */
						gstrBCMPacket.enuBCMStateMachine = BCM_STATE_ERROR;
						/* update function status to be E_ERROR */
						enuErrorStatus = E_ERROR;
					}//end else
					break;
				/* if state is RX size */
				case BCM_STATE_RXSIZE:
					/* if byte's counter is zero */
					if (gstrBCMPacket.u16ByteCounter == 0)
					{
						/* update RX size */
						u16RxSize = (uint8_t)(gu16Buffer - '0');
						/* increase the byte's counter by 1 */
						gstrBCMPacket.u16ByteCounter++;
					}//end if
					else
					{
						/* shift the size left by 8 */
						u16RxSize <<=8;
						/* update RX size */
						u16RxSize |= (uint8_t)(gu16Buffer - '0');
						/* update byte's counter to be zero */
						gstrBCMPacket.u16ByteCounter=0;
						/* if Rx size is less than data size */
						if (u16RxSize<=gstrBCMPacket.strBCMTaskCfg->u16Size)
						{
							/* update state machine to be RX Data */
							gstrBCMPacket.enuBCMStateMachine= BCM_STATE_RXDATA;
							/* update function status to be E_OK */
							enuErrorStatus = E_OK;
						}//end if
						else
						{
							/* update state machine to be ERROR */
							gstrBCMPacket.enuBCMStateMachine= BCM_STATE_ERROR;
							/* update function status to be E_ERROR */
							enuErrorStatus = E_ERROR;
						}//end else
					}
					break;
				/* if state is RX Data */
				case BCM_STATE_RXDATA:
					/* if byte's counter less than RX size */
					if (gstrBCMPacket.u16ByteCounter < (u16RxSize - 1))
					{
						/* update the buffer with the received data */
						gstrBCMPacket.strBCMTaskCfg->pu8DataBuffer[gstrBCMPacket.u16ByteCounter]=(uint8_t)gu16Buffer;
						/* increase the byte's counter by 1 */
						gstrBCMPacket.u16ByteCounter++;
						/* update the check sum */
						gu64LocalCheckSum+=gu16Buffer-'0';
					}//end if
					else
					{
						/* update the buffer with the received data */
						gstrBCMPacket.strBCMTaskCfg->pu8DataBuffer[gstrBCMPacket.u16ByteCounter]=(uint8_t)gu16Buffer;
						/* update the byte's counter with zero */
						gstrBCMPacket.u16ByteCounter = 0;
						/* update the check sum */
						gu64LocalCheckSum+=gu16Buffer-'0';
						/* update state machine to be RX complete */
						gstrBCMPacket.enuBCMStateMachine= BCM_STATE_RXCS;
						/* update the check sum with zero */
						gstrBCMPacket.strBCMTaskCfg->u64CheckSum=0;
					}//end else
					/* update the function status to be E_OK */
					enuErrorStatus = E_OK;
					break;
                /* if state is RX complete */
				case BCM_STATE_RXCS:
					/* if byte's counter less than 7 */
					if (gstrBCMPacket.u16ByteCounter < 7)
					{
 						gstrBCMPacket.strBCMTaskCfg->u64CheckSum<<=8;
 						gstrBCMPacket.strBCMTaskCfg->u64CheckSum|=(uint8_t)gu16Buffer-'0';
						/* increase the byte's counter by 1 */
						gstrBCMPacket.u16ByteCounter++;
					}//end if
					else
					{
						
 						gstrBCMPacket.strBCMTaskCfg->u64CheckSum<<=8;
 						gstrBCMPacket.strBCMTaskCfg->u64CheckSum|=(uint8_t)gu16Buffer-'0';
						/* update the byte's counter with zero */
						gstrBCMPacket.u16ByteCounter=0;
						/* if check sum equals the received check sum */
						if (gu64LocalCheckSum == gstrBCMPacket.strBCMTaskCfg->u64CheckSum)
						{
							/* call the notify function */
							gstrBCMPacket.strBCMTaskCfg->pfTaskCompleteNotify();
							/* update state machine to be finished */
							gstrBCMPacket.enuBCMStateMachine= BCM_STATE_FINISHED;
							/* update the function status to be E_OK */
							enuErrorStatus = E_OK;
						}//end if
						else
						{
							/* update state machine to be ERROR */
							gstrBCMPacket.enuBCMStateMachine= BCM_STATE_ERROR;
							/* update the function status to be E_ERROR */
							enuErrorStatus = E_ERROR;
						}//end else
					}//end else
					break;
                /* if state is finished */
				case BCM_STATE_FINISHED:
					/* update the function status to be E_OK */
					enuErrorStatus = E_OK;
					break;
               
				default:
					/* update the function status to be E_ERROR */
					enuErrorStatus = E_ERROR;
			}
			/* update the global flag to be LOW */
			gu8Flag=LOW;
		}//end if
		else
		{
			/* update the function status to be E_ERROR */
			enuErrorStatus = E_ERROR;
		}//end else
	}
	/* return the function status */
	return enuErrorStatus;
}//end BCM_RxDispatcher

/* private function to TX interrupt handler */
static void BCM_TxHandler(void)
{
	/* update the TX global flag to be HIGH */
	gu8Flag = HIGH;
}//end BCM_TxHandler

/* private function to RX interrupt handler */
static void BCM_RxHandler(void)
{
	/* if state machine is idle */
	if(gstrBCMPacket.enuBCMStateMachine == BCM_STATE_IDLE)
	{
		/* update state machine to be RX command */
		gstrBCMPacket.enuBCMStateMachine = BCM_STATE_RXCMD;		
	}//end if
	else
	{
		/* Do nothing */
	}//end else
	/* receive byte */
	BCM_ReceiveByte(&gu16Buffer);
	/* update the RX global flag to be HIGH */
	gu8Flag=HIGH;
}//end BCM_RxHandler

/* private function to send byte */
static void BCM_SendByte(uint16_t u16Data)
{
	/* if selected channel is uart */
	if(gstrBCMPacket.strBCMTaskCfg->enuBCMChannel == BCM_UART_CHANNEL)
	{
		/* send byte via uart */
		UART_SendDataNoBLOCK(u16Data);
	}//end if
	/* if selected channel is spi */
	else if(gstrBCMPacket.strBCMTaskCfg->enuBCMChannel == BCM_SPI_CHANNEL)
	{
		/* send byte via spi */
		SPI_sendData((uint8_t)u16Data);
	}//end else if
	/* if selected channel is i2c */
	else
	{
		/* send start bit */
		TWI_start();
		/* send slave address */
		TWI_write(BCM_TWI_SLAVE_ADDRESS);
		/* send byte via spi */
		TWI_write((uint8_t)u16Data);
		/* send stop bit */
		TWI_stop();
	}//end else
}//end BCM_SendByte

/* private function to receive byte */
static void BCM_ReceiveByte(uint16_t* pu16Data)
{
	/* if selected channel is uart */
	if(gstrBCMPacket.strBCMTaskCfg->enuBCMChannel == BCM_UART_CHANNEL)
	{
		/* send byte via uart */
		UART_RecieveDataNoBLOCK(pu16Data);
	}//end if
	/* if selected channel is spi */
	else if(gstrBCMPacket.strBCMTaskCfg->enuBCMChannel == BCM_SPI_CHANNEL)
	{
		/* send byte via spi */
		SPI_receiveData((uint8_t*)pu16Data);
	}//end else if
	/* if selected channel is i2c */
	else
	{
		/* send start bit */
		TWI_start();
		/* send slave address */
		TWI_write(BCM_TWI_SLAVE_ADDRESS | (1 << 0));
		/* receive byte via spi */
		TWI_readWithACK((uint8_t*)pu16Data);
		/* send stop bit */
		TWI_stop();
	}//end else
}//end BCM_ReceiveByte