/*****************************************************************************
* Task       : External Interrupt driver
* File Name  : EXT_INT.c
* Description: Source file for External Interrupt driver
* Author     : Kariman Karam
* Date       : 3/8/2021
******************************************************************************/

/*- INCLUDES --------------------------------------------------*/
#include "EXTINT.h"

/*- PREPROCESSOR MACROS --------------------------------------------------*/
#define EXT_INT_MCUCR_R_BIT0											(0)
#define EXT_INT_MCUCR_R_BIT1											(1)
#define EXT_INT_MCUCR_R_BIT2											(2)
#define EXT_INT_MCUCR_R_BIT3											(3)

/*- GLOBAL VARIABLES --------------------------------------------------*/
void(*pEXTINT0CallBack)(void);
void(*pEXTINT1CallBack)(void);
void(*pEXTINT2CallBack)(void);


/*- APIs IMPLEMENTATION-----------------------------------*/

/***********************************************************************************************
* Parameters (in) : strEXTINTConfigType
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to initialize External Interrupt
************************************************************************************************/
enuErrorStatus_t EXTINT_Init(strEXTINTConfigType_t* strEXTINTConfigType)
{
	enuErrorStatus_t enuStatus = E_ERROR;

	if(strEXTINTConfigType->enuEXTINTNum == EXTINT_INT0)
   {
		switch(strEXTINTConfigType->enuEXTINTSenseControl)
		{
			case EXT_LOW_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			case EXT_RAISING_MODE :
			 SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			 SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			case EXT_FAILLING_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			case EXT_CHANGE_MODE :
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			default:
				break;
		}
		//Assign the address of the callback function into a temporary variable
		pEXTINT0CallBack = strEXTINTConfigType->pfEXTINTCallBack;
		//Enable peripheral interrupt enable
		SET_BIT(GICR_R, INT0_B);
   }
   else if(strEXTINTConfigType->enuEXTINTNum == EXTINT_INT1)
   {
		switch(strEXTINTConfigType->enuEXTINTSenseControl)
		{
			case EXT_LOW_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
				CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			case EXT_RAISING_MODE :
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			case EXT_FAILLING_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			case EXT_CHANGE_MODE :
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			default:
				break;
		}
		//Assign the address of the callback function into a temporary variable
		pEXTINT1CallBack = strEXTINTConfigType->pfEXTINTCallBack;
		//Enable peripheral interrupt enable
		SET_BIT(GICR_R, INT1_B);
   }
   else if (strEXTINTConfigType->enuEXTINTNum == EXTINT_INT2)
   {
		switch(strEXTINTConfigType->enuEXTINTSenseControl)
		{
			case EXT_RAISING_MODE :
				SET_BIT(MCUCSR_R,ISC2_B);
				break;
			case EXT_FAILLING_MODE :
				CLEAR_BIT(MCUCSR_R,ISC2_B);
				break;
			default:
				break;
		}
		//Assign the address of the callback function into a temporary variable
		pEXTINT2CallBack = strEXTINTConfigType->pfEXTINTCallBack;
		//Enable peripheral interrupt enable
		SET_BIT(GICR_R, INT2_B);
   }
   else
   {
   }
   //Enable GIE
	SET_BIT(SREG_R, SREG_I_B);
	enuStatus = E_OK;
	return enuStatus;
}

/***********************************************************************************************
* Parameters (in) : u8GroupId, enuEXTINTSenseControl
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to set the sense mode of the External interrupt
************************************************************************************************/
enuErrorStatus_t EXTINT_SetSenseMode(uint8_t u8GroupId, enuEXTINTSenseControl_t enuEXTINTSenseControl)
{
	enuErrorStatus_t enuStatus = E_ERROR;

	if(astrEXTINTConfigParameters[u8GroupId].enuEXTINTNum == EXTINT_INT0)
   {
		switch(enuEXTINTSenseControl)
		{
			case EXT_LOW_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			case EXT_RAISING_MODE :
			 SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			 SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			case EXT_FAILLING_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			case EXT_CHANGE_MODE :
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT0);
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT1);
			  break;
			default:
				break;
		}
   }
   else if(astrEXTINTConfigParameters[u8GroupId].enuEXTINTNum == EXTINT_INT1)
   {
		switch(enuEXTINTSenseControl)
		{
			case EXT_LOW_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
				CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			case EXT_RAISING_MODE :
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			case EXT_FAILLING_MODE :
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			case EXT_CHANGE_MODE :
			  SET_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT2);
			  CLEAR_BIT(MCUCR_R,EXT_INT_MCUCR_R_BIT3);
			  break;
			default:
				break;
		}
   }
   else if (astrEXTINTConfigParameters[u8GroupId].enuEXTINTNum == EXTINT_INT2)
   {
		switch(enuEXTINTSenseControl)
		{
			case EXT_RAISING_MODE :
				SET_BIT(MCUCSR_R,ISC2_B);
				break;
			case EXT_FAILLING_MODE :
				CLEAR_BIT(MCUCSR_R,ISC2_B);
				break;
			default:
				break;
		}
   }
   else
   {
   }
	enuStatus = E_OK;
   return enuStatus;
}

/***********************************************************************************************
* Parameters (in) : u8GroupId, pfEXTINTCallBack
* Parameters (out): None
* Return Value    : enuErrorStatus_t - For error handling if the user input an invalid parameter
* Description     : A function to set the callback function of the External interrupt
************************************************************************************************/
enuErrorStatus_t EXTINT_SetCallBack(uint8_t u8GroupId, void(*pfEXTINTCallBack)(void))
{
	enuErrorStatus_t enuStatus = E_ERROR;

	if(astrEXTINTConfigParameters[u8GroupId].enuEXTINTNum == EXTINT_INT0)
	{
		pEXTINT0CallBack = pfEXTINTCallBack;
	}
	else if(astrEXTINTConfigParameters[u8GroupId].enuEXTINTNum == EXTINT_INT1)
	{
		pEXTINT1CallBack = pfEXTINTCallBack;
	}
	else if(astrEXTINTConfigParameters[u8GroupId].enuEXTINTNum == EXTINT_INT2)
	{
		pEXTINT2CallBack = pfEXTINTCallBack;
	}
	else
	{
	}
	enuStatus = E_OK;
   return enuStatus;
}


/* ISR to handle the External Interrupt Request 0 */
void __vector_1(void) __attribute__ ((signal,used));
/* ISR to handle the External Interrupt Request 1 */
void __vector_2(void) __attribute__ ((signal,used));
/* ISR to handle the External Interrupt Request 2 */
void __vector_3(void) __attribute__ ((signal,used));

/* ISR to handle the External Interrupt Request 0 */
void __vector_1(void)
{
	pEXTINT0CallBack();
}

/* ISR to handle the External Interrupt Request 1 */
void __vector_2(void)
{
	pEXTINT1CallBack();
}

/* ISR to handle the External Interrupt Request 2 */
void __vector_3(void)
{
	pEXTINT2CallBack();
}
