/*****************************************************************************
* Task: Car Project
* File Name: DIO_prog.c
* Description: functions for the dio driver
* Author: TEAM2
* Date: 8/7/2021
******************************************************************************/

/*- INCLUDES -----------------------------------------------*/
#include "BIT_MANIPULATE.h"
#include "STD_TYPES.h"
#include "DIO_init.h"
#include "DIO_cfg.h"
#include "DIO_registers.h"

/*- APIs IMPLEMENTATION-----------------------------------*/

/*************************************************************************************************
* Parameters (in) : None
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize all the DIO groups according to the configuration
*************************************************************************************************/
enuErrorStatus_t DIO_Init(void)
{
	uint8_t u8_i;

	for(u8_i=0; u8_i<DIO_GROUPS_NO; u8_i++)
	{
		//set the pin direction (input, output)
		if(astrDIOConfigParameters[u8_i].enuPinDir == INPUT)
		{
			//if PORTA
			if((astrDIOConfigParameters[u8_i].enuPinNo >= 0) && (astrDIOConfigParameters[u8_i].enuPinNo < 8))
			{
				CLEAR_BIT(DDRA_R, astrDIOConfigParameters[u8_i].enuPinNo);
			}//end if

			//if PORTB
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 8) && (astrDIOConfigParameters[u8_i].enuPinNo < 16))
			{
				CLEAR_BIT(DDRB_R,(astrDIOConfigParameters[u8_i].enuPinNo - 8));
			}//end else if

			//if PORTC
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 16) && (astrDIOConfigParameters[u8_i].enuPinNo < 24))
			{
				CLEAR_BIT(DDRC_R,(astrDIOConfigParameters[u8_i].enuPinNo - 16));
			}//end else if

			//if PORTD
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 24) && (astrDIOConfigParameters[u8_i].enuPinNo < 32))
			{
				CLEAR_BIT(DDRD_R,(astrDIOConfigParameters[u8_i].enuPinNo - 24));
			}//end else if
		}//end if

		else if(astrDIOConfigParameters[u8_i].enuPinDir == OUTPUT)
		{
			if((astrDIOConfigParameters[u8_i].enuPinNo >= 0) && (astrDIOConfigParameters[u8_i].enuPinNo < 8))	//if porta
			{
				SET_BIT(DDRA_R, astrDIOConfigParameters[u8_i].enuPinNo);
			}//end if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 8) && (astrDIOConfigParameters[u8_i].enuPinNo < 16))	//if portb
			{
				SET_BIT(DDRB_R,(astrDIOConfigParameters[u8_i].enuPinNo - 8));
			}//end else if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 16) && (astrDIOConfigParameters[u8_i].enuPinNo < 24))	//if portc
			{
				SET_BIT(DDRC_R,(astrDIOConfigParameters[u8_i].enuPinNo - 16));
			}//end else if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 24) && (astrDIOConfigParameters[u8_i].enuPinNo < 32))	//if portd
			{
				SET_BIT(DDRD_R,(astrDIOConfigParameters[u8_i].enuPinNo - 24));
			}//end else if
		}//end else if

		//enable or disable pull-up resistor
		if(astrDIOConfigParameters[u8_i].enuPinDir == INPUT &&
		   astrDIOConfigParameters[u8_i].enuPullupResEn == NO_CONNECTION)
		{
			if((astrDIOConfigParameters[u8_i].enuPinNo >= 0) && (astrDIOConfigParameters[u8_i].enuPinNo < 8))	//if porta
			{
				CLEAR_BIT(PORTA_R, astrDIOConfigParameters[u8_i].enuPinNo);
			}//end if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 8) && (astrDIOConfigParameters[u8_i].enuPinNo < 16))	//if portb
			{
				CLEAR_BIT(PORTB_R,(astrDIOConfigParameters[u8_i].enuPinNo - 8));
			}//end else if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 16) && (astrDIOConfigParameters[u8_i].enuPinNo < 24))	//if portc
			{
				CLEAR_BIT(PORTC_R,(astrDIOConfigParameters[u8_i].enuPinNo - 16));
			}//end else if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 24) && (astrDIOConfigParameters[u8_i].enuPinNo < 32))	//if portd
			{
				CLEAR_BIT(PORTD_R,(astrDIOConfigParameters[u8_i].enuPinNo - 24));
			}//end else if
		}//end if

		else if(astrDIOConfigParameters[u8_i].enuPinDir == INPUT &&
				astrDIOConfigParameters[u8_i].enuPullupResEn == PULL_UP_ENABLE)
		{
			if((astrDIOConfigParameters[u8_i].enuPinNo >= 0) && (astrDIOConfigParameters[u8_i].enuPinNo < 8))	//if porta
			{
				SET_BIT(PORTA_R, astrDIOConfigParameters[u8_i].enuPinNo);
			}//end if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 8) && (astrDIOConfigParameters[u8_i].enuPinNo < 16))	//if portb
			{
				SET_BIT(PORTB_R,(astrDIOConfigParameters[u8_i].enuPinNo - 8));
			}//end else if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 16) && (astrDIOConfigParameters[u8_i].enuPinNo < 24))	//if portc
			{
				SET_BIT(PORTC_R,(astrDIOConfigParameters[u8_i].enuPinNo - 16));
			}//end else if
			else if((astrDIOConfigParameters[u8_i].enuPinNo >= 24) && (astrDIOConfigParameters[u8_i].enuPinNo < 32))	//if portd
			{
				SET_BIT(PORTD_R,(astrDIOConfigParameters[u8_i].enuPinNo - 24));
			}//end else if
		}//end else if

	}//end for
    return E_OK;
}//end DIO_Init


/*************************************************************************************************
* Parameters (in) : u8_GroupId(index to the pin number), u8_Data(the value to be written to the pin)
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to write a digital value into the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Write(uint8_t u8_GroupId, uint8_t u8_Data)
{
//	if(DIO_Init() == E_ERROR)	return E_ERROR;
//	else{

		if (u8_Data == LOW)
	    {
			if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 0) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 8))	//if porta
			{
				CLEAR_BIT(PORTA_R, astrDIOConfigParameters[u8_GroupId].enuPinNo);
			}//end if
			else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 8) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 16))	//if portb
			{
				CLEAR_BIT(PORTB_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 8));
			}//end else if
			else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 16) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 24))	//if portc
			{
				CLEAR_BIT(PORTC_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 16));
			}//end else if
			else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 24) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 32))	//if portd
			{
				CLEAR_BIT(PORTD_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 24));
			}//end else if
	    }//end if

	    else if(u8_Data == HIGH)
	    {
			if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 0) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 8))	//if porta
			{
				SET_BIT(PORTA_R, astrDIOConfigParameters[u8_GroupId].enuPinNo);
			}//end if
			else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 8) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 16))	//if portb
			{
				SET_BIT(PORTB_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 8));
			}//end else if
			else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 16) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 24))	//if portc
			{
				SET_BIT(PORTC_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 16));
			}//end else if
			else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 24) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 32))	//if portd
			{
				SET_BIT(PORTD_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 24));
			}//end else if
	    }//end else if
		return E_OK;
}//end DIO_write

/*************************************************************************************************
* Parameters (in) : u8_GroupId(index to the pin number), pu8_Data(pointer to store the value of the pin)
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to read a digital value from the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_Read(uint8_t u8_GroupId, uint8_t* pu8_Data)
{
	if(pu8_Data == NULLPTR)		return E_ERROR;
	else
	{
		if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 0) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 8))	//if porta
		{
			*pu8_Data = GET_BIT(PINA_R, astrDIOConfigParameters[u8_GroupId].enuPinNo);
		}//end if
		else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 8) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 16))	//if portb
		{
			*pu8_Data = GET_BIT(PINB_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 8));
		}//end else if
		else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 16) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 24))	//if portc
		{
			*pu8_Data = GET_BIT(PINC_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 16));
		}//end else if
		else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 24) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 32))	//if portd
		{
			*pu8_Data = GET_BIT(PIND_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 24));
		}//end else if

		return E_OK;
	}//end else
}//end DIO_Read


/*************************************************************************************************
* Parameters (in) : u8_GroupId(index to the pin number)
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to toggle the selected DIO pin
*************************************************************************************************/
enuErrorStatus_t DIO_toggle(uint8_t u8_GroupId)
{
	if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 0) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 8))	//if porta
	{
		TOGGLE_BIT(PORTA_R, astrDIOConfigParameters[u8_GroupId].enuPinNo);
	}//end if
	else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 8) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 16))	//if portb
	{
		TOGGLE_BIT(PORTB_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 8));
	}//end else if
	else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 16) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 24))	//if portc
	{
		TOGGLE_BIT(PORTC_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 16));
	}//end else if
	else if((astrDIOConfigParameters[u8_GroupId].enuPinNo >= 24) && (astrDIOConfigParameters[u8_GroupId].enuPinNo < 32))	//if portd
	{
		TOGGLE_BIT(PORTD_R,(astrDIOConfigParameters[u8_GroupId].enuPinNo - 24));
	}//end else if

	return E_OK;
}//end DIO_toggle
