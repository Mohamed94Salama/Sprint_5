/*****************************************************************************
* Task: Autonomous Car
* File Name: SW_ICU.c
* Description: File for Software ICU driver functions
* Author: Amr Mohamed
* Date: 12/8/2021
******************************************************************************/

/*Includes -----------------------------------------------------------------*/
#include "SW_ICU.h"

/*Preprocessor Macros ---------------------------------------------------*/
#define SW_ICU_THOUSAND_MULTIPLIER									(1000)
#define SW_ICU_TWO_DIVISOR												(2)
#define SW_ICU_TIMER_RESET_VALUE										(0)

/*API Implementation ---------------------------------------------------*/

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to initialize the Software ICU
************************************************************************************/
enuErrorStatus_t SW_ICU_Init(uint8_t u8GroupId)
{
	//initialize the software ICU
   EXTINT_Init(&astrEXTINTConfigParameters[astrSWICUConfigParameters[u8GroupId].u8SWICUEXTINTGroupId]);
   //initialize timer1 
   Timer1_Init(TIMER1_FASTPWM_ICR_TOP_MODE,TIMER1_SCALER_8);
   //reset timer 1 overflow counter
   Timer1_ResetOVFCount();
   //enable timer1 overflow interrupt
   Timer_OVInterruptEnable(astrSWICUConfigParameters[u8GroupId].u8SWICUTimerGroupId);
   //update the ICR register value to apply the new frequency
   ICR1_R = (astrSWICUConfigParameters[u8GroupId].u8TimerFreqKHz * SW_ICU_THOUSAND_MULTIPLIER) / SW_ICU_TWO_DIVISOR;
   //reset timer register
   TCNT1_R = SW_ICU_TIMER_RESET_VALUE;
   //return OK status
   return E_OK;
}

/************************************************************************************
* Parameters (in) : u8GroupId, enuSWICUTriggerMode
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to set the detection edge of the SW_ICU
************************************************************************************/
enuErrorStatus_t SW_ICU_SetEdge(uint8_t u8GroupId, enuSWICUTriggerMode_t enuSWICUTriggerMode)
{
   //Set external interrupt channel to detect the specified edge
   EXTINT_SetSenseMode(astrSWICUConfigParameters[u8GroupId].u8SWICUEXTINTGroupId, enuSWICUTriggerMode);
   //return OK status
   return E_OK;
}

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): *u32Buffer, enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to get the SW_ICU register reading
************************************************************************************/
enuErrorStatus_t SW_ICU_GetReading(uint8_t u8GroupId, uint32_t *u32Buffer)
{
   //check if sent pointer points to a valid location
   if (u32Buffer == NULLPTR)
   {
      //return a NOK status
      return E_ERROR;
   }
   //get a snapshot of the timer register
   uint16_t Buffer = TCNT1_R;
   uint32_t overflows = 0;
   //get the overflow counter value
   Timer1_GetOVFCount(&overflows);
   //calculate actual timer value based on overflows number
   *u32Buffer = Buffer + (overflows * astrSWICUConfigParameters[u8GroupId].u16TimerMax);
   //return OK status
   return E_OK;   
}

/************************************************************************************
* Parameters (in) : u8GroupId
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to reset the SW_ICU logic
************************************************************************************/
enuErrorStatus_t SW_ICU_Reset_Reading(uint8_t u8GroupId)
{
   //reset timer 1 overflow counter
   Timer1_ResetOVFCount();
   //return OK status
   return E_OK;
}

/************************************************************************************
* Parameters (in) : u8GroupId, void(*pfSWICUCallback)(void)
* Parameters (out): enuErrorStatus_t
* Return value    : 1=E_OK - 0=E_ERROR
* Description     : A function to set callback function to be called in case of ICU event
************************************************************************************/
enuErrorStatus_t SW_ICU_SetCallback(uint8_t u8GroupId, void(*pfSWICUCallback)(void))
{
   //check if sent pointer points to a valid location
   if (pfSWICUCallback == NULLPTR)
   {
      //return a NOK status
      return E_ERROR;
   }
   //set the external interrupt callback to call the sent function
   EXTINT_SetCallBack(astrSWICUConfigParameters[u8GroupId].u8SWICUEXTINTGroupId, pfSWICUCallback);
   //return OK status
   return E_OK;
}
